/////////////////////////////////////////////////////////////////////////////
// Name:        event.h
// Purpose:     Event classes
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id$
// Copyright:   (c)
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_EVENTH__
#define _WX_EVENTH__

#ifdef __GNUG__
#pragma interface "event.h"
#endif

#include "wx/defs.h"
#include "wx/object.h"
#include "wx/gdicmn.h"

/*
 * Event types
 *
 */

typedef int wxEventType;

const wxEventType wxEVT_NULL = 0;
const wxEventType wxEVT_FIRST = 10000;

const wxEventType wxEVT_COMMAND_BUTTON_CLICKED =            wxEVT_FIRST + 1;
const wxEventType wxEVT_COMMAND_CHECKBOX_CLICKED =          wxEVT_FIRST + 2;
const wxEventType wxEVT_COMMAND_CHOICE_SELECTED =           wxEVT_FIRST + 3;
const wxEventType wxEVT_COMMAND_LISTBOX_SELECTED =          wxEVT_FIRST + 4;
const wxEventType wxEVT_COMMAND_LISTBOX_DOUBLECLICKED =     wxEVT_FIRST + 5;
const wxEventType wxEVT_COMMAND_CHECKLISTBOX_TOGGLED =      wxEVT_FIRST + 6;
const wxEventType wxEVT_COMMAND_TEXT_UPDATED =              wxEVT_FIRST + 7;
const wxEventType wxEVT_COMMAND_TEXT_ENTER =                wxEVT_FIRST + 8;
const wxEventType wxEVT_COMMAND_MENU_SELECTED =             wxEVT_FIRST + 9;
const wxEventType wxEVT_COMMAND_TOOL_CLICKED =              wxEVT_COMMAND_MENU_SELECTED;
const wxEventType wxEVT_COMMAND_SLIDER_UPDATED =            wxEVT_FIRST + 10;
const wxEventType wxEVT_COMMAND_RADIOBOX_SELECTED =         wxEVT_FIRST + 11;
const wxEventType wxEVT_COMMAND_RADIOBUTTON_SELECTED =      wxEVT_FIRST + 12;
//const wxEventType wxEVT_COMMAND_SCROLLBAR_UPDATED is now obsolete since we use wxEVT_SCROLL... events
const wxEventType wxEVT_COMMAND_SCROLLBAR_UPDATED =         wxEVT_FIRST + 13;
const wxEventType wxEVT_COMMAND_VLBOX_SELECTED =            wxEVT_FIRST + 14;
const wxEventType wxEVT_COMMAND_COMBOBOX_SELECTED =         wxEVT_FIRST + 15;
const wxEventType wxEVT_COMMAND_TOOL_RCLICKED =             wxEVT_FIRST + 16;
const wxEventType wxEVT_COMMAND_TOOL_ENTER =                wxEVT_FIRST + 17;

/* Mouse event types */
const wxEventType wxEVT_LEFT_DOWN =                         wxEVT_FIRST + 100;
const wxEventType wxEVT_LEFT_UP =                           wxEVT_FIRST + 101;
const wxEventType wxEVT_MIDDLE_DOWN =                       wxEVT_FIRST + 102;
const wxEventType wxEVT_MIDDLE_UP =                         wxEVT_FIRST + 103;
const wxEventType wxEVT_RIGHT_DOWN =                        wxEVT_FIRST + 104;
const wxEventType wxEVT_RIGHT_UP =                          wxEVT_FIRST + 105;
const wxEventType wxEVT_MOTION =                            wxEVT_FIRST + 106;
const wxEventType wxEVT_ENTER_WINDOW =                      wxEVT_FIRST + 107;
const wxEventType wxEVT_LEAVE_WINDOW =                      wxEVT_FIRST + 108;
const wxEventType wxEVT_LEFT_DCLICK =                       wxEVT_FIRST + 109;
const wxEventType wxEVT_MIDDLE_DCLICK =                     wxEVT_FIRST + 110;
const wxEventType wxEVT_RIGHT_DCLICK =                      wxEVT_FIRST + 111;
const wxEventType wxEVT_SET_FOCUS =                         wxEVT_FIRST + 112;
const wxEventType wxEVT_KILL_FOCUS =                        wxEVT_FIRST + 113;

 // Non-client mouse events
const wxEventType wxEVT_NC_LEFT_DOWN =                      wxEVT_FIRST + 200;
const wxEventType wxEVT_NC_LEFT_UP =                        wxEVT_FIRST + 201;
const wxEventType wxEVT_NC_MIDDLE_DOWN =                    wxEVT_FIRST + 202;
const wxEventType wxEVT_NC_MIDDLE_UP =                      wxEVT_FIRST + 203;
const wxEventType wxEVT_NC_RIGHT_DOWN =                     wxEVT_FIRST + 204;
const wxEventType wxEVT_NC_RIGHT_UP =                       wxEVT_FIRST + 205;
const wxEventType wxEVT_NC_MOTION =                         wxEVT_FIRST + 206;
const wxEventType wxEVT_NC_ENTER_WINDOW =                   wxEVT_FIRST + 207;
const wxEventType wxEVT_NC_LEAVE_WINDOW =                   wxEVT_FIRST + 208;
const wxEventType wxEVT_NC_LEFT_DCLICK =                    wxEVT_FIRST + 209;
const wxEventType wxEVT_NC_MIDDLE_DCLICK =                  wxEVT_FIRST + 210;
const wxEventType wxEVT_NC_RIGHT_DCLICK =                   wxEVT_FIRST + 211;

/* Character input event type  */
const wxEventType wxEVT_CHAR =                              wxEVT_FIRST + 212;
const wxEventType wxEVT_NAVIGATION_KEY =                    wxEVT_FIRST + 213;

 /*
  * Scrollbar event identifiers
  */
const wxEventType wxEVT_SCROLL_TOP =                        wxEVT_FIRST + 300;
const wxEventType wxEVT_SCROLL_BOTTOM =                     wxEVT_FIRST + 301;
const wxEventType wxEVT_SCROLL_LINEUP =                     wxEVT_FIRST + 302;
const wxEventType wxEVT_SCROLL_LINEDOWN =                   wxEVT_FIRST + 303;
const wxEventType wxEVT_SCROLL_PAGEUP =                     wxEVT_FIRST + 304;
const wxEventType wxEVT_SCROLL_PAGEDOWN =                   wxEVT_FIRST + 305;
const wxEventType wxEVT_SCROLL_THUMBTRACK =                 wxEVT_FIRST + 306;

const wxEventType wxEVT_SIZE =                              wxEVT_FIRST + 400;
const wxEventType wxEVT_MOVE =                              wxEVT_FIRST + 401;
const wxEventType wxEVT_CLOSE_WINDOW =                      wxEVT_FIRST + 402;
const wxEventType wxEVT_END_SESSION =                       wxEVT_FIRST + 403;
const wxEventType wxEVT_QUERY_END_SESSION =                 wxEVT_FIRST + 404;
const wxEventType wxEVT_ACTIVATE_APP =                      wxEVT_FIRST + 405;
const wxEventType wxEVT_POWER =                             wxEVT_FIRST + 406;
const wxEventType wxEVT_CHAR_HOOK =                         wxEVT_FIRST + 407;
const wxEventType wxEVT_KEY_UP =                            wxEVT_FIRST + 408;
const wxEventType wxEVT_ACTIVATE =                          wxEVT_FIRST + 409;
const wxEventType wxEVT_CREATE =                            wxEVT_FIRST + 410;
const wxEventType wxEVT_DESTROY =                           wxEVT_FIRST + 411;
const wxEventType wxEVT_SHOW =                              wxEVT_FIRST + 412;
const wxEventType wxEVT_ICONIZE =                           wxEVT_FIRST + 413;
const wxEventType wxEVT_MAXIMIZE =                          wxEVT_FIRST + 414;
const wxEventType wxEVT_MOUSE_CAPTURE_CHANGED =             wxEVT_FIRST + 415;
const wxEventType wxEVT_PAINT =                             wxEVT_FIRST + 416;
const wxEventType wxEVT_ERASE_BACKGROUND =                  wxEVT_FIRST + 417;
const wxEventType wxEVT_NC_PAINT =                          wxEVT_FIRST + 418;
const wxEventType wxEVT_PAINT_ICON =                        wxEVT_FIRST + 419;
const wxEventType wxEVT_MENU_CHAR =                         wxEVT_FIRST + 420;
const wxEventType wxEVT_MENU_INIT =                         wxEVT_FIRST + 421;
const wxEventType wxEVT_MENU_HIGHLIGHT =                    wxEVT_FIRST + 422;
const wxEventType wxEVT_POPUP_MENU_INIT =                   wxEVT_FIRST + 423;
const wxEventType wxEVT_CONTEXT_MENU =                      wxEVT_FIRST + 424;
const wxEventType wxEVT_SYS_COLOUR_CHANGED =                wxEVT_FIRST + 425;
const wxEventType wxEVT_SETTING_CHANGED =                   wxEVT_FIRST + 426;
const wxEventType wxEVT_QUERY_NEW_PALETTE =                 wxEVT_FIRST + 427;
const wxEventType wxEVT_PALETTE_CHANGED =                   wxEVT_FIRST + 428;
const wxEventType wxEVT_JOY_BUTTON_DOWN =                   wxEVT_FIRST + 429;
const wxEventType wxEVT_JOY_BUTTON_UP =                     wxEVT_FIRST + 430;
const wxEventType wxEVT_JOY_MOVE =                          wxEVT_FIRST + 431;
const wxEventType wxEVT_JOY_ZMOVE =                         wxEVT_FIRST + 432;
const wxEventType wxEVT_DROP_FILES =                        wxEVT_FIRST + 433;
const wxEventType wxEVT_DRAW_ITEM =                         wxEVT_FIRST + 434;
const wxEventType wxEVT_MEASURE_ITEM =                      wxEVT_FIRST + 435;
const wxEventType wxEVT_COMPARE_ITEM =                      wxEVT_FIRST + 436;
const wxEventType wxEVT_INIT_DIALOG =                       wxEVT_FIRST + 437;
const wxEventType wxEVT_IDLE =                              wxEVT_FIRST + 438;
const wxEventType wxEVT_UPDATE_UI =                         wxEVT_FIRST + 439;
 /* System misc. */
const wxEventType wxEVT_END_PROCESS =                       wxEVT_FIRST + 440;

 /* Generic command events */
 // Note: a click is a higher-level event
 // than button down/up
const wxEventType wxEVT_COMMAND_LEFT_CLICK =                wxEVT_FIRST + 500;
const wxEventType wxEVT_COMMAND_LEFT_DCLICK =               wxEVT_FIRST + 501;
const wxEventType wxEVT_COMMAND_RIGHT_CLICK =               wxEVT_FIRST + 502;
const wxEventType wxEVT_COMMAND_RIGHT_DCLICK =              wxEVT_FIRST + 503;
const wxEventType wxEVT_COMMAND_SET_FOCUS =                 wxEVT_FIRST + 504;
const wxEventType wxEVT_COMMAND_KILL_FOCUS =                wxEVT_FIRST + 505;
const wxEventType wxEVT_COMMAND_ENTER =                     wxEVT_FIRST + 506;

 /* Tree control event types */
const wxEventType wxEVT_COMMAND_TREE_BEGIN_DRAG =           wxEVT_FIRST + 600;
const wxEventType wxEVT_COMMAND_TREE_BEGIN_RDRAG =          wxEVT_FIRST + 601;
const wxEventType wxEVT_COMMAND_TREE_BEGIN_LABEL_EDIT =     wxEVT_FIRST + 602;
const wxEventType wxEVT_COMMAND_TREE_END_LABEL_EDIT =       wxEVT_FIRST + 603;
const wxEventType wxEVT_COMMAND_TREE_DELETE_ITEM =          wxEVT_FIRST + 604;
const wxEventType wxEVT_COMMAND_TREE_GET_INFO =             wxEVT_FIRST + 605;
const wxEventType wxEVT_COMMAND_TREE_SET_INFO =             wxEVT_FIRST + 606;
const wxEventType wxEVT_COMMAND_TREE_ITEM_EXPANDED =        wxEVT_FIRST + 607;
const wxEventType wxEVT_COMMAND_TREE_ITEM_EXPANDING =       wxEVT_FIRST + 608;
const wxEventType wxEVT_COMMAND_TREE_ITEM_COLLAPSED =       wxEVT_FIRST + 609;
const wxEventType wxEVT_COMMAND_TREE_ITEM_COLLAPSING =      wxEVT_FIRST + 610;
const wxEventType wxEVT_COMMAND_TREE_SEL_CHANGED =          wxEVT_FIRST + 611;
const wxEventType wxEVT_COMMAND_TREE_SEL_CHANGING =         wxEVT_FIRST + 612;
const wxEventType wxEVT_COMMAND_TREE_KEY_DOWN =             wxEVT_FIRST + 613;
const wxEventType wxEVT_COMMAND_TREE_ITEM_ACTIVATED =       wxEVT_FIRST + 614;

 /* List control event types */
const wxEventType wxEVT_COMMAND_LIST_BEGIN_DRAG =           wxEVT_FIRST + 700;
const wxEventType wxEVT_COMMAND_LIST_BEGIN_RDRAG =          wxEVT_FIRST + 701;
const wxEventType wxEVT_COMMAND_LIST_BEGIN_LABEL_EDIT =     wxEVT_FIRST + 702;
const wxEventType wxEVT_COMMAND_LIST_END_LABEL_EDIT =       wxEVT_FIRST + 703;
const wxEventType wxEVT_COMMAND_LIST_DELETE_ITEM =          wxEVT_FIRST + 704;
const wxEventType wxEVT_COMMAND_LIST_DELETE_ALL_ITEMS =     wxEVT_FIRST + 705;
const wxEventType wxEVT_COMMAND_LIST_GET_INFO =             wxEVT_FIRST + 706;
const wxEventType wxEVT_COMMAND_LIST_SET_INFO =             wxEVT_FIRST + 707;
const wxEventType wxEVT_COMMAND_LIST_ITEM_SELECTED =        wxEVT_FIRST + 708;
const wxEventType wxEVT_COMMAND_LIST_ITEM_DESELECTED =      wxEVT_FIRST + 709;
const wxEventType wxEVT_COMMAND_LIST_KEY_DOWN =             wxEVT_FIRST + 710;
const wxEventType wxEVT_COMMAND_LIST_INSERT_ITEM =          wxEVT_FIRST + 711;
const wxEventType wxEVT_COMMAND_LIST_COL_CLICK =            wxEVT_FIRST + 712;
const wxEventType wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK =     wxEVT_FIRST + 713;
const wxEventType wxEVT_COMMAND_LIST_ITEM_MIDDLE_CLICK =    wxEVT_FIRST + 714;
const wxEventType wxEVT_COMMAND_LIST_ITEM_ACTIVATED =       wxEVT_FIRST + 715;

 /* Tab and notebook control event types */
const wxEventType wxEVT_COMMAND_TAB_SEL_CHANGED =           wxEVT_FIRST + 800;
const wxEventType wxEVT_COMMAND_TAB_SEL_CHANGING =          wxEVT_FIRST + 801;
const wxEventType wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED =     wxEVT_FIRST + 802;
const wxEventType wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGING =    wxEVT_FIRST + 803;

const wxEventType wxEVT_USER_FIRST =                        wxEVT_FIRST + 2000;

// Compatibility

#if WXWIN_COMPATIBILITY

#define wxEVENT_TYPE_BUTTON_COMMAND             wxEVT_COMMAND_BUTTON_CLICKED
#define wxEVENT_TYPE_CHECKBOX_COMMAND           wxEVT_COMMAND_CHECKBOX_CLICKED
#define wxEVENT_TYPE_CHOICE_COMMAND             wxEVT_COMMAND_CHOICE_SELECTED
#define wxEVENT_TYPE_LISTBOX_COMMAND            wxEVT_COMMAND_LISTBOX_SELECTED
#define wxEVENT_TYPE_LISTBOX_DCLICK_COMMAND     wxEVT_COMMAND_LISTBOX_DOUBLECLICKED
#define wxEVENT_TYPE_TEXT_COMMAND               wxEVT_COMMAND_TEXT_UPDATED
#define wxEVENT_TYPE_MULTITEXT_COMMAND          wxEVT_COMMAND_TEXT_UPDATED
#define wxEVENT_TYPE_MENU_COMMAND               wxEVT_COMMAND_MENU_SELECTED
#define wxEVENT_TYPE_SLIDER_COMMAND             wxEVT_COMMAND_SLIDER_UPDATED
#define wxEVENT_TYPE_RADIOBOX_COMMAND           wxEVT_COMMAND_RADIOBOX_SELECTED
#define wxEVENT_TYPE_RADIOBUTTON_COMMAND        wxEVT_COMMAND_RADIOBUTTON_SELECTED
#define wxEVENT_TYPE_TEXT_ENTER_COMMAND         wxEVT_COMMAND_TEXT_ENTER
#define wxEVENT_TYPE_SET_FOCUS                  wxEVT_SET_FOCUS
#define wxEVENT_TYPE_KILL_FOCUS                 wxEVT_KILL_FOCUS
#define wxEVENT_TYPE_SCROLLBAR_COMMAND          wxEVT_COMMAND_SCROLLBAR_UPDATED
#define wxEVENT_TYPE_VIRT_LISTBOX_COMMAND       wxEVT_COMMAND_VLBOX_SELECTED
#define wxEVENT_TYPE_COMBOBOX_COMMAND           wxEVT_COMMAND_COMBOBOX_SELECTED

#define wxEVENT_TYPE_LEFT_DOWN                  wxEVT_LEFT_DOWN
#define wxEVENT_TYPE_LEFT_UP                    wxEVT_LEFT_UP
#define wxEVENT_TYPE_MIDDLE_DOWN                wxEVT_MIDDLE_DOWN
#define wxEVENT_TYPE_MIDDLE_UP                  wxEVT_MIDDLE_UP
#define wxEVENT_TYPE_RIGHT_DOWN                 wxEVT_RIGHT_DOWN
#define wxEVENT_TYPE_RIGHT_UP                   wxEVT_RIGHT_UP
#define wxEVENT_TYPE_MOTION                     wxEVT_MOTION
#define wxEVENT_TYPE_ENTER_WINDOW               wxEVT_ENTER_WINDOW
#define wxEVENT_TYPE_LEAVE_WINDOW               wxEVT_LEAVE_WINDOW
#define wxEVENT_TYPE_LEFT_DCLICK                wxEVT_LEFT_DCLICK
#define wxEVENT_TYPE_MIDDLE_DCLICK              wxEVT_MIDDLE_DCLICK
#define wxEVENT_TYPE_RIGHT_DCLICK               wxEVT_RIGHT_DCLICK
#define wxEVENT_TYPE_CHAR                       wxEVT_CHAR
#define wxEVENT_TYPE_SCROLL_TOP                 wxEVT_SCROLL_TOP
#define wxEVENT_TYPE_SCROLL_BOTTOM              wxEVT_SCROLL_BOTTOM
#define wxEVENT_TYPE_SCROLL_LINEUP              wxEVT_SCROLL_LINEUP
#define wxEVENT_TYPE_SCROLL_LINEDOWN            wxEVT_SCROLL_LINEDOWN
#define wxEVENT_TYPE_SCROLL_PAGEUP              wxEVT_SCROLL_PAGEUP
#define wxEVENT_TYPE_SCROLL_PAGEDOWN            wxEVT_SCROLL_PAGEDOWN
#define wxEVENT_TYPE_SCROLL_THUMBTRACK          wxEVT_SCROLL_THUMBTRACK

#endif

/*
 * wxWindows events, covering all interesting things that might happen
 * (button clicking, resizing, setting text in widgets, etc.).
 *
 * For each completely new event type, derive a new event class.
 * An event CLASS represents a C++ class defining a range of similar event TYPES;
 * examples are canvas events, panel item command events.
 * An event TYPE is a unique identifier for a particular system event,
 * such as a button press or a listbox deselection.
 *
 */

class WXDLLEXPORT wxEvent: public wxObject
{
  DECLARE_ABSTRACT_CLASS(wxEvent)

public:
  wxEvent(int id = 0);
  inline ~wxEvent(void) {}

  inline void SetEventType(wxEventType typ) { m_eventType = typ; }
  inline wxEventType GetEventType(void) const { return m_eventType; }
  inline wxObject *GetEventObject(void) const { return m_eventObject; }
  inline void SetEventObject(wxObject *obj) { m_eventObject = obj; }
  inline long GetTimestamp(void) const { return m_timeStamp; }
  inline void SetTimestamp(long ts = 0) { m_timeStamp = ts; }
  inline int GetId() const { return m_id; }
  inline void SetId(int Id) { m_id = Id; }

  // Can instruct event processor that we wish to ignore this event
  // (treat as if the event table entry had not been found)
  inline void Skip(bool skip = TRUE) { m_skipped = skip; }
  inline bool GetSkipped(void) const { return m_skipped; };

public:
  bool              m_skipped;
  wxObject*         m_eventObject;
  char*             m_eventHandle;         // Handle of an underlying windowing system event
  wxEventType            m_eventType;
  long              m_timeStamp;
  int               m_id;
  wxObject*         m_callbackUserData;

};

// Item or menu event class
/*
 wxEVT_COMMAND_BUTTON_CLICKED
 wxEVT_COMMAND_CHECKBOX_CLICKED
 wxEVT_COMMAND_CHOICE_SELECTED
 wxEVT_COMMAND_LISTBOX_SELECTED
 wxEVT_COMMAND_LISTBOX_DOUBLECLICKED
 wxEVT_COMMAND_TEXT_UPDATED
 wxEVT_COMMAND_TEXT_ENTER
 wxEVT_COMMAND_MENU_SELECTED
 wxEVT_COMMAND_SLIDER_UPDATED
 wxEVT_COMMAND_RADIOBOX_SELECTED
 wxEVT_COMMAND_RADIOBUTTON_SELECTED
 wxEVT_COMMAND_SCROLLBAR_UPDATED
 wxEVT_COMMAND_VLBOX_SELECTED
 wxEVT_COMMAND_COMBOBOX_SELECTED
*/

class WXDLLEXPORT wxClientData;

class WXDLLEXPORT wxCommandEvent: public wxEvent
{
  DECLARE_DYNAMIC_CLASS(wxCommandEvent)

  wxCommandEvent(wxEventType commandType = wxEVT_NULL, int id = 0);
  inline ~wxCommandEvent(void) {}

  /*
   * Accessors dependent on context
   *
   */

  // Set/Get client data from controls
  inline void SetClientData(void* clientData) { m_clientData = clientData; }
  inline void *GetClientData() const { return m_clientData; }

  // Set/Get client object from controls
  inline void SetClientObject(wxClientData* clientObject) { m_clientObject = clientObject; }
  inline void *GetClientObject() const { return m_clientObject; }

  // Get listbox selection if single-choice
  inline int GetSelection() const { return m_commandInt; }

  // Set/Get listbox/choice selection string
  inline void SetString(char* s) { m_commandString = s; }
  inline char *GetString() const { return m_commandString; }

  // Get checkbox value
  inline bool Checked() const { return (m_commandInt != 0); }

  // TRUE if the listbox event was a selection.
  inline bool IsSelection() const { return (m_extraLong != 0); }

  inline void SetExtraLong(long extraLong) { m_extraLong = extraLong; }
  inline long GetExtraLong(void) const { return m_extraLong ; }

  inline void SetInt(int i) { m_commandInt = i; }
  inline long GetInt(void) const { return m_commandInt ; }

 public:
  char*             m_commandString; // String event argument
  int               m_commandInt;
  long              m_extraLong;     // Additional information (e.g. select/deselect)
  void*             m_clientData;    // Arbitrary client data
  wxClientData*     m_clientObject;  // Arbitrary client object
};

// this class adds a possibility to react (from the user) code to a control
// notification: allow or veto the operation being reported.
class WXDLLEXPORT wxNotifyEvent : public wxCommandEvent
{
public:
    wxNotifyEvent(wxEventType commandType = wxEVT_NULL, int id = 0)
                : wxCommandEvent(commandType, id) { m_bAllow = TRUE; }

    // veto the operation (by default it's allowed)
    void Veto() { m_bAllow = FALSE; }

    // for implementation code only: is the operation allowed?
    bool IsAllowed() const { return m_bAllow; }

private:
    bool m_bAllow;

    DECLARE_DYNAMIC_CLASS(wxNotifyEvent)
};

// Scroll event class
/*
 wxEVT_SCROLL_TOP
 wxEVT_SCROLL_BOTTOM
 wxEVT_SCROLL_LINEUP
 wxEVT_SCROLL_LINEDOWN
 wxEVT_SCROLL_PAGEUP
 wxEVT_SCROLL_PAGEDOWN
 wxEVT_SCROLL_THUMBTRACK
*/

class WXDLLEXPORT wxScrollEvent: public wxCommandEvent
{
  DECLARE_DYNAMIC_CLASS(wxScrollEvent)

 public:
  wxScrollEvent(wxEventType commandType = wxEVT_NULL, int id = 0, int pos = 0, int orient = 0);
  inline ~wxScrollEvent(void) {}

  /*
   * Accessors
   *
   */

  inline int GetOrientation(void) const { return (int) m_extraLong ; }
  inline int GetPosition(void) const { return m_commandInt ; }
  inline void SetOrientation(int orient) { m_extraLong = (long) orient; }
  inline void SetPosition(int pos) { m_commandInt = pos; }
};

// Mouse event class

/*
 wxEVT_LEFT_DOWN
 wxEVT_LEFT_UP
 wxEVT_MIDDLE_DOWN
 wxEVT_MIDDLE_UP
 wxEVT_RIGHT_DOWN
 wxEVT_RIGHT_UP
 wxEVT_MOTION
 wxEVT_ENTER_WINDOW
 wxEVT_LEAVE_WINDOW
 wxEVT_LEFT_DCLICK
 wxEVT_MIDDLE_DCLICK
 wxEVT_RIGHT_DCLICK
 wxEVT_NC_LEFT_DOWN
 wxEVT_NC_LEFT_UP,
 wxEVT_NC_MIDDLE_DOWN,
 wxEVT_NC_MIDDLE_UP,
 wxEVT_NC_RIGHT_DOWN,
 wxEVT_NC_RIGHT_UP,
 wxEVT_NC_MOTION,
 wxEVT_NC_ENTER_WINDOW,
 wxEVT_NC_LEAVE_WINDOW,
 wxEVT_NC_LEFT_DCLICK,
 wxEVT_NC_MIDDLE_DCLICK,
 wxEVT_NC_RIGHT_DCLICK,
*/

class WXDLLEXPORT wxDC;
class WXDLLEXPORT wxMouseEvent: public wxEvent
{
  DECLARE_DYNAMIC_CLASS(wxMouseEvent)

 public:
  wxMouseEvent(wxEventType mouseType = wxEVT_NULL);

  // Was it a button event? (*doesn't* mean: is any button *down*?)
  inline bool IsButton(void) const { return Button(-1); }

  // Was it a down event from button 1, 2 or 3 or any?
  bool ButtonDown(int but = -1) const;

  // Was it a dclick event from button 1, 2 or 3 or any?
  bool ButtonDClick(int but = -1) const;

  // Was it a up event from button 1, 2 or 3 or any?
  bool ButtonUp(int but = -1) const;

  // Was the given button 1,2,3 or any changing state?
  bool Button(int but) const;

  // Was the given button 1,2,3 or any in Down state?
  bool ButtonIsDown(int but) const;

  // Find state of shift/control keys
  inline bool ControlDown(void) const { return m_controlDown; }
  inline bool MetaDown(void) const { return m_metaDown; }
  inline bool AltDown(void) const { return m_altDown; }
  inline bool ShiftDown(void) const { return m_shiftDown; }

  // Find which event was just generated
  inline bool LeftDown(void) const { return (m_eventType == wxEVT_LEFT_DOWN); }
  inline bool MiddleDown(void) const { return (m_eventType == wxEVT_MIDDLE_DOWN); }
  inline bool RightDown(void) const { return (m_eventType == wxEVT_RIGHT_DOWN); }

  inline bool LeftUp(void) const { return (m_eventType == wxEVT_LEFT_UP); }
  inline bool MiddleUp(void) const { return (m_eventType == wxEVT_MIDDLE_UP); }
  inline bool RightUp(void) const { return (m_eventType == wxEVT_RIGHT_UP); }

  inline bool LeftDClick(void) const { return (m_eventType == wxEVT_LEFT_DCLICK); }
  inline bool MiddleDClick(void) const { return (m_eventType == wxEVT_MIDDLE_DCLICK); }
  inline bool RightDClick(void) const { return (m_eventType == wxEVT_RIGHT_DCLICK); }

  // Find the current state of the mouse buttons (regardless
  // of current event type)
  inline bool LeftIsDown(void) const { return m_leftDown; }
  inline bool MiddleIsDown(void) const { return m_middleDown; }
  inline bool RightIsDown(void) const { return m_rightDown; }

  // True if a button is down and the mouse is moving
  inline bool Dragging(void) const { return ((m_eventType == wxEVT_MOTION) && (LeftIsDown() || MiddleIsDown() || RightIsDown())); }

  // True if the mouse is moving, and no button is down
  inline bool Moving(void) const { return (m_eventType == wxEVT_MOTION); }

  // True if the mouse is just entering the window
  inline bool Entering(void) const { return (m_eventType == wxEVT_ENTER_WINDOW); }

  // True if the mouse is just leaving the window
  inline bool Leaving(void) const { return (m_eventType == wxEVT_LEAVE_WINDOW); }

  // Find the position of the event
  inline void Position(long *xpos, long *ypos) const { *xpos = m_x; *ypos = m_y; }

  // Find the position of the event
  inline wxPoint GetPosition() const { return wxPoint(m_x, m_y); }

  // Find the logical position of the event given the DC
  wxPoint GetLogicalPosition(const wxDC& dc) const ;

  // Compatibility
  inline void Position(float *xpos, float *ypos) const { *xpos = (float) m_x; *ypos = (float) m_y; }

  // Get X position
  inline long GetX(void) const { return m_x; }

  // Get Y position
  inline long GetY(void) const { return m_y; }

public:
  long          m_x;
  long          m_y;
  bool          m_leftDown;
  bool          m_middleDown;
  bool          m_rightDown;

  bool          m_controlDown;
  bool          m_shiftDown;
  bool          m_altDown;
  bool          m_metaDown;

};

// Keyboard input event class

/*
 wxEVT_CHAR
 wxEVT_CHAR_HOOK
 wxEVT_KEY_UP
 */

class WXDLLEXPORT wxKeyEvent: public wxEvent
{
  DECLARE_DYNAMIC_CLASS(wxKeyEvent)

public:
  wxKeyEvent(wxEventType keyType = wxEVT_NULL);

  // Find state of shift/control keys
  inline bool ControlDown(void) const { return m_controlDown; }
  inline bool MetaDown(void) const { return m_metaDown; }
  inline bool AltDown(void) const { return m_altDown; }
  inline bool ShiftDown(void) const { return m_shiftDown; }
  inline long KeyCode(void) const { return m_keyCode; }

  // Find the position of the event
  inline void Position(float *xpos, float *ypos) const { *xpos = m_x; *ypos = m_y; }

  // Get X position
  inline float GetX(void) const { return m_x; }

  // Get Y position
  inline float GetY(void) const { return m_y; }

public:
  float         m_x ;
  float         m_y ;
  long          m_keyCode;
  bool          m_controlDown;
  bool          m_shiftDown;
  bool          m_altDown;
  bool          m_metaDown;

};

// Size event class
/*
 wxEVT_SIZE
 */

class WXDLLEXPORT wxSizeEvent: public wxEvent
{
  DECLARE_DYNAMIC_CLASS(wxSizeEvent)

 public:
  wxSize m_size;

  inline wxSizeEvent(void) { m_eventType = wxEVT_SIZE; }
  inline wxSizeEvent(const wxSize& sz, int id = 0)
     { m_eventType = wxEVT_SIZE; m_size.x = sz.x; m_size.y = sz.y; m_id = id; }

  inline wxSize GetSize(void) const { return m_size; }
};

// Move event class

/*
 wxEVT_MOVE
 */

class WXDLLEXPORT wxMoveEvent: public wxEvent
{
  DECLARE_DYNAMIC_CLASS(wxMoveEvent)

 public:
  wxPoint m_pos;

  inline wxMoveEvent(void) { m_eventType = wxEVT_MOVE; }
  inline wxMoveEvent(const wxPoint& pos, int id = 0)
     { m_eventType = wxEVT_MOVE; m_pos.x = pos.x; m_pos.y = pos.y; m_id = id; }

  inline wxPoint GetPosition(void) const { return m_pos; }
};

// Paint event class
/*
 wxEVT_PAINT
 wxEVT_NC_PAINT
 wxEVT_PAINT_ICON
 */

class WXDLLEXPORT wxPaintEvent: public wxEvent
{
  DECLARE_DYNAMIC_CLASS(wxPaintEvent)

 public:
  inline wxPaintEvent(int Id = 0) { m_eventType = wxEVT_PAINT; m_id = Id; }
};

// Erase background event class
/*
 wxEVT_ERASE_BACKGROUND
 */

class WXDLLEXPORT wxDC;
class WXDLLEXPORT wxEraseEvent: public wxEvent
{
  DECLARE_DYNAMIC_CLASS(wxEraseEvent)
 public:
  wxDC *m_dc ;
  inline wxEraseEvent(int Id = 0, wxDC *dc = (wxDC *) NULL) { m_eventType = wxEVT_ERASE_BACKGROUND; m_id = Id; m_dc = dc; }
  inline wxDC *GetDC() const { return m_dc; }
};

// Focus event class
/*
 wxEVT_SET_FOCUS
 wxEVT_KILL_FOCUS
 */

class WXDLLEXPORT wxFocusEvent: public wxEvent
{
  DECLARE_DYNAMIC_CLASS(wxFocusEvent)

 public:
  inline wxFocusEvent(wxEventType type = wxEVT_NULL, int Id = 0) { m_eventType = type; m_id = Id; }
};

// Activate event class
/*
 wxEVT_ACTIVATE
 wxEVT_ACTIVATE_APP
 */

class WXDLLEXPORT wxActivateEvent: public wxEvent
{
  DECLARE_DYNAMIC_CLASS(wxActivateEvent)

 public:
  bool m_active;
  inline wxActivateEvent(wxEventType type = wxEVT_NULL, bool active = TRUE, int Id = 0) { m_eventType = type; m_active = active; m_id = Id; }
  inline bool GetActive(void) const { return m_active; }
};

// InitDialog event class
/*
 wxEVT_INIT_DIALOG
 */

class WXDLLEXPORT wxInitDialogEvent: public wxEvent
{
  DECLARE_DYNAMIC_CLASS(wxInitDialogEvent)

 public:
  inline wxInitDialogEvent(int Id = 0) { m_eventType = wxEVT_INIT_DIALOG; m_id = Id; }
};

// Miscellaneous menu event class
/*
 wxEVT_MENU_CHAR,
 wxEVT_MENU_INIT,
 wxEVT_MENU_HIGHLIGHT,
 wxEVT_POPUP_MENU_INIT,
 wxEVT_CONTEXT_MENU,
*/

class WXDLLEXPORT wxMenuEvent: public wxEvent
{
  DECLARE_DYNAMIC_CLASS(wxMenuEvent)

public:
  int m_menuId;

  inline wxMenuEvent(wxEventType type = wxEVT_NULL, int id = 0) { m_eventType = type; m_menuId = id; }

  inline int GetMenuId(void) const { return m_menuId; }
};

// Window close or session close event class
/*
 wxEVT_CLOSE_WINDOW,
 wxEVT_END_SESSION,
 wxEVT_QUERY_END_SESSION
 */

class WXDLLEXPORT wxCloseEvent: public wxEvent
{
  DECLARE_DYNAMIC_CLASS(wxCloseEvent)
public:

  inline wxCloseEvent(wxEventType type = wxEVT_NULL, int id = 0)
     { m_eventType = type; m_loggingOff = TRUE; m_veto = FALSE;
       m_id = id; m_force = FALSE; m_canVeto = FALSE; }

  inline void SetLoggingOff(bool logOff) { m_loggingOff = logOff; }
  inline bool GetLoggingOff(void) const { return m_loggingOff; }
  inline void Veto(bool veto = TRUE) { m_veto = veto; }
  inline void SetCanVeto(bool canVeto) { m_canVeto = canVeto; }
  inline bool CanVeto() const { return m_canVeto; }
  inline bool GetVeto(void) const { return m_veto; }

  // This is probably obsolete now, since we use CanVeto instead, in
  // both OnCloseWindow and OnQueryEndSession.
  // m_force == ! m_canVeto i.e., can't veto means we must force it to close.
  inline void SetForce(bool force) { m_force = force; }
  inline bool GetForce(void) const { return m_force; }

 protected:
  bool m_loggingOff;
  bool m_veto;
  bool m_force;
  bool m_canVeto;
};

/*
 wxEVT_SHOW
 */

class WXDLLEXPORT wxShowEvent: public wxEvent
{
  DECLARE_DYNAMIC_CLASS(wxShowEvent)
public:

  inline wxShowEvent(int id = 0, bool show = FALSE)
  { m_eventType = wxEVT_SHOW; m_id = id; m_show = show; }

  inline void SetShow(bool show) { m_show = show; }
  inline bool GetShow(void) const { return m_show; }

protected:
  bool m_show;
};

/*
 wxEVT_ICONIZE
 */

class WXDLLEXPORT wxIconizeEvent: public wxEvent
{
  DECLARE_DYNAMIC_CLASS(wxIconizeEvent)
public:

  inline wxIconizeEvent(int id = 0)
  { m_eventType = wxEVT_ICONIZE; m_id = id; }
};

/*
 wxEVT_MAXIMIZE
 */

class WXDLLEXPORT wxMaximizeEvent: public wxEvent
{
  DECLARE_DYNAMIC_CLASS(wxMaximizeEvent)
public:

  inline wxMaximizeEvent(int id = 0)
  { m_eventType = wxEVT_MAXIMIZE; m_id = id; }
};

// Joystick event class
/*
 wxEVT_JOY_BUTTON_DOWN,
 wxEVT_JOY_BUTTON_UP,
 wxEVT_JOY_MOVE,
 wxEVT_JOY_ZMOVE
*/

// Which joystick? Same as Windows ids so no conversion necessary.
#define wxJOYSTICK1     0
#define wxJOYSTICK2     1

// Which button is down?
#define wxJOY_BUTTON1   1
#define wxJOY_BUTTON2   2
#define wxJOY_BUTTON3   4
#define wxJOY_BUTTON4   8
#define wxJOY_BUTTON_ANY -1

class WXDLLEXPORT wxJoystickEvent: public wxEvent
{
  DECLARE_DYNAMIC_CLASS(wxJoystickEvent)

 public:
  wxPoint   m_pos;
  int       m_zPosition;
  int       m_buttonChange; // Which button changed?
  int       m_buttonState; // Which buttons are down?
  int       m_joyStick; // Which joystick?

  inline wxJoystickEvent(wxEventType type = wxEVT_NULL, int state = 0, int joystick = wxJOYSTICK1, int change = 0)
     { m_eventType = type; m_buttonState = state; m_pos = wxPoint(0,0); m_zPosition = 0;
       m_joyStick = joystick; m_buttonChange = change; }

  inline wxPoint GetPosition(void) const { return m_pos; }
  inline int GetZPosition(void) const { return m_zPosition; }
  inline int GetButtonState(void) const { return m_buttonState; }
  inline int GetButtonChange(void) const { return m_buttonChange; }
  inline int GetJoystick(void) const { return m_joyStick; }

  inline void SetJoystick(int stick) { m_joyStick = stick; }
  inline void SetButtonState(int state) { m_buttonState = state; }
  inline void SetButtonChange(int change) { m_buttonChange = change; }
  inline void SetPosition(const wxPoint& pos) { m_pos = pos; }
  inline void SetZPosition(int zPos) { m_zPosition = zPos; }

  // Was it a button event? (*doesn't* mean: is any button *down*?)
  inline bool IsButton(void) const { return ((GetEventType() == wxEVT_JOY_BUTTON_DOWN) ||
    (GetEventType() == wxEVT_JOY_BUTTON_DOWN)); }

  // Was it a move event?
  inline bool IsMove(void) const { return (GetEventType() == wxEVT_JOY_MOVE) ; }

  // Was it a zmove event?
  inline bool IsZMove(void) const { return (GetEventType() == wxEVT_JOY_ZMOVE) ; }

  // Was it a down event from button 1, 2, 3, 4 or any?
  inline bool ButtonDown(int but = wxJOY_BUTTON_ANY) const
    { return ((GetEventType() == wxEVT_JOY_BUTTON_DOWN) &&
              ((but == wxJOY_BUTTON_ANY) || (but == m_buttonChange))); }

  // Was it a up event from button 1, 2, 3 or any?
  inline bool ButtonUp(int but = wxJOY_BUTTON_ANY) const
    { return ((GetEventType() == wxEVT_JOY_BUTTON_UP) &&
                ((but == wxJOY_BUTTON_ANY) || (but == m_buttonChange))); }

  // Was the given button 1,2,3,4 or any in Down state?
  inline bool ButtonIsDown(int but =  wxJOY_BUTTON_ANY) const
    { return (((but == wxJOY_BUTTON_ANY) && (m_buttonState != 0)) ||
              ((m_buttonState & but) == but)); }
};

// Drop files event class
/*
 wxEVT_DROP_FILES
 */

class WXDLLEXPORT wxDropFilesEvent: public wxEvent
{
  DECLARE_DYNAMIC_CLASS(wxDropFilesEvent)

 public:
  int       m_noFiles;
  wxPoint   m_pos;
  wxString* m_files;        // Memory (de)allocated by code calling ProcessEvent

  inline wxDropFilesEvent(wxEventType type = wxEVT_NULL, int noFiles = 0, wxString *files = (wxString *) NULL)
     { m_eventType = type; m_noFiles = noFiles; m_files = files; }

  inline wxPoint GetPosition(void) const { return m_pos; }
  inline int GetNumberOfFiles(void) const { return m_noFiles; }
  inline wxString *GetFiles(void) const { return m_files; }
};

// Idle event
/*
 wxEVT_IDLE
 */

class WXDLLEXPORT wxIdleEvent: public wxEvent
{
  DECLARE_DYNAMIC_CLASS(wxIdleEvent)

public:
  inline wxIdleEvent(void)
     { m_eventType = wxEVT_IDLE; m_requestMore = FALSE; }

  inline void RequestMore(bool needMore = TRUE) { m_requestMore = needMore; }
  inline bool MoreRequested(void) const { return m_requestMore; }

protected:
  bool m_requestMore;
};

// Update UI event
/*
 wxEVT_UPDATE_UI
 */

class WXDLLEXPORT wxMenu;
class WXDLLEXPORT wxWindow;

class WXDLLEXPORT wxUpdateUIEvent: public wxCommandEvent
{
  DECLARE_DYNAMIC_CLASS(wxUpdateUIEvent)

  inline wxUpdateUIEvent(wxWindowID commandId = 0)
     { m_eventType = wxEVT_UPDATE_UI; m_id = commandId;
       m_checked = FALSE; m_setChecked = FALSE; m_enabled = FALSE; m_setEnabled = FALSE;
       m_setText = FALSE; m_text = ""; }

  inline bool GetChecked(void) const { return m_checked; }
  inline bool GetEnabled(void) const { return m_enabled; }
  inline wxString GetText(void) const { return m_text; }
  inline bool GetSetText(void) const { return m_setText; }
  inline bool GetSetChecked(void) const { return m_setChecked; }
  inline bool GetSetEnabled(void) const { return m_setEnabled; }

  inline void Check(bool check) { m_checked = check; m_setChecked = TRUE; }
  inline void Enable(bool enable) { m_enabled = enable; m_setEnabled = TRUE; }
  inline void SetText(const wxString& text) { m_text = text; m_setText = TRUE; }

 protected:

  bool          m_checked;
  bool          m_enabled;
  bool          m_setEnabled;
  bool          m_setText;
  bool          m_setChecked;
  wxString      m_text;

};

/*
 wxEVT_SYS_COLOUR_CHANGED
 */

// TODO: shouldn't all events record the window ID?
class WXDLLEXPORT wxSysColourChangedEvent: public wxEvent
{
  DECLARE_DYNAMIC_CLASS(wxSysColourChangedEvent)

 public:
  inline wxSysColourChangedEvent(void)
     { m_eventType = wxEVT_SYS_COLOUR_CHANGED; }
};

/*
 wxEVT_PALETTE_CHANGED
 */

class WXDLLEXPORT wxPaletteChangedEvent: public wxEvent
{
  DECLARE_DYNAMIC_CLASS(wxPaletteChangedEvent)

public:
  inline wxPaletteChangedEvent(wxWindowID id = 0): wxEvent(id)
     { m_eventType = wxEVT_PALETTE_CHANGED; m_changedWindow = (wxWindow *) NULL; }

  inline void SetChangedWindow(wxWindow* win) { m_changedWindow = win; }
  inline wxWindow* GetChangedWindow() const { return m_changedWindow; }

protected:
  wxWindow*     m_changedWindow;
};

/*
 wxEVT_QUERY_NEW_PALETTE
 Indicates the window is getting keyboard focus and should re-do its palette.
 */

class WXDLLEXPORT wxQueryNewPaletteEvent: public wxEvent
{
  DECLARE_DYNAMIC_CLASS(wxQueryNewPaletteEvent)

public:
  inline wxQueryNewPaletteEvent(wxWindowID id = 0): wxEvent(id)
     { m_eventType = wxEVT_QUERY_NEW_PALETTE; m_paletteRealized = FALSE; }

  // App sets this if it changes the palette.
  inline void SetPaletteRealized(bool realized) { m_paletteRealized = realized; }
  inline bool GetPaletteRealized() const { return m_paletteRealized; }

protected:
  bool      m_paletteRealized;
};

/*
 Event generated by dialog navigation keys
 wxEVT_NAVIGATION_KEY
 */
// must derive from command event to be propagated to the parent
class WXDLLEXPORT wxNavigationKeyEvent : public wxCommandEvent
{
  DECLARE_DYNAMIC_CLASS(wxNavigationKeyEvent)

public:
  wxNavigationKeyEvent() : wxCommandEvent(wxEVT_NAVIGATION_KEY) { }

  // direction: forward (true) or backward (false)
  bool GetDirection() const        { return m_commandInt == 1; }
  void SetDirection(bool bForward) { m_commandInt = bForward;  }

  // it may be a window change event (MDI, notebook pages...) or a control
  // change event
  bool IsWindowChange() const    { return m_extraLong == 1; }
  void SetWindowChange(bool bIs) { m_extraLong = bIs; }

  // the child which has the focus currently (may be NULL - use 
  // wxWindow::FindFocus then)
  wxWindow* GetCurrentFocus() const { return (wxWindow *)m_clientData; }
  void SetCurrentFocus(wxWindow *win) { m_clientData = (void *)win; }
};

/* TODO
 wxEVT_POWER,
 wxEVT_CREATE,
 wxEVT_DESTROY,
 wxEVT_MOUSE_CAPTURE_CHANGED,
 wxEVT_SETTING_CHANGED, // WM_WININICHANGE (NT) / WM_SETTINGCHANGE (Win95)
// wxEVT_FONT_CHANGED,  // WM_FONTCHANGE: roll into wxEVT_SETTING_CHANGED, but remember to propagate
                        // wxEVT_FONT_CHANGED to all other windows (maybe).
 wxEVT_DRAW_ITEM, // Leave these three as virtual functions in wxControl?? Platform-specific.
 wxEVT_MEASURE_ITEM,
 wxEVT_COMPARE_ITEM
*/

class WXDLLEXPORT wxWindow;
class WXDLLEXPORT wxControl;

// struct WXDLLEXPORT wxEventTableEntry;

typedef void (wxObject::*wxObjectEventFunction)(wxEvent&);

struct WXDLLEXPORT wxEventTableEntry
{
    // For some reason, this can't be wxEventType, or VC++ complains.
	int                     m_eventType;	        // main event type
	int		                m_id;		            // control/menu/toolbar id
	int		                m_lastId;		        // used for ranges of ids
	wxObjectEventFunction	m_fn;	// function to call: not wxEventFunction, because
        				            // of dependency problems
	wxObject*       m_callbackUserData;
};

struct WXDLLEXPORT wxEventTable
{
    const wxEventTable *baseTable;	// Points to base event table (next in chain)
    const wxEventTableEntry *entries;	// Points to bottom of entry array
};

class WXDLLEXPORT wxEvtHandler: public wxObject
{
  DECLARE_DYNAMIC_CLASS(wxEvtHandler)
 public:
  wxEvtHandler(void);
  ~wxEvtHandler(void);

  inline wxEvtHandler *GetNextHandler(void) const { return m_nextHandler; }
  inline wxEvtHandler *GetPreviousHandler(void) const { return m_previousHandler; }
  inline void SetNextHandler(wxEvtHandler *handler) { m_nextHandler = handler; }
  inline void SetPreviousHandler(wxEvtHandler *handler) { m_previousHandler = handler; }

  inline void SetEvtHandlerEnabled(bool en) { m_enabled = en; }
  inline bool GetEvtHandlerEnabled(void) const { return m_enabled; }

  inline virtual void OnCommand(wxWindow& WXUNUSED(win), wxCommandEvent& WXUNUSED(event)) {};
                                                 // Called if child control has no
                                                 // callback function
  // Default behaviour
  virtual long Default(void) { if (GetNextHandler()) return GetNextHandler()->Default(); else return 0; };

  virtual bool OnClose(void);

  virtual bool ProcessEvent(wxEvent& event);
  virtual bool SearchEventTable(wxEventTable& table, wxEvent& event);

  // Dynamic association of a member function handler with the event handler, id and event type
  void Connect( int id, int lastId, wxEventType eventType,
		wxObjectEventFunction func,
		wxObject *userData = (wxObject *) NULL );

  // Convenience function: take just one id
  inline void Connect( int id, wxEventType eventType,
		wxObjectEventFunction func,
		wxObject *userData = (wxObject *) NULL ) { Connect(id, -1, eventType, func, userData); }
		
  bool SearchDynamicEventTable( wxEvent& event );

private:
	static const wxEventTableEntry 	sm_eventTableEntries[];
protected:
    static const wxEventTable	sm_eventTable;
	virtual const wxEventTable*	GetEventTable() const;
protected:
    wxEvtHandler*     m_nextHandler;
    wxEvtHandler*     m_previousHandler;
    bool              m_enabled;                      // Is event handler enabled?
    wxList*           m_dynamicEvents;

};

typedef void (wxEvtHandler::*wxEventFunction)(wxEvent&);
typedef void (wxEvtHandler::*wxCommandEventFunction)(wxCommandEvent&);
typedef void (wxEvtHandler::*wxScrollEventFunction)(wxScrollEvent&);
typedef void (wxEvtHandler::*wxSizeEventFunction)(wxSizeEvent&);
typedef void (wxEvtHandler::*wxMoveEventFunction)(wxMoveEvent&);
typedef void (wxEvtHandler::*wxPaintEventFunction)(wxPaintEvent&);
typedef void (wxEvtHandler::*wxEraseEventFunction)(wxEraseEvent&);
typedef void (wxEvtHandler::*wxMouseEventFunction)(wxMouseEvent&);
typedef void (wxEvtHandler::*wxCharEventFunction)(wxKeyEvent&);
typedef void (wxEvtHandler::*wxFocusEventFunction)(wxFocusEvent&);
typedef void (wxEvtHandler::*wxActivateEventFunction)(wxActivateEvent&);
typedef void (wxEvtHandler::*wxMenuEventFunction)(wxMenuEvent&);
typedef void (wxEvtHandler::*wxJoystickEventFunction)(wxJoystickEvent&);
typedef void (wxEvtHandler::*wxDropFilesEventFunction)(wxDropFilesEvent&);
typedef void (wxEvtHandler::*wxInitDialogEventFunction)(wxInitDialogEvent&);
typedef void (wxEvtHandler::*wxSysColourChangedFunction)(wxSysColourChangedEvent&);
typedef void (wxEvtHandler::*wxUpdateUIEventFunction)(wxUpdateUIEvent&);
typedef void (wxEvtHandler::*wxIdleEventFunction)(wxIdleEvent&);
typedef void (wxEvtHandler::*wxCloseEventFunction)(wxCloseEvent&);
typedef void (wxEvtHandler::*wxShowEventFunction)(wxShowEvent&);
typedef void (wxEvtHandler::*wxIconizeEventFunction)(wxShowEvent&);
typedef void (wxEvtHandler::*wxMaximizeEventFunction)(wxShowEvent&);
typedef void (wxEvtHandler::*wxNavigationKeyEventFunction)(wxNavigationKeyEvent&);
typedef void (wxEvtHandler::*wxPaletteChangedEventFunction)(wxPaletteChangedEvent&);
typedef void (wxEvtHandler::*wxQueryNewPaletteEventFunction)(wxQueryNewPaletteEvent&);

// N.B. In GNU-WIN32, you *have* to take the address of a member function
// (use &) or the compiler crashes...

#define DECLARE_EVENT_TABLE() \
private:\
	static const wxEventTableEntry 	sm_eventTableEntries[];\
protected:\
	static const wxEventTable	sm_eventTable;\
	virtual const wxEventTable*	GetEventTable() const;

#define BEGIN_EVENT_TABLE(theClass, baseClass) \
const wxEventTable *theClass::GetEventTable() const { return &theClass::sm_eventTable; }\
const wxEventTable theClass::sm_eventTable =\
	{ &baseClass::sm_eventTable, &theClass::sm_eventTableEntries[0] };\
const wxEventTableEntry theClass::sm_eventTableEntries[] = { \

#define END_EVENT_TABLE() \
 { 0, 0, 0, 0, 0 } };
 
/*
 * Event table macros
 */

// Generic events
#define EVT_CUSTOM(event, id, func) { event, id, -1, (wxObjectEventFunction) (wxEventFunction) & func, (wxObject *) NULL },
#define EVT_CUSTOM_RANGE(event, id1, id2, func) { event, id1, id2, (wxObjectEventFunction) (wxEventFunction) & func, (wxObject *) NULL },

// Miscellaneous
#define EVT_SIZE(func)  { wxEVT_SIZE, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxSizeEventFunction) & func, (wxObject *) NULL },
#define EVT_MOVE(func)  { wxEVT_MOVE, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxMoveEventFunction) & func, (wxObject *) NULL },
#define EVT_CLOSE(func)  { wxEVT_CLOSE_WINDOW, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxCloseEventFunction) & func, (wxObject *) NULL },
#define EVT_END_SESSION(func)  { wxEVT_END_SESSION, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxCloseEventFunction) & func, (wxObject *) NULL },
#define EVT_QUERY_END_SESSION(func)  { wxEVT_QUERY_END_SESSION, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxCloseEventFunction) & func, (wxObject *) NULL },
#define EVT_PAINT(func)  { wxEVT_PAINT, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxPaintEventFunction) & func, (wxObject *) NULL },
#define EVT_ERASE_BACKGROUND(func)  { wxEVT_ERASE_BACKGROUND, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxEraseEventFunction) & func, (wxObject *) NULL },
#define EVT_CHAR(func)  { wxEVT_CHAR, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxCharEventFunction) & func, (wxObject *) NULL },
#define EVT_CHAR_HOOK(func)  { wxEVT_CHAR_HOOK, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxCharEventFunction) & func, NULL },
#define EVT_MENU_HIGHLIGHT(id, func)  { wxEVT_MENU_HIGHLIGHT, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxMenuEventFunction) & func, (wxObject *) NULL },
#define EVT_MENU_HIGHLIGHT_ALL(func)  { wxEVT_MENU_HIGHLIGHT, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxMenuEventFunction) & func, (wxObject *) NULL },
#define EVT_SET_FOCUS(func)  { wxEVT_SET_FOCUS, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxFocusEventFunction) & func, (wxObject *) NULL },
#define EVT_KILL_FOCUS(func)  { wxEVT_KILL_FOCUS, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxFocusEventFunction) & func, (wxObject *) NULL },
#define EVT_ACTIVATE(func)  { wxEVT_ACTIVATE, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxActivateEventFunction) & func, (wxObject *) NULL },
#define EVT_ACTIVATE_APP(func)  { wxEVT_ACTIVATE_APP, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxActivateEventFunction) & func, (wxObject *) NULL },
#define EVT_END_SESSION(func)  { wxEVT_END_SESSION, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxCloseEventFunction) & func, (wxObject *) NULL },
#define EVT_QUERY_END_SESSION(func)  { wxEVT_QUERY_END_SESSION, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxCloseEventFunction) & func, (wxObject *) NULL },
#define EVT_DROP_FILES(func)  { wxEVT_DROP_FILES, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxDropFilesEventFunction) & func, (wxObject *) NULL },
#define EVT_INIT_DIALOG(func)  { wxEVT_INIT_DIALOG, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxInitDialogEventFunction) & func, (wxObject *) NULL },
#define EVT_SYS_COLOUR_CHANGED(func)  { wxEVT_SYS_COLOUR_CHANGED, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxSysColourChangedFunction) & func, (wxObject *) NULL },
#define EVT_SHOW(func) { wxEVT_SHOW, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxShowEventFunction) & func, (wxObject *) NULL },
#define EVT_MAXIMIZE(func) { wxEVT_MAXIMIZE, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxMaximizeEventFunction) & func, (wxObject *) NULL },
#define EVT_ICONIZE(func) { wxEVT_ICONIZE, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxIconizeEventFunction) & func, (wxObject *) NULL },
#define EVT_NAVIGATION_KEY(func) { wxEVT_NAVIGATION_KEY, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) (wxNavigationKeyEventFunction) & func, (wxObject *) NULL },
#define EVT_PALETTE_CHANGED(func) { wxEVT_PALETTE_CHANGED, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxPaletteChangedEventFunction) & func, (wxObject *) NULL },
#define EVT_QUERY_NEW_PALETTE(func) { wxEVT_QUERY_NEW_PALETTE, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxQueryNewPaletteEventFunction) & func, (wxObject *) NULL },

// Mouse events
#define EVT_LEFT_DOWN(func) { wxEVT_LEFT_DOWN, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxMouseEventFunction) & func, (wxObject *) NULL },
#define EVT_LEFT_UP(func) { wxEVT_LEFT_UP, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxMouseEventFunction) & func, (wxObject *) NULL },
#define EVT_MIDDLE_DOWN(func) { wxEVT_MIDDLE_DOWN, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxMouseEventFunction) & func, (wxObject *) NULL },
#define EVT_MIDDLE_UP(func) { wxEVT_MIDDLE_UP, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxMouseEventFunction) & func, (wxObject *) NULL },
#define EVT_RIGHT_DOWN(func) { wxEVT_RIGHT_DOWN, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxMouseEventFunction) & func, (wxObject *) NULL },
#define EVT_RIGHT_UP(func) { wxEVT_RIGHT_UP, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxMouseEventFunction) & func, (wxObject *) NULL },
#define EVT_MOTION(func) { wxEVT_MOTION, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxMouseEventFunction) & func, (wxObject *) NULL },
#define EVT_LEFT_DCLICK(func) { wxEVT_LEFT_DCLICK, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxMouseEventFunction) & func, (wxObject *) NULL },
#define EVT_MIDDLE_DCLICK(func) { wxEVT_MIDDLE_DCLICK, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxMouseEventFunction) & func, (wxObject *) NULL },
#define EVT_RIGHT_DCLICK(func) { wxEVT_RIGHT_DCLICK, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxMouseEventFunction) & func, (wxObject *) NULL },
#define EVT_LEAVE_WINDOW(func) { wxEVT_LEAVE_WINDOW, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxMouseEventFunction) & func, (wxObject *) NULL },
#define EVT_ENTER_WINDOW(func) { wxEVT_ENTER_WINDOW, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxMouseEventFunction) & func, (wxObject *) NULL },

// All mouse events
#define EVT_MOUSE_EVENTS(func) \
 { wxEVT_LEFT_DOWN, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxMouseEventFunction) & func, (wxObject *) NULL },\
 { wxEVT_LEFT_UP, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxMouseEventFunction) & func, (wxObject *) NULL },\
 { wxEVT_MIDDLE_DOWN, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxMouseEventFunction) & func, (wxObject *) NULL },\
 { wxEVT_MIDDLE_UP, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxMouseEventFunction) & func, (wxObject *) NULL },\
 { wxEVT_RIGHT_DOWN, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxMouseEventFunction) & func, (wxObject *) NULL },\
 { wxEVT_RIGHT_UP, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxMouseEventFunction) & func, (wxObject *) NULL },\
 { wxEVT_MOTION, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxMouseEventFunction) & func, (wxObject *) NULL },\
 { wxEVT_LEFT_DCLICK, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxMouseEventFunction) & func, (wxObject *) NULL },\
 { wxEVT_MIDDLE_DCLICK, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxMouseEventFunction) & func, (wxObject *) NULL },\
 { wxEVT_RIGHT_DCLICK, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxMouseEventFunction) & func, (wxObject *) NULL },\
 { wxEVT_ENTER_WINDOW, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxMouseEventFunction) & func, (wxObject *) NULL },\
 { wxEVT_LEAVE_WINDOW, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxMouseEventFunction) & func, (wxObject *) NULL },

// EVT_COMMAND
#define EVT_COMMAND(id, cmd, fn)  { cmd, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) & fn, (wxObject *) NULL },
#define EVT_COMMAND_RANGE(id1, id2, cmd, fn)  { cmd, id1, id2, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) & fn, (wxObject *) NULL },

// Scrolling
#define EVT_SCROLL(func) \
  { wxEVT_SCROLL_TOP, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxScrollEventFunction) & func, (wxObject *) NULL },\
  { wxEVT_SCROLL_BOTTOM, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxScrollEventFunction) & func, (wxObject *) NULL },\
  { wxEVT_SCROLL_LINEUP, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxScrollEventFunction) & func, (wxObject *) NULL },\
  { wxEVT_SCROLL_LINEDOWN, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxScrollEventFunction) & func, (wxObject *) NULL },\
  { wxEVT_SCROLL_PAGEUP, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxScrollEventFunction) & func, (wxObject *) NULL },\
  { wxEVT_SCROLL_PAGEDOWN, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxScrollEventFunction) & func, (wxObject *) NULL },\
  { wxEVT_SCROLL_THUMBTRACK, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxScrollEventFunction) & func, (wxObject *) NULL },

#define EVT_SCROLL_TOP(func) { wxEVT_SCROLL_TOP, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxScrollEventFunction) & func, (wxObject *) NULL },
#define EVT_SCROLL_BOTTOM(func) { wxEVT_SCROLL_BOTTOM, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxScrollEventFunction) & func, (wxObject *) NULL },
#define EVT_SCROLL_LINEUP(func) { wxEVT_SCROLL_LINEUP, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxScrollEventFunction) & func, (wxObject *) NULL },
#define EVT_SCROLL_LINEDOWN(func) { wxEVT_SCROLL_LINEDOWN, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxScrollEventFunction) & func, (wxObject *) NULL },
#define EVT_SCROLL_PAGEUP(func) { wxEVT_SCROLL_PAGEUP, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxScrollEventFunction) & func, (wxObject *) NULL },
#define EVT_SCROLL_PAGEDOWN(func) { wxEVT_SCROLL_PAGEDOWN, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxScrollEventFunction) & func, (wxObject *) NULL },
#define EVT_SCROLL_THUMBTRACK(func) { wxEVT_SCROLL_THUMBTRACK, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxScrollEventFunction) & func, (wxObject *) NULL },

// Scrolling, with an id
#define EVT_COMMAND_SCROLL(id, func) \
  { wxEVT_SCROLL_TOP, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxScrollEventFunction) & func, (wxObject *) NULL },\
  { wxEVT_SCROLL_BOTTOM, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxScrollEventFunction) & func, (wxObject *) NULL },\
  { wxEVT_SCROLL_LINEUP, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxScrollEventFunction) & func, (wxObject *) NULL },\
  { wxEVT_SCROLL_LINEDOWN, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxScrollEventFunction) & func, (wxObject *) NULL },\
  { wxEVT_SCROLL_PAGEUP, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxScrollEventFunction) & func, (wxObject *) NULL },\
  { wxEVT_SCROLL_PAGEDOWN, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxScrollEventFunction) & func, (wxObject *) NULL },\
  { wxEVT_SCROLL_THUMBTRACK, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxScrollEventFunction) & func, (wxObject *) NULL },

#define EVT_COMMAND_SCROLL_TOP(id, func) { wxEVT_SCROLL_TOP, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxScrollEventFunction) & func, (wxObject *) NULL },
#define EVT_COMMAND_SCROLL_BOTTOM(id, func) { wxEVT_SCROLL_BOTTOM, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxScrollEventFunction) & func, (wxObject *) NULL },
#define EVT_COMMAND_SCROLL_LINEUP(id, func) { wxEVT_SCROLL_LINEUP, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxScrollEventFunction) & func, (wxObject *) NULL },
#define EVT_COMMAND_SCROLL_LINEDOWN(id, func) { wxEVT_SCROLL_LINEDOWN, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxScrollEventFunction) & func, (wxObject *) NULL },
#define EVT_COMMAND_SCROLL_PAGEUP(id, func) { wxEVT_SCROLL_PAGEUP, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxScrollEventFunction) & func, (wxObject *) NULL },
#define EVT_COMMAND_SCROLL_PAGEDOWN(id, func) { wxEVT_SCROLL_PAGEDOWN, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxScrollEventFunction) & func, (wxObject *) NULL },
#define EVT_COMMAND_SCROLL_THUMBTRACK(id, func) { wxEVT_SCROLL_THUMBTRACK, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxScrollEventFunction) & func, (wxObject *) NULL },

// Convenience macros for commonly-used commands
#define EVT_BUTTON(id, fn) { wxEVT_COMMAND_BUTTON_CLICKED, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) & fn, (wxObject *) NULL },
#define EVT_CHECKBOX(id, fn) { wxEVT_COMMAND_CHECKBOX_CLICKED, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) & fn, (wxObject *) NULL },
#define EVT_CHOICE(id, fn) { wxEVT_COMMAND_CHOICE_SELECTED, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) & fn, (wxObject *) NULL },
#define EVT_LISTBOX(id, fn) { wxEVT_COMMAND_LISTBOX_SELECTED, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) & fn, (wxObject *) NULL },
#define EVT_LISTBOX_DCLICK(id, fn) { wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) & fn, (wxObject *) NULL },
#define EVT_TEXT(id, fn) { wxEVT_COMMAND_TEXT_UPDATED, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) & fn, (wxObject *) NULL },
#define EVT_TEXT_ENTER(id, fn) { wxEVT_COMMAND_TEXT_ENTER, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) & fn, (wxObject *) NULL },
#define EVT_MENU(id, fn) { wxEVT_COMMAND_MENU_SELECTED, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) & fn, (wxObject *) NULL },
#define EVT_MENU_RANGE(id1, id2, fn) { wxEVT_COMMAND_MENU_SELECTED, id1, id2, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) & fn, (wxObject *) NULL },
#define EVT_SLIDER(id, fn) { wxEVT_COMMAND_SLIDER_UPDATED, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) & fn, (wxObject *) NULL },
#define EVT_RADIOBOX(id, fn) { wxEVT_COMMAND_RADIOBOX_SELECTED, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) & fn, (wxObject *) NULL },
#define EVT_RADIOBUTTON(id, fn) { wxEVT_COMMAND_RADIOBUTTON_SELECTED, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) & fn, (wxObject *) NULL },
// EVT_SCROLLBAR is now obsolete since we use EVT_COMMAND_SCROLL... events
#define EVT_SCROLLBAR(id, fn) { wxEVT_COMMAND_SCROLLBAR_UPDATED, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) & fn, (wxObject *) NULL },
#define EVT_VLBOX(id, fn) { wxEVT_COMMAND_VLBOX_SELECTED, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) & fn, (wxObject *) NULL },
#define EVT_COMBOBOX(id, fn) { wxEVT_COMMAND_COMBOBOX_SELECTED, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) & fn, (wxObject *) NULL },
#define EVT_TOOL(id, fn) { wxEVT_COMMAND_TOOL_CLICKED, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) & fn, (wxObject *) NULL },
#define EVT_TOOL_RANGE(id1, id2, fn) { wxEVT_COMMAND_TOOL_CLICKED, id1, id2, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) & fn, (wxObject *) NULL },
#define EVT_TOOL_RCLICKED(id, fn) { wxEVT_COMMAND_TOOL_RCLICKED, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) & fn, (wxObject *) NULL },
#define EVT_TOOL_RCLICKED_RANGE(id1, id2, fn) { wxEVT_COMMAND_TOOL_RCLICKED, id1, id2, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) & fn, (wxObject *) NULL },
#define EVT_TOOL_ENTER(id, fn) { wxEVT_COMMAND_TOOL_ENTER, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) & fn, (wxObject *) NULL },
#define EVT_CHECKLISTBOX(id, fn) { wxEVT_COMMAND_CHECKLISTBOX_TOGGLED, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) & fn, (wxObject *) NULL },

// Generic command events
#define EVT_COMMAND_LEFT_CLICK(id, fn) { wxEVT_COMMAND_LEFT_CLICK, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) & fn, (wxObject *) NULL },
#define EVT_COMMAND_LEFT_DCLICK(id, fn) { wxEVT_COMMAND_LEFT_DCLICK, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) & fn, (wxObject *) NULL },
#define EVT_COMMAND_RIGHT_CLICK(id, fn) { wxEVT_COMMAND_RIGHT_CLICK, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) & fn, (wxObject *) NULL },
#define EVT_COMMAND_RIGHT_DCLICK(id, fn) { wxEVT_COMMAND_RIGHT_DCLICK, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) & fn, (wxObject *) NULL },
#define EVT_COMMAND_SET_FOCUS(id, fn) { wxEVT_COMMAND_SET_FOCUS, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) & fn, (wxObject *) NULL },
#define EVT_COMMAND_KILL_FOCUS(id, fn) { wxEVT_COMMAND_KILL_FOCUS, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) & fn, (wxObject *) NULL },
#define EVT_COMMAND_ENTER(id, fn) { wxEVT_COMMAND_ENTER, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) & fn, (wxObject *) NULL },

// Joystick events
#define EVT_JOY_DOWN(func) \
 { wxEVT_JOY_BUTTON_DOWN, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxJoystickEventFunction) & func, (wxObject *) NULL },
#define EVT_JOY_UP(func) \
 { wxEVT_JOY_BUTTON_UP, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxJoystickEventFunction) & func, (wxObject *) NULL },
#define EVT_JOY_MOVE(func) \
 { wxEVT_JOY_MOVE, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxJoystickEventFunction) & func, (wxObject *) NULL },
#define EVT_JOY_ZMOVE(func) \
 { wxEVT_JOY_ZMOVE, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxJoystickEventFunction) & func, (wxObject *) NULL },

// All joystick events
#define EVT_JOYSTICK_EVENTS(func) \
 { wxEVT_JOY_BUTTON_DOWN, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxJoystickEventFunction) & func, (wxObject *) NULL },\
 { wxEVT_JOY_BUTTON_UP, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxJoystickEventFunction) & func, (wxObject *) NULL },\
 { wxEVT_JOY_MOVE, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxJoystickEventFunction) & func, (wxObject *) NULL },\
 { wxEVT_JOY_ZMOVE, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxJoystickEventFunction) & func, (wxObject *) NULL },\

// Idle event
#define EVT_IDLE(func) \
 { wxEVT_IDLE, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxIdleEventFunction) & func, (wxObject *) NULL },\

// Update UI event
#define EVT_UPDATE_UI(id, func) \
 { wxEVT_UPDATE_UI, id, -1, (wxObjectEventFunction) (wxEventFunction) (wxUpdateUIEventFunction) & func, (wxObject *) NULL },\

#endif
	// _WX_EVENTH__
