///////////////////////////////////////////////////////////////////////////////
// Name:        os2/dataobj.cpp
// Purpose:     implementation of wx[I]DataObject class
// Author:      David Webster
// Modified by:
// Created:     10/21/99
// RCS-ID:      $Id$
// Copyright:   (c) 1999 David Webster
// Licence:     wxWindows license
///////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#ifdef __GNUG__
  #pragma implementation "dataobj.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/intl.h"
#endif
#include "wx/defs.h"

#include "wx/log.h"
#include "wx/dataobj.h"
#include "wx/mstream.h"
#include "wx/image.h"
#include "wx/mac/private.h"

// ----------------------------------------------------------------------------
// functions
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// wxDataFormat
// ----------------------------------------------------------------------------

wxDataFormat::wxDataFormat()
{
    m_type = wxDF_INVALID;
    m_format = 0;
}

wxDataFormat::wxDataFormat(  wxDataFormatId   vType )
{
    SetType(vType);
}

wxDataFormat::wxDataFormat(  const wxChar*  zId)
{
    SetId(zId);
}

wxDataFormat::wxDataFormat(  const wxString&   rId)
{
    SetId(rId);
}

wxDataFormat::wxDataFormat( NativeFormat vFormat)
{
    SetId(vFormat);
}

void wxDataFormat::SetType(  wxDataFormatId  Type )
{
    m_type = Type;

    if (m_type == wxDF_TEXT)
        m_format = 'TEXT';
    else if (m_type == wxDF_BITMAP || m_type == wxDF_METAFILE )
        m_format = 'PICT';
    else if (m_type == wxDF_FILENAME)
        m_format = kDragFlavorTypeHFS ;
    else
    {
       wxFAIL_MSG( wxT("invalid dataformat") );
    }
}

wxDataFormatId wxDataFormat::GetType() const
{
    return m_type;
}

wxString wxDataFormat::GetId() const
{
    wxString sRet("");  // TODO: to name of ( m_format ) );
    return sRet;
}

void wxDataFormat::SetId(  NativeFormat  format )
{
    m_format = format;

    if (m_format == 'TEXT')
        m_type = wxDF_TEXT;
    else
    if (m_format == 'PICT')
        m_type = wxDF_BITMAP;
    else
    if (m_format == kDragFlavorTypeHFS )
        m_type = wxDF_FILENAME;
    else
        m_type = wxDF_PRIVATE;
}

void wxDataFormat::SetId( const wxChar* zId )
{
    wxString                        tmp(zId);

    m_type = wxDF_PRIVATE;
    m_format = 0;// TODO: get the format gdk_atom_intern( wxMBSTRINGCAST tmp.mbc_str(), FALSE );
}

//-------------------------------------------------------------------------
// wxDataObject
//-------------------------------------------------------------------------

wxDataObject::wxDataObject()
{
}

bool wxDataObject::IsSupportedFormat(
  const wxDataFormat&               rFormat
, Direction                         vDir
) const
{
    size_t                          nFormatCount = GetFormatCount(vDir);

    if (nFormatCount == 1)
    {
        return rFormat == GetPreferredFormat();
    }
    else
    {
        wxDataFormat*               pFormats = new wxDataFormat[nFormatCount];
        GetAllFormats( pFormats
                      ,vDir
                     );

        size_t                      n;

        for (n = 0; n < nFormatCount; n++)
        {
            if (pFormats[n] == rFormat)
                break;
        }

        delete [] pFormats;

        // found?
        return n < nFormatCount;
    }
}

// ----------------------------------------------------------------------------
// wxFileDataObject
// ----------------------------------------------------------------------------

bool wxFileDataObject::GetDataHere(
  void*                             pBuf
) const
{
    wxString                        sFilenames;

    for (size_t i = 0; i < m_filenames.GetCount(); i++)
    {
        sFilenames += m_filenames[i];
        sFilenames += (wxChar)0;
    }

    memcpy(pBuf, sFilenames.mbc_str(), sFilenames.Len() + 1);
    return TRUE;
}

size_t wxFileDataObject::GetDataSize() const
{
    size_t                          nRes = 0;

    for (size_t i = 0; i < m_filenames.GetCount(); i++)
    {
        nRes += m_filenames[i].Len();
        nRes += 1;
    }

    return nRes + 1;
}

bool wxFileDataObject::SetData(
  size_t                            WXUNUSED(nSize)
, const void*                       pBuf
)
{
    m_filenames.Empty();

    wxString                        sFile( (const char *)pBuf);  /* char, not wxChar */

    AddFile(sFile);

    return TRUE;
}

void wxFileDataObject::AddFile(
  const wxString&                   rFilename
)
{
    m_filenames.Add(rFilename);
}

// ----------------------------------------------------------------------------
// wxBitmapDataObject
// ----------------------------------------------------------------------------

wxBitmapDataObject::wxBitmapDataObject()
{
    Init();
}

wxBitmapDataObject::wxBitmapDataObject(
  const wxBitmap&                   rBitmap
)
: wxBitmapDataObjectBase(rBitmap)
{
    Init();

    DoConvertToPng();
}

wxBitmapDataObject::~wxBitmapDataObject()
{
    Clear();
}

void wxBitmapDataObject::SetBitmap(
  const wxBitmap&                   rBitmap
)
{
    ClearAll();
    wxBitmapDataObjectBase::SetBitmap(rBitmap);
    DoConvertToPng();
}

bool wxBitmapDataObject::GetDataHere(
  void*                             pBuf
) const
{
    if (!m_pngSize)
    {
        wxFAIL_MSG(wxT("attempt to copy empty bitmap failed"));
        return FALSE;
    }
    memcpy(pBuf, m_pngData, m_pngSize);
    return TRUE;
}

bool wxBitmapDataObject::SetData(
  size_t                            nSize
, const void*                       pBuf
)
{
    Clear();
    m_pngSize = nSize;
    m_pngData = malloc(m_pngSize);

    memcpy(m_pngData, pBuf, m_pngSize);

    wxMemoryInputStream             vMstream((char*)m_pngData, m_pngSize);
    wxImage                         vImage;
    wxPNGHandler                    vHandler;

    if (!vHandler.LoadFile(&vImage, vMstream))
    {
        return FALSE;
    }

    m_bitmap = wxBitmap( vImage ) ; 
    return m_bitmap.Ok();
}

void wxBitmapDataObject::DoConvertToPng()
{
    if (!m_bitmap.Ok())
        return;

    wxCHECK_RET( wxImage::FindHandler(wxBITMAP_TYPE_PNG) != NULL,
                 wxT("You must call wxImage::AddHandler(new wxPNGHandler); to be able to use clipboard with bitmaps!") );

    wxImage image = m_bitmap.ConvertToImage();

    wxCountingOutputStream count;
    image.SaveFile(count, wxBITMAP_TYPE_PNG);

    m_pngSize = count.GetSize() + 100; // sometimes the size seems to vary ???
    m_pngData = malloc(m_pngSize);

    wxMemoryOutputStream mstream((char*) m_pngData, m_pngSize);
    image.SaveFile(mstream, wxBITMAP_TYPE_PNG);
}

