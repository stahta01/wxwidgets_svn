/////////////////////////////////////////////////////////////////////////////
// Name:        msgdlg.h
// Purpose:     wxMessageDialog for GTK+2
// Author:      Vaclav Slavik
// Modified by:
// Created:     2003/02/28
// RCS-ID:      $Id$
// Copyright:   (c) Vaclav Slavik, 2003
// Licence:   	wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __MSGDLG_H__
#define __MSGDLG_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "msgdlg.h"
#endif

#include "wx/setup.h"
#include "wx/dialog.h"

// type is an 'or' (|) of wxOK, wxCANCEL, wxYES_NO
// Returns wxYES/NO/OK/CANCEL

WXDLLEXPORT_DATA(extern const wxChar*) wxMessageBoxCaptionStr;

class WXDLLEXPORT wxMessageDialog: public wxDialog
{
public:
    wxMessageDialog(wxWindow *parent, const wxString& message,
                    const wxString& caption = wxMessageBoxCaptionStr,
                    long style = wxOK|wxCENTRE,
                    const wxPoint& pos = wxDefaultPosition);

    int ShowModal();
    virtual bool Show( bool show = true ) { return false; };

protected:
    // implement some base class methods to do nothing to avoid asserts and
    // GTK warnings, since this is not a real wxDialog.
    virtual void DoSetSize(int x, int y,
                           int width, int height,
                           int sizeFlags = wxSIZE_AUTO) {}
    virtual void DoMoveWindow(int x, int y, int width, int height) {}

private:
    wxString m_caption;
    wxString m_message;
    long m_dialogStyle;

    DECLARE_DYNAMIC_CLASS(wxMessageDialog)
};

#endif
