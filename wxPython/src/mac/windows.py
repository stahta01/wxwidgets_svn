# This file was created automatically by SWIG.
# Don't modify this file, modify the SWIG interface instead.

import _windows

import core
wx = core 
#---------------------------------------------------------------------------

class Panel(core.Window):
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxPanel instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """
        __init__(Window parent, int id=-1, Point pos=DefaultPosition, 
            Size size=DefaultSize, long style=wxTAB_TRAVERSAL|wxNO_BORDER, 
            String name=PanelNameStr) -> Panel
        """
        newobj = _windows.new_Panel(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
        self._setOORInfo(self)

    def Create(*args, **kwargs):
        """
        Create(Window parent, int id, Point pos=DefaultPosition, Size size=DefaultSize, 
            long style=wxTAB_TRAVERSAL|wxNO_BORDER, 
            String name=PanelNameStr) -> bool

        Create the GUI part of the Window for 2-phase creation mode.
        """
        return _windows.Panel_Create(*args, **kwargs)

    def InitDialog(*args, **kwargs):
        """InitDialog()"""
        return _windows.Panel_InitDialog(*args, **kwargs)


class PanelPtr(Panel):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = Panel
_windows.Panel_swigregister(PanelPtr)

def PrePanel(*args, **kwargs):
    """PrePanel() -> Panel"""
    val = _windows.new_PrePanel(*args, **kwargs)
    val.thisown = 1
    return val

#---------------------------------------------------------------------------

class ScrolledWindow(Panel):
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxScrolledWindow instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """
        __init__(Window parent, int id=-1, Point pos=DefaultPosition, 
            Size size=DefaultSize, long style=wxHSCROLL|wxVSCROLL, 
            String name=PanelNameStr) -> ScrolledWindow
        """
        newobj = _windows.new_ScrolledWindow(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
        self._setOORInfo(self)

    def Create(*args, **kwargs):
        """
        Create(Window parent, int id=-1, Point pos=DefaultPosition, 
            Size size=DefaultSize, long style=wxHSCROLL|wxVSCROLL, 
            String name=PanelNameStr) -> bool

        Create the GUI part of the Window for 2-phase creation mode.
        """
        return _windows.ScrolledWindow_Create(*args, **kwargs)

    def SetScrollbars(*args, **kwargs):
        """
        SetScrollbars(int pixelsPerUnitX, int pixelsPerUnitY, int noUnitsX, 
            int noUnitsY, int xPos=0, int yPos=0, bool noRefresh=False)
        """
        return _windows.ScrolledWindow_SetScrollbars(*args, **kwargs)

    def Scroll(*args, **kwargs):
        """Scroll(int x, int y)"""
        return _windows.ScrolledWindow_Scroll(*args, **kwargs)

    def GetScrollPageSize(*args, **kwargs):
        """GetScrollPageSize(int orient) -> int"""
        return _windows.ScrolledWindow_GetScrollPageSize(*args, **kwargs)

    def SetScrollPageSize(*args, **kwargs):
        """SetScrollPageSize(int orient, int pageSize)"""
        return _windows.ScrolledWindow_SetScrollPageSize(*args, **kwargs)

    def SetScrollRate(*args, **kwargs):
        """SetScrollRate(int xstep, int ystep)"""
        return _windows.ScrolledWindow_SetScrollRate(*args, **kwargs)

    def GetScrollPixelsPerUnit(*args, **kwargs):
        """
        GetScrollPixelsPerUnit() -> (xUnit, yUnit)

        Get the size of one logical unit in physical units.
        """
        return _windows.ScrolledWindow_GetScrollPixelsPerUnit(*args, **kwargs)

    def EnableScrolling(*args, **kwargs):
        """EnableScrolling(bool x_scrolling, bool y_scrolling)"""
        return _windows.ScrolledWindow_EnableScrolling(*args, **kwargs)

    def GetViewStart(*args, **kwargs):
        """
        GetViewStart() -> (x,y)

        Get the view start
        """
        return _windows.ScrolledWindow_GetViewStart(*args, **kwargs)

    def SetScale(*args, **kwargs):
        """SetScale(double xs, double ys)"""
        return _windows.ScrolledWindow_SetScale(*args, **kwargs)

    def GetScaleX(*args, **kwargs):
        """GetScaleX() -> double"""
        return _windows.ScrolledWindow_GetScaleX(*args, **kwargs)

    def GetScaleY(*args, **kwargs):
        """GetScaleY() -> double"""
        return _windows.ScrolledWindow_GetScaleY(*args, **kwargs)

    def CalcScrolledPosition(*args):
        """
        CalcScrolledPosition(Point pt) -> Point
        CalcScrolledPosition(int x, int y) -> (sx, sy)

        Translate between scrolled and unscrolled coordinates.
        """
        return _windows.ScrolledWindow_CalcScrolledPosition(*args)

    def CalcUnscrolledPosition(*args):
        """
        CalcUnscrolledPosition(Point pt) -> Point
        CalcUnscrolledPosition(int x, int y) -> (ux, uy)

        Translate between scrolled and unscrolled coordinates.
        """
        return _windows.ScrolledWindow_CalcUnscrolledPosition(*args)

    def AdjustScrollbars(*args, **kwargs):
        """AdjustScrollbars()"""
        return _windows.ScrolledWindow_AdjustScrollbars(*args, **kwargs)

    def CalcScrollInc(*args, **kwargs):
        """CalcScrollInc(ScrollWinEvent event) -> int"""
        return _windows.ScrolledWindow_CalcScrollInc(*args, **kwargs)

    def SetTargetWindow(*args, **kwargs):
        """SetTargetWindow(Window target)"""
        return _windows.ScrolledWindow_SetTargetWindow(*args, **kwargs)

    def GetTargetWindow(*args, **kwargs):
        """GetTargetWindow() -> Window"""
        return _windows.ScrolledWindow_GetTargetWindow(*args, **kwargs)

    def SetTargetRect(*args, **kwargs):
        """SetTargetRect(Rect rect)"""
        return _windows.ScrolledWindow_SetTargetRect(*args, **kwargs)

    def GetTargetRect(*args, **kwargs):
        """GetTargetRect() -> Rect"""
        return _windows.ScrolledWindow_GetTargetRect(*args, **kwargs)


class ScrolledWindowPtr(ScrolledWindow):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = ScrolledWindow
_windows.ScrolledWindow_swigregister(ScrolledWindowPtr)

def PreScrolledWindow(*args, **kwargs):
    """PreScrolledWindow() -> ScrolledWindow"""
    val = _windows.new_PreScrolledWindow(*args, **kwargs)
    val.thisown = 1
    return val

#---------------------------------------------------------------------------

FULLSCREEN_NOMENUBAR = _windows.FULLSCREEN_NOMENUBAR
FULLSCREEN_NOTOOLBAR = _windows.FULLSCREEN_NOTOOLBAR
FULLSCREEN_NOSTATUSBAR = _windows.FULLSCREEN_NOSTATUSBAR
FULLSCREEN_NOBORDER = _windows.FULLSCREEN_NOBORDER
FULLSCREEN_NOCAPTION = _windows.FULLSCREEN_NOCAPTION
FULLSCREEN_ALL = _windows.FULLSCREEN_ALL
TOPLEVEL_EX_DIALOG = _windows.TOPLEVEL_EX_DIALOG
class TopLevelWindow(core.Window):
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxTopLevelWindow instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def Maximize(*args, **kwargs):
        """Maximize(bool maximize=True)"""
        return _windows.TopLevelWindow_Maximize(*args, **kwargs)

    def Restore(*args, **kwargs):
        """Restore()"""
        return _windows.TopLevelWindow_Restore(*args, **kwargs)

    def Iconize(*args, **kwargs):
        """Iconize(bool iconize=True)"""
        return _windows.TopLevelWindow_Iconize(*args, **kwargs)

    def IsMaximized(*args, **kwargs):
        """IsMaximized() -> bool"""
        return _windows.TopLevelWindow_IsMaximized(*args, **kwargs)

    def IsIconized(*args, **kwargs):
        """IsIconized() -> bool"""
        return _windows.TopLevelWindow_IsIconized(*args, **kwargs)

    def GetIcon(*args, **kwargs):
        """GetIcon() -> Icon"""
        return _windows.TopLevelWindow_GetIcon(*args, **kwargs)

    def SetIcon(*args, **kwargs):
        """SetIcon(Icon icon)"""
        return _windows.TopLevelWindow_SetIcon(*args, **kwargs)

    def SetIcons(*args, **kwargs):
        """SetIcons(wxIconBundle icons)"""
        return _windows.TopLevelWindow_SetIcons(*args, **kwargs)

    def ShowFullScreen(*args, **kwargs):
        """ShowFullScreen(bool show, long style=FULLSCREEN_ALL) -> bool"""
        return _windows.TopLevelWindow_ShowFullScreen(*args, **kwargs)

    def IsFullScreen(*args, **kwargs):
        """IsFullScreen() -> bool"""
        return _windows.TopLevelWindow_IsFullScreen(*args, **kwargs)

    def SetTitle(*args, **kwargs):
        """
        SetTitle(String title)

        Sets the window's title. Applicable only to frames and dialogs.
        """
        return _windows.TopLevelWindow_SetTitle(*args, **kwargs)

    def GetTitle(*args, **kwargs):
        """
        GetTitle() -> String

        Gets the window's title. Applicable only to frames and dialogs.
        """
        return _windows.TopLevelWindow_GetTitle(*args, **kwargs)

    def SetShape(*args, **kwargs):
        """SetShape(Region region) -> bool"""
        return _windows.TopLevelWindow_SetShape(*args, **kwargs)


class TopLevelWindowPtr(TopLevelWindow):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = TopLevelWindow
_windows.TopLevelWindow_swigregister(TopLevelWindowPtr)
cvar = _windows.cvar
FrameNameStr = cvar.FrameNameStr
DialogNameStr = cvar.DialogNameStr
StatusLineNameStr = cvar.StatusLineNameStr
ToolBarNameStr = cvar.ToolBarNameStr

#---------------------------------------------------------------------------

class Frame(TopLevelWindow):
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxFrame instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """
        __init__(Window parent, int id, String title, Point pos=DefaultPosition, 
            Size size=DefaultSize, long style=DEFAULT_FRAME_STYLE, 
            String name=FrameNameStr) -> Frame
        """
        newobj = _windows.new_Frame(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
        self._setOORInfo(self)

    def Create(*args, **kwargs):
        """
        Create(Window parent, int id, String title, Point pos=DefaultPosition, 
            Size size=DefaultSize, long style=DEFAULT_FRAME_STYLE, 
            String name=FrameNameStr) -> bool
        """
        return _windows.Frame_Create(*args, **kwargs)

    def GetClientAreaOrigin(*args, **kwargs):
        """
        GetClientAreaOrigin() -> Point

        Get the origin of the client area of the window relative to the
        window's top left corner (the client area may be shifted because of
        the borders, scrollbars, other decorations...)
        """
        return _windows.Frame_GetClientAreaOrigin(*args, **kwargs)

    def SendSizeEvent(*args, **kwargs):
        """SendSizeEvent()"""
        return _windows.Frame_SendSizeEvent(*args, **kwargs)

    def SetMenuBar(*args, **kwargs):
        """SetMenuBar(MenuBar menubar)"""
        return _windows.Frame_SetMenuBar(*args, **kwargs)

    def GetMenuBar(*args, **kwargs):
        """GetMenuBar() -> MenuBar"""
        return _windows.Frame_GetMenuBar(*args, **kwargs)

    def ProcessCommand(*args, **kwargs):
        """ProcessCommand(int winid) -> bool"""
        return _windows.Frame_ProcessCommand(*args, **kwargs)

    Command = ProcessCommand 
    def CreateStatusBar(*args, **kwargs):
        """
        CreateStatusBar(int number=1, long style=ST_SIZEGRIP, int winid=0, 
            String name=StatusLineNameStr) -> StatusBar
        """
        return _windows.Frame_CreateStatusBar(*args, **kwargs)

    def GetStatusBar(*args, **kwargs):
        """GetStatusBar() -> StatusBar"""
        return _windows.Frame_GetStatusBar(*args, **kwargs)

    def SetStatusBar(*args, **kwargs):
        """SetStatusBar(StatusBar statBar)"""
        return _windows.Frame_SetStatusBar(*args, **kwargs)

    def SetStatusText(*args, **kwargs):
        """SetStatusText(String text, int number=0)"""
        return _windows.Frame_SetStatusText(*args, **kwargs)

    def SetStatusWidths(*args, **kwargs):
        """SetStatusWidths(int widths, int widths_field)"""
        return _windows.Frame_SetStatusWidths(*args, **kwargs)

    def PushStatusText(*args, **kwargs):
        """PushStatusText(String text, int number=0)"""
        return _windows.Frame_PushStatusText(*args, **kwargs)

    def PopStatusText(*args, **kwargs):
        """PopStatusText(int number=0)"""
        return _windows.Frame_PopStatusText(*args, **kwargs)

    def SetStatusBarPane(*args, **kwargs):
        """SetStatusBarPane(int n)"""
        return _windows.Frame_SetStatusBarPane(*args, **kwargs)

    def GetStatusBarPane(*args, **kwargs):
        """GetStatusBarPane() -> int"""
        return _windows.Frame_GetStatusBarPane(*args, **kwargs)

    def CreateToolBar(*args, **kwargs):
        """CreateToolBar(long style=-1, int winid=-1, String name=ToolBarNameStr) -> wxToolBar"""
        return _windows.Frame_CreateToolBar(*args, **kwargs)

    def GetToolBar(*args, **kwargs):
        """GetToolBar() -> wxToolBar"""
        return _windows.Frame_GetToolBar(*args, **kwargs)

    def SetToolBar(*args, **kwargs):
        """SetToolBar(wxToolBar toolbar)"""
        return _windows.Frame_SetToolBar(*args, **kwargs)

    def DoGiveHelp(*args, **kwargs):
        """DoGiveHelp(String text, bool show)"""
        return _windows.Frame_DoGiveHelp(*args, **kwargs)

    def DoMenuUpdates(*args, **kwargs):
        """DoMenuUpdates(Menu menu=None)"""
        return _windows.Frame_DoMenuUpdates(*args, **kwargs)


class FramePtr(Frame):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = Frame
_windows.Frame_swigregister(FramePtr)

def PreFrame(*args, **kwargs):
    """PreFrame() -> Frame"""
    val = _windows.new_PreFrame(*args, **kwargs)
    val.thisown = 1
    return val

#---------------------------------------------------------------------------

class Dialog(TopLevelWindow):
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxDialog instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """
        __init__(Window parent, int id, String title, Point pos=DefaultPosition, 
            Size size=DefaultSize, long style=DEFAULT_DIALOG_STYLE, 
            String name=DialogNameStr) -> Dialog
        """
        newobj = _windows.new_Dialog(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
        self._setOORInfo(self)

    def Create(*args, **kwargs):
        """
        Create(Window parent, int id, String title, Point pos=DefaultPosition, 
            Size size=DefaultSize, long style=DEFAULT_DIALOG_STYLE, 
            String name=DialogNameStr) -> bool
        """
        return _windows.Dialog_Create(*args, **kwargs)

    def SetReturnCode(*args, **kwargs):
        """SetReturnCode(int returnCode)"""
        return _windows.Dialog_SetReturnCode(*args, **kwargs)

    def GetReturnCode(*args, **kwargs):
        """GetReturnCode() -> int"""
        return _windows.Dialog_GetReturnCode(*args, **kwargs)

    def CreateTextSizer(*args, **kwargs):
        """CreateTextSizer(String message) -> Sizer"""
        return _windows.Dialog_CreateTextSizer(*args, **kwargs)

    def CreateButtonSizer(*args, **kwargs):
        """CreateButtonSizer(long flags) -> Sizer"""
        return _windows.Dialog_CreateButtonSizer(*args, **kwargs)

    def IsModal(*args, **kwargs):
        """IsModal() -> bool"""
        return _windows.Dialog_IsModal(*args, **kwargs)

    def ShowModal(*args, **kwargs):
        """ShowModal() -> int"""
        return _windows.Dialog_ShowModal(*args, **kwargs)

    def EndModal(*args, **kwargs):
        """EndModal(int retCode)"""
        return _windows.Dialog_EndModal(*args, **kwargs)

    def IsModalShowing(*args, **kwargs):
        """IsModalShowing() -> bool"""
        return _windows.Dialog_IsModalShowing(*args, **kwargs)


class DialogPtr(Dialog):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = Dialog
_windows.Dialog_swigregister(DialogPtr)

def PreDialog(*args, **kwargs):
    """PreDialog() -> Dialog"""
    val = _windows.new_PreDialog(*args, **kwargs)
    val.thisown = 1
    return val

#---------------------------------------------------------------------------

class MiniFrame(Frame):
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxMiniFrame instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """
        __init__(Window parent, int id, String title, Point pos=DefaultPosition, 
            Size size=DefaultSize, long style=DEFAULT_FRAME_STYLE, 
            String name=FrameNameStr) -> MiniFrame
        """
        newobj = _windows.new_MiniFrame(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
        self._setOORInfo(self)

    def Create(*args, **kwargs):
        """
        Create(Window parent, int id, String title, Point pos=DefaultPosition, 
            Size size=DefaultSize, long style=DEFAULT_FRAME_STYLE, 
            String name=FrameNameStr) -> bool
        """
        return _windows.MiniFrame_Create(*args, **kwargs)


class MiniFramePtr(MiniFrame):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = MiniFrame
_windows.MiniFrame_swigregister(MiniFramePtr)

def PreMiniFrame(*args, **kwargs):
    """PreMiniFrame() -> MiniFrame"""
    val = _windows.new_PreMiniFrame(*args, **kwargs)
    val.thisown = 1
    return val

#---------------------------------------------------------------------------

SPLASH_CENTRE_ON_PARENT = _windows.SPLASH_CENTRE_ON_PARENT
SPLASH_CENTRE_ON_SCREEN = _windows.SPLASH_CENTRE_ON_SCREEN
SPLASH_NO_CENTRE = _windows.SPLASH_NO_CENTRE
SPLASH_TIMEOUT = _windows.SPLASH_TIMEOUT
SPLASH_NO_TIMEOUT = _windows.SPLASH_NO_TIMEOUT
class SplashScreenWindow(core.Window):
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxSplashScreenWindow instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """
        __init__(Bitmap bitmap, Window parent, int id, Point pos=DefaultPosition, 
            Size size=DefaultSize, long style=NO_BORDER) -> SplashScreenWindow
        """
        newobj = _windows.new_SplashScreenWindow(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
        self._setOORInfo(self)

    def SetBitmap(*args, **kwargs):
        """SetBitmap(Bitmap bitmap)"""
        return _windows.SplashScreenWindow_SetBitmap(*args, **kwargs)

    def GetBitmap(*args, **kwargs):
        """GetBitmap() -> Bitmap"""
        return _windows.SplashScreenWindow_GetBitmap(*args, **kwargs)


class SplashScreenWindowPtr(SplashScreenWindow):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = SplashScreenWindow
_windows.SplashScreenWindow_swigregister(SplashScreenWindowPtr)

class SplashScreen(Frame):
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxSplashScreen instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """
        __init__(Bitmap bitmap, long splashStyle, int milliseconds, 
            Window parent, int id, Point pos=DefaultPosition, 
            Size size=DefaultSize, long style=wxSIMPLE_BORDER|wxFRAME_NO_TASKBAR|wxSTAY_ON_TOP) -> SplashScreen
        """
        newobj = _windows.new_SplashScreen(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
        self._setOORInfo(self)

    def GetSplashStyle(*args, **kwargs):
        """GetSplashStyle() -> long"""
        return _windows.SplashScreen_GetSplashStyle(*args, **kwargs)

    def GetSplashWindow(*args, **kwargs):
        """GetSplashWindow() -> SplashScreenWindow"""
        return _windows.SplashScreen_GetSplashWindow(*args, **kwargs)

    def GetTimeout(*args, **kwargs):
        """GetTimeout() -> int"""
        return _windows.SplashScreen_GetTimeout(*args, **kwargs)


class SplashScreenPtr(SplashScreen):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = SplashScreen
_windows.SplashScreen_swigregister(SplashScreenPtr)

#---------------------------------------------------------------------------

class StatusBar(core.Window):
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxStatusBar instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """__init__(Window parent, int id=-1, long style=ST_SIZEGRIP, String name=StatusLineNameStr) -> StatusBar"""
        newobj = _windows.new_StatusBar(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
        self._setOORInfo(self)

    def Create(*args, **kwargs):
        """Create(Window parent, int id, long style=ST_SIZEGRIP, String name=StatusLineNameStr) -> bool"""
        return _windows.StatusBar_Create(*args, **kwargs)

    def SetFieldsCount(*args, **kwargs):
        """SetFieldsCount(int number=1)"""
        return _windows.StatusBar_SetFieldsCount(*args, **kwargs)

    def GetFieldsCount(*args, **kwargs):
        """GetFieldsCount() -> int"""
        return _windows.StatusBar_GetFieldsCount(*args, **kwargs)

    def SetStatusText(*args, **kwargs):
        """SetStatusText(String text, int number=0)"""
        return _windows.StatusBar_SetStatusText(*args, **kwargs)

    def GetStatusText(*args, **kwargs):
        """GetStatusText(int number=0) -> String"""
        return _windows.StatusBar_GetStatusText(*args, **kwargs)

    def PushStatusText(*args, **kwargs):
        """PushStatusText(String text, int number=0)"""
        return _windows.StatusBar_PushStatusText(*args, **kwargs)

    def PopStatusText(*args, **kwargs):
        """PopStatusText(int number=0)"""
        return _windows.StatusBar_PopStatusText(*args, **kwargs)

    def SetStatusWidths(*args, **kwargs):
        """SetStatusWidths(int widths, int widths_field)"""
        return _windows.StatusBar_SetStatusWidths(*args, **kwargs)

    def GetFieldRect(*args, **kwargs):
        """GetFieldRect(int i) -> Rect"""
        return _windows.StatusBar_GetFieldRect(*args, **kwargs)

    def SetMinHeight(*args, **kwargs):
        """SetMinHeight(int height)"""
        return _windows.StatusBar_SetMinHeight(*args, **kwargs)

    def GetBorderX(*args, **kwargs):
        """GetBorderX() -> int"""
        return _windows.StatusBar_GetBorderX(*args, **kwargs)

    def GetBorderY(*args, **kwargs):
        """GetBorderY() -> int"""
        return _windows.StatusBar_GetBorderY(*args, **kwargs)


class StatusBarPtr(StatusBar):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = StatusBar
_windows.StatusBar_swigregister(StatusBarPtr)

def PreStatusBar(*args, **kwargs):
    """PreStatusBar() -> StatusBar"""
    val = _windows.new_PreStatusBar(*args, **kwargs)
    val.thisown = 1
    return val

#---------------------------------------------------------------------------

SP_NOBORDER = _windows.SP_NOBORDER
SP_NOSASH = _windows.SP_NOSASH
SP_PERMIT_UNSPLIT = _windows.SP_PERMIT_UNSPLIT
SP_LIVE_UPDATE = _windows.SP_LIVE_UPDATE
SP_3DSASH = _windows.SP_3DSASH
SP_3DBORDER = _windows.SP_3DBORDER
SP_NO_XP_THEME = _windows.SP_NO_XP_THEME
SP_BORDER = _windows.SP_BORDER
SP_3D = _windows.SP_3D
SPLIT_HORIZONTAL = _windows.SPLIT_HORIZONTAL
SPLIT_VERTICAL = _windows.SPLIT_VERTICAL
SPLIT_DRAG_NONE = _windows.SPLIT_DRAG_NONE
SPLIT_DRAG_DRAGGING = _windows.SPLIT_DRAG_DRAGGING
SPLIT_DRAG_LEFT_DOWN = _windows.SPLIT_DRAG_LEFT_DOWN
class SplitterWindow(core.Window):
    """
    wx.SplitterWindow manages up to two subwindows or panes,
    with an optional vertical or horizontal split which can be
    used with the mouse or programmatically.

    """
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxSplitterWindow instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """
        __init__(Window parent, int id, Point pos=DefaultPosition, Size size=DefaultSize, 
            long style=SP_3D, String name=SplitterNameStr) -> SplitterWindow

        Constructor.  Creates and shows a SplitterWindow.
        """
        if kwargs.has_key('point'): kwargs['pos'] = kwargs['point']
        newobj = _windows.new_SplitterWindow(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
        self._setOORInfo(self)

    def Create(*args, **kwargs):
        """
        Create(Window parent, int id, Point pos=DefaultPosition, Size size=DefaultSize, 
            long style=SP_3D, String name=SplitterNameStr) -> bool

        Create the GUI part of the SplitterWindow for the 2-phase create.
        """
        return _windows.SplitterWindow_Create(*args, **kwargs)

    def GetWindow1(*args, **kwargs):
        """
        GetWindow1() -> Window

        Gets the only or left/top pane.
        """
        return _windows.SplitterWindow_GetWindow1(*args, **kwargs)

    def GetWindow2(*args, **kwargs):
        """
        GetWindow2() -> Window

        Gets the right/bottom pane.
        """
        return _windows.SplitterWindow_GetWindow2(*args, **kwargs)

    def SetSplitMode(*args, **kwargs):
        """
        SetSplitMode(int mode)

        Sets the split mode.  The mode can be wx.SPLIT_VERTICAL or
        wx.SPLIT_HORIZONTAL.  This only sets the internal variable;
        does not update the display.
        """
        return _windows.SplitterWindow_SetSplitMode(*args, **kwargs)

    def GetSplitMode(*args, **kwargs):
        """
        GetSplitMode() -> int

        Gets the split mode
        """
        return _windows.SplitterWindow_GetSplitMode(*args, **kwargs)

    def Initialize(*args, **kwargs):
        """
        Initialize(Window window)

        Initializes the splitter window to have one pane.  This
        should be called if you wish to initially view only a single
        pane in the splitter window.
        """
        return _windows.SplitterWindow_Initialize(*args, **kwargs)

    def SplitVertically(*args, **kwargs):
        """
        SplitVertically(Window window1, Window window2, int sashPosition=0) -> bool

        Initializes the left and right panes of the splitter window.

            window1       The left pane.
            window2       The right pane.
            sashPosition  The initial position of the sash. If this
                          value is positive, it specifies the size
                          of the left pane. If it is negative, it is
                          absolute value gives the size of the right
                          pane. Finally, specify 0 (default) to
                          choose the default position (half of the
                          total window width).

        Returns True if successful, False otherwise (the window was
        already split).

        SplitVertically should be called if you wish to initially
        view two panes. It can also be called at any subsequent
        time, but the application should check that the window is
        not currently split using IsSplit.
        """
        return _windows.SplitterWindow_SplitVertically(*args, **kwargs)

    def SplitHorizontally(*args, **kwargs):
        """
        SplitHorizontally(Window window1, Window window2, int sashPosition=0) -> bool

        Initializes the top and bottom panes of the splitter window.

            window1       The top pane.
            window2       The bottom pane.
            sashPosition  The initial position of the sash. If this
                          value is positive, it specifies the size
                          of the upper pane. If it is negative, it
                          is absolute value gives the size of the
                          lower pane. Finally, specify 0 (default)
                          to choose the default position (half of
                          the total window height).

        Returns True if successful, False otherwise (the window was
        already split).

        SplitHorizontally should be called if you wish to initially
        view two panes. It can also be called at any subsequent
        time, but the application should check that the window is
        not currently split using IsSplit.
        """
        return _windows.SplitterWindow_SplitHorizontally(*args, **kwargs)

    def Unsplit(*args, **kwargs):
        """
        Unsplit(Window toRemove=None) -> bool

        Unsplits the window.  Pass the pane to remove, or None to
        remove the right or bottom pane.  Returns True if
        successful, False otherwise (the window was not split).

        This function will not actually delete the pane being
        removed; it sends EVT_SPLITTER_UNSPLIT which can be handled
        for the desired behaviour. By default, the pane being
        removed is only hidden.
        """
        return _windows.SplitterWindow_Unsplit(*args, **kwargs)

    def ReplaceWindow(*args, **kwargs):
        """
        ReplaceWindow(Window winOld, Window winNew) -> bool

        This function replaces one of the windows managed by the
        SplitterWindow with another one. It is in general better to
        use it instead of calling Unsplit() and then resplitting the
        window back because it will provoke much less flicker. It is
        valid to call this function whether the splitter has two
        windows or only one.

        Both parameters should be non-None and winOld must specify
        one of the windows managed by the splitter. If the
        parameters are incorrect or the window couldn't be replaced,
        False is returned. Otherwise the function will return True,
        but please notice that it will not Destroy the replaced
        window and you may wish to do it yourself.
        """
        return _windows.SplitterWindow_ReplaceWindow(*args, **kwargs)

    def UpdateSize(*args, **kwargs):
        """
        UpdateSize()

        Causes any pending sizing of the sash and child panes to
        take place immediately.

        Such resizing normally takes place in idle time, in order to
        wait for layout to be completed. However, this can cause
        unacceptable flicker as the panes are resized after the
        window has been shown. To work around this, you can perform
        window layout (for example by sending a size event to the
        parent window), and then call this function, before showing
        the top-level window.
        """
        return _windows.SplitterWindow_UpdateSize(*args, **kwargs)

    def IsSplit(*args, **kwargs):
        """
        IsSplit() -> bool

        Is the window split?
        """
        return _windows.SplitterWindow_IsSplit(*args, **kwargs)

    def SetSashSize(*args, **kwargs):
        """
        SetSashSize(int width)

        Sets the sash size
        """
        return _windows.SplitterWindow_SetSashSize(*args, **kwargs)

    def SetBorderSize(*args, **kwargs):
        """
        SetBorderSize(int width)

        Sets the border size
        """
        return _windows.SplitterWindow_SetBorderSize(*args, **kwargs)

    def GetSashSize(*args, **kwargs):
        """
        GetSashSize() -> int

        Gets the sash size
        """
        return _windows.SplitterWindow_GetSashSize(*args, **kwargs)

    def GetBorderSize(*args, **kwargs):
        """
        GetBorderSize() -> int

        Gets the border size
        """
        return _windows.SplitterWindow_GetBorderSize(*args, **kwargs)

    def SetSashPosition(*args, **kwargs):
        """
        SetSashPosition(int position, bool redraw=True)

        Sets the sash position, in pixels.  If redraw is Ttrue then
        the panes are resized and the sash and border are redrawn.
        """
        return _windows.SplitterWindow_SetSashPosition(*args, **kwargs)

    def GetSashPosition(*args, **kwargs):
        """
        GetSashPosition() -> int

        Returns the surrent sash position.
        """
        return _windows.SplitterWindow_GetSashPosition(*args, **kwargs)

    def SetMinimumPaneSize(*args, **kwargs):
        """
        SetMinimumPaneSize(int min)

        Sets the minimum pane size in pixels.

        The default minimum pane size is zero, which means that
        either pane can be reduced to zero by dragging the sash,
        thus removing one of the panes. To prevent this behaviour (and
        veto out-of-range sash dragging), set a minimum size,
        for example 20 pixels. If the wx.SP_PERMIT_UNSPLIT style is
        used when a splitter window is created, the window may be
        unsplit even if minimum size is non-zero.
        """
        return _windows.SplitterWindow_SetMinimumPaneSize(*args, **kwargs)

    def GetMinimumPaneSize(*args, **kwargs):
        """
        GetMinimumPaneSize() -> int

        Gets the minimum pane size in pixels.
        """
        return _windows.SplitterWindow_GetMinimumPaneSize(*args, **kwargs)

    def SashHitTest(*args, **kwargs):
        """
        SashHitTest(int x, int y, int tolerance=5) -> bool

        Tests for x, y over the sash
        """
        return _windows.SplitterWindow_SashHitTest(*args, **kwargs)

    def SizeWindows(*args, **kwargs):
        """
        SizeWindows()

        Resizes subwindows
        """
        return _windows.SplitterWindow_SizeWindows(*args, **kwargs)

    def SetNeedUpdating(*args, **kwargs):
        """SetNeedUpdating(bool needUpdating)"""
        return _windows.SplitterWindow_SetNeedUpdating(*args, **kwargs)

    def GetNeedUpdating(*args, **kwargs):
        """GetNeedUpdating() -> bool"""
        return _windows.SplitterWindow_GetNeedUpdating(*args, **kwargs)


class SplitterWindowPtr(SplitterWindow):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = SplitterWindow
_windows.SplitterWindow_swigregister(SplitterWindowPtr)
SplitterNameStr = cvar.SplitterNameStr

def PreSplitterWindow(*args, **kwargs):
    """
    PreSplitterWindow() -> SplitterWindow

    Precreate a SplitterWindow for 2-phase creation.
    """
    val = _windows.new_PreSplitterWindow(*args, **kwargs)
    val.thisown = 1
    return val

class SplitterEvent(core.NotifyEvent):
    """This class represents the events generated by a splitter control."""
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxSplitterEvent instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """
        __init__(wxEventType type=wxEVT_NULL, SplitterWindow splitter=(wxSplitterWindow *) NULL) -> SplitterEvent

        This class represents the events generated by a splitter control.
        """
        newobj = _windows.new_SplitterEvent(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
    def SetSashPosition(*args, **kwargs):
        """
        SetSashPosition(int pos)

        This funciton is only meaningful during
        EVT_SPLITTER_SASH_POS_CHANGING and
        EVT_SPLITTER_SASH_POS_CHANGED events.  In the case of
        _CHANGED events, sets the the new sash position. In the case
        of _CHANGING events, sets the new tracking bar position so
        visual feedback during dragging will represent that change
        that will actually take place. Set to -1 from the event
        handler code to prevent repositioning.
        """
        return _windows.SplitterEvent_SetSashPosition(*args, **kwargs)

    def GetSashPosition(*args, **kwargs):
        """
        GetSashPosition() -> int

        Returns the new sash position while in
        EVT_SPLITTER_SASH_POS_CHANGING and
        EVT_SPLITTER_SASH_POS_CHANGED events.
        """
        return _windows.SplitterEvent_GetSashPosition(*args, **kwargs)

    def GetWindowBeingRemoved(*args, **kwargs):
        """
        GetWindowBeingRemoved() -> Window

        Returns a pointer to the window being removed when a
        splitter window is unsplit.
        """
        return _windows.SplitterEvent_GetWindowBeingRemoved(*args, **kwargs)

    def GetX(*args, **kwargs):
        """
        GetX() -> int

        Returns the x coordinate of the double-click point in a
        EVT_SPLITTER_DCLICK event.
        """
        return _windows.SplitterEvent_GetX(*args, **kwargs)

    def GetY(*args, **kwargs):
        """
        GetY() -> int

        Returns the y coordinate of the double-click point in a
        EVT_SPLITTER_DCLICK event.
        """
        return _windows.SplitterEvent_GetY(*args, **kwargs)


class SplitterEventPtr(SplitterEvent):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = SplitterEvent
_windows.SplitterEvent_swigregister(SplitterEventPtr)

wxEVT_COMMAND_SPLITTER_SASH_POS_CHANGED = _windows.wxEVT_COMMAND_SPLITTER_SASH_POS_CHANGED
wxEVT_COMMAND_SPLITTER_SASH_POS_CHANGING = _windows.wxEVT_COMMAND_SPLITTER_SASH_POS_CHANGING
wxEVT_COMMAND_SPLITTER_DOUBLECLICKED = _windows.wxEVT_COMMAND_SPLITTER_DOUBLECLICKED
wxEVT_COMMAND_SPLITTER_UNSPLIT = _windows.wxEVT_COMMAND_SPLITTER_UNSPLIT
EVT_SPLITTER_SASH_POS_CHANGED = wx.PyEventBinder( wxEVT_COMMAND_SPLITTER_SASH_POS_CHANGED, 1 )
EVT_SPLITTER_SASH_POS_CHANGING = wx.PyEventBinder( wxEVT_COMMAND_SPLITTER_SASH_POS_CHANGING, 1 )
EVT_SPLITTER_DOUBLECLICKED = wx.PyEventBinder( wxEVT_COMMAND_SPLITTER_DOUBLECLICKED, 1 )
EVT_SPLITTER_UNSPLIT = wx.PyEventBinder( wxEVT_COMMAND_SPLITTER_UNSPLIT, 1 )
EVT_SPLITTER_DCLICK = EVT_SPLITTER_DOUBLECLICKED

#---------------------------------------------------------------------------

SASH_DRAG_NONE = _windows.SASH_DRAG_NONE
SASH_DRAG_DRAGGING = _windows.SASH_DRAG_DRAGGING
SASH_DRAG_LEFT_DOWN = _windows.SASH_DRAG_LEFT_DOWN
SW_NOBORDER = _windows.SW_NOBORDER
SW_BORDER = _windows.SW_BORDER
SW_3DSASH = _windows.SW_3DSASH
SW_3DBORDER = _windows.SW_3DBORDER
SW_3D = _windows.SW_3D
SASH_TOP = _windows.SASH_TOP
SASH_RIGHT = _windows.SASH_RIGHT
SASH_BOTTOM = _windows.SASH_BOTTOM
SASH_LEFT = _windows.SASH_LEFT
SASH_NONE = _windows.SASH_NONE
class SashWindow(core.Window):
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxSashWindow instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """
        __init__(Window parent, int id, Point pos=DefaultPosition, Size size=DefaultSize, 
            long style=wxCLIP_CHILDREN|wxSW_3D, 
            String name=SashNameStr) -> SashWindow
        """
        newobj = _windows.new_SashWindow(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
        self._setOORInfo(self)

    def Create(*args, **kwargs):
        """
        Create(Window parent, int id, Point pos=DefaultPosition, Size size=DefaultSize, 
            long style=wxCLIP_CHILDREN|wxSW_3D, 
            String name=SashNameStr) -> bool
        """
        return _windows.SashWindow_Create(*args, **kwargs)

    def SetSashVisible(*args, **kwargs):
        """SetSashVisible(int edge, bool sash)"""
        return _windows.SashWindow_SetSashVisible(*args, **kwargs)

    def GetSashVisible(*args, **kwargs):
        """GetSashVisible(int edge) -> bool"""
        return _windows.SashWindow_GetSashVisible(*args, **kwargs)

    def SetSashBorder(*args, **kwargs):
        """SetSashBorder(int edge, bool border)"""
        return _windows.SashWindow_SetSashBorder(*args, **kwargs)

    def HasBorder(*args, **kwargs):
        """HasBorder(int edge) -> bool"""
        return _windows.SashWindow_HasBorder(*args, **kwargs)

    def GetEdgeMargin(*args, **kwargs):
        """GetEdgeMargin(int edge) -> int"""
        return _windows.SashWindow_GetEdgeMargin(*args, **kwargs)

    def SetDefaultBorderSize(*args, **kwargs):
        """SetDefaultBorderSize(int width)"""
        return _windows.SashWindow_SetDefaultBorderSize(*args, **kwargs)

    def GetDefaultBorderSize(*args, **kwargs):
        """GetDefaultBorderSize() -> int"""
        return _windows.SashWindow_GetDefaultBorderSize(*args, **kwargs)

    def SetExtraBorderSize(*args, **kwargs):
        """SetExtraBorderSize(int width)"""
        return _windows.SashWindow_SetExtraBorderSize(*args, **kwargs)

    def GetExtraBorderSize(*args, **kwargs):
        """GetExtraBorderSize() -> int"""
        return _windows.SashWindow_GetExtraBorderSize(*args, **kwargs)

    def SetMinimumSizeX(*args, **kwargs):
        """SetMinimumSizeX(int min)"""
        return _windows.SashWindow_SetMinimumSizeX(*args, **kwargs)

    def SetMinimumSizeY(*args, **kwargs):
        """SetMinimumSizeY(int min)"""
        return _windows.SashWindow_SetMinimumSizeY(*args, **kwargs)

    def GetMinimumSizeX(*args, **kwargs):
        """GetMinimumSizeX() -> int"""
        return _windows.SashWindow_GetMinimumSizeX(*args, **kwargs)

    def GetMinimumSizeY(*args, **kwargs):
        """GetMinimumSizeY() -> int"""
        return _windows.SashWindow_GetMinimumSizeY(*args, **kwargs)

    def SetMaximumSizeX(*args, **kwargs):
        """SetMaximumSizeX(int max)"""
        return _windows.SashWindow_SetMaximumSizeX(*args, **kwargs)

    def SetMaximumSizeY(*args, **kwargs):
        """SetMaximumSizeY(int max)"""
        return _windows.SashWindow_SetMaximumSizeY(*args, **kwargs)

    def GetMaximumSizeX(*args, **kwargs):
        """GetMaximumSizeX() -> int"""
        return _windows.SashWindow_GetMaximumSizeX(*args, **kwargs)

    def GetMaximumSizeY(*args, **kwargs):
        """GetMaximumSizeY() -> int"""
        return _windows.SashWindow_GetMaximumSizeY(*args, **kwargs)

    def SashHitTest(*args, **kwargs):
        """SashHitTest(int x, int y, int tolerance=2) -> int"""
        return _windows.SashWindow_SashHitTest(*args, **kwargs)

    def SizeWindows(*args, **kwargs):
        """SizeWindows()"""
        return _windows.SashWindow_SizeWindows(*args, **kwargs)


class SashWindowPtr(SashWindow):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = SashWindow
_windows.SashWindow_swigregister(SashWindowPtr)
SashNameStr = cvar.SashNameStr
SashLayoutNameStr = cvar.SashLayoutNameStr

def PreSashWindow(*args, **kwargs):
    """PreSashWindow() -> SashWindow"""
    val = _windows.new_PreSashWindow(*args, **kwargs)
    val.thisown = 1
    return val

SASH_STATUS_OK = _windows.SASH_STATUS_OK
SASH_STATUS_OUT_OF_RANGE = _windows.SASH_STATUS_OUT_OF_RANGE
class SashEvent(core.CommandEvent):
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxSashEvent instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """__init__(int id=0, int edge=SASH_NONE) -> SashEvent"""
        newobj = _windows.new_SashEvent(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
    def SetEdge(*args, **kwargs):
        """SetEdge(int edge)"""
        return _windows.SashEvent_SetEdge(*args, **kwargs)

    def GetEdge(*args, **kwargs):
        """GetEdge() -> int"""
        return _windows.SashEvent_GetEdge(*args, **kwargs)

    def SetDragRect(*args, **kwargs):
        """SetDragRect(Rect rect)"""
        return _windows.SashEvent_SetDragRect(*args, **kwargs)

    def GetDragRect(*args, **kwargs):
        """GetDragRect() -> Rect"""
        return _windows.SashEvent_GetDragRect(*args, **kwargs)

    def SetDragStatus(*args, **kwargs):
        """SetDragStatus(int status)"""
        return _windows.SashEvent_SetDragStatus(*args, **kwargs)

    def GetDragStatus(*args, **kwargs):
        """GetDragStatus() -> int"""
        return _windows.SashEvent_GetDragStatus(*args, **kwargs)


class SashEventPtr(SashEvent):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = SashEvent
_windows.SashEvent_swigregister(SashEventPtr)

wxEVT_SASH_DRAGGED = _windows.wxEVT_SASH_DRAGGED
EVT_SASH_DRAGGED = wx.PyEventBinder( wxEVT_SASH_DRAGGED, 1 )
EVT_SASH_DRAGGED_RANGE = wx.PyEventBinder( wxEVT_SASH_DRAGGED, 2 )

#---------------------------------------------------------------------------

LAYOUT_HORIZONTAL = _windows.LAYOUT_HORIZONTAL
LAYOUT_VERTICAL = _windows.LAYOUT_VERTICAL
LAYOUT_NONE = _windows.LAYOUT_NONE
LAYOUT_TOP = _windows.LAYOUT_TOP
LAYOUT_LEFT = _windows.LAYOUT_LEFT
LAYOUT_RIGHT = _windows.LAYOUT_RIGHT
LAYOUT_BOTTOM = _windows.LAYOUT_BOTTOM
LAYOUT_LENGTH_Y = _windows.LAYOUT_LENGTH_Y
LAYOUT_LENGTH_X = _windows.LAYOUT_LENGTH_X
LAYOUT_MRU_LENGTH = _windows.LAYOUT_MRU_LENGTH
LAYOUT_QUERY = _windows.LAYOUT_QUERY
wxEVT_QUERY_LAYOUT_INFO = _windows.wxEVT_QUERY_LAYOUT_INFO
wxEVT_CALCULATE_LAYOUT = _windows.wxEVT_CALCULATE_LAYOUT
class QueryLayoutInfoEvent(core.Event):
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxQueryLayoutInfoEvent instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """__init__(int id=0) -> QueryLayoutInfoEvent"""
        newobj = _windows.new_QueryLayoutInfoEvent(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
    def SetRequestedLength(*args, **kwargs):
        """SetRequestedLength(int length)"""
        return _windows.QueryLayoutInfoEvent_SetRequestedLength(*args, **kwargs)

    def GetRequestedLength(*args, **kwargs):
        """GetRequestedLength() -> int"""
        return _windows.QueryLayoutInfoEvent_GetRequestedLength(*args, **kwargs)

    def SetFlags(*args, **kwargs):
        """SetFlags(int flags)"""
        return _windows.QueryLayoutInfoEvent_SetFlags(*args, **kwargs)

    def GetFlags(*args, **kwargs):
        """GetFlags() -> int"""
        return _windows.QueryLayoutInfoEvent_GetFlags(*args, **kwargs)

    def SetSize(*args, **kwargs):
        """SetSize(Size size)"""
        return _windows.QueryLayoutInfoEvent_SetSize(*args, **kwargs)

    def GetSize(*args, **kwargs):
        """GetSize() -> Size"""
        return _windows.QueryLayoutInfoEvent_GetSize(*args, **kwargs)

    def SetOrientation(*args, **kwargs):
        """SetOrientation(int orient)"""
        return _windows.QueryLayoutInfoEvent_SetOrientation(*args, **kwargs)

    def GetOrientation(*args, **kwargs):
        """GetOrientation() -> int"""
        return _windows.QueryLayoutInfoEvent_GetOrientation(*args, **kwargs)

    def SetAlignment(*args, **kwargs):
        """SetAlignment(int align)"""
        return _windows.QueryLayoutInfoEvent_SetAlignment(*args, **kwargs)

    def GetAlignment(*args, **kwargs):
        """GetAlignment() -> int"""
        return _windows.QueryLayoutInfoEvent_GetAlignment(*args, **kwargs)


class QueryLayoutInfoEventPtr(QueryLayoutInfoEvent):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = QueryLayoutInfoEvent
_windows.QueryLayoutInfoEvent_swigregister(QueryLayoutInfoEventPtr)

class CalculateLayoutEvent(core.Event):
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxCalculateLayoutEvent instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """__init__(int id=0) -> CalculateLayoutEvent"""
        newobj = _windows.new_CalculateLayoutEvent(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
    def SetFlags(*args, **kwargs):
        """SetFlags(int flags)"""
        return _windows.CalculateLayoutEvent_SetFlags(*args, **kwargs)

    def GetFlags(*args, **kwargs):
        """GetFlags() -> int"""
        return _windows.CalculateLayoutEvent_GetFlags(*args, **kwargs)

    def SetRect(*args, **kwargs):
        """SetRect(Rect rect)"""
        return _windows.CalculateLayoutEvent_SetRect(*args, **kwargs)

    def GetRect(*args, **kwargs):
        """GetRect() -> Rect"""
        return _windows.CalculateLayoutEvent_GetRect(*args, **kwargs)


class CalculateLayoutEventPtr(CalculateLayoutEvent):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CalculateLayoutEvent
_windows.CalculateLayoutEvent_swigregister(CalculateLayoutEventPtr)

EVT_QUERY_LAYOUT_INFO = wx.PyEventBinder( wxEVT_QUERY_LAYOUT_INFO )
EVT_CALCULATE_LAYOUT = wx.PyEventBinder( wxEVT_CALCULATE_LAYOUT )

class SashLayoutWindow(SashWindow):
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxSashLayoutWindow instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """
        __init__(Window parent, int id, Point pos=DefaultPosition, Size size=DefaultSize, 
            long style=wxCLIP_CHILDREN|wxSW_3D, 
            String name=SashLayoutNameStr) -> SashLayoutWindow
        """
        newobj = _windows.new_SashLayoutWindow(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
        self._setOORInfo(self)

    def Create(*args, **kwargs):
        """
        Create(Window parent, int id, Point pos=DefaultPosition, Size size=DefaultSize, 
            long style=wxCLIP_CHILDREN|wxSW_3D, 
            String name=SashLayoutNameStr) -> bool
        """
        return _windows.SashLayoutWindow_Create(*args, **kwargs)

    def GetAlignment(*args, **kwargs):
        """GetAlignment() -> int"""
        return _windows.SashLayoutWindow_GetAlignment(*args, **kwargs)

    def GetOrientation(*args, **kwargs):
        """GetOrientation() -> int"""
        return _windows.SashLayoutWindow_GetOrientation(*args, **kwargs)

    def SetAlignment(*args, **kwargs):
        """SetAlignment(int alignment)"""
        return _windows.SashLayoutWindow_SetAlignment(*args, **kwargs)

    def SetDefaultSize(*args, **kwargs):
        """SetDefaultSize(Size size)"""
        return _windows.SashLayoutWindow_SetDefaultSize(*args, **kwargs)

    def SetOrientation(*args, **kwargs):
        """SetOrientation(int orientation)"""
        return _windows.SashLayoutWindow_SetOrientation(*args, **kwargs)


class SashLayoutWindowPtr(SashLayoutWindow):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = SashLayoutWindow
_windows.SashLayoutWindow_swigregister(SashLayoutWindowPtr)

def PreSashLayoutWindow(*args, **kwargs):
    """PreSashLayoutWindow() -> SashLayoutWindow"""
    val = _windows.new_PreSashLayoutWindow(*args, **kwargs)
    val.thisown = 1
    return val

class LayoutAlgorithm(core.Object):
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxLayoutAlgorithm instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """__init__() -> LayoutAlgorithm"""
        newobj = _windows.new_LayoutAlgorithm(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
    def __del__(self, destroy=_windows.delete_LayoutAlgorithm):
        """__del__()"""
        try:
            if self.thisown: destroy(self)
        except: pass

    def LayoutMDIFrame(*args, **kwargs):
        """LayoutMDIFrame(MDIParentFrame frame, Rect rect=None) -> bool"""
        return _windows.LayoutAlgorithm_LayoutMDIFrame(*args, **kwargs)

    def LayoutFrame(*args, **kwargs):
        """LayoutFrame(Frame frame, Window mainWindow=None) -> bool"""
        return _windows.LayoutAlgorithm_LayoutFrame(*args, **kwargs)

    def LayoutWindow(*args, **kwargs):
        """LayoutWindow(Window parent, Window mainWindow=None) -> bool"""
        return _windows.LayoutAlgorithm_LayoutWindow(*args, **kwargs)


class LayoutAlgorithmPtr(LayoutAlgorithm):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = LayoutAlgorithm
_windows.LayoutAlgorithm_swigregister(LayoutAlgorithmPtr)

class PopupWindow(core.Window):
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxPopupWindow instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """__init__(Window parent, int flags=BORDER_NONE) -> PopupWindow"""
        newobj = _windows.new_PopupWindow(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown

class PopupWindowPtr(PopupWindow):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = PopupWindow
_windows.PopupWindow_swigregister(PopupWindowPtr)

def PrePopupWindow(*args, **kwargs):
    """PrePopupWindow() -> PopupWindow"""
    val = _windows.new_PrePopupWindow(*args, **kwargs)
    val.thisown = 1
    return val

class PopupTransientWindow(PopupWindow):
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxPyPopupTransientWindow instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """__init__(Window parent, int style=BORDER_NONE) -> PopupTransientWindow"""
        newobj = _windows.new_PopupTransientWindow(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown

class PopupTransientWindowPtr(PopupTransientWindow):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = PopupTransientWindow
_windows.PopupTransientWindow_swigregister(PopupTransientWindowPtr)

def PrePopupTransientWindow(*args, **kwargs):
    """PrePopupTransientWindow() -> PopupTransientWindow"""
    val = _windows.new_PrePopupTransientWindow(*args, **kwargs)
    val.thisown = 1
    return val

#---------------------------------------------------------------------------

class TipWindow(Frame):
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxTipWindow instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """__init__(Window parent, String text, int maxLength=100, Rect rectBound=None) -> TipWindow"""
        newobj = _windows.new_TipWindow(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
        self._setOORInfo(self)

    def SetBoundingRect(*args, **kwargs):
        """SetBoundingRect(Rect rectBound)"""
        return _windows.TipWindow_SetBoundingRect(*args, **kwargs)

    def Close(*args, **kwargs):
        """Close()"""
        return _windows.TipWindow_Close(*args, **kwargs)


class TipWindowPtr(TipWindow):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = TipWindow
_windows.TipWindow_swigregister(TipWindowPtr)

#---------------------------------------------------------------------------

class VScrolledWindow(Panel):
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxPyVScrolledWindow instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """
        __init__(Window parent, int id=ID_ANY, Point pos=DefaultPosition, 
            Size size=DefaultSize, long style=0, String name=PanelNameStr) -> VScrolledWindow
        """
        newobj = _windows.new_VScrolledWindow(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
        self._setOORInfo(self); self._setCallbackInfo(self, VScrolledWindow)

    def _setCallbackInfo(*args, **kwargs):
        """_setCallbackInfo(PyObject self, PyObject _class)"""
        return _windows.VScrolledWindow__setCallbackInfo(*args, **kwargs)

    def Create(*args, **kwargs):
        """
        Create(Window parent, int id=ID_ANY, Point pos=DefaultPosition, 
            Size size=DefaultSize, long style=0, String name=PanelNameStr) -> bool
        """
        return _windows.VScrolledWindow_Create(*args, **kwargs)

    def SetLineCount(*args, **kwargs):
        """SetLineCount(size_t count)"""
        return _windows.VScrolledWindow_SetLineCount(*args, **kwargs)

    def ScrollToLine(*args, **kwargs):
        """ScrollToLine(size_t line) -> bool"""
        return _windows.VScrolledWindow_ScrollToLine(*args, **kwargs)

    def ScrollLines(*args, **kwargs):
        """
        ScrollLines(int lines) -> bool

        If the platform and window class supports it, scrolls the window by
        the given number of lines down, if lines is positive, or up if lines
        is negative.  Returns True if the window was scrolled, False if it was
        already on top/bottom and nothing was done.
        """
        return _windows.VScrolledWindow_ScrollLines(*args, **kwargs)

    def ScrollPages(*args, **kwargs):
        """
        ScrollPages(int pages) -> bool

        If the platform and window class supports it,  scrolls the window by
        the given number of pages down, if pages is positive, or up if pages
        is negative.  Returns True if the window was scrolled, False if it was
        already on top/bottom and nothing was done.
        """
        return _windows.VScrolledWindow_ScrollPages(*args, **kwargs)

    def RefreshLine(*args, **kwargs):
        """RefreshLine(size_t line)"""
        return _windows.VScrolledWindow_RefreshLine(*args, **kwargs)

    def RefreshLines(*args, **kwargs):
        """RefreshLines(size_t from, size_t to)"""
        return _windows.VScrolledWindow_RefreshLines(*args, **kwargs)

    def HitTestXT(*args, **kwargs):
        """
        HitTestXT(int x, int y) -> int

        Test where the given (in client coords) point lies
        """
        return _windows.VScrolledWindow_HitTestXT(*args, **kwargs)

    def HitTest(*args, **kwargs):
        """
        HitTest(Point pt) -> int

        Test where the given (in client coords) point lies
        """
        return _windows.VScrolledWindow_HitTest(*args, **kwargs)

    def RefreshAll(*args, **kwargs):
        """RefreshAll()"""
        return _windows.VScrolledWindow_RefreshAll(*args, **kwargs)

    def GetLineCount(*args, **kwargs):
        """GetLineCount() -> size_t"""
        return _windows.VScrolledWindow_GetLineCount(*args, **kwargs)

    def GetFirstVisibleLine(*args, **kwargs):
        """GetFirstVisibleLine() -> size_t"""
        return _windows.VScrolledWindow_GetFirstVisibleLine(*args, **kwargs)

    def GetLastVisibleLine(*args, **kwargs):
        """GetLastVisibleLine() -> size_t"""
        return _windows.VScrolledWindow_GetLastVisibleLine(*args, **kwargs)

    def IsVisible(*args, **kwargs):
        """IsVisible(size_t line) -> bool"""
        return _windows.VScrolledWindow_IsVisible(*args, **kwargs)


class VScrolledWindowPtr(VScrolledWindow):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = VScrolledWindow
_windows.VScrolledWindow_swigregister(VScrolledWindowPtr)

def PreVScrolledWindow(*args, **kwargs):
    """PreVScrolledWindow() -> VScrolledWindow"""
    val = _windows.new_PreVScrolledWindow(*args, **kwargs)
    val.thisown = 1
    return val

class VListBox(VScrolledWindow):
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxPyVListBox instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """
        __init__(Window parent, int id=ID_ANY, Point pos=DefaultPosition, 
            Size size=DefaultSize, long style=0, String name=VListBoxNameStr) -> VListBox
        """
        newobj = _windows.new_VListBox(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
        self._setOORInfo(self);self._setCallbackInfo(self, VListBox)

    def _setCallbackInfo(*args, **kwargs):
        """_setCallbackInfo(PyObject self, PyObject _class)"""
        return _windows.VListBox__setCallbackInfo(*args, **kwargs)

    def Create(*args, **kwargs):
        """
        Create(Window parent, int id=ID_ANY, Point pos=DefaultPosition, 
            Size size=DefaultSize, long style=0, String name=VListBoxNameStr) -> bool
        """
        return _windows.VListBox_Create(*args, **kwargs)

    def GetItemCount(*args, **kwargs):
        """GetItemCount() -> size_t"""
        return _windows.VListBox_GetItemCount(*args, **kwargs)

    def HasMultipleSelection(*args, **kwargs):
        """HasMultipleSelection() -> bool"""
        return _windows.VListBox_HasMultipleSelection(*args, **kwargs)

    def GetSelection(*args, **kwargs):
        """GetSelection() -> int"""
        return _windows.VListBox_GetSelection(*args, **kwargs)

    def IsCurrent(*args, **kwargs):
        """IsCurrent(size_t item) -> bool"""
        return _windows.VListBox_IsCurrent(*args, **kwargs)

    def IsSelected(*args, **kwargs):
        """IsSelected(size_t item) -> bool"""
        return _windows.VListBox_IsSelected(*args, **kwargs)

    def GetSelectedCount(*args, **kwargs):
        """GetSelectedCount() -> size_t"""
        return _windows.VListBox_GetSelectedCount(*args, **kwargs)

    def GetFirstSelected(*args, **kwargs):
        """GetFirstSelected(unsigned long cookie) -> int"""
        return _windows.VListBox_GetFirstSelected(*args, **kwargs)

    def GetNextSelected(*args, **kwargs):
        """GetNextSelected(unsigned long cookie) -> int"""
        return _windows.VListBox_GetNextSelected(*args, **kwargs)

    def GetMargins(*args, **kwargs):
        """GetMargins() -> Point"""
        return _windows.VListBox_GetMargins(*args, **kwargs)

    def GetSelectionBackground(*args, **kwargs):
        """GetSelectionBackground() -> Colour"""
        return _windows.VListBox_GetSelectionBackground(*args, **kwargs)

    def SetItemCount(*args, **kwargs):
        """SetItemCount(size_t count)"""
        return _windows.VListBox_SetItemCount(*args, **kwargs)

    def Clear(*args, **kwargs):
        """Clear()"""
        return _windows.VListBox_Clear(*args, **kwargs)

    def SetSelection(*args, **kwargs):
        """SetSelection(int selection)"""
        return _windows.VListBox_SetSelection(*args, **kwargs)

    def Select(*args, **kwargs):
        """Select(size_t item, bool select=True) -> bool"""
        return _windows.VListBox_Select(*args, **kwargs)

    def SelectRange(*args, **kwargs):
        """SelectRange(size_t from, size_t to) -> bool"""
        return _windows.VListBox_SelectRange(*args, **kwargs)

    def Toggle(*args, **kwargs):
        """Toggle(size_t item)"""
        return _windows.VListBox_Toggle(*args, **kwargs)

    def SelectAll(*args, **kwargs):
        """SelectAll() -> bool"""
        return _windows.VListBox_SelectAll(*args, **kwargs)

    def DeselectAll(*args, **kwargs):
        """DeselectAll() -> bool"""
        return _windows.VListBox_DeselectAll(*args, **kwargs)

    def SetMargins(*args, **kwargs):
        """SetMargins(Point pt)"""
        return _windows.VListBox_SetMargins(*args, **kwargs)

    def SetMarginsXY(*args, **kwargs):
        """SetMarginsXY(int x, int y)"""
        return _windows.VListBox_SetMarginsXY(*args, **kwargs)

    def SetSelectionBackground(*args, **kwargs):
        """SetSelectionBackground(Colour col)"""
        return _windows.VListBox_SetSelectionBackground(*args, **kwargs)


class VListBoxPtr(VListBox):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = VListBox
_windows.VListBox_swigregister(VListBoxPtr)
VListBoxNameStr = cvar.VListBoxNameStr

def PreVListBox(*args, **kwargs):
    """PreVListBox() -> VListBox"""
    val = _windows.new_PreVListBox(*args, **kwargs)
    val.thisown = 1
    return val

class HtmlListBox(VListBox):
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxPyHtmlListBox instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """
        __init__(Window parent, int id=ID_ANY, Point pos=DefaultPosition, 
            Size size=DefaultSize, long style=0, String name=VListBoxNameStr) -> HtmlListBox
        """
        newobj = _windows.new_HtmlListBox(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
        self._setOORInfo(self);self._setCallbackInfo(self, HtmlListBox)

    def _setCallbackInfo(*args, **kwargs):
        """_setCallbackInfo(PyObject self, PyObject _class)"""
        return _windows.HtmlListBox__setCallbackInfo(*args, **kwargs)

    def Create(*args, **kwargs):
        """
        Create(Window parent, int id=ID_ANY, Point pos=DefaultPosition, 
            Size size=DefaultSize, long style=0, String name=VListBoxNameStr) -> bool
        """
        return _windows.HtmlListBox_Create(*args, **kwargs)

    def RefreshAll(*args, **kwargs):
        """RefreshAll()"""
        return _windows.HtmlListBox_RefreshAll(*args, **kwargs)

    def SetItemCount(*args, **kwargs):
        """SetItemCount(size_t count)"""
        return _windows.HtmlListBox_SetItemCount(*args, **kwargs)


class HtmlListBoxPtr(HtmlListBox):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = HtmlListBox
_windows.HtmlListBox_swigregister(HtmlListBoxPtr)

def PreHtmlListBox(*args, **kwargs):
    """PreHtmlListBox() -> HtmlListBox"""
    val = _windows.new_PreHtmlListBox(*args, **kwargs)
    val.thisown = 1
    return val

#---------------------------------------------------------------------------

class TaskBarIcon(core.EvtHandler):
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxTaskBarIcon instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """__init__() -> TaskBarIcon"""
        newobj = _windows.new_TaskBarIcon(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
    def __del__(self, destroy=_windows.delete_TaskBarIcon):
        """__del__()"""
        try:
            if self.thisown: destroy(self)
        except: pass


class TaskBarIconPtr(TaskBarIcon):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = TaskBarIcon
_windows.TaskBarIcon_swigregister(TaskBarIconPtr)

class TaskBarIconEvent(core.Event):
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxTaskBarIconEvent instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """__init__(wxEventType evtType, TaskBarIcon tbIcon) -> TaskBarIconEvent"""
        newobj = _windows.new_TaskBarIconEvent(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown

class TaskBarIconEventPtr(TaskBarIconEvent):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = TaskBarIconEvent
_windows.TaskBarIconEvent_swigregister(TaskBarIconEventPtr)

wxEVT_TASKBAR_MOVE = _windows.wxEVT_TASKBAR_MOVE
wxEVT_TASKBAR_LEFT_DOWN = _windows.wxEVT_TASKBAR_LEFT_DOWN
wxEVT_TASKBAR_LEFT_UP = _windows.wxEVT_TASKBAR_LEFT_UP
wxEVT_TASKBAR_RIGHT_DOWN = _windows.wxEVT_TASKBAR_RIGHT_DOWN
wxEVT_TASKBAR_RIGHT_UP = _windows.wxEVT_TASKBAR_RIGHT_UP
wxEVT_TASKBAR_LEFT_DCLICK = _windows.wxEVT_TASKBAR_LEFT_DCLICK
wxEVT_TASKBAR_RIGHT_DCLICK = _windows.wxEVT_TASKBAR_RIGHT_DCLICK
EVT_TASKBAR_MOVE = wx.PyEventBinder (         wxEVT_TASKBAR_MOVE )
EVT_TASKBAR_LEFT_DOWN = wx.PyEventBinder (    wxEVT_TASKBAR_LEFT_DOWN )
EVT_TASKBAR_LEFT_UP = wx.PyEventBinder (      wxEVT_TASKBAR_LEFT_UP )
EVT_TASKBAR_RIGHT_DOWN = wx.PyEventBinder (   wxEVT_TASKBAR_RIGHT_DOWN )
EVT_TASKBAR_RIGHT_UP = wx.PyEventBinder (     wxEVT_TASKBAR_RIGHT_UP )
EVT_TASKBAR_LEFT_DCLICK = wx.PyEventBinder (  wxEVT_TASKBAR_LEFT_DCLICK )
EVT_TASKBAR_RIGHT_DCLICK = wx.PyEventBinder ( wxEVT_TASKBAR_RIGHT_DCLICK )

#---------------------------------------------------------------------------

class ColourData(core.Object):
    """This class holds a variety of information related to colour dialogs."""
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxColourData instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """
        __init__() -> ColourData

        Constructor, sets default values.
        """
        newobj = _windows.new_ColourData(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
    def __del__(self, destroy=_windows.delete_ColourData):
        """__del__()"""
        try:
            if self.thisown: destroy(self)
        except: pass

    def GetChooseFull(*args, **kwargs):
        """
        GetChooseFull() -> bool

        Under Windows, determines whether the Windows colour dialog will display
        the full dialog with custom colour selection controls. Has no meaning
        under other platforms.  The default value is true.
        """
        return _windows.ColourData_GetChooseFull(*args, **kwargs)

    def GetColour(*args, **kwargs):
        """
        GetColour() -> Colour

        Gets the colour (pre)selected by the dialog.
        """
        return _windows.ColourData_GetColour(*args, **kwargs)

    def GetCustomColour(*args, **kwargs):
        """
        GetCustomColour(int i) -> Colour

        Gets the i'th custom colour associated with the colour dialog. i should
        be an integer between 0 and 15. The default custom colours are all white.
        """
        return _windows.ColourData_GetCustomColour(*args, **kwargs)

    def SetChooseFull(*args, **kwargs):
        """
        SetChooseFull(int flag)

        Under Windows, tells the Windows colour dialog to display the full dialog
        with custom colour selection controls. Under other platforms, has no effect.
        The default value is true.
        """
        return _windows.ColourData_SetChooseFull(*args, **kwargs)

    def SetColour(*args, **kwargs):
        """
        SetColour(Colour colour)

        Sets the default colour for the colour dialog.  The default colour is black.
        """
        return _windows.ColourData_SetColour(*args, **kwargs)

    def SetCustomColour(*args, **kwargs):
        """
        SetCustomColour(int i, Colour colour)

        Sets the i'th custom colour for the colour dialog. i should be an integer
        between 0 and 15. The default custom colours are all white.
        """
        return _windows.ColourData_SetCustomColour(*args, **kwargs)


class ColourDataPtr(ColourData):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = ColourData
_windows.ColourData_swigregister(ColourDataPtr)
FileSelectorPromptStr = cvar.FileSelectorPromptStr
DirSelectorPromptStr = cvar.DirSelectorPromptStr
DirDialogNameStr = cvar.DirDialogNameStr
FileSelectorDefaultWildcardStr = cvar.FileSelectorDefaultWildcardStr
GetTextFromUserPromptStr = cvar.GetTextFromUserPromptStr
MessageBoxCaptionStr = cvar.MessageBoxCaptionStr

class ColourDialog(Dialog):
    """This class represents the colour chooser dialog."""
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxColourDialog instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """
        __init__(Window parent, ColourData data=None) -> ColourDialog

        Constructor. Pass a parent window, and optionally a ColourData, which
        will be copied to the colour dialog's internal ColourData instance.
        """
        newobj = _windows.new_ColourDialog(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
        self._setOORInfo(self)

    def GetColourData(*args, **kwargs):
        """
        GetColourData() -> ColourData

        Returns a reference to the ColourData used by the dialog.
        """
        return _windows.ColourDialog_GetColourData(*args, **kwargs)


class ColourDialogPtr(ColourDialog):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = ColourDialog
_windows.ColourDialog_swigregister(ColourDialogPtr)

class DirDialog(Dialog):
    """This class represents the directory chooser dialog."""
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxDirDialog instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """
        __init__(Window parent, String message=DirSelectorPromptStr, 
            String defaultPath=EmptyString, long style=0, 
            Point pos=DefaultPosition, Size size=DefaultSize, 
            String name=DirDialogNameStr) -> DirDialog

        Constructor.  Use ShowModal method to show the dialog.
        """
        newobj = _windows.new_DirDialog(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
        self._setOORInfo(self)

    def GetPath(*args, **kwargs):
        """
        GetPath() -> String

        Returns the default or user-selected path.
        """
        return _windows.DirDialog_GetPath(*args, **kwargs)

    def GetMessage(*args, **kwargs):
        """
        GetMessage() -> String

        Returns the message that will be displayed on the dialog.
        """
        return _windows.DirDialog_GetMessage(*args, **kwargs)

    def GetStyle(*args, **kwargs):
        """
        GetStyle() -> long

        Returns the dialog style.
        """
        return _windows.DirDialog_GetStyle(*args, **kwargs)

    def SetMessage(*args, **kwargs):
        """
        SetMessage(String message)

        Sets the message that will be displayed on the dialog.
        """
        return _windows.DirDialog_SetMessage(*args, **kwargs)

    def SetPath(*args, **kwargs):
        """
        SetPath(String path)

        Sets the default path.
        """
        return _windows.DirDialog_SetPath(*args, **kwargs)


class DirDialogPtr(DirDialog):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = DirDialog
_windows.DirDialog_swigregister(DirDialogPtr)

class FileDialog(Dialog):
    """This class represents the file chooser dialog."""
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxFileDialog instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """
        __init__(Window parent, String message=FileSelectorPromptStr, 
            String defaultDir=EmptyString, String defaultFile=EmptyString, 
            String wildcard=FileSelectorDefaultWildcardStr, 
            long style=0, Point pos=DefaultPosition) -> FileDialog

        Constructor.  Use ShowModal method to show the dialog.
        """
        newobj = _windows.new_FileDialog(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
        self._setOORInfo(self)

    def SetMessage(*args, **kwargs):
        """
        SetMessage(String message)

        Sets the message that will be displayed on the dialog.
        """
        return _windows.FileDialog_SetMessage(*args, **kwargs)

    def SetPath(*args, **kwargs):
        """
        SetPath(String path)

        Sets the path (the combined directory and filename that will
        be returned when the dialog is dismissed).
        """
        return _windows.FileDialog_SetPath(*args, **kwargs)

    def SetDirectory(*args, **kwargs):
        """
        SetDirectory(String dir)

        Sets the default directory.
        """
        return _windows.FileDialog_SetDirectory(*args, **kwargs)

    def SetFilename(*args, **kwargs):
        """
        SetFilename(String name)

        Sets the default filename.
        """
        return _windows.FileDialog_SetFilename(*args, **kwargs)

    def SetWildcard(*args, **kwargs):
        """
        SetWildcard(String wildCard)

        Sets the wildcard, which can contain multiple file types, for example:
            "BMP files (*.bmp)|*.bmp|GIF files (*.gif)|*.gif"
        """
        return _windows.FileDialog_SetWildcard(*args, **kwargs)

    def SetStyle(*args, **kwargs):
        """
        SetStyle(long style)

        Sets the dialog style.
        """
        return _windows.FileDialog_SetStyle(*args, **kwargs)

    def SetFilterIndex(*args, **kwargs):
        """
        SetFilterIndex(int filterIndex)

        Sets the default filter index, starting from zero.
        """
        return _windows.FileDialog_SetFilterIndex(*args, **kwargs)

    def GetMessage(*args, **kwargs):
        """
        GetMessage() -> String

        Returns the message that will be displayed on the dialog.
        """
        return _windows.FileDialog_GetMessage(*args, **kwargs)

    def GetPath(*args, **kwargs):
        """
        GetPath() -> String

        Returns the full path (directory and filename) of the selected file.
        """
        return _windows.FileDialog_GetPath(*args, **kwargs)

    def GetDirectory(*args, **kwargs):
        """
        GetDirectory() -> String

        Returns the default directory.
        """
        return _windows.FileDialog_GetDirectory(*args, **kwargs)

    def GetFilename(*args, **kwargs):
        """
        GetFilename() -> String

        Returns the default filename.
        """
        return _windows.FileDialog_GetFilename(*args, **kwargs)

    def GetWildcard(*args, **kwargs):
        """
        GetWildcard() -> String

        Returns the file dialog wildcard.
        """
        return _windows.FileDialog_GetWildcard(*args, **kwargs)

    def GetStyle(*args, **kwargs):
        """
        GetStyle() -> long

        Returns the dialog style.
        """
        return _windows.FileDialog_GetStyle(*args, **kwargs)

    def GetFilterIndex(*args, **kwargs):
        """
        GetFilterIndex() -> int

        Returns the index into the list of filters supplied, optionally, in
        the wildcard parameter. Before the dialog is shown, this is the index
        which will be used when the dialog is first displayed. After the dialog
        is shown, this is the index selected by the user.
        """
        return _windows.FileDialog_GetFilterIndex(*args, **kwargs)

    def GetFilenames(*args, **kwargs):
        """
        GetFilenames() -> PyObject

        Returns a list of filenames chosen in the dialog.  This function should
        only be used with the dialogs which have wx.MULTIPLE style, use
        GetFilename for the others.
        """
        return _windows.FileDialog_GetFilenames(*args, **kwargs)

    def GetPaths(*args, **kwargs):
        """
        GetPaths() -> PyObject

        Fills the array paths with the full paths of the files chosen. This
        function should only be used with the dialogs which have wx.MULTIPLE style,
        use GetPath for the others.
        """
        return _windows.FileDialog_GetPaths(*args, **kwargs)


class FileDialogPtr(FileDialog):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = FileDialog
_windows.FileDialog_swigregister(FileDialogPtr)

CHOICEDLG_STYLE = _windows.CHOICEDLG_STYLE
class MultiChoiceDialog(Dialog):
    """A simple dialog with a multi selection listbox."""
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxMultiChoiceDialog instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """
        __init__(Window parent, String message, String caption,
            List choices=[], long style=CHOICEDLG_STYLE,
            Point pos=DefaultPosition) -> MultiChoiceDialog

        Constructor.  Use ShowModal method to show the dialog.
        """
        newobj = _windows.new_MultiChoiceDialog(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
        self._setOORInfo(self)

    def SetSelections(*args, **kwargs):
        """
        SetSelections(List selections)

        Specify the items in the list that shoudl be selected, using a list of integers.
        """
        return _windows.MultiChoiceDialog_SetSelections(*args, **kwargs)

    def GetSelections(*args, **kwargs):
        """
        GetSelections() -> [selections]

        Returns a list of integers representing the items that are selected.
        """
        return _windows.MultiChoiceDialog_GetSelections(*args, **kwargs)


class MultiChoiceDialogPtr(MultiChoiceDialog):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = MultiChoiceDialog
_windows.MultiChoiceDialog_swigregister(MultiChoiceDialogPtr)

class SingleChoiceDialog(Dialog):
    """A simple dialog with a single selection listbox."""
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxSingleChoiceDialog instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """
        __init__(Window parent, String message, String caption,
            List choices=[], long style=CHOICEDLG_STYLE,
            Point pos=DefaultPosition) -> SingleChoiceDialog

        Constructor.  Use ShowModal method to show the dialog.
        """
        newobj = _windows.new_SingleChoiceDialog(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
        self._setOORInfo(self)

    def GetSelection(*args, **kwargs):
        """
        GetSelection() -> int

        Get the index of teh currently selected item.
        """
        return _windows.SingleChoiceDialog_GetSelection(*args, **kwargs)

    def GetStringSelection(*args, **kwargs):
        """
        GetStringSelection() -> String

        Returns the string value of the currently selected item
        """
        return _windows.SingleChoiceDialog_GetStringSelection(*args, **kwargs)

    def SetSelection(*args, **kwargs):
        """
        SetSelection(int sel)

        Set the current selected item to sel
        """
        return _windows.SingleChoiceDialog_SetSelection(*args, **kwargs)


class SingleChoiceDialogPtr(SingleChoiceDialog):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = SingleChoiceDialog
_windows.SingleChoiceDialog_swigregister(SingleChoiceDialogPtr)

class TextEntryDialog(Dialog):
    """A dialog with text control, [ok] and [cancel] buttons"""
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxTextEntryDialog instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """
        __init__(Window parent, String message, String caption=GetTextFromUserPromptStr, 
            String defaultValue=EmptyString, 
            long style=wxOK|wxCANCEL|wxCENTRE, Point pos=DefaultPosition) -> TextEntryDialog

        Constructor.  Use ShowModal method to show the dialog.
        """
        newobj = _windows.new_TextEntryDialog(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
        self._setOORInfo(self)

    def GetValue(*args, **kwargs):
        """
        GetValue() -> String

        Returns the text that the user has entered if the user has pressed OK,
        or the original value if the user has pressed Cancel.
        """
        return _windows.TextEntryDialog_GetValue(*args, **kwargs)

    def SetValue(*args, **kwargs):
        """
        SetValue(String value)

        Sets the default text value.
        """
        return _windows.TextEntryDialog_SetValue(*args, **kwargs)


class TextEntryDialogPtr(TextEntryDialog):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = TextEntryDialog
_windows.TextEntryDialog_swigregister(TextEntryDialogPtr)

class FontData(core.Object):
    """This class holds a variety of information related to font dialogs."""
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxFontData instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """
        __init__() -> FontData

        This class holds a variety of information related to font dialogs.
        """
        newobj = _windows.new_FontData(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
    def __del__(self, destroy=_windows.delete_FontData):
        """__del__()"""
        try:
            if self.thisown: destroy(self)
        except: pass

    def EnableEffects(*args, **kwargs):
        """
        EnableEffects(bool enable)

        Enables or disables 'effects' under MS Windows only. This refers
        to the controls for manipulating colour, strikeout and underline
        properties.  The default value is true.
        """
        return _windows.FontData_EnableEffects(*args, **kwargs)

    def GetAllowSymbols(*args, **kwargs):
        """
        GetAllowSymbols() -> bool

        Under MS Windows, returns a flag determining whether symbol fonts can be
        selected. Has no effect on other platforms. The default value is true.
        """
        return _windows.FontData_GetAllowSymbols(*args, **kwargs)

    def GetColour(*args, **kwargs):
        """
        GetColour() -> Colour

        Gets the colour associated with the font dialog. The default value is black.
        """
        return _windows.FontData_GetColour(*args, **kwargs)

    def GetChosenFont(*args, **kwargs):
        """
        GetChosenFont() -> Font

        Gets the font chosen by the user.
        """
        return _windows.FontData_GetChosenFont(*args, **kwargs)

    def GetEnableEffects(*args, **kwargs):
        """
        GetEnableEffects() -> bool

        Determines whether 'effects' are enabled under Windows.
        """
        return _windows.FontData_GetEnableEffects(*args, **kwargs)

    def GetInitialFont(*args, **kwargs):
        """
        GetInitialFont() -> Font

        Gets the font that will be initially used by the font dialog. This should have
        previously been set by the application.
        """
        return _windows.FontData_GetInitialFont(*args, **kwargs)

    def GetShowHelp(*args, **kwargs):
        """
        GetShowHelp() -> bool

        Returns true if the Help button will be shown (Windows only).  The default
        value is false.
        """
        return _windows.FontData_GetShowHelp(*args, **kwargs)

    def SetAllowSymbols(*args, **kwargs):
        """
        SetAllowSymbols(bool allowSymbols)

        Under MS Windows, determines whether symbol fonts can be selected. Has no
        effect on other platforms.  The default value is true.
        """
        return _windows.FontData_SetAllowSymbols(*args, **kwargs)

    def SetChosenFont(*args, **kwargs):
        """
        SetChosenFont(Font font)

        Sets the font that will be returned to the user (for internal use only).
        """
        return _windows.FontData_SetChosenFont(*args, **kwargs)

    def SetColour(*args, **kwargs):
        """
        SetColour(Colour colour)

        Sets the colour that will be used for the font foreground colour.  The default
        colour is black.
        """
        return _windows.FontData_SetColour(*args, **kwargs)

    def SetInitialFont(*args, **kwargs):
        """
        SetInitialFont(Font font)

        Sets the font that will be initially used by the font dialog.
        """
        return _windows.FontData_SetInitialFont(*args, **kwargs)

    def SetRange(*args, **kwargs):
        """
        SetRange(int min, int max)

        Sets the valid range for the font point size (Windows only).  The default is
        0, 0 (unrestricted range).
        """
        return _windows.FontData_SetRange(*args, **kwargs)

    def SetShowHelp(*args, **kwargs):
        """
        SetShowHelp(bool showHelp)

        Determines whether the Help button will be displayed in the font dialog
        (Windows only).  The default value is false.
        """
        return _windows.FontData_SetShowHelp(*args, **kwargs)


class FontDataPtr(FontData):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = FontData
_windows.FontData_swigregister(FontDataPtr)

class FontDialog(Dialog):
    """This class represents the font chooser dialog."""
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxFontDialog instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """
        __init__(Window parent, FontData data) -> FontDialog

        Constructor. Pass a parent window and the FontData object to be
        used to initialize the dialog controls.
        """
        newobj = _windows.new_FontDialog(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
        self._setOORInfo(self)

    def GetFontData(*args, **kwargs):
        """
        GetFontData() -> FontData

        Returns a reference to the internal FontData used by the FontDialog.
        """
        return _windows.FontDialog_GetFontData(*args, **kwargs)


class FontDialogPtr(FontDialog):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = FontDialog
_windows.FontDialog_swigregister(FontDialogPtr)

class MessageDialog(Dialog):
    """
    This class provides a dialog that shows a single or multi-line message, with
    a choice of OK, Yes, No and Cancel buttons.
    """
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxMessageDialog instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """
        __init__(Window parent, String message, String caption=MessageBoxCaptionStr, 
            long style=wxOK|wxCANCEL|wxCENTRE, 
            Point pos=DefaultPosition) -> MessageDialog

        This class provides a dialog that shows a single or multi-line message, with
        a choice of OK, Yes, No and Cancel buttons.
        """
        newobj = _windows.new_MessageDialog(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
        self._setOORInfo(self)


class MessageDialogPtr(MessageDialog):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = MessageDialog
_windows.MessageDialog_swigregister(MessageDialogPtr)

class ProgressDialog(Frame):
    """
    A dialog that shows a short message and a progress bar. Optionally, it can
    display an ABORT button.
    """
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxProgressDialog instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """
        __init__(String title, String message, int maximum=100, Window parent=None, 
            int style=wxPD_AUTO_HIDE|wxPD_APP_MODAL) -> ProgressDialog

        Constructor. Creates the dialog, displays it and disables user input for other
        windows, or, if wxPD_APP_MODAL flag is not given, for its parent window only.
        """
        newobj = _windows.new_ProgressDialog(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
        self._setOORInfo(self)

    def Update(*args, **kwargs):
        """
        Update(int value, String newmsg=EmptyString) -> bool

        Updates the dialog, setting the progress bar to the new value and, if given
        changes the message above it. Returns true unless the Cancel button has been
        pressed.

        If false is returned, the application can either immediately destroy the
        dialog or ask the user for the confirmation and if the abort is not confirmed
        the dialog may be resumed with Resume function.
        """
        return _windows.ProgressDialog_Update(*args, **kwargs)

    def Resume(*args, **kwargs):
        """
        Resume()

        Can be used to continue with the dialog, after the user had chosen to abort.
        """
        return _windows.ProgressDialog_Resume(*args, **kwargs)


class ProgressDialogPtr(ProgressDialog):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = ProgressDialog
_windows.ProgressDialog_swigregister(ProgressDialogPtr)

FR_DOWN = _windows.FR_DOWN
FR_WHOLEWORD = _windows.FR_WHOLEWORD
FR_MATCHCASE = _windows.FR_MATCHCASE
FR_REPLACEDIALOG = _windows.FR_REPLACEDIALOG
FR_NOUPDOWN = _windows.FR_NOUPDOWN
FR_NOMATCHCASE = _windows.FR_NOMATCHCASE
FR_NOWHOLEWORD = _windows.FR_NOWHOLEWORD
wxEVT_COMMAND_FIND = _windows.wxEVT_COMMAND_FIND
wxEVT_COMMAND_FIND_NEXT = _windows.wxEVT_COMMAND_FIND_NEXT
wxEVT_COMMAND_FIND_REPLACE = _windows.wxEVT_COMMAND_FIND_REPLACE
wxEVT_COMMAND_FIND_REPLACE_ALL = _windows.wxEVT_COMMAND_FIND_REPLACE_ALL
wxEVT_COMMAND_FIND_CLOSE = _windows.wxEVT_COMMAND_FIND_CLOSE
EVT_FIND = wx.PyEventBinder( wxEVT_COMMAND_FIND, 1 )
EVT_FIND_NEXT = wx.PyEventBinder( wxEVT_COMMAND_FIND_NEXT, 1 )
EVT_FIND_REPLACE = wx.PyEventBinder( wxEVT_COMMAND_FIND_REPLACE, 1 )
EVT_FIND_REPLACE_ALL = wx.PyEventBinder( wxEVT_COMMAND_FIND_REPLACE_ALL, 1 )
EVT_FIND_CLOSE = wx.PyEventBinder( wxEVT_COMMAND_FIND_CLOSE, 1 )

# For backwards compatibility.  Should they be removed?
EVT_COMMAND_FIND             = EVT_FIND 
EVT_COMMAND_FIND_NEXT        = EVT_FIND_NEXT
EVT_COMMAND_FIND_REPLACE     = EVT_FIND_REPLACE
EVT_COMMAND_FIND_REPLACE_ALL = EVT_FIND_REPLACE_ALL
EVT_COMMAND_FIND_CLOSE       = EVT_FIND_CLOSE        

class FindDialogEvent(core.CommandEvent):
    """Events for the FindReplaceDialog"""
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxFindDialogEvent instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """
        __init__(wxEventType commandType=wxEVT_NULL, int id=0) -> FindDialogEvent

        Events for the FindReplaceDialog
        """
        newobj = _windows.new_FindDialogEvent(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
    def GetFlags(*args, **kwargs):
        """
        GetFlags() -> int

        Get the currently selected flags: this is the combination of
        wx.FR_DOWN, wx.FR_WHOLEWORD and wx.FR_MATCHCASE flags.
        """
        return _windows.FindDialogEvent_GetFlags(*args, **kwargs)

    def GetFindString(*args, **kwargs):
        """
        GetFindString() -> String

        Return the string to find (never empty).
        """
        return _windows.FindDialogEvent_GetFindString(*args, **kwargs)

    def GetReplaceString(*args, **kwargs):
        """
        GetReplaceString() -> String

        Return the string to replace the search string with (only
        for replace and replace all events).
        """
        return _windows.FindDialogEvent_GetReplaceString(*args, **kwargs)

    def GetDialog(*args, **kwargs):
        """
        GetDialog() -> FindReplaceDialog

        Return the pointer to the dialog which generated this event.
        """
        return _windows.FindDialogEvent_GetDialog(*args, **kwargs)

    def SetFlags(*args, **kwargs):
        """SetFlags(int flags)"""
        return _windows.FindDialogEvent_SetFlags(*args, **kwargs)

    def SetFindString(*args, **kwargs):
        """SetFindString(String str)"""
        return _windows.FindDialogEvent_SetFindString(*args, **kwargs)

    def SetReplaceString(*args, **kwargs):
        """SetReplaceString(String str)"""
        return _windows.FindDialogEvent_SetReplaceString(*args, **kwargs)


class FindDialogEventPtr(FindDialogEvent):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = FindDialogEvent
_windows.FindDialogEvent_swigregister(FindDialogEventPtr)

class FindReplaceData(core.Object):
    """
    FindReplaceData holds the data for FindReplaceDialog. It is used to initialize
    the dialog with the default values and will keep the last values from the
    dialog when it is closed. It is also updated each time a wxFindDialogEvent is
    generated so instead of using the wxFindDialogEvent methods you can also
    directly query this object.

    Note that all SetXXX() methods may only be called before showing the dialog
    and calling them has no effect later.

     Flags
        wxFR_DOWN:          downward search/replace selected (otherwise, upwards)

        wxFR_WHOLEWORD:     whole word search/replace selected

        wxFR_MATCHCASE:     case sensitive search/replace selected (otherwise,
                            case insensitive)

    """
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxFindReplaceData instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """
        __init__(int flags=0) -> FindReplaceData

        Constuctor initializes the flags to default value (0).
        """
        newobj = _windows.new_FindReplaceData(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
    def __del__(self, destroy=_windows.delete_FindReplaceData):
        """__del__()"""
        try:
            if self.thisown: destroy(self)
        except: pass

    def GetFindString(*args, **kwargs):
        """
        GetFindString() -> String

        Get the string to find.
        """
        return _windows.FindReplaceData_GetFindString(*args, **kwargs)

    def GetReplaceString(*args, **kwargs):
        """
        GetReplaceString() -> String

        Get the replacement string.
        """
        return _windows.FindReplaceData_GetReplaceString(*args, **kwargs)

    def GetFlags(*args, **kwargs):
        """
        GetFlags() -> int

        Get the combination of flag values.
        """
        return _windows.FindReplaceData_GetFlags(*args, **kwargs)

    def SetFlags(*args, **kwargs):
        """
        SetFlags(int flags)

        Set the flags to use to initialize the controls of the dialog.
        """
        return _windows.FindReplaceData_SetFlags(*args, **kwargs)

    def SetFindString(*args, **kwargs):
        """
        SetFindString(String str)

        Set the string to find (used as initial value by the dialog).
        """
        return _windows.FindReplaceData_SetFindString(*args, **kwargs)

    def SetReplaceString(*args, **kwargs):
        """
        SetReplaceString(String str)

        Set the replacement string (used as initial value by the dialog).
        """
        return _windows.FindReplaceData_SetReplaceString(*args, **kwargs)


class FindReplaceDataPtr(FindReplaceData):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = FindReplaceData
_windows.FindReplaceData_swigregister(FindReplaceDataPtr)

class FindReplaceDialog(Dialog):
    """
    FindReplaceDialog is a standard modeless dialog which is used to allow the
    user to search for some text (and possibly replace it with something
    else). The actual searching is supposed to be done in the owner window which
    is the parent of this dialog. Note that it means that unlike for the other
    standard dialogs this one must have a parent window. Also note that there is
    no way to use this dialog in a modal way; it is always, by design and
    implementation, modeless.
    """
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxFindReplaceDialog instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """
        __init__(Window parent, FindReplaceData data, String title, 
            int style=0) -> FindReplaceDialog

        Create a FindReplaceDialog.  The parent and data parameters must be
        non-None.  Use Show to display the dialog.
        """
        newobj = _windows.new_FindReplaceDialog(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
        self._setOORInfo(self)

    def Create(*args, **kwargs):
        """
        Create(Window parent, FindReplaceData data, String title, 
            int style=0) -> bool

        Create the dialog, for 2-phase create.
        """
        return _windows.FindReplaceDialog_Create(*args, **kwargs)

    def GetData(*args, **kwargs):
        """
        GetData() -> FindReplaceData

        Get the FindReplaceData object used by this dialog.
        """
        return _windows.FindReplaceDialog_GetData(*args, **kwargs)

    def SetData(*args, **kwargs):
        """
        SetData(FindReplaceData data)

        Set the FindReplaceData object used by this dialog.
        """
        return _windows.FindReplaceDialog_SetData(*args, **kwargs)


class FindReplaceDialogPtr(FindReplaceDialog):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = FindReplaceDialog
_windows.FindReplaceDialog_swigregister(FindReplaceDialogPtr)

def PreFindReplaceDialog(*args, **kwargs):
    """
    PreFindReplaceDialog() -> FindReplaceDialog

    Precreate a FindReplaceDialog for 2-phase creation
    """
    val = _windows.new_PreFindReplaceDialog(*args, **kwargs)
    val.thisown = 1
    return val

#---------------------------------------------------------------------------

IDM_WINDOWTILE = _windows.IDM_WINDOWTILE
IDM_WINDOWTILEHOR = _windows.IDM_WINDOWTILEHOR
IDM_WINDOWCASCADE = _windows.IDM_WINDOWCASCADE
IDM_WINDOWICONS = _windows.IDM_WINDOWICONS
IDM_WINDOWNEXT = _windows.IDM_WINDOWNEXT
IDM_WINDOWTILEVERT = _windows.IDM_WINDOWTILEVERT
FIRST_MDI_CHILD = _windows.FIRST_MDI_CHILD
LAST_MDI_CHILD = _windows.LAST_MDI_CHILD
class MDIParentFrame(Frame):
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxMDIParentFrame instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """
        __init__(Window parent, int id, String title, Point pos=DefaultPosition, 
            Size size=DefaultSize, long style=wxDEFAULT_FRAME_STYLE|wxVSCROLL|wxHSCROLL, 
            String name=FrameNameStr) -> MDIParentFrame
        """
        newobj = _windows.new_MDIParentFrame(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
        self._setOORInfo(self)

    def Create(*args, **kwargs):
        """
        Create(Window parent, int id, String title, Point pos=DefaultPosition, 
            Size size=DefaultSize, long style=wxDEFAULT_FRAME_STYLE|wxVSCROLL|wxHSCROLL, 
            String name=FrameNameStr) -> bool
        """
        return _windows.MDIParentFrame_Create(*args, **kwargs)

    def ActivateNext(*args, **kwargs):
        """ActivateNext()"""
        return _windows.MDIParentFrame_ActivateNext(*args, **kwargs)

    def ActivatePrevious(*args, **kwargs):
        """ActivatePrevious()"""
        return _windows.MDIParentFrame_ActivatePrevious(*args, **kwargs)

    def ArrangeIcons(*args, **kwargs):
        """ArrangeIcons()"""
        return _windows.MDIParentFrame_ArrangeIcons(*args, **kwargs)

    def Cascade(*args, **kwargs):
        """Cascade()"""
        return _windows.MDIParentFrame_Cascade(*args, **kwargs)

    def GetActiveChild(*args, **kwargs):
        """GetActiveChild() -> MDIChildFrame"""
        return _windows.MDIParentFrame_GetActiveChild(*args, **kwargs)

    def GetClientWindow(*args, **kwargs):
        """GetClientWindow() -> MDIClientWindow"""
        return _windows.MDIParentFrame_GetClientWindow(*args, **kwargs)

    def GetToolBar(*args, **kwargs):
        """GetToolBar() -> Window"""
        return _windows.MDIParentFrame_GetToolBar(*args, **kwargs)

    def Tile(*args, **kwargs):
        """Tile()"""
        return _windows.MDIParentFrame_Tile(*args, **kwargs)


class MDIParentFramePtr(MDIParentFrame):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = MDIParentFrame
_windows.MDIParentFrame_swigregister(MDIParentFramePtr)

def PreMDIParentFrame(*args, **kwargs):
    """PreMDIParentFrame() -> MDIParentFrame"""
    val = _windows.new_PreMDIParentFrame(*args, **kwargs)
    val.thisown = 1
    return val

class MDIChildFrame(Frame):
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxMDIChildFrame instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """
        __init__(MDIParentFrame parent, int id, String title, Point pos=DefaultPosition, 
            Size size=DefaultSize, long style=DEFAULT_FRAME_STYLE, 
            String name=FrameNameStr) -> MDIChildFrame
        """
        newobj = _windows.new_MDIChildFrame(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
        self._setOORInfo(self)

    def Create(*args, **kwargs):
        """
        Create(MDIParentFrame parent, int id, String title, Point pos=DefaultPosition, 
            Size size=DefaultSize, long style=DEFAULT_FRAME_STYLE, 
            String name=FrameNameStr) -> bool
        """
        return _windows.MDIChildFrame_Create(*args, **kwargs)

    def Activate(*args, **kwargs):
        """Activate()"""
        return _windows.MDIChildFrame_Activate(*args, **kwargs)

    def Maximize(*args, **kwargs):
        """Maximize(bool maximize)"""
        return _windows.MDIChildFrame_Maximize(*args, **kwargs)

    def Restore(*args, **kwargs):
        """Restore()"""
        return _windows.MDIChildFrame_Restore(*args, **kwargs)


class MDIChildFramePtr(MDIChildFrame):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = MDIChildFrame
_windows.MDIChildFrame_swigregister(MDIChildFramePtr)

def PreMDIChildFrame(*args, **kwargs):
    """PreMDIChildFrame() -> MDIChildFrame"""
    val = _windows.new_PreMDIChildFrame(*args, **kwargs)
    val.thisown = 1
    return val

class MDIClientWindow(core.Window):
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxMDIClientWindow instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """__init__(MDIParentFrame parent, long style=0) -> MDIClientWindow"""
        newobj = _windows.new_MDIClientWindow(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
        self._setOORInfo(self)

    def Create(*args, **kwargs):
        """Create(MDIParentFrame parent, long style=0) -> bool"""
        return _windows.MDIClientWindow_Create(*args, **kwargs)


class MDIClientWindowPtr(MDIClientWindow):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = MDIClientWindow
_windows.MDIClientWindow_swigregister(MDIClientWindowPtr)

def PreMDIClientWindow(*args, **kwargs):
    """PreMDIClientWindow() -> MDIClientWindow"""
    val = _windows.new_PreMDIClientWindow(*args, **kwargs)
    val.thisown = 1
    return val

#---------------------------------------------------------------------------

class PyWindow(core.Window):
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxPyWindow instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """
        __init__(Window parent, int id, Point pos=DefaultPosition, Size size=DefaultSize, 
            long style=0, String name=PanelNameStr) -> PyWindow
        """
        newobj = _windows.new_PyWindow(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
        self._setOORInfo(self); self._setCallbackInfo(self, PyWindow)

    def _setCallbackInfo(*args, **kwargs):
        """_setCallbackInfo(PyObject self, PyObject _class)"""
        return _windows.PyWindow__setCallbackInfo(*args, **kwargs)

    def base_DoMoveWindow(*args, **kwargs):
        """base_DoMoveWindow(int x, int y, int width, int height)"""
        return _windows.PyWindow_base_DoMoveWindow(*args, **kwargs)

    def base_DoSetSize(*args, **kwargs):
        """base_DoSetSize(int x, int y, int width, int height, int sizeFlags=SIZE_AUTO)"""
        return _windows.PyWindow_base_DoSetSize(*args, **kwargs)

    def base_DoSetClientSize(*args, **kwargs):
        """base_DoSetClientSize(int width, int height)"""
        return _windows.PyWindow_base_DoSetClientSize(*args, **kwargs)

    def base_DoSetVirtualSize(*args, **kwargs):
        """base_DoSetVirtualSize(int x, int y)"""
        return _windows.PyWindow_base_DoSetVirtualSize(*args, **kwargs)

    def base_DoGetSize(*args, **kwargs):
        """base_DoGetSize() -> (width, height)"""
        return _windows.PyWindow_base_DoGetSize(*args, **kwargs)

    def base_DoGetClientSize(*args, **kwargs):
        """base_DoGetClientSize() -> (width, height)"""
        return _windows.PyWindow_base_DoGetClientSize(*args, **kwargs)

    def base_DoGetPosition(*args, **kwargs):
        """base_DoGetPosition() -> (x,y)"""
        return _windows.PyWindow_base_DoGetPosition(*args, **kwargs)

    def base_DoGetVirtualSize(*args, **kwargs):
        """base_DoGetVirtualSize() -> Size"""
        return _windows.PyWindow_base_DoGetVirtualSize(*args, **kwargs)

    def base_DoGetBestSize(*args, **kwargs):
        """base_DoGetBestSize() -> Size"""
        return _windows.PyWindow_base_DoGetBestSize(*args, **kwargs)

    def base_InitDialog(*args, **kwargs):
        """base_InitDialog()"""
        return _windows.PyWindow_base_InitDialog(*args, **kwargs)

    def base_TransferDataToWindow(*args, **kwargs):
        """base_TransferDataToWindow() -> bool"""
        return _windows.PyWindow_base_TransferDataToWindow(*args, **kwargs)

    def base_TransferDataFromWindow(*args, **kwargs):
        """base_TransferDataFromWindow() -> bool"""
        return _windows.PyWindow_base_TransferDataFromWindow(*args, **kwargs)

    def base_Validate(*args, **kwargs):
        """base_Validate() -> bool"""
        return _windows.PyWindow_base_Validate(*args, **kwargs)

    def base_AcceptsFocus(*args, **kwargs):
        """base_AcceptsFocus() -> bool"""
        return _windows.PyWindow_base_AcceptsFocus(*args, **kwargs)

    def base_AcceptsFocusFromKeyboard(*args, **kwargs):
        """base_AcceptsFocusFromKeyboard() -> bool"""
        return _windows.PyWindow_base_AcceptsFocusFromKeyboard(*args, **kwargs)

    def base_GetMaxSize(*args, **kwargs):
        """base_GetMaxSize() -> Size"""
        return _windows.PyWindow_base_GetMaxSize(*args, **kwargs)

    def base_AddChild(*args, **kwargs):
        """base_AddChild(Window child)"""
        return _windows.PyWindow_base_AddChild(*args, **kwargs)

    def base_RemoveChild(*args, **kwargs):
        """base_RemoveChild(Window child)"""
        return _windows.PyWindow_base_RemoveChild(*args, **kwargs)

    def base_ShouldInheritColours(*args, **kwargs):
        """base_ShouldInheritColours() -> bool"""
        return _windows.PyWindow_base_ShouldInheritColours(*args, **kwargs)

    def base_ApplyParentThemeBackground(*args, **kwargs):
        """base_ApplyParentThemeBackground(Colour c)"""
        return _windows.PyWindow_base_ApplyParentThemeBackground(*args, **kwargs)


class PyWindowPtr(PyWindow):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = PyWindow
_windows.PyWindow_swigregister(PyWindowPtr)

def PrePyWindow(*args, **kwargs):
    """PrePyWindow() -> PyWindow"""
    val = _windows.new_PrePyWindow(*args, **kwargs)
    val.thisown = 1
    return val

class PyPanel(Panel):
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxPyPanel instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """
        __init__(Window parent, int id, Point pos=DefaultPosition, Size size=DefaultSize, 
            long style=0, String name=PanelNameStr) -> PyPanel
        """
        newobj = _windows.new_PyPanel(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
        self._setOORInfo(self); self._setCallbackInfo(self, PyPanel)

    def _setCallbackInfo(*args, **kwargs):
        """_setCallbackInfo(PyObject self, PyObject _class)"""
        return _windows.PyPanel__setCallbackInfo(*args, **kwargs)

    def base_DoMoveWindow(*args, **kwargs):
        """base_DoMoveWindow(int x, int y, int width, int height)"""
        return _windows.PyPanel_base_DoMoveWindow(*args, **kwargs)

    def base_DoSetSize(*args, **kwargs):
        """base_DoSetSize(int x, int y, int width, int height, int sizeFlags=SIZE_AUTO)"""
        return _windows.PyPanel_base_DoSetSize(*args, **kwargs)

    def base_DoSetClientSize(*args, **kwargs):
        """base_DoSetClientSize(int width, int height)"""
        return _windows.PyPanel_base_DoSetClientSize(*args, **kwargs)

    def base_DoSetVirtualSize(*args, **kwargs):
        """base_DoSetVirtualSize(int x, int y)"""
        return _windows.PyPanel_base_DoSetVirtualSize(*args, **kwargs)

    def base_DoGetSize(*args, **kwargs):
        """base_DoGetSize() -> (width, height)"""
        return _windows.PyPanel_base_DoGetSize(*args, **kwargs)

    def base_DoGetClientSize(*args, **kwargs):
        """base_DoGetClientSize() -> (width, height)"""
        return _windows.PyPanel_base_DoGetClientSize(*args, **kwargs)

    def base_DoGetPosition(*args, **kwargs):
        """base_DoGetPosition() -> (x,y)"""
        return _windows.PyPanel_base_DoGetPosition(*args, **kwargs)

    def base_DoGetVirtualSize(*args, **kwargs):
        """base_DoGetVirtualSize() -> Size"""
        return _windows.PyPanel_base_DoGetVirtualSize(*args, **kwargs)

    def base_DoGetBestSize(*args, **kwargs):
        """base_DoGetBestSize() -> Size"""
        return _windows.PyPanel_base_DoGetBestSize(*args, **kwargs)

    def base_InitDialog(*args, **kwargs):
        """base_InitDialog()"""
        return _windows.PyPanel_base_InitDialog(*args, **kwargs)

    def base_TransferDataToWindow(*args, **kwargs):
        """base_TransferDataToWindow() -> bool"""
        return _windows.PyPanel_base_TransferDataToWindow(*args, **kwargs)

    def base_TransferDataFromWindow(*args, **kwargs):
        """base_TransferDataFromWindow() -> bool"""
        return _windows.PyPanel_base_TransferDataFromWindow(*args, **kwargs)

    def base_Validate(*args, **kwargs):
        """base_Validate() -> bool"""
        return _windows.PyPanel_base_Validate(*args, **kwargs)

    def base_AcceptsFocus(*args, **kwargs):
        """base_AcceptsFocus() -> bool"""
        return _windows.PyPanel_base_AcceptsFocus(*args, **kwargs)

    def base_AcceptsFocusFromKeyboard(*args, **kwargs):
        """base_AcceptsFocusFromKeyboard() -> bool"""
        return _windows.PyPanel_base_AcceptsFocusFromKeyboard(*args, **kwargs)

    def base_GetMaxSize(*args, **kwargs):
        """base_GetMaxSize() -> Size"""
        return _windows.PyPanel_base_GetMaxSize(*args, **kwargs)

    def base_AddChild(*args, **kwargs):
        """base_AddChild(Window child)"""
        return _windows.PyPanel_base_AddChild(*args, **kwargs)

    def base_RemoveChild(*args, **kwargs):
        """base_RemoveChild(Window child)"""
        return _windows.PyPanel_base_RemoveChild(*args, **kwargs)

    def base_ShouldInheritColours(*args, **kwargs):
        """base_ShouldInheritColours() -> bool"""
        return _windows.PyPanel_base_ShouldInheritColours(*args, **kwargs)

    def base_ApplyParentThemeBackground(*args, **kwargs):
        """base_ApplyParentThemeBackground(Colour c)"""
        return _windows.PyPanel_base_ApplyParentThemeBackground(*args, **kwargs)


class PyPanelPtr(PyPanel):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = PyPanel
_windows.PyPanel_swigregister(PyPanelPtr)

def PrePyPanel(*args, **kwargs):
    """PrePyPanel() -> PyPanel"""
    val = _windows.new_PrePyPanel(*args, **kwargs)
    val.thisown = 1
    return val

class PyScrolledWindow(ScrolledWindow):
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxPyScrolledWindow instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """
        __init__(Window parent, int id, Point pos=DefaultPosition, Size size=DefaultSize, 
            long style=0, String name=PanelNameStr) -> PyScrolledWindow
        """
        newobj = _windows.new_PyScrolledWindow(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
        self._setOORInfo(self); self._setCallbackInfo(self, PyPanel)

    def _setCallbackInfo(*args, **kwargs):
        """_setCallbackInfo(PyObject self, PyObject _class)"""
        return _windows.PyScrolledWindow__setCallbackInfo(*args, **kwargs)

    def base_DoMoveWindow(*args, **kwargs):
        """base_DoMoveWindow(int x, int y, int width, int height)"""
        return _windows.PyScrolledWindow_base_DoMoveWindow(*args, **kwargs)

    def base_DoSetSize(*args, **kwargs):
        """base_DoSetSize(int x, int y, int width, int height, int sizeFlags=SIZE_AUTO)"""
        return _windows.PyScrolledWindow_base_DoSetSize(*args, **kwargs)

    def base_DoSetClientSize(*args, **kwargs):
        """base_DoSetClientSize(int width, int height)"""
        return _windows.PyScrolledWindow_base_DoSetClientSize(*args, **kwargs)

    def base_DoSetVirtualSize(*args, **kwargs):
        """base_DoSetVirtualSize(int x, int y)"""
        return _windows.PyScrolledWindow_base_DoSetVirtualSize(*args, **kwargs)

    def base_DoGetSize(*args, **kwargs):
        """base_DoGetSize() -> (width, height)"""
        return _windows.PyScrolledWindow_base_DoGetSize(*args, **kwargs)

    def base_DoGetClientSize(*args, **kwargs):
        """base_DoGetClientSize() -> (width, height)"""
        return _windows.PyScrolledWindow_base_DoGetClientSize(*args, **kwargs)

    def base_DoGetPosition(*args, **kwargs):
        """base_DoGetPosition() -> (x,y)"""
        return _windows.PyScrolledWindow_base_DoGetPosition(*args, **kwargs)

    def base_DoGetVirtualSize(*args, **kwargs):
        """base_DoGetVirtualSize() -> Size"""
        return _windows.PyScrolledWindow_base_DoGetVirtualSize(*args, **kwargs)

    def base_DoGetBestSize(*args, **kwargs):
        """base_DoGetBestSize() -> Size"""
        return _windows.PyScrolledWindow_base_DoGetBestSize(*args, **kwargs)

    def base_InitDialog(*args, **kwargs):
        """base_InitDialog()"""
        return _windows.PyScrolledWindow_base_InitDialog(*args, **kwargs)

    def base_TransferDataToWindow(*args, **kwargs):
        """base_TransferDataToWindow() -> bool"""
        return _windows.PyScrolledWindow_base_TransferDataToWindow(*args, **kwargs)

    def base_TransferDataFromWindow(*args, **kwargs):
        """base_TransferDataFromWindow() -> bool"""
        return _windows.PyScrolledWindow_base_TransferDataFromWindow(*args, **kwargs)

    def base_Validate(*args, **kwargs):
        """base_Validate() -> bool"""
        return _windows.PyScrolledWindow_base_Validate(*args, **kwargs)

    def base_AcceptsFocus(*args, **kwargs):
        """base_AcceptsFocus() -> bool"""
        return _windows.PyScrolledWindow_base_AcceptsFocus(*args, **kwargs)

    def base_AcceptsFocusFromKeyboard(*args, **kwargs):
        """base_AcceptsFocusFromKeyboard() -> bool"""
        return _windows.PyScrolledWindow_base_AcceptsFocusFromKeyboard(*args, **kwargs)

    def base_GetMaxSize(*args, **kwargs):
        """base_GetMaxSize() -> Size"""
        return _windows.PyScrolledWindow_base_GetMaxSize(*args, **kwargs)

    def base_AddChild(*args, **kwargs):
        """base_AddChild(Window child)"""
        return _windows.PyScrolledWindow_base_AddChild(*args, **kwargs)

    def base_RemoveChild(*args, **kwargs):
        """base_RemoveChild(Window child)"""
        return _windows.PyScrolledWindow_base_RemoveChild(*args, **kwargs)

    def base_ShouldInheritColours(*args, **kwargs):
        """base_ShouldInheritColours() -> bool"""
        return _windows.PyScrolledWindow_base_ShouldInheritColours(*args, **kwargs)

    def base_ApplyParentThemeBackground(*args, **kwargs):
        """base_ApplyParentThemeBackground(Colour c)"""
        return _windows.PyScrolledWindow_base_ApplyParentThemeBackground(*args, **kwargs)


class PyScrolledWindowPtr(PyScrolledWindow):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = PyScrolledWindow
_windows.PyScrolledWindow_swigregister(PyScrolledWindowPtr)

def PrePyScrolledWindow(*args, **kwargs):
    """PrePyScrolledWindow() -> PyScrolledWindow"""
    val = _windows.new_PrePyScrolledWindow(*args, **kwargs)
    val.thisown = 1
    return val

#---------------------------------------------------------------------------

PRINT_MODE_NONE = _windows.PRINT_MODE_NONE
PRINT_MODE_PREVIEW = _windows.PRINT_MODE_PREVIEW
PRINT_MODE_FILE = _windows.PRINT_MODE_FILE
PRINT_MODE_PRINTER = _windows.PRINT_MODE_PRINTER
PRINT_MODE_STREAM = _windows.PRINT_MODE_STREAM
class PrintData(core.Object):
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxPrintData instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """__init__() -> PrintData"""
        newobj = _windows.new_PrintData(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
    def __del__(self, destroy=_windows.delete_PrintData):
        """__del__()"""
        try:
            if self.thisown: destroy(self)
        except: pass

    def GetNoCopies(*args, **kwargs):
        """GetNoCopies() -> int"""
        return _windows.PrintData_GetNoCopies(*args, **kwargs)

    def GetCollate(*args, **kwargs):
        """GetCollate() -> bool"""
        return _windows.PrintData_GetCollate(*args, **kwargs)

    def GetOrientation(*args, **kwargs):
        """GetOrientation() -> int"""
        return _windows.PrintData_GetOrientation(*args, **kwargs)

    def Ok(*args, **kwargs):
        """Ok() -> bool"""
        return _windows.PrintData_Ok(*args, **kwargs)

    def GetPrinterName(*args, **kwargs):
        """GetPrinterName() -> String"""
        return _windows.PrintData_GetPrinterName(*args, **kwargs)

    def GetColour(*args, **kwargs):
        """GetColour() -> bool"""
        return _windows.PrintData_GetColour(*args, **kwargs)

    def GetDuplex(*args, **kwargs):
        """GetDuplex() -> int"""
        return _windows.PrintData_GetDuplex(*args, **kwargs)

    def GetPaperId(*args, **kwargs):
        """GetPaperId() -> int"""
        return _windows.PrintData_GetPaperId(*args, **kwargs)

    def GetPaperSize(*args, **kwargs):
        """GetPaperSize() -> Size"""
        return _windows.PrintData_GetPaperSize(*args, **kwargs)

    def GetQuality(*args, **kwargs):
        """GetQuality() -> int"""
        return _windows.PrintData_GetQuality(*args, **kwargs)

    def SetNoCopies(*args, **kwargs):
        """SetNoCopies(int v)"""
        return _windows.PrintData_SetNoCopies(*args, **kwargs)

    def SetCollate(*args, **kwargs):
        """SetCollate(bool flag)"""
        return _windows.PrintData_SetCollate(*args, **kwargs)

    def SetOrientation(*args, **kwargs):
        """SetOrientation(int orient)"""
        return _windows.PrintData_SetOrientation(*args, **kwargs)

    def SetPrinterName(*args, **kwargs):
        """SetPrinterName(String name)"""
        return _windows.PrintData_SetPrinterName(*args, **kwargs)

    def SetColour(*args, **kwargs):
        """SetColour(bool colour)"""
        return _windows.PrintData_SetColour(*args, **kwargs)

    def SetDuplex(*args, **kwargs):
        """SetDuplex(int duplex)"""
        return _windows.PrintData_SetDuplex(*args, **kwargs)

    def SetPaperId(*args, **kwargs):
        """SetPaperId(int sizeId)"""
        return _windows.PrintData_SetPaperId(*args, **kwargs)

    def SetPaperSize(*args, **kwargs):
        """SetPaperSize(Size sz)"""
        return _windows.PrintData_SetPaperSize(*args, **kwargs)

    def SetQuality(*args, **kwargs):
        """SetQuality(int quality)"""
        return _windows.PrintData_SetQuality(*args, **kwargs)

    def GetPrinterCommand(*args, **kwargs):
        """GetPrinterCommand() -> String"""
        return _windows.PrintData_GetPrinterCommand(*args, **kwargs)

    def GetPrinterOptions(*args, **kwargs):
        """GetPrinterOptions() -> String"""
        return _windows.PrintData_GetPrinterOptions(*args, **kwargs)

    def GetPreviewCommand(*args, **kwargs):
        """GetPreviewCommand() -> String"""
        return _windows.PrintData_GetPreviewCommand(*args, **kwargs)

    def GetFilename(*args, **kwargs):
        """GetFilename() -> String"""
        return _windows.PrintData_GetFilename(*args, **kwargs)

    def GetFontMetricPath(*args, **kwargs):
        """GetFontMetricPath() -> String"""
        return _windows.PrintData_GetFontMetricPath(*args, **kwargs)

    def GetPrinterScaleX(*args, **kwargs):
        """GetPrinterScaleX() -> double"""
        return _windows.PrintData_GetPrinterScaleX(*args, **kwargs)

    def GetPrinterScaleY(*args, **kwargs):
        """GetPrinterScaleY() -> double"""
        return _windows.PrintData_GetPrinterScaleY(*args, **kwargs)

    def GetPrinterTranslateX(*args, **kwargs):
        """GetPrinterTranslateX() -> long"""
        return _windows.PrintData_GetPrinterTranslateX(*args, **kwargs)

    def GetPrinterTranslateY(*args, **kwargs):
        """GetPrinterTranslateY() -> long"""
        return _windows.PrintData_GetPrinterTranslateY(*args, **kwargs)

    def GetPrintMode(*args, **kwargs):
        """GetPrintMode() -> int"""
        return _windows.PrintData_GetPrintMode(*args, **kwargs)

    def SetPrinterCommand(*args, **kwargs):
        """SetPrinterCommand(String command)"""
        return _windows.PrintData_SetPrinterCommand(*args, **kwargs)

    def SetPrinterOptions(*args, **kwargs):
        """SetPrinterOptions(String options)"""
        return _windows.PrintData_SetPrinterOptions(*args, **kwargs)

    def SetPreviewCommand(*args, **kwargs):
        """SetPreviewCommand(String command)"""
        return _windows.PrintData_SetPreviewCommand(*args, **kwargs)

    def SetFilename(*args, **kwargs):
        """SetFilename(String filename)"""
        return _windows.PrintData_SetFilename(*args, **kwargs)

    def SetFontMetricPath(*args, **kwargs):
        """SetFontMetricPath(String path)"""
        return _windows.PrintData_SetFontMetricPath(*args, **kwargs)

    def SetPrinterScaleX(*args, **kwargs):
        """SetPrinterScaleX(double x)"""
        return _windows.PrintData_SetPrinterScaleX(*args, **kwargs)

    def SetPrinterScaleY(*args, **kwargs):
        """SetPrinterScaleY(double y)"""
        return _windows.PrintData_SetPrinterScaleY(*args, **kwargs)

    def SetPrinterScaling(*args, **kwargs):
        """SetPrinterScaling(double x, double y)"""
        return _windows.PrintData_SetPrinterScaling(*args, **kwargs)

    def SetPrinterTranslateX(*args, **kwargs):
        """SetPrinterTranslateX(long x)"""
        return _windows.PrintData_SetPrinterTranslateX(*args, **kwargs)

    def SetPrinterTranslateY(*args, **kwargs):
        """SetPrinterTranslateY(long y)"""
        return _windows.PrintData_SetPrinterTranslateY(*args, **kwargs)

    def SetPrinterTranslation(*args, **kwargs):
        """SetPrinterTranslation(long x, long y)"""
        return _windows.PrintData_SetPrinterTranslation(*args, **kwargs)

    def SetPrintMode(*args, **kwargs):
        """SetPrintMode(int printMode)"""
        return _windows.PrintData_SetPrintMode(*args, **kwargs)

    def GetOutputStream(*args, **kwargs):
        """GetOutputStream() -> OutputStream"""
        return _windows.PrintData_GetOutputStream(*args, **kwargs)

    def SetOutputStream(*args, **kwargs):
        """SetOutputStream(OutputStream outputstream)"""
        return _windows.PrintData_SetOutputStream(*args, **kwargs)

    def __nonzero__(self): return self.Ok() 

class PrintDataPtr(PrintData):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = PrintData
_windows.PrintData_swigregister(PrintDataPtr)
PrintoutTitleStr = cvar.PrintoutTitleStr
PreviewCanvasNameStr = cvar.PreviewCanvasNameStr

class PageSetupDialogData(core.Object):
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxPageSetupDialogData instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """__init__() -> PageSetupDialogData"""
        newobj = _windows.new_PageSetupDialogData(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
    def __del__(self, destroy=_windows.delete_PageSetupDialogData):
        """__del__()"""
        try:
            if self.thisown: destroy(self)
        except: pass

    def EnableHelp(*args, **kwargs):
        """EnableHelp(bool flag)"""
        return _windows.PageSetupDialogData_EnableHelp(*args, **kwargs)

    def EnableMargins(*args, **kwargs):
        """EnableMargins(bool flag)"""
        return _windows.PageSetupDialogData_EnableMargins(*args, **kwargs)

    def EnableOrientation(*args, **kwargs):
        """EnableOrientation(bool flag)"""
        return _windows.PageSetupDialogData_EnableOrientation(*args, **kwargs)

    def EnablePaper(*args, **kwargs):
        """EnablePaper(bool flag)"""
        return _windows.PageSetupDialogData_EnablePaper(*args, **kwargs)

    def EnablePrinter(*args, **kwargs):
        """EnablePrinter(bool flag)"""
        return _windows.PageSetupDialogData_EnablePrinter(*args, **kwargs)

    def GetDefaultMinMargins(*args, **kwargs):
        """GetDefaultMinMargins() -> bool"""
        return _windows.PageSetupDialogData_GetDefaultMinMargins(*args, **kwargs)

    def GetEnableMargins(*args, **kwargs):
        """GetEnableMargins() -> bool"""
        return _windows.PageSetupDialogData_GetEnableMargins(*args, **kwargs)

    def GetEnableOrientation(*args, **kwargs):
        """GetEnableOrientation() -> bool"""
        return _windows.PageSetupDialogData_GetEnableOrientation(*args, **kwargs)

    def GetEnablePaper(*args, **kwargs):
        """GetEnablePaper() -> bool"""
        return _windows.PageSetupDialogData_GetEnablePaper(*args, **kwargs)

    def GetEnablePrinter(*args, **kwargs):
        """GetEnablePrinter() -> bool"""
        return _windows.PageSetupDialogData_GetEnablePrinter(*args, **kwargs)

    def GetEnableHelp(*args, **kwargs):
        """GetEnableHelp() -> bool"""
        return _windows.PageSetupDialogData_GetEnableHelp(*args, **kwargs)

    def GetDefaultInfo(*args, **kwargs):
        """GetDefaultInfo() -> bool"""
        return _windows.PageSetupDialogData_GetDefaultInfo(*args, **kwargs)

    def GetMarginTopLeft(*args, **kwargs):
        """GetMarginTopLeft() -> Point"""
        return _windows.PageSetupDialogData_GetMarginTopLeft(*args, **kwargs)

    def GetMarginBottomRight(*args, **kwargs):
        """GetMarginBottomRight() -> Point"""
        return _windows.PageSetupDialogData_GetMarginBottomRight(*args, **kwargs)

    def GetMinMarginTopLeft(*args, **kwargs):
        """GetMinMarginTopLeft() -> Point"""
        return _windows.PageSetupDialogData_GetMinMarginTopLeft(*args, **kwargs)

    def GetMinMarginBottomRight(*args, **kwargs):
        """GetMinMarginBottomRight() -> Point"""
        return _windows.PageSetupDialogData_GetMinMarginBottomRight(*args, **kwargs)

    def GetPaperId(*args, **kwargs):
        """GetPaperId() -> int"""
        return _windows.PageSetupDialogData_GetPaperId(*args, **kwargs)

    def GetPaperSize(*args, **kwargs):
        """GetPaperSize() -> Size"""
        return _windows.PageSetupDialogData_GetPaperSize(*args, **kwargs)

    def GetPrintData(*args, **kwargs):
        """GetPrintData() -> PrintData"""
        return _windows.PageSetupDialogData_GetPrintData(*args, **kwargs)

    def Ok(*args, **kwargs):
        """Ok() -> bool"""
        return _windows.PageSetupDialogData_Ok(*args, **kwargs)

    def SetDefaultInfo(*args, **kwargs):
        """SetDefaultInfo(bool flag)"""
        return _windows.PageSetupDialogData_SetDefaultInfo(*args, **kwargs)

    def SetDefaultMinMargins(*args, **kwargs):
        """SetDefaultMinMargins(bool flag)"""
        return _windows.PageSetupDialogData_SetDefaultMinMargins(*args, **kwargs)

    def SetMarginTopLeft(*args, **kwargs):
        """SetMarginTopLeft(Point pt)"""
        return _windows.PageSetupDialogData_SetMarginTopLeft(*args, **kwargs)

    def SetMarginBottomRight(*args, **kwargs):
        """SetMarginBottomRight(Point pt)"""
        return _windows.PageSetupDialogData_SetMarginBottomRight(*args, **kwargs)

    def SetMinMarginTopLeft(*args, **kwargs):
        """SetMinMarginTopLeft(Point pt)"""
        return _windows.PageSetupDialogData_SetMinMarginTopLeft(*args, **kwargs)

    def SetMinMarginBottomRight(*args, **kwargs):
        """SetMinMarginBottomRight(Point pt)"""
        return _windows.PageSetupDialogData_SetMinMarginBottomRight(*args, **kwargs)

    def SetPaperId(*args, **kwargs):
        """SetPaperId(int id)"""
        return _windows.PageSetupDialogData_SetPaperId(*args, **kwargs)

    def SetPaperSize(*args, **kwargs):
        """SetPaperSize(Size size)"""
        return _windows.PageSetupDialogData_SetPaperSize(*args, **kwargs)

    def SetPrintData(*args, **kwargs):
        """SetPrintData(PrintData printData)"""
        return _windows.PageSetupDialogData_SetPrintData(*args, **kwargs)

    def __nonzero__(self): return self.Ok() 

class PageSetupDialogDataPtr(PageSetupDialogData):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = PageSetupDialogData
_windows.PageSetupDialogData_swigregister(PageSetupDialogDataPtr)

class PageSetupDialog(Dialog):
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxPageSetupDialog instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """__init__(Window parent, PageSetupDialogData data=None) -> PageSetupDialog"""
        newobj = _windows.new_PageSetupDialog(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
        self._setOORInfo(self)

    def GetPageSetupData(*args, **kwargs):
        """GetPageSetupData() -> PageSetupDialogData"""
        return _windows.PageSetupDialog_GetPageSetupData(*args, **kwargs)

    def ShowModal(*args, **kwargs):
        """ShowModal() -> int"""
        return _windows.PageSetupDialog_ShowModal(*args, **kwargs)


class PageSetupDialogPtr(PageSetupDialog):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = PageSetupDialog
_windows.PageSetupDialog_swigregister(PageSetupDialogPtr)

class PrintDialogData(core.Object):
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxPrintDialogData instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args):
        """
        __init__() -> PrintDialogData
        __init__(PrintData printData) -> PrintDialogData
        """
        newobj = _windows.new_PrintDialogData(*args)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
    def __del__(self, destroy=_windows.delete_PrintDialogData):
        """__del__()"""
        try:
            if self.thisown: destroy(self)
        except: pass

    def GetFromPage(*args, **kwargs):
        """GetFromPage() -> int"""
        return _windows.PrintDialogData_GetFromPage(*args, **kwargs)

    def GetToPage(*args, **kwargs):
        """GetToPage() -> int"""
        return _windows.PrintDialogData_GetToPage(*args, **kwargs)

    def GetMinPage(*args, **kwargs):
        """GetMinPage() -> int"""
        return _windows.PrintDialogData_GetMinPage(*args, **kwargs)

    def GetMaxPage(*args, **kwargs):
        """GetMaxPage() -> int"""
        return _windows.PrintDialogData_GetMaxPage(*args, **kwargs)

    def GetNoCopies(*args, **kwargs):
        """GetNoCopies() -> int"""
        return _windows.PrintDialogData_GetNoCopies(*args, **kwargs)

    def GetAllPages(*args, **kwargs):
        """GetAllPages() -> bool"""
        return _windows.PrintDialogData_GetAllPages(*args, **kwargs)

    def GetSelection(*args, **kwargs):
        """GetSelection() -> bool"""
        return _windows.PrintDialogData_GetSelection(*args, **kwargs)

    def GetCollate(*args, **kwargs):
        """GetCollate() -> bool"""
        return _windows.PrintDialogData_GetCollate(*args, **kwargs)

    def GetPrintToFile(*args, **kwargs):
        """GetPrintToFile() -> bool"""
        return _windows.PrintDialogData_GetPrintToFile(*args, **kwargs)

    def GetSetupDialog(*args, **kwargs):
        """GetSetupDialog() -> bool"""
        return _windows.PrintDialogData_GetSetupDialog(*args, **kwargs)

    def SetFromPage(*args, **kwargs):
        """SetFromPage(int v)"""
        return _windows.PrintDialogData_SetFromPage(*args, **kwargs)

    def SetToPage(*args, **kwargs):
        """SetToPage(int v)"""
        return _windows.PrintDialogData_SetToPage(*args, **kwargs)

    def SetMinPage(*args, **kwargs):
        """SetMinPage(int v)"""
        return _windows.PrintDialogData_SetMinPage(*args, **kwargs)

    def SetMaxPage(*args, **kwargs):
        """SetMaxPage(int v)"""
        return _windows.PrintDialogData_SetMaxPage(*args, **kwargs)

    def SetNoCopies(*args, **kwargs):
        """SetNoCopies(int v)"""
        return _windows.PrintDialogData_SetNoCopies(*args, **kwargs)

    def SetAllPages(*args, **kwargs):
        """SetAllPages(bool flag)"""
        return _windows.PrintDialogData_SetAllPages(*args, **kwargs)

    def SetSelection(*args, **kwargs):
        """SetSelection(bool flag)"""
        return _windows.PrintDialogData_SetSelection(*args, **kwargs)

    def SetCollate(*args, **kwargs):
        """SetCollate(bool flag)"""
        return _windows.PrintDialogData_SetCollate(*args, **kwargs)

    def SetPrintToFile(*args, **kwargs):
        """SetPrintToFile(bool flag)"""
        return _windows.PrintDialogData_SetPrintToFile(*args, **kwargs)

    def SetSetupDialog(*args, **kwargs):
        """SetSetupDialog(bool flag)"""
        return _windows.PrintDialogData_SetSetupDialog(*args, **kwargs)

    def EnablePrintToFile(*args, **kwargs):
        """EnablePrintToFile(bool flag)"""
        return _windows.PrintDialogData_EnablePrintToFile(*args, **kwargs)

    def EnableSelection(*args, **kwargs):
        """EnableSelection(bool flag)"""
        return _windows.PrintDialogData_EnableSelection(*args, **kwargs)

    def EnablePageNumbers(*args, **kwargs):
        """EnablePageNumbers(bool flag)"""
        return _windows.PrintDialogData_EnablePageNumbers(*args, **kwargs)

    def EnableHelp(*args, **kwargs):
        """EnableHelp(bool flag)"""
        return _windows.PrintDialogData_EnableHelp(*args, **kwargs)

    def GetEnablePrintToFile(*args, **kwargs):
        """GetEnablePrintToFile() -> bool"""
        return _windows.PrintDialogData_GetEnablePrintToFile(*args, **kwargs)

    def GetEnableSelection(*args, **kwargs):
        """GetEnableSelection() -> bool"""
        return _windows.PrintDialogData_GetEnableSelection(*args, **kwargs)

    def GetEnablePageNumbers(*args, **kwargs):
        """GetEnablePageNumbers() -> bool"""
        return _windows.PrintDialogData_GetEnablePageNumbers(*args, **kwargs)

    def GetEnableHelp(*args, **kwargs):
        """GetEnableHelp() -> bool"""
        return _windows.PrintDialogData_GetEnableHelp(*args, **kwargs)

    def Ok(*args, **kwargs):
        """Ok() -> bool"""
        return _windows.PrintDialogData_Ok(*args, **kwargs)

    def GetPrintData(*args, **kwargs):
        """GetPrintData() -> PrintData"""
        return _windows.PrintDialogData_GetPrintData(*args, **kwargs)

    def SetPrintData(*args, **kwargs):
        """SetPrintData(PrintData printData)"""
        return _windows.PrintDialogData_SetPrintData(*args, **kwargs)

    def __nonzero__(self): return self.Ok() 

class PrintDialogDataPtr(PrintDialogData):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = PrintDialogData
_windows.PrintDialogData_swigregister(PrintDialogDataPtr)

class PrintDialog(Dialog):
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxPrintDialog instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """__init__(Window parent, PrintDialogData data=None) -> PrintDialog"""
        newobj = _windows.new_PrintDialog(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
        self._setOORInfo(self)

    def GetPrintDialogData(*args, **kwargs):
        """GetPrintDialogData() -> PrintDialogData"""
        return _windows.PrintDialog_GetPrintDialogData(*args, **kwargs)

    def GetPrintDC(*args, **kwargs):
        """GetPrintDC() -> DC"""
        return _windows.PrintDialog_GetPrintDC(*args, **kwargs)

    def ShowModal(*args, **kwargs):
        """ShowModal() -> int"""
        return _windows.PrintDialog_ShowModal(*args, **kwargs)


class PrintDialogPtr(PrintDialog):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = PrintDialog
_windows.PrintDialog_swigregister(PrintDialogPtr)

PRINTER_NO_ERROR = _windows.PRINTER_NO_ERROR
PRINTER_CANCELLED = _windows.PRINTER_CANCELLED
PRINTER_ERROR = _windows.PRINTER_ERROR
class Printer(core.Object):
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxPrinter instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """__init__(PrintDialogData data=None) -> Printer"""
        newobj = _windows.new_Printer(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
    def __del__(self, destroy=_windows.delete_Printer):
        """__del__()"""
        try:
            if self.thisown: destroy(self)
        except: pass

    def CreateAbortWindow(*args, **kwargs):
        """CreateAbortWindow(Window parent, Printout printout)"""
        return _windows.Printer_CreateAbortWindow(*args, **kwargs)

    def GetPrintDialogData(*args, **kwargs):
        """GetPrintDialogData() -> PrintDialogData"""
        return _windows.Printer_GetPrintDialogData(*args, **kwargs)

    def Print(*args, **kwargs):
        """Print(Window parent, Printout printout, int prompt=True) -> bool"""
        return _windows.Printer_Print(*args, **kwargs)

    def PrintDialog(*args, **kwargs):
        """PrintDialog(Window parent) -> DC"""
        return _windows.Printer_PrintDialog(*args, **kwargs)

    def ReportError(*args, **kwargs):
        """ReportError(Window parent, Printout printout, String message)"""
        return _windows.Printer_ReportError(*args, **kwargs)

    def Setup(*args, **kwargs):
        """Setup(Window parent) -> bool"""
        return _windows.Printer_Setup(*args, **kwargs)

    def GetAbort(*args, **kwargs):
        """GetAbort() -> bool"""
        return _windows.Printer_GetAbort(*args, **kwargs)

    def GetLastError(*args, **kwargs):
        """GetLastError() -> int"""
        return _windows.Printer_GetLastError(*args, **kwargs)

    GetLastError = staticmethod(GetLastError)

class PrinterPtr(Printer):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = Printer
_windows.Printer_swigregister(PrinterPtr)

def Printer_GetLastError(*args, **kwargs):
    """Printer_GetLastError() -> int"""
    return _windows.Printer_GetLastError(*args, **kwargs)

class Printout(core.Object):
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxPyPrintout instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """__init__(String title=PrintoutTitleStr) -> Printout"""
        newobj = _windows.new_Printout(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
        self._setCallbackInfo(self, Printout)

    def _setCallbackInfo(*args, **kwargs):
        """_setCallbackInfo(PyObject self, PyObject _class)"""
        return _windows.Printout__setCallbackInfo(*args, **kwargs)

    def GetTitle(*args, **kwargs):
        """GetTitle() -> String"""
        return _windows.Printout_GetTitle(*args, **kwargs)

    def GetDC(*args, **kwargs):
        """GetDC() -> DC"""
        return _windows.Printout_GetDC(*args, **kwargs)

    def SetDC(*args, **kwargs):
        """SetDC(DC dc)"""
        return _windows.Printout_SetDC(*args, **kwargs)

    def SetPageSizePixels(*args, **kwargs):
        """SetPageSizePixels(int w, int h)"""
        return _windows.Printout_SetPageSizePixels(*args, **kwargs)

    def GetPageSizePixels(*args, **kwargs):
        """GetPageSizePixels() -> (w, h)"""
        return _windows.Printout_GetPageSizePixels(*args, **kwargs)

    def SetPageSizeMM(*args, **kwargs):
        """SetPageSizeMM(int w, int h)"""
        return _windows.Printout_SetPageSizeMM(*args, **kwargs)

    def GetPageSizeMM(*args, **kwargs):
        """GetPageSizeMM() -> (w, h)"""
        return _windows.Printout_GetPageSizeMM(*args, **kwargs)

    def SetPPIScreen(*args, **kwargs):
        """SetPPIScreen(int x, int y)"""
        return _windows.Printout_SetPPIScreen(*args, **kwargs)

    def GetPPIScreen(*args, **kwargs):
        """GetPPIScreen() -> (x,y)"""
        return _windows.Printout_GetPPIScreen(*args, **kwargs)

    def SetPPIPrinter(*args, **kwargs):
        """SetPPIPrinter(int x, int y)"""
        return _windows.Printout_SetPPIPrinter(*args, **kwargs)

    def GetPPIPrinter(*args, **kwargs):
        """GetPPIPrinter() -> (x,y)"""
        return _windows.Printout_GetPPIPrinter(*args, **kwargs)

    def IsPreview(*args, **kwargs):
        """IsPreview() -> bool"""
        return _windows.Printout_IsPreview(*args, **kwargs)

    def SetIsPreview(*args, **kwargs):
        """SetIsPreview(bool p)"""
        return _windows.Printout_SetIsPreview(*args, **kwargs)

    def base_OnBeginDocument(*args, **kwargs):
        """base_OnBeginDocument(int startPage, int endPage) -> bool"""
        return _windows.Printout_base_OnBeginDocument(*args, **kwargs)

    def base_OnEndDocument(*args, **kwargs):
        """base_OnEndDocument()"""
        return _windows.Printout_base_OnEndDocument(*args, **kwargs)

    def base_OnBeginPrinting(*args, **kwargs):
        """base_OnBeginPrinting()"""
        return _windows.Printout_base_OnBeginPrinting(*args, **kwargs)

    def base_OnEndPrinting(*args, **kwargs):
        """base_OnEndPrinting()"""
        return _windows.Printout_base_OnEndPrinting(*args, **kwargs)

    def base_OnPreparePrinting(*args, **kwargs):
        """base_OnPreparePrinting()"""
        return _windows.Printout_base_OnPreparePrinting(*args, **kwargs)

    def base_HasPage(*args, **kwargs):
        """base_HasPage(int page) -> bool"""
        return _windows.Printout_base_HasPage(*args, **kwargs)

    def base_GetPageInfo(*args, **kwargs):
        """base_GetPageInfo() -> (minPage, maxPage, pageFrom, pageTo)"""
        return _windows.Printout_base_GetPageInfo(*args, **kwargs)


class PrintoutPtr(Printout):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = Printout
_windows.Printout_swigregister(PrintoutPtr)

class PreviewCanvas(ScrolledWindow):
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxPreviewCanvas instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """
        __init__(PrintPreview preview, Window parent, Point pos=DefaultPosition, 
            Size size=DefaultSize, long style=0, 
            String name=PreviewCanvasNameStr) -> PreviewCanvas
        """
        newobj = _windows.new_PreviewCanvas(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
        self._setOORInfo(self)


class PreviewCanvasPtr(PreviewCanvas):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = PreviewCanvas
_windows.PreviewCanvas_swigregister(PreviewCanvasPtr)

class PreviewFrame(Frame):
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxPreviewFrame instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """
        __init__(PrintPreview preview, Frame parent, String title, Point pos=DefaultPosition, 
            Size size=DefaultSize, 
            long style=DEFAULT_FRAME_STYLE, String name=FrameNameStr) -> PreviewFrame
        """
        newobj = _windows.new_PreviewFrame(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
        self._setOORInfo(self)

    def Initialize(*args, **kwargs):
        """Initialize()"""
        return _windows.PreviewFrame_Initialize(*args, **kwargs)

    def CreateControlBar(*args, **kwargs):
        """CreateControlBar()"""
        return _windows.PreviewFrame_CreateControlBar(*args, **kwargs)

    def CreateCanvas(*args, **kwargs):
        """CreateCanvas()"""
        return _windows.PreviewFrame_CreateCanvas(*args, **kwargs)

    def GetControlBar(*args, **kwargs):
        """GetControlBar() -> PreviewControlBar"""
        return _windows.PreviewFrame_GetControlBar(*args, **kwargs)


class PreviewFramePtr(PreviewFrame):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = PreviewFrame
_windows.PreviewFrame_swigregister(PreviewFramePtr)

PREVIEW_PRINT = _windows.PREVIEW_PRINT
PREVIEW_PREVIOUS = _windows.PREVIEW_PREVIOUS
PREVIEW_NEXT = _windows.PREVIEW_NEXT
PREVIEW_ZOOM = _windows.PREVIEW_ZOOM
PREVIEW_FIRST = _windows.PREVIEW_FIRST
PREVIEW_LAST = _windows.PREVIEW_LAST
PREVIEW_GOTO = _windows.PREVIEW_GOTO
PREVIEW_DEFAULT = _windows.PREVIEW_DEFAULT
ID_PREVIEW_CLOSE = _windows.ID_PREVIEW_CLOSE
ID_PREVIEW_NEXT = _windows.ID_PREVIEW_NEXT
ID_PREVIEW_PREVIOUS = _windows.ID_PREVIEW_PREVIOUS
ID_PREVIEW_PRINT = _windows.ID_PREVIEW_PRINT
ID_PREVIEW_ZOOM = _windows.ID_PREVIEW_ZOOM
ID_PREVIEW_FIRST = _windows.ID_PREVIEW_FIRST
ID_PREVIEW_LAST = _windows.ID_PREVIEW_LAST
ID_PREVIEW_GOTO = _windows.ID_PREVIEW_GOTO
class PreviewControlBar(Panel):
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxPreviewControlBar instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """
        __init__(PrintPreview preview, long buttons, Window parent, 
            Point pos=DefaultPosition, Size size=DefaultSize, 
            long style=TAB_TRAVERSAL, String name=PanelNameStr) -> PreviewControlBar
        """
        newobj = _windows.new_PreviewControlBar(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
        self._setOORInfo(self)

    def GetZoomControl(*args, **kwargs):
        """GetZoomControl() -> int"""
        return _windows.PreviewControlBar_GetZoomControl(*args, **kwargs)

    def SetZoomControl(*args, **kwargs):
        """SetZoomControl(int zoom)"""
        return _windows.PreviewControlBar_SetZoomControl(*args, **kwargs)

    def GetPrintPreview(*args, **kwargs):
        """GetPrintPreview() -> PrintPreview"""
        return _windows.PreviewControlBar_GetPrintPreview(*args, **kwargs)

    def OnNext(*args, **kwargs):
        """OnNext()"""
        return _windows.PreviewControlBar_OnNext(*args, **kwargs)

    def OnPrevious(*args, **kwargs):
        """OnPrevious()"""
        return _windows.PreviewControlBar_OnPrevious(*args, **kwargs)

    def OnFirst(*args, **kwargs):
        """OnFirst()"""
        return _windows.PreviewControlBar_OnFirst(*args, **kwargs)

    def OnLast(*args, **kwargs):
        """OnLast()"""
        return _windows.PreviewControlBar_OnLast(*args, **kwargs)

    def OnGoto(*args, **kwargs):
        """OnGoto()"""
        return _windows.PreviewControlBar_OnGoto(*args, **kwargs)


class PreviewControlBarPtr(PreviewControlBar):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = PreviewControlBar
_windows.PreviewControlBar_swigregister(PreviewControlBarPtr)

class PrintPreview(core.Object):
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxPrintPreview instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args):
        """
        __init__(Printout printout, Printout printoutForPrinting, PrintDialogData data=None) -> PrintPreview
        __init__(Printout printout, Printout printoutForPrinting, PrintData data) -> PrintPreview
        """
        newobj = _windows.new_PrintPreview(*args)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
    def SetCurrentPage(*args, **kwargs):
        """SetCurrentPage(int pageNum) -> bool"""
        return _windows.PrintPreview_SetCurrentPage(*args, **kwargs)

    def GetCurrentPage(*args, **kwargs):
        """GetCurrentPage() -> int"""
        return _windows.PrintPreview_GetCurrentPage(*args, **kwargs)

    def SetPrintout(*args, **kwargs):
        """SetPrintout(Printout printout)"""
        return _windows.PrintPreview_SetPrintout(*args, **kwargs)

    def GetPrintout(*args, **kwargs):
        """GetPrintout() -> Printout"""
        return _windows.PrintPreview_GetPrintout(*args, **kwargs)

    def GetPrintoutForPrinting(*args, **kwargs):
        """GetPrintoutForPrinting() -> Printout"""
        return _windows.PrintPreview_GetPrintoutForPrinting(*args, **kwargs)

    def SetFrame(*args, **kwargs):
        """SetFrame(Frame frame)"""
        return _windows.PrintPreview_SetFrame(*args, **kwargs)

    def SetCanvas(*args, **kwargs):
        """SetCanvas(PreviewCanvas canvas)"""
        return _windows.PrintPreview_SetCanvas(*args, **kwargs)

    def GetFrame(*args, **kwargs):
        """GetFrame() -> Frame"""
        return _windows.PrintPreview_GetFrame(*args, **kwargs)

    def GetCanvas(*args, **kwargs):
        """GetCanvas() -> PreviewCanvas"""
        return _windows.PrintPreview_GetCanvas(*args, **kwargs)

    def PaintPage(*args, **kwargs):
        """PaintPage(PreviewCanvas canvas, DC dc) -> bool"""
        return _windows.PrintPreview_PaintPage(*args, **kwargs)

    def DrawBlankPage(*args, **kwargs):
        """DrawBlankPage(PreviewCanvas canvas, DC dc) -> bool"""
        return _windows.PrintPreview_DrawBlankPage(*args, **kwargs)

    def RenderPage(*args, **kwargs):
        """RenderPage(int pageNum) -> bool"""
        return _windows.PrintPreview_RenderPage(*args, **kwargs)

    def AdjustScrollbars(*args, **kwargs):
        """AdjustScrollbars(PreviewCanvas canvas)"""
        return _windows.PrintPreview_AdjustScrollbars(*args, **kwargs)

    def GetPrintDialogData(*args, **kwargs):
        """GetPrintDialogData() -> PrintDialogData"""
        return _windows.PrintPreview_GetPrintDialogData(*args, **kwargs)

    def SetZoom(*args, **kwargs):
        """SetZoom(int percent)"""
        return _windows.PrintPreview_SetZoom(*args, **kwargs)

    def GetZoom(*args, **kwargs):
        """GetZoom() -> int"""
        return _windows.PrintPreview_GetZoom(*args, **kwargs)

    def GetMaxPage(*args, **kwargs):
        """GetMaxPage() -> int"""
        return _windows.PrintPreview_GetMaxPage(*args, **kwargs)

    def GetMinPage(*args, **kwargs):
        """GetMinPage() -> int"""
        return _windows.PrintPreview_GetMinPage(*args, **kwargs)

    def Ok(*args, **kwargs):
        """Ok() -> bool"""
        return _windows.PrintPreview_Ok(*args, **kwargs)

    def SetOk(*args, **kwargs):
        """SetOk(bool ok)"""
        return _windows.PrintPreview_SetOk(*args, **kwargs)

    def Print(*args, **kwargs):
        """Print(bool interactive) -> bool"""
        return _windows.PrintPreview_Print(*args, **kwargs)

    def DetermineScaling(*args, **kwargs):
        """DetermineScaling()"""
        return _windows.PrintPreview_DetermineScaling(*args, **kwargs)

    def __nonzero__(self): return self.Ok() 

class PrintPreviewPtr(PrintPreview):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = PrintPreview
_windows.PrintPreview_swigregister(PrintPreviewPtr)

class PyPrintPreview(PrintPreview):
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxPyPrintPreview instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args):
        """
        __init__(Printout printout, Printout printoutForPrinting, PrintDialogData data=None) -> PyPrintPreview
        __init__(Printout printout, Printout printoutForPrinting, PrintData data) -> PyPrintPreview
        """
        newobj = _windows.new_PyPrintPreview(*args)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
        self._setCallbackInfo(self, PyPrintPreview)

    def _setCallbackInfo(*args, **kwargs):
        """_setCallbackInfo(PyObject self, PyObject _class)"""
        return _windows.PyPrintPreview__setCallbackInfo(*args, **kwargs)

    def base_SetCurrentPage(*args, **kwargs):
        """base_SetCurrentPage(int pageNum) -> bool"""
        return _windows.PyPrintPreview_base_SetCurrentPage(*args, **kwargs)

    def base_PaintPage(*args, **kwargs):
        """base_PaintPage(PreviewCanvas canvas, DC dc) -> bool"""
        return _windows.PyPrintPreview_base_PaintPage(*args, **kwargs)

    def base_DrawBlankPage(*args, **kwargs):
        """base_DrawBlankPage(PreviewCanvas canvas, DC dc) -> bool"""
        return _windows.PyPrintPreview_base_DrawBlankPage(*args, **kwargs)

    def base_RenderPage(*args, **kwargs):
        """base_RenderPage(int pageNum) -> bool"""
        return _windows.PyPrintPreview_base_RenderPage(*args, **kwargs)

    def base_SetZoom(*args, **kwargs):
        """base_SetZoom(int percent)"""
        return _windows.PyPrintPreview_base_SetZoom(*args, **kwargs)

    def base_Print(*args, **kwargs):
        """base_Print(bool interactive) -> bool"""
        return _windows.PyPrintPreview_base_Print(*args, **kwargs)

    def base_DetermineScaling(*args, **kwargs):
        """base_DetermineScaling()"""
        return _windows.PyPrintPreview_base_DetermineScaling(*args, **kwargs)


class PyPrintPreviewPtr(PyPrintPreview):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = PyPrintPreview
_windows.PyPrintPreview_swigregister(PyPrintPreviewPtr)

class PyPreviewFrame(PreviewFrame):
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxPyPreviewFrame instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """
        __init__(PrintPreview preview, Frame parent, String title, Point pos=DefaultPosition, 
            Size size=DefaultSize, 
            long style=DEFAULT_FRAME_STYLE, String name=FrameNameStr) -> PyPreviewFrame
        """
        newobj = _windows.new_PyPreviewFrame(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
        self._setCallbackInfo(self, PyPreviewFrame); self._setOORInfo(self)

    def _setCallbackInfo(*args, **kwargs):
        """_setCallbackInfo(PyObject self, PyObject _class)"""
        return _windows.PyPreviewFrame__setCallbackInfo(*args, **kwargs)

    def SetPreviewCanvas(*args, **kwargs):
        """SetPreviewCanvas(PreviewCanvas canvas)"""
        return _windows.PyPreviewFrame_SetPreviewCanvas(*args, **kwargs)

    def SetControlBar(*args, **kwargs):
        """SetControlBar(PreviewControlBar bar)"""
        return _windows.PyPreviewFrame_SetControlBar(*args, **kwargs)

    def base_Initialize(*args, **kwargs):
        """base_Initialize()"""
        return _windows.PyPreviewFrame_base_Initialize(*args, **kwargs)

    def base_CreateCanvas(*args, **kwargs):
        """base_CreateCanvas()"""
        return _windows.PyPreviewFrame_base_CreateCanvas(*args, **kwargs)

    def base_CreateControlBar(*args, **kwargs):
        """base_CreateControlBar()"""
        return _windows.PyPreviewFrame_base_CreateControlBar(*args, **kwargs)


class PyPreviewFramePtr(PyPreviewFrame):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = PyPreviewFrame
_windows.PyPreviewFrame_swigregister(PyPreviewFramePtr)

class PyPreviewControlBar(PreviewControlBar):
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxPyPreviewControlBar instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """
        __init__(PrintPreview preview, long buttons, Window parent, 
            Point pos=DefaultPosition, Size size=DefaultSize, 
            long style=0, String name=PanelNameStr) -> PyPreviewControlBar
        """
        newobj = _windows.new_PyPreviewControlBar(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
        self._setCallbackInfo(self, PyPreviewControlBar); self._setOORInfo(self)

    def _setCallbackInfo(*args, **kwargs):
        """_setCallbackInfo(PyObject self, PyObject _class)"""
        return _windows.PyPreviewControlBar__setCallbackInfo(*args, **kwargs)

    def SetPrintPreview(*args, **kwargs):
        """SetPrintPreview(PrintPreview preview)"""
        return _windows.PyPreviewControlBar_SetPrintPreview(*args, **kwargs)

    def base_CreateButtons(*args, **kwargs):
        """base_CreateButtons()"""
        return _windows.PyPreviewControlBar_base_CreateButtons(*args, **kwargs)

    def base_SetZoomControl(*args, **kwargs):
        """base_SetZoomControl(int zoom)"""
        return _windows.PyPreviewControlBar_base_SetZoomControl(*args, **kwargs)


class PyPreviewControlBarPtr(PyPreviewControlBar):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = PyPreviewControlBar
_windows.PyPreviewControlBar_swigregister(PyPreviewControlBarPtr)


