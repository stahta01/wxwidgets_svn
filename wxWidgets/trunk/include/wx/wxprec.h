/////////////////////////////////////////////////////////////////////////////
// Name:        wxprec.h
// Purpose:     Includes the appropriate files for precompiled headers
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id$
// Copyright:   (c)
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/setup.h"   // always include this file (under Unix it's generated by configure)
#include "wx/defs.h"    // compiler definitions

// check if to use precompiled headers
#if (defined(__BORLANDC__) || defined(__VISUALC__) || defined(__WATCOMC__)) && defined(__WXMSW__)
    #if !defined(NOPCH)
        #define WX_PRECOMP
    #endif
#endif

// For some reason, this must be defined for common dialogs to work.
#ifdef __WATCOMC__
#define INCLUDE_COMMDLG_H  1
#endif

// include the wx definitions
#ifdef WX_PRECOMP

#ifdef __WXMSW__
#include <windows.h>
#include "wx/msw/winundef.h"
#endif

#include "wx/wx.h"


#endif // WX_PRECOMP

