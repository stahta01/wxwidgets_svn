/////////////////////////////////////////////////////////////////////////////
// Name:        frame.cpp
// Purpose:
// Author:      Robert Roebling
// Id:          $Id$
// Copyright:   (c) 1998 Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "frame.h"
#endif

#include "wx/frame.h"
#include "wx/dialog.h"
#include "wx/control.h"
#include "wx/app.h"
#include "wx/menu.h"
#include "wx/toolbar.h"
#include "wx/statusbr.h"
#include "wx/dcclient.h"
#include "wx/gtk/win_gtk.h"

//-----------------------------------------------------------------------------
// constants
//-----------------------------------------------------------------------------

const int wxMENU_HEIGHT    = 30;
const int wxSTATUS_HEIGHT  = 25;

//-----------------------------------------------------------------------------
// data
//-----------------------------------------------------------------------------

extern wxList wxTopLevelWindows;
extern wxList wxPendingDelete;

//-----------------------------------------------------------------------------
// "size_allocate"
//-----------------------------------------------------------------------------

static void gtk_frame_size_callback( GtkWidget *WXUNUSED(widget), GtkAllocation* alloc, wxFrame *win )
{
    if (!win->HasVMT()) return;
    
/*
    printf( "OnFrameResize from " );
    if (win->GetClassInfo() && win->GetClassInfo()->GetClassName())
        printf( win->GetClassInfo()->GetClassName() );
    printf( ".\n" );
*/
    
   if ((win->m_width != alloc->width) || (win->m_height != alloc->height))
   {
       win->m_sizeSet = FALSE;
       win->m_width = alloc->width;
       win->m_height = alloc->height;
   }
}

//-----------------------------------------------------------------------------
// "delete_event"
//-----------------------------------------------------------------------------

static gint gtk_frame_delete_callback( GtkWidget *WXUNUSED(widget), GdkEvent *WXUNUSED(event), wxFrame *win )
{
/*
    printf( "OnDelete from " );
    if (win->GetClassInfo() && win->GetClassInfo()->GetClassName())
        printf( win->GetClassInfo()->GetClassName() );
    printf( ".\n" );
*/

    win->Close();

    return TRUE;
}

//-----------------------------------------------------------------------------
// "configure_event"
//-----------------------------------------------------------------------------

static gint gtk_frame_configure_callback( GtkWidget *WXUNUSED(widget), GdkEventConfigure *event, wxFrame *win )
{
    if (!win->HasVMT()) return FALSE;
  
    win->m_x = event->x;
    win->m_y = event->y;
  
    return FALSE;
}

//-----------------------------------------------------------------------------
// wxFrame
//-----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(wxFrame, wxWindow)
    EVT_SIZE(wxFrame::OnSize)
    EVT_CLOSE(wxFrame::OnCloseWindow)
    EVT_IDLE(wxFrame::OnIdle)
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC_CLASS(wxFrame,wxWindow)

wxFrame::wxFrame()
{
    m_frameMenuBar = (wxMenuBar *) NULL;
    m_frameStatusBar = (wxStatusBar *) NULL;
    m_frameToolBar = (wxToolBar *) NULL;
    m_sizeSet = FALSE;
    wxTopLevelWindows.Insert( this );
}

wxFrame::wxFrame( wxWindow *parent, wxWindowID id, const wxString &title,
      const wxPoint &pos, const wxSize &size,
      long style, const wxString &name )
{
    m_frameMenuBar = (wxMenuBar *) NULL;
    m_frameStatusBar = (wxStatusBar *) NULL;
    m_frameToolBar = (wxToolBar *) NULL;
    m_sizeSet = FALSE;
    Create( parent, id, title, pos, size, style, name );
    wxTopLevelWindows.Insert( this );
}

bool wxFrame::Create( wxWindow *parent, wxWindowID id, const wxString &title,
      const wxPoint &pos, const wxSize &size,
      long style, const wxString &name )
{
    m_needParent = FALSE;

    PreCreation( parent, id, pos, size, style, name );

    m_title = title;

    GtkWindowType win_type = GTK_WINDOW_TOPLEVEL;
    if (style & wxSIMPLE_BORDER) win_type = GTK_WINDOW_POPUP;
  
    m_widget = gtk_window_new( win_type );
    
    if ((size.x != -1) && (size.y != -1))
        gtk_widget_set_usize( m_widget, m_width, m_height );
    if ((pos.x != -1) && (pos.y != -1))
        gtk_widget_set_uposition( m_widget, m_x, m_y );

    gtk_window_set_title( GTK_WINDOW(m_widget), title );
    GTK_WIDGET_UNSET_FLAGS( m_widget, GTK_CAN_FOCUS );

    gtk_widget_set( m_widget, "GtkWindow::allow_shrink", TRUE, NULL );

    gtk_signal_connect( GTK_OBJECT(m_widget), "delete_event",
        GTK_SIGNAL_FUNC(gtk_frame_delete_callback), (gpointer)this );

    m_wxwindow = gtk_myfixed_new();
    gtk_widget_show( m_wxwindow );
    GTK_WIDGET_UNSET_FLAGS( m_wxwindow, GTK_CAN_FOCUS );

    gtk_container_add( GTK_CONTAINER(m_widget), m_wxwindow );

    gtk_signal_connect( GTK_OBJECT(m_widget), "size_allocate",
        GTK_SIGNAL_FUNC(gtk_frame_size_callback), (gpointer)this );

    gtk_signal_connect( GTK_OBJECT(m_widget), "configure_event",
        GTK_SIGNAL_FUNC(gtk_frame_configure_callback), (gpointer)this );
    
    if (m_parent) m_parent->AddChild( this );
  
    PostCreation();
    
    return TRUE;
}

wxFrame::~wxFrame()
{
    if (m_frameMenuBar) delete m_frameMenuBar;
    if (m_frameStatusBar) delete m_frameStatusBar;
    if (m_frameToolBar) delete m_frameToolBar;

    wxTopLevelWindows.DeleteObject( this );
    if (wxTopLevelWindows.Number() == 0) wxTheApp->ExitMainLoop();
}

bool wxFrame::Show( bool show )
{
    wxASSERT_MSG( (m_widget != NULL), "invalid frame" );
  
    if (show && !m_sizeSet)
    {
	// by calling GtkOnSize here, we don't have to call
	// either after showing the frame, which would entail
	// much ugly flicker nor from within the size_allocate
	// handler, because GTK 1.1.X forbids that.
	
        GtkOnSize( m_x, m_y, m_width, m_height );
    }
	
    return wxWindow::Show( show );
}

void wxFrame::OnCloseWindow( wxCloseEvent &event )
{
    if (GetEventHandler()->OnClose() || event.GetForce()) this->Destroy();
}

bool wxFrame::Destroy()
{
    wxASSERT_MSG( (m_widget != NULL), "invalid frame" );
  
    if (!wxPendingDelete.Member(this)) wxPendingDelete.Append(this);

    return TRUE;
}

wxPoint wxFrame::GetClientAreaOrigin() const
{
    wxPoint pt(0, 0);
    if (m_frameMenuBar)
    {
        int h = 0;
        m_frameMenuBar->GetSize( (int*)NULL, &h );
        pt.y += h + 2;
    }
    if (m_frameToolBar)
    {
        int h = 0;
        m_frameToolBar->GetSize( (int*)NULL, &h );
        pt.y += h;
    }
    return pt;
}

void wxFrame::SetSize( int x, int y, int width, int height, int sizeFlags )
{
    wxASSERT_MSG( (m_widget != NULL), "invalid window" );
  
    // Don't do anything for children of wxMDIChildFrame
    if (!m_wxwindow) return;

    if (m_resizing) return; // I don't like recursions
    m_resizing = TRUE;

    int old_x = m_x;
    int old_y = m_y;
    int old_width = m_width;
    int old_height = m_height;
  
    if ((sizeFlags & wxSIZE_USE_EXISTING) == wxSIZE_USE_EXISTING)
    {
        if (x != -1) m_x = x;
        if (y != -1) m_y = y;
        if (width != -1) m_width = width;
        if (height != -1) m_height = height;
    }
    else
    {
        m_x = x;
        m_y = y;
        m_width = width;
        m_height = height;
    }

    if ((sizeFlags & wxSIZE_AUTO_WIDTH) == wxSIZE_AUTO_WIDTH)
    {
        if (width == -1) m_width = 80;
    }

    if ((sizeFlags & wxSIZE_AUTO_HEIGHT) == wxSIZE_AUTO_HEIGHT)
    {
       if (height == -1) m_height = 26;
    }
  
    if ((m_minWidth != -1) && (m_width < m_minWidth)) m_width = m_minWidth;
    if ((m_minHeight != -1) && (m_height < m_minHeight)) m_height = m_minHeight;
    if ((m_maxWidth != -1) && (m_width > m_maxWidth)) m_width = m_minWidth;
    if ((m_maxHeight != -1) && (m_height > m_maxHeight)) m_height = m_minHeight;

    if ((m_x != -1) || (m_y != -1))
    {
        if ((m_x != old_x) || (m_y != old_y)) 
            gtk_widget_set_uposition( m_widget, m_x, m_y );
    }
  
    if ((m_width != old_width) || (m_height != old_height))
    {
        gtk_widget_set_usize( m_widget, m_width, m_height );
    }
  
    m_sizeSet = TRUE;

    wxSizeEvent event( wxSize(m_width,m_height), GetId() );
    event.SetEventObject( this );
    GetEventHandler()->ProcessEvent( event );

    m_resizing = FALSE;
}

void wxFrame::SetSize( int width, int height )
{
    SetSize( -1, -1, width, height, wxSIZE_USE_EXISTING );
}

void wxFrame::Centre( int direction )
{
    wxASSERT_MSG( (m_widget != NULL), "invalid frame" );
  
    int x = 0;
    int y = 0;
    
    if (direction & wxHORIZONTAL == wxHORIZONTAL) x = (gdk_screen_width () - m_width) / 2;
    if (direction & wxVERTICAL == wxVERTICAL) y = (gdk_screen_height () - m_height) / 2;
  
    Move( x, y );
}

void wxFrame::GetClientSize( int *width, int *height ) const
{
    wxASSERT_MSG( (m_widget != NULL), "invalid frame" );
  
    wxWindow::GetClientSize( width, height );
    if (height)
    {
        if (m_frameMenuBar) (*height) -= wxMENU_HEIGHT;
        if (m_frameStatusBar) (*height) -= wxSTATUS_HEIGHT;
        if (m_frameToolBar)
        {
            int y = 0;
            m_frameToolBar->GetSize( (int *) NULL, &y );
            (*height) -= y;
        }
    }
}

void wxFrame::SetClientSize( int const width, int const height )
{
    wxASSERT_MSG( (m_widget != NULL), "invalid frame" );
  
    int h = height;
    if (m_frameMenuBar) h += wxMENU_HEIGHT;
    if (m_frameStatusBar) h += wxSTATUS_HEIGHT;
    if (m_frameToolBar)
    {
        int y = 0;
        m_frameToolBar->GetSize( (int *) NULL, &y );
        h += y;
    }
    wxWindow::SetClientSize( width, h );
}

void wxFrame::GtkOnSize( int WXUNUSED(x), int WXUNUSED(y), int width, int height )
{
    // due to a bug in gtk, x,y are always 0
    // m_x = x;
    // m_y = y;

    if (m_resizing) return;
    m_resizing = TRUE;
    
    if (!m_wxwindow) return;
  
    m_width = width;
    m_height = height;
  
    if ((m_minWidth != -1) && (m_width < m_minWidth)) m_width = m_minWidth;
    if ((m_minHeight != -1) && (m_height < m_minHeight)) m_height = m_minHeight;
    if ((m_maxWidth != -1) && (m_width > m_maxWidth)) m_width = m_minWidth;
    if ((m_maxHeight != -1) && (m_height > m_maxHeight)) m_height = m_minHeight;

    gtk_widget_set_usize( m_widget, m_width, m_height );
    
    // This emulates the new wxMSW behaviour

    if (m_frameMenuBar)
    {
        m_frameMenuBar->m_x = 1;  
        m_frameMenuBar->m_y = 1;
        m_frameMenuBar->m_width = m_width-2;
        m_frameMenuBar->m_height = wxMENU_HEIGHT-2;
        gtk_myfixed_move( GTK_MYFIXED(m_wxwindow), m_frameMenuBar->m_widget, 1, 1 );
        gtk_widget_set_usize( m_frameMenuBar->m_widget, m_width-2, wxMENU_HEIGHT-2 );
    }

    if (m_frameToolBar)
    {
        int y = 0;
        if (m_frameMenuBar) y = wxMENU_HEIGHT;
        int h = m_frameToolBar->m_height;
    
        m_frameToolBar->m_x = 2;  
        gtk_myfixed_move( GTK_MYFIXED(m_wxwindow), m_frameToolBar->m_widget, 2, y );
        gtk_widget_set_usize( m_frameToolBar->m_widget, m_width-3, h );
    }
  
    if (m_frameStatusBar)
    {
        // OK, this hurts in the eye, but I don't want to call SetSize()
        // because I don't want to call any non-native functions here.
        m_frameStatusBar->m_x = 0;  
        m_frameStatusBar->m_y = m_height-wxSTATUS_HEIGHT;
        m_frameStatusBar->m_width = m_width;
        m_frameStatusBar->m_height = wxSTATUS_HEIGHT;
        gtk_myfixed_move( GTK_MYFIXED(m_wxwindow), m_frameStatusBar->m_widget, 0, m_height-wxSTATUS_HEIGHT );
        gtk_widget_set_usize( m_frameStatusBar->m_widget, m_width, wxSTATUS_HEIGHT );
    }
    
    m_sizeSet = TRUE;
    
    wxSizeEvent event( wxSize(m_width,m_height), GetId() );
    event.SetEventObject( this );
    GetEventHandler()->ProcessEvent( event );
    
    m_resizing = FALSE;
}

void wxFrame::OnIdle(wxIdleEvent& WXUNUSED(event) )
{
    if (!m_sizeSet)
        GtkOnSize( m_x, m_y, m_width, m_height );
  
    DoMenuUpdates();
}

void wxFrame::OnSize( wxSizeEvent &WXUNUSED(event) )
{
    wxASSERT_MSG( (m_widget != NULL), "invalid frame" );
  
    if (GetAutoLayout())
    {
        Layout();
    }
    else 
    {
        // no child: go out !
        if (!GetChildren()->First()) return;
      
        // do we have exactly one child?
        wxWindow *child = (wxWindow *) NULL;
        for(wxNode *node = GetChildren()->First(); node; node = node->Next())
        {
            wxWindow *win = (wxWindow *)node->Data();
            if (!IS_KIND_OF(win,wxFrame) && !IS_KIND_OF(win,wxDialog)
#if 0  // not in m_children anyway ?
              && (win != m_frameMenuBar) &&
              (win != m_frameToolBar) &&
              (win != m_frameStatusBar)
#endif
            )
            {
	        // it's the second one: do nothing
                if (child) return;
                child = win;
            }
        }

        // yes: set it's size to fill all the frame
        int client_x, client_y;
        GetClientSize( &client_x, &client_y );
        child->SetSize( 1, 1, client_x-2, client_y);
    }
}

static void SetInvokingWindow( wxMenu *menu, wxWindow *win )
{
    menu->SetInvokingWindow( win );
    wxNode *node = menu->m_items.First();
    while (node)
    {
        wxMenuItem *menuitem = (wxMenuItem*)node->Data();
        if (menuitem->IsSubMenu())
            SetInvokingWindow( menuitem->GetSubMenu(), win );
        node = node->Next();
    }
}

void wxFrame::SetMenuBar( wxMenuBar *menuBar )
{
    wxASSERT_MSG( (m_widget != NULL), "invalid frame" );
    wxASSERT_MSG( (m_wxwindow != NULL), "invalid frame" );
  
    m_frameMenuBar = menuBar;
  
    if (m_frameMenuBar)
    {
        wxNode *node = m_frameMenuBar->m_menus.First();
        while (node)
        {
            wxMenu *menu = (wxMenu*)node->Data();
            SetInvokingWindow( menu, this );
            node = node->Next();
        }
    
        if (m_frameMenuBar->m_parent != this)
        {
            m_frameMenuBar->m_parent = this;
            gtk_myfixed_put( GTK_MYFIXED(m_wxwindow),
                m_frameMenuBar->m_widget, m_frameMenuBar->m_x, m_frameMenuBar->m_y );
        }
    }
}

wxMenuBar *wxFrame::GetMenuBar(void) const
{
    return m_frameMenuBar;
}

wxToolBar* wxFrame::CreateToolBar(long style, wxWindowID id, const wxString& name)
{
    wxASSERT_MSG( (m_widget != NULL), "invalid frame" );
  
    wxCHECK_MSG( m_frameToolBar == NULL, FALSE, "recreating toolbar in wxFrame" );

    m_frameToolBar = OnCreateToolBar( style, id, name );
  
    GetChildren()->DeleteObject( m_frameToolBar );
  
    return m_frameToolBar;
}

wxToolBar* wxFrame::OnCreateToolBar( long style, wxWindowID id, const wxString& name )
{
    return new wxToolBar( this, id, wxDefaultPosition, wxDefaultSize, style, name );
}

wxToolBar *wxFrame::GetToolBar(void) const 
{ 
    return m_frameToolBar; 
}

wxStatusBar* wxFrame::CreateStatusBar( int number, long style, wxWindowID id, const wxString& name )
{
    wxASSERT_MSG( (m_widget != NULL), "invalid frame" );
  
    wxCHECK_MSG( m_frameStatusBar == NULL, FALSE, "recreating status bar in wxFrame" );

    m_frameStatusBar = OnCreateStatusBar( number, style, id, name );
  
    return m_frameStatusBar;
}

wxStatusBar *wxFrame::OnCreateStatusBar( int number, long style, wxWindowID id, const wxString& name )
{
    wxStatusBar *statusBar = (wxStatusBar *) NULL;
    
    statusBar = new wxStatusBar(this, id, wxPoint(0, 0), wxSize(100, 20), style, name);
    
    // Set the height according to the font and the border size
    wxClientDC dc(statusBar);
    dc.SetFont( *statusBar->GetFont() );

    long x, y;
    dc.GetTextExtent( "X", &x, &y );

    int height = (int)( (y  * 1.1) + 2* statusBar->GetBorderY());

    statusBar->SetSize( -1, -1, 100, height );

    statusBar->SetFieldsCount( number );
    return statusBar;
}

void wxFrame::SetStatusText(const wxString& text, int number)
{
    wxASSERT_MSG( (m_widget != NULL), "invalid frame" );
  
    wxCHECK_RET( m_frameStatusBar != NULL, "no statusbar to set text for" );

    m_frameStatusBar->SetStatusText(text, number);
}

void wxFrame::SetStatusWidths(int n, const int widths_field[] )
{
    wxASSERT_MSG( (m_widget != NULL), "invalid frame" );
  
    wxCHECK_RET( m_frameStatusBar != NULL, "no statusbar to set widths for" );

    m_frameStatusBar->SetStatusWidths(n, widths_field);
}

wxStatusBar *wxFrame::GetStatusBar(void) const
{
    return m_frameStatusBar;
}

void wxFrame::SetTitle( const wxString &title )
{
    wxASSERT_MSG( (m_widget != NULL), "invalid frame" );
  
    m_title = title;
    if (m_title.IsNull()) m_title = "";
    gtk_window_set_title( GTK_WINDOW(m_widget), title );
}

void wxFrame::SetIcon( const wxIcon &icon )
{
    wxASSERT_MSG( (m_widget != NULL), "invalid frame" );
  
    m_icon = icon;
    if (!icon.Ok()) return;
  
    wxMask *mask = icon.GetMask();
    GdkBitmap *bm = (GdkBitmap *) NULL;
    if (mask) bm = mask->GetBitmap();
  
    gdk_window_set_icon( m_widget->window, (GdkWindow *) NULL, icon.GetPixmap(), bm );
}
