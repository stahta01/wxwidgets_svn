/////////////////////////////////////////////////////////////////////////////
// Name:        dcclient.h
// Purpose:
// Author:      Robert Roebling
// Id:          $Id$
// Copyright:   (c) 1998 Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __GTKDCCLIENTH__
#define __GTKDCCLIENTH__

#include "wx/dc.h"
#include "wx/region.h"

class WXDLLIMPEXP_FWD_CORE wxWindow;

//-----------------------------------------------------------------------------
// wxWindowDC
//-----------------------------------------------------------------------------

#if wxUSE_NEW_DC
class WXDLLIMPEXP_CORE wxGTKWindowImplDC : public wxGTKImplDC
#else
#define wxGTKWindowImplDC wxWindowDC
class WXDLLIMPEXP_CORE wxWindowDC : public wxDC
#endif
{
public:
    wxGTKWindowImplDC();
    wxGTKWindowImplDC( wxWindow *win );

    virtual ~wxGTKWindowImplDC();

    virtual bool CanDrawBitmap() const { return true; }
    virtual bool CanGetTextExtent() const { return true; }

protected:
    virtual void DoGetSize(int *width, int *height) const;
    virtual bool DoFloodFill( wxCoord x, wxCoord y, const wxColour& col, int style=wxFLOOD_SURFACE );
    virtual bool DoGetPixel( wxCoord x1, wxCoord y1, wxColour *col ) const;

    virtual void DoDrawLine( wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2 );
    virtual void DoCrossHair( wxCoord x, wxCoord y );
    virtual void DoDrawArc( wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2,
                            wxCoord xc, wxCoord yc );
    virtual void DoDrawEllipticArc( wxCoord x, wxCoord y, wxCoord width, wxCoord height,
                                    double sa, double ea );
    virtual void DoDrawPoint( wxCoord x, wxCoord y );

    virtual void DoDrawLines(int n, wxPoint points[],
                             wxCoord xoffset, wxCoord yoffset);
    virtual void DoDrawPolygon(int n, wxPoint points[],
                               wxCoord xoffset, wxCoord yoffset,
                               int fillStyle = wxODDEVEN_RULE);

    virtual void DoDrawRectangle( wxCoord x, wxCoord y, wxCoord width, wxCoord height );
    virtual void DoDrawRoundedRectangle( wxCoord x, wxCoord y, wxCoord width, wxCoord height, double radius = 20.0 );
    virtual void DoDrawEllipse( wxCoord x, wxCoord y, wxCoord width, wxCoord height );

    virtual void DoDrawIcon( const wxIcon &icon, wxCoord x, wxCoord y );
    virtual void DoDrawBitmap( const wxBitmap &bitmap, wxCoord x, wxCoord y,
                               bool useMask = false );

    virtual bool DoBlit( wxCoord xdest, wxCoord ydest, wxCoord width, wxCoord height,
                         wxDC *source, wxCoord xsrc, wxCoord ysrc,
                         int logical_func = wxCOPY, bool useMask = false, wxCoord xsrcMask = -1, wxCoord ysrcMask = -1 );

    virtual void DoDrawText( const wxString &text, wxCoord x, wxCoord y );
    virtual void DoDrawRotatedText(const wxString& text, wxCoord x, wxCoord y,
                                   double angle);
    virtual void DoGetTextExtent( const wxString &string,
                                wxCoord *width, wxCoord *height,
                                wxCoord *descent = (wxCoord *) NULL,
                                wxCoord *externalLeading = (wxCoord *) NULL,
                                const wxFont *theFont = (wxFont *) NULL) const;
    virtual bool DoGetPartialTextExtents(const wxString& text, wxArrayInt& widths) const;
    virtual void DoSetClippingRegion( wxCoord x, wxCoord y, wxCoord width, wxCoord height );
    virtual void DoSetClippingRegionAsRegion( const wxRegion &region );


public:
    virtual wxCoord GetCharWidth() const;
    virtual wxCoord GetCharHeight() const;

    virtual void Clear();

    virtual void SetFont( const wxFont &font );
    virtual void SetPen( const wxPen &pen );
    virtual void SetBrush( const wxBrush &brush );
    virtual void SetBackground( const wxBrush &brush );
    virtual void SetLogicalFunction( int function );
    virtual void SetTextForeground( const wxColour &col );
    virtual void SetTextBackground( const wxColour &col );
    virtual void SetBackgroundMode( int mode );
    virtual void SetPalette( const wxPalette& palette );

    virtual void DestroyClippingRegion();

    // Resolution in pixels per logical inch
    virtual wxSize GetPPI() const;
    virtual int GetDepth() const;

    // overrriden here for RTL
    virtual void SetDeviceOrigin( wxCoord x, wxCoord y );
    virtual void SetAxisOrientation( bool xLeftRight, bool yBottomUp );

// protected:
    // implementation
    // --------------

    GdkWindow    *m_window;
    GdkGC        *m_penGC;
    GdkGC        *m_brushGC;
    GdkGC        *m_textGC;
    GdkGC        *m_bgGC;
    GdkColormap  *m_cmap;
    bool          m_isMemDC;
    bool          m_isScreenDC;
    wxWindow     *m_owner;
    wxRegion      m_currentClippingRegion;
    wxRegion      m_paintClippingRegion;

    // PangoContext stuff for GTK 2.0
    PangoContext *m_context;
    PangoLayout *m_layout;
    PangoFontDescription *m_fontdesc;

    void SetUpDC();
    void Destroy();
    
    virtual void ComputeScaleAndOrigin();

    virtual GdkWindow *GetGDKWindow() const { return m_window; }

private:
    DECLARE_DYNAMIC_CLASS(wxGTKWindowImplDC)
};

//-----------------------------------------------------------------------------
// wxClientDC
//-----------------------------------------------------------------------------

#if wxUSE_NEW_DC
class WXDLLIMPEXP_CORE wxGTKClientImplDC : public wxGTKWindowImplDC
#else
#define wxGTKClientImplDC wxClientDC
class WXDLLIMPEXP_CORE wxClientDC : public wxWindowDC
#endif
{
public:
    wxGTKClientImplDC() { }
    wxGTKClientImplDC( wxWindow *win );

protected:
    virtual void DoGetSize(int *width, int *height) const;

private:
    DECLARE_DYNAMIC_CLASS(wxGTKClientImplDC)
};

//-----------------------------------------------------------------------------
// wxPaintDC
//-----------------------------------------------------------------------------

#if wxUSE_NEW_DC
class WXDLLIMPEXP_CORE wxGTKPaintImplDC : public wxGTKClientImplDC
#else
#define wxGTKPaintImplDC wxPaintDC
class WXDLLIMPEXP_CORE wxPaintDC : public wxClientDC
#endif
{
public:
    wxGTKPaintImplDC() { }
    wxGTKPaintImplDC( wxWindow *win );

private:
    DECLARE_DYNAMIC_CLASS(wxGTKPaintImplDC)
};

#endif // __GTKDCCLIENTH__
