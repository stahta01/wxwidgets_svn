/////////////////////////////////////////////////////////////////////////////
// Name:        osx/core/gsockosx.cpp
// Purpose:     wxSocketImpl implementation for OS X
// Authors:     Brian Victor, Vadim Zeitlin
// Created:     February 2002
// RCS-ID:      $Id$
// Copyright:   (c) 2002 Brian Victor
//              (c) 2008 Vadim Zeitlin
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#if wxUSE_SOCKETS

#include "wx/private/socket.h"
#include "wx/unix/private/sockunix.h"
#include "wx/apptrait.h"

#include <CoreFoundation/CoreFoundation.h>

namespace
{

// ----------------------------------------------------------------------------
// global variables
// ----------------------------------------------------------------------------

// Sockets must use the event loop to monitor the events so we store a
// reference to the main thread event loop here
static CFRunLoopRef gs_mainRunLoop = NULL;

// ----------------------------------------------------------------------------
// Mac-specific socket implementation
// ----------------------------------------------------------------------------

class wxSocketImplMac : public wxSocketImplUnix
{
public:
    wxSocketImplMac(wxSocketBase& wxsocket)
        : wxSocketImplUnix(wxsocket)
    {
        m_socket = NULL;
        m_source = NULL;
    }

    virtual ~wxSocketImplMac()
    {
        wxASSERT_MSG( !m_source && !m_socket, "forgot to call Close()?" );
    }

    // get the underlying socket: creates it on demand
    CFSocketRef GetSocket() /* const */
    {
        if ( !m_socket )
            Initialize();

        return m_socket;
    }

private:
    virtual void DoClose()
    {
        wxSocketManager * const manager = wxSocketManager::Get();
        if ( manager )
        {
            manager->Uninstall_Callback(this, wxSOCKET_INPUT);
            manager->Uninstall_Callback(this, wxSOCKET_OUTPUT);
        }

        // VZ: CFRunLoopRemoveSource() is probably unnecessary as
        //     CFSocketInvalidate() seems to do it internally from reading the
        //     docs, please remove it (and this comment) after testing
        CFRunLoopRemoveSource(gs_mainRunLoop, m_source, kCFRunLoopCommonModes);
        CFSocketInvalidate(m_socket);

        CFRelease(m_source);
        CFRelease(m_socket);
    }

    // initialize the data associated with the given socket
    bool Initialize()
    {
        // we need a valid Unix socket to create a CFSocket
        if ( m_fd < 0 )
            return false;

        CFSocketContext cont;
        cont.version = 0;               // this currently must be 0
        cont.info = this;               // pointer passed to our callback
        cont.retain = NULL;             // no need to retain/release/copy the
        cont.release = NULL;            //  socket pointer, so all callbacks
        cont.copyDescription = NULL;    //  can be left NULL

        m_socket = CFSocketCreateWithNative
                   (
                        NULL,                   // default allocator
                        m_fd,
                        kCFSocketReadCallBack |
                        kCFSocketWriteCallBack |
                        kCFSocketConnectCallBack,
                        SocketCallback,
                        &cont
                   );
        if ( !m_socket )
            return false;

        m_source = CFSocketCreateRunLoopSource(NULL, m_socket, 0);

        if ( !m_source )
        {
            CFRelease(m_socket);
            return false;
        }

        CFRunLoopAddSource(gs_mainRunLoop, m_source, kCFRunLoopCommonModes);

        return true;
    }

    static void SocketCallback(CFSocketRef WXUNUSED(s),
                               CFSocketCallBackType callbackType,
                               CFDataRef WXUNUSED(address),
                               const void* data,
                               void* info)
    {
        wxSocketImplMac * const socket = static_cast<wxSocketImplMac *>(info);

        switch (callbackType)
        {
            case kCFSocketConnectCallBack:
                wxASSERT(!socket->m_server);
                // KH: If data is non-NULL, the connect failed, do not call Detected_Write,
                // which will only end up creating a spurious connect event because the
                // call to getsocketopt SO_ERROR inexplicably returns no error.
                // The change in behavior cannot be traced to any particular commit or
                // timeframe so I'm not sure what to think, but after so many hours,
                // this seems to address the issue and it's time to move on.
                if (data == NULL)
                    socket->OnWriteWaiting();
                break;

            case kCFSocketReadCallBack:
                socket->OnReadWaiting();
                break;

            case kCFSocketWriteCallBack:
                socket->OnWriteWaiting();
                break;

            default:
                wxFAIL_MSG( "unexpected socket callback" );
        }
    }

    CFSocketRef m_socket;
    CFRunLoopSourceRef m_source;

    DECLARE_NO_COPY_CLASS(wxSocketImplMac)
};

} // anonymous namespace


// ----------------------------------------------------------------------------
// CoreFoundation implementation of wxSocketManager
// ----------------------------------------------------------------------------

class wxSocketManagerMac : public wxSocketManager
{
public:
    virtual bool OnInit();
    virtual void OnExit();

    virtual wxSocketImpl *CreateSocket(wxSocketBase& wxsocket)
    {
        return new wxSocketImplMac(wxsocket);
    }

    virtual void Install_Callback(wxSocketImpl *socket, wxSocketNotify event);
    virtual void Uninstall_Callback(wxSocketImpl *socket, wxSocketNotify event);

private:
    // return CFSocket callback mask corresponding to the given event (the
    // socket parameter is needed because some events are interpreted
    // differently depending on whether they happen on a server or on a client
    // socket)
    static int GetCFCallback(wxSocketImpl *socket, wxSocketNotify event);
};

bool wxSocketManagerMac::OnInit()
{
    // No need to store the main loop again
    if (gs_mainRunLoop != NULL)
        return true;

    // Get the loop for the main thread so our events will actually fire.
    // The common socket.cpp code will assert if initialize is called from a
    // secondary thread, otherwise Mac would have the same problems as MSW
    gs_mainRunLoop = CFRunLoopGetCurrent();
    if ( !gs_mainRunLoop )
        return false;

    CFRetain(gs_mainRunLoop);

    return true;
}

void wxSocketManagerMac::OnExit()
{
    // Release the reference count, and set the reference back to NULL
    CFRelease(gs_mainRunLoop);
    gs_mainRunLoop = NULL;
}

/* static */
int wxSocketManagerMac::GetCFCallback(wxSocketImpl *socket, wxSocketNotify event)
{
    switch ( event )
    {
        case wxSOCKET_CONNECTION:
            return socket->m_server ? kCFSocketReadCallBack
                                    : kCFSocketConnectCallBack;

        case wxSOCKET_LOST:
        case wxSOCKET_INPUT:
            return kCFSocketReadCallBack;

        case wxSOCKET_OUTPUT:
            return kCFSocketWriteCallBack;

        case wxSOCKET_MAX_EVENT:
            wxFAIL_MSG( "invalid wxSocketNotify" );
            return 0;

        default:
            wxFAIL_MSG( "unknown wxSocketNotify" );
            return 0;
    }
}

void wxSocketManagerMac::Install_Callback(wxSocketImpl *socket_,
                                          wxSocketNotify event)
{
    wxSocketImplMac * const socket = static_cast<wxSocketImplMac *>(socket_);

    CFSocketEnableCallBacks(socket->GetSocket(), GetCFCallback(socket, event));
}

void wxSocketManagerMac::Uninstall_Callback(wxSocketImpl *socket_,
                                            wxSocketNotify event)
{
    wxSocketImplMac * const socket = static_cast<wxSocketImplMac *>(socket_);

    CFSocketDisableCallBacks(socket->GetSocket(), GetCFCallback(socket, event));
}

// set the wxBase variable to point to our wxSocketManager implementation
//
// see comments in wx/apptrait.h for the explanation of why do we do it
// like this
static struct ManagerSetter
{
    ManagerSetter()
    {
        static wxSocketManagerMac s_manager;
        wxAppTraits::SetDefaultSocketManager(&s_manager);
    }
} gs_managerSetter;

#endif // wxUSE_SOCKETS
