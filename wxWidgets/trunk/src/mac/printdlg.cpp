/////////////////////////////////////////////////////////////////////////////
// Name:        printdlg.cpp
// Purpose:     wxPrintDialog, wxPageSetupDialog
// Author:      AUTHOR
// Modified by:
// Created:     ??/??/98
// RCS-ID:      $Id$
// Copyright:   (c) AUTHOR
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "printdlg.h"
#endif

#include "wx/object.h"
#include "wx/printdlg.h"
#include "wx/dcprint.h"

// Use generic page setup dialog: use your own native one if one exists.

IMPLEMENT_DYNAMIC_CLASS(wxPrintDialog, wxDialog)
IMPLEMENT_CLASS(wxPageSetupDialog, wxDialog)

wxPrintDialog::wxPrintDialog()
{
    m_dialogParent = NULL;
    m_printerDC = NULL;
    m_destroyDC = TRUE;
}

wxPrintDialog::wxPrintDialog(wxWindow *p, wxPrintDialogData* data)
{
    Create(p, data);
}

wxPrintDialog::wxPrintDialog(wxWindow *p, wxPrintData* data)
{
    wxPrintDialogData data2;
    if ( data )
        data2 = *data;

    Create(p, &data2);
}

bool wxPrintDialog::Create(wxWindow *p, wxPrintDialogData* data)
{
    m_dialogParent = p;
    m_printerDC = NULL;
    m_destroyDC = TRUE;

    if ( data )
        m_printDialogData = *data;

    return TRUE;
}

wxPrintDialog::~wxPrintDialog()
{
    if (m_destroyDC && m_printerDC)
        delete m_printerDC;
}

int wxPrintDialog::ShowModal()
{
	int result = wxID_CANCEL ;
	OSErr err ;
	wxString message ;
	::PrOpen() ;
	err = PrError() ;
	
	if ( !err )
	{
		m_printDialogData.ConvertToNative() ;
		if  ( m_printDialogData.m_macPrintInfo && ::PrJobDialog( m_printDialogData.m_macPrintInfo ) )
		{
			m_printDialogData.ConvertFromNative() ;
			result = wxID_OK ;
		}

	}
	else
	{
		message.Printf( "Print Error %d", err ) ;
		wxMessageDialog dialog( NULL , message  , "", wxICON_HAND | wxOK) ;
	}
	::PrClose() ;

	return result ;
}

wxDC *wxPrintDialog::GetPrintDC()
{
    return new wxPrinterDC( m_printDialogData.GetPrintData() ) ;
}

/*
 * wxPageSetupDialog
 */

wxPageSetupDialog::wxPageSetupDialog():
 wxDialog()
{
  m_dialogParent = NULL;
}

wxPageSetupDialog::wxPageSetupDialog(wxWindow *p, wxPageSetupData *data):
 wxDialog()
{
    Create(p, data);
}

bool wxPageSetupDialog::Create(wxWindow *p, wxPageSetupData *data)
{
    m_dialogParent = p;

    if (data)
        m_pageSetupData = (*data);

    return TRUE;
}

wxPageSetupDialog::~wxPageSetupDialog()
{
}

int wxPageSetupDialog::ShowModal()
{
	int result = wxID_CANCEL ;
	OSErr err ;
	wxString message ;
	::PrOpen() ;
	err = PrError() ;
	
	if ( !err )
	{
		m_pageSetupData.ConvertToNative() ;
		if  ( m_pageSetupData.m_macPageSetupInfo && ::PrStlDialog( m_pageSetupData.m_macPageSetupInfo ) )
		{
			m_pageSetupData.ConvertFromNative() ;
			result = wxID_OK ;
		}

	}
	else
	{
		message.Printf( "Print Error %d", err ) ;
		wxMessageDialog dialog( NULL , message , "", wxICON_HAND | wxOK) ;
	}
	::PrClose() ;

	return result ;
}

