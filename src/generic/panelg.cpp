/////////////////////////////////////////////////////////////////////////////
// Name:        panelg.cpp
// Purpose:     wxPanel
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:   	wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "panelg.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/settings.h"
#endif

#include "wx/generic/panelg.h"

#if !USE_SHARED_LIBRARY
IMPLEMENT_DYNAMIC_CLASS(wxPanel, wxWindow)

BEGIN_EVENT_TABLE(wxPanel, wxWindow)
  EVT_SYS_COLOUR_CHANGED(wxPanel::OnSysColourChanged)
  EVT_NAVIGATION_KEY(wxPanel::OnNavigationKey)
END_EVENT_TABLE()

#endif

wxPanel::wxPanel()
{
}

bool wxPanel::Create(wxWindow *parent, wxWindowID id,
                     const wxPoint& pos,
                     const wxSize& size,
                     long style,
                     const wxString& name)
{
    bool ret = wxWindow::Create(parent, id, pos, size, style, name);

    if ( ret ) 
    {
#ifndef __WXGTK__
        SetBackgroundColour(wxSystemSettings::GetSystemColour(wxSYS_COLOUR_3DFACE));
        SetFont(wxSystemSettings::GetSystemFont(wxSYS_DEFAULT_GUI_FONT));
#endif
    }

    return ret;
}

void wxPanel::InitDialog(void)
{
    wxInitDialogEvent event(GetId());
    event.SetEventObject(this);
    GetEventHandler()->ProcessEvent(event);
}

// Responds to colour changes, and passes event on to children.
void wxPanel::OnSysColourChanged(wxSysColourChangedEvent& event)
{
    SetBackgroundColour(wxSystemSettings::GetSystemColour(wxSYS_COLOUR_3DFACE));
    Refresh();

    // Propagate the event to the non-top-level children
    wxWindow::OnSysColourChanged(event);
}

void wxPanel::OnNavigationKey( wxNavigationKeyEvent& event )
{
    if (m_children.GetCount() < 2) 
    {
        event.Skip();
        return;
    }

    // don't process these ones here
    if (event.IsWindowChange()) 
    {
        event.Skip();
        return;
    }

    wxWindow *winFocus = event.GetCurrentFocus();
    if (!winFocus) winFocus = wxWindow::FindFocus();
    
    if (!winFocus)
    {
        event.Skip();
        return;
    }
    
    wxNode *start_node = m_children.Find( winFocus );
    if (!start_node) start_node = m_children.First();
    
    wxNode *node = event.GetDirection() ? start_node->Next() : start_node->Previous();
	    
    while (node != start_node)
    {
	if (!node)
	{
/*
            if (GetParent() != NULL) 
	    {
                wxNavigationKeyEvent new_event;
                new_event.SetDirection( event.GetDirection() );
                new_event.SetWindowChange(FALSE);
                new_event.SetCurrentFocus( this );

                if (GetParent()->GetEventHandler()->ProcessEvent(new_event))
		{  
		    return;
		}
            }
*/
	    
	    node = event.GetDirection() ? m_children.First() : m_children.Last();
	}
	        
	wxWindow *child = (wxWindow*) node->Data();
		
	if (child->AcceptsFocus())
	{
	    child->SetFocus();
	    return;
	}
		
	node = node->Next();
    }
    
    event.Skip();
}

