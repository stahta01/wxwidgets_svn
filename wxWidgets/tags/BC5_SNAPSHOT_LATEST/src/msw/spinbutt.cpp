/////////////////////////////////////////////////////////////////////////////
// Name:        spinbutt.cpp
// Purpose:     wxSpinButton
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
    #pragma implementation "spinbutt.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

// Can't resolve reference to CreateUpDownControl in
// TWIN32, but could probably use normal CreateWindow instead.

#if defined(__WIN95__) && !defined(__TWIN32__)

#include "wx/spinbutt.h"
#include "wx/msw/private.h"

#if !defined(__GNUWIN32__) || defined(__TWIN32__)
    #include <commctrl.h>
#endif

#if !USE_SHARED_LIBRARY
    IMPLEMENT_DYNAMIC_CLASS(wxSpinButton, wxControl)
#endif

wxSpinButton::wxSpinButton()
{
    m_min = 0;
    m_max = 100;
}

bool wxSpinButton::Create(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size,
            long style, const wxString& name)
{
  wxSystemSettings settings;
  m_backgroundColour = parent->GetBackgroundColour() ;
  m_foregroundColour = parent->GetForegroundColour() ;

  SetName(name);

  int x = pos.x;
  int y = pos.y;
  int width = size.x;
  int height = size.y;

  m_windowStyle = style;

  SetParent(parent);

  if (width <= 0)
    width = 100;
  if (height <= 0)
    height = 30;
  if (x < 0)
    x = 0;
  if (y < 0)
    y = 0;

  m_min = 0;
  m_max = 100;

  m_windowId = (id == -1) ? NewControlId() : id;

  DWORD wstyle = WS_VISIBLE | WS_CHILD | WS_TABSTOP;
  
  if ( m_windowStyle & wxSP_HORIZONTAL )
    wstyle |= UDS_HORZ;
  if ( m_windowStyle & wxSP_ARROW_KEYS )
    wstyle |= UDS_ARROWKEYS;
  if ( m_windowStyle & wxSP_WRAP )
    wstyle |= UDS_WRAP;

  // Create the ListView control.
  HWND hWndListControl = CreateUpDownControl(wstyle,
    x, y, width, height,
    (HWND) parent->GetHWND(),
    m_windowId,
    wxGetInstance(),
    0,
    m_min, m_max, m_min);

  m_hWnd = (WXHWND) hWndListControl;
  if (parent) parent->AddChild(this);

  // TODO: have this for all controls.
  if ( !m_hWnd )
    return FALSE;
  
  SubclassWin((WXHWND) m_hWnd);

  return TRUE;
}

wxSpinButton::~wxSpinButton()
{
}

// Attributes
////////////////////////////////////////////////////////////////////////////

int wxSpinButton::GetValue() const
{
    return (int) ::SendMessage((HWND) GetHWND(), UDM_GETPOS, 0, 0);
}

void wxSpinButton::SetValue(int val)
{
    ::SendMessage((HWND) GetHWND(), UDM_SETPOS, 0, (LPARAM) MAKELONG((short) val, 0));
}

void wxSpinButton::SetRange(int minVal, int maxVal)
{
    m_min = minVal;
    m_max = maxVal;
    ::SendMessage((HWND) GetHWND(), UDM_SETRANGE, 0,
                   (LPARAM) MAKELONG((short)maxVal, (short)minVal));
}

bool wxSpinButton::MSWOnScroll(int orientation, WXWORD wParam,
                               WXWORD pos, WXHWND control)
{
    if ( !control )
        return FALSE;

    wxSpinEvent event(wxEVT_NULL, m_windowId);
    event.SetPosition(pos);
    event.SetOrientation(orientation);
    event.SetEventObject(this);

    switch ( wParam )
    {
        case SB_TOP:
            event.m_eventType = wxEVT_SCROLL_TOP;
            break;

        case SB_BOTTOM:
            event.m_eventType = wxEVT_SCROLL_BOTTOM;
            break;

        case SB_LINEUP:
            event.m_eventType = wxEVT_SCROLL_LINEUP;
            break;

        case SB_LINEDOWN:
            event.m_eventType = wxEVT_SCROLL_LINEDOWN;
            break;

        case SB_PAGEUP:
            event.m_eventType = wxEVT_SCROLL_PAGEUP;
            break;

        case SB_PAGEDOWN:
            event.m_eventType = wxEVT_SCROLL_PAGEDOWN;
            break;

        case SB_THUMBTRACK:
        case SB_THUMBPOSITION:
            event.m_eventType = wxEVT_SCROLL_THUMBTRACK;
            break;

        default:
            return FALSE;
    }

    return GetEventHandler()->ProcessEvent(event);
}

bool wxSpinButton::MSWCommand(WXUINT cmd, WXWORD id)
{
    // No command messages
    return FALSE;
}

// Spin event
IMPLEMENT_DYNAMIC_CLASS(wxSpinEvent, wxScrollEvent)

wxSpinEvent::wxSpinEvent(wxEventType commandType, int id)
           : wxScrollEvent(commandType, id)
{
}

#endif // __WIN95__
