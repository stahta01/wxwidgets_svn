/////////////////////////////////////////////////////////////////////////////
// Name:        wx/qt/printqt.h
// Author:      Peter Most
// Id:          $Id$
// Copyright:   (c) Peter Most
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_QT_PRINTQT_H_
#define _WX_QT_PRINTQT_H_

#include "wx/prntbase.h"

class WXDLLIMPEXP_CORE wxQtPrinter : public wxPrinterBase
{
public:
    wxQtPrinter( wxPrintDialogData *data = NULL );

    virtual bool Setup(wxWindow *parent);
    virtual bool Print(wxWindow *parent, wxPrintout *printout, bool prompt = true);
    virtual wxDC* PrintDialog(wxWindow *parent);
private:
};



class WXDLLIMPEXP_CORE wxQtPrintPreview : public wxPrintPreviewBase
{
public:
    wxQtPrintPreview(wxPrintout *printout,
                          wxPrintout *printoutForPrinting = NULL,
                          wxPrintDialogData *data = NULL);
    wxQtPrintPreview(wxPrintout *printout,
                          wxPrintout *printoutForPrinting,
                          wxPrintData *data);
    
    virtual bool Print(bool interactive);
    virtual void DetermineScaling();

protected:
};

#endif // _WX_QT_PRINTQT_H_
