///////////////////////////////////////////////////////////////////////////////
// Name:        univ/window.cpp
// Purpose:     implementation of extra wxWindow methods for wxUniv port
// Author:      Vadim Zeitlin
// Modified by:
// Created:     06.08.00
// RCS-ID:      $Id$
// Copyright:   (c) 2000 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// Licence:     wxWindows license
///////////////////////////////////////////////////////////////////////////////

// ===========================================================================
// declarations
// ===========================================================================

// ---------------------------------------------------------------------------
// headers
// ---------------------------------------------------------------------------

#ifdef __GNUG__
    #pragma implementation "univwindow.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/app.h"
    #include "wx/window.h"
    #include "wx/dcclient.h"
    #include "wx/dcmemory.h"
    #include "wx/event.h"
    #include "wx/scrolbar.h"
#endif // WX_PRECOMP

#include "wx/univ/renderer.h"
#include "wx/univ/theme.h"

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// event tables
// ----------------------------------------------------------------------------

// we can't use wxWindowNative here as it won't be expanded inside the macro
#if defined(__WXMSW__)
    IMPLEMENT_DYNAMIC_CLASS(wxWindow, wxWindowMSW)
#elif defined(__WXGTK__)
    IMPLEMENT_DYNAMIC_CLASS(wxWindow, wxWindowGTK)
#endif

BEGIN_EVENT_TABLE(wxWindow, wxWindowNative)
    EVT_SIZE(wxWindow::OnSize)

    EVT_PAINT(wxWindow::OnPaint)
    EVT_ERASE_BACKGROUND(wxWindow::OnErase)
END_EVENT_TABLE()

// ----------------------------------------------------------------------------
// creation
// ----------------------------------------------------------------------------

void wxWindow::Init()
{
    m_scrollbarVert =
    m_scrollbarHorz = (wxScrollBar *)NULL;

    m_isCurrent = FALSE;

    m_renderer = wxTheme::Get()->GetRenderer();
}

bool wxWindow::Create(wxWindow *parent,
                      wxWindowID id,
                      const wxPoint& pos,
                      const wxSize& size,
                      long style,
                      const wxString& name)
{
    if ( !wxWindowNative::Create(parent, id, pos, size,
                                 style | wxCLIP_CHILDREN, name) )
    {
        return FALSE;
    }

    // if we should always have the scrollbar, do show it
    if ( GetWindowStyle() & wxALWAYS_SHOW_SB )
    {
        m_scrollbarVert = new wxScrollBar(this, -1,
                                          wxDefaultPosition, wxDefaultSize,
                                          wxSB_VERTICAL);
    }

    // the colours/fonts are default
    m_hasBgCol =
    m_hasFgCol =
    m_hasFont = FALSE;

    return TRUE;
}

// ----------------------------------------------------------------------------
// background pixmap
// ----------------------------------------------------------------------------

void wxWindow::SetBackground(const wxBitmap& bitmap,
                              int alignment,
                              wxStretch stretch)
{
    m_bitmapBg = bitmap;
    m_alignBgBitmap = alignment;
    m_stretchBgBitmap = stretch;
}

const wxBitmap& wxWindow::GetBackgroundBitmap(int *alignment,
                                               wxStretch *stretch) const
{
    if ( m_bitmapBg.Ok() )
    {
        if ( alignment )
            *alignment = m_alignBgBitmap;
        if ( stretch )
            *stretch = m_stretchBgBitmap;
    }

    return m_bitmapBg;
}

// ----------------------------------------------------------------------------
// painting
// ----------------------------------------------------------------------------

// the event handler executed when the window background must be painted
void wxWindow::OnErase(wxEraseEvent& event)
{
    if ( !m_renderer )
    {
        event.Skip();

        return;
    }

    wxControlRenderer renderer(this, *event.GetDC(), m_renderer);

    DoDrawBackground(&renderer);

    // if we have both scrollbars, we also have a square in the corner between
    // them which we must paint
    if ( m_scrollbarVert && m_scrollbarHorz )
    {
        wxSize size = GetSize();
        wxRect rectClient = GetClientRect(),
               rectBorder = m_renderer->GetBorderDimensions(GetBorder());

        wxRect rectCorner;
        rectCorner.x = rectClient.GetRight() + 1;
        rectCorner.y = rectClient.GetBottom() + 1;
        rectCorner.SetRight(size.x - rectBorder.width);
        rectCorner.SetBottom(size.y - rectBorder.height);

        m_renderer->DrawScrollCorner(*event.GetDC(), rectCorner);
    }
}

// the event handler executed when the window must be repainted
void wxWindow::OnPaint(wxPaintEvent& event)
{
    if ( !m_renderer )
    {
        // it is a native control which paints itself
        event.Skip();
    }
    else
    {
        // get the DC to use and create renderer on it
        wxPaintDC dc(this);
        wxControlRenderer renderer(this, dc, m_renderer);

        // first, draw the border
        DoDrawBorder(&renderer);

        // and then draw the control
        DoDraw(&renderer);
    }
}

bool wxWindow::DoDrawBackground(wxControlRenderer *renderer)
{
    renderer->DrawBackgroundBitmap();

    return TRUE;
}

void wxWindow::DoDrawBorder(wxControlRenderer *renderer)
{
    renderer->DrawBorder();
}

void wxWindow::DoDraw(wxControlRenderer *renderer)
{
}

// ----------------------------------------------------------------------------
// state flags
// ----------------------------------------------------------------------------

bool wxWindow::IsFocused() const
{
    wxWindow *self = wxConstCast(this, wxWindow);
    return self->FindFocus() == self;
}

bool wxWindow::IsPressed() const
{
    return FALSE;
}

bool wxWindow::IsDefault() const
{
    return FALSE;
}

bool wxWindow::IsCurrent() const
{
    return m_isCurrent;
}

void wxWindow::SetCurrent(bool doit)
{
    m_isCurrent = doit;
}

int wxWindow::GetStateFlags() const
{
    int flags = 0;
    if ( !IsEnabled() )
        flags |= wxCONTROL_DISABLED;

    // the following states are only possible if our application is active - if
    // it is not, even our default/focused controls shouldn't appear as such
    if ( wxTheApp->IsActive() )
    {
        if ( IsCurrent() )
            flags |= wxCONTROL_CURRENT;
        if ( IsFocused() )
            flags |= wxCONTROL_FOCUSED;
        if ( IsPressed() )
            flags |= wxCONTROL_PRESSED;
        if ( IsDefault() )
            flags |= wxCONTROL_ISDEFAULT;
    }

    return flags;
}

// ----------------------------------------------------------------------------
// size
// ----------------------------------------------------------------------------

void wxWindow::OnSize(wxSizeEvent& event)
{
    if ( m_scrollbarVert || m_scrollbarHorz )
    {
        PositionScrollbars();
    }

    event.Skip();
}

wxSize wxWindow::DoGetBestSize() const
{
    return AdjustSize(DoGetBestClientSize());
}

wxSize wxWindow::DoGetBestClientSize() const
{
    return wxWindowNative::DoGetBestSize();
}

wxSize wxWindow::AdjustSize(const wxSize& size) const
{
    wxSize sz = size;
    if ( m_renderer )
        m_renderer->AdjustSize(&sz, this);
    return sz;
}

wxPoint wxWindow::GetClientAreaOrigin() const
{
    wxPoint pt = wxWindowBase::GetClientAreaOrigin();

    if ( m_renderer )
        pt += m_renderer->GetBorderDimensions(GetBorder()).GetPosition();

    return pt;
}

void wxWindow::DoGetClientSize(int *width, int *height) const
{
    wxWindowNative::DoGetClientSize(width, height);

    // we assume that the scrollbars are positioned correctly (by a previous
    // call to PositionScrollbars()) here

    wxRect rectBorder;
    if ( m_renderer )
        rectBorder = m_renderer->GetBorderDimensions(GetBorder());

    wxSize size = GetSize();

    if ( width )
    {
        if ( m_scrollbarVert )
            *width -= size.x - m_scrollbarVert->GetPosition().x;
        else
            *width -= rectBorder.width;

        *width -= rectBorder.x;
    }

    if ( height )
    {
        if ( m_scrollbarHorz )
            *height -= size.y - m_scrollbarHorz->GetPosition().y;
        else
            *height -= rectBorder.height;

        *height -= rectBorder.y;
    }
}

void wxWindow::DoSetClientSize(int width, int height)
{
    // take into account the borders
    wxRect rectBorder = m_renderer->GetBorderDimensions(GetBorder());
    width += rectBorder.x;
    height += rectBorder.y;

    // and the scrollbars (as they may be offset into the border, use the
    // scrollbar position, not size - this supposes that PositionScrollbars()
    // had been called before)
    wxSize size = GetSize();
    if ( m_scrollbarVert )
        width += size.x - m_scrollbarVert->GetPosition().x;
    else
        width += rectBorder.width;

    if ( m_scrollbarHorz )
        height += size.y - m_scrollbarHorz->GetPosition().y;
    else
        height += rectBorder.height;

    wxWindowNative::DoSetClientSize(width, height);
}

// ----------------------------------------------------------------------------
// scrolling: we implement it entirely ourselves except for ScrollWindow()
// function which is supposed to be (efficiently) implemented by the native
// window class
// ----------------------------------------------------------------------------

void wxWindow::PositionScrollbars()
{
    // do not use GetClientSize/Rect as it relies on the scrollbars being
    // correctly positioned

    wxSize size = GetSize();
    wxBorder border = GetBorder();
    wxRect rectBorder = m_renderer->GetBorderDimensions(border);
    bool inside = m_renderer->AreScrollbarsInsideBorder();

    int height = m_scrollbarHorz ? m_scrollbarHorz->GetSize().y : 0;
    int width = m_scrollbarVert ? m_scrollbarVert->GetSize().x : 0;

    wxRect rectBar;
    if ( m_scrollbarVert )
    {
        rectBar.x = size.x - width;
        if ( inside )
           rectBar.x -= rectBorder.width;
        rectBar.width = width;
        rectBar.y = 0;
        if ( inside )
            rectBar.y += rectBorder.y;
        rectBar.height = size.y - height;
        if ( inside )
            rectBar.height -= rectBorder.y + rectBorder.height;

        m_scrollbarVert->SetSize(rectBar, wxSIZE_NO_ADJUSTMENTS);
    }

    if ( m_scrollbarHorz )
    {
        rectBar.y = size.y - height;
        if ( inside )
            rectBar.y -= rectBorder.height;
        rectBar.height = height;
        rectBar.x = 0;
        if ( inside )
            rectBar.x += rectBorder.x;
        rectBar.width = size.x - width;
        if ( inside )
            rectBar.width -= rectBorder.x + rectBorder.width;

        m_scrollbarHorz->SetSize(rectBar, wxSIZE_NO_ADJUSTMENTS);
    }
}

void wxWindow::SetScrollbar(int orient,
                            int pos,
                            int pageSize,
                            int range,
                            bool refresh)
{
    wxScrollBar *scrollbar = GetScrollbar(orient);
    if ( range )
    {
        if ( !scrollbar )
        {
            // create it
            scrollbar = new wxScrollBar(this, -1,
                                        wxDefaultPosition, wxDefaultSize,
                                        orient & wxVERTICAL ? wxSB_VERTICAL
                                                            : wxSB_HORIZONTAL);
            if ( orient & wxVERTICAL )
                m_scrollbarVert = scrollbar;
            else
                m_scrollbarHorz = scrollbar;

            PositionScrollbars();
        }
        else if ( GetWindowStyle() & wxALWAYS_SHOW_SB )
        {
            // we might have disabled it before
            scrollbar->Enable();
        }

        scrollbar->SetScrollbar(pos, pageSize, range, pageSize, refresh);
    }
    else // no range means no scrollbar
    {
        if ( scrollbar )
        {
            if ( GetWindowStyle() & wxALWAYS_SHOW_SB )
            {
                scrollbar->Disable();
            }
            else
            {
                delete scrollbar;

                if ( orient & wxVERTICAL )
                    m_scrollbarVert = NULL;
                else
                    m_scrollbarHorz = NULL;
            }
        }
    }
}

void wxWindow::SetScrollPos(int orient, int pos, bool refresh)
{
    wxScrollBar *scrollbar = GetScrollbar(orient);
    wxCHECK_RET( scrollbar, _T("no scrollbar to set position for") );

    scrollbar->SetThumbPosition(pos);
    if ( refresh )
        Refresh();
}

int wxWindow::GetScrollPos(int orient) const
{
    wxScrollBar *scrollbar = GetScrollbar(orient);
    return scrollbar ? scrollbar->GetThumbPosition() : 0;
}

int wxWindow::GetScrollThumb(int orient) const
{
    wxScrollBar *scrollbar = GetScrollbar(orient);
    return scrollbar ? scrollbar->GetThumbSize() : 0;
}

int wxWindow::GetScrollRange(int orient) const
{
    wxScrollBar *scrollbar = GetScrollbar(orient);
    return scrollbar ? scrollbar->GetRange() : 0;
}

void wxWindow::ScrollWindow(int dx, int dy, const wxRect *rect)
{
    wxASSERT_MSG( !rect, _T("scrolling only part of window not implemented") );

    if ( !dx && !dy )
    {
        // nothing to do
        return;
    }

    // calculate the part of the window which we can just redraw in the new
    // location
    wxSize sizeTotal = GetClientSize();

    wxLogTrace(_T("scroll"), _T("window is %dx%d, scroll by %d, %d"),
               sizeTotal.x, sizeTotal.y, dx, dy);

    wxPoint ptSource, ptDest, ptOrigin;
    ptSource =
    ptDest =
    ptOrigin = GetClientAreaOrigin();
    wxSize size;
    size.x = sizeTotal.x - abs(dx) - 1;
    size.y = sizeTotal.y - abs(dy) - 1;
    if ( size.x <= 0 || size.y <= 0 )
    {
        // just redraw everything as nothing of the displayed image will stay
        wxLogTrace(_T("scroll"), _T("refreshing everything"));

        Refresh();
    }
    else // move the part which doesn't change to the new location
    {
        // note that when we scroll the canvas in some direction we move the
        // block which doesn't need to be refreshed in the opposite direction

        if ( dx < 0 )
        {
            // scroll to the right, move to the left
            ptSource.x -= dx;
        }
        else
        {
            // scroll to the left, move to the right
            ptDest.x += dx;
        }

        if ( dy < 0 )
        {
            // scroll down, move up
            ptSource.y -= dy;
        }
        else
        {
            // scroll up, move down
            ptDest.y += dy;
        }

        // do move
        wxClientDC dc(this);
        wxBitmap bmp(size.x, size.y);
        wxMemoryDC dcMem;
        dcMem.SelectObject(bmp);
        dcMem.Blit(wxPoint(0, 0), size, &dc, ptSource);
        dc.Blit(ptDest, size, &dcMem, wxPoint(0, 0));

        wxLogTrace(_T("scroll"),
                   _T("Blit: (%d, %d) of size %dx%d -> (%d, %d)"),
                   ptSource.x, ptSource.y,
                   size.x, size.y,
                   ptDest.x, ptDest.y);

        // and now repaint the uncovered area

        // FIXME: we repaint the intersection of these rectangles twice - is
        //        it bad?

        wxRect rect;
        rect.x = ptOrigin.x;
        rect.y = ptOrigin.y;

        if ( dx )
        {
            if ( dx < 0 )
            {
                // refresh the area along the right border
                rect.x += size.x;
                rect.width = -dx;
            }
            else
            {
                // refresh the area along the left border
                rect.width = dx;
            }

            rect.height = sizeTotal.y;

            wxLogTrace(_T("scroll"), _T("refreshing (%d, %d)-(%d, %d)"),
                       rect.x, rect.y, rect.GetRight(), rect.GetBottom());

            Refresh(TRUE /* erase bkgnd */, &rect);
        }

        if ( dy )
        {
            if ( dy < 0 )
            {
                // refresh the area along the bottom border
                rect.y += size.y;
                rect.height = -dy;
            }
            else
            {
                // refresh the area along the top border
                rect.height = dy;
            }

            rect.width = sizeTotal.x;

            wxLogTrace(_T("scroll"), _T("refreshing (%d, %d)-(%d, %d)"),
                       rect.x, rect.y, rect.GetRight(), rect.GetBottom());

            Refresh(TRUE /* erase bkgnd */, &rect);
        }
    }
}

// ----------------------------------------------------------------------------
// colours/fonts
// ----------------------------------------------------------------------------

bool wxWindow::SetBackgroundColour(const wxColour& colour)
{
    if ( !wxWindowNative::SetBackgroundColour(colour) )
        return FALSE;

    m_hasBgCol = TRUE;

    return TRUE;
}

bool wxWindow::SetForegroundColour(const wxColour& colour)
{
    if ( !wxWindowNative::SetForegroundColour(colour) )
        return FALSE;

    m_hasFgCol = TRUE;

    return TRUE;
}

bool wxWindow::SetFont(const wxFont& font)
{
    if ( !wxWindowNative::SetFont(font) )
        return FALSE;

    m_hasFont = TRUE;

    return TRUE;
}
