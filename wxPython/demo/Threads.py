
from wxPython.wx import *

import thread
import time
from   whrandom import random

#----------------------------------------------------------------------

wxEVT_UPDATE_BARGRAPH = wxNewEventType()

def EVT_UPDATE_BARGRAPH(win, func):
    win.Connect(-1, -1, wxEVT_UPDATE_BARGRAPH, func)


class UpdateBarEvent(wxPyEvent):
    def __init__(self, barNum, value):
        wxPyEvent.__init__(self)
        self.SetEventType(wxEVT_UPDATE_BARGRAPH)
        self.barNum = barNum
        self.value = value


#----------------------------------------------------------------------

class CalcBarThread:
    def __init__(self, win, barNum, val):
        self.win = win
        self.barNum = barNum
        self.val = val

    def Start(self):
        self.keepGoing = self.running = True
        thread.start_new_thread(self.Run, ())

    def Stop(self):
        self.keepGoing = False

    def IsRunning(self):
        return self.running

    def Run(self):
        while self.keepGoing:
            evt = UpdateBarEvent(self.barNum, int(self.val))
            wxPostEvent(self.win, evt)
            #del evt

            sleeptime = (random() * 2) + 0.5
            time.sleep(sleeptime/4)

            sleeptime = sleeptime * 5
            if int(random() * 2):
                self.val = self.val + sleeptime
            else:
                self.val = self.val - sleeptime

            if self.val < 0: self.val = 0
            if self.val > 300: self.val = 300

        self.running = False

#----------------------------------------------------------------------


class GraphWindow(wxWindow):
    def __init__(self, parent, labels):
        wxWindow.__init__(self, parent, -1)

        self.values = []
        for label in labels:
            self.values.append((label, 0))

        font = wxFont(12, wxSWISS, wxNORMAL, wxBOLD)
        self.SetFont(font)

        self.colors = [ wxRED, wxGREEN, wxBLUE, wxCYAN,
                        "Yellow", "Navy" ]

        EVT_ERASE_BACKGROUND(self, self.OnEraseBackground)
        EVT_PAINT(self, self.OnPaint)


    def SetValue(self, index, value):
        assert index < len(self.values)
        cur = self.values[index]
        self.values[index:index+1] = [(cur[0], value)]


    def SetFont(self, font):
        wxWindow.SetFont(self, font)
        wmax = hmax = 0
        for label, val in self.values:
            w,h = self.GetTextExtent(label)
            if w > wmax: wmax = w
            if h > hmax: hmax = h
        self.linePos = wmax + 10
        self.barHeight = hmax


    def GetBestHeight(self):
        return 2 * (self.barHeight + 1) * len(self.values)


    def Draw(self, dc, size):
        dc.SetFont(self.GetFont())
        dc.SetTextForeground(wxBLUE)
        dc.SetBackground(wxBrush(self.GetBackgroundColour()))
        dc.Clear()
        dc.SetPen(wxPen(wxBLACK, 3, wxSOLID))
        dc.DrawLine(self.linePos, 0, self.linePos, size.height-10)

        bh = ypos = self.barHeight
        for x in range(len(self.values)):
            label, val = self.values[x]
            dc.DrawText(label, 5, ypos)

            if val:
                color = self.colors[ x % len(self.colors) ]
                dc.SetPen(wxPen(color))
                dc.SetBrush(wxBrush(color))
                dc.DrawRectangle(self.linePos+3, ypos, val, bh)

            ypos = ypos + 2*bh
            if ypos > size.height-10:
                break


    def OnPaint(self, evt):
        size = self.GetSize()
        bmp = wxEmptyBitmap(size.width, size.height)
        dc = wxMemoryDC()
        dc.SelectObject(bmp)
        self.Draw(dc, size)

        wdc = wxPaintDC(self)
        wdc.BeginDrawing()
        wdc.Blit(0,0, size.width, size.height, dc, 0,0)
        wdc.EndDrawing()

        dc.SelectObject(wxNullBitmap)


    def OnEraseBackground(self, evt):
        pass




#----------------------------------------------------------------------

class TestFrame(wxFrame):
    def __init__(self, parent, log):
        wxFrame.__init__(self, parent, -1, "Thread Test", size=(450,300))
        self.log = log

        #self.CenterOnParent()

        panel = wxPanel(self, -1)
        panel.SetFont(wxFont(10, wxSWISS, wxNORMAL, wxBOLD))
        wxStaticText(panel, -1,
                     "This demo shows multiple threads interacting with this\n"
                     "window by sending events to it, one thread for each bar.",
                     wxPoint(5,5))
        panel.Fit()

        self.graph = GraphWindow(self, ['Zero', 'One', 'Two', 'Three', 'Four',
                                        'Five', 'Six', 'Seven'])
        self.graph.SetSize((450, self.graph.GetBestHeight()))

        sizer = wxBoxSizer(wxVERTICAL)
        sizer.Add(panel, 0, wxEXPAND)
        sizer.Add(self.graph, 1, wxEXPAND)

        self.SetSizer(sizer)
        self.SetAutoLayout(True)
        sizer.Fit(self)

        EVT_UPDATE_BARGRAPH(self, self.OnUpdate)
        self.threads = []
        self.threads.append(CalcBarThread(self, 0, 50))
        self.threads.append(CalcBarThread(self, 1, 75))
        self.threads.append(CalcBarThread(self, 2, 100))
        self.threads.append(CalcBarThread(self, 3, 150))
        self.threads.append(CalcBarThread(self, 4, 225))
        self.threads.append(CalcBarThread(self, 5, 300))
        self.threads.append(CalcBarThread(self, 6, 250))
        self.threads.append(CalcBarThread(self, 7, 175))

        for t in self.threads:
            t.Start()

        EVT_CLOSE(self, self.OnCloseWindow)


    def OnUpdate(self, evt):
        self.graph.SetValue(evt.barNum, evt.value)
        self.graph.Refresh(False)


    def OnCloseWindow(self, evt):
        busy = wxBusyInfo("One moment please, waiting for threads to die...")
        wxYield()
        for t in self.threads:
            t.Stop()
        running = 1
        while running:
            running = 0
            for t in self.threads:
                running = running + t.IsRunning()
            time.sleep(0.1)
        self.Destroy()



#----------------------------------------------------------------------

def runTest(frame, nb, log):
    win = TestFrame(frame, log)
    frame.otherWin = win
    win.Show(True)
    return None

#----------------------------------------------------------------------




overview = """\
The main issue with multi-threaded GUI programming is the thread safty
of the GUI itself.  On most platforms the GUI is not thread safe and
so any cross platform GUI Toolkit and applications written with it
need to take that into account.

The solution is to only allow interaction with the GUI from a single
thread, but this often severly limits what can be done in an
application and makes it difficult to use additional threads at all.

Since wxPython already makes extensive use of event handlers, it is a
logical extension to allow events to be sent to GUI objects from
alternate threads.  A function called wxPostEvent allows you to do
this.  It accepts an event and an event handler (window) and instead
of sending the event immediately in the current context like
ProcessEvent does, it processes it later from the context of the GUI
thread.

"""



if __name__ == '__main__':
    import sys,os
    import run
    run.main(['', os.path.basename(sys.argv[0])])

