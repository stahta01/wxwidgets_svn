/////////////////////////////////////////////////////////////////////////////
// Name:        src/gtk/combobox.cpp
// Purpose:
// Author:      Robert Roebling
// Id:          $Id$
// Copyright:   (c) 1998 Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#if wxUSE_COMBOBOX

#include "wx/combobox.h"

#ifndef WX_PRECOMP
    #include "wx/intl.h"
    #include "wx/settings.h"
    #include "wx/textctrl.h"    // for wxEVT_COMMAND_TEXT_UPDATED
    #include "wx/arrstr.h"
#endif

#include "wx/gtk/private.h"

// ----------------------------------------------------------------------------
// GTK callbacks
// ----------------------------------------------------------------------------

extern "C" {
static void
gtkcombobox_text_changed_callback( GtkWidget *WXUNUSED(widget), wxComboBox *combo )
{
    if (!combo->m_hasVMT) return;

    wxCommandEvent event( wxEVT_COMMAND_TEXT_UPDATED, combo->GetId() );
    event.SetString( combo->GetValue() );
    event.SetEventObject( combo );
    combo->HandleWindowEvent( event );
}

static void
gtkcombobox_changed_callback( GtkWidget *WXUNUSED(widget), wxComboBox *combo )
{
    combo->SendSelectionChangedEvent(wxEVT_COMMAND_COMBOBOX_SELECTED);
}
}

//-----------------------------------------------------------------------------
// wxComboBox
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxComboBox, wxChoice)

BEGIN_EVENT_TABLE(wxComboBox, wxChoice)
    EVT_CHAR(wxComboBox::OnChar)

    EVT_MENU(wxID_CUT, wxComboBox::OnCut)
    EVT_MENU(wxID_COPY, wxComboBox::OnCopy)
    EVT_MENU(wxID_PASTE, wxComboBox::OnPaste)
    EVT_MENU(wxID_UNDO, wxComboBox::OnUndo)
    EVT_MENU(wxID_REDO, wxComboBox::OnRedo)
    EVT_MENU(wxID_CLEAR, wxComboBox::OnDelete)
    EVT_MENU(wxID_SELECTALL, wxComboBox::OnSelectAll)

    EVT_UPDATE_UI(wxID_CUT, wxComboBox::OnUpdateCut)
    EVT_UPDATE_UI(wxID_COPY, wxComboBox::OnUpdateCopy)
    EVT_UPDATE_UI(wxID_PASTE, wxComboBox::OnUpdatePaste)
    EVT_UPDATE_UI(wxID_UNDO, wxComboBox::OnUpdateUndo)
    EVT_UPDATE_UI(wxID_REDO, wxComboBox::OnUpdateRedo)
    EVT_UPDATE_UI(wxID_CLEAR, wxComboBox::OnUpdateDelete)
    EVT_UPDATE_UI(wxID_SELECTALL, wxComboBox::OnUpdateSelectAll)
END_EVENT_TABLE()

bool wxComboBox::Create( wxWindow *parent, wxWindowID id,
                         const wxString& value,
                         const wxPoint& pos, const wxSize& size,
                         const wxArrayString& choices,
                         long style, const wxValidator& validator,
                         const wxString& name )
{
    wxCArrayString chs(choices);

    return Create( parent, id, value, pos, size, chs.GetCount(),
                   chs.GetStrings(), style, validator, name );
}

bool wxComboBox::Create( wxWindow *parent, wxWindowID id, const wxString& value,
                         const wxPoint& pos, const wxSize& size,
                         int n, const wxString choices[],
                         long style, const wxValidator& validator,
                         const wxString& name )
{
    m_strings = NULL;

    if (!PreCreation( parent, pos, size ) ||
        !CreateBase( parent, id, pos, size, style, validator, name ))
    {
        wxFAIL_MSG( wxT("wxComboBox creation failed") );
        return false;
    }

    if (HasFlag(wxCB_SORT))
        m_strings = new wxSortedArrayString();

    m_widget = gtk_combo_box_entry_new_text();

    // Set it up to trigger default item on enter key press
    GtkWidget *widget = gtk_bin_get_child(GTK_BIN(m_widget));
    gtk_entry_set_activates_default(GTK_ENTRY(widget),
                                    !HasFlag(wxTE_PROCESS_ENTER));

    if (HasFlag(wxBORDER_NONE))
    {
        // Doesn't seem to work
        // g_object_set (m_widget, "has-frame", FALSE, NULL);
    }

    GtkEntry * const entry = GetEntry();

    gtk_entry_set_editable( entry, TRUE );

    Append(n, choices);

    m_parent->DoAddChild( this );

    m_focusWidget = GTK_WIDGET( entry );

    PostCreation(size);

    gtk_entry_set_text( entry, wxGTK_CONV(value) );

    if (style & wxCB_READONLY)
        gtk_entry_set_editable( entry, FALSE );

    g_signal_connect_after (entry, "changed",
                        G_CALLBACK (gtkcombobox_text_changed_callback), this);

    g_signal_connect_after (m_widget, "changed",
                        G_CALLBACK (gtkcombobox_changed_callback), this);

    SetInitialSize(size); // need this too because this is a wxControlWithItems

    return true;
}

GtkEntry *wxComboBox::GetEntry() const
{
    return GTK_ENTRY(GTK_BIN(m_widget)->child);
}

GtkEditable *wxComboBox::GetEditable() const
{
    return GTK_EDITABLE( GTK_BIN(m_widget)->child );
}

void wxComboBox::OnChar( wxKeyEvent &event )
{
    switch ( event.GetKeyCode() )
    {
        case WXK_RETURN:
            if ( HasFlag(wxTE_PROCESS_ENTER) )
            {
                // GTK automatically selects an item if its in the list
                wxCommandEvent eventEnter(wxEVT_COMMAND_TEXT_ENTER, GetId());
                eventEnter.SetString( GetValue() );
                eventEnter.SetInt( GetSelection() );
                eventEnter.SetEventObject( this );

                if ( HandleWindowEvent(eventEnter) )
                {
                    // Catch GTK event so that GTK doesn't open the drop
                    // down list upon RETURN.
                    return;
                }
            }
            break;
    }

    event.Skip();
}

void wxComboBox::DisableEvents()
{
    g_signal_handlers_block_by_func(GTK_BIN(m_widget)->child,
        (gpointer)gtkcombobox_text_changed_callback, this);

    g_signal_handlers_block_by_func(m_widget,
        (gpointer)gtkcombobox_changed_callback, this);
}

void wxComboBox::EnableEvents()
{
    g_signal_handlers_unblock_by_func(GTK_BIN(m_widget)->child,
        (gpointer)gtkcombobox_text_changed_callback, this);

    g_signal_handlers_unblock_by_func(m_widget,
        (gpointer)gtkcombobox_changed_callback, this);
}

GtkWidget* wxComboBox::GetConnectWidget()
{
    return GTK_WIDGET( GetEntry() );
}

GdkWindow *wxComboBox::GTKGetWindow(wxArrayGdkWindows& windows) const
{
    wxUnusedVar(windows);

    return GetEntry()->text_area;
}

// static
wxVisualAttributes
wxComboBox::GetClassDefaultAttributes(wxWindowVariant WXUNUSED(variant))
{
    return GetDefaultAttributesFromGTKWidget(gtk_combo_box_entry_new, true);
}

// ----------------------------------------------------------------------------
// standard event handling
// ----------------------------------------------------------------------------

void wxComboBox::OnCut(wxCommandEvent& WXUNUSED(event))
{
    Cut();
}

void wxComboBox::OnCopy(wxCommandEvent& WXUNUSED(event))
{
    Copy();
}

void wxComboBox::OnPaste(wxCommandEvent& WXUNUSED(event))
{
    Paste();
}

void wxComboBox::OnUndo(wxCommandEvent& WXUNUSED(event))
{
    Undo();
}

void wxComboBox::OnRedo(wxCommandEvent& WXUNUSED(event))
{
    Redo();
}

void wxComboBox::OnDelete(wxCommandEvent& WXUNUSED(event))
{
    RemoveSelection();
}

void wxComboBox::OnSelectAll(wxCommandEvent& WXUNUSED(event))
{
    SelectAll();
}

void wxComboBox::OnUpdateCut(wxUpdateUIEvent& event)
{
    event.Enable( CanCut() );
}

void wxComboBox::OnUpdateCopy(wxUpdateUIEvent& event)
{
    event.Enable( CanCopy() );
}

void wxComboBox::OnUpdatePaste(wxUpdateUIEvent& event)
{
    event.Enable( CanPaste() );
}

void wxComboBox::OnUpdateUndo(wxUpdateUIEvent& event)
{
    event.Enable( CanUndo() );
}

void wxComboBox::OnUpdateRedo(wxUpdateUIEvent& event)
{
    event.Enable( CanRedo() );
}

void wxComboBox::OnUpdateDelete(wxUpdateUIEvent& event)
{
    event.Enable(HasSelection() && IsEditable()) ;
}

void wxComboBox::OnUpdateSelectAll(wxUpdateUIEvent& event)
{
    event.Enable(!wxTextEntry::IsEmpty());
}

#endif // wxUSE_COMBOBOX
