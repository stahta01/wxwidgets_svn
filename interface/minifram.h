/////////////////////////////////////////////////////////////////////////////
// Name:        minifram.h
// Purpose:     interface of wxMiniFrame
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxMiniFrame
    @wxheader{minifram.h}

    A miniframe is a frame with a small title bar. It is suitable for floating
    toolbars that must not
    take up too much screen area.

    An example of mini frame can be seen in the @ref overview_sampledialogs
    "dialogs sample"
    using the "Mini frame'' command of the "Generic dialogs'' submenu.

    @beginStyleTable
    @style{wxICONIZE}:
           Display the frame iconized (minimized) (Windows only).
    @style{wxCAPTION}:
           Puts a caption on the frame.
    @style{wxMINIMIZE}:
           Identical to wxICONIZE.
    @style{wxMINIMIZE_BOX}:
           Displays a minimize box on the frame (Windows and Motif only).
    @style{wxMAXIMIZE}:
           Displays the frame maximized (Windows only).
    @style{wxMAXIMIZE_BOX}:
           Displays a maximize box on the frame (Windows and Motif only).
    @style{wxCLOSE_BOX}:
           Displays a close box on the frame.
    @style{wxSTAY_ON_TOP}:
           Stay on top of other windows (Windows only).
    @style{wxSYSTEM_MENU}:
           Displays a system menu (Windows and Motif only).
    @style{wxTINY_CAPTION_HORIZ}:
           This style is obsolete and not used any longer.
    @style{wxTINY_CAPTION_VERT}:
           This style is obsolete and not used any longer.
    @style{wxRESIZE_BORDER}:
           Displays a resizeable border around the window.
    @endStyleTable

    @library{wxcore}
    @category{managedwnd}

    @see wxMDIParentFrame, wxMDIChildFrame, wxFrame, wxDialog
*/
class wxMiniFrame : public wxFrame
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
            The window style. See wxMiniFrame.
        @param name
            The name of the window. This parameter is used to associate a name with the
        item,
            allowing the application user to set Motif resource values for
            individual windows.

        @remarks The frame behaves like a normal frame on non-Windows platforms.

        @see Create()
    */
    wxMiniFrame();
    wxMiniFrame(wxWindow* parent, wxWindowID id,
                const wxString& title,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxCAPTION | wxRESIZE_BORDER,
                const wxString& name = "frame");
    //@}

    /**
        Destructor. Destroys all child windows and menu bar if present.
    */
    ~wxMiniFrame();

    /**
        Used in two-step frame construction. See wxMiniFrame()
        for further details.
    */
    bool Create(wxWindow* parent, wxWindowID id,
                const wxString& title,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxCAPTION | wxRESIZE_BORDER,
                const wxString& name = "frame");
};

