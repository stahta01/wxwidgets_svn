/////////////////////////////////////////////////////////////////////////////
// Name:        dcscreen.h
// Purpose:     wxScreenDC class
// Author:      Julian Smart
// Modified by:
// Created:     17/09/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_DCSCREEN_H_
#define _WX_DCSCREEN_H_

#ifdef __GNUG__
#pragma interface "dcscreen.h"
#endif

#include "wx/dcclient.h"

class WXDLLEXPORT wxScreenDC: public wxPaintDC
{
  DECLARE_DYNAMIC_CLASS(wxScreenDC)

 public:
  // Create a DC representing the whole screen
  wxScreenDC();
  ~wxScreenDC();

  // Compatibility with X's requirements for
  // drawing on top of all windows
  static bool StartDrawingOnTop(wxWindow* WXUNUSED(window)) { return TRUE; }
  static bool StartDrawingOnTop(wxRectangle* WXUNUSED(rect) = NULL) { return TRUE; }
  static bool EndDrawingOnTop() { return TRUE; }
};

#endif
    // _WX_DCSCREEN_H_

