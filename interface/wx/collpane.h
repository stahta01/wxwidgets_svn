/////////////////////////////////////////////////////////////////////////////
// Name:        collpane.h
// Purpose:     interface of wxCollapsiblePane
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#define wxCP_DEFAULT_STYLE          (wxTAB_TRAVERSAL | wxNO_BORDER)
#define wxCP_NO_TLW_RESIZE          (0x0002)

/**
    @class wxCollapsiblePaneEvent

    This event class is used for the events generated by wxCollapsiblePane.

    @beginEventTable{wxCollapsiblePaneEvent}
    @event{EVT_COLLAPSIBLEPANE_CHANGED(id, func)}
           The user expanded or collapsed the collapsible pane.
    @endEventTable

    @library{wxcore}
    @category{events}

    @see wxCollapsiblePane
*/
class wxCollapsiblePaneEvent : public wxCommandEvent
{
public:
    /**
        The constructor is not normally used by the user code.
    */
    wxCollapsiblePaneEvent(wxObject* generator, int id, bool collapsed);

    /**
        Returns @true if the pane has been collapsed.
    */
    bool GetCollapsed() const;

    /**
        Sets this as a collapsed pane event (if @a collapsed is @true) or as an
        expanded pane event (if @a collapsed is @false).
    */
    void SetCollapsed(bool collapsed);
};

wxEventType wxEVT_COMMAND_COLLPANE_CHANGED;

/**
    @class wxCollapsiblePane

    A collapsible pane is a container with an embedded button-like control
    which can be used by the user to collapse or expand the pane's contents.

    Once constructed you should use the GetPane() function to access the pane
    and add your controls inside it (i.e. use the returned pointer from
    GetPane() as parent for the controls which must go in the pane, @b not the
    wxCollapsiblePane itself!).

    Note that because of its nature of control which can dynamically (and
    drastically) change its size at run-time under user-input, when putting
    wxCollapsiblePane inside a wxSizer you should be careful to add it with a
    proportion value of zero; this is because otherwise all other windows with
    non-null proportion values will automatically resize each time the user
    expands or collapse the pane window usually resulting in a weird,
    flickering effect.

    Usage sample:

    @code
    wxCollapsiblePane *collpane = new wxCollapsiblePane(this, wxID_ANY, "Details:");

    // add the pane with a zero proportion value to the 'sz' sizer which contains it
    sz->Add(collpane, 0, wxGROW|wxALL, 5);

    // now add a test label in the collapsible pane using a sizer to layout it:
    wxWindow *win = collpane->GetPane();
    wxSizer *paneSz = new wxBoxSizer(wxVERTICAL);
    paneSz->Add(new wxStaticText(win, wxID_ANY, "test!"), 1, wxGROW|wxALL, 2);
    win->SetSizer(paneSz);
    paneSz->SetSizeHints(win);
    @endcode

    It is only available if @c wxUSE_COLLPANE is set to 1 (the default).

    @beginStyleTable
    @style{wxCP_DEFAULT_STYLE}
        The default style. It includes wxTAB_TRAVERSAL and wxBORDER_NONE.
    @style{wxCP_NO_TLW_RESIZE}
        By default wxCollapsiblePane resizes the top level window containing it
        when its own size changes. This allows to easily implement dialogs
        containing an optionally shown part, for example, and so is the default
        behaviour but can be inconvenient in some specific cases -- use this
        flag to disable this automatic parent resizing then.
    @endStyleTable

    @beginEventEmissionTable{wxCollapsiblePaneEvent,wxNavigationKeyEvent}
    @event{EVT_COLLAPSIBLEPANE_CHANGED(id, func)}
        The user expanded or collapsed the collapsible pane.
    @event{EVT_NAVIGATION_KEY(func)}
        Process a navigation key event.
    @endEventTable

    @library{wxcore}
    @category{ctrl}
    @appearance{collapsiblepane.png}

    @see wxPanel, wxCollapsiblePaneEvent
*/
class wxCollapsiblePane : public wxControl
{
public:
    /**
        Default constructor.
    */
    wxCollapsiblePane();

    /**
        Initializes the object and calls Create() with all the parameters.
    */
    wxCollapsiblePane(wxWindow* parent, wxWindowID id,
                      const wxString& label,
                      const wxPoint& pos = wxDefaultPosition,
                      const wxSize& size = wxDefaultSize,
                      long style = wxCP_DEFAULT_STYLE,
                      const wxValidator& validator = wxDefaultValidator,
                      const wxString& name = wxCollapsiblePaneNameStr);

    /**
        @param parent
            Parent window, must not be non-@NULL.
        @param id
            The identifier for the control.
        @param label
            The initial label shown in the button which allows the user to
            expand or collapse the pane window.
        @param pos
            Initial position.
        @param size
            Initial size.
        @param style
            The window style, see wxCP_* flags.
        @param validator
            Validator which can be used for additional date checks.
        @param name
            Control name.

        @return @true if the control was successfully created or @false if
                creation failed.
    */
    bool Create(wxWindow* parent, wxWindowID id,
                const wxString& label,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxCP_DEFAULT_STYLE,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = wxCollapsiblePaneNameStr);

    /**
        Collapses or expands the pane window.
    */
    virtual void Collapse(bool collapse = true);

    /**
        Same as calling Collapse(@false).
    */
    void Expand();

    /**
        Returns a pointer to the pane window. Add controls to the returned
        wxWindow to make them collapsible.
    */
    virtual wxWindow* GetPane() const;

    /**
        Returns @true if the pane window is currently hidden.
    */
    virtual bool IsCollapsed() const;

    /**
        Returns @true if the pane window is currently shown.
    */
    bool IsExpanded() const;
};
