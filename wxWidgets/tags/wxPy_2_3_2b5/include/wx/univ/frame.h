///////////////////////////////////////////////////////////////////////////////
// Name:        wx/univ/frame.h
// Purpose:     wxFrame class for wxUniversal
// Author:      Vadim Zeitlin
// Modified by:
// Created:     19.05.01
// RCS-ID:      $Id$
// Copyright:   (c) 2001 SciTech Software, Inc. (www.scitechsoft.com)
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_UNIV_FRAME_H_
#define _WX_UNIV_FRAME_H_

#ifdef __GNUG__
    #pragma interface "univframe.h"
#endif

// ----------------------------------------------------------------------------
// wxFrame
// ----------------------------------------------------------------------------

class wxFrame : public wxFrameBase
{
public:
    wxFrame() {}
    wxFrame(wxWindow *parent,
            wxWindowID id,
            const wxString& title,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxDEFAULT_FRAME_STYLE,
            const wxString& name = wxFrameNameStr)
    {
        Create(parent, id, title, pos, size, style, name);
    }
            
    bool Create(wxWindow *parent,
                wxWindowID id,
                const wxString& title,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxDEFAULT_FRAME_STYLE,
                const wxString& name = wxFrameNameStr);

    virtual wxPoint GetClientAreaOrigin() const;
    virtual bool Enable(bool enable = TRUE);

protected:
    void OnSize(wxSizeEvent& event);

    virtual void DoGetClientSize(int *width, int *height) const;
    virtual void DoSetClientSize(int width, int height);

#if wxUSE_MENUS
    // override to update menu bar position when the frame size changes
    virtual void PositionMenuBar();
#endif // wxUSE_MENUS

#if wxUSE_STATUSBAR
    // override to update statusbar position when the frame size changes
    virtual void PositionStatusBar();
#endif // wxUSE_MENUS

    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(wxFrame)
};

#endif // _WX_UNIV_FRAME_H_
