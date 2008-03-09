/////////////////////////////////////////////////////////////////////////////
// Name:        srchctrl.h
// Purpose:     documentation for wxSearchCtrl class
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxSearchCtrl
    @wxheader{srchctrl.h}

    A search control is a composite control with a search button, a text
    control, and a cancel button.

    @beginStyleTable
    @style{wxTE_PROCESS_ENTER}:
           The control will generate the event wxEVT_COMMAND_TEXT_ENTER
           (otherwise pressing Enter key is either processed internally by the
           control or used for navigation between dialog controls).
    @style{wxTE_PROCESS_TAB}:
           The control will receive wxEVT_CHAR events for TAB pressed -
           normally, TAB is used for passing to the next control in a dialog
           instead. For the control created with this style, you can still use
           Ctrl-Enter to pass to the next control from the keyboard.
    @style{wxTE_NOHIDESEL}:
           By default, the Windows text control doesn't show the selection
           when it doesn't have focus - use this style to force it to always
           show it. It doesn't do anything under other platforms.
    @style{wxTE_LEFT}:
           The text in the control will be left-justified (default).
    @style{wxTE_CENTRE}:
           The text in the control will be centered (currently wxMSW and
           wxGTK2 only).
    @style{wxTE_RIGHT}:
           The text in the control will be right-justified (currently wxMSW
           and wxGTK2 only).
    @style{wxTE_CAPITALIZE}:
           On PocketPC and Smartphone, causes the first letter to be
           capitalized.
    @endStyleTable

    @library{wxcore}
    @category{FIXME}

    @seealso
    wxTextCtrl::Create, wxValidator
*/
class wxSearchCtrl : public wxTextCtrl
{
public:
    //@{
    /**
        Constructor, creating and showing a text control.
        
        @param parent
            Parent window. Should not be @NULL.
        @param id
            Control identifier. A value of -1 denotes a default value.
        @param value
            Default text value.
        @param pos
            Text control position.
        @param size
            Text control size.
        @param style
            Window style. See wxSearchCtrl.
        @param validator
            Window validator.
        @param name
            Window name.
        
        @see wxTextCtrl::Create, wxValidator
    */
    wxSearchCtrl();
    wxSearchCtrl(wxWindow* parent, wxWindowID id,
                 const wxString& value = "",
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = 0,
                 const wxValidator& validator = wxDefaultValidator,
                 const wxString& name = wxSearchCtrlNameStr);
    //@}

    /**
        Destructor, destroying the search control.
    */
    ~wxSearchCtrl();

    /**
        Returns a pointer to the search control's menu object or @NULL if there is no
        menu attached.
    */
    virtual wxMenu* GetMenu();

    /**
        Returns the search button visibility value.
        If there is a menu attached, the search button will be visible regardless of
        the search
        button visibility value.
        This always returns @false in Mac OS X v10.3
    */
    virtual bool IsSearchButtonVisible();

    /**
        Sets the search control's menu object.  If there is already a menu associated
        with
        the search control it is deleted.
        
        @param menu
            Menu to attach to the search control.
    */
    virtual void SetMenu(wxMenu* menu);

    /**
        Shows or hides the cancel button.
    */
    virtual void ShowCancelButton(bool show);

    /**
        Sets the search button visibility value on the search control.
        If there is a menu attached, the search button will be visible regardless of
        the search
        button visibility value.
        This has no effect in Mac OS X v10.3
    */
    virtual void ShowSearchButton(bool show);
};
