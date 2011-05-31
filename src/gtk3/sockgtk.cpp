///////////////////////////////////////////////////////////////////////////////
// Name:        gtk/gsockgtk.cpp
// Purpose:     implementation of wxGTK-specific socket event handling
// Author:      Guilhem Lavaux, Vadim Zeitlin
// Created:     1999
// RCS-ID:      $Id: sockgtk.cpp 62789 2009-12-05 19:57:58Z PC $
// Copyright:   (c) 1999, 2007 wxWidgets dev team
//              (c) 2009 Vadim Zeitlin
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#if wxUSE_SOCKETS

#include "wx/apptrait.h"
#include "wx/private/fdiomanager.h"

#include <gdk/gdk.h>

extern "C" {
static
void wxSocket_GDK_Input(gpointer data,
                        gint WXUNUSED(source),
                        GIOCondition condition)
{
    wxFDIOHandler * const handler = static_cast<wxFDIOHandler *>(data);

    if ( condition & G_IO_IN )
    {
        handler->OnReadWaiting();

        // we could have lost connection while reading in which case we
        // shouldn't call OnWriteWaiting() as the socket is now closed and it
        // would assert
        if ( !handler->IsOk() )
            return;
    }

    if ( condition & G_IO_OUT )
        handler->OnWriteWaiting();
}
}

class GTKFDIOManager : public wxFDIOManager
{
public:
    // virtual int AddInput(wxFDIOHandler *handler, int fd, Direction d)
    // {
    //     return g_io_add_watch
    //            (
    //                 fd,
    //                 d == OUTPUT ? G_IO_OUT : G_IO_IN,
    //                 wxSocket_GDK_Input,
    //                 handler
    //            );
    // }

    // virtual void
    // RemoveInput(wxFDIOHandler* WXUNUSED(handler), int fd, Direction WXUNUSED(d))
    // {
    //     gdk_input_remove(fd);
    // }
};

wxFDIOManager *wxGUIAppTraits::GetFDIOManager()
{
    // static GTKFDIOManager s_manager;
    // return &s_manager;
}

#endif // wxUSE_SOCKETS
