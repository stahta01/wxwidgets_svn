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


class Crust(wx.wxSplitterWindow):
    """PyCrust Crust based on wxSplitterWindow."""

    name = 'PyCrust Crust'
    revision = __revision__

    def __init__(self, parent, id=-1, pos=wx.wxDefaultPosition, 
                 size=wx.wxDefaultSize, style=wx.wxSP_3D, name='Crust Window', 
                 rootObject=None, rootLabel=None, rootIsNamespace=1, 
                 intro='', locals=None, 
                 InterpClass=None, *args, **kwds):
        """Create a PyCrust Crust instance."""
        wx.wxSplitterWindow.__init__(self, parent, id, pos, size, style, name)
        self.shell = Shell(parent=self, introText=intro, 
                           locals=locals, InterpClass=InterpClass, 
                           *args, **kwds)
        self.filling = Filling(parent=self, 
                               rootObject=self.shell.interp.locals, 
                               rootLabel=rootLabel, rootIsNamespace=1)
        """Add 'filling' to the interpreter's locals."""
        self.shell.interp.locals['filling'] = self.filling
        self.SplitHorizontally(self.shell, self.filling, 300)
        self.SetMinimumPaneSize(1)


class CrustFrame(wx.wxFrame, ShellMenu):
    """Frame containing all the PyCrust components."""

    name = 'PyCrust Frame'
    revision = __revision__

    def __init__(self, parent=None, id=-1, title='PyCrust', 
                 pos=wx.wxDefaultPosition, size=wx.wxDefaultSize, 
                 style=wx.wxDEFAULT_FRAME_STYLE, 
                 rootObject=None, rootLabel=None, rootIsNamespace=1, 
                 locals=None, InterpClass=None, *args, **kwds):
        """Create a PyCrust CrustFrame instance."""
        wx.wxFrame.__init__(self, parent, id, title, pos, size, style)
        intro = 'Welcome To PyCrust %s - The Flakiest Python Shell' % VERSION
        intro += '\nSponsored by Orbtech - Your source for Python programming expertise.'
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
        self.crust.filling.fillingTree.setStatusText = self.SetStatusText
        # Override the shell so that status messages go to the status bar.
        self.crust.shell.setStatusText = self.SetStatusText
        # Fix a problem with the sash shrinking to nothing.
        self.crust.filling.SetSashPosition(200)
        # Set focus to the shell editor.
        self.crust.shell.SetFocus()
        # Temporary hack to share menus between PyCrust and PyShell.
        self.shell = self.crust.shell
        self.createMenus()
        wx.EVT_CLOSE(self, self.OnCloseWindow)

    def OnCloseWindow(self, event):
        self.crust.shell.destroy()
        self.Destroy()


