/////////////////////////////////////////////////////////////////////////////
// Name:        object.cpp
// Purpose:     wxObject implementation
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:   	wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "object.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/hash.h"
#ifdef USE_STORABLE_CLASSES
#include "wx/objstrm.h"
#include "wx/serbase.h"
#endif
#endif

#include <string.h>
#include <assert.h>

#if (WXDEBUG && USE_MEMORY_TRACING) || USE_DEBUG_CONTEXT
#include "wx/memory.h"
#endif

#if WXDEBUG || USE_DEBUG_CONTEXT
  // for wxObject::Dump
  #include <iostream.h>
#endif

#if !USE_SHARED_LIBRARY
wxClassInfo wxObject::classwxObject((char *) "wxObject", (char *) NULL, (char *) NULL, (int ) sizeof(wxObject), (wxObjectConstructorFn) NULL);
wxClassInfo *wxClassInfo::first = (wxClassInfo *) NULL;
wxHashTable wxClassInfo::classTable(wxKEY_STRING);
#endif

/*
 * wxWindows root object.
 */

wxObject::wxObject(void)
{
  m_refData = (wxObjectRefData *) NULL;
#ifdef USE_STORABLE_CLASSES
  m_serialObj = (wxObject_Serialize *)NULL;
#endif
}

wxObject::~wxObject(void)
{
	UnRef();
#ifdef USE_STORABLE_CLASSES
	if (m_serialObj)
	  delete m_serialObj;
#endif
}

/*
 * Is this object a kind of (a subclass of) 'info'?
 * E.g. is wxWindow a kind of wxObject?
 * Go from this class to superclass, taking into account
 * two possible base classes.
 */
 
bool wxObject::IsKindOf(wxClassInfo *info)
{
  wxClassInfo *thisInfo = GetClassInfo();
  if (thisInfo)
    return thisInfo->IsKindOf(info);
  else
    return FALSE;
}

#if WXDEBUG || USE_DEBUG_CONTEXT
void wxObject::Dump(ostream& str)
{
  if (GetClassInfo() && GetClassInfo()->GetClassName())
    str << GetClassInfo()->GetClassName();
  else
    str << "unknown object class";
}
#endif

#if WXDEBUG && USE_MEMORY_TRACING

#ifdef new
#undef new
#endif

void * wxObject::operator new (size_t size, char * fileName, int lineNum)
{
  return wxDebugAlloc(size, fileName, lineNum, TRUE);
}

void wxObject::operator delete (void * buf)
{
  wxDebugFree(buf);
}

// Cause problems for VC++ - crashes
#ifndef _MSC_VER
void * wxObject::operator new[] (size_t size, char * fileName, int lineNum)
{
  return wxDebugAlloc(size, fileName, lineNum, TRUE, TRUE);
}

void wxObject::operator delete[] (void * buf)
{
  wxDebugFree(buf, TRUE);
}
#endif

#endif

/*
 * Class info: provides run-time class type information.
 */

wxClassInfo::wxClassInfo(char *cName, char *baseName1, char *baseName2, int sz, wxObjectConstructorFn constr)
{
  className = cName;
  baseClassName1 = baseName1;
  baseClassName2 = baseName2;

  objectSize = sz;
  objectConstructor = constr;
  
  next = first;
  first = this;

  baseInfo1 = (wxClassInfo *) NULL;
  baseInfo2 = (wxClassInfo *) NULL;
}

wxObject *wxClassInfo::CreateObject(void)
{
  if (objectConstructor)
    return (wxObject *)(*objectConstructor)();
  else
    return (wxObject *) NULL;
}

wxClassInfo *wxClassInfo::FindClass(char *c)
{
  wxClassInfo *p = first;
  while (p)
  {
    if (p && p->GetClassName() && strcmp(p->GetClassName(), c) == 0)
      return p;
    p = p->next;
  }
  return (wxClassInfo *) NULL;
}

// Climb upwards through inheritance hierarchy.
// Dual inheritance is catered for.
bool wxClassInfo::IsKindOf(wxClassInfo *info)
{
  if (info == NULL)
    return FALSE;

  // For some reason, when making/using a DLL, static data has to be included
  // in both the DLL and the application. This can lead to duplicate
  // wxClassInfo objects, so we have to test the name instead of the pointers.
#if WXMAKINGDLL
  if (GetClassName() && info->GetClassName() && (strcmp(GetClassName(), info->GetClassName()) == 0))
    return TRUE;
#else
  if (this == info)
    return TRUE;
#endif

  if (baseInfo1)
    if (baseInfo1->IsKindOf(info))
      return TRUE;

  if (baseInfo2)
    return baseInfo2->IsKindOf(info);

  return FALSE;
}

// Set pointers to base class(es) to speed up IsKindOf
void wxClassInfo::InitializeClasses(void)
{
  // Index all class infos by their class name
  wxClassInfo *info = first;
  while (info)
  {
    if (info->className)
      classTable.Put(info->className, (wxObject *)info);
    info = info->next;
  }

  // Set base pointers for each wxClassInfo
  info = first;
  while (info)
  {
    if (info->GetBaseClassName1())
      info->baseInfo1 = (wxClassInfo *)classTable.Get(info->GetBaseClassName1());
    if (info->GetBaseClassName2())
      info->baseInfo2 = (wxClassInfo *)classTable.Get(info->GetBaseClassName2());
    info = info->next;
  }
  first = NULL;
}

wxObject *wxCreateDynamicObject(char *name)
{
  wxClassInfo *info;

  info = (wxClassInfo *)wxClassInfo::classTable.Get(name);
  if (!info)
    return (wxObject *)NULL;

  return info->CreateObject();
}

#ifdef USE_STORABLE_CLASSES

#include "wx/serbase.h"
#include "wx/dynlib.h"
#include "wx/msgdlg.h"

wxObject* wxCreateStoredObject( wxInputStream &stream )
{
  wxObjectInputStream obj_s(stream);
  return obj_s.LoadObject();
};

void wxObject::StoreObject( wxObjectOutputStream& stream )
{
  wxString obj_name = wxString(GetClassInfo()->GetClassName()) + "_Serialize";
  wxLibrary *lib = wxTheLibraries.LoadLibrary("wxserial");

  if (!lib) {
    wxMessageBox("Can't load wxSerial dynamic library.", "Alert !");
    return;
  }
  if (!m_serialObj) {
    m_serialObj = (WXSERIAL(wxObject) *)lib->CreateObject( obj_name );

    if (!m_serialObj) {
      wxString message;

      message.Printf("Can't find the serialization object (%s) for the object %s",
                     WXSTRINGCAST obj_name,
                     WXSTRINGCAST GetClassInfo()->GetClassName());
      wxMessageBox(message, "Alert !");
      return;
    }
    m_serialObj->SetObject(this);
  }

  m_serialObj->StoreObject(stream);
}

void wxObject::LoadObject( wxObjectInputStream& stream )
{
  wxString obj_name = wxString(GetClassInfo()->GetClassName()) + "_Serialize";
  wxLibrary *lib = wxTheLibraries.LoadLibrary("wxserial");

  if (!m_serialObj) {
    m_serialObj = (WXSERIAL(wxObject) *)lib->CreateObject( obj_name );

    if (!m_serialObj) {
      wxString message;

      message.Printf("Can't find the serialization object (%s) for the object %s",
                     WXSTRINGCAST obj_name,
                     WXSTRINGCAST GetClassInfo()->GetClassName());
      wxMessageBox(message, "Alert !");
      return;
    }
    m_serialObj->SetObject(this);
  }

  m_serialObj->LoadObject(stream);
}

#endif

/*
 * wxObject: cloning of objects
 */

void wxObject::Ref(const wxObject& clone)
{
    // delete reference to old data
    UnRef();
    // reference new data
    if (clone.m_refData) {
        m_refData = clone.m_refData;
        ++(m_refData->m_count);
    }
}

void wxObject::UnRef(void)
{
    if (m_refData) {
        assert(m_refData->m_count > 0);
        --(m_refData->m_count);
        if (m_refData->m_count == 0)
            delete m_refData;
    }
    m_refData = (wxObjectRefData *) NULL;
}

/*
 * wxObjectData
 */

wxObjectRefData::wxObjectRefData(void) : m_count(1)
{
}

wxObjectRefData::~wxObjectRefData(void)
{
}

