/////////////////////////////////////////////////////////////////////////////
// Name:        control.cpp
// Purpose:     wxControl class
// Author:      Julian Smart
// Modified by:
// Created:     17/09/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "control.h"
#endif

#include "wx/control.h"
#include "wx/utils.h"

#include <Xm/Xm.h>

#if !USE_SHARED_LIBRARY
IMPLEMENT_ABSTRACT_CLASS(wxControl, wxWindow)

BEGIN_EVENT_TABLE(wxControl, wxWindow)
END_EVENT_TABLE()
#endif

// Item members
wxControl::wxControl()
{
    m_backgroundColour = *wxWHITE;
    m_foregroundColour = *wxBLACK;
    m_callback = 0;
    m_inSetValue = FALSE;
}

wxControl::~wxControl()
{
    // If we delete an item, we should initialize the parent panel,
    // because it could now be invalid.
    wxWindow *parent = (wxWindow *)GetParent();
    if (parent)
    {
        if (parent->GetDefaultItem() == (wxButton*) this)
            parent->SetDefaultItem(NULL);
    }
}

void wxControl::SetLabel(const wxString& label)
{
    Widget widget = (Widget) GetLabelWidget() ;
    if (!widget)
        return;
    
    wxStripMenuCodes((char*) (const char*) label, wxBuffer);
    
    XmString text = XmStringCreateSimple (wxBuffer);
    XtVaSetValues (widget,
        XmNlabelString, text,
        XmNlabelType, XmSTRING,
        NULL);
    XmStringFree (text);
}

wxString wxControl::GetLabel() const
{
    Widget widget = (Widget) GetLabelWidget() ;
    if (!widget)
        return wxEmptyString;
    
    XmString text;
    char *s;
    XtVaGetValues (widget,
        XmNlabelString, &text,
        NULL);
    
    if (XmStringGetLtoR (text, XmSTRING_DEFAULT_CHARSET, &s))
    {
        wxString str(s);
        XtFree (s);
        XmStringFree(text);
        return str;
    }
    else
    {
        XmStringFree(text);
        return wxEmptyString;
    }
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

