/////////////////////////////////////////////////////////////////////////////
// Name:        slider.cpp
// Purpose:
// Author:      Robert Roebling
// Created:     01/02/97
// Id:
// Copyright:   (c) 1998 Robert Roebling, Julian Smart and Markus Holzem
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#ifdef __GNUG__
#pragma implementation "slider.h"
#endif

#include "wx/slider.h"
#include "wx/utils.h"

//-----------------------------------------------------------------------------
// data
//-----------------------------------------------------------------------------

extern bool   g_blockEventsOnDrag;

//-----------------------------------------------------------------------------
// wxSlider
//-----------------------------------------------------------------------------

static void gtk_slider_callback( GtkWidget *WXUNUSED(widget), wxSlider *win )
{ 
  if (!win->HasVMT()) return;
  if (g_blockEventsOnDrag) return;
    
  float diff = win->m_adjust->value - win->m_oldPos;
  if (fabs(diff) < 0.2) return;
  
  wxEventType command = wxEVT_NULL;
  
  float line_step = win->m_adjust->step_increment;
  float page_step = win->m_adjust->page_increment;
  
  if (fabs(diff-line_step) < 0.2) command = wxEVT_SCROLL_LINEDOWN;
  else if (fabs(diff+line_step) < 0.2) command = wxEVT_SCROLL_LINEUP;
  else if (fabs(diff-page_step) < 0.2) command = wxEVT_SCROLL_PAGEDOWN;
  else if (fabs(diff+page_step) < 0.2) command = wxEVT_SCROLL_PAGEUP;
  else command = wxEVT_SCROLL_THUMBTRACK;

  int value = (int)(win->m_adjust->value+0.5);
      
  int orient = wxHORIZONTAL;
  if (win->GetWindowStyleFlag() & wxSB_VERTICAL == wxSB_VERTICAL) orient = wxHORIZONTAL;
  
  wxScrollEvent event( command, win->GetId(), value, orient );
  event.SetEventObject( win );
  win->ProcessEvent( event );
  
  wxCommandEvent cevent( wxEVT_COMMAND_SLIDER_UPDATED, win->GetId() );
  cevent.SetEventObject( win );
  win->ProcessEvent( cevent );
}

IMPLEMENT_DYNAMIC_CLASS(wxSlider,wxControl)

wxSlider::wxSlider(void)
{
}

wxSlider::~wxSlider(void)
{
}

bool wxSlider::Create(wxWindow *parent, wxWindowID id,
        int value, int minValue, int maxValue,
        const wxPoint& pos, const wxSize& size,
        long style, const wxValidator& validator, const wxString& name )
{
  m_needParent = TRUE;
  
  PreCreation( parent, id, pos, size, style, name );
  
  SetValidator( validator );

  m_oldPos = 0.0;

  if (style & wxSL_VERTICAL == wxSL_VERTICAL)
    m_widget = gtk_hscale_new( NULL );
  else
    m_widget = gtk_vscale_new( NULL );
    
  m_adjust = gtk_range_get_adjustment( GTK_RANGE(m_widget) );
  
  gtk_signal_connect (GTK_OBJECT (m_adjust), "value_changed",
		      (GtkSignalFunc) gtk_slider_callback, (gpointer) this );
  SetRange( minValue, maxValue );
  SetValue( value );
  
  PostCreation();
  
  Show( TRUE );
    
  return TRUE;
}

int wxSlider::GetValue(void) const
{
  return (int)(m_adjust->value+0.5);
}

void wxSlider::SetValue( int value )
{
  float fpos = (float)value;
  m_oldPos = fpos;
  if (fabs(fpos-m_adjust->value) < 0.2) return;
  m_adjust->value = fpos;
  
  gtk_signal_emit_by_name( GTK_OBJECT(m_adjust), "value_changed" );
}

void wxSlider::SetRange( int minValue, int maxValue )
{
  float fmin = (float)minValue;
  float fmax = (float)maxValue;
      
  if ((fabs(fmin-m_adjust->lower) < 0.2) &&
      (fabs(fmax-m_adjust->upper) < 0.2))
      return;
      
  m_adjust->lower = fmin;
  m_adjust->upper = fmax;

  gtk_signal_emit_by_name( GTK_OBJECT(m_adjust), "changed" );
}

int wxSlider::GetMin(void) const
{
  return (int)(m_adjust->lower+0.5);
}

int wxSlider::GetMax(void) const
{
  return (int)(m_adjust->upper+0.5);
}

void wxSlider::SetPageSize( int pageSize )
{
  float fpage = (float)pageSize;
      
  if (fabs(fpage-m_adjust->page_increment) < 0.2) return;
      
  m_adjust->page_increment = fpage;

  gtk_signal_emit_by_name( GTK_OBJECT(m_adjust), "changed" );
}

int wxSlider::GetPageSize(void) const
{
  return (int)(m_adjust->page_increment+0.5);
}

void wxSlider::SetThumbLength( int len )
{
  float flen = (float)len;
      
  if (fabs(flen-m_adjust->page_size) < 0.2) return;
      
  m_adjust->page_size = flen;

  gtk_signal_emit_by_name( GTK_OBJECT(m_adjust), "changed" );
}

int wxSlider::GetThumbLength(void) const
{
  return (int)(m_adjust->page_size+0.5);
}

void wxSlider::SetLineSize( int WXUNUSED(lineSize) )
{
}

int wxSlider::GetLineSize(void) const
{
  return 0;
}

// not supported in wxGTK (and GTK)

void wxSlider::GetSize( int *x, int *y ) const
{
  wxWindow::GetSize( x, y );
}

void wxSlider::SetSize( int x, int y, int width, int height, int sizeFlags )
{
  wxWindow::SetSize( x, y, width, height, sizeFlags );
}

void wxSlider::GetPosition( int *x, int *y ) const
{
  wxWindow::GetPosition( x, y );
}

void wxSlider::SetTick( int WXUNUSED(tickPos) )
{
}

void wxSlider::SetTickFreq( int WXUNUSED(n), int WXUNUSED(pos) )
{
}

int wxSlider::GetTickFreq(void) const
{
  return 0;
}

void wxSlider::ClearTicks(void)
{
}

void wxSlider::SetSelection( int WXUNUSED(minPos), int WXUNUSED(maxPos) )
{
}

int wxSlider::GetSelEnd(void) const
{
  return 0;
}

int wxSlider::GetSelStart(void) const
{
  return 0;
}

void wxSlider::ClearSel(void)
{
}

