/////////////////////////////////////////////////////////////////////////////
// Name:        msw/frame.cpp
// Purpose:     wxFrame
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
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

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "frame.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/frame.h"
    #include "wx/app.h"
    #include "wx/menu.h"
    #include "wx/utils.h"
    #include "wx/dialog.h"
    #include "wx/settings.h"
    #include "wx/dcclient.h"
    #include "wx/mdi.h"
    #include "wx/panel.h"
#endif // WX_PRECOMP

#include "wx/msw/private.h"

#ifdef __WXWINCE__
#include <commctrl.h>
#endif

#if wxUSE_STATUSBAR
    #include "wx/statusbr.h"
    #include "wx/generic/statusbr.h"
#endif // wxUSE_STATUSBAR

#if wxUSE_TOOLBAR
    #include "wx/toolbar.h"
#endif // wxUSE_TOOLBAR

#include "wx/menuitem.h"
#include "wx/log.h"

#ifdef __WXUNIVERSAL__
    #include "wx/univ/theme.h"
    #include "wx/univ/colschem.h"
#endif // __WXUNIVERSAL__

// ----------------------------------------------------------------------------
// globals
// ----------------------------------------------------------------------------

#if wxUSE_MENUS_NATIVE
    extern wxMenu *wxCurrentPopupMenu;
#endif // wxUSE_MENUS_NATIVE

// ----------------------------------------------------------------------------
// event tables
// ----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(wxFrame, wxFrameBase)
    EVT_SYS_COLOUR_CHANGED(wxFrame::OnSysColourChanged)
END_EVENT_TABLE()

#if wxUSE_EXTENDED_RTTI
WX_DEFINE_FLAGS( wxFrameStyle )

wxBEGIN_FLAGS( wxFrameStyle )
    // new style border flags, we put them first to
    // use them for streaming out
    wxFLAGS_MEMBER(wxBORDER_SIMPLE)
    wxFLAGS_MEMBER(wxBORDER_SUNKEN)
    wxFLAGS_MEMBER(wxBORDER_DOUBLE)
    wxFLAGS_MEMBER(wxBORDER_RAISED)
    wxFLAGS_MEMBER(wxBORDER_STATIC)
    wxFLAGS_MEMBER(wxBORDER_NONE)
    
    // old style border flags
    wxFLAGS_MEMBER(wxSIMPLE_BORDER)
    wxFLAGS_MEMBER(wxSUNKEN_BORDER)
    wxFLAGS_MEMBER(wxDOUBLE_BORDER)
    wxFLAGS_MEMBER(wxRAISED_BORDER)
    wxFLAGS_MEMBER(wxSTATIC_BORDER)
    wxFLAGS_MEMBER(wxBORDER)

    // standard window styles
    wxFLAGS_MEMBER(wxTAB_TRAVERSAL)
    wxFLAGS_MEMBER(wxCLIP_CHILDREN)
    wxFLAGS_MEMBER(wxTRANSPARENT_WINDOW)
    wxFLAGS_MEMBER(wxWANTS_CHARS)
    wxFLAGS_MEMBER(wxFULL_REPAINT_ON_RESIZE)
    wxFLAGS_MEMBER(wxALWAYS_SHOW_SB )
    wxFLAGS_MEMBER(wxVSCROLL)
    wxFLAGS_MEMBER(wxHSCROLL)

    // frame styles
    wxFLAGS_MEMBER(wxSTAY_ON_TOP)
    wxFLAGS_MEMBER(wxCAPTION)
    wxFLAGS_MEMBER(wxTHICK_FRAME)
    wxFLAGS_MEMBER(wxSYSTEM_MENU)
    wxFLAGS_MEMBER(wxRESIZE_BORDER)
    wxFLAGS_MEMBER(wxRESIZE_BOX)
    wxFLAGS_MEMBER(wxCLOSE_BOX)
    wxFLAGS_MEMBER(wxMAXIMIZE_BOX)
    wxFLAGS_MEMBER(wxMINIMIZE_BOX)

    wxFLAGS_MEMBER(wxFRAME_TOOL_WINDOW)
    wxFLAGS_MEMBER(wxFRAME_FLOAT_ON_PARENT)

    wxFLAGS_MEMBER(wxFRAME_SHAPED)

wxEND_FLAGS( wxFrameStyle )

IMPLEMENT_DYNAMIC_CLASS_XTI(wxFrame, wxTopLevelWindow,"wx/frame.h")

wxBEGIN_PROPERTIES_TABLE(wxFrame)
	wxEVENT_PROPERTY( Menu , wxEVT_COMMAND_MENU_SELECTED , wxCommandEvent)

    wxPROPERTY( Title,wxString, SetTitle, GetTitle, wxString() , 0 /*flags*/ , wxT("Helpstring") , wxT("group"))
    wxPROPERTY_FLAGS( WindowStyle , wxFrameStyle , long , SetWindowStyleFlag , GetWindowStyleFlag , EMPTY_MACROVALUE , 0 /*flags*/ , wxT("Helpstring") , wxT("group")) // style
    wxPROPERTY( MenuBar , wxMenuBar * , SetMenuBar , GetMenuBar , EMPTY_MACROVALUE , 0 /*flags*/ , wxT("Helpstring") , wxT("group"))
wxEND_PROPERTIES_TABLE()

wxBEGIN_HANDLERS_TABLE(wxFrame)
wxEND_HANDLERS_TABLE()

wxCONSTRUCTOR_6( wxFrame , wxWindow* , Parent , wxWindowID , Id , wxString , Title , wxPoint , Position , wxSize , Size , long , WindowStyle) 

#else
IMPLEMENT_DYNAMIC_CLASS(wxFrame, wxTopLevelWindow)
#endif

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// static class members
// ----------------------------------------------------------------------------

#if wxUSE_STATUSBAR
    #if wxUSE_NATIVE_STATUSBAR
        bool wxFrame::m_useNativeStatusBar = TRUE;
    #else
        bool wxFrame::m_useNativeStatusBar = FALSE;
    #endif
#endif // wxUSE_NATIVE_STATUSBAR

// ----------------------------------------------------------------------------
// creation/destruction
// ----------------------------------------------------------------------------

void wxFrame::Init()
{
#if wxUSE_TOOLTIPS
    m_hwndToolTip = 0;
#endif

    // Data to save/restore when calling ShowFullScreen
    m_fsStatusBarFields = 0;
    m_fsStatusBarHeight = 0;
    m_fsToolBarHeight = 0;

    m_wasMinimized = FALSE;
}

bool wxFrame::Create(wxWindow *parent,
                     wxWindowID id,
                     const wxString& title,
                     const wxPoint& pos,
                     const wxSize& size,
                     long style,
                     const wxString& name)
{
    if ( !wxTopLevelWindow::Create(parent, id, title, pos, size, style, name) )
        return FALSE;

    SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_APPWORKSPACE));

    return TRUE;
}

wxFrame::~wxFrame()
{
    m_isBeingDeleted = TRUE;
    DeleteAllBars();
}

// ----------------------------------------------------------------------------
// wxFrame client size calculations
// ----------------------------------------------------------------------------

void wxFrame::DoSetClientSize(int width, int height)
{
    // leave enough space for the status bar if we have (and show) it
#if wxUSE_STATUSBAR
    wxStatusBar *statbar = GetStatusBar();
    if ( statbar && statbar->IsShown() )
    {
        height += statbar->GetSize().y;
    }
#endif // wxUSE_STATUSBAR

    // call GetClientAreaOrigin() to take the toolbar into account
    wxPoint pt = GetClientAreaOrigin();
    width += pt.x;
    height += pt.y;

    wxTopLevelWindow::DoSetClientSize(width, height);
}

// Get size *available for subwindows* i.e. excluding menu bar, toolbar etc.
void wxFrame::DoGetClientSize(int *x, int *y) const
{
    wxTopLevelWindow::DoGetClientSize(x, y);

    // account for the possible toolbar
    wxPoint pt = GetClientAreaOrigin();
    if ( x )
        *x -= pt.x;

    if ( y )
        *y -= pt.y;

#if wxUSE_STATUSBAR
    // adjust client area height to take the status bar into account
    if ( y )
    {
        wxStatusBar *statbar = GetStatusBar();
        if ( statbar && statbar->IsShown() )
        {
            *y -= statbar->GetClientSize().y;
        }
    }
#endif // wxUSE_STATUSBAR
}

// ----------------------------------------------------------------------------
// wxFrame: various geometry-related functions
// ----------------------------------------------------------------------------

void wxFrame::Raise()
{
    ::SetForegroundWindow(GetHwnd());
}

// generate an artificial resize event
void wxFrame::SendSizeEvent()
{
    if ( !m_iconized )
    {
        RECT r = wxGetWindowRect(GetHwnd());

        (void)::PostMessage(GetHwnd(), WM_SIZE,
                            IsMaximized() ? SIZE_MAXIMIZED : SIZE_RESTORED,
                            MAKELPARAM(r.right - r.left, r.bottom - r.top));
    }
}

#if wxUSE_STATUSBAR
wxStatusBar *wxFrame::OnCreateStatusBar(int number,
                                        long style,
                                        wxWindowID id,
                                        const wxString& name)
{
    wxStatusBar *statusBar = NULL;

#if wxUSE_NATIVE_STATUSBAR
    if ( !UsesNativeStatusBar() )
    {
        statusBar = (wxStatusBar *)new wxStatusBarGeneric(this, id, style);
    }
    else
#endif
    {
        statusBar = new wxStatusBar(this, id, style, name);
    }

    statusBar->SetFieldsCount(number);

    return statusBar;
}

void wxFrame::PositionStatusBar()
{
    if ( !m_frameStatusBar || !m_frameStatusBar->IsShown() )
        return;

    int w, h;
    GetClientSize(&w, &h);
    int sw, sh;
    m_frameStatusBar->GetSize(&sw, &sh);

    // Since we wish the status bar to be directly under the client area,
    // we use the adjusted sizes without using wxSIZE_NO_ADJUSTMENTS.
    m_frameStatusBar->SetSize(0, h, w, sh);
}
#endif // wxUSE_STATUSBAR

#if wxUSE_MENUS_NATIVE

void wxFrame::AttachMenuBar(wxMenuBar *menubar)
{
#if defined(__WXWINCE__) && (_WIN32_WCE < 400 || defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP))
    if (!GetToolBar())
    {
        wxToolBar* toolBar = new wxToolBar(this, -1,
                         wxDefaultPosition, wxDefaultSize,
                         wxBORDER_NONE | wxTB_HORIZONTAL,
                         wxToolBarNameStr, menubar);
        SetToolBar(toolBar);
        menubar->SetToolBar(toolBar);
    }
	// Now adjust size for menu bar
	int menuHeight = 26;
	
	//When the main window is created using CW_USEDEFAULT the height of the
	// is created is not taken into account). So we resize the window after
	// if a menubar is present
	{
		RECT rc;
		::GetWindowRect((HWND) GetHWND(), &rc);
		// adjust for menu / titlebar height
		rc.bottom -= (2*menuHeight-1);
		
		MoveWindow((HWND) GetHWND(), rc.left, rc.top, rc.right, rc.bottom, FALSE);
	}
#endif

    wxFrameBase::AttachMenuBar(menubar);

    if ( !menubar )
    {
        // actually remove the menu from the frame
        m_hMenu = (WXHMENU)0;
        InternalSetMenuBar();
    }
    else // set new non NULL menu bar
    {
#if !defined(__WXWINCE__) || (_WIN32_WCE >= 400 && !defined(WIN32_PLATFORM_PSPC) && !defined(WIN32_PLATFORM_WFSP))
        // Can set a menubar several times.
        if ( menubar->GetHMenu() )
        {
            m_hMenu = menubar->GetHMenu();
        }
        else // no HMENU yet
        {
            m_hMenu = menubar->Create();

            if ( !m_hMenu )
            {
                wxFAIL_MSG( _T("failed to create menu bar") );
                return;
            }
        }
#endif
        InternalSetMenuBar();
    }
}

void wxFrame::InternalSetMenuBar()
{
#if defined(__WXMICROWIN__) || defined(__WXWINCE__)
    // Nothing
#else
    if ( !::SetMenu(GetHwnd(), (HMENU)m_hMenu) )
    {
        wxLogLastError(wxT("SetMenu"));
    }
#endif
}

#endif // wxUSE_MENUS_NATIVE

// Responds to colour changes, and passes event on to children.
void wxFrame::OnSysColourChanged(wxSysColourChangedEvent& event)
{
    SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_APPWORKSPACE));
    Refresh();

#if wxUSE_STATUSBAR
    if ( m_frameStatusBar )
    {
        wxSysColourChangedEvent event2;
        event2.SetEventObject( m_frameStatusBar );
        m_frameStatusBar->GetEventHandler()->ProcessEvent(event2);
    }
#endif // wxUSE_STATUSBAR

    // Propagate the event to the non-top-level children
    wxWindow::OnSysColourChanged(event);
}

// Pass TRUE to show full screen, FALSE to restore.
bool wxFrame::ShowFullScreen(bool show, long style)
{
    if ( IsFullScreen() == show )
        return FALSE;

    if (show)
    {
#if wxUSE_TOOLBAR
#if defined(__WXWINCE__) && (_WIN32_WCE < 400 || defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP))
        // TODO: hide commandbar
#else
        wxToolBar *theToolBar = GetToolBar();
        if (theToolBar)
            theToolBar->GetSize(NULL, &m_fsToolBarHeight);

        // zap the toolbar, menubar, and statusbar

        if ((style & wxFULLSCREEN_NOTOOLBAR) && theToolBar)
        {
            theToolBar->SetSize(-1,0);
            theToolBar->Show(FALSE);
        }
#endif // __WXWINCE__
#endif // wxUSE_TOOLBAR

#if defined(__WXMICROWIN__)
#elif defined(__WXWINCE__)
        // TODO: make it work for WinCE
#else
        if (style & wxFULLSCREEN_NOMENUBAR)
            SetMenu((HWND)GetHWND(), (HMENU) NULL);
#endif

#if wxUSE_STATUSBAR
        wxStatusBar *theStatusBar = GetStatusBar();
        if (theStatusBar)
            theStatusBar->GetSize(NULL, &m_fsStatusBarHeight);

        // Save the number of fields in the statusbar
        if ((style & wxFULLSCREEN_NOSTATUSBAR) && theStatusBar)
        {
            //m_fsStatusBarFields = theStatusBar->GetFieldsCount();
            //SetStatusBar((wxStatusBar*) NULL);
            //delete theStatusBar;
            theStatusBar->Show(FALSE);
        }
        else
            m_fsStatusBarFields = 0;
#endif // wxUSE_STATUSBAR
    }
    else
    {
#if wxUSE_TOOLBAR
#if defined(__WXWINCE__) && (_WIN32_WCE < 400 || defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP))
        // TODO: show commandbar
#else
        wxToolBar *theToolBar = GetToolBar();

        // restore the toolbar, menubar, and statusbar
        if (theToolBar && (m_fsStyle & wxFULLSCREEN_NOTOOLBAR))
        {
            theToolBar->SetSize(-1, m_fsToolBarHeight);
            theToolBar->Show(TRUE);
        }
#endif // __WXWINCE__
#endif // wxUSE_TOOLBAR

#if wxUSE_STATUSBAR
        if ( m_fsStyle & wxFULLSCREEN_NOSTATUSBAR )
        {
            //CreateStatusBar(m_fsStatusBarFields);
            if (GetStatusBar())
            {
                GetStatusBar()->Show(TRUE);
                PositionStatusBar();
            }
        }
#endif // wxUSE_STATUSBAR

#if defined(__WXMICROWIN__)
#elif defined(__WXWINCE__)
        // TODO: make it work for WinCE
#else
        if ((m_fsStyle & wxFULLSCREEN_NOMENUBAR) && (m_hMenu != 0))
            SetMenu((HWND)GetHWND(), (HMENU)m_hMenu);
#endif
    }

    return wxFrameBase::ShowFullScreen(show, style);
}

// ----------------------------------------------------------------------------
// tool/status bar stuff
// ----------------------------------------------------------------------------

#if wxUSE_TOOLBAR

wxToolBar* wxFrame::CreateToolBar(long style, wxWindowID id, const wxString& name)
{
#if defined(__WXWINCE__) && (_WIN32_WCE < 400 || defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP))
    // We may already have a toolbar from calling SetMenuBar.
    if (GetToolBar())
        return GetToolBar();
#endif
    if ( wxFrameBase::CreateToolBar(style, id, name) )
    {
        PositionToolBar();
    }

    return m_frameToolBar;
}

void wxFrame::PositionToolBar()
{
    wxToolBar *toolbar = GetToolBar();
    if ( toolbar && toolbar->IsShown() )
    {
#if defined(__WXWINCE__) && (_WIN32_WCE < 400 || defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP))
        // We want to do something different in WinCE, because
        // the toolbar should be associated with the commandbar,
        // and not an independent window.
        // TODO
#else
        // don't call our (or even wxTopLevelWindow) version because we want
        // the real (full) client area size, not excluding the tool/status bar
        int width, height;
        wxWindow::DoGetClientSize(&width, &height);

#if wxUSE_STATUSBAR
        wxStatusBar *statbar = GetStatusBar();
        if ( statbar && statbar->IsShown() )
        {
            height -= statbar->GetClientSize().y;
        }
#endif // wxUSE_STATUSBAR

        int tx, ty;
        int tw, th;
        toolbar->GetPosition(&tx, &ty);
        toolbar->GetSize(&tw, &th);
        
        // Adjust
        if (ty < 0 && (-ty == th))
            ty = 0;
        if (tx < 0 && (-tx == tw))
            tx = 0;        
        
        int desiredW = tw;
        int desiredH = th;

        if ( toolbar->GetWindowStyleFlag() & wxTB_VERTICAL )
        {
            desiredH = height;
        }
        else
        {
            desiredW = width;
//            if ( toolbar->GetWindowStyleFlag() & wxTB_FLAT )
//                desiredW -= 3;
        }        

        // use the 'real' MSW position here, don't offset relativly to the
        // client area origin

        // Optimise such that we don't have to always resize the toolbar
        // when the frame changes, otherwise we'll get a lot of flicker.        
        bool heightChanging = TRUE;
        bool widthChanging = TRUE;
        
        if ( toolbar->GetWindowStyleFlag() & wxTB_VERTICAL )
        {
            // It's OK if the current height is greater than what can be shown.
            heightChanging = (desiredH > th) ;
            widthChanging = (desiredW != tw) ;
            
            // The next time around, we may not have to set the size            
            if (heightChanging)
                desiredH = desiredH + 200;
        }
        else
        {
            // It's OK if the current width is greater than what can be shown.
            widthChanging = (desiredW > tw) ;
            heightChanging = (desiredH != th) ;

            // The next time around, we may not have to set the size            
            if (widthChanging)
                desiredW = desiredW + 200;
        }
        
        if (tx != 0 || ty != 0 || widthChanging || heightChanging)
            toolbar->SetSize(0, 0, desiredW, desiredH, wxSIZE_NO_ADJUSTMENTS);
        
#endif // __WXWINCE__
    }
}

#endif // wxUSE_TOOLBAR

// ----------------------------------------------------------------------------
// frame state (iconized/maximized/...)
// ----------------------------------------------------------------------------

// propagate our state change to all child frames: this allows us to emulate X
// Windows behaviour where child frames float independently of the parent one
// on the desktop, but are iconized/restored with it
void wxFrame::IconizeChildFrames(bool bIconize)
{
    for ( wxWindowList::compatibility_iterator node = GetChildren().GetFirst();
          node;
          node = node->GetNext() )
    {
        wxWindow *win = node->GetData();

        // iconizing the frames with this style under Win95 shell puts them at
        // the bottom of the screen (as the MDI children) instead of making
        // them appear in the taskbar because they are, by virtue of this
        // style, not managed by the taskbar - instead leave Windows take care
        // of them
#ifdef __WIN95__
        if ( win->GetWindowStyle() & wxFRAME_TOOL_WINDOW )
            continue;
#endif // Win95

        // the child MDI frames are a special case and should not be touched by
        // the parent frame - instead, they are managed by the user
        wxFrame *frame = wxDynamicCast(win, wxFrame);
        if ( frame
#if wxUSE_MDI_ARCHITECTURE
                && !wxDynamicCast(frame, wxMDIChildFrame)
#endif // wxUSE_MDI_ARCHITECTURE
           )
        {
            // we don't want to restore the child frames which had been
            // iconized even before we were iconized, so save the child frame
            // status when iconizing the parent frame and check it when
            // restoring it
            if ( bIconize )
            {
                frame->m_wasMinimized = frame->IsIconized();
            }

            // note that we shouldn't touch the hidden frames neither because
            // iconizing/restoring them would show them as a side effect
            if ( !frame->m_wasMinimized && frame->IsShown() )
                frame->Iconize(bIconize);
        }
    }
}

WXHICON wxFrame::GetDefaultIcon() const
{
    // we don't have any standard icons (any more)
    return (WXHICON)0;
}

// ===========================================================================
// message processing
// ===========================================================================

// ---------------------------------------------------------------------------
// preprocessing
// ---------------------------------------------------------------------------

bool wxFrame::MSWTranslateMessage(WXMSG* pMsg)
{
    if ( wxWindow::MSWTranslateMessage(pMsg) )
        return TRUE;

#if wxUSE_MENUS && wxUSE_ACCEL && !defined(__WXUNIVERSAL__)
    // try the menu bar accels
    wxMenuBar *menuBar = GetMenuBar();
    if ( !menuBar )
        return FALSE;

    const wxAcceleratorTable& acceleratorTable = menuBar->GetAccelTable();
    return acceleratorTable.Translate(this, pMsg);
#else
    return FALSE;
#endif // wxUSE_MENUS && wxUSE_ACCEL
}

// ---------------------------------------------------------------------------
// our private (non virtual) message handlers
// ---------------------------------------------------------------------------

bool wxFrame::HandlePaint()
{
    RECT rect;
    if ( GetUpdateRect(GetHwnd(), &rect, FALSE) )
    {
#if !defined(__WXMICROWIN__) && !defined(__WXWINCE__)
        if ( m_iconized )
        {
            const wxIcon& icon = GetIcon();
            HICON hIcon = icon.Ok() ? GetHiconOf(icon)
                                    : (HICON)GetDefaultIcon();

            // Hold a pointer to the dc so long as the OnPaint() message
            // is being processed
            PAINTSTRUCT ps;
            HDC hdc = ::BeginPaint(GetHwnd(), &ps);

            // Erase background before painting or we get white background
            MSWDefWindowProc(WM_ICONERASEBKGND, (WORD)(LONG)ps.hdc, 0L);

            if ( hIcon )
            {
                RECT rect;
                ::GetClientRect(GetHwnd(), &rect);

                // FIXME: why hardcoded?
                static const int icon_width = 32;
                static const int icon_height = 32;

                int icon_x = (int)((rect.right - icon_width)/2);
                int icon_y = (int)((rect.bottom - icon_height)/2);

                ::DrawIcon(hdc, icon_x, icon_y, hIcon);
            }

            ::EndPaint(GetHwnd(), &ps);

            return TRUE;
        }
        else
 #endif
        {
            return wxWindow::HandlePaint();
        }
    }
    else
    {
        // nothing to paint - processed
        return TRUE;
    }
}

bool wxFrame::HandleSize(int x, int y, WXUINT id)
{
    bool processed = FALSE;
#if !defined(__WXMICROWIN__) && !defined(__WXWINCE__)

    switch ( id )
    {
        case SIZENORMAL:
            // only do it it if we were iconized before, otherwise resizing the
            // parent frame has a curious side effect of bringing it under it's
            // children
            if ( !m_iconized )
                break;

            // restore all child frames too
            IconizeChildFrames(FALSE);

            (void)SendIconizeEvent(FALSE);

            // fall through

        case SIZEFULLSCREEN:
            m_iconized = FALSE;
            break;

        case SIZEICONIC:
            // iconize all child frames too
            IconizeChildFrames(TRUE);

            (void)SendIconizeEvent();

            m_iconized = TRUE;
            break;
    }
#endif

    if ( !m_iconized )
    {
#if wxUSE_STATUSBAR
        PositionStatusBar();
#endif // wxUSE_STATUSBAR

#if wxUSE_TOOLBAR
        PositionToolBar();
#endif // wxUSE_TOOLBAR

        processed = wxWindow::HandleSize(x, y, id);
    }

    return processed;
}

bool wxFrame::HandleCommand(WXWORD id, WXWORD cmd, WXHWND control)
{
    if ( control )
    {
        // In case it's e.g. a toolbar.
        wxWindow *win = wxFindWinFromHandle(control);
        if ( win )
            return win->MSWCommand(cmd, id);
    }

    // handle here commands from menus and accelerators
    if ( cmd == 0 || cmd == 1 )
    {
#if wxUSE_MENUS_NATIVE
        if ( wxCurrentPopupMenu )
        {
            wxMenu *popupMenu = wxCurrentPopupMenu;
            wxCurrentPopupMenu = NULL;

            return popupMenu->MSWCommand(cmd, id);
        }
#endif // wxUSE_MENUS_NATIVE

        if ( ProcessCommand(id) )
        {
            return TRUE;
        }
    }

    return FALSE;
}

bool wxFrame::HandleMenuSelect(WXWORD nItem, WXWORD flags, WXHMENU hMenu)
{
    int item;
    if ( flags == 0xFFFF && hMenu == 0 )
    {
        // menu was removed from screen
        item = -1;
    }
#ifndef __WXMICROWIN__
    else if ( !(flags & MF_POPUP) && !(flags & MF_SEPARATOR) )
    {
        item = nItem;
    }
#endif
    else
    {
        // don't give hints for separators (doesn't make sense) nor for the
        // items opening popup menus (they don't have them anyhow) but do clear
        // the status line - otherwise, we would be left with the help message
        // for the previous item which doesn't apply any more
        DoGiveHelp(wxEmptyString, FALSE);

        return FALSE;
    }

    wxMenuEvent event(wxEVT_MENU_HIGHLIGHT, item);
    event.SetEventObject(this);

    return GetEventHandler()->ProcessEvent(event);
}

bool wxFrame::HandleMenuLoop(const wxEventType& evtType, WXWORD isPopup)
{
    // we don't have the menu id here, so we use the id to specify if the event
    // was from a popup menu or a normal one
    wxMenuEvent event(evtType, isPopup ? -1 : 0);
    event.SetEventObject(this);

    return GetEventHandler()->ProcessEvent(event);
}

// ---------------------------------------------------------------------------
// the window proc for wxFrame
// ---------------------------------------------------------------------------

WXLRESULT wxFrame::MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam)
{
    WXLRESULT rc = 0;
    bool processed = FALSE;

    switch ( message )
    {
        case WM_CLOSE:
            // if we can't close, tell the system that we processed the
            // message - otherwise it would close us
            processed = !Close();
            break;

        case WM_SIZE:
            processed = HandleSize(LOWORD(lParam), HIWORD(lParam), wParam);
            break;

        case WM_COMMAND:
            {
                WORD id, cmd;
                WXHWND hwnd;
                UnpackCommand((WXWPARAM)wParam, (WXLPARAM)lParam,
                              &id, &hwnd, &cmd);

                processed = HandleCommand(id, cmd, (WXHWND)hwnd);
            }
            break;

        case WM_PAINT:
            processed = HandlePaint();
            break;

        case WM_INITMENUPOPUP:
            processed = HandleInitMenuPopup((WXHMENU) wParam);
            break;

#if !defined(__WXMICROWIN__) && !defined(__WXWINCE__)
        case WM_MENUSELECT:
            {
                WXWORD item, flags;
                WXHMENU hmenu;
                UnpackMenuSelect(wParam, lParam, &item, &flags, &hmenu);

                processed = HandleMenuSelect(item, flags, hmenu);
            }
            break;

        case WM_EXITMENULOOP:
            processed = HandleMenuLoop(wxEVT_MENU_CLOSE, wParam);
            break;

        case WM_QUERYDRAGICON:
            {
                const wxIcon& icon = GetIcon();
                HICON hIcon = icon.Ok() ? GetHiconOf(icon)
                                        : (HICON)GetDefaultIcon();
                rc = (long)hIcon;
                processed = rc != 0;
            }
            break;
#endif // !__WXMICROWIN__
    }

    if ( !processed )
        rc = wxFrameBase::MSWWindowProc(message, wParam, lParam);

    return rc;
}

// handle WM_INITMENUPOPUP message
bool wxFrame::HandleInitMenuPopup(WXHMENU hMenu)
{
    wxMenu* menu = NULL;
    if (GetMenuBar())
    {
        int nCount = GetMenuBar()->GetMenuCount();
        for (int n = 0; n < nCount; n++)
        {
            if (GetMenuBar()->GetMenu(n)->GetHMenu() == hMenu)
            {
                menu = GetMenuBar()->GetMenu(n);
                break;
            }
        }
    }
    
    wxMenuEvent event(wxEVT_MENU_OPEN, 0, menu);
    event.SetEventObject(this);

    return GetEventHandler()->ProcessEvent(event);
}
