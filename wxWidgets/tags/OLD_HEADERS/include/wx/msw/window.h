/////////////////////////////////////////////////////////////////////////////
// Name:        window.h
// Purpose:     wxWindow class
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:           wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_WINDOW_H_
#define _WX_WINDOW_H_

#ifdef __GNUG__
#pragma interface "window.h"
#endif

#include "wx/gdicmn.h"
#include "wx/icon.h"
#include "wx/cursor.h"
#include "wx/pen.h"
#include "wx/font.h"
#include "wx/validate.h"
#include "wx/event.h"
#include "wx/string.h"
#include "wx/list.h"
#include "wx/region.h"
#include "wx/msw/accel.h"
#include "wx/intl.h"

#define wxKEY_SHIFT     1
#define wxKEY_CTRL      2

/*
 * Base class for frame, panel, canvas, panel items, dialog box.
 *
 */

/*
 * Event handler: windows have themselves as their event handlers
 * by default, but their event handlers could be set to another
 * object entirely. This separation can reduce the amount of
 * derivation required, and allow alteration of a window's functionality
 * (e.g. by a resource editor that temporarily switches event handlers).
 */

class WXDLLEXPORT wxWindow;
class WXDLLEXPORT wxEvent;
class WXDLLEXPORT wxCommandEvent;
class WXDLLEXPORT wxKeyEvent;
class WXDLLEXPORT wxControl;
class WXDLLEXPORT wxCursor;
class WXDLLEXPORT wxColourMap;
class WXDLLEXPORT wxFont;
class WXDLLEXPORT wxMenu;
class WXDLLEXPORT wxRect;
class WXDLLEXPORT wxBitmap;
class WXDLLEXPORT wxSizer;
class WXDLLEXPORT wxList;
class WXDLLEXPORT wxLayoutConstraints;
class WXDLLEXPORT wxMouseEvent;
class WXDLLEXPORT wxButton;
class WXDLLEXPORT wxColour;
class WXDLLEXPORT wxBrush;
class WXDLLEXPORT wxPen;
class WXDLLEXPORT wxIcon;
class WXDLLEXPORT wxDC;
class WXDLLEXPORT wxValidator;
class WXDLLEXPORT wxToolTip;

#if wxUSE_DRAG_AND_DROP
class WXDLLEXPORT wxDropTarget;
#endif

#if wxUSE_WX_RESOURCES
class WXDLLEXPORT wxResourceTable;
class WXDLLEXPORT wxItemResource;
#endif

WXDLLEXPORT_DATA(extern const wxChar*) wxPanelNameStr;

WXDLLEXPORT_DATA(extern const wxSize) wxDefaultSize;
WXDLLEXPORT_DATA(extern const wxPoint) wxDefaultPosition;

//-----------------------------------------------------------------------------
// wxClientData
//-----------------------------------------------------------------------------

class wxClientData
{
public:
    wxClientData() { }
    virtual ~wxClientData() { }
};

//-----------------------------------------------------------------------------
// wxStringClientData
//-----------------------------------------------------------------------------

class wxStringClientData: public wxClientData
{
public:
    wxStringClientData() { }
    wxStringClientData( wxString &data ) { m_data = data; }
    void SetData( wxString &data ) { m_data = data; }
    wxString GetData() const { return m_data; }

private:
    wxString  m_data;
};

// Clash with Windows headers
#ifdef GetCharWidth
#undef GetCharWidth
#endif

#ifdef FindWindow
#undef FindWindow
#endif

class WXDLLEXPORT wxWindow : public wxEvtHandler
{
    DECLARE_ABSTRACT_CLASS(wxWindow)

    friend class wxDC;
    friend class wxPaintDC;

public:
    wxWindow();
    wxWindow(wxWindow *parent, wxWindowID id,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = 0,
            const wxString& name = wxPanelNameStr)
    {
        Init();
        Create(parent, id, pos, size, style, name);
    }

    virtual ~wxWindow();

    bool Create(wxWindow *parent, wxWindowID id,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = 0,
            const wxString& name = wxPanelNameStr);

    // Fit the window around the items
    virtual void Fit();

    // Show or hide the window
    virtual bool Show(bool show);

    // Is the window shown?
    virtual bool IsShown() const;

    // Raise the window to the top of the Z order
    virtual void Raise();

    // Lower the window to the bottom of the Z order
    virtual void Lower();

    // Is the window enabled?
    virtual bool IsEnabled() const;

    // For compatibility
    inline bool Enabled() const { return IsEnabled(); }

    // Dialog support: override these and call
    // base class members to add functionality
    // that can't be done using validators.

    // Transfer values to controls. If returns FALSE,
    // it's an application error (pops up a dialog)
    virtual bool TransferDataToWindow();

    // Transfer values from controls. If returns FALSE,
    // transfer failed: don't quit
    virtual bool TransferDataFromWindow();

    // Validate controls. If returns FALSE,
    // validation failed: don't quit
    virtual bool Validate();

    // Return code for dialogs
    inline void SetReturnCode(int retCode);
    inline int GetReturnCode();

    // Set the cursor
    virtual void SetCursor(const wxCursor& cursor);
    inline virtual wxCursor& GetCursor() const { return (wxCursor& ) m_windowCursor; };

    // Get the window with the focus
    static wxWindow *FindFocus();

    // Get character size
    virtual int GetCharHeight() const;
    virtual int GetCharWidth() const;

    // moving/resizing
    // ---------------

    // set the window size and/or position
    void SetSize( int x, int y, int width, int height,
            int sizeFlags = wxSIZE_AUTO )
        {  DoSetSize(x, y, width, height, sizeFlags); }

    void SetSize( int width, int height )
        { DoSetSize(-1, -1, width, height, wxSIZE_USE_EXISTING); }

    void SetSize( const wxSize& size )
        { SetSize(size.x, size.y); }

    void SetSize(const wxRect& rect, int sizeFlags = wxSIZE_AUTO)
        { DoSetSize(rect.x, rect.y, rect.width, rect.height, sizeFlags); }

    void Move( int x, int y )
        { DoSetSize( x, y, -1, -1, wxSIZE_USE_EXISTING ); }

    void Move(const wxPoint& pt)
        { Move(pt.x, pt.y); }

    // client size is the size of area available for subwindows
    void SetClientSize( int width, int height )
        { DoSetClientSize(width, height); }

    void SetClientSize( const wxSize& size )
        { DoSetClientSize(size.x, size.y); }

    void SetClientSize(const wxRect& rect)
        { SetClientSize( rect.width, rect.height ); }

    // get the window position and/or size
    virtual void GetPosition( int *x, int *y ) const;
    wxPoint GetPosition() const
    {
        int w, h;
        GetPosition(& w, & h);

        return wxPoint(w, h);
    }

    virtual void GetSize( int *width, int *height ) const;

    wxSize GetSize() const
    {
        int w, h;
        GetSize(& w, & h);
        return wxSize(w, h);
    }

    wxRect GetRect() const
    {
        int x, y, w, h;
        GetPosition(& x, & y);
        GetSize(& w, & h);

        return wxRect(x, y, w, h);
    }

    virtual void GetClientSize( int *width, int *height ) const;
    wxSize GetClientSize() const
    {
        int w, h;
        GetClientSize(& w, & h);
        return wxSize(w, h);
    }

    wxRect GetClientRect() const
    {
        int w, h;
        GetSize(& w, & h);

        return wxRect(0, 0, w, h);
    }

    // Convert client to screen coordinates
    virtual void ClientToScreen(int *x, int *y) const;
    wxPoint ClientToScreen(const wxPoint& pt) const
    { int x = pt.x; int y = pt.y; ClientToScreen(& x, & y); return wxPoint(x, y); }

    // Convert screen to client coordinates
    virtual void ScreenToClient(int *x, int *y) const;
    wxPoint ScreenToClient(const wxPoint& pt) const
    { int x = pt.x; int y = pt.y; ScreenToClient(& x, & y); return wxPoint(x, y); }

    // Set the focus to this window
    virtual void SetFocus();

    // Capture/release mouse
    virtual void CaptureMouse();
    virtual void ReleaseMouse();

    // Enable or disable the window
    virtual void Enable(bool enable);

#if wxUSE_DRAG_AND_DROP
    // Associate a drop target with this window (if the window already had a drop
    // target, it's deleted!) and return the current drop target (may be NULL).
    void          SetDropTarget(wxDropTarget *pDropTarget);
    wxDropTarget *GetDropTarget() const { return m_pDropTarget; }
#endif

#if wxUSE_TOOLTIPS
    // tooltips
    // create a tooltip with this text
    void SetToolTip(const wxString &tip);
    // pointer may be NULL to remove the tooltip
    void SetToolTip(wxToolTip *tooltip);
    // get the current tooltip (may return NULL if none)
    wxToolTip* GetToolTip() const { return m_tooltip; }

#endif // wxUSE_TOOLTIPS

    // Accept files for dragging
    virtual void DragAcceptFiles(bool accept);

    // Update region access
    virtual wxRegion GetUpdateRegion() const;
    virtual bool IsExposed(int x, int y, int w, int h) const;
    virtual bool IsExposed(const wxPoint& pt) const;
    virtual bool IsExposed(const wxRect& rect) const;

    // Set/get the window title
    virtual inline void SetTitle(const wxString& WXUNUSED(title)) {};
    inline virtual wxString GetTitle() const { return wxString(""); };
    // Most windows have the concept of a label; for frames, this is the
    // title; for items, this is the label or button text.
    inline virtual wxString GetLabel() const { return GetTitle(); }

    // Set/get the window name (used for resource setting in X)
    inline virtual wxString GetName() const;
    inline virtual void SetName(const wxString& name);

    // Centre the window
    virtual void Centre(int direction) ;
    inline void Center(int direction = wxHORIZONTAL) { Centre(direction); }

    // Popup a menu
    virtual bool PopupMenu(wxMenu *menu, int x, int y);

    // Send the window a refresh event
    virtual void Refresh(bool eraseBack = TRUE, const wxRect *rect = NULL);

#if WXWIN_COMPATIBILITY
    // Set/get scroll attributes
    virtual void SetScrollRange(int orient, int range, bool refresh = TRUE);
    virtual void SetScrollPage(int orient, int page, bool refresh = TRUE);
    virtual int OldGetScrollRange(int orient) const;
    virtual int GetScrollPage(int orient) const;
#endif

    // New functions that will replace the above.
    virtual void SetScrollbar(int orient, int pos, int thumbVisible,
            int range, bool refresh = TRUE);

    virtual void SetScrollPos(int orient, int pos, bool refresh = TRUE);
    virtual int GetScrollPos(int orient) const;
    virtual int GetScrollRange(int orient) const;
    virtual int GetScrollThumb(int orient) const;

    virtual void ScrollWindow(int dx, int dy, const wxRect *rect = NULL);

    // Caret manipulation
    virtual void CreateCaret(int w, int h);
    virtual void CreateCaret(const wxBitmap *bitmap);
    virtual void DestroyCaret();
    virtual void ShowCaret(bool show);
    virtual void SetCaretPos(int x, int y);
    virtual void GetCaretPos(int *x, int *y) const;

    // Tell window how much it can be sized
    virtual void SetSizeHints(int minW = -1, int minH = -1, int maxW = -1, int maxH = -1, int incW = -1, int incH = -1);

    // Set/get the window's identifier
    inline int GetId() const;
    inline void SetId(int id);

    // Make the window modal (all other windows unresponsive)
    virtual void MakeModal(bool modal);

    // Get the private handle (platform-dependent)
    inline void *GetHandle() const;

    // Set/get the window's relatives
    inline wxWindow *GetParent() const;
    inline void SetParent(wxWindow *p) ;
    inline wxWindow *GetGrandParent() const;
    inline wxList& GetChildren() const;
    // Set this window to be the child of 'parent'.
    // Returns FALSE it's not possible (some systems
    // won't allow it)
    virtual bool Reparent(wxWindow *parent);

    // Set/get the window's font
    virtual void SetFont(const wxFont& f);
    inline virtual wxFont& GetFont() const;

    // Set/get the window's validator
    void SetValidator(const wxValidator& validator);
    inline wxValidator *GetValidator() const;

    // Set/get the window's style
    inline void SetWindowStyleFlag(long flag);
    inline long GetWindowStyleFlag() const;

    // Set/get double-clickability
    // TODO: we probably wish to get rid of this, and
    // always allow double clicks.
    inline void SetDoubleClick(bool flag);
    inline bool GetDoubleClick() const;
    inline void AllowDoubleClick(bool value)  { SetDoubleClick(value); }

    // Handle a control command
    virtual void OnCommand(wxWindow& win, wxCommandEvent& event);

    // Set/get event handler
    inline void SetEventHandler(wxEvtHandler *handler);
    inline wxEvtHandler *GetEventHandler() const;

    // Push/pop event handler (i.e. allow a chain of event handlers
    // be searched)
    void PushEventHandler(wxEvtHandler *handler) ;
    wxEvtHandler *PopEventHandler(bool deleteHandler = FALSE) ;

    // Close the window by calling OnClose, posting a deletion
    virtual bool Close(bool force = FALSE);

    // Destroy the window (delayed, if a managed window)
    virtual bool Destroy() ;

    // Mode for telling default OnSize members to
    // call Layout(), if not using Sizers, just top-down constraints
    inline void SetAutoLayout(bool a);
    inline bool GetAutoLayout() const;

    // Set/get constraints
    inline wxLayoutConstraints *GetConstraints() const;
    void SetConstraints(wxLayoutConstraints *c);

    // Set/get window background colour
    inline virtual void SetBackgroundColour(const wxColour& col);
    inline virtual wxColour GetBackgroundColour() const;

    // Set/get window foreground colour
    inline virtual void SetForegroundColour(const wxColour& col);
    inline virtual wxColour GetForegroundColour() const;

    // For backward compatibility
    inline virtual void SetButtonFont(const wxFont& font) { SetFont(font); }
    inline virtual void SetLabelFont(const wxFont& font) { SetFont(font); }
    inline virtual wxFont& GetLabelFont() const { return GetFont(); };
    inline virtual wxFont& GetButtonFont() const { return GetFont(); };

    // Get the default button, if there is one
    inline virtual wxButton *GetDefaultItem() const;
    inline virtual void SetDefaultItem(wxButton *but);

    virtual void SetAcceleratorTable(const wxAcceleratorTable& accel);
    inline virtual wxAcceleratorTable& GetAcceleratorTable() const { return (wxAcceleratorTable&) m_acceleratorTable; }

    // Override to define new behaviour for default action (e.g. double clicking
    // on a listbox)
    virtual void OnDefaultAction(wxControl *initiatingItem);

    // Resource loading
#if wxUSE_WX_RESOURCES
    virtual bool LoadFromResource(wxWindow *parent, const wxString& resourceName, const wxResourceTable *table = NULL);
    virtual wxControl *CreateItem(const wxItemResource* childResource, const wxItemResource* parentResource, const wxResourceTable *table = NULL);
#endif

    // Native resource loading
    virtual bool LoadNativeDialog(wxWindow* parent, wxWindowID& id);
    virtual bool LoadNativeDialog(wxWindow* parent, const wxString& name);
    virtual wxWindow* GetWindowChild1(wxWindowID& id);
    virtual wxWindow* GetWindowChild(wxWindowID& id);

    virtual void GetTextExtent(const wxString& string, int *x, int *y,
            int *descent = NULL,
            int *externalLeading = NULL,
            const wxFont *theFont = NULL, bool use16 = FALSE) const;

    // Is the window retained?
    inline bool IsRetained() const;

    // Warp the pointer the given position
    virtual void WarpPointer(int x_pos, int y_pos) ;

    // Clear the window
    virtual void Clear();

    // Find a window by id or name
    virtual wxWindow *FindWindow(long id);
    virtual wxWindow *FindWindow(const wxString& name);

    // Constraint operations
    bool Layout();
    void SetSizer(wxSizer *sizer);    // Adds sizer child to this window
    inline wxSizer *GetSizer() const ;
    inline wxWindow *GetSizerParent() const ;
    inline void SetSizerParent(wxWindow *win);

    // Do Update UI processing for controls
    void UpdateWindowUI();

    void OnEraseBackground(wxEraseEvent& event);
    void OnKeyDown(wxKeyEvent& event);
    void OnKeyUp(wxKeyEvent& event);
    void OnPaint(wxPaintEvent& event);
    void OnChar(wxKeyEvent& event);
    void OnIdle(wxIdleEvent& event);

    // Does this window want to accept keyboard focus?
    virtual bool AcceptsFocus() const;

    virtual void PrepareDC( wxDC& WXUNUSED(dc) ) {};
public:
    ////////////////////////////////////////////////////////////////////////
    //// IMPLEMENTATION

    // For implementation purposes - sometimes decorations make the client area
    // smaller
    virtual wxPoint GetClientAreaOrigin() const;

    // Makes an adjustment to the window position (for example, a frame that has
    // a toolbar that it manages itself).
    virtual void AdjustForParentClientOrigin(int& x, int& y, int sizeFlags);

    // Windows subclassing
    void SubclassWin(WXHWND hWnd);
    void UnsubclassWin();
    virtual long Default();
    virtual bool MSWCommand(WXUINT param, WXWORD id);

    // returns TRUE if the event was processed
    virtual bool MSWNotify(WXWPARAM wParam, WXLPARAM lParam, WXLPARAM *result);

    virtual wxWindow *FindItem(int id) const;
    virtual wxWindow *FindItemByHWND(WXHWND hWnd, bool controlOnly = FALSE) const ;
    virtual void PreDelete(WXHDC dc);              // Allows system cleanup
    // TO DO: how many of these need to be virtual?
    virtual WXHWND GetHWND() const ;
    virtual void SetHWND(WXHWND hWnd);

    // Make a Windows extended style from the given wxWindows window style
    virtual WXDWORD MakeExtendedStyle(long style, bool eliminateBorders = TRUE);
    // Determine whether 3D effects are wanted
    virtual WXDWORD Determine3DEffects(WXDWORD defaultBorderStyle, bool *want3D);

    virtual void AddChild(wxWindow *child);         // Adds reference to the child object
    virtual void RemoveChild(wxWindow *child);   // Removes reference to child
    // (but doesn't delete the child object)
    virtual void DestroyChildren();  // Removes and destroys all children

    inline bool IsBeingDeleted();

    // MSW only: TRUE if this control is part of the main control
    virtual bool ContainsHWND(WXHWND WXUNUSED(hWnd)) const { return FALSE; };

    // Constraint implementation
    void UnsetConstraints(wxLayoutConstraints *c);
    inline wxList *GetConstraintsInvolvedIn() const ;
    // Back-pointer to other windows we're involved with, so if we delete
    // this window, we must delete any constraints we're involved with.
    void AddConstraintReference(wxWindow *otherWin);
    void RemoveConstraintReference(wxWindow *otherWin);
    void DeleteRelatedConstraints();

    virtual void ResetConstraints();
    virtual void SetConstraintSizes(bool recurse = TRUE);
    virtual bool LayoutPhase1(int *noChanges);
    virtual bool LayoutPhase2(int *noChanges);
    virtual bool DoPhase(int);
    // Transforms from sizer coordinate space to actual
    // parent coordinate space
    virtual void TransformSizerToActual(int *x, int *y) const ;

    // Set size with transformation to actual coordinates if nec.
    virtual void SizerSetSize(int x, int y, int w, int h);
    virtual void SizerMove(int x, int y);

    // Only set/get the size/position of the constraint (if any)
    virtual void SetSizeConstraint(int x, int y, int w, int h);
    virtual void MoveConstraint(int x, int y);
    virtual void GetSizeConstraint(int *w, int *h) const ;
    virtual void GetClientSizeConstraint(int *w, int *h) const ;
    virtual void GetPositionConstraint(int *x, int *y) const ;

    // Dialog units translations. Implemented in wincmn.cpp.
    wxPoint ConvertPixelsToDialog(const wxPoint& pt) ;
    wxPoint ConvertDialogToPixels(const wxPoint& pt) ;
    inline wxSize ConvertPixelsToDialog(const wxSize& sz)
    { wxPoint pt(ConvertPixelsToDialog(wxPoint(sz.x, sz.y))); return wxSize(pt.x, pt.y); }
    inline wxSize ConvertDialogToPixels(const wxSize& sz)
    { wxPoint pt(ConvertDialogToPixels(wxPoint(sz.x, sz.y))); return wxSize(pt.x, pt.y); }

    wxObject *GetChild(int number) const ;

    void MSWCreate(int id, wxWindow *parent, const char *wclass, wxWindow *wx_win, const char *title,
            int x, int y, int width, int height,
            WXDWORD style, const char *dialog_template = NULL,
            WXDWORD exendedStyle = 0);

    // Actually defined in wx_canvs.cc since requires wxCanvas declaration
    virtual void MSWDeviceToLogical(float *x, float *y) const ;

    // Create an appropriate wxWindow from a HWND
    virtual wxWindow* CreateWindowFromHWND(wxWindow* parent, WXHWND hWnd);

    // Make sure the window style reflects the HWND style (roughly)
    virtual void AdoptAttributesFromHWND();

    // Setup background and foreground colours correctly
    virtual void SetupColours();

    // Saves the last message information before calling base version
    virtual bool ProcessEvent(wxEvent& event);

    // Handlers
    virtual void MSWOnCreate(WXLPCREATESTRUCT cs);
    virtual bool MSWOnPaint();
    virtual WXHICON MSWOnQueryDragIcon() { return 0; }
    virtual void MSWOnSize(int x, int y, WXUINT flag);
    virtual void MSWOnWindowPosChanging(void *lpPos);
    virtual void MSWOnHScroll(WXWORD nSBCode, WXWORD pos, WXHWND control);
    virtual void MSWOnVScroll(WXWORD nSBCode, WXWORD pos, WXHWND control);
    virtual bool MSWOnCommand(WXWORD id, WXWORD cmd, WXHWND control);
    virtual long MSWOnSysCommand(WXWPARAM wParam, WXLPARAM lParam);
    virtual long MSWOnNotify(WXWPARAM wParam, WXLPARAM lParam);
    virtual WXHBRUSH MSWOnCtlColor(WXHDC dc, WXHWND pWnd, WXUINT nCtlColor,
            WXUINT message, WXWPARAM wParam, WXLPARAM lParam);
    virtual bool MSWOnColorChange(WXHWND hWnd, WXUINT message, WXWPARAM wParam, WXLPARAM lParam);
    virtual long MSWOnPaletteChanged(WXHWND hWndPalChange);
    virtual long MSWOnQueryNewPalette();
    virtual bool MSWOnEraseBkgnd(WXHDC pDC);
    virtual void MSWOnMenuHighlight(WXWORD item, WXWORD flags, WXHMENU sysmenu);
    virtual void MSWOnInitMenuPopup(WXHMENU menu, int pos, bool isSystem);
    virtual bool MSWOnClose();
    // Return TRUE to end session, FALSE to veto end session.
    virtual bool MSWOnQueryEndSession(long logOff);
    virtual bool MSWOnEndSession(bool endSession, long logOff);
    virtual bool MSWOnDestroy();
    virtual bool MSWOnSetFocus(WXHWND wnd);
    virtual bool MSWOnKillFocus(WXHWND wnd);
    virtual void MSWOnDropFiles(WXWPARAM wParam);
    virtual bool MSWOnInitDialog(WXHWND hWndFocus);
    virtual void MSWOnShow(bool show, int status);

    // TODO: rationalise these functions into 1 or 2 which take the
    // event type as argument.
    virtual void MSWOnLButtonDown(int x, int y, WXUINT flags);
    virtual void MSWOnLButtonUp(int x, int y, WXUINT flags);
    virtual void MSWOnLButtonDClick(int x, int y, WXUINT flags);

    virtual void MSWOnMButtonDown(int x, int y, WXUINT flags);
    virtual void MSWOnMButtonUp(int x, int y, WXUINT flags);
    virtual void MSWOnMButtonDClick(int x, int y, WXUINT flags);

    virtual void MSWOnRButtonDown(int x, int y, WXUINT flags);
    virtual void MSWOnRButtonUp(int x, int y, WXUINT flags);
    virtual void MSWOnRButtonDClick(int x, int y, WXUINT flags);

    virtual void MSWOnMouseMove(int x, int y, WXUINT flags);
    virtual void MSWOnMouseEnter(int x, int y, WXUINT flags);
    virtual void MSWOnMouseLeave(int x, int y, WXUINT flags);

    // These return TRUE if an event handler was found, FALSE otherwise (not processed)
    virtual bool MSWOnChar(WXWORD wParam, WXLPARAM lParam, bool isASCII = FALSE);
    virtual bool MSWOnKeyDown(WXWORD wParam, WXLPARAM lParam);
    virtual bool MSWOnKeyUp(WXWORD wParam, WXLPARAM lParam);

    virtual bool MSWOnActivate(int flag, bool minimized, WXHWND activate);
    virtual long MSWOnMDIActivate(long flag, WXHWND activate, WXHWND deactivate);

    virtual bool MSWOnDrawItem(int id, WXDRAWITEMSTRUCT *item);
    virtual bool MSWOnMeasureItem(int id, WXMEASUREITEMSTRUCT *item);

    virtual void MSWOnJoyDown(int joystick, int x, int y, WXUINT flags);
    virtual void MSWOnJoyUp(int joystick, int x, int y, WXUINT flags);
    virtual void MSWOnJoyMove(int joystick, int x, int y, WXUINT flags);
    virtual void MSWOnJoyZMove(int joystick, int z, WXUINT flags);

    virtual long MSWGetDlgCode();

    // Window procedure
    virtual long MSWWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam);

    // Calls an appropriate default window procedure
    virtual long MSWDefWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam);
    virtual bool MSWProcessMessage(WXMSG* pMsg);
    virtual bool MSWTranslateMessage(WXMSG* pMsg);
    virtual void MSWDestroyWindow();

    // Detach "Window" menu from menu bar so it doesn't get deleted
    void MSWDetachWindowMenu();

    inline WXFARPROC MSWGetOldWndProc() const;
    inline void MSWSetOldWndProc(WXFARPROC proc);

    // Define for each class of dialog and control
    virtual WXHBRUSH OnCtlColor(WXHDC pDC, WXHWND pWnd, WXUINT nCtlColor,
            WXUINT message, WXWPARAM wParam, WXLPARAM lParam);

    inline void SetShowing(bool show);
    inline bool IsUserEnabled() const;
    inline bool GetUseCtl3D() const ;
    inline bool GetTransparentBackground() const ;

    // Responds to colour changes: passes event on to children.
    void OnSysColourChanged(wxSysColourChangedEvent& event);

    // Transfers data to any child controls
    void OnInitDialog(wxInitDialogEvent& event);

    // Sends an OnInitDialog event, which in turns transfers data to
    // to the window via validators.
    virtual void InitDialog();

    ////////////////////////////////////////////////////////////////////////
    //// PROTECTED DATA
protected:
    wxAcceleratorTable    m_acceleratorTable;
    int                   m_windowId;
    long                  m_windowStyle;                         // Store the window's style
    wxEvtHandler *        m_windowEventHandler;         // Usually is 'this'
    wxLayoutConstraints * m_constraints;           // Constraints for this window
    wxList *              m_constraintsInvolvedIn; // List of constraints we're involved in
    wxSizer *             m_windowSizer;                       // Window's top-level sizer (if any)
    wxWindow *            m_sizerParent;                      // Window's parent sizer (if any)
    bool                  m_autoLayout;                            // Whether to call Layout() in OnSize
    wxWindow *            m_windowParent;                     // Each window always knows its parent
    wxValidator *         m_windowValidator;
    // Old window proc, for subclassed controls
    WXFARPROC             m_oldWndProc;
    bool                  m_useCtl3D;             // Using CTL3D for this control

    bool                  m_inOnSize;                         // Protection against OnSize reentry
#ifndef _WX_WIN32__
    // Pointer to global memory, for EDIT controls that need
    // special treatment to reduce USER area consumption.
    WXHGLOBAL             m_globalHandle;
#endif

    bool                  m_winEnabled;
    int                   m_minSizeX;
    int                   m_minSizeY;
    int                   m_maxSizeX;
    int                   m_maxSizeY;

    // Caret data
    int                   m_caretWidth;
    int                   m_caretHeight;
    bool                  m_caretEnabled;
    bool                  m_caretShown;
    wxFont                m_windowFont;                               // Window's font
    bool                  m_isShown;
    bool                  m_doubleClickAllowed ;
    wxCursor              m_windowCursor;                        // Window's cursor
    bool                  m_winCaptured;
    wxString              m_windowName;                            // Window name

#if wxUSE_EXTENDED_STATICS
    wxList                m_staticItems;
#endif

    wxButton *            m_defaultItem;
    wxColour              m_backgroundColour ;
    wxColour              m_foregroundColour ;
    bool                  m_backgroundTransparent;

    int                   m_xThumbSize;
    int                   m_yThumbSize;

    float                 m_lastXPos;
    float                 m_lastYPos;
    int                   m_lastEvent;

    bool                  m_mouseInWindow;

#if wxUSE_DRAG_AND_DROP
    wxDropTarget         *m_pDropTarget;    // the current drop target or NULL
#endif  //USE_DRAG_AND_DROP

public:
    WXHWND                m_hWnd;                         // MS Windows window handle
    WXUINT                m_lastMsg;
    WXWPARAM              m_lastWParam;
    WXLPARAM              m_lastLParam;

    wxRegion              m_updateRegion;
    /*
       wxRect           m_updateRect;             // Bounding box for screen damage area
#ifdef __WIN32__
WXHRGN                m_updateRgn;                  // NT allows access to the rectangle list
#endif
     */

    //  WXHANDLE              m_acceleratorTable;
    WXHMENU               m_hMenu; // Menu, if any
    wxList *              m_children;                           // Window's children
    int                   m_returnCode;
    bool                  m_isBeingDeleted; // Fudge because can't access parent
    // when being deleted

    // this is the virtual function to be overriden in any derived class which
    // wants to change how SetSize() or Move() works - it is called by all
    // versions of these functions in the base class
    virtual void DoSetSize(int x, int y,
            int width, int height,
            int sizeFlags = wxSIZE_AUTO);

    // same as DoSetSize() for the client size
    virtual void DoSetClientSize(int width, int height);

private:
    // common part of all ctors
    void Init();

    // the associated tooltip (may be NULL if none)
#if wxUSE_TOOLTIPS
    wxToolTip *m_tooltip;
#endif // tooltips

    DECLARE_EVENT_TABLE()
};

////////////////////////////////////////////////////////////////////////
//// INLINES

inline void *wxWindow::GetHandle() const { return (void *)GetHWND(); }
inline int wxWindow::GetId() const { return m_windowId; }
inline void wxWindow::SetId(int id) { m_windowId = id; }
inline wxWindow *wxWindow::GetParent() const { return m_windowParent; }
inline void wxWindow::SetParent(wxWindow *p) { m_windowParent = p; }
inline wxWindow *wxWindow::GetGrandParent() const { return (m_windowParent ? m_windowParent->m_windowParent : (wxWindow*) NULL); }
inline wxList& wxWindow::GetChildren() const { return (wxList&) *m_children; }
inline wxFont& wxWindow::GetFont() const { return (wxFont&) m_windowFont; }
inline wxString wxWindow::GetName() const { return m_windowName; }
inline void wxWindow::SetName(const wxString& name) { m_windowName = name; }
inline long wxWindow::GetWindowStyleFlag() const { return m_windowStyle; }
inline void wxWindow::SetWindowStyleFlag(long flag) { m_windowStyle = flag; }
inline void wxWindow::SetDoubleClick(bool flag) { m_doubleClickAllowed = flag; }
inline bool wxWindow::GetDoubleClick() const { return m_doubleClickAllowed; }
inline void wxWindow::SetEventHandler(wxEvtHandler *handler) { m_windowEventHandler = handler; }
inline wxEvtHandler *wxWindow::GetEventHandler() const { return m_windowEventHandler; }
inline void wxWindow::SetAutoLayout(bool a) { m_autoLayout = a; }
inline bool wxWindow::GetAutoLayout() const { return m_autoLayout; }
inline wxLayoutConstraints *wxWindow::GetConstraints() const { return m_constraints; }
inline void wxWindow::SetBackgroundColour(const wxColour& col) { m_backgroundColour = col; };
inline wxColour wxWindow::GetBackgroundColour() const { return m_backgroundColour; };
inline void wxWindow::SetForegroundColour(const wxColour& col) { m_foregroundColour = col; };
inline wxColour wxWindow::GetForegroundColour() const { return m_foregroundColour; };

inline wxButton *wxWindow::GetDefaultItem() const { return m_defaultItem; }
inline void wxWindow::SetDefaultItem(wxButton *but) { m_defaultItem = but; }
inline bool wxWindow::IsRetained() const { return ((m_windowStyle & wxRETAINED) == wxRETAINED); }

inline void wxWindow::SetShowing(bool show) { m_isShown = show; }
inline wxList *wxWindow::GetConstraintsInvolvedIn() const { return m_constraintsInvolvedIn; }
inline wxSizer *wxWindow::GetSizer() const { return m_windowSizer; }
inline wxWindow *wxWindow::GetSizerParent() const { return m_sizerParent; }
inline void wxWindow::SetSizerParent(wxWindow *win) { m_sizerParent = win; }
inline WXFARPROC wxWindow::MSWGetOldWndProc() const { return m_oldWndProc; }
inline void wxWindow::MSWSetOldWndProc(WXFARPROC proc) { m_oldWndProc = proc; }
inline wxValidator *wxWindow::GetValidator() const { return m_windowValidator; }
inline bool wxWindow::IsUserEnabled() const { return m_winEnabled; }
inline bool wxWindow::GetUseCtl3D() const { return m_useCtl3D; }
inline bool wxWindow::GetTransparentBackground() const { return m_backgroundTransparent; }
inline void wxWindow::SetReturnCode(int retCode) { m_returnCode = retCode; }
inline int wxWindow::GetReturnCode() { return m_returnCode; }
inline bool wxWindow::IsBeingDeleted() { return m_isBeingDeleted; }

// Window specific (so far)
WXDLLEXPORT wxWindow* wxGetActiveWindow();

WXDLLEXPORT_DATA(extern wxWindowList) wxTopLevelWindows;

WXDLLEXPORT int wxCharCodeMSWToWX(int keySym);
WXDLLEXPORT int wxCharCodeWXToMSW(int id, bool *IsVirtual);

// Allocates control ids
WXDLLEXPORT int NewControlId();

#endif
    // _WX_WINDOW_H_
