/////////////////////////////////////////////////////////////////////////////
// Name:        windows.cpp
// Purpose:     wxWindow
// Author:      Julian Smart
// Modified by:
// Created:     17/09/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#ifdef __GNUG__
    #pragma implementation "window.h"
#endif

#ifdef __VMS
#define XtDisplay XTDISPLAY
#define XtWindow XTWINDOW
#define XtScreen XTSCREEN
#endif

#include "wx/setup.h"
#include "wx/menu.h"
#include "wx/dc.h"
#include "wx/dcclient.h"
#include "wx/utils.h"
#include "wx/app.h"
#include "wx/layout.h"
#include "wx/button.h"
#include "wx/settings.h"
#include "wx/frame.h"
#include "wx/scrolwin.h"
#include "wx/module.h"
#include "wx/menuitem.h"
#include "wx/log.h"
#include "wx/evtloop.h"
#include "wx/hash.h"

#if  wxUSE_DRAG_AND_DROP
    #include "wx/dnd.h"
#endif

// DoSetSizeIntr and DoMoveWindowIntr
// PROBLEM:
// under Motif composite controls (such as wxCalendarCtrl or generic wxSpinCtrl
// did nott work and/or segfaulted because
// 1) wxWindow::Create calls SetSize,
//    which results in a call to DoSetSize much earlier than in the other ports
// 2) if wxWindow::Create is called (wxControl::Create calls it)
//    then DoSetSize is never called, causing layout problems in composite
//    controls
//
// SOLUTION:
// 1) don't call SetSize, DoSetSize, DoMoveWindow, DoGetPosition,
//    DoSetPosition directly or indirectly from wxWindow::Create
// 2) call DoMoveWindow from DoSetSize, allowing controls to override it

#ifdef __VMS__
#pragma message disable nosimpint
#endif
#include <Xm/Xm.h>

#include <Xm/DrawingA.h>
#include <Xm/ScrolledW.h>
#include <Xm/ScrollBar.h>
#include <Xm/Frame.h>
#include <Xm/Label.h>
#include <Xm/RowColumn.h>           // for XmMenuPosition
#ifdef __VMS__
#pragma message enable nosimpint
#endif

#include "wx/motif/private.h"

#include <string.h>

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

static const int SCROLL_MARGIN = 4;

// ----------------------------------------------------------------------------
// global variables for this module
// ----------------------------------------------------------------------------

extern wxHashTable *wxWidgetHashTable;
static wxWindow* g_captureWindow = NULL;


// ----------------------------------------------------------------------------
// private functions
// ----------------------------------------------------------------------------

static void wxCanvasRepaintProc(Widget, XtPointer, XmDrawingAreaCallbackStruct * cbs);
static void wxCanvasInputEvent(Widget drawingArea, XtPointer data, XmDrawingAreaCallbackStruct * cbs);
static void wxCanvasMotionEvent(Widget, XButtonEvent * event);
static void wxCanvasEnterLeave(Widget drawingArea, XtPointer clientData, XCrossingEvent * event);
static void wxScrollBarCallback(Widget widget, XtPointer clientData,
                                XmScrollBarCallbackStruct *cbs);
static void wxPanelItemEventHandler(Widget    wid,
                                    XtPointer client_data,
                                    XEvent*   event,
                                    Boolean  *continueToDispatch);

// unused for now
#if 0

// Helper function for 16-bit fonts
static int str16len(const char *s)
{
    int count = 0;

    while (s[0] && s[1]) {
        count++;
        s += 2;
    }

    return count;
}

#endif // 0

// ----------------------------------------------------------------------------
// macros
// ----------------------------------------------------------------------------

#define event_left_is_down(x) ((x)->xbutton.state & Button1Mask)
#define event_middle_is_down(x) ((x)->xbutton.state & Button2Mask)
#define event_right_is_down(x) ((x)->xbutton.state & Button3Mask)

// ----------------------------------------------------------------------------
// event tables
// ----------------------------------------------------------------------------

    IMPLEMENT_DYNAMIC_CLASS(wxWindow, wxWindowBase)

    BEGIN_EVENT_TABLE(wxWindow, wxWindowBase)
        EVT_SYS_COLOUR_CHANGED(wxWindow::OnSysColourChanged)
        EVT_IDLE(wxWindow::OnIdle)
    END_EVENT_TABLE()

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// helper functions
// ----------------------------------------------------------------------------

void wxWindow::UnmanageAndDestroy(WXWidget widget)
{
    Widget w = (Widget)widget;
    if ( w )
    {
        XtUnmanageChild(w);
        XtDestroyWidget(w);
    }
}

bool wxWindow::MapOrUnmap(WXWidget widget, bool domap)
{
    Widget w = (Widget)widget;
    if ( !w )
        return FALSE;

    //   Rationale: a lot of common operations (including but not
    // limited to moving, resizing and appending items to a listbox)
    // unmamange the widget, do their work, then manage it again.
    // This means that, for example adding an item to a listbox will show it,
    // or that most controls are shown every time they are moved or resized!
    XtSetMappedWhenManaged( w, domap );

    // if the widget is not unmanaged, it still intercepts
    // mouse events, even if it is not mapped (and hence invisible)
    if ( domap )
    {
        XtManageChild(w);
        // XtMapWidget(w);
    }
    else
    {
        XtUnmanageChild(w);
        // XtUnmapWidget(w);
    }

    return TRUE;
}

// ----------------------------------------------------------------------------
// constructors
// ----------------------------------------------------------------------------

void wxWindow::Init()
{
    // generic initializations first
    InitBase();

    // Motif-specific
    m_needsRefresh = TRUE;
    m_mainWidget = (WXWidget) 0;

    m_winCaptured = FALSE;

    m_isShown = TRUE;
    m_isBeingDeleted = FALSE;
    
    m_hScrollBar =
    m_vScrollBar =
    m_borderWidget =
    m_scrolledWindow =
    m_drawingArea = (WXWidget) 0;

    m_scrollPosX =
    m_scrollPosY = 0;

    m_backingPixmap = (WXPixmap) 0;
    m_pixmapWidth =
    m_pixmapHeight = 0;

    m_pixmapOffsetX =
    m_pixmapOffsetY = 0;

    m_lastTS = 0;
    m_lastButton = 0;
}

// real construction (Init() must have been called before!)
bool wxWindow::Create(wxWindow *parent, wxWindowID id,
                      const wxPoint& pos,
                      const wxSize& size,
                      long style,
                      const wxString& name)
{
    wxCHECK_MSG( parent, FALSE, "can't create wxWindow without parent" );

    CreateBase(parent, id, pos, size, style, wxDefaultValidator, name);

    parent->AddChild(this);

    m_backgroundColour = wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE);
    m_foregroundColour = *wxBLACK;

    //// TODO: we should probably optimize by only creating a
    //// a drawing area if we have one or more scrollbars (wxVSCROLL/wxHSCROLL).
    //// But for now, let's simplify things by always creating the
    //// drawing area, since otherwise the translations are different.

    // New translations for getting mouse motion feedback
    static const String translations =
"<Btn1Motion>: wxCanvasMotionEvent() DrawingAreaInput() ManagerGadgetButtonMotion()\n\
<Btn2Motion>: wxCanvasMotionEvent() DrawingAreaInput() ManagerGadgetButtonMotion()\n\
<Btn3Motion>: wxCanvasMotionEvent() DrawingAreaInput() ManagerGadgetButtonMotion()\n\
<BtnMotion>: wxCanvasMotionEvent() DrawingAreaInput() ManagerGadgetButtonMotion()\n\
<Btn1Down>: DrawingAreaInput() ManagerGadgetArm()\n\
<Btn2Down>: DrawingAreaInput() ManagerGadgetArm()\n\
<Btn3Down>: DrawingAreaInput() ManagerGadgetArm()\n\
<Btn1Up>: DrawingAreaInput() ManagerGadgetActivate()\n\
<Btn2Up>: DrawingAreaInput() ManagerGadgetActivate()\n\
<Btn3Up>: DrawingAreaInput() ManagerGadgetActivate()\n\
<Motion>: wxCanvasMotionEvent() DrawingAreaInput()\n\
<EnterWindow>: wxCanvasMotionEvent() DrawingAreaInput()\n\
<LeaveWindow>: wxCanvasMotionEvent() DrawingAreaInput()\n\
<Key>: DrawingAreaInput()";

    XtActionsRec actions[1];
    actions[0].string = "wxCanvasMotionEvent";
    actions[0].proc = (XtActionProc) wxCanvasMotionEvent;
    XtAppAddActions ((XtAppContext) wxTheApp->GetAppContext(), actions, 1);

    Widget parentWidget = (Widget) parent->GetClientWidget();
    m_borderWidget = wxCreateBorderWidget( (WXWidget)parentWidget, style );

    m_scrolledWindow = (WXWidget)XtVaCreateManagedWidget
                                 (
                                  "scrolledWindow",
                                  xmScrolledWindowWidgetClass,
                                  m_borderWidget ? (Widget) m_borderWidget
                                                 : parentWidget,
                                  XmNresizePolicy, XmRESIZE_NONE,
                                  XmNspacing, 0,
                                  XmNscrollingPolicy, XmAPPLICATION_DEFINED,
                                  //XmNscrollBarDisplayPolicy, XmAS_NEEDED,
                                  NULL
                                 );

    XtTranslations ptr = XtParseTranslationTable(translations);
    m_drawingArea = (WXWidget)XtVaCreateWidget
                              (
                               name,
                               xmDrawingAreaWidgetClass, (Widget) m_scrolledWindow,
                               XmNunitType, XmPIXELS,
                               // XmNresizePolicy, XmRESIZE_ANY,
                               XmNresizePolicy, XmRESIZE_NONE,
                               XmNmarginHeight, 0,
                               XmNmarginWidth, 0,
                               XmNtranslations, ptr,
                               NULL
                              );
    XtFree((char *) ptr);

#if 0
    if (GetWindowStyleFlag() & wxOVERRIDE_KEY_TRANSLATIONS)
    {
        ptr = XtParseTranslationTable ("<Key>: DrawingAreaInput()");
        XtOverrideTranslations ((Widget) m_drawingArea, ptr);
        XtFree ((char *) ptr);
    }
#endif // 0

    wxAddWindowToTable((Widget) m_drawingArea, this);
    wxAddWindowToTable((Widget) m_scrolledWindow, this);

    // This order is very important in Motif 1.2.1
    XtRealizeWidget ((Widget) m_scrolledWindow);
    XtRealizeWidget ((Widget) m_drawingArea);
    XtManageChild ((Widget) m_drawingArea);

    ptr = XtParseTranslationTable("<Configure>: resize()");
    XtOverrideTranslations((Widget) m_drawingArea, ptr);
    XtFree ((char *) ptr);

    XtAddCallback ((Widget) m_drawingArea, XmNexposeCallback, (XtCallbackProc) wxCanvasRepaintProc, (XtPointer) this);
    XtAddCallback ((Widget) m_drawingArea, XmNinputCallback, (XtCallbackProc) wxCanvasInputEvent, (XtPointer) this);

    XtAddEventHandler(
                      (Widget)m_drawingArea,
                       PointerMotionHintMask | EnterWindowMask |
                       LeaveWindowMask | FocusChangeMask,
                       False,
                       (XtEventHandler) wxCanvasEnterLeave,
                       (XtPointer) this
                     );

    // Scrolled widget needs to have its colour changed or we get a little blue
    // square where the scrollbars abutt
    wxColour backgroundColour = wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE);
    wxDoChangeBackgroundColour(m_scrolledWindow, backgroundColour, TRUE);
    wxDoChangeBackgroundColour(m_drawingArea, backgroundColour, TRUE);

    XmScrolledWindowSetAreas(
                             (Widget)m_scrolledWindow,
                             (Widget) 0, (Widget) 0,
                             (Widget) m_drawingArea);

    // Without this, the cursor may not be restored properly (e.g. in splitter
    // sample).
    SetCursor(*wxSTANDARD_CURSOR);
    SetFont(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT));
    DoSetSizeIntr(pos.x, pos.y, size.x,size.y, wxSIZE_AUTO, TRUE);
    return TRUE;
}

// Destructor
wxWindow::~wxWindow()
{
    if (g_captureWindow == this)
        g_captureWindow = NULL;
    
    m_isBeingDeleted = TRUE;
    
    // Motif-specific actions first
    WXWidget wMain = GetMainWidget();
    if ( wMain )
    {
        // Removes event handlers
        DetachWidget(wMain);
    }

    if ( m_parent )
        m_parent->RemoveChild( this );

    // If m_drawingArea, we're a fully-fledged window with drawing area,
    // scrollbars etc. (what wxCanvas used to be)
    if ( m_drawingArea )
    {
        // Destroy children before destroying self
        DestroyChildren();

        if (m_backingPixmap)
            XFreePixmap (XtDisplay ((Widget) GetMainWidget()), (Pixmap) m_backingPixmap);

        Widget w = (Widget) m_drawingArea;
        wxDeleteWindowFromTable(w);

        if (w)
        {
            XtDestroyWidget(w);
            m_drawingArea = (WXWidget) 0;
        }

        // Only if we're _really_ a canvas (not a dialog box/panel)
        if (m_scrolledWindow)
        {
            wxDeleteWindowFromTable((Widget) m_scrolledWindow);
        }

        if (m_hScrollBar)
        {
            wxDeleteWindowFromTable((Widget) m_hScrollBar);
            XtUnmanageChild((Widget) m_hScrollBar);
        }
        if (m_vScrollBar)
        {
            wxDeleteWindowFromTable((Widget) m_vScrollBar);
            XtUnmanageChild((Widget) m_vScrollBar);
        }

        if (m_hScrollBar)
            XtDestroyWidget((Widget) m_hScrollBar);
        if (m_vScrollBar)
            XtDestroyWidget((Widget) m_vScrollBar);

        UnmanageAndDestroy(m_scrolledWindow);

        if (m_borderWidget)
        {
            XtDestroyWidget ((Widget) m_borderWidget);
            m_borderWidget = (WXWidget) 0;
        }
    }
    else // Why wasn't this here before? JACS 8/3/2000
        DestroyChildren();


    // Destroy the window
    if (GetMainWidget())
    {
        // If this line (XtDestroyWidget) causes a crash, you may comment it out.
        // Child widgets will get destroyed automatically when a frame
        // or dialog is destroyed, but before that you may get some memory
        // leaks and potential layout problems if you delete and then add
        // child windows.

        // GRG, Feb/2000: commented this out when adding support for
        //   wxSCROLL[WIN]_THUMBRELEASE events. Also it was reported
        //   that this call crashed wxMotif under OS/2, so it seems
        //   that leaving it out is the right thing to do.
        // SN, Feb/2000: newgrid/griddemo shows why it is needed :-(
        XtDestroyWidget((Widget) GetMainWidget());
        SetMainWidget((WXWidget) NULL);
    }
}

// ----------------------------------------------------------------------------
// scrollbar management
// ----------------------------------------------------------------------------

WXWidget wxWindow::DoCreateScrollBar(WXWidget parent,
                                     wxOrientation orientation,
                                     void (*callback)())
{
    int orient = ( orientation & wxHORIZONTAL ) ? XmHORIZONTAL : XmVERTICAL;
    Widget sb =
        XtVaCreateManagedWidget( "scrollBarWidget",
                                 xmScrollBarWidgetClass, (Widget)parent,
                                 XmNorientation, orient,
                                 XmNincrement, 1,
                                 XmNvalue, 0,
                                 NULL );

    XtPointer o = (XtPointer)orientation;
    XtCallbackProc cb = (XtCallbackProc)callback;

    XtAddCallback( sb, XmNvalueChangedCallback, cb, o );
    XtAddCallback( sb, XmNdragCallback, cb, o );
    XtAddCallback( sb, XmNincrementCallback, cb, o );
    XtAddCallback( sb, XmNdecrementCallback, cb, o );
    XtAddCallback( sb, XmNpageIncrementCallback, cb, o );
    XtAddCallback( sb, XmNpageDecrementCallback, cb, o );
    XtAddCallback( sb, XmNtoTopCallback, cb, o );
    XtAddCallback( sb, XmNtoBottomCallback, cb, o );

    return (WXWidget)sb;
}

// Helper function
void wxWindow::CreateScrollbar(wxOrientation orientation)
{
    wxCHECK_RET( m_drawingArea, "this window can't have scrollbars" );

    XtVaSetValues( (Widget) m_scrolledWindow,
                   XmNresizePolicy, XmRESIZE_NONE,
                   NULL );

    wxColour backgroundColour = wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE);
    // Add scrollbars if required
    if (orientation == wxHORIZONTAL)
    {
        m_hScrollBar = DoCreateScrollBar( m_scrolledWindow, wxHORIZONTAL,
                                          (void (*)())wxScrollBarCallback );

        wxDoChangeBackgroundColour(m_hScrollBar, backgroundColour, TRUE);

        XtRealizeWidget( (Widget)m_hScrollBar );

        XtVaSetValues((Widget) m_scrolledWindow,
            XmNhorizontalScrollBar, (Widget) m_hScrollBar,
            NULL);

        wxAddWindowToTable( (Widget)m_hScrollBar, this );
    }
    else if (orientation == wxVERTICAL)
    {
        m_vScrollBar = DoCreateScrollBar( m_scrolledWindow, wxVERTICAL,
                                          (void (*)())wxScrollBarCallback );

        wxDoChangeBackgroundColour(m_vScrollBar, backgroundColour, TRUE);

        XtRealizeWidget((Widget)m_vScrollBar);

        XtVaSetValues((Widget) m_scrolledWindow,
            XmNverticalScrollBar, (Widget) m_vScrollBar,
            NULL);

        wxAddWindowToTable( (Widget)m_vScrollBar, this );
    }

    XtVaSetValues( (Widget) m_scrolledWindow,
                   XmNresizePolicy, XmRESIZE_ANY,
                   NULL );
}

void wxWindow::DestroyScrollbar(wxOrientation orientation)
{
    wxCHECK_RET( m_drawingArea, "this window can't have scrollbars" );

    XtVaSetValues((Widget) m_scrolledWindow,
                  XmNresizePolicy, XmRESIZE_NONE,
                  NULL);
    String stringSB = orientation == wxHORIZONTAL ?
        XmNhorizontalScrollBar : XmNverticalScrollBar;
    WXWidget* widgetSB = orientation == wxHORIZONTAL ?
        &m_hScrollBar : &m_vScrollBar;

    if( *widgetSB )
    {
        wxDeleteWindowFromTable( (Widget)*widgetSB );
        XtDestroyWidget( (Widget)*widgetSB );
        *widgetSB = (WXWidget)NULL;
    }

    XtVaSetValues( (Widget)m_scrolledWindow,
                   stringSB, (Widget) 0,
                   NULL );

    XtVaSetValues((Widget) m_scrolledWindow,
                  XmNresizePolicy, XmRESIZE_ANY,
                  NULL);
}

// ---------------------------------------------------------------------------
// basic operations
// ---------------------------------------------------------------------------

void wxWindow::SetFocus()
{
    Widget wMain = (Widget) GetMainWidget();
    XmProcessTraversal(wMain, XmTRAVERSE_CURRENT);
    XmProcessTraversal((Widget) GetMainWidget(), XmTRAVERSE_CURRENT);
}

// Get the window with the focus
wxWindow *wxWindowBase::FindFocus()
{
    // TODO Problems:
    // (1) Can there be multiple focussed widgets in an application?
    // In which case we need to find the top-level window that's
    // currently active.
    // (2) The widget with the focus may not be in the widget table
    // depending on which widgets I put in the table
    wxWindow *winFocus = (wxWindow *)NULL;
    for ( wxWindowList::Node *node = wxTopLevelWindows.GetFirst();
          node;
          node = node->GetNext() )
    {
        wxWindow *win = node->GetData();

        Widget w = XmGetFocusWidget ((Widget) win->GetTopWidget());

        if (w != (Widget) NULL)
        {
            winFocus = wxGetWindowFromTable(w);
            if ( winFocus )
                break;
        }
    }

    return winFocus;
}

bool wxWindow::Enable(bool enable)
{
    if ( !wxWindowBase::Enable(enable) )
        return FALSE;

    Widget wMain = (Widget)GetMainWidget();
    if ( wMain )
    {
        XtSetSensitive(wMain, enable);
        XmUpdateDisplay(wMain);
    }

    return TRUE;
}

bool wxWindow::Show(bool show)
{
    if ( !wxWindowBase::Show(show) )
        return FALSE;

    if (m_borderWidget || m_scrolledWindow)
    {
        MapOrUnmap(m_borderWidget ? m_borderWidget : m_scrolledWindow, show);
        // MapOrUnmap(m_drawingArea, show);
    }
    else
    {
        if ( !MapOrUnmap(GetTopWidget(), show) )
            MapOrUnmap(GetMainWidget(), show);
    }

    return TRUE;
}

// Raise the window to the top of the Z order
void wxWindow::Raise()
{
    Widget wTop = (Widget) GetTopWidget();
    Window window = XtWindow(wTop);
    XRaiseWindow(XtDisplay(wTop), window);
}

// Lower the window to the bottom of the Z order
void wxWindow::Lower()
{
    Widget wTop = (Widget) GetTopWidget();
    Window window = XtWindow(wTop);
    XLowerWindow(XtDisplay(wTop), window);
}

void wxWindow::SetTitle(const wxString& title)
{
    XtVaSetValues((Widget)GetMainWidget(), XmNtitle, title.c_str(), NULL);
}

wxString wxWindow::GetTitle() const
{
    char *title;
    XtVaGetValues((Widget)GetMainWidget(), XmNtitle, &title, NULL);

    return wxString(title);
}

void wxWindow::DoCaptureMouse()
{
    g_captureWindow = this;
    if ( m_winCaptured )
        return;

    Widget wMain = (Widget)GetMainWidget();
    if ( wMain )
        XtAddGrab(wMain, TRUE, FALSE);

    m_winCaptured = TRUE;
}

void wxWindow::DoReleaseMouse()
{
    g_captureWindow = NULL;
    if ( !m_winCaptured )
        return;

    Widget wMain = (Widget)GetMainWidget();
    if ( wMain )
        XtRemoveGrab(wMain);

    m_winCaptured = FALSE;
}

bool wxWindow::SetFont(const wxFont& font)
{
    if ( !wxWindowBase::SetFont(font) )
    {
        // nothing to do
        return FALSE;
    }

    ChangeFont();

    return TRUE;
}

bool wxWindow::SetCursor(const wxCursor& cursor)
{
    if ( !wxWindowBase::SetCursor(cursor) )
    {
        // no change
        return FALSE;
    }

    //    wxASSERT_MSG( m_cursor.Ok(),
    //                  wxT("cursor must be valid after call to the base version"));
    wxCursor* cursor2 = NULL;
    if (m_cursor.Ok())
        cursor2 = & m_cursor;
    else
        cursor2 = wxSTANDARD_CURSOR;

    WXDisplay *dpy = GetXDisplay();
    WXCursor x_cursor = cursor2->GetXCursor(dpy);

    Widget w = (Widget) GetMainWidget();
    Window win = XtWindow(w);
    XDefineCursor((Display*) dpy, win, (Cursor) x_cursor);

    return TRUE;
}

// Coordinates relative to the window
void wxWindow::WarpPointer (int x, int y)
{
    Widget wClient = (Widget)GetClientWidget();

    XWarpPointer(XtDisplay(wClient), None, XtWindow(wClient), 0, 0, 0, 0, x, y);
}

// ---------------------------------------------------------------------------
// scrolling stuff
// ---------------------------------------------------------------------------

int wxWindow::GetScrollPos(int orient) const
{
    if (orient == wxHORIZONTAL)
        return m_scrollPosX;
    else
        return m_scrollPosY;

#if 0
    Widget scrollBar = (Widget) ((orient == wxHORIZONTAL) ? m_hScrollBar : m_vScrollBar);
    if (scrollBar)
    {
        int pos;
        XtVaGetValues(scrollBar, XmNvalue, &pos, NULL);
        return pos;
    }
    else
        return 0;
#endif // 0
}

// This now returns the whole range, not just the number of positions that we
// can scroll.
int wxWindow::GetScrollRange(int orient) const
{
    Widget scrollBar = (Widget)GetScrollbar((wxOrientation)orient);
    wxCHECK_MSG( scrollBar, 0, "no such scrollbar" );

    int range = 0;
    if (scrollBar) 
        XtVaGetValues(scrollBar, XmNmaximum, &range, NULL);
    return range;
}

int wxWindow::GetScrollThumb(int orient) const
{
    Widget scrollBar = (Widget)GetScrollbar((wxOrientation)orient);
    wxCHECK_MSG( scrollBar, 0, "no such scrollbar" );

    int thumb;
    XtVaGetValues(scrollBar, XmNsliderSize, &thumb, NULL);
    return thumb;
}

void wxWindow::SetScrollPos(int orient, int pos, bool WXUNUSED(refresh))
{
    Widget scrollBar = (Widget)GetScrollbar((wxOrientation)orient);

    if ( scrollBar )
    {
        XtVaSetValues (scrollBar, XmNvalue, pos, NULL);
    }

    SetInternalScrollPos((wxOrientation)orient, pos);
}

// New function that will replace some of the above.
void wxWindow::SetScrollbar(int orient, int pos, int thumbVisible,
                            int range, bool WXUNUSED(refresh))
{
    int oldW, oldH;
    GetSize(& oldW, & oldH);

    if (range == 0)
        range = 1;
    if (thumbVisible == 0)
        thumbVisible = 1;

    if (thumbVisible > range)
        thumbVisible = range;

    // Save the old state to see if it changed
    WXWidget oldScrollBar = GetScrollbar((wxOrientation)orient);

    if (orient == wxHORIZONTAL)
    {
        if (thumbVisible == range)
        {
            if (m_hScrollBar)
                DestroyScrollbar(wxHORIZONTAL);
        }
        else
        {
            if (!m_hScrollBar)
                CreateScrollbar(wxHORIZONTAL);
        }
    }
    if (orient == wxVERTICAL)
    {
        if (thumbVisible == range)
        {
            if (m_vScrollBar)
                DestroyScrollbar(wxVERTICAL);
        }
        else
        {
            if (!m_vScrollBar)
                CreateScrollbar(wxVERTICAL);
        }
    }
    WXWidget newScrollBar =  GetScrollbar((wxOrientation)orient);

    if (oldScrollBar != newScrollBar)
    {
        // This is important! Without it, scrollbars misbehave badly.
        XtUnrealizeWidget((Widget) m_scrolledWindow);
        XmScrolledWindowSetAreas ((Widget) m_scrolledWindow, (Widget) m_hScrollBar, (Widget) m_vScrollBar, (Widget) m_drawingArea);
        XtRealizeWidget((Widget) m_scrolledWindow);
        XtManageChild((Widget) m_scrolledWindow);
    }

    if (newScrollBar)
    {
        XtVaSetValues((Widget) newScrollBar,
        XmNvalue, pos,
        XmNminimum, 0,
        XmNmaximum, range,
        XmNsliderSize, thumbVisible,
        NULL);
    }

    SetInternalScrollPos((wxOrientation)orient, pos);

    int newW, newH;
    GetSize(& newW, & newH);

    // Adjusting scrollbars can resize the canvas accidentally
    if (newW != oldW || newH != oldH)
        SetSize(-1, -1, oldW, oldH);
}

// Does a physical scroll
void wxWindow::ScrollWindow(int dx, int dy, const wxRect *rect)
{
    int x, y, w, h;
    if (rect)
    {
        // Use specified rectangle
        x = rect->x; y = rect->y; w = rect->width; h = rect->height;
    }
    else
    {
        // Use whole client area
        x = 0; y = 0;
        GetClientSize(& w, & h);
    }

    int x1 = (dx >= 0) ? x : x - dx;
    int y1 = (dy >= 0) ? y : y - dy;
    int w1 = w - abs(dx);
    int h1 = h - abs(dy);
    int x2 = (dx >= 0) ? x + dx : x;
    int y2 = (dy >= 0) ? y + dy : y;

    wxClientDC dc(this);

    dc.SetLogicalFunction (wxCOPY);

    Widget widget = (Widget) GetMainWidget();
    Window window = XtWindow(widget);
    Display* display = XtDisplay(widget);

    XCopyArea(display, window, window, (GC) dc.GetGC(),
              x1, y1, w1, h1, x2, y2);

    dc.SetAutoSetting(TRUE);
    wxBrush brush(GetBackgroundColour(), wxSOLID);
    dc.SetBrush(brush); // FIXME: needed?

    wxWindowList::Node *cnode = m_children.GetFirst();
    while (cnode)
    {
        wxWindow *child = cnode->GetData();
        int sx = 0;
        int sy = 0;
        child->GetSize( &sx, &sy );
        wxPoint pos( child->GetPosition() );
        child->SetSize( pos.x + dx, pos.y + dy, sx, sy, wxSIZE_ALLOW_MINUS_ONE );
        cnode = cnode->GetNext();
    }

    // We'll add rectangles to the list of update rectangles according to which
    // bits we've exposed.
    wxList updateRects;

    if (dx > 0)
    {
        wxRect *rect = new wxRect;
        rect->x = x;
        rect->y = y;
        rect->width = dx;
        rect->height = h;

        XFillRectangle(display, window,
            (GC) dc.GetGC(), rect->x, rect->y, rect->width, rect->height);

        rect->x = rect->x;
        rect->y = rect->y;
        rect->width = rect->width;
        rect->height = rect->height;

        updateRects.Append((wxObject*) rect);
    }
    else if (dx < 0)
    {
        wxRect *rect = new wxRect;

        rect->x = x + w + dx;
        rect->y = y;
        rect->width = -dx;
        rect->height = h;

        XFillRectangle(display, window,
            (GC) dc.GetGC(), rect->x, rect->y, rect->width,
            rect->height);

        rect->x = rect->x;
        rect->y = rect->y;
        rect->width = rect->width;
        rect->height = rect->height;

        updateRects.Append((wxObject*) rect);
    }
    if (dy > 0)
    {
        wxRect *rect = new wxRect;

        rect->x = x;
        rect->y = y;
        rect->width = w;
        rect->height = dy;

        XFillRectangle(display, window,
            (GC) dc.GetGC(), rect->x, rect->y, rect->width, rect->height);

        rect->x = rect->x;
        rect->y = rect->y;
        rect->width = rect->width;
        rect->height = rect->height;

        updateRects.Append((wxObject*) rect);
    }
    else if (dy < 0)
    {
        wxRect *rect = new wxRect;

        rect->x = x;
        rect->y = y + h + dy;
        rect->width = w;
        rect->height = -dy;

        XFillRectangle(display, window,
            (GC) dc.GetGC(), rect->x, rect->y, rect->width, rect->height);

        rect->x = rect->x;
        rect->y = rect->y;
        rect->width = rect->width;
        rect->height = rect->height;

        updateRects.Append((wxObject*) rect);
    }
    dc.SetBrush(wxNullBrush);

    // Now send expose events

    wxList::Node* node = updateRects.GetFirst();
    while (node)
    {
        wxRect* rect = (wxRect*) node->GetData();
        XExposeEvent event;

        event.type = Expose;
        event.display = display;
        event.send_event = True;
        event.window = window;

        event.x = rect->x;
        event.y = rect->y;
        event.width = rect->width;
        event.height = rect->height;

        event.count = 0;

        XSendEvent(display, window, False, ExposureMask, (XEvent *)&event);

        node = node->GetNext();

    }

    // Delete the update rects
    node = updateRects.GetFirst();
    while (node)
    {
        wxRect* rect = (wxRect*) node->GetData();
        delete rect;
        node = node->GetNext();
    }

    XmUpdateDisplay((Widget) GetMainWidget());
}

// ---------------------------------------------------------------------------
// drag and drop
// ---------------------------------------------------------------------------

#if wxUSE_DRAG_AND_DROP

void wxWindow::SetDropTarget(wxDropTarget * WXUNUSED(pDropTarget))
{
    // TODO
}

#endif

// Old style file-manager drag&drop
void wxWindow::DragAcceptFiles(bool WXUNUSED(accept))
{
    // TODO
}

// ----------------------------------------------------------------------------
// tooltips
// ----------------------------------------------------------------------------

#if wxUSE_TOOLTIPS

void wxWindow::DoSetToolTip(wxToolTip * WXUNUSED(tooltip))
{
    // TODO
}

#endif // wxUSE_TOOLTIPS

// ----------------------------------------------------------------------------
// popup menus
// ----------------------------------------------------------------------------

#if wxUSE_MENUS

bool wxWindow::DoPopupMenu(wxMenu *menu, int x, int y)
{
    Widget widget = (Widget) GetMainWidget();

    /* The menuId field seems to be usused, so we'll use it to
    indicate whether a menu is popped up or not:
    0: Not currently created as a popup
    -1: Created as a popup, but not active
    1: Active popup.
    */

    if (menu->GetParent() && (menu->GetId() != -1))
        return FALSE;

    if (menu->GetMainWidget())
    {
        menu->DestroyMenu(TRUE);
    }

    menu->SetId(1); /* Mark as popped-up */
    menu->CreateMenu(NULL, widget, menu);
    menu->SetInvokingWindow(this);

    menu->UpdateUI();

    //  menu->SetParent(parent);
    //  parent->children->Append(menu);  // Store menu for later deletion

    Widget menuWidget = (Widget) menu->GetMainWidget();

    int rootX = 0;
    int rootY = 0;

    int deviceX = x;
    int deviceY = y;
    /*
    if (this->IsKindOf(CLASSINFO(wxCanvas)))
    {
    wxCanvas *canvas = (wxCanvas *) this;
    deviceX = canvas->GetDC ()->LogicalToDeviceX (x);
    deviceY = canvas->GetDC ()->LogicalToDeviceY (y);
    }
    */

    Display *display = XtDisplay (widget);
    Window rootWindow = RootWindowOfScreen (XtScreen((Widget)widget));
    Window thisWindow = XtWindow (widget);
    Window childWindow;
    XTranslateCoordinates (display, thisWindow, rootWindow, (int) deviceX, (int) deviceY,
        &rootX, &rootY, &childWindow);

    XButtonPressedEvent event;
    event.type = ButtonPress;
    event.button = 1;

    event.x = deviceX;
    event.y = deviceY;

    event.x_root = rootX;
    event.y_root = rootY;

    XmMenuPosition (menuWidget, &event);
    XtManageChild (menuWidget);

    // The ID of a pop-up menu is 1 when active, and is set to 0 by the
    // idle-time destroy routine.
    // Waiting until this ID changes causes this function to block until
    // the menu has been dismissed and the widgets cleaned up.
    // In other words, once this routine returns, it is safe to delete
    // the menu object.
    // Ian Brown <ian.brown@printsoft.de>

    wxEventLoop evtLoop;

    while (menu->GetId() == 1)
    {
        wxDoEventLoopIteration( evtLoop );
    }

    return TRUE;
}

#endif

// ---------------------------------------------------------------------------
// moving and resizing
// ---------------------------------------------------------------------------

bool wxWindow::PreResize()
{
    return TRUE;
}

// Get total size
void wxWindow::DoGetSize(int *x, int *y) const
{
    Widget widget = (Widget)( !m_drawingArea ? GetTopWidget() :
                              ( m_borderWidget ? m_borderWidget :
                                m_scrolledWindow ? m_scrolledWindow :
                                m_drawingArea ) );
    Dimension xx, yy;

    XtVaGetValues( widget,
                   XmNwidth, &xx,
                   XmNheight, &yy,
                   NULL );
    if(x) *x = xx; 
    if(y) *y = yy;
}

void wxWindow::DoGetPosition(int *x, int *y) const
{
    Widget widget = (Widget)
        ( m_drawingArea ?
          ( m_borderWidget ? m_borderWidget : m_scrolledWindow ) : 
          GetTopWidget() );

    Position xx, yy;
    XtVaGetValues(widget, XmNx, &xx, XmNy, &yy, NULL);

    // We may be faking the client origin. So a window that's really at (0, 30)
    // may appear (to wxWin apps) to be at (0, 0).
    if (GetParent())
    {
        wxPoint pt(GetParent()->GetClientAreaOrigin());
        xx -= pt.x;
        yy -= pt.y;
    }

    if(x) *x = xx;
    if(y) *y = yy;
}

void wxWindow::DoScreenToClient(int *x, int *y) const
{
    Widget widget = (Widget) GetClientWidget();
    Display *display = XtDisplay((Widget) GetMainWidget());
    Window rootWindow = RootWindowOfScreen(XtScreen(widget));
    Window thisWindow = XtWindow(widget);

    Window childWindow;
    int xx = *x;
    int yy = *y;
    XTranslateCoordinates(display, rootWindow, thisWindow, xx, yy, x, y, &childWindow);
}

void wxWindow::DoClientToScreen(int *x, int *y) const
{
    Widget widget = (Widget) GetClientWidget();
    Display *display = XtDisplay(widget);
    Window rootWindow = RootWindowOfScreen(XtScreen(widget));
    Window thisWindow = XtWindow(widget);

    Window childWindow;
    int xx = *x;
    int yy = *y;
    XTranslateCoordinates(display, thisWindow, rootWindow, xx, yy, x, y, &childWindow);
}


// Get size *available for subwindows* i.e. excluding menu bar etc.
void wxWindow::DoGetClientSize(int *x, int *y) const
{
    Widget widget = (Widget) GetClientWidget();
    Dimension xx, yy;
    XtVaGetValues(widget, XmNwidth, &xx, XmNheight, &yy, NULL);
    if(x) *x = xx; if(y) *y = yy;
}

void wxWindow::DoSetSize(int x, int y, int width, int height, int sizeFlags)
{
    DoSetSizeIntr(x, y, width, height, sizeFlags, FALSE);
}

void wxWindow::DoSetSizeIntr(int x, int y, int width, int height,
                             int sizeFlags, bool fromCtor)
{
    // A bit of optimization to help sort out the flickers.
    int oldX = -1, oldY = -1, oldW = -1, oldH = -1;
    if( !fromCtor )
    {
        GetSize(& oldW, & oldH);
        GetPosition(& oldX, & oldY);
    }

    if ( !(sizeFlags & wxSIZE_ALLOW_MINUS_ONE) )
    {
        if ( x == -1 )
            x = oldX;
        if ( y == -1 )
            y = oldY;
    }

    wxSize size(-1, -1);
    if ( width <= 0 )
    {
        if ( ( sizeFlags & wxSIZE_AUTO_WIDTH ) && !fromCtor )
        {
            size = DoGetBestSize();
            width = size.x;
        }
        else
        {
            width = oldW;
        }
    }

    if ( height == -1 )
    {
        if( ( sizeFlags & wxSIZE_AUTO_HEIGHT ) && !fromCtor )
        {
            if( size.x == -1 ) size = DoGetBestSize();
            height = size.y;
        }
        else
        {
            height = oldH;
        }
    }

    if ( x != oldX || y != oldY || width != oldW || height != oldH
         || !wxNoOptimize::CanOptimize() )
    {
        if (m_drawingArea)
        {
            int flags = 0;

            if (x > -1 || (sizeFlags & wxSIZE_ALLOW_MINUS_ONE))
                flags |= wxMOVE_X;

            if (y > -1 || (sizeFlags & wxSIZE_ALLOW_MINUS_ONE))
                flags |= wxMOVE_Y;

            if (width > 0)
                flags |= wxMOVE_WIDTH;

            if (height > 0)
                flags |= wxMOVE_HEIGHT;

            int xx = x; int yy = y;
            AdjustForParentClientOrigin(xx, yy, sizeFlags);
            if( !fromCtor )
                DoMoveWindow( xx, yy, width, height );
            else
                DoMoveWindowIntr( xx, yy, width, height, flags );

            return;
        }

        Widget widget = (Widget) GetTopWidget();
        if (!widget)
            return;

        bool managed = XtIsManaged( widget );
        if (managed)
            XtUnmanageChild(widget);

        int xx = x;
        int yy = y;
        AdjustForParentClientOrigin(xx, yy, sizeFlags);

        DoMoveWindow(xx, yy, width, height);

        if (managed)
            XtManageChild(widget);
    }
}

void wxWindow::DoSetClientSize(int width, int height)
{
    if (m_drawingArea)
    {
        Widget drawingArea = (Widget) m_drawingArea;

        XtVaSetValues(drawingArea, XmNresizePolicy, XmRESIZE_ANY, NULL);

        if (width > -1)
            XtVaSetValues(drawingArea, XmNwidth, width, NULL);
        if (height > -1)
            XtVaSetValues(drawingArea, XmNheight, height, NULL);

        XtVaSetValues(drawingArea, XmNresizePolicy, XmRESIZE_NONE, NULL);
        return;
    }

    Widget widget = (Widget) GetTopWidget();

    if (width > -1)
        XtVaSetValues(widget, XmNwidth, width, NULL);
    if (height > -1)
        XtVaSetValues(widget, XmNheight, height, NULL);
}

// For implementation purposes - sometimes decorations make the client area
// smaller
wxPoint wxWindow::GetClientAreaOrigin() const
{
    return wxPoint(0, 0);
}

void wxWindow::DoMoveWindowIntr(int xx, int yy, int w, int h,
                                int flags)
{
    if (m_drawingArea)
    {
        Widget drawingArea = (Widget) m_drawingArea;
        Widget borderOrScrolled = m_borderWidget ?
            (Widget) m_borderWidget :
            (Widget) m_scrolledWindow;

        bool managed = XtIsManaged(borderOrScrolled);
        if (managed)
            XtUnmanageChild (borderOrScrolled);
        XtVaSetValues(drawingArea, XmNresizePolicy, XmRESIZE_ANY, NULL);

        if (flags & wxMOVE_X)
            XtVaSetValues (borderOrScrolled,
                           XmNx, xx,
                           NULL);
        if (flags & wxMOVE_Y)
            XtVaSetValues (borderOrScrolled,
                           XmNy, yy,
                           NULL);

        if (flags & wxMOVE_WIDTH)
        {
            if (m_borderWidget)
            {
                XtVaSetValues ((Widget) m_borderWidget, XmNwidth, w, NULL);
                short thick, margin;
                XtVaGetValues ((Widget) m_borderWidget,
                               XmNshadowThickness, &thick,
                               XmNmarginWidth, &margin,
                               NULL);
                w -= 2 * (thick + margin);
            }

            XtVaSetValues ((Widget) m_scrolledWindow, XmNwidth, w, NULL);
        }

        if (flags & wxMOVE_HEIGHT)
        {
            if (m_borderWidget)
            {
                XtVaSetValues ((Widget) m_borderWidget, XmNheight, h, NULL);
                short thick, margin;
                XtVaGetValues ((Widget) m_borderWidget,
                               XmNshadowThickness, &thick,
                               XmNmarginHeight, &margin,
                               NULL);
                h -= 2 * (thick + margin);
            }

            XtVaSetValues ((Widget) m_scrolledWindow, XmNheight, h, NULL);
        }

        if (managed)
            XtManageChild (borderOrScrolled);
        XtVaSetValues(drawingArea, XmNresizePolicy, XmRESIZE_NONE, NULL);
    }
    else
    {
        if( w < 1 ) w = 1;
        if( h < 1 ) h = 1;

        XtVaSetValues((Widget)GetTopWidget(),
                      XmNx, xx,
                      XmNy, yy,
                      XmNwidth, w,
                      XmNheight, h,
                      NULL);
    }
}

void wxWindow::DoMoveWindow(int x, int y, int width, int height)
{
    DoMoveWindowIntr (x, y, width, height,
                      wxMOVE_X|wxMOVE_Y|wxMOVE_WIDTH|wxMOVE_HEIGHT);
}

// ---------------------------------------------------------------------------
// text metrics
// ---------------------------------------------------------------------------

int wxWindow::GetCharHeight() const
{
    wxCHECK_MSG( m_font.Ok(), 0, "valid window font needed" );

    WXFontStructPtr pFontStruct = m_font.GetFontStruct(1.0, GetXDisplay());

    int direction, ascent, descent;
    XCharStruct overall;
    XTextExtents ((XFontStruct*) pFontStruct, "x", 1, &direction, &ascent,
        &descent, &overall);

    //  return (overall.ascent + overall.descent);
    return (ascent + descent);
}

int wxWindow::GetCharWidth() const
{
    wxCHECK_MSG( m_font.Ok(), 0, "valid window font needed" );

    WXFontStructPtr pFontStruct = m_font.GetFontStruct(1.0, GetXDisplay());

    int direction, ascent, descent;
    XCharStruct overall;
    XTextExtents ((XFontStruct*) pFontStruct, "x", 1, &direction, &ascent,
        &descent, &overall);

    return overall.width;
}

void wxWindow::GetTextExtent(const wxString& string,
                             int *x, int *y,
                             int *descent, int *externalLeading,
                             const wxFont *theFont) const
{
    wxFont *fontToUse = (wxFont *)theFont;
    if (!fontToUse)
        fontToUse = (wxFont *) & m_font;

    wxCHECK_RET( fontToUse->Ok(), "valid window font needed" );
    
    WXFontStructPtr pFontStruct = fontToUse->GetFontStruct(1.0, GetXDisplay());

    int direction, ascent, descent2;
    XCharStruct overall;
    int slen = string.Len();

#if 0
    if (use16)
        XTextExtents16((XFontStruct*) pFontStruct, (XChar2b *) (char*) (const char*) string, slen, &direction,
        &ascent, &descent2, &overall);
#endif

    XTextExtents((XFontStruct*) pFontStruct, string, slen,
                 &direction, &ascent, &descent2, &overall);

    if ( x )
        *x = (overall.width);
    if ( y )
        *y = (ascent + descent2);
    if (descent)
        *descent = descent2;
    if (externalLeading)
        *externalLeading = 0;

}

// ----------------------------------------------------------------------------
// painting
// ----------------------------------------------------------------------------

void wxWindow::AddUpdateRect(int x, int y, int w, int h)
{
    m_updateRegion.Union( x, y, w, h );
}

void wxWindow::Refresh(bool eraseBack, const wxRect *rect)
{
    m_needsRefresh = TRUE;
    Display *display = XtDisplay((Widget) GetMainWidget());
    Window thisWindow = XtWindow((Widget) GetMainWidget());

    XExposeEvent dummyEvent;
    int width, height;
    GetSize(&width, &height);

    dummyEvent.type = Expose;
    dummyEvent.display = display;
    dummyEvent.send_event = True;
    dummyEvent.window = thisWindow;
    if (rect)
    {
        dummyEvent.x = rect->x;
        dummyEvent.y = rect->y;
        dummyEvent.width = rect->width;
        dummyEvent.height = rect->height;
    }
    else
    {
        dummyEvent.x = 0;
        dummyEvent.y = 0;
        dummyEvent.width = width;
        dummyEvent.height = height;
    }
    dummyEvent.count = 0;

    if (eraseBack)
    {
        wxClientDC dc(this);
        wxBrush backgroundBrush(GetBackgroundColour(), wxSOLID);
        dc.SetBackground(backgroundBrush);
        if (rect)
            dc.Clear(*rect);
        else
            dc.Clear();
    }

    XSendEvent(display, thisWindow, False, ExposureMask, (XEvent *)&dummyEvent);
}

void wxWindow::Clear()
{
    wxClientDC dc(this);
    wxBrush brush(GetBackgroundColour(), wxSOLID);
    dc.SetBackground(brush);
    dc.Clear();
}

void wxWindow::DoPaint()
{
    //TODO : make a temporary gc so we can do the XCopyArea below
    if (m_backingPixmap && !m_needsRefresh)
    {
      wxPaintDC dc(this);

      GC tempGC = (GC) dc.GetBackingGC();

      Widget widget = (Widget) GetMainWidget();

      int scrollPosX = 0;
      int scrollPosY = 0;

      // We have to test whether it's a wxScrolledWindow (hack!) because
      // otherwise we don't know how many pixels have been scrolled. We might
      // solve this in the future by defining virtual wxWindow functions to get
      // the scroll position in pixels. Or, each kind of scrolled window has to
      // implement backing stores itself, using generic wxWindows code.
      wxScrolledWindow* scrolledWindow = wxDynamicCast(this, wxScrolledWindow);
      if ( scrolledWindow )
      {
          int x, y;
          scrolledWindow->CalcScrolledPosition(0, 0, &x, &y);

          scrollPosX = - x;
          scrollPosY = - y;
      }

      // TODO: This could be optimized further by only copying the areas in the
      //       current update region.

      // Only blit the part visible in the client area. The backing pixmap
      // always starts at 0, 0 but we may be looking at only a portion of it.
      wxSize clientArea = GetClientSize();
      int toBlitX = m_pixmapWidth - scrollPosX;
      int toBlitY = m_pixmapHeight - scrollPosY;

      // Copy whichever is samller, the amount of pixmap we have to copy,
      // or the size of the client area.
      toBlitX = wxMin(toBlitX, clientArea.x);
      toBlitY = wxMin(toBlitY, clientArea.y);

      // Make sure we're not negative
      toBlitX = wxMax(0, toBlitX);
      toBlitY = wxMax(0, toBlitY);

      XCopyArea
      (
       XtDisplay(widget),
       (Pixmap) m_backingPixmap,
       XtWindow (widget),
       tempGC,
       scrollPosX, scrollPosY, // Start at the scroll position
       toBlitX, toBlitY,       // How much of the pixmap to copy
       0, 0                    // Destination
      );
    }
    else
    {
        wxWindowDC dc(this);
        // Set an erase event first
        wxEraseEvent eraseEvent(GetId(), &dc);
        eraseEvent.SetEventObject(this);
        GetEventHandler()->ProcessEvent(eraseEvent);

        wxPaintEvent event(GetId());
        event.SetEventObject(this);
        GetEventHandler()->ProcessEvent(event);

        m_needsRefresh = FALSE;
    }
}

// ----------------------------------------------------------------------------
// event handlers
// ----------------------------------------------------------------------------

// Responds to colour changes: passes event on to children.
void wxWindow::OnSysColourChanged(wxSysColourChangedEvent& event)
{
    wxWindowList::Node *node = GetChildren().GetFirst();
    while ( node )
    {
        // Only propagate to non-top-level windows
        wxWindow *win = node->GetData();
        if ( win->GetParent() )
        {
            wxSysColourChangedEvent event2;
            event.m_eventObject = win;
            win->GetEventHandler()->ProcessEvent(event2);
        }

        node = node->GetNext();
    }
}

void wxWindow::OnIdle(wxIdleEvent& WXUNUSED(event))
{
    // This calls the UI-update mechanism (querying windows for
    // menu/toolbar/control state information)
    if (wxUpdateUIEvent::CanUpdate())
        UpdateWindowUI();
}

// ----------------------------------------------------------------------------
// accelerators
// ----------------------------------------------------------------------------

bool wxWindow::ProcessAccelerator(wxKeyEvent& event)
{
#if wxUSE_ACCEL
    if (!m_acceleratorTable.Ok())
        return FALSE;

    int count = m_acceleratorTable.GetCount();
    wxAcceleratorEntry* entries = m_acceleratorTable.GetEntries();
    int i;
    for (i = 0; i < count; i++)
    {
        wxAcceleratorEntry* entry = & (entries[i]);
        if (entry->MatchesEvent(event))
        {
            // Bingo, we have a match. Now find a control that matches the
            // entry command id.

            // Need to go up to the top of the window hierarchy, since it might
            // be e.g. a menu item
            wxWindow* parent = this;
            while ( parent && !parent->IsTopLevel() )
                parent = parent->GetParent();

            if (!parent)
                return FALSE;

            wxFrame* frame = wxDynamicCast(parent, wxFrame);
            if ( frame )
            {
#if wxUSE_MENUS
                // Try for a menu command
                if (frame->GetMenuBar())
                {
                    wxMenuItem* item = frame->GetMenuBar()->FindItem(entry->GetCommand());
                    if (item)
                    {
                        wxCommandEvent commandEvent(wxEVT_COMMAND_MENU_SELECTED, entry->GetCommand());
                        commandEvent.SetEventObject(frame);

                        // If ProcessEvent returns TRUE (it was handled), then
                        // the calling code will skip the event handling.
                        return frame->GetEventHandler()->ProcessEvent(commandEvent);
                    }
                }
#endif
            }

            // Find a child matching the command id
            wxWindow* child = parent->FindWindow(entry->GetCommand());

            // No such child
            if (!child)
                return FALSE;

            // Now we process those kinds of windows that we can.
            // For now, only buttons.
            if ( wxDynamicCast(child, wxButton) )
            {
                wxCommandEvent commandEvent (wxEVT_COMMAND_BUTTON_CLICKED, child->GetId());
                commandEvent.SetEventObject(child);
                return child->GetEventHandler()->ProcessEvent(commandEvent);
            }

            return FALSE;
        } // matches event
    }// for
#endif

    // We didn't match the key event against an accelerator.
    return FALSE;
}

// ============================================================================
// Motif-specific stuff from here on
// ============================================================================

// ----------------------------------------------------------------------------
// function which maintain the global hash table mapping Widgets to wxWindows
// ----------------------------------------------------------------------------

bool wxAddWindowToTable(Widget w, wxWindow *win)
{
    wxWindow *oldItem = NULL;
    if ((oldItem = (wxWindow *)wxWidgetHashTable->Get ((long) w)))
    {
        wxLogDebug("Widget table clash: new widget is %ld, %s",
                   (long)w, win->GetClassInfo()->GetClassName());
        return FALSE;
    }

    wxWidgetHashTable->Put((long) w, win);

    wxLogTrace("widget", "Widget 0x%p <-> window %p (%s)",
               (WXWidget)w, win, win->GetClassInfo()->GetClassName());

    return TRUE;
}

wxWindow *wxGetWindowFromTable(Widget w)
{
    return (wxWindow *)wxWidgetHashTable->Get((long) w);
}

void wxDeleteWindowFromTable(Widget w)
{
    wxLogTrace("widget", "Widget 0x%p", (WXWidget)w);

    wxWidgetHashTable->Delete((long)w);
}

// ----------------------------------------------------------------------------
// add/remove window from the table
// ----------------------------------------------------------------------------

// Add to hash table, add event handler
bool wxWindow::AttachWidget (wxWindow* WXUNUSED(parent), WXWidget mainWidget,
                             WXWidget formWidget, int x, int y, int width, int height)
{
    wxAddWindowToTable((Widget) mainWidget, this);
    XtAddEventHandler( (Widget) mainWidget,
                       ButtonPressMask | ButtonReleaseMask
                     | PointerMotionMask,
                       False,
                       wxPanelItemEventHandler,
                       (XtPointer) this);

    if (!formWidget)
    {
        XtTranslations ptr;
        XtOverrideTranslations ((Widget) mainWidget,
            ptr = XtParseTranslationTable ("<Configure>: resize()"));
        XtFree ((char *) ptr);
    }

    // Some widgets have a parent form widget, e.g. wxRadioBox
    if (formWidget)
    {
        if (!wxAddWindowToTable((Widget) formWidget, this))
            return FALSE;

        XtTranslations ptr;
        XtOverrideTranslations ((Widget) formWidget,
            ptr = XtParseTranslationTable ("<Configure>: resize()"));
        XtFree ((char *) ptr);
    }

    if (x == -1)
        x = 0;
    if (y == -1)
        y = 0;
    SetSize (x, y, width, height);

    return TRUE;
}

// Remove event handler, remove from hash table
bool wxWindow::DetachWidget(WXWidget widget)
{
    XtRemoveEventHandler( (Widget) widget,
                          ButtonPressMask | ButtonReleaseMask
                        | PointerMotionMask,
                          False,
                          wxPanelItemEventHandler,
                          (XtPointer)this);

    wxDeleteWindowFromTable((Widget) widget);
    return TRUE;
}

// ----------------------------------------------------------------------------
// Motif-specific accessors
// ----------------------------------------------------------------------------

// Get the underlying X window
WXWindow wxWindow::GetXWindow() const
{
    Widget wMain = (Widget)GetMainWidget();
    if ( wMain )
        return (WXWindow) XtWindow(wMain);
    else
        return (WXWindow) 0;
}

// Get the underlying X display
WXDisplay *wxWindow::GetXDisplay() const
{
    Widget wMain = (Widget)GetMainWidget();
    if ( wMain )
        return (WXDisplay*) XtDisplay(wMain);
    else
        return (WXDisplay*) NULL;
}

WXWidget wxWindow::GetMainWidget() const
{
    if (m_drawingArea)
        return m_drawingArea;
    else
        return m_mainWidget;
}

WXWidget wxWindow::GetClientWidget() const
{
    if (m_drawingArea != (WXWidget) 0)
        return m_drawingArea;
    else
        return GetMainWidget();
}

WXWidget wxWindow::GetTopWidget() const
{
    return GetMainWidget();
}

WXWidget wxWindow::GetLabelWidget() const
{
    return GetMainWidget();
}

// ----------------------------------------------------------------------------
// Motif callbacks
// ----------------------------------------------------------------------------

// All widgets should have this as their resize proc.
// OnSize sent to wxWindow via client data.
void wxWidgetResizeProc(Widget w, XConfigureEvent *WXUNUSED(event),
                        String WXUNUSED(args)[], int *WXUNUSED(num_args))
{
    wxWindow *win = wxGetWindowFromTable(w);
    if (!win)
        return;

    if (win->PreResize())
    {
        int width, height;
        win->GetSize(&width, &height);
        wxSizeEvent sizeEvent(wxSize(width, height), win->GetId());
        sizeEvent.SetEventObject(win);
        win->GetEventHandler()->ProcessEvent(sizeEvent);
    }
}

static void wxCanvasRepaintProc(Widget drawingArea,
                                XtPointer clientData,
                                XmDrawingAreaCallbackStruct * cbs)
{
    if (!wxGetWindowFromTable(drawingArea))
        return;

    XEvent * event = cbs->event;
    wxWindow * win = (wxWindow *) clientData;

    switch (event->type)
    {
    case Expose:
        {
            win->AddUpdateRect(event->xexpose.x, event->xexpose.y,
                               event->xexpose.width, event->xexpose.height);

            if (event -> xexpose.count == 0)
            {
                win->DoPaint();
            }
            break;
        }
    }
}

// Unable to deal with Enter/Leave without a separate EventHandler (Motif 1.1.4)
static void wxCanvasEnterLeave(Widget drawingArea,
                               XtPointer WXUNUSED(clientData),
                               XCrossingEvent * event)
{
    XmDrawingAreaCallbackStruct cbs;
    XEvent ev;

    ((XCrossingEvent &) ev) = *event;

    cbs.reason = XmCR_INPUT;
    cbs.event = &ev;

    wxCanvasInputEvent(drawingArea, (XtPointer) NULL, &cbs);
}

// Fix to make it work under Motif 1.0 (!)
static void wxCanvasMotionEvent (Widget WXUNUSED(drawingArea),
                                 XButtonEvent *WXUNUSED(event))
{
#if XmVersion <= 1000
    XmDrawingAreaCallbackStruct cbs;
    XEvent ev;

    ev = *((XEvent *) event);
    cbs.reason = XmCR_INPUT;
    cbs.event = &ev;

    wxCanvasInputEvent (drawingArea, (XtPointer) NULL, &cbs);
#endif // XmVersion <= 1000
}

static void wxCanvasInputEvent(Widget drawingArea,
                               XtPointer WXUNUSED(data),
                               XmDrawingAreaCallbackStruct * cbs)
{
    wxWindow *canvas = wxGetWindowFromTable(drawingArea);
    XEvent local_event;

    if (canvas==NULL)
        return;

    if (cbs->reason != XmCR_INPUT)
        return;

    local_event = *(cbs->event);    // We must keep a copy!

    switch (local_event.xany.type)
    {
    case EnterNotify:
    case LeaveNotify:
    case ButtonPress:
    case ButtonRelease:
    case MotionNotify:
    {
        wxMouseEvent wxevent;
        if(wxTranslateMouseEvent(wxevent, canvas, drawingArea, &local_event))
        {
            canvas->GetEventHandler()->ProcessEvent(wxevent);
        }
        break;
    }
    case KeyPress:
        {
            wxKeyEvent event (wxEVT_CHAR);
            if (wxTranslateKeyEvent (event, canvas, (Widget) 0, &local_event))
            {
                // Implement wxFrame::OnCharHook by checking ancestor.
                wxWindow *parent = canvas;
                while (parent && !parent->IsTopLevel())
                    parent = parent->GetParent();

                if (parent)
                {
                    event.SetEventType(wxEVT_CHAR_HOOK);
                    if (parent->GetEventHandler()->ProcessEvent(event))
                        return;
                }

                // For simplicity, OnKeyDown is the same as OnChar
                // TODO: filter modifier key presses from OnChar
                event.SetEventType(wxEVT_KEY_DOWN);

                // Only process OnChar if OnKeyDown didn't swallow it
                if (!canvas->GetEventHandler()->ProcessEvent (event))
                {
                    event.SetEventType(wxEVT_CHAR);
                    canvas->GetEventHandler()->ProcessEvent (event);
                }
            }
            break;
        }
    case KeyRelease:
        {
            wxKeyEvent event (wxEVT_KEY_UP);
            if (wxTranslateKeyEvent (event, canvas, (Widget) 0, &local_event))
            {
                canvas->GetEventHandler()->ProcessEvent (event);
            }
            break;
        }
    case FocusIn:
        {
            if (local_event.xfocus.detail != NotifyPointer)
            {
                wxFocusEvent event(wxEVT_SET_FOCUS, canvas->GetId());
                event.SetEventObject(canvas);
                canvas->GetEventHandler()->ProcessEvent(event);
            }
            break;
        }
    case FocusOut:
        {
            if (local_event.xfocus.detail != NotifyPointer)
            {
                wxFocusEvent event(wxEVT_KILL_FOCUS, canvas->GetId());
                event.SetEventObject(canvas);
                canvas->GetEventHandler()->ProcessEvent(event);
            }
            break;
        }
    default:
        break;
    }
}

static void wxPanelItemEventHandler(Widget    wid,
                                    XtPointer WXUNUSED(client_data),
                                    XEvent*   event,
                                    Boolean  *continueToDispatch)
{
    // Widget can be a label or the actual widget.

    wxWindow *window = wxGetWindowFromTable(wid);
    if (window)
    {
        wxMouseEvent wxevent(0);
        if (wxTranslateMouseEvent(wxevent, window, wid, event))
        {
            window->GetEventHandler()->ProcessEvent(wxevent);
        }
    }

    // TODO: probably the key to allowing default behaviour to happen. Say we
    // set a m_doDefault flag to FALSE at the start of this function. Then in
    // e.g. wxWindow::OnMouseEvent we can call Default() which sets this flag to
    // TRUE, indicating that default processing can happen. Thus, behaviour can
    // appear to be overridden just by adding an event handler and not calling
    // wxWindow::OnWhatever. ALSO, maybe we can use this instead of the current
    // way of handling drawing area events, to simplify things.
    *continueToDispatch = True;
}

static void wxScrollBarCallback(Widget scrollbar,
                                XtPointer clientData,
                                XmScrollBarCallbackStruct *cbs)
{
    wxWindow *win = wxGetWindowFromTable(scrollbar);
    wxOrientation orientation = (wxOrientation)(int)clientData;

    wxEventType eventType = wxEVT_NULL;
    switch (cbs->reason)
    {
    case XmCR_INCREMENT:
        {
            eventType = wxEVT_SCROLLWIN_LINEDOWN;
            break;
        }
    case XmCR_DECREMENT:
        {
            eventType = wxEVT_SCROLLWIN_LINEUP;
            break;
        }
    case XmCR_DRAG:
        {
            eventType = wxEVT_SCROLLWIN_THUMBTRACK;
            break;
        }
    case XmCR_VALUE_CHANGED:
        {
            eventType = wxEVT_SCROLLWIN_THUMBRELEASE;
            break;
        }
    case XmCR_PAGE_INCREMENT:
        {
            eventType = wxEVT_SCROLLWIN_PAGEDOWN;
            break;
        }
    case XmCR_PAGE_DECREMENT:
        {
            eventType = wxEVT_SCROLLWIN_PAGEUP;
            break;
        }
    case XmCR_TO_TOP:
        {
            eventType = wxEVT_SCROLLWIN_TOP;
            break;
        }
    case XmCR_TO_BOTTOM:
        {
            eventType = wxEVT_SCROLLWIN_BOTTOM;
            break;
        }
    default:
        {
            // Should never get here
            wxFAIL_MSG("Unknown scroll event.");
            break;
        }
    }

    wxScrollWinEvent event(eventType,
                           cbs->value,
                           orientation);
    event.SetEventObject( win );
    win->GetEventHandler()->ProcessEvent(event);
}

// For repainting arbitrary windows
void wxUniversalRepaintProc(Widget w, XtPointer WXUNUSED(c_data), XEvent *event, char *)
{
    Window window;
    Display *display;

    wxWindow* win = wxGetWindowFromTable(w);
    if (!win)
        return;

    switch(event -> type)
    {
    case Expose:
        {
            window = (Window) win -> GetXWindow();
            display = (Display *) win -> GetXDisplay();

            win->AddUpdateRect(event->xexpose.x, event->xexpose.y,
                               event->xexpose.width, event->xexpose.height);

            if (event -> xexpose.count == 0)
            {
                win->DoPaint();
            }

            break;
        }
    }
}

// ----------------------------------------------------------------------------
// TranslateXXXEvent() functions
// ----------------------------------------------------------------------------

bool wxTranslateMouseEvent(wxMouseEvent& wxevent, wxWindow *win,
                           Widget widget, XEvent *xevent)
{
    switch (xevent->xany.type)
    {
        case EnterNotify:
        case LeaveNotify:
#if 0
            fprintf(stderr, "Widget 0x%p <-> window %p (%s), %s\n",
                    (WXWidget)widget, win, win->GetClassInfo()->GetClassName(),
                    (xevent->xany.type == EnterNotify ? "ENTER" : "LEAVE"));
#endif
        case ButtonPress:
        case ButtonRelease:
        case MotionNotify:
        {
            wxEventType eventType = wxEVT_NULL;

            if (xevent->xany.type == LeaveNotify)
            {
                eventType = wxEVT_LEAVE_WINDOW;
            }
            if (xevent->xany.type == EnterNotify)
            {
                eventType = wxEVT_ENTER_WINDOW;
            }
            else if (xevent->xany.type == MotionNotify)
            {
                eventType = wxEVT_MOTION;
            }
            else if (xevent->xany.type == ButtonPress)
            {
                wxevent.SetTimestamp(xevent->xbutton.time);
                int button = 0;
                if (xevent->xbutton.button == Button1)
                {
                    eventType = wxEVT_LEFT_DOWN;
                    button = 1;
                }
                else if (xevent->xbutton.button == Button2)
                {
                    eventType = wxEVT_MIDDLE_DOWN;
                    button = 2;
                }
                else if (xevent->xbutton.button == Button3)
                {
                    eventType = wxEVT_RIGHT_DOWN;
                    button = 3;
                }

                // check for a double click
                //
                long dclickTime = XtGetMultiClickTime(xevent->xany.display);
                long ts = wxevent.GetTimestamp();

                int buttonLast = win->GetLastClickedButton();
                long lastTS = win->GetLastClickTime();
                if ( buttonLast && buttonLast == button &&
                     (ts - lastTS) < dclickTime )
                {
                    // I have a dclick
                    win->SetLastClick(0, ts);
                    if ( eventType == wxEVT_LEFT_DOWN )
                        eventType = wxEVT_LEFT_DCLICK;
                    else if ( eventType == wxEVT_MIDDLE_DOWN )
                        eventType = wxEVT_MIDDLE_DCLICK;
                    else if ( eventType == wxEVT_RIGHT_DOWN )
                        eventType = wxEVT_RIGHT_DCLICK;
                }
                else
                {
                    // not fast enough or different button
                    win->SetLastClick(button, ts);
                }
            }
            else if (xevent->xany.type == ButtonRelease)
            {
                if (xevent->xbutton.button == Button1)
                {
                    eventType = wxEVT_LEFT_UP;
                }
                else if (xevent->xbutton.button == Button2)
                {
                    eventType = wxEVT_MIDDLE_UP;
                }
                else if (xevent->xbutton.button == Button3)
                {
                    eventType = wxEVT_RIGHT_UP;
                }
                else
                    return FALSE;
            }
            else
            {
                return FALSE;
            }

            wxevent.SetEventType(eventType);

            Position x1, y1;
            XtVaGetValues(widget, XmNx, &x1, XmNy, &y1, NULL);

            int x2, y2;
            win->GetPosition(&x2, &y2);

            // The button x/y must be translated to wxWindows
            // window space - the widget might be a label or button,
            // within a form.
            int dx = 0;
            int dy = 0;
            if (widget != (Widget)win->GetMainWidget())
            {
                dx = x1;
                dy = y1;
            }

            wxevent.m_x = xevent->xbutton.x + dx;
            wxevent.m_y = xevent->xbutton.y + dy;

            wxevent.m_leftDown = ((eventType == wxEVT_LEFT_DOWN)
                || (event_left_is_down (xevent)
                && (eventType != wxEVT_LEFT_UP)));
            wxevent.m_middleDown = ((eventType == wxEVT_MIDDLE_DOWN)
                || (event_middle_is_down (xevent)
                && (eventType != wxEVT_MIDDLE_UP)));
            wxevent.m_rightDown = ((eventType == wxEVT_RIGHT_DOWN)
                || (event_right_is_down (xevent)
                && (eventType != wxEVT_RIGHT_UP)));

            wxevent.m_shiftDown = xevent->xbutton.state & ShiftMask;
            wxevent.m_controlDown = xevent->xbutton.state & ControlMask;
            wxevent.m_altDown = xevent->xbutton.state & Mod3Mask;
            wxevent.m_metaDown = xevent->xbutton.state & Mod1Mask;

            wxevent.SetId(win->GetId());
            wxevent.SetEventObject(win);

            return TRUE;
        }
    }
    return FALSE;
}

bool wxTranslateKeyEvent(wxKeyEvent& wxevent, wxWindow *win,
                         Widget WXUNUSED(widget), XEvent *xevent)
{
    switch (xevent->xany.type)
    {
    case KeyPress:
    case KeyRelease:
        {
            char buf[20];

            KeySym keySym;
            (void) XLookupString((XKeyEvent *)xevent, buf, 20, &keySym, NULL);
            int id = wxCharCodeXToWX (keySym);
            // id may be WXK_xxx code - these are outside ASCII range, so we
            // can't just use toupper() on id
            if (id >= 'a' && id <= 'z')
                id = toupper(id);

            if (xevent->xkey.state & ShiftMask)
                wxevent.m_shiftDown = TRUE;
            if (xevent->xkey.state & ControlMask)
                wxevent.m_controlDown = TRUE;
            if (xevent->xkey.state & Mod3Mask)
                wxevent.m_altDown = TRUE;
            if (xevent->xkey.state & Mod1Mask)
                wxevent.m_metaDown = TRUE;
            wxevent.SetEventObject(win);
            wxevent.m_keyCode = id;
            wxevent.SetTimestamp(xevent->xkey.time);

            wxevent.m_x = xevent->xbutton.x;
            wxevent.m_y = xevent->xbutton.y;

            if (id > -1)
                return TRUE;
            else
                return FALSE;
            break;
        }
    default:
        break;
    }
    return FALSE;
}

// ----------------------------------------------------------------------------
// Colour stuff
// ----------------------------------------------------------------------------

#define YAllocColor XAllocColor
XColor g_itemColors[5];
int wxComputeColours (Display *display, wxColour * back, wxColour * fore)
{
    int result;
    static XmColorProc colorProc;

    result = wxNO_COLORS;

    if (back)
    {
        g_itemColors[0].red = (((long) back->Red ()) << 8);
        g_itemColors[0].green = (((long) back->Green ()) << 8);
        g_itemColors[0].blue = (((long) back->Blue ()) << 8);
        g_itemColors[0].flags = DoRed | DoGreen | DoBlue;
        if (colorProc == (XmColorProc) NULL)
        {
            // Get a ptr to the actual function
            colorProc = XmSetColorCalculation ((XmColorProc) NULL);
            // And set it back to motif.
            XmSetColorCalculation (colorProc);
        }
        (*colorProc) (&g_itemColors[wxBACK_INDEX],
            &g_itemColors[wxFORE_INDEX],
            &g_itemColors[wxSELE_INDEX],
            &g_itemColors[wxTOPS_INDEX],
            &g_itemColors[wxBOTS_INDEX]);
        result = wxBACK_COLORS;
    }
    if (fore)
    {
        g_itemColors[wxFORE_INDEX].red = (((long) fore->Red ()) << 8);
        g_itemColors[wxFORE_INDEX].green = (((long) fore->Green ()) << 8);
        g_itemColors[wxFORE_INDEX].blue = (((long) fore->Blue ()) << 8);
        g_itemColors[wxFORE_INDEX].flags = DoRed | DoGreen | DoBlue;
        if (result == wxNO_COLORS)
            result = wxFORE_COLORS;
    }

    Display *dpy = display;
    Colormap cmap = (Colormap) wxTheApp->GetMainColormap((WXDisplay*) dpy);

    if (back)
    {
        /* 5 Colours to allocate */
        for (int i = 0; i < 5; i++)
            if (!YAllocColor (dpy, cmap, &g_itemColors[i]))
                result = wxNO_COLORS;
    }
    else if (fore)
    {
        /* Only 1 colour to allocate */
        if (!YAllocColor (dpy, cmap, &g_itemColors[wxFORE_INDEX]))
            result = wxNO_COLORS;
    }

    return result;
}

// Changes the foreground and background colours to be derived from the current
// background colour. To change the foreground colour, you must call
// SetForegroundColour explicitly.
void wxWindow::ChangeBackgroundColour()
{
    WXWidget mainWidget = GetMainWidget();
    if ( mainWidget )
        wxDoChangeBackgroundColour(mainWidget, m_backgroundColour);
}

void wxWindow::ChangeForegroundColour()
{
    WXWidget mainWidget = GetMainWidget();
    if ( mainWidget )
        wxDoChangeForegroundColour(mainWidget, m_foregroundColour);
    if ( m_scrolledWindow && mainWidget != m_scrolledWindow )
        wxDoChangeForegroundColour(m_scrolledWindow, m_foregroundColour);
}

bool wxWindow::SetBackgroundColour(const wxColour& col)
{
    if ( !wxWindowBase::SetBackgroundColour(col) )
        return FALSE;

    ChangeBackgroundColour();

    return TRUE;
}

bool wxWindow::SetForegroundColour(const wxColour& col)
{
    if ( !wxWindowBase::SetForegroundColour(col) )
        return FALSE;

    ChangeForegroundColour();

    return TRUE;
}

void wxWindow::ChangeFont(bool keepOriginalSize)
{
    // Note that this causes the widget to be resized back
    // to its original size! We therefore have to set the size
    // back again. TODO: a better way in Motif?
    Widget w = (Widget) GetLabelWidget(); // Usually the main widget
    if (w && m_font.Ok())
    {
        int width, height, width1, height1;
        GetSize(& width, & height);

        wxDoChangeFont( GetLabelWidget(), m_font );

        GetSize(& width1, & height1);
        if (keepOriginalSize && (width != width1 || height != height1))
        {
            SetSize(-1, -1, width, height);
        }
    }
}

// ----------------------------------------------------------------------------
// global functions
// ----------------------------------------------------------------------------

wxWindow *wxGetActiveWindow()
{
    // TODO
    wxFAIL_MSG("Not implemented");
    return NULL;
}

/* static */
wxWindow *wxWindowBase::GetCapture()
{
    return (wxWindow *)g_captureWindow;
}


// Find the wxWindow at the current mouse position, returning the mouse
// position.
wxWindow* wxFindWindowAtPointer(wxPoint& pt)
{
    return wxFindWindowAtPoint(wxGetMousePosition());
}

// Get the current mouse position.
wxPoint wxGetMousePosition()
{
    Display *display = wxGlobalDisplay();
    Window rootWindow = RootWindowOfScreen (DefaultScreenOfDisplay(display));
    Window rootReturn, childReturn;
    int rootX, rootY, winX, winY;
    unsigned int maskReturn;

    XQueryPointer (display,
                   rootWindow,
                   &rootReturn,
                   &childReturn,
                   &rootX, &rootY, &winX, &winY, &maskReturn);
    return wxPoint(rootX, rootY);
}


// ----------------------------------------------------------------------------
// wxNoOptimize: switch off size optimization
// ----------------------------------------------------------------------------

int wxNoOptimize::ms_count = 0;

