/* -------------------------------------------------------------------------
 * Project: GSocket (Generic Socket) for WX
 * Name:    gsockunx.h
 * Purpose: GSocket Unix header
 * CVSID:   $Id$
 * -------------------------------------------------------------------------
 */

#ifndef __GSOCK_UNX_H
#define __GSOCK_UNX_H

#ifndef __GSOCKET_STANDALONE__
#include "wx/setup.h"
#endif

#if wxUSE_SOCKETS || defined(__GSOCKET_STANDALONE__)

#ifndef __GSOCKET_STANDALONE__
#include "wx/gsocket.h"
#else
#include "gsocket.h"
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

class GSocketGUIFunctionsTableConcrete: public GSocketGUIFunctionsTable
{
public:
    virtual bool OnInit();
    virtual void OnExit();
    virtual bool CanUseEventLoop();
    virtual bool Init_Socket(GSocket *socket);
    virtual void Destroy_Socket(GSocket *socket);
    virtual void Install_Callback(GSocket *socket, GSocketEvent event);
    virtual void Uninstall_Callback(GSocket *socket, GSocketEvent event);
    virtual void Enable_Events(GSocket *socket);
    virtual void Disable_Events(GSocket *socket);
};

class GSocket
{
public:
    GSocket();
    virtual ~GSocket();
    bool IsOk() { return m_ok; }
    void Close();
    void Shutdown();
    GSocketError SetLocal(GAddress *address);
    GSocketError SetPeer(GAddress *address);
    GAddress *GetLocal();
    GAddress *GetPeer();
    GSocketError SetServer();
    GSocket *WaitConnection();
    int SetReusable();
    GSocketError Connect(GSocketStream stream);
    GSocketError SetNonOriented();
    int Read(char *buffer, int size);
    int Write(const char *buffer, int size);
    GSocketEventFlags Select(GSocketEventFlags flags);
    void SetNonBlocking(int non_block);
    void SetTimeout(unsigned long millisec);
    GSocketError WXDLLIMPEXP_NET GetError();
    void SetCallback(GSocketEventFlags flags,
        GSocketCallback callback, char *cdata);
    void UnsetCallback(GSocketEventFlags flags);
    GSocketError GetSockOpt(int level, int optname, void *optval, int *optlen);
    GSocketError SetSockOpt(int level, int optname,
        const void *optval, int optlen);
    /* API compatibility functions */
    static void _GSocket_Detected_Read(GSocket *socket);
    static void _GSocket_Detected_Write(GSocket *socket);
    virtual void Detected_Read();
    virtual void Detected_Write();
protected:
    void Enable(GSocketEvent event);
    void Disable(GSocketEvent event);
    GSocketError Input_Timeout();
    GSocketError Output_Timeout();
    int Recv_Stream(char *buffer, int size);
    int Recv_Dgram(char *buffer, int size);
    int Send_Stream(const char *buffer, int size);
    int Send_Dgram(const char *buffer, int size);
    bool m_ok;
public:
    /* DFE: We can't protect these data member until the GUI code is updated */
    /* protected: */
  int m_fd;
  GAddress *m_local;
  GAddress *m_peer;
  GSocketError m_error;

  int m_non_blocking;
  int m_server;
  int m_stream;
  int m_establishing;
  int m_reusable;
  unsigned long m_timeout;

  /* Callbacks */
  GSocketEventFlags m_detected;
  GSocketCallback m_cbacks[GSOCK_MAX_EVENT];
  char *m_data[GSOCK_MAX_EVENT];

  char *m_gui_dependent;

};

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/* Definition of GAddress */
struct _GAddress
{
  struct sockaddr *m_addr;
  size_t m_len;

  GAddressType m_family;
  int m_realfamily;

  GSocketError m_error;
};
#ifdef __cplusplus
}
#endif  /* __cplusplus */

/* Compatibility methods to support old C API (from gsocket.h) */
inline void GSocket_Shutdown(GSocket *socket)
{   socket->Shutdown(); }
inline GSocketError GSocket_SetLocal(GSocket *socket, GAddress *address)
{   return socket->SetLocal(address); }
inline GSocketError GSocket_SetPeer(GSocket *socket, GAddress *address)
{   return socket->SetPeer(address); }
inline GAddress *GSocket_GetLocal(GSocket *socket)
{   return socket->GetLocal(); }
inline GAddress *GSocket_GetPeer(GSocket *socket)
{   return socket->GetPeer(); }
inline GSocketError GSocket_SetServer(GSocket *socket)
{   return socket->SetServer(); }
inline GSocket *GSocket_WaitConnection(GSocket *socket)
{   return socket->WaitConnection(); }
inline int GSocket_SetReusable(GSocket *socket)
{   return socket->SetReusable(); }
inline GSocketError GSocket_Connect(GSocket *socket, GSocketStream stream)
{   return socket->Connect(stream); }
inline GSocketError GSocket_SetNonOriented(GSocket *socket)
{   return socket->SetNonOriented(); }
inline int GSocket_Read(GSocket *socket, char *buffer, int size)
{   return socket->Read(buffer,size); }
inline int GSocket_Write(GSocket *socket, const char *buffer, int size)
{   return socket->Write(buffer,size); }
inline GSocketEventFlags GSocket_Select(GSocket *socket, GSocketEventFlags flags)
{   return socket->Select(flags); }
inline void GSocket_SetNonBlocking(GSocket *socket, int non_block)
{   socket->SetNonBlocking(non_block); }
inline void GSocket_SetTimeout(GSocket *socket, unsigned long millisec)
{   socket->SetTimeout(millisec); }
inline GSocketError GSocket_GetError(GSocket *socket)
{   return socket->GetError(); }
inline void GSocket_SetCallback(GSocket *socket, GSocketEventFlags flags,
                         GSocketCallback fallback, char *cdata)
{   socket->SetCallback(flags,fallback,cdata); }
inline void GSocket_UnsetCallback(GSocket *socket, GSocketEventFlags flags)
{   socket->UnsetCallback(flags); }
inline GSocketError GSocket_GetSockOpt(GSocket *socket, int level, int optname,
                        void *optval, int *optlen)
{   return socket->GetSockOpt(level,optname,optval,optlen); }
inline GSocketError GSocket_SetSockOpt(GSocket *socket, int level, int optname,
                        const void *optval, int optlen)
{   return socket->SetSockOpt(level,optname,optval,optlen); }
inline void GSocket_destroy(GSocket *socket)
{   delete socket; }


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/* GAddress */

GSocketError _GAddress_translate_from(GAddress *address,
                                      struct sockaddr *addr, int len);
GSocketError _GAddress_translate_to  (GAddress *address,
                                      struct sockaddr **addr, int *len);
GSocketError _GAddress_Init_INET(GAddress *address);
GSocketError _GAddress_Init_UNIX(GAddress *address);


#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* wxUSE_SOCKETS || defined(__GSOCKET_STANDALONE__) */

#endif  /* __GSOCK_UNX_H */
