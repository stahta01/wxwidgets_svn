/////////////////////////////////////////////////////////////////////////////
// Name:        wx/gtk/frame.h
// Purpose:
// Author:      Robert Roebling
// Id:          $Id$
// Copyright:   (c) 1998 Robert Roebling, Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#ifndef __GTKFRAMEH__
#define __GTKFRAMEH__

#ifdef __GNUG__
    #pragma interface "frame.h"
#endif

//-----------------------------------------------------------------------------
// classes
//-----------------------------------------------------------------------------

class wxMDIChildFrame;
class wxMDIClientWindow;
class wxMenu;
class wxMenuBar;
class wxToolBar;
class wxStatusBar;

//-----------------------------------------------------------------------------
// wxFrameGTK
//-----------------------------------------------------------------------------

class wxFrameGTK : public wxFrameBase
{
public:
    // construction
    wxFrameGTK() { Init(); }
    wxFrameGTK(wxWindow *parent,
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

    virtual ~wxFrameGTK();

#if wxUSE_STATUSBAR
    virtual void PositionStatusBar();

    virtual wxStatusBar* CreateStatusBar(int number = 1,
                                         long style = wxST_SIZEGRIP,
                                         wxWindowID id = 0,
                                         const wxString& name = wxStatusLineNameStr);
#endif // wxUSE_STATUSBAR

#if wxUSE_TOOLBAR
    virtual wxToolBar* CreateToolBar(long style = wxNO_BORDER | wxTB_HORIZONTAL | wxTB_FLAT,
                                     wxWindowID id = -1,
                                     const wxString& name = wxToolBarNameStr);
    void SetToolBar(wxToolBar *toolbar);
#endif // wxUSE_TOOLBAR
    
    wxPoint GetClientAreaOrigin() const { return wxPoint(0, 0); }

    // implementation from now on
    // --------------------------

    // GTK callbacks
    virtual void GtkOnSize( int x, int y, int width, int height );
    virtual void OnInternalIdle();

    bool          m_menuBarDetached;
    bool          m_toolBarDetached;

protected:
    // common part of all ctors
    void Init();

    // override wxWindow methods to take into account tool/menu/statusbars
    virtual void DoSetClientSize(int width, int height);
    virtual void DoGetClientSize( int *width, int *height ) const;

#if wxUSE_MENUS_NATIVE
    virtual void DetachMenuBar();
    virtual void AttachMenuBar(wxMenuBar *menubar);
#endif // wxUSE_MENUS_NATIVE
};

#endif // __GTKFRAMEH__
