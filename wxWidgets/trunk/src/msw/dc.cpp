/////////////////////////////////////////////////////////////////////////////
// Name:        dc.cpp
// Purpose:     wxDC class
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ===========================================================================
// declarations
// ===========================================================================

// ---------------------------------------------------------------------------
// headers
// ---------------------------------------------------------------------------

#ifdef __GNUG__
    #pragma implementation "dc.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"
#include "wx/msw/private.h" // needs to be before #include <commdlg.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/window.h"
    #include "wx/dc.h"
    #include "wx/utils.h"
    #include "wx/dialog.h"
    #include "wx/app.h"
    #include "wx/bitmap.h"
    #include "wx/dcmemory.h"
    #include "wx/log.h"
    #include "wx/icon.h"
#endif

#include "wx/dcprint.h"

#include <string.h>
#include <math.h>

#if wxUSE_COMMON_DIALOGS
#if wxUSE_NORLANDER_HEADERS
    #include <windows.h>
#endif
    #include <commdlg.h>
#endif

#ifndef __WIN32__
    #include <print.h>
#endif

    IMPLEMENT_ABSTRACT_CLASS(wxDC, wxObject)

// ---------------------------------------------------------------------------
// constants
// ---------------------------------------------------------------------------

static const int VIEWPORT_EXTENT = 1000;

static const int MM_POINTS = 9;
static const int MM_METRIC = 10;

// usually this is defined in math.h
#ifndef M_PI
    static const double M_PI = 3.14159265358979323846;
#endif // M_PI

// ---------------------------------------------------------------------------
// private functions
// ---------------------------------------------------------------------------

// convert degrees to radians
static inline double DegToRad(double deg) { return (deg * M_PI) / 180.0; }

// ===========================================================================
// implementation
// ===========================================================================

// ---------------------------------------------------------------------------
// wxDC
// ---------------------------------------------------------------------------

// Default constructor
wxDC::wxDC()
{
    m_canvas = NULL;

    m_oldBitmap = 0;
    m_oldPen = 0;
    m_oldBrush = 0;
    m_oldFont = 0;
    m_oldPalette = 0;

    m_bOwnsDC = FALSE;
    m_hDC = 0;

    m_windowExtX = VIEWPORT_EXTENT;
    m_windowExtY = VIEWPORT_EXTENT;

    m_hDCCount = 0;
}


wxDC::~wxDC()
{
    if ( m_hDC != 0 ) {
        SelectOldObjects(m_hDC);
        if ( m_bOwnsDC ) {
            if ( m_canvas == NULL )
                ::DeleteDC(GetHdc());
            else
                ::ReleaseDC((HWND)m_canvas->GetHWND(), GetHdc());
        }
    }

}

// This will select current objects out of the DC,
// which is what you have to do before deleting the
// DC.
void wxDC::SelectOldObjects(WXHDC dc)
{
    if (dc)
    {
        if (m_oldBitmap)
        {
            ::SelectObject((HDC) dc, (HBITMAP) m_oldBitmap);
            if (m_selectedBitmap.Ok())
            {
                m_selectedBitmap.SetSelectedInto(NULL);
            }
        }
        m_oldBitmap = 0;
        if (m_oldPen)
        {
            ::SelectObject((HDC) dc, (HPEN) m_oldPen);
        }
        m_oldPen = 0;
        if (m_oldBrush)
        {
            ::SelectObject((HDC) dc, (HBRUSH) m_oldBrush);
        }
        m_oldBrush = 0;
        if (m_oldFont)
        {
            ::SelectObject((HDC) dc, (HFONT) m_oldFont);
        }
        m_oldFont = 0;
        if (m_oldPalette)
        {
            ::SelectPalette((HDC) dc, (HPALETTE) m_oldPalette, TRUE);
        }
        m_oldPalette = 0;
    }

    m_brush = wxNullBrush;
    m_pen = wxNullPen;
    m_palette = wxNullPalette;
    m_font = wxNullFont;
    m_backgroundBrush = wxNullBrush;
    m_selectedBitmap = wxNullBitmap;
}

// ---------------------------------------------------------------------------
// clipping
// ---------------------------------------------------------------------------

void wxDC::DoSetClippingRegion(wxCoord cx, wxCoord cy, wxCoord cw, wxCoord ch)
{
    m_clipping = TRUE;
    m_clipX1 = (int)cx;
    m_clipY1 = (int)cy;
    m_clipX2 = (int)(cx + cw);
    m_clipY2 = (int)(cy + ch);

    DoClipping((WXHDC) m_hDC);
}

void wxDC::DoSetClippingRegionAsRegion(const wxRegion& region)
{
    wxCHECK_RET( region.GetHRGN(), wxT("invalid clipping region") );

    wxRect box = region.GetBox();

    m_clipping = TRUE;
    m_clipX1 = box.x;
    m_clipY1 = box.y;
    m_clipX2 = box.x + box.width;
    m_clipY2 = box.y + box.height;

#ifdef __WIN16__
    SelectClipRgn(GetHdc(), (HRGN) region.GetHRGN());
#else
    ExtSelectClipRgn(GetHdc(), (HRGN) region.GetHRGN(), RGN_AND);
#endif
}

void wxDC::DoClipping(WXHDC dc)
{
    if (m_clipping && dc)
    {
        IntersectClipRect((HDC) dc, XLOG2DEV(m_clipX1), YLOG2DEV(m_clipY1),
                                    XLOG2DEV(m_clipX2), YLOG2DEV(m_clipY2));
    }
}

void wxDC::DestroyClippingRegion()
{
    if (m_clipping && m_hDC)
    {
        // TODO: this should restore the previous clipping region,
        // so that OnPaint processing works correctly, and the update clipping region
        // doesn't get destroyed after the first DestroyClippingRegion.
        HRGN rgn = CreateRectRgn(0, 0, 32000, 32000);
        SelectClipRgn(GetHdc(), rgn);
        DeleteObject(rgn);
    }
    m_clipping = FALSE;
}

// ---------------------------------------------------------------------------
// query capabilities
// ---------------------------------------------------------------------------

bool wxDC::CanDrawBitmap() const
{
    return TRUE;
}

bool wxDC::CanGetTextExtent() const
{
    // What sort of display is it?
    int technology = ::GetDeviceCaps(GetHdc(), TECHNOLOGY);

    return (technology == DT_RASDISPLAY) || (technology == DT_RASPRINTER);
}

int wxDC::GetDepth() const
{
    return (int)::GetDeviceCaps(GetHdc(), BITSPIXEL);
}

// ---------------------------------------------------------------------------
// drawing
// ---------------------------------------------------------------------------

void wxDC::Clear()
{
    RECT rect;
    if ( m_canvas )
    {
        GetClientRect((HWND) m_canvas->GetHWND(), &rect);
    }
    else
    {
        wxCHECK_RET( m_selectedBitmap.Ok(), wxT("this DC can't be cleared") );

        rect.left = 0; rect.top = 0;
        rect.right = m_selectedBitmap.GetWidth();
        rect.bottom = m_selectedBitmap.GetHeight();
    }

    (void) ::SetMapMode(GetHdc(), MM_TEXT);

    DWORD colour = GetBkColor(GetHdc());
    HBRUSH brush = CreateSolidBrush(colour);
    FillRect(GetHdc(), &rect, brush);
    DeleteObject(brush);

    ::SetMapMode(GetHdc(), MM_ANISOTROPIC);
    ::SetViewportExtEx(GetHdc(), VIEWPORT_EXTENT, VIEWPORT_EXTENT, NULL);
    ::SetWindowExtEx(GetHdc(), m_windowExtX, m_windowExtY, NULL);
    ::SetViewportOrgEx(GetHdc(), (int)m_deviceOriginX, (int)m_deviceOriginY, NULL);
    ::SetWindowOrgEx(GetHdc(), (int)m_logicalOriginX, (int)m_logicalOriginY, NULL);
}

void wxDC::DoFloodFill(wxCoord x, wxCoord y, const wxColour& col, int style)
{
    (void)ExtFloodFill(GetHdc(), XLOG2DEV(x), YLOG2DEV(y),
                       col.GetPixel(),
                       style == wxFLOOD_SURFACE ? FLOODFILLSURFACE
                                                : FLOODFILLBORDER);

    CalcBoundingBox(x, y);
}

bool wxDC::DoGetPixel(wxCoord x, wxCoord y, wxColour *col) const
{
    // added by steve 29.12.94 (copied from DrawPoint)
    // returns TRUE for pixels in the color of the current pen
    // and FALSE for all other pixels colors
    // if col is non-NULL return the color of the pixel

    // get the color of the pixel
    COLORREF pixelcolor = ::GetPixel(GetHdc(), XLOG2DEV(x), YLOG2DEV(y));
    // get the color of the pen
    COLORREF pencolor = 0x00ffffff;
    if (m_pen.Ok())
    {
        pencolor = m_pen.GetColour().GetPixel();
    }

    // return the color of the pixel
    if(col)
        col->Set(GetRValue(pixelcolor),GetGValue(pixelcolor),GetBValue(pixelcolor));

    // check, if color of the pixels is the same as the color
    // of the current pen
    return(pixelcolor==pencolor);
}

void wxDC::DoCrossHair(wxCoord x, wxCoord y)
{
    wxCoord x1 = x-VIEWPORT_EXTENT;
    wxCoord y1 = y-VIEWPORT_EXTENT;
    wxCoord x2 = x+VIEWPORT_EXTENT;
    wxCoord y2 = y+VIEWPORT_EXTENT;

    (void)MoveToEx(GetHdc(), XLOG2DEV(x1), YLOG2DEV(y), NULL);
    (void)LineTo(GetHdc(), XLOG2DEV(x2), YLOG2DEV(y));

    (void)MoveToEx(GetHdc(), XLOG2DEV(x), YLOG2DEV(y1), NULL);
    (void)LineTo(GetHdc(), XLOG2DEV(x), YLOG2DEV(y2));

    CalcBoundingBox(x1, y1);
    CalcBoundingBox(x2, y2);
}

void wxDC::DoDrawLine(wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2)
{
    (void)MoveToEx(GetHdc(), XLOG2DEV(x1), YLOG2DEV(y1), NULL);
    (void)LineTo(GetHdc(), XLOG2DEV(x2), YLOG2DEV(y2));

    /* MATTHEW: [6] New normalization */
#if WX_STANDARD_GRAPHICS
    (void)LineTo(GetHdc(), XLOG2DEV(x2) + 1, YLOG2DEV(y2));
#endif

    CalcBoundingBox(x1, y1);
    CalcBoundingBox(x2, y2);
}

void wxDC::DoDrawArc(wxCoord x1,wxCoord y1,wxCoord x2,wxCoord y2, wxCoord xc, wxCoord yc)
{
    double dx = xc-x1;
    double dy = yc-y1;
    double radius = (double)sqrt(dx*dx+dy*dy) ;;
    if (x1==x2 && x2==y2)
    {
        DrawEllipse(xc,yc,(wxCoord)(radius*2.0),(wxCoord)(radius*2.0));
        return;
    }

    wxCoord xx1 = XLOG2DEV(x1);
    wxCoord yy1 = YLOG2DEV(y1);
    wxCoord xx2 = XLOG2DEV(x2);
    wxCoord yy2 = YLOG2DEV(y2);
    wxCoord xxc = XLOG2DEV(xc);
    wxCoord yyc = YLOG2DEV(yc);
    wxCoord ray = (wxCoord) sqrt(double((xxc-xx1)*(xxc-xx1)+(yyc-yy1)*(yyc-yy1)));

    (void)MoveToEx(GetHdc(), (int) xx1, (int) yy1, NULL);
    wxCoord xxx1 = (wxCoord) (xxc-ray);
    wxCoord yyy1 = (wxCoord) (yyc-ray);
    wxCoord xxx2 = (wxCoord) (xxc+ray);
    wxCoord yyy2 = (wxCoord) (yyc+ray);
    if (m_brush.Ok() && m_brush.GetStyle() !=wxTRANSPARENT)
    {
        // Have to add 1 to bottom-right corner of rectangle
        // to make semi-circles look right (crooked line otherwise).
        // Unfortunately this is not a reliable method, depends
        // on the size of shape.
        // TODO: figure out why this happens!
        Pie(GetHdc(),xxx1,yyy1,xxx2+1,yyy2+1,
            xx1,yy1,xx2,yy2);
    }
    else
        Arc(GetHdc(),xxx1,yyy1,xxx2,yyy2,
        xx1,yy1,xx2,yy2);

    CalcBoundingBox((wxCoord)(xc-radius), (wxCoord)(yc-radius));
    CalcBoundingBox((wxCoord)(xc+radius), (wxCoord)(yc+radius));
}

void wxDC::DoDrawPoint(wxCoord x, wxCoord y)
{
    COLORREF color = 0x00ffffff;
    if (m_pen.Ok())
    {
        color = m_pen.GetColour().GetPixel();
    }

    SetPixel(GetHdc(), XLOG2DEV(x), YLOG2DEV(y), color);

    CalcBoundingBox(x, y);
}

void wxDC::DoDrawPolygon(int n, wxPoint points[], wxCoord xoffset, wxCoord yoffset,int fillStyle)
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
        int prev = SetPolyFillMode(GetHdc(),fillStyle==wxODDEVEN_RULE?ALTERNATE:WINDING);
        (void)Polygon(GetHdc(), cpoints, n);
        SetPolyFillMode(GetHdc(),prev);
        delete[] cpoints;
    }
    else
    {
        int i;
        for (i = 0; i < n; i++)
            CalcBoundingBox(points[i].x, points[i].y);

        int prev = SetPolyFillMode(GetHdc(),fillStyle==wxODDEVEN_RULE?ALTERNATE:WINDING);
        (void)Polygon(GetHdc(), (POINT*) points, n);
        SetPolyFillMode(GetHdc(),prev);
    }
}

void wxDC::DoDrawLines(int n, wxPoint points[], wxCoord xoffset, wxCoord yoffset)
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
        (void)Polyline(GetHdc(), cpoints, n);
        delete[] cpoints;
    }
    else
    {
        int i;
        for (i = 0; i < n; i++)
            CalcBoundingBox(points[i].x, points[i].y);

        (void)Polyline(GetHdc(), (POINT*) points, n);
    }
}

void wxDC::DoDrawRectangle(wxCoord x, wxCoord y, wxCoord width, wxCoord height)
{
    wxCoord x2 = x + width;
    wxCoord y2 = y + height;

    /* MATTHEW: [6] new normalization */
#if WX_STANDARD_GRAPHICS
    bool do_brush, do_pen;

    do_brush = m_brush.Ok() && m_brush.GetStyle() != wxTRANSPARENT;
    do_pen = m_pen.Ok() && m_pen.GetStyle() != wxTRANSPARENT;

    if (do_brush) {
        HPEN orig_pen = NULL;

        if (do_pen || !m_pen.Ok())
            orig_pen = (HPEN) ::SelectObject(GetHdc(), (HPEN) ::GetStockObject(NULL_PEN));

        (void)Rectangle(GetHdc(), XLOG2DEV(x), YLOG2DEV(y),
            XLOG2DEV(x2) + 1, YLOG2DEV(y2) + 1);

        if (do_pen || !m_pen.Ok())
            ::SelectObject(GetHdc() , orig_pen);
    }
    if (do_pen) {
        HBRUSH orig_brush = NULL;

        if (do_brush || !m_brush.Ok())
            orig_brush = (HBRUSH) ::SelectObject(GetHdc(), (HBRUSH) ::GetStockObject(NULL_BRUSH));

        (void)Rectangle(GetHdc(), XLOG2DEV(x), YLOG2DEV(y),
            XLOG2DEV(x2), YLOG2DEV(y2));

        if (do_brush || !m_brush.Ok())
            ::SelectObject(GetHdc(), orig_brush);
    }
#else
    (void)Rectangle(GetHdc(), XLOG2DEV(x), YLOG2DEV(y), XLOG2DEV(x2), YLOG2DEV(y2));
#endif

    CalcBoundingBox(x, y);
    CalcBoundingBox(x2, y2);
}

void wxDC::DoDrawRoundedRectangle(wxCoord x, wxCoord y, wxCoord width, wxCoord height, double radius)
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

    wxCoord x2 = (x+width);
    wxCoord y2 = (y+height);

    (void)RoundRect(GetHdc(), XLOG2DEV(x), YLOG2DEV(y), XLOG2DEV(x2),
        YLOG2DEV(y2), (int) (2*XLOG2DEV(radius)), (int)( 2*YLOG2DEV(radius)));

    CalcBoundingBox(x, y);
    CalcBoundingBox(x2, y2);
}

void wxDC::DoDrawEllipse(wxCoord x, wxCoord y, wxCoord width, wxCoord height)
{
    wxCoord x2 = (x+width);
    wxCoord y2 = (y+height);

    (void)Ellipse(GetHdc(), XLOG2DEV(x), YLOG2DEV(y), XLOG2DEV(x2), YLOG2DEV(y2));

    CalcBoundingBox(x, y);
    CalcBoundingBox(x2, y2);
}

// Chris Breeze 20/5/98: first implementation of DrawEllipticArc on Windows
void wxDC::DoDrawEllipticArc(wxCoord x,wxCoord y,wxCoord w,wxCoord h,double sa,double ea)
{
    wxCoord x2 = (x+w);
    wxCoord y2 = (y+h);

    int rx1 = XLOG2DEV(x+w/2);
    int ry1 = YLOG2DEV(y+h/2);
    int rx2 = rx1;
    int ry2 = ry1;

    sa = DegToRad(sa);
    ea = DegToRad(ea);

    rx1 += (int)(100.0 * abs(w) * cos(sa));
    ry1 -= (int)(100.0 * abs(h) * m_signY * sin(sa));
    rx2 += (int)(100.0 * abs(w) * cos(ea));
    ry2 -= (int)(100.0 * abs(h) * m_signY * sin(ea));

    // draw pie with NULL_PEN first and then outline otherwise a line is
    // drawn from the start and end points to the centre
    HPEN orig_pen = (HPEN) ::SelectObject(GetHdc(), (HPEN) ::GetStockObject(NULL_PEN));
    if (m_signY > 0)
    {
        (void)Pie(GetHdc(), XLOG2DEV(x), YLOG2DEV(y), XLOG2DEV(x2)+1, YLOG2DEV(y2)+1,
            rx1, ry1, rx2, ry2);
    }
    else
    {
        (void)Pie(GetHdc(), XLOG2DEV(x), YLOG2DEV(y)-1, XLOG2DEV(x2)+1, YLOG2DEV(y2),
            rx1, ry1-1, rx2, ry2-1);
    }
    ::SelectObject(GetHdc(), orig_pen);
    (void)Arc(GetHdc(), XLOG2DEV(x), YLOG2DEV(y), XLOG2DEV(x2), YLOG2DEV(y2),
        rx1, ry1, rx2, ry2);

    CalcBoundingBox(x, y);
    CalcBoundingBox(x2, y2);
}

void wxDC::DoDrawIcon(const wxIcon& icon, wxCoord x, wxCoord y)
{
#if defined(__WIN32__) && !defined(__SC__) && !defined(__TWIN32__)
    ::DrawIconEx(GetHdc(), XLOG2DEV(x), YLOG2DEV(y), (HICON) icon.GetHICON(),
        icon.GetWidth(), icon.GetHeight(), 0, 0, DI_NORMAL);
#else
    ::DrawIcon(GetHdc(), XLOG2DEV(x), YLOG2DEV(y), (HICON) icon.GetHICON());
#endif

    CalcBoundingBox(x, y);
    CalcBoundingBox(x+icon.GetWidth(), y+icon.GetHeight());
}

void wxDC::DoDrawBitmap( const wxBitmap &bmp, wxCoord x, wxCoord y, bool useMask )
{
    if (!bmp.Ok())
        return;
    
    bool needsPixelCopy = FALSE ;
    bool isPrinter = FALSE ;
    if (IsKindOf(CLASSINFO(wxPrinterDC)) )
    {
        isPrinter = TRUE ;
        if ( ::GetDeviceCaps((HDC) m_hDC, RASTERCAPS) & RC_STRETCHDIB )
       	{
        }
       	else
       	{
            needsPixelCopy = TRUE ;
       	}
    }
    // If we're not drawing transparently, and not drawing to a printer,
    // optimize this function to use Windows functions.
    if (!useMask && !needsPixelCopy)
    {
        if ( isPrinter )
        {
            BITMAPINFO *info = (BITMAPINFO *) malloc( sizeof( BITMAPINFOHEADER ) + 256 * sizeof(RGBQUAD ) ) ;
            int iBitsSize = ((bmp.GetWidth() + 3 ) & ~3 ) * bmp.GetHeight() ;
            
            void* bits = malloc( iBitsSize ) ;
            
            memset( info , 0 , sizeof( BITMAPINFOHEADER ) ) ;
            
            info->bmiHeader.biSize = sizeof( BITMAPINFOHEADER ) ;
            info->bmiHeader.biWidth = bmp.GetWidth() ;
            info->bmiHeader.biHeight = bmp.GetHeight() ;
            info->bmiHeader.biPlanes = 1 ;
            info->bmiHeader.biBitCount = 8 ;
            info->bmiHeader.biCompression = BI_RGB ;
            
            HDC display = GetDC( NULL ) ;
            if ( GetDIBits( display , (HBITMAP) bmp.GetHBITMAP( ) , 0 , bmp.GetHeight() , bits , info , DIB_RGB_COLORS ) )
            {
                StretchDIBits( (HDC) m_hDC, 
                    x, y, bmp.GetWidth(), bmp.GetHeight() , 
                    0 , 0 ,bmp.GetWidth(), bmp.GetHeight() , 
                    bits , info , DIB_RGB_COLORS , SRCCOPY ) ;
            }
            ReleaseDC( NULL , display ) ;
            free ( bits ) ;
            free( info ) ;
        }
        else
        {
            HDC cdc = GetHdc();
            HDC memdc = ::CreateCompatibleDC( cdc );
            HBITMAP hbitmap = (HBITMAP) bmp.GetHBITMAP( );
            
            wxASSERT_MSG( hbitmap, wxT("bitmap is ok but HBITMAP is NULL?") );
            
            COLORREF old_textground = ::GetTextColor(GetHdc());
            COLORREF old_background = ::GetBkColor(GetHdc());
            if (m_textForegroundColour.Ok())
            {
                ::SetTextColor(GetHdc(), m_textForegroundColour.GetPixel() );
            }
            if (m_textBackgroundColour.Ok())
            {
                ::SetBkColor(GetHdc(), m_textBackgroundColour.GetPixel() );
            }

            ::SelectObject( memdc, hbitmap );
            ::BitBlt( cdc, x, y, bmp.GetWidth(), bmp.GetHeight(), memdc, 0, 0, SRCCOPY);
            ::DeleteDC( memdc );

            ::SetTextColor(GetHdc(), old_textground);
            ::SetBkColor(GetHdc(), old_background);
        }
    }
    else
    {
        // Rather than reproduce wxDC::Blit, let's do it at the wxWin API level
        wxMemoryDC memDC;
        memDC.SelectObject(bmp);
        
        /* Not sure if we need this. The mask should leave the
        * masked areas as per the original background of this DC.
        */
        /*
        // There might be transparent areas, so make these
        // the same colour as this DC
        memDC.SetBackground(* GetBackground());
        memDC.Clear();
        */
        
        Blit(x, y, bmp.GetWidth(), bmp.GetHeight(), & memDC, 0, 0, wxCOPY, useMask);
        
        memDC.SelectObject(wxNullBitmap);
    }
}

void wxDC::DoDrawText(const wxString& text, wxCoord x, wxCoord y)
{
    DrawAnyText(text, x, y);

    // update the bounding box
    CalcBoundingBox(x, y);

    wxCoord w, h;
    GetTextExtent(text, &w, &h);
    CalcBoundingBox(x + w, y + h);
}

void wxDC::DrawAnyText(const wxString& text, wxCoord x, wxCoord y)
{
    // prepare for drawing the text
    if ( m_textForegroundColour.Ok() )
        SetTextColor(GetHdc(), m_textForegroundColour.GetPixel());

    DWORD old_background = 0;
    if ( m_textBackgroundColour.Ok() )
    {
        old_background = SetBkColor(GetHdc(), m_textBackgroundColour.GetPixel() );
    }

    SetBkMode(GetHdc(), m_backgroundMode == wxTRANSPARENT ? TRANSPARENT
                                                          : OPAQUE);

    if ( ::TextOut(GetHdc(), XLOG2DEV(x), YLOG2DEV(y),
                   text.c_str(), text.length()) == 0 )
    {
        wxLogLastError("TextOut");
    }

    // restore the old parameters (text foreground colour may be left because
    // it never is set to anything else, but background should remain
    // transparent even if we just drew an opaque string)
    if ( m_textBackgroundColour.Ok() )
        (void)SetBkColor(GetHdc(), old_background);

    SetBkMode(GetHdc(), TRANSPARENT);
}

void wxDC::DoDrawRotatedText(const wxString& text,
                             wxCoord x, wxCoord y,
                             double angle)
{
    // we test that we have some font because otherwise we should still use the
    // "else" part below to avoid that DrawRotatedText(angle = 180) and
    // DrawRotatedText(angle = 0) use different fonts (we can't use the default
    // font for drawing rotated fonts unfortunately)
    if ( (angle == 0.0) && m_font.Ok() )
    {
        DoDrawText(text, x, y);
    }
    else
    {
        // NB: don't take DEFAULT_GUI_FONT because it's not TrueType and so
        //     can't have non zero orientation/escapement
        wxFont font = m_font.Ok() ? m_font : *wxNORMAL_FONT;
        HFONT hfont = (HFONT)font.GetResourceHandle();
        LOGFONT lf;
        if ( ::GetObject(hfont, sizeof(lf), &lf) == 0 )
        {
            wxLogLastError("GetObject(hfont)");
        }

        // GDI wants the angle in tenth of degree
        long angle10 = (long)(angle * 10);
        lf.lfEscapement = angle10;
        lf. lfOrientation = angle10;

        hfont = ::CreateFontIndirect(&lf);
        if ( !hfont )
        {
            wxLogLastError("CreateFont");
        }
        else
        {
            HFONT hfontOld = (HFONT)::SelectObject(GetHdc(), hfont);

            DrawAnyText(text, x, y);

            (void)::SelectObject(GetHdc(), hfontOld);
        }

        // call the bounding box by adding all four vertices of the rectangle
        // containing the text to it (simpler and probably not slower than
        // determining which of them is really topmost/leftmost/...)
        wxCoord w, h;
        GetTextExtent(text, &w, &h);

        double rad = DegToRad(angle);

        // "upper left" and "upper right"
        CalcBoundingBox(x, y);
        CalcBoundingBox(x + w*cos(rad), y - h*sin(rad));

        // "bottom left" and "bottom right"
        x += (wxCoord)(h*sin(rad));
        y += (wxCoord)(h*cos(rad));
        CalcBoundingBox(x, y);
        CalcBoundingBox(x + h*sin(rad), y + h*cos(rad));
    }
}

// ---------------------------------------------------------------------------
// set GDI objects
// ---------------------------------------------------------------------------

void wxDC::SetPalette(const wxPalette& palette)
{
    // Set the old object temporarily, in case the assignment deletes an object
    // that's not yet selected out.
    if (m_oldPalette)
    {
        ::SelectPalette(GetHdc(), (HPALETTE) m_oldPalette, TRUE);
        m_oldPalette = 0;
    }

    m_palette = palette;

    if (!m_palette.Ok())
    {
        // Setting a NULL colourmap is a way of restoring
        // the original colourmap
        if (m_oldPalette)
        {
            ::SelectPalette(GetHdc(), (HPALETTE) m_oldPalette, TRUE);
            m_oldPalette = 0;
        }

        return;
    }

    if (m_palette.Ok() && m_palette.GetHPALETTE())
    {
        HPALETTE oldPal = ::SelectPalette(GetHdc(), (HPALETTE) m_palette.GetHPALETTE(), TRUE);
        if (!m_oldPalette)
            m_oldPalette = (WXHPALETTE) oldPal;

        ::RealizePalette(GetHdc());
    }
}

void wxDC::SetFont(const wxFont& the_font)
{
    // Set the old object temporarily, in case the assignment deletes an object
    // that's not yet selected out.
    if (m_oldFont)
    {
        ::SelectObject(GetHdc(), (HFONT) m_oldFont);
        m_oldFont = 0;
    }

    m_font = the_font;

    if (!the_font.Ok())
    {
        if (m_oldFont)
            ::SelectObject(GetHdc(), (HFONT) m_oldFont);
        m_oldFont = 0;
    }

    if (m_font.Ok() && m_font.GetResourceHandle())
    {
        HFONT f = (HFONT) ::SelectObject(GetHdc(), (HFONT) m_font.GetResourceHandle());
        if (f == (HFONT) NULL)
        {
            wxLogDebug(wxT("::SelectObject failed in wxDC::SetFont."));
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
        ::SelectObject(GetHdc(), (HPEN) m_oldPen);
        m_oldPen = 0;
    }

    m_pen = pen;

    if (!m_pen.Ok())
    {
        if (m_oldPen)
            ::SelectObject(GetHdc(), (HPEN) m_oldPen);
        m_oldPen = 0;
    }

    if (m_pen.Ok())
    {
        if (m_pen.GetResourceHandle())
        {
            HPEN p = (HPEN) ::SelectObject(GetHdc(), (HPEN)m_pen.GetResourceHandle());
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
        ::SelectObject(GetHdc(), (HBRUSH) m_oldBrush);
        m_oldBrush = 0;
    }

    m_brush = brush;

    if (!m_brush.Ok())
    {
        if (m_oldBrush)
            ::SelectObject(GetHdc(), (HBRUSH) m_oldBrush);
        m_oldBrush = 0;
    }

    if (m_brush.Ok())
    {
        if (m_brush.GetResourceHandle())
        {
            HBRUSH b = 0;
            b = (HBRUSH) ::SelectObject(GetHdc(), (HBRUSH)m_brush.GetResourceHandle());
            if (!m_oldBrush)
                m_oldBrush = (WXHBRUSH) b;
        }
    }
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
                m_canvas->SetTransparent(TRUE);
            }
            else
            {
                // New behaviour, 10/2/99: setting the background brush of a DC
                // doesn't affect the window background colour. However,
                // I'm leaving in the transparency setting because it's needed by
                // various controls (e.g. wxStaticText) to determine whether to draw
                // transparently or not. TODO: maybe this should be a new function
                // wxWindow::SetTransparency(). Should that apply to the child itself, or the
                // parent?
                //        m_canvas->SetBackgroundColour(m_backgroundBrush.GetColour());
                m_canvas->SetTransparent(FALSE);
            }
        }
    }
    COLORREF new_color = m_backgroundBrush.GetColour().GetPixel();
    {
        (void)SetBkColor(GetHdc(), new_color);
    }
}

void wxDC::SetBackgroundMode(int mode)
{
    m_backgroundMode = mode;

    // SetBackgroundColour now only refers to text background
    // and m_backgroundMode is used there

/*
    if (m_backgroundMode == wxTRANSPARENT)
        ::SetBkMode(GetHdc(), TRANSPARENT);
    else
	::SetBkMode(GetHdc(), OPAQUE);
*/
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
    // We might be previewing, so return TRUE to let it continue.
    return TRUE;
}

void wxDC::EndDoc()
{
}

void wxDC::StartPage()
{
}

void wxDC::EndPage()
{
}

// ---------------------------------------------------------------------------
// text metrics
// ---------------------------------------------------------------------------

wxCoord wxDC::GetCharHeight() const
{
    TEXTMETRIC lpTextMetric;

    GetTextMetrics(GetHdc(), &lpTextMetric);

    return YDEV2LOGREL(lpTextMetric.tmHeight);
}

wxCoord wxDC::GetCharWidth() const
{
    TEXTMETRIC lpTextMetric;

    GetTextMetrics(GetHdc(), &lpTextMetric);

    return XDEV2LOGREL(lpTextMetric.tmAveCharWidth);
}

void wxDC::DoGetTextExtent(const wxString& string, wxCoord *x, wxCoord *y,
                           wxCoord *descent, wxCoord *externalLeading,
                           wxFont *theFont) const
{
    wxFont *fontToUse = (wxFont*) theFont;
    if (!fontToUse)
        fontToUse = (wxFont*) &m_font;

    SIZE sizeRect;
    TEXTMETRIC tm;

    GetTextExtentPoint(GetHdc(), WXSTRINGCAST string, wxStrlen(WXSTRINGCAST string), &sizeRect);
    GetTextMetrics(GetHdc(), &tm);

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

    pixel_width = GetDeviceCaps(GetHdc(), HORZRES);
    pixel_height = GetDeviceCaps(GetHdc(), VERTRES);
    mm_width = GetDeviceCaps(GetHdc(), HORZSIZE);
    mm_height = GetDeviceCaps(GetHdc(), VERTSIZE);

    if ((pixel_width == 0) || (pixel_height == 0) || (mm_width == 0) || (mm_height == 0))
    {
        return;
    }

    double mm2pixelsX = pixel_width/mm_width;
    double mm2pixelsY = pixel_height/mm_height;

    switch (mode)
    {
    case wxMM_TWIPS:
        {
            m_logicalScaleX = (twips2mm * mm2pixelsX);
            m_logicalScaleY = (twips2mm * mm2pixelsY);
            break;
        }
    case wxMM_POINTS:
        {
            m_logicalScaleX = (pt2mm * mm2pixelsX);
            m_logicalScaleY = (pt2mm * mm2pixelsY);
            break;
        }
    case wxMM_METRIC:
        {
            m_logicalScaleX = mm2pixelsX;
            m_logicalScaleY = mm2pixelsY;
            break;
        }
    case wxMM_LOMETRIC:
        {
            m_logicalScaleX = (mm2pixelsX/10.0);
            m_logicalScaleY = (mm2pixelsY/10.0);
            break;
        }
    default:
    case wxMM_TEXT:
        {
            m_logicalScaleX = 1.0;
            m_logicalScaleY = 1.0;
            break;
        }
    }

    if (::GetMapMode(GetHdc()) != MM_ANISOTROPIC)
        ::SetMapMode(GetHdc(), MM_ANISOTROPIC);

    SetViewportExtEx(GetHdc(), VIEWPORT_EXTENT, VIEWPORT_EXTENT, NULL);
    m_windowExtX = (int)MS_XDEV2LOGREL(VIEWPORT_EXTENT);
    m_windowExtY = (int)MS_YDEV2LOGREL(VIEWPORT_EXTENT);
    ::SetWindowExtEx(GetHdc(), m_windowExtX, m_windowExtY, NULL);
    ::SetViewportOrgEx(GetHdc(), (int)m_deviceOriginX, (int)m_deviceOriginY, NULL);
    ::SetWindowOrgEx(GetHdc(), (int)m_logicalOriginX, (int)m_logicalOriginY, NULL);
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
    m_scaleX = x;
    m_scaleY = y;

    SetMapMode(m_mappingMode);
}

void wxDC::SetLogicalOrigin(wxCoord x, wxCoord y)
{
    m_logicalOriginX = x;
    m_logicalOriginY = y;

    ::SetWindowOrgEx(GetHdc(), (int)m_logicalOriginX, (int)m_logicalOriginY, NULL);
}

void wxDC::SetDeviceOrigin(wxCoord x, wxCoord y)
{
    m_deviceOriginX = x;
    m_deviceOriginY = y;

    ::SetViewportOrgEx(GetHdc(), (int)m_deviceOriginX, (int)m_deviceOriginY, NULL);
}

// ---------------------------------------------------------------------------
// coordinates transformations
// ---------------------------------------------------------------------------

wxCoord wxDCBase::DeviceToLogicalX(wxCoord x) const
{
    return (wxCoord) (((x) - m_deviceOriginX)/(m_logicalScaleX*m_userScaleX*m_signX*m_scaleX) - m_logicalOriginX);
}

wxCoord wxDCBase::DeviceToLogicalXRel(wxCoord x) const
{
    return (wxCoord) ((x)/(m_logicalScaleX*m_userScaleX*m_signX*m_scaleX));
}

wxCoord wxDCBase::DeviceToLogicalY(wxCoord y) const
{
    return (wxCoord) (((y) - m_deviceOriginY)/(m_logicalScaleY*m_userScaleY*m_signY*m_scaleY) - m_logicalOriginY);
}

wxCoord wxDCBase::DeviceToLogicalYRel(wxCoord y) const
{
    return (wxCoord) ((y)/(m_logicalScaleY*m_userScaleY*m_signY*m_scaleY));
}

wxCoord wxDCBase::LogicalToDeviceX(wxCoord x) const
{
    return (wxCoord) ((x - m_logicalOriginX)*m_logicalScaleX*m_userScaleX*m_signX*m_scaleX + m_deviceOriginX);
}

wxCoord wxDCBase::LogicalToDeviceXRel(wxCoord x) const
{
    return (wxCoord) (x*m_logicalScaleX*m_userScaleX*m_signX*m_scaleX);
}

wxCoord wxDCBase::LogicalToDeviceY(wxCoord y) const
{
    return (wxCoord) ((y - m_logicalOriginY)*m_logicalScaleY*m_userScaleY*m_signY*m_scaleY + m_deviceOriginY);
}

wxCoord wxDCBase::LogicalToDeviceYRel(wxCoord y) const
{
    return (wxCoord) (y*m_logicalScaleY*m_userScaleY*m_signY*m_scaleY);
}

// ---------------------------------------------------------------------------
// bit blit
// ---------------------------------------------------------------------------
bool wxDC::DoBlit(wxCoord xdest, wxCoord ydest, wxCoord width, wxCoord height,
                  wxDC *source, wxCoord xsrc, wxCoord ysrc, int rop, bool useMask)
{
    wxCoord xdest1 = xdest;
    wxCoord ydest1 = ydest;
    wxCoord xsrc1 = xsrc;
    wxCoord ysrc1 = ysrc;

    COLORREF old_textground = ::GetTextColor(GetHdc());
    COLORREF old_background = ::GetBkColor(GetHdc());
    if (m_textForegroundColour.Ok())
    {
        ::SetTextColor(GetHdc(), m_textForegroundColour.GetPixel() );
    }
    if (m_textBackgroundColour.Ok())
    {
        ::SetBkColor(GetHdc(), m_textBackgroundColour.GetPixel() );
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

    bool success = TRUE;
    bool needsPixelCopy = FALSE ;
    bool isPrinter = FALSE ;

    if (IsKindOf(CLASSINFO(wxPrinterDC)) )
    {
        isPrinter = TRUE ;
        if ( ::GetDeviceCaps((HDC) m_hDC, RASTERCAPS) & RC_STRETCHDIB )
       	{
        }
       	else
       	{
            needsPixelCopy = TRUE ;
       	}
    }
    if (useMask && !source->m_selectedBitmap.Ok())
        return FALSE;

    if (useMask && !source->m_selectedBitmap.GetMask())
        useMask = FALSE;

    if (useMask)
    {

#if 0 // __WIN32__
        // Not implemented under Win95 (or maybe a specific device?)
        if (MaskBlt(GetHdc(), xdest1, ydest1, (int)width, (int)height,
            (HDC) source->m_hDC, xsrc1, ysrc1, (HBITMAP) source->m_selectedBitmap.GetMask()->GetMaskBitmap(),
            0, 0, 0xAACC0020))
        {
            // Success
        }
        else
#endif
        {
            // New code from Chris Breeze, 15/7/98
            // Blit bitmap with mask

            if (isPrinter)
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
                            rect.left = xdest1 + x; rect.right = rect.left + 1;
                            rect.top = ydest1 + y;  rect.bottom = rect.top + 1;
                            ::FillRect(GetHdc(), &rect, brush);
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
                HDC dc_buffer = ::CreateCompatibleDC(GetHdc());
                HBITMAP buffer_bmap = ::CreateCompatibleBitmap(GetHdc(), width, height);
                ::SelectObject(dc_mask, (HBITMAP) source->m_selectedBitmap.GetMask()->GetMaskBitmap());
                ::SelectObject(dc_buffer, buffer_bmap);

                // copy dest to buffer
                ::BitBlt(dc_buffer, 0, 0, (int)width, (int)height,
                    GetHdc(), xdest1, ydest1, SRCCOPY);

                // copy src to buffer using selected raster op
                ::BitBlt(dc_buffer, 0, 0, (int)width, (int)height,
                    (HDC) source->m_hDC, xsrc1, ysrc1, dwRop);

                // set masked area in buffer to BLACK (pixel value 0)
                COLORREF prevBkCol = ::SetBkColor(GetHdc(), RGB(255, 255, 255));
                COLORREF prevCol = ::SetTextColor(GetHdc(), RGB(0, 0, 0));
                ::BitBlt(dc_buffer, 0, 0, (int)width, (int)height,
                    dc_mask, xsrc1, ysrc1, SRCAND);

                // set unmasked area in dest to BLACK
                ::SetBkColor(GetHdc(), RGB(0, 0, 0));
                ::SetTextColor(GetHdc(), RGB(255, 255, 255));
                ::BitBlt(GetHdc(), xdest1, ydest1, (int)width, (int)height,
                    dc_mask, xsrc1, ysrc1, SRCAND);
                ::SetBkColor(GetHdc(), prevBkCol);   // restore colours to original values
                ::SetTextColor(GetHdc(), prevCol);

                // OR buffer to dest
                success = (::BitBlt(GetHdc(), xdest1, ydest1, (int)width, (int)height,
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
    else if (needsPixelCopy) // not masked, but we need pixel copy. Only true if it's a printer
    {
        // If we are printing, source colours are screen colours
        // not printer colours and so we need copy the bitmap
        // pixel by pixel.
        if (isPrinter)
        {
            HDC dc_src = (HDC) source->m_hDC;
	        RECT rect;
	        for (int y = 0; y < height; y++)
	        {
                // This is Stefan Csomor's optimisation, where
                // identical adjacent pixels are drawn together.
	      	    for (int x = 0; x < width; x++)
	            {
	                COLORREF col = ::GetPixel(dc_src, x, y) ;
	                HBRUSH brush = ::CreateSolidBrush( col );

	                rect.left = xdest1 + x;
	                rect.top = ydest1 + y;
	                while( (x + 1 < width) && (::GetPixel(dc_src, x + 1, y) == col ) )
	                {
	          	        ++x ;
	                }
	                rect.right = xdest1 + x + 1;
	                rect.bottom = rect.top + 1;
	                ::FillRect((HDC) m_hDC, &rect, brush);
	                ::DeleteObject(brush);
                }
	        }
	    }
        else
        {
            wxFAIL_MSG( "If needsPixelCopy is true, isPrinter should be true also." );
        }
    }
    else if (isPrinter) // not masked, not pixel copy
    {
        wxBitmap& bmp = source->m_selectedBitmap ;
        BITMAPINFO *info = (BITMAPINFO *) malloc( sizeof( BITMAPINFOHEADER ) + 256 * sizeof(RGBQUAD ) ) ;
        int iBitsSize = ((bmp.GetWidth() + 3 ) & ~3 ) * bmp.GetHeight() ;
            
        void* bits = malloc( iBitsSize ) ;
            
        memset( info , 0 , sizeof( BITMAPINFOHEADER ) ) ;
            
        info->bmiHeader.biSize = sizeof( BITMAPINFOHEADER ) ;
        info->bmiHeader.biWidth = bmp.GetWidth() ;
        info->bmiHeader.biHeight = bmp.GetHeight() ;
        info->bmiHeader.biPlanes = 1 ;
        info->bmiHeader.biBitCount = 8 ;
        info->bmiHeader.biCompression = BI_RGB ;
            
        HDC display = GetDC( NULL ) ;
        if ( GetDIBits( display , (HBITMAP) bmp.GetHBITMAP( ) , 0 , bmp.GetHeight() , bits , info , DIB_RGB_COLORS ) )
        {
            success = (GDI_ERROR != StretchDIBits( (HDC) m_hDC,
                    xdest1, ydest1, bmp.GetWidth(), bmp.GetHeight() ,
                    xsrc1 , ysrc1 ,bmp.GetWidth(), bmp.GetHeight() ,
                    bits , info , DIB_RGB_COLORS , SRCCOPY )) ;
        }
        else
            success = FALSE;
        ReleaseDC( NULL , display ) ;
        free ( bits ) ;
        free( info ) ;
    }
    else // Not masked, not printer, not pixel copy
    {
        success = (BitBlt(GetHdc(), xdest1, ydest1, (int)width, (int)height, (HDC) source->m_hDC,
             xsrc1, ysrc1, dwRop) != 0);
    }
    ::SetTextColor(GetHdc(), old_textground);
    ::SetBkColor(GetHdc(), old_background);

    return success;
}

void wxDC::DoGetSize(int *w, int *h) const
{
    if ( w ) *w = ::GetDeviceCaps(GetHdc(), HORZRES);
    if ( h ) *h = ::GetDeviceCaps(GetHdc(), VERTRES);
}

void wxDC::DoGetSizeMM(int *w, int *h) const
{
    if ( w ) *w = ::GetDeviceCaps(GetHdc(), HORZSIZE);
    if ( h ) *h = ::GetDeviceCaps(GetHdc(), VERTSIZE);
}

wxSize wxDC::GetPPI() const
{
    int x = ::GetDeviceCaps(GetHdc(), LOGPIXELSX);
    int y = ::GetDeviceCaps(GetHdc(), LOGPIXELSY);

    return wxSize(x, y);
}

// For use by wxWindows only, unless custom units are required.
void wxDC::SetLogicalScale(double x, double y)
{
    m_logicalScaleX = x;
    m_logicalScaleY = y;
}

#if WXWIN_COMPATIBILITY
void wxDC::DoGetTextExtent(const wxString& string, float *x, float *y,
                         float *descent, float *externalLeading,
                         wxFont *theFont, bool use16bit) const
{
    wxCoord x1, y1, descent1, externalLeading1;
    GetTextExtent(string, & x1, & y1, & descent1, & externalLeading1, theFont, use16bit);
    *x = x1; *y = y1;
    if (descent)
        *descent = descent1;
    if (externalLeading)
        *externalLeading = externalLeading1;
}
#endif

// ---------------------------------------------------------------------------
// spline drawing code
// ---------------------------------------------------------------------------

#if wxUSE_SPLINES

class wxSpline: public wxObject
{
public:
    int type;
    wxList *points;

    wxSpline(wxList *list);
    void DeletePoints();

    // Doesn't delete points
    ~wxSpline();
};

void wx_draw_open_spline(wxDC *dc, wxSpline *spline);

void wx_quadratic_spline(double a1, double b1, double a2, double b2,
                         double a3, double b3, double a4, double b4);
void wx_clear_stack();
int wx_spline_pop(double *x1, double *y1, double *x2, double *y2, double *x3,
                  double *y3, double *x4, double *y4);
void wx_spline_push(double x1, double y1, double x2, double y2, double x3, double y3,
                    double x4, double y4);
static bool wx_spline_add_point(double x, double y);
static void wx_spline_draw_point_array(wxDC *dc);
wxSpline *wx_make_spline(int x1, int y1, int x2, int y2, int x3, int y3);

void wxDC::DoDrawSpline(wxList *list)
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

#define     half(z1, z2)    ((z1+z2)/2.0)
#define     THRESHOLD   5

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

void wx_clear_stack()
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
    wxPoint *point = new wxPoint;
    point->x = (int) x;
    point->y = (int) y;
    wx_spline_point_list.Append((wxObject*)point);
    return TRUE;
}

static void wx_spline_draw_point_array(wxDC *dc)
{
    dc->DrawLines(&wx_spline_point_list, 0, 0);
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

wxSpline::~wxSpline()
{
}

void wxSpline::DeletePoints()
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

