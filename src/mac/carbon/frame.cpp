/////////////////////////////////////////////////////////////////////////////
// Name:        frame.cpp
// Purpose:     wxFrame
// Author:      AUTHOR
// Modified by:
// Created:     ??/??/98
// RCS-ID:      $Id$
// Copyright:   (c) AUTHOR
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "frame.h"
#endif

#include "wx/frame.h"
#include "wx/statusbr.h"
#include "wx/toolbar.h"
#include "wx/menuitem.h"
#include "wx/menu.h"
#include "wx/dcclient.h"
#include "wx/dialog.h"
#include "wx/settings.h"
#include "wx/app.h"

#include <wx/mac/uma.h>

extern wxList wxModelessWindows;
extern wxList wxPendingDelete;

#if !USE_SHARED_LIBRARY
BEGIN_EVENT_TABLE(wxFrame, wxWindow)
  EVT_SIZE(wxFrame::OnSize)
  EVT_ACTIVATE(wxFrame::OnActivate)
  EVT_MENU_HIGHLIGHT_ALL(wxFrame::OnMenuHighlight)
  EVT_SYS_COLOUR_CHANGED(wxFrame::OnSysColourChanged)
  EVT_IDLE(wxFrame::OnIdle)
  EVT_CLOSE(wxFrame::OnCloseWindow)
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC_CLASS(wxFrame, wxWindow)
#endif

#if wxUSE_NATIVE_STATUSBAR
bool wxFrame::m_useNativeStatusBar = TRUE;
#else
bool wxFrame::m_useNativeStatusBar = FALSE;
#endif

wxFrame::wxFrame()
{
#if wxUSE_TOOLBAR
  m_frameToolBar = NULL ;
#endif

	// in order to be able to give size events on show
  m_frameMenuBar = NULL;
  m_frameStatusBar = NULL;
  m_iconized = FALSE;
  m_isShown = FALSE;
}

bool wxFrame::Create(wxWindow *parent,
           wxWindowID id,
           const wxString& title,
           const wxPoint& pos,
           const wxSize& size,
           long style,
           const wxString& name)
{
  if (!parent)
    wxTopLevelWindows.Append(this);

  SetName(name);
  m_windowStyle = style;
  m_frameMenuBar = NULL;
  m_isShown = FALSE;

#if wxUSE_TOOLBAR
  m_frameToolBar = NULL ;
#endif
  m_frameStatusBar = NULL;

  SetBackgroundColour(wxSystemSettings::GetSystemColour(wxSYS_COLOUR_APPWORKSPACE));

  if ( id > -1 )
    m_windowId = id;
  else
    m_windowId = (int)NewControlId();

  if (parent) parent->AddChild(this);

  wxModelessWindows.Append(this);

  // create frame.

	Rect theBoundsRect;

  m_x = (int)pos.x;
  m_y = (int)pos.y;
  if ( m_y < 50 )
  	m_y = 50 ;
  if ( m_x < 20 )
  	m_x = 20 ;
  	
  m_width = size.x;
	if (m_width == -1) 
		m_width = 20;
  m_height = size.y;
	if (m_height == -1) 
		m_height = 20;

	m_macWindowData = new MacWindowData() ;

	::SetRect(&theBoundsRect, m_x, m_y, m_x + m_width, m_y + m_height);

	WindowClass wclass = kDocumentWindowClass ;
	WindowAttributes attr = kWindowNoAttributes ;
	
	if ( ( m_windowStyle & wxMINIMIZE_BOX ) || ( m_windowStyle & wxMAXIMIZE_BOX ) )
	{
		attr |= kWindowFullZoomAttribute ;
		attr |= kWindowResizableAttribute ;
	}
	if ( m_windowStyle & wxSTAY_ON_TOP )
	{
		wclass = kFloatingWindowClass ;
		
//			if ( m_windowStyle & wxCAPTION )
//				attr |= kHasPaletteTitlebarMask ;
	}
	else
	{
	}
	if ( m_windowStyle & wxSYSTEM_MENU )
	{
		attr |= kWindowCloseBoxAttribute ;
	}
	UMACreateNewWindow( wclass , attr , &theBoundsRect , &m_macWindowData->m_macWindow ) ;
	wxAssociateWinWithMacWindow( m_macWindowData->m_macWindow , this ) ;
	wxString label ;
	if( wxApp::s_macDefaultEncodingIsPC )
		label = wxMacMakeMacStringFromPC( title ) ;
	else
		label = title ;
	UMASetWTitleC( m_macWindowData->m_macWindow , label ) ;
	UMACreateRootControl( m_macWindowData->m_macWindow , &m_macWindowData->m_macRootControl ) ;
	m_macWindowData->m_macWindowBackgroundTheme = kThemeBrushDocumentWindowBackground ;
	m_macWindowData->m_macFocus = NULL ;
  return TRUE;
}

wxFrame::~wxFrame()
{
  wxTopLevelWindows.DeleteObject(this);

  if (m_frameStatusBar)
    delete m_frameStatusBar;
  if (m_frameMenuBar)
    delete m_frameMenuBar;

/* Check if it's the last top-level window */

  if (wxTheApp && (wxTopLevelWindows.Number() == 0))
  {
    wxTheApp->SetTopWindow(NULL);

    if (wxTheApp->GetExitOnFrameDelete())
    {
       wxTheApp->ExitMainLoop() ;
    }
  }

  wxModelessWindows.DeleteObject(this);
}


void wxFrame::Iconize(bool iconize)
{
    // TODO
}

// Equivalent to maximize/restore in Windows
void wxFrame::Maximize(bool maximize)
{
    // TODO
}

bool wxFrame::IsIconized() const
{
    // TODO
    return FALSE;
}

// Is the frame maximized?
bool wxFrame::IsMaximized(void) const
{
    // TODO
    return FALSE;
}

void wxFrame::SetIcon(const wxIcon& icon)
{
  m_icon = icon;
  // TODO
}

wxStatusBar *wxFrame::OnCreateStatusBar(int number, long style, wxWindowID id,
    const wxString& name)
{
    wxStatusBar *statusBar = NULL;

    statusBar = new wxStatusBar(this, id, wxPoint(0, 0), wxSize(100, 17),
        style, name);

    // Set the height according to the font and the border size
    // we shouldn't do this on the mac, because we have to fit the grow box
    /*
    wxClientDC dc(statusBar);
    dc.SetFont(statusBar->GetFont());

    long x, y;
    dc.GetTextExtent("X", &x, &y);

    int height = (int)( (y  * 1.1) + 2* statusBar->GetBorderY());

    statusBar->SetSize(-1, -1, 100, height);

		*/

    statusBar->SetFieldsCount(number);
    return statusBar;
}

wxStatusBar* wxFrame::CreateStatusBar(int number, long style, wxWindowID id,
    const wxString& name)
{
  // Calling CreateStatusBar twice is an error.
  wxCHECK_MSG( m_frameStatusBar == NULL, FALSE, 
               "recreating status bar in wxFrame" );

  m_frameStatusBar = OnCreateStatusBar(number, style, id,
    name);
  if ( m_frameStatusBar )
  {
    PositionStatusBar();
    return m_frameStatusBar;
  }
  else
    return NULL;
}

void wxFrame::SetStatusText(const wxString& text, int number)
{
  wxCHECK_RET( m_frameStatusBar != NULL, "no statusbar to set text for" );

  m_frameStatusBar->SetStatusText(text, number);
}

void wxFrame::SetStatusWidths(int n, const int widths_field[])
{
  wxCHECK_RET( m_frameStatusBar != NULL, "no statusbar to set widths for" );

  m_frameStatusBar->SetStatusWidths(n, widths_field);
  PositionStatusBar();
}

void wxFrame::PositionStatusBar()
{
  if (m_frameStatusBar )
  {
    int w, h;
    GetClientSize(&w, &h);
    int sw, sh;
    m_frameStatusBar->GetSize(&sw, &sh);

    // Since we wish the status bar to be directly under the client area,
    // we use the adjusted sizes without using wxSIZE_NO_ADJUSTMENTS.
    m_frameStatusBar->SetSize(0, h, w, sh);
   }
}

void wxFrame::SetMenuBar(wxMenuBar *menuBar)
{
    if (!menuBar)
    {
        m_frameMenuBar = NULL;
        return;
    }
  
    m_frameMenuBar = menuBar;
		// TODO : we move this into the app code
    m_frameMenuBar->MacInstallMenuBar() ;
}

void wxFrame::Fit()
{
  // Work out max. size
  wxNode *node = GetChildren().First();
  int max_width = 0;
  int max_height = 0;
  while (node)
  {
    // Find a child that's a subwindow, but not a dialog box.
    wxWindow *win = (wxWindow *)node->Data();

    if (!win->IsKindOf(CLASSINFO(wxFrame)) &&
         !win->IsKindOf(CLASSINFO(wxDialog)))
    {
      int width, height;
      int x, y;
      win->GetSize(&width, &height);
      win->GetPosition(&x, &y);

      if ((x + width) > max_width)
        max_width = x + width;
      if ((y + height) > max_height)
        max_height = y + height;
    }
    node = node->Next();
  }
  SetClientSize(max_width, max_height);
}

// Responds to colour changes, and passes event on to children.
void wxFrame::OnSysColourChanged(wxSysColourChangedEvent& event)
{
    SetBackgroundColour(wxSystemSettings::GetSystemColour(wxSYS_COLOUR_APPWORKSPACE));
    Refresh();

    if ( m_frameStatusBar )
    {
        wxSysColourChangedEvent event2;
        event2.SetEventObject( m_frameStatusBar );
        m_frameStatusBar->ProcessEvent(event2);
    }

    // Propagate the event to the non-top-level children
    wxWindow::OnSysColourChanged(event);
}

// Default resizing behaviour - if only ONE subwindow,
// resize to client rectangle size
void wxFrame::OnIdle(wxIdleEvent& WXUNUSED(event) )
{
    DoMenuUpdates();
}


// update all menus
void wxFrame::DoMenuUpdates()
{
    wxMenuBar* bar = GetMenuBar();

    if ( bar != NULL )
    {
        int nCount = bar->GetMenuCount();
        for (int n = 0; n < nCount; n++)
            DoMenuUpdates(bar->GetMenu(n), (wxWindow*) NULL);
    }
}

// update a menu and all submenus recursively
void wxFrame::DoMenuUpdates(wxMenu* menu, wxWindow* WXUNUSED(focusWin))
{
    wxEvtHandler* evtHandler = GetEventHandler();
    wxMenuItemList::Node* node = menu->GetMenuItems().GetFirst();
    while (node)
    {
        wxMenuItem* item = node->GetData();
        if ( !item->IsSeparator() )
        {
            wxWindowID id = item->GetId();
            wxUpdateUIEvent event(id);
            event.SetEventObject( this );

            if (evtHandler->ProcessEvent(event))
            {
                if (event.GetSetText())
                    menu->SetLabel(id, event.GetText());
                if (event.GetSetChecked())
                    menu->Check(id, event.GetChecked());
                if (event.GetSetEnabled())
                    menu->Enable(id, event.GetEnabled());
            }

            if (item->GetSubMenu())
                DoMenuUpdates(item->GetSubMenu(), (wxWindow*) NULL);
        }
        node = node->GetNext();
    }
}

void wxFrame::OnSize(wxSizeEvent& event)
{
  // if we're using constraints - do use them
  #if wxUSE_CONSTRAINTS
    if ( GetAutoLayout() ) {
      Layout();
      return;
    }
  #endif

  // do we have _exactly_ one child?
  wxWindow *child = NULL;
  for ( wxNode *node = GetChildren().First(); node; node = node->Next() )
  {
    wxWindow *win = (wxWindow *)node->Data();
    if ( !win->IsKindOf(CLASSINFO(wxFrame))  &&
         !win->IsKindOf(CLASSINFO(wxDialog)) && 
         (win != GetStatusBar()) 
#if wxUSE_TOOLBAR
         &&
         (win != GetToolBar()) 
#endif
         )
    {
      if ( child )
        return;     // it's our second subwindow - nothing to do
      child = win;
    }
  }

  if ( child ) {
    // we have exactly one child - set it's size to fill the whole frame
    int clientW, clientH;
    GetClientSize(&clientW, &clientH);

    int x = 0;
    int y = 0;

    child->SetSize(x, y, clientW, clientH);
  }
}

// Default activation behaviour - set the focus for the first child
// subwindow found.
void wxFrame::OnActivate(wxActivateEvent& event)
{
  for(wxNode *node = GetChildren().First(); node; node = node->Next())
  {
    // Find a child that's a subwindow, but not a dialog box.
    wxWindow *child = (wxWindow *)node->Data();
    if (!child->IsKindOf(CLASSINFO(wxFrame)) &&
         !child->IsKindOf(CLASSINFO(wxDialog)))
    {
      child->SetFocus();
      return;
    }
  }
}

// The default implementation for the close window event.
void wxFrame::OnCloseWindow(wxCloseEvent& event)
{
    this->Destroy();
}

// Destroy the window (delayed, if a managed window)
bool wxFrame::Destroy()
{
  if (!wxPendingDelete.Member(this))
    wxPendingDelete.Append(this);
  return TRUE;
}

// Default menu selection behaviour - display a help string
void wxFrame::OnMenuHighlight(wxMenuEvent& event)
{
  if (GetStatusBar())
  {
    if (event.GetMenuId() == -1)
      SetStatusText("");
    else
    {
      wxMenuBar *menuBar = GetMenuBar();
      if (menuBar)
      {
        wxString helpString(menuBar->GetHelpString(event.GetMenuId()));
        if (helpString != "")
          SetStatusText(helpString);
      }
    }
  }
}

wxMenuBar *wxFrame::GetMenuBar() const
{
  return m_frameMenuBar;
}


// Call this to simulate a menu command
void wxFrame::Command(int id)
{
  ProcessCommand(id);
}

void wxFrame::ProcessCommand(int id)
{
  wxCommandEvent commandEvent(wxEVT_COMMAND_MENU_SELECTED, id);
  commandEvent.SetInt( id );
  commandEvent.SetEventObject( this );

  wxMenuBar *bar = GetMenuBar() ;
  if (!bar)
    return;

/* TODO: check the menu item if required
  wxMenuItem *item = bar->FindItemForId(id) ;
  if (item && item->IsCheckable())
  {
    bar->Check(id,!bar->Checked(id)) ;
  }
*/

  GetEventHandler()->ProcessEvent(commandEvent);
}

// Checks if there is a toolbar, and returns the first free client position
wxPoint wxFrame::GetClientAreaOrigin() const
{
    wxPoint pt(0, 0);
#if wxUSE_TOOLBAR
    if (GetToolBar())
    {
        int w, h;
        GetToolBar()->GetSize(& w, & h);

        if (GetToolBar()->GetWindowStyleFlag() & wxTB_VERTICAL)
        {
            pt.x += w;
        }
        else
        {
            pt.y += h;
        }
    }
#endif
    return pt;
}

void wxFrame::DoGetClientSize(int *x, int *y) const
{
	wxWindow::DoGetClientSize( x , y ) ;

  if ( GetStatusBar() )
  {
    int statusX, statusY;
    GetStatusBar()->GetClientSize(&statusX, &statusY);
    *y -= statusY;
  }

  wxPoint pt(GetClientAreaOrigin());
  *y -= pt.y;
  *x -= pt.x;
}

void wxFrame::DoSetClientSize(int clientwidth, int clientheight)
{
	int currentclientwidth , currentclientheight ;
	int currentwidth , currentheight ;
	
	GetClientSize( &currentclientwidth , &currentclientheight ) ;
	GetSize( &currentwidth , &currentheight ) ;
	
	// find the current client size

  // Find the difference between the entire window (title bar and all)
  // and the client area; add this to the new client size to move the
  // window

	DoSetSize( -1 , -1 , currentwidth + clientwidth - currentclientwidth ,
		currentheight + clientheight - currentclientheight , wxSIZE_USE_EXISTING ) ;
}


#if wxUSE_TOOLBAR
wxToolBar* wxFrame::CreateToolBar(long style, wxWindowID id, const wxString& name)
{
    wxCHECK_MSG( m_frameToolBar == NULL, FALSE,
               "recreating toolbar in wxFrame" );

    wxToolBar* toolBar = OnCreateToolBar(style, id, name);
    if (toolBar)
    {
        SetToolBar(toolBar);
        PositionToolBar();
        return toolBar;
    }
    else
    {
        return NULL;
    }
}

wxToolBar* wxFrame::OnCreateToolBar(long style, wxWindowID id, const wxString& name)
{
    return new wxToolBar(this, id, wxDefaultPosition, wxDefaultSize, style, name);
}

void wxFrame::PositionToolBar()
{
    int cw, ch;

    // TODO: we actually need to use the low-level client size, before
    // the toolbar/status bar were added.
    // So DEFINITELY replace the line below with something appropriate.

   //  GetClientSize(& cw, &ch);

		cw = m_width ;
		ch = m_height ;

    if ( GetStatusBar() )
    {
      int statusX, statusY;
      GetStatusBar()->GetClientSize(&statusX, &statusY);
      ch -= statusY;
    }

    if (GetToolBar())
    {
        int tw, th;
        GetToolBar()->GetSize(& tw, & th);

        if (GetToolBar()->GetWindowStyleFlag() & wxTB_VERTICAL)
        {
            // Use the 'real' position. wxSIZE_NO_ADJUSTMENTS
            // means, pretend we don't have toolbar/status bar, so we
            // have the original client size.
            GetToolBar()->SetSize(0, 0, tw, ch, wxSIZE_NO_ADJUSTMENTS);
        }
        else
        {
            // Use the 'real' position
            GetToolBar()->SetSize(0, 0, cw, th, wxSIZE_NO_ADJUSTMENTS);
        }
    }
}
#endif
