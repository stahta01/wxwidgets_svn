/////////////////////////////////////////////////////////////////////////////
// Name:        src/msw/windows.cpp
// Purpose:     wxWindow
// Author:      Julian Smart
// Modified by: VZ on 13.05.99: no more Default(), MSWOnXXX() reorganisation
// Created:     04/01/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

// ===========================================================================
// declarations
// ===========================================================================

// ---------------------------------------------------------------------------
// headers
// ---------------------------------------------------------------------------

#ifdef __GNUG__
    #pragma implementation "window.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <windows.h>
    #include "wx/msw/winundef.h"
    #include "wx/window.h"
    #include "wx/accel.h"
    #include "wx/setup.h"
    #include "wx/menu.h"
    #include "wx/dc.h"
    #include "wx/dcclient.h"
    #include "wx/utils.h"
    #include "wx/app.h"
    #include "wx/panel.h"
    #include "wx/layout.h"
    #include "wx/dialog.h"
    #include "wx/frame.h"
    #include "wx/listbox.h"
    #include "wx/button.h"
    #include "wx/msgdlg.h"
    #include "wx/settings.h"
    #include "wx/statbox.h"
#endif

#if wxUSE_OWNER_DRAWN
    #include "wx/ownerdrw.h"
#endif

#if wxUSE_DRAG_AND_DROP
    #include "wx/dnd.h"
#endif

#include "wx/menuitem.h"
#include "wx/log.h"

#include "wx/msw/private.h"

#if wxUSE_TOOLTIPS
    #include "wx/tooltip.h"
#endif

#if wxUSE_CARET
    #include "wx/caret.h"
#endif // wxUSE_CARET

#if wxUSE_SPINCTRL
    #include "wx/spinctrl.h"
#endif // wxUSE_SPINCTRL

#include "wx/intl.h"
#include "wx/log.h"

#include "wx/textctrl.h"
#include "wx/notebook.h"

#include <string.h>

#if (!defined(__GNUWIN32_OLD__) && !defined(__WXMICROWIN__)) || defined(__CYGWIN10__)
    #include <shellapi.h>
    #include <mmsystem.h>
#endif

#ifdef __WIN32__
    #include <windowsx.h>
#endif

#if (!defined(__GNUWIN32_OLD__) && !defined(__TWIN32__) && !defined(__WXMICROWIN__)) || defined(__CYGWIN10__)
    #ifdef __WIN95__
        #include <commctrl.h>
    #endif
#elif !defined(__WXMICROWIN__) // broken compiler
    #ifndef __TWIN32__
        #include "wx/msw/gnuwin32/extra.h"
    #endif
#endif

// This didn't appear in mingw until 2.95.2
#ifndef SIF_TRACKPOS
#define SIF_TRACKPOS 16
#endif

#if wxUSE_MOUSEWHEEL
    #ifndef WM_MOUSEWHEEL
        #define WM_MOUSEWHEEL           0x020A
    #endif
    #ifndef WHEEL_DELTA
        #define WHEEL_DELTA             120
    #endif
    #ifndef SPI_GETWHEELSCROLLLINES
        #define SPI_GETWHEELSCROLLLINES 104
    #endif
#endif


// ---------------------------------------------------------------------------
// global variables
// ---------------------------------------------------------------------------

// the last Windows message we got (MT-UNSAFE)
extern MSG s_currentMsg;

#if wxUSE_MENUS_NATIVE
wxMenu *wxCurrentPopupMenu = NULL;
#endif // wxUSE_MENUS_NATIVE

extern const wxChar *wxCanvasClassName;

// ---------------------------------------------------------------------------
// private functions
// ---------------------------------------------------------------------------

// the window proc for all our windows
LRESULT WXDLLEXPORT APIENTRY _EXPORT wxWndProc(HWND hWnd, UINT message,
                                   WPARAM wParam, LPARAM lParam);

#ifdef  __WXDEBUG__
    const char *wxGetMessageName(int message);
#endif  //__WXDEBUG__

void wxRemoveHandleAssociation(wxWindowMSW *win);
void wxAssociateWinWithHandle(HWND hWnd, wxWindowMSW *win);
wxWindow *wxFindWinFromHandle(WXHWND hWnd);

// this magical function is used to translate VK_APPS key presses to right
// mouse clicks
static void TranslateKbdEventToMouse(wxWindowMSW *win,
                                     int *x, int *y, WPARAM *flags);

// get the text metrics for the current font
static TEXTMETRIC wxGetTextMetrics(const wxWindowMSW *win);

// check if the mouse is in the window or its child
//static bool IsMouseInWindow(HWND hwnd);

// ---------------------------------------------------------------------------
// event tables
// ---------------------------------------------------------------------------

// in wxUniv/MSW this class is abstract because it doesn't have DoPopupMenu()
// method
#ifdef __WXUNIVERSAL__
    IMPLEMENT_ABSTRACT_CLASS(wxWindowMSW, wxWindowBase)
#else // __WXMSW__
    IMPLEMENT_DYNAMIC_CLASS(wxWindow, wxWindowBase)
#endif // __WXUNIVERSAL__/__WXMSW__

BEGIN_EVENT_TABLE(wxWindowMSW, wxWindowBase)
    EVT_ERASE_BACKGROUND(wxWindowMSW::OnEraseBackground)
    EVT_SYS_COLOUR_CHANGED(wxWindowMSW::OnSysColourChanged)
    EVT_INIT_DIALOG(wxWindowMSW::OnInitDialog)
    EVT_IDLE(wxWindowMSW::OnIdle)
    EVT_SET_FOCUS(wxWindowMSW::OnSetFocus)
END_EVENT_TABLE()

// ===========================================================================
// implementation
// ===========================================================================

// ---------------------------------------------------------------------------
// wxWindow utility functions
// ---------------------------------------------------------------------------

// Find an item given the MS Windows id
wxWindow *wxWindowMSW::FindItem(long id) const
{
#if wxUSE_CONTROLS
    wxControl *item = wxDynamicCastThis(wxControl);
    if ( item )
    {
        // is it we or one of our "internal" children?
        if ( item->GetId() == id
#ifndef __WXUNIVERSAL__
                || (item->GetSubcontrols().Index(id) != wxNOT_FOUND)
#endif // __WXUNIVERSAL__
           )
        {
            return item;
        }
    }
#endif // wxUSE_CONTROLS

    wxWindowList::Node *current = GetChildren().GetFirst();
    while (current)
    {
        wxWindow *childWin = current->GetData();

        wxWindow *wnd = childWin->FindItem(id);
        if ( wnd )
            return wnd;

        current = current->GetNext();
    }

    return NULL;
}

// Find an item given the MS Windows handle
wxWindow *wxWindowMSW::FindItemByHWND(WXHWND hWnd, bool controlOnly) const
{
    wxWindowList::Node *current = GetChildren().GetFirst();
    while (current)
    {
        wxWindow *parent = current->GetData();

        // Do a recursive search.
        wxWindow *wnd = parent->FindItemByHWND(hWnd);
        if ( wnd )
            return wnd;

        if ( !controlOnly
#if wxUSE_CONTROLS
                || parent->IsKindOf(CLASSINFO(wxControl))
#endif // wxUSE_CONTROLS
           )
        {
            wxWindow *item = current->GetData();
            if ( item->GetHWND() == hWnd )
                return item;
            else
            {
                if ( item->ContainsHWND(hWnd) )
                    return item;
            }
        }

        current = current->GetNext();
    }
    return NULL;
}

// Default command handler
bool wxWindowMSW::MSWCommand(WXUINT WXUNUSED(param), WXWORD WXUNUSED(id))
{
    return FALSE;
}

// ----------------------------------------------------------------------------
// constructors and such
// ----------------------------------------------------------------------------

void wxWindowMSW::Init()
{
    // generic
    InitBase();

    // MSW specific
    m_doubleClickAllowed = 0;

    m_isBeingDeleted = FALSE;
    m_oldWndProc = 0;
    m_useCtl3D = FALSE;
    m_mouseInWindow = FALSE;

    // wxWnd
    m_hMenu = 0;

    m_hWnd = 0;

    // pass WM_GETDLGCODE to DefWindowProc()
    m_lDlgCode = 0;

    m_xThumbSize = 0;
    m_yThumbSize = 0;
    m_backgroundTransparent = FALSE;

    // as all windows are created with WS_VISIBLE style...
    m_isShown = TRUE;

#if wxUSE_MOUSEEVENT_HACK
    m_lastMouseX =
    m_lastMouseY = -1;
    m_lastMouseEvent = -1;
#endif // wxUSE_MOUSEEVENT_HACK
}

// Destructor
wxWindowMSW::~wxWindowMSW()
{
    m_isBeingDeleted = TRUE;

    MSWDetachWindowMenu();

    // VS: make sure there's no wxFrame with last focus set to us:
    for (wxWindow *win = GetParent(); win; win = win->GetParent())
    {
        wxFrame *frame = wxDynamicCast(win, wxFrame);
        if ( frame )
        {
            if ( frame->GetLastFocus() == this )
                frame->SetLastFocus((wxWindow*)NULL);
            break;
        }
    }

    // VS: destroy children first and _then_ detach *this from its parent.
    //     If we'd do it the other way around, children wouldn't be able
    //     find their parent frame (see above).
    DestroyChildren();

    if ( m_parent )
        m_parent->RemoveChild(this);

    if ( m_hWnd )
    {
        // VZ: test temp removed to understand what really happens here
        //if (::IsWindow(GetHwnd()))
        {
            if ( !::DestroyWindow(GetHwnd()) )
                wxLogLastError(wxT("DestroyWindow"));
        }

        // remove hWnd <-> wxWindow association
        wxRemoveHandleAssociation(this);
    }
}

// real construction (Init() must have been called before!)
bool wxWindowMSW::Create(wxWindow *parent,
                         wxWindowID id,
                         const wxPoint& pos,
                         const wxSize& size,
                         long style,
                         const wxString& name)
{
    wxCHECK_MSG( parent, FALSE, wxT("can't create wxWindow without parent") );

#if wxUSE_STATBOX
    // wxGTK doesn't allow to create controls with static box as the parent so
    // this will result in a crash when the program is ported to wxGTK - warn
    // about it
    //
    // the correct solution is to create the controls as siblings of the
    // static box
    wxASSERT_MSG( !wxDynamicCastThis(wxStaticBox),
                  _T("wxStaticBox can't be used as a window parent!") );
#endif // wxUSE_STATBOX

    if ( !CreateBase(parent, id, pos, size, style, wxDefaultValidator, name) )
        return FALSE;

    parent->AddChild(this);

    // all windows are created visible
    DWORD msflags = WS_CHILD | WS_VISIBLE;

#ifdef __WXUNIVERSAL__
    // no 3d effects, we draw them ourselves
    WXDWORD exStyle = 0;
#else // !wxUniversal
    if ( style & wxCLIP_CHILDREN )
        msflags |= WS_CLIPCHILDREN;
    if ( style & wxCLIP_SIBLINGS )
        msflags |= WS_CLIPSIBLINGS;

    bool want3D;
    WXDWORD exStyle = Determine3DEffects(WS_EX_CLIENTEDGE, &want3D);

    // Even with extended styles, need to combine with WS_BORDER
    // for them to look right.
    if ( want3D ||
        (m_windowStyle & (wxBORDER |
                          wxSIMPLE_BORDER |
                          wxRAISED_BORDER |
                          wxSUNKEN_BORDER |
                          wxDOUBLE_BORDER)) )
    {
        msflags |= WS_BORDER;
    }

    // calculate the value to return from WM_GETDLGCODE handler
    if ( GetWindowStyleFlag() & wxWANTS_CHARS )
    {
        // want everything: i.e. all keys and WM_CHAR message
        m_lDlgCode = DLGC_WANTARROWS | DLGC_WANTCHARS |
                     DLGC_WANTTAB | DLGC_WANTMESSAGE;
    }
#endif // wxUniversal/!wxUniversal

    if ( style & wxPOPUP_WINDOW )
    {
        // a popup window floats on top of everything
        exStyle |= WS_EX_TOPMOST | WS_EX_TOOLWINDOW;

        // it is also created hidden as other top level windows
        msflags &= ~WS_VISIBLE;
        m_isShown = FALSE;
    }

    return MSWCreate(m_windowId, parent, wxCanvasClassName,
                     (wxWindow *)this, NULL,
                     pos.x, pos.y,
                     WidthDefault(size.x), HeightDefault(size.y),
                     msflags, NULL, exStyle);
}

// ---------------------------------------------------------------------------
// basic operations
// ---------------------------------------------------------------------------

void wxWindowMSW::SetFocus()
{
    HWND hWnd = GetHwnd();
    wxCHECK_RET( hWnd, _T("can't set focus to invalid window") );

#ifndef __WXMICROWIN__
    ::SetLastError(0);
#endif

    if ( !::SetFocus(hWnd) )
    {
        // was there really an error?
#ifndef __WXMICROWIN__
        DWORD dwRes = ::GetLastError();
#else

        DWORD dwRes = 0;
#endif
        if ( dwRes )
        {
            wxLogApiError(_T("SetFocus"), dwRes);
        }

        // VZ: just why does this happen sometimes?? any idea?
#if 0
        HWND hwndFocus = ::GetFocus();
        wxASSERT_MSG( hwndFocus == hWnd, _T("SetFocus() didn't work?") );
#endif // 0
    }
}

// Get the window with the focus
wxWindow *wxWindowBase::FindFocus()
{
    HWND hWnd = ::GetFocus();
    if ( hWnd )
    {
        return wxGetWindowFromHWND((WXHWND)hWnd);
    }

    return NULL;
}

bool wxWindowMSW::Enable(bool enable)
{
    if ( !wxWindowBase::Enable(enable) )
        return FALSE;

    HWND hWnd = GetHwnd();
    if ( hWnd )
        ::EnableWindow(hWnd, (BOOL)enable);

    // VZ: no, this is a bad idea: imagine that you have a dialog with some
    //     disabled controls and disable it - you really wouldn't like the
    //     disabled controls be reenabled too when you reenable the dialog!
#if 0
    wxWindowList::Node *node = GetChildren().GetFirst();
    while ( node )
    {
        wxWindow *child = node->GetData();
        child->Enable(enable);

        node = node->GetNext();
    }
#endif // 0

    return TRUE;
}

bool wxWindowMSW::Show(bool show)
{
    if ( !wxWindowBase::Show(show) )
        return FALSE;

    HWND hWnd = GetHwnd();
    int cshow = show ? SW_SHOW : SW_HIDE;
    ::ShowWindow(hWnd, cshow);

    if ( show )
    {
#ifdef __WXMICROWIN__
        // It seems that MicroWindows brings the _parent_ of the
        // window to the top, which can be the wrong one.

        // activate (set focus to) specified window
        ::SetFocus(hWnd);

        // raise top level parent to top of z order
        ::SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0,
                SWP_NOMOVE|SWP_NOSIZE);
#else
        BringWindowToTop(hWnd);
#endif
    }

    return TRUE;
}

// Raise the window to the top of the Z order
void wxWindowMSW::Raise()
{
#ifdef __WIN16__
    ::BringWindowToTop(GetHwnd());
#else // Win32
#ifdef __WXMICROWIN__
    // It seems that MicroWindows brings the _parent_ of the
    // window to the top, which can be the wrong one.

    // activate (set focus to) specified window
    ::SetFocus(GetHwnd());

    // raise top level parent to top of z order
    ::SetWindowPos(GetHwnd(), HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
#else
    ::SetForegroundWindow(GetHwnd());
#endif
#endif
}

// Lower the window to the bottom of the Z order
void wxWindowMSW::Lower()
{
    ::SetWindowPos(GetHwnd(), HWND_BOTTOM, 0, 0, 0, 0,
                   SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
}

void wxWindowMSW::SetTitle( const wxString& title)
{
    SetWindowText(GetHwnd(), title.c_str());
}

wxString wxWindowMSW::GetTitle() const
{
    return wxGetWindowText(GetHWND());
}

void wxWindowMSW::CaptureMouse()
{
    HWND hWnd = GetHwnd();
    if ( hWnd )
    {
        ::SetCapture(hWnd);
    }
}

void wxWindowMSW::ReleaseMouse()
{
    if ( !::ReleaseCapture() )
    {
        wxLogLastError(_T("ReleaseCapture"));
    }
}

/* static */ wxWindow *wxWindowBase::GetCapture()
{
    HWND hwnd = ::GetCapture();
    return hwnd ? wxFindWinFromHandle((WXHWND)hwnd) : (wxWindow *)NULL;
}

bool wxWindowMSW::SetFont(const wxFont& font)
{
    if ( !wxWindowBase::SetFont(font) )
    {
        // nothing to do
        return FALSE;
    }

    HWND hWnd = GetHwnd();
    if ( hWnd != 0 )
    {
        WXHANDLE hFont = m_font.GetResourceHandle();

        wxASSERT_MSG( hFont, wxT("should have valid font") );

        ::SendMessage(hWnd, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
    }

    return TRUE;
}
bool wxWindowMSW::SetCursor(const wxCursor& cursor)
{
    if ( !wxWindowBase::SetCursor(cursor) )
    {
        // no change
        return FALSE;
    }

    if ( m_cursor.Ok() )
    {
        HWND hWnd = GetHwnd();

        // Change the cursor NOW if we're within the correct window
        POINT point;
        ::GetCursorPos(&point);

        RECT rect;
        ::GetWindowRect(hWnd, &rect);

        if ( ::PtInRect(&rect, point) && !wxIsBusy() )
            ::SetCursor(GetHcursorOf(m_cursor));
    }

    return TRUE;
}

void wxWindowMSW::WarpPointer (int x, int y)
{
    ClientToScreen(&x, &y);

    if ( !::SetCursorPos(x, y) )
    {
        wxLogLastError(_T("SetCursorPos"));
    }
}

#if WXWIN_COMPATIBILITY
void wxWindowMSW::MSWDeviceToLogical (float *x, float *y) const
{
}
#endif // WXWIN_COMPATIBILITY

// ---------------------------------------------------------------------------
// scrolling stuff
// ---------------------------------------------------------------------------

#if WXWIN_COMPATIBILITY
void wxWindowMSW::SetScrollRange(int orient, int range, bool refresh)
{
#if defined(__WIN95__)

    int range1 = range;

    // Try to adjust the range to cope with page size > 1
    // - a Windows API quirk
    int pageSize = GetScrollPage(orient);
    if ( pageSize > 1 && range > 0)
    {
        range1 += (pageSize - 1);
    }

    SCROLLINFO info;
    int dir;

    if ( orient == wxHORIZONTAL ) {
        dir = SB_HORZ;
    } else {
        dir = SB_VERT;
    }

    info.cbSize = sizeof(SCROLLINFO);
    info.nPage = pageSize; // Have to set this, or scrollbar goes awry
    info.nMin = 0;
    info.nMax = range1;
    info.nPos = 0;
    info.fMask = SIF_RANGE | SIF_PAGE;

    HWND hWnd = GetHwnd();
    if ( hWnd )
        ::SetScrollInfo(hWnd, dir, &info, refresh);
#else
    int wOrient;
    if ( orient == wxHORIZONTAL )
        wOrient = SB_HORZ;
    else
        wOrient = SB_VERT;

    HWND hWnd = GetHwnd();
    if ( hWnd )
        ::SetScrollRange(hWnd, wOrient, 0, range, refresh);
#endif
}

void wxWindowMSW::SetScrollPage(int orient, int page, bool refresh)
{
#if defined(__WIN95__)
    SCROLLINFO info;
    int dir;

    if ( orient == wxHORIZONTAL ) {
        dir = SB_HORZ;
        m_xThumbSize = page;
    } else {
        dir = SB_VERT;
        m_yThumbSize = page;
    }

    info.cbSize = sizeof(SCROLLINFO);
    info.nPage = page;
    info.nMin = 0;
    info.fMask = SIF_PAGE;

    HWND hWnd = GetHwnd();
    if ( hWnd )
        ::SetScrollInfo(hWnd, dir, &info, refresh);
#else
    if ( orient == wxHORIZONTAL )
        m_xThumbSize = page;
    else
        m_yThumbSize = page;
#endif
}

int wxWindowMSW::OldGetScrollRange(int orient) const
{
    int wOrient;
    if ( orient == wxHORIZONTAL )
        wOrient = SB_HORZ;
    else
        wOrient = SB_VERT;

#if __WATCOMC__ && defined(__WINDOWS_386__)
    short minPos, maxPos;
#else
    int minPos, maxPos;
#endif
    HWND hWnd = GetHwnd();
    if ( hWnd )
    {
        ::GetScrollRange(hWnd, wOrient, &minPos, &maxPos);
#if defined(__WIN95__)
        // Try to adjust the range to cope with page size > 1
        // - a Windows API quirk
        int pageSize = GetScrollPage(orient);
        if ( pageSize > 1 )
        {
            maxPos -= (pageSize - 1);
        }
#endif
        return maxPos;
    }
    else
        return 0;
}

int wxWindowMSW::GetScrollPage(int orient) const
{
    if ( orient == wxHORIZONTAL )
        return m_xThumbSize;
    else
        return m_yThumbSize;
}

#endif // WXWIN_COMPATIBILITY

inline int GetScrollPosition(HWND hWnd, int wOrient)
{
#ifdef __WXMICROWIN__
    return ::GetScrollPosWX(hWnd, wOrient);
#else
    return ::GetScrollPos(hWnd, wOrient);
#endif
}

int wxWindowMSW::GetScrollPos(int orient) const
{
    int wOrient;
    if ( orient == wxHORIZONTAL )
        wOrient = SB_HORZ;
    else
        wOrient = SB_VERT;

    HWND hWnd = GetHwnd();
    wxCHECK_MSG( hWnd, 0, _T("no HWND in GetScrollPos") );

    return GetScrollPosition(hWnd, wOrient);
}

// This now returns the whole range, not just the number
// of positions that we can scroll.
int wxWindowMSW::GetScrollRange(int orient) const
{
    int wOrient;
    if ( orient == wxHORIZONTAL )
        wOrient = SB_HORZ;
    else
        wOrient = SB_VERT;

#if __WATCOMC__ && defined(__WINDOWS_386__)
    short minPos, maxPos;
#else
    int minPos, maxPos;
#endif
    HWND hWnd = GetHwnd();
    if ( hWnd )
    {
        ::GetScrollRange(hWnd, wOrient, &minPos, &maxPos);
#if defined(__WIN95__)
        // Try to adjust the range to cope with page size > 1
        // - a Windows API quirk
        int pageSize = GetScrollThumb(orient);
        if ( pageSize > 1 )
        {
            maxPos -= (pageSize - 1);
        }
        // October 10th: new range concept.
        maxPos += pageSize;
#endif

        return maxPos;
    }
    else
        return 0;
}

int wxWindowMSW::GetScrollThumb(int orient) const
{
    if ( orient == wxHORIZONTAL )
        return m_xThumbSize;
    else
        return m_yThumbSize;
}

void wxWindowMSW::SetScrollPos(int orient, int pos, bool refresh)
{
#if defined(__WIN95__)
    SCROLLINFO info;
    int dir;

    if ( orient == wxHORIZONTAL ) {
        dir = SB_HORZ;
    } else {
        dir = SB_VERT;
    }

    info.cbSize = sizeof(SCROLLINFO);
    info.nPage = 0;
    info.nMin = 0;
    info.nPos = pos;
    info.fMask = SIF_POS;

    HWND hWnd = GetHwnd();
    if ( hWnd )
        ::SetScrollInfo(hWnd, dir, &info, refresh);
#else
    int wOrient;
    if ( orient == wxHORIZONTAL )
        wOrient = SB_HORZ;
    else
        wOrient = SB_VERT;

    HWND hWnd = GetHwnd();
    if ( hWnd )
        ::SetScrollPos(hWnd, wOrient, pos, refresh);
#endif
}

// New function that will replace some of the above.
void wxWindowMSW::SetScrollbar(int orient, int pos, int thumbVisible,
                            int range, bool refresh)
{
#if defined(__WIN95__)
    int oldRange = range - thumbVisible;

    int range1 = oldRange;

    // Try to adjust the range to cope with page size > 1
    // - a Windows API quirk
    int pageSize = thumbVisible;
    if ( pageSize > 1 && range > 0)
    {
        range1 += (pageSize - 1);
    }

    SCROLLINFO info;
    int dir;

    if ( orient == wxHORIZONTAL ) {
        dir = SB_HORZ;
    } else {
        dir = SB_VERT;
    }

    info.cbSize = sizeof(SCROLLINFO);
    info.nPage = pageSize; // Have to set this, or scrollbar goes awry
    info.nMin = 0;
    info.nMax = range1;
    info.nPos = pos;
    info.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;

    HWND hWnd = GetHwnd();
    if ( hWnd )
        ::SetScrollInfo(hWnd, dir, &info, refresh);
#else
    int wOrient;
    if ( orient == wxHORIZONTAL )
        wOrient = SB_HORZ;
    else
        wOrient = SB_VERT;

    HWND hWnd = GetHwnd();
    if ( hWnd )
    {
        ::SetScrollRange(hWnd, wOrient, 0, range, FALSE);
        ::SetScrollPos(hWnd, wOrient, pos, refresh);
    }
#endif
    if ( orient == wxHORIZONTAL ) {
        m_xThumbSize = thumbVisible;
    } else {
        m_yThumbSize = thumbVisible;
    }
}

void wxWindowMSW::ScrollWindow(int dx, int dy, const wxRect *prect)
{
    RECT rect;
    if ( prect )
    {
        rect.left = prect->x;
        rect.top = prect->y;
        rect.right = prect->x + prect->width;
        rect.bottom = prect->y + prect->height;
    }

    ::ScrollWindow(GetHwnd(), dx, dy, prect ? &rect : NULL, NULL);
}

static bool ScrollVertically(HWND hwnd, int kind, int count)
{
    int posStart = GetScrollPosition(hwnd, SB_VERT);

    int pos = posStart;
    for ( int n = 0; n < count; n++ )
    {
        ::SendMessage(hwnd, WM_VSCROLL, kind, 0);

        int posNew = GetScrollPosition(hwnd, SB_VERT);
        if ( posNew == pos )
        {
            // don't bother to continue, we're already at top/bottom
            break;
        }

        pos = posNew;
    }

    return pos != posStart;
}

bool wxWindowMSW::ScrollLines(int lines)
{
    bool down = lines > 0;

    return ScrollVertically(GetHwnd(),
                            down ? SB_LINEDOWN : SB_LINEUP,
                            down ? lines : -lines);
}

bool wxWindowMSW::ScrollPages(int pages)
{
    bool down = pages > 0;

    return ScrollVertically(GetHwnd(),
                            down ? SB_PAGEDOWN : SB_PAGEUP,
                            down ? pages : -pages);
}

// ---------------------------------------------------------------------------
// subclassing
// ---------------------------------------------------------------------------

void wxWindowMSW::SubclassWin(WXHWND hWnd)
{
    wxASSERT_MSG( !m_oldWndProc, wxT("subclassing window twice?") );

    HWND hwnd = (HWND)hWnd;
    wxCHECK_RET( ::IsWindow(hwnd), wxT("invalid HWND in SubclassWin") );

    wxAssociateWinWithHandle(hwnd, this);

    m_oldWndProc = (WXFARPROC) GetWindowLong(hwnd, GWL_WNDPROC);

    wxASSERT( (WXFARPROC) m_oldWndProc != (WXFARPROC) wxWndProc );

    SetWindowLong(hwnd, GWL_WNDPROC, (LONG) wxWndProc);
}

void wxWindowMSW::UnsubclassWin()
{
    wxRemoveHandleAssociation(this);

    // Restore old Window proc
    HWND hwnd = GetHwnd();
    if ( hwnd )
    {
        m_hWnd = 0;

        wxCHECK_RET( ::IsWindow(hwnd), wxT("invalid HWND in UnsubclassWin") );

        FARPROC farProc = (FARPROC) GetWindowLong(hwnd, GWL_WNDPROC);
        if ( (m_oldWndProc != 0) && (farProc != (FARPROC) m_oldWndProc) )
        {
            SetWindowLong(hwnd, GWL_WNDPROC, (LONG) m_oldWndProc);
            m_oldWndProc = 0;
        }
    }
}

// Make a Windows extended style from the given wxWindows window style
WXDWORD wxWindowMSW::MakeExtendedStyle(long style, bool eliminateBorders)
{
    WXDWORD exStyle = 0;
    if ( style & wxTRANSPARENT_WINDOW )
        exStyle |= WS_EX_TRANSPARENT;

    if ( !eliminateBorders )
    {
        if ( style & wxSUNKEN_BORDER )
            exStyle |= WS_EX_CLIENTEDGE;
        if ( style & wxDOUBLE_BORDER )
            exStyle |= WS_EX_DLGMODALFRAME;
#if defined(__WIN95__)
        if ( style & wxRAISED_BORDER )
            // It seems that WS_EX_WINDOWEDGE doesn't work, but WS_EX_DLGMODALFRAME does
            exStyle |= WS_EX_DLGMODALFRAME; /* WS_EX_WINDOWEDGE */;
        if ( style & wxSTATIC_BORDER )
            exStyle |= WS_EX_STATICEDGE;
#endif
    }
    return exStyle;
}

// Determines whether native 3D effects or CTL3D should be used,
// applying a default border style if required, and returning an extended
// style to pass to CreateWindowEx.
WXDWORD wxWindowMSW::Determine3DEffects(WXDWORD defaultBorderStyle,
                                     bool *want3D) const
{
    // If matches certain criteria, then assume no 3D effects
    // unless specifically requested (dealt with in MakeExtendedStyle)
    if ( !GetParent()
#if wxUSE_CONTROLS
            || !IsKindOf(CLASSINFO(wxControl))
#endif // wxUSE_CONTROLS
            || (m_windowStyle & wxNO_BORDER) )
    {
        *want3D = FALSE;
        return MakeExtendedStyle(m_windowStyle, FALSE);
    }

    // Determine whether we should be using 3D effects or not.
    bool nativeBorder = FALSE; // by default, we don't want a Win95 effect

    // 1) App can specify global 3D effects
    *want3D = wxTheApp->GetAuto3D();

    // 2) If the parent is being drawn with user colours, or simple border specified,
    // switch effects off. TODO: replace wxUSER_COLOURS with wxNO_3D
    if ( GetParent() && (GetParent()->GetWindowStyleFlag() & wxUSER_COLOURS) || (m_windowStyle & wxSIMPLE_BORDER) )
        *want3D = FALSE;

    // 3) Control can override this global setting by defining
    // a border style, e.g. wxSUNKEN_BORDER
    if ( m_windowStyle & wxSUNKEN_BORDER  )
        *want3D = TRUE;

    // 4) If it's a special border, CTL3D can't cope so we want a native border
    if ( (m_windowStyle & wxDOUBLE_BORDER) || (m_windowStyle & wxRAISED_BORDER) ||
        (m_windowStyle & wxSTATIC_BORDER) )
    {
        *want3D = TRUE;
        nativeBorder = TRUE;
    }

    // 5) If this isn't a Win95 app, and we are using CTL3D, remove border
    // effects from extended style
#if wxUSE_CTL3D
    if ( *want3D )
        nativeBorder = FALSE;
#endif

    DWORD exStyle = MakeExtendedStyle(m_windowStyle, !nativeBorder);

    // If we want 3D, but haven't specified a border here,
    // apply the default border style specified.
    // TODO what about non-Win95 WIN32? Does it have borders?
#if defined(__WIN95__) && !wxUSE_CTL3D
    if ( defaultBorderStyle && (*want3D) && ! ((m_windowStyle & wxDOUBLE_BORDER) || (m_windowStyle & wxRAISED_BORDER ) ||
        (m_windowStyle & wxSTATIC_BORDER) || (m_windowStyle & wxSIMPLE_BORDER) ))
        exStyle |= defaultBorderStyle; // WS_EX_CLIENTEDGE;
#endif

    return exStyle;
}

#if WXWIN_COMPATIBILITY
// If nothing defined for this, try the parent.
// E.g. we may be a button loaded from a resource, with no callback function
// defined.
void wxWindowMSW::OnCommand(wxWindow& win, wxCommandEvent& event)
{
    if ( GetEventHandler()->ProcessEvent(event)  )
        return;
    if ( m_parent )
        m_parent->GetEventHandler()->OnCommand(win, event);
}
#endif // WXWIN_COMPATIBILITY_2

#if WXWIN_COMPATIBILITY
wxObject* wxWindowMSW::GetChild(int number) const
{
    // Return a pointer to the Nth object in the Panel
    wxNode *node = GetChildren().First();
    int n = number;
    while (node && n--)
        node = node->Next();
    if ( node )
    {
        wxObject *obj = (wxObject *)node->Data();
        return(obj);
    }
    else
        return NULL;
}
#endif // WXWIN_COMPATIBILITY

// Setup background and foreground colours correctly
void wxWindowMSW::SetupColours()
{
    if ( GetParent() )
        SetBackgroundColour(GetParent()->GetBackgroundColour());
}

bool wxWindowMSW::IsMouseInWindow() const
{
    // get the mouse position
    POINT pt;
    ::GetCursorPos(&pt);

    // find the window which currently has the cursor and go up the window
    // chain until we find this window - or exhaust it
    HWND hwnd = ::WindowFromPoint(pt);
    while ( hwnd && (hwnd != GetHwnd()) )
        hwnd = ::GetParent(hwnd);

    return hwnd != NULL;
}

void wxWindowMSW::OnIdle(wxIdleEvent& WXUNUSED(event))
{
    // Check if we need to send a LEAVE event
    if ( m_mouseInWindow )
    {
        if ( !IsMouseInWindow() )
        {
            // Generate a LEAVE event
            m_mouseInWindow = FALSE;

            // Unfortunately the mouse button and keyboard state may have
            // changed by the time the OnIdle function is called, so 'state'
            // may be meaningless.
            int state = 0;
            if ( wxIsShiftDown() )
                state |= MK_SHIFT;
            if ( wxIsCtrlDown() )
                state |= MK_CONTROL;
            if ( GetKeyState( VK_LBUTTON ) )
                state |= MK_LBUTTON;
            if ( GetKeyState( VK_MBUTTON ) )
                state |= MK_MBUTTON;
            if ( GetKeyState( VK_RBUTTON ) )
                state |= MK_RBUTTON;

            POINT pt;
            if ( !::GetCursorPos(&pt) )
            {
                wxLogLastError(_T("GetCursorPos"));
            }

            // we need to have client coordinates here for symmetry with
            // wxEVT_ENTER_WINDOW
            RECT rect;
            if ( !::GetWindowRect(GetHwnd(), &rect) )
            {
                wxLogLastError(_T("GetWindowRect"));
            }
            pt.x -= rect.left;
            pt.y -= rect.top;

            wxMouseEvent event2(wxEVT_LEAVE_WINDOW);
            InitMouseEvent(event2, pt.x, pt.y, state);

            (void)GetEventHandler()->ProcessEvent(event2);
        }
    }

    UpdateWindowUI();
}

// Set this window to be the child of 'parent'.
bool wxWindowMSW::Reparent(wxWindowBase *parent)
{
    if ( !wxWindowBase::Reparent(parent) )
        return FALSE;

    HWND hWndChild = GetHwnd();
    HWND hWndParent = GetParent() ? GetWinHwnd(GetParent()) : (HWND)0;

    ::SetParent(hWndChild, hWndParent);

    return TRUE;
}

void wxWindowMSW::Clear()
{
    wxClientDC dc((wxWindow *)this);
    wxBrush brush(GetBackgroundColour(), wxSOLID);
    dc.SetBackground(brush);
    dc.Clear();
}

static inline void SendSetRedraw(HWND hwnd, bool on)
{
    ::SendMessage(hwnd, WM_SETREDRAW, (WPARAM)on, 0);
}

void wxWindowMSW::Freeze()
{
    SendSetRedraw(GetHwnd(), FALSE);
}

void wxWindowMSW::Thaw()
{
    SendSetRedraw(GetHwnd(), TRUE);

    // we need to refresh everything or otherwise he invalidated area is not
    // repainted
    Refresh();
}

void wxWindowMSW::Refresh(bool eraseBack, const wxRect *rect)
{
    HWND hWnd = GetHwnd();
    if ( hWnd )
    {
        if ( rect )
        {
            RECT mswRect;
            mswRect.left = rect->x;
            mswRect.top = rect->y;
            mswRect.right = rect->x + rect->width;
            mswRect.bottom = rect->y + rect->height;

            ::InvalidateRect(hWnd, &mswRect, eraseBack);
        }
        else
            ::InvalidateRect(hWnd, NULL, eraseBack);
    }
}

void wxWindowMSW::Update()
{
    if ( !::UpdateWindow(GetHwnd()) )
    {
        wxLogLastError(_T("UpdateWindow"));
    }

#if defined(__WIN32__) && !defined(__WXMICROWIN__)
    // just calling UpdateWindow() is not enough, what we did in our WM_PAINT
    // handler needs to be really drawn right now
    (void)::GdiFlush();
#endif // __WIN32__
}

// ---------------------------------------------------------------------------
// drag and drop
// ---------------------------------------------------------------------------

#if wxUSE_DRAG_AND_DROP

void wxWindowMSW::SetDropTarget(wxDropTarget *pDropTarget)
{
    if ( m_dropTarget != 0 ) {
        m_dropTarget->Revoke(m_hWnd);
        delete m_dropTarget;
    }

    m_dropTarget = pDropTarget;
    if ( m_dropTarget != 0 )
        m_dropTarget->Register(m_hWnd);
}

#endif // wxUSE_DRAG_AND_DROP

// old style file-manager drag&drop support: we retain the old-style
// DragAcceptFiles in parallel with SetDropTarget.
void wxWindowMSW::DragAcceptFiles(bool accept)
{
    HWND hWnd = GetHwnd();
    if ( hWnd )
        ::DragAcceptFiles(hWnd, (BOOL)accept);
}

// ----------------------------------------------------------------------------
// tooltips
// ----------------------------------------------------------------------------

#if wxUSE_TOOLTIPS

void wxWindowMSW::DoSetToolTip(wxToolTip *tooltip)
{
    wxWindowBase::DoSetToolTip(tooltip);

    if ( m_tooltip )
        m_tooltip->SetWindow(this);
}

#endif // wxUSE_TOOLTIPS

// ---------------------------------------------------------------------------
// moving and resizing
// ---------------------------------------------------------------------------

// Get total size
void wxWindowMSW::DoGetSize(int *x, int *y) const
{
    HWND hWnd = GetHwnd();
    RECT rect;
#ifdef __WIN16__
    ::GetWindowRect(hWnd, &rect);
#else
    if ( !::GetWindowRect(hWnd, &rect) )
    {
        wxLogLastError(_T("GetWindowRect"));
    }
#endif
    if ( x )
        *x = rect.right - rect.left;
    if ( y )
        *y = rect.bottom - rect.top;
}

void wxWindowMSW::DoGetPosition(int *x, int *y) const
{
    HWND hWnd = GetHwnd();

    RECT rect;
    GetWindowRect(hWnd, &rect);

    POINT point;
    point.x = rect.left;
    point.y = rect.top;

    // we do the adjustments with respect to the parent only for the "real"
    // children, not for the dialogs/frames
    if ( !IsTopLevel() )
    {
        HWND hParentWnd = 0;
        wxWindow *parent = GetParent();
        if ( parent )
            hParentWnd = GetWinHwnd(parent);

        // Since we now have the absolute screen coords, if there's a parent we
        // must subtract its top left corner
        if ( hParentWnd )
        {
            ::ScreenToClient(hParentWnd, &point);
        }

        if ( parent )
        {
            // We may be faking the client origin. So a window that's really at (0,
            // 30) may appear (to wxWin apps) to be at (0, 0).
            wxPoint pt(parent->GetClientAreaOrigin());
            point.x -= pt.x;
            point.y -= pt.y;
        }
    }

    if ( x )
        *x = point.x;
    if ( y )
        *y = point.y;
}

void wxWindowMSW::DoScreenToClient(int *x, int *y) const
{
    POINT pt;
    if ( x )
        pt.x = *x;
    if ( y )
        pt.y = *y;

    HWND hWnd = GetHwnd();
    ::ScreenToClient(hWnd, &pt);

    if ( x )
        *x = pt.x;
    if ( y )
        *y = pt.y;
}

void wxWindowMSW::DoClientToScreen(int *x, int *y) const
{
    POINT pt;
    if ( x )
        pt.x = *x;
    if ( y )
        pt.y = *y;

    HWND hWnd = GetHwnd();
    ::ClientToScreen(hWnd, &pt);

    if ( x )
        *x = pt.x;
    if ( y )
        *y = pt.y;
}

// Get size *available for subwindows* i.e. excluding menu bar etc.
void wxWindowMSW::DoGetClientSize(int *x, int *y) const
{
    HWND hWnd = GetHwnd();
    RECT rect;
    ::GetClientRect(hWnd, &rect);
    if ( x )
        *x = rect.right;
    if ( y )
        *y = rect.bottom;
}

void wxWindowMSW::DoMoveWindow(int x, int y, int width, int height)
{
    if ( !::MoveWindow(GetHwnd(), x, y, width, height, TRUE) )
    {
        wxLogLastError(wxT("MoveWindow"));
    }
}

// set the size of the window: if the dimensions are positive, just use them,
// but if any of them is equal to -1, it means that we must find the value for
// it ourselves (unless sizeFlags contains wxSIZE_ALLOW_MINUS_ONE flag, in
// which case -1 is a valid value for x and y)
//
// If sizeFlags contains wxSIZE_AUTO_WIDTH/HEIGHT flags (default), we calculate
// the width/height to best suit our contents, otherwise we reuse the current
// width/height
void wxWindowMSW::DoSetSize(int x, int y, int width, int height, int sizeFlags)
{
    // get the current size and position...
    int currentX, currentY;
    GetPosition(&currentX, &currentY);
    int currentW,currentH;
    GetSize(&currentW, &currentH);

    // ... and don't do anything (avoiding flicker) if it's already ok
    if ( x == currentX && y == currentY &&
         width == currentW && height == currentH )
    {
        return;
    }

    if ( x == -1 && !(sizeFlags & wxSIZE_ALLOW_MINUS_ONE) )
        x = currentX;
    if ( y == -1 && !(sizeFlags & wxSIZE_ALLOW_MINUS_ONE) )
        y = currentY;

    AdjustForParentClientOrigin(x, y, sizeFlags);

    wxSize size(-1, -1);
    if ( width == -1 )
    {
        if ( sizeFlags & wxSIZE_AUTO_WIDTH )
        {
            size = DoGetBestSize();
            width = size.x;
        }
        else
        {
            // just take the current one
            width = currentW;
        }
    }

    if ( height == -1 )
    {
        if ( sizeFlags & wxSIZE_AUTO_HEIGHT )
        {
            if ( size.x == -1 )
            {
                size = DoGetBestSize();
            }
            //else: already called DoGetBestSize() above

            height = size.y;
        }
        else
        {
            // just take the current one
            height = currentH;
        }
    }

    DoMoveWindow(x, y, width, height);
}

void wxWindowMSW::DoSetClientSize(int width, int height)
{
    wxWindow *parent = GetParent();
    HWND hWnd = GetHwnd();
    HWND hParentWnd = (HWND) 0;
    if ( parent )
        hParentWnd = (HWND) parent->GetHWND();

    RECT rect;
    ::GetClientRect(hWnd, &rect);

    RECT rect2;
    GetWindowRect(hWnd, &rect2);

    // Find the difference between the entire window (title bar and all)
    // and the client area; add this to the new client size to move the
    // window
    int actual_width = rect2.right - rect2.left - rect.right + width;
    int actual_height = rect2.bottom - rect2.top - rect.bottom + height;

    // If there's a parent, must subtract the parent's top left corner
    // since MoveWindow moves relative to the parent

    POINT point;
    point.x = rect2.left;
    point.y = rect2.top;
    if ( parent )
    {
        ::ScreenToClient(hParentWnd, &point);
    }

    DoMoveWindow(point.x, point.y, actual_width, actual_height);

    wxSizeEvent event(wxSize(width, height), m_windowId);
    event.SetEventObject(this);
    GetEventHandler()->ProcessEvent(event);
}

// For implementation purposes - sometimes decorations make the client area
// smaller
wxPoint wxWindowMSW::GetClientAreaOrigin() const
{
    return wxPoint(0, 0);
}

// Makes an adjustment to the window position (for example, a frame that has
// a toolbar that it manages itself).
void wxWindowMSW::AdjustForParentClientOrigin(int& x, int& y, int sizeFlags)
{
    // don't do it for the dialogs/frames - they float independently of their
    // parent
    if ( !IsTopLevel() )
    {
        wxWindow *parent = GetParent();
        if ( !(sizeFlags & wxSIZE_NO_ADJUSTMENTS) && parent )
        {
            wxPoint pt(parent->GetClientAreaOrigin());
            x += pt.x;
            y += pt.y;
        }
    }
}

// ---------------------------------------------------------------------------
// text metrics
// ---------------------------------------------------------------------------

int wxWindowMSW::GetCharHeight() const
{
    return wxGetTextMetrics(this).tmHeight;
}

int wxWindowMSW::GetCharWidth() const
{
    // +1 is needed because Windows apparently adds it when calculating the
    // dialog units size in pixels
#if wxDIALOG_UNIT_COMPATIBILITY
    return wxGetTextMetrics(this).tmAveCharWidth;
#else
    return wxGetTextMetrics(this).tmAveCharWidth + 1;
#endif
}

void wxWindowMSW::GetTextExtent(const wxString& string,
                             int *x, int *y,
                             int *descent, int *externalLeading,
                             const wxFont *theFont) const
{
    const wxFont *fontToUse = theFont;
    if ( !fontToUse )
        fontToUse = &m_font;

    HWND hWnd = GetHwnd();
    HDC dc = ::GetDC(hWnd);

    HFONT fnt = 0;
    HFONT hfontOld = 0;
    if ( fontToUse && fontToUse->Ok() )
    {
        fnt = (HFONT)((wxFont *)fontToUse)->GetResourceHandle(); // const_cast
        if ( fnt )
            hfontOld = (HFONT)SelectObject(dc,fnt);
    }

    SIZE sizeRect;
    TEXTMETRIC tm;
    GetTextExtentPoint(dc, string, (int)string.Length(), &sizeRect);
    GetTextMetrics(dc, &tm);

    if ( fontToUse && fnt && hfontOld )
        SelectObject(dc, hfontOld);

    ReleaseDC(hWnd, dc);

    if ( x )
        *x = sizeRect.cx;
    if ( y )
        *y = sizeRect.cy;
    if ( descent )
        *descent = tm.tmDescent;
    if ( externalLeading )
        *externalLeading = tm.tmExternalLeading;
}

#if wxUSE_CARET && WXWIN_COMPATIBILITY
// ---------------------------------------------------------------------------
// Caret manipulation
// ---------------------------------------------------------------------------

void wxWindowMSW::CreateCaret(int w, int h)
{
    SetCaret(new wxCaret(this, w, h));
}

void wxWindowMSW::CreateCaret(const wxBitmap *WXUNUSED(bitmap))
{
    wxFAIL_MSG("not implemented");
}

void wxWindowMSW::ShowCaret(bool show)
{
    wxCHECK_RET( m_caret, "no caret to show" );

    m_caret->Show(show);
}

void wxWindowMSW::DestroyCaret()
{
    SetCaret(NULL);
}

void wxWindowMSW::SetCaretPos(int x, int y)
{
    wxCHECK_RET( m_caret, "no caret to move" );

    m_caret->Move(x, y);
}

void wxWindowMSW::GetCaretPos(int *x, int *y) const
{
    wxCHECK_RET( m_caret, "no caret to get position of" );

    m_caret->GetPosition(x, y);
}
#endif // wxUSE_CARET

// ---------------------------------------------------------------------------
// popup menu
// ---------------------------------------------------------------------------

// yield for WM_COMMAND events only, i.e. process all WM_COMMANDs in the queue
// immediately, without waiting for the next event loop iteration
//
// NB: this function should probably be made public later as it can almost
//     surely replace wxYield() elsewhere as well
static void wxYieldForCommandsOnly()
{
    // peek all WM_COMMANDs (it will always return WM_QUIT too but we don't
    // want to process it here)
    MSG msg;
    while ( ::PeekMessage(&msg, (HWND)0, WM_COMMAND, WM_COMMAND, PM_REMOVE)
                && msg.message != WM_QUIT )
    {
        wxTheApp->DoMessage((WXMSG *)&msg);
    }
}

#if wxUSE_MENUS_NATIVE

bool wxWindowMSW::DoPopupMenu(wxMenu *menu, int x, int y)
{
    menu->SetInvokingWindow(this);
    menu->UpdateUI();

    HWND hWnd = GetHwnd();
    HMENU hMenu = GetHmenuOf(menu);
    POINT point;
    point.x = x;
    point.y = y;
    ::ClientToScreen(hWnd, &point);
    wxCurrentPopupMenu = menu;
    ::TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, point.x, point.y, 0, hWnd, NULL);

    // we need to do it righ now as otherwise the events are never going to be
    // sent to wxCurrentPopupMenu from HandleCommand()
    //
    // note that even eliminating (ugly) wxCurrentPopupMenu global wouldn't
    // help and we'd still need wxYieldForCommandsOnly() as the menu may be
    // destroyed as soon as we return (it can be a local variable in the caller
    // for example) and so we do need to process the event immediately
    wxYieldForCommandsOnly();

    wxCurrentPopupMenu = NULL;

    menu->SetInvokingWindow(NULL);

    return TRUE;
}

#endif // wxUSE_MENUS_NATIVE

// ===========================================================================
// pre/post message processing
// ===========================================================================

long wxWindowMSW::MSWDefWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam)
{
    if ( m_oldWndProc )
        return ::CallWindowProc(CASTWNDPROC m_oldWndProc, GetHwnd(), (UINT) nMsg, (WPARAM) wParam, (LPARAM) lParam);
    else
        return ::DefWindowProc(GetHwnd(), nMsg, wParam, lParam);
}

bool wxWindowMSW::MSWProcessMessage(WXMSG* pMsg)
{
    // wxUniversal implements tab traversal itself
#ifndef __WXUNIVERSAL__
    if ( m_hWnd != 0 && (GetWindowStyleFlag() & wxTAB_TRAVERSAL) )
    {
        // intercept dialog navigation keys
        MSG *msg = (MSG *)pMsg;

        // here we try to do all the job which ::IsDialogMessage() usually does
        // internally
#if 1
        bool bProcess = TRUE;
        if ( msg->message != WM_KEYDOWN )
            bProcess = FALSE;

        if ( bProcess && (HIWORD(msg->lParam) & KF_ALTDOWN) == KF_ALTDOWN )
            bProcess = FALSE;

        if ( bProcess )
        {
            bool bCtrlDown = wxIsCtrlDown();
            bool bShiftDown = wxIsShiftDown();

            // WM_GETDLGCODE: ask the control if it wants the key for itself,
            // don't process it if it's the case (except for Ctrl-Tab/Enter
            // combinations which are always processed)
            LONG lDlgCode = 0;
            if ( !bCtrlDown )
            {
                lDlgCode = ::SendMessage(msg->hwnd, WM_GETDLGCODE, 0, 0);
            }

            bool bForward = TRUE,
                 bWindowChange = FALSE;

            switch ( msg->wParam )
            {
                case VK_TAB:
                    // assume that nobody wants Shift-TAB for himself - if we
                    // don't do it there is no easy way for a control to grab
                    // TABs but still let Shift-TAB work as navugation key
                    if ( (lDlgCode & DLGC_WANTTAB) && !bShiftDown ) {
                        bProcess = FALSE;
                    }
                    else {
                        // Ctrl-Tab cycles thru notebook pages
                        bWindowChange = bCtrlDown;
                        bForward = !bShiftDown;
                    }
                    break;

                case VK_UP:
                case VK_LEFT:
                    if ( (lDlgCode & DLGC_WANTARROWS) || bCtrlDown )
                        bProcess = FALSE;
                    else
                        bForward = FALSE;
                    break;

                case VK_DOWN:
                case VK_RIGHT:
                    if ( (lDlgCode & DLGC_WANTARROWS) || bCtrlDown )
                        bProcess = FALSE;
                    break;

                case VK_RETURN:
                    {
                        if ( (lDlgCode & DLGC_WANTMESSAGE) && !bCtrlDown )
                        {
                            // control wants to process Enter itself, don't
                            // call IsDialogMessage() which would interpret
                            // it
                            return FALSE;
                        }
                        else if ( lDlgCode & DLGC_BUTTON )
                        {
                            // let IsDialogMessage() handle this for all
                            // buttons except the owner-drawn ones which it
                            // just seems to ignore
                            long style = ::GetWindowLong(msg->hwnd, GWL_STYLE);
                            if ( (style & BS_OWNERDRAW) == BS_OWNERDRAW )
                            {
                                // emulate the button click
                                wxWindow *btn = wxFindWinFromHandle((WXHWND)msg->hwnd);
                                if ( btn )
                                    btn->MSWCommand(BN_CLICKED, 0 /* unused */);
                            }

                            bProcess = FALSE;
                        }
#if wxUSE_BUTTON
                        else
                        {
                            wxPanel *panel = wxDynamicCastThis(wxPanel);
                            wxButton *btn = NULL;
                            if ( panel )
                            {
                                // panel may have a default button which should
                                // be activated by Enter
                                btn = panel->GetDefaultItem();
                            }

                            if ( btn && btn->IsEnabled() )
                            {
                                // if we do have a default button, do press it
                                btn->MSWCommand(BN_CLICKED, 0 /* unused */);

                                return TRUE;
                            }
                            // else: but if it does not it makes sense to make
                            //       it work like a TAB - and that's what we do.
                            //       Note that Ctrl-Enter always works this way.
                        }
#endif // wxUSE_BUTTON
                    }
                    break;

                default:
                    bProcess = FALSE;
            }

            if ( bProcess )
            {
                wxNavigationKeyEvent event;
                event.SetDirection(bForward);
                event.SetWindowChange(bWindowChange);
                event.SetEventObject(this);

                if ( GetEventHandler()->ProcessEvent(event) )
                {
#if wxUSE_BUTTON
                    wxButton *btn = wxDynamicCast(FindFocus(), wxButton);
                    if ( btn )
                    {
                        // the button which has focus should be default
                        btn->SetDefault();
                    }
#endif // wxUSE_BUTTON

                    return TRUE;
                }
            }
        }
#else
        // let ::IsDialogMessage() do almost everything and handle just the
        // things it doesn't here: Ctrl-TAB for switching notebook pages
        if ( msg->message == WM_KEYDOWN )
        {
            // don't process system keys here
            if ( !(HIWORD(msg->lParam) & KF_ALTDOWN) )
            {
                if ( (msg->wParam == VK_TAB) && wxIsCtrlDown() )
                {
                    // find the first notebook parent and change its page
                    wxWindow *win = this;
                    wxNotebook *nbook = NULL;
                    while ( win && !nbook )
                    {
                        nbook = wxDynamicCast(win, wxNotebook);
                        win = win->GetParent();
                    }

                    if ( nbook )
                    {
                        bool forward = !wxIsShiftDown();

                        nbook->AdvanceSelection(forward);
                    }
                }
            }
        }
#endif // 0

        if ( ::IsDialogMessage(GetHwnd(), msg) )
        {
            // IsDialogMessage() did something...
            return TRUE;
        }
    }
#endif // __WXUNIVERSAL__

#if wxUSE_TOOLTIPS
    if ( m_tooltip )
    {
        // relay mouse move events to the tooltip control
        MSG *msg = (MSG *)pMsg;
        if ( msg->message == WM_MOUSEMOVE )
            m_tooltip->RelayEvent(pMsg);
    }
#endif // wxUSE_TOOLTIPS

    return FALSE;
}

bool wxWindowMSW::MSWTranslateMessage(WXMSG* pMsg)
{
#if wxUSE_ACCEL && !defined(__WXUNIVERSAL__)
    return m_acceleratorTable.Translate(this, pMsg);
#else
    return FALSE;
#endif // wxUSE_ACCEL
}

// ---------------------------------------------------------------------------
// message params unpackers (different for Win16 and Win32)
// ---------------------------------------------------------------------------

#ifdef __WIN32__

void wxWindowMSW::UnpackCommand(WXWPARAM wParam, WXLPARAM lParam,
                             WORD *id, WXHWND *hwnd, WORD *cmd)
{
    *id = LOWORD(wParam);
    *hwnd = (WXHWND)lParam;
    *cmd = HIWORD(wParam);
}

void wxWindowMSW::UnpackActivate(WXWPARAM wParam, WXLPARAM lParam,
                              WXWORD *state, WXWORD *minimized, WXHWND *hwnd)
{
    *state = LOWORD(wParam);
    *minimized = HIWORD(wParam);
    *hwnd = (WXHWND)lParam;
}

void wxWindowMSW::UnpackScroll(WXWPARAM wParam, WXLPARAM lParam,
                            WXWORD *code, WXWORD *pos, WXHWND *hwnd)
{
    *code = LOWORD(wParam);
    *pos = HIWORD(wParam);
    *hwnd = (WXHWND)lParam;
}

void wxWindowMSW::UnpackCtlColor(WXWPARAM wParam, WXLPARAM lParam,
                              WXWORD *nCtlColor, WXHDC *hdc, WXHWND *hwnd)
{
#ifndef __WXMICROWIN__
    *nCtlColor = CTLCOLOR_BTN;
    *hwnd = (WXHWND)lParam;
    *hdc = (WXHDC)wParam;
#endif
}

void wxWindowMSW::UnpackMenuSelect(WXWPARAM wParam, WXLPARAM lParam,
                                WXWORD *item, WXWORD *flags, WXHMENU *hmenu)
{
    *item = (WXWORD)wParam;
    *flags = HIWORD(wParam);
    *hmenu = (WXHMENU)lParam;
}

#else // Win16

void wxWindowMSW::UnpackCommand(WXWPARAM wParam, WXLPARAM lParam,
                             WXWORD *id, WXHWND *hwnd, WXWORD *cmd)
{
    *id = (WXWORD)wParam;
    *hwnd = (WXHWND)LOWORD(lParam);
    *cmd = HIWORD(lParam);
}

void wxWindowMSW::UnpackActivate(WXWPARAM wParam, WXLPARAM lParam,
                              WXWORD *state, WXWORD *minimized, WXHWND *hwnd)
{
    *state = (WXWORD)wParam;
    *minimized = LOWORD(lParam);
    *hwnd = (WXHWND)HIWORD(lParam);
}

void wxWindowMSW::UnpackScroll(WXWPARAM wParam, WXLPARAM lParam,
                            WXWORD *code, WXWORD *pos, WXHWND *hwnd)
{
    *code = (WXWORD)wParam;
    *pos = LOWORD(lParam);
    *hwnd = (WXHWND)HIWORD(lParam);
}

void wxWindowMSW::UnpackCtlColor(WXWPARAM wParam, WXLPARAM lParam,
                              WXWORD *nCtlColor, WXHDC *hdc, WXHWND *hwnd)
{
    *hwnd = (WXHWND)LOWORD(lParam);
    *nCtlColor = (int)HIWORD(lParam);
    *hdc = (WXHDC)wParam;
}

void wxWindowMSW::UnpackMenuSelect(WXWPARAM wParam, WXLPARAM lParam,
                                WXWORD *item, WXWORD *flags, WXHMENU *hmenu)
{
    *item = (WXWORD)wParam;
    *flags = LOWORD(lParam);
    *hmenu = (WXHMENU)HIWORD(lParam);
}

#endif // Win32/16

// ---------------------------------------------------------------------------
// Main wxWindows window proc and the window proc for wxWindow
// ---------------------------------------------------------------------------

// Hook for new window just as it's being created, when the window isn't yet
// associated with the handle
wxWindowMSW *wxWndHook = NULL;

// Main window proc
LRESULT WXDLLEXPORT APIENTRY _EXPORT wxWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // trace all messages - useful for the debugging
#ifdef __WXDEBUG__
    wxLogTrace(wxTraceMessages, wxT("Processing %s(wParam=%8lx, lParam=%8lx)"),
               wxGetMessageName(message), wParam, lParam);
#endif // __WXDEBUG__

    wxWindowMSW *wnd = wxFindWinFromHandle((WXHWND) hWnd);

    // when we get the first message for the HWND we just created, we associate
    // it with wxWindow stored in wxWndHook
    if ( !wnd && wxWndHook )
    {
#if 0 // def __WXDEBUG__
        char buf[512];
        ::GetClassNameA((HWND) hWnd, buf, 512);
        wxString className(buf);
#endif

        wxAssociateWinWithHandle(hWnd, wxWndHook);
        wnd = wxWndHook;
        wxWndHook = NULL;
        wnd->SetHWND((WXHWND)hWnd);
    }

    LRESULT rc;

    // Stop right here if we don't have a valid handle in our wxWindow object.
    if ( wnd && !wnd->GetHWND() )
    {
        // FIXME: why do we do this?
        wnd->SetHWND((WXHWND) hWnd);
        rc = wnd->MSWDefWindowProc(message, wParam, lParam );
        wnd->SetHWND(0);
    }
    else
    {
        if ( wnd )
            rc = wnd->MSWWindowProc(message, wParam, lParam);
        else
            rc = DefWindowProc( hWnd, message, wParam, lParam );
    }

    return rc;
}

long wxWindowMSW::MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam)
{
    // did we process the message?
    bool processed = FALSE;

    // the return value
    union
    {
        bool        allow;
        long        result;
        WXHICON     hIcon;
        WXHBRUSH    hBrush;
    } rc;

    // for most messages we should return 0 when we do process the message
    rc.result = 0;

    switch ( message )
    {
        case WM_CREATE:
            {
                bool mayCreate;
                processed = HandleCreate((WXLPCREATESTRUCT)lParam, &mayCreate);
                if ( processed )
                {
                    // return 0 to allow window creation
                    rc.result = mayCreate ? 0 : -1;
                }
            }
            break;

        case WM_DESTROY:
            processed = HandleDestroy();
            break;

        case WM_MOVE:
            processed = HandleMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            break;

        case WM_SIZE:
            switch ( wParam )
            {
                case SIZE_MAXHIDE:
                case SIZE_MAXSHOW:
                    // we're not interested in these messages at all
                    break;

                case SIZE_MINIMIZED:
                    // we shouldn't send sizev events for these messages as the
                    // client size may be negative which breaks existing code
                    //
                    // OTOH we might send another (wxMinimizedEvent?) one or
                    // add an additional parameter to wxSizeEvent if this is
                    // useful to anybody
                    break;

                default:
                    wxFAIL_MSG( _T("unexpected WM_SIZE parameter") );
                    // fall through nevertheless

                case SIZE_MAXIMIZED:
                case SIZE_RESTORED:
                    processed = HandleSize(LOWORD(lParam), HIWORD(lParam),
                                           wParam);
            }
            break;

#ifndef __WXMICROWIN__
        case WM_ACTIVATEAPP:
            wxTheApp->SetActive(wParam != 0, FindFocus());
            break;
#endif

        case WM_ACTIVATE:
            {
                WXWORD state, minimized;
                WXHWND hwnd;
                UnpackActivate(wParam, lParam, &state, &minimized, &hwnd);

                processed = HandleActivate(state, minimized != 0, (WXHWND)hwnd);
            }
            break;

        case WM_SETFOCUS:
            processed = HandleSetFocus((WXHWND)(HWND)wParam);
            break;

        case WM_KILLFOCUS:
            processed = HandleKillFocus((WXHWND)(HWND)wParam);
            break;

        case WM_PAINT:
            processed = HandlePaint();
            break;

        case WM_CLOSE:
            // don't let the DefWindowProc() destroy our window - we'll do it
            // ourselves in ~wxWindow
            processed = TRUE;
            rc.result = TRUE;
            break;

        case WM_SHOWWINDOW:
            processed = HandleShow(wParam != 0, (int)lParam);
            break;

        case WM_MOUSEMOVE:
            processed = HandleMouseMove(GET_X_LPARAM(lParam),
                                        GET_Y_LPARAM(lParam),
                                        wParam);
            break;

#if wxUSE_MOUSEWHEEL
        case WM_MOUSEWHEEL:
            processed = HandleMouseWheel(wParam, lParam);
            break;
#endif

        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_LBUTTONDBLCLK:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_RBUTTONDBLCLK:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_MBUTTONDBLCLK:
         {
                processed = FALSE;
#ifdef __WXMICROWIN__
                // MicroWindows seems to ignore the fact that a window is
                // disabled. So catch mouse events and throw them away if
                // necessary.
                wxWindowMSW* win = this;
                while (win)
                {
                    if (!win->IsEnabled())
                    {
                        processed = TRUE;
                        break;
                    }
                    win = win->GetParent();
                    if (win && win->IsTopLevel())
                        break;
                }
#endif // __WXMICROWIN__
                if (!processed)
                {
                    if (message == WM_LBUTTONDOWN && AcceptsFocus())
                        SetFocus();
                     processed = HandleMouseEvent(message,
                                         GET_X_LPARAM(lParam),
                                         GET_Y_LPARAM(lParam),
                                                  wParam);
                }
                break;
         }

#ifdef __WXMICROWIN__
        case WM_NCLBUTTONDOWN:
        case WM_NCLBUTTONUP:
        case WM_NCLBUTTONDBLCLK:
        case WM_NCRBUTTONDOWN:
        case WM_NCRBUTTONUP:
        case WM_NCRBUTTONDBLCLK:
#if 0
        case WM_NCMBUTTONDOWN:
        case WM_NCMBUTTONUP:
        case WM_NCMBUTTONDBLCLK:
#endif
            {
                // MicroWindows seems to ignore the fact that a window
                // is disabled. So catch mouse events and throw them away if necessary.
                processed = FALSE;
                wxWindowMSW* win = this;
                while (win)
                {
                    if (!win->IsEnabled())
                    {
                        processed = TRUE;
                        break;
                    }
                    win = win->GetParent();
                    if (win && win->IsTopLevel())
                        break;
                }
                break;
            }
#endif // __WXMICROWIN__

#ifdef MM_JOY1MOVE
        case MM_JOY1MOVE:
        case MM_JOY2MOVE:
        case MM_JOY1ZMOVE:
        case MM_JOY2ZMOVE:
        case MM_JOY1BUTTONDOWN:
        case MM_JOY2BUTTONDOWN:
        case MM_JOY1BUTTONUP:
        case MM_JOY2BUTTONUP:
            processed = HandleJoystickEvent(message,
                                            GET_X_LPARAM(lParam),
                                            GET_Y_LPARAM(lParam),
                                            wParam);
            break;
#endif // __WXMICROWIN__

        case WM_SYSCOMMAND:
            processed = HandleSysCommand(wParam, lParam);
            break;

        case WM_COMMAND:
            {
                WORD id, cmd;
                WXHWND hwnd;
                UnpackCommand(wParam, lParam, &id, &hwnd, &cmd);

                processed = HandleCommand(id, cmd, hwnd);
            }
            break;

#ifdef __WIN95__
        case WM_NOTIFY:
            processed = HandleNotify((int)wParam, lParam, &rc.result);
            break;
#endif  // Win95

            // for these messages we must return TRUE if process the message
#ifdef WM_DRAWITEM
        case WM_DRAWITEM:
        case WM_MEASUREITEM:
            {
                int idCtrl = (UINT)wParam;
                if ( message == WM_DRAWITEM )
                {
                    processed = MSWOnDrawItem(idCtrl,
                                              (WXDRAWITEMSTRUCT *)lParam);
                }
                else
                {
                    processed = MSWOnMeasureItem(idCtrl,
                                                 (WXMEASUREITEMSTRUCT *)lParam);
                }

                if ( processed )
                    rc.result = TRUE;
            }
            break;
#endif // defined(WM_DRAWITEM)

        case WM_GETDLGCODE:
            if ( m_lDlgCode )
            {
                rc.result = m_lDlgCode;
                processed = TRUE;
            }
            //else: get the dlg code from the DefWindowProc()
            break;

        case WM_SYSKEYDOWN:
        case WM_KEYDOWN:
            // If this has been processed by an event handler,
            // return 0 now (we've handled it).
            if ( HandleKeyDown((WORD) wParam, lParam) )
            {
                processed = TRUE;

                break;
            }

            // we consider these message "not interesting" to OnChar
            if ( wParam == VK_SHIFT || wParam == VK_CONTROL )
            {
                processed = TRUE;

                break;
            }

            switch ( wParam )
            {
                // avoid duplicate messages to OnChar for these ASCII keys: they
                // will be translated by TranslateMessage() and received in WM_CHAR
                case VK_ESCAPE:
                case VK_SPACE:
                case VK_RETURN:
                case VK_BACK:
                case VK_TAB:
                case VK_ADD:
                case VK_SUBTRACT:
                    // but set processed to FALSE, not TRUE to still pass them to
                    // the control's default window proc - otherwise built-in
                    // keyboard handling won't work
                    processed = FALSE;

                    break;

#ifdef VK_APPS
                // special case of VK_APPS: treat it the same as right mouse
                // click because both usually pop up a context menu
                case VK_APPS:
                    {
                        WPARAM flags;
                        int x, y;

                        TranslateKbdEventToMouse(this, &x, &y, &flags);
                        processed = HandleMouseEvent(WM_RBUTTONDOWN, x, y, flags);
                    }
                    break;
#endif // VK_APPS

                case VK_LEFT:
                case VK_RIGHT:
                case VK_DOWN:
                case VK_UP:
                default:
                    processed = HandleChar((WORD)wParam, lParam);
            }
            break;

        case WM_SYSKEYUP:
        case WM_KEYUP:
#ifdef VK_APPS
            // special case of VK_APPS: treat it the same as right mouse button
            if ( wParam == VK_APPS )
            {
                WPARAM flags;
                int x, y;

                TranslateKbdEventToMouse(this, &x, &y, &flags);
                processed = HandleMouseEvent(WM_RBUTTONUP, x, y, flags);
            }
            else
#endif // VK_APPS
            {
                processed = HandleKeyUp((WORD) wParam, lParam);
            }
            break;

        case WM_SYSCHAR:
        case WM_CHAR: // Always an ASCII character
            processed = HandleChar((WORD)wParam, lParam, TRUE);
            break;

        case WM_HSCROLL:
        case WM_VSCROLL:
            {
                WXWORD code, pos;
                WXHWND hwnd;
                UnpackScroll(wParam, lParam, &code, &pos, &hwnd);

                processed = MSWOnScroll(message == WM_HSCROLL ? wxHORIZONTAL
                                                              : wxVERTICAL,
                                        code, pos, hwnd);
            }
            break;

        // CTLCOLOR messages are sent by children to query the parent for their
        // colors#ifndef __WXMICROWIN__
#ifndef __WXMICROWIN__
#ifdef __WIN32__
        case WM_CTLCOLORMSGBOX:
        case WM_CTLCOLOREDIT:
        case WM_CTLCOLORLISTBOX:
        case WM_CTLCOLORBTN:
        case WM_CTLCOLORDLG:
        case WM_CTLCOLORSCROLLBAR:
        case WM_CTLCOLORSTATIC:
#else // Win16
        case WM_CTLCOLOR:
#endif // Win32/16
            {
                WXWORD nCtlColor;
                WXHDC hdc;
                WXHWND hwnd;
                UnpackCtlColor(wParam, lParam, &nCtlColor, &hdc, &hwnd);

                processed = HandleCtlColor(&rc.hBrush,
                                           (WXHDC)hdc,
                                           (WXHWND)hwnd,
                                           nCtlColor,
                                           message,
                                           wParam,
                                           lParam);
            }
            break;
#endif // !__WXMICROWIN__

            // the return value for this message is ignored
        case WM_SYSCOLORCHANGE:
            processed = HandleSysColorChange();
            break;

        case WM_PALETTECHANGED:
            processed = HandlePaletteChanged((WXHWND) (HWND) wParam);
            break;

        case WM_QUERYNEWPALETTE:
            processed = HandleQueryNewPalette();
            break;

        case WM_ERASEBKGND:
            processed = HandleEraseBkgnd((WXHDC)(HDC)wParam);
            if ( processed )
            {
                // we processed the message, i.e. erased the background
                rc.result = TRUE;
            }
            break;

        case WM_DROPFILES:
            processed = HandleDropFiles(wParam);
            break;

        case WM_INITDIALOG:
            processed = HandleInitDialog((WXHWND)(HWND)wParam);

            if ( processed )
            {
                // we never set focus from here
                rc.result = FALSE;
            }
            break;

        case WM_QUERYENDSESSION:
            processed = HandleQueryEndSession(lParam, &rc.allow);
            break;

        case WM_ENDSESSION:
            processed = HandleEndSession(wParam != 0, lParam);
            break;

        case WM_GETMINMAXINFO:
            processed = HandleGetMinMaxInfo((MINMAXINFO*)lParam);
            break;

        case WM_SETCURSOR:
            processed = HandleSetCursor((WXHWND)(HWND)wParam,
                                        LOWORD(lParam),     // hit test
                                        HIWORD(lParam));    // mouse msg

            if ( processed )
            {
                // returning TRUE stops the DefWindowProc() from further
                // processing this message - exactly what we need because we've
                // just set the cursor.
                rc.result = TRUE;
            }
            break;

#if defined(__WIN32__) && defined(WM_HELP)
        case WM_HELP:
            {
                HELPINFO* info = (HELPINFO*) lParam;
                // Don't yet process menu help events, just windows
                if (info->iContextType == HELPINFO_WINDOW)
                {
                    wxWindowMSW* subjectOfHelp = this;
                    bool eventProcessed = FALSE;
                    while (subjectOfHelp && !eventProcessed)
                    {
                        wxHelpEvent helpEvent(wxEVT_HELP,
                                              subjectOfHelp->GetId(),
                                              wxPoint(info->MousePos.x,
                                              info->MousePos.y) );
                        helpEvent.SetEventObject(this);
                        eventProcessed =
                            GetEventHandler()->ProcessEvent(helpEvent);

                        // Go up the window hierarchy until the event is
                        // handled (or not)
                        subjectOfHelp = subjectOfHelp->GetParent();
                    }

                    processed = eventProcessed;
                }
                else if (info->iContextType == HELPINFO_MENUITEM)
                {
                    wxHelpEvent helpEvent(wxEVT_HELP, info->iCtrlId);
                    helpEvent.SetEventObject(this);
                    processed = GetEventHandler()->ProcessEvent(helpEvent);

                }
                //else: processed is already FALSE
            }
            break;

        case WM_CONTEXTMENU:
            {
                // we don't convert from screen to client coordinates as
                // the event may be handled by a parent window
                wxPoint pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

                wxContextMenuEvent evtCtx(wxEVT_CONTEXT_MENU, GetId(), pt);
                processed = GetEventHandler()->ProcessEvent(evtCtx);
            }
            break;
#endif // __WIN32__

#ifdef __WXUNIVERSAL__
        case WM_NCHITTEST:
            // we shouldn't allow the windows which don't want to get focus to
            // get it
            if ( !AcceptsFocus() )
            {
                rc.result = HTTRANSPARENT;
                processed = TRUE;
            }
            break;
#endif // __WXUNIVERSAL__
    }

    if ( !processed )
    {
#ifdef __WXDEBUG__
        wxLogTrace(wxTraceMessages, wxT("Forwarding %s to DefWindowProc."),
                   wxGetMessageName(message));
#endif // __WXDEBUG__
        rc.result = MSWDefWindowProc(message, wParam, lParam);
    }

    return rc.result;
}

// Dialog window proc
LONG APIENTRY _EXPORT
wxDlgProc(HWND WXUNUSED(hWnd), UINT message, WPARAM WXUNUSED(wParam), LPARAM WXUNUSED(lParam))
{
    if ( message == WM_INITDIALOG )
    {
        // for this message, returning TRUE tells system to set focus to the
        // first control in the dialog box
        return TRUE;
    }
    else
    {
        // for all the other ones, FALSE means that we didn't process the
        // message
        return 0;
    }
}

wxList *wxWinHandleList = NULL;
wxWindow *wxFindWinFromHandle(WXHWND hWnd)
{
    wxNode *node = wxWinHandleList->Find((long)hWnd);
    if ( !node )
        return NULL;
    return (wxWindow *)node->Data();
}

#if 0 // def __WXDEBUG__
static int gs_AssociationCount = 0;
#endif

void wxAssociateWinWithHandle(HWND hWnd, wxWindowMSW *win)
{
    // adding NULL hWnd is (first) surely a result of an error and
    // (secondly) breaks menu command processing
    wxCHECK_RET( hWnd != (HWND)NULL,
                 wxT("attempt to add a NULL hWnd to window list ignored") );


    wxWindow *oldWin = wxFindWinFromHandle((WXHWND) hWnd);
    if ( oldWin && (oldWin != win) )
    {
        wxString str(win->GetClassInfo()->GetClassName());
        wxLogError(wxT("Bug! Found existing HWND %X for new window of class %s"), (int) hWnd, (const wxChar*) str);
    }
    else if (!oldWin)
    {
#if 0 // def __WXDEBUG__
        gs_AssociationCount ++;
        wxLogDebug("+ Association %d", gs_AssociationCount);
#endif

        wxWinHandleList->Append((long)hWnd, win);
    }
}

void wxRemoveHandleAssociation(wxWindowMSW *win)
{
#if 0 // def __WXDEBUG__
    if (wxWinHandleList->Member(win))
    {
        wxLogDebug("- Association %d", gs_AssociationCount);
        gs_AssociationCount --;
    }
#endif
    wxWinHandleList->DeleteObject(win);
}

// Default destroyer - override if you destroy it in some other way
// (e.g. with MDI child windows)
void wxWindowMSW::MSWDestroyWindow()
{
}

void wxWindowMSW::MSWDetachWindowMenu()
{
#ifndef __WXUNIVERSAL__
    if ( m_hMenu )
    {
        wxChar buf[1024];
        HMENU hMenu = (HMENU)m_hMenu;

        int N = ::GetMenuItemCount(hMenu);
        for ( int i = 0; i < N; i++ )
        {
            if ( !::GetMenuString(hMenu, i, buf, WXSIZEOF(buf), MF_BYPOSITION) )
            {
                wxLogLastError(wxT("GetMenuString"));

                continue;
            }

            if ( wxStrcmp(buf, _("&Window")) == 0 )
            {
                if ( !::RemoveMenu(hMenu, i, MF_BYPOSITION) )
                {
                    wxLogLastError(wxT("RemoveMenu"));
                }

                break;
            }
        }
    }
#endif
}

bool wxWindowMSW::MSWCreate(int id,
                            wxWindow *parent,
                            const wxChar *wclass,
                            wxWindow * WXUNUSED(wx_win),
                            const wxChar *title,
                            int x,
                            int y,
                            int width,
                            int height,
                            WXDWORD style,
                            const wxChar *dialog_template,
                            WXDWORD extendedStyle)
{
    int x1 = CW_USEDEFAULT;
    int y1 = 0;
    int width1 = CW_USEDEFAULT;
    int height1 = 100;

    // Find parent's size, if it exists, to set up a possible default
    // panel size the size of the parent window
    RECT rectParent;
    if ( parent )
    {
        ::GetClientRect(GetHwndOf(parent), &rectParent);

        width1 = rectParent.right - rectParent.left;
        height1 = rectParent.bottom - rectParent.top;
    }

    if ( x != -1 )
        x1 = x;
    if ( y != -1 )
        y1 = y;
    if ( width != -1 )
        width1 = width;
    if ( height != -1 )
        height1 = height;

    // unfortunately, setting WS_EX_CONTROLPARENT only for some windows in the
    // hierarchy with several embedded panels (and not all of them) causes the
    // program to hang during the next call to IsDialogMessage() due to the bug
    // in this function (at least in Windows NT 4.0, it seems to work ok in
    // Win2K)
#if 0
    // if we have wxTAB_TRAVERSAL style, we want WS_EX_CONTROLPARENT or
    // IsDialogMessage() won't work for us
    if ( GetWindowStyleFlag() & wxTAB_TRAVERSAL )
    {
        extendedStyle |= WS_EX_CONTROLPARENT;
    }
#endif // 0

    HWND hParent;
    if ( GetWindowStyleFlag() & wxPOPUP_WINDOW )
    {
        // popup windows should have desktop as parent because they shouldn't
        // be limited to the parents client area as child windows usually are
        hParent = ::GetDesktopWindow();
    }
    else if ( parent )
    {
        hParent = GetHwndOf(parent);
    }
    else
    {
        // top level window
        hParent = NULL;
    }

    wxWndHook = this;

#ifndef __WXMICROWIN__
    if ( dialog_template )
    {
        // for the dialogs without wxDIALOG_NO_PARENT style, use the top level
        // app window as parent - this avoids creating modal dialogs without
        // parent
        if ( !hParent && !(GetWindowStyleFlag() & wxDIALOG_NO_PARENT) )
        {
            wxWindow *winTop = wxTheApp->GetTopWindow();
            if ( winTop )
                hParent = GetHwndOf(winTop);
        }

        m_hWnd = (WXHWND)::CreateDialog(wxGetInstance(),
                                        dialog_template,
                                        hParent,
                                        (DLGPROC)wxDlgProc);

        if ( m_hWnd == 0 )
        {
            wxLogError(_("Can't find dialog template '%s'!\nCheck resource include path for finding wx.rc."),
                       dialog_template);

            return FALSE;
        }

        if ( extendedStyle != 0 )
        {
            ::SetWindowLong(GetHwnd(), GWL_EXSTYLE, extendedStyle);
            ::SetWindowPos(GetHwnd(), NULL, 0, 0, 0, 0,
                           SWP_NOSIZE |
                           SWP_NOMOVE |
                           SWP_NOZORDER |
                           SWP_NOACTIVATE);
        }

#if defined(__WIN95__)
        // For some reason, the system menu is activated when we use the
        // WS_EX_CONTEXTHELP style, so let's set a reasonable icon
        if (extendedStyle & WS_EX_CONTEXTHELP)
        {
            wxFrame *winTop = wxDynamicCast(wxTheApp->GetTopWindow(), wxFrame);
            if ( winTop )
            {
                wxIcon icon = winTop->GetIcon();
                if ( icon.Ok() )
                {
                    ::SendMessage(GetHwnd(), WM_SETICON,
                                  (WPARAM)TRUE,
                                  (LPARAM)GetHiconOf(icon));
                }
            }
        }
#endif // __WIN95__


        // JACS: is the following still necessary? The above seems to work.

        // ::SetWindowLong(GWL_EXSTYLE) doesn't work for the dialogs, so try
        // to take care of (at least some) extended style flags ourselves
        if ( extendedStyle & WS_EX_TOPMOST )
        {
            if ( !::SetWindowPos(GetHwnd(), HWND_TOPMOST, 0, 0, 0, 0,
                                 SWP_NOSIZE | SWP_NOMOVE) )
            {
                wxLogLastError(wxT("SetWindowPos"));
            }
        }

        // move the dialog to its initial position without forcing repainting
        if ( !::MoveWindow(GetHwnd(), x1, y1, width1, height1, FALSE) )
        {
            wxLogLastError(wxT("MoveWindow"));
        }

    }
    else // creating a normal window, not a dialog
#endif // !__WXMICROWIN__
    {
        int controlId = 0;
        if ( style & WS_CHILD )
        {
            controlId = id;

            if ( GetWindowStyleFlag() & wxCLIP_SIBLINGS )
            {
                style |= WS_CLIPSIBLINGS;
            }
        }

        wxString className(wclass);
        if ( GetWindowStyleFlag() & wxNO_FULL_REPAINT_ON_RESIZE )
        {
            className += wxT("NR");
        }

        m_hWnd = (WXHWND)CreateWindowEx(extendedStyle,
                                        className,
                                        title ? title : wxT(""),
                                        style,
                                        x1, y1,
                                        width1, height1,
                                        hParent, (HMENU)controlId,
                                        wxGetInstance(),
                                        NULL);

        if ( !m_hWnd )
        {
            wxLogError(_("Can't create window of class %s!\nPossible Windows 3.x compatibility problem?"),
                       wclass);

            return FALSE;
        }
    }

    wxWndHook = NULL;

#ifdef __WXDEBUG__
    wxNode* node = wxWinHandleList->Member(this);
    if (node)
    {
        HWND hWnd = (HWND) node->GetKeyInteger();
        if (hWnd != (HWND) m_hWnd)
        {
            wxLogError(wxT("A second HWND association is being added for the same window!"));
        }
    }
#endif // Debug

    wxAssociateWinWithHandle((HWND) m_hWnd, this);

    SetFont(wxSystemSettings::GetSystemFont(wxSYS_DEFAULT_GUI_FONT));

    return TRUE;
}

// ===========================================================================
// MSW message handlers
// ===========================================================================

// ---------------------------------------------------------------------------
// WM_NOTIFY
// ---------------------------------------------------------------------------

#ifdef __WIN95__
// FIXME: VZ: I'm not sure at all that the order of processing is correct
bool wxWindowMSW::HandleNotify(int idCtrl, WXLPARAM lParam, WXLPARAM *result)
{
#ifndef __WXMICROWIN__
    LPNMHDR hdr = (LPNMHDR)lParam;
    HWND hWnd = hdr->hwndFrom;
    wxWindow *win = wxFindWinFromHandle((WXHWND)hWnd);

    // is this one of our windows?
    if ( win )
    {
        return win->MSWOnNotify(idCtrl, lParam, result);
    }

    // try all our children
    wxWindowList::Node *node = GetChildren().GetFirst();
    while ( node )
    {
        wxWindow *child = node->GetData();
        if ( child->MSWOnNotify(idCtrl, lParam, result) )
        {
            return TRUE;
        }

        node = node->GetNext();
    }

    // finally try this window too (catches toolbar case)
    return MSWOnNotify(idCtrl, lParam, result);
#else // __WXMICROWIN__
    return FALSE;
#endif
}

bool wxWindowMSW::MSWOnNotify(int WXUNUSED(idCtrl),
                           WXLPARAM lParam,
                           WXLPARAM* WXUNUSED(result))
{
#if wxUSE_TOOLTIPS
    NMHDR* hdr = (NMHDR *)lParam;
    if ( (int)hdr->code == TTN_NEEDTEXT && m_tooltip )
    {
        TOOLTIPTEXT *ttt = (TOOLTIPTEXT *)lParam;
        ttt->lpszText = (wxChar *)m_tooltip->GetTip().c_str();

        // processed
        return TRUE;
    }
#endif // wxUSE_TOOLTIPS

    return FALSE;
}
#endif // __WIN95__

// ---------------------------------------------------------------------------
// end session messages
// ---------------------------------------------------------------------------

bool wxWindowMSW::HandleQueryEndSession(long logOff, bool *mayEnd)
{
    wxCloseEvent event(wxEVT_QUERY_END_SESSION, -1);
    event.SetEventObject(wxTheApp);
    event.SetCanVeto(TRUE);
    event.SetLoggingOff(logOff == (long)ENDSESSION_LOGOFF);

    bool rc = wxTheApp->ProcessEvent(event);

    if ( rc )
    {
        // we may end only if the app didn't veto session closing (double
        // negation...)
        *mayEnd = !event.GetVeto();
    }

    return rc;
}

bool wxWindowMSW::HandleEndSession(bool endSession, long logOff)
{
    // do nothing if the session isn't ending
    if ( !endSession )
        return FALSE;

    wxCloseEvent event(wxEVT_END_SESSION, -1);
    event.SetEventObject(wxTheApp);
    event.SetCanVeto(FALSE);
    event.SetLoggingOff( (logOff == (long)ENDSESSION_LOGOFF) );
    if ( (this == wxTheApp->GetTopWindow()) && // Only send once
        wxTheApp->ProcessEvent(event))
    {
    }
    return TRUE;
}

// ---------------------------------------------------------------------------
// window creation/destruction
// ---------------------------------------------------------------------------

bool wxWindowMSW::HandleCreate(WXLPCREATESTRUCT WXUNUSED(cs), bool *mayCreate)
{
    // TODO: should generate this event from WM_NCCREATE
    wxWindowCreateEvent event((wxWindow *)this);
    (void)GetEventHandler()->ProcessEvent(event);

    *mayCreate = TRUE;

    return TRUE;
}

bool wxWindowMSW::HandleDestroy()
{
    wxWindowDestroyEvent event((wxWindow *)this);
    (void)GetEventHandler()->ProcessEvent(event);

    // delete our drop target if we've got one
#if wxUSE_DRAG_AND_DROP
    if ( m_dropTarget != NULL )
    {
        m_dropTarget->Revoke(m_hWnd);

        delete m_dropTarget;
        m_dropTarget = NULL;
    }
#endif // wxUSE_DRAG_AND_DROP

    // WM_DESTROY handled
    return TRUE;
}

// ---------------------------------------------------------------------------
// activation/focus
// ---------------------------------------------------------------------------

void wxWindowMSW::OnSetFocus(wxFocusEvent& event)
{
    // panel wants to track the window which was the last to have focus in it,
    // so we want to set ourselves as the window which last had focus
    //
    // notice that it's also important to do it upwards the tree becaus
    // otherwise when the top level panel gets focus, it won't set it back to
    // us, but to some other sibling
    wxWindow *win = (wxWindow *)this;
    while ( win )
    {
        wxWindow *parent = win->GetParent();
        wxPanel *panel = wxDynamicCast(parent, wxPanel);
        if ( panel )
        {
            panel->SetLastFocus(win);
        }

        win = parent;
    }

    wxLogTrace(_T("focus"), _T("%s (0x%08x) gets focus"),
               GetClassInfo()->GetClassName(), GetHandle());

    event.Skip();
}

bool wxWindowMSW::HandleActivate(int state,
                              bool WXUNUSED(minimized),
                              WXHWND WXUNUSED(activate))
{
    wxActivateEvent event(wxEVT_ACTIVATE,
                          (state == WA_ACTIVE) || (state == WA_CLICKACTIVE),
                          m_windowId);
    event.SetEventObject(this);

    return GetEventHandler()->ProcessEvent(event);
}

bool wxWindowMSW::HandleSetFocus(WXHWND hwnd)
{
#if wxUSE_CARET
    // Deal with caret
    if ( m_caret )
    {
        m_caret->OnSetFocus();
    }
#endif // wxUSE_CARET

    wxFocusEvent event(wxEVT_SET_FOCUS, m_windowId);
    event.SetEventObject(this);

    // wxFindWinFromHandle() may return NULL, it is ok
    event.SetWindow(wxFindWinFromHandle(hwnd));

    return GetEventHandler()->ProcessEvent(event);
}

bool wxWindowMSW::HandleKillFocus(WXHWND hwnd)
{
#if wxUSE_CARET
    // Deal with caret
    if ( m_caret )
    {
        m_caret->OnKillFocus();
    }
#endif // wxUSE_CARET

    wxFocusEvent event(wxEVT_KILL_FOCUS, m_windowId);
    event.SetEventObject(this);

    // wxFindWinFromHandle() may return NULL, it is ok
    event.SetWindow(wxFindWinFromHandle(hwnd));

    return GetEventHandler()->ProcessEvent(event);
}

// ---------------------------------------------------------------------------
// miscellaneous
// ---------------------------------------------------------------------------

bool wxWindowMSW::HandleShow(bool show, int WXUNUSED(status))
{
    wxShowEvent event(GetId(), show);
    event.m_eventObject = this;

    return GetEventHandler()->ProcessEvent(event);
}

bool wxWindowMSW::HandleInitDialog(WXHWND WXUNUSED(hWndFocus))
{
    wxInitDialogEvent event(GetId());
    event.m_eventObject = this;

    return GetEventHandler()->ProcessEvent(event);
}

bool wxWindowMSW::HandleDropFiles(WXWPARAM wParam)
{
#ifndef __WXMICROWIN__
    HDROP hFilesInfo = (HDROP) wParam;
    POINT dropPoint;
    DragQueryPoint(hFilesInfo, (LPPOINT) &dropPoint);

    // Get the total number of files dropped
    WORD gwFilesDropped = (WORD)::DragQueryFile
                            (
                                (HDROP)hFilesInfo,
                                (UINT)-1,
                                (LPTSTR)0,
                                (UINT)0
                            );

    wxString *files = new wxString[gwFilesDropped];
    int wIndex;
    for (wIndex=0; wIndex < (int)gwFilesDropped; wIndex++)
    {
        DragQueryFile (hFilesInfo, wIndex, (LPTSTR) wxBuffer, 1000);
        files[wIndex] = wxBuffer;
    }
    DragFinish (hFilesInfo);

    wxDropFilesEvent event(wxEVT_DROP_FILES, gwFilesDropped, files);
    event.m_eventObject = this;
    event.m_pos.x = dropPoint.x; event.m_pos.x = dropPoint.y;

    bool rc = GetEventHandler()->ProcessEvent(event);

    delete[] files;

    return rc;
#else // __WXMICROWIN__
    return FALSE;
#endif
}

bool wxWindowMSW::HandleSetCursor(WXHWND WXUNUSED(hWnd),
                                  short nHitTest,
                                  int WXUNUSED(mouseMsg))
{
#ifndef __WXMICROWIN__
    // the logic is as follows:
    // -1. don't set cursor for non client area, including but not limited to
    //     the title bar, scrollbars, &c
    //  0. allow the user to override default behaviour by using EVT_SET_CURSOR
    //  1. if we have the cursor set it unless wxIsBusy()
    //  2. if we're a top level window, set some cursor anyhow
    //  3. if wxIsBusy(), set the busy cursor, otherwise the global one

    if ( nHitTest != HTCLIENT )
    {
        return FALSE;
    }

    HCURSOR hcursor = 0;

    // first ask the user code - it may wish to set the cursor in some very
    // specific way (for example, depending on the current position)
    POINT pt;
#ifdef __WIN32__
    if ( !::GetCursorPos(&pt) )
    {
        wxLogLastError(wxT("GetCursorPos"));
    }
#else
    // In WIN16 it doesn't return a value.
    ::GetCursorPos(&pt);
#endif

    int x = pt.x,
        y = pt.y;
    ScreenToClient(&x, &y);
    wxSetCursorEvent event(x, y);

    bool processedEvtSetCursor = GetEventHandler()->ProcessEvent(event);
    if ( processedEvtSetCursor && event.HasCursor() )
    {
        hcursor = GetHcursorOf(event.GetCursor());
    }

    if ( !hcursor )
    {
        bool isBusy = wxIsBusy();

        // the test for processedEvtSetCursor is here to prevent using m_cursor
        // if the user code caught EVT_SET_CURSOR() and returned nothing from
        // it - this is a way to say that our cursor shouldn't be used for this
        // point
        if ( !processedEvtSetCursor && m_cursor.Ok() )
        {
            hcursor = GetHcursorOf(m_cursor);
        }

        if ( !GetParent() )
        {
            if ( isBusy )
            {
                hcursor = wxGetCurrentBusyCursor();
            }
            else if ( !hcursor )
            {
                const wxCursor *cursor = wxGetGlobalCursor();
                if ( cursor && cursor->Ok() )
                {
                    hcursor = GetHcursorOf(*cursor);
                }
            }
        }
    }

    if ( hcursor )
    {
        ::SetCursor(hcursor);

        // cursor set, stop here
        return TRUE;
    }
#endif // __WXMICROWIN__

    // pass up the window chain
    return FALSE;
}

// ---------------------------------------------------------------------------
// owner drawn stuff
// ---------------------------------------------------------------------------

bool wxWindowMSW::MSWOnDrawItem(int id, WXDRAWITEMSTRUCT *itemStruct)
{
#if wxUSE_OWNER_DRAWN

#if wxUSE_MENUS_NATIVE
    // is it a menu item?
    if ( id == 0 )
    {
        DRAWITEMSTRUCT *pDrawStruct = (DRAWITEMSTRUCT *)itemStruct;
        wxMenuItem *pMenuItem = (wxMenuItem *)(pDrawStruct->itemData);

        wxCHECK( pMenuItem->IsKindOf(CLASSINFO(wxMenuItem)), FALSE );

        // prepare to call OnDrawItem(): notice using of wxDCTemp to prevent
        // the DC from being released
        wxDCTemp dc((WXHDC)pDrawStruct->hDC);
        wxRect rect(pDrawStruct->rcItem.left, pDrawStruct->rcItem.top,
                    pDrawStruct->rcItem.right - pDrawStruct->rcItem.left,
                    pDrawStruct->rcItem.bottom - pDrawStruct->rcItem.top);

        return pMenuItem->OnDrawItem
               (
                dc,
                rect,
                (wxOwnerDrawn::wxODAction)pDrawStruct->itemAction,
                (wxOwnerDrawn::wxODStatus)pDrawStruct->itemState
               );
    }
#endif // wxUSE_MENUS_NATIVE

#if wxUSE_CONTROLS
    wxWindow *item = FindItem(id);
    if ( item && item->IsKindOf(CLASSINFO(wxControl)) )
    {
        return ((wxControl *)item)->MSWOnDraw(itemStruct);
    }
#endif // wxUSE_CONTROLS

#endif // USE_OWNER_DRAWN

    return FALSE;
}

bool wxWindowMSW::MSWOnMeasureItem(int id, WXMEASUREITEMSTRUCT *itemStruct)
{
#if wxUSE_OWNER_DRAWN
    // is it a menu item?
    if ( id == 0 )
    {
        MEASUREITEMSTRUCT *pMeasureStruct = (MEASUREITEMSTRUCT *)itemStruct;
        wxMenuItem *pMenuItem = (wxMenuItem *)(pMeasureStruct->itemData);

        wxCHECK( pMenuItem->IsKindOf(CLASSINFO(wxMenuItem)), FALSE );

        return pMenuItem->OnMeasureItem(&pMeasureStruct->itemWidth,
                                        &pMeasureStruct->itemHeight);
    }

    wxWindow *item = FindItem(id);
    if ( item && item->IsKindOf(CLASSINFO(wxControl)) )
    {
        return ((wxControl *)item)->MSWOnMeasure(itemStruct);
    }
#endif  // owner-drawn menus
    return FALSE;
}

// ---------------------------------------------------------------------------
// colours and palettes
// ---------------------------------------------------------------------------

bool wxWindowMSW::HandleSysColorChange()
{
    wxSysColourChangedEvent event;
    event.SetEventObject(this);

    return GetEventHandler()->ProcessEvent(event);
}

bool wxWindowMSW::HandleCtlColor(WXHBRUSH *brush,
                              WXHDC pDC,
                              WXHWND pWnd,
                              WXUINT nCtlColor,
                              WXUINT message,
                              WXWPARAM wParam,
                              WXLPARAM lParam)
{
#ifndef __WXMICROWIN__
    WXHBRUSH hBrush = 0;

    if ( nCtlColor == CTLCOLOR_DLG )
    {
        hBrush = OnCtlColor(pDC, pWnd, nCtlColor, message, wParam, lParam);
    }
#if wxUSE_CONTROLS
    else
    {
        wxControl *item = (wxControl *)FindItemByHWND(pWnd, TRUE);
        if ( item )
            hBrush = item->OnCtlColor(pDC, pWnd, nCtlColor, message, wParam, lParam);
    }
#endif // wxUSE_CONTROLS

    if ( hBrush )
        *brush = hBrush;

    return hBrush != 0;
#else // __WXMICROWIN__
    return FALSE;
#endif
}

// Define for each class of dialog and control
WXHBRUSH wxWindowMSW::OnCtlColor(WXHDC WXUNUSED(hDC),
                                 WXHWND WXUNUSED(hWnd),
                                 WXUINT WXUNUSED(nCtlColor),
                                 WXUINT WXUNUSED(message),
                                 WXWPARAM WXUNUSED(wParam),
                                 WXLPARAM WXUNUSED(lParam))
{
    return (WXHBRUSH)0;
}

bool wxWindowMSW::HandlePaletteChanged(WXHWND hWndPalChange)
{
    wxPaletteChangedEvent event(GetId());
    event.SetEventObject(this);
    event.SetChangedWindow(wxFindWinFromHandle(hWndPalChange));

    return GetEventHandler()->ProcessEvent(event);
}

bool wxWindowMSW::HandleQueryNewPalette()
{
    wxQueryNewPaletteEvent event(GetId());
    event.SetEventObject(this);

    return GetEventHandler()->ProcessEvent(event) && event.GetPaletteRealized();
}

// Responds to colour changes: passes event on to children.
void wxWindowMSW::OnSysColourChanged(wxSysColourChangedEvent& event)
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

// ---------------------------------------------------------------------------
// painting
// ---------------------------------------------------------------------------

bool wxWindowMSW::HandlePaint()
{
#ifdef __WIN32__
    HRGN hRegion = ::CreateRectRgn(0, 0, 0, 0); // Dummy call to get a handle
    if ( !hRegion )
        wxLogLastError(wxT("CreateRectRgn"));
    if ( ::GetUpdateRgn(GetHwnd(), hRegion, FALSE) == ERROR )
        wxLogLastError(wxT("GetUpdateRgn"));

    m_updateRegion = wxRegion((WXHRGN) hRegion);
#else // Win16
    RECT updateRect;
    ::GetUpdateRect(GetHwnd(), &updateRect, FALSE);

    m_updateRegion = wxRegion(updateRect.left, updateRect.top,
                              updateRect.right - updateRect.left,
                              updateRect.bottom - updateRect.top);
#endif // Win32/16

    wxPaintEvent event(m_windowId);
    event.SetEventObject(this);

    bool processed = GetEventHandler()->ProcessEvent(event);

    // note that we must generate NC event after the normal one as otherwise
    // BeginPaint() will happily overwrite our decorations with the background
    // colour
    wxNcPaintEvent eventNc(m_windowId);
    eventNc.SetEventObject(this);
    GetEventHandler()->ProcessEvent(eventNc);

    return processed;
}

// Can be called from an application's OnPaint handler
void wxWindowMSW::OnPaint(wxPaintEvent& event)
{
#ifdef __WXUNIVERSAL__
    event.Skip();
#else
    HDC hDC = (HDC) wxPaintDC::FindDCInCache((wxWindow*) event.GetEventObject());
    if (hDC != 0)
    {
        MSWDefWindowProc(WM_PAINT, (WPARAM) hDC, 0);
    }
#endif
}

bool wxWindowMSW::HandleEraseBkgnd(WXHDC hdc)
{
    // Prevents flicker when dragging
    if ( ::IsIconic(GetHwnd()) )
        return TRUE;

    wxDCTemp dc(hdc);

    dc.SetHDC(hdc);
    dc.SetWindow((wxWindow *)this);
    dc.BeginDrawing();

    wxEraseEvent event(m_windowId, &dc);
    event.SetEventObject(this);
    bool rc = GetEventHandler()->ProcessEvent(event);

    dc.EndDrawing();

    // must be called manually as ~wxDC doesn't do anything for wxDCTemp
    dc.SelectOldObjects(hdc);

    return rc;
}

void wxWindowMSW::OnEraseBackground(wxEraseEvent& event)
{
    RECT rect;
    ::GetClientRect(GetHwnd(), &rect);

    COLORREF ref = PALETTERGB(m_backgroundColour.Red(),
                              m_backgroundColour.Green(),
                              m_backgroundColour.Blue());
    HBRUSH hBrush = ::CreateSolidBrush(ref);
    if ( !hBrush )
        wxLogLastError(wxT("CreateSolidBrush"));

    HDC hdc = (HDC)event.GetDC()->GetHDC();

    int mode = ::SetMapMode(hdc, MM_TEXT);

    ::FillRect(hdc, &rect, hBrush);
    ::DeleteObject(hBrush);
    ::SetMapMode(hdc, mode);
}

// ---------------------------------------------------------------------------
// moving and resizing
// ---------------------------------------------------------------------------

bool wxWindowMSW::HandleMinimize()
{
    wxIconizeEvent event(m_windowId);
    event.SetEventObject(this);

    return GetEventHandler()->ProcessEvent(event);
}

bool wxWindowMSW::HandleMaximize()
{
    wxMaximizeEvent event(m_windowId);
    event.SetEventObject(this);

    return GetEventHandler()->ProcessEvent(event);
}

bool wxWindowMSW::HandleMove(int x, int y)
{
    wxMoveEvent event(wxPoint(x, y), m_windowId);
    event.SetEventObject(this);

    return GetEventHandler()->ProcessEvent(event);
}

bool wxWindowMSW::HandleSize(int w, int h, WXUINT WXUNUSED(flag))
{
    wxSizeEvent event(wxSize(w, h), m_windowId);
    event.SetEventObject(this);

    return GetEventHandler()->ProcessEvent(event);
}

bool wxWindowMSW::HandleGetMinMaxInfo(void *mmInfo)
{
    MINMAXINFO *info = (MINMAXINFO *)mmInfo;

    bool rc = FALSE;

    if ( m_minWidth != -1 )
    {
        info->ptMinTrackSize.x = m_minWidth;
        rc = TRUE;
    }

    if ( m_minHeight != -1 )
    {
        info->ptMinTrackSize.y = m_minHeight;
        rc = TRUE;
    }

    if ( m_maxWidth != -1 )
    {
        info->ptMaxTrackSize.x = m_maxWidth;
        rc = TRUE;
    }

    if ( m_maxHeight != -1 )
    {
        info->ptMaxTrackSize.y = m_maxHeight;
        rc = TRUE;
    }

    return rc;
}

// ---------------------------------------------------------------------------
// command messages
// ---------------------------------------------------------------------------

bool wxWindowMSW::HandleCommand(WXWORD id, WXWORD cmd, WXHWND control)
{
#if wxUSE_MENUS_NATIVE
    if ( wxCurrentPopupMenu )
    {
        wxMenu *popupMenu = wxCurrentPopupMenu;
        wxCurrentPopupMenu = NULL;

        return popupMenu->MSWCommand(cmd, id);
    }
#endif // wxUSE_MENUS_NATIVE

    wxWindow *win = (wxWindow*) NULL;
    if ( cmd == 0 || cmd == 1 ) // menu or accel - use id
    {
        // must cast to a signed type before comparing with other ids!
        win = FindItem((signed short)id);
    }

    if (!win && control)
    {
        // find it from HWND - this works even with the broken programs using
        // the same ids for different controls
        win = wxFindWinFromHandle(control);
    }

    if ( win )
    {
        return win->MSWCommand(cmd, id);
    }

    // the messages sent from the in-place edit control used by the treectrl
    // for label editing have id == 0, but they should _not_ be treated as menu
    // messages (they are EN_XXX ones, in fact) so don't translate anything
    // coming from a control to wxEVT_COMMAND_MENU_SELECTED
    if ( !control )
    {
        // If no child window, it may be an accelerator, e.g. for a popup menu
        // command

        wxCommandEvent event(wxEVT_COMMAND_MENU_SELECTED);
        event.SetEventObject(this);
        event.SetId(id);
        event.SetInt(id);

        return GetEventHandler()->ProcessEvent(event);
    }
#if wxUSE_SPINCTRL && !defined(__WXUNIVERSAL__)
    else
    {
        // the text ctrl which is logically part of wxSpinCtrl sends WM_COMMAND
        // notifications to its parent which we want to reflect back to
        // wxSpinCtrl
        wxSpinCtrl *spin = wxSpinCtrl::GetSpinForTextCtrl(control);
        if ( spin && spin->ProcessTextCommand(cmd, id) )
            return TRUE;
    }
#endif // wxUSE_SPINCTRL

    return FALSE;
}

bool wxWindowMSW::HandleSysCommand(WXWPARAM wParam, WXLPARAM WXUNUSED(lParam))
{
    // 4 bits are reserved
    switch ( wParam & 0xFFFFFFF0 )
    {
        case SC_MAXIMIZE:
            return HandleMaximize();

        case SC_MINIMIZE:
            return HandleMinimize();
    }

    return FALSE;
}

// ---------------------------------------------------------------------------
// mouse events
// ---------------------------------------------------------------------------

void wxWindowMSW::InitMouseEvent(wxMouseEvent& event,
                                 int x, int y,
                                 WXUINT flags)
{
    // our client coords are not quite the same as Windows ones
    wxPoint pt = GetClientAreaOrigin();
    event.m_x = x - pt.x;
    event.m_y = y - pt.y;

    event.m_shiftDown = (flags & MK_SHIFT) != 0;
    event.m_controlDown = (flags & MK_CONTROL) != 0;
    event.m_leftDown = (flags & MK_LBUTTON) != 0;
    event.m_middleDown = (flags & MK_MBUTTON) != 0;
    event.m_rightDown = (flags & MK_RBUTTON) != 0;
    event.m_altDown = (::GetKeyState(VK_MENU) & 0x80000000) != 0;

    event.SetTimestamp(s_currentMsg.time);
    event.m_eventObject = this;

#if wxUSE_MOUSEEVENT_HACK
    m_lastMouseX = x;
    m_lastMouseY = y;
    m_lastMouseEvent = event.GetEventType();
#endif // wxUSE_MOUSEEVENT_HACK
}

bool wxWindowMSW::HandleMouseEvent(WXUINT msg, int x, int y, WXUINT flags)
{
    // the mouse events take consecutive IDs from WM_MOUSEFIRST to
    // WM_MOUSELAST, so it's enough to substract WM_MOUSEMOVE == WM_MOUSEFIRST
    // from the message id and take the value in the table to get wxWin event
    // id
    static const wxEventType eventsMouse[] =
    {
        wxEVT_MOTION,
        wxEVT_LEFT_DOWN,
        wxEVT_LEFT_UP,
        wxEVT_LEFT_DCLICK,
        wxEVT_RIGHT_DOWN,
        wxEVT_RIGHT_UP,
        wxEVT_RIGHT_DCLICK,
        wxEVT_MIDDLE_DOWN,
        wxEVT_MIDDLE_UP,
        wxEVT_MIDDLE_DCLICK
    };

    wxMouseEvent event(eventsMouse[msg - WM_MOUSEMOVE]);
    InitMouseEvent(event, x, y, flags);

    return GetEventHandler()->ProcessEvent(event);
}

bool wxWindowMSW::HandleMouseMove(int x, int y, WXUINT flags)
{
    if ( !m_mouseInWindow )
    {
        // it would be wrong to assume that just because we get a mouse move
        // event that the mouse is inside the window: although this is usually
        // true, it is not if we had captured the mouse, so we need to check
        // the mouse coordinates here
        if ( !HasCapture() || IsMouseInWindow() )
        {
            // Generate an ENTER event
            m_mouseInWindow = TRUE;

            wxMouseEvent event(wxEVT_ENTER_WINDOW);
            InitMouseEvent(event, x, y, flags);

            (void)GetEventHandler()->ProcessEvent(event);
        }
    }

#if wxUSE_MOUSEEVENT_HACK
    // Window gets a click down message followed by a mouse move message even
    // if position isn't changed!  We want to discard the trailing move event
    // if x and y are the same.
    if ( (m_lastMouseEvent == wxEVT_RIGHT_DOWN ||
          m_lastMouseEvent == wxEVT_LEFT_DOWN ||
          m_lastMouseEvent == wxEVT_MIDDLE_DOWN) &&
         (m_lastMouseX == x && m_lastMouseY == y) )
    {
        m_lastMouseEvent = wxEVT_MOTION;

        return FALSE;
    }
#endif // wxUSE_MOUSEEVENT_HACK

    return HandleMouseEvent(WM_MOUSEMOVE, x, y, flags);
}


bool wxWindowMSW::HandleMouseWheel(WXWPARAM wParam, WXLPARAM lParam)
{
#if wxUSE_MOUSEWHEEL
    wxMouseEvent event(wxEVT_MOUSEWHEEL);
    InitMouseEvent(event,
                   GET_X_LPARAM(lParam),
                   GET_Y_LPARAM(lParam),
                   LOWORD(wParam));
    event.m_wheelRotation = (short)HIWORD(wParam);
    event.m_wheelDelta = WHEEL_DELTA;

#ifdef __WIN32__
    static int s_linesPerRotation = -1;
    if ( s_linesPerRotation == -1 )
    {
        if ( !::SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0,
                                     &s_linesPerRotation, 0))
        {
            // this is not supposed to happen
            wxLogLastError(_T("SystemParametersInfo(GETWHEELSCROLLLINES)"));

            // the default is 3, so use it if SystemParametersInfo() failed
            s_linesPerRotation = 3;
        }
    }
#else // Win16
    // no SystemParametersInfo() under Win16
    static const int s_linesPerRotation = 3;
#endif

    event.m_linesPerAction = s_linesPerRotation;
    return GetEventHandler()->ProcessEvent(event);

#else
    (void) wParam;
    (void) lParam;

    return FALSE;
#endif
}


// ---------------------------------------------------------------------------
// keyboard handling
// ---------------------------------------------------------------------------

// create the key event of the given type for the given key - used by
// HandleChar and HandleKeyDown/Up
wxKeyEvent wxWindowMSW::CreateKeyEvent(wxEventType evType,
                                    int id,
                                    WXLPARAM lParam) const
{
    wxKeyEvent event(evType);
    event.SetId(GetId());
    event.m_shiftDown = wxIsShiftDown();
    event.m_controlDown = wxIsCtrlDown();
    event.m_altDown = (HIWORD(lParam) & KF_ALTDOWN) == KF_ALTDOWN;

    event.m_eventObject = (wxWindow *)this; // const_cast
    event.m_keyCode = id;
    event.SetTimestamp(s_currentMsg.time);

    // translate the position to client coords
    POINT pt;
    GetCursorPos(&pt);
    RECT rect;
    GetWindowRect(GetHwnd(),&rect);
    pt.x -= rect.left;
    pt.y -= rect.top;

    event.m_x = pt.x;
    event.m_y = pt.y;

    return event;
}

// isASCII is TRUE only when we're called from WM_CHAR handler and not from
// WM_KEYDOWN one
bool wxWindowMSW::HandleChar(WXWPARAM wParam, WXLPARAM lParam, bool isASCII)
{
    bool ctrlDown = FALSE;

    int id;
    if ( isASCII )
    {
        // If 1 -> 26, translate to CTRL plus a letter.
        id = wParam;
        if ( (id > 0) && (id < 27) )
        {
            switch (id)
            {
                case 13:
                    id = WXK_RETURN;
                    break;

                case 8:
                    id = WXK_BACK;
                    break;

                case 9:
                    id = WXK_TAB;
                    break;

                default:
                    ctrlDown = TRUE;
                    id = id + 96;
            }
        }
    }
    else if ( (id = wxCharCodeMSWToWX(wParam)) == 0 )
    {
        // it's ASCII and will be processed here only when called from
        // WM_CHAR (i.e. when isASCII = TRUE), don't process it now
        id = -1;
    }

    if ( id != -1 )
    {
        wxKeyEvent event(CreateKeyEvent(wxEVT_CHAR, id, lParam));
        if ( ctrlDown )
        {
            event.m_controlDown = TRUE;
        }

        if ( GetEventHandler()->ProcessEvent(event) )
            return TRUE;
    }

    return FALSE;
}

bool wxWindowMSW::HandleKeyDown(WXWPARAM wParam, WXLPARAM lParam)
{
    int id = wxCharCodeMSWToWX(wParam);

    if ( !id )
    {
        // normal ASCII char
        id = wParam;
    }

    if ( id != -1 ) // VZ: does this ever happen (FIXME)?
    {
        wxKeyEvent event(CreateKeyEvent(wxEVT_KEY_DOWN, id, lParam));
        if ( GetEventHandler()->ProcessEvent(event) )
        {
            return TRUE;
        }
    }

    return FALSE;
}

bool wxWindowMSW::HandleKeyUp(WXWPARAM wParam, WXLPARAM lParam)
{
    int id = wxCharCodeMSWToWX(wParam);

    if ( !id )
    {
        // normal ASCII char
        id = wParam;
    }

    if ( id != -1 ) // VZ: does this ever happen (FIXME)?
    {
        wxKeyEvent event(CreateKeyEvent(wxEVT_KEY_UP, id, lParam));
        if ( GetEventHandler()->ProcessEvent(event) )
            return TRUE;
    }

    return FALSE;
}

// ---------------------------------------------------------------------------
// joystick
// ---------------------------------------------------------------------------

bool wxWindowMSW::HandleJoystickEvent(WXUINT msg, int x, int y, WXUINT flags)
{
#ifdef JOY_BUTTON1
    int change = 0;
    if ( flags & JOY_BUTTON1CHG )
        change = wxJOY_BUTTON1;
    if ( flags & JOY_BUTTON2CHG )
        change = wxJOY_BUTTON2;
    if ( flags & JOY_BUTTON3CHG )
        change = wxJOY_BUTTON3;
    if ( flags & JOY_BUTTON4CHG )
        change = wxJOY_BUTTON4;

    int buttons = 0;
    if ( flags & JOY_BUTTON1 )
        buttons |= wxJOY_BUTTON1;
    if ( flags & JOY_BUTTON2 )
        buttons |= wxJOY_BUTTON2;
    if ( flags & JOY_BUTTON3 )
        buttons |= wxJOY_BUTTON3;
    if ( flags & JOY_BUTTON4 )
        buttons |= wxJOY_BUTTON4;

    // the event ids aren't consecutive so we can't use table based lookup
    int joystick;
    wxEventType eventType;
    switch ( msg )
    {
        case MM_JOY1MOVE:
            joystick = 1;
            eventType = wxEVT_JOY_MOVE;
            break;

        case MM_JOY2MOVE:
            joystick = 2;
            eventType = wxEVT_JOY_MOVE;
            break;

        case MM_JOY1ZMOVE:
            joystick = 1;
            eventType = wxEVT_JOY_ZMOVE;
            break;

        case MM_JOY2ZMOVE:
            joystick = 2;
            eventType = wxEVT_JOY_ZMOVE;
            break;

        case MM_JOY1BUTTONDOWN:
            joystick = 1;
            eventType = wxEVT_JOY_BUTTON_DOWN;
            break;

        case MM_JOY2BUTTONDOWN:
            joystick = 2;
            eventType = wxEVT_JOY_BUTTON_DOWN;
            break;

        case MM_JOY1BUTTONUP:
            joystick = 1;
            eventType = wxEVT_JOY_BUTTON_UP;
            break;

        case MM_JOY2BUTTONUP:
            joystick = 2;
            eventType = wxEVT_JOY_BUTTON_UP;
            break;

        default:
            wxFAIL_MSG(wxT("no such joystick event"));

            return FALSE;
    }

    wxJoystickEvent event(eventType, buttons, joystick, change);
    event.SetPosition(wxPoint(x, y));
    event.SetEventObject(this);

    return GetEventHandler()->ProcessEvent(event);
#else
    return FALSE;
#endif
}

// ---------------------------------------------------------------------------
// scrolling
// ---------------------------------------------------------------------------

bool wxWindowMSW::MSWOnScroll(int orientation, WXWORD wParam,
                           WXWORD pos, WXHWND control)
{
    if ( control )
    {
        wxWindow *child = wxFindWinFromHandle(control);
        if ( child )
            return child->MSWOnScroll(orientation, wParam, pos, control);
    }

    wxScrollWinEvent event;
    event.SetPosition(pos);
    event.SetOrientation(orientation);
    event.m_eventObject = this;

    switch ( wParam )
    {
    case SB_TOP:
        event.m_eventType = wxEVT_SCROLLWIN_TOP;
        break;

    case SB_BOTTOM:
        event.m_eventType = wxEVT_SCROLLWIN_BOTTOM;
        break;

    case SB_LINEUP:
        event.m_eventType = wxEVT_SCROLLWIN_LINEUP;
        break;

    case SB_LINEDOWN:
        event.m_eventType = wxEVT_SCROLLWIN_LINEDOWN;
        break;

    case SB_PAGEUP:
        event.m_eventType = wxEVT_SCROLLWIN_PAGEUP;
        break;

    case SB_PAGEDOWN:
        event.m_eventType = wxEVT_SCROLLWIN_PAGEDOWN;
        break;

    case SB_THUMBPOSITION:
    case SB_THUMBTRACK:
#ifdef __WIN32__
        // under Win32, the scrollbar range and position are 32 bit integers,
        // but WM_[HV]SCROLL only carry the low 16 bits of them, so we must
        // explicitly query the scrollbar for the correct position (this must
        // be done only for these two SB_ events as they are the only one
        // carrying the scrollbar position)
        {
            SCROLLINFO scrollInfo;
            wxZeroMemory(scrollInfo);
            scrollInfo.cbSize = sizeof(SCROLLINFO);
            scrollInfo.fMask = SIF_TRACKPOS;

            if ( !::GetScrollInfo(GetHwnd(),
                                  orientation == wxHORIZONTAL ? SB_HORZ
                                                              : SB_VERT,
                                  &scrollInfo) )
            {
                wxLogLastError(_T("GetScrollInfo"));
            }

            event.SetPosition(scrollInfo.nTrackPos);
        }
#endif // Win32

        event.m_eventType = wParam == SB_THUMBPOSITION
                                ? wxEVT_SCROLLWIN_THUMBRELEASE
                                : wxEVT_SCROLLWIN_THUMBTRACK;
        break;

    default:
        return FALSE;
    }

    return GetEventHandler()->ProcessEvent(event);
}

// ===========================================================================
// global functions
// ===========================================================================

void wxGetCharSize(WXHWND wnd, int *x, int *y, const wxFont *the_font)
{
    TEXTMETRIC tm;
    HDC dc = ::GetDC((HWND) wnd);
    HFONT fnt =0;
    HFONT was = 0;
    if ( the_font )
    {
        //    the_font->UseResource();
        //    the_font->RealizeResource();
        fnt = (HFONT)((wxFont *)the_font)->GetResourceHandle(); // const_cast
        if ( fnt )
            was = (HFONT) SelectObject(dc,fnt);
    }
    GetTextMetrics(dc, &tm);
    if ( the_font && fnt && was )
    {
        SelectObject(dc,was);
    }
    ReleaseDC((HWND)wnd, dc);

    if ( x )
        *x = tm.tmAveCharWidth;
    if ( y )
        *y = tm.tmHeight + tm.tmExternalLeading;

    //  if ( the_font )
    //    the_font->ReleaseResource();
}

// Returns 0 if was a normal ASCII value, not a special key. This indicates that
// the key should be ignored by WM_KEYDOWN and processed by WM_CHAR instead.
int wxCharCodeMSWToWX(int keySym)
{
    int id;
    switch (keySym)
    {
        case VK_CANCEL:     id = WXK_CANCEL; break;
        case VK_BACK:       id = WXK_BACK; break;
        case VK_TAB:        id = WXK_TAB; break;
        case VK_CLEAR:      id = WXK_CLEAR; break;
        case VK_RETURN:     id = WXK_RETURN; break;
        case VK_SHIFT:      id = WXK_SHIFT; break;
        case VK_CONTROL:    id = WXK_CONTROL; break;
        case VK_MENU :      id = WXK_MENU; break;
        case VK_PAUSE:      id = WXK_PAUSE; break;
        case VK_SPACE:      id = WXK_SPACE; break;
        case VK_ESCAPE:     id = WXK_ESCAPE; break;
        case VK_PRIOR:      id = WXK_PRIOR; break;
        case VK_NEXT :      id = WXK_NEXT; break;
        case VK_END:        id = WXK_END; break;
        case VK_HOME :      id = WXK_HOME; break;
        case VK_LEFT :      id = WXK_LEFT; break;
        case VK_UP:         id = WXK_UP; break;
        case VK_RIGHT:      id = WXK_RIGHT; break;
        case VK_DOWN :      id = WXK_DOWN; break;
        case VK_SELECT:     id = WXK_SELECT; break;
        case VK_PRINT:      id = WXK_PRINT; break;
        case VK_EXECUTE:    id = WXK_EXECUTE; break;
        case VK_INSERT:     id = WXK_INSERT; break;
        case VK_DELETE:     id = WXK_DELETE; break;
        case VK_HELP :      id = WXK_HELP; break;
        case VK_NUMPAD0:    id = WXK_NUMPAD0; break;
        case VK_NUMPAD1:    id = WXK_NUMPAD1; break;
        case VK_NUMPAD2:    id = WXK_NUMPAD2; break;
        case VK_NUMPAD3:    id = WXK_NUMPAD3; break;
        case VK_NUMPAD4:    id = WXK_NUMPAD4; break;
        case VK_NUMPAD5:    id = WXK_NUMPAD5; break;
        case VK_NUMPAD6:    id = WXK_NUMPAD6; break;
        case VK_NUMPAD7:    id = WXK_NUMPAD7; break;
        case VK_NUMPAD8:    id = WXK_NUMPAD8; break;
        case VK_NUMPAD9:    id = WXK_NUMPAD9; break;
        case VK_MULTIPLY:   id = WXK_NUMPAD_MULTIPLY; break;
        case VK_ADD:        id = WXK_NUMPAD_ADD; break;
        case VK_SUBTRACT:   id = WXK_NUMPAD_SUBTRACT; break;
        case VK_DECIMAL:    id = WXK_NUMPAD_DECIMAL; break;
        case VK_DIVIDE:     id = WXK_NUMPAD_DIVIDE; break;
        case VK_F1:         id = WXK_F1; break;
        case VK_F2:         id = WXK_F2; break;
        case VK_F3:         id = WXK_F3; break;
        case VK_F4:         id = WXK_F4; break;
        case VK_F5:         id = WXK_F5; break;
        case VK_F6:         id = WXK_F6; break;
        case VK_F7:         id = WXK_F7; break;
        case VK_F8:         id = WXK_F8; break;
        case VK_F9:         id = WXK_F9; break;
        case VK_F10:        id = WXK_F10; break;
        case VK_F11:        id = WXK_F11; break;
        case VK_F12:        id = WXK_F12; break;
        case VK_F13:        id = WXK_F13; break;
        case VK_F14:        id = WXK_F14; break;
        case VK_F15:        id = WXK_F15; break;
        case VK_F16:        id = WXK_F16; break;
        case VK_F17:        id = WXK_F17; break;
        case VK_F18:        id = WXK_F18; break;
        case VK_F19:        id = WXK_F19; break;
        case VK_F20:        id = WXK_F20; break;
        case VK_F21:        id = WXK_F21; break;
        case VK_F22:        id = WXK_F22; break;
        case VK_F23:        id = WXK_F23; break;
        case VK_F24:        id = WXK_F24; break;
        case VK_NUMLOCK:    id = WXK_NUMLOCK; break;
        case VK_SCROLL:     id = WXK_SCROLL; break;
        default:
            id = 0;
    }

    return id;
}

int wxCharCodeWXToMSW(int id, bool *isVirtual)
{
    *isVirtual = TRUE;
    int keySym = 0;
    switch (id)
    {
    case WXK_CANCEL:    keySym = VK_CANCEL; break;
    case WXK_CLEAR:     keySym = VK_CLEAR; break;
    case WXK_SHIFT:     keySym = VK_SHIFT; break;
    case WXK_CONTROL:   keySym = VK_CONTROL; break;
    case WXK_MENU :     keySym = VK_MENU; break;
    case WXK_PAUSE:     keySym = VK_PAUSE; break;
    case WXK_PRIOR:     keySym = VK_PRIOR; break;
    case WXK_NEXT :     keySym = VK_NEXT; break;
    case WXK_END:       keySym = VK_END; break;
    case WXK_HOME :     keySym = VK_HOME; break;
    case WXK_LEFT :     keySym = VK_LEFT; break;
    case WXK_UP:        keySym = VK_UP; break;
    case WXK_RIGHT:     keySym = VK_RIGHT; break;
    case WXK_DOWN :     keySym = VK_DOWN; break;
    case WXK_SELECT:    keySym = VK_SELECT; break;
    case WXK_PRINT:     keySym = VK_PRINT; break;
    case WXK_EXECUTE:   keySym = VK_EXECUTE; break;
    case WXK_INSERT:    keySym = VK_INSERT; break;
    case WXK_DELETE:    keySym = VK_DELETE; break;
    case WXK_HELP :     keySym = VK_HELP; break;
    case WXK_NUMPAD0:   keySym = VK_NUMPAD0; break;
    case WXK_NUMPAD1:   keySym = VK_NUMPAD1; break;
    case WXK_NUMPAD2:   keySym = VK_NUMPAD2; break;
    case WXK_NUMPAD3:   keySym = VK_NUMPAD3; break;
    case WXK_NUMPAD4:   keySym = VK_NUMPAD4; break;
    case WXK_NUMPAD5:   keySym = VK_NUMPAD5; break;
    case WXK_NUMPAD6:   keySym = VK_NUMPAD6; break;
    case WXK_NUMPAD7:   keySym = VK_NUMPAD7; break;
    case WXK_NUMPAD8:   keySym = VK_NUMPAD8; break;
    case WXK_NUMPAD9:   keySym = VK_NUMPAD9; break;
    case WXK_NUMPAD_MULTIPLY:  keySym = VK_MULTIPLY; break;
    case WXK_NUMPAD_ADD:       keySym = VK_ADD; break;
    case WXK_NUMPAD_SUBTRACT:  keySym = VK_SUBTRACT; break;
    case WXK_NUMPAD_DECIMAL:   keySym = VK_DECIMAL; break;
    case WXK_NUMPAD_DIVIDE:    keySym = VK_DIVIDE; break;
    case WXK_F1:        keySym = VK_F1; break;
    case WXK_F2:        keySym = VK_F2; break;
    case WXK_F3:        keySym = VK_F3; break;
    case WXK_F4:        keySym = VK_F4; break;
    case WXK_F5:        keySym = VK_F5; break;
    case WXK_F6:        keySym = VK_F6; break;
    case WXK_F7:        keySym = VK_F7; break;
    case WXK_F8:        keySym = VK_F8; break;
    case WXK_F9:        keySym = VK_F9; break;
    case WXK_F10:       keySym = VK_F10; break;
    case WXK_F11:       keySym = VK_F11; break;
    case WXK_F12:       keySym = VK_F12; break;
    case WXK_F13:       keySym = VK_F13; break;
    case WXK_F14:       keySym = VK_F14; break;
    case WXK_F15:       keySym = VK_F15; break;
    case WXK_F16:       keySym = VK_F16; break;
    case WXK_F17:       keySym = VK_F17; break;
    case WXK_F18:       keySym = VK_F18; break;
    case WXK_F19:       keySym = VK_F19; break;
    case WXK_F20:       keySym = VK_F20; break;
    case WXK_F21:       keySym = VK_F21; break;
    case WXK_F22:       keySym = VK_F22; break;
    case WXK_F23:       keySym = VK_F23; break;
    case WXK_F24:       keySym = VK_F24; break;
    case WXK_NUMLOCK:   keySym = VK_NUMLOCK; break;
    case WXK_SCROLL:    keySym = VK_SCROLL; break;
    default:
        {
            *isVirtual = FALSE;
            keySym = id;
            break;
        }
    }
    return keySym;
}

wxWindow *wxGetActiveWindow()
{
    HWND hWnd = GetActiveWindow();
    if ( hWnd != 0 )
    {
        return wxFindWinFromHandle((WXHWND) hWnd);
    }
    return NULL;
}

extern wxWindow *wxGetWindowFromHWND(WXHWND hWnd)
{
    HWND hwnd = (HWND)hWnd;

    // For a radiobutton, we get the radiobox from GWL_USERDATA (which is set
    // by code in msw/radiobox.cpp), for all the others we just search up the
    // window hierarchy
    wxWindow *win = (wxWindow *)NULL;
    if ( hwnd )
    {
        win = wxFindWinFromHandle((WXHWND)hwnd);
        if ( !win )
        {
            // all these hacks only work under Win32 anyhow
#ifdef __WIN32__

#if wxUSE_RADIOBOX
            // native radiobuttons return DLGC_RADIOBUTTON here and for any
            // wxWindow class which overrides WM_GETDLGCODE processing to
            // do it as well, win would be already non NULL
            if ( ::SendMessage(hwnd, WM_GETDLGCODE, 0, 0) & DLGC_RADIOBUTTON )
            {
                win = (wxWindow *)::GetWindowLong(hwnd, GWL_USERDATA);
            }
            //else: it's a wxRadioButton, not a radiobutton from wxRadioBox
#endif // wxUSE_RADIOBOX

            // spin control text buddy window should be mapped to spin ctrl
            // itself so try it too
#if wxUSE_SPINCTRL && !defined(__WXUNIVERSAL__)
            if ( !win )
            {
                win = wxSpinCtrl::GetSpinForTextCtrl((WXHWND)hwnd);
            }
#endif // wxUSE_SPINCTRL

#endif // Win32
        }
    }

    while ( hwnd && !win )
    {
        // this is a really ugly hack needed to avoid mistakenly returning the
        // parent frame wxWindow for the find/replace modeless dialog HWND -
        // this, in turn, is needed to call IsDialogMessage() from
        // wxApp::ProcessMessage() as for this we must return NULL from here
        //
        // FIXME: this is clearly not the best way to do it but I think we'll
        //        need to change HWND <-> wxWindow code more heavily than I can
        //        do it now to fix it
        if ( ::GetWindow(hwnd, GW_OWNER) )
        {
            // it's a dialog box, don't go upwards
            break;
        }

        hwnd = ::GetParent(hwnd);
        win = wxFindWinFromHandle((WXHWND)hwnd);
    }

    return win;
}

#ifndef __WXMICROWIN__

// Windows keyboard hook. Allows interception of e.g. F1, ESCAPE
// in active frames and dialogs, regardless of where the focus is.
static HHOOK wxTheKeyboardHook = 0;
static FARPROC wxTheKeyboardHookProc = 0;
int APIENTRY _EXPORT
wxKeyboardHook(int nCode, WORD wParam, DWORD lParam);

void wxSetKeyboardHook(bool doIt)
{
    if ( doIt )
    {
        wxTheKeyboardHookProc = MakeProcInstance((FARPROC) wxKeyboardHook, wxGetInstance());
        wxTheKeyboardHook = SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC) wxTheKeyboardHookProc, wxGetInstance(),

#if defined(__WIN32__) && !defined(__TWIN32__)
            GetCurrentThreadId()
        //      (DWORD)GetCurrentProcess()); // This is another possibility. Which is right?
#else
            GetCurrentTask()
#endif
            );
    }
    else
    {
        UnhookWindowsHookEx(wxTheKeyboardHook);

        // avoids warning about statement with no effect (FreeProcInstance
        // doesn't do anything under Win32)
#if !defined(WIN32) && !defined(_WIN32) && !defined(__WIN32__) && !defined(__NT__) && !defined(__GNUWIN32__)
        FreeProcInstance(wxTheKeyboardHookProc);
#endif
    }
}

int APIENTRY _EXPORT
wxKeyboardHook(int nCode, WORD wParam, DWORD lParam)
{
    DWORD hiWord = HIWORD(lParam);
    if ( nCode != HC_NOREMOVE && ((hiWord & KF_UP) == 0) )
    {
        int id = wxCharCodeMSWToWX(wParam);
        if ( id != 0 )
        {
            wxKeyEvent event(wxEVT_CHAR_HOOK);
            if ( (HIWORD(lParam) & KF_ALTDOWN) == KF_ALTDOWN )
                event.m_altDown = TRUE;

            event.m_eventObject = NULL;
            event.m_keyCode = id;
            event.m_shiftDown = wxIsShiftDown();
            event.m_controlDown = wxIsCtrlDown();
            event.SetTimestamp(s_currentMsg.time);

            wxWindow *win = wxGetActiveWindow();
            wxEvtHandler *handler;
            if ( win )
            {
                handler = win->GetEventHandler();
                event.SetId(win->GetId());
            }
            else
            {
                handler = wxTheApp;
                event.SetId(-1);
            }

            if ( handler && handler->ProcessEvent(event) )
            {
                // processed
                return 1;
            }
        }
    }

    return (int)CallNextHookEx(wxTheKeyboardHook, nCode, wParam, lParam);
}

#endif // !__WXMICROWIN__

#ifdef __WXDEBUG__
const char *wxGetMessageName(int message)
{
    switch ( message )
    {
        case 0x0000: return "WM_NULL";
        case 0x0001: return "WM_CREATE";
        case 0x0002: return "WM_DESTROY";
        case 0x0003: return "WM_MOVE";
        case 0x0005: return "WM_SIZE";
        case 0x0006: return "WM_ACTIVATE";
        case 0x0007: return "WM_SETFOCUS";
        case 0x0008: return "WM_KILLFOCUS";
        case 0x000A: return "WM_ENABLE";
        case 0x000B: return "WM_SETREDRAW";
        case 0x000C: return "WM_SETTEXT";
        case 0x000D: return "WM_GETTEXT";
        case 0x000E: return "WM_GETTEXTLENGTH";
        case 0x000F: return "WM_PAINT";
        case 0x0010: return "WM_CLOSE";
        case 0x0011: return "WM_QUERYENDSESSION";
        case 0x0012: return "WM_QUIT";
        case 0x0013: return "WM_QUERYOPEN";
        case 0x0014: return "WM_ERASEBKGND";
        case 0x0015: return "WM_SYSCOLORCHANGE";
        case 0x0016: return "WM_ENDSESSION";
        case 0x0017: return "WM_SYSTEMERROR";
        case 0x0018: return "WM_SHOWWINDOW";
        case 0x0019: return "WM_CTLCOLOR";
        case 0x001A: return "WM_WININICHANGE";
        case 0x001B: return "WM_DEVMODECHANGE";
        case 0x001C: return "WM_ACTIVATEAPP";
        case 0x001D: return "WM_FONTCHANGE";
        case 0x001E: return "WM_TIMECHANGE";
        case 0x001F: return "WM_CANCELMODE";
        case 0x0020: return "WM_SETCURSOR";
        case 0x0021: return "WM_MOUSEACTIVATE";
        case 0x0022: return "WM_CHILDACTIVATE";
        case 0x0023: return "WM_QUEUESYNC";
        case 0x0024: return "WM_GETMINMAXINFO";
        case 0x0026: return "WM_PAINTICON";
        case 0x0027: return "WM_ICONERASEBKGND";
        case 0x0028: return "WM_NEXTDLGCTL";
        case 0x002A: return "WM_SPOOLERSTATUS";
        case 0x002B: return "WM_DRAWITEM";
        case 0x002C: return "WM_MEASUREITEM";
        case 0x002D: return "WM_DELETEITEM";
        case 0x002E: return "WM_VKEYTOITEM";
        case 0x002F: return "WM_CHARTOITEM";
        case 0x0030: return "WM_SETFONT";
        case 0x0031: return "WM_GETFONT";
        case 0x0037: return "WM_QUERYDRAGICON";
        case 0x0039: return "WM_COMPAREITEM";
        case 0x0041: return "WM_COMPACTING";
        case 0x0044: return "WM_COMMNOTIFY";
        case 0x0046: return "WM_WINDOWPOSCHANGING";
        case 0x0047: return "WM_WINDOWPOSCHANGED";
        case 0x0048: return "WM_POWER";

#ifdef  __WIN32__
        case 0x004A: return "WM_COPYDATA";
        case 0x004B: return "WM_CANCELJOURNAL";
        case 0x004E: return "WM_NOTIFY";
        case 0x0050: return "WM_INPUTLANGCHANGEREQUEST";
        case 0x0051: return "WM_INPUTLANGCHANGE";
        case 0x0052: return "WM_TCARD";
        case 0x0053: return "WM_HELP";
        case 0x0054: return "WM_USERCHANGED";
        case 0x0055: return "WM_NOTIFYFORMAT";
        case 0x007B: return "WM_CONTEXTMENU";
        case 0x007C: return "WM_STYLECHANGING";
        case 0x007D: return "WM_STYLECHANGED";
        case 0x007E: return "WM_DISPLAYCHANGE";
        case 0x007F: return "WM_GETICON";
        case 0x0080: return "WM_SETICON";
#endif  //WIN32

        case 0x0081: return "WM_NCCREATE";
        case 0x0082: return "WM_NCDESTROY";
        case 0x0083: return "WM_NCCALCSIZE";
        case 0x0084: return "WM_NCHITTEST";
        case 0x0085: return "WM_NCPAINT";
        case 0x0086: return "WM_NCACTIVATE";
        case 0x0087: return "WM_GETDLGCODE";
        case 0x00A0: return "WM_NCMOUSEMOVE";
        case 0x00A1: return "WM_NCLBUTTONDOWN";
        case 0x00A2: return "WM_NCLBUTTONUP";
        case 0x00A3: return "WM_NCLBUTTONDBLCLK";
        case 0x00A4: return "WM_NCRBUTTONDOWN";
        case 0x00A5: return "WM_NCRBUTTONUP";
        case 0x00A6: return "WM_NCRBUTTONDBLCLK";
        case 0x00A7: return "WM_NCMBUTTONDOWN";
        case 0x00A8: return "WM_NCMBUTTONUP";
        case 0x00A9: return "WM_NCMBUTTONDBLCLK";
        case 0x0100: return "WM_KEYDOWN";
        case 0x0101: return "WM_KEYUP";
        case 0x0102: return "WM_CHAR";
        case 0x0103: return "WM_DEADCHAR";
        case 0x0104: return "WM_SYSKEYDOWN";
        case 0x0105: return "WM_SYSKEYUP";
        case 0x0106: return "WM_SYSCHAR";
        case 0x0107: return "WM_SYSDEADCHAR";
        case 0x0108: return "WM_KEYLAST";

#ifdef  __WIN32__
        case 0x010D: return "WM_IME_STARTCOMPOSITION";
        case 0x010E: return "WM_IME_ENDCOMPOSITION";
        case 0x010F: return "WM_IME_COMPOSITION";
#endif  //WIN32

        case 0x0110: return "WM_INITDIALOG";
        case 0x0111: return "WM_COMMAND";
        case 0x0112: return "WM_SYSCOMMAND";
        case 0x0113: return "WM_TIMER";
        case 0x0114: return "WM_HSCROLL";
        case 0x0115: return "WM_VSCROLL";
        case 0x0116: return "WM_INITMENU";
        case 0x0117: return "WM_INITMENUPOPUP";
        case 0x011F: return "WM_MENUSELECT";
        case 0x0120: return "WM_MENUCHAR";
        case 0x0121: return "WM_ENTERIDLE";
        case 0x0200: return "WM_MOUSEMOVE";
        case 0x0201: return "WM_LBUTTONDOWN";
        case 0x0202: return "WM_LBUTTONUP";
        case 0x0203: return "WM_LBUTTONDBLCLK";
        case 0x0204: return "WM_RBUTTONDOWN";
        case 0x0205: return "WM_RBUTTONUP";
        case 0x0206: return "WM_RBUTTONDBLCLK";
        case 0x0207: return "WM_MBUTTONDOWN";
        case 0x0208: return "WM_MBUTTONUP";
        case 0x0209: return "WM_MBUTTONDBLCLK";
        case 0x020A: return "WM_MOUSEWHEEL";
        case 0x0210: return "WM_PARENTNOTIFY";
        case 0x0211: return "WM_ENTERMENULOOP";
        case 0x0212: return "WM_EXITMENULOOP";

#ifdef  __WIN32__
        case 0x0213: return "WM_NEXTMENU";
        case 0x0214: return "WM_SIZING";
        case 0x0215: return "WM_CAPTURECHANGED";
        case 0x0216: return "WM_MOVING";
        case 0x0218: return "WM_POWERBROADCAST";
        case 0x0219: return "WM_DEVICECHANGE";
#endif  //WIN32

        case 0x0220: return "WM_MDICREATE";
        case 0x0221: return "WM_MDIDESTROY";
        case 0x0222: return "WM_MDIACTIVATE";
        case 0x0223: return "WM_MDIRESTORE";
        case 0x0224: return "WM_MDINEXT";
        case 0x0225: return "WM_MDIMAXIMIZE";
        case 0x0226: return "WM_MDITILE";
        case 0x0227: return "WM_MDICASCADE";
        case 0x0228: return "WM_MDIICONARRANGE";
        case 0x0229: return "WM_MDIGETACTIVE";
        case 0x0230: return "WM_MDISETMENU";
        case 0x0233: return "WM_DROPFILES";

#ifdef  __WIN32__
        case 0x0281: return "WM_IME_SETCONTEXT";
        case 0x0282: return "WM_IME_NOTIFY";
        case 0x0283: return "WM_IME_CONTROL";
        case 0x0284: return "WM_IME_COMPOSITIONFULL";
        case 0x0285: return "WM_IME_SELECT";
        case 0x0286: return "WM_IME_CHAR";
        case 0x0290: return "WM_IME_KEYDOWN";
        case 0x0291: return "WM_IME_KEYUP";
#endif  //WIN32

        case 0x0300: return "WM_CUT";
        case 0x0301: return "WM_COPY";
        case 0x0302: return "WM_PASTE";
        case 0x0303: return "WM_CLEAR";
        case 0x0304: return "WM_UNDO";
        case 0x0305: return "WM_RENDERFORMAT";
        case 0x0306: return "WM_RENDERALLFORMATS";
        case 0x0307: return "WM_DESTROYCLIPBOARD";
        case 0x0308: return "WM_DRAWCLIPBOARD";
        case 0x0309: return "WM_PAINTCLIPBOARD";
        case 0x030A: return "WM_VSCROLLCLIPBOARD";
        case 0x030B: return "WM_SIZECLIPBOARD";
        case 0x030C: return "WM_ASKCBFORMATNAME";
        case 0x030D: return "WM_CHANGECBCHAIN";
        case 0x030E: return "WM_HSCROLLCLIPBOARD";
        case 0x030F: return "WM_QUERYNEWPALETTE";
        case 0x0310: return "WM_PALETTEISCHANGING";
        case 0x0311: return "WM_PALETTECHANGED";

#ifdef __WIN32__
        // common controls messages - although they're not strictly speaking
        // standard, it's nice to decode them nevertheless

        // listview
        case 0x1000 + 0: return "LVM_GETBKCOLOR";
        case 0x1000 + 1: return "LVM_SETBKCOLOR";
        case 0x1000 + 2: return "LVM_GETIMAGELIST";
        case 0x1000 + 3: return "LVM_SETIMAGELIST";
        case 0x1000 + 4: return "LVM_GETITEMCOUNT";
        case 0x1000 + 5: return "LVM_GETITEMA";
        case 0x1000 + 75: return "LVM_GETITEMW";
        case 0x1000 + 6: return "LVM_SETITEMA";
        case 0x1000 + 76: return "LVM_SETITEMW";
        case 0x1000 + 7: return "LVM_INSERTITEMA";
        case 0x1000 + 77: return "LVM_INSERTITEMW";
        case 0x1000 + 8: return "LVM_DELETEITEM";
        case 0x1000 + 9: return "LVM_DELETEALLITEMS";
        case 0x1000 + 10: return "LVM_GETCALLBACKMASK";
        case 0x1000 + 11: return "LVM_SETCALLBACKMASK";
        case 0x1000 + 12: return "LVM_GETNEXTITEM";
        case 0x1000 + 13: return "LVM_FINDITEMA";
        case 0x1000 + 83: return "LVM_FINDITEMW";
        case 0x1000 + 14: return "LVM_GETITEMRECT";
        case 0x1000 + 15: return "LVM_SETITEMPOSITION";
        case 0x1000 + 16: return "LVM_GETITEMPOSITION";
        case 0x1000 + 17: return "LVM_GETSTRINGWIDTHA";
        case 0x1000 + 87: return "LVM_GETSTRINGWIDTHW";
        case 0x1000 + 18: return "LVM_HITTEST";
        case 0x1000 + 19: return "LVM_ENSUREVISIBLE";
        case 0x1000 + 20: return "LVM_SCROLL";
        case 0x1000 + 21: return "LVM_REDRAWITEMS";
        case 0x1000 + 22: return "LVM_ARRANGE";
        case 0x1000 + 23: return "LVM_EDITLABELA";
        case 0x1000 + 118: return "LVM_EDITLABELW";
        case 0x1000 + 24: return "LVM_GETEDITCONTROL";
        case 0x1000 + 25: return "LVM_GETCOLUMNA";
        case 0x1000 + 95: return "LVM_GETCOLUMNW";
        case 0x1000 + 26: return "LVM_SETCOLUMNA";
        case 0x1000 + 96: return "LVM_SETCOLUMNW";
        case 0x1000 + 27: return "LVM_INSERTCOLUMNA";
        case 0x1000 + 97: return "LVM_INSERTCOLUMNW";
        case 0x1000 + 28: return "LVM_DELETECOLUMN";
        case 0x1000 + 29: return "LVM_GETCOLUMNWIDTH";
        case 0x1000 + 30: return "LVM_SETCOLUMNWIDTH";
        case 0x1000 + 31: return "LVM_GETHEADER";
        case 0x1000 + 33: return "LVM_CREATEDRAGIMAGE";
        case 0x1000 + 34: return "LVM_GETVIEWRECT";
        case 0x1000 + 35: return "LVM_GETTEXTCOLOR";
        case 0x1000 + 36: return "LVM_SETTEXTCOLOR";
        case 0x1000 + 37: return "LVM_GETTEXTBKCOLOR";
        case 0x1000 + 38: return "LVM_SETTEXTBKCOLOR";
        case 0x1000 + 39: return "LVM_GETTOPINDEX";
        case 0x1000 + 40: return "LVM_GETCOUNTPERPAGE";
        case 0x1000 + 41: return "LVM_GETORIGIN";
        case 0x1000 + 42: return "LVM_UPDATE";
        case 0x1000 + 43: return "LVM_SETITEMSTATE";
        case 0x1000 + 44: return "LVM_GETITEMSTATE";
        case 0x1000 + 45: return "LVM_GETITEMTEXTA";
        case 0x1000 + 115: return "LVM_GETITEMTEXTW";
        case 0x1000 + 46: return "LVM_SETITEMTEXTA";
        case 0x1000 + 116: return "LVM_SETITEMTEXTW";
        case 0x1000 + 47: return "LVM_SETITEMCOUNT";
        case 0x1000 + 48: return "LVM_SORTITEMS";
        case 0x1000 + 49: return "LVM_SETITEMPOSITION32";
        case 0x1000 + 50: return "LVM_GETSELECTEDCOUNT";
        case 0x1000 + 51: return "LVM_GETITEMSPACING";
        case 0x1000 + 52: return "LVM_GETISEARCHSTRINGA";
        case 0x1000 + 117: return "LVM_GETISEARCHSTRINGW";
        case 0x1000 + 53: return "LVM_SETICONSPACING";
        case 0x1000 + 54: return "LVM_SETEXTENDEDLISTVIEWSTYLE";
        case 0x1000 + 55: return "LVM_GETEXTENDEDLISTVIEWSTYLE";
        case 0x1000 + 56: return "LVM_GETSUBITEMRECT";
        case 0x1000 + 57: return "LVM_SUBITEMHITTEST";
        case 0x1000 + 58: return "LVM_SETCOLUMNORDERARRAY";
        case 0x1000 + 59: return "LVM_GETCOLUMNORDERARRAY";
        case 0x1000 + 60: return "LVM_SETHOTITEM";
        case 0x1000 + 61: return "LVM_GETHOTITEM";
        case 0x1000 + 62: return "LVM_SETHOTCURSOR";
        case 0x1000 + 63: return "LVM_GETHOTCURSOR";
        case 0x1000 + 64: return "LVM_APPROXIMATEVIEWRECT";
        case 0x1000 + 65: return "LVM_SETWORKAREA";

        // tree view
        case 0x1100 + 0: return "TVM_INSERTITEMA";
        case 0x1100 + 50: return "TVM_INSERTITEMW";
        case 0x1100 + 1: return "TVM_DELETEITEM";
        case 0x1100 + 2: return "TVM_EXPAND";
        case 0x1100 + 4: return "TVM_GETITEMRECT";
        case 0x1100 + 5: return "TVM_GETCOUNT";
        case 0x1100 + 6: return "TVM_GETINDENT";
        case 0x1100 + 7: return "TVM_SETINDENT";
        case 0x1100 + 8: return "TVM_GETIMAGELIST";
        case 0x1100 + 9: return "TVM_SETIMAGELIST";
        case 0x1100 + 10: return "TVM_GETNEXTITEM";
        case 0x1100 + 11: return "TVM_SELECTITEM";
        case 0x1100 + 12: return "TVM_GETITEMA";
        case 0x1100 + 62: return "TVM_GETITEMW";
        case 0x1100 + 13: return "TVM_SETITEMA";
        case 0x1100 + 63: return "TVM_SETITEMW";
        case 0x1100 + 14: return "TVM_EDITLABELA";
        case 0x1100 + 65: return "TVM_EDITLABELW";
        case 0x1100 + 15: return "TVM_GETEDITCONTROL";
        case 0x1100 + 16: return "TVM_GETVISIBLECOUNT";
        case 0x1100 + 17: return "TVM_HITTEST";
        case 0x1100 + 18: return "TVM_CREATEDRAGIMAGE";
        case 0x1100 + 19: return "TVM_SORTCHILDREN";
        case 0x1100 + 20: return "TVM_ENSUREVISIBLE";
        case 0x1100 + 21: return "TVM_SORTCHILDRENCB";
        case 0x1100 + 22: return "TVM_ENDEDITLABELNOW";
        case 0x1100 + 23: return "TVM_GETISEARCHSTRINGA";
        case 0x1100 + 64: return "TVM_GETISEARCHSTRINGW";
        case 0x1100 + 24: return "TVM_SETTOOLTIPS";
        case 0x1100 + 25: return "TVM_GETTOOLTIPS";

        // header
        case 0x1200 + 0: return "HDM_GETITEMCOUNT";
        case 0x1200 + 1: return "HDM_INSERTITEMA";
        case 0x1200 + 10: return "HDM_INSERTITEMW";
        case 0x1200 + 2: return "HDM_DELETEITEM";
        case 0x1200 + 3: return "HDM_GETITEMA";
        case 0x1200 + 11: return "HDM_GETITEMW";
        case 0x1200 + 4: return "HDM_SETITEMA";
        case 0x1200 + 12: return "HDM_SETITEMW";
        case 0x1200 + 5: return "HDM_LAYOUT";
        case 0x1200 + 6: return "HDM_HITTEST";
        case 0x1200 + 7: return "HDM_GETITEMRECT";
        case 0x1200 + 8: return "HDM_SETIMAGELIST";
        case 0x1200 + 9: return "HDM_GETIMAGELIST";
        case 0x1200 + 15: return "HDM_ORDERTOINDEX";
        case 0x1200 + 16: return "HDM_CREATEDRAGIMAGE";
        case 0x1200 + 17: return "HDM_GETORDERARRAY";
        case 0x1200 + 18: return "HDM_SETORDERARRAY";
        case 0x1200 + 19: return "HDM_SETHOTDIVIDER";

        // tab control
        case 0x1300 + 2: return "TCM_GETIMAGELIST";
        case 0x1300 + 3: return "TCM_SETIMAGELIST";
        case 0x1300 + 4: return "TCM_GETITEMCOUNT";
        case 0x1300 + 5: return "TCM_GETITEMA";
        case 0x1300 + 60: return "TCM_GETITEMW";
        case 0x1300 + 6: return "TCM_SETITEMA";
        case 0x1300 + 61: return "TCM_SETITEMW";
        case 0x1300 + 7: return "TCM_INSERTITEMA";
        case 0x1300 + 62: return "TCM_INSERTITEMW";
        case 0x1300 + 8: return "TCM_DELETEITEM";
        case 0x1300 + 9: return "TCM_DELETEALLITEMS";
        case 0x1300 + 10: return "TCM_GETITEMRECT";
        case 0x1300 + 11: return "TCM_GETCURSEL";
        case 0x1300 + 12: return "TCM_SETCURSEL";
        case 0x1300 + 13: return "TCM_HITTEST";
        case 0x1300 + 14: return "TCM_SETITEMEXTRA";
        case 0x1300 + 40: return "TCM_ADJUSTRECT";
        case 0x1300 + 41: return "TCM_SETITEMSIZE";
        case 0x1300 + 42: return "TCM_REMOVEIMAGE";
        case 0x1300 + 43: return "TCM_SETPADDING";
        case 0x1300 + 44: return "TCM_GETROWCOUNT";
        case 0x1300 + 45: return "TCM_GETTOOLTIPS";
        case 0x1300 + 46: return "TCM_SETTOOLTIPS";
        case 0x1300 + 47: return "TCM_GETCURFOCUS";
        case 0x1300 + 48: return "TCM_SETCURFOCUS";
        case 0x1300 + 49: return "TCM_SETMINTABWIDTH";
        case 0x1300 + 50: return "TCM_DESELECTALL";

        // toolbar
        case WM_USER+1: return "TB_ENABLEBUTTON";
        case WM_USER+2: return "TB_CHECKBUTTON";
        case WM_USER+3: return "TB_PRESSBUTTON";
        case WM_USER+4: return "TB_HIDEBUTTON";
        case WM_USER+5: return "TB_INDETERMINATE";
        case WM_USER+9: return "TB_ISBUTTONENABLED";
        case WM_USER+10: return "TB_ISBUTTONCHECKED";
        case WM_USER+11: return "TB_ISBUTTONPRESSED";
        case WM_USER+12: return "TB_ISBUTTONHIDDEN";
        case WM_USER+13: return "TB_ISBUTTONINDETERMINATE";
        case WM_USER+17: return "TB_SETSTATE";
        case WM_USER+18: return "TB_GETSTATE";
        case WM_USER+19: return "TB_ADDBITMAP";
        case WM_USER+20: return "TB_ADDBUTTONS";
        case WM_USER+21: return "TB_INSERTBUTTON";
        case WM_USER+22: return "TB_DELETEBUTTON";
        case WM_USER+23: return "TB_GETBUTTON";
        case WM_USER+24: return "TB_BUTTONCOUNT";
        case WM_USER+25: return "TB_COMMANDTOINDEX";
        case WM_USER+26: return "TB_SAVERESTOREA";
        case WM_USER+76: return "TB_SAVERESTOREW";
        case WM_USER+27: return "TB_CUSTOMIZE";
        case WM_USER+28: return "TB_ADDSTRINGA";
        case WM_USER+77: return "TB_ADDSTRINGW";
        case WM_USER+29: return "TB_GETITEMRECT";
        case WM_USER+30: return "TB_BUTTONSTRUCTSIZE";
        case WM_USER+31: return "TB_SETBUTTONSIZE";
        case WM_USER+32: return "TB_SETBITMAPSIZE";
        case WM_USER+33: return "TB_AUTOSIZE";
        case WM_USER+35: return "TB_GETTOOLTIPS";
        case WM_USER+36: return "TB_SETTOOLTIPS";
        case WM_USER+37: return "TB_SETPARENT";
        case WM_USER+39: return "TB_SETROWS";
        case WM_USER+40: return "TB_GETROWS";
        case WM_USER+42: return "TB_SETCMDID";
        case WM_USER+43: return "TB_CHANGEBITMAP";
        case WM_USER+44: return "TB_GETBITMAP";
        case WM_USER+45: return "TB_GETBUTTONTEXTA";
        case WM_USER+75: return "TB_GETBUTTONTEXTW";
        case WM_USER+46: return "TB_REPLACEBITMAP";
        case WM_USER+47: return "TB_SETINDENT";
        case WM_USER+48: return "TB_SETIMAGELIST";
        case WM_USER+49: return "TB_GETIMAGELIST";
        case WM_USER+50: return "TB_LOADIMAGES";
        case WM_USER+51: return "TB_GETRECT";
        case WM_USER+52: return "TB_SETHOTIMAGELIST";
        case WM_USER+53: return "TB_GETHOTIMAGELIST";
        case WM_USER+54: return "TB_SETDISABLEDIMAGELIST";
        case WM_USER+55: return "TB_GETDISABLEDIMAGELIST";
        case WM_USER+56: return "TB_SETSTYLE";
        case WM_USER+57: return "TB_GETSTYLE";
        case WM_USER+58: return "TB_GETBUTTONSIZE";
        case WM_USER+59: return "TB_SETBUTTONWIDTH";
        case WM_USER+60: return "TB_SETMAXTEXTROWS";
        case WM_USER+61: return "TB_GETTEXTROWS";
        case WM_USER+41: return "TB_GETBITMAPFLAGS";

#endif //WIN32

        default:
            static char s_szBuf[128];
            sprintf(s_szBuf, "<unknown message = %d>", message);
            return s_szBuf;
    }
}
#endif //__WXDEBUG__

static void TranslateKbdEventToMouse(wxWindowMSW *win,
                                     int *x, int *y, WPARAM *flags)
{
    // construct the key mask
    WPARAM& fwKeys = *flags;

    fwKeys = MK_RBUTTON;
    if ( wxIsCtrlDown() )
        fwKeys |= MK_CONTROL;
    if ( wxIsShiftDown() )
        fwKeys |= MK_SHIFT;

    // simulate right mouse button click
    DWORD dwPos = ::GetMessagePos();
    *x = GET_X_LPARAM(dwPos);
    *y = GET_Y_LPARAM(dwPos);

    win->ScreenToClient(x, y);
}

static TEXTMETRIC wxGetTextMetrics(const wxWindowMSW *win)
{
    // prepare the DC
    TEXTMETRIC tm;
    HWND hwnd = GetHwndOf(win);
    HDC hdc = ::GetDC(hwnd);

#if !wxDIALOG_UNIT_COMPATIBILITY
    // and select the current font into it
    HFONT hfont = GetHfontOf(win->GetFont());
    if ( hfont )
    {
        hfont = (HFONT)::SelectObject(hdc, hfont);
    }
#endif

    // finally retrieve the text metrics from it
    GetTextMetrics(hdc, &tm);

#if !wxDIALOG_UNIT_COMPATIBILITY
    // and clean up
    if ( hfont )
    {
        (void)::SelectObject(hdc, hfont);
    }
#endif

    ::ReleaseDC(hwnd, hdc);

    return tm;
}

// Find the wxWindow at the current mouse position, returning the mouse
// position.
wxWindow* wxFindWindowAtPointer(wxPoint& WXUNUSED(pt))
{
    return wxFindWindowAtPoint(wxGetMousePosition());
}

wxWindow* wxFindWindowAtPoint(const wxPoint& pt)
{
    POINT pt2;
    pt2.x = pt.x;
    pt2.y = pt.y;
    HWND hWndHit = ::WindowFromPoint(pt2);

    wxWindow* win = wxFindWinFromHandle((WXHWND) hWndHit) ;
    HWND hWnd = hWndHit;

    // Try to find a window with a wxWindow associated with it
    while (!win && (hWnd != 0))
    {
        hWnd = ::GetParent(hWnd);
        win = wxFindWinFromHandle((WXHWND) hWnd) ;
    }
    return win;
}

// Get the current mouse position.
wxPoint wxGetMousePosition()
{
    POINT pt;
    GetCursorPos( & pt );
    return wxPoint(pt.x, pt.y);
}

