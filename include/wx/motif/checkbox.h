/////////////////////////////////////////////////////////////////////////////
// Name:        checkbox.h
// Purpose:     wxCheckBox class
// Author:      Julian Smart
// Modified by:
// Created:     17/09/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:   	wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_CHECKBOX_H_
#define _WX_CHECKBOX_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "checkbox.h"
#endif

#include "wx/control.h"

// Checkbox item (single checkbox)
class WXDLLEXPORT wxCheckBox: public wxCheckBoxBase
{
    DECLARE_DYNAMIC_CLASS(wxCheckBox)
        
public:
    inline wxCheckBox() { Init(); }
    inline wxCheckBox(wxWindow *parent, wxWindowID id, const wxString& label,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = 0,
        const wxValidator& validator = wxDefaultValidator,
        const wxString& name = wxCheckBoxNameStr)
    {
        Init();

        Create(parent, id, label, pos, size, style, validator, name);
    }
    
    bool Create(wxWindow *parent, wxWindowID id, const wxString& label,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = 0,
        const wxValidator& validator = wxDefaultValidator,
        const wxString& name = wxCheckBoxNameStr);
    virtual void SetValue(bool);
    virtual bool GetValue() const ;
    virtual void Command(wxCommandEvent& event);
    
    // Implementation
    virtual void ChangeBackgroundColour();
private:
    // common part of all constructors
    void Init()
    {
        m_evtType = wxEVT_COMMAND_CHECKBOX_CLICKED;
    }

    // public for the callback
public:
    // either exEVT_COMMAND_CHECKBOX_CLICKED or ..._TOGGLEBUTTON_CLICKED
    wxEventType m_evtType;
};

#endif
// _WX_CHECKBOX_H_
