/////////////////////////////////////////////////////////////////////////////
// Name:        window.h
// Purpose:     wxWindow class
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __WINDOWH__
#define __WINDOWH__

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
class WXDLLEXPORT wxRectangle;
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

#if USE_DRAG_AND_DROP
class wxDropTarget;
#endif

#if USE_WX_RESOURCES
class WXDLLEXPORT wxResourceTable;
class WXDLLEXPORT wxItemResource;
#endif

WXDLLEXPORT_DATA(extern const char*) wxPanelNameStr;

WXDLLEXPORT_DATA(extern const wxSize) wxDefaultSize;
WXDLLEXPORT_DATA(extern const wxPoint) wxDefaultPosition;

class WXDLLEXPORT wxWindow: public wxEvtHandler
{
  DECLARE_ABSTRACT_CLASS(wxWindow)

  friend class wxUpdateIterator;
  friend class wxDC;
  friend class wxPaintDC;

public:
  wxWindow(void);
  inline wxWindow(wxWindow *parent, const wxWindowID id,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize,
           const long style = 0,
           const wxString& name = wxPanelNameStr)
  {
  	  m_children = new wxList;
      Create(parent, id, pos, size, style, name);
  }

  virtual ~wxWindow(void);

  bool Create(wxWindow *parent, const wxWindowID id,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize,
           const long style = 0,
           const wxString& name = wxPanelNameStr);

  // Fit the window around the items
  virtual void Fit(void);

  // Show or hide the window
  virtual bool Show(const bool show);

  // Is the window shown?
  virtual bool IsShown(void) const;

  // Raise the window to the top of the Z order
  virtual void Raise(void);

  // Lower the window to the bottom of the Z order
  virtual void Lower(void);

  // Is the window enabled?
  virtual bool IsEnabled(void) const;

  // For compatibility
  inline bool Enabled(void) const { return IsEnabled(); }

  // Dialog support: override these and call
  // base class members to add functionality
  // that can't be done using validators.

  // Transfer values to controls. If returns FALSE,
  // it's an application error (pops up a dialog)
  virtual bool TransferDataToWindow(void);

  // Transfer values from controls. If returns FALSE,
  // transfer failed: don't quit
  virtual bool TransferDataFromWindow(void);

  // Validate controls. If returns FALSE,
  // validation failed: don't quit
  virtual bool Validate(void);

  // Return code for dialogs
  inline void SetReturnCode(int retCode);
  inline int GetReturnCode(void);

  // Set the cursor
  virtual void SetCursor(const wxCursor& cursor);
  inline virtual wxCursor *GetCursor(void) const { return (wxCursor *)& m_windowCursor; };

  // Get the window with the focus
  static wxWindow *FindFocus(void);

  // Get character size
  virtual int GetCharHeight(void) const;
  virtual int GetCharWidth(void) const;

  // Get overall window size
  virtual void GetSize(int *width, int *height) const;

  // Get window position, relative to parent (or screen if no parent)
  virtual void GetPosition(int *x, int *y) const;

  // Get client (application-useable) size
  virtual void GetClientSize(int *width, int *height) const;

  // Set overall size and position
  virtual void SetSize(const int x, const int y, const int width, const int height, const int sizeFlags = wxSIZE_AUTO);
  inline virtual void SetSize(const int width, const int height) { SetSize(-1, -1, width, height, wxSIZE_USE_EXISTING); }
  inline virtual void Move(const int x, const int y) { SetSize(x, y, -1, -1, wxSIZE_USE_EXISTING); }

  // Set client size
  virtual void SetClientSize(const int width, const int size);

  // Convert client to screen coordinates
  virtual void ClientToScreen(int *x, int *y) const;

  // Convert screen to client coordinates
  virtual void ScreenToClient(int *x, int *y) const;

  // Set the focus to this window
  virtual void SetFocus(void);

  // Capture/release mouse
  virtual void CaptureMouse(void);
  virtual void ReleaseMouse(void);

  // Enable or disable the window
  virtual void Enable(const bool enable);

#if USE_DRAG_AND_DROP
  // Associate a drop target with this window (if the window already had a drop
  // target, it's deleted!) and return the current drop target (may be NULL).
  void          SetDropTarget(wxDropTarget *pDropTarget);
  wxDropTarget *GetDropTarget() const { return m_pDropTarget; }
#endif

  // Accept files for dragging
  virtual void DragAcceptFiles(const bool accept);

  // Set/get the window title
  virtual inline void SetTitle(const wxString& WXUNUSED(title)) {};
  inline virtual wxString GetTitle(void) const { return wxString(""); };
  // Most windows have the concept of a label; for frames, this is the
  // title; for items, this is the label or button text.
  inline virtual wxString GetLabel(void) const { return GetTitle(); }

  // Set/get the window name (used for resource setting in X)
  inline virtual wxString GetName(void) const;
  inline virtual void SetName(const wxString& name);

  // Centre the window
  virtual void Centre(const int direction) ;
  inline void Center(const int direction = wxHORIZONTAL) { Centre(direction); }

  // Popup a menu
  virtual bool PopupMenu(wxMenu *menu, const int x, const int y);

  // Send the window a refresh event
  virtual void Refresh(const bool eraseBack = TRUE, const wxRectangle *rect = NULL);

#if WXWIN_COMPATIBILITY
  // Set/get scroll attributes
  virtual void SetScrollRange(const int orient, const int range, const bool refresh = TRUE);
  virtual void SetScrollPage(const int orient, const int page, const bool refresh = TRUE);
  virtual int OldGetScrollRange(const int orient) const;
  virtual int GetScrollPage(const int orient) const;
#endif

  // New functions that will replace the above.
  virtual void SetScrollbar(const int orient, const int pos, const int thumbVisible,
    const int range, const bool refresh = TRUE);

  virtual void SetScrollPos(const int orient, const int pos, const bool refresh = TRUE);
  virtual int GetScrollPos(const int orient) const;
  virtual int GetScrollRange(const int orient) const;
  virtual int GetScrollThumb(const int orient) const;

  virtual void ScrollWindow(const int dx, const int dy, const wxRectangle *rect = NULL);

  // Caret manipulation
  virtual void CreateCaret(const int w, const int h);
  virtual void CreateCaret(const wxBitmap *bitmap);
  virtual void DestroyCaret(void);
  virtual void ShowCaret(const bool show);
  virtual void SetCaretPos(const int x, const int y);
  virtual void GetCaretPos(int *x, int *y) const;

  // Tell window how much it can be sized
  virtual void SetSizeHints(const int minW = -1, const int minH = -1, const int maxW = -1, const int maxH = -1, const int incW = -1, const int incH = -1);

  // Set/get the window's identifier
  inline int GetId() const;
  inline void SetId(const int id);

  // Make the window modal (all other windows unresponsive)
  virtual void MakeModal(const bool modal);

  // Get the private handle (platform-dependent)
  inline void *GetHandle(void) const;

  // Set/get the window's relatives
  inline wxWindow *GetParent(void) const;
  inline void SetParent(wxWindow *p) ;
  inline wxWindow *GetGrandParent(void) const;
  inline wxList *GetChildren() const;

  // Set/get the window's font
  virtual void SetFont(const wxFont& f);
  inline virtual wxFont *GetFont(void) const;

  // Set/get the window's validator
  void SetValidator(const wxValidator& validator);
  inline wxValidator *GetValidator(void) const;

  // Set/get the window's style
  inline void SetWindowStyleFlag(const long flag);
  inline long GetWindowStyleFlag(void) const;

  // Set/get double-clickability
  // TODO: we probably wish to get rid of this, and
  // always allow double clicks.
  inline void SetDoubleClick(const bool flag);
  inline bool GetDoubleClick(void) const;
  inline void AllowDoubleClick(const bool value)  { SetDoubleClick(value); }

  // Old way to handle a control command
  virtual void OnCommand(wxWindow& win, wxCommandEvent& event);

  // Set/get event handler
  inline void SetEventHandler(wxEvtHandler *handler);
  inline wxEvtHandler *GetEventHandler(void) const;

  // Push/pop event handler (i.e. allow a chain of event handlers
  // be searched)
  void PushEventHandler(wxEvtHandler *handler) ;
  wxEvtHandler *PopEventHandler(bool deleteHandler = FALSE) ;
  
  // Close the window by calling OnClose, posting a deletion
  virtual bool Close(const bool force = FALSE);

  // Destroy the window (delayed, if a managed window)
  virtual bool Destroy(void) ;

  // Mode for telling default OnSize members to
  // call Layout(), if not using Sizers, just top-down constraints
  inline void SetAutoLayout(const bool a);
  inline bool GetAutoLayout(void) const;

  // Set/get constraints
  inline wxLayoutConstraints *GetConstraints(void) const;
  void SetConstraints(wxLayoutConstraints *c);

  // Set/get window background colour
  inline virtual void SetBackgroundColour(const wxColour& col);
  inline virtual wxColour GetBackgroundColour(void) const;

  // Set/get window foreground colour
  inline virtual void SetForegroundColour(const wxColour& col);
  inline virtual wxColour GetForegroundColour(void) const;

  // TODO: are these really necessary???
  // Set/get window default background colour (for children to inherit)
  inline virtual void SetDefaultBackgroundColour(const wxColour& col);
  inline virtual wxColour GetDefaultBackgroundColour(void) const;

  // Set/get window default foreground colour (for children to inherit)
  inline virtual void SetDefaultForegroundColour(const wxColour& col);
  inline virtual wxColour GetDefaultForegroundColour(void) const;

  // For backward compatibility
  inline virtual void SetButtonFont(const wxFont& font) { SetFont(font); }
  inline virtual void SetLabelFont(const wxFont& font) { SetFont(font); }
  inline virtual wxFont  *GetLabelFont(void) const { return GetFont(); };
  inline virtual wxFont  *GetButtonFont(void) const { return GetFont(); };

  // Get the default button, if there is one
  inline virtual wxButton *GetDefaultItem(void) const;
  inline virtual void SetDefaultItem(wxButton *but);

  // Override to define new behaviour for default action (e.g. double clicking
  // on a listbox)
  virtual void OnDefaultAction(wxControl *initiatingItem);

  // Resource loading
#if USE_WX_RESOURCES
  virtual bool LoadFromResource(wxWindow *parent, const wxString& resourceName, const wxResourceTable *table = NULL);
  virtual wxControl *CreateItem(const wxItemResource *childResource, const wxResourceTable *table = NULL);
#endif

  // Native resource loading
  virtual bool LoadNativeDialog(wxWindow* parent, const wxWindowID& id);
  virtual bool LoadNativeDialog(wxWindow* parent, const wxString& name);
  virtual wxWindow* GetWindowChild1(const wxWindowID& id);
  virtual wxWindow* GetWindowChild(const wxWindowID& id);

  virtual void GetTextExtent(const wxString& string, int *x, int *y,
                             int *descent = NULL,
                             int *externalLeading = NULL,
                             const wxFont *theFont = NULL, const bool use16 = FALSE) const;
#if WXWIN_COMPATIBILITY
  void GetTextExtent(const wxString& string, float *x, float *y,
                             float *descent = NULL,
                             float *externalLeading = NULL,
                             const wxFont *theFont = NULL, const bool use16 = FALSE) const;
#endif

  // Gets 'context' member
  // OBSOLETE
  // TODO: how to make backward compatible?
//  inline wxDC *GetDC(void) const;

  // Is the window retained?
  inline bool IsRetained(void) const;

  // Set the window's colourmap/palette
  // OBSOLETE
//  virtual void SetPalette(wxPalette *palette);

  // Warp the pointer the given position
  virtual void WarpPointer(const int x_pos, const int y_pos) ;

  // Clear the window
  virtual void Clear(void);

  // Find a window by id or name
  virtual wxWindow *FindWindow(const long id);
  virtual wxWindow *FindWindow(const wxString& name);

  // Constraint operations
  bool Layout(void);
  void SetSizer(wxSizer *sizer);    // Adds sizer child to this window
  inline wxSizer *GetSizer(void) const ;
  inline wxWindow *GetSizerParent(void) const ;
  inline void SetSizerParent(wxWindow *win);

  // Do Update UI processing for controls
  void UpdateWindowUI(void);

  void OnSize(wxSizeEvent& event);
  void OnEraseBackground(wxEraseEvent& event);
  void OnChar(wxKeyEvent& event);
  void OnPaint(wxPaintEvent& event);
  void OnIdle(wxIdleEvent& event);

//  virtual void OnChangeFocus(wxControl *from, wxControl *to);
//  virtual bool OnFunctionKey(wxKeyEvent &event);

#if WXWIN_COMPATIBILITY
  virtual void OldOnMenuSelect(int WXUNUSED(cmd));
  virtual void OldOnInitMenuPopup(int WXUNUSED(pos));
  virtual void OldOnScroll(wxCommandEvent& WXUNUSED(event));
  virtual void OldOnPaint(void);                 // Called when needs painting
  virtual void OldOnSize(int width, int height);           // Called on resize
  virtual void OldOnMouseEvent(wxMouseEvent& event);  // Called on mouse event
  virtual void OldOnChar(wxKeyEvent& event);     // Called on character event
  virtual void OldOnMenuCommand(int cmd); // Dealt with properly in wxFrame
  inline virtual void OldOnMove(int WXUNUSED(x), int WXUNUSED(y));          // Called on move
  inline virtual void OldOnActivate(bool WXUNUSED(active));       // Called on window activation (MSW)
  virtual void OldOnSetFocus(void);              // Called on setting focus
  virtual void OldOnKillFocus(void);             // Called on killing focus
#endif

/* THIS IS NOW OBSOLETE - all positions are in device units
  // Calculates the position of a point on the window
  // taking into account the position of scrollbars.
  // Windows doesn't automatically reflect the position of the
  // scrollbars - (0, 0) is always the top left of the visible window,
  // whereas in XView, (0, 0) moves according to scrollbar positions.
  virtual void CalcScrolledPosition(const int x, const int y, int *xx, int *yy) const ;

  // Calculate logical (scroll-bar/scaling aware) position from
  // device (pixel) position
  virtual void CalcUnscrolledPosition(const int x, const int y, float *xx, float *yy) const ;
*/

public:
  ////////////////////////////////////////////////////////////////////////
  //// IMPLEMENTATION
  
  // Windows subclassing
  void SubclassWin(WXHWND hWnd);
  void UnsubclassWin(void);
  virtual long Default(void);
  virtual bool MSWCommand(const WXUINT param, const WXWORD id);
  virtual bool MSWNotify(const WXWPARAM wParam, const WXLPARAM lParam);
  virtual wxWindow *FindItem(const int id) const;
  virtual wxWindow *FindItemByHWND(const WXHWND hWnd, bool controlOnly = FALSE) const ;
  virtual void PreDelete(const WXHDC dc);              // Allows system cleanup
  // TO DO: how many of these need to be virtual?
  virtual WXHWND GetHWND(void) const ;
  virtual void SetHWND(WXHWND hWnd);

  // Make a Windows extended style from the given wxWindows window style
  virtual WXDWORD MakeExtendedStyle(long style, bool eliminateBorders = TRUE);
  // Determine whether 3D effects are wanted
  virtual WXDWORD Determine3DEffects(WXDWORD defaultBorderStyle, bool *want3D);

  virtual void AddChild(wxWindow *child);         // Adds reference to the child object
  virtual void RemoveChild(wxWindow *child);   // Removes reference to child
                                       // (but doesn't delete the child object)
  virtual void DestroyChildren(void);  // Removes and destroys all children

  inline bool IsBeingDeleted(void);

  // MSW only: TRUE if this control is part of the main control
  virtual bool ContainsHWND(WXHWND WXUNUSED(hWnd)) const { return FALSE; };

  // Constraint implementation
  void UnsetConstraints(wxLayoutConstraints *c);
  inline wxList *GetConstraintsInvolvedIn(void) const ;
  // Back-pointer to other windows we're involved with, so if we delete
  // this window, we must delete any constraints we're involved with.
  void AddConstraintReference(wxWindow *otherWin);
  void RemoveConstraintReference(wxWindow *otherWin);
  void DeleteRelatedConstraints(void);

  virtual void ResetConstraints(void);
  virtual void SetConstraintSizes(const bool recurse = TRUE);
  virtual bool LayoutPhase1(int *noChanges);
  virtual bool LayoutPhase2(int *noChanges);
  virtual bool DoPhase(const int);
  // Transforms from sizer coordinate space to actual
  // parent coordinate space
  virtual void TransformSizerToActual(int *x, int *y) const ;

  // Set size with transformation to actual coordinates if nec.
  virtual void SizerSetSize(const int x, const int y, const int w, const int h);
  virtual void SizerMove(const int x, const int y);

  // Only set/get the size/position of the constraint (if any)
  virtual void SetSizeConstraint(const int x, const int y, const int w, const int h);
  virtual void MoveConstraint(const int x, const int y);
  virtual void GetSizeConstraint(int *w, int *h) const ;
  virtual void GetClientSizeConstraint(int *w, int *h) const ;
  virtual void GetPositionConstraint(int *x, int *y) const ;

  wxObject *GetChild(const int number) const ;

  void MSWCreate(const int id, wxWindow *parent, const char *wclass, wxWindow *wx_win, const char *title,
               const int x, const int y, const int width, const int height,
               const WXDWORD style, const char *dialog_template = NULL,
               const WXDWORD exendedStyle = 0);

  // Actually defined in wx_canvs.cc since requires wxCanvas declaration
  virtual void MSWDeviceToLogical(float *x, float *y) const ;

  // Create an appropriate wxWindow from a HWND
  virtual wxWindow* CreateWindowFromHWND(wxWindow* parent, WXHWND hWnd);

  // Make sure the window style reflects the HWND style (roughly)
  virtual void AdoptAttributesFromHWND(void);

  // Setup background and foreground colours correctly
  virtual void SetupColours(void);

  // Handlers
  virtual void MSWOnCreate(WXLPCREATESTRUCT cs);
  virtual bool MSWOnPaint(void);
  virtual WXHICON MSWOnQueryDragIcon(void) { return 0; }
  virtual void MSWOnSize(const int x, const int y, const WXUINT flag);
  virtual void MSWOnWindowPosChanging(void *lpPos);
  virtual void MSWOnHScroll(const WXWORD nSBCode, const WXWORD pos, const WXHWND control);
  virtual void MSWOnVScroll(const WXWORD nSBCode, const WXWORD pos, const WXHWND control);
  virtual bool MSWOnCommand(const WXWORD id, const WXWORD cmd, const WXHWND control);
  virtual long MSWOnSysCommand(WXWPARAM wParam, WXLPARAM lParam);
  virtual bool MSWOnNotify(const WXWPARAM wParam, const WXLPARAM lParam);
  virtual WXHBRUSH MSWOnCtlColor(const WXHDC dc, const WXHWND pWnd, const WXUINT nCtlColor,
                              const WXUINT message, const WXWPARAM wParam, const WXLPARAM lParam);
  virtual bool MSWOnColorChange(const WXHWND hWnd, const WXUINT message, const WXWPARAM wParam, const WXLPARAM lParam);
  virtual bool MSWOnEraseBkgnd(const WXHDC pDC);
  virtual void MSWOnMenuHighlight(const WXWORD item, const WXWORD flags, const WXHMENU sysmenu);
  virtual void MSWOnInitMenuPopup(const WXHMENU menu, const int pos, const bool isSystem);
  virtual bool MSWOnClose(void);
  virtual bool MSWOnDestroy(void);
  virtual bool MSWOnSetFocus(const WXHWND wnd);
  virtual bool MSWOnKillFocus(const WXHWND wnd);
  virtual void MSWOnDropFiles(const WXWPARAM wParam);
  virtual bool MSWOnInitDialog(WXHWND hWndFocus);
  virtual void MSWOnShow(bool show, int status);

  // TODO: rationalise these functions into 1 or 2 which take the
  // event type as argument.
  virtual void MSWOnLButtonDown(const int x, const int y, const WXUINT flags);
  virtual void MSWOnLButtonUp(const int x, const int y, const WXUINT flags);
  virtual void MSWOnLButtonDClick(const int x, const int y, const WXUINT flags);

  virtual void MSWOnMButtonDown(const int x, const int y, const WXUINT flags);
  virtual void MSWOnMButtonUp(const int x, const int y, const WXUINT flags);
  virtual void MSWOnMButtonDClick(const int x, const int y, const WXUINT flags);

  virtual void MSWOnRButtonDown(const int x, const int y, const WXUINT flags);
  virtual void MSWOnRButtonUp(const int x, const int y, const WXUINT flags);
  virtual void MSWOnRButtonDClick(const int x, const int y, const WXUINT flags);

  virtual void MSWOnMouseMove(const int x, const int y, const WXUINT flags);
  virtual void MSWOnMouseEnter(const int x, const int y, const WXUINT flags);
  virtual void MSWOnMouseLeave(const int x, const int y, const WXUINT flags);

  virtual void MSWOnChar(const WXWORD wParam, const WXLPARAM lParam, const bool isASCII = FALSE);

  virtual bool MSWOnActivate(const int flag, const bool minimized, const WXHWND activate);
  virtual long MSWOnMDIActivate(const long flag, const WXHWND activate, const WXHWND deactivate);

  virtual bool MSWOnDrawItem(const int id, WXDRAWITEMSTRUCT *item);
  virtual bool MSWOnMeasureItem(const int id, WXMEASUREITEMSTRUCT *item);

  virtual void MSWOnJoyDown(const int joystick, const int x, const int y, const WXUINT flags);
  virtual void MSWOnJoyUp(const int joystick, const int x, const int y, const WXUINT flags);
  virtual void MSWOnJoyMove(const int joystick, const int x, const int y, const WXUINT flags);
  virtual void MSWOnJoyZMove(const int joystick, const int z, const WXUINT flags);

  // Window procedure
  virtual long MSWWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam);

  // Calls an appropriate default window procedure
  virtual long MSWDefWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam);
  virtual bool MSWProcessMessage(WXMSG* pMsg);
  virtual void MSWDestroyWindow(void);

  // Detach "Window" menu from menu bar so it doesn't get deleted
  void MSWDetachWindowMenu(void);
  
  inline WXFARPROC MSWGetOldWndProc() const;
  inline void MSWSetOldWndProc(const WXFARPROC proc);

  // Define for each class of dialog and control
  virtual WXHBRUSH OnCtlColor(const WXHDC pDC, const WXHWND pWnd, const WXUINT nCtlColor,
			WXUINT message, WXWPARAM wParam, WXLPARAM lParam);

  inline void SetShowing(const bool show);
  inline bool IsUserEnabled(void) const;
  inline bool GetUseCtl3D(void) const ;
  inline bool GetTransparentBackground(void) const ;

  // Responds to colour changes: passes event on to children.
  void OnSysColourChanged(wxSysColourChangedEvent& event);

  // Transfers data to any child controls
  void OnInitDialog(wxInitDialogEvent& event);

  // Sends an OnInitDialog event, which in turns transfers data to
  // to the window via validators.
  virtual void InitDialog(void);

  ////////////////////////////////////////////////////////////////////////
  //// PROTECTED DATA
protected:
  int                   m_windowId;
  long                  m_windowStyle; 			// Store the window's style
  wxEvtHandler *        m_windowEventHandler; 	// Usually is 'this'
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

  bool                  m_inOnSize; 			// Protection against OnSize reentry
#ifndef __WIN32__
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

  // Device context being stored whilst drawing is done
//  WXHDC                 m_tempHDC;
  // Temporary device context stored during an OnPaint
//  WXHDC                 m_paintHDC;
  wxFont                m_windowFont;                               // Window's font
  bool                  m_isShown;
  bool                  m_doubleClickAllowed ;
  wxCursor              m_windowCursor;                        // Window's cursor
  bool                  m_winCaptured;
  wxString              m_windowName;                            // Window name

#if USE_EXTENDED_STATICS
  wxList                m_staticItems;
#endif

  wxButton *            m_defaultItem;

  wxColour              m_backgroundColour ;
  wxColour              m_defaultBackgroundColour;

  wxColour              m_foregroundColour ;
  wxColour              m_defaultForegroundColour;

  bool                  m_backgroundTransparent;

//  wxDC *                m_windowDC;    // The canvas's device context

  int                   m_xThumbSize;
  int                   m_yThumbSize;

  float                 m_lastXPos;
  float                 m_lastYPos;
  int                   m_lastEvent;

  bool                  m_mouseInWindow;

#if USE_DRAG_AND_DROP
  wxDropTarget         *m_pDropTarget;    // the current drop target or NULL
#endif  //USE_DRAG_AND_DROP

public:
  WXHWND                m_hWnd; 			// MS Windows window handle
  WXUINT                m_lastMsg;
  WXWPARAM              m_lastWParam;
  WXLPARAM              m_lastLParam;
  wxRectangle           m_updateRect;             // Bounding box for screen damage area
#ifdef __WIN32__
  WXHRGN                m_updateRgn;                  // NT allows access to the rectangle list
#endif
  WXHANDLE              m_acceleratorTable;
  WXHMENU               m_hMenu; // Menu, if any
  wxList *              m_children;                           // Window's children
  int                   m_returnCode;
  bool                  m_isBeingDeleted; // Fudge because can't access parent
                        // when being deleted

DECLARE_EVENT_TABLE()
};

////////////////////////////////////////////////////////////////////////
//// INLINES

inline void *wxWindow::GetHandle(void) const { return (void *)GetHWND(); }
inline int wxWindow::GetId() const { return m_windowId; }
inline void wxWindow::SetId(const int id) { m_windowId = id; }
inline wxWindow *wxWindow::GetParent(void) const { return m_windowParent; }
inline void wxWindow::SetParent(wxWindow *p) { m_windowParent = p; }
inline wxWindow *wxWindow::GetGrandParent(void) const { return (m_windowParent ? m_windowParent->m_windowParent : NULL); }
inline wxList *wxWindow::GetChildren() const { return m_children; }
inline wxFont *wxWindow::GetFont(void) const { return (wxFont *) & m_windowFont; }
inline wxString wxWindow::GetName(void) const { return m_windowName; }
inline void wxWindow::SetName(const wxString& name) { m_windowName = name; }
inline long wxWindow::GetWindowStyleFlag(void) const { return m_windowStyle; }
inline void wxWindow::SetWindowStyleFlag(const long flag) { m_windowStyle = flag; }
inline void wxWindow::SetDoubleClick(const bool flag) { m_doubleClickAllowed = flag; }
inline bool wxWindow::GetDoubleClick(void) const { return m_doubleClickAllowed; }
inline void wxWindow::SetEventHandler(wxEvtHandler *handler) { m_windowEventHandler = handler; }
inline wxEvtHandler *wxWindow::GetEventHandler(void) const { return m_windowEventHandler; }
inline void wxWindow::SetAutoLayout(const bool a) { m_autoLayout = a; }
inline bool wxWindow::GetAutoLayout(void) const { return m_autoLayout; }
inline wxLayoutConstraints *wxWindow::GetConstraints(void) const { return m_constraints; }
inline void wxWindow::SetBackgroundColour(const wxColour& col) { m_backgroundColour = col; };
inline wxColour wxWindow::GetBackgroundColour(void) const { return m_backgroundColour; };
inline void wxWindow::SetForegroundColour(const wxColour& col) { m_foregroundColour = col; };
inline wxColour wxWindow::GetForegroundColour(void) const { return m_foregroundColour; };
inline void wxWindow::SetDefaultForegroundColour(const wxColour& col) { m_defaultForegroundColour = col; };
inline wxColour wxWindow::GetDefaultForegroundColour(void) const { return m_defaultForegroundColour; };
inline void wxWindow::SetDefaultBackgroundColour(const wxColour& col) { m_defaultBackgroundColour = col; };
inline wxColour wxWindow::GetDefaultBackgroundColour(void) const { return m_defaultBackgroundColour; };

inline wxButton *wxWindow::GetDefaultItem(void) const { return m_defaultItem; }
inline void wxWindow::SetDefaultItem(wxButton *but) { m_defaultItem = but; }
// inline wxDC *wxWindow::GetDC(void) const { return m_windowDC; }
inline bool wxWindow::IsRetained(void) const { return ((m_windowStyle & wxRETAINED) == wxRETAINED); }
#if WXWIN_COMPATIBILITY
inline void wxWindow::OldOnMenuSelect(int WXUNUSED(cmd)) { Default(); };
inline void wxWindow::OldOnInitMenuPopup(int WXUNUSED(pos)) { Default(); };
inline void wxWindow::OldOnScroll(wxCommandEvent& WXUNUSED(event)) { Default(); };
inline void wxWindow::OldOnMenuCommand(int WXUNUSED(cmd)) { Default(); } // Dealt with properly in wxFrame
inline void wxWindow::OldOnMove(int WXUNUSED(x), int WXUNUSED(y)) { Default(); };          // Called on move
inline void wxWindow::OldOnActivate(bool WXUNUSED(active)) { Default(); };       // Called on window activation (MSW)
#endif
inline void wxWindow::SetShowing(const bool show) { m_isShown = show; }
inline wxList *wxWindow::GetConstraintsInvolvedIn(void) const { return m_constraintsInvolvedIn; }
inline wxSizer *wxWindow::GetSizer(void) const { return m_windowSizer; }
inline wxWindow *wxWindow::GetSizerParent(void) const { return m_sizerParent; }
inline void wxWindow::SetSizerParent(wxWindow *win) { m_sizerParent = win; }
inline WXFARPROC wxWindow::MSWGetOldWndProc() const { return m_oldWndProc; }
inline void wxWindow::MSWSetOldWndProc(const WXFARPROC proc) { m_oldWndProc = proc; }
inline wxValidator *wxWindow::GetValidator(void) const { return m_windowValidator; }
inline bool wxWindow::IsUserEnabled(void) const { return m_winEnabled; }
inline bool wxWindow::GetUseCtl3D(void) const { return m_useCtl3D; }
inline bool wxWindow::GetTransparentBackground(void) const { return m_backgroundTransparent; }
inline void wxWindow::SetReturnCode(int retCode) { m_returnCode = retCode; }
inline int wxWindow::GetReturnCode(void) { return m_returnCode; }
inline bool wxWindow::IsBeingDeleted(void) { return m_isBeingDeleted; }

// Window specific (so far)
wxWindow* WXDLLEXPORT wxGetActiveWindow(void);

// Allows iteration through damaged rectangles in OnPaint
class WXDLLEXPORT wxUpdateIterator
{
  int rects;						// How many rects in Update region
  int current;					        // Current rectangle index
  void *rp;						// current rectangle
#ifdef	__WIN32__
  WXRGNDATA *rlist;					// Storage for regiondata
#endif

 public:
  wxUpdateIterator(wxWindow* wnd);
  ~wxUpdateIterator(void);

  operator int (void);
  wxUpdateIterator* operator ++(int);
  void GetRect(wxRectangle *rect);
  int GetX();
  int GetY();
  int GetW();
  int GetH();
};

WXDLLEXPORT_DATA(extern wxList) wxTopLevelWindows;

int WXDLLEXPORT wxCharCodeMSWToWX(int keySym);
int WXDLLEXPORT wxCharCodeWXToMSW(int id, bool *IsVirtual);

// Allocates control ids
int WXDLLEXPORT NewControlId(void);

#endif
    // __WINDOWH__
