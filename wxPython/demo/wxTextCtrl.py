import sys
from wxPython.wx import *

#---------------------------------------------------------------------------

class TestPanel(wxPanel):
    def __init__(self, parent, log):
        wxPanel.__init__(self, parent, -1)
        self.log = log

        l1 = wxStaticText(self, -1, "wxTextCtrl")
        t1 = wxTextCtrl(self, 10, "Test it out and see", size=(125, -1))
        t1.SetInsertionPoint(0)
        EVT_TEXT(self, 10, self.EvtText)
        EVT_CHAR(t1, self.EvtChar)


        l2 = wxStaticText(self, -1, "Passsword")
        t2 = wxTextCtrl(self, 20, "", size=(125, -1), style=wxTE_PASSWORD)
        EVT_TEXT(self, 20, self.EvtText)

        l3 = wxStaticText(self, -1, "Multi-line")
        t3 = wxTextCtrl(self, 30, "How does it work with a long line of text set in the control",
                       size=(200, 100), style=wxTE_MULTILINE)
        t3.SetInsertionPoint(0)
        EVT_TEXT(self, 30, self.EvtText)

        l4 = wxStaticText(self, -1, "Rich Text")
        t4 = wxTextCtrl(self, 40, "If supported by the native control, this is red, and this is a different font.",
                        size=(200, 100), style=wxTE_MULTILINE|wxTE_RICH)
        t4.SetInsertionPoint(0)
        t4.SetStyle(44, 47, wxTextAttr("RED", "YELLOW"))

        points = t4.GetFont().GetPointSize()  # get the current size
        f = wxFont(points+2, wxROMAN, wxITALIC, wxBOLD, true)
##         print 'a1', sys.getrefcount(f)
##         t4.SetStyle(63, 77, wxTextAttr("BLUE", font=f))
        t4.SetStyle(63, 77, wxTextAttr("BLUE", wxNullColour, f))
##         print 'a2', sys.getrefcount(f)

        sizer = wxFlexGridSizer(cols=2, hgap=6, vgap=6)
        sizer.AddMany([ l1, t1,
                        l2, t2,
                        l3, t3,
                        l4, t4,
                        ])
        border = wxBoxSizer(wxVERTICAL)
        border.Add(sizer, 0, wxALL, 25)
        self.SetSizer(border)
        self.SetAutoLayout(true)


    def EvtText(self, event):
        self.log.WriteText('EvtText: %s\n' % event.GetString())


    def EvtChar(self, event):
        self.log.WriteText('EvtChar: %d\n' % event.GetKeyCode())
        event.Skip()


#---------------------------------------------------------------------------

def runTest(frame, nb, log):
    win = TestPanel(nb, log)
    return win

#---------------------------------------------------------------------------




overview = """\
"""
