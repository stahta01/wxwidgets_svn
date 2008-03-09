/////////////////////////////////////////////////////////////////////////////
// Name:        richtext/richtextctrl.h
// Purpose:     documentation for wxRichTextEvent class
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxRichTextEvent
    @headerfile richtextctrl.h wx/richtext/richtextctrl.h

    This is the event class for wxRichTextCtrl notifications.

    @library{wxrichtext}
    @category{events}
*/
class wxRichTextEvent : public wxNotifyEvent
{
public:
    //@{
    /**
        Constructors.
    */
    wxRichTextEvent(const wxRichTextEvent& event);
    wxRichTextEvent(wxEventType commandType = wxEVT_NULL,
                    int winid = 0);
    //@}

    /**
        Clones the event.
    */
    wxEvent* Clone() const;

    /**
        Returns the character pressed, within a wxEVT_COMMAND_RICHTEXT_CHARACTER event.
    */
    wxChar GetCharacter() const;

    /**
        Returns flags indicating modifier keys pressed. Possible values are
        wxRICHTEXT_CTRL_DOWN,
        wxRICHTEXT_SHIFT_DOWN, and wxRICHTEXT_ALT_DOWN.
    */
    int GetFlags() const;

    /**
        Returns the new style sheet. Can be used in a
        wxEVT_COMMAND_RICHTEXT_STYLESHEET_CHANGING or
        wxEVT_COMMAND_RICHTEXT_STYLESHEET_CHANGED event handler.
    */
    wxRichTextStyleSheet* GetNewStyleSheet() const;

    /**
        Returns the old style sheet. Can be used in a
        wxEVT_COMMAND_RICHTEXT_STYLESHEET_CHANGING or
        wxEVT_COMMAND_RICHTEXT_STYLESHEET_CHANGED event handler.
    */
    wxRichTextStyleSheet* GetOldStyleSheet() const;

    /**
        Returns the buffer position at which the event occured.
    */
    long GetPosition() const;

    /**
        Gets the range for the current operation.
    */
    wxRichTextRange GetRange() const;

    /**
        Sets the character variable.
    */
    void SetCharacter(wxChar ch);

    /**
        Sets flags indicating modifier keys pressed. Possible values are
        wxRICHTEXT_CTRL_DOWN,
        wxRICHTEXT_SHIFT_DOWN, and wxRICHTEXT_ALT_DOWN.
    */
    void SetFlags(int flags);

    /**
        Sets the new style sheet variable.
    */
    void SetNewStyleSheet(wxRichTextStyleSheet* sheet);

    /**
        Sets the old style sheet variable.
    */
    void SetOldStyleSheet(wxRichTextStyleSheet* sheet);

    /**
        Sets the buffer position variable.
    */
    void SetPosition(long pos);

    /**
        Sets the range variable.
    */
    void SetRange(const wxRichTextRange& range);
};


/**
    @class wxRichTextCtrl
    @headerfile richtextctrl.h wx/richtext/richtextctrl.h

    wxRichTextCtrl provides a generic, ground-up implementation of a text control
    capable of showing multiple styles and images.

    wxRichTextCtrl sends notification events: see wxRichTextEvent.
    It also sends the standard wxTextCtrl events wxEVT_COMMAND_TEXT_ENTER and
    wxEVT_COMMAND_TEXT_UPDATED,
    and wxTextUrlEvent when URL content is clicked.

    For more information, see the @ref overview_wxrichtextctrloverview
    "wxRichTextCtrl overview".

    @library{wxrichtext}
    @category{ctrl}
    @appearance{richtextctrl.png}
*/
class wxRichTextCtrl
{
public:
    //@{
    /**
        Constructors.
    */
    wxRichTextCtrl();
    wxRichTextCtrl(wxWindow* parent, wxWindowID id = wxID_ANY,
                   const wxString& value = wxEmptyString,
                   const wxPoint& pos = wxDefaultPosition,
                   const wxSize& size = wxDefaultSize,
                   long style = wxRE_MULTILINE,
                   const wxValidator& validator = wxDefaultValidator,
                   const wxString& name = wxTextCtrlNameStr);
    //@}

    /**
        Destructor.
    */
    ~wxRichTextCtrl();

    /**
        Adds an image to the control's buffer.
    */
    wxRichTextRange AddImage(const wxImage& image);

    /**
        Adds a new paragraph of text to the end of the buffer.
    */
    wxRichTextRange AddParagraph(const wxString& text);

    /**
        Sets the insertion point to the end of the buffer and writes the text.
    */
    void AppendText(const wxString& text);

    /**
        Applies the given alignment to the selection (undoable).
        For alignment values, see wxTextAttr.
    */
    bool ApplyAlignmentToSelection(wxTextAttrAlignment alignment);

    /**
        Apples bold to the selection (undoable).
    */
    bool ApplyBoldToSelection();

    /**
        Applies italic to the selection (undoable).
    */
    bool ApplyItalicToSelection();

    /**
        Applies the given style to the selection.
    */
    bool ApplyStyle(wxRichTextStyleDefinition* def);

    /**
        Applies the style sheet to the buffer, matching paragraph styles in the sheet
        against named styles
        in the buffer. This might be useful if the styles have changed. If @a sheet is
        @NULL, the
        sheet set with SetStyleSheet is used.
        Currently this applies paragraph styles only.
    */
    bool ApplyStyleSheet(wxRichTextStyleSheet* sheet = NULL);

    /**
        Applies underline to the selection (undoable).
    */
    bool ApplyUnderlineToSelection();

    /**
        Returns @true if undo commands are being batched.
    */
    bool BatchingUndo() const;

    /**
        Begins using alignment
        For alignment values, see wxTextAttr.
    */
    bool BeginAlignment(wxTextAttrAlignment alignment);

    /**
        Starts batching undo history for commands.
    */
    bool BeginBatchUndo(const wxString& cmdName);

    /**
        Begins using bold.
    */
    bool BeginBold();

    /**
        Begins using the named character style.
    */
    bool BeginCharacterStyle(const wxString& characterStyle);

    /**
        Begins using this font.
    */
    bool BeginFont(const wxFont& font);

    /**
        Begins using the given point size.
    */
    bool BeginFontSize(int pointSize);

    /**
        Begins using italic.
    */
    bool BeginItalic();

    /**
        Begins applying a left indent and subindent in tenths of a millimetre.
        The sub-indent is an offset from the left of the paragraph, and is used for all
        but the
        first line in a paragraph. A positive value will cause the first line to appear
        to the left
        of the subsequent lines, and a negative value will cause the first line to be
        indented
        relative to the subsequent lines.
        wxRichTextBuffer uses indentation to render a bulleted item. The left indent is
        the distance between
        the margin and the bullet. The content of the paragraph, including the first
        line, starts
        at leftMargin + leftSubIndent. So the distance between the left edge of the
        bullet and the
        left of the actual paragraph is leftSubIndent.
    */
    bool BeginLeftIndent(int leftIndent, int leftSubIndent = 0);

    /**
        Begins appling line spacing. @e spacing is a multiple, where 10 means
        single-spacing,
        15 means 1.5 spacing, and 20 means double spacing. The following constants are
        defined for convenience:
    */
    bool BeginLineSpacing(int lineSpacing);

    /**
        Begins using a specified list style. Optionally, you can also pass a level and
        a number.
    */
    bool BeginListStyle(const wxString& listStyle, int level = 1,
                        int number = 1);

    /**
        Begins a numbered bullet. This call will be needed for each item in the list,
        and the
        application should take care of incrementing the numbering.
        @a bulletNumber is a number, usually starting with 1.
        @a leftIndent and @a leftSubIndent are values in tenths of a millimetre.
        @a bulletStyle is a bitlist of the following values:
        
        wxRichTextBuffer uses indentation to render a bulleted item. The left indent is
        the distance between
        the margin and the bullet. The content of the paragraph, including the first
        line, starts
        at leftMargin + leftSubIndent. So the distance between the left edge of the
        bullet and the
        left of the actual paragraph is leftSubIndent.
    */
    bool BeginNumberedBullet(int bulletNumber, int leftIndent,
                             int leftSubIndent,
                             int bulletStyle = wxTEXT_ATTR_BULLET_STYLE_ARABIC|wxTEXT_ATTR_BULLET_STYLE_PERIOD);

    /**
        Begins paragraph spacing; pass the before-paragraph and after-paragraph spacing
        in tenths of
        a millimetre.
    */
    bool BeginParagraphSpacing(int before, int after);

    /**
        Begins applying the named paragraph style.
    */
    bool BeginParagraphStyle(const wxString& paragraphStyle);

    /**
        Begins a right indent, specified in tenths of a millimetre.
    */
    bool BeginRightIndent(int rightIndent);

    /**
        Begins applying a style.
    */
    bool BeginStyle(const wxTextAttr& style);

    /**
        Starts suppressing undo history for commands.
    */
    bool BeginSuppressUndo();

    /**
        Begins applying a symbol bullet, using a character from the current font. See
        BeginNumberedBullet() for
        an explanation of how indentation is used to render the bulleted paragraph.
    */
    bool BeginSymbolBullet(wxChar symbol, int leftIndent,
                           int leftSubIndent,
                           int bulletStyle = wxTEXT_ATTR_BULLET_STYLE_SYMBOL);

    /**
        Begins using this colour.
    */
    bool BeginTextColour(const wxColour& colour);

    /**
        Begins applying wxTEXT_ATTR_URL to the content. Pass a URL and optionally, a
        character style to apply,
        since it is common to mark a URL with a familiar style such as blue text with
        underlining.
    */
    bool BeginURL(const wxString& url,
                  const wxString& characterStyle = wxEmptyString);

    /**
        Begins using underlining.
    */
    bool BeginUnderline();

    /**
        Returns @true if selected content can be copied to the clipboard.
    */
    bool CanCopy() const;

    /**
        Returns @true if selected content can be copied to the clipboard and deleted.
    */
    bool CanCut() const;

    /**
        Returns @true if selected content can be deleted.
    */
    bool CanDeleteSelection() const;

    /**
        Returns @true if the clipboard content can be pasted to the buffer.
    */
    bool CanPaste() const;

    /**
        Returns @true if there is a command in the command history that can be redone.
    */
    bool CanRedo() const;

    /**
        Returns @true if there is a command in the command history that can be undone.
    */
    bool CanUndo() const;

    /**
        Clears the buffer content, leaving a single empty paragraph. Cannot be undone.
    */
    void Clear();

    //@{
    /**
        Clears the list style from the given range, clearing list-related attributes
        and applying any named paragraph style associated with each paragraph.
        @a flags is a bit list of the following:
         wxRICHTEXT_SETSTYLE_WITH_UNDO: specifies that this command will be undoable.
        See also SetListStyle(), PromoteList(), NumberList().
    */
    bool ClearListStyle(const wxRichTextRange& range,
                        int flags = wxRICHTEXT_SETSTYLE_WITH_UNDO);
    bool ClearListStyle(const wxRichTextRange& range,
                        int flags = wxRICHTEXT_SETSTYLE_WITH_UNDO);
    //@}

    /**
        Sends the event to the control.
    */
    void Command(wxCommandEvent& event);

    /**
        Copies the selected content (if any) to the clipboard.
    */
    void Copy();

    /**
        Creates the underlying window.
    */
    bool Create(wxWindow* parent, wxWindowID id = wxID_ANY,
                const wxString& value = wxEmptyString,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxRE_MULTILINE,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = wxTextCtrlNameStr);

    /**
        Copies the selected content (if any) to the clipboard and deletes the selection.
        This is undoable.
    */
    void Cut();

    /**
        Deletes the content within the given range.
    */
    bool Delete(const wxRichTextRange& range);

    /**
        Deletes content if there is a selection, e.g. when pressing a key.
        Returns the new caret position in @e newPos, or leaves it if there
        was no action. This is undoable.
    */
    bool DeleteSelectedContent(long* newPos = NULL);

    /**
        Deletes the content in the selection, if any. This is undoable.
    */
    void DeleteSelection();

    /**
        Sets the buffer's modified status to @false, and clears the buffer's command
        history.
    */
    void DiscardEdits();

    /**
        Currently this simply returns @c wxSize(10, 10).
    */
    wxSize DoGetBestSize() const;

    /**
        Ends alignment.
    */
    bool EndAlignment();

    /**
        Ends application of all styles in the current style stack.
    */
    bool EndAllStyles();

    /**
        Ends batching undo command history.
    */
    bool EndBatchUndo();

    /**
        Ends using bold.
    */
    bool EndBold();

    /**
        Ends application of a named character style.
    */
    bool EndCharacterStyle();

    /**
        Ends using a font.
    */
    bool EndFont();

    /**
        Ends using a point size.
    */
    bool EndFontSize();

    /**
        Ends using italic.
    */
    bool EndItalic();

    /**
        Ends left indent.
    */
    bool EndLeftIndent();

    /**
        Ends line spacing.
    */
    bool EndLineSpacing();

    /**
        Ends using a specified list style.
    */
    bool EndListStyle();

    /**
        Ends application of a numbered bullet.
    */
    bool EndNumberedBullet();

    /**
        Ends paragraph spacing.
    */
    bool EndParagraphSpacing();

    /**
        Ends application of a named character style.
    */
    bool EndParagraphStyle();

    /**
        Ends right indent.
    */
    bool EndRightIndent();

    /**
        Ends the current style.
    */
    bool EndStyle();

    /**
        Ends suppressing undo command history.
    */
    bool EndSuppressUndo();

    /**
        Ends applying a symbol bullet.
    */
    bool EndSymbolBullet();

    /**
        Ends applying a text colour.
    */
    bool EndTextColour();

    /**
        Ends applying a URL.
    */
    bool EndURL();

    /**
        End applying underlining.
    */
    bool EndUnderline();

    /**
        Helper function for extending the selection, returning @true if the selection
        was
        changed. Selections are in caret positions.
    */
    bool ExtendSelection(long oldPosition, long newPosition,
                         int flags);

    /**
        Helper function for finding the caret position for the next word. Direction
        is 1 (forward) or -1 (backwards).
    */
    long FindNextWordPosition(int direction = 1) const;

    /**
        Call this function to prevent refresh and allow fast updates, and then Thaw() to
        refresh the control.
    */
    void Freeze();

    /**
        Gets the basic (overall) style. This is the style of the whole
        buffer before further styles are applied, unlike the default style, which
        only affects the style currently being applied (for example, setting the default
        style to bold will cause subsequently inserted text to be bold).
    */
    const wxTextAttr GetBasicStyle() const;

    //@{
    /**
        Returns the buffer associated with the control.
    */
    const wxRichTextBuffer GetBuffer();
    const wxRichTextBuffer&  GetBuffer();
    //@}

    /**
        Returns the current caret position.
    */
    long GetCaretPosition() const;

    /**
        Returns the caret height and position for the given character position
    */
    bool GetCaretPositionForIndex(long position, wxRect& rect);

    /**
        Gets the command processor associated with the control's buffer.
    */
    wxCommandProcessor* GetCommandProcessor() const;

    /**
        Returns the current default style, which can be used to change how subsequently
        inserted
        text is displayed.
    */
    const wxTextAttr GetDefaultStyle() const;

    /**
        Gets the size of the buffer beyond which layout is delayed during resizing.
        This optimizes sizing for large buffers. The default is 20000.
    */
    long GetDelayedLayoutThreshold() const;

    /**
        Gets the current filename associated with the control.
    */
    wxString GetFilename() const;

    /**
        Returns the first visible position in the current view.
    */
    long GetFirstVisiblePosition() const;

    /**
        Returns flags that change the behaviour of loading or saving. See the
        documentation for each
        handler class to see what flags are relevant for each handler.
    */
    int GetHandlerFlags() const;

    /**
        Returns the current insertion point.
    */
    long GetInsertionPoint() const;

    /**
        Returns the last position in the buffer.
    */
    wxTextPos GetLastPosition() const;

    /**
        Returns the length of the specified line in characters.
    */
    int GetLineLength(long lineNo) const;

    /**
        Returns the text for the given line.
    */
    wxString GetLineText(long lineNo) const;

    /**
        Transforms physical window position to logical (unscrolled) position.
    */
    wxPoint GetLogicalPoint(const wxPoint& ptPhysical) const;

    /**
        Returns the number of lines in the buffer.
    */
    int GetNumberOfLines() const;

    /**
        Transforms logical (unscrolled) position to physical window position.
    */
    wxPoint GetPhysicalPoint(const wxPoint& ptLogical) const;

    /**
        Gets the text for the given range.
        The end point of range is specified as the last character position of the span
        of text, plus one.
    */
    wxString GetRange(long from, long to) const;

    /**
        Returns the range of the current selection.
        The end point of range is specified as the last character position of the span
        of text, plus one.
        If the return values @a from and @a to are the same, there is no selection.
    */
    void GetSelection(long* from, long* to) const;

    /**
        Returns the selection range in character positions. -1, -1 means no selection.
    */
    const wxRichTextRange GetSelectionRange() const;

    /**
        Returns the text within the current selection range, if any.
    */
    wxString GetStringSelection() const;

    /**
        Gets the attributes at the given position.
        This function gets the combined style - that is, the style you see on the
        screen as a result
        of combining base style, paragraph style and character style attributes. To get
        the character
        or paragraph style alone, use GetUncombinedStyle().
    */
    bool GetStyle(long position, wxTextAttr& style);

    /**
        Gets the attributes common to the specified range. Attributes that differ in
        value within the range will
        not be included in @e style's flags.
    */
    bool GetStyleForRange(const wxRichTextRange& range,
                          wxTextAttr& style);

    /**
        Returns the style sheet associated with the control, if any. A style sheet
        allows named
        character and paragraph styles to be applied.
    */
    wxRichTextStyleSheet* GetStyleSheet() const;

    /**
        Gets the attributes at the given position.
        This function gets the @e uncombined style - that is, the attributes associated
        with the
        paragraph or character content, and not necessarily the combined attributes you
        see on the
        screen. To get the combined attributes, use GetStyle().
        If you specify (any) paragraph attribute in @e style's flags, this function
        will fetch
        the paragraph attributes. Otherwise, it will return the character attributes.
    */
    bool GetUncombinedStyle(long position, wxTextAttr& style);

    /**
        Returns the content of the entire control as a string.
    */
    wxString GetValue() const;

    /**
        Internal helper function returning the line for the visible caret position. If
        the caret is
        shown at the very end of the line, it means the next character is actually
        on the following line. So this function gets the line we're expecting to find
        if this is the case.
    */
    wxRichTextLine* GetVisibleLineForCaretPosition(long caretPosition) const;

    /**
        Test if this whole range has character attributes of the specified kind. If any
        of the attributes are different within the range, the test fails. You
        can use this to implement, for example, bold button updating. @a style must have
        flags indicating which attributes are of interest.
    */
    bool HasCharacterAttributes(const wxRichTextRange& range,
                                const wxTextAttr& style) const;

    /**
        Test if this whole range has paragraph attributes of the specified kind. If any
        of the attributes are different within the range, the test fails. You
        can use this to implement, for example, centering button updating. @a style
        must have
        flags indicating which attributes are of interest.
    */
    bool HasParagraphAttributes(const wxRichTextRange& range,
                                const wxTextAttr& style) const;

    /**
        Returns @true if there is a selection.
    */
    bool HasSelection() const;

    //@{
    /**
        Finds the character at the given position in pixels.
        @a pt is in device coords (not adjusted for the client area origin nor for
        scrolling).
    */
    wxTextCtrlHitTestResult HitTest(const wxPoint& pt, long* pos) const;
    const wxTextCtrlHitTestResult HitTest(const wxPoint& pt,
                                          wxTextCoord* col,
                                          wxTextCoord* row) const;
    //@}

    /**
        Initialises the members of the control.
    */
    void Init();

    /**
        Initialises the command event.
    */
    void InitCommandEvent(wxCommandEvent& event) const;

    /**
        Returns @true if the user has recently set the default style without moving
        the caret,
        and therefore the UI needs to reflect the default style and not the style at
        the caret.
        Below is an example of code that uses this function to determine whether the UI
        should show that the current style is bold.
        
        See also SetAndShowDefaultStyle().
    */
    bool IsDefaultStyleShowing() const;

    /**
        Returns @true if the control is editable.
    */
    bool IsEditable() const;

    /**
        Returns @true if Freeze has been called without a Thaw.
    */
    bool IsFrozen() const;

    /**
        Returns @true if the buffer has been modified.
    */
    bool IsModified() const;

    /**
        Returns @true if the control is multiline.
    */
    bool IsMultiLine() const;

    /**
        Returns @true if the given position is visible on the screen.
    */
    bool IsPositionVisible(long pos) const;

    /**
        Returns @true if all of the selection is aligned according to the specified
        flag.
    */
    bool IsSelectionAligned(wxTextAttrAlignment alignment) const;

    /**
        Returns @true if all of the selection is bold.
    */
    bool IsSelectionBold() const;

    /**
        Returns @true if all of the selection is italic.
    */
    bool IsSelectionItalics() const;

    /**
        Returns @true if all of the selection is underlined.
    */
    bool IsSelectionUnderlined() const;

    /**
        Returns @true if the control is single-line. Currently wxRichTextCtrl does not
        support single-line editing.
    */
    bool IsSingleLine() const;

    /**
        Helper function implementing keyboard navigation.
    */
    bool KeyboardNavigate(int keyCode, int flags);

    /**
        Lays out the buffer, which must be done before certain operations, such as
        setting the caret position. This function should not normally be required by the
        application.
    */
    bool LayoutContent(bool onlyVisibleRect = false);

    /**
        Inserts a line break at the current insertion point. A line break forces
        wrapping within a paragraph, and
        can be introduced by using this function, by appending the wxChar value @b
        wxRichTextLineBreakChar to text content,
        or by typing Shift-Return.
    */
    bool LineBreak();

    /**
        Loads content into the control's buffer using the given type. If the specified
        type
        is wxRICHTEXT_TYPE_ANY, the type is deduced from the filename extension.
        This function looks for a suitable wxRichTextFileHandler object.
    */
    bool LoadFile(const wxString& file,
                  int type = wxRICHTEXT_TYPE_ANY);

    /**
        Marks the buffer as modified.
    */
    void MarkDirty();

    /**
        Move the caret to the given character position.
    */
    bool MoveCaret(long pos, bool showAtLineStart = false);

    /**
        Move the caret one visual step forward: this may mean setting a flag
        and keeping the same position if we're going from the end of one line
        to the start of the next, which may be the exact same caret position.
    */
    void MoveCaretBack(long oldPosition);

    /**
        Move the caret one visual step forward: this may mean setting a flag
        and keeping the same position if we're going from the end of one line
        to the start of the next, which may be the exact same caret position.
    */
    void MoveCaretForward(long oldPosition);

    /**
        Moves the caret down.
    */
    bool MoveDown(int noLines = 1, int flags = 0);

    /**
        Moves to the end of the buffer.
    */
    bool MoveEnd(int flags = 0);

    /**
        Moves to the start of the buffer.
    */
    bool MoveHome(int flags = 0);

    /**
        Moves left.
    */
    bool MoveLeft(int noPositions = 1, int flags = 0);

    /**
        Moves right.
    */
    bool MoveRight(int noPositions = 1, int flags = 0);

    /**
        Moves to the end of the line.
    */
    bool MoveToLineEnd(int flags = 0);

    /**
        Moves to the start of the line.
    */
    bool MoveToLineStart(int flags = 0);

    /**
        Moves to the end of the paragraph.
    */
    bool MoveToParagraphEnd(int flags = 0);

    /**
        Moves to the start of the paragraph.
    */
    bool MoveToParagraphStart(int flags = 0);

    /**
        Moves up.
    */
    bool MoveUp(int noLines = 1, int flags = 0);

    /**
        Inserts a new paragraph at the current insertion point. See also LineBreak().
    */
    bool Newline();

    //@{
    /**
        Numbers the paragraphs in the given range. Pass flags to determine how the
        attributes are set.
        Either the style definition or the name of the style definition (in the current
        sheet) can be passed.
        @a flags is a bit list of the following:
         wxRICHTEXT_SETSTYLE_WITH_UNDO: specifies that this command will be undoable.
         wxRICHTEXT_SETSTYLE_RENUMBER: specifies that numbering should start from @e
        startFrom, otherwise existing attributes are used.
         wxRICHTEXT_SETSTYLE_SPECIFY_LEVEL: specifies that @a listLevel should be used
        as the level for all paragraphs, otherwise the current indentation will be used.
        See also SetListStyle(), PromoteList(), ClearListStyle().
    */
    bool NumberList(const wxRichTextRange& range,
                    const wxRichTextListStyleDefinition* style,
                    int flags = wxRICHTEXT_SETSTYLE_WITH_UNDO,
                    int startFrom = -1,
                    int listLevel = -1);
    bool Number(const wxRichTextRange& range,
                const wxString& styleName,
                int flags = wxRICHTEXT_SETSTYLE_WITH_UNDO,
                int startFrom = -1,
                int listLevel = -1);
    //@}

    /**
        Standard handler for the wxID_CLEAR command.
    */
    void OnClear(wxCommandEvent& event);

    /**
        Shows a standard context menu with undo, redo, cut, copy, paste, clear, and
        select all commands.
    */
    void OnContextMenu(wxContextMenuEvent& event);

    /**
        Standard handler for the wxID_COPY command.
    */
    void OnCopy(wxCommandEvent& event);

    /**
        Standard handler for the wxID_CUT command.
    */
    void OnCut(wxCommandEvent& event);

    /**
        Loads the first dropped file.
    */
    void OnDropFiles(wxDropFilesEvent& event);

    /**
        Standard handler for the wxID_PASTE command.
    */
    void OnPaste(wxCommandEvent& event);

    /**
        Standard handler for the wxID_REDO command.
    */
    void OnRedo(wxCommandEvent& event);

    /**
        Standard handler for the wxID_SELECTALL command.
    */
    void OnSelectAll(wxCommandEvent& event);

    /**
        Standard handler for the wxID_PASTE command.
    */
    void OnUndo(wxCommandEvent& event);

    /**
        Standard update handler for the wxID_CLEAR command.
    */
    void OnUpdateClear(wxUpdateUIEvent& event);

    /**
        Standard update handler for the wxID_COPY command.
    */
    void OnUpdateCopy(wxUpdateUIEvent& event);

    /**
        Standard update handler for the wxID_CUT command.
    */
    void OnUpdateCut(wxUpdateUIEvent& event);

    /**
        Standard update handler for the wxID_PASTE command.
    */
    void OnUpdatePaste(wxUpdateUIEvent& event);

    /**
        Standard update handler for the wxID_REDO command.
    */
    void OnUpdateRedo(wxUpdateUIEvent& event);

    /**
        Standard update handler for the wxID_SELECTALL command.
    */
    void OnUpdateSelectAll(wxUpdateUIEvent& event);

    /**
        Standard update handler for the wxID_UNDO command.
    */
    void OnUpdateUndo(wxUpdateUIEvent& event);

    /**
        Moves one or more pages down.
    */
    bool PageDown(int noPages = 1, int flags = 0);

    /**
        Moves one or more pages up.
    */
    bool PageUp(int noPages = 1, int flags = 0);

    /**
        Paints the background.
    */
    void PaintBackground(wxDC& dc);

    /**
        Pastes content from the clipboard to the buffer.
    */
    void Paste();

    /**
        Internal function to position the visible caret according to the current caret
        position.
    */
    void PositionCaret();

    /**
        Converts a text position to zero-based column and line numbers.
    */
    bool PositionToXY(long pos, long* x, long* y) const;

    //@{
    /**
        Promotes or demotes the paragraphs in the given range. A positive @a promoteBy
        produces a smaller indent, and a negative number
        produces a larger indent. Pass flags to determine how the attributes are set.
        Either the style definition or the name of the style definition (in the current
        sheet) can be passed.
        @a flags is a bit list of the following:
         wxRICHTEXT_SETSTYLE_WITH_UNDO: specifies that this command will be undoable.
         wxRICHTEXT_SETSTYLE_RENUMBER: specifies that numbering should start from @e
        startFrom, otherwise existing attributes are used.
         wxRICHTEXT_SETSTYLE_SPECIFY_LEVEL: specifies that @a listLevel should be used
        as the level for all paragraphs, otherwise the current indentation will be used.
        See also SetListStyle(), See also SetListStyle(), ClearListStyle().
    */
    bool PromoteList(int promoteBy, const wxRichTextRange& range,
                     const wxRichTextListStyleDefinition* style,
                     int flags = wxRICHTEXT_SETSTYLE_WITH_UNDO,
                     int listLevel = -1);
    bool PromoteList(int promoteBy, const wxRichTextRange& range,
                     const wxString& styleName,
                     int flags = wxRICHTEXT_SETSTYLE_WITH_UNDO,
                     int listLevel = -1);
    //@}

    /**
        Redoes the current command.
    */
    void Redo();

    /**
        Removes the content in the specified range.
    */
    void Remove(long from, long to);

    /**
        Replaces the content in the specified range with the string specified by @e
        value.
    */
    void Replace(long from, long to, const wxString& value);

    /**
        Saves the buffer content using the given type. If the specified type
        is wxRICHTEXT_TYPE_ANY, the type is deduced from the filename extension.
        This function looks for a suitable wxRichTextFileHandler object.
    */
    bool SaveFile(const wxString& file = wxEmptyString,
                  int type = wxRICHTEXT_TYPE_ANY);

    /**
        Scrolls @a position into view. This function takes a caret position.
    */
    bool ScrollIntoView(long position, int keyCode);

    /**
        Selects all the text in the buffer.
    */
    void SelectAll();

    /**
        Cancels any selection.
    */
    void SelectNone();

    /**
        Sets @a attr as the default style and tells the control that the UI should
        reflect
        this attribute until the user moves the caret.
        See also IsDefaultStyleShowing().
    */
    void SetAndShowDefaultStyle(const wxTextAttr& attr);

    /**
        Sets the basic (overall) style. This is the style of the whole
        buffer before further styles are applied, unlike the default style, which
        only affects the style currently being applied (for example, setting the default
        style to bold will cause subsequently inserted text to be bold).
    */
    void SetBasicStyle(const wxTextAttr& style);

    /**
        The caret position is the character position just before the caret.
        A value of -1 means the caret is at the start of the buffer.
    */
    void SetCaretPosition(long position,
                          bool showAtLineStart = false);

    /**
        Sets the current default style, which can be used to change how subsequently
        inserted
        text is displayed.
    */
    bool SetDefaultStyle(const wxTextAttr& style);

    /**
        Sets the default style to the style under the cursor.
    */
    bool SetDefaultStyleToCursorStyle();

    /**
        Sets the size of the buffer beyond which layout is delayed during resizing.
        This optimizes sizing for large buffers. The default is 20000.
    */
    void SetDelayedLayoutThreshold(long threshold);

    /**
        Makes the control editable, or not.
    */
    void SetEditable(bool editable);

    /**
        Sets the current filename.
    */
    void SetFilename(const wxString& filename);

    /**
        Sets the font, and also the basic and default attributes (see
        wxRichTextCtrl::SetDefaultStyle).
    */
    bool SetFont(const wxFont& font);

    /**
        Sets flags that change the behaviour of loading or saving. See the
        documentation for each
        handler class to see what flags are relevant for each handler.
    */
    void SetHandlerFlags(int flags);

    /**
        Sets the insertion point.
    */
    void SetInsertionPoint(long pos);

    /**
        Sets the insertion point to the end of the text control.
    */
    void SetInsertionPointEnd();

    //@{
    /**
        Sets the list attributes for the given range, passing flags to determine how
        the attributes are set.
        Either the style definition or the name of the style definition (in the current
        sheet) can be passed.
        @a flags is a bit list of the following:
         wxRICHTEXT_SETSTYLE_WITH_UNDO: specifies that this command will be undoable.
         wxRICHTEXT_SETSTYLE_RENUMBER: specifies that numbering should start from @e
        startFrom, otherwise existing attributes are used.
         wxRICHTEXT_SETSTYLE_SPECIFY_LEVEL: specifies that @a listLevel should be used
        as the level for all paragraphs, otherwise the current indentation will be used.
        See also NumberList(), PromoteList(), ClearListStyle().
    */
    bool SetListStyle(const wxRichTextRange& range,
                      const wxRichTextListStyleDefinition* style,
                      int flags = wxRICHTEXT_SETSTYLE_WITH_UNDO,
                      int startFrom = -1,
                      int listLevel = -1);
    bool SetListStyle(const wxRichTextRange& range,
                      const wxString& styleName,
                      int flags = wxRICHTEXT_SETSTYLE_WITH_UNDO,
                      int startFrom = -1,
                      int listLevel = -1);
    //@}

    /**
        Sets the selection to the given range.
        The end point of range is specified as the last character position of the span
        of text, plus one.
        So, for example, to set the selection for a character at position 5, use the
        range (5,6).
    */
    void SetSelection(long from, long to);

    /**
        Sets the selection to the given range.
        The end point of range is specified as the last character position of the span
        of text, plus one.
        So, for example, to set the selection for a character at position 5, use the
        range (5,6).
    */
    void SetSelectionRange(const wxRichTextRange& range);

    //@{
    /**
        Sets the attributes for the given range.
        The end point of range is specified as the last character position of the span
        of text, plus one.
        So, for example, to set the style for a character at position 5, use the range
        (5,6).
    */
    bool SetStyle(const wxRichTextRange& range,
                  const wxTextAttr& style);
    bool SetStyle(long start, long end, const wxTextAttr& style);
    //@}

    //@{
    /**
        Sets the attributes for the given range, passing flags to determine how the
        attributes are set.
        The end point of range is specified as the last character position of the span
        of text, plus one.
        So, for example, to set the style for a character at position 5, use the range
        (5,6).
        @a flags may contain a bit list of the following values:
         wxRICHTEXT_SETSTYLE_NONE: no style flag.
         wxRICHTEXT_SETSTYLE_WITH_UNDO: specifies that this operation should be
        undoable.
         wxRICHTEXT_SETSTYLE_OPTIMIZE: specifies that the style should not be applied
        if the
        combined style at this point is already the style in question.
         wxRICHTEXT_SETSTYLE_PARAGRAPHS_ONLY: specifies that the style should only be
        applied to paragraphs,
        and not the content. This allows content styling to be preserved independently
        from that of e.g. a named paragraph style.
         wxRICHTEXT_SETSTYLE_CHARACTERS_ONLY: specifies that the style should only be
        applied to characters,
        and not the paragraph. This allows content styling to be preserved
        independently from that of e.g. a named paragraph style.
         wxRICHTEXT_SETSTYLE_RESET: resets (clears) the existing style before applying
        the new style.
         wxRICHTEXT_SETSTYLE_REMOVE: removes the specified style. Only the style flags
        are used in this operation.
    */
    bool SetStyleEx(const wxRichTextRange& range,
                    const wxTextAttr& style,
                    int flags = wxRICHTEXT_SETSTYLE_WITH_UNDO);
    bool SetStyleEx(long start, long end,
                    const wxTextAttr& style,
                    int flags = wxRICHTEXT_SETSTYLE_WITH_UNDO);
    //@}

    /**
        Sets the style sheet associated with the control. A style sheet allows named
        character and paragraph styles to be applied.
    */
    void SetStyleSheet(wxRichTextStyleSheet* styleSheet);

    /**
        Replaces existing content with the given text.
    */
    void SetValue(const wxString& value);

    /**
        A helper function setting up scrollbars, for example after a resize.
    */
    void SetupScrollbars(bool atTop = false);

    /**
        Scrolls the buffer so that the given position is in view.
    */
    void ShowPosition(long pos);

    /**
        Returns @true if undo history suppression is on.
    */
    bool SuppressingUndo() const;

    /**
        Call this function to end a Freeze and refresh the display.
    */
    void Thaw();

    /**
        Undoes the command at the top of the command history, if there is one.
    */
    void Undo();

    /**
        Moves a number of words to the left.
    */
    bool WordLeft(int noWords = 1, int flags = 0);

    /**
        Move a nuber of words to the right.
    */
    bool WordRight(int noWords = 1, int flags = 0);

    //@{
    /**
        Write a bitmap or image at the current insertion point. Supply an optional type
        to use
        for internal and file storage of the raw data.
    */
    bool WriteImage(const wxString& filename, int bitmapType);
    bool WriteImage(const wxRichTextImageBlock& imageBlock);
    bool WriteImage(const wxBitmap& bitmap,
                    int bitmapType = wxBITMAP_TYPE_PNG);
    bool WriteImage(const wxImage& image,
                    int bitmapType = wxBITMAP_TYPE_PNG);
    //@}

    /**
        Writes text at the current position.
    */
    void WriteText(const wxString& text);

    /**
        Translates from column and line number to position.
    */
    long XYToPosition(long x, long y) const;
};
