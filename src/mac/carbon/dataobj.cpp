///////////////////////////////////////////////////////////////////////////////
// Name:        src/mac/carbon/dataobj.cpp
// Purpose:     implementation of wxDataObject class
// Author:      Stefan Csomor
// Modified by:
// Created:     10/21/99
// RCS-ID:      $Id$
// Copyright:   (c) 1999 Stefan Csomor
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#if wxUSE_DATAOBJ

#include "wx/dataobj.h"

#ifndef WX_PRECOMP
    #include "wx/intl.h"
    #include "wx/log.h"
    #include "wx/dcmemory.h"
    #include "wx/image.h"
#endif

#include "wx/mstream.h"
#include "wx/metafile.h"
#include "wx/tokenzr.h"

#include "wx/mac/uma.h"

#ifndef __DARWIN__
#include <Scrap.h>
#else
    #include <QuickTime/QuickTime.h>
#endif


// ----------------------------------------------------------------------------
// wxDataFormat
// ----------------------------------------------------------------------------

wxDataFormat::wxDataFormat()
{
    m_type = wxDF_INVALID;
    m_format = 0;
}

wxDataFormat::wxDataFormat( wxDataFormatId vType )
{
    SetType( vType );
}

wxDataFormat::wxDataFormat( const wxChar *zId )
{
    SetId( zId );
}

wxDataFormat::wxDataFormat( const wxString& rId )
{
    SetId( rId );
}

wxDataFormat::wxDataFormat( NativeFormat vFormat )
{
    SetId( vFormat );
}

void wxDataFormat::SetType( wxDataFormatId dataType )
{
    m_type = dataType;

    switch (m_type)
    {
    case wxDF_TEXT:
        m_format = kScrapFlavorTypeText;
        break;

    case wxDF_UNICODETEXT:
        m_format = kScrapFlavorTypeUnicode;
        break;

    case wxDF_BITMAP:
#if wxMAC_USE_CORE_GRAPHICS
        m_format = 'TIFF';
#else
        m_format = kScrapFlavorTypePicture;
#endif
        break;
    case wxDF_METAFILE:
#if wxMAC_USE_CORE_GRAPHICS
        m_format = 'PDF ';
#else
        m_format = kScrapFlavorTypePicture;
#endif
        break;

    case wxDF_FILENAME:
        m_format = kDragFlavorTypeHFS;
        break;

    default:
       wxFAIL_MSG( wxT("invalid data format") );

       // NB: this translates to '????' ASCII but it can't be used in the code
       // because '??' will get parsed as a trigraph!
       m_format = 0x3f3f3f3f;
       break;
    }
}

wxString wxDataFormat::GetId() const
{
    wxCHECK_MSG( !IsStandard(), wxEmptyString,
                 wxT("name of predefined format cannot be retrieved") );

    return m_id;
}

void wxDataFormat::SetId( NativeFormat format )
{
    m_format = format;

    switch (m_format)
    {
    case kScrapFlavorTypeText:
        m_type = wxDF_TEXT;
        break;

    case kScrapFlavorTypeUnicode:
        m_type = wxDF_UNICODETEXT;
        break;
        
#if wxMAC_USE_CORE_GRAPHCIS
    case 'TIFF':
        m_type = wxDF_BITMAP;
        break;
#endif
    case kScrapFlavorTypePicture:
        m_type = wxDF_METAFILE;
        break;

    case kDragFlavorTypeHFS:
        m_type = wxDF_FILENAME;
        break;

    default:
        m_type = wxDF_PRIVATE;
        char text[5];
        memcpy( text, (const char*)&format, 4 );
        text[4] = 0;
        m_id = wxString::FromAscii( text );
        break;
    }
}

void wxDataFormat::SetId( const wxChar* zId )
{
    m_type = wxDF_PRIVATE;
    m_id = zId;
    m_format = 'WXPR';
}

bool wxDataFormat::operator==(const wxDataFormat& format) const
{
    if (IsStandard() || format.IsStandard())
        return (format.m_type == m_type);
    else
        return (m_id == format.m_id);
}

//-------------------------------------------------------------------------
// wxDataObject
//-------------------------------------------------------------------------

wxDataObject::wxDataObject()
{
}

bool wxDataObject::IsSupportedFormat( const wxDataFormat& rFormat, Direction vDir ) const
{
    size_t nFormatCount = GetFormatCount( vDir );
    bool found = false;

    if (nFormatCount == 1)
    {
        found = (rFormat == GetPreferredFormat());
    }
    else
    {
        wxDataFormat *pFormats = new wxDataFormat[nFormatCount];
        GetAllFormats( pFormats, vDir );

        for (size_t n = 0; n < nFormatCount; n++)
        {
            if (pFormats[n] == rFormat)
            {
                found = true;
                break;
            }
        }

        delete [] pFormats;
    }

    return found;
}

// ----------------------------------------------------------------------------
// wxTextDataObject
// ----------------------------------------------------------------------------

#if wxUSE_UNICODE
void wxTextDataObject::GetAllFormats( wxDataFormat *formats, wxDataObjectBase::Direction dir ) const
{
    *formats++ = wxDataFormat( wxDF_TEXT );
    *formats = wxDataFormat( wxDF_UNICODETEXT );
}
#endif

// ----------------------------------------------------------------------------
// wxFileDataObject
// ----------------------------------------------------------------------------

void wxFileDataObject::GetFileNames( wxCharBuffer &buf ) const
{
    wxString filenames;

    for (size_t i = 0; i < m_filenames.GetCount(); i++)
    {
        filenames += m_filenames[i];
        filenames += wxT('\n');
    }

    buf = filenames.fn_str();
}

bool wxFileDataObject::GetDataHere( void *pBuf ) const
{
    if (pBuf == NULL)
        return false;

    wxCharBuffer buf;
    size_t buffLength;

    GetFileNames( buf );
    buffLength = strlen( buf );
    memcpy( pBuf, (const char*)buf, buffLength + 1 );

    return true;
}

size_t wxFileDataObject::GetDataSize() const
{
    wxCharBuffer buf;
    size_t buffLength;

    GetFileNames( buf );
    buffLength = strlen( buf );

    return buffLength + 1;
}

bool wxFileDataObject::SetData( size_t nSize, const void *pBuf )
{
    wxString filenames;

#if wxUSE_UNICODE
    filenames = wxString( (const char*)pBuf, *wxConvFileName );
#else
    filenames = wxString (wxConvLocal.cWC2WX(wxConvFileName->cMB2WC( (const char*)pBuf)));
#endif

    m_filenames = wxStringTokenize( filenames, wxT("\n"), wxTOKEN_STRTOK );

    return true;
}

void wxFileDataObject::AddFile( const wxString& rFilename )
{
    m_filenames.Add( rFilename );
}

// ----------------------------------------------------------------------------
// wxBitmapDataObject
// ----------------------------------------------------------------------------

wxBitmapDataObject::wxBitmapDataObject()
{
    Init();
}

wxBitmapDataObject::wxBitmapDataObject( const wxBitmap& rBitmap )
: wxBitmapDataObjectBase( rBitmap )
{
    Init();

    if (m_bitmap.Ok())
    {
#if wxMAC_USE_CORE_GRAPHICS
		SetBitmap( rBitmap );
#else
        m_pictHandle = m_bitmap.GetBitmapData()->GetPictHandle();
        m_pictCreated = false;
#endif
    }
}

wxBitmapDataObject::~wxBitmapDataObject()
{
    Clear();
}

void wxBitmapDataObject::SetBitmap( const wxBitmap& rBitmap )
{
    Clear();
    wxBitmapDataObjectBase::SetBitmap( rBitmap );
    if (m_bitmap.Ok())
    {
#if wxMAC_USE_CORE_GRAPHICS
        CGImageRef cgImageRef = (CGImageRef) m_bitmap.CGImageCreate();
#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_4
        if ( UMAGetSystemVersion() >= 0x1040 )
        {
            CFMutableDataRef data = CFDataCreateMutable(kCFAllocatorDefault, 0);
            CGImageDestinationRef destination = CGImageDestinationCreateWithData( data , kUTTypeTIFF , 1 , NULL );
            if ( destination )
            {
                CGImageDestinationAddImage( destination, cgImageRef, NULL );
                CGImageDestinationFinalize( destination );
                CFRelease( destination );
            }
            m_pictHandle = NewHandle(CFDataGetLength(data));
            if ( m_pictHandle )
            {
                memcpy( *(Handle)m_pictHandle, (const char *)CFDataGetBytePtr(data), CFDataGetLength(data) );
            }
            CFRelease( data );
        }
        else
#endif
#ifndef __LP64__
        {
            // export as TIFF
            GraphicsExportComponent exporter = 0;
            OSStatus err = OpenADefaultComponent(GraphicsExporterComponentType, kQTFileTypeTIFF, &exporter);
            if (noErr == err)
            {
                m_pictHandle = NewHandle(0);
                if ( m_pictHandle )
                {
                    err = GraphicsExportSetInputCGImage( exporter, cgImageRef);
                    err = GraphicsExportSetOutputHandle(exporter, (Handle)m_pictHandle);
                    err = GraphicsExportDoExport(exporter, NULL);
                }
                CloseComponent( exporter );
            }
        }
#endif
        CGImageRelease(cgImageRef);
#else
        m_pictHandle = m_bitmap.GetBitmapData()->GetPictHandle();
        m_pictCreated = false;
#endif
    }
}

void wxBitmapDataObject::Init()
{
    m_pictHandle = NULL;
    m_pictCreated = false;
}

void wxBitmapDataObject::Clear()
{
    if (m_pictHandle != NULL)
    {
#if wxMAC_USE_CORE_GRAPHICS
        DisposeHandle( (Handle) m_pictHandle );
#else
        if (m_pictCreated)
            KillPicture( (PicHandle)m_pictHandle );
#endif
        m_pictHandle = NULL;
    }
    m_pictCreated = false;
}

bool wxBitmapDataObject::GetDataHere( void *pBuf ) const
{
    if (m_pictHandle == NULL)
    {
        wxFAIL_MSG( wxT("attempt to copy empty bitmap failed") );
        return false;
    }

    if (pBuf == NULL)
        return false;

    memcpy( pBuf, *(Handle)m_pictHandle, GetHandleSize( (Handle)m_pictHandle ) );

    return true;
}

size_t wxBitmapDataObject::GetDataSize() const
{
    if (m_pictHandle != NULL)
        return GetHandleSize( (Handle)m_pictHandle );
    else
        return 0;
}

Handle MacCreateDataReferenceHandle(Handle theDataHandle)
{
    Handle  dataRef = NULL;
    OSErr   err     = noErr;

    // Create a data reference handle for our data.
    err = PtrToHand( &theDataHandle, &dataRef, sizeof(Handle));

    return dataRef;
}

bool wxBitmapDataObject::SetData( size_t nSize, const void *pBuf )
{
    Clear();

    if ((pBuf == NULL) || (nSize == 0))
        return false;

#if wxMAC_USE_CORE_GRAPHICS
    Handle picHandle = NewHandle( nSize );
    memcpy( *picHandle, pBuf, nSize );
    m_pictHandle = picHandle;
    CGImageRef cgImageRef = 0;
#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_4
    if ( UMAGetSystemVersion() >= 0x1040 )
    {
        CFDataRef data = CFDataCreateWithBytesNoCopy( kCFAllocatorDefault, (const UInt8*) pBuf, nSize, kCFAllocatorNull);
        CGImageSourceRef source = CGImageSourceCreateWithData( data, NULL );
        if ( source )
        {
            cgImageRef = CGImageSourceCreateImageAtIndex(source, 0, NULL);
        }
        CFRelease( source );
        CFRelease( data );
    }
    else
#endif
#ifndef __LP64__
    {
        // import from TIFF
        GraphicsImportComponent importer = 0;
        OSStatus err = OpenADefaultComponent(GraphicsImporterComponentType, kQTFileTypeTIFF, &importer);
        if (noErr == err)
        {
            if ( picHandle )
            {
                ComponentResult result = GraphicsImportSetDataHandle(importer, picHandle);
                if ( result == noErr )
                {
                    Rect frame;
                    GraphicsImportGetNaturalBounds( importer, &frame );
                    GraphicsImportCreateCGImage( importer, &cgImageRef, kGraphicsImportCreateCGImageUsingCurrentSettings );
                }
            }
            CloseComponent( importer );
        }
    }
#endif
    if ( cgImageRef )
    {
        m_bitmap.Create( CGImageGetWidth(cgImageRef)  , CGImageGetHeight(cgImageRef) );
        CGRect r = CGRectMake( 0 , 0 , CGImageGetWidth(cgImageRef)  , CGImageGetHeight(cgImageRef) );
        CGContextDrawImage( (CGContextRef) m_bitmap.GetHBITMAP() , r, cgImageRef ) ;
        CGImageRelease(cgImageRef);
        cgImageRef = NULL;
    }
#else
    PicHandle picHandle = (PicHandle)NewHandle( nSize );
    memcpy( *picHandle, pBuf, nSize );
    m_pictHandle = picHandle;
    // ownership is transferred to the bitmap
    m_pictCreated = false;
#ifndef __LP64__
    Rect frame;
    wxMacGetPictureBounds( picHandle, &frame );
#if wxUSE_METAFILE
    wxMetafile mf;
    mf.SetHMETAFILE( (WXHMETAFILE)m_pictHandle );
#endif
    wxMemoryDC mdc;
    m_bitmap.Create( frame.right - frame.left, frame.bottom - frame.top );
    mdc.SelectObject( m_bitmap );
#if wxUSE_METAFILE  
    mf.Play( &mdc );
#endif
    mdc.SelectObject( wxNullBitmap );
#endif
#endif

    return m_bitmap.Ok();
}

#endif
