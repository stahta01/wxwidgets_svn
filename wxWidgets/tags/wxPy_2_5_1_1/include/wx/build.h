///////////////////////////////////////////////////////////////////////////////
// Name:        wx/build.h
// Purpose:     Runtime build options checking
// Author:      Vadim Zeitlin, Vaclav Slavik
// Modified by:
// Created:     07.05.02
// RCS-ID:      $Id$
// Copyright:   (c) 2002 Vadim Zeitlin <vadim@wxwindows.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_BUILD_H_
#define _WX_BUILD_H_

#include "wx/version.h"

// ----------------------------------------------------------------------------
// WX_BUILD_OPTIONS_SIGNATURE
// ----------------------------------------------------------------------------

#define __WX_BO_STRINGIZE(x)   __WX_BO_STRINGIZE0(x)
#define __WX_BO_STRINGIZE0(x)  #x

#if (wxMINOR_VERSION % 2) == 0
    #define __WX_BO_VERSION(x,y,z) \
        __WX_BO_STRINGIZE(x) "." __WX_BO_STRINGIZE(y)
#else
    #define __WX_BO_VERSION(x,y,z) \
        __WX_BO_STRINGIZE(x) "." __WX_BO_STRINGIZE(y) "." __WX_BO_STRINGIZE(z)
#endif

#ifdef __WXDEBUG__
    #define __WX_BO_DEBUG "debug"
#else
    #define __WX_BO_DEBUG "no debug"
#endif

#if wxUSE_UNICODE
    #define __WX_BO_UNICODE "Unicode"
#else
    #define __WX_BO_UNICODE "ANSI"
#endif

// GCC and Intel C++ share same C++ ABI, check if compiler versions are
// compatible:
#if (defined(__GNUG__) || defined(__INTEL_COMPILER) && \
     defined(__GXX_ABI_VERSION))
    #define __WX_BO_COMPILER \
            ",compiler with C++ ABI " __WX_BO_STRINGIZE(__GXX_ABI_VERSION)
#else
    #define __WX_BO_COMPILER
#endif

// WXWIN_COMPATIBILITY macros affect presence of virtual functions
#if WXWIN_COMPATIBILITY_2_2
    #define __WX_BO_WXWIN_COMPAT_2_2 ",compatible with 2.2"
#else
    #define __WX_BO_WXWIN_COMPAT_2_2
#endif
#if WXWIN_COMPATIBILITY_2_4
    #define __WX_BO_WXWIN_COMPAT_2_4 ",compatible with 2.4"
#else
    #define __WX_BO_WXWIN_COMPAT_2_4
#endif

// deriving wxWin containers from STL ones changes them completely:
#if wxUSE_STL
    #define __WX_BO_STL ",STL containers"
#else
    #define __WX_BO_STL ",wx containers"
#endif
 
// This macro is passed as argument to wxConsoleApp::CheckBuildOptions()
#define WX_BUILD_OPTIONS_SIGNATURE \
    __WX_BO_VERSION(wxMAJOR_VERSION, wxMINOR_VERSION, wxRELEASE_NUMBER) \
    " (" __WX_BO_DEBUG "," __WX_BO_UNICODE \
     __WX_BO_COMPILER \
     __WX_BO_STL \
     __WX_BO_WXWIN_COMPAT_2_2 __WX_BO_WXWIN_COMPAT_2_4 \
     ")"


// ----------------------------------------------------------------------------
// WX_CHECK_BUILD_OPTIONS
// ----------------------------------------------------------------------------

// Use this macro to check build options. Adding it to a file in DLL will
// ensure that the DLL checks build options in same way IMPLEMENT_APP() does.
#define WX_CHECK_BUILD_OPTIONS(libName)                                 \
    static bool wxCheckBuildOptions()                                   \
    {                                                                   \
        wxAppConsole::CheckBuildOptions(WX_BUILD_OPTIONS_SIGNATURE,     \
                                        libName);                       \
        return true;                                                    \
    };                                                                  \
    static bool gs_buildOptionsCheck = wxCheckBuildOptions();


#if WXWIN_COMPATIBILITY_2_4

// ----------------------------------------------------------------------------
// wxBuildOptions
// ----------------------------------------------------------------------------

// NB: Don't use this class in new code, it relies on the ctor being always
//     inlined. WX_BUILD_OPTIONS_SIGNATURE always works.
class wxBuildOptions
{
public:
    // the ctor must be inline to get the compilation settings of the code
    // which included this header
    wxBuildOptions() : m_signature(WX_BUILD_OPTIONS_SIGNATURE) {}

private:
    const char *m_signature;

    // actually only CheckBuildOptions() should be our friend but well...
    friend class wxAppConsole;
};

#endif // WXWIN_COMPATIBILITY_2_4

#endif // _WX_BUILD_H_
