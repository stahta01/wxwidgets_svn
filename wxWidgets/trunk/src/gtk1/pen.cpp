/////////////////////////////////////////////////////////////////////////////
// Name:        pen.cpp
// Purpose:
// Author:      Robert Roebling
// Created:     01/02/97
// Id:
// Copyright:   (c) 1998 Robert Roebling, Julian Smart and Markus Holzem
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#ifdef __GNUG__
#pragma implementation "pen.h"
#endif

#include "wx/pen.h"

//-----------------------------------------------------------------------------
// wxPen
//-----------------------------------------------------------------------------

class wxPenRefData: public wxObjectRefData
{
  public:
  
    wxPenRefData(void);
    wxPenRefData(const wxPenRefData& data);
  
    int        m_width;
    int        m_style;
    int        m_joinStyle;
    int        m_capStyle;
    wxColour   m_colour;
};

wxPenRefData::wxPenRefData(void)
{
  m_width = 1;
  m_style = wxSOLID;
  m_joinStyle = wxJOIN_ROUND;
  m_capStyle = wxCAP_ROUND;
}

wxPenRefData::wxPenRefData( const wxPenRefData& data )
{
  m_style = data.m_style;
  m_width = data.m_width;
  m_joinStyle = data.m_joinStyle;
  m_capStyle = data.m_capStyle;
  m_colour = data.m_colour;
}

//-----------------------------------------------------------------------------

#define M_PENDATA ((wxPenRefData *)m_refData)

IMPLEMENT_DYNAMIC_CLASS(wxPen,wxGDIObject)

wxPen::wxPen(void)
{
  if (wxThePenList) wxThePenList->AddPen( this );
}

wxPen::wxPen( const wxColour &colour, int width, int style )
{
  m_refData = new wxPenRefData();
  M_PENDATA->m_width = width;
  M_PENDATA->m_style = style;
  M_PENDATA->m_colour = colour;
  
  if (wxThePenList) wxThePenList->AddPen( this );
}

wxPen::wxPen( const wxPen& pen )
{
  Ref( pen );
  if (wxThePenList) wxThePenList->AddPen( this );
}

wxPen::wxPen( const wxPen* pen )
{
  UnRef();
  if (pen) Ref( *pen ); 
  
  if (wxThePenList) wxThePenList->AddPen( this );
}

wxPen::~wxPen(void)
{
  if (wxThePenList) wxThePenList->RemovePen( this );
}

wxPen& wxPen::operator = ( const wxPen& pen )
{
  if (*this == pen) return (*this); 
  Ref( pen ); 
  return *this; 
}

bool wxPen::operator == ( const wxPen& pen )
{
  return m_refData == pen.m_refData; 
}

bool wxPen::operator != ( const wxPen& pen )
{
  return m_refData != pen.m_refData; 
}

void wxPen::SetColour( const wxColour &colour )
{
  Unshare();
  M_PENDATA->m_colour = colour;
}

void wxPen::SetColour( int red, int green, int blue )
{
  Unshare();
  M_PENDATA->m_colour.Set( red, green, blue );
}

void wxPen::SetCap( int capStyle )
{
  Unshare();
  M_PENDATA->m_capStyle = capStyle;
}

void wxPen::SetJoin( int joinStyle )
{
  Unshare();
  M_PENDATA->m_joinStyle = joinStyle;
}

void wxPen::SetStyle( int style )
{
  Unshare();
  M_PENDATA->m_style = style;
}

void wxPen::SetWidth( int width )
{
  Unshare();
  M_PENDATA->m_width = width;
}

int wxPen::GetCap(void) const
{
  if (!m_refData)
  {
    wxFAIL_MSG( "invalid pen" );
    return -1;
  }
  
  return M_PENDATA->m_capStyle;
}

int wxPen::GetJoin(void) const
{
  if (!m_refData)
  {
    wxFAIL_MSG( "invalid pen" );
    return -1;
  }
  
  return M_PENDATA->m_joinStyle;
}

int wxPen::GetStyle(void) const
{
  if (!m_refData)
  {
    wxFAIL_MSG( "invalid pen" );
    return -1;
  }
  
  return M_PENDATA->m_style;
}

int wxPen::GetWidth(void) const
{
  if (!m_refData)
  {
    wxFAIL_MSG( "invalid pen" );
    return -1;
  }
  
  return M_PENDATA->m_width;
}

wxColour &wxPen::GetColour(void) const
{
  if (!m_refData)
  {
    wxFAIL_MSG( "invalid pen" );
    return wxNullColour;
  }
  
  return M_PENDATA->m_colour;
}

bool wxPen::Ok(void) const
{
  return (m_refData != NULL);
}

void wxPen::Unshare(void)
{
  if (!m_refData)
  {
    m_refData = new wxPenRefData();
  }
  else
  {
    wxPenRefData* ref = new wxPenRefData( *(wxPenRefData*)m_refData );
    UnRef();
    m_refData = ref;
  }
}

