/////////////////////////////////////////////////////////////////////////////
// Name:        wx/qt/msgdlg.h
// Author:      Peter Most, Javier Torres
// Id:          $Id$
// Copyright:   (c) Peter Most, Javier Torres
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_QT_MSGDLG_H_
#define _WX_QT_MSGDLG_H_

#include "wx/msgdlg.h"

#include "wx/qt/winevent_qt.h"
#include <QtGui/QMessageBox>

class WXDLLIMPEXP_CORE wxMessageDialog : public wxMessageDialogBase
{
public:
    wxMessageDialog();
    wxMessageDialog(wxWindow *parent, const wxString& message,
                    const wxString& caption = wxMessageBoxCaptionStr,
                    long style = wxOK|wxCENTRE,
                    const wxPoint& pos = wxDefaultPosition);

    // Reimplemented to translate return codes from Qt to wx
    virtual int ShowModal();
    
protected:
    virtual QMessageBox *GetHandle() const;

private:
    QPointer< QMessageBox > m_qtMessageBox;
};

class WXDLLIMPEXP_CORE wxQtMessageDialog : public wxQtEventForwarder< QMessageBox >
{
    Q_OBJECT
    
    public:
        wxQtMessageDialog( wxMessageDialog *dialog, QWidget *parent );
        
    protected:
        virtual wxWindow *GetEventReceiver();
        
    private:
        wxMessageDialog *m_messageDialog;
        
};

#endif // _WX_QT_MSGDLG_H_
