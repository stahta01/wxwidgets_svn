/////////////////////////////////////////////////////////////////////////////
// Name:        app.h
// Purpose:     wxApp class
// Author:      David Webster
// Modified by:
// Created:     10/13/99
// RCS-ID:      $Id$
// Copyright:   (c) David Webster
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_APP_H_
#define _WX_APP_H_

#ifdef __WATCOMC__

#include <sys/ioctl.h>
#include <sys/select.h>

#else

#include <sys/time.h>
#include <sys/types.h>

#ifdef __EMX__
#include <unistd.h>
#else
#include <utils.h>
#undef BYTE_ORDER
#include <types.h>
#define INCL_ORDERS
#endif

#endif

#include "wx/event.h"
#include "wx/icon.h"

class WXDLLEXPORT wxFrame;
class WXDLLEXPORT wxWindow;
class WXDLLEXPORT wxApp;
class WXDLLEXPORT wxKeyEvent;
class WXDLLEXPORT wxLog;

WXDLLEXPORT_DATA(extern wxApp*) wxTheApp;
WXDLLEXPORT_DATA(extern HAB)    vHabmain;

// Force an exit from main loop
void WXDLLEXPORT wxExit(void);

// Yield to other apps/messages
bool WXDLLEXPORT wxYield(void);

extern MRESULT EXPENTRY wxWndProc( HWND
                                  ,ULONG
                                  ,MPARAM
                                  ,MPARAM
                                 );


// Represents the application. Derive OnInit and declare
// a new App object to start application
class WXDLLEXPORT wxApp : public wxAppBase
{
    DECLARE_DYNAMIC_CLASS(wxApp)

public:
    wxApp();
    virtual ~wxApp();

    virtual bool OnInitGui(void);

    // override base class (pure) virtuals
    virtual int  MainLoop(void);
    virtual void ExitMainLoop(void);
    virtual bool Initialized(void);
    virtual bool Pending(void) ;
    virtual bool Dispatch(void);

    virtual void Exit();

    virtual bool Yield(bool onlyIfNeeded = FALSE);
    virtual void WakeUpIdle(void);

    virtual void SetPrintMode(int mode) { m_nPrintMode = mode; }
    virtual int  GetPrintMode(void) const { return m_nPrintMode; }

    // implementation only
    void OnIdle(wxIdleEvent& rEvent);
    void OnEndSession(wxCloseEvent& rEvent);
    void OnQueryEndSession(wxCloseEvent& rEvent);

    int AddSocketHandler(int handle, int mask,
                         void (*callback)(void*), void * gsock);
    void RemoveSocketHandler(int handle);
    void HandleSockets();

protected:
    bool                            m_bShowOnInit;
    int                             m_nPrintMode; // wxPRINT_WINDOWS, wxPRINT_POSTSCRIPT

    //
    // PM-specific wxApp definitions */
    //
private:
    int                             m_maxSocketHandles;
    int                             m_maxSocketNr;
    int                             m_lastUsedHandle;
    fd_set                          m_readfds;
    fd_set                          m_writefds;
    void*                           m_sockCallbackInfo;

public:

    // Implementation
    virtual bool  Initialize(int& argc, wxChar **argv);
    virtual void  CleanUp(void);

    static bool  RegisterWindowClasses(HAB vHab);
    virtual void DoMessage(WXMSG *pMsg);
    virtual bool DoMessage(void);
    virtual bool ProcessMessage(WXMSG* pMsg);

public:
    int                             m_nCmdShow;
    HMQ                             m_hMq;

protected:
    bool                            m_bKeepGoing ;

    DECLARE_EVENT_TABLE()
};
#endif
    // _WX_APP_H_

