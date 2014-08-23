/////////////////////////////////////////////////////////////////////////////
// Name:        src/qt/textctrl.cpp
// Author:      Peter Most
// Copyright:   (c) Peter Most
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/textctrl.h"
#include "wx/qt/converter.h"
#include "wx/qt/utils.h"

wxTextCtrl::wxTextCtrl()
{
}

wxTextCtrl::wxTextCtrl(wxWindow *parent,
           wxWindowID id,
           const wxString &value,
           const wxPoint &pos,
           const wxSize &size,
           long style,
           const wxValidator& validator,
           const wxString &name)
{
    Create( parent, id, value, pos, size, style, validator, name );
}

bool wxTextCtrl::Create(wxWindow *parent,
            wxWindowID id,
            const wxString &value,
            const wxPoint &pos,
            const wxSize &size,
            long style,
            const wxValidator& validator,
            const wxString &name)
{
    bool multiline = (style & wxTE_MULTILINE) != 0;

    if (!multiline)
    {
        m_qtLineEdit = new QLineEdit( wxQtConvertString( value ), parent->GetHandle() );
    }
    else
    {
        m_qtTextEdit = new QTextEdit( wxQtConvertString( value ), parent->GetHandle() );
    }
    return QtCreateControl( parent, id, pos, size, style, validator, name );
}

wxSize wxTextCtrl::DoGetBestSize() const
{
    return wxTextCtrlBase::DoGetBestSize();

}
int wxTextCtrl::GetLineLength(long lineNo) const
{
    return 0;
}

wxString wxTextCtrl::GetLineText(long lineNo) const
{
    return wxString();
}

int wxTextCtrl::GetNumberOfLines() const
{
    return 0;
}

bool wxTextCtrl::IsModified() const
{
    return false;
}

void wxTextCtrl::MarkDirty()
{
}

void wxTextCtrl::DiscardEdits()
{
}

bool wxTextCtrl::SetStyle(long start, long end, const wxTextAttr& style)
{
    return false;
}

bool wxTextCtrl::GetStyle(long position, wxTextAttr& style)
{
    return false;
}

bool wxTextCtrl::SetDefaultStyle(const wxTextAttr& style)
{
    return false;
}

long wxTextCtrl::XYToPosition(long x, long y) const
{
    return 0;
}

bool wxTextCtrl::PositionToXY(long pos, long *x, long *y) const
{
    return false;
}

void wxTextCtrl::ShowPosition(long pos)
{
}

bool wxTextCtrl::DoLoadFile(const wxString& file, int fileType)
{
    return false;
}

bool wxTextCtrl::DoSaveFile(const wxString& file, int fileType)
{
    return false;
}

void wxTextCtrl::SetInsertionPoint(long pos)
{
    QTextCursor::MoveOperation op;
    QTextCursor cursor;

    // check if pos indicates end of text:
    if ( pos == -1 )
    {
        pos = 0;
        op = QTextCursor::End;
    }
    else
    {
        op = QTextCursor::Start;
    }
    if ( !IsMultiLine() )
    {
        if (op == QTextCursor::End)
            m_qtLineEdit->end(false);
        else
            m_qtLineEdit->setCursorPosition(pos);
    }
    else
    {
        cursor = m_qtTextEdit->textCursor();
        cursor.movePosition(op, QTextCursor::MoveAnchor, pos);
        m_qtTextEdit->setTextCursor(cursor);
        m_qtTextEdit->ensureCursorVisible();
    }
}

long wxTextCtrl::GetInsertionPoint() const
{
    QTextCursor cursor;

    if ( !IsMultiLine() )
    {
        return m_qtLineEdit->cursorPosition();
    }
    else
    {
        cursor = m_qtTextEdit->textCursor();
        return cursor.position();
    }
}


void wxTextCtrl::WriteText( const wxString &text )
{
    // Insert the text
    if ( !IsMultiLine() )
    {
        m_qtLineEdit->insert(wxQtConvertString( text ));
    }
    else
    {
        m_qtTextEdit->insertPlainText(wxQtConvertString( text ));
        // the cursor is moved to the end, ensure it is shown
        m_qtTextEdit->ensureCursorVisible();
    }

}

QWidget *wxTextCtrl::GetHandle() const
{
    if (!m_qtLineEdit.isNull())
        return (QWidget *) m_qtLineEdit;
    else
        return (QWidget *) m_qtTextEdit;
}
