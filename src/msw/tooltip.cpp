///////////////////////////////////////////////////////////////////////////////
// Name:        msw/tooltip.cpp
// Purpose:     wxToolTip class implementation for MSW
// Author:      Vadim Zeitlin
// Modified by:
// Created:     31.01.99
// RCS-ID:      $Id$
// Copyright:   (c) 1999 Vadim Zeitlin
// Licence:     wxWindows license
///////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#if wxUSE_TOOLTIPS

#include "wx/tooltip.h"
#include "wx/msw/private.h"

#if defined(__WIN95__) && (!defined(__GNUWIN32__) || defined(__MINGW32__))
    #include <commctrl.h>
#endif

// VZ: normally, the trick with subclassing the tooltip control and processing
//     TTM_WINDOWFROMPOINT should work but, somehow, it doesn't. I leave the
//     code here for now (but it's not compiled) in case we need it later.
//
//     For now, instead of this, we just add all radiobox buttons to the
//     tooltip control as well (see SetWindow) - this is probably less
//     efficient, but it works.
#define wxUSE_TTM_WINDOWFROMPOINT   0

// ----------------------------------------------------------------------------
// global variables
// ----------------------------------------------------------------------------

// the tooltip parent window
WXHWND wxToolTip::ms_hwndTT = (WXHWND)NULL;

#if wxUSE_TTM_WINDOWFROMPOINT

// the tooltip window proc
static WNDPROC gs_wndprocToolTip = (WNDPROC)NULL;

#endif // wxUSE_TTM_WINDOWFROMPOINT

// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

// a simple wrapper around TOOLINFO Win32 structure
#ifdef __VISUALC__
    #pragma warning( disable : 4097 ) // we inherit from a typedef - so what?
#endif
class wxToolInfo : public TOOLINFO
{
public:
    wxToolInfo(HWND hwnd)
    {
        // initialize all members
        ::ZeroMemory(this, sizeof(TOOLINFO));

        cbSize = sizeof(TOOLINFO);
        uFlags = TTF_IDISHWND;
        uId = (UINT)hwnd;
    }
};
#ifdef __VISUALC__
    #pragma warning( default : 4097 )
#endif

// ----------------------------------------------------------------------------
// private functions
// ----------------------------------------------------------------------------

// send a message to the tooltip control
inline LRESULT SendTooltipMessage(WXHWND hwnd,
                                  UINT msg,
                                  WPARAM wParam,
                                  void *lParam)
{
    return hwnd ? ::SendMessage((HWND)hwnd, msg, wParam, (LPARAM)lParam)
                : 0;
}

// send a message to all existing tooltip controls
static void SendTooltipMessageToAll(WXHWND hwnd,
                                    UINT msg,
                                    WPARAM wParam,
                                    LPARAM lParam)
{
    (void)SendTooltipMessage((WXHWND)hwnd, msg, wParam, (void *)lParam);
}

// ============================================================================
// implementation
// ============================================================================

#if wxUSE_TTM_WINDOWFROMPOINT

// ----------------------------------------------------------------------------
// window proc for our tooltip control
// ----------------------------------------------------------------------------

LRESULT APIENTRY wxToolTipWndProc(HWND hwndTT,
                                  UINT msg,
                                  WPARAM wParam,
                                  LPARAM lParam)
{
    if ( msg == TTM_WINDOWFROMPOINT )
    {
        LPPOINT ppt = (LPPOINT)lParam;
        // is the window under control a wxWindow?
        HWND hwnd = ::WindowFromPoint(*ppt);

        // return a HWND correspondign to wxWindow because only wxWindows are
        // associated with tooltips using TTM_ADDTOOL
        while ( hwnd && !wxFindWinFromHandle((WXHWND)hwnd) )
        {
            hwnd = ::GetParent(hwnd);
        }

        if ( hwnd )
        {
            // modify the point too!
            RECT rect;
            GetWindowRect(hwnd, &rect);

            ppt->x = rect.left;
            ppt->y = rect.top;

            return (LRESULT)hwnd;
        }
    }

    return ::CallWindowProc(gs_wndprocToolTip, hwndTT, msg, wParam, lParam);
}

#endif // wxUSE_TTM_WINDOWFROMPOINT

// ----------------------------------------------------------------------------
// static functions
// ----------------------------------------------------------------------------

void wxToolTip::Enable(bool flag)
{
    SendTooltipMessageToAll(ms_hwndTT, TTM_ACTIVATE, flag, 0);
}

void wxToolTip::SetDelay(long milliseconds)
{
    SendTooltipMessageToAll(ms_hwndTT, TTM_SETDELAYTIME,
                            TTDT_INITIAL, milliseconds);
}

// ---------------------------------------------------------------------------
// implementation helpers
// ---------------------------------------------------------------------------

// create the tooltip ctrl for our parent frame if it doesn't exist yet
WXHWND wxToolTip::GetToolTipCtrl()
{
    if ( !ms_hwndTT )
    {
        ms_hwndTT = (WXHWND)::CreateWindow(TOOLTIPS_CLASS,
                                           (LPSTR)NULL,
                                           TTS_ALWAYSTIP,
                                           CW_USEDEFAULT, CW_USEDEFAULT,
                                           CW_USEDEFAULT, CW_USEDEFAULT,
                                           NULL, (HMENU)NULL,
                                           wxGetInstance(),
                                           NULL);
       if ( ms_hwndTT )
       {
           HWND hwnd = (HWND)ms_hwndTT;
           SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0,
                        SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

#if wxUSE_TTM_WINDOWFROMPOINT
           // subclass the newly created control
           gs_wndprocToolTip = (WNDPROC)::GetWindowLong(hwnd, GWL_WNDPROC);
           ::SetWindowLong(hwnd, GWL_WNDPROC, (long)wxToolTipWndProc);
#endif // wxUSE_TTM_WINDOWFROMPOINT
       }
    }

    return ms_hwndTT;
}

void wxToolTip::RelayEvent(WXMSG *msg)
{
    (void)SendTooltipMessage(GetToolTipCtrl(), TTM_RELAYEVENT, 0, msg);
}

// ----------------------------------------------------------------------------
// ctor & dtor
// ----------------------------------------------------------------------------

wxToolTip::wxToolTip(const wxString &tip)
         : m_text(tip)
{
    m_window = NULL;
}

wxToolTip::~wxToolTip()
{
    // there is no need to Remove() this tool - it will be done automatically
    // anyhow
}

// ----------------------------------------------------------------------------
// others
// ----------------------------------------------------------------------------

void wxToolTip::Remove()
{
    // remove this tool from the tooltip control
    if ( m_window )
    {
        wxToolInfo ti(GetHwndOf(m_window));
        (void)SendTooltipMessage(GetToolTipCtrl(), TTM_DELTOOL, 0, &ti);
    }
}

void wxToolTip::Add(WXHWND hWnd)
{
    HWND hwnd = (HWND)hWnd;

    wxToolInfo ti(hwnd);

    // as we store our text anyhow, it seems useless to waste system memory
    // by asking the tooltip ctrl to remember it too - instead it will send
    // us TTN_NEEDTEXT (via WM_NOTIFY) when it is about to be shown
    ti.hwnd = hwnd;
    ti.lpszText = LPSTR_TEXTCALLBACK;
    // instead of: ti.lpszText = (char *)m_text.c_str();

    if ( !SendTooltipMessage(GetToolTipCtrl(), TTM_ADDTOOL, 0, &ti) )
    {
        wxLogSysError(_("Failed to create the tooltip '%s'"),
                      m_text.c_str());
    }
}

void wxToolTip::SetWindow(wxWindow *win)
{
    Remove();

    m_window = win;

    // add the window itself
    if ( m_window )
    {
        Add(m_window->GetHWND());
    }

    // and all of its subcontrols (e.g. radiobuttons in a radiobox) as well
    wxControl *control = wxDynamicCast(m_window, wxControl);
    if ( control )
    {
        size_t count = control->GetSubcontrols().GetCount();
        for ( size_t n = 0; n < count; n++ )
        {
            wxWindowID id = control->GetSubcontrols()[n];
            HWND hwnd = GetDlgItem(GetHwndOf(m_window), id);

            if ( hwnd )
            {
                Add((WXHWND)hwnd);
            }
        }
    }
}

void wxToolTip::SetTip(const wxString& tip)
{
    m_text = tip;

    if ( m_window )
    {
        // update it immediately
        wxToolInfo ti(GetHwndOf(m_window));
        ti.lpszText = (wxChar *)m_text.c_str();

        (void)SendTooltipMessage(GetToolTipCtrl(), TTM_UPDATETIPTEXT, 0, &ti);
    }
}

#endif // wxUSE_TOOLTIPS
