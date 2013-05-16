/////////////////////////////////////////////////////////////////////////////
// Name:        wx/richtext/richtextctrl.h
// Purpose:     A rich edit control
// Author:      Julian Smart
// Modified by:
// Created:     2005-09-30
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/*
 * Styles and flags
 */

/**
    Styles
*/

#define wxRE_READONLY          0x0010
#define wxRE_MULTILINE         0x0020
#define wxRE_CENTRE_CARET      0x8000
#define wxRE_CENTER_CARET      wxRE_CENTRE_CARET

/**
    Flags
*/

#define wxRICHTEXT_SHIFT_DOWN  0x01
#define wxRICHTEXT_CTRL_DOWN   0x02
#define wxRICHTEXT_ALT_DOWN    0x04

/**
    Extra flags
*/

// Don't draw guide lines around boxes and tables
#define wxRICHTEXT_EX_NO_GUIDELINES 0x00000100


/*
    Defaults
*/

#define wxRICHTEXT_DEFAULT_OVERALL_SIZE wxSize(-1, -1)
#define wxRICHTEXT_DEFAULT_IMAGE_SIZE wxSize(80, 80)
#define wxRICHTEXT_DEFAULT_SPACING 3
#define wxRICHTEXT_DEFAULT_MARGIN 3
#define wxRICHTEXT_DEFAULT_UNFOCUSSED_BACKGROUND wxColour(175, 175, 175)
#define wxRICHTEXT_DEFAULT_FOCUSSED_BACKGROUND wxColour(140, 140, 140)
#define wxRICHTEXT_DEFAULT_UNSELECTED_BACKGROUND wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE)
#define wxRICHTEXT_DEFAULT_TYPE_COLOUR wxColour(0, 0, 200)
#define wxRICHTEXT_DEFAULT_FOCUS_RECT_COLOUR wxColour(100, 80, 80)
#define wxRICHTEXT_DEFAULT_CARET_WIDTH 2
// Minimum buffer size before delayed layout kicks in
#define wxRICHTEXT_DEFAULT_DELAYED_LAYOUT_THRESHOLD 20000
// Milliseconds before layout occurs after resize
#define wxRICHTEXT_DEFAULT_LAYOUT_INTERVAL 50

/* Identifiers
 */
#define wxID_RICHTEXT_PROPERTIES1   (wxID_HIGHEST + 1)
#define wxID_RICHTEXT_PROPERTIES2   (wxID_HIGHEST + 2)
#define wxID_RICHTEXT_PROPERTIES3   (wxID_HIGHEST + 3)

/*
    Normal selection occurs initially and as user drags within one container.
    Common ancestor selection occurs when the user starts dragging across containers
    that have a common ancestor, for example the cells in a table.
 */

enum wxRichTextCtrlSelectionState
{
    wxRichTextCtrlSelectionState_Normal,
    wxRichTextCtrlSelectionState_CommonAncestor
};

/**
    @class wxRichTextContextMenuPropertiesInfo

    wxRichTextContextMenuPropertiesInfo keeps track of objects that appear in the context menu,
    whose properties are available to be edited.
 */

class wxRichTextContextMenuPropertiesInfo
{
public:
    /**
        Constructor.
    */
    wxRichTextContextMenuPropertiesInfo();

// Operations

    /**
        Initialisation.
    */
    void Init();

    /**
        Adds an item.
    */
    bool AddItem(const wxString& label, wxRichTextObject* obj);

    /**
        Returns the number of menu items that were added.
    */
    int AddMenuItems(wxMenu* menu, int startCmd = wxID_RICHTEXT_PROPERTIES1) const;

    /**
        Adds appropriate menu items for the current container and clicked on object
        (and container's parent, if appropriate).
    */
    int AddItems(wxRichTextCtrl* ctrl, wxRichTextObject* container, wxRichTextObject* obj);

    /**
        Clears the items.
    */
    void Clear();

// Accessors

    /**
        Returns the nth label.
    */
    wxString GetLabel(int n) const;

    /**
        Returns the nth object.
    */
    wxRichTextObject* GetObject(int n) const;

    /**
        Returns the array of objects.
    */
    wxRichTextObjectPtrArray& GetObjects();

    /**
        Returns the array of objects.
    */
    const wxRichTextObjectPtrArray& GetObjects() const;

    /**
        Returns the array of labels.
    */
    wxArrayString& GetLabels();

    /**
        Returns the array of labels.
    */
    const wxArrayString& GetLabels() const;

    /**
        Returns the number of items.
    */
    int GetCount() const;

    wxRichTextObjectPtrArray    m_objects;
    wxArrayString               m_labels;
};

/**
    @class wxRichTextCtrl

    wxRichTextCtrl provides a generic, ground-up implementation of a text control
    capable of showing multiple styles and images.

    wxRichTextCtrl sends notification events: see wxRichTextEvent.

    It also sends the standard wxTextCtrl events @c wxEVT_TEXT_ENTER and
    @c wxEVT_TEXT, and wxTextUrlEvent when URL content is clicked.

    For more information, see the @ref overview_richtextctrl.

    @beginStyleTable
    @style{wxRE_CENTRE_CARET}
           The control will try to keep the caret line centred vertically while editing.
           wxRE_CENTER_CARET is a synonym for this style.
    @style{wxRE_MULTILINE}
           The control will be multiline (mandatory).
    @style{wxRE_READONLY}
           The control will not be editable.
    @endStyleTable

    @library{wxrichtext}
    @category{richtext}
    @appearance{richtextctrl}

 */

class wxRichTextCtrl : public wxControl,
                       public wxTextCtrlIface,
                       public wxScrollHelper
{
public:
// Constructors

    /**
        Default constructor.
    */
    wxRichTextCtrl( );

    /**
        Constructor, creating and showing a rich text control.

        @param parent
            Parent window. Must not be @NULL.
        @param id
            Window identifier. The value @c wxID_ANY indicates a default value.
        @param value
            Default string.
        @param pos
            Window position.
        @param size
            Window size.
        @param style
            Window style.
        @param validator
            Window validator.
        @param name
            Window name.

        @see Create(), wxValidator
    */
    wxRichTextCtrl( wxWindow* parent, wxWindowID id = -1, const wxString& value = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
        long style = wxRE_MULTILINE, const wxValidator& validator = wxDefaultValidator, const wxString& name = wxTextCtrlNameStr);

    /**
        Destructor.
    */
    virtual ~wxRichTextCtrl( );

// Operations

    /**
        Creates the underlying window.
    */
    bool Create( wxWindow* parent, wxWindowID id = -1, const wxString& value = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
        long style = wxRE_MULTILINE, const wxValidator& validator = wxDefaultValidator, const wxString& name = wxTextCtrlNameStr );

    /**
        Initialises the members of the control.
    */
    void Init();

// Accessors

    /**
        Gets the text for the given range.
        The end point of range is specified as the last character position of
        the span of text, plus one.
    */
    virtual wxString GetRange(long from, long to) const;

    /**
        Returns the length of the specified line in characters.
    */
    virtual int GetLineLength(long lineNo) const ;

    /**
        Returns the text for the given line.
    */
    virtual wxString GetLineText(long lineNo) const ;

    /**
        Returns the number of lines in the buffer.
    */
    virtual int GetNumberOfLines() const ;

    /**
        Returns @true if the buffer has been modified.
    */
    virtual bool IsModified() const ;

    /**
        Returns @true if the control is editable.
    */
    virtual bool IsEditable() const ;

    /**
        Returns @true if the control is single-line.
        Currently wxRichTextCtrl does not support single-line editing.
    */
    bool IsSingleLine() const;

    /**
        Returns @true if the control is multiline.
    */
    bool IsMultiLine() const;

    //@{
    /**
        Returns the range of the current selection.
        The end point of range is specified as the last character position of the span
        of text, plus one.
        If the return values @a from and @a to are the same, there is no selection.
    */
    virtual void GetSelection(long* from, long* to) const;
    const wxRichTextSelection& GetSelection() const;
    wxRichTextSelection& GetSelection();
    //@}

    /**
        Returns the text within the current selection range, if any.
    */
    virtual wxString GetStringSelection() const;

    /**
        Gets the current filename associated with the control.
    */
    wxString GetFilename() const;

    /**
        Sets the current filename.
    */
    void SetFilename(const wxString& filename);

    /**
        Sets the size of the buffer beyond which layout is delayed during resizing.
        This optimizes sizing for large buffers. The default is 20000.
    */
    void SetDelayedLayoutThreshold(long threshold);

    /**
        Gets the size of the buffer beyond which layout is delayed during resizing.
        This optimizes sizing for large buffers. The default is 20000.
    */
    long GetDelayedLayoutThreshold() const;

    /**
    */
    bool GetFullLayoutRequired() const;

    /**
    */
    void SetFullLayoutRequired(bool b);

    /**
    */
    wxLongLong GetFullLayoutTime() const;

    /**
    */
    void SetFullLayoutTime(wxLongLong t);

    /**
    */
    long GetFullLayoutSavedPosition() const;

    /**
    */
    void SetFullLayoutSavedPosition(long p);

    // Force any pending layout due to large buffer
    /**
    */
    void ForceDelayedLayout();

    /**
        Sets the text (normal) cursor.
    */
    void SetTextCursor(const wxCursor& cursor );

    /**
        Returns the text (normal) cursor.
    */
    wxCursor GetTextCursor() const;

    /**
        Sets the cursor to be used over URLs.
    */
    void SetURLCursor(const wxCursor& cursor );

    /**
        Returns the cursor to be used over URLs.
    */
    wxCursor GetURLCursor() const;

    /**
        Returns @true if we are showing the caret position at the start of a line
        instead of at the end of the previous one.
    */
    bool GetCaretAtLineStart() const;

    /**
        Sets a flag to remember that we are showing the caret position at the start of a line
        instead of at the end of the previous one.
    */
    void SetCaretAtLineStart(bool atStart);

    /**
        Returns @true if we are extending a selection.
    */
    bool GetDragging() const;

    /**
        Sets a flag to remember if we are extending a selection.
    */
    void SetDragging(bool dragging);

    /**
        Are we trying to start Drag'n'Drop?
    */
    bool GetPreDrag() const;

    /**
        Set if we're trying to start Drag'n'Drop
    */
    void SetPreDrag(bool pd);

    /**
        Get the possible Drag'n'Drop start point
    */
    const wxPoint GetDragStartPoint() const;

    /**
        Set the possible Drag'n'Drop start point
    */
    void SetDragStartPoint(wxPoint sp);

    /**
        Get the possible Drag'n'Drop start time
    */
    const wxDateTime GetDragStartTime() const;

    /**
        Set the possible Drag'n'Drop start time
    */
    void SetDragStartTime(wxDateTime st);

#if wxRICHTEXT_BUFFERED_PAINTING
    //@{
    /**
        Returns the buffer bitmap if using buffered painting.
    */
    const wxBitmap& GetBufferBitmap() const;
    wxBitmap& GetBufferBitmap();
    //@}
#endif

    /**
        Returns the current context menu.
    */
    wxMenu* GetContextMenu() const;

    /**
        Sets the current context menu.
    */
    void SetContextMenu(wxMenu* menu);

    /**
        Returns an anchor so we know how to extend the selection.
        It's a caret position since it's between two characters.
    */
    long GetSelectionAnchor() const;

    /**
        Sets an anchor so we know how to extend the selection.
        It's a caret position since it's between two characters.
    */
    void SetSelectionAnchor(long anchor);

    /**
        Returns the anchor object if selecting multiple containers.
    */
    wxRichTextObject* GetSelectionAnchorObject() const;

    /**
        Sets the anchor object if selecting multiple containers.
    */
    void SetSelectionAnchorObject(wxRichTextObject* anchor);

    //@{
    /**
        Returns an object that stores information about context menu property item(s),
        in order to communicate between the context menu event handler and the code
        that responds to it. The wxRichTextContextMenuPropertiesInfo stores one
        item for each object that could respond to a property-editing event. If
        objects are nested, several might be editable.
    */
    wxRichTextContextMenuPropertiesInfo& GetContextMenuPropertiesInfo();
    const wxRichTextContextMenuPropertiesInfo& GetContextMenuPropertiesInfo() const;
    //@}

    /**
        Returns the wxRichTextObject object that currently has the editing focus.
        If there are no composite objects, this will be the top-level buffer.
    */
    wxRichTextParagraphLayoutBox* GetFocusObject() const;

    /**
        Setter for m_focusObject.
    */
    void StoreFocusObject(wxRichTextParagraphLayoutBox* obj);

    /**
        Sets the wxRichTextObject object that currently has the editing focus.
        @param setCaretPosition
            Optionally set the caret position.
    */
    bool SetFocusObject(wxRichTextParagraphLayoutBox* obj, bool setCaretPosition = true);

// Operations

    /**
        Invalidates the whole buffer to trigger painting later.
    */
    void Invalidate();

    /**
        Clears the buffer content, leaving a single empty paragraph. Cannot be undone.
    */
    virtual void Clear();

    /**
        Replaces the content in the specified range with the string specified by
        @a value.
    */
    virtual void Replace(long from, long to, const wxString& value);

    /**
        Removes the content in the specified range.
    */
    virtual void Remove(long from, long to);

    /**
        Loads content into the control's buffer using the given type.

        If the specified type is wxRICHTEXT_TYPE_ANY, the type is deduced from
        the filename extension.

        This function looks for a suitable wxRichTextFileHandler object.
    */
    bool LoadFile(const wxString& file,
                  int type = wxRICHTEXT_TYPE_ANY);

    /**
        Helper function for LoadFile(). Loads content into the control's buffer using the given type.

        If the specified type is wxRICHTEXT_TYPE_ANY, the type is deduced from
        the filename extension.

        This function looks for a suitable wxRichTextFileHandler object.
    */
    virtual bool DoLoadFile(const wxString& file, int fileType);

    /**
        Saves the buffer content using the given type.

        If the specified type is wxRICHTEXT_TYPE_ANY, the type is deduced from
        the filename extension.

        This function looks for a suitable wxRichTextFileHandler object.
    */
    bool SaveFile(const wxString& file = wxEmptyString,
                  int type = wxRICHTEXT_TYPE_ANY);

    /**
        Helper function for SaveFile(). Saves the buffer content using the given type.

        If the specified type is wxRICHTEXT_TYPE_ANY, the type is deduced from
        the filename extension.

        This function looks for a suitable wxRichTextFileHandler object.
    */
    virtual bool DoSaveFile(const wxString& file = wxEmptyString,
                            int fileType = wxRICHTEXT_TYPE_ANY);

    /**
        Sets flags that change the behaviour of loading or saving.

        See the documentation for each handler class to see what flags are
        relevant for each handler.
    */
    void SetHandlerFlags(int flags);

    /**
        Returns flags that change the behaviour of loading or saving.
        See the documentation for each handler class to see what flags are
        relevant for each handler.
    */
    int GetHandlerFlags() const;

    /**
        Marks the buffer as modified.
    */
    virtual void MarkDirty();

    /**
        Sets the buffer's modified status to @false, and clears the buffer's command
        history.
    */
    virtual void DiscardEdits();

    
    void SetModified(bool modified);

    /**
        Sets the maximum number of characters that may be entered in a single line
        text control. For compatibility only; currently does nothing.
    */
    virtual void SetMaxLength(unsigned long len);

    /**
        Writes text at the current position.
    */
    virtual void WriteText(const wxString& text);

    /**
        Sets the insertion point to the end of the buffer and writes the text.
    */
    virtual void AppendText(const wxString& text);

    //@{
    /**
        Gets the attributes at the given position.
        This function gets the combined style - that is, the style you see on the
        screen as a result of combining base style, paragraph style and character
        style attributes.

        To get the character or paragraph style alone, use GetUncombinedStyle().

        @beginWxPerlOnly
        In wxPerl this method is implemented as GetStyle(@a position)
        returning a 2-element list (ok, attr).
        @endWxPerlOnly
    */
    virtual bool GetStyle(long position, wxTextAttr& style);
    virtual bool GetStyle(long position, wxRichTextAttr& style);
    virtual bool GetStyle(long position, wxRichTextAttr& style, wxRichTextParagraphLayoutBox* container);
    //@}

    //@{
    /**
        Sets the attributes for the given range.
        The end point of range is specified as the last character position of the span
        of text, plus one.

        So, for example, to set the style for a character at position 5, use the range
        (5,6).
    */
    virtual bool SetStyle(long start, long end, const wxTextAttr& style);
    virtual bool SetStyle(long start, long end, const wxRichTextAttr& style);
    virtual bool SetStyle(const wxRichTextRange& range, const wxTextAttr& style);
    virtual bool SetStyle(const wxRichTextRange& range, const wxRichTextAttr& style);
    //@}

    /**
        Sets the attributes for a single object
    */
    virtual void SetStyle(wxRichTextObject *obj, const wxRichTextAttr& textAttr, int flags = wxRICHTEXT_SETSTYLE_WITH_UNDO);

    //@{
    /**
        Gets the attributes common to the specified range.
        Attributes that differ in value within the range will not be included
        in @a style flags.

        @beginWxPerlOnly
        In wxPerl this method is implemented as GetStyleForRange(@a position)
        returning a 2-element list (ok, attr).
        @endWxPerlOnly
    */
    virtual bool GetStyleForRange(const wxRichTextRange& range, wxTextAttr& style);
    virtual bool GetStyleForRange(const wxRichTextRange& range, wxRichTextAttr& style);
    virtual bool GetStyleForRange(const wxRichTextRange& range, wxRichTextAttr& style, wxRichTextParagraphLayoutBox* container);
    //@}

    /**
        Sets the attributes for the given range, passing flags to determine how the
        attributes are set.

        The end point of range is specified as the last character position of the span
        of text, plus one. So, for example, to set the style for a character at
        position 5, use the range (5,6).

        @a flags may contain a bit list of the following values:
        - wxRICHTEXT_SETSTYLE_NONE: no style flag.
        - wxRICHTEXT_SETSTYLE_WITH_UNDO: specifies that this operation should be
          undoable.
        - wxRICHTEXT_SETSTYLE_OPTIMIZE: specifies that the style should not be applied
          if the combined style at this point is already the style in question.
        - wxRICHTEXT_SETSTYLE_PARAGRAPHS_ONLY: specifies that the style should only be
          applied to paragraphs, and not the content.
          This allows content styling to be preserved independently from that
          of e.g. a named paragraph style.
        - wxRICHTEXT_SETSTYLE_CHARACTERS_ONLY: specifies that the style should only be
          applied to characters, and not the paragraph.
          This allows content styling to be preserved independently from that
          of e.g. a named paragraph style.
        - wxRICHTEXT_SETSTYLE_RESET: resets (clears) the existing style before applying
          the new style.
        - wxRICHTEXT_SETSTYLE_REMOVE: removes the specified style. Only the style flags
          are used in this operation.
    */
    virtual bool SetStyleEx(const wxRichTextRange& range, const wxRichTextAttr& style, int flags = wxRICHTEXT_SETSTYLE_WITH_UNDO);

    //@{
    /**
        Gets the attributes at the given position.
        This function gets the @e uncombined style - that is, the attributes associated
        with the paragraph or character content, and not necessarily the combined
        attributes you see on the screen.
        To get the combined attributes, use GetStyle().

        If you specify (any) paragraph attribute in @e style's flags, this function
        will fetch the paragraph attributes.
        Otherwise, it will return the character attributes.

        @beginWxPerlOnly
        In wxPerl this method is implemented as GetUncombinedStyle(@a position)
        returning a 2-element list (ok, attr).
        @endWxPerlOnly
    */
    virtual bool GetUncombinedStyle(long position, wxRichTextAttr& style);
    virtual bool GetUncombinedStyle(long position, wxRichTextAttr& style, wxRichTextParagraphLayoutBox* container);
    //@}

    //@{
    /**
        Sets the current default style, which can be used to change how subsequently
        inserted text is displayed.
    */
    virtual bool SetDefaultStyle(const wxTextAttr& style);
    virtual bool SetDefaultStyle(const wxRichTextAttr& style);
    //@}

    /**
        Returns the current default style, which can be used to change how subsequently
        inserted text is displayed.
    */
    virtual const wxRichTextAttr& GetDefaultStyleEx() const;

    //virtual const wxTextAttr& GetDefaultStyle() const;

    //@{
    /**
        Sets the list attributes for the given range, passing flags to determine how
        the attributes are set.

        Either the style definition or the name of the style definition (in the current
        sheet) can be passed.
        @a flags is a bit list of the following:
        - wxRICHTEXT_SETSTYLE_WITH_UNDO: specifies that this command will be undoable.
        - wxRICHTEXT_SETSTYLE_RENUMBER: specifies that numbering should start from
          @a startFrom, otherwise existing attributes are used.
        - wxRICHTEXT_SETSTYLE_SPECIFY_LEVEL: specifies that @a listLevel should be used
          as the level for all paragraphs, otherwise the current indentation will be used.

        @see NumberList(), PromoteList(), ClearListStyle().
    */
    virtual bool SetListStyle(const wxRichTextRange& range, wxRichTextListStyleDefinition* def, int flags = wxRICHTEXT_SETSTYLE_WITH_UNDO, int startFrom = 1, int specifiedLevel = -1);
    virtual bool SetListStyle(const wxRichTextRange& range, const wxString& defName, int flags = wxRICHTEXT_SETSTYLE_WITH_UNDO, int startFrom = 1, int specifiedLevel = -1);
    //@}

    /**
        Clears the list style from the given range, clearing list-related attributes
        and applying any named paragraph style associated with each paragraph.

        @a flags is a bit list of the following:
        - wxRICHTEXT_SETSTYLE_WITH_UNDO: specifies that this command will be undoable.

        @see SetListStyle(), PromoteList(), NumberList().
    */
    virtual bool ClearListStyle(const wxRichTextRange& range, int flags = wxRICHTEXT_SETSTYLE_WITH_UNDO);

    //@{
    /**
        Numbers the paragraphs in the given range.
        Pass flags to determine how the attributes are set.

        Either the style definition or the name of the style definition (in the current
        sheet) can be passed.

        @a flags is a bit list of the following:
        - wxRICHTEXT_SETSTYLE_WITH_UNDO: specifies that this command will be undoable.
        - wxRICHTEXT_SETSTYLE_RENUMBER: specifies that numbering should start from
          @a startFrom, otherwise existing attributes are used.
        - wxRICHTEXT_SETSTYLE_SPECIFY_LEVEL: specifies that @a listLevel should be used
          as the level for all paragraphs, otherwise the current indentation will be used.

        @see SetListStyle(), PromoteList(), ClearListStyle().
    */
    virtual bool NumberList(const wxRichTextRange& range, wxRichTextListStyleDefinition* def = NULL, int flags = wxRICHTEXT_SETSTYLE_WITH_UNDO, int startFrom = 1, int specifiedLevel = -1);
    virtual bool NumberList(const wxRichTextRange& range, const wxString& defName, int flags = wxRICHTEXT_SETSTYLE_WITH_UNDO, int startFrom = 1, int specifiedLevel = -1);
    //@}

    //@{
    /**
        Promotes or demotes the paragraphs in the given range.
        A positive @a promoteBy produces a smaller indent, and a negative number
        produces a larger indent. Pass flags to determine how the attributes are set.
        Either the style definition or the name of the style definition (in the current
        sheet) can be passed.

        @a flags is a bit list of the following:
        - wxRICHTEXT_SETSTYLE_WITH_UNDO: specifies that this command will be undoable.
        - wxRICHTEXT_SETSTYLE_RENUMBER: specifies that numbering should start from
          @a startFrom, otherwise existing attributes are used.
        - wxRICHTEXT_SETSTYLE_SPECIFY_LEVEL: specifies that @a listLevel should be used
        as the level for all paragraphs, otherwise the current indentation will be used.

        @see SetListStyle(), @see SetListStyle(), ClearListStyle().
    */
    virtual bool PromoteList(int promoteBy, const wxRichTextRange& range, wxRichTextListStyleDefinition* def = NULL, int flags = wxRICHTEXT_SETSTYLE_WITH_UNDO, int specifiedLevel = -1);
    virtual bool PromoteList(int promoteBy, const wxRichTextRange& range, const wxString& defName, int flags = wxRICHTEXT_SETSTYLE_WITH_UNDO, int specifiedLevel = -1);
    //@}

    /**
        Sets the properties for the given range, passing flags to determine how the
        attributes are set. You can merge properties or replace them.

        The end point of range is specified as the last character position of the span
        of text, plus one. So, for example, to set the properties for a character at
        position 5, use the range (5,6).

        @a flags may contain a bit list of the following values:
        - wxRICHTEXT_SETSPROPERTIES_NONE: no flag.
        - wxRICHTEXT_SETPROPERTIES_WITH_UNDO: specifies that this operation should be
          undoable.
        - wxRICHTEXT_SETPROPERTIES_PARAGRAPHS_ONLY: specifies that the properties should only be
          applied to paragraphs, and not the content.
        - wxRICHTEXT_SETPROPERTIES_CHARACTERS_ONLY: specifies that the properties should only be
          applied to characters, and not the paragraph.
        - wxRICHTEXT_SETPROPERTIES_RESET: resets (clears) the existing properties before applying
          the new properties.
        - wxRICHTEXT_SETPROPERTIES_REMOVE: removes the specified properties.
    */
    virtual bool SetProperties(const wxRichTextRange& range, const wxRichTextProperties& properties, int flags = wxRICHTEXT_SETPROPERTIES_WITH_UNDO);

    /**
        Deletes the content within the given range.
    */
    virtual bool Delete(const wxRichTextRange& range);

    /**
        Translates from column and line number to position.
    */
    virtual long XYToPosition(long x, long y) const;

    /**
        Converts a text position to zero-based column and line numbers.
    */
    virtual bool PositionToXY(long pos, long *x, long *y) const;

    /**
        Scrolls the buffer so that the given position is in view.
    */
    virtual void ShowPosition(long pos);

    //@{
    /**
        Finds the character at the given position in pixels.
        @a pt is in device coords (not adjusted for the client area origin nor for
        scrolling).
    */
    virtual wxTextCtrlHitTestResult HitTest(const wxPoint& pt, long *pos) const;
    virtual wxTextCtrlHitTestResult HitTest(const wxPoint& pt,
                                            wxTextCoord *col,
                                            wxTextCoord *row) const;

    /**
        Finds the container at the given point, which is assumed to be in client coordinates.
    */
    wxRichTextParagraphLayoutBox* FindContainerAtPoint(const wxPoint pt, long& position, int& hit, wxRichTextObject* hitObj, int flags = 0);
    //@}

// Clipboard operations

    /**
        Copies the selected content (if any) to the clipboard.
    */
    virtual void Copy();

    /**
        Copies the selected content (if any) to the clipboard and deletes the selection.
        This is undoable.
    */
    virtual void Cut();

    /**
        Pastes content from the clipboard to the buffer.
    */
    virtual void Paste();

    /**
        Deletes the content in the selection, if any. This is undoable.
    */
    virtual void DeleteSelection();

    /**
        Returns @true if selected content can be copied to the clipboard.
    */
    virtual bool CanCopy() const;

    /**
        Returns @true if selected content can be copied to the clipboard and deleted.
    */
    virtual bool CanCut() const;

    /**
        Returns @true if the clipboard content can be pasted to the buffer.
    */
    virtual bool CanPaste() const;

    /**
        Returns @true if selected content can be deleted.
    */
    virtual bool CanDeleteSelection() const;

    /**
        Undoes the command at the top of the command history, if there is one.
    */
    virtual void Undo();

    /**
        Redoes the current command.
    */
    virtual void Redo();

    /**
        Returns @true if there is a command in the command history that can be undone.
    */
    virtual bool CanUndo() const;

    /**
        Returns @true if there is a command in the command history that can be redone.
    */
    virtual bool CanRedo() const;

    /**
        Sets the insertion point and causes the current editing style to be taken from
        the new position (unlike wxRichTextCtrl::SetCaretPosition).
    */
    virtual void SetInsertionPoint(long pos);

    /**
        Sets the insertion point to the end of the text control.
    */
    virtual void SetInsertionPointEnd();

    /**
        Returns the current insertion point.
    */
    virtual long GetInsertionPoint() const;

    /**
        Returns the last position in the buffer.
    */
    virtual wxTextPos GetLastPosition() const;

    //@{
    /**
        Sets the selection to the given range.
        The end point of range is specified as the last character position of the span
        of text, plus one.

        So, for example, to set the selection for a character at position 5, use the
        range (5,6).
    */
    virtual void SetSelection(long from, long to);
    void SetSelection(const wxRichTextSelection& sel);
    //@}


    /**
        Selects all the text in the buffer.
    */
    virtual void SelectAll();

    /**
        Makes the control editable, or not.
    */
    virtual void SetEditable(bool editable);

    /**
        Returns @true if there is a selection and the object containing the selection
        was the same as the current focus object.
    */
    virtual bool HasSelection() const;

    /**
        Returns @true if there was a selection, whether or not the current focus object
        is the same as the selection's container object.
    */
    virtual bool HasUnfocusedSelection() const;

    //@{
    /**
        Write a bitmap or image at the current insertion point.
        Supply an optional type to use for internal and file storage of the raw data.
    */
    virtual bool WriteImage(const wxImage& image, wxBitmapType bitmapType = wxBITMAP_TYPE_PNG,
                            const wxRichTextAttr& textAttr = wxRichTextAttr());

    virtual bool WriteImage(const wxBitmap& bitmap, wxBitmapType bitmapType = wxBITMAP_TYPE_PNG,
                            const wxRichTextAttr& textAttr = wxRichTextAttr());
    //@}

    /**
        Loads an image from a file and writes it at the current insertion point.
    */
    virtual bool WriteImage(const wxString& filename, wxBitmapType bitmapType,
                            const wxRichTextAttr& textAttr = wxRichTextAttr());

    /**
        Writes an image block at the current insertion point.
    */
    virtual bool WriteImage(const wxRichTextImageBlock& imageBlock,
                            const wxRichTextAttr& textAttr = wxRichTextAttr());

    /**
        Write a text box at the current insertion point, returning the text box.
        You can then call SetFocusObject() to set the focus to the new object.
    */
    virtual wxRichTextBox* WriteTextBox(const wxRichTextAttr& textAttr = wxRichTextAttr());

    /**
        Writes a field at the current insertion point.

        @param fieldType
            The field type, matching an existing field type definition.
        @param properties
            Extra data for the field.
        @param textAttr
            Optional attributes.

        @see wxRichTextField, wxRichTextFieldType, wxRichTextFieldTypeStandard
    */
    virtual wxRichTextField* WriteField(const wxString& fieldType, const wxRichTextProperties& properties,
                            const wxRichTextAttr& textAttr = wxRichTextAttr());

    /**
        Write a table at the current insertion point, returning the table.
        You can then call SetFocusObject() to set the focus to the new object.
    */
    virtual wxRichTextTable* WriteTable(int rows, int cols, const wxRichTextAttr& tableAttr = wxRichTextAttr(), const wxRichTextAttr& cellAttr = wxRichTextAttr());

    /**
        Inserts a new paragraph at the current insertion point. @see LineBreak().
    */
    virtual bool Newline();

    /**
        Inserts a line break at the current insertion point.

        A line break forces wrapping within a paragraph, and can be introduced by
        using this function, by appending the wxChar value @b  wxRichTextLineBreakChar
        to text content, or by typing Shift-Return.
    */
    virtual bool LineBreak();

    /**
        Sets the basic (overall) style.

        This is the style of the whole buffer before further styles are applied,
        unlike the default style, which only affects the style currently being
        applied (for example, setting the default style to bold will cause
        subsequently inserted text to be bold).
    */
    virtual void SetBasicStyle(const wxRichTextAttr& style);

    /**
        Gets the basic (overall) style.

        This is the style of the whole buffer before further styles are applied,
        unlike the default style, which only affects the style currently being
        applied (for example, setting the default style to bold will cause
        subsequently inserted text to be bold).
    */
    virtual const wxRichTextAttr& GetBasicStyle() const;

    /**
        Begins applying a style.
    */
    virtual bool BeginStyle(const wxRichTextAttr& style);

    /**
        Ends the current style.
    */
    virtual bool EndStyle();

    /**
        Ends application of all styles in the current style stack.
    */
    virtual bool EndAllStyles();

    /**
        Begins using bold.
    */
    bool BeginBold();

    /**
        Ends using bold.
    */
    bool EndBold();

    /**
        Begins using italic.
    */
    bool BeginItalic();

    /**
        Ends using italic.
    */
    bool EndItalic();

    /**
        Begins using underlining.
    */
    bool BeginUnderline();

    /**
        End applying underlining.
    */
    bool EndUnderline();

    /**
        Begins using the given point size.
    */
    bool BeginFontSize(int pointSize);

    /**
        Ends using a point size.
    */
    bool EndFontSize();

    /**
        Begins using this font.
    */
    bool BeginFont(const wxFont& font);

    /**
        Ends using a font.
    */
    bool EndFont();

    /**
        Begins using this colour.
    */
    bool BeginTextColour(const wxColour& colour);

    /**
        Ends applying a text colour.
    */
    bool EndTextColour();

    /**
        Begins using alignment.
        For alignment values, see wxTextAttr.
    */
    bool BeginAlignment(wxTextAttrAlignment alignment);

    /**
        Ends alignment.
    */
    bool EndAlignment();

    /**
        Begins applying a left indent and subindent in tenths of a millimetre.
        The subindent is an offset from the left edge of the paragraph, and is
        used for all but the first line in a paragraph. A positive value will
        cause the first line to appear to the left of the subsequent lines, and
        a negative value will cause the first line to be indented to the right
        of the subsequent lines.

        wxRichTextBuffer uses indentation to render a bulleted item. The
        content of the paragraph, including the first line, starts at the
        @a leftIndent plus the @a leftSubIndent.

        @param leftIndent
            The distance between the margin and the bullet.
        @param leftSubIndent
             The distance between the left edge of the bullet and the left edge
             of the actual paragraph.
    */
    bool BeginLeftIndent(int leftIndent, int leftSubIndent = 0);

    /**
        Ends left indent.
    */
    bool EndLeftIndent();

    /**
        Begins a right indent, specified in tenths of a millimetre.
    */
    bool BeginRightIndent(int rightIndent);

    /**
        Ends right indent.
    */
    bool EndRightIndent();

    /**
        Begins paragraph spacing; pass the before-paragraph and after-paragraph spacing
        in tenths of a millimetre.
    */
    bool BeginParagraphSpacing(int before, int after);

    /**
        Ends paragraph spacing.
    */
    bool EndParagraphSpacing();

    /**
        Begins appling line spacing. @e spacing is a multiple, where 10 means
        single-spacing, 15 means 1.5 spacing, and 20 means double spacing.

        The ::wxTextAttrLineSpacing constants are defined for convenience.
    */
    bool BeginLineSpacing(int lineSpacing);

    /**
        Ends line spacing.
    */
    bool EndLineSpacing();

    /**
        Begins a numbered bullet.

        This call will be needed for each item in the list, and the
        application should take care of incrementing the numbering.

        @a bulletNumber is a number, usually starting with 1.
        @a leftIndent and @a leftSubIndent are values in tenths of a millimetre.
        @a bulletStyle is a bitlist of the  ::wxTextAttrBulletStyle values.

        wxRichTextBuffer uses indentation to render a bulleted item.
        The left indent is the distance between the margin and the bullet.
        The content of the paragraph, including the first line, starts
        at leftMargin + leftSubIndent.
        So the distance between the left edge of the bullet and the
        left of the actual paragraph is leftSubIndent.
    */
    bool BeginNumberedBullet(int bulletNumber, int leftIndent, int leftSubIndent, int bulletStyle = wxTEXT_ATTR_BULLET_STYLE_ARABIC|wxTEXT_ATTR_BULLET_STYLE_PERIOD);
    
    /**
        Ends application of a numbered bullet.
    */
    bool EndNumberedBullet();

    /**
        Begins applying a symbol bullet, using a character from the current font.
        See BeginNumberedBullet() for an explanation of how indentation is used
        to render the bulleted paragraph.
    */
    bool BeginSymbolBullet(const wxString& symbol, int leftIndent, int leftSubIndent, int bulletStyle = wxTEXT_ATTR_BULLET_STYLE_SYMBOL);
    
    /**
        Ends applying a symbol bullet.
    */
    bool EndSymbolBullet();

    /**
        Begins applying a symbol bullet.
    */
    bool BeginStandardBullet(const wxString& bulletName, int leftIndent, int leftSubIndent, int bulletStyle = wxTEXT_ATTR_BULLET_STYLE_STANDARD);

    /**
        Begins applying a standard bullet.
    */
    bool EndStandardBullet();

    /**
        Begins using the named character style.
    */
    bool BeginCharacterStyle(const wxString& characterStyle);

    /**
        Ends application of a named character style.
    */
    bool EndCharacterStyle();

    /**
        Begins applying the named paragraph style.
    */
    bool BeginParagraphStyle(const wxString& paragraphStyle);

    /**
        Ends application of a named paragraph style.
    */
    bool EndParagraphStyle();

    /**
        Begins using a specified list style.
        Optionally, you can also pass a level and a number.
    */
    bool BeginListStyle(const wxString& listStyle, int level = 1, int number = 1);

    /**
        Ends using a specified list style.
    */
    bool EndListStyle();

    /**
        Begins applying wxTEXT_ATTR_URL to the content.

        Pass a URL and optionally, a character style to apply, since it is common
        to mark a URL with a familiar style such as blue text with underlining.
    */
    bool BeginURL(const wxString& url, const wxString& characterStyle = wxEmptyString);

    /**
        Ends applying a URL.
    */
    bool EndURL();

    /**
        Sets the default style to the style under the cursor.
    */
    bool SetDefaultStyleToCursorStyle();

    /**
        Cancels any selection.
    */
    virtual void SelectNone();

    /**
        Selects the word at the given character position.
    */
    virtual bool SelectWord(long position);

    /**
        Returns the selection range in character positions. -1, -1 means no selection.

        The range is in API convention, i.e. a single character selection is denoted
        by (n, n+1)
    */
    wxRichTextRange GetSelectionRange() const;

    /**
        Sets the selection to the given range.
        The end point of range is specified as the last character position of the span
        of text, plus one.

        So, for example, to set the selection for a character at position 5, use the
        range (5,6).
    */
    void SetSelectionRange(const wxRichTextRange& range);

    /**
        Returns the selection range in character positions. -2, -2 means no selection
        -1, -1 means select everything.
        The range is in internal format, i.e. a single character selection is denoted
        by (n, n)
    */
    wxRichTextRange GetInternalSelectionRange() const;

    /**
        Sets the selection range in character positions. -2, -2 means no selection
        -1, -1 means select everything.
        The range is in internal format, i.e. a single character selection is denoted
        by (n, n)
    */
    void SetInternalSelectionRange(const wxRichTextRange& range);

    /**
        Adds a new paragraph of text to the end of the buffer.
    */
    virtual wxRichTextRange AddParagraph(const wxString& text);

    /**
        Adds an image to the control's buffer.
    */
    virtual wxRichTextRange AddImage(const wxImage& image);

    /**
        Lays out the buffer, which must be done before certain operations, such as
        setting the caret position.
        This function should not normally be required by the application.
    */
    virtual bool LayoutContent(bool onlyVisibleRect = false);

    /**
        Move the caret to the given character position.

        Please note that this does not update the current editing style
        from the new position; to do that, call wxRichTextCtrl::SetInsertionPoint instead.
    */
    virtual bool MoveCaret(long pos, bool showAtLineStart = false, wxRichTextParagraphLayoutBox* container = NULL);

    /**
        Moves right.
    */
    virtual bool MoveRight(int noPositions = 1, int flags = 0);

    /**
        Moves left.
    */
    virtual bool MoveLeft(int noPositions = 1, int flags = 0);

    /**
        Moves to the start of the paragraph.
    */
    virtual bool MoveUp(int noLines = 1, int flags = 0);

    /**
        Moves the caret down.
    */
    virtual bool MoveDown(int noLines = 1, int flags = 0);

    /**
        Moves to the end of the line.
    */
    virtual bool MoveToLineEnd(int flags = 0);

    /**
        Moves to the start of the line.
    */
    virtual bool MoveToLineStart(int flags = 0);

    /**
        Moves to the end of the paragraph.
    */
    virtual bool MoveToParagraphEnd(int flags = 0);

    /**
        Moves to the start of the paragraph.
    */
    virtual bool MoveToParagraphStart(int flags = 0);

    /**
        Moves to the start of the buffer.
    */
    virtual bool MoveHome(int flags = 0);

    /**
        Moves to the end of the buffer.
    */
    virtual bool MoveEnd(int flags = 0);

    /**
        Moves one or more pages up.
    */
    virtual bool PageUp(int noPages = 1, int flags = 0);

    /**
        Moves one or more pages down.
    */
    virtual bool PageDown(int noPages = 1, int flags = 0);

    /**
        Moves a number of words to the left.
    */
    virtual bool WordLeft(int noPages = 1, int flags = 0);

    /**
        Move a nuber of words to the right.
    */
    virtual bool WordRight(int noPages = 1, int flags = 0);

    //@{
    /**
        Returns the buffer associated with the control.
    */
    wxRichTextBuffer& GetBuffer();
    const wxRichTextBuffer& GetBuffer() const;
    //@}

    /**
        Starts batching undo history for commands.
    */
    virtual bool BeginBatchUndo(const wxString& cmdName);

    /**
        Ends batching undo command history.
    */
    virtual bool EndBatchUndo();

    /**
        Returns @true if undo commands are being batched.
    */
    virtual bool BatchingUndo() const;

    /**
        Starts suppressing undo history for commands.
    */
    virtual bool BeginSuppressUndo();

    /**
        Ends suppressing undo command history.
    */
    virtual bool EndSuppressUndo();

    /**
        Returns @true if undo history suppression is on.
    */
    virtual bool SuppressingUndo() const;

    /**
        Test if this whole range has character attributes of the specified kind.
        If any of the attributes are different within the range, the test fails.

        You can use this to implement, for example, bold button updating.
        @a style must have flags indicating which attributes are of interest.
    */
    virtual bool HasCharacterAttributes(const wxRichTextRange& range, const wxRichTextAttr& style) const;
    

    /**
        Test if this whole range has paragraph attributes of the specified kind.
        If any of the attributes are different within the range, the test fails.
        You can use this to implement, for example, centering button updating.
        @a style must have flags indicating which attributes are of interest.
    */
    virtual bool HasParagraphAttributes(const wxRichTextRange& range, const wxRichTextAttr& style) const;
    

    /**
        Returns @true if all of the selection, or the content at the caret position, is bold.
    */
    virtual bool IsSelectionBold();

    /**
        Returns @true if all of the selection, or the content at the caret position, is italic.
    */
    virtual bool IsSelectionItalics();

    /**
        Returns @true if all of the selection, or the content at the caret position, is underlined.
    */
    virtual bool IsSelectionUnderlined();

    /**
        Returns @true if all of the selection, or the content at the current caret position, has the supplied wxTextAttrEffects flag(s).
    */
    virtual bool DoesSelectionHaveTextEffectFlag(int flag);

    /**
        Returns @true if all of the selection is aligned according to the specified flag.
    */
    virtual bool IsSelectionAligned(wxTextAttrAlignment alignment);

    /**
        Apples bold to the selection or the default style (undoable).
    */
    virtual bool ApplyBoldToSelection();

    /**
        Applies italic to the selection or the default style (undoable).
    */
    virtual bool ApplyItalicToSelection();

    /**
        Applies underline to the selection or the default style (undoable).
    */
    virtual bool ApplyUnderlineToSelection();

    /**
        Applies one or more wxTextAttrEffects flags to the selection (undoable).
        If there is no selection, it is applied to the default style.
    */
    virtual bool ApplyTextEffectToSelection(int flags);

    /**
        Applies the given alignment to the selection or the default style (undoable).
        For alignment values, see wxTextAttr.
    */
    virtual bool ApplyAlignmentToSelection(wxTextAttrAlignment alignment);

    /**
        Applies the style sheet to the buffer, matching paragraph styles in the sheet
        against named styles in the buffer.

        This might be useful if the styles have changed.
        If @a sheet is @NULL, the sheet set with SetStyleSheet() is used.
        Currently this applies paragraph styles only.
    */
    virtual bool ApplyStyle(wxRichTextStyleDefinition* def);

    /**
        Sets the style sheet associated with the control.
        A style sheet allows named character and paragraph styles to be applied.
    */
    void SetStyleSheet(wxRichTextStyleSheet* styleSheet);

    /**
        Returns the style sheet associated with the control, if any.
        A style sheet allows named character and paragraph styles to be applied.
    */
    wxRichTextStyleSheet* GetStyleSheet() const;

    /**
        Push the style sheet to top of stack.
    */
    bool PushStyleSheet(wxRichTextStyleSheet* styleSheet);

    /**
        Pops the style sheet from top of stack.
    */
    wxRichTextStyleSheet* PopStyleSheet();

    /**
        Applies the style sheet to the buffer, for example if the styles have changed.
    */
    bool ApplyStyleSheet(wxRichTextStyleSheet* styleSheet = NULL);

    /**
        Shows the given context menu, optionally adding appropriate property-editing commands for the current position in the object hierarchy.
    */
    virtual bool ShowContextMenu(wxMenu* menu, const wxPoint& pt, bool addPropertyCommands);

    /**
        Prepares the context menu, optionally adding appropriate property-editing commands.
        Returns the number of property commands added.
    */
    virtual int PrepareContextMenu(wxMenu* menu, const wxPoint& pt, bool addPropertyCommands);

    /**
        Returns @true if we can edit the object's properties via a GUI.
    */
    virtual bool CanEditProperties(wxRichTextObject* obj) const;

    /**
        Edits the object's properties via a GUI.
    */
    virtual bool EditProperties(wxRichTextObject* obj, wxWindow* parent);

    /**
        Gets the object's properties menu label.
    */
    virtual wxString GetPropertiesMenuLabel(wxRichTextObject* obj);

    /**
        Prepares the content just before insertion (or after buffer reset). Called by the same function in wxRichTextBuffer.
        Currently is only called if undo mode is on.
    */
    virtual void PrepareContent(wxRichTextParagraphLayoutBox& container);

    /**
        Can we delete this range?
        Sends an event to the control.
    */
    virtual bool CanDeleteRange(wxRichTextParagraphLayoutBox& container, const wxRichTextRange& range) const;

    /**
        Can we insert content at this position?
        Sends an event to the control.
    */
    virtual bool CanInsertContent(wxRichTextParagraphLayoutBox& container, long pos) const;

    /**
        Enable or disable the vertical scrollbar.
    */
    virtual void EnableVerticalScrollbar(bool enable);

    /**
        Returns @true if the vertical scrollbar is enabled.
    */
    virtual bool GetVerticalScrollbarEnabled() const;

    /**
        Sets the scale factor for displaying fonts, for example for more comfortable
        editing.
    */
    void SetFontScale(double fontScale, bool refresh = false);

    /**
        Returns the scale factor for displaying fonts, for example for more comfortable
        editing.
    */
    double GetFontScale() const { return GetBuffer().GetFontScale(); }

    /**
        Sets the scale factor for displaying certain dimensions such as indentation and
        inter-paragraph spacing. This can be useful when editing in a small control
        where you still want legible text, but a minimum of wasted white space.
    */
    void SetDimensionScale(double dimScale, bool refresh = false);

    /**
        Returns the scale factor for displaying certain dimensions such as indentation
        and inter-paragraph spacing.
    */
    double GetDimensionScale() const { return GetBuffer().GetDimensionScale(); }

    /**
        Sets an overall scale factor for displaying and editing the content.
    */
    void SetScale(double scale, bool refresh = false);

    /**
        Returns an overall scale factor for displaying and editing the content.
    */
    double GetScale() const;

    /**
        Returns an unscaled point.
    */
    wxPoint GetUnscaledPoint(const wxPoint& pt) const;

    /**
        Returns a scaled point.
    */
    wxPoint GetScaledPoint(const wxPoint& pt) const;

    /**
        Returns an unscaled size.
    */
    wxSize GetUnscaledSize(const wxSize& sz) const;

    /**
        Returns a scaled size.
    */
    wxSize GetScaledSize(const wxSize& sz) const;

    /**
        Returns an unscaled rectangle.
    */
    wxRect GetUnscaledRect(const wxRect& rect) const;

    /**
        Returns a scaled rectangle.
    */
    wxRect GetScaledRect(const wxRect& rect) const;

    /**
        Returns @true if this control can use virtual attributes and virtual text.
        The default is @false.
    */
    bool GetVirtualAttributesEnabled() const;

    /**
        Pass @true to let the control use virtual attributes.
        The default is @false.
    */
    void EnableVirtualAttributes(bool b);

// Command handlers

    /**
        Sends the event to the control.
    */
    void Command(wxCommandEvent& event);

    /**
        Loads the first dropped file.
    */
    void OnDropFiles(wxDropFilesEvent& event);

    void OnCaptureLost(wxMouseCaptureLostEvent& event);
    void OnSysColourChanged(wxSysColourChangedEvent& event);

    /**
        Standard handler for the wxID_CUT command.
    */
    void OnCut(wxCommandEvent& event);

    /**
        Standard handler for the wxID_COPY command.
    */
    void OnCopy(wxCommandEvent& event);

    /**
        Standard handler for the wxID_PASTE command.
    */
    void OnPaste(wxCommandEvent& event);

    /**
        Standard handler for the wxID_UNDO command.
    */
    void OnUndo(wxCommandEvent& event);

    /**
        Standard handler for the wxID_REDO command.
    */
    void OnRedo(wxCommandEvent& event);

    /**
        Standard handler for the wxID_SELECTALL command.
    */
    void OnSelectAll(wxCommandEvent& event);

    /**
        Standard handler for property commands.
    */
    void OnProperties(wxCommandEvent& event);

    /**
        Standard handler for the wxID_CLEAR command.
    */
    void OnClear(wxCommandEvent& event);

    /**
        Standard update handler for the wxID_CUT command.
    */
    void OnUpdateCut(wxUpdateUIEvent& event);

    /**
        Standard update handler for the wxID_COPY command.
    */
    void OnUpdateCopy(wxUpdateUIEvent& event);

    /**
        Standard update handler for the wxID_PASTE command.
    */
    void OnUpdatePaste(wxUpdateUIEvent& event);

    /**
        Standard update handler for the wxID_UNDO command.
    */
    void OnUpdateUndo(wxUpdateUIEvent& event);

    /**
        Standard update handler for the wxID_REDO command.
    */
    void OnUpdateRedo(wxUpdateUIEvent& event);

    /**
        Standard update handler for the wxID_SELECTALL command.
    */
    void OnUpdateSelectAll(wxUpdateUIEvent& event);

    /**
        Standard update handler for property commands.
    */

    void OnUpdateProperties(wxUpdateUIEvent& event);

    /**
        Standard update handler for the wxID_CLEAR command.
    */
    void OnUpdateClear(wxUpdateUIEvent& event);

    /**
        Shows a standard context menu with undo, redo, cut, copy, paste, clear, and
        select all commands.
    */
    void OnContextMenu(wxContextMenuEvent& event);

// Event handlers

    // Painting
    void OnPaint(wxPaintEvent& event);
    void OnEraseBackground(wxEraseEvent& event);

    // Left-click
    void OnLeftClick(wxMouseEvent& event);

    // Left-up
    void OnLeftUp(wxMouseEvent& event);

    // Motion
    void OnMoveMouse(wxMouseEvent& event);

    // Left-double-click
    void OnLeftDClick(wxMouseEvent& event);

    // Middle-click
    void OnMiddleClick(wxMouseEvent& event);

    // Right-click
    void OnRightClick(wxMouseEvent& event);

    // Key press
    void OnChar(wxKeyEvent& event);

    // Sizing
    void OnSize(wxSizeEvent& event);

    // Setting/losing focus
    void OnSetFocus(wxFocusEvent& event);
    void OnKillFocus(wxFocusEvent& event);

    // Idle-time processing
    void OnIdle(wxIdleEvent& event);

    // Scrolling
    void OnScroll(wxScrollWinEvent& event);

    /**
        Sets the font, and also the basic and default attributes
        (see wxRichTextCtrl::SetDefaultStyle).
    */
    virtual bool SetFont(const wxFont& font);

    /**
        A helper function setting up scrollbars, for example after a resize.
    */
    virtual void SetupScrollbars(bool atTop = false);

    /**
        Helper function implementing keyboard navigation.
    */
    virtual bool KeyboardNavigate(int keyCode, int flags);

    /**
        Paints the background.
    */
    virtual void PaintBackground(wxDC& dc);

    /**
        Other user defined painting after everything else (i.e. all text) is painted.

        @since 2.9.1
    */
    virtual void PaintAboveContent(wxDC& WXUNUSED(dc)) {}

#if wxRICHTEXT_BUFFERED_PAINTING
    /**
        Recreates the buffer bitmap if necessary.
    */
    virtual bool RecreateBuffer(const wxSize& size = wxDefaultSize);
#endif

    // Write text
    virtual void DoWriteText(const wxString& value, int flags = 0);

    // Should we inherit colours?
    virtual bool ShouldInheritColours() const;

    /**
        Internal function to position the visible caret according to the current caret
        position.
    */
    virtual void PositionCaret(wxRichTextParagraphLayoutBox* container = NULL);

    /**
        Helper function for extending the selection, returning @true if the selection
        was changed. Selections are in caret positions.
    */
    virtual bool ExtendSelection(long oldPosition, long newPosition, int flags);

    /**
        Scrolls @a position into view. This function takes a caret position.
    */
    virtual bool ScrollIntoView(long position, int keyCode);

    /**
        Refreshes the area affected by a selection change.
    */
    bool RefreshForSelectionChange(const wxRichTextSelection& oldSelection, const wxRichTextSelection& newSelection);

    /**
        Sets the caret position.

        The caret position is the character position just before the caret.
        A value of -1 means the caret is at the start of the buffer.
        Please note that this does not update the current editing style
        from the new position or cause the actual caret to be refreshed; to do that,
        call wxRichTextCtrl::SetInsertionPoint instead.
    */
    void SetCaretPosition(long position, bool showAtLineStart = false) ;

    /**
        Returns the current caret position.
    */
    long GetCaretPosition() const;

    /**
        The adjusted caret position is the character position adjusted to take
        into account whether we're at the start of a paragraph, in which case
        style information should be taken from the next position, not current one.
    */
    long GetAdjustedCaretPosition(long caretPos) const;

    /**
        Move the caret one visual step forward: this may mean setting a flag
        and keeping the same position if we're going from the end of one line
        to the start of the next, which may be the exact same caret position.
    */
    void MoveCaretForward(long oldPosition) ;

    /**
        Move the caret one visual step forward: this may mean setting a flag
        and keeping the same position if we're going from the end of one line
        to the start of the next, which may be the exact same caret position.
    */
    void MoveCaretBack(long oldPosition) ;

    /**
        Returns the caret height and position for the given character position.
        If container is null, the current focus object will be used.

        @beginWxPerlOnly
        In wxPerl this method is implemented as
        GetCaretPositionForIndex(@a position) returning a
        2-element list (ok, rect).
        @endWxPerlOnly
    */
    bool GetCaretPositionForIndex(long position, wxRect& rect, wxRichTextParagraphLayoutBox* container = NULL);

    /**
        Internal helper function returning the line for the visible caret position.
        If the caret is shown at the very end of the line, it means the next character
        is actually on the following line.
        So this function gets the line we're expecting to find if this is the case.
    */
    wxRichTextLine* GetVisibleLineForCaretPosition(long caretPosition) const;

    /**
        Gets the command processor associated with the control's buffer.
    */
    wxCommandProcessor* GetCommandProcessor() const;

    /**
        Deletes content if there is a selection, e.g. when pressing a key.
        Returns the new caret position in @e newPos, or leaves it if there
        was no action. This is undoable.

        @beginWxPerlOnly
        In wxPerl this method takes no arguments and returns a 2-element
        list (ok, newPos).
        @endWxPerlOnly
    */
    bool DeleteSelectedContent(long* newPos= NULL);

    /**
        Transforms logical (unscrolled) position to physical window position.
    */
    wxPoint GetPhysicalPoint(const wxPoint& ptLogical) const;

    /**
        Transforms physical window position to logical (unscrolled) position.
    */
    wxPoint GetLogicalPoint(const wxPoint& ptPhysical) const;

    /**
        Helper function for finding the caret position for the next word.
        Direction is 1 (forward) or -1 (backwards).
    */
    virtual long FindNextWordPosition(int direction = 1) const;

    /**
        Returns @true if the given position is visible on the screen.
    */
    bool IsPositionVisible(long pos) const;

    /**
        Returns the first visible position in the current view.
    */
    long GetFirstVisiblePosition() const;

    /**
        Returns the caret position since the default formatting was changed. As
        soon as this position changes, we no longer reflect the default style
        in the UI. A value of -2 means that we should only reflect the style of the
        content under the caret.
    */
    long GetCaretPositionForDefaultStyle() const;

    /**
        Set the caret position for the default style that the user is selecting.
    */
    void SetCaretPositionForDefaultStyle(long pos);

    /**
        Returns @true if the user has recently set the default style without moving
        the caret, and therefore the UI needs to reflect the default style and not
        the style at the caret.

        Below is an example of code that uses this function to determine whether the UI
        should show that the current style is bold.

        @see SetAndShowDefaultStyle().
    */
    bool IsDefaultStyleShowing() const;

    /**
        Sets @a attr as the default style and tells the control that the UI should
        reflect this attribute until the user moves the caret.

        @see IsDefaultStyleShowing().
    */
    void SetAndShowDefaultStyle(const wxRichTextAttr& attr);

    /**
        Returns the first visible point in the window.
    */
    wxPoint GetFirstVisiblePoint() const;

    /**
        Returns the content of the entire control as a string.
    */
    virtual wxString GetValue() const;

    /**
        Replaces existing content with the given text.
    */
    virtual void SetValue(const wxString& value);


// Implementation

    /**
        Processes the back key.
    */
    virtual bool ProcessBackKey(wxKeyEvent& event, int flags);

    /**
        Given a character position at which there is a list style, find the range
        encompassing the same list style by looking backwards and forwards.
    */
    virtual wxRichTextRange FindRangeForList(long pos, bool& isNumberedList);

    /**
        Sets up the caret for the given position and container, after a mouse click.
    */
    bool SetCaretPositionAfterClick(wxRichTextParagraphLayoutBox* container, long position, int hitTestFlags, bool extendSelection = false);

    /**
        Find the caret position for the combination of hit-test flags and character position.
        Returns the caret position and also an indication of where to place the caret (caretLineStart)
        since this is ambiguous (same position used for end of line and start of next).
    */
    long FindCaretPositionForCharacterPosition(long position, int hitTestFlags, wxRichTextParagraphLayoutBox* container,
                                               bool& caretLineStart);

    /**
        Processes mouse movement in order to change the cursor
    */
    virtual bool ProcessMouseMovement(wxRichTextParagraphLayoutBox* container, wxRichTextObject* obj, long position, const wxPoint& pos);

    /**
        Font names take a long time to retrieve, so cache them (on demand).
    */
    static const wxArrayString& GetAvailableFontNames();

    /**
        Clears the cache of available font names.
    */
    static void ClearAvailableFontNames();

    // implement wxTextEntry methods
    virtual wxString DoGetValue() const;

protected:
    // implement the wxTextEntry pure virtual method
    virtual wxWindow *GetEditableWindow();

    // margins functions
    virtual bool DoSetMargins(const wxPoint& pt);
    virtual wxPoint DoGetMargins() const;


// Overrides
protected:

    /**
        Currently this simply returns @c wxSize(10, 10).
    */
    virtual wxSize DoGetBestSize() const ;

    virtual void DoSetValue(const wxString& value, int flags = 0);

    virtual void DoThaw();


// Data members
protected:
#if wxRICHTEXT_BUFFERED_PAINTING
    /// Buffer bitmap
    wxBitmap                m_bufferBitmap;
#endif

    /// Text buffer
    wxRichTextBuffer        m_buffer;

    wxMenu*                 m_contextMenu;

    /// Caret position (1 less than the character position, so -1 is the
    /// first caret position).
    long                    m_caretPosition;

    /// Caret position when the default formatting has been changed. As
    /// soon as this position changes, we no longer reflect the default style
    /// in the UI.
    long                    m_caretPositionForDefaultStyle;

    /// Selection range in character positions. -2, -2 means no selection.
    wxRichTextSelection     m_selection;

    wxRichTextCtrlSelectionState m_selectionState;

    /// Anchor so we know how to extend the selection
    /// It's a caret position since it's between two characters.
    long                    m_selectionAnchor;

    /// Anchor object if selecting multiple container objects, such as grid cells.
    wxRichTextObject*       m_selectionAnchorObject;

    /// Are we editable?
    bool                    m_editable;

    /// Are we showing the caret position at the start of a line
    /// instead of at the end of the previous one?
    bool                    m_caretAtLineStart;

    /// Are we dragging a selection?
    bool                    m_dragging;

    /// Do we need full layout in idle?
    bool                    m_fullLayoutRequired;
    wxLongLong              m_fullLayoutTime;
    long                    m_fullLayoutSavedPosition;

    /// Threshold for doing delayed layout
    long                    m_delayedLayoutThreshold;

    /// Cursors
    wxCursor                m_textCursor;
    wxCursor                m_urlCursor;

    static wxArrayString    sm_availableFontNames;

    wxRichTextContextMenuPropertiesInfo m_contextMenuPropertiesInfo;

    /// The object that currently has the editing focus
    wxRichTextParagraphLayoutBox* m_focusObject;
};

/**
    @class wxRichTextEvent

    This is the event class for wxRichTextCtrl notifications.

    @beginEventTable{wxRichTextEvent}
    @event{EVT_RICHTEXT_LEFT_CLICK(id, func)}
        Process a @c wxEVT_RICHTEXT_LEFT_CLICK event, generated when the user
        releases the left mouse button over an object.
    @event{EVT_RICHTEXT_RIGHT_CLICK(id, func)}
        Process a @c wxEVT_RICHTEXT_RIGHT_CLICK event, generated when the user
        releases the right mouse button over an object.
    @event{EVT_RICHTEXT_MIDDLE_CLICK(id, func)}
        Process a @c wxEVT_RICHTEXT_MIDDLE_CLICK event, generated when the user
        releases the middle mouse button over an object.
    @event{EVT_RICHTEXT_LEFT_DCLICK(id, func)}
        Process a @c wxEVT_RICHTEXT_LEFT_DCLICK event, generated when the user
        double-clicks an object.
    @event{EVT_RICHTEXT_RETURN(id, func)}
        Process a @c wxEVT_RICHTEXT_RETURN event, generated when the user
        presses the return key. Valid event functions: GetFlags, GetPosition.
    @event{EVT_RICHTEXT_CHARACTER(id, func)}
        Process a @c wxEVT_RICHTEXT_CHARACTER event, generated when the user
        presses a character key. Valid event functions: GetFlags, GetPosition, GetCharacter.
    @event{EVT_RICHTEXT_DELETE(id, func)}
        Process a @c wxEVT_RICHTEXT_DELETE event, generated when the user
        presses the backspace or delete key. Valid event functions: GetFlags, GetPosition.
    @event{EVT_RICHTEXT_RETURN(id, func)}
        Process a @c wxEVT_RICHTEXT_RETURN event, generated when the user
        presses the return key. Valid event functions: GetFlags, GetPosition.
    @event{EVT_RICHTEXT_STYLE_CHANGED(id, func)}
        Process a @c wxEVT_RICHTEXT_STYLE_CHANGED event, generated when
        styling has been applied to the control. Valid event functions: GetPosition, GetRange.
    @event{EVT_RICHTEXT_STYLESHEET_CHANGED(id, func)}
        Process a @c wxEVT_RICHTEXT_STYLESHEET_CHANGING event, generated
        when the control's stylesheet has changed, for example the user added,
        edited or deleted a style. Valid event functions: GetRange, GetPosition.
    @event{EVT_RICHTEXT_STYLESHEET_REPLACING(id, func)}
        Process a @c wxEVT_RICHTEXT_STYLESHEET_REPLACING event, generated
        when the control's stylesheet is about to be replaced, for example when
        a file is loaded into the control.
        Valid event functions: Veto, GetOldStyleSheet, GetNewStyleSheet.
    @event{EVT_RICHTEXT_STYLESHEET_REPLACED(id, func)}
        Process a @c wxEVT_RICHTEXT_STYLESHEET_REPLACED event, generated
        when the control's stylesheet has been replaced, for example when a file
        is loaded into the control.
        Valid event functions: GetOldStyleSheet, GetNewStyleSheet.
    @event{EVT_RICHTEXT_PROPERTIES_CHANGED(id, func)}
        Process a @c wxEVT_RICHTEXT_PROPERTIES_CHANGED event, generated when
        properties have been applied to the control. Valid event functions: GetPosition, GetRange.
    @event{EVT_RICHTEXT_CONTENT_INSERTED(id, func)}
        Process a @c wxEVT_RICHTEXT_CONTENT_INSERTED event, generated when
        content has been inserted into the control.
        Valid event functions: GetPosition, GetRange.
    @event{EVT_RICHTEXT_CONTENT_DELETED(id, func)}
        Process a @c wxEVT_RICHTEXT_CONTENT_DELETED event, generated when
        content has been deleted from the control.
        Valid event functions: GetPosition, GetRange.
    @event{EVT_RICHTEXT_BUFFER_RESET(id, func)}
        Process a @c wxEVT_RICHTEXT_BUFFER_RESET event, generated when the
        buffer has been reset by deleting all content.
        You can use this to set a default style for the first new paragraph.
    @event{EVT_RICHTEXT_SELECTION_CHANGED(id, func)}
        Process a @c wxEVT_RICHTEXT_SELECTION_CHANGED event, generated when the
        selection range has changed.
    @event{EVT_RICHTEXT_FOCUS_OBJECT_CHANGED(id, func)}
        Process a @c wxEVT_RICHTEXT_FOCUS_OBJECT_CHANGED event, generated when the
        current focus object has changed.
    @endEventTable

    @library{wxrichtext}
    @category{events,richtext}
*/

class wxRichTextEvent : public wxNotifyEvent
{
public:
    /**
        Constructor.

        @param commandType
            The type of the event.
        @param id
            Window identifier. The value @c wxID_ANY indicates a default value.
    */
    wxRichTextEvent(wxEventType commandType = wxEVT_NULL, int winid = 0);

    /**
        Copy constructor.
    */
    wxRichTextEvent(const wxRichTextEvent& event);
    
    /**
        Returns the buffer position at which the event occurred.
    */
    long GetPosition() const;

    /**
        Sets the buffer position variable.
    */
    void SetPosition(long pos);

    /**
        Returns flags indicating modifier keys pressed.

        Possible values are @c wxRICHTEXT_CTRL_DOWN, @c wxRICHTEXT_SHIFT_DOWN, and @c wxRICHTEXT_ALT_DOWN.
    */
    int GetFlags() const;

    /**
        Sets flags indicating modifier keys pressed.

        Possible values are @c wxRICHTEXT_CTRL_DOWN, @c wxRICHTEXT_SHIFT_DOWN, and @c wxRICHTEXT_ALT_DOWN.
    */
    void SetFlags(int flags);

    /**
        Returns the old style sheet.

        Can be used in a @c wxEVT_RICHTEXT_STYLESHEET_CHANGING or
        @c wxEVT_RICHTEXT_STYLESHEET_CHANGED event handler.
    */
    wxRichTextStyleSheet* GetOldStyleSheet() const;

    /**
        Sets the old style sheet variable.
    */
    void SetOldStyleSheet(wxRichTextStyleSheet* sheet);

    /**
        Returns the new style sheet.

        Can be used in a @c wxEVT_RICHTEXT_STYLESHEET_CHANGING or
        @c wxEVT_RICHTEXT_STYLESHEET_CHANGED event handler.
    */
    wxRichTextStyleSheet* GetNewStyleSheet() const;

    /**
        Sets the new style sheet variable.
    */
    void SetNewStyleSheet(wxRichTextStyleSheet* sheet);

    /**
        Gets the range for the current operation.
    */
    const wxRichTextRange& GetRange() const;

    /**
        Sets the range variable.
    */
    void SetRange(const wxRichTextRange& range);

    /**
        Returns the character pressed, within a @c wxEVT_RICHTEXT_CHARACTER event.
    */
    wxChar GetCharacter() const;

    /**
        Sets the character variable.
    */
    void SetCharacter(wxChar ch);

    /**
        Returns the container for which the event is relevant.
    */
    wxRichTextParagraphLayoutBox* GetContainer() const;

    /**
        Sets the container for which the event is relevant.
    */
    void SetContainer(wxRichTextParagraphLayoutBox* container);

    /**
        Returns the old container, for a focus change event.
    */
    wxRichTextParagraphLayoutBox* GetOldContainer() const;

    /**
        Sets the old container, for a focus change event.
    */
    void SetOldContainer(wxRichTextParagraphLayoutBox* container);

    virtual wxEvent *Clone() const;

protected:
    int                             m_flags;
    long                            m_position;
    wxRichTextStyleSheet*           m_oldStyleSheet;
    wxRichTextStyleSheet*           m_newStyleSheet;
    wxRichTextRange                 m_range;
    wxChar                          m_char;
    wxRichTextParagraphLayoutBox*   m_container;
    wxRichTextParagraphLayoutBox*   m_oldContainer;

};
