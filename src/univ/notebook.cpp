/////////////////////////////////////////////////////////////////////////////
// Name:        univ/notebook.cpp
// Purpose:     wxNotebook implementation
// Author:      Vadim Zeitlin
// Modified by:
// Created:     01.02.01
// RCS-ID:      $Id$
// Copyright:   (c) 2001 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#ifdef __GNUG__
    #pragma implementation "univnotebook.h"
#endif

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_NOTEBOOK

#include "wx/imaglist.h"
#include "wx/notebook.h"

#include "wx/spinbutt.h"

#include "wx/univ/renderer.h"

// ----------------------------------------------------------------------------
// macros
// ----------------------------------------------------------------------------

#define IS_VALID_PAGE(nPage) (((nPage) >= 0) && ((nPage) < GetPageCount()))

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

static const size_t INVALID_PAGE = (size_t)-1;

// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

class wxNotebookSpinBtn : public wxSpinButton
{
public:
    wxNotebookSpinBtn(wxNotebook *nb)
        : wxSpinButton(nb, -1,
                       wxDefaultPosition, wxDefaultSize,
                       nb->IsVertical() ? wxSP_VERTICAL : wxSP_HORIZONTAL)
    {
        m_nb = nb;
    }

protected:
    void OnSpin(wxSpinEvent& event)
    {
        m_nb->PerformAction(wxACTION_NOTEBOOK_GOTO, event.GetPosition());
    }

private:
    wxNotebook *m_nb;

    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(wxNotebookSpinBtn, wxSpinButton)
    EVT_SPIN(-1, wxNotebookSpinBtn::OnSpin)
END_EVENT_TABLE()

// ============================================================================
// implementation
// ============================================================================

IMPLEMENT_DYNAMIC_CLASS(wxNotebook, wxControl)
IMPLEMENT_DYNAMIC_CLASS(wxNotebookEvent, wxCommandEvent)

// ----------------------------------------------------------------------------
// wxNotebook creation
// ----------------------------------------------------------------------------

void wxNotebook::Init()
{
    m_sel = INVALID_PAGE;

    m_heightTab =
    m_widthMax = 0;

    m_firstVisible =
    m_lastVisible = 0;
    m_offset = 0;

    m_spinbtn = NULL;
}

bool wxNotebook::Create(wxWindow *parent,
                        wxWindowID id,
                        const wxPoint& pos,
                        const wxSize& size,
                        long style,
                        const wxString& name)
{
    if ( !wxControl::Create(parent, id, pos, size, style,
                            wxDefaultValidator, name) )
        return FALSE;

    m_sizePad = GetRenderer()->GetTabPadding();

    SetBestSize(size);

    CreateInputHandler(wxINP_HANDLER_NOTEBOOK);

    return TRUE;
}

// ----------------------------------------------------------------------------
// wxNotebook page titles and images
// ----------------------------------------------------------------------------

wxString wxNotebook::GetPageText(int nPage) const
{
    wxCHECK_MSG( IS_VALID_PAGE(nPage), _T(""), _T("invalid notebook page") );

    return m_titles[nPage];
}

bool wxNotebook::SetPageText(int nPage, const wxString& strText)
{
    wxCHECK_MSG( IS_VALID_PAGE(nPage), FALSE, _T("invalid notebook page") );

    if ( strText != m_titles[nPage] )
    {
        m_accels[nPage] = FindAccelIndex(strText, &m_titles[nPage]);

        if ( FixedSizeTabs() )
        {
            // it's enough to just reresh this one
            RefreshTab(nPage);
        }
        else // var width tabs
        {
            // we need to resize the tab to fit the new string
            ResizeTab(nPage);
        }
    }

    return TRUE;
}

int wxNotebook::GetPageImage(int nPage) const
{
    wxCHECK_MSG( IS_VALID_PAGE(nPage), -1, _T("invalid notebook page") );

    return m_images[nPage];
}

bool wxNotebook::SetPageImage(int nPage, int nImage)
{
    wxCHECK_MSG( IS_VALID_PAGE(nPage), FALSE, _T("invalid notebook page") );

    wxCHECK_MSG( m_imageList && nImage < m_imageList->GetImageCount(), FALSE,
                 _T("invalid image index in SetPageImage()") );

    if ( nImage != m_images[nPage] )
    {
        // if the item didn't have an icon before or, on the contrary, did have
        // it but has lost it now, its size will change - but if the icon just
        // changes, it won't
        bool tabSizeChanges = nImage == -1 || m_images[nPage] == -1;
        m_images[nPage] = nImage;

        if ( tabSizeChanges )
            RefreshAllTabs();
        else
            RefreshTab(nPage);
    }

    return TRUE;
}

wxNotebook::~wxNotebook()
{
}

// ----------------------------------------------------------------------------
// wxNotebook page switching
// ----------------------------------------------------------------------------

int wxNotebook::SetSelection(int nPage)
{
    wxCHECK_MSG( IS_VALID_PAGE(nPage), -1, _T("invalid notebook page") );

    if ( (size_t)nPage == m_sel )
    {
        // don't do anything if there is nothing to do
        return m_sel;
    }

    if ( m_sel != -1 )
    {
        RefreshTab(m_sel);

        m_pages[m_sel]->Hide();
    }

    m_sel = nPage;

    if ( m_sel != -1 ) // yes, this is impossible - but test nevertheless
    {
        if ( HasSpinBtn() )
        {
            // keep it in sync
            m_spinbtn->SetValue(m_sel);
        }

        if ( m_sel < m_firstVisible )
            ScrollTo(m_sel);
        else if ( m_sel >= m_lastVisible )
            ScrollLastTo(m_sel);
        else // no need to scroll
            RefreshTab(m_sel);

        m_pages[m_sel]->SetSize(GetPageRect());
        m_pages[m_sel]->Show();
    }

    return m_sel;
}

void wxNotebook::ChangePage(int nPage)
{
    wxCHECK_RET( IS_VALID_PAGE(nPage), _T("invalid notebook page") );

    if ( (size_t)nPage == m_sel )
    {
        // nothing to do
        return;
    }

    wxNotebookEvent event(wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGING, m_windowId);
    event.SetSelection(nPage);
    event.SetOldSelection(m_sel);
    event.SetEventObject(this);
    if ( GetEventHandler()->ProcessEvent(event) && !event.IsAllowed() )
    {
        // program doesn't allow the page change
        return;
    }

    SetSelection(nPage);

    event.SetEventType(wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED);
    GetEventHandler()->ProcessEvent(event);
}

// ----------------------------------------------------------------------------
// wxNotebook pages adding/deleting
// ----------------------------------------------------------------------------

bool wxNotebook::InsertPage(int nPage,
                            wxNotebookPage *pPage,
                            const wxString& strText,
                            bool bSelect,
                            int imageId)
{
    int nPages = GetPageCount();
    wxCHECK_MSG( nPage == nPages || IS_VALID_PAGE(nPage), FALSE,
                 _T("invalid notebook page in InsertPage()") );

    // modify the data
    m_pages.Insert(pPage, nPage);

    wxString label;
    m_accels.Insert(FindAccelIndex(strText, &label), nPage);
    m_titles.Insert(label, nPage);

    m_images.Insert(imageId, nPage);

    // cache the tab geometry here
    wxSize sizeTab = CalcTabSize(nPage);

    if ( sizeTab.y > m_heightTab )
        m_heightTab = sizeTab.y;

    if ( FixedSizeTabs() && sizeTab.x > m_widthMax )
        m_widthMax = sizeTab.x;

    m_widths.Insert(sizeTab.x, nPage);

    // spin button may appear if we didn't have it before - but even if we did,
    // its range should change, so update it unconditionally
    UpdateSpinBtn();

    // if the tab has just appeared, we have to relayout everything, otherwise
    // it's enough to just redraw the tabs
    if ( nPages == 0 )
    {
        // always select the first tab to have at least some selection
        bSelect = TRUE;

        Relayout();
    }
    else // not the first tab
    {
        RefreshAllTabs();
    }

    if ( bSelect )
    {
        SetSelection(nPage);
    }
    else // pages added to the notebook are initially hidden
    {
        pPage->Hide();
    }

    return TRUE;
}

bool wxNotebook::DeleteAllPages()
{
    if ( !wxNotebookBase::DeleteAllPages() )
        return FALSE;

    Relayout();

    return TRUE;
}

wxNotebookPage *wxNotebook::DoRemovePage(int nPage)
{
    wxCHECK_MSG( IS_VALID_PAGE(nPage), NULL, _T("invalid notebook page") );

    wxNotebookPage *page = m_pages[nPage];
    m_pages.RemoveAt(nPage);
    m_titles.RemoveAt(nPage);
    m_accels.RemoveAt(nPage);
    m_widths.RemoveAt(nPage);
    m_images.RemoveAt(nPage);

    // the spin button might not be needed any more
    if ( HasSpinBtn() )
    {
        UpdateSpinBtn();
    }

    if ( GetPageCount() )
    {
        // some tabs are still left, just redraw them
        RefreshAllTabs();

        SetSelection(0);
    }
    else // no more tabs left
    {
        // have to refresh everything
        Relayout();

        m_sel = INVALID_PAGE;
    }

    return page;
}

// ----------------------------------------------------------------------------
// wxNotebook drawing
// ----------------------------------------------------------------------------

void wxNotebook::RefreshTab(int page)
{
    wxCHECK_RET( IS_VALID_PAGE(page), _T("invalid notebook page") );

    wxRect r = GetTabRect(page);
    if ( (size_t)page == m_sel )
    {
        const wxSize indent = GetRenderer()->GetTabIndent();
        r.Inflate(2*indent.x, indent.y);
    }

    Refresh(TRUE, &r);
}

void wxNotebook::RefreshAllTabs()
{
    wxRect r = GetAllTabsRect();
    Refresh(TRUE, &r);
}

void wxNotebook::DoDrawTab(wxDC& dc, const wxRect& rect, size_t n)
{
    wxBitmap bmp;
    if ( HasImage(n) )
    {
        int image = m_images[n];

#ifdef __WXMSW__    // FIXME
        int w, h;
        m_imageList->GetSize(n, w, h);
        bmp.Create(w, h);
        wxMemoryDC dc;
        dc.SelectObject(bmp);
        dc.SetBackground(wxBrush(GetBackgroundColour(), wxSOLID));
        m_imageList->Draw(image, dc, 0, 0, wxIMAGELIST_DRAW_NORMAL, TRUE);
#else
        bmp = *m_imageList->GetBitmap(image);
#endif
    }

    int flags = 0;
    if ( n == m_sel )
    {
        flags |= wxCONTROL_SELECTED;

        if ( wxTheApp->IsActive() )
            flags |= wxCONTROL_FOCUSED;
    }

    GetRenderer()->DrawTab
                   (
                     dc,
                     rect,
                     GetTabOrientation(),
                     m_titles[n],
                     bmp,
                     flags,
                     m_accels[n]
                   );
}

void wxNotebook::DoDraw(wxControlRenderer *renderer)
{
    //wxRect rectUpdate = GetUpdateClientRect(); -- unused

    wxDC& dc = renderer->GetDC();
    dc.SetFont(GetFont());
    dc.SetTextForeground(GetForegroundColour());

    // redraw the border - it's simpler to always do it instead of checking
    // whether this needs to be done
    GetRenderer()->DrawBorder(dc, wxBORDER_RAISED, GetPagePart());

    wxRect rect = GetTabsPart();
    bool isVertical = IsVertical();

    wxRect rectSel;
    for ( size_t n = m_firstVisible; n < m_lastVisible; n++ )
    {
        GetTabSize(n, &rect.width, &rect.height);

        if ( n == m_sel )
        {
            // don't redraw it now as this tab has to be drawn over the other
            // ones as it takes more place and spills over to them
            rectSel = rect;
        }
        else // not selected tab
        {
            // unfortunately we can't do this because the selected tab hangs
            // over its neighbours and so we might need to refresh more tabs -
            // of course, we could still avoid rereshing some of them with more
            // complicated checks, but it doesn't seem too bad to refresh all
            // of them, I still don't see flicker, so leaving as is for now

            //if ( rectUpdate.Intersects(rect) )
            {
                DoDrawTab(dc, rect, n);
            }
            //else: doesn't need to be refreshed
        }

        // move the rect to the next tab
        if ( isVertical )
            rect.y += rect.height;
        else
            rect.x += rect.width;
    }

    if ( rectSel.width )
    {
        DoDrawTab(dc, rectSel, m_sel);
    }
}

// ----------------------------------------------------------------------------
// wxNotebook geometry
// ----------------------------------------------------------------------------

int wxNotebook::HitTest(const wxPoint& pt) const
{
    // first check that it is in this window at all
    if ( !GetClientRect().Inside(pt) )
    {
        return -1;
    }

    wxRect rectTabs = GetAllTabsRect();

    switch ( GetTabOrientation() )
    {
        default:
            wxFAIL_MSG(_T("unknown tab orientation"));
            // fall through

        case wxTOP:
            if ( pt.y > rectTabs.GetBottom() )
                return -1;
            break;

        case wxBOTTOM:
            if ( pt.y < rectTabs.y )
                return -1;
            break;

        case wxLEFT:
            if ( pt.x > rectTabs.GetRight() )
                return -1;
            break;

        case wxRIGHT:
            if ( pt.x < rectTabs.x )
                return -1;
            break;
    }

    for ( size_t n = m_firstVisible; n < m_lastVisible; n++ )
    {
        GetTabSize(n, &rectTabs.width, &rectTabs.height);

        if ( rectTabs.Inside(pt) )
            return n;

        // move the rectTabs to the next tab
        if ( IsVertical() )
            rectTabs.y += rectTabs.height;
        else
            rectTabs.x += rectTabs.width;
    }

    return -1;
}

bool wxNotebook::IsVertical() const
{
    wxDirection dir = GetTabOrientation();

    return dir == wxLEFT || dir == wxRIGHT;
}

wxDirection wxNotebook::GetTabOrientation() const
{
    long style = GetWindowStyle();
    if ( style & wxNB_BOTTOM )
        return wxBOTTOM;
    else if ( style & wxNB_RIGHT )
        return wxRIGHT;
    else if ( style & wxNB_LEFT )
        return wxLEFT;

    // wxNB_TOP == 0 so we don't have to test for it
    return wxTOP;
}

wxRect wxNotebook::GetTabRect(int page) const
{
    wxRect rect;
    wxCHECK_MSG( IS_VALID_PAGE(page), rect, _T("invalid notebook page") );

    // calc the size of this tab and of the preceding ones
    wxCoord widthThis, widthBefore;
    if ( FixedSizeTabs() )
    {
        widthThis = m_widthMax;
        widthBefore = page*m_widthMax;
    }
    else
    {
        widthBefore = 0;
        for ( int n = 0; n < page; n++ )
        {
            widthBefore += m_widths[n];
        }

        widthThis = m_widths[page];
    }

    rect = GetAllTabsRect();
    if ( IsVertical() )
    {
        rect.y = widthBefore;
        rect.height = widthThis;
    }
    else // horz
    {
        rect.x = widthBefore;
        rect.width = widthThis;
    }

    rect.x -= m_offset;

    return rect;
}

wxRect wxNotebook::GetAllTabsRect() const
{
    wxRect rect;

    if ( GetPageCount() )
    {
        const wxSize indent = GetRenderer()->GetTabIndent();
        wxSize size = GetClientSize();

        if ( IsVertical() )
        {
            rect.width = m_heightTab + indent.x;
            rect.x = GetTabOrientation() == wxLEFT ? 0 : size.x - rect.width;
            rect.y = 0;
            rect.height = size.y;
        }
        else // horz
        {
            rect.x = 0;
            rect.width = size.x;
            rect.height = m_heightTab + indent.y;
            rect.y = GetTabOrientation() == wxTOP ? 0 : size.y - rect.height;
        }
    }
    //else: no pages

    return rect;
}

wxRect wxNotebook::GetTabsPart() const
{
    wxRect rect = GetAllTabsRect();

    wxDirection dir = GetTabOrientation();

    const wxSize indent = GetRenderer()->GetTabIndent();
    if ( IsVertical() )
    {
        rect.x += indent.y;
        rect.y += indent.x;
    }
    else // horz
    {
        rect.x += indent.x;
        if ( dir == wxTOP )
        {
            rect.y += indent.y;
            rect.height -= indent.y;
        }
        else // wxBOTTOM
        {
            rect.height -= indent.y;
        }
    }

    return rect;
}

void wxNotebook::GetTabSize(int page, wxCoord *w, wxCoord *h) const
{
    wxCHECK_RET( w && h, _T("NULL pointer in GetTabSize") );

    if ( IsVertical() )
    {
        // width and height have inverted meaning
        wxCoord *tmp = w;
        w = h;
        h = tmp;
    }

    // height is always fixed
    *h = m_heightTab;

    // width may also be fixed and be the same for all tabs
    *w = GetTabWidth(page);
}

void wxNotebook::SetTabSize(const wxSize& sz)
{
    wxCHECK_RET( FixedSizeTabs(), _T("SetTabSize() ignored") );

    if ( IsVertical() )
    {
        m_heightTab = sz.x;
        m_widthMax = sz.y;
    }
    else // horz
    {
        m_widthMax = sz.x;
        m_heightTab = sz.y;
    }
}

wxSize wxNotebook::CalcTabSize(int page) const
{
    // NB: don't use m_widthMax, m_heightTab or m_widths here because this
    //     method is called to calculate them

    wxSize size;

    wxCHECK_MSG( IS_VALID_PAGE(page), size, _T("invalid notebook page") );

    GetTextExtent(m_titles[page], &size.x, &size.y);

    if ( HasImage(page) )
    {
        wxSize sizeImage;
        m_imageList->GetSize(m_images[page], sizeImage.x, sizeImage.y);

        size.x += sizeImage.x + 5; // FIXME: hard coded margin

        if ( sizeImage.y > size.y )
            size.y = sizeImage.y;
    }

    size.x += 2*m_sizePad.x;
    size.y += 2*m_sizePad.y;

    return size;
}

void wxNotebook::ResizeTab(int page)
{
    wxSize sizeTab = CalcTabSize(page);

    // we only need full relayout if the page size changes
    bool needsRelayout = FALSE;

    if ( IsVertical() )
    {
        // swap coordinates
        wxCoord tmp = sizeTab.x;
        sizeTab.x = sizeTab.y;
        sizeTab.y = tmp;
    }

    if ( sizeTab.y > m_heightTab )
    {
        needsRelayout = TRUE;

        m_heightTab = sizeTab.y;
    }

    m_widths[page] = sizeTab.x;

    if ( sizeTab.x > m_widthMax )
        m_widthMax = sizeTab.x;

    // the total of the tabs has changed too
    UpdateSpinBtn();

    if ( needsRelayout )
        Relayout();
    else
        RefreshAllTabs();
}

void wxNotebook::SetPadding(const wxSize& padding)
{
    if ( padding != m_sizePad )
    {
        m_sizePad = padding;

        Relayout();
    }
}

void wxNotebook::Relayout()
{
    if ( m_sel != -1 )
    {
        RefreshAllTabs();

        wxRect rectPage = GetPageRect();

        m_pages[m_sel]->SetSize(rectPage);
    }
}

wxRect wxNotebook::GetPagePart() const
{
    wxRect rectPage = GetClientRect();

    if ( GetPageCount() )
    {
        wxRect rectTabs = GetAllTabsRect();
        wxDirection dir = GetTabOrientation();
        if ( IsVertical() )
        {
            rectPage.width -= rectTabs.width;
            if ( dir == wxLEFT )
                rectPage.x += rectTabs.width;
        }
        else // horz
        {
            rectPage.height -= rectTabs.height;
            if ( dir == wxTOP )
                rectPage.y += rectTabs.height;
        }
    }
    //else: no pages at all

    return rectPage;
}

wxRect wxNotebook::GetPageRect() const
{
    wxRect rect = GetPagePart();

    // leave space for the border
    wxRect rectBorder = GetRenderer()->GetBorderDimensions(wxBORDER_RAISED);

    // FIXME: hardcoded +2!
    rect.Inflate(-(rectBorder.x + rectBorder.width + 2),
                 -(rectBorder.y + rectBorder.height + 2));

    return rect;
}

wxSize wxNotebook::GetSizeForPage(const wxSize& size) const
{
    wxSize sizeNb = size;
    wxRect rect = GetAllTabsRect();
    if ( IsVertical() )
        sizeNb.x += rect.width;
    else
        sizeNb.y += rect.height;

    return sizeNb;
}

void wxNotebook::SetPageSize(const wxSize& size)
{
    SetClientSize(GetSizeForPage(size));
}

// ----------------------------------------------------------------------------
// wxNotebook spin button
// ----------------------------------------------------------------------------

bool wxNotebook::HasSpinBtn() const
{
    return m_spinbtn && m_spinbtn->IsShown();
}

size_t wxNotebook::CalcLastVisibleTab() const
{
    wxCoord width = GetClientSize().x + m_offset;
    wxRect rect = GetTabsPart();
    bool isVertical = IsVertical();

    size_t n,
           count = GetPageCount();
    for ( n = m_firstVisible; n < count; n++ )
    {
        GetTabSize(n, &rect.width, &rect.height);
        if ( rect.GetRight() > width )
        {
            break;
        }

        // move the rect to the next tab
        if ( isVertical )
            rect.y += rect.height;
        else
            rect.x += rect.width;
    }

    // if even the first tab isn't fully visible, we can't do much but show it
    // nevertheless, so still return 0 in this case
    return n;
}

void wxNotebook::UpdateSpinBtn()
{
    // first decide if we need a spin button
    m_lastVisible = CalcLastVisibleTab();
    while ( m_lastVisible == (size_t)GetPageCount() )
    {
        if ( m_firstVisible > 0 )
        {
            m_offset -= GetTabWidth(m_firstVisible--);
        }
        else
        {
            // we don't need spin button as all tabs are visible after we
            // rewound to the bginning
            break;
        }
    }

    if ( m_lastVisible < (size_t)GetPageCount() )
    {
        if ( !m_spinbtn )
        {
            // create it once only
            m_spinbtn = new wxNotebookSpinBtn(this);

            // set the correct value to keep it in sync
            m_spinbtn->SetValue(m_sel);

            // and position it correctly
            PositionSpinBtn();
        }

        m_spinbtn->Show();
        m_spinbtn->SetRange(0, GetPageCount() - 1);
    }
    else // we don't need spin button
    {
        if ( m_spinbtn )
        {
            m_spinbtn->Hide();
        }
    }
}

void wxNotebook::PositionSpinBtn()
{
    if ( !m_spinbtn )
        return;

    wxCoord wBtn, hBtn;
    m_spinbtn->GetSize(&wBtn, &hBtn);

    wxRect rectTabs = GetAllTabsRect();

    wxCoord x, y;
    switch ( GetTabOrientation() )
    {
        default:
            wxFAIL_MSG(_T("unknown tab orientation"));
            // fall through

        case wxTOP:
            x = rectTabs.GetRight() - wBtn;
            y = rectTabs.GetBottom() - hBtn;
            break;

        case wxBOTTOM:
            x = rectTabs.GetRight() - wBtn;
            y = rectTabs.GetTop();
            break;

        case wxLEFT:
            x = rectTabs.GetRight() - wBtn;
            y = rectTabs.GetBottom() - hBtn;
            break;

        case wxRIGHT:
            x = rectTabs.GetLeft();
            y = rectTabs.GetBottom() - hBtn;
            break;
    }

    m_spinbtn->Move(x, y);
}

// ----------------------------------------------------------------------------
// wxNotebook scrolling
// ----------------------------------------------------------------------------

void wxNotebook::ScrollTo(int page)
{
    wxCHECK_RET( IS_VALID_PAGE(page), _T("invalid notebook page") );

    // set the first visible tab and offset (easy)
    m_firstVisible = (size_t)page;
    m_offset = 0;
    for ( size_t n = 0; n < m_firstVisible; n++ )
    {
        m_offset += GetTabWidth(n);
    }

    // find the last visible tab
    wxCoord width = GetClientSize().x,
            cur = 0;

    size_t count = GetPageCount();
    for ( m_lastVisible = m_firstVisible;
          m_lastVisible < count;
          m_lastVisible++ )
    {
        cur += GetTabWidth(m_lastVisible);

        // cur now contains the width of all tabs between m_firstVisible and
        // m_lastVisible
        if ( cur > width )
            break;
    }

    RefreshAllTabs();
}

void wxNotebook::ScrollLastTo(int page)
{
    wxCHECK_RET( IS_VALID_PAGE(page), _T("invalid notebook page") );

    // go backwards until we find the first tab which can be made visible
    // withotu hiding the given one
    wxCoord widthAll = GetClientSize().x,
            widthTabs = GetTabWidth(page);

    m_firstVisible = page;
    while ( (m_firstVisible > 0) && (widthTabs <= widthAll) )
    {
        widthTabs += GetTabWidth(--m_firstVisible);
    }

    if ( widthTabs > widthAll )
    {
        // take one step back (that it is forward)
        m_firstVisible++;
    }

    // go to it
    ScrollTo(m_firstVisible);
}

// ----------------------------------------------------------------------------
// wxNotebook sizing/moving
// ----------------------------------------------------------------------------

wxSize wxNotebook::DoGetBestClientSize() const
{
    // calculate the max page size
    wxSize size(0, 0);

    size_t count = GetPageCount();
    for ( size_t n = 0; n < count; n++ )
    {
        wxSize sizePage = m_pages[n]->GetSize();

        if ( size.x < sizePage.x )
            size.x = sizePage.x;
        if ( size.y < sizePage.y )
            size.y = sizePage.y;
    }

    return GetSizeForPage(size);
}

void wxNotebook::DoMoveWindow(int x, int y, int width, int height)
{
    wxControl::DoMoveWindow(x, y, width, height);

    // move the spin ctrl too (NOP if it doesn't exist)
    PositionSpinBtn();
}

void wxNotebook::DoSetSize(int x, int y,
                           int width, int height,
                           int sizeFlags)
{
    wxControl::DoSetSize(x, y, width, height, sizeFlags);

    Relayout();
}

// ----------------------------------------------------------------------------
// wxNotebook input processing
// ----------------------------------------------------------------------------

bool wxNotebook::PerformAction(const wxControlAction& action,
                               long numArg,
                               const wxString& strArg)
{
    if ( action == wxACTION_NOTEBOOK_NEXT )
        ChangePage(GetNextPage(TRUE));
    else if ( action == wxACTION_NOTEBOOK_PREV )
        ChangePage(GetNextPage(FALSE));
    else if ( action == wxACTION_NOTEBOOK_GOTO )
        ChangePage((int)numArg);
    else
        return wxControl::PerformAction(action, numArg, strArg);

    return TRUE;
}

// ----------------------------------------------------------------------------
// wxStdNotebookInputHandler
// ----------------------------------------------------------------------------

wxStdNotebookInputHandler::wxStdNotebookInputHandler(wxInputHandler *inphand)
                         : wxStdInputHandler(inphand)
{
}

bool wxStdNotebookInputHandler::HandleKey(wxControl *control,
                                          const wxKeyEvent& event,
                                          bool pressed)
{
    // ignore the key releases
    if ( pressed )
    {
        wxNotebook *notebook = wxStaticCast(control, wxNotebook);

        int page = 0;
        wxControlAction action;
        switch ( event.GetKeyCode() )
        {
            case WXK_UP:
                if ( notebook->IsVertical() )
                    action = wxACTION_NOTEBOOK_PREV;
                break;

            case WXK_LEFT:
                if ( !notebook->IsVertical() )
                    action = wxACTION_NOTEBOOK_PREV;
                break;

            case WXK_DOWN:
                if ( notebook->IsVertical() )
                    action = wxACTION_NOTEBOOK_NEXT;
                break;

            case WXK_RIGHT:
                if ( !notebook->IsVertical() )
                    action = wxACTION_NOTEBOOK_NEXT;
                break;

            case WXK_HOME:
                action = wxACTION_NOTEBOOK_GOTO;
                // page = 0; -- already has this value
                break;

            case WXK_END:
                action = wxACTION_NOTEBOOK_GOTO;
                page = notebook->GetPageCount() - 1;
                break;
        }

        if ( !!action )
        {
            return control->PerformAction(action, page);
        }
    }

    return wxStdInputHandler::HandleKey(control, event, pressed);
}

bool wxStdNotebookInputHandler::HandleMouse(wxControl *control,
                                            const wxMouseEvent& event)
{
    if ( event.ButtonDown(1) )
    {
        wxNotebook *notebook = wxStaticCast(control, wxNotebook);
        int page = notebook->HitTest(event.GetPosition());
        if ( page != -1 )
        {
            control->PerformAction(wxACTION_NOTEBOOK_GOTO, page);

            return FALSE;
        }
    }

    return wxStdInputHandler::HandleMouse(control, event);
}

bool wxStdNotebookInputHandler::HandleMouseMove(wxControl *control,
                                                const wxMouseEvent& event)
{
    return wxStdInputHandler::HandleMouseMove(control, event);
}

bool wxStdNotebookInputHandler::HandleFocus(wxControl *control,
                                            const wxFocusEvent& event)
{
    // buttons change appearance when they get/lose focus, so return TRUE to
    // refresh
    return TRUE;
}

bool wxStdNotebookInputHandler::HandleActivation(wxControl *control,
                                                 bool activated)
{
    // the default button changes appearance when the app is [de]activated, so
    // return TRUE to refresh
    return TRUE;
}

#endif // wxUSE_NOTEBOOK

