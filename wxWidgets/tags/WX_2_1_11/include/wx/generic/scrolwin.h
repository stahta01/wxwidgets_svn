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
#include "wx/panel.h"

WXDLLEXPORT_DATA(extern const wxChar*) wxPanelNameStr;

class WXDLLEXPORT wxScrolledWindow : public wxPanel
{
public:
    wxScrolledWindow();
    inline wxScrolledWindow(wxWindow *parent, wxWindowID id = -1,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize,
           long style = wxHSCROLL|wxVSCROLL,
           const wxString& name = wxPanelNameStr)
    {
      Create(parent, id, pos, size, style, name);
    }

    ~wxScrolledWindow();

    bool Create(wxWindow *parent, wxWindowID id,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize,
           long style = wxHSCROLL|wxVSCROLL,
           const wxString& name = wxPanelNameStr);

    // Normally the wxScrolledWindow will scroll itself, but in 
    // some rare occasions you might want it to scroll another 
    // window (e.g. a child of it in order to scroll only a portion 
    // the area between the scrollbars (spreadsheet: only cell area
    // will move). 
    virtual void SetTargetWindow( wxWindow *target );
    virtual wxWindow *GetTargetWindow();

    // Number of pixels per user unit (0 or -1 for no scrollbar)
    // Length of virtual canvas in user units
    // Length of page in user units
    virtual void SetScrollbars(int pixelsPerUnitX, int pixelsPerUnitY,
                             int noUnitsX, int noUnitsY,
                             int xPos = 0, int yPos = 0,
			     bool noRefresh = FALSE );

    // Physically scroll the window
    virtual void Scroll(int x_pos, int y_pos);

#if WXWIN_COMPATIBILITY
    virtual void GetScrollUnitsPerPage(int *x_page, int *y_page) const;
    virtual void CalcUnscrolledPosition(int x, int y, float *xx, float *yy) const ;
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
    virtual void EnableScrolling(bool x_scrolling, bool y_scrolling);

    // Get the view start
    virtual void ViewStart(int *x, int *y) const;

    // Actual size in pixels when scrolling is taken into account
    virtual void GetVirtualSize(int *x, int *y) const;

    // Set the scale factor, used in PrepareDC
    void SetScale(double xs, double ys) { m_scaleX = xs; m_scaleY = ys; }
    double GetScaleX() const { return m_scaleX; }
    double GetScaleY() const { return m_scaleY; }

    virtual void CalcScrolledPosition(int x, int y, int *xx, int *yy) const ;
    virtual void CalcUnscrolledPosition(int x, int y, int *xx, int *yy) const ;

    // Adjust the scrollbars
    virtual void AdjustScrollbars(void);

    // Override this function to draw the graphic (or just process EVT_PAINT)
    virtual void OnDraw(wxDC& WXUNUSED(dc)) {};

    // Override this function if you don't want to have wxScrolledWindow
    // automatically change the origin according to the scroll position.
    virtual void PrepareDC(wxDC& dc);

    // implementation from now on
    void OnScroll(wxScrollWinEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnPaint(wxPaintEvent& event);

    // Calculate scroll increment
    virtual int CalcScrollInc(wxScrollWinEvent& event);

protected:
    wxWindow             *m_targetWindow;
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
    double                m_scaleX;
    double                m_scaleY;

private:
    DECLARE_EVENT_TABLE()
    DECLARE_ABSTRACT_CLASS(wxScrolledWindow)
};

#endif
    // __SCROLWINH_G__
