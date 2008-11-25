/* -------------------------------------------------------------------------
 * Project:     GSocket (Generic Socket) for WX
 * Name:        gsockunx.h
 * Copyright:   (c) Guilhem Lavaux
 * Licence:     wxWindows Licence
 * Purpose:     GSocket Unix header
 * CVSID:       $Id$
 * -------------------------------------------------------------------------
 */

#ifndef _WX_UNIX_GSOCKUNX_H_
#define _WX_UNIX_GSOCKUNX_H_

#include <unistd.h>

class wxGSocketIOHandler;

class GSocket : public GSocketBase
{
public:
    GSocket(wxSocketBase& wxsocket);
    virtual ~GSocket();

    virtual void Shutdown();
    virtual GSocket *WaitConnection(wxSocketBase& wxsocket);

    GSocketError SetServer();
    bool SetReusable();
    bool SetBroadcast();
    bool DontDoBind();
    GSocketError Connect(GSocketStream stream);
    GSocketError SetNonOriented();
    int Read(char *buffer, int size);
    int Write(const char *buffer, int size);
    void SetNonBlocking(bool non_block);
    GSocketError WXDLLIMPEXP_NET GetError();
    GSocketError GetSockOpt(int level, int optname, void *optval, int *optlen);
    GSocketError SetSockOpt(int level, int optname,
        const void *optval, int optlen);
    //attach or detach from main loop
    void Notify(bool flag);
    void Detected_Read();
    void Detected_Write();

private:
    // enable or disable notifications for socket input/output events but only
    // if m_use_events is true; do nothing otherwise
    void EnableEvents()
    {
        if ( m_use_events )
            DoEnableEvents(true);
    }

    void DisableEvents()
    {
        if ( m_use_events )
            DoEnableEvents(false);
    }

    // really enable or disable socket input/output events, regardless of
    // m_use_events value
    void DoEnableEvents(bool enable);


    // enable or disable events for the given event if m_use_events; do nothing
    // otherwise
    //
    // notice that these functions also update m_detected: EnableEvent() clears
    // the corresponding bit in it and DisableEvent() sets it
    void EnableEvent(GSocketEvent event);
    void DisableEvent(GSocketEvent event);


    GSocketError Input_Timeout();
    GSocketError Output_Timeout();
    int Recv_Stream(char *buffer, int size);
    int Recv_Dgram(char *buffer, int size);
    int Send_Stream(const char *buffer, int size);
    int Send_Dgram(const char *buffer, int size);
public:
    /* DFE: We can't protect these data member until the GUI code is updated */
    /* protected: */
  wxGSocketIOHandler *m_handler;

  // true if socket should fire events
  bool m_use_events;

  // pointer for storing extra (usually GUI-specific) data
  void *m_gui_dependent;

private:
    // notify the associated wxSocket about a change in socket state and shut
    // down the socket if the event is GSOCK_LOST
    void OnStateChange(GSocketEvent event);
};

// A version of GSocketManager which uses FDs for socket IO
//
// This class uses GSocket::m_gui_dependent field to store the 2 (for input and
// output) FDs associated with the socket.
class GSocketFDBasedManager : public GSocketManager
{
public:
    // no special initialization/cleanup needed when using FDs
    virtual bool OnInit() { return true; }
    virtual void OnExit() { }

    // allocate/free the storage we need
    virtual bool Init_Socket(GSocket *socket)
    {
        socket->m_gui_dependent = malloc(sizeof(int)*2);
        int * const fds = static_cast<int *>(socket->m_gui_dependent);

        fds[0] = -1;
        fds[1] = -1;

        return true;
    }

    virtual void Close_Socket(GSocket *socket)
    {
        Uninstall_Callback(socket, GSOCK_INPUT);
        Uninstall_Callback(socket, GSOCK_OUTPUT);

        close(socket->m_fd);
    }

    virtual void Destroy_Socket(GSocket *socket)
    {
        free(socket->m_gui_dependent);
    }

protected:
    // identifies either input or output direction
    //
    // NB: the values of this enum shouldn't change
    enum SocketDir
    {
        FD_INPUT,
        FD_OUTPUT
    };

    // get the FD index corresponding to the given GSocketEvent
    SocketDir GetDirForEvent(GSocket *socket, GSocketEvent event)
    {
        switch ( event )
        {
            default:
                wxFAIL_MSG( "unexpected socket event" );
                // fall through

            case GSOCK_LOST:
                // fall through

            case GSOCK_INPUT:
                return FD_INPUT;

            case GSOCK_OUTPUT:
                return FD_OUTPUT;

            case GSOCK_CONNECTION:
                // FIXME: explain this?
                return socket->m_server ? FD_INPUT : FD_OUTPUT;
        }
    }

    // access the FDs we store
    int& FD(GSocket *socket, SocketDir d)
    {
        return static_cast<int *>(socket->m_gui_dependent)[d];
    }
};

// Common base class for all ports using X11-like (and hence implemented in
// X11, Motif and GTK) AddInput() and RemoveInput() functions
class GSocketInputBasedManager : public GSocketFDBasedManager
{
public:
    virtual void Install_Callback(GSocket *socket, GSocketEvent event)
    {
        wxCHECK_RET( socket->m_fd != -1,
                        "shouldn't be called on invalid socket" );

        const SocketDir d = GetDirForEvent(socket, event);

        int& fd = FD(socket, d);
        if ( fd != -1 )
            RemoveInput(fd);

        fd = AddInput(socket, d);
    }

    virtual void Uninstall_Callback(GSocket *socket, GSocketEvent event)
    {
        const SocketDir d = GetDirForEvent(socket, event);

        int& fd = FD(socket, d);
        if ( fd != -1 )
        {
            RemoveInput(fd);
            fd = -1;
        }
    }

private:
    // these functions map directly to XtAdd/RemoveInput() or
    // gdk_input_add/remove()
    virtual int AddInput(GSocket *socket, SocketDir d) = 0;
    virtual void RemoveInput(int fd) = 0;
};

#endif  /* _WX_UNIX_GSOCKUNX_H_ */
