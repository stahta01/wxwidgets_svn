/////////////////////////////////////////////////////////////////////////////
// Name:        src/common/xtistrm.cpp
// Purpose:     streaming runtime metadata information
// Author:      Stefan Csomor
// Modified by:
// Created:     27/07/03
// RCS-ID:      $Id$
// Copyright:   (c) 2003 Stefan Csomor
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "xtistrm.h"
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
#include "wx/xtistrm.h"
#include "wx/xtixml.h"
#include "wx/txtstrm.h"
#include "wx/event.h"

#if wxUSE_EXTENDED_RTTI

#include "wx/beforestd.h"
#include <map>
#include <vector>
#include <string>
#include "wx/afterstd.h"

using namespace std ;

//
// XML Streaming
//

// convenience functions

void wxXmlAddContentToNode( wxXmlNode* node , const wxString& data )
{
    node->AddChild(new wxXmlNode(wxXML_TEXT_NODE, "value", data ) );
}

wxString wxXmlGetContentFromNode( wxXmlNode *node )
{
    if ( node->GetChildren() )
        return node->GetChildren()->GetContent() ;
    else
        return wxEmptyString ;
}

struct wxXmlWriter::wxXmlWriterInternal
{
    wxXmlNode *m_root ;
    wxXmlNode *m_current ;
    vector< wxXmlNode * > m_objectStack ;

    void Push( wxXmlNode *newCurrent )
    {
        m_objectStack.push_back( m_current ) ;
        m_current = newCurrent ;
    }

    void Pop()
    {
        m_current = m_objectStack.back() ;
        m_objectStack.pop_back() ;
    }
} ;

wxXmlWriter::wxXmlWriter( wxXmlNode * rootnode )
{
    m_data = new wxXmlWriterInternal() ;
    m_data->m_root = rootnode ;
    m_data->m_current = rootnode ;
}

wxXmlWriter::~wxXmlWriter()
{
    delete m_data ;
}

void wxXmlWriter::DoBeginWriteTopLevelEntry( const wxString &name )
{
    wxXmlNode *pnode;
    pnode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("entry"));
    pnode->AddProperty(wxString("name"), name);
    m_data->m_current->AddChild(pnode) ;
    m_data->Push( pnode ) ;
}

void wxXmlWriter::DoEndWriteTopLevelEntry( const wxString &WXUNUSED(name) )
{
    m_data->Pop() ;
}

void wxXmlWriter::DoBeginWriteObject(const wxObject *WXUNUSED(object), const wxClassInfo *classInfo, int objectID , wxxVariantArray &metadata   )
{
    wxXmlNode *pnode;
    pnode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("object"));
    pnode->AddProperty(wxT("class"), wxString(classInfo->GetClassName()));
    pnode->AddProperty(wxT("id"), wxString::Format( "%d" , objectID ) );

    for ( size_t i = 0 ; i < metadata.GetCount() ; ++i )
    {
        pnode->AddProperty( metadata[i].GetName() , metadata[i].GetAsString() ) ;
    }
    m_data->m_current->AddChild(pnode) ;
    m_data->Push( pnode ) ;
}

// end of writing the root object
void wxXmlWriter::DoEndWriteObject(const wxObject *WXUNUSED(object), const wxClassInfo *WXUNUSED(classInfo), int WXUNUSED(objectID) )
{
    m_data->Pop() ;
}

// writes a property in the stream format
void wxXmlWriter::DoWriteSimpleType( wxxVariant &value )
{
    wxXmlAddContentToNode( m_data->m_current ,value.GetAsString() ) ;
}

void wxXmlWriter::DoBeginWriteElement()
{
    wxXmlNode *pnode;
    pnode = new wxXmlNode(wxXML_ELEMENT_NODE, "element" );
    m_data->m_current->AddChild(pnode) ;
    m_data->Push( pnode ) ;
}

void wxXmlWriter::DoEndWriteElement()
{
    m_data->Pop() ;
}

void wxXmlWriter::DoBeginWriteProperty(const wxPropertyInfo *pi )
{
    wxXmlNode *pnode;
    pnode = new wxXmlNode(wxXML_ELEMENT_NODE, "prop" );
    pnode->AddProperty(wxT("name"), pi->GetName() );
    m_data->m_current->AddChild(pnode) ;
    m_data->Push( pnode ) ;
}

void wxXmlWriter::DoEndWriteProperty(const wxPropertyInfo *WXUNUSED(propInfo) )
{
    m_data->Pop() ;
}



// insert an object reference to an already written object
void wxXmlWriter::DoWriteRepeatedObject( int objectID )
{
    wxXmlNode *pnode;
    pnode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("object"));
    pnode->AddProperty(wxString("href"), wxString::Format( "%d" , objectID ) );
    m_data->m_current->AddChild(pnode) ;
}

// insert a null reference
void wxXmlWriter::DoWriteNullObject()
{
    wxXmlNode *pnode;
    pnode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("object"));
    m_data->m_current->AddChild(pnode) ;
}

// writes a delegate in the stream format
void wxXmlWriter::DoWriteDelegate( const wxObject *WXUNUSED(object),  const wxClassInfo* WXUNUSED(classInfo) , const wxPropertyInfo *WXUNUSED(pi) ,
                                  const wxObject *eventSink, int sinkObjectID , const wxClassInfo* WXUNUSED(eventSinkClassInfo) , const wxHandlerInfo* handlerInfo )
{
    if ( eventSink != NULL && handlerInfo != NULL )
    {
        wxXmlAddContentToNode( m_data->m_current ,wxString::Format(wxT("%d.%s"), sinkObjectID , handlerInfo->GetName().c_str()) ) ;
    }
}

// ----------------------------------------------------------------------------
// reading objects in
// ----------------------------------------------------------------------------



// ----------------------------------------------------------------------------
// reading xml in
// ----------------------------------------------------------------------------

/*
Reading components has not to be extended for components
as properties are always sought by typeinfo over all levels
and create params are always toplevel class only
*/

int wxXmlReader::ReadComponent(wxXmlNode *node, wxDepersister *callbacks)
{
    wxASSERT_MSG( callbacks , wxT("Does not support reading without a Depersistor") ) ;
    wxString className;
    wxClassInfo *classInfo;

    wxxVariant *createParams ;
    int *createParamOids ;
    const wxClassInfo** createClassInfos ;
    wxXmlNode *children;
    int objectID;
    wxString ObjectIdString ;

    children = node->GetChildren();
    if (!children)
    {
        // check for a null object or href
        if (node->GetPropVal("href" , &ObjectIdString ) )
        {
            objectID = atoi( ObjectIdString.c_str() ) ;
            wxASSERT_MSG( HasObjectClassInfo( objectID ) , wxT("Forward hrefs are not supported") ) ;
            return objectID ;
        }
        if ( !node->GetPropVal("id" , &ObjectIdString ) )
        {
            return wxNullObjectID;
        }
    }
    if (!node->GetPropVal("class", &className))
    {
        // No class name.  Eek. FIXME: error handling
        return wxInvalidObjectID;
    }
    classInfo = wxClassInfo::FindClass(className);
    wxASSERT_MSG( classInfo , wxString::Format(wxT("unknown class %s"),className ) ) ;
    wxASSERT_MSG( !children || children->GetType() != wxXML_TEXT_NODE , wxT("objects cannot have XML Text Nodes") ) ;
    if (!node->GetPropVal("id", &ObjectIdString))
    {
        wxASSERT_MSG(0,wxT("Objects must have an id attribute") ) ;
        // No object id.  Eek. FIXME: error handling
        return wxInvalidObjectID;
    }
    objectID = atoi( ObjectIdString.c_str() ) ;
    // is this object already has been streamed in, return it here
    wxASSERT_MSG( !HasObjectClassInfo( objectID ) , wxString::Format(wxT("Doubly used id : %d"), objectID ) ) ;

    // new object, start with allocation
    // first make the object know to our internal registry
    SetObjectClassInfo( objectID , classInfo ) ;

    wxxVariantArray metadata ;
    wxXmlProperty *xp = node->GetProperties() ;
    while ( xp )
    {
        if ( xp->GetName() != wxString("class") && xp->GetName() != wxString("id") )
        {
            metadata.Add( new wxxVariant( xp->GetValue() , xp->GetName() ) ) ;
        }
        xp = xp->GetNext() ;
    }
    callbacks->AllocateObject(objectID, classInfo, metadata);

    //
    // stream back the Create parameters first
    createParams = new wxxVariant[ classInfo->GetCreateParamCount() ] ;
    createParamOids = new int[classInfo->GetCreateParamCount() ] ;
    createClassInfos = new const wxClassInfo*[classInfo->GetCreateParamCount() ] ;

    typedef map<string, wxXmlNode *> PropertyNodes ;
    typedef vector<string> PropertyNames ;

    PropertyNodes propertyNodes ;
    PropertyNames propertyNames ;

    while( children )
    {
        wxString name ;
        children->GetPropVal( wxT("name") , &name ) ;
        propertyNames.push_back( name.c_str() ) ;
        propertyNodes[name.c_str()] = children->GetChildren() ;
        children = children->GetNext() ;
    }

    for ( int i = 0 ; i <classInfo->GetCreateParamCount() ; ++i )
    {
        const wxChar* paramName = classInfo->GetCreateParamName(i) ;
        PropertyNodes::iterator propiter = propertyNodes.find( paramName ) ;
        const wxPropertyInfo* pi = classInfo->FindPropertyInfo( paramName ) ;
        wxASSERT_MSG(pi,wxString::Format("Unkown Property %s",paramName) ) ;
        // if we don't have the value of a create param set in the xml
        // we use the default value
        if ( propiter != propertyNodes.end() )
        {
            wxXmlNode* prop = propiter->second ;
            if ( pi->GetTypeInfo()->IsObjectType() )
            {
                createParamOids[i] = ReadComponent( prop , callbacks ) ;
                createClassInfos[i] = dynamic_cast<const wxClassTypeInfo*>(pi->GetTypeInfo())->GetClassInfo() ;
            }
            else
            {
                createParamOids[i] = wxInvalidObjectID ;
                createParams[i] = ReadValue( prop , pi->GetTypeInfo() ) ;
                if( pi->GetFlags() & wxPROP_ENUM_STORE_LONG )
                {
                    const wxEnumTypeInfo *eti = dynamic_cast<const wxEnumTypeInfo*>( pi->GetTypeInfo() ) ;
                    wxASSERT_MSG( eti , wxT("Type must have enum - long conversion") ) ;

                    long realval ;
                    eti->ConvertToLong( createParams[i]  , realval ) ;
                    createParams[i] = wxxVariant( realval ) ;
                }
                createClassInfos[i] = NULL ;
            }

            for ( size_t j = 0 ; j < propertyNames.size() ; ++j )
            {
                if ( propertyNames[j] == paramName )
                {
                    propertyNames[j] = "" ;
                    break ;
                }
            }
        }
        else
        {
            createParams[i] = pi->GetDefaultValue() ;
        }
    }

    // got the parameters.  Call the Create method
    callbacks->CreateObject(objectID, classInfo,
        classInfo->GetCreateParamCount(),
        createParams, createParamOids, createClassInfos, metadata );

    // now stream in the rest of the properties, in the sequence their properties were written in the xml
    for ( size_t j = 0 ; j < propertyNames.size() ; ++j )
    {
        if ( propertyNames[j].length() )
        {
            PropertyNodes::iterator propiter = propertyNodes.find( propertyNames[j] ) ;
            if ( propiter != propertyNodes.end() )
            {
                wxXmlNode* prop = propiter->second ;
                const wxPropertyInfo* pi = classInfo->FindPropertyInfo( propertyNames[j].c_str() ) ;
                if ( pi->GetTypeInfo()->GetKind() == wxT_COLLECTION )
                {
                    const wxCollectionTypeInfo* collType = dynamic_cast< const wxCollectionTypeInfo* >( pi->GetTypeInfo() ) ;
                    const wxTypeInfo * elementType = collType->GetElementType() ;
                    while( prop )
                    {
                        wxASSERT_MSG(prop->GetName() == wxT("element") , wxT("A non empty collection must consist of 'element' nodes")) ;
                        wxXmlNode* elementContent = prop->GetChildren() ;
                        if ( elementContent )
                        {
                            // we skip empty elements
                            if ( elementType->IsObjectType() )
                            {
                                int valueId = ReadComponent( elementContent , callbacks ) ;
                                if ( valueId != wxInvalidObjectID )
                                {
                                    if ( pi->GetAccessor()->HasAdder() )
                                        callbacks->AddToPropertyCollectionAsObject( objectID , classInfo , pi , valueId ) ;
                                    // TODO for collections we must have a notation on taking over ownership or not
                                    if ( elementType->GetKind() == wxT_OBJECT && valueId != wxNullObjectID )
                                        callbacks->DestroyObject( valueId , GetObjectClassInfo( valueId ) ) ;
                                }
                            }
                            else
                            {
                                wxxVariant elementValue = ReadValue( elementContent , elementType ) ;
                                if ( pi->GetAccessor()->HasAdder() )
                                    callbacks->AddToPropertyCollection( objectID , classInfo ,pi , elementValue ) ;
                            }
                        }
                        prop = prop->GetNext() ;
                    }
                }
                else if ( pi->GetTypeInfo()->IsObjectType() )
                {
                    int valueId = ReadComponent( prop , callbacks ) ;
                    if ( valueId != wxInvalidObjectID )
                    {
                        callbacks->SetPropertyAsObject( objectID , classInfo , pi , valueId ) ;
                        if ( pi->GetTypeInfo()->GetKind() == wxT_OBJECT && valueId != wxNullObjectID )
                            callbacks->DestroyObject( valueId , GetObjectClassInfo( valueId ) ) ;
                    }
                }
                else if ( pi->GetTypeInfo()->IsDelegateType() )
                {
                    if ( prop )
                    {
                        wxString resstring = prop->GetContent() ;
                        wxInt32 pos = resstring.Find('.') ;
                        assert( pos != wxNOT_FOUND ) ;
                        int sinkOid = atol(resstring.Left(pos)) ;
                        wxString handlerName = resstring.Mid(pos+1) ;
                        wxClassInfo* sinkClassInfo = GetObjectClassInfo( sinkOid ) ;

                        callbacks->SetConnect( objectID , classInfo , dynamic_cast<const wxDelegateTypeInfo*>(pi->GetTypeInfo()) , sinkClassInfo ,
                            sinkClassInfo->FindHandlerInfo(handlerName) ,  sinkOid ) ;
                    }

                }
                else
                {
                    wxxVariant nodeval ;
                    callbacks->SetProperty( objectID, classInfo ,pi , ReadValue( prop , pi->GetTypeInfo() ) ) ;
                }
            }
        }
    }

    delete[] createParams ;
    delete[] createParamOids ;
    delete[] createClassInfos ;

    return objectID;
}

wxxVariant wxXmlReader::ReadValue(wxXmlNode *node,
                                  const wxTypeInfo *type )
{
    wxString content ;
    if ( node )
        content = node->GetContent() ;
    wxxVariant result ;
    type->ConvertFromString( content , result ) ;
    return result ;
}

int wxXmlReader::ReadObject( const wxString &name , wxDepersister *callbacks)
{
    wxXmlNode *iter = m_parent->GetChildren() ;
    while ( iter )
    {
        wxString entryName ;
        if ( iter->GetPropVal("name", &entryName) )
        {
            if ( entryName == name )
                return ReadComponent( iter->GetChildren() , callbacks ) ;
        }
        iter = iter->GetNext() ;
    }
    return wxInvalidObjectID ;
}

#endif
