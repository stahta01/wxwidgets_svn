/////////////////////////////////////////////////////////////////////////////
// Name:        colour.cpp
// Purpose:     wxColour class
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "colour.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include "wx/gdicmn.h"
#include "wx/msw/private.h"

#include <string.h>

#if wxUSE_EXTENDED_RTTI
IMPLEMENT_DYNAMIC_CLASS_WITH_COPY_XTI( wxColour , wxObject , "wx/colour.h" ) 

WX_BEGIN_PROPERTIES_TABLE(wxColour)
	WX_READONLY_PROPERTY( Red, unsigned char , Red , 0 )
	WX_READONLY_PROPERTY( Green, unsigned char  , Green , 0 ) 
	WX_READONLY_PROPERTY( Blue, unsigned char , Blue , 0 )
WX_END_PROPERTIES_TABLE()

WX_CONSTRUCTOR_3( wxColour , unsigned char , Red , unsigned char , Green , unsigned char , Blue )  

WX_BEGIN_HANDLERS_TABLE(wxColour)
WX_END_HANDLERS_TABLE()
#else
IMPLEMENT_DYNAMIC_CLASS(wxColour, wxObject)
#endif

// Colour

wxColour::wxColour ()
{
  m_isInit = FALSE;
  m_pixel = 0;
  m_red = m_blue = m_green = 0;
}

wxColour::wxColour (unsigned char r, unsigned char g, unsigned char b)
{
  m_red = r;
  m_green = g;
  m_blue = b;
  m_isInit = TRUE;
  m_pixel = PALETTERGB (m_red, m_green, m_blue);
}

wxColour::wxColour (const wxColour& col)
{
  m_red = col.m_red;
  m_green = col.m_green;
  m_blue = col.m_blue;
  m_isInit = col.m_isInit;
  m_pixel = col.m_pixel;
}

wxColour& wxColour::operator =(const wxColour& col)
{
  m_red = col.m_red;
  m_green = col.m_green;
  m_blue = col.m_blue;
  m_isInit = col.m_isInit;
  m_pixel = col.m_pixel;
  return *this;
}

void wxColour::InitFromName(const wxString& col)
{
  wxColour *the_colour = wxTheColourDatabase->FindColour (col);
  if (the_colour)
    {
      m_red = the_colour->Red ();
      m_green = the_colour->Green ();
      m_blue = the_colour->Blue ();
      m_isInit = TRUE;
    }
  else
    {
      m_red = 0;
      m_green = 0;
      m_blue = 0;
      m_isInit = FALSE;
    }
  m_pixel = PALETTERGB (m_red, m_green, m_blue);
}

wxColour::~wxColour()
{
}

void wxColour::Set (unsigned char r, unsigned char g, unsigned char b)
{
  m_red = r;
  m_green = g;
  m_blue = b;
  m_isInit = TRUE;
  m_pixel = PALETTERGB (m_red, m_green, m_blue);
}

// Obsolete
#if WXWIN_COMPATIBILITY
void wxColour::Get (unsigned char *r, unsigned char *g, unsigned char *b) const
{
  *r = m_red;
  *g = m_green;
  *b = m_blue;
}
#endif

