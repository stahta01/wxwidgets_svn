/////////////////////////////////////////////////////////////////////////////
// Name:        radiobox.cpp
// Purpose:
// Author:      Robert Roebling
// Id:          $Id$
// Copyright:   (c) 1998 Robert Roebling
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
// data
//-----------------------------------------------------------------------------

extern bool   g_blockEventsOnDrag;

//-----------------------------------------------------------------------------
// "clicked"
//-----------------------------------------------------------------------------

static void gtk_radiobutton_clicked_callback( GtkWidget *WXUNUSED(widget), wxRadioBox *rb )
{
  if (!rb->HasVMT()) return;
  if (g_blockEventsOnDrag) return;
  
  if (rb->m_alreadySent)
  {
    rb->m_alreadySent = FALSE;
    return;
  }

  rb->m_alreadySent = TRUE;
  
  wxCommandEvent event( wxEVT_COMMAND_RADIOBOX_SELECTED, rb->GetId() );
  event.SetInt( rb->GetSelection() );
  wxString tmp( rb->GetStringSelection() );
  event.SetString( WXSTRINGCAST(tmp) );
  event.SetEventObject( rb );
  rb->GetEventHandler()->ProcessEvent(event);
}

//-----------------------------------------------------------------------------
// wxRadioBox
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxRadioBox,wxControl)

BEGIN_EVENT_TABLE(wxRadioBox, wxControl)
  EVT_SIZE(wxRadioBox::OnSize)
END_EVENT_TABLE()

wxRadioBox::wxRadioBox(void)
{
}

bool wxRadioBox::Create( wxWindow *parent, wxWindowID id, const wxString& title,
      const wxPoint &pos, const wxSize &size,
      int n, const wxString choices[], int WXUNUSED(majorDim), 
      long style, const wxValidator& validator, const wxString &name )
{
  m_alreadySent = FALSE;
  m_needParent = TRUE;
  
  PreCreation( parent, id, pos, size, style, name );

  SetValidator( validator );

  m_widget = gtk_frame_new( title );
  
  int x = m_x+5;
  int y = m_y+15;
  int maxLen = 0;
  int height = 20;
  
  GtkRadioButton *m_radio = (GtkRadioButton*) NULL;
  
//  if (((m_style & wxRA_VERTICAL) == wxRA_VERTICAL) && (n > 0))
  if (n > 0)
  {
    GSList *radio_button_group = (GSList *) NULL;
    for (int i = 0; i < n; i++)
    {
      if (i) radio_button_group = gtk_radio_button_group( GTK_RADIO_BUTTON(m_radio) );
      
      m_radio = GTK_RADIO_BUTTON( gtk_radio_button_new_with_label( radio_button_group, choices[i] ) );
      
      m_boxes.Append( (wxObject*) m_radio );
      
      ConnectWidget( GTK_WIDGET(m_radio) );
  
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
      
    }
  }
  
  wxSize newSize = size;
  if (newSize.x == -1) newSize.x = maxLen+10;
  if (newSize.y == -1) newSize.y = height;
  SetSize( newSize.x, newSize.y );
  
  PostCreation();
  
  SetLabel( title );
  
  SetBackgroundColour( parent->GetBackgroundColour() );

  Show( TRUE );
    
  return TRUE;
}

wxRadioBox::~wxRadioBox(void)
{
  wxNode *node = m_boxes.First();
  while (node)
  {
    GtkWidget *button = GTK_WIDGET( node->Data() );
    gtk_widget_destroy( button );
    node = node->Next();
  }
}

void wxRadioBox::OnSize( wxSizeEvent &event )
{
  wxControl::OnSize( event );
  
  int x = m_x+5;
  int y = m_y+15;
  
  wxNode *node = m_boxes.First();
  while (node)
  {
    GtkWidget *button = GTK_WIDGET( node->Data() );
    
    gtk_myfixed_move( GTK_MYFIXED(m_parent->m_wxwindow), button, x, y );
    y += 20;
    
    int w = m_width-10;
    if (w < 15) w = 15;
    gtk_widget_set_usize( button, w, 20 );
      
    node = node->Next();
  }
}

bool wxRadioBox::Show( bool show )
{
  wxCHECK_MSG( m_widget != NULL, FALSE, "invalid radiobox" );
  
  wxWindow::Show( show );

  wxNode *node = m_boxes.First();
  while (node)
  {
    GtkWidget *button = GTK_WIDGET( node->Data() );
    
    if (show) gtk_widget_show( button ); else gtk_widget_hide( button );
    
    node = node->Next();
  }

  return TRUE;
}

int wxRadioBox::FindString( const wxString &s ) const
{
  wxCHECK_MSG( m_widget != NULL, -1, "invalid radiobox" );
  
  int count = 0;
  
  wxNode *node = m_boxes.First();
  while (node)
  {
    GtkButton *button = GTK_BUTTON( node->Data() );
    
    GtkLabel *label = GTK_LABEL( button->child );
    if (s == label->label) return count;
    count++;
    
    node = node->Next();
  }
  
  return -1;
}

void wxRadioBox::SetSelection( int n )
{
  wxCHECK_RET( m_widget != NULL, "invalid radiobox" );
  
  wxNode *node = m_boxes.Nth( n );
  
  if (!node)
  {
    wxFAIL_MSG( "wxRadioBox wrong index" );
    return;
  }
  
  GtkToggleButton *button = GTK_TOGGLE_BUTTON( node->Data() );
  
  gtk_toggle_button_set_state( button, 1 );
}

int wxRadioBox::GetSelection(void) const
{
  wxCHECK_MSG( m_widget != NULL, -1, "invalid radiobox" );
  
  int count = 0;
  
  wxNode *node = m_boxes.First();
  while (node)
  {
    GtkToggleButton *button = GTK_TOGGLE_BUTTON( node->Data() );
    if (button->active) return count;
    count++;
    node = node->Next();
  }
  
  wxFAIL_MSG( "wxRadioBox none selected" );
  
  return -1;
}

wxString wxRadioBox::GetString( int n ) const
{
  wxCHECK_MSG( m_widget != NULL, "", "invalid radiobox" );
  
  wxNode *node = m_boxes.Nth( n );
  
  if (!node)
  {
    wxFAIL_MSG( "wxRadioBox wrong index" );
    return "";
  }
  
  GtkButton *button = GTK_BUTTON( node->Data() );
  GtkLabel *label = GTK_LABEL( button->child );
  
  return wxString( label->label );
}

wxString wxRadioBox::GetLabel( int item ) const
{
  wxCHECK_MSG( m_widget != NULL, "", "invalid radiobox" );
  
  return GetString( item );
}

void wxRadioBox::SetLabel( const wxString& label )
{
  wxCHECK_RET( m_widget != NULL, "invalid radiobox" );
  
  wxControl::SetLabel( label );
  
  gtk_frame_set_label( GTK_FRAME(m_widget), wxControl::GetLabel() );
}

void wxRadioBox::SetLabel( int item, const wxString& label )
{
  wxCHECK_RET( m_widget != NULL, "invalid radiobox" );
  
  wxNode *node = m_boxes.Nth( item );
  
  if (!node)
  {
    wxFAIL_MSG( "wxRadioBox wrong index" );
    return;
  }
  
  GtkButton *button = GTK_BUTTON( node->Data() );
  GtkLabel *g_label = GTK_LABEL( button->child );
  
  gtk_label_set( g_label, label );
}

void wxRadioBox::SetLabel( int WXUNUSED(item), wxBitmap *WXUNUSED(bitmap) )
{
  wxFAIL_MSG("wxRadioBox::SetLabel not implemented.");
}

void wxRadioBox::Enable( bool enable )
{
  wxControl::Enable( enable );
  
  wxNode *node = m_boxes.First();
  while (node)
  {
    GtkButton *button = GTK_BUTTON( node->Data() );
    GtkWidget *label = button->child;
    gtk_widget_set_sensitive( GTK_WIDGET(button), enable );
    gtk_widget_set_sensitive( label, enable );
    node = node->Next();
  }
}

void wxRadioBox::Enable( int item, bool enable )
{
  wxNode *node = m_boxes.Nth( item );
  
  if (!node)
  {
    wxFAIL_MSG( "wxRadioBox wrong index" );
    return;
  }
  
  GtkButton *button = GTK_BUTTON( node->Data() );
  GtkWidget *label = button->child;
  gtk_widget_set_sensitive( GTK_WIDGET(button), enable );
  gtk_widget_set_sensitive( label, enable );
}

void wxRadioBox::Show( int item, bool show )
{
  wxNode *node = m_boxes.Nth( item );
  
  if (!node)
  {
    wxFAIL_MSG( "wxRadioBox wrong index" );
    return;
  }
  
  GtkWidget *button = GTK_WIDGET( node->Data() );

  if (show)
    gtk_widget_show( button );
  else
    gtk_widget_hide( button );
}

wxString wxRadioBox::GetStringSelection(void) const
{
  wxNode *node = m_boxes.First();
  while (node)
  {
    GtkToggleButton *button = GTK_TOGGLE_BUTTON( node->Data() );
    if (button->active)
    {
      GtkLabel *label = GTK_LABEL( GTK_BUTTON(button)->child );
      return label->label;
    }
    node = node->Next();
  }
  
  wxFAIL_MSG( "wxRadioBox none selected" );
  return "";
}

bool wxRadioBox::SetStringSelection( const wxString&s )
{
  int res = FindString( s );
  if (res == -1) return FALSE;
  SetSelection( res );
  return TRUE;
}

int wxRadioBox::Number(void) const
{
  return m_boxes.Number();
}

int wxRadioBox::GetNumberOfRowsOrCols(void) const
{
  return 1;
}

void wxRadioBox::SetNumberOfRowsOrCols( int WXUNUSED(n) )
{
  wxFAIL_MSG("wxRadioBox::SetNumberOfRowsOrCols not implemented.");
}

void wxRadioBox::SetFont( const wxFont &font )
{
  wxCHECK_RET( m_widget != NULL, "invalid radiobox" );
  
  wxControl::SetFont( font );
   
  wxNode *node = m_boxes.First();
  while (node)
  {
    GtkButton *button = GTK_BUTTON( node->Data() );
    
    gtk_widget_set_style( button->child, 
      gtk_style_ref(
        gtk_widget_get_style( m_widget ) ) ); 
    
    node = node->Next();
  }
}

void wxRadioBox::SetBackgroundColour( const wxColour &colour )
{
  return;

  wxCHECK_RET( m_widget != NULL, "invalid radiobox" );
  
  wxControl::SetBackgroundColour( colour );
  
  if (!m_backgroundColour.Ok()) return;
  
  wxNode *node = m_boxes.First();
  while (node)
  {
    GtkWidget *button = GTK_WIDGET( node->Data() );
    
    gtk_widget_set_style( button, 
      gtk_style_ref(
        gtk_widget_get_style( m_widget ) ) ); 
    
    node = node->Next();
  }
}

bool wxRadioBox::IsOwnGtkWindow( GdkWindow *window )
{
  if (window == m_widget->window) return TRUE;
  
  wxNode *node = m_boxes.First();
  while (node)
  {
    GtkWidget *button = GTK_WIDGET( node->Data() );
    
    if (window == button->window) return TRUE;
    
    node = node->Next();
  }
  
  return FALSE;
}
