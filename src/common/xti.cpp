/////////////////////////////////////////////////////////////////////////////
// Name:        src/common/xti.cpp
// Purpose:     runtime metadata information (extended class info
// Author:      Stefan Csomor
// Modified by: 
// Created:     27/07/03
// RCS-ID:      $Id$
// Copyright:   (c) 1997 Julian Smart
//              (c) 2003 Stefan Csomor
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "xti.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/hash.h"
    #include "wx/object.h"
#endif

#include "wx/xml/xml.h"
#include "wx/tokenzr.h"

#include <string.h>

#if wxUSE_XTI

// ----------------------------------------------------------------------------
// Enum Support
// ----------------------------------------------------------------------------

wxEnumData::wxEnumData( wxEnumMemberData* data ) 
{
	m_members = data ; 
    for ( m_count = 0; m_members[m_count].m_name ; m_count++)
		{} ;
}

bool wxEnumData::HasEnumMemberValue(const wxChar *name, int *value)
{
    int i;
    for (i = 0; m_members[i].m_name ; i++ )
	{
		if (!strcmp(name, m_members[i].m_name))
		{
			if ( value )
				*value = m_members[i].m_value;
			return true ;
		}
	}
	return false ;
}

int wxEnumData::GetEnumMemberValue(const wxChar *name)
{
    int i;
    for (i = 0; m_members[i].m_name ; i++ )
	{
		if (!strcmp(name, m_members[i].m_name))
		{
			return m_members[i].m_value;
		}
	}
	return 0 ;
}

const wxChar *wxEnumData::GetEnumMemberName(int value)
{
    int i;
    for (i = 0; m_members[i].m_name ; i++)
		if (value == m_members[i].m_value)
			return m_members[i].m_name;

	return wxT("") ;
}

int wxEnumData::GetEnumMemberValueByIndex( int idx ) 
{
	// we should cache the count in order to avoid out-of-bounds errors
	return m_members[idx].m_value ;
}

const char * wxEnumData::GetEnumMemberNameByIndex( int idx ) 
{
	// we should cache the count in order to avoid out-of-bounds errors
	return m_members[idx].m_name ;
}

// ----------------------------------------------------------------------------
// Type Information
// ----------------------------------------------------------------------------

const wxTypeInfo* wxGetTypeInfo( void * )
{
	static wxBuiltInTypeInfo s_typeInfo( wxT_VOID ) ;
	return &s_typeInfo ;
}

const wxTypeInfo* wxGetTypeInfo( bool * )
{
	static wxBuiltInTypeInfo s_typeInfo( wxT_BOOL ) ;
	return &s_typeInfo ;
}

const wxTypeInfo* wxGetTypeInfo( char * )
{
	static wxBuiltInTypeInfo s_typeInfo( wxT_CHAR ) ;
	return &s_typeInfo ;
}

const wxTypeInfo* wxGetTypeInfo( unsigned char * )
{
	static wxBuiltInTypeInfo s_typeInfo( wxT_UCHAR ) ;
	return &s_typeInfo ;
}

const wxTypeInfo* wxGetTypeInfo( int * )
{
	static wxBuiltInTypeInfo s_typeInfo( wxT_CHAR ) ;
	return &s_typeInfo ;
}

const wxTypeInfo* wxGetTypeInfo( unsigned int * )
{
	static wxBuiltInTypeInfo s_typeInfo( wxT_UCHAR ) ;
	return &s_typeInfo ;
}

const wxTypeInfo* wxGetTypeInfo( long * )
{
	static wxBuiltInTypeInfo s_typeInfo( wxT_CHAR ) ;
	return &s_typeInfo ;
}

const wxTypeInfo* wxGetTypeInfo( unsigned long * )
{
	static wxBuiltInTypeInfo s_typeInfo( wxT_UCHAR ) ;
	return &s_typeInfo ;
}

const wxTypeInfo* wxGetTypeInfo( float * )
{
	static wxBuiltInTypeInfo s_typeInfo( wxT_FLOAT ) ;
	return &s_typeInfo ;
}

const wxTypeInfo* wxGetTypeInfo( double * )
{
	static wxBuiltInTypeInfo s_typeInfo( wxT_DOUBLE ) ;
	return &s_typeInfo ;
}

const wxTypeInfo* wxGetTypeInfo( wxString * )
{
	static wxBuiltInTypeInfo s_typeInfo( wxT_STRING ) ;
	return &s_typeInfo ;
}

// this is a compiler induced specialization which is never used anywhere
// const char * should never be active

const wxTypeInfo* wxGetTypeInfo( char const ** )
{
	assert(0) ;
	static wxBuiltInTypeInfo s_typeInfo( wxT_VOID ) ;
	return &s_typeInfo ;
}

void wxStringReadValue(const wxString & , const char* & )
{
	assert(0) ;
}

void wxStringWriteValue(wxString & , char const * const & )
{
	assert(0) ;
}


// ----------------------------------------------------------------------------
// value streaming 
// ----------------------------------------------------------------------------

// convenience function (avoids including xml headers in users code)

void wxXmlAddContentToNode( wxXmlNode* node , const wxString& data ) 
{
	node->AddChild(new wxXmlNode(wxXML_TEXT_NODE, "value", data ) ); 
}

wxString wxXmlGetContentFromNode( wxXmlNode *node ) 
{
	return node->GetChildren()->GetContent() ;
}

// streamer specializations

// TODO for all built-in types

// long

void wxStringReadValue(const wxString &s , long &data )
{
	wxSscanf(s, _T("%ld"), &data ) ;
}

void wxStringWriteValue(wxString &s , const long &data )
{
	s = wxString::Format("%ld", data ) ;
}

// long

void wxStringReadValue(const wxString &s , int &data )
{
	wxSscanf(s, _T("%d"), &data ) ;
}

void wxStringWriteValue(wxString &s , const int &data )
{
	s = wxString::Format("%d", data ) ;
}

// wxString

void wxStringReadValue(const wxString &s , wxString &data )
{
	data = s ;
}

void wxStringWriteValue(wxString &s , const wxString &data )
{
	s = data ;
}

/*
	Custom Data Streaming / Type Infos
	we will have to add this for all wx non object types, but it is also an example
	for custom data structures
*/

// wxPoint

void wxStringReadValue(const wxString &s , wxPoint &data )
{
	wxSscanf(s, _T("%d,%d"), &data.x , &data.y ) ;
}

void wxStringWriteValue(wxString &s , const wxPoint &data )
{
	s = wxString::Format("%d,%d", data.x , data.y ) ;
}

WX_CUSTOM_TYPE_INFO(wxPoint)

// removing header dependancy on string tokenizer

void wxSetStringToArray( const wxString &s , wxArrayString &array ) 
{
    wxStringTokenizer tokenizer(s, wxT("| \t\n"), wxTOKEN_STRTOK);
    wxString flag;
	array.Clear() ;
    while (tokenizer.HasMoreTokens())
    {
		array.Add(tokenizer.GetNextToken()) ;
	}
}

// ----------------------------------------------------------------------------
// wxClassInfo 
// ----------------------------------------------------------------------------


void wxClassInfo::Register(const char *WXUNUSED(name), wxClassInfo *WXUNUSED(info))
{
	/*
    if (!ExtendedTypeMap)
	ExtendedTypeMap = new ClassMap;
    (*ExtendedTypeMap)[string(Name)] = Info;
	*/
}

void wxClassInfo::Unregister(const char *WXUNUSED(name))
{
	/*
    assert(ExtendedTypeMap);
    ExtendedTypeMap->erase(Name);
	*/
}

const wxPropertyAccessor *wxClassInfo::FindAccessor(const char *PropertyName)
{
    const wxPropertyInfo* info = FindPropInfo( PropertyName ) ;
	
	if ( info )
		return info->GetAccessor() ;

	return NULL ;
}

const wxPropertyInfo *wxClassInfo::FindPropInfo (const char *PropertyName) const
{
	const wxPropertyInfo* info = GetFirstProperty() ;

	while( info )
	{
		if ( strcmp( info->GetName() , PropertyName ) == 0 )
			return info ;
		info = info->GetNext() ;
	}

	const wxClassInfo** parents = GetParents() ;
	for ( int i = 0 ; parents[i] ; ++ i )
	{
		if ( ( info = parents[i]->FindPropInfo( PropertyName ) ) != NULL )
			return info ;
	}

    return 0;
}

void wxClassInfo::SetProperty(wxObject *object, const char *propertyName, const wxxVariant &value)
{
    const wxPropertyAccessor *accessor;

    accessor = FindAccessor(propertyName);
    wxASSERT(accessor->HasSetter());
	accessor->SetProperty( object , value ) ;
}

wxxVariant wxClassInfo::GetProperty(wxObject *object, const char *propertyName)
{
    const wxPropertyAccessor *accessor;

    accessor = FindAccessor(propertyName);
    wxASSERT(accessor->HasGetter());
    return accessor->GetProperty(object);
}

/*
VARIANT TO OBJECT
*/

wxObject* wxxVariant::GetAsObject() const
{
	const wxClassTypeInfo *ti = dynamic_cast<const wxClassTypeInfo*>( m_data->GetTypeInfo() ) ;
	if ( ti )
		return ti->GetClassInfo()->VariantToInstance(*this) ;
	else
		return NULL ;
}


#endif
