# This file was created automatically by SWIG.
import controls2c

from misc import *

from windows import *

from gdi import *

from clip_dnd import *

from events import *

from controls import *
import wx
class wxListItemAttrPtr :
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def SetTextColour(self, *_args, **_kwargs):
        val = apply(controls2c.wxListItemAttr_SetTextColour,(self,) + _args, _kwargs)
        return val
    def SetBackgroundColour(self, *_args, **_kwargs):
        val = apply(controls2c.wxListItemAttr_SetBackgroundColour,(self,) + _args, _kwargs)
        return val
    def SetFont(self, *_args, **_kwargs):
        val = apply(controls2c.wxListItemAttr_SetFont,(self,) + _args, _kwargs)
        return val
    def HasTextColour(self, *_args, **_kwargs):
        val = apply(controls2c.wxListItemAttr_HasTextColour,(self,) + _args, _kwargs)
        return val
    def HasBackgroundColour(self, *_args, **_kwargs):
        val = apply(controls2c.wxListItemAttr_HasBackgroundColour,(self,) + _args, _kwargs)
        return val
    def HasFont(self, *_args, **_kwargs):
        val = apply(controls2c.wxListItemAttr_HasFont,(self,) + _args, _kwargs)
        return val
    def GetTextColour(self, *_args, **_kwargs):
        val = apply(controls2c.wxListItemAttr_GetTextColour,(self,) + _args, _kwargs)
        if val: val = wxColourPtr(val) 
        return val
    def GetBackgroundColour(self, *_args, **_kwargs):
        val = apply(controls2c.wxListItemAttr_GetBackgroundColour,(self,) + _args, _kwargs)
        if val: val = wxColourPtr(val) 
        return val
    def GetFont(self, *_args, **_kwargs):
        val = apply(controls2c.wxListItemAttr_GetFont,(self,) + _args, _kwargs)
        if val: val = wxFontPtr(val) 
        return val
    def __repr__(self):
        return "<C wxListItemAttr instance at %s>" % (self.this,)
class wxListItemAttr(wxListItemAttrPtr):
    def __init__(self,*_args,**_kwargs):
        self.this = apply(controls2c.new_wxListItemAttr,_args,_kwargs)
        self.thisown = 1




class wxListItemPtr(wxObjectPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def __del__(self,controls2c=controls2c):
        if self.thisown == 1 :
            controls2c.delete_wxListItem(self)
    def Clear(self, *_args, **_kwargs):
        val = apply(controls2c.wxListItem_Clear,(self,) + _args, _kwargs)
        return val
    def ClearAttributes(self, *_args, **_kwargs):
        val = apply(controls2c.wxListItem_ClearAttributes,(self,) + _args, _kwargs)
        return val
    def SetMask(self, *_args, **_kwargs):
        val = apply(controls2c.wxListItem_SetMask,(self,) + _args, _kwargs)
        return val
    def SetId(self, *_args, **_kwargs):
        val = apply(controls2c.wxListItem_SetId,(self,) + _args, _kwargs)
        return val
    def SetColumn(self, *_args, **_kwargs):
        val = apply(controls2c.wxListItem_SetColumn,(self,) + _args, _kwargs)
        return val
    def SetState(self, *_args, **_kwargs):
        val = apply(controls2c.wxListItem_SetState,(self,) + _args, _kwargs)
        return val
    def SetStateMask(self, *_args, **_kwargs):
        val = apply(controls2c.wxListItem_SetStateMask,(self,) + _args, _kwargs)
        return val
    def SetText(self, *_args, **_kwargs):
        val = apply(controls2c.wxListItem_SetText,(self,) + _args, _kwargs)
        return val
    def SetImage(self, *_args, **_kwargs):
        val = apply(controls2c.wxListItem_SetImage,(self,) + _args, _kwargs)
        return val
    def SetData(self, *_args, **_kwargs):
        val = apply(controls2c.wxListItem_SetData,(self,) + _args, _kwargs)
        return val
    def SetWidth(self, *_args, **_kwargs):
        val = apply(controls2c.wxListItem_SetWidth,(self,) + _args, _kwargs)
        return val
    def SetAlign(self, *_args, **_kwargs):
        val = apply(controls2c.wxListItem_SetAlign,(self,) + _args, _kwargs)
        return val
    def SetTextColour(self, *_args, **_kwargs):
        val = apply(controls2c.wxListItem_SetTextColour,(self,) + _args, _kwargs)
        return val
    def SetBackgroundColour(self, *_args, **_kwargs):
        val = apply(controls2c.wxListItem_SetBackgroundColour,(self,) + _args, _kwargs)
        return val
    def SetFont(self, *_args, **_kwargs):
        val = apply(controls2c.wxListItem_SetFont,(self,) + _args, _kwargs)
        return val
    def GetMask(self, *_args, **_kwargs):
        val = apply(controls2c.wxListItem_GetMask,(self,) + _args, _kwargs)
        return val
    def GetId(self, *_args, **_kwargs):
        val = apply(controls2c.wxListItem_GetId,(self,) + _args, _kwargs)
        return val
    def GetColumn(self, *_args, **_kwargs):
        val = apply(controls2c.wxListItem_GetColumn,(self,) + _args, _kwargs)
        return val
    def GetState(self, *_args, **_kwargs):
        val = apply(controls2c.wxListItem_GetState,(self,) + _args, _kwargs)
        return val
    def GetText(self, *_args, **_kwargs):
        val = apply(controls2c.wxListItem_GetText,(self,) + _args, _kwargs)
        return val
    def GetImage(self, *_args, **_kwargs):
        val = apply(controls2c.wxListItem_GetImage,(self,) + _args, _kwargs)
        return val
    def GetData(self, *_args, **_kwargs):
        val = apply(controls2c.wxListItem_GetData,(self,) + _args, _kwargs)
        return val
    def GetWidth(self, *_args, **_kwargs):
        val = apply(controls2c.wxListItem_GetWidth,(self,) + _args, _kwargs)
        return val
    def GetAlign(self, *_args, **_kwargs):
        val = apply(controls2c.wxListItem_GetAlign,(self,) + _args, _kwargs)
        return val
    def GetAttributes(self, *_args, **_kwargs):
        val = apply(controls2c.wxListItem_GetAttributes,(self,) + _args, _kwargs)
        if val: val = wxListItemAttrPtr(val) 
        return val
    def HasAttributes(self, *_args, **_kwargs):
        val = apply(controls2c.wxListItem_HasAttributes,(self,) + _args, _kwargs)
        return val
    def GetTextColour(self, *_args, **_kwargs):
        val = apply(controls2c.wxListItem_GetTextColour,(self,) + _args, _kwargs)
        if val: val = wxColourPtr(val) ; val.thisown = 1
        return val
    def GetBackgroundColour(self, *_args, **_kwargs):
        val = apply(controls2c.wxListItem_GetBackgroundColour,(self,) + _args, _kwargs)
        if val: val = wxColourPtr(val) ; val.thisown = 1
        return val
    def GetFont(self, *_args, **_kwargs):
        val = apply(controls2c.wxListItem_GetFont,(self,) + _args, _kwargs)
        if val: val = wxFontPtr(val) ; val.thisown = 1
        return val
    def __setattr__(self,name,value):
        if name == "m_mask" :
            controls2c.wxListItem_m_mask_set(self,value)
            return
        if name == "m_itemId" :
            controls2c.wxListItem_m_itemId_set(self,value)
            return
        if name == "m_col" :
            controls2c.wxListItem_m_col_set(self,value)
            return
        if name == "m_state" :
            controls2c.wxListItem_m_state_set(self,value)
            return
        if name == "m_stateMask" :
            controls2c.wxListItem_m_stateMask_set(self,value)
            return
        if name == "m_text" :
            controls2c.wxListItem_m_text_set(self,value)
            return
        if name == "m_image" :
            controls2c.wxListItem_m_image_set(self,value)
            return
        if name == "m_data" :
            controls2c.wxListItem_m_data_set(self,value)
            return
        if name == "m_format" :
            controls2c.wxListItem_m_format_set(self,value)
            return
        if name == "m_width" :
            controls2c.wxListItem_m_width_set(self,value)
            return
        self.__dict__[name] = value
    def __getattr__(self,name):
        if name == "m_mask" : 
            return controls2c.wxListItem_m_mask_get(self)
        if name == "m_itemId" : 
            return controls2c.wxListItem_m_itemId_get(self)
        if name == "m_col" : 
            return controls2c.wxListItem_m_col_get(self)
        if name == "m_state" : 
            return controls2c.wxListItem_m_state_get(self)
        if name == "m_stateMask" : 
            return controls2c.wxListItem_m_stateMask_get(self)
        if name == "m_text" : 
            return controls2c.wxListItem_m_text_get(self)
        if name == "m_image" : 
            return controls2c.wxListItem_m_image_get(self)
        if name == "m_data" : 
            return controls2c.wxListItem_m_data_get(self)
        if name == "m_format" : 
            return controls2c.wxListItem_m_format_get(self)
        if name == "m_width" : 
            return controls2c.wxListItem_m_width_get(self)
        raise AttributeError,name
    def __repr__(self):
        return "<C wxListItem instance at %s>" % (self.this,)
class wxListItem(wxListItemPtr):
    def __init__(self,*_args,**_kwargs):
        self.this = apply(controls2c.new_wxListItem,_args,_kwargs)
        self.thisown = 1




class wxListEventPtr(wxNotifyEventPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def GetCode(self, *_args, **_kwargs):
        val = apply(controls2c.wxListEvent_GetCode,(self,) + _args, _kwargs)
        return val
    def GetIndex(self, *_args, **_kwargs):
        val = apply(controls2c.wxListEvent_GetIndex,(self,) + _args, _kwargs)
        return val
    def GetOldIndex(self, *_args, **_kwargs):
        val = apply(controls2c.wxListEvent_GetOldIndex,(self,) + _args, _kwargs)
        return val
    def GetOldItem(self, *_args, **_kwargs):
        val = apply(controls2c.wxListEvent_GetOldItem,(self,) + _args, _kwargs)
        return val
    def GetColumn(self, *_args, **_kwargs):
        val = apply(controls2c.wxListEvent_GetColumn,(self,) + _args, _kwargs)
        return val
    def Cancelled(self, *_args, **_kwargs):
        val = apply(controls2c.wxListEvent_Cancelled,(self,) + _args, _kwargs)
        return val
    def GetPoint(self, *_args, **_kwargs):
        val = apply(controls2c.wxListEvent_GetPoint,(self,) + _args, _kwargs)
        if val: val = wxPointPtr(val) ; val.thisown = 1
        return val
    def GetLabel(self, *_args, **_kwargs):
        val = apply(controls2c.wxListEvent_GetLabel,(self,) + _args, _kwargs)
        return val
    def GetText(self, *_args, **_kwargs):
        val = apply(controls2c.wxListEvent_GetText,(self,) + _args, _kwargs)
        return val
    def GetImage(self, *_args, **_kwargs):
        val = apply(controls2c.wxListEvent_GetImage,(self,) + _args, _kwargs)
        return val
    def GetData(self, *_args, **_kwargs):
        val = apply(controls2c.wxListEvent_GetData,(self,) + _args, _kwargs)
        return val
    def GetMask(self, *_args, **_kwargs):
        val = apply(controls2c.wxListEvent_GetMask,(self,) + _args, _kwargs)
        return val
    def GetItem(self, *_args, **_kwargs):
        val = apply(controls2c.wxListEvent_GetItem,(self,) + _args, _kwargs)
        return val
    def __setattr__(self,name,value):
        if name == "m_code" :
            controls2c.wxListEvent_m_code_set(self,value)
            return
        if name == "m_itemIndex" :
            controls2c.wxListEvent_m_itemIndex_set(self,value)
            return
        if name == "m_oldItemIndex" :
            controls2c.wxListEvent_m_oldItemIndex_set(self,value)
            return
        if name == "m_col" :
            controls2c.wxListEvent_m_col_set(self,value)
            return
        if name == "m_cancelled" :
            controls2c.wxListEvent_m_cancelled_set(self,value)
            return
        if name == "m_pointDrag" :
            controls2c.wxListEvent_m_pointDrag_set(self,value.this)
            return
        if name == "m_item" :
            controls2c.wxListEvent_m_item_set(self,value.this)
            return
        self.__dict__[name] = value
    def __getattr__(self,name):
        if name == "m_code" : 
            return controls2c.wxListEvent_m_code_get(self)
        if name == "m_itemIndex" : 
            return controls2c.wxListEvent_m_itemIndex_get(self)
        if name == "m_oldItemIndex" : 
            return controls2c.wxListEvent_m_oldItemIndex_get(self)
        if name == "m_col" : 
            return controls2c.wxListEvent_m_col_get(self)
        if name == "m_cancelled" : 
            return controls2c.wxListEvent_m_cancelled_get(self)
        if name == "m_pointDrag" : 
            return wxPointPtr(controls2c.wxListEvent_m_pointDrag_get(self))
        if name == "m_item" : 
            return wxListItemPtr(controls2c.wxListEvent_m_item_get(self))
        raise AttributeError,name
    def __repr__(self):
        return "<C wxListEvent instance at %s>" % (self.this,)
class wxListEvent(wxListEventPtr):
    def __init__(self,this):
        self.this = this




class wxListCtrlPtr(wxControlPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def Arrange(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_Arrange,(self,) + _args, _kwargs)
        return val
    def AssignImageList(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_AssignImageList,(self,) + _args, _kwargs)
        _args[0].thisown = 0
        return val
    def DeleteItem(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_DeleteItem,(self,) + _args, _kwargs)
        return val
    def DeleteAllItems(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_DeleteAllItems,(self,) + _args, _kwargs)
        return val
    def DeleteColumn(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_DeleteColumn,(self,) + _args, _kwargs)
        return val
    def DeleteAllColumns(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_DeleteAllColumns,(self,) + _args, _kwargs)
        return val
    def ClearAll(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_ClearAll,(self,) + _args, _kwargs)
        return val
    def EditLabel(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_EditLabel,(self,) + _args, _kwargs)
        return val
    def EndEditLabel(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_EndEditLabel,(self,) + _args, _kwargs)
        return val
    def GetEditControl(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_GetEditControl,(self,) + _args, _kwargs)
        return val
    def EnsureVisible(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_EnsureVisible,(self,) + _args, _kwargs)
        return val
    def FindItem(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_FindItem,(self,) + _args, _kwargs)
        return val
    def FindItemData(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_FindItemData,(self,) + _args, _kwargs)
        return val
    def FindItemAtPos(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_FindItemAtPos,(self,) + _args, _kwargs)
        return val
    def GetColumn(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_GetColumn,(self,) + _args, _kwargs)
        return val
    def GetColumnWidth(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_GetColumnWidth,(self,) + _args, _kwargs)
        return val
    def GetCountPerPage(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_GetCountPerPage,(self,) + _args, _kwargs)
        return val
    def GetImageList(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_GetImageList,(self,) + _args, _kwargs)
        return val
    def GetItemData(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_GetItemData,(self,) + _args, _kwargs)
        return val
    def GetItem(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_GetItem,(self,) + _args, _kwargs)
        return val
    def GetItemPosition(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_GetItemPosition,(self,) + _args, _kwargs)
        if val: val = wxPointPtr(val) ; val.thisown = 1
        return val
    def GetItemRect(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_GetItemRect,(self,) + _args, _kwargs)
        if val: val = wxRectPtr(val) ; val.thisown = 1
        return val
    def GetItemState(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_GetItemState,(self,) + _args, _kwargs)
        return val
    def GetItemCount(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_GetItemCount,(self,) + _args, _kwargs)
        return val
    def GetItemSpacing(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_GetItemSpacing,(self,) + _args, _kwargs)
        return val
    def GetItemText(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_GetItemText,(self,) + _args, _kwargs)
        return val
    def GetNextItem(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_GetNextItem,(self,) + _args, _kwargs)
        return val
    def GetSelectedItemCount(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_GetSelectedItemCount,(self,) + _args, _kwargs)
        return val
    def GetTextColour(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_GetTextColour,(self,) + _args, _kwargs)
        if val: val = wxColourPtr(val) ; val.thisown = 1
        return val
    def SetTextColour(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_SetTextColour,(self,) + _args, _kwargs)
        return val
    def GetTopItem(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_GetTopItem,(self,) + _args, _kwargs)
        return val
    def HitTest(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_HitTest,(self,) + _args, _kwargs)
        return val
    def InsertColumnInfo(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_InsertColumnInfo,(self,) + _args, _kwargs)
        return val
    def InsertColumn(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_InsertColumn,(self,) + _args, _kwargs)
        return val
    def InsertItem(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_InsertItem,(self,) + _args, _kwargs)
        return val
    def InsertStringItem(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_InsertStringItem,(self,) + _args, _kwargs)
        return val
    def InsertImageItem(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_InsertImageItem,(self,) + _args, _kwargs)
        return val
    def InsertImageStringItem(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_InsertImageStringItem,(self,) + _args, _kwargs)
        return val
    def ScrollList(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_ScrollList,(self,) + _args, _kwargs)
        return val
    def SetBackgroundColour(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_SetBackgroundColour,(self,) + _args, _kwargs)
        return val
    def SetColumn(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_SetColumn,(self,) + _args, _kwargs)
        return val
    def SetColumnWidth(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_SetColumnWidth,(self,) + _args, _kwargs)
        return val
    def SetImageList(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_SetImageList,(self,) + _args, _kwargs)
        return val
    def SetItem(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_SetItem,(self,) + _args, _kwargs)
        return val
    def SetStringItem(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_SetStringItem,(self,) + _args, _kwargs)
        return val
    def SetItemData(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_SetItemData,(self,) + _args, _kwargs)
        return val
    def SetItemImage(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_SetItemImage,(self,) + _args, _kwargs)
        return val
    def SetItemPosition(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_SetItemPosition,(self,) + _args, _kwargs)
        return val
    def SetItemState(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_SetItemState,(self,) + _args, _kwargs)
        return val
    def SetItemText(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_SetItemText,(self,) + _args, _kwargs)
        return val
    def SetSingleStyle(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_SetSingleStyle,(self,) + _args, _kwargs)
        return val
    def SetWindowStyleFlag(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_SetWindowStyleFlag,(self,) + _args, _kwargs)
        return val
    def SortItems(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_SortItems,(self,) + _args, _kwargs)
        return val
    def __repr__(self):
        return "<C wxListCtrl instance at %s>" % (self.this,)
    
    def GetItem(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_GetItem,(self,) + _args, _kwargs)
        val.thisown = 1
        return val
    
class wxListCtrl(wxListCtrlPtr):
    def __init__(self,*_args,**_kwargs):
        self.this = apply(controls2c.new_wxListCtrl,_args,_kwargs)
        self.thisown = 1
        #wx._StdWindowCallbacks(self)




class wxTreeItemIdPtr :
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def __del__(self,controls2c=controls2c):
        if self.thisown == 1 :
            controls2c.delete_wxTreeItemId(self)
    def IsOk(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeItemId_IsOk,(self,) + _args, _kwargs)
        return val
    def __cmp__(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeItemId___cmp__,(self,) + _args, _kwargs)
        return val
    def __repr__(self):
        return "<C wxTreeItemId instance at %s>" % (self.this,)
class wxTreeItemId(wxTreeItemIdPtr):
    def __init__(self,*_args,**_kwargs):
        self.this = apply(controls2c.new_wxTreeItemId,_args,_kwargs)
        self.thisown = 1




class wxTreeItemDataPtr(wxObjectPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def GetData(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeItemData_GetData,(self,) + _args, _kwargs)
        return val
    def SetData(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeItemData_SetData,(self,) + _args, _kwargs)
        return val
    def GetId(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeItemData_GetId,(self,) + _args, _kwargs)
        if val: val = wxTreeItemIdPtr(val) 
        return val
    def SetId(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeItemData_SetId,(self,) + _args, _kwargs)
        return val
    def __repr__(self):
        return "<C wxTreeItemData instance at %s>" % (self.this,)
class wxTreeItemData(wxTreeItemDataPtr):
    def __init__(self,*_args,**_kwargs):
        self.this = apply(controls2c.new_wxTreeItemData,_args,_kwargs)
        self.thisown = 1




class wxTreeEventPtr(wxNotifyEventPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def GetItem(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeEvent_GetItem,(self,) + _args, _kwargs)
        if val: val = wxTreeItemIdPtr(val) ; val.thisown = 1
        return val
    def GetOldItem(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeEvent_GetOldItem,(self,) + _args, _kwargs)
        if val: val = wxTreeItemIdPtr(val) ; val.thisown = 1
        return val
    def GetPoint(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeEvent_GetPoint,(self,) + _args, _kwargs)
        if val: val = wxPointPtr(val) ; val.thisown = 1
        return val
    def GetCode(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeEvent_GetCode,(self,) + _args, _kwargs)
        return val
    def GetLabel(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeEvent_GetLabel,(self,) + _args, _kwargs)
        return val
    def __repr__(self):
        return "<C wxTreeEvent instance at %s>" % (self.this,)
class wxTreeEvent(wxTreeEventPtr):
    def __init__(self,this):
        self.this = this




class wxTreeCtrlPtr(wxControlPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def _setSelf(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl__setSelf,(self,) + _args, _kwargs)
        return val
    def AssignImageList(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_AssignImageList,(self,) + _args, _kwargs)
        _args[0].thisown = 0
        return val
    def GetCount(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_GetCount,(self,) + _args, _kwargs)
        return val
    def GetIndent(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_GetIndent,(self,) + _args, _kwargs)
        return val
    def SetIndent(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_SetIndent,(self,) + _args, _kwargs)
        return val
    def GetImageList(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_GetImageList,(self,) + _args, _kwargs)
        return val
    def GetStateImageList(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_GetStateImageList,(self,) + _args, _kwargs)
        return val
    def SetImageList(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_SetImageList,(self,) + _args, _kwargs)
        return val
    def SetStateImageList(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_SetStateImageList,(self,) + _args, _kwargs)
        return val
    def GetSpacing(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_GetSpacing,(self,) + _args, _kwargs)
        return val
    def SetSpacing(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_SetSpacing,(self,) + _args, _kwargs)
        return val
    def GetItemText(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_GetItemText,(self,) + _args, _kwargs)
        return val
    def GetItemImage(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_GetItemImage,(self,) + _args, _kwargs)
        return val
    def GetItemSelectedImage(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_GetItemSelectedImage,(self,) + _args, _kwargs)
        return val
    def SetItemText(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_SetItemText,(self,) + _args, _kwargs)
        return val
    def SetItemImage(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_SetItemImage,(self,) + _args, _kwargs)
        return val
    def SetItemSelectedImage(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_SetItemSelectedImage,(self,) + _args, _kwargs)
        return val
    def SetItemHasChildren(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_SetItemHasChildren,(self,) + _args, _kwargs)
        return val
    def GetItemData(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_GetItemData,(self,) + _args, _kwargs)
        if val: val = wxTreeItemDataPtr(val) 
        return val
    def SetItemData(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_SetItemData,(self,) + _args, _kwargs)
        return val
    def GetPyData(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_GetPyData,(self,) + _args, _kwargs)
        return val
    def SetPyData(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_SetPyData,(self,) + _args, _kwargs)
        return val
    def IsVisible(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_IsVisible,(self,) + _args, _kwargs)
        return val
    def ItemHasChildren(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_ItemHasChildren,(self,) + _args, _kwargs)
        return val
    def IsExpanded(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_IsExpanded,(self,) + _args, _kwargs)
        return val
    def IsSelected(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_IsSelected,(self,) + _args, _kwargs)
        return val
    def GetRootItem(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_GetRootItem,(self,) + _args, _kwargs)
        if val: val = wxTreeItemIdPtr(val) ; val.thisown = 1
        return val
    def GetSelection(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_GetSelection,(self,) + _args, _kwargs)
        if val: val = wxTreeItemIdPtr(val) ; val.thisown = 1
        return val
    def GetItemParent(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_GetItemParent,(self,) + _args, _kwargs)
        if val: val = wxTreeItemIdPtr(val) ; val.thisown = 1
        return val
    def GetSelections(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_GetSelections,(self,) + _args, _kwargs)
        return val
    def GetChildrenCount(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_GetChildrenCount,(self,) + _args, _kwargs)
        return val
    def GetFirstChild(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_GetFirstChild,(self,) + _args, _kwargs)
        return val
    def GetNextChild(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_GetNextChild,(self,) + _args, _kwargs)
        return val
    def GetNextSibling(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_GetNextSibling,(self,) + _args, _kwargs)
        if val: val = wxTreeItemIdPtr(val) ; val.thisown = 1
        return val
    def GetPrevSibling(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_GetPrevSibling,(self,) + _args, _kwargs)
        if val: val = wxTreeItemIdPtr(val) ; val.thisown = 1
        return val
    def GetFirstVisibleItem(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_GetFirstVisibleItem,(self,) + _args, _kwargs)
        if val: val = wxTreeItemIdPtr(val) ; val.thisown = 1
        return val
    def GetNextVisible(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_GetNextVisible,(self,) + _args, _kwargs)
        if val: val = wxTreeItemIdPtr(val) ; val.thisown = 1
        return val
    def GetPrevVisible(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_GetPrevVisible,(self,) + _args, _kwargs)
        if val: val = wxTreeItemIdPtr(val) ; val.thisown = 1
        return val
    def GetLastChild(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_GetLastChild,(self,) + _args, _kwargs)
        if val: val = wxTreeItemIdPtr(val) ; val.thisown = 1
        return val
    def AddRoot(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_AddRoot,(self,) + _args, _kwargs)
        if val: val = wxTreeItemIdPtr(val) ; val.thisown = 1
        return val
    def PrependItem(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_PrependItem,(self,) + _args, _kwargs)
        if val: val = wxTreeItemIdPtr(val) ; val.thisown = 1
        return val
    def InsertItem(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_InsertItem,(self,) + _args, _kwargs)
        if val: val = wxTreeItemIdPtr(val) ; val.thisown = 1
        return val
    def InsertItemBefore(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_InsertItemBefore,(self,) + _args, _kwargs)
        if val: val = wxTreeItemIdPtr(val) ; val.thisown = 1
        return val
    def AppendItem(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_AppendItem,(self,) + _args, _kwargs)
        if val: val = wxTreeItemIdPtr(val) ; val.thisown = 1
        return val
    def Delete(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_Delete,(self,) + _args, _kwargs)
        return val
    def DeleteChildren(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_DeleteChildren,(self,) + _args, _kwargs)
        return val
    def DeleteAllItems(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_DeleteAllItems,(self,) + _args, _kwargs)
        return val
    def Expand(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_Expand,(self,) + _args, _kwargs)
        return val
    def Collapse(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_Collapse,(self,) + _args, _kwargs)
        return val
    def CollapseAndReset(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_CollapseAndReset,(self,) + _args, _kwargs)
        return val
    def Toggle(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_Toggle,(self,) + _args, _kwargs)
        return val
    def Unselect(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_Unselect,(self,) + _args, _kwargs)
        return val
    def UnselectAll(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_UnselectAll,(self,) + _args, _kwargs)
        return val
    def SelectItem(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_SelectItem,(self,) + _args, _kwargs)
        return val
    def EnsureVisible(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_EnsureVisible,(self,) + _args, _kwargs)
        return val
    def ScrollTo(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_ScrollTo,(self,) + _args, _kwargs)
        return val
    def EditLabel(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_EditLabel,(self,) + _args, _kwargs)
        return val
    def GetEditControl(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_GetEditControl,(self,) + _args, _kwargs)
        return val
    def EndEditLabel(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_EndEditLabel,(self,) + _args, _kwargs)
        return val
    def SortChildren(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_SortChildren,(self,) + _args, _kwargs)
        return val
    def SetItemBold(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_SetItemBold,(self,) + _args, _kwargs)
        return val
    def IsBold(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_IsBold,(self,) + _args, _kwargs)
        return val
    def HitTest(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_HitTest,(self,) + _args, _kwargs)
        return val
    def SetItemTextColour(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_SetItemTextColour,(self,) + _args, _kwargs)
        return val
    def SetItemBackgroundColour(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_SetItemBackgroundColour,(self,) + _args, _kwargs)
        return val
    def SetItemFont(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_SetItemFont,(self,) + _args, _kwargs)
        return val
    def SetItemDropHighlight(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_SetItemDropHighlight,(self,) + _args, _kwargs)
        return val
    def GetBoundingRect(self, *_args, **_kwargs):
        val = apply(controls2c.wxTreeCtrl_GetBoundingRect,(self,) + _args, _kwargs)
        return val
    def __repr__(self):
        return "<C wxTreeCtrl instance at %s>" % (self.this,)
    
    # Redefine some methods that SWIG gets a bit confused on...
    def GetFirstChild(self, *_args, **_kwargs):
        val1,val2 = apply(controls2c.wxTreeCtrl_GetFirstChild,(self,) + _args, _kwargs)
        val1 = wxTreeItemIdPtr(val1)
        val1.thisown = 1
        return (val1,val2)
    def GetNextChild(self, *_args, **_kwargs):
        val1,val2 = apply(controls2c.wxTreeCtrl_GetNextChild,(self,) + _args, _kwargs)
        val1 = wxTreeItemIdPtr(val1)
        val1.thisown = 1
        return (val1,val2)
    def HitTest(self, *_args, **_kwargs):
        val1, val2 = apply(controls2c.wxTreeCtrl_HitTest,(self,) + _args, _kwargs)
        val1 = wxTreeItemIdPtr(val1)
        val1.thisown = 1
        return (val1,val2)

class wxTreeCtrl(wxTreeCtrlPtr):
    def __init__(self,*_args,**_kwargs):
        self.this = apply(controls2c.new_wxTreeCtrl,_args,_kwargs)
        self.thisown = 1
        #wx._StdWindowCallbacks(self)
        self._setSelf(self, wxTreeCtrl)






#-------------- FUNCTION WRAPPERS ------------------



#-------------- VARIABLE WRAPPERS ------------------

wxLIST_MASK_TEXT = controls2c.wxLIST_MASK_TEXT
wxLIST_MASK_IMAGE = controls2c.wxLIST_MASK_IMAGE
wxLIST_MASK_DATA = controls2c.wxLIST_MASK_DATA
wxLIST_MASK_WIDTH = controls2c.wxLIST_MASK_WIDTH
wxLIST_MASK_FORMAT = controls2c.wxLIST_MASK_FORMAT
wxLIST_MASK_STATE = controls2c.wxLIST_MASK_STATE
wxLIST_STATE_DONTCARE = controls2c.wxLIST_STATE_DONTCARE
wxLIST_STATE_DROPHILITED = controls2c.wxLIST_STATE_DROPHILITED
wxLIST_STATE_FOCUSED = controls2c.wxLIST_STATE_FOCUSED
wxLIST_STATE_SELECTED = controls2c.wxLIST_STATE_SELECTED
wxLIST_STATE_CUT = controls2c.wxLIST_STATE_CUT
wxLIST_HITTEST_ABOVE = controls2c.wxLIST_HITTEST_ABOVE
wxLIST_HITTEST_BELOW = controls2c.wxLIST_HITTEST_BELOW
wxLIST_HITTEST_NOWHERE = controls2c.wxLIST_HITTEST_NOWHERE
wxLIST_HITTEST_ONITEMICON = controls2c.wxLIST_HITTEST_ONITEMICON
wxLIST_HITTEST_ONITEMLABEL = controls2c.wxLIST_HITTEST_ONITEMLABEL
wxLIST_HITTEST_ONITEMRIGHT = controls2c.wxLIST_HITTEST_ONITEMRIGHT
wxLIST_HITTEST_ONITEMSTATEICON = controls2c.wxLIST_HITTEST_ONITEMSTATEICON
wxLIST_HITTEST_TOLEFT = controls2c.wxLIST_HITTEST_TOLEFT
wxLIST_HITTEST_TORIGHT = controls2c.wxLIST_HITTEST_TORIGHT
wxLIST_HITTEST_ONITEM = controls2c.wxLIST_HITTEST_ONITEM
wxLIST_NEXT_ABOVE = controls2c.wxLIST_NEXT_ABOVE
wxLIST_NEXT_ALL = controls2c.wxLIST_NEXT_ALL
wxLIST_NEXT_BELOW = controls2c.wxLIST_NEXT_BELOW
wxLIST_NEXT_LEFT = controls2c.wxLIST_NEXT_LEFT
wxLIST_NEXT_RIGHT = controls2c.wxLIST_NEXT_RIGHT
wxLIST_ALIGN_DEFAULT = controls2c.wxLIST_ALIGN_DEFAULT
wxLIST_ALIGN_LEFT = controls2c.wxLIST_ALIGN_LEFT
wxLIST_ALIGN_TOP = controls2c.wxLIST_ALIGN_TOP
wxLIST_ALIGN_SNAP_TO_GRID = controls2c.wxLIST_ALIGN_SNAP_TO_GRID
wxLIST_AUTOSIZE = controls2c.wxLIST_AUTOSIZE
wxLIST_AUTOSIZE_USEHEADER = controls2c.wxLIST_AUTOSIZE_USEHEADER
wxLIST_RECT_BOUNDS = controls2c.wxLIST_RECT_BOUNDS
wxLIST_RECT_ICON = controls2c.wxLIST_RECT_ICON
wxLIST_RECT_LABEL = controls2c.wxLIST_RECT_LABEL
wxLIST_FIND_UP = controls2c.wxLIST_FIND_UP
wxLIST_FIND_DOWN = controls2c.wxLIST_FIND_DOWN
wxLIST_FIND_LEFT = controls2c.wxLIST_FIND_LEFT
wxLIST_FIND_RIGHT = controls2c.wxLIST_FIND_RIGHT
wxLIST_FORMAT_LEFT = controls2c.wxLIST_FORMAT_LEFT
wxLIST_FORMAT_RIGHT = controls2c.wxLIST_FORMAT_RIGHT
wxLIST_FORMAT_CENTRE = controls2c.wxLIST_FORMAT_CENTRE
wxLIST_FORMAT_CENTER = controls2c.wxLIST_FORMAT_CENTER
wxEVT_COMMAND_LIST_BEGIN_DRAG = controls2c.wxEVT_COMMAND_LIST_BEGIN_DRAG
wxEVT_COMMAND_LIST_BEGIN_RDRAG = controls2c.wxEVT_COMMAND_LIST_BEGIN_RDRAG
wxEVT_COMMAND_LIST_BEGIN_LABEL_EDIT = controls2c.wxEVT_COMMAND_LIST_BEGIN_LABEL_EDIT
wxEVT_COMMAND_LIST_END_LABEL_EDIT = controls2c.wxEVT_COMMAND_LIST_END_LABEL_EDIT
wxEVT_COMMAND_LIST_DELETE_ITEM = controls2c.wxEVT_COMMAND_LIST_DELETE_ITEM
wxEVT_COMMAND_LIST_DELETE_ALL_ITEMS = controls2c.wxEVT_COMMAND_LIST_DELETE_ALL_ITEMS
wxEVT_COMMAND_LIST_GET_INFO = controls2c.wxEVT_COMMAND_LIST_GET_INFO
wxEVT_COMMAND_LIST_SET_INFO = controls2c.wxEVT_COMMAND_LIST_SET_INFO
wxEVT_COMMAND_LIST_ITEM_SELECTED = controls2c.wxEVT_COMMAND_LIST_ITEM_SELECTED
wxEVT_COMMAND_LIST_ITEM_DESELECTED = controls2c.wxEVT_COMMAND_LIST_ITEM_DESELECTED
wxEVT_COMMAND_LIST_KEY_DOWN = controls2c.wxEVT_COMMAND_LIST_KEY_DOWN
wxEVT_COMMAND_LIST_INSERT_ITEM = controls2c.wxEVT_COMMAND_LIST_INSERT_ITEM
wxEVT_COMMAND_LIST_COL_CLICK = controls2c.wxEVT_COMMAND_LIST_COL_CLICK
wxEVT_COMMAND_LIST_ITEM_ACTIVATED = controls2c.wxEVT_COMMAND_LIST_ITEM_ACTIVATED
wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK = controls2c.wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK
wxEVT_COMMAND_LIST_ITEM_MIDDLE_CLICK = controls2c.wxEVT_COMMAND_LIST_ITEM_MIDDLE_CLICK
wxLC_ICON = controls2c.wxLC_ICON
wxLC_SMALL_ICON = controls2c.wxLC_SMALL_ICON
wxLC_LIST = controls2c.wxLC_LIST
wxLC_REPORT = controls2c.wxLC_REPORT
wxLC_ALIGN_TOP = controls2c.wxLC_ALIGN_TOP
wxLC_ALIGN_LEFT = controls2c.wxLC_ALIGN_LEFT
wxLC_AUTOARRANGE = controls2c.wxLC_AUTOARRANGE
wxLC_USER_TEXT = controls2c.wxLC_USER_TEXT
wxLC_EDIT_LABELS = controls2c.wxLC_EDIT_LABELS
wxLC_NO_HEADER = controls2c.wxLC_NO_HEADER
wxLC_NO_SORT_HEADER = controls2c.wxLC_NO_SORT_HEADER
wxLC_SINGLE_SEL = controls2c.wxLC_SINGLE_SEL
wxLC_SORT_ASCENDING = controls2c.wxLC_SORT_ASCENDING
wxLC_SORT_DESCENDING = controls2c.wxLC_SORT_DESCENDING
wxLC_MASK_TYPE = controls2c.wxLC_MASK_TYPE
wxLC_MASK_ALIGN = controls2c.wxLC_MASK_ALIGN
wxLC_MASK_SORT = controls2c.wxLC_MASK_SORT
wxLC_HRULES = controls2c.wxLC_HRULES
wxLC_VRULES = controls2c.wxLC_VRULES
wxLC_VIRTUAL = controls2c.wxLC_VIRTUAL
wxTreeItemIcon_Normal = controls2c.wxTreeItemIcon_Normal
wxTreeItemIcon_Selected = controls2c.wxTreeItemIcon_Selected
wxTreeItemIcon_Expanded = controls2c.wxTreeItemIcon_Expanded
wxTreeItemIcon_SelectedExpanded = controls2c.wxTreeItemIcon_SelectedExpanded
wxTreeItemIcon_Max = controls2c.wxTreeItemIcon_Max
wxTREE_HITTEST_ABOVE = controls2c.wxTREE_HITTEST_ABOVE
wxTREE_HITTEST_BELOW = controls2c.wxTREE_HITTEST_BELOW
wxTREE_HITTEST_NOWHERE = controls2c.wxTREE_HITTEST_NOWHERE
wxTREE_HITTEST_ONITEMBUTTON = controls2c.wxTREE_HITTEST_ONITEMBUTTON
wxTREE_HITTEST_ONITEMICON = controls2c.wxTREE_HITTEST_ONITEMICON
wxTREE_HITTEST_ONITEMINDENT = controls2c.wxTREE_HITTEST_ONITEMINDENT
wxTREE_HITTEST_ONITEMLABEL = controls2c.wxTREE_HITTEST_ONITEMLABEL
wxTREE_HITTEST_ONITEMRIGHT = controls2c.wxTREE_HITTEST_ONITEMRIGHT
wxTREE_HITTEST_ONITEMSTATEICON = controls2c.wxTREE_HITTEST_ONITEMSTATEICON
wxTREE_HITTEST_TOLEFT = controls2c.wxTREE_HITTEST_TOLEFT
wxTREE_HITTEST_TORIGHT = controls2c.wxTREE_HITTEST_TORIGHT
wxTREE_HITTEST_ONITEMUPPERPART = controls2c.wxTREE_HITTEST_ONITEMUPPERPART
wxTREE_HITTEST_ONITEMLOWERPART = controls2c.wxTREE_HITTEST_ONITEMLOWERPART
wxTREE_HITTEST_ONITEM = controls2c.wxTREE_HITTEST_ONITEM
wxEVT_COMMAND_TREE_BEGIN_DRAG = controls2c.wxEVT_COMMAND_TREE_BEGIN_DRAG
wxEVT_COMMAND_TREE_BEGIN_RDRAG = controls2c.wxEVT_COMMAND_TREE_BEGIN_RDRAG
wxEVT_COMMAND_TREE_BEGIN_LABEL_EDIT = controls2c.wxEVT_COMMAND_TREE_BEGIN_LABEL_EDIT
wxEVT_COMMAND_TREE_END_LABEL_EDIT = controls2c.wxEVT_COMMAND_TREE_END_LABEL_EDIT
wxEVT_COMMAND_TREE_DELETE_ITEM = controls2c.wxEVT_COMMAND_TREE_DELETE_ITEM
wxEVT_COMMAND_TREE_GET_INFO = controls2c.wxEVT_COMMAND_TREE_GET_INFO
wxEVT_COMMAND_TREE_SET_INFO = controls2c.wxEVT_COMMAND_TREE_SET_INFO
wxEVT_COMMAND_TREE_ITEM_EXPANDED = controls2c.wxEVT_COMMAND_TREE_ITEM_EXPANDED
wxEVT_COMMAND_TREE_ITEM_EXPANDING = controls2c.wxEVT_COMMAND_TREE_ITEM_EXPANDING
wxEVT_COMMAND_TREE_ITEM_COLLAPSED = controls2c.wxEVT_COMMAND_TREE_ITEM_COLLAPSED
wxEVT_COMMAND_TREE_ITEM_COLLAPSING = controls2c.wxEVT_COMMAND_TREE_ITEM_COLLAPSING
wxEVT_COMMAND_TREE_SEL_CHANGED = controls2c.wxEVT_COMMAND_TREE_SEL_CHANGED
wxEVT_COMMAND_TREE_SEL_CHANGING = controls2c.wxEVT_COMMAND_TREE_SEL_CHANGING
wxEVT_COMMAND_TREE_KEY_DOWN = controls2c.wxEVT_COMMAND_TREE_KEY_DOWN
wxEVT_COMMAND_TREE_ITEM_ACTIVATED = controls2c.wxEVT_COMMAND_TREE_ITEM_ACTIVATED
wxEVT_COMMAND_TREE_ITEM_RIGHT_CLICK = controls2c.wxEVT_COMMAND_TREE_ITEM_RIGHT_CLICK
wxEVT_COMMAND_TREE_ITEM_MIDDLE_CLICK = controls2c.wxEVT_COMMAND_TREE_ITEM_MIDDLE_CLICK
wxEVT_COMMAND_TREE_END_DRAG = controls2c.wxEVT_COMMAND_TREE_END_DRAG
