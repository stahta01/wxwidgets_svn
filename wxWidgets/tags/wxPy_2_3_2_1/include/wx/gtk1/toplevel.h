/////////////////////////////////////////////////////////////////////////////
// Name:        wx/gtk/toplevel.h
// Purpose:
// Author:      Robert Roebling
// Id:          $Id$
// Copyright:   (c) 1998 Robert Roebling, Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#ifndef __GTKTOPLEVELH__
#define __GTKTOPLEVELH__

#ifdef __GNUG__
    #pragma interface "toplevel.h"
#endif


//-----------------------------------------------------------------------------
// wxTopLevelWindowGTK
//-----------------------------------------------------------------------------

class wxTopLevelWindowGTK : public wxTopLevelWindowBase
{
public:
    // construction
    wxTopLevelWindowGTK() { Init(); }
    wxTopLevelWindowGTK(wxWindow *parent,
                        wxWindowID id,
                        const wxString& title,
                        const wxPoint& pos = wxDefaultPosition,
                        const wxSize& size = wxDefaultSize,
                        long style = wxDEFAULT_FRAME_STYLE,
                        const wxString& name = wxFrameNameStr)
    {
        Init();

        Create(parent, id, title, pos, size, style, name);
    }

    bool Create(wxWindow *parent,
                wxWindowID id,
                const wxString& title,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxDEFAULT_FRAME_STYLE,
                const wxString& name = wxFrameNameStr);

    virtual ~wxTopLevelWindowGTK();

    // implement base class pure virtuals
    virtual void Maximize(bool maximize = TRUE);
    virtual bool IsMaximized() const;
    virtual void Iconize(bool iconize = TRUE);
    virtual bool IsIconized() const;
    virtual void SetIcon(const wxIcon& icon);
    virtual void Restore();

    virtual bool ShowFullScreen(bool show, long style = wxFULLSCREEN_ALL);
    virtual bool IsFullScreen() const { return m_fsIsShowing; };

    virtual bool Show(bool show = TRUE);

    virtual void SetTitle( const wxString &title );
    virtual wxString GetTitle() const { return m_title; }

    // implementation from now on
    // --------------------------

    // move the window to the specified location and resize it: this is called
    // from both DoSetSize() and DoSetClientSize()
    virtual void DoMoveWindow(int x, int y, int width, int height);

    // GTK callbacks
    virtual void GtkOnSize( int x, int y, int width, int height );
    virtual void OnInternalIdle();

    // do *not* call this to iconize the frame, this is a private function!
    void SetIconizeState(bool iconic);

    wxString      m_title;
    int           m_miniEdge,
                  m_miniTitle;
    GtkWidget    *m_mainWidget;
    bool          m_insertInClientArea;  /* not from within OnCreateXXX */

    bool          m_fsIsShowing;         /* full screen */
    long          m_fsSaveGdkFunc, m_fsSaveGdkDecor;
    long          m_fsSaveFlag;
    wxRect        m_fsSaveFrame;
    
    long          m_gdkFunc, m_gdkDecor; // m_windowStyle translated to GDK's terms

protected:
    // common part of all ctors
    void Init();

    // override wxWindow methods to take into account tool/menu/statusbars
    virtual void DoSetSize(int x, int y,
                           int width, int height,
                           int sizeFlags = wxSIZE_AUTO);

    virtual void DoSetClientSize(int width, int height);
    virtual void DoGetClientSize( int *width, int *height ) const;

    // is the frame currently iconized?
    bool m_isIconized;
};

#endif // __GTKTOPLEVELH__
