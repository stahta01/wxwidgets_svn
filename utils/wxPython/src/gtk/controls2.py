# This file was created automatically by SWIG.
import controls2c

from misc import *

from windows import *

from gdi import *

from events import *

from controls import *
import wx
class wxListItemPtr :
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def __del__(self, controls2c=controls2c):
        if self.thisown == 1 :
            controls2c.delete_wxListItem(self.this)
    def __setattr__(self,name,value):
        if name == "m_mask" :
            controls2c.wxListItem_m_mask_set(self.this,value)
            return
        if name == "m_itemId" :
            controls2c.wxListItem_m_itemId_set(self.this,value)
            return
        if name == "m_col" :
            controls2c.wxListItem_m_col_set(self.this,value)
            return
        if name == "m_state" :
            controls2c.wxListItem_m_state_set(self.this,value)
            return
        if name == "m_stateMask" :
            controls2c.wxListItem_m_stateMask_set(self.this,value)
            return
        if name == "m_text" :
            controls2c.wxListItem_m_text_set(self.this,value)
            return
        if name == "m_image" :
            controls2c.wxListItem_m_image_set(self.this,value)
            return
        if name == "m_data" :
            controls2c.wxListItem_m_data_set(self.this,value)
            return
        if name == "m_format" :
            controls2c.wxListItem_m_format_set(self.this,value)
            return
        if name == "m_width" :
            controls2c.wxListItem_m_width_set(self.this,value)
            return
        self.__dict__[name] = value
    def __getattr__(self,name):
        if name == "m_mask" : 
            return controls2c.wxListItem_m_mask_get(self.this)
        if name == "m_itemId" : 
            return controls2c.wxListItem_m_itemId_get(self.this)
        if name == "m_col" : 
            return controls2c.wxListItem_m_col_get(self.this)
        if name == "m_state" : 
            return controls2c.wxListItem_m_state_get(self.this)
        if name == "m_stateMask" : 
            return controls2c.wxListItem_m_stateMask_get(self.this)
        if name == "m_text" : 
            return controls2c.wxListItem_m_text_get(self.this)
        if name == "m_image" : 
            return controls2c.wxListItem_m_image_get(self.this)
        if name == "m_data" : 
            return controls2c.wxListItem_m_data_get(self.this)
        if name == "m_format" : 
            return controls2c.wxListItem_m_format_get(self.this)
        if name == "m_width" : 
            return controls2c.wxListItem_m_width_get(self.this)
        raise AttributeError,name
    def __repr__(self):
        return "<C wxListItem instance>"
class wxListItem(wxListItemPtr):
    def __init__(self) :
        self.this = controls2c.new_wxListItem()
        self.thisown = 1




class wxListEventPtr(wxCommandEventPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def __setattr__(self,name,value):
        if name == "m_code" :
            controls2c.wxListEvent_m_code_set(self.this,value)
            return
        if name == "m_itemIndex" :
            controls2c.wxListEvent_m_itemIndex_set(self.this,value)
            return
        if name == "m_oldItemIndex" :
            controls2c.wxListEvent_m_oldItemIndex_set(self.this,value)
            return
        if name == "m_col" :
            controls2c.wxListEvent_m_col_set(self.this,value)
            return
        if name == "m_cancelled" :
            controls2c.wxListEvent_m_cancelled_set(self.this,value)
            return
        if name == "m_pointDrag" :
            controls2c.wxListEvent_m_pointDrag_set(self.this,value.this)
            return
        if name == "m_item" :
            controls2c.wxListEvent_m_item_set(self.this,value.this)
            return
        self.__dict__[name] = value
    def __getattr__(self,name):
        if name == "m_code" : 
            return controls2c.wxListEvent_m_code_get(self.this)
        if name == "m_itemIndex" : 
            return controls2c.wxListEvent_m_itemIndex_get(self.this)
        if name == "m_oldItemIndex" : 
            return controls2c.wxListEvent_m_oldItemIndex_get(self.this)
        if name == "m_col" : 
            return controls2c.wxListEvent_m_col_get(self.this)
        if name == "m_cancelled" : 
            return controls2c.wxListEvent_m_cancelled_get(self.this)
        if name == "m_pointDrag" : 
            return wxPointPtr(controls2c.wxListEvent_m_pointDrag_get(self.this))
        if name == "m_item" : 
            return wxListItemPtr(controls2c.wxListEvent_m_item_get(self.this))
        raise AttributeError,name
    def __repr__(self):
        return "<C wxListEvent instance>"
class wxListEvent(wxListEventPtr):
    def __init__(self,this):
        self.this = this




class wxListCtrlPtr(wxControlPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def Arrange(self,*args):
        val = apply(controls2c.wxListCtrl_Arrange,(self.this,)+args)
        return val
    def DeleteItem(self,arg0):
        val = controls2c.wxListCtrl_DeleteItem(self.this,arg0)
        return val
    def DeleteAllItems(self):
        val = controls2c.wxListCtrl_DeleteAllItems(self.this)
        return val
    def DeleteColumn(self,arg0):
        val = controls2c.wxListCtrl_DeleteColumn(self.this,arg0)
        return val
    def DeleteAllColumns(self):
        val = controls2c.wxListCtrl_DeleteAllColumns(self.this)
        return val
    def ClearAll(self):
        val = controls2c.wxListCtrl_ClearAll(self.this)
        return val
    def EditLabel(self,arg0):
        val = controls2c.wxListCtrl_EditLabel(self.this,arg0)
        return val
    def EnsureVisible(self,arg0):
        val = controls2c.wxListCtrl_EnsureVisible(self.this,arg0)
        return val
    def FindItem(self,arg0,arg1,*args):
        val = apply(controls2c.wxListCtrl_FindItem,(self.this,arg0,arg1,)+args)
        return val
    def FindItemData(self,arg0,arg1):
        val = controls2c.wxListCtrl_FindItemData(self.this,arg0,arg1)
        return val
    def FindItemAtPos(self,arg0,arg1,arg2):
        val = controls2c.wxListCtrl_FindItemAtPos(self.this,arg0,arg1.this,arg2)
        return val
    def GetColumn(self,arg0,arg1):
        val = controls2c.wxListCtrl_GetColumn(self.this,arg0,arg1.this)
        return val
    def GetColumnWidth(self,arg0):
        val = controls2c.wxListCtrl_GetColumnWidth(self.this,arg0)
        return val
    def GetCountPerPage(self):
        val = controls2c.wxListCtrl_GetCountPerPage(self.this)
        return val
    def GetImageList(self,arg0):
        val = controls2c.wxListCtrl_GetImageList(self.this,arg0)
        val = wxImageListPtr(val)
        return val
    def GetItemData(self,arg0):
        val = controls2c.wxListCtrl_GetItemData(self.this,arg0)
        return val
    def GetItem(self,arg0):
        val = controls2c.wxListCtrl_GetItem(self.this,arg0)
        val = wxListItemPtr(val)
        val.thisown = 1
        return val
    def GetItemPosition(self,arg0):
        val = controls2c.wxListCtrl_GetItemPosition(self.this,arg0)
        val = wxPointPtr(val)
        val.thisown = 1
        return val
    def GetItemRect(self,arg0,*args):
        val = apply(controls2c.wxListCtrl_GetItemRect,(self.this,arg0,)+args)
        val = wxRectPtr(val)
        val.thisown = 1
        return val
    def GetItemState(self,arg0,arg1):
        val = controls2c.wxListCtrl_GetItemState(self.this,arg0,arg1)
        return val
    def GetItemCount(self):
        val = controls2c.wxListCtrl_GetItemCount(self.this)
        return val
    def GetItemSpacing(self,arg0):
        val = controls2c.wxListCtrl_GetItemSpacing(self.this,arg0)
        return val
    def GetItemText(self,arg0):
        val = controls2c.wxListCtrl_GetItemText(self.this,arg0)
        return val
    def GetNextItem(self,arg0,*args):
        val = apply(controls2c.wxListCtrl_GetNextItem,(self.this,arg0,)+args)
        return val
    def GetSelectedItemCount(self):
        val = controls2c.wxListCtrl_GetSelectedItemCount(self.this)
        return val
    def GetTopItem(self):
        val = controls2c.wxListCtrl_GetTopItem(self.this)
        return val
    def HitTest(self,arg0):
        val = controls2c.wxListCtrl_HitTest(self.this,arg0.this)
        return val
    def InsertColumnWith(self,arg0,arg1):
        val = controls2c.wxListCtrl_InsertColumnWith(self.this,arg0,arg1.this)
        return val
    def InsertColumn(self,arg0,arg1,*args):
        val = apply(controls2c.wxListCtrl_InsertColumn,(self.this,arg0,arg1,)+args)
        return val
    def InsertItem(self,arg0):
        val = controls2c.wxListCtrl_InsertItem(self.this,arg0.this)
        return val
    def InsertStringItem(self,arg0,arg1):
        val = controls2c.wxListCtrl_InsertStringItem(self.this,arg0,arg1)
        return val
    def InsertImageItem(self,arg0,arg1):
        val = controls2c.wxListCtrl_InsertImageItem(self.this,arg0,arg1)
        return val
    def InsertImageStringItem(self,arg0,arg1,arg2):
        val = controls2c.wxListCtrl_InsertImageStringItem(self.this,arg0,arg1,arg2)
        return val
    def ScrollList(self,arg0,arg1):
        val = controls2c.wxListCtrl_ScrollList(self.this,arg0,arg1)
        return val
    def SetBackgroundColour(self,arg0):
        val = controls2c.wxListCtrl_SetBackgroundColour(self.this,arg0.this)
        return val
    def SetColumn(self,arg0,arg1):
        val = controls2c.wxListCtrl_SetColumn(self.this,arg0,arg1.this)
        return val
    def SetColumnWidth(self,arg0,arg1):
        val = controls2c.wxListCtrl_SetColumnWidth(self.this,arg0,arg1)
        return val
    def SetImageList(self,arg0,arg1):
        val = controls2c.wxListCtrl_SetImageList(self.this,arg0.this,arg1)
        return val
    def SetItem(self,arg0):
        val = controls2c.wxListCtrl_SetItem(self.this,arg0.this)
        return val
    def SetStringItem(self,arg0,arg1,arg2,*args):
        val = apply(controls2c.wxListCtrl_SetStringItem,(self.this,arg0,arg1,arg2,)+args)
        return val
    def SetItemData(self,arg0,arg1):
        val = controls2c.wxListCtrl_SetItemData(self.this,arg0,arg1)
        return val
    def SetItemImage(self,arg0,arg1,arg2):
        val = controls2c.wxListCtrl_SetItemImage(self.this,arg0,arg1,arg2)
        return val
    def SetItemPosition(self,arg0,arg1):
        val = controls2c.wxListCtrl_SetItemPosition(self.this,arg0,arg1.this)
        return val
    def SetItemState(self,arg0,arg1,arg2):
        val = controls2c.wxListCtrl_SetItemState(self.this,arg0,arg1,arg2)
        return val
    def SetItemText(self,arg0,arg1):
        val = controls2c.wxListCtrl_SetItemText(self.this,arg0,arg1)
        return val
    def SetSingleStyle(self,arg0,*args):
        val = apply(controls2c.wxListCtrl_SetSingleStyle,(self.this,arg0,)+args)
        return val
    def SetWindowStyleFlag(self,arg0):
        val = controls2c.wxListCtrl_SetWindowStyleFlag(self.this,arg0)
        return val
    def __repr__(self):
        return "<C wxListCtrl instance>"
class wxListCtrl(wxListCtrlPtr):
    def __init__(self,arg0,arg1,*args) :
        argl = map(None,args)
        try: argl[0] = argl[0].this
        except: pass
        try: argl[1] = argl[1].this
        except: pass
        args = tuple(argl)
        self.this = apply(controls2c.new_wxListCtrl,(arg0.this,arg1,)+args)
        self.thisown = 1
        wx._StdWindowCallbacks(self)




class wxTreeItemIdPtr :
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def __del__(self, controls2c=controls2c):
        if self.thisown == 1 :
            controls2c.delete_wxTreeItemId(self.this)
    def IsOk(self):
        val = controls2c.wxTreeItemId_IsOk(self.this)
        return val
    def __repr__(self):
        return "<C wxTreeItemId instance>"
class wxTreeItemId(wxTreeItemIdPtr):
    def __init__(self) :
        self.this = controls2c.new_wxTreeItemId()
        self.thisown = 1




class wxTreeItemDataPtr :
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def GetData(self):
        val = controls2c.wxTreeItemData_GetData(self.this)
        return val
    def SetData(self,arg0):
        val = controls2c.wxTreeItemData_SetData(self.this,arg0)
        return val
    def GetId(self):
        val = controls2c.wxTreeItemData_GetId(self.this)
        val = wxTreeItemIdPtr(val)
        return val
    def SetId(self,arg0):
        val = controls2c.wxTreeItemData_SetId(self.this,arg0.this)
        return val
    def __repr__(self):
        return "<C wxTreeItemData instance>"
class wxTreeItemData(wxTreeItemDataPtr):
    def __init__(self,*args) :
        self.this = apply(controls2c.new_wxTreeItemData,()+args)
        self.thisown = 1




class wxTreeEventPtr(wxNotifyEventPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def GetItem(self):
        val = controls2c.wxTreeEvent_GetItem(self.this)
        val = wxTreeItemIdPtr(val)
        val.thisown = 1
        return val
    def GetOldItem(self):
        val = controls2c.wxTreeEvent_GetOldItem(self.this)
        val = wxTreeItemIdPtr(val)
        val.thisown = 1
        return val
    def GetPoint(self):
        val = controls2c.wxTreeEvent_GetPoint(self.this)
        val = wxPointPtr(val)
        val.thisown = 1
        return val
    def GetCode(self):
        val = controls2c.wxTreeEvent_GetCode(self.this)
        return val
    def GetLabel(self):
        val = controls2c.wxTreeEvent_GetLabel(self.this)
        return val
    def __repr__(self):
        return "<C wxTreeEvent instance>"
class wxTreeEvent(wxTreeEventPtr):
    def __init__(self,this):
        self.this = this




class wxTreeCtrlPtr(wxControlPtr):
    def __init__(self,this):
        self.this = this
        self.thisown = 0
    def GetCount(self):
        val = controls2c.wxTreeCtrl_GetCount(self.this)
        return val
    def GetIndent(self):
        val = controls2c.wxTreeCtrl_GetIndent(self.this)
        return val
    def SetIndent(self,arg0):
        val = controls2c.wxTreeCtrl_SetIndent(self.this,arg0)
        return val
    def GetImageList(self):
        val = controls2c.wxTreeCtrl_GetImageList(self.this)
        val = wxImageListPtr(val)
        return val
    def GetStateImageList(self):
        val = controls2c.wxTreeCtrl_GetStateImageList(self.this)
        val = wxImageListPtr(val)
        return val
    def SetImageList(self,arg0):
        val = controls2c.wxTreeCtrl_SetImageList(self.this,arg0.this)
        return val
    def SetStateImageList(self,arg0):
        val = controls2c.wxTreeCtrl_SetStateImageList(self.this,arg0.this)
        return val
    def GetItemText(self,arg0):
        val = controls2c.wxTreeCtrl_GetItemText(self.this,arg0.this)
        return val
    def GetItemImage(self,arg0):
        val = controls2c.wxTreeCtrl_GetItemImage(self.this,arg0.this)
        return val
    def GetItemSelectedImage(self,arg0):
        val = controls2c.wxTreeCtrl_GetItemSelectedImage(self.this,arg0.this)
        return val
    def SetItemText(self,arg0,arg1):
        val = controls2c.wxTreeCtrl_SetItemText(self.this,arg0.this,arg1)
        return val
    def SetItemImage(self,arg0,arg1):
        val = controls2c.wxTreeCtrl_SetItemImage(self.this,arg0.this,arg1)
        return val
    def SetItemSelectedImage(self,arg0,arg1):
        val = controls2c.wxTreeCtrl_SetItemSelectedImage(self.this,arg0.this,arg1)
        return val
    def SetItemHasChildren(self,arg0,*args):
        val = apply(controls2c.wxTreeCtrl_SetItemHasChildren,(self.this,arg0.this,)+args)
        return val
    def GetItemData(self,arg0):
        val = controls2c.wxTreeCtrl_GetItemData(self.this,arg0.this)
        val = wxTreeItemDataPtr(val)
        return val
    def SetItemData(self,arg0,arg1):
        val = controls2c.wxTreeCtrl_SetItemData(self.this,arg0.this,arg1.this)
        return val
    def GetPyData(self,arg0):
        val = controls2c.wxTreeCtrl_GetPyData(self.this,arg0.this)
        return val
    def SetPyData(self,arg0,arg1):
        val = controls2c.wxTreeCtrl_SetPyData(self.this,arg0.this,arg1)
        return val
    def IsVisible(self,arg0):
        val = controls2c.wxTreeCtrl_IsVisible(self.this,arg0.this)
        return val
    def ItemHasChildren(self,arg0):
        val = controls2c.wxTreeCtrl_ItemHasChildren(self.this,arg0.this)
        return val
    def IsExpanded(self,arg0):
        val = controls2c.wxTreeCtrl_IsExpanded(self.this,arg0.this)
        return val
    def IsSelected(self,arg0):
        val = controls2c.wxTreeCtrl_IsSelected(self.this,arg0.this)
        return val
    def GetRootItem(self):
        val = controls2c.wxTreeCtrl_GetRootItem(self.this)
        val = wxTreeItemIdPtr(val)
        val.thisown = 1
        return val
    def GetSelection(self):
        val = controls2c.wxTreeCtrl_GetSelection(self.this)
        val = wxTreeItemIdPtr(val)
        val.thisown = 1
        return val
    def GetParent(self,arg0):
        val = controls2c.wxTreeCtrl_GetParent(self.this,arg0.this)
        val = wxTreeItemIdPtr(val)
        val.thisown = 1
        return val
    def GetChildrenCount(self,arg0,*args):
        val = apply(controls2c.wxTreeCtrl_GetChildrenCount,(self.this,arg0.this,)+args)
        return val
    def GetFirstChild(self,arg0,arg1):
        val = controls2c.wxTreeCtrl_GetFirstChild(self.this,arg0.this,arg1)
        return val
    def GetNextChild(self,arg0,arg1):
        val = controls2c.wxTreeCtrl_GetNextChild(self.this,arg0.this,arg1)
        return val
    def GetNextSibling(self,arg0):
        val = controls2c.wxTreeCtrl_GetNextSibling(self.this,arg0.this)
        val = wxTreeItemIdPtr(val)
        val.thisown = 1
        return val
    def GetPrevSibling(self,arg0):
        val = controls2c.wxTreeCtrl_GetPrevSibling(self.this,arg0.this)
        val = wxTreeItemIdPtr(val)
        val.thisown = 1
        return val
    def GetFirstVisibleItem(self):
        val = controls2c.wxTreeCtrl_GetFirstVisibleItem(self.this)
        val = wxTreeItemIdPtr(val)
        val.thisown = 1
        return val
    def GetNextVisible(self,arg0):
        val = controls2c.wxTreeCtrl_GetNextVisible(self.this,arg0.this)
        val = wxTreeItemIdPtr(val)
        val.thisown = 1
        return val
    def GetPrevVisible(self,arg0):
        val = controls2c.wxTreeCtrl_GetPrevVisible(self.this,arg0.this)
        val = wxTreeItemIdPtr(val)
        val.thisown = 1
        return val
    def AddRoot(self,arg0,*args):
        argl = map(None,args)
        try: argl[2] = argl[2].this
        except: pass
        args = tuple(argl)
        val = apply(controls2c.wxTreeCtrl_AddRoot,(self.this,arg0,)+args)
        val = wxTreeItemIdPtr(val)
        val.thisown = 1
        return val
    def PrependItem(self,arg0,arg1,*args):
        argl = map(None,args)
        try: argl[2] = argl[2].this
        except: pass
        args = tuple(argl)
        val = apply(controls2c.wxTreeCtrl_PrependItem,(self.this,arg0.this,arg1,)+args)
        val = wxTreeItemIdPtr(val)
        val.thisown = 1
        return val
    def InsertItem(self,arg0,arg1,arg2,*args):
        argl = map(None,args)
        try: argl[2] = argl[2].this
        except: pass
        args = tuple(argl)
        val = apply(controls2c.wxTreeCtrl_InsertItem,(self.this,arg0.this,arg1.this,arg2,)+args)
        val = wxTreeItemIdPtr(val)
        val.thisown = 1
        return val
    def AppendItem(self,arg0,arg1,*args):
        argl = map(None,args)
        try: argl[2] = argl[2].this
        except: pass
        args = tuple(argl)
        val = apply(controls2c.wxTreeCtrl_AppendItem,(self.this,arg0.this,arg1,)+args)
        val = wxTreeItemIdPtr(val)
        val.thisown = 1
        return val
    def Delete(self,arg0):
        val = controls2c.wxTreeCtrl_Delete(self.this,arg0.this)
        return val
    def DeleteChildren(self,arg0):
        val = controls2c.wxTreeCtrl_DeleteChildren(self.this,arg0.this)
        return val
    def DeleteAllItems(self):
        val = controls2c.wxTreeCtrl_DeleteAllItems(self.this)
        return val
    def Expand(self,arg0):
        val = controls2c.wxTreeCtrl_Expand(self.this,arg0.this)
        return val
    def Collapse(self,arg0):
        val = controls2c.wxTreeCtrl_Collapse(self.this,arg0.this)
        return val
    def CollapseAndReset(self,arg0):
        val = controls2c.wxTreeCtrl_CollapseAndReset(self.this,arg0.this)
        return val
    def Toggle(self,arg0):
        val = controls2c.wxTreeCtrl_Toggle(self.this,arg0.this)
        return val
    def Unselect(self):
        val = controls2c.wxTreeCtrl_Unselect(self.this)
        return val
    def UnselectAll(self):
        val = controls2c.wxTreeCtrl_UnselectAll(self.this)
        return val
    def SelectItem(self,arg0):
        val = controls2c.wxTreeCtrl_SelectItem(self.this,arg0.this)
        return val
    def EnsureVisible(self,arg0):
        val = controls2c.wxTreeCtrl_EnsureVisible(self.this,arg0.this)
        return val
    def ScrollTo(self,arg0):
        val = controls2c.wxTreeCtrl_ScrollTo(self.this,arg0.this)
        return val
    def EditLabel(self,arg0):
        val = controls2c.wxTreeCtrl_EditLabel(self.this,arg0.this)
        return val
    def SetItemBold(self,arg0,*args):
        val = apply(controls2c.wxTreeCtrl_SetItemBold,(self.this,arg0.this,)+args)
        return val
    def IsBold(self,arg0):
        val = controls2c.wxTreeCtrl_IsBold(self.this,arg0.this)
        return val
    def HitTest(self,arg0):
        val = controls2c.wxTreeCtrl_HitTest(self.this,arg0.this)
        val = wxTreeItemIdPtr(val)
        val.thisown = 1
        return val
    def __repr__(self):
        return "<C wxTreeCtrl instance>"
    
    # Redefine a couple methods that SWIG gets a bit confused on...
    def GetFirstChild(self,arg0,arg1):
        val1, val2 = controls2c.wxTreeCtrl_GetFirstChild(self.this,arg0.this,arg1)
        val1 = wxTreeItemIdPtr(val1)
        val1.thisown = 1
        return (val1,val2)
    def GetNextChild(self,arg0,arg1):
        val1, val2 = controls2c.wxTreeCtrl_GetNextChild(self.this,arg0.this,arg1)
        val1 = wxTreeItemIdPtr(val1)
        val1.thisown = 1
        return (val1,val2)

class wxTreeCtrl(wxTreeCtrlPtr):
    def __init__(self,arg0,*args) :
        argl = map(None,args)
        try: argl[1] = argl[1].this
        except: pass
        try: argl[2] = argl[2].this
        except: pass
        args = tuple(argl)
        self.this = apply(controls2c.new_wxTreeCtrl,(arg0.this,)+args)
        self.thisown = 1
        wx._StdWindowCallbacks(self)






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
wxLIST_FORMAT_LEFT = controls2c.wxLIST_FORMAT_LEFT
wxLIST_FORMAT_RIGHT = controls2c.wxLIST_FORMAT_RIGHT
wxLIST_FORMAT_CENTRE = controls2c.wxLIST_FORMAT_CENTRE
wxLIST_FORMAT_CENTER = controls2c.wxLIST_FORMAT_CENTER
wxLIST_AUTOSIZE = controls2c.wxLIST_AUTOSIZE
wxLIST_AUTOSIZE_USEHEADER = controls2c.wxLIST_AUTOSIZE_USEHEADER
wxLIST_RECT_BOUNDS = controls2c.wxLIST_RECT_BOUNDS
wxLIST_RECT_ICON = controls2c.wxLIST_RECT_ICON
wxLIST_RECT_LABEL = controls2c.wxLIST_RECT_LABEL
wxLIST_FIND_UP = controls2c.wxLIST_FIND_UP
wxLIST_FIND_DOWN = controls2c.wxLIST_FIND_DOWN
wxLIST_FIND_LEFT = controls2c.wxLIST_FIND_LEFT
wxLIST_FIND_RIGHT = controls2c.wxLIST_FIND_RIGHT
