/////////////////////////////////////////////////////////////////////////////
// Name:        htmlcell.cpp
// Purpose:     wxHtmlCell - basic element of HTML output
// Author:      Vaclav Slavik
// Copyright:   (c) 1999 Vaclav Slavik
// Licence:     wxWindows Licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "htmlcell.h"
#endif

#include "wx/wxprec.h"

#if wxUSE_HTML

#ifdef __BORDLANDC__
#pragma hdrstop
#endif

#ifndef WXPRECOMP
#include "wx/wx.h"
#endif

#include "wx/html/htmlcell.h"
#include "wx/html/htmlwin.h"
#include <stdlib.h>


//-----------------------------------------------------------------------------
// wxHtmlCell
//-----------------------------------------------------------------------------


void wxHtmlCell::OnMouseClick(wxWindow *parent, int x, int y,
                              bool WXUNUSED(left),
                              bool WXUNUSED(middle),
                              bool WXUNUSED(right))
{
    wxString lnk = GetLink(x, y);
    if (lnk != wxEmptyString)
        ((wxHtmlWindow*)parent) -> OnLinkClicked(lnk);
        // note : this overcasting is legal because parent is *always* wxHtmlWindow
}



//-----------------------------------------------------------------------------
// wxHtmlWordCell
//-----------------------------------------------------------------------------

wxHtmlWordCell::wxHtmlWordCell(const wxString& word, wxDC& dc) : wxHtmlCell()
{
    m_Word = word;
    m_Word.Replace("&nbsp;", " ", TRUE);
    m_Word.Replace("&quot;", "\"", TRUE);
    m_Word.Replace("&lt;", "<", TRUE);
    m_Word.Replace("&gt;", ">", TRUE);
    m_Word.Replace("&amp;", "&", TRUE);
    dc.GetTextExtent(m_Word, &m_Width, &m_Height, &m_Descent);
}



void wxHtmlWordCell::Draw(wxDC& dc, int x, int y, int view_y1, int view_y2)
{
    dc.DrawText(m_Word, x + m_PosX, y + m_PosY);
    wxHtmlCell::Draw(dc, x, y, view_y1, view_y2);
}



//-----------------------------------------------------------------------------
// wxHtmlContainerCell
//-----------------------------------------------------------------------------


wxHtmlContainerCell::wxHtmlContainerCell(wxHtmlContainerCell *parent) : wxHtmlCell()
{
    m_Cells = m_LastCell = NULL;
    m_Parent = parent;
    if (m_Parent) m_Parent -> InsertCell(this);
    m_AlignHor = HTML_ALIGN_LEFT;
    m_AlignVer = HTML_ALIGN_BOTTOM;
    m_IndentLeft = m_IndentRight = m_IndentTop = m_IndentBottom = 0;
    m_WidthFloat = 100; m_WidthFloatUnits = HTML_UNITS_PERCENT;
    m_UseBkColour = FALSE;
    m_UseBorder = FALSE;
    m_MinHeight = m_MaxLineWidth = 0;
    m_MinHeightAlign = HTML_ALIGN_TOP;
}



void wxHtmlContainerCell::SetIndent(int i, int what, int units)
{
    int val = (units == HTML_UNITS_PIXELS) ? i : -i;
    if (what & HTML_INDENT_LEFT) m_IndentLeft = val;
    if (what & HTML_INDENT_RIGHT) m_IndentRight = val;
    if (what & HTML_INDENT_TOP) m_IndentTop = val;
    if (what & HTML_INDENT_BOTTOM) m_IndentBottom = val;
}



int wxHtmlContainerCell::GetIndent(int ind) const
{
    if (ind & HTML_INDENT_LEFT) return m_IndentLeft;
    else if (ind & HTML_INDENT_RIGHT) return m_IndentRight;
    else if (ind & HTML_INDENT_TOP) return m_IndentTop;
    else if (ind & HTML_INDENT_BOTTOM) return m_IndentBottom;
    else return -1; /* BUG! Should not be called... */
}




int wxHtmlContainerCell::GetIndentUnits(int ind) const
{
    bool p = FALSE;
    if (ind & HTML_INDENT_LEFT) p = m_IndentLeft < 0;
    else if (ind & HTML_INDENT_RIGHT) p = m_IndentRight < 0;
    else if (ind & HTML_INDENT_TOP) p = m_IndentTop < 0;
    else if (ind & HTML_INDENT_BOTTOM) p = m_IndentBottom < 0;
    if (p) return HTML_UNITS_PERCENT;
    else return HTML_UNITS_PIXELS;
}



void wxHtmlContainerCell::Layout(int w)
{
    wxHtmlCell *cell = m_Cells, *line = m_Cells;
    long xpos = 0, ypos = m_IndentTop;
    int xdelta = 0, ybasicpos = 0, ydiff;
    int s_width, s_indent;
    int ysizeup = 0, ysizedown = 0;

    /*
    
    WIDTH ADJUSTING :
    
    */

    if (m_WidthFloatUnits == HTML_UNITS_PERCENT) {
        if (m_WidthFloat < 0) m_Width = (100 + m_WidthFloat) * w / 100;
        else m_Width = m_WidthFloat * w / 100;
    }
    else {
        if (m_WidthFloat < 0) m_Width = w + m_WidthFloat;
        else m_Width = m_WidthFloat;
    }

    if (m_Cells) {
        int l = (m_IndentLeft < 0) ? (-m_IndentLeft * m_Width / 100) : m_IndentLeft;
        int r = (m_IndentRight < 0) ? (-m_IndentRight * m_Width / 100) : m_IndentRight;
        m_Cells -> Layout(m_Width - (l + r));
    }

    /*

    LAYOUTING :
    
    */

    // adjust indentation:
    s_indent = (m_IndentLeft < 0) ? (-m_IndentLeft * m_Width / 100) : m_IndentLeft;
    s_width = m_Width - s_indent - ((m_IndentRight < 0) ? (-m_IndentRight * m_Width / 100) : m_IndentRight);

    m_MaxLineWidth = 0;
    
    // my own layouting:
    while (cell != NULL) {
        switch (m_AlignVer) {
            case HTML_ALIGN_TOP :      ybasicpos = 0; break;
            case HTML_ALIGN_BOTTOM :   ybasicpos = - cell -> GetHeight(); break;
            case HTML_ALIGN_CENTER :   ybasicpos = - cell -> GetHeight() / 2; break;
        }
        ydiff = cell -> GetHeight() + ybasicpos;

        if (cell -> GetDescent() + ydiff > ysizedown) ysizedown = cell -> GetDescent() + ydiff;
        if (ybasicpos + cell -> GetDescent() < -ysizeup) ysizeup = - (ybasicpos + cell -> GetDescent());

        cell -> SetPos(xpos, ybasicpos + cell -> GetDescent());
        xpos += cell -> GetWidth();
        cell = cell -> GetNext();

        // force new line if occured:
        if ((cell == NULL) || (xpos + cell -> GetWidth() > s_width)) {
            if (xpos > m_MaxLineWidth) m_MaxLineWidth = xpos;
            if (ysizeup < 0) ysizeup = 0;
            if (ysizedown < 0) ysizedown = 0;
            switch (m_AlignHor) {
                case HTML_ALIGN_LEFT : xdelta = 0; break;
                case HTML_ALIGN_RIGHT : xdelta = 0 + (s_width - xpos); break;
                case HTML_ALIGN_CENTER : xdelta = 0 + (s_width - xpos) / 2; break;
            }
            if (xdelta < 0) xdelta = 0;
            xdelta += s_indent;

            ypos += ysizeup;
            while (line != cell) {
                line -> SetPos(line -> GetPosX() + xdelta, ypos + line -> GetPosY());
                line = line -> GetNext();
            }

            ypos += ysizedown;
            xpos = 0;
            ysizeup = ysizedown = 0;
            line = cell;
        }
    }

    // setup height & width, depending on container layout:
    m_Height = ypos + (ysizedown + ysizeup) + m_IndentBottom;

    if (m_Height < m_MinHeight) {
        if (m_MinHeightAlign != HTML_ALIGN_TOP) {
            int diff = m_MinHeight - m_Height;
            if (m_MinHeightAlign == HTML_ALIGN_CENTER) diff /= 2;
            cell = m_Cells;
            while (cell) {
                cell -> SetPos(cell -> GetPosX(), cell -> GetPosY() + diff);
                cell = cell -> GetNext();
            }
        }
        m_Height = m_MinHeight;
    }

    m_MaxLineWidth += s_indent + ((m_IndentRight < 0) ? (-m_IndentRight * m_Width / 100) : m_IndentRight);
    if (m_Width < m_MaxLineWidth) m_Width = m_MaxLineWidth;

    wxHtmlCell::Layout(w);
}


#define mMin(a, b) (((a) < (b)) ? (a) : (b))
#define mMax(a, b) (((a) < (b)) ? (b) : (a))

void wxHtmlContainerCell::Draw(wxDC& dc, int x, int y, int view_y1, int view_y2)
{
    // container visible, draw it:
    if ((y + m_PosY < view_y2) && (y + m_PosY + m_Height > view_y1)) {

        if (m_UseBkColour) {
            wxBrush myb = wxBrush(m_BkColour, wxSOLID);

            int real_y1 = mMax(y + m_PosY, view_y1);
            int real_y2 = mMin(y + m_PosY + m_Height - 1, view_y2);

            dc.SetBrush(myb);
            dc.SetPen(*wxTRANSPARENT_PEN);
            dc.DrawRectangle(x + m_PosX, real_y1, m_Width, real_y2 - real_y1 + 1);
        }

        if (m_UseBorder) {
            wxPen mypen1(m_BorderColour1, 1, wxSOLID);
            wxPen mypen2(m_BorderColour2, 1, wxSOLID);

            dc.SetPen(mypen1);
            dc.DrawLine(x + m_PosX, y + m_PosY, x + m_PosX, y + m_PosY + m_Height - 1);
            dc.DrawLine(x + m_PosX, y + m_PosY, x + m_PosX + m_Width - 1, y + m_PosY);
            dc.SetPen(mypen2);
            dc.DrawLine(x + m_PosX + m_Width - 1, y + m_PosY, x + m_PosX +  m_Width - 1, y + m_PosY + m_Height - 1);
            dc.DrawLine(x + m_PosX, y + m_PosY + m_Height - 1, x + m_PosX + m_Width - 1, y + m_PosY + m_Height - 1);
        }

        if (m_Cells) m_Cells -> Draw(dc, x + m_PosX, y + m_PosY, view_y1, view_y2);
    }
    // container invisible, just proceed font+color changing:
    else {
        if (m_Cells) m_Cells -> DrawInvisible(dc, x + m_PosX, y + m_PosY);
    }

    wxHtmlCell::Draw(dc, x, y, view_y1, view_y2);
}



void wxHtmlContainerCell::DrawInvisible(wxDC& dc, int x, int y)
{
    if (m_Cells) m_Cells -> DrawInvisible(dc, x + m_PosX, y + m_PosY);
    wxHtmlCell::DrawInvisible(dc, x, y);
}



wxString wxHtmlContainerCell::GetLink(int x, int y) const
{
    wxHtmlCell *c = m_Cells;
    int cx, cy, cw, ch;

    while (c) {
        cx = c -> GetPosX(), cy = c -> GetPosY();
        cw = c -> GetWidth(), ch = c -> GetHeight();
        if ((x >= cx) && (x < cx + cw) && (y >= cy) && (y < cy + ch))
            return c -> GetLink(x - cx, y - cy);
        c = c -> GetNext();
    }
    return wxEmptyString;
}



void wxHtmlContainerCell::InsertCell(wxHtmlCell *f)
{
    if (!m_Cells) m_Cells = m_LastCell = f;
    else {
        m_LastCell -> SetNext(f);
        m_LastCell = f;
        if (m_LastCell) while (m_LastCell -> GetNext()) m_LastCell = m_LastCell -> GetNext();
    }
    f -> SetParent(this);
}



void wxHtmlContainerCell::SetAlign(const wxHtmlTag& tag)
{
    if (tag.HasParam("ALIGN")) {
        wxString alg = tag.GetParam("ALIGN");
        alg.MakeUpper();
        if (alg == "CENTER")
            SetAlignHor(HTML_ALIGN_CENTER);
        else if (alg == "LEFT")
            SetAlignHor(HTML_ALIGN_LEFT);
        else if (alg == "RIGHT")
            SetAlignHor(HTML_ALIGN_RIGHT);
    }
}



void wxHtmlContainerCell::SetWidthFloat(const wxHtmlTag& tag)
{
    if (tag.HasParam("WIDTH")) {
        int wdi;
        wxString wd = tag.GetParam("WIDTH");

        if (wd[wd.Length()-1] == '%') {
            sscanf(wd.c_str(), "%i%%", &wdi);
            SetWidthFloat(wdi, HTML_UNITS_PERCENT);
        }
        else {
            sscanf(wd.c_str(), "%i", &wdi);
            SetWidthFloat(wdi, HTML_UNITS_PIXELS);
        }
    }
}



const wxHtmlCell* wxHtmlContainerCell::Find(int condition, const void* param) const
{
    const wxHtmlCell *r = NULL;
    
    if (m_Cells) {
        r = m_Cells -> Find(condition, param);
        if (r) return r;
    }

    return wxHtmlCell::Find(condition, param);
}



void wxHtmlContainerCell::OnMouseClick(wxWindow *parent, int x, int y, bool left, bool middle, bool right)
{
    if (m_Cells) {
        wxHtmlCell *c = m_Cells;
        while (c) {
            if (    (c -> GetPosX() <= x) &&
                    (c -> GetPosY() <= y) &&
                    (c -> GetPosX() + c -> GetWidth() > x) &&
                    (c -> GetPosY() + c -> GetHeight() > y)) {
                c -> OnMouseClick(parent, x - c -> GetPosX(), y - c -> GetPosY(), left, middle, right);
                break;
            }
            c = c -> GetNext();
        }
    }
}





//--------------------------------------------------------------------------------
// wxHtmlColourCell
//--------------------------------------------------------------------------------

void wxHtmlColourCell::Draw(wxDC& dc, int x, int y, int view_y1, int view_y2)
{
    if (m_Flags & HTML_CLR_FOREGROUND)
        dc.SetTextForeground(m_Colour);
    if (m_Flags & HTML_CLR_BACKGROUND) {
        dc.SetBackground(wxBrush(m_Colour, wxSOLID));
        dc.SetTextBackground(m_Colour);
    }
    wxHtmlCell::Draw(dc, x, y, view_y1, view_y2);
}

void wxHtmlColourCell::DrawInvisible(wxDC& dc, int x, int y)
{
    if (m_Flags & HTML_CLR_FOREGROUND)
        dc.SetTextForeground(m_Colour);
    if (m_Flags & HTML_CLR_BACKGROUND) {
        dc.SetBackground(wxBrush(m_Colour, wxSOLID));
        dc.SetTextBackground(m_Colour);
    }
    wxHtmlCell::DrawInvisible(dc, x, y);
}




//--------------------------------------------------------------------------------
// wxHtmlFontCell
//--------------------------------------------------------------------------------

void wxHtmlFontCell::Draw(wxDC& dc, int x, int y, int view_y1, int view_y2)
{
    dc.SetFont(*m_Font);
    wxHtmlCell::Draw(dc, x, y, view_y1, view_y2);
}

void wxHtmlFontCell::DrawInvisible(wxDC& dc, int x, int y)
{
    dc.SetFont(*m_Font);
    wxHtmlCell::DrawInvisible(dc, x, y);
}








//--------------------------------------------------------------------------------
// wxHtmlWidgetCell
//--------------------------------------------------------------------------------

wxHtmlWidgetCell::wxHtmlWidgetCell(wxWindow *wnd, int w)
{
    int sx, sy;
    m_Wnd = wnd;
    m_Wnd -> GetSize(&sx, &sy);
    m_Width = sx, m_Height = sy;
    m_WidthFloat = w;
}


void wxHtmlWidgetCell::Draw(wxDC& dc, int x, int y, int view_y1, int view_y2)
{
    int absx = 0, absy = 0, stx, sty;
    wxHtmlCell *c = this;

    while (c) {
        absx += c -> GetPosX();
        absy += c -> GetPosY();
        c = c -> GetParent();
    }

    ((wxScrolledWindow*)(m_Wnd -> GetParent())) -> ViewStart(&stx, &sty);

    m_Wnd -> SetSize(absx - HTML_SCROLL_STEP * stx, absy  - HTML_SCROLL_STEP * sty, m_Width, m_Height);
//    m_Wnd -> Refresh();

    wxHtmlCell::Draw(dc, x, y, view_y1, view_y2);
}



void wxHtmlWidgetCell::DrawInvisible(wxDC& dc, int x, int y)
{
    int absx = 0, absy = 0, stx, sty;
    wxHtmlCell *c = this;

    while (c) {
        absx += c -> GetPosX();
        absy += c -> GetPosY();
        c = c -> GetParent();
    }
    ((wxScrolledWindow*)(m_Wnd -> GetParent())) -> ViewStart(&stx, &sty);

    m_Wnd -> SetSize(absx - HTML_SCROLL_STEP * stx, absy  - HTML_SCROLL_STEP * sty, m_Width, m_Height);
    wxHtmlCell::DrawInvisible(dc, x, y);
}



void wxHtmlWidgetCell::Layout(int w)
{
    if (m_WidthFloat != 0) {
        m_Width = (w * m_WidthFloat) / 100;
        m_Wnd -> SetSize(m_Width, m_Height);
    }

    wxHtmlCell::Layout(w);
}

#endif
