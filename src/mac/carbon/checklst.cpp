///////////////////////////////////////////////////////////////////////////////
// Name:        checklst.cpp
// Purpose:     implementation of wxCheckListBox class
// Author:      AUTHOR
// Modified by: 
// Created:     ??/??/98
// RCS-ID:      $Id$
// Copyright:   (c) AUTHOR
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// ============================================================================
// headers & declarations
// ============================================================================

#ifdef __GNUG__
#pragma implementation "checklst.h"
#endif

#include "wx/defs.h"

#if wxUSE_CHECKLISTBOX

#include "wx/checklst.h"

// ============================================================================
// implementation
// ============================================================================

#if !USE_SHARED_LIBRARY
  IMPLEMENT_DYNAMIC_CLASS(wxCheckListBox, wxListBox)
#endif

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
wxCheckListBox::wxCheckListBox() : wxCheckListBoxBase()
{
}

// ctor which creates the associated control
wxCheckListBox::wxCheckListBox(wxWindow *parent, wxWindowID id,
                               const wxPoint& pos, const wxSize& size,
                               int nStrings, const wxString choices[],
                               long style, const wxValidator& val,
                               const wxString& name)
              : wxCheckListBoxBase()
{
    // TODO: you'll probably need a separate Create instead of using
    // the wxListBox one as here.
    Create(parent, id, pos, size, nStrings, choices, style|wxLB_OWNERDRAW, val, name);
}

// check items
// -----------

bool wxCheckListBox::IsChecked(size_t uiIndex) const
{
    // TODO
    return FALSE;
}

void wxCheckListBox::Check(size_t uiIndex, bool bCheck)
{
    // TODO
}

#endif // wxUSE_CHECKLISTBOX

