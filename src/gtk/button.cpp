/////////////////////////////////////////////////////////////////////////////
// Name:        button.cpp
// Purpose:
// Author:      Robert Roebling
// Id:          $Id$
// Copyright:   (c) 1998 Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "button.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/defs.h"

#if wxUSE_BUTTON

#include "wx/button.h"

#include "wx/gtk/private.h"

//-----------------------------------------------------------------------------
// classes
//-----------------------------------------------------------------------------

class wxButton;

//-----------------------------------------------------------------------------
// idle system
//-----------------------------------------------------------------------------

extern void wxapp_install_idle_handler();
extern bool g_isIdle;

//-----------------------------------------------------------------------------
// data
//-----------------------------------------------------------------------------

extern bool   g_blockEventsOnDrag;

//-----------------------------------------------------------------------------
// "clicked"
//-----------------------------------------------------------------------------

static void gtk_button_clicked_callback( GtkWidget *WXUNUSED(widget), wxButton *button )
{
    if (g_isIdle)
       wxapp_install_idle_handler();

    if (!button->m_hasVMT) return;
    if (g_blockEventsOnDrag) return;

    wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, button->GetId());
    event.SetEventObject(button);
    button->GetEventHandler()->ProcessEvent(event);
}

//-----------------------------------------------------------------------------
// wxButton
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxButton,wxControl)

wxButton::wxButton()
{
}

wxButton::~wxButton()
{
}

bool wxButton::Create(  wxWindow *parent, wxWindowID id, const wxString &label,
      const wxPoint &pos, const wxSize &size,
      long style, const wxValidator& validator, const wxString &name )
{
    m_needParent = TRUE;
    m_acceptsFocus = TRUE;

    if (!PreCreation( parent, pos, size ) ||
        !CreateBase( parent, id, pos, size, style, validator, name ))
    {
        wxFAIL_MSG( wxT("wxButton creation failed") );
        return FALSE;
    }

/*
    wxString label2( label );
    for (size_t i = 0; i < label2.Len(); i++)
    {
        if (label2.GetChar(i) == wxT('&'))
        label2.SetChar(i,wxT('_'));
    }

    GtkWidget *accel_label = gtk_accel_label_new( label2.mb_str() );
    gtk_widget_show( accel_label );

    m_widget = gtk_button_new();
    gtk_container_add( GTK_CONTAINER(m_widget), accel_label );

    gtk_accel_label_set_accel_widget( GTK_ACCEL_LABEL(accel_label), m_widget );

    guint accel_key = gtk_label_parse_uline (GTK_LABEL(accel_label), label2.mb_str() );
    gtk_accel_label_refetch( GTK_ACCEL_LABEL(accel_label) );

    wxControl::SetLabel( label );
*/

    m_widget = gtk_button_new_with_label("");

    float x_alignment = 0.5;
    if (HasFlag(wxBU_LEFT))
        x_alignment = 0.0;
    else if (HasFlag(wxBU_RIGHT))
        x_alignment = 1.0;

    float y_alignment = 0.5;
    if (HasFlag(wxBU_TOP))
        y_alignment = 0.0;
    else if (HasFlag(wxBU_BOTTOM))
        y_alignment = 1.0;

    gtk_misc_set_alignment (GTK_MISC (BUTTON_CHILD (m_widget)),
                            x_alignment, y_alignment);

    SetLabel( label );

    if (style & wxNO_BORDER)
       gtk_button_set_relief( GTK_BUTTON(m_widget), GTK_RELIEF_NONE );

    gtk_signal_connect( GTK_OBJECT(m_widget), "clicked",
      GTK_SIGNAL_FUNC(gtk_button_clicked_callback), (gpointer*)this );

    m_parent->DoAddChild( this );

    PostCreation(size);

    return TRUE;
}

void wxButton::SetDefault()
{
    wxWindow *parent = GetParent();
    wxCHECK_RET( parent, _T("button without parent?") );

    parent->SetDefaultItem(this);
    
    GTK_WIDGET_SET_FLAGS( m_widget, GTK_CAN_DEFAULT );
    gtk_widget_grab_default( m_widget );

    SetSize( m_x, m_y, m_width, m_height );
}

/* static */
wxSize wxButton::GetDefaultSize()
{
    return wxSize(80,26);
}

void wxButton::SetLabel( const wxString &label )
{
    wxCHECK_RET( m_widget != NULL, wxT("invalid button") );

    wxControl::SetLabel( label );

#ifdef __WXGTK20__
    wxString label2 = PrepareLabelMnemonics( label );
    gtk_label_set_text_with_mnemonic( GTK_LABEL( BUTTON_CHILD(m_widget) ), wxGTK_CONV( label2 ) );
#else
    gtk_label_set( GTK_LABEL( BUTTON_CHILD(m_widget) ), wxGTK_CONV( GetLabel() ) );
#endif
}

bool wxButton::Enable( bool enable )
{
    if ( !wxControl::Enable( enable ) )
        return FALSE;

    gtk_widget_set_sensitive( BUTTON_CHILD(m_widget), enable );

    return TRUE;
}

bool wxButton::IsOwnGtkWindow( GdkWindow *window )
{
#ifdef __WXGTK20__
    return GTK_BUTTON(m_widget)->event_window;
#else
    return (window == m_widget->window);
#endif
}

void wxButton::ApplyWidgetStyle()
{
    SetWidgetStyle();
    gtk_widget_set_style( m_widget, m_widgetStyle );
    gtk_widget_set_style( BUTTON_CHILD(m_widget), m_widgetStyle );
}

wxSize wxButton::DoGetBestSize() const
{
    // the default button in wxGTK is bigger than the other ones because of an
    // extra border around it, but we don't want to take it into account in
    // our size calculations (otherwsie the result is visually ugly), so
    // always return the size of non default button from here
    const bool isDefault = GTK_WIDGET_HAS_DEFAULT(m_widget);
    if ( isDefault )
    {
        // temporarily unset default flag
        GTK_WIDGET_UNSET_FLAGS( m_widget, GTK_CAN_DEFAULT );
    }

    wxSize ret( wxControl::DoGetBestSize() );

    if ( isDefault )
    {
        // set it back again
        GTK_WIDGET_SET_FLAGS( m_widget, GTK_CAN_DEFAULT );
    }

#ifndef __WXGTK20__
    ret.x += 10;  // add a few pixels for sloppy (but common) themes
#endif
    
    if (!HasFlag(wxBU_EXACTFIT))
    {
        if (ret.x < 80) ret.x = 80;
    }

    return ret;
}

#endif // wxUSE_BUTTON

