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

#ifndef WX_PRECOMP
#include "wx/listbox.h"
#include "wx/settings.h"
#endif

#include "wx/msw/private.h"

#include <windows.h>
#include <windowsx.h>

#ifdef __GNUWIN32__
#include <wx/msw/gnuwin32/extra.h>
#endif

#ifdef GetCharWidth
#undef GetCharWidth
#endif

#if wxUSE_OWNER_DRAWN
  #include  "wx/ownerdrw.h"
#endif

#include "wx/dynarray.h"
#include "wx/log.h"

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

// this macro is dangerous but still better than tons of (HWND)GetHWND()
#define   hwnd    (HWND)GetHWND()

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
      if (str != "")
        event.m_commandString = copystring((char *)(const char *)str);
    }
    else
    {
      event.m_commandInt = -1 ;
      event.m_commandString = copystring("") ;
    }

    event.SetEventObject( this );
    ProcessCommand(event);
    if (event.m_commandString)
      delete[] event.m_commandString ;
    return TRUE;
  }
  else if (param == LBN_DBLCLK)
  {
    wxCommandEvent event(wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, m_windowId);
    event.SetEventObject( this );
    GetEventHandler()->ProcessEvent(event) ;
    return TRUE;
/*
    {
#if WXWIN_COMPATIBILITY
      wxWindow *parent = (wxWindow *)GetParent();
      if (parent)
        parent->GetEventHandler()->OnDefaultAction(this);
#endif
      return TRUE;
    }
 */
  }
  return FALSE;
}

// Listbox item
wxListBox::wxListBox(void)
{
  m_noItems = 0;
  m_selected = 0;
}

bool wxListBox::Create(wxWindow *parent, wxWindowID id,
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

  if (parent) parent->AddChild(this);

  wxSystemSettings settings;
  SetBackgroundColour(settings.GetSystemColour(wxSYS_COLOUR_WINDOW));
  SetForegroundColour(parent->GetForegroundColour());

  m_windowId = ( id == -1 ) ? (int)NewControlId() : id;

  int x = pos.x;
  int y = pos.y;
  int width = size.x;
  int height = size.y;
  m_windowStyle = style;

  DWORD wstyle = WS_VSCROLL | WS_TABSTOP | LBS_NOTIFY | LBS_HASSTRINGS;
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

  m_hWnd = (WXHWND)::CreateWindowEx(exStyle, "LISTBOX", NULL,
                                wstyle | WS_CHILD,
                                0, 0, 0, 0, 
                                (HWND)parent->GetHWND(), (HMENU)m_windowId,
                                wxGetInstance(), NULL);

  wxCHECK_MSG( m_hWnd, FALSE, "Failed to create listbox" );

#if CTL3D
  if (want3D)
  {
    Ctl3dSubclassCtl(hwnd);
    m_useCtl3D = TRUE;
  }
#endif

  // Subclass again to catch messages
  SubclassWin(m_hWnd);

  size_t ui;
  for (ui = 0; ui < (size_t)n; ui++) {
    Append(choices[ui]);
  }

  /* Not needed -- done in Append
#if wxUSE_OWNER_DRAWN
    if ( m_windowStyle & wxLB_OWNERDRAW ) {
      for (ui = 0; ui < (size_t)n; ui++) {
        // create new item which will process WM_{DRAW|MEASURE}ITEM messages
        wxOwnerDrawn *pNewItem = CreateItem(ui);
        pNewItem->SetName(choices[ui]);
        m_aItems.Add(pNewItem);
        ListBox_SetItemData(hwnd, ui, pNewItem);
      }
    }
#endif
*/

  if ( (m_windowStyle & wxLB_MULTIPLE) == 0 )
    SendMessage(hwnd, LB_SETCURSEL, 0, 0);

  SetFont(* parent->GetFont());

  SetSize(x, y, width, height);

  Show(TRUE);

  return TRUE;
}

wxListBox::~wxListBox(void)
{
#if wxUSE_OWNER_DRAWN
    size_t uiCount = m_aItems.Count();
    while ( uiCount-- != 0 ) {
      delete m_aItems[uiCount];
    }
#endif
}

void wxListBox::SetupColours(void)
{
  SetBackgroundColour(wxSystemSettings::GetSystemColour(wxSYS_COLOUR_WINDOW));
  SetForegroundColour(GetParent()->GetForegroundColour());
}

void wxListBox::SetFirstItem(int N)
{
  SendMessage(hwnd,LB_SETTOPINDEX,(WPARAM)N,(LPARAM)0) ;
}

void wxListBox::SetFirstItem(const wxString& s)
{
  int N = FindString(s) ;

  if (N>=0)
    SetFirstItem(N) ;
}

void wxListBox::Delete(int N)
{
  SendMessage(hwnd, LB_DELETESTRING, N, 0);
  m_noItems --;
  SetHorizontalExtent("");
}

void wxListBox::Append(const wxString& item)
{
  int index = ListBox_AddString(hwnd, item);
  m_noItems ++;

#if wxUSE_OWNER_DRAWN
    if ( m_windowStyle & wxLB_OWNERDRAW ) {
      wxOwnerDrawn *pNewItem = CreateItem(index); // dummy argument
      pNewItem->SetName(item);
      m_aItems.Add(pNewItem);
      ListBox_SetItemData(hwnd, index, pNewItem);
    }
#endif

  SetHorizontalExtent(item);
}

void wxListBox::Append(const wxString& item, char *Client_data)
{
  int index = ListBox_AddString(hwnd, item);
  m_noItems ++;

#if wxUSE_OWNER_DRAWN
    if ( m_windowStyle & wxLB_OWNERDRAW ) {
      // client data must be pointer to wxOwnerDrawn, otherwise we would crash
      // in OnMeasure/OnDraw.
      wxFAIL_MSG("Can't use client data with owner-drawn listboxes");
    }
    else
#endif
      ListBox_SetItemData(hwnd, index, Client_data);

  SetHorizontalExtent(item);
}

void wxListBox::Set(int n, const wxString *choices, char** clientData)
{
  ShowWindow(hwnd, SW_HIDE);
  ListBox_ResetContent(hwnd);
  int i;
  for (i = 0; i < n; i++)
  {
    ListBox_AddString(hwnd, choices[i]);
    if ( clientData )
      ListBox_SetItemData(hwnd, i, clientData[i]);
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
        ListBox_SetItemData(hwnd, ui, pNewItem);

        wxASSERT_MSG(clientData[ui] == NULL,
                     "Can't use client data with owner-drawn listboxes");
      }
    }
#endif

  SetHorizontalExtent("");
  ShowWindow(hwnd, SW_SHOW);
}

int wxListBox::FindString(const wxString& s) const
{
 int pos = ListBox_FindStringExact(hwnd, (WPARAM)-1, s);
 if (pos == LB_ERR)
   return -1;
 else
   return pos;
}

void wxListBox::Clear(void)
{
  ListBox_ResetContent(hwnd);

  m_noItems = 0;
  ListBox_GetHorizontalExtent(hwnd);
}

void wxListBox::SetSelection(int N, bool select)
{
  if ((m_windowStyle & wxLB_MULTIPLE) || (m_windowStyle & wxLB_EXTENDED))
    SendMessage(hwnd, LB_SETSEL, select, N);
  else
  {
    int N1 = N;
    if (!select)
        N1 = -1;
    SendMessage(hwnd, LB_SETCURSEL, N1, 0);
  }
}

bool wxListBox::Selected(int N) const
{
  return SendMessage(hwnd, LB_GETSEL, N, 0) == 0 ? FALSE : TRUE;
}

void wxListBox::Deselect(int N)
{
  if ((m_windowStyle & wxLB_MULTIPLE) || (m_windowStyle & wxLB_EXTENDED))
    SendMessage(hwnd, LB_SETSEL, FALSE, N);
}

char *wxListBox::GetClientData(int N) const
{
  return (char *)SendMessage(hwnd, LB_GETITEMDATA, N, 0);
}

void wxListBox::SetClientData(int N, char *Client_data)
{
  if ( ListBox_SetItemData(hwnd, N, Client_data) == LB_ERR )
    wxLogDebug("LB_SETITEMDATA failed");
}

// Return number of selections and an array of selected integers
int wxListBox::GetSelections(wxArrayInt& aSelections) const
{
  aSelections.Empty();

  if ((m_windowStyle & wxLB_MULTIPLE) || (m_windowStyle & wxLB_EXTENDED))
  {
    int no_sel = ListBox_GetSelCount(hwnd);
    if (no_sel != 0) {
      int *selections = new int[no_sel];
      if ( ListBox_GetSelItems(hwnd, no_sel, selections) == LB_ERR ) {
        wxFAIL_MSG("This listbox can't have single-selection style!");
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
    aSelections.Add(ListBox_GetCurSel(hwnd));

    return 1;
  }
}

// Get single selection, for single choice list items
int wxListBox::GetSelection() const
{
  wxCHECK_MSG( !(m_windowStyle & wxLB_MULTIPLE) && 
               !(m_windowStyle & wxLB_EXTENDED), 
               -1,
               "GetSelection() can't be used with multiple-selection "
               "listboxes, use GetSelections() instead." );

  return ListBox_GetCurSel(hwnd);
}

// Find string for position
wxString wxListBox::GetString(int N) const
{
  if (N < 0 || N > m_noItems)
    return wxString("");
    
  int len = (int)SendMessage(hwnd, LB_GETTEXT, N, (LONG)wxBuffer);
  wxBuffer[len] = 0;
  return wxString(wxBuffer);
}

void wxListBox::SetSize(int x, int y, int width, int height, int sizeFlags)
{
  int currentX, currentY;
  GetPosition(&currentX, &currentY);

  int x1 = x;
  int y1 = y;
  int w1 = width;
  int h1 = height;

  if (x == -1 || (sizeFlags & wxSIZE_ALLOW_MINUS_ONE))
    x1 = currentX;
  if (y == -1 || (sizeFlags & wxSIZE_ALLOW_MINUS_ONE))
    y1 = currentY;

  AdjustForParentClientOrigin(x1, y1, sizeFlags);

  // If we're prepared to use the existing size, then...
  if (width == -1 && height == -1 && ((sizeFlags & wxSIZE_AUTO) != wxSIZE_AUTO))
  {
    GetSize(&w1, &h1);
  }

  int cx; // button font dimensions
  int cy;

  wxGetCharSize(GetHWND(), &cx, &cy,GetFont());

  float control_width, control_height, control_x, control_y;

  // Deal with default size (using -1 values)
  if (w1<=0)
    w1 = DEFAULT_ITEM_WIDTH;

  if (h1<=0)
    h1 = DEFAULT_ITEM_HEIGHT;

  control_x = (float)x1;
  control_y = (float)y1;
  control_width = (float)w1;
  control_height = (float)h1;

  // Calculations may have made size too small
  if (control_height <= 0)
    control_height = (float)DEFAULT_ITEM_HEIGHT;

  if (control_width <= 0)
    control_width = (float)DEFAULT_ITEM_WIDTH;

//  wxDebugMsg("About to set the listbox height to %d", (int)control_height);
  MoveWindow(hwnd, (int)control_x, (int)control_y,
                   (int)control_width, (int)control_height, TRUE);

}

// Windows-specific code to set the horizontal extent of
// the listbox, if necessary. If s is non-NULL, it's
// used to calculate the horizontal extent.
// Otherwise, all strings are used.
void wxListBox::SetHorizontalExtent(const wxString& s)
{
  // Only necessary if we want a horizontal scrollbar
  if (!(m_windowStyle & wxHSCROLL))
    return;
  TEXTMETRIC lpTextMetric;

  if (s != "")
  {
    int existingExtent = (int)SendMessage(hwnd, LB_GETHORIZONTALEXTENT, 0, 0L);
    HDC dc = GetWindowDC(hwnd);
    HFONT oldFont = 0;
    if (GetFont() && GetFont()->GetResourceHandle())
      oldFont = (HFONT) ::SelectObject(dc, (HFONT) GetFont()->GetResourceHandle());

    GetTextMetrics(dc, &lpTextMetric);
    SIZE extentXY;
    ::GetTextExtentPoint(dc, (LPSTR) (const char *)s, s.Length(), &extentXY);
    int extentX = (int)(extentXY.cx + lpTextMetric.tmAveCharWidth);
    
    if (oldFont)
      ::SelectObject(dc, oldFont);

    ReleaseDC(hwnd, dc);
    if (extentX > existingExtent)
      SendMessage(hwnd, LB_SETHORIZONTALEXTENT, LOWORD(extentX), 0L);
    return;
  }
  else
  {
    int largestExtent = 0;
    HDC dc = GetWindowDC(hwnd);
    HFONT oldFont = 0;
    if (GetFont() && GetFont()->GetResourceHandle())
      oldFont = (HFONT) ::SelectObject(dc, (HFONT) GetFont()->GetResourceHandle());

    GetTextMetrics(dc, &lpTextMetric);
    int i;
    for (i = 0; i < m_noItems; i++)
    {
      int len = (int)SendMessage(hwnd, LB_GETTEXT, i, (LONG)wxBuffer);
      wxBuffer[len] = 0;
      SIZE extentXY;
      ::GetTextExtentPoint(dc, (LPSTR)wxBuffer, len, &extentXY);
      int extentX = (int)(extentXY.cx + lpTextMetric.tmAveCharWidth);
      if (extentX > largestExtent)
        largestExtent = extentX;
    }
    if (oldFont)
      ::SelectObject(dc, oldFont);

    ReleaseDC(hwnd, dc);
    SendMessage(hwnd, LB_SETHORIZONTALEXTENT, LOWORD(largestExtent), 0L);
  }
}

void
wxListBox::InsertItems(int nItems, const wxString items[], int pos)
{
  int i;
  for (i = 0; i < nItems; i++)
    ListBox_InsertString(hwnd, i + pos, items[i]);
  m_noItems += nItems;

  #if wxUSE_OWNER_DRAWN
    if ( m_windowStyle & wxLB_OWNERDRAW ) {
      for ( i = 0; i < nItems; i++ ) {
        wxOwnerDrawn *pNewItem = CreateItem((size_t)(pos + i));
        pNewItem->SetName(items[i]);
        m_aItems.Insert(pNewItem, (size_t)(pos + i));
        ListBox_SetItemData(hwnd, i, pNewItem);
      }
    }
  #endif

  SetHorizontalExtent("");
}

void wxListBox::SetString(int N, const wxString& s)
{
  int sel = -1;
  if (!(m_windowStyle & wxLB_MULTIPLE) && !(m_windowStyle & wxLB_EXTENDED))
    sel = GetSelection();
  
  char *oldData = (char *)wxListBox::GetClientData(N);
  
  SendMessage(hwnd, LB_DELETESTRING, N, 0);

  int newN = N;
  if (N == (m_noItems - 1))
    newN = -1;
    
  SendMessage(hwnd, LB_INSERTSTRING, newN, (LPARAM) (const char *)s);
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

int wxListBox::Number (void) const
{
  return m_noItems;
}

// For single selection items only
wxString wxListBox::GetStringSelection (void) const
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
#if CTL3D
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
#if 0
  switch (nMsg)
  {
        case WM_INITDIALOG:
        case WM_ACTIVATE:
        case WM_SETFOCUS:
        case WM_KILLFOCUS:
	case WM_CREATE:
	case WM_PAINT:
	case WM_QUERYDRAGICON:
        case WM_SIZE:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_RBUTTONDBLCLK:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_MBUTTONDBLCLK:
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_LBUTTONDBLCLK:
        case WM_MOUSEMOVE:
        case WM_COMMAND:
        case WM_NOTIFY:
        case WM_DESTROY:
        case WM_MENUSELECT:
        case WM_INITMENUPOPUP:
        case WM_DRAWITEM:
        case WM_MEASUREITEM:
        case WM_KEYDOWN:
        case WM_KEYUP:
        case WM_CHAR: // Always an ASCII character
        case WM_HSCROLL:
        case WM_VSCROLL:
        case WM_CTLCOLORBTN:
        case WM_CTLCOLORDLG:
        case WM_CTLCOLORLISTBOX:
        case WM_CTLCOLORMSGBOX:
        case WM_CTLCOLORSCROLLBAR:
        case WM_CTLCOLORSTATIC:
        case WM_CTLCOLOREDIT:
        case WM_SYSCOLORCHANGE:
        case WM_ERASEBKGND:
        case WM_MDIACTIVATE:
        case WM_DROPFILES:
        case WM_QUERYENDSESSION:
        case WM_CLOSE:
        case WM_GETMINMAXINFO:
		case WM_NCHITTEST:
            return MSWDefWindowProc(nMsg, wParam, lParam );
    }
#endif

  return wxControl::MSWWindowProc(nMsg, wParam, lParam);
}

#if wxUSE_OWNER_DRAWN

// drawing
// -------

// space beneath/above each row in pixels
// "standard" checklistbox use 1 here, some might prefer 2. 0 is ugly.
#define OWNER_DRAWN_LISTBOX_EXTRA_SPACE    (1)

// the height is the same for all items
// ## should be changed for LBS_OWNERDRAWVARIABLE style listboxes
//    NB: can't forward this to wxListBoxItem because LB_SETITEMDATA
//        message is not yet sent when we get here!
bool wxListBox::MSWOnMeasure(WXMEASUREITEMSTRUCT *item)
{
  // only owner-drawn control should receive this message
  wxCHECK( ((m_windowStyle & wxLB_OWNERDRAW) == wxLB_OWNERDRAW), FALSE );

  MEASUREITEMSTRUCT *pStruct = (MEASUREITEMSTRUCT *)item;

  wxDC dc;
  dc.SetHDC((WXHDC)CreateIC("DISPLAY", NULL, NULL, 0));
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
  wxListBoxItem *pItem = (wxListBoxItem *)SendMessage(hwnd, LB_GETITEMDATA, 
                                                      pStruct->itemID, 0);

  wxCHECK( (int)pItem != LB_ERR, FALSE );

  wxDC dc;
  dc.SetHDC((WXHDC)pStruct->hDC, FALSE);
  wxRect rect(pStruct->rcItem.left, pStruct->rcItem.top,
              pStruct->rcItem.right - pStruct->rcItem.left, 
              pStruct->rcItem.bottom - pStruct->rcItem.top);

  return pItem->OnDrawItem(dc, rect, 
                           (wxOwnerDrawn::wxODAction)pStruct->itemAction,
                           (wxOwnerDrawn::wxODStatus)pStruct->itemState);
}

#endif
  // wxUSE_OWNER_DRAWN
