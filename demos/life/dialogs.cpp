/////////////////////////////////////////////////////////////////////////////
// Name:        dialogs.cpp
// Purpose:     Life! dialogs
// Author:      Guillermo Rodriguez Garcia, <guille@iies.es>
// Modified by:
// Created:     Jan/2000
// RCS-ID:      $Id$
// Copyright:   (c) 2000, Guillermo Rodriguez Garcia
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ==========================================================================
// headers, declarations, constants
// ==========================================================================

#ifdef __GNUG__
    #pragma implementation "dialogs.h"
#endif

#include "wx/statline.h"
#include "wx/spinctrl.h"

#include "dialogs.h"
#include "life.h"
#include "game.h"

// --------------------------------------------------------------------------
// resources
// --------------------------------------------------------------------------

#if defined(__WXGTK__) || defined(__WXMOTIF__)
    // logo for the about dialog
    #include "bitmaps/life.xpm"
#endif

// sample configurations
#include "samples.inc"

// --------------------------------------------------------------------------
// constants
// --------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
    // bmp window in about dialog
    ID_BMPWIN = 2001,

    // listbox in samples dialog
    ID_LISTBOX
};

// --------------------------------------------------------------------------
// event tables and other macros for wxWindows
// --------------------------------------------------------------------------

// Event tables
BEGIN_EVENT_TABLE(LifeSamplesDialog, wxDialog)
    EVT_LISTBOX (ID_LISTBOX, LifeSamplesDialog::OnListBox)
    
END_EVENT_TABLE()


// ==========================================================================
// implementation
// ==========================================================================

// --------------------------------------------------------------------------
// LifeSamplesDialog
// --------------------------------------------------------------------------

LifeSamplesDialog::LifeSamplesDialog(wxWindow *parent)
                 : wxDialog(parent, -1,
                            _("Sample games"),
                            wxDefaultPosition,
                            wxDefaultSize,
                            wxDEFAULT_DIALOG_STYLE | wxDIALOG_MODAL)
{
    m_value = 0;
   
    // create and populate the list of available samples
    m_list = new wxListBox( this, ID_LISTBOX,
        wxDefaultPosition,
        wxDefaultSize,
        0, NULL,
        wxLB_SINGLE | wxLB_NEEDED_SB | wxLB_HSCROLL );

    for (unsigned i = 0; i < (sizeof(g_shapes) / sizeof(LifeShape)); i++)
        m_list->Append(g_shapes[i].m_name);

    // descriptions
    wxStaticBox *statbox = new wxStaticBox( this, -1, _("Description"));
    m_life   = new Life();
    m_life->SetShape(g_shapes[0]);
    m_canvas = new LifeCanvas( this, m_life, FALSE );
    m_text   = new wxTextCtrl( this, -1,
        g_shapes[0].m_desc,
        wxDefaultPosition,
        wxSize(300, 60),
        wxTE_MULTILINE | wxTE_READONLY);

    // layout components
    wxStaticBoxSizer *sizer1 = new wxStaticBoxSizer( statbox, wxVERTICAL );
    sizer1->Add( m_canvas, 2, wxGROW | wxCENTRE | wxALL, 5);
    sizer1->Add( m_text, 1, wxGROW | wxCENTRE | wxALL, 5 );

    wxBoxSizer *sizer2 = new wxBoxSizer( wxHORIZONTAL );
    sizer2->Add( m_list, 0, wxGROW | wxCENTRE | wxALL, 5 );
    sizer2->Add( sizer1, 1, wxGROW | wxCENTRE | wxALL, 5 );

    wxBoxSizer *sizer3 = new wxBoxSizer( wxVERTICAL );
    sizer3->Add( CreateTextSizer(_("Select one configuration")), 0, wxALL, 10 );
    sizer3->Add( new wxStaticLine(this, -1), 0, wxGROW | wxLEFT | wxRIGHT, 10 );
    sizer3->Add( sizer2, 1, wxGROW | wxCENTRE | wxALL, 5 );
    sizer3->Add( new wxStaticLine(this, -1), 0, wxGROW | wxLEFT | wxRIGHT, 10 );
    sizer3->Add( CreateButtonSizer(wxOK | wxCANCEL), 0, wxCENTRE | wxALL, 10 );

    // activate
    SetSizer(sizer3);
    SetAutoLayout(TRUE);
    sizer3->SetSizeHints(this);
    sizer3->Fit(this);
    Centre(wxBOTH | wxCENTRE_ON_SCREEN);
}

LifeSamplesDialog::~LifeSamplesDialog()
{
    m_canvas->Destroy();
}

const LifeShape& LifeSamplesDialog::GetShape()
{
    return g_shapes[m_value];
}

void LifeSamplesDialog::OnListBox(wxCommandEvent& event)
{
    int sel = event.GetSelection();

    if (sel != -1)
    {
        m_value = m_list->GetSelection();
        m_text->SetValue(g_shapes[ sel ].m_desc);
        m_life->SetShape(g_shapes[ sel ]);

        // quick and dirty :-)
        if ((g_shapes[ sel ].m_width > 36) ||
            (g_shapes[ sel ].m_height > 22))
            m_canvas->SetCellSize(2);
        else
            m_canvas->SetCellSize(8);
    }
}

// --------------------------------------------------------------------------
// LifeAboutDialog
// --------------------------------------------------------------------------

LifeAboutDialog::LifeAboutDialog(wxWindow *parent)
               : wxDialog(parent, -1,
                          _("About Life!"),
                          wxDefaultPosition,
                          wxDefaultSize,
                          wxDEFAULT_DIALOG_STYLE | wxDIALOG_MODAL)
{
    // logo
    wxBitmap bmp = wxBITMAP(life);
#if !defined(__WXGTK__) && !defined(__WXMOTIF__)
    bmp.SetMask(new wxMask(bmp, *wxBLUE));
#endif
    wxStaticBitmap *sbmp = new wxStaticBitmap(this, -1, bmp);

    // layout components
    wxBoxSizer *sizer = new wxBoxSizer( wxVERTICAL );
    sizer->Add( sbmp, 0, wxCENTRE | wxALL, 10 );
    sizer->Add( new wxStaticLine(this, -1), 0, wxGROW | wxLEFT | wxRIGHT, 5 );
    sizer->Add( CreateTextSizer(_("Life! for wxWindows, version 2.0\n\n"
                                  "(c) 2000 Guillermo Rodriguez Garcia\n\n"
                                  "<guille@iies.es>\n\n"
                                  "Portions of the code are based in XLife\n"
                                  "XLife is (c) 1989 by Jon Bennett et al.")),
                                  0, wxCENTRE | wxALL, 20 );
    sizer->Add( new wxStaticLine(this, -1), 0, wxGROW | wxLEFT | wxRIGHT, 5 );
    sizer->Add( CreateButtonSizer(wxOK), 0, wxCENTRE | wxALL, 10 );

    // activate
    SetSizer(sizer);
    SetAutoLayout(TRUE);
    sizer->SetSizeHints(this);
    sizer->Fit(this);
    Centre(wxBOTH | wxCENTRE_ON_SCREEN);
}
