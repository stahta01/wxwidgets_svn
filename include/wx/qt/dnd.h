///////////////////////////////////////////////////////////////////////////////
// Name:        dnd.h
// Purpose:     declaration of the wxDropTarget class
// Author:      Robert Roebling
// RCS-ID:      
// Copyright:   (c) 1998 Vadim Zeitlin, Robert Roebling
// Licence:     wxWindows license
///////////////////////////////////////////////////////////////////////////////


#ifndef __GTKDNDH__
#define __GTKDNDH__

#ifdef __GNUG__
#pragma interface
#endif

#include "wx/defs.h"
#include "wx/object.h"
#include "wx/string.h"
#include "wx/cursor.h"

//-------------------------------------------------------------------------
// classes
//-------------------------------------------------------------------------

class wxWindow;

class wxDataObject;
class wxTextDataObject;
class wxFileDataObject;

class wxDropTarget;
class wxTextDropTarget;
class wxFileDropTarget;

class wxDropSource;

//-------------------------------------------------------------------------
// wxDataObject
//-------------------------------------------------------------------------

class wxDataObject: public wxObject
{
public:
  // all data formats (values are the same as in windows.h, do not change!)
  enum StdFormat
  {
    Invalid,
    Text,
    Bitmap,
    MetafilePict,
    Sylk,
    Dif,
    Tiff,
    OemText,
    Dib,
    Palette,
    Pendata,
    Riff,
    Wave,
    UnicodeText,
    EnhMetafile,
    Hdrop,
    Locale,
    Max
  };

  // function to return symbolic name of clipboard format (debug messages)
  static const char *GetFormatName(wxDataFormat format);

  // ctor & dtor
  wxDataObject() {};
  ~wxDataObject() {};

  // pure virtuals to override
    // get the best suited format for our data
  virtual wxDataFormat GetPreferredFormat() const = 0;
    // decide if we support this format (should be one of values of
    // StdFormat enumerations or a user-defined format)
  virtual bool IsSupportedFormat(wxDataFormat format) const = 0;
    // get the (total) size of data
  virtual uint GetDataSize() const = 0;
    // copy raw data to provided pointer
  virtual void GetDataHere(void *pBuf) const = 0;

};

// ----------------------------------------------------------------------------
// wxTextDataObject is a specialization of wxDataObject for text data
// ----------------------------------------------------------------------------

class wxTextDataObject : public wxDataObject
{
public:
  // ctors
  wxTextDataObject() { }
  wxTextDataObject(const wxString& strText) : m_strText(strText) { }
  void Init(const wxString& strText) { m_strText = strText; }

  // implement base class pure virtuals
  virtual wxDataFormat GetPreferredFormat() const
    { return wxDF_TEXT; }
  virtual bool IsSupportedFormat(wxDataFormat format) const
    { return format == wxDF_TEXT; }
  virtual uint GetDataSize() const
    { return m_strText.Len() + 1; } // +1 for trailing '\0'of course
  virtual void GetDataHere(void *pBuf) const
    { memcpy(pBuf, m_strText.c_str(), GetDataSize()); }

private:
  wxString  m_strText;
  
};

// ----------------------------------------------------------------------------
// wxFileDataObject is a specialization of wxDataObject for file names
// ----------------------------------------------------------------------------

class wxFileDataObject : public wxDataObject
{
public:

  wxFileDataObject(void) { }
  void AddFile( const wxString &file )
    { m_files += file; m_files += ";"; }

  // implement base class pure virtuals
  virtual wxDataFormat GetPreferredFormat() const
    { return wxDF_FILENAME; }
  virtual bool IsSupportedFormat(wxDataFormat format) const
    { return format == wxDF_FILENAME; }
  virtual uint GetDataSize() const
    { return m_files.Len() + 1; } // +1 for trailing '\0'of course
  virtual void GetDataHere(void *pBuf) const
    { memcpy(pBuf, m_files.c_str(), GetDataSize()); }

private:
  wxString  m_files;
  
};
//-------------------------------------------------------------------------
// wxDropTarget
//-------------------------------------------------------------------------

class wxDropTarget: public wxObject
{
  public:

    wxDropTarget();
    ~wxDropTarget();
    
    virtual void OnEnter() { }
    virtual void OnLeave() { }
    virtual bool OnDrop( long x, long y, const void *pData ) = 0;

//  protected:
      
    friend wxWindow;
    
    // Override these to indicate what kind of data you support: 
  
    virtual size_t GetFormatCount() const = 0;
    virtual wxDataFormat GetFormat(size_t n) const = 0;
};

//-------------------------------------------------------------------------
// wxTextDropTarget
//-------------------------------------------------------------------------

class wxTextDropTarget: public wxDropTarget
{
  public:

    wxTextDropTarget() {};
    virtual bool OnDrop( long x, long y, const void *pData );
    virtual bool OnDropText( long x, long y, const char *psz );
    
  protected:
  
    virtual size_t GetFormatCount() const;
    virtual wxDataFormat GetFormat(size_t n) const;
};

// ----------------------------------------------------------------------------
// A drop target which accepts files (dragged from File Manager or Explorer)
// ----------------------------------------------------------------------------

class wxFileDropTarget: public wxDropTarget
{
  public:
    
    wxFileDropTarget() {};
    
    virtual bool OnDrop(long x, long y, const void *pData);
    virtual bool OnDropFiles( long x, long y, 
                              size_t nFiles, const char * const aszFiles[]);

  protected:
  
    virtual size_t GetFormatCount() const;
    virtual wxDataFormat GetFormat(size_t n) const;
};

//-------------------------------------------------------------------------
// wxDropSource
//-------------------------------------------------------------------------

class wxDropSource: public wxObject
{
  public:

  enum DragResult
  {
    Error,    // error prevented the d&d operation from completing
    None,     // drag target didn't accept the data
    Copy,     // the data was successfully copied
    Move,     // the data was successfully moved
    Cancel    // the operation was cancelled by user (not an error)
  };

    wxDropSource( wxWindow *win );
    wxDropSource( wxDataObject &data, wxWindow *win );
    
    ~wxDropSource(void);
    
    void SetData( wxDataObject &data  );
    DragResult DoDragDrop( bool bAllowMove = FALSE );
    
    virtual bool GiveFeedback( DragResult WXUNUSED(effect), bool WXUNUSED(bScrolling) ) { return TRUE; };

  protected:
  
    wxDataObject  *m_data;
};

#endif  
       //__GTKDNDH__

