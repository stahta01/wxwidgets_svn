/////////////////////////////////////////////////////////////////////////////
// Name:        combobox.cpp
// Purpose:     wxComboBox class
// Author:      AUTHOR
// Modified by:
// Created:     ??/??/98
// RCS-ID:      $Id$
// Copyright:   (c) AUTHOR
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "combobox.h"
#endif

#include "wx/combobox.h"

#if !USE_SHARED_LIBRARY
IMPLEMENT_DYNAMIC_CLASS(wxComboBox, wxControl)
#endif

bool wxComboBox::Create(wxWindow *parent, wxWindowID id,
           const wxString& value,
           const wxPoint& pos,
           const wxSize& size,
		   int n, const wxString choices[],
		   long style,
           const wxValidator& validator,
           const wxString& name)
{
    SetName(name);
    SetValidator(validator);
    m_noStrings = n;
    m_windowStyle = style;

    if (parent) parent->AddChild(this);

    if ( id == -1 )
    	m_windowId = (int)NewControlId();
    else
	m_windowId = id;

    // TODO: create combobox control

    return TRUE;
}

wxString wxComboBox::GetValue() const
{
    // TODO
    return wxString("");
}

void wxComboBox::SetValue(const wxString& value)
{
    // TODO
}

// Clipboard operations
void wxComboBox::Copy()
{
    // TODO
}

void wxComboBox::Cut()
{
    // TODO
}

void wxComboBox::Paste()
{
    // TODO
}

void wxComboBox::SetEditable(bool editable)
{
    // TODO
}

void wxComboBox::SetInsertionPoint(long pos)
{
    // TODO
}

void wxComboBox::SetInsertionPointEnd()
{
    // TODO
}

long wxComboBox::GetInsertionPoint() const
{
    // TODO
    return 0;
}

long wxComboBox::GetLastPosition() const
{
    // TODO
    return 0;
}

void wxComboBox::Replace(long from, long to, const wxString& value)
{
    // TODO
}

void wxComboBox::Remove(long from, long to)
{
    // TODO
}

void wxComboBox::SetSelection(long from, long to)
{
    // TODO
}

void wxComboBox::Append(const wxString& item)
{
    // TODO
}

void wxComboBox::Delete(int n)
{
    // TODO
}

void wxComboBox::Clear()
{
    // TODO
}

int wxComboBox::GetSelection() const
{
    // TODO
    return -1;
}

void wxComboBox::SetSelection(int n)
{
    // TODO
}

int wxComboBox::FindString(const wxString& s) const
{
    // TODO
    return -1;
}

wxString wxComboBox::GetString(int n) const
{
    // TODO
    return wxString("");
}

wxString wxComboBox::GetStringSelection() const
{
    // TODO
    return wxString("");
}

bool wxComboBox::SetStringSelection(const wxString& sel)
{
    // TODO
    return FALSE;
}
