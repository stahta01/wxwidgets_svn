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

#include "glib.h"
#include "gdk/gdk.h"
#include "gtk/gtk.h"
#include "wx/gtk/win_gtk.h"

//-----------------------------------------------------------------------------
// constants
//-----------------------------------------------------------------------------

const int wxMENU_HEIGHT    = 27;
const int wxSTATUS_HEIGHT  = 25;

//-----------------------------------------------------------------------------
// data
//-----------------------------------------------------------------------------

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

    wxMoveEvent mevent( wxPoint(win->m_x,win->m_y), win->GetId() );
    mevent.SetEventObject( win );
    win->GetEventHandler()->ProcessEvent( mevent );

    return FALSE;
}

//-----------------------------------------------------------------------------
// InsertChild for wxFrame
//-----------------------------------------------------------------------------

/* Callback for wxFrame. This very strange beast has to be used because
 * C++ has no virtual methods in a constructor. We have to emulate a
 * virtual function here as wxWindows requires different ways to insert
 * a child in container classes. */

static void wxInsertChildInFrame( wxWindow* parent, wxWindow* child )
{
    if (wxIS_KIND_OF(child,wxToolBar) || wxIS_KIND_OF(child,wxMenuBar))
    {
        /* these are outside the client area */
	wxFrame* frame = (wxFrame*) parent;
        gtk_myfixed_put( GTK_MYFIXED(frame->m_mainWidget),
                         GTK_WIDGET(child->m_widget),
                         child->m_x,
                         child->m_y );
    }
    else
    {
        /* these are inside the client area */
        gtk_myfixed_put( GTK_MYFIXED(parent->m_wxwindow),
                         GTK_WIDGET(child->m_widget),
                         child->m_x,
                         child->m_y );
    }

    gtk_widget_set_usize( GTK_WIDGET(child->m_widget),
                          child->m_width,
                          child->m_height );

    /* resize on OnInternalIdle */
    parent->m_sizeSet = FALSE;

    if (parent->m_windowStyle & wxTAB_TRAVERSAL)
    {
        /* we now allow a window to get the focus as long as it
           doesn't have any children. */
        GTK_WIDGET_UNSET_FLAGS( parent->m_wxwindow, GTK_CAN_FOCUS );
    }
}

//-----------------------------------------------------------------------------
// wxFrame
//-----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(wxFrame, wxWindow)
    EVT_SIZE(wxFrame::OnSize)
    EVT_CLOSE(wxFrame::OnCloseWindow)
    EVT_MENU_HIGHLIGHT_ALL(wxFrame::OnMenuHighlight)
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC_CLASS(wxFrame,wxWindow)

wxFrame::wxFrame()
{
    m_frameMenuBar = (wxMenuBar *) NULL;
    m_mdiMenuBar = (wxMenuBar *) NULL;
    m_frameStatusBar = (wxStatusBar *) NULL;
    m_frameToolBar = (wxToolBar *) NULL;
    m_sizeSet = FALSE;
    m_miniEdge = 0;
    m_miniTitle = 0;
    m_mainWidget = (GtkWidget*) NULL;
}

wxFrame::wxFrame( wxWindow *parent, wxWindowID id, const wxString &title,
      const wxPoint &pos, const wxSize &size,
      long style, const wxString &name )
{
    m_frameMenuBar = (wxMenuBar *) NULL;
    m_mdiMenuBar = (wxMenuBar *) NULL;
    m_frameStatusBar = (wxStatusBar *) NULL;
    m_frameToolBar = (wxToolBar *) NULL;
    m_sizeSet = FALSE;
    m_miniEdge = 0;
    m_miniTitle = 0;
    m_mainWidget = (GtkWidget*) NULL;
    Create( parent, id, title, pos, size, style, name );
}

bool wxFrame::Create( wxWindow *parent, wxWindowID id, const wxString &title,
      const wxPoint &pos, const wxSize &size,
      long style, const wxString &name )
{
    wxTopLevelWindows.Append( this );

    m_needParent = FALSE;

    PreCreation( parent, id, pos, size, style, name );

    m_title = title;
    
    m_insertCallback = wxInsertChildInFrame;

    GtkWindowType win_type = GTK_WINDOW_TOPLEVEL;
    if (style & wxSIMPLE_BORDER) win_type = GTK_WINDOW_POPUP;

    m_widget = gtk_window_new( win_type );

    gtk_window_set_title( GTK_WINDOW(m_widget), title );
    GTK_WIDGET_UNSET_FLAGS( m_widget, GTK_CAN_FOCUS );

    /* needed ? */
    gtk_window_set_policy( GTK_WINDOW(m_widget), 1, 1, 0 );

    gtk_signal_connect( GTK_OBJECT(m_widget), "delete_event",
        GTK_SIGNAL_FUNC(gtk_frame_delete_callback), (gpointer)this );

    /* m_mainWidget holds the toolbar, the menubar and the client area */
    m_mainWidget = gtk_myfixed_new();
    gtk_widget_show( m_mainWidget );
    GTK_WIDGET_UNSET_FLAGS( m_mainWidget, GTK_CAN_FOCUS );
    gtk_container_add( GTK_CONTAINER(m_widget), m_mainWidget );
    gtk_widget_realize( m_mainWidget );
    
    /* m_wxwindow only represents the client area without toolbar and menubar */
    m_wxwindow = gtk_myfixed_new();
    gtk_widget_show( m_wxwindow );
    GTK_WIDGET_UNSET_FLAGS( m_wxwindow, GTK_CAN_FOCUS );
    gtk_container_add( GTK_CONTAINER(m_mainWidget), m_wxwindow );

    if (m_parent) m_parent->AddChild( this );

    PostCreation();

    gtk_widget_realize( m_widget );
    
    /* all this is for Motif Window Manager "hints" and is supposed to be
       recognized by other WM as well. not tested. */
    long decor = (long) GDK_DECOR_ALL;
    long func = (long) GDK_FUNC_ALL;
    if ((m_windowStyle & wxCAPTION) == 0)
	decor |= GDK_DECOR_TITLE;
/*
    if ((m_windowStyle & wxMINIMIZE) == 0)
	func |= GDK_FUNC_MINIMIZE;
    if ((m_windowStyle & wxMAXIMIZE) == 0)
	func |= GDK_FUNC_MAXIMIZE;
*/
    if ((m_windowStyle & wxSYSTEM_MENU) == 0)
	decor |= GDK_DECOR_MENU;
    if ((m_windowStyle & wxMINIMIZE_BOX) == 0)
	decor |= GDK_DECOR_MINIMIZE;
    if ((m_windowStyle & wxMAXIMIZE_BOX) == 0)
	decor |= GDK_DECOR_MAXIMIZE;
    if ((m_windowStyle & wxRESIZE_BORDER) == 0)
	func |= GDK_FUNC_RESIZE;
    gdk_window_set_decorations(m_widget->window, (GdkWMDecoration)decor);
    gdk_window_set_functions(m_widget->window, (GdkWMFunction)func);
      
    /* the user resized the frame by dragging etc. */
    gtk_signal_connect( GTK_OBJECT(m_widget), "size_allocate",
        GTK_SIGNAL_FUNC(gtk_frame_size_callback), (gpointer)this );

    /* the only way to get the window size is to connect to this event */
    gtk_signal_connect( GTK_OBJECT(m_widget), "configure_event",
        GTK_SIGNAL_FUNC(gtk_frame_configure_callback), (gpointer)this );

    return TRUE;
}

wxFrame::~wxFrame()
{
    if (m_frameMenuBar) delete m_frameMenuBar;
    m_frameMenuBar = (wxMenuBar *) NULL;
    
    if (m_frameStatusBar) delete m_frameStatusBar;
    m_frameStatusBar = (wxStatusBar *) NULL;
    
    if (m_frameToolBar) delete m_frameToolBar;
    m_frameToolBar = (wxToolBar *) NULL;

    wxTopLevelWindows.DeleteObject( this );

    if (wxTheApp->GetTopWindow() == this)
    {
        wxTheApp->SetTopWindow( (wxWindow*) NULL );
    }

    if (wxTopLevelWindows.Number() == 0)
    {
        wxTheApp->ExitMainLoop();
    }
}

bool wxFrame::Show( bool show )
{
    wxASSERT_MSG( (m_widget != NULL), "invalid frame" );

    if (show && !m_sizeSet)
    {
        /* by calling GtkOnSize here, we don't have to call
           either after showing the frame, which would entail
           much ugly flicker or from within the size_allocate
           handler, because GTK 1.1.X forbids that. */

        GtkOnSize( m_x, m_y, m_width, m_height );
    }

    return wxWindow::Show( show );
}

bool wxFrame::Destroy()
{
    wxASSERT_MSG( (m_widget != NULL), "invalid frame" );

    if (!wxPendingDelete.Member(this)) wxPendingDelete.Append(this);

    return TRUE;
}

void wxFrame::DoSetSize( int x, int y, int width, int height, int sizeFlags )
{
    wxASSERT_MSG( (m_widget != NULL), "invalid frame" );

    /* this shouldn't happen: wxFrame, wxMDIParentFrame and wxMDIChildFrame have m_wxwindow */
    wxASSERT_MSG( (m_wxwindow != NULL), "invalid frame" );
    
    /* avoid recursions */
    if (m_resizing) return; 
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
    if ((m_maxWidth != -1) && (m_width > m_maxWidth)) m_width = m_maxWidth;
    if ((m_maxHeight != -1) && (m_height > m_maxHeight)) m_height = m_maxHeight;

    if ((m_x != -1) || (m_y != -1))
    {
        if ((m_x != old_x) || (m_y != old_y))
        {
            /* m_sizeSet = FALSE; */
            gtk_widget_set_uposition( m_widget, m_x, m_y );
        }
    }

    if ((m_width != old_width) || (m_height != old_height))
    {
        /* we set the size in GtkOnSize, i.e. mostly the actual resizing is
	   done either directly before the frame is shown or in idle time
	   so that different calls to SetSize() don't lead to flicker. */
        m_sizeSet = FALSE;
    }

    m_resizing = FALSE;
}

void wxFrame::Centre( int direction )
{
    wxASSERT_MSG( (m_widget != NULL), "invalid frame" );

    int x = 0;
    int y = 0;

    if ((direction & wxHORIZONTAL) == wxHORIZONTAL) x = (gdk_screen_width () - m_width) / 2;
    if ((direction & wxVERTICAL) == wxVERTICAL) y = (gdk_screen_height () - m_height) / 2;

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
        (*height) -= m_miniEdge*2 + m_miniTitle;
    }
    if (width)
    {
        (*width) -= m_miniEdge*2;
    }
}

void wxFrame::DoSetClientSize( int width, int height )
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
    wxWindow::DoSetClientSize( width + m_miniEdge*2, h  + m_miniEdge*2 + m_miniTitle );
}

void wxFrame::GtkOnSize( int WXUNUSED(x), int WXUNUSED(y), int width, int height )
{
    // due to a bug in gtk, x,y are always 0
    // m_x = x;
    // m_y = y;

    /* avoid recursions */
    if (m_resizing) return;
    m_resizing = TRUE;

    /* this shouldn't happen: wxFrame, wxMDIParentFrame and wxMDIChildFrame have m_wxwindow */
    wxASSERT_MSG( (m_wxwindow != NULL), "invalid frame" );
    
    m_width = width;
    m_height = height;

    /* space occupied by m_frameToolBar and m_frameMenuBar */
    int client_area_y_offset = 0; 

    /* wxMDIChildFrame derives from wxFrame but it _is_ a wxWindow as it uses
       wxWindow::Create to create it's GTK equivalent. m_mainWidget is only
       set in wxFrame::Create so it is used to check what kind of frame we
       have here. if m_mainWidget is NULL it is a wxMDIChildFrame and so we
       skip the part which handles m_frameMenuBar, m_frameToolBar and (most
       importantly) m_mainWidget */
       
    if (m_mainWidget)
    {
        /* check if size is in legal range */
        if ((m_minWidth != -1) && (m_width < m_minWidth)) m_width = m_minWidth;
        if ((m_minHeight != -1) && (m_height < m_minHeight)) m_height = m_minHeight;
        if ((m_maxWidth != -1) && (m_width > m_maxWidth)) m_width = m_maxWidth;
        if ((m_maxHeight != -1) && (m_height > m_maxHeight)) m_height = m_maxHeight;

        /* I revert back to wxGTK's original behaviour. m_mainWidget holds the
         * menubar, the toolbar and the client area, which is represented by
         * m_wxwindow.
         * this hurts in the eye, but I don't want to call SetSize()
         * because I don't want to call any non-native functions here. */
     
        if (m_frameMenuBar)
        {
            int xx = m_miniEdge;
            int yy = m_miniEdge + m_miniTitle;
            int ww = m_width  - 2*m_miniEdge;
            int hh = wxMENU_HEIGHT;
            m_frameMenuBar->m_x = xx;
            m_frameMenuBar->m_y = yy;
            m_frameMenuBar->m_width = ww;
            m_frameMenuBar->m_height = hh;

            gtk_myfixed_move( GTK_MYFIXED(m_mainWidget), m_frameMenuBar->m_widget, xx, yy );
            gtk_widget_set_usize( m_frameMenuBar->m_widget, ww, hh );
	
	    client_area_y_offset += hh;
        }
    
        if (m_frameToolBar)
        {
            int xx = m_miniEdge;
            int yy = m_miniEdge + m_miniTitle;
            if ((m_frameMenuBar) || (m_mdiMenuBar)) yy += wxMENU_HEIGHT;
            int ww = m_width - 2*m_miniEdge;
            int hh = m_frameToolBar->m_height;

            m_frameToolBar->m_x = xx;
            m_frameToolBar->m_y = yy;
            m_frameToolBar->m_height = hh;
            m_frameToolBar->m_width = ww;

            gtk_myfixed_move( GTK_MYFIXED(m_mainWidget), m_frameToolBar->m_widget, xx, yy );
            gtk_widget_set_usize( m_frameToolBar->m_widget, ww, hh );
	
	    client_area_y_offset += hh;
        }
    
        gtk_myfixed_move( GTK_MYFIXED(m_mainWidget), m_wxwindow, 0, client_area_y_offset );
    }
    
    gtk_widget_set_usize( m_wxwindow, m_width, m_height-client_area_y_offset );

    if (m_frameStatusBar)
    {
        int xx = 0 + m_miniEdge;
        int yy = m_height - wxSTATUS_HEIGHT - m_miniEdge - client_area_y_offset;
        int ww = m_width - 2*m_miniEdge;
        int hh = wxSTATUS_HEIGHT;

        m_frameStatusBar->m_x = xx;
        m_frameStatusBar->m_y = yy;
        m_frameStatusBar->m_width = ww;
        m_frameStatusBar->m_height = hh;

        gtk_myfixed_move( GTK_MYFIXED(m_wxwindow), m_frameStatusBar->m_widget, xx, yy );
        gtk_widget_set_usize( m_frameStatusBar->m_widget, ww, hh );
    }

    /* we actually set the size of a frame here and no-where else */
    gtk_widget_set_usize( m_widget, m_width, m_height );

    m_sizeSet = TRUE;

    /* send size event to frame */
    wxSizeEvent event( wxSize(m_width,m_height), GetId() );
    event.SetEventObject( this );
    GetEventHandler()->ProcessEvent( event );

    /* send size event to status bar */
    if (m_frameStatusBar)
    {
        wxSizeEvent event2( wxSize(m_frameStatusBar->m_width,m_frameStatusBar->m_height), m_frameStatusBar->GetId() );
        event2.SetEventObject( m_frameStatusBar );
        m_frameStatusBar->GetEventHandler()->ProcessEvent( event2 );
    }

    m_resizing = FALSE;
}

void wxFrame::OnInternalIdle()
{
    if (!m_sizeSet)
        GtkOnSize( m_x, m_y, m_width, m_height );

    DoMenuUpdates();
}

void wxFrame::OnCloseWindow( wxCloseEvent& event )
{
    Destroy();
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
        // do we have exactly one child?
        wxWindow *child = (wxWindow *)NULL;
        for ( wxNode *node = GetChildren().First(); node; node = node->Next() )
        {
            wxWindow *win = (wxWindow *)node->Data();
            if ( !wxIS_KIND_OF(win,wxFrame) && !wxIS_KIND_OF(win,wxDialog) )
            {
                if ( child )
                {
                    // it's the second one: do nothing
                    return;
                }

                child = win;
            }
        }

        // no children at all?
        if ( child )
        {
            // yes: set it's size to fill all the frame
            int client_x, client_y;
            GetClientSize( &client_x, &client_y );
            child->SetSize( 1, 1, client_x-2, client_y-2 );
        }
    }
}

static void SetInvokingWindow( wxMenu *menu, wxWindow *win )
{
    menu->SetInvokingWindow( win );
    wxNode *node = menu->GetItems().First();
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
        wxNode *node = m_frameMenuBar->GetMenus().First();
        while (node)
        {
            wxMenu *menu = (wxMenu*)node->Data();
            SetInvokingWindow( menu, this );
            node = node->Next();
        }

        if (m_frameMenuBar->m_parent != this)
        {
            m_frameMenuBar->m_parent = this;
            gtk_myfixed_put( GTK_MYFIXED(m_mainWidget),
                m_frameMenuBar->m_widget, m_frameMenuBar->m_x, m_frameMenuBar->m_y );

            /* an mdi child menu bar might be underneath */
            if (m_mdiMenuBar)
                m_frameMenuBar->Show( FALSE );
        }
    }

    m_sizeSet = FALSE;
}

wxMenuBar *wxFrame::GetMenuBar() const
{
    return m_frameMenuBar;
}

void wxFrame::OnMenuHighlight(wxMenuEvent& event)
{
    if (GetStatusBar())
    {
        // if no help string found, we will clear the status bar text
        wxString helpString;

        int menuId = event.GetMenuId();
        if ( menuId != -1 )
        {
            wxMenuBar *menuBar = GetMenuBar();
            if (menuBar)
            {
                helpString = menuBar->GetHelpString(menuId);
            }
        }

        SetStatusText(helpString);
    }
}

wxToolBar* wxFrame::CreateToolBar(long style, wxWindowID id, const wxString& name)
{
    wxASSERT_MSG( (m_widget != NULL), "invalid frame" );

    wxCHECK_MSG( m_frameToolBar == NULL, FALSE, "recreating toolbar in wxFrame" );

    m_frameToolBar = OnCreateToolBar( style, id, name );

    GetChildren().DeleteObject( m_frameToolBar );

    m_sizeSet = FALSE;

    return m_frameToolBar;
}

wxToolBar* wxFrame::OnCreateToolBar( long style, wxWindowID id, const wxString& name )
{
    return new wxToolBar( this, id, wxDefaultPosition, wxDefaultSize, style, name );
}

wxToolBar *wxFrame::GetToolBar() const
{
    return m_frameToolBar;
}

wxStatusBar* wxFrame::CreateStatusBar( int number, long style, wxWindowID id, const wxString& name )
{
    wxASSERT_MSG( (m_widget != NULL), "invalid frame" );

    wxCHECK_MSG( m_frameStatusBar == NULL, FALSE, "recreating status bar in wxFrame" );

    m_frameStatusBar = OnCreateStatusBar( number, style, id, name );

    m_sizeSet = FALSE;

    return m_frameStatusBar;
}

wxStatusBar *wxFrame::OnCreateStatusBar( int number, long style, wxWindowID id, const wxString& name )
{
    wxStatusBar *statusBar = (wxStatusBar *) NULL;

    statusBar = new wxStatusBar(this, id, wxPoint(0, 0), wxSize(100, 20), style, name);

    // Set the height according to the font and the border size
    wxClientDC dc(statusBar);
    dc.SetFont( statusBar->GetFont() );

    long x, y;
    dc.GetTextExtent( "X", &x, &y );

    int height = (int)( (y  * 1.1) + 2* statusBar->GetBorderY());

    statusBar->SetSize( -1, -1, 100, height );

    statusBar->SetFieldsCount( number );
    return statusBar;
}

void wxFrame::Command( int id )
{
    wxCommandEvent commandEvent(wxEVT_COMMAND_MENU_SELECTED, id);
    commandEvent.SetInt( id );
    commandEvent.SetEventObject( this );

    wxMenuBar *bar = GetMenuBar();
    if (!bar) return;

    wxMenuItem *item = bar->FindItemForId(id) ;
    if (item && item->IsCheckable())
    {
        bar->Check(id,!bar->Checked(id)) ;
    }

    wxEvtHandler* evtHandler = GetEventHandler();

    evtHandler->ProcessEvent(commandEvent);
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

wxStatusBar *wxFrame::GetStatusBar() const
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

