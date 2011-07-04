/////////////////////////////////////////////////////////////////////////////
// Name:        src/mobile/generic/mo_listbox_n.mm
// Purpose:     wxMoListBox class
// Author:      Julian Smart, Linas Valiukas
// Modified by:
// Created:     2011-06-14
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart, Linas Valiukas
// Licence:     wxWindows Licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_LISTBOX

#include "wx/dcscreen.h"
#include "wx/settings.h"

#include "wx/listbox.h"
#include "wx/mobile/native/listbox.h"

IMPLEMENT_CLASS(wxMoListBox, wxListBox)

BEGIN_EVENT_TABLE(wxMoListBox, wxListBox)
    EVT_WHEEL_SELECTED(wxID_ANY, wxMoListBox::OnWheelSelected)
END_EVENT_TABLE()


/// Default constructor.
wxMoListBox::wxMoListBox()
{
    Init();
}

/// Constructor.
wxMoListBox::wxMoListBox(wxWindow *parent,
                         wxWindowID id,
                         const wxPoint& pos,
                         const wxSize& size,
                         const wxArrayString& strings,
                         long style,
                         const wxValidator& validator,
                         const wxString& name)
{
    Init();
    
    Create(parent, id, pos, size, strings, style, validator, name);
}

wxMoListBox::~wxMoListBox()
{
    
}

void wxMoListBox::Init()
{
    
}

bool wxMoListBox::Create(wxWindow *parent,
                         wxWindowID id,
                         const wxPoint& pos,
                         const wxSize& size,
                         const wxArrayString& strings,
                         long style,
                         const wxValidator& validator,
                         const wxString& name)
{
    return wxListBox::Create(parent, id, pos, size, strings, style, validator, name);
}

#if 0
wxMoWheelsTextDataSource* wxMoListBox::GetTextDataSource() const
{
    // FIXME stub

    return NULL;
}

void wxMoListBox::Clear()
{
    // FIXME stub
}

void wxMoListBox::Set(const wxArrayString& strings)
{
    // FIXME stub
}

void wxMoListBox::Append(const wxString& str)
{
    // FIXME stub
}

int wxMoListBox::Insert(const wxString& str, int pos)
{
    // FIXME stub
    
    return 0;
}

void wxMoListBox::InsertItems(const wxArrayString& items, int pos)
{
    // FIXME stub
}

wxString wxMoListBox::GetStringSelection() const
{
    // FIXME stub

    return wxEmptyString;
}

bool wxMoListBox::SetStringSelection(const wxString& str)
{
    // FIXME stub
    return false;
}

wxString wxMoListBox::GetString(int n) const
{
    // FIXME stub

    return wxEmptyString;
}

void wxMoListBox::SetString(int n, const wxString& str)
{
    // FIXME stub
}

wxArrayString wxMoListBox::GetStrings() const
{
    // FIXME stub

    wxArrayString empty;
    return empty;
}

int wxMoListBox::GetSelections(wxArrayInt& selections) const
{
    // FIXME stub

    return 0;
}

// Returns the number of strings in the listbox.
unsigned int wxMoListBox::GetCount() const
{
    // FIXME stub

    return 0;
}

wxSize wxMoListBox::DoGetBestSize() const
{
    // FIXME stub

    wxSize empty(1, 1);
    return empty;
}
#endif  // 0

void wxMoListBox::OnWheelSelected(wxWheelsCtrlEvent& event)
{
    // FIXME stub
}

#endif  // wxUSE_LISTBOX
