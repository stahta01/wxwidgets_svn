/////////////////////////////////////////////////////////////////////////////
// Name:        src/gtk/brush.cpp
// Purpose:
// Author:      Robert Roebling
// Id:          $Id$
// Copyright:   (c) 1998 Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "brush.h"
#endif

#include "wx/brush.h"

#include <gdk/gdk.h>

//-----------------------------------------------------------------------------
// wxBrush
//-----------------------------------------------------------------------------

class wxBrushRefData: public wxObjectRefData
{
public:
    wxBrushRefData();
    wxBrushRefData( const wxBrushRefData& data );

    int        m_style;
    wxBitmap   m_stipple;
    wxColour   m_colour;
};

wxBrushRefData::wxBrushRefData()
{
    m_style = 0;
}

wxBrushRefData::wxBrushRefData( const wxBrushRefData& data )
{
    m_style = data.m_style;
    m_stipple = data.m_stipple;
    m_colour = data.m_colour;
}

//-----------------------------------------------------------------------------

#define M_BRUSHDATA ((wxBrushRefData *)m_refData)

IMPLEMENT_DYNAMIC_CLASS(wxBrush,wxGDIObject)

wxBrush::wxBrush()
{
}

wxBrush::wxBrush( const wxColour &colour, int style )
{
    m_refData = new wxBrushRefData();
    M_BRUSHDATA->m_style = style;
    M_BRUSHDATA->m_colour = colour;
}

wxBrush::wxBrush( const wxBitmap &stippleBitmap )
{
    m_refData = new wxBrushRefData();
    M_BRUSHDATA->m_colour = *wxBLACK;

    M_BRUSHDATA->m_stipple = stippleBitmap;

    if (M_BRUSHDATA->m_stipple.GetMask())
        M_BRUSHDATA->m_style = wxSTIPPLE_MASK_OPAQUE;
    else
        M_BRUSHDATA->m_style = wxSTIPPLE;
}

wxBrush::wxBrush( const wxBrush &brush )
{
    Ref( brush );
}

wxBrush::~wxBrush()
{
}

wxBrush& wxBrush::operator = ( const wxBrush& brush )
{
    if ( m_refData != brush.m_refData )
        Ref( brush );

    return *this;
}

bool wxBrush::operator == ( const wxBrush& brush ) const
{
    return m_refData == brush.m_refData;
}

bool wxBrush::operator != ( const wxBrush& brush ) const
{
    return m_refData != brush.m_refData;
}

bool wxBrush::Ok() const
{
    return ((m_refData) && M_BRUSHDATA->m_colour.Ok());
}

int wxBrush::GetStyle() const
{
    if (m_refData == NULL)
    {
        wxFAIL_MSG( wxT("invalid brush") );
        return 0;
    }

    return M_BRUSHDATA->m_style;
}

wxColour &wxBrush::GetColour() const
{
    if (m_refData == NULL)
    {
        wxFAIL_MSG( wxT("invalid brush") );
        return wxNullColour;
    }

    return M_BRUSHDATA->m_colour;
}

wxBitmap *wxBrush::GetStipple() const
{
    if (m_refData == NULL)
    {
        wxFAIL_MSG( wxT("invalid brush") );
        return &wxNullBitmap;
    }

    return &M_BRUSHDATA->m_stipple;
}

void wxBrush::SetColour( const wxColour& col )
{
    Unshare();
    M_BRUSHDATA->m_colour = col;
}

void wxBrush::SetColour( unsigned char r, unsigned char g, unsigned char b )
{
    Unshare();
    M_BRUSHDATA->m_colour.Set( r, g, b );
}

void wxBrush::SetStyle( int style )
{
    Unshare();
    M_BRUSHDATA->m_style = style;
}

void wxBrush::SetStipple( const wxBitmap& stipple )
{
    Unshare();
    M_BRUSHDATA->m_stipple = stipple;
    if (M_BRUSHDATA->m_stipple.GetMask())
    {
        M_BRUSHDATA->m_style = wxSTIPPLE_MASK_OPAQUE;
    }
    else
    {
        M_BRUSHDATA->m_style = wxSTIPPLE;
    }
}

void wxBrush::Unshare()
{
    if (!m_refData)
    {
        m_refData = new wxBrushRefData();
    }
    else
    {
        wxBrushRefData* ref = new wxBrushRefData( *(wxBrushRefData*)m_refData );
        UnRef();
        m_refData = ref;
    }
}

