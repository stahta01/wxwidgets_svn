/////////////////////////////////////////////////////////////////////////////
// Name:        wxprec.h
// Purpose:     Includes the appropriate files for precompiled headers
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id$
// Copyright:   (c)
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// check if to use precompiled headers
#if (defined(__BORLANDC__) || defined(_MSC_VER) || defined(__WATCOMC__) || defined(__GNUWIN32__)) && defined(__WXMSW__)
#if !NOPCH
#define WX_PRECOMP
#endif
#endif

// For some reason, this must be defined for common dialogs to work.
#ifdef __WATCOMC__
#define INCLUDE_COMMDLG_H	1
#endif

// include the wx definitions
#ifdef WX_PRECOMP
#include "wx/wx.h"

// Comment this out if you don't mind slower compilation of the wxWindows
// library
#include <windows.h>

#ifdef GetClassInfo
#undef GetClassInfo
#endif

#ifdef GetClassName
#undef GetClassName
#endif

#ifdef DrawText
#undef DrawText
#endif

#ifdef GetCharWidth
#undef GetCharWidth
#endif

#ifdef StartDoc
#undef StartDoc
#endif

#ifdef FindWindow
#undef FindWindow
#endif

#ifdef FindResource
#undef FindResource
#endif

#endif

