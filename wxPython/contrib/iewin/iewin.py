# This file was created automatically by SWIG.
# Don't modify this file, modify the SWIG interface instead.

import _iewin

import core
wx = core 
class MSHTMLEvent(core.NotifyEvent):
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxMSHTMLEvent instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """__init__(wxEventType commandType=wxEVT_NULL, int id=0) -> MSHTMLEvent"""
        newobj = _iewin.new_MSHTMLEvent(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
    def GetText1(*args, **kwargs):
        """GetText1() -> String"""
        return _iewin.MSHTMLEvent_GetText1(*args, **kwargs)

    def GetLong1(*args, **kwargs):
        """GetLong1() -> long"""
        return _iewin.MSHTMLEvent_GetLong1(*args, **kwargs)

    def GetLong2(*args, **kwargs):
        """GetLong2() -> long"""
        return _iewin.MSHTMLEvent_GetLong2(*args, **kwargs)


class MSHTMLEventPtr(MSHTMLEvent):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = MSHTMLEvent
_iewin.MSHTMLEvent_swigregister(MSHTMLEventPtr)

wxEVT_COMMAND_MSHTML_BEFORENAVIGATE2 = _iewin.wxEVT_COMMAND_MSHTML_BEFORENAVIGATE2
wxEVT_COMMAND_MSHTML_NEWWINDOW2 = _iewin.wxEVT_COMMAND_MSHTML_NEWWINDOW2
wxEVT_COMMAND_MSHTML_DOCUMENTCOMPLETE = _iewin.wxEVT_COMMAND_MSHTML_DOCUMENTCOMPLETE
wxEVT_COMMAND_MSHTML_PROGRESSCHANGE = _iewin.wxEVT_COMMAND_MSHTML_PROGRESSCHANGE
wxEVT_COMMAND_MSHTML_STATUSTEXTCHANGE = _iewin.wxEVT_COMMAND_MSHTML_STATUSTEXTCHANGE
wxEVT_COMMAND_MSHTML_TITLECHANGE = _iewin.wxEVT_COMMAND_MSHTML_TITLECHANGE
EVT_MSHTML_BEFORENAVIGATE2      = wx.PyEventBinder(wxEVT_COMMAND_MSHTML_BEFORENAVIGATE2, 1)
EVT_MSHTML_NEWWINDOW2           = wx.PyEventBinder(wxEVT_COMMAND_MSHTML_NEWWINDOW2, 1)
EVT_MSHTML_DOCUMENTCOMPLETE     = wx.PyEventBinder(wxEVT_COMMAND_MSHTML_DOCUMENTCOMPLETE, 1)
EVT_MSHTML_PROGRESSCHANGE       = wx.PyEventBinder(wxEVT_COMMAND_MSHTML_PROGRESSCHANGE, 1)
EVT_MSHTML_STATUSTEXTCHANGE     = wx.PyEventBinder(wxEVT_COMMAND_MSHTML_STATUSTEXTCHANGE, 1)
EVT_MSHTML_TITLECHANGE          = wx.PyEventBinder(wxEVT_COMMAND_MSHTML_TITLECHANGE, 1)

IEHTML_REFRESH_NORMAL = _iewin.IEHTML_REFRESH_NORMAL
IEHTML_REFRESH_IFEXPIRED = _iewin.IEHTML_REFRESH_IFEXPIRED
IEHTML_REFRESH_CONTINUE = _iewin.IEHTML_REFRESH_CONTINUE
IEHTML_REFRESH_COMPLETELY = _iewin.IEHTML_REFRESH_COMPLETELY
class IEHtmlWin(core.Window):
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxIEHtmlWin instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """
        __init__(Window parent, int id=-1, Point pos=DefaultPosition, 
            Size size=DefaultSize, long style=0, String name=PanelNameStr) -> IEHtmlWin
        """
        newobj = _iewin.new_IEHtmlWin(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
        self._setOORInfo(self)

    def LoadUrl(*args, **kwargs):
        """LoadUrl(String ??)"""
        return _iewin.IEHtmlWin_LoadUrl(*args, **kwargs)

    def LoadString(*args, **kwargs):
        """LoadString(String html) -> bool"""
        return _iewin.IEHtmlWin_LoadString(*args, **kwargs)

    def LoadStream(*args, **kwargs):
        """LoadStream(InputStream is) -> bool"""
        return _iewin.IEHtmlWin_LoadStream(*args, **kwargs)

    Navigate = LoadUrl 
    def SetCharset(*args, **kwargs):
        """SetCharset(String charset)"""
        return _iewin.IEHtmlWin_SetCharset(*args, **kwargs)

    def SetEditMode(*args, **kwargs):
        """SetEditMode(bool seton)"""
        return _iewin.IEHtmlWin_SetEditMode(*args, **kwargs)

    def GetEditMode(*args, **kwargs):
        """GetEditMode() -> bool"""
        return _iewin.IEHtmlWin_GetEditMode(*args, **kwargs)

    def GetStringSelection(*args, **kwargs):
        """GetStringSelection(bool asHTML=False) -> String"""
        return _iewin.IEHtmlWin_GetStringSelection(*args, **kwargs)

    def GetText(*args, **kwargs):
        """GetText(bool asHTML=False) -> String"""
        return _iewin.IEHtmlWin_GetText(*args, **kwargs)

    def GoBack(*args, **kwargs):
        """GoBack() -> bool"""
        return _iewin.IEHtmlWin_GoBack(*args, **kwargs)

    def GoForward(*args, **kwargs):
        """GoForward() -> bool"""
        return _iewin.IEHtmlWin_GoForward(*args, **kwargs)

    def GoHome(*args, **kwargs):
        """GoHome() -> bool"""
        return _iewin.IEHtmlWin_GoHome(*args, **kwargs)

    def GoSearch(*args, **kwargs):
        """GoSearch() -> bool"""
        return _iewin.IEHtmlWin_GoSearch(*args, **kwargs)

    def RefreshPage(*args, **kwargs):
        """RefreshPage(int level) -> bool"""
        return _iewin.IEHtmlWin_RefreshPage(*args, **kwargs)

    def Stop(*args, **kwargs):
        """Stop() -> bool"""
        return _iewin.IEHtmlWin_Stop(*args, **kwargs)


class IEHtmlWinPtr(IEHtmlWin):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = IEHtmlWin
_iewin.IEHtmlWin_swigregister(IEHtmlWinPtr)


