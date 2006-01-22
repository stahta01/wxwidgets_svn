///////////////////////////////////////////////////////////////////////////////
// Name:        wx/gtk/private.h
// Purpose:     wxGTK private macros, functions &c
// Author:      Vadim Zeitlin
// Modified by:
// Created:     12.03.02
// RCS-ID:      $Id$
// Copyright:   (c) 2002 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_GTK_PRIVATE_H_
#define _WX_GTK_PRIVATE_H_

#include <gdk/gdk.h>
#include <gtk/gtk.h>

#include "wx/event.h"

// fail all version tests if the GTK+ version is so ancient that it doesn't
// even have GTK_CHECK_VERSION
#ifndef GTK_CHECK_VERSION
    #define GTK_CHECK_VERSION(a, b, c) 0
#endif

#if wxUSE_UNICODE
    #define wxGTK_CONV(s) wxConvUTF8.cWX2MB(s)
    #define wxGTK_CONV_BACK(s) wxConvUTF8.cMB2WX(s)
#else
    #define wxGTK_CONV(s) wxConvUTF8.cWC2MB( wxConvLocal.cWX2WC(s) )
    #define wxGTK_CONV_BACK(s)  wxConvLocal.cWC2WX( (wxConvUTF8.cMB2WC( s ) ) )
#endif

// FIXME: Make gtk2 only, so no macros needed - MR
// GTK+ 2.0 compatibility define is broken when used from C++ as it
// casts enum to int implicitly
#undef gtk_signal_disconnect_by_func
#define gtk_signal_disconnect_by_func(object,func,data) \
    gtk_signal_compat_matched((object), (func), (data), \
                              (GSignalMatchType)(G_SIGNAL_MATCH_FUNC | \
                                                 G_SIGNAL_MATCH_DATA), 0)

// child is not a member of GTK_BUTTON() any more in GTK+ 2.0
#define BUTTON_CHILD(w) GTK_BIN((w))->child

// event_window has disappeared from GtkToggleButton in GTK+ 2.0
#define TOGGLE_BUTTON_EVENT_WIN(w) GTK_BUTTON((w))->event_window

// gtk_editable_{copy|cut|paste}_clipboard() had an extra argument under
// previous GTK+ versions but no more
#if defined(__WXGTK20__) || (GTK_MINOR_VERSION > 0)
    #define DUMMY_CLIPBOARD_ARG
#else
    #define DUMMY_CLIPBOARD_ARG  ,0
#endif

// _GtkEditable is now private
#define GET_EDITABLE_POS(w) gtk_editable_get_position(GTK_EDITABLE(w))
#define SET_EDITABLE_POS(w, pos) \
    gtk_editable_set_position(GTK_EDITABLE(w), (pos))

// this GtkNotebook struct field has been renamed
#define NOTEBOOK_PANEL(nb)  GTK_NOTEBOOK(nb)->event_window

#define SCROLLBAR_CBACK_ARG
#define GET_SCROLL_TYPE(w)   GTK_SCROLL_JUMP

// translate a GTK+ scroll type to a wxEventType
inline wxEventType GtkScrollTypeToWx(guint scrollType)
{
    wxEventType command;
    switch ( scrollType )
    {
        case GTK_SCROLL_STEP_BACKWARD:
            command = wxEVT_SCROLL_LINEUP;
            break;

        case GTK_SCROLL_STEP_FORWARD:
            command = wxEVT_SCROLL_LINEDOWN;
            break;

        case GTK_SCROLL_PAGE_BACKWARD:
            command = wxEVT_SCROLL_PAGEUP;
            break;

        case GTK_SCROLL_PAGE_FORWARD:
            command = wxEVT_SCROLL_PAGEDOWN;
            break;

        default:
            command = wxEVT_SCROLL_THUMBTRACK;
    }

    return command;
}

inline wxEventType GtkScrollWinTypeToWx(guint scrollType)
{
    // GtkScrollTypeToWx() returns SCROLL_XXX, not SCROLLWIN_XXX as we need
    return GtkScrollTypeToWx(scrollType) +
            wxEVT_SCROLLWIN_TOP - wxEVT_SCROLL_TOP;
}

// Needed for implementing e.g. combobox on wxGTK within a modal dialog.
void wxAddGrab(wxWindow* window);
void wxRemoveGrab(wxWindow* window);

// Escapes string so that it is valid Pango markup XML string:
WXDLLIMPEXP_CORE wxString wxEscapeStringForPangoMarkup(const wxString& str);

// The declaration for gtk_icon_size_lookup was accidentally ifdefed out in
// GTK+ 2.1.0 which Sun seem to have shipped with some versions of JDS
// for Solaris 9 x86.
#ifdef NEED_GTK_ICON_SIZE_LOOKUP
extern "C" gboolean gtk_icon_size_lookup  (GtkIconSize  size,
                                           gint         *width,
                                           gint         *height);
#endif

#endif // _WX_GTK_PRIVATE_H_

