/////////////////////////////////////////////////////////////////////////////
// Name:        univ/listbox.cpp
// Purpose:     wxListBox implementation
// Author:      Vadim Zeitlin
// Modified by:
// Created:     30.08.00
// RCS-ID:      $Id$
// Copyright:   (c) 2000 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#ifdef __GNUG__
    #pragma implementation "univlistbox.h"
#endif

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_LISTBOX

#ifndef WX_PRECOMP
    #include "wx/log.h"

    #include "wx/dcclient.h"
    #include "wx/listbox.h"
    #include "wx/validate.h"
#endif

#include "wx/univ/renderer.h"
#include "wx/univ/inphand.h"
#include "wx/univ/theme.h"

// ============================================================================
// implementation of wxListBox
// ============================================================================

IMPLEMENT_DYNAMIC_CLASS(wxListBox, wxControl)

BEGIN_EVENT_TABLE(wxListBox, wxListBoxBase)
    EVT_SIZE(wxListBox::OnSize)

    EVT_IDLE(wxListBox::OnIdle)
END_EVENT_TABLE()

// ----------------------------------------------------------------------------
// construction
// ----------------------------------------------------------------------------

void wxListBox::Init()
{
    // will be calculated later when needed
    m_lineHeight = 0;
    m_itemsPerPage = 0;
    m_maxWidth = 0;

    // no items hence no current item
    m_current = -1;
    m_currentChanged = FALSE;

    // no need to update anything initially
    m_updateCount = 0;

    // no scrollbars to show nor update
    m_updateScrollbarX =
    m_showScrollbarX =
    m_updateScrollbarY =
    m_showScrollbarY = FALSE;
}

bool wxListBox::Create(wxWindow *parent,
                       wxWindowID id,
                       const wxPoint &pos,
                       const wxSize &size,
                       int n,
                       const wxString choices[],
                       long style,
                       const wxValidator& validator,
                       const wxString &name)
{
    // for compatibility accept both the new and old styles - they mean the
    // same thing for us
    if ( style & wxLB_ALWAYS_SB )
        style |= wxALWAYS_SHOW_SB;

    if ( !wxControl::Create(parent, id, pos, size, style, wxDefaultValidator, name) )
        return FALSE;

    SetWindow(this);

    SetBackgroundColour(*wxWHITE);

    if ( style & wxLB_SORT )
        m_strings = wxArrayString(TRUE /* auto sort */);

    Set(n, choices);

    SetBestSize(size);

    return TRUE;
}

wxListBox::~wxListBox()
{
}

// ----------------------------------------------------------------------------
// adding/inserting strings
// ----------------------------------------------------------------------------

int wxListBox::DoAppend(const wxString& item)
{
    size_t index = m_strings.Add(item);
    m_itemsClientData.Insert(NULL, index);

    m_updateScrollbarY = TRUE;

    if ( HasHorzScrollbar() )
    {
        // has the max width increased?
        wxClientDC dc(this);
        wxCoord width;
        dc.GetTextExtent(item, &width, NULL);
        if ( width > m_maxWidth )
        {
            m_maxWidth = width;
            m_updateScrollbarX = TRUE;
        }
    }

    RefreshItem(m_strings.GetCount() - 1);

    return index;
}

void wxListBox::DoInsertItems(const wxArrayString& items, int pos)
{
    // the position of the item being added to a sorted listbox can't be
    // specified
    wxCHECK_RET( !IsSorted(), _T("can't insert items into sorted listbox") );

    size_t count = items.GetCount();
    for ( size_t n = 0; n < count; n++ )
    {
        m_strings.Insert(items[n], pos + n);
        m_itemsClientData.Insert(NULL, pos + n);
    }

    // the number of items has changed
    m_updateScrollbarY = TRUE;

    // the max width also might have changed - just recalculate it instead of
    // keeping track of it here, this is probably more efficient for a typical
    // use pattern
    m_maxWidth = 0;
    m_updateScrollbarX = TRUE;

    RefreshItems(pos, count);
}

void wxListBox::DoSetItems(const wxArrayString& items, void **clientData)
{
    DoClear();

    size_t count = items.GetCount();
    if ( !count )
        return;

    m_strings.Alloc(count);
    m_itemsClientData.Alloc(count);
    for ( size_t n = 0; n < count; n++ )
    {
        size_t index = m_strings.Add(items[n]);
        m_itemsClientData.Insert(clientData ? clientData[n] : NULL, index);
    }

    m_updateScrollbarY = TRUE;

    RefreshAll();
}

void wxListBox::SetString(int n, const wxString& s)
{
    if ( HasHorzScrollbar() )
    {
        // we need to update m_maxWidth as changing the string may cause the
        // horz scrollbar [dis]appear
        wxCoord widthOld;
        GetTextExtent(m_strings[n], &widthOld, NULL);
        m_strings[n] = s;

        // has the max length changed?
        wxCoord width;
        GetTextExtent(s, &width, NULL);

        // it might have increased if the new string is long
        if ( width > m_maxWidth )
        {
            m_maxWidth = width;
            m_updateScrollbarX = TRUE;
        }
        // or also decreased if the old string was the longest one
        else if ( (width < m_maxWidth) && (widthOld == m_maxWidth) )
        {
            m_maxWidth = 0;
            m_updateScrollbarX = TRUE;
        }
    }
    else // no horz scrollbar
    {
        m_strings[n] = s;
    }

    RefreshItem(n);
}

// ----------------------------------------------------------------------------
// removing strings
// ----------------------------------------------------------------------------

void wxListBox::DoClear()
{
    m_strings.Clear();

    if ( HasClientObjectData() )
    {
        size_t count = m_itemsClientData.GetCount();
        for ( size_t n = 0; n < count; n++ )
        {
            delete m_itemsClientData[n];
        }
    }

    m_itemsClientData.Clear();
}

void wxListBox::Clear()
{
    DoClear();

    m_updateScrollbarY = TRUE;

    RefreshAll();
}

void wxListBox::Delete(int n)
{
    wxCHECK_RET( n < GetCount(), _T("invalid index in wxListBox::Delete") );

    m_strings.RemoveAt(n);

    if ( HasClientObjectData() )
    {
        delete m_itemsClientData[n];
    }

    m_itemsClientData.RemoveAt(n);

    m_updateScrollbarY = TRUE;

    RefreshItems(n, GetCount() - n);
}

// ----------------------------------------------------------------------------
// client data handling
// ----------------------------------------------------------------------------

void wxListBox::DoSetItemClientData(int n, void* clientData)
{
    m_itemsClientData[n] = clientData;
}

void *wxListBox::DoGetItemClientData(int n) const
{
    return m_itemsClientData[n];
}

void wxListBox::DoSetItemClientObject(int n, wxClientData* clientData)
{
    m_itemsClientData[n] = clientData;
}

wxClientData* wxListBox::DoGetItemClientObject(int n) const
{
    return (wxClientData *)m_itemsClientData[n];
}

// ----------------------------------------------------------------------------
// selection
// ----------------------------------------------------------------------------

void wxListBox::SetSelection(int n, bool select)
{
    if ( select )
    {
        if ( m_selections.Index(n) == wxNOT_FOUND )
        {
            m_selections.Add(n);

            RefreshItem(n);
        }
        //else: already selected
    }
    else // unselect
    {
        int index = m_selections.Index(n);
        if ( index != wxNOT_FOUND )
        {
            m_selections.RemoveAt(index);

            RefreshItem(n);
        }
        //else: not selected
    }
}

int wxListBox::GetSelection() const
{
    wxCHECK_MSG( !HasMultipleSelection(), -1,
                 _T("use wxListBox::GetSelections for ths listbox") );

    return m_selections.IsEmpty() ? -1 : m_selections[0];
}

int wxListBox::GetSelections(wxArrayInt& selections) const
{
    selections = m_selections;

    return m_selections.GetCount();
}

// ----------------------------------------------------------------------------
// refresh logic: we use delayed refreshing which allows to avoid multiple
// refreshes (and hence flicker) in case when several listbox items are
// added/deleted/changed subsequently
// ----------------------------------------------------------------------------

void wxListBox::Refresh(bool eraseBackground, const wxRect *rect)
{
    if ( rect )
        wxLogTrace(_T("listbox"), _T("Refreshing (%d, %d)-(%d, %d)"),
                   rect->x, rect->y,
                   rect->x + rect->width, rect->y + rect->height);
    else
        wxLogTrace(_T("listbox"), _T("Refreshing all"));

    wxControl::Refresh(eraseBackground, rect);
}

void wxListBox::RefreshItems(int from, int count)
{
    switch ( m_updateCount )
    {
        case 0:
            m_updateFrom = from;
            m_updateCount = count;
            break;

        case -1:
            // we refresh everything anyhow
            break;

        default:
            // add these items to the others which we have to refresh
            if ( m_updateFrom < from )
            {
                count += from - m_updateFrom;
                if ( m_updateCount < count )
                    m_updateCount = count;
            }
            else // m_updateFrom >= from
            {
                m_updateCount = wxMax(m_updateCount,
                                      from + count - m_updateFrom);
                m_updateFrom = from;
            }
    }
}

void wxListBox::RefreshItem(int n)
{
    switch ( m_updateCount )
    {
        case 0:
            // refresh this item only
            m_updateFrom = n;
            m_updateCount = 1;
            break;

        case -1:
            // we refresh everything anyhow
            break;

        default:
            // add this item to the others which we have to refresh
            if ( m_updateFrom < n )
            {
                if ( m_updateCount < n - m_updateFrom + 1 )
                    m_updateCount = n - m_updateFrom + 1;
            }
            else // n <= m_updateFrom
            {
                m_updateCount += m_updateFrom - n;
                m_updateFrom = n;
            }
    }
}

void wxListBox::RefreshAll()
{
    m_updateCount = -1;
}

void wxListBox::OnIdle(wxIdleEvent& event)
{
    if ( m_updateScrollbarY || m_updateScrollbarX )
    {
        wxSize size = GetClientSize();

        // is our height enough to show all items?
        int nLines = GetCount();
        wxCoord lineHeight = GetLineHeight();
        bool showScrollbarY = nLines*lineHeight > size.y;

        // check the width too if required
        wxCoord charWidth, maxWidth;
        bool showScrollbarX;
        if ( HasHorzScrollbar() )
        {
            charWidth = GetCharWidth();
            maxWidth = GetMaxWidth();
            showScrollbarX = maxWidth > size.x;
        }
        else // never show it
        {
            charWidth = maxWidth = 0;
            showScrollbarX = FALSE;
        }

        // reset scrollbars if something changed
        if ( (showScrollbarY != m_showScrollbarY) ||
             (showScrollbarX != m_showScrollbarX) )
        {
            int x, y;
            GetViewStart(&x, &y);
            SetScrollbars(charWidth, lineHeight,
                          showScrollbarX
                            ? (maxWidth + 2*charWidth - 1) / charWidth
                            : 0,
                          showScrollbarY
                            ? nLines
                            : 0,
                          x, y);

            m_showScrollbarX = showScrollbarX;
            m_showScrollbarY = showScrollbarY;
        }

        m_updateScrollbarX = FALSE;
        m_updateScrollbarY = FALSE;
    }

    if ( m_currentChanged )
    {
        EnsureVisible();

        m_currentChanged = FALSE;
    }

    if ( m_updateCount )
    {
        // only refresh the items which must be refreshed
        if ( m_updateCount == -1 )
        {
            // refresh all
            wxLogTrace(_T("listbox"), _T("Refreshing all"));

            Refresh();
        }
        else
        {
            wxRect rect = GetClientRect();
            rect.y += m_updateFrom*GetLineHeight();
            rect.height = m_updateCount*GetLineHeight();
            CalcScrolledPosition(rect.x, rect.y, &rect.x, &rect.y);

            wxLogTrace(_T("listbox"), _T("Refreshing items %d..%d (%d-%d)"),
                       m_updateFrom, m_updateFrom + m_updateCount - 1,
                       rect.GetTop(), rect.GetBottom());

            Refresh(TRUE, &rect);
        }

        m_updateCount = 0;
    }
}

// ----------------------------------------------------------------------------
// drawing
// ----------------------------------------------------------------------------

wxBorder wxListBox::GetDefaultBorder() const
{
    return wxBORDER_SUNKEN;
}

void wxListBox::DoDraw(wxControlRenderer *renderer)
{
#if 0
    // draw the border first
    if ( m_showScrollbarY )
    {
        // we need to draw a border around the client area
        renderer->GetRect().width -= GetScrollbar(wxVERTICAL)->GetSize().x;
    }

    // the base class version does it for us
    wxControl::DoDraw(renderer);
#endif

    // adjust the DC to account for scrolling
    wxDC& dc = renderer->GetDC();
    PrepareDC(dc);

    // get the items which must be redrawn
#if 1
    wxCoord lineHeight = GetLineHeight();
    wxRegion rgnUpdate = GetUpdateRegion();
    rgnUpdate.Intersect(GetClientRect());
    //dc.SetClippingRegion(rgnUpdate);
    wxRect rectUpdate = rgnUpdate.GetBox();

    int yTop, yBottom;
    CalcUnscrolledPosition(0, rectUpdate.GetTop(), NULL, &yTop);
    CalcUnscrolledPosition(0, rectUpdate.GetBottom(), NULL, &yBottom);
    size_t itemFirst = yTop / lineHeight,
           itemLast = (yBottom + lineHeight - 1) / lineHeight,
           itemMax = m_strings.GetCount();

    if ( itemFirst >= itemMax )
        return;

    if ( itemLast > itemMax )
        itemLast = itemMax;
#else
    size_t itemFirst = 0,
           itemLast = m_strings.GetCount();
#endif

    // do draw them
    wxLogTrace(_T("listbox"), _T("Repainting items %d..%d"),
               itemFirst, itemLast);

    renderer->DrawItems(this, itemFirst, itemLast);
}

// ----------------------------------------------------------------------------
// size calculations
// ----------------------------------------------------------------------------

bool wxListBox::SetFont(const wxFont& font)
{
    if ( !wxControl::SetFont(font) )
        return FALSE;

    CalcItemsPerPage();

    RefreshAll();

    return TRUE;
}

void wxListBox::CalcItemsPerPage()
{
    m_lineHeight = GetRenderer()->
                    GetListboxItemHeight(wxClientDC(this).GetCharHeight());
    m_itemsPerPage = GetClientSize().y / m_lineHeight;
}

int wxListBox::GetItemsPerPage() const
{
    if ( !m_itemsPerPage )
    {
        wxConstCast(this, wxListBox)->CalcItemsPerPage();
    }

    return m_itemsPerPage;
}

wxCoord wxListBox::GetLineHeight() const
{
    if ( !m_lineHeight )
    {
        wxConstCast(this, wxListBox)->CalcItemsPerPage();
    }

    return m_lineHeight;
}

wxCoord wxListBox::GetMaxWidth() const
{
    if ( m_maxWidth == -1 )
    {
        wxCoord width;
        size_t count = m_strings.GetCount();
        for ( size_t n =0; n < count; n++ )
        {
            GetTextExtent(m_strings[n], &width, NULL);
            if ( width > m_maxWidth )
            {
                wxConstCast(this, wxListBox)->m_maxWidth = width;
            }
        }
    }

    return m_maxWidth;
}

void wxListBox::OnSize(wxSizeEvent& event)
{
    // recalculate the number of items per page
    CalcItemsPerPage();

    event.Skip();
}

void wxListBox::DoSetFirstItem(int n)
{
    SetCurrentItem(n);
}

wxSize wxListBox::DoGetBestClientSize() const
{
    wxClientDC dc(wxConstCast(this, wxListBox));

    wxCoord width = 0,
            height = 0;

    size_t count = m_strings.GetCount();
    for ( size_t n = 0; n < count; n++ )
    {
        wxCoord w,h;
        dc.GetTextExtent(m_strings[n], &w, &h);

        if ( w > width )
            width = w;
        if ( h > height )
            height = h;
    }

    // if the listbox is empty, still give it some non zero (even if
    // arbitrary) size - otherwise, leave small margin around the strings
    if ( !width )
        width = 100;
    else
        width += 3*dc.GetCharWidth();

    if ( !height )
        height = dc.GetCharHeight();

    // we need the height of the entire listbox, not just of one line
    height *= wxMax(count, 7);

    return wxSize(width, height);
}

// ----------------------------------------------------------------------------
// listbox actions
// ----------------------------------------------------------------------------

bool wxListBox::SendEvent(int item, wxEventType type)
{
    wxCommandEvent event(type, m_windowId);
    event.SetEventObject(this);

    int n;
    if ( m_selections.GetCount() > 0 )
    {
        n = m_selections[0];
        if ( HasClientObjectData() )
            event.SetClientObject(GetClientObject(n));
        else if ( HasClientUntypedData() )
            event.SetClientData(GetClientData(n));

        event.SetString(GetString(n));
    }
    else // no selection
    {
        n = -1;
    }

    event.m_commandInt = n;

    return GetEventHandler()->ProcessEvent(event);
}

void wxListBox::SetCurrentItem(int n)
{
    if ( n != m_current )
    {
        if ( m_current != -1 )
            RefreshItem(m_current);

        m_current = n;

        if ( m_current != -1 )
        {
            if ( !HasMultipleSelection() )
            {
                // for a single selection listbox, the current item is always
                // the one selected
                Select(TRUE);
            }

            m_currentChanged = TRUE;

            RefreshItem(n);
        }
    }
    //else: nothing to do
}

void wxListBox::EnsureVisible()
{
    if ( !m_showScrollbarY )
    {
        // nothing to do - everything is shown anyhow
        return;
    }

    int first;
    GetViewStart(0, &first);
    if ( first > m_current )
    {
        // we need to scroll upwards, so make the current item appear on top
        // of the shown range
        Scroll(0, m_current);
    }
    else
    {
        int last = first + GetClientSize().y / GetLineHeight() - 1;
        if ( last < m_current )
        {
            // scroll down: the current item appears at the bottom of the
            // range
            Scroll(0, m_current - (last - first));
        }
    }
}

void wxListBox::ChangeCurrent(int diff)
{
    int current = m_current == -1 ? 0 : m_current;

    current += diff;

    int last = GetCount() - 1;
    if ( current < 0 )
        current = 0;
    else if ( current > last )
        current = last;

    SetCurrentItem(current);
}

void wxListBox::Select(bool sel)
{
    if ( sel && !HasMultipleSelection() )
    {
        // deselect the old item first
        int selOld = GetSelection();
        if ( selOld != -1 )
        {
            SetSelection(selOld, FALSE);
        }
    }

    if ( m_current != -1 )
    {
        // [de]select the new one
        SetSelection(m_current, sel);

        if ( sel )
        {
            SendEvent(m_current, wxEVT_COMMAND_LISTBOX_SELECTED);
        }
    }
}

void wxListBox::Activate()
{
    if ( m_current != -1 )
    {
        Select();

        SendEvent(m_current, wxEVT_COMMAND_LISTBOX_DOUBLECLICKED);
    }
}

bool wxListBox::PerformAction(const wxControlAction& action,
                              long numArg,
                              const wxString& strArg)
{
    if ( action == wxACTION_LISTBOX_SETFOCUS )
        SetCurrentItem(numArg);
    else if ( action == wxACTION_LISTBOX_ACTIVATE )
        Activate();
    else if ( action == wxACTION_LISTBOX_TOGGLE )
        Select(!IsSelected(m_current));
    else if ( action == wxACTION_LISTBOX_SELECT )
        Select(TRUE);
    else if ( action == wxACTION_LISTBOX_UNSELECT )
        Select(FALSE);
    else if ( action == wxACTION_LISTBOX_MOVEDOWN )
        ChangeCurrent(1);
    else if ( action == wxACTION_LISTBOX_MOVEUP )
        ChangeCurrent(-1);
    else if ( action == wxACTION_LISTBOX_PAGEDOWN )
        ChangeCurrent(GetItemsPerPage());
    else if ( action == wxACTION_LISTBOX_PAGEUP )
        ChangeCurrent(-GetItemsPerPage());
    else if ( action == wxACTION_LISTBOX_START )
        SetCurrentItem(0);
    else if ( action == wxACTION_LISTBOX_END )
        SetCurrentItem(GetCount() - 1);
    else
        return wxControl::PerformAction(action, numArg, strArg);

    return TRUE;
}

// ============================================================================
// implementation of wxStdListboxInputHandler
// ============================================================================

wxStdListboxInputHandler::wxStdListboxInputHandler(wxInputHandler *handler)
                        : wxStdInputHandler(handler)
{
    m_winCapture = NULL;
    m_btnCapture = 0;
}

int wxStdListboxInputHandler::HitTest(const wxListBox *lbox,
                                      const wxMouseEvent& event)
{
    int y;
    lbox->CalcUnscrolledPosition(0, event.GetPosition().y, NULL, &y);
    int item = y / lbox->GetLineHeight();

    return item < lbox->GetCount() ? item : -1;
}

bool wxStdListboxInputHandler::HandleKey(wxControl *control,
                                         const wxKeyEvent& event,
                                         bool pressed)
{
    // we're only interested in the (normal) key presses
    if ( pressed && !event.AltDown() )
    {
        wxControlAction action;
        switch ( event.GetKeyCode() )
        {
            // movement
            case WXK_UP:    action = wxACTION_LISTBOX_MOVEUP; break;
            case WXK_DOWN:  action = wxACTION_LISTBOX_MOVEDOWN; break;
            case WXK_PRIOR: action = wxACTION_LISTBOX_PAGEUP; break;
            case WXK_NEXT:  action = wxACTION_LISTBOX_PAGEDOWN; break;
            case WXK_HOME:  action = wxACTION_LISTBOX_START; break;
            case WXK_END:   action = wxACTION_LISTBOX_END; break;

            // selection
            case WXK_SPACE: action = wxACTION_LISTBOX_TOGGLE; break;
            case WXK_RETURN:action = wxACTION_LISTBOX_ACTIVATE; break;
        }

        if ( !!action )
        {
            control->PerformAction(action);

            return TRUE;
        }
    }

    return wxStdInputHandler::HandleKey(control, event, pressed);
}

bool wxStdListboxInputHandler::HandleMouse(wxControl *control,
                                           const wxMouseEvent& event)
{
    // single and extended listboxes behave similarly with respect to the
    // mouse events: for both of them clicking the item selects or toggles it,
    // but multiple selection listboxes are different: the item is focused
    // when clicked and only toggled when the button is released
    if ( (m_btnCapture &&
          (control->GetWindowStyle() & wxLB_MULTIPLE) &&
          event.ButtonUp(m_btnCapture))
            || event.ButtonDown()
            || event.LeftDClick() )
    {
        wxListBox *lbox = wxStaticCast(control, wxListBox);
        int item = HitTest(lbox, event);
        if ( item != -1 )
        {
            wxControlAction action;
            if ( m_btnCapture && event.ButtonUp(m_btnCapture) )
            {
                if ( m_winCapture )
                {
                    m_winCapture->ReleaseMouse();
                    m_winCapture = NULL;
                    m_btnCapture = 0;
                }
                else
                {
                    // this is not supposed to happen - if we get here, the
                    // even is from the mouse button which had been pressed
                    // before and we must have captured the mouse back then
                    wxFAIL_MSG(_T("logic error in listbox mosue handling"));
                }

                action = wxACTION_LISTBOX_TOGGLE;
            }
            else if ( event.ButtonDown() )
            {
                if ( lbox->HasMultipleSelection() )
                {
                    if ( lbox->GetWindowStyle() & wxLB_MULTIPLE )
                    {
                        // capture the mouse to track the selected item
                        m_winCapture = lbox;
                        m_winCapture->CaptureMouse();
                        m_btnCapture = event.LeftDown()
                                        ? 1
                                        : event.RightDown()
                                            ? 3
                                            : 2;
                    }
                    else
                    {
                        action = wxACTION_LISTBOX_TOGGLE;
                    }
                }
                else // single selection
                {
                    action = wxACTION_LISTBOX_SELECT;
                }

                // in any case, clicking an item makes it the current one
                lbox->PerformAction(wxACTION_LISTBOX_SETFOCUS, item);
            }
            else // event.LeftDClick()
            {
                action = wxACTION_LISTBOX_ACTIVATE;
            }

            if ( !!action )
            {
                lbox->PerformAction(action, item);
            }

            return TRUE;
        }
        //else: click outside the item area, ignore
    }

    return wxStdInputHandler::HandleMouse(control, event);
}

bool wxStdListboxInputHandler::HandleMouseMove(wxControl *control,
                                           const wxMouseEvent& event)
{
    if ( !m_winCapture || (event.GetEventObject() != m_winCapture) )
    {
        // we don't react to this
        return FALSE;
    }

    // TODO: not yet... should track the mouse outside and start an auto
    //       scroll timer - but this should be probably done in
    //       wxScrolledWindow itself (?)
    if ( !event.Moving() )
        return FALSE;

    wxListBox *lbox = wxStaticCast(control, wxListBox);
    int item = HitTest(lbox, event);
    if ( item == -1 )
    {
        // mouse is below the last item
        return FALSE;
    }

    lbox->PerformAction(wxACTION_LISTBOX_SETFOCUS, item);

    return TRUE;
}

#endif // wxUSE_LISTBOX
