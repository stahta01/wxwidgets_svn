/////////////////////////////////////////////////////////////////////////////
// Name:        window.h
// Purpose:     wxWindow class
// Author:      AUTHOR
// Modified by:
// Created:     ??/??/98
// RCS-ID:      $Id$
// Copyright:   (c) AUTHOR
// Licence:   	wxWindows licence
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
#include "wx/accel.h"

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

#if wxUSE_DRAG_AND_DROP
class WXDLLEXPORT wxDropTarget;
#endif

#if wxUSE_WX_RESOURCES
class WXDLLEXPORT wxResourceTable;
class WXDLLEXPORT wxItemResource;
#endif

WXDLLEXPORT_DATA(extern const char*) wxPanelNameStr;

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

class WXDLLEXPORT wxWindow: public wxEvtHandler
{
  DECLARE_ABSTRACT_CLASS(wxWindow)

  friend class wxDC;
  friend class wxPaintDC;

public:
  wxWindow();
  inline wxWindow(wxWindow *parent, wxWindowID id,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize,
           long style = 0,
           const wxString& name = wxPanelNameStr)
  {
  	  m_children = new wxList;
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
  inline virtual wxCursor *GetCursor() const { return (wxCursor *)& m_windowCursor; };

  // Get the window with the focus
  static wxWindow *FindFocus();

  // Get character size
  virtual int GetCharHeight() const;
  virtual int GetCharWidth() const;

  // Get overall window size
  virtual void GetSize(int *width, int *height) const;
  wxSize GetSize() const { int w, h; GetSize(& w, & h); return wxSize(w, h); }

  // Get window position, relative to parent (or screen if no parent)
  virtual void GetPosition(int *x, int *y) const;
  wxPoint GetPosition() const
    { int x, y; GetPosition(&x, &y); return wxPoint(x, y); }

  // Get size and position
  wxRect GetRect() const
    { int x, y, w, h; GetPosition(& x, & y); GetSize(& w, & h); return wxRect(x, y, w, h); }

  // Get client (application-useable) size
  virtual void GetClientSize(int *width, int *height) const;
  wxSize GetClientSize() const { int w, h; GetClientSize(& w, & h); return wxSize(w, h); }

  // Set overall size and position
  virtual void SetSize(int x, int y, int width, int height, int sizeFlags = wxSIZE_AUTO);
  void SetSize(const wxRect& rect, int sizeFlags = wxSIZE_AUTO)
    { SetSize(rect.x, rect.y, rect.width, rect.height, sizeFlags); }

    // set size only
  void SetSize(int width, int height)
    { SetSize(-1, -1, width, height, wxSIZE_USE_EXISTING); }
  void SetSize(const wxSize& size)
   { SetSize(-1, -1, size.x, size.y, wxSIZE_USE_EXISTING); }

    // set position only
  virtual void Move(int x, int y) { SetSize(x, y, -1, -1, wxSIZE_USE_EXISTING); }
  void Move(const wxPoint& pt) { SetSize(pt.x, pt.y, -1, -1, wxSIZE_USE_EXISTING); }

  // Set client size
  virtual void SetClientSize(int width, int height);
  void SetClientSize(const wxSize& sz) { SetClientSize(sz.x, sz.y); }

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

  virtual void SetAcceleratorTable(const wxAcceleratorTable& accel);
  inline virtual wxAcceleratorTable& GetAcceleratorTable() const { return (wxAcceleratorTable&) m_acceleratorTable; }

  // Make the window modal (all other windows unresponsive)
  virtual void MakeModal(bool modal);

  // Get the private handle (platform-dependent)
  inline void *GetHandle() const;

  // Set/get the window's relatives
  inline wxWindow *GetParent() const;
  inline void SetParent(wxWindow *p) ;
  inline wxWindow *GetGrandParent() const;
  inline wxList& GetChildren() const;

  // Set/get the window's font
  virtual void SetFont(const wxFont& f);
  inline virtual wxFont& GetFont() const;

  // Set/get the window's validator
  void SetValidator(const wxValidator& validator);
  inline wxValidator *GetValidator() const;

  // Set/get the window's style
  inline void SetWindowStyleFlag(long flag);
  inline long GetWindowStyleFlag() const;

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

  // Get the default button, if there is one
  inline virtual wxButton *GetDefaultItem() const;
  inline virtual void SetDefaultItem(wxButton *but);

  // Override to define new behaviour for default action (e.g. double clicking
  // on a listbox)
  virtual void OnDefaultAction(wxControl *initiatingItem);

  // Resource loading
#if wxUSE_WX_RESOURCES
  virtual bool LoadFromResource(wxWindow *parent, const wxString& resourceName, const wxResourceTable *table = NULL);
  virtual wxControl *CreateItem(const wxItemResource* childResource, const wxItemResource* parentResource,
                                 const wxResourceTable *table = (const wxResourceTable *) NULL);
#endif

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
  void OnChar(wxKeyEvent& event);
  void OnPaint(wxPaintEvent& event);
  void OnIdle(wxIdleEvent& event);

  // Does this window want to accept keyboard focus?
  virtual bool AcceptsFocus() const;

  virtual void PrepareDC( wxDC &dc ) {};


public:
  ////////////////////////////////////////////////////////////////////////
  //// IMPLEMENTATION
  
  // For implementation purposes - sometimes decorations make the client area
  // smaller
  virtual wxPoint GetClientAreaOrigin() const;

  // Makes an adjustment to the window position (for example, a frame that has
  // a toolbar that it manages itself).
  virtual void AdjustForParentClientOrigin(int& x, int& y, int sizeFlags);

  // Executes the default message
  virtual long Default();

/* TODO: you may need something like this
  // Determine whether 3D effects are wanted
  virtual WXDWORD Determine3DEffects(WXDWORD defaultBorderStyle, bool *want3D);
*/

  virtual void AddChild(wxWindow *child);         // Adds reference to the child object
  virtual void RemoveChild(wxWindow *child);   // Removes reference to child
                                       // (but doesn't delete the child object)
  virtual void DestroyChildren();  // Removes and destroys all children

  inline bool IsBeingDeleted() const { return FALSE; } // TODO: Should probably eliminate this

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

  // Generates a new id for controls
  static int NewControlId();

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
  wxCursor              m_windowCursor;                        // Window's cursor
  wxString              m_windowName;                            // Window name

  wxButton *            m_defaultItem;

  wxColour              m_backgroundColour ;
  wxColour              m_foregroundColour ;
  wxAcceleratorTable    m_acceleratorTable;

#if wxUSE_DRAG_AND_DROP
  wxDropTarget         *m_pDropTarget;    // the current drop target or NULL
#endif  //USE_DRAG_AND_DROP

public:
  wxRegion              m_updateRegion;
  wxList *              m_children;                           // Window's children
  int                   m_returnCode;

DECLARE_EVENT_TABLE()
};

////////////////////////////////////////////////////////////////////////
//// INLINES

inline void *wxWindow::GetHandle() const { return (void *)NULL; }
inline int wxWindow::GetId() const { return m_windowId; }
inline void wxWindow::SetId(int id) { m_windowId = id; }
inline wxWindow *wxWindow::GetParent() const { return m_windowParent; }
inline void wxWindow::SetParent(wxWindow *p) { m_windowParent = p; }
inline wxWindow *wxWindow::GetGrandParent() const { return (m_windowParent ? m_windowParent->m_windowParent : (wxWindow*) NULL); }
inline wxList& wxWindow::GetChildren() const { return (wxList&) * m_children; }
inline wxFont& wxWindow::GetFont() const { return (wxFont&) m_windowFont; }
inline wxString wxWindow::GetName() const { return m_windowName; }
inline void wxWindow::SetName(const wxString& name) { m_windowName = name; }
inline long wxWindow::GetWindowStyleFlag() const { return m_windowStyle; }
inline void wxWindow::SetWindowStyleFlag(long flag) { m_windowStyle = flag; }
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

inline wxList *wxWindow::GetConstraintsInvolvedIn() const { return m_constraintsInvolvedIn; }
inline wxSizer *wxWindow::GetSizer() const { return m_windowSizer; }
inline wxWindow *wxWindow::GetSizerParent() const { return m_sizerParent; }
inline void wxWindow::SetSizerParent(wxWindow *win) { m_sizerParent = win; }
inline wxValidator *wxWindow::GetValidator() const { return m_windowValidator; }
inline void wxWindow::SetReturnCode(int retCode) { m_returnCode = retCode; }
inline int wxWindow::GetReturnCode() { return m_returnCode; }

// Get the active window.
wxWindow* WXDLLEXPORT wxGetActiveWindow();

WXDLLEXPORT_DATA(extern wxList) wxTopLevelWindows;

#endif
    // _WX_WINDOW_H_
