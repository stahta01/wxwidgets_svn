# This file was created automatically by SWIG.
import sizersc

from misc import *

from windows import *

from gdi import *

from fonts import *

from clip_dnd import *

from controls import *

from events import *
import wx
import string
class wxSizerItemPtr(wxObjectPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def DeleteWindows(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizerItem_DeleteWindows,(self,) + _args, _kwargs)
        return val
    def GetPosition(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizerItem_GetPosition,(self,) + _args, _kwargs)
        if val: val = wxPointPtr(val) ; val.thisown = 1
        return val
    def GetSize(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizerItem_GetSize,(self,) + _args, _kwargs)
        if val: val = wxSizePtr(val) ; val.thisown = 1
        return val
    def CalcMin(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizerItem_CalcMin,(self,) + _args, _kwargs)
        if val: val = wxSizePtr(val) ; val.thisown = 1
        return val
    def SetDimension(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizerItem_SetDimension,(self,) + _args, _kwargs)
        return val
    def SetRatioWH(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizerItem_SetRatioWH,(self,) + _args, _kwargs)
        return val
    def SetRatioSize(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizerItem_SetRatioSize,(self,) + _args, _kwargs)
        return val
    def SetRatio(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizerItem_SetRatio,(self,) + _args, _kwargs)
        return val
    def GetRatio(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizerItem_GetRatio,(self,) + _args, _kwargs)
        return val
    def IsWindow(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizerItem_IsWindow,(self,) + _args, _kwargs)
        return val
    def IsSizer(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizerItem_IsSizer,(self,) + _args, _kwargs)
        return val
    def IsSpacer(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizerItem_IsSpacer,(self,) + _args, _kwargs)
        return val
    def GetWindow(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizerItem_GetWindow,(self,) + _args, _kwargs)
        return val
    def SetWindow(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizerItem_SetWindow,(self,) + _args, _kwargs)
        return val
    def GetSizer(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizerItem_GetSizer,(self,) + _args, _kwargs)
        return val
    def SetSizer(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizerItem_SetSizer,(self,) + _args, _kwargs)
        return val
    def GetOption(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizerItem_GetOption,(self,) + _args, _kwargs)
        return val
    def GetFlag(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizerItem_GetFlag,(self,) + _args, _kwargs)
        return val
    def GetBorder(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizerItem_GetBorder,(self,) + _args, _kwargs)
        return val
    def SetInitSize(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizerItem_SetInitSize,(self,) + _args, _kwargs)
        return val
    def SetOption(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizerItem_SetOption,(self,) + _args, _kwargs)
        return val
    def SetFlag(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizerItem_SetFlag,(self,) + _args, _kwargs)
        return val
    def SetBorder(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizerItem_SetBorder,(self,) + _args, _kwargs)
        return val
    def GetUserData(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizerItem_GetUserData,(self,) + _args, _kwargs)
        return val
    def __repr__(self):
        return "<C wxSizerItem instance at %s>" % (self.this,)
class wxSizerItem(wxSizerItemPtr):
    def __init__(self,this):
        self.this = this




class wxSizerPtr(wxObjectPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def _setOORInfo(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizer__setOORInfo,(self,) + _args, _kwargs)
        return val
    def Destroy(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizer_Destroy,(self,) + _args, _kwargs)
        return val
    def AddWindow(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizer_AddWindow,(self,) + _args, _kwargs)
        return val
    def AddSizer(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizer_AddSizer,(self,) + _args, _kwargs)
        return val
    def AddSpacer(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizer_AddSpacer,(self,) + _args, _kwargs)
        return val
    def InsertWindow(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizer_InsertWindow,(self,) + _args, _kwargs)
        return val
    def InsertSizer(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizer_InsertSizer,(self,) + _args, _kwargs)
        return val
    def InsertSpacer(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizer_InsertSpacer,(self,) + _args, _kwargs)
        return val
    def PrependWindow(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizer_PrependWindow,(self,) + _args, _kwargs)
        return val
    def PrependSizer(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizer_PrependSizer,(self,) + _args, _kwargs)
        return val
    def PrependSpacer(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizer_PrependSpacer,(self,) + _args, _kwargs)
        return val
    def RemoveWindow(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizer_RemoveWindow,(self,) + _args, _kwargs)
        return val
    def RemoveSizer(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizer_RemoveSizer,(self,) + _args, _kwargs)
        return val
    def RemovePos(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizer_RemovePos,(self,) + _args, _kwargs)
        return val
    def SetDimension(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizer_SetDimension,(self,) + _args, _kwargs)
        return val
    def SetMinSize(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizer_SetMinSize,(self,) + _args, _kwargs)
        return val
    def SetItemMinSizeWindow(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizer_SetItemMinSizeWindow,(self,) + _args, _kwargs)
        return val
    def SetItemMinSizeSizer(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizer_SetItemMinSizeSizer,(self,) + _args, _kwargs)
        return val
    def SetItemMinSizePos(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizer_SetItemMinSizePos,(self,) + _args, _kwargs)
        return val
    def GetSize(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizer_GetSize,(self,) + _args, _kwargs)
        if val: val = wxSizePtr(val) ; val.thisown = 1
        return val
    def GetPosition(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizer_GetPosition,(self,) + _args, _kwargs)
        if val: val = wxPointPtr(val) ; val.thisown = 1
        return val
    def GetMinSize(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizer_GetMinSize,(self,) + _args, _kwargs)
        if val: val = wxSizePtr(val) ; val.thisown = 1
        return val
    def Layout(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizer_Layout,(self,) + _args, _kwargs)
        return val
    def Fit(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizer_Fit,(self,) + _args, _kwargs)
        return val
    def SetSizeHints(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizer_SetSizeHints,(self,) + _args, _kwargs)
        return val
    def Clear(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizer_Clear,(self,) + _args, _kwargs)
        return val
    def DeleteWindows(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizer_DeleteWindows,(self,) + _args, _kwargs)
        return val
    def GetChildren(self, *_args, **_kwargs):
        val = apply(sizersc.wxSizer_GetChildren,(self,) + _args, _kwargs)
        return val
    def __repr__(self):
        return "<C wxSizer instance at %s>" % (self.this,)
    
    def Add(self, *args, **kw):
        if type(args[0]) == type(1):
            apply(self.AddSpacer, args, kw)
        elif string.find(args[0].this, 'Sizer') != -1:
            apply(self.AddSizer, args, kw)
        else:
            apply(self.AddWindow, args, kw)

    def Insert(self, *args, **kw):
        if type(args[1]) == type(1):
            apply(self.InsertSpacer, args, kw)
        elif string.find(args[1].this, 'Sizer') != -1:
            apply(self.InsertSizer, args, kw)
        else:
            apply(self.InsertWindow, args, kw)

    def Prepend(self, *args, **kw):
        if type(args[0]) == type(1):
            apply(self.PrependSpacer, args, kw)
        elif string.find(args[0].this, 'Sizer') != -1:
            apply(self.PrependSizer, args, kw)
        else:
            apply(self.PrependWindow, args, kw)

    def Remove(self, *args, **kw):
        if type(args[0]) == type(1):
            apply(self.RemovePos, args, kw)
        elif string.find(args[0].this, 'Sizer') != -1:
            apply(self.RemoveSizer, args, kw)
        else:
            apply(self.RemoveWindow, args, kw)

    def AddMany(self, widgets):
        for childinfo in widgets:
            if type(childinfo) != type(()):
                childinfo = (childinfo, )
            apply(self.Add, childinfo)

    
    def SetItemMinSize(self, *args):
        if type(args[0]) == type(1):
            apply(self.SetItemMinSizePos, args)
        elif string.find(args[0].this, 'Sizer') != -1:
            apply(self.SetItemMinSizeSizer, args)
        else:
            apply(self.SetItemMinSizeWindow, args)
     
class wxSizer(wxSizerPtr):
    def __init__(self,this):
        self.this = this




class wxPySizerPtr(wxSizerPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def _setCallbackInfo(self, *_args, **_kwargs):
        val = apply(sizersc.wxPySizer__setCallbackInfo,(self,) + _args, _kwargs)
        return val
    def __repr__(self):
        return "<C wxPySizer instance at %s>" % (self.this,)
class wxPySizer(wxPySizerPtr):
    def __init__(self,*_args,**_kwargs):
        self.this = apply(sizersc.new_wxPySizer,_args,_kwargs)
        self.thisown = 1
        self._setCallbackInfo(self, wxPySizer)
        self._setOORInfo(self)




class wxBoxSizerPtr(wxSizerPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def GetOrientation(self, *_args, **_kwargs):
        val = apply(sizersc.wxBoxSizer_GetOrientation,(self,) + _args, _kwargs)
        return val
    def RecalcSizes(self, *_args, **_kwargs):
        val = apply(sizersc.wxBoxSizer_RecalcSizes,(self,) + _args, _kwargs)
        return val
    def CalcMin(self, *_args, **_kwargs):
        val = apply(sizersc.wxBoxSizer_CalcMin,(self,) + _args, _kwargs)
        if val: val = wxSizePtr(val) ; val.thisown = 1
        return val
    def __repr__(self):
        return "<C wxBoxSizer instance at %s>" % (self.this,)
class wxBoxSizer(wxBoxSizerPtr):
    def __init__(self,*_args,**_kwargs):
        self.this = apply(sizersc.new_wxBoxSizer,_args,_kwargs)
        self.thisown = 1
        self._setOORInfo(self)




class wxStaticBoxSizerPtr(wxBoxSizerPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def GetStaticBox(self, *_args, **_kwargs):
        val = apply(sizersc.wxStaticBoxSizer_GetStaticBox,(self,) + _args, _kwargs)
        return val
    def RecalcSizes(self, *_args, **_kwargs):
        val = apply(sizersc.wxStaticBoxSizer_RecalcSizes,(self,) + _args, _kwargs)
        return val
    def CalcMin(self, *_args, **_kwargs):
        val = apply(sizersc.wxStaticBoxSizer_CalcMin,(self,) + _args, _kwargs)
        if val: val = wxSizePtr(val) ; val.thisown = 1
        return val
    def __repr__(self):
        return "<C wxStaticBoxSizer instance at %s>" % (self.this,)
class wxStaticBoxSizer(wxStaticBoxSizerPtr):
    def __init__(self,*_args,**_kwargs):
        self.this = apply(sizersc.new_wxStaticBoxSizer,_args,_kwargs)
        self.thisown = 1
        self._setOORInfo(self)




class wxNotebookSizerPtr(wxSizerPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def RecalcSizes(self, *_args, **_kwargs):
        val = apply(sizersc.wxNotebookSizer_RecalcSizes,(self,) + _args, _kwargs)
        return val
    def CalcMin(self, *_args, **_kwargs):
        val = apply(sizersc.wxNotebookSizer_CalcMin,(self,) + _args, _kwargs)
        if val: val = wxSizePtr(val) ; val.thisown = 1
        return val
    def GetNotebook(self, *_args, **_kwargs):
        val = apply(sizersc.wxNotebookSizer_GetNotebook,(self,) + _args, _kwargs)
        return val
    def __repr__(self):
        return "<C wxNotebookSizer instance at %s>" % (self.this,)
class wxNotebookSizer(wxNotebookSizerPtr):
    def __init__(self,*_args,**_kwargs):
        self.this = apply(sizersc.new_wxNotebookSizer,_args,_kwargs)
        self.thisown = 1
        self._setOORInfo(self)




class wxGridSizerPtr(wxSizerPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def RecalcSizes(self, *_args, **_kwargs):
        val = apply(sizersc.wxGridSizer_RecalcSizes,(self,) + _args, _kwargs)
        return val
    def CalcMin(self, *_args, **_kwargs):
        val = apply(sizersc.wxGridSizer_CalcMin,(self,) + _args, _kwargs)
        if val: val = wxSizePtr(val) ; val.thisown = 1
        return val
    def SetCols(self, *_args, **_kwargs):
        val = apply(sizersc.wxGridSizer_SetCols,(self,) + _args, _kwargs)
        return val
    def SetRows(self, *_args, **_kwargs):
        val = apply(sizersc.wxGridSizer_SetRows,(self,) + _args, _kwargs)
        return val
    def SetVGap(self, *_args, **_kwargs):
        val = apply(sizersc.wxGridSizer_SetVGap,(self,) + _args, _kwargs)
        return val
    def SetHGap(self, *_args, **_kwargs):
        val = apply(sizersc.wxGridSizer_SetHGap,(self,) + _args, _kwargs)
        return val
    def GetCols(self, *_args, **_kwargs):
        val = apply(sizersc.wxGridSizer_GetCols,(self,) + _args, _kwargs)
        return val
    def GetRows(self, *_args, **_kwargs):
        val = apply(sizersc.wxGridSizer_GetRows,(self,) + _args, _kwargs)
        return val
    def GetVGap(self, *_args, **_kwargs):
        val = apply(sizersc.wxGridSizer_GetVGap,(self,) + _args, _kwargs)
        return val
    def GetHGap(self, *_args, **_kwargs):
        val = apply(sizersc.wxGridSizer_GetHGap,(self,) + _args, _kwargs)
        return val
    def __repr__(self):
        return "<C wxGridSizer instance at %s>" % (self.this,)
class wxGridSizer(wxGridSizerPtr):
    def __init__(self,*_args,**_kwargs):
        self.this = apply(sizersc.new_wxGridSizer,_args,_kwargs)
        self.thisown = 1
        self._setOORInfo(self)




class wxFlexGridSizerPtr(wxGridSizerPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def RecalcSizes(self, *_args, **_kwargs):
        val = apply(sizersc.wxFlexGridSizer_RecalcSizes,(self,) + _args, _kwargs)
        return val
    def CalcMin(self, *_args, **_kwargs):
        val = apply(sizersc.wxFlexGridSizer_CalcMin,(self,) + _args, _kwargs)
        if val: val = wxSizePtr(val) ; val.thisown = 1
        return val
    def AddGrowableRow(self, *_args, **_kwargs):
        val = apply(sizersc.wxFlexGridSizer_AddGrowableRow,(self,) + _args, _kwargs)
        return val
    def RemoveGrowableRow(self, *_args, **_kwargs):
        val = apply(sizersc.wxFlexGridSizer_RemoveGrowableRow,(self,) + _args, _kwargs)
        return val
    def AddGrowableCol(self, *_args, **_kwargs):
        val = apply(sizersc.wxFlexGridSizer_AddGrowableCol,(self,) + _args, _kwargs)
        return val
    def RemoveGrowableCol(self, *_args, **_kwargs):
        val = apply(sizersc.wxFlexGridSizer_RemoveGrowableCol,(self,) + _args, _kwargs)
        return val
    def __repr__(self):
        return "<C wxFlexGridSizer instance at %s>" % (self.this,)
class wxFlexGridSizer(wxFlexGridSizerPtr):
    def __init__(self,*_args,**_kwargs):
        self.this = apply(sizersc.new_wxFlexGridSizer,_args,_kwargs)
        self.thisown = 1
        self._setOORInfo(self)






#-------------- FUNCTION WRAPPERS ------------------



#-------------- VARIABLE WRAPPERS ------------------

