/////////////////////////////////////////////////////////////////////////////
// Name:        windows.cpp
// Purpose:     wxWindow
// Author:      Julian Smart
// Modified by:
// Created:     17/09/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "window.h"
#endif

#include "wx/setup.h"
#include "wx/menu.h"
#include "wx/dc.h"
#include "wx/dcclient.h"
#include "wx/utils.h"
#include "wx/app.h"
#include "wx/panel.h"
#include "wx/layout.h"
#include "wx/dialog.h"
#include "wx/listbox.h"
#include "wx/button.h"
#include "wx/settings.h"
#include "wx/msgdlg.h"
#include "wx/frame.h"
#include "wx/scrolwin.h"

#include "wx/menuitem.h"
#include "wx/log.h"

#if  wxUSE_DRAG_AND_DROP
#include "wx/dnd.h"
#endif

#include <Xm/Xm.h>

#include <Xm/DrawingA.h>
#include <Xm/ScrolledW.h>
#include <Xm/ScrollBar.h>
#include <Xm/Frame.h>
#include <Xm/Label.h>

#include "wx/motif/private.h"

#include <string.h>

#define SCROLL_MARGIN 4
void wxCanvasRepaintProc (Widget, XtPointer, XmDrawingAreaCallbackStruct * cbs);
void wxCanvasInputEvent (Widget drawingArea, XtPointer data, XmDrawingAreaCallbackStruct * cbs);
void wxCanvasMotionEvent (Widget, XButtonEvent * event);
void wxCanvasEnterLeave (Widget drawingArea, XtPointer clientData, XCrossingEvent * event);
static void wxScrollBarCallback(Widget widget, XtPointer clientData,
                                XmScaleCallbackStruct *cbs);
void wxPanelItemEventHandler (Widget    wid,
                              XtPointer client_data,
                              XEvent*   event,
                              Boolean *continueToDispatch);

#define event_left_is_down(x) ((x)->xbutton.state & Button1Mask)
#define event_middle_is_down(x) ((x)->xbutton.state & Button2Mask)
#define event_right_is_down(x) ((x)->xbutton.state & Button3Mask)

extern wxList wxPendingDelete;

#if !USE_SHARED_LIBRARY
IMPLEMENT_DYNAMIC_CLASS(wxWindow, wxEvtHandler)

BEGIN_EVENT_TABLE(wxWindow, wxEvtHandler)
EVT_CHAR(wxWindow::OnChar)
EVT_KEY_DOWN(wxWindow::OnKeyDown)
EVT_KEY_UP(wxWindow::OnKeyUp)
EVT_ERASE_BACKGROUND(wxWindow::OnEraseBackground)
EVT_SYS_COLOUR_CHANGED(wxWindow::OnSysColourChanged)
EVT_INIT_DIALOG(wxWindow::OnInitDialog)
EVT_IDLE(wxWindow::OnIdle)
END_EVENT_TABLE()

#endif


// Constructor
wxWindow::wxWindow()
{
    // Generic
    m_windowId = 0;
    m_windowStyle = 0;
    m_windowParent = NULL;
    m_windowEventHandler = this;
    m_windowName = "";
    m_windowCursor = *wxSTANDARD_CURSOR;
    m_children = new wxList;
    m_constraints = NULL;
    m_constraintsInvolvedIn = NULL;
    m_windowSizer = NULL;
    m_sizerParent = NULL;
    m_autoLayout = FALSE;
    m_windowValidator = NULL;
    m_defaultItem = NULL;
    m_returnCode = 0;
    m_caretWidth = 0; m_caretHeight = 0;
    m_caretEnabled = FALSE;
    m_caretShown = FALSE;
    // m_backgroundColour = wxSystemSettings::GetSystemColour(wxSYS_COLOUR_WINDOW) ; ;
    m_backgroundColour = wxSystemSettings::GetSystemColour(wxSYS_COLOUR_3DFACE) ;
    m_foregroundColour = *wxBLACK;
    
#if  wxUSE_DRAG_AND_DROP
    m_pDropTarget = NULL;
#endif
    m_clientObject = (wxClientData*) NULL;
    m_clientData = NULL;
    
    /// Motif-specific
    m_needsRefresh = TRUE;
    m_mainWidget = (WXWidget) 0;
    m_button1Pressed = FALSE;
    m_button2Pressed = FALSE;
    m_button3Pressed = FALSE;
    m_winCaptured = FALSE;
    m_isShown = TRUE;
    m_hScrollBar = (WXWidget) 0;
    m_vScrollBar = (WXWidget) 0;
    m_borderWidget = (WXWidget) 0;
    m_scrolledWindow = (WXWidget) 0;
    m_drawingArea = (WXWidget) 0;
    m_hScroll = FALSE;
    m_vScroll = FALSE;
    m_backingPixmap = (WXPixmap) 0;
    m_pixmapWidth = 0;
    m_pixmapHeight = 0;
    m_pixmapOffsetX = 0;
    m_pixmapOffsetY = 0;
    m_lastTS = 0;
    m_lastButton = 0;
    m_canAddEventHandler = FALSE;
    m_scrollPosX = 0;
    m_scrollPosY = 0;
}

// Destructor
wxWindow::~wxWindow()
{
    // Remove potential dangling pointer
    if (GetParent() && GetParent()->IsKindOf(CLASSINFO(wxPanel)))
    {
        wxPanel* panel = (wxPanel*) GetParent();
        if (panel->GetLastFocus() == this)
            panel->SetLastFocus((wxWindow*) NULL);
    }

    //// Motif-specific
    
    if (GetMainWidget())
        DetachWidget(GetMainWidget()); // Removes event handlers
    
    // If m_drawingArea, we're a fully-fledged window with drawing area, scrollbars etc. (what wxCanvas used to be)
    if (m_drawingArea)
    {
        // Destroy children before destroying self
        DestroyChildren();
        
        if (m_backingPixmap)
            XFreePixmap (XtDisplay ((Widget) GetMainWidget()), (Pixmap) m_backingPixmap);
        
        Widget w = (Widget) m_drawingArea;
        wxDeleteWindowFromTable(w);
        
        if (w)
            XtDestroyWidget(w);
        m_mainWidget = (WXWidget) 0;
        
        // Only if we're _really_ a canvas (not a dialog box/panel)
        if (m_scrolledWindow)
        {
            wxDeleteWindowFromTable((Widget) m_scrolledWindow);
        }
        
        if (m_hScrollBar)
        {
            XtUnmanageChild ((Widget) m_hScrollBar);
            XtDestroyWidget ((Widget) m_hScrollBar);
        }
        if (m_vScrollBar)
        {
            XtUnmanageChild ((Widget) m_vScrollBar);
            XtDestroyWidget ((Widget) m_vScrollBar);
        }
        if (m_scrolledWindow)
        {
            XtUnmanageChild ((Widget) m_scrolledWindow);
            XtDestroyWidget ((Widget) m_scrolledWindow);
        }
        
        if (m_borderWidget)
        {
            XtDestroyWidget ((Widget) m_borderWidget);
            m_borderWidget = (WXWidget) 0;
        }
    }
    
    //// Generic stuff
    
    // Have to delete constraints/sizer FIRST otherwise
    // sizers may try to look at deleted windows as they
    // delete themselves.
#if wxUSE_CONSTRAINTS
    DeleteRelatedConstraints();
    if (m_constraints)
    {
        // This removes any dangling pointers to this window
        // in other windows' constraintsInvolvedIn lists.
        UnsetConstraints(m_constraints);
        delete m_constraints;
        m_constraints = NULL;
    }
    if (m_windowSizer)
    {
        delete m_windowSizer;
        m_windowSizer = NULL;
    }
    // If this is a child of a sizer, remove self from parent
    if (m_sizerParent)
        m_sizerParent->RemoveChild((wxWindow *)this);
#endif
    
    if (m_windowParent)
        m_windowParent->RemoveChild(this);
    
    DestroyChildren();
    
    // Destroy the window
    if (GetMainWidget())
    {
        // If this line (XtDestroyWidget) causes a crash, you may comment it out.
        // Child widgets will get destroyed automatically when a frame
        // or dialog is destroyed, but before that you may get some memory
        // leaks and potential layout problems if you delete and then add
        // child windows.
        XtDestroyWidget((Widget) GetMainWidget());
        SetMainWidget((WXWidget) NULL);
    }
    
    delete m_children;
    m_children = NULL;
    
    // Just in case the window has been Closed, but
    // we're then deleting immediately: don't leave
    // dangling pointers.
    wxPendingDelete.DeleteObject(this);
    
    if ( m_windowValidator ) delete m_windowValidator;
    if (m_clientObject) delete m_clientObject;
    
    ClearUpdateRects();
}

// Destroy the window (delayed, if a managed window)
bool wxWindow::Destroy()
{
    delete this;
    return TRUE;
}

// Constructor
bool wxWindow::Create(wxWindow *parent, wxWindowID id,
                      const wxPoint& pos,
                      const wxSize& size,
                      long style,
                      const wxString& name)
{
    // Generic
    m_windowId = 0;
    m_windowStyle = 0;
    m_windowParent = NULL;
    m_windowEventHandler = this;
    m_windowName = "";
    m_windowCursor = *wxSTANDARD_CURSOR;
    m_constraints = NULL;
    m_constraintsInvolvedIn = NULL;
    m_windowSizer = NULL;
    m_sizerParent = NULL;
    m_autoLayout = FALSE;
    m_windowValidator = NULL;
#if wxUSE_DRAG_AND_DROP
    m_pDropTarget = NULL;
#endif
    m_caretWidth = 0; m_caretHeight = 0;
    m_caretEnabled = FALSE;
    m_caretShown = FALSE;
    m_minSizeX = -1;
    m_minSizeY = -1;
    m_maxSizeX = -1;
    m_maxSizeY = -1;
    m_defaultItem = NULL;
    m_windowParent = NULL;
    m_clientObject = (wxClientData*) NULL;
    m_clientData = NULL;
    
    // Motif-specific
    m_needsRefresh = TRUE;
    m_canAddEventHandler = FALSE;
    m_mainWidget = (WXWidget) 0;
    m_button1Pressed = FALSE;
    m_button2Pressed = FALSE;
    m_button3Pressed = FALSE;
    m_winCaptured = FALSE;
    m_isShown = TRUE;
    m_hScrollBar = (WXWidget) 0;
    m_vScrollBar = (WXWidget) 0;
    m_borderWidget = (WXWidget) 0;
    m_scrolledWindow = (WXWidget) 0;
    m_drawingArea = (WXWidget) 0;
    m_hScroll = FALSE;
    m_vScroll = FALSE;
    m_backingPixmap = (WXPixmap) 0;
    m_pixmapWidth = 0;
    m_pixmapHeight = 0;
    m_pixmapOffsetX = 0;
    m_pixmapOffsetY = 0;
    m_scrollPosX = 0;
    m_scrollPosY = 0;
    
    if (!parent)
        return FALSE;
    
    if (parent) parent->AddChild(this);
    
    m_returnCode = 0;
    
    SetName(name);
    
    if ( id == -1 )
        m_windowId = (int)NewControlId();
    else
        m_windowId = id;
    
    // m_backgroundColour = wxSystemSettings::GetSystemColour(wxSYS_COLOUR_WINDOW) ; ;
    m_backgroundColour = wxSystemSettings::GetSystemColour(wxSYS_COLOUR_3DFACE) ;
    m_foregroundColour = *wxBLACK;
    
    m_windowStyle = style;
    
    if ( id == -1 )
        m_windowId = (int)NewControlId();
    else
        m_windowId = id;
    
    //// TODO: we should probably optimize by only creating a
    //// a drawing area if we have one or more scrollbars (wxVSCROLL/wxHSCROLL).
    //// But for now, let's simplify things by always creating the
    //// drawing area, since otherwise the translations are different.
    
    // New translations for getting mouse motion feedback
    String translations =
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
    if (style & wxBORDER)
        m_borderWidget = (WXWidget) XtVaCreateManagedWidget ("canvasBorder",
        xmFrameWidgetClass, parentWidget,
        XmNshadowType, XmSHADOW_IN,
        NULL);
    
    m_scrolledWindow = (WXWidget) XtVaCreateManagedWidget ("scrolledWindow",
        xmScrolledWindowWidgetClass, m_borderWidget ? (Widget) m_borderWidget : parentWidget,
        XmNresizePolicy, XmRESIZE_NONE,
        XmNspacing, 0,
        XmNscrollingPolicy, XmAPPLICATION_DEFINED,
        //                                  XmNscrollBarDisplayPolicy, XmAS_NEEDED,
        NULL);
    
    XtTranslations ptr;
    m_drawingArea = (WXWidget) XtVaCreateWidget ((char*) (const char*) name,
        xmDrawingAreaWidgetClass, (Widget) m_scrolledWindow,
        XmNunitType, XmPIXELS,
        //					 XmNresizePolicy, XmRESIZE_ANY,
        XmNresizePolicy, XmRESIZE_NONE,
        XmNmarginHeight, 0,
        XmNmarginWidth, 0,
        XmNtranslations, ptr = XtParseTranslationTable (translations),
        NULL);
        /*
        if (GetWindowStyleFlag() & wxOVERRIDE_KEY_TRANSLATIONS)
        {
        XtFree ((char *) ptr);
        ptr = XtParseTranslationTable ("<Key>: DrawingAreaInput()");
        XtOverrideTranslations ((Widget) m_drawingArea, ptr);
        XtFree ((char *) ptr);
        }
    */
    
    wxAddWindowToTable((Widget) m_drawingArea, this);
    wxAddWindowToTable((Widget) m_scrolledWindow, this);
    
    /*
    * This order is very important in Motif 1.2.1
    *
    */
    
    XtRealizeWidget ((Widget) m_scrolledWindow);
    XtRealizeWidget ((Widget) m_drawingArea);
    XtManageChild ((Widget) m_drawingArea);
    
    XtOverrideTranslations ((Widget) m_drawingArea,
        ptr = XtParseTranslationTable ("<Configure>: resize()"));
    XtFree ((char *) ptr);
    
    XtAddCallback ((Widget) m_drawingArea, XmNexposeCallback, (XtCallbackProc) wxCanvasRepaintProc, (XtPointer) this);
    XtAddCallback ((Widget) m_drawingArea, XmNinputCallback, (XtCallbackProc) wxCanvasInputEvent, (XtPointer) this);
    
    /* TODO?
    display = XtDisplay (scrolledWindow);
    xwindow = XtWindow (drawingArea);
    */
    
    XtAddEventHandler ((Widget) m_drawingArea, PointerMotionHintMask | EnterWindowMask | LeaveWindowMask | FocusChangeMask,
        False, (XtEventHandler) wxCanvasEnterLeave, (XtPointer) this);
    
    // Scrolled widget needs to have its colour changed or we get
    // a little blue square where the scrollbars abutt
    wxColour backgroundColour = wxSystemSettings::GetSystemColour(wxSYS_COLOUR_3DFACE);
    DoChangeBackgroundColour(m_scrolledWindow, backgroundColour, TRUE);
    DoChangeBackgroundColour(m_drawingArea, backgroundColour, TRUE);
    
    XmScrolledWindowSetAreas ((Widget) m_scrolledWindow, (Widget) 0, (Widget) 0, (Widget) m_drawingArea);
    
    /*
    if (m_hScrollBar)
    XtRealizeWidget ((Widget) m_hScrollBar);
    if (m_vScrollBar)
    XtRealizeWidget ((Widget) m_vScrollBar);
    */
    
    // Without this, the cursor may not be restored properly
    // (e.g. in splitter sample).
    SetCursor(*wxSTANDARD_CURSOR);
    SetFont(wxSystemSettings::GetSystemFont(wxSYS_DEFAULT_GUI_FONT));
    SetSize(pos.x, pos.y, size.x, size.y);
    
    return TRUE;
}

// Helper function
void wxWindow::CreateScrollbar(int orientation)
{
    if (!m_drawingArea)
        return;
    
    XtVaSetValues((Widget) m_scrolledWindow, XmNresizePolicy, XmRESIZE_NONE, NULL);
    
    // Add scrollbars if required
    if (orientation == wxHORIZONTAL)
    {
        Widget hScrollBar = XtVaCreateManagedWidget ("hsb",
            xmScrollBarWidgetClass, (Widget) m_scrolledWindow,
            XmNorientation, XmHORIZONTAL,
            NULL);
        //	  XtAddCallback (hScrollBar, XmNvalueChangedCallback, (XtCallbackProc) wxScrollBarCallback, (XtPointer) XmHORIZONTAL);
        XtAddCallback (hScrollBar, XmNdragCallback, (XtCallbackProc) wxScrollBarCallback, (XtPointer) XmHORIZONTAL);
        XtAddCallback (hScrollBar, XmNincrementCallback, (XtCallbackProc) wxScrollBarCallback, (XtPointer) XmHORIZONTAL);
        XtAddCallback (hScrollBar, XmNdecrementCallback, (XtCallbackProc) wxScrollBarCallback, (XtPointer) XmHORIZONTAL);
        XtAddCallback (hScrollBar, XmNpageIncrementCallback, (XtCallbackProc) wxScrollBarCallback, (XtPointer) XmHORIZONTAL);
        XtAddCallback (hScrollBar, XmNpageDecrementCallback, (XtCallbackProc) wxScrollBarCallback, (XtPointer) XmHORIZONTAL);
        XtAddCallback (hScrollBar, XmNtoTopCallback, (XtCallbackProc) wxScrollBarCallback, (XtPointer) XmHORIZONTAL);
        XtAddCallback (hScrollBar, XmNtoBottomCallback, (XtCallbackProc) wxScrollBarCallback, (XtPointer) XmHORIZONTAL);
        
        XtVaSetValues (hScrollBar,
            XmNincrement, 1,
            XmNvalue, 0,
            NULL);
        
        m_hScrollBar = (WXWidget) hScrollBar;
        
        wxColour backgroundColour = wxSystemSettings::GetSystemColour(wxSYS_COLOUR_3DFACE);
        DoChangeBackgroundColour(m_hScrollBar, backgroundColour, TRUE);
        
        XtRealizeWidget(hScrollBar);
        
        XtVaSetValues((Widget) m_scrolledWindow,
            XmNhorizontalScrollBar, (Widget) m_hScrollBar,
            NULL);
        
        m_hScroll = TRUE;
    }
    
    if (orientation == wxVERTICAL)
    {
        Widget vScrollBar = XtVaCreateManagedWidget ("vsb",
            xmScrollBarWidgetClass, (Widget) m_scrolledWindow,
            XmNorientation, XmVERTICAL,
            NULL);
        //	  XtAddCallback (vScrollBar, XmNvalueChangedCallback, (XtCallbackProc) wxScrollBarCallback, (XtPointer) XmVERTICAL);
        XtAddCallback (vScrollBar, XmNdragCallback, (XtCallbackProc) wxScrollBarCallback, (XtPointer) XmVERTICAL);
        XtAddCallback (vScrollBar, XmNincrementCallback, (XtCallbackProc) wxScrollBarCallback, (XtPointer) XmVERTICAL);
        XtAddCallback (vScrollBar, XmNdecrementCallback, (XtCallbackProc) wxScrollBarCallback, (XtPointer) XmVERTICAL);
        XtAddCallback (vScrollBar, XmNpageIncrementCallback, (XtCallbackProc) wxScrollBarCallback, (XtPointer) XmVERTICAL);
        XtAddCallback (vScrollBar, XmNpageDecrementCallback, (XtCallbackProc) wxScrollBarCallback, (XtPointer) XmVERTICAL);
        XtAddCallback (vScrollBar, XmNtoTopCallback, (XtCallbackProc) wxScrollBarCallback, (XtPointer) XmVERTICAL);
        XtAddCallback (vScrollBar, XmNtoBottomCallback, (XtCallbackProc) wxScrollBarCallback, (XtPointer) XmVERTICAL);
        
        XtVaSetValues (vScrollBar,
            XmNincrement, 1,
            XmNvalue, 0,
            NULL);
        
        m_vScrollBar = (WXWidget) vScrollBar;
        wxColour backgroundColour = wxSystemSettings::GetSystemColour(wxSYS_COLOUR_3DFACE);
        DoChangeBackgroundColour(m_vScrollBar, backgroundColour, TRUE);
        
        XtRealizeWidget(vScrollBar);
        
        XtVaSetValues((Widget) m_scrolledWindow,
            XmNverticalScrollBar, (Widget) m_vScrollBar,
            NULL);
        
        m_vScroll = TRUE;
    }
    
    XtVaSetValues((Widget) m_scrolledWindow, XmNresizePolicy, XmRESIZE_ANY, NULL);
}
void wxWindow::DestroyScrollbar(int orientation)
{
    if (!m_drawingArea)
        return;
    
    XtVaSetValues((Widget) m_scrolledWindow, XmNresizePolicy, XmRESIZE_NONE, NULL);
    // Add scrollbars if required
    if (orientation == wxHORIZONTAL)
    {
        if (m_hScrollBar)
        {
            XtDestroyWidget((Widget) m_hScrollBar);
        }
        m_hScrollBar = (WXWidget) 0;
        m_hScroll = FALSE;
        
        XtVaSetValues((Widget) m_scrolledWindow,
            XmNhorizontalScrollBar, (Widget) 0,
            NULL);
        
    }
    
    if (orientation == wxVERTICAL)
    {
        if (m_vScrollBar)
        {
            XtDestroyWidget((Widget) m_vScrollBar);
        }
        m_vScrollBar = (WXWidget) 0;
        m_vScroll = TRUE;
        
        XtVaSetValues((Widget) m_scrolledWindow,
            XmNverticalScrollBar, (Widget) 0,
            NULL);
        
    }
    XtVaSetValues((Widget) m_scrolledWindow, XmNresizePolicy, XmRESIZE_ANY, NULL);
}

void wxWindow::SetFocus()
{
    XmProcessTraversal((Widget) GetMainWidget(), XmTRAVERSE_CURRENT);
    XmProcessTraversal((Widget) GetMainWidget(), XmTRAVERSE_CURRENT);
}

void wxWindow::Enable(bool enable)
{
    if (GetMainWidget())
    {
        XtSetSensitive((Widget) GetMainWidget(), enable);
        XmUpdateDisplay((Widget) GetMainWidget());
    }
}

void wxWindow::CaptureMouse()
{
    if (m_winCaptured)
        return;
    
    if (GetMainWidget())
        XtAddGrab((Widget) GetMainWidget(), TRUE, FALSE);
    
    m_winCaptured = TRUE;
}

void wxWindow::ReleaseMouse()
{
    if (!m_winCaptured)
        return;
    
    if (GetMainWidget())
        XtRemoveGrab((Widget) GetMainWidget());
    m_winCaptured = FALSE;
}

// Push/pop event handler (i.e. allow a chain of event handlers
// be searched)
void wxWindow::PushEventHandler(wxEvtHandler *handler)
{
    handler->SetNextHandler(GetEventHandler());
    SetEventHandler(handler);
}

wxEvtHandler *wxWindow::PopEventHandler(bool deleteHandler)
{
    if ( GetEventHandler() )
    {
        wxEvtHandler *handlerA = GetEventHandler();
        wxEvtHandler *handlerB = handlerA->GetNextHandler();
        handlerA->SetNextHandler(NULL);
        SetEventHandler(handlerB);
        if ( deleteHandler )
        {
            delete handlerA;
            return NULL;
        }
        else
            return handlerA;
    }
    else
        return NULL;
}

#if    wxUSE_DRAG_AND_DROP

void wxWindow::SetDropTarget(wxDropTarget *pDropTarget)
{
    if ( m_pDropTarget != 0 ) {
        delete m_pDropTarget;
    }
    
    m_pDropTarget = pDropTarget;
    if ( m_pDropTarget != 0 )
    {
        // TODO
    }
}

#endif

// Old style file-manager drag&drop
void wxWindow::DragAcceptFiles(bool accept)
{
    // TODO
}

// Get total size
void wxWindow::GetSize(int *x, int *y) const
{
    if (m_drawingArea)
    {
        CanvasGetSize(x, y);
        return;
    }
    
    Widget widget = (Widget) GetTopWidget();
    Dimension xx, yy;
    XtVaGetValues(widget, XmNwidth, &xx, XmNheight, &yy, NULL);
    *x = xx; *y = yy;
}

void wxWindow::GetPosition(int *x, int *y) const
{
    if (m_drawingArea)
    {
        CanvasGetPosition(x, y);
        return;
    }
    Widget widget = (Widget) GetTopWidget();
    Position xx, yy;
    XtVaGetValues(widget, XmNx, &xx, XmNy, &yy, NULL);
    
    // We may be faking the client origin.
    // So a window that's really at (0, 30) may appear
    // (to wxWin apps) to be at (0, 0).
    if (GetParent())
    {
        wxPoint pt(GetParent()->GetClientAreaOrigin());
        xx -= pt.x;
        yy -= pt.y;
    }
    
    *x = xx; *y = yy;
}

void wxWindow::ScreenToClient(int *x, int *y) const
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

void wxWindow::ClientToScreen(int *x, int *y) const
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

void wxWindow::SetCursor(const wxCursor& cursor)
{
    m_windowCursor = cursor;
    if (m_windowCursor.Ok())
    {
        WXDisplay *dpy = GetXDisplay();
        WXCursor x_cursor = ((wxCursor&)cursor).GetXCursor(dpy);
        
        Widget w = (Widget) GetMainWidget();
        Window win = XtWindow(w);
        XDefineCursor((Display*) dpy, win, (Cursor) x_cursor);
    }
}


// Get size *available for subwindows* i.e. excluding menu bar etc.
void wxWindow::GetClientSize(int *x, int *y) const
{
    Widget widget = (Widget) GetClientWidget();
    Dimension xx, yy;
    XtVaGetValues(widget, XmNwidth, &xx, XmNheight, &yy, NULL);
    *x = xx; *y = yy;
}

void wxWindow::SetSize(int x, int y, int width, int height, int sizeFlags)
{
    // A bit of optimization to help sort out the flickers.
    int oldX, oldY, oldW, oldH;
    GetSize(& oldW, & oldH);
    GetPosition(& oldX, & oldY);
    
    bool useOldPos = FALSE;
    bool useOldSize = FALSE;
    
    if ((x == -1) && (x == -1) && ((sizeFlags & wxSIZE_ALLOW_MINUS_ONE) == 0))
        useOldPos = TRUE;
    else if (x == oldX && y == oldY)
        useOldPos = TRUE;
    
    if ((width == -1) && (height == -1))
        useOldSize = TRUE;
    else if (width == oldW && height == oldH)
        useOldSize = TRUE;
    
    if (!wxNoOptimize::CanOptimize())
    {
        useOldSize = FALSE; useOldPos = FALSE;
    }
    
    if (useOldPos && useOldSize)
        return;
    
    if (m_drawingArea)
    {
        CanvasSetSize(x, y, width, height, sizeFlags);
        return;
    }
    Widget widget = (Widget) GetTopWidget();
    if (!widget)
        return;
    
    bool managed = XtIsManaged( widget );
    if (managed)
        XtUnmanageChild(widget);
    
    int xx = x; int yy = y;
    AdjustForParentClientOrigin(xx, yy, sizeFlags);
    
    if (!useOldPos)
    {
        if (x > -1 || (sizeFlags & wxSIZE_ALLOW_MINUS_ONE))
            XtVaSetValues(widget, XmNx, xx, NULL);
        if (y > -1 || (sizeFlags & wxSIZE_ALLOW_MINUS_ONE))
            XtVaSetValues(widget, XmNy, yy, NULL);
    }
    if (!useOldSize)
    {
        if (width > -1)
            XtVaSetValues(widget, XmNwidth, width, NULL);
        if (height > -1)
            XtVaSetValues(widget, XmNheight, height, NULL);
    }
    
    if (managed)
        XtManageChild(widget);
    
    // How about this bit. Maybe we don't need to generate size events
    // all the time -- they'll be generated when the window is sized anyway.
    /*
    wxSizeEvent sizeEvent(wxSize(width, height), GetId());
    sizeEvent.SetEventObject(this);
    
      GetEventHandler()->ProcessEvent(sizeEvent);
    */
}

void wxWindow::SetClientSize(int width, int height)
{
    if (m_drawingArea)
    {
        CanvasSetClientSize(width, height);
        return;
    }
    
    Widget widget = (Widget) GetTopWidget();
    
    if (width > -1)
        XtVaSetValues(widget, XmNwidth, width, NULL);
    if (height > -1)
        XtVaSetValues(widget, XmNheight, height, NULL);
    
    wxSizeEvent sizeEvent(wxSize(width, height), GetId());
    sizeEvent.SetEventObject(this);
    
    GetEventHandler()->ProcessEvent(sizeEvent);
}

// For implementation purposes - sometimes decorations make the client area
// smaller
wxPoint wxWindow::GetClientAreaOrigin() const
{
    return wxPoint(0, 0);
}

// Makes an adjustment to the window position (for example, a frame that has
// a toolbar that it manages itself).
void wxWindow::AdjustForParentClientOrigin(int& x, int& y, int sizeFlags)
{
    if (((sizeFlags & wxSIZE_NO_ADJUSTMENTS) == 0) && GetParent())
    {
        wxPoint pt(GetParent()->GetClientAreaOrigin());
        x += pt.x; y += pt.y;
    }
}

bool wxWindow::Show(bool show)
{
    if (show)
    {
        if (m_borderWidget || m_scrolledWindow)
        {
            if (m_drawingArea)
                XtMapWidget((Widget) m_drawingArea);
            XtMapWidget(m_borderWidget ? (Widget) m_borderWidget : (Widget) m_scrolledWindow);
        }
        else
        {
            WXWidget topWidget = GetTopWidget();
            if (GetTopWidget())
                XtMapWidget((Widget) GetTopWidget());
            else if (GetMainWidget())
                XtMapWidget((Widget) GetMainWidget());
        }
    }
    else
    {
        if (m_borderWidget || m_scrolledWindow)
        {
            if (m_drawingArea)
                XtUnmapWidget((Widget) m_drawingArea);
            XtUnmapWidget(m_borderWidget ? (Widget) m_borderWidget : (Widget) m_scrolledWindow);
        }
        else
        {
            if (GetTopWidget())
                XtUnmapWidget((Widget) GetTopWidget());
            else if (GetMainWidget())
                XtUnmapWidget((Widget) GetMainWidget());
        }
    }
    
    /*
    Window xwin = (Window) GetXWindow();
    Display *xdisp = (Display*) GetXDisplay();
    if (show)
    XMapWindow(xdisp, xwin);
    else
    XUnmapWindow(xdisp, xwin);
    */
    
    m_isShown = show;
    
    return TRUE;
}

bool wxWindow::IsShown() const
{
    return m_isShown;
}

int wxWindow::GetCharHeight() const
{
    if (!m_windowFont.Ok())
        return 0;
    
    WXFontStructPtr pFontStruct = m_windowFont.GetFontStruct(1.0, GetXDisplay());
    
    int direction, ascent, descent;
    XCharStruct overall;
    XTextExtents ((XFontStruct*) pFontStruct, "x", 1, &direction, &ascent,
        &descent, &overall);
    //  return (overall.ascent + overall.descent);
    return (ascent + descent);
}

int wxWindow::GetCharWidth() const
{
    if (!m_windowFont.Ok())
        return 0;
    
    WXFontStructPtr pFontStruct = m_windowFont.GetFontStruct(1.0, GetXDisplay());
    
    int direction, ascent, descent;
    XCharStruct overall;
    XTextExtents ((XFontStruct*) pFontStruct, "x", 1, &direction, &ascent,
        &descent, &overall);
    return overall.width;
}

/* Helper function for 16-bit fonts */
static int str16len(const char *s)
{
    int count = 0;
    
    while (s[0] && s[1]) {
        count++;
        s += 2;
    }
    
    return count;
}

void wxWindow::GetTextExtent(const wxString& string, int *x, int *y,
                             int *descent, int *externalLeading, const wxFont *theFont, bool use16) const
{
    wxFont *fontToUse = (wxFont *)theFont;
    if (!fontToUse)
        fontToUse = (wxFont *) & m_windowFont;
    
    if (!fontToUse->Ok())
        return;
    
    WXFontStructPtr pFontStruct = theFont->GetFontStruct(1.0, GetXDisplay());
    
    int direction, ascent, descent2;
    XCharStruct overall;
    int slen;
    
    if (use16) slen = str16len(string); else slen = strlen(string);
    
    if (use16)
        XTextExtents16((XFontStruct*) pFontStruct, (XChar2b *) (char*) (const char*) string, slen, &direction,
        &ascent, &descent2, &overall);
    else
        XTextExtents((XFontStruct*) pFontStruct, (char*) (const char*) string, slen, &direction,
        &ascent, &descent2, &overall);
    
    *x = (overall.width);
    *y = (ascent + descent2);
    if (descent)
        *descent = descent2;
    if (externalLeading)
        *externalLeading = 0;
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

// Responds to colour changes: passes event on to children.
void wxWindow::OnSysColourChanged(wxSysColourChangedEvent& event)
{
    wxNode *node = GetChildren().First();
    while ( node )
    {
        // Only propagate to non-top-level windows
        wxWindow *win = (wxWindow *)node->Data();
        if ( win->GetParent() )
        {
            wxSysColourChangedEvent event2;
            event.m_eventObject = win;
            win->GetEventHandler()->ProcessEvent(event2);
        }
        
        node = node->Next();
    }
}

// This can be called by the app (or wxWindows) to do default processing for the current
// event. Save message/event info in wxWindow so they can be used in this function.
long wxWindow::Default()
{
    // TODO
    return 0;
}

void wxWindow::InitDialog()
{
    wxInitDialogEvent event(GetId());
    event.SetEventObject( this );
    GetEventHandler()->ProcessEvent(event);
}

// Default init dialog behaviour is to transfer data to window
void wxWindow::OnInitDialog(wxInitDialogEvent& event)
{
    TransferDataToWindow();
}

// Caret manipulation
void wxWindow::CreateCaret(int w, int h)
{
    m_caretWidth = w;
    m_caretHeight = h;
    m_caretEnabled = TRUE;
}

void wxWindow::CreateCaret(const wxBitmap *WXUNUSED(bitmap))
{
    // TODO
}

void wxWindow::ShowCaret(bool show)
{
    // TODO
}

void wxWindow::DestroyCaret()
{
    // TODO
    m_caretEnabled = FALSE;
}

void wxWindow::SetCaretPos(int x, int y)
{
    // TODO
}

void wxWindow::GetCaretPos(int *x, int *y) const
{
    // TODO
}

wxWindow *wxGetActiveWindow()
{
    // TODO
    return NULL;
}

void wxWindow::SetSizeHints(int minW, int minH, int maxW, int maxH, int incW, int incH)
{
    m_minSizeX = minW;
    m_minSizeY = minH;
    m_maxSizeX = maxW;
    m_maxSizeY = maxH;
    
    if (!this->IsKindOf(CLASSINFO(wxFrame)))
        return;
    
    wxFrame *frame = (wxFrame *)this;
    Widget widget = (Widget) frame->GetShellWidget();
    
    if (minW > -1)
        XtVaSetValues(widget, XmNminWidth, minW, NULL);
    if (minH > -1)
        XtVaSetValues(widget, XmNminHeight, minH, NULL);
    if (maxW > -1)
        XtVaSetValues(widget, XmNmaxWidth, maxW, NULL);
    if (maxH > -1)
        XtVaSetValues(widget, XmNmaxHeight, maxH, NULL);
    if (incW > -1)
        XtVaSetValues(widget, XmNwidthInc, incW, NULL);
    if (incH > -1)
        XtVaSetValues(widget, XmNheightInc, incH, NULL);
}

void wxWindow::Centre(int direction)
{
    int x, y, width, height, panel_width, panel_height, new_x, new_y;
    
    wxWindow *father = (wxWindow *)GetParent();
    if (!father)
        return;
    
    father->GetClientSize(&panel_width, &panel_height);
    GetSize(&width, &height);
    GetPosition(&x, &y);
    
    new_x = -1;
    new_y = -1;
    
    if (direction & wxHORIZONTAL)
        new_x = (int)((panel_width - width)/2);
    
    if (direction & wxVERTICAL)
        new_y = (int)((panel_height - height)/2);
    
    SetSize(new_x, new_y, -1, -1);
    
}

// Coordinates relative to the window
void wxWindow::WarpPointer (int x, int y)
{
    XWarpPointer (XtDisplay((Widget) GetClientWidget()), None, XtWindow((Widget) GetClientWidget()), 0, 0, 0, 0, x, y);
}

void wxWindow::OnEraseBackground(wxEraseEvent& event)
{
    // TODO
    Default();
}

int wxWindow::GetScrollPos(int orient) const
{
    if (orient == wxHORIZONTAL)
        return m_scrollPosX;
    else
        return m_scrollPosY;
        /*
        Widget scrollBar = (Widget) ((orient == wxHORIZONTAL) ? m_hScrollBar : m_vScrollBar);
        if (scrollBar)
        {
        int pos;
        XtVaGetValues(scrollBar,
        XmNvalue, &pos, NULL);
        return pos;
        }
        else
        return 0;
    */
}

// This now returns the whole range, not just the number
// of positions that we can scroll.
int wxWindow::GetScrollRange(int orient) const
{
    Widget scrollBar = (Widget) ((orient == wxHORIZONTAL) ? m_hScrollBar : m_vScrollBar);
    if (scrollBar)
    {
        int range;
        XtVaGetValues(scrollBar,
            XmNmaximum, &range, NULL);
        return range;
    }
    else
        return 0;
}

int wxWindow::GetScrollThumb(int orient) const
{
    Widget scrollBar = (Widget) ((orient == wxHORIZONTAL) ? m_hScrollBar : m_vScrollBar);
    if (scrollBar)
    {
        int thumb;
        XtVaGetValues(scrollBar,
            XmNsliderSize, &thumb, NULL);
        return thumb;
    }
    else
        return 0;
}

void wxWindow::SetScrollPos(int orient, int pos, bool WXUNUSED(refresh))
{
    Widget scrollBar = (Widget) ((orient == wxHORIZONTAL) ? m_hScrollBar : m_vScrollBar);
    if (scrollBar)
    {
        XtVaSetValues (scrollBar,
            XmNvalue, pos,
            NULL);
    }
    if (orient == wxHORIZONTAL)
        m_scrollPosX = pos;
    else
        m_scrollPosY = pos;
    
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
    WXWidget oldScrollBar =  ((orient == wxHORIZONTAL) ? m_hScrollBar : m_vScrollBar );
    
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
    WXWidget newScrollBar =  ((orient == wxHORIZONTAL) ? m_hScrollBar : m_vScrollBar );
    
    if (oldScrollBar != newScrollBar)
    {
        // This is important! Without it, scrollbars misbehave
        // badly.
        XtUnrealizeWidget((Widget) m_scrolledWindow);
        XmScrolledWindowSetAreas ((Widget) m_scrolledWindow, (Widget) m_hScrollBar, (Widget) m_vScrollBar, (Widget) m_drawingArea);
        XtRealizeWidget((Widget) m_scrolledWindow);
        XtManageChild((Widget) m_scrolledWindow);
    }
    
    if (newScrollBar)
        XtVaSetValues((Widget) newScrollBar,
        XmNvalue, pos,
        XmNminimum, 0,
        XmNmaximum, range,
        XmNsliderSize, thumbVisible,
        NULL);
    
    if (orient == wxHORIZONTAL)
        m_scrollPosX = pos;
    else
        m_scrollPosY = pos;
    
    int newW, newH;
    GetSize(& newW, & newH);
    
    // Adjusting scrollbars can resize the canvas accidentally
    if (newW != oldW || newH != oldH)
        SetSize(-1, -1, oldW, oldH);
}

// Does a physical scroll
void wxWindow::ScrollWindow(int dx, int dy, const wxRect *rect)
{
    //  cerr << "Scrolling. delta = " << dx << ", " << dy << endl;
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
    
    int x1 = (dx >= 0) ? x : x - dx ;
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
    
    XCopyArea(display, window,
        window, (GC) dc.GetGC(),
        x1, y1,
        w1, h1,
        x2, y2);
    
    dc.SetAutoSetting(TRUE);
    wxBrush brush(GetBackgroundColour(), wxSOLID);
    dc.SetBrush(brush); // ??
    
    // We'll add rectangles to the list of update rectangles
    // according to which bits we've exposed.
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
    
    wxNode* node = updateRects.First();
    while (node)
    {
        wxRect* rect = (wxRect*) node->Data();
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
        
        node = node->Next();
        
    }
    
    // Delete the update rects
    node = updateRects.First();
    while (node)
    {
        wxRect* rect = (wxRect*) node->Data();
        delete rect;
        node = node->Next();
    }

    XmUpdateDisplay((Widget) GetMainWidget());
}

void wxWindow::OnChar(wxKeyEvent& event)
{
    event.Skip();
/* ??
if ( event.KeyCode() == WXK_TAB ) {
// propagate the TABs to the parent - it's up to it to decide what
// to do with it
if ( GetParent() ) {
if ( GetParent()->ProcessEvent(event) )
return;
}
}
    */
}

void wxWindow::OnKeyDown(wxKeyEvent& event)
{
    event.Skip();
}

void wxWindow::OnKeyUp(wxKeyEvent& event)
{
    event.Skip();
}

void wxWindow::OnPaint(wxPaintEvent& event)
{
    Default();
}

bool wxWindow::IsEnabled() const
{
    // TODO. Is this right?
    // return XtGetSensitive((Widget) GetMainWidget());
    return FALSE;
}

// Dialog support: override these and call
// base class members to add functionality
// that can't be done using validators.
// NOTE: these functions assume that controls
// are direct children of this window, not grandchildren
// or other levels of descendant.

// Transfer values to controls. If returns FALSE,
// it's an application error (pops up a dialog)
bool wxWindow::TransferDataToWindow()
{
    wxNode *node = GetChildren().First();
    while ( node )
    {
        wxWindow *child = (wxWindow *)node->Data();
        if ( child->GetValidator() &&
            !child->GetValidator()->TransferToWindow() )
        {
            wxLogError("Could not transfer data to window.");
            return FALSE;
        }
        
        node = node->Next();
    }
    return TRUE;
}

// Transfer values from controls. If returns FALSE,
// validation failed: don't quit
bool wxWindow::TransferDataFromWindow()
{
    wxNode *node = GetChildren().First();
    while ( node )
    {
        wxWindow *child = (wxWindow *)node->Data();
        if ( child->GetValidator() && !child->GetValidator()->TransferFromWindow() )
        {
            return FALSE;
        }
        
        node = node->Next();
    }
    return TRUE;
}

bool wxWindow::Validate()
{
    wxNode *node = GetChildren().First();
    while ( node )
    {
        wxWindow *child = (wxWindow *)node->Data();
        if ( child->GetValidator() && /* child->GetValidator()->Ok() && */ !child->GetValidator()->Validate(this) )
        {
            return FALSE;
        }
        
        node = node->Next();
    }
    return TRUE;
}

// Get the window with the focus
wxWindow *wxWindow::FindFocus()
{
    // TODO
    return NULL;
}

void wxWindow::AddChild(wxWindow *child)
{
    GetChildren().Append(child);
    child->m_windowParent = this;
}

void wxWindow::RemoveChild(wxWindow *child)
{
    GetChildren().DeleteObject(child);
    child->m_windowParent = NULL;
}

// Reparents this window to have the new parent.
bool wxWindow::Reparent(wxWindow* WXUNUSED(parent))
{
    // For now, we indicate that this isn't implemented.
    return FALSE;
}

void wxWindow::DestroyChildren()
{
    wxNode *node = GetChildren().First();
    while (node)
    {
        wxNode* next = node->Next();
        wxWindow* child = (wxWindow*) node->Data();
        delete child;
        node = next;
    }
    GetChildren().Clear();
#if 0
    wxNode *node;
    while ((node = GetChildren().First()) != (wxNode *)NULL) {
        wxWindow *child;
        if ((child = (wxWindow *)node->Data()) != (wxWindow *)NULL) {
            delete child;
            if ( GetChildren().Member(child) )
                delete node;
        }
    } /* while */
#endif
}

void wxWindow::MakeModal(bool modal)
{
    // Disable all other windows
    if (this->IsKindOf(CLASSINFO(wxDialog)) || this->IsKindOf(CLASSINFO(wxFrame)))
    {
        wxNode *node = wxTopLevelWindows.First();
        while (node)
        {
            wxWindow *win = (wxWindow *)node->Data();
            if (win != this)
                win->Enable(!modal);
            
            node = node->Next();
        }
    }
}

// If nothing defined for this, try the parent.
// E.g. we may be a button loaded from a resource, with no callback function
// defined.
void wxWindow::OnCommand(wxWindow& win, wxCommandEvent& event)
{
    if (GetEventHandler()->ProcessEvent(event) )
        return;
    if (m_windowParent)
        m_windowParent->GetEventHandler()->OnCommand(win, event);
}

void wxWindow::SetConstraints(wxLayoutConstraints *c)
{
    if (m_constraints)
    {
        UnsetConstraints(m_constraints);
        delete m_constraints;
    }
    m_constraints = c;
    if (m_constraints)
    {
        // Make sure other windows know they're part of a 'meaningful relationship'
        if (m_constraints->left.GetOtherWindow() && (m_constraints->left.GetOtherWindow() != this))
            m_constraints->left.GetOtherWindow()->AddConstraintReference((wxWindow *)this);
        if (m_constraints->top.GetOtherWindow() && (m_constraints->top.GetOtherWindow() != this))
            m_constraints->top.GetOtherWindow()->AddConstraintReference((wxWindow *)this);
        if (m_constraints->right.GetOtherWindow() && (m_constraints->right.GetOtherWindow() != this))
            m_constraints->right.GetOtherWindow()->AddConstraintReference((wxWindow *)this);
        if (m_constraints->bottom.GetOtherWindow() && (m_constraints->bottom.GetOtherWindow() != this))
            m_constraints->bottom.GetOtherWindow()->AddConstraintReference((wxWindow *)this);
        if (m_constraints->width.GetOtherWindow() && (m_constraints->width.GetOtherWindow() != this))
            m_constraints->width.GetOtherWindow()->AddConstraintReference((wxWindow *)this);
        if (m_constraints->height.GetOtherWindow() && (m_constraints->height.GetOtherWindow() != this))
            m_constraints->height.GetOtherWindow()->AddConstraintReference((wxWindow *)this);
        if (m_constraints->centreX.GetOtherWindow() && (m_constraints->centreX.GetOtherWindow() != this))
            m_constraints->centreX.GetOtherWindow()->AddConstraintReference((wxWindow *)this);
        if (m_constraints->centreY.GetOtherWindow() && (m_constraints->centreY.GetOtherWindow() != this))
            m_constraints->centreY.GetOtherWindow()->AddConstraintReference((wxWindow *)this);
    }
}

// This removes any dangling pointers to this window
// in other windows' constraintsInvolvedIn lists.
void wxWindow::UnsetConstraints(wxLayoutConstraints *c)
{
    if (c)
    {
        if (c->left.GetOtherWindow() && (c->top.GetOtherWindow() != this))
            c->left.GetOtherWindow()->RemoveConstraintReference((wxWindow *)this);
        if (c->top.GetOtherWindow() && (c->top.GetOtherWindow() != this))
            c->top.GetOtherWindow()->RemoveConstraintReference((wxWindow *)this);
        if (c->right.GetOtherWindow() && (c->right.GetOtherWindow() != this))
            c->right.GetOtherWindow()->RemoveConstraintReference((wxWindow *)this);
        if (c->bottom.GetOtherWindow() && (c->bottom.GetOtherWindow() != this))
            c->bottom.GetOtherWindow()->RemoveConstraintReference((wxWindow *)this);
        if (c->width.GetOtherWindow() && (c->width.GetOtherWindow() != this))
            c->width.GetOtherWindow()->RemoveConstraintReference((wxWindow *)this);
        if (c->height.GetOtherWindow() && (c->height.GetOtherWindow() != this))
            c->height.GetOtherWindow()->RemoveConstraintReference((wxWindow *)this);
        if (c->centreX.GetOtherWindow() && (c->centreX.GetOtherWindow() != this))
            c->centreX.GetOtherWindow()->RemoveConstraintReference((wxWindow *)this);
        if (c->centreY.GetOtherWindow() && (c->centreY.GetOtherWindow() != this))
            c->centreY.GetOtherWindow()->RemoveConstraintReference((wxWindow *)this);
    }
}

// Back-pointer to other windows we're involved with, so if we delete
// this window, we must delete any constraints we're involved with.
void wxWindow::AddConstraintReference(wxWindow *otherWin)
{
    if (!m_constraintsInvolvedIn)
        m_constraintsInvolvedIn = new wxList;
    if (!m_constraintsInvolvedIn->Member(otherWin))
        m_constraintsInvolvedIn->Append(otherWin);
}

// REMOVE back-pointer to other windows we're involved with.
void wxWindow::RemoveConstraintReference(wxWindow *otherWin)
{
    if (m_constraintsInvolvedIn)
        m_constraintsInvolvedIn->DeleteObject(otherWin);
}

// Reset any constraints that mention this window
void wxWindow::DeleteRelatedConstraints()
{
    if (m_constraintsInvolvedIn)
    {
        wxNode *node = m_constraintsInvolvedIn->First();
        while (node)
        {
            wxWindow *win = (wxWindow *)node->Data();
            wxNode *next = node->Next();
            wxLayoutConstraints *constr = win->GetConstraints();
            
            // Reset any constraints involving this window
            if (constr)
            {
                constr->left.ResetIfWin((wxWindow *)this);
                constr->top.ResetIfWin((wxWindow *)this);
                constr->right.ResetIfWin((wxWindow *)this);
                constr->bottom.ResetIfWin((wxWindow *)this);
                constr->width.ResetIfWin((wxWindow *)this);
                constr->height.ResetIfWin((wxWindow *)this);
                constr->centreX.ResetIfWin((wxWindow *)this);
                constr->centreY.ResetIfWin((wxWindow *)this);
            }
            delete node;
            node = next;
        }
        delete m_constraintsInvolvedIn;
        m_constraintsInvolvedIn = NULL;
    }
}

void wxWindow::SetSizer(wxSizer *sizer)
{
    m_windowSizer = sizer;
    if (sizer)
        sizer->SetSizerParent((wxWindow *)this);
}

/*
* New version
*/

bool wxWindow::Layout()
{
    if (GetConstraints())
    {
        int w, h;
        GetClientSize(&w, &h);
        GetConstraints()->width.SetValue(w);
        GetConstraints()->height.SetValue(h);
    }
    
    // If top level (one sizer), evaluate the sizer's constraints.
    if (GetSizer())
    {
        int noChanges;
        GetSizer()->ResetConstraints();   // Mark all constraints as unevaluated
        GetSizer()->LayoutPhase1(&noChanges);
        GetSizer()->LayoutPhase2(&noChanges);
        GetSizer()->SetConstraintSizes(); // Recursively set the real window sizes
        return TRUE;
    }
    else
    {
        // Otherwise, evaluate child constraints
        ResetConstraints();   // Mark all constraints as unevaluated
        DoPhase(1);           // Just one phase need if no sizers involved
        DoPhase(2);
        SetConstraintSizes(); // Recursively set the real window sizes
    }
    return TRUE;
}


// Do a phase of evaluating constraints:
// the default behaviour. wxSizers may do a similar
// thing, but also impose their own 'constraints'
// and order the evaluation differently.
bool wxWindow::LayoutPhase1(int *noChanges)
{
    wxLayoutConstraints *constr = GetConstraints();
    if (constr)
    {
        return constr->SatisfyConstraints((wxWindow *)this, noChanges);
    }
    else
        return TRUE;
}

bool wxWindow::LayoutPhase2(int *noChanges)
{
    *noChanges = 0;
    
    // Layout children
    DoPhase(1);
    DoPhase(2);
    return TRUE;
}

// Do a phase of evaluating child constraints
bool wxWindow::DoPhase(int phase)
{
    int noIterations = 0;
    int maxIterations = 500;
    int noChanges = 1;
    int noFailures = 0;
    wxList succeeded;
    while ((noChanges > 0) && (noIterations < maxIterations))
    {
        noChanges = 0;
        noFailures = 0;
        wxNode *node = GetChildren().First();
        while (node)
        {
            wxWindow *child = (wxWindow *)node->Data();
            if (!child->IsKindOf(CLASSINFO(wxFrame)) && !child->IsKindOf(CLASSINFO(wxDialog)))
            {
                wxLayoutConstraints *constr = child->GetConstraints();
                if (constr)
                {
                    if (succeeded.Member(child))
                    {
                    }
                    else
                    {
                        int tempNoChanges = 0;
                        bool success = ( (phase == 1) ? child->LayoutPhase1(&tempNoChanges) : child->LayoutPhase2(&tempNoChanges) ) ;
                        noChanges += tempNoChanges;
                        if (success)
                        {
                            succeeded.Append(child);
                        }
                    }
                }
            }
            node = node->Next();
        }
        noIterations ++;
    }
    return TRUE;
}

void wxWindow::ResetConstraints()
{
    wxLayoutConstraints *constr = GetConstraints();
    if (constr)
    {
        constr->left.SetDone(FALSE);
        constr->top.SetDone(FALSE);
        constr->right.SetDone(FALSE);
        constr->bottom.SetDone(FALSE);
        constr->width.SetDone(FALSE);
        constr->height.SetDone(FALSE);
        constr->centreX.SetDone(FALSE);
        constr->centreY.SetDone(FALSE);
    }
    wxNode *node = GetChildren().First();
    while (node)
    {
        wxWindow *win = (wxWindow *)node->Data();
        if (!win->IsKindOf(CLASSINFO(wxFrame)) && !win->IsKindOf(CLASSINFO(wxDialog)))
            win->ResetConstraints();
        node = node->Next();
    }
}

// Need to distinguish between setting the 'fake' size for
// windows and sizers, and setting the real values.
void wxWindow::SetConstraintSizes(bool recurse)
{
    wxLayoutConstraints *constr = GetConstraints();
    if (constr && constr->left.GetDone() && constr->right.GetDone() &&
        constr->width.GetDone() && constr->height.GetDone())
    {
        int x = constr->left.GetValue();
        int y = constr->top.GetValue();
        int w = constr->width.GetValue();
        int h = constr->height.GetValue();
        
        // If we don't want to resize this window, just move it...
        if ((constr->width.GetRelationship() != wxAsIs) ||
            (constr->height.GetRelationship() != wxAsIs))
        {
            // Calls Layout() recursively. AAAGH. How can we stop that.
            // Simply take Layout() out of non-top level OnSizes.
            SizerSetSize(x, y, w, h);
        }
        else
        {
            SizerMove(x, y);
        }
    }
    else if (constr)
    {
        char *windowClass = this->GetClassInfo()->GetClassName();
        
        wxString winName;
        if (GetName() == "")
            winName = "unnamed";
        else
            winName = GetName();
        wxDebugMsg("Constraint(s) not satisfied for window of type %s, name %s:\n", (const char *)windowClass, (const char *)winName);
        if (!constr->left.GetDone())
            wxDebugMsg("  unsatisfied 'left' constraint.\n");
        if (!constr->right.GetDone())
            wxDebugMsg("  unsatisfied 'right' constraint.\n");
        if (!constr->width.GetDone())
            wxDebugMsg("  unsatisfied 'width' constraint.\n");
        if (!constr->height.GetDone())
            wxDebugMsg("  unsatisfied 'height' constraint.\n");
        wxDebugMsg("Please check constraints: try adding AsIs() constraints.\n");
    }
    
    if (recurse)
    {
        wxNode *node = GetChildren().First();
        while (node)
        {
            wxWindow *win = (wxWindow *)node->Data();
            if (!win->IsKindOf(CLASSINFO(wxFrame)) && !win->IsKindOf(CLASSINFO(wxDialog)))
                win->SetConstraintSizes();
            node = node->Next();
        }
    }
}

// This assumes that all sizers are 'on' the same
// window, i.e. the parent of this window.
void wxWindow::TransformSizerToActual(int *x, int *y) const
{
    if (!m_sizerParent || m_sizerParent->IsKindOf(CLASSINFO(wxDialog)) ||
        m_sizerParent->IsKindOf(CLASSINFO(wxFrame)) )
        return;
    
    int xp, yp;
    m_sizerParent->GetPosition(&xp, &yp);
    m_sizerParent->TransformSizerToActual(&xp, &yp);
    *x += xp;
    *y += yp;
}

void wxWindow::SizerSetSize(int x, int y, int w, int h)
{
    int xx = x;
    int yy = y;
    TransformSizerToActual(&xx, &yy);
    SetSize(xx, yy, w, h);
}

void wxWindow::SizerMove(int x, int y)
{
    int xx = x;
    int yy = y;
    TransformSizerToActual(&xx, &yy);
    Move(xx, yy);
}

// Only set the size/position of the constraint (if any)
void wxWindow::SetSizeConstraint(int x, int y, int w, int h)
{
    wxLayoutConstraints *constr = GetConstraints();
    if (constr)
    {
        if (x != -1)
        {
            constr->left.SetValue(x);
            constr->left.SetDone(TRUE);
        }
        if (y != -1)
        {
            constr->top.SetValue(y);
            constr->top.SetDone(TRUE);
        }
        if (w != -1)
        {
            constr->width.SetValue(w);
            constr->width.SetDone(TRUE);
        }
        if (h != -1)
        {
            constr->height.SetValue(h);
            constr->height.SetDone(TRUE);
        }
    }
}

void wxWindow::MoveConstraint(int x, int y)
{
    wxLayoutConstraints *constr = GetConstraints();
    if (constr)
    {
        if (x != -1)
        {
            constr->left.SetValue(x);
            constr->left.SetDone(TRUE);
        }
        if (y != -1)
        {
            constr->top.SetValue(y);
            constr->top.SetDone(TRUE);
        }
    }
}

void wxWindow::GetSizeConstraint(int *w, int *h) const
{
    wxLayoutConstraints *constr = GetConstraints();
    if (constr)
    {
        *w = constr->width.GetValue();
        *h = constr->height.GetValue();
    }
    else
        GetSize(w, h);
}

void wxWindow::GetClientSizeConstraint(int *w, int *h) const
{
    wxLayoutConstraints *constr = GetConstraints();
    if (constr)
    {
        *w = constr->width.GetValue();
        *h = constr->height.GetValue();
    }
    else
        GetClientSize(w, h);
}

void wxWindow::GetPositionConstraint(int *x, int *y) const
{
    wxLayoutConstraints *constr = GetConstraints();
    if (constr)
    {
        *x = constr->left.GetValue();
        *y = constr->top.GetValue();
    }
    else
        GetPosition(x, y);
}

bool wxWindow::Close(bool force)
{
    wxCloseEvent event(wxEVT_CLOSE_WINDOW, m_windowId);
    event.SetEventObject(this);
#if WXWIN_COMPATIBILITY
    event.SetForce(force);
#endif
    event.SetCanVeto(!force);
    
    return GetEventHandler()->ProcessEvent(event);
}

wxObject* wxWindow::GetChild(int number) const
{
    // Return a pointer to the Nth object in the window
    wxNode *node = GetChildren().First();
    int n = number;
    while (node && n--)
        node = node->Next() ;
    if (node)
    {
        wxObject *obj = (wxObject *)node->Data();
        return(obj) ;
    }
    else
        return NULL ;
}

void wxWindow::OnDefaultAction(wxControl *initiatingItem)
{
    // Obsolete function
}

void wxWindow::Clear()
{
    wxClientDC dc(this);
    wxBrush brush(GetBackgroundColour(), wxSOLID);
    dc.SetBackground(brush);
    dc.Clear();
}

// Fits the panel around the items
void wxWindow::Fit()
{
    int maxX = 0;
    int maxY = 0;
    wxNode *node = GetChildren().First();
    while ( node )
    {
        wxWindow *win = (wxWindow *)node->Data();
        int wx, wy, ww, wh;
        win->GetPosition(&wx, &wy);
        win->GetSize(&ww, &wh);
        if ( wx + ww > maxX )
            maxX = wx + ww;
        if ( wy + wh > maxY )
            maxY = wy + wh;
        
        node = node->Next();
    }
    SetClientSize(maxX + 5, maxY + 5);
}

void wxWindow::SetValidator(const wxValidator& validator)
{
    if ( m_windowValidator )
        delete m_windowValidator;
    m_windowValidator = validator.Clone();
    
    if ( m_windowValidator )
        m_windowValidator->SetWindow(this) ;
}

void wxWindow::SetClientObject( wxClientData *data )
{
    if (m_clientObject) delete m_clientObject;
    m_clientObject = data;
}

wxClientData *wxWindow::GetClientObject()
{
    return m_clientObject;
}

void wxWindow::SetClientData( void *data )
{
    m_clientData = data;
}

void *wxWindow::GetClientData()
{
    return m_clientData;
}

// Find a window by id or name
wxWindow *wxWindow::FindWindow(long id)
{
    if ( GetId() == id)
        return this;
    
    wxNode *node = GetChildren().First();
    while ( node )
    {
        wxWindow *child = (wxWindow *)node->Data();
        wxWindow *found = child->FindWindow(id);
        if ( found )
            return found;
        node = node->Next();
    }
    return NULL;
}

wxWindow *wxWindow::FindWindow(const wxString& name)
{
    if ( GetName() == name)
        return this;
    
    wxNode *node = GetChildren().First();
    while ( node )
    {
        wxWindow *child = (wxWindow *)node->Data();
        wxWindow *found = child->FindWindow(name);
        if ( found )
            return found;
        node = node->Next();
    }
    return NULL;
}

void wxWindow::OnIdle(wxIdleEvent& event)
{
    // This calls the UI-update mechanism (querying windows for
    // menu/toolbar/control state information)
    UpdateWindowUI();
}

// Raise the window to the top of the Z order
void wxWindow::Raise()
{
    Window window = XtWindow((Widget) GetTopWidget());
    XRaiseWindow(XtDisplay((Widget) GetTopWidget()), window);
}

// Lower the window to the bottom of the Z order
void wxWindow::Lower()
{
    Window window = XtWindow((Widget) GetTopWidget());
    XLowerWindow(XtDisplay((Widget) GetTopWidget()), window);
}

bool wxWindow::AcceptsFocus() const
{
    return IsShown() && IsEnabled();
}

// Update region access
wxRegion& wxWindow::GetUpdateRegion() const
{
    return (wxRegion&) m_updateRegion;
}

bool wxWindow::IsExposed(int x, int y, int w, int h) const
{
    return (m_updateRegion.Contains(x, y, w, h) != wxOutRegion);
}

bool wxWindow::IsExposed(const wxPoint& pt) const
{
    return (m_updateRegion.Contains(pt) != wxOutRegion);
}

bool wxWindow::IsExposed(const wxRect& rect) const
{
    return (m_updateRegion.Contains(rect) != wxOutRegion);
}

/*
* Allocates control IDs
*/

int wxWindow::NewControlId()
{
    static int s_controlId = 0;
    s_controlId ++;
    return s_controlId;
}

void wxWindow::SetAcceleratorTable(const wxAcceleratorTable& accel)
{
    m_acceleratorTable = accel;
}

// All widgets should have this as their resize proc.
// OnSize sent to wxWindow via client data.
void wxWidgetResizeProc(Widget w, XConfigureEvent *event, String args[], int *num_args)
{
    wxWindow *win = (wxWindow *)wxWidgetHashTable->Get((long)w);
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

bool wxAddWindowToTable(Widget w, wxWindow *win)
{
    wxWindow *oldItem = NULL;
    //  printf("Adding widget %ld, name = %s\n", w, win->GetClassInfo()->GetClassName());
    if ((oldItem = (wxWindow *)wxWidgetHashTable->Get ((long) w)))
    {
        wxLogError("Widget table clash: new widget is %ld, %s", (long)w, win->GetClassInfo()->GetClassName());
        return FALSE;
    }
    
    wxWidgetHashTable->Put ((long) w, win);
    return TRUE;
}

wxWindow *wxGetWindowFromTable(Widget w)
{
    return (wxWindow *)wxWidgetHashTable->Get ((long) w);
}

void wxDeleteWindowFromTable(Widget w)
{
    wxWidgetHashTable->Delete((long)w);
}

// Get the underlying X window and display
WXWindow wxWindow::GetXWindow() const
{
    return (WXWindow) XtWindow((Widget) GetMainWidget());
}

WXDisplay *wxWindow::GetXDisplay() const
{
    return (WXDisplay*) XtDisplay((Widget) GetMainWidget());
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

void wxCanvasRepaintProc (Widget drawingArea, XtPointer clientData,
                          XmDrawingAreaCallbackStruct * cbs)
{
    if (!wxWidgetHashTable->Get ((long) (Widget) drawingArea))
        return;
    
    XEvent * event = cbs->event;
    wxWindow * win = (wxWindow *) clientData;
    Display * display = (Display *) win->GetXDisplay();
    
    switch (event->type)
    {
    case Expose:
        {
            wxRect* rect = new wxRect(event->xexpose.x, event->xexpose.y,
                event->xexpose.width, event->xexpose.height);
                /*
                cout << "Expose proc. wxRect: " << rect->x << ", " << rect->y << ", ";
                cout << rect->width << ", " << rect->height << "\n\n";
            */
            
            win->m_updateRects.Append((wxObject*) rect);
            
            if (event -> xexpose.count == 0)
            {
	      /*
                wxPaintEvent event(win->GetId());
                event.SetEventObject(win);
                win->GetEventHandler()->ProcessEvent(event);
		*/
                
                win->DoPaint();
                win->ClearUpdateRects();
            }
            break;
        }
    default:
        {
            cout << "\n\nNew Event ! is = " << event -> type << "\n";
            break;
        }
    }
}

// Unable to deal with Enter/Leave without a separate EventHandler (Motif 1.1.4)
void 
wxCanvasEnterLeave (Widget drawingArea, XtPointer clientData, XCrossingEvent * event)
{
    XmDrawingAreaCallbackStruct cbs;
    XEvent ev;
    
    //if (event->mode!=NotifyNormal)
    //  return ;
    
    //  ev = *((XEvent *) event); // Causes Purify error (copying too many bytes)
    ((XCrossingEvent &) ev) = *event;
    
    cbs.reason = XmCR_INPUT;
    cbs.event = &ev;
    
    wxCanvasInputEvent (drawingArea, (XtPointer) NULL, &cbs);
}

// Fix to make it work under Motif 1.0 (!)
void wxCanvasMotionEvent (Widget drawingArea, XButtonEvent * event)
{
#if   XmVersion<=1000
    
    XmDrawingAreaCallbackStruct cbs;
    XEvent ev;
    
    //ev.xbutton = *event;
    ev = *((XEvent *) event);
    cbs.reason = XmCR_INPUT;
    cbs.event = &ev;
    
    wxCanvasInputEvent (drawingArea, (XtPointer) NULL, &cbs);
#endif
}

void wxCanvasInputEvent (Widget drawingArea, XtPointer data, XmDrawingAreaCallbackStruct * cbs)
{
    wxWindow *canvas = (wxWindow *) wxWidgetHashTable->Get ((long) (Widget) drawingArea);
    XEvent local_event;
    
    if (canvas==NULL)
        return ;
    
    if (cbs->reason != XmCR_INPUT)
        return;
    
    local_event = *(cbs->event);	// We must keep a copy!
    
  /*
  switch (local_event.xany.type)
  {
  case EnterNotify:
  cout << "EnterNotify\n";
  break;
  case LeaveNotify:
  cout << "LeaveNotify\n";
  break;
  case ButtonPress:
  cout << "ButtonPress\n";
  break;
  case ButtonRelease:
  cout << "ButtonRelease\n";
  break;
  case MotionNotify:
  cout << "MotionNotify\n";
  break;
  default:
  cout << "Something else\n";
  break;
  }
    */
    
    switch (local_event.xany.type)
    {
    case EnterNotify:
    case LeaveNotify:
    case ButtonPress:
    case ButtonRelease:
    case MotionNotify:
        {
            wxEventType eventType = wxEVT_NULL;
            
            if (local_event.xany.type == EnterNotify)
            {
                //if (local_event.xcrossing.mode!=NotifyNormal)
                //  return ; // Ignore grab events
                eventType = wxEVT_ENTER_WINDOW;
                //            canvas->GetEventHandler()->OnSetFocus();
            }
            else if (local_event.xany.type == LeaveNotify)
            {
                //if (local_event.xcrossing.mode!=NotifyNormal)
                //  return ; // Ignore grab events
                eventType = wxEVT_LEAVE_WINDOW;
                //            canvas->GetEventHandler()->OnKillFocus();
            }
            else if (local_event.xany.type == MotionNotify)
            {
                eventType = wxEVT_MOTION;
                if (local_event.xmotion.is_hint == NotifyHint)
                {
                    Window root, child;
                    Display *dpy = XtDisplay (drawingArea);
                    
                    XQueryPointer (dpy, XtWindow (drawingArea),
                        &root, &child,
                        &local_event.xmotion.x_root,
                        &local_event.xmotion.y_root,
                        &local_event.xmotion.x,
                        &local_event.xmotion.y,
                        &local_event.xmotion.state);
                }
                else
                {
                }
            }
            
            else if (local_event.xany.type == ButtonPress)
            {
                if (local_event.xbutton.button == Button1)
                {
                    eventType = wxEVT_LEFT_DOWN;
                    canvas->m_button1Pressed = TRUE;
                }
                else if (local_event.xbutton.button == Button2)
                {
                    eventType = wxEVT_MIDDLE_DOWN;
                    canvas->m_button2Pressed = TRUE;
                }
                else if (local_event.xbutton.button == Button3)
                {
                    eventType = wxEVT_RIGHT_DOWN;
                    canvas->m_button3Pressed = TRUE;
                }
            }
            else if (local_event.xany.type == ButtonRelease)
            {
                if (local_event.xbutton.button == Button1)
                {
                    eventType = wxEVT_LEFT_UP;
                    canvas->m_button1Pressed = FALSE;
                }
                else if (local_event.xbutton.button == Button2)
                {
                    eventType = wxEVT_MIDDLE_UP;
                    canvas->m_button2Pressed = FALSE;
                }
                else if (local_event.xbutton.button == Button3)
                {
                    eventType = wxEVT_RIGHT_UP;
                    canvas->m_button3Pressed = FALSE;
                }
            }
            
            wxMouseEvent wxevent (eventType);
            wxevent.m_eventHandle = (char *) &local_event;
            
            wxevent.m_leftDown = ((eventType == wxEVT_LEFT_DOWN)
                || (event_left_is_down (&local_event) 
                && (eventType != wxEVT_LEFT_UP)));
            wxevent.m_middleDown = ((eventType == wxEVT_MIDDLE_DOWN)
                || (event_middle_is_down (&local_event) 
                && (eventType != wxEVT_MIDDLE_UP)));
            wxevent.m_rightDown = ((eventType == wxEVT_RIGHT_DOWN)
                || (event_right_is_down (&local_event) 
                && (eventType != wxEVT_RIGHT_UP)));
            
            wxevent.m_shiftDown = local_event.xbutton.state & ShiftMask;
            wxevent.m_controlDown = local_event.xbutton.state & ControlMask;
            wxevent.m_altDown = local_event.xbutton.state & Mod3Mask;
            wxevent.m_metaDown = local_event.xbutton.state & Mod1Mask;
            wxevent.SetTimestamp(local_event.xbutton.time);
            
            // Now check if we need to translate this event into a double click
            if (TRUE) // canvas->doubleClickAllowed)
            {
                if (wxevent.ButtonDown())
                {
                    long dclickTime = XtGetMultiClickTime((Display*) wxGetDisplay()) ;
                    
                    // get button and time-stamp
                    int button = 0;
                    if      (wxevent.LeftDown())   button = 1;
                    else if (wxevent.MiddleDown()) button = 2;
                    else if (wxevent.RightDown())  button = 3;
                    long ts = wxevent.GetTimestamp();
                    // check, if single or double click
                    if (canvas->m_lastButton && canvas->m_lastButton==button && (ts - canvas->m_lastTS) < dclickTime)
                    {
                        // I have a dclick
                        canvas->m_lastButton = 0;
                        switch ( eventType )
                        {
                        case wxEVT_LEFT_DOWN:
                            wxevent.SetEventType(wxEVT_LEFT_DCLICK);
                            break;
                        case wxEVT_MIDDLE_DOWN:
                            wxevent.SetEventType(wxEVT_MIDDLE_DCLICK);
                            break;
                        case wxEVT_RIGHT_DOWN:
                            wxevent.SetEventType(wxEVT_RIGHT_DCLICK);
                            break;
                            
                        default :
                            break;
                        }
                        
                    }
                    else
                    {
                        // not fast enough or different button
                        canvas->m_lastTS     = ts;
                        canvas->m_lastButton = button;
                    }
                }
            }
            
            wxevent.SetId(canvas->GetId());
            wxevent.SetEventObject(canvas);
            wxevent.m_x = local_event.xbutton.x;
            wxevent.m_y = local_event.xbutton.y;
            canvas->GetEventHandler()->ProcessEvent (wxevent);
            /*
            if (eventType == wxEVT_ENTER_WINDOW ||
            eventType == wxEVT_LEAVE_WINDOW ||
            eventType == wxEVT_MOTION
            )
            return;
            */
            break;
      }
    case KeyPress:
        {
            KeySym keySym;
            //	XComposeStatus compose;
            //	(void) XLookupString ((XKeyEvent *) & local_event, wxBuffer, 20, &keySym, &compose);
            (void) XLookupString ((XKeyEvent *) & local_event, wxBuffer, 20, &keySym, NULL);
            int id = wxCharCodeXToWX (keySym);

            wxEventType eventType = wxEVT_CHAR;

            wxKeyEvent event (eventType);
            
            if (local_event.xkey.state & ShiftMask)
                event.m_shiftDown = TRUE;
            if (local_event.xkey.state & ControlMask)
                event.m_controlDown = TRUE;
            if (local_event.xkey.state & Mod3Mask)
                event.m_altDown = TRUE;
            if (local_event.xkey.state & Mod1Mask)
                event.m_metaDown = TRUE;
            event.SetEventObject(canvas);
            event.m_keyCode = id;
            event.SetTimestamp(local_event.xkey.time);
            
            if (id > -1)
            {
                // Implement wxFrame::OnCharHook by checking ancestor.
                wxWindow *parent = canvas->GetParent();
                while (parent && !parent->IsKindOf(CLASSINFO(wxFrame)))
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
            KeySym keySym;
            (void) XLookupString ((XKeyEvent *) & local_event, wxBuffer, 20, &keySym, NULL);
            int id = wxCharCodeXToWX (keySym);
            
            wxKeyEvent event (wxEVT_KEY_UP);
            
            if (local_event.xkey.state & ShiftMask)
                event.m_shiftDown = TRUE;
            if (local_event.xkey.state & ControlMask)
                event.m_controlDown = TRUE;
            if (local_event.xkey.state & Mod3Mask)
                event.m_altDown = TRUE;
            if (local_event.xkey.state & Mod1Mask)
                event.m_metaDown = TRUE;
            event.SetEventObject(canvas);
            event.m_keyCode = id;
            event.SetTimestamp(local_event.xkey.time);
            
            if (id > -1)
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

      // We have to test whether it's a wxScrolledWindow (hack!)
      // because otherwise we don't know how many pixels have been
      // scrolled. We might solve this in the future by defining
      // virtual wxWindow functions to get the scroll position in pixels.
      // Or, each kind of scrolled window has to implement backing
      // stores itself, using generic wxWindows code.
      if (this->IsKindOf(CLASSINFO(wxScrolledWindow)))
      {
          wxScrolledWindow* scrolledWindow = (wxScrolledWindow*) this;
          int x, y;
          scrolledWindow->CalcScrolledPosition(0, 0, & x, & y);

          scrollPosX = - x;
          scrollPosY = - y;
      }

      // TODO: This could be optimized further by only copying the
      // areas in the current update region.

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
    
      XCopyArea (XtDisplay (widget), (Pixmap) m_backingPixmap, XtWindow (widget), tempGC,
        scrollPosX, scrollPosY, // Start at the scroll position
        toBlitX, toBlitY,       // How much of the pixmap to copy
        0, 0);                  // Destination
    }
    else
    {
        // Set an erase event first
        wxEraseEvent eraseEvent(GetId());
        eraseEvent.SetEventObject(this);
        GetEventHandler()->ProcessEvent(eraseEvent);

        wxPaintEvent event(GetId());
        event.SetEventObject(this);
        GetEventHandler()->ProcessEvent(event);

        m_needsRefresh = FALSE;
    }
}

// SetSize, but as per old wxCanvas (with drawing widget etc.)
void wxWindow::CanvasSetSize (int x, int y, int w, int h, int sizeFlags)
{
    // A bit of optimization to help sort out the flickers.
    int oldX, oldY, oldW, oldH;
    GetSize(& oldW, & oldH);
    GetPosition(& oldX, & oldY);
    
    bool useOldPos = FALSE;
    bool useOldSize = FALSE;
    
    if ((x == -1) && (x == -1) && ((sizeFlags & wxSIZE_ALLOW_MINUS_ONE) == 0))
        useOldPos = TRUE;
    else if (x == oldX && y == oldY)
        useOldPos = TRUE;
    
    if ((w == -1) && (h == -1))
        useOldSize = TRUE;
    else if (w == oldW && h == oldH)
        useOldSize = TRUE;
    
    if (!wxNoOptimize::CanOptimize())
    {
        useOldSize = FALSE; useOldPos = FALSE;
    }
    
    if (useOldPos && useOldSize)
        return;
    
    Widget drawingArea = (Widget) m_drawingArea;
    bool managed = XtIsManaged(m_borderWidget ? (Widget) m_borderWidget : (Widget) m_scrolledWindow);
    
    if (managed)
        XtUnmanageChild (m_borderWidget ? (Widget) m_borderWidget : (Widget) m_scrolledWindow);
    XtVaSetValues((Widget) m_drawingArea, XmNresizePolicy, XmRESIZE_ANY, NULL);
    
    int xx = x; int yy = y;
    AdjustForParentClientOrigin(xx, yy, sizeFlags);
    
    if (!useOldPos)
    {
        if (x > -1 || (sizeFlags & wxSIZE_ALLOW_MINUS_ONE))
        {
            XtVaSetValues (m_borderWidget ? (Widget) m_borderWidget : (Widget) m_scrolledWindow,
                XmNx, xx, NULL);
        }
        
        if (y > -1 || (sizeFlags & wxSIZE_ALLOW_MINUS_ONE))
        {
            XtVaSetValues (m_borderWidget ? (Widget) m_borderWidget : (Widget) m_scrolledWindow,
                XmNy, yy, NULL);
        }
    }
    
    if (!useOldSize)
    {
        
        if (w > -1)
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
            
            Dimension spacing;
            Widget sbar;
            XtVaGetValues ((Widget) m_scrolledWindow,
                XmNspacing, &spacing,
                XmNverticalScrollBar, &sbar,
                NULL);
            Dimension wsbar;
            if (sbar)
                XtVaGetValues (sbar, XmNwidth, &wsbar, NULL);
            else
                wsbar = 0;
            
            w -= (spacing + wsbar);
            
            //      XtVaSetValues ((Widget) m_drawingArea, XmNwidth, w, NULL);
        }
        if (h > -1)
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
            
            Dimension spacing;
            Widget sbar;
            XtVaGetValues ((Widget) m_scrolledWindow,
                XmNspacing, &spacing,
                XmNhorizontalScrollBar, &sbar,
                NULL);
            Dimension wsbar;
            if (sbar)
                XtVaGetValues (sbar, XmNheight, &wsbar, NULL);
            else
                wsbar = 0;
            
            h -= (spacing + wsbar);
            
            //      XtVaSetValues ((Widget) m_drawingArea, XmNheight, h, NULL);
            
        }
    }
    
    if (managed)
        XtManageChild (m_borderWidget ? (Widget) m_borderWidget : (Widget) m_scrolledWindow);
    XtVaSetValues((Widget) m_drawingArea, XmNresizePolicy, XmRESIZE_NONE, NULL);
    
    /*
    int ww, hh;
    GetClientSize (&ww, &hh);
    wxSizeEvent sizeEvent(wxSize(ww, hh), GetId());
    sizeEvent.SetEventObject(this);
    
      GetEventHandler()->ProcessEvent(sizeEvent);
    */
    
}

void wxWindow::CanvasSetClientSize (int w, int h)
{
    Widget drawingArea = (Widget) m_drawingArea;
    
    XtVaSetValues((Widget) m_drawingArea, XmNresizePolicy, XmRESIZE_ANY, NULL);
    
    if (w > -1)
        XtVaSetValues ((Widget) m_drawingArea, XmNwidth, w, NULL);
    if (h > -1)
        XtVaSetValues ((Widget) m_drawingArea, XmNheight, h, NULL);
        /* TODO: is this necessary?
        allowRepainting = FALSE;
        
          XSync (XtDisplay (drawingArea), FALSE);
          XEvent event;
          while (XtAppPending (wxTheApp->appContext))
          {
          XFlush (XtDisplay (drawingArea));
          XtAppNextEvent (wxTheApp->appContext, &event);
          XtDispatchEvent (&event);
          }
    */
    
    XtVaSetValues((Widget) m_drawingArea, XmNresizePolicy, XmRESIZE_NONE, NULL);
    
    /* TODO
    allowRepainting = TRUE;
    DoRefresh ();
    */
    
    /*
    wxSizeEvent sizeEvent(wxSize(w, h), GetId());
    sizeEvent.SetEventObject(this);
    
      GetEventHandler()->ProcessEvent(sizeEvent);
    */
}

void wxWindow::CanvasGetClientSize (int *w, int *h) const
{
    // Must return the same thing that was set via SetClientSize
    Dimension xx, yy;
    XtVaGetValues ((Widget) m_drawingArea, XmNwidth, &xx, XmNheight, &yy, NULL);
    *w = xx;
    *h = yy;
}

void wxWindow::CanvasGetSize (int *w, int *h) const
{
    Dimension xx, yy;
    if ((Widget) m_borderWidget)
        XtVaGetValues ((Widget) m_borderWidget, XmNwidth, &xx, XmNheight, &yy, NULL);
    else if ((Widget) m_scrolledWindow)
        XtVaGetValues ((Widget) m_scrolledWindow, XmNwidth, &xx, XmNheight, &yy, NULL);
    else
        XtVaGetValues ((Widget) m_drawingArea, XmNwidth, &xx, XmNheight, &yy, NULL);
    
    *w = xx;
    *h = yy;
}

void wxWindow::CanvasGetPosition (int *x, int *y) const
{
    Position xx, yy;
    XtVaGetValues (m_borderWidget ? (Widget) m_borderWidget : (Widget) m_scrolledWindow, XmNx, &xx, XmNy, &yy, NULL);
    
    // We may be faking the client origin.
    // So a window that's really at (0, 30) may appear
    // (to wxWin apps) to be at (0, 0).
    if (GetParent())
    {
        wxPoint pt(GetParent()->GetClientAreaOrigin());
        xx -= pt.x;
        yy -= pt.y;
    }
    
    *x = xx;
    *y = yy;
}

// Add to hash table, add event handler
bool wxWindow::AttachWidget (wxWindow* parent, WXWidget mainWidget,
                             WXWidget formWidget, int x, int y, int width, int height)
{
    wxAddWindowToTable((Widget) mainWidget, this);
    if (CanAddEventHandler())
    {
        XtAddEventHandler((Widget) mainWidget,
            ButtonPressMask | ButtonReleaseMask | PointerMotionMask, // | KeyPressMask,
            False,
            wxPanelItemEventHandler,
            (XtPointer) this);
    }
    
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
    if (CanAddEventHandler())
    {
        XtRemoveEventHandler((Widget) widget,
            ButtonPressMask | ButtonReleaseMask | PointerMotionMask, // | KeyPressMask,
            False,
            wxPanelItemEventHandler,
            (XtPointer)this);
    }
    
    wxDeleteWindowFromTable((Widget) widget);
    return TRUE;
}

void wxPanelItemEventHandler (Widget    wid,
                              XtPointer client_data,
                              XEvent*   event,
                              Boolean *continueToDispatch)
{
    // Widget can be a label or the actual widget.
    
    wxWindow *window = (wxWindow *)wxWidgetHashTable->Get((long)wid);
    if (window)
    {
        wxMouseEvent wxevent(0);
        if (wxTranslateMouseEvent(wxevent, window, wid, event))
        {
            window->GetEventHandler()->ProcessEvent(wxevent);
        }
    }
    // TODO: probably the key to allowing default behaviour
    // to happen.
    // Say we set a m_doDefault flag to FALSE at the start of this
    // function. Then in e.g. wxWindow::OnMouseEvent we can
    // call Default() which sets this flag to TRUE, indicating
    // that default processing can happen. Thus, behaviour can appear
    // to be overridden just by adding an event handler and not calling
    // wxWindow::OnWhatever.
    // ALSO, maybe we can use this instead of the current way of handling
    // drawing area events, to simplify things.
    *continueToDispatch = True;
}

static void wxScrollBarCallback(Widget scrollbar, XtPointer clientData,
                                XmScaleCallbackStruct *cbs)
{
    Widget scrolledWindow = XtParent (scrollbar);
    wxWindow *win = (wxWindow *) wxWidgetHashTable->Get ((long) scrolledWindow);
    int orientation = (int) clientData;
    
    wxEventType eventType = wxEVT_NULL;
    switch (cbs->reason)
    {
    case XmCR_INCREMENT:
        {
            eventType = wxEVT_SCROLL_LINEDOWN;
            break;
        }
    case XmCR_DECREMENT:
        {
            eventType = wxEVT_SCROLL_LINEUP;
            break;
        }
    case XmCR_DRAG:
        {
            eventType = wxEVT_SCROLL_THUMBTRACK;
            break;
        }
    case XmCR_VALUE_CHANGED:
        {
            // TODO: Should this be intercepted too, or will it cause
            // duplicate events?
            eventType = wxEVT_SCROLL_THUMBTRACK;
            break;
        }
    case XmCR_PAGE_INCREMENT:
        {
            eventType = wxEVT_SCROLL_PAGEDOWN;
            break;
        }
    case XmCR_PAGE_DECREMENT:
        {
            eventType = wxEVT_SCROLL_PAGEUP;
            break;
        }
    case XmCR_TO_TOP:
        {
            eventType = wxEVT_SCROLL_TOP;
            break;
        }
    case XmCR_TO_BOTTOM:
        {
            eventType = wxEVT_SCROLL_BOTTOM;
            break;
        }
    default:
        {
            // Should never get here
            wxFAIL_MSG("Unknown scroll event.");
            break;
        }
    }
    
    wxScrollEvent event(eventType, win->GetId());
    event.SetEventObject(win);
    event.SetPosition(cbs->value);
    event.SetOrientation( (orientation == XmHORIZONTAL) ? wxHORIZONTAL : wxVERTICAL );
    
    win->GetEventHandler()->ProcessEvent(event);
}

bool wxTranslateMouseEvent(wxMouseEvent& wxevent, wxWindow *win, Widget widget, XEvent *xevent)
{
    switch (xevent->xany.type)
    {
    case EnterNotify:
    case LeaveNotify:
    case ButtonPress:
    case ButtonRelease:
    case MotionNotify:
        {
            wxEventType eventType = wxEVT_NULL;
            
            if (xevent->xany.type == LeaveNotify)
            {
                win->m_button1Pressed = FALSE;
                win->m_button2Pressed = FALSE;
                win->m_button3Pressed = FALSE;
                return FALSE;
            }
            else if (xevent->xany.type == MotionNotify)
            {
                eventType = wxEVT_MOTION;
            }
            else if (xevent->xany.type == ButtonPress)
            {
                if (xevent->xbutton.button == Button1)
                {
                    eventType = wxEVT_LEFT_DOWN;
                    win->m_button1Pressed = TRUE;
                }
                else if (xevent->xbutton.button == Button2)
                {
                    eventType = wxEVT_MIDDLE_DOWN;
                    win->m_button2Pressed = TRUE;
                }
                else if (xevent->xbutton.button == Button3)
                {
                    eventType = wxEVT_RIGHT_DOWN;
                    win->m_button3Pressed = TRUE;
                }
            }
            else if (xevent->xany.type == ButtonRelease)
            {
                if (xevent->xbutton.button == Button1)
                {
                    eventType = wxEVT_LEFT_UP;
                    win->m_button1Pressed = FALSE;
                }
                else if (xevent->xbutton.button == Button2)
                {
                    eventType = wxEVT_MIDDLE_UP;
                    win->m_button2Pressed = FALSE;
                }
                else if (xevent->xbutton.button == Button3)
                {
                    eventType = wxEVT_RIGHT_UP;
                    win->m_button3Pressed = FALSE;
                }
                else return FALSE;
            }
            else return FALSE;
            
            wxevent.m_eventHandle = (char *)xevent;
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
            return TRUE;
        }
    }
    return FALSE;
}

bool wxTranslateKeyEvent(wxKeyEvent& wxevent, wxWindow *win, Widget widget, XEvent *xevent)
{
    switch (xevent->xany.type)
    {
    case KeyPress:
        {
            char buf[20];
            
            KeySym keySym;
            //	XComposeStatus compose;
            //	(void) XLookupString ((XKeyEvent *) xevent, buf, 20, &keySym, &compose);
            (void) XLookupString ((XKeyEvent *) xevent, buf, 20, &keySym, NULL);
            int id = wxCharCodeXToWX (keySym);
            
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
    
    return (result);
    
}

// Changes the foreground and background colours to be derived
// from the current background colour.
// To change the foreground colour, you must call SetForegroundColour
// explicitly.
void wxWindow::ChangeBackgroundColour()
{
    if (GetMainWidget())
        DoChangeBackgroundColour(GetMainWidget(), m_backgroundColour);
    
    // This not necessary
#if 0
    
    if (m_scrolledWindow && (GetMainWidget() != m_scrolledWindow))
    {
        DoChangeBackgroundColour(m_scrolledWindow, m_backgroundColour);
        // Have to set the scrollbar colours back since
        // the scrolled window seemed to change them
        wxColour backgroundColour = wxSystemSettings::GetSystemColour(wxSYS_COLOUR_3DFACE) ;
        
        if (m_hScrollBar)
            DoChangeBackgroundColour(m_hScrollBar, backgroundColour);
        if (m_vScrollBar)
            DoChangeBackgroundColour(m_vScrollBar, backgroundColour);
    }
#endif
}

void wxWindow::ChangeForegroundColour()
{
    if (GetMainWidget())
        DoChangeForegroundColour(GetMainWidget(), m_foregroundColour);
    if (m_scrolledWindow && (GetMainWidget() != m_scrolledWindow))
        DoChangeForegroundColour(m_scrolledWindow, m_foregroundColour);
}

// Change a widget's foreground and background colours.

void wxWindow::DoChangeForegroundColour(WXWidget widget, wxColour& foregroundColour)
{
    // When should we specify the foreground, if it's calculated
    // by wxComputeColours?
    // Solution: say we start with the default (computed) foreground colour.
    // If we call SetForegroundColour explicitly for a control or window,
    // then the foreground is changed.
    // Therefore SetBackgroundColour computes the foreground colour, and
    // SetForegroundColour changes the foreground colour. The ordering is
    // important.
    
    XtVaSetValues ((Widget) widget,
        XmNforeground, foregroundColour.AllocColour(XtDisplay((Widget) widget)),
        NULL);
}

void wxWindow::DoChangeBackgroundColour(WXWidget widget, wxColour& backgroundColour, bool changeArmColour)
{
    wxComputeColours (XtDisplay((Widget) widget), & backgroundColour,
        (wxColour*) NULL);
    
    XtVaSetValues ((Widget) widget,
        XmNbackground, g_itemColors[wxBACK_INDEX].pixel,
        XmNtopShadowColor, g_itemColors[wxTOPS_INDEX].pixel,
        XmNbottomShadowColor, g_itemColors[wxBOTS_INDEX].pixel,
        XmNforeground, g_itemColors[wxFORE_INDEX].pixel,
        NULL);
    
    if (changeArmColour)
        XtVaSetValues ((Widget) widget,
        XmNarmColor, g_itemColors[wxSELE_INDEX].pixel,
        NULL);
}

void wxWindow::SetBackgroundColour(const wxColour& col)
{
    m_backgroundColour = col;
    ChangeBackgroundColour();
}

void wxWindow::SetForegroundColour(const wxColour& col)
{
    m_foregroundColour = col;
    ChangeForegroundColour();
}

void wxWindow::ChangeFont(bool keepOriginalSize)
{
    // Note that this causes the widget to be resized back
    // to its original size! We therefore have to set the size
    // back again. TODO: a better way in Motif?
    Widget w = (Widget) GetLabelWidget(); // Usually the main widget
    if (w && m_windowFont.Ok())
    {
        int width, height, width1, height1;
        GetSize(& width, & height);
        
        // lesstif 0.87 hangs here
#ifndef LESSTIF_VERSION	
        XtVaSetValues (w,
            XmNfontList, (XmFontList) m_windowFont.GetFontList(1.0, XtDisplay(w)),
            NULL);
#endif
        
        GetSize(& width1, & height1);
        if (keepOriginalSize && (width != width1 || height != height1))
        {
            SetSize(-1, -1, width, height);
        }
    }
}

void wxWindow::SetFont(const wxFont& font)
{
    m_windowFont = font;
    ChangeFont();
}

void wxWindow::SetToolTip(const wxString& tooltip)
{
    // TODO
}

void wxWindow::ClearUpdateRects()
{
    wxNode* node = m_updateRects.First();
    while (node)
    {
        wxRect* rect = (wxRect*) node->Data();
        delete rect;
        node = node->Next();
    }
    m_updateRects.Clear();
}

bool wxWindow::ProcessAccelerator(wxKeyEvent& event)
{
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
            // Bingo, we have a match. Now find a control
            // that matches the entry command id.
            
            // Need to go up to the top of the window hierarchy,
            // since it might be e.g. a menu item
            wxWindow* parent = this;
            while (parent && !parent->IsKindOf(CLASSINFO(wxFrame)) && !parent->IsKindOf(CLASSINFO(wxDialog)))
                parent = parent->GetParent();
            
            if (!parent)
                return FALSE;
            
            if (parent->IsKindOf(CLASSINFO(wxFrame)))
            {
                // Try for a menu command
                wxFrame* frame = (wxFrame*) parent;
                if (frame->GetMenuBar())
                {
                    wxMenuItem* item = frame->GetMenuBar()->FindItemForId(entry->GetCommand());
                    if (item)
                    {
                        wxCommandEvent commandEvent(wxEVT_COMMAND_MENU_SELECTED, entry->GetCommand());
                        commandEvent.SetEventObject(frame);
                        
                        // If ProcessEvent returns TRUE (it was handled), then
                        // the calling code will skip the event handling.
                        return frame->GetEventHandler()->ProcessEvent(commandEvent);
                    }
                }
            }
            
            // Find a child matching the command id
            wxWindow* child = parent->FindWindow(entry->GetCommand());
            
            // No such child
            if (!child)
                return FALSE;
            
            // Now we process those kinds of windows that we can.
            // For now, only buttons.
            if (child->IsKindOf(CLASSINFO(wxButton)))
            {
                wxCommandEvent commandEvent (wxEVT_COMMAND_BUTTON_CLICKED, child->GetId());
                commandEvent.SetEventObject(child);
                return child->GetEventHandler()->ProcessEvent(commandEvent);
            }
            
            return FALSE;
        } // matches event
    }// for
    
    // We didn't match the key event against an accelerator.
    return FALSE;
}

/*
* wxNoOptimize: switch off size optimization
*/

int wxNoOptimize::m_count = 0;

wxNoOptimize::wxNoOptimize()
{
    m_count ++;
}

wxNoOptimize::~wxNoOptimize()
{
    m_count --;
}

bool wxNoOptimize::CanOptimize()
{
    return (m_count == 0);
}

