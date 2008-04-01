/////////////////////////////////////////////////////////////////////////////
// Name:        include/wx/scrolwin.h
// Purpose:     wxScrolledWindow, wxScrolledControl and wxScrollHelper
// Author:      Vadim Zeitlin
// Modified by:
// Created:     30.08.00
// RCS-ID:      $Id$
// Copyright:   (c) 2000 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_SCROLWIN_H_BASE_
#define _WX_SCROLWIN_H_BASE_

#include "wx/panel.h"

class WXDLLIMPEXP_FWD_CORE wxScrollHelperEvtHandler;
class WXDLLIMPEXP_FWD_BASE wxTimer;

// default scrolled window style: scroll in both directions
#define wxScrolledWindowStyle (wxHSCROLL | wxVSCROLL)

// ----------------------------------------------------------------------------
// The hierarchy of scrolling classes is a bit complicated because we want to
// put as much functionality as possible in a mix-in class not deriving from
// wxWindow so that other classes could derive from the same base class on all
// platforms irrespectively of whether they are native controls (and hence
// don't use our scrolling) or not.
//
// So we have
//
//                             wxScrollHelper
//                                   |
//                                   |
//                                  \|/
//      wxWindow            wxScrollHelperNative
//       |  \                   /        /
//       |   \                 /        /
//       |    _|             |_        /
//       |     wxScrolledWindow       /
//       |                           /
//      \|/                         /
//   wxControl                     /
//         \                      /
//          \                    /
//           _|                |_
//            wxScrolledControl
//
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxScrollHelper
{
public:
    // ctor must be given the associated window
    wxScrollHelper(wxWindow *winToScroll);
    virtual ~wxScrollHelper();

    // configure the scrolling
    virtual void SetScrollbars(int pixelsPerUnitX, int pixelsPerUnitY,
                               int noUnitsX, int noUnitsY,
                               int xPos = 0, int yPos = 0,
                               bool noRefresh = false );

    // scroll to the given (in logical coords) position
    virtual void Scroll(int x, int y);

    // get/set the page size for this orientation (wxVERTICAL/wxHORIZONTAL)
    int GetScrollPageSize(int orient) const;
    void SetScrollPageSize(int orient, int pageSize);

    // get the number of lines the window can scroll, 
    // returns 0 if no scrollbars are there.
    int GetScrollLines( int orient ) const;

    // Set the x, y scrolling increments.
    void SetScrollRate( int xstep, int ystep );

    // get the size of one logical unit in physical ones
    virtual void GetScrollPixelsPerUnit(int *pixelsPerUnitX,
                                        int *pixelsPerUnitY) const;

    // Enable/disable Windows scrolling in either direction. If true, wxWidgets
    // scrolls the canvas and only a bit of the canvas is invalidated; no
    // Clear() is necessary. If false, the whole canvas is invalidated and a
    // Clear() is necessary. Disable for when the scroll increment is used to
    // actually scroll a non-constant distance
    virtual void EnableScrolling(bool x_scrolling, bool y_scrolling);

    // Get the view start
    virtual void GetViewStart(int *x, int *y) const;

    // Set the scale factor, used in PrepareDC
    void SetScale(double xs, double ys) { m_scaleX = xs; m_scaleY = ys; }
    double GetScaleX() const { return m_scaleX; }
    double GetScaleY() const { return m_scaleY; }

    // translate between scrolled and unscrolled coordinates
    void CalcScrolledPosition(int x, int y, int *xx, int *yy) const
        {  DoCalcScrolledPosition(x, y, xx, yy); }
    wxPoint CalcScrolledPosition(const wxPoint& pt) const
    {
        wxPoint p2;
        DoCalcScrolledPosition(pt.x, pt.y, &p2.x, &p2.y);
        return p2;
    }

    void CalcUnscrolledPosition(int x, int y, int *xx, int *yy) const
        {  DoCalcUnscrolledPosition(x, y, xx, yy); }
    wxPoint CalcUnscrolledPosition(const wxPoint& pt) const
    {
        wxPoint p2;
        DoCalcUnscrolledPosition(pt.x, pt.y, &p2.x, &p2.y);
        return p2;
    }

    virtual void DoCalcScrolledPosition(int x, int y, int *xx, int *yy) const;
    virtual void DoCalcUnscrolledPosition(int x, int y, int *xx, int *yy) const;

    // Adjust the scrollbars
    virtual void AdjustScrollbars(void);

    // Calculate scroll increment
    virtual int CalcScrollInc(wxScrollWinEvent& event);

    // Normally the wxScrolledWindow will scroll itself, but in some rare
    // occasions you might want it to scroll [part of] another window (e.g. a
    // child of it in order to scroll only a portion the area between the
    // scrollbars (spreadsheet: only cell area will move).
    virtual void SetTargetWindow(wxWindow *target);
    virtual wxWindow *GetTargetWindow() const;

    void SetTargetRect(const wxRect& rect) { m_rectToScroll = rect; }
    wxRect GetTargetRect() const { return m_rectToScroll; }

    // Override this function to draw the graphic (or just process EVT_PAINT)
    virtual void OnDraw(wxDC& WXUNUSED(dc)) { }

    // change the DC origin according to the scroll position.
    virtual void DoPrepareDC(wxDC& dc);

    // are we generating the autoscroll events?
    bool IsAutoScrolling() const { return m_timerAutoScroll != NULL; }

    // stop generating the scroll events when mouse is held outside the window
    void StopAutoScrolling();

    // this method can be overridden in a derived class to forbid sending the
    // auto scroll events - note that unlike StopAutoScrolling() it doesn't
    // stop the timer, so it will be called repeatedly and will typically
    // return different values depending on the current mouse position
    //
    // the base class version just returns true
    virtual bool SendAutoScrollEvents(wxScrollWinEvent& event) const;

    // the methods to be called from the window event handlers
    void HandleOnScroll(wxScrollWinEvent& event);
    void HandleOnSize(wxSizeEvent& event);
    void HandleOnPaint(wxPaintEvent& event);
    void HandleOnChar(wxKeyEvent& event);
    void HandleOnMouseEnter(wxMouseEvent& event);
    void HandleOnMouseLeave(wxMouseEvent& event);
#if wxUSE_MOUSEWHEEL
    void HandleOnMouseWheel(wxMouseEvent& event);
#endif // wxUSE_MOUSEWHEEL
    void HandleOnChildFocus(wxChildFocusEvent& event);

    // FIXME: this is needed for now for wxPlot compilation, should be removed
    //        once it is fixed!
    void OnScroll(wxScrollWinEvent& event) { HandleOnScroll(event); }

protected:
    // get pointer to our scroll rect if we use it or NULL
    const wxRect *GetScrollRect() const
    {
        return m_rectToScroll.width != 0 ? &m_rectToScroll : NULL;
    }

    // get the size of the target window
    wxSize GetTargetSize() const
    {
        return m_rectToScroll.width != 0 ? m_rectToScroll.GetSize()
                                         : m_targetWindow->GetClientSize();
    }

    void GetTargetSize(int *w, int *h) const
    {
        wxSize size = GetTargetSize();
        if ( w )
            *w = size.x;
        if ( h )
            *h = size.y;
    }

    // implementations of various wxWindow virtual methods which should be
    // forwarded to us (this can be done by WX_FORWARD_TO_SCROLL_HELPER())
    bool ScrollLayout();
    void ScrollDoSetVirtualSize(int x, int y);
    wxSize ScrollGetBestVirtualSize() const;

    // change just the target window (unlike SetWindow which changes m_win as
    // well)
    void DoSetTargetWindow(wxWindow *target);

    // delete the event handler we installed
    void DeleteEvtHandler();

    // calls wxScrollHelperEvtHandler::ResetDrawnFlag(), see explanation
    // in wxScrollHelperEvtHandler::ProcessEvent()
    void ResetDrawnFlag();


    double                m_scaleX;
    double                m_scaleY;

    wxWindow             *m_win,
                         *m_targetWindow;

    wxRect                m_rectToScroll;

    wxTimer              *m_timerAutoScroll;

    int                   m_xScrollPixelsPerLine;
    int                   m_yScrollPixelsPerLine;
    int                   m_xScrollPosition;
    int                   m_yScrollPosition;
    int                   m_xScrollLines;
    int                   m_yScrollLines;
    int                   m_xScrollLinesPerPage;
    int                   m_yScrollLinesPerPage;

    bool                  m_xScrollingEnabled;
    bool                  m_yScrollingEnabled;

#if wxUSE_MOUSEWHEEL
    int m_wheelRotation;
#endif // wxUSE_MOUSEWHEEL

    wxScrollHelperEvtHandler *m_handler;

    DECLARE_NO_COPY_CLASS(wxScrollHelper)
};

// this macro can be used in a wxScrollHelper-derived class to forward wxWindow
// methods to corresponding wxScrollHelper methods
#define WX_FORWARD_TO_SCROLL_HELPER()                                         \
public:                                                                       \
    virtual void PrepareDC(wxDC& dc) { DoPrepareDC(dc); }                     \
    virtual bool Layout() { return ScrollLayout(); }                          \
    virtual void DoSetVirtualSize(int x, int y)                               \
        { ScrollDoSetVirtualSize(x, y); }                                     \
    virtual wxSize GetBestVirtualSize() const                                 \
        { return ScrollGetBestVirtualSize(); }

// include the declaration of wxScrollHelperNative if needed
#if defined(__WXGTK20__) && !defined(__WXUNIVERSAL__)
    #include "wx/gtk/scrolwin.h"
#elif defined(__WXGTK__) && !defined(__WXUNIVERSAL__)
    #include "wx/gtk1/scrolwin.h"
#else
    typedef wxScrollHelper wxScrollHelperNative;
#endif

// ----------------------------------------------------------------------------
// wxScrolled<T>: a wxWindow which knows how to scroll
// ----------------------------------------------------------------------------

// helper class for wxScrolled<T> below
struct WXDLLIMPEXP_CORE wxScrolledT_Helper
{
    static wxSize FilterBestSize(const wxWindow *win,
                                 const wxScrollHelperNative *helper,
                                 const wxSize& origBest);
#ifdef __WXMSW__
    static WXLRESULT FilterMSWWindowProc(WXLRESULT origResult);
#endif
};

// Scrollable window base on window type T. This used to be wxScrolledWindow,
// but wxScrolledWindow includes wxControlContainer functionality and that's
// not always desirable.
template<class T>
class WXDLLIMPEXP_CORE wxScrolled : public T,
                                    public wxScrollHelperNative,
                                    private wxScrolledT_Helper
{
public:
    wxScrolled() : wxScrollHelperNative(this) { }
    wxScrolled(wxWindow *parent,
               wxWindowID winid = wxID_ANY,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = wxScrolledWindowStyle,
               const wxString& name = wxPanelNameStr)
        : wxScrollHelperNative(this)
    {
        Create(parent, winid, pos, size, style, name);
    }

    bool Create(wxWindow *parent,
                wxWindowID winid,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxScrolledWindowStyle,
                const wxString& name = wxPanelNameStr)
    {
        m_targetWindow = this;

#ifdef __WXMAC__
        MacSetClipChildren(true);
#endif

        this->Connect(wxEVT_PAINT, wxPaintEventHandler(wxScrolled::OnPaint));

        // by default, we're scrollable in both directions (but if one of the
        // styles is specified explicitly, we shouldn't add the other one
        // automatically)
        if ( !(style & (wxHSCROLL | wxVSCROLL)) )
            style |= wxHSCROLL | wxVSCROLL;

        return T::Create(parent, winid, pos, size, style, name);
    }

    // we need to return a special WM_GETDLGCODE value to process just the
    // arrows but let the other navigation characters through
#ifdef __WXMSW__
    virtual WXLRESULT MSWWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam)
    {
        return FilterMSWWindowProc(T::MSWWindowProc(nMsg, wParam, lParam));
    }
#endif // __WXMSW__

    WX_FORWARD_TO_SCROLL_HELPER()

protected:
    virtual wxSize DoGetBestSize() const
    {
        return FilterBestSize(this, this, T::DoGetBestSize());
    }

private:
    // this is needed for wxEVT_PAINT processing hack described in
    // wxScrollHelperEvtHandler::ProcessEvent()
    void OnPaint(wxPaintEvent& event)
    {
        // the user code didn't really draw the window if we got here, so set
        // this flag to try to call OnDraw() later
        ResetDrawnFlag();
        event.Skip();
    }

    DECLARE_NO_COPY_CLASS(wxScrolled)
};

// VC++ <= 6 requires this; it's unlikely any other specializations would
// be needed by user code _and_ they were using VC6, so we list only wxWindow
// (typical use) and wxPanel (wxScrolledWindow use) specializations here
WXDLLIMPEXP_TEMPLATE_INSTANCE_CORE( wxScrolled<wxPanel> )
WXDLLIMPEXP_TEMPLATE_INSTANCE_CORE( wxScrolled<wxWindow> )

// for compatibility with existing code, we provide wxScrolledWindow
// "typedef" for wxScrolled<wxPanel>. It's not a real typedef because we
// want wxScrolledWindow to show in wxRTTI information (the class is widely
// used and likelihood of its wxRTTI information being used too is high):
class WXDLLIMPEXP_CORE wxScrolledWindow : public wxScrolled<wxPanel>
{
public:
    wxScrolledWindow() : wxScrolled<wxPanel>() {}
    wxScrolledWindow(wxWindow *parent,
                     wxWindowID winid = wxID_ANY,
                     const wxPoint& pos = wxDefaultPosition,
                     const wxSize& size = wxDefaultSize,
                     long style = wxScrolledWindowStyle,
                     const wxString& name = wxPanelNameStr)
        : wxScrolled<wxPanel>(parent, winid, pos, size, style, name) {}

    DECLARE_DYNAMIC_CLASS_NO_COPY(wxScrolledWindow)
};

typedef wxScrolled<wxWindow> wxScrolledCanvas;

#endif // _WX_SCROLWIN_H_BASE_
