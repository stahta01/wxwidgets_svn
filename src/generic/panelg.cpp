/////////////////////////////////////////////////////////////////////////////
// Name:        src/generic/panelg.cpp
// Purpose:     wxPanel and the keyboard handling code
// Author:      Julian Smart, Robert Roebling, Vadim Zeitlin
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#ifdef __GNUG__
    #pragma implementation "panelg.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/object.h"
    #include "wx/font.h"
    #include "wx/colour.h"
    #include "wx/settings.h"
    #include "wx/log.h"
#endif

#include "wx/containr.h"
#include "wx/panel.h"

// ----------------------------------------------------------------------------
// wxWin macros
// ----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxPanel, wxWindow)

BEGIN_EVENT_TABLE(wxPanel, wxWindow)
    EVT_SYS_COLOUR_CHANGED(wxPanel::OnSysColourChanged)

    EVT_SIZE(wxPanel::OnSize)

    WX_EVENT_TABLE_CONTROL_CONTAINER(wxPanel)
END_EVENT_TABLE()

// ============================================================================
// implementation
// ============================================================================

WX_DELEGATE_TO_CONTROL_CONTAINER(wxPanel, m_container)

// ----------------------------------------------------------------------------
// wxPanel creation
// ----------------------------------------------------------------------------

void wxPanel::Init()
{
    m_container = new wxControlContainer(this);
}

bool wxPanel::Create(wxWindow *parent, wxWindowID id,
                     const wxPoint& pos,
                     const wxSize& size,
                     long style,
                     const wxString& name)
{
    return wxWindow::Create(parent, id, pos, size, style, name);
}

wxPanel::~wxPanel()
{
    delete m_container;
}

// ----------------------------------------------------------------------------
// misc
// ----------------------------------------------------------------------------

void wxPanel::InitDialog()
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

void wxPanel::OnSize(wxSizeEvent& WXUNUSED(event))
{
#if wxUSE_CONSTRAINTS
    if (GetAutoLayout())
        Layout();
#endif // wxUSE_CONSTRAINTS
}

