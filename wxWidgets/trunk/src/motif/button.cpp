/////////////////////////////////////////////////////////////////////////////
// Name:        button.cpp
// Purpose:     wxButton
// Author:      Julian Smart
// Modified by:
// Created:     17/09/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "button.h"
#endif

#include "wx/button.h"

#if !USE_SHARED_LIBRARY
IMPLEMENT_DYNAMIC_CLASS(wxButton, wxControl)
#endif

// Button

bool wxButton::Create(wxWindow *parent, wxWindowID id, const wxString& label,
           const wxPoint& pos,
           const wxSize& size, long style,
           const wxValidator& validator,
           const wxString& name)
{
    SetName(name);
    SetValidator(validator);
    m_windowStyle = style;

    parent->AddChild((wxButton *)this);

    if (id == -1)
        m_windowId = NewControlId();
    else
        m_windowId = id;

    // TODO: create button

    return FALSE;
}

void wxButton::SetSize(int x, int y, int width, int height, int sizeFlags)
{
    // TODO
}

void wxButton::SetDefault()
{
    wxWindow *parent = (wxWindow *)GetParent();
    if (parent)
        parent->SetDefaultItem(this);

    // TODO: make button the default
}

wxString wxButton::GetLabel() const
{
    // TODO
    return wxString("");
}

void wxButton::SetLabel(const wxString& label)
{
    // TODO
}

void wxButton::Command (wxCommandEvent & event)
{
    ProcessCommand (event);
}

