/////////////////////////////////////////////////////////////////////////////
// Name:        msg.h
// Purpose:     wxMailMessage
// Author:      Julian Smart
// Modified by:
// Created:     2001-08-21
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma interface "msg.h"
#endif

#ifndef _WX_MSG_H_
#define _WX_MSG_H_

/*
 * wxMailMessage
 * Encapsulates an email message
 */

class wxMailMessage
{
public:

    // A common usage
    wxMailMessage(const wxString& subject, const wxString& to,
        const wxString& body, const wxString& attachment = wxEmptyString,
        const wxString& attachmentTitle = wxEmptyString)
    {
        m_to.Add(to);
        m_subject = subject;
        m_body = body;
        if (!attachment.IsEmpty())
        {
            m_attachments.Add(attachment);
            m_attachmentTitles.Add(attachmentTitle);
        }
    }

    wxMailMessage() {};

//// Accessors

    void AddTo(const wxString& to) { m_to.Add(to); }
    void AddCc(const wxString& cc) { m_cc.Add(cc); }
    void AddBcc(const wxString& bcc) { m_bcc.Add(bcc); }
    void AddAttachment(const wxString& attach, const wxString& title = wxEmptyString)
    { m_attachments.Add(attach); m_attachmentTitles.Add(title); }

    void SetSubject(const wxString& subject) { m_subject = subject; }
    void SetBody(const wxString& body) { m_body = body; }

public:
    wxArrayString  m_to;               //The To: Recipients
    wxArrayString  m_cc;               //The CC: Recipients
    wxArrayString  m_bcc;              //The BCC Recipients
    wxString       m_subject;         //The Subject of the message
    wxString       m_body;            //The Body of the message
    wxArrayString  m_attachments;      //Files to attach to the email
    wxArrayString  m_attachmentTitles; //Titles to use for the email file attachments
};

#endif // _WX_MSG_H_

