/////////////////////////////////////////////////////////////////////////////
// Name:        msw/spinctrl.cpp
// Purpose:     wxSpinCtrl class implementation for Win32
// Author:      Vadim Zeitlin
// Modified by:
// Created:     22.07.99
// RCS-ID:      $Id$
// Copyright:   (c) Vadim Zeitlin
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

#ifdef __GNUG__
    #pragma implementation "spinctrlbase.h"
    #pragma implementation "spinctrl.h"
#endif

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// for compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#if defined(__WIN95__)

#include "wx/spinctrl.h"
#include "wx/msw/private.h"

#if (defined(__WIN95__) && !defined(__GNUWIN32__)) || defined(__TWIN32__) || defined(wxUSE_NORLANDER_HEADERS)
    #include <commctrl.h>
#endif

// ----------------------------------------------------------------------------
// macros
// ----------------------------------------------------------------------------

#if !USE_SHARED_LIBRARY
    IMPLEMENT_DYNAMIC_CLASS(wxSpinCtrl, wxControl)
#endif

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// the margin between the up-down control and its buddy
static const int MARGIN_BETWEEN = 5;

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// construction
// ----------------------------------------------------------------------------

bool wxSpinCtrl::Create(wxWindow *parent,
                        wxWindowID id,
                        const wxPoint& pos,
                        const wxSize& size,
                        long style,
                        int min, int max, int initial,
                        const wxString& name)
{
    // before using DoGetBestSize(), have to set style to let the base class
    // know whether this is a horizontal or vertical control (we're always
    // vertical)
    SetWindowStyle(style | wxSP_VERTICAL);

    // calculate the sizes: the size given is the toal size for both controls
    // and we need to fit them both in the given width (height is the same)
    wxSize sizeText(size), sizeBtn(size);
    sizeBtn.x = wxSpinButton::DoGetBestSize().x;
    sizeText.x -= sizeBtn.x + MARGIN_BETWEEN;
    if ( sizeText.x <= 0 )
    {
        wxLogDebug(_T("not enough space for wxSpinCtrl!"));
    }

    wxPoint posBtn(pos);
    posBtn.x += sizeText.x + MARGIN_BETWEEN;

    // create the spin button
    if ( !wxSpinButton::Create(parent, id, posBtn, sizeBtn, style, name) )
    {
        return FALSE;
    }

    SetRange(min, max);
    SetValue(initial);

    // create the text window
    if ( sizeText.y <= 0 )
    {
        // make it the same height as the button then
        int x, y;
        wxSpinButton::DoGetSize(&x, &y);

        sizeText.y = y;
    }

    m_hwndBuddy = (WXHWND)::CreateWindowEx
                    (
                     WS_EX_CLIENTEDGE,                  // sunken border
                     _T("EDIT"),                        // window class
                     NULL,                              // no window title
                     WS_CHILD | WS_VISIBLE | WS_BORDER, // style
                     pos.x, pos.y,                      // position
                     sizeText.x, sizeText.y,            // size
                     GetHwndOf(parent),                 // parent
                     (HMENU)-1,                         // control id
                     wxGetInstance(),                   // app instance
                     NULL                               // unused client data
                    );

    if ( !m_hwndBuddy )
    {
        wxLogLastError("CreateWindow(buddy text window)");

        return FALSE;
    }

    // should have the same font as the other controls
    WXHANDLE hFont = GetParent()->GetFont().GetResourceHandle();
    ::SendMessage((HWND)m_hwndBuddy, WM_SETFONT, (WPARAM)hFont, TRUE);

    // associate the text window with the spin button
    (void)SendMessage(GetHwnd(), UDM_SETBUDDY, (WPARAM)m_hwndBuddy, 0);

    return TRUE;
}

// ----------------------------------------------------------------------------
// size calculations
// ----------------------------------------------------------------------------

void wxSpinCtrl::DoMoveWindow(int x, int y, int width, int height)
{
    int widthBtn = DoGetBestSize().x;
    int widthText = width - widthBtn - MARGIN_BETWEEN;
    if ( widthText <= 0 )
    {
        wxLogDebug(_T("not enough space for wxSpinCtrl!"));
    }

    if ( !::MoveWindow((HWND)m_hwndBuddy, x, y, widthText, height, TRUE) )
    {
        wxLogLastError("MoveWindow(buddy)");
    }

    x += widthText + MARGIN_BETWEEN;
    if ( !::MoveWindow(GetHwnd(), x, y, widthBtn, height, TRUE) )
    {
        wxLogLastError("MoveWindow");
    }
}

#endif // __WIN95__
