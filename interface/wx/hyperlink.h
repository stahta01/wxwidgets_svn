/////////////////////////////////////////////////////////////////////////////
// Name:        hyperlink.h
// Purpose:     interface of wxHyperlinkEvent
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxHyperlinkEvent

    This event class is used for the events generated by wxHyperlinkCtrl.

    @beginEventTable{wxHyperlinkEvent}
    @event{EVT_HYPERLINK(id, func)}
        User clicked on an hyperlink.
    @endEventTable

    @library{wxadv}
    @category{events}
*/
class wxHyperlinkEvent : public wxCommandEvent
{
public:
    /**
        The constructor is not normally used by the user code.
    */
    wxHyperlinkEvent(wxObject* generator, int id, const wxString& url);

    /**
        Returns the URL of the hyperlink where the user has just clicked.
    */
    wxString GetURL() const;

    /**
        Sets the URL associated with the event.
    */
    void SetURL(const wxString& url);
};



/**
    @class wxHyperlinkCtrl

    This class shows a static text element which links to an URL.
    Appearance and behaviour is completely customizable.

    In fact, when the user clicks on the hyperlink, a wxHyperlinkEvent is
    sent but if that event is not handled (or it's skipped; see wxEvent::Skip),
    then a call to wxLaunchDefaultBrowser() is done with the hyperlink's URL.

    Note that standard wxWindow functions like wxWindow::SetBackgroundColour,
    wxWindow::SetFont, wxWindow::SetCursor, wxWindow::SetLabel can be used to
    customize appearance of the hyperlink.

    @beginStyleTable
    @style{wxHL_ALIGN_LEFT}
           Align the text to the left.
    @style{wxHL_ALIGN_RIGHT}
           Align the text to the right.
    @style{wxHL_ALIGN_CENTRE}
           Center the text (horizontally).
    @style{wxHL_CONTEXTMENU}
           Pop up a context menu when the hyperlink is right-clicked. The
           context menu contains a "Copy URL" menu item which is automatically
           handled by the hyperlink and which just copies in the clipboard the
           URL (not the label) of the control.
    @style{wxHL_DEFAULT_STYLE}
           The default style for wxHyperlinkCtrl:
           wxBORDER_NONE|wxHL_CONTEXTMENU|wxHL_ALIGN_CENTRE.
    @endStyleTable

    @beginEventEmissionTable{wxHyperlinkEvent}
    @event{EVT_HYPERLINK(id, func)}
        The hyperlink was (left) clicked. If this event is not handled in user's
        code (or it's skipped; see wxEvent::Skip), then a call to wxLaunchDefaultBrowser
        is done with the hyperlink's URL.
    @endEventTable


    @library{wxadv}
    @category{ctrl}
    @appearance{hyperlinkctrl.png}

    @see wxURL, wxHyperlinkEvent
*/
class wxHyperlinkCtrl : public wxControl
{
public:
    /**
        Constructor. See Create() for more info.
    */
    wxHyperLinkCtrl(wxWindow* parent, wxWindowID id,
                    const wxString& label,
                    const wxString& url,
                    const wxPoint& pos = wxDefaultPosition,
                    const wxSize& size = wxDefaultSize,
                    long style = wxHL_DEFAULT_STYLE,
                    const wxString& name = wxHyperlinkCtrlNameStr);

    /**
        Creates the hyperlink control.

        @param parent
            Parent window. Must not be @NULL.
        @param id
            Window identifier. A value of wxID_ANY indicates a default value.
        @param label
            The label of the hyperlink.
        @param url
            The URL associated with the given label.
        @param pos
            Window position.
        @param size
            Window size.
            If the wxDefaultSize is specified then the window is sized appropriately.
        @param style
            Window style. See wxHyperlinkCtrl.
        @param name
            Window name.
    */
    bool Create(wxWindow* parent, wxWindowID id, const wxString& label,
                const wxString& url, const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxHL_DEFAULT_STYLE,
                const wxString& name = wxHyperlinkCtrlNameStr);

    /**
        Returns the colour used to print the label of the hyperlink when the mouse is
        over the control.
    */
    virtual wxColour GetHoverColour() const;

    /**
        Returns the colour used to print the label when the link has never been clicked
        before (i.e. the link has not been @e visited) and the mouse is not over the control.
    */
    virtual wxColour GetNormalColour() const;

    /**
        Returns the URL associated with the hyperlink.
    */
    virtual wxString GetURL() const;

    /**
        Returns @true if the hyperlink has already been clicked by the user at least
        one time.
    */
    virtual bool GetVisited() const = 0;

    /**
        Returns the colour used to print the label when the mouse is not over the
        control and the link has already been clicked before (i.e. the link has
        been @e visited).
    */
    virtual wxColour GetVisitedColour() const;

    /**
        Sets the colour used to print the label of the hyperlink when the mouse is over
        the control.
    */
    virtual void SetHoverColour(const wxColour& colour);

    /**
        Sets the colour used to print the label when the link has never been clicked before
        (i.e. the link has not been @e visited) and the mouse is not over the control.
    */
    virtual void SetNormalColour(const wxColour& colour);

    /**
        Sets the URL associated with the hyperlink.
    */
    virtual void SetURL(const wxString& url);

    /**
        Marks the hyperlink as visited (see wxHyperlinkCtrl::SetVisitedColour).
    */
    virtual void SetVisited(bool visited = true) = 0;

    /**
        Sets the colour used to print the label when the mouse is not over the control
        and the link has already been clicked before (i.e. the link has been @e visited).
    */
    virtual void SetVisitedColour(const wxColour& colour);
};

