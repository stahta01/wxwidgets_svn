#----------------------------------------------------------------------------

# Use Python's bool constants if available, make some if not
try:
    True
except NameError:
    __builtins__.True = 1==1
    __builtins__.False = 1==0



# workarounds for bad wxRTTI names
__wxPyPtrTypeMap['wxGauge95']    = 'wxGauge'
__wxPyPtrTypeMap['wxSlider95']   = 'wxSlider'
__wxPyPtrTypeMap['wxStatusBar95']   = 'wxStatusBar'


#----------------------------------------------------------------------------
# Load version numbers from __version__...  Ensure that major and minor
# versions are the same for both wxPython and wxWindows.

from __version__ import *
__version__ = VERSION_STRING

assert MAJOR_VERSION == _core.MAJOR_VERSION, "wxPython/wxWindows version mismatch"
assert MINOR_VERSION == _core.MINOR_VERSION, "wxPython/wxWindows version mismatch"
if RELEASE_VERSION != _core.RELEASE_VERSION:
    import warnings
    warnings.warn("wxPython/wxWindows release number mismatch")

#----------------------------------------------------------------------------

class PyDeadObjectError(AttributeError):
    pass


class _wxPyDeadObject(object):
    """
    Instances of wx objects that are OOR capable will have their __class__
    changed to this class when the C++ object is deleted.  This should help
    prevent crashes due to referencing a bogus C++ pointer.
    """
    reprStr = "wxPython wrapper for DELETED %s object! (The C++ object no longer exists.)"
    attrStr = "The C++ part of the %s object has been deleted, attribute access no longer allowed."

    def __repr__( self ):
        if not hasattr(self, "_name"):
            self._name = "[unknown]"
        return self.reprStr % self._name

    def __getattr__( self, *args ):
        if not hasattr(self, "_name"):
            self._name = "[unknown]"
        raise PyDeadObjectError( self.attrStr % self._name )

    def __nonzero__(self):
        return 0


#----------------------------------------------------------------------------
_wxPyCallAfterId = None

def CallAfter(callable, *args, **kw):
    """
    Call the specified function after the current and pending event
    handlers have been completed.  This is also good for making GUI
    method calls from non-GUI threads.
    """
    app = wx.GetApp()
    assert app, 'No wxApp created yet'

    global _wxPyCallAfterId
    if _wxPyCallAfterId is None:
        _wxPyCallAfterId = wx.NewEventType()
        app.Connect(-1, -1, _wxPyCallAfterId,
              lambda event: event.callable(*event.args, **event.kw) )
    evt = wx.PyEvent()
    evt.SetEventType(_wxPyCallAfterId)
    evt.callable = callable
    evt.args = args
    evt.kw = kw
    wx.PostEvent(app, evt)


#----------------------------------------------------------------------------


class FutureCall:
    """
    A convenience class for wxTimer, that calls the given callable
    object once after the given amount of milliseconds, passing any
    positional or keyword args.  The return value of the callable is
    availbale after it has been run with the GetResult method.

    If you don't need to get the return value or restart the timer
    then there is no need to hold a reference to this object.  It will
    hold a reference to itself while the timer is running (the timer
    has a reference to self.Notify) but the cycle will be broken when
    the timer completes, automatically cleaning up the wx.FutureCall
    object.
    """
    def __init__(self, millis, callable, *args, **kwargs):
        self.millis = millis
        self.callable = callable
        self.SetArgs(*args, **kwargs)
        self.runCount = 0
        self.hasRun = False
        self.result = None
        self.timer = None
        self.Start()

    def __del__(self):
        self.Stop()


    def Start(self, millis=None, *args, **kwargs):
        """
        (Re)start the timer
        """
        self.hasRun = False
        if millis is not None:
            self.millis = millis
        if args or kwargs:
            self.SetArgs(*args, **kwargs)
        self.Stop()
        self.timer = wx.PyTimer(self.Notify)
        self.timer.Start(self.millis, wx.TIMER_ONE_SHOT)
    Restart = Start


    def Stop(self):
        """
        Stop and destroy the timer.
        """
        if self.timer is not None:
            self.timer.Stop()
            self.timer = None


    def GetInterval(self):
        if self.timer is not None:
            return self.timer.GetInterval()
        else:
            return 0


    def IsRunning(self):
        return self.timer is not None and self.timer.IsRunning()


    def SetArgs(self, *args, **kwargs):
        """
        (Re)set the args passed to the callable object.  This is
        useful in conjunction with Restart if you want to schedule a
        new call to the same callable object but with different
        parameters.
        """
        self.args = args
        self.kwargs = kwargs


    def HasRun(self):
        return self.hasRun

    def GetResult(self):
        return self.result

    def Notify(self):
        """
        The timer has expired so call the callable.
        """
        if self.callable and getattr(self.callable, 'im_self', True):
            self.runCount += 1
            self.result = self.callable(*self.args, **self.kwargs)
        self.hasRun = True
        wx.CallAfter(self.Stop)


#----------------------------------------------------------------------------
#----------------------------------------------------------------------------

# Import other modules in this package that should show up in the
# "core" wx namespace
from gdi import *
from windows import *
from controls import *
from misc import *

#----------------------------------------------------------------------------
#----------------------------------------------------------------------------
