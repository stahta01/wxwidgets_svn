/////////////////////////////////////////////////////////////////////////////
// Name:        _toplvl.i
// Purpose:     SWIG definitions for wxTopLevelWindow, wxFrame, wxDialog and etc.
//
// Author:      Robin Dunn
//
// Created:     27-Aug-1998
// RCS-ID:      $Id$
// Copyright:   (c) 2003 by Total Control Software
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

// Not a %module


//---------------------------------------------------------------------------

%{
    // Put some wx default wxChar* values into wxStrings.
    DECLARE_DEF_STRING(FrameNameStr);
    DECLARE_DEF_STRING(DialogNameStr);
    DECLARE_DEF_STRING(StatusLineNameStr);
    DECLARE_DEF_STRING(ToolBarNameStr);
%}

//---------------------------------------------------------------------------
%newgroup

enum
{
    wxFULLSCREEN_NOMENUBAR,
    wxFULLSCREEN_NOTOOLBAR,
    wxFULLSCREEN_NOSTATUSBAR,
    wxFULLSCREEN_NOBORDER,
    wxFULLSCREEN_NOCAPTION,
    wxFULLSCREEN_ALL,

    wxTOPLEVEL_EX_DIALOG,
};



%typemap(in) (int widths, int* widths_field) {
    $1 = PyList_Size($input);
    $2 =  int_LIST_helper($input);
    if ($2 == NULL) SWIG_fail;
}

%typemap(freearg) (int widths, int* widths_field) {
    if ($2) delete [] $2;
}


//---------------------------------------------------------------------------

class  wxTopLevelWindow : public wxWindow
{
public:

    // No constructor as it can not be used directly from Python

    // maximize = True => maximize, otherwise - restore
    virtual void Maximize(bool maximize = True);

    // undo Maximize() or Iconize()
    virtual void Restore();

    // iconize = True => iconize, otherwise - restore
    virtual void Iconize(bool iconize = True);

    // return True if the frame is maximized
    virtual bool IsMaximized() const;

    // return True if the frame is iconized
    virtual bool IsIconized() const;

    // get the frame icon
    wxIcon GetIcon() const;

    // set the frame icon
    virtual void SetIcon(const wxIcon& icon);

    // set the frame icons
    virtual void SetIcons(const wxIconBundle& icons );

    // maximize the window to cover entire screen
    virtual bool ShowFullScreen(bool show, long style = wxFULLSCREEN_ALL);

    // return True if the frame is in fullscreen mode
    virtual bool IsFullScreen() const;

    virtual void SetTitle(const wxString& title);
    virtual wxString GetTitle() const;

    // Set the shape of the window to the given region.
    // Returns True if the platform supports this feature (and the operation
    // is successful.)
    virtual bool SetShape(const wxRegion& region);

};


//---------------------------------------------------------------------------
%newgroup

// wxFrame is a top-level window with optional menubar, statusbar and toolbar
//
// For each of *bars, a frame may have several of them, but only one is
// managed by the frame, i.e. resized/moved when the frame is and whose size
// is accounted for in client size calculations - all others should be taken
// care of manually.

class wxFrame : public wxTopLevelWindow {
public:
    %addtofunc wxFrame         "self._setOORInfo(self)"
    %addtofunc wxFrame()       ""

    wxFrame(wxWindow* parent, const wxWindowID id, const wxString& title,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxDEFAULT_FRAME_STYLE,
            const wxString& name = wxPyFrameNameStr);
    %name(PreFrame)wxFrame();

    bool Create(wxWindow* parent, const wxWindowID id, const wxString& title,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxDEFAULT_FRAME_STYLE,
                const wxString& name = wxPyFrameNameStr);


    // frame state
    // -----------

    // get the origin of the client area (which may be different from (0, 0)
    // if the frame has a toolbar) in client coordinates
    virtual wxPoint GetClientAreaOrigin() const;

    // sends a size event to the window using its current size -- this has an
    // effect of refreshing the window layout
    //
    virtual void SendSizeEvent() { }


    // menu bar functions
    // ------------------

    virtual void SetMenuBar(wxMenuBar *menubar);
    virtual wxMenuBar *GetMenuBar() const;


    // process menu command: returns True if processed
    bool ProcessCommand(int winid);
    %pythoncode { Command = ProcessCommand }


    // status bar functions
    // --------------------

    // create the main status bar by calling OnCreateStatusBar()
    virtual wxStatusBar* CreateStatusBar(int number = 1,
                                         long style = wxST_SIZEGRIP,
                                         wxWindowID winid = 0,
                                         const wxString& name = wxPyStatusLineNameStr);
// TODO: with directors?
//     // return a new status bar
//     virtual wxStatusBar *OnCreateStatusBar(int number,
//                                            long style,
//                                            wxWindowID winid,
//                                            const wxString& name);

    // get the main status bar
    virtual wxStatusBar *GetStatusBar() const;

    // sets the main status bar
    void SetStatusBar(wxStatusBar *statBar);

    // forward these to status bar
    virtual void SetStatusText(const wxString &text, int number = 0);
    virtual void SetStatusWidths(int widths, const int* widths_field); //uses typemap above
    void PushStatusText(const wxString &text, int number = 0);
    void PopStatusText(int number = 0);

    // set the status bar pane the help will be shown in
    void SetStatusBarPane(int n);
    int GetStatusBarPane() const;


    // toolbar functions
    // -----------------

    // create main toolbar bycalling OnCreateToolBar()
    virtual wxToolBar* CreateToolBar(long style = -1,
                                     wxWindowID winid = -1,
                                     const wxString& name = wxPyToolBarNameStr);
// TODO: with directors?
//     // return a new toolbar
//     virtual wxToolBar *OnCreateToolBar(long style,
//                                        wxWindowID winid,
//                                        const wxString& name );

    // get/set the main toolbar
    virtual wxToolBar *GetToolBar() const;
    virtual void SetToolBar(wxToolBar *toolbar);


    // show help text (typically in the statusbar); show is False
    // if you are hiding the help, True otherwise
    virtual void DoGiveHelp(const wxString& text, bool show);


    // send wxUpdateUIEvents for all menu items in the menubar,
    // or just for menu if non-NULL
    void DoMenuUpdates(wxMenu* menu = NULL);
};

//---------------------------------------------------------------------------
%newgroup

class wxDialog : public wxTopLevelWindow {
public:
    %addtofunc wxDialog   "self._setOORInfo(self)"
    %addtofunc wxDialog() ""

    wxDialog(wxWindow* parent,
             const wxWindowID id,
             const wxString& title,
             const wxPoint& pos = wxDefaultPosition,
             const wxSize& size = wxDefaultSize,
             long style = wxDEFAULT_DIALOG_STYLE,
             const wxString& name = wxPyDialogNameStr);
    %name(PreDialog)wxDialog();

    bool Create(wxWindow* parent,
                const wxWindowID id,
                const wxString& title,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxDEFAULT_DIALOG_STYLE,
                const wxString& name = wxPyDialogNameStr);


    // the modal dialogs have a return code - usually the id of the last
    // pressed button
    void SetReturnCode(int returnCode) { m_returnCode = returnCode; }
    int GetReturnCode() const { return m_returnCode; }

    // splits text up at newlines and places the
    // lines into a vertical wxBoxSizer
    wxSizer *CreateTextSizer( const wxString &message );

    // places buttons into a horizontal wxBoxSizer
    wxSizer *CreateButtonSizer( long flags );


    void SetModal(bool flag);
    virtual bool IsModal() const;

    // Shows the dialog and starts a nested event loop that returns when
    // EndModal is called.
    virtual int ShowModal();

    // may be called to terminate the dialog with the given return code
    virtual void EndModal(int retCode);

    // returns True if we're in a modal loop
    %extend {
        bool IsModalShowing() {
        #ifdef __WXGTK__
            return self->m_modalShowing;
        #else
            return self->IsModalShowing();
        #endif
        }
    }
};

//---------------------------------------------------------------------------
%newgroup


class wxMiniFrame : public wxFrame {
public:
    %addtofunc wxMiniFrame         "self._setOORInfo(self)"
    %addtofunc wxMiniFrame()       ""

    wxMiniFrame(wxWindow* parent, const wxWindowID id, const wxString& title,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxDEFAULT_FRAME_STYLE,
                const wxString& name = wxPyFrameNameStr);
    %name(PreMiniFrame)wxMiniFrame();

    bool Create(wxWindow* parent, const wxWindowID id, const wxString& title,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxDEFAULT_FRAME_STYLE,
                const wxString& name = wxPyFrameNameStr);

};


//---------------------------------------------------------------------------
%newgroup


enum {
    wxSPLASH_CENTRE_ON_PARENT,
    wxSPLASH_CENTRE_ON_SCREEN,
    wxSPLASH_NO_CENTRE,
    wxSPLASH_TIMEOUT,
    wxSPLASH_NO_TIMEOUT,
};


class wxSplashScreenWindow: public wxWindow
{
public:
    %addtofunc wxSplashScreenWindow         "self._setOORInfo(self)"

    wxSplashScreenWindow(const wxBitmap& bitmap,
                         wxWindow* parent, wxWindowID id,
                         const wxPoint& pos = wxDefaultPosition,
                         const wxSize& size = wxDefaultSize,
                         long style = wxNO_BORDER);

    void SetBitmap(const wxBitmap& bitmap);
    wxBitmap& GetBitmap();
};


class wxSplashScreen : public wxFrame {
public:
    %addtofunc wxSplashScreen         "self._setOORInfo(self)"

    wxSplashScreen(const wxBitmap& bitmap, long splashStyle, int milliseconds,
                   wxWindow* parent, wxWindowID id,
                   const wxPoint& pos = wxDefaultPosition,
                   const wxSize& size = wxDefaultSize,
                   long style = wxSIMPLE_BORDER|wxFRAME_NO_TASKBAR|wxSTAY_ON_TOP);

    long GetSplashStyle() const;
    wxSplashScreenWindow* GetSplashWindow() const;
    int GetTimeout() const;
};


//---------------------------------------------------------------------------
