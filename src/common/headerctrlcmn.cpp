///////////////////////////////////////////////////////////////////////////////
// Name:        src/common/headerctrlcmn.cpp
// Purpose:     implementation of wxHeaderCtrlBase
// Author:      Vadim Zeitlin
// Created:     2008-12-02
// RCS-ID:      $Id$
// Copyright:   (c) 2008 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// for compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
#endif // WX_PRECOMP

#include "wx/headerctrl.h"

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

namespace
{

const unsigned int wxNO_COLUMN = static_cast<unsigned>(-1);

} // anonymous namespace

// ============================================================================
// wxHeaderCtrlBase implementation
// ============================================================================

extern WXDLLIMPEXP_DATA_CORE(const char) wxHeaderCtrlNameStr[] = "wxHeaderCtrl";

BEGIN_EVENT_TABLE(wxHeaderCtrlBase, wxControl)
    EVT_HEADER_SEPARATOR_DCLICK(wxID_ANY, wxHeaderCtrlBase::OnSeparatorDClick)
END_EVENT_TABLE()

void wxHeaderCtrlBase::ScrollWindow(int dx,
                                    int WXUNUSED_UNLESS_DEBUG(dy),
                                    const wxRect * WXUNUSED_UNLESS_DEBUG(rect))

{
    // this doesn't make sense at all
    wxASSERT_MSG( !dy, "header window can't be scrolled vertically" );

    // this would actually be nice to support for "frozen" headers but it isn't
    // supported currently
    wxASSERT_MSG( !rect, "header window can't be scrolled partially" );

    DoScrollHorz(dx);
}

void wxHeaderCtrlBase::OnSeparatorDClick(wxHeaderCtrlEvent& event)
{
    const unsigned col = event.GetColumn();

    int w = wxWindowBase::GetTextExtent(GetColumn(col).GetTitle()).x;
    w += 2*GetCharWidth(); // add some arbitrary margins around text

    if ( !UpdateColumnWidthToFit(col, w) )
        event.Skip();
    else
        UpdateColumn(col);
}

// ============================================================================
// wxHeaderCtrlSimple implementation
// ============================================================================

void wxHeaderCtrlSimple::Init()
{
    m_sortKey = wxNO_COLUMN;
}

wxHeaderColumnBase& wxHeaderCtrlSimple::GetColumn(unsigned int idx)
{
    return m_cols[idx];
}

void wxHeaderCtrlSimple::DoInsert(const wxHeaderColumnSimple& col, unsigned int idx)
{
    m_cols.insert(m_cols.begin() + idx, col);

    UpdateColumnCount();
}

void wxHeaderCtrlSimple::DoDelete(unsigned int idx)
{
    m_cols.erase(m_cols.begin() + idx);
    if ( idx == m_sortKey )
        m_sortKey = wxNO_COLUMN;

    UpdateColumnCount();
}

void wxHeaderCtrlSimple::DeleteAllColumns()
{
    m_cols.clear();
    m_sortKey = wxNO_COLUMN;

    UpdateColumnCount();
}


void wxHeaderCtrlSimple::DoShowColumn(unsigned int idx, bool show)
{
    if ( show != m_cols[idx].IsShown() )
    {
        m_cols[idx].SetHidden(!show);

        UpdateColumn(idx);
    }
}

void wxHeaderCtrlSimple::DoShowSortIndicator(unsigned int idx, bool ascending)
{
    RemoveSortIndicator();

    m_cols[idx].SetAsSortKey(ascending);
    m_sortKey = idx;

    UpdateColumn(idx);
}

void wxHeaderCtrlSimple::RemoveSortIndicator()
{
    if ( m_sortKey != wxNO_COLUMN )
    {
        const unsigned sortOld = m_sortKey;
        m_sortKey = wxNO_COLUMN;

        m_cols[sortOld].UnsetAsSortKey();

        UpdateColumn(sortOld);
    }
}

bool
wxHeaderCtrlSimple::UpdateColumnWidthToFit(unsigned int idx, int widthTitle)
{
    const int widthContents = GetBestFittingWidth(idx);
    if ( widthContents == -1 )
        return false;

    m_cols[idx].SetWidth(wxMax(widthContents, widthTitle));
    UpdateColumn(idx);

    return true;
}

// ============================================================================
// wxHeaderCtrlEvent implementation
// ============================================================================

IMPLEMENT_DYNAMIC_CLASS(wxHeaderCtrlEvent, wxNotifyEvent)

const wxEventType wxEVT_COMMAND_HEADER_CLICK = wxNewEventType();
const wxEventType wxEVT_COMMAND_HEADER_RIGHT_CLICK = wxNewEventType();
const wxEventType wxEVT_COMMAND_HEADER_MIDDLE_CLICK = wxNewEventType();

const wxEventType wxEVT_COMMAND_HEADER_DCLICK = wxNewEventType();
const wxEventType wxEVT_COMMAND_HEADER_RIGHT_DCLICK = wxNewEventType();
const wxEventType wxEVT_COMMAND_HEADER_MIDDLE_DCLICK = wxNewEventType();

const wxEventType wxEVT_COMMAND_HEADER_SEPARATOR_DCLICK = wxNewEventType();
