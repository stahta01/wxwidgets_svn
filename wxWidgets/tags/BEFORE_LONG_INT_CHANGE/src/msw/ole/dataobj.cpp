///////////////////////////////////////////////////////////////////////////////
// Name:        msw/ole/dataobj.cpp
// Purpose:     implementation of wx[I]DataObject class
// Author:      Vadim Zeitlin
// Modified by:
// Created:     10.05.98
// RCS-ID:      $Id$
// Copyright:   (c) 1998 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
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

#if defined(__BORLANDC__)
  #pragma hdrstop
#endif
#ifndef WX_PRECOMP
#include "wx/intl.h"
#endif
#include "wx/defs.h"

#if defined(__WIN32__) && !defined(__GNUWIN32__) || defined(wxUSE_NORLANDER_HEADERS)

#include "wx/log.h"
#include "wx/dataobj.h"

#include <windows.h>
#ifdef wxUSE_NORLANDER_HEADERS
  #include <ole2.h>
#endif
#include <oleauto.h>

#ifndef __WIN32__  
  #include <ole2.h>
  #include <olestd.h>
#endif

#include  "wx/msw/ole/oleutils.h"

// ----------------------------------------------------------------------------
// functions
// ----------------------------------------------------------------------------

static const char *GetTymedName(DWORD tymed);

// ----------------------------------------------------------------------------
// wxIEnumFORMATETC interface implementation
// ----------------------------------------------------------------------------
class wxIEnumFORMATETC : public IEnumFORMATETC
{
public:
  wxIEnumFORMATETC(CLIPFORMAT cf);

  DECLARE_IUNKNOWN_METHODS;

  // IEnumFORMATETC
  STDMETHODIMP Next(ULONG celt, FORMATETC *rgelt, ULONG *pceltFetched);
  STDMETHODIMP Skip(ULONG celt);
  STDMETHODIMP Reset();
  STDMETHODIMP Clone(IEnumFORMATETC **ppenum);

private:
  FORMATETC   m_format;   // (unique @@@) format we can provide data in
  ULONG       m_nCurrent; // current enum position (currently either 0 or 1)
};

// ----------------------------------------------------------------------------
// wxIDataObject implementation of IDataObject interface
// ----------------------------------------------------------------------------
class wxIDataObject : public IDataObject
{
public:
  wxIDataObject(wxDataObject *pDataObject);

  DECLARE_IUNKNOWN_METHODS;

  // IDataObject
  STDMETHODIMP GetData(FORMATETC *pformatetcIn, STGMEDIUM *pmedium);
  STDMETHODIMP GetDataHere(FORMATETC *pformatetc, STGMEDIUM *pmedium);
  STDMETHODIMP QueryGetData(FORMATETC *pformatetc);
  STDMETHODIMP GetCanonicalFormatEtc(FORMATETC *In, FORMATETC *pOut);
  STDMETHODIMP SetData(FORMATETC *pfetc, STGMEDIUM *pmedium, BOOL fRelease);
  STDMETHODIMP EnumFormatEtc(DWORD dwDirection, IEnumFORMATETC **ppenumFEtc);
  STDMETHODIMP DAdvise(FORMATETC *pfetc, DWORD ad, IAdviseSink *p, DWORD *pdw);
  STDMETHODIMP DUnadvise(DWORD dwConnection);
  STDMETHODIMP EnumDAdvise(IEnumSTATDATA **ppenumAdvise);

private:
  wxDataObject *m_pDataObject;      // pointer to C++ class we belong to
};

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxDataFormat
// ----------------------------------------------------------------------------

void wxDataFormat::SetId(const wxChar *format)
{
    m_format = ::RegisterClipboardFormat(format);
    if ( !m_format )
    {
        wxLogError(_("Couldn't register clipboard format '%s'."), format);
    }
}

wxString wxDataFormat::GetId() const
{
    static const int max = 256;

    wxString s;

    wxCHECK_MSG( !IsStandard(), s,
                 wxT("name of predefined format cannot be retrieved") );

    int len = ::GetClipboardFormatName(m_format, s.GetWriteBuf(max), max);
    s.UngetWriteBuf();

    if ( !len )
    {
        wxLogError(_("The clipboard format '%d' doesn't exist."), m_format);
    }

    return s;
}

// ----------------------------------------------------------------------------
// wxIEnumFORMATETC
// ----------------------------------------------------------------------------

BEGIN_IID_TABLE(wxIEnumFORMATETC)
  ADD_IID(Unknown)
  ADD_IID(EnumFORMATETC)
END_IID_TABLE;

IMPLEMENT_IUNKNOWN_METHODS(wxIEnumFORMATETC)

wxIEnumFORMATETC::wxIEnumFORMATETC(CLIPFORMAT cf)
{
  m_format.cfFormat = cf;
  m_format.ptd      = NULL;
  m_format.dwAspect = DVASPECT_CONTENT;
  m_format.lindex   = -1;
  m_format.tymed    = TYMED_HGLOBAL;
  m_cRef = 0;
  m_nCurrent = 0;
}

STDMETHODIMP wxIEnumFORMATETC::Next(ULONG      celt,
                                    FORMATETC *rgelt,
                                    ULONG     *pceltFetched)
{
  wxLogTrace(wxT("wxIEnumFORMATETC::Next"));

  if ( celt > 1 )
    return S_FALSE;

  if ( m_nCurrent == 0 ) {
    *rgelt = m_format;
    m_nCurrent++;

    return S_OK;
  }
  else
    return S_FALSE;
}

STDMETHODIMP wxIEnumFORMATETC::Skip(ULONG celt)
{
  wxLogTrace(wxT("wxIEnumFORMATETC::Skip"));

  if ( m_nCurrent == 0 )
    m_nCurrent++;

  return S_FALSE;
}

STDMETHODIMP wxIEnumFORMATETC::Reset()
{
  wxLogTrace(wxT("wxIEnumFORMATETC::Reset"));

  m_nCurrent = 0;

  return S_OK;
}

STDMETHODIMP wxIEnumFORMATETC::Clone(IEnumFORMATETC **ppenum)
{
  wxLogTrace(wxT("wxIEnumFORMATETC::Clone"));

  wxIEnumFORMATETC *pNew = new wxIEnumFORMATETC(m_format.cfFormat);
  pNew->AddRef();
  *ppenum = pNew;

  return S_OK;
}

// ----------------------------------------------------------------------------
// wxIDataObject
// ----------------------------------------------------------------------------

BEGIN_IID_TABLE(wxIDataObject)
  ADD_IID(Unknown)
  ADD_IID(DataObject)
END_IID_TABLE;

IMPLEMENT_IUNKNOWN_METHODS(wxIDataObject)

wxIDataObject::wxIDataObject(wxDataObject *pDataObject)
{
  m_cRef = 0;
  m_pDataObject = pDataObject;
}

// get data functions
STDMETHODIMP wxIDataObject::GetData(FORMATETC *pformatetcIn, STGMEDIUM *pmedium)
{
  wxLogTrace(wxT("wxIDataObject::GetData"));

  // is data is in our format?
  HRESULT hr = QueryGetData(pformatetcIn);
  if ( FAILED(hr) )
    return hr;

  // alloc memory
  HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE | GMEM_SHARE,
                                m_pDataObject->GetDataSize());
  if ( hGlobal == NULL ) {
    wxLogLastError("GlobalAlloc");
    return E_OUTOFMEMORY;
  }

  // copy data
  pmedium->tymed          = TYMED_HGLOBAL;
  pmedium->hGlobal        = hGlobal;
  pmedium->pUnkForRelease = NULL;

  hr = GetDataHere(pformatetcIn, pmedium);
  if ( FAILED(hr) ) {
    GlobalFree(hGlobal);
    return hr;
  }

  return S_OK;
}

STDMETHODIMP wxIDataObject::GetDataHere(FORMATETC *pformatetc,
                                        STGMEDIUM *pmedium)
{
  wxLogTrace(wxT("wxIDataObject::GetDataHere"));

  // put data in caller provided medium
  if ( pmedium->tymed != TYMED_HGLOBAL )
    return DV_E_TYMED;

  // copy data
  void *pBuf = GlobalLock(pmedium->hGlobal);
  if ( pBuf == NULL ) {
    wxLogLastError(wxT("GlobalLock"));
    return E_OUTOFMEMORY;
  }

  m_pDataObject->GetDataHere(pBuf);

  GlobalUnlock(pmedium->hGlobal);

  return S_OK;
}

// set data functions (not implemented)
STDMETHODIMP wxIDataObject::SetData(FORMATETC *pformatetc,
                                    STGMEDIUM *pmedium,
                                    BOOL       fRelease)
{
  wxLogTrace(wxT("wxIDataObject::SetData"));
  return E_NOTIMPL;
}

// information functions
STDMETHODIMP wxIDataObject::QueryGetData(FORMATETC *pformatetc)
{
  // do we accept data in this format?
  if ( pformatetc == NULL ) {
    wxLogTrace(wxT("wxIDataObject::QueryGetData: invalid ptr."));
    return E_INVALIDARG;
  }

  // the only one allowed by current COM implementation
  if ( pformatetc->lindex != -1 ) {
    wxLogTrace(wxT("wxIDataObject::QueryGetData: bad lindex %d"),
               pformatetc->lindex);
    return DV_E_LINDEX;
  }

  // we don't support anything other (THUMBNAIL, ICON, DOCPRINT...)
  if ( pformatetc->dwAspect != DVASPECT_CONTENT ) {
    wxLogTrace(wxT("wxIDataObject::QueryGetData: bad dwAspect %d"),
               pformatetc->dwAspect);
    return DV_E_DVASPECT;
  }

  // @@ we only transfer data by global memory (bad for large amounts of it!)
  if ( !(pformatetc->tymed & TYMED_HGLOBAL) ) {
    wxLogTrace(wxT("wxIDataObject::QueryGetData: %s != TYMED_HGLOBAL."),
               GetTymedName(pformatetc->tymed));
    return DV_E_TYMED;
  }

  // and now check the type of data requested
  if ( m_pDataObject->IsSupportedFormat((wxDataFormatId)pformatetc->cfFormat) ) {
    wxLogTrace(wxT("wxIDataObject::QueryGetData: %s ok"),
               wxDataObject::GetFormatName((wxDataFormatId)pformatetc->cfFormat));
    return S_OK;
  }
  else {
    wxLogTrace(wxT("wxIDataObject::QueryGetData: %s unsupported"),
               wxDataObject::GetFormatName((wxDataFormatId)pformatetc->cfFormat));
    return DV_E_FORMATETC;
  }
}

STDMETHODIMP wxIDataObject::GetCanonicalFormatEtc(FORMATETC *pFormatetcIn,
                                                  FORMATETC *pFormatetcOut)
{
  wxLogTrace(wxT("wxIDataObject::GetCanonicalFormatEtc"));

  // @@ implementation is trivial, we might want something better here
  if ( pFormatetcOut != NULL )
    pFormatetcOut->ptd = NULL;
  return DATA_S_SAMEFORMATETC;
}

STDMETHODIMP wxIDataObject::EnumFormatEtc(DWORD dwDirection,
                                          IEnumFORMATETC **ppenumFormatEtc)
{
  wxLogTrace(wxT("wxIDataObject::EnumFormatEtc"));

  if ( dwDirection == DATADIR_SET ) {
    // we don't allow setting of data anyhow
    return E_NOTIMPL;
  }

  wxIEnumFORMATETC *pEnum =
    new wxIEnumFORMATETC(m_pDataObject->GetPreferredFormat());
  pEnum->AddRef();
  *ppenumFormatEtc = pEnum;

  return S_OK;
}

// advise sink functions (not implemented)
STDMETHODIMP wxIDataObject::DAdvise(FORMATETC   *pformatetc,
                                    DWORD        advf,
                                    IAdviseSink *pAdvSink,
                                    DWORD       *pdwConnection)
{
  return OLE_E_ADVISENOTSUPPORTED;
}

STDMETHODIMP wxIDataObject::DUnadvise(DWORD dwConnection)
{
  return OLE_E_ADVISENOTSUPPORTED;
}

STDMETHODIMP wxIDataObject::EnumDAdvise(IEnumSTATDATA **ppenumAdvise)
{
  return OLE_E_ADVISENOTSUPPORTED;
}

// ----------------------------------------------------------------------------
// wxDataObject
// ----------------------------------------------------------------------------

wxDataObject::wxDataObject()
{
  m_pIDataObject = new wxIDataObject(this);
  m_pIDataObject->AddRef();
}

wxDataObject::~wxDataObject()
{
  m_pIDataObject->Release();
}

const char *wxDataObject::GetFormatName(wxDataFormat format)
{
#ifdef __WXDEBUG__
  // case 'xxx' is not a valid value for switch of enum 'wxDataFormat'
  #ifdef __VISUALC__
    #pragma warning(disable:4063)
  #endif // VC++

  static char s_szBuf[128];
  switch ( format ) {
    case CF_TEXT:         return "CF_TEXT";
    case CF_BITMAP:       return "CF_BITMAP";
    case CF_METAFILEPICT: return "CF_METAFILEPICT";
    case CF_SYLK:         return "CF_SYLK";
    case CF_DIF:          return "CF_DIF";
    case CF_TIFF:         return "CF_TIFF";
    case CF_OEMTEXT:      return "CF_OEMTEXT";
    case CF_DIB:          return "CF_DIB";
    case CF_PALETTE:      return "CF_PALETTE";
    case CF_PENDATA:      return "CF_PENDATA";
    case CF_RIFF:         return "CF_RIFF";
    case CF_WAVE:         return "CF_WAVE";
    case CF_UNICODETEXT:  return "CF_UNICODETEXT";
    case CF_ENHMETAFILE:  return "CF_ENHMETAFILE";
    case CF_HDROP:        return "CF_HDROP";
    case CF_LOCALE:       return "CF_LOCALE";
    default:
      sprintf(s_szBuf, "clipboard format %d (unknown)", format);
      return s_szBuf;
  }

  #ifdef __VISUALC__
    #pragma warning(default:4063)
  #endif // VC++

#else // !Debug
  return "";
#endif // Debug
}

// ----------------------------------------------------------------------------
// wxPrivateDataObject
// ----------------------------------------------------------------------------

wxPrivateDataObject::wxPrivateDataObject()
{
    m_size = 0;
    m_data = NULL;
}

void wxPrivateDataObject::Free()
{
    if ( m_data )
        free(m_data);
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

// ----------------------------------------------------------------------------
// private functions
// ----------------------------------------------------------------------------
static const char *GetTymedName(DWORD tymed)
{
  static char s_szBuf[128];
  switch ( tymed ) {
    case TYMED_HGLOBAL:   return "TYMED_HGLOBAL";
    case TYMED_FILE:      return "TYMED_FILE";
    case TYMED_ISTREAM:   return "TYMED_ISTREAM";
    case TYMED_ISTORAGE:  return "TYMED_ISTORAGE";
    case TYMED_GDI:       return "TYMED_GDI";
    case TYMED_MFPICT:    return "TYMED_MFPICT";
    case TYMED_ENHMF:     return "TYMED_ENHMF";
    default:
      sprintf(s_szBuf, "type of media format %d (unknown)", tymed);
      return s_szBuf;
  }
}

// TODO: OLE parts of wxBitmap/File/MetafileDataObject

#endif

