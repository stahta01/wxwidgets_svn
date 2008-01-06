/* -------------------------------------------------------------------------
 * Project: GSocket (Generic Socket) for WX
 * Name:    gsockpm.c
 * Purpose: GSocket: PM part
 * Licence: The wxWindows licence
 * CVSID:   $Id$
 * ------------------------------------------------------------------------- */

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#if wxUSE_SOCKETS

#include <stdlib.h>
#include "wx/unix/gsockunx.h"
#include "wx/gsocket.h"
#include "wx/app.h"

#define wxSockReadMask  0x01
#define wxSockWriteMask 0x02

static void _GSocket_PM_Input(void *data)
{
    GSocket *socket = (GSocket *) data;
    socket->Detected_Read();
}

static void _GSocket_PM_Output(void *data)
{
    GSocket *socket = (GSocket *) data;
    socket->Detected_Write();
}

class PMSocketManager : public GSocketInputBasedManager
{
public:
    virtual int AddInput(GSocket *socket, SocketDir d)
    {

      if (d == FD_OUTPUT)
          return wxTheApp->AddSocketHandler(socket->m_fd, wxSockWriteMask,
					    _GSocket_PM_Output, (void *)socket);
      else
          return wxTheApp->AddSocketHandler(socket->m_fd, wxSockReadMask,
					  _GSocket_PM_Input, (void *)socket);
    }

    virtual void RemoveInput(int fd)
    {
        wxTheApp->RemoveSocketHandler(fd);
    }
};

GSocketManager *wxGUIAppTraits::GetSocketManager()
{
    static GTKSocketManager s_manager;
    return &s_manager;
}


#else /* !wxUSE_SOCKETS */

/* some compilers don't like having empty source files */
static int wxDummyGsockVar = 0;

#endif /* wxUSE_SOCKETS/!wxUSE_SOCKETS */
