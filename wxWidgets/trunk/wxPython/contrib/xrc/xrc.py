# This file was created automatically by SWIG.
import xrcc

from misc import *

from misc2 import *

from windows import *

from gdi import *

from fonts import *

from clip_dnd import *

from events import *

from streams import *

from utils import *

from mdi import *

from frames import *

from stattool import *

from controls import *

from controls2 import *

from windows2 import *

from cmndlgs import *

from windows3 import *

from image import *

from printfw import *

from sizers import *

from filesys import *

def XRCID(str_id):
    return wxXmlResource_GetXRCID(str_id)

def XRCCTRL(window, str_id, *ignoreargs):
    return window.FindWindowById(XRCID(str_id))

XMLID = XRCID
XMLCTRL = XRCCTRL

class wxXmlResourcePtr(wxObjectPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def __del__(self,xrcc=xrcc):
        if self.thisown == 1 :
            xrcc.delete_wxXmlResource(self)
    def Load(self, *_args, **_kwargs):
        val = apply(xrcc.wxXmlResource_Load,(self,) + _args, _kwargs)
        return val
    def LoadFromString(self, *_args, **_kwargs):
        val = apply(xrcc.wxXmlResource_LoadFromString,(self,) + _args, _kwargs)
        return val
    def InitAllHandlers(self, *_args, **_kwargs):
        val = apply(xrcc.wxXmlResource_InitAllHandlers,(self,) + _args, _kwargs)
        return val
    def AddHandler(self, *_args, **_kwargs):
        val = apply(xrcc.wxXmlResource_AddHandler,(self,) + _args, _kwargs)
        return val
    def ClearHandlers(self, *_args, **_kwargs):
        val = apply(xrcc.wxXmlResource_ClearHandlers,(self,) + _args, _kwargs)
        return val
    def LoadMenu(self, *_args, **_kwargs):
        val = apply(xrcc.wxXmlResource_LoadMenu,(self,) + _args, _kwargs)
        return val
    def LoadMenuBar(self, *_args, **_kwargs):
        val = apply(xrcc.wxXmlResource_LoadMenuBar,(self,) + _args, _kwargs)
        return val
    def LoadMenuBarOnFrame(self, *_args, **_kwargs):
        val = apply(xrcc.wxXmlResource_LoadMenuBarOnFrame,(self,) + _args, _kwargs)
        return val
    def LoadToolBar(self, *_args, **_kwargs):
        val = apply(xrcc.wxXmlResource_LoadToolBar,(self,) + _args, _kwargs)
        return val
    def LoadDialog(self, *_args, **_kwargs):
        val = apply(xrcc.wxXmlResource_LoadDialog,(self,) + _args, _kwargs)
        if val: val = wxDialogPtr(val) 
        return val
    def LoadOnDialog(self, *_args, **_kwargs):
        val = apply(xrcc.wxXmlResource_LoadOnDialog,(self,) + _args, _kwargs)
        return val
    def LoadPanel(self, *_args, **_kwargs):
        val = apply(xrcc.wxXmlResource_LoadPanel,(self,) + _args, _kwargs)
        if val: val = wxPanelPtr(val) 
        return val
    def LoadOnPanel(self, *_args, **_kwargs):
        val = apply(xrcc.wxXmlResource_LoadOnPanel,(self,) + _args, _kwargs)
        return val
    def LoadFrame(self, *_args, **_kwargs):
        val = apply(xrcc.wxXmlResource_LoadFrame,(self,) + _args, _kwargs)
        return val
    def LoadBitmap(self, *_args, **_kwargs):
        val = apply(xrcc.wxXmlResource_LoadBitmap,(self,) + _args, _kwargs)
        if val: val = wxBitmapPtr(val) ; val.thisown = 1
        return val
    def LoadIcon(self, *_args, **_kwargs):
        val = apply(xrcc.wxXmlResource_LoadIcon,(self,) + _args, _kwargs)
        if val: val = wxIconPtr(val) ; val.thisown = 1
        return val
    def AttachUnknownControl(self, *_args, **_kwargs):
        val = apply(xrcc.wxXmlResource_AttachUnknownControl,(self,) + _args, _kwargs)
        return val
    def GetVersion(self, *_args, **_kwargs):
        val = apply(xrcc.wxXmlResource_GetVersion,(self,) + _args, _kwargs)
        return val
    def CompareVersion(self, *_args, **_kwargs):
        val = apply(xrcc.wxXmlResource_CompareVersion,(self,) + _args, _kwargs)
        return val
    def __repr__(self):
        return "<C wxXmlResource instance at %s>" % (self.this,)
class wxXmlResource(wxXmlResourcePtr):
    def __init__(self,*_args,**_kwargs):
        self.this = apply(xrcc.new_wxXmlResource,_args,_kwargs)
        self.thisown = 1
        self.InitAllHandlers()



def wxEmptyXmlResource(*_args,**_kwargs):
    val = wxXmlResourcePtr(apply(xrcc.new_wxEmptyXmlResource,_args,_kwargs))
    val.thisown = 1
    val.InitAllHandlers()
    return val




#-------------- FUNCTION WRAPPERS ------------------

wxXmlResource_GetXRCID = xrcc.wxXmlResource_GetXRCID

def wxXmlResource_Get(*_args, **_kwargs):
    val = apply(xrcc.wxXmlResource_Get,_args,_kwargs)
    if val: val = wxXmlResourcePtr(val)
    return val

def wxXmlResource_Set(*_args, **_kwargs):
    val = apply(xrcc.wxXmlResource_Set,_args,_kwargs)
    if val: val = wxXmlResourcePtr(val)
    return val



#-------------- VARIABLE WRAPPERS ------------------

wxXRC_USE_LOCALE = xrcc.wxXRC_USE_LOCALE
wxXRC_NO_SUBCLASSING = xrcc.wxXRC_NO_SUBCLASSING


#-------------- USER INCLUDE -----------------------


# The global was removed  in favor of static accessor functions.  This is for
# backwards compatibility:
wxTheXmlResource = wxXmlResource_Get()
