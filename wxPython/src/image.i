/////////////////////////////////////////////////////////////////////////////
// Name:        image.i
// Purpose:     SWIG interface file for wxImage, wxImageHandler, etc.
//
// Author:      Robin Dunn
//
// Created:     28-Apr-1999
// RCS-ID:      $Id$
// Copyright:   (c) 1998 by Total Control Software
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////


%module image

%{
#include "helpers.h"
#include "pyistream.h"
#include <wx/image.h>
%}

//----------------------------------------------------------------------

%include typemaps.i
%include my_typemaps.i

// Import some definitions of other classes, etc.
%import _defs.i
%import misc.i
%import gdi.i
%import streams.i

//---------------------------------------------------------------------------

class wxImageHandler : public wxObject {
public:
    // wxImageHandler();    Abstract Base Class
    wxString GetName();
    wxString GetExtension();
    long GetType();
    wxString GetMimeType();

    //bool LoadFile(wxImage* image, wxInputStream& stream);
    //bool SaveFile(wxImage* image, wxOutputStream& stream);
    //virtual int GetImageCount( wxInputStream& stream );
    //bool CanRead( wxInputStream& stream );

    bool CanRead( const wxString& name );

    void SetName(const wxString& name);
    void SetExtension(const wxString& extension);
    void SetType(long type);
    void SetMimeType(const wxString& mimetype);
};

//---------------------------------------------------------------------------

class wxPNGHandler : public wxImageHandler {
public:
    wxPNGHandler();
};


class wxJPEGHandler : public wxImageHandler {
public:
    wxJPEGHandler();
};


class wxBMPHandler : public wxImageHandler {
public:
    wxBMPHandler();
};

class wxICOHandler : public wxBMPHandler {
public:
    wxICOHandler();
};

class wxCURHandler : public wxICOHandler {
public:
    wxCURHandler();
};

class wxANIHandler : public wxCURHandler {
public:
    wxANIHandler();
};

class wxGIFHandler : public wxImageHandler {
public:
    wxGIFHandler();
};

class wxPNMHandler : public wxImageHandler {
public:
    wxPNMHandler();
};

class wxPCXHandler : public wxImageHandler {
public:
    wxPCXHandler();
};

class wxTIFFHandler : public wxImageHandler {
public:
    wxTIFFHandler();
};



//---------------------------------------------------------------------------

class wxImage : public wxObject {
public:
    wxImage( const wxString& name, long type = wxBITMAP_TYPE_ANY, int index = -1 );
    ~wxImage();

    void Create( int width, int height );
    void Destroy();

    wxImage Scale( int width, int height );
    wxImage& Rescale(int width, int height);

    void SetRGB( int x, int y, unsigned char r, unsigned char g, unsigned char b );
    unsigned char GetRed( int x, int y );
    unsigned char GetGreen( int x, int y );
    unsigned char GetBlue( int x, int y );

    static bool CanRead( const wxString& name );
    static int GetImageCount( const wxString& name, long type = wxBITMAP_TYPE_ANY );

    bool LoadFile( const wxString& name, long type = wxBITMAP_TYPE_ANY, int index = -1 );
    %name(LoadMimeFile)bool LoadFile( const wxString& name, const wxString& mimetype, int index = -1 );

    bool SaveFile( const wxString& name, int type );
    %name(SaveMimeFile)bool SaveFile( const wxString& name, const wxString& mimetype );

    %name(CanReadStream) static bool CanRead( wxInputStream& stream );
    %name(LoadStream) bool LoadFile( wxInputStream& stream, long type = wxBITMAP_TYPE_ANY, int index = -1 );
    %name(LoadMimeStream) bool LoadFile( wxInputStream& stream, const wxString& mimetype, int index = -1 );

    bool Ok();
    int GetWidth();
    int GetHeight();

    wxImage GetSubImage(const wxRect& rect);
    wxImage Copy();
    void Paste( const wxImage &image, int x, int y );

    //unsigned char *GetData();
    //void SetData( unsigned char *data );

    %addmethods {
        PyObject* GetData() {
            unsigned char* data = self->GetData();
            int len = self->GetWidth() * self->GetHeight() * 3;
            return PyString_FromStringAndSize((char*)data, len);
        }

        void SetData(PyObject* data) {
            unsigned char* dataPtr;

            if (! PyString_Check(data)) {
                PyErr_SetString(PyExc_TypeError, "Expected string object");
                return /* NULL */ ;
            }

            size_t len = self->GetWidth() * self->GetHeight() * 3;
            dataPtr = (unsigned char*) malloc(len);
            memcpy(dataPtr, PyString_AsString(data), len);
            self->SetData(dataPtr);
        }
    }

    void SetMaskColour( unsigned char r, unsigned char g, unsigned char b );
    unsigned char GetMaskRed();
    unsigned char GetMaskGreen();
    unsigned char GetMaskBlue();
    void SetMask( bool mask = TRUE );
    bool HasMask();

    wxImage Rotate(double angle, const wxPoint & centre_of_rotation,
                   bool interpolating = TRUE, wxPoint * offset_after_rotation = NULL) const ;
    wxImage Rotate90( bool clockwise = TRUE ) ;
    wxImage Mirror( bool horizontally = TRUE ) ;

    void Replace( unsigned char r1, unsigned char g1, unsigned char b1,
                  unsigned char r2, unsigned char g2, unsigned char b2 );

    // convert to monochrome image (<r,g,b> will be replaced by white, everything else by black)
    wxImage ConvertToMono( unsigned char r, unsigned char g, unsigned char b ) const;

    void SetOption(const wxString& name, const wxString& value);
    %name(SetOptionInt)void SetOption(const wxString& name, int value);
    wxString GetOption(const wxString& name) const;
    int GetOptionInt(const wxString& name) const;
    bool HasOption(const wxString& name) const;

    unsigned long CountColours( unsigned long stopafter = (unsigned long) -1 );
    // TODO: unsigned long ComputeHistogram( wxHashTable &h );

    static void AddHandler( wxImageHandler *handler );
    static void InsertHandler( wxImageHandler *handler );
    static bool RemoveHandler( const wxString& name );


    %addmethods {
        wxBitmap ConvertToBitmap() {
            wxBitmap bitmap(*self);
            return bitmap;
        }

        wxBitmap ConvertToMonoBitmap( unsigned char red,
                                      unsigned char green,
                                      unsigned char blue ) {
            wxImage mono = self->ConvertToMono( red, green, blue );
            wxBitmap bitmap( mono, 1 );
            return bitmap;
        }
    }
};


// Alternate constructors
%new wxImage* wxEmptyImage(int width=0, int height=0);
%new wxImage* wxImageFromMime(const wxString& name, const wxString& mimetype, int index = -1);
%new wxImage* wxImageFromBitmap(const wxBitmap &bitmap);
%new wxImage* wxImageFromData(int width, int height, unsigned char* data);
%new wxImage* wxImageFromStream(wxInputStream& stream, long type = wxBITMAP_TYPE_ANY, int index = -1);
%new wxImage* wxImageFromStreamMime(wxInputStream& stream, const wxString& mimetype, int index = -1 );

%{
    wxImage* wxEmptyImage(int width=0, int height=0) {
        if (width == 0 && height == 0)
            return new wxImage;
        else
            return new wxImage(width, height);
    }


    wxImage* wxImageFromMime(const wxString& name, const wxString& mimetype, int index) {
        return new wxImage(name, mimetype, index);
    }


    wxImage* wxImageFromBitmap(const wxBitmap &bitmap) {
        return new wxImage(bitmap.ConvertToImage());
    }


    wxImage* wxImageFromData(int width, int height, unsigned char* data) {
        // Copy the source data so the wxImage can clean it up later
        unsigned char* copy = (unsigned char*)malloc(width*height*3);
        if (copy == NULL) {
            PyErr_NoMemory();
            return NULL;
        }
        memcpy(copy, data, width*height*3);
        return new wxImage(width, height, copy, FALSE);
    }


    wxImage* wxImageFromStream(wxInputStream& stream,
                               long type = wxBITMAP_TYPE_ANY, int index = -1) {
        return new wxImage(stream, type, index);
    }


    wxImage* wxImageFromStreamMime(wxInputStream& stream,
                                   const wxString& mimetype, int index = -1 ) {
        return new wxImage(stream, mimetype, index);
    }
%}



void wxInitAllImageHandlers();


%readonly
%{
#if 0
%}

extern wxImage    wxNullImage;

%readwrite
%{
#endif
%}



//---------------------------------------------------------------------------
// This one is here to avoid circular imports

%new wxBitmap* wxBitmapFromImage(const wxImage& img, int depth=-1);

%{
    wxBitmap* wxBitmapFromImage(const wxImage& img, int depth=-1) {
        return new wxBitmap(img, depth);
    }

%}


//---------------------------------------------------------------------------
