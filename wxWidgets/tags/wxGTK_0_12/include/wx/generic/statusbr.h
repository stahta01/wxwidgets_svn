/////////////////////////////////////////////////////////////////////////////
// Name:        statusbr.h
// Purpose:     wxStatusBar class
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:   	wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifndef __STATUSBRH_G__
#define __STATUSBRH_G__

#ifdef __GNUG__
#pragma interface "statusbr.h"
#endif

#include "wx/window.h"

WXDLLEXPORT_DATA(extern const char*) wxPanelNameStr;

class WXDLLEXPORT wxStatusBar: public wxWindow
{
  DECLARE_DYNAMIC_CLASS(wxStatusBar)

public:
  wxStatusBar(void);
  inline wxStatusBar(wxWindow *parent, const wxWindowID id,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize,
           const long style = 0,
           const wxString& name = wxPanelNameStr)
  {
      Create(parent, id, pos, size, style, name);
  }

  ~wxStatusBar(void);

  bool Create(wxWindow *parent, const wxWindowID id,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize,
           const long style = 0,
           const wxString& name = wxPanelNameStr);

  // Create status line
  virtual void SetFieldsCount(const int number=1, const int *widths = NULL);
  inline int GetFieldsCount(void) const { return m_nFields; }

  // Set status line text
  virtual void SetStatusText(const wxString& text, const int number = 0);
  virtual wxString GetStatusText(const int number = 0) const;

  // Set status line widths
  virtual void SetStatusWidths(const int n, const int *widths_field);

  virtual void DrawFieldText(wxDC& dc, const int i);
  virtual void DrawField(wxDC& dc, const int i);

  // Get the position and size of the field's internal bounding rectangle
  virtual bool GetFieldRect(const int i, wxRectangle& rect) const;

  inline int GetBorderX(void) const { return m_borderX; }
  inline int GetBorderY(void) const { return m_borderY; }
  inline void SetBorderX(const int x);
  inline void SetBorderY(const int y);

  ////////////////////////////////////////////////////////////////////////
  // Implementation

  void OnPaint(wxPaintEvent& event);

  virtual void InitColours(void);

  // Responds to colour changes
  void OnSysColourChanged(wxSysColourChangedEvent& event);

protected:
  int *             m_statusWidths;
  int               m_nFields;
  wxString *        m_statusStrings;
  int               m_borderX;
  int               m_borderY;
  wxFont            m_defaultStatusBarFont;
  wxPen             m_mediumShadowPen;
  wxPen             m_hilightPen;

  DECLARE_EVENT_TABLE()
};

#endif
    // __STATUSBRH_G__
