/////////////////////////////////////////////////////////////////////////////
// Name:        button.cpp
// Purpose:
// Author:      Robert Roebling
// Created:     01/02/97
// Id:
// Copyright:   (c) 1998 Robert Roebling, Julian Smart and Markus Holzem
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "button.h"
#endif

#include "wx/button.h"

//-----------------------------------------------------------------------------
// classes
//-----------------------------------------------------------------------------

class wxButton;

//-----------------------------------------------------------------------------
// data
//-----------------------------------------------------------------------------

extern bool   g_blockEventsOnDrag;

//-----------------------------------------------------------------------------
// "clicked"
//-----------------------------------------------------------------------------

static void gtk_button_clicked_callback( GtkWidget *WXUNUSED(widget), wxButton *button )
{
  if (!button->HasVMT()) return;
  if (g_blockEventsOnDrag) return;
  
  wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, button->GetId());
  event.SetEventObject(button);
  button->GetEventHandler()->ProcessEvent(event);
}

//-----------------------------------------------------------------------------
// wxButton
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxButton,wxControl)

wxButton::wxButton(void)
{
}

bool wxButton::Create(  wxWindow *parent, wxWindowID id, const wxString &label,
      const wxPoint &pos, const wxSize &size,
      long style, const wxValidator& validator, const wxString &name )
{
  m_needParent = TRUE;

  wxSize newSize = size;

  PreCreation( parent, id, pos, newSize, style, name );
  
  SetValidator( validator );

  m_widget = gtk_button_new_with_label( m_label );
  SetLabel(label);

  if (newSize.x == -1) newSize.x = 15+gdk_string_measure( m_widget->style->font, label );
  if (newSize.y == -1) newSize.y = 26;
  SetSize( newSize.x, newSize.y );

  gtk_signal_connect( GTK_OBJECT(m_widget), "clicked",
    GTK_SIGNAL_FUNC(gtk_button_clicked_callback), (gpointer*)this );

  PostCreation();

  Show( TRUE );

  return TRUE;
}

void wxButton::SetDefault(void)
{
/*
  GTK_WIDGET_SET_FLAGS( m_widget, GTK_CAN_DEFAULT );
  gtk_widget_grab_default( m_widget );
*/
}

void wxButton::SetLabel( const wxString &label )
{
  wxControl::SetLabel( label );
  GtkButton *bin = GTK_BUTTON( m_widget );
  GtkLabel *g_label = GTK_LABEL( bin->child );
  gtk_label_set( g_label, GetLabel() );
}

void wxButton::Enable( bool enable )
{
  wxControl::Enable( enable );
  GtkButton *bin = GTK_BUTTON( m_widget );
  GtkWidget *label = bin->child;
  gtk_widget_set_sensitive( label, enable );
}

void wxButton::SetFont( const wxFont &font )
{
  if (((wxFont*)&font)->Ok())
    m_font = font;
  else
    m_font = *wxSWISS_FONT;

  GtkButton *bin = GTK_BUTTON( m_widget );
  GtkWidget *label = bin->child;
  
  GtkStyle *style = (GtkStyle*) NULL;
  if (!m_hasOwnStyle)
  {
    m_hasOwnStyle = TRUE;
    style = gtk_style_copy( gtk_widget_get_style( label ) );
  }
  else
  {
    style = gtk_widget_get_style( label );
  }
  
  gdk_font_unref( style->font );
  style->font = gdk_font_ref( m_font.GetInternalFont( 1.0 ) );
  
  gtk_widget_set_style( label, style );
}
