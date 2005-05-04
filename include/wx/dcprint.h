/////////////////////////////////////////////////////////////////////////////
// Name:        wx/dcprint.h
// Purpose:     wxPrinterDC base header
// Author:      Julian Smart
// Modified by:
// Created:
// Copyright:   (c) Julian Smart
// RCS-ID:      $Id$
// Licence:     wxWindows Licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_DCPRINT_H_BASE_
#define _WX_DCPRINT_H_BASE_

#if defined(__WXMSW__)
#include "wx/msw/dcprint.h"
#endif
#if defined(__WXPM__)
#include "wx/os2/dcprint.h"
#endif
#if defined(__WXMAC__)
#include "wx/mac/dcprint.h"
#endif

#endif
    // _WX_DCPRINT_H_BASE_
