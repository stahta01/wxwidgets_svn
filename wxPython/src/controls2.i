/////////////////////////////////////////////////////////////////////////////
// Name:        controls2.i
// Purpose:     More control (widget) classes for wxPython
//
// Author:      Robin Dunn
//
// Created:     6/10/98
// RCS-ID:      $Id$
// Copyright:   (c) 1998 by Total Control Software
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

%module controls2

%{
#include "helpers.h"
#ifdef __WXMSW__
#include <windows.h>
#endif
#include <wx/listctrl.h>
#include <wx/treectrl.h>
#include <wx/imaglist.h>
%}

//----------------------------------------------------------------------

%include typemaps.i
%include my_typemaps.i

// Import some definitions of other classes, etc.
%import _defs.i
%import misc.i
%import windows.i
%import gdi.i
%import events.i
%import controls.i

%pragma(python) code = "import wx"


//----------------------------------------------------------------------
//----------------------------------------------------------------------

enum {
    /* List control event types */
    wxEVT_COMMAND_LIST_BEGIN_DRAG,
    wxEVT_COMMAND_LIST_BEGIN_RDRAG,
    wxEVT_COMMAND_LIST_BEGIN_LABEL_EDIT,
    wxEVT_COMMAND_LIST_END_LABEL_EDIT,
    wxEVT_COMMAND_LIST_DELETE_ITEM,
    wxEVT_COMMAND_LIST_DELETE_ALL_ITEMS,
    wxEVT_COMMAND_LIST_GET_INFO,
    wxEVT_COMMAND_LIST_SET_INFO,
    wxEVT_COMMAND_LIST_ITEM_SELECTED,
    wxEVT_COMMAND_LIST_ITEM_DESELECTED,
    wxEVT_COMMAND_LIST_KEY_DOWN,
    wxEVT_COMMAND_LIST_INSERT_ITEM,
    wxEVT_COMMAND_LIST_COL_CLICK,
    wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK,
    wxEVT_COMMAND_LIST_ITEM_MIDDLE_CLICK,
    wxEVT_COMMAND_LIST_ITEM_ACTIVATED,
    wxEVT_COMMAND_LIST_CACHE_HINT,
};


%pragma(python) code = "
def EVT_LIST_BEGIN_DRAG(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_LIST_BEGIN_DRAG, func)

def EVT_LIST_BEGIN_RDRAG(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_LIST_BEGIN_RDRAG, func)

def EVT_LIST_BEGIN_LABEL_EDIT(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_LIST_BEGIN_LABEL_EDIT, func)

def EVT_LIST_END_LABEL_EDIT(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_LIST_END_LABEL_EDIT, func)

def EVT_LIST_DELETE_ITEM(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_LIST_DELETE_ITEM, func)

def EVT_LIST_DELETE_ALL_ITEMS(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_LIST_DELETE_ALL_ITEMS, func)

def EVT_LIST_GET_INFO(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_LIST_GET_INFO, func)

def EVT_LIST_SET_INFO(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_LIST_SET_INFO, func)

def EVT_LIST_ITEM_SELECTED(win, id, func):
    win.Connect(id, -1,  wxEVT_COMMAND_LIST_ITEM_SELECTED, func)

def EVT_LIST_ITEM_DESELECTED(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_LIST_ITEM_DESELECTED, func)

def EVT_LIST_KEY_DOWN(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_LIST_KEY_DOWN, func)

def EVT_LIST_INSERT_ITEM(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_LIST_INSERT_ITEM, func)

def EVT_LIST_COL_CLICK(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_LIST_COL_CLICK, func)

def EVT_LIST_ITEM_RIGHT_CLICK(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK, func)

def EVT_LIST_ITEM_MIDDLE_CLICK(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_LIST_ITEM_MIDDLE_CLICK, func)

def EVT_LIST_ITEM_ACTIVATED(win, id, func):
    win.Connect(id, -1,  wxEVT_COMMAND_LIST_ITEM_ACTIVATED, func)

def EVT_LIST_CACHE_HINT(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_LIST_CACHE_HINT, func)
"


enum {
    /* Style flags */
    wxLC_VRULES,
    wxLC_HRULES,

    wxLC_ICON,
    wxLC_SMALL_ICON,
    wxLC_LIST,
    wxLC_REPORT,

    wxLC_ALIGN_TOP,
    wxLC_ALIGN_LEFT,
    wxLC_AUTOARRANGE,
    wxLC_VIRTUAL,
    wxLC_EDIT_LABELS,
    wxLC_NO_HEADER,
    wxLC_NO_SORT_HEADER,
    wxLC_SINGLE_SEL,
    wxLC_SORT_ASCENDING,
    wxLC_SORT_DESCENDING,

    wxLC_MASK_TYPE,
    wxLC_MASK_ALIGN,
    wxLC_MASK_SORT,

};


enum {
    // Mask flags
    wxLIST_MASK_STATE,
    wxLIST_MASK_TEXT,
    wxLIST_MASK_IMAGE,
    wxLIST_MASK_DATA,
    wxLIST_SET_ITEM,
    wxLIST_MASK_WIDTH,
    wxLIST_MASK_FORMAT,

    // State flags
    wxLIST_STATE_DONTCARE,
    wxLIST_STATE_DROPHILITED,
    wxLIST_STATE_FOCUSED,
    wxLIST_STATE_SELECTED,
    wxLIST_STATE_CUT,

    // Hit test flags
    wxLIST_HITTEST_ABOVE,
    wxLIST_HITTEST_BELOW,
    wxLIST_HITTEST_NOWHERE,
    wxLIST_HITTEST_ONITEMICON,
    wxLIST_HITTEST_ONITEMLABEL,
    wxLIST_HITTEST_ONITEMRIGHT,
    wxLIST_HITTEST_ONITEMSTATEICON,
    wxLIST_HITTEST_TOLEFT,
    wxLIST_HITTEST_TORIGHT,
    wxLIST_HITTEST_ONITEM,

    // Flags for GetNextItem
    wxLIST_NEXT_ABOVE,
    wxLIST_NEXT_ALL,
    wxLIST_NEXT_BELOW,
    wxLIST_NEXT_LEFT,
    wxLIST_NEXT_RIGHT,

    // Alignment flags
    wxLIST_ALIGN_DEFAULT,
    wxLIST_ALIGN_LEFT,
    wxLIST_ALIGN_TOP,
    wxLIST_ALIGN_SNAP_TO_GRID,

    // Autosize values for SetColumnWidth
    wxLIST_AUTOSIZE = -1,
    wxLIST_AUTOSIZE_USEHEADER = -2,

    // Flag values for GetItemRect
    wxLIST_RECT_BOUNDS,
    wxLIST_RECT_ICON,
    wxLIST_RECT_LABEL,

    // Flag values for FindItem (MSW only)
    wxLIST_FIND_UP,
    wxLIST_FIND_DOWN,
    wxLIST_FIND_LEFT,
    wxLIST_FIND_RIGHT,


};



enum wxListColumnFormat
{
    wxLIST_FORMAT_LEFT,
    wxLIST_FORMAT_RIGHT,
    wxLIST_FORMAT_CENTRE,
    wxLIST_FORMAT_CENTER = wxLIST_FORMAT_CENTRE
};




class wxListItemAttr
{
public:
    // ctors
    //wxListItemAttr();
    wxListItemAttr(const wxColour& colText = wxNullColour,
                   const wxColour& colBack = wxNullColour,
                   const wxFont& font = wxNullFont);


    // setters
    void SetTextColour(const wxColour& colText);
    void SetBackgroundColour(const wxColour& colBack);
    void SetFont(const wxFont& font);

    // accessors
    bool HasTextColour();
    bool HasBackgroundColour();
    bool HasFont();

    const wxColour& GetTextColour();
    const wxColour& GetBackgroundColour();
    const wxFont& GetFont();
};


class wxListItem : public wxObject {
public:
    wxListItem();
    ~wxListItem();

    // resetting
    void Clear();
    void ClearAttributes();

    // setters
    void SetMask(long mask);
    void SetId(long id);
    void SetColumn(int col);
    void SetState(long state);
    void SetStateMask(long stateMask);
    void SetText(const wxString& text);
    void SetImage(int image);
    void SetData(long data);

    void SetWidth(int width);
    void SetAlign(wxListColumnFormat align);

    void SetTextColour(const wxColour& colText);
    void SetBackgroundColour(const wxColour& colBack);
    void SetFont(const wxFont& font);

    // accessors
    long GetMask();
    long GetId();
    int GetColumn();
    long GetState();
    const wxString& GetText();
    int GetImage();
    long GetData();

    int GetWidth();
    wxListColumnFormat GetAlign();

    wxListItemAttr *GetAttributes();
    bool HasAttributes();

    wxColour GetTextColour() const;
    wxColour GetBackgroundColour() const;
    wxFont GetFont() const;

    // these members are public for compatibility
    long            m_mask;     // Indicates what fields are valid
    long            m_itemId;   // The zero-based item position
    int             m_col;      // Zero-based column, if in report mode
    long            m_state;    // The state of the item
    long            m_stateMask;// Which flags of m_state are valid (uses same flags)
    wxString        m_text;     // The label/header text
    int             m_image;    // The zero-based index into an image list
    long            m_data;     // App-defined data

    // For columns only
    int             m_format;   // left, right, centre
    int             m_width;    // width of column

};


class wxListEvent: public wxNotifyEvent {
public:
    int           m_code;
    long          m_itemIndex;
    long          m_oldItemIndex;
    int           m_col;
    bool          m_cancelled;
    wxPoint       m_pointDrag;
    wxListItem    m_item;

    int GetCode();
    long GetIndex();
    long GetOldIndex();
    long GetOldItem();
    int GetColumn();
    bool Cancelled();
    wxPoint GetPoint();
    const wxString& GetLabel();
    const wxString& GetText();
    int GetImage();
    long GetData();
    long GetMask();
    const wxListItem& GetItem();
};


%{
class wxPyListCtrl : public wxListCtrl {
    DECLARE_ABSTRACT_CLASS(wxPyListCtrl);
public:
    wxPyListCtrl(wxWindow* parent, wxWindowID id,
                 const wxPoint& pos,
                 const wxSize& size,
                 long style,
                 const wxValidator& validator,
                 char* name) :
        wxListCtrl(parent, id, pos, size, style, validator, name) {}

    DEC_PYCALLBACK_STRING_LONGLONG(OnGetItemText);
    DEC_PYCALLBACK_INT_LONG(OnGetItemImage);
    DEC_PYCALLBACK_LISTATTR_LONG(OnGetItemAttr);

    PYPRIVATE;
};

IMPLEMENT_ABSTRACT_CLASS(wxPyListCtrl, wxListCtrl);

IMP_PYCALLBACK_STRING_LONGLONG(wxPyListCtrl, wxListCtrl, OnGetItemText);
IMP_PYCALLBACK_INT_LONG(wxPyListCtrl, wxListCtrl, OnGetItemImage);
IMP_PYCALLBACK_LISTATTR_LONG(wxPyListCtrl, wxListCtrl, OnGetItemAttr);
%}


%name(wxListCtrl)class wxPyListCtrl : public wxControl {
public:
    wxPyListCtrl(wxWindow* parent, wxWindowID id = -1,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = wxLC_ICON,
                 const wxValidator& validator = wxDefaultValidator,
                 char* name = "listCtrl");

    void _setSelf(PyObject* self, PyObject* _class);
    %pragma(python) addtomethod = "__init__:#wx._StdWindowCallbacks(self)"
    %pragma(python) addtomethod = "__init__:self._setSelf(self, wxListCtrl)"


    // Set the control colours
    bool SetForegroundColour(const wxColour& col);
    bool SetBackgroundColour(const wxColour& col);

    // Gets information about this column
    bool GetColumn(int col, wxListItem& item) const;

    // Sets information about this column
    bool SetColumn(int col, wxListItem& item) ;

    // Gets the column width
    int GetColumnWidth(int col) const;

    // Sets the column width
    bool SetColumnWidth(int col, int width) ;

    // Gets the number of items that can fit vertically in the
    // visible area of the list control (list or report view)
    // or the total number of items in the list control (icon
    // or small icon view)
    int GetCountPerPage() const;

#ifdef __WXMSW__
    // Gets the edit control for editing labels.
    wxTextCtrl* GetEditControl() const;
#endif

    //bool GetItem(wxListItem& info) const ;
    %addmethods {
        // Gets information about the item
        %new wxListItem* GetItem(long itemId, int col=0) {
            wxListItem* info = new wxListItem;
            info->m_itemId = itemId;
            info->m_col = col;
            info->m_mask = 0xFFFF;
            self->GetItem(*info);
            return info;
        }
    }  // The OOR typemaps don't know what to do with the %new, so fix it up.
    %pragma(python) addtoclass = "
    def GetItem(self, *_args, **_kwargs):
        val = apply(controls2c.wxListCtrl_GetItem,(self,) + _args, _kwargs)
        val.thisown = 1
        return val
    "


    // Sets information about the item
    bool SetItem(wxListItem& info) ;

    // Sets a string field at a particular column
    %name(SetStringItem)long SetItem(long index, int col, const wxString& label, int imageId = -1);

    // Gets the item state
    int  GetItemState(long item, long stateMask) const ;

    // Sets the item state
    bool SetItemState(long item, long state, long stateMask) ;

    // Sets the item image
    bool SetItemImage(long item, int image, int selImage) ;

    // Gets the item text
    wxString GetItemText(long item) const ;

    // Sets the item text
    void SetItemText(long item, const wxString& str) ;

    // Gets the item data
    long GetItemData(long item) const ;

    // Sets the item data
    bool SetItemData(long item, long data) ;


    //bool GetItemRect(long item, wxRect& rect, int code = wxLIST_RECT_BOUNDS) const ;
    //bool GetItemPosition(long item, wxPoint& pos) const ;

    // Gets the item position
    %addmethods {
        %new wxPoint* GetItemPosition(long item) {
            wxPoint* pos = new wxPoint;
            self->GetItemPosition(item, *pos);
            return pos;
        }
        // Gets the item rectangle
        %new wxRect* GetItemRect(long item, int code = wxLIST_RECT_BOUNDS) {
            wxRect* rect= new wxRect;
            self->GetItemRect(item, *rect, code);
            return rect;
        }
    }


    // Sets the item position
    bool SetItemPosition(long item, const wxPoint& pos) ;

    // Gets the number of items in the list control
    int GetItemCount() const;

    // Gets the number of columns in the list control
    int GetColumnCount() const { return m_colCount; }

    // Retrieves the spacing between icons in pixels.
    // If small is TRUE, gets the spacing for the small icon
    // view, otherwise the large icon view.
    int GetItemSpacing(bool isSmall) const;

    // Gets the number of selected items in the list control
    int GetSelectedItemCount() const;

    // Gets the text colour of the listview
    wxColour GetTextColour() const;

    // Sets the text colour of the listview
    void SetTextColour(const wxColour& col);

    // Gets the index of the topmost visible item when in
    // list or report view
    long GetTopItem() const ;

    // Add or remove a single window style
    void SetSingleStyle(long style, bool add = TRUE) ;

    // Set the whole window style
    void SetWindowStyleFlag(long style) ;

    // Searches for an item, starting from 'item'.
    // item can be -1 to find the first item that matches the
    // specified flags.
    // Returns the item or -1 if unsuccessful.
    long GetNextItem(long item, int geometry = wxLIST_NEXT_ALL, int state = wxLIST_STATE_DONTCARE) const ;

    // Gets one of the three image lists
    wxImageList *GetImageList(int which) const ;

    // Sets the image list
    void SetImageList(wxImageList *imageList, int which) ;
    void AssignImageList(wxImageList *imageList, int which) ;
    %pragma(python) addtomethod = "AssignImageList:_args[0].thisown = 0"

    // returns true if it is a virtual list control
    bool IsVirtual() const;

    // refresh items selectively (only useful for virtual list controls)
    void RefreshItem(long item);
    void RefreshItems(long itemFrom, long itemTo);




    // Arranges the items
    bool Arrange(int flag = wxLIST_ALIGN_DEFAULT);

    // Deletes an item
    bool DeleteItem(long item);

    // Deletes all items
    bool DeleteAllItems() ;

    // Deletes a column
    bool DeleteColumn(int col);

    // Deletes all columns
    bool DeleteAllColumns();

    // Clears items, and columns if there are any.
    void ClearAll();

#ifdef __WXMSW__
    // Edit the label
    wxTextCtrl* EditLabel(long item /*, wxClassInfo* textControlClass = CLASSINFO(wxTextCtrl)*/);

    // End label editing, optionally cancelling the edit
    bool EndEditLabel(bool cancel);
#endif

    // Ensures this item is visible
    bool EnsureVisible(long item) ;

    // Find an item whose label matches this string, starting from the item after 'start'
    // or the beginning if 'start' is -1.
    long FindItem(long start, const wxString& str, bool partial = FALSE);

    // Find an item whose data matches this data, starting from the item after 'start'
    // or the beginning if 'start' is -1.
    %name(FindItemData)long FindItem(long start, long data);

    // Find an item nearest this position in the specified direction, starting from
    // the item after 'start' or the beginning if 'start' is -1.
    %name(FindItemAtPos)long FindItem(long start, const wxPoint& pt, int direction);

    // Determines which item (if any) is at the specified point,
    // giving details in the second return value (see wxLIST_HITTEST_... flags above)
    long HitTest(const wxPoint& point, int& OUTPUT);

    // Inserts an item, returning the index of the new item if successful,
    // -1 otherwise.
    long InsertItem(wxListItem& info);

    // Insert a string item
    %name(InsertStringItem)long InsertItem(long index, const wxString& label);

    // Insert an image item
    %name(InsertImageItem)long InsertItem(long index, int imageIndex);

    // Insert an image/string item
    %name(InsertImageStringItem)long InsertItem(long index, const wxString& label, int imageIndex);

    // For list view mode (only), inserts a column.
    %name(InsertColumnInfo)long InsertColumn(long col, wxListItem& info);

    long InsertColumn(long col,
                      const wxString& heading,
                      int format = wxLIST_FORMAT_LEFT,
                      int width = -1);

    // set the number of items in a virtual list control
    void SetItemCount(long count);

    // Scrolls the list control. If in icon, small icon or report view mode,
    // x specifies the number of pixels to scroll. If in list view mode, x
    // specifies the number of columns to scroll.
    // If in icon, small icon or list view mode, y specifies the number of pixels
    // to scroll. If in report view mode, y specifies the number of lines to scroll.
    bool ScrollList(int dx, int dy);

    // bool SortItems(wxListCtrlCompare fn, long data);
    %addmethods {
        // Sort items.
        // func is a function which takes 2 long arguments: item1, item2.
        // item1 is the long data associated with a first item (NOT the index).
        // item2 is the long data associated with a second item (NOT the index).
        // The return value is a negative number if the first item should precede the second
        // item, a positive number of the second item should precede the first,
        // or zero if the two items are equivalent.
        bool SortItems(PyObject* func) {
            if (!PyCallable_Check(func))
                return FALSE;
            return self->SortItems(wxPyListCtrl_SortItems, (long)func);
        }
    }

};



%{
    int wxCALLBACK wxPyListCtrl_SortItems(long item1, long item2, long funcPtr) {
        int retval = 0;
        PyObject* func = (PyObject*)funcPtr;
        bool doSave = wxPyRestoreThread();

        PyObject* args = Py_BuildValue("(ii)", item1, item2);
        PyObject* result = PyEval_CallObject(func, args);
        Py_DECREF(args);
        if (result) {
            retval = PyInt_AsLong(result);
            Py_DECREF(result);
        }

        wxPySaveThread(doSave);
        return retval;
    }

%}

//----------------------------------------------------------------------

// wxTreeCtrl flags
enum {
    wxTR_NO_BUTTONS,
    wxTR_HAS_BUTTONS,
    wxTR_TWIST_BUTTONS,
    wxTR_NO_LINES,
    wxTR_MAC_BUTTONS,

    wxTR_SINGLE,
    wxTR_MULTIPLE,
    wxTR_EXTENDED,

    wxTR_EDIT_LABELS,
    wxTR_LINES_AT_ROOT,
    wxTR_HIDE_ROOT,
    wxTR_ROW_LINES,
    wxTR_HAS_VARIABLE_ROW_HEIGHT,

    wxTR_DEFAULT_STYLE,
};

enum wxTreeItemIcon
{
    wxTreeItemIcon_Normal,              // not selected, not expanded
    wxTreeItemIcon_Selected,            //     selected, not expanded
    wxTreeItemIcon_Expanded,            // not selected,     expanded
    wxTreeItemIcon_SelectedExpanded,    //     selected,     expanded
    wxTreeItemIcon_Max
};


// constants for HitTest
enum {
    wxTREE_HITTEST_ABOVE,
    wxTREE_HITTEST_BELOW,
    wxTREE_HITTEST_NOWHERE,
    wxTREE_HITTEST_ONITEMBUTTON,
    wxTREE_HITTEST_ONITEMICON,
    wxTREE_HITTEST_ONITEMINDENT,
    wxTREE_HITTEST_ONITEMLABEL,
    wxTREE_HITTEST_ONITEMRIGHT,
    wxTREE_HITTEST_ONITEMSTATEICON,
    wxTREE_HITTEST_TOLEFT,
    wxTREE_HITTEST_TORIGHT,
    wxTREE_HITTEST_ONITEMUPPERPART,
    wxTREE_HITTEST_ONITEMLOWERPART,
    wxTREE_HITTEST_ONITEM
};


enum {
    /* Tree control event types */
    wxEVT_COMMAND_TREE_BEGIN_DRAG,
    wxEVT_COMMAND_TREE_BEGIN_RDRAG,
    wxEVT_COMMAND_TREE_BEGIN_LABEL_EDIT,
    wxEVT_COMMAND_TREE_END_LABEL_EDIT,
    wxEVT_COMMAND_TREE_DELETE_ITEM,
    wxEVT_COMMAND_TREE_GET_INFO,
    wxEVT_COMMAND_TREE_SET_INFO,
    wxEVT_COMMAND_TREE_ITEM_EXPANDED,
    wxEVT_COMMAND_TREE_ITEM_EXPANDING,
    wxEVT_COMMAND_TREE_ITEM_COLLAPSED,
    wxEVT_COMMAND_TREE_ITEM_COLLAPSING,
    wxEVT_COMMAND_TREE_SEL_CHANGED,
    wxEVT_COMMAND_TREE_SEL_CHANGING,
    wxEVT_COMMAND_TREE_KEY_DOWN,
    wxEVT_COMMAND_TREE_ITEM_ACTIVATED,
    wxEVT_COMMAND_TREE_ITEM_RIGHT_CLICK,
    wxEVT_COMMAND_TREE_ITEM_MIDDLE_CLICK,
    wxEVT_COMMAND_TREE_END_DRAG,
};


%pragma(python) code = "
# wxTreeCtrl events
def EVT_TREE_BEGIN_DRAG(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_TREE_BEGIN_DRAG, func)

def EVT_TREE_BEGIN_RDRAG(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_TREE_BEGIN_RDRAG, func)

def EVT_TREE_END_DRAG(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_TREE_END_DRAG, func)

def EVT_TREE_BEGIN_LABEL_EDIT(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_TREE_BEGIN_LABEL_EDIT, func)

def EVT_TREE_END_LABEL_EDIT(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_TREE_END_LABEL_EDIT, func)

def EVT_TREE_GET_INFO(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_TREE_GET_INFO, func)

def EVT_TREE_SET_INFO(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_TREE_SET_INFO, func)

def EVT_TREE_ITEM_EXPANDED(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_TREE_ITEM_EXPANDED, func)

def EVT_TREE_ITEM_EXPANDING(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_TREE_ITEM_EXPANDING, func)

def EVT_TREE_ITEM_COLLAPSED(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_TREE_ITEM_COLLAPSED, func)

def EVT_TREE_ITEM_COLLAPSING(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_TREE_ITEM_COLLAPSING, func)

def EVT_TREE_SEL_CHANGED(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_TREE_SEL_CHANGED, func)

def EVT_TREE_SEL_CHANGING(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_TREE_SEL_CHANGING, func)

def EVT_TREE_KEY_DOWN(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_TREE_KEY_DOWN, func)

def EVT_TREE_DELETE_ITEM(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_TREE_DELETE_ITEM, func)

def EVT_TREE_ITEM_ACTIVATED(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_TREE_ITEM_ACTIVATED, func)

def EVT_TREE_ITEM_RIGHT_CLICK(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_TREE_ITEM_RIGHT_CLICK, func)

def EVT_TREE_ITEM_MIDDLE_CLICK(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_TREE_ITEM_MIDDLE_CLICK, func)
"


class wxTreeItemAttr
{
public:
    // ctors
    //wxTreeItemAttr() { }
    wxTreeItemAttr(const wxColour& colText = wxNullColour,
                   const wxColour& colBack = wxNullColour,
                   const wxFont& font = wxNullFont);

    // setters
    void SetTextColour(const wxColour& colText);
    void SetBackgroundColour(const wxColour& colBack);
    void SetFont(const wxFont& font);

    // accessors
    bool HasTextColour();
    bool HasBackgroundColour();
    bool HasFont();

    const wxColour& GetTextColour();
    const wxColour& GetBackgroundColour();
    const wxFont& GetFont();
};


class wxTreeItemId {
public:
    wxTreeItemId();
    ~wxTreeItemId();
    bool IsOk();

    %addmethods {
        int __cmp__(wxTreeItemId* other) {
            if (! other) return -1;
            return *self != *other;
        }
    }
};



%{
class wxPyTreeItemData : public wxTreeItemData {
public:
    wxPyTreeItemData(PyObject* obj = NULL) {
        if (obj == NULL)
            obj = Py_None;
        Py_INCREF(obj);
        m_obj = obj;
    }

    ~wxPyTreeItemData() {
        bool doSave = wxPyRestoreThread();
        Py_DECREF(m_obj);
        wxPySaveThread(doSave);
    }

    PyObject* GetData() {
        Py_INCREF(m_obj);
        return m_obj;
    }

    void SetData(PyObject* obj) {
        bool doSave = wxPyRestoreThread();
        Py_DECREF(m_obj);
        wxPySaveThread(doSave);
        m_obj = obj;
        Py_INCREF(obj);
    }

    PyObject* m_obj;
};
%}



%name(wxTreeItemData) class wxPyTreeItemData : public wxObject {
public:
    wxPyTreeItemData(PyObject* obj = NULL);

    PyObject* GetData();
    void      SetData(PyObject* obj);

    const wxTreeItemId& GetId();
    void                SetId(const wxTreeItemId& id);
};



class wxTreeEvent : public wxNotifyEvent {
public:
    wxTreeEvent(wxEventType commandType = wxEVT_NULL, int id = 0);

    wxTreeItemId GetItem();
    wxTreeItemId GetOldItem();
    wxPoint GetPoint();
    int GetCode();
    const wxString& GetLabel();
};



%{
class wxPyTreeCtrl : public wxTreeCtrl {
    DECLARE_ABSTRACT_CLASS(wxPyTreeCtrl);
public:
    wxPyTreeCtrl(wxWindow *parent, wxWindowID id,
                 const wxPoint& pos,
                 const wxSize& size,
                 long style,
                 const wxValidator& validator,
                 char* name) :
        wxTreeCtrl(parent, id, pos, size, style, validator, name) {}

    int OnCompareItems(const wxTreeItemId& item1,
                       const wxTreeItemId& item2) {
        int rval = 0;
        bool doSave = wxPyRestoreThread();
        if (m_myInst.findCallback("OnCompareItems"))
            rval = m_myInst.callCallback(Py_BuildValue(
                "(OO)",
                wxPyConstructObject((void*)&item1, "wxTreeItemId"),
                wxPyConstructObject((void*)&item2, "wxTreeItemId")));
        else
            rval = wxTreeCtrl::OnCompareItems(item1, item2);
        wxPySaveThread(doSave);
        return rval;
    }
    PYPRIVATE;
};

IMPLEMENT_ABSTRACT_CLASS(wxPyTreeCtrl, wxTreeCtrl);

%}

// These are for the GetFirstChild/GetNextChild methods below
%typemap(python, in)     long& INOUT = long* INOUT;
%typemap(python, argout) long& INOUT = long* INOUT;


%name(wxTreeCtrl)class wxPyTreeCtrl : public wxControl {
public:
    wxPyTreeCtrl(wxWindow *parent, wxWindowID id = -1,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = wxTR_HAS_BUTTONS | wxTR_LINES_AT_ROOT,
               const wxValidator& validator = wxDefaultValidator,
               char* name = "wxTreeCtrl");

    void _setSelf(PyObject* self, PyObject* _class);
    %pragma(python) addtomethod = "__init__:#wx._StdWindowCallbacks(self)"
    %pragma(python) addtomethod = "__init__:self._setSelf(self, wxTreeCtrl)"

    size_t GetCount();
    unsigned int GetIndent();
    void SetIndent(unsigned int indent);
    wxImageList *GetImageList();
    wxImageList *GetStateImageList();
    void SetImageList(wxImageList *imageList);
    void SetStateImageList(wxImageList *imageList);
    void AssignImageList(wxImageList* imageList);
    %pragma(python) addtomethod = "AssignImageList:_args[0].thisown = 0"
    void AssignStateImageList(wxImageList* imageList);
    %pragma(python) addtomethod = "AssignStateImageList:_args[0].thisown = 0"

    unsigned int GetSpacing();
    void SetSpacing(unsigned int spacing);

    wxString GetItemText(const wxTreeItemId& item);
    int GetItemImage(const wxTreeItemId& item,
                     wxTreeItemIcon which = wxTreeItemIcon_Normal);
    int GetItemSelectedImage(const wxTreeItemId& item);

    void SetItemText(const wxTreeItemId& item, const wxString& text);
    void SetItemImage(const wxTreeItemId& item, int image,
                      wxTreeItemIcon which = wxTreeItemIcon_Normal);
    void SetItemSelectedImage(const wxTreeItemId& item, int image);
    void SetItemHasChildren(const wxTreeItemId& item, bool hasChildren = TRUE);

    %addmethods {
        // [Get|Set]ItemData substitutes.  Automatically create wxPyTreeItemData
        // if needed.
        wxPyTreeItemData* GetItemData(const wxTreeItemId& item) {
            wxPyTreeItemData* data = (wxPyTreeItemData*)self->GetItemData(item);
            if (data == NULL) {
                data = new wxPyTreeItemData();
                data->SetId(item); // set the id
                self->SetItemData(item, data);
            }
            return data;
        }

        void SetItemData(const wxTreeItemId& item, wxPyTreeItemData* data) {
            data->SetId(item); // set the id
            self->SetItemData(item, data);
        }

        // [Get|Set]PyData are short-cuts.  Also made somewhat crash-proof by
        // automatically creating data classes.
        PyObject* GetPyData(const wxTreeItemId& item) {
            wxPyTreeItemData* data = (wxPyTreeItemData*)self->GetItemData(item);
            if (data == NULL) {
                data = new wxPyTreeItemData();
                data->SetId(item); // set the id
                self->SetItemData(item, data);
            }
            return data->GetData();
        }

        void SetPyData(const wxTreeItemId& item, PyObject* obj) {
            wxPyTreeItemData* data = (wxPyTreeItemData*)self->GetItemData(item);
            if (data == NULL) {
                data = new wxPyTreeItemData(obj);
                data->SetId(item); // set the id
                self->SetItemData(item, data);
            } else
                data->SetData(obj);
        }
    }


    bool IsVisible(const wxTreeItemId& item);
    bool ItemHasChildren(const wxTreeItemId& item);
    bool IsExpanded(const wxTreeItemId& item);
    bool IsSelected(const wxTreeItemId& item);

    wxTreeItemId GetRootItem();
    wxTreeItemId GetSelection();
    %name(GetItemParent) wxTreeItemId GetParent(const wxTreeItemId& item);
    //size_t GetSelections(wxArrayTreeItemIds& selection);
    %addmethods {
        PyObject* GetSelections() {
            bool doSave = wxPyRestoreThread();
            PyObject*           rval = PyList_New(0);
            wxArrayTreeItemIds  array;
            size_t              num, x;
            num = self->GetSelections(array);
            for (x=0; x < num; x++) {
                wxTreeItemId *tii = new wxTreeItemId(array.Item(x));
                PyObject* item = wxPyConstructObject((void*)tii, "wxTreeItemId", TRUE);
                PyList_Append(rval, item);
            }
            wxPySaveThread(doSave);
            return rval;
        }
    }



    size_t GetChildrenCount(const wxTreeItemId& item, bool recursively = TRUE);

    wxTreeItemId GetFirstChild(const wxTreeItemId& item, long& INOUT);
    wxTreeItemId GetNextChild(const wxTreeItemId& item, long& INOUT);
    wxTreeItemId GetNextSibling(const wxTreeItemId& item);
    wxTreeItemId GetPrevSibling(const wxTreeItemId& item);
    wxTreeItemId GetFirstVisibleItem();
    wxTreeItemId GetNextVisible(const wxTreeItemId& item);
    wxTreeItemId GetPrevVisible(const wxTreeItemId& item);
    wxTreeItemId GetLastChild(const wxTreeItemId& item);



    wxTreeItemId AddRoot(const wxString& text,
                         int image = -1, int selectedImage = -1,
                         wxPyTreeItemData *data = NULL);
    wxTreeItemId PrependItem(const wxTreeItemId& parent,
                             const wxString& text,
                             int image = -1, int selectedImage = -1,
                             wxPyTreeItemData *data = NULL);
    wxTreeItemId InsertItem(const wxTreeItemId& parent,
                            const wxTreeItemId& idPrevious,
                            const wxString& text,
                            int image = -1, int selectedImage = -1,
                            wxPyTreeItemData *data = NULL);
    %name(InsertItemBefore)
        wxTreeItemId InsertItem(const wxTreeItemId& parent,
                                size_t before,
                                const wxString& text,
                                int image = -1, int selectedImage = -1,
                                wxTreeItemData *data = NULL);
    wxTreeItemId AppendItem(const wxTreeItemId& parent,
                            const wxString& text,
                            int image = -1, int selectedImage = -1,
                            wxPyTreeItemData *data = NULL);

    void Delete(const wxTreeItemId& item);
    void DeleteChildren(const wxTreeItemId& item);
    void DeleteAllItems();

    void Expand(const wxTreeItemId& item);
    void Collapse(const wxTreeItemId& item);
    void CollapseAndReset(const wxTreeItemId& item);
    void Toggle(const wxTreeItemId& item);

    void Unselect();
    void UnselectAll();
    void SelectItem(const wxTreeItemId& item);
    void EnsureVisible(const wxTreeItemId& item);
    void ScrollTo(const wxTreeItemId& item);
#ifdef __WXMSW__
    wxTextCtrl* EditLabel(const wxTreeItemId& item);
    wxTextCtrl* GetEditControl();
    void EndEditLabel(const wxTreeItemId& item, int discardChanges = FALSE);
#else
    void EditLabel(const wxTreeItemId& item);
#endif

    void SortChildren(const wxTreeItemId& item);

    void SetItemBold(const wxTreeItemId& item, int bold = TRUE);
    bool IsBold(const wxTreeItemId& item) const;
    wxTreeItemId HitTest(const wxPoint& point, int& OUTPUT);



    void SetItemTextColour(const wxTreeItemId& item, const wxColour& col);
    void SetItemBackgroundColour(const wxTreeItemId& item, const wxColour& col);
    void SetItemFont(const wxTreeItemId& item, const wxFont& font);

#ifdef __WXMSW__
    void SetItemDropHighlight(const wxTreeItemId& item, int highlight = TRUE);

    //bool GetBoundingRect(const wxTreeItemId& item, wxRect& rect, int textOnly = FALSE)
    %addmethods {
        PyObject* GetBoundingRect(const wxTreeItemId& item, int textOnly = FALSE) {
            wxRect rect;
            if (self->GetBoundingRect(item, rect, textOnly)) {
                bool doSave = wxPyRestoreThread();
                wxRect* r = new wxRect(rect);
                PyObject* val = wxPyConstructObject((void*)r, "wxRect");
                wxPySaveThread(doSave);
                return val;
            }
            else {
                Py_INCREF(Py_None);
                return Py_None;
            }
        }
    }
#endif

%pragma(python) addtoclass = "
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
"
};


//----------------------------------------------------------------------

#ifdef SKIPTHIS
#ifdef __WXMSW__


enum {
     /* tab control event types */
    wxEVT_COMMAND_TAB_SEL_CHANGED,
    wxEVT_COMMAND_TAB_SEL_CHANGING,
};


class wxTabEvent : public wxCommandEvent {
public:
};



class wxTabCtrl : public wxControl {
public:
    wxTabCtrl(wxWindow* parent, wxWindowID id,
              const wxPoint& pos = wxDefaultPosition,
              const wxSize& size = wxDefaultSize,
              long style = 0,
              char* name = "tabCtrl");

    %pragma(python) addtomethod = "__init__:#wx._StdWindowCallbacks(self)"

    bool DeleteAllItems();
    bool DeleteItem(int item);
    wxImageList* GetImageList();
    int GetItemCount();
    // TODO: void* GetItemData();
    int GetItemImage(int item);

    %addmethods {
        %new wxRect* GetItemRect(int item) {
            wxRect* rect = new wxRect;
            self->GetItemRect(item, *rect);
            return rect;
        }
    }

    wxString GetItemText(int item);
    bool GetRowCount();
    int GetSelection();
    int HitTest(const wxPoint& pt, long& OUTPUT);
    void InsertItem(int item, const wxString& text,
                    int imageId = -1, void* clientData = NULL);
    // TODO: bool SetItemData(int item, void* data);
    bool SetItemImage(int item, int image);
    void SetImageList(wxImageList* imageList);
    void SetItemSize(const wxSize& size);
    bool SetItemText(int item, const wxString& text);
    void SetPadding(const wxSize& padding);
    int SetSelection(int item);

};

#endif
#endif

//----------------------------------------------------------------------

%init %{
    wxPyPtrTypeMap_Add("wxTreeItemData", "wxPyTreeItemData");
    wxPyPtrTypeMap_Add("wxTreeCtrl", "wxPyTreeCtrl");
%}

//----------------------------------------------------------------------


