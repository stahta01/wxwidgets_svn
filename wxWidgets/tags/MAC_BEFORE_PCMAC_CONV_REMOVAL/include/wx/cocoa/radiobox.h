/////////////////////////////////////////////////////////////////////////////
// Name:        wx/cocoa/radiobox.h
// Purpose:     wxRadioBox class
// Author:      David Elliott
// Modified by:
// Created:     2003/03/18
// RCS-ID:      $Id:
// Copyright:   (c) 2003 David Elliott
// Licence:   	wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifndef __WX_COCOA_RADIOBOX_H__
#define __WX_COCOA_RADIOBOX_H__

// #include "wx/cocoa/NSButton.h"

// ========================================================================
// wxRadioBox
// ========================================================================
class WXDLLEXPORT wxRadioBox: public wxControl, public wxRadioBoxBase// , protected wxCocoaNSButton
{
    DECLARE_DYNAMIC_CLASS(wxRadioBox)
    DECLARE_EVENT_TABLE()
//    WX_DECLARE_COCOA_OWNER(NSButton,NSControl,NSView)
// ------------------------------------------------------------------------
// initialization
// ------------------------------------------------------------------------
public:
    wxRadioBox() { }
    wxRadioBox(wxWindow *parent, wxWindowID winid,
            const wxString& title,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            int n = 0, const wxString choices[] = NULL,
            int majorDim = 0,
            long style = 0, const wxValidator& validator = wxDefaultValidator,
            const wxString& name = wxRadioBoxNameStr)
    {
        Create(parent, winid, title, pos, size, n, choices, majorDim, style, validator, name);
    }

    bool Create(wxWindow *parent, wxWindowID winid,
            const wxString& title,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            int n = 0, const wxString choices[] = NULL,
            int majorDim = 0,
            long style = 0,
            const wxValidator& validator = wxDefaultValidator,
            const wxString& name = wxRadioBoxNameStr);
    virtual ~wxRadioBox();

// ------------------------------------------------------------------------
// Cocoa callbacks
// ------------------------------------------------------------------------
protected:
    // Static boxes cannot be enabled/disabled
    virtual void CocoaSetEnabled(bool enable) { }
// ------------------------------------------------------------------------
// Implementation
// ------------------------------------------------------------------------
public:
// Pure virtuals
    // selection
    virtual void SetSelection(int n);
    virtual int GetSelection() const;
    // string access
    virtual int GetCount() const;
    virtual wxString GetString(int n) const;
    virtual void SetString(int n, const wxString& label);
    // change the individual radio button state
    virtual void Enable(int n, bool enable = TRUE);
    virtual void Show(int n, bool show = TRUE);
    // layout parameters
    virtual int GetColumnCount() const;
    virtual int GetRowCount() const;
};

#endif // __WX_COCOA_RADIOBOX_H__
