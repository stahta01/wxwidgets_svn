/////////////////////////////////////////////////////////////////////////////
// Name:        listctrl.h
// Purpose:     interface of wxListCtrl
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxListCtrl

    A list control presents lists in a number of formats: list view, report view,
    icon view and small icon view. In any case, elements are numbered from zero.
    For all these modes, the items are stored in the control and must be added to
    it using wxListCtrl::InsertItem method.

    A special case of report view quite different from the other modes of the list
    control is a virtual control in which the items data (including text, images
    and attributes) is managed by the main program and is requested by the control
    itself only when needed which allows to have controls with millions of items
    without consuming much memory. To use virtual list control you must use
    wxListCtrl::SetItemCount first and overload at least
    wxListCtrl::OnGetItemText (and optionally
    wxListCtrl::OnGetItemImage or wxListCtrl::OnGetItemColumnImage and
    wxListCtrl::OnGetItemAttr) to return the information
    about the items when the control requests it. Virtual list control can be used
    as a normal one except that no operations which can take time proportional to
    the number of items in the control happen -- this is required to allow having a
    practically infinite number of items. For example, in a multiple selection
    virtual list control, the selections won't be sent when many items are selected
    at once because this could mean iterating over all the items.

    Using many of wxListCtrl features is shown in the
    @ref overview_samplelistctrl "corresponding sample".

    To intercept events from a list control, use the event table macros described
    in wxListEvent.

    @b Mac Note: Starting with 2.8, wxListCtrl uses a native implementation for
    report mode, and uses a generic implementation for other modes. You can use the
    generic implementation for report mode as well by setting the
    mac.listctrl.always_use_generic wxSystemOption() to
    1.

    @beginStyleTable
    @style{wxLC_LIST}
           Multicolumn list view, with optional small icons. Columns are
           computed automatically, i.e. you don't set columns as in
           wxLC_REPORT. In other words, the list wraps, unlike a wxListBox.
    @style{wxLC_REPORT}
           Single or multicolumn report view, with optional header.
    @style{wxLC_VIRTUAL}
           The application provides items text on demand. May only be used
           with wxLC_REPORT.
    @style{wxLC_ICON}
           Large icon view, with optional labels.
    @style{wxLC_SMALL_ICON}
           Small icon view, with optional labels.
    @style{wxLC_ALIGN_TOP}
           Icons align to the top. Win32 default, Win32 only.
    @style{wxLC_ALIGN_LEFT}
           Icons align to the left.
    @style{wxLC_AUTOARRANGE}
           Icons arrange themselves. Win32 only.
    @style{wxLC_EDIT_LABELS}
           Labels are editable: the application will be notified when editing
           starts.
    @style{wxLC_NO_HEADER}
           No header in report mode.
    @style{wxLC_SINGLE_SEL}
           Single selection (default is multiple).
    @style{wxLC_SORT_ASCENDING}
           Sort in ascending order. (You must still supply a comparison callback
           in wxListCtrl::SortItems.)
    @style{wxLC_SORT_DESCENDING}
           Sort in descending order. (You must still supply a comparison callback
           in wxListCtrl::SortItems.)
    @style{wxLC_HRULES}
           Draws light horizontal rules between rows in report mode.
    @style{wxLC_VRULES}
           Draws light vertical rules between columns in report mode.
    @endStyleTable

    @library{wxcore}
    @category{ctrl}
    <!-- @appearance{listctrl.png} -->

    @see @ref overview_listctrl "wxListCtrl Overview", wxListView,
    wxListBox, wxTreeCtrl, wxImageList, wxListEvent, wxListItem
*/
class wxListCtrl : public wxControl
{
public:
    //@{
    /**
    Constructor, creating and showing a list control.
    */
    wxListCtrl();

    /**
        Constructor, creating and showing a list control.

        @param parent
            Parent window. Must not be @NULL.
        @param id
            Window identifier. The value wxID_ANY indicates a default value.
        @param pos
            Window position.
        @param size
            Window size. If wxDefaultSize is specified then the window is
            sized appropriately.
        @param style
            Window style. See wxListCtrl.
        @param validator
            Window validator.
        @param name
            Window name.

        @see Create(), wxValidator
    */
    wxListCtrl(wxWindow* parent, wxWindowID id,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = wxLC_ICON,
               const wxValidator& validator = wxDefaultValidator,
               const wxString& name = wxListCtrlNameStr);
    //@}

    /**
        Destructor, destroying the list control.
    */
    ~wxListCtrl();

    /**
        Arranges the items in icon or small icon view. This only has effect on Win32.
        @a flag is one of:

        wxLIST_ALIGN_DEFAULT

        Default alignment.

        wxLIST_ALIGN_LEFT

        Align to the left side of the control.

        wxLIST_ALIGN_TOP

        Align to the top side of the control.

        wxLIST_ALIGN_SNAP_TO_GRID

        Snap to grid.
    */
    bool Arrange(int flag = wxLIST_ALIGN_DEFAULT);

    /**
        Sets the image list associated with the control and
        takes ownership of it (i.e. the control will, unlike when using
        SetImageList, delete the list when destroyed). @a which is one of
        wxIMAGE_LIST_NORMAL, wxIMAGE_LIST_SMALL, wxIMAGE_LIST_STATE (the last is
        unimplemented).

        @see SetImageList()
    */
    void AssignImageList(wxImageList* imageList, int which);

    /**
        Deletes all items and all columns.
    */
    void ClearAll();

    /**
        Creates the list control. See wxListCtrl() for further details.
    */
    bool Create(wxWindow* parent, wxWindowID id,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxLC_ICON,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = wxListCtrlNameStr);

    /**
        Deletes all items in the list control.
        @note This function does @e not send the
        @c wxEVT_COMMAND_LIST_DELETE_ITEM event because deleting many items
        from the control would be too slow then (unlike wxListCtrl::DeleteItem).
    */
    bool DeleteAllItems();

    /**
        Deletes a column.
    */
    bool DeleteColumn(int col);

    /**
        Deletes the specified item. This function sends the
        @c wxEVT_COMMAND_LIST_DELETE_ITEM event for the item being deleted.
        See also: DeleteAllItems()
    */
    bool DeleteItem(long item);

    /**
        Starts editing the label of the given item. This function generates a
        EVT_LIST_BEGIN_LABEL_EDIT event which can be vetoed so that no
        text control will appear for in-place editing.
        If the user changed the label (i.e. s/he does not press ESC or leave
        the text control without changes, a EVT_LIST_END_LABEL_EDIT event
        will be sent which can be vetoed as well.
    */
    void EditLabel(long item);

    /**
        Ensures this item is visible.
    */
    bool EnsureVisible(long item);

    //@{
    /**
        Find an item nearest this position in the specified direction, starting from
        @a start or the beginning if @a start is -1.


        @b FindItem( start, str, partial = @false )


        @b FindItemData( start, data )


        @b FindItemAtPos( start, point, direction )
    */
    long FindItem(long start, const wxString& str,
                  bool partial = false);
    long FindItem(long start, long data);
    long FindItem(long start, const wxPoint& pt, int direction);
    //@}

    /**
        Gets information about this column. See SetItem() for more
        information.
    */
    bool GetColumn(int col, wxListItem& item) const;

    /**
        Returns the number of columns.
    */
    int GetColumnCount() const;

    /**
        Gets the column number by visual order index (report view only).
    */
    int GetColumnIndexFromOrder(int order) const;

    /**
        Gets the column visual order index (valid in report view only).
    */
    int GetColumnOrder(int col) const;

    /**
        Gets the column width (report view only).
    */
    int GetColumnWidth(int col) const;

    /**
        Returns the array containing the orders of all columns. On error, an empty
        array is returned.
    */
    wxArrayInt GetColumnsOrder() const;

    /**
        Gets the number of items that can fit vertically in the
        visible area of the list control (list or report view)
        or the total number of items in the list control (icon
        or small icon view).
    */
    int GetCountPerPage() const;

    /**
        Returns the edit control being currently used to edit a label. Returns @NULL
        if no label is being edited.
        @note It is currently only implemented for wxMSW and the generic version,
        not for the native Mac OS X version.
    */
    wxTextCtrl* GetEditControl() const;

    /**
        Returns the specified image list. @a which may be one of:

        @b wxIMAGE_LIST_NORMAL

        The normal (large icon) image list.

        @b wxIMAGE_LIST_SMALL

        The small icon image list.

        @b wxIMAGE_LIST_STATE

        The user-defined state image list (unimplemented).
    */
    wxImageList* GetImageList(int which) const;

    /**
        Gets information about the item. See SetItem() for more
        information.
        You must call @e info.SetId() to the ID of item you're interested in
        before calling this method.
    */
    bool GetItem(wxListItem& info) const;

    /**
        Returns the colour for this item. If the item has no specific colour, returns
        an invalid colour (and not the default background control of the control
        itself).

        @see GetItemTextColour()
    */
    wxColour GetItemBackgroundColour(long item) const;

    /**
        Returns the number of items in the list control.
    */
    int GetItemCount() const;

    /**
        Gets the application-defined data associated with this item.
    */
    long GetItemData(long item) const;

    /**
        Returns the item's font.
    */
    wxFont GetItemFont(long item) const;

    /**
        Returns the position of the item, in icon or small icon view.
    */
    bool GetItemPosition(long item, wxPoint& pos) const;

    /**
        Returns the rectangle representing the item's size and position, in physical
        coordinates.
        @a code is one of wxLIST_RECT_BOUNDS, wxLIST_RECT_ICON, wxLIST_RECT_LABEL.
    */
    bool GetItemRect(long item, wxRect& rect,
                     int code = wxLIST_RECT_BOUNDS) const;

    /**
        Retrieves the spacing between icons in pixels: horizontal spacing is returned
        as @c x component of the wxSize object and the vertical
        spacing as its @c y component.
    */
    wxSize GetItemSpacing() const;

    /**
        Gets the item state. For a list of state flags, see SetItem().
        The @b stateMask indicates which state flags are of interest.
    */
    int GetItemState(long item, long stateMask) const;

    /**
        Gets the item text for this item.
    */
    wxString GetItemText(long item) const;

    /**
        Returns the colour for this item. If the item has no specific colour, returns
        an invalid colour (and not the default foreground control of the control itself
        as this wouldn't allow distinguishing between items having the same colour as
        the current control foreground and items with default colour which, hence, have
        always the same colour as the control).
    */
    wxColour GetItemTextColour(long item) const;

    /**
        Searches for an item with the given geometry or state, starting from
        @a item but excluding the @a item itself. If @a item is -1,
        the first item that matches the specified flags will be returned.
        Returns the first item with given state following @a item or -1 if
        no such item found.
        This function may be used to find all selected items in the control like this:

        @a geometry can be one of:

        wxLIST_NEXT_ABOVE

        Searches for an item above the specified item.

        wxLIST_NEXT_ALL

        Searches for subsequent item by index.

        wxLIST_NEXT_BELOW

        Searches for an item below the specified item.

        wxLIST_NEXT_LEFT

        Searches for an item to the left of the specified item.

        wxLIST_NEXT_RIGHT

        Searches for an item to the right of the specified item.

        @note this parameter is only supported by wxMSW currently and ignored on
        other platforms.
        @a state can be a bitlist of the following:

        wxLIST_STATE_DONTCARE

        Don't care what the state is.

        wxLIST_STATE_DROPHILITED

        The item indicates it is a drop target.

        wxLIST_STATE_FOCUSED

        The item has the focus.

        wxLIST_STATE_SELECTED

        The item is selected.

        wxLIST_STATE_CUT

        The item is selected as part of a cut and paste operation.
    */
    long GetNextItem(long item, int geometry = wxLIST_NEXT_ALL,
                     int state = wxLIST_STATE_DONTCARE) const;

    /**
        Returns the number of selected items in the list control.
    */
    int GetSelectedItemCount() const;

    /**
        Returns the rectangle representing the size and position, in physical
        coordinates, of the given subitem, i.e. the part of the row @a item in the
        column @e subItem.
        This method is only meaningfull when the wxListCtrl is in the report mode. If
        @a subItem parameter is equal to the special value
        @c wxLIST_GETSUBITEMRECT_WHOLEITEM the return value is the same as
        for GetItemRect().
        @a code can be one of @c wxLIST_RECT_BOUNDS,
        @c wxLIST_RECT_ICON or @c wxLIST_RECT_LABEL.

        @since 2.7.0
    */
    bool GetSubItemRect(long item, long subItem, wxRect& rect,
                        int code = wxLIST_RECT_BOUNDS) const;

    /**
        Gets the text colour of the list control.
    */
    wxColour GetTextColour() const;

    /**
        Gets the index of the topmost visible item when in
        list or report view.
    */
    long GetTopItem() const;

    /**
        Returns the rectangle taken by all items in the control. In other words, if the
        controls client size were equal to the size of this rectangle, no scrollbars
        would be needed and no free space would be left.
        Note that this function only works in the icon and small icon views, not in
        list or report views (this is a limitation of the native Win32 control).
    */
    wxRect GetViewRect() const;

    /**
        Determines which item (if any) is at the specified point,
        giving details in @e flags. Returns index of the item or @c wxNOT_FOUND
        if no item is at the specified point.
        @a flags will be a combination of the following flags:

        wxLIST_HITTEST_ABOVE

        Above the client area.

        wxLIST_HITTEST_BELOW

        Below the client area.

        wxLIST_HITTEST_NOWHERE

        In the client area but below the last item.

        wxLIST_HITTEST_ONITEMICON

        On the bitmap associated with an item.

        wxLIST_HITTEST_ONITEMLABEL

        On the label (string) associated with an item.

        wxLIST_HITTEST_ONITEMRIGHT

        In the area to the right of an item.

        wxLIST_HITTEST_ONITEMSTATEICON

        On the state icon for a tree view item that is in a user-defined state.

        wxLIST_HITTEST_TOLEFT

        To the right of the client area.

        wxLIST_HITTEST_TORIGHT

        To the left of the client area.

        wxLIST_HITTEST_ONITEM

        Combination of wxLIST_HITTEST_ONITEMICON, wxLIST_HITTEST_ONITEMLABEL,
        wxLIST_HITTEST_ONITEMSTATEICON.

        If @a ptrSubItem is not @NULL and the wxListCtrl is in the report
        mode the subitem (or column) number will also be provided.
        This feature is only available in version 2.7.0 or higher and is currently only
        implemented under wxMSW and requires at least comctl32.dll of verion 4.70 on
        the host system or the value stored in @a ptrSubItem will be always -1. To
        compile this feature into wxWidgets library you need to have access to
        commctrl.h of version 4.70 that is provided by Microsoft.
    */
    long HitTest(const wxPoint& point, int& flags,
                 long* ptrSubItem) const;

    //@{
    /**
        For report view mode (only), inserts a column. For more details, see SetItem().
    */
    long InsertColumn(long col, wxListItem& info);
    long InsertColumn(long col, const wxString& heading,
                      int format = wxLIST_FORMAT_LEFT,
                      int width = -1);
    //@}

    //@{
    /**
        Insert a wxListItem.
        @param info
            wxListItem object
    */
    long InsertItem(wxListItem& info);

    /**
        Insert an string item.
        @param index
            Index of the new item, supplied by the application
        @param label
            String label
    */
    long InsertItem(long index, const wxString& label);
    
    /**
        Insert an image item.
        @param index
            Index of the new item, supplied by the application
        @param imageIndex
            Index into the image list associated with this control and view style
    */    
    long InsertItem(long index, int imageIndex);
    
    /**
        Insert an image/string item.
        @param index
            Index of the new item, supplied by the application
        @param label
            String label
        @param imageIndex
            Index into the image list associated with this control and view style
    */    
    long InsertItem(long index, const wxString& label,
                    int imageIndex);
    //@}

    /**
        This function may be overloaded in the derived class for a control with
        @c wxLC_VIRTUAL style. It should return the attribute for the
        for the specified @c item or @NULL to use the default appearance
        parameters.
        wxListCtrl will not delete the pointer or keep a reference of it. You can
        return the same wxListItemAttr pointer for every OnGetItemAttr call.
        The base class version always returns @NULL.

        @see OnGetItemImage(), OnGetItemColumnImage(),
             OnGetItemText()
    */
    virtual wxListItemAttr* OnGetItemAttr(long item) const;

    /**
        Overload this function in the derived class for a control with
        @c wxLC_VIRTUAL and @c wxLC_REPORT styles in order to specify the image
        index for the given line and column.
        The base class version always calls OnGetItemImage for the first column, else
        it returns -1.

        @see OnGetItemText(), OnGetItemImage(),
             OnGetItemAttr()
    */
    virtual int OnGetItemColumnImage(long item, long column) const;

    /**
        This function must be overloaded in the derived class for a control with
        @c wxLC_VIRTUAL style having an @ref SetImageList() "image list"
        (if the control doesn't have an image list, it is not necessary to overload
         it). It should return the index of the items image in the controls image list
        or -1 for no image.
        In a control with @c wxLC_REPORT style, OnGetItemImage only gets called for
        the first column of each line.
        The base class version always returns -1.

        @see OnGetItemText(), OnGetItemColumnImage(),
             OnGetItemAttr()
    */
    virtual int OnGetItemImage(long item) const;

    /**
        This function @b must be overloaded in the derived class for a control with
        @c wxLC_VIRTUAL style. It should return the string containing the text of
        the given @a column for the specified @c item.

        @see SetItemCount(), OnGetItemImage(),
             OnGetItemColumnImage(), OnGetItemAttr()
    */
    virtual wxString OnGetItemText(long item, long column) const;

    /**
        Redraws the given @e item. This is only useful for the virtual list controls
        as without calling this function the displayed value of the item doesn't change
        even when the underlying data does change.

        @see RefreshItems()
    */
    void RefreshItem(long item);

    /**
        Redraws the items between @a itemFrom and @e itemTo. The starting item
        must be less than or equal to the ending one.
        Just as RefreshItem() this is only useful for
        virtual list controls.
    */
    void RefreshItems(long itemFrom, long itemTo);

    /**
        Scrolls the list control. If in icon, small icon or report view mode,
        @a dx specifies the number of pixels to scroll. If in list view mode,
        @a dx specifies the number of columns to scroll. @a dy always specifies
        the number of pixels to scroll vertically.
        @note This method is currently only implemented in the Windows version.
    */
    bool ScrollList(int dx, int dy);

    /**
        Sets the background colour (GetBackgroundColour already implicit in
        wxWindow class).
    */
    void SetBackgroundColour(const wxColour& col);

    /**
        Sets information about this column. See SetItem() for more
        information.
    */
    bool SetColumn(int col, wxListItem& item);

    /**
        Sets the column width.
        @a width can be a width in pixels or wxLIST_AUTOSIZE (-1) or
        wxLIST_AUTOSIZE_USEHEADER (-2).
        wxLIST_AUTOSIZE will resize the column to the length of its longest item.
        wxLIST_AUTOSIZE_USEHEADER
        will resize the column to the length of the header (Win32) or 80 pixels (other
        platforms).
        In small or normal icon view, @a col must be -1, and the column width is set
        for all columns.
    */
    bool SetColumnWidth(int col, int width);

    /**
        Sets the order of all columns at once. The @a orders array must have the
        same number elements as the number of columns and contain each position exactly
        once.
        This function is valid in report view only.
    */
    bool SetColumnOrder(const wxArrayInt& orders) const;

    /**
        Sets the image list associated with the control. @a which is one of
        wxIMAGE_LIST_NORMAL, wxIMAGE_LIST_SMALL, wxIMAGE_LIST_STATE (the last is
        unimplemented).
        This method does not take ownership of the image list, you have to
        delete it yourself.

        @see AssignImageList()
    */
    void SetImageList(wxImageList* imageList, int which);

    //@{
    /**
        Sets a string field at a particular column.
    */
    bool SetItem(wxListItem& info);
    long SetItem(long index, int col, const wxString& label,
                 int imageId = -1);
    m_mask m_state field is valid.





    wxLIST_MASK_TEXT




    The m_text field is valid.





    wxLIST_MASK_IMAGE




    The m_image field is valid.





    wxLIST_MASK_DATA




    The m_data field is valid.





    wxLIST_MASK_WIDTH




    The m_width field is valid.





    wxLIST_MASK_FORMAT




    The m_format field is valid.





The m_stateMask and m_state members take flags from the following:







    wxLIST_STATE_DONTCARE




    Don't care what the state is. Win32 only.





    wxLIST_STATE_DROPHILITED




    The item is highlighted to receive a drop event. Win32 only.





    wxLIST_STATE_FOCUSED




    The item has the focus.





    wxLIST_STATE_SELECTED




    The item is selected.





    wxLIST_STATE_CUT




    The item is in the cut state. Win32 only.





    The wxListItem object can also contain item-specific colour and font
    information: for this you need to call one of SetTextColour(),
    SetBackgroundColour() or SetFont() functions on it passing it the colour/font
    to use. If the colour/font is not specified, the default list control
    colour/font is used.
    long SetItem(long index, int col, const wxString& label,
    int imageId = -1);
    //@}

    /**
    Sets the background colour for this item. This function only works in report
    view.
    The colour can be retrieved using
    GetItemBackgroundColour().
    */
    void SetItemBackgroundColour(long item, const wxColour& col);

    /**
    Sets the image associated with the item. In report view, you can specify the
    column.
    The image is an index into the image list associated with the list control.
    */
    bool SetItemColumnImage(long item, long column, int image);

    /**
        This method can only be used with virtual list controls.

        It is used to indicate to the control the number of items it contains.
        After calling it, the main program should be ready to handle calls to
        various item callbacks (such as wxListCtrl::OnGetItemText) for all
        items in the range from 0 to @a count.

        Notice that the control is not necessarily redrawn after this call as
        it may be undesirable if an item which is not visible on the screen
        anyhow was added to or removed from a control displaying many items, if
        you do need to refresh the display you can just call Refresh() manually.
    */
    void SetItemCount(long count);

    /**
    Associates application-defined data with this item.
    Notice that this function cannot be used to associate pointers with the control
    items, use SetItemPtrData() instead.
    */
    bool SetItemData(long item, long data);

    /**
    Sets the item's font.
    */
    void SetItemFont(long item, const wxFont& font);

    //@{
    /**
        Sets the unselected and selected images associated with the item. The images
        are indices into the
        image list associated with the list control. This form is deprecated: @a
        selImage is not
        used.
    */
    bool SetItemImage(long item, int image);
    bool SetItemImage(long item, int image, int selImage);
    //@}

    /**
        Sets the position of the item, in icon or small icon view. Windows only.
    */
    bool SetItemPosition(long item, const wxPoint& pos);

    /**
        Associates application-defined data with this item. The @a data parameter may
        be either an integer or a pointer cast to the @c wxUIntPtr type which is
        guaranteed to be large enough to be able to contain all integer types and
        pointers.

        @since 2.8.4
    */
    bool SetItemPtrData(long item, wxUIntPtr data);

    /**
        Sets the item state. For a list of state flags, see SetItem().
        The @b stateMask indicates which state flags are valid.
    */
    bool SetItemState(long item, long state, long stateMask);

    /**
        Sets the item text for this item.
    */
    void SetItemText(long item, const wxString& text);

    /**
        Sets the colour for this item. This function only works in report view.
        The colour can be retrieved using
        GetItemTextColour().
    */
    void SetItemTextColour(long item, const wxColour& col);

    /**
        Adds or removes a single window style.
    */
    void SetSingleStyle(long style, bool add = true);

    /**
        Sets the text colour of the list control.
    */
    void SetTextColour(const wxColour& col);

    /**
        Sets the whole window style, deleting all items.
    */
    void SetWindowStyleFlag(long style);

    /**
        Call this function to sort the items in the list control. Sorting is done
        using the specified @a fnSortCallBack function. This function must have the
        following prototype:

        It is called each time when the two items must be compared and should return 0
        if the items are equal, negative value if the first item is less than the
        second one and positive value if the first one is greater than the second one
        (the same convention as used by @c qsort(3)).

        @param item1
            client data associated with the first item (NOT the index).
        @param item2
            client data associated with the second item (NOT the index).
        @param data
            the value passed to SortItems() itself.
    */
    bool SortItems(wxListCtrlCompare fnSortCallBack, long data);
};



/**
    @class wxListEvent

    A list event holds information about events associated with wxListCtrl objects.

    @library{wxbase}
    @category{events}

    @see wxListCtrl
*/
class wxListEvent : public wxNotifyEvent
{
public:
    /**
        Constructor.
    */
    wxListEvent(wxEventType commandType = 0, int id = 0);

    /**
        For @c EVT_LIST_CACHE_HINT event only: return the first item which the
        list control advises us to cache.
    */
    long GetCacheFrom() const;

    /**
        For @c EVT_LIST_CACHE_HINT event only: return the last item (inclusive)
        which the list control advises us to cache.
    */
    long GetCacheTo() const;

    /**
        The column position: it is only used with @c COL events. For the column
        dragging events, it is the column to the left of the divider being dragged, for
        the column click events it may be -1 if the user clicked in the list control
        header outside any column.
    */
    int GetColumn() const;

    /**
        The data.
    */
    long GetData() const;

    /**
        The image.
    */
    int GetImage() const;

    /**
        The item index.
    */
    long GetIndex() const;

    /**
        An item object, used by some events. See also wxListCtrl::SetItem.
    */
    const wxListItem GetItem() const;

    /**
        Key code if the event is a keypress event.
    */
    int GetKeyCode() const;

    /**
        The (new) item label for @c EVT_LIST_END_LABEL_EDIT event.
    */
    const wxString GetLabel() const;

    /**
        The mask.
    */
    long GetMask() const;

    /**
        The position of the mouse pointer if the event is a drag event.
    */
    wxPoint GetPoint() const;

    /**
        The text.
    */
    const wxString GetText() const;

    /**
        This method only makes sense for @c EVT_LIST_END_LABEL_EDIT message
        and returns @true if it the label editing has been cancelled by the user
        (GetLabel() returns an empty string in this case
        but it doesn't allow the application to distinguish between really cancelling
        the edit and
        the admittedly rare case when the user wants to rename it to an empty string).
    */
    bool IsEditCancelled() const;
};



/**
    @class wxListItemAttr

    Represents the attributes (color, font, ...) of a
    wxListCtrl wxListItem.

    @library{wxbase}
    @category{FIXME}

    @see @ref overview_listctrl "wxListCtrl Overview", wxListCtrl,
    wxListItem
*/
class wxListItemAttr
{
public:
    //@{
    /**
        Construct a wxListItemAttr with the specified foreground and
        background colors and font.
    */
    wxListItemAttr();
    wxListItemAttr(const wxColour colText,
                   const wxColour colBack,
                   const wxFont font);
    //@}

    /**
        Returns the currently set background color.
    */
    const wxColour GetBackgroundColour() const;

    /**
        Returns the currently set font.
    */
    const wxFont GetFont() const;

    /**
        Returns the currently set text color.
    */
    const wxColour GetTextColour() const;

    /**
        Returns @true if the currently set background color is valid.
    */
    bool HasBackgroundColour() const;

    /**
        Returns @true if the currently set font is valid.
    */
    bool HasFont() const;

    /**
        Returns @true if the currently set text color is valid.
    */
    bool HasTextColour() const;

    /**
        Sets a new background color.
    */
    void SetBackgroundColour(const wxColour& colour);

    /**
        Sets a new font.
    */
    void SetFont(const wxFont& font);

    /**
        Sets a new text color.
    */
    void SetTextColour(const wxColour& colour);
};



/**
    @class wxListView

    This class currently simply presents a simpler to use interface for the
    wxListCtrl -- it can be thought of as a @e faade
    for that complicated class. Using it is preferable to using
    wxListCtrl directly whenever possible because in the
    future some ports might implement wxListView but not the full set of wxListCtrl
    features.

    Other than different interface, this class is identical to wxListCtrl. In
    particular, it uses the same events, same window styles and so on.

    @library{wxcore}
    @category{ctrl}
    <!-- @appearance{listview.png} -->

    @see wxListView::SetColumnImage
*/
class wxListView : public wxListCtrl
{
public:
    /**
        Resets the column image -- after calling this function, no image will be shown.

        @param col
            the column to clear image for

        @see SetColumnImage()
    */
    void ClearColumnImage(int col);

    /**
        Sets focus to the item with the given @e index.
    */
    void Focus(long index);

    /**
        Returns the first selected item in a (presumably) multiple selection control.
        Together with GetNextSelected() it can be
        used to iterate over all selected items in the control.

        @return The first selected item, if any, -1 otherwise.
    */
    long GetFirstSelected() const;

    /**
        Returns the currently focused item or -1 if none.

        @see IsSelected(), Focus()
    */
    long GetFocusedItem() const;

    /**
        Used together with GetFirstSelected() to
        iterate over all selected items in the control.

        @return Returns the next selected item or -1 if there are no more of
                 them.
    */
    long GetNextSelected(long item) const;

    /**
        Returns @true if the item with the given @a index is selected,
        @false otherwise.

        @see GetFirstSelected(), GetNextSelected()
    */
    bool IsSelected(long index) const;

    /**
        Selects or unselects the given item.

        @param n
            the item to select or unselect
        @param on
            if @true (default), selects the item, otherwise unselects it

        @see wxListCtrl::SetItemState
    */
    void Select(bool on = true);

    /**
        Sets the column image for the specified column. To use the column images, the
        control must have a valid image list with at least one image.

        @param col
            the column to set image for
        @param image
            the index of the column image in the controls image list
    */
    void SetColumnImage(int col, int image);
};



/**
    @class wxListItem

    This class stores information about a wxListCtrl item or column.

    @library{wxbase}
    @category{FIXME}
*/
class wxListItem : public wxObject
{
public:
    /**
        Constructor.
    */
    wxListItem();

    /**
        Resets the item state to the default.
    */
    void Clear();

    /**
        Returns the alignment for this item. Can be one of
        wxLIST_FORMAT_LEFT, wxLIST_FORMAT_RIGHT or wxLIST_FORMAT_CENTRE.
    */
    wxListColumnFormat GetAlign() const;

    /**
        Returns the background colour for this item.
    */
    wxColour GetBackgroundColour() const;

    /**
        Returns the zero-based column; meaningful only in report mode.
    */
    int GetColumn() const;

    /**
        Returns client data associated with the control. Please note that
        client data is associated with the item and not with subitems.
    */
    long GetData() const;

    /**
        Returns the font used to display the item.
    */
    wxFont GetFont() const;

    /**
        Returns the zero-based item position.
    */
    long GetId() const;

    /**
        Returns the zero-based index of the image
        associated with the item into the image list.
    */
    int GetImage() const;

    /**
        Returns a bit mask indicating which fields of the structure are valid;
        can be any combination of the following values:

        wxLIST_MASK_STATE

        @b GetState is valid.

        wxLIST_MASK_TEXT

        @b GetText is valid.

        wxLIST_MASK_IMAGE

        @b GetImage is valid.

        wxLIST_MASK_DATA

        @b GetData is valid.

        wxLIST_MASK_WIDTH

        @b GetWidth is valid.

        wxLIST_MASK_FORMAT

        @b GetFormat is valid.
    */
    long GetMask() const;

    /**
        Returns a bit field representing the state of the item. Can be any
        combination of:

        wxLIST_STATE_DONTCARE

        Don't care what the state is. Win32 only.

        wxLIST_STATE_DROPHILITED

        The item is highlighted to receive a drop event. Win32 only.

        wxLIST_STATE_FOCUSED

        The item has the focus.

        wxLIST_STATE_SELECTED

        The item is selected.

        wxLIST_STATE_CUT

        The item is in the cut state. Win32 only.
    */
    long GetState() const;

    /**
        Returns the label/header text.
    */
    const wxString GetText() const;

    /**
        Returns the text colour.
    */
    wxColour GetTextColour() const;

    /**
        Meaningful only for column headers in report mode. Returns the column width.
    */
    int GetWidth() const;

    /**
        Sets the alignment for the item. See also
        GetAlign()
    */
    void SetAlign(wxListColumnFormat align);

    /**
        Sets the background colour for the item.
    */
    void SetBackgroundColour(const wxColour& colBack);

    /**
        Sets the zero-based column. Meaningful only in report mode.
    */
    void SetColumn(int col);

    //@{
    /**
        Sets client data for the item. Please note that
        client data is associated with the item and not with subitems.
    */
    void SetData(long data);
    void SetData(void* data);
    //@}

    /**
        Sets the font for the item.
    */
    void SetFont(const wxFont& font);

    /**
        Sets the zero-based item position.
    */
    void SetId(long id);

    /**
        Sets the zero-based index of the image associated with the item
        into the image list.
    */
    void SetImage(int image);

    /**
        Sets the mask of valid fields. See GetMask().
    */
    void SetMask(long mask);

    /**
        Sets the item state flags (note that the valid state flags are influenced
        by the value of the state mask, see
        wxListItem::SetStateMask).
        See GetState() for valid flag
        values.
    */
    void SetState(long state);

    /**
        Sets the bitmask that is used to determine which of the state flags
        are to be set. See also SetState().
    */
    void SetStateMask(long stateMask);

    /**
        Sets the text label for the item.
    */
    void SetText(const wxString& text);

    /**
        Sets the text colour for the item.
    */
    void SetTextColour(const wxColour& colText);

    /**
        Meaningful only for column headers in report mode. Sets the column width.
    */
    void SetWidth(int width);
};

