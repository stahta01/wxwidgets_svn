/////////////////////////////////////////////////////////////////////////////
// Name:        gauge95.cpp
// Purpose:     wxGauge95 class
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "gauge95.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/defs.h"
#endif

#if wxUSE_GAUGE && defined(__WIN95__)

#include "wx/msw/gauge95.h"
#include "wx/msw/private.h"

#if (defined(__WIN95__) && !defined(__GNUWIN32__)) || defined(__TWIN32__)
#include <commctrl.h>
#endif

#if !USE_SHARED_LIBRARY
IMPLEMENT_DYNAMIC_CLASS(wxGauge95, wxControl)
#endif

bool wxGauge95::Create(wxWindow *parent, wxWindowID id,
           int range,
           const wxPoint& pos,
           const wxSize& size,
           long style,
           const wxValidator& validator,
           const wxString& name)
{
  SetName(name);
  SetValidator(validator);

  if (parent) parent->AddChild(this);
  m_rangeMax = range;
  m_gaugePos = 0;

  SetBackgroundColour(parent->GetBackgroundColour()) ;
  SetForegroundColour(parent->GetForegroundColour()) ;

  m_windowStyle = style;

  if ( id == -1 )
  	m_windowId = (int)NewControlId();
  else
	m_windowId = id;

  int x = pos.x;
  int y = pos.y;
  int width = size.x;
  int height = size.y;

  long msFlags = WS_CHILD | WS_VISIBLE | WS_TABSTOP;

#ifndef PBS_VERTICAL
#define PBS_VERTICAL 0x04
#endif

  if (m_windowStyle & wxGA_VERTICAL)
    msFlags |= PBS_VERTICAL;

#ifndef PBS_SMOOTH
#define PBS_SMOOTH 0x01
#endif

  if (m_windowStyle & wxGA_SMOOTH)
    msFlags |= PBS_SMOOTH;

  HWND wx_button =
      CreateWindowEx(MakeExtendedStyle(m_windowStyle), PROGRESS_CLASS, NULL, msFlags,
                    0, 0, 0, 0, (HWND) parent->GetHWND(), (HMENU)m_windowId,
                    wxGetInstance(), NULL);

  m_hWnd = (WXHWND)wx_button;

  // Subclass again for purposes of dialog editing mode
  SubclassWin((WXHWND) wx_button);

  SendMessage((HWND) GetHWND(), PBM_SETRANGE, 0, MAKELPARAM(0, range));

  SetFont(parent->GetFont());

  if (width == -1)
    width = 50;
  if (height == -1)
    height = 28;
  SetSize(x, y, width, height);

  ShowWindow((HWND) GetHWND(), SW_SHOW);

  return TRUE;
}

void wxGauge95::DoSetSize(int x, int y, int width, int height, int sizeFlags)
{
  int currentX, currentY;
  GetPosition(&currentX, &currentY);
  int x1 = x;
  int y1 = y;
  int w1 = width;
  int h1 = height;

  if (x == -1 || (sizeFlags & wxSIZE_ALLOW_MINUS_ONE))
    x1 = currentX;
  if (y == -1 || (sizeFlags & wxSIZE_ALLOW_MINUS_ONE))
    y1 = currentY;

  AdjustForParentClientOrigin(x1, y1, sizeFlags);

  // If we're prepared to use the existing size, then...
  if (width == -1 && height == -1 && ((sizeFlags & wxSIZE_AUTO) != wxSIZE_AUTO))
  {
    GetSize(&w1, &h1);
  }

  // Deal with default size (using -1 values)
  if (w1<=0)
    w1 = DEFAULT_ITEM_WIDTH;

  if (h1<=0)
    h1 = DEFAULT_ITEM_HEIGHT;

  MoveWindow((HWND) GetHWND(), x1, y1, w1, h1, TRUE);
}

void wxGauge95::SetShadowWidth(int w)
{
}

void wxGauge95::SetBezelFace(int w)
{
}

void wxGauge95::SetRange(int r)
{
  m_rangeMax = r;

  SendMessage((HWND) GetHWND(), PBM_SETRANGE, 0, MAKELPARAM(0, r));
}

void wxGauge95::SetValue(int pos)
{
  m_gaugePos = pos;

  SendMessage((HWND) GetHWND(), PBM_SETPOS, pos, 0);
}

int wxGauge95::GetShadowWidth(void) const
{
  return 0;
}

int wxGauge95::GetBezelFace(void) const
{
  return 0;
}

int wxGauge95::GetRange(void) const
{
  return m_rangeMax;
}

int wxGauge95::GetValue(void) const
{
  return m_gaugePos;
}

void wxGauge95::SetForegroundColour(const wxColour& col)
{
  m_foregroundColour = col ;
}

void wxGauge95::SetBackgroundColour(const wxColour& col)
{
  m_backgroundColour = col ;
}

#endif // wxUSE_GAUGE
