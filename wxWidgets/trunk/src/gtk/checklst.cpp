/////////////////////////////////////////////////////////////////////////////
// Name:        checklst.cpp
// Purpose:
// Author:      Robert Roebling
// Id:          $Id$
// Copyright:   (c) 1998 Robert Roebling
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "checklst.h"
#endif

#include "wx/checklst.h"

//-----------------------------------------------------------------------------
// wxCheckListBox
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxCheckListBox,wxListBox)

wxCheckListBox::wxCheckListBox() :
   wxListBox()
{
    m_hasCheckBoxes = TRUE;
}
wxCheckListBox::wxCheckListBox(wxWindow *parent, wxWindowID id,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 int nStrings = 0, 
                 const wxString choices[] = NULL,
                 long style = 0,
                 const wxValidator& validator = wxDefaultValidator,
                 const wxString& name = wxListBoxNameStr)
{
    m_hasCheckBoxes = TRUE;
    wxListBox::Create( parent, id, pos, size, nStrings, choices, style, validator, name );
}

bool wxCheckListBox::IsChecked( int index ) const
{
    wxCHECK_MSG( m_list != NULL, FALSE, "invalid checklistbox" );
  
    GList *child = g_list_nth( m_list->children, index );
    if (child)
    {
        GtkBin *bin = GTK_BIN( child->data );
        GtkLabel *label = GTK_LABEL( bin->child );
	
	wxString str = label->label;
    
        return (str[1] == 'X');
    }
    
    wxFAIL_MSG("wrong checklistbox index");
    return FALSE;
}

void wxCheckListBox::Check( int index, bool check )
{
    wxCHECK_RET( m_list != NULL, "invalid checklistbox" );
  
    GList *child = g_list_nth( m_list->children, index );
    if (child)
    {
        GtkBin *bin = GTK_BIN( child->data );
        GtkLabel *label = GTK_LABEL( bin->child );
	
	wxString str = label->label;
    
        if (check == (str[1] == 'X')) return;
	
	if (check)
	    str.SetChar( 1, 'X' );
	else
	    str.SetChar( 1, '-' );
	    
	gtk_label_set( label, str );
	    
	wxCommandEvent event( wxEVT_COMMAND_CHECKLISTBOX_TOGGLED, GetId() );
	event.SetEventObject( this );
	event.SetInt( index );
	GetEventHandler()->ProcessEvent( event );
	
	return;
    }
    
    wxFAIL_MSG("wrong checklistbox index");
}

