/////////////////////////////////////////////////////////////////////////////
// Name:        choice.cpp
// Purpose:     wxChoice
// Author:      AUTHOR
// Modified by:
// Created:     ??/??/98
// RCS-ID:      $Id$
// Copyright:   (c) AUTHOR
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "choice.h"
#endif

#include "wx/defs.h"

#include "wx/choice.h"
#include "wx/menu.h"
#include "wx/mac/uma.h"

#if !USE_SHARED_LIBRARY
IMPLEMENT_DYNAMIC_CLASS(wxChoice, wxControl)
#endif

short nextMenuId = 100 ; // wxMenu takes the lower ids

wxChoice::~wxChoice()
{
	// DeleteMenu( m_macPopUpMenuId ) ;
	// DisposeMenu( m_macPopUpMenuHandle ) ;
}

bool wxChoice::Create(wxWindow *parent, wxWindowID id,
           const wxPoint& pos,
           const wxSize& size,
		   int n, const wxString choices[],
		   long style,
           const wxValidator& validator,
           const wxString& name)
{

		Rect bounds ;
		Str255 title ;
	
		MacPreControlCreate( parent , id ,  "" , pos , size ,style, validator , name , &bounds , title ) ;

		m_macControl = UMANewControl( parent->GetMacRootWindow() , &bounds , title , false , 0 , -12345 , 0 , 
	  	kControlPopupButtonProc + kControlPopupFixedWidthVariant , (long) this ) ; 
	
		m_macPopUpMenuHandle =  NewMenu( 1 , "\pPopUp Menu" ) ;
		SetControlData( m_macControl , kControlNoPart , kControlPopupButtonMenuHandleTag , sizeof( MenuHandle ) , (char*) &m_macPopUpMenuHandle) ;
	 	SetControlMinimum( m_macControl , 0 ) ;
		SetControlMaximum( m_macControl , 0) ;
		if ( n > 0 )
			SetControlValue( m_macControl , 1 ) ;

		MacPostControlCreate() ;

    for ( int i = 0; i < n; i++ )
    {
        Append(choices[i]);
    }
  	return TRUE;
}

// ----------------------------------------------------------------------------
// adding/deleting items to/from the list
// ----------------------------------------------------------------------------

int wxChoice::DoAppend(const wxString& item)
{
	Str255 label;
	wxMenuItem::MacBuildMenuString( label , NULL , NULL , item ,false);
	AppendMenu( m_macPopUpMenuHandle , label ) ;
	m_strings.Add( item ) ;
	m_datas.Add( NULL ) ;
	int index = m_strings.GetCount()  - 1  ;
	DoSetItemClientData( index , NULL ) ;
	SetControlMaximum( m_macControl , GetCount()) ;
	return index ;
}

void wxChoice::Delete(int n)
{
    wxCHECK_RET( n < GetCount(), wxT("invalid item index in wxChoice::Delete") );

    if ( HasClientObjectData() )
    {
        delete GetClientObject(n);
    }

    ::DeleteMenuItem( m_macPopUpMenuHandle , n + 1) ;
    m_strings.Remove( n ) ;
    m_datas.RemoveAt( n ) ;
	SetControlMaximum( m_macControl , GetCount()) ;
}

void wxChoice::Clear()
{
    Free();

    for ( int i = 0 ; i < GetCount() ; i++ )
    {
    	::DeleteMenuItem( m_macPopUpMenuHandle , 1 ) ;
	  }
    m_strings.Empty() ;
    m_datas.Empty() ;
	SetControlMaximum( m_macControl , 0 ) ;
}

void wxChoice::Free()
{
    if ( HasClientObjectData() )
    {
        size_t count = GetCount();
        for ( size_t n = 0; n < count; n++ )
        {
            delete GetClientObject(n);
        }
    }
}

// ----------------------------------------------------------------------------
// selection
// ----------------------------------------------------------------------------

int wxChoice::GetSelection() const
{
    return GetControlValue( m_macControl ) -1 ;
}

void wxChoice::SetSelection(int n)
{
    SetControlValue( m_macControl , n + 1 ) ;
}

// ----------------------------------------------------------------------------
// string list functions
// ----------------------------------------------------------------------------

int wxChoice::GetCount() const
{
    return m_strings.GetCount() ;
}

int wxChoice::FindString(const wxString& s) const
{
    for( int i = 0 ; i < GetCount() ; i++ )
    {
    	if ( GetString( i ).IsSameAs(s, FALSE) )
    		return i ; 
    }
    return wxNOT_FOUND ;
}

void wxChoice::SetString(int n, const wxString& s)
{
    wxFAIL_MSG(wxT("not implemented"));

#if 0 // should do this, but no Insert() so far
    Delete(n);
    Insert(n + 1, s);
#endif
}


wxString wxChoice::GetString(int n) const
{
	return m_strings[n] ;
}

// ----------------------------------------------------------------------------
// client data
// ----------------------------------------------------------------------------

void wxChoice::DoSetItemClientData( int n, void* clientData )
{
    wxCHECK_RET( n >= 0 && n < m_datas.GetCount(),
                 "invalid index in wxChoice::SetClientData" );
	wxASSERT_MSG( m_datas.GetCount() >= n , "invalid client_data array" ) ;
	
	if ( m_datas.GetCount() > n )
	{
    	m_datas[n] = (char*) clientData ;
    }
    else
    {
    	m_datas.Add( (char*) clientData ) ;
    }
}

void *wxChoice::DoGetItemClientData(int N) const
{
    wxCHECK_MSG( N >= 0 && N < m_datas.GetCount(), NULL,
                 "invalid index in wxChoice::GetClientData" );

    return (void *)m_datas[N];
}

void wxChoice::DoSetItemClientObject( int n, wxClientData* clientData )
{
    DoSetItemClientData(n, clientData);
}

wxClientData* wxChoice::DoGetItemClientObject( int n ) const
{
    return (wxClientData *)DoGetItemClientData(n);
}

void wxChoice::MacHandleControlClick( ControlHandle control , SInt16 controlpart ) 
{
    wxCommandEvent event(wxEVT_COMMAND_CHOICE_SELECTED, m_windowId );
	  event.SetInt(GetSelection());
    event.SetEventObject(this);
    event.SetString(GetStringSelection());
    ProcessCommand(event);
}
/*
void wxChoice::Command(wxCommandEvent & event)
{
    SetSelection (event.GetInt());
    ProcessCommand (event);
}
*/
