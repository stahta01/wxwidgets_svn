# This file was created automatically by SWIG.
import windowsc

from misc import *

from gdi import *

from clip_dnd import *
import wx

def wxDLG_PNT(win, point_or_x, y=None):
    if y is None:
        return win.ConvertDialogPointToPixels(point_or_x)
    else:
        return win.ConvertDialogPointToPixels(wxPoint(point_or_x, y))

def wxDLG_SZE(win, size_width, height=None):
    if height is None:
        return win.ConvertDialogSizeToPixels(size_width)
    else:
        return win.ConvertDialogSizeToPixels(wxSize(size_width, height))

class wxEvtHandlerPtr(wxObjectPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def ProcessEvent(self, *_args, **_kwargs):
        val = apply(windowsc.wxEvtHandler_ProcessEvent,(self,) + _args, _kwargs)
        return val
    def AddPendingEvent(self, *_args, **_kwargs):
        val = apply(windowsc.wxEvtHandler_AddPendingEvent,(self,) + _args, _kwargs)
        return val
    def GetEvtHandlerEnabled(self, *_args, **_kwargs):
        val = apply(windowsc.wxEvtHandler_GetEvtHandlerEnabled,(self,) + _args, _kwargs)
        return val
    def SetEvtHandlerEnabled(self, *_args, **_kwargs):
        val = apply(windowsc.wxEvtHandler_SetEvtHandlerEnabled,(self,) + _args, _kwargs)
        return val
    def GetNextHandler(self, *_args, **_kwargs):
        val = apply(windowsc.wxEvtHandler_GetNextHandler,(self,) + _args, _kwargs)
        return val
    def GetPreviousHandler(self, *_args, **_kwargs):
        val = apply(windowsc.wxEvtHandler_GetPreviousHandler,(self,) + _args, _kwargs)
        return val
    def SetNextHandler(self, *_args, **_kwargs):
        val = apply(windowsc.wxEvtHandler_SetNextHandler,(self,) + _args, _kwargs)
        return val
    def SetPreviousHandler(self, *_args, **_kwargs):
        val = apply(windowsc.wxEvtHandler_SetPreviousHandler,(self,) + _args, _kwargs)
        return val
    def Connect(self, *_args, **_kwargs):
        val = apply(windowsc.wxEvtHandler_Connect,(self,) + _args, _kwargs)
        return val
    def Disconnect(self, *_args, **_kwargs):
        val = apply(windowsc.wxEvtHandler_Disconnect,(self,) + _args, _kwargs)
        return val
    def _setOORInfo(self, *_args, **_kwargs):
        val = apply(windowsc.wxEvtHandler__setOORInfo,(self,) + _args, _kwargs)
        return val
    def __repr__(self):
        return "<C wxEvtHandler instance at %s>" % (self.this,)
class wxEvtHandler(wxEvtHandlerPtr):
    def __init__(self,*_args,**_kwargs):
        self.this = apply(windowsc.new_wxEvtHandler,_args,_kwargs)
        self.thisown = 1




class wxValidatorPtr(wxEvtHandlerPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def Clone(self, *_args, **_kwargs):
        val = apply(windowsc.wxValidator_Clone,(self,) + _args, _kwargs)
        return val
    def GetWindow(self, *_args, **_kwargs):
        val = apply(windowsc.wxValidator_GetWindow,(self,) + _args, _kwargs)
        return val
    def SetWindow(self, *_args, **_kwargs):
        val = apply(windowsc.wxValidator_SetWindow,(self,) + _args, _kwargs)
        return val
    def __repr__(self):
        return "<C wxValidator instance at %s>" % (self.this,)
class wxValidator(wxValidatorPtr):
    def __init__(self,*_args,**_kwargs):
        self.this = apply(windowsc.new_wxValidator,_args,_kwargs)
        self.thisown = 1
        self._setOORInfo(self)




class wxPyValidatorPtr(wxValidatorPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def _setCallbackInfo(self, *_args, **_kwargs):
        val = apply(windowsc.wxPyValidator__setCallbackInfo,(self,) + _args, _kwargs)
        return val
    def __repr__(self):
        return "<C wxPyValidator instance at %s>" % (self.this,)
class wxPyValidator(wxPyValidatorPtr):
    def __init__(self,*_args,**_kwargs):
        self.this = apply(windowsc.new_wxPyValidator,_args,_kwargs)
        self.thisown = 1
        self._setCallbackInfo(self, wxPyValidator, 1)
        self._setOORInfo(self)




class wxWindowPtr(wxEvtHandlerPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def Create(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_Create,(self,) + _args, _kwargs)
        return val
    def CaptureMouse(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_CaptureMouse,(self,) + _args, _kwargs)
        return val
    def Center(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_Center,(self,) + _args, _kwargs)
        return val
    def Centre(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_Centre,(self,) + _args, _kwargs)
        return val
    def CentreOnParent(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_CentreOnParent,(self,) + _args, _kwargs)
        return val
    def CenterOnParent(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_CenterOnParent,(self,) + _args, _kwargs)
        return val
    def CentreOnScreen(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_CentreOnScreen,(self,) + _args, _kwargs)
        return val
    def CenterOnScreen(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_CenterOnScreen,(self,) + _args, _kwargs)
        return val
    def Clear(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_Clear,(self,) + _args, _kwargs)
        return val
    def ClientToScreenXY(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_ClientToScreenXY,(self,) + _args, _kwargs)
        return val
    def ClientToScreen(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_ClientToScreen,(self,) + _args, _kwargs)
        if val: val = wxPointPtr(val) ; val.thisown = 1
        return val
    def Close(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_Close,(self,) + _args, _kwargs)
        return val
    def Destroy(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_Destroy,(self,) + _args, _kwargs)
        return val
    def DestroyChildren(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_DestroyChildren,(self,) + _args, _kwargs)
        return val
    def IsBeingDeleted(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_IsBeingDeleted,(self,) + _args, _kwargs)
        return val
    def DragAcceptFiles(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_DragAcceptFiles,(self,) + _args, _kwargs)
        return val
    def Enable(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_Enable,(self,) + _args, _kwargs)
        return val
    def FindWindowById(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_FindWindowById,(self,) + _args, _kwargs)
        return val
    def FindWindowByName(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_FindWindowByName,(self,) + _args, _kwargs)
        return val
    def Fit(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_Fit,(self,) + _args, _kwargs)
        return val
    def GetBackgroundColour(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_GetBackgroundColour,(self,) + _args, _kwargs)
        if val: val = wxColourPtr(val) ; val.thisown = 1
        return val
    def GetChildren(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_GetChildren,(self,) + _args, _kwargs)
        return val
    def GetCharHeight(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_GetCharHeight,(self,) + _args, _kwargs)
        return val
    def GetCharWidth(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_GetCharWidth,(self,) + _args, _kwargs)
        return val
    def GetClientSizeTuple(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_GetClientSizeTuple,(self,) + _args, _kwargs)
        return val
    def GetClientSize(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_GetClientSize,(self,) + _args, _kwargs)
        if val: val = wxSizePtr(val) ; val.thisown = 1
        return val
    def GetConstraints(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_GetConstraints,(self,) + _args, _kwargs)
        if val: val = wxLayoutConstraintsPtr(val) 
        return val
    def GetEventHandler(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_GetEventHandler,(self,) + _args, _kwargs)
        return val
    def GetFont(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_GetFont,(self,) + _args, _kwargs)
        if val: val = wxFontPtr(val) 
        return val
    def GetForegroundColour(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_GetForegroundColour,(self,) + _args, _kwargs)
        if val: val = wxColourPtr(val) ; val.thisown = 1
        return val
    def GetGrandParent(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_GetGrandParent,(self,) + _args, _kwargs)
        return val
    def GetHandle(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_GetHandle,(self,) + _args, _kwargs)
        return val
    def GetId(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_GetId,(self,) + _args, _kwargs)
        return val
    def GetLabel(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_GetLabel,(self,) + _args, _kwargs)
        return val
    def SetLabel(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_SetLabel,(self,) + _args, _kwargs)
        return val
    def GetName(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_GetName,(self,) + _args, _kwargs)
        return val
    def GetParent(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_GetParent,(self,) + _args, _kwargs)
        return val
    def GetPositionTuple(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_GetPositionTuple,(self,) + _args, _kwargs)
        return val
    def GetPosition(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_GetPosition,(self,) + _args, _kwargs)
        if val: val = wxPointPtr(val) ; val.thisown = 1
        return val
    def GetRect(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_GetRect,(self,) + _args, _kwargs)
        if val: val = wxRectPtr(val) ; val.thisown = 1
        return val
    def GetScrollThumb(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_GetScrollThumb,(self,) + _args, _kwargs)
        return val
    def GetScrollPos(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_GetScrollPos,(self,) + _args, _kwargs)
        return val
    def GetScrollRange(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_GetScrollRange,(self,) + _args, _kwargs)
        return val
    def GetSizeTuple(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_GetSizeTuple,(self,) + _args, _kwargs)
        return val
    def GetSize(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_GetSize,(self,) + _args, _kwargs)
        if val: val = wxSizePtr(val) ; val.thisown = 1
        return val
    def GetTextExtent(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_GetTextExtent,(self,) + _args, _kwargs)
        return val
    def GetFullTextExtent(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_GetFullTextExtent,(self,) + _args, _kwargs)
        return val
    def GetTitle(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_GetTitle,(self,) + _args, _kwargs)
        return val
    def GetUpdateRegion(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_GetUpdateRegion,(self,) + _args, _kwargs)
        if val: val = wxRegionPtr(val) ; val.thisown = 1
        return val
    def GetWindowStyleFlag(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_GetWindowStyleFlag,(self,) + _args, _kwargs)
        return val
    def SetWindowStyleFlag(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_SetWindowStyleFlag,(self,) + _args, _kwargs)
        return val
    def SetWindowStyle(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_SetWindowStyle,(self,) + _args, _kwargs)
        return val
    def Hide(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_Hide,(self,) + _args, _kwargs)
        return val
    def InitDialog(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_InitDialog,(self,) + _args, _kwargs)
        return val
    def IsEnabled(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_IsEnabled,(self,) + _args, _kwargs)
        return val
    def IsExposed(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_IsExposed,(self,) + _args, _kwargs)
        return val
    def IsExposedPoint(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_IsExposedPoint,(self,) + _args, _kwargs)
        return val
    def IsExposedRect(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_IsExposedRect,(self,) + _args, _kwargs)
        return val
    def IsRetained(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_IsRetained,(self,) + _args, _kwargs)
        return val
    def IsShown(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_IsShown,(self,) + _args, _kwargs)
        return val
    def IsTopLevel(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_IsTopLevel,(self,) + _args, _kwargs)
        return val
    def Layout(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_Layout,(self,) + _args, _kwargs)
        return val
    def LoadFromResource(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_LoadFromResource,(self,) + _args, _kwargs)
        return val
    def Lower(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_Lower,(self,) + _args, _kwargs)
        return val
    def MakeModal(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_MakeModal,(self,) + _args, _kwargs)
        return val
    def MoveXY(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_MoveXY,(self,) + _args, _kwargs)
        return val
    def Move(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_Move,(self,) + _args, _kwargs)
        return val
    def PopEventHandler(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_PopEventHandler,(self,) + _args, _kwargs)
        return val
    def PushEventHandler(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_PushEventHandler,(self,) + _args, _kwargs)
        return val
    def PopupMenuXY(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_PopupMenuXY,(self,) + _args, _kwargs)
        return val
    def PopupMenu(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_PopupMenu,(self,) + _args, _kwargs)
        return val
    def Raise(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_Raise,(self,) + _args, _kwargs)
        return val
    def Refresh(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_Refresh,(self,) + _args, _kwargs)
        return val
    def RefreshRect(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_RefreshRect,(self,) + _args, _kwargs)
        return val
    def ReleaseMouse(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_ReleaseMouse,(self,) + _args, _kwargs)
        return val
    def RemoveChild(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_RemoveChild,(self,) + _args, _kwargs)
        return val
    def Reparent(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_Reparent,(self,) + _args, _kwargs)
        return val
    def ScreenToClientXY(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_ScreenToClientXY,(self,) + _args, _kwargs)
        return val
    def ScreenToClient(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_ScreenToClient,(self,) + _args, _kwargs)
        if val: val = wxPointPtr(val) ; val.thisown = 1
        return val
    def ScrollWindow(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_ScrollWindow,(self,) + _args, _kwargs)
        return val
    def SetAutoLayout(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_SetAutoLayout,(self,) + _args, _kwargs)
        return val
    def GetAutoLayout(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_GetAutoLayout,(self,) + _args, _kwargs)
        return val
    def SetBackgroundColour(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_SetBackgroundColour,(self,) + _args, _kwargs)
        return val
    def SetConstraints(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_SetConstraints,(self,) + _args, _kwargs)
        return val
    def UnsetConstraints(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_UnsetConstraints,(self,) + _args, _kwargs)
        return val
    def SetFocus(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_SetFocus,(self,) + _args, _kwargs)
        return val
    def AcceptsFocus(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_AcceptsFocus,(self,) + _args, _kwargs)
        return val
    def SetFont(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_SetFont,(self,) + _args, _kwargs)
        return val
    def SetForegroundColour(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_SetForegroundColour,(self,) + _args, _kwargs)
        return val
    def SetId(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_SetId,(self,) + _args, _kwargs)
        return val
    def SetName(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_SetName,(self,) + _args, _kwargs)
        return val
    def SetScrollbar(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_SetScrollbar,(self,) + _args, _kwargs)
        return val
    def SetScrollPos(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_SetScrollPos,(self,) + _args, _kwargs)
        return val
    def SetDimensions(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_SetDimensions,(self,) + _args, _kwargs)
        return val
    def SetSize(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_SetSize,(self,) + _args, _kwargs)
        return val
    def SetPosition(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_SetPosition,(self,) + _args, _kwargs)
        return val
    def SetRect(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_SetRect,(self,) + _args, _kwargs)
        return val
    def SetSizeHints(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_SetSizeHints,(self,) + _args, _kwargs)
        return val
    def SetClientSizeWH(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_SetClientSizeWH,(self,) + _args, _kwargs)
        return val
    def SetClientSize(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_SetClientSize,(self,) + _args, _kwargs)
        return val
    def SetCursor(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_SetCursor,(self,) + _args, _kwargs)
        return val
    def SetEventHandler(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_SetEventHandler,(self,) + _args, _kwargs)
        return val
    def SetExtraStyle(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_SetExtraStyle,(self,) + _args, _kwargs)
        return val
    def SetTitle(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_SetTitle,(self,) + _args, _kwargs)
        return val
    def Show(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_Show,(self,) + _args, _kwargs)
        return val
    def TransferDataFromWindow(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_TransferDataFromWindow,(self,) + _args, _kwargs)
        return val
    def TransferDataToWindow(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_TransferDataToWindow,(self,) + _args, _kwargs)
        return val
    def Validate(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_Validate,(self,) + _args, _kwargs)
        return val
    def WarpPointer(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_WarpPointer,(self,) + _args, _kwargs)
        return val
    def ConvertDialogPointToPixels(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_ConvertDialogPointToPixels,(self,) + _args, _kwargs)
        if val: val = wxPointPtr(val) ; val.thisown = 1
        return val
    def ConvertDialogSizeToPixels(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_ConvertDialogSizeToPixels,(self,) + _args, _kwargs)
        if val: val = wxSizePtr(val) ; val.thisown = 1
        return val
    def DLG_PNT(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_DLG_PNT,(self,) + _args, _kwargs)
        if val: val = wxPointPtr(val) ; val.thisown = 1
        return val
    def DLG_SZE(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_DLG_SZE,(self,) + _args, _kwargs)
        if val: val = wxSizePtr(val) ; val.thisown = 1
        return val
    def ConvertPixelPointToDialog(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_ConvertPixelPointToDialog,(self,) + _args, _kwargs)
        if val: val = wxPointPtr(val) ; val.thisown = 1
        return val
    def ConvertPixelSizeToDialog(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_ConvertPixelSizeToDialog,(self,) + _args, _kwargs)
        if val: val = wxSizePtr(val) ; val.thisown = 1
        return val
    def SetToolTipString(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_SetToolTipString,(self,) + _args, _kwargs)
        return val
    def SetToolTip(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_SetToolTip,(self,) + _args, _kwargs)
        return val
    def GetToolTip(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_GetToolTip,(self,) + _args, _kwargs)
        return val
    def SetSizer(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_SetSizer,(self,) + _args, _kwargs)
        return val
    def GetSizer(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_GetSizer,(self,) + _args, _kwargs)
        return val
    def GetValidator(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_GetValidator,(self,) + _args, _kwargs)
        return val
    def SetValidator(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_SetValidator,(self,) + _args, _kwargs)
        return val
    def SetDropTarget(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_SetDropTarget,(self,) + _args, _kwargs)
        _args[0].thisown = 0
        return val
    def GetDropTarget(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_GetDropTarget,(self,) + _args, _kwargs)
        if val: val = wxDropTargetPtr(val) 
        return val
    def GetBestSize(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_GetBestSize,(self,) + _args, _kwargs)
        if val: val = wxSizePtr(val) ; val.thisown = 1
        return val
    def SetCaret(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_SetCaret,(self,) + _args, _kwargs)
        return val
    def GetCaret(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_GetCaret,(self,) + _args, _kwargs)
        if val: val = wxCaretPtr(val) 
        return val
    def Freeze(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_Freeze,(self,) + _args, _kwargs)
        return val
    def Thaw(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_Thaw,(self,) + _args, _kwargs)
        return val
    def Update(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_Update,(self,) + _args, _kwargs)
        return val
    def GetHelpText(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_GetHelpText,(self,) + _args, _kwargs)
        return val
    def SetHelpText(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_SetHelpText,(self,) + _args, _kwargs)
        return val
    def ScrollLines(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_ScrollLines,(self,) + _args, _kwargs)
        return val
    def ScrollPages(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_ScrollPages,(self,) + _args, _kwargs)
        return val
    def LineUp(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_LineUp,(self,) + _args, _kwargs)
        return val
    def LineDown(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_LineDown,(self,) + _args, _kwargs)
        return val
    def PageUp(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_PageUp,(self,) + _args, _kwargs)
        return val
    def PageDown(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_PageDown,(self,) + _args, _kwargs)
        return val
    def SetAcceleratorTable(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_SetAcceleratorTable,(self,) + _args, _kwargs)
        return val
    def GetAcceleratorTable(self, *_args, **_kwargs):
        val = apply(windowsc.wxWindow_GetAcceleratorTable,(self,) + _args, _kwargs)
        if val: val = wxAcceleratorTablePtr(val) 
        return val
    def __repr__(self):
        return "<C wxWindow instance at %s>" % (self.this,)
    # replaces broken shadow method
    def GetCaret(self, *_args, **_kwargs):
        from misc2 import wxCaretPtr
        val = apply(windowsc.wxWindow_GetCaret,(self,) + _args, _kwargs)
        if val: val = wxCaretPtr(val)
        return val
    
class wxWindow(wxWindowPtr):
    def __init__(self,*_args,**_kwargs):
        self.this = apply(windowsc.new_wxWindow,_args,_kwargs)
        self.thisown = 1
        self._setOORInfo(self)



def wxPreWindow(*_args,**_kwargs):
    val = wxWindowPtr(apply(windowsc.new_wxPreWindow,_args,_kwargs))
    val.thisown = 1
    val._setOORInfo(self)
    return val


class wxPanelPtr(wxWindowPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def Create(self, *_args, **_kwargs):
        val = apply(windowsc.wxPanel_Create,(self,) + _args, _kwargs)
        return val
    def InitDialog(self, *_args, **_kwargs):
        val = apply(windowsc.wxPanel_InitDialog,(self,) + _args, _kwargs)
        return val
    def GetDefaultItem(self, *_args, **_kwargs):
        val = apply(windowsc.wxPanel_GetDefaultItem,(self,) + _args, _kwargs)
        return val
    def SetDefaultItem(self, *_args, **_kwargs):
        val = apply(windowsc.wxPanel_SetDefaultItem,(self,) + _args, _kwargs)
        return val
    def __repr__(self):
        return "<C wxPanel instance at %s>" % (self.this,)
class wxPanel(wxPanelPtr):
    def __init__(self,*_args,**_kwargs):
        self.this = apply(windowsc.new_wxPanel,_args,_kwargs)
        self.thisown = 1
        self._setOORInfo(self)



def wxPrePanel(*_args,**_kwargs):
    val = wxPanelPtr(apply(windowsc.new_wxPrePanel,_args,_kwargs))
    val.thisown = 1
    return val


class wxScrolledWindowPtr(wxPanelPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def Create(self, *_args, **_kwargs):
        val = apply(windowsc.wxScrolledWindow_Create,(self,) + _args, _kwargs)
        return val
    def EnableScrolling(self, *_args, **_kwargs):
        val = apply(windowsc.wxScrolledWindow_EnableScrolling,(self,) + _args, _kwargs)
        return val
    def GetScrollPageSize(self, *_args, **_kwargs):
        val = apply(windowsc.wxScrolledWindow_GetScrollPageSize,(self,) + _args, _kwargs)
        return val
    def GetScrollPixelsPerUnit(self, *_args, **_kwargs):
        val = apply(windowsc.wxScrolledWindow_GetScrollPixelsPerUnit,(self,) + _args, _kwargs)
        return val
    def GetTargetWindow(self, *_args, **_kwargs):
        val = apply(windowsc.wxScrolledWindow_GetTargetWindow,(self,) + _args, _kwargs)
        return val
    def GetVirtualSize(self, *_args, **_kwargs):
        val = apply(windowsc.wxScrolledWindow_GetVirtualSize,(self,) + _args, _kwargs)
        return val
    def IsRetained(self, *_args, **_kwargs):
        val = apply(windowsc.wxScrolledWindow_IsRetained,(self,) + _args, _kwargs)
        return val
    def PrepareDC(self, *_args, **_kwargs):
        val = apply(windowsc.wxScrolledWindow_PrepareDC,(self,) + _args, _kwargs)
        return val
    def Scroll(self, *_args, **_kwargs):
        val = apply(windowsc.wxScrolledWindow_Scroll,(self,) + _args, _kwargs)
        return val
    def SetScrollbars(self, *_args, **_kwargs):
        val = apply(windowsc.wxScrolledWindow_SetScrollbars,(self,) + _args, _kwargs)
        return val
    def SetScrollPageSize(self, *_args, **_kwargs):
        val = apply(windowsc.wxScrolledWindow_SetScrollPageSize,(self,) + _args, _kwargs)
        return val
    def SetTargetWindow(self, *_args, **_kwargs):
        val = apply(windowsc.wxScrolledWindow_SetTargetWindow,(self,) + _args, _kwargs)
        return val
    def GetViewStart(self, *_args, **_kwargs):
        val = apply(windowsc.wxScrolledWindow_GetViewStart,(self,) + _args, _kwargs)
        return val
    def CalcScrolledPosition(self, *_args, **_kwargs):
        val = apply(windowsc.wxScrolledWindow_CalcScrolledPosition,(self,) + _args, _kwargs)
        return val
    def CalcUnscrolledPosition(self, *_args, **_kwargs):
        val = apply(windowsc.wxScrolledWindow_CalcUnscrolledPosition,(self,) + _args, _kwargs)
        return val
    def SetScale(self, *_args, **_kwargs):
        val = apply(windowsc.wxScrolledWindow_SetScale,(self,) + _args, _kwargs)
        return val
    def GetScaleX(self, *_args, **_kwargs):
        val = apply(windowsc.wxScrolledWindow_GetScaleX,(self,) + _args, _kwargs)
        return val
    def GetScaleY(self, *_args, **_kwargs):
        val = apply(windowsc.wxScrolledWindow_GetScaleY,(self,) + _args, _kwargs)
        return val
    def AdjustScrollbars(self, *_args, **_kwargs):
        val = apply(windowsc.wxScrolledWindow_AdjustScrollbars,(self,) + _args, _kwargs)
        return val
    def __repr__(self):
        return "<C wxScrolledWindow instance at %s>" % (self.this,)
    ViewStart = GetViewStart
class wxScrolledWindow(wxScrolledWindowPtr):
    def __init__(self,*_args,**_kwargs):
        self.this = apply(windowsc.new_wxScrolledWindow,_args,_kwargs)
        self.thisown = 1
        self._setOORInfo(self)



def wxPreScrolledWindow(*_args,**_kwargs):
    val = wxScrolledWindowPtr(apply(windowsc.new_wxPreScrolledWindow,_args,_kwargs))
    val.thisown = 1
    val._setOORInfo(self)
    return val


class wxMenuPtr(wxEvtHandlerPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def Append(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenu_Append,(self,) + _args, _kwargs)
        return val
    def AppendMenu(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenu_AppendMenu,(self,) + _args, _kwargs)
        return val
    def AppendItem(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenu_AppendItem,(self,) + _args, _kwargs)
        return val
    def AppendSeparator(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenu_AppendSeparator,(self,) + _args, _kwargs)
        return val
    def Break(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenu_Break,(self,) + _args, _kwargs)
        return val
    def Check(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenu_Check,(self,) + _args, _kwargs)
        return val
    def IsChecked(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenu_IsChecked,(self,) + _args, _kwargs)
        return val
    def Enable(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenu_Enable,(self,) + _args, _kwargs)
        return val
    def IsEnabled(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenu_IsEnabled,(self,) + _args, _kwargs)
        return val
    def FindItem(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenu_FindItem,(self,) + _args, _kwargs)
        return val
    def FindItemById(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenu_FindItemById,(self,) + _args, _kwargs)
        return val
    def GetTitle(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenu_GetTitle,(self,) + _args, _kwargs)
        return val
    def SetTitle(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenu_SetTitle,(self,) + _args, _kwargs)
        return val
    def GetLabel(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenu_GetLabel,(self,) + _args, _kwargs)
        return val
    def SetLabel(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenu_SetLabel,(self,) + _args, _kwargs)
        return val
    def GetHelpString(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenu_GetHelpString,(self,) + _args, _kwargs)
        return val
    def SetHelpString(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenu_SetHelpString,(self,) + _args, _kwargs)
        return val
    def UpdateUI(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenu_UpdateUI,(self,) + _args, _kwargs)
        return val
    def Delete(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenu_Delete,(self,) + _args, _kwargs)
        return val
    def DeleteItem(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenu_DeleteItem,(self,) + _args, _kwargs)
        return val
    def Insert(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenu_Insert,(self,) + _args, _kwargs)
        return val
    def Remove(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenu_Remove,(self,) + _args, _kwargs)
        return val
    def RemoveItem(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenu_RemoveItem,(self,) + _args, _kwargs)
        return val
    def Destroy(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenu_Destroy,(self,) + _args, _kwargs)
        return val
    def DestroyId(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenu_DestroyId,(self,) + _args, _kwargs)
        return val
    def DestroyItem(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenu_DestroyItem,(self,) + _args, _kwargs)
        return val
    def GetMenuItemCount(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenu_GetMenuItemCount,(self,) + _args, _kwargs)
        return val
    def GetMenuItems(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenu_GetMenuItems,(self,) + _args, _kwargs)
        return val
    def SetEventHandler(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenu_SetEventHandler,(self,) + _args, _kwargs)
        return val
    def GetEventHandler(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenu_GetEventHandler,(self,) + _args, _kwargs)
        return val
    def SetInvokingWindow(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenu_SetInvokingWindow,(self,) + _args, _kwargs)
        return val
    def GetInvokingWindow(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenu_GetInvokingWindow,(self,) + _args, _kwargs)
        return val
    def GetStyle(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenu_GetStyle,(self,) + _args, _kwargs)
        return val
    def IsAttached(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenu_IsAttached,(self,) + _args, _kwargs)
        return val
    def SetParent(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenu_SetParent,(self,) + _args, _kwargs)
        return val
    def GetParent(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenu_GetParent,(self,) + _args, _kwargs)
        return val
    def __repr__(self):
        return "<C wxMenu instance at %s>" % (self.this,)
class wxMenu(wxMenuPtr):
    def __init__(self,*_args,**_kwargs):
        self.this = apply(windowsc.new_wxMenu,_args,_kwargs)
        self.thisown = 1
        self._setOORInfo(self)




class wxMenuBarPtr(wxWindowPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def Append(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuBar_Append,(self,) + _args, _kwargs)
        return val
    def Insert(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuBar_Insert,(self,) + _args, _kwargs)
        return val
    def GetMenuCount(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuBar_GetMenuCount,(self,) + _args, _kwargs)
        return val
    def GetMenu(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuBar_GetMenu,(self,) + _args, _kwargs)
        return val
    def Replace(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuBar_Replace,(self,) + _args, _kwargs)
        return val
    def Remove(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuBar_Remove,(self,) + _args, _kwargs)
        return val
    def EnableTop(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuBar_EnableTop,(self,) + _args, _kwargs)
        return val
    def SetLabelTop(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuBar_SetLabelTop,(self,) + _args, _kwargs)
        return val
    def GetLabelTop(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuBar_GetLabelTop,(self,) + _args, _kwargs)
        return val
    def FindMenu(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuBar_FindMenu,(self,) + _args, _kwargs)
        return val
    def FindMenuItem(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuBar_FindMenuItem,(self,) + _args, _kwargs)
        return val
    def FindItemById(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuBar_FindItemById,(self,) + _args, _kwargs)
        return val
    def Enable(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuBar_Enable,(self,) + _args, _kwargs)
        return val
    def Check(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuBar_Check,(self,) + _args, _kwargs)
        return val
    def IsChecked(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuBar_IsChecked,(self,) + _args, _kwargs)
        return val
    def IsEnabled(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuBar_IsEnabled,(self,) + _args, _kwargs)
        return val
    def SetLabel(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuBar_SetLabel,(self,) + _args, _kwargs)
        return val
    def GetLabel(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuBar_GetLabel,(self,) + _args, _kwargs)
        return val
    def SetHelpString(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuBar_SetHelpString,(self,) + _args, _kwargs)
        return val
    def GetHelpString(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuBar_GetHelpString,(self,) + _args, _kwargs)
        return val
    def __repr__(self):
        return "<C wxMenuBar instance at %s>" % (self.this,)
class wxMenuBar(wxMenuBarPtr):
    def __init__(self,*_args,**_kwargs):
        self.this = apply(windowsc.new_wxMenuBar,_args,_kwargs)
        self.thisown = 1
        self._setOORInfo(self)




class wxMenuItemPtr(wxObjectPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def GetMenu(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuItem_GetMenu,(self,) + _args, _kwargs)
        return val
    def SetId(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuItem_SetId,(self,) + _args, _kwargs)
        return val
    def GetId(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuItem_GetId,(self,) + _args, _kwargs)
        return val
    def IsSeparator(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuItem_IsSeparator,(self,) + _args, _kwargs)
        return val
    def SetText(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuItem_SetText,(self,) + _args, _kwargs)
        return val
    def GetLabel(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuItem_GetLabel,(self,) + _args, _kwargs)
        return val
    def GetText(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuItem_GetText,(self,) + _args, _kwargs)
        return val
    def SetCheckable(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuItem_SetCheckable,(self,) + _args, _kwargs)
        return val
    def IsCheckable(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuItem_IsCheckable,(self,) + _args, _kwargs)
        return val
    def IsSubMenu(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuItem_IsSubMenu,(self,) + _args, _kwargs)
        return val
    def SetSubMenu(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuItem_SetSubMenu,(self,) + _args, _kwargs)
        return val
    def GetSubMenu(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuItem_GetSubMenu,(self,) + _args, _kwargs)
        return val
    def Enable(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuItem_Enable,(self,) + _args, _kwargs)
        return val
    def IsEnabled(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuItem_IsEnabled,(self,) + _args, _kwargs)
        return val
    def Check(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuItem_Check,(self,) + _args, _kwargs)
        return val
    def IsChecked(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuItem_IsChecked,(self,) + _args, _kwargs)
        return val
    def Toggle(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuItem_Toggle,(self,) + _args, _kwargs)
        return val
    def SetHelp(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuItem_SetHelp,(self,) + _args, _kwargs)
        return val
    def GetHelp(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuItem_GetHelp,(self,) + _args, _kwargs)
        return val
    def GetAccel(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuItem_GetAccel,(self,) + _args, _kwargs)
        if val: val = wxAcceleratorEntryPtr(val) 
        return val
    def SetAccel(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuItem_SetAccel,(self,) + _args, _kwargs)
        return val
    def SetFont(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuItem_SetFont,(self,) + _args, _kwargs)
        return val
    def GetFont(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuItem_GetFont,(self,) + _args, _kwargs)
        if val: val = wxFontPtr(val) 
        return val
    def SetTextColour(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuItem_SetTextColour,(self,) + _args, _kwargs)
        return val
    def GetTextColour(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuItem_GetTextColour,(self,) + _args, _kwargs)
        if val: val = wxColourPtr(val) ; val.thisown = 1
        return val
    def SetBackgroundColour(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuItem_SetBackgroundColour,(self,) + _args, _kwargs)
        return val
    def GetBackgroundColour(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuItem_GetBackgroundColour,(self,) + _args, _kwargs)
        if val: val = wxColourPtr(val) ; val.thisown = 1
        return val
    def SetBitmaps(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuItem_SetBitmaps,(self,) + _args, _kwargs)
        return val
    def SetBitmap(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuItem_SetBitmap,(self,) + _args, _kwargs)
        return val
    def GetBitmap(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuItem_GetBitmap,(self,) + _args, _kwargs)
        if val: val = wxBitmapPtr(val) 
        return val
    def SetMarginWidth(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuItem_SetMarginWidth,(self,) + _args, _kwargs)
        return val
    def GetMarginWidth(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuItem_GetMarginWidth,(self,) + _args, _kwargs)
        return val
    def IsOwnerDrawn(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuItem_IsOwnerDrawn,(self,) + _args, _kwargs)
        return val
    def ResetOwnerDrawn(self, *_args, **_kwargs):
        val = apply(windowsc.wxMenuItem_ResetOwnerDrawn,(self,) + _args, _kwargs)
        return val
    def __repr__(self):
        return "<C wxMenuItem instance at %s>" % (self.this,)
class wxMenuItem(wxMenuItemPtr):
    def __init__(self,*_args,**_kwargs):
        self.this = apply(windowsc.new_wxMenuItem,_args,_kwargs)
        self.thisown = 1






#-------------- FUNCTION WRAPPERS ------------------

def wxWindow_FromHWND(*_args, **_kwargs):
    val = apply(windowsc.wxWindow_FromHWND,_args,_kwargs)
    return val

wxValidator_IsSilent = windowsc.wxValidator_IsSilent

wxValidator_SetBellOnError = windowsc.wxValidator_SetBellOnError

def wxWindow_FindFocus(*_args, **_kwargs):
    val = apply(windowsc.wxWindow_FindFocus,_args,_kwargs)
    return val

wxWindow_NewControlId = windowsc.wxWindow_NewControlId

wxWindow_NextControlId = windowsc.wxWindow_NextControlId

wxWindow_PrevControlId = windowsc.wxWindow_PrevControlId

wxMenuItem_GetLabelFromText = windowsc.wxMenuItem_GetLabelFromText

wxMenuItem_GetDefaultMarginWidth = windowsc.wxMenuItem_GetDefaultMarginWidth



#-------------- VARIABLE WRAPPERS ------------------

