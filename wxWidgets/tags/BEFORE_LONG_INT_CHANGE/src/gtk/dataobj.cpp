///////////////////////////////////////////////////////////////////////////////
// Name:        dataobj.cpp
// Purpose:     wxDataObject class
// Author:      Robert Roebling
// Id:          $Id$
// Copyright:   (c) 1998 Robert Roebling
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
    #pragma implementation "dataobj.h"
#endif

#include "wx/dataobj.h"
#include "wx/app.h"
#include "wx/debug.h"
#include "wx/mstream.h"
#include "wx/image.h"

#include "gdk/gdk.h"


//-------------------------------------------------------------------------
// global data
//-------------------------------------------------------------------------

GdkAtom  g_textAtom        = 0;
GdkAtom  g_pngAtom         = 0;

//-------------------------------------------------------------------------
// wxDataFormat
//-------------------------------------------------------------------------

IMPLEMENT_CLASS(wxDataFormat, wxObject)

wxDataFormat::wxDataFormat()
{
    PrepareFormats();
    m_type = wxDF_INVALID;
    m_hasAtom = FALSE;
    m_atom = (GdkAtom) 0;
}

wxDataFormat::wxDataFormat( wxDataFormatId type )
{
    PrepareFormats();
    SetType( type );
}

wxDataFormat::wxDataFormat( const wxChar *id )
{
    PrepareFormats();
    SetId( id );
}

wxDataFormat::wxDataFormat( const wxString &id )
{
    PrepareFormats();
    SetId( id );
}

wxDataFormat::wxDataFormat( const wxDataFormat &format )
{
    PrepareFormats();
    m_type = format.GetType();
    m_id = format.GetId();
    m_hasAtom = TRUE;
    m_atom = ((wxDataFormat &)format).GetAtom();    // const_cast
}

wxDataFormat::wxDataFormat( const GdkAtom atom )
{
    PrepareFormats();
    m_hasAtom = TRUE;

    m_atom = atom;

    if (m_atom == g_textAtom)
    {
        m_type = wxDF_TEXT;
    } else
    if (m_atom == GDK_TARGET_BITMAP)
    {
        m_type = wxDF_BITMAP;
    } else
    {
        m_type = wxDF_PRIVATE;
	m_id = gdk_atom_name( m_atom );

	if (m_id == wxT("file:ALL"))
	{
	    m_type = wxDF_FILENAME;
	}
    }
}

void wxDataFormat::SetType( wxDataFormatId type )
{
    m_type = type;

    if (m_type == wxDF_TEXT)
    {
        m_id = wxT("STRING");
    }
    else
    if (m_type == wxDF_BITMAP)
    {
        m_id = wxT("image/png");
    }
    else
    if (m_type == wxDF_FILENAME)
    {
        m_id = wxT("file:ALL");
    }
    else
    {
       wxFAIL_MSG( wxT("invalid dataformat") );
    }

    m_hasAtom = FALSE;
}

wxDataFormatId wxDataFormat::GetType() const
{
    return m_type;
}

wxString wxDataFormat::GetId() const
{
    return m_id;
}

void wxDataFormat::SetId( const wxChar *id )
{
    m_type = wxDF_PRIVATE;
    m_id = id;
    m_hasAtom = FALSE;
}

GdkAtom wxDataFormat::GetAtom()
{
    if (!m_hasAtom)
    {
        m_hasAtom = TRUE;

	if (m_type == wxDF_TEXT)
	{
            m_atom = g_textAtom;
        }
	else
        if (m_type == wxDF_BITMAP)
        {
            m_atom = GDK_TARGET_BITMAP;
        }
	else
        if (m_type == wxDF_PRIVATE)
        {
            m_atom = gdk_atom_intern( wxMBSTRINGCAST m_id.mbc_str(), FALSE );
        }
	else
	if (m_type == wxDF_FILENAME)
	{
	    m_atom = gdk_atom_intern( "file:ALL", FALSE );
	}
	else
	{
	    m_hasAtom = FALSE;
	    m_atom = (GdkAtom) 0;
	}
    }

    return m_atom;
}

void wxDataFormat::PrepareFormats()
{
    if (!g_textAtom) g_textAtom = gdk_atom_intern( "STRING", FALSE );
    if (!g_pngAtom) g_pngAtom = gdk_atom_intern( "image/png", FALSE );
}

//-------------------------------------------------------------------------
// wxDataBroker
//-------------------------------------------------------------------------

IMPLEMENT_CLASS(wxDataBroker,wxObject)

wxDataBroker::wxDataBroker()
{
    m_dataObjects.DeleteContents(TRUE);
    m_preferred = 0;
}

void wxDataBroker::Add( wxDataObject *dataObject, bool preferred )
{
    if (preferred) m_preferred = m_dataObjects.GetCount();
    m_dataObjects.Append( dataObject );
}

size_t wxDataBroker::GetFormatCount() const
{
    return m_dataObjects.GetCount();
}

wxDataFormatId wxDataBroker::GetPreferredFormat() const
{
    wxNode *node = m_dataObjects.Nth( m_preferred );

    wxASSERT( node );

    wxDataObject* data_obj = (wxDataObject*)node->Data();

    return data_obj->GetFormat().GetType();
}

wxDataFormat &wxDataBroker::GetNthFormat( size_t nth ) const
{
    wxNode *node = m_dataObjects.Nth( nth );

    wxASSERT( node );

    wxDataObject* data_obj = (wxDataObject*)node->Data();

    return data_obj->GetFormat();
}

bool wxDataBroker::IsSupportedFormat( wxDataFormat &format ) const
{
    wxNode *node = m_dataObjects.First();
    while (node)
    {
        wxDataObject *dobj = (wxDataObject*)node->Data();

	if (dobj->GetFormat().GetAtom() == format.GetAtom())
	{
	    return TRUE;
	}

        node = node->Next();
    }

    return FALSE;
}

size_t wxDataBroker::GetSize( wxDataFormat& format ) const
{
    wxNode *node = m_dataObjects.First();
    while (node)
    {
        wxDataObject *dobj = (wxDataObject*)node->Data();

	if (dobj->GetFormat().GetAtom() == format.GetAtom())
	{
	    return dobj->GetSize();
	}

        node = node->Next();
    }

    return 0;
}

void wxDataBroker::WriteData( wxDataFormat& format, void *dest ) const
{
    wxNode *node = m_dataObjects.First();
    while (node)
    {
        wxDataObject *dobj = (wxDataObject*)node->Data();

	if (dobj->GetFormat().GetAtom() == format.GetAtom())
	{
	    dobj->WriteData( dest );
	}

        node = node->Next();
    }
}

//-------------------------------------------------------------------------
// wxDataObject
//-------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_CLASS( wxDataObject, wxObject )

wxDataObject::wxDataObject()
{
}

wxDataObject::~wxDataObject()
{
}

wxDataFormat &wxDataObject::GetFormat()
{
    return m_format;
}

wxDataFormatId wxDataObject::GetFormatType() const
{
    return m_format.GetType();
}

wxString wxDataObject::GetFormatId() const
{
    return m_format.GetId();
}

GdkAtom wxDataObject::GetFormatAtom() const
{
    GdkAtom ret = ((wxDataObject*) this)->m_format.GetAtom();
    return ret;
}

// ----------------------------------------------------------------------------
// wxTextDataObject
// ----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS( wxTextDataObject, wxDataObject )

wxTextDataObject::wxTextDataObject()
{
    m_format.SetType( wxDF_TEXT );
}

wxTextDataObject::wxTextDataObject( const wxString& data )
{
    m_format.SetType( wxDF_TEXT );

    m_data = data;
}

void wxTextDataObject::SetText( const wxString& data )
{
    m_data = data;
}

wxString wxTextDataObject::GetText() const
{
    return m_data;
}

void wxTextDataObject::WriteData( void *dest ) const
{
    WriteString( m_data, dest );
}

size_t wxTextDataObject::GetSize() const
{
    return m_data.Len() + 1;
}

void wxTextDataObject::WriteString( const wxString &str, void *dest ) const
{
    memcpy( dest, str.mb_str(), str.Len()+1 );
}

// ----------------------------------------------------------------------------
// wxFileDataObject
// ----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS( wxFileDataObject, wxDataObject )

wxFileDataObject::wxFileDataObject()
{
    m_format.SetType( wxDF_FILENAME );
}

void wxFileDataObject::AddFile( const wxString &file )
{
    m_files += file;
    m_files += (wxChar)0;
}

wxString wxFileDataObject::GetFiles() const
{
    return m_files;
}

void wxFileDataObject::WriteData( void *dest ) const
{
    memcpy( dest, m_files.mbc_str(), GetSize() );
}

size_t wxFileDataObject::GetSize() const
{
    return m_files.Len() + 1;
}

// ----------------------------------------------------------------------------
// wxBitmapDataObject
// ----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS( wxBitmapDataObject, wxDataObject )

wxBitmapDataObject::wxBitmapDataObject()
{
    m_format.SetType( wxDF_BITMAP );
    m_pngData = (char*)NULL;
    m_pngSize = 0;
}

wxBitmapDataObject::wxBitmapDataObject( const wxBitmap& bitmap )
{
    m_format.SetType( wxDF_BITMAP );
    m_pngData = (char*)NULL;
    m_pngSize = 0;
    m_bitmap = bitmap;
    DoConvertToPng();
}

wxBitmapDataObject::~wxBitmapDataObject()
{
    if (m_pngData) delete[] m_pngData;
}

void wxBitmapDataObject::SetBitmap( const wxBitmap &bitmap )
{
    m_bitmap = bitmap;
    DoConvertToPng();
}

wxBitmap wxBitmapDataObject::GetBitmap() const
{
    return m_bitmap;
}

void wxBitmapDataObject::WriteData( void *dest ) const
{
    WriteBitmap( m_bitmap, dest );
}

size_t wxBitmapDataObject::GetSize() const
{
    return m_pngSize;
}

void wxBitmapDataObject::WriteBitmap( const wxBitmap &WXUNUSED(bitmap), void *dest ) const
{
//    if (m_bitmap == bitmap)
        memcpy( dest, m_pngData, m_pngSize );
}

void wxBitmapDataObject::SetPngData( const char *pngData, size_t pngSize )
{
    if (m_pngData) delete[] m_pngData;
    m_pngData = (char*) NULL;
    m_pngSize = pngSize;
    m_pngData = new char[m_pngSize];
    memcpy( m_pngData, pngData, m_pngSize );
    
    wxMemoryInputStream mstream( pngData, pngSize );
    wxImage image;
    wxPNGHandler handler;
    handler.LoadFile( &image, mstream );
    m_bitmap = image.ConvertToBitmap();
}

void wxBitmapDataObject::DoConvertToPng()
{
    if (m_pngData) delete[] m_pngData;
    
    wxImage image( m_bitmap );
    wxPNGHandler handler;
    
    wxCountingOutputStream count;
    handler.SaveFile( &image, count );
    m_pngSize = count.GetSize() + 100; // sometimes the size seems to vary ???
    m_pngData = new char[m_pngSize];
    
    wxMemoryOutputStream mstream( m_pngData, m_pngSize );
    handler.SaveFile( &image, mstream );
}

// ----------------------------------------------------------------------------
// wxPrivateDataObject
// ----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS( wxPrivateDataObject, wxDataObject )

void wxPrivateDataObject::Free()
{
    if ( m_data )
        free(m_data);
}

wxPrivateDataObject::wxPrivateDataObject()
{
    wxString id = wxT("application/");
    id += wxTheApp->GetAppName();

    m_format.SetId( id );

    m_size = 0;
    m_data = (void *)NULL;
}

void wxPrivateDataObject::SetData( const void *data, size_t size )
{
    Free();

    m_size = size;
    m_data = malloc(size);

    memcpy( m_data, data, size );
}

void wxPrivateDataObject::WriteData( void *dest ) const
{
    WriteData( m_data, dest );
}

size_t wxPrivateDataObject::GetSize() const
{
    return m_size;
}

void wxPrivateDataObject::WriteData( const void *data, void *dest ) const
{
    memcpy( dest, data, GetSize() );
}

