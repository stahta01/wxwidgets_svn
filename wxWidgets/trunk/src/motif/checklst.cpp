///////////////////////////////////////////////////////////////////////////////
// Name:        checklst.cpp
// Purpose:     implementation of wxCheckListBox class
// Author:      Julian Smart
// Modified by:
// Created:     17/09/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// ============================================================================
// headers & declarations
// ============================================================================

#ifdef __GNUG__
#pragma implementation "checklst.h"
#endif

#include "wx/defs.h"

#include "wx/checklst.h"

// ============================================================================
// implementation
// ============================================================================

IMPLEMENT_DYNAMIC_CLASS(wxCheckListBox, wxListBox)

// ----------------------------------------------------------------------------
// implementation of wxCheckListBox class
// ----------------------------------------------------------------------------

// define event table
// ------------------
BEGIN_EVENT_TABLE(wxCheckListBox, wxListBox)
END_EVENT_TABLE()

// control creation
// ----------------

// def ctor: use Create() to really create the control
wxCheckListBox::wxCheckListBox() : wxListBox()
{
}

// ctor which creates the associated control
wxCheckListBox::wxCheckListBox(wxWindow *parent, wxWindowID id,
                               const wxPoint& pos, const wxSize& size,
                               int nStrings, const wxString choices[],
                               long style, const wxValidator& val,
                               const wxString& name)
                               : wxListBox()
{
    // TODO: you'll probably need a separate Create instead of using
    // the wxListBox one as here.
    Create(parent, id, pos, size, nStrings, choices, style|wxLB_OWNERDRAW, val, name);
}

// check items
// -----------

bool wxCheckListBox::IsChecked(size_t WXUNUSED(uiIndex)) const
{
    // TODO
    return FALSE;
}

void wxCheckListBox::Check(size_t WXUNUSED(uiIndex), bool WXUNUSED(bCheck))
{
    // TODO
}


