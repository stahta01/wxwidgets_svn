/////////////////////////////////////////////////////////////////////////////
// Name:        msw/radiobut.cpp
// Purpose:     wxRadioButton
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
    #pragma implementation "radiobut.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_RADIOBTN

#ifndef WX_PRECOMP
    #include "wx/radiobut.h"
    #include "wx/settings.h"
    #include "wx/brush.h"
#endif

#include "wx/msw/private.h"

IMPLEMENT_DYNAMIC_CLASS(wxRadioButton, wxControl)
// IMPLEMENT_DYNAMIC_CLASS(wxBitmapRadioButton, wxRadioButton)

bool wxRadioButton::MSWCommand(WXUINT param, WXWORD WXUNUSED(id))
{
  if (param == BN_CLICKED)
  {
    wxCommandEvent event(wxEVT_COMMAND_RADIOBUTTON_SELECTED, m_windowId);
    event.SetEventObject( this );
    event.SetInt( GetValue() );
    ProcessCommand(event);
    return TRUE;
  }
  else return FALSE;
}

bool wxRadioButton::Create(wxWindow *parent, wxWindowID id,
       const wxString& label,
           const wxPoint& pos,
           const wxSize& size, long style,
           const wxValidator& validator,
           const wxString& name)
{
  SetName(name);
#if wxUSE_VALIDATORS
  SetValidator(validator);
#endif // wxUSE_VALIDATORS

  if (parent) parent->AddChild(this);

  SetBackgroundColour(parent->GetBackgroundColour());
  SetForegroundColour(parent->GetForegroundColour());

  if ( id == -1 )
    m_windowId = (int)NewControlId();
  else
  m_windowId = id;

  int x = pos.x;
  int y = pos.y;
  int width = size.x;
  int height = size.y;

  m_windowStyle = style ;

  long groupStyle = 0;
  if (m_windowStyle & wxRB_GROUP)
    groupStyle = WS_GROUP;

//  long msStyle = groupStyle | RADIO_FLAGS;
  long msStyle = groupStyle | BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE /* | WS_CLIPSIBLINGS */;

  if ( m_windowStyle & wxCLIP_SIBLINGS )
        msStyle |= WS_CLIPSIBLINGS;


  bool want3D;
  WXDWORD exStyle = Determine3DEffects(0, &want3D) ;

  // Even with extended styles, need to combine with WS_BORDER
  // for them to look right.
/*
  if ( want3D || wxStyleHasBorder(m_windowStyle) )
    msStyle |= WS_BORDER;
*/

  m_hWnd = (WXHWND) CreateWindowEx(exStyle, RADIO_CLASS, (const wxChar *)label,
                          msStyle,0,0,0,0,
                          (HWND) parent->GetHWND(), (HMENU)m_windowId, wxGetInstance(), NULL);

  wxCHECK_MSG( m_hWnd, FALSE, wxT("Failed to create radiobutton") );

#if wxUSE_CTL3D
  if (want3D)
  {
    Ctl3dSubclassCtl((HWND) m_hWnd);
   m_useCtl3D = TRUE;
  }
#endif

  SetFont(parent->GetFont());

  // Subclass again for purposes of dialog editing mode
  SubclassWin((WXHWND)m_hWnd);

//  SetValue(value);

  // start GRW fix
  if (label != wxT(""))
  {
    int label_width, label_height;
    GetTextExtent(label, &label_width, &label_height, NULL, NULL, & this->GetFont());
    if (width < 0)
      width = (int)(label_width + RADIO_SIZE);
    if (height<0)
    {
      height = (int)(label_height);
      if (height < RADIO_SIZE)
        height = RADIO_SIZE;
    }
  }
  else
  {
    if (width < 0)
      width = RADIO_SIZE;
    if (height < 0)
      height = RADIO_SIZE;
  }
  // end GRW fix

  SetSize(x, y, width, height);

  // for compatibility with wxGTK, the first radio button in a group is
  // always checked (this makes sense anyhow as you need to ensure that at
  // least one button in the group is checked and this is the simlpest way to
  // do it)
  if ( m_windowStyle & wxRB_GROUP )
      SetValue(TRUE);

  return TRUE;
}


void wxRadioButton::SetLabel(const wxString& label)
{
  SetWindowText((HWND) GetHWND(), (const wxChar *)label);
}

void wxRadioButton::SetValue(bool value)
{
// Following necessary for Win32s, because Win32s translate BM_SETCHECK
  SendMessage((HWND) GetHWND(), BM_SETCHECK, (WPARAM)value, 0L);
}

// Get single selection
bool wxRadioButton::GetValue(void) const
{
  return (SendMessage((HWND) GetHWND(), BM_GETCHECK, 0, 0L) != 0);
}

void wxRadioButton::Command (wxCommandEvent & event)
{
  SetValue ( (event.m_commandInt != 0) );
  ProcessCommand (event);
}

WXHBRUSH wxRadioButton::OnCtlColor(WXHDC pDC, WXHWND WXUNUSED(pWnd), WXUINT WXUNUSED(nCtlColor),
#if wxUSE_CTL3D
                               WXUINT message,
                               WXWPARAM wParam,
                               WXLPARAM lParam
#else
                               WXUINT WXUNUSED(message),
                               WXWPARAM WXUNUSED(wParam),
                               WXLPARAM WXUNUSED(lParam)
#endif
    )
{
#if wxUSE_CTL3D
    if ( m_useCtl3D )
    {
        HBRUSH hbrush = Ctl3dCtlColorEx(message, wParam, lParam);
        return (WXHBRUSH) hbrush;
    }
#endif // wxUSE_CTL3D

    HDC hdc = (HDC)pDC;
    if (GetParent()->GetTransparentBackground())
        SetBkMode(hdc, TRANSPARENT);
    else
        SetBkMode(hdc, OPAQUE);

    wxColour colBack = GetBackgroundColour();

    if (!IsEnabled())
        colBack = wxSystemSettings::GetSystemColour(wxSYS_COLOUR_3DFACE);

    ::SetBkColor(hdc, wxColourToRGB(colBack));
    ::SetTextColor(hdc, wxColourToRGB(GetForegroundColour()));

    wxBrush *brush = wxTheBrushList->FindOrCreateBrush(colBack, wxSOLID);

    return (WXHBRUSH)brush->GetResourceHandle();
}

// Not implemented
#if 0
bool wxBitmapRadioButton::Create(wxWindow *parent, wxWindowID id,
       const wxBitmap *bitmap,
           const wxPoint& pos,
           const wxSize& size, long style,
           const wxValidator& validator,
           const wxString& name)
{
  SetName(name);
  SetValidator(validator);

  if (parent) parent->AddChild(this);
  SetBackgroundColour(parent->GetBackgroundColour());
  SetForegroundColour(parent->GetForegroundColour());

  if ( id == -1 )
    m_windowId = (int)NewControlId();
  else
  m_windowId = id;

  int x = pos.x;
  int y = pos.y;
  int width = size.x;
  int height = size.y;
  m_windowStyle = style ;

  long groupStyle = 0;
  if (m_windowStyle & wxRB_GROUP)
    groupStyle = WS_GROUP;

//  long msStyle = groupStyle | RADIO_FLAGS;
  long msStyle = groupStyle | BS_RADIOBUTTON | WS_CHILD | WS_VISIBLE ;

  m_hWnd = (WXHWND) CreateWindowEx(MakeExtendedStyle(m_windowStyle), RADIO_CLASS, "toggle",
                          msStyle,0,0,0,0,
                          (HWND) parent->GetHWND(), (HMENU)m_windowId, wxGetInstance(), NULL);

  wxCHECK_MSG( m_hWnd, "Failed to create radio button", FALSE );

#if wxUSE_CTL3D
  if (!(GetParent()->GetWindowStyleFlag() & wxUSER_COLOURS))
  {
    Ctl3dSubclassCtl((HWND) GetHWND());
    m_useCtl3D = TRUE;
  }
#endif

  // Subclass again for purposes of dialog editing mode
  SubclassWin(GetHWND());

  SetSize(x, y, width, height);

  return TRUE;
}

void wxBitmapRadioButton::SetLabel(const wxBitmap *bitmap)
{
}

void wxBitmapRadioButton::SetValue(bool value)
{
// Following necessary for Win32s, because Win32s translate BM_SETCHECK
  SendMessage((HWND) GetHWND(), BM_SETCHECK, (WPARAM)value, 0L);
}

// Get single selection, for single choice list items
bool wxBitmapRadioButton::GetValue(void) const
{
  return (bool)SendMessage((HWND) GetHWND(), BM_GETCHECK, 0, 0L);
}

#endif

#endif // wxUSE_RADIOBTN
