
import  wx

from Main import opj

#----------------------------------------------------------------------

class TestPanel(wx.Panel):
    def __init__(self, parent, log):
        wx.Panel.__init__(self, parent, -1)
        self.log = log

        b = wx.Button(self, -1, "Play Sound 1 (sync)", (25, 25))
        self.Bind(wx.EVT_BUTTON, self.OnButton1, b)

        b = wx.Button(self, -1, "Play Sound 2 (async)", (25, 65))
        self.Bind(wx.EVT_BUTTON, self.OnButton2, b)

        b = wx.Button(self, -1, "Select .WAV file", (25, 105))
        self.Bind(wx.EVT_BUTTON, self.OnSelectSound, b)


    def OnButton1(self, evt):
        try:
            self.sound = wx.Sound(opj('data/anykey.wav'))
            self.log.write("before Play...\n")
            self.sound.Play(wx.SOUND_SYNC)
            self.log.write("...after Play\n")
        except NotImplementedError, v:
            wx.MessageBox(str(v), "Exception Message")


    def OnButton2(self, evt):
        try:
            if True:
                self.sound = wx.Sound(opj('data/plan.wav'))
            else:
                # sounds can also be loaded from a buffer object
                data = open(opj('data/plan.wav'), 'rb').read()
                self.sound = wx.SoundFromData(data)
                
            self.log.write("before Play...\n")
            self.sound.Play(wx.SOUND_ASYNC)
            wx.YieldIfNeeded()
            self.log.write("...after Play\n")
        except NotImplementedError, v:
            wx.MessageBox(str(v), "Exception Message")


    def OnSelectSound(self, evt):
        dlg = wx.FileDialog(wx.GetTopLevelParent(self),
                            "Choose a sound file",
                            wildcard="WAV files (*.wav)|*.wav",
                            style=wx.OPEN)
        if dlg.ShowModal() == wx.ID_OK:
            try:
                #self.sound = wx.Sound(dlg.GetPath())
                #self.sound.Play()
                wx.Sound.PlaySound(dlg.GetPath())
            except NotImplementedError, v:
                wx.MessageBox(str(v), "Exception Message")
        dlg.Destroy()


#----------------------------------------------------------------------

def runTest(frame, nb, log):
    win = TestPanel(nb, log)
    return win

#----------------------------------------------------------------------


overview = """<html><body>
<h2>Sound</h2>
This class represents a short wave file, in Windows WAV format, that can
be stored in memory and played. Currently this class is implemented on Windows
and GTK (Linux) only.
<p>
This demo offers two examples, both driven by buttons, but obviously the event
that drives the playing of the sound can come from anywhere.

</body></html>
"""


if __name__ == '__main__':
    import sys,os
    import run
    run.main(['', os.path.basename(sys.argv[0])] + sys.argv[1:])
