/////////////////////////////////////////////////////////////////////////////
// Name:        colordlg.cpp
// Purpose:     wxColourDialog class
// Author:      AUTHOR
// Modified by:
// Created:     ??/??/98
// RCS-ID:      $Id$
// Copyright:   (c) AUTHOR
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "colordlg.h"
#endif

#include "wx/colordlg.h"

#if !USE_SHARED_LIBRARY
IMPLEMENT_DYNAMIC_CLASS(wxColourDialog, wxDialog)
#endif

/*
 * wxColourDialog
 */

wxColourDialog::wxColourDialog()
{
    m_dialogParent = NULL;
}

wxColourDialog::wxColourDialog(wxWindow *parent, wxColourData *data)
{
    Create(parent, data);
}

bool wxColourDialog::Create(wxWindow *parent, wxColourData *data)
{
    m_dialogParent = parent;
  
    if (data)
        m_colourData = *data;
    return TRUE;
}

int wxColourDialog::ShowModal()
{
    /* TODO: implement dialog
     */

    return wxID_CANCEL;
}

