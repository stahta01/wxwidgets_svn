/////////////////////////////////////////////////////////////////////////////
// Name:        object.h
// Purpose:     wxObject class, plus run-time type information macros
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_OBJECTH__
#define _WX_OBJECTH__

#ifdef __GNUG__
#pragma interface "object.h"
#endif

#include "wx/defs.h"

class WXDLLEXPORT wxObject;

#if USE_DYNAMIC_CLASSES

#ifdef __GNUWIN32__
#ifdef GetClassName
#undef GetClassName
#endif
#endif

class WXDLLEXPORT wxClassInfo;
class WXDLLIMPORT ostream;
class WXDLLEXPORT wxInputStream;
class WXDLLEXPORT wxObjectInputStream;
class WXDLLEXPORT wxObjectOutputStream;
class WXDLLEXPORT wxHashTable;
class WXDLLEXPORT wxObject_Serialize;

/*
 * Dynamic object system declarations
 */

typedef wxObject * (*wxObjectConstructorFn) (void);

class WXDLLEXPORT wxClassInfo
{
 public:
   wxClassInfo(char *cName, char *baseName1, char *baseName2, int sz, wxObjectConstructorFn fn);

   wxObject *CreateObject(void);
   
   inline char *GetClassName(void) const { return m_className; }
   inline char *GetBaseClassName1(void) const { return m_baseClassName1; }
   inline char *GetBaseClassName2(void) const { return m_baseClassName2; }
   inline wxClassInfo* GetBaseClass1() const { return m_baseInfo1; }
   inline wxClassInfo* GetBaseClass2() const { return m_baseInfo2; }
   inline int GetSize(void) const { return m_objectSize; }
   inline wxObjectConstructorFn GetConstructor() const { return m_objectConstructor; }
   inline wxClassInfo* GetFirst() const { return sm_first; }
   inline wxClassInfo* GetNext() const { return m_next; }
   bool IsKindOf(wxClassInfo *info);

   static wxClassInfo *FindClass(char *c);

   // Initializes parent pointers and hash table for fast searching.
   static void InitializeClasses(void);

   // Cleans up hash table used for fast searching.
   static void CleanUpClasses(void);

public:
   char*                    m_className;
   char*                    m_baseClassName1;
   char*                    m_baseClassName2;
   int                      m_objectSize;
   wxObjectConstructorFn    m_objectConstructor;
   
   // Pointers to base wxClassInfos: set in InitializeClasses
   // called from wx_main.cc
   wxClassInfo*             m_baseInfo1;
   wxClassInfo*             m_baseInfo2;

   static wxClassInfo*      sm_first;
   wxClassInfo*             m_next;

   static wxHashTable*      sm_classTable;
};

wxObject* WXDLLEXPORT wxCreateDynamicObject(const char *name);

#ifdef USE_SERIAL
wxObject* WXDLLEXPORT wxCreateStoredObject( wxInputStream& stream );
#endif

#define DECLARE_DYNAMIC_CLASS(name) \
 public:\
  static wxClassInfo sm_class##name;\
  wxClassInfo *GetClassInfo() \
   { return &name::sm_class##name; }

#define DECLARE_ABSTRACT_CLASS(name) DECLARE_DYNAMIC_CLASS(name)
#define DECLARE_CLASS(name) DECLARE_DYNAMIC_CLASS(name)

//////
////// for concrete classes
//////

// Single inheritance with one base class
#define IMPLEMENT_DYNAMIC_CLASS(name, basename) \
wxObject* WXDLLEXPORT_CTORFN wxConstructorFor##name(void) \
   { return new name; }\
 wxClassInfo name::sm_class##name((char *) #name, (char *) #basename, (char *) NULL, (int) sizeof(name), (wxObjectConstructorFn) wxConstructorFor##name);

// Multiple inheritance with two base classes
#define IMPLEMENT_DYNAMIC_CLASS2(name, basename1, basename2) \
wxObject* WXDLLEXPORT_CTORFN wxConstructorFor##name(void) \
   { return new name; }\
 wxClassInfo name::sm_class##name((char *) #name, (char *) #basename1, (char *) #basename2, (int) sizeof(name), (wxObjectConstructorFn) wxConstructorFor##name);

//////
////// for abstract classes
//////

// Single inheritance with one base class
#define IMPLEMENT_ABSTRACT_CLASS(name, basename) \
 wxClassInfo name::sm_class##name((char *) #name, (char *) #basename, \
		 (char *) NULL, (int) sizeof(name), (wxObjectConstructorFn) NULL);

// Multiple inheritance with two base classes
#define IMPLEMENT_ABSTRACT_CLASS2(name, basename1, basename2) \
 wxClassInfo name::sm_class##name((char *) #name, (char *) #basename1, (char *) #basename2, (int) sizeof(name), (wxObjectConstructorFn) NULL);

#define IMPLEMENT_CLASS IMPLEMENT_ABSTRACT_CLASS
#define IMPLEMENT_CLASS2 IMPLEMENT_ABSTRACT_CLASS2

#define CLASSINFO(name) (&name::sm_class##name)

#else

// No dynamic class system: so stub out the macros
#define DECLARE_DYNAMIC_CLASS(name)
#define DECLARE_ABSTRACT_CLASS(name)
#define DECLARE_CLASS(name)
#define IMPLEMENT_DYNAMIC_CLASS(name, basename)
#define IMPLEMENT_DYNAMIC_CLASS2(name, basename1, basename2)
#define IMPLEMENT_ABSTRACT_CLASS(name, basename)
#define IMPLEMENT_ABSTRACT_CLASS2(name, basename1, basename2)
#define IMPLEMENT_CLASS IMPLEMENT_ABSTRACT_CLASS
#define IMPLEMENT_CLASS2 IMPLEMENT_ABSTRACT_CLASS2

#endif

#define IS_KIND_OF(obj, className) obj->IsKindOf(&className::sm_class##className)

// Unfortunately Borland seems to need this include.
#ifdef __BORLANDC__
#if USE_IOSTREAMH
#include <iostream.h>
#else
#include <iostream>
#endif
#endif

class WXDLLEXPORT wxObjectRefData;

class WXDLLEXPORT wxObject
{
 public:

  // This is equivalent to using the macro DECLARE_ABSTRACT_CLASS
  static wxClassInfo sm_classwxObject;

  wxObject(void);
  virtual ~wxObject(void);

  virtual wxClassInfo *GetClassInfo(void) { return &sm_classwxObject; }

  bool IsKindOf(wxClassInfo *info);

#if WXDEBUG && USE_MEMORY_TRACING
  void * operator new (size_t size, char * fileName = NULL, int lineNum = 0);
  void operator delete (void * buf);
  
  // Cause problems for VC++
#ifndef _MSC_VER
  void * operator new[] (size_t size, char * fileName = NULL, int lineNum = 0);
  void operator delete[] (void * buf);
#endif

#endif

#if WXDEBUG || USE_DEBUG_CONTEXT
  virtual void Dump(ostream& str);
#endif

#ifdef USE_SERIAL
  virtual void StoreObject( wxObjectOutputStream &stream );
  virtual void LoadObject( wxObjectInputStream &stream );
#endif

  // make a 'clone' of the object
  void Ref(const wxObject& clone);

  // destroy a reference
  void UnRef(void);

  inline wxObjectRefData *GetRefData(void) const { return m_refData; }
  inline void SetRefData(wxObjectRefData *data) { m_refData = data; }

protected:
  wxObjectRefData*      m_refData;
#ifdef USE_SERIAL
  wxObject_Serialize*   m_serialObj;
#endif
};

/*
 * wxObjectData
 */

class WXDLLEXPORT wxObjectRefData {

    friend class wxObject;

public:
    wxObjectRefData(void);
    virtual ~wxObjectRefData(void);

    inline int GetRefCount(void) const { return m_count; }
private:
    int m_count;
};

#if WXDEBUG && USE_GLOBAL_MEMORY_OPERATORS
#ifndef WXDEBUG_NEW
#define WXDEBUG_NEW new(__FILE__,__LINE__)
#endif
#define new WXDEBUG_NEW
#endif

#endif
    // _WX_OBJECTH__


