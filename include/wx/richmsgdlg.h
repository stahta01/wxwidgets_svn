/////////////////////////////////////////////////////////////////////////////
// Name:        wx/richmsgdlg.h
// Purpose:     wxRichMessageDialogBase
// Author:      Rickard Westerlund
// Created:     2010-07-03
// RCS-ID:      $Id$
// Copyright:   (c) 2010 wxWidgets team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_RICHMSGDLG_H_BASE_
#define _WX_RICHMSGDLG_H_BASE_

#include "wx/defs.h"

#if wxUSE_RICHMSGDLG

#include "wx/msgdlg.h"

// Extends a message dialog with an optional checkbox and user-expandable
// detailed text.
class WXDLLIMPEXP_CORE wxRichMessageDialogBase : public wxGenericMessageDialog
{
public:
    wxRichMessageDialogBase( wxWindow *parent,
                             const wxString& message,
                             const wxString& caption,
                             long style )
        : wxGenericMessageDialog( parent, message, caption, style ),
          m_detailsExpanderCollapsedLabel( _("&See details") ),
          m_detailsExpanderExpandedLabel( _("&Hide details") ),
          m_checkBoxValue( false )
        { }

    void ShowCheckBox(const wxString &checkBoxText, bool checked = false)
    {
        m_checkBoxText = checkBoxText;
        m_checkBoxValue = checked;
    }

    wxString GetCheckBoxText() const { return m_checkBoxText; }

    void ShowDetailedText(const wxString &detailedText)
        { m_detailedText = detailedText; }

    wxString GetDetailedText() const { return m_detailedText; }

    virtual bool IsCheckBoxChecked() const { return m_checkBoxValue; };

protected:
    const wxString m_detailsExpanderCollapsedLabel;
    const wxString m_detailsExpanderExpandedLabel;

    wxString m_checkBoxText;
    bool m_checkBoxValue;
    wxString m_detailedText;

private:
    void ShowDetails(bool shown);

    wxDECLARE_NO_COPY_CLASS(wxRichMessageDialogBase);
};

#include "wx/generic/richmsgdlgg.h"

#ifdef __WXMSW__
    #include "wx/msw/richmsgdlg.h"
#else
    class WXDLLIMPEXP_CORE wxRichMessageDialog
                           : public wxGenericRichMessageDialog
    {
    public:
        wxRichMessageDialog( wxWindow *parent,
                             const wxString& message,
                             const wxString& caption,
                             long style )
            : wxGenericRichMessageDialog( parent, message, caption, style )
            { }

    private:
        wxDECLARE_NO_COPY_CLASS( wxRichMessageDialog );
    };
#endif

#endif // wxUSE_RICHMSGDLG

#endif // _WX_RICHMSGDLG_H_BASE_
