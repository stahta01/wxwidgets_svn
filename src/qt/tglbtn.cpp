/////////////////////////////////////////////////////////////////////////////
// Name:        src/qt/tglbtn.cpp
// Author:      Peter Most
// Id:          $Id$
// Copyright:   (c) Peter Most
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/tglbtn.h"
#include "wx/bitmap.h"
#include "wx/qt/converter.h"

wxDEFINE_EVENT( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEvent );

IMPLEMENT_DYNAMIC_CLASS( wxBitmapToggleButton, wxToggleButtonBase )

wxBitmapToggleButton::wxBitmapToggleButton()
{
}

wxBitmapToggleButton::wxBitmapToggleButton(wxWindow *parent,
               wxWindowID id,
               const wxBitmap& label,
               const wxPoint& pos,
               const wxSize& size,
               long style,
               const wxValidator& validator,
               const wxString& name)
{
    Create( parent, id, label, pos, size, style, validator, name );
}

bool wxBitmapToggleButton::Create(wxWindow *parent,
            wxWindowID id,
            const wxBitmap& label,
            const wxPoint& pos,
            const wxSize& size, long style,
            const wxValidator& validator,
            const wxString& name)
{
    m_qtPushButton = new QPushButton( parent->GetHandle() );
    m_qtPushButton->setIcon( QIcon( *label.GetHandle() ));

    return wxToggleButtonBase::Create( parent, id, pos, size, style, validator, name );
}

void wxBitmapToggleButton::SetValue(bool state)
{
}

bool wxBitmapToggleButton::GetValue() const
{
    return false;
}

QPushButton *wxBitmapToggleButton::GetHandle() const
{
    return m_qtPushButton;
}

//##############################################################################

wxToggleButton::wxToggleButton()
{
}

wxToggleButton::wxToggleButton(wxWindow *parent,
               wxWindowID id,
               const wxString& label,
               const wxPoint& pos,
               const wxSize& size,
               long style,
               const wxValidator& validator,
               const wxString& name)
{
    Create( parent, id, label, pos, size, style, validator, name );
}

bool wxToggleButton::Create(wxWindow *parent,
            wxWindowID id,
            const wxString& label,
            const wxPoint& pos,
            const wxSize& size, long style,
            const wxValidator& validator,
            const wxString& name)
{
    m_qtPushButton = new QPushButton( wxQtConvertString( label ), parent->GetHandle() );

    return wxToggleButtonBase::Create( parent, id, pos, size, style, validator, name );
}

void wxToggleButton::SetValue(bool state)
{
}

bool wxToggleButton::GetValue() const
{
    return false;
}

QPushButton *wxToggleButton::GetHandle() const
{
    return m_qtPushButton;
}
