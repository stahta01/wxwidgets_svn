/////////////////////////////////////////////////////////////////////////////
// Name:        process.h
// Purpose:     documentation for wxProcess class
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxProcess
    @wxheader{process.h}

    The objects of this class are used in conjunction with the
    wxExecute function. When a wxProcess object is passed to
    wxExecute(), its wxProcess::OnTerminate virtual method
    is called when the process terminates. This allows the program to be
    (asynchronously) notified about the process termination and also retrieve its
    exit status which is unavailable from wxExecute() in the case of
    asynchronous execution.

    Please note that if the process termination notification is processed by the
    parent, it is responsible for deleting the wxProcess object which sent it.
    However, if it is not processed, the object will delete itself and so the
    library users should only delete those objects whose notifications have been
    processed (and call wxProcess::Detach for others).

    wxProcess also supports IO redirection of the child process. For this, you have
    to call its wxProcess::Redirect method before passing it to
    wxExecute. If the child process was launched successfully,
    wxProcess::GetInputStream,
    wxProcess::GetOutputStream and
    wxProcess::GetErrorStream can then be used to retrieve
    the streams corresponding to the child process standard output, input and
    error output respectively.

    @b wxPerl note: In wxPerl this class has an additional @c Destroy method,
    for explicit destruction.

    @library{wxbase}
    @category{appmanagement}

    @seealso
    wxExecute, @ref overview_sampleexec "exec sample"
*/
class wxProcess : public wxEvtHandler
{
public:
    //@{
    /**
        Constructs a process object. @e id is only used in the case you want to
        use wxWidgets events. It identifies this object, or another window that will
        receive the event.
        
        If the @e parent parameter is different from @NULL, it will receive
        a wxEVT_END_PROCESS notification event (you should insert EVT_END_PROCESS
        macro in the event table of the parent to handle it) with the given @e id.
        
        The second constructor creates an object without any associated parent (and
        hence no id neither) but allows to specify the @e flags which can have the
        value of @c wxPROCESS_DEFAULT or @c wxPROCESS_REDIRECT. Specifying the
        former value has no particular effect while using the latter one is equivalent
        to calling Redirect().
        
        @param parent
        The event handler parent.
        
        @param id
        id of an event.
        
        @param flags
        either wxPROCESS_DEFAULT or wxPROCESS_REDIRECT
    */
    wxProcess(wxEvtHandler * parent = @NULL, int id = -1);
    wxProcess(int flags);
    //@}

    /**
        Destroys the wxProcess object.
    */
    ~wxProcess();

    /**
        Closes the output stream (the one connected to the stdin of the child
        process). This function can be used to indicate to the child process that
        there is no more data to be read - usually, a filter program will only
        terminate when the input stream is closed.
    */
    void CloseOutput();

    /**
        Normally, a wxProcess object is deleted by its parent when it receives the
        notification about the process termination. However, it might happen that the
        parent object is destroyed before the external process is terminated (e.g. a
        window from which this external process was launched is closed by the user)
        and in this case it @b should not delete the wxProcess object, but
        @b should call Detach() instead. After the wxProcess object is detached
        from its parent, no notification events will be sent to the parent and the
        object will delete itself upon reception of the process termination
        notification.
    */
    void Detach();

    /**
        Returns @true if the given process exists in the system.
        
        @sa Kill(), @ref overview_sampleexec "Exec sample"
    */
    static bool Exists(int pid);

    /**
        Returns an input stream which corresponds to the standard error output (stderr)
        of the child process.
    */
    wxInputStream* GetErrorStream();

    /**
        It returns an input stream corresponding to the standard output stream of the
        subprocess. If it is @NULL, you have not turned on the redirection.
        See Redirect().
    */
    wxInputStream* GetInputStream();

    /**
        It returns an output stream correspoding to the input stream of the subprocess.
        If it is @NULL, you have not turned on the redirection.
        See Redirect().
    */
    wxOutputStream* GetOutputStream();

    /**
        Returns the process ID of the process launched by Open().
    */
    long GetPid();

    /**
        Returns @true if there is data to be read on the child process standard
        error stream.
        
        @sa IsInputAvailable()
    */
    bool IsErrorAvailable();

    /**
        Returns @true if there is data to be read on the child process standard
        output stream. This allows to write simple (and extremely inefficient)
        polling-based code waiting for a better mechanism in future wxWidgets versions.
        
        See the @ref overview_sampleexec "exec sample" for an example of using this
        function.
        
        @sa IsInputOpened()
    */
    bool IsInputAvailable();

    /**
        Returns @true if the child process standard output stream is opened.
    */
    bool IsInputOpened();

    /**
        Send the specified signal to the given process. Possible signal values are:
        @c wxSIGNONE, @c wxSIGKILL and @c wxSIGTERM have the same meaning
        under both Unix and Windows but all the other signals are equivalent to
        @c wxSIGTERM under Windows.
        
        The @e flags parameter can be wxKILL_NOCHILDREN (the default),
        or wxKILL_CHILDREN, in which case the child processes of this
        process will be killed too. Note that under Unix, for wxKILL_CHILDREN
        to work you should have created the process passing wxEXEC_MAKE_GROUP_LEADER.
        
        Returns the element of @c wxKillError enum:
        
        @sa Exists(), wxKill, @ref overview_sampleexec "Exec sample"
    */
    static wxKillError Kill(int pid, wxSignal signal = wxSIGNONE,
                            int flags = wxKILL_NOCHILDREN);

    /**
        It is called when the process with the pid
        
        @param pid finishes.
        It raises a wxWidgets event when it isn't overridden.
        
        pid
        The pid of the process which has just terminated.
        
        @param status
        The exit code of the process.
    */
    void OnTerminate(int pid, int status);

    /**
        This static method replaces the standard @c popen() function: it launches
        the process specified by the @e cmd parameter and returns the wxProcess
        object which can be used to retrieve the streams connected to the standard
        input, output and error output of the child process.
        
        If the process couldn't be launched, @NULL is returned. Note that in any
        case the returned pointer should @b not be deleted, rather the process
        object will be destroyed automatically when the child process terminates. This
        does mean that the child process should be told to quit before the main program
        exits to avoid memory leaks.
        
        @param cmd
        The command to execute, including optional arguments.
        
        @param flags
        The flags to pass to wxExecute.
          NOTE: wxEXEC_SYNC should not be used.
        
        @returns A pointer to new wxProcess object or @NULL on error.
        
        @sa wxExecute
    */
    static wxProcess * Open(const wxString& cmd,
                            int flags = wxEXEC_ASYNC);

    /**
        Turns on redirection. wxExecute will try to open a couple of pipes
        to catch the subprocess stdio. The caught input stream is returned by
        GetOutputStream() as a non-seekable stream. The caught output stream is returned
        by GetInputStream() as a non-seekable stream.
    */
    void Redirect();
};


/**
    @class wxProcessEvent
    @wxheader{process.h}

    A process event is sent when a process is terminated.

    @library{wxbase}
    @category{events}

    @seealso
    wxProcess, @ref overview_eventhandlingoverview "Event handling overview"
*/
class wxProcessEvent : public wxEvent
{
public:
    /**
        Constructor. Takes a wxProcessObject or window id, a process id and an
        exit status.
    */
    wxProcessEvent(int id = 0, int pid = 0, int exitcode = 0);

    /**
        Returns the exist status.
    */
    int GetExitCode();

    /**
        Returns the process id.
    */
    int GetPid();
};
