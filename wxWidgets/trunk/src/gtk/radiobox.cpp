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

#include "gdk/gdk.h"
#include "gtk/gtk.h"
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
                         int n, const wxString choices[], int majorDim, 
                         long style, const wxValidator& validator, 
			 const wxString &name )
{
    m_alreadySent = FALSE;
    m_needParent = TRUE;
    m_acceptsFocus = TRUE;
  
    PreCreation( parent, id, pos, size, style, name );

    SetValidator( validator );

    m_widget = gtk_frame_new( title );
  
    m_majorDim = majorDim;
  
    GtkRadioButton *m_radio = (GtkRadioButton*) NULL;
  
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
       
        gtk_myfixed_put( GTK_MYFIXED(m_parent->m_wxwindow), GTK_WIDGET(m_radio), m_x+10, m_y+10+(i*24) );
      
    }
    
    wxSize ls = LayoutItems();
  
    wxSize newSize = size;
    if (newSize.x == -1) newSize.x = ls.x;
    if (newSize.y == -1) newSize.y = ls.y;
    SetSize( newSize.x, newSize.y );
  
    m_parent->AddChild( this );

    (m_parent->m_insertCallback)( m_parent, this );
  
    PostCreation();
  
    SetLabel( title );
  
    SetBackgroundColour( parent->GetBackgroundColour() );
    SetForegroundColour( parent->GetForegroundColour() );

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
  
    LayoutItems();
}

wxSize wxRadioBox::LayoutItems()
{
    int x = 7;
    int y = 15;
    
    int num_per_major = (m_boxes.GetCount() - 1) / m_majorDim +1;
    
    wxSize res( 0, 0 );
  
    if (m_windowStyle & wxRA_HORIZONTAL)
    {
	
        for (int j = 0; j < m_majorDim; j++)
	{
            y = 15;
	    
            int max_len = 0;
            wxNode *node = m_boxes.Nth( j*num_per_major );
	    for (int i = 0; i< num_per_major; i++)
	    {
                GtkWidget *button = GTK_WIDGET( node->Data() );
                GtkLabel *label = GTK_LABEL( GTK_BUTTON(button)->child );
                GdkFont *font = m_widget->style->font;
                int len = 22+gdk_string_measure( font, label->label );
                if (len > max_len) max_len = len;
		
                gtk_myfixed_move( GTK_MYFIXED(m_parent->m_wxwindow), button, m_x+x, m_y+y );
                y += 22;
		
	        node = node->Next();
		if (!node) break;
	    }
	    
	    // we don't know the max_len before
	    
            node = m_boxes.Nth( j*num_per_major );
	    for (int i = 0; i< num_per_major; i++)
	    {
                GtkWidget *button = GTK_WIDGET( node->Data() );
		
		gtk_widget_set_usize( button, max_len, 22 );
		
	        node = node->Next();
		if (!node) break;
	    }
	    
	    if (y > res.y) res.y = y;
	    
	    x += max_len + 2;
        }
	
	res.x = x+4;
	res.y += 9;
    }
    else
    {
        int max = 0;

        wxNode *node = m_boxes.First();
        while (node)
        {
            GtkButton *button = GTK_BUTTON( node->Data() );
            GtkLabel *label = GTK_LABEL( button->child );
      
            GdkFont *font = m_widget->style->font;
            int len = 22+gdk_string_measure( font, label->label );
            if (len > max) max = len;
      
            node = node->Next();
        }
    
        node = m_boxes.First();
        while (node)
        {
            GtkWidget *button = GTK_WIDGET( node->Data() );
    
            gtk_myfixed_move( GTK_MYFIXED(m_parent->m_wxwindow), button, m_x+x, m_y+y );
            x += max;
            gtk_widget_set_usize( button, max, 20 );
      
            node = node->Next();
        }
	res.x = x+4;
	res.y = 42;
    }
    
    return res;
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

void wxRadioBox::SetFocus()
{
    wxCHECK_RET( m_widget != NULL, "invalid radiobox" );
    
    if (m_boxes.GetCount() == 0) return;
    
    wxNode *node = m_boxes.First();
    while (node)
    {
        GtkToggleButton *button = GTK_TOGGLE_BUTTON( node->Data() );
        if (button->active)
	{
            gtk_widget_grab_focus( GTK_WIDGET(button) );
	    
	    return;
	}
        node = node->Next();
    }
    
}

void wxRadioBox::SetSelection( int n )
{
    wxCHECK_RET( m_widget != NULL, "invalid radiobox" );
  
    wxNode *node = m_boxes.Nth( n );
  
    wxCHECK_RET( node, "radiobox wrong index" );
  
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
  
    wxCHECK_MSG( node, "", "radiobox wrong index" );
  
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
  
    wxCHECK_RET( node, "radiobox wrong index" );
  
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
    wxCHECK_RET( m_widget != NULL, "invalid radiobox" );
  
    wxNode *node = m_boxes.Nth( item );
  
    wxCHECK_RET( node, "radiobox wrong index" );
  
    GtkButton *button = GTK_BUTTON( node->Data() );
    GtkWidget *label = button->child;
    gtk_widget_set_sensitive( GTK_WIDGET(button), enable );
    gtk_widget_set_sensitive( label, enable );
}

void wxRadioBox::Show( int item, bool show )
{
    wxCHECK_RET( m_widget != NULL, "invalid radiobox" );
  
    wxNode *node = m_boxes.Nth( item );
  
    wxCHECK_RET( node, "radiobox wrong index" );
  
    GtkWidget *button = GTK_WIDGET( node->Data() );

    if (show)
        gtk_widget_show( button );
    else
        gtk_widget_hide( button );
}

wxString wxRadioBox::GetStringSelection(void) const
{
    wxCHECK_MSG( m_widget != NULL, "", "invalid radiobox" );
  
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

bool wxRadioBox::SetStringSelection( const wxString &s )
{
    wxCHECK_MSG( m_widget != NULL, FALSE, "invalid radiobox" ); 
  
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

void wxRadioBox::ApplyWidgetStyle()
{
    SetWidgetStyle();
  
    gtk_widget_set_style( m_widget, m_widgetStyle );
  
    wxNode *node = m_boxes.First();
    while (node)
    {
        GtkWidget *widget = GTK_WIDGET( node->Data() );
        gtk_widget_set_style( widget, m_widgetStyle );
    
        GtkButton *button = GTK_BUTTON( node->Data() );
        gtk_widget_set_style( button->child, m_widgetStyle );
    
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
