/////////////////////////////////////////////////////////////////////////////
// Name:        _image.i
// Purpose:     SWIG definitions for wxImage and such
//
// Author:      Robin Dunn
//
// Created:     25-Sept-2000
// RCS-ID:      $Id$
// Copyright:   (c) 2003 by Total Control Software
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

// Not a %module


//---------------------------------------------------------------------------

%{
#include "wx/wxPython/pyistream.h"
%}

//---------------------------------------------------------------------------
%newgroup


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

class wxImageHistogram /* : public wxImageHistogramBase */
{
public:
    wxImageHistogram();

    // get the key in the histogram for the given RGB values
    static unsigned long MakeKey(unsigned char r,
                                 unsigned char g,
                                 unsigned char b);

    // find first colour that is not used in the image and has higher
    // RGB values than RGB(startR, startG, startB)
    //
    // returns true and puts this colour in r, g, b (each of which may be NULL)
    // on success or returns false if there are no more free colours
    bool FindFirstUnusedColour(unsigned char *OUTPUT,
                               unsigned char *OUTPUT,
                               unsigned char *OUTPUT,
                               unsigned char startR = 1,
                               unsigned char startG = 0,
                               unsigned char startB = 0 ) const;
};


//---------------------------------------------------------------------------


class wxImage : public wxObject {
public:
    wxImage( const wxString& name, long type = wxBITMAP_TYPE_ANY, int index = -1 );
    ~wxImage();

    // Alternate constructors
    %name(ImageFromMime) wxImage(const wxString& name, const wxString& mimetype, int index = -1);
    %name(ImageFromStream) wxImage(wxInputStream& stream, long type = wxBITMAP_TYPE_ANY, int index = -1);
    %name(ImageFromStreamMime) wxImage(wxInputStream& stream, const wxString& mimetype, int index = -1 );
    %extend {
        %name(EmptyImage) wxImage(int width=0, int height=0, bool clear = TRUE) {
            if (width > 0 && height > 0)
                return new wxImage(width, height, clear);
            else
                return new wxImage;
        }   
           
        %name(ImageFromBitmap) wxImage(const wxBitmap &bitmap) {
            return new wxImage(bitmap.ConvertToImage());
        }
   
        %name(ImageFromData) wxImage(int width, int height, unsigned char* data) {
            // Copy the source data so the wxImage can clean it up later
            unsigned char* copy = (unsigned char*)malloc(width*height*3);
            if (copy == NULL) {
                PyErr_NoMemory();
                return NULL;
            }
            memcpy(copy, data, width*height*3);
            return new wxImage(width, height, copy, FALSE);
        }
    }
    
    void Create( int width, int height );
    void Destroy();

    wxImage Scale( int width, int height );
    wxImage ShrinkBy( int xFactor , int yFactor ) const ;
    wxImage& Rescale(int width, int height);

    void SetRGB( int x, int y, unsigned char r, unsigned char g, unsigned char b );
    unsigned char GetRed( int x, int y );
    unsigned char GetGreen( int x, int y );
    unsigned char GetBlue( int x, int y );

    void SetAlpha(int x, int y, unsigned char alpha);
    unsigned char GetAlpha(int x, int y);
    bool HasAlpha();

    // find first colour that is not used in the image and has higher
    // RGB values than <startR,startG,startB>
    bool FindFirstUnusedColour( byte *OUTPUT, byte *OUTPUT, byte *OUTPUT,
                                byte startR = 0, byte startG = 0, byte startB = 0 ) const;

    // Set image's mask to the area of 'mask' that has <mr,mg,mb> colour
    bool SetMaskFromImage(const wxImage & mask,
                          byte mr, byte mg, byte mb);

//      void DoFloodFill (wxCoord x, wxCoord y,
//          const wxBrush & fillBrush,
//          const wxColour& testColour,
//          int style = wxFLOOD_SURFACE,
//          int LogicalFunction = wxCOPY /* currently unused */ ) ;

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

    %extend {
        PyObject* GetData() {
            unsigned char* data = self->GetData();
            int len = self->GetWidth() * self->GetHeight() * 3;
            PyObject* rv;
            wxPyBLOCK_THREADS( rv = PyString_FromStringAndSize((char*)data, len));
            return rv;
        }
        void SetData(PyObject* data) {
            unsigned char* dataPtr;

            if (! PyString_Check(data)) {
                PyErr_SetString(PyExc_TypeError, "Expected string object");
                return /* NULL */ ;
            }

            size_t len = self->GetWidth() * self->GetHeight() * 3;
            dataPtr = (unsigned char*) malloc(len);
            wxPyBLOCK_THREADS( memcpy(dataPtr, PyString_AsString(data), len) );
            self->SetData(dataPtr);
            // wxImage takes ownership of dataPtr...
        }



        PyObject* GetDataBuffer() {
            unsigned char* data = self->GetData();
            int len = self->GetWidth() * self->GetHeight() * 3;
            PyObject* rv;
            wxPyBLOCK_THREADS( rv = PyBuffer_FromReadWriteMemory(data, len) );
            return rv;
        }
        void SetDataBuffer(PyObject* data) {
            unsigned char* buffer;
            int size;

            wxPyBeginBlockThreads();
            if (!PyArg_Parse(data, "t#", &buffer, &size))
                goto done;

            if (size != self->GetWidth() * self->GetHeight() * 3) {
                PyErr_SetString(PyExc_TypeError, "Incorrect buffer size");
                goto done;
            }
            self->SetData(buffer);
        done:
            wxPyEndBlockThreads();
        }



        PyObject* GetAlphaData() {
            unsigned char* data = self->GetAlpha();
            if (! data) {
                RETURN_NONE();
            } else {
                int len = self->GetWidth() * self->GetHeight();
                PyObject* rv;
                wxPyBLOCK_THREADS( rv = PyString_FromStringAndSize((char*)data, len) );
                return rv;
            }
        }
        void SetAlphaData(PyObject* data) {
            unsigned char* dataPtr;

            if (! PyString_Check(data)) {
                PyErr_SetString(PyExc_TypeError, "Expected string object");
                return /* NULL */ ;
            }

            size_t len = self->GetWidth() * self->GetHeight();
            dataPtr = (unsigned char*) malloc(len);
            wxPyBLOCK_THREADS( memcpy(dataPtr, PyString_AsString(data), len) );
            self->SetAlpha(dataPtr);
            // wxImage takes ownership of dataPtr...
        }



        PyObject* GetAlphaBuffer() {
            unsigned char* data = self->GetAlpha();
            int len = self->GetWidth() * self->GetHeight();
            PyObject* rv;
            wxPyBLOCK_THREADS( rv = PyBuffer_FromReadWriteMemory(data, len) );
            return rv;
        }
        void SetAlphaBuffer(PyObject* data) {
            unsigned char* buffer;
            int size;

            wxPyBeginBlockThreads();
            if (!PyArg_Parse(data, "t#", &buffer, &size))
                goto done;

            if (size != self->GetWidth() * self->GetHeight()) {
                PyErr_SetString(PyExc_TypeError, "Incorrect buffer size");
                goto done;
            }
            self->SetAlpha(buffer);
        done:
            wxPyEndBlockThreads();
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
    unsigned long ComputeHistogram( wxImageHistogram& h );

    static void AddHandler( wxImageHandler *handler );
    static void InsertHandler( wxImageHandler *handler );
    static bool RemoveHandler( const wxString& name );
    static wxString GetImageExtWildcard();


    %extend {
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

    %pythoncode { def __nonzero__(self): return self.Ok() }
};



void wxInitAllImageHandlers();


// See also wxPy_ReinitStockObjects in helpers.cpp
%immutable;
const wxImage    wxNullImage;
%mutable;

//---------------------------------------------------------------------------


MAKE_CONST_WXSTRING(IMAGE_OPTION_BMP_FORMAT);
MAKE_CONST_WXSTRING(IMAGE_OPTION_CUR_HOTSPOT_X);
MAKE_CONST_WXSTRING(IMAGE_OPTION_CUR_HOTSPOT_Y);
MAKE_CONST_WXSTRING(IMAGE_OPTION_RESOLUTION);
MAKE_CONST_WXSTRING(IMAGE_OPTION_RESOLUTIONUNIT);   

enum
{
    wxIMAGE_RESOLUTION_INCHES = 1,
    wxIMAGE_RESOLUTION_CM = 2
};


enum
{
    wxBMP_24BPP        = 24, // default, do not need to set
    //wxBMP_16BPP      = 16, // wxQuantize can only do 236 colors?
    wxBMP_8BPP         =  8, // 8bpp, quantized colors
    wxBMP_8BPP_GREY    =  9, // 8bpp, rgb averaged to greys
    wxBMP_8BPP_GRAY    =  wxBMP_8BPP_GREY,
    wxBMP_8BPP_RED     = 10, // 8bpp, red used as greyscale
    wxBMP_8BPP_PALETTE = 11, // 8bpp, use the wxImage's palette
    wxBMP_4BPP         =  4, // 4bpp, quantized colors
    wxBMP_1BPP         =  1, // 1bpp, quantized "colors"
    wxBMP_1BPP_BW      =  2  // 1bpp, black & white from red
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


//---------------------------------------------------------------------------

class wxPNGHandler : public wxImageHandler {
public:
    wxPNGHandler();
};


class wxGIFHandler : public wxImageHandler {
public:
    wxGIFHandler();
};


class wxPCXHandler : public wxImageHandler {
public:
    wxPCXHandler();
};


class wxJPEGHandler : public wxImageHandler {
public:
    wxJPEGHandler();
};


class wxPNMHandler : public wxImageHandler {
public:
    wxPNMHandler();
};

class wxXPMHandler : public wxImageHandler {
public:
    wxXPMHandler();
};

class wxTIFFHandler : public wxImageHandler {
public:
    wxTIFFHandler();
};


#if wxUSE_IFF
class wxIFFHandler : public wxImageHandler {
public:
    wxIFFHandler();
};
#endif

//---------------------------------------------------------------------------
