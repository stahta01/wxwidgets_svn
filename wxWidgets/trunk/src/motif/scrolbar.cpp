/////////////////////////////////////////////////////////////////////////////
// Name:        scrolbar.cpp
// Purpose:     wxScrollBar
// Author:      Julian Smart
// Modified by:
// Created:     17/09/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "scrolbar.h"
#endif

#include "wx/scrolbar.h"

#if !USE_SHARED_LIBRARY
IMPLEMENT_DYNAMIC_CLASS(wxScrollBar, wxControl)

#endif

// Scrollbar
bool wxScrollBar::Create(wxWindow *parent, wxWindowID id,
           const wxPoint& pos,
           const wxSize& size, long style,
           const wxValidator& validator,
           const wxString& name)
{
    if (!parent)
        return FALSE;
    parent->AddChild(this);
    SetName(name);
	SetValidator(validator);
    
    m_windowStyle = style;

    if ( id == -1 )
  	    m_windowId = (int)NewControlId();
    else
	    m_windowId = id;

    // TODO create scrollbar
    return TRUE;
}

wxScrollBar::~wxScrollBar()
{
}

void wxScrollBar::SetPosition(int viewStart)
{
    // TODO
}

int wxScrollBar::GetPosition() const
{
    // TODO
    return 0;
}

void wxScrollBar::SetScrollbar(int position, int thumbSize, int range, int pageSize,
    bool refresh)
{
    m_viewSize = pageSize;
    m_pageSize = thumbSize;
    m_objectSize = range;

    // TODO
}


void wxScrollBar::Command(wxCommandEvent& event)
{
    SetPosition(event.m_commandInt);
    ProcessCommand(event);
}

