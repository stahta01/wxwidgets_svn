/////////////////////////////////////////////////////////////////////////////
// Name:        events.i
// Purpose:     SWIGgable Event classes for wxPython
//
// Author:      Robin Dunn
//
// Created:     5/24/98
// RCS-ID:      $Id$
// Copyright:   (c) 1998 by Total Control Software
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////


%module events

%{
#include "helpers.h"
#include <wx/spinbutt.h>
%}

//----------------------------------------------------------------------

%include typemaps.i
%include my_typemaps.i

// Import some definitions of other classes, etc.
%import _defs.i
%import misc.i
%import gdi.i

//---------------------------------------------------------------------------

enum Propagation_state
{
    // don't propagate it at all
    wxEVENT_PROPAGATE_NONE = 0,

    // propagate it until it is processed
    wxEVENT_PROPAGATE_MAX = INT_MAX
};


int wxNewEventType();



class wxEvent : public wxObject {
public:
    // wxEvent(int winid = 0, wxEventType commandType = wxEVT_NULL);     // *** This class is now an ABC
    ~wxEvent();

    void SetEventType(wxEventType typ);
    wxEventType GetEventType() const;
    wxObject *GetEventObject() const;
    void SetEventObject(wxObject *obj);
    long GetTimestamp() const;
    void SetTimestamp(long ts = 0);
    int GetId() const;
    void SetId(int Id);


    bool IsCommandEvent() const;

    // Can instruct event processor that we wish to ignore this event
    // (treat as if the event table entry had not been found): this must be done
    // to allow the event processing by the base classes (calling event.Skip()
    // is the analog of calling the base class verstion of a virtual function)
    void Skip(bool skip = TRUE);
    bool GetSkipped() const;

    // Determine if this event should be propagating to the parent window.
    bool ShouldPropagate() const;

    // Stop an event from propagating to its parent window, returns the old
    // propagation level value
    int StopPropagation();

    // Resume the event propagation by restoring the propagation level
    // (returned by StopPropagation())
    void ResumePropagation(int propagationLevel);

    // this function is used to create a copy of the event polymorphically and
    // all derived classes must implement it because otherwise wxPostEvent()
    // for them wouldn't work (it needs to do a copy of the event)
    virtual wxEvent *Clone() /* =0*/;
};

//---------------------------------------------------------------------------

// Helper class to temporarily change an event not to propagate.
class  wxPropagationDisabler
{
public:
    wxPropagationDisabler(wxEvent& event);
    ~wxPropagationDisabler();
};


// Another one to temporarily lower propagation level.
class  wxPropagateOnce
{
public:
    wxPropagateOnce(wxEvent& event);
    ~wxPropagateOnce();
};

//---------------------------------------------------------------------------

class wxSizeEvent : public wxEvent {
public:
    wxSizeEvent(const wxSize& sz, int id = 0);
    wxSize GetSize();
    wxRect GetRect() const;
    void SetRect(wxRect rect);
};

//---------------------------------------------------------------------------

class wxCloseEvent : public wxEvent {
public:
    wxCloseEvent(int commandEventType = 0, int id = 0);

    void SetLoggingOff(bool loggingOff);
    bool GetLoggingOff();
    void Veto(bool veto = TRUE);
    bool CanVeto();
    bool GetVeto();
    void SetCanVeto(bool canVeto);
};

//---------------------------------------------------------------------------

class wxCommandEvent : public wxEvent {
public:
    wxCommandEvent(int commandEventType = 0, int id = 0);

    bool IsChecked();
    %name(Checked)bool IsChecked();
    long GetExtraLong();
    int GetInt();
    int GetSelection();
    wxString GetString();
    bool IsSelection();
    void SetString(const wxString& s);
    void SetExtraLong(long extraLong);
    void SetInt(int i);

    %addmethods {
        PyObject* GetClientData() {
            wxPyClientData* data = (wxPyClientData*)self->GetClientObject();
            if (data) {
                Py_INCREF(data->m_obj);
                return data->m_obj;
            } else {
                Py_INCREF(Py_None);
                return Py_None;
            }
        }
    }
};


//---------------------------------------------------------------------------

class wxScrollEvent: public wxCommandEvent {
public:
    wxScrollEvent(int commandType = 0, int id = 0, int pos = 0,
                  int orientation = 0);

    int GetOrientation();
    int GetPosition();
};

//---------------------------------------------------------------------------

class wxScrollWinEvent: public wxEvent {
public:
    wxScrollWinEvent(int commandType = 0, int pos = 0,
                     int orientation = 0);

    int GetOrientation();
    int GetPosition();
};

//---------------------------------------------------------------------------

class wxSpinEvent : public wxScrollEvent {
public:
    wxSpinEvent(int commandType = 0, int id = 0);

};

//---------------------------------------------------------------------------

class wxMouseEvent: public wxEvent {
public:
    wxMouseEvent(int mouseEventType = 0);

    bool IsButton();
    bool ButtonDown(int but = -1);
    bool ButtonDClick(int but = -1);
    bool ButtonUp(int but = -1);
    bool Button(int but);
    bool ButtonIsDown(int but);
    bool ControlDown();
    bool MetaDown();
    bool AltDown();
    bool ShiftDown();
    bool LeftDown();
    bool MiddleDown();
    bool RightDown();
    bool LeftUp();
    bool MiddleUp();
    bool RightUp();
    bool LeftDClick();
    bool MiddleDClick();
    bool RightDClick();
    bool LeftIsDown();
    bool MiddleIsDown();
    bool RightIsDown();
    bool Dragging();
    bool Moving();
    bool Entering();
    bool Leaving();
    wxPoint GetPosition();
    %name(GetPositionTuple)void GetPosition(long *OUTPUT, long *OUTPUT);
    wxPoint GetLogicalPosition(const wxDC& dc);
    long GetX();
    long GetY();

    int GetWheelRotation() const { return m_wheelRotation; }
    int GetWheelDelta() const { return m_wheelDelta; }
    int GetLinesPerAction() const { return m_linesPerAction; }

    long          m_x, m_y;
    bool          m_leftDown;
    bool          m_middleDown;
    bool          m_rightDown;
    bool          m_controlDown;
    bool          m_shiftDown;
    bool          m_altDown;
    bool          m_metaDown;
    int           m_wheelRotation;
    int           m_wheelDelta;
    int           m_linesPerAction;
};

//---------------------------------------------------------------------------

class wxMouseCaptureChangedEvent : public wxEvent
{
public:
    wxMouseCaptureChangedEvent(wxWindowID id = 0, wxWindow* gainedCapture = NULL);
    wxWindow* GetCapturedWindow() const;
};

//---------------------------------------------------------------------------

class wxSetCursorEvent : public wxEvent
{
public:
    wxSetCursorEvent(wxCoord x = 0, wxCoord y = 0);

    wxCoord GetX() const;
    wxCoord GetY() const;

    void SetCursor(const wxCursor& cursor);
    const wxCursor& GetCursor() const;
    bool HasCursor() const;
};

//---------------------------------------------------------------------------

class wxKeyEvent: public wxEvent {
public:
    wxKeyEvent(int keyEventType);

    bool ControlDown();
    bool MetaDown();
    bool AltDown();
    bool ShiftDown();

    long GetKeyCode();
    %pragma(python) addtoclass = "KeyCode = GetKeyCode"
    bool HasModifiers();

    // get the raw key code (platform-dependent)
    long GetRawKeyCode() const;

    // get the raw key flags (platform-dependent)
    long GetRawKeyFlags() const;

    long GetX();
    long GetY();
    wxPoint GetPosition();
    %name(GetPositionTuple) void GetPosition(long* OUTPUT, long* OUTPUT);

    long        m_x, m_y;
    long        m_keyCode;
    bool        m_controlDown;
    bool        m_shiftDown;
    bool        m_altDown;
    bool        m_metaDown;
    bool        m_scanCode;
    long        m_rawCode;
    long        m_rawFlags;

};

//---------------------------------------------------------------------------

class wxNavigationKeyEvent : public wxEvent {
public:
    wxNavigationKeyEvent();

    bool GetDirection();
    void SetDirection(bool bForward);
    bool IsWindowChange();
    void SetWindowChange(bool bIs);
    wxWindow* GetCurrentFocus();
    void SetCurrentFocus(wxWindow *win);
};


//---------------------------------------------------------------------------

class wxMoveEvent: public wxEvent {
public:
    wxMoveEvent(const wxPoint& pt, int id = 0);
    wxPoint GetPosition();
    wxRect GetRect() const;
    void SetRect(wxRect rect);
};

//---------------------------------------------------------------------------

class wxPaintEvent: public wxEvent {
public:
    wxPaintEvent(int id = 0);

};

//---------------------------------------------------------------------------

class wxEraseEvent: public wxEvent {
public:
    wxEraseEvent(int id = 0, wxDC* dc = NULL);

    wxDC *GetDC();
};

//---------------------------------------------------------------------------

class wxFocusEvent: public wxEvent {
public:
    wxFocusEvent(int eventType = 0, int id = 0);
};

//---------------------------------------------------------------------------

// wxChildFocusEvent notifies the parent that a child has got the focus: unlike
// wxFocusEvent it is propgated upwards the window chain
class  wxChildFocusEvent : public wxCommandEvent
{
public:
    wxChildFocusEvent(wxWindow *win = NULL);
    wxWindow *GetWindow() const;
};


//---------------------------------------------------------------------------

class wxActivateEvent: public wxEvent{
public:
    wxActivateEvent(int eventType = 0, int active = TRUE, int id = 0);
    bool GetActive();
};

//---------------------------------------------------------------------------

class wxInitDialogEvent: public wxEvent {
public:
    wxInitDialogEvent(int id = 0);
};

//---------------------------------------------------------------------------

class wxMenuEvent: public wxEvent {
public:
    wxMenuEvent(int id = 0, int winid = 0, wxMenu* menu = NULL);
    int GetMenuId();
    bool IsPopup();
    wxMenu* GetMenu() const;
};

//---------------------------------------------------------------------------

class wxShowEvent: public wxEvent {
public:
    wxShowEvent(int id = 0, int show = FALSE);
    void SetShow(bool show);
    bool GetShow();
};

//---------------------------------------------------------------------------

class wxIconizeEvent: public wxEvent {
public:
    wxIconizeEvent(int id = 0, bool iconized = TRUE);
    bool Iconized();
};

//---------------------------------------------------------------------------

class wxMaximizeEvent: public wxEvent {
public:
    wxMaximizeEvent(int id = 0);
};

//---------------------------------------------------------------------------

class wxJoystickEvent: public wxEvent {
public:
    wxJoystickEvent(int type = wxEVT_NULL,
                    int state = 0,
                    int joystick = wxJOYSTICK1,
                    int change = 0);
    wxPoint GetPosition();
    int GetZPosition();
    int GetButtonState();
    int GetButtonChange();
    int GetJoystick();
    void SetJoystick(int stick);
    void SetButtonState(int state);
    void SetButtonChange(int change);
    void SetPosition(const wxPoint& pos);
    void SetZPosition(int zPos);
    bool IsButton();
    bool IsMove();
    bool IsZMove();
    bool ButtonDown(int but = wxJOY_BUTTON_ANY);
    bool ButtonUp(int but = wxJOY_BUTTON_ANY);
    bool ButtonIsDown(int but =  wxJOY_BUTTON_ANY);
};

//---------------------------------------------------------------------------

class wxDropFilesEvent: public wxEvent {
public:
    wxPoint GetPosition();
    int GetNumberOfFiles();

    %addmethods {
        PyObject* GetFiles() {
            int         count = self->GetNumberOfFiles();
            wxString*   files = self->GetFiles();
            PyObject*   list  = PyList_New(count);

            if (!list) {
                PyErr_SetString(PyExc_MemoryError, "Can't allocate list of files!");
                return NULL;
            }

            for (int i=0; i<count; i++) {
#if wxUSE_UNICODE
                PyList_SetItem(list, i, PyUnicode_FromWideChar(files[i], files[i].Len()));
#else
                PyList_SetItem(list, i, PyString_FromString((const char*)files[i]));
#endif
            }
            return list;
        }
    }
};

//---------------------------------------------------------------------------

// Whether to always send idle events to windows, or
// to only send update events to those with the
// wxWS_EX_PROCESS_IDLE style.

enum wxIdleMode
{
        // Send idle events to all windows
    wxIDLE_PROCESS_ALL,

        // Send idle events to windows that have
        // the wxWS_EX_PROCESS_IDLE flag specified
    wxIDLE_PROCESS_SPECIFIED
};


class wxIdleEvent: public wxEvent {
public:
    wxIdleEvent();
    void RequestMore(bool needMore = TRUE);
    bool MoreRequested();

    // Specify how wxWindows will send idle events: to
    // all windows, or only to those which specify that they
    // will process the events.
    static void SetMode(wxIdleMode mode) { sm_idleMode = mode; }

    // Returns the idle event mode
    static wxIdleMode GetMode() { return sm_idleMode ; }

    // Can we send an idle event?
    static bool CanSend(wxWindow* win) ;

};

//---------------------------------------------------------------------------

// Whether to always send update events to windows, or
// to only send update events to those with the
// wxWS_EX_PROCESS_UI_UPDATES style.

enum wxUpdateUIMode
{
        // Send UI update events to all windows
    wxUPDATE_UI_PROCESS_ALL,

        // Send UI update events to windows that have
        // the wxWS_EX_PROCESS_UI_UPDATES flag specified
    wxUPDATE_UI_PROCESS_SPECIFIED
};


class wxUpdateUIEvent: public wxEvent {
public:
    wxUpdateUIEvent(wxWindowID commandId = 0);
    bool GetChecked();
    bool GetEnabled();
    wxString GetText();
    bool GetSetText();
    bool GetSetChecked();
    bool GetSetEnabled();

    void Check(bool check);
    void Enable(bool enable);
    void SetText(const wxString& text);


    // Sets the interval between updates in milliseconds.
    // Set to -1 to disable updates, or to 0 to update as frequently as possible.
    static void SetUpdateInterval(long updateInterval);

    // Returns the current interval between updates in milliseconds
    static long GetUpdateInterval();

    // Can we update this window?
    static bool CanUpdate(wxWindow* win);

    // Reset the update time to provide a delay until the next
    // time we should update
    static void ResetUpdateTime();

    // Specify how wxWindows will send update events: to
    // all windows, or only to those which specify that they
    // will process the events.
    static void SetMode(wxUpdateUIMode mode);

    // Returns the UI update mode
    static wxUpdateUIMode GetMode();
};

//---------------------------------------------------------------------------

class wxSysColourChangedEvent: public wxEvent {
public:
    wxSysColourChangedEvent();
};

//---------------------------------------------------------------------------


class wxNotifyEvent : public wxCommandEvent {
public:
    wxNotifyEvent(int commandType = wxEVT_NULL, int id = 0);
    bool IsAllowed();
    void Allow();
    void Veto();
};


//---------------------------------------------------------------------------

class wxDisplayChangedEvent : public wxEvent
{
public:
    wxDisplayChangedEvent();
};


//---------------------------------------------------------------------------

class  wxPaletteChangedEvent : public wxEvent {
public:
    wxPaletteChangedEvent(wxWindowID id = 0);

    void SetChangedWindow(wxWindow* win);
    wxWindow* GetChangedWindow();

};

//---------------------------------------------------------------------------

class  wxQueryNewPaletteEvent : public wxEvent {
public:
    wxQueryNewPaletteEvent(wxWindowID id = 0);

    void SetPaletteRealized(bool realized);
    bool GetPaletteRealized();
};


//---------------------------------------------------------------------------

class wxWindowCreateEvent : public wxCommandEvent {
public:
    wxWindowCreateEvent(wxWindow *win = NULL);

    wxWindow *GetWindow();
};

class wxWindowDestroyEvent : public wxCommandEvent {
public:
    wxWindowDestroyEvent(wxWindow *win = NULL);

    wxWindow *GetWindow();
};

//---------------------------------------------------------------------------

class wxContextMenuEvent : public wxCommandEvent
{
public:
    wxContextMenuEvent(wxEventType type = wxEVT_NULL,
                       wxWindowID id = 0,
                       const wxPoint& pt = wxDefaultPosition);
    const wxPoint& GetPosition();
    void SetPosition(const wxPoint& pos);
};

//----------------------------------------------------------------------

class  wxTimerEvent : public wxEvent
{
public:
    wxTimerEvent(int id = 0, int interval = 0);
    int GetInterval();
};

//---------------------------------------------------------------------------

class wxTextUrlEvent : public wxCommandEvent
{
public:
    wxTextUrlEvent(int id, const wxMouseEvent& evtMouse,
                   long start, long end);
    const wxMouseEvent& GetMouseEvent();
    long GetURLStart();
    long GetURLEnd();
};

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// These classes can be derived from in Python and passed through the event
// system without loosing anything.  They do this by keeping a reference to
// themselves and some special case handling in wxPyCallback::EventThunker.

class wxPyEvent : public wxEvent {
public:
    wxPyEvent(int winid=0, wxEventType commandType = wxEVT_NULL );
    ~wxPyEvent();

    %pragma(python) addtomethod = "__init__:self.SetSelf(self)"

    void SetSelf(PyObject* self);
    PyObject* GetSelf();
};


class wxPyCommandEvent : public wxCommandEvent {
public:
    wxPyCommandEvent(wxEventType commandType = wxEVT_NULL, int id=0);
    ~wxPyCommandEvent();

    %pragma(python) addtomethod = "__init__:self.SetSelf(self)"

    void SetSelf(PyObject* self);
    PyObject* GetSelf();
};




//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

