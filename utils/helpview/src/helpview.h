/////////////////////////////////////////////////////////////////////////////
// Name:        helpview.h
// Purpose:     HelpView application class
// Author:      Vaclav Slavik, Julian Smart
// Modified by:
// Created:     2002-07-09
// RCS-ID:      $Id$
// Copyright:   (c) 2002 Vaclav Slavik, Julian Smart and others
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_HELPVIEW_H_
#define _WX_HELPVIEW_H_

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "help.cpp"
#endif

// If 1, start a server to allow this to be used
// as an external help viewer.
#define hvUSE_IPC 1

#if hvUSE_IPC
#include <wx/ipc.h>

class hvConnection;
class hvServer;
#endif

/*!
 * The helpview application class.
 */

class hvApp : public wxApp
{
public:
    hvApp();

    /// Initialise the application.
    virtual bool OnInit();

    /// Clean up the application's data.
    virtual int OnExit();
    
    /// Prompt the user for a book to open
    bool OpenBook(wxHtmlHelpController* controller);

    /// Returns the help controller.
    wxHtmlHelpController* GetHelpController() { return m_helpController; }

#if hvUSE_IPC
    /// Returns the list of connections.
    wxList& GetConnections() { return m_connections; }
#endif

private:
    wxHtmlHelpController*   m_helpController;
    
#if hvUSE_IPC
    wxList                  m_connections;
    hvServer*               m_server;
#endif
    
};

#if hvUSE_IPC
class hvConnection : public wxConnection
{
public:
    hvConnection();
    ~hvConnection();

    bool OnExecute(const wxString& topic, wxChar*data, int size, wxIPCFormat format);
    wxChar *OnRequest(const wxString& topic, const wxString& item, int *size, wxIPCFormat format);
    bool OnPoke(const wxString& topic, const wxString& item, wxChar *data, int size, wxIPCFormat format);
    bool OnStartAdvise(const wxString& topic, const wxString& item);

private:
};

class hvServer: public wxServer
{
public:
    wxConnectionBase *OnAcceptConnection(const wxString& topic);
};


#endif

#endif
  // _WX_HELPVIEW_H_

