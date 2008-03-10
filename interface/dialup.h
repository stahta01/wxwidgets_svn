/////////////////////////////////////////////////////////////////////////////
// Name:        dialup.h
// Purpose:     interface of wxDialUpManager
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxDialUpManager
    @wxheader{dialup.h}

    This class encapsulates functions dealing with verifying the connection status
    of the workstation (connected to the Internet via a direct connection,
    connected through a modem or not connected at all) and to establish this
    connection if possible/required (i.e. in the case of the modem).

    The program may also wish to be notified about the change in the connection
    status (for example, to perform some action when the user connects to the
    network the next time or, on the contrary, to stop receiving data from the net
    when the user hangs up the modem). For this, you need to use one of the event
    macros described below.

    This class is different from other wxWidgets classes in that there is at most
    one instance of this class in the program accessed via
    wxDialUpManager::Create and you can't
    create the objects of this class directly.

    @library{wxcore}
    @category{net}

    @see @ref overview_sampledialup "dialup sample", wxDialUpEvent
*/
class wxDialUpManager
{
public:
    /**
        Destructor.
    */
    ~wxDialUpManager();

    /**
        Cancel dialing the number initiated with Dial()
        with async parameter equal to @true.
        Note that this won't result in DISCONNECTED event being sent.
        
        @see IsDialing()
    */
    bool CancelDialing();

    /**
        This function should create and return the object of the platform-specific
        class derived from wxDialUpManager. You should delete the pointer when you are
        done with it.
    */
    wxDialUpManager* Create();

    /**
        Dial the given ISP, use @a username and @a password to authenticate.
        The parameters are only used under Windows currently, for Unix you should use
        SetConnectCommand() to customize this
        functions behaviour.
        If no @a nameOfISP is given, the function will select the default one
        (proposing the user to choose among all connections defined on this machine)
        and if no username and/or password are given, the function will try to do
        without them, but will ask the user if really needed.
        If @a async parameter is @false, the function waits until the end of dialing
        and returns @true upon successful completion.
        If @a async is @true, the function only initiates the connection and
        returns immediately - the result is reported via events (an event is sent
        anyhow, but if dialing failed it will be a DISCONNECTED one).
    */
    bool Dial(const wxString& nameOfISP = wxEmptyString,
              const wxString& username = wxEmptyString,
              const wxString& password = wxEmptyString,
              bool async = true);

    /**
        Disable automatic check for connection status change - notice that the
        @c wxEVT_DIALUP_XXX events won't be sent any more neither.
    */
    void DisableAutoCheckOnlineStatus();

    /**
        Enable automatic checks for the connection status and sending of
        @c wxEVT_DIALUP_CONNECTED/wxEVT_DIALUP_DISCONNECTED events. The interval
        parameter is only for Unix where we do the check manually and specifies how
        often should we repeat the check (each minute by default). Under Windows, the
        notification about the change of connection status is sent by the system and so
        we don't do any polling and this parameter is ignored.
        Returns @false if couldn't set up automatic check for online status.
    */
    bool EnableAutoCheckOnlineStatus(size_t nSeconds = 60);

    /**
        This function is only implemented under Windows.
        Fills the array with the names of all possible values for the first
        parameter to Dial() on this machine and returns
        their number (may be 0).
    */
    size_t GetISPNames(wxArrayString& names) const;

    /**
        Hang up the currently active dial up connection.
    */
    bool HangUp();

    /**
        Returns @true if the computer has a permanent network connection (i.e. is
        on a LAN) and so there is no need to use Dial() function to go online.
        @b NB: this functions tries to guess the result and it is not always
        guaranteed to be correct, so it is better to ask user for
        confirmation or give him a possibility to override it.
    */
    bool IsAlwaysOnline() const;

    /**
        Returns @true if (async) dialing is in progress.
        
        @see Dial()
    */
    bool IsDialing() const;

    /**
        Returns @true if the dialup manager was initialized correctly. If this
        function returns @false, no other functions will work neither, so it is a
        good idea to call this function and check its result before calling any other
        wxDialUpManager methods
    */
    bool IsOk() const;

    /**
        Returns @true if the computer is connected to the network: under Windows,
        this just means that a RAS connection exists, under Unix we check that
        the "well-known host" (as specified by
        wxDialUpManager::SetWellKnownHost) is reachable.
    */
    bool IsOnline() const;

    /**
        ,  wxString&@e commandHangup = wxT("/usr/bin/poff"))
        This method is for Unix only.
        Sets the commands to start up the network and to hang up again.
    */
    void SetConnectCommand() const;

    /**
        Sometimes the built-in logic for determining the online status may fail,
        so, in general, the user should be allowed to override it. This function
        allows to forcefully set the online status - whatever our internal
        algorithm may think about it.
        
        @see IsOnline()
    */
    void SetOnlineStatus(bool isOnline = true);

    /**
        This method is for Unix only.
        Under Unix, the value of well-known host is used to check whether we're
        connected to the internet. It is unused under Windows, but this function
        is always safe to call. The default value is @c www.yahoo.com:80.
    */
    void SetWellKnownHost(const wxString& hostname, int portno = 80);
};



/**
    @class wxDialUpEvent
    @wxheader{dialup.h}

    This is the event class for the dialup events sent by
    wxDialUpManager.

    @library{wxcore}
    @category{events}
*/
class wxDialUpEvent : public wxEvent
{
public:
    /**
        Constructor is only used by wxDialUpManager.
    */
    wxDialUpEvent(bool isConnected, bool isOwnEvent);

    /**
        Is this a @c CONNECTED or @c DISCONNECTED event? In other words, does it
        notify about transition from offline to online state or vice versa?
    */
    bool IsConnectedEvent() const;

    /**
        Does this event come from wxDialUpManager::Dial() or from some extrenal
        process (i.e. does it result from our own attempt to establish the
        connection)?
    */
    bool IsOwnEvent() const;
};

