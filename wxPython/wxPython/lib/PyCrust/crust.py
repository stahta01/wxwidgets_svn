"""PyCrust Crust combines the shell and filling into one control."""

__author__ = "Patrick K. O'Brien <pobrien@orbtech.com>"
__cvsid__ = "$Id$"
__revision__ = "$Revision$"[11:-2]

from wxPython import wx
from filling import Filling
import os
from shell import Shell
from shellmenu import ShellMenu
from version import VERSION

try:
    True
except NameError:
    True = 1==1
    False = 1==0


class Crust(wx.wxSplitterWindow):
    """PyCrust Crust based on wxSplitterWindow."""

    name = 'PyCrust Crust'
    revision = __revision__

    def __init__(self, parent, id=-1, pos=wx.wxDefaultPosition, 
                 size=wx.wxDefaultSize, style=wx.wxSP_3D,
                 name='Crust Window', rootObject=None, rootLabel=None,
                 rootIsNamespace=True, intro='', locals=None, 
                 InterpClass=None, *args, **kwds):
        """Create a PyCrust Crust instance."""
        wx.wxSplitterWindow.__init__(self, parent, id, pos, size, style, name)
        self.shell = Shell(parent=self, introText=intro, 
                           locals=locals, InterpClass=InterpClass, 
                           *args, **kwds)
        if rootObject is None:
            rootObject = self.shell.interp.locals
        self.notebook = wx.wxNotebook(parent=self, id=-1)
        self.shell.interp.locals['notebook'] = self.notebook
        self.filling = Filling(parent=self.notebook, 
                               rootObject=rootObject, 
                               rootLabel=rootLabel, 
                               rootIsNamespace=rootIsNamespace)
        # Add 'filling' to the interpreter's locals.
        self.shell.interp.locals['filling'] = self.filling
##        self.notebook.AddPage(pPage=self.filling, strText='Namespace', bSelect=True)
        self.notebook.AddPage(self.filling, 'Namespace', True)
        self.sessionlisting = SessionListing(parent=self.notebook)
        self.notebook.AddPage(self.sessionlisting, 'Session')
        self.dispatcherlisting = DispatcherListing(parent=self.notebook)
        self.notebook.AddPage(self.dispatcherlisting, 'Dispatcher')
        from decor import wxDecor
        self.wxdocs = Filling(parent=self.notebook, 
                              rootObject=wxDecor,
                              rootLabel='wx', 
                              rootIsNamespace=False,
                              static=True)
        self.notebook.AddPage(self.wxdocs, 'wxPython Docs')
        from decor import stcDecor
        self.stcdocs = Filling(parent=self.notebook, 
                               rootObject=stcDecor.wxStyledTextCtrl,
                               rootLabel='wxStyledTextCtrl', 
                               rootIsNamespace=False,
                               static=True)
        self.notebook.AddPage(self.stcdocs, 'wxSTC Docs')
        self.SplitHorizontally(self.shell, self.notebook, 300)
        self.SetMinimumPaneSize(1)


class SessionListing(wx.wxTextCtrl):
    """Text control containing all commands for session."""

    def __init__(self, parent=None, id=-1):
        import dispatcher
        style = wx.wxTE_MULTILINE | wx.wxTE_READONLY | \
                wx.wxTE_RICH2 | wx.wxTE_DONTWRAP
        wx.wxTextCtrl.__init__(self, parent=parent, id=id, style=style)
        dispatcher.connect(receiver=self.push, signal='Interpreter.push')

    def push(self, command, more):
        """Receiver for Interpreter.push signal."""
        if command and not more:
            self.SetInsertionPointEnd()
            start, end = self.GetSelection()
            if start != end:
                self.SetSelection(0, 0)
            self.AppendText(command + '\n')


class DispatcherListing(wx.wxTextCtrl):
    """Text control containing all dispatches for session."""

    def __init__(self, parent=None, id=-1):
        import dispatcher
        style = wx.wxTE_MULTILINE | wx.wxTE_READONLY | \
                wx.wxTE_RICH2 | wx.wxTE_DONTWRAP
        wx.wxTextCtrl.__init__(self, parent=parent, id=id, style=style)
        dispatcher.connect(receiver=self.spy)

    def spy(self, signal, sender):
        """Receiver for Any signal from Any sender."""
        text = '%r from %s' % (signal, sender)
        self.SetInsertionPointEnd()
        start, end = self.GetSelection()
        if start != end:
            self.SetSelection(0, 0)
        self.AppendText(text + '\n')


class CrustFrame(wx.wxFrame, ShellMenu):
    """Frame containing all the PyCrust components."""

    name = 'PyCrust Frame'
    revision = __revision__

    def __init__(self, parent=None, id=-1, title='PyCrust', 
                 pos=wx.wxDefaultPosition, size=wx.wxDefaultSize, 
                 style=wx.wxDEFAULT_FRAME_STYLE, 
                 rootObject=None, rootLabel=None, rootIsNamespace=True, 
                 locals=None, InterpClass=None, *args, **kwds):
        """Create a PyCrust CrustFrame instance."""
        wx.wxFrame.__init__(self, parent, id, title, pos, size, style)
        intro = 'PyCrust %s - The Flakiest Python Shell' % VERSION
        intro += '\nSponsored by Orbtech - '
        intro += 'Your source for Python programming expertise.'
        self.CreateStatusBar()
        self.SetStatusText(intro.replace('\n', ', '))
        import images
        self.SetIcon(images.getPyCrustIcon())
        self.crust = Crust(parent=self, intro=intro, 
                           rootObject=rootObject, 
                           rootLabel=rootLabel, 
                           rootIsNamespace=rootIsNamespace, 
                           locals=locals, 
                           InterpClass=InterpClass, *args, **kwds)
        # Override the filling so that status messages go to the status bar.
        self.crust.filling.tree.setStatusText = self.SetStatusText
        # Override the shell so that status messages go to the status bar.
        self.crust.shell.setStatusText = self.SetStatusText
        # Fix a problem with the sash shrinking to nothing.
        self.crust.filling.SetSashPosition(200)
        self.shell = self.crust.shell
        self.createMenus()
        wx.EVT_CLOSE(self, self.OnCloseWindow)
        # Set focus to the shell editor.
        self.crust.shell.SetFocus()

    def OnCloseWindow(self, event):
        self.crust.shell.destroy()
        self.Destroy()


