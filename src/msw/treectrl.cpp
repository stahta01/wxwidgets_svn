/////////////////////////////////////////////////////////////////////////////
// Name:        treectrl.cpp
// Purpose:     wxTreeCtrl
// Author:      Julian Smart
// Modified by: Vadim Zeitlin to be less MSW-specific on 10.10.98
// Created:     1997
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------
#ifdef __GNUG__
    #pragma implementation "treectrl.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/msw/private.h"

// Mingw32 is a bit mental even though this is done in winundef
#ifdef GetFirstChild
    #undef GetFirstChild
#endif

#ifdef GetNextSibling
    #undef GetNextSibling
#endif

#if defined(__WIN95__)

#include "wx/log.h"
#include "wx/dynarray.h"
#include "wx/imaglist.h"
#include "wx/treectrl.h"
#include "wx/settings.h"

#include "wx/msw/dragimag.h"

#ifdef __GNUWIN32_OLD__
    #include "wx/msw/gnuwin32/extra.h"
#endif

#if defined(__WIN95__) && !(defined(__GNUWIN32_OLD__) || defined(__TWIN32__))
    #include <commctrl.h>
#endif

// Bug in headers, sometimes
#ifndef TVIS_FOCUSED
    #define TVIS_FOCUSED            0x0001
#endif

#ifndef TV_FIRST
    #define TV_FIRST                0x1100
#endif

// old headers might miss these messages (comctl32.dll 4.71+ only)
#ifndef TVM_SETBKCOLOR
    #define TVM_SETBKCOLOR          (TV_FIRST + 29)
    #define TVM_SETTEXTCOLOR        (TV_FIRST + 30)
#endif

// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

// a convenient wrapper around TV_ITEM struct which adds a ctor
#ifdef __VISUALC__
#pragma warning( disable : 4097 )
#endif

struct wxTreeViewItem : public TV_ITEM
{
    wxTreeViewItem(const wxTreeItemId& item,    // the item handle
                   UINT mask_,                  // fields which are valid
                   UINT stateMask_ = 0)         // for TVIF_STATE only
    {
        // hItem member is always valid
        mask = mask_ | TVIF_HANDLE;
        stateMask = stateMask_;
        hItem = (HTREEITEM) (WXHTREEITEM) item;
    }
};

#ifdef __VISUALC__
#pragma warning( default : 4097 )
#endif

// a class which encapsulates the tree traversal logic: it vists all (unless
// OnVisit() returns FALSE) items under the given one
class wxTreeTraversal
{
public:
    wxTreeTraversal(const wxTreeCtrl *tree)
    {
        m_tree = tree;
    }

    // do traverse the tree: visit all items (recursively by default) under the
    // given one; return TRUE if all items were traversed or FALSE if the
    // traversal was aborted because OnVisit returned FALSE
    bool DoTraverse(const wxTreeItemId& root, bool recursively = TRUE);

    // override this function to do whatever is needed for each item, return
    // FALSE to stop traversing
    virtual bool OnVisit(const wxTreeItemId& item) = 0;

protected:
    const wxTreeCtrl *GetTree() const { return m_tree; }

private:
    bool Traverse(const wxTreeItemId& root, bool recursively);

    const wxTreeCtrl *m_tree;
};

// internal class for getting the selected items
class TraverseSelections : public wxTreeTraversal
{
public:
    TraverseSelections(const wxTreeCtrl *tree,
                       wxArrayTreeItemIds& selections)
        : wxTreeTraversal(tree), m_selections(selections)
        {
            m_selections.Empty();

            DoTraverse(tree->GetRootItem());
        }

    virtual bool OnVisit(const wxTreeItemId& item)
    {
        if ( GetTree()->IsItemChecked(item) )
        {
            m_selections.Add(item);
        }

        return TRUE;
    }

    size_t GetCount() const { return m_selections.GetCount(); }

private:
    wxArrayTreeItemIds& m_selections;
};

// internal class for counting tree items
class TraverseCounter : public wxTreeTraversal
{
public:
    TraverseCounter(const wxTreeCtrl *tree,
                    const wxTreeItemId& root,
                    bool recursively)
        : wxTreeTraversal(tree)
        {
            m_count = 0;

            DoTraverse(root, recursively);
        }

    virtual bool OnVisit(const wxTreeItemId& item)
    {
        m_count++;

        return TRUE;
    }

    size_t GetCount() const { return m_count; }

private:
    size_t m_count;
};

// ----------------------------------------------------------------------------
// This class is needed for support of different images: the Win32 common
// control natively supports only 2 images (the normal one and another for the
// selected state). We wish to provide support for 2 more of them for folder
// items (i.e. those which have children): for expanded state and for expanded
// selected state. For this we use this structure to store the additional items
// images.
//
// There is only one problem with this: when we retrieve the item's data, we
// don't know whether we get a pointer to wxTreeItemData or
// wxTreeItemIndirectData. So we have to maintain a list of all items which
// have indirect data inside the listctrl itself.
// ----------------------------------------------------------------------------

class wxTreeItemIndirectData
{
public:
    // ctor associates this data with the item and the real item data becomes
    // available through our GetData() method
    wxTreeItemIndirectData(wxTreeCtrl *tree, const wxTreeItemId& item)
    {
        for ( size_t n = 0; n < WXSIZEOF(m_images); n++ )
        {
            m_images[n] = -1;
        }

        // save the old data
        m_data = tree->GetItemData(item);

        // and set ourselves as the new one
        tree->SetIndirectItemData(item, this);
    }

    // dtor deletes the associated data as well
    ~wxTreeItemIndirectData() { delete m_data; }

    // accessors
        // get the real data associated with the item
    wxTreeItemData *GetData() const { return m_data; }
        // change it
    void SetData(wxTreeItemData *data) { m_data = data; }

        // do we have such image?
    bool HasImage(wxTreeItemIcon which) const { return m_images[which] != -1; }
        // get image
    int GetImage(wxTreeItemIcon which) const { return m_images[which]; }
        // change it
    void SetImage(int image, wxTreeItemIcon which) { m_images[which] = image; }

private:
    // all the images associated with the item
    int m_images[wxTreeItemIcon_Max];

    wxTreeItemData *m_data;
};

// ----------------------------------------------------------------------------
// private functions
// ----------------------------------------------------------------------------

static HTREEITEM GetItemFromPoint(HWND hwndTV, int x, int y)
{
    TVHITTESTINFO tvht;
    tvht.pt.x = x;
    tvht.pt.y = y;

    return TreeView_HitTest(hwndTV, &tvht);
}

// ----------------------------------------------------------------------------
// macros
// ----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxTreeCtrl, wxControl)

// ----------------------------------------------------------------------------
// variables
// ----------------------------------------------------------------------------

// handy table for sending events
static const wxEventType g_events[2][2] =
{
    { wxEVT_COMMAND_TREE_ITEM_COLLAPSED, wxEVT_COMMAND_TREE_ITEM_COLLAPSING },
    { wxEVT_COMMAND_TREE_ITEM_EXPANDED,  wxEVT_COMMAND_TREE_ITEM_EXPANDING  }
};

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// tree traversal
// ----------------------------------------------------------------------------

bool wxTreeTraversal::DoTraverse(const wxTreeItemId& root, bool recursively)
{
    if ( !OnVisit(root) )
        return FALSE;

    return Traverse(root, recursively);
}

bool wxTreeTraversal::Traverse(const wxTreeItemId& root, bool recursively)
{
    long cookie;
    wxTreeItemId child = m_tree->GetFirstChild(root, cookie);
    while ( child.IsOk() )
    {
        // depth first traversal
        if ( recursively && !Traverse(child, TRUE) )
            return FALSE;

        if ( !OnVisit(child) )
            return FALSE;

        child = m_tree->GetNextChild(root, cookie);
    }

    return TRUE;
}

// ----------------------------------------------------------------------------
// construction and destruction
// ----------------------------------------------------------------------------

void wxTreeCtrl::Init()
{
    m_imageListNormal = NULL;
    m_imageListState = NULL;
    m_textCtrl = NULL;
    m_hasAnyAttr = FALSE;
    m_dragImage = NULL;
}

bool wxTreeCtrl::Create(wxWindow *parent,
                        wxWindowID id,
                        const wxPoint& pos,
                        const wxSize& size,
                        long style,
                        const wxValidator& validator,
                        const wxString& name)
{
    Init();

    if ( !CreateControl(parent, id, pos, size, style, validator, name) )
        return FALSE;

    DWORD wstyle = WS_VISIBLE | WS_CHILD | WS_TABSTOP |
                   TVS_HASLINES | TVS_SHOWSELALWAYS;

    if ( m_windowStyle & wxTR_HAS_BUTTONS )
        wstyle |= TVS_HASBUTTONS;

    if ( m_windowStyle & wxTR_EDIT_LABELS )
        wstyle |= TVS_EDITLABELS;

    if ( m_windowStyle & wxTR_LINES_AT_ROOT )
        wstyle |= TVS_LINESATROOT;

#if !defined( __GNUWIN32_OLD__ ) && \
    !defined( __BORLANDC__ ) && \
    !defined( __WATCOMC__ ) && \
    (!defined(__VISUALC__) || (__VISUALC__ > 1010))
    // we emulate the multiple selection tree controls by using checkboxes: set
    // up the image list we need for this if we do have multiple selections
    if ( m_windowStyle & wxTR_MULTIPLE )
        wstyle |= TVS_CHECKBOXES;
#endif

    // Create the tree control.
    if ( !MSWCreateControl(WC_TREEVIEW, wstyle) )
        return FALSE;

    SetBackgroundColour(wxSystemSettings::GetSystemColour(wxSYS_COLOUR_WINDOW));
    SetForegroundColour(wxWindow::GetParent()->GetForegroundColour());

    // VZ: this is some experimental code which may be used to get the
    //     TVS_CHECKBOXES style functionality for comctl32.dll < 4.71.
    //     AFAIK, the standard DLL does about the same thing anyhow.
#if 0
    if ( m_windowStyle & wxTR_MULTIPLE )
    {
        wxBitmap bmp;

        // create the DC compatible with the current screen
        HDC hdcMem = CreateCompatibleDC(NULL);

        // create a mono bitmap of the standard size
        int x = GetSystemMetrics(SM_CXMENUCHECK);
        int y = GetSystemMetrics(SM_CYMENUCHECK);
        wxImageList imagelistCheckboxes(x, y, FALSE, 2);
        HBITMAP hbmpCheck = CreateBitmap(x, y,   // bitmap size
                                         1,      // # of color planes
                                         1,      // # bits needed for one pixel
                                         0);     // array containing colour data
        SelectObject(hdcMem, hbmpCheck);

        // then draw a check mark into it
        RECT rect = { 0, 0, x, y };
        if ( !::DrawFrameControl(hdcMem, &rect,
                                 DFC_BUTTON,
                                 DFCS_BUTTONCHECK | DFCS_CHECKED) )
        {
            wxLogLastError(wxT("DrawFrameControl(check)"));
        }

        bmp.SetHBITMAP((WXHBITMAP)hbmpCheck);
        imagelistCheckboxes.Add(bmp);

        if ( !::DrawFrameControl(hdcMem, &rect,
                                 DFC_BUTTON,
                                 DFCS_BUTTONCHECK) )
        {
            wxLogLastError(wxT("DrawFrameControl(uncheck)"));
        }

        bmp.SetHBITMAP((WXHBITMAP)hbmpCheck);
        imagelistCheckboxes.Add(bmp);

        // clean up
        ::DeleteDC(hdcMem);

        // set the imagelist
        SetStateImageList(&imagelistCheckboxes);
    }
#endif // 0

    SetSize(pos.x, pos.y, size.x, size.y);

    return TRUE;
}

wxTreeCtrl::~wxTreeCtrl()
{
    // delete any attributes
    if ( m_hasAnyAttr )
    {
        for ( wxNode *node = m_attrs.Next(); node; node = m_attrs.Next() )
        {
            delete (wxTreeItemAttr *)node->Data();
        }

        // prevent TVN_DELETEITEM handler from deleting the attributes again!
        m_hasAnyAttr = FALSE;
    }

    DeleteTextCtrl();

    // delete user data to prevent memory leaks
    DeleteAllItems();
}

// ----------------------------------------------------------------------------
// accessors
// ----------------------------------------------------------------------------

// simple wrappers which add error checking in debug mode

bool wxTreeCtrl::DoGetItem(wxTreeViewItem* tvItem) const
{
    if ( !TreeView_GetItem(GetHwnd(), tvItem) )
    {
        wxLogLastError("TreeView_GetItem");

        return FALSE;
    }

    return TRUE;
}

void wxTreeCtrl::DoSetItem(wxTreeViewItem* tvItem)
{
    if ( TreeView_SetItem(GetHwnd(), tvItem) == -1 )
    {
        wxLogLastError("TreeView_SetItem");
    }
}

size_t wxTreeCtrl::GetCount() const
{
    return (size_t)TreeView_GetCount(GetHwnd());
}

unsigned int wxTreeCtrl::GetIndent() const
{
    return TreeView_GetIndent(GetHwnd());
}

void wxTreeCtrl::SetIndent(unsigned int indent)
{
    TreeView_SetIndent(GetHwnd(), indent);
}

wxImageList *wxTreeCtrl::GetImageList() const
{
    return m_imageListNormal;
}

wxImageList *wxTreeCtrl::GetStateImageList() const
{
    return m_imageListNormal;
}

void wxTreeCtrl::SetAnyImageList(wxImageList *imageList, int which)
{
    // no error return
    TreeView_SetImageList(GetHwnd(),
                          imageList ? imageList->GetHIMAGELIST() : 0,
                          which);
}

void wxTreeCtrl::SetImageList(wxImageList *imageList)
{
    SetAnyImageList(m_imageListNormal = imageList, TVSIL_NORMAL);
}

void wxTreeCtrl::SetStateImageList(wxImageList *imageList)
{
    SetAnyImageList(m_imageListState = imageList, TVSIL_STATE);
}

size_t wxTreeCtrl::GetChildrenCount(const wxTreeItemId& item,
                                    bool recursively) const
{
    TraverseCounter counter(this, item, recursively);

    return counter.GetCount() - 1;
}

// ----------------------------------------------------------------------------
// control colours
// ----------------------------------------------------------------------------

bool wxTreeCtrl::SetBackgroundColour(const wxColour &colour)
{
    if ( !wxWindowBase::SetBackgroundColour(colour) )
        return FALSE;

    SendMessage(GetHwnd(), TVM_SETBKCOLOR, 0, colour.GetPixel());

    return TRUE;
}

bool wxTreeCtrl::SetForegroundColour(const wxColour &colour)
{
    if ( !wxWindowBase::SetForegroundColour(colour) )
        return FALSE;

    SendMessage(GetHwnd(), TVM_SETTEXTCOLOR, 0, colour.GetPixel());

    return TRUE;
}

// ----------------------------------------------------------------------------
// Item access
// ----------------------------------------------------------------------------

wxString wxTreeCtrl::GetItemText(const wxTreeItemId& item) const
{
    wxChar buf[512];  // the size is arbitrary...

    wxTreeViewItem tvItem(item, TVIF_TEXT);
    tvItem.pszText = buf;
    tvItem.cchTextMax = WXSIZEOF(buf);
    if ( !DoGetItem(&tvItem) )
    {
        // don't return some garbage which was on stack, but an empty string
        buf[0] = wxT('\0');
    }

    return wxString(buf);
}

void wxTreeCtrl::SetItemText(const wxTreeItemId& item, const wxString& text)
{
    wxTreeViewItem tvItem(item, TVIF_TEXT);
    tvItem.pszText = (wxChar *)text.c_str();  // conversion is ok
    DoSetItem(&tvItem);
}

int wxTreeCtrl::DoGetItemImageFromData(const wxTreeItemId& item,
                                       wxTreeItemIcon which) const
{
    wxTreeViewItem tvItem(item, TVIF_PARAM);
    if ( !DoGetItem(&tvItem) )
    {
        return -1;
    }

    return ((wxTreeItemIndirectData *)tvItem.lParam)->GetImage(which);
}

void wxTreeCtrl::DoSetItemImageFromData(const wxTreeItemId& item,
                                        int image,
                                        wxTreeItemIcon which) const
{
    wxTreeViewItem tvItem(item, TVIF_PARAM);
    if ( !DoGetItem(&tvItem) )
    {
        return;
    }

    wxTreeItemIndirectData *data = ((wxTreeItemIndirectData *)tvItem.lParam);

    data->SetImage(image, which);

    // make sure that we have selected images as well
    if ( which == wxTreeItemIcon_Normal &&
         !data->HasImage(wxTreeItemIcon_Selected) )
    {
        data->SetImage(image, wxTreeItemIcon_Selected);
    }

    if ( which == wxTreeItemIcon_Expanded &&
         !data->HasImage(wxTreeItemIcon_SelectedExpanded) )
    {
        data->SetImage(image, wxTreeItemIcon_SelectedExpanded);
    }
}

void wxTreeCtrl::DoSetItemImages(const wxTreeItemId& item,
                                 int image,
                                 int imageSel)
{
    wxTreeViewItem tvItem(item, TVIF_IMAGE | TVIF_SELECTEDIMAGE);
    tvItem.iSelectedImage = imageSel;
    tvItem.iImage = image;
    DoSetItem(&tvItem);
}

int wxTreeCtrl::GetItemImage(const wxTreeItemId& item,
                             wxTreeItemIcon which) const
{
    if ( HasIndirectData(item) )
    {
        return DoGetItemImageFromData(item, which);
    }

    UINT mask;
    switch ( which )
    {
        default:
            wxFAIL_MSG( wxT("unknown tree item image type") );

        case wxTreeItemIcon_Normal:
            mask = TVIF_IMAGE;
            break;

        case wxTreeItemIcon_Selected:
            mask = TVIF_SELECTEDIMAGE;
            break;

        case wxTreeItemIcon_Expanded:
        case wxTreeItemIcon_SelectedExpanded:
            return -1;
    }

    wxTreeViewItem tvItem(item, mask);
    DoGetItem(&tvItem);

    return mask == TVIF_IMAGE ? tvItem.iImage : tvItem.iSelectedImage;
}

void wxTreeCtrl::SetItemImage(const wxTreeItemId& item, int image,
                              wxTreeItemIcon which)
{
    int imageNormal, imageSel;
    switch ( which )
    {
        default:
            wxFAIL_MSG( wxT("unknown tree item image type") );

        case wxTreeItemIcon_Normal:
            imageNormal = image;
            imageSel = GetItemSelectedImage(item);
            break;

        case wxTreeItemIcon_Selected:
            imageNormal = GetItemImage(item);
            imageSel = image;
            break;

        case wxTreeItemIcon_Expanded:
        case wxTreeItemIcon_SelectedExpanded:
            if ( !HasIndirectData(item) )
            {
                // we need to get the old images first, because after we create
                // the wxTreeItemIndirectData GetItemXXXImage() will use it to
                // get the images
                imageNormal = GetItemImage(item);
                imageSel = GetItemSelectedImage(item);

                // if it doesn't have it yet, add it
                wxTreeItemIndirectData *data = new
                    wxTreeItemIndirectData(this, item);

                // copy the data to the new location
                data->SetImage(imageNormal, wxTreeItemIcon_Normal);
                data->SetImage(imageSel, wxTreeItemIcon_Selected);
            }

            DoSetItemImageFromData(item, image, which);

            // reset the normal/selected images because we won't use them any
            // more - now they're stored inside the indirect data
            imageNormal =
            imageSel = I_IMAGECALLBACK;
            break;
    }

    // NB: at least in version 5.00.0518.9 of comctl32.dll we need to always
    //     change both normal and selected image - otherwise the change simply
    //     doesn't take place!
    DoSetItemImages(item, imageNormal, imageSel);
}

wxTreeItemData *wxTreeCtrl::GetItemData(const wxTreeItemId& item) const
{
    wxTreeViewItem tvItem(item, TVIF_PARAM);
    if ( !DoGetItem(&tvItem) )
    {
        return NULL;
    }

    if ( HasIndirectData(item) )
    {
        return ((wxTreeItemIndirectData *)tvItem.lParam)->GetData();
    }
    else
    {
        return (wxTreeItemData *)tvItem.lParam;
    }
}

void wxTreeCtrl::SetItemData(const wxTreeItemId& item, wxTreeItemData *data)
{
    wxTreeViewItem tvItem(item, TVIF_PARAM);

    if ( HasIndirectData(item) )
    {
        if ( DoGetItem(&tvItem) )
        {
            ((wxTreeItemIndirectData *)tvItem.lParam)->SetData(data);
        }
        else
        {
            wxFAIL_MSG( wxT("failed to change tree items data") );
        }
    }
    else
    {
        tvItem.lParam = (LPARAM)data;
        DoSetItem(&tvItem);
    }
}

void wxTreeCtrl::SetIndirectItemData(const wxTreeItemId& item,
                                     wxTreeItemIndirectData *data)
{
    // this should never happen because it's unnecessary and will probably lead
    // to crash too because the code elsewhere supposes that the pointer the
    // wxTreeItemIndirectData has is a real wxItemData and not
    // wxTreeItemIndirectData as well
    wxASSERT_MSG( !HasIndirectData(item), wxT("setting indirect data twice?") );

    SetItemData(item, (wxTreeItemData *)data);

    m_itemsWithIndirectData.Add(item);
}

bool wxTreeCtrl::HasIndirectData(const wxTreeItemId& item) const
{
    return m_itemsWithIndirectData.Index(item) != wxNOT_FOUND;
}

void wxTreeCtrl::SetItemHasChildren(const wxTreeItemId& item, bool has)
{
    wxTreeViewItem tvItem(item, TVIF_CHILDREN);
    tvItem.cChildren = (int)has;
    DoSetItem(&tvItem);
}

void wxTreeCtrl::SetItemBold(const wxTreeItemId& item, bool bold)
{
    wxTreeViewItem tvItem(item, TVIF_STATE, TVIS_BOLD);
    tvItem.state = bold ? TVIS_BOLD : 0;
    DoSetItem(&tvItem);
}

void wxTreeCtrl::SetItemDropHighlight(const wxTreeItemId& item, bool highlight)
{
    wxTreeViewItem tvItem(item, TVIF_STATE, TVIS_DROPHILITED);
    tvItem.state = highlight ? TVIS_DROPHILITED : 0;
    DoSetItem(&tvItem);
}

void wxTreeCtrl::SetItemTextColour(const wxTreeItemId& item,
                                   const wxColour& col)
{
    m_hasAnyAttr = TRUE;

    long id = (long)(WXHTREEITEM)item;
    wxTreeItemAttr *attr = (wxTreeItemAttr *)m_attrs.Get(id);
    if ( !attr )
    {
        attr = new wxTreeItemAttr;
        m_attrs.Put(id, (wxObject *)attr);
    }

    attr->SetTextColour(col);
}

void wxTreeCtrl::SetItemBackgroundColour(const wxTreeItemId& item,
                                         const wxColour& col)
{
    m_hasAnyAttr = TRUE;

    long id = (long)(WXHTREEITEM)item;
    wxTreeItemAttr *attr = (wxTreeItemAttr *)m_attrs.Get(id);
    if ( !attr )
    {
        attr = new wxTreeItemAttr;
        m_attrs.Put(id, (wxObject *)attr);
    }

    attr->SetBackgroundColour(col);
}

void wxTreeCtrl::SetItemFont(const wxTreeItemId& item, const wxFont& font)
{
    m_hasAnyAttr = TRUE;

    long id = (long)(WXHTREEITEM)item;
    wxTreeItemAttr *attr = (wxTreeItemAttr *)m_attrs.Get(id);
    if ( !attr )
    {
        attr = new wxTreeItemAttr;
        m_attrs.Put(id, (wxObject *)attr);
    }

    attr->SetFont(font);
}

// ----------------------------------------------------------------------------
// Item status
// ----------------------------------------------------------------------------

bool wxTreeCtrl::IsVisible(const wxTreeItemId& item) const
{
    // Bug in Gnu-Win32 headers, so don't use the macro TreeView_GetItemRect
    RECT rect;

    // this ugliness comes directly from MSDN - it *is* the correct way to pass
    // the HTREEITEM with TVM_GETITEMRECT
    *(WXHTREEITEM *)&rect = (WXHTREEITEM)item;

    // FALSE means get item rect for the whole item, not only text
    return SendMessage(GetHwnd(), TVM_GETITEMRECT, FALSE, (LPARAM)&rect) != 0;

}

bool wxTreeCtrl::ItemHasChildren(const wxTreeItemId& item) const
{
    wxTreeViewItem tvItem(item, TVIF_CHILDREN);
    DoGetItem(&tvItem);

    return tvItem.cChildren != 0;
}

bool wxTreeCtrl::IsExpanded(const wxTreeItemId& item) const
{
    // probably not a good idea to put it here
    //wxASSERT( ItemHasChildren(item) );

    wxTreeViewItem tvItem(item, TVIF_STATE, TVIS_EXPANDED);
    DoGetItem(&tvItem);

    return (tvItem.state & TVIS_EXPANDED) != 0;
}

bool wxTreeCtrl::IsSelected(const wxTreeItemId& item) const
{
    wxTreeViewItem tvItem(item, TVIF_STATE, TVIS_SELECTED);
    DoGetItem(&tvItem);

    return (tvItem.state & TVIS_SELECTED) != 0;
}

bool wxTreeCtrl::IsBold(const wxTreeItemId& item) const
{
    wxTreeViewItem tvItem(item, TVIF_STATE, TVIS_BOLD);
    DoGetItem(&tvItem);

    return (tvItem.state & TVIS_BOLD) != 0;
}

// ----------------------------------------------------------------------------
// navigation
// ----------------------------------------------------------------------------

wxTreeItemId wxTreeCtrl::GetRootItem() const
{
    return wxTreeItemId((WXHTREEITEM) TreeView_GetRoot(GetHwnd()));
}

wxTreeItemId wxTreeCtrl::GetSelection() const
{
    wxCHECK_MSG( !(m_windowStyle & wxTR_MULTIPLE), (WXHTREEITEM)0,
                 wxT("this only works with single selection controls") );

    return wxTreeItemId((WXHTREEITEM) TreeView_GetSelection(GetHwnd()));
}

wxTreeItemId wxTreeCtrl::GetParent(const wxTreeItemId& item) const
{
    return wxTreeItemId((WXHTREEITEM) TreeView_GetParent(GetHwnd(), (HTREEITEM) (WXHTREEITEM) item));
}

wxTreeItemId wxTreeCtrl::GetFirstChild(const wxTreeItemId& item,
                                       long& _cookie) const
{
    // remember the last child returned in 'cookie'
    _cookie = (long)TreeView_GetChild(GetHwnd(), (HTREEITEM) (WXHTREEITEM)item);

    return wxTreeItemId((WXHTREEITEM)_cookie);
}

wxTreeItemId wxTreeCtrl::GetNextChild(const wxTreeItemId& WXUNUSED(item),
                                      long& _cookie) const
{
    wxTreeItemId l = wxTreeItemId((WXHTREEITEM)TreeView_GetNextSibling(GetHwnd(),
                                   (HTREEITEM)(WXHTREEITEM)_cookie));
    _cookie = (long)l;

    return l;
}

wxTreeItemId wxTreeCtrl::GetLastChild(const wxTreeItemId& item) const
{
    // can this be done more efficiently?
    long cookie;

    wxTreeItemId childLast,
    child = GetFirstChild(item, cookie);
    while ( child.IsOk() )
    {
        childLast = child;
        child = GetNextChild(item, cookie);
    }

    return childLast;
}

wxTreeItemId wxTreeCtrl::GetNextSibling(const wxTreeItemId& item) const
{
    return wxTreeItemId((WXHTREEITEM) TreeView_GetNextSibling(GetHwnd(), (HTREEITEM) (WXHTREEITEM) item));
}

wxTreeItemId wxTreeCtrl::GetPrevSibling(const wxTreeItemId& item) const
{
    return wxTreeItemId((WXHTREEITEM) TreeView_GetPrevSibling(GetHwnd(), (HTREEITEM) (WXHTREEITEM) item));
}

wxTreeItemId wxTreeCtrl::GetFirstVisibleItem() const
{
    return wxTreeItemId((WXHTREEITEM) TreeView_GetFirstVisible(GetHwnd()));
}

wxTreeItemId wxTreeCtrl::GetNextVisible(const wxTreeItemId& item) const
{
    wxASSERT_MSG( IsVisible(item), wxT("The item you call GetNextVisible() "
                                      "for must be visible itself!"));

    return wxTreeItemId((WXHTREEITEM) TreeView_GetNextVisible(GetHwnd(), (HTREEITEM) (WXHTREEITEM) item));
}

wxTreeItemId wxTreeCtrl::GetPrevVisible(const wxTreeItemId& item) const
{
    wxASSERT_MSG( IsVisible(item), wxT("The item you call GetPrevVisible() "
                                      "for must be visible itself!"));

    return wxTreeItemId((WXHTREEITEM) TreeView_GetPrevVisible(GetHwnd(), (HTREEITEM) (WXHTREEITEM) item));
}

// ----------------------------------------------------------------------------
// multiple selections emulation
// ----------------------------------------------------------------------------

bool wxTreeCtrl::IsItemChecked(const wxTreeItemId& item) const
{
    // receive the desired information.
    wxTreeViewItem tvItem(item, TVIF_STATE, TVIS_STATEIMAGEMASK);
    DoGetItem(&tvItem);

    // state image indices are 1 based
    return ((tvItem.state >> 12) - 1) == 1;
}

void wxTreeCtrl::SetItemCheck(const wxTreeItemId& item, bool check)
{
    // receive the desired information.
    wxTreeViewItem tvItem(item, TVIF_STATE, TVIS_STATEIMAGEMASK);

    // state images are one-based
    tvItem.state = (check ? 2 : 1) << 12;

    DoSetItem(&tvItem);
}

size_t wxTreeCtrl::GetSelections(wxArrayTreeItemIds& selections) const
{
    TraverseSelections selector(this, selections);

    return selector.GetCount();
}

// ----------------------------------------------------------------------------
// Usual operations
// ----------------------------------------------------------------------------

wxTreeItemId wxTreeCtrl::DoInsertItem(const wxTreeItemId& parent,
                                      wxTreeItemId hInsertAfter,
                                      const wxString& text,
                                      int image, int selectedImage,
                                      wxTreeItemData *data)
{
    TV_INSERTSTRUCT tvIns;
    tvIns.hParent = (HTREEITEM) (WXHTREEITEM)parent;
    tvIns.hInsertAfter = (HTREEITEM) (WXHTREEITEM) hInsertAfter;

    // this is how we insert the item as the first child: supply a NULL
    // hInsertAfter
    if ( !tvIns.hInsertAfter )
    {
        tvIns.hInsertAfter = TVI_FIRST;
    }

    UINT mask = 0;
    if ( !text.IsEmpty() )
    {
        mask |= TVIF_TEXT;
        tvIns.item.pszText = (wxChar *)text.c_str();  // cast is ok
    }

    if ( image != -1 )
    {
        mask |= TVIF_IMAGE;
        tvIns.item.iImage = image;

        if ( selectedImage == -1 )
        {
            // take the same image for selected icon if not specified
            selectedImage = image;
        }
    }

    if ( selectedImage != -1 )
    {
        mask |= TVIF_SELECTEDIMAGE;
        tvIns.item.iSelectedImage = selectedImage;
    }

    if ( data != NULL )
    {
        mask |= TVIF_PARAM;
        tvIns.item.lParam = (LPARAM)data;
    }

    tvIns.item.mask = mask;

    HTREEITEM id = (HTREEITEM) TreeView_InsertItem(GetHwnd(), &tvIns);
    if ( id == 0 )
    {
        wxLogLastError("TreeView_InsertItem");
    }

    if ( data != NULL )
    {
        // associate the application tree item with Win32 tree item handle
        data->SetId((WXHTREEITEM)id);
    }

    return wxTreeItemId((WXHTREEITEM)id);
}

// for compatibility only
wxTreeItemId wxTreeCtrl::InsertItem(const wxTreeItemId& parent,
                                    const wxString& text,
                                    int image, int selImage,
                                    long insertAfter)
{
    return DoInsertItem(parent, (WXHTREEITEM)insertAfter, text,
                        image, selImage, NULL);
}

wxTreeItemId wxTreeCtrl::AddRoot(const wxString& text,
                                 int image, int selectedImage,
                                 wxTreeItemData *data)
{
    return DoInsertItem(wxTreeItemId((WXHTREEITEM) 0), (WXHTREEITEM) 0,
                        text, image, selectedImage, data);
}

wxTreeItemId wxTreeCtrl::PrependItem(const wxTreeItemId& parent,
                                     const wxString& text,
                                     int image, int selectedImage,
                                     wxTreeItemData *data)
{
    return DoInsertItem(parent, (WXHTREEITEM) TVI_FIRST,
                        text, image, selectedImage, data);
}

wxTreeItemId wxTreeCtrl::InsertItem(const wxTreeItemId& parent,
                                    const wxTreeItemId& idPrevious,
                                    const wxString& text,
                                    int image, int selectedImage,
                                    wxTreeItemData *data)
{
    return DoInsertItem(parent, idPrevious, text, image, selectedImage, data);
}

wxTreeItemId wxTreeCtrl::InsertItem(const wxTreeItemId& parent,
                                    size_t index,
                                    const wxString& text,
                                    int image, int selectedImage,
                                    wxTreeItemData *data)
{
    // find the item from index
    long cookie;
    wxTreeItemId idPrev, idCur = GetFirstChild(parent, cookie);
    while ( index != 0 && idCur.IsOk() )
    {
        index--;

        idPrev = idCur;
        idCur = GetNextChild(parent, cookie);
    }

    // assert, not check: if the index is invalid, we will append the item
    // to the end
    wxASSERT_MSG( index == 0, _T("bad index in wxTreeCtrl::InsertItem") );

    return DoInsertItem(parent, idPrev, text, image, selectedImage, data);
}

wxTreeItemId wxTreeCtrl::AppendItem(const wxTreeItemId& parent,
                                    const wxString& text,
                                    int image, int selectedImage,
                                    wxTreeItemData *data)
{
    return DoInsertItem(parent, (WXHTREEITEM) TVI_LAST,
                        text, image, selectedImage, data);
}

void wxTreeCtrl::Delete(const wxTreeItemId& item)
{
    if ( !TreeView_DeleteItem(GetHwnd(), (HTREEITEM)(WXHTREEITEM)item) )
    {
        wxLogLastError("TreeView_DeleteItem");
    }
}

// delete all children (but don't delete the item itself)
void wxTreeCtrl::DeleteChildren(const wxTreeItemId& item)
{
    long cookie;

    wxArrayLong children;
    wxTreeItemId child = GetFirstChild(item, cookie);
    while ( child.IsOk() )
    {
        children.Add((long)(WXHTREEITEM)child);

        child = GetNextChild(item, cookie);
    }

    size_t nCount = children.Count();
    for ( size_t n = 0; n < nCount; n++ )
    {
        if ( !TreeView_DeleteItem(GetHwnd(), (HTREEITEM)children[n]) )
        {
            wxLogLastError("TreeView_DeleteItem");
        }
    }
}

void wxTreeCtrl::DeleteAllItems()
{
    if ( !TreeView_DeleteAllItems(GetHwnd()) )
    {
        wxLogLastError("TreeView_DeleteAllItems");
    }
}

void wxTreeCtrl::DoExpand(const wxTreeItemId& item, int flag)
{
    wxASSERT_MSG( flag == TVE_COLLAPSE ||
                  flag == (TVE_COLLAPSE | TVE_COLLAPSERESET) ||
                  flag == TVE_EXPAND   ||
                  flag == TVE_TOGGLE,
                  wxT("Unknown flag in wxTreeCtrl::DoExpand") );

    // TreeView_Expand doesn't send TVN_ITEMEXPAND(ING) messages, so we must
    // emulate them. This behaviour has changed slightly with comctl32.dll
    // v 4.70 - now it does send them but only the first time. To maintain
    // compatible behaviour and also in order to not have surprises with the
    // future versions, don't rely on this and still do everything ourselves.
    // To avoid that the messages be sent twice when the item is expanded for
    // the first time we must clear TVIS_EXPANDEDONCE style manually.

    wxTreeViewItem tvItem(item, TVIF_STATE, TVIS_EXPANDEDONCE);
    tvItem.state = 0;
    DoSetItem(&tvItem);

    if ( TreeView_Expand(GetHwnd(), (HTREEITEM) (WXHTREEITEM) item, flag) != 0 )
    {
        wxTreeEvent event(wxEVT_NULL, m_windowId);
        event.m_item = item;

        bool isExpanded = IsExpanded(item);

        event.SetEventObject(this);

        // FIXME return value of {EXPAND|COLLAPS}ING event handler is discarded
        event.SetEventType(g_events[isExpanded][TRUE]);
        GetEventHandler()->ProcessEvent(event);

        event.SetEventType(g_events[isExpanded][FALSE]);
        GetEventHandler()->ProcessEvent(event);
    }
    //else: change didn't took place, so do nothing at all
}

void wxTreeCtrl::Expand(const wxTreeItemId& item)
{
    DoExpand(item, TVE_EXPAND);
}

void wxTreeCtrl::Collapse(const wxTreeItemId& item)
{
    DoExpand(item, TVE_COLLAPSE);
}

void wxTreeCtrl::CollapseAndReset(const wxTreeItemId& item)
{
    DoExpand(item, TVE_COLLAPSE | TVE_COLLAPSERESET);
}

void wxTreeCtrl::Toggle(const wxTreeItemId& item)
{
    DoExpand(item, TVE_TOGGLE);
}

void wxTreeCtrl::ExpandItem(const wxTreeItemId& item, int action)
{
    DoExpand(item, action);
}

void wxTreeCtrl::Unselect()
{
    wxASSERT_MSG( !(m_windowStyle & wxTR_MULTIPLE), wxT("doesn't make sense") );

    // just remove the selection
    SelectItem(wxTreeItemId((WXHTREEITEM) 0));
}

void wxTreeCtrl::UnselectAll()
{
    if ( m_windowStyle & wxTR_MULTIPLE )
    {
        wxArrayTreeItemIds selections;
        size_t count = GetSelections(selections);
        for ( size_t n = 0; n < count; n++ )
        {
            SetItemCheck(selections[n], FALSE);
        }
    }
    else
    {
        // just remove the selection
        Unselect();
    }
}

void wxTreeCtrl::SelectItem(const wxTreeItemId& item)
{
    if ( m_windowStyle & wxTR_MULTIPLE )
    {
        // selecting the item means checking it
        SetItemCheck(item);
    }
    else
    {
        // inspite of the docs (MSDN Jan 99 edition), we don't seem to receive
        // the notification from the control (i.e. TVN_SELCHANG{ED|ING}), so
        // send them ourselves

        wxTreeEvent event(wxEVT_NULL, m_windowId);
        event.m_item = item;
        event.SetEventObject(this);

        event.SetEventType(wxEVT_COMMAND_TREE_SEL_CHANGING);
        if ( !GetEventHandler()->ProcessEvent(event) || event.IsAllowed() )
        {
            if ( !TreeView_SelectItem(GetHwnd(), (HTREEITEM) (WXHTREEITEM) item) )
            {
                wxLogLastError("TreeView_SelectItem");
            }
            else
            {
                event.SetEventType(wxEVT_COMMAND_TREE_SEL_CHANGED);
                (void)GetEventHandler()->ProcessEvent(event);
            }
        }
        //else: program vetoed the change
    }
}

void wxTreeCtrl::EnsureVisible(const wxTreeItemId& item)
{
    // no error return
    TreeView_EnsureVisible(GetHwnd(), (HTREEITEM) (WXHTREEITEM) item);
}

void wxTreeCtrl::ScrollTo(const wxTreeItemId& item)
{
    if ( !TreeView_SelectSetFirstVisible(GetHwnd(), (HTREEITEM) (WXHTREEITEM) item) )
    {
        wxLogLastError("TreeView_SelectSetFirstVisible");
    }
}

wxTextCtrl* wxTreeCtrl::GetEditControl() const
{
    return m_textCtrl;
}

void wxTreeCtrl::DeleteTextCtrl()
{
    if ( m_textCtrl )
    {
        m_textCtrl->UnsubclassWin();
        m_textCtrl->SetHWND(0);
        delete m_textCtrl;
        m_textCtrl = NULL;
    }
}

wxTextCtrl* wxTreeCtrl::EditLabel(const wxTreeItemId& item,
                                  wxClassInfo* textControlClass)
{
    wxASSERT( textControlClass->IsKindOf(CLASSINFO(wxTextCtrl)) );

    HWND hWnd = (HWND) TreeView_EditLabel(GetHwnd(), (HTREEITEM) (WXHTREEITEM) item);

    // this is not an error - the TVN_BEGINLABELEDIT handler might have
    // returned FALSE
    if ( !hWnd )
    {
        return NULL;
    }

    DeleteTextCtrl();

    m_textCtrl = (wxTextCtrl *)textControlClass->CreateObject();
    m_textCtrl->SetHWND((WXHWND)hWnd);
    m_textCtrl->SubclassWin((WXHWND)hWnd);

    return m_textCtrl;
}

// End label editing, optionally cancelling the edit
void wxTreeCtrl::EndEditLabel(const wxTreeItemId& item, bool discardChanges)
{
    TreeView_EndEditLabelNow(GetHwnd(), discardChanges);

    DeleteTextCtrl();
}

wxTreeItemId wxTreeCtrl::HitTest(const wxPoint& point, int& flags)
{
    TV_HITTESTINFO hitTestInfo;
    hitTestInfo.pt.x = (int)point.x;
    hitTestInfo.pt.y = (int)point.y;

    TreeView_HitTest(GetHwnd(), &hitTestInfo);

    flags = 0;

    // avoid repetition
    #define TRANSLATE_FLAG(flag) if ( hitTestInfo.flags & TVHT_##flag ) \
                                    flags |= wxTREE_HITTEST_##flag

    TRANSLATE_FLAG(ABOVE);
    TRANSLATE_FLAG(BELOW);
    TRANSLATE_FLAG(NOWHERE);
    TRANSLATE_FLAG(ONITEMBUTTON);
    TRANSLATE_FLAG(ONITEMICON);
    TRANSLATE_FLAG(ONITEMINDENT);
    TRANSLATE_FLAG(ONITEMLABEL);
    TRANSLATE_FLAG(ONITEMRIGHT);
    TRANSLATE_FLAG(ONITEMSTATEICON);
    TRANSLATE_FLAG(TOLEFT);
    TRANSLATE_FLAG(TORIGHT);

    #undef TRANSLATE_FLAG

    return wxTreeItemId((WXHTREEITEM) hitTestInfo.hItem);
}

bool wxTreeCtrl::GetBoundingRect(const wxTreeItemId& item,
                                 wxRect& rect,
                                 bool textOnly) const
{
    RECT rc;
    if ( TreeView_GetItemRect(GetHwnd(), (HTREEITEM)(WXHTREEITEM)item,
                              &rc, textOnly) )
    {
        rect = wxRect(wxPoint(rc.left, rc.top), wxPoint(rc.right, rc.bottom));

        return TRUE;
    }
    else
    {
        // couldn't retrieve rect: for example, item isn't visible
        return FALSE;
    }
}

// ----------------------------------------------------------------------------
// sorting stuff
// ----------------------------------------------------------------------------

static int CALLBACK TreeView_CompareCallback(wxTreeItemData *pItem1,
                                             wxTreeItemData *pItem2,
                                             wxTreeCtrl *tree)
{
    wxCHECK_MSG( pItem1 && pItem2, 0,
                 wxT("sorting tree without data doesn't make sense") );

    return tree->OnCompareItems(pItem1->GetId(), pItem2->GetId());
}

int wxTreeCtrl::OnCompareItems(const wxTreeItemId& item1,
                               const wxTreeItemId& item2)
{
    return wxStrcmp(GetItemText(item1), GetItemText(item2));
}

void wxTreeCtrl::SortChildren(const wxTreeItemId& item)
{
    // rely on the fact that TreeView_SortChildren does the same thing as our
    // default behaviour, i.e. sorts items alphabetically and so call it
    // directly if we're not in derived class (much more efficient!)
    if ( GetClassInfo() == CLASSINFO(wxTreeCtrl) )
    {
        TreeView_SortChildren(GetHwnd(), (HTREEITEM)(WXHTREEITEM)item, 0);
    }
    else
    {
        TV_SORTCB tvSort;
        tvSort.hParent = (HTREEITEM)(WXHTREEITEM)item;
        tvSort.lpfnCompare = (PFNTVCOMPARE)TreeView_CompareCallback;
        tvSort.lParam = (LPARAM)this;
        TreeView_SortChildrenCB(GetHwnd(), &tvSort, 0 /* reserved */);
    }
}

// ----------------------------------------------------------------------------
// implementation
// ----------------------------------------------------------------------------

bool wxTreeCtrl::MSWCommand(WXUINT cmd, WXWORD id)
{
    if ( cmd == EN_UPDATE )
    {
        wxCommandEvent event(wxEVT_COMMAND_TEXT_UPDATED, id);
        event.SetEventObject( this );
        ProcessCommand(event);
    }
    else if ( cmd == EN_KILLFOCUS )
    {
        wxCommandEvent event(wxEVT_KILL_FOCUS, id);
        event.SetEventObject( this );
        ProcessCommand(event);
    }
    else
    {
        // nothing done
        return FALSE;
    }

    // command processed
    return TRUE;
}

// we hook into WndProc to process WM_MOUSEMOVE/WM_BUTTONUP messages - as we
// only do it during dragging, minimize wxWin overhead (this is important for
// WM_MOUSEMOVE as they're a lot of them) by catching Windows messages directly
// instead of passing by wxWin events
long wxTreeCtrl::MSWWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam)
{
    if ( m_dragImage )
    {
        switch ( nMsg )
        {
            case WM_MOUSEMOVE:
                {
                    int x = GET_X_LPARAM(lParam),
                        y = GET_Y_LPARAM(lParam);

                    m_dragImage->Move(wxPoint(x, y), this);

                    HTREEITEM htiTarget = GetItemFromPoint(GetHwnd(), x, y);
                    if ( htiTarget )
                    {
                        // highlight the item as target (hiding drag image is
                        // necessary - otherwise the display will be corrupted)
                        m_dragImage->Hide(this);
                        TreeView_SelectDropTarget(GetHwnd(), htiTarget);
                        m_dragImage->Show(this);
                    }
                }
                break;

            case WM_LBUTTONUP:
            case WM_RBUTTONUP:
                {
                    m_dragImage->EndDrag(this);
                    delete m_dragImage;
                    m_dragImage = NULL;

                    // generate the drag end event
                    wxTreeEvent event(wxEVT_COMMAND_TREE_END_DRAG, m_windowId);

                    int x = GET_X_LPARAM(lParam),
                        y = GET_Y_LPARAM(lParam);

                    event.m_item
                        = (WXHTREEITEM)GetItemFromPoint(GetHwnd(), x, y);
                    event.m_pointDrag = wxPoint(x, y);
                    event.SetEventObject(this);

                    (void)GetEventHandler()->ProcessEvent(event);
                }
                break;
        }
    }

    return wxControl::MSWWindowProc(nMsg, wParam, lParam);
}

// process WM_NOTIFY Windows message
bool wxTreeCtrl::MSWOnNotify(int idCtrl, WXLPARAM lParam, WXLPARAM *result)
{
    wxTreeEvent event(wxEVT_NULL, m_windowId);
    wxEventType eventType = wxEVT_NULL;
    NMHDR *hdr = (NMHDR *)lParam;

    switch ( hdr->code )
    {
        case NM_RCLICK:
            {
                if ( wxControl::MSWOnNotify(idCtrl, lParam, result) )
                    return TRUE;

                TV_HITTESTINFO tvhti;
                ::GetCursorPos(&(tvhti.pt));
                ::ScreenToClient(GetHwnd(),&(tvhti.pt));
                if ( TreeView_HitTest(GetHwnd(),&tvhti) )
                {
                    if( tvhti.flags & TVHT_ONITEM )
                    {
                        event.m_item = (WXHTREEITEM) tvhti.hItem;
                        eventType = wxEVT_COMMAND_TREE_ITEM_RIGHT_CLICK;
                    }
                }
            }
            break;

        case TVN_BEGINDRAG:
            eventType = wxEVT_COMMAND_TREE_BEGIN_DRAG;
            // fall through

        case TVN_BEGINRDRAG:
            {
                if ( eventType == wxEVT_NULL )
                    eventType = wxEVT_COMMAND_TREE_BEGIN_RDRAG;
                //else: left drag, already set above

                NM_TREEVIEW *tv = (NM_TREEVIEW *)lParam;

                event.m_item = (WXHTREEITEM) tv->itemNew.hItem;
                event.m_pointDrag = wxPoint(tv->ptDrag.x, tv->ptDrag.y);

                // don't allow dragging by default: the user code must
                // explicitly say that it wants to allow it to avoid breaking
                // the old apps
                event.Veto();
            }
            break;

        case TVN_BEGINLABELEDIT:
            {
                eventType = wxEVT_COMMAND_TREE_BEGIN_LABEL_EDIT;
                TV_DISPINFO *info = (TV_DISPINFO *)lParam;

                event.m_item = (WXHTREEITEM) info->item.hItem;
                event.m_label = info->item.pszText;
            }
            break;

        case TVN_DELETEITEM:
            {
                eventType = wxEVT_COMMAND_TREE_DELETE_ITEM;
                NM_TREEVIEW *tv = (NM_TREEVIEW *)lParam;

                event.m_item = (WXHTREEITEM)tv->itemOld.hItem;

                if ( m_hasAnyAttr )
                {
                    delete (wxTreeItemAttr *)m_attrs.
                                Delete((long)tv->itemOld.hItem);
                }
            }
            break;

        case TVN_ENDLABELEDIT:
            {
                eventType = wxEVT_COMMAND_TREE_END_LABEL_EDIT;
                TV_DISPINFO *info = (TV_DISPINFO *)lParam;

                event.m_item = (WXHTREEITEM)info->item.hItem;
                event.m_label = info->item.pszText;
                if (info->item.pszText == NULL)
                    return FALSE;
                break;
            }

        case TVN_GETDISPINFO:
            eventType = wxEVT_COMMAND_TREE_GET_INFO;
            // fall through

        case TVN_SETDISPINFO:
            {
                if ( eventType == wxEVT_NULL )
                    eventType = wxEVT_COMMAND_TREE_SET_INFO;
                //else: get, already set above

                TV_DISPINFO *info = (TV_DISPINFO *)lParam;

                event.m_item = (WXHTREEITEM) info->item.hItem;
                break;
            }

        case TVN_ITEMEXPANDING:
            event.m_code = FALSE;
            // fall through

        case TVN_ITEMEXPANDED:
            {
                NM_TREEVIEW* tv = (NM_TREEVIEW*)lParam;

                bool expand = FALSE;
                switch ( tv->action )
                {
                    case TVE_EXPAND:
                        expand = TRUE;
                        break;

                    case TVE_COLLAPSE:
                        expand = FALSE;
                        break;

                    default:
                        wxLogDebug(wxT("unexpected code %d in TVN_ITEMEXPAND "
                                      "message"), tv->action);
                }

                bool ing = ((int)hdr->code == TVN_ITEMEXPANDING);
                eventType = g_events[expand][ing];

                event.m_item = (WXHTREEITEM) tv->itemNew.hItem;
            }
            break;

        case TVN_KEYDOWN:
            {
                eventType = wxEVT_COMMAND_TREE_KEY_DOWN;
                TV_KEYDOWN *info = (TV_KEYDOWN *)lParam;

                event.m_code = wxCharCodeMSWToWX(info->wVKey);

                // a separate event for this case
                if ( info->wVKey == VK_SPACE || info->wVKey == VK_RETURN )
                {
                    wxTreeEvent event2(wxEVT_COMMAND_TREE_ITEM_ACTIVATED,
                                       m_windowId);
                    event2.SetEventObject(this);

                    GetEventHandler()->ProcessEvent(event2);
                }
            }
            break;

        case TVN_SELCHANGED:
            eventType = wxEVT_COMMAND_TREE_SEL_CHANGED;
            // fall through

        case TVN_SELCHANGING:
            {
                if ( eventType == wxEVT_NULL )
                    eventType = wxEVT_COMMAND_TREE_SEL_CHANGING;
                //else: already set above

                NM_TREEVIEW* tv = (NM_TREEVIEW *)lParam;

                event.m_item = (WXHTREEITEM) tv->itemNew.hItem;
                event.m_itemOld = (WXHTREEITEM) tv->itemOld.hItem;
            }
            break;

#if defined(_WIN32_IE) && _WIN32_IE >= 0x300
        case NM_CUSTOMDRAW:
            {
                LPNMTVCUSTOMDRAW lptvcd = (LPNMTVCUSTOMDRAW)lParam;
                NMCUSTOMDRAW& nmcd = lptvcd->nmcd;
                switch( nmcd.dwDrawStage )
                {
                    case CDDS_PREPAINT:
                        // if we've got any items with non standard attributes,
                        // notify us before painting each item
                        *result = m_hasAnyAttr ? CDRF_NOTIFYITEMDRAW
                                               : CDRF_DODEFAULT;
                        return TRUE;

                    case CDDS_ITEMPREPAINT:
                        {
                            wxTreeItemAttr *attr =
                                (wxTreeItemAttr *)m_attrs.Get(nmcd.dwItemSpec);

                            if ( !attr )
                            {
                                // nothing to do for this item
                                return CDRF_DODEFAULT;
                            }

                            HFONT hFont;
                            wxColour colText, colBack;
                            if ( attr->HasFont() )
                            {
                                wxFont font = attr->GetFont();
                                hFont = (HFONT)font.GetResourceHandle();
                            }
                            else
                            {
                                hFont = 0;
                            }

                            if ( attr->HasTextColour() )
                            {
                                colText = attr->GetTextColour();
                            }
                            else
                            {
                                colText = GetForegroundColour();
                            }

                            // selection colours should override ours
                            if ( nmcd.uItemState & CDIS_SELECTED )
                            {
                                DWORD clrBk = ::GetSysColor(COLOR_HIGHLIGHT);
                                lptvcd->clrTextBk = clrBk;

                                // try to make the text visible
                                lptvcd->clrText = wxColourToRGB(colText);
                                lptvcd->clrText |= ~clrBk;
                                lptvcd->clrText &= 0x00ffffff;
                            }
                            else
                            {
                                if ( attr->HasBackgroundColour() )
                                {
                                    colBack = attr->GetBackgroundColour();
                                }
                                else
                                {
                                    colBack = GetBackgroundColour();
                                }

                                lptvcd->clrText = wxColourToRGB(colText);
                                lptvcd->clrTextBk = wxColourToRGB(colBack);
                            }

                            // note that if we wanted to set colours for
                            // individual columns (subitems), we would have
                            // returned CDRF_NOTIFYSUBITEMREDRAW from here
                            if ( hFont )
                            {
                                ::SelectObject(nmcd.hdc, hFont);

                                *result = CDRF_NEWFONT;
                            }
                            else
                            {
                                *result = CDRF_DODEFAULT;
                            }

                            return TRUE;
                        }

                    default:
                        *result = CDRF_DODEFAULT;
                        return TRUE;
                }
            }
            break;
#endif // _WIN32_IE >= 0x300

        default:
            return wxControl::MSWOnNotify(idCtrl, lParam, result);
    }

    event.SetEventObject(this);
    event.SetEventType(eventType);

    bool processed = GetEventHandler()->ProcessEvent(event);

    // post processing
    switch ( hdr->code )
    {
        case TVN_BEGINDRAG:
        case TVN_BEGINRDRAG:
            if ( event.IsAllowed() )
            {
                // normally this is impossible because the m_dragImage is
                // deleted once the drag operation is over
                wxASSERT_MSG( !m_dragImage, _T("starting to drag once again?") );

                m_dragImage = new wxDragImage(*this, event.m_item);
                m_dragImage->BeginDrag(wxPoint(0, 0), this);
                m_dragImage->Show(this);
            }
            break;

        case TVN_DELETEITEM:
            {
                // NB: we might process this message using wxWindows event
                //     tables, but due to overhead of wxWin event system we
                //     prefer to do it here ourself (otherwise deleting a tree
                //     with many items is just too slow)
                NM_TREEVIEW* tv = (NM_TREEVIEW *)lParam;

                wxTreeItemId item = event.m_item;
                if ( HasIndirectData(item) )
                {
                    wxTreeItemIndirectData *data = (wxTreeItemIndirectData *)
                                                        tv->itemOld.lParam;
                    delete data; // can't be NULL here

                    m_itemsWithIndirectData.Remove(item);
                }
                else
                {
                    wxTreeItemData *data = (wxTreeItemData *)tv->itemOld.lParam;
                    delete data; // may be NULL, ok
                }

                processed = TRUE; // Make sure we don't get called twice
            }
            break;

        case TVN_BEGINLABELEDIT:
            // return TRUE to cancel label editing
            *result = !event.IsAllowed();
            break;

        case TVN_ENDLABELEDIT:
            // return TRUE to set the label to the new string
            *result = event.IsAllowed();

            // ensure that we don't have the text ctrl which is going to be
            // deleted any more
            DeleteTextCtrl();
            break;

        case TVN_SELCHANGING:
        case TVN_ITEMEXPANDING:
            // return TRUE to prevent the action from happening
            *result = !event.IsAllowed();
            break;

        case TVN_GETDISPINFO:
            // NB: so far the user can't set the image himself anyhow, so do it
            //     anyway - but this may change later
            if ( /* !processed && */ 1 )
            {
                wxTreeItemId item = event.m_item;
                TV_DISPINFO *info = (TV_DISPINFO *)lParam;
                if ( info->item.mask & TVIF_IMAGE )
                {
                    info->item.iImage =
                        DoGetItemImageFromData
                        (
                         item,
                         IsExpanded(item) ? wxTreeItemIcon_Expanded
                                          : wxTreeItemIcon_Normal
                        );
                }
                if ( info->item.mask & TVIF_SELECTEDIMAGE )
                {
                    info->item.iSelectedImage =
                        DoGetItemImageFromData
                        (
                         item,
                         IsExpanded(item) ? wxTreeItemIcon_SelectedExpanded
                                          : wxTreeItemIcon_Selected
                        );
                }
            }
            break;

        //default:
            // for the other messages the return value is ignored and there is
            // nothing special to do
    }

    return processed;
}

// ----------------------------------------------------------------------------
// Tree event
// ----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxTreeEvent, wxNotifyEvent)

wxTreeEvent::wxTreeEvent(wxEventType commandType, int id)
           : wxNotifyEvent(commandType, id)
{
    m_code = 0;
    m_itemOld = 0;
}

#endif // __WIN95__

