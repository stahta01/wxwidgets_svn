/////////////////////////////////////////////////////////////////////////////
// Name:        palette.cpp
// Purpose:
// Author:      Robert Roebling
// Created:     01/02/97
// Id:
// Copyright:   (c) 1998 Robert Roebling, Julian Smart and Markus Holzem
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#ifdef __GNUG__
#pragma implementation "palette.h"
#endif

#include "wx/palette.h"

//-----------------------------------------------------------------------------
// wxPalette
//-----------------------------------------------------------------------------

class wxPaletteRefData: public wxObjectRefData
{
  public:
  
    wxPaletteRefData(void);
    ~wxPaletteRefData(void);
  
    GdkColormap  *m_colormap;
};

wxPaletteRefData::wxPaletteRefData(void)
{
  m_colormap = NULL;
};

wxPaletteRefData::~wxPaletteRefData(void)
{
  if (m_colormap) gdk_colormap_unref( m_colormap );
};

//-----------------------------------------------------------------------------

#define M_PALETTEDATA ((wxPaletteRefData *)m_refData)

IMPLEMENT_DYNAMIC_CLASS(wxPalette,wxGDIObject)

wxPalette::wxPalette(void)
{
};

wxPalette::wxPalette( const int n, const unsigned char *red, const unsigned char *green, const unsigned char *blue )
{
  m_refData = new wxPaletteRefData();
  Create( n, red, green, blue );
};

wxPalette::wxPalette( const wxPalette& palette )
{
  Ref( palette );
};

wxPalette::wxPalette( const wxPalette* palette )
{
  UnRef();
  if (palette) Ref( *palette ); 
};

wxPalette::~wxPalette(void)
{
};

wxPalette& wxPalette::operator = ( const wxPalette& palette )
{
  if (*this == palette) return (*this); 
  Ref( palette ); 
  return *this; 
};

bool wxPalette::operator == ( const wxPalette& palette )
{
  return m_refData == palette.m_refData; 
};

bool wxPalette::operator != ( const wxPalette& palette )
{
  return m_refData != palette.m_refData; 
};

bool wxPalette::Ok(void) const
{
  return (m_refData);
};

bool wxPalette::Create( const int n, const unsigned char *red, const unsigned char *green, const unsigned char *blue)
{
};

int wxPalette::GetPixel( const unsigned char red, const unsigned char green, const unsigned char blue ) const
{
};

bool wxPalette::GetRGB( const int pixel, unsigned char *red, unsigned char *green, unsigned char *blue ) const
{
};

