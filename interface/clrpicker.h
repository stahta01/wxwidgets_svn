/////////////////////////////////////////////////////////////////////////////
// Name:        clrpicker.h
// Purpose:     documentation for wxColourPickerCtrl class
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxColourPickerCtrl
    @wxheader{clrpicker.h}

    This control allows the user to select a colour. The generic implementation is
    a button which brings up a wxColourDialog when clicked. Native implementation
    may differ but this is usually a (small) widget which give access to the
    colour-chooser
    dialog.
    It is only available if @c wxUSE_COLOURPICKERCTRL is set to 1 (the default).

    @beginStyleTable
    @style{wxCLRP_DEFAULT_STYLE}:
           The default style: 0.
    @style{wxCLRP_USE_TEXTCTRL}:
           Creates a text control to the left of the picker button which is
           completely managed by the wxColourPickerCtrl and which can be used
           by the user to specify a colour (see SetColour). The text control
           is automatically synchronized with button's value. Use functions
           defined in wxPickerBase to modify the text control.
    @style{wxCLRP_SHOW_LABEL}:
           Shows the colour in HTML form (AABBCC) as colour button label
           (instead of no label at all).
    @endStyleTable

    @library{wxcore}
    @category{miscpickers}
    @appearance{colourpickerctrl.png}

    @seealso
    wxColourDialog, wxColourPickerEvent
*/
class wxColourPickerCtrl : public wxPickerBase
{
public:
    /**
        Initializes the object and calls Create() with
        all the parameters.
    */
    wxColourPickerCtrl(wxWindow* parent, wxWindowID id,
                       const wxColour& colour = wxBLACK,
                       const wxPoint& pos = wxDefaultPosition,
                       const wxSize& size = wxDefaultSize,
                       long style = wxCLRP_DEFAULT_STYLE,
                       const wxValidator& validator = wxDefaultValidator,
                       const wxString& name = "colourpickerctrl");

    /**
        @param parent
            Parent window, must not be non-@NULL.
        @param id
            The identifier for the control.
        @param colour
            The initial colour shown in the control.
        @param pos
            Initial position.
        @param size
            Initial size.
        @param style
            The window style, see wxCRLP_* flags.
        @param validator
            Validator which can be used for additional date checks.
        @param name
            Control name.
        
        @returns @true if the control was successfully created or @false if
                 creation failed.
    */
    bool Create(wxWindow* parent, wxWindowID id,
                const wxColour& colour = wxBLACK,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxCLRP_DEFAULT_STYLE,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = "colourpickerctrl");

    /**
        Returns the currently selected colour.
    */
    wxColour GetColour() const;

    //@{
    /**
        Sets the currently selected colour. See wxColour::Set.
    */
    void SetColour(const wxColour& col);
    void SetColour(const wxString& colname);
    //@}
};


/**
    @class wxColourPickerEvent
    @wxheader{clrpicker.h}

    This event class is used for the events generated by
    wxColourPickerCtrl.

    @library{wxcore}
    @category{FIXME}

    @seealso
    wxColourPickerCtrl
*/
class wxColourPickerEvent : public wxCommandEvent
{
public:
    /**
        The constructor is not normally used by the user code.
    */
    wxColourPickerEvent(wxObject* generator, int id,
                        const wxColour& colour);

    /**
        Retrieve the colour the user has just selected.
    */
    wxColour GetColour() const;

    /**
        Set the colour associated with the event.
    */
    void SetColour(const wxColour& pos);
};
