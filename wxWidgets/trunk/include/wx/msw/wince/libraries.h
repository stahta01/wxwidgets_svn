/////////////////////////////////////////////////////////////////////////////
// Name:        wx/msw/wince/libraries.h
// Purpose:     VC++ pragmas for linking against SDK libs
// Author:      Vaclav Slavik
// Modified by:
// Created:     2004-04-11
// RCS-ID:      $Id$
// Copyright:   (c) 2004 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_LIBRARIES_H_
#define _WX_LIBRARIES_H_

// NB: According to Microsoft, it is up to the OEM to decide whether 
//     some of libraries will be included in the system or not. For example, 
//     MS' STANDARDSDK does not include cyshell.lib and aygshell.lib, while
//     Pocket PC 2003 SDK does. We depend on some symbols that are in these
//     libraries in some SDKs and in different libs in others. Fortunately we
//     can detect what SDK is used in C++ code, so we take advantage of
//     VC++'s #pragma to link against the libraries conditionally, instead of
//     including libraries in project files.

#ifdef __VISUALC__

#if defined(WCE_PLATFORM_STANDARDSDK)
    // DoDragDrop:
    #pragma comment(lib,"olece400.lib")
#elif defined(WIN32_PLATFORM_PSPC)
    // DoDragDrop:
    #pragma comment(lib,"ceshell.lib")
#else
    #error "Unknown SDK, please fill-in missing pieces"
#endif

#endif // __VISUALC__

#endif // _WX_LIBRARIES_H_
