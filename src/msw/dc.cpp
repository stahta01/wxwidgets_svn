/////////////////////////////////////////////////////////////////////////////
// Name:        dc.cpp
// Purpose:     wxDC class
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "dc.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/frame.h"
#include "wx/dc.h"
#include "wx/utils.h"
#include "wx/dialog.h"
#include "wx/app.h"
#endif

#include "wx/dcprint.h"
#include "wx/msw/private.h"

#include <string.h>
#include <math.h>
#include <fstream.h>

#if wxUSE_COMMON_DIALOGS
#include <commdlg.h>
#endif

#ifndef __WIN32__
#include <print.h>
#endif

#ifdef DrawText
#undef DrawText
#endif

#ifdef GetCharWidth
#undef GetCharWidth
#endif

#ifdef StartDoc
#undef StartDoc
#endif

#if !USE_SHARED_LIBRARY
IMPLEMENT_ABSTRACT_CLASS(wxDC, wxObject)
#endif

// Declarations local to this file

#define YSCALE(y) (yorigin - (y))

// #define     wx_round(a)    (int)((a)+.5)

// Default constructor
wxDC::wxDC(void)
{
  m_minX = 0; m_minY = 0; m_maxX = 0; m_maxY = 0;
  m_clipping = FALSE;

  m_filename = "";
  m_canvas = NULL;
  m_oldBitmap = 0;
  m_oldPen = 0;
  m_oldBrush = 0;
  m_oldFont = 0;
  m_oldPalette = 0;
  m_minX = 0; m_minY = 0; m_maxX = 0; m_maxY = 0;
  m_logicalOriginX = 0;
  m_logicalOriginY = 0;
  m_deviceOriginX = 0;
  m_deviceOriginY = 0;
  m_logicalScaleX = 1.0;
  m_logicalScaleY = 1.0;
  m_userScaleX = 1.0;
  m_userScaleY = 1.0;
  m_signX = 1;
  m_signY = 1;
  m_systemScaleX = 1.0;
  m_systemScaleY = 1.0;
  m_mappingMode = MM_TEXT;
  m_bOwnsDC = FALSE;
  m_hDC = 0;
  m_clipping = FALSE;
  m_ok = TRUE;
  m_windowExtX = VIEWPORT_EXTENT;
  m_windowExtY = VIEWPORT_EXTENT;
  m_logicalFunction = -1;

  m_backgroundBrush = *wxWHITE_BRUSH;

  m_textForegroundColour = *wxBLACK;
  m_textBackgroundColour = *wxWHITE;

  m_colour = wxColourDisplay();

  m_hDCCount = 0;
}


wxDC::~wxDC(void)
{
  if ( m_hDC != 0 ) {
    SelectOldObjects(m_hDC);
    if ( m_bOwnsDC ) {
      if ( m_canvas == NULL )
        ::DeleteDC((HDC)m_hDC);
      else
        ::ReleaseDC((HWND)m_canvas->GetHWND(), (HDC)m_hDC);
    }
  }

}

// This will select current objects out of the DC,
// which is what you have to do before deleting the
// DC.
void wxDC::SelectOldObjects(WXHDC dc)
{
#if WXDEBUG > 1
  wxDebugMsg("wxDC::SelectOldObjects %X\n", this);
#endif
  if (dc)
  {
    if (m_oldBitmap)
    {
#if WXDEBUG > 1
      wxDebugMsg("wxDC::SelectOldObjects: Selecting old HBITMAP %X\n", m_oldBitmap);
#endif
      ::SelectObject((HDC) dc, (HBITMAP) m_oldBitmap);
      if (m_selectedBitmap.Ok())
      {
        m_selectedBitmap.SetSelectedInto(NULL);
      }
    }
    m_oldBitmap = 0 ;
    if (m_oldPen)
    {
#if WXDEBUG > 1
      wxDebugMsg("wxDC::SelectOldObjects: Selecting old HPEN %X\n", m_oldPen);
#endif
      ::SelectObject((HDC) dc, (HPEN) m_oldPen);
    }
    m_oldPen = 0 ;
    if (m_oldBrush)
    {
#if WXDEBUG > 1
      wxDebugMsg("wxDC::SelectOldObjects: Selecting old HBRUSH %X\n", m_oldBrush);
#endif
      ::SelectObject((HDC) dc, (HBRUSH) m_oldBrush);
    }
    m_oldBrush = 0 ;
    if (m_oldFont)
    {
#if WXDEBUG > 1
      wxDebugMsg("wxDC::SelectOldObjects: Selecting old HFONT %X\n", m_oldFont);
#endif
      ::SelectObject((HDC) dc, (HFONT) m_oldFont);
    }
    m_oldFont = 0 ;
    if (m_oldPalette)
    {
#if WXDEBUG > 1
      wxDebugMsg("wxDC::SelectOldObjects: Selecting old HPALETTE %X\n", m_oldPalette);
#endif
      ::SelectPalette((HDC) dc, (HPALETTE) m_oldPalette, TRUE);
    }
#if WXDEBUG > 1
    wxDebugMsg("wxDC::SelectOldObjects: Done.\n");
#endif
    m_oldPalette = 0 ;
  }

  m_brush = wxNullBrush ;
  m_pen = wxNullPen;
  m_palette = wxNullPalette;
  m_font = wxNullFont;
  m_backgroundBrush = wxNullBrush;
  m_selectedBitmap = wxNullBitmap;
}

void wxDC::SetClippingRegion(long cx, long cy, long cw, long ch)
{
  m_clipping = TRUE;
  m_clipX1 = (int)cx;
  m_clipY1 = (int)cy;
  m_clipX2 = (int)(cx + cw);
  m_clipY2 = (int)(cy + ch);

  DoClipping((WXHDC) m_hDC);
}

void wxDC::DoClipping(WXHDC dc)
{
  if (m_clipping && dc)
  {
    IntersectClipRect((HDC) dc, XLOG2DEV(m_clipX1), YLOG2DEV(m_clipY1),
                          XLOG2DEV(m_clipX2), YLOG2DEV(m_clipY2));
  }
}

void wxDC::DestroyClippingRegion(void)
{
  if (m_clipping && m_hDC)
  {
    HRGN rgn = CreateRectRgn(0, 0, 32000, 32000);
#if WXDEBUG > 1
    wxDebugMsg("wxDC::DestroyClippingRegion: Selecting HRGN %X\n", rgn);
#endif
    SelectClipRgn((HDC) m_hDC, rgn);
#if WXDEBUG > 1
    wxDebugMsg("wxDC::DestroyClippingRegion: Deleting HRGN %X\n", rgn);
#endif
    DeleteObject(rgn);
   }
   m_clipping = FALSE;
}

bool wxDC::CanDrawBitmap(void) const
{
  return TRUE;
}

bool wxDC::CanGetTextExtent(void) const
{
  // What sort of display is it?
  int technology = ::GetDeviceCaps((HDC) m_hDC, TECHNOLOGY);

  bool ok;

  if (technology != DT_RASDISPLAY && technology != DT_RASPRINTER)
    ok = FALSE;
  else ok = TRUE;

  return ok;
}

void wxDC::SetPalette(const wxPalette& palette)
{
  // Set the old object temporarily, in case the assignment deletes an object
  // that's not yet selected out.
  if (m_oldPalette)
  {
    ::SelectPalette((HDC) m_hDC, (HPALETTE) m_oldPalette, TRUE);
    m_oldPalette = 0;
  }

  m_palette = m_palette;

  if (!m_palette.Ok())
  {
    // Setting a NULL colourmap is a way of restoring
    // the original colourmap
    if (m_oldPalette)
    {
      ::SelectPalette((HDC) m_hDC, (HPALETTE) m_oldPalette, TRUE);
#if WXDEBUG > 1
      wxDebugMsg("wxDC::SetPalette: set old palette %X\n", m_oldPalette);
#endif
      m_oldPalette = 0;
    }

	return;
  }
    
  if (m_palette.Ok() && m_palette.GetHPALETTE())
  {
    HPALETTE oldPal = ::SelectPalette((HDC) m_hDC, (HPALETTE) m_palette.GetHPALETTE(), TRUE);
    if (!m_oldPalette)
      m_oldPalette = (WXHPALETTE) oldPal;

#if WXDEBUG > 1
    wxDebugMsg("wxDC::SetPalette %X: selected palette %X\n", this, m_palette.GetHPALETTE());
    if (oldPal)
      wxDebugMsg("wxDC::SetPalette: oldPal was palette %X\n", oldPal);
    if (m_oldPalette)
      wxDebugMsg("wxDC::SetPalette: m_oldPalette is palette %X\n", m_oldPalette);
#endif
    ::RealizePalette((HDC) m_hDC);
  }
}

void wxDC::Clear(void)
{
  RECT rect;
  if (m_canvas)
    GetClientRect((HWND) m_canvas->GetHWND(), &rect);
  else if (m_selectedBitmap.Ok())
  {
    rect.left = 0; rect.top = 0;
    rect.right = m_selectedBitmap.GetWidth();
    rect.bottom = m_selectedBitmap.GetHeight();
  }
  (void) ::SetMapMode((HDC) m_hDC, MM_TEXT);

  DWORD colour = GetBkColor((HDC) m_hDC);
  HBRUSH brush = CreateSolidBrush(colour);
  FillRect((HDC) m_hDC, &rect, brush);
  DeleteObject(brush);

  ::SetMapMode((HDC) m_hDC, MM_ANISOTROPIC);
  ::SetViewportExtEx((HDC) m_hDC, VIEWPORT_EXTENT, VIEWPORT_EXTENT, NULL);
  ::SetWindowExtEx((HDC) m_hDC, m_windowExtX, m_windowExtY, NULL);
  ::SetViewportOrgEx((HDC) m_hDC, (int)m_deviceOriginX, (int)m_deviceOriginY, NULL);
  ::SetWindowOrgEx((HDC) m_hDC, (int)m_logicalOriginX, (int)m_logicalOriginY, NULL);
}

void wxDC::FloodFill(long x, long y, const wxColour& col, int style)
{
  (void)ExtFloodFill((HDC) m_hDC, XLOG2DEV(x), YLOG2DEV(y),
                        col.GetPixel(),
                        style==wxFLOOD_SURFACE?
                          FLOODFILLSURFACE:FLOODFILLBORDER
                        );

  CalcBoundingBox(x, y);
}

bool wxDC::GetPixel(long x, long y, wxColour *col) const
{
  // added by steve 29.12.94 (copied from DrawPoint)
  // returns TRUE for pixels in the color of the current pen
  // and FALSE for all other pixels colors
  // if col is non-NULL return the color of the pixel

  // get the color of the pixel
  COLORREF pixelcolor = ::GetPixel((HDC) m_hDC, XLOG2DEV(x), YLOG2DEV(y));
  // get the color of the pen
  COLORREF pencolor = 0x00ffffff;
  if (m_pen.Ok())
  {
    pencolor = m_pen.GetColour().GetPixel() ;
  }
  
  // return the color of the pixel
  if(col)
    col->Set(GetRValue(pixelcolor),GetGValue(pixelcolor),GetBValue(pixelcolor));
  
  // check, if color of the pixels is the same as the color
  // of the current pen
  return(pixelcolor==pencolor);
}

void wxDC::CrossHair(long x, long y)
{
      // We suppose that our screen is 2000x2000 max.
      long x1 = x-2000;
      long y1 = y-2000;
      long x2 = x+2000;
      long y2 = y+2000;

      (void)MoveToEx((HDC) m_hDC, XLOG2DEV(x1), YLOG2DEV(y), NULL);
      (void)LineTo((HDC) m_hDC, XLOG2DEV(x2), YLOG2DEV(y));

      (void)MoveToEx((HDC) m_hDC, XLOG2DEV(x), YLOG2DEV(y1), NULL);
      (void)LineTo((HDC) m_hDC, XLOG2DEV(x), YLOG2DEV(y2));

      CalcBoundingBox(x1, y1);
      CalcBoundingBox(x2, y2);
}

void wxDC::DrawLine(long x1, long y1, long x2, long y2)
{
  (void)MoveToEx((HDC) m_hDC, XLOG2DEV(x1), YLOG2DEV(y1), NULL);
  (void)LineTo((HDC) m_hDC, XLOG2DEV(x2), YLOG2DEV(y2));

  /* MATTHEW: [6] New normalization */
#if WX_STANDARD_GRAPHICS
  (void)LineTo((HDC) m_hDC, XLOG2DEV(x2) + 1, YLOG2DEV(y2));
#endif

  CalcBoundingBox(x1, y1);
  CalcBoundingBox(x2, y2);
}

void wxDC::DrawArc(long x1,long y1,long x2,long y2, long xc, long yc)
{
  double dx = xc-x1 ;
  double dy = yc-y1 ;
  double radius = (double)sqrt(dx*dx+dy*dy) ;;
  if (x1==x2 && x2==y2)
  {
    DrawEllipse(xc,yc,(double)(radius*2.0),(double)(radius*2)) ;
    return ;
  }

  long xx1 = XLOG2DEV(x1) ;
  long yy1 = YLOG2DEV(y1) ;
  long xx2 = XLOG2DEV(x2) ;
  long yy2 = YLOG2DEV(y2) ;
  long xxc = XLOG2DEV(xc) ;
  long yyc = YLOG2DEV(yc) ;
  long ray = (long) sqrt(double((xxc-xx1)*(xxc-xx1)+(yyc-yy1)*(yyc-yy1))) ;

  (void)MoveToEx((HDC) m_hDC, (int) xx1, (int) yy1, NULL);
  long xxx1 = (long) (xxc-ray);
  long yyy1 = (long) (yyc-ray);
  long xxx2 = (long) (xxc+ray);
  long yyy2 = (long) (yyc+ray);
  if (m_brush.Ok() && m_brush.GetStyle() !=wxTRANSPARENT)
  {
    // Have to add 1 to bottom-right corner of rectangle
    // to make semi-circles look right (crooked line otherwise).
    // Unfortunately this is not a reliable method, depends
    // on the size of shape.
    // TODO: figure out why this happens!
    Pie((HDC) m_hDC,xxx1,yyy1,xxx2+1,yyy2+1,
        xx1,yy1,xx2,yy2) ;
  }
  else
    Arc((HDC) m_hDC,xxx1,yyy1,xxx2,yyy2,
        xx1,yy1,xx2,yy2) ;

  CalcBoundingBox((xc-radius), (yc-radius));
  CalcBoundingBox((xc+radius), (yc+radius));
}

void wxDC::DrawPoint(long x, long y)
{
  COLORREF color = 0x00ffffff;
  if (m_pen.Ok())
  {
    color = m_pen.GetColour().GetPixel() ;
  }

  SetPixel((HDC) m_hDC, XLOG2DEV(x), YLOG2DEV(y), color);

  CalcBoundingBox(x, y);
}

void wxDC::DrawPolygon(int n, wxPoint points[], long xoffset, long yoffset,int fillStyle)
{
  // Do things less efficiently if we have offsets
  if (xoffset != 0 || yoffset != 0)
  {
    POINT *cpoints = new POINT[n];
    int i;
    for (i = 0; i < n; i++)
    {
        cpoints[i].x = (int)(points[i].x + xoffset);
        cpoints[i].y = (int)(points[i].y + yoffset);

        CalcBoundingBox(cpoints[i].x, cpoints[i].y);
    }
    int prev = SetPolyFillMode((HDC) m_hDC,fillStyle==wxODDEVEN_RULE?ALTERNATE:WINDING) ;
    (void)Polygon((HDC) m_hDC, cpoints, n);
    SetPolyFillMode((HDC) m_hDC,prev) ;
    delete[] cpoints;
  }
  else
  {
    int i;
    for (i = 0; i < n; i++)
        CalcBoundingBox(points[i].x, points[i].y);

    int prev = SetPolyFillMode((HDC) m_hDC,fillStyle==wxODDEVEN_RULE?ALTERNATE:WINDING) ;
    (void)Polygon((HDC) m_hDC, (POINT*) points, n);
    SetPolyFillMode((HDC) m_hDC,prev) ;
  }
}

void wxDC::DrawLines(int n, wxPoint points[], long xoffset, long yoffset)
{
  // Do things less efficiently if we have offsets
  if (xoffset != 0 || yoffset != 0)
  {
    POINT *cpoints = new POINT[n];
    int i;
    for (i = 0; i < n; i++)
    {
        cpoints[i].x = (int)(points[i].x + xoffset);
        cpoints[i].y = (int)(points[i].y + yoffset);

        CalcBoundingBox(cpoints[i].x, cpoints[i].y);
    }
    (void)Polyline((HDC) m_hDC, cpoints, n);
    delete[] cpoints;
  }
  else
  {
    int i;
    for (i = 0; i < n; i++)
        CalcBoundingBox(points[i].x, points[i].y);

    (void)Polyline((HDC) m_hDC, (POINT*) points, n);
  }
}

void wxDC::DrawRectangle(long x, long y, long width, long height)
{
  long x2 = x + width;
  long y2 = y + height;

/* MATTHEW: [6] new normalization */
#if WX_STANDARD_GRAPHICS
  bool do_brush, do_pen;

  do_brush = m_brush.Ok() && m_brush.GetStyle() != wxTRANSPARENT;
  do_pen = m_pen.Ok() && m_pen.GetStyle() != wxTRANSPARENT;

  if (do_brush) {
	 HPEN orig_pen = NULL;

	 if (do_pen || !m_pen.Ok())
		orig_pen = (HPEN) ::SelectObject((HDC) m_hDC, (HPEN) ::GetStockObject(NULL_PEN));

	 (void)Rectangle((HDC) m_hDC, XLOG2DEV(x), YLOG2DEV(y),
		  XLOG2DEV(x2) + 1, YLOG2DEV(y2) + 1);

	 if (do_pen || !m_pen.Ok())
		::SelectObject((HDC) m_hDC , orig_pen);
  }
  if (do_pen) {
	 HBRUSH orig_brush = NULL;

	 if (do_brush || !m_brush.Ok())
		orig_brush = (HBRUSH) ::SelectObject((HDC) m_hDC, (HBRUSH) ::GetStockObject(NULL_BRUSH));

	 (void)Rectangle((HDC) m_hDC, XLOG2DEV(x), YLOG2DEV(y),
		  XLOG2DEV(x2), YLOG2DEV(y2));

	 if (do_brush || !m_brush.Ok())
		::SelectObject((HDC) m_hDC, orig_brush);
  }
#else
  (void)Rectangle((HDC) m_hDC, XLOG2DEV(x), YLOG2DEV(y), XLOG2DEV(x2), YLOG2DEV(y2));
#endif

  CalcBoundingBox(x, y);
  CalcBoundingBox(x2, y2);
}

void wxDC::DrawRoundedRectangle(long x, long y, long width, long height, double radius)
{
  // Now, a negative radius value is interpreted to mean
  // 'the proportion of the smallest X or Y dimension'

  if (radius < 0.0)
  {
    double smallest = 0.0;
    if (width < height)
      smallest = width;
    else
      smallest = height;
    radius = (- radius * smallest);
  }

  long x2 = (x+width);
  long y2 = (y+height);

  (void)RoundRect((HDC) m_hDC, XLOG2DEV(x), YLOG2DEV(y), XLOG2DEV(x2),
                      YLOG2DEV(y2), 2*XLOG2DEV(radius), 2*YLOG2DEV(radius));

  CalcBoundingBox(x, y);
  CalcBoundingBox(x2, y2);
}

void wxDC::DrawEllipse(long x, long y, long width, long height)
{
  long x2 = (x+width);
  long y2 = (y+height);

  (void)Ellipse((HDC) m_hDC, XLOG2DEV(x), YLOG2DEV(y), XLOG2DEV(x2), YLOG2DEV(y2));

  CalcBoundingBox(x, y);
  CalcBoundingBox(x2, y2);
}

// Chris Breeze 20/5/98: first implementation of DrawEllipticArc on Windows
void wxDC::DrawEllipticArc(long x,long y,long w,long h,double sa,double ea)
{
  long x2 = (x+w);
  long y2 = (y+h);

  const double deg2rad = 3.14159265359 / 180.0;
  int rx1 = XLOG2DEV(x+w/2);
  int ry1 = YLOG2DEV(y+h/2);
  int rx2 = rx1;
  int ry2 = ry1;
  rx1 += (int)(100.0 * abs(w) * cos(sa * deg2rad));
  ry1 -= (int)(100.0 * abs(h) * m_signY * sin(sa * deg2rad));
  rx2 += (int)(100.0 * abs(w) * cos(ea * deg2rad));
  ry2 -= (int)(100.0 * abs(h) * m_signY * sin(ea * deg2rad));

  // draw pie with NULL_PEN first and then outline otherwise a line is
  // drawn from the start and end points to the centre
  HPEN orig_pen = (HPEN) ::SelectObject((HDC) m_hDC, (HPEN) ::GetStockObject(NULL_PEN));
  if (m_signY > 0)
  {
    (void)Pie((HDC) m_hDC, XLOG2DEV(x), YLOG2DEV(y), XLOG2DEV(x2)+1, YLOG2DEV(y2)+1,
	    rx1, ry1, rx2, ry2);
  }
  else
  {
    (void)Pie((HDC) m_hDC, XLOG2DEV(x), YLOG2DEV(y)-1, XLOG2DEV(x2)+1, YLOG2DEV(y2),
	    rx1, ry1-1, rx2, ry2-1);
  }
  ::SelectObject((HDC) m_hDC, orig_pen);
  (void)Arc((HDC) m_hDC, XLOG2DEV(x), YLOG2DEV(y), XLOG2DEV(x2), YLOG2DEV(y2),
	  rx1, ry1, rx2, ry2);

  CalcBoundingBox(x, y);
  CalcBoundingBox(x2, y2);
}

void wxDC::DrawIcon(const wxIcon& icon, long x, long y)
{
  ::DrawIcon((HDC) m_hDC, XLOG2DEV(x), YLOG2DEV(y), (HICON) icon.GetHICON());
  CalcBoundingBox(x, y);
  CalcBoundingBox(x+icon.GetWidth(), y+icon.GetHeight());
}

void wxDC::SetFont(const wxFont& the_font)
{
  // Set the old object temporarily, in case the assignment deletes an object
  // that's not yet selected out.
  if (m_oldFont)
  {
    ::SelectObject((HDC) m_hDC, (HFONT) m_oldFont);
    m_oldFont = 0;
  }

  m_font = the_font;

  if (!the_font.Ok())
  {
    if (m_oldFont)
      ::SelectObject((HDC) m_hDC, (HFONT) m_oldFont);
    m_oldFont = 0 ;
  }

  if (m_font.Ok() && m_font.GetResourceHandle())
  {
#if WXDEBUG > 1
    wxDebugMsg("wxDC::SetFont: Selecting HFONT %X\n", m_font.GetResourceHandle());
#endif
    HFONT f = (HFONT) ::SelectObject((HDC) m_hDC, (HFONT) m_font.GetResourceHandle());
    if (f == NULL)
    {
        wxDebugMsg("::SelectObject failed in wxDC::SetFont.");
    }
    if (!m_oldFont)
      m_oldFont = (WXHFONT) f;
  }
}

void wxDC::SetPen(const wxPen& pen)
{
  // Set the old object temporarily, in case the assignment deletes an object
  // that's not yet selected out.
  if (m_oldPen)
  {
    ::SelectObject((HDC) m_hDC, (HPEN) m_oldPen);
    m_oldPen = 0;
  }

  m_pen = pen;

  if (!m_pen.Ok())
  {
    if (m_oldPen)
      ::SelectObject((HDC) m_hDC, (HPEN) m_oldPen);
    m_oldPen = 0 ;
  }

  if (m_pen.Ok())
  {
    if (m_pen.GetResourceHandle())
    {
      HPEN p = (HPEN) ::SelectObject((HDC) m_hDC, (HPEN)m_pen.GetResourceHandle()) ;
      if (!m_oldPen)
        m_oldPen = (WXHPEN) p;
    }
  }
}

void wxDC::SetBrush(const wxBrush& brush)
{
  // Set the old object temporarily, in case the assignment deletes an object
  // that's not yet selected out.
  if (m_oldBrush)
  {
    ::SelectObject((HDC) m_hDC, (HBRUSH) m_oldBrush);
    m_oldBrush = 0;
  }

  m_brush = brush;

  if (!m_brush.Ok())
  {
    if (m_oldBrush)
      ::SelectObject((HDC) m_hDC, (HBRUSH) m_oldBrush);
    m_oldBrush = 0 ;
  }

  if (m_brush.Ok())
  {
    if (m_brush.GetResourceHandle())
    {
      HBRUSH b = 0;
      b = (HBRUSH) ::SelectObject((HDC) m_hDC, (HBRUSH)m_brush.GetResourceHandle()) ;
      if (!m_oldBrush)
        m_oldBrush = (WXHBRUSH) b;
    }
  }
}

void wxDC::DrawText(const wxString& text, long x, long y, bool use16bit)
{
    // Should be unnecessary: SetFont should have done this already.
#if 0
  if (m_font.Ok() && m_font.GetResourceHandle())
  {
#if WXDEBUG > 1
    wxDebugMsg("wxDC::DrawText: Selecting HFONT %X\n", m_font.GetResourceHandle());
#endif
    HFONT f = (HFONT) ::SelectObject((HDC) m_hDC, (HFONT) m_font.GetResourceHandle());
    if (!m_oldFont)
      m_oldFont = (WXHFONT) f;
  }
#endif

  if (m_textForegroundColour.Ok())
    SetTextColor((HDC) m_hDC, m_textForegroundColour.GetPixel() ) ;

  DWORD old_background;
  if (m_textBackgroundColour.Ok())
  {
    old_background = SetBkColor((HDC) m_hDC, m_textBackgroundColour.GetPixel() ) ;
  }

  if (m_backgroundMode == wxTRANSPARENT)
    SetBkMode((HDC) m_hDC, TRANSPARENT);
  else
    SetBkMode((HDC) m_hDC, OPAQUE);

  (void)TextOut((HDC) m_hDC, XLOG2DEV(x), YLOG2DEV(y), (char *) (const char *)text, strlen((const char *)text));

  if (m_textBackgroundColour.Ok())
    (void)SetBkColor((HDC) m_hDC, old_background);

  CalcBoundingBox(x, y);

  long w, h;
  GetTextExtent(text, &w, &h);
  CalcBoundingBox((x + w), (y + h));
}

void wxDC::SetBackground(const wxBrush& brush)
{
  m_backgroundBrush = brush;

  if (!m_backgroundBrush.Ok())
    return;

  if (m_canvas)
  {
    bool customColours = TRUE;
    // If we haven't specified wxUSER_COLOURS, don't allow the panel/dialog box to
    // change background colours from the control-panel specified colours.
    if (m_canvas->IsKindOf(CLASSINFO(wxWindow)) && ((m_canvas->GetWindowStyleFlag() & wxUSER_COLOURS) != wxUSER_COLOURS))
      customColours = FALSE;
      
    if (customColours)
    {
      if (m_backgroundBrush.GetStyle()==wxTRANSPARENT)
      {
        m_canvas->m_backgroundTransparent = TRUE;
      }
      else
      {
        m_canvas->SetBackgroundColour(m_backgroundBrush.GetColour());
        m_canvas->m_backgroundTransparent = FALSE;
      }
    }
  }
  COLORREF new_color = m_backgroundBrush.GetColour().GetPixel() ;
  {
    (void)SetBkColor((HDC) m_hDC, new_color);
  }
}

void wxDC::SetBackgroundMode(int mode)
{
  m_backgroundMode = mode;

  if (m_backgroundMode == wxTRANSPARENT)
    ::SetBkMode((HDC) m_hDC, TRANSPARENT);
  else
    ::SetBkMode((HDC) m_hDC, OPAQUE);
}

void wxDC::SetLogicalFunction(int function)
{
  m_logicalFunction = function;

  SetRop((WXHDC) m_hDC);
}

void wxDC::SetRop(WXHDC dc)
{
  if (!dc || m_logicalFunction < 0)
    return;

  int c_rop;
  // These may be wrong
  switch (m_logicalFunction)
  {
//    case wxXOR: c_rop = R2_XORPEN; break;
    case wxXOR: c_rop = R2_NOTXORPEN; break;
    case wxINVERT: c_rop = R2_NOT; break;
    case wxOR_REVERSE: c_rop = R2_MERGEPENNOT; break;
    case wxAND_REVERSE: c_rop = R2_MASKPENNOT; break;
    case wxCLEAR: c_rop = R2_WHITE; break;
    case wxSET: c_rop = R2_BLACK; break;
    case wxSRC_INVERT: c_rop = R2_NOTCOPYPEN; break;
    case wxOR_INVERT: c_rop = R2_MERGENOTPEN; break;
    case wxAND: c_rop = R2_MASKPEN; break;
    case wxOR: c_rop = R2_MERGEPEN; break;
    case wxAND_INVERT: c_rop = R2_MASKNOTPEN; break;
    case wxEQUIV:
    case wxNAND:
    case wxCOPY:
    default:
      c_rop = R2_COPYPEN; break;
  }
  SetROP2((HDC) dc, c_rop);
}

bool wxDC::StartDoc(const wxString& message)
{
  if (!this->IsKindOf(CLASSINFO(wxPrinterDC)))
    return TRUE;
    
  DOCINFO docinfo;
  docinfo.cbSize = sizeof(DOCINFO);
  docinfo.lpszDocName = (const char *)message;

  if (m_filename.IsEmpty())
    docinfo.lpszOutput = NULL;
  else
    docinfo.lpszOutput = (const char *)m_filename;

#if defined(__WIN95__)
  docinfo.lpszDatatype = NULL;
  docinfo.fwType = 0;
#endif

  if (!m_hDC)
    return FALSE;

  int ret =
#ifndef __WIN32__
     ::StartDoc((HDC) m_hDC, &docinfo);
#else
#ifdef UNICODE
     ::StartDocW((HDC) m_hDC, &docinfo);
#else
     ::StartDocA((HDC) m_hDC, &docinfo);
#endif
#endif

  if (ret <= 0)
  {
    DWORD lastError = GetLastError();
    wxDebugMsg("wxDC::StartDoc failed with error: %d\n", lastError);
  }
  return (ret > 0);
}

void wxDC::EndDoc(void)
{
  if (!this->IsKindOf(CLASSINFO(wxPrinterDC)))
    return;
  if (m_hDC) ::EndDoc((HDC) m_hDC);
}

void wxDC::StartPage(void)
{
  if (!this->IsKindOf(CLASSINFO(wxPrinterDC)))
    return;
  if (m_hDC)
    ::StartPage((HDC) m_hDC);
}

void wxDC::EndPage(void)
{
  if (!this->IsKindOf(CLASSINFO(wxPrinterDC)))
    return;
  if (m_hDC)
    ::EndPage((HDC) m_hDC);
}

long wxDC::GetCharHeight(void) const
{
  TEXTMETRIC lpTextMetric;

  GetTextMetrics((HDC) m_hDC, &lpTextMetric);

  return YDEV2LOGREL(lpTextMetric.tmHeight);
}

long wxDC::GetCharWidth(void) const
{
  TEXTMETRIC lpTextMetric;

  GetTextMetrics((HDC) m_hDC, &lpTextMetric);

  return XDEV2LOGREL(lpTextMetric.tmAveCharWidth);
}

void wxDC::GetTextExtent(const wxString& string, long *x, long *y,
                         long *descent, long *externalLeading, wxFont *theFont, bool use16bit) const
{
  wxFont *fontToUse = (wxFont*) theFont;
  if (!fontToUse)
    fontToUse = (wxFont*) &m_font;

  SIZE sizeRect;
  TEXTMETRIC tm;

  GetTextExtentPoint((HDC) m_hDC, (char *)(const char *) string, strlen((char *)(const char *) string), &sizeRect);
  GetTextMetrics((HDC) m_hDC, &tm);

  if (x) *x = XDEV2LOGREL(sizeRect.cx);
  if (y) *y = YDEV2LOGREL(sizeRect.cy);
  if (descent) *descent = tm.tmDescent;
  if (externalLeading) *externalLeading = tm.tmExternalLeading;
}

void wxDC::SetMapMode(int mode)
{
  m_mappingMode = mode;

  int pixel_width = 0;
  int pixel_height = 0;
  int mm_width = 0;
  int mm_height = 0;

  pixel_width = GetDeviceCaps((HDC) m_hDC, HORZRES);
  pixel_height = GetDeviceCaps((HDC) m_hDC, VERTRES);
  mm_width = GetDeviceCaps((HDC) m_hDC, HORZSIZE);
  mm_height = GetDeviceCaps((HDC) m_hDC, VERTSIZE);

  if ((pixel_width == 0) || (pixel_height == 0) || (mm_width == 0) || (mm_height == 0))
  {
    return;
  }

  double mm2pixelsX = pixel_width/mm_width;
  double mm2pixelsY = pixel_height/mm_height;

  switch (mode)
  {
    case MM_TWIPS:
    {
      m_logicalScaleX = (twips2mm * mm2pixelsX);
      m_logicalScaleY = (twips2mm * mm2pixelsY);
      break;
    }
    case MM_POINTS:
    {
      m_logicalScaleX = (pt2mm * mm2pixelsX);
      m_logicalScaleY = (pt2mm * mm2pixelsY);
      break;
    }
    case MM_METRIC:
    {
      m_logicalScaleX = mm2pixelsX;
      m_logicalScaleY = mm2pixelsY;
      break;
    }
    case MM_LOMETRIC:
    {
      m_logicalScaleX = (mm2pixelsX/10.0);
      m_logicalScaleY = (mm2pixelsY/10.0);
      break;
    }
    default:
    case MM_TEXT:
    {
      m_logicalScaleX = 1.0;
      m_logicalScaleY = 1.0;
      break;
    }
  }

  if (::GetMapMode((HDC) m_hDC) != MM_ANISOTROPIC)
    ::SetMapMode((HDC) m_hDC, MM_ANISOTROPIC);

  SetViewportExtEx((HDC) m_hDC, VIEWPORT_EXTENT, VIEWPORT_EXTENT, NULL);
  m_windowExtX = (int)MS_XDEV2LOGREL(VIEWPORT_EXTENT);
  m_windowExtY = (int)MS_YDEV2LOGREL(VIEWPORT_EXTENT);
  ::SetWindowExtEx((HDC) m_hDC, m_windowExtX, m_windowExtY, NULL);
  ::SetViewportOrgEx((HDC) m_hDC, (int)m_deviceOriginX, (int)m_deviceOriginY, NULL);
  ::SetWindowOrgEx((HDC) m_hDC, (int)m_logicalOriginX, (int)m_logicalOriginY, NULL);
}

void wxDC::SetUserScale(double x, double y)
{
  m_userScaleX = x;
  m_userScaleY = y;

  SetMapMode(m_mappingMode);
}

void wxDC::SetAxisOrientation(bool xLeftRight, bool yBottomUp)
{
  m_signX = xLeftRight ? 1 : -1;
  m_signY = yBottomUp ? -1 : 1;

  SetMapMode(m_mappingMode);
}

void wxDC::SetSystemScale(double x, double y)
{
  m_systemScaleX = x;
  m_systemScaleY = y;

  SetMapMode(m_mappingMode);
}

void wxDC::SetLogicalOrigin(long x, long y)
{
  m_logicalOriginX = x;
  m_logicalOriginY = y;

  ::SetWindowOrgEx((HDC) m_hDC, (int)m_logicalOriginX, (int)m_logicalOriginY, NULL);
}

void wxDC::SetDeviceOrigin(long x, long y)
{
  m_deviceOriginX = x;
  m_deviceOriginY = y;

  ::SetViewportOrgEx((HDC) m_hDC, (int)m_deviceOriginX, (int)m_deviceOriginY, NULL);
}

long wxDC::DeviceToLogicalX(long x) const
{
	return (long) (((x) - m_deviceOriginX)/(m_logicalScaleX*m_userScaleX*m_signX*m_systemScaleX) - m_logicalOriginX) ;
}

long wxDC::DeviceToLogicalXRel(long x) const
{
	return (long) ((x)/(m_logicalScaleX*m_userScaleX*m_signX*m_systemScaleX)) ;
}

long wxDC::DeviceToLogicalY(long y) const
{
   	return (long) (((y) - m_deviceOriginY)/(m_logicalScaleY*m_userScaleY*m_signY*m_systemScaleY) - m_logicalOriginY) ;
}

long wxDC::DeviceToLogicalYRel(long y) const
{
	return (long) ((y)/(m_logicalScaleY*m_userScaleY*m_signY*m_systemScaleY)) ;
}

long wxDC::LogicalToDeviceX(long x) const
{
	return (long) (floor((x) - m_logicalOriginX)*m_logicalScaleX*m_userScaleX*m_signX*m_systemScaleX + m_deviceOriginX) ;
}

long wxDC::LogicalToDeviceXRel(long x) const
{
	return (long) (floor(x)*m_logicalScaleX*m_userScaleX*m_signX*m_systemScaleX) ;
}

long wxDC::LogicalToDeviceY(long y) const
{
	return (long) (floor((y) - m_logicalOriginY)*m_logicalScaleY*m_userScaleY*m_signY*m_systemScaleY + m_deviceOriginY);
}

long wxDC::LogicalToDeviceYRel(long y) const
{
	return (long) (floor(y)*m_logicalScaleY*m_userScaleY*m_signY*m_systemScaleY) ;
}

// This group of functions may not do any conversion
// if m_scaleGDI is TRUE, since the HDC does the
// conversion automatically.

long wxDC::ImplDeviceToLogicalX(long x) const
{
//	return (m_scaleGDI ?  x :  DeviceToLogicalX(x));
	return x;
}

long wxDC::ImplDeviceToLogicalY(long y) const
{
//	return (m_scaleGDI ?  y :  DeviceToLogicalY(y));
	return y;
}

long wxDC::ImplDeviceToLogicalXRel(long x) const
{
//	return (m_scaleGDI ?  x :  DeviceToLogicalXRel(x));
	return x;
}

long wxDC::ImplDeviceToLogicalYRel(long y) const
{
//	return (m_scaleGDI ?  y :  DeviceToLogicalYRel(y));
	return y;
}

long wxDC::ImplLogicalToDeviceX(long x) const
{
//	return (m_scaleGDI ?  (floor(double(x))) :  LogicalToDeviceX(x));
	return x;
}

long wxDC::ImplLogicalToDeviceY(long y) const
{
//	return (m_scaleGDI ?  (floor(double(y))) :  LogicalToDeviceY(y));
	return y;
}

long wxDC::ImplLogicalToDeviceXRel(long x) const
{
//	return (m_scaleGDI ?  (floor(double(x))) :  LogicalToDeviceXRel(x));
	return x;
}

long wxDC::ImplLogicalToDeviceYRel(long y) const
{
//	return (m_scaleGDI ?  (floor(double(y))) :  LogicalToDeviceYRel(y));
	return y;
}

bool wxDC::Blit(long xdest, long ydest, long width, long height,
                wxDC *source, long xsrc, long ysrc, int rop, bool useMask)
{
  long xdest1 = xdest;
  long ydest1 = ydest;
  long xsrc1 = xsrc;
  long ysrc1 = ysrc;

  // Chris Breeze 18/5/98: use text foreground/background colours
  // when blitting from 1-bit bitmaps
  COLORREF old_textground = ::GetTextColor((HDC)m_hDC);
  COLORREF old_background = ::GetBkColor((HDC)m_hDC);
  if (m_textForegroundColour.Ok())
  {
	::SetTextColor((HDC) m_hDC, m_textForegroundColour.GetPixel() ) ;
  }
  if (m_textBackgroundColour.Ok())
  {
	::SetBkColor((HDC) m_hDC, m_textBackgroundColour.GetPixel() ) ;
  }

  DWORD dwRop = rop == wxCOPY ? SRCCOPY :
                rop == wxCLEAR ? WHITENESS :
                rop == wxSET ? BLACKNESS :
                rop == wxINVERT ? DSTINVERT :
                rop == wxAND ? MERGECOPY :
                rop == wxOR ? MERGEPAINT :
                rop == wxSRC_INVERT ? NOTSRCCOPY :
                rop == wxXOR ? SRCINVERT :
                rop == wxOR_REVERSE ? MERGEPAINT :
                rop == wxAND_REVERSE ? SRCERASE :
                rop == wxSRC_OR ? SRCPAINT :
                rop == wxSRC_AND ? SRCAND :
                SRCCOPY;

  bool success;
  if (useMask && source->m_selectedBitmap.Ok() && source->m_selectedBitmap.GetMask())
  {

#if 0 // __WIN32__
	// Not implemented under Win95 (or maybe a specific device?)
    if (MaskBlt((HDC) m_hDC, xdest1, ydest1, (int)width, (int)height,
            (HDC) source->m_hDC, xsrc1, ysrc1, (HBITMAP) source->m_selectedBitmap.GetMask()->GetMaskBitmap(),
            0, 0, 0xAACC0020))
    {
        // Success
    }
    else
#endif
    {
        // Old code
#if 0
	    HDC dc_mask = CreateCompatibleDC((HDC) source->m_hDC);
	    ::SelectObject(dc_mask, (HBITMAP) source->m_selectedBitmap.GetMask()->GetMaskBitmap());
        success = (BitBlt((HDC) m_hDC, xdest1, ydest1, (int)width, (int)height,
					 dc_mask, xsrc1, ysrc1, 0x00220326 /* NOTSRCAND */) != 0);
        success = (BitBlt((HDC) m_hDC, xdest1, ydest1, (int)width, (int)height,
					 (HDC) source->m_hDC, xsrc1, ysrc1, SRCPAINT) != 0);
	    ::SelectObject(dc_mask, 0);
	    ::DeleteDC(dc_mask);
#endif
        // New code from Chris Breeze, 15/7/98
		// Blit bitmap with mask

		if (IsKindOf(CLASSINFO(wxPrinterDC)))
		{
			// If we are printing source colours are screen colours
			// not printer colours and so we need copy the bitmap
			// pixel by pixel.
			RECT rect;
			HDC dc_mask = ::CreateCompatibleDC((HDC) source->m_hDC);
			HDC dc_src = (HDC) source->m_hDC;

			::SelectObject(dc_mask, (HBITMAP) source->m_selectedBitmap.GetMask()->GetMaskBitmap());
			for (int x = 0; x < width; x++)
			{
				for (int y = 0; y < height; y++)
				{
					COLORREF cref = ::GetPixel(dc_mask, x, y);
					if (cref)
					{
						HBRUSH brush = ::CreateSolidBrush(::GetPixel(dc_src, x, y));
						rect.left = xdest1 + x;	rect.right = rect.left + 1;
						rect.top = ydest1 + y;	rect.bottom = rect.top + 1;
						::FillRect((HDC) m_hDC, &rect, brush);
						::DeleteObject(brush);
					}
				}
			}
			::SelectObject(dc_mask, 0);
			::DeleteDC(dc_mask);
		}
		else
		{
			// create a temp buffer bitmap and DCs to access it and the mask
			HDC dc_mask = ::CreateCompatibleDC((HDC) source->m_hDC);
			HDC dc_buffer = ::CreateCompatibleDC((HDC) m_hDC);
			HBITMAP buffer_bmap = ::CreateCompatibleBitmap((HDC) m_hDC, width, height);
			::SelectObject(dc_mask, (HBITMAP) source->m_selectedBitmap.GetMask()->GetMaskBitmap());
			::SelectObject(dc_buffer, buffer_bmap);

			// copy dest to buffer
			::BitBlt(dc_buffer, 0, 0, (int)width, (int)height,
				(HDC) m_hDC, xdest1, ydest1, SRCCOPY);

			// copy src to buffer using selected raster op
			::BitBlt(dc_buffer, 0, 0, (int)width, (int)height,
				(HDC) source->m_hDC, xsrc1, ysrc1, dwRop);

			// set masked area in buffer to BLACK (pixel value 0)
			COLORREF prevBkCol = ::SetBkColor((HDC) m_hDC, RGB(255, 255, 255));
			COLORREF prevCol = ::SetTextColor((HDC) m_hDC, RGB(0, 0, 0));
			::BitBlt(dc_buffer, 0, 0, (int)width, (int)height,
				dc_mask, xsrc1, ysrc1, SRCAND);

			// set unmasked area in dest to BLACK
			::SetBkColor((HDC) m_hDC, RGB(0, 0, 0));
			::SetTextColor((HDC) m_hDC, RGB(255, 255, 255));
			::BitBlt((HDC) m_hDC, xdest1, ydest1, (int)width, (int)height,
				dc_mask, xsrc1, ysrc1, SRCAND);
			::SetBkColor((HDC) m_hDC, prevBkCol);	// restore colours to original values
			::SetTextColor((HDC) m_hDC, prevCol);

			// OR buffer to dest
			success = (::BitBlt((HDC) m_hDC, xdest1, ydest1, (int)width, (int)height,
				dc_buffer, 0, 0, SRCPAINT) != 0);

			// tidy up temporary DCs and bitmap
			::SelectObject(dc_mask, 0);
			::DeleteDC(dc_mask);
			::SelectObject(dc_buffer, 0);
			::DeleteDC(dc_buffer);
			::DeleteObject(buffer_bmap);
		}
	}
  }
  else
  {
	if (IsKindOf(CLASSINFO(wxPrinterDC)))
	{
      // If we are printing source colours are screen colours
      // not printer colours and so we need copy the bitmap
      // pixel by pixel.
      HDC dc_src = (HDC) source->m_hDC;
      RECT rect;
      for (int x = 0; x < width; x++)
	  {
        for (int y = 0; y < height; y++)
		{
          HBRUSH brush = ::CreateSolidBrush(::GetPixel(dc_src, x, y));
          rect.left = xdest1 + x;	rect.right = rect.left + 1;
          rect.top = ydest1 + y;	rect.bottom = rect.top + 1;
          ::FillRect((HDC) m_hDC, &rect, brush);
          ::DeleteObject(brush);
		}
	  }
    }
	else
	{
      success = (BitBlt((HDC) m_hDC, xdest1, ydest1, (int)width, (int)height, (HDC) source->m_hDC,
                            xsrc1, ysrc1, dwRop) != 0);
	}
  }
  ::SetTextColor((HDC)m_hDC, old_textground);
  ::SetBkColor((HDC)m_hDC, old_background);

  return success;
}

void wxDC::GetSize(int* width, int* height) const
{
  long w=::GetDeviceCaps((HDC) m_hDC,HORZRES);
  long h=::GetDeviceCaps((HDC) m_hDC,VERTRES);
  *width = w;
  *height = h;
}

void wxDC::GetSizeMM(long *width, long *height) const
{
  long w=::GetDeviceCaps((HDC) m_hDC,HORZSIZE);
  long h=::GetDeviceCaps((HDC) m_hDC,VERTSIZE);
  *width = w;
  *height = h;
}

void wxDC::DrawPolygon(wxList *list, long xoffset, long yoffset,int fillStyle)
{
  int n = list->Number();
  wxPoint *points = new wxPoint[n];

  int i = 0;
  for(wxNode *node = list->First(); node; node = node->Next()) {
    wxPoint *point = (wxPoint *)node->Data();
    points[i].x = point->x;
    points[i++].y = point->y;
  }
  DrawPolygon(n, points, xoffset, yoffset,fillStyle);
  delete[] points;
}

void wxDC::DrawLines(wxList *list, long xoffset, long yoffset)
{
  int n = list->Number();
  wxPoint *points = new wxPoint[n];

  int i = 0;
  for(wxNode *node = list->First(); node; node = node->Next()) {
    wxPoint *point = (wxPoint *)node->Data();
    points[i].x = point->x;
    points[i++].y = point->y;
  }
  DrawLines(n, points, xoffset, yoffset);
  delete []points;
}

void wxDC::SetTextForeground(const wxColour& colour)
{
    m_textForegroundColour = colour;
}

void wxDC::SetTextBackground(const wxColour& colour)
{
    m_textBackgroundColour = colour;
}

// For use by wxWindows only, unless custom units are required.
void wxDC::SetLogicalScale(double x, double y)
{
  m_logicalScaleX = x;
  m_logicalScaleY = y;
}

void wxDC::CalcBoundingBox(long x, long y)
{
  if (x < m_minX) m_minX = x;
  if (y < m_minY) m_minY = y;
  if (x > m_maxX) m_maxX = x;
  if (y > m_maxY) m_maxY = y;
}

void wxDC::GetClippingBox(long *x,long *y,long *w,long *h) const
{
  if (m_clipping)
  {
    *x = m_clipX1 ;
    *y = m_clipY1 ;
    *w = (m_clipX2 - m_clipX1) ;
    *h = (m_clipY2 - m_clipY1) ;
  }
  else
   *x = *y = *w = *h = 0 ;
}

#if WXWIN_COMPATIBILITY
void wxDC::GetTextExtent(const wxString& string, float *x, float *y,
                     float *descent, float *externalLeading,
                     wxFont *theFont, bool use16bit) const
{
    long x1, y1, descent1, externalLeading1;
	GetTextExtent(string, & x1, & y1, & descent1, & externalLeading1, theFont, use16bit);
	*x = x1; *y = y1;
    if (descent)
        *descent = descent1;
    if (externalLeading)
        *externalLeading = externalLeading1;
}
#endif

int wxDC::GetDepth(void) const
{
  return (int) ::GetDeviceCaps((HDC) m_hDC,BITSPIXEL);
}

#if wxUSE_SPLINES

// Make a 3-point spline
void wxDC::DrawSpline(long x1, long y1, long x2, long y2, long x3, long y3)
{
  wxList *point_list = new wxList;

  wxPoint *point1 = new wxPoint;
  point1->x = x1; point1->y = y1;
  point_list->Append((wxObject*)point1);

  wxPoint *point2 = new wxPoint;
  point2->x = x2; point2->y = y2;
  point_list->Append((wxObject*)point2);

  wxPoint *point3 = new wxPoint;
  point3->x = x3; point3->y = y3;
  point_list->Append((wxObject*)point3);

  DrawSpline(point_list);

  for(wxNode *node = point_list->First(); node; node = node->Next()) {
    wxPoint *p = (wxPoint *)node->Data();
    delete p;
  }
  delete point_list;
}

////#define     wx_round(a)    (int)((a)+.5)
//#define     wx_round(a)    (a)

class wxSpline: public wxObject
{
 public:
  int type;
  wxList *points;

  wxSpline(wxList *list);
  void DeletePoints(void);

  // Doesn't delete points
  ~wxSpline(void);
};

void wxDC::DrawSpline(int n, wxPoint points[])
{
  wxList list;
  int i;
  for (i =0; i < n; i++)
    list.Append((wxObject*)&points[i]);
  DrawSpline((wxList *)&list);
}

void wx_draw_open_spline(wxDC *dc, wxSpline *spline);

void wx_quadratic_spline(double a1, double b1, double a2, double b2,
                         double a3, double b3, double a4, double b4);
void wx_clear_stack(void);
int wx_spline_pop(double *x1, double *y1, double *x2, double *y2, double *x3,
        double *y3, double *x4, double *y4);
void wx_spline_push(double x1, double y1, double x2, double y2, double x3, double y3,
          double x4, double y4);
static bool wx_spline_add_point(double x, double y);
static void wx_spline_draw_point_array(wxDC *dc);
wxSpline *wx_make_spline(int x1, int y1, int x2, int y2, int x3, int y3);

void wxDC::DrawSpline(wxList *list)
{
  wxSpline spline(list);

  wx_draw_open_spline(this, &spline);
}


wxList wx_spline_point_list;

void wx_draw_open_spline(wxDC *dc, wxSpline *spline)
{
    wxPoint *p;
    double           cx1, cy1, cx2, cy2, cx3, cy3, cx4, cy4;
    double           x1, y1, x2, y2;

    wxNode *node = spline->points->First();
    p = (wxPoint *)node->Data();

    x1 = p->x;
    y1 = p->y;

    node = node->Next();
    p = (wxPoint *)node->Data();

    x2 = p->x;
    y2 = p->y;
    cx1 = (double)((x1 + x2) / 2);
    cy1 = (double)((y1 + y2) / 2);
    cx2 = (double)((cx1 + x2) / 2);
    cy2 = (double)((cy1 + y2) / 2);

    wx_spline_add_point(x1, y1);

    while ((node = node->Next()) != NULL)
    {
        p = (wxPoint *)node->Data();
	x1 = x2;
	y1 = y2;
	x2 = p->x;
	y2 = p->y;
        cx4 = (double)(x1 + x2) / 2;
        cy4 = (double)(y1 + y2) / 2;
        cx3 = (double)(x1 + cx4) / 2;
        cy3 = (double)(y1 + cy4) / 2;

        wx_quadratic_spline(cx1, cy1, cx2, cy2, cx3, cy3, cx4, cy4);

	cx1 = cx4;
	cy1 = cy4;
        cx2 = (double)(cx1 + x2) / 2;
        cy2 = (double)(cy1 + y2) / 2;
    }

    wx_spline_add_point((double)wx_round(cx1), (double)wx_round(cy1));
    wx_spline_add_point(x2, y2);

    wx_spline_draw_point_array(dc);

}

/********************* CURVES FOR SPLINES *****************************

	The following spline drawing routine is from

	"An Algorithm for High-Speed Curve Generation"
	by George Merrill Chaikin,
	Computer Graphics and Image Processing, 3, Academic Press,
	1974, 346-349.

	and

	"On Chaikin's Algorithm" by R. F. Riesenfeld,
	Computer Graphics and Image Processing, 4, Academic Press,
	1975, 304-310.

***********************************************************************/

#define		half(z1, z2)	((z1+z2)/2.0)
#define		THRESHOLD	5

/* iterative version */

void wx_quadratic_spline(double a1, double b1, double a2, double b2, double a3, double b3, double a4,
                 double b4)
{
    register double  xmid, ymid;
    double           x1, y1, x2, y2, x3, y3, x4, y4;

    wx_clear_stack();
    wx_spline_push(a1, b1, a2, b2, a3, b3, a4, b4);

    while (wx_spline_pop(&x1, &y1, &x2, &y2, &x3, &y3, &x4, &y4)) {
        xmid = (double)half(x2, x3);
        ymid = (double)half(y2, y3);
	if (fabs(x1 - xmid) < THRESHOLD && fabs(y1 - ymid) < THRESHOLD &&
	    fabs(xmid - x4) < THRESHOLD && fabs(ymid - y4) < THRESHOLD) {
            wx_spline_add_point((double)wx_round(x1), (double)wx_round(y1));
            wx_spline_add_point((double)wx_round(xmid), (double)wx_round(ymid));
	} else {
            wx_spline_push(xmid, ymid, (double)half(xmid, x3), (double)half(ymid, y3),
                 (double)half(x3, x4), (double)half(y3, y4), x4, y4);
            wx_spline_push(x1, y1, (double)half(x1, x2), (double)half(y1, y2),
                 (double)half(x2, xmid), (double)half(y2, ymid), xmid, ymid);
	}
    }
}


/* utilities used by spline drawing routines */


typedef struct wx_spline_stack_struct {
    double           x1, y1, x2, y2, x3, y3, x4, y4;
}
                Stack;

#define         SPLINE_STACK_DEPTH             20
static Stack    wx_spline_stack[SPLINE_STACK_DEPTH];
static Stack   *wx_stack_top;
static int      wx_stack_count;

void wx_clear_stack(void)
{
    wx_stack_top = wx_spline_stack;
    wx_stack_count = 0;
}

void wx_spline_push(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
{
    wx_stack_top->x1 = x1;
    wx_stack_top->y1 = y1;
    wx_stack_top->x2 = x2;
    wx_stack_top->y2 = y2;
    wx_stack_top->x3 = x3;
    wx_stack_top->y3 = y3;
    wx_stack_top->x4 = x4;
    wx_stack_top->y4 = y4;
    wx_stack_top++;
    wx_stack_count++;
}

int wx_spline_pop(double *x1, double *y1, double *x2, double *y2,
                  double *x3, double *y3, double *x4, double *y4)
{
    if (wx_stack_count == 0)
	return (0);
    wx_stack_top--;
    wx_stack_count--;
    *x1 = wx_stack_top->x1;
    *y1 = wx_stack_top->y1;
    *x2 = wx_stack_top->x2;
    *y2 = wx_stack_top->y2;
    *x3 = wx_stack_top->x3;
    *y3 = wx_stack_top->y3;
    *x4 = wx_stack_top->x4;
    *y4 = wx_stack_top->y4;
    return (1);
}

static bool wx_spline_add_point(double x, double y)
{
  wxPoint *point = new wxPoint ;
  point->x = (int) x;
  point->y = (int) y;
  wx_spline_point_list.Append((wxObject*)point);
  return TRUE;
}

static void wx_spline_draw_point_array(wxDC *dc)
{
  dc->DrawLines(&wx_spline_point_list, (double)0.0, (double)0.0);
  wxNode *node = wx_spline_point_list.First();
  while (node)
  {
    wxPoint *point = (wxPoint *)node->Data();
    delete point;
    delete node;
    node = wx_spline_point_list.First();
  }
}

wxSpline::wxSpline(wxList *list)
{
  points = list;
}

wxSpline::~wxSpline(void)
{
}

void wxSpline::DeletePoints(void)
{
  for(wxNode *node = points->First(); node; node = points->First())
  {
    wxPoint *point = (wxPoint *)node->Data();
    delete point;
    delete node;
  }
  delete points;
}


#endif // wxUSE_SPLINES

