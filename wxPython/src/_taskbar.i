/////////////////////////////////////////////////////////////////////////////
// Name:        _taskbar.i
// Purpose:     SWIG interface defs for wxTaskBarIcon
//
// Author:      Robin Dunn
//
// Created:     2-June-1998
// RCS-ID:      $Id$
// Copyright:   (c) 2003 by Total Control Software
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

// Not a %module


//---------------------------------------------------------------------------

%{
%}

//---------------------------------------------------------------------------
%newgroup;


%{
#ifdef __WXMAC__
// implement dummy classes and such for wxMac

class wxTaskBarIcon : public wxEvtHandler
{
public:
    wxTaskBarIcon()  { wxPyRaiseNotImplemented(); }
};


class wxTaskBarIconEvent : public wxEvent
{
public:
    wxTaskBarIconEvent(wxEventType, wxTaskBarIcon *)
        { wxPyRaiseNotImplemented(); }
    virtual wxEvent* Clone() const { return NULL; }
};

enum {
    wxEVT_TASKBAR_MOVE = 0,
    wxEVT_TASKBAR_LEFT_DOWN = 0,
    wxEVT_TASKBAR_LEFT_UP = 0,
    wxEVT_TASKBAR_RIGHT_DOWN = 0,
    wxEVT_TASKBAR_RIGHT_UP = 0,
    wxEVT_TASKBAR_LEFT_DCLICK = 0,
    wxEVT_TASKBAR_RIGHT_DCLICK = 0,
};


#else
// // Otherwise make a class that can virtualize CreatePopupMenu
// class wxPyTaskBarIcon : public wxTaskBarIcon
// {
//     DECLARE_ABSTRACT_CLASS(wxPyTaskBarIcon);
// public:
//     wxPyTaskBarIcon() : wxTaskBarIcon()
//     {}

//     wxMenu* CreatePopupMenu() {
//         wxMenu *rval = NULL;
//         bool found;
//         bool blocked = wxPyBeginBlockThreads();
//         if ((found = wxPyCBH_findCallback(m_myInst, "CreatePopupMenu"))) {
//             PyObject* ro;
//             wxMenu* ptr;
//             ro = wxPyCBH_callCallbackObj(m_myInst, Py_BuildValue("()"));
//             if (ro) {
//                 if (wxPyConvertSwigPtr(ro, (void **)&ptr, wxT("wxMenu")))
//                     rval = ptr;
//                 Py_DECREF(ro);
//             }
//         }
//         wxPyEndBlockThreads(blocked);
//         if (! found)
//             rval = wxTaskBarIcon::CreatePopupMenu();
//         return rval;
//     }

//     PYPRIVATE;
// };

// IMPLEMENT_ABSTRACT_CLASS(wxPyTaskBarIcon, wxTaskBarIcon);

#endif
%}


// NOTE: TaskbarIcon has not yet been changed to be able to virtualize the
// CreatePopupMenu method because it is just before a release and I worry that
// there will be a problem in this case with it holding a reference to itself
// (since it depends on the dtor for cleanup.)  Better safe than sorry!
//
// Perhaps a better mechanism for wxPython woudl be to turn CreatePopupMenu
// into an event...

MustHaveApp(wxTaskBarIcon);

class wxTaskBarIcon : public wxEvtHandler
{
public:
    wxTaskBarIcon();
    ~wxTaskBarIcon();

    %extend {
        void Destroy() {
        #ifndef __WXMAC__
            self->RemoveIcon();
        #endif
        }
    }

#ifndef __WXMAC__
    bool IsOk() const;
    %pythoncode { def __nonzero__(self): return self.IsOk() }

    bool IsIconInstalled() const;

    bool SetIcon(const wxIcon& icon, const wxString& tooltip = wxPyEmptyString);
    bool RemoveIcon();
    bool PopupMenu(wxMenu *menu);
#endif
};



class wxTaskBarIconEvent : public wxEvent
{
public:
    wxTaskBarIconEvent(wxEventType evtType, wxTaskBarIcon *tbIcon);
};



%constant wxEventType wxEVT_TASKBAR_MOVE;
%constant wxEventType wxEVT_TASKBAR_LEFT_DOWN;
%constant wxEventType wxEVT_TASKBAR_LEFT_UP;
%constant wxEventType wxEVT_TASKBAR_RIGHT_DOWN;
%constant wxEventType wxEVT_TASKBAR_RIGHT_UP;
%constant wxEventType wxEVT_TASKBAR_LEFT_DCLICK;
%constant wxEventType wxEVT_TASKBAR_RIGHT_DCLICK;


%pythoncode {
EVT_TASKBAR_MOVE = wx.PyEventBinder (         wxEVT_TASKBAR_MOVE )
EVT_TASKBAR_LEFT_DOWN = wx.PyEventBinder (    wxEVT_TASKBAR_LEFT_DOWN )
EVT_TASKBAR_LEFT_UP = wx.PyEventBinder (      wxEVT_TASKBAR_LEFT_UP )
EVT_TASKBAR_RIGHT_DOWN = wx.PyEventBinder (   wxEVT_TASKBAR_RIGHT_DOWN )
EVT_TASKBAR_RIGHT_UP = wx.PyEventBinder (     wxEVT_TASKBAR_RIGHT_UP )
EVT_TASKBAR_LEFT_DCLICK = wx.PyEventBinder (  wxEVT_TASKBAR_LEFT_DCLICK )
EVT_TASKBAR_RIGHT_DCLICK = wx.PyEventBinder ( wxEVT_TASKBAR_RIGHT_DCLICK )
}

//---------------------------------------------------------------------------
