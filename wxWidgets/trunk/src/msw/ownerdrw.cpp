///////////////////////////////////////////////////////////////////////////////
// Name:        msw/ownerdrw.cpp
// Purpose:     implementation of wxOwnerDrawn class
// Author:      Vadim Zeitlin
// Modified by:
// Created:     13.11.97
// RCS-ID:      $Id$
// Copyright:   (c) 1998 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// Licence:     wxWindows license
///////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"
#include "wx/msw/private.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
  #include "wx/window.h"
  #include "wx/msw/private.h"
  #include "wx/font.h"
  #include "wx/bitmap.h"
  #include "wx/dcmemory.h"
  #include "wx/menu.h"
  #include "wx/utils.h"
#endif

#include "wx/settings.h"
#include "wx/ownerdrw.h"
#include "wx/menuitem.h"
#include "wx/fontutil.h"

#if wxUSE_OWNER_DRAWN


// ============================================================================
// implementation of wxOwnerDrawn class
// ============================================================================

// ctor
// ----
wxOwnerDrawn::wxOwnerDrawn(const wxString& str,
                           bool bCheckable, bool WXUNUSED(bMenuItem))
            : m_strName(str)
{
#if defined(__WXMSW__) && defined(__WIN32__) && defined(SM_CXMENUCHECK)
    // get the default menu height and font from the system
    NONCLIENTMETRICS nm;
    nm.cbSize = sizeof (NONCLIENTMETRICS);
    SystemParametersInfo (SPI_GETNONCLIENTMETRICS,0,&nm,0); 
    m_nMinHeight = nm.iMenuHeight;
    
    // nm.iMenuWidth is the system default for the width of
    // menu icons and checkmarks
    if (ms_nDefaultMarginWidth == 0)
    {
       ms_nDefaultMarginWidth = nm.iMenuWidth;
       ms_nLastMarginWidth = nm.iMenuWidth;
    }

    wxNativeFontInfo info;
    memcpy(&info.lf, &nm.lfMenuFont, sizeof(LOGFONT));
    m_font.Create(info);
#else
    // windows clean install default
    m_nMinHeight = 18;
    
    if (ms_nDefaultMarginWidth == 0)
    {
        ms_nDefaultMarginWidth = 18;
        ms_nLastMarginWidth = 18;
    }
    if (wxNORMAL_FONT)
       m_font = *wxNORMAL_FONT;
#endif

    m_bCheckable   = bCheckable;
    m_bOwnerDrawn  = FALSE;
    m_nHeight      = 0;
    m_nMarginWidth = ms_nLastMarginWidth;
}


// these items will be set during the first invocation of the c'tor,
// because the values will be determined by checking the system settings,
// which is a chunk of code  
size_t wxOwnerDrawn::ms_nDefaultMarginWidth = 0;
size_t wxOwnerDrawn::ms_nLastMarginWidth = 0;


// drawing
// -------

// get size of the item
bool wxOwnerDrawn::OnMeasureItem(size_t *pwidth, size_t *pheight)
{
  wxMemoryDC dc;

  wxString str = wxStripMenuCodes(m_strName);

  // # without this menu items look too tightly packed (at least under Windows)
  str += wxT('W'); // 'W' is typically the widest letter

  if (m_font.Ok())
      dc.SetFont(GetFont());

  dc.GetTextExtent(str, (long *)pwidth, (long *)pheight);

  // JACS: items still look too tightly packed, so adding 5 pixels.
  (*pheight) = (*pheight) + 5;

  // Ray Gilbert's changes - Corrects the problem of a BMP
  // being placed next to text in a menu item, and the BMP does
  // not match the size expected by the system.  This will
  // resize the space so the BMP will fit.  Without this, BMPs
  // must be no larger or smaller than 16x16.
  if (m_bmpChecked.Ok())
  {
      // Is BMP height larger then text height?
      size_t adjustedHeight = m_bmpChecked.GetHeight() +
                              wxSystemSettings::GetMetric(wxSYS_EDGE_Y);
      if (*pheight < adjustedHeight)
          *pheight = adjustedHeight;

      // Does BMP encroach on default check menu position?
      size_t adjustedWidth = m_bmpChecked.GetWidth() +
                             (wxSystemSettings::GetMetric(wxSYS_EDGE_X) * 2);
      if (ms_nDefaultMarginWidth < adjustedWidth)
          *pwidth += adjustedWidth - ms_nDefaultMarginWidth;

      // Do we need to widen margin to fit BMP?
      if ((size_t)GetMarginWidth() < adjustedWidth)
          SetMarginWidth(adjustedWidth);
  }

  // make sure that this item is at least as
  // tall as the user's system settings specify
  if (*pheight < m_nMinHeight)
    *pheight = m_nMinHeight;

  m_nHeight = *pheight;                // remember height for use in OnDrawItem

  return TRUE;
}

// searching for this macro you'll find all the code where I'm using the native
// Win32 GDI functions and not wxWindows ones. Might help to whoever decides to
// port this code to X. (VZ)

// JACS: TODO. Why does a disabled but highlighted item still
// get drawn embossed? How can we tell DrawState that we don't want the
// embossing?

#if defined(__WIN32__) && !defined(__SC__) && !defined(__TWIN32__)
#define   O_DRAW_NATIVE_API     // comments below explain why I use it
#endif

// draw the item
bool wxOwnerDrawn::OnDrawItem(wxDC& dc,
                              const wxRect& rc,
                              wxODAction act,
                              wxODStatus st)
{
  // we do nothing on focus change
  if ( act == wxODFocusChanged )
    return TRUE;

  // wxColor <-> RGB
  #define   ToRGB(col)  PALETTERGB(col.Red(), col.Green(), col.Blue())
  #define   UnRGB(col)  GetRValue(col), GetGValue(col), GetBValue(col)

  // set the colors
  // --------------
  DWORD colBack, colText;
  if ( st & wxODSelected ) {
    colBack = GetSysColor(COLOR_HIGHLIGHT);
    colText = GetSysColor(COLOR_HIGHLIGHTTEXT);
  }
  else {
    // fall back to default colors if none explicitly specified
    colBack = m_colBack.Ok() ? ToRGB(m_colBack) : GetSysColor(COLOR_WINDOW);
    colText = m_colText.Ok() ? ToRGB(m_colText) : GetSysColor(COLOR_WINDOWTEXT);
  }

  #ifdef  O_DRAW_NATIVE_API
    #define  hdc           (HDC)dc.GetHDC()
    COLORREF colOldText = ::SetTextColor(hdc, colText),
             colOldBack = ::SetBkColor(hdc, colBack);
  #else
    dc.SetTextForeground(wxColor(UnRGB(colText)));
    dc.SetTextBackground(wxColor(UnRGB(colBack)));
  #endif

  // select the font and draw the text
  // ---------------------------------


  // determine where to draw and leave space for a check-mark.
  // Add 3 pixel padding so text appears well within highlight rectangle
  int x = rc.x + GetMarginWidth() + 3;


  // using native API because it reckognizes '&'
  #ifdef  O_DRAW_NATIVE_API
    int nPrevMode = SetBkMode(hdc, TRANSPARENT);
    HBRUSH hbr = CreateSolidBrush(colBack),
           hPrevBrush = (HBRUSH)SelectObject(hdc, hbr);

    RECT rectFill = { rc.GetLeft(), rc.GetTop(), rc.GetRight()+1, rc.GetBottom() };

    if ( st & wxODSelected && m_bmpChecked.Ok()) {
        // only draw the highlight under the text, not under
        // the bitmap or checkmark; leave a 1-pixel gap.
        rectFill.left = GetMarginWidth() + 1;
    }

    FillRect(hdc, &rectFill, hbr);

    DeleteObject(hbr);

    // use default font if no font set
    HFONT hfont;
    if ( m_font.Ok() ) {
      m_font.RealizeResource();
      hfont = (HFONT)m_font.GetResourceHandle();
    }
    else {
      hfont = (HFONT)::GetStockObject(SYSTEM_FONT);
    }

    HFONT hPrevFont = (HFONT) ::SelectObject(hdc, hfont);

    wxString strStrippedName = wxStripMenuCodes(m_strName);

    ::DrawState(hdc, NULL, NULL,
                (LPARAM)strStrippedName.c_str(), strStrippedName.length(),
                x, rc.y + 1, rc.GetWidth(), rc.GetHeight(),
                DST_PREFIXTEXT | (st & wxODDisabled ? DSS_DISABLED : 0));

    if ( !m_strAccel.empty() )
    {
        RECT r;
        r.top = rc.GetTop() + 1;
        r.left = rc.GetLeft();
        r.right = rc.GetRight() - 16;
        r.bottom = rc.GetBottom();

        DrawText(hdc, m_strAccel, m_strAccel.length(), &r,
                 DT_SINGLELINE | DT_RIGHT);
    }

    (void)SelectObject(hdc, hPrevBrush);
    (void)SelectObject(hdc, hPrevFont);
    (void)SetBkMode(hdc, nPrevMode);
  #else
    dc.SetFont(GetFont());
    dc.DrawText(wxStripMenuCodes(m_strName), x, rc.y);
  #endif  //O_DRAW_NATIVE_API

  // draw the bitmap
  // ---------------
  if ( IsCheckable() && !m_bmpChecked.Ok() ) {
    if ( st & wxODChecked ) {
      // using native APIs for performance and simplicity
#ifdef  O_DRAW_NATIVE_API
      // what goes on: DrawFrameControl creates a b/w mask,
      // then we copy it to screen to have right colors

        // first create a monochrome bitmap in a memory DC
      HDC hdcMem = CreateCompatibleDC(hdc);
      HBITMAP hbmpCheck = CreateBitmap(GetMarginWidth(), m_nHeight, 1, 1, 0);
      SelectObject(hdcMem, hbmpCheck);

        // then draw a check mark into it
      RECT rect = { 0, 0, GetMarginWidth(), m_nHeight };
      if ( m_nHeight > 0 )
      {
        ::DrawFrameControl(hdcMem, &rect, DFC_MENU, DFCS_MENUCHECK);
      }

        // finally copy it to screen DC and clean up
      BitBlt(hdc, rc.x, rc.y, GetMarginWidth(), m_nHeight,
             hdcMem, 0, 0, SRCCOPY);

      DeleteDC(hdcMem);
      DeleteObject(hbmpCheck);
#else
        // #### to do: perhaps using Marlett font (create equiv. font under X)
//        wxFAIL("not implemented");
#endif  //O_DRAW_NATIVE_API
    }
  }
  else {
    // for uncheckable item we use only the 'checked' bitmap
    wxBitmap bmp(GetBitmap(IsCheckable() ? ((st & wxODChecked) != 0) : TRUE));
    if ( bmp.Ok() ) {
      wxMemoryDC dcMem(&dc);
      dcMem.SelectObject(bmp);

      // center bitmap
      int nBmpWidth = bmp.GetWidth(),
          nBmpHeight = bmp.GetHeight();

      // there should be enough place!
      wxASSERT((nBmpWidth <= rc.GetWidth()) && (nBmpHeight <= rc.GetHeight()));

      int heightDiff = m_nHeight - nBmpHeight;
      dc.Blit(rc.x + (GetMarginWidth() - nBmpWidth) / 2,
              rc.y + heightDiff / 2,
              nBmpWidth, nBmpHeight,
              &dcMem, 0, 0, wxCOPY, TRUE /* use mask */);

      if ( st & wxODSelected ) {

          int x1, y1, x2, y2;
          x1 = rc.x;
          y1 = rc.y;
          x2 = x1 + GetMarginWidth() - 1;
          y2 = y1 + m_nHeight - 1;

          dc.SetPen(*wxWHITE_PEN);
          dc.DrawLine(x1, y1, x2, y1);
          dc.DrawLine(x1, y1, x1, y2);
          dc.SetPen(*wxGREY_PEN);
          dc.DrawLine(x1, y2-1, x2, y2-1);
          dc.DrawLine(x2, y1, x2, y2);
      }
    }
  }

  #ifdef  O_DRAW_NATIVE_API
    ::SetTextColor(hdc, colOldText);
    ::SetBkColor(hdc, colOldBack);

    #undef  hdc
  #endif  //O_DRAW_NATIVE_API

  return TRUE;
}


#endif // wxUSE_OWNER_DRAWN

