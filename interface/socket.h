/////////////////////////////////////////////////////////////////////////////
// Name:        socket.h
// Purpose:     documentation for wxIPV4address class
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxIPV4address
    @wxheader{socket.h}


    @library{wxbase}
    @category{net}
*/
class wxIPV4address : public wxIPaddress
{
public:
    /**
        Set address to any of the addresses of the current machine. Whenever
        possible, use this function instead of LocalHost(),
        as this correctly handles multi-homed hosts and avoids other small
        problems. Internally, this is the same as setting the IP address
        to @b INADDR_ANY.
        
        @returns Returns @true on success, @false if something went wrong.
    */
    bool AnyAddress();

    //@{
    /**
        Returns the hostname which matches the IP address.
    */
    bool Hostname(const wxString& hostname);
    Return value wxString  Hostname();
    //@}

    /**
        Returns a wxString containing the IP address in dot quad (127.0.0.1) format.
    */
    wxString IPAddress();

    /**
        Set address to localhost (127.0.0.1). Whenever possible, use the
        AnyAddress(),
        function instead of this one, as this will correctly handle multi-homed
        hosts and avoid other small problems.
    */
    bool LocalHost();

    //@{
    /**
        Returns the current service.
    */
    bool Service(const wxString& service);
    Return value bool Service(unsigned short service);
    Return value unsigned short Service();
    //@}
};


/**
    @class wxSocketServer
    @wxheader{socket.h}


    @library{wxnet}
    @category{net}

    @seealso
    wxSocketServer::WaitForAccept, wxSocketBase::SetNotify, wxSocketBase::Notify,
    wxSocketServer::AcceptWith
*/
class wxSocketServer : public wxSocketBase
{
public:
    /**
        Constructs a new server and tries to bind to the specified @e address.
        Before trying to accept new connections, test whether it succeeded with
        @ref wxSocketBase::isok wxSocketBase:IsOk.
        
        @param address
            Specifies the local address for the server (e.g. port number).
        @param flags
            Socket flags (See wxSocketBase::SetFlags)
    */
    wxSocketServer(const wxSockAddress& address,
                   wxSocketFlags flags = wxSOCKET_NONE);

    /**
        Destructor (it doesn't close the accepted connections).
    */
    ~wxSocketServer();

    /**
        Accepts an incoming connection request, and creates a new
        wxSocketBase object which represents
        the server-side of the connection.
        If @a wait is @true and there are no pending connections to be
        accepted, it will wait for the next incoming connection to
        arrive. @b Warning: This will block the GUI.
        If @a wait is @false, it will try to accept a pending connection
        if there is one, but it will always return immediately without blocking
        the GUI. If you want to use Accept in this way, you can either check for
        incoming connections with WaitForAccept()
        or catch @b wxSOCKET_CONNECTION events, then call Accept once you know
        that there is an incoming connection waiting to be accepted.
        
        @returns Returns an opened socket connection, or @NULL if an error
                 occurred or if the wait parameter was @false and there
                 were no pending connections.
        
        @see WaitForAccept(), wxSocketBase::SetNotify,
             wxSocketBase::Notify, AcceptWith()
    */
    wxSocketBase* Accept(bool wait = true);

    /**
        Accept an incoming connection using the specified socket object.
        
        @param socket
            Socket to be initialized
        
        @returns Returns @true on success, or @false if an error occurred or if the
                 wait parameter was @false and there were no pending
                 connections.
    */
    bool AcceptWith(wxSocketBase& socket, bool wait = true);

    /**
        This function waits for an incoming connection. Use it if you want to call
        Accept() or AcceptWith()
        with @e wait set to @false, to detect when an incoming connection is waiting
        to be accepted.
        
        @param seconds
            Number of seconds to wait.
            If -1, it will wait for the default timeout,
            as set with SetTimeout.
        @param millisecond
            Number of milliseconds to wait.
        
        @returns Returns @true if an incoming connection arrived, @false if the
                 timeout elapsed.
    */
    bool WaitForAccept(long seconds = -1, long millisecond = 0);
};


/**
    @class wxIPaddress
    @wxheader{socket.h}

    wxIPaddress is an abstract base class for all internet protocol address
    objects. Currently, only wxIPV4address
    is implemented. An experimental implementation for IPV6, wxIPV6address,
    is being developed.

    @library{wxbase}
    @category{net}
*/
class wxIPaddress : public wxSockAddress
{
public:
    /**
        Internally, this is the same as setting the IP address
        to @b INADDR_ANY.
        On IPV4 implementations, 0.0.0.0
        On IPV6 implementations, ::
        
        @returns Returns @true on success, @false if something went wrong.
    */
    virtual bool AnyAddress();

    /**
        Internally, this is the same as setting the IP address
        to @b INADDR_BROADCAST.
        On IPV4 implementations, 255.255.255.255
        
        @returns Returns @true on success, @false if something went wrong.
    */
    virtual bool BroadcastAddress();

    //@{
    /**
        Returns the hostname which matches the IP address.
    */
    virtual bool Hostname(const wxString& hostname);
    Return value virtual wxString  Hostname();
    //@}

    /**
        Returns a wxString containing the IP address.
    */
    virtual wxString IPAddress();

    /**
        Determines if current address is set to localhost.
    */
    virtual bool IsLocalHost();

    /**
        Set address to localhost.
        On IPV4 implementations, 127.0.0.1
        On IPV6 implementations, ::1
        
        @returns Returns @true on success, @false if something went wrong.
    */
    virtual bool LocalHost();

    //@{
    /**
        Returns the current service.
    */
    virtual bool Service(const wxString& service);
    Return value virtual bool Service(unsigned short service);
    Return value virtual unsigned short Service();
    //@}
};


/**
    @class wxSocketClient
    @wxheader{socket.h}


    @library{wxnet}
    @category{net}

    @seealso
    wxSocketClient::WaitOnConnect, wxSocketBase::SetNotify, wxSocketBase::Notify
*/
class wxSocketClient : public wxSocketBase
{
public:
    /**
        Constructor.
        
        @param flags
            Socket flags (See wxSocketBase::SetFlags)
    */
    wxSocketClient(wxSocketFlags flags = wxSOCKET_NONE);

    /**
        Destructor. Please see wxSocketBase::Destroy.
    */
    ~wxSocketClient();

    //@{
    /**
        Connects to a server using the specified address.
        If @a wait is @true, Connect will wait until the connection
        completes. @b Warning: This will block the GUI.
        If @a wait is @false, Connect will try to establish the connection and
        return immediately, without blocking the GUI. When used this way, even if
        Connect returns @false, the connection request can be completed later.
        To detect this, use WaitOnConnect(),
        or catch @b wxSOCKET_CONNECTION events (for successful establishment)
        and @b wxSOCKET_LOST events (for connection failure).
        
        @param address
            Address of the server.
        @param local
            Bind to the specified local address and port before connecting.
            The local address and port can also be set using SetLocal,
            and then using the 2-parameter Connect method.
        @param wait
            If @true, waits for the connection to complete.
        
        @returns Returns @true if the connection is established and no error
                 occurs.
        
        @see WaitOnConnect(), wxSocketBase::SetNotify,
             wxSocketBase::Notify
    */
    bool Connect(wxSockAddress& address, bool wait = true);
    bool Connect(wxSockAddress& address, wxSockAddress& local,
                 bool wait = true);
    //@}

    /**
        Wait until a connection request completes, or until the specified timeout
        elapses. Use this function after issuing a call
        to Connect() with @e wait set to @false.
        
        @param seconds
            Number of seconds to wait.
            If -1, it will wait for the default timeout,
            as set with SetTimeout.
        @param millisecond
            Number of milliseconds to wait.
        
        @returns WaitOnConnect returns @true if the connection request completes.
                 This does not necessarily mean that the connection was
                 successfully established; it might also happen that the
                 connection was refused by the peer. Use  IsConnected to
                 distinguish between these two situations.
    */
    bool WaitOnConnect(long seconds = -1, long milliseconds = 0);
};


/**
    @class wxSockAddress
    @wxheader{socket.h}

    You are unlikely to need to use this class: only wxSocketBase uses it.

    @library{wxbase}
    @category{FIXME}

    @seealso
    wxSocketBase, wxIPaddress, wxIPV4address
*/
class wxSockAddress : public wxObject
{
public:
    /**
        Default constructor.
    */
    wxSockAddress();

    /**
        Default destructor.
    */
    ~wxSockAddress();

    /**
        Delete all informations about the address.
    */
    void Clear();

    /**
        Returns the length of the socket address.
    */
    int SockAddrLen();
};


/**
    @class wxSocketEvent
    @wxheader{socket.h}

    This event class contains information about socket events.

    @library{wxnet}
    @category{net}

    @seealso
    wxSocketBase, wxSocketClient, wxSocketServer
*/
class wxSocketEvent : public wxEvent
{
public:
    /**
        Constructor.
    */
    wxSocketEvent(int id = 0);

    /**
        Gets the client data of the socket which generated this event, as
        set with wxSocketBase::SetClientData.
    */
    void* GetClientData();

    /**
        Returns the socket object to which this event refers to. This makes
        it possible to use the same event handler for different sockets.
    */
    wxSocketBase* GetSocket();

    /**
        Returns the socket event type.
    */
    wxSocketNotify GetSocketEvent();
};


/**
    @class wxSocketBase
    @wxheader{socket.h}

    wxSocketBase is the base class for all socket-related objects, and it
    defines all basic IO functionality.

    Note: (Workaround for implementation limitation for wxWidgets up to 2.5.x)
    If you want to use sockets or derived classes such as wxFTP in a secondary
    thread,
    call wxSocketBase::Initialize() (undocumented) from the main thread before
    creating
    any sockets - in wxApp::OnInit for example.
    See http://wiki.wxwidgets.org/wiki.pl?WxSocket or
    http://www.litwindow.com/knowhow/knowhow.html for more details.

    @library{wxnet}
    @category{net}

    @seealso
    wxSocketEvent, wxSocketClient, wxSocketServer, @ref overview_samplesockets
    "Sockets sample"
*/
class wxSocketBase : public wxObject
{
public:
    /**
        Default constructor. Don't use it directly; instead, use
        wxSocketClient to construct a socket client, or
        wxSocketServer to construct a socket server.
    */
    wxSocketBase();

    /**
        Destructor. Do not destroy a socket using the delete operator directly;
        use Destroy() instead. Also, do not create
        socket objects in the stack.
    */
    ~wxSocketBase();

    /**
        Functions that perform basic IO functionality.
        Close()
        
        Discard()
        
        Peek()
        
        Read()
        
        ReadMsg()
        
        Unread()
        
        Write()
        
        WriteMsg()
        Functions that perform a timed wait on a certain IO condition.
        InterruptWait()
        
        Wait()
        
        WaitForLost()
        
        WaitForRead()
        
        WaitForWrite()
        
        and also:
        wxSocketServer::WaitForAccept
        
        wxSocketClient::WaitOnConnect
        Functions that allow applications to customize socket IO as needed.
        GetFlags()
        
        SetFlags()
        
        SetTimeout()
        
        SetLocal()
    */


    /**
        This function shuts down the socket, disabling further transmission and
        reception of data; it also disables events for the socket and frees the
        associated system resources. Upon socket destruction, Close is automatically
        called, so in most cases you won't need to do it yourself, unless you
        explicitly want to shut down the socket, typically to notify the peer
        that you are closing the connection.
    */
    void Close();

    /**
        @ref construct() wxSocketBase
        
        @ref destruct() ~wxSocketBase
        
        Destroy()
    */


    /**
        Destroys the socket safely. Use this function instead of the delete operator,
        since otherwise socket events could reach the application even after the
        socket has been destroyed. To prevent this problem, this function appends
        the wxSocket to a list of object to be deleted on idle time, after all
        events have been processed. For the same reason, you should avoid creating
        socket objects in the stack.
        Destroy calls Close() automatically.
        
        @returns Always @true.
    */
    bool Destroy();

    /**
        This function simply deletes all bytes in the incoming queue. This function
        always returns immediately and its operation is not affected by IO flags.
        Use LastCount() to verify the number of bytes actually discarded.
        If you use Error(), it will always return @false.
    */
    wxSocketBase Discard();

    /**
        Returns @true if an error occurred in the last IO operation.
        Use this function to check for an error condition after one of the
        following calls: Discard, Peek, Read, ReadMsg, Unread, Write, WriteMsg.
    */
    bool Error();

    /**
        Returns a pointer of the client data for this socket, as set with
        SetClientData()
    */
    void* GetClientData();

    /**
        Returns current IO flags, as set with SetFlags()
    */
    wxSocketFlags GetFlags();

    /**
        This function returns the local address field of the socket. The local
        address field contains the complete local address of the socket (local
        address, local port, ...).
        
        @returns @true if no error happened, @false otherwise.
    */
    bool GetLocal(wxSockAddress& addr);

    /**
        This function returns the peer address field of the socket. The peer
        address field contains the complete peer host address of the socket
        (address, port, ...).
        
        @returns @true if no error happened, @false otherwise.
    */
    bool GetPeer(wxSockAddress& addr);

    /**
        Functions that allow applications to receive socket events.
        Notify()
        
        SetNotify()
        
        GetClientData()
        
        SetClientData()
        
        SetEventHandler()
    */


    /**
        Use this function to interrupt any wait operation currently in progress.
        Note that this is not intended as a regular way to interrupt a Wait call,
        but only as an escape mechanism for exceptional situations where it is
        absolutely necessary to use it, for example to abort an operation due to
        some exception or abnormal problem. InterruptWait is automatically called
        when you Close() a socket (and thus also upon
        socket destruction), so you don't need to use it in these cases.
        Wait(),
        wxSocketServer::WaitForAccept,
        WaitForLost(),
        WaitForRead(),
        WaitForWrite(),
        wxSocketClient::WaitOnConnect
    */
    void InterruptWait();

    /**
        Returns @true if the socket is connected.
    */
    bool IsConnected();

    /**
        This function waits until the socket is readable. This might mean that
        queued data is available for reading or, for streamed sockets, that
        the connection has been closed, so that a read operation will complete
        immediately without blocking (unless the @b wxSOCKET_WAITALL flag
        is set, in which case the operation might still block).
    */
    bool IsData();

    /**
        Returns @true if the socket is not connected.
    */
    bool IsDisconnected();

    /**
        Returns @true if the socket is initialized and ready and @false in other
        cases.
    */
    bool IsOk();

    /**
        Returns the number of bytes read or written by the last IO call.
        Use this function to get the number of bytes actually transferred
        after using one of the following IO calls: Discard, Peek, Read,
        ReadMsg, Unread, Write, WriteMsg.
    */
    wxUint32 LastCount();

    /**
        Returns the last wxSocket error. See @ref overview_wxsocketbase "wxSocket
        errors".
        Please note that this function merely returns the last error code,
        but it should not be used to determine if an error has occurred (this
        is because successful operations do not change the LastError value).
        Use Error() first, in order to determine
        if the last IO call failed. If this returns @true, use LastError
        to discover the cause of the error.
    */
    wxSocketError LastError();

    /**
        According to the @a notify value, this function enables
        or disables socket events. If @a notify is @true, the events
        configured with SetNotify() will
        be sent to the application. If @a notify is @false; no events
        will be sent.
    */
    void Notify(bool notify);

    /**
        This function peeks a buffer of @a nbytes bytes from the socket.
        Peeking a buffer doesn't delete it from the socket input queue.
        Use LastCount() to verify the number of bytes actually peeked.
        Use Error() to determine if the operation succeeded.
        
        @param buffer
            Buffer where to put peeked data.
        @param nbytes
            Number of bytes.
        
        @returns Returns a reference to the current object.
        
        @see Error(), LastError(), LastCount(),
             SetFlags()
    */
    wxSocketBase Peek(void* buffer, wxUint32 nbytes);

    /**
        This function reads a buffer of @a nbytes bytes from the socket.
        Use LastCount() to verify the number of bytes actually read.
        Use Error() to determine if the operation succeeded.
        
        @param buffer
            Buffer where to put read data.
        @param nbytes
            Number of bytes.
        
        @returns Returns a reference to the current object.
        
        @see Error(), LastError(), LastCount(),
             SetFlags()
    */
    wxSocketBase Read(void* buffer, wxUint32 nbytes);

    /**
        This function reads a buffer sent by WriteMsg()
        on a socket. If the buffer passed to the function isn't big enough, the
        remaining bytes will be discarded. This function always waits for the
        buffer to be entirely filled, unless an error occurs.
        Use LastCount() to verify the number of bytes actually read.
        Use Error() to determine if the operation succeeded.
        
        @param buffer
            Buffer where to put read data.
        @param nbytes
            Size of the buffer.
        
        @returns Returns a reference to the current object.
        
        @see Error(), LastError(), LastCount(),
             SetFlags(), WriteMsg()
    */
    wxSocketBase ReadMsg(void* buffer, wxUint32 nbytes);

    /**
        This function restores the previous state of the socket, as saved
        with SaveState()
        Calls to SaveState and RestoreState can be nested.
        
        @see SaveState()
    */
    void RestoreState();

    /**
        This function saves the current state of the socket in a stack. Socket
        state includes flags, as set with SetFlags(),
        event mask, as set with SetNotify() and
        Notify(), user data, as set with
        SetClientData().
        Calls to SaveState and RestoreState can be nested.
        
        @see RestoreState()
    */
    void SaveState();

    /**
        Sets user-supplied client data for this socket. All socket events will
        contain a pointer to this data, which can be retrieved with
        the wxSocketEvent::GetClientData function.
    */
    void SetClientData(void* data);

    /**
        Sets an event handler to be called when a socket event occurs. The
        handler will be called for those events for which notification is
        enabled with SetNotify() and
        Notify().
        
        @param handler
            Specifies the event handler you want to use.
        @param id
            The id of socket event.
        
        @see SetNotify(), Notify(), wxSocketEvent, wxEvtHandler
    */
    void SetEventHandler(wxEvtHandler& handler, int id = -1);

    /**
        Use SetFlags to customize IO operation for this socket.
        The @a flags parameter may be a combination of flags ORed together.
        The following flags can be used:
        
        @b wxSOCKET_NONE
        
        Normal functionality.
        
        @b wxSOCKET_NOWAIT
        
        Read/write as much data as possible and return immediately.
        
        @b wxSOCKET_WAITALL
        
        Wait for all required data to be read/written unless an error occurs.
        
        @b wxSOCKET_BLOCK
        
        Block the GUI (do not yield) while reading/writing data.
        
        @b wxSOCKET_REUSEADDR
        
        Allows the use of an in-use port (wxServerSocket only)
        
        @b wxSOCKET_BROADCAST
        
        Switches the socket to broadcast mode
        
        @b wxSOCKET_NOBIND
        
        Stops the socket from being bound to a specific adapter (normally used in
        conjunction with @b wxSOCKET_BROADCAST)
        
        A brief overview on how to use these flags follows.
        If no flag is specified (this is the same as @b wxSOCKET_NONE),
        IO calls will return after some data has been read or written, even
        when the transfer might not be complete. This is the same as issuing
        exactly one blocking low-level call to recv() or send(). Note
        that @e blocking here refers to when the function returns, not
        to whether the GUI blocks during this time.
        If @b wxSOCKET_NOWAIT is specified, IO calls will return immediately.
        Read operations will retrieve only available data. Write operations will
        write as much data as possible, depending on how much space is available
        in the output buffer. This is the same as issuing exactly one nonblocking
        low-level call to recv() or send(). Note that @e nonblocking here
        refers to when the function returns, not to whether the GUI blocks during
        this time.
        If @b wxSOCKET_WAITALL is specified, IO calls won't return until ALL
        the data has been read or written (or until an error occurs), blocking if
        necessary, and issuing several low level calls if necessary. This is the
        same as having a loop which makes as many blocking low-level calls to
        recv() or send() as needed so as to transfer all the data. Note
        that @e blocking here refers to when the function returns, not
        to whether the GUI blocks during this time.
        The @b wxSOCKET_BLOCK flag controls whether the GUI blocks during
        IO operations. If this flag is specified, the socket will not yield
        during IO calls, so the GUI will remain blocked until the operation
        completes. If it is not used, then the application must take extra
        care to avoid unwanted reentrance.
        The @b wxSOCKET_REUSEADDR flag controls the use of the SO_REUSEADDR standard
        setsockopt() flag. This flag allows the socket to bind to a port that is
        already in use.
        This is mostly used on UNIX-based systems to allow rapid starting and stopping
        of a server -
        otherwise you may have to wait several minutes for the port to become available.
        wxSOCKET_REUSEADDR can also be used with socket clients to (re)bind to a
        particular local port
        for an outgoing connection.
        This option can have surprising platform dependent behavior, so check the
        documentation for
        your platform's implementation of setsockopt(). Note that on BSD-based systems
        (e.g. Mac OS X),
        use of wxSOCKET_REUSEADDR implies SO_REUSEPORT in addition to SO_REUSEADDR to
        be consistent
        with Windows.
        The @b wxSOCKET_BROADCAST flag controls the use of the SO_BROADCAST standard
        setsockopt() flag. This flag allows the socket to use the broadcast address,
        and is generally
        used in conjunction with @b wxSOCKET_NOBIND and wxIPaddress::BroadcastAddress.
        So:
        @b wxSOCKET_NONE will try to read at least SOME data, no matter how much.
        @b wxSOCKET_NOWAIT will always return immediately, even if it cannot
        read or write ANY data.
        @b wxSOCKET_WAITALL will only return when it has read or written ALL
        the data.
        @b wxSOCKET_BLOCK has nothing to do with the previous flags and
        it controls whether the GUI blocks.
        @b wxSOCKET_REUSEADDR controls special platform-specific behavior for
        reusing local addresses/ports.
    */
    void SetFlags(wxSocketFlags flags);

    /**
        This function allows you to set the local address and port,
        useful when an application needs to reuse a particular port. When
        a local port is set for a wxSocketClient,
        @b bind will be called before @b connect.
    */
    bool SetLocal(wxIPV4address& local);

    /**
        SetNotify specifies which socket events are to be sent to the event handler.
        The @a flags parameter may be combination of flags ORed together. The
        following flags can be used:
        
        @b wxSOCKET_INPUT_FLAG
        
        to receive wxSOCKET_INPUT
        
        @b wxSOCKET_OUTPUT_FLAG
        
        to receive wxSOCKET_OUTPUT
        
        @b wxSOCKET_CONNECTION_FLAG
        
        to receive wxSOCKET_CONNECTION
        
        @b wxSOCKET_LOST_FLAG
        
        to receive wxSOCKET_LOST
        
        For example:
        
        In this example, the user will be notified about incoming socket data and
        whenever the connection is closed.
        For more information on socket events see @ref overview_wxsocketbase "wxSocket
        events".
    */
    void SetNotify(wxSocketEventFlags flags);

    /**
        This function sets the default socket timeout in seconds. This timeout
        applies to all IO calls, and also to the Wait() family
        of functions if you don't specify a wait interval. Initially, the default
        timeout is 10 minutes.
    */
    void SetTimeout(int seconds);

    /**
        Functions to retrieve current state and miscellaneous info.
        Error()
        
        GetLocal()
        
        GetPeer()
        IsConnected()
        
        IsData()
        
        IsDisconnected()
        
        LastCount()
        
        LastError()
        
        IsOk()
        
        SaveState()
        
        RestoreState()
    */


    /**
        This function unreads a buffer. That is, the data in the buffer is put back
        in the incoming queue. This function is not affected by wxSocket flags.
        If you use LastCount(), it will always return @e nbytes.
        If you use Error(), it will always return @false.
        
        @param buffer
            Buffer to be unread.
        @param nbytes
            Number of bytes.
        
        @returns Returns a reference to the current object.
        
        @see Error(), LastCount(), LastError()
    */
    wxSocketBase Unread(const void* buffer, wxUint32 nbytes);

    /**
        This function waits until any of the following conditions is @true:
        
         The socket becomes readable.
         The socket becomes writable.
         An ongoing connection request has completed (wxSocketClient only)
         An incoming connection request has arrived (wxSocketServer only)
         The connection has been closed.
        Note that it is recommended to use the individual Wait functions
        to wait for the required condition, instead of this one.
        
        @param seconds
            Number of seconds to wait.
            If -1, it will wait for the default timeout,
            as set with SetTimeout.
        @param millisecond
            Number of milliseconds to wait.
        
        @returns Returns @true when any of the above conditions is satisfied,
                 @false if the timeout was reached.
        
        @see InterruptWait(), wxSocketServer::WaitForAccept,
             WaitForLost(), WaitForRead(),
             WaitForWrite(), wxSocketClient::WaitOnConnect
    */
    bool Wait(long seconds = -1, long millisecond = 0);

    /**
        This function waits until the connection is lost. This may happen if
        the peer gracefully closes the connection or if the connection breaks.
        
        @param seconds
            Number of seconds to wait.
            If -1, it will wait for the default timeout,
            as set with SetTimeout.
        @param millisecond
            Number of milliseconds to wait.
        
        @returns Returns @true if the connection was lost, @false if the timeout
                 was reached.
        
        @see InterruptWait(), Wait()
    */
    bool Wait(long seconds = -1, long millisecond = 0);

    /**
        This function waits until the socket is readable. This might mean that
        queued data is available for reading or, for streamed sockets, that
        the connection has been closed, so that a read operation will complete
        immediately without blocking (unless the @b wxSOCKET_WAITALL flag
        is set, in which case the operation might still block).
        
        @param seconds
            Number of seconds to wait.
            If -1, it will wait for the default timeout,
            as set with SetTimeout.
        @param millisecond
            Number of milliseconds to wait.
        
        @returns Returns @true if the socket becomes readable, @false on timeout.
        
        @see InterruptWait(), Wait()
    */
    bool WaitForRead(long seconds = -1, long millisecond = 0);

    /**
        This function waits until the socket becomes writable. This might mean that
        the socket is ready to send new data, or for streamed sockets, that the
        connection has been closed, so that a write operation is guaranteed to
        complete immediately (unless the @b wxSOCKET_WAITALL flag is set,
        in which case the operation might still block).
        
        @param seconds
            Number of seconds to wait.
            If -1, it will wait for the default timeout,
            as set with SetTimeout.
        @param millisecond
            Number of milliseconds to wait.
        
        @returns Returns @true if the socket becomes writable, @false on timeout.
        
        @see InterruptWait(), Wait()
    */
    bool WaitForWrite(long seconds = -1, long millisecond = 0);

    /**
        This function writes a buffer of @a nbytes bytes to the socket.
        Use LastCount() to verify the number of bytes actually written.
        Use Error() to determine if the operation succeeded.
        
        @param buffer
            Buffer with the data to be sent.
        @param nbytes
            Number of bytes.
        
        @returns Returns a reference to the current object.
        
        @see Error(), LastError(), LastCount(),
             SetFlags()
    */
    wxSocketBase Write(const void* buffer, wxUint32 nbytes);

    /**
        This function writes a buffer of @a nbytes bytes from the socket, but it
        writes a short header before so that ReadMsg()
        knows how much data should it actually read. So, a buffer sent with WriteMsg
        @b must be read with ReadMsg. This function always waits for the entire
        buffer to be sent, unless an error occurs.
        Use LastCount() to verify the number of bytes actually written.
        Use Error() to determine if the operation succeeded.
        
        @param buffer
            Buffer with the data to be sent.
        @param nbytes
            Number of bytes to send.
        
        @returns Returns a reference to the current object.
    */
    wxSocketBase WriteMsg(const void* buffer, wxUint32 nbytes);
};


/**
    @class wxDatagramSocket
    @wxheader{socket.h}


    @library{wxnet}
    @category{FIXME}

    @seealso
    wxSocketBase::Error, wxSocketBase::LastError, wxSocketBase::LastCount,
    wxSocketBase::SetFlags,
*/
class wxDatagramSocket : public wxSocketBase
{
public:
    /**
        Constructor.
        
        @param flags
            Socket flags (See wxSocketBase::SetFlags)
    */
    wxDatagramSocket(wxSocketFlags flags = wxSOCKET_NONE);

    /**
        Destructor. Please see wxSocketBase::Destroy.
    */
    ~wxDatagramSocket();

    /**
        This function reads a buffer of @a nbytes bytes from the socket.
        Use wxSocketBase::LastCount to verify the number of bytes actually read.
        Use wxSocketBase::Error to determine if the operation succeeded.
        
        @param address
            Any address - will be overwritten with the address of the peer that sent
        that data.
        @param buffer
            Buffer where to put read data.
        @param nbytes
            Number of bytes.
        
        @returns Returns a reference to the current object, and the address of
                 the peer that sent the data on address param.
        
        @see wxSocketBase::Error, wxSocketBase::LastError, wxSocketBase::LastCount,
             wxSocketBase::SetFlags,
    */
    wxDatagramSocket ReceiveFrom(wxSockAddress& address,
                                 void* buffer,
                                 wxUint32 nbytes);

    /**
        This function writes a buffer of @a nbytes bytes to the socket.
        Use wxSocketBase::LastCount to verify the number of bytes actually wrote.
        Use wxSocketBase::Error to determine if the operation succeeded.
        
        @param address
            The address of the destination peer for this data.
        @param buffer
            Buffer where read data is.
        @param nbytes
            Number of bytes.
        
        @returns Returns a reference to the current object.
    */
    wxDatagramSocket SendTo(const wxSockAddress& address,
                            const void* buffer,
                            wxUint32 nbytes);
};
