
import  wx

#---------------------------------------------------------------------------

class TestSashWindow(wx.Panel):
    ID_WINDOW_TOP    = 5100
    ID_WINDOW_LEFT1  = 5101
    ID_WINDOW_LEFT2  = 5102
    ID_WINDOW_BOTTOM = 5103


    def __init__(self, parent, log):
        wx.Panel.__init__(self, parent, -1)

        self.log = log

        # will occupy the space not used by the Layout Algorithm
        self.remainingSpace = wx.Panel(self, -1, style=wx.SUNKEN_BORDER)

        self.Bind(
            wx.EVT_SASH_DRAGGED_RANGE, self.OnSashDrag,
            id=self.ID_WINDOW_TOP, id2=self.ID_WINDOW_BOTTOM, 
            )

        self.Bind(wx.EVT_SIZE, self.OnSize)


        # Create some layout windows
        # A window like a toolbar
        win = wx.SashLayoutWindow(
            self, self.ID_WINDOW_TOP, wx.DefaultPosition, (200, 30), 
            wx.NO_BORDER|wx.SW_3D
            )

        win.SetDefaultSize((1000, 30))
        win.SetOrientation(wx.LAYOUT_HORIZONTAL)
        win.SetAlignment(wx.LAYOUT_TOP)
        win.SetBackgroundColour(wx.Colour(255, 0, 0))
        win.SetSashVisible(wx.SASH_BOTTOM, True)

        self.topWindow = win


        # A window like a statusbar
        win = wx.SashLayoutWindow(
                self, self.ID_WINDOW_BOTTOM, wx.DefaultPosition, (200, 30), 
                wx.NO_BORDER|wx.SW_3D
                )

        win.SetDefaultSize((1000, 30))
        win.SetOrientation(wx.LAYOUT_HORIZONTAL)
        win.SetAlignment(wx.LAYOUT_BOTTOM)
        win.SetBackgroundColour(wx.Colour(0, 0, 255))
        win.SetSashVisible(wx.SASH_TOP, True)

        self.bottomWindow = win

        # A window to the left of the client window
        win =  wx.SashLayoutWindow(
                self, self.ID_WINDOW_LEFT1, wx.DefaultPosition, (200, 30), 
                wx.NO_BORDER|wx.SW_3D
                )

        win.SetDefaultSize((120, 1000))
        win.SetOrientation(wx.LAYOUT_VERTICAL)
        win.SetAlignment(wx.LAYOUT_LEFT)
        win.SetBackgroundColour(wx.Colour(0, 255, 0))
        win.SetSashVisible(wx.SASH_RIGHT, True)
        win.SetExtraBorderSize(10)
        textWindow = wx.TextCtrl(
                        win, -1, "", wx.DefaultPosition, wx.DefaultSize, 
                        wx.TE_MULTILINE|wx.SUNKEN_BORDER
                        )

        textWindow.SetValue("A sub window")

        self.leftWindow1 = win


        # Another window to the left of the client window
        win = wx.SashLayoutWindow(
                self, self.ID_WINDOW_LEFT2, wx.DefaultPosition, (200, 30), 
                wx.NO_BORDER|wx.SW_3D
                )

        win.SetDefaultSize((120, 1000))
        win.SetOrientation(wx.LAYOUT_VERTICAL)
        win.SetAlignment(wx.LAYOUT_LEFT)
        win.SetBackgroundColour(wx.Colour(0, 255, 255))
        win.SetSashVisible(wx.SASH_RIGHT, True)

        self.leftWindow2 = win


    def OnSashDrag(self, event):
        if event.GetDragStatus() == wx.SASH_STATUS_OUT_OF_RANGE:
            return

        eID = event.GetId()

        if eID == self.ID_WINDOW_TOP:
            self.topWindow.SetDefaultSize((1000, event.GetDragRect().height))

        elif eID == self.ID_WINDOW_LEFT1:
            self.leftWindow1.SetDefaultSize((event.GetDragRect().width, 1000))


        elif eID == self.ID_WINDOW_LEFT2:
            self.leftWindow2.SetDefaultSize((event.GetDragRect().width, 1000))

        elif eID == self.ID_WINDOW_BOTTOM:
            self.bottomWindow.SetDefaultSize((1000, event.GetDragRect().height))

        wx.LayoutAlgorithm().LayoutWindow(self, self.remainingSpace)
        self.remainingSpace.Refresh()

    def OnSize(self, event):
        wx.LayoutAlgorithm().LayoutWindow(self, self.remainingSpace)

#---------------------------------------------------------------------------

def runTest(frame, nb, log):
    win = TestSashWindow(nb, log)
    return win

#---------------------------------------------------------------------------


overview = """\
wx.SashLayoutWindow responds to OnCalculateLayout events generated by 
wxLayoutAlgorithm. It allows the application to use simple accessors to 
specify how the window should be laid out, rather than having to respond 
to events. The fact that the class derives from wx.SashWindow allows sashes 
to be used if required, to allow the windows to be user-resizable.

The documentation for wx.LayoutAlgorithm explains the purpose of this class 
in more detail.

"""


if __name__ == '__main__':
    import sys,os
    import run
    run.main(['', os.path.basename(sys.argv[0])] + sys.argv[1:])
