
""" Demonstrate filebrowsebutton module of the wxPython.lib Library.

14.1.2001 Bernhard Reiter <bernhard@intevation.de>
    Added demo for DirBrowseButton and improved overview text.
"""

import  wx
import  wx.lib.filebrowsebutton as filebrowse

#----------------------------------------------------------------------

class TestPanel(wx.Panel):
    def __init__(self, parent, ID, log):
        wx.Panel.__init__(self, parent, ID)
        self.log = log

        self.fbb = filebrowse.FileBrowseButton(
            self, -1, (20, 20), (450, -1), changeCallback = self.fbbCallback
            )

        self.fbbh = filebrowse.FileBrowseButtonWithHistory(
            self, -1, (20, 50), (450, -1),  changeCallback = self.fbbhCallback
            )

        self.dbb = filebrowse.DirBrowseButton(
            self, -1, (20, 80), (450, -1), changeCallback = self.dbbCallback
            )

        self.fbbh.SetHistory(['You', 'can', 'put', 'some', 'file', 'names', 'here'])


    def fbbCallback(self, evt):
        self.log.write('FileBrowseButton: %s\n' % evt.GetString())


    def fbbhCallback(self, evt):
        if hasattr(self, 'fbbh'):
            value = evt.GetString()
            self.log.write('FileBrowseButtonWithHistory: %s\n' % value)
            history = self.fbbh.GetHistory()
            history.append(value)
            self.fbbh.SetHistory(history)


    def dbbCallback(self, evt):
        self.log.write('DirBrowseButton: %s\n' % evt.GetString())


#----------------------------------------------------------------------

def runTest(frame, nb, log):
    win = TestPanel(nb, -1, log)
    return win


#----------------------------------------------------------------------

overview = """<html><body>
<h2>class FileBrowseButton:</h2>
<small><pre>
%s
</pre></small>

<h2>class FileBrowseButtonWithHistory(FileBrowseButton):</h2>
<small><pre>
%s
</pre></small>

<h2>class DirBrowseButton(FileBrowseButton):</h2>
<small><pre>
%s
</pre></small>

</body><</html>
""" % ( filebrowse.FileBrowseButton.__doc__,
        filebrowse.FileBrowseButtonWithHistory.__doc__ , 
        filebrowse.DirBrowseButton.__doc__
        ) 


if __name__ == '__main__':
    import sys,os
    import run
    run.main(['', os.path.basename(sys.argv[0])] + sys.argv[1:])

