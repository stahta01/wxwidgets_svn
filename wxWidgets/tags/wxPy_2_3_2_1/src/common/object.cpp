/////////////////////////////////////////////////////////////////////////////
// Name:        object.cpp
// Purpose:     wxObject implementation
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:     wxWindows license
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
    #if wxUSE_SERIAL
        #include "wx/objstrm.h"
        #include "wx/serbase.h"

        // for error messages
        #include "wx/log.h"
        #include "wx/intl.h"
    #endif // wxUSE_SERIAL
#endif // WX_PRECOMP

#include <string.h>
#include <assert.h>

#if (defined(__WXDEBUG__) && wxUSE_MEMORY_TRACING) || wxUSE_DEBUG_CONTEXT
#include "wx/memory.h"
#endif

#if defined(__WXDEBUG__) || wxUSE_DEBUG_CONTEXT
    // for wxObject::Dump
    #include "wx/ioswrap.h"
    #if defined(__VISAGECPP__)
    // help with VA debugging
        #define DEBUG_PRINTF(NAME)   { static int raz=0; \
          printf( #NAME " %i\n",raz); fflush(stdout);       \
           raz++;                                        \
         }
    #else
        #define DEBUG_PRINTF(NAME)
    #endif
#endif

wxClassInfo wxObject::sm_classwxObject((wxChar *) wxT("wxObject"), (wxChar *) NULL, (wxChar *) NULL, (int ) sizeof(wxObject), (wxObjectConstructorFn) NULL);
wxClassInfo* wxClassInfo::sm_first = (wxClassInfo *) NULL;
wxHashTable* wxClassInfo::sm_classTable = (wxHashTable*) NULL;

// These are here so we can avoid 'always true/false' warnings
// by referring to these instead of TRUE/FALSE
const bool wxTrue = TRUE;
const bool wxFalse = FALSE;

/*
 * wxWindows root object.
 */

wxObject::wxObject()
{
    m_refData = (wxObjectRefData *) NULL;
#if wxUSE_SERIAL
    m_serialObj = (wxObject_Serialize *)NULL;
#endif
}

wxObject::~wxObject()
{
    UnRef();
#if wxUSE_SERIAL
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

bool wxObject::IsKindOf(wxClassInfo *info) const
{
    wxClassInfo *thisInfo = GetClassInfo();
    if (thisInfo)
        return thisInfo->IsKindOf(info);
    else
        return FALSE;
}

#if wxUSE_STD_IOSTREAM && (defined(__WXDEBUG__) || wxUSE_DEBUG_CONTEXT)
void wxObject::Dump(wxSTD ostream& str)
{
    if (GetClassInfo() && GetClassInfo()->GetClassName())
        str << GetClassInfo()->GetClassName();
    else
        str << "unknown object class";
}
#endif

#if defined(__WXDEBUG__) && wxUSE_MEMORY_TRACING

#ifdef new
#undef new
#endif

void *wxObject::operator new (size_t size, wxChar * fileName, int lineNum)
{
    return wxDebugAlloc(size, fileName, lineNum, TRUE);
}

#if defined(__VISAGECPP__)
#  if __DEBUG_ALLOC__
void wxObject::operator delete (void * buf,const char * _fname, size_t _line)
{
    wxDebugFree(buf);
}
#  endif  //__DEBUG_ALLOC__
#else
void wxObject::operator delete (void * buf)
{
    wxDebugFree(buf);
}
#endif // __VISAGECPP__

// VC++ 6.0
#if defined(__VISUALC__) && (__VISUALC__ >= 1200)
void wxObject::operator delete(void* pData, wxChar* /* fileName */, int /* lineNum */)
{
    ::operator delete(pData);
}
#endif

// Cause problems for VC++ - crashes
#if (!defined(__VISUALC__) && wxUSE_ARRAY_MEMORY_OPERATORS ) || defined(__MWERKS__)
void * wxObject::operator new[] (size_t size, wxChar * fileName, int lineNum)
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

wxClassInfo::wxClassInfo(const wxChar *cName,
                         const wxChar *baseName1,
                         const wxChar *baseName2,
                         int sz,
                         wxObjectConstructorFn constr)
{
    m_className = cName;
    m_baseClassName1 = baseName1;
    m_baseClassName2 = baseName2;

    m_objectSize = sz;
    m_objectConstructor = constr;

    m_next = sm_first;
    sm_first = this;

    m_baseInfo1 = (wxClassInfo *) NULL;
    m_baseInfo2 = (wxClassInfo *) NULL;
}

wxObject *wxClassInfo::CreateObject()
{
    if (m_objectConstructor)
        return (wxObject *)(*m_objectConstructor)();
    else
        return (wxObject *) NULL;
}

wxClassInfo *wxClassInfo::FindClass(const wxChar *c)
{
    wxClassInfo *p = sm_first;
    while (p)
    {
        if ( wxStrcmp(p->GetClassName(), c) == 0 )
            break;

        p = p->m_next;
    }

    return p;
}

// Climb upwards through inheritance hierarchy.
// Dual inheritance is catered for.
bool wxClassInfo::IsKindOf(const wxClassInfo *info) const
{
    if (info == NULL)
        return FALSE;

    if (this == info)
        return TRUE;

    if (m_baseInfo1)
        if (m_baseInfo1->IsKindOf(info))
            return TRUE;

    if (m_baseInfo2)
        return m_baseInfo2->IsKindOf(info);

    return FALSE;
}

// Set pointers to base class(es) to speed up IsKindOf
void wxClassInfo::InitializeClasses()
{
    // using IMPLEMENT_DYNAMIC_CLASS() macro twice (which may happen if you
    // link any object module twice mistakenly) will break this function
    // because it will enter an infinite loop and eventually die with "out of
    // memory" - as this is quite hard to detect if you're unaware of this,
    // try to do some checks here
#ifdef __WXDEBUG__
    // more classes than we'll ever have
    static const size_t nMaxClasses = 10000;
    size_t nClass = 0;
#endif // Debug

    wxClassInfo::sm_classTable = new wxHashTable(wxKEY_STRING);

    // Index all class infos by their class name
    wxClassInfo *info = sm_first;
    while (info)
    {
        if (info->m_className)
        {
            wxASSERT_MSG( ++nClass < nMaxClasses,
                          _T("an infinite loop detected - have you used IMPLEMENT_DYNAMIC_CLASS() twice (may be by linking some object module(s) twice)?") );

            sm_classTable->Put(info->m_className, (wxObject *)info);
        }

        info = info->m_next;
    }

    // Set base pointers for each wxClassInfo
    info = sm_first;
    while (info)
    {
        if (info->GetBaseClassName1())
            info->m_baseInfo1 = (wxClassInfo *)sm_classTable->Get(info->GetBaseClassName1());
        if (info->GetBaseClassName2())
            info->m_baseInfo2 = (wxClassInfo *)sm_classTable->Get(info->GetBaseClassName2());
        info = info->m_next;
    }
}

void wxClassInfo::CleanUpClasses()
{
    delete wxClassInfo::sm_classTable;
    wxClassInfo::sm_classTable = NULL;
}

wxObject *wxCreateDynamicObject(const wxChar *name)
{
#if defined(__WXDEBUG__) || wxUSE_DEBUG_CONTEXT
 DEBUG_PRINTF(wxObject *wxCreateDynamicObject)
#endif

    if (wxClassInfo::sm_classTable)
    {
        wxClassInfo *info = (wxClassInfo *)wxClassInfo::sm_classTable->Get(name);
        if (!info)
            return (wxObject *)NULL;

        return info->CreateObject();
    }
    else
    {
        wxClassInfo *info = wxClassInfo::sm_first;
        while (info)
        {
            if (info->m_className && wxStrcmp(info->m_className, name) == 0)
                return info->CreateObject();
            info = info->m_next;
        }
        return (wxObject*) NULL;
    }
}

#if wxUSE_SERIAL

#include "wx/serbase.h"
#include "wx/dynlib.h"

wxObject* wxCreateStoredObject( wxInputStream &stream )
{
    wxObjectInputStream obj_s(stream);
    return obj_s.LoadObject();
};

void wxObject::StoreObject( wxObjectOutputStream& stream )
{
#if defined(__WXDEBUG__) || wxUSE_DEBUG_CONTEXT
 DEBUG_PRINTF(wxObject::StoreObject)
#endif

    wxString obj_name = wxString(GetClassInfo()->GetClassName()) + "_Serialize";
    wxLibrary *lib = wxTheLibraries.LoadLibrary("wxserial");

    if (!lib) {
        wxLogError(_("Can't load wxSerial dynamic library."));
        return;
    }
    if (!m_serialObj) {
        m_serialObj = (WXSERIAL(wxObject) *)lib->CreateObject( obj_name );

        if (!m_serialObj) {
            wxLogError(_("Can't find the serialization object '%s' "
                        "for the object '%s'."),
                    obj_name.c_str(),
                    GetClassInfo()->GetClassName());
            return;
        }
        m_serialObj->SetObject(this);
    }

    m_serialObj->StoreObject(stream);
}

void wxObject::LoadObject( wxObjectInputStream& stream )
{
#if defined(__WXDEBUG__) || wxUSE_DEBUG_CONTEXT
 DEBUG_PRINTF(wxObject::LoadObject)
#endif

    wxString obj_name = wxString(GetClassInfo()->GetClassName()) + "_Serialize";
    wxLibrary *lib = wxTheLibraries.LoadLibrary("wxserial");

    if (!m_serialObj) {
        m_serialObj = (WXSERIAL(wxObject) *)lib->CreateObject( obj_name );

        if (!m_serialObj) {
            wxLogError(_("Can't find the serialization object '%s' "
                        "for the object '%s'."),
                    obj_name.c_str(),
                    GetClassInfo()->GetClassName());
            return;
        }
        m_serialObj->SetObject(this);
    }

    m_serialObj->LoadObject(stream);
}

#endif // wxUSE_SERIAL

/*
 * wxObject: cloning of objects
 */

void wxObject::Ref(const wxObject& clone)
{
#if defined(__WXDEBUG__) || wxUSE_DEBUG_CONTEXT
 DEBUG_PRINTF(wxObject::Ref)
#endif
     // delete reference to old data
    UnRef();
    // reference new data
    if (clone.m_refData) {
        m_refData = clone.m_refData;
        ++(m_refData->m_count);
    }
}

void wxObject::UnRef()
{
    if ( m_refData )
    {
        wxASSERT_MSG( m_refData->m_count > 0, _T("invalid ref data count") );

        if ( !--m_refData->m_count )
            delete m_refData;
        m_refData = (wxObjectRefData *) NULL;
    }
}

/*
 * wxObjectData
 */

wxObjectRefData::wxObjectRefData(void) : m_count(1)
{
}

wxObjectRefData::~wxObjectRefData()
{
}

#if defined(__DARWIN__) && defined(DYLIB_INIT)

extern "C" {
    void __initialize_Cplusplus(void);
    void wxWindowsDylibInit(void);
};

// Dynamic shared library (dylib) initialization routine
//   required to initialize static C++ objects bacause of lazy dynamic linking
//   http://developer.apple.com/techpubs/macosx/Essentials/
//          SystemOverview/Frameworks/Dynamic_Shared_Libraries.html
//
void wxWindowsDylibInit()
{
    // The function __initialize_Cplusplus() must be called from the shared
    // library initialization routine to cause the static C++ objects in
    // the library to be initialized (reference number 2441683).

    __initialize_Cplusplus();
}

#endif
