///////////////////////////////////////////////////////////////////////////////
// Name:        wx/x11/toplevel.h
// Purpose:     wxTopLevelWindowX11 is the X11 implementation of wxTLW
// Author:      Julian Smart
// Modified by:
// Created:     20.09.01
// RCS-ID:      $Id$
// Copyright:   (c) 2002 Julian Smart
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_X11_TOPLEVEL_H_
#define _WX_X11_TOPLEVEL_H_

#ifdef __GNUG__
    #pragma interface "toplevel.h"
#endif

// ----------------------------------------------------------------------------
// wxTopLevelWindowX11
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxTopLevelWindowX11 : public wxTopLevelWindowBase
{
public:
    // constructors and such
    wxTopLevelWindowX11() { Init(); }

    wxTopLevelWindowX11(wxWindow *parent,
                        wxWindowID id,
                        const wxString& title,
                        const wxPoint& pos = wxDefaultPosition,
                        const wxSize& size = wxDefaultSize,
                        long style = wxDEFAULT_FRAME_STYLE,
                        const wxString& name = wxFrameNameStr)
    {
        Init();

        (void)Create(parent, id, title, pos, size, style, name);
    }

    bool Create(wxWindow *parent,
                wxWindowID id,
                const wxString& title,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxDEFAULT_FRAME_STYLE,
                const wxString& name = wxFrameNameStr);

    virtual ~wxTopLevelWindowX11();

    // implement base class pure virtuals
    virtual void Maximize(bool maximize = TRUE);
    virtual bool IsMaximized() const;
    virtual void Iconize(bool iconize = TRUE);
    virtual bool IsIconized() const;
    virtual void SetIcon(const wxIcon& icon);
    virtual void Restore();

    virtual bool Show(bool show = TRUE);

    virtual bool ShowFullScreen(bool show, long style = wxFULLSCREEN_ALL);
    virtual bool IsFullScreen() const { return m_fsIsShowing; }

    virtual void SetTitle( const wxString& title);
    virtual wxString GetTitle() const;
    
    // implementation from now on
    // --------------------------

protected:
    // common part of all ctors
    void Init();

    // is the frame currently iconized?
    bool m_iconized;

    // should the frame be maximized when it will be shown? set by Maximize()
    // when it is called while the frame is hidden
    bool m_maximizeOnShow;

    // Data to save/restore when calling ShowFullScreen
    long                  m_fsStyle; // Passed to ShowFullScreen
    wxRect                m_fsOldSize;
    bool                  m_fsIsMaximized;
    bool                  m_fsIsShowing;
    wxString              m_title;
};

// list of all frames and modeless dialogs
//extern WXDLLEXPORT_DATA(wxWindowList) wxModelessWindows;

#endif // _WX_X11_TOPLEVEL_H_

