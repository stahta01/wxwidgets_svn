# 11/21/2003 - Jeff Grimmett (grimmtooth@softhome.net)
#
# o Updated for wx namespace
# 
# 11/30/2003 - Jeff Grimmett (grimmtooth@softhome.net)
#
# o The rightalign library needs converted for this to work correctly.
# 
# 12/11/2003 - Jeff Grimmett (grimmtooth@softhome.net)
#
# o All issues resolved.
# 
# 12/20/2003 - Jeff Grimmett (grimmtooth@softhome.net)
#
# o wxRightTextCtrl -> RightTextCtrl
#

############################################################################\
# Note: this demo has been converted, but the control is deprecated because |
# wx.TextCtrl now supports the wx.TE_RIGHT style flag, which makes this     |
# control completely superfluous.                                           |
############################################################################/

import  wx
import  wx.lib.rightalign   as right

#----------------------------------------------------------------------

class TestPanel(wx.Panel):
    def __init__(self, parent):
        wx.Panel.__init__(self, parent, -1)

        fgs = wx.FlexGridSizer(cols=2, vgap=5, hgap=5)

        txt = wx.StaticText(
                self, -1,
                "These text controls will align their contents to\n"
                "the right (on wxMSW) when they don't have focus.", 
                style=wx.ALIGN_RIGHT 
                )

        fgs.Add(txt)
        fgs.Add(right.RightTextCtrl(self, -1, "", size=(75, -1)))

        fgs.Add((10,10))
        fgs.Add(right.RightTextCtrl(self, -1, "123.45", size=(75, -1)))

        fgs.Add((10,10))
        fgs.Add(right.RightTextCtrl(self, -1, "234.56", size=(75, -1)))

        fgs.Add((10,10))
        fgs.Add(right.RightTextCtrl(self, -1, "345.67", size=(75, -1)))

        fgs.Add((10,10))
        fgs.Add(right.RightTextCtrl(self, -1, "456.78", size=(75, -1)))

        sizer = wx.BoxSizer(wx.VERTICAL)
        sizer.Add(fgs, 0, wx.ALL, 25)

        self.SetSizer(sizer)
        self.SetAutoLayout(True)



#----------------------------------------------------------------------

def runTest(frame, nb, log):
    win = TestPanel(nb)
    return win

#----------------------------------------------------------------------

overview = right.__doc__


if __name__ == '__main__':
    import sys,os
    import run
    run.main(['', os.path.basename(sys.argv[0])])
