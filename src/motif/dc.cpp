/////////////////////////////////////////////////////////////////////////////
// Name:        src/motif/dc.cpp
// Purpose:     wxDC class
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/dc.h"

#ifndef WX_PRECOMP
    #include "wx/dcmemory.h"
#endif

IMPLEMENT_ABSTRACT_CLASS(wxDC, wxObject)

//-----------------------------------------------------------------------------
// wxDC
//-----------------------------------------------------------------------------

wxDC::wxDC()
{
    m_ok = false;

    m_backgroundMode = wxTRANSPARENT;

    m_isInteractive = false;
}

void wxDC::DoDrawIcon( const wxIcon &icon, wxCoord x, wxCoord y)
{
    wxCHECK_RET( Ok(), "invalid dc" );
    wxCHECK_RET( icon.Ok(), "invalid icon" );

    DoDrawBitmap(icon, x, y, true);
}

void wxDC::DoDrawBitmap( const wxBitmap& bitmap, wxCoord x, wxCoord y, bool useMask )
{
    wxCHECK_RET( bitmap.Ok(), "invalid bitmap" );

    wxMemoryDC memDC;
    memDC.SelectObjectAsSource(bitmap);

#if 0
    // Not sure if we need this. The mask should leave the masked areas as per
    // the original background of this DC.
    if (useMask)
    {
        // There might be transparent areas, so make these the same colour as this
        // DC
        memDC.SetBackground(* GetBackground());
        memDC.Clear();
    }
#endif // 0

    Blit(x, y, bitmap.GetWidth(), bitmap.GetHeight(), &memDC, 0, 0, wxCOPY, useMask);

    memDC.SelectObject(wxNullBitmap);
}

void wxDC::DoSetClippingRegion( wxCoord x, wxCoord y, wxCoord width, wxCoord height )
{
    m_clipping = true;
    m_clipX1 = x;
    m_clipY1 = y;
    m_clipX2 = x + width;
    m_clipY2 = y + height;
}

void wxDC::DoGetSize( int* width, int* height ) const
{
    if ( width )
        *width = m_maxX - m_minX;
    if ( height )
        *height = m_maxY - m_minY;
}

void wxDC::DoGetSizeMM( int* width, int* height ) const
{
    int w, h;
    GetSize( &w, &h );

    if ( width )
        *width = int( double(w) / (m_scaleX*m_mm_to_pix_x) );
    if ( height )
        *height = int( double(h) / (m_scaleY*m_mm_to_pix_y) );
}

// Resolution in pixels per logical inch
wxSize wxDC::GetPPI() const
{
    // TODO (should probably be pure virtual)
    return wxSize(0, 0);
}
