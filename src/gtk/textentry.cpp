///////////////////////////////////////////////////////////////////////////////
// Name:        src/gtk/textentry.cpp
// Purpose:     wxTextEntry implementation for wxGTK
// Author:      Vadim Zeitlin
// Created:     2007-09-24
// RCS-ID:      $Id$
// Copyright:   (c) 2007 Vadim Zeitlin <vadim@wxwindows.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// for compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_TEXTCTRL || wxUSE_COMBOBOX

#ifndef WX_PRECOMP
    #include "wx/textentry.h"
    #include "wx/window.h"
    #include "wx/textctrl.h"
#endif //WX_PRECOMP

#include <gtk/gtk.h>
#include "wx/gtk/private.h"
#include "wx/gtk/private/gtk2-compat.h"

// ============================================================================
// signal handlers implementation
// ============================================================================

// "insert_text" handler for GtkEntry
extern "C"
void
wx_gtk_insert_text_callback(GtkEditable *editable,
                            const gchar * new_text,
                            gint WXUNUSED(new_text_length),
                            gint * WXUNUSED(position),
                            wxTextEntry *text)
{
    GtkEntry *entry = GTK_ENTRY (editable);

#if GTK_CHECK_VERSION(3,0,0) || defined(GSEAL_ENABLE)
    const int text_max_length = gtk_entry_buffer_get_max_length(gtk_entry_get_buffer(entry));
#else
    const int text_max_length = entry->text_max_length;
#endif

    // we should only be called if we have a max len limit at all
    wxCHECK_RET(text_max_length, "shouldn't be called");

    // check that we don't overflow the max length limit

    const int text_length = gtk_entry_get_text_length(entry);

    // We can't use new_text_length as it is in bytes while we want to count
    // characters (in first approximation, anyhow...).
    if ( text_length + g_utf8_strlen(new_text, -1) > text_max_length )
    {
        // Prevent the new text from being inserted.
        g_signal_stop_emission_by_name (editable, "insert_text");

        // Currently we don't insert anything at all, but it would be better to
        // insert as many characters as would fit into the text control and
        // only discard the rest.

        // Notify the user code about overflow.
        text->SendMaxLenEvent();
    }
}

//-----------------------------------------------------------------------------
//  clipboard events: "copy-clipboard", "cut-clipboard", "paste-clipboard"
//-----------------------------------------------------------------------------

// common part of the event handlers below
static void
DoHandleClipboardCallback( GtkWidget *widget,
                           wxWindow *win,
                           wxEventType eventType,
                           const gchar* signal_name)
{
    wxClipboardTextEvent event( eventType, win->GetId() );
    event.SetEventObject( win );
    if ( win->HandleWindowEvent( event ) )
    {
        // don't let the default processing to take place if we did something
        // ourselves in the event handler
        g_signal_stop_emission_by_name (widget, signal_name);
    }
}

extern "C"
{

static void
wx_gtk_copy_clipboard_callback( GtkWidget *widget, wxWindow *win )
{
    DoHandleClipboardCallback(
        widget, win, wxEVT_COMMAND_TEXT_COPY, "copy-clipboard" );
}

static void
wx_gtk_cut_clipboard_callback( GtkWidget *widget, wxWindow *win )
{
    DoHandleClipboardCallback(
        widget, win, wxEVT_COMMAND_TEXT_CUT, "cut-clipboard" );
}

static void
wx_gtk_paste_clipboard_callback( GtkWidget *widget, wxWindow *win )
{
    DoHandleClipboardCallback(
        widget, win, wxEVT_COMMAND_TEXT_PASTE, "paste-clipboard" );
}

} // extern "C"

// ============================================================================
// wxTextEntry implementation
// ============================================================================

// ----------------------------------------------------------------------------
// text operations
// ----------------------------------------------------------------------------

void wxTextEntry::WriteText(const wxString& value)
{
    GtkEditable * const edit = GetEditable();

    // remove the selection if there is one and suppress the text change event
    // generated by this: we only want to generate one event for this change,
    // not two
    {
        EventsSuppressor noevents(this);
        gtk_editable_delete_selection(edit);
    }

    // insert new text at the cursor position
    gint len = gtk_editable_get_position(edit);
    gtk_editable_insert_text
    (
        edit,
        wxGTK_CONV_FONT(value, GetEditableWindow()->GetFont()),
        -1,     // text: length: compute it using strlen()
        &len    // will be updated to position after the text end
    );

    // and move cursor to the end of new text
    gtk_editable_set_position(edit, len);
}

void wxTextEntry::DoSetValue(const wxString& value, int flags)
{
    if (value != DoGetValue())
    {
        // use Remove() rather than SelectAll() to avoid unnecessary clipboard
        // operations, and prevent triggering an apparent bug in GTK which
        // causes the the subsequent WriteText() to append rather than overwrite
        {
            EventsSuppressor noevents(this);
            Remove(0, -1);
        }
        EventsSuppressor noeventsIf(this, !(flags & SetValue_SendEvent));
        WriteText(value);
    }
    else if (flags & SetValue_SendEvent)
        SendTextUpdatedEvent(GetEditableWindow());

    SetInsertionPoint(0);
}

wxString wxTextEntry::DoGetValue() const
{
    const wxGtkString value(gtk_editable_get_chars(GetEditable(), 0, -1));

    return wxGTK_CONV_BACK_FONT(value,
            const_cast<wxTextEntry *>(this)->GetEditableWindow()->GetFont());
}

void wxTextEntry::Remove(long from, long to)
{
    gtk_editable_delete_text(GetEditable(), from, to);
}

// ----------------------------------------------------------------------------
// clipboard operations
// ----------------------------------------------------------------------------

void wxTextEntry::GTKConnectClipboardSignals(GtkWidget* entry)
{
    g_signal_connect(entry, "copy-clipboard",
                     G_CALLBACK (wx_gtk_copy_clipboard_callback),
                     GetEditableWindow());
    g_signal_connect(entry, "cut-clipboard",
                     G_CALLBACK (wx_gtk_cut_clipboard_callback),
                     GetEditableWindow());
    g_signal_connect(entry, "paste-clipboard",
                     G_CALLBACK (wx_gtk_paste_clipboard_callback),
                     GetEditableWindow());
}

void wxTextEntry::Copy()
{
    gtk_editable_copy_clipboard(GetEditable());
}

void wxTextEntry::Cut()
{
    gtk_editable_cut_clipboard(GetEditable());
}

void wxTextEntry::Paste()
{
    gtk_editable_paste_clipboard(GetEditable());
}

// ----------------------------------------------------------------------------
// undo/redo
// ----------------------------------------------------------------------------

void wxTextEntry::Undo()
{
    // TODO: not implemented
}

void wxTextEntry::Redo()
{
    // TODO: not implemented
}

bool wxTextEntry::CanUndo() const
{
    return false;
}

bool wxTextEntry::CanRedo() const
{
    return false;
}

// ----------------------------------------------------------------------------
// insertion point
// ----------------------------------------------------------------------------

void wxTextEntry::SetInsertionPoint(long pos)
{
    gtk_editable_set_position(GetEditable(), pos);
}

long wxTextEntry::GetInsertionPoint() const
{
    return gtk_editable_get_position(GetEditable());
}

long wxTextEntry::GetLastPosition() const
{
    // this can't be implemented for arbitrary GtkEditable so only do it for
    // GtkEntries
    long pos = -1;
    GtkEntry* entry = (GtkEntry*)GetEditable();
    if (GTK_IS_ENTRY(entry))
        pos = gtk_entry_get_text_length(entry);

    return pos;
}

// ----------------------------------------------------------------------------
// selection
// ----------------------------------------------------------------------------

void wxTextEntry::SetSelection(long from, long to)
{
    // in wx convention, (-1, -1) means the entire range but GTK+ translates -1
    // (or any negative number for that matter) into last position so we need
    // to translate manually
    if ( from == -1 && to == -1 )
        from = 0;

    // for compatibility with MSW, exchange from and to parameters so that the
    // insertion point is set to the start of the selection and not its end as
    // GTK+ does by default
    gtk_editable_select_region(GetEditable(), to, from);

#ifndef __WXGTK3__
    // avoid reported problem with RHEL 5 GTK+ 2.10 where selection is reset by
    // a clipboard callback, see #13277
    if (gtk_check_version(2,12,0))
    {
        GtkEntry* entry = GTK_ENTRY(GetEditable());
        if (to < 0)
            to = entry->text_length;
        entry->selection_bound = to;
    }
#endif
}

void wxTextEntry::GetSelection(long *from, long *to) const
{
    gint start, end;
    if ( gtk_editable_get_selection_bounds(GetEditable(), &start, &end) )
    {
        // the output must always be in order, although in GTK+ it isn't
        if ( start > end )
        {
            gint tmp = start;
            start = end;
            end = tmp;
        }
    }
    else // no selection
    {
        // for compatibility with MSW return the empty selection at cursor
        start =
        end = GetInsertionPoint();
    }

    if ( from )
        *from = start;

    if ( to )
        *to = end;
}

// ----------------------------------------------------------------------------
// auto completion
// ----------------------------------------------------------------------------

bool wxTextEntry::DoAutoCompleteStrings(const wxArrayString& choices)
{
    GtkEntry* const entry = (GtkEntry*)GetEditable();
    wxCHECK_MSG(GTK_IS_ENTRY(entry), false, "auto completion doesn't work with this control");

    GtkListStore * const store = gtk_list_store_new(1, G_TYPE_STRING);
    GtkTreeIter iter;

    for ( wxArrayString::const_iterator i = choices.begin();
          i != choices.end();
          ++i )
    {
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
                           0, (const gchar *)i->utf8_str(),
                           -1);
    }

    GtkEntryCompletion * const completion = gtk_entry_completion_new();
    gtk_entry_completion_set_model(completion, GTK_TREE_MODEL(store));
    gtk_entry_completion_set_text_column(completion, 0);
    gtk_entry_set_completion(entry, completion);
    g_object_unref(completion);
    return true;
}

// ----------------------------------------------------------------------------
// editable status
// ----------------------------------------------------------------------------

bool wxTextEntry::IsEditable() const
{
    return gtk_editable_get_editable(GetEditable()) != 0;
}

void wxTextEntry::SetEditable(bool editable)
{
    gtk_editable_set_editable(GetEditable(), editable);
}

// ----------------------------------------------------------------------------
// max text length
// ----------------------------------------------------------------------------

void wxTextEntry::SetMaxLength(unsigned long len)
{
    GtkEntry* const entry = (GtkEntry*)GetEditable();
    if (!GTK_IS_ENTRY(entry))
        return;

    gtk_entry_set_max_length(entry, len);

    // there is a bug in GTK+ 1.2.x: "changed" signal is emitted even if we had
    // tried to enter more text than allowed by max text length and the text
    // wasn't really changed
    //
    // to detect this and generate TEXT_MAXLEN event instead of TEXT_CHANGED
    // one in this case we also catch "insert_text" signal
    //
    // when max len is set to 0 we disconnect our handler as it means that we
    // shouldn't check anything any more
    if ( len )
    {
        g_signal_connect
        (
            entry,
            "insert_text",
            G_CALLBACK(wx_gtk_insert_text_callback),
            this
        );
    }
    else // no max length
    {
        g_signal_handlers_disconnect_by_func
        (
            entry,
            (gpointer)wx_gtk_insert_text_callback,
            this
        );
    }
}

void wxTextEntry::SendMaxLenEvent()
{
    // remember that the next changed signal is to be ignored to avoid
    // generating a dummy wxEVT_COMMAND_TEXT_UPDATED event
    //IgnoreNextTextUpdate();

    wxWindow * const win = GetEditableWindow();
    wxCommandEvent event(wxEVT_COMMAND_TEXT_MAXLEN, win->GetId());
    event.SetEventObject(win);
    event.SetString(GetValue());
    win->HandleWindowEvent(event);
}

// ----------------------------------------------------------------------------
// margins support
// ----------------------------------------------------------------------------

bool wxTextEntry::DoSetMargins(const wxPoint& margins)
{
#if GTK_CHECK_VERSION(2,10,0)
    GtkEntry* entry = GetEntry();

    if ( !entry )
        return false;

    const GtkBorder* oldBorder = gtk_entry_get_inner_border(entry);
    GtkBorder* newBorder;

    if ( oldBorder )
    {
        newBorder = gtk_border_copy(oldBorder);
    }
    else
    {
    #if GTK_CHECK_VERSION(2,14,0)
        newBorder = gtk_border_new();
    #else
        newBorder = g_slice_new0(GtkBorder);
    #endif
        // Use some reasonable defaults for initial margins
        newBorder->left = 2;
        newBorder->right = 2;

        // These numbers seem to let the text remain vertically centered
        // in common use scenarios when margins.y == -1.
        newBorder->top = 3;
        newBorder->bottom = 3;
    }

    if ( margins.x != -1 )
        newBorder->left = (gint) margins.x;

    if ( margins.y != -1 )
        newBorder->top = (gint) margins.y;

    gtk_entry_set_inner_border(entry, newBorder);

#if GTK_CHECK_VERSION(2,14,0)
    gtk_border_free(newBorder);
#else
    g_slice_free(GtkBorder, newBorder);
#endif

    return true;
#else
    wxUnusedVar(margins);
    return false;
#endif
}

wxPoint wxTextEntry::DoGetMargins() const
{
#if GTK_CHECK_VERSION(2,10,0)
    GtkEntry* entry = GetEntry();

    if ( !entry )
        return wxPoint(-1, -1);

    const GtkBorder* border = gtk_entry_get_inner_border(entry);

    if ( !border )
        return wxPoint(-1, -1);

    return wxPoint((wxCoord) border->left, (wxCoord) border->top);
#else
    return wxPoint(-1, -1);
#endif
}

#endif // wxUSE_TEXTCTRL || wxUSE_COMBOBOX
