/////////////////////////////////////////////////////////////////////////////
// Name:        wx/generic/spinctlg.h
// Purpose:     generic wxSpinCtrl class
// Author:      Vadim Zeitlin
// Modified by:
// Created:     28.10.99
// RCS-ID:      $Id$
// Copyright:   (c) Vadim Zeitlin
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_MAC_SPINCTRL_H_
#define _WX_MAC_SPINCTRL_H_

// ----------------------------------------------------------------------------
// wxSpinCtrl is a combination of wxSpinButton and wxTextCtrl, so if
// wxSpinButton is available, this is what we do - but if it isn't, we still
// define wxSpinCtrl class which then has the same appearance as wxTextCtrl but
// the different interface. This allows to write programs using wxSpinCtrl
// without tons of #ifdefs.
// ----------------------------------------------------------------------------

#if wxUSE_SPINBTN

#include "wx/containr.h"

class WXDLLIMPEXP_FWD_CORE wxSpinButton;
class WXDLLIMPEXP_FWD_CORE wxTextCtrl;
class WXDLLIMPEXP_FWD_CORE wxSpinCtrlText;
class WXDLLIMPEXP_FWD_CORE wxSpinCtrlButton;

// ----------------------------------------------------------------------------
// wxSpinCtrl is a combination of wxTextCtrl and wxSpinButton
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxSpinCtrl : public wxControl
{
public:
    wxSpinCtrl() { Init(); }

    wxSpinCtrl(wxWindow *parent,
               wxWindowID id = -1,
               const wxString& value = wxEmptyString,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = wxSP_ARROW_KEYS | wxALIGN_RIGHT,
               int min = 0, int max = 100, int initial = 0,
               const wxString& name = wxT("wxSpinCtrl"))
    {
        Init();
        Create(parent, id, value, pos, size, style, min, max, initial, name);
    }

    bool Create(wxWindow *parent,
                wxWindowID id = -1,
                const wxString& value = wxEmptyString,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxSP_ARROW_KEYS | wxALIGN_RIGHT,
                int min = 0, int max = 100, int initial = 0,
                const wxString& name = wxT("wxSpinCtrl"));

    // wxTextCtrl-like method
    void SetSelection(long from, long to);

    virtual ~wxSpinCtrl();

    // operations
    void SetValue(int val);
    void SetValue(const wxString& text);
    void SetRange(int min, int max);

    // accessors
    int GetValue() const;
    int GetMin() const;
    int GetMax() const;

    // implementation from now on

    // forward these functions to all subcontrols
    virtual bool Enable(bool enable = TRUE);
    virtual bool Show(bool show = TRUE);

    // get the subcontrols
    wxTextCtrl *GetText() const { return m_text; }
    wxSpinButton *GetSpinButton() const { return m_btn; }

    // set the value of the text (only)
    void SetTextValue(int val);

    // put the numeric value of the string in the text ctrl into val and return
    // TRUE or return FALSE if the text ctrl doesn't contain a number or if the
    // number is out of range
    bool GetTextValue(int *val) const;

    WX_DECLARE_CONTROL_CONTAINER();

protected:
    // override the base class virtuals involved into geometry calculations
    virtual wxSize DoGetBestSize() const;
    virtual void DoMoveWindow(int x, int y, int width, int height);

    // common part of all ctors
    void Init();

private:
    // the subcontrols
    wxTextCtrl *m_text;
    wxSpinButton *m_btn;

    friend class wxSpinCtrlText;
    friend class wxSpinCtrlButton;

    int            m_oldValue;
private:
    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(wxSpinCtrl)
};

#else // !wxUSE_SPINBTN

// ----------------------------------------------------------------------------
// wxSpinCtrl is just a text control
// ----------------------------------------------------------------------------

#include "wx/textctrl.h"

class WXDLLIMPEXP_CORE wxSpinCtrl : public wxTextCtrl
{
public:
    wxSpinCtrl() { Init(); }

    wxSpinCtrl(wxWindow *parent,
               wxWindowID id = -1,
               const wxString& value = wxEmptyString,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = wxSP_ARROW_KEYS | wxALIGN_RIGHT,
               int min = 0, int max = 100, int initial = 0,
               const wxString& name = wxT("wxSpinCtrl"))
    {
        Create(parent, id, value, pos, size, style, min, max, initial, name);
    }

    bool Create(wxWindow *parent,
                wxWindowID id = -1,
                const wxString& value = wxEmptyString,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxSP_ARROW_KEYS | wxALIGN_RIGHT,
                int min = 0, int max = 100, int initial = 0,
                const wxString& name = wxT("wxSpinCtrl"))
    {
        SetRange(min, max);

        bool ok = wxTextCtrl::Create(parent, id, value, pos, size, style,
                                     wxDefaultValidator, name);
        SetValue(initial);

        return ok;
    }

    // accessors
    int GetValue(int WXUNUSED(dummy) = 1) const
    {
        int n;
        if ( (wxSscanf(wxTextCtrl::GetValue(), wxT("%d"), &n) != 1) )
            n = INT_MIN;

        return n;
    }

    int GetMin() const { return m_min; }
    int GetMax() const { return m_max; }

    // operations
    void SetValue(const wxString& value) { wxTextCtrl::SetValue(value); }
    void SetValue(int val) { wxString s; s << val; wxTextCtrl::SetValue(s); }
    void SetRange(int min, int max) { m_min = min; m_max = max; }

protected:
    // initialize m_min/max with the default values
    void Init() { SetRange(0, 100); }

    int   m_min;
    int   m_max;

private:
    DECLARE_DYNAMIC_CLASS(wxSpinCtrl)
};

#endif // wxUSE_SPINBTN/!wxUSE_SPINBTN

#endif // _WX_MAC_SPINCTRL_H_

