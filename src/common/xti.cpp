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

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
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

#if wxUSE_EXTENDED_RTTI

#include "wx/xti.h"
#include "wx/xml/xml.h"
#include "wx/tokenzr.h"
#include "wx/list.h"
#include <string.h>

#include "wx/beforestd.h"
#include <map>
#include <string>
#include "wx/afterstd.h"

using namespace std ;

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
        if (!wxStrcmp(name, m_members[i].m_name))
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
        if (!wxStrcmp(name, m_members[i].m_name))
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

const wxChar * wxEnumData::GetEnumMemberNameByIndex( int idx )
{
    // we should cache the count in order to avoid out-of-bounds errors
    return m_members[idx].m_name ;
}

// ----------------------------------------------------------------------------
// Type Information
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// value streaming
// ----------------------------------------------------------------------------

// streamer specializations
// for all built-in types

// bool

template<> void wxStringReadValue(const wxString &s , bool &data )
{
    int intdata ;
    wxSscanf(s, _T("%d"), &intdata ) ;
    data = bool(intdata) ;
}

template<> void wxStringWriteValue(wxString &s , const bool &data )
{
    s = wxString::Format(_T("%d"), data ) ;
}

// char

template<> void wxStringReadValue(const wxString &s , char &data )
{
    int intdata ;
    wxSscanf(s, _T("%d"), &intdata ) ;
    data = char(intdata) ;
}

template<> void wxStringWriteValue(wxString &s , const char &data )
{
    s = wxString::Format(_T("%d"), data ) ;
}

// unsigned char

template<> void wxStringReadValue(const wxString &s , unsigned char &data )
{
    int intdata ;
    wxSscanf(s, _T("%d"), &intdata ) ;
    data = (unsigned char)(intdata) ;
}

template<> void wxStringWriteValue(wxString &s , const unsigned char &data )
{
    s = wxString::Format(_T("%d"), data ) ;
}

// int

template<> void wxStringReadValue(const wxString &s , int &data )
{
    wxSscanf(s, _T("%d"), &data ) ;
}

template<> void wxStringWriteValue(wxString &s , const int &data )
{
    s = wxString::Format(_T("%d"), data ) ;
}

// unsigned int

template<> void wxStringReadValue(const wxString &s , unsigned int &data )
{
    wxSscanf(s, _T("%d"), &data ) ;
}

template<> void wxStringWriteValue(wxString &s , const unsigned int &data )
{
    s = wxString::Format(_T("%d"), data ) ;
}

// long

template<> void wxStringReadValue(const wxString &s , long &data )
{
    wxSscanf(s, _T("%ld"), &data ) ;
}

template<> void wxStringWriteValue(wxString &s , const long &data )
{
    s = wxString::Format(_T("%ld"), data ) ;
}

// unsigned long

template<> void wxStringReadValue(const wxString &s , unsigned long &data )
{
    wxSscanf(s, _T("%ld"), &data ) ;
}

template<> void wxStringWriteValue(wxString &s , const unsigned long &data )
{
    s = wxString::Format(_T("%ld"), data ) ;
}

// float

template<> void wxStringReadValue(const wxString &s , float &data )
{
    wxSscanf(s, _T("%f"), &data ) ;
}

template<> void wxStringWriteValue(wxString &s , const float &data )
{
    s = wxString::Format(_T("%f"), data ) ;
}

// double

template<> void wxStringReadValue(const wxString &s , double &data )
{
    wxSscanf(s, _T("%lf"), &data ) ;
}

template<> void wxStringWriteValue(wxString &s , const double &data )
{
    s = wxString::Format(_T("%lf"), data ) ;
}

// wxString

template<> void wxStringReadValue(const wxString &s , wxString &data )
{
    data = s ;
}

template<> void wxStringWriteValue(wxString &s , const wxString &data )
{
    s = data ;
}

// built-ins
//

wxBuiltInTypeInfo s_typeInfovoid( wxT_VOID , NULL , NULL , typeid(void).name() ) ;
wxBuiltInTypeInfo s_typeInfobool( wxT_BOOL , &wxToStringConverter<bool> , &wxFromStringConverter<bool>, typeid(bool).name()) ;
wxBuiltInTypeInfo s_typeInfochar( wxT_CHAR , &wxToStringConverter<char> , &wxFromStringConverter<char>, typeid(char).name()) ;
wxBuiltInTypeInfo s_typeInfounsignedchar( wxT_UCHAR , &wxToStringConverter< unsigned char > , &wxFromStringConverter<unsigned char>, typeid(unsigned char).name()) ;
wxBuiltInTypeInfo s_typeInfoint( wxT_INT , &wxToStringConverter<int> , &wxFromStringConverter<int>, typeid(int).name()) ;
wxBuiltInTypeInfo s_typeInfounsignedint( wxT_UINT , &wxToStringConverter<unsigned int> , &wxFromStringConverter<unsigned int>, typeid(unsigned int).name()) ;
wxBuiltInTypeInfo s_typeInfolong( wxT_LONG , &wxToStringConverter<long> , &wxFromStringConverter<long>, typeid(long).name()) ;
wxBuiltInTypeInfo s_typeInfounsignedlong( wxT_ULONG , &wxToStringConverter<unsigned long> , &wxFromStringConverter<unsigned long>, typeid(unsigned long).name()) ;
wxBuiltInTypeInfo s_typeInfofloat( wxT_FLOAT , &wxToStringConverter<float> , &wxFromStringConverter<float>, typeid(float).name()) ;
wxBuiltInTypeInfo s_typeInfodouble( wxT_DOUBLE , &wxToStringConverter<double> , &wxFromStringConverter<double>, typeid(double).name()) ;
wxBuiltInTypeInfo s_typeInfowxString( wxT_STRING , &wxToStringConverter<wxString> , &wxFromStringConverter<wxString>, typeid(wxString).name()) ;

// this are compiler induced specialization which are never used anywhere

WX_ILLEGAL_TYPE_SPECIALIZATION( char const * )
WX_ILLEGAL_TYPE_SPECIALIZATION( char * )
WX_ILLEGAL_TYPE_SPECIALIZATION( unsigned char * )
WX_ILLEGAL_TYPE_SPECIALIZATION( int * )
WX_ILLEGAL_TYPE_SPECIALIZATION( bool * )
WX_ILLEGAL_TYPE_SPECIALIZATION( long * )
WX_ILLEGAL_TYPE_SPECIALIZATION( wxString * )

WX_COLLECTION_TYPE_INFO( wxString , wxArrayString ) ;

template<> void wxCollectionToVariantArray( wxArrayString const &theArray, wxxVariantArray &value)
{
    wxArrayCollectionToVariantArray( theArray , value ) ;
}

wxTypeInfoMap *wxTypeInfo::sm_typeTable = NULL ;

wxTypeInfo *wxTypeInfo::FindType(const wxChar *typeName)
{
    wxTypeInfoMap::iterator iter = sm_typeTable->find(typeName) ;
    wxASSERT_MSG( iter != sm_typeTable->end() , wxT("lookup for a non-existent type-info") ) ;
    return (wxTypeInfo *)iter->second;
}

wxClassTypeInfo::wxClassTypeInfo( wxTypeKind kind , wxClassInfo* classInfo , converterToString_t to , converterFromString_t from , const wxString &name) :
wxTypeInfo( kind , to , from , name)
{ wxASSERT_MSG( kind == wxT_OBJECT_PTR || kind == wxT_OBJECT , wxT("Illegal Kind for Enum Type")) ; m_classInfo = classInfo ;}

wxDelegateTypeInfo::wxDelegateTypeInfo( int eventType , wxClassInfo* eventClass , converterToString_t to , converterFromString_t from ) :
wxTypeInfo ( wxT_DELEGATE , to , from , wxEmptyString )
{ m_eventClass = eventClass ; m_eventType = eventType ;}

void wxTypeInfo::Register()
{    
    if ( sm_typeTable == NULL )
        sm_typeTable = new wxTypeInfoMap() ;

    if( !m_name.IsEmpty() )
        (*sm_typeTable)[m_name] = this ;
}

void wxTypeInfo::Unregister()
{
    if( !m_name.IsEmpty() )
        sm_typeTable->erase(m_name);
}

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

wxPropertyInfo::~wxPropertyInfo()
{
    if ( this == m_itsClass->m_firstProperty )
    {
        m_itsClass->m_firstProperty = m_next;
    }
    else
    {
        wxPropertyInfo *info = m_itsClass->m_firstProperty;
        while (info)
        {
            if ( info->m_next == this )
            {
                info->m_next = m_next;
                break;
            }

            info = info->m_next;
        }
    }
}

wxHandlerInfo::~wxHandlerInfo()
{
    if ( this == m_itsClass->m_firstHandler )
    {
        m_itsClass->m_firstHandler = m_next;
    }
    else
    {
        wxHandlerInfo *info = m_itsClass->m_firstHandler;
        while (info)
        {
            if ( info->m_next == this )
            {
                info->m_next = m_next;
                break;
            }

            info = info->m_next;
        }
    }
}

const wxPropertyAccessor *wxClassInfo::FindAccessor(const wxChar *PropertyName) const
{
    const wxPropertyInfo* info = FindPropertyInfo( PropertyName ) ;

    if ( info )
        return info->GetAccessor() ;

    return NULL ;
}

wxPropertyInfo *wxClassInfo::FindPropertyInfoInThisClass (const wxChar *PropertyName) const
{
    wxPropertyInfo* info = m_firstProperty ;

    while( info )
    {
        if ( wxStrcmp( info->GetName() , PropertyName ) == 0 )
            return info ;
        info = info->GetNext() ;
    }

    return 0;
}

const wxPropertyInfo *wxClassInfo::FindPropertyInfo (const wxChar *PropertyName) const
{
    const wxPropertyInfo* info = FindPropertyInfoInThisClass( PropertyName ) ;
    if ( info )
        return info ;

    const wxClassInfo** parents = GetParents() ;
    for ( int i = 0 ; parents[i] ; ++ i )
    {
        if ( ( info = parents[i]->FindPropertyInfo( PropertyName ) ) != NULL )
            return info ;
    }

    return 0;
}

wxHandlerInfo *wxClassInfo::FindHandlerInfoInThisClass (const wxChar *PropertyName) const
{
    wxHandlerInfo* info = m_firstHandler ;

    while( info )
    {
        if ( wxStrcmp( info->GetName() , PropertyName ) == 0 )
            return info ;
        info = info->GetNext() ;
    }

    return 0;
}

const wxHandlerInfo *wxClassInfo::FindHandlerInfo (const wxChar *PropertyName) const
{
    const wxHandlerInfo* info = FindHandlerInfoInThisClass( PropertyName ) ;

    if ( info )
        return info ;

    const wxClassInfo** parents = GetParents() ;
    for ( int i = 0 ; parents[i] ; ++ i )
    {
        if ( ( info = parents[i]->FindHandlerInfo( PropertyName ) ) != NULL )
            return info ;
    }

    return 0;
}

wxObjectStreamingCallback wxClassInfo::GetStreamingCallback() const
{
    if ( m_streamingCallback )
        return m_streamingCallback ;

    wxObjectStreamingCallback retval = NULL ;
    const wxClassInfo** parents = GetParents() ;
    for ( int i = 0 ; parents[i] && retval == NULL ; ++ i )
    {
        retval = parents[i]->GetStreamingCallback() ;
    }
    return retval ;
}

bool wxClassInfo::BeforeWriteObject( const wxObject *obj, wxWriter *streamer , wxPersister *persister , wxxVariantArray &metadata) const  
{
    wxObjectStreamingCallback sb = GetStreamingCallback() ;
    if ( sb )
        return (*sb)(obj , streamer , persister , metadata ) ;

    return true ;
}

void wxClassInfo::SetProperty(wxObject *object, const wxChar *propertyName, const wxxVariant &value) const
{
    const wxPropertyAccessor *accessor;

    accessor = FindAccessor(propertyName);
    wxASSERT(accessor->HasSetter());
    accessor->SetProperty( object , value ) ;
}

wxxVariant wxClassInfo::GetProperty(wxObject *object, const wxChar *propertyName) const
{
    const wxPropertyAccessor *accessor;

    accessor = FindAccessor(propertyName);
    wxASSERT(accessor->HasGetter());
    wxxVariant result ;
    accessor->GetProperty(object,result);
    return result ;
}

wxxVariantArray wxClassInfo::GetPropertyCollection(wxObject *object, const wxChar *propertyName) const
{
    const wxPropertyAccessor *accessor;

    accessor = FindAccessor(propertyName);
    wxASSERT(accessor->HasGetter());
    wxxVariantArray result ;
    accessor->GetPropertyCollection(object,result);
    return result ;
}

void wxClassInfo::AddToPropertyCollection(wxObject *object, const wxChar *propertyName , const wxxVariant& value) const
{
    const wxPropertyAccessor *accessor;

    accessor = FindAccessor(propertyName);
    wxASSERT(accessor->HasAdder());
    accessor->AddToPropertyCollection( object , value ) ;
}

void wxClassInfo::GetProperties( wxPropertyInfoMap &map ) const
{
    const wxPropertyInfo *pi = GetFirstProperty() ;
    while( pi ) 
    {
        if ( map.find( pi->GetName() ) == map.end() )
            map[pi->GetName()] = (wxPropertyInfo*) pi ;

        pi = pi->GetNext() ;
    }

    const wxClassInfo** parents = GetParents() ;
    for ( int i = 0 ; parents[i] ; ++ i )
    {
        parents[i]->GetProperties( map ) ;
    }
}

/*
VARIANT TO OBJECT
*/

wxObject* wxxVariant::GetAsObject()
{
    const wxClassTypeInfo *ti = dynamic_cast<const wxClassTypeInfo*>( m_data->GetTypeInfo() ) ;
    if ( ti )
        return ti->GetClassInfo()->VariantToInstance(*this) ;
    else
        return NULL ;
}

// ----------------------------------------------------------------------------
// wxDynamicObject support
// ----------------------------------------------------------------------------
//
// Dynamic Objects are objects that have a real superclass instance and carry their
// own attributes in a hash map. Like this it is possible to create the objects and
// stream them, as if their class information was already available from compiled data

struct wxDynamicObject::wxDynamicObjectInternal
{
    map<string,wxxVariant> m_properties ;
} ;

// instantiates this object with an instance of its superclass
wxDynamicObject::wxDynamicObject(wxObject* superClassInstance, const wxDynamicClassInfo *info)
{
    m_superClassInstance = superClassInstance ;
    m_classInfo = info ;
    m_data = new wxDynamicObjectInternal ;
}

wxDynamicObject::~wxDynamicObject()
{
    delete m_data ;
    delete m_superClassInstance ;
}

void wxDynamicObject::SetProperty (const wxChar *propertyName, const wxxVariant &value)
{
    wxASSERT_MSG(m_classInfo->FindPropertyInfoInThisClass(propertyName),wxT("Accessing Unknown Property in a Dynamic Object") ) ;
    m_data->m_properties[propertyName] = value ;
}

wxxVariant wxDynamicObject::GetProperty (const wxChar *propertyName) const
{
    wxASSERT_MSG(m_classInfo->FindPropertyInfoInThisClass(propertyName),wxT("Accessing Unknown Property in a Dynamic Object") ) ;
    return m_data->m_properties[propertyName] ;
}

// ----------------------------------------------------------------------------
// wxDynamiClassInfo
// ----------------------------------------------------------------------------

wxDynamicClassInfo::wxDynamicClassInfo( const wxChar *unitName, const wxChar *className , const wxClassInfo* superClass ) :
wxClassInfo( unitName, className , new const wxClassInfo*[2])
{
    GetParents()[0] = superClass ;
    GetParents()[1] = NULL ;
}

wxDynamicClassInfo::~wxDynamicClassInfo()
{
    delete[] GetParents() ;
}

wxObject *wxDynamicClassInfo::AllocateObject() const
{
    wxObject* parent = GetParents()[0]->AllocateObject() ;
    return new wxDynamicObject( parent , this ) ;
}

void wxDynamicClassInfo::Create (wxObject *object, int paramCount, wxxVariant *params) const
{
    wxDynamicObject *dynobj = dynamic_cast< wxDynamicObject *>( object ) ;
    wxASSERT_MSG( dynobj , wxT("cannot call wxDynamicClassInfo::Create on an object other than wxDynamicObject") ) ;
    GetParents()[0]->Create( dynobj->GetSuperClassInstance() , paramCount , params ) ;
}

// get number of parameters for constructor
int wxDynamicClassInfo::GetCreateParamCount() const
{
    return GetParents()[0]->GetCreateParamCount() ;
}

// get i-th constructor parameter
const wxChar* wxDynamicClassInfo::GetCreateParamName(int i) const
{
    return GetParents()[0]->GetCreateParamName( i ) ;
}

void wxDynamicClassInfo::SetProperty(wxObject *object, const wxChar *propertyName, const wxxVariant &value) const
{
    wxDynamicObject* dynobj = dynamic_cast< wxDynamicObject * >( object ) ;
    wxASSERT_MSG( dynobj , wxT("cannot call wxDynamicClassInfo::SetProperty on an object other than wxDynamicObject") ) ;
    if ( FindPropertyInfoInThisClass(propertyName) )
        dynobj->SetProperty( propertyName , value ) ;
    else
        GetParents()[0]->SetProperty( dynobj->GetSuperClassInstance() , propertyName , value ) ;
}

wxxVariant wxDynamicClassInfo::GetProperty(wxObject *object, const wxChar *propertyName) const
{
    wxDynamicObject* dynobj = dynamic_cast< wxDynamicObject * >( object ) ;
    wxASSERT_MSG( dynobj , wxT("cannot call wxDynamicClassInfo::SetProperty on an object other than wxDynamicObject") ) ;
    if ( FindPropertyInfoInThisClass(propertyName) )
        return dynobj->GetProperty( propertyName ) ;
    else
        return GetParents()[0]->GetProperty( dynobj->GetSuperClassInstance() , propertyName ) ;
}

void wxDynamicClassInfo::AddProperty( const wxChar *propertyName , const wxTypeInfo* typeInfo )
{
    new wxPropertyInfo( m_firstProperty , this , propertyName , typeInfo->GetTypeName() , new wxGenericPropertyAccessor( propertyName ) , wxxVariant() ) ;
}

void wxDynamicClassInfo::AddHandler( const wxChar *handlerName , wxObjectEventFunction address , const wxClassInfo* eventClassInfo )
{
    new wxHandlerInfo( m_firstHandler , this , handlerName , address , eventClassInfo ) ;
}

// removes an existing runtime-property
void wxDynamicClassInfo::RemoveProperty( const wxChar *propertyName ) 
{
    delete FindPropertyInfoInThisClass(propertyName) ;
}

// removes an existing runtime-handler
void wxDynamicClassInfo::RemoveHandler( const wxChar *handlerName ) 
{
    delete FindHandlerInfoInThisClass(handlerName) ;
}

// renames an existing runtime-property
void wxDynamicClassInfo::RenameProperty( const wxChar *oldPropertyName , const wxChar *newPropertyName ) 
{
    wxPropertyInfo* pi = FindPropertyInfoInThisClass(oldPropertyName) ;
    wxASSERT_MSG( pi ,wxT("not existing property") ) ;
    pi->m_name = newPropertyName ;
    dynamic_cast<wxGenericPropertyAccessor*>(pi->GetAccessor())->RenameProperty( oldPropertyName , newPropertyName ) ;
}

// renames an existing runtime-handler
void wxDynamicClassInfo::RenameHandler( const wxChar *oldHandlerName , const wxChar *newHandlerName ) 
{
    wxASSERT_MSG(FindHandlerInfoInThisClass(oldHandlerName),wxT("not existing handler") ) ;
    FindHandlerInfoInThisClass(oldHandlerName)->m_name = newHandlerName ;
}

// ----------------------------------------------------------------------------
// wxGenericPropertyAccessor
// ----------------------------------------------------------------------------

struct wxGenericPropertyAccessor::wxGenericPropertyAccessorInternal
{
    char filler ;
} ;

wxGenericPropertyAccessor::wxGenericPropertyAccessor( const wxString& propertyName )
: wxPropertyAccessor( NULL , NULL , NULL , NULL )
{
    m_data = new wxGenericPropertyAccessorInternal ;
    m_propertyName = propertyName ;
    m_getterName = wxT("Get")+propertyName ;
    m_setterName = wxT("Set")+propertyName ;
}

wxGenericPropertyAccessor::~wxGenericPropertyAccessor()
{
    delete m_data ;
}
void wxGenericPropertyAccessor::SetProperty(wxObject *object, const wxxVariant &value) const
{
    wxDynamicObject* dynobj = dynamic_cast< wxDynamicObject * >( object ) ;
    wxASSERT_MSG( dynobj , wxT("cannot call wxDynamicClassInfo::SetProperty on an object other than wxDynamicObject") ) ;
    dynobj->SetProperty(m_propertyName , value ) ;
}

void wxGenericPropertyAccessor::GetProperty(const wxObject *object, wxxVariant& value) const
{
    const wxDynamicObject* dynobj = dynamic_cast< const wxDynamicObject * >( object ) ;
    wxASSERT_MSG( dynobj , wxT("cannot call wxDynamicClassInfo::SetProperty on an object other than wxDynamicObject") ) ;
    value = dynobj->GetProperty( m_propertyName ) ;
}
#endif
