# This file was created automatically by SWIG.
import windowsc

from misc import *

from gdi import *
import wx

def wxDLG_PNT(win, point):
    return win.ConvertDialogPointToPixels(point)

def wxDLG_SZE(win, size):
    return win.ConvertDialogSizeToPixels(size)

class wxEvtHandlerPtr :
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def ProcessEvent(self,arg0):
        val = windowsc.wxEvtHandler_ProcessEvent(self.this,arg0.this)
        return val
    def Connect(self,arg0,arg1,arg2,arg3):
        val = windowsc.wxEvtHandler_Connect(self.this,arg0,arg1,arg2,arg3)
        return val
    def __repr__(self):
        return "<C wxEvtHandler instance>"
class wxEvtHandler(wxEvtHandlerPtr):
    def __init__(self,this):
        self.this = this




class wxWindowPtr(wxEvtHandlerPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def CaptureMouse(self):
        val = windowsc.wxWindow_CaptureMouse(self.this)
        return val
    def Center(self,*args):
        val = apply(windowsc.wxWindow_Center,(self.this,)+args)
        return val
    def Centre(self,*args):
        val = apply(windowsc.wxWindow_Centre,(self.this,)+args)
        return val
    def ClientToScreenXY(self,arg0,arg1):
        val = windowsc.wxWindow_ClientToScreenXY(self.this,arg0,arg1)
        return val
    def ClientToScreen(self,arg0):
        val = windowsc.wxWindow_ClientToScreen(self.this,arg0.this)
        val = wxPointPtr(val)
        val.thisown = 1
        return val
    def Close(self,*args):
        val = apply(windowsc.wxWindow_Close,(self.this,)+args)
        return val
    def Destroy(self):
        val = windowsc.wxWindow_Destroy(self.this)
        return val
    def DestroyChildren(self):
        val = windowsc.wxWindow_DestroyChildren(self.this)
        return val
    def DragAcceptFiles(self,arg0):
        val = windowsc.wxWindow_DragAcceptFiles(self.this,arg0)
        return val
    def Enable(self,arg0):
        val = windowsc.wxWindow_Enable(self.this,arg0)
        return val
    def FindWindowById(self,arg0):
        val = windowsc.wxWindow_FindWindowById(self.this,arg0)
        val = wxWindowPtr(val)
        return val
    def FindWindowByName(self,arg0):
        val = windowsc.wxWindow_FindWindowByName(self.this,arg0)
        val = wxWindowPtr(val)
        return val
    def Fit(self):
        val = windowsc.wxWindow_Fit(self.this)
        return val
    def GetBackgroundColour(self):
        val = windowsc.wxWindow_GetBackgroundColour(self.this)
        val = wxColourPtr(val)
        val.thisown = 1
        return val
    def GetCharHeight(self):
        val = windowsc.wxWindow_GetCharHeight(self.this)
        return val
    def GetCharWidth(self):
        val = windowsc.wxWindow_GetCharWidth(self.this)
        return val
    def GetClientSizeTuple(self):
        val = windowsc.wxWindow_GetClientSizeTuple(self.this)
        return val
    def GetClientSize(self):
        val = windowsc.wxWindow_GetClientSize(self.this)
        val = wxSizePtr(val)
        val.thisown = 1
        return val
    def GetConstraints(self):
        val = windowsc.wxWindow_GetConstraints(self.this)
        val = wxLayoutConstraintsPtr(val)
        return val
    def GetDefaultItem(self):
        val = windowsc.wxWindow_GetDefaultItem(self.this)
        val = wxButtonPtr(val)
        return val
    def GetFont(self):
        val = windowsc.wxWindow_GetFont(self.this)
        val = wxFontPtr(val)
        return val
    def GetForegroundColour(self):
        val = windowsc.wxWindow_GetForegroundColour(self.this)
        val = wxColourPtr(val)
        val.thisown = 1
        return val
    def GetGrandParent(self):
        val = windowsc.wxWindow_GetGrandParent(self.this)
        val = wxWindowPtr(val)
        return val
    def GetId(self):
        val = windowsc.wxWindow_GetId(self.this)
        return val
    def GetLabel(self):
        val = windowsc.wxWindow_GetLabel(self.this)
        return val
    def GetName(self):
        val = windowsc.wxWindow_GetName(self.this)
        return val
    def GetParent(self):
        val = windowsc.wxWindow_GetParent(self.this)
        val = wxWindowPtr(val)
        return val
    def GetPositionTuple(self):
        val = windowsc.wxWindow_GetPositionTuple(self.this)
        return val
    def GetPosition(self):
        val = windowsc.wxWindow_GetPosition(self.this)
        val = wxPointPtr(val)
        val.thisown = 1
        return val
    def GetRect(self):
        val = windowsc.wxWindow_GetRect(self.this)
        val = wxRectPtr(val)
        val.thisown = 1
        return val
    def GetReturnCode(self):
        val = windowsc.wxWindow_GetReturnCode(self.this)
        return val
    def GetScrollThumb(self,arg0):
        val = windowsc.wxWindow_GetScrollThumb(self.this,arg0)
        return val
    def GetScrollPos(self,arg0):
        val = windowsc.wxWindow_GetScrollPos(self.this,arg0)
        return val
    def GetScrollRange(self,arg0):
        val = windowsc.wxWindow_GetScrollRange(self.this,arg0)
        return val
    def GetSizeTuple(self):
        val = windowsc.wxWindow_GetSizeTuple(self.this)
        return val
    def GetSize(self):
        val = windowsc.wxWindow_GetSize(self.this)
        val = wxSizePtr(val)
        val.thisown = 1
        return val
    def GetTextExtent(self,arg0):
        val = windowsc.wxWindow_GetTextExtent(self.this,arg0)
        return val
    def GetFullTextExtent(self,arg0,*args):
        argl = map(None,args)
        try: argl[0] = argl[0].this
        except: pass
        args = tuple(argl)
        val = apply(windowsc.wxWindow_GetFullTextExtent,(self.this,arg0,)+args)
        return val
    def GetTitle(self):
        val = windowsc.wxWindow_GetTitle(self.this)
        return val
    def GetWindowStyleFlag(self):
        val = windowsc.wxWindow_GetWindowStyleFlag(self.this)
        return val
    def InitDialog(self):
        val = windowsc.wxWindow_InitDialog(self.this)
        return val
    def IsEnabled(self):
        val = windowsc.wxWindow_IsEnabled(self.this)
        return val
    def IsRetained(self):
        val = windowsc.wxWindow_IsRetained(self.this)
        return val
    def IsShown(self):
        val = windowsc.wxWindow_IsShown(self.this)
        return val
    def Layout(self):
        val = windowsc.wxWindow_Layout(self.this)
        return val
    def LoadFromResource(self,arg0,arg1,*args):
        val = apply(windowsc.wxWindow_LoadFromResource,(self.this,arg0.this,arg1,)+args)
        return val
    def Lower(self):
        val = windowsc.wxWindow_Lower(self.this)
        return val
    def MakeModal(self,arg0):
        val = windowsc.wxWindow_MakeModal(self.this,arg0)
        return val
    def MoveXY(self,arg0,arg1):
        val = windowsc.wxWindow_MoveXY(self.this,arg0,arg1)
        return val
    def Move(self,arg0):
        val = windowsc.wxWindow_Move(self.this,arg0.this)
        return val
    def PopupMenu(self,arg0,arg1,arg2):
        val = windowsc.wxWindow_PopupMenu(self.this,arg0.this,arg1,arg2)
        return val
    def Raise(self):
        val = windowsc.wxWindow_Raise(self.this)
        return val
    def Refresh(self,*args):
        argl = map(None,args)
        try: argl[1] = argl[1].this
        except: pass
        args = tuple(argl)
        val = apply(windowsc.wxWindow_Refresh,(self.this,)+args)
        return val
    def ReleaseMouse(self):
        val = windowsc.wxWindow_ReleaseMouse(self.this)
        return val
    def ScreenToClientXY(self,arg0,arg1):
        val = windowsc.wxWindow_ScreenToClientXY(self.this,arg0,arg1)
        return val
    def ScreenToClient(self,arg0):
        val = windowsc.wxWindow_ScreenToClient(self.this,arg0.this)
        val = wxPointPtr(val)
        val.thisown = 1
        return val
    def ScrollWindow(self,arg0,arg1,*args):
        argl = map(None,args)
        try: argl[0] = argl[0].this
        except: pass
        args = tuple(argl)
        val = apply(windowsc.wxWindow_ScrollWindow,(self.this,arg0,arg1,)+args)
        return val
    def SetAcceleratorTable(self,arg0):
        val = windowsc.wxWindow_SetAcceleratorTable(self.this,arg0.this)
        return val
    def SetAutoLayout(self,arg0):
        val = windowsc.wxWindow_SetAutoLayout(self.this,arg0)
        return val
    def SetBackgroundColour(self,arg0):
        val = windowsc.wxWindow_SetBackgroundColour(self.this,arg0.this)
        return val
    def SetConstraints(self,arg0):
        val = windowsc.wxWindow_SetConstraints(self.this,arg0.this)
        return val
    def SetDoubleClick(self,arg0):
        val = windowsc.wxWindow_SetDoubleClick(self.this,arg0)
        return val
    def SetFocus(self):
        val = windowsc.wxWindow_SetFocus(self.this)
        return val
    def SetFont(self,arg0):
        val = windowsc.wxWindow_SetFont(self.this,arg0.this)
        return val
    def SetForegroundColour(self,arg0):
        val = windowsc.wxWindow_SetForegroundColour(self.this,arg0.this)
        return val
    def SetId(self,arg0):
        val = windowsc.wxWindow_SetId(self.this,arg0)
        return val
    def SetName(self,arg0):
        val = windowsc.wxWindow_SetName(self.this,arg0)
        return val
    def SetReturnCode(self,arg0):
        val = windowsc.wxWindow_SetReturnCode(self.this,arg0)
        return val
    def SetScrollbar(self,arg0,arg1,arg2,arg3,*args):
        val = apply(windowsc.wxWindow_SetScrollbar,(self.this,arg0,arg1,arg2,arg3,)+args)
        return val
    def SetScrollPos(self,arg0,arg1,*args):
        val = apply(windowsc.wxWindow_SetScrollPos,(self.this,arg0,arg1,)+args)
        return val
    def SetDimensions(self,arg0,arg1,arg2,arg3,*args):
        val = apply(windowsc.wxWindow_SetDimensions,(self.this,arg0,arg1,arg2,arg3,)+args)
        return val
    def SetSize(self,arg0):
        val = windowsc.wxWindow_SetSize(self.this,arg0.this)
        return val
    def SetPosition(self,arg0):
        val = windowsc.wxWindow_SetPosition(self.this,arg0.this)
        return val
    def SetSizeHints(self,*args):
        val = apply(windowsc.wxWindow_SetSizeHints,(self.this,)+args)
        return val
    def SetClientSizeWH(self,arg0,arg1):
        val = windowsc.wxWindow_SetClientSizeWH(self.this,arg0,arg1)
        return val
    def SetClientSize(self,arg0):
        val = windowsc.wxWindow_SetClientSize(self.this,arg0.this)
        return val
    def SetCursor(self,arg0):
        val = windowsc.wxWindow_SetCursor(self.this,arg0.this)
        return val
    def SetTitle(self,arg0):
        val = windowsc.wxWindow_SetTitle(self.this,arg0)
        return val
    def Show(self,arg0):
        val = windowsc.wxWindow_Show(self.this,arg0)
        return val
    def TransferDataFromWindow(self):
        val = windowsc.wxWindow_TransferDataFromWindow(self.this)
        return val
    def TransferDataToWindow(self):
        val = windowsc.wxWindow_TransferDataToWindow(self.this)
        return val
    def Validate(self):
        val = windowsc.wxWindow_Validate(self.this)
        return val
    def WarpPointer(self,arg0,arg1):
        val = windowsc.wxWindow_WarpPointer(self.this,arg0,arg1)
        return val
    def ConvertDialogPointToPixels(self,arg0):
        val = windowsc.wxWindow_ConvertDialogPointToPixels(self.this,arg0.this)
        val = wxPointPtr(val)
        val.thisown = 1
        return val
    def ConvertDialogSizeToPixels(self,arg0):
        val = windowsc.wxWindow_ConvertDialogSizeToPixels(self.this,arg0.this)
        val = wxSizePtr(val)
        val.thisown = 1
        return val
    def DLG_PNT(self,arg0):
        val = windowsc.wxWindow_DLG_PNT(self.this,arg0.this)
        val = wxPointPtr(val)
        val.thisown = 1
        return val
    def DLG_SZE(self,arg0):
        val = windowsc.wxWindow_DLG_SZE(self.this,arg0.this)
        val = wxSizePtr(val)
        val.thisown = 1
        return val
    def ConvertPixelPointToDialog(self,arg0):
        val = windowsc.wxWindow_ConvertPixelPointToDialog(self.this,arg0.this)
        val = wxPointPtr(val)
        val.thisown = 1
        return val
    def ConvertPixelSizeToDialog(self,arg0):
        val = windowsc.wxWindow_ConvertPixelSizeToDialog(self.this,arg0.this)
        val = wxSizePtr(val)
        val.thisown = 1
        return val
    def SetToolTipString(self,arg0):
        val = windowsc.wxWindow_SetToolTipString(self.this,arg0)
        return val
    def SetToolTip(self,arg0):
        val = windowsc.wxWindow_SetToolTip(self.this,arg0.this)
        return val
    def GetToolTip(self):
        val = windowsc.wxWindow_GetToolTip(self.this)
        val = wxToolTipPtr(val)
        return val
    def __repr__(self):
        return "<C wxWindow instance>"
class wxWindow(wxWindowPtr):
    def __init__(self,arg0,arg1,*args) :
        argl = map(None,args)
        try: argl[0] = argl[0].this
        except: pass
        try: argl[1] = argl[1].this
        except: pass
        args = tuple(argl)
        self.this = apply(windowsc.new_wxWindow,(arg0.this,arg1,)+args)
        self.thisown = 1
        wx._StdWindowCallbacks(self)




class wxPanelPtr(wxWindowPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def InitDialog(self):
        val = windowsc.wxPanel_InitDialog(self.this)
        return val
    def __repr__(self):
        return "<C wxPanel instance>"
class wxPanel(wxPanelPtr):
    def __init__(self,arg0,arg1,*args) :
        argl = map(None,args)
        try: argl[0] = argl[0].this
        except: pass
        try: argl[1] = argl[1].this
        except: pass
        args = tuple(argl)
        self.this = apply(windowsc.new_wxPanel,(arg0.this,arg1,)+args)
        self.thisown = 1
        wx._StdWindowCallbacks(self)




class wxDialogPtr(wxPanelPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def Centre(self,*args):
        val = apply(windowsc.wxDialog_Centre,(self.this,)+args)
        return val
    def EndModal(self,arg0):
        val = windowsc.wxDialog_EndModal(self.this,arg0)
        return val
    def GetTitle(self):
        val = windowsc.wxDialog_GetTitle(self.this)
        return val
    def Iconize(self,arg0):
        val = windowsc.wxDialog_Iconize(self.this,arg0)
        return val
    def IsIconized(self):
        val = windowsc.wxDialog_IsIconized(self.this)
        return val
    def SetModal(self,arg0):
        val = windowsc.wxDialog_SetModal(self.this,arg0)
        return val
    def IsModal(self):
        val = windowsc.wxDialog_IsModal(self.this)
        return val
    def SetTitle(self,arg0):
        val = windowsc.wxDialog_SetTitle(self.this,arg0)
        return val
    def Show(self,arg0):
        val = windowsc.wxDialog_Show(self.this,arg0)
        return val
    def ShowModal(self):
        val = windowsc.wxDialog_ShowModal(self.this)
        return val
    def __repr__(self):
        return "<C wxDialog instance>"
class wxDialog(wxDialogPtr):
    def __init__(self,arg0,arg1,arg2,*args) :
        argl = map(None,args)
        try: argl[0] = argl[0].this
        except: pass
        try: argl[1] = argl[1].this
        except: pass
        args = tuple(argl)
        self.this = apply(windowsc.new_wxDialog,(arg0.this,arg1,arg2,)+args)
        self.thisown = 1
        wx._StdDialogCallbacks(self)




class wxScrolledWindowPtr(wxWindowPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def EnableScrolling(self,arg0,arg1):
        val = windowsc.wxScrolledWindow_EnableScrolling(self.this,arg0,arg1)
        return val
    def GetScrollPixelsPerUnit(self):
        val = windowsc.wxScrolledWindow_GetScrollPixelsPerUnit(self.this)
        return val
    def GetVirtualSize(self):
        val = windowsc.wxScrolledWindow_GetVirtualSize(self.this)
        return val
    def IsRetained(self):
        val = windowsc.wxScrolledWindow_IsRetained(self.this)
        return val
    def PrepareDC(self,arg0):
        val = windowsc.wxScrolledWindow_PrepareDC(self.this,arg0.this)
        return val
    def Scroll(self,arg0,arg1):
        val = windowsc.wxScrolledWindow_Scroll(self.this,arg0,arg1)
        return val
    def SetScrollbars(self,arg0,arg1,arg2,arg3,*args):
        val = apply(windowsc.wxScrolledWindow_SetScrollbars,(self.this,arg0,arg1,arg2,arg3,)+args)
        return val
    def ViewStart(self):
        val = windowsc.wxScrolledWindow_ViewStart(self.this)
        return val
    def __repr__(self):
        return "<C wxScrolledWindow instance>"
class wxScrolledWindow(wxScrolledWindowPtr):
    def __init__(self,arg0,*args) :
        argl = map(None,args)
        try: argl[1] = argl[1].this
        except: pass
        try: argl[2] = argl[2].this
        except: pass
        args = tuple(argl)
        self.this = apply(windowsc.new_wxScrolledWindow,(arg0.this,)+args)
        self.thisown = 1
        wx._StdWindowCallbacks(self)
        wx._StdOnScrollCallbacks(self)




class wxMenuPtr(wxEvtHandlerPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def Append(self,arg0,arg1,*args):
        val = apply(windowsc.wxMenu_Append,(self.this,arg0,arg1,)+args)
        return val
    def AppendMenu(self,arg0,arg1,arg2,*args):
        val = apply(windowsc.wxMenu_AppendMenu,(self.this,arg0,arg1,arg2.this,)+args)
        return val
    def AppendItem(self,arg0):
        val = windowsc.wxMenu_AppendItem(self.this,arg0.this)
        return val
    def AppendSeparator(self):
        val = windowsc.wxMenu_AppendSeparator(self.this)
        return val
    def Break(self):
        val = windowsc.wxMenu_Break(self.this)
        return val
    def Check(self,arg0,arg1):
        val = windowsc.wxMenu_Check(self.this,arg0,arg1)
        return val
    def Enable(self,arg0,arg1):
        val = windowsc.wxMenu_Enable(self.this,arg0,arg1)
        return val
    def FindItem(self,arg0):
        val = windowsc.wxMenu_FindItem(self.this,arg0)
        return val
    def GetTitle(self):
        val = windowsc.wxMenu_GetTitle(self.this)
        return val
    def SetTitle(self,arg0):
        val = windowsc.wxMenu_SetTitle(self.this,arg0)
        return val
    def FindItemForId(self,arg0):
        val = windowsc.wxMenu_FindItemForId(self.this,arg0)
        val = wxMenuItemPtr(val)
        return val
    def GetHelpString(self,arg0):
        val = windowsc.wxMenu_GetHelpString(self.this,arg0)
        return val
    def GetLabel(self,arg0):
        val = windowsc.wxMenu_GetLabel(self.this,arg0)
        return val
    def SetHelpString(self,arg0,arg1):
        val = windowsc.wxMenu_SetHelpString(self.this,arg0,arg1)
        return val
    def IsChecked(self,arg0):
        val = windowsc.wxMenu_IsChecked(self.this,arg0)
        return val
    def IsEnabled(self,arg0):
        val = windowsc.wxMenu_IsEnabled(self.this,arg0)
        return val
    def SetLabel(self,arg0,arg1):
        val = windowsc.wxMenu_SetLabel(self.this,arg0,arg1)
        return val
    def __repr__(self):
        return "<C wxMenu instance>"
class wxMenu(wxMenuPtr):
    def __init__(self,*args) :
        self.this = apply(windowsc.new_wxMenu,()+args)
        self.thisown = 1




class wxPyMenuPtr(wxMenuPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def __del__(self):
        if self.thisown == 1 :
            windowsc.delete_wxPyMenu(self.this)
    def __repr__(self):
        return "<C wxPyMenu instance>"
class wxPyMenu(wxPyMenuPtr):
    def __init__(self,*args) :
        self.this = apply(windowsc.new_wxPyMenu,()+args)
        self.thisown = 1




class wxMenuBarPtr(wxEvtHandlerPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def Append(self,arg0,arg1):
        val = windowsc.wxMenuBar_Append(self.this,arg0.this,arg1)
        return val
    def Check(self,arg0,arg1):
        val = windowsc.wxMenuBar_Check(self.this,arg0,arg1)
        return val
    def Checked(self,arg0):
        val = windowsc.wxMenuBar_Checked(self.this,arg0)
        return val
    def Enable(self,arg0,arg1):
        val = windowsc.wxMenuBar_Enable(self.this,arg0,arg1)
        return val
    def Enabled(self,arg0):
        val = windowsc.wxMenuBar_Enabled(self.this,arg0)
        return val
    def FindMenuItem(self,arg0,arg1):
        val = windowsc.wxMenuBar_FindMenuItem(self.this,arg0,arg1)
        return val
    def FindItemForId(self,arg0):
        val = windowsc.wxMenuBar_FindItemForId(self.this,arg0)
        val = wxMenuItemPtr(val)
        return val
    def EnableTop(self,arg0,arg1):
        val = windowsc.wxMenuBar_EnableTop(self.this,arg0,arg1)
        return val
    def GetHelpString(self,arg0):
        val = windowsc.wxMenuBar_GetHelpString(self.this,arg0)
        return val
    def GetLabel(self,arg0):
        val = windowsc.wxMenuBar_GetLabel(self.this,arg0)
        return val
    def SetHelpString(self,arg0,arg1):
        val = windowsc.wxMenuBar_SetHelpString(self.this,arg0,arg1)
        return val
    def SetLabel(self,arg0,arg1):
        val = windowsc.wxMenuBar_SetLabel(self.this,arg0,arg1)
        return val
    def GetLabelTop(self,arg0):
        val = windowsc.wxMenuBar_GetLabelTop(self.this,arg0)
        return val
    def SetLabelTop(self,arg0,arg1):
        val = windowsc.wxMenuBar_SetLabelTop(self.this,arg0,arg1)
        return val
    def GetMenuCount(self):
        val = windowsc.wxMenuBar_GetMenuCount(self.this)
        return val
    def GetMenu(self,arg0):
        val = windowsc.wxMenuBar_GetMenu(self.this,arg0)
        val = wxMenuPtr(val)
        return val
    def __repr__(self):
        return "<C wxMenuBar instance>"
class wxMenuBar(wxMenuBarPtr):
    def __init__(self) :
        self.this = windowsc.new_wxMenuBar()
        self.thisown = 1




class wxMenuItemPtr :
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def IsSeparator(self):
        val = windowsc.wxMenuItem_IsSeparator(self.this)
        return val
    def IsEnabled(self):
        val = windowsc.wxMenuItem_IsEnabled(self.this)
        return val
    def IsChecked(self):
        val = windowsc.wxMenuItem_IsChecked(self.this)
        return val
    def IsCheckable(self):
        val = windowsc.wxMenuItem_IsCheckable(self.this)
        return val
    def GetId(self):
        val = windowsc.wxMenuItem_GetId(self.this)
        return val
    def GetSubMenu(self):
        val = windowsc.wxMenuItem_GetSubMenu(self.this)
        val = wxMenuPtr(val)
        return val
    def SetName(self,arg0):
        val = windowsc.wxMenuItem_SetName(self.this,arg0)
        return val
    def GetName(self):
        val = windowsc.wxMenuItem_GetName(self.this)
        return val
    def GetHelp(self):
        val = windowsc.wxMenuItem_GetHelp(self.this)
        return val
    def SetHelp(self,arg0):
        val = windowsc.wxMenuItem_SetHelp(self.this,arg0)
        return val
    def Enable(self,*args):
        val = apply(windowsc.wxMenuItem_Enable,(self.this,)+args)
        return val
    def Check(self,*args):
        val = apply(windowsc.wxMenuItem_Check,(self.this,)+args)
        return val
    def GetBackgroundColour(self):
        val = windowsc.wxMenuItem_GetBackgroundColour(self.this)
        val = wxColourPtr(val)
        return val
    def GetBitmap(self,*args):
        val = apply(windowsc.wxMenuItem_GetBitmap,(self.this,)+args)
        val = wxBitmapPtr(val)
        val.thisown = 1
        return val
    def GetFont(self):
        val = windowsc.wxMenuItem_GetFont(self.this)
        val = wxFontPtr(val)
        return val
    def GetMarginWidth(self):
        val = windowsc.wxMenuItem_GetMarginWidth(self.this)
        return val
    def GetTextColour(self):
        val = windowsc.wxMenuItem_GetTextColour(self.this)
        val = wxColourPtr(val)
        return val
    def SetBackgroundColour(self,arg0):
        val = windowsc.wxMenuItem_SetBackgroundColour(self.this,arg0.this)
        return val
    def SetBitmaps(self,arg0,*args):
        argl = map(None,args)
        try: argl[0] = argl[0].this
        except: pass
        args = tuple(argl)
        val = apply(windowsc.wxMenuItem_SetBitmaps,(self.this,arg0.this,)+args)
        return val
    def SetFont(self,arg0):
        val = windowsc.wxMenuItem_SetFont(self.this,arg0.this)
        return val
    def SetMarginWidth(self,arg0):
        val = windowsc.wxMenuItem_SetMarginWidth(self.this,arg0)
        return val
    def SetTextColour(self,arg0):
        val = windowsc.wxMenuItem_SetTextColour(self.this,arg0.this)
        return val
    def DeleteSubMenu(self):
        val = windowsc.wxMenuItem_DeleteSubMenu(self.this)
        return val
    def __repr__(self):
        return "<C wxMenuItem instance>"
class wxMenuItem(wxMenuItemPtr):
    def __init__(self,*args) :
        argl = map(None,args)
        try: argl[0] = argl[0].this
        except: pass
        try: argl[5] = argl[5].this
        except: pass
        args = tuple(argl)
        self.this = apply(windowsc.new_wxMenuItem,()+args)
        self.thisown = 1






#-------------- FUNCTION WRAPPERS ------------------

def wxWindow_FindFocus():
    val = windowsc.wxWindow_FindFocus()
    val = wxWindowPtr(val)
    return val

def wxWindow_FromHWND(arg0):
    val = windowsc.wxWindow_FromHWND(arg0)
    val = wxWindowPtr(val)
    return val



#-------------- VARIABLE WRAPPERS ------------------

