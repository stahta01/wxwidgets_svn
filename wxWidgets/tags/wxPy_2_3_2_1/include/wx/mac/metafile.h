/////////////////////////////////////////////////////////////////////////////
// Name:        metafile.h
// Purpose:     wxMetaFile, wxMetaFileDC classes.
//              This probably should be restricted to Windows platforms,
//              but if there is an equivalent on your platform, great.
// Author:      AUTHOR
// Modified by:
// Created:     ??/??/98
// RCS-ID:      $Id$
// Copyright:   (c) AUTHOR
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#ifndef _WX_METAFIILE_H_
#define _WX_METAFIILE_H_

#if wxUSE_METAFILE
#include "wx/dc.h"
#include "wx/gdiobj.h"

#if wxUSE_DATAOBJ
#include "wx/dataobj.h"
#endif

/*
 * Metafile and metafile device context classes
 *
 */

#define wxMetaFile wxMetafile
#define wxMetaFileDC wxMetafileDC

class WXDLLEXPORT wxMetafile;

class WXDLLEXPORT wxMetafileRefData: public wxGDIRefData
{
    friend class WXDLLEXPORT wxMetafile;
public:
    wxMetafileRefData(void);
    ~wxMetafileRefData(void);

public:
    PicHandle m_metafile;
};

#define M_METAFILEDATA ((wxMetafileRefData *)m_refData)

class WXDLLEXPORT wxMetafile: public wxGDIObject
{
  DECLARE_DYNAMIC_CLASS(wxMetafile)
 public:
  // Copy constructor
  inline wxMetafile(const wxMetafile& metafile)
  { Ref(metafile); }

  wxMetafile(const wxString& file = "");
  ~wxMetafile(void);

  // After this is called, the metafile cannot be used for anything
  // since it is now owned by the clipboard.
  virtual bool SetClipboard(int width = 0, int height = 0);

  virtual bool Play(wxDC *dc);
  inline bool Ok(void) const { return (M_METAFILEDATA && (M_METAFILEDATA->m_metafile != 0)); };

  // Implementation
  inline PicHandle GetHMETAFILE(void) { return M_METAFILEDATA->m_metafile; }
  void SetHMETAFILE(PicHandle mf) ;

  // Operators
  inline wxMetafile& operator = (const wxMetafile& metafile) { if (*this == metafile) return (*this); Ref(metafile); return *this; }
  inline bool operator == (const wxMetafile& metafile) { return m_refData == metafile.m_refData; }
  inline bool operator != (const wxMetafile& metafile) { return m_refData != metafile.m_refData; }

protected:
};

class WXDLLEXPORT wxMetafileDC: public wxDC
{
  DECLARE_DYNAMIC_CLASS(wxMetafileDC)

 public:
  // Don't supply origin and extent
  // Supply them to wxMakeMetaFilePlaceable instead.
  wxMetafileDC(const wxString& file = "");

  // Supply origin and extent (recommended).
  // Then don't need to supply them to wxMakeMetaFilePlaceable.
  wxMetafileDC(const wxString& file, int xext, int yext, int xorg, int yorg);

  ~wxMetafileDC(void);

  // Should be called at end of drawing
  virtual wxMetafile *Close(void);

  // Implementation
  inline wxMetafile *GetMetaFile(void) const { return m_metaFile; }
  inline void SetMetaFile(wxMetafile *mf) { m_metaFile = mf; }

protected:
  wxMetafile*   m_metaFile;
};

/*
 * Pass filename of existing non-placeable metafile, and bounding box.
 * Adds a placeable metafile header, sets the mapping mode to anisotropic,
 * and sets the window origin and extent to mimic the wxMM_TEXT mapping mode.
 *
 */

// No origin or extent
#define wxMakeMetaFilePlaceable wxMakeMetafilePlaceable
bool WXDLLEXPORT wxMakeMetafilePlaceable(const wxString& filename, float scale = 1.0);

// Optional origin and extent
bool WXDLLEXPORT wxMakeMetaFilePlaceable(const wxString& filename, int x1, int y1, int x2, int y2, float scale = 1.0, bool useOriginAndExtent = TRUE);

// ----------------------------------------------------------------------------
// wxMetafileDataObject is a specialization of wxDataObject for metafile data
// ----------------------------------------------------------------------------

#if wxUSE_DATAOBJ
class WXDLLEXPORT wxMetafileDataObject : public wxDataObject
{
public:
  // ctors
  wxMetafileDataObject() { m_width = 0; m_height = 0; };
  wxMetafileDataObject(const wxMetafile& metafile, int width = 0, int height = 0):
    m_metafile(metafile), m_width(width), m_height(height) { }

  void SetMetafile(const wxMetafile& metafile, int w = 0, int h = 0)
    { m_metafile = metafile; m_width = w; m_height = h; }
  wxMetafile GetMetafile() const { return m_metafile; }
  int GetWidth() const { return m_width; }
  int GetHeight() const { return m_height; }

  virtual wxDataFormat GetFormat() const { return wxDF_METAFILE; }

/* ??
  // implement base class pure virtuals
  virtual wxDataFormat GetPreferredFormat() const
    { return (wxDataFormat) wxDataObject::Text; }
  virtual bool IsSupportedFormat(wxDataFormat format) const
    { return format == wxDataObject::Text || format == wxDataObject::Locale; }
  virtual size_t GetDataSize() const
    { return m_strText.Len() + 1; } // +1 for trailing '\0'of course
  virtual void GetDataHere(void *pBuf) const
    { memcpy(pBuf, m_strText.c_str(), GetDataSize()); }
*/

private:
  wxMetafile   m_metafile;
  int          m_width;
  int          m_height;
};
#endif

#endif // wxUSE_METAFILE


#endif
    // _WX_METAFIILE_H_
