///////////////////////////////////////////////////////////////////////////////
// Name:        listbox.cpp
// Purpose:     wxListBox
// Author:      Julian Smart
// Modified by: Vadim Zeitlin (owner drawn stuff)
// Created:
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows license
///////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
    #pragma implementation "listbox.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/window.h"
#include "wx/msw/private.h"

#ifndef WX_PRECOMP
#include "wx/listbox.h"
#include "wx/settings.h"
#include "wx/brush.h"
#include "wx/font.h"
#include "wx/dc.h"
#include "wx/utils.h"
#endif

#include <windowsx.h>

#ifdef __WXWINE__
  #if defined(GetWindowStyle)
    #undef GetWindowStyle
  #endif
#endif

#include "wx/dynarray.h"
#include "wx/log.h"

#if wxUSE_OWNER_DRAWN
    #include  "wx/ownerdrw.h"
#endif

#ifndef __TWIN32__
    #if defined(__GNUWIN32__)
        #include <wx/msw/gnuwin32/extra.h>
    #endif
#endif

#ifdef __WXWINE__
  #ifndef ListBox_SetItemData
    #define ListBox_SetItemData(hwndCtl, index, data) \
      ((int)(DWORD)SendMessage((hwndCtl), LB_SETITEMDATA, (WPARAM)(int)(index), (LPARAM)(data)))
  #endif
  #ifndef ListBox_GetHorizontalExtent
    #define ListBox_GetHorizontalExtent(hwndCtl) \
      ((int)(DWORD)SendMessage((hwndCtl), LB_GETHORIZONTALEXTENT, 0L, 0L))
  #endif
  #ifndef ListBox_GetSelCount
    #define ListBox_GetSelCount(hwndCtl) \
      ((int)(DWORD)SendMessage((hwndCtl), LB_GETSELCOUNT, 0L, 0L))
  #endif
  #ifndef ListBox_GetSelItems
    #define ListBox_GetSelItems(hwndCtl, cItems, lpItems) \
      ((int)(DWORD)SendMessage((hwndCtl), LB_GETSELITEMS, (WPARAM)(int)(cItems), (LPARAM)(int *)(lpItems)))
  #endif
  #ifndef ListBox_GetTextLen
    #define ListBox_GetTextLen(hwndCtl, index) \
      ((int)(DWORD)SendMessage((hwndCtl), LB_GETTEXTLEN, (WPARAM)(int)(index), 0L))
  #endif
  #ifndef ListBox_GetText
    #define ListBox_GetText(hwndCtl, index, lpszBuffer) \
      ((int)(DWORD)SendMessage((hwndCtl), LB_GETTEXT, (WPARAM)(int)(index), (LPARAM)(LPCTSTR)(lpszBuffer)))
  #endif
#endif

#if !USE_SHARED_LIBRARY
    IMPLEMENT_DYNAMIC_CLASS(wxListBox, wxControl)
#endif

// ============================================================================
// list box item declaration and implementation
// ============================================================================

#if wxUSE_OWNER_DRAWN

class wxListBoxItem : public wxOwnerDrawn
{
public:
    wxListBoxItem(const wxString& str = "");
};

wxListBoxItem::wxListBoxItem(const wxString& str) : wxOwnerDrawn(str, FALSE)
{
    // no bitmaps/checkmarks
    SetMarginWidth(0);
}

wxOwnerDrawn *wxListBox::CreateItem(size_t n)
{
    return new wxListBoxItem();
}

#endif  //USE_OWNER_DRAWN

// ============================================================================
// list box control implementation
// ============================================================================

bool wxListBox::MSWCommand(WXUINT param, WXWORD WXUNUSED(id))
{
    /*
       if (param == LBN_SELCANCEL)
       {
       event.extraLong = FALSE;
       }
     */
    if (param == LBN_SELCHANGE)
    {
        wxCommandEvent event(wxEVT_COMMAND_LISTBOX_SELECTED, m_windowId);
        wxArrayInt aSelections;
        int count = GetSelections(aSelections);
        if ( count > 0 )
        {
            event.m_commandInt = aSelections[0] ;
            event.m_clientData = GetClientData(event.m_commandInt);
            wxString str(GetString(event.m_commandInt));
            if (str != _T(""))
            {
		event.m_commandString = str;
            }
        }
        else
        {
            event.m_commandInt = -1 ;
            event.m_commandString.Empty();
        }

        event.SetEventObject( this );
        ProcessCommand(event);
        return TRUE;
    }
    else if (param == LBN_DBLCLK)
    {
        wxCommandEvent event(wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, m_windowId);
        event.SetEventObject( this );
        GetEventHandler()->ProcessEvent(event) ;
        return TRUE;
    }

    return FALSE;
}

    // Listbox item
wxListBox::wxListBox()
{
    m_noItems = 0;
    m_selected = 0;
}

bool wxListBox::Create(wxWindow *parent,
                       wxWindowID id,
                       const wxPoint& pos,
                       const wxSize& size,
                       int n, const wxString choices[],
                       long style,
                       const wxValidator& validator,
                       const wxString& name)
{
    m_noItems = 0;
    m_hWnd = 0;
    m_selected = 0;

    SetName(name);
    SetValidator(validator);

    if (parent)
        parent->AddChild(this);

    wxSystemSettings settings;
    SetBackgroundColour(settings.GetSystemColour(wxSYS_COLOUR_WINDOW));
    SetForegroundColour(parent->GetForegroundColour());

    m_windowId = ( id == -1 ) ? (int)NewControlId() : id;

    int x = pos.x;
    int y = pos.y;
    int width = size.x;
    int height = size.y;
    m_windowStyle = style;

    DWORD wstyle = WS_VISIBLE | WS_VSCROLL | WS_TABSTOP |
                   LBS_NOTIFY | LBS_HASSTRINGS;
    if (m_windowStyle & wxLB_MULTIPLE)
        wstyle |= LBS_MULTIPLESEL;
    else if (m_windowStyle & wxLB_EXTENDED)
        wstyle |= LBS_EXTENDEDSEL;

    if (m_windowStyle & wxLB_ALWAYS_SB)
        wstyle |= LBS_DISABLENOSCROLL ;
    if (m_windowStyle & wxLB_HSCROLL)
        wstyle |= WS_HSCROLL;
    if (m_windowStyle & wxLB_SORT)
        wstyle |= LBS_SORT;

#if wxUSE_OWNER_DRAWN
    if ( m_windowStyle & wxLB_OWNERDRAW ) {
        // we don't support LBS_OWNERDRAWVARIABLE yet
        wstyle |= LBS_OWNERDRAWFIXED;
    }
#endif

    // Without this style, you get unexpected heights, so e.g. constraint layout
    // doesn't work properly
    wstyle |= LBS_NOINTEGRALHEIGHT;

    bool want3D;
    WXDWORD exStyle = Determine3DEffects(WS_EX_CLIENTEDGE, &want3D) ;

    // Even with extended styles, need to combine with WS_BORDER
    // for them to look right.
    if ( want3D || wxStyleHasBorder(m_windowStyle) )
    {
        wstyle |= WS_BORDER;
    }

    m_hWnd = (WXHWND)::CreateWindowEx(exStyle, _T("LISTBOX"), NULL,
            wstyle | WS_CHILD,
            0, 0, 0, 0,
            (HWND)parent->GetHWND(), (HMENU)m_windowId,
            wxGetInstance(), NULL);

    wxCHECK_MSG( m_hWnd, FALSE, _T("Failed to create listbox") );

#if wxUSE_CTL3D
    if (want3D)
    {
        Ctl3dSubclassCtl(GetHwnd());
        m_useCtl3D = TRUE;
    }
#endif

    // Subclass again to catch messages
    SubclassWin(m_hWnd);

    size_t ui;
    for (ui = 0; ui < (size_t)n; ui++) {
        Append(choices[ui]);
    }

    if ( (m_windowStyle & wxLB_MULTIPLE) == 0 )
        SendMessage(GetHwnd(), LB_SETCURSEL, 0, 0);

    SetFont(parent->GetFont());

    SetSize(x, y, width, height);

    Show(TRUE);

    return TRUE;
}

wxListBox::~wxListBox()
{
#if wxUSE_OWNER_DRAWN
    size_t uiCount = m_aItems.Count();
    while ( uiCount-- != 0 ) {
        delete m_aItems[uiCount];
    }
#endif // wxUSE_OWNER_DRAWN
}

void wxListBox::SetupColours()
{
    SetBackgroundColour(wxSystemSettings::GetSystemColour(wxSYS_COLOUR_WINDOW));
    SetForegroundColour(GetParent()->GetForegroundColour());
}

void wxListBox::SetFirstItem(int N)
{
    wxCHECK_RET( N >= 0 && N < m_noItems,
                 _T("invalid index in wxListBox::SetFirstItem") );

    SendMessage(GetHwnd(),LB_SETTOPINDEX,(WPARAM)N,(LPARAM)0) ;
}

void wxListBox::SetFirstItem(const wxString& s)
{
    int N = FindString(s) ;

    if ( N >= 0 )
        SetFirstItem(N) ;
}

void wxListBox::Delete(int N)
{
    wxCHECK_RET( N >= 0 && N < m_noItems,
                 _T("invalid index in wxListBox::Delete") );

    SendMessage(GetHwnd(), LB_DELETESTRING, N, 0);
    m_noItems--;

    SetHorizontalExtent("");
}

void wxListBox::Append(const wxString& item)
{
    int index = ListBox_AddString(GetHwnd(), item);
    m_noItems ++;

#if wxUSE_OWNER_DRAWN
    if ( m_windowStyle & wxLB_OWNERDRAW ) {
        wxOwnerDrawn *pNewItem = CreateItem(index); // dummy argument
        pNewItem->SetName(item);
        m_aItems.Add(pNewItem);
        ListBox_SetItemData(GetHwnd(), index, pNewItem);
    }
#endif

    SetHorizontalExtent(item);
}

void wxListBox::Append(const wxString& item, void *Client_data)
{
    int index = ListBox_AddString(GetHwnd(), item);
    m_noItems ++;

#if wxUSE_OWNER_DRAWN
    if ( m_windowStyle & wxLB_OWNERDRAW ) {
        // client data must be pointer to wxOwnerDrawn, otherwise we would crash
        // in OnMeasure/OnDraw.
        wxFAIL_MSG(_T("Can't use client data with owner-drawn listboxes"));
    }
    else
#endif

    ListBox_SetItemData(GetHwnd(), index, Client_data);

    SetHorizontalExtent(item);
}

void wxListBox::Set(int n, const wxString *choices, void** clientData)
{
    ShowWindow(GetHwnd(), SW_HIDE);
    ListBox_ResetContent(GetHwnd());
    int i;
    for (i = 0; i < n; i++)
    {
        ListBox_AddString(GetHwnd(), choices[i]);
        if ( clientData )
            ListBox_SetItemData(GetHwnd(), i, clientData[i]);
    }
    m_noItems = n;

#if wxUSE_OWNER_DRAWN
    if ( m_windowStyle & wxLB_OWNERDRAW ) {
        // first delete old items
        size_t ui = m_aItems.Count();
        while ( ui-- != 0 ) {
            delete m_aItems[ui];
        }
        m_aItems.Empty();

        // then create new ones
        for (ui = 0; ui < (size_t)n; ui++) {
            wxOwnerDrawn *pNewItem = CreateItem(ui);
            pNewItem->SetName(choices[ui]);
            m_aItems.Add(pNewItem);
            ListBox_SetItemData(GetHwnd(), ui, pNewItem);

            wxASSERT_MSG(clientData[ui] == NULL,
                    _T("Can't use client data with owner-drawn listboxes"));
        }
    }
#endif

    SetHorizontalExtent("");
    ShowWindow(GetHwnd(), SW_SHOW);
}

int wxListBox::FindString(const wxString& s) const
{
    int pos = ListBox_FindStringExact(GetHwnd(), (WPARAM)-1, s);
    if (pos == LB_ERR)
        return -1;
    else
        return pos;
}

void wxListBox::Clear()
{
    ListBox_ResetContent(GetHwnd());

#if wxUSE_OWNER_DRAWN
    size_t uiCount = m_aItems.Count();
    while ( uiCount-- != 0 ) {
        delete m_aItems[uiCount];
    }

    m_aItems.Clear();
#endif // wxUSE_OWNER_DRAWN

    m_noItems = 0;
    ListBox_GetHorizontalExtent(GetHwnd());
}

void wxListBox::SetSelection(int N, bool select)
{
    wxCHECK_RET( N >= 0 && N < m_noItems,
                 _T("invalid index in wxListBox::SetSelection") );

    if ((m_windowStyle & wxLB_MULTIPLE) || (m_windowStyle & wxLB_EXTENDED))
        SendMessage(GetHwnd(), LB_SETSEL, select, N);
    else
    {
        int N1 = N;
        if (!select)
            N1 = -1;
        SendMessage(GetHwnd(), LB_SETCURSEL, N1, 0);
    }
}

bool wxListBox::Selected(int N) const
{
    wxCHECK_MSG( N >= 0 && N < m_noItems, FALSE,
                 _T("invalid index in wxListBox::Selected") );

    return SendMessage(GetHwnd(), LB_GETSEL, N, 0) == 0 ? FALSE : TRUE;
}

void wxListBox::Deselect(int N)
{
    wxCHECK_RET( N >= 0 && N < m_noItems,
                 _T("invalid index in wxListBox::Deselect") );

    if ((m_windowStyle & wxLB_MULTIPLE) || (m_windowStyle & wxLB_EXTENDED))
        SendMessage(GetHwnd(), LB_SETSEL, FALSE, N);
}

void *wxListBox::GetClientData(int N) const
{
    wxCHECK_MSG( N >= 0 && N < m_noItems, NULL,
                 _T("invalid index in wxListBox::GetClientData") );

    return (void *)SendMessage(GetHwnd(), LB_GETITEMDATA, N, 0);
}

void wxListBox::SetClientData(int N, void *Client_data)
{
    wxCHECK_RET( N >= 0 && N < m_noItems,
                 _T("invalid index in wxListBox::SetClientData") );

    if ( ListBox_SetItemData(GetHwnd(), N, Client_data) == LB_ERR )
        wxLogDebug(_T("LB_SETITEMDATA failed"));
}

// Return number of selections and an array of selected integers
int wxListBox::GetSelections(wxArrayInt& aSelections) const
{
    aSelections.Empty();

    if ((m_windowStyle & wxLB_MULTIPLE) || (m_windowStyle & wxLB_EXTENDED))
    {
        int no_sel = ListBox_GetSelCount(GetHwnd());
        if (no_sel != 0) {
            int *selections = new int[no_sel];
            if ( ListBox_GetSelItems(GetHwnd(), no_sel, selections) == LB_ERR ) {
                wxFAIL_MSG(_T("This listbox can't have single-selection style!"));
            }

            aSelections.Alloc(no_sel);
            for ( int n = 0; n < no_sel; n++ )
                aSelections.Add(selections[n]);

            delete [] selections;
        }

        return no_sel;
    }
    else  // single-selection listbox
    {
        aSelections.Add(ListBox_GetCurSel(GetHwnd()));

        return 1;
    }
}

// Get single selection, for single choice list items
int wxListBox::GetSelection() const
{
    wxCHECK_MSG( !(m_windowStyle & wxLB_MULTIPLE) &&
                 !(m_windowStyle & wxLB_EXTENDED),
                 -1,
                 _T("GetSelection() can't be used with multiple-selection "
                    "listboxes, use GetSelections() instead.") );

    return ListBox_GetCurSel(GetHwnd());
}

// Find string for position
wxString wxListBox::GetString(int N) const
{
    wxCHECK_MSG( N >= 0 && N < m_noItems, "",
                 _T("invalid index in wxListBox::GetClientData") );

    int len = ListBox_GetTextLen(GetHwnd(), N);

    // +1 for terminating NUL
    wxString result;
    ListBox_GetText(GetHwnd(), N, result.GetWriteBuf(len + 1));
    result.UngetWriteBuf();

    return result;
}

// Windows-specific code to set the horizontal extent of the listbox, if
// necessary. If s is non-NULL, it's used to calculate the horizontal extent.
// Otherwise, all strings are used.
void wxListBox::SetHorizontalExtent(const wxString& s)
{
    // Only necessary if we want a horizontal scrollbar
    if (!(m_windowStyle & wxHSCROLL))
        return;
    TEXTMETRIC lpTextMetric;

    if (s != _T(""))
    {
        int existingExtent = (int)SendMessage(GetHwnd(), LB_GETHORIZONTALEXTENT, 0, 0L);
        HDC dc = GetWindowDC(GetHwnd());
        HFONT oldFont = 0;
        if (GetFont().Ok() && GetFont().GetResourceHandle())
            oldFont = (HFONT) ::SelectObject(dc, (HFONT) GetFont().GetResourceHandle());

        GetTextMetrics(dc, &lpTextMetric);
        SIZE extentXY;
        ::GetTextExtentPoint(dc, (LPTSTR) (const wxChar *)s, s.Length(), &extentXY);
        int extentX = (int)(extentXY.cx + lpTextMetric.tmAveCharWidth);

        if (oldFont)
            ::SelectObject(dc, oldFont);

        ReleaseDC(GetHwnd(), dc);
        if (extentX > existingExtent)
            SendMessage(GetHwnd(), LB_SETHORIZONTALEXTENT, LOWORD(extentX), 0L);
        return;
    }
    else
    {
        int largestExtent = 0;
        HDC dc = GetWindowDC(GetHwnd());
        HFONT oldFont = 0;
        if (GetFont().Ok() && GetFont().GetResourceHandle())
            oldFont = (HFONT) ::SelectObject(dc, (HFONT) GetFont().GetResourceHandle());

        GetTextMetrics(dc, &lpTextMetric);
        int i;
        for (i = 0; i < m_noItems; i++)
        {
            int len = (int)SendMessage(GetHwnd(), LB_GETTEXT, i, (LONG)wxBuffer);
            wxBuffer[len] = 0;
            SIZE extentXY;
            ::GetTextExtentPoint(dc, (LPTSTR)wxBuffer, len, &extentXY);
            int extentX = (int)(extentXY.cx + lpTextMetric.tmAveCharWidth);
            if (extentX > largestExtent)
                largestExtent = extentX;
        }
        if (oldFont)
            ::SelectObject(dc, oldFont);

        ReleaseDC(GetHwnd(), dc);
        SendMessage(GetHwnd(), LB_SETHORIZONTALEXTENT, LOWORD(largestExtent), 0L);
    }
}

void
wxListBox::InsertItems(int nItems, const wxString items[], int pos)
{
    wxCHECK_RET( pos >= 0 && pos <= m_noItems,
                 _T("invalid index in wxListBox::InsertItems") );

    int i;
    for (i = 0; i < nItems; i++)
        ListBox_InsertString(GetHwnd(), i + pos, items[i]);
    m_noItems += nItems;

    SetHorizontalExtent(_T(""));
}

void wxListBox::SetString(int N, const wxString& s)
{
    wxCHECK_RET( N >= 0 && N < m_noItems,
                 _T("invalid index in wxListBox::SetString") );

    int sel = -1;
    if (!(m_windowStyle & wxLB_MULTIPLE) && !(m_windowStyle & wxLB_EXTENDED))
        sel = GetSelection();

    void *oldData = wxListBox::GetClientData(N);

    SendMessage(GetHwnd(), LB_DELETESTRING, N, 0);

    int newN = N;
    if (N == (m_noItems - 1))
        newN = -1;

    SendMessage(GetHwnd(), LB_INSERTSTRING, newN, (LPARAM) (const wxChar *)s);
    if (oldData)
        wxListBox::SetClientData(N, oldData);

    // Selection may have changed
    if (sel >= 0)
        SetSelection(sel);

#if       wxUSE_OWNER_DRAWN
    if ( m_windowStyle & wxLB_OWNERDRAW )
        // update item's text
        m_aItems[N]->SetName(s);
#endif  //USE_OWNER_DRAWN
}

int wxListBox::Number () const
{
    return m_noItems;
}

// For single selection items only
wxString wxListBox::GetStringSelection () const
{
    int sel = GetSelection ();
    if (sel > -1)
        return this->GetString (sel);
    else
        return wxString("");
}

bool wxListBox::SetStringSelection (const wxString& s, bool flag)
{
    int sel = FindString (s);
    if (sel > -1)
    {
        SetSelection (sel, flag);
        return TRUE;
    }
    else
        return FALSE;
}

wxSize wxListBox::DoGetBestSize()
{
    // find the widest string
    int wLine;
    int wListbox = 0;
    for ( int i = 0; i < m_noItems; i++ )
    {
        wxString str(GetString(i));
        GetTextExtent(str, &wLine, NULL);
        if ( wLine > wListbox )
            wListbox = wLine;
    }

    // give it some reasonable default value if there are no strings in the
    // list
    if ( wListbox == 0 )
        wListbox = 100;

    // the listbox should be slightly larger than the widest string
    int cx, cy;
    wxGetCharSize(GetHWND(), &cx, &cy, &GetFont());

    wListbox += 3*cx;

    int hListbox = EDIT_HEIGHT_FROM_CHAR_HEIGHT(cy)*(wxMax(m_noItems, 7));

    return wxSize(wListbox, hListbox);
}

// Is this the right thing? Won't setselection generate a command
// event too? No! It'll just generate a setselection event.
// But we still can't have this being called whenever a real command
// is generated, because it sets the selection, which will already
// have been done! (Unless we have an optional argument for calling
// by the actual window system, or a separate function, ProcessCommand)
void wxListBox::Command (wxCommandEvent & event)
{
    if (event.m_extraLong)
        SetSelection (event.m_commandInt);
    else
    {
        Deselect (event.m_commandInt);
        return;
    }
    ProcessCommand (event);
}

WXHBRUSH wxListBox::OnCtlColor(WXHDC pDC, WXHWND pWnd, WXUINT nCtlColor,
        WXUINT message, WXWPARAM wParam, WXLPARAM lParam)
{
#if wxUSE_CTL3D
    if ( m_useCtl3D )
    {
        HBRUSH hbrush = Ctl3dCtlColorEx(message, wParam, lParam);
        return (WXHBRUSH) hbrush;
    }
#endif

    if (GetParent()->GetTransparentBackground())
        SetBkMode((HDC) pDC, TRANSPARENT);
    else
        SetBkMode((HDC) pDC, OPAQUE);

    ::SetBkColor((HDC) pDC, RGB(GetBackgroundColour().Red(), GetBackgroundColour().Green(), GetBackgroundColour().Blue()));
    ::SetTextColor((HDC) pDC, RGB(GetForegroundColour().Red(), GetForegroundColour().Green(), GetForegroundColour().Blue()));

    wxBrush *backgroundBrush = wxTheBrushList->FindOrCreateBrush(GetBackgroundColour(), wxSOLID);

    // Note that this will be cleaned up in wxApp::OnIdle, if backgroundBrush
    // has a zero usage count.
    backgroundBrush->RealizeResource();
    return (WXHBRUSH) backgroundBrush->GetResourceHandle();
}

long wxListBox::MSWWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam)
{
    return wxControl::MSWWindowProc(nMsg, wParam, lParam);
}

#if wxUSE_OWNER_DRAWN

// drawing
// -------

// space beneath/above each row in pixels
// "standard" checklistbox use 1 here, some might prefer 2. 0 is ugly.
#define OWNER_DRAWN_LISTBOX_EXTRA_SPACE    (1)

// the height is the same for all items
// TODO should be changed for LBS_OWNERDRAWVARIABLE style listboxes

// NB: can't forward this to wxListBoxItem because LB_SETITEMDATA
//     message is not yet sent when we get here!
bool wxListBox::MSWOnMeasure(WXMEASUREITEMSTRUCT *item)
{
    // only owner-drawn control should receive this message
    wxCHECK( ((m_windowStyle & wxLB_OWNERDRAW) == wxLB_OWNERDRAW), FALSE );

    MEASUREITEMSTRUCT *pStruct = (MEASUREITEMSTRUCT *)item;

    wxDC dc;
    dc.SetHDC((WXHDC)CreateIC(_T("DISPLAY"), NULL, NULL, 0));
    dc.SetFont(wxSystemSettings::GetSystemFont(wxSYS_ANSI_VAR_FONT));

    pStruct->itemHeight = dc.GetCharHeight() + 2*OWNER_DRAWN_LISTBOX_EXTRA_SPACE;
    pStruct->itemWidth  = dc.GetCharWidth();

    return TRUE;
}

// forward the message to the appropriate item
bool wxListBox::MSWOnDraw(WXDRAWITEMSTRUCT *item)
{
    // only owner-drawn control should receive this message
    wxCHECK( ((m_windowStyle & wxLB_OWNERDRAW) == wxLB_OWNERDRAW), FALSE );

    DRAWITEMSTRUCT *pStruct = (DRAWITEMSTRUCT *)item;

    long data = ListBox_GetItemData(GetHwnd(), pStruct->itemID);

    wxCHECK( data && (data != LB_ERR), FALSE );

    wxListBoxItem *pItem = (wxListBoxItem *)data;

    wxDC dc;
    dc.SetHDC((WXHDC)pStruct->hDC, FALSE);
    wxRect rect(wxPoint(pStruct->rcItem.left, pStruct->rcItem.top),
            wxPoint(pStruct->rcItem.right, pStruct->rcItem.bottom));

    return pItem->OnDrawItem(dc, rect,
            (wxOwnerDrawn::wxODAction)pStruct->itemAction,
            (wxOwnerDrawn::wxODStatus)pStruct->itemState);
}

#endif
    // wxUSE_OWNER_DRAWN
