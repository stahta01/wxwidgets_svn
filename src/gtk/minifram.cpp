/////////////////////////////////////////////////////////////////////////////
// Name:        minifram.cpp
// Purpose:
// Author:      Robert Roebling
// Id:          $Id$
// Copyright:   (c) 1998 Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "minifram.h"
#endif

#include "wx/minifram.h"
#include "wx/dcscreen.h"

#include "gtk/gtk.h"
#include "wx/gtk/win_gtk.h"

#include "gdk/gdk.h"
#include "gdk/gdkprivate.h"
#include "gdk/gdkx.h"

//-----------------------------------------------------------------------------
// idle system
//-----------------------------------------------------------------------------

extern void wxapp_install_idle_handler();
extern bool g_isIdle;

//-----------------------------------------------------------------------------
// data
//-----------------------------------------------------------------------------

extern bool   g_blockEventsOnDrag;
extern bool   g_blockEventsOnScroll;

//-----------------------------------------------------------------------------
// local functions
//-----------------------------------------------------------------------------

/* draw XOR rectangle when moving mine frame around */

static void DrawFrame( GtkWidget *widget, int x, int y, int w, int h )
{
    int org_x = 0;    
    int org_y = 0;
    gdk_window_get_origin( widget->window, &org_x, &org_y );
    x += org_x;
    y += org_y;

    GdkGC *gc = gdk_gc_new( GDK_ROOT_PARENT() );
    gdk_gc_set_subwindow( gc, GDK_INCLUDE_INFERIORS );
    gdk_gc_set_function( gc, GDK_INVERT );
    
    gdk_draw_rectangle( GDK_ROOT_PARENT(), gc, FALSE, x, y, w, h );
    gdk_gc_unref( gc );
}

//-----------------------------------------------------------------------------
// "expose_event" of m_mainWidget
//-----------------------------------------------------------------------------

static void gtk_window_own_expose_callback( GtkWidget *widget, GdkEventExpose *gdk_event, wxFrame *win )
{
    if (g_isIdle) wxapp_install_idle_handler();

    if (!win->HasVMT()) return;
    if (gdk_event->count > 0) return;
    
    gtk_draw_shadow( widget->style, 
                     widget->window,
		     GTK_STATE_NORMAL,
		     GTK_SHADOW_OUT,
		     0, 0,
		     win->m_width, win->m_height );
}

//-----------------------------------------------------------------------------
// "draw" of m_mainWidget
//-----------------------------------------------------------------------------

static void gtk_window_own_draw_callback( GtkWidget *widget, GdkRectangle *WXUNUSED(rect), wxFrame *win )
{
    if (g_isIdle) wxapp_install_idle_handler();

    if (!win->HasVMT()) return;
    
    gtk_draw_shadow( widget->style, 
                     widget->window,
		     GTK_STATE_NORMAL,
		     GTK_SHADOW_OUT,
		     0, 0,
		     win->m_width, win->m_height );
}

//-----------------------------------------------------------------------------
// "button_press_event" of m_mainWidget
//-----------------------------------------------------------------------------

static gint gtk_window_button_press_callback( GtkWidget *widget, GdkEventButton *gdk_event, wxMiniFrame *win )
{
    if (g_isIdle) wxapp_install_idle_handler();

    if (!win->HasVMT()) return FALSE;
    if (g_blockEventsOnDrag) return TRUE;
    if (g_blockEventsOnScroll) return TRUE;

    if (win->m_isDragging) return TRUE;

    gdk_pointer_grab( widget->window, FALSE,
                      (GdkEventMask)
                         (GDK_BUTTON_PRESS_MASK |
                          GDK_BUTTON_RELEASE_MASK |
                          GDK_POINTER_MOTION_MASK	|
                          GDK_POINTER_MOTION_HINT_MASK  |
                          GDK_BUTTON_MOTION_MASK	|
                          GDK_BUTTON1_MOTION_MASK),
                      (GdkWindow *) NULL,
                      (GdkCursor *) NULL,
                      GDK_CURRENT_TIME );
		     
    win->m_diffX = (int)gdk_event->x;
    win->m_diffY = (int)gdk_event->y;
    DrawFrame( widget, 0, 0, win->m_width, win->m_height );
    win->m_oldX = 0;
    win->m_oldY = 0;
    
    win->m_isDragging = TRUE;

    return TRUE;
}

//-----------------------------------------------------------------------------
// "button_release_event" of m_mainWidget
//-----------------------------------------------------------------------------

static gint gtk_window_button_release_callback( GtkWidget *widget, GdkEventButton *gdk_event, wxMiniFrame *win )
{
    if (g_isIdle) wxapp_install_idle_handler();

    if (!win->HasVMT()) return FALSE;
    if (g_blockEventsOnDrag) return TRUE;
    if (g_blockEventsOnScroll) return TRUE;

    if (!win->m_isDragging) return TRUE;
    
    win->m_isDragging = FALSE;
     
    int x = (int)gdk_event->x;
    int y = (int)gdk_event->y;
    
    DrawFrame( widget, win->m_oldX, win->m_oldY, win->m_width, win->m_height );
    gdk_pointer_ungrab ( GDK_CURRENT_TIME );
    int org_x = 0;    
    int org_y = 0;
    gdk_window_get_origin( widget->window, &org_x, &org_y );
    x += org_x - win->m_diffX;
    y += org_y - win->m_diffY;
    win->m_x = x;
    win->m_y = y;
    gtk_widget_set_uposition( win->m_widget, x, y );

    return TRUE;
}

//-----------------------------------------------------------------------------
// "motion_notify_event" of m_mainWidget
//-----------------------------------------------------------------------------

static gint gtk_window_motion_notify_callback( GtkWidget *widget, GdkEventMotion *gdk_event, wxMiniFrame *win )
{
    if (g_isIdle) wxapp_install_idle_handler();

    if (!win->HasVMT()) return FALSE;
    if (g_blockEventsOnDrag) return TRUE;
    if (g_blockEventsOnScroll) return TRUE;

    if (!win->m_isDragging) return TRUE;
    
    if (gdk_event->is_hint)
    {
       int x = 0;
       int y = 0;
       GdkModifierType state;
       gdk_window_get_pointer(gdk_event->window, &x, &y, &state);
       gdk_event->x = x;
       gdk_event->y = y;
       gdk_event->state = state;
    }

    DrawFrame( widget, win->m_oldX, win->m_oldY, win->m_width, win->m_height );
    win->m_oldX = (int)gdk_event->x - win->m_diffX;
    win->m_oldY = (int)gdk_event->y - win->m_diffY;
    DrawFrame( widget, win->m_oldX, win->m_oldY, win->m_width, win->m_height );
    
    return TRUE;
}

//-----------------------------------------------------------------------------
// "clicked" of X system button
//-----------------------------------------------------------------------------

static void gtk_button_clicked_callback( GtkWidget *WXUNUSED(widget), wxMiniFrame *mf )
{
    if (g_isIdle) wxapp_install_idle_handler();

    mf->Close();
}

//-----------------------------------------------------------------------------
// wxMiniFrame
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxMiniFrame,wxFrame)

bool wxMiniFrame::Create( wxWindow *parent, wxWindowID id, const wxString &title,
      const wxPoint &pos, const wxSize &size,
      long style, const wxString &name )
{
    style = style | wxSIMPLE_BORDER;
    
    m_miniEdge = 3;
    m_miniTitle = 13;
    m_isDragging = FALSE;
    m_oldX = -1;
    m_oldY = -1;
    m_diffX = 0;
    m_diffY = 0;
    
    wxFrame::Create( parent, id, title, pos, size, style, name );

    GtkWidget *close_button = gtk_button_new_with_label( "x" );
    
    gtk_myfixed_put( GTK_MYFIXED(m_mainWidget), close_button, 4, 4 );
    gtk_widget_set_usize( close_button, 12, 11 );
    
    gtk_widget_show( close_button );
    
    gtk_signal_connect( GTK_OBJECT(close_button), "clicked",
      GTK_SIGNAL_FUNC(gtk_button_clicked_callback), (gpointer*)this );
    
    /* these are called when the borders are drawn */
    gtk_signal_connect( GTK_OBJECT(m_mainWidget), "expose_event",
        GTK_SIGNAL_FUNC(gtk_window_own_expose_callback), (gpointer)this );

    gtk_signal_connect( GTK_OBJECT(m_mainWidget), "draw",
       GTK_SIGNAL_FUNC(gtk_window_own_draw_callback), (gpointer)this );
       
    /* these are required for dragging the mini frame around */
    gtk_signal_connect( GTK_OBJECT(m_mainWidget), "button_press_event",
      GTK_SIGNAL_FUNC(gtk_window_button_press_callback), (gpointer)this );

    gtk_signal_connect( GTK_OBJECT(m_mainWidget), "button_release_event",
      GTK_SIGNAL_FUNC(gtk_window_button_release_callback), (gpointer)this );

    gtk_signal_connect( GTK_OBJECT(m_mainWidget), "motion_notify_event",
      GTK_SIGNAL_FUNC(gtk_window_motion_notify_callback), (gpointer)this );

    return TRUE;
}
