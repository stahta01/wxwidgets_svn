/////////////////////////////////////////////////////////////////////////////
// Name:        clipbrd.cpp
// Purpose:
// Author:      Robert Roebling
// Id:          $Id$
// Copyright:   (c) 1998 Robert Roebling
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "clipbrd.h"
#endif

#include "wx/clipbrd.h"

#if wxUSE_CLIPBOARD

#include "wx/utils.h"

#include "glib.h"
#include "gdk/gdk.h"
#include "gtk/gtk.h"

//-----------------------------------------------------------------------------
// data
//-----------------------------------------------------------------------------

wxClipboard *wxTheClipboard = (wxClipboard*) NULL;

GdkAtom  g_clipboardAtom   = 0;
GdkAtom  g_targetsAtom     = 0;

//-----------------------------------------------------------------------------
// reminder
//-----------------------------------------------------------------------------

/* The contents of a selection are returned in a GtkSelectionData
   structure. selection/target identify the request. 
   type specifies the type of the return; if length < 0, and
   the data should be ignored. This structure has object semantics -
   no fields should be modified directly, they should not be created
   directly, and pointers to them should not be stored beyond the duration of
   a callback. (If the last is changed, we'll need to add reference
   counting)

struct _GtkSelectionData
{
  GdkAtom selection;
  GdkAtom target;
  GdkAtom type;
  gint	  format;
  guchar *data;
  gint	  length;
};

*/

//-----------------------------------------------------------------------------
// "selection_received" for targets
//-----------------------------------------------------------------------------

static void
targets_selection_received( GtkWidget *WXUNUSED(widget), 
                            GtkSelectionData *selection_data, 
#if (GTK_MINOR_VERSION > 0)
                            guint32 WXUNUSED(time),
#endif
		            wxClipboard *clipboard )
{
    if (!wxTheClipboard)
    {
        clipboard->m_waiting = FALSE;
        return;
    }
  
    if (selection_data->length <= 0)
    {
        clipboard->m_waiting = FALSE;
        return;
    }
    
    /* make sure we got the data in the correct form */
    if (selection_data->type != GDK_SELECTION_TYPE_ATOM)
    {
        clipboard->m_waiting = FALSE;
        return;
    }
  
    // the atoms we received, holding a list of targets (= formats) 
    GdkAtom *atoms = (GdkAtom *)selection_data->data;

    for (unsigned int i=0; i<selection_data->length/sizeof(GdkAtom); i++)
    {
/*      char *name = gdk_atom_name (atoms[i]);
        if (name) printf( "Format available: %s.\n", name ); */
      
        if (atoms[i] == clipboard->m_targetRequested)
        {
            clipboard->m_waiting = FALSE;
            clipboard->m_formatSupported = TRUE;
            return;
        }
    }

    clipboard->m_waiting = FALSE;
    return;
}

//-----------------------------------------------------------------------------
// "selection_received" for the actual data
//-----------------------------------------------------------------------------

static void 
selection_received( GtkWidget *WXUNUSED(widget), 
                    GtkSelectionData *selection_data, 
#if (GTK_MINOR_VERSION > 0)
                    guint32 WXUNUSED(time),
#endif
		    wxClipboard *clipboard )
{
    if (!wxTheClipboard)
    {
        clipboard->m_waiting = FALSE;
        return;
    }
  
    wxDataObject *data_object = clipboard->m_receivedData;
    
    if (!data_object)
    {
        clipboard->m_waiting = FALSE;
        return;
    }
    
    if (selection_data->length <= 0)
    {
        clipboard->m_waiting = FALSE;
        return;
    }
  
    /* make sure we got the data in the correct format */
    if (data_object->GetFormat().GetAtom() != selection_data->target)
    {
        clipboard->m_waiting = FALSE;
        return;
    }
    
    /* make sure we got the data in the correct form (selection type).
       if so, copy data to target object */
    
    switch (data_object->GetFormat().GetType())
    {
        case wxDF_TEXT:
	{
            if (selection_data->type != GDK_SELECTION_TYPE_STRING)
            {
                clipboard->m_waiting = FALSE;
                return;
            }
	    
	    wxTextDataObject *text_object = (wxTextDataObject *) data_object;
	    
	    wxString text = (const char*) selection_data->data;
	    
	    text_object->SetText( text );
	    
	    break;
	}
	
	case wxDF_BITMAP:
	{
            if (selection_data->type != GDK_SELECTION_TYPE_BITMAP)
            {
                clipboard->m_waiting = FALSE;
                return;
            }
	    
	    break;
	}
	
	case wxDF_PRIVATE:
	{
            if (selection_data->type != GDK_SELECTION_TYPE_STRING)
            {
                clipboard->m_waiting = FALSE;
                return;
            }
	    
	    wxPrivateDataObject *private_object = (wxPrivateDataObject *) data_object;
	    
	    private_object->SetData( (const char*) selection_data->data, (size_t) selection_data->length );
	    
	    break;
	}
	
	default:
	{
            clipboard->m_waiting = FALSE;
	    return;
	}
    }
    
    wxTheClipboard->m_formatSupported = TRUE;
    clipboard->m_waiting = FALSE;
}

//-----------------------------------------------------------------------------
// "selection_clear"
//-----------------------------------------------------------------------------

static gint
selection_clear_clip( GtkWidget *WXUNUSED(widget), GdkEventSelection *event )
{
    if (!wxTheClipboard) return TRUE;
    
    if (event->selection == GDK_SELECTION_PRIMARY)
    {
        wxTheClipboard->m_ownsPrimarySelection = FALSE;
    }
    else
    if (event->selection == g_clipboardAtom)
    {
        wxTheClipboard->m_ownsClipboard = FALSE;
    }
    else
    {
        wxTheClipboard->m_waiting = FALSE;
        return FALSE;
    }
    
    if ((!wxTheClipboard->m_ownsPrimarySelection) &&
        (!wxTheClipboard->m_ownsClipboard))
    {
        /* the clipboard is no longer in our hands. we can the  clipboard data. */
      
        if (wxTheClipboard->m_dataBroker)
	{
	    delete wxTheClipboard->m_dataBroker;
	    wxTheClipboard->m_dataBroker = (wxDataBroker*) NULL;
	}
    }
  
    wxTheClipboard->m_waiting = FALSE;
    return TRUE;
}

//-----------------------------------------------------------------------------
// selection handler for supplying data
//-----------------------------------------------------------------------------

static void
selection_handler( GtkWidget *WXUNUSED(widget), GtkSelectionData *selection_data, gpointer WXUNUSED(data) )
{
    if (!wxTheClipboard) return;
  
    if (!wxTheClipboard->m_dataBroker) return;
  
    wxNode *node = wxTheClipboard->m_dataBroker->m_dataObjects.First();
    
    while (node)
    {
        wxDataObject *data_object = (wxDataObject *)node->Data();
    
	if (data_object->GetFormat().GetAtom() != selection_data->target)
	{
	    node = node->Next();
	    break;
	}
	
	switch (data_object->GetFormat().GetType())
	{
	    case wxDF_TEXT:
	    {
	        wxTextDataObject *text_object = (wxTextDataObject*) data_object;
	    
	        wxString text = text_object->GetText();
	    
#if wxUSE_UNICODE
		const wxWX2MBbuf s = text.mbc_str();
		int len = strlen(s);
#else // more efficient in non-Unicode
		const char *s = text.c_str();
		int len = (int) text.Length();
#endif
		
                gtk_selection_data_set( 
                    selection_data, 
		    GDK_SELECTION_TYPE_STRING, 
		    8*sizeof(gchar),
		    (unsigned char*) (const char*) s,
		    len ); 
		    
		break;
	    }
	    
	    case wxDF_BITMAP:
	    {
	        // wxBitmapDataObject *private_object = (wxBitmapDataObject*) data_object;
	    
		// how do we do that ?
		
		break;
	    }
	    
	    case wxDF_PRIVATE:
            {
	        wxPrivateDataObject *private_object = (wxPrivateDataObject*) data_object;
	    
	        if (private_object->GetSize() == 0) return;
	    
                gtk_selection_data_set( 
                    selection_data, 
		    GDK_SELECTION_TYPE_STRING, 
		    8*sizeof(gchar), 
		    (unsigned char*) private_object->GetData(), 
		    (int) private_object->GetSize() );
	    }
	    
	    default:
	      break;
        }
	
	node = node->Next();
    }
}

//-----------------------------------------------------------------------------
// wxClipboard
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxClipboard,wxObject)

wxClipboard::wxClipboard()
{
    m_open = FALSE;

    m_ownsClipboard = FALSE;
    m_ownsPrimarySelection = FALSE;

    m_dataBroker = (wxDataBroker*) NULL;
  
    m_receivedData = (wxDataObject*) NULL;

    /* we use m_targetsWidget to query what formats are available */
    
    m_targetsWidget = gtk_window_new( GTK_WINDOW_POPUP );
    gtk_widget_realize( m_targetsWidget );

    gtk_signal_connect( GTK_OBJECT(m_targetsWidget), 
                        "selection_received",
		        GTK_SIGNAL_FUNC( targets_selection_received ), 
		        (gpointer) this );
			
    /* we use m_clipboardWidget to get and to offer data */
    
    m_clipboardWidget = gtk_window_new( GTK_WINDOW_POPUP );
    gtk_widget_realize( m_clipboardWidget );

    gtk_signal_connect( GTK_OBJECT(m_clipboardWidget), 
                        "selection_received",
		        GTK_SIGNAL_FUNC( selection_received ), 
		        (gpointer) this );

    gtk_signal_connect( GTK_OBJECT(m_clipboardWidget), 
                        "selection_clear_event",
		        GTK_SIGNAL_FUNC( selection_clear_clip ), 
		        (gpointer) NULL );
		      
    if (!g_clipboardAtom) g_clipboardAtom = gdk_atom_intern( "CLIPBOARD", FALSE );
    if (!g_targetsAtom) g_targetsAtom = gdk_atom_intern ("TARGETS", FALSE);
  
    m_formatSupported = FALSE;
    m_targetRequested = 0;
}

wxClipboard::~wxClipboard()
{
    Clear();  
  
    if (m_clipboardWidget) gtk_widget_destroy( m_clipboardWidget );
    if (m_targetsWidget) gtk_widget_destroy( m_targetsWidget );
}

void wxClipboard::Clear()
{
    if (m_dataBroker)
    { 
        /*  As we have data we also own the clipboard. Once we no longer own
            it, clear_selection is called which will set m_data to zero */
     
        if (gdk_selection_owner_get( g_clipboardAtom ) == m_clipboardWidget->window)
        {
            m_waiting = TRUE;
	    
            gtk_selection_owner_set( (GtkWidget*) NULL, g_clipboardAtom, GDK_CURRENT_TIME );
	    
            while (m_waiting) gtk_main_iteration();
        }
    
        if (gdk_selection_owner_get( GDK_SELECTION_PRIMARY ) == m_clipboardWidget->window)
        {
            m_waiting = TRUE;
	    
            gtk_selection_owner_set( (GtkWidget*) NULL, GDK_SELECTION_PRIMARY, GDK_CURRENT_TIME );
	    
            while (m_waiting) gtk_main_iteration();
        }
    
        if (m_dataBroker)
	{
	    delete m_dataBroker;
	    m_dataBroker = (wxDataBroker*) NULL;
	}
    }
  
    m_targetRequested = 0;
  
    m_formatSupported = FALSE;
}

bool wxClipboard::Open()
{
    wxCHECK_MSG( !m_open, FALSE, _T("clipboard already open") );
  
    m_open = TRUE;
  
    return TRUE;
}

bool wxClipboard::SetData( wxDataObject *data )
{
    wxCHECK_MSG( m_open, FALSE, _T("clipboard not open") );
  
    wxCHECK_MSG( data, FALSE, _T("data is invalid") );
  
    Clear();

    return AddData( data );
}

bool wxClipboard::AddData( wxDataObject *data )
{
    wxCHECK_MSG( m_open, FALSE, _T("clipboard not open") );
  
    wxCHECK_MSG( data, FALSE, _T("data is invalid") );
    
    /* if clipboard has been cleared before, create new data broker */
  
    if (!m_dataBroker) m_dataBroker = new wxDataBroker();
  
    /* add new data to list of offered data objects */
  
    m_dataBroker->Add( data );
    
    /* get native format id of new data object */
    
    GdkAtom format = data->GetFormat().GetAtom();
	
    wxCHECK_MSG( format, FALSE, _T("data has invalid format") );
    
    /* This should happen automatically, but to be on the safe side */
      
    m_ownsClipboard = FALSE;
    m_ownsPrimarySelection = FALSE;
    
    /* Add handlers if someone requests data */
  

#if (GTK_MINOR_VERSION > 0)

    gtk_selection_add_target( GTK_WIDGET(m_clipboardWidget), 
                              GDK_SELECTION_PRIMARY,
			      format, 
			      0 );  /* what is info ? */
			     
    gtk_selection_add_target( GTK_WIDGET(m_clipboardWidget), 
                              g_clipboardAtom,
			      format, 
			      0 );  /* what is info ? */
			     
    gtk_signal_connect( GTK_OBJECT(m_clipboardWidget), 
                        "selection_get",
		        GTK_SIGNAL_FUNC(selection_handler), 
		        (gpointer) NULL );

#else

    gtk_selection_add_handler( m_clipboardWidget, 
                               g_clipboardAtom,
			       format,
			       selection_handler,
			       (gpointer) NULL );
			       
    gtk_selection_add_handler( m_clipboardWidget, 
                               GDK_SELECTION_PRIMARY,
			       format,
			       selection_handler,
			       (gpointer) NULL );
#endif
			       
    /* Tell the world we offer clipboard data */
  
    if (!gtk_selection_owner_set( m_clipboardWidget, 
                                  g_clipboardAtom,
				  GDK_CURRENT_TIME ))
    {
        return FALSE;
    }
    m_ownsClipboard = TRUE;
    
    if (!gtk_selection_owner_set( m_clipboardWidget, 
                                  GDK_SELECTION_PRIMARY,
				  GDK_CURRENT_TIME ))
    {  
        return FALSE;
    }
    m_ownsPrimarySelection = TRUE;
	
    return TRUE;
}

void wxClipboard::Close()
{
    wxCHECK_RET( m_open, _T("clipboard not open") );
    
    m_open = FALSE;
}

bool wxClipboard::IsSupported( wxDataFormat format )
{
    wxCHECK_MSG( m_open, FALSE, _T("clipboard not open") );
    
    /* store requested format to be asked for by callbacks */
    
    m_targetRequested = format.GetAtom();
  
    wxCHECK_MSG( m_targetRequested, FALSE, _T("invalid clipboard format") );
    
    m_formatSupported = FALSE;
  
    /* perform query. this will set m_formatSupported to 
       TRUE if m_targetRequested is supported.
       alsom we have to wait for the "answer" from the 
       clipboard owner which is an asynchronous process.
       therefore we set m_waiting = TRUE here and wait
       until the callback "targets_selection_received" 
       sets it to FALSE */

    m_waiting = TRUE;

    gtk_selection_convert( m_targetsWidget,
			   g_clipboardAtom, 
			   g_targetsAtom,
			   GDK_CURRENT_TIME );

    while (m_waiting) gtk_main_iteration();
    
    if (!m_formatSupported) return FALSE;
    
    return TRUE;
}    
    
bool wxClipboard::GetData( wxDataObject *data )
{
    wxCHECK_MSG( m_open, FALSE, _T("clipboard not open") );
    
    /* is data supported by clipboard ? */
    
    if (!IsSupported( data->GetFormat() )) return FALSE;
    
    /* store pointer to data object to be filled up by callbacks */
    
    m_receivedData = data;
    
    /* store requested format to be asked for by callbacks */
    
    m_targetRequested = data->GetFormat().GetAtom();
  
    wxCHECK_MSG( m_targetRequested, FALSE, _T("invalid clipboard format") );
    
    /* start query */
    
    m_formatSupported = FALSE;
  
    /* ask for clipboard contents.  this will set 
       m_formatSupported to TRUE if m_targetRequested 
       is supported.
       also, we have to wait for the "answer" from the 
       clipboard owner which is an asynchronous process.
       therefore we set m_waiting = TRUE here and wait
       until the callback "targets_selection_received" 
       sets it to FALSE */

    m_waiting = TRUE;

    gtk_selection_convert( m_clipboardWidget,
			   g_clipboardAtom, 
			   m_targetRequested,
			   GDK_CURRENT_TIME );
  
    while (m_waiting) gtk_main_iteration();

    /* this is a true error as we checked for the presence of such data before */

    wxCHECK_MSG( m_formatSupported, FALSE, _T("error retrieving data from clipboard") );
  
    return TRUE;
}

//-----------------------------------------------------------------------------
// wxClipboardModule
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxClipboardModule,wxModule)

bool wxClipboardModule::OnInit()
{
    wxTheClipboard = new wxClipboard();
  
    return TRUE;
}

void wxClipboardModule::OnExit()
{
    if (wxTheClipboard) delete wxTheClipboard;
    wxTheClipboard = (wxClipboard*) NULL;
}

#endif

  // wxUSE_CLIPBOARD

