/////////////////////////////////////////////////////////////////////////////
// Name:        wx/univ/textctrl.h
// Purpose:     wxTextCtrl class
// Author:      Vadim Zeitlin
// Modified by:
// Created:     15.09.00
// RCS-ID:      $Id$
// Copyright:   (c) 2000 Vadim Zeitlin
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_UNIV_TEXTCTRL_H_
#define _WX_UNIV_TEXTCTRL_H_

#ifdef __GNUG__
    #pragma interface "univtextctrl.h"
#endif

class WXDLLEXPORT wxCaret;
class WXDLLEXPORT wxTextCtrlCommandProcessor;

#include "wx/scrolwin.h"    // for wxScrollHelper

// ----------------------------------------------------------------------------
// wxTextCtrl actions
// ----------------------------------------------------------------------------

// cursor movement and also selection and delete operations
#define wxACTION_TEXT_GOTO          _T("goto")  // to pos in numArg
#define wxACTION_TEXT_FIRST         _T("first") // go to pos 0
#define wxACTION_TEXT_LAST          _T("last")  // go to last pos
#define wxACTION_TEXT_HOME          _T("home")
#define wxACTION_TEXT_END           _T("end")
#define wxACTION_TEXT_LEFT          _T("left")
#define wxACTION_TEXT_RIGHT         _T("right")
#define wxACTION_TEXT_UP            _T("up")
#define wxACTION_TEXT_DOWN          _T("down")
#define wxACTION_TEXT_WORD_LEFT     _T("wordleft")
#define wxACTION_TEXT_WORD_RIGHT    _T("wordright")
#define wxACTION_TEXT_PAGE_UP       _T("pageup")
#define wxACTION_TEXT_PAGE_DOWN     _T("pagedown")

// clipboard operations
#define wxACTION_TEXT_COPY          _T("copy")
#define wxACTION_TEXT_CUT           _T("cut")
#define wxACTION_TEXT_PASTE         _T("paste")

// insert text at the cursor position: the text is in strArg of PerformAction
#define wxACTION_TEXT_INSERT        _T("insert")

// if the action starts with either of these prefixes and the rest of the
// string is one of the movement commands, it means to select/delete text from
// the current cursor position to the new one
#define wxACTION_TEXT_PREFIX_SEL    _T("sel")
#define wxACTION_TEXT_PREFIX_DEL    _T("del")

// mouse selection
#define wxACTION_TEXT_ANCHOR_SEL    _T("anchorsel")
#define wxACTION_TEXT_EXTEND_SEL    _T("extendsel")
#define wxACTION_TEXT_SEL_WORD      _T("wordsel")
#define wxACTION_TEXT_SEL_LINE      _T("linesel")

// undo or redo
#define wxACTION_TEXT_UNDO          _T("undo")
#define wxACTION_TEXT_REDO          _T("redo")

// ----------------------------------------------------------------------------
// wxTextCtrl types
// ----------------------------------------------------------------------------

// wxTextPos is the position in the text
typedef long wxTextPos;

// wxTextCoord is the line or row number (which should have been unsigned but
// is long for backwards compatibility)
typedef long wxTextCoord;

// ----------------------------------------------------------------------------
// wxTextCtrl::HitTest return values
// ----------------------------------------------------------------------------

// the point asked is ...
enum wxTextCtrlHitTestResult
{
    wxTE_HT_BEFORE = -1,    // either to the left or upper
    wxTE_HT_ON_TEXT,        // directly on
    wxTE_HT_BELOW,          // below [the last line]
    wxTE_HT_BEYOND          // after [the end of line]
};
// ... the character returned

// ----------------------------------------------------------------------------
// wxTextCtrl
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxTextCtrl : public wxTextCtrlBase, public wxScrollHelper
{
public:
    // creation
    // --------

    wxTextCtrl() { Init(); }

    wxTextCtrl(wxWindow *parent,
               wxWindowID id,
               const wxString& value = wxEmptyString,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = 0,
               const wxValidator& validator = wxDefaultValidator,
               const wxString& name = wxTextCtrlNameStr)
    {
        Init();

        Create(parent, id, value, pos, size, style, validator, name);
    }

    bool Create(wxWindow *parent,
                wxWindowID id,
                const wxString& value = wxEmptyString,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = wxTextCtrlNameStr);

    virtual ~wxTextCtrl();

    // implement base class pure virtuals
    // ----------------------------------

    virtual wxString GetValue() const;
    virtual void SetValue(const wxString& value);

    virtual int GetLineLength(wxTextCoord lineNo) const;
    virtual wxString GetLineText(wxTextCoord lineNo) const;
    virtual int GetNumberOfLines() const;

    virtual bool IsModified() const;
    virtual bool IsEditable() const;

    // If the return values from and to are the same, there is no selection.
    virtual void GetSelection(wxTextPos* from, wxTextPos* to) const;

    // operations
    // ----------

    // editing
    virtual void Clear();
    virtual void Replace(wxTextPos from, wxTextPos to, const wxString& value);
    virtual void Remove(wxTextPos from, wxTextPos to);

    // clears the dirty flag
    virtual void DiscardEdits();

    // writing text inserts it at the current position, appending always
    // inserts it at the end
    virtual void WriteText(const wxString& text);
    virtual void AppendText(const wxString& text);

    // translate between the position (which is just an index in the text ctrl
    // considering all its contents as a single strings) and (x, y) coordinates
    // which represent column and line.
    virtual wxTextPos XYToPosition(wxTextCoord x, wxTextCoord y) const;
    virtual bool PositionToXY(wxTextPos pos,
                              wxTextCoord *x, wxTextCoord *y) const;

    // wxUniv-specific: find a screen position (in client coordinates) of the
    // given text position or of the caret
    bool PositionToLogicalXY(wxTextPos pos, wxCoord *x, wxCoord *y) const;
    bool PositionToDeviceXY(wxTextPos pos, wxCoord *x, wxCoord *y) const;
    wxPoint GetCaretPosition() const;

    virtual void ShowPosition(wxTextPos pos);

    // Clipboard operations
    virtual void Copy();
    virtual void Cut();
    virtual void Paste();

    // Undo/redo
    virtual void Undo();
    virtual void Redo();

    virtual bool CanUndo() const;
    virtual bool CanRedo() const;

    // Insertion point
    virtual void SetInsertionPoint(wxTextPos pos);
    virtual void SetInsertionPointEnd();
    virtual wxTextPos GetInsertionPoint() const;
    virtual wxTextPos GetLastPosition() const;

    virtual void SetSelection(wxTextPos from, wxTextPos to);
    virtual void SetEditable(bool editable);

    // wxUniv-specific methods
    // -----------------------

    // caret stuff
    virtual void ShowCaret(bool show = TRUE);
    void HideCaret() { ShowCaret(FALSE); }
    void CreateCaret(); // for the current font size

    // helpers for cursor movement
    wxTextPos GetWordStart() const;
    wxTextPos GetWordEnd() const;

    // selection helpers
    bool HasSelection() const
        { return m_selStart != -1 && m_selEnd > m_selStart; }
    void ClearSelection();
    void RemoveSelection();
    wxString GetSelectionText() const;

    // find the character at this position, return 0 if the character is
    // really there, -1 if the point is before the beginning of the text/line
    // and the returned character is the first one to follow it or +1 if it the
    // position is beyond the end of line/text and the returned character is
    // the last one
    //
    // NB: pt is in device coords (not adjusted for the client area origin nor
    //     for the scrolling)
    wxTextCtrlHitTestResult HitTest(const wxPoint& pt,
                                    wxTextCoord *col, wxTextCoord *row) const;

    // find the character at this position in the given line, return value as
    // for HitTest()
    //
    // NB: x is the logical coord (client and unscrolled)
    wxTextCtrlHitTestResult HitTestLine(const wxString& line,
                                        wxCoord x,
                                        wxTextCoord *colOut) const;

    // bring the given position into view
    void ShowHorzPosition(wxCoord pos);

    // scroll the window horizontally so that the first character shown is in
    // position pos
    void ScrollText(wxTextCoord col);

    // adjust the DC for horz text control scrolling too
    virtual void DoPrepareDC(wxDC& dc);

    // implementation only from now on
    // -------------------------------

    // override this to take into account our scrollbar-less scrolling
    virtual void CalcUnscrolledPosition(int x, int y, int *xx, int *yy) const;

    // set the right colours
    virtual bool IsContainerWindow() const { return TRUE; }
    virtual wxBorder GetDefaultBorder() const { return wxBORDER_SUNKEN; }

    // perform an action
    virtual bool PerformAction(const wxControlAction& action,
                               long numArg = -1,
                               const wxString& strArg = wxEmptyString);

    // override these methods to handle the caret
    virtual bool SetFont(const wxFont &font);
    virtual bool Enable(bool enable = TRUE);

protected:
    // draw the text
    void DrawTextLine(wxDC& dc, const wxRect& rect,
                      const wxString& text,
                      wxTextPos selStart, wxTextPos selEnd);

    void DoDrawTextInRect(wxDC& dc, const wxRect& rectUpdate);

    virtual void DoDraw(wxControlRenderer *renderer);

    // calc the size from the text extent
    virtual wxSize DoGetBestClientSize() const;

    // input support
    virtual wxString GetInputHandlerType() const;

    // common part of all ctors
    void Init();

    // more readable flag testing methods
    bool IsSingleLine() const { return !(GetWindowStyle() & wxTE_MULTILINE); }
    bool IsPassword() const { return (GetWindowStyle() & wxTE_PASSWORD) != 0; }
    bool WrapLines() const { return !(GetWindowStyle() & wxHSCROLL); }

    // get the extent (width) of the text
    wxCoord GetTextWidth(const wxString& text) const;

    // get the logical text width (accounting for scrolling)
    wxCoord GetTotalWidth() const;

    // find the number of rows in this line (only if WrapLines())
    wxTextCoord GetRowsPerLine(wxTextCoord line) const;

    // get total number of rows before the given line
    wxTextCoord GetNumberOfRowsBefore(wxTextCoord line) const;

    // the text area is the part of the window in which the text can be
    // displayed, i.e. part of it inside the margins and the real text area is
    // the area in which the text *is* currently displayed: for example, in the
    // multiline control case the text area can have extra space at the bottom
    // which is not tall enough for another line and which is then not included
    // into the real text area
    wxRect GetRealTextArea() const;

    // refresh the text in the given (in logical coords) rect
    void RefreshTextRect(const wxRect& rect);

    // refresh the text in the given range (in logical coords) of this line, if
    // width is 0, refresh to the end of line
    void RefreshPixelRange(wxTextCoord line, wxCoord start, wxCoord width);

    // refresh the text in the given range (in text coords) in this line
    void RefreshColRange(wxTextCoord line, wxTextPos start, size_t count);

    // refresh the text from in the given line range (inclusive)
    void RefreshLineRange(wxTextCoord lineFirst, wxTextCoord lineLast);

    // refresh the text in the given range which can span multiple lines
    // (this method accepts arguments in any order)
    void RefreshTextRange(wxTextPos start, wxTextPos end);

    // get the text to show: either the text itself or the text replaced with
    // starts for wxTE_PASSWORD control
    wxString GetTextToShow(const wxString& text) const;

    // find the number of characters of a line before it wraps
    // (and optionally also the real width of the line)
    size_t GetPartOfWrappedLine(const wxChar* text,
                                wxCoord *widthReal = NULL) const;

    // get the start and end of the selection for this line: if the line is
    // outside the selection, both will be -1 and FALSE will be returned
    bool GetSelectedPartOfLine(wxTextCoord line,
                               wxTextPos *start, wxTextPos *end) const;

    // update the text rect: the zone inside our client rect (its coords are
    // client coords) which contains the text
    void UpdateTextRect();

    // calculate the last visible position
    void UpdateLastVisible();

    // move caret to the given position unconditionally
    // (SetInsertionPoint() does nothing if the position didn't change)
    void DoSetInsertionPoint(wxTextPos pos);

    // set the caret to its initial (default) position
    void InitInsertionPoint();

    // get the width of the longest line in pixels
    wxCoord GetMaxWidth() const;

    // force recalculation of the max line width
    void RecalcMaxWidth()
    {
        m_widthMax = -1;
        (void)GetMaxWidth();
    }

    // update the max width after the given line was modified
    void UpdateMaxWidth(wxTextCoord line);

    // HitTest2() is more efficient than 2 consecutive HitTest()s with the same
    // line (i.e. y) and it also returns the offset of the starting position in
    // pixels
    //
    // as the last hack, this function accepts either logical or device (by
    // default) coords depending on devCoords flag
    wxTextCtrlHitTestResult HitTest2(wxCoord y,
                                     wxCoord x1,
                                     wxCoord x2,
                                     wxTextCoord *row,
                                     wxTextCoord *colStart,
                                     wxTextCoord *colEnd,
                                     wxTextCoord *colRowStart,
                                     bool devCoords = TRUE) const;

    // HitTest() version which takes the logical text coordinates and not the
    // device ones
    wxTextCtrlHitTestResult HitTestLogical(const wxPoint& pos,
                                           wxTextCoord *col,
                                           wxTextCoord *row) const;

    // event handlers
    void OnIdle(wxIdleEvent& event);
    void OnChar(wxKeyEvent& event);
    void OnSize(wxSizeEvent& event);

    // accessors for derived classes (SL stands for single line)
    const wxString& GetSLValue() const
    {
        wxASSERT_MSG( IsSingleLine(), _T("only for single line controls") );

        return m_value;
    }

    void SetSLValue(const wxString& value)
    {
        wxASSERT_MSG( IsSingleLine(), _T("only for single line controls") );
        m_value = value;
    }

    // clipboard operations (unlike the versions without Do prefix, they have a
    // return code)
    bool DoCut();
    bool DoPaste();

private:
    // update the scrollbars (only called from OnIdle)
    void UpdateScrollbars();

    // the initially specified control size
    wxSize m_sizeInitial;

    // the control text (only used for single line controls)
    wxString m_value;

    // the lines of text (only used for multiline controls)
    wxArrayString m_lines;

    // current position
    wxTextPos m_curPos;
    wxTextCoord m_curCol,
                m_curRow;

    // last position (only used by GetLastPosition())
    wxTextPos m_posLast;

    // selection
    wxTextPos m_selAnchor,
              m_selStart,
              m_selEnd;

    // flags
    bool m_isModified:1,
         m_isEditable:1,
         m_hasCaret:1;

    // the rectangle (in client coordinates) to draw text inside
    wxRect m_rectText;

    // this section is for the controls without horz scrollbar only

    // the position of the first visible pixel and the first visible column
    wxCoord m_ofsHorz;
    wxTextCoord m_colStart;

    // and the last ones (m_posLastVisible is the width but m_colLastVisible
    // is an absolute value)
    wxCoord m_posLastVisible;
    wxTextCoord m_colLastVisible;

    // this section is for the controls with scrollbar(s)

    // the current ranges of the scrollbars
    int m_scrollRangeX,
        m_scrollRangeY;

    // should we adjust the horz/vert scrollbar?
    bool m_updateScrollbarX,
         m_updateScrollbarY;

    // the max line length in pixels
    wxCoord m_widthMax;

    // the index of the line which has the length of m_widthMax
    wxTextCoord m_lineLongest;

    // the object to which we delegate our undo/redo implementation
    wxTextCtrlCommandProcessor *m_cmdProcessor;

    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(wxTextCtrl)
};

#endif // _WX_UNIV_TEXTCTRL_H_

