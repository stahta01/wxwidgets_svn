/////////////////////////////////////////////////////////////////////////////
// Name:        mod_tables.cpp
// Purpose:     wxHtml module for tables
// Author:      Vaclav Slavik
// RCS-ID:      $Id$
// Copyright:   (c) 1999 Vaclav Slavik
// Licence:     wxWindows Licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation
#endif

#include <wx/wxprec.h>

#if wxUSE_HTML
#ifdef __BORDLANDC__
#pragma hdrstop
#endif

#ifndef WXPRECOMP
#include <wx/wx.h>
#endif


/*
REMARKS:
    1. This version of mod_tables doesn't support auto-layout algorithm.
       This means that all columns are of same width unless explicitly specified.
*/


#include "wx/html/forcelnk.h"
#include "wx/html/m_templ.h"

#include "wx/html/htmlcell.h"

FORCE_LINK_ME(mod_tables)


#define TABLE_BORDER_CLR_1  wxColour(0xC5, 0xC2, 0xC5)
#define TABLE_BORDER_CLR_2  wxColour(0x62, 0x61, 0x62)


//-----------------------------------------------------------------------------
// wxHtmlTableCell
//-----------------------------------------------------------------------------


typedef struct {
        int width, units;                      // universal
        int leftpos, pixwidth, maxrealwidth;   // temporary (depends on width of table)
    } colStruct;

typedef enum {
        cellSpan,
        cellUsed,
        cellFree
    } cellState;

typedef struct {
        wxHtmlContainerCell *cont;
        int colspan, rowspan;
        int minheight, valign;
        cellState flag;
    } cellStruct;


class wxHtmlTableCell : public wxHtmlContainerCell
{
    protected:
        /* These are real attributes: */
        bool m_HasBorders;
                // should we draw borders or not?
        int m_NumCols, m_NumRows;
                // number of columns; rows
        colStruct *m_ColsInfo;
                // array of column information
        cellStruct **m_CellInfo;
                // 2D array of all cells in the table : m_CellInfo[row][column]
        int m_Spacing;
                // spaces between cells
        int m_Padding;
                // cells internal indentation

    private:
        /* ...and these are valid only during parsing of table: */
        int m_ActualCol, m_ActualRow;
                // number of actual column (ranging from 0..m_NumCols)

        // default values (for table and row):
        int m_tBkg, m_rBkg;
        wxString m_tValign, m_rValign;


    public:
        wxHtmlTableCell(wxHtmlContainerCell *parent, const wxHtmlTag& tag);
        ~wxHtmlTableCell();
        virtual void Layout(int w);

        void AddRow(const wxHtmlTag& tag);
        void AddCell(wxHtmlContainerCell *cell, const wxHtmlTag& tag);
    private:
        void ReallocCols(int cols);
        void ReallocRows(int rows);
                // reallocates memory to given number of cols/rows
                // and changes m_NumCols/m_NumRows value to reflect this change
                // NOTE! You CAN'T change m_NumCols/m_NumRows before calling this!!
};



wxHtmlTableCell::wxHtmlTableCell(wxHtmlContainerCell *parent, const wxHtmlTag& tag)
 : wxHtmlContainerCell(parent)
{
    m_HasBorders = (tag.HasParam("BORDER") && tag.GetParam("BORDER") != "0");
    m_ColsInfo = NULL;
    m_NumCols = m_NumRows = 0;
    m_CellInfo = NULL;
    m_ActualCol = m_ActualRow = -1;

    /* scan params: */
    m_tBkg = m_rBkg = -1;
    if (tag.HasParam("BGCOLOR")) tag.ScanParam("BGCOLOR", "#%lX", &m_tBkg);
    if (tag.HasParam("VALIGN")) m_tValign = tag.GetParam("VALIGN"); else m_tValign = wxEmptyString;
    if (tag.HasParam("CELLSPACING") && tag.ScanParam("CELLSPACING", "%i", &m_Spacing) == 1) {} else m_Spacing = 2;
    if (tag.HasParam("CELLPADDING") && tag.ScanParam("CELLPADDING", "%i", &m_Padding) == 1) {} else m_Padding = 3;

    if (m_HasBorders)
        SetBorder(TABLE_BORDER_CLR_1, TABLE_BORDER_CLR_2);
}



wxHtmlTableCell::~wxHtmlTableCell()
{
    if (m_ColsInfo) free(m_ColsInfo);
    if (m_CellInfo) {
        for (int i = 0; i < m_NumRows; i++)
            free(m_CellInfo[i]);
        free(m_CellInfo);
    }
}



void wxHtmlTableCell::ReallocCols(int cols)
{
    int i,j;

    for (i = 0; i < m_NumRows; i++) {
        m_CellInfo[i] = (cellStruct*) realloc(m_CellInfo[i], sizeof(cellStruct) * cols);
        for (j = m_NumCols; j < cols; j++)
            m_CellInfo[i][j].flag = cellFree;
    }

    m_ColsInfo = (colStruct*) realloc(m_ColsInfo, sizeof(colStruct) * cols);
    for (j = m_NumCols; j < cols; j++) {
           m_ColsInfo[j].width = 0;
           m_ColsInfo[j].units = HTML_UNITS_PERCENT;
    }

    m_NumCols = cols;
}



void wxHtmlTableCell::ReallocRows(int rows)
{
    m_CellInfo = (cellStruct**) realloc(m_CellInfo, sizeof(cellStruct*) * rows);
    if (m_NumCols != 0) {
        int x = rows - 1;
        m_CellInfo[x] = (cellStruct*) malloc(sizeof(cellStruct) * m_NumCols);
        for (int i = 0; i < m_NumCols; i++)
            m_CellInfo[x][i].flag = cellFree;
    }
    else
        m_CellInfo[rows - 1] = NULL;
    m_NumRows = rows;
}



void wxHtmlTableCell::AddRow(const wxHtmlTag& tag)
{
    if (m_ActualRow + 1 > m_NumRows - 1)
        ReallocRows(m_ActualRow + 2);
    m_ActualRow++;
    m_ActualCol = -1;

    /* scan params: */
    m_rBkg = m_tBkg;
    if (tag.HasParam("BGCOLOR")) tag.ScanParam("BGCOLOR", "#%lX", &m_rBkg);
    if (tag.HasParam("VALIGN")) m_rValign = tag.GetParam("VALIGN"); else m_rValign = m_tValign;
}



void wxHtmlTableCell::AddCell(wxHtmlContainerCell *cell, const wxHtmlTag& tag)
{
    do {
        m_ActualCol++;
    } while ((m_ActualCol < m_NumCols) && (m_CellInfo[m_ActualRow][m_ActualCol].flag != cellFree));
    if (m_ActualCol > m_NumCols - 1)
        ReallocCols(m_ActualCol + 1);

    int r = m_ActualRow, c = m_ActualCol;

    m_CellInfo[r][c].cont = cell;
    m_CellInfo[r][c].colspan = 1;
    m_CellInfo[r][c].rowspan = 1;
    m_CellInfo[r][c].flag = cellUsed;
    m_CellInfo[r][c].minheight = 0;
    m_CellInfo[r][c].valign = HTML_ALIGN_TOP;

    /* scan for parameters: */

    // width:
    {
        if (tag.HasParam("WIDTH")) {
            wxString wd = tag.GetParam("WIDTH");

            if (wd[wd.Length()-1] == '%') {
                sscanf(wd.c_str(), "%i%%", &m_ColsInfo[c].width);
                m_ColsInfo[c].units = HTML_UNITS_PERCENT;
            }
            else {
                sscanf(wd.c_str(), "%i", &m_ColsInfo[c].width);
                m_ColsInfo[c].units = HTML_UNITS_PIXELS;
            }
        }
    }


    // spanning:
    {
        if (tag.HasParam("COLSPAN")) tag.ScanParam("COLSPAN", "%i", &m_CellInfo[r][c].colspan);
        if (tag.HasParam("ROWSPAN")) tag.ScanParam("ROWSPAN", "%i", &m_CellInfo[r][c].rowspan);
        if ((m_CellInfo[r][c].colspan != 1) || (m_CellInfo[r][c].rowspan != 1)) {
            int i, j;

            if (r + m_CellInfo[r][c].rowspan > m_NumRows) ReallocRows(r + m_CellInfo[r][c].rowspan);
            if (c + m_CellInfo[r][c].colspan > m_NumCols) ReallocCols(c + m_CellInfo[r][c].colspan);
            for (i = r; i < r + m_CellInfo[r][c].rowspan; i++)
                for (j = c; j < c + m_CellInfo[r][c].colspan; j++)
                    m_CellInfo[i][j].flag = cellSpan;
            m_CellInfo[r][c].flag = cellUsed;
        }
    }

    //background color:
    {
        int bk = m_rBkg;
        if (tag.HasParam("BGCOLOR")) tag.ScanParam("BGCOLOR", "#%lX", &bk);
        if (bk != -1) {
            wxColour clr = wxColour((bk & 0xFF0000) >> 16 , (bk & 0x00FF00) >> 8, (bk & 0x0000FF));
            cell -> SetBackgroundColour(clr);
        }
    }
    if (m_HasBorders)
        cell -> SetBorder(TABLE_BORDER_CLR_2, TABLE_BORDER_CLR_1);

    // vertical alignment:
    {
        wxString valign;
        if (tag.HasParam("VALIGN")) valign = tag.GetParam("VALIGN"); else valign = m_tValign;
        valign.MakeUpper();
        if (valign == "TOP") m_CellInfo[r][c].valign = HTML_ALIGN_TOP;
        else if (valign == "BOTTOM") m_CellInfo[r][c].valign = HTML_ALIGN_BOTTOM;
        else m_CellInfo[r][c].valign = HTML_ALIGN_CENTER;
    }

    cell -> SetIndent(m_Padding, HTML_INDENT_ALL, HTML_UNITS_PIXELS);
}





void wxHtmlTableCell::Layout(int w)
{
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


    /*

    LAYOUTING :

    */

    /* 1.  setup columns widths: */
    {
        int wpix = m_Width - (m_NumCols + 1) * m_Spacing;
        int i, j;
        int wtemp = 0;

        // 1a. setup fixed-width columns:
        for (i = 0; i < m_NumCols; i++)
            if (m_ColsInfo[i].units == HTML_UNITS_PIXELS)
                wpix -= (m_ColsInfo[i].pixwidth = m_ColsInfo[i].width);

        // 1b. setup floating-width columns:
        for (i = 0; i < m_NumCols; i++)
            if ((m_ColsInfo[i].units == HTML_UNITS_PERCENT) && (m_ColsInfo[i].width != 0))
                wtemp += (m_ColsInfo[i].pixwidth = m_ColsInfo[i].width * wpix / 100);
        wpix -= wtemp;

        // 1c. setup defalut columns (no width specification supplied):
        // NOTE! This algorithm doesn't conform to HTML standard : it assigns equal widths
        // instead of optimal
        for (i = j = 0; i < m_NumCols; i++)
            if (m_ColsInfo[i].width == 0) j++;
        for (i = 0; i < m_NumCols; i++)
            if (m_ColsInfo[i].width == 0)
                m_ColsInfo[i].pixwidth = wpix / j;
    }

    /* 2.  compute positions of columns: */
    {
        int wpos = m_Spacing;
        for (int i = 0; i < m_NumCols; i++) {
            m_ColsInfo[i].leftpos = wpos;
            wpos += m_ColsInfo[i].pixwidth + m_Spacing;
        }
    }

    /* 3.  sub-layout all cells: */
    {
        int *ypos = new int[m_NumRows + 1];

        int actcol, actrow;
        int fullwid;
        wxHtmlContainerCell *actcell;

        for (actrow = 0; actrow <= m_NumRows; actrow++) ypos[actrow] = m_Spacing;

        for (actrow = 0; actrow < m_NumRows; actrow++) {

            // 3a. sub-layout and detect max height:

            for (actcol = 0; actcol < m_NumCols; actcol++) {
                if (m_CellInfo[actrow][actcol].flag != cellUsed) continue;
                actcell = m_CellInfo[actrow][actcol].cont;
                fullwid = 0;
                for (int i = actcol; i < m_CellInfo[actrow][actcol].colspan + actcol; i++)
                    fullwid += m_ColsInfo[i].pixwidth;
                actcell -> SetMinHeight(m_CellInfo[actrow][actcol].minheight, m_CellInfo[actrow][actcol].valign);
                actcell -> Layout(fullwid);

                if (ypos[actrow] + actcell -> GetHeight() + m_CellInfo[actrow][actcol].rowspan * m_Spacing > ypos[actrow + m_CellInfo[actrow][actcol].rowspan])
                    ypos[actrow + m_CellInfo[actrow][actcol].rowspan] =
                            ypos[actrow] + actcell -> GetHeight() + m_CellInfo[actrow][actcol].rowspan * m_Spacing;
            }
        }


        for (actrow = 0; actrow < m_NumRows; actrow++) {

            // 3b. place cells in row & let'em all have same height:

            for (actcol = 0; actcol < m_NumCols; actcol++) {
                if (m_CellInfo[actrow][actcol].flag != cellUsed) continue;
                actcell = m_CellInfo[actrow][actcol].cont;
                actcell -> SetMinHeight(
                                 ypos[actrow + m_CellInfo[actrow][actcol].rowspan] - ypos[actrow] - m_CellInfo[actrow][actcol].rowspan * m_Spacing,
                                 m_CellInfo[actrow][actcol].valign);
                fullwid = 0;
                for (int i = actcol; i < m_CellInfo[actrow][actcol].colspan + actcol; i++)
                    fullwid += m_ColsInfo[i].pixwidth;
                actcell -> Layout(fullwid);
                actcell -> SetPos(m_ColsInfo[actcol].leftpos, ypos[actrow]);
            }

        }
        m_Height = ypos[m_NumRows];
        delete[] ypos;
    }
}






//-----------------------------------------------------------------------------
// The tables handler:
//-----------------------------------------------------------------------------


TAG_HANDLER_BEGIN(TABLE, "TABLE,TR,TD,TH")

    TAG_HANDLER_VARS
        wxHtmlTableCell* m_Table;
        wxString m_tAlign, m_rAlign;
        int m_OldAlign;

    TAG_HANDLER_CONSTR(TABLE)
    {
        m_Table = NULL;
    m_tAlign = m_rAlign = wxEmptyString;
    m_OldAlign = HTML_ALIGN_LEFT;
    }


    TAG_HANDLER_PROC(tag)
    {
        wxHtmlContainerCell *c;

        // new table started, backup upper-level table (if any) and create new:
        if (tag.GetName() == "TABLE") {
            wxHtmlTableCell *oldt = m_Table;
            wxHtmlContainerCell *oldcont;
            int m_OldAlign;

            oldcont = c = m_WParser -> OpenContainer();

            c -> SetWidthFloat(tag);
            m_Table = new wxHtmlTableCell(c, tag);
            m_OldAlign = m_WParser -> GetAlign();
            m_tAlign = wxEmptyString;
            if (tag.HasParam("ALIGN")) m_tAlign = tag.GetParam("ALIGN");

            ParseInner(tag);

            m_WParser -> SetAlign(m_OldAlign);
            m_WParser -> SetContainer(oldcont);
            m_WParser -> CloseContainer();
            m_Table = oldt;
            return TRUE;
        }


        else if (m_Table && !tag.IsEnding()) {
            // new row in table
            if (tag.GetName() == "TR") {
                m_Table -> AddRow(tag);
                m_rAlign = m_tAlign;
                if (tag.HasParam("ALIGN")) m_rAlign = tag.GetParam("ALIGN");
            }

            // new cell
            else {
                m_WParser -> SetAlign(m_OldAlign);
                c = m_WParser -> SetContainer(new wxHtmlContainerCell(m_Table));
                m_Table -> AddCell(c, tag);

                m_WParser -> OpenContainer();

                if (tag.GetName() == "TH") /*header style*/ {
                    m_WParser -> SetAlign(HTML_ALIGN_CENTER);
                }

                {
                    wxString als;

                    als = m_rAlign;
                    if (tag.HasParam("ALIGN")) als = tag.GetParam("ALIGN");
                    als.MakeUpper();
                    if (als == "RIGHT") m_WParser -> SetAlign(HTML_ALIGN_RIGHT);
                    else if (als == "CENTER") m_WParser -> SetAlign(HTML_ALIGN_CENTER);
                }
                m_WParser -> OpenContainer();
            }
        }
        return FALSE;
    }

TAG_HANDLER_END(TABLE)





TAGS_MODULE_BEGIN(Tables)

    TAGS_MODULE_ADD(TABLE)

TAGS_MODULE_END(Tables)


#endif
