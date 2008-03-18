/////////////////////////////////////////////////////////////////////////////
// Name:        spinctrl.h
// Purpose:     wxSpinCtrlBase class
// Author:      Vadim Zeitlin
// Modified by:
// Created:     22.07.99
// RCS-ID:      $Id$
// Copyright:   (c) Vadim Zeitlin
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_SPINCTRL_H_
#define _WX_SPINCTRL_H_

#include "wx/defs.h"

#if wxUSE_SPINCTRL

#include "wx/spinbutt.h"        // should make wxSpinEvent visible to the app

// ----------------------------------------------------------------------------
// A spin ctrl is a text control with a spin button which is usually used to
// prompt the user for a numeric input.
// There are two kinds for number types T=integer or T=double.
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxSpinCtrlBase : public wxControl
{
public:
    wxSpinCtrlBase() {}

    // accessor functions that derived classes are expected to have
    // T GetValue() const
    // T GetMin() const
    // T GetMax() const
    // T GetIncrement() const
    virtual bool GetSnapToTicks() const = 0;
    // unsigned GetDigits() const                   - wxSpinCtrlDouble only

    // operation functions that derived classes are expected to have
    virtual void SetValue(const wxString& value) = 0;
    // void SetValue(T val)
    // void SetRange(T minVal, T maxVal)
    // void SetIncrement(T inc)
    virtual void SetSnapToTicks(bool snap_to_ticks) = 0;
    // void SetDigits(unsigned digits)              - wxSpinCtrlDouble only

    // Select text in the textctrl
    virtual void SetSelection(long from, long to) = 0;

private:
    DECLARE_NO_COPY_CLASS(wxSpinCtrlBase)
};

// ----------------------------------------------------------------------------
// wxSpinDoubleEvent - a wxSpinEvent for double valued controls
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxSpinDoubleEvent : public wxNotifyEvent
{
public:
    wxSpinDoubleEvent(wxEventType commandType = wxEVT_NULL, int winid = 0,
                      double value = 0)
        : wxNotifyEvent(commandType, winid), m_value(value)
    {
    }

    wxSpinDoubleEvent(const wxSpinDoubleEvent& event)
        : wxNotifyEvent(event), m_value(event.GetValue())
    {
    }

    double GetValue() const       { return m_value; }
    void   SetValue(double value) { m_value = value; }

    virtual wxEvent *Clone() const { return new wxSpinDoubleEvent(*this); }

protected:
    double m_value;

private:
    DECLARE_DYNAMIC_CLASS_NO_ASSIGN(wxSpinDoubleEvent)
};

// ----------------------------------------------------------------------------
// wxSpinDoubleEvent event type, see also wxSpinEvent in wx/spinbutt.h
// ----------------------------------------------------------------------------

typedef void (wxEvtHandler::*wxSpinDoubleEventFunction)(wxSpinDoubleEvent&);

#define wxSpinDoubleEventHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(wxSpinDoubleEventFunction, &func)

// macros for handling spinctrl events

#define EVT_SPINCTRL(id, fn) \
    wx__DECLARE_EVT1(wxEVT_COMMAND_SPINCTRL_UPDATED, id, wxSpinEventHandler(fn))

#define EVT_SPINCTRLDOUBLE(id, fn) \
    wx__DECLARE_EVT1(wxEVT_COMMAND_SPINCTRLDOUBLE_UPDATED, id, wxSpinDoubleEventHandler(fn))

// ----------------------------------------------------------------------------
// include the platform-dependent class implementation
// ----------------------------------------------------------------------------

// we may have a native wxSpinCtrl implementation, native wxSpinCtrl and
// wxSpinCtrlDouble implementations or neither, define the appropriate symbols
// and include the generic version if necessary to provide the missing class(es)

#if defined(__WXUNIVERSAL__) || \
    defined(__WXMOTIF__) || \
    defined(__WXCOCOA__)
    // nothing, use generic controls
#elif defined(__WXMSW__)
    #define wxHAS_NATIVE_SPINCTRL
    #include "wx/msw/spinctrl.h"
#elif defined(__WXPM__)
    #define wxHAS_NATIVE_SPINCTRL
    #include "wx/os2/spinctrl.h"
#elif defined(__WXGTK20__)
    #define wxHAS_NATIVE_SPINCTRL
    #define wxHAS_NATIVE_SPINCTRLDOUBLE
    #include "wx/gtk/spinctrl.h"
#elif defined(__WXGTK__)
    #define wxHAS_NATIVE_SPINCTRL
    #include "wx/gtk1/spinctrl.h"
#elif defined(__WXMAC__)
    #define wxHAS_NATIVE_SPINCTRL
    #include "wx/mac/spinctrl.h"
#endif // platform

#if !defined(wxHAS_NATIVE_SPINCTRL) || !defined(wxHAS_NATIVE_SPINCTRLDOUBLE)
    #include "wx/generic/spinctlg.h"
#endif

#endif // wxUSE_SPINCTRL

#endif // _WX_SPINCTRL_H_
