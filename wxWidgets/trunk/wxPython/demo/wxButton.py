
from wxPython.wx import *

import images

#----------------------------------------------------------------------

class TestPanel(wxPanel):
    def __init__(self, parent, log):
        wxPanel.__init__(self, parent, -1)
        self.log = log

        b = wxButton(self, 10, "Hello", wxPoint(20, 20))
        EVT_BUTTON(self, 10, self.OnClick)
        b.SetBackgroundColour(wxBLUE)
        b.SetForegroundColour(wxWHITE)
        b.SetDefault()

        b = wxButton(self, 20, "HELLO AGAIN!", wxPoint(20, 60), wxSize(120, 45))
        EVT_BUTTON(self, 20, self.OnClick)

        b.SetToolTipString("This is a Hello button...")

        bmp = images.getTest2Bitmap()
        mask = wxMaskColour(bmp, wxBLUE)
        bmp.SetMask(mask)

        wxBitmapButton(self, 30, bmp, wxPoint(160, 20),
                       wxSize(bmp.GetWidth()+10, bmp.GetHeight()+10))
        EVT_BUTTON(self, 30, self.OnClick)

    def OnClick(self, event):
        self.log.WriteText("Click! (%d)\n" % event.GetId())


#----------------------------------------------------------------------

def runTest(frame, nb, log):
    win = TestPanel(nb, log)
    return win

#----------------------------------------------------------------------










overview = """\
"""


