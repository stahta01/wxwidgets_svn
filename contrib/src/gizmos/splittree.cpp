/////////////////////////////////////////////////////////////////////////////
// Name:        splittree.cpp
// Purpose:     Classes to achieve a remotely-scrolled tree in a splitter
//              window that can be scrolled by a scrolled window higher in the
//              hierarchy
// Author:      Julian Smart
// Modified by:
// Created:     8/7/2000
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
    #pragma interface "splittree.cpp"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWindows headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wx/generic/treectlg.h"

#include "wx/gizmos/splittree.h"

/*
 * wxRemotelyScrolledTreeCtrl
 */

#if USE_GENERIC_TREECTRL
IMPLEMENT_CLASS(wxRemotelyScrolledTreeCtrl, wxGenericTreeCtrl)
#else
IMPLEMENT_CLASS(wxRemotelyScrolledTreeCtrl, wxTreeCtrl)
#endif

#if USE_GENERIC_TREECTRL
BEGIN_EVENT_TABLE(wxRemotelyScrolledTreeCtrl, wxGenericTreeCtrl)
#else
BEGIN_EVENT_TABLE(wxRemotelyScrolledTreeCtrl, wxTreeCtrl)
#endif
	EVT_SIZE(wxRemotelyScrolledTreeCtrl::OnSize)
	EVT_PAINT(wxRemotelyScrolledTreeCtrl::OnPaint)
	EVT_TREE_ITEM_EXPANDED(-1, wxRemotelyScrolledTreeCtrl::OnExpand)
	EVT_TREE_ITEM_COLLAPSED(-1, wxRemotelyScrolledTreeCtrl::OnExpand)
	EVT_SCROLLWIN(wxRemotelyScrolledTreeCtrl::OnScroll)
END_EVENT_TABLE()

wxRemotelyScrolledTreeCtrl::wxRemotelyScrolledTreeCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pt,
        const wxSize& sz, long style):
        wxTreeCtrl(parent, id, pt, sz, style)
{
}

wxRemotelyScrolledTreeCtrl::~wxRemotelyScrolledTreeCtrl()
{
}

void wxRemotelyScrolledTreeCtrl::HideVScrollbar()
{
#ifdef __WXMSW__
    if (!IsKindOf(CLASSINFO(wxGenericTreeCtrl)))
    {
        ::ShowScrollBar((HWND) GetHWND(), SB_VERT, FALSE);
    }
    else
#endif
    {
        // Implicit in overriding SetScrollbars
    }
}

// Number of pixels per user unit (0 or -1 for no scrollbar)
// Length of virtual canvas in user units
// Length of page in user units
void wxRemotelyScrolledTreeCtrl::SetScrollbars(int pixelsPerUnitX, int pixelsPerUnitY,
                             int noUnitsX, int noUnitsY,
                             int xPos, int yPos,
                             bool noRefresh)
{
    if (IsKindOf(CLASSINFO(wxGenericTreeCtrl)))
    {
        wxGenericTreeCtrl* win = (wxGenericTreeCtrl*) this;
        win->wxGenericTreeCtrl::SetScrollbars(pixelsPerUnitX, 0, noUnitsX, 0, xPos, 0, noRefresh);

	    wxScrolledWindow* scrolledWindow = GetScrolledWindow();
	    if (scrolledWindow)
	    {
            scrolledWindow->SetScrollbars(0, pixelsPerUnitY, 0, noUnitsY, 0, yPos, noRefresh);
        }
    }
}

// In case we're using the generic tree control.
// Get the view start
void wxRemotelyScrolledTreeCtrl::GetViewStart(int *x, int *y) const
{
    if (IsKindOf(CLASSINFO(wxGenericTreeCtrl)))
    {
	    wxScrolledWindow* scrolledWindow = GetScrolledWindow();

        wxGenericTreeCtrl* win = (wxGenericTreeCtrl*) this;
        int x1, y1, x2, y2;
        win->wxGenericTreeCtrl::GetViewStart(& x1, & y1);
        * x = x1; * y = y1;
        if (!scrolledWindow)
            return;

        scrolledWindow->GetViewStart(& x2, & y2);
        * y = y2;
    }
}

// In case we're using the generic tree control.
void wxRemotelyScrolledTreeCtrl::PrepareDC(wxDC& dc)
{
    if (IsKindOf(CLASSINFO(wxGenericTreeCtrl)))
    {
	    wxScrolledWindow* scrolledWindow = GetScrolledWindow();

        wxGenericTreeCtrl* win = (wxGenericTreeCtrl*) this;

        int startX, startY;
        GetViewStart(& startX, & startY);

        int xppu1, yppu1, xppu2, yppu2;
        win->wxGenericTreeCtrl::GetScrollPixelsPerUnit(& xppu1, & yppu1);
        scrolledWindow->GetScrollPixelsPerUnit(& xppu2, & yppu2);

        dc.SetDeviceOrigin( -startX * xppu1, -startY * yppu2 );
        dc.SetUserScale( win->GetScaleX(), win->GetScaleY() );
    }
}

// Scroll to the given line (in scroll units where each unit is
// the height of an item)
void wxRemotelyScrolledTreeCtrl::ScrollToLine(int posHoriz, int posVert)
{
#ifdef __WXMSW__
    if (!IsKindOf(CLASSINFO(wxGenericTreeCtrl)))
    {
	    UINT sbCode = SB_THUMBPOSITION;
	    HWND vertScrollBar = 0;
	    MSWDefWindowProc((WXUINT) WM_VSCROLL, MAKELONG(sbCode, posVert), (WXHWND) vertScrollBar);
    }
    else
#endif
    {
        wxGenericTreeCtrl* win = (wxGenericTreeCtrl*) this;
		win->Refresh();
		/* Doesn't work yet because scrolling is ignored by Scroll
        int xppu, yppu;
	    wxScrolledWindow* scrolledWindow = GetScrolledWindow();
	    if (scrolledWindow)
	    {
			scrolledWindow->GetScrollPixelsPerUnit(& xppu, & yppu);
			win->Scroll(-1, posVert*yppu);
		}
		*/
    }
}

void wxRemotelyScrolledTreeCtrl::OnSize(wxSizeEvent& event)
{
	HideVScrollbar();
	AdjustRemoteScrollbars();
	event.Skip();
}

void wxRemotelyScrolledTreeCtrl::OnExpand(wxTreeEvent& event)
{
	AdjustRemoteScrollbars();
	event.Skip();

    // If we don't have this, we get some bits of lines still remaining
    if (event.GetEventType() == wxEVT_COMMAND_TREE_ITEM_COLLAPSED)
        Refresh();
}

// Adjust the containing wxScrolledWindow's scrollbars appropriately
void wxRemotelyScrolledTreeCtrl::AdjustRemoteScrollbars()
{
    // WILL THIS BE DONE AUTOMATICALLY BY THE GENERIC TREE CONTROL?
/*

Problem with remote-scrolling the generic tree control. It relies
on PrepareDC for adjusting the device origin, which in turn takes
values from wxScrolledWindow: which we've turned off in order to use
a different scrollbar :-( So we could override PrepareDC and use
the _other_ scrolled window's position instead.
Note also ViewStart would need to be overridden.
Plus, wxGenericTreeCtrl::OnPaint will reset the device origin.

*/

    // Assumption: wxGenericTreeCtrl will adjust the scrollbars automatically,
    // since it'll call SetScrollbars and we've defined this to Do The Right Thing.
    if (!IsKindOf(CLASSINFO(wxGenericTreeCtrl)))
        return;

	wxScrolledWindow* scrolledWindow = GetScrolledWindow();
	if (scrolledWindow)
	{
		wxRect itemRect;
		if (GetBoundingRect(GetRootItem(), itemRect))
		{
			int itemHeight = itemRect.GetHeight();

			int w, h;
			GetClientSize(&w, &h);
			
			wxRect rect(0, 0, 0, 0);
			CalcTreeSize(rect);
			int treeViewHeight = rect.GetHeight()/itemHeight;
			
			int scrollPixelsPerLine = itemHeight;
			int scrollPos = - (itemRect.y / itemHeight);
			
			scrolledWindow->SetScrollbars(0, scrollPixelsPerLine, 0, treeViewHeight, 0, scrollPos);
			
			// Ensure that when a scrollbar becomes hidden or visible,
			// the contained window sizes are right.
			// Problem: this is called too early (?)
			wxSizeEvent event(scrolledWindow->GetSize(), scrolledWindow->GetId());
			scrolledWindow->GetEventHandler()->ProcessEvent(event);
		}
	}
}


// Calculate the area that contains both rectangles
static wxRect CombineRectangles(const wxRect& rect1, const wxRect& rect2)
{
    wxRect rect;

    int right1 = rect1.GetRight();
    int bottom1 = rect1.GetBottom();
    int right2 = rect2.GetRight();
    int bottom2 = rect2.GetBottom();
    
    wxPoint topLeft = wxPoint(wxMin(rect1.x, rect2.x), wxMin(rect1.y, rect2.y));
    wxPoint bottomRight = wxPoint(wxMax(right1, right2), wxMax(bottom1, bottom2));
    
    rect.x = topLeft.x; rect.y = topLeft.y;
    rect.SetRight(bottomRight.x);
    rect.SetBottom(bottomRight.y);

    return rect;
}


// Calculate the tree overall size so we can set the scrollbar
// correctly
void wxRemotelyScrolledTreeCtrl::CalcTreeSize(wxRect& rect)
{
	CalcTreeSize(GetRootItem(), rect);
}

void wxRemotelyScrolledTreeCtrl::CalcTreeSize(wxTreeItemId& id, wxRect& rect)
{
	// TODO: implement GetFirst/NextVisibleItem
	// for wxGenericTreeCtrl, plus GetBoundingRect.

	// More efficient implementation would be to find the last item (but how?)
	// Q: is the bounding rect relative to the top of the virtual tree workspace
	// or the top of the window? How would we convert?
	wxRect itemSize;
	if (GetBoundingRect(id, itemSize))
	{
		rect = CombineRectangles(rect, itemSize);
	}

	long cookie;
	wxTreeItemId childId = GetFirstChild(id, cookie);
	while (childId != 0)
	{
		CalcTreeSize(childId, rect);
		childId = GetNextChild(childId, cookie);
	}
}

// Find the scrolled window that contains this control
wxScrolledWindow* wxRemotelyScrolledTreeCtrl::GetScrolledWindow() const
{
	wxWindow* parent = wxWindow::GetParent();
	while (parent)
	{
		if (parent->IsKindOf(CLASSINFO(wxScrolledWindow)))
			return (wxScrolledWindow*) parent;
		parent = parent->GetParent();
	}
	return NULL;
}

void wxRemotelyScrolledTreeCtrl::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);

	wxTreeCtrl::OnPaint(event);

    // Reset the device origin since it may have been set
    dc.SetDeviceOrigin(0, 0);

	wxSize sz = GetClientSize();

	wxPen pen(wxSystemSettings::GetSystemColour(wxSYS_COLOUR_3DLIGHT), 1, wxSOLID);
	dc.SetPen(pen);
	dc.SetBrush(* wxTRANSPARENT_BRUSH);

	wxRect itemRect;
	if (GetBoundingRect(GetRootItem(), itemRect))
	{
		int itemHeight = itemRect.GetHeight();
		wxRect rcClient = GetRect();
		int cy=0;
		wxTreeItemId h;
		for(h=GetFirstVisibleItem();h;h=GetNextVisible(h))
		{
			dc.DrawLine(rcClient.x, cy, rcClient.x + rcClient.width, cy);
			cy += itemHeight;
		}
		dc.DrawLine(rcClient.x, cy, rcClient.x + rcClient.width, cy);
	}
}

void wxRemotelyScrolledTreeCtrl::OnScroll(wxScrollWinEvent& event)
{
    int orient = event.GetOrientation();
    if (orient == wxHORIZONTAL)
    {
        // Don't 'skip' or we'd get into infinite recursion
        return;
    }
    wxScrolledWindow* scrollWin = GetScrolledWindow();
    if (!scrollWin)
        return;

    int x, y;
    scrollWin->GetViewStart(& x, & y);

    ScrollToLine(-1, y);
}

/*
 * wxThinSplitterWindow
 */

IMPLEMENT_CLASS(wxThinSplitterWindow, wxSplitterWindow)

BEGIN_EVENT_TABLE(wxThinSplitterWindow, wxSplitterWindow)
	EVT_SIZE(wxThinSplitterWindow::OnSize)
END_EVENT_TABLE()

wxThinSplitterWindow::wxThinSplitterWindow(wxWindow* parent, wxWindowID id,
      const wxPoint& pos,
      const wxSize& sz,
      long style):
      wxSplitterWindow(parent, id, pos, sz, style)
{
}

void wxThinSplitterWindow::SizeWindows()
{
	// The client size may have changed inbetween
	// the sizing of the first window and the sizing of
	// the second. So repeat SizeWindows.
    wxSplitterWindow::SizeWindows();
    wxSplitterWindow::SizeWindows();
}

// Tests for x, y over sash
bool wxThinSplitterWindow::SashHitTest(int x, int y, int tolerance)
{
	return wxSplitterWindow::SashHitTest(x, y, 4);
}

void wxThinSplitterWindow::DrawSash(wxDC& dc)
{
    if ( m_sashPosition == 0 || !m_windowTwo)
        return;
    if (GetWindowStyle() & wxSP_NOSASH)
        return;

    int w, h;
    GetClientSize(&w, &h);

	if ( m_splitMode == wxSPLIT_VERTICAL )
	{
		dc.SetPen(* m_facePen);
		dc.SetBrush(* m_faceBrush);
		int h1 = h-1;
		int y1 = 0;
		if ( (GetWindowStyleFlag() & wxSP_BORDER) != wxSP_BORDER && (GetWindowStyleFlag() & wxSP_3DBORDER) != wxSP_3DBORDER )
			h1 += 1; // Not sure why this is necessary...
		if ( (GetWindowStyleFlag() & wxSP_3DBORDER) == wxSP_3DBORDER)
		{
			y1 = 2; h1 -= 3;
		}
		dc.DrawRectangle(m_sashPosition, y1, m_sashSize, h1);
	}
	else
	{
		dc.SetPen(* m_facePen);
		dc.SetBrush(* m_faceBrush);
		int w1 = w-1;
		int x1 = 0;
		if ( (GetWindowStyleFlag() & wxSP_BORDER) != wxSP_BORDER && (GetWindowStyleFlag() & wxSP_3DBORDER) != wxSP_3DBORDER )
			w1 ++;
		if ( (GetWindowStyleFlag() & wxSP_3DBORDER) == wxSP_3DBORDER)
		{
			x1 = 2; w1 -= 3;
		}
		dc.DrawRectangle(x1, m_sashPosition, w1, m_sashSize);
	}
	
    dc.SetPen(wxNullPen);
    dc.SetBrush(wxNullBrush);
}

void wxThinSplitterWindow::OnSize(wxSizeEvent& event)
{
	wxSplitterWindow::OnSize(event);
}

/*
 * wxSplitterScrolledWindow
 */

IMPLEMENT_CLASS(wxSplitterScrolledWindow, wxScrolledWindow)

BEGIN_EVENT_TABLE(wxSplitterScrolledWindow, wxScrolledWindow)
	EVT_SCROLLWIN(wxSplitterScrolledWindow::OnScroll)
	EVT_SIZE(wxSplitterScrolledWindow::OnSize)
END_EVENT_TABLE()

wxSplitterScrolledWindow::wxSplitterScrolledWindow(wxWindow* parent, wxWindowID id,
      const wxPoint& pos,
      const wxSize& sz,
      long style):
      wxScrolledWindow(parent, id, pos, sz, style)
{
}

void wxSplitterScrolledWindow::OnSize(wxSizeEvent& event)
{
	wxSize sz = GetClientSize();
	if (GetChildren().First())
	{
		((wxWindow*) GetChildren().First()->Data())->SetSize(0, 0, sz.x, sz.y);
	}
}

void wxSplitterScrolledWindow::OnScroll(wxScrollWinEvent& event)
{
    // Ensure that events being propagated back up the window hierarchy
    // don't cause an infinite loop
    static bool inOnScroll = FALSE;
    if (inOnScroll)
        return;
    inOnScroll = TRUE;

    int orient = event.GetOrientation();

    int nScrollInc = CalcScrollInc(event);
    if (nScrollInc == 0)
    {
        inOnScroll = FALSE;
        return;
    }

    if (orient == wxHORIZONTAL)
    {
        int newPos = m_xScrollPosition + nScrollInc;
        SetScrollPos(wxHORIZONTAL, newPos, TRUE );
    }
    else
    {
        int newPos = m_yScrollPosition + nScrollInc;
        SetScrollPos(wxVERTICAL, newPos, TRUE );
    }

    if (orient == wxHORIZONTAL)
    {
        m_xScrollPosition += nScrollInc;
    }
    else
    {
        m_yScrollPosition += nScrollInc;
    }

    // Find targets in splitter window and send the event to them
    wxNode* node = GetChildren().First();
    while (node)
    {
        wxWindow* child = (wxWindow*) node->Data();
        if (child->IsKindOf(CLASSINFO(wxSplitterWindow)))
        {
            wxSplitterWindow* splitter = (wxSplitterWindow*) child;
            if (splitter->GetWindow1())
                splitter->GetWindow1()->ProcessEvent(event);
            if (splitter->GetWindow2())
                splitter->GetWindow2()->ProcessEvent(event);
            break;
        }
        node = node->Next();
    }

#ifdef __WXMAC__
    m_targetWindow->MacUpdateImmediately() ;
#endif

    inOnScroll = FALSE;
}

