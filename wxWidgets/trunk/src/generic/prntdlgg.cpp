/////////////////////////////////////////////////////////////////////////////
// Name:        prntdlgg.cpp
// Purpose:     Generic print dialogs
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#ifdef __GNUG__
    #pragma implementation "prntdlgg.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/defs.h"

#if wxUSE_PRINTING_ARCHITECTURE

#ifndef WX_PRECOMP
    #include "wx/utils.h"
    #include "wx/dc.h"
    #include "wx/app.h"
    #include "wx/frame.h"
    #include "wx/stattext.h"
    #include "wx/statbox.h"
    #include "wx/button.h"
    #include "wx/checkbox.h"
    #include "wx/textctrl.h"
    #include "wx/radiobox.h"
    #include "wx/filedlg.h"
    #include "wx/choice.h"
    #include "wx/combobox.h"
    #include <wx/intl.h>
    #include "wx/sizer.h"
#endif

#if wxUSE_STATLINE
  #include "wx/statline.h"
#endif

#include "wx/generic/prntdlgg.h"

#if wxUSE_POSTSCRIPT
    #include "wx/generic/dcpsg.h"
#endif

#include "wx/printdlg.h"
#include "wx/paper.h"

// For print paper things
#include "wx/prntbase.h"

#include <stdlib.h>
#include <string.h>

// ----------------------------------------------------------------------------
// wxWin macros
// ----------------------------------------------------------------------------

#if !USE_SHARED_LIBRARY

#if wxUSE_POSTSCRIPT

    IMPLEMENT_CLASS(wxGenericPrintDialog, wxDialog)
    IMPLEMENT_CLASS(wxGenericPrintSetupDialog, wxDialog)

    BEGIN_EVENT_TABLE(wxGenericPrintDialog, wxDialog)
        EVT_BUTTON(wxID_OK, wxGenericPrintDialog::OnOK)
        EVT_BUTTON(wxPRINTID_SETUP, wxGenericPrintDialog::OnSetup)
        EVT_RADIOBOX(wxPRINTID_RANGE, wxGenericPrintDialog::OnRange)
    END_EVENT_TABLE()
#endif

    IMPLEMENT_CLASS(wxGenericPageSetupDialog, wxDialog)

    BEGIN_EVENT_TABLE(wxGenericPageSetupDialog, wxDialog)
        EVT_BUTTON(wxPRINTID_SETUP, wxGenericPageSetupDialog::OnPrinter)
    END_EVENT_TABLE()
#endif // USE_SHARED_LIBRARY

// ----------------------------------------------------------------------------
// global vars
// ----------------------------------------------------------------------------

extern wxPrintPaperDatabase *wxThePrintPaperDatabase;

#if wxUSE_POSTSCRIPT

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// Generic print dialog for non-Windows printing use.
// ----------------------------------------------------------------------------

wxGenericPrintDialog::wxGenericPrintDialog(wxWindow *parent,
                                           wxPrintDialogData* data)
                    : wxDialog(parent, -1, _("Print"),
                               wxPoint(0, 0), wxSize(600, 600),
                               wxDEFAULT_DIALOG_STYLE |
                               wxDIALOG_MODAL |
                               wxTAB_TRAVERSAL)
{
    if ( data )
        m_printDialogData = *data;

    Init(parent);
}

wxGenericPrintDialog::wxGenericPrintDialog(wxWindow *parent,
                                           wxPrintData* data)
                    : wxDialog(parent, -1, _("Print"),
                               wxPoint(0, 0), wxSize(600, 600),
                               wxDEFAULT_DIALOG_STYLE |
                               wxDIALOG_MODAL |
                               wxTAB_TRAVERSAL)
{
    if ( data )
        m_printDialogData = *data;

    Init(parent);
}

void wxGenericPrintDialog::Init(wxWindow * WXUNUSED(parent))
{
  //    wxDialog::Create(parent, -1, _("Print"), wxPoint(0, 0), wxSize(600, 600),
  //                     wxDEFAULT_DIALOG_STYLE | wxDIALOG_MODAL | wxTAB_TRAVERSAL);

    wxBoxSizer *mainsizer = new wxBoxSizer( wxVERTICAL );

    // 1) top row 
        
    wxStaticBoxSizer *topsizer = new wxStaticBoxSizer( 
        new wxStaticBox( this, -1, _( "Printer options" ) ), wxHORIZONTAL );
    m_printToFileCheckBox = new wxCheckBox( this, wxPRINTID_PRINTTOFILE, _("Print to File") );
    topsizer->Add( m_printToFileCheckBox, 0, wxCENTER|wxALL, 5 );

    topsizer->Add( 60,2,1 );

    m_setupButton = new wxButton(this, wxPRINTID_SETUP, _("Setup...") );
    topsizer->Add( m_setupButton, 0, wxCENTER|wxALL, 5 );
    
    mainsizer->Add( topsizer, 0, wxLEFT|wxTOP|wxRIGHT, 10 );
    
    // 2) middle row with radio box
    
    wxString *choices = new wxString[2];
    choices[0] = _("All");
    choices[1] = _("Pages");

    m_fromText = (wxTextCtrl*)NULL;
    m_toText = (wxTextCtrl*)NULL;
    m_rangeRadioBox = (wxRadioBox *)NULL;
    
    if (m_printDialogData.GetFromPage() != 0)
    {
        m_rangeRadioBox = new wxRadioBox(this, wxPRINTID_RANGE, _("Print Range"),
                                         wxDefaultPosition, wxDefaultSize,
                                         2, choices,
                                         1, wxRA_VERTICAL);
        m_rangeRadioBox->SetSelection(1);
	
	mainsizer->Add( m_rangeRadioBox, 0, wxLEFT|wxTOP|wxRIGHT, 10 );
    }

    // 3) bottom row
    
    wxBoxSizer *bottomsizer = new wxBoxSizer( wxHORIZONTAL );

    if (m_printDialogData.GetFromPage() != 0)
    {
        bottomsizer->Add( new wxStaticText(this, wxPRINTID_STATIC, _("From:") ), 0, wxCENTER|wxALL, 5 );
        m_fromText = new wxTextCtrl(this, wxPRINTID_FROM, "", wxDefaultPosition, wxSize(40, -1));
	bottomsizer->Add( m_fromText, 1, wxCENTER|wxRIGHT, 10 );

        bottomsizer->Add( new wxStaticText(this, wxPRINTID_STATIC, _("To:") ), 0, wxCENTER|wxALL, 5);
        m_toText = new wxTextCtrl(this, wxPRINTID_TO, "", wxDefaultPosition, wxSize(40, -1));
	bottomsizer->Add( m_toText, 1, wxCENTER|wxRIGHT, 10 );
    }

    bottomsizer->Add( new wxStaticText(this, wxPRINTID_STATIC, _("Copies:") ), 0, wxCENTER|wxALL, 5 );
    m_noCopiesText = new wxTextCtrl(this, wxPRINTID_COPIES, "", wxPoint(252, 130), wxSize(40, -1));
    bottomsizer->Add( m_noCopiesText, 1, wxCENTER|wxRIGHT, 10 );

    mainsizer->Add( bottomsizer, 0, wxTOP|wxLEFT|wxRIGHT, 12 );

#if wxUSE_STATLINE
    // 4) static line
    mainsizer->Add( new wxStaticLine( this, -1 ), 0, wxEXPAND | wxLEFT|wxRIGHT|wxTOP, 10 );
#endif

    // 5) buttons

    mainsizer->Add( CreateButtonSizer( wxOK|wxCANCEL), 0, wxCENTER|wxALL, 10 );

    SetAutoLayout( TRUE );
    SetSizer( mainsizer );

    mainsizer->Fit( this );
    Centre(wxBOTH);

    // Calls wxWindow::OnInitDialog and then wxGenericPrintDialog::TransferDataToWindow
    InitDialog();
    delete[] choices;
}

int wxGenericPrintDialog::ShowModal()
{
    if ( m_printDialogData.GetSetupDialog() )
    {
        // Make sure wxPrintData object reflects the settings now, in case the setup dialog
        // changes it. In fact there aren't any common settings at
        // present, but there might be in future.
        // TransferDataFromWindow();

        wxGenericPrintSetupDialog *genericPrintSetupDialog =
            new wxGenericPrintSetupDialog(this, & m_printDialogData.GetPrintData());
        int ret = genericPrintSetupDialog->ShowModal();
        if ( ret != wxID_CANCEL )
        {
            // Transfer settings to the global object (for compatibility) and to
            // the print dialog's print data.
            *wxThePrintSetupData = genericPrintSetupDialog->GetPrintData();
            m_printDialogData.GetPrintData() = genericPrintSetupDialog->GetPrintData();
        }
        genericPrintSetupDialog->Destroy();

        // Restore the wxPrintData settings again (uncomment if any settings become common
        // to both dialogs)
        // TransferDataToWindow();

        return ret;
    }
    else
    {
        return wxDialog::ShowModal();
    }
}

wxGenericPrintDialog::~wxGenericPrintDialog()
{
}

void wxGenericPrintDialog::OnOK(wxCommandEvent& WXUNUSED(event))
{
    TransferDataFromWindow();

    // There are some interactions between the global setup data
    // and the standard print dialog. The global printing 'mode'
    // is determined by whether the user checks Print to file
    // or not.
    if (m_printDialogData.GetPrintToFile())
    {
        m_printDialogData.GetPrintData().SetPrintMode(wxPRINT_MODE_FILE);
        wxThePrintSetupData->SetPrinterMode(wxPRINT_MODE_FILE);

        wxString f = wxFileSelector(_("PostScript file"),
            wxPathOnly(wxThePrintSetupData->GetPrinterFile()),
            wxFileNameFromPath(wxThePrintSetupData->GetPrinterFile()),
            _T("ps"), _T("*.ps"), 0, this);
        if ( f.IsEmpty() )
            return;

        m_printDialogData.GetPrintData().SetFilename(f);
        wxThePrintSetupData->SetPrinterFile(f);
    }
    else
    {
        m_printDialogData.GetPrintData().SetPrintMode(wxPRINT_MODE_PRINTER);
        wxThePrintSetupData->SetPrinterMode(wxPRINT_MODE_PRINTER);
    }

    EndModal(wxID_OK);
}

void wxGenericPrintDialog::OnRange(wxCommandEvent& event)
{
    if (!m_fromText) return;

    if (event.GetInt() == 0)
    {
        m_fromText->Enable(FALSE);
        m_toText->Enable(FALSE);
    }
    else if (event.GetInt() == 1)
    {
        m_fromText->Enable(TRUE);
        m_toText->Enable(TRUE);
    }
}

void wxGenericPrintDialog::OnSetup(wxCommandEvent& WXUNUSED(event))
{
   *wxThePrintSetupData = m_printDialogData.GetPrintData();
    wxGenericPrintSetupDialog *genericPrintSetupDialog =
        new wxGenericPrintSetupDialog(this, wxThePrintSetupData);
    int ret = genericPrintSetupDialog->ShowModal();
    if ( ret != wxID_CANCEL )
    {
        *wxThePrintSetupData = genericPrintSetupDialog->GetPrintData();
        m_printDialogData = genericPrintSetupDialog->GetPrintData();
    }

    genericPrintSetupDialog->Close(TRUE);
}

bool wxGenericPrintDialog::TransferDataToWindow()
{
    char buf[10];

    if(m_printDialogData.GetFromPage() != 0)
    {
       if(m_fromText)
       {
          if (m_printDialogData.GetEnablePageNumbers())
          {
             m_fromText->Enable(TRUE);
             m_toText->Enable(TRUE);
             sprintf(buf, "%d", m_printDialogData.GetFromPage());
             m_fromText->SetValue(buf);
             sprintf(buf, "%d", m_printDialogData.GetToPage());
             m_toText->SetValue(buf);
             if(m_rangeRadioBox)
                if (m_printDialogData.GetAllPages())
                   m_rangeRadioBox->SetSelection(0);
                else
                   m_rangeRadioBox->SetSelection(1);
          }
          else
          {
             m_fromText->Enable(FALSE);
             m_toText->Enable(FALSE);
             if(m_rangeRadioBox)
             {
                m_rangeRadioBox->SetSelection(0);
                m_rangeRadioBox->wxRadioBox::Enable(1, FALSE);
             }
          }
       }
    }
    sprintf(buf, "%d", m_printDialogData.GetNoCopies());
    m_noCopiesText->SetValue(buf);

    m_printToFileCheckBox->SetValue(m_printDialogData.GetPrintToFile());
    m_printToFileCheckBox->Enable(m_printDialogData.GetEnablePrintToFile());
    return TRUE;
}

bool wxGenericPrintDialog::TransferDataFromWindow()
{
    if(m_printDialogData.GetFromPage() != -1)
    {
        if (m_printDialogData.GetEnablePageNumbers())
        {
           if(m_fromText) m_printDialogData.SetFromPage(wxAtoi(m_fromText->GetValue()));
           if(m_toText)   m_printDialogData.SetToPage(wxAtoi(m_toText->GetValue()));
        }
        if(m_rangeRadioBox)
        {
           if (m_rangeRadioBox->GetSelection() == 0)
              m_printDialogData.SetAllPages(TRUE);
           else
              m_printDialogData.SetAllPages(FALSE);
        }
    }
    else
    { // continuous printing
        m_printDialogData.SetFromPage(1);
        m_printDialogData.SetToPage(32000);
    }
    m_printDialogData.SetNoCopies(wxAtoi(m_noCopiesText->GetValue()));
    m_printDialogData.SetPrintToFile(m_printToFileCheckBox->GetValue());

    return TRUE;
}

/*
TODO: collate and noCopies should be duplicated across dialog data and print data objects
(slightly different semantics on Windows but let's ignore this for a bit).
*/

wxDC *wxGenericPrintDialog::GetPrintDC()
{
  //    return new wxPostScriptDC(wxThePrintSetupData->GetPrinterFile(), FALSE, (wxWindow *) NULL);
  return new wxPostScriptDC(GetPrintDialogData().GetPrintData());
}

// ----------------------------------------------------------------------------
// Generic print setup dialog
// ----------------------------------------------------------------------------

wxGenericPrintSetupDialog::wxGenericPrintSetupDialog(wxWindow *parent, wxPrintData* data):
wxDialog(parent, -1, _("Print Setup"), wxPoint(0, 0), wxSize(600, 600), wxDEFAULT_DIALOG_STYLE|wxDIALOG_MODAL|wxTAB_TRAVERSAL)
{
    Init(data);
}

// Convert wxPrintSetupData to standard wxPrintData object
wxGenericPrintSetupDialog::wxGenericPrintSetupDialog(wxWindow *parent, wxPrintSetupData* data):
wxDialog(parent, -1, _("Print Setup"), wxPoint(0, 0), wxSize(600, 600), wxDEFAULT_DIALOG_STYLE|wxDIALOG_MODAL|wxTAB_TRAVERSAL)
{
    wxPrintData printData;
    if (data)
        printData = * data;
    else
        printData = * wxThePrintSetupData;

    Init(& printData);
}

void wxGenericPrintSetupDialog::Init(wxPrintData* data)
{
    if ( data )
        m_printData = *data;

    int staticBoxWidth = 300;

    (void) new wxStaticBox(this, wxPRINTID_STATIC, _("Paper size"), wxPoint(10, 10), wxSize(staticBoxWidth, 60) );

    int xPos = 20;
    int yPos = 30;
    m_paperTypeChoice = CreatePaperTypeChoice(&xPos, &yPos);

    wxString *choices =  new wxString[2];
    choices[0] = _("Portrait");
    choices[1] = _("Landscape");

    m_orientationRadioBox = new wxRadioBox(this, wxPRINTID_ORIENTATION, _("Orientation"),
        wxPoint(10, 80), wxSize(-1, -1), 2, choices, 1, wxRA_VERTICAL );
    m_orientationRadioBox->SetSelection(0);

    (void) new wxStaticBox(this, wxPRINTID_STATIC, _("Options"), wxPoint(10, 130), wxSize(staticBoxWidth, 50) );

    int colourYPos = 145;

#ifdef __WXMOTIF__
    colourYPos = 150;
#endif

    m_colourCheckBox = new wxCheckBox(this, wxPRINTID_PRINTCOLOUR, _("Print in colour"), wxPoint(15, colourYPos));

    (void) new wxStaticBox(this, wxPRINTID_STATIC, _("Print spooling"), wxPoint(330, 10), wxSize(200,170) );

    (void) new wxStaticText(this, wxPRINTID_STATIC, _("Printer command:"), wxPoint(340, 30));

    m_printerCommandText = new wxTextCtrl(this, wxPRINTID_COMMAND, "", wxPoint(360, 55), wxSize(150, -1));

    (void) new wxStaticText(this, wxPRINTID_STATIC, _("Printer options:"), wxPoint(340, 110));

    m_printerOptionsText = new wxTextCtrl(this, wxPRINTID_OPTIONS, "", wxPoint(360, 135), wxSize(150, -1));

    wxButton *okButton = new wxButton(this, wxID_OK, _("OK"), wxPoint(130, 200), wxSize(80, -1));
    (void) new wxButton(this, wxID_CANCEL, _("Cancel"), wxPoint(320, 200), wxSize(80, -1));

    okButton->SetDefault();
    okButton->SetFocus();

    Fit();
    Centre(wxBOTH);

    InitDialog();
    delete[] choices;
}

wxGenericPrintSetupDialog::~wxGenericPrintSetupDialog()
{
}

bool wxGenericPrintSetupDialog::TransferDataToWindow()
{
    if (m_printerCommandText && m_printData.GetPrinterCommand())
        m_printerCommandText->SetValue(m_printData.GetPrinterCommand());
    if (m_printerOptionsText && m_printData.GetPrinterOptions())
        m_printerOptionsText->SetValue(m_printData.GetPrinterOptions());
    if (m_colourCheckBox)
        m_colourCheckBox->SetValue(m_printData.GetColour());

    if (m_orientationRadioBox)
    {
        if (m_printData.GetOrientation() == wxPORTRAIT)
            m_orientationRadioBox->SetSelection(0);
        else
            m_orientationRadioBox->SetSelection(1);
    }
    return TRUE;
}

bool wxGenericPrintSetupDialog::TransferDataFromWindow()
{
    if (m_printerCommandText)
        m_printData.SetPrinterCommand(m_printerCommandText->GetValue());
    if (m_printerOptionsText)
        m_printData.SetPrinterOptions(m_printerOptionsText->GetValue());
    if (m_colourCheckBox)
        m_printData.SetColour(m_colourCheckBox->GetValue());
    if (m_orientationRadioBox)
    {
        int sel = m_orientationRadioBox->GetSelection();
        if (sel == 0)
            m_printData.SetOrientation(wxPORTRAIT);
        else
            m_printData.SetOrientation(wxLANDSCAPE);
    }
    if (m_paperTypeChoice)
    {
        wxString val(m_paperTypeChoice->GetStringSelection());
        if (!val.IsNull() && val != "")
            m_printData.SetPaperId(wxThePrintPaperDatabase->ConvertNameToId(val));
    }

    // This is for backward compatibility only
    *wxThePrintSetupData = GetPrintData();
    return TRUE;
}

wxComboBox *wxGenericPrintSetupDialog::CreatePaperTypeChoice(int *x, int *y)
{
/* Should not be necessary
    if (!wxThePrintPaperDatabase)
    {
        wxThePrintPaperDatabase = new wxPrintPaperDatabase;
        wxThePrintPaperDatabase->CreateDatabase();
    }
*/
    int n = wxThePrintPaperDatabase->Number();
    wxString *choices = new wxString [n];
    int sel = 0;
    int i;
    for (i = 0; i < n; i++)
    {
        wxPrintPaperType *paper = (wxPrintPaperType *)wxThePrintPaperDatabase->Nth(i)->Data();
        choices[i] = paper->GetName();
        if (m_printData.GetPaperId() == paper->GetId())
            sel = i;
    }

    int width = 250;

    wxComboBox *choice = new wxComboBox(this, wxPRINTID_PAPERSIZE,
                                        _("Paper Size"),
                                        wxPoint(*x, *y), wxSize(width, -1), n,
        choices);

    //    SetFont(thisFont);

    delete[] choices;

    choice->SetSelection(sel);
    return choice;
}
#endif // wxUSE_POSTSCRIPT

// ----------------------------------------------------------------------------
// Generic page setup dialog
// ----------------------------------------------------------------------------

void wxGenericPageSetupDialog::OnPrinter(wxCommandEvent& WXUNUSED(event))
{
    // We no longer query GetPrintMode, so we can eliminate the need
    // to call SetPrintMode.
    // This has the limitation that we can't explicitly call the PostScript
    // print setup dialog from the generic Page Setup dialog under Windows,
    // but since this choice would only happen when trying to do PostScript
    // printing under Windows (and only in 16-bit Windows which
    // doesn't have a Windows-specific page setup dialog) it's worth it.

    // First save the current settings, so the wxPrintData object is up to date.
    TransferDataFromWindow();

    // Transfer the current print settings from this dialog to the page setup dialog.
    wxPrintDialogData data;
    data = GetPageSetupData().GetPrintData();
    data.SetSetupDialog(TRUE);
    wxPrintDialog *printDialog = new wxPrintDialog(this, & data);
    printDialog->ShowModal();

    // Transfer the page setup print settings from the page dialog to this dialog again, in case
    // the page setup dialog changed something.
    GetPageSetupData().GetPrintData() = printDialog->GetPrintDialogData().GetPrintData();
    GetPageSetupData().CalculatePaperSizeFromId(); // Make sure page size reflects the id in wxPrintData

    printDialog->Destroy();

    // Now update the dialog in case the page setup dialog changed some of our settings.
    TransferDataToWindow();
}

wxGenericPageSetupDialog::wxGenericPageSetupDialog(wxWindow *parent, wxPageSetupData* data):
wxDialog(parent, -1, _("Page Setup"), wxPoint(0, 0), wxSize(600, 600), wxDIALOG_MODAL|wxDEFAULT_DIALOG_STYLE|wxTAB_TRAVERSAL)
{
    if (data)
        m_pageData = *data;
	
    int textWidth = 80;
	
    wxBoxSizer *mainsizer = new wxBoxSizer( wxVERTICAL );
    
    // 1) top
    wxStaticBoxSizer *topsizer = new wxStaticBoxSizer( 
      new wxStaticBox(this,wxPRINTID_STATIC, _("Paper size")), wxHORIZONTAL );
    
    int n = wxThePrintPaperDatabase->Number();
    wxString *choices = new wxString [n];
    int i;
    for (i = 0; i < n; i++)
    {
        wxPrintPaperType *paper = (wxPrintPaperType *)wxThePrintPaperDatabase->Nth(i)->Data();
        choices[i] = paper->GetName();
    }

    m_paperTypeChoice = new wxComboBox(this, wxPRINTID_PAPERSIZE, _("Paper Size"),
                                        wxDefaultPosition, wxSize(300, -1), n, choices);
    topsizer->Add( m_paperTypeChoice, 1, wxEXPAND|wxALL, 5 );
//  m_paperTypeChoice->SetSelection(sel);

    mainsizer->Add( topsizer, 0, wxTOP|wxLEFT|wxRIGHT | wxEXPAND, 10 );

    // 2) middle sizer with radio box

    wxString *choices2 = new wxString[2];
    choices2[0] = _("Portrait");
    choices2[1] = _("Landscape");
    m_orientationRadioBox = new wxRadioBox(this, wxPRINTID_ORIENTATION, _("Orientation"),
        wxDefaultPosition, wxDefaultSize, 2, choices2, 2);
    m_orientationRadioBox->SetSelection(0);

    mainsizer->Add( m_orientationRadioBox, 0, wxTOP|wxLEFT|wxRIGHT, 10 );

    // 3) margins

    wxBoxSizer *table = new wxBoxSizer( wxHORIZONTAL );

    wxBoxSizer *column1 = new wxBoxSizer( wxVERTICAL );
    column1->Add( new wxStaticText(this, wxPRINTID_STATIC, _("Left margin (mm):")),1,wxALL|wxALIGN_RIGHT,5 );
    column1->Add( new wxStaticText(this, wxPRINTID_STATIC, _("Top margin (mm):")),1,wxALL|wxALIGN_RIGHT,5 );
    table->Add( column1, 0, wxALL | wxEXPAND, 5 );
    
    wxBoxSizer *column2 = new wxBoxSizer( wxVERTICAL );
    m_marginLeftText = new wxTextCtrl(this, wxPRINTID_LEFTMARGIN, "", wxDefaultPosition, wxSize(textWidth, -1));
    m_marginTopText = new wxTextCtrl(this, wxPRINTID_TOPMARGIN, "", wxDefaultPosition, wxSize(textWidth, -1));
    column2->Add( m_marginLeftText, 1, wxALL, 5 );
    column2->Add( m_marginTopText, 1, wxALL, 5 );
    table->Add( column2, 0, wxRIGHT|wxTOP|wxBOTTOM | wxEXPAND, 5 );
    
    wxBoxSizer *column3 = new wxBoxSizer( wxVERTICAL );
    column3->Add( new wxStaticText(this, wxPRINTID_STATIC, _("Right margin (mm):")),1,wxALL|wxALIGN_RIGHT,5 );
    column3->Add( new wxStaticText(this, wxPRINTID_STATIC, _("Bottom margin (mm):")),1,wxALL|wxALIGN_RIGHT,5 );
    table->Add( column3, 0, wxALL | wxEXPAND, 5 );
    
    wxBoxSizer *column4 = new wxBoxSizer( wxVERTICAL );
    m_marginRightText = new wxTextCtrl(this, wxPRINTID_RIGHTMARGIN, "", wxDefaultPosition, wxSize(textWidth, -1));
    m_marginBottomText = new wxTextCtrl(this, wxPRINTID_BOTTOMMARGIN, "", wxDefaultPosition, wxSize(textWidth, -1));
    column4->Add( m_marginRightText, 1, wxALL, 5 );
    column4->Add( m_marginBottomText, 1, wxALL, 5 );
    table->Add( column4, 0, wxRIGHT|wxTOP|wxBOTTOM | wxEXPAND, 5 );

    mainsizer->Add( table, 0 );

#if wxUSE_STATLINE
    // 5) static line
    mainsizer->Add( new wxStaticLine( this, -1 ), 0, wxEXPAND | wxLEFT|wxRIGHT|wxTOP, 10 );
#endif

    // 6) buttons
    
    wxSizer* buttonsizer = CreateButtonSizer( wxOK|wxCANCEL);
    m_printerButton = new wxButton(this, wxPRINTID_SETUP, _("Printer...") );
    buttonsizer->Add( m_printerButton, 0, wxLEFT|wxRIGHT, 10 );
    if ( !m_pageData.GetEnablePrinter() )
        m_printerButton->Enable(FALSE);
    //  if (m_printData.GetEnableHelp())
    //  wxButton *helpButton = new wxButton(this, (wxFunction)wxGenericPageSetupHelpProc, _("Help"), -1, -1, buttonWidth, buttonHeight);
    mainsizer->Add( buttonsizer, 0, wxCENTER|wxALL, 10 );


    SetAutoLayout( TRUE );
    SetSizer( mainsizer );

    mainsizer->Fit( this );
    Centre(wxBOTH);

    InitDialog();
    
    delete[] choices;
    delete [] choices2;
}

wxGenericPageSetupDialog::~wxGenericPageSetupDialog()
{
}

bool wxGenericPageSetupDialog::TransferDataToWindow()
{
    if (m_marginLeftText)
        m_marginLeftText->SetValue(IntToString((int) m_pageData.GetMarginTopLeft().x));
    if (m_marginTopText)
        m_marginTopText->SetValue(IntToString((int) m_pageData.GetMarginTopLeft().y));
    if (m_marginRightText)
        m_marginRightText->SetValue(IntToString((int) m_pageData.GetMarginBottomRight().x));
    if (m_marginBottomText)
        m_marginBottomText->SetValue(IntToString((int) m_pageData.GetMarginBottomRight().y));

    if (m_orientationRadioBox)
    {
        if (m_pageData.GetPrintData().GetOrientation() == wxPORTRAIT)
            m_orientationRadioBox->SetSelection(0);
        else
            m_orientationRadioBox->SetSelection(1);
    }

    // Find the paper type from either the current paper size in the wxPageSetupDialogData, or
    // failing that, the id in the wxPrintData object.

    wxPrintPaperType* type = wxThePrintPaperDatabase->FindPaperType(
             wxSize(m_pageData.GetPaperSize().x * 10, m_pageData.GetPaperSize().y * 10));

    if (!type && m_pageData.GetPrintData().GetPaperId() != wxPAPER_NONE)
        type = wxThePrintPaperDatabase->FindPaperType(m_pageData.GetPrintData().GetPaperId());

    if (type)
    {
        m_paperTypeChoice->SetStringSelection(type->GetName());
    }

    return TRUE;
}

bool wxGenericPageSetupDialog::TransferDataFromWindow()
{
    if (m_marginLeftText && m_marginTopText)
        m_pageData.SetMarginTopLeft(wxPoint(wxAtoi((const wxChar *)m_marginLeftText->GetValue()),wxAtoi((const wxChar *)m_marginTopText->GetValue())));
    if (m_marginRightText && m_marginBottomText)
        m_pageData.SetMarginBottomRight(wxPoint(wxAtoi((const wxChar *)m_marginRightText->GetValue()),wxAtoi((const wxChar *)m_marginBottomText->GetValue())));

    if (m_orientationRadioBox)
    {
        int sel = m_orientationRadioBox->GetSelection();
        if (sel == 0)
        {
#if wxUSE_POSTSCRIPT
            wxThePrintSetupData->SetPrinterOrientation(wxPORTRAIT);
#endif
            m_pageData.GetPrintData().SetOrientation(wxPORTRAIT);
        }
        else
        {
#if wxUSE_POSTSCRIPT
            wxThePrintSetupData->SetPrinterOrientation(wxLANDSCAPE);
#endif
            m_pageData.GetPrintData().SetOrientation(wxLANDSCAPE);
        }
    }
    if (m_paperTypeChoice)
    {
        wxString val(m_paperTypeChoice->GetStringSelection());
        if (!val.IsNull() && val != "")
        {
            wxPrintPaperType* paper = wxThePrintPaperDatabase->FindPaperType(val);
            if ( paper )
            {
                m_pageData.SetPaperSize(wxSize(paper->GetWidth()/10, paper->GetHeight()/10));
                m_pageData.GetPrintData().SetPaperId(paper->GetId());
            }
        }
    }

    return TRUE;
}

wxComboBox *wxGenericPageSetupDialog::CreatePaperTypeChoice(int *x, int *y)
{
/*
    if (!wxThePrintPaperDatabase)
    {
        wxThePrintPaperDatabase = new wxPrintPaperDatabase;
        wxThePrintPaperDatabase->CreateDatabase();
    }
*/

    int n = wxThePrintPaperDatabase->Number();
    wxString *choices = new wxString [n];
    int i;
    for (i = 0; i < n; i++)
    {
        wxPrintPaperType *paper = (wxPrintPaperType *)wxThePrintPaperDatabase->Nth(i)->Data();
        choices[i] = paper->GetName();
    }

    (void) new wxStaticText(this, wxPRINTID_STATIC, _("Paper size"), wxPoint(*x, *y));
    *y += 25;

    wxComboBox *choice = new wxComboBox(this, wxPRINTID_PAPERSIZE,
                                        _("Paper Size"),
                                        wxPoint(*x, *y), wxSize(300, -1), n,
        choices);
    *y += 35;
    delete[] choices;

//    choice->SetSelection(sel);
    return choice;
}

#endif
