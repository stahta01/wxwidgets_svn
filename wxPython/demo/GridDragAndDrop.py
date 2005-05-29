"""
Example showing how to make a grid a drop target for files.

"""

from wxPython.wx import *
from wxPython.grid import *

#---------------------------------------------------------------------------
# Set VIRTUAL to 1 to use a virtual grid

VIRTUAL = 1

class GridFileDropTarget(wxFileDropTarget):
    def __init__(self, grid):
        wxFileDropTarget.__init__(self)
        self.grid = grid

    def OnDropFiles(self, x, y, filenames):
        # the x,y coordinates here are Unscrolled coordinates.  They must be changed
        # to scrolled coordinates.
        x, y = self.grid.CalcUnscrolledPosition(x, y)

        # now we need to get the row and column from the grid
        # but we need to first remove the RowLabel and ColumnLabel
        # bounding boxes
        # Why this isn't done for us, I'll never know...
        x = x - self.grid.GetGridRowLabelWindow().GetRect().width
        y = y - self.grid.GetGridColLabelWindow().GetRect().height
        col = self.grid.XToCol(x)
        row = self.grid.YToRow(y)

        if row > -1 and col > -1:
            self.grid.SetCellValue(row, col, filenames[0])
            self.grid.AutoSizeColumn(col)
            self.grid.Refresh()



class FooTable(wxPyGridTableBase):
    def __init__(self):
        wxPyGridTableBase.__init__(self)
        self.dropTargets = {(0,0):"Drag",
                            (1,0):"A",
                            (2,0):"File",
                            (3,0):"To",
                            (4,0):"A",
                            (5,0):"Cell"}
    def GetNumberCols(self):
        return 100
    def GetNumberRows(self):
        return 100
    def GetValue(self, row, col):
        return self.dropTargets.get((row, col), "")



class SimpleGrid(wxGrid):
    def __init__(self, parent, log):
        wxGrid.__init__(self, parent, -1)
        self.log = log
        self.moveTo = None
        if VIRTUAL:
            self.table = FooTable()
            self.SetTable(self.table)
        else:
            self.CreateGrid(25, 25)

        # set the drag and drop target
        dropTarget = GridFileDropTarget(self)
        self.SetDropTarget(dropTarget)
        self.EnableDragRowSize()
        self.EnableDragColSize()

    def SetCellValue(self, row, col, value):
        if VIRTUAL:
            self.table.dropTargets[row, col] = value
        else:
            wxGrid.SetCellValue(self, row, col, value)



class TestFrame(wxFrame):
    def __init__(self, parent, log):
        wxFrame.__init__(self, parent, -1, "DragAndDrop Grid", size=(640,480))
        grid = SimpleGrid(self, log)



#---------------------------------------------------------------------------

if __name__ == '__main__':
    import sys
    app = wxPySimpleApp()
    frame = TestFrame(None, sys.stdout)
    frame.Show(True)
    app.MainLoop()


#---------------------------------------------------------------------------




