/////////////////////////////////////////////////////////////////////////
// File:        wx/msw/taskbar.h
// Purpose:     Defines wxTaskBarIcon class for manipulating icons on the
//              Windows task bar.
// Author:      Julian Smart
// Modified by:
// Created:     24/3/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////

#ifndef _TASKBAR_H_
#define _TASKBAR_H_

#ifdef __GNUG__
#pragma interface "taskbar.h"
#endif

#include <wx/event.h>
#include <wx/list.h>
#include <wx/icon.h>

class WXDLLEXPORT wxTaskBarIcon: public wxEvtHandler {
    DECLARE_DYNAMIC_CLASS(wxTaskBarIcon)
public:
    wxTaskBarIcon(void);
    virtual ~wxTaskBarIcon(void);

// Accessors
    inline WXHWND GetHWND() const { return m_hWnd; }
    inline bool IsOK() const { return (m_hWnd != 0) ; }
    inline bool IsIconInstalled() const { return m_iconAdded; }

// Operations
    bool SetIcon(const wxIcon& icon, const wxString& tooltip = "");
    bool RemoveIcon(void);
    bool PopupMenu(wxMenu *menu); //, int x, int y);

// Overridables
    virtual void OnMouseMove(wxEvent&);
    virtual void OnLButtonDown(wxEvent&);
    virtual void OnLButtonUp(wxEvent&);
    virtual void OnRButtonDown(wxEvent&);
    virtual void OnRButtonUp(wxEvent&);
    virtual void OnLButtonDClick(wxEvent&);
    virtual void OnRButtonDClick(wxEvent&);

// Implementation
    static wxTaskBarIcon* FindObjectForHWND(WXHWND hWnd);
    static void AddObject(wxTaskBarIcon* obj);
    static void RemoveObject(wxTaskBarIcon* obj);
    static bool RegisterWindowClass();
    static WXHWND CreateTaskBarWindow();
    long WindowProc( WXHWND hWnd, unsigned int msg, unsigned int wParam, long lParam );

// Data members
protected:
    WXHWND          m_hWnd;
    bool            m_iconAdded;
    static wxList   sm_taskBarIcons;
    static bool     sm_registeredClass;
    static unsigned int sm_taskbarMsg;

    // non-virtual default event handlers to forward events to the virtuals
    void _OnMouseMove(wxEvent&);
    void _OnLButtonDown(wxEvent&);
    void _OnLButtonUp(wxEvent&);
    void _OnRButtonDown(wxEvent&);
    void _OnRButtonUp(wxEvent&);
    void _OnLButtonDClick(wxEvent&);
    void _OnRButtonDClick(wxEvent&);


    DECLARE_EVENT_TABLE()
};


// ----------------------------------------------------------------------------
// wxTaskBarIcon events
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxTaskBarIconEvent : public wxEvent
{
public:
    wxTaskBarIconEvent(wxEventType evtType, wxTaskBarIcon *tbIcon)
        : wxEvent(-1, evtType)
        {
            SetEventObject(tbIcon);
        }

    virtual wxEvent *Clone() const { return new wxTaskBarIconEvent(*this); }
};

const wxEventType wxEVT_TASKBAR_MOVE =                  wxEVT_FIRST + 1550;
const wxEventType wxEVT_TASKBAR_LEFT_DOWN =             wxEVT_FIRST + 1551;
const wxEventType wxEVT_TASKBAR_LEFT_UP =               wxEVT_FIRST + 1552;
const wxEventType wxEVT_TASKBAR_RIGHT_DOWN =            wxEVT_FIRST + 1553;
const wxEventType wxEVT_TASKBAR_RIGHT_UP =              wxEVT_FIRST + 1554;
const wxEventType wxEVT_TASKBAR_LEFT_DCLICK =           wxEVT_FIRST + 1555;
const wxEventType wxEVT_TASKBAR_RIGHT_DCLICK =          wxEVT_FIRST + 1556;


#define EVT_TASKBAR_MOVE(fn)         DECLARE_EVENT_TABLE_ENTRY(wxEVT_TASKBAR_MOVE, -1, -1, (wxObjectEventFunction) (wxEventFunction) &fn, NULL),
#define EVT_TASKBAR_LEFT_DOWN(fn)    DECLARE_EVENT_TABLE_ENTRY(wxEVT_TASKBAR_LEFT_DOWN, -1, -1, (wxObjectEventFunction) (wxEventFunction) &fn, NULL),
#define EVT_TASKBAR_LEFT_UP(fn)      DECLARE_EVENT_TABLE_ENTRY(wxEVT_TASKBAR_LEFT_UP, -1, -1, (wxObjectEventFunction) (wxEventFunction) &fn, NULL),
#define EVT_TASKBAR_RIGHT_DOWN(fn)   DECLARE_EVENT_TABLE_ENTRY(wxEVT_TASKBAR_RIGHT_DOWN, -1, -1, (wxObjectEventFunction) (wxEventFunction) &fn, NULL),
#define EVT_TASKBAR_RIGHT_UP(fn)     DECLARE_EVENT_TABLE_ENTRY(wxEVT_TASKBAR_RIGHT_UP, -1, -1, (wxObjectEventFunction) (wxEventFunction) &fn, NULL),
#define EVT_TASKBAR_LEFT_DCLICK(fn)  DECLARE_EVENT_TABLE_ENTRY(wxEVT_TASKBAR_LEFT_DCLICK, -1, -1, (wxObjectEventFunction) (wxEventFunction) &fn, NULL),
#define EVT_TASKBAR_RIGHT_DCLICK(fn) DECLARE_EVENT_TABLE_ENTRY(wxEVT_TASKBAR_RIGHT_DCLICK, -1, -1, (wxObjectEventFunction) (wxEventFunction) &fn, NULL),


#endif
    // _TASKBAR_H_





