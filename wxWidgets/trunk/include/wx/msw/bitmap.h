/////////////////////////////////////////////////////////////////////////////
// Name:        bitmap.h
// Purpose:     wxBitmap class
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_BITMAP_H_
#define _WX_BITMAP_H_

#ifdef __GNUG__
    #pragma interface "bitmap.h"
#endif

#include "wx/gdiobj.h"
#include "wx/gdicmn.h"
#include "wx/palette.h"

// Bitmap
class WXDLLEXPORT wxDC;
class WXDLLEXPORT wxControl;
class WXDLLEXPORT wxBitmap;
class WXDLLEXPORT wxBitmapHandler;
class WXDLLEXPORT wxIcon;
class WXDLLEXPORT wxCursor;
class WXDLLEXPORT wxControl;

// A mask is a mono bitmap used for drawing bitmaps
// transparently.
class WXDLLEXPORT wxMask: public wxObject
{
  DECLARE_DYNAMIC_CLASS(wxMask)

public:
  wxMask();

  // Construct a mask from a bitmap and a colour indicating
  // the transparent area
  wxMask(const wxBitmap& bitmap, const wxColour& colour);

  // Construct a mask from a bitmap and a palette index indicating
  // the transparent area
  wxMask(const wxBitmap& bitmap, int paletteIndex);

  // Construct a mask from a mono bitmap (copies the bitmap).
  wxMask(const wxBitmap& bitmap);

  ~wxMask();

  bool Create(const wxBitmap& bitmap, const wxColour& colour);
  bool Create(const wxBitmap& bitmap, int paletteIndex);
  bool Create(const wxBitmap& bitmap);

  // Implementation
  WXHBITMAP GetMaskBitmap() const { return m_maskBitmap; }
  void SetMaskBitmap(WXHBITMAP bmp) { m_maskBitmap = bmp; }

protected:
  WXHBITMAP m_maskBitmap;
};

class WXDLLEXPORT wxBitmapRefData: public wxGDIRefData
{
    friend class WXDLLEXPORT wxBitmap;
    friend class WXDLLEXPORT wxIcon;
    friend class WXDLLEXPORT wxCursor;
public:
    wxBitmapRefData();
    ~wxBitmapRefData();

public:
  int           m_width;
  int           m_height;
  int           m_depth;
  bool          m_ok;
  int           m_numColors;
  wxPalette     m_bitmapPalette;
  int           m_quality;

#ifdef __WXMSW__
  WXHBITMAP     m_hBitmap;
  wxDC *        m_selectedInto; // So bitmap knows whether it's been selected into
                      // a device context (for error checking)
  wxMask *      m_bitmapMask; // Option mask
#endif
};

#define M_BITMAPDATA ((wxBitmapRefData *)m_refData)

class WXDLLEXPORT wxBitmapHandler: public wxObject
{
  DECLARE_DYNAMIC_CLASS(wxBitmapHandler)
public:
  wxBitmapHandler() { m_name = ""; m_extension = ""; m_type = 0; };

  virtual bool Create(wxBitmap *bitmap, void *data, long flags, int width, int height, int depth = 1);
  virtual bool LoadFile(wxBitmap *bitmap, const wxString& name, long flags,
      int desiredWidth, int desiredHeight);
  virtual bool SaveFile(wxBitmap *bitmap, const wxString& name, int type, const wxPalette *palette = NULL);

  void SetName(const wxString& name) { m_name = name; }
  void SetExtension(const wxString& ext) { m_extension = ext; }
  void SetType(long type) { m_type = type; }
  wxString GetName() const { return m_name; }
  wxString GetExtension() const { return m_extension; }
  long GetType() const { return m_type; }
protected:
  wxString  m_name;
  wxString  m_extension;
  long      m_type;
};
#define M_BITMAPHANDLERDATA ((wxBitmapRefData *)bitmap->GetRefData())

class WXDLLEXPORT wxBitmap: public wxGDIObject
{
friend class WXDLLEXPORT wxBitmapHandler;

public:
    // default ctor creates an invalid bitmap, you must Create() it later
    wxBitmap() { Init(); }

    // Copy constructors
    wxBitmap(const wxBitmap& bitmap) { Init(); Ref(bitmap); }

    // Initialize with raw data
    wxBitmap(const char bits[], int width, int height, int depth = 1);

    // Initialize with XPM data
    wxBitmap(char **data, wxControl *anItem = NULL);

    // Load a file or resource
    wxBitmap(const wxString& name, long type = wxBITMAP_TYPE_BMP_RESOURCE);

    // New constructor for generalised creation from data
    wxBitmap(void *data, long type, int width, int height, int depth = 1);

    // If depth is omitted, will create a bitmap compatible with the display
    wxBitmap(int width, int height, int depth = -1);

    // we must have this, otherwise icons are silently copied into bitmaps using
    // the copy ctor but the resulting bitmap is invalid!
    wxBitmap(const wxIcon& icon) { Init(); CopyFromIcon(icon); }

    wxBitmap& operator=(const wxBitmap& bitmap)
    {
        if ( m_refData != bitmap.m_refData )
            Ref(bitmap);
        return *this;
    }

    wxBitmap& operator=(const wxIcon& icon)
    {
        (void)CopyFromIcon(icon);

        return *this;
    }

    virtual ~wxBitmap();

    // copies the contents and mask of the given (colour) icon to the bitmap
    bool CopyFromIcon(const wxIcon& icon);

    virtual bool Create(int width, int height, int depth = -1);
    virtual bool Create(void *data, long type, int width, int height, int depth = 1);
    virtual bool LoadFile(const wxString& name, long type = wxBITMAP_TYPE_BMP_RESOURCE);
    virtual bool SaveFile(const wxString& name, int type, const wxPalette *cmap = NULL);

    bool Ok() const { return (M_BITMAPDATA && M_BITMAPDATA->m_ok); }
    int GetWidth() const { return (M_BITMAPDATA ? M_BITMAPDATA->m_width : 0); }
    int GetHeight() const { return (M_BITMAPDATA ? M_BITMAPDATA->m_height : 0); }
    int GetDepth() const { return (M_BITMAPDATA ? M_BITMAPDATA->m_depth : 0); }
    int GetQuality() const { return (M_BITMAPDATA ? M_BITMAPDATA->m_quality : 0); }
    void SetWidth(int w);
    void SetHeight(int h);
    void SetDepth(int d);
    void SetQuality(int q);
    void SetOk(bool isOk);
#if WXWIN_COMPATIBILITY
    wxPalette *GetColourMap() const { return GetPalette(); }
    void SetColourMap(wxPalette *cmap) { SetPalette(*cmap); };
#endif
    wxPalette* GetPalette() const { return (M_BITMAPDATA ? (& M_BITMAPDATA->m_bitmapPalette) : (wxPalette*) NULL); }
    void SetPalette(const wxPalette& palette);

    wxMask *GetMask() const { return (M_BITMAPDATA ? M_BITMAPDATA->m_bitmapMask : (wxMask*) NULL); }
    void SetMask(wxMask *mask) ;

    bool operator==(const wxBitmap& bitmap) { return m_refData == bitmap.m_refData; }
    bool operator!=(const wxBitmap& bitmap) { return m_refData != bitmap.m_refData; }

    // Format handling
    static wxList& GetHandlers() { return sm_handlers; }
    static void AddHandler(wxBitmapHandler *handler);
    static void InsertHandler(wxBitmapHandler *handler);
    static bool RemoveHandler(const wxString& name);
    static wxBitmapHandler *FindHandler(const wxString& name);
    static wxBitmapHandler *FindHandler(const wxString& extension, long bitmapType);
    static wxBitmapHandler *FindHandler(long bitmapType);

    static void InitStandardHandlers();
    static void CleanUpHandlers();

protected:
    static wxList sm_handlers;

    // Implementation
public:
    void SetHBITMAP(WXHBITMAP bmp);
    WXHBITMAP GetHBITMAP() const { return (M_BITMAPDATA ? M_BITMAPDATA->m_hBitmap : 0); }
    void SetSelectedInto(wxDC *dc) { if (M_BITMAPDATA) M_BITMAPDATA->m_selectedInto = dc; }
    wxDC *GetSelectedInto() const { return (M_BITMAPDATA ? M_BITMAPDATA->m_selectedInto : (wxDC*) NULL); }
    bool FreeResource(bool force = FALSE);

    // Creates a bitmap that matches the device context's depth, from
    // an arbitray bitmap. At present, the original bitmap must have an
    // associated palette. (TODO: use a default palette if no palette exists.)
    // This function is necessary for you to Blit an arbitrary bitmap (which may have
    // the wrong depth). wxDC::SelectObject will compare the depth of the bitmap
    // with the DC's depth, and create a new bitmap if the depths differ.
    // Eventually we should perhaps make this a public API function so that
    // an app can efficiently produce bitmaps of the correct depth.
    // The Windows solution is to use SetDibBits to blit an arbotrary DIB directly to a DC, but
    // this is too Windows-specific, hence this solution of quietly converting the wxBitmap.
    // Contributed by Frederic Villeneuve <frederic.villeneuve@natinst.com>
    wxBitmap GetBitmapForDC(wxDC& dc) const;

protected:
    // common part of all ctors
    void Init();

private:
    DECLARE_DYNAMIC_CLASS(wxBitmap)
};

#endif
  // _WX_BITMAP_H_
