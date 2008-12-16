///////////////////////////////////////////////////////////////////////////////
// Name:        src/common/rearrangectrl.cpp
// Purpose:     implementation of classes in wx/rearrangectrl.h
// Author:      Vadim Zeitlin
// Created:     2008-12-15
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

#include "wx/rearrangectrl.h"

// ============================================================================
// wxRearrangeList implementation
// ============================================================================

extern
WXDLLIMPEXP_DATA_CORE(const char) wxRearrangeListNameStr[] = "wxRearrangeList";

BEGIN_EVENT_TABLE(wxRearrangeList, wxCheckListBox)
    EVT_CHECKLISTBOX(wxID_ANY, wxRearrangeList::OnCheck)
END_EVENT_TABLE()

bool wxRearrangeList::Create(wxWindow *parent,
                             wxWindowID id,
                             const wxPoint& pos,
                             const wxSize& size,
                             const wxArrayInt& order,
                             const wxArrayString& items,
                             long style,
                             const wxValidator& validator,
                             const wxString& name)
{
    // construct the array of items in the order in which they should appear in
    // the control
    const size_t count = items.size();
    wxCHECK_MSG( order.size() == count, false, "arrays not in sync" );

    wxArrayString itemsInOrder;
    itemsInOrder.reserve(count);
    size_t n;
    for ( n = 0; n < count; n++ )
    {
        int idx = order[n];
        if ( idx < 0 )
            idx = -idx - 1;
        itemsInOrder.push_back(items[idx]);
    }

    // do create the real control
    if ( !wxCheckListBox::Create(parent, id, pos, size, itemsInOrder,
                                 style, validator, name) )
        return false;

    // and now check all the items which should be initially checked
    for ( n = 0; n < count; n++ )
    {
        if ( order[n] >= 0 )
            Check(n);
    }

    m_order = order;

    return true;
}

bool wxRearrangeList::CanMoveCurrentUp() const
{
    const int sel = GetSelection();
    return sel != wxNOT_FOUND && sel != 0;
}

bool wxRearrangeList::CanMoveCurrentDown() const
{
    const int sel = GetSelection();
    return sel != wxNOT_FOUND && static_cast<unsigned>(sel) != GetCount() - 1;
}

bool wxRearrangeList::MoveCurrentUp()
{
    const int sel = GetSelection();
    if ( sel == wxNOT_FOUND || sel == 0 )
        return false;

    Swap(sel, sel - 1);
    SetSelection(sel - 1);

    return true;
}

bool wxRearrangeList::MoveCurrentDown()
{
    const int sel = GetSelection();
    if ( sel == wxNOT_FOUND || static_cast<unsigned>(sel) == GetCount() - 1 )
        return false;

    Swap(sel, sel + 1);
    SetSelection(sel + 1);

    return true;
}

void wxRearrangeList::Swap(int pos1, int pos2)
{
    wxSwap(m_order[pos1], m_order[pos2]);

    const wxString stringTmp = GetString(pos1);
    const bool checkedTmp = IsChecked(pos1);

    SetString(pos1, GetString(pos2));
    Check(pos1, IsChecked(pos2));

    SetString(pos2, stringTmp);
    Check(pos2, checkedTmp);
}

void wxRearrangeList::OnCheck(wxCommandEvent& event)
{
    // update the internal state to match the new item state
    const int n = event.GetInt();

    const bool checked = IsChecked(n);
    wxASSERT_MSG( (m_order[n] >= 0) == !checked,
                  "discrepancy between internal state and GUI" );

    m_order[n] = ~m_order[n];
}

// ============================================================================
// wxRearrangeCtrl implementation
// ============================================================================

BEGIN_EVENT_TABLE(wxRearrangeCtrl, wxPanel)
    EVT_UPDATE_UI(wxID_UP, wxRearrangeCtrl::OnUpdateButtonUI)
    EVT_UPDATE_UI(wxID_DOWN, wxRearrangeCtrl::OnUpdateButtonUI)

    EVT_BUTTON(wxID_UP, wxRearrangeCtrl::OnButton)
    EVT_BUTTON(wxID_DOWN, wxRearrangeCtrl::OnButton)
END_EVENT_TABLE()

void wxRearrangeCtrl::Init()
{
    m_list = NULL;
}

bool
wxRearrangeCtrl::Create(wxWindow *parent,
                        wxWindowID id,
                        const wxPoint& pos,
                        const wxSize& size,
                        const wxArrayInt& order,
                        const wxArrayString& items,
                        long style,
                        const wxValidator& validator,
                        const wxString& name)
{
    // create all the windows
    if ( !wxPanel::Create(parent, id, pos, size, wxTAB_TRAVERSAL, name) )
        return false;

    m_list = new wxRearrangeList(this, wxID_ANY,
                                 wxDefaultPosition, wxDefaultSize,
                                 order, items,
                                 style, validator);
    wxButton * const btnUp = new wxButton(this, wxID_UP);
    wxButton * const btnDown = new wxButton(this, wxID_DOWN);

    // arrange them in a sizer
    wxSizer * const sizerBtns = new wxBoxSizer(wxVERTICAL);
    sizerBtns->Add(btnUp, wxSizerFlags().Centre().Border(wxBOTTOM));
    sizerBtns->Add(btnDown, wxSizerFlags().Centre().Border(wxTOP));

    wxSizer * const sizerTop = new wxBoxSizer(wxHORIZONTAL);
    sizerTop->Add(m_list, wxSizerFlags(1).Expand().Border(wxRIGHT));
    sizerTop->Add(sizerBtns, wxSizerFlags(0).Centre().Border(wxLEFT));
    SetSizer(sizerTop);

    m_list->SetFocus();

    return true;
}

void wxRearrangeCtrl::OnUpdateButtonUI(wxUpdateUIEvent& event)
{
    event.Enable( event.GetId() == wxID_UP ? m_list->CanMoveCurrentUp()
                                           : m_list->CanMoveCurrentDown() );
}

void wxRearrangeCtrl::OnButton(wxCommandEvent& event)
{
    if ( event.GetId() == wxID_UP )
        m_list->MoveCurrentUp();
    else
        m_list->MoveCurrentDown();
}

// ============================================================================
// wxRearrangeDialog implementation
// ============================================================================

extern
WXDLLIMPEXP_DATA_CORE(const char) wxRearrangeDialogNameStr[] = "wxRearrangeDlg";

wxRearrangeDialog::wxRearrangeDialog(wxWindow *parent,
                                     const wxString& message,
                                     const wxString& title,
                                     const wxArrayInt& order,
                                     const wxArrayString& items,
                                     const wxPoint& pos,
                                     const wxString& name)
                 : wxDialog(parent, wxID_ANY, title,
                            pos, wxDefaultSize,
                            wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER,
                            name)
{
    m_ctrl = new wxRearrangeCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                 order, items);

    wxSizer * const sizerTop = new wxBoxSizer(wxVERTICAL);
    sizerTop->Add(new wxStaticText(this, wxID_ANY, message),
                  wxSizerFlags().DoubleBorder());
    sizerTop->Add(m_ctrl,
                  wxSizerFlags(1).Expand().Border());
    sizerTop->Add(CreateSeparatedButtonSizer(wxOK | wxCANCEL),
                  wxSizerFlags().Expand().Border());
    SetSizerAndFit(sizerTop);
}
