/////////////////////////////////////////////////////////////////////////////
// Name:        dialog.cpp
// Purpose:
// Author:      Robert Roebling
// Id:          $Id$
// Copyright:   (c) 1998 Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "dialog.h"
#endif

#include "wx/dialog.h"
#include "wx/frame.h"
#include "wx/app.h"
#include "wx/cursor.h"

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include "wx/gtk/win_gtk.h"

//-----------------------------------------------------------------------------
// idle system
//-----------------------------------------------------------------------------

extern void wxapp_install_idle_handler();
extern bool g_isIdle;
extern int g_openDialogs;

//-----------------------------------------------------------------------------
// data
//-----------------------------------------------------------------------------

extern wxList wxPendingDelete;

//-----------------------------------------------------------------------------
// "focus" from m_window
//-----------------------------------------------------------------------------

static gint gtk_dialog_focus_callback( GtkWidget *widget, GtkDirectionType WXUNUSED(d), wxWindow *WXUNUSED(win) )
{
    if (g_isIdle)
        wxapp_install_idle_handler();

    // This disables GTK's tab traversal
    gtk_signal_emit_stop_by_name( GTK_OBJECT(widget), "focus" );
    return TRUE;
}

//-----------------------------------------------------------------------------
// "delete_event"
//-----------------------------------------------------------------------------

bool gtk_dialog_delete_callback( GtkWidget *WXUNUSED(widget), GdkEvent *WXUNUSED(event), wxDialog *win )
{
    if (g_isIdle)
        wxapp_install_idle_handler();

    win->Close();

    return TRUE;
}

//-----------------------------------------------------------------------------
// "size_allocate"
//-----------------------------------------------------------------------------

static void gtk_dialog_size_callback( GtkWidget *WXUNUSED(widget), GtkAllocation* alloc, wxDialog *win )
{
    if (g_isIdle)
        wxapp_install_idle_handler();

    if (!win->m_hasVMT) return;

    if ((win->m_width != alloc->width) || (win->m_height != alloc->height))
    {
        win->m_width = alloc->width;
        win->m_height = alloc->height;
        win->UpdateSize();
    }
}

//-----------------------------------------------------------------------------
// "configure_event"
//-----------------------------------------------------------------------------

static gint
#if (GTK_MINOR_VERSION > 0)
gtk_dialog_configure_callback( GtkWidget *WXUNUSED(widget), GdkEventConfigure *WXUNUSED(event), wxDialog *win )
#else
gtk_dialog_configure_callback( GtkWidget *WXUNUSED(widget), GdkEventConfigure *event, wxDialog *win )
#endif
{
    if (g_isIdle)
        wxapp_install_idle_handler();

    if (!win->m_hasVMT) return FALSE;

#if (GTK_MINOR_VERSION > 0)
    int x = 0;
    int y = 0;
    gdk_window_get_root_origin( win->m_widget->window, &x, &y );
    win->m_x = x;
    win->m_y = y;
#else
    win->m_x = event->x;
    win->m_y = event->y;
#endif

    wxMoveEvent mevent( wxPoint(win->m_x,win->m_y), win->GetId() );
    mevent.SetEventObject( win );
    win->GetEventHandler()->ProcessEvent( mevent );

    return FALSE;
}

//-----------------------------------------------------------------------------
// "realize" from m_widget
//-----------------------------------------------------------------------------

/* we cannot MWM hints and icons before the widget has been realized,
   so we do this directly after realization */

static gint
gtk_dialog_realized_callback( GtkWidget *widget, wxDialog *win )
{
    if (g_isIdle)
        wxapp_install_idle_handler();

    /* I haven't been able to set the position of
       the dialog before it is shown, so I set the
       position in "realize" */
    gtk_widget_set_uposition( widget, win->m_x, win->m_y );

    /* all this is for Motif Window Manager "hints" and is supposed to be
       recognized by other WM as well. not tested. */
    long decor = (long) GDK_DECOR_BORDER;
    long func = (long) GDK_FUNC_MOVE ;

    /* Some WM don't display any border around the frame contents if
       used with these hints, so we add a resize border around it,
       without automatically allowinng it to be resized though.

       This avoids the problem, but looks odd. What shall we do?
    */
    decor |= GDK_DECOR_RESIZEH;

    if ((win->GetWindowStyle() & wxCAPTION) != 0)
        decor |= GDK_DECOR_TITLE;
    if ((win->GetWindowStyle() & wxSYSTEM_MENU) != 0)
    {
       decor |= GDK_DECOR_MENU;
       func |= GDK_FUNC_CLOSE;
    }
    if ((win->GetWindowStyle() & wxMINIMIZE_BOX) != 0)
    {
        func |= GDK_FUNC_MINIMIZE;
        decor |= GDK_DECOR_MINIMIZE;
    }
    if ((win->GetWindowStyle() & wxMAXIMIZE_BOX) != 0)
    {
        decor |= GDK_DECOR_MAXIMIZE;
        func |= GDK_FUNC_MAXIMIZE;
    }
    if ((win->GetWindowStyle() & wxRESIZE_BORDER) != 0)
    {
       func |= GDK_FUNC_RESIZE;
       decor |= GDK_DECOR_RESIZEH;
    }
    gdk_window_set_decorations( win->m_widget->window, (GdkWMDecoration)decor);
    gdk_window_set_functions( win->m_widget->window, (GdkWMFunction)func);

    /* GTK's shrinking/growing policy */
    if ((win->GetWindowStyle() & wxRESIZE_BORDER) == 0)
        gtk_window_set_policy(GTK_WINDOW(win->m_widget), 0, 0, 1);
    else
        gtk_window_set_policy(GTK_WINDOW(win->m_widget), 1, 1, 1);

    /* set size hints */
    gint flag = 0; // GDK_HINT_POS;
    if ((win->GetMinWidth() != -1) || (win->GetMinHeight() != -1)) flag |= GDK_HINT_MIN_SIZE;
    if ((win->GetMaxWidth() != -1) || (win->GetMaxHeight() != -1)) flag |= GDK_HINT_MAX_SIZE;
    if (flag)
    {
        gdk_window_set_hints( win->m_widget->window,
                              win->m_x, win->m_y,
                              win->GetMinWidth(), win->GetMinHeight(),
                              win->GetMaxWidth(), win->GetMaxHeight(),
                              flag );
    }

    /* reset the icon */
    if (win->m_icon != wxNullIcon)
    {
        wxIcon icon( win->m_icon );
        win->m_icon = wxNullIcon;
        win->SetIcon( icon );
    }

    return FALSE;
}

//-----------------------------------------------------------------------------
// InsertChild for wxDialog
//-----------------------------------------------------------------------------

/* Callback for wxFrame. This very strange beast has to be used because
 * C++ has no virtual methods in a constructor. We have to emulate a
 * virtual function here as wxWindows requires different ways to insert
 * a child in container classes. */

static void wxInsertChildInDialog( wxDialog* parent, wxWindow* child )
{
    gtk_pizza_put( GTK_PIZZA(parent->m_wxwindow),
                     GTK_WIDGET(child->m_widget),
                     child->m_x,
                     child->m_y,
                     child->m_width,
                     child->m_height );

    if (parent->HasFlag(wxTAB_TRAVERSAL))
    {
        /* we now allow a window to get the focus as long as it
           doesn't have any children. */
        GTK_WIDGET_UNSET_FLAGS( parent->m_wxwindow, GTK_CAN_FOCUS );
    }
}

//-----------------------------------------------------------------------------
// wxDialog
//-----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(wxDialog,wxPanel)
    EVT_BUTTON  (wxID_OK,       wxDialog::OnOK)
    EVT_BUTTON  (wxID_CANCEL,   wxDialog::OnCancel)
    EVT_BUTTON  (wxID_APPLY,    wxDialog::OnApply)
    EVT_SIZE    (wxDialog::OnSize)
    EVT_CLOSE   (wxDialog::OnCloseWindow)
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC_CLASS(wxDialog,wxPanel)

void wxDialog::Init()
{
    m_returnCode = 0;
    m_sizeSet = FALSE;
    m_modalShowing = FALSE;
    m_isFrame = TRUE;
}

wxDialog::wxDialog( wxWindow *parent,
                    wxWindowID id, const wxString &title,
                    const wxPoint &pos, const wxSize &size,
                    long style, const wxString &name )
{
    Init();

    Create( parent, id, title, pos, size, style, name );
}

bool wxDialog::Create( wxWindow *parent,
                       wxWindowID id, const wxString &title,
                       const wxPoint &pos, const wxSize &size,
                       long style, const wxString &name )
{
    g_openDialogs++;

    wxTopLevelWindows.Append( this );

    m_needParent = FALSE;

    if (!PreCreation( parent, pos, size ) ||
        !CreateBase( parent, id, pos, size, style, wxDefaultValidator, name ))
    {
        wxFAIL_MSG( wxT("wxDialog creation failed") );
        return FALSE;
    }

    // All dialogs should really have this style
    m_windowStyle |= wxTAB_TRAVERSAL;

    m_insertCallback = (wxInsertChildFunction) wxInsertChildInDialog;

    m_widget = gtk_window_new( GTK_WINDOW_DIALOG );

    if ((m_parent) && (GTK_IS_WINDOW(m_parent->m_widget)))
        gtk_window_set_transient_for( GTK_WINDOW(m_widget), GTK_WINDOW(m_parent->m_widget) );

    if (!name.IsEmpty())
        gtk_window_set_wmclass( GTK_WINDOW(m_widget), name.mb_str(), name.mb_str() );

    GTK_WIDGET_UNSET_FLAGS( m_widget, GTK_CAN_FOCUS );

    gtk_signal_connect( GTK_OBJECT(m_widget), "delete_event",
        GTK_SIGNAL_FUNC(gtk_dialog_delete_callback), (gpointer)this );

    m_wxwindow = gtk_pizza_new();
    gtk_widget_show( m_wxwindow );
    GTK_WIDGET_UNSET_FLAGS( m_wxwindow, GTK_CAN_FOCUS );

    gtk_container_add( GTK_CONTAINER(m_widget), m_wxwindow );

    SetTitle( title );

    if (m_parent) m_parent->AddChild( this );

    PostCreation();

    /*  we cannot set MWM hints  before the widget has
        been realized, so we do this directly after realization */
    gtk_signal_connect( GTK_OBJECT(m_widget), "realize",
                        GTK_SIGNAL_FUNC(gtk_dialog_realized_callback), (gpointer) this );

    /* the user resized the frame by dragging etc. */
    gtk_signal_connect( GTK_OBJECT(m_widget), "size_allocate",
        GTK_SIGNAL_FUNC(gtk_dialog_size_callback), (gpointer)this );

    gtk_signal_connect( GTK_OBJECT(m_widget), "configure_event",
        GTK_SIGNAL_FUNC(gtk_dialog_configure_callback), (gpointer)this );

    /* disable native tab traversal */
    gtk_signal_connect( GTK_OBJECT(m_widget), "focus",
        GTK_SIGNAL_FUNC(gtk_dialog_focus_callback), (gpointer)this );

    return TRUE;
}

wxDialog::~wxDialog()
{
    m_isBeingDeleted = TRUE;

    wxTopLevelWindows.DeleteObject( this );

    if (wxTheApp->GetTopWindow() == this)
    {
        wxTheApp->SetTopWindow( (wxWindow*) NULL );
    }

    if (wxTopLevelWindows.Number() == 0)
    {
        wxTheApp->ExitMainLoop();
    }
    
    g_openDialogs--;
}

void wxDialog::SetTitle( const wxString& title )
{
    m_title = title;
    if (m_title.IsNull()) m_title = wxT("");
    gtk_window_set_title( GTK_WINDOW(m_widget), m_title.mbc_str() );
}

wxString wxDialog::GetTitle() const
{
    return (wxString&)m_title;
}

void wxDialog::OnApply( wxCommandEvent &WXUNUSED(event) )
{
    if (Validate()) TransferDataFromWindow();
}

void wxDialog::OnCancel( wxCommandEvent &WXUNUSED(event) )
{
    if (IsModal())
    {
        EndModal(wxID_CANCEL);
    }
    else
    {
        SetReturnCode(wxID_CANCEL);
        Show(FALSE);
    }
}

void wxDialog::OnOK( wxCommandEvent &WXUNUSED(event) )
{
    if (Validate() && TransferDataFromWindow())
    {
        if (IsModal())
        {
            EndModal(wxID_OK);
        }
        else
        {
            SetReturnCode(wxID_OK);
            this->Show(FALSE);
        }
    }
}

void wxDialog::OnPaint( wxPaintEvent& WXUNUSED(event) )
{
    // yes
}

void wxDialog::OnCloseWindow(wxCloseEvent& WXUNUSED(event))
{
    // We'll send a Cancel message by default,
    // which may close the dialog.
    // Check for looping if the Cancel event handler calls Close().

    // Note that if a cancel button and handler aren't present in the dialog,
    // nothing will happen when you close the dialog via the window manager, or
    // via Close().
    // We wouldn't want to destroy the dialog by default, since the dialog may have been
    // created on the stack.
    // However, this does mean that calling dialog->Close() won't delete the dialog
    // unless the handler for wxID_CANCEL does so. So use Destroy() if you want to be
    // sure to destroy the dialog.
    // The default OnCancel (above) simply ends a modal dialog, and hides a modeless dialog.

    static wxList s_closing;

    if (s_closing.Member(this))
        return;   // no loops

    s_closing.Append(this);

    wxCommandEvent cancelEvent(wxEVT_COMMAND_BUTTON_CLICKED, wxID_CANCEL);
    cancelEvent.SetEventObject( this );
    GetEventHandler()->ProcessEvent(cancelEvent);
    s_closing.DeleteObject(this);
}

bool wxDialog::Destroy()
{
    if (!wxPendingDelete.Member(this)) wxPendingDelete.Append(this);

    return TRUE;
}

void wxDialog::OnSize( wxSizeEvent &WXUNUSED(event) )
{
    wxASSERT_MSG( (m_widget != NULL), wxT("invalid dialog") );

#if wxUSE_CONSTRAINTS
    if (GetAutoLayout())
    {
        Layout();
    }
    else
#endif // wxUSE_CONSTRAINTS
    {
        /* no child: go out ! */
        if (!GetChildren().First()) return;

        /* do we have exactly one child? */
        wxWindow *child = (wxWindow *) NULL;
        for(wxNode *node = GetChildren().First(); node; node = node->Next())
        {
            wxWindow *win = (wxWindow *)node->Data();
            if (!wxIS_KIND_OF(win,wxFrame) && !wxIS_KIND_OF(win,wxDialog))
            {
                /* it's the second one: do nothing */
                if (child) return;
                child = win;
            }
        }

        /* yes: set it's size to fill all the frame */
        int client_x, client_y;
        GetClientSize( &client_x, &client_y );
        child->SetSize( 1, 1, client_x-2, client_y);
    }
}

void wxDialog::DoSetSize( int x, int y, int width, int height, int sizeFlags )
{
    wxASSERT_MSG( (m_widget != NULL), wxT("invalid dialog") );
    wxASSERT_MSG( (m_wxwindow != NULL), wxT("invalid dialog") );

    if (m_resizing) return; /* I don't like recursions */
    m_resizing = TRUE;

    int old_x = m_x;
    int old_y = m_y;

    int old_width = m_width;
    int old_height = m_height;

    if ((sizeFlags & wxSIZE_ALLOW_MINUS_ONE) == 0)
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

/*
    if ((sizeFlags & wxSIZE_AUTO_WIDTH) == wxSIZE_AUTO_WIDTH)
    {
        if (width == -1) m_width = 80;
    }

    if ((sizeFlags & wxSIZE_AUTO_HEIGHT) == wxSIZE_AUTO_HEIGHT)
    {
       if (height == -1) m_height = 26;
    }
*/

    if ((m_minWidth != -1) && (m_width < m_minWidth)) m_width = m_minWidth;
    if ((m_minHeight != -1) && (m_height < m_minHeight)) m_height = m_minHeight;
    if ((m_maxWidth != -1) && (m_width > m_maxWidth)) m_width = m_maxWidth;
    if ((m_maxHeight != -1) && (m_height > m_maxHeight)) m_height = m_maxHeight;

    if ((m_x != -1) || (m_y != -1))
    {
        if ((m_x != old_x) || (m_y != old_y))
        {
            /* we set the position here and when showing the dialog
               for the first time in idle time */
            gtk_widget_set_uposition( m_widget, m_x, m_y );
        }
    }

    if ((m_width != old_width) || (m_height != old_height))
    {
        /* actual resizing is deferred to GtkOnSize in idle time and
           when showing the dialog */
        m_sizeSet = FALSE;
    }

    m_resizing = FALSE;
}

void wxDialog::GtkOnSize( int WXUNUSED(x), int WXUNUSED(y), int width, int height )
{
    // due to a bug in gtk, x,y are always 0
    // m_x = x;
    // m_y = y;

    if ((m_height == height) && (m_width == width) && (m_sizeSet)) return;
    if (!m_wxwindow) return;

    m_width = width;
    m_height = height;

    if ((m_minWidth != -1) && (m_width < m_minWidth)) m_width = m_minWidth;
    if ((m_minHeight != -1) && (m_height < m_minHeight)) m_height = m_minHeight;
    if ((m_maxWidth != -1) && (m_width > m_maxWidth)) m_width = m_maxWidth;
    if ((m_maxHeight != -1) && (m_height > m_maxHeight)) m_height = m_maxHeight;

    /* we actually set the size of a frame here and no-where else */
    gtk_widget_set_usize( m_widget, m_width, m_height );

    m_sizeSet = TRUE;

    wxSizeEvent event( wxSize(m_width,m_height), GetId() );
    event.SetEventObject( this );
    GetEventHandler()->ProcessEvent( event );
}

void wxDialog::OnInternalIdle()
{
    if (!m_sizeSet && GTK_WIDGET_REALIZED(m_wxwindow))
        GtkOnSize( m_x, m_y, m_width, m_height );

    wxWindow::OnInternalIdle();
}

bool wxDialog::Show( bool show )
{
    if (!show && IsModal())
    {
        EndModal( wxID_CANCEL );
    }

    if (show && !m_sizeSet)
    {
        /* by calling GtkOnSize here, we don't have to call
           either after showing the frame, which would entail
           much ugly flicker nor from within the size_allocate
           handler, because GTK 1.1.X forbids that. */

        GtkOnSize( m_x, m_y, m_width, m_height );
    }

    bool ret = wxWindow::Show( show );

    if (show) InitDialog();

    return ret;
}

bool wxDialog::IsModal() const
{
    return m_modalShowing;
}

void wxDialog::SetModal( bool WXUNUSED(flag) )
{
/*
  if (flag)
    m_windowStyle |= wxDIALOG_MODAL;
  else
    if (m_windowStyle & wxDIALOG_MODAL) m_windowStyle -= wxDIALOG_MODAL;
*/
    wxFAIL_MSG( wxT("wxDialog:SetModal obsolete now") );
}

int wxDialog::ShowModal()
{
    if (IsModal())
    {
       wxFAIL_MSG( wxT("wxDialog:ShowModal called twice") );
       return GetReturnCode();
    }

    wxBusyCursorSuspender cs; // temporarily suppress the busy cursor

    Show( TRUE );

    m_modalShowing = TRUE;

    gtk_grab_add( m_widget );
    gtk_main();
    gtk_grab_remove( m_widget );

    return GetReturnCode();
}

void wxDialog::EndModal( int retCode )
{
    SetReturnCode( retCode );

    if (!IsModal())
    {
        wxFAIL_MSG( wxT("wxDialog:EndModal called twice") );
        return;
    }

    m_modalShowing = FALSE;

    gtk_main_quit();

    Show( FALSE );
}

void wxDialog::InitDialog()
{
    wxWindow::InitDialog();
}

void wxDialog::SetIcon( const wxIcon &icon )
{
    m_icon = icon;
    if (!icon.Ok()) return;

    if (!m_widget->window) return;

    wxMask *mask = icon.GetMask();
    GdkBitmap *bm = (GdkBitmap *) NULL;
    if (mask) bm = mask->GetBitmap();

    gdk_window_set_icon( m_widget->window, (GdkWindow *) NULL, icon.GetPixmap(), bm );
}
