/////////////////////////////////////////////////////////////////////////////
// Name:        generic/statusbr.cpp
// Purpose:     wxStatusBarGeneric class implementation
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "statusbr.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#if wxUSE_STATUSBAR

#ifndef WX_PRECOMP
#include "wx/setup.h"
#include "wx/frame.h"
#include "wx/settings.h"
#include "wx/dcclient.h"
#endif

#include "wx/statusbr.h"

// we only have to do it here when we use wxStatusBarGeneric in addition to the
// standard wxStatusBar class, if wxStatusBarGeneric is the same as
// wxStatusBar, then the corresponding IMPLEMENT_DYNAMIC_CLASS is already in
// common/statbar.cpp
#if defined(__WXMAC__) || \
    (defined(wxUSE_NATIVE_STATUSBAR) && wxUSE_NATIVE_STATUSBAR)
    #include "wx/generic/statusbr.h"

    IMPLEMENT_DYNAMIC_CLASS(wxStatusBarGeneric, wxWindow)
#endif // wxUSE_NATIVE_STATUSBAR

BEGIN_EVENT_TABLE(wxStatusBarGeneric, wxWindow)
    EVT_PAINT(wxStatusBarGeneric::OnPaint)
    EVT_SYS_COLOUR_CHANGED(wxStatusBarGeneric::OnSysColourChanged)
END_EVENT_TABLE()

// Default status border dimensions
#define         wxTHICK_LINE_BORDER 2
#define         wxTHICK_LINE_WIDTH  1

wxStatusBarGeneric::wxStatusBarGeneric()
{
  m_statusWidths = (int *) NULL;
  m_statusStrings = (wxString *) NULL;
  m_nFields = 0;
  m_borderX = wxTHICK_LINE_BORDER;
  m_borderY = wxTHICK_LINE_BORDER;
}

wxStatusBarGeneric::~wxStatusBarGeneric()
{
#   ifdef __WXMSW__
        SetFont(wxNullFont);
#   endif // MSW

    if ( m_statusStrings )
        delete[] m_statusStrings;
}

bool wxStatusBarGeneric::Create(wxWindow *parent,
                         wxWindowID id,
                         long style,
                         const wxString& name)
{
  m_statusWidths = (int *) NULL;
  m_statusStrings = (wxString *) NULL;
  m_nFields = 0;
  m_borderX = wxTHICK_LINE_BORDER;
  m_borderY = wxTHICK_LINE_BORDER;

  bool success = wxWindow::Create(parent, id,
                                  wxDefaultPosition, wxDefaultSize,
                                  style | wxTAB_TRAVERSAL, name);

  // Don't wish this to be found as a child
#ifndef __WXMAC__
  parent->GetChildren().DeleteObject(this);
#endif
  InitColours();

  SetFont(m_defaultStatusBarFont);

  // Set the height according to the font and the border size
  wxClientDC dc(this);
  dc.SetFont(GetFont());

  wxCoord y;
  dc.GetTextExtent(_T("X"), NULL, &y );

  int height = (int)( (11*y)/10 + 2*GetBorderY());

  SetSize(-1, -1, -1, height);

  return success;
}

void wxStatusBarGeneric::SetFieldsCount(int number, const int *widths)
{
    if ( number != m_nFields )
    {
        m_nFields = number;

        delete[] m_statusStrings;
        m_statusStrings = new wxString[number];
    }

    SetStatusWidths(number, widths);
}

void wxStatusBarGeneric::SetStatusText(const wxString& text, int number)
{
    wxCHECK_RET( (number >= 0) && (number < m_nFields),
                 _T("invalid status bar field index") );

    m_statusStrings[number] = text;

    wxRect rect;
    GetFieldRect(number, rect);
    wxClientDC dc(this);

    dc.SetBackground( wxBrush(GetBackgroundColour(), wxSOLID) );
    dc.SetClippingRegion( rect.x+1, rect.y+1, rect.width-1, rect.height-1 );
    dc.Clear();
    dc.DestroyClippingRegion();

    DrawFieldText( dc, number );
}

wxString wxStatusBarGeneric::GetStatusText(int n) const
{
    wxCHECK_MSG( (n >= 0) && (n < m_nFields), wxEmptyString,
                 _T("invalid status bar field index") );

    return m_statusStrings[n];
}

void wxStatusBarGeneric::SetStatusWidths(int n, const int widths_field[])
{
    // only set status widths, when n == number of statuswindows
    wxCHECK_RET( n == m_nFields, _T("status bar field count mismatch") );

    // delete the old widths in any case - this function may be used to reset
    // the widths to the default (all equal)
    delete [] m_statusWidths;

    if ( !widths_field )
    {
        // not an error, see the comment above
        m_statusWidths = (int *)NULL;
        Refresh();
        return;
    }

    int i;

    // VZ: this doesn't do anything as is_variable is unused later
#if 0
    // when one window (minimum) is variable (width <= 0)
    bool is_variable = FALSE;
    for (i = 0; i < m_nFields; i++)
    {
        if (widths_field[i] <= 0)
            is_variable = TRUE;
    }
#endif // 0

    // set widths
    m_statusWidths = new int[n];
    for (i = 0; i < m_nFields; i++)
    {
        m_statusWidths[i] = widths_field[i];
    }
    Refresh();
}

void wxStatusBarGeneric::OnPaint(wxPaintEvent& WXUNUSED(event) )
{
  wxPaintDC dc(this);


  int i;
  if ( GetFont().Ok() )
    dc.SetFont(GetFont());
  dc.SetBackgroundMode(wxTRANSPARENT);

#ifdef __WXPM__
  wxColour                          vColor;

  vColor.InitFromName("GREY");
  ::WinFillRect(dc.m_hPS, &dc.m_vRclPaint, vColor.GetPixel());
#endif

  for ( i = 0; i < m_nFields; i ++ )
    DrawField(dc, i);

#ifdef __WXMSW__
  dc.SetFont(wxNullFont);
#endif // MSW
}

void wxStatusBarGeneric::DrawFieldText(wxDC& dc, int i)
{
  int leftMargin = 2;

  wxRect rect;
  GetFieldRect(i, rect);

  wxString text(GetStatusText(i));

  long x, y;

  dc.GetTextExtent(text, &x, &y);

  int xpos = rect.x + leftMargin;
  int ypos = (int) (((rect.height - y) / 2 ) + rect.y + 0.5) ;

#if defined( __WXGTK__ ) || defined(__WXMAC__)
  xpos++;
  ypos++;
#endif

  dc.SetClippingRegion(rect.x, rect.y, rect.width, rect.height);

  dc.DrawText(text, xpos, ypos);

  dc.DestroyClippingRegion();
}

void wxStatusBarGeneric::DrawField(wxDC& dc, int i)
{
  wxRect rect;
  GetFieldRect(i, rect);

    // Draw border
    // Have grey background, plus 3-d border -
    // One black rectangle.
    // Inside this, left and top sides - dark grey. Bottom and right -
    // white.

    dc.SetPen(m_hilightPen);

#ifndef __WXPM__

    // Right and bottom white lines
    dc.DrawLine(rect.x + rect.width, rect.y,
                rect.x + rect.width, rect.y + rect.height);
    dc.DrawLine(rect.x + rect.width, rect.y + rect.height,
                rect.x, rect.y + rect.height);

    dc.SetPen(m_mediumShadowPen);

    // Left and top grey lines
    dc.DrawLine(rect.x, rect.y + rect.height,
           rect.x, rect.y);
    dc.DrawLine(rect.x, rect.y,
        rect.x + rect.width, rect.y);
#else
    // Right
    dc.DrawLine(rect.x + rect.width, rect.y,
                rect.x + rect.width, rect.y + rect.height + 2);
    dc.SetPen(m_mediumShadowPen);
    dc.DrawLine(rect.x + rect.width + 1, rect.y,
                rect.x + rect.width + 1, rect.y + rect.height + 2);
    dc.DrawLine(rect.x + rect.width + 2, rect.y,
                rect.x + rect.width + 2, rect.y + rect.height + 2);
    // Top
    dc.DrawLine(rect.x + rect.width + 2, rect.y,
                rect.x - 2, rect.y);
    dc.DrawLine(rect.x + rect.width + 1, rect.y - 1,
                rect.x - 2, rect.y - 1);
    dc.SetPen(m_hilightPen);
    dc.DrawLine(rect.x + rect.width, rect.y - 2,
                rect.x - 2, rect.y - 2);

#endif

    DrawFieldText(dc, i);
}

  // Get the position and size of the field's internal bounding rectangle
bool wxStatusBarGeneric::GetFieldRect(int n, wxRect& rect) const
{
  wxCHECK_MSG( (n >= 0) && (n < m_nFields), FALSE,
               _T("invalid status bar field index") );

  int width, height;
#ifdef __WXPM__
  GetSize(&width, &height);
#else
  GetClientSize(&width, &height);
#endif

  int i;
  int sum_of_nonvar = 0;
  int num_of_var = 0;
  bool do_same_width = FALSE;

  int fieldWidth = 0;
  int fieldPosition = 0;

  if (m_statusWidths)
  {
    // if sum(not variable Windows) > c_width - (20 points per variable_window)
    // then do_same_width = TRUE;
    for (i = 0; i < m_nFields; i++)
    {
       if (m_statusWidths[i] > 0) sum_of_nonvar += m_statusWidths[i];
        else num_of_var++;
     }
     if (sum_of_nonvar > (width - 20*num_of_var)) do_same_width = TRUE;
  }
  else do_same_width = TRUE;
  if (do_same_width)
  {
    for (i = 0; i < m_nFields; i++)
    {
      fieldWidth = (int)(width/m_nFields);
      fieldPosition = i*fieldWidth;
      if ( i == n )
        break;
    }
  }
  else // no_same_width
  {
    int *tempwidth = new int[m_nFields];
    int temppos = 0;
    for (i = 0; i < m_nFields; i++)
    {
      if (m_statusWidths[i] > 0) tempwidth[i] = m_statusWidths[i];
      else tempwidth[i] = (width - sum_of_nonvar) / num_of_var;
    }
    for (i = 0; i < m_nFields; i++)
    {
        fieldWidth = tempwidth[i];
        fieldPosition = temppos;

          temppos += tempwidth[i];

        if ( i == n )
            break;
    }
    delete [] tempwidth;
  }

    rect.x = fieldPosition + wxTHICK_LINE_BORDER;
    rect.y = wxTHICK_LINE_BORDER;

    rect.width = fieldWidth - 2 * wxTHICK_LINE_BORDER ;
    rect.height = height - 2 * wxTHICK_LINE_BORDER ;

    return TRUE;
}

// Initialize colours
void wxStatusBarGeneric::InitColours()
{
    // Shadow colours
#if defined(__WIN95__)
    wxColour mediumShadowColour(wxSystemSettings::GetSystemColour(wxSYS_COLOUR_3DSHADOW));
    m_mediumShadowPen = wxPen(mediumShadowColour, 1, wxSOLID);

    wxColour hilightColour(wxSystemSettings::GetSystemColour(wxSYS_COLOUR_3DHILIGHT));
    m_hilightPen = wxPen(hilightColour, 1, wxSOLID);
#elif defined(__WXPM__)
    m_mediumShadowPen = wxPen("LIGHT GREY", 1, wxSOLID);
    m_hilightPen = wxPen("WHITE", 1, wxSOLID);
#else
    m_mediumShadowPen = wxPen("GREY", 1, wxSOLID);
    m_hilightPen = wxPen("WHITE", 1, wxSOLID);
#endif

    m_defaultStatusBarFont = wxSystemSettings::GetSystemFont(wxSYS_DEFAULT_GUI_FONT);
    SetBackgroundColour(wxSystemSettings::GetSystemColour(wxSYS_COLOUR_3DFACE));
}

// Responds to colour changes, and passes event on to children.
void wxStatusBarGeneric::OnSysColourChanged(wxSysColourChangedEvent& event)
{
    InitColours();
    Refresh();

    // Propagate the event to the non-top-level children
    wxWindow::OnSysColourChanged(event);
}

void wxStatusBarGeneric::SetMinHeight(int height)
{
    // check that this min height is not less than minimal height for the
    // current font
    wxClientDC dc(this);
    wxCoord y;
    dc.GetTextExtent( _T("X"), NULL, &y );

    if ( height > (11*y)/10 )
    {
        SetSize(-1, -1, -1, height + 2*m_borderY);
    }
}

#endif // wxUSE_STATUSBAR
