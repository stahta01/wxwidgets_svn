#----------------------------------------------------------------------
# Name:        wxPython.lib.grids
# Purpose:     An example sizer derived from the C++ wxPySizer that
#              sizes items in a fixed or flexible grid.
#
# Author:      Robin Dunn
#
# Created:     21-Sept-1999
# RCS-ID:      $Id$
# Copyright:   (c) 1999 by Total Control Software
# Licence:     wxWindows license
#----------------------------------------------------------------------

"""
In this module you will find wxGridSizer and wxFlexGridSizer.  Please
note that these sizers have since been ported to C++ and those
versions are now exposed in the regular wxPython wrappers.  However I
am also leaving them here in the library so they can serve as an
example of how to implement sizers in Python.

wxGridSizer: Sizes and positions items such that all rows are the same
height and all columns are the same width.  You can specify a gap in
pixels to be used between the rows and/or the columns.  When you
create the sizer you specify the number of rows or the number of
columns and then as you add items it figures out the other dimension
automatically.  Like other sizers, items can be set to fill their
available space, or to be aligned on a side, in a corner, or in the
center of the space.  When the sizer is resized, all the items are
resized the same amount so all rows and all columns remain the same
size.

wxFlexGridSizer: Derives from wxGridSizer and adds the ability for
particular rows and/or columns to be marked as growable.  This means
that when the sizer changes size, the growable rows and colums are the
ones that stretch.  The others remain at their initial size.

See the demo for a couple examples for how to use them.
"""


from wxPython.wx import *

import operator

#----------------------------------------------------------------------

class wxGridSizer(wxPySizer):
    def __init__(self, rows=0, cols=0, hgap=0, vgap=0):
        wxPySizer.__init__(self)
        if rows == 0 and cols == 0:
            raise ValueError, "rows and cols cannot both be zero"

        self.rows = rows
        self.cols = cols
        self.hgap = hgap
        self.vgap = vgap


    def SetRows(self, rows):
        if rows == 0 and self.cols == 0:
            raise ValueError, "rows and cols cannot both be zero"
        self.rows = rows

    def SetColumns(self, cols):
        if self.rows == 0 and cols == 0:
            raise ValueError, "rows and cols cannot both be zero"
        self.cols = cols

    def GetRows(self):
        return self.rows

    def GetColumns(self):
        return self.cols

    def SetHgap(self, hgap):
        self.hgap = hgap

    def SetVgap(self, vgap):
        self.vgap = vgap

    def GetHgap(self, hgap):
        return self.hgap

    def GetVgap(self, vgap):
        return self.vgap

    #--------------------------------------------------
    def CalcMin(self):
        items = self.GetChildren()
        nitems = len(items)
        nrows = self.rows
        ncols = self.cols

        if ncols > 0:
            nrows = (nitems + ncols-1) / ncols
        else:
            ncols = (nitems + nrows-1) / nrows

        # Find the max width and height for any component.
        w = 0
        h = 0
        for item in items:
            size = item.CalcMin()
            w = max(w, size.width)
            h = max(h, size.height)

        return wxSize(ncols * w + (ncols-1) * self.hgap,
                      nrows * h + (nrows-1) * self.vgap)


    #--------------------------------------------------
    def RecalcSizes(self):
        items = self.GetChildren()
        if not items:
            return

        nitems = len(items)
        nrows = self.rows
        ncols = self.cols

        if ncols > 0:
            nrows = (nitems + ncols-1) / ncols
        else:
            ncols = (nitems + nrows-1) / nrows


        sz = self.GetSize()
        pt = self.GetPosition()
        w = (sz.width  - (ncols - 1) * self.hgap) / ncols;
        h = (sz.height - (nrows - 1) * self.vgap) / nrows;

        x = pt.x
        for c in range(ncols):
            y = pt.y
            for r in range(nrows):
                i = r * ncols + c
                if i < nitems:
                    self.SetItemBounds(items[i], x, y, w, h)
                y = y + h + self.vgap
            x = x + w + self.hgap


    #--------------------------------------------------
    def SetItemBounds(self, item, x, y, w, h):
        # calculate the item's size and position within
        # its grid cell
        ipt = wxPoint(x, y)
        isz = item.CalcMin()
        flag = item.GetFlag()

        if flag & wxEXPAND or flag & wxSHAPED:
            isz = wxSize(w, h)
        else:
            if flag & wxALIGN_CENTER_HORIZONTAL:
                ipt.x = x + (w - isz.width) / 2
            elif flag & wxALIGN_RIGHT:
                ipt.x = x + (w - isz.width)

            if flag & wxALIGN_CENTER_VERTICAL:
                ipt.y = y + (h - isz.height) / 2
            elif flag & wxALIGN_BOTTOM:
                ipt.y = y + (h - isz.height)

        item.SetDimension(ipt, isz)


#----------------------------------------------------------------------



class wxFlexGridSizer(wxGridSizer):
    def __init__(self, rows=0, cols=0, hgap=0, vgap=0):
        wxGridSizer.__init__(self, rows, cols, hgap, vgap)
        self.rowHeights = []
        self.colWidths  = []
        self.growableRows = []
        self.growableCols = []

    def AddGrowableRow(self, idx):
        self.growableRows.append(idx)

    def AddGrowableCol(self, idx):
        self.growableCols.append(idx)

    #--------------------------------------------------
    def CalcMin(self):
        items = self.GetChildren()
        nitems = len(items)
        nrows = self.rows
        ncols = self.cols

        if ncols > 0:
            nrows = (nitems + ncols-1) / ncols
        else:
            ncols = (nitems + nrows-1) / nrows

        # Find the max width and height for any component.
        self.rowHeights = [0] * nrows
        self.colWidths  = [0] * ncols
        for i in range(len(items)):
            size = items[i].CalcMin()
            row  = i / ncols
            col  = i % ncols
            self.rowHeights[row] = max(size.height, self.rowHeights[row])
            self.colWidths[col]  = max(size.width,  self.colWidths[col])

        # Add up all the widths and heights
        cellsWidth = reduce(operator.__add__, self.colWidths)
        cellHeight = reduce(operator.__add__, self.rowHeights)

        return wxSize(cellsWidth + (ncols-1) * self.hgap,
                      cellHeight + (nrows-1) * self.vgap)


    #--------------------------------------------------
    def RecalcSizes(self):
        items = self.GetChildren()
        if not items:
            return

        nitems = len(items)
        nrows = self.rows
        ncols = self.cols

        if ncols > 0:
            nrows = (nitems + ncols-1) / ncols
        else:
            ncols = (nitems + nrows-1) / nrows

        minsz = self.CalcMin()
        sz = self.GetSize()
        pt = self.GetPosition()

        # Check for growables
        if self.growableRows and sz.height > minsz.height:
            delta = (sz.height - minsz.height) / len(self.growableRows)
            for idx in self.growableRows:
                self.rowHeights[idx] = self.rowHeights[idx] + delta

        if self.growableCols and sz.width > minsz.width:
            delta = (sz.width - minsz.width) / len(self.growableCols)
            for idx in self.growableCols:
                self.colWidths[idx] = self.colWidths[idx] + delta

        # bottom right corner
        sz = wxSize(pt.x + sz.width, pt.y + sz.height)

        # Layout each cell
        x = pt.x
        for c in range(ncols):
            y = pt.y
            for r in range(nrows):
                i = r * ncols + c
                if i < nitems:
                    w = max(0, min(self.colWidths[c], sz.width - x))
                    h = max(0, min(self.rowHeights[r], sz.height - y))
                    self.SetItemBounds(items[i], x, y, w, h)
                y = y + self.rowHeights[r] + self.vgap
            x = x + self.colWidths[c] + self.hgap

#----------------------------------------------------------------------






