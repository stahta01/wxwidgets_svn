/////////////////////////////////////////////////////////////////////////////
// Name:        propdlg.cpp
// Purpose:     wxPropertySheetDialog
// Author:      Julian Smart
// Modified by:
// Created:     2005-03-12
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "propdlg.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include "wx/defs.h"

#ifndef WX_PRECOMP
    #include "wx/button.h"
    #include "wx/sizer.h"
    #include "wx/intl.h"
    #include "wx/log.h"
    #include "wx/msgdlg.h"
#endif

#include "wx/notebook.h"
#include "wx/generic/propdlg.h"

//-----------------------------------------------------------------------------
// wxPropertySheetDialog
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxPropertySheetDialog, wxDialog)

bool wxPropertySheetDialog::Create(wxWindow* parent, wxWindowID id, const wxString& title, 
                                       const wxPoint& pos, const wxSize& sz, long style,
                                       const wxString& name)
{
    if (!wxDialog::Create(parent, id, title, pos, sz, style, name))
        return false;
    
    wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
    SetSizer(topSizer);

    // This gives more space around the edges
    m_innerSizer = new wxBoxSizer( wxVERTICAL );

    int extraSpace = 5;
#ifdef __WXWINCE__
    extraSpace=0;
#endif
    topSizer->Add(m_innerSizer, 1, wxGROW|wxALL, extraSpace);

    m_bookCtrl = CreateBookCtrl();
    AddBookCtrl(m_innerSizer);

    return true;
}

void wxPropertySheetDialog::Init()
{
    m_innerSizer = NULL;
    m_bookCtrl = NULL;
}

// Layout the dialog, to be called after pages have been created
void wxPropertySheetDialog::LayoutDialog()
{
#ifndef __WXWINCE__
    GetSizer()->Fit(this);
    Centre(wxBOTH);
#endif
}

// Creates the buttons, if any
void wxPropertySheetDialog::CreateButtons(int flags)
{
#ifndef __WXWINCE__
    wxSizer* sizer = CreateButtonSizer(flags);
    m_innerSizer->Add( sizer, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM, 5);
#endif
}

// Creates the book control
wxBookCtrlBase* wxPropertySheetDialog::CreateBookCtrl()
{
    int style = 0;
#ifdef __WXWINCE__
    style |= wxNB_BOTTOM|wxNB_FLAT;
#endif
    return new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, style );
}

// Adds the book control to the inner sizer.
void wxPropertySheetDialog::AddBookCtrl(wxSizer* sizer)
{
#ifdef __WXWINCE__
    // The book control has to be sized larger than the dialog because of a border bug
    // in WinCE
    sizer->Add( m_bookCtrl, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxRIGHT, -3 );
#else
    sizer->Add( m_bookCtrl, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
#endif
}

