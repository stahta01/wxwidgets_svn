/////////////////////////////////////////////////////////////////////////////
// Name:        msw/frame.cpp
// Purpose:     wxFrame
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#ifdef __GNUG__
    #pragma implementation "frame.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/setup.h"
    #include "wx/frame.h"
    #include "wx/menu.h"
    #include "wx/app.h"
    #include "wx/utils.h"
    #include "wx/dialog.h"
    #include "wx/settings.h"
    #include "wx/dcclient.h"
#endif // WX_PRECOMP

#include "wx/msw/private.h"

#if wxUSE_STATUSBAR
    #include "wx/statusbr.h"

    #if wxUSE_NATIVE_STATUSBAR
        #include "wx/msw/statbr95.h"
    #endif
#endif // wxUSE_STATUSBAR

#if wxUSE_TOOLBAR
    #include "wx/toolbar.h"
#endif // wxUSE_TOOLBAR

#include "wx/menuitem.h"
#include "wx/log.h"

// ----------------------------------------------------------------------------
// globals
// ----------------------------------------------------------------------------

extern wxWindowList wxModelessWindows;
extern wxList WXDLLEXPORT wxPendingDelete;
extern wxChar wxFrameClassName[];
extern wxMenu *wxCurrentPopupMenu;

// ----------------------------------------------------------------------------
// event tables
// ----------------------------------------------------------------------------

#if !USE_SHARED_LIBRARY
BEGIN_EVENT_TABLE(wxFrame, wxFrameBase)
    EVT_ACTIVATE(wxFrame::OnActivate)
    EVT_SYS_COLOUR_CHANGED(wxFrame::OnSysColourChanged)
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC_CLASS(wxFrame, wxWindow)
#endif

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// static class members
// ----------------------------------------------------------------------------

#if wxUSE_NATIVE_STATUSBAR
    bool wxFrame::m_useNativeStatusBar = TRUE;
#else
    bool wxFrame::m_useNativeStatusBar = FALSE;
#endif

// ----------------------------------------------------------------------------
// creation/destruction
// ----------------------------------------------------------------------------

void wxFrame::Init()
{
    m_iconized = FALSE;

#if wxUSE_TOOLTIPS
    m_hwndToolTip = 0;
#endif
}

bool wxFrame::Create(wxWindow *parent,
                     wxWindowID id,
                     const wxString& title,
                     const wxPoint& pos,
                     const wxSize& size,
                     long style,
                     const wxString& name)
{
  SetName(name);
  m_windowStyle = style;
  m_frameMenuBar = NULL;
  m_frameToolBar = NULL;
  m_frameStatusBar = NULL;

  SetBackgroundColour(wxSystemSettings::GetSystemColour(wxSYS_COLOUR_APPWORKSPACE));

  if ( id > -1 )
    m_windowId = id;
  else
    m_windowId = (int)NewControlId();

  if (parent) parent->AddChild(this);

  int x = pos.x;
  int y = pos.y;
  int width = size.x;
  int height = size.y;

  m_iconized = FALSE;

  // we pass NULL as parent to MSWCreate because frames with parents behave
  // very strangely under Win95 shell
  // Alteration by JACS: keep normal Windows behaviour (float on top of parent)
  // with this style.
  if ((m_windowStyle & wxFRAME_FLOAT_ON_PARENT) == 0)
    parent = NULL;

  if (!parent)
    wxTopLevelWindows.Append(this);

  MSWCreate(m_windowId, parent, wxFrameClassName, this, title,
            x, y, width, height, style);

  wxModelessWindows.Append(this);
  return TRUE;
}

wxFrame::~wxFrame()
{
  m_isBeingDeleted = TRUE;
  wxTopLevelWindows.DeleteObject(this);

  DeleteAllBars();

  if (wxTheApp && (wxTopLevelWindows.Number() == 0))
  {
    wxTheApp->SetTopWindow(NULL);

    if (wxTheApp->GetExitOnFrameDelete())
    {
       PostQuitMessage(0);
    }
  }

  wxModelessWindows.DeleteObject(this);

  // For some reason, wxWindows can activate another task altogether
  // when a frame is destroyed after a modal dialog has been invoked.
  // Try to bring the parent to the top.
  // MT:Only do this if this frame is currently the active window, else weird
  // things start to happen
  if ( wxGetActiveWindow() == this )
  if (GetParent() && GetParent()->GetHWND())
    ::BringWindowToTop((HWND) GetParent()->GetHWND());
}

// Get size *available for subwindows* i.e. excluding menu bar, toolbar etc.
void wxFrame::DoGetClientSize(int *x, int *y) const
{
  RECT rect;
  ::GetClientRect(GetHwnd(), &rect);

#if wxUSE_STATUSBAR
  if ( GetStatusBar() )
  {
    int statusX, statusY;
    GetStatusBar()->GetClientSize(&statusX, &statusY);
    rect.bottom -= statusY;
  }
#endif // wxUSE_STATUSBAR

  wxPoint pt(GetClientAreaOrigin());
  rect.bottom -= pt.y;
  rect.right -= pt.x;

  if ( x )
    *x = rect.right;
  if ( y )
    *y = rect.bottom;
}

// Set the client size (i.e. leave the calculation of borders etc.
// to wxWindows)
void wxFrame::DoSetClientSize(int width, int height)
{
  HWND hWnd = GetHwnd();

  RECT rect;
  ::GetClientRect(hWnd, &rect);

  RECT rect2;
  GetWindowRect(hWnd, &rect2);

  // Find the difference between the entire window (title bar and all)
  // and the client area; add this to the new client size to move the
  // window
  int actual_width = rect2.right - rect2.left - rect.right + width;
  int actual_height = rect2.bottom - rect2.top - rect.bottom + height;

#if wxUSE_STATUSBAR
  if ( GetStatusBar() )
  {
    int statusX, statusY;
    GetStatusBar()->GetClientSize(&statusX, &statusY);
    actual_height += statusY;
  }
#endif // wxUSE_STATUSBAR

  wxPoint pt(GetClientAreaOrigin());
  actual_width += pt.y;
  actual_height += pt.x;

  POINT point;
  point.x = rect2.left;
  point.y = rect2.top;

  MoveWindow(hWnd, point.x, point.y, actual_width, actual_height, (BOOL)TRUE);

  wxSizeEvent event(wxSize(width, height), m_windowId);
  event.SetEventObject( this );
  GetEventHandler()->ProcessEvent(event);
}

void wxFrame::DoGetSize(int *width, int *height) const
{
  RECT rect;
  GetWindowRect(GetHwnd(), &rect);
  *width = rect.right - rect.left;
  *height = rect.bottom - rect.top;
}

void wxFrame::DoGetPosition(int *x, int *y) const
{
  RECT rect;
  GetWindowRect(GetHwnd(), &rect);
  POINT point;
  point.x = rect.left;
  point.y = rect.top;

  *x = point.x;
  *y = point.y;
}

// ----------------------------------------------------------------------------
// variations around ::ShowWindow()
// ----------------------------------------------------------------------------

void wxFrame::DoShowWindow(int nShowCmd)
{
    ::ShowWindow(GetHwnd(), nShowCmd);

    m_iconized = nShowCmd == SW_MINIMIZE;
}

bool wxFrame::Show(bool show)
{
    DoShowWindow(show ? SW_SHOW : SW_HIDE);

    if ( show )
    {
        ::BringWindowToTop(GetHwnd());

        wxActivateEvent event(wxEVT_ACTIVATE, TRUE, m_windowId);
        event.SetEventObject( this );
        GetEventHandler()->ProcessEvent(event);
    }
    else
    {
        // Try to highlight the correct window (the parent)
        if ( GetParent() )
        {
            HWND hWndParent = GetHwndOf(GetParent());
            if (hWndParent)
                ::BringWindowToTop(hWndParent);
        }
    }

    return TRUE;
}

void wxFrame::Iconize(bool iconize)
{
    DoShowWindow(iconize ? SW_MINIMIZE : SW_RESTORE);
}

void wxFrame::Maximize(bool maximize)
{
    DoShowWindow(maximize ? SW_MAXIMIZE : SW_RESTORE);
}

void wxFrame::Restore()
{
    DoShowWindow(SW_RESTORE);
}

bool wxFrame::IsIconized() const
{
  ((wxFrame *)this)->m_iconized = (::IsIconic(GetHwnd()) != 0);
  return m_iconized;
}

// Is it maximized?
bool wxFrame::IsMaximized() const
{
    return (::IsZoomed(GetHwnd()) != 0);
}

void wxFrame::SetIcon(const wxIcon& icon)
{
    wxFrameBase::SetIcon(icon);

#if defined(__WIN95__)
    if ( m_icon.Ok() )
    {
        SendMessage(GetHwnd(), WM_SETICON,
                    (WPARAM)TRUE, (LPARAM)(HICON) m_icon.GetHICON());
    }
#endif // __WIN95__
}

#if wxUSE_STATUSBAR
wxStatusBar *wxFrame::OnCreateStatusBar(int number,
                                        long style,
                                        wxWindowID id,
                                        const wxString& name)
{
    wxStatusBar *statusBar = NULL;

#if wxUSE_NATIVE_STATUSBAR
    if ( UsesNativeStatusBar() )
    {
        statusBar = new wxStatusBar95(this, id, style);

        statusBar->SetFieldsCount(number);
    }
    else
#endif
    {
        statusBar = wxFrameBase::OnCreateStatusBar(number, style, id, name);
    }

    return statusBar;
}

void wxFrame::PositionStatusBar()
{
    // native status bar positions itself
    if ( m_frameStatusBar
#if wxUSE_NATIVE_STATUSBAR
         && !m_frameStatusBar->IsKindOf(CLASSINFO(wxStatusBar95))
#endif
       )
    {
        int w, h;
        GetClientSize(&w, &h);
        int sw, sh;
        m_frameStatusBar->GetSize(&sw, &sh);

        // Since we wish the status bar to be directly under the client area,
        // we use the adjusted sizes without using wxSIZE_NO_ADJUSTMENTS.
        m_frameStatusBar->SetSize(0, h, w, sh);
    }
}
#endif // wxUSE_STATUSBAR

void wxFrame::DetachMenuBar()
{
    if (m_frameMenuBar)
    {
        m_frameMenuBar->Detach();
        m_frameMenuBar = NULL;
    }
}

void wxFrame::SetMenuBar(wxMenuBar *menu_bar)
{
    if (!menu_bar)
    {
        DetachMenuBar();
        return;
    }

    m_frameMenuBar = NULL;

    // Can set a menubar several times.
    // TODO: how to prevent a memory leak if you have a currently-unattached
    // menubar? wxWindows assumes that the frame will delete the menu (otherwise
    // there are problems for MDI).
    if (menu_bar->GetHMenu())
    {
        m_hMenu = menu_bar->GetHMenu();
    }
    else
    {
        menu_bar->Detach();

        m_hMenu = menu_bar->Create();

        if ( !m_hMenu )
            return;
    }

    InternalSetMenuBar();

    m_frameMenuBar = menu_bar;
    menu_bar->Attach(this);

#if 0 // Old code that assumes only one call of SetMenuBar per frame.
    if (!menu_bar)
    {
        DetachMenuBar();
        return;
    }

    wxCHECK_RET( !menu_bar->GetFrame(), wxT("this menubar is already attached") );

    if (m_frameMenuBar)
        delete m_frameMenuBar;

    m_hMenu = menu_bar->Create();

    if ( !m_hMenu )
        return;

    InternalSetMenuBar();

    m_frameMenuBar = menu_bar;
    menu_bar->Attach(this);
#endif
}

void wxFrame::InternalSetMenuBar()
{
    if ( !::SetMenu(GetHwnd(), (HMENU)m_hMenu) )
    {
        wxLogLastError("SetMenu");
    }
}

// Responds to colour changes, and passes event on to children.
void wxFrame::OnSysColourChanged(wxSysColourChangedEvent& event)
{
    SetBackgroundColour(wxSystemSettings::GetSystemColour(wxSYS_COLOUR_APPWORKSPACE));
    Refresh();

    if ( m_frameStatusBar )
    {
        wxSysColourChangedEvent event2;
        event2.SetEventObject( m_frameStatusBar );
        m_frameStatusBar->GetEventHandler()->ProcessEvent(event2);
    }

    // Propagate the event to the non-top-level children
    wxWindow::OnSysColourChanged(event);
}

/*
 * Frame window
 *
 */

bool wxFrame::MSWCreate(int id, wxWindow *parent, const wxChar *wclass, wxWindow *wx_win, const wxChar *title,
                   int x, int y, int width, int height, long style)

{
  m_defaultIcon = (WXHICON) (wxSTD_FRAME_ICON ? wxSTD_FRAME_ICON : wxDEFAULT_FRAME_ICON);

  // If child windows aren't properly drawn initially, WS_CLIPCHILDREN
  // could be the culprit. But without it, you can get a lot of flicker.

  DWORD msflags = 0;
  if ((style & wxCAPTION) == wxCAPTION)
    msflags = WS_OVERLAPPED;
  else
    msflags = WS_POPUP;

  if (style & wxMINIMIZE_BOX)
    msflags |= WS_MINIMIZEBOX;
  if (style & wxMAXIMIZE_BOX)
    msflags |= WS_MAXIMIZEBOX;
  if (style & wxTHICK_FRAME)
    msflags |= WS_THICKFRAME;
  if (style & wxSYSTEM_MENU)
    msflags |= WS_SYSMENU;
  if ((style & wxMINIMIZE) || (style & wxICONIZE))
    msflags |= WS_MINIMIZE;
  if (style & wxMAXIMIZE)
    msflags |= WS_MAXIMIZE;
  if (style & wxCAPTION)
    msflags |= WS_CAPTION;
  if (style & wxCLIP_CHILDREN)
    msflags |= WS_CLIPCHILDREN;

  // Keep this in wxFrame because it saves recoding this function
  // in wxTinyFrame
#if wxUSE_ITSY_BITSY
  if (style & wxTINY_CAPTION_VERT)
    msflags |= IBS_VERTCAPTION;
  if (style & wxTINY_CAPTION_HORIZ)
    msflags |= IBS_HORZCAPTION;
#else
  if (style & wxTINY_CAPTION_VERT)
    msflags |= WS_CAPTION;
  if (style & wxTINY_CAPTION_HORIZ)
    msflags |= WS_CAPTION;
#endif
  if ((style & wxTHICK_FRAME) == 0)
    msflags |= WS_BORDER;

  WXDWORD extendedStyle = MakeExtendedStyle(style);

#if !defined(__WIN16__) && !defined(__SC__)
  if (style & wxFRAME_TOOL_WINDOW)
    extendedStyle |= WS_EX_TOOLWINDOW;
#endif

  if (style & wxSTAY_ON_TOP)
    extendedStyle |= WS_EX_TOPMOST;

  m_iconized = FALSE;
  if ( !wxWindow::MSWCreate(id, parent, wclass, wx_win, title, x, y, width, height,
         msflags, NULL, extendedStyle) )
         return FALSE;

  // Seems to be necessary if we use WS_POPUP
  // style instead of WS_OVERLAPPED
  if (width > -1 && height > -1)
    ::PostMessage(GetHwnd(), WM_SIZE, SIZE_RESTORED, MAKELPARAM(width, height));

  return TRUE;
}

// Default activation behaviour - set the focus for the first child
// subwindow found.
void wxFrame::OnActivate(wxActivateEvent& event)
{
    for ( wxWindowList::Node *node = GetChildren().GetFirst();
          node;
          node = node->GetNext() )
    {
        // FIXME all this is totally bogus - we need to do the same as wxPanel,
        //       but how to do it without duplicating the code?

        // restore focus
        wxWindow *child = node->GetData();

        if ( !child->IsTopLevel()
#if wxUSE_TOOLBAR
             && !wxDynamicCast(child, wxToolBar)
#endif // wxUSE_TOOLBAR
#if wxUSE_STATUSBAR
             && !wxDynamicCast(child, wxStatusBar)
#endif // wxUSE_STATUSBAR
           )
        {
            child->SetFocus();
            return;
        }
    }
}

// ----------------------------------------------------------------------------
// wxFrame size management: we exclude the areas taken by menu/status/toolbars
// from the client area, so the client area is what's really available for the
// frame contents
// ----------------------------------------------------------------------------

// Checks if there is a toolbar, and returns the first free client position
wxPoint wxFrame::GetClientAreaOrigin() const
{
    wxPoint pt(0, 0);
    if (GetToolBar())
    {
        int w, h;
        GetToolBar()->GetSize(& w, & h);

        if (GetToolBar()->GetWindowStyleFlag() & wxTB_VERTICAL)
        {
            pt.x += w;
        }
        else
        {
            pt.y += h;
        }
    }
    return pt;
}

void wxFrame::DoScreenToClient(int *x, int *y) const
{
    wxWindow::DoScreenToClient(x, y);

    // We may be faking the client origin.
    // So a window that's really at (0, 30) may appear
    // (to wxWin apps) to be at (0, 0).
    wxPoint pt(GetClientAreaOrigin());
    *x -= pt.x;
    *y -= pt.y;
}

void wxFrame::DoClientToScreen(int *x, int *y) const
{
    // We may be faking the client origin.
    // So a window that's really at (0, 30) may appear
    // (to wxWin apps) to be at (0, 0).
    wxPoint pt1(GetClientAreaOrigin());
    *x += pt1.x;
    *y += pt1.y;

    wxWindow::DoClientToScreen(x, y);
}

// ----------------------------------------------------------------------------
// tool/status bar stuff
// ----------------------------------------------------------------------------

#if wxUSE_TOOLBAR

wxToolBar* wxFrame::CreateToolBar(long style, wxWindowID id, const wxString& name)
{
    if ( wxFrameBase::CreateToolBar(style, id, name) )
    {
        PositionToolBar();
    }

    return m_frameToolBar;
}

void wxFrame::PositionToolBar()
{
    RECT rect;
    ::GetClientRect(GetHwnd(), &rect);

#if wxUSE_STATUSBAR
    if ( GetStatusBar() )
    {
        int statusX, statusY;
        GetStatusBar()->GetClientSize(&statusX, &statusY);
        rect.bottom -= statusY;
    }
#endif // wxUSE_STATUSBAR

    if ( GetToolBar() )
    {
        int tw, th;
        GetToolBar()->GetSize(&tw, &th);

        if ( GetToolBar()->GetWindowStyleFlag() & wxTB_VERTICAL )
        {
            th = rect.bottom;
        }
        else
        {
            tw = rect.right;
        }

        // Use the 'real' MSW position here
        GetToolBar()->SetSize(0, 0, tw, th, wxSIZE_NO_ADJUSTMENTS);
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
    for ( wxWindowList::Node *node = GetChildren().GetFirst();
          node;
          node = node->GetNext() )
    {
        wxWindow *win = node->GetData();

        if ( win->IsKindOf(CLASSINFO(wxFrame)) )
        {
            ((wxFrame *)win)->Iconize(bIconize);
        }
    }
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

    // try the menu bar accels
    wxMenuBar *menuBar = GetMenuBar();
    if ( !menuBar )
        return FALSE;

    const wxAcceleratorTable& acceleratorTable = menuBar->GetAccelTable();
    return acceleratorTable.Translate(this, pMsg);
}

// ---------------------------------------------------------------------------
// our private (non virtual) message handlers
// ---------------------------------------------------------------------------

bool wxFrame::HandlePaint()
{
    RECT rect;
    if ( GetUpdateRect(GetHwnd(), &rect, FALSE) )
    {
        if ( m_iconized )
        {
            HICON hIcon = m_icon.Ok() ? GetHiconOf(m_icon)
                                      : (HICON)m_defaultIcon;

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

            // fall through

        case SIZEFULLSCREEN:
            m_iconized = FALSE;
            break;

        case SIZEICONIC:
            // iconize all child frames too
            IconizeChildFrames(TRUE);

            m_iconized = TRUE;
            break;
    }

    if ( !m_iconized )
    {
        // forward WM_SIZE to status bar control
#if wxUSE_NATIVE_STATUSBAR
        if (m_frameStatusBar && m_frameStatusBar->IsKindOf(CLASSINFO(wxStatusBar95)))
        {
            wxSizeEvent event(wxSize(x, y), m_frameStatusBar->GetId());
            event.SetEventObject( m_frameStatusBar );

            ((wxStatusBar95 *)m_frameStatusBar)->OnSize(event);
        }
#endif // wxUSE_NATIVE_STATUSBAR

        PositionStatusBar();
        PositionToolBar();

        wxSizeEvent event(wxSize(x, y), m_windowId);
        event.SetEventObject( this );
        processed = GetEventHandler()->ProcessEvent(event);
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
        if ( wxCurrentPopupMenu )
        {
            wxMenu *popupMenu = wxCurrentPopupMenu;
            wxCurrentPopupMenu = NULL;

            return popupMenu->MSWCommand(cmd, id);
        }

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
    else if ( !(flags & MF_POPUP) && !(flags & MF_SEPARATOR) )
    {
        item = nItem;
    }
    else
    {
        // don't give hints for separators (doesn't make sense) nor for the
        // items opening popup menus (they don't have them anyhow)
        return FALSE;
    }

    wxMenuEvent event(wxEVT_MENU_HIGHLIGHT, item);
    event.SetEventObject( this );

    return GetEventHandler()->ProcessEvent(event);
}

// ---------------------------------------------------------------------------
// the window proc for wxFrame
// ---------------------------------------------------------------------------

long wxFrame::MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam)
{
    long rc = 0;
    bool processed = FALSE;

    switch ( message )
    {
        case WM_CLOSE:
            // if we can't close, tell the system that we processed the
            // message - otherwise it would close us
            processed = !Close();
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

        case WM_MENUSELECT:
            {
                WXWORD item, flags;
                WXHMENU hmenu;
                UnpackMenuSelect(wParam, lParam, &item, &flags, &hmenu);

                processed = HandleMenuSelect(item, flags, hmenu);
            }
            break;

        case WM_PAINT:
            processed = HandlePaint();
            break;

        case WM_QUERYDRAGICON:
            {
                HICON hIcon = m_icon.Ok() ? GetHiconOf(m_icon)
                                          : (HICON)(m_defaultIcon);
                rc = (long)hIcon;
                processed = rc != 0;
            }
            break;

        case WM_SIZE:
            processed = HandleSize(LOWORD(lParam), HIWORD(lParam), wParam);
            break;
    }

    if ( !processed )
        rc = wxWindow::MSWWindowProc(message, wParam, lParam);

    return rc;
}

