/////////////////////////////////////////////////////////////////////////////
// Name:        mdi.h
// Purpose:     MDI (Multiple Document Interface) classes.
// Author:      Julian Smart
// Modified by:
// Created:     17/09/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_MDI_H_
#define _WX_MDI_H_

#ifdef __GNUG__
#pragma interface "mdi.h"
#endif

#include "wx/frame.h"

WXDLLEXPORT_DATA(extern const char*) wxFrameNameStr;
WXDLLEXPORT_DATA(extern const char*) wxStatusLineNameStr;

class WXDLLEXPORT wxMDIClientWindow;
class WXDLLEXPORT wxMDIChildFrame;

#if wxUSE_MDI_WIDGETS
class XsMDICanvas;
class wxXsMDIWindow;
#endif

class WXDLLEXPORT wxMDIParentFrame: public wxFrame
{
DECLARE_DYNAMIC_CLASS(wxMDIParentFrame)

  friend class WXDLLEXPORT wxMDIChildFrame;
public:

  wxMDIParentFrame();
  inline wxMDIParentFrame(wxWindow *parent,
           wxWindowID id,
           const wxString& title,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize,
           long style = wxDEFAULT_FRAME_STYLE | wxVSCROLL | wxHSCROLL,  // Scrolling refers to client window
           const wxString& name = wxFrameNameStr)
  {
      Create(parent, id, title, pos, size, style, name);
  }

  ~wxMDIParentFrame();

  bool Create(wxWindow *parent,
           wxWindowID id,
           const wxString& title,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize,
           long style = wxDEFAULT_FRAME_STYLE | wxVSCROLL | wxHSCROLL,
           const wxString& name = wxFrameNameStr);

  void OnSize(wxSizeEvent& event);
  void OnActivate(wxActivateEvent& event);
  void OnSysColourChanged(wxSysColourChangedEvent& event);

  void SetMenuBar(wxMenuBar *menu_bar);

  // Gets the size available for subwindows after menu size, toolbar size
  // and status bar size have been subtracted. If you want to manage your own
  // toolbar(s), don't call SetToolBar.
  void GetClientSize(int *width, int *height) const;

  // Get the active MDI child window
  wxMDIChildFrame *GetActiveChild() const ;

  // Get the client window
  inline wxMDIClientWindow *GetClientWindow() const { return m_clientWindow; };

  // Create the client window class (don't Create the window,
  // just return a new class)
  virtual wxMDIClientWindow *OnCreateClient() ;

  // MDI operations
  virtual void Cascade();
  virtual void Tile();
  virtual void ArrangeIcons();
  virtual void ActivateNext();
  virtual void ActivatePrevious();

// Implementation
  inline void SetActiveChild(wxMDIChildFrame* child) { m_activeChild = child; }

protected:

  wxMDIClientWindow*    m_clientWindow;
  wxMDIChildFrame*      m_activeChild;

DECLARE_EVENT_TABLE()
};

class WXDLLEXPORT wxMDIChildFrame: public wxFrame
{
DECLARE_DYNAMIC_CLASS(wxMDIChildFrame)
public:

  wxMDIChildFrame();
  inline wxMDIChildFrame(wxMDIParentFrame *parent,
           wxWindowID id,
           const wxString& title,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize,
           long style = wxDEFAULT_FRAME_STYLE,
           const wxString& name = wxFrameNameStr)
  {
      Create(parent, id, title, pos, size, style, name);
  }

  ~wxMDIChildFrame();

  bool Create(wxMDIParentFrame *parent,
           wxWindowID id,
           const wxString& title,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize,
           long style = wxDEFAULT_FRAME_STYLE,
           const wxString& name = wxFrameNameStr);

  // Set menu bar
  void SetMenuBar(wxMenuBar *menu_bar);
  void SetTitle(const wxString& title);
  void SetClientSize(int width, int height);
  void GetClientSize(int *width, int *height) const;
  void SetSize(int x, int y, int width, int height, int sizeFlags = wxSIZE_AUTO);
  void GetSize(int *width, int *height) const;
  void GetPosition(int *x, int *y) const ;

  // Set icon
  virtual void SetIcon(const wxIcon& icon);

  // MDI operations
  virtual void Maximize();
  inline void Minimize() { Iconize(TRUE); };
  virtual void Iconize(bool iconize);
  virtual void Restore();
  virtual void Activate();
  virtual bool IsIconized() const ;

  bool Show(bool show);
  void BuildClientArea(WXWidget parent);
  inline WXWidget GetTopWidget() const { return m_mainWidget; };
#if wxUSE_MDI_WIDGETS
  inline wxXsMDIWindow *GetMDIWindow() const { return m_mdiWindow; };
#endif
  virtual void OnRaise();
  virtual void OnLower();

protected:
  wxXsMDIWindow*    m_mdiWindow ;
};

/* The client window is a child of the parent MDI frame, and itself
 * contains the child MDI frames.
 * However, you create the MDI children as children of the MDI parent:
 * only in the implementation does the client window become the parent
 * of the children. Phew! So the children are sort of 'adopted'...
 */

class WXDLLEXPORT wxMDIClientWindow: public wxWindow
{
  DECLARE_DYNAMIC_CLASS(wxMDIClientWindow)
 public:

  wxMDIClientWindow() ;
  inline wxMDIClientWindow(wxMDIParentFrame *parent, long style = 0)
  {
      CreateClient(parent, style);
  }

  ~wxMDIClientWindow();

   void SetSize(int x, int y, int width, int height, int sizeFlags = wxSIZE_AUTO);
   void SetClientSize(int width, int height);
   void GetClientSize(int *width, int *height) const;

   void GetSize(int *width, int *height) const ;
   void GetPosition(int *x, int *y) const ;


  // Note: this is virtual, to allow overridden behaviour.
  virtual bool CreateClient(wxMDIParentFrame *parent, long style = wxVSCROLL | wxHSCROLL);

  // Explicitly call default scroll behaviour
  void OnScroll(wxScrollEvent& event);

#if wxUSE_MDI_WIDGETS
  inline XsMDICanvas* GetMDICanvas() const { return m_mdiCanvas; }
  WXWidget GetTopWidget() const { return m_topWidget; }
#endif

protected:

#if wxUSE_MDI_WIDGETS
  XsMDICanvas*   m_mdiCanvas;
  WXWidget       m_topWidget;
#endif

DECLARE_EVENT_TABLE()
};

#endif
    // _WX_MDI_H_
