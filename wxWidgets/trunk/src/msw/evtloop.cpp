///////////////////////////////////////////////////////////////////////////////
// Name:        msw/evtloop.cpp
// Purpose:     implements wxEventLoop for MSW
// Author:      Vadim Zeitlin
// Modified by:
// Created:     01.06.01
// RCS-ID:      $Id$
// Copyright:   (c) 2001 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// License:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #if wxUSE_GUI
        #include "wx/window.h"
    #endif
    #include "wx/app.h"
#endif //WX_PRECOMP

#include "wx/evtloop.h"
#include "wx/thread.h"
#include "wx/except.h"
#include "wx/ptr_scpd.h"
#include "wx/msw/private.h"

#if wxUSE_GUI
    #include "wx/tooltip.h"
    #if wxUSE_THREADS
        // define the list of MSG strutures
        WX_DECLARE_LIST(MSG, wxMsgList);

        #include "wx/listimpl.cpp"

        WX_DEFINE_LIST(wxMsgList)
    #endif // wxUSE_THREADS
#endif //wxUSE_GUI

#if wxUSE_BASE

// ============================================================================
// wxMSWEventLoopBase implementation
// ============================================================================

// ----------------------------------------------------------------------------
// ctor/dtor
// ----------------------------------------------------------------------------

wxMSWEventLoopBase::wxMSWEventLoopBase()
{
    m_shouldExit = false;
    m_exitcode = 0;
}

// ----------------------------------------------------------------------------
// wxEventLoop message processing dispatching
// ----------------------------------------------------------------------------

bool wxMSWEventLoopBase::Pending() const
{
    MSG msg;
    return ::PeekMessage(&msg, 0, 0, 0, PM_NOREMOVE) != 0;
}

bool wxMSWEventLoopBase::GetNextMessage(WXMSG* msg)
{
    wxCHECK_MSG( IsRunning(), false, _T("can't get messages if not running") );

    const BOOL rc = ::GetMessage(msg, NULL, 0, 0);

    if ( rc == 0 )
    {
        // got WM_QUIT
        return false;
    }

    if ( rc == -1 )
    {
        // should never happen, but let's test for it nevertheless
        wxLogLastError(wxT("GetMessage"));

        // still break from the loop
        return false;
    }

    return true;
}

int wxMSWEventLoopBase::GetNextMessageTimeout(WXMSG *msg, unsigned long timeout)
{
    // MsgWaitForMultipleObjects() won't notice any input which was already
    // examined (e.g. using PeekMessage()) but not yet removed from the queue
    // so we need to remove any immediately messages manually
    //
    // NB: using MsgWaitForMultipleObjectsEx() could simplify the code here but
    //     it is not available in very old Windows versions
    if ( !::PeekMessage(msg, 0, 0, 0, PM_REMOVE) )
    {
        // we use this function just in order to not block longer than the
        // given timeout, so we don't pass any handles to it at all
        DWORD rc = ::MsgWaitForMultipleObjects
                     (
                        0, NULL,
                        FALSE,
                        timeout,
                        QS_ALLINPUT
                     );

        switch ( rc )
        {
            default:
                wxLogDebug("unexpected MsgWaitForMultipleObjects() return "
                           "value %lu", rc);
                // fall through

            case WAIT_TIMEOUT:
                return -1;

            case WAIT_OBJECT_0:
                if ( !::PeekMessage(msg, 0, 0, 0, PM_REMOVE) )
                {
                    // somehow it may happen that MsgWaitForMultipleObjects()
                    // returns true but there are no messages -- just treat it
                    // the same as timeout then
                    return -1;
                }
                break;
        }
    }

    return msg->message != WM_QUIT;
}


#endif // wxUSE_BASE

#if wxUSE_GUI

// ============================================================================
// GUI wxEventLoop implementation
// ============================================================================

wxWindowMSW *wxGUIEventLoop::ms_winCritical = NULL;

bool wxGUIEventLoop::IsChildOfCriticalWindow(wxWindowMSW *win)
{
    while ( win )
    {
        if ( win == ms_winCritical )
            return true;

        win = win->GetParent();
    }

    return false;
}

bool wxGUIEventLoop::PreProcessMessage(WXMSG *msg)
{
    HWND hwnd = msg->hwnd;
    wxWindow *wndThis = wxGetWindowFromHWND((WXHWND)hwnd);
    wxWindow *wnd;

    // this might happen if we're in a modeless dialog, or if a wx control has
    // children which themselves were not created by wx (i.e. wxActiveX control children)
    if ( !wndThis )
    {
        while ( hwnd && (::GetWindowLong(hwnd, GWL_STYLE) & WS_CHILD ))
        {
            hwnd = ::GetParent(hwnd);

            // If the control has a wx parent, break and give the parent a chance
            // to process the window message
            wndThis = wxGetWindowFromHWND((WXHWND)hwnd);
            if (wndThis != NULL)
                break;
        }

        if ( !wndThis )
        {
            // this may happen if the event occurred in a standard modeless dialog (the
            // only example of which I know of is the find/replace dialog) - then call
            // IsDialogMessage() to make TAB navigation in it work

            // NOTE: IsDialogMessage() just eats all the messages (i.e. returns true for
            // them) if we call it for the control itself
            return hwnd && ::IsDialogMessage(hwnd, msg) != 0;
        }
    }

    if ( !AllowProcessing(wndThis) )
    {
        // not a child of critical window, so we eat the event but take care to
        // stop an endless stream of WM_PAINTs which would have resulted if we
        // didn't validate the invalidated part of the window
        if ( msg->message == WM_PAINT )
            ::ValidateRect(hwnd, NULL);

        return true;
    }

#if wxUSE_TOOLTIPS
    // we must relay WM_MOUSEMOVE events to the tooltip ctrl if we want it to
    // popup the tooltip bubbles
    if ( msg->message == WM_MOUSEMOVE )
    {
        // we should do it if one of window children has an associated tooltip
        // (and not just if the window has a tooltip itself)
        if ( wndThis->HasToolTips() )
            wxToolTip::RelayEvent((WXMSG *)msg);
    }
#endif // wxUSE_TOOLTIPS

    // allow the window to prevent certain messages from being
    // translated/processed (this is currently used by wxTextCtrl to always
    // grab Ctrl-C/V/X, even if they are also accelerators in some parent)
    if ( !wndThis->MSWShouldPreProcessMessage((WXMSG *)msg) )
    {
        return false;
    }

    // try translations first: the accelerators override everything
    for ( wnd = wndThis; wnd; wnd = wnd->GetParent() )
    {
        if ( wnd->MSWTranslateMessage((WXMSG *)msg))
            return true;

        // stop at first top level window, i.e. don't try to process the key
        // strokes originating in a dialog using the accelerators of the parent
        // frame - this doesn't make much sense
        if ( wnd->IsTopLevel() )
            break;
    }

    // now try the other hooks (kbd navigation is handled here)
    for ( wnd = wndThis; wnd; wnd = wnd->GetParent() )
    {
        if ( wnd->MSWProcessMessage((WXMSG *)msg) )
            return true;

        // also stop at first top level window here, just as above because
        // if we don't do this, pressing ESC on a modal dialog shown as child
        // of a modal dialog with wxID_CANCEL will cause the parent dialog to
        // be closed, for example
        if ( wnd->IsTopLevel() )
            break;
    }

    // no special preprocessing for this message, dispatch it normally
    return false;
}

void wxGUIEventLoop::ProcessMessage(WXMSG *msg)
{
    // give us the chance to preprocess the message first
    if ( !PreProcessMessage(msg) )
    {
        // if it wasn't done, dispatch it to the corresponding window
        ::TranslateMessage(msg);
        ::DispatchMessage(msg);
    }
}

bool wxGUIEventLoop::Dispatch()
{
    MSG msg;
    if ( !GetNextMessage(&msg) )
        return false;

#if wxUSE_THREADS
    wxASSERT_MSG( wxThread::IsMain(),
                  wxT("only the main thread can process Windows messages") );

    static bool s_hadGuiLock = true;
    static wxMsgList s_aSavedMessages;

    // if a secondary thread owning the mutex is doing GUI calls, save all
    // messages for later processing - we can't process them right now because
    // it will lead to recursive library calls (and we're not reentrant)
    if ( !wxGuiOwnedByMainThread() )
    {
        s_hadGuiLock = false;

        // leave out WM_COMMAND messages: too dangerous, sometimes
        // the message will be processed twice
        if ( !wxIsWaitingForThread() || msg.message != WM_COMMAND )
        {
            MSG* pMsg = new MSG(msg);
            s_aSavedMessages.Append(pMsg);
        }

        return true;
    }
    else
    {
        // have we just regained the GUI lock? if so, post all of the saved
        // messages
        //
        // FIXME of course, it's not _exactly_ the same as processing the
        //       messages normally - expect some things to break...
        if ( !s_hadGuiLock )
        {
            s_hadGuiLock = true;

            wxMsgList::compatibility_iterator node = s_aSavedMessages.GetFirst();
            while (node)
            {
                MSG* pMsg = node->GetData();
                s_aSavedMessages.Erase(node);

                ProcessMessage(pMsg);
                delete pMsg;

                node = s_aSavedMessages.GetFirst();
            }
        }
    }
#endif // wxUSE_THREADS

    ProcessMessage(&msg);

    return true;
}

int wxGUIEventLoop::DispatchTimeout(unsigned long timeout)
{
    MSG msg;
    int rc = GetNextMessageTimeout(&msg, timeout);
    if ( rc != 1 )
        return rc;

    ProcessMessage(&msg);

    return 1;
}

void wxGUIEventLoop::OnNextIteration()
{
#if wxUSE_THREADS
    wxMutexGuiLeaveOrEnter();
#endif // wxUSE_THREADS
}

void wxGUIEventLoop::WakeUp()
{
    ::PostMessage(NULL, WM_NULL, 0, 0);
}

#else // !wxUSE_GUI

#if wxUSE_CONSOLE_EVENTLOOP

void wxConsoleEventLoop::OnNextIteration()
{
    if ( wxTheApp )
        wxTheApp->ProcessPendingEvents();
}

void wxConsoleEventLoop::WakeUp()
{
#if wxUSE_THREADS
    wxWakeUpMainThread();
#endif
}

void wxConsoleEventLoop::ProcessMessage(WXMSG *msg)
{
    if ( msg->message == WM_TIMER )
    {
        TIMERPROC proc = (TIMERPROC)msg->lParam;
        if ( proc )
            (*proc)(NULL, 0, msg->wParam, 0);
    }
    else
    {
        ::DispatchMessage(msg);
    }
}

bool wxConsoleEventLoop::Dispatch()
{
    MSG msg;
    if ( !GetNextMessage(&msg) )
        return false;

    ProcessMessage(&msg);

    return !m_shouldExit;
}

int wxConsoleEventLoop::DispatchTimeout(unsigned long timeout)
{
    MSG msg;
    int rc = GetNextMessageTimeout(&msg, timeout);
    if ( rc != 1 )
        return rc;

    ProcessMessage(&msg);

    return !m_shouldExit;
}

#endif // wxUSE_CONSOLE_EVENTLOOP

#endif //wxUSE_GUI
