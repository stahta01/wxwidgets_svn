# 11/21/2003 - Jeff Grimmett (grimmtooth@softhome.net)
#
# o Updated for wx namespace
# 

import  wx

#----------------------------------------------------------------------

class TestPanel(wx.Panel):
    def __init__(self, parent, log):
        wx.Panel.__init__(self, parent, -1)
        self.log = log
        self.count = 0

        wx.StaticText(self, -1, "This example uses the wxSpinCtrl control.", (45, 15))
        sc = wx.SpinCtrl(self, -1, "", (30, 50), (80, -1))
        sc.SetRange(1,100)
        sc.SetValue(5)
        #sc.Enable(False)


#----------------------------------------------------------------------

def runTest(frame, nb, log):
    win = TestPanel(nb, log)
    return win

#----------------------------------------------------------------------


overview = """\
wxSpinCtrl combines wxTextCtrl and wxSpinButton in one control.

Portable programs should try to use wxSpinCtrl instead as wxSpinButton is not 
implemented for all platforms (Win32 and GTK only currently).

NB: the range supported by this control depends on the platform 
but is at least -0x8000 to 0x7fff. Under GTK and Win32 with sufficiently new version 
of comctrl32.dll (at least 4.71 is required, 5.80 is recommended) the full 32 bit 
range is supported.


"""


if __name__ == '__main__':
    import sys,os
    import run
    run.main(['', os.path.basename(sys.argv[0])])
