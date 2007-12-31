/* -------------------------------------------------------------------------
 * Project:     GSocket (Generic Socket)
 * Name:        src/msw/gsocket.cpp
 * Copyright:   (c) Guilhem Lavaux
 * Licence:     wxWindows Licence
 * Author:      Guillermo Rodriguez Garcia <guille@iies.es>
 * Purpose:     GSocket main MSW file
 * Licence:     The wxWindows licence
 * CVSID:       $Id$
 * -------------------------------------------------------------------------
 */

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifdef _MSC_VER
   /* RPCNOTIFICATION_ROUTINE in rasasync.h (included from winsock.h),
    * warning: conditional expression is constant.
    */
#  pragma warning(disable:4115)
   /* FD_SET,
    * warning: named type definition in parentheses.
    */
#  pragma warning(disable:4127)
   /* GAddress_UNIX_GetPath,
    * warning: unreferenced formal parameter.
    */
#  pragma warning(disable:4100)

#ifdef __WXWINCE__
    /* windows.h results in tons of warnings at max warning level */
#   ifdef _MSC_VER
#       pragma warning(push, 1)
#   endif
#   include <windows.h>
#   ifdef _MSC_VER
#       pragma warning(pop)
#       pragma warning(disable:4514)
#   endif
#endif

#endif /* _MSC_VER */

#if defined(__CYGWIN__)
    //CYGWIN gives annoying warning about runtime stuff if we don't do this
#   define USE_SYS_TYPES_FD_SET
#   include <sys/types.h>
#endif

#include <winsock.h>

#include "wx/platform.h"

#if wxUSE_SOCKETS

#include "wx/gsocket.h"
#include "wx/link.h"

wxFORCE_LINK_MODULE(gsockmsw)

#ifdef __WXWINCE__
#ifndef isdigit
#define isdigit(x) (x > 47 && x < 58)
#endif
#include "wx/msw/wince/net.h"
#endif

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>

/* if we use configure for MSW WX_SOCKLEN_T will be already defined */
#ifndef WX_SOCKLEN_T
#  define WX_SOCKLEN_T int
#endif

#if wxUSE_IPV6
typedef struct sockaddr_storage wxSockAddr;
#else
typedef struct sockaddr wxSockAddr;
#endif

bool GSocket_Init()
{
  WSADATA wsaData;

  GSocketManager * const manager = GSocketManager::Get();
  if ( !manager || !manager->OnInit() )
      return false;

  /* Initialize WinSocket */
  return WSAStartup((1 << 8) | 1, &wsaData) == 0;
}

void GSocket_Cleanup()
{
  GSocketManager * const manager = GSocketManager::Get();
  if ( manager )
    manager->OnExit();

  /* Cleanup WinSocket */
  WSACleanup();
}

/* Constructors / Destructors for GSocket */

GSocket::GSocket()
{
  int i;

  m_fd              = INVALID_SOCKET;
  for (i = 0; i < GSOCK_MAX_EVENT; i++)
  {
    m_cbacks[i]     = NULL;
  }
  m_detected        = 0;
  m_local           = NULL;
  m_peer            = NULL;
  m_error           = GSOCK_NOERROR;
  m_server          = false;
  m_stream          = true;
  m_non_blocking    = false;
  m_timeout.tv_sec  = 10 * 60;  /* 10 minutes */
  m_timeout.tv_usec = 0;
  m_establishing    = false;
  m_reusable        = false;
  m_broadcast       = false;
  m_dobind          = true;
  m_initialRecvBufferSize = -1;
  m_initialSendBufferSize = -1;

  m_ok = GSocketManager::Get()->Init_Socket(this);
}

void GSocket::Close()
{
    GSocketManager::Get()->Disable_Events(this);
    closesocket(m_fd);
    m_fd = INVALID_SOCKET;
}

GSocket::~GSocket()
{
  GSocketManager::Get()->Destroy_Socket(this);

  /* Check that the socket is really shutdowned */
  if (m_fd != INVALID_SOCKET)
    Shutdown();

  /* Destroy private addresses */
  if (m_local)
    GAddress_destroy(m_local);

  if (m_peer)
    GAddress_destroy(m_peer);
}

/* GSocket_Shutdown:
 *  Disallow further read/write operations on this socket, close
 *  the fd and disable all callbacks.
 */
void GSocket::Shutdown()
{
  int evt;

  /* If socket has been created, shutdown it */
  if (m_fd != INVALID_SOCKET)
  {
    shutdown(m_fd, 1 /* SD_SEND */);
    Close();
  }

  /* Disable GUI callbacks */
  for (evt = 0; evt < GSOCK_MAX_EVENT; evt++)
    m_cbacks[evt] = NULL;

  m_detected = GSOCK_LOST_FLAG;
}

/* Address handling */

/* GSocket_SetLocal:
 * GSocket_GetLocal:
 * GSocket_SetPeer:
 * GSocket_GetPeer:
 *  Set or get the local or peer address for this socket. The 'set'
 *  functions return GSOCK_NOERROR on success, an error code otherwise.
 *  The 'get' functions return a pointer to a GAddress object on success,
 *  or NULL otherwise, in which case they set the error code of the
 *  corresponding GSocket.
 *
 *  Error codes:
 *    GSOCK_INVSOCK - the socket is not valid.
 *    GSOCK_INVADDR - the address is not valid.
 */
GSocketError GSocket::SetLocal(GAddress *address)
{
  /* the socket must be initialized, or it must be a server */
  if (m_fd != INVALID_SOCKET && !m_server)
  {
    m_error = GSOCK_INVSOCK;
    return GSOCK_INVSOCK;
  }

  /* check address */
  if (address == NULL || address->m_family == GSOCK_NOFAMILY)
  {
    m_error = GSOCK_INVADDR;
    return GSOCK_INVADDR;
  }

  if (m_local)
    GAddress_destroy(m_local);

  m_local = GAddress_copy(address);

  return GSOCK_NOERROR;
}

GSocketError GSocket::SetPeer(GAddress *address)
{
  /* check address */
  if (address == NULL || address->m_family == GSOCK_NOFAMILY)
  {
    m_error = GSOCK_INVADDR;
    return GSOCK_INVADDR;
  }

  if (m_peer)
    GAddress_destroy(m_peer);

  m_peer = GAddress_copy(address);

  return GSOCK_NOERROR;
}

GAddress *GSocket::GetLocal()
{
  GAddress *address;
  wxSockAddr addr;
  WX_SOCKLEN_T size = sizeof(addr);
  GSocketError err;

  /* try to get it from the m_local var first */
  if (m_local)
    return GAddress_copy(m_local);

  /* else, if the socket is initialized, try getsockname */
  if (m_fd == INVALID_SOCKET)
  {
    m_error = GSOCK_INVSOCK;
    return NULL;
  }

  if (getsockname(m_fd, (sockaddr*)&addr, &size) == SOCKET_ERROR)
  {
    m_error = GSOCK_IOERR;
    return NULL;
  }

  /* got a valid address from getsockname, create a GAddress object */
  if ((address = GAddress_new()) == NULL)
  {
     m_error = GSOCK_MEMERR;
     return NULL;
  }

  if ((err = _GAddress_translate_from(address, (sockaddr*)&addr, size)) != GSOCK_NOERROR)
  {
     GAddress_destroy(address);
     m_error = err;
     return NULL;
  }

  return address;
}

GAddress *GSocket::GetPeer()
{
  /* try to get it from the m_peer var */
  if (m_peer)
    return GAddress_copy(m_peer);

  return NULL;
}

/* Server specific parts */

/* GSocket_SetServer:
 *  Sets up this socket as a server. The local address must have been
 *  set with GSocket_SetLocal() before GSocket_SetServer() is called.
 *  Returns GSOCK_NOERROR on success, one of the following otherwise:
 *
 *  Error codes:
 *    GSOCK_INVSOCK - the socket is in use.
 *    GSOCK_INVADDR - the local address has not been set.
 *    GSOCK_IOERR   - low-level error.
 */
GSocketError GSocket::SetServer()
{
  u_long arg = 1;

  /* must not be in use */
  if (m_fd != INVALID_SOCKET)
  {
    m_error = GSOCK_INVSOCK;
    return GSOCK_INVSOCK;
  }

  /* the local addr must have been set */
  if (!m_local)
  {
    m_error = GSOCK_INVADDR;
    return GSOCK_INVADDR;
  }

  /* Initialize all fields */
  m_server   = true;
  m_stream   = true;

  /* Create the socket */
  m_fd = socket(m_local->m_realfamily, SOCK_STREAM, 0);

  if (m_fd == INVALID_SOCKET)
  {
    m_error = GSOCK_IOERR;
    return GSOCK_IOERR;
  }

  ioctlsocket(m_fd, FIONBIO, (u_long FAR *) &arg);
  GSocketManager::Get()->Enable_Events(this);

  /* allow a socket to re-bind if the socket is in the TIME_WAIT
     state after being previously closed.
   */
  if (m_reusable)
  {
    setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&arg, sizeof(arg));
  }

  /* Bind to the local address,
   * retrieve the actual address bound,
   * and listen up to 5 connections.
   */
  if ((bind(m_fd, m_local->m_addr, m_local->m_len) != 0) ||
      (getsockname(m_fd,
                   m_local->m_addr,
                   (WX_SOCKLEN_T *)&m_local->m_len) != 0) ||
      (listen(m_fd, 5) != 0))
  {
    Close();
    m_error = GSOCK_IOERR;
    return GSOCK_IOERR;
  }

  return GSOCK_NOERROR;
}

/* GSocket_WaitConnection:
 *  Waits for an incoming client connection. Returns a pointer to
 *  a GSocket object, or NULL if there was an error, in which case
 *  the last error field will be updated for the calling GSocket.
 *
 *  Error codes (set in the calling GSocket)
 *    GSOCK_INVSOCK    - the socket is not valid or not a server.
 *    GSOCK_TIMEDOUT   - timeout, no incoming connections.
 *    GSOCK_WOULDBLOCK - the call would block and the socket is nonblocking.
 *    GSOCK_MEMERR     - couldn't allocate memory.
 *    GSOCK_IOERR      - low-level error.
 */
GSocket *GSocket::WaitConnection()
{
  GSocket *connection;
  wxSockAddr from;
  WX_SOCKLEN_T fromlen = sizeof(from);
  GSocketError err;
  u_long arg = 1;

  /* Reenable CONNECTION events */
  m_detected &= ~GSOCK_CONNECTION_FLAG;

  /* If the socket has already been created, we exit immediately */
  if (m_fd == INVALID_SOCKET || !m_server)
  {
    m_error = GSOCK_INVSOCK;
    return NULL;
  }

  /* Create a GSocket object for the new connection */
  connection = GSocket_new();

  if (!connection)
  {
    m_error = GSOCK_MEMERR;
    return NULL;
  }

  /* Wait for a connection (with timeout) */
  if (Input_Timeout() == GSOCK_TIMEDOUT)
  {
    delete connection;
    /* m_error set by _GSocket_Input_Timeout */
    return NULL;
  }

  connection->m_fd = accept(m_fd, (sockaddr*)&from, &fromlen);

  if (connection->m_fd == INVALID_SOCKET)
  {
    if (WSAGetLastError() == WSAEWOULDBLOCK)
      m_error = GSOCK_WOULDBLOCK;
    else
      m_error = GSOCK_IOERR;

    delete connection;
    return NULL;
  }

  /* Initialize all fields */
  connection->m_server   = false;
  connection->m_stream   = true;

  /* Setup the peer address field */
  connection->m_peer = GAddress_new();
  if (!connection->m_peer)
  {
    delete connection;
    m_error = GSOCK_MEMERR;
    return NULL;
  }
  err = _GAddress_translate_from(connection->m_peer, (sockaddr*)&from, fromlen);
  if (err != GSOCK_NOERROR)
  {
    GAddress_destroy(connection->m_peer);
    delete connection;
    m_error = err;
    return NULL;
  }

  ioctlsocket(connection->m_fd, FIONBIO, (u_long FAR *) &arg);
  GSocketManager::Get()->Enable_Events(connection);

  return connection;
}

/* GSocket_SetReusable:
*  Simply sets the m_resuable flag on the socket. GSocket_SetServer will
*  make the appropriate setsockopt() call.
*  Implemented as a GSocket function because clients (ie, wxSocketServer)
*  don't have access to the GSocket struct information.
*  Returns true if the flag was set correctly, false if an error occurred
*  (ie, if the parameter was NULL)
*/
bool GSocket::SetReusable()
{
    /* socket must not be null, and must not be in use/already bound */
    if (this && m_fd == INVALID_SOCKET) {
        m_reusable = true;
        return true;
    }
    return false;
}

/* GSocket_SetBroadcast:
*  Simply sets the m_broadcast flag on the socket. GSocket_SetServer will
*  make the appropriate setsockopt() call.
*  Implemented as a GSocket function because clients (ie, wxSocketServer)
*  don't have access to the GSocket struct information.
*  Returns true if the flag was set correctly, false if an error occurred
*  (ie, if the parameter was NULL)
*/
bool GSocket::SetBroadcast()
{
    /* socket must not be in use/already bound */
    if (m_fd == INVALID_SOCKET) {
        m_broadcast = true;
        return true;
    }
    return false;
}

bool GSocket::DontDoBind()
{
    /* socket must not be in use/already bound */
    if (m_fd == INVALID_SOCKET) {
        m_dobind = false;
        return true;
    }
    return false;
}

/* Client specific parts */

/* GSocket_Connect:
 *  For stream (connection oriented) sockets, GSocket_Connect() tries
 *  to establish a client connection to a server using the peer address
 *  as established with GSocket_SetPeer(). Returns GSOCK_NOERROR if the
 *  connection has been successfully established, or one of the error
 *  codes listed below. Note that for nonblocking sockets, a return
 *  value of GSOCK_WOULDBLOCK doesn't mean a failure. The connection
 *  request can be completed later; you should use GSocket_Select()
 *  to poll for GSOCK_CONNECTION | GSOCK_LOST, or wait for the
 *  corresponding asynchronous events.
 *
 *  For datagram (non connection oriented) sockets, GSocket_Connect()
 *  just sets the peer address established with GSocket_SetPeer() as
 *  default destination.
 *
 *  Error codes:
 *    GSOCK_INVSOCK    - the socket is in use or not valid.
 *    GSOCK_INVADDR    - the peer address has not been established.
 *    GSOCK_TIMEDOUT   - timeout, the connection failed.
 *    GSOCK_WOULDBLOCK - connection in progress (nonblocking sockets only)
 *    GSOCK_MEMERR     - couldn't allocate memory.
 *    GSOCK_IOERR      - low-level error.
 */
GSocketError GSocket::Connect(GSocketStream stream)
{
  int ret, err;
  u_long arg = 1;

  /* Enable CONNECTION events (needed for nonblocking connections) */
  m_detected &= ~GSOCK_CONNECTION_FLAG;

  if (m_fd != INVALID_SOCKET)
  {
    m_error = GSOCK_INVSOCK;
    return GSOCK_INVSOCK;
  }

  if (!m_peer)
  {
    m_error = GSOCK_INVADDR;
    return GSOCK_INVADDR;
  }

  /* Streamed or dgram socket? */
  m_stream   = (stream == GSOCK_STREAMED);
  m_server   = false;
  m_establishing = false;

  /* Create the socket */
  m_fd = socket(m_peer->m_realfamily,
                     m_stream? SOCK_STREAM : SOCK_DGRAM, 0);

  if (m_fd == INVALID_SOCKET)
  {
    m_error = GSOCK_IOERR;
    return GSOCK_IOERR;
  }

  ioctlsocket(m_fd, FIONBIO, (u_long FAR *) &arg);
  GSocketManager::Get()->Enable_Events(this);

  // If the reuse flag is set, use the applicable socket reuse flag
  if (m_reusable)
  {
     setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&arg, sizeof(arg));
  }

  if (m_initialRecvBufferSize >= 0)
    setsockopt(m_fd, SOL_SOCKET, SO_RCVBUF, (const char*)&m_initialRecvBufferSize, sizeof(m_initialRecvBufferSize));
  if (m_initialSendBufferSize >= 0)
    setsockopt(m_fd, SOL_SOCKET, SO_SNDBUF, (const char*)&m_initialSendBufferSize, sizeof(m_initialSendBufferSize));

  // If a local address has been set, then we need to bind to it before calling connect
  if (m_local && m_local->m_addr)
  {
    bind(m_fd, m_local->m_addr, m_local->m_len);
  }

  /* Connect it to the peer address, with a timeout (see below) */
  ret = connect(m_fd, m_peer->m_addr, m_peer->m_len);

  if (ret == SOCKET_ERROR)
  {
    err = WSAGetLastError();

    /* If connect failed with EWOULDBLOCK and the GSocket object
     * is in blocking mode, we select() for the specified timeout
     * checking for writability to see if the connection request
     * completes.
     */
    if ((err == WSAEWOULDBLOCK) && (!m_non_blocking))
    {
      err = Connect_Timeout();

      if (err != GSOCK_NOERROR)
      {
        Close();
        /* m_error is set in _GSocket_Connect_Timeout */
      }

      return (GSocketError) err;
    }

    /* If connect failed with EWOULDBLOCK and the GSocket object
     * is set to nonblocking, we set m_error to GSOCK_WOULDBLOCK
     * (and return GSOCK_WOULDBLOCK) but we don't close the socket;
     * this way if the connection completes, a GSOCK_CONNECTION
     * event will be generated, if enabled.
     */
    if ((err == WSAEWOULDBLOCK) && (m_non_blocking))
    {
      m_establishing = true;
      m_error = GSOCK_WOULDBLOCK;
      return GSOCK_WOULDBLOCK;
    }

    /* If connect failed with an error other than EWOULDBLOCK,
     * then the call to GSocket_Connect() has failed.
     */
    Close();
    m_error = GSOCK_IOERR;
    return GSOCK_IOERR;
  }

  return GSOCK_NOERROR;
}

/* Datagram sockets */

/* GSocket_SetNonOriented:
 *  Sets up this socket as a non-connection oriented (datagram) socket.
 *  Before using this function, the local address must have been set
 *  with GSocket_SetLocal(), or the call will fail. Returns GSOCK_NOERROR
 *  on success, or one of the following otherwise.
 *
 *  Error codes:
 *    GSOCK_INVSOCK - the socket is in use.
 *    GSOCK_INVADDR - the local address has not been set.
 *    GSOCK_IOERR   - low-level error.
 */
GSocketError GSocket::SetNonOriented()
{
  u_long arg = 1;

  if (m_fd != INVALID_SOCKET)
  {
    m_error = GSOCK_INVSOCK;
    return GSOCK_INVSOCK;
  }

  if (!m_local)
  {
    m_error = GSOCK_INVADDR;
    return GSOCK_INVADDR;
  }

  /* Initialize all fields */
  m_stream   = false;
  m_server   = false;

  /* Create the socket */
  m_fd = socket(m_local->m_realfamily, SOCK_DGRAM, 0);

  if (m_fd == INVALID_SOCKET)
  {
    m_error = GSOCK_IOERR;
    return GSOCK_IOERR;
  }

  ioctlsocket(m_fd, FIONBIO, (u_long FAR *) &arg);
  GSocketManager::Get()->Enable_Events(this);

  if (m_reusable)
  {
    setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&arg, sizeof(arg));
  }
  if (m_broadcast)
  {
    setsockopt(m_fd, SOL_SOCKET, SO_BROADCAST, (const char*)&arg, sizeof(arg));
  }
  if (m_dobind)
  {
    /* Bind to the local address,
     * and retrieve the actual address bound.
     */
    if ((bind(m_fd, m_local->m_addr, m_local->m_len) != 0) ||
        (getsockname(m_fd,
                     m_local->m_addr,
                     (WX_SOCKLEN_T *)&m_local->m_len) != 0))
    {
      Close();
      m_error = GSOCK_IOERR;
      return GSOCK_IOERR;
    }
  }

  return GSOCK_NOERROR;
}

/* Generic IO */

/* Like recv(), send(), ... */
int GSocket::Read(char *buffer, int size)
{
  int ret;

  /* Reenable INPUT events */
  m_detected &= ~GSOCK_INPUT_FLAG;

  if (m_fd == INVALID_SOCKET || m_server)
  {
    m_error = GSOCK_INVSOCK;
    return -1;
  }

  /* If the socket is blocking, wait for data (with a timeout) */
  if (Input_Timeout() == GSOCK_TIMEDOUT)
  {
    m_error = GSOCK_TIMEDOUT;
    return -1;
  }

  /* Read the data */
  if (m_stream)
    ret = Recv_Stream(buffer, size);
  else
    ret = Recv_Dgram(buffer, size);

  if (ret == SOCKET_ERROR)
  {
    if (WSAGetLastError() != WSAEWOULDBLOCK)
      m_error = GSOCK_IOERR;
    else
      m_error = GSOCK_WOULDBLOCK;
    return -1;
  }

  return ret;
}

int GSocket::Write(const char *buffer, int size)
{
  int ret;

  if (m_fd == INVALID_SOCKET || m_server)
  {
    m_error = GSOCK_INVSOCK;
    return -1;
  }

  /* If the socket is blocking, wait for writability (with a timeout) */
  if (Output_Timeout() == GSOCK_TIMEDOUT)
    return -1;

  /* Write the data */
  if (m_stream)
    ret = Send_Stream(buffer, size);
  else
    ret = Send_Dgram(buffer, size);

  if (ret == SOCKET_ERROR)
  {
    if (WSAGetLastError() != WSAEWOULDBLOCK)
      m_error = GSOCK_IOERR;
    else
      m_error = GSOCK_WOULDBLOCK;

    /* Only reenable OUTPUT events after an error (just like WSAAsyncSelect
     * does). Once the first OUTPUT event is received, users can assume
     * that the socket is writable until a read operation fails. Only then
     * will further OUTPUT events be posted.
     */
    m_detected &= ~GSOCK_OUTPUT_FLAG;
    return -1;
  }

  return ret;
}

/* GSocket_Select:
 *  Polls the socket to determine its status. This function will
 *  check for the events specified in the 'flags' parameter, and
 *  it will return a mask indicating which operations can be
 *  performed. This function won't block, regardless of the
 *  mode (blocking | nonblocking) of the socket.
 */
GSocketEventFlags GSocket::Select(GSocketEventFlags flags)
{
  return flags & m_detected;
}

/* Attributes */

/* GSocket_SetNonBlocking:
 *  Sets the socket to non-blocking mode. All IO calls will return
 *  immediately.
 */
void GSocket::SetNonBlocking(bool non_block)
{
  m_non_blocking = non_block;
}

/* GSocket_SetTimeout:
 *  Sets the timeout for blocking calls. Time is expressed in
 *  milliseconds.
 */
void GSocket::SetTimeout(unsigned long millis)
{
  m_timeout.tv_sec  = (millis / 1000);
  m_timeout.tv_usec = (millis % 1000) * 1000;
}

/* GSocket_GetError:
 *  Returns the last error occurred for this socket. Note that successful
 *  operations do not clear this back to GSOCK_NOERROR, so use it only
 *  after an error.
 */
GSocketError WXDLLIMPEXP_NET GSocket::GetError()
{
  return m_error;
}

/* Callbacks */

/* GSOCK_INPUT:
 *   There is data to be read in the input buffer. If, after a read
 *   operation, there is still data available, the callback function will
 *   be called again.
 * GSOCK_OUTPUT:
 *   The socket is available for writing. That is, the next write call
 *   won't block. This event is generated only once, when the connection is
 *   first established, and then only if a call failed with GSOCK_WOULDBLOCK,
 *   when the output buffer empties again. This means that the app should
 *   assume that it can write since the first OUTPUT event, and no more
 *   OUTPUT events will be generated unless an error occurs.
 * GSOCK_CONNECTION:
 *   Connection successfully established, for client sockets, or incoming
 *   client connection, for server sockets. Wait for this event (also watch
 *   out for GSOCK_LOST) after you issue a nonblocking GSocket_Connect() call.
 * GSOCK_LOST:
 *   The connection is lost (or a connection request failed); this could
 *   be due to a failure, or due to the peer closing it gracefully.
 */

/* GSocket_SetCallback:
 *  Enables the callbacks specified by 'flags'. Note that 'flags'
 *  may be a combination of flags OR'ed toghether, so the same
 *  callback function can be made to accept different events.
 *  The callback function must have the following prototype:
 *
 *  void function(GSocket *socket, GSocketEvent event, char *cdata)
 */
void GSocket::SetCallback(GSocketEventFlags flags,
                         GSocketCallback callback, char *cdata)
{
  int count;

  for (count = 0; count < GSOCK_MAX_EVENT; count++)
  {
    if ((flags & (1 << count)) != 0)
    {
      m_cbacks[count] = callback;
      m_data[count] = cdata;
    }
  }
}

/* GSocket_UnsetCallback:
 *  Disables all callbacks specified by 'flags', which may be a
 *  combination of flags OR'ed toghether.
 */
void GSocket::UnsetCallback(GSocketEventFlags flags)
{
  int count;

  for (count = 0; count < GSOCK_MAX_EVENT; count++)
  {
    if ((flags & (1 << count)) != 0)
    {
      m_cbacks[count] = NULL;
      m_data[count] = NULL;
    }
  }
}

GSocketError GSocket::GetSockOpt(int level, int optname,
                                void *optval, int *optlen)
{
    if (getsockopt(m_fd, level, optname, (char*)optval, optlen) == 0)
    {
        return GSOCK_NOERROR;
    }
    return GSOCK_OPTERR;
}

GSocketError GSocket::SetSockOpt(int level, int optname,
                                const void *optval, int optlen)
{
    if (setsockopt(m_fd, level, optname, (char*)optval, optlen) == 0)
    {
        return GSOCK_NOERROR;
    }
    return GSOCK_OPTERR;
}

/* Internals (IO) */

/* _GSocket_Input_Timeout:
 *  For blocking sockets, wait until data is available or
 *  until timeout ellapses.
 */
GSocketError GSocket::Input_Timeout()
{
  fd_set readfds;

  if (!m_non_blocking)
  {
    FD_ZERO(&readfds);
    FD_SET(m_fd, &readfds);
    if (select(0, &readfds, NULL, NULL, &m_timeout) == 0)
    {
      m_error = GSOCK_TIMEDOUT;
      return GSOCK_TIMEDOUT;
    }
  }
  return GSOCK_NOERROR;
}

/* _GSocket_Output_Timeout:
 *  For blocking sockets, wait until data can be sent without
 *  blocking or until timeout ellapses.
 */
GSocketError GSocket::Output_Timeout()
{
  fd_set writefds;

  if (!m_non_blocking)
  {
    FD_ZERO(&writefds);
    FD_SET(m_fd, &writefds);
    if (select(0, NULL, &writefds, NULL, &m_timeout) == 0)
    {
      m_error = GSOCK_TIMEDOUT;
      return GSOCK_TIMEDOUT;
    }
  }
  return GSOCK_NOERROR;
}

/* _GSocket_Connect_Timeout:
 *  For blocking sockets, wait until the connection is
 *  established or fails, or until timeout ellapses.
 */
GSocketError GSocket::Connect_Timeout()
{
  fd_set writefds;
  fd_set exceptfds;

  FD_ZERO(&writefds);
  FD_ZERO(&exceptfds);
  FD_SET(m_fd, &writefds);
  FD_SET(m_fd, &exceptfds);
  if (select(0, NULL, &writefds, &exceptfds, &m_timeout) == 0)
  {
    m_error = GSOCK_TIMEDOUT;
    return GSOCK_TIMEDOUT;
  }
  if (!FD_ISSET(m_fd, &writefds))
  {
    m_error = GSOCK_IOERR;
    return GSOCK_IOERR;
  }

  return GSOCK_NOERROR;
}

int GSocket::Recv_Stream(char *buffer, int size)
{
  return recv(m_fd, buffer, size, 0);
}

int GSocket::Recv_Dgram(char *buffer, int size)
{
  wxSockAddr from;
  WX_SOCKLEN_T fromlen = sizeof(from);
  int ret;
  GSocketError err;

  ret = recvfrom(m_fd, buffer, size, 0, (sockaddr*)&from, &fromlen);

  if (ret == SOCKET_ERROR)
    return SOCKET_ERROR;

  /* Translate a system address into a GSocket address */
  if (!m_peer)
  {
    m_peer = GAddress_new();
    if (!m_peer)
    {
      m_error = GSOCK_MEMERR;
      return -1;
    }
  }
  err = _GAddress_translate_from(m_peer, (sockaddr*)&from, fromlen);
  if (err != GSOCK_NOERROR)
  {
    GAddress_destroy(m_peer);
    m_peer  = NULL;
    m_error = err;
    return -1;
  }

  return ret;
}

int GSocket::Send_Stream(const char *buffer, int size)
{
  return send(m_fd, buffer, size, 0);
}

int GSocket::Send_Dgram(const char *buffer, int size)
{
  struct sockaddr *addr;
  int len, ret;
  GSocketError err;

  if (!m_peer)
  {
    m_error = GSOCK_INVADDR;
    return -1;
  }

  err = _GAddress_translate_to(m_peer, &addr, &len);
  if (err != GSOCK_NOERROR)
  {
    m_error = err;
    return -1;
  }

  ret = sendto(m_fd, buffer, size, 0, addr, len);

  /* Frees memory allocated by _GAddress_translate_to */
  free(addr);

  return ret;
}

/* Compatibility functions for GSocket */
GSocket *GSocket_new()
{
    GSocket *newsocket = new GSocket();
    if(newsocket->IsOk())
        return newsocket;
    delete newsocket;
    return NULL;
}


/*
 * -------------------------------------------------------------------------
 * GAddress
 * -------------------------------------------------------------------------
 */

/* CHECK_ADDRESS verifies that the current address family is either
 * GSOCK_NOFAMILY or GSOCK_*family*, and if it is GSOCK_NOFAMILY, it
 * initalizes it to be a GSOCK_*family*. In other cases, it returns
 * an appropiate error code.
 *
 * CHECK_ADDRESS_RETVAL does the same but returning 'retval' on error.
 */
#define CHECK_ADDRESS(address, family)                              \
{                                                                   \
  if (address->m_family == GSOCK_NOFAMILY)                          \
    if (_GAddress_Init_##family(address) != GSOCK_NOERROR)          \
      return address->m_error;                                      \
  if (address->m_family != GSOCK_##family)                          \
  {                                                                 \
    address->m_error = GSOCK_INVADDR;                               \
    return GSOCK_INVADDR;                                           \
  }                                                                 \
}

#define CHECK_ADDRESS_RETVAL(address, family, retval)               \
{                                                                   \
  if (address->m_family == GSOCK_NOFAMILY)                          \
    if (_GAddress_Init_##family(address) != GSOCK_NOERROR)          \
      return retval;                                                \
  if (address->m_family != GSOCK_##family)                          \
  {                                                                 \
    address->m_error = GSOCK_INVADDR;                               \
    return retval;                                                  \
  }                                                                 \
}


GAddress *GAddress_new()
{
  GAddress *address;

  if ((address = (GAddress *) malloc(sizeof(GAddress))) == NULL)
    return NULL;

  address->m_family = GSOCK_NOFAMILY;
  address->m_addr   = NULL;
  address->m_len    = 0;

  return address;
}

GAddress *GAddress_copy(GAddress *address)
{
  GAddress *addr2;

  if ((addr2 = (GAddress *) malloc(sizeof(GAddress))) == NULL)
    return NULL;

  memcpy(addr2, address, sizeof(GAddress));

  if (address->m_addr)
  {
    addr2->m_addr = (struct sockaddr *) malloc(addr2->m_len);
    if (addr2->m_addr == NULL)
    {
      free(addr2);
      return NULL;
    }
    memcpy(addr2->m_addr, address->m_addr, addr2->m_len);
  }

  return addr2;
}

void GAddress_destroy(GAddress *address)
{
  if (address->m_addr)
    free(address->m_addr);

  free(address);
}

void GAddress_SetFamily(GAddress *address, GAddressType type)
{
  address->m_family = type;
}

GAddressType GAddress_GetFamily(GAddress *address)
{
  return address->m_family;
}

GSocketError _GAddress_translate_from(GAddress *address,
                                      struct sockaddr *addr, int len)
{
  address->m_realfamily = addr->sa_family;
  switch (addr->sa_family)
  {
    case AF_INET:
      address->m_family = GSOCK_INET;
      break;
    case AF_UNIX:
      address->m_family = GSOCK_UNIX;
      break;
#if wxUSE_IPV6
    case AF_INET6:
      address->m_family = GSOCK_INET6;
      break;
#endif
    default:
    {
      address->m_error = GSOCK_INVOP;
      return GSOCK_INVOP;
    }
  }

  if (address->m_addr)
    free(address->m_addr);

  address->m_len = len;
  address->m_addr = (struct sockaddr *) malloc(len);

  if (address->m_addr == NULL)
  {
    address->m_error = GSOCK_MEMERR;
    return GSOCK_MEMERR;
  }
  memcpy(address->m_addr, addr, len);

  return GSOCK_NOERROR;
}

GSocketError _GAddress_translate_to(GAddress *address,
                                    struct sockaddr **addr, int *len)
{
  if (!address->m_addr)
  {
    address->m_error = GSOCK_INVADDR;
    return GSOCK_INVADDR;
  }

  *len = address->m_len;
  *addr = (struct sockaddr *) malloc(address->m_len);
  if (*addr == NULL)
  {
    address->m_error = GSOCK_MEMERR;
    return GSOCK_MEMERR;
  }

  memcpy(*addr, address->m_addr, address->m_len);
  return GSOCK_NOERROR;
}

/*
 * -------------------------------------------------------------------------
 * Internet address family
 * -------------------------------------------------------------------------
 */

GSocketError _GAddress_Init_INET(GAddress *address)
{
  address->m_len  = sizeof(struct sockaddr_in);
  address->m_addr = (struct sockaddr *) malloc(address->m_len);
  if (address->m_addr == NULL)
  {
    address->m_error = GSOCK_MEMERR;
    return GSOCK_MEMERR;
  }

  address->m_family = GSOCK_INET;
  address->m_realfamily = AF_INET;
  ((struct sockaddr_in *)address->m_addr)->sin_family = AF_INET;
  ((struct sockaddr_in *)address->m_addr)->sin_addr.s_addr = INADDR_ANY;

  return GSOCK_NOERROR;
}

GSocketError GAddress_INET_SetHostName(GAddress *address, const char *hostname)
{
  struct hostent *he;
  struct in_addr *addr;

  CHECK_ADDRESS(address, INET);

  addr = &(((struct sockaddr_in *)address->m_addr)->sin_addr);

  addr->s_addr = inet_addr(hostname);

  /* If it is a numeric host name, convert it now */
  if (addr->s_addr == INADDR_NONE)
  {
    struct in_addr *array_addr;

    /* It is a real name, we solve it */
    if ((he = gethostbyname(hostname)) == NULL)
    {
      /* addr->s_addr = INADDR_NONE just done by inet_addr() above */
      address->m_error = GSOCK_NOHOST;
      return GSOCK_NOHOST;
    }
    array_addr = (struct in_addr *) *(he->h_addr_list);
    addr->s_addr = array_addr[0].s_addr;
  }
  return GSOCK_NOERROR;
}

GSocketError GAddress_INET_SetBroadcastAddress(GAddress *address)
{
  return GAddress_INET_SetHostAddress(address, INADDR_BROADCAST);
}

GSocketError GAddress_INET_SetAnyAddress(GAddress *address)
{
  return GAddress_INET_SetHostAddress(address, INADDR_ANY);
}

GSocketError GAddress_INET_SetHostAddress(GAddress *address,
                                          unsigned long hostaddr)
{
  struct in_addr *addr;

  CHECK_ADDRESS(address, INET);

  addr = &(((struct sockaddr_in *)address->m_addr)->sin_addr);
  addr->s_addr = htonl(hostaddr);

  return GSOCK_NOERROR;
}

GSocketError GAddress_INET_SetPortName(GAddress *address, const char *port,
                                       const char *protocol)
{
  struct servent *se;
  struct sockaddr_in *addr;

  CHECK_ADDRESS(address, INET);

  if (!port)
  {
    address->m_error = GSOCK_INVPORT;
    return GSOCK_INVPORT;
  }

  se = getservbyname(port, protocol);
  if (!se)
  {
    if (isdigit(port[0]))
    {
      int port_int;

      port_int = atoi(port);
      addr = (struct sockaddr_in *)address->m_addr;
      addr->sin_port = htons((u_short) port_int);
      return GSOCK_NOERROR;
    }

    address->m_error = GSOCK_INVPORT;
    return GSOCK_INVPORT;
  }

  addr = (struct sockaddr_in *)address->m_addr;
  addr->sin_port = se->s_port;

  return GSOCK_NOERROR;
}

GSocketError GAddress_INET_SetPort(GAddress *address, unsigned short port)
{
  struct sockaddr_in *addr;

  CHECK_ADDRESS(address, INET);

  addr = (struct sockaddr_in *)address->m_addr;
  addr->sin_port = htons(port);

  return GSOCK_NOERROR;
}

GSocketError GAddress_INET_GetHostName(GAddress *address, char *hostname, size_t sbuf)
{
  struct hostent *he;
  char *addr_buf;
  struct sockaddr_in *addr;

  CHECK_ADDRESS(address, INET);

  addr = (struct sockaddr_in *)address->m_addr;
  addr_buf = (char *)&(addr->sin_addr);

  he = gethostbyaddr(addr_buf, sizeof(addr->sin_addr), AF_INET);
  if (he == NULL)
  {
    address->m_error = GSOCK_NOHOST;
    return GSOCK_NOHOST;
  }

  strncpy(hostname, he->h_name, sbuf);

  return GSOCK_NOERROR;
}

unsigned long GAddress_INET_GetHostAddress(GAddress *address)
{
  struct sockaddr_in *addr;

  CHECK_ADDRESS_RETVAL(address, INET, 0);

  addr = (struct sockaddr_in *)address->m_addr;

  return ntohl(addr->sin_addr.s_addr);
}

unsigned short GAddress_INET_GetPort(GAddress *address)
{
  struct sockaddr_in *addr;

  CHECK_ADDRESS_RETVAL(address, INET, 0);

  addr = (struct sockaddr_in *)address->m_addr;
  return ntohs(addr->sin_port);
}


#if wxUSE_IPV6
/*
 * -------------------------------------------------------------------------
 * Internet IPv6 address family
 * -------------------------------------------------------------------------
 */
#include "ws2tcpip.h"

#ifdef __VISUALC__
    #pragma comment(lib,"ws2_32")
#endif // __VISUALC__

GSocketError _GAddress_Init_INET6(GAddress *address)
{
  struct in6_addr any_address = IN6ADDR_ANY_INIT;
  address->m_len  = sizeof(struct sockaddr_in6);
  address->m_addr = (struct sockaddr *) malloc(address->m_len);
  if (address->m_addr == NULL)
  {
    address->m_error = GSOCK_MEMERR;
    return GSOCK_MEMERR;
  }
  memset(address->m_addr,0,address->m_len);

  address->m_family = GSOCK_INET6;
  address->m_realfamily = AF_INET6;
  ((struct sockaddr_in6 *)address->m_addr)->sin6_family = AF_INET6;
  ((struct sockaddr_in6 *)address->m_addr)->sin6_addr = any_address;

  return GSOCK_NOERROR;
}

GSocketError GAddress_INET6_SetHostName(GAddress *address, const char *hostname)
{
  CHECK_ADDRESS(address, INET6);

  addrinfo hints;
  memset( & hints, 0, sizeof( hints ) );
  hints.ai_family = AF_INET6;
  addrinfo * info = 0;
  if ( getaddrinfo( hostname, "0", & hints, & info ) || ! info )
  {
    address->m_error = GSOCK_NOHOST;
    return GSOCK_NOHOST;
  }

  memcpy( address->m_addr, info->ai_addr, info->ai_addrlen );
  freeaddrinfo( info );
  return GSOCK_NOERROR;
}

GSocketError GAddress_INET6_SetAnyAddress(GAddress *address)
{
  CHECK_ADDRESS(address, INET6);

  struct in6_addr addr;
  memset( & addr, 0, sizeof( addr ) );
  return GAddress_INET6_SetHostAddress(address, addr);
}
GSocketError GAddress_INET6_SetHostAddress(GAddress *address,
                                          struct in6_addr hostaddr)
{
  CHECK_ADDRESS(address, INET6);

  ((struct sockaddr_in6 *)address->m_addr)->sin6_addr = hostaddr;

  return GSOCK_NOERROR;
}

GSocketError GAddress_INET6_SetPortName(GAddress *address, const char *port,
                                       const char *protocol)
{
  struct servent *se;
  struct sockaddr_in6 *addr;

  CHECK_ADDRESS(address, INET6);

  if (!port)
  {
    address->m_error = GSOCK_INVPORT;
    return GSOCK_INVPORT;
  }

  se = getservbyname(port, protocol);
  if (!se)
  {
    if (isdigit(port[0]))
    {
      int port_int;

      port_int = atoi(port);
      addr = (struct sockaddr_in6 *)address->m_addr;
      addr->sin6_port = htons((u_short) port_int);
      return GSOCK_NOERROR;
    }

    address->m_error = GSOCK_INVPORT;
    return GSOCK_INVPORT;
  }

  addr = (struct sockaddr_in6 *)address->m_addr;
  addr->sin6_port = se->s_port;

  return GSOCK_NOERROR;
}

GSocketError GAddress_INET6_SetPort(GAddress *address, unsigned short port)
{
  struct sockaddr_in6 *addr;

  CHECK_ADDRESS(address, INET6);

  addr = (struct sockaddr_in6 *)address->m_addr;
  addr->sin6_port = htons(port);

  return GSOCK_NOERROR;
}

GSocketError GAddress_INET6_GetHostName(GAddress *address, char *hostname, size_t sbuf)
{
  struct hostent *he;
  char *addr_buf;
  struct sockaddr_in6 *addr;

  CHECK_ADDRESS(address, INET6);

  addr = (struct sockaddr_in6 *)address->m_addr;
  addr_buf = (char *)&(addr->sin6_addr);

  he = gethostbyaddr(addr_buf, sizeof(addr->sin6_addr), AF_INET6);
  if (he == NULL)
  {
    address->m_error = GSOCK_NOHOST;
    return GSOCK_NOHOST;
  }

  strncpy(hostname, he->h_name, sbuf);

  return GSOCK_NOERROR;
}

GSocketError GAddress_INET6_GetHostAddress(GAddress *address,struct in6_addr *hostaddr)
{
  CHECK_ADDRESS_RETVAL(address, INET6, GSOCK_INVADDR);
  *hostaddr = ( (struct sockaddr_in6 *)address->m_addr )->sin6_addr;
  return GSOCK_NOERROR;
}

unsigned short GAddress_INET6_GetPort(GAddress *address)
{
  CHECK_ADDRESS_RETVAL(address, INET6, 0);

  return ntohs( ((struct sockaddr_in6 *)address->m_addr)->sin6_port );
}

#endif // wxUSE_IPV6

/*
 * -------------------------------------------------------------------------
 * Unix address family
 * -------------------------------------------------------------------------
 */

GSocketError _GAddress_Init_UNIX(GAddress *address)
{
  address->m_error = GSOCK_INVADDR;
  return GSOCK_INVADDR;
}

GSocketError GAddress_UNIX_SetPath(GAddress *address, const char *WXUNUSED(path))
{
  address->m_error = GSOCK_INVADDR;
  return GSOCK_INVADDR;
}

GSocketError GAddress_UNIX_GetPath(GAddress *address, char *WXUNUSED(path), size_t WXUNUSED(sbuf))
{
  address->m_error = GSOCK_INVADDR;
  return GSOCK_INVADDR;
}

#endif  // wxUSE_SOCKETS
