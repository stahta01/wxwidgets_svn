/////////////////////////////////////////////////////////////////////////////
// Name:         dynload.cpp
// Purpose:      Dynamic loading framework
// Author:       Ron Lee, David Falkinder, Vadim Zeitlin and a cast of 1000's
//               (derived in part from dynlib.cpp (c) 1998 Guilhem Lavaux)
// Modified by:
// Created:      03/12/01
// RCS-ID:       $Id$
// Copyright:    (c) 2001 Ron Lee <ron@debian.org>
// Licence:      wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "dynload.h"
#endif

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#if wxUSE_DYNAMIC_LOADER

#ifdef __WINDOWS__
#include "wx/msw/private.h"
#endif

#ifndef WX_PRECOMP
#include "wx/log.h"
#include "wx/intl.h"
#endif

#include "wx/dynload.h"


// ============================================================================
// implementation
// ============================================================================

// ---------------------------------------------------------------------------
// wxDynamicLibrary
// ---------------------------------------------------------------------------

//FIXME:  This class isn't really common at all, it should be moved into
//        platform dependent files.

#if defined(__WINDOWS__) || defined(__WXPM__) || defined(__EMX__)
const wxString wxDynamicLibrary::ms_dllext( _T(".dll") );
#elif defined(__UNIX__)
#if defined(__HPUX__)
const wxString wxDynamicLibrary::ms_dllext( _T(".sl") );
#else
const wxString wxDynamicLibrary::ms_dllext( _T(".so") );
#endif
#endif

wxDllType wxDynamicLibrary::GetProgramHandle()
{
#if defined( HAVE_DLOPEN ) && !defined(__EMX__)
   return dlopen(0, RTLD_LAZY);
#elif defined (HAVE_SHL_LOAD)
   return PROG_HANDLE;
#else
   wxFAIL_MSG( wxT("This method is not implemented under Windows or OS/2"));
   return 0;
#endif
}

bool wxDynamicLibrary::Load(wxString libname, int flags)
{
    wxASSERT_MSG(m_handle == 0, _T("Library already loaded."));

    if( !(flags & wxDL_VERBATIM) )
        libname += GetDllExt();

#if defined(__WXMAC__) && !defined(__UNIX__)
    FSSpec      myFSSpec;
    Ptr         myMainAddr;
    Str255      myErrName;

    wxMacFilename2FSSpec( libname , &myFSSpec );

    if( GetDiskFragment( &myFSSpec,
                         0,
                         kCFragGoesToEOF,
                         "\p",
                         kPrivateCFragCopy,
                         &m_handle,
                         &myMainAddr,
                         myErrName ) != noErr )
    {
        p2cstr( myErrName );
        wxLogSysError( _("Failed to load shared library '%s' Error '%s'"),
                       libname.c_str(),
                       (char*)myErrName );
        m_handle = 0;
    }

#elif defined(__WXPM__) || defined(__EMX__)
    char    err[256] = "";
    DosLoadModule(err, sizeof(err), libname.c_str(), &m_handle)

#elif defined(HAVE_DLOPEN)

#ifdef __VMS
    m_handle = dlopen(libname.c_str(), 0);  // The second parameter is ignored
#else
    int rtldFlags = 0;

    if( flags & wxDL_LAZY )
    {
        wxASSERT_MSG( (flags & wxDL_NOW) == 0,
                      _T("wxDL_LAZY and wxDL_NOW are mutually exclusive.") );
        rtldFlags |= RTLD_LAZY;
    }
    else if( flags & wxDL_NOW )
    {
        rtldFlags |= RTLD_NOW;
    }
    if( flags & wxDL_GLOBAL )
    {
#ifdef __osf__
        wxLogDebug(_T("WARNING: RTLD_GLOBAL is not a supported on this platform."));
#endif
        rtldFlags |= RTLD_GLOBAL;
    }

    m_handle = dlopen(libname.c_str(), rtldFlags);
#endif  // __VMS

#elif defined(HAVE_SHL_LOAD)
    int shlFlags = 0;

    if( flags & wxDL_LAZY )
    {
        wxASSERT_MSG( (flags & wxDL_NOW) == 0,
                      _T("wxDL_LAZY and wxDL_NOW are mutually exclusive.") );
        shlFlags |= BIND_DEFERRED;
    }
    else if( flags & wxDL_NOW )
    {
        shlFlags |= BIND_IMMEDIATE;
    }
    m_handle = shl_load(libname.c_str(), BIND_DEFERRED, 0);

#elif defined(__DARWIN__)
    NSObjectFileImage   ofile;
    int                 dyld_result = NSCreateObjectFileImageFromFile(libname.c_str(), &ofile);

    if (dyld_result != NSObjectFileImageSuccess)
    {
        TranslateError(libname.c_str(), OFImage, dyld_result);
    }
    else
    {
        // NSLinkModule will cause the run to abort on any link error's
        // not very friendly but the error recovery functionality is limited.
        m_handle = NSLinkModule(ofile, libname.c_str(), TRUE);
    }

#elif defined(__WINDOWS__)
    m_handle = ::LoadLibrary(libname.c_str());

#else
#error  "runtime shared lib support not implemented"
#endif

    if ( m_handle == 0 )
    {
        wxString msg(_("Failed to load shared library '%s'"));
#ifdef HAVE_DLERROR
        const wxChar  *err = dlerror();
        if( err )
            wxLogError( msg, err );
#else
        wxLogSysError( msg, libname.c_str() );
#endif
    }

    return IsLoaded();
}

void wxDynamicLibrary::Unload()
{
    if( IsLoaded() )
    {
#if defined(__WXPM__) || defined(__EMX__)
        DosFreeModule( m_handle );
#elif defined(HAVE_DLOPEN)
        dlclose( m_handle );
#elif defined(HAVE_SHL_LOAD)
        shl_unload( m_handle );
#elif defined(__WINDOWS__)
        ::FreeLibrary( m_handle );
#elif defined(__WXMAC__)
        CloseConnection( &m_handle );
#else
#error  "runtime shared lib support not implemented"
#endif
        m_handle = 0;
    }
}

void *wxDynamicLibrary::GetSymbol(const wxString &name, bool *success) const
{
    wxCHECK_MSG( IsLoaded(), NULL,
                 _T("Can't load symbol from unloaded library") );

    bool     failed = FALSE;
    void    *symbol = 0;

#if defined(__WXMAC__) && !defined(__UNIX__)
    Ptr                 symAddress;
    CFragSymbolClass    symClass;
    Str255              symName;
#if TARGET_CARBON
    c2pstrcpy( (StringPtr) symName, name );
#else
    strcpy( (char *)symName, name );
    c2pstr( (char *)symName );
#endif
    if( FindSymbol( dllHandle, symName, &symAddress, &symClass ) == noErr )
        symbol = (void *)symAddress;

#elif defined(__WXPM__) || defined(__EMX__)
    DosQueryProcAddr( m_handle, 1L, name.c_str(), (PFN*)symbol );

#elif defined(HAVE_DLOPEN)
    symbol = dlsym( m_handle, name.c_str() );

#elif defined(HAVE_SHL_LOAD)
    if( shl_findsym( &m_handle, name.c_str(), TYPE_UNDEFINED, &symbol ) != 0 )
        symbol = 0;

#elif defined(__DARWIN__)
    if( NSIsSymbolNameDefined( name.c_str() ) )
        symbol = NSAddressOfSymbol( NSLookupAndBindSymbol( name.c_str() ) );

#elif defined(__WINDOWS__)
    symbol = ::GetProcAddress( m_handle, name.mb_str() );

#else
#error  "runtime shared lib support not implemented"
#endif

    if ( !symbol )
    {
        wxString msg(_("wxDynamicLibrary failed to GetSymbol '%s'"));
#ifdef HAVE_DLERROR
        const wxChar *err = dlerror();
        if( err )
        {
            failed = TRUE;
            wxLogError( msg, err );
        }
#else
        failed = TRUE;
        wxLogSysError(_("Couldn't find symbol '%s' in a dynamic library"),
                      name.c_str());
#endif
    }
    if( success )
        *success = !failed;

    return symbol;
}


// ---------------------------------------------------------------------------
// wxPluginLibrary
// ---------------------------------------------------------------------------


wxDLImports  wxPluginLibrary::ms_classes(wxKEY_STRING);

wxPluginLibrary::wxPluginLibrary(const wxString &libname, int flags)
        : m_linkcount(1)
        , m_objcount(0)
{
    m_before = wxClassInfo::sm_first;
    Load( libname, flags );
    m_after = wxClassInfo::sm_first;

    if( m_handle != 0 )
    {
        UpdateClassInfo();
        RegisterModules();
    }
    else
        --m_linkcount;      // Flag us for deletion
}

wxPluginLibrary::~wxPluginLibrary()
{
    if( m_handle != 0 )
    {
        UnregisterModules();
        RestoreClassInfo();
    }
}

bool wxPluginLibrary::UnrefLib()
{
    wxASSERT_MSG( m_objcount == 0, _T("Library unloaded before all objects were destroyed") );
    if( m_linkcount == 0 || --m_linkcount == 0 )
    {
        delete this;
        return TRUE;
    }
    return FALSE;
}

// ------------------------
// Private methods
// ------------------------

void wxPluginLibrary::UpdateClassInfo()
{
    wxClassInfo     *info;
    wxHashTable     *t = wxClassInfo::sm_classTable;

        // FIXME: Below is simply a cut and paste specialisation of
        //        wxClassInfo::InitializeClasses.  Once this stabilises,
        //        the two should probably be merged.
        //
        //        Actually it's becoming questionable whether we should merge
        //        this info with the main ClassInfo tables since we can nearly
        //        handle this completely internally now and it does expose
        //        certain (minimal % user_stupidy) risks.

    for(info = m_after; info != m_before; info = info->m_next)
    {
        if( info->m_className )
        {
            if( t->Get(info->m_className) == 0 )
                t->Put(info->m_className, (wxObject *)info);

            // Hash all the class names into a local table too so
            // we can quickly find the entry they correspond to.

            if( ms_classes.Get(info->m_className) == 0 )
                ms_classes.Put(info->m_className, (wxObject *) this);
        }
    }

    for(info = m_after; info != m_before; info = info->m_next)
    {
        if( info->m_baseClassName1 )
            info->m_baseInfo1 = (wxClassInfo *)t->Get(info->m_baseClassName1);
        if( info->m_baseClassName2 )
            info->m_baseInfo2 = (wxClassInfo *)t->Get(info->m_baseClassName2);
    }
}

void wxPluginLibrary::RestoreClassInfo()
{
    wxClassInfo *info;

    for(info = m_after; info != m_before; info = info->m_next)
    {
        wxClassInfo::sm_classTable->Delete(info->m_className);
        ms_classes.Delete(info->m_className);
    }

    if( wxClassInfo::sm_first == m_after )
        wxClassInfo::sm_first = m_before;
    else
    {
        info = wxClassInfo::sm_first;
        while( info->m_next && info->m_next != m_after ) info = info->m_next;

        wxASSERT_MSG( info, _T("ClassInfo from wxPluginLibrary not found on purge"))

        info->m_next = m_before;
    }
}

void wxPluginLibrary::RegisterModules()
{
    // Plugin libraries might have wxModules, Register and initialise them if
    // they do.
    //
    // Note that these classes are NOT included in the reference counting since
    // it's implicit that they will be unloaded if and when the last handle to
    // the library is.  We do have to keep a copy of the module's pointer
    // though, as there is currently no way to Unregister it without it.

    wxASSERT_MSG( m_linkcount == 1,
                  _T("RegisterModules should only be called for the first load") );

    for(wxClassInfo *info = m_after; info != m_before; info = info->m_next)
    {
        if( info->IsKindOf(CLASSINFO(wxModule)) )
        {
            wxModule *m = wxDynamicCast(info->CreateObject(), wxModule);

            wxASSERT_MSG( m, _T("wxDynamicCast of wxModule failed") );

            m_wxmodules.Append(m);
            wxModule::RegisterModule(m);
        }
    }

    // FIXME: Likewise this is (well was) very similar to InitializeModules()

    for(wxModuleList::Node *node = m_wxmodules.GetFirst(); node; node->GetNext())
    {
        if( !node->GetData()->Init() )
        {
            wxLogDebug(_T("wxModule::Init() failed for wxPluginLibrary"));

            // XXX: Watch this, a different hash implementation might break it,
            //      a good hash implementation would let us fix it though.

            // The name of the game is to remove any uninitialised modules and
            // let the dtor Exit the rest on shutdown, (which we'll initiate
            // shortly).

            wxModuleList::Node *oldNode = 0;
            do {
                node = node->GetNext();
                delete oldNode;
                wxModule::UnregisterModule( node->GetData() );
                oldNode = node;
            } while( node );

            --m_linkcount;     // Flag us for deletion
            break;
        }
    }
}

void wxPluginLibrary::UnregisterModules()
{
    wxModuleList::Node  *node;

    for(node = m_wxmodules.GetFirst(); node; node->GetNext())
        node->GetData()->Exit();

    for(node = m_wxmodules.GetFirst(); node; node->GetNext())
        wxModule::UnregisterModule( node->GetData() );

    m_wxmodules.DeleteContents(TRUE);
}


// ---------------------------------------------------------------------------
// wxPluginLibrary
// ---------------------------------------------------------------------------

wxDLManifest   wxPluginManager::ms_manifest(wxKEY_STRING);

// ------------------------
// Static accessors
// ------------------------

wxPluginLibrary *wxPluginManager::LoadLibrary(const wxString &libname, int flags)
{
    wxString realname(libname);

    if( !(flags & wxDL_VERBATIM) )
        realname += wxDynamicLibrary::GetDllExt();

    wxPluginLibrary *entry = (wxPluginLibrary*) ms_manifest.Get(realname);

    if( entry != 0 )
    {
        entry->RefLib();
    }
    else
    {
        entry = new wxPluginLibrary( libname, flags );

        if( entry->IsLoaded() )
        {
            ms_manifest.Put(realname, (wxObject*) entry);
        }
        else
        {
            wxCHECK_MSG( entry->UnrefLib(), 0,
                         _T("Currently linked library is, ..not loaded??") );
            entry = 0;
        }
    }
    return entry;
}

bool wxPluginManager::UnloadLibrary(const wxString &libname)
{
    wxPluginLibrary *entry = (wxPluginLibrary*) ms_manifest.Get(libname);

    if( !entry )
        entry = (wxPluginLibrary*) ms_manifest.Get(libname + wxDynamicLibrary::GetDllExt());

    if( entry )
        return entry->UnrefLib();

    wxLogDebug(_T("Attempt to Unlink library '%s' (which is not linked)."), libname.c_str());
    return FALSE;
}

#if WXWIN_COMPATIBILITY_2_2
wxPluginLibrary *wxPluginManager::GetObjectFromHandle(wxDllType handle)
{
    wxNode  *node;
    ms_manifest.BeginFind();

    for(node = ms_manifest.Next(); node; node = ms_manifest.Next())
        if( ((wxPluginLibrary*)node->GetData())->GetLibHandle() == handle )
            return (wxPluginLibrary*)node->GetData();

    return 0;
}
#endif

// ------------------------
// Class implementation
// ------------------------

bool wxPluginManager::Load(const wxString &libname, int flags)
{
    m_entry = wxPluginManager::LoadLibrary(libname, flags);
    return IsLoaded();
}

void wxPluginManager::Unload()
{
    wxNode  *node;
    ms_manifest.BeginFind();

    // It's either this or store the name of the lib just to do this.

    for(node = ms_manifest.Next(); node; node = ms_manifest.Next())
        if( (wxPluginLibrary*)node->GetData() == m_entry )
            break;

    if( m_entry && m_entry->UnrefLib() )
    {
        delete node;
        m_entry = 0;
    }
}


#ifdef __DARWIN__
// ---------------------------------------------------------------------------
// For Darwin/Mac OS X
//   supply the sun style dlopen functions in terms of Darwin NS*
// ---------------------------------------------------------------------------

extern "C" {
#import <mach-o/dyld.h>
};

enum dyldErrorSource
{
    OFImage,
};

static char dl_last_error[1024];

static void TranslateError(const char *path, enum dyldErrorSource type, int number)
{
    unsigned int index;
    static char *OFIErrorStrings[] =
    {
	"%s(%d): Object Image Load Failure\n",
	"%s(%d): Object Image Load Success\n",
	"%s(%d): Not an recognisable object file\n",
	"%s(%d): No valid architecture\n",
	"%s(%d): Object image has an invalid format\n",
	"%s(%d): Invalid access (permissions?)\n",
	"%s(%d): Unknown error code from NSCreateObjectFileImageFromFile\n",
    };
#define NUM_OFI_ERRORS (sizeof(OFIErrorStrings) / sizeof(OFIErrorStrings[0]))

    switch (type)
    {
     case OFImage:
	 index = number;
	 if (index > NUM_OFI_ERRORS - 1) {
	     index = NUM_OFI_ERRORS - 1;
	 }
	 sprintf(dl_last_error, OFIErrorStrings[index], path, number);
	 break;
	 
     default:
	 sprintf(dl_last_error, "%s(%d): Totally unknown error type %d\n",
		 path, number, type);
	 break;
    }
}

#endif // __DARWIN__


// ---------------------------------------------------------------------------
// wxDllLoader   (all these methods are static)
// ---------------------------------------------------------------------------

#if WXWIN_COMPATIBILITY_2_2

wxDllType wxDllLoader::LoadLibrary(const wxString &name)
{
    wxPluginLibrary *p = wxPluginManager::LoadLibrary(name, wxDL_DEFAULT | wxDL_VERBATIM);
    return p->GetLibHandle();
}

void wxDllLoader::UnloadLibrary(wxDllType handle)
{
    wxPluginLibrary *p = wxPluginManager::GetObjectFromHandle(handle);
    p->UnrefLib();
}

void *wxDllLoader::GetSymbol(wxDllType dllHandle, const wxString &name, bool *success)
{
    wxPluginLibrary *p = wxPluginManager::GetObjectFromHandle(dllHandle);
    return p->GetSymbol(name, success);
}

#endif  // WXWIN_COMPATIBILITY_2_2

#endif  // wxUSE_DYNAMIC_LOADER

// vi:sts=4:sw=4:et
