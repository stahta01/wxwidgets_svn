
import wx                  # This module uses the new wx namespace

#----------------------------------------------------------------------
gbsDescription = """\
The wxGridBagSizer is similar to the wxFlexGridSizer except the items are explicitly positioned
in a virtual cell of the layout grid, and column or row spanning is allowed.  For example, this
static text is positioned at (0,0) and it spans 7 columns.
"""


class TestFrame(wx.Frame):
    def __init__(self):
        wx.Frame.__init__(self, None, -1, "wxGridBagSizer")
        p = wx.Panel(self, -1)
        gbs = self.gbs = wx.GridBagSizer()

        gbs.Add( wx.StaticText(p, -1, gbsDescription),
                 (0,0), (1,7), wx.ALIGN_CENTER | wx.ALL, 5)


        gbs.Add( wx.TextCtrl(p, -1, "pos(1,0)"), (1,0) )
        gbs.Add( wx.TextCtrl(p, -1, "pos(1,1)"), (1,1) )
        gbs.Add( wx.TextCtrl(p, -1, "pos(2,0)"), (2,0) )
        gbs.Add( wx.TextCtrl(p, -1, "pos(2,1)"), (2,1) )
        
        gbs.Add( wx.TextCtrl(p, -1, "pos(3,2), span(1,2)\nthis row and col are growable", style=wx.TE_MULTILINE),
                 (3,2), (1,2), flag=wx.EXPAND )
        
        gbs.Add( wx.TextCtrl(p, -1, "pos(4,3), span(3,1)", style=wx.TE_MULTILINE),
                 (4,3), (3,1), wx.EXPAND)
        
        gbs.Add( wx.TextCtrl(p, -1, "pos(5,4)"), (5,4), flag=wx.EXPAND )
        gbs.Add( wx.TextCtrl(p, -1, "pos(6,5)"), (6,5), flag=wx.EXPAND )
        gbs.Add( wx.TextCtrl(p, -1, "pos(7,6)"), (7,6) )
        
        moveBtn1 = wx.Button(p, -1, "Move this to (3,6)")
        moveBtn2 = wx.Button(p, -1, "Move this to (3,6)");
        gbs.Add( moveBtn1, (10,2) )
        gbs.Add( moveBtn2, (10,3) )
    
        hideBtn = wx.Button(p, -1, "Hide this item -->")
        gbs.Add(hideBtn, (12, 3))

        hideTxt = wx.TextCtrl(p, -1, "pos(12,4), size(150, -1)", size = (150,-1))
        gbs.Add( hideTxt, (12,4) )
    
        showBtn = wx.Button(p, -1, "<-- Show it again")
        gbs.Add(showBtn, (12, 5))
        showBtn.Disable()
        self.hideBtn = hideBtn
        self.showBtn = showBtn
        self.hideTxt = hideTxt

        self.Bind(wx.EVT_BUTTON, self.OnHideButton, hideBtn)
        self.Bind(wx.EVT_BUTTON, self.OnShowButton, showBtn)
        self.Bind(wx.EVT_BUTTON, self.OnMoveButton, moveBtn1)
        self.Bind(wx.EVT_BUTTON, self.OnMoveButton, moveBtn2)
        
        # Add a spacer at the end to ensure some extra space at the bottom
        gbs.Add((10,10), (14,0))
  
        gbs.AddGrowableRow(3)
        gbs.AddGrowableCol(2)

        p.SetSizerAndFit(gbs)
        self.SetClientSize(p.GetSize())


    def OnHideButton(self, evt):
        self.gbs.Hide(self.hideTxt)
        self.hideBtn.Disable()
        self.showBtn.Enable()
        self.gbs.Layout()

    
    def OnShowButton(self, evt):
        self.gbs.Show(self.hideTxt)
        self.hideBtn.Enable()
        self.showBtn.Disable()
        self.gbs.Layout()

    
    def OnMoveButton(self, evt):
        btn = evt.GetEventObject()
        curPos = self.gbs.GetItemPosition(btn)

        # if it's already at the "other" spot then move it back
        if curPos == (3,6):
            self.gbs.SetItemPosition(btn, self.lastPos)
            btn.SetLabel("Move this to (3,6)")
        else:
            if self.gbs.CheckForIntersection( (3,6), (1,1) ):
                wx.MessageBox("""\
wxGridBagSizer will not allow items to be in the same cell as
another item, so this operation will fail.  You will also get an assert
when compiled in debug mode.""",
                              "Warning", wx.OK | wx.ICON_INFORMATION)

            try:
                if self.gbs.SetItemPosition(btn, (3,6)):
                    self.lastPos = curPos
                    btn.SetLabel("Move it back")
            except wx.PyAssertionError:
                pass
        
        self.gbs.Layout()
       
        
#----------------------------------------------------------------------

def runTest(frame, nb, log):
    win = TestFrame()
    frame.otherWin = win
    win.Show(True)


#----------------------------------------------------------------------



overview = """<html><body>
<h2><center>wxGridBagSizer</center></h2>

The wxGridBagSizer is more or less a port of the the RowColSizer (that
has been in the wxPython.lib package for quite some time) to C++.  It
allows items to be placed at specific layout grid cells, and items can
span across more than one row or column.
</body></html>
"""



if __name__ == '__main__':
    import sys,os
    import run
    run.main(['', os.path.basename(sys.argv[0])])

