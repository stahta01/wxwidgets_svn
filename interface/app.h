/////////////////////////////////////////////////////////////////////////////
// Name:        app.h
// Purpose:     documentation for wxApp class
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxApp
    @wxheader{app.h}

    The @b wxApp class represents the application itself. It is used
    to:

     set and get application-wide properties;
     implement the windowing system message or event loop;
     initiate application processing via wxApp::OnInit;
     allow default processing of events not handled by other
    objects in the application.

    You should use the macro IMPLEMENT_APP(appClass) in your application
    implementation
    file to tell wxWidgets how to create an instance of your application class.

    Use DECLARE_APP(appClass) in a header file if you want the wxGetApp function
    (which returns
    a reference to your application object) to be visible to other files.

    @library{wxbase}
    @category{appmanagement}

    @seealso
    @ref overview_wxappoverview "wxApp overview"
*/
class wxApp : public wxEvtHandler
{
public:
    /**
        Constructor. Called implicitly with a definition of a wxApp object.
    */
    wxApp();

    /**
        Destructor. Will be called implicitly on program exit if the wxApp
        object is created on the stack.
    */
    ~wxApp();

    /**
        Creates a wxLog class for the application to use for logging errors. The default
        implementation returns a new wxLogGui class.
        
        @sa wxLog
    */
    virtual wxLog* CreateLogTarget();

    /**
        Creates the wxAppTraits object when GetTraits()
        needs it for the first time.
        
        @sa wxAppTraits
    */
    virtual wxAppTraits * CreateTraits();

    /**
        Dispatches the next event in the windowing system event queue.
        
        This can be used for programming event loops, e.g.
        
        @sa Pending()
    */
    virtual void Dispatch();

    /**
        Call this to explicitly exit the main message (event) loop.
        You should normally exit the main loop (and the application) by deleting
        the top window.
    */
    virtual void ExitMainLoop();

    /**
        This function is called before processing any event and allows the application
        to preempt the processing of some events. If this method returns -1 the event
        is processed normally, otherwise either @true or @false should be
        returned and the event processing stops immediately considering that the event
        had been already processed (for the former return value) or that it is not
        going to be processed at all (for the latter one).
    */
    int FilterEvent(wxEvent& event);

    /**
        Returns the user-readable application name. The difference between this string
        and the one returned by GetAppName() is that this one
        is meant to be shown to the user and so should be used for the window titles,
        page headers and so on while the other one should be only used internally, e.g.
        for the file names or configuration file keys.
        
        By default, returns the same string as GetAppName().
        
        This function is new since wxWidgets version 2.9.0
    */
    wxString GetAppDisplayName();

    /**
        Returns the application name.
        
        @remarks wxWidgets sets this to a reasonable default before calling
                   OnInit(), but the application can reset it at
                   will.
        
        @sa GetAppDisplayName()
    */
    wxString GetAppName();

    /**
        Gets the class name of the application. The class name may be used in a
        platform specific
        manner to refer to the application.
        
        @sa SetClassName()
    */
    wxString GetClassName();

    /**
        Returns @true if the application will exit when the top-level window is deleted,
        @false
        otherwise.
        
        @sa SetExitOnFrameDelete(), @ref overview_wxappshutdownoverview "wxApp
              shutdown overview"
    */
    bool GetExitOnFrameDelete();

    /**
        Returns the one and only global application object.
        Usually @c wxTheApp is usead instead.
        
        @sa SetInstance()
    */
    static wxAppConsole * GetInstance();

    /**
        Returns a pointer to the top window.
        
        @remarks If the top window hasn't been set using SetTopWindow(),
                   this function will find the first top-level window
                   (frame or dialog) and return that.
        
        @sa SetTopWindow()
    */
    virtual wxWindow * GetTopWindow();

    /**
        Returns a pointer to the wxAppTraits object for the application.
        If you want to customize the wxAppTraits object, you must override the
        CreateTraits() function.
    */
    wxAppTraits * GetTraits();

    /**
        Returns @true if the application will use the best visual on systems that support
        different visuals, @false otherwise.
        
        @sa SetUseBestVisual()
    */
    bool GetUseBestVisual();

    /**
        Returns the user-readable vendor name. The difference between this string
        and the one returned by GetVendorName() is that this one
        is meant to be shown to the user and so should be used for the window titles,
        page headers and so on while the other one should be only used internally, e.g.
        for the file names or configuration file keys.
        
        By default, returns the same string as GetVendorName().
        
        This function is new since wxWidgets version 2.9.0
    */
    wxString GetVendorDisplayName();

    /**
        Returns the application's vendor name.
    */
    wxString GetVendorName();

    /**
        This function simply invokes the given method @e func of the specified
        event handler @e handler with the @e event as parameter. It exists solely
        to allow to catch the C++ exceptions which could be thrown by all event
        handlers in the application in one place: if you want to do this, override this
        function in your wxApp-derived class and add try/catch clause(s) to it.
    */
    virtual void HandleEvent(wxEvtHandler handler,
                             wxEventFunction func,
                             wxEvent& event);

    /**
        Returns @true if the application is active, i.e. if one of its windows is
        currently in the foreground. If this function returns @false and you need to
        attract users attention to the application, you may use
        wxTopLevelWindow::RequestUserAttention
        to do it.
    */
    bool IsActive();

    /**
        Returns @true if the main event loop is currently running, i.e. if the
        application is inside OnRun().
        
        This can be useful to test whether events can be dispatched. For example,
        if this function returns @false, non-blocking sockets cannot be used because
        the events from them would never be processed.
    */
    static bool IsMainLoopRunning();

    /**
        Mac specific. Called in response of an "open-application" Apple event.
        Override this to create a new document in your app.
    */
    void MacNewFile();

    /**
        Mac specific. Called in response of an "open-document" Apple event. You need to
        override this method in order to open a document file after the
        user double clicked on it or if the document file was dropped
        on either the running application or the application icon in
        Finder.
    */
    void MacOpenFile(const wxString& fileName);

    /**
        Mac specific. Called in response of a "get-url" Apple event.
    */
    void MacOpenURL(const wxString& url);

    /**
        Mac specific. Called in response of a "print-document" Apple event.
    */
    void MacPrintFile(const wxString& fileName);

    /**
        Mac specific. Called in response of a "reopen-application" Apple event.
    */
    void MacReopenApp();

    /**
        Called by wxWidgets on creation of the application. Override this if you wish
        to provide your own (environment-dependent) main loop.
        
        @returns Returns 0 under X, and the wParam of the WM_QUIT message under
                   Windows.
    */
    virtual int MainLoop();

    /**
        This function is called when an assert failure occurs, i.e. the condition
        specified in wxASSERT macro evaluated to @false.
        It is only called in debug mode (when @c __WXDEBUG__ is defined) as
        asserts are not left in the release code at all.
        
        The base class version shows the default assert failure dialog box proposing to
        the user to stop the program, continue or ignore all subsequent asserts.
        
        @param file
        the name of the source file where the assert occurred
        
        @param line
        the line number in this file where the assert occurred
        
        @param func
        the name of the function where the assert occurred, may be
        empty if the compiler doesn't support C99 __FUNCTION__
        
        @param cond
        the condition of the failed assert in text form
        
        @param msg
        the message specified as argument to
        wxASSERT_MSG or wxFAIL_MSG, will
        be @NULL if just wxASSERT or wxFAIL
        was used
    */
    void OnAssertFailure(const wxChar file, int line,
                         const wxChar func,
                         const wxChar cond,
                         const wxChar msg);

    /**
        Called when command line parsing fails (i.e. an incorrect command line option
        was specified by the user). The default behaviour is to show the program usage
        text and abort the program.
        
        Return @true to continue normal execution or @false to return
        @false from OnInit() thus terminating the program.
        
        @sa OnInitCmdLine()
    */
    bool OnCmdLineError(wxCmdLineParser& parser);

    /**
        Called when the help option (@c --help) was specified on the command line.
        The default behaviour is to show the program usage text and abort the program.
        
        Return @true to continue normal execution or @false to return
        @false from OnInit() thus terminating the program.
        
        @sa OnInitCmdLine()
    */
    bool OnCmdLineHelp(wxCmdLineParser& parser);

    /**
        Called after the command line had been successfully parsed. You may override
        this method to test for the values of the various parameters which could be
        set from the command line.
        
        Don't forget to call the base class version unless you want to suppress
        processing of the standard command line options.
        
        Return @true to continue normal execution or @false to return
        @false from OnInit() thus terminating the program.
        
        @sa OnInitCmdLine()
    */
    bool OnCmdLineParsed(wxCmdLineParser& parser);

    /**
        This function is called if an unhandled exception occurs inside the main
        application event loop. It can return @true to ignore the exception and to
        continue running the loop or @false to exit the loop and terminate the
        program. In the latter case it can also use C++ @c throw keyword to
        rethrow the current exception.
        
        The default behaviour of this function is the latter in all ports except under
        Windows where a dialog is shown to the user which allows him to choose between
        the different options. You may override this function in your class to do
        something more appropriate.
        
        Finally note that if the exception is rethrown from here, it can be caught in
        OnUnhandledException().
    */
    virtual bool OnExceptionInMainLoop();

    /**
        Override this member function for any processing which needs to be
        done as the application is about to exit. OnExit is called after
        destroying all application windows and controls, but before
        wxWidgets cleanup. Note that it is not called at all if
        OnInit() failed.
        
        The return value of this function is currently ignored, return the same value
        as returned by the base class method if you override it.
    */
    virtual int OnExit();

    /**
        This function may be called if something fatal happens: an unhandled
        exception under Win32 or a a fatal signal under Unix, for example. However,
        this will not happen by default: you have to explicitly call
        wxHandleFatalExceptions to enable this.
        
        Generally speaking, this function should only show a message to the user and
        return. You may attempt to save unsaved data but this is not guaranteed to
        work and, in fact, probably won't.
        
        @sa wxHandleFatalExceptions
    */
    void OnFatalException();

    /**
        This must be provided by the application, and will usually create the
        application's main window, optionally calling
        SetTopWindow(). You may use
        OnExit() to clean up anything initialized here, provided
        that the function returns @true.
        
        Notice that if you want to to use the command line processing provided by
        wxWidgets you have to call the base class version in the derived class
        OnInit().
        
        Return @true to continue processing, @false to exit the application
        immediately.
    */
    bool OnInit();

    /**
        Called from OnInit() and may be used to initialize the
        parser with the command line options for this application. The base class
        versions adds support for a few standard options only.
    */
    void OnInitCmdLine(wxCmdLineParser& parser);

    /**
        This virtual function is where the execution of a program written in wxWidgets
        starts. The default implementation just enters the main loop and starts
        handling the events until it terminates, either because
        ExitMainLoop() has been explicitly called or because
        the last frame has been deleted and
        GetExitOnFrameDelete() flag is @true (this
        is the default).
        
        The return value of this function becomes the exit code of the program, so it
        should return 0 in case of successful termination.
    */
    virtual int OnRun();

    /**
        This function is called when an unhandled C++ exception occurs inside
        OnRun() (the exceptions which occur during the program
        startup and shutdown might not be caught at all). Notice that by now the main
        event loop has been terminated and the program will exit, if you want to
        prevent this from happening (i.e. continue running after catching an exception)
        you need to override OnExceptionInMainLoop().
        
        The default implementation shows information about the exception in debug build
        but does nothing in the release build.
    */
    virtual void OnUnhandledException();

    /**
        Returns @true if unprocessed events are in the window system event queue.
        
        @sa Dispatch()
    */
    virtual bool Pending();

    /**
        Windows-only function for processing a message. This function
        is called from the main message loop, checking for windows that
        may wish to process it. The function returns @true if the message
        was processed, @false otherwise. If you use wxWidgets with another class
        library with its own message loop, you should make sure that this
        function is called to allow wxWidgets to receive messages. For example,
        to allow co-existence with the Microsoft Foundation Classes, override
        the PreTranslateMessage function:
    */
    bool ProcessMessage(WXMSG * msg);

    /**
        Sends idle events to a window and its children.
        
        Please note that this function is internal to wxWidgets and shouldn't be used
        by user code.
        
        @remarks These functions poll the top-level windows, and their children,
                   for idle event processing. If @true is returned, more
                   OnIdle processing is requested by one or more window.
        
        @sa wxIdleEvent
    */
    bool SendIdleEvents(wxWindow* win, wxIdleEvent& event);

    /**
        Set the application name to be used in the user-visible places such as window
        titles. See GetAppDisplayName() for more about
        the differences between the display name and name.
    */
    void SetAppDisplayName(const wxString& name);

    /**
        Sets the name of the application. This name should be used for file names,
        configuration file entries and other internal strings. For the user-visible
        strings, such as the window titles, the application display name set by
        SetAppDisplayName() is used instead.
        
        By default the application name is set to the name of its executable file.
        
        @sa GetAppName()
    */
    void SetAppName(const wxString& name);

    /**
        Sets the class name of the application. This may be used in a platform specific
        manner to refer to the application.
        
        @sa GetClassName()
    */
    void SetClassName(const wxString& name);

    /**
        Allows the programmer to specify whether the application will exit when the
        top-level frame is deleted.
        
        @param flag
        If @true (the default), the application will exit when the top-level frame is
        deleted. If @false, the application will continue to run.
        
        @sa GetExitOnFrameDelete(), @ref overview_wxappshutdownoverview "wxApp
              shutdown overview"
    */
    void SetExitOnFrameDelete(bool flag);

    /**
        Allows external code to modify global @c wxTheApp, but you should really
        know what you're doing if you call it.
        
        @param app
        Replacement for the global application object.
        
        @sa GetInstance()
    */
    static void SetInstance(wxAppConsole* app);

    /**
        Allows runtime switching of the UI environment theme. Currently implemented for
        wxGTK2-only.
        
        Return @true if theme was successfully changed.
        
        @param theme
        The name of the new theme or an absolute path to a gtkrc-theme-file
    */
    bool SetNativeTheme();

    /**
        Sets the 'top' window. You can call this from within OnInit() to
        let wxWidgets know which is the main window. You don't have to set the top
        window;
        it is only a convenience so that (for example) certain dialogs without parents
        can use a
        specific window as the top window. If no top window is specified by the
        application,
        wxWidgets just uses the first frame or dialog in its top-level window list,
        when it
        needs to use the top window.
        
        @param window
        The new top window.
        
        @sa GetTopWindow(), OnInit()
    */
    void SetTopWindow(wxWindow* window);

    /**
        Allows the programmer to specify whether the application will use the best
        visual
        on systems that support several visual on the same display. This is typically
        the
        case under Solaris and IRIX, where the default visual is only 8-bit whereas
        certain
        applications are supposed to run in TrueColour mode.
        
        If @e forceTrueColour is @true then the application will try to force
        using a TrueColour visual and abort the app if none is found.
        
        Note that this function has to be called in the constructor of the @c wxApp
        instance and won't have any effect when called later on.
        
        This function currently only has effect under GTK.
        
        @param flag
        If @true, the app will use the best visual.
    */
    void SetUseBestVisual(bool flag, bool forceTrueColour = @false);

    /**
        Set the vendor name to be used in the user-visible places. See
        GetVendorDisplayName() for more about
        the differences between the display name and name.
    */
    void SetVendorDisplayName(const wxString& name);

    /**
        Sets the name of application's vendor. The name will be used
        in registry access. A default name is set by
        wxWidgets.
        
        @sa GetVendorName()
    */
    void SetVendorName(const wxString& name);

    /**
        Yields control to pending messages in the windowing system. This can be useful,
        for example, when a
        time-consuming process writes to a text window. Without an occasional
        yield, the text window will not be updated properly, and on systems with
        cooperative multitasking, such as Windows 3.1 other processes will not respond.
        
        Caution should be exercised, however, since yielding may allow the
        user to perform actions which are not compatible with the current task.
        Disabling menu items or whole menus during processing can avoid unwanted
        reentrance of code: see ::wxSafeYield for a better
        function.
        
        Note that Yield() will not flush the message logs. This is intentional as
        calling Yield() is usually done to quickly update the screen and popping up a
        message box dialog may be undesirable. If you do wish to flush the log
        messages immediately (otherwise it will be done during the next idle loop
        iteration), call wxLog::FlushActive.
        
        Calling Yield() recursively is normally an error and an assert failure is
        raised in debug build if such situation is detected. However if the
        @e onlyIfNeeded parameter is @true, the method will just silently
        return @false instead.
    */
    bool Yield(bool onlyIfNeeded = @false);

    /**
        int argc
        
        Number of command line arguments (after environment-specific processing).
    */


    /**
        wxChar ** argv
        
        Command line arguments (after environment-specific processing).
        Under Windows and Linux/Unix, you should parse the command line
        arguments and check for files to be opened when starting your
        application. Under OS X, you need to override MacOpenFile()
        since command line arguments are used differently there.
        
        You may use the wxCmdLineParser to
        parse command line arguments.
    */
};


// ============================================================================
// Global functions/macros
// ============================================================================

//@{
/**
    For all normal, informational messages. They also appear in a message box by
    default (but it can be changed).
*/
void wxLogMessage(const char * formatString, ... );
void wxVLogMessage(const char * formatString, va_list argPtr);
//@}

//@{
/**
    For verbose output. Normally, it is suppressed, but
    might be activated if the user wishes to know more details about the program
    progress (another, but possibly confusing name for the same function is @b
    wxLogInfo).
*/
void wxLogVerbose(const char * formatString, ... );
void wxVLogVerbose(const char * formatString, va_list argPtr);
//@}

/**
    This is used in headers to create a forward declaration of the
    wxGetApp function implemented by
    wxIMPLEMENT_APP. It creates the declaration
    @c className wxGetApp(void).

    Example:
    @code
    wxDECLARE_APP(MyApp)
    @endcode
*/
#define wxDECLARE_APP()     /* implementation is private */

/**
    Exits application after calling wxApp::OnExit.
    Should only be used in an emergency: normally the top-level frame
    should be deleted (after deleting all other frames) to terminate the
    application. See wxCloseEvent and wxApp.
*/
void wxExit();

//@{
/**
    For warnings - they are also normally shown to the user, but don't interrupt
    the program work.
*/
void wxLogWarning(const char * formatString, ... );
void wxVLogWarning(const char * formatString, va_list argPtr);
//@}

//@{
/**
    Like wxLogError, but also
    terminates the program with the exit code 3. Using @e abort() standard
    function also terminates the program with this exit code.
*/
void wxLogFatalError(const char * formatString, ... );
void wxVLogFatalError(const char * formatString,
                      va_list argPtr);
//@}

/**
    If @e doIt is @true, the fatal exceptions (also known as general protection
    faults under Windows or segmentation violations in the Unix world) will be
    caught and passed to wxApp::OnFatalException.
    By default, i.e. before this function is called, they will be handled in the
    normal way which usually just means that the application will be terminated.
    Calling wxHandleFatalExceptions() with @e doIt equal to @false will restore
    this default behaviour.

    Notice that this function is only available if
    @c wxUSE_ON_FATAL_EXCEPTION is 1 and under Windows platform this
    requires a compiler with support for SEH (structured exception handling) which
    currently means only Microsoft Visual C++ or a recent Borland C++ version.
*/
bool wxHandleFatalExceptions(bool doIt = @true);

/**
    This is used in the application class implementation file to make the
    application class known to
    wxWidgets for dynamic construction. You use this instead of

    Old form:
    @code
    MyApp myApp;
    @endcode

    New form:
    @code
    IMPLEMENT_APP(MyApp)
    @endcode

    See also DECLARE_APP.
*/
#define IMPLEMENT_APP()     /* implementation is private */

/**
    Returns the error code from the last system call. This function uses
    @c errno on Unix platforms and @c GetLastError under Win32.

    @sa wxSysErrorMsg, wxLogSysError
*/
unsigned long wxSysErrorCode();

/**
    In a GUI application, this function posts @e event to the specified @e dest
    object using wxEvtHandler::AddPendingEvent.
    Otherwise, it dispatches @e event immediately using
    wxEvtHandler::ProcessEvent.
    See the respective documentation for details (and caveats).
*/
void wxPostEvent(wxEvtHandler * dest, wxEvent& event);

//@{
/**
    The functions to use for error messages, i.e. the messages that must be shown
    to the user. The default processing is to pop up a message box to inform the
    user about it.
*/
void wxLogError(const char * formatString, ... );
void wxVLogError(const char * formatString, va_list argPtr);
//@}

//@{
/**
    As @b wxLogDebug, trace functions only do something in debug build and
    expand to nothing in the release one. The reason for making
    it a separate function from it is that usually there are a lot of trace
    messages, so it might make sense to separate them from other debug messages.

    The trace messages also usually can be separated into different categories and
    the second and third versions of this function only log the message if the
    @e mask which it has is currently enabled in wxLog. This
    allows to selectively trace only some operations and not others by changing
    the value of the trace mask (possible during the run-time).

    For the second function (taking a string mask), the message is logged only if
    the mask has been previously enabled by the call to
    wxLog::AddTraceMask or by setting
    @ref overview_envvars "@c WXTRACE environment variable".
    The predefined string trace masks
    used by wxWidgets are:

     wxTRACE_MemAlloc: trace memory allocation (new/delete)
     wxTRACE_Messages: trace window messages/X callbacks
     wxTRACE_ResAlloc: trace GDI resource allocation
     wxTRACE_RefCount: trace various ref counting operations
     wxTRACE_OleCalls: trace OLE method calls (Win32 only)

    @b Caveats: since both the mask and the format string are strings,
    this might lead to function signature confusion in some cases:
    if you intend to call the format string only version of wxLogTrace,
    then add a %s format string parameter and then supply a second string parameter
    for that %s, the string mask version of wxLogTrace will erroneously get called instead, since you are supplying two string parameters to the function.
    In this case you'll unfortunately have to avoid having two leading
    string parameters, e.g. by adding a bogus integer (with its %d format string).

    The third version of the function only logs the message if all the bits
    corresponding to the @e mask are set in the wxLog trace mask which can be
    set by wxLog::SetTraceMask. This version is less
    flexible than the previous one because it doesn't allow defining the user
    trace masks easily - this is why it is deprecated in favour of using string
    trace masks.

     wxTraceMemAlloc: trace memory allocation (new/delete)
     wxTraceMessages: trace window messages/X callbacks
     wxTraceResAlloc: trace GDI resource allocation
     wxTraceRefCount: trace various ref counting operations
     wxTraceOleCalls: trace OLE method calls (Win32 only)
*/
void wxLogTrace(const char * formatString, ... );
void wxVLogTrace(const char * formatString, va_list argPtr);
void wxLogTrace(const char * mask, const char * formatString,
                ... );
void wxVLogTrace(const char * mask,
                 const char * formatString,
                 va_list argPtr);
void wxLogTrace(wxTraceMask mask, const char * formatString,
                ... );
void wxVLogTrace(wxTraceMask mask, const char * formatString,
                 va_list argPtr);
//@}

/**
    Returns the error message corresponding to the given system error code. If
    @e errCode is 0 (default), the last error code (as returned by
    wxSysErrorCode) is used.

    @sa wxSysErrorCode, wxLogSysError
*/
const wxChar * wxSysErrorMsg(unsigned long errCode = 0);

/**
    This function is for use in console (wxBase) programs only. It must be called
    once for each previous successful call to wxInitialize.
*/
void wxUninitialize();

//@{
/**
    The right functions for debug output. They only do something in debug
    mode (when the preprocessor symbol __WXDEBUG__ is defined) and expand to
    nothing in release mode (otherwise).
*/
void wxLogDebug(const char * formatString, ... );
void wxVLogDebug(const char * formatString, va_list argPtr);
//@}

/**
    This function doesn't exist in wxWidgets but it is created by using
    the IMPLEMENT_APP macro. Thus, before using it
    anywhere but in the same module where this macro is used, you must make it
    available using DECLARE_APP.

    The advantage of using this function compared to directly using the global
    wxTheApp pointer is that the latter is of type @c wxApp * and so wouldn't
    allow you to access the functions specific to your application class but not
    present in wxApp while wxGetApp() returns the object of the right type.
*/
wxAppDerivedClass wxGetApp();

//@{
/**
    Messages logged by these functions will appear in the statusbar of the @e frame
    or of the top level application window by default (i.e. when using
    the second version of the functions).

    If the target frame doesn't have a statusbar, the message will be lost.
*/
void wxLogStatus(wxFrame * frame, const char * formatString,
                 ... );
void wxVLogStatus(wxFrame * frame, const char * formatString,
                  va_list argPtr);
void wxLogStatus(const char * formatString, ... );
void wxVLogStatus(const char * formatString, va_list argPtr);
//@}

/**
    This function is used in wxBase only and only if you don't create
    wxApp object at all. In this case you must call it from your
    @c main() function before calling any other wxWidgets functions.

    If the function returns @false the initialization could not be performed,
    in this case the library cannot be used and
    wxUninitialize shouldn't be called neither.

    This function may be called several times but
    wxUninitialize must be called for each successful
    call to this function.
*/
bool wxInitialize();

/**
    This is used in headers to create a forward declaration of the
    wxGetApp function implemented by
    IMPLEMENT_APP. It creates the declaration
    @c className wxGetApp(void).

    Example:
    @code
    DECLARE_APP(MyApp)
    @endcode
*/
#define DECLARE_APP()     /* implementation is private */

/**
    Calls wxApp::Yield.

    This function is kept only for backwards compatibility. Please use
    the wxApp::Yield method instead in any new code.
*/
bool wxYield();

//@{
/**
    Mostly used by wxWidgets itself, but might be handy for logging errors after
    system call (API function) failure. It logs the specified message text as well
    as the last system error code (@e errno or @e ::GetLastError() depending
    on the platform) and the corresponding error message. The second form
    of this function takes the error code explicitly as the first argument.

    @sa wxSysErrorCode, wxSysErrorMsg
*/
void wxLogSysError(const char * formatString, ... );
void wxVLogSysError(const char * formatString,
                    va_list argPtr);
//@}

//@{
/**
    This initializes wxWidgets in a platform-dependent way. Use this if you are not
    using the default wxWidgets entry code (e.g. main or WinMain). For example, you
    can initialize wxWidgets from an Microsoft Foundation Classes application using
    this function.

    The following overload of wxEntry is available under all platforms:

    (notice that under Windows CE platform, and only there, the type of
    @e pCmdLine is @c wchar_t *, otherwise it is @c char *, even in
    Unicode build).

    @remarks To clean up wxWidgets, call wxApp::OnExit followed by the static
               function wxApp::CleanUp. For example, if exiting from
               an MFC application that also uses wxWidgets:

    @sa wxEntryStart
*/
int wxEntry(int& argc, wxChar ** argv);
int wxEntry(HINSTANCE hInstance,
            HINSTANCE hPrevInstance = @NULL,
            char * pCmdLine = @NULL,
            int nCmdShow = SW_SHOWNORMAL);
//@}

