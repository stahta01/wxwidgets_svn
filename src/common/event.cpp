/////////////////////////////////////////////////////////////////////////////
// Name:        event.cpp
// Purpose:     Event classes
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:       wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
    #pragma implementation "event.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/defs.h"
    #include "wx/control.h"
    #include "wx/utils.h"
    #include "wx/app.h"
    #include "wx/dc.h"
#endif

#include "wx/event.h"
#include "wx/validate.h"

#if !USE_SHARED_LIBRARY
    IMPLEMENT_DYNAMIC_CLASS(wxEvtHandler, wxObject)
    IMPLEMENT_ABSTRACT_CLASS(wxEvent, wxObject)
    IMPLEMENT_DYNAMIC_CLASS(wxCommandEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxNotifyEvent, wxCommandEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxScrollEvent, wxCommandEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxMouseEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxKeyEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxSizeEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxPaintEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxEraseEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxMoveEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxFocusEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxCloseEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxShowEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxMaximizeEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxIconizeEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxMenuEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxJoystickEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxDropFilesEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxActivateEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxInitDialogEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxSysColourChangedEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxIdleEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxUpdateUIEvent, wxCommandEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxNavigationKeyEvent, wxCommandEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxPaletteChangedEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxQueryNewPaletteEvent, wxEvent)

    const wxEventTable *wxEvtHandler::GetEventTable() const
        { return &wxEvtHandler::sm_eventTable; }

    const wxEventTable wxEvtHandler::sm_eventTable =
        { (const wxEventTable *)NULL, &wxEvtHandler::sm_eventTableEntries[0] };

    const wxEventTableEntry wxEvtHandler::sm_eventTableEntries[] =
        { { 0, 0, 0,
    #ifdef __SGI_CC__
        // stupid SGI compiler --- offer aug 98
        0L }
    #else // !SGI CC
        NULL }
    #endif // SGI/!SGI
        };

#endif // !USE_SHARED_LIBRARY

#if wxUSE_THREADS
/* To put pending event handlers */
extern wxList wxPendingEvents;
extern wxCriticalSection wxPendingEventsLocker;
#endif

/*
 * General wxWindows events, covering
 * all interesting things that might happen (button clicking, resizing,
 * setting text in widgets, etc.).
 *
 * For each completely new event type, derive a new event class.
 *
 */

wxEvent::wxEvent(int theId)
{
    m_eventType = wxEVT_NULL;
    m_eventObject = (wxObject *) NULL;
    m_eventHandle = (char *) NULL;
    m_timeStamp = 0;
    m_id = theId;
    m_skipped = FALSE;
    m_callbackUserData = (wxObject *) NULL;
    m_isCommandEvent = FALSE;
}

/*
 * Command events
 *
 */

wxCommandEvent::wxCommandEvent(wxEventType commandType, int theId)
{
    m_eventType = commandType;
    m_clientData = (char *) NULL;
    m_clientObject = (wxClientData *) NULL;
    m_extraLong = 0;
    m_commandInt = 0;
    m_id = theId;
    m_commandString = (char *) NULL;
    m_isCommandEvent = TRUE;
}

/*
 * Scroll events
 */

wxScrollEvent::wxScrollEvent(wxEventType commandType,
                             int id,
                             int pos,
                             int orient)
             : wxCommandEvent(commandType, id)
{
    m_extraLong = orient;
    m_commandInt = pos;
}


/*
 * Mouse events
 *
 */

wxMouseEvent::wxMouseEvent(wxEventType commandType)
{
    m_eventType = commandType;
    m_metaDown = FALSE;
    m_altDown = FALSE;
    m_controlDown = FALSE;
    m_shiftDown = FALSE;
    m_leftDown = FALSE;
    m_rightDown = FALSE;
    m_middleDown = FALSE;
    m_x = 0;
    m_y = 0;
}

// True if was a button dclick event (1 = left, 2 = middle, 3 = right)
// or any button dclick event (but = -1)
bool wxMouseEvent::ButtonDClick(int but) const
{
    switch (but)
    {
        case -1:
            return (LeftDClick() || MiddleDClick() || RightDClick());
        case 1:
            return LeftDClick();
        case 2:
            return MiddleDClick();
        case 3:
            return RightDClick();
        default:
            wxFAIL_MSG("invalid parameter in wxMouseEvent::ButtonDClick");
    }

    return FALSE;
}

// True if was a button down event (1 = left, 2 = middle, 3 = right)
// or any button down event (but = -1)
bool wxMouseEvent::ButtonDown(int but) const
{
    switch (but)
    {
        case -1:
            return (LeftDown() || MiddleDown() || RightDown());
        case 1:
            return LeftDown();
        case 2:
            return MiddleDown();
        case 3:
            return RightDown();
        default:
            wxFAIL_MSG("invalid parameter in wxMouseEvent::ButtonDown");
    }

    return FALSE;
}

// True if was a button up event (1 = left, 2 = middle, 3 = right)
// or any button up event (but = -1)
bool wxMouseEvent::ButtonUp(int but) const
{
    switch (but) {
        case -1:
            return (LeftUp() || MiddleUp() || RightUp());
        case 1:
            return LeftUp();
        case 2:
            return MiddleUp();
        case 3:
            return RightUp();
        default:
            wxFAIL_MSG("invalid parameter in wxMouseEvent::ButtonUp");
    }

    return FALSE;
}

// True if the given button is currently changing state
bool wxMouseEvent::Button(int but) const
{
    switch (but) {
        case -1:
            return (ButtonUp(-1) || ButtonDown(-1) || ButtonDClick(-1)) ;
        case 1:
            return (LeftDown() || LeftUp() || LeftDClick());
        case 2:
            return (MiddleDown() || MiddleUp() || MiddleDClick());
        case 3:
            return (RightDown() || RightUp() || RightDClick());
        default:
            wxFAIL_MSG("invalid parameter in wxMouseEvent::Button");
    }

    return FALSE;
}

bool wxMouseEvent::ButtonIsDown(int but) const
{
    switch (but) {
        case -1:
            return (LeftIsDown() || MiddleIsDown() || RightIsDown());
        case 1:
            return LeftIsDown();
        case 2:
            return MiddleIsDown();
        case 3:
            return RightIsDown();
        default:
            wxFAIL_MSG("invalid parameter in wxMouseEvent::ButtonIsDown");
    }

    return FALSE;
}

// Find the logical position of the event given the DC
wxPoint wxMouseEvent::GetLogicalPosition(const wxDC& dc) const
{
    wxPoint pt(dc.DeviceToLogicalX(m_x), dc.DeviceToLogicalY(m_y));
    return pt;
}


/*
 * Keyboard events
 *
 */

wxKeyEvent::wxKeyEvent(wxEventType type)
{
    m_eventType = type;
    m_shiftDown = FALSE;
    m_controlDown = FALSE;
    m_metaDown = FALSE;
    m_altDown = FALSE;
    m_keyCode = 0;
}

/*
 * Event handler
 */

wxEvtHandler::wxEvtHandler()
{
    m_nextHandler = (wxEvtHandler *) NULL;
    m_previousHandler = (wxEvtHandler *) NULL;
    m_enabled = TRUE;
    m_dynamicEvents = (wxList *) NULL;
    m_isWindow = FALSE;
#if wxUSE_THREADS
    m_eventsLocker = new wxCriticalSection();
#endif
    m_pendingEvents = (wxList *) NULL;
}

wxEvtHandler::~wxEvtHandler()
{
    // Takes itself out of the list of handlers
    if (m_previousHandler)
        m_previousHandler->m_nextHandler = m_nextHandler;

    if (m_nextHandler)
        m_nextHandler->m_previousHandler = m_previousHandler;

    if (m_dynamicEvents)
    {
        wxNode *node = m_dynamicEvents->First();
        while (node)
        {
            wxEventTableEntry *entry = (wxEventTableEntry*)node->Data();
            if (entry->m_callbackUserData) delete entry->m_callbackUserData;
            delete entry;
            node = node->Next();
        }
        delete m_dynamicEvents;
    };

    if (m_pendingEvents)
      delete m_pendingEvents;

#if wxUSE_THREADS
    delete m_eventsLocker;
#endif
}

#if wxUSE_THREADS
bool wxEvtHandler::ProcessThreadEvent(wxEvent& event)
{
    wxEvent *event_main;
    wxCriticalSectionLocker locker(*m_eventsLocker);

    // check that we are really in a child thread
    wxASSERT( !wxThread::IsMain() );

    if (m_pendingEvents == NULL)
      m_pendingEvents = new wxList();

    event_main = (wxEvent *)event.GetClassInfo()->CreateObject();
    *event_main = event;

    m_pendingEvents->Append(event_main);

    wxPendingEventsLocker.Enter();
    wxPendingEvents.Append(this);
    wxPendingEventsLocker.Leave();

    return TRUE;
}

void wxEvtHandler::ProcessPendingEvents()
{
    wxCriticalSectionLocker locker(*m_eventsLocker);
    wxNode *node = m_pendingEvents->First();
    wxEvent *event;

    while (node != NULL) {
      event = (wxEvent *)node->Data();
      ProcessEvent(*event);
      delete node;
      node = m_pendingEvents->First();
    }
}
#endif

/*
 * Event table stuff
 */

bool wxEvtHandler::ProcessEvent(wxEvent& event)
{
    // check that our flag corresponds to reality
    wxASSERT( m_isWindow == IsKindOf(CLASSINFO(wxWindow)) );

    // An event handler can be enabled or disabled
    if ( GetEvtHandlerEnabled() )
    {
#if wxUSE_THREADS
	// Check whether we are in a child thread.
        if (!wxThread::IsMain())
          return ProcessThreadEvent(event);
#endif
        // Handle per-instance dynamic event tables first

        if ( m_dynamicEvents && SearchDynamicEventTable(event) )
            return TRUE;

        // Then static per-class event tables

        const wxEventTable *table = GetEventTable();

        // Try the associated validator first, if this is a window.
        // Problem: if the event handler of the window has been replaced,
        // this wxEvtHandler may no longer be a window.
        // Therefore validators won't be processed if the handler
        // has been replaced with SetEventHandler.
        // THIS CAN BE CURED if PushEventHandler is used instead of
        // SetEventHandler, and then processing will be passed down the
        // chain of event handlers.
        if ( m_isWindow )
        {
            wxWindow *win = (wxWindow *)this;

            // Can only use the validator of the window which
            // is receiving the event
            if ( win == event.GetEventObject() )
            {
                wxValidator *validator = win->GetValidator();
                if ( validator && validator->ProcessEvent(event) )
                {
                    return TRUE;
                }
            }
        }

        // Search upwards through the inheritance hierarchy
        while ( table )
        {
            if ( SearchEventTable((wxEventTable&)*table, event) )
                return TRUE;
            table = table->baseTable;
        }
    }

    // Try going down the event handler chain
    if ( GetNextHandler() )
    {
        if ( GetNextHandler()->ProcessEvent(event) )
            return TRUE;
    }

    // Carry on up the parent-child hierarchy,
    // but only if event is a command event: it wouldn't
    // make sense for a parent to receive a child's size event, for example
    if ( m_isWindow && event.IsCommandEvent() )
    {
        wxWindow *win = (wxWindow *)this;
        wxWindow *parent = win->GetParent();
        if (parent && !parent->IsBeingDeleted())
            return parent->GetEventHandler()->ProcessEvent(event);
    }

    // Last try - application object.
    if ( wxTheApp && (this != wxTheApp) )
    {
        // Special case: don't pass wxEVT_IDLE to wxApp, since it'll always
        // swallow it. wxEVT_IDLE is sent explicitly to wxApp so it will be
        // processed appropriately via SearchEventTable.
        if ( event.GetEventType() != wxEVT_IDLE )
        {
            if ( wxTheApp->ProcessEvent(event) )
                return TRUE;
        }
    }

    return FALSE;
}

bool wxEvtHandler::SearchEventTable(wxEventTable& table, wxEvent& event)
{
    int i = 0;
    int commandId = event.GetId();

    // BC++ doesn't like while (table.entries[i].m_fn)

#ifdef __SC__
    while (table.entries[i].m_fn != 0)
#else
    while (table.entries[i].m_fn != 0L)
#endif
    {
        if ((event.GetEventType() == table.entries[i].m_eventType) &&
                (table.entries[i].m_id == -1 || // Match, if event spec says any id will do (id == -1)
                 (table.entries[i].m_lastId == -1 && commandId == table.entries[i].m_id) ||
                 (table.entries[i].m_lastId != -1 &&
                  (commandId >= table.entries[i].m_id && commandId <= table.entries[i].m_lastId))))
        {
            event.Skip(FALSE);
            event.m_callbackUserData = table.entries[i].m_callbackUserData;

            (this->*((wxEventFunction) (table.entries[i].m_fn)))(event);

            if ( event.GetSkipped() )
                return FALSE;
            else
                return TRUE;
        }
        i++;
    }
    return FALSE;
}
void wxEvtHandler::Connect( int id, int lastId,
                            wxEventType eventType,
                            wxObjectEventFunction func,
                            wxObject *userData )
{
    wxEventTableEntry *entry = new wxEventTableEntry;
    entry->m_id = id;
    entry->m_lastId = lastId;
    entry->m_eventType = eventType;
    entry->m_fn = func;
    entry->m_callbackUserData = userData;

    if (!m_dynamicEvents)
        m_dynamicEvents = new wxList;

    m_dynamicEvents->Append( (wxObject*) entry );
}

bool wxEvtHandler::SearchDynamicEventTable( wxEvent& event )
{
    wxCHECK_MSG( m_dynamicEvents, FALSE,
                 "caller should check that we have dynamic events" );

    int commandId = event.GetId();

    wxNode *node = m_dynamicEvents->First();
    while (node)
    {
        wxEventTableEntry *entry = (wxEventTableEntry*)node->Data();

        if (entry->m_fn)
        {
            // Match, if event spec says any id will do (id == -1)
            if ( (event.GetEventType() == entry->m_eventType) &&
                 (entry->m_id == -1 ||
                  (entry->m_lastId == -1 && commandId == entry->m_id) ||
                  (entry->m_lastId != -1 &&
                  (commandId >= entry->m_id && commandId <= entry->m_lastId))) )
            {
                event.Skip(FALSE);
                event.m_callbackUserData = entry->m_callbackUserData;

                (this->*((wxEventFunction) (entry->m_fn)))(event);

                if (event.GetSkipped())
                    return FALSE;
                else
                    return TRUE;
            }
        }
        node = node->Next();
    }
    return FALSE;
};

#if WXWIN_COMPATIBILITY
bool wxEvtHandler::OnClose()
{
    if (GetNextHandler())
        return GetNextHandler()->OnClose();
    else
        return FALSE;
}
#endif // WXWIN_COMPATIBILITY

