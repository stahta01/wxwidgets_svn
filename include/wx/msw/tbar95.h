/////////////////////////////////////////////////////////////////////////////
// Name:        tbar95.h
// Purpose:     wxToolBar95 (Windows 95 toolbar) class
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_TBAR95_H_
#define _WX_TBAR95_H_

#ifdef __GNUG__
#pragma interface "tbar95.h"
#endif

#if wxUSE_TOOLBAR
#include "wx/tbarbase.h"

WXDLLEXPORT_DATA(extern const wxChar*) wxToolBarNameStr;

class WXDLLEXPORT wxToolBar95: public wxToolBarBase
{
  DECLARE_DYNAMIC_CLASS(wxToolBar95)
 public:
  /*
   * Public interface
   */

  wxToolBar95();

  wxToolBar95(wxWindow *parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
            long style = wxNO_BORDER|wxTB_HORIZONTAL,
            const wxString& name = wxToolBarNameStr)
  {
    Create(parent, id, pos, size, style, name);
  }
  ~wxToolBar95();

  bool Create(wxWindow *parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
            long style = wxNO_BORDER|wxTB_HORIZONTAL,
            const wxString& name = wxToolBarNameStr);

  // Call default behaviour
  void OnMouseEvent(wxMouseEvent& event);

  // Handle wxToolBar95 events

  // If pushedBitmap is NULL, a reversed version of bitmap is
  // created and used as the pushed/toggled image.
  // If toggle is TRUE, the button toggles between the two states.
  wxToolBarTool *AddTool(int toolIndex, const wxBitmap& bitmap, const wxBitmap& pushedBitmap = wxNullBitmap,
               bool toggle = FALSE, long xPos = -1, long yPos = -1, wxObject *clientData = NULL,
               const wxString& helpString1 = "", const wxString& helpString2 = "");

  // Set default bitmap size
  void SetToolBitmapSize(const wxSize& size);
  void EnableTool(int toolIndex, bool enable); // additional drawing on enabling
  void ToggleTool(int toolIndex, bool toggle); // toggle is TRUE if toggled on
  void ClearTools();

  // The button size is bigger than the bitmap size
  wxSize GetToolSize() const;

  wxSize GetMaxSize() const;

  virtual bool GetToolState(int toolIndex) const;

  // Add all the buttons: required for Win95.
  virtual bool CreateTools();
  virtual void SetRows(int nRows);
  virtual void LayoutButtons() {}

  // The post-tool-addition call
  bool Realize() { return CreateTools(); };

  // IMPLEMENTATION
  virtual bool MSWCommand(WXUINT param, WXWORD id);
  virtual bool MSWOnNotify(int idCtrl, WXLPARAM lParam, WXLPARAM *result);

  // Responds to colour changes
  void OnSysColourChanged(wxSysColourChangedEvent& event);

protected:
  WXHBITMAP         m_hBitmap;

DECLARE_EVENT_TABLE()
};

#endif // wxUSE_TOOLBAR

#endif
    // _WX_TBAR95_H_
