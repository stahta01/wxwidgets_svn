/////////////////////////////////////////////////////////////////////////////
// Name:        gtk/fontdlg.cpp
// Purpose:     wxFontDialog
// Author:      Robert Roebling
// Id:          $Id$
// Copyright:   (c) 1998 Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "fontdlg.h"
#endif

#include "wx/defs.h"

#if wxUSE_FONTDLG

#include "wx/fontutil.h"
#include "wx/fontdlg.h"
#include "wx/utils.h"
#include "wx/intl.h"
#include "wx/debug.h"
#include "wx/msgdlg.h"

#include "wx/gtk/private.h"

//-----------------------------------------------------------------------------
// idle system
//-----------------------------------------------------------------------------

extern void wxapp_install_idle_handler();
extern bool g_isIdle;

//-----------------------------------------------------------------------------
// "delete_event"
//-----------------------------------------------------------------------------

static
bool gtk_fontdialog_delete_callback( GtkWidget *WXUNUSED(widget), GdkEvent *WXUNUSED(event), wxDialog *win )
{
    if (g_isIdle)
        wxapp_install_idle_handler();

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
// "clicked" for OK-button
//-----------------------------------------------------------------------------

#ifdef __WXGTK12__
static
void gtk_fontdialog_ok_callback( GtkWidget *WXUNUSED(widget), wxFontDialog *dialog )
{
    if (g_isIdle)
        wxapp_install_idle_handler();

    GtkFontSelectionDialog *fontdlg = GTK_FONT_SELECTION_DIALOG(dialog->m_widget);
    GdkFont *gfont = gtk_font_selection_dialog_get_font(fontdlg);

    if (!gfont)
    {
        wxMessageBox(_("Please choose a valid font."), _("Error"),
                     wxOK | wxICON_ERROR);
        return;
    }

    gchar *fontname = gtk_font_selection_dialog_get_font_name(fontdlg);

    dialog->SetChosenFont(fontname);

    g_free( fontname );

    wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, wxID_OK);
    event.SetEventObject( dialog );
    dialog->GetEventHandler()->ProcessEvent( event );
}
#endif // GTK+ 1.2 and later only

//-----------------------------------------------------------------------------
// "clicked" for Cancel-button
//-----------------------------------------------------------------------------

static
void gtk_fontdialog_cancel_callback( GtkWidget *WXUNUSED(w), wxFontDialog *dialog )
{
    if (g_isIdle)
        wxapp_install_idle_handler();

    wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, wxID_CANCEL);
    event.SetEventObject( dialog );
    dialog->GetEventHandler()->ProcessEvent( event );
}

//-----------------------------------------------------------------------------
// wxFontDialog
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxFontDialog, wxDialog)

bool wxFontDialog::DoCreate(wxWindow *parent)
{
    m_needParent = FALSE;

    if (!PreCreation( parent, wxDefaultPosition, wxDefaultSize ) ||
        !CreateBase( parent, -1, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE,
                     wxDefaultValidator, wxT("fontdialog") ))
    {
        wxFAIL_MSG( wxT("wxFontDialog creation failed") );
        return FALSE;
    }

    wxString m_message( _("Choose font") );
    m_widget = gtk_font_selection_dialog_new( m_message.mbc_str() );

    int x = (gdk_screen_width () - 400) / 2;
    int y = (gdk_screen_height () - 400) / 2;
    gtk_widget_set_uposition( m_widget, x, y );

    GtkFontSelectionDialog *sel = GTK_FONT_SELECTION_DIALOG(m_widget);

    gtk_signal_connect( GTK_OBJECT(sel->ok_button), "clicked",
      GTK_SIGNAL_FUNC(gtk_fontdialog_ok_callback), (gpointer*)this );

    // strange way to internationalize
    gtk_label_set( GTK_LABEL( BUTTON_CHILD(sel->ok_button) ), wxConvCurrent->cWX2MB(_("OK")) );

    gtk_signal_connect( GTK_OBJECT(sel->cancel_button), "clicked",
      GTK_SIGNAL_FUNC(gtk_fontdialog_cancel_callback), (gpointer*)this );

    // strange way to internationalize
    gtk_label_set( GTK_LABEL( BUTTON_CHILD(sel->cancel_button) ), wxConvCurrent->cWX2MB(_("Cancel")) );

    gtk_signal_connect( GTK_OBJECT(m_widget), "delete_event",
        GTK_SIGNAL_FUNC(gtk_fontdialog_delete_callback), (gpointer)this );

    wxFont font = m_fontData.GetInitialFont();
    if( font.Ok() )
    {
        wxNativeFontInfo *info = font.GetNativeFontInfo();

        if ( info )
        {
            const wxString& fontname = info->GetXFontName();
            if ( !fontname )
                font.GetInternalFont();
            gtk_font_selection_dialog_set_font_name
            (
                sel,
                wxConvCurrent->cWX2MB(fontname)
            );
        }
        else
        {
            // this is not supposed to happen!
            wxFAIL_MSG(_T("font is ok but no native font info?"));
        }
    }

    return TRUE;
}

wxFontDialog::~wxFontDialog()
{
}

void wxFontDialog::SetChosenFont(const char *fontname)
{
    m_fontData.SetChosenFont(wxFont(fontname));
}

#endif // wxUSE_FONTDLG

