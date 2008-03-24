/////////////////////////////////////////////////////////////////////////////
// Name:        apptrait.h
// Purpose:     interface of wxAppTraits
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxAppTraits
    @wxheader{apptrait.h}

    The wxAppTraits class defines various configurable aspects of a wxApp.
    You can access it using wxApp::GetTraits() function and you can create your
    own wxAppTraits overriding the wxApp::CreateTraits() function.

    Note that wxAppTraits is an abstract class since it contains many
    pure virtual functions.
    In fact, by default, wxWidgets creates a @c wxConsoleAppTraits object for
    console applications (i.e. those applications linked against wxBase library
    only - see the @ref page_libs page) and a @c wxGUIAppTraits object for GUI
    applications.
    Both these classes are derived by wxAppTraits and represent concrete
    implementation of the wxAppTraits interface.

    @library{wxbase}
    @category{appmanagement}

    @see @ref overview_app, wxApp
*/
class wxAppTraits
{
public:
    /**
        Called by wxWidgets to create the default configuration object for the
        application. The default version creates a registry-based wxRegConfig
        class under MSW and wxFileConfig under all other platforms.

        The wxApp::GetAppName and wxApp::GetVendorName methods are used to
        determine the registry key or file name.
    */
    virtual wxConfigBase* CreateConfig();

    /**
        Creates the global font mapper object used for encodings/charset mapping.
    */
    virtual wxFontMapper* CreateFontMapper() = 0;

    /**
        Creates a wxLog class for the application to use for logging errors.
        The default implementation returns a new wxLogGui class.

        @see wxLog
    */
    virtual wxLog* CreateLogTarget() = 0;

    /**
        Creates the global object used for printing out messages.
    */
    virtual wxMessageOutput* CreateMessageOutput() = 0;

    /**
        Returns the renderer to use for drawing the generic controls (return
        value may be @NULL in which case the default renderer for the current
        platform is used); this is used in GUI mode only and always returns @NULL
        in console.

        @note the returned pointer needs to be deleted by the caller.
    */
    virtual wxRendererNative* CreateRenderer() = 0;

    /**
        This method returns the name of the desktop environment currently
        running in a Unix desktop. Currently only "KDE" or "GNOME" are
        supported and the code uses the X11 session protocol vendor name
        to figure out, which desktop environment is running. The method
        returns an empty string otherwise and on all other platforms.
    */
    virtual wxString GetDesktopEnvironment() const = 0;

    /**
        Returns the wxStandardPaths object for the application.
        It's normally the same for wxBase and wxGUI except in the case of wxMac
        and wxCocoa.

        @todo the real function returns a reference to wxStandardPathsBase;
              user looking at these docs will write code:
                    wxStandardPaths &ref = ...->GetStandardPaths();
              which won't compile...
    */
    virtual wxStandardPaths& GetStandardPaths();

    /**
        Returns the wxWidgets port ID used by the running program and eventually
        fills the given pointers with the values of the major and minor digits
        of the native toolkit currently used.

        The version numbers returned are thus detected at run-time and not compile-time
        (except when this is not possible e.g. wxMotif).

        E.g. if your program is using wxGTK port this function will return wxPORT_GTK
        and put in given pointers the versions of the GTK library in use.
        See wxPlatformInfo for more details.
    */
    virtual wxPortId GetToolkitVersion(int* major = NULL, int* minor = NULL) const = 0;

    /**
        Returns @true if @c fprintf(stderr) goes somewhere, @false otherwise.
    */
    virtual bool HasStderr() = 0;

    /**
        Returns @true if the library was built as wxUniversal.
        Always returns @false for wxBase-only apps.
    */
    virtual bool IsUsingUniversalWidgets() const = 0;

    /**
        Shows the assert dialog with the specified message in GUI mode or just prints
        the string to stderr in console mode.
        Returns @true to suppress subsequent asserts, @false to continue as before.
    */
    virtual bool ShowAssertDialog(const wxString& msg) = 0;
};

