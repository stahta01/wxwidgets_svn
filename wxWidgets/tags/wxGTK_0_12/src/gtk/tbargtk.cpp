/////////////////////////////////////////////////////////////////////////////
// Name:        tbargtk.cpp
// Purpose:     GTK toolbar
// Author:      Robert Roebling
// Modified by:
// Created:     01/02/97
// RCS-ID:      
// Copyright:   (c) Robert Roebling
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "tbargtk.h"
#endif

#include "wx/toolbar.h"

//-----------------------------------------------------------------------------
// wxToolBarTool
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxToolBarTool,wxObject)
  
wxToolBarTool::wxToolBarTool( wxToolBarGTK *owner, const int theIndex, 
      const wxBitmap& bitmap1, const  wxBitmap& bitmap2, 
      const bool toggle, wxObject *clientData,
      const wxString& shortHelpString, const wxString& longHelpString )
{
  m_owner = owner;
  m_index = theIndex;
  m_bitmap1 = bitmap1;
  m_bitmap2 = bitmap2;
  m_isToggle = toggle;
  m_enabled = TRUE;
  m_toggleState = FALSE;
  m_shortHelpString = shortHelpString;
  m_longHelpString = longHelpString;
  m_isMenuCommand = TRUE;
  m_clientData = clientData;
  m_deleteSecondBitmap = FALSE;
};

wxToolBarTool::~wxToolBarTool(void)
{
};

//-----------------------------------------------------------------------------
// wxToolBarGTK
//-----------------------------------------------------------------------------

static void gtk_toolbar_callback( GtkWidget *WXUNUSED(widget), wxToolBarTool *tool )
{
  if (!tool->m_enabled) return;
  
  if (tool->m_isToggle) tool->m_toggleState = !tool->m_toggleState;
  
  tool->m_owner->OnLeftClick( tool->m_index, tool->m_toggleState );
};

//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxToolBarGTK,wxControl)

BEGIN_EVENT_TABLE(wxToolBarGTK, wxControl)
END_EVENT_TABLE()

wxToolBarGTK::wxToolBarGTK(void)
{
};

wxToolBarGTK::wxToolBarGTK( wxWindow *parent, const wxWindowID id, 
  const wxPoint& pos, const wxSize& size,
  const long style, const wxString& name )
{
  Create( parent, id, pos, size, style, name );
};

wxToolBarGTK::~wxToolBarGTK(void)
{
};

bool wxToolBarGTK::Create( wxWindow *parent, const wxWindowID id, 
  const wxPoint& pos, const wxSize& size,
  const long style, const wxString& name )
{
  m_needParent = TRUE;
  
  PreCreation( parent, id, pos, size, style, name );

  m_tools.DeleteContents( TRUE );
    
  m_widget = gtk_handle_box_new();
  
  m_toolbar = GTK_TOOLBAR( gtk_toolbar_new( GTK_ORIENTATION_HORIZONTAL, GTK_TOOLBAR_ICONS ) );
  
  gtk_container_add( GTK_CONTAINER(m_widget), GTK_WIDGET(m_toolbar) );
  
  gtk_widget_show( GTK_WIDGET(m_toolbar) );
  
  PostCreation();
  
  Show( TRUE );
    
  return TRUE;
};

bool wxToolBarGTK::OnLeftClick( int toolIndex, bool toggleDown )
{
  wxCommandEvent event(wxEVT_COMMAND_TOOL_CLICKED, toolIndex);
  event.SetEventObject(this);
  event.SetExtraLong((long) toggleDown);

  GetEventHandler()->ProcessEvent(event);

  return TRUE;
};

void wxToolBarGTK::OnRightClick( int toolIndex, float WXUNUSED(x), float WXUNUSED(y) )
{
  wxCommandEvent event(wxEVT_COMMAND_TOOL_RCLICKED, toolIndex);
  event.SetEventObject(this);

  GetEventHandler()->ProcessEvent(event);
};

void wxToolBarGTK::OnMouseEnter( int toolIndex )
{
  wxCommandEvent event(wxEVT_COMMAND_TOOL_ENTER, toolIndex);
  event.SetEventObject(this);

  GetEventHandler()->ProcessEvent(event);
};

wxToolBarTool *wxToolBarGTK::AddTool( const int toolIndex, const wxBitmap& bitmap, 
  const wxBitmap& pushedBitmap, const bool toggle, 
  const float WXUNUSED(xPos), const float WXUNUSED(yPos), wxObject *clientData,  
  const wxString& helpString1, const wxString& helpString2 )
{
  if (!bitmap.Ok()) return NULL;
  
  wxToolBarTool *tool = new wxToolBarTool( this, toolIndex, bitmap, pushedBitmap, toggle, 
  clientData, helpString1, helpString2 );
  
  GdkPixmap *pixmap = bitmap.GetPixmap();
  
  GdkBitmap *mask = NULL;
  if (bitmap.GetMask()) mask = bitmap.GetMask()->GetBitmap();
  
  GtkWidget *tool_pixmap = gtk_pixmap_new( pixmap, mask );
  gtk_misc_set_alignment( GTK_MISC(tool_pixmap), 0.5, 0.5 );
  
  GtkToolbarChildType ctype = GTK_TOOLBAR_CHILD_BUTTON;
  if (toggle) ctype = GTK_TOOLBAR_CHILD_TOGGLEBUTTON;
  
  gtk_toolbar_append_element( m_toolbar, 
    ctype, NULL, NULL, helpString1, "", tool_pixmap, (GtkSignalFunc)gtk_toolbar_callback, (gpointer)tool );
    
  m_tools.Append( tool );    
    
  return tool;
};

void wxToolBarGTK::AddSeparator(void)
{
  gtk_toolbar_append_space( m_toolbar );
};

void wxToolBarGTK::ClearTools(void)
{
};

void wxToolBarGTK::EnableTool(const int toolIndex, const bool enable)
{
};

void wxToolBarGTK::ToggleTool(const int toolIndex, const bool toggle)
{
};

void wxToolBarGTK::SetToggle(const int toolIndex, const bool toggle) 
{
};

wxObject *wxToolBarGTK::GetToolClientData(const int index) const
{
};

bool wxToolBarGTK::GetToolState(const int toolIndex) const
{
};

bool wxToolBarGTK::GetToolEnabled(const int toolIndex) const
{
};

void wxToolBarGTK::SetMargins(const int x, const int y)
{
};

void wxToolBarGTK::SetToolPacking(const int packing)
{
};

void wxToolBarGTK::SetToolSeparation(const int separation)
{
};

