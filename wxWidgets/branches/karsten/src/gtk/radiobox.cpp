/////////////////////////////////////////////////////////////////////////////
// Name:        radiobox.cpp
// Purpose:
// Author:      Robert Roebling
// Created:     01/02/97
// Id:
// Copyright:   (c) 1998 Robert Roebling, Julian Smart and Markus Holzem
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#ifdef __GNUG__
#pragma implementation "radiobox.h"
#endif

#include "wx/radiobox.h"
#include "wx/dialog.h"
#include "wx/frame.h"
#include "wx/gtk/win_gtk.h"

//-----------------------------------------------------------------------------
// wxRadioBox
//-----------------------------------------------------------------------------

void gtk_radiobutton_clicked_callback( GtkWidget *WXUNUSED(widget), gpointer data )
{
  wxRadioBox *rb = (wxRadioBox*)data;
  wxCommandEvent event( wxEVT_COMMAND_RADIOBOX_SELECTED, rb->GetId() );
  event.SetInt( rb->GetSelection() );
  wxString tmp( rb->GetStringSelection() );
  event.SetString( WXSTRINGCAST(tmp) );
  event.SetEventObject( rb );
  rb->ProcessEvent(event);
};

//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxRadioBox,wxControl)

wxRadioBox::wxRadioBox(void)
{
};

wxRadioBox::wxRadioBox( wxWindow *parent, const wxWindowID id, const wxString& title,
      const wxPoint &pos, const wxSize &size, 
      const int n, const wxString choices[],
      const int majorDim, const long style,
      const wxString &name )
{
  Create( parent, id, title, pos, size, n, choices, majorDim, style, name );
};

bool wxRadioBox::Create( wxWindow *parent, const wxWindowID id, const wxString& title,
      const wxPoint &pos, const wxSize &size, 
      const int n, const wxString choices[],
      const int WXUNUSED(majorDim), const long style,
      const wxString &name )
{
  m_needParent = TRUE;
  
  PreCreation( parent, id, pos, size, style, name );

  m_widget = gtk_frame_new( title );
  
  int x = m_x+5;
  int y = m_y+15;
  int maxLen = 0;
  int height = 20;
  
//  if (((m_style & wxRA_VERTICAL) == wxRA_VERTICAL) && (n > 0))
  if (n > 0)
  {
    GSList *radio_button_group = NULL;
    for (int i = 0; i < n; i++)
    {
      if (i) radio_button_group = gtk_radio_button_group( GTK_RADIO_BUTTON(m_radio) );
      m_radio = GTK_RADIO_BUTTON( gtk_radio_button_new_with_label( radio_button_group, choices[i] ) );
      
      if (!i) gtk_toggle_button_set_state( GTK_TOGGLE_BUTTON(m_radio), TRUE );
      
      gtk_signal_connect( GTK_OBJECT(m_radio), "clicked", 
        GTK_SIGNAL_FUNC(gtk_radiobutton_clicked_callback), (gpointer*)this );
       
      gtk_myfixed_put( GTK_MYFIXED(m_parent->m_wxwindow), GTK_WIDGET(m_radio), x, y );
      
      int tmp = 22+gdk_string_measure( GTK_WIDGET(m_radio)->style->font, choices[i] );
      if (tmp > maxLen) maxLen = tmp;
      
      int width = m_width-10;
      if (size.x == -1) width = tmp;
      gtk_widget_set_usize( GTK_WIDGET(m_radio), width, 20 );
      
      y += 20;
      height += 20;
      
    };
  };

  wxSize newSize = size;
  if (newSize.x == -1) newSize.x = maxLen+10;
  if (newSize.y == -1) newSize.y = height;
  SetSize( newSize.x, newSize.y );
  
  PostCreation();
  
  Show( TRUE );
    
  return TRUE;
};

bool wxRadioBox::Show( const bool show )
{
  wxWindow::Show( show );

  GSList *item = gtk_radio_button_group( m_radio );
  while (item)
  {
    GtkWidget *w = GTK_WIDGET( item->data );
    if (show) gtk_widget_show( w ); else gtk_widget_hide( w );
    item = item->next;
  };

  return TRUE;
};

int wxRadioBox::FindString( const wxString& WXUNUSED(s) ) const
{
  return 0;
};

void wxRadioBox::SetSelection( const int WXUNUSED(n) )
{
};

int wxRadioBox::GetSelection(void) const
{
  GSList *item = gtk_radio_button_group( m_radio );
  int count = 0;
  while (item)
  {
    GtkButton *button = GTK_BUTTON( item->data );
    if (GTK_TOGGLE_BUTTON(button)->active) return count;
    count++;
    item = item->next;
  };
  return -1;
};

wxString wxRadioBox::GetString( const int WXUNUSED(n) ) const
{
  return "";
};

wxString wxRadioBox::GetLabel(void) const
{
  return wxControl::GetLabel();
};

void wxRadioBox::SetLabel( const wxString& WXUNUSED(label) )
{
};

void wxRadioBox::SetLabel( const int WXUNUSED(item), const wxString& WXUNUSED(label) )
{
};

void wxRadioBox::SetLabel( const int WXUNUSED(item), wxBitmap *WXUNUSED(bitmap) )
{
};

wxString wxRadioBox::GetLabel( const int WXUNUSED(item) ) const
{
  return "";
};

void wxRadioBox::Enable( const bool WXUNUSED(enable) )
{
};

void wxRadioBox::Enable( const int WXUNUSED(item), const bool WXUNUSED(enable) )
{
};

void wxRadioBox::Show( const int WXUNUSED(item), const bool WXUNUSED(show) )
{
};

wxString wxRadioBox::GetStringSelection(void) const
{
  GSList *item = gtk_radio_button_group( m_radio );
  while (item)
  {
    GtkButton *button = GTK_BUTTON( item->data );
    if (GTK_TOGGLE_BUTTON(button)->active)
    {
      GtkLabel *label = GTK_LABEL( button->child );
      return label->label;
    };
    item = item->next;
  };
  return "";
};

bool wxRadioBox::SetStringSelection( const wxString& WXUNUSED(s) )
{
  return TRUE;
};

int wxRadioBox::Number(void) const
{
  int count = 0;
  GSList *item = gtk_radio_button_group( m_radio );
  while (item)
  {
    item = item->next;
    count++;
  };
  return count;
};

int wxRadioBox::GetNumberOfRowsOrCols(void) const
{
  return 1;
};

void wxRadioBox::SetNumberOfRowsOrCols( const int WXUNUSED(n) )
{
};

