/////////////////////////////////////////////////////////////////////////////
// Name:        src/univ/control.cpp
// Purpose:     universal wxControl: adds handling of mnemonics
// Author:      Vadim Zeitlin
// Modified by:
// Created:     14.08.00
// RCS-ID:      $Id$
// Copyright:   (c) 2000 Vadim Zeitlin <vadim@wxwindows.org>
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

#ifdef __GNUG__
    #pragma implementation "control.h"
#endif

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_CONTROLS

#ifndef WX_PRECOMP
    #include "wx/control.h"
#endif

#include "wx/univ/renderer.h"
#include "wx/univ/theme.h"

// ============================================================================
// implementation
// ============================================================================

IMPLEMENT_DYNAMIC_CLASS(wxControl, wxWindow)

BEGIN_EVENT_TABLE(wxControl, wxControlBase)
    EVT_PAINT(wxControl::OnPaint)
END_EVENT_TABLE()

// ----------------------------------------------------------------------------
// creation
// ----------------------------------------------------------------------------

wxControl::wxControl()
{
    m_indexAccel = -1;
}

// ----------------------------------------------------------------------------
// state flags
// ----------------------------------------------------------------------------

bool wxControl::IsFocused() const
{
    wxControl *self = wxConstCast(this, wxControl);
    return self->FindFocus() == self;
}

bool wxControl::IsPressed() const
{
    return FALSE;
}

bool wxControl::IsDefault() const
{
    return FALSE;
}

// ----------------------------------------------------------------------------
// mnemonics handling
// ----------------------------------------------------------------------------

void wxControl::SetLabel(const wxString& label)
{
    // the character following MNEMONIC_PREFIX is the accelerator for this
    // control unless it is MNEMONIC_PREFIX too - this allows to insert
    // literal MNEMONIC_PREFIX chars into the label
    static const wxChar MNEMONIC_PREFIX = _T('&');

    wxString labelOld = m_label;
    m_indexAccel = -1;
    m_label.clear();
    for ( const wxChar *pc = label; *pc != wxT('\0'); pc++ )
    {
        if ( *pc == MNEMONIC_PREFIX )
        {
            pc++; // skip it
            if ( *pc != MNEMONIC_PREFIX )
            {
                if ( m_indexAccel == -1 )
                {
                    // remember it (-1 is for MNEMONIC_PREFIX itself
                    m_indexAccel = pc - label.c_str() - 1;
                }
                else
                {
                    wxFAIL_MSG(_T("duplicate accel char in control label"));
                }
            }
        }

        m_label += *pc;
    }

    if ( m_label != labelOld )
    {
        Refresh();
    }
}

wxString wxControl::GetLabel() const
{
    return m_label;
}

// ----------------------------------------------------------------------------
// painting
// ----------------------------------------------------------------------------

// the event handler executed when the window must be repainted
void wxControl::OnPaint(wxPaintEvent& event)
{
    // get the DC to use and create renderer on it
    wxPaintDC dc(this);
    wxControlRenderer renderer(this, dc, wxTheme::Get()->GetRenderer());

    // do draw the control!
    DoDraw(&renderer);
}

void wxControl::DoDraw(wxControlRenderer *renderer)
{
    renderer->DrawBorder();
}

#endif // wxUSE_CONTROLS
