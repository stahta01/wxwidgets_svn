/////////////////////////////////////////////////////////////////////////////
// Name:        event.cpp
// Purpose:     Event classes
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

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
    #include "wx/app.h"
    #include "wx/list.h"

    #if wxUSE_GUI
        #include "wx/control.h"
        #include "wx/utils.h"
        #include "wx/dc.h"
    #endif // wxUSE_GUI
#endif

#include "wx/event.h"

#if wxUSE_GUI
    #include "wx/validate.h"
#endif // wxUSE_GUI

// ----------------------------------------------------------------------------
// wxWin macros
// ----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxEvtHandler, wxObject)
IMPLEMENT_ABSTRACT_CLASS(wxEvent, wxObject)

#if wxUSE_GUI
    IMPLEMENT_DYNAMIC_CLASS(wxIdleEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxCommandEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxNotifyEvent, wxCommandEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxScrollEvent, wxCommandEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxScrollWinEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxMouseEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxKeyEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxSizeEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxPaintEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxNcPaintEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxEraseEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxMoveEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxFocusEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxChildFocusEvent, wxCommandEvent)
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
    IMPLEMENT_DYNAMIC_CLASS(wxUpdateUIEvent, wxCommandEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxNavigationKeyEvent, wxCommandEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxPaletteChangedEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxQueryNewPaletteEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxWindowCreateEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxWindowDestroyEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxHelpEvent, wxCommandEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxContextMenuEvent, wxCommandEvent)
#endif // wxUSE_GUI

const wxEventTable *wxEvtHandler::GetEventTable() const
    { return &wxEvtHandler::sm_eventTable; }

const wxEventTable wxEvtHandler::sm_eventTable =
    { (const wxEventTable *)NULL, &wxEvtHandler::sm_eventTableEntries[0] };

const wxEventTableEntry wxEvtHandler::sm_eventTableEntries[] =
    { DECLARE_EVENT_TABLE_ENTRY(wxEVT_NULL, 0, 0, (wxObjectEventFunction)NULL, NULL) };

// ----------------------------------------------------------------------------
// global variables
// ----------------------------------------------------------------------------

// To put pending event handlers
wxList *wxPendingEvents = (wxList *)NULL;

#if wxUSE_THREADS
    // protects wxPendingEvents list
    wxCriticalSection *wxPendingEventsLocker = (wxCriticalSection *)NULL;
#endif

#if !WXWIN_COMPATIBILITY_EVENT_TYPES

// common event types are defined here, other event types are defined by the
// components which use them

DEFINE_EVENT_TYPE(wxEVT_NULL)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_BUTTON_CLICKED)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_CHECKBOX_CLICKED)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_CHOICE_SELECTED)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LISTBOX_SELECTED)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LISTBOX_DOUBLECLICKED)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_CHECKLISTBOX_TOGGLED)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_MENU_SELECTED)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_SLIDER_UPDATED)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_RADIOBOX_SELECTED)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_RADIOBUTTON_SELECTED)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_SCROLLBAR_UPDATED)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_VLBOX_SELECTED)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_COMBOBOX_SELECTED)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_TOOL_RCLICKED)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_TOOL_ENTER)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_SPINCTRL_UPDATED)

// Sockets and timers send events, too
DEFINE_EVENT_TYPE(wxEVT_SOCKET)
DEFINE_EVENT_TYPE(wxEVT_TIMER)

// Mouse event types
DEFINE_EVENT_TYPE(wxEVT_LEFT_DOWN)
DEFINE_EVENT_TYPE(wxEVT_LEFT_UP)
DEFINE_EVENT_TYPE(wxEVT_MIDDLE_DOWN)
DEFINE_EVENT_TYPE(wxEVT_MIDDLE_UP)
DEFINE_EVENT_TYPE(wxEVT_RIGHT_DOWN)
DEFINE_EVENT_TYPE(wxEVT_RIGHT_UP)
DEFINE_EVENT_TYPE(wxEVT_MOTION)
DEFINE_EVENT_TYPE(wxEVT_ENTER_WINDOW)
DEFINE_EVENT_TYPE(wxEVT_LEAVE_WINDOW)
DEFINE_EVENT_TYPE(wxEVT_LEFT_DCLICK)
DEFINE_EVENT_TYPE(wxEVT_MIDDLE_DCLICK)
DEFINE_EVENT_TYPE(wxEVT_RIGHT_DCLICK)
DEFINE_EVENT_TYPE(wxEVT_SET_FOCUS)
DEFINE_EVENT_TYPE(wxEVT_KILL_FOCUS)
DEFINE_EVENT_TYPE(wxEVT_CHILD_FOCUS)
DEFINE_EVENT_TYPE(wxEVT_MOUSEWHEEL)

// Non-client mouse events
DEFINE_EVENT_TYPE(wxEVT_NC_LEFT_DOWN)
DEFINE_EVENT_TYPE(wxEVT_NC_LEFT_UP)
DEFINE_EVENT_TYPE(wxEVT_NC_MIDDLE_DOWN)
DEFINE_EVENT_TYPE(wxEVT_NC_MIDDLE_UP)
DEFINE_EVENT_TYPE(wxEVT_NC_RIGHT_DOWN)
DEFINE_EVENT_TYPE(wxEVT_NC_RIGHT_UP)
DEFINE_EVENT_TYPE(wxEVT_NC_MOTION)
DEFINE_EVENT_TYPE(wxEVT_NC_ENTER_WINDOW)
DEFINE_EVENT_TYPE(wxEVT_NC_LEAVE_WINDOW)
DEFINE_EVENT_TYPE(wxEVT_NC_LEFT_DCLICK)
DEFINE_EVENT_TYPE(wxEVT_NC_MIDDLE_DCLICK)
DEFINE_EVENT_TYPE(wxEVT_NC_RIGHT_DCLICK)

// Character input event type
DEFINE_EVENT_TYPE(wxEVT_CHAR)
DEFINE_EVENT_TYPE(wxEVT_CHAR_HOOK)
DEFINE_EVENT_TYPE(wxEVT_NAVIGATION_KEY)
DEFINE_EVENT_TYPE(wxEVT_KEY_DOWN)
DEFINE_EVENT_TYPE(wxEVT_KEY_UP)

// Set cursor event
DEFINE_EVENT_TYPE(wxEVT_SET_CURSOR)

// wxScrollbar and wxSlider event identifiers
DEFINE_EVENT_TYPE(wxEVT_SCROLL_TOP)
DEFINE_EVENT_TYPE(wxEVT_SCROLL_BOTTOM)
DEFINE_EVENT_TYPE(wxEVT_SCROLL_LINEUP)
DEFINE_EVENT_TYPE(wxEVT_SCROLL_LINEDOWN)
DEFINE_EVENT_TYPE(wxEVT_SCROLL_PAGEUP)
DEFINE_EVENT_TYPE(wxEVT_SCROLL_PAGEDOWN)
DEFINE_EVENT_TYPE(wxEVT_SCROLL_THUMBTRACK)
DEFINE_EVENT_TYPE(wxEVT_SCROLL_THUMBRELEASE)

// Scroll events from wxWindow
DEFINE_EVENT_TYPE(wxEVT_SCROLLWIN_TOP)
DEFINE_EVENT_TYPE(wxEVT_SCROLLWIN_BOTTOM)
DEFINE_EVENT_TYPE(wxEVT_SCROLLWIN_LINEUP)
DEFINE_EVENT_TYPE(wxEVT_SCROLLWIN_LINEDOWN)
DEFINE_EVENT_TYPE(wxEVT_SCROLLWIN_PAGEUP)
DEFINE_EVENT_TYPE(wxEVT_SCROLLWIN_PAGEDOWN)
DEFINE_EVENT_TYPE(wxEVT_SCROLLWIN_THUMBTRACK)
DEFINE_EVENT_TYPE(wxEVT_SCROLLWIN_THUMBRELEASE)

// System events
DEFINE_EVENT_TYPE(wxEVT_SIZE)
DEFINE_EVENT_TYPE(wxEVT_MOVE)
DEFINE_EVENT_TYPE(wxEVT_CLOSE_WINDOW)
DEFINE_EVENT_TYPE(wxEVT_END_SESSION)
DEFINE_EVENT_TYPE(wxEVT_QUERY_END_SESSION)
DEFINE_EVENT_TYPE(wxEVT_ACTIVATE_APP)
DEFINE_EVENT_TYPE(wxEVT_POWER)
DEFINE_EVENT_TYPE(wxEVT_ACTIVATE)
DEFINE_EVENT_TYPE(wxEVT_CREATE)
DEFINE_EVENT_TYPE(wxEVT_DESTROY)
DEFINE_EVENT_TYPE(wxEVT_SHOW)
DEFINE_EVENT_TYPE(wxEVT_ICONIZE)
DEFINE_EVENT_TYPE(wxEVT_MAXIMIZE)
DEFINE_EVENT_TYPE(wxEVT_MOUSE_CAPTURE_CHANGED)
DEFINE_EVENT_TYPE(wxEVT_PAINT)
DEFINE_EVENT_TYPE(wxEVT_ERASE_BACKGROUND)
DEFINE_EVENT_TYPE(wxEVT_NC_PAINT)
DEFINE_EVENT_TYPE(wxEVT_PAINT_ICON)
DEFINE_EVENT_TYPE(wxEVT_MENU_CHAR)
DEFINE_EVENT_TYPE(wxEVT_MENU_INIT)
DEFINE_EVENT_TYPE(wxEVT_MENU_HIGHLIGHT)
DEFINE_EVENT_TYPE(wxEVT_POPUP_MENU_INIT)
DEFINE_EVENT_TYPE(wxEVT_CONTEXT_MENU)
DEFINE_EVENT_TYPE(wxEVT_SYS_COLOUR_CHANGED)
DEFINE_EVENT_TYPE(wxEVT_SETTING_CHANGED)
DEFINE_EVENT_TYPE(wxEVT_QUERY_NEW_PALETTE)
DEFINE_EVENT_TYPE(wxEVT_PALETTE_CHANGED)
DEFINE_EVENT_TYPE(wxEVT_JOY_BUTTON_DOWN)
DEFINE_EVENT_TYPE(wxEVT_JOY_BUTTON_UP)
DEFINE_EVENT_TYPE(wxEVT_JOY_MOVE)
DEFINE_EVENT_TYPE(wxEVT_JOY_ZMOVE)
DEFINE_EVENT_TYPE(wxEVT_DROP_FILES)
DEFINE_EVENT_TYPE(wxEVT_DRAW_ITEM)
DEFINE_EVENT_TYPE(wxEVT_MEASURE_ITEM)
DEFINE_EVENT_TYPE(wxEVT_COMPARE_ITEM)
DEFINE_EVENT_TYPE(wxEVT_INIT_DIALOG)
DEFINE_EVENT_TYPE(wxEVT_IDLE)
DEFINE_EVENT_TYPE(wxEVT_UPDATE_UI)

// Generic command events
// Note: a click is a higher-level event than button down/up
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LEFT_CLICK)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LEFT_DCLICK)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_RIGHT_CLICK)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_RIGHT_DCLICK)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_SET_FOCUS)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_KILL_FOCUS)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_ENTER)

// Help events
DEFINE_EVENT_TYPE(wxEVT_HELP)
DEFINE_EVENT_TYPE(wxEVT_DETAILED_HELP)

#endif // !WXWIN_COMPATIBILITY_EVENT_TYPES

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// event initialization
// ----------------------------------------------------------------------------

int wxNewEventType()
{
    // MT-FIXME
    static int s_lastUsedEventType = wxEVT_FIRST;

#if WXWIN_COMPATIBILITY_2
    // check that we don't overlap with the user-defined types: if it does
    // happen, the best solution is probably to update the existing code to
    // use wxNewEventType() instead of wxEVT_USER_FIRST
    //
    // due to the uncertainty
    wxASSERT_MSG( s_lastUsedEventType < wxEVT_USER_FIRST - 1,
                  _T("possible event type conflict") );
#endif // WXWIN_COMPATIBILITY_2

    return s_lastUsedEventType++;
}

// ----------------------------------------------------------------------------
// wxEvent
// ----------------------------------------------------------------------------

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
    m_timeStamp = 0;
    m_id = theId;
    m_skipped = FALSE;
    m_callbackUserData = (wxObject *) NULL;
    m_isCommandEvent = FALSE;
}

void wxEvent::CopyObject(wxObject& object_dest) const
{
    wxEvent *obj = (wxEvent *)&object_dest;
    wxObject::CopyObject(object_dest);

    obj->m_eventType = m_eventType;
    obj->m_eventObject = m_eventObject;
    obj->m_timeStamp = m_timeStamp;
    obj->m_id = m_id;
    obj->m_skipped = m_skipped;
    obj->m_callbackUserData = m_callbackUserData;
    obj->m_isCommandEvent = m_isCommandEvent;
}

#if wxUSE_GUI

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
    m_commandString = wxEmptyString;
    m_isCommandEvent = TRUE;
}

void wxCommandEvent::CopyObject(wxObject& obj_d) const
{
    wxCommandEvent *obj = (wxCommandEvent *)&obj_d;

    wxEvent::CopyObject(obj_d);

    obj->m_clientData    = m_clientData;
    obj->m_clientObject  = m_clientObject;
    obj->m_extraLong     = m_extraLong;
    obj->m_commandInt    = m_commandInt;
    obj->m_commandString = m_commandString;
}

/*
 * Notify events
 */

void wxNotifyEvent::CopyObject(wxObject& obj_d) const
{
    wxNotifyEvent *obj = (wxNotifyEvent *)&obj_d;

    wxEvent::CopyObject(obj_d);

    if (!m_bAllow) obj->Veto();
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
 * ScrollWin events
 */

wxScrollWinEvent::wxScrollWinEvent(wxEventType commandType,
                                   int pos,
                                   int orient)
{
    m_eventType = commandType;
    m_extraLong = orient;
    m_commandInt = pos;
}

void wxScrollWinEvent::CopyObject(wxObject& obj_d) const
{
    wxScrollWinEvent *obj = (wxScrollWinEvent*)&obj_d;

    wxEvent::CopyObject(obj_d);

    obj->m_extraLong    = m_extraLong;
    obj->m_commandInt   = m_commandInt;
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
    m_wheelRotation = 0;
    m_wheelDelta = 0;
    m_linesPerAction = 0;
}

void wxMouseEvent::CopyObject(wxObject& obj_d) const
{
    wxMouseEvent *obj = (wxMouseEvent *)&obj_d;

    wxEvent::CopyObject(obj_d);

    obj->m_metaDown = m_metaDown;
    obj->m_altDown = m_altDown;
    obj->m_controlDown = m_controlDown;
    obj->m_shiftDown = m_shiftDown;
    obj->m_leftDown = m_leftDown;
    obj->m_rightDown = m_rightDown;
    obj->m_middleDown = m_middleDown;
    obj->m_x = m_x;
    obj->m_y = m_y;
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
            wxFAIL_MSG(wxT("invalid parameter in wxMouseEvent::ButtonDClick"));
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
            wxFAIL_MSG(wxT("invalid parameter in wxMouseEvent::ButtonDown"));
    }

    return FALSE;
}

// True if was a button up event (1 = left, 2 = middle, 3 = right)
// or any button up event (but = -1)
bool wxMouseEvent::ButtonUp(int but) const
{
    switch (but)
    {
        case -1:
            return (LeftUp() || MiddleUp() || RightUp());
        case 1:
            return LeftUp();
        case 2:
            return MiddleUp();
        case 3:
            return RightUp();
        default:
            wxFAIL_MSG(wxT("invalid parameter in wxMouseEvent::ButtonUp"));
    }

    return FALSE;
}

// True if the given button is currently changing state
bool wxMouseEvent::Button(int but) const
{
    switch (but)
    {
        case -1:
            return (ButtonUp(-1) || ButtonDown(-1) || ButtonDClick(-1));
        case 1:
            return (LeftDown() || LeftUp() || LeftDClick());
        case 2:
            return (MiddleDown() || MiddleUp() || MiddleDClick());
        case 3:
            return (RightDown() || RightUp() || RightDClick());
        default:
            wxFAIL_MSG(wxT("invalid parameter in wxMouseEvent::Button"));
    }

    return FALSE;
}

bool wxMouseEvent::ButtonIsDown(int but) const
{
    switch (but)
    {
        case -1:
            return (LeftIsDown() || MiddleIsDown() || RightIsDown());
        case 1:
            return LeftIsDown();
        case 2:
            return MiddleIsDown();
        case 3:
            return RightIsDown();
        default:
            wxFAIL_MSG(wxT("invalid parameter in wxMouseEvent::ButtonIsDown"));
    }

    return FALSE;
}

int wxMouseEvent::GetButton() const
{
    for ( int i = 1; i <= 3; i++ )
    {
        if ( Button(i) )
        {
            return i;
        }
    }

    return -1;
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
    m_scanCode = 0;
}

void wxKeyEvent::CopyObject(wxObject& obj_d) const
{
    wxKeyEvent *obj = (wxKeyEvent *)&obj_d;
    wxEvent::CopyObject(obj_d);

    obj->m_x = m_x;
    obj->m_y = m_y;
    obj->m_keyCode = m_keyCode;

    obj->m_shiftDown   = m_shiftDown;
    obj->m_controlDown = m_controlDown;
    obj->m_metaDown    = m_metaDown;
    obj->m_altDown     = m_altDown;
    obj->m_keyCode     = m_keyCode;
}


/*
 * Misc events
 */

void wxSizeEvent::CopyObject(wxObject& obj_d) const
{
    wxSizeEvent *obj = (wxSizeEvent *)&obj_d;
    wxEvent::CopyObject(obj_d);

    obj->m_size = m_size;
}

void wxMoveEvent::CopyObject(wxObject& obj_d) const
{
    wxMoveEvent *obj = (wxMoveEvent *)&obj_d;
    wxEvent::CopyObject(obj_d);

    obj->m_pos = m_pos;
}

void wxEraseEvent::CopyObject(wxObject& obj_d) const
{
    wxEraseEvent *obj = (wxEraseEvent *)&obj_d;
    wxEvent::CopyObject(obj_d);

    obj->m_dc = m_dc;
}

void wxActivateEvent::CopyObject(wxObject& obj_d) const
{
    wxActivateEvent *obj = (wxActivateEvent *)&obj_d;
    wxEvent::CopyObject(obj_d);

    obj->m_active = m_active;
}

void wxMenuEvent::CopyObject(wxObject& obj_d) const
{
    wxMenuEvent *obj = (wxMenuEvent *)&obj_d;
    wxEvent::CopyObject(obj_d);

    obj->m_menuId = m_menuId;
}

void wxCloseEvent::CopyObject(wxObject& obj_d) const
{
    wxCloseEvent *obj = (wxCloseEvent *)&obj_d;
    wxEvent::CopyObject(obj_d);

    obj->m_loggingOff = m_loggingOff;
    obj->m_veto = m_veto;
#if WXWIN_COMPATIBILITY
    obj->m_force = m_force;
#endif
    obj->m_canVeto = m_canVeto;
}

void wxShowEvent::CopyObject(wxObject& obj_d) const
{
    wxShowEvent *obj = (wxShowEvent *)&obj_d;
    wxEvent::CopyObject(obj_d);

    obj->m_show = m_show;
}

void wxJoystickEvent::CopyObject(wxObject& obj_d) const
{
    wxJoystickEvent *obj = (wxJoystickEvent *)&obj_d;
    wxEvent::CopyObject(obj_d);

    obj->m_pos = m_pos;
    obj->m_zPosition = m_zPosition;
    obj->m_buttonChange = m_buttonChange;
    obj->m_buttonState = m_buttonState;
    obj->m_joyStick = m_joyStick;
}

void wxDropFilesEvent::CopyObject(wxObject& obj_d) const
{
    wxDropFilesEvent *obj = (wxDropFilesEvent *)&obj_d;
    wxEvent::CopyObject(obj_d);

    obj->m_noFiles = m_noFiles;
    obj->m_pos = m_pos;
    // TODO: Problem with obj->m_files. It should be deallocated by the
    // destructor of the event.
}

void wxUpdateUIEvent::CopyObject(wxObject &obj_d) const
{
    wxUpdateUIEvent *obj = (wxUpdateUIEvent *)&obj_d;
    wxEvent::CopyObject(obj_d);

    obj->m_checked = m_checked;
    obj->m_enabled = m_enabled;
    obj->m_text = m_text;
    obj->m_setText = m_setText;
    obj->m_setChecked = m_setChecked;
    obj->m_setEnabled = m_setEnabled;
}

void wxPaletteChangedEvent::CopyObject(wxObject &obj_d) const
{
    wxPaletteChangedEvent *obj = (wxPaletteChangedEvent *)&obj_d;
    wxEvent::CopyObject(obj_d);

    obj->m_changedWindow = m_changedWindow;
}

void wxQueryNewPaletteEvent::CopyObject(wxObject& obj_d) const
{
    wxQueryNewPaletteEvent *obj = (wxQueryNewPaletteEvent *)&obj_d;
    wxEvent::CopyObject(obj_d);

    obj->m_paletteRealized = m_paletteRealized;
}

wxWindowCreateEvent::wxWindowCreateEvent(wxWindow *win)
{
    SetEventType(wxEVT_CREATE);
    SetEventObject(win);
}

wxWindowDestroyEvent::wxWindowDestroyEvent(wxWindow *win)
{
    SetEventType(wxEVT_DESTROY);
    SetEventObject(win);
}

void wxIdleEvent::CopyObject(wxObject& obj_d) const
{
    wxIdleEvent *obj = (wxIdleEvent *)&obj_d;
    wxEvent::CopyObject(obj_d);

    obj->m_requestMore = m_requestMore;
}

wxChildFocusEvent::wxChildFocusEvent(wxWindow *win)
                 : wxCommandEvent(wxEVT_CHILD_FOCUS)
{
    SetEventObject(win);
}

#endif // wxUSE_GUI

// ----------------------------------------------------------------------------
// wxEvtHandler
// ----------------------------------------------------------------------------

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
    m_pendingEvents = (wxList *) NULL;
#if wxUSE_THREADS
#  if !defined(__VISAGECPP__)
    m_eventsLocker = new wxCriticalSection;
#  endif
#endif
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
#if WXWIN_COMPATIBILITY_EVENT_TYPES
            wxEventTableEntry *entry = (wxEventTableEntry*)node->Data();
#else // !WXWIN_COMPATIBILITY_EVENT_TYPES
            wxDynamicEventTableEntry *entry = (wxDynamicEventTableEntry*)node->Data();
#endif // WXWIN_COMPATIBILITY_EVENT_TYPES/!WXWIN_COMPATIBILITY_EVENT_TYPES

            if (entry->m_callbackUserData)
                delete entry->m_callbackUserData;
            delete entry;
            node = node->Next();
        }
        delete m_dynamicEvents;
    };

    delete m_pendingEvents;

#if wxUSE_THREADS
#  if !defined(__VISAGECPP__)
    delete m_eventsLocker;
#  endif
#endif
}

#if wxUSE_THREADS

bool wxEvtHandler::ProcessThreadEvent(wxEvent& event)
{
    // check that we are really in a child thread
    wxASSERT_MSG( !wxThread::IsMain(),
                  wxT("use ProcessEvent() in main thread") );

    AddPendingEvent(event);

    return TRUE;
}

#endif // wxUSE_THREADS

void wxEvtHandler::AddPendingEvent(wxEvent& event)
{
    // 1) Add event to list of pending events of this event handler

#if defined(__VISAGECPP__)
    wxENTER_CRIT_SECT( m_eventsLocker);
#else
    wxENTER_CRIT_SECT( *m_eventsLocker);
#endif

    if ( !m_pendingEvents )
      m_pendingEvents = new wxList;

    wxEvent *event2 = (wxEvent *)event.Clone();

    m_pendingEvents->Append(event2);

#if defined(__VISAGECPP__)
    wxLEAVE_CRIT_SECT( m_eventsLocker);
#else
    wxLEAVE_CRIT_SECT( *m_eventsLocker);
#endif

    // 2) Add this event handler to list of event handlers that
    //    have pending events.

    wxENTER_CRIT_SECT(*wxPendingEventsLocker);

    if ( !wxPendingEvents )
        wxPendingEvents = new wxList;
    wxPendingEvents->Append(this);

    wxLEAVE_CRIT_SECT(*wxPendingEventsLocker);

    // 3) Inform the system that new pending events are somwehere,
    //    and that these should be processed in idle time.
    wxWakeUpIdle();
}

void wxEvtHandler::ProcessPendingEvents()
{
#if defined(__VISAGECPP__)
    wxENTER_CRIT_SECT( m_eventsLocker);
#else
    wxENTER_CRIT_SECT( *m_eventsLocker);
#endif

    wxNode *node = m_pendingEvents->First();
    while ( node )
    {
        wxEvent *event = (wxEvent *)node->Data();
        delete node;

        // In ProcessEvent, new events might get added and
        // we can safely leave the crtical section here.
#if defined(__VISAGECPP__)
        wxLEAVE_CRIT_SECT( m_eventsLocker);
#else
        wxLEAVE_CRIT_SECT( *m_eventsLocker);
#endif
        ProcessEvent(*event);
        delete event;
#if defined(__VISAGECPP__)
        wxENTER_CRIT_SECT( m_eventsLocker);
#else
        wxENTER_CRIT_SECT( *m_eventsLocker);
#endif

        node = m_pendingEvents->First();
    }

#if defined(__VISAGECPP__)
    wxLEAVE_CRIT_SECT( m_eventsLocker);
#else
    wxLEAVE_CRIT_SECT( *m_eventsLocker);
#endif
}

/*
 * Event table stuff
 */

bool wxEvtHandler::ProcessEvent(wxEvent& event)
{
#if wxUSE_GUI

    // We have to use the actual window or processing events from wxWindowNative
    // destructor won't work (we don't see the wxWindow class)
#ifdef __WXDEBUG__
    // check that our flag corresponds to reality
    wxClassInfo* info = NULL;
#ifdef __WXUNIVERSAL__
#  if defined(__WXMSW__)
    info = CLASSINFO(wxWindowMSW);
#  elif defined(__WXGTK__)
    info = CLASSINFO(wxWindowGTK);
#  elif defined(__WXMGL__)
    info = CLASSINFO(wxWindowMGL);
#  elif defined(__WXMAC__)
    info = CLASSINFO(wxWindowMac);
#  elif defined(__WXMOTIF__)
    info = CLASSINFO(wxWindowMotif);
#  endif
#else
    info = CLASSINFO(wxWindow);
#endif

    wxASSERT_MSG( m_isWindow == IsKindOf(info),
                  _T("this should [not] be a window but it is [not]") );
#endif

#endif // wxUSE_GUI

    // An event handler can be enabled or disabled
    if ( GetEvtHandlerEnabled() )
    {

#if 0
/*
        What is this? When using GUI threads, a non main
        threads can send an event and process it itself.
        This breaks GTK's GUI threads, so please explain.
*/

        // Check whether we are in a child thread.
        if ( !wxThread::IsMain() )
          return ProcessThreadEvent(event);
#endif

        // Handle per-instance dynamic event tables first
        if ( m_dynamicEvents && SearchDynamicEventTable(event) )
            return TRUE;

        // Then static per-class event tables
        const wxEventTable *table = GetEventTable();

#if wxUSE_GUI && wxUSE_VALIDATORS
        // Try the associated validator first, if this is a window.
        // Problem: if the event handler of the window has been replaced,
        // this wxEvtHandler may no longer be a window.
        // Therefore validators won't be processed if the handler
        // has been replaced with SetEventHandler.
        // THIS CAN BE CURED if PushEventHandler is used instead of
        // SetEventHandler, and then processing will be passed down the
        // chain of event handlers.
        if (m_isWindow)
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
#endif

        // Search upwards through the inheritance hierarchy
        while (table)
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

#if wxUSE_GUI
    // Carry on up the parent-child hierarchy, but only if event is a command
    // event: it wouldn't make sense for a parent to receive a child's size
    // event, for example
    if ( m_isWindow && event.IsCommandEvent() )
    {
        wxWindow *win = (wxWindow *)this;

        // also, don't propagate events beyond the first top level window: it
        // doesn't make sense to process dialogs events in the parent frame
        if ( !win->IsTopLevel() )
        {
            wxWindow *parent = win->GetParent();
            if (parent && !parent->IsBeingDeleted())
                return parent->GetEventHandler()->ProcessEvent(event);
        }
    }
#endif // wxUSE_GUI

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
    wxEventType eventType = event.GetEventType();
    int eventId = event.GetId();

    // BC++ doesn't like testing for m_fn without != 0
    for ( int i = 0; table.entries[i].m_fn != 0; i++ )
    {
        // the line using reference exposes a bug in gcc: although it _seems_
        // to work, it leads to weird crashes later on during program
        // execution
#ifdef __GNUG__
        wxEventTableEntry entry = table.entries[i];
#else
        const wxEventTableEntry& entry = table.entries[i];
#endif

        // match only if the event type is the same and the id is either -1 in
        // the event table (meaning "any") or the event id matches the id
        // specified in the event table either exactly or by falling into
        // range between first and last
        if ( eventType == entry.m_eventType )
        {
            int tableId1 = entry.m_id,
                tableId2 = entry.m_lastId;

            if ( (tableId1 == -1) ||
                 (tableId2 == -1 && eventId == tableId1) ||
                 (tableId2 != -1 &&
                    (eventId >= tableId1 && eventId <= tableId2)) )
            {
                event.Skip(FALSE);
                event.m_callbackUserData = entry.m_callbackUserData;

                (this->*((wxEventFunction) (entry.m_fn)))(event);

                return !event.GetSkipped();
            }
        }
    }

    return FALSE;
}

void wxEvtHandler::Connect( int id, int lastId,
                            int eventType,
                            wxObjectEventFunction func,
                            wxObject *userData )
{
#if WXWIN_COMPATIBILITY_EVENT_TYPES
    wxEventTableEntry *entry = new wxEventTableEntry;
    entry->m_eventType = eventType;
    entry->m_id = id;
    entry->m_lastId = lastId;
    entry->m_fn = func;
    entry->m_callbackUserData = userData;
#else // !WXWIN_COMPATIBILITY_EVENT_TYPES
    wxDynamicEventTableEntry *entry =
        new wxDynamicEventTableEntry(eventType, id, lastId, func, userData);
#endif // WXWIN_COMPATIBILITY_EVENT_TYPES/!WXWIN_COMPATIBILITY_EVENT_TYPES

    if (!m_dynamicEvents)
        m_dynamicEvents = new wxList;

    m_dynamicEvents->Append( (wxObject*) entry );
}

bool wxEvtHandler::Disconnect( int id, int lastId, wxEventType eventType,
                  wxObjectEventFunction func,
                  wxObject *userData )
{
    if (!m_dynamicEvents)
        return FALSE;

    wxNode *node = m_dynamicEvents->First();
    while (node)
    {
#if WXWIN_COMPATIBILITY_EVENT_TYPES
            wxEventTableEntry *entry = (wxEventTableEntry*)node->Data();
#else // !WXWIN_COMPATIBILITY_EVENT_TYPES
            wxDynamicEventTableEntry *entry = (wxDynamicEventTableEntry*)node->Data();
#endif // WXWIN_COMPATIBILITY_EVENT_TYPES/!WXWIN_COMPATIBILITY_EVENT_TYPES

        if ((entry->m_id == id) &&
            ((entry->m_lastId == lastId) || (lastId == -1)) &&
            ((entry->m_eventType == eventType) || (eventType == wxEVT_NULL)) &&
            ((entry->m_fn == func) || (func == (wxObjectEventFunction)NULL)) &&
            ((entry->m_callbackUserData == userData) || (userData == (wxObject*)NULL)))
        {
            if (entry->m_callbackUserData)
                delete entry->m_callbackUserData;
            m_dynamicEvents->DeleteNode( node );
            delete entry;
            return TRUE;
        }
        node = node->Next();
    }
    return FALSE;
}

bool wxEvtHandler::SearchDynamicEventTable( wxEvent& event )
{
    wxCHECK_MSG( m_dynamicEvents, FALSE,
                 wxT("caller should check that we have dynamic events") );

    int commandId = event.GetId();

    wxNode *node = m_dynamicEvents->First();
    while (node)
    {
#if WXWIN_COMPATIBILITY_EVENT_TYPES
            wxEventTableEntry *entry = (wxEventTableEntry*)node->Data();
#else // !WXWIN_COMPATIBILITY_EVENT_TYPES
            wxDynamicEventTableEntry *entry = (wxDynamicEventTableEntry*)node->Data();
#endif // WXWIN_COMPATIBILITY_EVENT_TYPES/!WXWIN_COMPATIBILITY_EVENT_TYPES

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

#if wxUSE_GUI

// Find a window with the focus, that is also a descendant of the given window.
// This is used to determine the window to initially send commands to.
wxWindow* wxFindFocusDescendant(wxWindow* ancestor)
{
    // Process events starting with the window with the focus, if any.
    wxWindow* focusWin = wxWindow::FindFocus();
    wxWindow* win = focusWin;

    // Check if this is a descendant of this frame.
    // If not, win will be set to NULL.
    while (win)
    {
        if (win == ancestor)
            break;
        else
            win = win->GetParent();
    }
    if (win == (wxWindow*) NULL)
        focusWin = (wxWindow*) NULL;

    return focusWin;
}

#endif // wxUSE_GUI
