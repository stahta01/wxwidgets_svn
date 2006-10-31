///////////////////////////////////////////////////////////////////////////////
// Name:        src/aui/floatpane.cpp
// Purpose:     wxaui: wx advanced user interface - docking window manager
// Author:      Benjamin I. Williams
// Modified by:
// Created:     2005-05-17
// RCS-ID:      $Id$
// Copyright:   (C) Copyright 2005-2006, Kirix Corporation, All Rights Reserved
// Licence:     wxWindows Library Licence, Version 3.1
///////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_AUI

#include "wx/aui/framemanager.h"
#include "wx/aui/floatpane.h"
#include "wx/aui/dockart.h"

#ifndef WX_PRECOMP
#endif

IMPLEMENT_CLASS( wxFloatingPane, wxFloatingPaneBaseClass )

wxFloatingPane::wxFloatingPane(wxWindow* parent,
                wxFrameManager* owner_mgr,
                const wxPaneInfo& pane,
                wxWindowID id /*= wxID_ANY*/,
                long style /*=wxRESIZE_BORDER | wxSYSTEM_MENU | wxCAPTION |
                              wxFRAME_NO_TASKBAR | wxFRAME_FLOAT_ON_PARENT | 
                              wxCLIP_CHILDREN
                           */)
                : wxFloatingPaneBaseClass(parent, id, wxEmptyString,
                        pane.floating_pos, pane.floating_size,
                        style | 
                        (pane.HasCloseButton()?wxCLOSE_BOX:0) |
                        (pane.IsFixed()?0:wxRESIZE_BORDER)
                        )
{
    m_owner_mgr = owner_mgr;
    m_moving = false;
    m_mgr.SetManagedWindow(this);
    m_solid_drag = true;
    
    // find out if the system supports solid window drag.
    // on non-msw systems, this is assumed to be the case
    #ifdef __WXMSW__
    BOOL b = TRUE;
    SystemParametersInfo(38 /*SPI_GETDRAGFULLWINDOWS*/, 0, &b, 0);
    m_solid_drag = b ? true : false;
    #endif
    
    SetExtraStyle(wxWS_EX_PROCESS_IDLE);
}

wxFloatingPane::~wxFloatingPane()
{
    m_mgr.UnInit();
}

void wxFloatingPane::SetPaneWindow(const wxPaneInfo& pane)
{
    m_pane_window = pane.window;
    m_pane_window->Reparent(this);

    wxPaneInfo contained_pane = pane;
    contained_pane.Dock().Center().Show().
                    CaptionVisible(false).
                    PaneBorder(false).
                    Layer(0).Row(0).Position(0);

    // Carry over the minimum size
    SetMinSize(pane.window->GetMinSize());

    m_mgr.AddPane(m_pane_window, contained_pane);
    m_mgr.Update();

    if (pane.min_size.IsFullySpecified())
    {
        // because SetSizeHints() calls Fit() too (which sets the window
        // size to its minimum allowed), we keep the size before calling
        // SetSizeHints() and reset it afterwards...
        wxSize tmp = GetSize();
        GetSizer()->SetSizeHints(this);
        SetSize(tmp);
    }

    SetTitle(pane.caption);

    if (pane.floating_size != wxDefaultSize)
    {
        SetSize(pane.floating_size);
    }
        else
    {
        wxSize size = pane.best_size;
        if (size == wxDefaultSize)
            size = pane.min_size;
        if (size == wxDefaultSize)
            size = m_pane_window->GetSize();
        if (pane.HasGripper())
        {
            if (pane.HasGripperTop())
                size.y += m_owner_mgr->m_art->GetMetric(wxAUI_ART_GRIPPER_SIZE);
            else
                size.x += m_owner_mgr->m_art->GetMetric(wxAUI_ART_GRIPPER_SIZE);
        }

        SetClientSize(size);
    }
}

void wxFloatingPane::OnSize(wxSizeEvent& event)
{
    m_owner_mgr->OnFloatingPaneResized(m_pane_window, event.GetSize());
}

void wxFloatingPane::OnClose(wxCloseEvent& evt)
{
    m_owner_mgr->OnFloatingPaneClosed(m_pane_window, evt);
    if (!evt.GetVeto())
        Destroy();
}

void wxFloatingPane::OnMoveEvent(wxMoveEvent& event)
{
    if (!m_solid_drag)
    {
        // systems without solid window dragging need to be
        // handled slightly differently, due to the lack of
        // the constant stream of EVT_MOVING events
        if (!isMouseDown())
            return;
        OnMoveStart();
        OnMoving(event.GetRect(), wxNORTH);
        m_moving = true;
        return;
    }
    
    
    wxRect win_rect = GetRect();

    if (win_rect == m_last_rect)
        return;

    // skip the first move event
    if (m_last_rect.IsEmpty())
    {
        m_last_rect = win_rect;
        return;
    }

    // skip if moving too fast to avoid massive redraws and 
    // jumping hint windows
    if ((abs(win_rect.x - m_last_rect.x) > 3) ||
        (abs(win_rect.y - m_last_rect.y) > 3))
    {
        m_last3_rect = m_last2_rect;
        m_last2_rect = m_last_rect;
        m_last_rect = win_rect;
        return;
    }

    // prevent frame redocking during resize
    if (m_last_rect.GetSize() != win_rect.GetSize())
    {
        m_last3_rect = m_last2_rect;
        m_last2_rect = m_last_rect;
        m_last_rect = win_rect;
        return;
    }

    wxDirection dir = wxALL;
    
    int horiz_dist = abs(win_rect.x - m_last3_rect.x);
    int vert_dist = abs(win_rect.y - m_last3_rect.y);
    
    if (vert_dist >= horiz_dist)
    {
        if (win_rect.y < m_last3_rect.y)
            dir = wxNORTH;
        else
            dir = wxSOUTH;
    }
    else
    {
        if (win_rect.x < m_last3_rect.x)
            dir = wxWEST;
        else
            dir = wxEAST;
    }
    
    m_last3_rect = m_last2_rect;
    m_last2_rect = m_last_rect;
    m_last_rect = win_rect;

    if (!isMouseDown())
        return;

    if (!m_moving)
    {
        OnMoveStart();
        m_moving = true;
    }

    if (m_last3_rect.IsEmpty())
        return;
        
    OnMoving(event.GetRect(), dir);
}

void wxFloatingPane::OnIdle(wxIdleEvent& event)
{
    if (m_moving)
    {
        if (!isMouseDown())
        {
            m_moving = false;
            OnMoveFinished();
        }
         else
        {
            event.RequestMore();
        }
    }
}

void wxFloatingPane::OnMoveStart()
{
    // notify the owner manager that the pane has started to move
    m_owner_mgr->OnFloatingPaneMoveStart(m_pane_window);
}

void wxFloatingPane::OnMoving(const wxRect& WXUNUSED(window_rect), wxDirection dir)
{
    // notify the owner manager that the pane is moving
    m_owner_mgr->OnFloatingPaneMoving(m_pane_window, dir);
    m_lastDirection = dir;
}

void wxFloatingPane::OnMoveFinished()
{
    // notify the owner manager that the pane has finished moving
    m_owner_mgr->OnFloatingPaneMoved(m_pane_window, m_lastDirection);
}

void wxFloatingPane::OnActivate(wxActivateEvent& event)
{
    if (event.GetActive())
    {
        m_owner_mgr->OnFloatingPaneActivated(m_pane_window);
    }
}

// utility function which determines the state of the mouse button
// (independant of having a wxMouseEvent handy) - utimately a better
// mechanism for this should be found (possibly by adding the
// functionality to wxWidgets itself)
bool wxFloatingPane::isMouseDown()
{
    return wxGetMouseState().LeftDown();
}


BEGIN_EVENT_TABLE(wxFloatingPane, wxFloatingPaneBaseClass)
    EVT_SIZE(wxFloatingPane::OnSize)
    EVT_MOVE(wxFloatingPane::OnMoveEvent)
    EVT_MOVING(wxFloatingPane::OnMoveEvent)
    EVT_CLOSE(wxFloatingPane::OnClose)
    EVT_IDLE(wxFloatingPane::OnIdle)
    EVT_ACTIVATE(wxFloatingPane::OnActivate)
END_EVENT_TABLE()


#endif // wxUSE_AUI
