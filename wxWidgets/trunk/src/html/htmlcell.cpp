/////////////////////////////////////////////////////////////////////////////
// Name:        htmlcell.cpp
// Purpose:     wxHtmlCell - basic element of HTML output
// Author:      Vaclav Slavik
// RCS-ID:      $Id$
// Copyright:   (c) 1999 Vaclav Slavik
// Licence:     wxWindows Licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation
#endif

#include "wx/wxprec.h"

#include "wx/defs.h"

#if wxUSE_HTML && wxUSE_STREAMS

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

wxHtmlCell::wxHtmlCell() : wxObject() 
{
    m_Next = NULL; 
    m_Parent = NULL; 
    m_Width = m_Height = m_Descent = 0; 
    m_CanLiveOnPagebreak = TRUE;
    m_Link = NULL;
}

wxHtmlCell::~wxHtmlCell() 
{
    if (m_Link) delete m_Link; 
    if (m_Next) delete m_Next;
}


void wxHtmlCell::OnMouseClick(wxWindow *parent, int x, int y, 
                              const wxMouseEvent& event)
{
    wxHtmlLinkInfo *lnk = GetLink(x, y);
    if (lnk != NULL)
    {
        wxHtmlLinkInfo lnk2(*lnk);
        lnk2.SetEvent(&event);
        lnk2.SetHtmlCell(this);
        ((wxHtmlWindow*)parent)->OnLinkClicked(lnk2);
        // note : this overcasting is legal because parent is *always* wxHtmlWindow
    }
}



bool wxHtmlCell::AdjustPagebreak(int *pagebreak) const
{
    if ((!m_CanLiveOnPagebreak) && 
                m_PosY < *pagebreak && m_PosY + m_Height > *pagebreak) 
	{
        *pagebreak = m_PosY;
        if (m_Next != NULL) m_Next->AdjustPagebreak(pagebreak);
        return TRUE;
    }
    
    else 
    {
        if (m_Next != NULL) return m_Next->AdjustPagebreak(pagebreak);
        else return FALSE;
    }
}



void wxHtmlCell::SetLink(const wxHtmlLinkInfo& link) 
{
    if (m_Link) delete m_Link;
    m_Link = NULL;
    if (link.GetHref() != wxEmptyString)
        m_Link = new wxHtmlLinkInfo(link);
}



void wxHtmlCell::Layout(int w) 
{
    SetPos(0, 0); 
    if (m_Next) m_Next->Layout(w);
}


void wxHtmlCell::Draw(wxDC& dc, int x, int y, int view_y1, int view_y2) 
{
    if (m_Next) m_Next->Draw(dc, x, y, view_y1, view_y2);
}



void wxHtmlCell::DrawInvisible(wxDC& dc, int x, int y) 
{
    if (m_Next) m_Next->DrawInvisible(dc, x, y);
}



const wxHtmlCell* wxHtmlCell::Find(int condition, const void* param) const 
{
    if (m_Next) return m_Next->Find(condition, param); 
    else return NULL;
}



//-----------------------------------------------------------------------------
// wxHtmlWordCell
//-----------------------------------------------------------------------------

wxHtmlWordCell::wxHtmlWordCell(const wxString& word, wxDC& dc) : wxHtmlCell()
{
    m_Word = word;
    
    if (m_Word.Find(wxT('&')) != -1) 
    {
#define ESCSEQ(escape, subst)  \
                  { _T("&") _T(escape) _T(";"), _T("&") _T(escape) _T(" "), _T("&") _T(escape), _T(subst) }
        static wxChar* substitutions[][4] =
                {
                ESCSEQ("quot", "\""),
                ESCSEQ("#34", "\""),
                ESCSEQ("#8220", "\""),
                ESCSEQ("#8221", "\""),
                ESCSEQ("lt", "<"),
                ESCSEQ("#60", "<"),
                ESCSEQ("gt", ">"),
                ESCSEQ("#62", ">"),

                ESCSEQ("#94", "^"), /* ^ */
    
                ESCSEQ("nbsp", " "),
                ESCSEQ("#32", " "),
                ESCSEQ("iexcl", "!"),
                ESCSEQ("#33", "!"),
                ESCSEQ("cent", "�"/* � */),
                ESCSEQ("#162", "�"/* � */),
    
                ESCSEQ("trade", "(TM)"),
                ESCSEQ("#153", "(TM)"),
                ESCSEQ("#8482", "(TM)"),

                ESCSEQ("yen", "�"),
                ESCSEQ("#165", "�"),
                ESCSEQ("brkbar", "�"),
                ESCSEQ("#166", "�"),
                ESCSEQ("sect", "�"),
                ESCSEQ("#167", "�"),
                ESCSEQ("uml", "�"),
                ESCSEQ("#168", "�"),
    
                ESCSEQ("copy", "�"), /* � */
                ESCSEQ("#169", "�"),
                ESCSEQ("ordf", "�"),
                ESCSEQ("#170", "�"),
                ESCSEQ("laquo", "�"), /* � */
                ESCSEQ("#171", "�"),
                ESCSEQ("not", "�"),
                ESCSEQ("#172", "�"),
    
                ESCSEQ("reg", "�"), /* � */
                ESCSEQ("#174", "�"),
    
                ESCSEQ("deg", "�"), /* � */
                ESCSEQ("#176", "�"),
                ESCSEQ("plusm", "�"), /* � */
                ESCSEQ("#177", "�"),
    
                ESCSEQ("acute", "�"),
                ESCSEQ("#180", "�"),
                ESCSEQ("macron", "�"),
                ESCSEQ("#175", "�"),
                ESCSEQ("micro", "�"), /* � */
                ESCSEQ("#181", "�"),
                ESCSEQ("para", "�"), /* � */
                ESCSEQ("#182", "�"),
    
                ESCSEQ("ordm", "�"), /* � */
                ESCSEQ("#186", "�"),
                ESCSEQ("raquo", "�"), /* � */
                ESCSEQ("#187", "�"),
    
                ESCSEQ("iquest", "�"), /* � */
                ESCSEQ("#191", "�"),
                ESCSEQ("Agrave", "\300"/* � */),
                ESCSEQ("#193", "\300"/* � */),
    
                ESCSEQ("Acirc", "\302"/* � */),
                ESCSEQ("Atilde", "\303"/* � */),
                ESCSEQ("Auml", "\304"/* � */),
                ESCSEQ("Aring", " "),
                ESCSEQ("AElig", " "),
                ESCSEQ("Ccedil", "\347"/* � */),
                ESCSEQ("Egrave", "\310"/* � */),
                ESCSEQ("Eacute", "\311"/* � */),
                ESCSEQ("Ecirc", "\312"/* � */),
                ESCSEQ("Euml", "\313"/* � */),
                ESCSEQ("Igrave", "\314"/* � */),

                ESCSEQ("Icirc", "\316"/* � */),
                ESCSEQ("Iuml", "\317"/* � */),
    
                ESCSEQ("Ntilde", "\321"/* � */),
                ESCSEQ("Ograve", "\322"/* � */),
    
                ESCSEQ("Ocirc", "\324"/* � */),
                ESCSEQ("Otilde", "\325"/* � */),
                ESCSEQ("Ouml", "\326"/* � */),
    
                ESCSEQ("Oslash", " "),
                ESCSEQ("Ugrave", "\331"/* � */),
    
                ESCSEQ("Ucirc", " "),
                ESCSEQ("Uuml", "\334"/* � */),
    
                ESCSEQ("szlig", "\247"/* � */),
                ESCSEQ("agrave","\340"/* � */),
                ESCSEQ("aacute", "\341"/* � */),
                ESCSEQ("acirc", "\342"/* � */),
                ESCSEQ("atilde", "\343"/* � */),
                ESCSEQ("auml", "\344"/* � */),
                ESCSEQ("aring", "a"),
                ESCSEQ("aelig", "ae"),
                ESCSEQ("ccedil", "\347"/* � */),
                ESCSEQ("egrave", "\350"/* � */),
                ESCSEQ("eacute", "\351"/* � */),
                ESCSEQ("ecirc", "\352"/* � */),
                ESCSEQ("euml", "\353"/* � */),
                ESCSEQ("igrave", "\354"/* � */),
                ESCSEQ("iacute", "\355"/* � */),
                ESCSEQ("icirc", " "),
                ESCSEQ("iuml", "\357"/* � */),
                ESCSEQ("eth", " "),
                ESCSEQ("ntilde", "\361"/* � */),
                ESCSEQ("ograve", "\362"/* � */),
                ESCSEQ("oacute", "\363"/* � */),
                ESCSEQ("ocirc", "\364"/* � */),
                ESCSEQ("otilde", "\365"/* � */),
                ESCSEQ("ouml", "\366"/* � */),
                ESCSEQ("divide", " "),
                ESCSEQ("oslash", " "),
                ESCSEQ("ugrave", "\371"/* � */),
                ESCSEQ("uacute", "\372"/* � */),
                ESCSEQ("ucirc", "\373"/* � */),
                ESCSEQ("uuml", "\374"/* � */),
    
                ESCSEQ("yuml", ""),

                /* this one should ALWAYS stay the last one!!! */
                ESCSEQ("amp", "&"),
                ESCSEQ("#38", "&"),

                { NULL, NULL, NULL }
                };

        for (int i = 0; substitutions[i][0] != NULL; i++) 
        {
            m_Word.Replace(substitutions[i][0], substitutions[i][3], TRUE);
            m_Word.Replace(substitutions[i][1], substitutions[i][3], TRUE);
            m_Word.Replace(substitutions[i][2], substitutions[i][3], TRUE);
        }
    }

    dc.GetTextExtent(m_Word, &m_Width, &m_Height, &m_Descent);
    SetCanLiveOnPagebreak(FALSE);
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
    if (m_Parent) m_Parent->InsertCell(this);
    m_AlignHor = wxHTML_ALIGN_LEFT;
    m_AlignVer = wxHTML_ALIGN_BOTTOM;
    m_IndentLeft = m_IndentRight = m_IndentTop = m_IndentBottom = 0;
    m_WidthFloat = 100; m_WidthFloatUnits = wxHTML_UNITS_PERCENT;
    m_UseBkColour = FALSE;
    m_UseBorder = FALSE;
    m_MinHeight = 0;
    m_MinHeightAlign = wxHTML_ALIGN_TOP;
    m_LastLayout = -1;
}

wxHtmlContainerCell::~wxHtmlContainerCell() 
{
    if (m_Cells) delete m_Cells;
}



void wxHtmlContainerCell::SetIndent(int i, int what, int units)
{
    int val = (units == wxHTML_UNITS_PIXELS) ? i : -i;
    if (what & wxHTML_INDENT_LEFT) m_IndentLeft = val;
    if (what & wxHTML_INDENT_RIGHT) m_IndentRight = val;
    if (what & wxHTML_INDENT_TOP) m_IndentTop = val;
    if (what & wxHTML_INDENT_BOTTOM) m_IndentBottom = val;
    m_LastLayout = -1;
}



int wxHtmlContainerCell::GetIndent(int ind) const
{
    if (ind & wxHTML_INDENT_LEFT) return m_IndentLeft;
    else if (ind & wxHTML_INDENT_RIGHT) return m_IndentRight;
    else if (ind & wxHTML_INDENT_TOP) return m_IndentTop;
    else if (ind & wxHTML_INDENT_BOTTOM) return m_IndentBottom;
    else return -1; /* BUG! Should not be called... */
}




int wxHtmlContainerCell::GetIndentUnits(int ind) const
{
    bool p = FALSE;
    if (ind & wxHTML_INDENT_LEFT) p = m_IndentLeft < 0;
    else if (ind & wxHTML_INDENT_RIGHT) p = m_IndentRight < 0;
    else if (ind & wxHTML_INDENT_TOP) p = m_IndentTop < 0;
    else if (ind & wxHTML_INDENT_BOTTOM) p = m_IndentBottom < 0;
    if (p) return wxHTML_UNITS_PERCENT;
    else return wxHTML_UNITS_PIXELS;
}



bool wxHtmlContainerCell::AdjustPagebreak(int *pagebreak) const
{
    if (!m_CanLiveOnPagebreak) 
        return wxHtmlCell::AdjustPagebreak(pagebreak);

    else 
    {
        wxHtmlCell *c = GetFirstCell();
        bool rt = FALSE;
        int pbrk = *pagebreak - m_PosY;

        while (c) 
	    {
            if (c->AdjustPagebreak(&pbrk)) rt = TRUE;
            c = c->GetNext();
        }
        if (rt) *pagebreak = pbrk + m_PosY;
        return rt;
    }
}



void wxHtmlContainerCell::Layout(int w)
{
    if (m_LastLayout == w) 
    {
        wxHtmlCell::Layout(w);
        return;
    } 

    wxHtmlCell *cell = m_Cells, *line = m_Cells;
    long xpos = 0, ypos = m_IndentTop;
    int xdelta = 0, ybasicpos = 0, ydiff;
    int s_width, s_indent;
    int ysizeup = 0, ysizedown = 0;
    int MaxLineWidth = 0;
    int xcnt = 0;


    /*

    WIDTH ADJUSTING :

    */

    if (m_WidthFloatUnits == wxHTML_UNITS_PERCENT) 
    {
        if (m_WidthFloat < 0) m_Width = (100 + m_WidthFloat) * w / 100;
        else m_Width = m_WidthFloat * w / 100;
    }
    else 
    {
        if (m_WidthFloat < 0) m_Width = w + m_WidthFloat;
        else m_Width = m_WidthFloat;
    }

    if (m_Cells) 
    {
        int l = (m_IndentLeft < 0) ? (-m_IndentLeft * m_Width / 100) : m_IndentLeft;
        int r = (m_IndentRight < 0) ? (-m_IndentRight * m_Width / 100) : m_IndentRight;
        m_Cells->Layout(m_Width - (l + r));
    }

    /*

    LAYOUTING :

    */

    // adjust indentation:
    s_indent = (m_IndentLeft < 0) ? (-m_IndentLeft * m_Width / 100) : m_IndentLeft;
    s_width = m_Width - s_indent - ((m_IndentRight < 0) ? (-m_IndentRight * m_Width / 100) : m_IndentRight);

    // my own layouting:
    while (cell != NULL) 
    {
        switch (m_AlignVer) 
	    {
            case wxHTML_ALIGN_TOP :      ybasicpos = 0; break;
            case wxHTML_ALIGN_BOTTOM :   ybasicpos = - cell->GetHeight(); break;
            case wxHTML_ALIGN_CENTER :   ybasicpos = - cell->GetHeight() / 2; break;
        }
        ydiff = cell->GetHeight() + ybasicpos;

        if (cell->GetDescent() + ydiff > ysizedown) ysizedown = cell->GetDescent() + ydiff;
        if (ybasicpos + cell->GetDescent() < -ysizeup) ysizeup = - (ybasicpos + cell->GetDescent());

        cell->SetPos(xpos, ybasicpos + cell->GetDescent());
        xpos += cell->GetWidth();
        cell = cell->GetNext();
        xcnt++;

        // force new line if occured:
        if ((cell == NULL) || (xpos + cell->GetWidth() > s_width)) 
	    {
            if (xpos > MaxLineWidth) MaxLineWidth = xpos;
            if (ysizeup < 0) ysizeup = 0;
            if (ysizedown < 0) ysizedown = 0;
            switch (m_AlignHor) {
                case wxHTML_ALIGN_LEFT : 
                case wxHTML_ALIGN_JUSTIFY : 
                         xdelta = 0; 
                         break;
                case wxHTML_ALIGN_RIGHT : 
                         xdelta = 0 + (s_width - xpos); 
                         break;
                case wxHTML_ALIGN_CENTER : 
                         xdelta = 0 + (s_width - xpos) / 2; 
                         break;
            }
            if (xdelta < 0) xdelta = 0;
            xdelta += s_indent;

            ypos += ysizeup;
            
            if (m_AlignHor != wxHTML_ALIGN_JUSTIFY || cell == NULL)
                while (line != cell) 
		        {
                    line->SetPos(line->GetPosX() + xdelta, 
                                   ypos + line->GetPosY());
                    line = line->GetNext();
                }
            else
            { 
                int counter = 0;
                int step = (s_width - xpos);
                if (step < 0) step = 0;
                xcnt--;
                if (xcnt > 0) while (line != cell) 
		        {
                    line->SetPos(line->GetPosX() + s_indent +
                                   (counter++ * step / xcnt),
                                   ypos + line->GetPosY());
                    line = line->GetNext();
                }
                xcnt++;
            }

            ypos += ysizedown;
            xpos = xcnt = 0;
            ysizeup = ysizedown = 0;
            line = cell;
        }
    }

    // setup height & width, depending on container layout:
    m_Height = ypos + (ysizedown + ysizeup) + m_IndentBottom;

    if (m_Height < m_MinHeight) 
    {
        if (m_MinHeightAlign != wxHTML_ALIGN_TOP) 
	    {
            int diff = m_MinHeight - m_Height;
            if (m_MinHeightAlign == wxHTML_ALIGN_CENTER) diff /= 2;
            cell = m_Cells;
            while (cell) 
	        {
                cell->SetPos(cell->GetPosX(), cell->GetPosY() + diff);
                cell = cell->GetNext();
            }
        }
        m_Height = m_MinHeight;
    }

    MaxLineWidth += s_indent + ((m_IndentRight < 0) ? (-m_IndentRight * m_Width / 100) : m_IndentRight);
    if (m_Width < MaxLineWidth) m_Width = MaxLineWidth;

    m_LastLayout = w;

    wxHtmlCell::Layout(w);
}


#define mMin(a, b) (((a) < (b)) ? (a) : (b))
#define mMax(a, b) (((a) < (b)) ? (b) : (a))

void wxHtmlContainerCell::Draw(wxDC& dc, int x, int y, int view_y1, int view_y2)
{
    // container visible, draw it:
    if ((y + m_PosY < view_y2) && (y + m_PosY + m_Height > view_y1)) 
    {

        if (m_UseBkColour) 
	    {
            wxBrush myb = wxBrush(m_BkColour, wxSOLID);

            int real_y1 = mMax(y + m_PosY, view_y1);
            int real_y2 = mMin(y + m_PosY + m_Height - 1, view_y2);

            dc.SetBrush(myb);
            dc.SetPen(*wxTRANSPARENT_PEN);
            dc.DrawRectangle(x + m_PosX, real_y1, m_Width, real_y2 - real_y1 + 1);
        }

        if (m_UseBorder) 
	    {
            wxPen mypen1(m_BorderColour1, 1, wxSOLID);
            wxPen mypen2(m_BorderColour2, 1, wxSOLID);

            dc.SetPen(mypen1);
            dc.DrawLine(x + m_PosX, y + m_PosY, x + m_PosX, y + m_PosY + m_Height - 1);
            dc.DrawLine(x + m_PosX, y + m_PosY, x + m_PosX + m_Width - 1, y + m_PosY);
            dc.SetPen(mypen2);
            dc.DrawLine(x + m_PosX + m_Width - 1, y + m_PosY, x + m_PosX +  m_Width - 1, y + m_PosY + m_Height - 1);
            dc.DrawLine(x + m_PosX, y + m_PosY + m_Height - 1, x + m_PosX + m_Width - 1, y + m_PosY + m_Height - 1);
        }

        if (m_Cells) m_Cells->Draw(dc, x + m_PosX, y + m_PosY, view_y1, view_y2);
    }
    // container invisible, just proceed font+color changing:
    else 
    {
        if (m_Cells) m_Cells->DrawInvisible(dc, x + m_PosX, y + m_PosY);
    }

    wxHtmlCell::Draw(dc, x, y, view_y1, view_y2);
}



void wxHtmlContainerCell::DrawInvisible(wxDC& dc, int x, int y)
{
    if (m_Cells) m_Cells->DrawInvisible(dc, x + m_PosX, y + m_PosY);
    wxHtmlCell::DrawInvisible(dc, x, y);
}



wxHtmlLinkInfo *wxHtmlContainerCell::GetLink(int x, int y) const
{
    wxHtmlCell *c = m_Cells;
    int cx, cy, cw, ch;

    while (c) 
    {
        cx = c->GetPosX(), cy = c->GetPosY();
        cw = c->GetWidth(), ch = c->GetHeight();
        if ((x >= cx) && (x < cx + cw) && (y >= cy) && (y < cy + ch))
            return c->GetLink(x - cx, y - cy);
        c = c->GetNext();
    }
    return NULL;
}



void wxHtmlContainerCell::InsertCell(wxHtmlCell *f)
{
    if (!m_Cells) m_Cells = m_LastCell = f;
    else 
    {
        m_LastCell->SetNext(f);
        m_LastCell = f;
        if (m_LastCell) while (m_LastCell->GetNext()) m_LastCell = m_LastCell->GetNext();
    }
    f->SetParent(this);
    m_LastLayout = -1;
}



void wxHtmlContainerCell::SetAlign(const wxHtmlTag& tag)
{
    if (tag.HasParam(wxT("ALIGN"))) 
    {
        wxString alg = tag.GetParam(wxT("ALIGN"));
        alg.MakeUpper();
        if (alg == wxT("CENTER"))
            SetAlignHor(wxHTML_ALIGN_CENTER);
        else if (alg == wxT("LEFT"))
            SetAlignHor(wxHTML_ALIGN_LEFT);
        else if (alg == wxT("JUSTIFY"))
            SetAlignHor(wxHTML_ALIGN_JUSTIFY);
        else if (alg == wxT("RIGHT"))
            SetAlignHor(wxHTML_ALIGN_RIGHT);
        m_LastLayout = -1;
    }
}



void wxHtmlContainerCell::SetWidthFloat(const wxHtmlTag& tag, double pixel_scale)
{
    if (tag.HasParam(wxT("WIDTH"))) 
    {
        int wdi;
        wxString wd = tag.GetParam(wxT("WIDTH"));

        if (wd[wd.Length()-1] == wxT('%')) 
	    {
            wxSscanf(wd.c_str(), wxT("%i%%"), &wdi);
            SetWidthFloat(wdi, wxHTML_UNITS_PERCENT);
        }
        else 
	    {
            wxSscanf(wd.c_str(), wxT("%i"), &wdi);
            SetWidthFloat((int)(pixel_scale * (double)wdi), wxHTML_UNITS_PIXELS);
        }
        m_LastLayout = -1;
    }
}



const wxHtmlCell* wxHtmlContainerCell::Find(int condition, const void* param) const
{
    const wxHtmlCell *r = NULL;

    if (m_Cells) 
    {
        r = m_Cells->Find(condition, param);
        if (r) return r;
    }

    return wxHtmlCell::Find(condition, param);
}



void wxHtmlContainerCell::OnMouseClick(wxWindow *parent, int x, int y, const wxMouseEvent& event)
{
    if (m_Cells) 
    {
        wxHtmlCell *c = m_Cells;
        while (c) 
	    {
            if (    (c->GetPosX() <= x) &&
                    (c->GetPosY() <= y) &&
                    (c->GetPosX() + c->GetWidth() > x) &&
                    (c->GetPosY() + c->GetHeight() > y)) 
		    {
                c->OnMouseClick(parent, x - c->GetPosX(), y - c->GetPosY(), event);
                break;
            }
            c = c->GetNext();
        }
    }
}





//--------------------------------------------------------------------------------
// wxHtmlColourCell
//--------------------------------------------------------------------------------

void wxHtmlColourCell::Draw(wxDC& dc, int x, int y, int view_y1, int view_y2)
{
    if (m_Flags & wxHTML_CLR_FOREGROUND)
        dc.SetTextForeground(m_Colour);
    if (m_Flags & wxHTML_CLR_BACKGROUND) 
    {
        dc.SetBackground(wxBrush(m_Colour, wxSOLID));
        dc.SetTextBackground(m_Colour);
    }
    wxHtmlCell::Draw(dc, x, y, view_y1, view_y2);
}

void wxHtmlColourCell::DrawInvisible(wxDC& dc, int x, int y)
{
    if (m_Flags & wxHTML_CLR_FOREGROUND)
        dc.SetTextForeground(m_Colour);
    if (m_Flags & wxHTML_CLR_BACKGROUND) 
    {
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
    dc.SetFont(m_Font);
    wxHtmlCell::Draw(dc, x, y, view_y1, view_y2);
}

void wxHtmlFontCell::DrawInvisible(wxDC& dc, int x, int y)
{
    dc.SetFont(m_Font);
    wxHtmlCell::DrawInvisible(dc, x, y);
}








//--------------------------------------------------------------------------------
// wxHtmlWidgetCell
//--------------------------------------------------------------------------------

wxHtmlWidgetCell::wxHtmlWidgetCell(wxWindow *wnd, int w)
{
    int sx, sy;
    m_Wnd = wnd;
    m_Wnd->GetSize(&sx, &sy);
    m_Width = sx, m_Height = sy;
    m_WidthFloat = w;
}


void wxHtmlWidgetCell::Draw(wxDC& dc, int x, int y, int view_y1, int view_y2)
{
    int absx = 0, absy = 0, stx, sty;
    wxHtmlCell *c = this;

    while (c) 
    {
        absx += c->GetPosX();
        absy += c->GetPosY();
        c = c->GetParent();
    }

    ((wxScrolledWindow*)(m_Wnd->GetParent()))->ViewStart(&stx, &sty);
    m_Wnd->SetSize(absx - wxHTML_SCROLL_STEP * stx, absy  - wxHTML_SCROLL_STEP * sty, m_Width, m_Height);

    wxHtmlCell::Draw(dc, x, y, view_y1, view_y2);
}



void wxHtmlWidgetCell::DrawInvisible(wxDC& dc, int x, int y)
{
    int absx = 0, absy = 0, stx, sty;
    wxHtmlCell *c = this;

    while (c) 
    {
        absx += c->GetPosX();
        absy += c->GetPosY();
        c = c->GetParent();
    }

    ((wxScrolledWindow*)(m_Wnd->GetParent()))->ViewStart(&stx, &sty);
    m_Wnd->SetSize(absx - wxHTML_SCROLL_STEP * stx, absy  - wxHTML_SCROLL_STEP * sty, m_Width, m_Height);

    wxHtmlCell::DrawInvisible(dc, x, y);
}



void wxHtmlWidgetCell::Layout(int w)
{
    if (m_WidthFloat != 0) 
    {
        m_Width = (w * m_WidthFloat) / 100;
        m_Wnd->SetSize(m_Width, m_Height);
    }

    wxHtmlCell::Layout(w);
}

#endif
