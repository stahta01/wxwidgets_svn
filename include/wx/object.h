/////////////////////////////////////////////////////////////////////////////
// Name:        wx/object.h
// Purpose:     wxObject class, plus run-time type information macros
// Author:      Julian Smart
// Modified by: Ron Lee
// Created:     01/02/97
// RCS-ID:      $Id$
// Copyright:   (c) 1997 Julian Smart
//              (c) 2001 Ron Lee <ron@debian.org>
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_OBJECTH__
#define _WX_OBJECTH__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "object.h"
#endif

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include "wx/defs.h"
#include "wx/memory.h"

class WXDLLIMPEXP_BASE wxObject;

#if wxUSE_DYNAMIC_CLASSES

#ifndef wxUSE_EXTENDED_RTTI
#define wxUSE_EXTENDED_RTTI 0
#endif

#if wxUSE_EXTENDED_RTTI
#include "wx/xti.h"
#else

// ----------------------------------------------------------------------------
// conditional compilation
// ----------------------------------------------------------------------------

// this shouldn't be needed any longer as <wx/msw/private.h> does it but it
// doesn't hurt neither
#ifdef GetClassName
#undef GetClassName
#endif
#ifdef GetClassInfo
#undef GetClassInfo
#endif

class WXDLLIMPEXP_BASE wxClassInfo;
class WXDLLIMPEXP_BASE wxHashTable;
class WXDLLIMPEXP_BASE wxObjectRefData;

// ----------------------------------------------------------------------------
// wxClassInfo
// ----------------------------------------------------------------------------

typedef wxObject *(*wxObjectConstructorFn)(void);

class WXDLLIMPEXP_BASE wxClassInfo
{
public:
    wxClassInfo( const wxChar *className,
                 const wxClassInfo *baseInfo1,
                 const wxClassInfo *baseInfo2,
                 int size,
                 wxObjectConstructorFn ctor )
        : m_className(className)
        , m_objectSize(size)
        , m_objectConstructor(ctor)
        , m_baseInfo1(baseInfo1)
        , m_baseInfo2(baseInfo2)
        , m_next(sm_first)
        { 
            sm_first = this;
            Register();
        }

    ~wxClassInfo();

    wxObject *CreateObject() { return m_objectConstructor ? (*m_objectConstructor)() : 0; }

    const wxChar       *GetClassName() const { return m_className; }
    const wxChar       *GetBaseClassName1() const
        { return m_baseInfo1 ? m_baseInfo1->GetClassName() : NULL; }
    const wxChar       *GetBaseClassName2() const
        { return m_baseInfo2 ? m_baseInfo2->GetClassName() : NULL; }
    const wxClassInfo  *GetBaseClass1() const { return m_baseInfo1; }
    const wxClassInfo  *GetBaseClass2() const { return m_baseInfo2; }
    int                 GetSize() const { return m_objectSize; }

    wxObjectConstructorFn      GetConstructor() const { return m_objectConstructor; }
    static const wxClassInfo  *GetFirst() { return sm_first; }
    const wxClassInfo         *GetNext() const { return m_next; }
    static wxClassInfo        *FindClass(const wxChar *className);

        // Climb upwards through inheritance hierarchy.
        // Dual inheritance is catered for.

    bool IsKindOf(const wxClassInfo *info) const
    {
        return info != 0 &&
               ( info == this ||
                 ( m_baseInfo1 && m_baseInfo1->IsKindOf(info) ) ||
                 ( m_baseInfo2 && m_baseInfo2->IsKindOf(info) ) );
    }

#if WXWIN_COMPATIBILITY_2_4
    // Initializes parent pointers and hash table for fast searching.
    wxDEPRECATED( static void InitializeClasses() );
    // Cleans up hash table used for fast searching.
    wxDEPRECATED( static void CleanUpClasses() );
#endif
    static void     CleanUp();
    
public:
    const wxChar            *m_className;
    int                      m_objectSize;
    wxObjectConstructorFn    m_objectConstructor;

        // Pointers to base wxClassInfos: set in InitializeClasses

    const wxClassInfo       *m_baseInfo1;
    const wxClassInfo       *m_baseInfo2;

        // class info object live in a linked list:
        // pointers to its head and the next element in it

    static wxClassInfo      *sm_first;
    wxClassInfo             *m_next;

    // FIXME: this should be private (currently used directly by way too
    //        many clients)
    static wxHashTable      *sm_classTable;

private:
    // InitializeClasses() helper
    static wxClassInfo *GetBaseByName(const wxChar *name);

    DECLARE_NO_COPY_CLASS(wxClassInfo)
   
protected: 
    // registers the class
    void Register();
    void Unregister();
};

WXDLLIMPEXP_BASE wxObject *wxCreateDynamicObject(const wxChar *name);

#if WXWIN_COMPATIBILITY_2_4
inline void wxClassInfo::InitializeClasses() {}
inline void wxClassInfo::CleanUpClasses() {}
#endif

// ----------------------------------------------------------------------------
// Dynamic class macros
// ----------------------------------------------------------------------------

#define DECLARE_DYNAMIC_CLASS(name)           \
 public:                                      \
  static wxClassInfo ms_classInfo;            \
  static wxObject* wxCreateObject();          \
  virtual wxClassInfo *GetClassInfo() const   \
   { return &name::ms_classInfo; }

#define DECLARE_DYNAMIC_CLASS_NO_ASSIGN(name)   \
    DECLARE_NO_ASSIGN_CLASS(name)               \
    DECLARE_DYNAMIC_CLASS(name)

#define DECLARE_DYNAMIC_CLASS_NO_COPY(name)   \
    DECLARE_NO_COPY_CLASS(name)               \
    DECLARE_DYNAMIC_CLASS(name)

#define DECLARE_ABSTRACT_CLASS(name) DECLARE_DYNAMIC_CLASS(name)
#define DECLARE_CLASS(name) DECLARE_DYNAMIC_CLASS(name)

// -----------------------------------
// for concrete classes
// -----------------------------------

    // Single inheritance with one base class

#define IMPLEMENT_DYNAMIC_CLASS(name, basename)                 \
 wxObject* name::wxCreateObject()                             \
  { return new name; }                                          \
 wxClassInfo name::ms_classInfo(wxT(#name),                     \
            &basename::ms_classInfo, NULL,                      \
            (int) sizeof(name),                                 \
            (wxObjectConstructorFn) name::wxCreateObject);

    // Multiple inheritance with two base classes

#define IMPLEMENT_DYNAMIC_CLASS2(name, basename1, basename2)    \
 wxObject* name::wxCreateObject()                             \
  { return new name; }                                          \
 wxClassInfo name::ms_classInfo(wxT(#name),                     \
            &basename1::ms_classInfo,                           \
            &basename2::ms_classInfo,                           \
            wxT(#basename2), (int) sizeof(name),                \
            (wxObjectConstructorFn) name::wxCreateObject);

// -----------------------------------
// for abstract classes
// -----------------------------------

    // Single inheritance with one base class

#define IMPLEMENT_ABSTRACT_CLASS(name, basename)                \
 wxClassInfo name::ms_classInfo(wxT(#name),                     \
            &basename::ms_classInfo, NULL,                      \
            (int) sizeof(name), (wxObjectConstructorFn) 0);

    // Multiple inheritance with two base classes

#define IMPLEMENT_ABSTRACT_CLASS2(name, basename1, basename2)   \
 wxClassInfo name::ms_classInfo(wxT(#name),                     \
            &basename1::ms_classInfo,                           \
            &basename2::ms_classInfo,                           \
            (int) sizeof(name),                                 \
            (wxObjectConstructorFn) 0);

#define IMPLEMENT_CLASS IMPLEMENT_ABSTRACT_CLASS
#define IMPLEMENT_CLASS2 IMPLEMENT_ABSTRACT_CLASS2

#endif // !wxUSE_EXTENDED_RTTI


// -----------------------------------
// for pluggable classes
// -----------------------------------

    // NOTE: this should probably be the very first statement
    //       in the class declaration so wxPluginSentinel is
    //       the first member initialised and the last destroyed.

// _DECLARE_DL_SENTINEL(name) wxPluginSentinel m_pluginsentinel;

#if wxUSE_NESTED_CLASSES

#define _DECLARE_DL_SENTINEL(name, exportdecl)  \
class exportdecl name##PluginSentinel {         \
private:                                        \
    static const wxString sm_className;         \
public:                                         \
    name##PluginSentinel();                     \
    ~name##PluginSentinel();                    \
};                                              \
name##PluginSentinel  m_pluginsentinel;

#define _IMPLEMENT_DL_SENTINEL(name)                                \
 const wxString name::name##PluginSentinel::sm_className(#name);    \
 name::name##PluginSentinel::name##PluginSentinel() {               \
    wxPluginLibrary *e = (wxPluginLibrary*) wxPluginLibrary::ms_classes.Get(#name);   \
    if( e != 0 ) { e->RefObj(); }                                      \
 }                                                                  \
 name::name##PluginSentinel::~name##PluginSentinel() {            \
    wxPluginLibrary *e = (wxPluginLibrary*) wxPluginLibrary::ms_classes.Get(#name);   \
    if( e != 0 ) { e->UnrefObj(); }                                 \
 }
#else

#define _DECLARE_DL_SENTINEL(name)
#define _IMPLEMENT_DL_SENTINEL(name)

#endif  // wxUSE_NESTED_CLASSES

#define DECLARE_PLUGGABLE_CLASS(name) \
 DECLARE_DYNAMIC_CLASS(name) _DECLARE_DL_SENTINEL(name, WXDLLEXPORT)
#define DECLARE_ABSTRACT_PLUGGABLE_CLASS(name)  \
 DECLARE_ABSTRACT_CLASS(name) _DECLARE_DL_SENTINEL(name, WXDLLEXPORT)

#define DECLARE_USER_EXPORTED_PLUGGABLE_CLASS(name, usergoo) \
 DECLARE_DYNAMIC_CLASS(name) _DECLARE_DL_SENTINEL(name, usergoo)
#define DECLARE_USER_EXPORTED_ABSTRACT_PLUGGABLE_CLASS(name, usergoo)  \
 DECLARE_ABSTRACT_CLASS(name) _DECLARE_DL_SENTINEL(name, usergoo)

#define IMPLEMENT_PLUGGABLE_CLASS(name, basename) \
 IMPLEMENT_DYNAMIC_CLASS(name, basename) _IMPLEMENT_DL_SENTINEL(name)
#define IMPLEMENT_PLUGGABLE_CLASS2(name, basename1, basename2)  \
 IMPLEMENT_DYNAMIC_CLASS2(name, basename1, basename2) _IMPLEMENT_DL_SENTINEL(name)
#define IMPLEMENT_ABSTRACT_PLUGGABLE_CLASS(name, basename) \
 IMPLEMENT_ABSTRACT_CLASS(name, basename) _IMPLEMENT_DL_SENTINEL(name)
#define IMPLEMENT_ABSTRACT_PLUGGABLE_CLASS2(name, basename1, basename2)  \
 IMPLEMENT_ABSTRACT_CLASS2(name, basename1, basename2) _IMPLEMENT_DL_SENTINEL(name)

#define IMPLEMENT_USER_EXPORTED_PLUGGABLE_CLASS(name, basename) \
 IMPLEMENT_PLUGGABLE_CLASS(name, basename)
#define IMPLEMENT_USER_EXPORTED_PLUGGABLE_CLASS2(name, basename1, basename2)  \
 IMPLEMENT_PLUGGABLE_CLASS2(name, basename1, basename2)
#define IMPLEMENT_USER_EXPORTED_ABSTRACT_PLUGGABLE_CLASS(name, basename) \
 IMPLEMENT_ABSTRACT_PLUGGABLE_CLASS(name, basename)
#define IMPLEMENT_USER_EXPORTED_ABSTRACT_PLUGGABLE_CLASS2(name, basename1, basename2)  \
 IMPLEMENT_ABSTRACT_PLUGGABLE_CLASS2(name, basename1, basename2)

#define CLASSINFO(name) (&name::ms_classInfo)

#else // !wxUSE_DYNAMIC_CLASSES

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

#define DECLARE_PLUGGABLE_CLASS(name)
#define DECLARE_ABSTRACT_PLUGGABLE_CLASS(name)
#define IMPLEMENT_PLUGGABLE_CLASS(name, basename)
#define IMPLEMENT_PLUGGABLE_CLASS2(name, basename1, basename2)
#define IMPLEMENT_ABSTRACT_PLUGGABLE_CLASS(name, basename)
#define IMPLEMENT_ABSTRACT_PLUGGABLE_CLASS2(name, basename1, basename2)

#define DECLARE_USER_EXPORTED_PLUGGABLE_CLASS(name, usergoo)
#define DECLARE_USER_EXPORTED_ABSTRACT_PLUGGABLE_CLASS(name, usergoo)
#define IMPLEMENT_USER_EXPORTED_PLUGGABLE_CLASS(name, basename)
#define IMPLEMENT_USER_EXPORTED_PLUGGABLE_CLASS2(name, basename1, basename2)
#define IMPLEMENT_USER_EXPORTED_ABSTRACT_PLUGGABLE_CLASS(name, basename)
#define IMPLEMENT_USER_EXPORTED_ABSTRACT_PLUGGABLE_CLASS2(name, basename1, basename2)

#endif // wxUSE_DYNAMIC_CLASSES

#define wxIS_KIND_OF(obj, className) obj->IsKindOf(&className::ms_classInfo)

// Just seems a bit nicer-looking (pretend it's not a macro)
#define wxIsKindOf(obj, className) obj->IsKindOf(&className::ms_classInfo)

// to be replaced by dynamic_cast<> in the future
#define wxDynamicCast(obj, className) \
 ((className *) wxCheckDynamicCast((wxObject*)(obj), &className::ms_classInfo))

// The 'this' pointer is always true, so use this version
// to cast the this pointer and avoid compiler warnings.
#define wxDynamicCastThis(className) \
 (IsKindOf(&className::ms_classInfo) ? (className *)(this) : (className *)0)

#ifdef __WXDEBUG__
inline void* wxCheckCast(void *ptr)
{
    wxASSERT_MSG( ptr, _T("wxStaticCast() used incorrectly") );
    return ptr;
}
#define wxStaticCast(obj, className) \
 ((className *)wxCheckCast(wxDynamicCast(obj, className)))

#else  // !__WXDEBUG__
#define wxStaticCast(obj, className) ((className *)(obj))

#endif  // __WXDEBUG__

// ----------------------------------------------------------------------------
// set up memory debugging macros
// ----------------------------------------------------------------------------

/*
    Which new/delete operator variants do we want?

    _WX_WANT_NEW_SIZET_WXCHAR_INT             = void *operator new (size_t size, wxChar *fileName = 0, int lineNum = 0)
    _WX_WANT_DELETE_VOID                      = void operator delete (void * buf)
    _WX_WANT_DELETE_VOID_CONSTCHAR_SIZET      = void operator delete (void *buf, const char *_fname, size_t _line)
    _WX_WANT_DELETE_VOID_WXCHAR_INT           = void operator delete(void *buf, wxChar*, int)
    _WX_WANT_ARRAY_NEW_SIZET_WXCHAR_INT       = void *operator new[] (size_t size, wxChar *fileName , int lineNum = 0)
    _WX_WANT_ARRAY_DELETE_VOID                = void operator delete[] (void *buf)
    _WX_WANT_ARRAY_DELETE_VOID_WXCHAR_INT     = void operator delete[] (void* buf, wxChar*, int )
*/

#if defined(__WXDEBUG__) && wxUSE_MEMORY_TRACING

// All compilers get this one
#define _WX_WANT_NEW_SIZET_WXCHAR_INT

// Everyone except Visage gets the next one
#ifndef __VISAGECPP__
    #define _WX_WANT_DELETE_VOID
#endif

// Only visage gets this one under the correct circumstances
#if defined(__VISAGECPP__) && __DEBUG_ALLOC__
    #define _WX_WANT_DELETE_VOID_CONSTCHAR_SIZET
#endif

// Only VC++ 6.0 and CodeWarrior compilers get overloaded delete that matches new
#if ( defined(__VISUALC__) && (__VISUALC__ >= 1200) ) || (defined(__MWERKS__) && (__MWERKS__ >= 0x2400))
    #define _WX_WANT_DELETE_VOID_WXCHAR_INT
#endif

// Now see who (if anyone) gets the array memory operators
#if wxUSE_ARRAY_MEMORY_OPERATORS

    // Everyone except Visual C++ (cause problems for VC++ - crashes)
    #if !defined(__VISUALC__)
        #define _WX_WANT_ARRAY_NEW_SIZET_WXCHAR_INT
    #endif

    // Everyone except Visual C++ (cause problems for VC++ - crashes)
    #if !defined(__VISUALC__)
        #define _WX_WANT_ARRAY_DELETE_VOID
    #endif

    // Only CodeWarrior 6 or higher
    #if defined(__MWERKS__) && (__MWERKS__ >= 0x2400)
        #define _WX_WANT_ARRAY_DELETE_VOID_WXCHAR_INT
    #endif

#endif // wxUSE_ARRAY_MEMORY_OPERATORS

#endif // WXDEBUG && wxUSE_MEMORY_TRACING

// ----------------------------------------------------------------------------
// wxObject: the root class of wxWindows object hierarchy
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_BASE wxObject
{
    DECLARE_ABSTRACT_CLASS(wxObject)

private:
    void InitFrom(const wxObject& other);

public:
    wxObject() { m_refData = NULL; }
    virtual ~wxObject() { UnRef(); }

    wxObject(const wxObject& other)
        {
            InitFrom(other);
        }

    wxObject& operator=(const wxObject& other)
    {
        if ( this != &other )
        {
            UnRef();
            InitFrom(other);
        }
        return *this;
    }

    bool IsKindOf(wxClassInfo *info) const;


    // Turn on the correct set of new and delete operators

#ifdef _WX_WANT_NEW_SIZET_WXCHAR_INT
    void *operator new ( size_t size, const wxChar *fileName = NULL, int lineNum = 0 );
#endif

#ifdef _WX_WANT_DELETE_VOID
    void operator delete ( void * buf );
#endif

#ifdef _WX_WANT_DELETE_VOID_CONSTCHAR_SIZET
    void operator delete ( void *buf, const char *_fname, size_t _line );
#endif

#ifdef _WX_WANT_DELETE_VOID_WXCHAR_INT
    void operator delete ( void *buf, const wxChar*, int );
#endif

#ifdef _WX_WANT_ARRAY_NEW_SIZET_WXCHAR_INT
    void *operator new[] ( size_t size, const wxChar *fileName = NULL, int lineNum = 0 );
#endif

#ifdef _WX_WANT_ARRAY_DELETE_VOID
    void operator delete[] ( void *buf );
#endif

#ifdef _WX_WANT_ARRAY_DELETE_VOID_WXCHAR_INT
    void operator delete[] (void* buf, const wxChar*, int );
#endif

    // ref counted data handling methods

    // get/set
    wxObjectRefData *GetRefData() const { return m_refData; }
    void SetRefData(wxObjectRefData *data) { m_refData = data; }

    // make a 'clone' of the object
    void Ref(const wxObject& clone);

    // destroy a reference
    void UnRef();

protected:
    // ensure that our data is not shared with anybody else: if we have no
    // data, it is created using CreateRefData() below, if we have shared data
    // it is copied using CloneRefData(), otherwise nothing is done
    void AllocExclusive();

    // both methods must be implemented if Unshare() is used, not pure virtual
    // only because of the backwards compatibility reasons

    // create a new m_refData
    virtual wxObjectRefData *CreateRefData() const;

    // create a new m_refData initialized with the given one
    virtual wxObjectRefData *CloneRefData(const wxObjectRefData *data) const;

    wxObjectRefData *m_refData;
};

// ----------------------------------------------------------------------------
// wxObjectRefData: ref counted data meant to be stored in wxObject
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_BASE wxObjectRefData
{
    friend class WXDLLIMPEXP_BASE wxObject;

public:
    wxObjectRefData() : m_count(1) { }
    virtual ~wxObjectRefData() { }

    int GetRefCount() const { return m_count; }

private:
    int m_count;
};


inline wxObject *wxCheckDynamicCast(wxObject *obj, wxClassInfo *classInfo)
{
    return obj && obj->GetClassInfo()->IsKindOf(classInfo) ? obj : NULL;
}

#if wxUSE_EXTENDED_RTTI
class WXDLLIMPEXP_BASE wxDynamicObject : public wxObject
{
    friend class WXDLLIMPEXP_BASE wxDynamicClassInfo ;
public:
    // instantiates this object with an instance of its superclass
    wxDynamicObject(wxObject* superClassInstance, const wxDynamicClassInfo *info) ;
    ~wxDynamicObject();

    void SetProperty (const wxChar *propertyName, const wxxVariant &value);
    wxxVariant GetProperty (const wxChar *propertyName) const ;

    // get the runtime identity of this object
    wxClassInfo *GetClassInfo() const
    {
		return const_cast<wxClassInfo*>((const wxClassInfo*)m_classInfo);
    }

    wxObject* GetSuperClassInstance() const
    {
        return m_superClassInstance ;
    }
private :
    // removes an existing runtime-property
    void RemoveProperty( const wxChar *propertyName ) ;

    // renames an existing runtime-property
    void RenameProperty( const wxChar *oldPropertyName , const wxChar *newPropertyName ) ;

    wxObject *m_superClassInstance ;
    const wxDynamicClassInfo *m_classInfo;
    struct wxDynamicObjectInternal;
    wxDynamicObjectInternal *m_data;
};
#endif

// ----------------------------------------------------------------------------
// more debugging macros
// ----------------------------------------------------------------------------

#ifdef __WXDEBUG__
    #ifndef WXDEBUG_NEW
        #define WXDEBUG_NEW new(__TFILE__,__LINE__)
    #endif
#else // !__WXDEBUG__
    #define WXDEBUG_NEW new
#endif

// Redefine new to be the debugging version. This doesn't work with all
// compilers, in which case you need to use WXDEBUG_NEW explicitly if you wish
// to use the debugging version.

#if defined(__WXDEBUG__) && wxUSE_GLOBAL_MEMORY_OPERATORS && wxUSE_DEBUG_NEW_ALWAYS
    #define new new(__TFILE__,__LINE__)
#endif

#endif  // _WX_OBJECTH__

