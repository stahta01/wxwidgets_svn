/////////////////////////////////////////////////////////////////////////////
// Name:        print.cpp
// Purpose:     Print framework
// Author:      Julian Smart
// Modified by:
// Created:     17/09/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "print.h"
#endif

#include "wx/motif/print.h"
#include "wx/motif/printdlg.h"

#if !USE_SHARED_LIBRARY
IMPLEMENT_DYNAMIC_CLASS(wxPrinter, wxPrinterBase)
IMPLEMENT_CLASS(wxPrintPreview, wxPrintPreviewBase)
#endif

/*
 * Printer
 */
 
wxPrinter::wxPrinter(wxPrintData *data):
  wxPrinterBase(data)
{
}

wxPrinter::~wxPrinter()
{
}

bool wxPrinter::Print(wxWindow *parent, wxPrintout *printout, bool prompt)
{
    // TODO. See wxPostScriptPrinter::Print for hints.
    return FALSE;
}

bool wxPrinter::PrintDialog(wxWindow *parent)
{
    wxPrintDialog dialog(parent, & m_printData);
    return (dialog.ShowModal() == wxID_OK);
}

bool wxPrinter::Setup(wxWindow *parent)
{
    wxPrintDialog dialog(parent, & m_printData);
    dialog.GetPrintData().SetSetupDialog(TRUE);
    return (dialog.ShowModal() == wxID_OK);
}

/*
 * Print preview
 */

wxPrintPreview::wxPrintPreview(wxPrintout *printout, wxPrintout *printoutForPrinting, wxPrintData *data):
  wxPrintPreviewBase(printout, printoutForPrinting, data)
{
    DetermineScaling();
}

wxPrintPreview::~wxPrintPreview()
{
}

bool wxPrintPreview::Print(bool interactive)
{
    if (!m_printPrintout)
        return FALSE;
    wxPrinter printer(&m_printData);
    return printer.Print(m_previewFrame, m_printPrintout, interactive);
}

void wxPrintPreview::DetermineScaling()
{
    // TODO
}

