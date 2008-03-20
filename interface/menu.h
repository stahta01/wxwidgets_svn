/////////////////////////////////////////////////////////////////////////////
// Name:        menu.h
// Purpose:     interface of wxMenuBar
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxMenuBar
    @wxheader{menu.h}

    A menu bar is a series of menus accessible from the top of a frame.

    @library{wxcore}
    @category{menus}

    @see wxMenu, @ref overview_eventhandlingoverview
*/
class wxMenuBar : public wxWindow
{
public:
    //@{
    /**
        Construct a menu bar from arrays of menus and titles.

        @param n
            The number of menus.
        @param menus
            An array of menus. Do not use this array again - it now belongs to the
            menu bar.
        @param titles
            An array of title strings. Deallocate this array after creating the menu
        bar.
        @param style
            If wxMB_DOCKABLE the menu bar can be detached (wxGTK only).
    */
    wxMenuBar(long style = 0);
    wxMenuBar(size_t n, wxMenu* menus[], const wxString titles[],
              long style = 0);
    //@}

    /**
        Destructor, destroying the menu bar and removing it from the parent frame (if
        any).
    */
    ~wxMenuBar();

    /**
        Adds the item to the end of the menu bar.

        @param menu
            The menu to add. Do not deallocate this menu after calling Append.
        @param title
            The title of the menu.

        @returns @true on success, @false if an error occurred.

        @see Insert()
    */
    bool Append(wxMenu* menu, const wxString& title);

    /**
        Checks or unchecks a menu item.

        @param id
            The menu item identifier.
        @param check
            If @true, checks the menu item, otherwise the item is unchecked.

        @remarks Only use this when the menu bar has been associated with a
                 frame; otherwise, use the wxMenu equivalent call.
    */
    void Check(int id, const bool check);

    /**
        Enables or disables (greys out) a menu item.

        @param id
            The menu item identifier.
        @param enable
            @true to enable the item, @false to disable it.

        @remarks Only use this when the menu bar has been associated with a
                 frame; otherwise, use the wxMenu equivalent call.
    */
    void Enable(int id, const bool enable);

    /**
        Enables or disables a whole menu.

        @param pos
            The position of the menu, starting from zero.
        @param enable
            @true to enable the menu, @false to disable it.

        @remarks Only use this when the menu bar has been associated with a frame.
    */
    void EnableTop(int pos, const bool enable);

    /**
        Finds the menu item object associated with the given menu item identifier.

        @param id
            Menu item identifier.
        @param menu
            If not @NULL, menu will get set to the associated menu.

        @returns The found menu item object, or @NULL if one was not found.
    */
    wxMenuItem* FindItem(int id, wxMenu menu = NULL) const;

    /**
        Returns the index of the menu with the given @a title or @c wxNOT_FOUND if no
        such menu exists in this menubar. The @a title parameter may specify either
        the menu title (with accelerator characters, i.e. @c "File") or just the
        menu label (@c "File") indifferently.
    */
    int FindMenu(const wxString& title) const;

    /**
        Finds the menu item id for a menu name/menu item string pair.

        @param menuString
            Menu title to find.
        @param itemString
            Item to find.

        @returns The menu item identifier, or wxNOT_FOUND if none was found.

        @remarks Any special menu codes are stripped out of source and target
                 strings before matching.
    */
    int FindMenuItem(const wxString& menuString,
                     const wxString& itemString) const;

    /**
        Gets the help string associated with the menu item identifier.

        @param id
            The menu item identifier.

        @returns The help string, or the empty string if there was no help string
                 or the menu item was not found.

        @see SetHelpString()
    */
    wxString GetHelpString(int id) const;

    /**
        Gets the label associated with a menu item.

        @param id
            The menu item identifier.

        @returns The menu item label, or the empty string if the item was not
                 found.

        @remarks Use only after the menubar has been associated with a frame.
    */
    wxString GetLabel(int id) const;

    /**
        Returns the label of a top-level menu. Note that the returned string does not
        include the accelerator characters which could have been specified in the menu
        title string during its construction.

        @param pos
            Position of the menu on the menu bar, starting from zero.

        @returns The menu label, or the empty string if the menu was not found.

        @remarks Use only after the menubar has been associated with a frame.

        @see SetLabelTop()
    */
    wxString GetLabelTop(int pos) const;

    /**
        Returns the menu at @a menuIndex (zero-based).
    */
    wxMenu* GetMenu(int menuIndex) const;

    /**
        Returns the number of menus in this menubar.
    */
    size_t GetMenuCount() const;

    /**
        Returns the label of a top-level menu. Note that the returned string
        includes the accelerator characters that have been specified in the menu
        title string during its construction.

        @param pos
            Position of the menu on the menu bar, starting from zero.

        @returns The menu label, or the empty string if the menu was not found.

        @remarks Use only after the menubar has been associated with a frame.

        @see GetMenuLabelText(), SetMenuLabel()
    */
    wxString GetMenuLabel(int pos) const;

    /**
        Returns the label of a top-level menu. Note that the returned string does not
        include any accelerator characters that may have been specified in the menu
        title string during its construction.

        @param pos
            Position of the menu on the menu bar, starting from zero.

        @returns The menu label, or the empty string if the menu was not found.

        @remarks Use only after the menubar has been associated with a frame.

        @see GetMenuLabel(), SetMenuLabel()
    */
    wxString GetMenuLabelText(int pos) const;

    /**
        Inserts the menu at the given position into the menu bar. Inserting menu at
        position 0 will insert it in the very beginning of it, inserting at position
        GetMenuCount() is the same as calling
        Append().

        @param pos
            The position of the new menu in the menu bar
        @param menu
            The menu to add. wxMenuBar owns the menu and will free it.
        @param title
            The title of the menu.

        @returns @true on success, @false if an error occurred.

        @see Append()
    */
    bool Insert(size_t pos, wxMenu* menu, const wxString& title);

    /**
        Determines whether an item is checked.

        @param id
            The menu item identifier.

        @returns @true if the item was found and is checked, @false otherwise.
    */
    bool IsChecked(int id) const;

    /**
        Determines whether an item is enabled.

        @param id
            The menu item identifier.

        @returns @true if the item was found and is enabled, @false otherwise.
    */
    bool IsEnabled(int id) const;

    /**
        Redraw the menu bar
    */
    void Refresh();

    /**
        Removes the menu from the menu bar and returns the menu object - the caller is
        responsible for deleting it. This function may be used together with
        Insert() to change the menubar
        dynamically.

        @see Replace()
    */
    wxMenu* Remove(size_t pos);

    /**
        Replaces the menu at the given position with another one.

        @param pos
            The position of the new menu in the menu bar
        @param menu
            The menu to add.
        @param title
            The title of the menu.

        @returns The menu which was previously at position pos. The caller is
                 responsible for deleting it.

        @see Insert(), Remove()
    */
    wxMenu* Replace(size_t pos, wxMenu* menu, const wxString& title);

    /**
        Sets the help string associated with a menu item.

        @param id
            Menu item identifier.
        @param helpString
            Help string to associate with the menu item.

        @see GetHelpString()
    */
    void SetHelpString(int id, const wxString& helpString);

    /**
        Sets the label of a menu item.

        @param id
            Menu item identifier.
        @param label
            Menu item label.

        @remarks Use only after the menubar has been associated with a frame.

        @see GetLabel()
    */
    void SetLabel(int id, const wxString& label);

    /**
        Sets the label of a top-level menu.

        @param pos
            The position of a menu on the menu bar, starting from zero.
        @param label
            The menu label.

        @remarks Use only after the menubar has been associated with a frame.

        @see GetLabelTop()
    */
    void SetLabelTop(int pos, const wxString& label);

    /**
        Sets the label of a top-level menu.

        @param pos
            The position of a menu on the menu bar, starting from zero.
        @param label
            The menu label.

        @remarks Use only after the menubar has been associated with a frame.
    */
    void SetMenuLabel(int pos, const wxString& label);
};



/**
    @class wxMenu
    @wxheader{menu.h}

    A menu is a popup (or pull down) list of items, one of which may be
    selected before the menu goes away (clicking elsewhere dismisses the
    menu). Menus may be used to construct either menu bars or popup menus.

    A menu item has an integer ID associated with it which can be used to
    identify the selection, or to change the menu item in some way. A menu item
    with a special identifier -1 is a separator item and doesn't have an
    associated command but just makes a separator line appear in the menu.

    @b NB: Please note that @e wxID_ABOUT and @e wxID_EXIT are
    predefined by wxWidgets and have a special meaning since entries
    using these IDs will be taken out of the normal menus under MacOS X
    and will be inserted into the system menu (following the appropriate
    MacOS X interface guideline). On PalmOS @e wxID_EXIT is disabled according
    to Palm OS Companion guidelines.

    Menu items may be either normal items, check items or radio items. Normal items
    don't have any special properties while the check items have a boolean flag
    associated to them and they show a checkmark in the menu when the flag is set.
    wxWidgets automatically toggles the flag value when the item is clicked and its
    value may be retrieved using either wxMenu::IsChecked method
    of wxMenu or wxMenuBar itself or by using
    wxEvent::IsChecked when you get the menu
    notification for the item in question.

    The radio items are similar to the check items except that all the other items
    in the same radio group are unchecked when a radio item is checked. The radio
    group is formed by a contiguous range of radio items, i.e. it starts at the
    first item of this kind and ends with the first item of a different kind (or
    the end of the menu). Notice that because the radio groups are defined in terms
    of the item positions inserting or removing the items in the menu containing
    the radio items risks to not work correctly. Finally note that radio items
    are not supported under Motif.

    @library{wxcore}
    @category{menus}

    @see wxMenuBar, wxWindow::PopupMenu, @ref overview_eventhandlingoverview, @ref
    overview_wxfilehistory "wxFileHistory (most recently used files menu)"
*/
class wxMenu : public wxEvtHandler
{
public:
    //@{
    /**
        Constructs a wxMenu object.

        @param style
            If set to wxMENU_TEAROFF, the menu will be detachable (wxGTK only).
    */
    wxMenu(const wxString& title = "", long style = 0);
    wxMenu(long style);
    //@}

    /**
        Destructor, destroying the menu.
        Note: under Motif, a popup menu must have a valid parent (the window
        it was last popped up on) when being destroyed. Therefore, make sure
        you delete or re-use the popup menu @e before destroying the
        parent window. Re-use in this context means popping up the menu on
        a different window from last time, which causes an implicit destruction
        and recreation of internal data structures.
    */
    ~wxMenu();

    //@{
    /**
        Adds a menu item object. This is the most generic variant of Append() method
        because it may be used for both items (including separators) and submenus and
        because you can also specify various extra properties of a menu item this way,
        such as bitmaps and fonts.

        @param id
            The menu command identifier.
        @param item
            The string to appear on the menu item.
        @param menu
            Pull-right submenu.
        @param kind
            May be wxITEM_SEPARATOR, wxITEM_NORMAL,
            wxITEM_CHECK or wxITEM_RADIO
        @param helpString
            An optional help string associated with the item.
            By default, the handler for the wxEVT_MENU_HIGHLIGHT event displays
            this string in the status line.
        @param menuItem
            A menuitem object. It will be owned by the wxMenu object after this function
            is called, so do not delete it yourself.

        @remarks This command can be used after the menu has been shown, as well
                 as on initial creation of a menu or menubar.

        @see AppendSeparator(), AppendCheckItem(), AppendRadioItem(),
             AppendSubMenu(), Insert(), SetLabel(),
             GetHelpString(), SetHelpString(), wxMenuItem
    */
    wxMenuItem* Append(int id, const wxString& item = "",
                       const wxString& helpString = "",
                       wxItemKind kind = wxITEM_NORMAL);
    wxMenuItem* Append(int id, const wxString& item,
                       wxMenu* subMenu,
                       const wxString& helpString = "");
    wxMenuItem* Append(wxMenuItem* menuItem);
    //@}

    /**
        Adds a checkable item to the end of the menu.

        @see Append(), InsertCheckItem()
    */
    wxMenuItem* AppendCheckItem(int id, const wxString& item,
                                const wxString& helpString = "");

    /**
        Adds a radio item to the end of the menu. All consequent radio items form a
        group and when an item in the group is checked, all the others are
        automatically unchecked.

        @see Append(), InsertRadioItem()
    */
    wxMenuItem* AppendRadioItem(int id, const wxString& item,
                                const wxString& helpString = "");

    /**
        Adds a separator to the end of the menu.

        @see Append(), InsertSeparator()
    */
    wxMenuItem* AppendSeparator();

    /**
        Adds the given @a submenu to this menu. @a text is the text shown in the
        menu for it and @a help is the help string shown in the status bar when the
        submenu item is selected.
    */
    wxMenuItem* AppendSubMenu(wxMenu* submenu, const wxString& text,
                              const wxString& help = wxEmptyString);

    /**
        Inserts a break in a menu, causing the next appended item to appear in a new
        column.
    */
    void Break();

    /**
        Checks or unchecks the menu item.

        @param id
            The menu item identifier.
        @param check
            If @true, the item will be checked, otherwise it will be unchecked.

        @see IsChecked()
    */
    void Check(int id, const bool check);

    //@{
    /**
        Deletes the menu item from the menu. If the item is a submenu, it will
        @b not be deleted. Use Destroy() if you want to
        delete a submenu.

        @param id
            Id of the menu item to be deleted.
        @param item
            Menu item to be deleted.

        @see FindItem(), Destroy(), Remove()
    */
    void Delete(int id);
    void Delete(wxMenuItem* item);
    //@}

    //@{
    /**
        Deletes the menu item from the menu. If the item is a submenu, it will
        be deleted. Use Remove() if you want to keep the submenu
        (for example, to reuse it later).

        @param id
            Id of the menu item to be deleted.
        @param item
            Menu item to be deleted.

        @see FindItem(), Deletes(), Remove()
    */
    void Destroy(int id);
    void Destroy(wxMenuItem* item);
    //@}

    /**
        Enables or disables (greys out) a menu item.

        @param id
            The menu item identifier.
        @param enable
            @true to enable the menu item, @false to disable it.

        @see IsEnabled()
    */
    void Enable(int id, const bool enable);

    //@{
    /**
        Finds the menu item object associated with the given menu item identifier and,
        optionally, the (sub)menu it belongs to.

        @param itemString
            Menu item string to find.
        @param id
            Menu item identifier.
        @param menu
            If the pointer is not @NULL, it will be filled with the item's
            parent menu (if the item was found)

        @returns First form: menu item identifier, or wxNOT_FOUND if none is
                 found.

        @remarks Any special menu codes are stripped out of source and target
                 strings before matching.
    */
    int FindItem(const wxString& itemString) const;
    const wxMenuItem *  FindItem(int id, wxMenu** menu = NULL) const;
    //@}

    /**
        Returns the wxMenuItem given a position in the menu.
    */
    wxMenuItem* FindItemByPosition(size_t position) const;

    /**
        Returns the help string associated with a menu item.

        @param id
            The menu item identifier.

        @returns The help string, or the empty string if there is no help string
                 or the item was not found.

        @see SetHelpString(), Append()
    */
    wxString GetHelpString(int id) const;

    /**
        Returns a menu item label.

        @param id
            The menu item identifier.

        @returns The item label, or the empty string if the item was not found.

        @see GetLabelText(), SetLabel()
    */
    wxString GetLabel(int id) const;

    /**
        Returns a menu item label, without any of the original mnemonics and
        accelerators.

        @param id
            The menu item identifier.

        @returns The item label, or the empty string if the item was not found.

        @see GetLabel(), SetLabel()
    */
    wxString GetLabelText(int id) const;

    /**
        Returns the number of items in the menu.
    */
    size_t GetMenuItemCount() const;

    /**
        Returns the list of items in the menu. wxMenuItemList is a pseudo-template
        list class containing wxMenuItem pointers, see wxList.
    */
    wxMenuItemList GetMenuItems() const;

    /**
        Returns the title of the menu.

        @remarks This is relevant only to popup menus, use
                 wxMenuBar::GetMenuLabel for the menus in the menubar.

        @see SetTitle()
    */
    wxString GetTitle() const;

    //@{
    /**
        Inserts the given @a item before the position @e pos. Inserting the item
        at position GetMenuItemCount() is the same
        as appending it.

        @see Append(), Prepend()
    */
    wxMenuItem* Insert(size_t pos, wxMenuItem* item);
    wxMenuItem* Insert(size_t pos, int id,
                       const wxString& item = "",
                       const wxString& helpString = "",
                       wxItemKind kind = wxITEM_NORMAL);
    //@}

    /**
        Inserts a checkable item at the given position.

        @see Insert(), AppendCheckItem()
    */
    wxMenuItem* InsertCheckItem(size_t pos, int id,
                                const wxString& item,
                                const wxString& helpString = "");

    /**
        Inserts a radio item at the given position.

        @see Insert(), AppendRadioItem()
    */
    wxMenuItem* InsertRadioItem(size_t pos, int id,
                                const wxString& item,
                                const wxString& helpString = "");

    /**
        Inserts a separator at the given position.

        @see Insert(), AppendSeparator()
    */
    wxMenuItem* InsertSeparator(size_t pos);

    /**
        Determines whether a menu item is checked.

        @param id
            The menu item identifier.

        @returns @true if the menu item is checked, @false otherwise.

        @see Check()
    */
    bool IsChecked(int id) const;

    /**
        Determines whether a menu item is enabled.

        @param id
            The menu item identifier.

        @returns @true if the menu item is enabled, @false otherwise.

        @see Enable()
    */
    bool IsEnabled(int id) const;

    //@{
    /**
        Inserts the given @a item at position 0, i.e. before all the other
        existing items.

        @see Append(), Insert()
    */
    wxMenuItem* Prepend(wxMenuItem* item);
    wxMenuItem* Prepend(int id, const wxString& item = "",
                        const wxString& helpString = "",
                        wxItemKind kind = wxITEM_NORMAL);
    //@}

    /**
        Inserts a checkable item at position 0.

        @see Prepend(), AppendCheckItem()
    */
    wxMenuItem* PrependCheckItem(int id, const wxString& item,
                                 const wxString& helpString = "");

    /**
        Inserts a radio item at position 0.

        @see Prepend(), AppendRadioItem()
    */
    wxMenuItem* PrependRadioItem(int id, const wxString& item,
                                 const wxString& helpString = "");

    /**
        Inserts a separator at position 0.

        @see Prepend(), AppendSeparator()
    */
    wxMenuItem* PrependSeparator();

    //@{
    /**
        Removes the menu item from the menu but doesn't delete the associated C++
        object. This allows to reuse the same item later by adding it back to the menu
        (especially useful with submenus).

        @param id
            The identifier of the menu item to remove.
        @param item
            The menu item to remove.

        @returns The item which was detached from the menu.
    */
    wxMenuItem* Remove(int id);
    wxMenuItem* Remove(wxMenuItem* item);
    //@}

    /**
        Sets an item's help string.

        @param id
            The menu item identifier.
        @param helpString
            The help string to set.

        @see GetHelpString()
    */
    void SetHelpString(int id, const wxString& helpString);

    /**
        Sets the label of a menu item.

        @param id
            The menu item identifier.
        @param label
            The menu item label to set.

        @see Append(), GetLabel()
    */
    void SetLabel(int id, const wxString& label);

    /**
        Sets the title of the menu.

        @param title
            The title to set.

        @remarks This is relevant only to popup menus, use
                 wxMenuBar::SetLabelTop for the menus in the menubar.

        @see GetTitle()
    */
    void SetTitle(const wxString& title);

    /**
        Sends events to @a source (or owning window if @NULL) to update the
        menu UI. This is called just before the menu is popped up with
        wxWindow::PopupMenu, but
        the application may call it at other times if required.
    */
    void UpdateUI(wxEvtHandler* source = NULL) const;
};

