/////////////////////////////////////////////////////////////////////////////
// Name:        sckipc.cpp
// Purpose:     Interprocess communication implementation (wxSocket version)
// Author:      Julian Smart
// Modified by: Guilhem Lavaux (big rewrite) May 1997, 1998
//              Guillermo Rodriguez (updated for wxSocket v2) Jan 2000
// Created:     1993
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart 1993
//              (c) Guilhem Lavaux 1997, 1998
//              (c) 2000 Guillermo Rodriguez <guille@iies.es>
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "sckipc.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/defs.h"
#endif

#if wxUSE_SOCKETS && wxUSE_IPC

#include <stdlib.h>
#include <stdio.h>

#include "wx/socket.h"
#include "wx/sckipc.h"
#include "wx/log.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

IMPLEMENT_DYNAMIC_CLASS(wxTCPServer, wxServerBase)
IMPLEMENT_DYNAMIC_CLASS(wxTCPClient, wxClientBase)
IMPLEMENT_CLASS(wxTCPConnection, wxConnectionBase)

// It seems to be already defined somewhere in the Xt includes.
#ifndef __XT__
// Message codes
enum {
  IPC_EXECUTE = 1,
  IPC_REQUEST,
  IPC_POKE,
  IPC_ADVISE_START,
  IPC_ADVISE_REQUEST,
  IPC_ADVISE,
  IPC_ADVISE_STOP,
  IPC_REQUEST_REPLY,
  IPC_FAIL,
  IPC_CONNECT,
  IPC_DISCONNECT
};
#endif

void Server_OnRequest(wxSocketServer& server,
                      wxSocketNotify evt,
                      char *cdata);
void Client_OnRequest(wxSocketBase& sock,
                      wxSocketNotify evt,
                      char *cdata);


// All sockets will be created with the following flags

#define SCKIPC_FLAGS (wxSOCKET_WAITALL)

// ---------------------------------------------------------------------------
// wxTCPClient
// ---------------------------------------------------------------------------

wxTCPClient::wxTCPClient ()
  : wxClientBase()
{
}

wxTCPClient::~wxTCPClient ()
{
}

bool wxTCPClient::ValidHost(const wxString& host)
{
  wxIPV4address addr;

  return addr.Hostname(host);
}

wxConnectionBase *wxTCPClient::MakeConnection (const wxString& host,
                                               const wxString& server_name,
                                               const wxString& topic)
{
  wxSocketClient *client = new wxSocketClient(SCKIPC_FLAGS);
  wxSocketStream *stream = new wxSocketStream(*client);
  wxDataInputStream *data_is = new wxDataInputStream(*stream);
  wxDataOutputStream *data_os = new wxDataOutputStream(*stream);

  wxIPV4address addr;
  addr.Service(server_name);
  addr.Hostname(host);

  if (client->Connect(addr))
  {
    unsigned char msg;
  
    // Send topic name, and enquire whether this has succeeded
    data_os->Write8(IPC_CONNECT);
    data_os->WriteString(topic);
  
    msg = data_is->Read8();

    // OK! Confirmation.
    if (msg == IPC_CONNECT)
    {
      wxTCPConnection *connection = (wxTCPConnection *)OnMakeConnection ();

      if (connection)
      {
        if (connection->IsKindOf(CLASSINFO(wxTCPConnection)))
        {
          connection->m_topic = topic;
          connection->m_sock  = client;
          connection->m_sockstrm = stream;
          connection->m_codeci = data_is;
          connection->m_codeco = data_os;
          client->Callback(Client_OnRequest);
          client->CallbackData((char *)connection);
          client->SetNotify(wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
          client->Notify(TRUE);
          return connection;
        }
        else
        {
          delete connection;
          // and fall through to delete everything else
        }
      }
    }
  }

  // Something went wrong, delete everything
  delete data_is;
  delete data_os;
  delete stream;
  client->Destroy();

  return NULL;
}

wxConnectionBase *wxTCPClient::OnMakeConnection()
{
  return new wxTCPConnection;
}

// ---------------------------------------------------------------------------
// wxTCPServer
// ---------------------------------------------------------------------------

wxTCPServer::wxTCPServer ()
  : wxServerBase()
{
}

bool wxTCPServer::Create(const wxString& server_name)
{
  wxSocketServer *server;

  // wxIPV4address defaults to INADDR_ANY:0
  wxIPV4address addr;
  addr.Service(server_name);

  // Create a socket listening on specified port
  server = new wxSocketServer(addr, SCKIPC_FLAGS);
  server->Callback((wxSocketBase::wxSockCbk)Server_OnRequest);
  server->CallbackData((char *)this);
  server->SetNotify(wxSOCKET_CONNECTION_FLAG);
  server->Notify(TRUE);

  return TRUE;
}

wxTCPServer::~wxTCPServer()
{
}

wxConnectionBase *wxTCPServer::OnAcceptConnection( const wxString& WXUNUSED(topic) )
{
  return new wxTCPConnection();
}

// ---------------------------------------------------------------------------
// wxTCPConnection
// ---------------------------------------------------------------------------

wxTCPConnection::wxTCPConnection ()
  : wxConnectionBase(),
    m_sock(NULL), m_sockstrm(NULL), m_codeci(NULL), m_codeco(NULL)
{
}

wxTCPConnection::wxTCPConnection(char * WXUNUSED(buffer), int WXUNUSED(size))
{
}

wxTCPConnection::~wxTCPConnection ()
{
  wxDELETE(m_codeci);
  wxDELETE(m_codeco);
  wxDELETE(m_sockstrm);

  if (m_sock) m_sock->Destroy();
}

void wxTCPConnection::Compress(bool WXUNUSED(on))
{
  // Use wxLZWStream
}

// Calls that CLIENT can make.
bool wxTCPConnection::Disconnect ()
{
  // Send the the disconnect message to the peer.
  m_codeco->Write8(IPC_DISCONNECT);
  m_sock->Callback(NULL);
  m_sock->Close();

  return TRUE;
}

bool wxTCPConnection::Execute(const wxChar *data, int size, wxIPCFormat format)
{
  if (!m_sock->IsConnected())
    return FALSE;

  // Prepare EXECUTE message
  m_codeco->Write8(IPC_EXECUTE);
  m_codeco->Write8(format);

  if (size < 0)
    size = strlen(data) + 1;    // includes final NUL

  m_codeco->Write32(size);
  m_sockstrm->Write(data, size);

  return TRUE;
}

char *wxTCPConnection::Request (const wxString& item, int *size, wxIPCFormat format)
{
  if (!m_sock->IsConnected())
    return NULL;

  m_codeco->Write8(IPC_REQUEST);
  m_codeco->WriteString(item);
  m_codeco->Write8(format);

  // If Unpack doesn't initialize it.
  int ret;

  ret = m_codeci->Read8();
  if (ret == IPC_FAIL)
    return NULL;
  else
  {
    size_t s;
    char *data = NULL;

    s = m_codeci->Read32();
    data = new char[s];
    m_sockstrm->Read(data, s);

    if (size)
      *size = s;
    return data;
  }
}

bool wxTCPConnection::Poke (const wxString& item, wxChar *data, int size, wxIPCFormat format)
{
  if (!m_sock->IsConnected())
    return FALSE;

  m_codeco->Write8(IPC_POKE);
  m_codeco->WriteString(item);
  m_codeco->Write8(format);

  if (size < 0)
    size = strlen(data) + 1;    // includes final NUL

  m_codeco->Write32(size);
  m_sockstrm->Write(data, size);

  return TRUE;
}

bool wxTCPConnection::StartAdvise (const wxString& item)
{
  int ret;

  if (!m_sock->IsConnected())
    return FALSE;

  m_codeco->Write8(IPC_ADVISE_START);
  m_codeco->WriteString(item);

  ret = m_codeci->Read8();

  if (ret != IPC_FAIL)
    return TRUE;
  else
    return FALSE;
}

bool wxTCPConnection::StopAdvise (const wxString& item)
{
  int msg;

  if (!m_sock->IsConnected())
    return FALSE;

  m_codeco->Write8(IPC_ADVISE_STOP);
  m_codeco->WriteString(item);

  msg = m_codeci->Read8();

  if (msg != IPC_FAIL)
    return TRUE;
  else
    return FALSE;
}

// Calls that SERVER can make
bool wxTCPConnection::Advise (const wxString& item,
                              wxChar *data, int size, wxIPCFormat format)
{
  if (!m_sock->IsConnected())
    return FALSE;

  m_codeco->Write8(IPC_ADVISE);
  m_codeco->WriteString(item);
  m_codeco->Write8(format);

  if (size < 0)
    size = strlen(data) + 1;    // includes final NUL

  m_codeco->Write32(size);
  m_sockstrm->Write(data, size);

  return TRUE;
}

void Client_OnRequest(wxSocketBase& sock,
                      wxSocketNotify evt,
                      char *cdata)
{
  int msg = 0;
  wxTCPConnection *connection = (wxTCPConnection *)cdata;
  wxDataInputStream *codeci;
  wxDataOutputStream *codeco; 
  wxSocketStream *sockstrm;
  wxString topic_name = connection->m_topic;
  wxString item;

  // The socket handler signals us that we lost the connection: destroy all.
  if (evt == wxSOCKET_LOST)
  {
    sock.Callback(NULL);
    sock.Close();
    connection->OnDisconnect();
    return;
  }

  // Receive message number.
  codeci = connection->m_codeci;
  codeco = connection->m_codeco;
  sockstrm = connection->m_sockstrm;
  msg = codeci->Read8();

  switch (msg)
  {
  case IPC_EXECUTE:
  {
    char *data;
    size_t size; 
    wxIPCFormat format;
    
    format = (wxIPCFormat)codeci->Read8();
    size = codeci->Read32();
    data = new char[size];
    sockstrm->Read(data, size);

    connection->OnExecute (topic_name, data, size, format);

    delete [] data;
    break;
  }
  case IPC_ADVISE:
  {
    char *data;
    size_t size;
    wxIPCFormat format;

    item = codeci->ReadString();
    format = (wxIPCFormat)codeci->Read8();
    size = codeci->Read32();
    data = new char[size];
    sockstrm->Read(data, size);
    
    connection->OnAdvise (topic_name, item, data, size, format);

    delete [] data;
    break;
  }
  case IPC_ADVISE_START:
  {
    item = codeci->ReadString();

    bool ok = connection->OnStartAdvise (topic_name, item);
    if (ok)
      codeco->Write8(IPC_ADVISE_START);
    else
      codeco->Write8(IPC_FAIL);

    break;
  }
  case IPC_ADVISE_STOP:
  {
    item = codeci->ReadString();

    bool ok = connection->OnStopAdvise (topic_name, item);
    if (ok)
      codeco->Write8(IPC_ADVISE_STOP);
    else
      codeco->Write8(IPC_FAIL);

    break;
  }
  case IPC_POKE:
  {
    wxIPCFormat format;
    size_t size;
    wxChar *data;

    item = codeci->ReadString();
    format = (wxIPCFormat)codeci->Read8();
    size = codeci->Read32();
    data = new wxChar[size];
    sockstrm->Read(data, size);
    
    connection->OnPoke (topic_name, item, data, size, format);

    delete [] data;

    break;
  }
  case IPC_REQUEST:
  {
    wxIPCFormat format;

    item = codeci->ReadString();
    format = (wxIPCFormat)codeci->Read8();

    int user_size = -1;
    char *user_data = connection->OnRequest (topic_name, item, &user_size, format);

    if (user_data)
    {
      codeco->Write8(IPC_REQUEST_REPLY);

      if (user_size == -1)
        user_size = strlen(user_data) + 1;      // includes final NUL

      codeco->Write32(user_size);
      sockstrm->Write(user_data, user_size);
    }
    else
      codeco->Write8(IPC_FAIL);

    break;
  }
  case IPC_DISCONNECT:
  {
    wxLogDebug("IPC_DISCONNECT");
    sock.Callback(NULL);
    sock.Close();
    connection->OnDisconnect();
    break;
  }
  default:
    codeco->Write8(IPC_FAIL);
    break;
  }
}

void Server_OnRequest(wxSocketServer& server,
                      wxSocketNotify evt,
                      char *cdata)
{
  wxTCPServer *ipcserv = (wxTCPServer *)cdata;
  wxSocketStream *stream;
  wxDataInputStream *codeci;
  wxDataOutputStream *codeco;

  if (evt != wxSOCKET_CONNECTION)
    return;

  // Accept the connection, getting a new socket
  wxSocketBase *sock = server.Accept();
  if (!sock->Ok())
  {
    sock->Destroy();
    return;
  }

  stream = new wxSocketStream(*sock);
  codeci = new wxDataInputStream(*stream);
  codeco = new wxDataOutputStream(*stream);

  int msg;
  msg = codeci->Read8();

  if (msg == IPC_CONNECT)
  {
    wxString topic_name;
    topic_name = codeci->ReadString();

    wxTCPConnection *new_connection =
         (wxTCPConnection *)ipcserv->OnAcceptConnection (topic_name);

    if (new_connection)
    {
      if (new_connection->IsKindOf(CLASSINFO(wxTCPConnection)))
      {
        // Acknowledge success
        codeco->Write8(IPC_CONNECT);
        new_connection->m_topic = topic_name;
        new_connection->m_sock = sock;      
        new_connection->m_sockstrm = stream;
        new_connection->m_codeci = codeci;
        new_connection->m_codeco = codeco;
        sock->Callback(Client_OnRequest);
        sock->CallbackData((char *)new_connection);
        sock->SetNotify(wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
        sock->Notify(TRUE);
        return;
      }
      else
      {
        delete new_connection;
        // and fall through to delete everything else
      }
    }
  }

  // Something went wrong, send failure message and delete everything
  codeco->Write8(IPC_FAIL);

  delete codeco;
  delete codeci;
  delete stream;
  sock->Destroy();
}

#endif
    // wxUSE_SOCKETS && wxUSE_IPC
