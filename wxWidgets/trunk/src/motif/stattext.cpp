/////////////////////////////////////////////////////////////////////////////
// Name:        stattext.cpp
// Purpose:     wxStaticText
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "stattext.h"
#endif

#include "wx/app.h"
#include "wx/stattext.h"

#include <stdio.h>

#if !USE_SHARED_LIBRARY
IMPLEMENT_DYNAMIC_CLASS(wxStaticText, wxControl)
#endif

bool wxStaticText::Create(wxWindow *parent, wxWindowID id,
           const wxString& label,
           const wxPoint& pos,
           const wxSize& size,
           long style,
           const wxString& name)
{
  SetName(name);
  if (parent) parent->AddChild(this);

  SetBackgroundColour(parent->GetDefaultBackgroundColour()) ;
  SetForegroundColour(parent->GetDefaultForegroundColour()) ;

  if ( id == -1 )
  	m_windowId = (int)NewControlId();
  else
	m_windowId = id;

  m_windowStyle = style;

  SetFont(* parent->GetFont());

  // TODO
  return FALSE;
}

void wxStaticText::SetSize(int x, int y, int width, int height, int sizeFlags)
{
    // TODO
}

void wxStaticText::SetLabel(const wxString& label)
{
    // TODO
}

