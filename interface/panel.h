/////////////////////////////////////////////////////////////////////////////
// Name:        panel.h
// Purpose:     documentation for wxPanel class
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxPanel
    @wxheader{panel.h}

    A panel is a window on which controls are placed. It is usually placed within
    a frame. Its main feature over its parent class wxWindow is code for handling
    child windows and TAB traversal. Since wxWidgets 2.9, there is support both
    for TAB traversal implemented by wxWidgets itself as well as native TAB
    traversal (such as for GTK 2.0).

    @e Note: Tab traversal is implemented through an otherwise undocumented
    intermediate wxControlContainer class from which any class can derive
    in addition to the normal wxWindow base class. Please see wx/containr.h
    and wx/panel.h to find out how this is achieved.

    @e Note: if not all characters are being intercepted by your OnKeyDown or
    OnChar handler, it may be because you are using the wxTAB_TRAVERSAL style,
    which grabs some keypresses for use by child controls.

    @library{wxbase}
    @category{miscwnd}

    @seealso
    wxDialog
*/
class wxPanel : public wxWindow
{
public:
    //@{
    /**
        Constructor.
        
        @param parent
            The parent window.
        @param id
            An identifier for the panel. A value of -1 is taken to mean a default.
        @param pos
            The panel position. The value wxDefaultPosition indicates a default position,
        chosen by
            either the windowing system or wxWidgets, depending on platform.
        @param size
            The panel size. The value wxDefaultSize indicates a default size, chosen by
            either the windowing system or wxWidgets, depending on platform.
        @param style
            The window style. See wxPanel.
        @param name
            Used to associate a name with the window,
            allowing the application user to set Motif resource values for
            individual dialog boxes.
        
        @see Create()
    */
    wxPanel();
    wxPanel(wxWindow* parent, wxWindowID id = wxID_ANY,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxTAB_TRAVERSAL,
            const wxString& name = "panel");
    //@}

    /**
        Destructor. Deletes any child windows before deleting the physical window.
    */
    ~wxPanel();

    /**
        This method is overridden from wxWindow::AcceptsFocus
        and returns @true only if there is no child window in the panel which
        can accept the focus. This is reevaluated each time a child
        window is added or removed from the panel.
    */
    bool AcceptsFocus();

    /**
        Used for two-step panel construction. See wxPanel()
        for details.
    */
    bool Create(wxWindow* parent, wxWindowID id = wxID_ANY,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxTAB_TRAVERSAL,
                const wxString& name = "panel");

    /**
        Sends a wxInitDialogEvent, which
        in turn transfers data to the dialog via validators.
        
        @see wxInitDialogEvent
    */
    void InitDialog();

    /**
        The default handler for wxEVT_SYS_COLOUR_CHANGED.
        
        @param event
            The colour change event.
        
        @remarks Changes the panel's colour to conform to the current settings
                 (Windows only). Add an event table entry for your panel
                 class if you wish the behaviour to be different (such
                 as keeping a user-defined background colour). If you do
                 override this function, call wxEvent::Skip to propagate
                 the notification to child windows and controls.
        
        @see wxSysColourChangedEvent
    */
    void OnSysColourChanged(wxSysColourChangedEvent& event);

    /**
        Overrides wxWindow::SetFocus. This method
        uses the (undocumented) mix-in class wxControlContainer which manages
        the focus and TAB logic for controls which usually have child controls.
        In practice, if you call this method and the control has at least
        one child window, the focus will be given to the child window.
        
        @see wxFocusEvent, wxWindow::SetFocus
    */
    virtual void SetFocus();

    /**
        In contrast to SetFocus() (see above)
        this will set the focus to the panel even if there are child windows
        in the panel. This is only rarely needed.
    */
    virtual void SetFocusIgnoringChildren();
};
