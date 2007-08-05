/////////////////////////////////////////////////////////////////////////////
// Name:        wx/object.h
// Purpose:     wxObject class
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

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include "wx/memory.h"

// based on the value of wxUSE_EXTENDED_RTTI symbol,
// only one of the RTTI system will be compiled:
// - the "old" one (defined by rtti.h) or
// - the "new" one (defined by xti.h)
#include "wx/xti.h"
#include "wx/rtti.h"

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

#define wxDECLARE_PLUGGABLE_CLASS(name) \
 wxDECLARE_DYNAMIC_CLASS(name) _DECLARE_DL_SENTINEL(name, WXDLLEXPORT)
#define wxDECLARE_ABSTRACT_PLUGGABLE_CLASS(name)  \
 wxDECLARE_ABSTRACT_CLASS(name) _DECLARE_DL_SENTINEL(name, WXDLLEXPORT)

#define wxDECLARE_USER_EXPORTED_PLUGGABLE_CLASS(name, usergoo) \
 wxDECLARE_DYNAMIC_CLASS(name) _DECLARE_DL_SENTINEL(name, usergoo)
#define wxDECLARE_USER_EXPORTED_ABSTRACT_PLUGGABLE_CLASS(name, usergoo)  \
 wxDECLARE_ABSTRACT_CLASS(name) _DECLARE_DL_SENTINEL(name, usergoo)

#define wxIMPLEMENT_PLUGGABLE_CLASS(name, basename) \
 wxIMPLEMENT_DYNAMIC_CLASS(name, basename) _IMPLEMENT_DL_SENTINEL(name)
#define wxIMPLEMENT_PLUGGABLE_CLASS2(name, basename1, basename2)  \
 wxIMPLEMENT_DYNAMIC_CLASS2(name, basename1, basename2) _IMPLEMENT_DL_SENTINEL(name)
#define wxIMPLEMENT_ABSTRACT_PLUGGABLE_CLASS(name, basename) \
 wxIMPLEMENT_ABSTRACT_CLASS(name, basename) _IMPLEMENT_DL_SENTINEL(name)
#define wxIMPLEMENT_ABSTRACT_PLUGGABLE_CLASS2(name, basename1, basename2)  \
 wxIMPLEMENT_ABSTRACT_CLASS2(name, basename1, basename2) _IMPLEMENT_DL_SENTINEL(name)

#define wxIMPLEMENT_USER_EXPORTED_PLUGGABLE_CLASS(name, basename) \
 wxIMPLEMENT_PLUGGABLE_CLASS(name, basename)
#define wxIMPLEMENT_USER_EXPORTED_PLUGGABLE_CLASS2(name, basename1, basename2)  \
 wxIMPLEMENT_PLUGGABLE_CLASS2(name, basename1, basename2)
#define wxIMPLEMENT_USER_EXPORTED_ABSTRACT_PLUGGABLE_CLASS(name, basename) \
 wxIMPLEMENT_ABSTRACT_PLUGGABLE_CLASS(name, basename)
#define wxIMPLEMENT_USER_EXPORTED_ABSTRACT_PLUGGABLE_CLASS2(name, basename1, basename2)  \
 wxIMPLEMENT_ABSTRACT_PLUGGABLE_CLASS2(name, basename1, basename2)

// ----------------------------------------------------------------------------
// set up memory debugging macros
// ----------------------------------------------------------------------------

/*
    Which new/delete operator variants do we want?

  _WX_WANT_NEW_SIZET_WXCHAR_INT         = void *operator new (size_t size, wxChar *fileName = 0, int lineNum = 0)
  _WX_WANT_DELETE_VOID                  = void operator delete (void * buf)
  _WX_WANT_DELETE_VOID_CONSTCHAR_SIZET  = void operator delete (void *buf, const char *_fname, size_t _line)
  _WX_WANT_DELETE_VOID_WXCHAR_INT       = void operator delete(void *buf, wxChar*, int)
  _WX_WANT_ARRAY_NEW_SIZET_WXCHAR_INT   = void *operator new[] (size_t size, wxChar *fileName , int lineNum = 0)
  _WX_WANT_ARRAY_DELETE_VOID            = void operator delete[] (void *buf)
  _WX_WANT_ARRAY_DELETE_VOID_WXCHAR_INT = void operator delete[] (void* buf, wxChar*, int )
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

// Only VC++ 6 and CodeWarrior get overloaded delete that matches new
#if (defined(__VISUALC__) && (__VISUALC__ >= 1200)) || \
        (defined(__MWERKS__) && (__MWERKS__ >= 0x2400))
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

#endif // __WXDEBUG__ && wxUSE_MEMORY_TRACING

// ----------------------------------------------------------------------------
// wxObjectRefData: ref counted data meant to be stored in wxObject
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_BASE wxObjectRefData
{
    friend class WXDLLIMPEXP_BASE wxObject;

public:
    wxObjectRefData() : m_count(1) { }

    int GetRefCount() const { return m_count; }

    void IncRef() { m_count++; }
    void DecRef();

protected:
    // this object should never be destroyed directly but only as a
    // result of a DecRef() call:
    virtual ~wxObjectRefData() { }

private:
    // our refcount:
    int m_count;
};

// ----------------------------------------------------------------------------
// wxObjectDataPtr: helper class to avoid memleaks because of missing calls
//                  to wxObjectRefData::DecRef
// ----------------------------------------------------------------------------

template <class T>
class wxObjectDataPtr
{
public:
    typedef T element_type;

    wxEXPLICIT wxObjectDataPtr(T *ptr = NULL) : m_ptr(ptr) {}

    // copy ctor
    wxObjectDataPtr(const wxObjectDataPtr<T> &tocopy) 
        : m_ptr(tocopy.m_ptr)
    { 
        if (m_ptr)
            m_ptr->IncRef(); 
    }

    ~wxObjectDataPtr() 
    { 
        if (m_ptr) 
            m_ptr->DecRef(); 
    }

    T *get() const { return m_ptr; }
    T *operator->() const { return get(); }

    void reset(T *ptr)
    {
        if (m_ptr)
            m_ptr->DecRef();
        m_ptr = ptr;
    }

    wxObjectDataPtr& operator=(const wxObjectDataPtr &tocopy)
    { 
        if (m_ptr) 
            m_ptr->DecRef(); 
        m_ptr = tocopy.m_ptr; 
        if (m_ptr)
            m_ptr->IncRef(); 
        return *this;
    }

    wxObjectDataPtr& operator=(T *ptr)
    { 
        if (m_ptr) 
            m_ptr->DecRef(); 
        m_ptr = ptr; 
        if (m_ptr)
            m_ptr->IncRef(); 
        return *this;
    }

private:
    T *m_ptr;
};

// ----------------------------------------------------------------------------
// wxObject: the root class of wxWidgets object hierarchy
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_BASE wxObject
{
    DECLARE_ABSTRACT_CLASS(wxObject)

public:
    wxObject() { m_refData = NULL; }
    virtual ~wxObject() { UnRef(); }

    wxObject(const wxObject& other)
    {
         m_refData = other.m_refData;
         if (m_refData)
             m_refData->m_count++;
    }

    wxObject& operator=(const wxObject& other)
    {
        if ( this != &other )
        {
            Ref(other);
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

    // Make sure this object has only one reference
    void UnShare() { AllocExclusive(); }

    // check if this object references the same data as the other one
    bool IsSameAs(const wxObject& o) const { return m_refData == o.m_refData; }

protected:
    // ensure that our data is not shared with anybody else: if we have no
    // data, it is created using CreateRefData() below, if we have shared data
    // it is copied using CloneRefData(), otherwise nothing is done
    void AllocExclusive();

    // both methods must be implemented if AllocExclusive() is used, not pure
    // virtual only because of the backwards compatibility reasons

    // create a new m_refData
    virtual wxObjectRefData *CreateRefData() const;

    // create a new m_refData initialized with the given one
    virtual wxObjectRefData *CloneRefData(const wxObjectRefData *data) const;

    wxObjectRefData *m_refData;
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
    virtual ~wxDynamicObject();

    void SetProperty (const wxChar *propertyName, const wxxVariant &value);
    wxxVariant GetProperty (const wxChar *propertyName) const ;

    // get the runtime identity of this object
    wxClassInfo *GetClassInfo() const
    {
#ifdef _MSC_VER
        return (wxClassInfo*) m_classInfo;
#else
        wxDynamicClassInfo *nonconst = wx_const_cast(wxDynamicClassInfo *, m_classInfo);
        return wx_static_cast(wxClassInfo *, nonconst);
#endif
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

// Redefine new to be the debugging version. This doesn't work with all
// compilers, in which case you need to use WXDEBUG_NEW explicitly if you wish
// to use the debugging version.

#ifdef __WXDEBUG__
    #define WXDEBUG_NEW new(__TFILE__,__LINE__)

    #if wxUSE_DEBUG_NEW_ALWAYS
        #if wxUSE_GLOBAL_MEMORY_OPERATORS
            #define new WXDEBUG_NEW
        #elif defined(__VISUALC__)
            // Including this file redefines new and allows leak reports to
            // contain line numbers
            #include "wx/msw/msvcrt.h"
        #endif
    #endif // wxUSE_DEBUG_NEW_ALWAYS
#else // !__WXDEBUG__
    #define WXDEBUG_NEW new
#endif // __WXDEBUG__/!__WXDEBUG__


// ----------------------------------------------------------------------------
// macro name aliases for backward compatibility (with wx < 3.0.0)
// ----------------------------------------------------------------------------

#define DECLARE_DYNAMIC_CLASS(name)             wxDECLARE_DYNAMIC_CLASS(name)
#define DECLARE_DYNAMIC_CLASS_NO_ASSIGN(name)   wxDECLARE_DYNAMIC_CLASS_NO_ASSIGN(name)
#define DECLARE_DYNAMIC_CLASS_NO_COPY(name)     wxDECLARE_DYNAMIC_CLASS_NO_COPY(name)
#define DECLARE_ABSTRACT_CLASS(name)            wxDECLARE_ABSTRACT_CLASS(name)
#define DECLARE_CLASS(name)                     wxDECLARE_CLASS(name)
#define IMPLEMENT_DYNAMIC_CLASS_WITH_COPY(n,b)  wxIMPLEMENT_DYNAMIC_CLASS_WITH_COPY(n,b)
#define IMPLEMENT_DYNAMIC_CLASS(n,b)            wxIMPLEMENT_DYNAMIC_CLASS(n,b)
#define IMPLEMENT_DYNAMIC_CLASS2(n,b1,b2)       wxIMPLEMENT_DYNAMIC_CLASS2(n,b1,b2)
#define IMPLEMENT_ABSTRACT_CLASS(n,b)           wxIMPLEMENT_ABSTRACT_CLASS(n,b)
#define IMPLEMENT_ABSTRACT_CLASS2(n,b1,b2)      wxIMPLEMENT_ABSTRACT_CLASS2(n,b1,b2)
#define IMPLEMENT_CLASS(n,b)                    wxIMPLEMENT_CLASS(n,b)
#define IMPLEMENT_CLASS2(n,b1,b2)               wxIMPLEMENT_CLASS2(n,b1,b2)
#define CLASSINFO(name)                         wxCLASSINFO(name)

#define wxIS_KIND_OF(obj, className) obj->IsKindOf(&className::ms_classInfo)

// Just seems a bit nicer-looking (pretend it's not a macro)
#define wxIsKindOf(obj, className) obj->IsKindOf(&className::ms_classInfo)

// this cast does some more checks at compile time as it uses static_cast
// internally
//
// note that it still has different semantics from dynamic_cast<> and so can't
// be replaced by it as long as there are any compilers not supporting it
#define wxDynamicCast(obj, className) \
    ((className *) wxCheckDynamicCast( \
        wx_const_cast(wxObject *, wx_static_cast(const wxObject *, \
          wx_const_cast(className *, wx_static_cast(const className *, obj)))), \
        &className::ms_classInfo))

// The 'this' pointer is always true, so use this version
// to cast the this pointer and avoid compiler warnings.
#define wxDynamicCastThis(className) \
     (IsKindOf(&className::ms_classInfo) ? (className *)(this) : (className *)0)

#define wxConstCast(obj, className) wx_const_cast(className *, obj)

#ifdef __WXDEBUG__
inline void* wxCheckCast(void *ptr)
{
    wxASSERT_MSG( ptr, _T("wxStaticCast() used incorrectly") );
    return ptr;
}
#define wxStaticCast(obj, className) \
 ((className *)wxCheckCast(wxDynamicCast(obj, className)))

#else  // !__WXDEBUG__
#define wxStaticCast(obj, className) \
    wx_const_cast(className *, wx_static_cast(const className *, obj))

#endif  // __WXDEBUG__


#endif // _WX_OBJECTH__
