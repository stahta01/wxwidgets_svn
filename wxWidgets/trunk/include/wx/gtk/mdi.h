/////////////////////////////////////////////////////////////////////////////
// Name:        mdi.h
// Purpose:
// Author:      Robert Roebling
// Created:     01/02/97
// Id:
// Copyright:   (c) 1998 Robert Roebling, Julian Smart and Markus Holzem
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#ifndef __MDIH__
#define __MDIH__

#ifdef __GNUG__
#pragma interface
#endif

#include "wx/defs.h"
#include "wx/object.h"
#include "wx/list.h"
#include "wx/control.h"
#include "wx/panel.h"
#include "wx/frame.h"
#include "wx/toolbar.h"

//-----------------------------------------------------------------------------
// classes
//-----------------------------------------------------------------------------

class wxMDIParentFrame;
class wxMDIClientWindow;
class wxMDIChildFrame;

//-----------------------------------------------------------------------------
// global data
//-----------------------------------------------------------------------------

extern const char* wxFrameNameStr;
extern const char* wxStatusLineNameStr;

//-----------------------------------------------------------------------------
// wxMDIParentFrame
//-----------------------------------------------------------------------------

class wxMDIParentFrame: public wxFrame
{
  DECLARE_DYNAMIC_CLASS(wxMDIParentFrame)

  friend class wxMDIChildFrame;
  
  public:

    wxMDIParentFrame(void);
    wxMDIParentFrame( wxWindow *parent,
      wxWindowID id, const wxString& title,
      const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
      long style = wxDEFAULT_FRAME_STYLE | wxVSCROLL | wxHSCROLL,
      const wxString& name = wxFrameNameStr );
  ~wxMDIParentFrame(void);
   bool Create( wxWindow *parent,
      wxWindowID id, const wxString& title,
      const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
      long style = wxDEFAULT_FRAME_STYLE | wxVSCROLL | wxHSCROLL,
      const wxString& name = wxFrameNameStr );


    void GetClientSize(int *width, int *height) const;
    wxMDIChildFrame *GetActiveChild(void) const;
    
    wxMDIClientWindow *GetClientWindow(void) const; 
    virtual wxMDIClientWindow *OnCreateClient(void);
  
    virtual void Cascade(void) {};
    virtual void Tile(void) {};
    virtual void ArrangeIcons(void) {};
    virtual void ActivateNext(void);
    virtual void ActivatePrevious(void);

    void OnActivate( wxActivateEvent& event );
    void OnSysColourChanged( wxSysColourChangedEvent& event );
    
 //private: 
 
    wxMDIChildFrame                *m_currentChild;
    
    void SetMDIMenuBar( wxMenuBar *menu_bar );
    virtual void GtkOnSize( int x, int y, int width, int height );
    
 private:
 
    wxMDIClientWindow              *m_clientWindow;
    bool                            m_parentFrameActive;
    wxMenuBar                      *m_mdiMenuBar;

  DECLARE_EVENT_TABLE()    
};

//-----------------------------------------------------------------------------
// wxMDIChildFrame
//-----------------------------------------------------------------------------

class wxMDIChildFrame: public wxFrame
{
  DECLARE_DYNAMIC_CLASS(wxMDIChildFrame)
  
  public:

    wxMDIChildFrame(void);
    wxMDIChildFrame( wxMDIParentFrame *parent,
      wxWindowID id, const wxString& title,
      const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
      long style = wxDEFAULT_FRAME_STYLE, const wxString& name = wxFrameNameStr );
    ~wxMDIChildFrame(void);
    bool Create( wxMDIParentFrame *parent,
      wxWindowID id, const wxString& title,
      const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
      long style = wxDEFAULT_FRAME_STYLE, const wxString& name = wxFrameNameStr );
      
  virtual void SetMenuBar( wxMenuBar *menu_bar );
  virtual wxMenuBar *GetMenuBar();

  virtual void GetClientSize( int *width, int *height ) const;
  virtual void AddChild( wxWindow *child );

  virtual void Activate(void);
    
    // no status bars
  virtual wxStatusBar* CreateStatusBar( int WXUNUSED(number), long WXUNUSED(style), 
    wxWindowID WXUNUSED(id), const wxString& WXUNUSED(name) ) {return (wxStatusBar*)NULL; }
  virtual wxStatusBar *GetStatusBar() { return (wxStatusBar*)NULL; }
  virtual void SetStatusText( const wxString &WXUNUSED(text), int WXUNUSED(number) ) {}
  virtual void SetStatusWidths( int WXUNUSED(n), int *WXUNUSED(width) ) {}

    // no size hints
  virtual void SetSizeHints( int WXUNUSED(minW), int WXUNUSED(minH), int WXUNUSED(maxW), 
    int WXUNUSED(maxH), int WXUNUSED(incW) ) {}
  
    // no toolbar bars
  virtual wxToolBar* CreateToolBar( long WXUNUSED(style), wxWindowID WXUNUSED(id), 
    const wxString& WXUNUSED(name) ) { return (wxToolBar*)NULL; }
  virtual wxToolBar *GetToolBar() { return (wxToolBar*)NULL; }
  
    // no icon
  void SetIcon( const wxIcon &icon ) { m_icon = icon; }
    
    // no title 
  void SetTitle( const wxString &title ) { m_title = title; }
  wxString GetTitle() const { return m_title; }
  
    // no maximize etc
  virtual void Maximize(void) {}
  virtual void Restore(void) {}
    
    void OnActivate( wxActivateEvent &event );
    
  public:
  
    wxMenuBar         *m_menuBar;
    
//  private:
  
    GtkNotebookPage   *m_page;
    
  DECLARE_EVENT_TABLE()    
};

//-----------------------------------------------------------------------------
// wxMDIClientWindow
//-----------------------------------------------------------------------------

class wxMDIClientWindow: public wxWindow
{
  DECLARE_DYNAMIC_CLASS(wxMDIClientWindow)
  
  public:
  
    wxMDIClientWindow(void);
    wxMDIClientWindow( wxMDIParentFrame *parent, long style = 0 );
    ~wxMDIClientWindow(void);
    virtual bool CreateClient( wxMDIParentFrame *parent, long style = wxVSCROLL | wxHSCROLL );
    void AddChild( wxWindow *child );
};

#endif // __MDIH__

