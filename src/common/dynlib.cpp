/////////////////////////////////////////////////////////////////////////////
// Name:        dynlib.cpp
// Purpose:     Dynamic library management
// Author:      Guilhem Lavaux
// Modified by:
// Created:     20/07/98
// RCS-ID:      $Id$
// Copyright:   (c) Guilhem Lavaux
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#ifdef __GNUG__
    #pragma implementation "dynlib.h"
#endif

#include  "wx/wxprec.h"

#ifdef    __BORLANDC__
  #pragma hdrstop
#endif  //__BORLANDC__

// TODO should be done by configure
#if defined(__UNIX__) && !(defined(HAVE_DLOPEN) || defined(HAVE_SHLLOAD))
    #if defined(__LINUX__) || defined(__SOLARIS__) || defined(__SUNOS__)
        #ifndef HAVE_DLOPEN
            #define HAVE_DLOPEN
        #endif
    #elif defined(__HPUX__)
        #ifndef HAVE_SHLLOAD
            #define HAVE_SHLLOAD
        #endif
    #endif // Unix flavour
#endif // !Unix or already have some HAVE_xxx defined

#include "wx/dynlib.h"
#include "wx/filefn.h"
#include "wx/intl.h"
#include "wx/log.h"

// ----------------------------------------------------------------------------
// conditional compilation
// ----------------------------------------------------------------------------

#if defined(HAVE_DLOPEN)
    #define wxDllOpen(lib)                dlopen(lib, RTLD_LAZY)
    #define wxDllGetSymbol(handle, name)  dlsym(handle, (char *)name)
    #define wxDllClose                    dlclose
#elif defined(HAVE_SHLLOAD)
    #define wxDllOpen(lib)                shl_open(lib, BIND_DEFERRED, 0)
    #define wxDllClose      shl_unload

    static inline void *wxDllGetSymbol(shl_t *handle, const char *name)
    {
        void *sym;
        if ( shl_findsym(handle, name, TYPE_UNDEFINED, &sym) == 0 )
            return sym;
        else
            return (void *)0;
    }
#elif defined(__WINDOWS__)
    #define wxDllOpen(lib)                  ::LoadLibrary(lib)
    #define wxDllGetSymbol(handle, name)    ::GetProcAddress(handle, name)
    #define wxDllClose                      ::FreeLibrary
#else
    #error "Don't know how to load shared libraries on this platform."
#endif // OS

// ---------------------------------------------------------------------------
// Global variables
// ---------------------------------------------------------------------------

wxLibraries wxTheLibraries;

// ----------------------------------------------------------------------------
// private functions
// ----------------------------------------------------------------------------

// construct the full name from the base shared object name: adds a .dll
// suffix under Windows or .so under Unix
static wxString ConstructLibraryName(const wxString& basename)
{
    wxString fullname(basename);

#if defined(__UNIX__)
    fullname << ".so";
#elif defined(__WINDOWS__)
    fullname << ".dll";
#endif

    return fullname;
}

// ============================================================================
// implementation
// ============================================================================

// ---------------------------------------------------------------------------
// wxLibrary (one instance per dynamic library)
// ---------------------------------------------------------------------------

wxLibrary::wxLibrary(wxDllType handle)
{
    typedef wxClassInfo *(*t_get_first)(void);
    t_get_first get_first;

    m_handle = handle;

    // Some system may use a local heap for library.
    get_first = (t_get_first)GetSymbol("wxGetClassFirst");
    // It is a wxWindows DLL.
    if (get_first)
        PrepareClasses(get_first());
}

wxLibrary::~wxLibrary()
{
    if ( m_handle )
    {
        wxDllClose(m_handle);
    }
}

wxObject *wxLibrary::CreateObject(const wxString& name)
{
    wxClassInfo *info = (wxClassInfo *)classTable.Get(name);

    if (!info)
        return NULL;

    return info->CreateObject();
}

void wxLibrary::PrepareClasses(wxClassInfo *first)
{
    // Index all class infos by their class name
    wxClassInfo *info = first;
    while (info)
    {
        if (info->m_className)
            classTable.Put(info->m_className, (wxObject *)info);
        info = info->GetNext();
    }

    // Set base pointers for each wxClassInfo
    info = first;
    while (info)
    {
        if (info->GetBaseClassName1())
            info->m_baseInfo1 = (wxClassInfo *)classTable.Get(info->GetBaseClassName1());
        if (info->GetBaseClassName2())
            info->m_baseInfo2 = (wxClassInfo *)classTable.Get(info->GetBaseClassName2());
        info = info->m_next;
    }
}

void *wxLibrary::GetSymbol(const wxString& symbname)
{
    void *symbol = NULL;    // return value

#if defined( __WXMAC__ )
    Ptr symAddress ;
    CFragSymbolClass symClass ;
    Str255	symName ;

    strcpy( (char*) symName , symbname ) ;
    c2pstr( (char*) symName ) ;

    if ( FindSymbol( m_handle , symName , &symAddress , &symClass ) == noErr )
    {
        symbol = (void *)symAddress ; 
    }
#else
    // VZ: hmm... why is WXSTRINGCAST needed? if it's really modified, we
    //     should make a copy of it
    symbol = wxDllGetSymbol(m_handle, WXSTRINGCAST symbname);
#endif

    if ( !symbol )
    {
        wxLogSysError(_("Couldn't find symbol '%s' in a dynamic library"),
                      symbname.c_str());
    }

    return symbol;
}

// ---------------------------------------------------------------------------
// wxLibraries (only one instance should normally exist)
// ---------------------------------------------------------------------------

wxLibraries::wxLibraries()
{
}

wxLibraries::~wxLibraries()
{
    wxNode *node = m_loaded.First();

    while (node) {
        wxLibrary *lib = (wxLibrary *)node->Data();
        delete lib;

        node = node->Next();
    }
}

wxLibrary *wxLibraries::LoadLibrary(const wxString& name)
{
    wxNode *node;
    wxLibrary *lib;
    wxClassInfo *old_sm_first;

    if ( (node = m_loaded.Find(name.GetData())) )
        return ((wxLibrary *)node->Data());

    // If DLL shares data, this is necessary.
    old_sm_first = wxClassInfo::sm_first;
    wxClassInfo::sm_first = NULL;

    wxString lib_name = ConstructLibraryName(name);

#if defined(__UNIX__)
    // TODO use LD_LIBRARY_PATH!
    lib_name.Prepend("/lib");
#endif // __UNIX__

    wxDllType handle ;

#if defined(__WXMAC__)
    FSSpec myFSSpec ;
    Ptr	myMainAddr ;
    Str255	myErrName ;

    wxMacPathToFSSpec( lib_name , &myFSSpec ) ;
    if (GetDiskFragment( &myFSSpec , 0 , kCFragGoesToEOF , "\p" , kPrivateCFragCopy , &handle , &myMainAddr ,
                myErrName ) != noErr )
    {
        p2cstr( myErrName ) ;
        wxASSERT_MSG( 1 , (char*)myErrName ) ;
        return NULL ;
    }
#else // !Mac
    handle = wxDllOpen(lib_name);
#endif // OS

    if ( !handle )
    {
        wxLogSysError(_("Failed to load shared library '%s'"),
                      lib_name.c_str());

        return NULL;
    }

    lib = new wxLibrary(handle);

    wxClassInfo::sm_first = old_sm_first;

    m_loaded.Append(name.GetData(), lib);

    return lib;
}

wxObject *wxLibraries::CreateObject(const wxString& path)
{
    wxNode *node = m_loaded.First();
    wxObject *obj;

    while (node) {
        obj = ((wxLibrary *)node->Data())->CreateObject(path);
        if (obj)
            return obj;

        node = node->Next();
    }
    return NULL;
}
