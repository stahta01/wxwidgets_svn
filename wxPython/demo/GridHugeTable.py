from wxPython.wx import *
from wxPython.grid import *

#---------------------------------------------------------------------------

class HugeTable(wxPyGridTableBase):

    """
    This is all it takes to make a custom data table to plug into a
    wxGrid.  There are many more methods that can be overridden, but
    the ones shown below are the required ones.  This table simply
    provides strings containing the row and column values.
    """

    def __init__(self, log):
        wxPyGridTableBase.__init__(self)
        self.log = log

        self.odd=wxGridCellAttr()
        self.odd.SetBackgroundColour("sky blue")
        self.even=wxGridCellAttr()
        self.even.SetBackgroundColour("sea green")

    def GetAttr(self, row, col, kind):
        attr = [self.even, self.odd][row % 2]
        attr.IncRef()
        return attr

    def GetNumberRows(self):
        return 10000

    def GetNumberCols(self):
        return 10000

    def IsEmptyCell(self, row, col):
        return False

    def GetValue(self, row, col):
        return str( (row, col) )

    def SetValue(self, row, col, value):
        self.log.write('SetValue(%d, %d, "%s") ignored.\n' % (row, col, value))


#---------------------------------------------------------------------------



class HugeTableGrid(wxGrid):
    def __init__(self, parent, log):
        wxGrid.__init__(self, parent, -1)

        table = HugeTable(log)

        # The second parameter means that the grid is to take ownership of the
        # table and will destroy it when done.  Otherwise you would need to keep
        # a reference to it and call it's Destroy method later.
        self.SetTable(table, True)

        EVT_GRID_CELL_RIGHT_CLICK(self, self.OnRightDown)  #added

    def OnRightDown(self, event):                          #added
        print "hello"
        print self.GetSelectedRows()                       #added





#---------------------------------------------------------------------------

class TestFrame(wxFrame):
    def __init__(self, parent, log):
        wxFrame.__init__(self, parent, -1, "Huge (virtual) Table Demo", size=(640,480))
        grid = HugeTableGrid(self, log)

        grid.SetReadOnly(5,5, True)

#---------------------------------------------------------------------------

if __name__ == '__main__':
    import sys
    app = wxPySimpleApp()
    frame = TestFrame(None, sys.stdout)
    frame.Show(True)
    app.MainLoop()


#---------------------------------------------------------------------------
