/////////////////////////////////////////////////////////////////////////////
// Name:        window.h
// Purpose:
// Author:      Robert Roebling
// Id:          $Id$
// Copyright:   (c) 1998 Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#ifndef __GTKWINDOWH__
#define __GTKWINDOWH__

#ifdef __GNUG__
#pragma interface
#endif

#include "wx/defs.h"
#include "wx/object.h"
#include "wx/list.h"
#include "wx/event.h"
#include "wx/validate.h"
#include "wx/cursor.h"
#include "wx/font.h"
#include "wx/region.h"
#include "wx/accel.h"

//-----------------------------------------------------------------------------
// global data
//-----------------------------------------------------------------------------

extern const char *wxFrameNameStr;
extern wxList wxTopLevelWindows;

//-----------------------------------------------------------------------------
// global function
//-----------------------------------------------------------------------------

wxWindow* wxGetActiveWindow();

//-----------------------------------------------------------------------------
// classes
//-----------------------------------------------------------------------------

class wxLayoutConstraints;
class wxSizer;
class wxDC;
class wxClientData;
class wxVoidClientData;
class wxWindow;
#if wxUSE_WX_RESOURCES
class wxResourceTable;
class wxItemResource;
#endif
#if wxUSE_DRAG_AND_DROP
class wxDropTarget;
#endif

//-----------------------------------------------------------------------------
// callback definition for inserting a window (internal)
//-----------------------------------------------------------------------------

typedef void (*wxInsertChildFunction)( wxWindow*, wxWindow* );

//-----------------------------------------------------------------------------
// global data
//-----------------------------------------------------------------------------

extern const char *wxPanelNameStr;
extern const wxSize wxDefaultSize;
extern const wxPoint wxDefaultPosition;

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

//-----------------------------------------------------------------------------
// (debug)
//-----------------------------------------------------------------------------

#ifdef __WXDEBUG__

void debug_focus_in( GtkWidget* widget, const char* name, const char* window );

#endif

//-----------------------------------------------------------------------------
// wxWindow
//-----------------------------------------------------------------------------

class wxWindow: public wxEvtHandler
{
  DECLARE_DYNAMIC_CLASS(wxWindow)

public:
  wxWindow();
  wxWindow(wxWindow *parent, wxWindowID id,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize,
           long style = 0,
           const wxString& name = wxPanelNameStr);
  bool Create(wxWindow *parent, wxWindowID id,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize,
           long style = 0,
           const wxString& name = wxPanelNameStr);
  virtual ~wxWindow();

#if wxUSE_WX_RESOURCES
  virtual bool LoadFromResource( wxWindow *parent, const wxString& resourceName,
                                 const wxResourceTable *table = (const wxResourceTable *) NULL);
  virtual wxControl *CreateItem(const wxItemResource* childResource, const wxItemResource* parentResource,
                                 const wxResourceTable *table = (const wxResourceTable *) NULL);
#endif

  bool Close( bool force = FALSE );
  virtual bool Destroy();
  virtual bool DestroyChildren();

  virtual void PrepareDC( wxDC &dc );

  virtual void SetSize( int x, int y, int width, int height,
    int sizeFlags = wxSIZE_AUTO );
  virtual void SetSize( int width, int height );
  
  virtual void Move( int x, int y );
  
  virtual void GetSize( int *width, int *height ) const;
  wxSize GetSize() const { int w, h; GetSize(& w, & h); return wxSize(w, h); }
  
  virtual void SetClientSize( int const width, int const height );
  
  virtual void GetClientSize( int *width, int *height ) const;
  wxSize GetClientSize() const { int w, h; GetClientSize(& w, & h); return wxSize(w, h); }
  
  virtual void GetPosition( int *x, int *y ) const;
  wxPoint GetPosition() const { int w, h; GetPosition(& w, & h); return wxPoint(w, h); }
  
  wxRect GetRect() const
    { int x, y, w, h; GetPosition(& x, & y); GetSize(& w, & h); return wxRect(x, y, w, h); }
    
  virtual void Centre( int direction = wxHORIZONTAL );
  inline void Center(int direction = wxHORIZONTAL) { Centre(direction); }
  virtual void Fit();

  virtual void SetSizeHints( int minW, int minH, int maxW = -1, int maxH = -1, int incW = -1, int incH = -1 );

  // Dialog units translations. Implemented in wincmn.cpp.
  wxPoint ConvertPixelsToDialog(const wxPoint& pt) ;
  wxPoint ConvertDialogToPixels(const wxPoint& pt) ;
  inline wxSize ConvertPixelsToDialog(const wxSize& sz)
  { wxPoint pt(ConvertPixelsToDialog(wxPoint(sz.x, sz.y))); return wxSize(pt.x, pt.y); }
  inline wxSize ConvertDialogToPixels(const wxSize& sz)
  { wxPoint pt(ConvertDialogToPixels(wxPoint(sz.x, sz.y))); return wxSize(pt.x, pt.y); }

  void OnSize( wxSizeEvent &event );

  virtual bool Show( bool show );
  virtual void Enable( bool enable );
  virtual void MakeModal( bool modal );
  virtual bool IsEnabled() const { return m_isEnabled; }
  inline bool Enabled() const { return IsEnabled(); }
  virtual bool OnClose();

  virtual void SetFocus();
  static wxWindow *FindFocus();
  
  virtual void AddChild( wxWindow *child );
  wxList& GetChildren() { return m_children; }

  virtual void RemoveChild( wxWindow *child );
  void SetReturnCode( int retCode );
  int GetReturnCode();
  wxWindow *GetParent() const
    { return m_parent; }
  wxWindow *GetGrandParent() const
    { return (m_parent ? m_parent->m_parent : (wxWindow*)NULL); }
  void SetParent( wxWindow *p )
    { m_parent = p; }
  virtual wxWindow *ReParent( wxWindow *newParent );

  wxEvtHandler *GetEventHandler() const;
  void SetEventHandler( wxEvtHandler *handler );
  void PushEventHandler( wxEvtHandler *handler );
  wxEvtHandler *PopEventHandler( bool deleteHandler = FALSE );

  virtual wxValidator *GetValidator();
  virtual void SetValidator( const wxValidator &validator );

  virtual void SetClientObject( wxClientData *data );
  virtual wxClientData *GetClientObject();

  virtual void SetClientData( void *data );
  virtual void *GetClientData();

  virtual void SetAcceleratorTable( const wxAcceleratorTable& accel );
  virtual wxAcceleratorTable *GetAcceleratorTable() { return &m_acceleratorTable; }

  bool IsBeingDeleted();

  void SetId( wxWindowID id );
  wxWindowID GetId() const;

  void SetCursor( const wxCursor &cursor );

  void WarpPointer(int x, int y);
  
  virtual void Refresh( bool eraseBackground = TRUE, const wxRect *rect = (const wxRect *) NULL );
  virtual void Clear();

  virtual wxRegion GetUpdateRegion() const;
  virtual bool IsExposed(int x, int y) const;
  virtual bool IsExposed(int x, int y, int w, int h) const;
  virtual bool IsExposed(const wxPoint& pt) const;
  virtual bool IsExposed(const wxRect& rect) const;

  virtual wxColour GetBackgroundColour() const;
  virtual void SetBackgroundColour( const wxColour &colour );
  virtual wxColour GetForegroundColour() const;
  virtual void SetForegroundColour( const wxColour &colour );

  virtual int GetCharHeight() const;
  virtual int GetCharWidth() const;
  virtual void GetTextExtent( const wxString& string, int *x, int *y,
                             int *descent = (int *) NULL,
                             int *externalLeading = (int *) NULL,
                             const wxFont *theFont = (const wxFont *) NULL, bool use16 = FALSE) const;

  virtual void SetFont( const wxFont &font );
  virtual wxFont& GetFont() { return m_font; }

    // For backward compatibility
  inline virtual void SetButtonFont(const wxFont& font) { SetFont(font); }
  inline virtual void SetLabelFont(const wxFont& font) { SetFont(font); }
  inline virtual wxFont& GetLabelFont() { return GetFont(); };
  inline virtual wxFont& GetButtonFont() { return GetFont(); };

  virtual void SetWindowStyleFlag( long flag );
  virtual long GetWindowStyleFlag() const;

  virtual void CaptureMouse();
  virtual void ReleaseMouse();

  virtual void SetTitle( const wxString &title );
  virtual wxString GetTitle() const;
  virtual void SetName( const wxString &name );
  virtual wxString GetName() const;
  virtual wxString GetLabel() const;

  void OnSysColourChanged( wxSysColourChangedEvent &WXUNUSED(event) ) {};
  void OnKeyDown( wxKeyEvent &event );

  virtual bool IsShown() const;

  virtual void Raise();
  virtual void Lower();

  virtual bool IsRetained();
  virtual wxWindow *FindWindow( long id );
  virtual wxWindow *FindWindow( const wxString& name );

  void AllowDoubleClick( bool WXUNUSED(allow) ) {};
  void SetDoubleClick( bool WXUNUSED(allow) ) {};

  virtual void ClientToScreen( int *x, int *y );
  virtual void ScreenToClient( int *x, int *y );

  virtual bool Validate();
  virtual bool TransferDataToWindow();
  virtual bool TransferDataFromWindow();
  void OnInitDialog( wxInitDialogEvent &event );
  virtual void InitDialog();

  virtual bool PopupMenu( wxMenu *menu, int x, int y );

#if wxUSE_DRAG_AND_DROP
  virtual void SetDropTarget( wxDropTarget *dropTarget );
  virtual wxDropTarget *GetDropTarget() const;
#endif

  virtual void SetScrollbar( int orient, int pos, int thumbVisible,
    int range, bool refresh = TRUE );
  virtual void SetScrollPos( int orient, int pos, bool refresh = TRUE );
  virtual int GetScrollPos( int orient ) const;
  virtual int GetScrollThumb( int orient ) const;
  virtual int GetScrollRange( int orient ) const;
  virtual void ScrollWindow( int dx, int dy, const wxRect* rect = (wxRect *) NULL );

  virtual bool AcceptsFocus() const;

  void UpdateWindowUI();

  // implementation

  void PreCreation( wxWindow *parent, wxWindowID id, const wxPoint &pos,
                    const wxSize &size, long style, const wxString &name );
  void PostCreation();
  
  virtual GtkWidget* GetConnectWidget();
  virtual bool IsOwnGtkWindow( GdkWindow *window );
  void ConnectWidget( GtkWidget *widget );

  bool HasVMT();

  virtual wxPoint GetClientAreaOrigin() const;
  virtual void AdjustForParentClientOrigin( int& x, int& y, int sizeFlags );

  GtkStyle *GetWidgetStyle();
  void SetWidgetStyle();
  virtual void ApplyWidgetStyle();
  
  virtual void OnInternalIdle();

  wxWindow            *m_parent;
  wxList               m_children;
  int                  m_x,m_y;
  int                  m_width,m_height;
  int                  m_minWidth,m_minHeight;
  int                  m_maxWidth,m_maxHeight;
  int                  m_retCode;
  wxEvtHandler        *m_eventHandler;
  wxValidator         *m_windowValidator;
#if wxUSE_DRAG_AND_DROP
  wxDropTarget        *m_dropTarget;
#endif
  wxWindowID           m_windowId;
  wxCursor            *m_cursor;
  wxFont               m_font;
  wxColour             m_backgroundColour;
  wxColour             m_foregroundColour;
  wxRegion             m_updateRegion;
  long                 m_windowStyle;
  bool                 m_isShown;
  bool                 m_isEnabled;
  wxString             m_windowName;
  wxAcceleratorTable   m_acceleratorTable;
  wxClientData        *m_clientObject;
  void                *m_clientData;

  GtkWidget           *m_widget;
  GtkWidget           *m_wxwindow;
  GtkAdjustment       *m_hAdjust,*m_vAdjust;
  float                m_oldHorizontalPos;
  float                m_oldVerticalPos;
  bool                 m_needParent;
  bool                 m_hasScrolling;
  bool                 m_isScrolling;
  bool                 m_hasVMT;
  bool                 m_sizeSet;
  bool                 m_resizing;
  GdkGC               *m_scrollGC;
  GtkStyle            *m_widgetStyle;
  bool                 m_isStaticBox;
  bool                 m_acceptsFocus;

  wxInsertChildFunction  m_insertCallback;

public:

  wxLayoutConstraints *m_constraints;
  wxList              *m_constraintsInvolvedIn;
  wxSizer             *m_windowSizer;
  wxWindow            *m_sizerParent;
  bool                 m_autoLayout;

  wxLayoutConstraints *GetConstraints() const;
  void SetConstraints( wxLayoutConstraints *constraints );
  void SetAutoLayout( bool autoLayout );
  bool GetAutoLayout() const;
  bool Layout();
  void SetSizer( wxSizer *sizer );
  wxSizer *GetSizer() const;
  void SetSizerParent( wxWindow *win );
  wxWindow *GetSizerParent() const;
  void UnsetConstraints(wxLayoutConstraints *c);
  inline wxList *GetConstraintsInvolvedIn() const ;
  void AddConstraintReference(wxWindow *otherWin);
  void RemoveConstraintReference(wxWindow *otherWin);
  void DeleteRelatedConstraints();
  virtual void ResetConstraints();
  virtual void SetConstraintSizes(bool recurse = TRUE);
  virtual bool LayoutPhase1(int *noChanges);
  virtual bool LayoutPhase2(int *noChanges);
  virtual bool DoPhase(int);
  virtual void TransformSizerToActual(int *x, int *y) const ;
  virtual void SizerSetSize(int x, int y, int w, int h);
  virtual void SizerMove(int x, int y);
  virtual void SetSizeConstraint(int x, int y, int w, int h);
  virtual void MoveConstraint(int x, int y);
  virtual void GetSizeConstraint(int *w, int *h) const ;
  virtual void GetClientSizeConstraint(int *w, int *h) const ;
  virtual void GetPositionConstraint(int *x, int *y) const ;

private:
  DECLARE_EVENT_TABLE()
};

#endif // __GTKWINDOWH__
