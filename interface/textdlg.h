/////////////////////////////////////////////////////////////////////////////
// Name:        textdlg.h
// Purpose:     interface of wxPasswordEntryDialog
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxPasswordEntryDialog
    @wxheader{textdlg.h}

    This class represents a dialog that requests a one-line password string from
    the user.
    It is implemented as a generic wxWidgets dialog.

    @library{wxbase}
    @category{cmndlg}

    @see @ref overview_wxpasswordentrydialogoverview "wxPassowrdEntryDialog
    overview"
*/
class wxPasswordEntryDialog : public wxTextEntryDialog
{
public:

};



/**
    @class wxTextEntryDialog
    @wxheader{textdlg.h}

    This class represents a dialog that requests a one-line text string from the
    user.
    It is implemented as a generic wxWidgets dialog.

    @library{wxbase}
    @category{cmndlg}

    @see @ref overview_wxtextentrydialogoverview "wxTextEntryDialog overview"
*/
class wxTextEntryDialog : public wxDialog
{
public:
    /**
        Constructor. Use ShowModal() to show the dialog.
        
        @param parent
            Parent window.
        @param message
            Message to show on the dialog.
        @param defaultValue
            The default value, which may be the empty string.
        @param style
            A dialog style, specifying the buttons (wxOK, wxCANCEL)
            and an optional wxCENTRE style. Additionally, wxTextCtrl styles (such as
            wxTE_PASSWORD) may be specified here.
        @param pos
            Dialog position.
    */
    wxTextEntryDialog(wxWindow* parent, const wxString& message,
                      const wxString& caption = "Please enter text",
                      const wxString& defaultValue = "",
                      long style = wxOK | wxCANCEL | wxCENTRE,
                      const wxPoint& pos = wxDefaultPosition);

    /**
        Destructor.
    */
    ~wxTextEntryDialog();

    /**
        Returns the text that the user has entered if the user has pressed OK, or the
        original value
        if the user has pressed Cancel.
    */
    wxString GetValue() const;

    /**
        Sets the default text value.
    */
    void SetValue(const wxString& value);

    /**
        Shows the dialog, returning wxID_OK if the user pressed OK, and wxID_CANCEL
        otherwise.
    */
    int ShowModal();
};



// ============================================================================
// Global functions/macros
// ============================================================================

/**
    Pop up a dialog box with title set to @e caption, @e message, and a
    @e default_value.  The user may type in text and press OK to return this text,
    or press Cancel to return the empty string.
    If @a centre is @true, the message text (which may include new line characters)
    is centred; if @false, the message is left-justified.
*/
wxString wxGetTextFromUser(const wxString& message,
                           const wxString& caption = "Input text",
                           const wxString& default_value = "",
                           wxWindow* parent = NULL,
                           int x = wxDefaultCoord,
                           int y = wxDefaultCoord,
                           bool centre = true);

/**
    Similar to wxGetTextFromUser() but the text entered
    in the dialog is not shown on screen but replaced with stars. This is intended
    to be used for entering passwords as the function name implies.
*/
wxString wxGetPasswordFromUser(const wxString& message,
                               const wxString& caption = "Input text",
                               const wxString& default_value = "",
                               wxWindow* parent = NULL,
                               int x = wxDefaultCoord,
                               int y = wxDefaultCoord,
                               bool centre = true);

