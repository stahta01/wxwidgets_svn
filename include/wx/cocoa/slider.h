/////////////////////////////////////////////////////////////////////////////
// Name:        wx/cocoa/slider.h
// Purpose:     wxSlider class
// Author:      David Elliott
// Modified by:
// Created:     2003/06/19
// RCS-ID:      $Id$
// Copyright:   (c) 2003 David Elliott
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __WX_COCOA_SLIDER_H__
#define __WX_COCOA_SLIDER_H__

// #include "wx/cocoa/NSSlider.h"

// ========================================================================
// wxSlider
// ========================================================================
class WXDLLEXPORT wxSlider: public wxSliderBase// , protected wxCocoaNSSlider
{
    DECLARE_DYNAMIC_CLASS(wxSlider)
    DECLARE_EVENT_TABLE()
//    WX_DECLARE_COCOA_OWNER(NSSlider,NSControl,NSView)
// ------------------------------------------------------------------------
// initialization
// ------------------------------------------------------------------------
public:
    wxSlider() { }
    wxSlider(wxWindow *parent, wxWindowID winid,
            int value, int minValue, int maxValue,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxSL_HORIZONTAL,
            const wxValidator& validator = wxDefaultValidator,
            const wxString& name = wxSliderNameStr)
    {
        Create(parent, winid, value, minValue, maxValue,
            pos, size, style, validator, name);
    }

    bool Create(wxWindow *parent, wxWindowID winid,
            int value, int minValue, int maxValue,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxSL_HORIZONTAL,
            const wxValidator& validator = wxDefaultValidator,
            const wxString& name = wxSliderNameStr);
    virtual ~wxSlider();

// ------------------------------------------------------------------------
// Cocoa callbacks
// ------------------------------------------------------------------------
protected:
// ------------------------------------------------------------------------
// Implementation
// ------------------------------------------------------------------------
public:
    // Pure Virtuals
    virtual int GetValue() const { return 0; }
    virtual void SetValue(int value) { }

    // retrieve/change the range
    virtual void SetRange(int minValue, int maxValue) { }
    virtual int GetMin() const { return 0; }
    virtual int GetMax() const { return 0; }

    // the line/page size is the increment by which the slider moves when
    // cursor arrow key/page up or down are pressed (clicking the mouse is like
    // pressing PageUp/Down) and are by default set to 1 and 1/10 of the range
    virtual void SetLineSize(int lineSize) { }
    virtual void SetPageSize(int pageSize) { }
    virtual int GetLineSize() const { return 0; }
    virtual int GetPageSize() const { return 0; }

    // these methods get/set the length of the slider pointer in pixels
    virtual void SetThumbLength(int lenPixels) { }
    virtual int GetThumbLength() const { return 0; }

};

#endif // __WX_COCOA_SLIDER_H__
