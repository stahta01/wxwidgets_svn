/////////////////////////////////////////////////////////////////////////////
// Name:        statline.cpp
// Purpose:
// Author:      Robert Roebling
// Id:          $Id$
// Copyright:   (c) 1998 Robert Roebling
// Licence:           wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "statline.h"
#endif

#include "wx/statline.h"

#include "gdk/gdk.h"
#include "gtk/gtk.h"

//-----------------------------------------------------------------------------
// wxStaticLine
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxStaticLine,wxControl)

wxStaticLine::wxStaticLine(void)
{
}

wxStaticLine::wxStaticLine( wxWindow *parent, wxWindowID id,
      const wxPoint &pos, const wxSize &size, 
      long style, const wxString &name )
{
    Create( parent, id, pos, size, style, name );
}

bool wxStaticLine::Create( wxWindow *parent, wxWindowID id, 
      const wxPoint &pos, const wxSize &size, 
      long style, const wxString &name )
{
    m_needParent = TRUE;
  
    PreCreation( parent, id, pos, size, style, name );

    if (style & wxVERTICAL)
        m_widget = gtk_vseparator_new();
    else
        m_widget = gtk_hseparator_new();
  
    m_parent->DoAddChild( this );
  
    PostCreation();
  
    Show( TRUE );
    
    return TRUE;
}

