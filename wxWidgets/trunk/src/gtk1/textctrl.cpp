/////////////////////////////////////////////////////////////////////////////
// Name:        textctrl.cpp
// Purpose:
// Author:      Robert Roebling
// Id:          $Id$
// Copyright:   (c) 1998 Robert Roebling, Vadim Zeitlin
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "textctrl.h"
#endif

#include "wx/textctrl.h"
#include "wx/utils.h"
#include "wx/intl.h"
#include "wx/settings.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>

#include "gdk/gdk.h"
#include "gtk/gtk.h"
#include "gdk/gdkkeysyms.h"

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
//  "changed"
//-----------------------------------------------------------------------------

static void
gtk_text_changed_callback( GtkWidget *WXUNUSED(widget), wxTextCtrl *win )
{
    if (g_isIdle) wxapp_install_idle_handler();

    if (!win->m_hasVMT) return;

    win->SetModified();

    wxCommandEvent event( wxEVT_COMMAND_TEXT_UPDATED, win->m_windowId );
    event.SetString( win->GetValue() );
    event.SetEventObject( win );
    win->GetEventHandler()->ProcessEvent( event );
}

//-----------------------------------------------------------------------------
// "changed" from vertical scrollbar
//-----------------------------------------------------------------------------

static void
gtk_scrollbar_changed_callback( GtkWidget *WXUNUSED(widget), wxTextCtrl *win )
{
    if (g_isIdle) wxapp_install_idle_handler();

    if (!win->m_hasVMT) return;
    
    win->CalculateScrollbar();
}

//-----------------------------------------------------------------------------
//  wxTextCtrl
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxTextCtrl,wxControl)

BEGIN_EVENT_TABLE(wxTextCtrl, wxControl)
    EVT_CHAR(wxTextCtrl::OnChar)

    EVT_MENU(wxID_CUT, wxTextCtrl::OnCut)
    EVT_MENU(wxID_COPY, wxTextCtrl::OnCopy)
    EVT_MENU(wxID_PASTE, wxTextCtrl::OnPaste)
    EVT_MENU(wxID_UNDO, wxTextCtrl::OnUndo)
    EVT_MENU(wxID_REDO, wxTextCtrl::OnRedo)

    EVT_UPDATE_UI(wxID_CUT, wxTextCtrl::OnUpdateCut)
    EVT_UPDATE_UI(wxID_COPY, wxTextCtrl::OnUpdateCopy)
    EVT_UPDATE_UI(wxID_PASTE, wxTextCtrl::OnUpdatePaste)
    EVT_UPDATE_UI(wxID_UNDO, wxTextCtrl::OnUpdateUndo)
    EVT_UPDATE_UI(wxID_REDO, wxTextCtrl::OnUpdateRedo)
END_EVENT_TABLE()

#ifndef NO_TEXT_WINDOW_STREAM
wxTextCtrl::wxTextCtrl() : streambuf()
{
    if (allocate()) setp(base(),ebuf());

    m_modified = FALSE;
}
#else
wxTextCtrl::wxTextCtrl()
{
    m_modified = FALSE;
}
#endif

#ifndef NO_TEXT_WINDOW_STREAM
wxTextCtrl::wxTextCtrl( wxWindow *parent, wxWindowID id, const wxString &value,
      const wxPoint &pos, const wxSize &size,
      int style, const wxValidator& validator, const wxString &name ) : streambuf()
{
    if (allocate()) setp(base(),ebuf());

    m_modified = FALSE;
    Create( parent, id, value, pos, size, style, validator, name );
}
#else
wxTextCtrl::wxTextCtrl( wxWindow *parent, wxWindowID id, const wxString &value,
      const wxPoint &pos, const wxSize &size,
      int style, const wxValidator& validator, const wxString &name )
{
    m_modified = FALSE;
    Create( parent, id, value, pos, size, style, validator, name );
}
#endif

bool wxTextCtrl::Create( wxWindow *parent, wxWindowID id, const wxString &value,
      const wxPoint &pos, const wxSize &size,
      int style, const wxValidator& validator, const wxString &name )
{
    m_needParent = TRUE;
    m_acceptsFocus = TRUE;

    PreCreation( parent, id, pos, size, style, name );

    SetValidator( validator );

    m_vScrollbarVisible = FALSE;

    bool multi_line = (style & wxTE_MULTILINE) != 0;
    if ( multi_line )
    {
        /* a multi-line edit control: create a vertical scrollbar by default and
           horizontal if requested */
        bool bHasHScrollbar = (style & wxHSCROLL) != 0;

        /* create our control ... */
        m_text = gtk_text_new( (GtkAdjustment *) NULL, (GtkAdjustment *) NULL );

        /* ... and put into the upper left hand corner of the table */
        m_widget = gtk_table_new(bHasHScrollbar ? 2 : 1, 2, FALSE);
        GTK_WIDGET_UNSET_FLAGS( m_widget, GTK_CAN_FOCUS );
	
        gtk_table_attach( GTK_TABLE(m_widget), m_text, 0, 1, 0, 1,
                      (GtkAttachOptions)(GTK_FILL | GTK_EXPAND | GTK_SHRINK),
                      (GtkAttachOptions)(GTK_FILL | GTK_EXPAND | GTK_SHRINK),
                       0, 0);

        /* put the horizontal scrollbar in the lower left hand corner */
        if (bHasHScrollbar)
        {
            GtkWidget *hscrollbar = gtk_hscrollbar_new(GTK_TEXT(m_text)->hadj);
            GTK_WIDGET_UNSET_FLAGS( hscrollbar, GTK_CAN_FOCUS );
	    
            gtk_table_attach(GTK_TABLE(m_widget), hscrollbar, 0, 1, 1, 2,
                       (GtkAttachOptions)(GTK_EXPAND | GTK_FILL),
                       GTK_FILL,
                       0, 0);
            gtk_widget_show(hscrollbar);
        }

        /* we create the vertical scrollbar on demand */
	m_vScrollbar = (GtkWidget*) NULL;

    }
    else
    {
        /* a single-line text control: no need for scrollbars */
        m_widget =
          m_text = gtk_entry_new();
    }

    wxSize newSize = size;
    if (newSize.x == -1) newSize.x = 80;
    if (newSize.y == -1) newSize.y = 26;
    SetSize( newSize.x, newSize.y );

    m_parent->AddChild( this );

    (m_parent->m_insertCallback)( m_parent, this );

    PostCreation();

    if (multi_line)
    {
        gtk_widget_realize(m_text);
        gtk_widget_show(m_text);
    }

    /* we want to be notified about text changes */
    gtk_signal_connect( GTK_OBJECT(m_text), "changed",
      GTK_SIGNAL_FUNC(gtk_text_changed_callback), (gpointer)this);

    if (!value.IsEmpty())
    {
        gint tmp = 0;
#if wxUSE_UNICODE
	wxWX2MBbuf val = value.mbc_str();
        gtk_editable_insert_text( GTK_EDITABLE(m_text), val, strlen(val), &tmp );
#else
        gtk_editable_insert_text( GTK_EDITABLE(m_text), value, value.Length(), &tmp );
#endif
	
        if (multi_line)
        {
	    /* bring editable's cursor uptodate. bug in GTK. */
	
	    GTK_EDITABLE(m_text)->current_pos = gtk_text_get_point( GTK_TEXT(m_text) );
	}
    }

    if (style & wxTE_PASSWORD)
    {
        if (!multi_line)
            gtk_entry_set_visibility( GTK_ENTRY(m_text), FALSE );
    }

    if (style & wxTE_READONLY)
    {
        if (!multi_line)
            gtk_entry_set_editable( GTK_ENTRY(m_text), FALSE );
    }
    else
    {
        if (multi_line)
            gtk_text_set_editable( GTK_TEXT(m_text), 1 );
    }
    
    SetBackgroundColour( parent->GetBackgroundColour() );
    SetForegroundColour( parent->GetForegroundColour() );

    Show( TRUE );

    if (multi_line)
    {
        gtk_signal_connect(GTK_OBJECT(GTK_TEXT(m_text)->vadj), "changed",
          (GtkSignalFunc) gtk_scrollbar_changed_callback, (gpointer) this );
    }

    return TRUE;
}

void wxTextCtrl::CalculateScrollbar()
{
    if ((m_windowStyle & wxTE_MULTILINE) == 0) return;

    GtkAdjustment *adj = GTK_TEXT(m_text)->vadj;

    if (adj->upper - adj->page_size < 0.8)
    {
        if (m_vScrollbarVisible)
        {
            gtk_widget_hide( m_vScrollbar );

            m_vScrollbarVisible = FALSE;
        }
    }
    else
    {
        if (!m_vScrollbarVisible)
        {
	    if (!m_vScrollbar)
	    {
                /* finally, put the vertical scrollbar in the upper right corner */
                m_vScrollbar = gtk_vscrollbar_new( GTK_TEXT(m_text)->vadj );
                GTK_WIDGET_UNSET_FLAGS( m_vScrollbar, GTK_CAN_FOCUS );
	
                gtk_table_attach(GTK_TABLE(m_widget), m_vScrollbar, 1, 2, 0, 1,
                     GTK_FILL,
                     (GtkAttachOptions)(GTK_EXPAND | GTK_FILL | GTK_SHRINK),
                     0, 0);
	    }
	    
            gtk_widget_show( m_vScrollbar );

            m_vScrollbarVisible = TRUE;
        }
    }
}

wxString wxTextCtrl::GetValue() const
{
    wxCHECK_MSG( m_text != NULL, _T(""), _T("invalid text ctrl") );

    wxString tmp;
    if (m_windowStyle & wxTE_MULTILINE)
    {
        gint len = gtk_text_get_length( GTK_TEXT(m_text) );
        char *text = gtk_editable_get_chars( GTK_EDITABLE(m_text), 0, len );
        tmp = wxString(text,*wxConv_current);
        g_free( text );
    }
    else
    {
        tmp = wxString(gtk_entry_get_text( GTK_ENTRY(m_text) ),*wxConv_current);
    }
    return tmp;
}

void wxTextCtrl::SetValue( const wxString &value )
{
    wxCHECK_RET( m_text != NULL, _T("invalid text ctrl") );

    wxString tmp = _T("");
    if (!value.IsNull()) tmp = value;
    if (m_windowStyle & wxTE_MULTILINE)
    {
        gint len = gtk_text_get_length( GTK_TEXT(m_text) );
        gtk_editable_delete_text( GTK_EDITABLE(m_text), 0, len );
        len = 0;
#if wxUSE_UNICODE
	wxWX2MBbuf tmpbuf = tmp.mbc_str();
        gtk_editable_insert_text( GTK_EDITABLE(m_text), tmpbuf, strlen(tmpbuf), &len );
#else
        gtk_editable_insert_text( GTK_EDITABLE(m_text), tmp.mbc_str(), tmp.Length(), &len );
#endif
    }
    else
    {
        gtk_entry_set_text( GTK_ENTRY(m_text), tmp.mbc_str() );
    }
}

void wxTextCtrl::WriteText( const wxString &text )
{
    wxCHECK_RET( m_text != NULL, _T("invalid text ctrl") );

    if (text.IsNull()) return;

    if (m_windowStyle & wxTE_MULTILINE)
    {
        /* this moves the cursor pos to behind the inserted text */
	gint len = GTK_EDITABLE(m_text)->current_pos;
	
#if wxUSE_UNICODE
	wxWX2MBbuf buf = text.mbc_str();
        gtk_editable_insert_text( GTK_EDITABLE(m_text), buf, strlen(buf), &len );
#else
        gtk_editable_insert_text( GTK_EDITABLE(m_text), text, text.Length(), &len );
#endif
	
	/* bring editable's cursor uptodate. bug in GTK. */
	GTK_EDITABLE(m_text)->current_pos = gtk_text_get_point( GTK_TEXT(m_text) );
    }
    else
    {
        /* this moves the cursor pos to behind the inserted text */
	gint len = GTK_EDITABLE(m_text)->current_pos;
#if wxUSE_UNICODE
	wxWX2MBbuf buf = text.mbc_str();
        gtk_editable_insert_text( GTK_EDITABLE(m_text), buf, strlen(buf), &len );
#else
        gtk_editable_insert_text( GTK_EDITABLE(m_text), text, text.Length(), &len );
#endif
	
	/* bring editable's cursor uptodate. bug in GTK. */
	GTK_EDITABLE(m_text)->current_pos += text.Len();
	
	/* bring entry's cursor uptodate. bug in GTK. */
	gtk_entry_set_position( GTK_ENTRY(m_text), GTK_EDITABLE(m_text)->current_pos );
    }
}

void wxTextCtrl::AppendText( const wxString &text )
{
    wxCHECK_RET( m_text != NULL, _T("invalid text ctrl") );

    if (m_windowStyle & wxTE_MULTILINE)
    {
        /* we'll insert at the last position */
        gint len = gtk_text_get_length( GTK_TEXT(m_text) );
#if wxUSE_UNICODE
	wxWX2MBbuf buf = text.mbc_str();
        gtk_editable_insert_text( GTK_EDITABLE(m_text), buf, strlen(buf), &len );
#else
        gtk_editable_insert_text( GTK_EDITABLE(m_text), text, text.Length(), &len );
#endif
	
	/* bring editable's cursor uptodate. bug in GTK. */
	GTK_EDITABLE(m_text)->current_pos = gtk_text_get_point( GTK_TEXT(m_text) );
    }
    else
    {
        gtk_entry_append_text( GTK_ENTRY(m_text), text.mbc_str() );
    }
}

bool wxTextCtrl::LoadFile( const wxString &file )
{
  wxCHECK_MSG( m_text != NULL, FALSE, _T("invalid text ctrl") );

  if (!wxFileExists(file)) return FALSE;

  Clear();

  FILE *fp = (FILE*) NULL;
  struct stat statb;

  if ((stat (FNSTRINGCAST file.fn_str(), &statb) == -1) || (statb.st_mode & S_IFMT) != S_IFREG ||
      !(fp = fopen (FNSTRINGCAST file.fn_str(), "r")))
  {
      return FALSE;
  }
  else
  {
    gint len = statb.st_size;
    char *text;
    if (!(text = (char*)malloc ((unsigned) (len + 1))))
    {
      fclose (fp);
      return FALSE;
    }
    if (fread (text, sizeof (char), len, fp) != (size_t) len)
    {
    }
    fclose (fp);

    text[len] = 0;

    if (m_windowStyle & wxTE_MULTILINE)
    {
      gint pos = 0;
      gtk_editable_insert_text( GTK_EDITABLE(m_text), text, len, &pos );
    }
    else
    {
      gtk_entry_set_text( GTK_ENTRY(m_text), text );
    }

    free (text);
    m_modified = FALSE;
    return TRUE;
  }
  return FALSE;
}

bool wxTextCtrl::SaveFile( const wxString &file )
{
  wxCHECK_MSG( m_text != NULL, FALSE, _T("invalid text ctrl") );

  if (file == _T("")) return FALSE;

  FILE *fp;

  if (!(fp = fopen (FNSTRINGCAST file.fn_str(), "w")))
    {
      return FALSE;
    }
  else
    {
      char *text = (char*) NULL;
      gint len = 0;

      if (m_windowStyle & wxTE_MULTILINE)
      {
        len = gtk_text_get_length( GTK_TEXT(m_text) );
        text = gtk_editable_get_chars( GTK_EDITABLE(m_text), 0, len );
      }
      else
      {
        text = gtk_entry_get_text( GTK_ENTRY(m_text) );
      }

      if (fwrite (text, sizeof (char), len, fp) != (size_t) len)
      {
          // Did not write whole file
      }

      // Make sure newline terminates the file
      if (text[len - 1] != '\n')
          fputc ('\n', fp);

      fclose (fp);

      if (m_windowStyle & wxTE_MULTILINE) g_free( text );

      m_modified = FALSE;
      return TRUE;
    }

  return TRUE;
}

wxString wxTextCtrl::GetLineText( long lineNo ) const
{
  if (m_windowStyle & wxTE_MULTILINE)
  {
    gint len = gtk_text_get_length( GTK_TEXT(m_text) );
    char *text = gtk_editable_get_chars( GTK_EDITABLE(m_text), 0, len );

    if (text)
    {
        wxString buf(_T(""));
        long i;
        int currentLine = 0;
        for (i = 0; currentLine != lineNo && text[i]; i++ )
          if (text[i] == '\n')
            currentLine++;
        // Now get the text
        int j;
        for (j = 0; text[i] && text[i] != '\n'; i++, j++ )
            buf += text[i];

        g_free( text );
        return buf;
    }
    else
      return wxEmptyString;
  }
  else
  {
    if (lineNo == 0) return GetValue();
    return wxEmptyString;
  }
}

void wxTextCtrl::OnDropFiles( wxDropFilesEvent &WXUNUSED(event) )
{
  /* If you implement this, don't forget to update the documentation!
   * (file docs/latex/wx/text.tex) */
    wxFAIL_MSG( _T("wxTextCtrl::OnDropFiles not implemented") );
}

long wxTextCtrl::PositionToXY(long pos, long *x, long *y ) const
{
    if ( m_windowStyle & wxTE_MULTILINE )
    {
        wxString text = GetValue();

        // cast to prevent warning. But pos really should've been unsigned.
        if( (unsigned long)pos > text.Len()  )
            return FALSE;

        *x=0;   // First Col
        *y=0;   // First Line

        const wxChar* stop = text.c_str() + pos;
        for ( const wxChar *p = text.c_str(); p < stop; p++ )
        {
            if (*p == _T('\n'))
            {
                (*y)++;
                *x=0;
            }
            else
                (*x)++;
        }
    }
    else // single line control
    {
        if ( pos <= GTK_ENTRY(m_text)->text_length )
        {
            *y = 0;
            *x = pos;
        }
        else
        {
            // index out of bounds
            return FALSE;
        }
    }

    return TRUE;
}

long wxTextCtrl::XYToPosition(long x, long y ) const
{
    if (!(m_windowStyle & wxTE_MULTILINE)) return 0;

    long pos=0;
    for( int i=0; i<y; i++ ) pos += GetLineLength(i) + 1; // one for '\n'

    pos += x; 
    return pos;
}

int wxTextCtrl::GetLineLength(long lineNo) const
{
    wxString str = GetLineText (lineNo);
    return (int) str.Length();
}

int wxTextCtrl::GetNumberOfLines() const
{
    if (m_windowStyle & wxTE_MULTILINE)
    {
        gint len = gtk_text_get_length( GTK_TEXT(m_text) );
        char *text = gtk_editable_get_chars( GTK_EDITABLE(m_text), 0, len );

        if (text)
        {
            int currentLine = 0;
            for (int i = 0; i < len; i++ )
            {
                if (text[i] == '\n')
                    currentLine++;
            }
            g_free( text );

            // currentLine is 0 based, add 1 to get number of lines
            return currentLine + 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 1;
    }
}

void wxTextCtrl::SetInsertionPoint( long pos )
{
    wxCHECK_RET( m_text != NULL, _T("invalid text ctrl") );
    
    if (m_windowStyle & wxTE_MULTILINE) 
    {
        /* seems to be broken in GTK 1.0.X:
	   gtk_text_set_point( GTK_TEXT(m_text), (int)pos ); */
      
        gtk_signal_disconnect_by_func( GTK_OBJECT(m_text),
          GTK_SIGNAL_FUNC(gtk_text_changed_callback), (gpointer)this);
      
        /* we fake a set_point by inserting and deleting. as the user
	   isn't supposed to get to know about thos non-sense, we
	   disconnect so that no events are sent to the user program. */
      
        gint tmp = (gint)pos;
        gtk_editable_insert_text( GTK_EDITABLE(m_text), " ", 1, &tmp );
	gtk_editable_delete_text( GTK_EDITABLE(m_text), tmp-1, tmp );
	
        gtk_signal_connect( GTK_OBJECT(m_text), "changed",
          GTK_SIGNAL_FUNC(gtk_text_changed_callback), (gpointer)this);
	  
	/* bring editable's cursor uptodate. another bug in GTK. */
	
	GTK_EDITABLE(m_text)->current_pos = gtk_text_get_point( GTK_TEXT(m_text) );
    }
    else
    {
        gtk_entry_set_position( GTK_ENTRY(m_text), (int)pos );
	
	/* bring editable's cursor uptodate. bug in GTK. */
	
	GTK_EDITABLE(m_text)->current_pos = pos;
    }
}

void wxTextCtrl::SetInsertionPointEnd()
{
    wxCHECK_RET( m_text != NULL, _T("invalid text ctrl") );

    if (m_windowStyle & wxTE_MULTILINE)
        SetInsertionPoint(gtk_text_get_length(GTK_TEXT(m_text)));
    else
        gtk_entry_set_position( GTK_ENTRY(m_text), -1 );
}

void wxTextCtrl::SetEditable( bool editable )
{
    wxCHECK_RET( m_text != NULL, _T("invalid text ctrl") );

    if (m_windowStyle & wxTE_MULTILINE)
        gtk_text_set_editable( GTK_TEXT(m_text), editable );
    else
        gtk_entry_set_editable( GTK_ENTRY(m_text), editable );
}

void wxTextCtrl::SetSelection( long from, long to )
{
    wxCHECK_RET( m_text != NULL, _T("invalid text ctrl") );

    gtk_editable_select_region( GTK_EDITABLE(m_text), (gint)from, (gint)to );
}

void wxTextCtrl::ShowPosition( long WXUNUSED(pos) )
{
    wxFAIL_MSG( _T("wxTextCtrl::ShowPosition not implemented") );
}

long wxTextCtrl::GetInsertionPoint() const
{
    wxCHECK_MSG( m_text != NULL, 0, _T("invalid text ctrl") );

    return (long) GTK_EDITABLE(m_text)->current_pos;
}

long wxTextCtrl::GetLastPosition() const
{
    wxCHECK_MSG( m_text != NULL, 0, _T("invalid text ctrl") );

    int pos = 0;
    if (m_windowStyle & wxTE_MULTILINE)
        pos = gtk_text_get_length( GTK_TEXT(m_text) );
    else
        pos = GTK_ENTRY(m_text)->text_length;

    return (long)pos;
}

void wxTextCtrl::Remove( long from, long to )
{
    wxCHECK_RET( m_text != NULL, _T("invalid text ctrl") );

    gtk_editable_delete_text( GTK_EDITABLE(m_text), (gint)from, (gint)to );
}

void wxTextCtrl::Replace( long from, long to, const wxString &value )
{
    wxCHECK_RET( m_text != NULL, _T("invalid text ctrl") );

    gtk_editable_delete_text( GTK_EDITABLE(m_text), (gint)from, (gint)to );
    if (value.IsNull()) return;
    gint pos = (gint)from;
#if wxUSE_UNICODE
    wxWX2MBbuf buf = value.mbc_str();
    gtk_editable_insert_text( GTK_EDITABLE(m_text), buf, strlen(buf), &pos );
#else
    gtk_editable_insert_text( GTK_EDITABLE(m_text), value, value.Length(), &pos );
#endif
}

void wxTextCtrl::Cut()
{
    wxCHECK_RET( m_text != NULL, _T("invalid text ctrl") );

#if (GTK_MINOR_VERSION > 0)
    gtk_editable_cut_clipboard( GTK_EDITABLE(m_text) );
#else
    gtk_editable_cut_clipboard( GTK_EDITABLE(m_text), 0 );
#endif
}

void wxTextCtrl::Copy()
{
    wxCHECK_RET( m_text != NULL, _T("invalid text ctrl") );

#if (GTK_MINOR_VERSION > 0)
    gtk_editable_copy_clipboard( GTK_EDITABLE(m_text) );
#else
    gtk_editable_copy_clipboard( GTK_EDITABLE(m_text), 0 );
#endif
}

void wxTextCtrl::Paste()
{
    wxCHECK_RET( m_text != NULL, _T("invalid text ctrl") );

#if (GTK_MINOR_VERSION > 0)
    gtk_editable_paste_clipboard( GTK_EDITABLE(m_text) );
#else
    gtk_editable_paste_clipboard( GTK_EDITABLE(m_text), 0 );
#endif
}

bool wxTextCtrl::CanCopy() const
{
    // Can copy if there's a selection
    long from, to;
    GetSelection(& from, & to);
    return (from != to) ;
}

bool wxTextCtrl::CanCut() const
{
    // Can cut if there's a selection
    long from, to;
    GetSelection(& from, & to);
    return (from != to) ;
}

bool wxTextCtrl::CanPaste() const
{
    return IsEditable() ;
}

// Undo/redo
void wxTextCtrl::Undo()
{
    // TODO
    wxFAIL_MSG( _T("wxTextCtrl::Undo not implemented") );
}

void wxTextCtrl::Redo()
{
    // TODO
    wxFAIL_MSG( _T("wxTextCtrl::Redo not implemented") );
}

bool wxTextCtrl::CanUndo() const
{
    // TODO
    wxFAIL_MSG( _T("wxTextCtrl::CanUndo not implemented") );
    return FALSE;
}

bool wxTextCtrl::CanRedo() const
{
    // TODO
    wxFAIL_MSG( _T("wxTextCtrl::CanRedo not implemented") );
    return FALSE;
}

// If the return values from and to are the same, there is no
// selection.
void wxTextCtrl::GetSelection(long* from, long* to) const
{
    // TODO
    *from = 0;
    *to = 0;
    wxFAIL_MSG( _T("wxTextCtrl::GetSelection not implemented") );
}

bool wxTextCtrl::IsEditable() const
{
    // TODO
    wxFAIL_MSG( _T("wxTextCtrl::IsEditable not implemented") );
    return FALSE;
}

void wxTextCtrl::Clear()
{
    SetValue( _T("") );
}

void wxTextCtrl::OnChar( wxKeyEvent &key_event )
{
    wxCHECK_RET( m_text != NULL, _T("invalid text ctrl") );

    if ((key_event.KeyCode() == WXK_RETURN) && (m_windowStyle & wxPROCESS_ENTER))
    {
        wxCommandEvent event(wxEVT_COMMAND_TEXT_ENTER, m_windowId);
        event.SetEventObject(this);
        if (GetEventHandler()->ProcessEvent(event)) return;
    }

    key_event.Skip();
}

#ifndef NO_TEXT_WINDOW_STREAM
int wxTextCtrl::overflow( int WXUNUSED(c) )
{
    int len = pptr() - pbase();
    char *txt = new char[len+1];
    strncpy(txt, pbase(), len);
    txt[len] = '\0';
    (*this) << txt;
    setp(pbase(), epptr());
    delete[] txt;
    return EOF;
}

int wxTextCtrl::sync()
{
    int len = pptr() - pbase();
    char *txt = new char[len+1];
    strncpy(txt, pbase(), len);
    txt[len] = '\0';
    (*this) << txt;
    setp(pbase(), epptr());
    delete[] txt;
    return 0;
}

int wxTextCtrl::underflow()
{
    return EOF;
}

wxTextCtrl& wxTextCtrl::operator<<(const wxString& s)
{
    AppendText(s);
    return *this;
}

wxTextCtrl& wxTextCtrl::operator<<(float f)
{
    static char buf[100];
    sprintf(buf, "%.2f", f);
    AppendText(buf);
    return *this;
}

wxTextCtrl& wxTextCtrl::operator<<(double d)
{
    static char buf[100];
    sprintf(buf, "%.2f", d);
    AppendText(buf);
    return *this;
}

wxTextCtrl& wxTextCtrl::operator<<(int i)
{
    static char buf[100];
    sprintf(buf, "%i", i);
    AppendText(buf);
    return *this;
}

wxTextCtrl& wxTextCtrl::operator<<(long i)
{
    static char buf[100];
    sprintf(buf, "%ld", i);
    AppendText(buf);
    return *this;
}

wxTextCtrl& wxTextCtrl::operator<<(const char c)
{
    char buf[2];

    buf[0] = c;
    buf[1] = 0;
    AppendText(buf);
    return *this;
}
#endif

GtkWidget* wxTextCtrl::GetConnectWidget()
{
    return GTK_WIDGET(m_text);
}

bool wxTextCtrl::IsOwnGtkWindow( GdkWindow *window )
{
    if (m_windowStyle & wxTE_MULTILINE)
        return (window == GTK_TEXT(m_text)->text_area);
    else
        return (window == GTK_ENTRY(m_text)->text_area);
}

void wxTextCtrl::SetFont( const wxFont &WXUNUSED(font) )
{
    wxCHECK_RET( m_text != NULL, _T("invalid text ctrl") );

    // doesn't work
}

void wxTextCtrl::SetForegroundColour( const wxColour &WXUNUSED(colour) )
{
    wxCHECK_RET( m_text != NULL, _T("invalid text ctrl") );

    // doesn't work
}

void wxTextCtrl::SetBackgroundColour( const wxColour &colour )
{
    wxCHECK_RET( m_text != NULL, _T("invalid text ctrl") );

    wxControl::SetBackgroundColour( colour );

    wxColour sysbg = wxSystemSettings::GetSystemColour( wxSYS_COLOUR_BTNFACE );
    if (sysbg.Red() == colour.Red() &&
        sysbg.Green() == colour.Green() &&
        sysbg.Blue() == colour.Blue())
    {
        return;
    }

    if (!m_backgroundColour.Ok()) return;

    if (m_windowStyle & wxTE_MULTILINE)
    {
        GdkWindow *window = GTK_TEXT(m_text)->text_area;
        m_backgroundColour.CalcPixel( gdk_window_get_colormap( window ) );
        gdk_window_set_background( window, m_backgroundColour.GetColor() );
        gdk_window_clear( window );
    }
}

void wxTextCtrl::ApplyWidgetStyle()
{
    if (m_windowStyle & wxTE_MULTILINE)
    {
        // how ?
    }
    else
    {
        SetWidgetStyle();
        gtk_widget_set_style( m_text, m_widgetStyle );
    }
}

void wxTextCtrl::OnCut(wxCommandEvent& WXUNUSED(event))
{
    Cut();
}

void wxTextCtrl::OnCopy(wxCommandEvent& WXUNUSED(event))
{
    Copy();
}

void wxTextCtrl::OnPaste(wxCommandEvent& WXUNUSED(event))
{
    Paste();
}

void wxTextCtrl::OnUndo(wxCommandEvent& WXUNUSED(event))
{
    Undo();
}

void wxTextCtrl::OnRedo(wxCommandEvent& WXUNUSED(event))
{
    Redo();
}

void wxTextCtrl::OnUpdateCut(wxUpdateUIEvent& event)
{
    event.Enable( CanCut() );
}

void wxTextCtrl::OnUpdateCopy(wxUpdateUIEvent& event)
{
    event.Enable( CanCopy() );
}

void wxTextCtrl::OnUpdatePaste(wxUpdateUIEvent& event)
{
    event.Enable( CanPaste() );
}

void wxTextCtrl::OnUpdateUndo(wxUpdateUIEvent& event)
{
    event.Enable( CanUndo() );
}

void wxTextCtrl::OnUpdateRedo(wxUpdateUIEvent& event)
{
    event.Enable( CanRedo() );
}
