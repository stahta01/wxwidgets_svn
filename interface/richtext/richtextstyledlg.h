/////////////////////////////////////////////////////////////////////////////
// Name:        richtext/richtextstyledlg.h
// Purpose:     interface of wxRichTextStyleOrganiserDialog
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxRichTextStyleOrganiserDialog
    @headerfile richtextstyledlg.h wx/richtext/richtextstyledlg.h

    This class shows a style sheet and allows the user to edit, add and remove
    styles.
    It can also be used as a style browser, for example if the application is not
    using a permanent wxRichTextStyleComboCtrl or wxRichTextStyleListCtrl to
    present styles.

    @library{wxrichtext}
    @category{richtext}
*/
class wxRichTextStyleOrganiserDialog : public wxDialog
{
public:
    //@{
    /**
        Constructors.
        To create a dialog, pass a bitlist of @a flags (see below), a style sheet, a
        text control to apply a selected style to (or @NULL), followed by the usual window parameters.
        To specify the operations available to the user, pass a combination of these
        values to @e flags:
        
        @b wxRICHTEXT_ORGANISER_DELETE_STYLES
        
        Provides a button for deleting styles.
        
        @b wxRICHTEXT_ORGANISER_CREATE_STYLES
        
        Provides buttons for creating styles.
        
        @b wxRICHTEXT_ORGANISER_APPLY_STYLES
        
        Provides a button for applying the currently selected style to the selection.
        
        @b wxRICHTEXT_ORGANISER_EDIT_STYLES
        
        Provides a button for editing styles.
        
        @b wxRICHTEXT_ORGANISER_RENAME_STYLES
        
        Provides a button for renaming styles.
        
        @b wxRICHTEXT_ORGANISER_OK_CANCEL
        
        Provides OK and Cancel buttons.
        
        @b wxRICHTEXT_ORGANISER_RENUMBER
        
        Provides a checkbox for specifying that the selection should be renumbered.
        
        The following flags determine what will be displayed in the style list:
        
        @b wxRICHTEXT_ORGANISER_SHOW_CHARACTER
        
        Displays character styles only.
        
        @b wxRICHTEXT_ORGANISER_SHOW_PARAGRAPH
        
        Displays paragraph styles only.
        
        @b wxRICHTEXT_ORGANISER_SHOW_LIST
        
        Displays list styles only.
        
        @b wxRICHTEXT_ORGANISER_SHOW_ALL
        
        Displays all styles.
        
        The following symbols define commonly-used combinations of flags:
        
        @b wxRICHTEXT_ORGANISER_ORGANISE
        
        Enable all style editing operations so the dialog behaves as a style organiser.
        
        @b wxRICHTEXT_ORGANISER_BROWSE
        
        Show a list of all styles and their previews, but only allow application of a
        style or
        cancellation of the dialog. This makes the dialog behave as a style browser.
        
        @b wxRICHTEXT_ORGANISER_BROWSE_NUMBERING
        
        Enables only list style browsing, plus a control to specify renumbering. This
        allows the dialog to be used for applying list styles to the selection.
    */
    wxRichTextStyleOrganiserDialog(int flags,
                                   wxRichTextStyleSheet* sheet,
                                   wxRichTextCtrl* ctrl,
                                   wxWindow* parent,
                                   wxWindowID id = wxID_ANY);
    const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX)
    wxRichTextStyleOrganiserDialog();
    //@}

    /**
        Applies the selected style to selection in the given control or the control
        passed to the constructor.
    */
    bool ApplyStyle(wxRichTextCtrl* ctrl = NULL);

    /**
        ,  wxPoint&@e pos = wxDefaultPosition,  wxSize&@e size = wxDefaultSize, @b
        long@e style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX)
        Creates the dialog. See
    */
    bool Create(int flags, wxRichTextStyleSheet* sheet,
                wxRichTextCtrl* ctrl,
                wxWindow* parent,
                wxWindowID id = wxID_ANY) const;

    /**
        Returns @true if the user has opted to restart numbering.
    */
    bool GetRestartNumbering() const;

    /**
        Returns the associated rich text control (if any).
    */
    wxRichTextCtrl* GetRichTextCtrl() const;

    /**
        Returns selected style name.
    */
    wxString GetSelectedStyle() const;

    /**
        Returns selected style definition.
    */
    wxRichTextStyleDefinition* GetSelectedStyleDefinition() const;

    /**
        Returns the associated style sheet.
    */
    wxRichTextStyleSheet* GetStyleSheet() const;

    /**
        Sets the flags used to control the interface presented to the user.
    */
    void SetFlags(int flags);

    /**
        Checks or unchecks the restart numbering checkbox.
    */
    void SetRestartNumbering(bool restartNumbering);

    /**
        Sets the control to be associated with the dialog, for the purposes of applying
        a style to the selection.
    */
    void SetRichTextCtrl(wxRichTextCtrl* ctrl);

    /**
        Determines whether tooltips will be shown.
    */
    void SetShowToolTips(bool show);

    /**
        Sets the associated style sheet.
    */
    void SetStyleSheet(wxRichTextStyleSheet* sheet);

    /**
        Returns the flags used to control the interface presented to the user.
    */
    int GetFlags() const;
};

