/////////////////////////////////////////////////////////////////////////////
// Name:        socket.h
// Purpose:     Socket handling classes
// Authors:     Guilhem Lavaux, Guillermo Rodriguez Garcia
// Modified by:
// Created:     April 1997
// RCS-ID:      $Id$
// Copyright:   (c) Guilhem Lavaux
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_NETWORK_SOCKET_H
#define _WX_NETWORK_SOCKET_H

#ifdef __GNUG__
#pragma interface "socket.h"
#endif

#include "wx/defs.h"

#if wxUSE_SOCKETS

// ---------------------------------------------------------------------------
// wxSocket headers (generic)
// ---------------------------------------------------------------------------
#ifdef WXPREC
#  include "wx/wxprec.h"
#else
#  include "wx/event.h"
#  include "wx/string.h"
#endif

#include "wx/sckaddr.h"
#include "wx/gsocket.h"

// ------------------------------------------------------------------------
// GSocket type alias
// ------------------------------------------------------------------------

typedef enum {
  wxSOCKET_INPUT = GSOCK_INPUT,
  wxSOCKET_OUTPUT = GSOCK_OUTPUT,
  wxSOCKET_CONNECTION = GSOCK_CONNECTION,
  wxSOCKET_LOST = GSOCK_LOST
} wxSocketNotify;

enum {
  wxSOCKET_INPUT_FLAG = GSOCK_INPUT_FLAG,
  wxSOCKET_OUTPUT_FLAG = GSOCK_OUTPUT_FLAG,
  wxSOCKET_CONNECTION_FLAG = GSOCK_CONNECTION_FLAG,
  wxSOCKET_LOST_FLAG = GSOCK_LOST_FLAG
};

typedef GSocketEventFlags wxSocketEventFlags;

typedef enum {
  wxSOCKET_NOERROR = GSOCK_NOERROR,
  wxSOCKET_INPOP = GSOCK_INVOP,
  wxSOCKET_IOERR = GSOCK_IOERR,
  wxSOCKET_INVADDR = GSOCK_INVADDR,
  wxSOCKET_INVSOCK = GSOCK_INVSOCK,
  wxSOCKET_NOHOST = GSOCK_NOHOST,
  wxSOCKET_INVPORT = GSOCK_INVPORT,
  wxSOCKET_WOULDBLOCK = GSOCK_WOULDBLOCK,
  wxSOCKET_TIMEDOUT = GSOCK_TIMEDOUT,
  wxSOCKET_MEMERR = GSOCK_MEMERR
} wxSocketError;

// ------------------------------------------------------------------------
// wxSocket base
// ------------------------------------------------------------------------

class WXDLLEXPORT wxTimer;
class WXDLLEXPORT wxSocketEvent;
class WXDLLEXPORT wxSocketBase : public wxEvtHandler
{
  DECLARE_CLASS(wxSocketBase)
public:

  enum { NONE=0, NOWAIT=1, WAITALL=2, SPEED=4 };
  typedef int wxSockFlags;
  // Type of request

  enum wxSockType { SOCK_CLIENT, SOCK_SERVER, SOCK_INTERNAL, SOCK_UNINIT };
  typedef void (*wxSockCbk)(wxSocketBase& sock, wxSocketNotify evt, char *cdata);

protected:
  GSocket *m_socket;			// wxSocket socket
  wxSockFlags m_flags;			// wxSocket flags
  wxSockType m_type;			// wxSocket type
  wxSocketEventFlags m_neededreq;   // Which events we are interested in
  wxUint32 m_lcount;			// Last IO request size
  unsigned long m_timeout;		// IO timeout value
                     
  char *m_unread;               // Pushback buffer
  wxUint32 m_unrd_size;			// Pushback buffer size
  wxUint32 m_unrd_cur;			// Pushback pointer

  wxSockCbk m_cbk;              // C callback
  char *m_cdata;                // C callback data

  bool m_connected;             // Connected ?
  bool m_establishing;          // Pending connections ?
  bool m_notify_state;			// Notify state
  int m_id;                     // Socket id (for event handler)

  // Defering variables
  enum {
    DEFER_READ, DEFER_WRITE, NO_DEFER
  } m_defering;                 // Defering state
  char *m_defer_buffer;         // Defering target buffer
  wxUint32 m_defer_nbytes;      // Defering buffer size
  wxTimer *m_defer_timer;       // Timer for defering mode

  bool m_error;                 // Did an error occur in last IO call ?
  wxList m_states;              // Stack of states

public:
  wxSocketBase();
  virtual ~wxSocketBase();
  virtual bool Close();

  // Base IO
  wxSocketBase& Peek(char* buffer, wxUint32 nbytes);
  wxSocketBase& Read(char* buffer, wxUint32 nbytes);
  wxSocketBase& Write(const char *buffer, wxUint32 nbytes);
  wxSocketBase& Unread(const char *buffer, wxUint32 nbytes);
  wxSocketBase& ReadMsg(char *buffer, wxUint32 nbytes);
  wxSocketBase& WriteMsg(const char *buffer, wxUint32 nbytes);
  void Discard();

  // Status
  inline bool Ok() const { return (m_socket != NULL); };
  inline bool Error() const { return m_error; };
  inline bool IsConnected() const { return m_connected; };
  inline bool IsDisconnected() const { return !IsConnected(); };
  inline bool IsNoWait() const { return ((m_flags & NOWAIT) != 0); };
  bool IsData() const;
  inline wxUint32 LastCount() const { return m_lcount; }
  inline wxSocketError LastError() const { return (wxSocketError)GSocket_GetError(m_socket); }
  inline wxSockType GetType() const { return m_type; }

  // Some info on the socket...
  virtual bool GetPeer(wxSockAddress& addr_man) const;
  virtual bool GetLocal(wxSockAddress& addr_man) const;

  // Set attributes and flags
  void SetFlags(wxSockFlags _flags);
  wxSockFlags GetFlags() const;
  void SetTimeout(long seconds);

  // Wait functions
  //   seconds = -1 means default timeout (change with SetTimeout)
  //   seconds, milliseconds = 0 means no wait
  //   seconds, milliseconds > 0 means specified wait
  bool Wait(long seconds = -1, long milliseconds = 0);
  bool WaitForRead(long seconds = -1, long milliseconds = 0);
  bool WaitForWrite(long seconds = -1, long milliseconds = 0);
  bool WaitForLost(long seconds = -1, long milliseconds = 0);

  // Save the current state of Socket
  void SaveState();
  void RestoreState();

  // Setup event handler
  void SetEventHandler(wxEvtHandler& evt_hdlr, int id = -1);

  // Tell wxSocket which events to notify
  void SetNotify(wxSocketEventFlags flags);
  void Notify(bool notify);
  static wxSocketEventFlags EventToNotify(wxSocketNotify evt);
  inline wxSocketEventFlags NeededReq() const { return m_neededreq; }

  // External callback
  wxSockCbk Callback(wxSockCbk cbk_);
  char *CallbackData(char *data);

  // Public internal callback
  virtual void OldOnNotify(wxSocketNotify WXUNUSED(evt));

  // Do NOT use these functions; they should be protected!
  void CreatePushbackAfter(const char *buffer, wxUint32 size);
  void CreatePushbackBefore(const char *buffer, wxUint32 size);
  void OnRequest(wxSocketNotify req_evt);

protected:
  friend class wxSocketServer;
  friend class wxSocketClient;
  friend class wxSocketHandler;

#ifdef __SALFORDC__
public:
#endif

  wxSocketBase(wxSockFlags flags, wxSockType type);

#ifdef __SALFORDC__
protected:
#endif

  bool _Wait(long seconds, long milliseconds, wxSocketEventFlags flags);

  int DeferRead(char *buffer, wxUint32 nbytes);
  int DeferWrite(const char *buffer, wxUint32 nbytes);
  void DoDefer();

  // Pushback library
  wxUint32 GetPushback(char *buffer, wxUint32 size, bool peek);
};

////////////////////////////////////////////////////////////////////////

class WXDLLEXPORT wxSocketServer : public wxSocketBase
{
  DECLARE_CLASS(wxSocketServer)
public:
  // 'service' can be a name or a port-number

  wxSocketServer(wxSockAddress& addr_man, wxSockFlags flags = wxSocketBase::NONE);

  wxSocketBase* Accept(bool wait = TRUE);
  bool AcceptWith(wxSocketBase& sock, bool wait = TRUE);

  bool WaitForAccept(long seconds = -1, long milliseconds = 0);
};

////////////////////////////////////////////////////////////////////////

class WXDLLEXPORT wxSocketClient : public wxSocketBase
{
  DECLARE_CLASS(wxSocketClient)
public:

  wxSocketClient(wxSockFlags flags = wxSocketBase::NONE);
  virtual ~wxSocketClient();

  virtual bool Connect(wxSockAddress& addr_man, bool wait = TRUE);

  bool WaitOnConnect(long seconds = -1, long milliseconds = 0);
};

class WXDLLEXPORT wxSocketEvent : public wxEvent {
  DECLARE_DYNAMIC_CLASS(wxSocketEvent)
public:
  wxSocketEvent(int id = 0);

  wxSocketNotify SocketEvent() const { return m_skevt; }
  wxSocketBase *Socket() const { return m_socket; }

  void CopyObject(wxObject& obj_d) const;

public:
  wxSocketNotify m_skevt;
  wxSocketBase *m_socket;
};

typedef void (wxEvtHandler::*wxSocketEventFunction)(wxSocketEvent&);

#define EVT_SOCKET(id, func) { wxEVT_SOCKET, id, -1, \
  (wxObjectEventFunction) (wxEventFunction) (wxSocketEventFunction) & func, \
  (wxObject *) NULL  },

#endif
  // wxUSE_SOCKETS

#endif
  // _WX_NETWORK_SOCKET_H
