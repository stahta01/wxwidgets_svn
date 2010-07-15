/////////////////////////////////////////////////////////////////////////////
// Name:        wx/qt/dialog.h
// Author:      Peter Most, Javier Torres
// Id:          $Id$
// Copyright:   (c) Peter Most, Javier Torres
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_QT_DIALOG_H_
#define _WX_QT_DIALOG_H_

#include "wx/qt/winevent_qt.h"
#include <QtGui/QDialog>

class WXDLLIMPEXP_CORE wxDialog : public wxDialogBase
{
public:
    wxDialog();
    wxDialog( wxWindow *parent, wxWindowID id,
            const wxString &title,
            const wxPoint &pos = wxDefaultPosition,
            const wxSize &size = wxDefaultSize,
            long style = wxDEFAULT_DIALOG_STYLE,
            const wxString &name = wxDialogNameStr );
    bool Create( wxWindow *parent, wxWindowID id,
            const wxString &title,
            const wxPoint &pos = wxDefaultPosition,
            const wxSize &size = wxDefaultSize,
            long style = wxDEFAULT_DIALOG_STYLE,
            const wxString &name = wxDialogNameStr );

    virtual int ShowModal();
    virtual void EndModal(int retCode);
    virtual bool IsModal() const;

    virtual QDialog *GetHandle() const;
    virtual WXWidget QtGetScrollBarsContainer() const;

private:
    QPointer< QDialog > m_qtDialog;
};

class WXDLLIMPEXP_CORE wxQtDialog : public wxQtEventForwarder< QDialog >
{
    Q_OBJECT
    
    public:
        wxQtDialog( wxDialog *frame, QWidget *parent );
        
    protected:
        virtual wxWindow *GetEventReceiver();
        
    private:
        wxDialog *m_dialog;
        
};

#endif // _WX_QT_DIALOG_H_
