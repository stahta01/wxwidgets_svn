/////////////////////////////////////////////////////////////////////////////
// Name:        menuitem.h
// Purpose:     interface of wxMenu, wxMenuItem
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxMenuItem

    A menu item represents an item in a menu.

    Note that you usually don't have to deal with it directly as wxMenu methods
    usually construct an object of this class for you.

    Also please note that the methods related to fonts and bitmaps are currently
    only implemented for Windows, Mac and GTK+.

    @beginEventEmissionTable{wxCommandEvent,wxMenuEvent}
    @event{EVT_MENU(id, func)}
        Process a @c wxEVT_COMMAND_MENU_SELECTED command, which is generated by a menu item.
        This type of event is sent as wxCommandEvent.
    @event{EVT_MENU_RANGE(id1, id2, func)}
        Process a @c wxEVT_COMMAND_MENU_RANGE command, which is generated by a range of menu items.
        This type of event is sent as wxCommandEvent.
    @event{EVT_MENU_OPEN(func)}
        A menu is about to be opened. On Windows, this is only sent once for each
        navigation of the menubar (up until all menus have closed).
        This type of event is sent as wxMenuEvent.
    @event{EVT_MENU_CLOSE(func)}
        A menu has been just closed.
        This type of event is sent as wxMenuEvent.
    @event{EVT_MENU_HIGHLIGHT(id, func)}
        The menu item with the specified id has been highlighted: used to show
        help prompts in the status bar by wxFrame
        This type of event is sent as wxMenuEvent.
    @event{EVT_MENU_HIGHLIGHT_ALL(func)}
        A menu item has been highlighted, i.e. the currently selected menu item has changed.
        This type of event is sent as wxMenuEvent.
    @endEventTable

    @library{wxcore}
    @category{menus}

    @see wxMenuBar, wxMenu
*/
class wxMenuItem : public wxObject
{
public:
    /**
        Constructs a wxMenuItem object.

        Menu items can be standard, or "stock menu items", or custom.
        For the standard menu items (such as commands to open a file, exit the
        program and so on, see @ref page_stockitems for the full list) it is enough
        to specify just the stock ID and leave @a text and @a helpString empty.
        Some platforms (currently wxGTK only, and see the remark in SetBitmap()
        documentation) will also show standard bitmaps for stock menu items.

        Leaving at least @a text empty for the stock menu items is actually strongly
        recommended as they will have appearance and keyboard interface (including
        standard accelerators) familiar to the user.

        For the custom (non-stock) menu items, @a text must be specified and while
        @a helpString may be left empty, it's recommended to pass the item
        description (which is automatically shown by the library in the status bar
        when the menu item is selected) in this parameter.

        Finally note that you can e.g. use a stock menu label without using its stock
        help string:

        @code
        // use all stock properties:
        helpMenu->Append(wxID_ABOUT);

        // use the stock label and the stock accelerator but not the stock help string:
        helpMenu->Append(wxID_ABOUT, "", "My custom help string");

        // use all stock properties except for the bitmap:
        wxMenuItem *mymenu = new wxMenuItem(helpMenu, wxID_ABOUT);
        mymenu->SetBitmap(wxArtProvider::GetBitmap(wxART_WARNING));
        helpMenu->Append(mymenu);
        @endcode

        that is, stock properties are set independently one from the other.

        @param parentMenu
            Menu that the menu item belongs to. Can be @NULL if the item is
            going to be added to the menu later.
        @param id
            Identifier for this menu item. May be @c wxID_SEPARATOR, in which
            case the given kind is ignored and taken to be @c wxITEM_SEPARATOR
            instead.
        @param text
            Text for the menu item, as shown on the menu.
            See SetItemLabel() for more info.
        @param helpString
            Optional help string that will be shown on the status bar.
        @param kind
            May be @c wxITEM_SEPARATOR, @c wxITEM_NORMAL, @c wxITEM_CHECK or
            @c wxITEM_RADIO.
        @param subMenu
            If non-@NULL, indicates that the menu item is a submenu.
    */
    wxMenuItem(wxMenu* parentMenu = NULL, int id = wxID_SEPARATOR,
               const wxString& text = wxEmptyString,
               const wxString& helpString = wxEmptyString,
               wxItemKind kind = wxITEM_NORMAL,
               wxMenu* subMenu = NULL);

    /**
        Destructor.
    */
    virtual ~wxMenuItem();

    /**
        Checks or unchecks the menu item.
        Note that this only works when the item is already appended to a menu.
    */
    virtual void Check(bool check = true);

    /**
        Enables or disables the menu item.
    */
    virtual void Enable(bool enable = true);

    /**
        @deprecated This function is deprecated; please use GetLabelText() instead.

        @see GetLabelText()
    */
    static wxString GetLabelFromText(const wxString& text);

    /**
        Strips all accelerator characters and mnemonics from the given @a text.
        For example:

        @code
          wxMenuItem::GetLabelfromText("&Hello\tCtrl-h");
        @endcode

        will return just @c "Hello".

        @see GetItemLabelText(), GetItemLabel()
    */
    static wxString GetLabelText(const wxString& text);
    
    
    /**
        @name Getters
    */
    //@{
    
    /**
        Returns the background colour associated with the menu item.
        
        @onlyfor{wxmsw}
    */
    wxColour& GetBackgroundColour() const;

    /**
        Returns the checked or unchecked bitmap.
        
        @onlyfor{wxmsw}
    */
    virtual const wxBitmap& GetBitmap() const;

    /**
        Returns the font associated with the menu item.
        
        @onlyfor{wxmsw}
    */
    wxFont& GetFont() const;

    /**
        Returns the help string associated with the menu item.
    */
    const wxString& GetHelp() const;

    /**
        Returns the menu item identifier.
    */
    int GetId() const;

    /**
        Returns the text associated with the menu item including any accelerator
        characters that were passed to the constructor or SetItemLabel().

        @see GetItemLabelText(), GetLabelText()
    */
    virtual wxString GetItemLabel() const;

    /**
        Returns the text associated with the menu item, without any accelerator
        characters.

        @see GetItemLabel(), GetLabelText()
    */
    virtual wxString GetItemLabelText() const;

    /**
        Returns the item kind, one of @c wxITEM_SEPARATOR, @c wxITEM_NORMAL,
        @c wxITEM_CHECK or @c wxITEM_RADIO.
    */
    wxItemKind GetKind() const;

    /**
        Returns the text associated with the menu item without any accelerator
        characters it might contain.

        @deprecated This function is deprecated in favour of GetItemLabelText().

        @see GetItemLabelText()
    */
    wxString GetLabel() const;

    /**
        Gets the width of the menu item checkmark bitmap.
        
        @onlyfor{wxmsw}
    */
    int GetMarginWidth() const;

    /**
        Returns the menu this menu item is in, or @NULL if this menu item is not
        attached.
    */
    wxMenu* GetMenu() const;

    /**
        Returns the text associated with the menu item.

        @deprecated This function is deprecated. Please use GetItemLabel() or GetItemLabelText() instead.
        
        @see GetItemLabel(), GetItemLabelText()
    */
    wxString GetName() const;

    /**
        Returns the submenu associated with the menu item, or @NULL if there isn't one.
    */
    wxMenu* GetSubMenu() const;

    /**
        Returns the text associated with the menu item, such as it was passed to the
        wxMenuItem constructor, i.e. with any accelerator characters it may contain.

        @deprecated This function is deprecated in favour of GetItemLabel().

        @see GetLabelFromText()
    */
    const wxString& GetText() const;

    /**
        Returns the text colour associated with the menu item.
        
        @onlyfor{wxmsw}
    */
    wxColour& GetTextColour() const;
    
    //@}
    
    
    
    /**
        @name Checkers
    */
    //@{

    /**
        Returns @true if the item is checkable.
    */
    bool IsCheckable() const;

    /**
        Returns @true if the item is checked.
    */
    virtual bool IsChecked() const;

    /**
        Returns @true if the item is enabled.
    */
    virtual bool IsEnabled() const;

    /**
        Returns @true if the item is a separator.
    */
    bool IsSeparator() const;

    /**
        Returns @true if the item is a submenu.
    */
    bool IsSubMenu() const;
    
    //@}
    
    
    
    /**
        @name Setters
    */
    //@{

    /**
        Sets the background colour associated with the menu item.
        
        @onlyfor{wxmsw}
    */
    void SetBackgroundColour(const wxColour& colour);

    /**
        Sets the bitmap for the menu item.

        It is equivalent to wxMenuItem::SetBitmaps(bmp, wxNullBitmap) if 
        @a checked is @true (default value) or SetBitmaps(wxNullBitmap, bmp)
        otherwise.

        Notice that GTK+ uses a global setting called @c gtk-menu-images to
        determine if the images should be shown in the menus at all. If it is
        off (which is the case in e.g. Gnome 2.28 by default), no images will
        be shown, consistently with the native behaviour.

        @onlyfor{wxmsw,wxosx,wxgtk}
    */
    virtual void SetBitmap(const wxBitmap& bmp, bool checked = true);

    /**
        Sets the checked/unchecked bitmaps for the menu item.
        The first bitmap is also used as the single bitmap for uncheckable menu items.

        @onlyfor{wxmsw}
    */
    void SetBitmaps(const wxBitmap& checked,
                    const wxBitmap& unchecked = wxNullBitmap);

    /**
        Sets the font associated with the menu item.
        
        @onlyfor{wxmsw}
    */
    void SetFont(const wxFont& font);

    /**
        Sets the help string.
    */
    void SetHelp(const wxString& helpString);

    /**
        Sets the label associated with the menu item.
        
        Note that if the ID of this menu item corresponds to a stock ID, then it is 
        not necessary to specify a label: wxWidgets will automatically use the stock
        item label associated with that ID. See the @ref wxMenuItem::wxMenuItem "constructor"
        for more info.
        
        The label string for the normal menu items (not separators) may include the 
        accelerator which can be used to activate the menu item from keyboard.
        An accelerator key can be specified using the ampersand <tt>&</tt> character. 
        In order to embed an ampersand character in the menu item text, the ampersand 
        must be doubled.
        
        Optionally you can specify also an accelerator string appending a tab character 
        <tt>\\t</tt> followed by a valid key combination (e.g. <tt>CTRL+V</tt>).
        Its general syntax is any combination of @c "CTRL", @c "ALT" and @c "SHIFT" strings
        (case doesn't matter) separated by either @c '-' or @c '+' characters and followed
        by the accelerator itself.
        The accelerator may be any alphanumeric character, any function key
        (from F1 to F12) or one of the special characters listed in the table
        below (again, case doesn't matter):
        - @c DEL or @c DELETE: Delete key
        - @c BACK : Backspace key
        - @c INS or @c INSERT: Insert key
        - @c ENTER or @c RETURN: Enter key
        - @c PGUP: PageUp key
        - @c PGDN: PageDown key
        - @c LEFT: Left cursor arrow key
        - @c RIGHT: Right cursor arrow key
        - @c UP: Up cursor arrow key
        - @c DOWN: Down cursor arrow key
        - @c HOME: Home key
        - @c END: End key
        - @c SPACE: Space
        - @c TAB: Tab key
        - @c ESC or @c ESCAPE: Escape key (Windows only)

        Examples:

        @code
        m_pMyMenuItem->SetItemLabel("My &item\tCTRL+I");
        m_pMyMenuItem2->SetItemLabel("Clean && build\tF7");
        m_pMyMenuItem3->SetItemLabel("Simple item");
        m_pMyMenuItem4->SetItemLabel("Item with &accelerator");
        @endcode
        
        @see GetItemLabel(), GetItemLabelText()
    */
    virtual void SetItemLabel(const wxString& label);

    /**
        Sets the width of the menu item checkmark bitmap.
        
        @onlyfor{wxmsw}
    */
    void SetMarginWidth(int width);

    /**
        Sets the parent menu which will contain this menu item.
    */
    void SetMenu(wxMenu* menu);

    /**
        Sets the submenu of this menu item.
    */
    void SetSubMenu(wxMenu* menu);

    /**
        Sets the text associated with the menu item.

        @deprecated This function is deprecated in favour of SetItemLabel().
        
        @see SetItemLabel().
    */
    virtual void SetText(const wxString& text);

    /**
        Sets the text colour associated with the menu item.
        
        @onlyfor{wxmsw}
    */
    void SetTextColour(const wxColour& colour);
    
    //@}
};

