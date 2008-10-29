/////////////////////////////////////////////////////////////////////////////
// Name:        odcombo.h
// Purpose:     interface of wxOwnerDrawnComboBox
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////


enum wxOwnerDrawnComboBoxPaintingFlags
{
    /**
        Combo control is being painted, instead of a list item.
        Argument item may be @c wxNOT_FOUND in this case.
    */
    wxODCB_PAINTING_CONTROL         = 0x0001,

    /**
        An item with selection background is being painted.
        DC text colour should already be correct.
    */
    wxODCB_PAINTING_SELECTED        = 0x0002
};

/**
    @class wxOwnerDrawnComboBox

    wxOwnerDrawnComboBox is a combobox with owner-drawn list items.
    In essence, it is a wxComboCtrl with wxVListBox popup and wxControlWithItems
    interface.

    Implementing item drawing and measuring is similar to wxVListBox.
    Application needs to subclass wxOwnerDrawnComboBox and implement
    OnDrawItem(), OnMeasureItem() and OnMeasureItemWidth().

    @beginStyleTable
    @style{wxODCB_DCLICK_CYCLES}
           Double-clicking cycles item if wxCB_READONLY is also used.
           Synonymous with wxCC_SPECIAL_DCLICK.
    @style{wxODCB_STD_CONTROL_PAINT}
           Control itself is not custom painted using OnDrawItem. Even if this
           style is not used, writable wxOwnerDrawnComboBox is never custom
           painted unless SetCustomPaintWidth() is called.
    @endStyleTable

    @see wxComboCtrl window styles and @ref overview_windowstyles.

    @beginEventTable{wxCommandEvent}
    @event{EVT_COMBOBOX(id, func)}
           Process a wxEVT_COMMAND_COMBOBOX_SELECTED event, when an item on
           the list is selected. Note that calling GetValue() returns the new
           value of selection.
    @endEventTable

    @see Events emitted by wxComboCtrl.

    @library{wxadv}
    @category{ctrl}
    @appearance{ownerdrawncombobox.png}

    @see wxComboCtrl, wxComboBox, wxVListBox, wxCommandEvent
*/
class wxOwnerDrawnComboBox : public wxComboCtrl
{
public:
    /**
        Default constructor.
    */
    wxOwnerDrawnComboBox();

    //@{
    /**
        Constructor, creating and showing a owner-drawn combobox.

        @param parent
            Parent window. Must not be @NULL.
        @param id
            Window identifier. The value @c wxID_ANY indicates a default value.
        @param value
            Initial selection string. An empty string indicates no selection.
        @param pos
            Window position.
        @param size
            Window size.
            If ::wxDefaultSize is specified then the window is sized appropriately.
        @param n
            Number of strings with which to initialise the control.
        @param choices
            An array of strings with which to initialise the control.
        @param style
            Window style. See wxOwnerDrawnComboBox.
        @param validator
            Window validator.
        @param name
            Window name.

        @see Create(), wxValidator
    */
    wxOwnerDrawnComboBox(wxWindow* parent, wxWindowID id,
                         const wxString& value = "",
                         const wxPoint& pos = wxDefaultPosition,
                         const wxSize& size = wxDefaultSize,
                         int n = 0,
                         const wxString[] choices = NULL,
                         long style = 0,
                         const wxValidator& validator = wxDefaultValidator,
                         const wxString& name = "comboBox");
    /**
        Constructor, creating and showing a owner-drawn combobox.

        @param parent
            Parent window. Must not be @NULL.
        @param id
            Window identifier. The value @c wxID_ANY indicates a default value.
        @param value
            Initial selection string. An empty string indicates no selection.
        @param pos
            Window position.
        @param size
            Window size.
            If ::wxDefaultSize is specified then the window is sized appropriately.
        @param choices
            An array of strings with which to initialise the control.
        @param style
            Window style. See wxOwnerDrawnComboBox.
        @param validator
            Window validator.
        @param name
            Window name.

        @see Create(), wxValidator
    */
    wxOwnerDrawnComboBox(wxWindow* parent, wxWindowID id,
                         const wxString& value,
                         const wxPoint& pos,
                         const wxSize& size,
                         const wxArrayString& choices,
                         long style = 0,
                         const wxValidator& validator = wxDefaultValidator,
                         const wxString& name = "comboBox");
    //@}

    /**
        Destructor, destroying the owner-drawn combobox.
    */
    virtual ~wxOwnerDrawnComboBox();

    //@{
    /**
        Creates the combobox for two-step construction.
        See wxOwnerDrawnComboBox() for further details.

        @remarks Derived classes should call or replace this function.
    */
    bool Create(wxWindow* parent, wxWindowID id,
                const wxString& value = "",
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                int n, const wxString choices[],
                long style = 0,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = "comboBox");
    bool Create(wxWindow* parent, wxWindowID id,
                const wxString& value,
                const wxPoint& pos,
                const wxSize& size,
                const wxArrayString& choices,
                long style = 0,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = "comboBox");
    //@}

    /**
        Returns index to the widest item in the list.
    */
    virtual int GetWidestItem();

    /**
        Returns width of the widest item in the list.
    */
    virtual int GetWidestItemWidth();

protected:

    /**
        This method is used to draw the items background and, maybe, a border around it.

        The base class version implements a reasonable default behaviour which consists
        in drawing the selected item with the standard background colour and drawing a
        border around the item if it is either selected or current.

        @remarks flags has the same meaning as with OnDrawItem().
    */
    virtual void OnDrawBackground(wxDC& dc, const wxRect& rect, int item,
                                  int flags) const;

    /**
        The derived class may implement this function to actually draw the item
        with the given index on the provided DC.

        If function is not implemented, the item text is simply drawn, as if the control
        was a normal combobox.

        @param dc
            The device context to use for drawing
        @param rect
            The bounding rectangle for the item being drawn (DC clipping
            region is set to this rectangle before calling this function)
        @param item
            The index of the item to be drawn
        @param flags
            A combination of the ::wxOwnerDrawnComboBoxPaintingFlags enumeration values.
    */
    virtual void OnDrawItem(wxDC& dc, const wxRect& rect, int item,
                            int flags) const;

    /**
        The derived class may implement this method to return the height of the
        specified item (in pixels).

        The default implementation returns text height, as if this control was
        a normal combobox.
    */
    virtual wxCoord OnMeasureItem(size_t item) const;

    /**
        The derived class may implement this method to return the width of the
        specified item (in pixels). If -1 is returned, then the item text width
        is used.

        The default implementation returns -1.
    */
    virtual wxCoord OnMeasureItemWidth(size_t item) const;
};

