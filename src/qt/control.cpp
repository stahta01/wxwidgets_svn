/////////////////////////////////////////////////////////////////////////////
// Name:        control.cpp
// Purpose:     wxControl class
// Author:      AUTHOR
// Modified by:
// Created:     ??/??/98
// RCS-ID:      $Id$
// Copyright:   (c) AUTHOR
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "control.h"
#endif

#include "wx/control.h"

IMPLEMENT_ABSTRACT_CLASS(wxControl, wxWindow)

BEGIN_EVENT_TABLE(wxControl, wxWindow)
END_EVENT_TABLE()

// Item members
wxControl::wxControl()
{
    m_backgroundColour = *wxWHITE;
    m_foregroundColour = *wxBLACK;
    m_callback = 0;
}

wxControl::~wxControl()
{
    // If we delete an item, we should initialize the parent panel,
    // because it could now be invalid.
    wxWindow *parent = (wxWindow *)GetParent();
    if (parent)
    {
        if (parent->GetDefaultItem() == this)
            parent->SetDefaultItem(NULL);
    }
}

void wxControl::SetLabel(const wxString& label)
{
    // TODO
}

wxString wxControl::GetLabel() const
{
    // TODO
    return wxString("");
}

/*
 * Allocates control IDs within the appropriate range
 */

int NewControlId()
{
    static int s_controlId = 0;
    s_controlId ++;
    return s_controlId;
}

void wxControl::ProcessCommand (wxCommandEvent & event)
{
  // Tries:
  // 1) A callback function (to become obsolete)
  // 2) OnCommand, starting at this window and working up parent hierarchy
  // 3) OnCommand then calls ProcessEvent to search the event tables.
  if (m_callback)
    {
      (void) (*(m_callback)) (*this, event);
    }
    else
    {
      GetEventHandler()->OnCommand(*this, event);
    }
}

void wxControl::SetClientSize (int width, int height)
{
  SetSize (-1, -1, width, height);
}

void wxControl::Centre (int direction)
{
  int x, y, width, height, panel_width, panel_height, new_x, new_y;

  wxWindow *parent = (wxWindow *) GetParent ();
  if (!parent)
    return;

  parent->GetClientSize (&panel_width, &panel_height);
  GetSize (&width, &height);
  GetPosition (&x, &y);

  new_x = x;
  new_y = y;

  if (direction & wxHORIZONTAL)
    new_x = (int) ((panel_width - width) / 2);

  if (direction & wxVERTICAL)
    new_y = (int) ((panel_height - height) / 2);

  SetSize (new_x, new_y, width, height);
}

