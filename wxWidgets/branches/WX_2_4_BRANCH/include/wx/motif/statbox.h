/////////////////////////////////////////////////////////////////////////////
// Name:        statbox.h
// Purpose:     wxStaticBox class
// Author:      Julian Smart
// Modified by:
// Created:     17/09/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_STATBOX_H_
#define _WX_STATBOX_H_

#ifdef __GNUG__
#pragma interface "statbox.h"
#endif

#include "wx/control.h"

WXDLLEXPORT_DATA(extern const char*) wxStaticBoxNameStr;

// Group box
class WXDLLEXPORT wxStaticBox: public wxControl
{
    DECLARE_DYNAMIC_CLASS(wxStaticBox)
        
public:
    wxStaticBox();
    wxStaticBox(wxWindow *parent, wxWindowID id,
        const wxString& label,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = 0,
        const wxString& name = wxStaticBoxNameStr)
    {
        Create(parent, id, label, pos, size, style, name);
    }
    ~wxStaticBox();
    
    bool Create(wxWindow *parent, wxWindowID id,
        const wxString& label,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = 0,
        const wxString& name = wxStaticBoxNameStr);
    
    virtual bool ProcessCommand(wxCommandEvent& WXUNUSED(event))
    {
        return FALSE;
    }
    
    void SetLabel(const wxString& label);
    wxString GetLabel() const;
    
    // Implementation
    virtual void ChangeFont(bool keepOriginalSize = TRUE);
    virtual void ChangeBackgroundColour();
    virtual void ChangeForegroundColour();
    WXWidget GetTopWidget() const { return m_formWidget; }
    WXWidget GetLabelWidget() const { return m_labelWidget; }
    
protected:
    // Motif-specific
    WXWidget  m_formWidget;
    WXWidget  m_labelWidget;
    
    virtual void DoSetSize(int x, int y,
        int width, int height,
        int sizeFlags = wxSIZE_AUTO);
    
private:
    DECLARE_EVENT_TABLE()
};

#endif
// _WX_STATBOX_H_
