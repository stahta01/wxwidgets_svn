/////////////////////////////////////////////////////////////////////////////
// Name:        app.h
// Purpose:     wxApp class
// Author:      Julian Smart
// Modified by:
// Created:     17/09/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_APP_H_
#define _WX_APP_H_

#ifdef __GNUG__
#pragma interface "app.h"
#endif

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include "wx/event.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxFrame;
class WXDLLEXPORT wxWindow;
class WXDLLEXPORT wxApp;
class WXDLLEXPORT wxKeyEvent;
class WXDLLEXPORT wxLog;
class WXDLLEXPORT wxEventLoop;

// ----------------------------------------------------------------------------
// the wxApp class for Motif - see wxAppBase for more details
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxApp : public wxAppBase
{
    DECLARE_DYNAMIC_CLASS(wxApp)
        
public:
    wxApp();
    virtual ~wxApp();
    
    // override base class (pure) virtuals
    // -----------------------------------
    
    virtual int MainLoop();
    virtual void ExitMainLoop();
    virtual bool Initialized();
    virtual bool Pending();
    virtual void Dispatch();
    virtual bool Yield(bool onlyIfNeeded = FALSE);
    virtual bool ProcessIdle();
    
    virtual bool OnInitGui();
    
    // implementation from now on
    // --------------------------
    
    void OnIdle(wxIdleEvent& event);
    
    // Send idle event to all top-level windows.
    // Returns TRUE if more idle time is requested.
    bool SendIdleEvents();
    
    // Send idle event to window and all subwindows
    // Returns TRUE if more idle time is requested.
    bool SendIdleEvents(wxWindow* win);
    
protected:
    bool                  m_showOnInit;
    
public:
    // Implementation
    static bool Initialize();
    static void CleanUp();
    
    void DeletePendingObjects();
    
    // Motif-specific
    WXAppContext   GetAppContext() const { return m_appContext; }
    WXWidget       GetTopLevelWidget() const { return m_topLevelWidget; }
    WXColormap     GetMainColormap(WXDisplay* display);
    WXDisplay*     GetInitialDisplay() const { return m_initialDisplay; }
    long           GetMaxRequestSize() const { return m_maxRequestSize; }
    
    // This handler is called when a property change event occurs
    virtual void   HandlePropertyChange(WXEvent *event);
    
private:
    static long    sm_lastMessageTime;
    int            m_nCmdShow;

    wxEventLoop*    m_eventLoop;
    
    // Motif-specific
    WXAppContext          m_appContext;
    WXWidget              m_topLevelWidget;
    WXColormap            m_mainColormap;
    WXDisplay*            m_initialDisplay;
    long                  m_maxRequestSize;
    
    DECLARE_EVENT_TABLE()
};

int WXDLLEXPORT wxEntry( int argc, char *argv[] );

#endif
// _WX_APP_H_

