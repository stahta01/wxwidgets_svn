/////////////////////////////////////////////////////////////////////////////
// Name:        scrolwin.h
// Purpose:     wxScrolledWindow class
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:   	wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifndef __SCROLWINH_G__
#define __SCROLWINH_G__

#ifdef __GNUG__
#pragma interface "scrolwin.h"
#endif

#include "wx/window.h"

WXDLLEXPORT_DATA(extern const char*) wxPanelNameStr;

class WXDLLEXPORT wxScrolledWindow: public wxWindow
{
  DECLARE_ABSTRACT_CLASS(wxScrolledWindow)

public:
  wxScrolledWindow(void);
  inline wxScrolledWindow(wxWindow *parent, const wxWindowID id = -1,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize,
           const long style = wxHSCROLL|wxVSCROLL,
           const wxString& name = wxPanelNameStr)
  {
      Create(parent, id, pos, size, style, name);
  }

  inline ~wxScrolledWindow(void) {}

  bool Create(wxWindow *parent, const wxWindowID id,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize,
           const long style = wxHSCROLL|wxVSCROLL,
           const wxString& name = wxPanelNameStr);

  // Set client size
  // Should take account of scrollbars
//  virtual void SetClientSize(const int width, const int size);

  // Is the window retained?
//  inline bool IsRetained(void) const;

  // Number of pixels per user unit (0 or -1 for no scrollbar)
  // Length of virtual canvas in user units
  // Length of page in user units
  virtual void SetScrollbars(const int pixelsPerUnitX, const int pixelsPerUnitY,
                             const int noUnitsX, const int noUnitsY,
                             const int xPos = 0, const int yPos = 0, 
			     const bool noRefresh = FALSE );

  // Physically scroll the window
  virtual void Scroll(const int x_pos, const int y_pos);

#if WXWIN_COMPATIBILITY
  virtual void GetScrollUnitsPerPage(int *x_page, int *y_page) const;
#endif

  int GetScrollPageSize(int orient) const ;
  void SetScrollPageSize(int orient, int pageSize);

  virtual void GetScrollPixelsPerUnit(int *x_unit, int *y_unit) const;
  // Enable/disable Windows scrolling in either direction.
  // If TRUE, wxWindows scrolls the canvas and only a bit of
  // the canvas is invalidated; no Clear() is necessary.
  // If FALSE, the whole canvas is invalidated and a Clear() is
  // necessary. Disable for when the scroll increment is used
  // to actually scroll a non-constant distance
  virtual void EnableScrolling(const bool x_scrolling, const bool y_scrolling);

  // Get the view start
  virtual void ViewStart(int *x, int *y) const;

  // Actual size in pixels when scrolling is taken into account
  virtual void GetVirtualSize(int *x, int *y) const;

  virtual void CalcScrolledPosition(const int x, const int y, int *xx, int *yy) const ;
  virtual void CalcUnscrolledPosition(const int x, const int y, float *xx, float *yy) const ;

  // Adjust the scrollbars
  virtual void AdjustScrollbars(void);

/*
#if WXWIN_COMPATIBILITY
  virtual void OldOnScroll(wxCommandEvent& WXUNUSED(event));
  virtual void OldOnPaint(void);                 // Called when needs painting
  virtual void OldOnSize(int width, int height);           // Called on resize
  virtual void OldOnMouseEvent(wxMouseEvent& event);  // Called on mouse event
  virtual void OldOnChar(wxKeyEvent& event);     // Called on character event
#endif
*/

  void OnScroll(wxScrollEvent& event);
  void OnSize(wxSizeEvent& event);
  void OnPaint(wxPaintEvent& event);

  // Override this function to draw the graphic.
  virtual void OnDraw(wxDC& WXUNUSED(dc)) {};

  // Override this function if you don't want to have wxScrolledWindow
  // automatically change the origin according to the scroll position.
  virtual void PrepareDC(wxDC& dc);

public:
  ////////////////////////////////////////////////////////////////////////
  //// IMPLEMENTATION
  
  // Calculate scroll increment
  virtual int CalcScrollInc(wxScrollEvent& event);

  ////////////////////////////////////////////////////////////////////////
  //// PROTECTED DATA
protected:
  int                   m_xScrollPixelsPerLine;
  int                   m_yScrollPixelsPerLine;
  bool                  m_xScrollingEnabled;
  bool                  m_yScrollingEnabled;
  int                   m_xScrollPosition;
  int                   m_yScrollPosition;
  int                   m_xScrollLines;
  int                   m_yScrollLines;
  int                   m_xScrollLinesPerPage;
  int                   m_yScrollLinesPerPage;

DECLARE_EVENT_TABLE()
};

////////////////////////////////////////////////////////////////////////
//// INLINES

#endif
    // __SCROLWINH_G__
