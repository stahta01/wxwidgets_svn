///////////////////////////////////////////////////////////////////////////////
// Name:        gtk/dataobj2.h
// Purpose:     declaration of standard wxDataObjectSimple-derived classes
// Author:      Robert Roebling
// Created:     19.10.99 (extracted from gtk/dataobj.h)
// RCS-ID:      $Id$
// Copyright:   (c) 1998, 1999 Vadim Zeitlin, Robert Roebling
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_GTK_DATAOBJ2_H_
#define _WX_GTK_DATAOBJ2_H_

// ----------------------------------------------------------------------------
// wxBitmapDataObject is a specialization of wxDataObject for bitmaps
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxBitmapDataObject : public wxBitmapDataObjectBase
{
public:
    // ctors
    wxBitmapDataObject();
    wxBitmapDataObject(const wxBitmap& bitmap);

    // destr
    virtual ~wxBitmapDataObject();

    // override base class virtual to update PNG data too
    virtual void SetBitmap(const wxBitmap& bitmap);

    // implement base class pure virtuals
    // ----------------------------------

    virtual size_t GetDataSize() const { return m_pngSize; }
    virtual bool GetDataHere(void *buf) const;
    virtual bool SetData(size_t len, const void *buf);
    // Must provide overloads to avoid hiding them (and warnings about it)
    virtual size_t GetDataSize(const wxDataFormat&) const
    {
        return GetDataSize();
    }
    virtual bool GetDataHere(const wxDataFormat&, void *buf) const
    {
        return GetDataHere(buf);
    }
    virtual bool SetData(const wxDataFormat&, size_t len, const void *buf)
    {
        return SetData(len, buf);
    }

protected:
    void Init() { m_pngData = NULL; m_pngSize = 0; }
    void Clear() { free(m_pngData); }
    void ClearAll() { Clear(); Init(); }

    size_t      m_pngSize;
    void       *m_pngData;

    void DoConvertToPng();
};

// ----------------------------------------------------------------------------
// wxFileDataObject is a specialization of wxDataObject for file names
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxFileDataObject : public wxFileDataObjectBase
{
public:
    // implement base class pure virtuals
    // ----------------------------------

    void AddFile( const wxString &filename );

    virtual size_t GetDataSize() const;
    virtual bool GetDataHere(void *buf) const;
    virtual bool SetData(size_t len, const void *buf);
    // Must provide overloads to avoid hiding them (and warnings about it)
    virtual size_t GetDataSize(const wxDataFormat&) const
    {
        return GetDataSize();
    }
    virtual bool GetDataHere(const wxDataFormat&, void *buf) const
    {
        return GetDataHere(buf);
    }
    virtual bool SetData(const wxDataFormat&, size_t len, const void *buf)
    {
        return SetData(len, buf);
    }
};

// ----------------------------------------------------------------------------
// wxURLDataObject is a specialization of wxDataObject for URLs
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxURLDataObject : public wxDataObjectSimple
{
public:
    wxURLDataObject(const wxString& url = wxEmptyString);

    wxString GetURL() const { return m_url; }
    void SetURL(const wxString& url) { m_url = url; }

    virtual size_t GetDataSize() const;
    virtual bool GetDataHere(void *buf) const;
    virtual bool SetData(size_t len, const void *buf);

    // Must provide overloads to avoid hiding them (and warnings about it)
    virtual size_t GetDataSize(const wxDataFormat&) const
    {
        return GetDataSize();
    }
    virtual bool GetDataHere(const wxDataFormat&, void *buf) const
    {
        return GetDataHere(buf);
    }
    virtual bool SetData(const wxDataFormat&, size_t len, const void *buf)
    {
        return SetData(len, buf);
    }

private:
    wxString m_url;
};


#endif // _WX_GTK_DATAOBJ2_H_

