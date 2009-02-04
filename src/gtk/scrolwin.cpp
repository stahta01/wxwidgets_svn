/////////////////////////////////////////////////////////////////////////////
// Name:        gtk/scrolwin.cpp
// Purpose:     wxScrolledWindow implementation
// Author:      Robert Roebling
// Modified by: Ron Lee
//              Vadim Zeitlin: removed 90% of duplicated common code
// Created:     01/02/97
// RCS-ID:      $Id$
// Copyright:   (c) Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/scrolwin.h"
#include "wx/gtk/private.h"

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxScrollHelper implementation
// ----------------------------------------------------------------------------

void wxScrollHelper::SetScrollbars(int pixelsPerUnitX, int pixelsPerUnitY,
                                   int noUnitsX, int noUnitsY,
                                   int xPos, int yPos,
                                   bool noRefresh)
{
    int old_x = m_xScrollPosition * m_xScrollPixelsPerLine;
    int old_y = m_yScrollPosition * m_yScrollPixelsPerLine;

    m_xScrollPixelsPerLine = pixelsPerUnitX;
    m_yScrollPixelsPerLine = pixelsPerUnitY;

    m_win->m_scrollBar[wxWindow::ScrollDir_Horz]->adjustment->value =
    m_xScrollPosition = xPos;
    m_win->m_scrollBar[wxWindow::ScrollDir_Vert]->adjustment->value =
    m_yScrollPosition = yPos;

    // To get everything right, have to call ScrollWindow()
    // both before and after calling SetVirtualSize()
    int new_x = m_xScrollPosition * m_xScrollPixelsPerLine;
    int new_y = m_yScrollPosition * m_yScrollPixelsPerLine;
    if (!noRefresh)
    {
        m_targetWindow->ScrollWindow(old_x - new_x, old_y - new_y);
        old_x = new_x;
        old_y = new_y;
    }

    int w = noUnitsX * pixelsPerUnitX;
    int h = noUnitsY * pixelsPerUnitY;
    m_targetWindow->SetVirtualSize( w ? w : wxDefaultCoord,
                                    h ? h : wxDefaultCoord);

    if (!noRefresh)
    {
        new_x = m_xScrollPosition * m_xScrollPixelsPerLine;
        new_y = m_yScrollPosition * m_yScrollPixelsPerLine;
        m_targetWindow->ScrollWindow(old_x - new_x, old_y - new_y);
    }

    // If the target is not the same as the window with the scrollbars,
    // then we need to update the scrollbars here, since they won't have
    // been updated by SetVirtualSize().
    if (m_targetWindow != m_win)
    {
        AdjustScrollbars();
    }
}

void wxScrollHelper::DoAdjustScrollbar(GtkRange* range,
                                       int pixelsPerLine,
                                       int winSize,
                                       int virtSize,
                                       int *pos,
                                       int *lines,
                                       int *linesPerPage)
{
    int upper;
    int page_size;
    if (pixelsPerLine > 0 && winSize > 0 && winSize < virtSize)
    {
        upper = (virtSize + pixelsPerLine - 1) / pixelsPerLine;
        page_size = winSize / pixelsPerLine;
        *lines = upper;
        *linesPerPage = page_size;
    }
    else
    {
        // GtkRange won't allow upper == lower, so for disabled state use [0,1]
        //   with a page size of 1. This will also clamp position to 0.
        upper = 1;
        page_size = 1;
        *lines = 0;
        *linesPerPage = 0;
    }

    GtkAdjustment* adj = range->adjustment;
    adj->step_increment = 1;
    adj->page_increment = 
    adj->page_size = page_size;
    gtk_range_set_range(range, 0, upper);

    // ensure that the scroll position is always in valid range
    if (*pos > *lines)
        *pos = *lines;
}

void wxScrollHelper::AdjustScrollbars()
{
    int vw, vh;
    m_targetWindow->GetVirtualSize(&vw, &vh);

    int w, h;
    const wxSize availSize = GetSizeAvailableForScrollTarget(
        m_win->GetSize() - m_win->GetWindowBorderSize());
    if ( availSize.x >= vw && availSize.y >= vh )
    {
        w = availSize.x;
        h = availSize.y;

        // we know that the scrollbars will be removed
        DoAdjustHScrollbar(w, vw);
        DoAdjustVScrollbar(h, vh);

        return;
    }

    m_targetWindow->GetClientSize(&w, NULL);
    DoAdjustHScrollbar(w, vw);

    m_targetWindow->GetClientSize(NULL, &h);
    DoAdjustVScrollbar(h, vh);

    const int w_old = w;
    m_targetWindow->GetClientSize(&w, NULL);
    if ( w != w_old )
    {
        // It is necessary to repeat the calculations in this case to avoid an
        // observed infinite series of size events, involving alternating
        // changes in visibility of the scrollbars.
        // At this point, GTK+ has already queued a resize, which will cause
        // AdjustScrollbars() to be called again. If the scrollbar visibility
        // is not correct before then, yet another resize will occur, possibly
        // leading to an unending series if the sizes are just right.
        DoAdjustHScrollbar(w, vw);

        m_targetWindow->GetClientSize(NULL, &h);
        DoAdjustVScrollbar(h, vh);
    }
}

void wxScrollHelper::DoScrollOneDir(int orient,
                                    int pos,
                                    int pixelsPerLine,
                                    int *posOld)
{
    if ( pos != -1 && pos != *posOld && pixelsPerLine )
    {
        m_win->SetScrollPos(orient, pos);
        pos = m_win->GetScrollPos(orient);

        int diff = (*posOld - pos)*pixelsPerLine;
        m_targetWindow->ScrollWindow(orient == wxHORIZONTAL ? diff : 0,
                                     orient == wxHORIZONTAL ? 0 : diff);

        *posOld = pos;
    }
}

void wxScrollHelper::DoScroll( int x_pos, int y_pos )
{
    wxCHECK_RET( m_targetWindow != 0, _T("No target window") );

    DoScrollOneDir(wxHORIZONTAL, x_pos, m_xScrollPixelsPerLine, &m_xScrollPosition);
    DoScrollOneDir(wxVERTICAL, y_pos, m_yScrollPixelsPerLine, &m_yScrollPosition);
}

// ----------------------------------------------------------------------------
// scrollbars visibility
// ----------------------------------------------------------------------------

namespace
{

GtkPolicyType GtkPolicyFromWX(wxScrollbarVisibility visibility)
{
    GtkPolicyType policy;
    switch ( visibility )
    {
        case wxSHOW_SB_NEVER:
            policy = GTK_POLICY_NEVER;
            break;

        case wxSHOW_SB_DEFAULT:
            policy = GTK_POLICY_AUTOMATIC;
            break;

        case wxSHOW_SB_ALWAYS:
            policy = GTK_POLICY_ALWAYS;
            break;
    }

    return policy;
}

} // anonymous namespace

void wxScrollHelper::DoShowScrollbars(wxScrollbarVisibility horz,
                                      wxScrollbarVisibility vert)
{
    GtkScrolledWindow * const scrolled = GTK_SCROLLED_WINDOW(m_win->m_widget);
    wxCHECK_RET( scrolled, "window must be created" );

    gtk_scrolled_window_set_policy(scrolled,
                                   GtkPolicyFromWX(horz),
                                   GtkPolicyFromWX(vert));
}

