/////////////////////////////////////////////////////////////////////////////
// Name:        wx/cocoa/statbmp.h
// Purpose:     wxStaticBitmap class
// Author:      David Elliott
// Modified by:
// Created:     2003/03/16
// RCS-ID:      $Id:
// Copyright:   (c) 2003 David Elliott
// Licence:   	wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifndef __WX_COCOA_STATBMP_H__
#define __WX_COCOA_STATBMP_H__

// #include "wx/cocoa/NSxxx.h"

// ========================================================================
// wxStaticBitmap
// ========================================================================
class WXDLLEXPORT wxStaticBitmap : public wxStaticBitmapBase //, protected wxCocoaNSxxx
{
    DECLARE_DYNAMIC_CLASS(wxStaticBitmap)
    DECLARE_EVENT_TABLE()
//    WX_DECLARE_COCOA_OWNER(NSxxx,NSControl,NSView)
// ------------------------------------------------------------------------
// initialization
// ------------------------------------------------------------------------
public:
    wxStaticBitmap() {}
    wxStaticBitmap(wxWindow *parent, wxWindowID winid,
            const wxBitmap& bitmap,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize, long style = 0,
            const wxString& name = wxStaticBitmapNameStr)
    {
        Create(parent, winid, bitmap, pos, size, style, name);
    }

    bool Create(wxWindow *parent, wxWindowID winid,
            const wxBitmap& bitmap,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize, long style = 0,
            const wxString& name = wxStaticBitmapNameStr);
    virtual ~wxStaticBitmap();

// ------------------------------------------------------------------------
// Cocoa specifics
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
// Implementation
// ------------------------------------------------------------------------
    virtual void SetIcon(const wxIcon& icon);
    virtual void SetBitmap(const wxBitmap& bitmap);
    virtual wxBitmap GetBitmap() const;
};

#endif // __WX_COCOA_STATBMP_H__
