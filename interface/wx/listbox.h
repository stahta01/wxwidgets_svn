/////////////////////////////////////////////////////////////////////////////
// Name:        listbox.h
// Purpose:     interface of wxListBox
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxListBox

    A listbox is used to select one or more of a list of strings. The
    strings are displayed in a scrolling box, with the selected string(s)
    marked in reverse video. A listbox can be single selection (if an item
    is selected, the previous selection is removed) or multiple selection
    (clicking an item toggles the item on or off independently of other
    selections).

    List box elements are numbered from zero. Their number may be limited
    under some platforms.

    A listbox callback gets an event wxEVT_COMMAND_LISTBOX_SELECTED for
    single clicks, and wxEVT_COMMAND_LISTBOX_DOUBLECLICKED for double clicks.

    @beginStyleTable
    @style{wxLB_SINGLE}
           Single-selection list.
    @style{wxLB_MULTIPLE}
           Multiple-selection list: the user can toggle multiple items on and
           off. This is the same as wxLB_EXTENDED in wxGTK2 port.
    @style{wxLB_EXTENDED}
           Extended-selection list: the user can extend the selection by using
           @c SHIFT or @c CTRL keys together with the cursor movement keys or
           the mouse.
    @style{wxLB_HSCROLL}
           Create horizontal scrollbar if contents are too wide (Windows only).
    @style{wxLB_ALWAYS_SB}
           Always show a vertical scrollbar.
    @style{wxLB_NEEDED_SB}
           Only create a vertical scrollbar if needed.
    @style{wxLB_SORT}
           The listbox contents are sorted in alphabetical order.
    @endStyleTable

    @beginEventTable{wxCommandEvent}
    @event{EVT_LISTBOX(id, func)}
           Process a wxEVT_COMMAND_LISTBOX_SELECTED event, when an item on the
           list is selected or the selection changes.
    @event{EVT_LISTBOX_DCLICK(id, func)}
           Process a wxEVT_COMMAND_LISTBOXDOUBLECLICKED event, when the
           listbox is double-clicked.
    @endEventTable

    @library{wxcore}
    @category{ctrl}
    <!-- @appearance{listbox.png} -->

    @see wxChoice, wxComboBox, wxListCtrl, wxCommandEvent
*/
class wxListBox : public wxControlWithItems
{
public:
    /**
        Default constructor.
    */
    wxListBox();
    
    /**
        Constructor 

        @param n
            Number of strings with which to initialise the control.
        @param style
            Window style. See wxListBox.
    */
    
    wxListBox(wxWindow* parent, wxWindowID id,
              const wxPoint& pos = wxDefaultPosition,
              const wxSize& size = wxDefaultSize,
              int n = 0,
              const wxString choices[] = NULL,
              long style = 0,
              const wxValidator& validator = wxDefaultValidator,
              const wxString& name = "listBox");
              
    /**
        Constructor 

        @param choices
            An array of strings with which to initialise the control.
        @param style
            Window style. See wxListBox.
    */
    
    wxListBox(wxWindow* parent, wxWindowID id,
              const wxPoint& pos,
              const wxSize& size,
              const wxArrayString& choices,
              long style = 0,
              const wxValidator& validator = wxDefaultValidator,
              const wxString& name = "listBox");

    /**
        Destructor, destroying the list box.
    */
    ~wxListBox();

    //@{
    /**
        Creates the listbox for two-step construction. See wxListBox()
        for further details.
    */
    bool Create(wxWindow* parent, wxWindowID id,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                int n,
                const wxString choices[] = NULL,
                long style = 0,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = "listBox");
    bool Create(wxWindow* parent, wxWindowID id,
                const wxPoint& pos,
                const wxSize& size,
                const wxArrayString& choices,
                long style = 0,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = "listBox");
    //@}

    /**
        Deselects an item in the list box.

        @param n
            The zero-based item to deselect.

        @remarks This applies to multiple selection listboxes only.
    */
    void Deselect(int n);

    /**
        Fill an array of ints with the positions of the currently selected items.

        @param selections
            A reference to an wxArrayInt instance that is used to store the result of
        the query.

        @return The number of selections.

        @remarks Use this with a multiple selection listbox.

        @see wxControlWithItems::GetSelection, wxControlWithItems::GetStringSelection,
             wxControlWithItems::SetSelection
    */
    int GetSelections(wxArrayInt& selections) const;

    /**
        Returns the item located at @e point, or @c wxNOT_FOUND if there
        is no item located at @e point.

        It is currently implemented for wxMSW, wxMac and wxGTK2 ports.

        @param point
            Point of item (in client coordinates) to obtain

        @return Item located at point, or wxNOT_FOUND if unimplemented or the
                 item does not exist.

        @since 2.7.0
    */
    int HitTest(const wxPoint point) const;

    /**
        Insert the given number of strings before the specified position.

        @param nItems
            Number of items in the array items
        @param items
            Labels of items to be inserted
        @param pos
            Position before which to insert the items: if pos is 0 the
            items will be inserted in the beginning of the listbox
    */
    void InsertItems(int nItems, const wxString *items,
                     unsigned int pos);

    /**
        Insert the given number of strings before the specified position.

        @param items
            Labels of items to be inserted
        @param pos
            Position before which to insert the items: if pos is 0 the
            items will be inserted in the beginning of the listbox
    */
    void InsertItems(const wxArrayString& items,
                     unsigned int pos);

    /**
        Determines whether an item is selected.

        @param n
            The zero-based item index.

        @return @true if the given item is selected, @false otherwise.
    */
    bool IsSelected(int n) const;

    /**
        Clears the list box and adds the given strings to it.

        @param n
            The number of strings to set.
        @param choices
            An array of strings to set.
        @param clientData
            Options array of client data pointers
    */
    void Set(int n, const wxString* choices, void **clientData = NULL);

    /**
        Clears the list box and adds the given strings to it. You may
        free the array from the calling program after this method
        has been called.

        @param choices
            An array of strings to set.
        @param clientData
            Options array of client data pointers
    */
    void Set(const wxArrayString& choices,
             void **clientData = NULL);

    /**
        Set the specified item to be the first visible item.

        @param n
            The zero-based item index that should be visible.
    */
    void SetFirstItem(int n);

    /**
        Set the specified item to be the first visible item.

        @param string
            The string that should be visible.
    */
    void SetFirstItem(const wxString& string);
};

