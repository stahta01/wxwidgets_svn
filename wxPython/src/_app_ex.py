
#----------------------------------------------------------------------

class PyOnDemandOutputWindow:
    def __init__(self, title = "wxPython: stdout/stderr"):
        self.frame  = None
        self.title  = title
        self.parent = None

    def SetParent(self, parent):
        self.parent = parent

    def OnCloseWindow(self, event):
        if self.frame != None:
            self.frame.Destroy()
        self.frame = None
        self.text  = None

    # These methods provide the file-like output behaviour.
    def write(self, str):
        if not wx.Thread_IsMain():
            # Aquire the GUI mutex before making GUI calls.  Mutex is released
            # when locker is deleted at the end of this function.
            locker = wx.MutexGuiLocker()

        if not self.frame:
            self.frame = wx.Frame(self.parent, -1, self.title,
                                 style=wx.DEFAULT_FRAME_STYLE | wx.NO_FULL_REPAINT_ON_RESIZE)
            self.text  = wxTextCtrl(self.frame, -1, "",
                                    style = wx.TE_MULTILINE | wx.TE_READONLY)
            self.frame.SetSize((450, 300))
            self.frame.Show(True)
            EVT_CLOSE(self.frame, self.OnCloseWindow)
        self.text.AppendText(str)

    def close(self):
        if self.frame != None:
            if not wx.Thread_IsMain():
                locker = wx.MutexGuiLocker()
            self.frame.Close()


#----------------------------------------------------------------------
# The main application class.  Derive from this and implement an OnInit
# method that creates a frame and then calls self.SetTopWindow(frame)

_defRedirect = (wx.Platform == '__WXMSW__' or wx.Platform == '__WXMAC__')

class App(wx.PyApp):
    outputWindowClass = PyOnDemandOutputWindow

    def __init__(self, redirect=_defRedirect, filename=None, useBestVisual=False):
        wx.PyApp.__init__(self)

        if wx.Platform == "__WXMAC__":
            try:
                import MacOS
                if not MacOS.WMAvailable():
                    print """\
This program needs access to the screen. Please run with 'pythonw',
not 'python', and only when you are logged in on the main display of
your Mac."""
                    _sys.exit(1)
            except:
                pass

        # This has to be done before OnInit
        self.SetUseBestVisual(useBestVisual)

        # Save and redirect the stdio to a window?
        self.stdioWin = None
        self.saveStdio = (_sys.stdout, _sys.stderr)
        if redirect:
            self.RedirectStdio(filename)

        # Set the default handler for SIGINT.  This fixes a problem
        # where if Ctrl-C is pressed in the console that started this
        # app then it will not appear to do anything, (not even send
        # KeyboardInterrupt???)  but will later segfault on exit.  By
        # setting the default handler then the app will exit, as
        # expected (depending on platform.)
        try:
            import signal
            signal.signal(signal.SIGINT, signal.SIG_DFL)
        except:
            pass

        # This finishes the initialization of wxWindows and then calls
        # the OnInit that should be present in the derived class
        self._BootstrapApp()


    def __del__(self):
        try:
            self.RestoreStdio()  # Just in case the MainLoop was overridden
        except:
            pass


    def SetTopWindow(self, frame):
        if self.stdioWin:
            self.stdioWin.SetParent(frame)
        wx.PyApp.SetTopWindow(self, frame)


    def MainLoop(self):
        wx.PyApp.MainLoop(self)
        self.RestoreStdio()


    def RedirectStdio(self, filename):
        if filename:
            _sys.stdout = _sys.stderr = open(filename, 'a')
        else:
            self.stdioWin = self.outputWindowClass()
            _sys.stdout = _sys.stderr = self.stdioWin


    def RestoreStdio(self):
        _sys.stdout, _sys.stderr = self.saveStdio



# change from wxPyApp_ to wxApp_
App_GetMacDefaultEncodingIsPC    = _core.PyApp_GetMacDefaultEncodingIsPC
App_GetMacSupportPCMenuShortcuts = _core.PyApp_GetMacSupportPCMenuShortcuts
App_GetMacAboutMenuItemId        = _core.PyApp_GetMacAboutMenuItemId
App_GetMacPreferencesMenuItemId  = _core.PyApp_GetMacPreferencesMenuItemId
App_GetMacExitMenuItemId         = _core.PyApp_GetMacExitMenuItemId
App_GetMacHelpMenuTitleName      = _core.PyApp_GetMacHelpMenuTitleName
App_SetMacDefaultEncodingIsPC    = _core.PyApp_SetMacDefaultEncodingIsPC
App_SetMacSupportPCMenuShortcuts = _core.PyApp_SetMacSupportPCMenuShortcuts
App_SetMacAboutMenuItemId        = _core.PyApp_SetMacAboutMenuItemId
App_SetMacPreferencesMenuItemId  = _core.PyApp_SetMacPreferencesMenuItemId
App_SetMacExitMenuItemId         = _core.PyApp_SetMacExitMenuItemId
App_SetMacHelpMenuTitleName      = _core.PyApp_SetMacHelpMenuTitleName

if wx.Platform == '__WXMSW__':
    App_GetComCtl32Version       = _core.PyApp_GetComCtl32Version

#----------------------------------------------------------------------------

class PySimpleApp(wx.App):
    def __init__(self, redirect=False, filename=None):
        wx.App.__init__(self, redirect, filename)
    def OnInit(self):
        wx.InitAllImageHandlers()
        return True


class PyWidgetTester(wx.App):
    def __init__(self, size = (250, 100)):
        self.size = size
        wx.App.__init__(self, 0)

    def OnInit(self):
        self.frame = wxFrame(None, -1, "Widget Tester", pos=(0,0), size=self.size)
        self.SetTopWindow(self.frame)
        return True

    def SetWidget(self, widgetClass, *args):
        w = widgetClass(self.frame, *args)
        self.frame.Show(True)

#----------------------------------------------------------------------------
# DO NOT hold any other references to this object.  This is how we
# know when to cleanup system resources that wxWin is holding.  When
# the sys module is unloaded, the refcount on sys.__wxPythonCleanup
# goes to zero and it calls the wxApp_CleanUp function.

class __wxPyCleanup:
    def __init__(self):
        self.cleanup = _core.App_CleanUp
    def __del__(self):
        self.cleanup()

_sys.__wxPythonCleanup = __wxPyCleanup()

## # another possible solution, but it gets called too early...
## if sys.version[0] == '2':
##     import atexit
##     atexit.register(_core.wxApp_CleanUp)
## else:
##     sys.exitfunc = _core.wxApp_CleanUp


#----------------------------------------------------------------------------
