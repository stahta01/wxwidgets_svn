/////////////////////////////////////////////////////////////////////////////
// Name:        msgdlg.cpp
// Purpose:     wxMessageDialog
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

#ifdef __GNUG__
    #pragma implementation "msgdlg.h"
#endif

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include <X11/Xlib.h>

#include <Xm/Xm.h>
#include <Xm/MessageB.h>

#include "wx/app.h"
#include "wx/intl.h"
#include "wx/motif/msgdlg.h"
#include "wx/motif/private.h"

// ----------------------------------------------------------------------------
// macros
// ----------------------------------------------------------------------------

#if !USE_SHARED_LIBRARY
    IMPLEMENT_CLASS(wxMessageDialog, wxDialog)
#endif

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// the callbacks for message box buttons
// ----------------------------------------------------------------------------

// the common part
static void msgboxCallBack(Widget w, int client_data, int id)
{
    // close the dialog
    XtUnmanageChild(w);

    wxMessageDialog *dlg = (wxMessageDialog *)client_data;
    dlg->SetResult(id);
}

static void msgboxCallBackOk(Widget w,
                             int client_data,
                             XmAnyCallbackStruct *WXUNUSED(call_data))
{
    msgboxCallBack(w, client_data, wxID_OK);
}

static void msgboxCallBackCancel(Widget w,
                                 int client_data,
                                 XmAnyCallbackStruct *WXUNUSED(call_data))
{
    msgboxCallBack(w, client_data, wxID_CANCEL);
}

static void msgboxCallBackHelp(Widget w,
                               int client_data,
                               XmAnyCallbackStruct *WXUNUSED(call_data))
{
    msgboxCallBack(w, client_data, wxID_HELP);
}

static void msgboxCallBackClose(Widget w,
                                int client_data,
                                XmAnyCallbackStruct *WXUNUSED(call_data))
{
    msgboxCallBack(w, client_data, wxID_CANCEL);
}

// ----------------------------------------------------------------------------
// wxMessageDialog
// ----------------------------------------------------------------------------

wxMessageDialog::wxMessageDialog(wxWindow *parent,
                                 const wxString& message,
                                 const wxString& caption,
                                 long style,
                                 const wxPoint& WXUNUSED(pos))
{
    m_caption = caption;
    m_message = message;
    m_dialogStyle = style;
    m_parent = parent;
}

int wxMessageDialog::ShowModal()
{
    Widget (*dialogCreateFunction)(Widget, String, ArgList, Cardinal) = NULL;
    if ( m_dialogStyle & wxYES_NO )
    {
        // if we have [Yes], it must be a question
        dialogCreateFunction = XmCreateQuestionDialog;
    }
    else if ( m_dialogStyle & wxICON_STOP )
    {
        // error dialog is the one with error icon...
        dialogCreateFunction = XmCreateErrorDialog;
    }
    else if ( m_dialogStyle & wxICON_EXCLAMATION )
    {
        // ...and the warning dialog too
        dialogCreateFunction = XmCreateWarningDialog;
    }
    else
    {
        // finally, use the info dialog by default
        dialogCreateFunction = XmCreateInformationDialog;
    }

    // prepare the arg list
    Arg args[2];
    int ac = 0;

    wxXmString text(m_message);
    wxXmString title(m_caption);
    XtSetArg(args[ac], XmNmessageString, text()); ac++;
    XtSetArg(args[ac], XmNdialogTitle, title()); ac++;

    // do create message box
    Widget wParent = m_parent ? GetWidget(m_parent) : (Widget) 0;
    if ( !wParent )
    {
        wxWindow *window = wxTheApp->GetTopWindow();
        if ( !window )
        {
            wxFAIL_MSG("can't show message box without parent window");

            return wxID_CANCEL;
        }

        wParent = GetWidget(window);
    }

    Widget wMsgBox = (*dialogCreateFunction)(wParent, "", args, ac);

    wxCHECK_MSG( wMsgBox, wxID_CANCEL, "msg box creation failed" );

    // get the buttons which we might either remove or rename
    // depending on the requested style
    //
    Widget wBtnOk = XmMessageBoxGetChild(wMsgBox, XmDIALOG_OK_BUTTON);
    Widget wBtnHelp = XmMessageBoxGetChild(wMsgBox, XmDIALOG_HELP_BUTTON);
    Widget wBtnCancel = XmMessageBoxGetChild(wMsgBox, XmDIALOG_CANCEL_BUTTON);

    if ( m_dialogStyle & wxYES_NO )
    {
        wxXmString yes(_("Yes")), no(_("No")), cancel(_("Cancel"));            

        if ( m_dialogStyle & wxCANCEL )
        {
            // use the cancel button for No and the help button for
            // Cancel  Yuk :-)  MB
            //
            XtVaSetValues(wBtnOk, XmNlabelString, yes(), NULL);
            XtVaSetValues(wBtnCancel, XmNlabelString, no(), NULL);
            XtVaSetValues(wBtnHelp, XmNlabelString, cancel(), NULL);
        }
        else
        {
            // no cancel button requested...
            // remove the help button and use cancel for no
            //
            XtVaSetValues(wBtnCancel, XmNlabelString, no(), NULL);
            XtUnmanageChild(wBtnHelp);
        }
    }
    else
    {
        // remove the help button and the cancel button (unless it was
        // requested)
        //
        XtUnmanageChild(wBtnHelp);
        if ( !(m_dialogStyle & wxCANCEL ) ) XtUnmanageChild(wBtnCancel);
    }

    // set the callbacks for the message box buttons
    XtAddCallback(wMsgBox, XmNokCallback,
                  (XtCallbackProc)msgboxCallBackOk, (XtPointer)this);
    XtAddCallback(wMsgBox, XmNcancelCallback,
                  (XtCallbackProc)msgboxCallBackCancel, (XtPointer)this);
    XtAddCallback(wMsgBox, XmNhelpCallback,
                  (XtCallbackProc)msgboxCallBackHelp, (XtPointer)this);
    XtAddCallback(wMsgBox, XmNunmapCallback,
                  (XtCallbackProc)msgboxCallBackClose, (XtPointer)this);

    // show it as a modal dialog
    XtManageChild(wMsgBox);
    XtAddGrab(wMsgBox, True, False);

    // the m_result will be changed when message box goes away
    m_result = -1;

    // local message loop
    XtAppContext context = XtWidgetToApplicationContext(wParent);
    XEvent event;
    while ( m_result == -1 )
    {
        XtAppNextEvent(context, &event);
        XtDispatchEvent(&event);
    }

    // translate the result if necessary
    if ( m_dialogStyle & wxYES_NO )
    {
        if ( m_result == wxID_OK )
            m_result = wxID_YES;
        else if ( m_result == wxID_CANCEL )
            m_result = wxID_NO;
        else if ( m_result == wxID_HELP )
            m_result = wxID_CANCEL;
    }

    return m_result;
}

