///////////////////////////////////////////////////////////////////////////////
// Name:        msw/notebook.cpp
// Purpose:     implementation of wxNotebook
// Author:      Vadim Zeitlin
// Modified by:
// Created:     11.06.98
// RCS-ID:      $Id$
// Copyright:   (c) 1998 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// Licence:     wxWindows license
///////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "notebook.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_NOTEBOOK

// wxWindows
#ifndef WX_PRECOMP
  #include  "wx/string.h"
#endif  // WX_PRECOMP

#include  "wx/log.h"
#include  "wx/imaglist.h"
#include  "wx/event.h"
#include  "wx/control.h"
#include  "wx/notebook.h"

#include  "wx/msw/private.h"

// Windows standard headers
#ifndef   __WIN95__
  #error  "wxNotebook is only supported Windows 95 and above"
#endif    //Win95

#include  <windowsx.h>  // for SetWindowFont

#ifndef __TWIN32__
    #ifdef __GNUWIN32_OLD__
        #include "wx/msw/gnuwin32/extra.h"
    #endif
#endif

#if defined(__WIN95__) && !((defined(__GNUWIN32_OLD__) || defined(__TWIN32__)) && !defined(__CYGWIN10__))
    #include <commctrl.h>
#endif

// ----------------------------------------------------------------------------
// macros
// ----------------------------------------------------------------------------

// check that the page index is valid
#define IS_VALID_PAGE(nPage) (((nPage) >= 0) && ((nPage) < GetPageCount()))

// hide the ugly cast
#define m_hwnd    (HWND)GetHWND()

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// This is a work-around for missing defines in gcc-2.95 headers
#ifndef TCS_RIGHT
    #define TCS_RIGHT       0x0002
#endif

#ifndef TCS_VERTICAL
    #define TCS_VERTICAL    0x0080
#endif

#ifndef TCS_BOTTOM
    #define TCS_BOTTOM      TCS_RIGHT
#endif

// ----------------------------------------------------------------------------
// event table
// ----------------------------------------------------------------------------

DEFINE_EVENT_TYPE(wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGING)

BEGIN_EVENT_TABLE(wxNotebook, wxControl)
    EVT_NOTEBOOK_PAGE_CHANGED(-1, wxNotebook::OnSelChange)

    EVT_SIZE(wxNotebook::OnSize)

    EVT_SET_FOCUS(wxNotebook::OnSetFocus)

    EVT_NAVIGATION_KEY(wxNotebook::OnNavigationKey)
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC_CLASS(wxNotebook, wxControl)
IMPLEMENT_DYNAMIC_CLASS(wxNotebookEvent, wxNotifyEvent)

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxNotebook construction
// ----------------------------------------------------------------------------

// common part of all ctors
void wxNotebook::Init()
{
  m_imageList = NULL;
  m_nSelection = -1;
}

// default for dynamic class
wxNotebook::wxNotebook()
{
  Init();
}

// the same arguments as for wxControl
wxNotebook::wxNotebook(wxWindow *parent,
                       wxWindowID id,
                       const wxPoint& pos,
                       const wxSize& size,
                       long style,
                       const wxString& name)
{
  Init();

  Create(parent, id, pos, size, style, name);
}

// Create() function
bool wxNotebook::Create(wxWindow *parent,
                        wxWindowID id,
                        const wxPoint& pos,
                        const wxSize& size,
                        long style,
                        const wxString& name)
{
  // base init
  if ( !CreateBase(parent, id, pos, size, style, wxDefaultValidator, name) )
      return FALSE;

  // colors and font
  m_backgroundColour = wxColour(GetSysColor(COLOR_BTNFACE));
  m_foregroundColour = *wxBLACK;

  // style
  m_windowStyle = style | wxTAB_TRAVERSAL;

  long tabStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP | TCS_TABS;

  if ( m_windowStyle & wxCLIP_SIBLINGS )
    tabStyle |= WS_CLIPSIBLINGS;
  if (m_windowStyle & wxCLIP_CHILDREN)
    tabStyle |= WS_CLIPCHILDREN;
  if ( m_windowStyle & wxTC_MULTILINE )
    tabStyle |= TCS_MULTILINE;
  if ( m_windowStyle & wxBORDER )
    tabStyle |= WS_BORDER;
  if (m_windowStyle & wxNB_FIXEDWIDTH)
    tabStyle |= TCS_FIXEDWIDTH ;
  if (m_windowStyle & wxNB_BOTTOM)
    tabStyle |= TCS_RIGHT;
  if (m_windowStyle & wxNB_LEFT)
    tabStyle |= TCS_VERTICAL;
  if (m_windowStyle & wxNB_RIGHT)
    tabStyle |= TCS_VERTICAL|TCS_RIGHT;


  if ( !MSWCreate(GetId(), GetParent(), WC_TABCONTROL,
                  this, NULL, pos.x, pos.y, size.x, size.y,
                  tabStyle, NULL, 0) )
  {
    return FALSE;
  }

  // Not all compilers recognise SetWindowFont
  ::SendMessage(GetHwnd(), WM_SETFONT,
                (WPARAM)::GetStockObject(DEFAULT_GUI_FONT), TRUE);


  if ( parent != NULL )
    parent->AddChild(this);

  SubclassWin(m_hWnd);

  return TRUE;
}

// ----------------------------------------------------------------------------
// wxNotebook accessors
// ----------------------------------------------------------------------------

int wxNotebook::GetPageCount() const
{
  // consistency check
  wxASSERT( (int)m_pages.Count() == TabCtrl_GetItemCount(m_hwnd) );

  return m_pages.Count();
}

int wxNotebook::GetRowCount() const
{
  return TabCtrl_GetRowCount(m_hwnd);
}

int wxNotebook::SetSelection(int nPage)
{
  wxCHECK_MSG( IS_VALID_PAGE(nPage), -1, wxT("notebook page out of range") );

  ChangePage(m_nSelection, nPage);

  return TabCtrl_SetCurSel(m_hwnd, nPage);
}

bool wxNotebook::SetPageText(int nPage, const wxString& strText)
{
  wxCHECK_MSG( IS_VALID_PAGE(nPage), FALSE, wxT("notebook page out of range") );

  TC_ITEM tcItem;
  tcItem.mask = TCIF_TEXT;
  tcItem.pszText = (wxChar *)strText.c_str();

  return TabCtrl_SetItem(m_hwnd, nPage, &tcItem) != 0;
}

wxString wxNotebook::GetPageText(int nPage) const
{
  wxCHECK_MSG( IS_VALID_PAGE(nPage), wxT(""), wxT("notebook page out of range") );

  wxChar buf[256];
  TC_ITEM tcItem;
  tcItem.mask = TCIF_TEXT;
  tcItem.pszText = buf;
  tcItem.cchTextMax = WXSIZEOF(buf);

  wxString str;
  if ( TabCtrl_GetItem(m_hwnd, nPage, &tcItem) )
    str = tcItem.pszText;

  return str;
}

int wxNotebook::GetPageImage(int nPage) const
{
  wxCHECK_MSG( IS_VALID_PAGE(nPage), -1, wxT("notebook page out of range") );

  TC_ITEM tcItem;
  tcItem.mask = TCIF_IMAGE;

  return TabCtrl_GetItem(m_hwnd, nPage, &tcItem) ? tcItem.iImage : -1;
}

bool wxNotebook::SetPageImage(int nPage, int nImage)
{
  wxCHECK_MSG( IS_VALID_PAGE(nPage), FALSE, wxT("notebook page out of range") );

  TC_ITEM tcItem;
  tcItem.mask = TCIF_IMAGE;
  tcItem.iImage = nImage;

  return TabCtrl_SetItem(m_hwnd, nPage, &tcItem) != 0;
}

void wxNotebook::SetImageList(wxImageList* imageList)
{
  wxNotebookBase::SetImageList(imageList);

  if ( imageList )
  {
    TabCtrl_SetImageList(m_hwnd, (HIMAGELIST)imageList->GetHIMAGELIST());
  }
}

// ----------------------------------------------------------------------------
// wxNotebook size settings
// ----------------------------------------------------------------------------

void wxNotebook::SetPageSize(const wxSize& size)
{
    // transform the page size into the notebook size
    RECT rc;
    rc.left =
    rc.top = 0;
    rc.right = size.x;
    rc.bottom = size.y;

    TabCtrl_AdjustRect(GetHwnd(), TRUE, &rc);

    // and now set it
    SetSize(rc.right - rc.left, rc.bottom - rc.top);
}

void wxNotebook::SetPadding(const wxSize& padding)
{
    TabCtrl_SetPadding(GetHwnd(), padding.x, padding.y);
}

// Windows-only at present. Also, you must use the wxNB_FIXEDWIDTH
// style.
void wxNotebook::SetTabSize(const wxSize& sz)
{
    ::SendMessage(GetHwnd(), TCM_SETITEMSIZE, 0, MAKELPARAM(sz.x, sz.y));
}

// ----------------------------------------------------------------------------
// wxNotebook operations
// ----------------------------------------------------------------------------

// remove one page from the notebook
bool wxNotebook::DeletePage(int nPage)
{
  wxCHECK_MSG( IS_VALID_PAGE(nPage), FALSE, wxT("notebook page out of range") );

  if ( m_nSelection == nPage ) {
      // advance selection backwards - the page being deleted shouldn't be left
      // selected
      AdvanceSelection(FALSE);
  }

  TabCtrl_DeleteItem(m_hwnd, nPage);

  delete m_pages[nPage];
  m_pages.RemoveAt(nPage);

  if ( m_pages.IsEmpty() ) {
      // no selection if the notebook became empty
      m_nSelection = -1;
  }
  else
      m_nSelection = TabCtrl_GetCurSel(m_hwnd);


  return TRUE;
}

// remove one page from the notebook, without deleting
wxNotebookPage *wxNotebook::DoRemovePage(int nPage)
{
  wxNotebookPage *pageRemoved = wxNotebookBase::DoRemovePage(nPage);
  if ( !pageRemoved )
      return NULL;

  TabCtrl_DeleteItem(m_hwnd, nPage);

  if ( m_pages.IsEmpty() )
    m_nSelection = -1;
  else
    m_nSelection = TabCtrl_GetCurSel(m_hwnd);

  return pageRemoved;
}

// remove all pages
bool wxNotebook::DeleteAllPages()
{
  int nPageCount = GetPageCount();
  int nPage;
  for ( nPage = 0; nPage < nPageCount; nPage++ )
    delete m_pages[nPage];

  m_pages.Clear();

  TabCtrl_DeleteAllItems(m_hwnd);

  m_nSelection = -1;

  return TRUE;
}

// add a page to the notebook
bool wxNotebook::AddPage(wxNotebookPage *pPage,
                         const wxString& strText,
                         bool bSelect,
                         int imageId)
{
  return InsertPage(GetPageCount(), pPage, strText, bSelect, imageId);
}

// same as AddPage() but does it at given position
bool wxNotebook::InsertPage(int nPage,
                            wxNotebookPage *pPage,
                            const wxString& strText,
                            bool bSelect,
                            int imageId)
{
  wxASSERT( pPage != NULL );
  wxCHECK( IS_VALID_PAGE(nPage) || nPage == GetPageCount(), FALSE );

  // do add the tab to the control

  // init all fields to 0
  TC_ITEM tcItem;
  memset(&tcItem, 0, sizeof(tcItem));

  if ( imageId != -1 )
  {
    tcItem.mask |= TCIF_IMAGE;
    tcItem.iImage  = imageId;
  }

  if ( !strText.IsEmpty() )
  {
    tcItem.mask |= TCIF_TEXT;
    tcItem.pszText = (wxChar *)strText.c_str(); // const_cast
  }

  if ( TabCtrl_InsertItem(m_hwnd, nPage, &tcItem) == -1 ) {
    wxLogError(wxT("Can't create the notebook page '%s'."), strText.c_str());

    return FALSE;
  }

  // if the inserted page is before the selected one, we must update the
  // index of the selected page
  if ( nPage <= m_nSelection )
  {
    // one extra page added
    m_nSelection++;
  }

  // save the pointer to the page
  m_pages.Insert(pPage, nPage);

  // don't show pages by default (we'll need to adjust their size first)
  HWND hwnd = GetWinHwnd(pPage);
  SetWindowLong(hwnd, GWL_STYLE, GetWindowLong(hwnd, GWL_STYLE) & ~WS_VISIBLE);

  // this updates internal flag too - otherwise it will get out of sync
  pPage->Show(FALSE);

  // fit the notebook page to the tab control's display area
  RECT rc;
  rc.left = rc.top = 0;
  GetSize((int *)&rc.right, (int *)&rc.bottom);
  TabCtrl_AdjustRect(m_hwnd, FALSE, &rc);
  pPage->SetSize(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);

  // some page should be selected: either this one or the first one if there is
  // still no selection
  int selNew = -1;
  if ( bSelect )
    selNew = nPage;
  else if ( m_nSelection == -1 )
    selNew = 0;

  if ( selNew != -1 )
    SetSelection(selNew);

  return TRUE;
}

// ----------------------------------------------------------------------------
// wxNotebook callbacks
// ----------------------------------------------------------------------------

void wxNotebook::OnSize(wxSizeEvent& event)
{
  // fit the notebook page to the tab control's display area
  RECT rc;
  rc.left = rc.top = 0;
  GetSize((int *)&rc.right, (int *)&rc.bottom);

  TabCtrl_AdjustRect(m_hwnd, FALSE, &rc);

  int width = rc.right - rc.left,
      height = rc.bottom - rc.top;
  size_t nCount = m_pages.Count();
  for ( size_t nPage = 0; nPage < nCount; nPage++ ) {
    wxNotebookPage *pPage = m_pages[nPage];
    pPage->SetSize(rc.left, rc.top, width, height);
  }

  event.Skip();
}

void wxNotebook::OnSelChange(wxNotebookEvent& event)
{
  // is it our tab control?
  if ( event.GetEventObject() == this )
  {
      int sel = event.GetOldSelection();
      if ( sel != -1 )
        m_pages[sel]->Show(FALSE);

      sel = event.GetSelection();
      if ( sel != -1 )
      {
        wxNotebookPage *pPage = m_pages[sel];
        pPage->Show(TRUE);
        pPage->SetFocus();
      }

      m_nSelection = sel;
  }

  // we want to give others a chance to process this message as well
  event.Skip();
}

void wxNotebook::OnSetFocus(wxFocusEvent& event)
{
    // this function is only called when the focus is explicitly set (i.e. from
    // the program) to the notebook - in this case we don't need the
    // complicated OnNavigationKey() logic because the programmer knows better
    // what [s]he wants

    // set focus to the currently selected page if any
    if ( m_nSelection != -1 )
        m_pages[m_nSelection]->SetFocus();

    event.Skip();
}

void wxNotebook::OnNavigationKey(wxNavigationKeyEvent& event)
{
    if ( event.IsWindowChange() ) {
        // change pages
        AdvanceSelection(event.GetDirection());
    }
    else {
        // we get this event in 2 cases
        //
        // a) one of our pages might have generated it because the user TABbed
        // out from it in which case we should propagate the event upwards and
        // our parent will take care of setting the focus to prev/next sibling
        //
        // or
        //
        // b) the parent panel wants to give the focus to us so that we
        // forward it to our selected page. We can't deal with this in
        // OnSetFocus() because we don't know which direction the focus came
        // from in this case and so can't choose between setting the focus to
        // first or last panel child

        wxWindow *parent = GetParent();
        if ( event.GetEventObject() == parent )
        {
            // no, it doesn't come from child, case (b): forward to a page
            if ( m_nSelection != -1 )
            {
                // so that the page knows that the event comes from it's parent
                // and is being propagated downwards
                event.SetEventObject(this);

                wxWindow *page = m_pages[m_nSelection];
                if ( !page->GetEventHandler()->ProcessEvent(event) )
                {
                    page->SetFocus();
                }
                //else: page manages focus inside it itself
            }
            else
            {
                // we have no pages - still have to give focus to _something_
                SetFocus();
            }
        }
        else
        {
            // it comes from our child, case (a), pass to the parent
            if ( parent ) {
                event.SetCurrentFocus(this);
                parent->GetEventHandler()->ProcessEvent(event);
            }
        }
    }
}

// ----------------------------------------------------------------------------
// wxNotebook base class virtuals
// ----------------------------------------------------------------------------

// override these 2 functions to do nothing: everything is done in OnSize

void wxNotebook::SetConstraintSizes(bool WXUNUSED(recurse))
{
  // don't set the sizes of the pages - their correct size is not yet known
  wxControl::SetConstraintSizes(FALSE);
}

bool wxNotebook::DoPhase(int WXUNUSED(nPhase))
{
  return TRUE;
}

bool wxNotebook::MSWOnNotify(int idCtrl, WXLPARAM lParam, WXLPARAM* result)
{
  wxNotebookEvent event(wxEVT_NULL, m_windowId);

  NMHDR* hdr = (NMHDR *)lParam;
  switch ( hdr->code ) {
    case TCN_SELCHANGE:
      event.SetEventType(wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED);
      break;

    case TCN_SELCHANGING:
      event.SetEventType(wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGING);
      break;

    default:
      return wxControl::MSWOnNotify(idCtrl, lParam, result);
  }

  event.SetSelection(TabCtrl_GetCurSel(m_hwnd));
  event.SetOldSelection(m_nSelection);
  event.SetEventObject(this);
  event.SetInt(idCtrl);

  bool processed = GetEventHandler()->ProcessEvent(event);
  *result = !event.IsAllowed();
  return processed;
}

// ----------------------------------------------------------------------------
// wxNotebook helper functions
// ----------------------------------------------------------------------------

// generate the page changing and changed events, hide the currently active
// panel and show the new one
void wxNotebook::ChangePage(int nOldSel, int nSel)
{
  // MT-FIXME should use a real semaphore
  static bool s_bInsideChangePage = FALSE;

  // when we call ProcessEvent(), our own OnSelChange() is called which calls
  // this function - break the infinite loop
  if ( s_bInsideChangePage )
    return;

  // it's not an error (the message may be generated by the tab control itself)
  // and it may happen - just do nothing
  if ( nSel == nOldSel )
    return;

  s_bInsideChangePage = TRUE;

  wxNotebookEvent event(wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGING, m_windowId);
  event.SetSelection(nSel);
  event.SetOldSelection(nOldSel);
  event.SetEventObject(this);
  if ( GetEventHandler()->ProcessEvent(event) && !event.IsAllowed() )
  {
    // program doesn't allow the page change
    s_bInsideChangePage = FALSE;
    return;
  }

  event.SetEventType(wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED);
  GetEventHandler()->ProcessEvent(event);

  s_bInsideChangePage = FALSE;
}

#endif // wxUSE_NOTEBOOK
