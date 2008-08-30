/////////////////////////////////////////////////////////////////////////////
// Name:        checklst.h
// Purpose:     interface of wxCheckListBox
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxCheckListBox

    A wxCheckListBox is like a wxListBox, but allows items to be checked or
    unchecked.

    When using this class under Windows wxWidgets must be compiled with
    wxUSE_OWNER_DRAWN set to 1.

    Only the new functions for this class are documented; see also wxListBox.

    Please note that wxCheckListBox uses client data in its implementation,
    and therefore this is not available to the application.

    @beginEventTable{wxCommandEvent}
    @event{EVT_CHECKLISTBOX(id, func)}
           Process a wxEVT_COMMAND_CHECKLISTBOX_TOGGLED event, when an item in
           the check list box is checked or unchecked.
    @endEventTable

    @library{wxcore}
    @category{ctrl}
    <!-- @appearance{checklistbox.png} -->

    @see wxListBox, wxChoice, wxComboBox, wxListCtrl, wxCommandEvent
*/
class wxCheckListBox : public wxListBox
{
public:
    /**
        Default constructor.
    */
    wxCheckListBox();

    //@{
    /**
        Constructor, creating and showing a list box.

        @param parent
            Parent window. Must not be @NULL.
        @param id
            Window identifier. The value wxID_ANY indicates a default value.
        @param pos
            Window position.
        @param size
            Window size. If wxDefaultSize is specified then the window is sized
            appropriately.
        @param n
            Number of strings with which to initialise the control.
        @param choices
            An array of strings with which to initialise the control.
        @param style
            Window style. See wxCheckListBox.
        @param validator
            Window validator.
        @param name
            Window name.
    */
    wxCheckListBox(wxWindow* parent, wxWindowID id,
                   const wxPoint& pos = wxDefaultPosition,
                   const wxSize& size = wxDefaultSize,
                   int n = 0,
                   const wxString choices[] = NULL,
                   long style = 0,
                   const wxValidator& validator = wxDefaultValidator,
                   const wxString& name = "listBox");
    /**
        Constructor, creating and showing a list box.

        @param parent
            Parent window. Must not be @NULL.
        @param id
            Window identifier. The value wxID_ANY indicates a default value.
        @param pos
            Window position.
        @param size
            Window size. If wxDefaultSize is specified then the window is sized
            appropriately.
        @param choices
            An array of strings with which to initialise the control.
        @param style
            Window style. See wxCheckListBox.
        @param validator
            Window validator.
        @param name
            Window name.
    */
    wxCheckListBox(wxWindow* parent, wxWindowID id,
                   const wxPoint& pos,
                   const wxSize& size,
                   const wxArrayString& choices,
                   long style = 0,
                   const wxValidator& validator = wxDefaultValidator,
                   const wxString& name = "listBox");
    //@}

    /**
        Destructor, destroying the list box.
    */
    ~wxCheckListBox();

    /**
        Checks the given item. Note that calling this method does not result in
        a wxEVT_COMMAND_CHECKLISTBOX_TOGGLE event being emitted.

        @param item
            Index of item to check.
        @param check
            @true if the item is to be checked, @false otherwise.
    */
    void Check(int item, bool check = true);

    /**
        Returns @true if the given item is checked, @false otherwise.

        @param item
            Index of item whose check status is to be returned.
    */
    bool IsChecked(unsigned int item) const;
};

