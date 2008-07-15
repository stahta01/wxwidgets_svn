/////////////////////////////////////////////////////////////////////////////
// Name:        frame.h
// Purpose:     interface of wxFrame
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxFrame

    A frame is a window whose size and position can (usually) be changed by the user.

    It usually has thick borders and a title bar, and can optionally contain a
    menu bar, toolbar and status bar. A frame can contain any window that is not
    a frame or dialog.

    A frame that has a status bar and toolbar, created via the CreateStatusBar() and
    CreateToolBar() functions, manages these windows and adjusts the value returned
    by GetClientSize() to reflect the remaining size available to application windows.

    @remarks An application should normally define an wxCloseEvent handler for the
             frame to respond to system close events, for example so that related
             data and subwindows can be cleaned up.


    @section wxframe_defaultevent Default event processing

    wxFrame processes the following events:

    @li @c wxEVT_SIZE: if the frame has exactly one child window, not counting the
        status and toolbar, this child is resized to take the entire frame client area.
        If two or more windows are present, they should be laid out explicitly either
        by manually handling wxEVT_SIZE or using sizers;
    @li @c wxEVT_MENU_HIGHLIGHT: the default implementation displays the help string
        associated with the selected item in the first pane of the status bar, if there is one.


    @beginStyleTable
    @style{wxDEFAULT_FRAME_STYLE}
           Defined as wxMINIMIZE_BOX |  wxMAXIMIZE_BOX |  wxRESIZE_BORDER |
           wxSYSTEM_MENU |  wxCAPTION |  wxCLOSE_BOX |  wxCLIP_CHILDREN.
    @style{wxICONIZE}
           Display the frame iconized (minimized). Windows only.
    @style{wxCAPTION}
           Puts a caption on the frame.
    @style{wxMINIMIZE}
           Identical to wxICONIZE. Windows only.
    @style{wxMINIMIZE_BOX}
           Displays a minimize box on the frame.
    @style{wxMAXIMIZE}
           Displays the frame maximized. Windows only.
    @style{wxMAXIMIZE_BOX}
           Displays a maximize box on the frame.
    @style{wxCLOSE_BOX}
           Displays a close box on the frame.
    @style{wxSTAY_ON_TOP}
           Stay on top of all other windows, see also wxFRAME_FLOAT_ON_PARENT.
    @style{wxSYSTEM_MENU}
           Displays a system menu.
    @style{wxRESIZE_BORDER}
           Displays a resizeable border around the window.
    @style{wxFRAME_TOOL_WINDOW}
           Causes a frame with a small titlebar to be created; the frame does
           not appear in the taskbar under Windows or GTK+.
    @style{wxFRAME_NO_TASKBAR}
           Creates an otherwise normal frame but it does not appear in the
           taskbar under Windows or GTK+ (note that it will minimize to the
           desktop window under Windows which may seem strange to the users
           and thus it might be better to use this style only without
           wxMINIMIZE_BOX style). In wxGTK, the flag is respected only if GTK+
           is at least version 2.2 and the window manager supports
           _NET_WM_STATE_SKIP_TASKBAR hint. Has no effect under other platforms.
    @style{wxFRAME_FLOAT_ON_PARENT}
           The frame will always be on top of its parent (unlike wxSTAY_ON_TOP).
           A frame created with this style must have a non-@NULL parent.
    @style{wxFRAME_SHAPED}
           Windows with this style are   allowed to have their shape changed
           with the SetShape() method.
    @endStyleTable

    The default frame style is for normal, resizeable frames.
    To create a frame which can not be resized by user, you may use the following
    combination of styles:

    @code
        wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)
    @endcode

    See also the @ref overview_windowstyles.

    @beginExtraStyleTable
    @style{wxFRAME_EX_CONTEXTHELP}
           Under Windows, puts a query button on the caption. When pressed,
           Windows will go into a context-sensitive help mode and wxWidgets
           will send a wxEVT_HELP event if the user clicked on an application
           window. Note that this is an extended style and must be set by
           calling SetExtraStyle before Create is called (two-step
           construction). You cannot use this style together with
           wxMAXIMIZE_BOX or wxMINIMIZE_BOX, so you should use
           wxDEFAULT_FRAME_STYLE  ~ (wxMINIMIZE_BOX | wxMAXIMIZE_BOX) for the
           frames having this style (the dialogs don't have a minimize or a
           maximize box by default)
    @style{wxFRAME_EX_METAL}
           On Mac OS X, frames with this style will be shown with a metallic
           look. This is an extra style.
    @endExtraStyleTable

    @library{wxcore}
    @category{managedwnd}

    @see wxMDIParentFrame, wxMDIChildFrame, wxMiniFrame, wxDialog
*/
class wxFrame : public wxTopLevelWindow
{
public:
    //@{
    /**
        Constructor, creating the window.

        @param parent
            The window parent. This may be @NULL. If it is non-@NULL, the frame will
            always be displayed on top of the parent window on Windows.
        @param id
            The window identifier. It may take a value of -1 to indicate a default
        value.
        @param title
            The caption to be displayed on the frame's title bar.
        @param pos
            The window position. The value wxDefaultPosition indicates a default position,
        chosen by
            either the windowing system or wxWidgets, depending on platform.
        @param size
            The window size. The value wxDefaultSize indicates a default size, chosen by
            either the windowing system or wxWidgets, depending on platform.
        @param style
            The window style. See wxFrame.
        @param name
            The name of the window. This parameter is used to associate a name with the
        item,
            allowing the application user to set Motif resource values for
            individual windows.

        @remarks For Motif, MWM (the Motif Window Manager) should be running for
                 any window styles to work (otherwise all styles take
                 effect).

        @see Create()
    */
    wxFrame();
    wxFrame(wxWindow* parent, wxWindowID id,
            const wxString& title,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxDEFAULT_FRAME_STYLE,
            const wxString& name = "frame");
    //@}

    /**
        Destructor. Destroys all child windows and menu bar if present.
    */
    ~wxFrame();

    /**
        Centres the frame on the display.

        @param direction
            The parameter may be wxHORIZONTAL, wxVERTICAL or wxBOTH.
    */
    void Centre(int direction = wxBOTH);

    /**
        Used in two-step frame construction. See wxFrame()
        for further details.
    */
    bool Create(wxWindow* parent, wxWindowID id,
                const wxString& title,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxDEFAULT_FRAME_STYLE,
                const wxString& name = "frame");

    /**
        Creates a status bar at the bottom of the frame.

        @param number
            The number of fields to create. Specify a
            value greater than 1 to create a multi-field status bar.
        @param style
            The status bar style. See wxStatusBar for a list
            of valid styles.
        @param id
            The status bar window identifier. If -1, an identifier will be chosen by
            wxWidgets.
        @param name
            The status bar window name.

        @return A pointer to the status bar if it was created successfully, @NULL
                 otherwise.

        @remarks The width of the status bar is the whole width of the frame
                 (adjusted automatically when resizing), and the height
                 and text size are chosen by the host windowing system.

        @see SetStatusText(), OnCreateStatusBar(), GetStatusBar()
    */
    virtual wxStatusBar* CreateStatusBar(int number = 1,
                                         long style = 0,
                                         wxWindowID id = -1,
                                         const wxString& name = "statusBar");

    /**
        Creates a toolbar at the top or left of the frame.

        @param style
            The toolbar style. See wxToolBar for a list
            of valid styles.
        @param id
            The toolbar window identifier. If -1, an identifier will be chosen by
            wxWidgets.
        @param name
            The toolbar window name.

        @return A pointer to the toolbar if it was created successfully, @NULL
                 otherwise.

        @remarks By default, the toolbar is an instance of wxToolBar (which is
                 defined to be a suitable toolbar class on each
                 platform, such as wxToolBar95). To use a different
                 class, override OnCreateToolBar().

        @see CreateStatusBar(), OnCreateToolBar(), SetToolBar(),
             GetToolBar()
    */
    virtual wxToolBar* CreateToolBar(long style = wxBORDER_NONE | wxTB_HORIZONTAL,
                                     wxWindowID id = -1,
                                     const wxString& name = "toolBar");

    /**
        Returns the origin of the frame client area (in client coordinates). It may be
        different from (0, 0) if the frame has a toolbar.
    */
    wxPoint GetClientAreaOrigin() const;

    /**
        Returns a pointer to the menubar currently associated with the frame (if any).

        @see SetMenuBar(), wxMenuBar, wxMenu
    */
    wxMenuBar* GetMenuBar() const;

    /**
        Returns a pointer to the status bar currently associated with the frame (if
        any).

        @see CreateStatusBar(), wxStatusBar
    */
    wxStatusBar* GetStatusBar() const;

    /**
        Returns the status bar pane used to display menu and toolbar help.

        @see SetStatusBarPane()
    */
    int GetStatusBarPane();

    /**
        Returns a pointer to the toolbar currently associated with the frame (if any).

        @see CreateToolBar(), wxToolBar, SetToolBar()
    */
    wxToolBar* GetToolBar() const;

    /**
        Virtual function called when a status bar is requested by CreateStatusBar().

        @param number
            The number of fields to create.
        @param style
            The window style. See wxStatusBar for a list
            of valid styles.
        @param id
            The window identifier. If -1, an identifier will be chosen by
            wxWidgets.
        @param name
            The window name.

        @return A status bar object.

        @remarks An application can override this function to return a different
                 kind of status bar. The default implementation returns
                 an instance of wxStatusBar.

        @see CreateStatusBar(), wxStatusBar.
    */
    virtual wxStatusBar* OnCreateStatusBar(int number, long style,
                                           wxWindowID id,
                                           const wxString& name);

    /**
        Virtual function called when a toolbar is requested by CreateToolBar().

        @param style
            The toolbar style. See wxToolBar for a list
            of valid styles.
        @param id
            The toolbar window identifier. If -1, an identifier will be chosen by
            wxWidgets.
        @param name
            The toolbar window name.

        @return A toolbar object.

        @remarks An application can override this function to return a different
                 kind of toolbar. The default implementation returns an
                 instance of wxToolBar.

        @see CreateToolBar(), wxToolBar.
    */
    virtual wxToolBar* OnCreateToolBar(long style, wxWindowID id,
                                       const wxString& name);

    /**
        Simulate a menu command.

        @param id
            The identifier for a menu item.
    */
    void ProcessCommand(int id);

    /**
        This function sends a dummy @ref overview_wxsizeevent "size event" to the frame
        forcing it to reevaluate its children positions. It is sometimes useful to call
        this function after adding or deleting a children after the frame creation or
        if a child size changes.
        Note that if the frame is using either sizers or constraints for the children
        layout, it is enough to call wxWindow::Layout directly and
        this function should not be used in this case.
    */
    void SendSizeEvent();

    /**
        Tells the frame to show the given menu bar.

        @param menuBar
            The menu bar to associate with the frame.

        @remarks If the frame is destroyed, the menu bar and its menus will be
                 destroyed also, so do not delete the menu bar
                 explicitly (except by resetting the frame's menu bar to
                 another frame or @NULL).

        @see GetMenuBar(), wxMenuBar, wxMenu.
    */
    void SetMenuBar(wxMenuBar* menuBar);

    /**
        Associates a status bar with the frame.

        @see CreateStatusBar(), wxStatusBar, GetStatusBar()
    */
    void SetStatusBar(wxStatusBar* statusBar);

    /**
        Set the status bar pane used to display menu and toolbar help.
        Using -1 disables help display.
    */
    void SetStatusBarPane(int n);

    /**
        Sets the status bar text and redraws the status bar.

        @param text
            The text for the status field.
        @param number
            The status field (starting from zero).

        @remarks Use an empty string to clear the status bar.

        @see CreateStatusBar(), wxStatusBar
    */
    virtual void SetStatusText(const wxString& text, int number = 0);

    /**
        Sets the widths of the fields in the status bar.

        @param n
            The number of fields in the status bar. It must be the
            same used in CreateStatusBar.
        @param widths
            Must contain an array of n integers, each of which is a status field width
            in pixels. A value of -1 indicates that the field is variable width; at
        least one
            field must be -1. You should delete this array after calling
        SetStatusWidths.

        @remarks The widths of the variable fields are calculated from the total
                 width of all fields, minus the sum of widths of the
                 non-variable fields, divided by the number of variable
                 fields.
    */
    virtual void SetStatusWidths(int n, int* widths);

    /**
        Associates a toolbar with the frame.
    */
    void SetToolBar(wxToolBar* toolBar);
};

