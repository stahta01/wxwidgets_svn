/////////////////////////////////////////////////////////////////////////////
// Name:        wx/gtk/tglbtn.cpp
// Purpose:     Definition of the wxToggleButton class, which implements a
//              toggle button under wxGTK.
// Author:      John Norris, minor changes by Axel Schlueter
// Modified by:
// Created:     08.02.01
// RCS-ID:      $Id$
// Copyright:   (c) 2000 Johnny C. Norris II
// License:     Rocketeer license
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/tglbtn.h"
#include "wx/button.h"

#if wxUSE_TOGGLEBTN

#include "wx/gtk/private.h"

extern void wxapp_install_idle_handler();
extern bool g_isIdle;
extern bool      g_blockEventsOnDrag;
extern wxCursor   g_globalCursor;
extern wxWindowGTK *g_delayedFocus;

static void gtk_togglebutton_clicked_callback(GtkWidget *WXUNUSED(widget), wxToggleButton *cb)
{
   if (g_isIdle)
      wxapp_install_idle_handler();

   if (!cb->m_hasVMT || g_blockEventsOnDrag)
      return;
      
   if (cb->m_blockEvent) return;

   // Generate a wx event.
   wxCommandEvent event(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, cb->GetId());
   event.SetInt(cb->GetValue());
   event.SetEventObject(cb);
   cb->GetEventHandler()->ProcessEvent(event);
}

DEFINE_EVENT_TYPE(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED)

// ------------------------------------------------------------------------
// wxToggleBitmapButton
// ------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxToggleBitmapButton, wxControl)

bool wxToggleBitmapButton::Create(wxWindow *parent, wxWindowID id,
                            const wxBitmap &label, const wxPoint &pos,
                            const wxSize &size, long style,
                            const wxValidator& validator,
                            const wxString &name)
{
    m_needParent = TRUE;
    m_acceptsFocus = TRUE;
   
    m_blockEvent = FALSE;

    if (!PreCreation(parent, pos, size) ||
       !CreateBase(parent, id, pos, size, style, validator, name ))
    {
        wxFAIL_MSG(wxT("wxToggleBitmapButton creation failed"));
        return FALSE;
    }
    
    m_bitmap = label;

    // Create the gtk widget.
    m_widget = gtk_toggle_button_new();

    if (style & wxNO_BORDER)
       gtk_button_set_relief( GTK_BUTTON(m_widget), GTK_RELIEF_NONE );

    if (m_bitmap.Ok())
    {
        wxSize newSize = size;
        int border = (style & wxNO_BORDER) ? 4 : 10;
        if (newSize.x == -1)
            newSize.x = m_bitmap.GetWidth()+border;
        if (newSize.y == -1)
            newSize.y = m_bitmap.GetHeight()+border;
        SetSize( newSize.x, newSize.y );
        OnSetBitmap();
    }

    gtk_signal_connect(GTK_OBJECT(m_widget), "clicked",
                      GTK_SIGNAL_FUNC(gtk_togglebutton_clicked_callback),
                      (gpointer *)this);

    m_parent->DoAddChild(this);

    PostCreation();
    InheritAttributes();

    Show( TRUE );

    return TRUE;
}

// void SetValue(bool state)
// Set the value of the toggle button.
void wxToggleBitmapButton::SetValue(bool state)
{
   wxCHECK_RET(m_widget != NULL, wxT("invalid toggle button"));

   if (state == GetValue())
      return;

   m_blockEvent = TRUE;

   gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(m_widget), state);

   m_blockEvent = FALSE;
}

// bool GetValue() const
// Get the value of the toggle button.
bool wxToggleBitmapButton::GetValue() const
{
   wxCHECK_MSG(m_widget != NULL, FALSE, wxT("invalid toggle button"));

   return GTK_TOGGLE_BUTTON(m_widget)->active;
}

void wxToggleBitmapButton::SetLabel(const wxBitmap& label)
{
    wxCHECK_RET(m_widget != NULL, wxT("invalid toggle button"));

    m_bitmap = label;
    
    OnSetBitmap();
}

void wxToggleBitmapButton::OnSetBitmap()
{
    if (!m_bitmap.Ok()) return;

    GdkBitmap *mask = (GdkBitmap *) NULL;
    if (m_bitmap.GetMask()) mask = m_bitmap.GetMask()->GetBitmap();

    GtkWidget *child = BUTTON_CHILD(m_widget);
    if (child == NULL)
    {
        // initial bitmap
        GtkWidget *pixmap = gtk_pixmap_new(m_bitmap.GetPixmap(), mask);
        gtk_widget_show(pixmap);
        gtk_container_add(GTK_CONTAINER(m_widget), pixmap);
    }
    else
    {   // subsequent bitmaps
        GtkPixmap *g_pixmap = GTK_PIXMAP(child);
        gtk_pixmap_set(g_pixmap, m_bitmap.GetPixmap(), mask);
    }
}

bool wxToggleBitmapButton::Enable(bool enable /*=TRUE*/)
{
    if (!wxControl::Enable(enable))
        return FALSE;

    gtk_widget_set_sensitive(BUTTON_CHILD(m_widget), enable);

    return TRUE;
}

void wxToggleBitmapButton::ApplyWidgetStyle()
{
    SetWidgetStyle();
    gtk_widget_set_style(m_widget, m_widgetStyle);
    gtk_widget_set_style(BUTTON_CHILD(m_widget), m_widgetStyle);
}

bool wxToggleBitmapButton::IsOwnGtkWindow(GdkWindow *window)
{
    return window == TOGGLE_BUTTON_EVENT_WIN(m_widget);
}

void wxToggleBitmapButton::OnInternalIdle()
{
    wxCursor cursor = m_cursor;
    
    if (g_globalCursor.Ok())
        cursor = g_globalCursor;

    GdkWindow *win = TOGGLE_BUTTON_EVENT_WIN(m_widget);
    if ( win && cursor.Ok() )
    {
      /* I now set the cursor the anew in every OnInternalIdle call
         as setting the cursor in a parent window also effects the
         windows above so that checking for the current cursor is
         not possible. */

        gdk_window_set_cursor(win, cursor.GetCursor());
    }

    if (wxUpdateUIEvent::CanUpdate(this))
        UpdateWindowUI(wxUPDATE_UI_FROMIDLE);
}

// wxSize DoGetBestSize() const
// Get the "best" size for this control.
wxSize wxToggleBitmapButton::DoGetBestSize() const
{
    wxSize ret(wxControl::DoGetBestSize());
   
    if (!HasFlag(wxBU_EXACTFIT))
    {
        if (ret.x < 80) ret.x = 80;
    }
    

   return ret;
}
// ------------------------------------------------------------------------
// wxToggleButton
// ------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxToggleButton, wxControl)

bool wxToggleButton::Create(wxWindow *parent, wxWindowID id,
                            const wxString &label, const wxPoint &pos,
                            const wxSize &size, long style,
                            const wxValidator& validator,
                            const wxString &name)
{
   m_needParent = TRUE;
   m_acceptsFocus = TRUE;
   
   m_blockEvent = FALSE;

   if (!PreCreation(parent, pos, size) ||
       !CreateBase(parent, id, pos, size, style, validator, name )) {
      wxFAIL_MSG(wxT("wxToggleButton creation failed"));
      return FALSE;
   }

   wxControl::SetLabel(label);

   // Create the gtk widget.
   m_widget = gtk_toggle_button_new_with_label( wxGTK_CONV( m_label ) );

   gtk_signal_connect(GTK_OBJECT(m_widget), "clicked",
                      GTK_SIGNAL_FUNC(gtk_togglebutton_clicked_callback),
                      (gpointer *)this);

   m_parent->DoAddChild(this);

   PostCreation();
   InheritAttributes();

   wxSize size_best(DoGetBestSize());
   wxSize new_size(size);
   if (new_size.x == -1)
      new_size.x = size_best.x;
   if (new_size.y == -1)
      new_size.y = size_best.y;
   if ((new_size.x != size.x) || (new_size.y != size.y))
      SetSize(new_size.x, new_size.y);

   Show(TRUE);

   return TRUE;
}

// void SetValue(bool state)
// Set the value of the toggle button.
void wxToggleButton::SetValue(bool state)
{
   wxCHECK_RET(m_widget != NULL, wxT("invalid toggle button"));

   if (state == GetValue())
      return;

   m_blockEvent = TRUE;

   gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(m_widget), state);

   m_blockEvent = FALSE;
}

// bool GetValue() const
// Get the value of the toggle button.
bool wxToggleButton::GetValue() const
{
   wxCHECK_MSG(m_widget != NULL, FALSE, wxT("invalid toggle button"));

   return GTK_TOGGLE_BUTTON(m_widget)->active;
}

void wxToggleButton::SetLabel(const wxString& label)
{
    wxCHECK_RET(m_widget != NULL, wxT("invalid toggle button"));

    wxControl::SetLabel(label);

    gtk_label_set(GTK_LABEL(BUTTON_CHILD(m_widget)), wxGTK_CONV( GetLabel() ) );
}

bool wxToggleButton::Enable(bool enable /*=TRUE*/)
{
    if (!wxControl::Enable(enable))
        return FALSE;

    gtk_widget_set_sensitive(BUTTON_CHILD(m_widget), enable);

    return TRUE;
}

void wxToggleButton::ApplyWidgetStyle()
{
    SetWidgetStyle();
    gtk_widget_set_style(m_widget, m_widgetStyle);
    gtk_widget_set_style(BUTTON_CHILD(m_widget), m_widgetStyle);
}

bool wxToggleButton::IsOwnGtkWindow(GdkWindow *window)
{
    return window == TOGGLE_BUTTON_EVENT_WIN(m_widget);
}

void wxToggleButton::OnInternalIdle()
{
    wxCursor cursor = m_cursor;
    
    if (g_globalCursor.Ok())
        cursor = g_globalCursor;

    GdkWindow *win = TOGGLE_BUTTON_EVENT_WIN(m_widget);
    if ( win && cursor.Ok() )
    {
      /* I now set the cursor the anew in every OnInternalIdle call
         as setting the cursor in a parent window also effects the
         windows above so that checking for the current cursor is
         not possible. */

        gdk_window_set_cursor(win, cursor.GetCursor());
    }

    if (wxUpdateUIEvent::CanUpdate(this))
        UpdateWindowUI(wxUPDATE_UI_FROMIDLE);
}

// wxSize DoGetBestSize() const
// Get the "best" size for this control.
wxSize wxToggleButton::DoGetBestSize() const
{
    wxSize ret(wxControl::DoGetBestSize());
   
    if (!HasFlag(wxBU_EXACTFIT))
    {
        if (ret.x < 80) ret.x = 80;
    }
    

   return ret;
}

#endif // wxUSE_TOGGLEBTN

