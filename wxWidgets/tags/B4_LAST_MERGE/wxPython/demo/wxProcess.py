
from wxPython.wx import *

#----------------------------------------------------------------------

class TestPanel(wxPanel):
    def __init__(self, parent, ID, log):
        wxPanel.__init__(self, parent, ID)
        self.log = log

        self.process = None
        EVT_IDLE(self, self.OnIdle)

        # We can either derive from wxProcess and override OnTerminate
        # or we can let wxProcess send this window an event that is
        # caught in the normal way...
        EVT_END_PROCESS(self, -1, self.OnProcessEnded)


        # Make the controls
        prompt = wxStaticText(self, -1, 'Command line:')
        self.cmd = wxTextCtrl(self, -1, 'python data/echo.py')
        self.exBtn = wxButton(self, -1, 'Execute')

        self.out = wxTextCtrl(self, -1, '', style=wxTE_MULTILINE|wxTE_READONLY)

        self.inp = wxTextCtrl(self, -1, '', style=wxTE_PROCESS_ENTER)
        self.sndBtn = wxButton(self, -1, 'Send')
        self.termBtn = wxButton(self, -1, 'Close Stream')
        self.inp.Enable(false)
        self.sndBtn.Enable(false)
        self.termBtn.Enable(false)

        # Hook up the events
        EVT_BUTTON(self, self.exBtn.GetId(), self.OnExecuteBtn)
        EVT_BUTTON(self, self.sndBtn.GetId(), self.OnSendText)
        EVT_BUTTON(self, self.termBtn.GetId(), self.OnCloseStream)
        EVT_TEXT_ENTER(self, self.inp.GetId(), self.OnSendText)


        # Do the layout
        box1 = wxBoxSizer(wxHORIZONTAL)
        box1.Add(prompt, 0, wxALIGN_CENTER)
        box1.Add(self.cmd, 1, wxALIGN_CENTER|wxLEFT|wxRIGHT, 5)
        box1.Add(self.exBtn, 0)

        box2 = wxBoxSizer(wxHORIZONTAL)
        box2.Add(self.inp, 1, wxALIGN_CENTER)
        box2.Add(self.sndBtn, 0, wxLEFT, 5)
        box2.Add(self.termBtn, 0, wxLEFT, 5)

        sizer = wxBoxSizer(wxVERTICAL)
        sizer.Add(box1, 0, wxEXPAND|wxALL, 10)
        sizer.Add(self.out, 1, wxEXPAND|wxALL, 10)
        sizer.Add(box2, 0, wxEXPAND|wxALL, 10)

        self.SetSizer(sizer)
        self.SetAutoLayout(true)


    def __del__(self):
        if self.process is not None:
            self.process.Detach()
            self.process.CloseOutput()
            self.process = None


    def OnExecuteBtn(self, evt):
        cmd = self.cmd.GetValue()

        self.process = wxProcess(self)
        self.process.Redirect();
        pid = wxExecute(cmd, false, self.process)
        self.log.write('OnExecuteBtn: "%s" pid: %s\n' % (cmd, pid))

        self.inp.Enable(true)
        self.sndBtn.Enable(true)
        self.termBtn.Enable(true)
        self.cmd.Enable(false)
        self.exBtn.Enable(false)
        self.inp.SetFocus()


    def OnSendText(self, evt):
        text = self.inp.GetValue()
        self.inp.SetValue('')
        self.log.write('OnSendText: "%s"\n' % text)
        self.process.GetOutputStream().write(text + '\n')
        self.inp.SetFocus()


    def OnCloseStream(self, evt):
        self.log.write('OnCloseStream\n')
        self.process.CloseOutput()


    def OnIdle(self, evt):
        if self.process is not None:
            stream = self.process.GetInputStream()

            # Yes, this is weird.  For this particular stream, EOF
            # simply means that there is no data available to be read,
            # not truly the end of file.  Also, read() just reads all
            # the currently available data, not until the real EOF...
            if not stream.eof():
                text = stream.read()
                self.out.AppendText(text)


    def OnProcessEnded(self, evt):
        self.log.write('OnProcessEnded, pid:%s,  exitCode: %s\n' %
                       (evt.GetPid(), evt.GetExitCode()))

        stream = self.process.GetInputStream()
        if not stream.eof():
            text = stream.read()
            self.out.AppendText(text)

        self.process.Destroy()
        self.process = None
        self.inp.Enable(false)
        self.sndBtn.Enable(false)
        self.termBtn.Enable(false)
        self.cmd.Enable(true)
        self.exBtn.Enable(true)


#----------------------------------------------------------------------

def runTest(frame, nb, log):
    win = TestPanel(nb, -1, log)
    return win


#----------------------------------------------------------------------


overview = """\
<html><body>
<h2>wxProcess</h2>

wxProcess lets you get notified when an asyncronous child process
started by wxExecute terminates, and also to get input/output streams
for the child process's stdout, stderr and stdin.

<p>
This demo launches a simple python script that echos back on stdout
lines that it reads from stdin.  You can send text to the echo
process' stdin by typing in the lower textctrl and clicking Send.

<p>
Clicking the Close Stream button will close the demo's end of the
stdin pipe to the child process.  In our case that will cause the
child process to exit its main loop.

</body><html>
"""
