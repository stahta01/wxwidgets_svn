/////////////////////////////////////////////////////////////////////////////
// Name:        tbarsmpl.h
// Purpose:     wxToolBarSimple class
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __TBARSMPLH__
#define __TBARSMPLH__

#ifdef __GNUG__
#pragma interface "tbarsmpl.h"
#endif

#include "wx/defs.h"

#if USE_TOOLBAR

#include "wx/bitmap.h"
#include "wx/list.h"
#include "wx/tbarbase.h"

WXDLLEXPORT_DATA(extern const char*) wxToolBarNameStr;
WXDLLEXPORT_DATA(extern const wxSize) wxDefaultSize;
WXDLLEXPORT_DATA(extern const wxPoint) wxDefaultPosition;

// XView can't cope properly with panels that behave like canvases
// (e.g. no scrollbars in panels)
class WXDLLEXPORT wxToolBarSimple : public wxToolBarBase
{
  DECLARE_DYNAMIC_CLASS(wxToolBarSimple)

 public:

  wxToolBarSimple(void);
#if WXWIN_COMPATIBILITY > 0
  inline wxToolBarSimple(wxWindow *parent, int x, int y, int w, int h,
            long style = wxNO_BORDER, int orientation = wxVERTICAL, int RowsOrColumns = 1,
            const char *name = wxToolBarNameStr)
  {
    Create(parent, -1, wxPoint(x, y), wxSize(w, h), style, orientation, RowsOrColumns, name);
  }
#endif
  inline wxToolBarSimple(wxWindow *parent, const wxWindowID id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
            const long style = wxNO_BORDER, const int orientation = wxVERTICAL,
            const int RowsOrColumns = 1, const wxString& name = wxToolBarNameStr)
  {
    Create(parent, id, pos, size, style, orientation, RowsOrColumns, name);
  }
  ~wxToolBarSimple(void);

  bool Create(wxWindow *parent, const wxWindowID id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
            const long style = wxNO_BORDER, const int orientation = wxVERTICAL, const int RowsOrColumns = 1, const wxString& name = wxToolBarNameStr);

  void OnPaint(wxPaintEvent& event);
  void OnSize(wxSizeEvent& event);
  void OnMouseEvent(wxMouseEvent& event);
  void OnKillFocus(wxFocusEvent& event);

  // Handle wxToolBar events

  virtual void DrawTool(wxDC& dc, wxMemoryDC& memDC, wxToolBarTool *tool);
  virtual void ToggleTool(const int toolIndex, const bool toggle); // toggle is TRUE if toggled on

  virtual void SpringUpButton(const int index);

  DECLARE_EVENT_TABLE()
};

#endif // USE_TOOLBAR
#endif
    // __TBARSMPLH__

