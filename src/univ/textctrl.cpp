/////////////////////////////////////////////////////////////////////////////
// Name:        univ/textctrl.cpp
// Purpose:     wxTextCtrl
// Author:      Vadim Zeitlin
// Modified by:
// Created:     15.09.00
// RCS-ID:      $Id$
// Copyright:   (c) 2000 Vadim Zeitlin
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#ifdef __GNUG__
    #pragma implementation "univtextctrl.h"
#endif

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_TEXTCTRL

#ifndef WX_PRECOMP
    #include "wx/dcclient.h"
    #include "wx/validate.h"
    #include "wx/textctrl.h"
#endif

#include "wx/caret.h"

#include "wx/univ/inphand.h"
#include "wx/univ/renderer.h"
#include "wx/univ/theme.h"

// ============================================================================
// implementation
// ============================================================================

BEGIN_EVENT_TABLE(wxTextCtrl, wxControl)
    EVT_CHAR(OnChar)
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC_CLASS(wxTextCtrl, wxControl)

// ----------------------------------------------------------------------------
// creation
// ----------------------------------------------------------------------------

void wxTextCtrl::Init()
{
    m_selAnchor =
    m_selStart =
    m_selEnd = -1;

    m_isModified = FALSE;
    m_isEditable = TRUE;

    m_caret = (wxCaret *)NULL;
}

bool wxTextCtrl::Create(wxWindow *parent,
                        wxWindowID id,
                        const wxString& value,
                        const wxPoint& pos,
                        const wxSize& size,
                        long style,
                        const wxValidator& validator,
                        const wxString &name)
{
    if ( !wxControl::Create(parent, id, pos, size, style,
                            validator, name) )
    {
        return FALSE;
    }

    // FIXME use renderer
    wxCaret *caret = new wxCaret(this, 2, GetCharHeight());
    SetCaret(caret);
    caret->Show();

    SetCursor(wxCURSOR_IBEAM);

    SetValue(value);
    SetBestSize(size);

    return TRUE;
}

// ----------------------------------------------------------------------------
// set/get the value
// ----------------------------------------------------------------------------

void wxTextCtrl::SetValue(const wxString& value)
{
    if ( m_value == value )
        return;

    Replace(0, GetLastPosition(), value);
}

wxString wxTextCtrl::GetValue() const
{
    return m_value;
}

void wxTextCtrl::Clear()
{
    SetValue(_T(""));
}

void wxTextCtrl::Replace(long from, long to, const wxString& text)
{
    wxCHECK_RET( from >= 0 && to >= 0 && from <= to && to <= GetLastPosition(),
                 _T("invalid range in wxTextCtrl::Replace") );

    // replace the part of the text with the new value
    wxString valueNew(m_value, (size_t)from);
    valueNew += text;
    if ( (unsigned long)to < m_value.length() )
    {
        valueNew += m_value.c_str() + (size_t)to;
    }
    m_value = valueNew;

    // update the current position
    SetInsertionPoint(from + text.length());

    // and the selection (do it after setting the cursor to have correct value
    // for selection anchor)
    ClearSelection();

    // FIXME shouldn't refresh everything of course
    Refresh();
}

void wxTextCtrl::Remove(long from, long to)
{
    if ( from > to )
    {
        // Replace() only works with correctly ordered arguments, so exchange
        // them
        long tmp = from;
        from = to;
        to = tmp;
    }

    Replace(from, to, _T(""));
}

void wxTextCtrl::WriteText(const wxString& text)
{
    Replace(m_curPos, m_curPos, text);
}

void wxTextCtrl::AppendText(const wxString& text)
{
    SetInsertionPointEnd();
    WriteText(text);
}

// ----------------------------------------------------------------------------
// current position
// ----------------------------------------------------------------------------

void wxTextCtrl::SetInsertionPoint(long pos)
{
    wxCHECK_RET( pos >= 0 && pos <= GetLastPosition(),
                 _T("insertion poitn position out of range") );

    if ( pos != m_curPos )
    {
        HideCaret();

        m_curPos = pos;

        if ( IsSingleLine() )
        {
            m_curLine = 0;
            m_curRow = m_curPos;
        }
        else // multi line
        {
            wxFAIL_MSG(_T("unimplemented for multi line"));
        }

        ShowCaret();
    }
}

void wxTextCtrl::SetInsertionPointEnd()
{
    SetInsertionPoint(GetLastPosition());
}

long wxTextCtrl::GetInsertionPoint() const
{
    return m_curPos;
}

long wxTextCtrl::GetLastPosition() const
{
    if ( IsSingleLine() )
    {
        return m_value.length();
    }
    else
    {
        wxFAIL_MSG(_T("unimplemented for multi line"));

        return -1;
    }
}

// ----------------------------------------------------------------------------
// selection
// ----------------------------------------------------------------------------

void wxTextCtrl::GetSelection(long* from, long* to) const
{
    if ( from )
        *from = m_selStart;
    if ( to )
        *to = m_selEnd;
}

void wxTextCtrl::SetSelection(long from, long to)
{
    if ( from == -1 || to == -1 )
    {
        ClearSelection();
    }
    else // valid sel range
    {
        if ( from >= to )
        {
            long tmp = from;
            from = to;
            to = tmp;
        }

        wxCHECK_RET( to <= GetLastPosition(),
                     _T("invalid range in wxTextCtrl::SetSelection") );

        if ( from != m_selStart || to != m_selEnd )
        {
            m_selStart = from;
            m_selEnd = to;

            Refresh();
        }
        //else: nothing to do
    }
}

void wxTextCtrl::ClearSelection()
{
    m_selStart =
    m_selEnd = -1;

    m_selAnchor = m_curPos;

    Refresh();
}

void wxTextCtrl::RemoveSelection()
{
    if ( !HasSelection() )
        return;

    Remove(m_selStart, m_selEnd);
}

// ----------------------------------------------------------------------------
// flags
// ----------------------------------------------------------------------------

bool wxTextCtrl::IsModified() const
{
    return m_isModified;
}

bool wxTextCtrl::IsEditable() const
{
    return m_isEditable;
}

void wxTextCtrl::DiscardEdits()
{
    m_isModified = FALSE;
}

void wxTextCtrl::SetEditable(bool editable)
{
    if ( editable != m_isEditable )
    {
        m_isEditable = editable;

        Refresh();
    }
}

// ----------------------------------------------------------------------------
// col/lines <-> position correspondence
// ----------------------------------------------------------------------------

int wxTextCtrl::GetLineLength(long line) const
{
    if ( IsSingleLine() )
    {
        wxASSERT_MSG( line == 0, _T("invalid GetLineLength() parameter") );

        return m_value.length();
    }
    else // multiline
    {
        wxFAIL_MSG(_T("unimplemented for multi line"));

        return 0;
    }
}

wxString wxTextCtrl::GetLineText(long line) const
{
    if ( IsSingleLine() )
    {
        wxASSERT_MSG( line == 0, _T("invalid GetLineLength() parameter") );

        return m_value;
    }
    else // multiline
    {
        wxFAIL_MSG(_T("unimplemented for multi line"));

        return _T("");
    }
}

int wxTextCtrl::GetNumberOfLines() const
{
    if ( IsSingleLine() )
    {
        return 1;
    }
    else // multiline
    {
        wxFAIL_MSG(_T("unimplemented for multi line"));

        return 0;
    }
}

long wxTextCtrl::XYToPosition(long x, long y) const
{
    // note that this method should accept any values of x and y and return -1
    // if they are out of range
    if ( IsSingleLine() )
    {
        return x > GetLastPosition() || y > 0 ? -1 : x;
    }
    else // multiline
    {
        wxFAIL_MSG(_T("unimplemented for multi line"));

        return -1;
    }
}

bool wxTextCtrl::PositionToXY(long pos, long *x, long *y) const
{
    if ( IsSingleLine() )
    {
        if ( x )
            *x = m_curPos;
        if ( y )
            *y = 0;

        return TRUE;
    }
    else // multiline
    {
        wxFAIL_MSG(_T("unimplemented for multi line"));

        return FALSE;
    }
}

void wxTextCtrl::ShowPosition(long pos)
{
    wxFAIL_MSG(_T("not implemented"));
}

// ----------------------------------------------------------------------------
// word stuff
// ----------------------------------------------------------------------------

/*
    TODO: we could have (easy to do) vi-like options for word movement, i.e.
          distinguish between inlusive/exclusive words and between words and
          WORDS (in vim sense) and also, finally, make the set of characters
          which make up a word configurable - currently we use the exclusive
          WORDS only (coincidentally, this is what Windows edit control does)

          For future references, here is what vim help says:

          A word consists of a sequence of letters, digits and underscores, or
          a sequence of other non-blank characters, separated with white space
          (spaces, tabs, <EOL>).  This can be changed with the 'iskeyword'
          option.

          A WORD consists of a sequence of non-blank characters, separated with
          white space.  An empty line is also considered to be a word and a
          WORD.
 */

static inline bool IsWordChar(wxChar ch)
{
    return !wxIsspace(ch);
}

long wxTextCtrl::GetWordStart() const
{
    if ( m_curPos == -1 || m_curPos == 0 )
        return 0;

    // start at the previous position
    const wxChar *p0 = m_value.c_str();
    const wxChar *p = p0 + m_curPos - 1;

    // find the end of the previous word
    while ( (p > p0) && !IsWordChar(*p) )
        p--;

    // now find the beginning of this word
    while ( (p > p0) && IsWordChar(*p) )
        p--;

    // we might have gone too far
    if ( !IsWordChar(*p) )
        p++;

    return p - p0;
}

long wxTextCtrl::GetWordEnd() const
{
    if ( m_curPos == -1 )
        return 0;

    // start at the current position
    const wxChar *p0 = m_value.c_str();
    const wxChar *p = p0 + m_curPos;

    // find the start of the next word
    while ( *p && !IsWordChar(*p) )
        p++;

    // now find the end of it
    while ( *p && IsWordChar(*p) )
        p++;

    // and find the start of the next word
    while ( *p && !IsWordChar(*p) )
        p++;

    return p - p0;
}

// ----------------------------------------------------------------------------
// clipboard stuff
// ----------------------------------------------------------------------------

void wxTextCtrl::Copy()
{
    wxFAIL_MSG(_T("not implemented"));
}

void wxTextCtrl::Cut()
{
    wxFAIL_MSG(_T("not implemented"));
}

void wxTextCtrl::Paste()
{
    wxFAIL_MSG(_T("not implemented"));
}


void wxTextCtrl::Undo()
{
    wxFAIL_MSG(_T("not implemented"));
}

void wxTextCtrl::Redo()
{
    wxFAIL_MSG(_T("not implemented"));
}


bool wxTextCtrl::CanUndo() const
{
    return FALSE;
}

bool wxTextCtrl::CanRedo() const
{
    return FALSE;
}

// ----------------------------------------------------------------------------
// geometry
// ----------------------------------------------------------------------------

wxSize wxTextCtrl::DoGetBestClientSize() const
{
    wxCoord w, h;
    GetTextExtent(GetLineText(0), &w, &h);

    int wChar = GetCharWidth(),
        hChar = GetCharHeight();

    if ( w < wChar )
        w = 8*wChar;
    if ( h < hChar )
        h = hChar;

    w += 2*wChar;
    h += hChar / 2;

    return wxSize(w, h);
}

// ----------------------------------------------------------------------------
// drawing
// ----------------------------------------------------------------------------

void wxTextCtrl::DoDraw(wxControlRenderer *renderer)
{
    if ( IsSingleLine() )
    {
        // just redraw everything
        renderer->DrawTextLine(m_value, (int)m_selStart, (int)m_selEnd);
    }
    else
    {
        // TODO
    }
}

void wxTextCtrl::ShowCaret(bool show)
{
    wxCaret *caret = GetCaret();
    if ( caret )
    {
        caret->Show(show);

        if ( caret->IsVisible() )
        {
            // position it correctly
            wxString textBeforeCaret(m_value, (size_t)m_curPos);
            wxCoord x;
            GetTextExtent(textBeforeCaret, &x, NULL);

            // FIXME: use renderer
            caret->Move(x + 3, 3);
        }
    }
}

// ----------------------------------------------------------------------------
// input
// ----------------------------------------------------------------------------

wxString wxTextCtrl::GetInputHandlerType() const
{
    return wxINP_HANDLER_TEXTCTRL;
}

bool wxTextCtrl::PerformAction(const wxControlAction& actionOrig,
                               long numArg,
                               const wxString& strArg)
{
    bool textChanged = FALSE;

    wxString action;
    bool del = FALSE,
         sel = FALSE;
    if ( actionOrig.StartsWith(wxACTION_TEXT_PREFIX_DEL, &action) )
    {
        del = TRUE;
    }
    else if ( actionOrig.StartsWith(wxACTION_TEXT_PREFIX_SEL, &action) )
    {
        sel = TRUE;
    }
    else // not selection nor delete action
    {
        action = actionOrig;
    }

    long newPos = -1;
    if ( action == wxACTION_TEXT_HOME )
    {
        newPos = m_curPos - m_curRow;
    }
    else if ( action == wxACTION_TEXT_END )
    {
        newPos = m_curPos + GetLineLength(m_curLine) - m_curRow;
    }
    else if ( action == wxACTION_TEXT_LEFT )
    {
        newPos = m_curPos - 1;
    }
    else if ( action == wxACTION_TEXT_WORD_LEFT )
    {
        newPos = GetWordStart();
    }
    else if ( action == wxACTION_TEXT_RIGHT )
    {
        newPos = m_curPos + 1;
    }
    else if ( action == wxACTION_TEXT_WORD_RIGHT )
    {
        newPos = GetWordEnd();
    }
    else if ( action == wxACTION_TEXT_INSERT )
    {
        if ( !strArg.empty() )
        {
            // replace the selection with the new text
            RemoveSelection();

            WriteText(strArg);

            textChanged = TRUE;
        }
    }
    else if ( action == wxACTION_TEXT_ANCHOR_SEL )
    {
        newPos = numArg;
    }
    else if ( action == wxACTION_TEXT_EXTEND_SEL )
    {
        SetSelection(m_selAnchor, numArg);
    }
    else
    {
        return wxControl::PerformAction(action, numArg, strArg);
    }

    if ( newPos != -1 )
    {
        // bring the new position into the range
        if ( newPos < 0 )
            newPos = 0;

        long posLast = GetLastPosition();
        if ( newPos > posLast )
            newPos = posLast;

        if ( del )
        {
            // if we have the selection, remove just it
            if ( HasSelection() )
            {
                RemoveSelection();
            }
            else
            {
                // otherwise delete everything between current position and
                // the new one
                if ( m_curPos != newPos )
                {
                    Remove(m_curPos, newPos);

                    textChanged = TRUE;
                }
            }
        }
        else // cursor movement command
        {
            // just go there
            SetInsertionPoint(newPos);

            if ( sel )
            {
                SetSelection(m_selAnchor, m_curPos);
            }
            else // simple movement
            {
                // clear the existing selection
                ClearSelection();
            }
        }
    }

    if ( textChanged )
    {
        wxCommandEvent event(wxEVT_COMMAND_TEXT_UPDATED, GetId());
        InitCommandEvent(event);
        event.SetString(GetValue());
        GetEventHandler()->ProcessEvent(event);
    }

    return TRUE;
}

void wxTextCtrl::OnChar(wxKeyEvent& event)
{
    // only process the key events from "simple keys" here
    if ( !event.HasModifiers() )
    {
        int keycode = event.GetKeyCode();
        if ( keycode != WXK_DELETE && keycode != WXK_BACK )
        {
            PerformAction(wxACTION_TEXT_INSERT, -1, (wxChar)keycode);

            // skip event.Skip() below
            return;
        }
    }

    event.Skip();
}

// ----------------------------------------------------------------------------
// wxStdTextCtrlInputHandler
// ----------------------------------------------------------------------------

wxStdTextCtrlInputHandler::wxStdTextCtrlInputHandler(wxInputHandler *inphand)
                         : wxStdInputHandler(inphand)
{
    m_winCapture = (wxTextCtrl *)NULL;
}

/* static */
long wxStdTextCtrlInputHandler::HitTest(const wxTextCtrl *text,
                                        const wxPoint& pos)
{
    int x, y;
    text->CalcUnscrolledPosition(pos.x, pos.y, &x, &y);

    // row calculation is simple as we assume that all lines have the same
    // height
    int row = y / text->GetCharHeight();
    int rowMax = text->GetNumberOfLines() - 1;
    if ( row > rowMax )
    {
        // clicking below the text is the same as clicking on the last line
        row = rowMax;
    }

    // if the line is empty, the column can only be the first one
    wxString line = text->GetLineText(row);
    int col;
    wxClientDC dc(wxConstCast(text, wxTextCtrl));
    dc.SetFont(text->GetFont());

    wxCoord width;
    dc.GetTextExtent(line, &width, NULL);
    if ( x >= width )
    {
        // clicking beyond the end of line is equivalent to clicking at
        // the end of it
        col = line.length();
    }
    else // we're inside the line
    {
        // now calculate the column: first, approximate it with fixed-width
        // value and then calculate the correct value iteratively: note that
        // we use the first character of the line instead of (average)
        // GetCharWidth(): it is common to have lines of dashes, for example,
        // and this should give us much better approximation in such case
        dc.GetTextExtent(line[0], &width, NULL);
        col = x / width;

        // matchDir is -1 if we must move left, +1 to move right and 0 when
        // we're exactly on the character we need
        int matchDir = 0;
        for ( ;; )
        {
            // check that we didn't go beyond the line boundary
            if ( col < 0 )
            {
                col = 0;
                break;
            }
            if ( col > line.length() )
            {
                col = line.length();
                break;
            }

            wxString strBefore(line, (size_t)col);
            dc.GetTextExtent(strBefore, &width, NULL);
            if ( width >= x )
            {
                if ( matchDir == 1 )
                {
                    // we were going to the right and, finally, moved beyond
                    // the original position: so the current is the next after
                    // the correct one
                    col--;

                    break;
                }
                else if ( matchDir == 0 )
                {
                    // we just started iterating, now we know that we should
                    // move to the left
                    matchDir = -1;
                }
                //else: we are still to the right of the target, continue
            }
            else // width < x
            {
                // same logic as above ...
                if ( matchDir == -1 )
                {
                    // ... except that we don't need to back track
                    break;
                }
                else if ( matchDir == 0 )
                {
                    // go to the right
                    matchDir = 1;
                }
            }

            // this is not supposed to happen
            wxASSERT_MSG( matchDir, _T("logic error in wxTextCtrl::HitTest") );

            if ( matchDir == 1 )
                col++;
            else
                col--;
        }
    }

    return text->XYToPosition(col, row);
}

bool wxStdTextCtrlInputHandler::HandleKey(wxControl *control,
                                          const wxKeyEvent& event,
                                          bool pressed)
{
    if ( !pressed )
        return FALSE;

    wxControlAction action;
    wxString str;
    bool ctrlDown = event.ControlDown();
    if ( event.ShiftDown() )
    {
        action = wxACTION_TEXT_PREFIX_SEL;
    }

    int keycode = event.GetKeyCode();
    switch ( keycode )
    {
        // cursor movement
        case WXK_HOME:
            action << wxACTION_TEXT_HOME;
            break;

        case WXK_END:
            action << wxACTION_TEXT_END;
            break;

        case WXK_LEFT:
            action << (ctrlDown ? wxACTION_TEXT_WORD_LEFT
                                : wxACTION_TEXT_LEFT);
            break;

        case WXK_RIGHT:
            action << (ctrlDown ? wxACTION_TEXT_WORD_RIGHT
                                : wxACTION_TEXT_RIGHT);
            break;

        // delete
        case WXK_DELETE:
            action << wxACTION_TEXT_PREFIX_DEL << wxACTION_TEXT_RIGHT;
            break;

        case WXK_BACK:
            action << wxACTION_TEXT_PREFIX_DEL << wxACTION_TEXT_LEFT;
            break;

        // something else
        default:
            // reset the action as it could be already set to one of the
            // prefixes
            action = wxACTION_NONE;
    }

    if ( action != wxACTION_NONE )
    {
        control->PerformAction(action, -1, str);

        return TRUE;
    }

    return wxStdInputHandler::HandleKey(control, event, pressed);
}

bool wxStdTextCtrlInputHandler::HandleMouse(wxControl *control,
                                            const wxMouseEvent& event)
{
    if ( event.LeftDown() )
    {
        wxASSERT_MSG( !m_winCapture, _T("left button going down twice?") );

        wxTextCtrl *text = wxStaticCast(control, wxTextCtrl);

        m_winCapture = text;
        m_winCapture->CaptureMouse();

        text->HideCaret();

        long pos = HitTest(text, event.GetPosition());
        if ( pos != -1 )
        {
            text->PerformAction(wxACTION_TEXT_ANCHOR_SEL, pos);
        }
    }
    else if ( event.LeftDClick() )
    {
        // TODO: select the word the cursor is on
    }
    else if ( event.LeftUp() )
    {
        if ( m_winCapture )
        {
            m_winCapture->ShowCaret();

            m_winCapture->ReleaseMouse();
            m_winCapture = (wxTextCtrl *)NULL;
        }
    }

    return wxStdInputHandler::HandleMouse(control, event);
}

bool wxStdTextCtrlInputHandler::HandleMouseMove(wxControl *control,
                                                const wxMouseEvent& event)
{
    if ( m_winCapture )
    {
        // track it
        wxTextCtrl *text = wxStaticCast(m_winCapture, wxTextCtrl);
        long pos = HitTest(text, event.GetPosition());
        if ( pos != -1 )
        {
            text->PerformAction(wxACTION_TEXT_EXTEND_SEL, pos);
        }
    }

    return wxStdInputHandler::HandleMouseMove(control, event);
}

#endif // wxUSE_TEXTCTRL
