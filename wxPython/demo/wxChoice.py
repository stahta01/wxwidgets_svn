
from wxPython.wx import *

#---------------------------------------------------------------------------

class TestChoice(wxPanel):
    def __init__(self, parent, log):
        self.log = log
        wxPanel.__init__(self, parent, -1)

        sampleList = ['zero', 'one', 'two', 'three', 'four', 'five',
                      'six', 'seven', 'eight']

        wxStaticText(self, -1, "This example uses the wxChoice control.",
                               wxPoint(15, 10))

        wxStaticText(self, -1, "Select one:", wxPoint(15, 50), wxSize(75, 20))
        wxChoice(self, 40, (80, 50), (95, 125),
                 choices = sampleList)
        EVT_CHOICE(self, 40, self.EvtChoice)

    def EvtChoice(self, event):
        self.log.WriteText('EvtChoice: %s\n' % event.GetString())

#---------------------------------------------------------------------------

def runTest(frame, nb, log):
    win = TestChoice(nb, log)
    return win

#---------------------------------------------------------------------------












overview = """\
A choice item is used to select one of a list of strings. Unlike a listbox, only the selection is visible until the user pulls down the menu of choices.
"""
