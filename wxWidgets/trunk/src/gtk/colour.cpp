/////////////////////////////////////////////////////////////////////////////
// Name:        colour.cpp
// Purpose:
// Author:      Robert Roebling
// Created:     01/02/97
// Id:
// Copyright:   (c) 1998 Robert Roebling, Julian Smart and Markus Holzem
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#ifdef __GNUG__
#pragma implementation "colour.h"
#endif

#include "wx/gdicmn.h"

#ifdef wxUSE_GDK_IMLIB
#include "../gdk_imlib/gdk_imlib.h"
#endif

//-----------------------------------------------------------------------------
// wxColour
//-----------------------------------------------------------------------------

class wxColourRefData: public wxObjectRefData
{
  public:
  
    wxColourRefData(void);
    ~wxColourRefData(void);
    void FreeColour(void);
  
    GdkColor     m_color;
    GdkColormap *m_colormap;
    bool         m_hasPixel;
    
    friend wxColour;
};

wxColourRefData::wxColourRefData(void)
{
  m_color.red = 0;
  m_color.green = 0;
  m_color.blue = 0;
  m_color.pixel = 0;
  m_colormap = (GdkColormap *) NULL;
  m_hasPixel = FALSE;
}

wxColourRefData::~wxColourRefData(void)
{
  FreeColour();
}

void wxColourRefData::FreeColour(void)
{
//  if (m_hasPixel) gdk_colors_free( m_colormap, &m_color, 1, 0 );
}

//-----------------------------------------------------------------------------

#define M_COLDATA ((wxColourRefData *)m_refData)

#define SHIFT (8*(sizeof(short int)-sizeof(char)))

IMPLEMENT_DYNAMIC_CLASS(wxColour,wxGDIObject)

wxColour::wxColour(void)
{
}

wxColour::wxColour( char red, char green, char blue )
{
  m_refData = new wxColourRefData();
  M_COLDATA->m_color.red = ((unsigned short)red) << SHIFT;
  M_COLDATA->m_color.green = ((unsigned short)green) << SHIFT;
  M_COLDATA->m_color.blue = ((unsigned short)blue) << SHIFT;
  M_COLDATA->m_color.pixel = 0;
}
  
wxColour::wxColour( const wxString &colourName )
{
  wxNode *node = (wxNode *) NULL;
  if ( (wxTheColourDatabase) && (node = wxTheColourDatabase->Find(colourName)) ) 
  {
    wxColour *col = (wxColour*)node->Data();
    UnRef();
    if (col) Ref( *col );
  } 
  else 
  {
    m_refData = new wxColourRefData();
    if (!gdk_color_parse( colourName, &M_COLDATA->m_color ))
    {
      wxFAIL_MSG( "wxColour: couldn't find colour" );
      delete m_refData;
      m_refData = (wxObjectRefData *) NULL;
    }
  }
}

wxColour::wxColour( const wxColour& col )
{ 
  Ref( col ); 
}

wxColour::wxColour( const wxColour* col ) 
{ 
  if (col) Ref( *col ); 
}

wxColour::~wxColour(void)
{
}

wxColour& wxColour::operator = ( const wxColour& col ) 
{ 
  if (*this == col) return (*this); 
  Ref( col ); 
  return *this; 
}

wxColour& wxColour::operator = ( const wxString& colourName ) 
{ 
  UnRef();
  wxNode *node = (wxNode *) NULL;
  if ((wxTheColourDatabase) && (node = wxTheColourDatabase->Find(colourName)) ) 
  {
    wxColour *col = (wxColour*)node->Data();
    if (col) Ref( *col );
  } 
  else 
  {
    m_refData = new wxColourRefData();
    if (!gdk_color_parse( colourName, &M_COLDATA->m_color ))
    {
      wxFAIL_MSG( "wxColour: couldn't find colour" );
      delete m_refData;
      m_refData = (wxObjectRefData *) NULL;
    }
  }
  return *this; 
}

bool wxColour::operator == ( const wxColour& col ) 
{ 
  return m_refData == col.m_refData; 
}

bool wxColour::operator != ( const wxColour& col) 
{ 
  return m_refData != col.m_refData; 
}

void wxColour::Set( const unsigned char red, const unsigned char green, const unsigned char blue )
{
  UnRef();
  m_refData = new wxColourRefData();
  M_COLDATA->m_color.red = ((unsigned short)red) << SHIFT;
  M_COLDATA->m_color.green = ((unsigned short)green) << SHIFT;
  M_COLDATA->m_color.blue = ((unsigned short)blue) << SHIFT;
  M_COLDATA->m_color.pixel = 0;
}

unsigned char wxColour::Red(void) const
{
  if (!Ok())
  {
     wxFAIL_MSG( "invalid colour" );
     return 0;
  }
  
  return (unsigned char)(M_COLDATA->m_color.red >> SHIFT);
}

unsigned char wxColour::Green(void) const
{
  if (!Ok())
  {
     wxFAIL_MSG( "invalid colour" );
     return 0;
  }
  
  return (unsigned char)(M_COLDATA->m_color.green >> SHIFT);
}

unsigned char wxColour::Blue(void) const
{
  if (!Ok())
  {
     wxFAIL_MSG( "invalid colour" );
     return 0;
  }
  
  return (unsigned char)(M_COLDATA->m_color.blue >> SHIFT);
}

bool wxColour::Ok(void) const
{
  return (m_refData != NULL);
}

void wxColour::CalcPixel( GdkColormap *cmap )
{
  if (!Ok()) return;
  
  if ((M_COLDATA->m_hasPixel) && (M_COLDATA->m_colormap == cmap)) return;
  M_COLDATA->FreeColour();
  
#ifdef wxUSE_GDK_IMLIB

  int r = M_COLDATA->m_color.red >> SHIFT;
  int g = M_COLDATA->m_color.green >> SHIFT;
  int b = M_COLDATA->m_color.blue >> SHIFT;
  M_COLDATA->m_hasPixel = TRUE;
  M_COLDATA->m_color.pixel = gdk_imlib_best_color_match( &r, &g, &b );

#else

  M_COLDATA->m_hasPixel = gdk_color_alloc( cmap, &M_COLDATA->m_color );
  
#endif
  
  M_COLDATA->m_colormap = cmap;
}

int wxColour::GetPixel(void)
{
  if (!Ok()) return 0;
  
  return M_COLDATA->m_color.pixel;
}

GdkColor *wxColour::GetColor(void)
{
  if (!Ok()) return (GdkColor *) NULL;
  
  return &M_COLDATA->m_color;
}


