/////////////////////////////////////////////////////////////////////////////
// Name:        src/mac/carbon/databrow.cpp
// Purpose:     Classes and functions for the Carbon data browser
// Author:
// Modified by:
// Created:     2007-05-18
// RCS-ID:      $Id$
// Copyright:   (c)
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#if wxUSE_DATAVIEWCTRL && !defined(wxUSE_GENERICDATAVIEWCTRL)

#include <limits>

#ifndef WX_PRECOMP
    #include "wx/app.h"
    #include "wx/toplevel.h"
    #include "wx/font.h"
#endif

#include "wx/dataview.h"
#include "wx/utils.h"

#include "wx/mac/carbon/databrow.h"
#include "wx/mac/private.h"
#include "wx/mac/uma.h"

// ============================================================================
// wxMacDataBrowserTableViewControl
// ============================================================================

pascal Boolean wxMacDataBrowserTableViewControl::DataBrowserCompareProc(ControlRef browser, DataBrowserItemID itemOneID, DataBrowserItemID itemTwoID, DataBrowserPropertyID sortProperty)
{
  wxMacDataBrowserTableViewControl* ControlPtr(dynamic_cast<wxMacDataBrowserTableViewControl*>(wxMacControl::GetReferenceFromNativeControl(browser)));


  if (ControlPtr != NULL)
    return ControlPtr->DataBrowserCompareProc(itemOneID,itemTwoID,sortProperty);
  else
    return errDataBrowserPropertyNotSupported;
} /* wxMacDataBrowserTableViewControl::DataBrowserCompareProc(ControlRef, DataBrowserItemID, DataBrowserItemID, DataBrowserPropertyID) */

pascal void wxMacDataBrowserTableViewControl::DataBrowserDrawItemProc(ControlRef browser, DataBrowserItemID itemID, DataBrowserPropertyID property, DataBrowserItemState state, Rect const* rectangle, SInt16 bitDepth, Boolean colorDevice)
{
  wxMacDataBrowserTableViewControl* ControlPtr(dynamic_cast<wxMacDataBrowserTableViewControl*>(wxMacControl::GetReferenceFromNativeControl(browser)));


  if (ControlPtr != NULL)
    ControlPtr->DataBrowserDrawItemProc(itemID,property,state,rectangle,bitDepth,colorDevice);
} /* wxMacDataBrowserTableViewControl::DataBrowserDrawItemProc(ControlRef, DataBrowserItemID, DataBrowserPropertyID, DataBrowserItemState, Rect const*, SInt16, Boolean) */

pascal OSStatus wxMacDataBrowserTableViewControl::DataBrowserGetSetItemDataProc(ControlRef browser, DataBrowserItemID itemID, DataBrowserPropertyID property, DataBrowserItemDataRef itemData, Boolean getValue)
{
  wxMacDataBrowserTableViewControl* ControlPtr(dynamic_cast<wxMacDataBrowserTableViewControl*>(wxMacControl::GetReferenceFromNativeControl(browser)));


  if (ControlPtr != NULL)
    return ControlPtr->DataBrowserGetSetItemDataProc(itemID,property,itemData,getValue);
  else
    return errDataBrowserPropertyNotSupported;
} /* wxMacDataBrowserTableViewControl::DataBrowserGetSetItemDataProc(ControlRef, DataBrowserItemID, DataBrowserPropertyID, DataBrowserItemDataRef, Boolean) */

pascal void wxMacDataBrowserTableViewControl::DataBrowserItemNotificationProc(ControlRef browser, DataBrowserItemID itemID, DataBrowserItemNotification message, DataBrowserItemDataRef itemData)
{
  wxMacDataBrowserTableViewControl* ControlPtr(dynamic_cast<wxMacDataBrowserTableViewControl*>(wxMacControl::GetReferenceFromNativeControl(browser)));


  if (ControlPtr != NULL)
    ControlPtr->DataBrowserItemNotificationProc(itemID,message,itemData);
} /* wxMacDataBrowserTableViewControl::DataBrowserItemNotificationProc(ControlRef, DataBrowserItemID, DataBrowserItemNotification, DataBrowserItemDataRef) */

DataBrowserDrawItemUPP         gDataBrowserTableViewDrawItemUPP         = NULL;
DataBrowserItemCompareUPP      gDataBrowserTableViewItemCompareUPP      = NULL;
DataBrowserItemDataUPP         gDataBrowserTableViewItemDataUPP         = NULL;
DataBrowserItemNotificationUPP gDataBrowserTableViewItemNotificationUPP = NULL;

wxMacDataBrowserTableViewControl::wxMacDataBrowserTableViewControl(wxWindow* peer, wxPoint const& pos, wxSize const& size, long style)
                                 :wxMacControl(peer)
{
  Rect bounds = wxMacGetBoundsForControl(peer,pos,size);
  OSStatus err = ::CreateDataBrowserControl(MAC_WXHWND(peer->MacGetTopLevelWindowRef()),&bounds,kDataBrowserListView,&(this->m_controlRef));


  SetReferenceInNativeControl();
  verify_noerr(err);
  if (gDataBrowserTableViewDrawItemUPP         == NULL) gDataBrowserTableViewDrawItemUPP    = NewDataBrowserDrawItemUPP   (DataBrowserDrawItemProc);
  if (gDataBrowserTableViewItemCompareUPP      == NULL) gDataBrowserTableViewItemCompareUPP = NewDataBrowserItemCompareUPP(wxMacDataBrowserTableViewControl::DataBrowserCompareProc);
  if (gDataBrowserTableViewItemDataUPP         == NULL) gDataBrowserTableViewItemDataUPP    = NewDataBrowserItemDataUPP   (wxMacDataBrowserTableViewControl::DataBrowserGetSetItemDataProc);
  if (gDataBrowserTableViewItemNotificationUPP == NULL)
  {
    gDataBrowserTableViewItemNotificationUPP =
#if TARGET_API_MAC_OSX
      (DataBrowserItemNotificationUPP) NewDataBrowserItemNotificationWithItemUPP(wxMacDataBrowserTableViewControl::DataBrowserItemNotificationProc);
#else
      NewDataBrowserItemNotificationUPP(wxMacDataBrowserTableViewControl::DataBrowserItemNotificationProc);
#endif
  }

  DataBrowserCallbacks       callbacks;
  DataBrowserCustomCallbacks customCallbacks;

  InitializeDataBrowserCallbacks      (&callbacks,      kDataBrowserLatestCallbacks);
  InitializeDataBrowserCustomCallbacks(&customCallbacks,kDataBrowserLatestCallbacks);

  callbacks.u.v1.itemDataCallback         = gDataBrowserTableViewItemDataUPP;
  callbacks.u.v1.itemCompareCallback      = gDataBrowserTableViewItemCompareUPP;
  callbacks.u.v1.itemNotificationCallback = gDataBrowserTableViewItemNotificationUPP;
  this->SetCallbacks(&callbacks);

  customCallbacks.u.v1.drawItemCallback = gDataBrowserTableViewDrawItemUPP;
  this->SetCustomCallbacks(&customCallbacks);

 // style setting:
  this->EnableCellSizeModification();

  DataBrowserSelectionFlags flags; // variable definition

  if (this->GetSelectionFlags(&flags) == noErr) // get default settings
  {
    if ((style & wxDV_MULTIPLE) != 0)
      flags &= ~kDataBrowserSelectOnlyOne;
    else
      flags |= kDataBrowserSelectOnlyOne;
    (void) this->SetSelectionFlags(flags);
  } /* if */

  OptionBits attributes; // variable definition

  if (this->GetAttributes(&attributes) == noErr) // get default settings
  {
    if ((style & wxDV_VERT_RULES) != 0)
      attributes |= kDataBrowserAttributeListViewDrawColumnDividers;
    else
      attributes &= ~kDataBrowserAttributeListViewDrawColumnDividers;
    (void) this->SetAttributes(attributes);
  } /* if */

  if ((style & wxDV_NO_HEADER) != 0)
    this->SetHeaderButtonHeight(0);
} /* wxMacDataBrowserTableViewControl::wxMacDataBrowserTableViewControl(wxWindow*, wxPoint const&, wxSize const&, long) */

//
// callback handling
//
OSStatus wxMacDataBrowserTableViewControl::SetCallbacks(DataBrowserCallbacks const* callbacks)
{
  return ::SetDataBrowserCallbacks(this->m_controlRef,callbacks);
} /* wxMacDataBrowserTableViewControl::SetCallbacks(DataBrowserCallbacks const*) */

OSStatus wxMacDataBrowserTableViewControl::SetCustomCallbacks(DataBrowserCustomCallbacks const* customCallbacks)
{
  return ::SetDataBrowserCustomCallbacks(this->m_controlRef,customCallbacks);
} /* xMacDataBrowserTableViewControl::SetCustomCallbacks(DataBrowserCustomCallbacks const*) */

//
// header handling
//
OSStatus wxMacDataBrowserTableViewControl::GetHeaderDesc(DataBrowserPropertyID property, DataBrowserListViewHeaderDesc* desc) const
{
  desc->version = kDataBrowserListViewLatestHeaderDesc; // if this statement is missing the next call will fail (NOT DOCUMENTED!!)
  return ::GetDataBrowserListViewHeaderDesc(this->m_controlRef,property,desc);
}

OSStatus wxMacDataBrowserTableViewControl::SetHeaderDesc(DataBrowserPropertyID property, DataBrowserListViewHeaderDesc* desc)
{
  return ::SetDataBrowserListViewHeaderDesc(this->m_controlRef,property,desc);
}

//
// layout handling
//
OSStatus wxMacDataBrowserTableViewControl::AutoSizeColumns()
{
  return AutoSizeDataBrowserListViewColumns(this->m_controlRef);
}

OSStatus wxMacDataBrowserTableViewControl::EnableCellSizeModification(bool enableHeight, bool enableWidth)
{
  return ::SetDataBrowserTableViewGeometry(this->GetControlRef(),enableWidth,enableHeight);
} /* wxMacDataBrowserTableViewControl::EnableCellSizeModification(bool, bool) */

OSStatus wxMacDataBrowserTableViewControl::GetAttributes(OptionBits* attributes)
{
  return ::DataBrowserGetAttributes(this->GetControlRef(),attributes);
} /* wxMacDataBrowserTableViewControl::GetAttributes(OptionBits*) */

OSStatus wxMacDataBrowserTableViewControl::GetColumnWidth(DataBrowserPropertyID propertyID, UInt16* width) const
{
  return ::GetDataBrowserTableViewNamedColumnWidth(this->m_controlRef,propertyID,width);
}

OSStatus wxMacDataBrowserTableViewControl::GetDefaultColumnWidth( UInt16 *width ) const
{
    return GetDataBrowserTableViewColumnWidth(this->m_controlRef, width );
}

OSStatus wxMacDataBrowserTableViewControl::GetDefaultRowHeight(UInt16* height) const
{
  return ::GetDataBrowserTableViewRowHeight(this->m_controlRef,height);
}

OSStatus wxMacDataBrowserTableViewControl::GetHeaderButtonHeight(UInt16 *height)
{
  return ::GetDataBrowserListViewHeaderBtnHeight(this->m_controlRef,height);
}

OSStatus wxMacDataBrowserTableViewControl::GetPartBounds(DataBrowserItemID item, DataBrowserPropertyID property, DataBrowserPropertyPart part, Rect* bounds)
{
  return ::GetDataBrowserItemPartBounds(this->m_controlRef,item,property,part,bounds);
} /* wxMacDataBrowserTableViewControl::GetPartBounds(DataBrowserItemID, DataBrowswerPropertyID, DataBrowserPropertyPart, Rect*) */

OSStatus wxMacDataBrowserTableViewControl::GetRowHeight(DataBrowserItemID item, UInt16* height) const
{
  return ::GetDataBrowserTableViewItemRowHeight(this->m_controlRef,item,height);
} /* wxMacDataBrowserTableViewControl::GetRowHeight(DataBrowserItemID, UInt16*) const */

OSStatus wxMacDataBrowserTableViewControl::GetScrollPosition( UInt32 *top , UInt32 *left ) const
{
    return GetDataBrowserScrollPosition(this->m_controlRef, top , left );
}

OSStatus wxMacDataBrowserTableViewControl::SetAttributes(OptionBits attributes)
{
  return ::DataBrowserChangeAttributes(this->GetControlRef(),attributes,~attributes);
} /* wxMacDataBrowserTableViewControl::SetAttributes(OptionBits) */

OSStatus wxMacDataBrowserTableViewControl::SetColumnWidth(DataBrowserPropertyID propertyID, UInt16 width)
{
  return ::SetDataBrowserTableViewNamedColumnWidth(this->m_controlRef,propertyID,width);
}

OSStatus wxMacDataBrowserTableViewControl::SetDefaultColumnWidth(UInt16 width)
{
  return ::SetDataBrowserTableViewColumnWidth(this->m_controlRef,width);
}

OSStatus wxMacDataBrowserTableViewControl::SetDefaultRowHeight(UInt16 height)
{
  return ::SetDataBrowserTableViewRowHeight(this->m_controlRef,height);
}

OSStatus wxMacDataBrowserTableViewControl::SetHasScrollBars(bool horiz, bool vert)
{
  return ::SetDataBrowserHasScrollBars(this->m_controlRef,horiz,vert);
} /* wxMacDataBrowserTableViewControl::SetHasScrollBars(bool, bool) */

OSStatus wxMacDataBrowserTableViewControl::SetHeaderButtonHeight(UInt16 height)
{
  return ::SetDataBrowserListViewHeaderBtnHeight(this->m_controlRef,height);
} /* wxMacDataBrowserTableViewControl::SetHeaderButtonHeight(UInt16) */

OSStatus wxMacDataBrowserTableViewControl::SetHiliteStyle(DataBrowserTableViewHiliteStyle hiliteStyle)
{
  return ::SetDataBrowserTableViewHiliteStyle(this->m_controlRef,hiliteStyle);
} /*wxMacDataBrowserTableViewControl::SetHiliteStyle(DataBrowserTableViewHiliteStyle)  */

OSStatus wxMacDataBrowserTableViewControl::SetIndent(float Indent)
{
  return ::DataBrowserSetMetric(this->m_controlRef,kDataBrowserMetricDisclosureColumnPerDepthGap,true,Indent);
} /* wxMacDataBrowserTableViewControl::SetIndent(float* Indent) */

OSStatus wxMacDataBrowserTableViewControl::SetRowHeight(DataBrowserItemID item, UInt16 height)
{
  return ::SetDataBrowserTableViewItemRowHeight(this->m_controlRef,item,height);
}

OSStatus wxMacDataBrowserTableViewControl::SetScrollPosition(UInt32 top, UInt32 left)
{
  return ::SetDataBrowserScrollPosition(this->m_controlRef,top,left);
}

//
// column handling
//
OSStatus wxMacDataBrowserTableViewControl::GetColumnCount(UInt32* numColumns) const
{
  return ::GetDataBrowserTableViewColumnCount(this->m_controlRef,numColumns);
}

OSStatus wxMacDataBrowserTableViewControl::GetColumnIndex(DataBrowserPropertyID propertyID, DataBrowserTableViewColumnIndex* index) const
{
  return ::GetDataBrowserTableViewColumnPosition(this->m_controlRef,propertyID,index);
} /* wxMacDataBrowserTableViewControl::GetColumnIndex(DataBrowserPropertyID, DataBrowserTableViewColumnIndex*) const */

OSStatus wxMacDataBrowserTableViewControl::GetFreePropertyID(DataBrowserPropertyID* propertyID) const
{
  for (*propertyID=kMinPropertyID; *propertyID<std::numeric_limits<DataBrowserPropertyID>::max(); ++(*propertyID))
    if (this->IsUsedPropertyID(*propertyID) == errDataBrowserPropertyNotFound)
      return noErr;
  return errDataBrowserPropertyNotSupported;
} /* wxMacDataBrowserTableViewControl::GetFreePropertyID(DataBrowserPropertyID*) const */

OSStatus wxMacDataBrowserTableViewControl::GetPropertyFlags(DataBrowserPropertyID propertyID, DataBrowserPropertyFlags *flags) const
{
  return ::GetDataBrowserPropertyFlags(this->m_controlRef,propertyID,flags);
}

OSStatus wxMacDataBrowserTableViewControl::GetPropertyID(DataBrowserItemDataRef itemData, DataBrowserPropertyID* propertyID)
{
  return ::GetDataBrowserItemDataProperty(itemData,propertyID);
} /* wxMacDataBrowserTableViewControl::GetPropertyID(DataBrowserItemDataRef, DataBrowserPropertyID*) */

OSStatus wxMacDataBrowserTableViewControl::GetPropertyID(DataBrowserTableViewColumnIndex index, DataBrowserTableViewColumnID* propertyID)
{
  return ::GetDataBrowserTableViewColumnProperty(this->m_controlRef,index,propertyID);
} /* wxMacDataBrowserTableViewControl::GetPropertyID(DataBrowserTableViewColumnIndex, DataBrowserTableViewColumnID*) */

OSStatus wxMacDataBrowserTableViewControl::IsUsedPropertyID(DataBrowserPropertyID propertyID) const
{
 // as the Mac interface does not provide a function that checks if the property id is in use or not a function is chosen that should not
 // lead to a large overhead for the call but returns an error code if the property id does not exist, here we use the function that returns
 // the column position for the property id:
  DataBrowserTableViewColumnIndex index;

  return ::GetDataBrowserTableViewColumnPosition(this->m_controlRef,propertyID,&index);
} /* wxMacDataBrowserTableViewControl::IsUsedPropertyId(DataBrowserPropertyID) const */

OSStatus wxMacDataBrowserTableViewControl::RemoveColumnByProperty(DataBrowserTableViewColumnID propertyID)
{
  return ::RemoveDataBrowserTableViewColumn(this->m_controlRef,propertyID);
} /* wxMacDataBrowserTableViewControl::RemoveColumnByProperty(DataBrowserTableViewColumnID) */

OSStatus wxMacDataBrowserTableViewControl::RemoveColumnByIndex(DataBrowserTableViewColumnIndex index)
{
  DataBrowserTableViewColumnID propertyID;


  this->GetPropertyID(index,&propertyID);
  return ::RemoveDataBrowserTableViewColumn(this->m_controlRef,propertyID);
} /* wxMacDataBrowserTableViewControl::RemoveColumnByIndex(DataBrowserTableViewColumnIndex) */

OSStatus wxMacDataBrowserTableViewControl::SetColumnIndex(DataBrowserPropertyID propertyID, DataBrowserTableViewColumnIndex index)
{
  return ::SetDataBrowserTableViewColumnPosition(this->m_controlRef,propertyID,index);
} /* wxMacDataBrowserTableViewControl::SetColumnIndex(DataBrowserPropertyID, DataBrowserTableViewColumnIndex) */

OSStatus wxMacDataBrowserTableViewControl::SetDisclosureColumn(DataBrowserPropertyID propertyID, Boolean expandableRows)
{
  return ::SetDataBrowserListViewDisclosureColumn(this->m_controlRef,propertyID,expandableRows);
}

OSStatus wxMacDataBrowserTableViewControl::SetPropertyFlags(DataBrowserPropertyID propertyID, DataBrowserPropertyFlags flags)
{
  return ::SetDataBrowserPropertyFlags(this->m_controlRef,propertyID,flags);
} /* wxMacDataBrowserTableViewControl::SetPropertyFlags(DataBrowserPropertyID, DataBrowserPropertyFlags) */

//
// item handling
//
OSStatus wxMacDataBrowserTableViewControl::AddItems(DataBrowserItemID container, UInt32 numItems, DataBrowserItemID const* items, DataBrowserPropertyID preSortProperty)
{
  return ::AddDataBrowserItems(this->m_controlRef,container,numItems,items,preSortProperty);
} /* wxMacDataBrowserTableViewControl::AddItems(DataBrowserItemID, UInt32, DataBrowserItemID const*, DataBrowserPropertyID) */

OSStatus wxMacDataBrowserTableViewControl::GetFreeItemID(DataBrowserItemID* id) const
{
  ItemCount NoOfItems;


  verify_noerr(this->GetItemCount(&NoOfItems));
  if (NoOfItems == 0)
  {
    *id = 1;
    return noErr;
  } /* if */
  else
  {
   // as there might be a lot of items in the data browser and mostly the data is added item by item the largest used ID number is roughly in the order of magnitude
   // as the number of items; therefore, start from the number of items to look for a new ID:
    for (*id=NoOfItems; *id<std::numeric_limits<DataBrowserItemID>::max(); ++(*id))
      if (this->IsUsedItemID(*id) == errDataBrowserItemNotFound)
        return noErr;
   // as the first approach was not successful, try from the beginning:
    for (*id=0; *id<NoOfItems; ++(*id))
      if (this->IsUsedItemID(*id) == errDataBrowserItemNotFound)
        return noErr;
   // sorry, data browser is full:
    return errDataBrowserItemNotAdded;
  } /* if */
} /* wxMacDataBrowserTableViewControl::GetFreeItemID(DataBrowserItemID*) const */

OSStatus wxMacDataBrowserTableViewControl::GetItemCount(DataBrowserItemID container, Boolean recurse, DataBrowserItemState state, ItemCount* numItems) const
{
  return GetDataBrowserItemCount(this->m_controlRef,container,recurse,state,numItems);
} /* wxMacDataBrowserTableViewControl::GetItemCount(DataBrowserItemID, Boolean, DataBrowserItemState, ItemCount*) const */

OSStatus wxMacDataBrowserTableViewControl::GetItemID( DataBrowserTableViewRowIndex row, DataBrowserItemID * item ) const
{
  return GetDataBrowserTableViewItemID(this->m_controlRef,row,item);
}

OSStatus wxMacDataBrowserTableViewControl::GetItems(DataBrowserItemID container, Boolean recurse, DataBrowserItemState state, Handle items) const
{
  return GetDataBrowserItems(this->m_controlRef,container,recurse,state,items);
} /* wxMacDataBrowserTableViewControl::GetItems(DataBrowserItemID, Boolean, DataBrowserItemState, Handle) const */

OSStatus wxMacDataBrowserTableViewControl::GetItemRow(DataBrowserItemID item, DataBrowserTableViewRowIndex* row) const
{
  return GetDataBrowserTableViewItemRow(this->m_controlRef,item,row);
}

OSStatus wxMacDataBrowserTableViewControl::IsUsedItemID(DataBrowserItemID itemID) const
{
 // as the Mac interface does not provide a function that checks if the property id is in use or not a function is chosen that should not
 // lead to a large overhead for the call but returns an error code if the property id does not exist, here we use the function that returns
 // the column position for the property id:
  DataBrowserTableViewColumnIndex index;

  return ::GetDataBrowserTableViewItemRow(this->m_controlRef,itemID,&index);
} /* wxMacDataBrowserTableViewControl::IsUsedItemID(DataBrowserItemID) const */

OSStatus wxMacDataBrowserTableViewControl::RemoveItems(DataBrowserItemID container, UInt32 numItems, DataBrowserItemID const* items, DataBrowserPropertyID preSortProperty)
{
  return ::RemoveDataBrowserItems(this->m_controlRef,container,numItems,items,preSortProperty);
}

OSStatus wxMacDataBrowserTableViewControl::RevealItem(DataBrowserItemID item, DataBrowserPropertyID propertyID, DataBrowserRevealOptions options) const
{
  return ::RevealDataBrowserItem(this->m_controlRef,item,propertyID,options);
} /* wxMacDataBrowserTableViewControl::RevealItem(DataBrowserItemID item, DataBrowserPropertyID propertyID, DataBrowserRevealOptions options) const */

OSStatus wxMacDataBrowserTableViewControl::UpdateItems(DataBrowserItemID container, UInt32 numItems, DataBrowserItemID const* items, DataBrowserPropertyID preSortProperty,
                                                       DataBrowserPropertyID propertyID) const
{
  return UpdateDataBrowserItems(this->m_controlRef,container,numItems,items,preSortProperty,propertyID);
}

//
// item selection
//
size_t wxMacDataBrowserTableViewControl::GetSelectedItemIDs(wxArrayDataBrowserItemID& itemIDs) const
{
  DataBrowserItemID* itemIDPtr;
  Handle handle(::NewHandle(0));

  size_t NoOfItems;


  wxCHECK_MSG(this->GetItems(kDataBrowserNoItem,true,kDataBrowserItemIsSelected,handle) == noErr,0,_("Could not get selected items."));
  NoOfItems = static_cast<size_t>(::GetHandleSize(handle)/sizeof(DataBrowserItemID));
  itemIDs.Empty();
  itemIDs.Alloc(NoOfItems);
  HLock(handle);
  itemIDPtr = (DataBrowserItemID*) (*handle);
  for (size_t i=0; i<NoOfItems; ++i)
  {
    itemIDs.Add(*itemIDPtr);
    ++itemIDPtr;
  } /* for */
  HUnlock(handle);
  DisposeHandle(handle);
  return NoOfItems;
} /* wxMacDataBrowserTableViewControl::GetSelectedItemIDs(wxArrayDataBrowserItemID&) const*/

OSStatus wxMacDataBrowserTableViewControl::GetSelectionAnchor(DataBrowserItemID* first, DataBrowserItemID* last) const
{
  return ::GetDataBrowserSelectionAnchor(this->m_controlRef,first,last);
} /* wxMacDataBrowserTableViewControl::GetSelectionAnchor(DataBrowserItemID*, DataBrowserItemID*) const */

OSStatus wxMacDataBrowserTableViewControl::GetSelectionFlags(DataBrowserSelectionFlags* flags) const
{
  return ::GetDataBrowserSelectionFlags(this->m_controlRef,flags);
} /* wxMacDataBrowserTableViewControl::GetSelectionFlags(DataBrowserSelectionFlags*) const */

bool wxMacDataBrowserTableViewControl::IsItemSelected(DataBrowserItemID item) const
{
  return ::IsDataBrowserItemSelected(this->m_controlRef,item);
} /* wxMacDataBrowserTableViewControl::IsItemSelected(DataBrowserItemID) const */

OSStatus wxMacDataBrowserTableViewControl::SetSelectionFlags(DataBrowserSelectionFlags flags)
{
  return ::SetDataBrowserSelectionFlags(this->m_controlRef,flags);
} /* wxMacDataBrowserTableViewControl::SetSelectionFlags(DataBrowserSelectionFlags) */

OSStatus wxMacDataBrowserTableViewControl::SetSelectedItems(UInt32 numItems, DataBrowserItemID const* items, DataBrowserSetOption operation)
{
  return ::SetDataBrowserSelectedItems(this->m_controlRef, numItems, items, operation );
} /* wxMacDataBrowserTableViewControl::SetSelectedItems(UInt32, DataBrowserItemID const*, DataBrowserSetOption) */

OSStatus wxMacDataBrowserTableViewControl::GetSortOrder(DataBrowserSortOrder* order) const
{
  return ::GetDataBrowserSortOrder(this->m_controlRef,order);
}

OSStatus wxMacDataBrowserTableViewControl::GetSortProperty(DataBrowserPropertyID* propertyID) const
{
  return ::GetDataBrowserSortProperty(this->m_controlRef,propertyID);
}

OSStatus wxMacDataBrowserTableViewControl::Resort(DataBrowserItemID container, Boolean sortChildren)
{
  return ::SortDataBrowserContainer(this->m_controlRef,container,sortChildren);
} /* wxMacDataBrowserTableViewControl::Resort(DataBrowserItemID, Boolean) */

OSStatus wxMacDataBrowserTableViewControl::SetSortOrder(DataBrowserSortOrder order)
{
  return ::SetDataBrowserSortOrder(this->m_controlRef,order);
}

OSStatus wxMacDataBrowserTableViewControl::SetSortProperty(DataBrowserPropertyID propertyID)
{
  return ::SetDataBrowserSortProperty(this->m_controlRef,propertyID);
}

//
// container handling
//
OSStatus wxMacDataBrowserTableViewControl::CloseContainer(DataBrowserItemID containerID)
{
  return ::CloseDataBrowserContainer(this->m_controlRef,containerID);
} /* wxMacDataBrowserTableViewControl::CloseContainer(DataBrowserItemID) */

OSStatus wxMacDataBrowserTableViewControl::OpenContainer(DataBrowserItemID containerID)
{
  return ::OpenDataBrowserContainer(this->m_controlRef,containerID);
} /* wxMacDataBrowserTableViewControl::OpenContainer(DataBrowserItemID) */

IMPLEMENT_ABSTRACT_CLASS(wxMacDataBrowserTableViewControl,wxMacControl)

// ============================================================================
// wxMacDataBrowserListViewControl
// ============================================================================
#pragma mark -
//
// column handling
//
OSStatus wxMacDataBrowserListViewControl::AddColumn(DataBrowserListViewColumnDesc *columnDesc, DataBrowserTableViewColumnIndex position)
{
  return AddDataBrowserListViewColumn(this->m_controlRef,columnDesc,position);
} /* wxMacDataBrowserListViewControl::AddColumn(DataBrowserListViewColumnDesc*, DataBrowserTableViewColumnIndex) */

// ============================================================================
// wxMacDataViewDataBrowserListViewControl
// ============================================================================
#pragma mark -
//
// constructors / destructor
//
wxMacDataViewDataBrowserListViewControl::wxMacDataViewDataBrowserListViewControl(wxWindow* peer, wxPoint const& pos, wxSize const& size, long style)
                                        :wxMacDataBrowserListViewControl(peer,pos,size,style)
{
} /* wxMacDataViewDataBrowserListViewControl::wxMacDataViewDataBrowserListViewControl(wxWindow* , const wxPoint&, const wxSize&, long) */

//
// callback functions (inherited from wxMacDataBrowserTableViewControl)
//
Boolean wxMacDataViewDataBrowserListViewControl::DataBrowserCompareProc(DataBrowserItemID itemOneID, DataBrowserItemID itemTwoID, DataBrowserPropertyID sortProperty)
{
  DataBrowserSortOrder sortOrder;

  DataBrowserTableViewColumnIndex columnIndex;

  wxDataViewCtrl* dataViewCtrlPtr(dynamic_cast<wxDataViewCtrl*>(this->GetPeer()));


  wxCHECK_MSG(dataViewCtrlPtr != NULL,            false,_("Pointer to data view control not set correctly."));
  wxCHECK_MSG(dataViewCtrlPtr->GetModel() != NULL,false,_("Pointer to model not set correctly."));
  if (sortProperty >= kMinPropertyID)
  {
    wxCHECK_MSG(this->GetColumnIndex(sortProperty,&columnIndex) == noErr,false,_("Could not determine column index."));
  } /* if */
  else
    columnIndex = 0;
  this->GetSortOrder(&sortOrder);
  return static_cast<Boolean>(dataViewCtrlPtr->GetModel()->Compare(wxDataViewItem(reinterpret_cast<void*>(itemOneID)),wxDataViewItem(reinterpret_cast<void*>(itemTwoID)),
                              columnIndex,sortOrder != kDataBrowserOrderDecreasing) < 0);
} /* wxMacDataViewDataBrowserListViewControl::DataBrowserCompareProc(DataBrowserItemID, DataBrowserItemID, DataBrowserPropertyID) */

void wxMacDataViewDataBrowserListViewControl::DataBrowserDrawItemProc(DataBrowserItemID itemID, DataBrowserPropertyID propertyID, DataBrowserItemState state, Rect const* rectangle, SInt16 bitDepth, Boolean colorDevice)
{
  DataBrowserTableViewColumnIndex columnIndex;

  wxDataViewColumn* dataViewColumnPtr;

  wxDataViewCtrl* dataViewCtrlPtr;

  wxDataViewCustomRenderer* dataViewCustomRendererPtr;

  wxVariant dataToRender;


  dataViewCtrlPtr = dynamic_cast<wxDataViewCtrl*>(this->GetPeer());
  wxCHECK_RET(dataViewCtrlPtr != NULL,                               _("Pointer to data view control not set correctly."));
  wxCHECK_RET(dataViewCtrlPtr->GetModel() != NULL,                   _("Pointer to model not set correctly."));
  wxCHECK_RET(this->GetColumnIndex(propertyID,&columnIndex) == noErr,_("Could not determine column index."));
  dataViewColumnPtr = dataViewCtrlPtr->GetColumn(columnIndex);
  wxCHECK_RET(dataViewColumnPtr != NULL,_("No column for the specified column index existing."));
  dataViewCustomRendererPtr = dynamic_cast<wxDataViewCustomRenderer*>(dataViewColumnPtr->GetRenderer());
  wxCHECK_RET(dataViewCustomRendererPtr != NULL,_("No renderer or invalid renderer type specified for custom data column."));
  dataViewCtrlPtr->GetModel()->GetValue(dataToRender,wxDataViewItem(reinterpret_cast<void*>(itemID)),columnIndex);
  dataViewCustomRendererPtr->SetValue(dataToRender);

 // try to determine the content's size (drawable part):
  Rect      content;
  RgnHandle rgn(NewRgn());
  UInt16    headerHeight;

  if (this->GetRegion(kControlContentMetaPart,rgn) == noErr)
    GetRegionBounds(rgn,&content);
  else
    this->GetRect(&content);
  ::DisposeRgn(rgn);
 // space for the header
  this->GetHeaderButtonHeight(&headerHeight);
  content.top += headerHeight;
 // extra space for the frame (todo: do not how to determine the space automatically from the control)
  content.top    += 5;
  content.left   += 5;
  content.right  -= 3;
  content.bottom -= 3;
 // extra space for the scrollbars:
  content.bottom -= wxSystemSettings::GetMetric(wxSYS_HSCROLL_Y);
  content.right  -= wxSystemSettings::GetMetric(wxSYS_VSCROLL_X);

 // make sure that 'Render' can draw only in the allowed area:
  dataViewCustomRendererPtr->GetDC()->SetClippingRegion(content.left,content.top,content.right-content.left+1,content.bottom-content.top+1);
  (void) (dataViewCustomRendererPtr->Render(wxRect(static_cast<int>(rectangle->left),static_cast<int>(rectangle->top),
                                                   static_cast<int>(1+rectangle->right-rectangle->left),static_cast<int>(1+rectangle->bottom-rectangle->top)),
                                            dataViewCustomRendererPtr->GetDC(),((state == kDataBrowserItemIsSelected) ? wxDATAVIEW_CELL_SELECTED : 0)));
  dataViewCustomRendererPtr->GetDC()->DestroyClippingRegion(); // probably not necessary
} /* wxMacDataViewDataBrowserListViewControl::DataBrowserDrawItemProc(DataBrowserItemID, DataBrowserPropertyID, DataBrowserItemState, Rect const*, SInt16, Boolean) */

OSStatus wxMacDataViewDataBrowserListViewControl::DataBrowserGetSetItemDataProc(DataBrowserItemID itemID, DataBrowserPropertyID propertyID, DataBrowserItemDataRef itemData, Boolean getValue)
{
  if (getValue)
  {
   // variable definitions:
    wxDataViewCtrl* dataViewCtrlPtr;

    dataViewCtrlPtr = dynamic_cast<wxDataViewCtrl*>(this->GetPeer());
    wxCHECK_MSG(dataViewCtrlPtr != NULL,errDataBrowserNotConfigured,_("Pointer to data view control not set correctly."));
    if (dataViewCtrlPtr->IsDeleting())
      return noErr; // if a delete process is running the data of editable fields cannot be saved because the associated model variable may already have been deleted
    else
    {
     // variable definitions:
      DataBrowserTableViewColumnIndex columnIndex;
      OSStatus                        errorStatus;
      wxDataViewColumn*               dataViewColumnPtr;

      wxCHECK_MSG(dataViewCtrlPtr->GetModel() != NULL,errDataBrowserNotConfigured,_("Pointer to model not set correctly."));
      errorStatus = this->GetColumnIndex(propertyID,&columnIndex);
      wxCHECK_MSG(errorStatus == noErr,errorStatus,_("Could not determine column index"));
      dataViewColumnPtr = dataViewCtrlPtr->GetColumn(columnIndex);
      wxCHECK_MSG((dataViewColumnPtr != NULL) && (dataViewColumnPtr->GetRenderer() != NULL),errDataBrowserNotConfigured,_("There is no column or renderer for the specified column index."));
      switch (dataViewColumnPtr->GetRenderer()->GetPropertyType())
      {
        case kDataBrowserCheckboxType:
          {
           // variable definition:
            ThemeButtonValue buttonValue;

            errorStatus = ::GetDataBrowserItemDataButtonValue(itemData,&buttonValue);
            if (errorStatus == noErr)
            {
              if (buttonValue == kThemeButtonOn)
              {
               // variable definition and initialization:
                wxVariant modifiedData(true);

                return (dataViewCtrlPtr->GetModel()->SetValue(modifiedData,wxDataViewItem(reinterpret_cast<void*>(itemID)),static_cast<unsigned int>(columnIndex)) ? OSStatus(noErr) : OSStatus(errDataBrowserNotConfigured));
              } /* if */
              else if (buttonValue == kThemeButtonOff)
              {
               // variable definition and initialization:
                wxVariant modifiedData(false);

                return (dataViewCtrlPtr->GetModel()->SetValue(modifiedData,wxDataViewItem(reinterpret_cast<void*>(itemID)),static_cast<unsigned int>(columnIndex)) ? OSStatus(noErr) : OSStatus(errDataBrowserNotConfigured));
              } /* if */
              else
                return errDataBrowserInvalidPropertyData;
            } /* if */
            else
              return errorStatus;
          } /* block */
        case kDataBrowserTextType:
          {
           // variable definitions:
            CFStringRef stringReference;

            errorStatus = ::GetDataBrowserItemDataText(itemData,&stringReference);
            if (errorStatus == noErr)
            {
             // variable definitions and initializations:
              wxMacCFStringHolder modifiedString(stringReference);
              wxVariant           modifiedData(modifiedString.AsString());

              if (dataViewCtrlPtr->GetModel()->SetValue(modifiedData,wxDataViewItem(reinterpret_cast<void*>(itemID)),static_cast<unsigned int>(columnIndex)))
                return noErr;
              else
                return errDataBrowserNotConfigured;
            } /* if */
            else
              return errorStatus;
          } /* block */
        default:
          return errDataBrowserPropertyNotSupported;
      } /* switch */
    } /* if */
  } /* if */
  else
  {
    if (propertyID >= kMinPropertyID) // in case data columns set the data
    {
     // variable definitions:
      wxVariant         variant;
      wxDataViewColumn* dataViewColumnPtr;
      wxDataViewCtrl*   dataViewCtrlPtr;

      dataViewCtrlPtr = dynamic_cast<wxDataViewCtrl*>(this->GetPeer());
      wxCHECK_MSG(dataViewCtrlPtr             != NULL,errDataBrowserNotConfigured,_("Pointer to data view control not set correctly."));
      wxCHECK_MSG(dataViewCtrlPtr->GetModel() != NULL,errDataBrowserNotConfigured,_("Pointer to model not set correctly."));
      dataViewColumnPtr = dataViewCtrlPtr->GetColumnPtr(propertyID);
      wxCHECK_MSG(dataViewColumnPtr                != NULL,errDataBrowserNotConfigured,_("No column for the specified column position existing."));
      wxCHECK_MSG(dataViewColumnPtr->GetRenderer() != NULL,errDataBrowserNotConfigured,_("No renderer specified for column."));
      dataViewCtrlPtr->GetModel()->GetValue(variant,wxDataViewItem(reinterpret_cast<void*>(itemID)),dataViewColumnPtr->GetModelColumn());
      if (!(variant.IsNull()))
      {
        dataViewColumnPtr->GetRenderer()->SetDataReference(itemData);
        dataViewColumnPtr->GetRenderer()->SetValue(variant);
        wxCHECK_MSG(dataViewColumnPtr->GetRenderer()->Render(),errDataBrowserNotConfigured,_("Rendering failed."));
      } /* if */
      return noErr;
    } /* if */
    else // react on special system requests
    {
      switch (propertyID)
      {
        case kDataBrowserContainerIsClosableProperty:
          {
           // variable definitions:
            wxDataViewCtrl* dataViewCtrlPtr(dynamic_cast<wxDataViewCtrl*>(this->GetPeer()));

            wxCHECK_MSG(dataViewCtrlPtr != NULL,errDataBrowserNotConfigured,_("Pointer to data view control not set correctly."));
           // initialize wxWidget event:
            wxDataViewEvent dataViewEvent(wxEVT_COMMAND_DATAVIEW_ITEM_COLLAPSING,dataViewCtrlPtr->GetId()); // variable definition

            dataViewEvent.SetEventObject(dataViewCtrlPtr);
            dataViewEvent.SetItem(wxDataViewItem(reinterpret_cast<void*>(itemID)));
           // finally send the equivalent wxWidget event:
            dataViewCtrlPtr->GetEventHandler()->ProcessEvent(dataViewEvent);
           // opening the container is allowed if not vetoed:
            return ::SetDataBrowserItemDataBooleanValue(itemData,dataViewEvent.IsAllowed());
          } /* block */
        case kDataBrowserContainerIsOpenableProperty:
          {
           // variable definitions:
            wxDataViewCtrl* dataViewCtrlPtr(dynamic_cast<wxDataViewCtrl*>(this->GetPeer()));

            wxCHECK_MSG(dataViewCtrlPtr != NULL,errDataBrowserNotConfigured,_("Pointer to data view control not set correctly."));
           // initialize wxWidget event:
            wxDataViewEvent dataViewEvent(wxEVT_COMMAND_DATAVIEW_ITEM_EXPANDING,dataViewCtrlPtr->GetId()); // variable definition

            dataViewEvent.SetEventObject(dataViewCtrlPtr);
            dataViewEvent.SetItem(wxDataViewItem(reinterpret_cast<void*>(itemID)));
           // finally send the equivalent wxWidget event:
            dataViewCtrlPtr->GetEventHandler()->ProcessEvent(dataViewEvent);
           // opening the container is allowed if not vetoed:
            return ::SetDataBrowserItemDataBooleanValue(itemData,dataViewEvent.IsAllowed());
          } /* block */
        case kDataBrowserItemIsContainerProperty:
          {
           // variable definition:
            wxDataViewCtrl* dataViewCtrlPtr(dynamic_cast<wxDataViewCtrl*>(this->GetPeer()));

            wxCHECK_MSG(dataViewCtrlPtr             != NULL,errDataBrowserNotConfigured,_("Pointer to data view control not set correctly."));
            wxCHECK_MSG(dataViewCtrlPtr->GetModel() != NULL,errDataBrowserNotConfigured,_("Pointer to model not set correctly."));
            return ::SetDataBrowserItemDataBooleanValue(itemData,dataViewCtrlPtr->GetModel()->IsContainer(wxDataViewItem(reinterpret_cast<void*>(itemID))));
          } /* block */
        case kDataBrowserItemIsEditableProperty:
          return ::SetDataBrowserItemDataBooleanValue(itemData,true);
      } /* switch */
    } /* if */
  } /* if */
  return errDataBrowserPropertyNotSupported;
} /* wxMacDataViewDataBrowserListViewControl::DataBrowserGetSetItemDataProc(DataBrowserItemID, DataBrowserPropertyID, DataBrowserItemDataRef, Boolean) */

void wxMacDataViewDataBrowserListViewControl::DataBrowserItemNotificationProc(DataBrowserItemID itemID, DataBrowserItemNotification message, DataBrowserItemDataRef itemData)
{
  wxDataViewCtrl* dataViewCtrlPtr(dynamic_cast<wxDataViewCtrl*>(this->GetPeer()));


 // check if the data view control pointer still exists because this call back function can still be called when the control has already been deleted:
  if (dataViewCtrlPtr != NULL)
    switch (message)
    {
      case kDataBrowserContainerClosed:
        {
         // initialize wxWidget event:
          wxDataViewEvent dataViewEvent(wxEVT_COMMAND_DATAVIEW_ITEM_COLLAPSED,dataViewCtrlPtr->GetId()); // variable definition

          dataViewEvent.SetEventObject(dataViewCtrlPtr);
          dataViewEvent.SetItem(wxDataViewItem(reinterpret_cast<void*>(itemID)));
         // finally send the equivalent wxWidget event:
          dataViewCtrlPtr->GetEventHandler()->ProcessEvent(dataViewEvent);
        } /* block */
        break;
      case kDataBrowserContainerOpened:
        {
         // initialize wxWidget event:
          wxDataViewEvent dataViewEvent(wxEVT_COMMAND_DATAVIEW_ITEM_EXPANDED,dataViewCtrlPtr->GetId()); // variable definition

          dataViewEvent.SetEventObject(dataViewCtrlPtr);
          dataViewEvent.SetItem(wxDataViewItem(reinterpret_cast<void*>(itemID)));
         // finally send the equivalent wxWidget event:
          dataViewCtrlPtr->GetEventHandler()->ProcessEvent(dataViewEvent);
         // add children to the expanded container:
          dataViewCtrlPtr->AddChildrenLevel(wxDataViewItem(reinterpret_cast<void*>(itemID)));
        } /* block */
        break;
      case kDataBrowserEditStarted:
        {
         // initialize wxWidget event:
          DataBrowserPropertyID propertyID;
          wxDataViewEvent dataViewEvent(wxEVT_COMMAND_DATAVIEW_ITEM_EDITING_STARTED,dataViewCtrlPtr->GetId()); // variable definition

          dataViewEvent.SetEventObject(dataViewCtrlPtr);
          dataViewEvent.SetItem(wxDataViewItem(reinterpret_cast<void*>(itemID)));
          if (this->GetPropertyID(itemData,&propertyID) == noErr)
          {
           // variable definition and initialization:
            DataBrowserTableViewColumnIndex columnIndex;

            wxCHECK_RET(this->GetColumnIndex(propertyID,&columnIndex),_("Column index not found."));
            dataViewEvent.SetColumn(columnIndex);
            dataViewEvent.SetDataViewColumn(dataViewCtrlPtr->GetColumnPtr(propertyID));
          } /* if */
         // finally send the equivalent wxWidget event:
          dataViewCtrlPtr->GetEventHandler()->ProcessEvent(dataViewEvent);
        } /* block */
        break;
      case kDataBrowserEditStopped:
        {
         // initialize wxWidget event:
          DataBrowserPropertyID propertyID;
          wxDataViewEvent dataViewEvent(wxEVT_COMMAND_DATAVIEW_ITEM_EDITING_DONE,dataViewCtrlPtr->GetId()); // variable definition

          dataViewEvent.SetEventObject(dataViewCtrlPtr);
          dataViewEvent.SetItem(wxDataViewItem(reinterpret_cast<void*>(itemID)));
          if (this->GetPropertyID(itemData,&propertyID) == noErr)
          {
           // variable definition and initialization:
            DataBrowserTableViewColumnIndex columnIndex;

            wxCHECK_RET(this->GetColumnIndex(propertyID,&columnIndex),_("Column index not found."));
            dataViewEvent.SetColumn(columnIndex);
            dataViewEvent.SetDataViewColumn(dataViewCtrlPtr->GetColumnPtr(propertyID));
          } /* if */
         // finally send the equivalent wxWidget event:
          dataViewCtrlPtr->GetEventHandler()->ProcessEvent(dataViewEvent);
        } /* block */
        break;
      case kDataBrowserItemAdded:
        break; // not used anymore
      case kDataBrowserItemDeselected:
        break; // not implemented by wxWidgets; see kDataBrowserSelectionSetChanged
      case kDataBrowserItemDoubleClicked:
        {
         // initialize wxWidget event:
          wxDataViewEvent dataViewEvent(wxEVT_COMMAND_DATAVIEW_ITEM_ACTIVATED,dataViewCtrlPtr->GetId()); // variable definition

          dataViewEvent.SetEventObject(dataViewCtrlPtr);
          dataViewEvent.SetItem(wxDataViewItem(reinterpret_cast<void*>(itemID)));
         // finally send the equivalent wxWidget event:
          dataViewCtrlPtr->GetEventHandler()->ProcessEvent(dataViewEvent);
        } /* block */
        break;
      case kDataBrowserItemRemoved:
        break; // not used anymore
      case kDataBrowserItemSelected:
        break; // not implemented by wxWidgets; see kDataBrowserSelectionSetChanged
      case kDataBrowserSelectionSetChanged:
        {
         // initialize wxWidget event:
          wxDataViewEvent dataViewEvent(wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED,dataViewCtrlPtr->GetId()); // variable definition

          dataViewEvent.SetEventObject(dataViewCtrlPtr);
          dataViewEvent.SetModel      (dataViewCtrlPtr->GetModel());
         // finally send the equivalent wxWidget event:
          dataViewCtrlPtr->GetEventHandler()->ProcessEvent(dataViewEvent);
        } /* block */
        break;
      case kDataBrowserUserStateChanged:
        {
         // update column widths:
          for (size_t i=0; i<dataViewCtrlPtr->GetColumnCount(); ++i)
          {
           // constant definition for abbreviational purposes:
            wxDataViewColumn* const columnPtr = dataViewCtrlPtr->GetColumn(i);
           // variable definition:
            UInt16 columnWidth;

            wxCHECK_RET(this->GetColumnWidth(columnPtr->GetPropertyID(),&columnWidth) == noErr,_("Column width could not be determined"));
            columnPtr->SetWidthVariable(columnWidth);
          } /* for */
         // update order status:
          DataBrowserPropertyID propertyID; // variable definition

          if ((this->GetSortProperty(&propertyID) == noErr) && (propertyID >= kMinPropertyID))
          {
            DataBrowserSortOrder            sortOrder;
            DataBrowserTableViewColumnIndex columnIndex;

            if ((this->GetSortOrder(&sortOrder) == noErr) && (this->GetColumnIndex(propertyID,&columnIndex) == noErr))
            {
             // variable definition and initialization:
              wxDataViewColumn* columnPtr;
              columnPtr = dataViewCtrlPtr->GetColumn(columnIndex);
             // check if the sort order has changed:
              if (  columnPtr->IsSortOrderAscending()  && (sortOrder == kDataBrowserOrderDecreasing) ||
                  !(columnPtr->IsSortOrderAscending()) && (sortOrder == kDataBrowserOrderIncreasing))
              {
                columnPtr->SetSortOrder(!(columnPtr->IsSortOrderAscending()));
               // initialize wxWidget event:
                wxDataViewEvent dataViewEvent(wxEVT_COMMAND_DATAVIEW_COLUMN_SORTED,dataViewCtrlPtr->GetId()); // variable defintion

                dataViewEvent.SetEventObject(dataViewCtrlPtr);
                dataViewEvent.SetColumn(columnIndex);
                dataViewEvent.SetDataViewColumn(columnPtr);
               // finally send the equivalent wxWidget event:
                dataViewCtrlPtr->GetEventHandler()->ProcessEvent(dataViewEvent);
              } /* if */
            } /* if */
          } /* if */
        } /* block */
        break;
    } /* switch */
} /* wxMacDataViewDataBrowserListViewControl::DataBrowserItemNotificationProc(DataBrowserItemID, DataBrowserItemNotification, DataBrowserItemDataRef) */


#endif // wxUSE_DATAVIEWCTRL && !wxUSE_GENERICDATAVIEWCTRL
