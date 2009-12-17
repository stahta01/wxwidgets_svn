/////////////////////////////////////////////////////////////////////////////
// Name:        wx/msw/window.h
// Purpose:     wxWindowMSW class
// Author:      Julian Smart
// Modified by: Vadim Zeitlin on 13.05.99: complete refont of message handling,
//              elimination of Default(), ...
// Created:     01/02/97
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_WINDOW_H_
#define _WX_WINDOW_H_

// if this is set to 1, we use deferred window sizing to reduce flicker when
// resizing complicated window hierarchies, but this can in theory result in
// different behaviour than the old code so we keep the possibility to use it
// by setting this to 0 (in the future this should be removed completely)
#ifdef __WXWINCE__
    #define wxUSE_DEFERRED_SIZING 0
#else
    #define wxUSE_DEFERRED_SIZING 1
#endif

// ---------------------------------------------------------------------------
// wxWindow declaration for MSW
// ---------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxWindowMSW : public wxWindowBase
{
    friend class wxSpinCtrl;
    friend class wxSlider;
    friend class wxRadioBox;
#if defined __VISUALC__ && __VISUALC__ <= 1200
    friend class wxWindowMSW;
#endif
public:
    wxWindowMSW() { Init(); }

    wxWindowMSW(wxWindow *parent,
                wxWindowID id,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0,
                const wxString& name = wxPanelNameStr)
    {
        Init();
        Create(parent, id, pos, size, style, name);
    }

    virtual ~wxWindowMSW();

    bool Create(wxWindow *parent,
                wxWindowID id,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0,
                const wxString& name = wxPanelNameStr);

    // implement base class pure virtuals
    virtual void SetLabel(const wxString& label);
    virtual wxString GetLabel() const;

    virtual void Raise();
    virtual void Lower();

    virtual bool Show(bool show = true);
    virtual bool ShowWithEffect(wxShowEffect effect,
                                unsigned timeout = 0)
    {
        return MSWShowWithEffect(true, effect, timeout);
    }
    virtual bool HideWithEffect(wxShowEffect effect,
                                unsigned timeout = 0)
    {
        return MSWShowWithEffect(false, effect, timeout);
    }

    virtual void SetFocus();
    virtual void SetFocusFromKbd();

    virtual bool Reparent(wxWindowBase *newParent);

    virtual void WarpPointer(int x, int y);

    virtual void Refresh( bool eraseBackground = true,
                          const wxRect *rect = (const wxRect *) NULL );
    virtual void Update();

    virtual void SetWindowStyleFlag(long style);
    virtual void SetExtraStyle(long exStyle);
    virtual bool SetCursor( const wxCursor &cursor );
    virtual bool SetFont( const wxFont &font );

    virtual int GetCharHeight() const;
    virtual int GetCharWidth() const;

    virtual void SetScrollbar( int orient, int pos, int thumbVisible,
                               int range, bool refresh = true );
    virtual void SetScrollPos( int orient, int pos, bool refresh = true );
    virtual int GetScrollPos( int orient ) const;
    virtual int GetScrollThumb( int orient ) const;
    virtual int GetScrollRange( int orient ) const;
    virtual void ScrollWindow( int dx, int dy,
                               const wxRect* rect = NULL );

    virtual bool ScrollLines(int lines);
    virtual bool ScrollPages(int pages);

    virtual void SetLayoutDirection(wxLayoutDirection dir);
    virtual wxLayoutDirection GetLayoutDirection() const;
    virtual wxCoord AdjustForLayoutDirection(wxCoord x,
                                             wxCoord width,
                                             wxCoord widthTotal) const;

#if wxUSE_DRAG_AND_DROP
    virtual void SetDropTarget( wxDropTarget *dropTarget );
#endif // wxUSE_DRAG_AND_DROP

    // Accept files for dragging
    virtual void DragAcceptFiles(bool accept);

#ifndef __WXUNIVERSAL__
    // Native resource loading (implemented in src/msw/nativdlg.cpp)
    // FIXME: should they really be all virtual?
    virtual bool LoadNativeDialog(wxWindow* parent, wxWindowID& id);
    virtual bool LoadNativeDialog(wxWindow* parent, const wxString& name);
    wxWindow* GetWindowChild1(wxWindowID id);
    wxWindow* GetWindowChild(wxWindowID id);
#endif // __WXUNIVERSAL__

#if wxUSE_HOTKEY
    // install and deinstall a system wide hotkey
    virtual bool RegisterHotKey(int hotkeyId, int modifiers, int keycode);
    virtual bool UnregisterHotKey(int hotkeyId);
#endif // wxUSE_HOTKEY

#ifdef __POCKETPC__
    bool IsContextMenuEnabled() const { return m_contextMenuEnabled; }
    void EnableContextMenu(bool enable = true) { m_contextMenuEnabled = enable; }
#endif

    // window handle stuff
    // -------------------

    WXHWND GetHWND() const { return m_hWnd; }
    void SetHWND(WXHWND hWnd) { m_hWnd = hWnd; }
    virtual WXWidget GetHandle() const { return GetHWND(); }

    void AssociateHandle(WXWidget handle);
    void DissociateHandle();

    // does this window have deferred position and/or size?
    bool IsSizeDeferred() const;

    // these functions allow to register a global handler for the given Windows
    // message: it will be called from MSWWindowProc() of any window which gets
    // this event if it's not processed before (i.e. unlike a hook procedure it
    // does not override the normal processing)
    //
    // notice that if you want to process a message for a given window only you
    // should override its MSWWindowProc() instead

    // type of the handler: it is called with the message parameters (except
    // that the window object is passed instead of window handle) and should
    // return true if it handled the message or false if it should be passed to
    // DefWindowProc()
    typedef bool (*MSWMessageHandler)(wxWindowMSW *win,
                                      WXUINT nMsg,
                                      WXWPARAM wParam,
                                      WXLPARAM lParam);

    // install a handler, shouldn't be called more than one for the same message
    static bool MSWRegisterMessageHandler(int msg, MSWMessageHandler handler);

    // unregister a previously registered handler
    static void MSWUnregisterMessageHandler(int msg, MSWMessageHandler handler);


    // implementation from now on
    // ==========================

    // event handlers
    // --------------

    void OnPaint(wxPaintEvent& event);
#ifdef __WXWINCE__
    void OnInitDialog(wxInitDialogEvent& event);
#endif

public:
    // Windows subclassing
    void SubclassWin(WXHWND hWnd);
    void UnsubclassWin();

    WXFARPROC MSWGetOldWndProc() const { return m_oldWndProc; }
    void MSWSetOldWndProc(WXFARPROC proc) { m_oldWndProc = proc; }

    // return true if the window is of a standard (i.e. not wxWidgets') class
    //
    // to understand why does it work, look at SubclassWin() code and comments
    bool IsOfStandardClass() const { return m_oldWndProc != NULL; }

    wxWindow *FindItem(long id) const;
    wxWindow *FindItemByHWND(WXHWND hWnd, bool controlOnly = false) const;

    // MSW only: true if this control is part of the main control
    virtual bool ContainsHWND(WXHWND WXUNUSED(hWnd)) const { return false; }

#if wxUSE_TOOLTIPS
    // MSW only: true if this window or any of its children have a tooltip
    virtual bool HasToolTips() const { return GetToolTip() != NULL; }
#endif // wxUSE_TOOLTIPS

    // translate wxWidgets style flags for this control into the Windows style
    // and optional extended style for the corresponding native control
    //
    // this is the function that should be overridden in the derived classes,
    // but you will mostly use MSWGetCreateWindowFlags() below
    virtual WXDWORD MSWGetStyle(long flags, WXDWORD *exstyle = NULL) const ;

    // get the MSW window flags corresponding to wxWidgets ones
    //
    // the functions returns the flags (WS_XXX) directly and puts the ext
    // (WS_EX_XXX) flags into the provided pointer if not NULL
    WXDWORD MSWGetCreateWindowFlags(WXDWORD *exflags = NULL) const
        { return MSWGetStyle(GetWindowStyle(), exflags); }

    // update the real underlying window style flags to correspond to the
    // current wxWindow object style (safe to call even if window isn't fully
    // created yet)
    void MSWUpdateStyle(long flagsOld, long exflagsOld);

    // translate wxWidgets coords into Windows ones suitable to be passed to
    // ::CreateWindow()
    //
    // returns true if non default coords are returned, false otherwise
    bool MSWGetCreateWindowCoords(const wxPoint& pos,
                                  const wxSize& size,
                                  int& x, int& y,
                                  int& w, int& h) const;

    // get the HWND to be used as parent of this window with CreateWindow()
    virtual WXHWND MSWGetParent() const;

    // get the Win32 window class name used by all wxWindow objects by default
    static const wxChar *MSWGetRegisteredClassName();

    // creates the window of specified Windows class with given style, extended
    // style, title and geometry (default values
    //
    // returns true if the window has been created, false if creation failed
    bool MSWCreate(const wxChar *wclass,
                   const wxChar *title = NULL,
                   const wxPoint& pos = wxDefaultPosition,
                   const wxSize& size = wxDefaultSize,
                   WXDWORD style = 0,
                   WXDWORD exendedStyle = 0);

    virtual bool MSWCommand(WXUINT param, WXWORD id);

#ifndef __WXUNIVERSAL__
    // Create an appropriate wxWindow from a HWND
    virtual wxWindow* CreateWindowFromHWND(wxWindow* parent, WXHWND hWnd);

    // Make sure the window style reflects the HWND style (roughly)
    virtual void AdoptAttributesFromHWND();
#endif // __WXUNIVERSAL__

    // Setup background and foreground colours correctly
    virtual void SetupColours();

    // ------------------------------------------------------------------------
    // helpers for message handlers: these perform the same function as the
    // message crackers from <windowsx.h> - they unpack WPARAM and LPARAM into
    // the correct parameters
    // ------------------------------------------------------------------------

    void UnpackCommand(WXWPARAM wParam, WXLPARAM lParam,
                       WXWORD *id, WXHWND *hwnd, WXWORD *cmd);
    void UnpackActivate(WXWPARAM wParam, WXLPARAM lParam,
                        WXWORD *state, WXWORD *minimized, WXHWND *hwnd);
    void UnpackScroll(WXWPARAM wParam, WXLPARAM lParam,
                      WXWORD *code, WXWORD *pos, WXHWND *hwnd);
    void UnpackCtlColor(WXWPARAM wParam, WXLPARAM lParam,
                        WXHDC *hdc, WXHWND *hwnd);
    void UnpackMenuSelect(WXWPARAM wParam, WXLPARAM lParam,
                          WXWORD *item, WXWORD *flags, WXHMENU *hmenu);

    // ------------------------------------------------------------------------
    // internal handlers for MSW messages: all handlers return a boolean value:
    // true means that the handler processed the event and false that it didn't
    // ------------------------------------------------------------------------

    // there are several cases where we have virtual functions for Windows
    // message processing: this is because these messages often require to be
    // processed in a different manner in the derived classes. For all other
    // messages, however, we do *not* have corresponding MSWOnXXX() function
    // and if the derived class wants to process them, it should override
    // MSWWindowProc() directly.

    // scroll event (both horizontal and vertical)
    virtual bool MSWOnScroll(int orientation, WXWORD nSBCode,
                             WXWORD pos, WXHWND control);

    // child control notifications
    virtual bool MSWOnNotify(int idCtrl, WXLPARAM lParam, WXLPARAM *result);

    // owner-drawn controls need to process these messages
    virtual bool MSWOnDrawItem(int id, WXDRAWITEMSTRUCT *item);
    virtual bool MSWOnMeasureItem(int id, WXMEASUREITEMSTRUCT *item);

    // the rest are not virtual
    bool HandleCreate(WXLPCREATESTRUCT cs, bool *mayCreate);
    bool HandleInitDialog(WXHWND hWndFocus);
    bool HandleDestroy();

    bool HandlePaint();
    bool HandlePrintClient(WXHDC hDC);
    bool HandleEraseBkgnd(WXHDC hDC);

    bool HandleMinimize();
    bool HandleMaximize();
    bool HandleSize(int x, int y, WXUINT flag);
    bool HandleSizing(wxRect& rect);
    bool HandleGetMinMaxInfo(void *mmInfo);
    bool HandleEnterSizeMove();
    bool HandleExitSizeMove();

    bool HandleShow(bool show, int status);
    bool HandleActivate(int flag, bool minimized, WXHWND activate);

    bool HandleCommand(WXWORD id, WXWORD cmd, WXHWND control);

    bool HandleCtlColor(WXHBRUSH *hBrush, WXHDC hdc, WXHWND hWnd);

    bool HandlePaletteChanged(WXHWND hWndPalChange);
    bool HandleQueryNewPalette();
    bool HandleSysColorChange();
    bool HandleDisplayChange();
    bool HandleCaptureChanged(WXHWND gainedCapture);
    virtual bool HandleSettingChange(WXWPARAM wParam, WXLPARAM lParam);

    bool HandleQueryEndSession(long logOff, bool *mayEnd);
    bool HandleEndSession(bool endSession, long logOff);

    bool HandleSetFocus(WXHWND wnd);
    bool HandleKillFocus(WXHWND wnd);

    bool HandleDropFiles(WXWPARAM wParam);

    bool HandleMouseEvent(WXUINT msg, int x, int y, WXUINT flags);
    bool HandleMouseMove(int x, int y, WXUINT flags);
    bool HandleMouseWheel(WXWPARAM wParam, WXLPARAM lParam);

    bool HandleChar(WXWPARAM wParam, WXLPARAM lParam, bool isASCII = false);
    bool HandleKeyDown(WXWPARAM wParam, WXLPARAM lParam);
    bool HandleKeyUp(WXWPARAM wParam, WXLPARAM lParam);
#if wxUSE_ACCEL
    bool HandleHotKey(WXWPARAM wParam, WXLPARAM lParam);
#endif
#ifdef __WIN32__
    int HandleMenuChar(int chAccel, WXLPARAM lParam);
#endif
    // Create and process a clipboard event specified by type.
    bool HandleClipboardEvent( WXUINT nMsg );

    bool HandleQueryDragIcon(WXHICON *hIcon);

    bool HandleSetCursor(WXHWND hWnd, short nHitTest, int mouseMsg);

    bool HandlePower(WXWPARAM wParam, WXLPARAM lParam, bool *vetoed);


    // Window procedure
    virtual WXLRESULT MSWWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam);

    // Calls an appropriate default window procedure
    virtual WXLRESULT MSWDefWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam);

    // message processing helpers

    // return false if the message shouldn't be translated/preprocessed but
    // dispatched normally
    virtual bool MSWShouldPreProcessMessage(WXMSG* pMsg);

    // return true if the message was preprocessed and shouldn't be dispatched
    virtual bool MSWProcessMessage(WXMSG* pMsg);

    // return true if the message was translated and shouldn't be dispatched
    virtual bool MSWTranslateMessage(WXMSG* pMsg);

    // called when the window is about to be destroyed
    virtual void MSWDestroyWindow();


    // this function should return the brush to paint the children controls
    // background or 0 if this window doesn't impose any particular background
    // on its children
    //
    // the base class version returns a solid brush if we have a non default
    // background colour or 0 otherwise
    virtual WXHBRUSH MSWGetBgBrushForChild(WXHDC hDC, WXHWND hWnd);

    // return the background brush to use for painting the given window by
    // quering the parent windows via their MSWGetBgBrushForChild() recursively
    //
    // hWndToPaint is normally NULL meaning this window itself, but it can also
    // be a child of this window which is used by the static box and could be
    // potentially useful for other transparent controls
    WXHBRUSH MSWGetBgBrush(WXHDC hDC, WXHWND hWndToPaint = NULL);

    // gives the parent the possibility to draw its children background, e.g.
    // this is used by wxNotebook to do it using DrawThemeBackground()
    //
    // return true if background was drawn, false otherwise
    virtual bool MSWPrintChild(WXHDC WXUNUSED(hDC), wxWindow * WXUNUSED(child))
    {
        return false;
    }

    // some controls (e.g. wxListBox) need to set the return value themselves
    //
    // return true to let parent handle it if we don't, false otherwise
    virtual bool MSWShouldPropagatePrintChild()
    {
        return true;
    }

    // common part of Show/HideWithEffect()
    bool MSWShowWithEffect(bool show,
                           wxShowEffect effect,
                           unsigned timeout);

    // Responds to colour changes: passes event on to children.
    void OnSysColourChanged(wxSysColourChangedEvent& event);

    // initialize various fields of wxMouseEvent (common part of MSWOnMouseXXX)
    void InitMouseEvent(wxMouseEvent& event, int x, int y, WXUINT flags);

    // check if mouse is in the window
    bool IsMouseInWindow() const;

    // check if a native double-buffering applies for this window
    virtual bool IsDoubleBuffered() const;

    void SetDoubleBuffered(bool on);

    // synthesize a wxEVT_LEAVE_WINDOW event and set m_mouseInWindow to false
    void GenerateMouseLeave();

    // virtual function for implementing internal idle
    // behaviour
    virtual void OnInternalIdle();

protected:
    // this allows you to implement standard control borders without
    // repeating the code in different classes that are not derived from
    // wxControl
    virtual wxBorder GetDefaultBorderForControl() const;

    // choose the default border for this window
    virtual wxBorder GetDefaultBorder() const;

    // Translate wxBORDER_THEME (and other border styles if necessary to the value
    // that makes most sense for this Windows environment
    virtual wxBorder TranslateBorder(wxBorder border) const;

#if wxUSE_MENUS_NATIVE
    virtual bool DoPopupMenu( wxMenu *menu, int x, int y );
#endif // wxUSE_MENUS_NATIVE

    // the window handle
    WXHWND                m_hWnd;

    // the old window proc (we subclass all windows)
    WXFARPROC             m_oldWndProc;

    // additional (MSW specific) flags
    bool                  m_mouseInWindow:1;
    bool                  m_lastKeydownProcessed:1;

    // the size of one page for scrolling
    int                   m_xThumbSize;
    int                   m_yThumbSize;

    // implement the base class pure virtuals
    virtual void DoGetTextExtent(const wxString& string,
                                 int *x, int *y,
                                 int *descent = NULL,
                                 int *externalLeading = NULL,
                                 const wxFont *font = NULL) const;
    virtual void DoClientToScreen( int *x, int *y ) const;
    virtual void DoScreenToClient( int *x, int *y ) const;
    virtual void DoGetPosition( int *x, int *y ) const;
    virtual void DoGetSize( int *width, int *height ) const;
    virtual void DoGetClientSize( int *width, int *height ) const;
    virtual void DoSetSize(int x, int y,
                           int width, int height,
                           int sizeFlags = wxSIZE_AUTO);
    virtual void DoSetClientSize(int width, int height);

    virtual wxSize DoGetBorderSize() const;

    virtual void DoCaptureMouse();
    virtual void DoReleaseMouse();

    virtual void DoEnable(bool enable);

    virtual void DoFreeze();
    virtual void DoThaw();

    // this simply moves/resizes the given HWND which is supposed to be our
    // sibling (this is useful for controls which are composite at MSW level
    // and for which DoMoveWindow() is not enough)
    //
    // returns true if the window move was deferred, false if it was moved
    // immediately (no error return)
    bool DoMoveSibling(WXHWND hwnd, int x, int y, int width, int height);

    // move the window to the specified location and resize it: this is called
    // from both DoSetSize() and DoSetClientSize() and would usually just call
    // ::MoveWindow() except for composite controls which will want to arrange
    // themselves inside the given rectangle
    virtual void DoMoveWindow(int x, int y, int width, int height);

#if wxUSE_TOOLTIPS
    virtual void DoSetToolTip( wxToolTip *tip );

    // process TTN_NEEDTEXT message properly (i.e. fixing the bugs in
    // comctl32.dll in our code -- see the function body for more info)
    bool HandleTooltipNotify(WXUINT code,
                             WXLPARAM lParam,
                             const wxString& ttip);
#endif // wxUSE_TOOLTIPS

    // the helper functions used by HandleChar/KeyXXX methods
    wxKeyEvent CreateKeyEvent(wxEventType evType, int id,
                              WXLPARAM lParam = 0, WXWPARAM wParam = 0) const;


    // default OnEraseBackground() implementation, return true if we did erase
    // the background, false otherwise (i.e. the system should erase it)
    bool DoEraseBackground(WXHDC hDC);

    // generate WM_CHANGEUISTATE if it's needed for the OS we're running under
    //
    // action should be one of the UIS_XXX constants
    // state should be one or more of the UISF_XXX constants
    // if action == UIS_INITIALIZE then it doesn't seem to matter what we use
    // for state as the system will decide for us what needs to be set
    void MSWUpdateUIState(int action, int state = 0);

private:
    // common part of all ctors
    void Init();

    // the (non-virtual) handlers for the events
    bool HandleMove(int x, int y);
    bool HandleMoving(wxRect& rect);
    bool HandleJoystickEvent(WXUINT msg, int x, int y, WXUINT flags);
    bool HandleNotify(int idCtrl, WXLPARAM lParam, WXLPARAM *result);

#if wxUSE_DEFERRED_SIZING
protected:
    // this function is called after the window was resized to its new size
    virtual void MSWEndDeferWindowPos()
    {
        m_pendingPosition = wxDefaultPosition;
        m_pendingSize = wxDefaultSize;
    }

    // current defer window position operation handle (may be NULL)
    WXHANDLE m_hDWP;

    // When deferred positioning is done these hold the pending changes, and
    // are used for the default values if another size/pos changes is done on
    // this window before the group of deferred changes is completed.
    wxPoint     m_pendingPosition;
    wxSize      m_pendingSize;
#endif // wxUSE_DEFERRED_SIZING

private:
#ifdef __POCKETPC__
    bool        m_contextMenuEnabled;
#endif

    DECLARE_DYNAMIC_CLASS(wxWindowMSW)
    wxDECLARE_NO_COPY_CLASS(wxWindowMSW);
    DECLARE_EVENT_TABLE()
};

// ----------------------------------------------------------------------------
// inline functions
// ----------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// global functions
// ---------------------------------------------------------------------------

// kbd code translation
WXDLLIMPEXP_CORE int wxCharCodeMSWToWX(int keySym, WXLPARAM lParam = 0);
WXDLLIMPEXP_CORE WXWORD wxCharCodeWXToMSW(int id);

// window creation helper class: before creating a new HWND, instantiate an
// object of this class on stack - this allows to process the messages sent to
// the window even before CreateWindow() returns
class wxWindowCreationHook
{
public:
    wxWindowCreationHook(wxWindowMSW *winBeingCreated);
    ~wxWindowCreationHook();
};

#endif // _WX_WINDOW_H_
