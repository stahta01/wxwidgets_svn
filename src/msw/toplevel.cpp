///////////////////////////////////////////////////////////////////////////////
// Name:        msw/toplevel.cpp
// Purpose:     implements wxTopLevelWindow for MSW
// Author:      Vadim Zeitlin
// Modified by:
// Created:     24.09.01
// RCS-ID:      $Id$
// Copyright:   (c) 2001 SciTech Software, Inc. (www.scitechsoft.com)
// License:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "toplevel.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/app.h"
    #include "wx/toplevel.h"
    #include "wx/string.h"
    #include "wx/log.h"
    #include "wx/intl.h"
    #include "wx/frame.h"
    #include "wx/containr.h"        // wxSetFocusToChild()
#endif //WX_PRECOMP

#include "wx/module.h"

#include "wx/msw/private.h"
#if defined(__WXWINCE__)
  #include <ole2.h>
  #include <shellapi.h>
  #if _WIN32_WCE < 400
    #include <aygshell.h>
  #endif
#include "wx/msw/wince/missing.h"
#endif

#include "wx/msw/missing.h"
#include "wx/msw/winundef.h"

// This can't be undefed in winundef.h or
// there are further errors
#if defined(__WXWINCE__) && defined(CreateDialog)
#undef CreateDialog
#endif

#include "wx/display.h"

#ifndef ICON_BIG
    #define ICON_BIG 1
#endif

#ifndef ICON_SMALL
    #define ICON_SMALL 0
#endif

// ----------------------------------------------------------------------------
// stubs for missing functions under MicroWindows
// ----------------------------------------------------------------------------

#ifdef __WXMICROWIN__

// static inline bool IsIconic(HWND WXUNUSED(hwnd)) { return FALSE; }
static inline bool IsZoomed(HWND WXUNUSED(hwnd)) { return FALSE; }

#endif // __WXMICROWIN__

// NB: wxDlgProc must be defined here and not in dialog.cpp because the latter
//     is not included by wxUniv build which does need wxDlgProc
LONG APIENTRY _EXPORT
wxDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

// ----------------------------------------------------------------------------
// globals
// ----------------------------------------------------------------------------

// list of all frames and modeless dialogs
wxWindowList wxModelessWindows;

// the name of the default wxWindows class
extern const wxChar *wxCanvasClassName;

// ----------------------------------------------------------------------------
// wxTLWHiddenParentModule: used to manage the hidden parent window (we need a
// module to ensure that the window is always deleted)
// ----------------------------------------------------------------------------

class wxTLWHiddenParentModule : public wxModule
{
public:
    // module init/finalize
    virtual bool OnInit();
    virtual void OnExit();

    // get the hidden window (creates on demand)
    static HWND GetHWND();

private:
    // the HWND of the hidden parent
    static HWND ms_hwnd;

    // the class used to create it
    static const wxChar *ms_className;

    DECLARE_DYNAMIC_CLASS(wxTLWHiddenParentModule)
};

IMPLEMENT_DYNAMIC_CLASS(wxTLWHiddenParentModule, wxModule)

// ============================================================================
// wxTopLevelWindowMSW implementation
// ============================================================================

BEGIN_EVENT_TABLE(wxTopLevelWindowMSW, wxTopLevelWindowBase)
    EVT_ACTIVATE(wxTopLevelWindowMSW::OnActivate)
END_EVENT_TABLE()

// ----------------------------------------------------------------------------
// wxTopLevelWindowMSW creation
// ----------------------------------------------------------------------------

void wxTopLevelWindowMSW::Init()
{
    m_iconized =
    m_maximizeOnShow = FALSE;

    // Data to save/restore when calling ShowFullScreen
    m_fsStyle = 0;
    m_fsOldWindowStyle = 0;
    m_fsIsMaximized = FALSE;
    m_fsIsShowing = FALSE;

    m_winLastFocused = (wxWindow *)NULL;
}

WXDWORD wxTopLevelWindowMSW::MSWGetStyle(long style, WXDWORD *exflags) const
{
    // let the base class deal with the common styles but fix the ones which
    // don't make sense for us (we also deal with the borders ourselves)
    WXDWORD msflags = wxWindow::MSWGetStyle
                      (
                        (style & ~wxBORDER_MASK) | wxBORDER_NONE, exflags
                      ) & ~WS_CHILD & ~WS_VISIBLE;

    // first select the kind of window being created
    //
    // note that if we don't set WS_POPUP, Windows assumes WS_OVERLAPPED and
    // creates a window with both caption and border, hence we also test it
    // below in some other cases
    if ( style & wxFRAME_TOOL_WINDOW )
        msflags |= WS_POPUP;
    else
    {
#ifdef __WXWINCE__
        if (msflags & WS_BORDER)
#endif
            msflags |= WS_OVERLAPPED;
    }

    // border and caption styles
    if ( style & wxRESIZE_BORDER )
    {
#ifndef __WXWINCE__
        msflags |= WS_THICKFRAME;
#endif
    }
    else if ( exflags && ((style & wxBORDER_DOUBLE) || (style & wxBORDER_RAISED)) )
        *exflags |= WS_EX_DLGMODALFRAME;
    else if ( !(style & wxBORDER_NONE) )
        msflags |= WS_BORDER;
#ifndef __WXWINCE__
    else
        msflags |= WS_POPUP;
#endif

    if ( style & wxCAPTION )
        msflags |= WS_CAPTION;
#ifndef __WXWINCE__
    else
        msflags |= WS_POPUP;
#endif

    // next translate the individual flags
    if ( style & wxMINIMIZE_BOX )
        msflags |= WS_MINIMIZEBOX;
    if ( style & wxMAXIMIZE_BOX )
        msflags |= WS_MAXIMIZEBOX;
    if ( style & wxSYSTEM_MENU )
        msflags |= WS_SYSMENU;
#ifndef __WXWINCE__
    if ( style & wxMINIMIZE )
        msflags |= WS_MINIMIZE;
    if ( style & wxMAXIMIZE )
        msflags |= WS_MAXIMIZE;
#endif

    // Keep this here because it saves recoding this function in wxTinyFrame
#if wxUSE_ITSY_BITSY && !defined(__WIN32__)
    if ( style & wxTINY_CAPTION_VERT )
        msflags |= IBS_VERTCAPTION;
    if ( style & wxTINY_CAPTION_HORIZ )
        msflags |= IBS_HORZCAPTION;
#else
    if ( style & (wxTINY_CAPTION_VERT | wxTINY_CAPTION_HORIZ) )
        msflags |= WS_CAPTION;
#endif

    if ( exflags )
    {
        if ( !(GetExtraStyle() & wxTOPLEVEL_EX_DIALOG) )
        {
            if ( style & wxFRAME_TOOL_WINDOW )
            {
                // create the palette-like window
                *exflags |= WS_EX_TOOLWINDOW;

                // tool windows shouldn't appear on the taskbar (as documented)
                style |= wxFRAME_NO_TASKBAR;
            }

            // We have to solve 2 different problems here:
            //
            // 1. frames with wxFRAME_NO_TASKBAR flag shouldn't appear in the
            //    taskbar even if they don't have a parent
            //
            // 2. frames without this style should appear in the taskbar even
            //    if they're owned (Windows only puts non owned windows into
            //    the taskbar normally)
            //
            // The second one is solved here by using WS_EX_APPWINDOW flag, the
            // first one is dealt with in our MSWGetParent() method
            // implementation
#ifndef __WXWINCE__
            if ( !(style & wxFRAME_NO_TASKBAR) && GetParent() )
            {
                // need to force the frame to appear in the taskbar
                *exflags |= WS_EX_APPWINDOW;
            }
#endif
            //else: nothing to do [here]
        }

        if ( style & wxSTAY_ON_TOP )
            *exflags |= WS_EX_TOPMOST;

#ifdef __WIN32__
        if ( GetExtraStyle() & wxFRAME_EX_CONTEXTHELP )
            *exflags |= WS_EX_CONTEXTHELP;
#endif // __WIN32__
    }

    return msflags;
}

WXHWND wxTopLevelWindowMSW::MSWGetParent() const
{
    // for the frames without wxFRAME_FLOAT_ON_PARENT style we should use NULL
    // parent HWND or it would be always on top of its parent which is not what
    // we usually want (in fact, we only want it for frames with the
    // wxFRAME_FLOAT_ON_PARENT flag)
    HWND hwndParent = NULL;
    if ( HasFlag(wxFRAME_FLOAT_ON_PARENT) )
    {
        const wxWindow *parent = GetParent();

        if ( !parent )
        {
            // this flag doesn't make sense then and will be ignored
            wxFAIL_MSG( _T("wxFRAME_FLOAT_ON_PARENT but no parent?") );
        }
        else
        {
            hwndParent = GetHwndOf(parent);
        }
    }
    //else: don't float on parent, must not be owned

    // now deal with the 2nd taskbar-related problem (see comments above in
    // MSWGetStyle())
    if ( HasFlag(wxFRAME_NO_TASKBAR) && !hwndParent )
    {
        // use hidden parent
        hwndParent = wxTLWHiddenParentModule::GetHWND();
    }

    return (WXHWND)hwndParent;
}

bool wxTopLevelWindowMSW::CreateDialog(const void *dlgTemplate,
                                       const wxString& title,
                                       const wxPoint& pos,
                                       const wxSize& size)
{
#ifdef __WXMICROWIN__
    // no dialogs support under MicroWin yet
    return CreateFrame(title, pos, size);
#else // !__WXMICROWIN__
    wxWindow *parent = GetParent();

    // for the dialogs without wxDIALOG_NO_PARENT style, use the top level
    // app window as parent - this avoids creating modal dialogs without
    // parent
    if ( !parent && !(GetWindowStyleFlag() & wxDIALOG_NO_PARENT) )
    {
        parent = wxTheApp->GetTopWindow();

        if ( parent )
        {
            // don't use transient windows as parents, this is dangerous as it
            // can lead to a crash if the parent is destroyed before the child
            //
            // also don't use the window which is currently hidden as then the
            // dialog would be hidden as well
            if ( (parent->GetExtraStyle() & wxWS_EX_TRANSIENT) ||
                    !parent->IsShown() )
            {
                parent = NULL;
            }
        }
    }

    m_hWnd = (WXHWND)::CreateDialogIndirect
                       (
                        wxGetInstance(),
                        (DLGTEMPLATE*)dlgTemplate,
                        parent ? GetHwndOf(parent) : NULL,
                        (DLGPROC)wxDlgProc
                       );

    if ( !m_hWnd )
    {
        wxFAIL_MSG(wxT("Failed to create dialog. Incorrect DLGTEMPLATE?"));

        wxLogSysError(wxT("Can't create dialog using memory template"));

        return FALSE;
    }

    WXDWORD exflags;
    (void)MSWGetCreateWindowFlags(&exflags);

    if ( exflags )
    {
        ::SetWindowLong(GetHwnd(), GWL_EXSTYLE, exflags);
        ::SetWindowPos(GetHwnd(),
                       exflags & WS_EX_TOPMOST ? HWND_TOPMOST : 0,
                       0, 0, 0, 0,
                       SWP_NOSIZE |
                       SWP_NOMOVE |
                       (exflags & WS_EX_TOPMOST ? 0 : SWP_NOZORDER) |
                       SWP_NOACTIVATE);
    }

#if defined(__WIN95__)
    // For some reason, the system menu is activated when we use the
    // WS_EX_CONTEXTHELP style, so let's set a reasonable icon
    if ( exflags & WS_EX_CONTEXTHELP )
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

    // move the dialog to its initial position without forcing repainting
    int x, y, w, h;
    (void)MSWGetCreateWindowCoords(pos, size, x, y, w, h);

    if ( x == (int)CW_USEDEFAULT )
    {
        // centre it on the screen - what else can we do?
        wxSize sizeDpy = wxGetDisplaySize();

        x = (sizeDpy.x - w) / 2;
        y = (sizeDpy.y - h) / 2;
    }

    if ( !::MoveWindow(GetHwnd(), x, y, w, h, FALSE) )
    {
        wxLogLastError(wxT("MoveWindow"));
    }

    if ( !title.empty() )
    {
        ::SetWindowText(GetHwnd(), title);
    }

    SubclassWin(m_hWnd);

    return TRUE;
#endif // __WXMICROWIN__/!__WXMICROWIN__
}

bool wxTopLevelWindowMSW::CreateFrame(const wxString& title,
                                      const wxPoint& pos,
                                      const wxSize& size)
{
    WXDWORD exflags;
    WXDWORD flags = MSWGetCreateWindowFlags(&exflags);

    return MSWCreate(wxCanvasClassName, title, pos, size, flags, exflags);
}

bool wxTopLevelWindowMSW::Create(wxWindow *parent,
                                 wxWindowID id,
                                 const wxString& title,
                                 const wxPoint& pos,
                                 const wxSize& size,
                                 long style,
                                 const wxString& name)
{
    bool ret wxDUMMY_INITIALIZE(false);
    int w, h;
    
    // init our fields
    Init();
    w = WidthDefault(size.x);
    h = HeightDefault(size.y);

    m_windowStyle = style;

    SetName(name);

    m_windowId = id == -1 ? NewControlId() : id;

    wxTopLevelWindows.Append(this);

    if ( parent )
        parent->AddChild(this);

    if ( GetExtraStyle() & wxTOPLEVEL_EX_DIALOG )
    {
        // we have different dialog templates to allows creation of dialogs
        // with & without captions under MSWindows, resizeable or not (but a
        // resizeable dialog always has caption - otherwise it would look too
        // strange)

        // we need 3 additional WORDs for dialog menu, class and title (as we
        // don't use DS_SETFONT we don't need the fourth WORD for the font)
        static const int dlgsize = sizeof(DLGTEMPLATE) + (sizeof(WORD) * 3);
        DLGTEMPLATE *dlgTemplate = (DLGTEMPLATE *)malloc(dlgsize);
        memset(dlgTemplate, 0, dlgsize);

        // these values are arbitrary, they won't be used normally anyhow
        dlgTemplate->x  = 34;
        dlgTemplate->y  = 22;
        dlgTemplate->cx = 144;
        dlgTemplate->cy = 75;

        // reuse the code in MSWGetStyle() but correct the results slightly for
        // the dialog
        dlgTemplate->style = MSWGetStyle(style, NULL);

        // all dialogs are popups
        dlgTemplate->style |= WS_POPUP;

        // force 3D-look if necessary, it looks impossibly ugly otherwise
        if ( style & (wxRESIZE_BORDER | wxCAPTION) )
            dlgTemplate->style |= DS_MODALFRAME;

        ret = CreateDialog(dlgTemplate, title, pos, wxSize(w,h));
        free(dlgTemplate);
    }
    else // !dialog
    {
        ret = CreateFrame(title, pos, wxSize(w,h));
    }

    if ( ret && !(GetWindowStyleFlag() & wxCLOSE_BOX) )
    {
        EnableCloseButton(false);
    }

    // for some reason we need to manually send ourselves this message as
    // otherwise the mnemonics are always shown -- even if they're configured
    // to be hidden until "Alt" is pressed in the control panel
    //
    // this could indicate a bug somewhere else but for now this is the only
    // fix we have
    if ( ret )
    {
        SendMessage
        (
            GetHwnd(),
            WM_UPDATEUISTATE,
            MAKEWPARAM(UIS_INITIALIZE, UISF_HIDEFOCUS | UISF_HIDEACCEL),
            0
        );
    }

    return ret;
}

wxTopLevelWindowMSW::~wxTopLevelWindowMSW()
{
    if ( wxModelessWindows.Find(this) )
        wxModelessWindows.DeleteObject(this);

    // after destroying an owned window, Windows activates the next top level
    // window in Z order but it may be different from our owner (to reproduce
    // this simply Alt-TAB to another application and back before closing the
    // owned frame) whereas we always want to yield activation to our parent
    if ( HasFlag(wxFRAME_FLOAT_ON_PARENT) )
    {
        wxWindow *parent = GetParent();
        if ( parent )
        {
            ::BringWindowToTop(GetHwndOf(parent));
        }
    }
}

// ----------------------------------------------------------------------------
// wxTopLevelWindowMSW showing
// ----------------------------------------------------------------------------

void wxTopLevelWindowMSW::DoShowWindow(int nShowCmd)
{
    ::ShowWindow(GetHwnd(), nShowCmd);

    m_iconized = nShowCmd == SW_MINIMIZE;
}

bool wxTopLevelWindowMSW::Show(bool show)
{
    // don't use wxWindow version as we want to call DoShowWindow() ourselves
    if ( !wxWindowBase::Show(show) )
        return FALSE;

    int nShowCmd;
    if ( show )
    {
        if ( m_maximizeOnShow )
        {
            // show and maximize
            nShowCmd = SW_MAXIMIZE;

            m_maximizeOnShow = FALSE;
        }
        else // just show
        {
           if ( GetWindowStyle() & wxFRAME_TOOL_WINDOW )
               nShowCmd = SW_SHOWNA;
           else
               nShowCmd = SW_SHOW;
        }
    }
    else // hide
    {
        nShowCmd = SW_HIDE;
    }

    DoShowWindow(nShowCmd);

    if ( show )
    {
        ::BringWindowToTop(GetHwnd());

        wxActivateEvent event(wxEVT_ACTIVATE, TRUE, m_windowId);
        event.SetEventObject( this );
        GetEventHandler()->ProcessEvent(event);
    }
    else // hide
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

// ----------------------------------------------------------------------------
// wxTopLevelWindowMSW maximize/minimize
// ----------------------------------------------------------------------------

void wxTopLevelWindowMSW::Maximize(bool maximize)
{
    if ( IsShown() )
    {
        // just maximize it directly
        DoShowWindow(maximize ? SW_MAXIMIZE : SW_RESTORE);
    }
    else // hidden
    {
        // we can't maximize the hidden frame because it shows it as well, so
        // just remember that we should do it later in this case
        m_maximizeOnShow = maximize;
    }
}

bool wxTopLevelWindowMSW::IsMaximized() const
{
#ifdef __WXWINCE__
    return FALSE;
#else
    return ::IsZoomed(GetHwnd()) != 0;
#endif
}

void wxTopLevelWindowMSW::Iconize(bool iconize)
{
    DoShowWindow(iconize ? SW_MINIMIZE : SW_RESTORE);
}

bool wxTopLevelWindowMSW::IsIconized() const
{
#ifdef __WXWINCE__
    return FALSE;
#else
    // also update the current state
    ((wxTopLevelWindowMSW *)this)->m_iconized = ::IsIconic(GetHwnd()) != 0;

    return m_iconized;
#endif
}

void wxTopLevelWindowMSW::Restore()
{
    DoShowWindow(SW_RESTORE);
}

// ----------------------------------------------------------------------------
// wxTopLevelWindowMSW fullscreen
// ----------------------------------------------------------------------------

bool wxTopLevelWindowMSW::ShowFullScreen(bool show, long style)
{
    if ( show == IsFullScreen() )
    {
        // nothing to do
        return TRUE;
    }

    m_fsIsShowing = show;

    if ( show )
    {
        m_fsStyle = style;

        // zap the frame borders

        // save the 'normal' window style
        m_fsOldWindowStyle = GetWindowLong(GetHwnd(), GWL_STYLE);

        // save the old position, width & height, maximize state
        m_fsOldSize = GetRect();
        m_fsIsMaximized = IsMaximized();

        // decide which window style flags to turn off
        LONG newStyle = m_fsOldWindowStyle;
        LONG offFlags = 0;

        if (style & wxFULLSCREEN_NOBORDER)
        {
            offFlags |= WS_BORDER;
#ifndef __WXWINCE__
            offFlags |= WS_THICKFRAME;
#endif
        }
        if (style & wxFULLSCREEN_NOCAPTION)
            offFlags |= WS_CAPTION | WS_SYSMENU;

        newStyle &= ~offFlags;

        // change our window style to be compatible with full-screen mode
        ::SetWindowLong(GetHwnd(), GWL_STYLE, newStyle);

        wxRect rect;
#if wxUSE_DISPLAY
        // resize to the size of the display containing us
        int dpy = wxDisplay::GetFromWindow(this);
        if ( dpy != wxNOT_FOUND )
        {
            rect = wxDisplay(dpy).GetGeometry();
        }
        else // fall back to the main desktop
#else // wxUSE_DISPLAY
        {
            // resize to the size of the desktop
            wxCopyRECTToRect(wxGetWindowRect(::GetDesktopWindow()), rect);
#ifdef __WXWINCE__
            // FIXME: size of the bottom menu (toolbar)
            // should be taken in account
            rect.height += rect.y;
            rect.y       = 0;
#endif
        }
#endif // wxUSE_DISPLAY

        SetSize(rect);

        // now flush the window style cache and actually go full-screen
        long flags = SWP_FRAMECHANGED;

        // showing the frame full screen should also show it if it's still
        // hidden
        if ( !IsShown() )
        {
            // don't call wxWindow version to avoid flicker from calling
            // ::ShowWindow() -- we're going to show the window at the correct
            // location directly below -- but do call the wxWindowBase version
            // to sync the internal m_isShown flag
            wxWindowBase::Show();

            flags |= SWP_SHOWWINDOW;
        }

        SetWindowPos(GetHwnd(), HWND_TOP,
                     rect.x, rect.y, rect.width, rect.height,
                     flags);

#if defined(__WXWINCE__) && _WIN32_WCE < 400
        ::SHFullScreen(GetHwnd(), SHFS_HIDETASKBAR | SHFS_HIDESIPBUTTON);
#endif

        // finally send an event allowing the window to relayout itself &c
        wxSizeEvent event(rect.GetSize(), GetId());
        GetEventHandler()->ProcessEvent(event);
    }
    else // stop showing full screen
    {
#if defined(__WXWINCE__) && _WIN32_WCE < 400
        ::SHFullScreen(GetHwnd(), SHFS_SHOWTASKBAR | SHFS_SHOWSIPBUTTON);
#endif
        Maximize(m_fsIsMaximized);
        SetWindowLong(GetHwnd(),GWL_STYLE, m_fsOldWindowStyle);
        SetWindowPos(GetHwnd(),HWND_TOP,m_fsOldSize.x, m_fsOldSize.y,
            m_fsOldSize.width, m_fsOldSize.height, SWP_FRAMECHANGED);
    }

    return TRUE;
}

// ----------------------------------------------------------------------------
// wxTopLevelWindowMSW misc
// ----------------------------------------------------------------------------

void wxTopLevelWindowMSW::SetIcon(const wxIcon& icon)
{
    SetIcons( wxIconBundle( icon ) );
}

void wxTopLevelWindowMSW::SetIcons(const wxIconBundle& icons)
{
    wxTopLevelWindowBase::SetIcons(icons);

#if defined(__WIN95__) && !defined(__WXMICROWIN__)
    const wxIcon& sml = icons.GetIcon( wxSize( 16, 16 ) );
    if( sml.Ok() && sml.GetWidth() == 16 && sml.GetHeight() == 16 )
    {
        ::SendMessage( GetHwndOf( this ), WM_SETICON, ICON_SMALL,
                       (LPARAM)GetHiconOf(sml) );
    }

    const wxIcon& big = icons.GetIcon( wxSize( 32, 32 ) );
    if( big.Ok() && big.GetWidth() == 32 && big.GetHeight() == 32 )
    {
        ::SendMessage( GetHwndOf( this ), WM_SETICON, ICON_BIG,
                       (LPARAM)GetHiconOf(big) );
    }
#endif // __WIN95__
}

bool wxTopLevelWindowMSW::EnableCloseButton(bool enable)
{
#if !defined(__WXMICROWIN__)
    // get system (a.k.a. window) menu
    HMENU hmenu = GetSystemMenu(GetHwnd(), FALSE /* get it */);
    if ( !hmenu )
    {
        // no system menu at all -- ok if we want to remove the close button
        // anyhow, but bad if we want to show it
        return !enable;
    }

    // enabling/disabling the close item from it also automatically
    // disables/enables the close title bar button
    if ( ::EnableMenuItem(hmenu, SC_CLOSE,
                          MF_BYCOMMAND |
                          (enable ? MF_ENABLED : MF_GRAYED)) == -1 )
    {
        wxLogLastError(_T("EnableMenuItem(SC_CLOSE)"));

        return FALSE;
    }

    // update appearance immediately
    if ( !::DrawMenuBar(GetHwnd()) )
    {
        wxLogLastError(_T("DrawMenuBar"));
    }
#endif // !__WXMICROWIN__

    return TRUE;
}

bool wxTopLevelWindowMSW::SetShape(const wxRegion& region)
{
#ifdef __WXWINCE__
    return FALSE;
#else
    wxCHECK_MSG( HasFlag(wxFRAME_SHAPED), FALSE,
                 _T("Shaped windows must be created with the wxFRAME_SHAPED style."));

    // The empty region signifies that the shape should be removed from the
    // window.
    if ( region.IsEmpty() )
    {
        if (::SetWindowRgn(GetHwnd(), NULL, TRUE) == 0)
        {
            wxLogLastError(_T("SetWindowRgn"));
            return FALSE;
        }
        return TRUE;
    }

    // Windows takes ownership of the region, so
    // we'll have to make a copy of the region to give to it.
    DWORD noBytes = ::GetRegionData(GetHrgnOf(region), 0, NULL);
    RGNDATA *rgnData = (RGNDATA*) new char[noBytes];
    ::GetRegionData(GetHrgnOf(region), noBytes, rgnData);
    HRGN hrgn = ::ExtCreateRegion(NULL, noBytes, rgnData);
    delete[] (char*) rgnData;

    // SetWindowRgn expects the region to be in coordinants
    // relative to the window, not the client area.  Figure
    // out the offset, if any.
    RECT rect;
    DWORD dwStyle =   ::GetWindowLong(GetHwnd(), GWL_STYLE);
    DWORD dwExStyle = ::GetWindowLong(GetHwnd(), GWL_EXSTYLE);
    ::GetClientRect(GetHwnd(), &rect);
    ::AdjustWindowRectEx(&rect, dwStyle, FALSE, dwExStyle);
    ::OffsetRgn(hrgn, -rect.left, -rect.top);

    // Now call the shape API with the new region.
    if (::SetWindowRgn(GetHwnd(), hrgn, TRUE) == 0)
    {
        wxLogLastError(_T("SetWindowRgn"));
        return FALSE;
    }
    return TRUE;
#endif
}

// ----------------------------------------------------------------------------
// wxTopLevelWindow event handling
// ----------------------------------------------------------------------------

// Default activation behaviour - set the focus for the first child
// subwindow found.
void wxTopLevelWindowMSW::OnActivate(wxActivateEvent& event)
{
    if ( event.GetActive() )
    {
        // restore focus to the child which was last focused unless we already
        // have it
        wxLogTrace(_T("focus"), _T("wxTLW %08x activated."), (int) m_hWnd);

        wxWindow *winFocus = FindFocus();
        if ( !winFocus || wxGetTopLevelParent(winFocus) != this )
        {
            wxWindow *parent = m_winLastFocused ? m_winLastFocused->GetParent()
                                                : NULL;
            if ( !parent )
            {
                parent = this;
            }

            wxSetFocusToChild(parent, &m_winLastFocused);
        }
    }
    else // deactivating
    {
        // remember the last focused child if it is our child
        m_winLastFocused = FindFocus();

        if ( m_winLastFocused )
        {
            // let it know that it doesn't have focus any more
            m_winLastFocused->HandleKillFocus((WXHWND)NULL);

            // and don't remember it if it's a child from some other frame
            if ( wxGetTopLevelParent(m_winLastFocused) != this )
            {
                m_winLastFocused = NULL;
            }
        }

        wxLogTrace(_T("focus"),
                   _T("wxTLW %08x deactivated, last focused: %08x."),
                   (int) m_hWnd,
                   (int) (m_winLastFocused ? GetHwndOf(m_winLastFocused)
                                           : NULL));

        event.Skip();
    }
}

// the DialogProc for all wxWindows dialogs
LONG APIENTRY _EXPORT
wxDlgProc(HWND WXUNUSED(hDlg),
          UINT message,
          WPARAM WXUNUSED(wParam),
          LPARAM WXUNUSED(lParam))
{
    switch ( message )
    {
        case WM_INITDIALOG:
            // for this message, returning TRUE tells system to set focus to
            // the first control in the dialog box, but as we set the focus
            // ourselves, we return FALSE from here as well, so fall through

        default:
            // for all the other ones, FALSE means that we didn't process the
            // message
            return FALSE;
    }
}

// ============================================================================
// wxTLWHiddenParentModule implementation
// ============================================================================

HWND wxTLWHiddenParentModule::ms_hwnd = NULL;

const wxChar *wxTLWHiddenParentModule::ms_className = NULL;

bool wxTLWHiddenParentModule::OnInit()
{
    ms_hwnd = NULL;
    ms_className = NULL;

    return TRUE;
}

void wxTLWHiddenParentModule::OnExit()
{
    if ( ms_hwnd )
    {
        if ( !::DestroyWindow(ms_hwnd) )
        {
            wxLogLastError(_T("DestroyWindow(hidden TLW parent)"));
        }

        ms_hwnd = NULL;
    }

    if ( ms_className )
    {
        if ( !::UnregisterClass(ms_className, wxGetInstance()) )
        {
            wxLogLastError(_T("UnregisterClass(\"wxTLWHiddenParent\")"));
        }

        ms_className = NULL;
    }
}

/* static */
HWND wxTLWHiddenParentModule::GetHWND()
{
    if ( !ms_hwnd )
    {
        if ( !ms_className )
        {
            static const wxChar *HIDDEN_PARENT_CLASS = _T("wxTLWHiddenParent");

            WNDCLASS wndclass;
            wxZeroMemory(wndclass);

            wndclass.lpfnWndProc   = DefWindowProc;
            wndclass.hInstance     = wxGetInstance();
            wndclass.lpszClassName = HIDDEN_PARENT_CLASS;

            if ( !::RegisterClass(&wndclass) )
            {
                wxLogLastError(_T("RegisterClass(\"wxTLWHiddenParent\")"));
            }
            else
            {
                ms_className = HIDDEN_PARENT_CLASS;
            }
        }

        ms_hwnd = ::CreateWindow(ms_className, wxEmptyString, 0, 0, 0, 0, 0, NULL,
                                 (HMENU)NULL, wxGetInstance(), NULL);
        if ( !ms_hwnd )
        {
            wxLogLastError(_T("CreateWindow(hidden TLW parent)"));
        }
    }

    return ms_hwnd;
}


