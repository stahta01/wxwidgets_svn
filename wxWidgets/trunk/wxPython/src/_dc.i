/////////////////////////////////////////////////////////////////////////////
// Name:        _dc.i
// Purpose:     SWIG interface defs for wxDC and releated classes
//
// Author:      Robin Dunn
//
// Created:     7-July-1997
// RCS-ID:      $Id$
// Copyright:   (c) 2003 by Total Control Software
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

// Not a %module


//---------------------------------------------------------------------------

%{
#include "wx/wxPython/pydrawxxx.h"
%}

// TODO: 1. wrappers for wxDrawObject and wxDC::DrawObject


//---------------------------------------------------------------------------

%typemap(in) (int points, wxPoint* points_array ) {
    $2 = wxPoint_LIST_helper($input, &$1);
    if ($2 == NULL) SWIG_fail;
}
%typemap(freearg) (int points, wxPoint* points_array ) {
    if ($2) delete [] $2;
}


//---------------------------------------------------------------------------
%newgroup;


// wxDC is the device context - object on which any drawing is done
class wxDC : public wxObject {
public:
//    wxDC(); **** abstract base class, can't instantiate.
    ~wxDC();


    virtual void BeginDrawing();
    virtual void EndDrawing();


//    virtual void DrawObject(wxDrawObject* drawobject);



#if 0  // The old way

    bool FloodFill(wxCoord x, wxCoord y, const wxColour& col, int style = wxFLOOD_SURFACE);
    //bool GetPixel(wxCoord x, wxCoord y, wxColour *col) const;
    %extend {
        wxColour GetPixel(wxCoord x, wxCoord y) {
            wxColour col;
            self->GetPixel(x, y, &col);
            return col;
        }
    }
    void DrawLine(wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2);
    void CrossHair(wxCoord x, wxCoord y);
    void DrawArc(wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2, wxCoord xc, wxCoord yc);
    void DrawCheckMark(wxCoord x, wxCoord y, wxCoord width, wxCoord height);
    void DrawEllipticArc(wxCoord x, wxCoord y, wxCoord w, wxCoord h, double sa, double ea);
    void DrawPoint(wxCoord x, wxCoord y);
    void DrawRectangle(wxCoord x, wxCoord y, wxCoord width, wxCoord height);
    %name(DrawRectangleRect)void DrawRectangle(const wxRect& rect);
    void DrawRoundedRectangle(wxCoord x, wxCoord y, wxCoord width, wxCoord height, double radius);
    void DrawCircle(wxCoord x, wxCoord y, wxCoord radius);
    void DrawEllipse(wxCoord x, wxCoord y, wxCoord width, wxCoord height);
    void DrawIcon(const wxIcon& icon, wxCoord x, wxCoord y);
    void DrawBitmap(const wxBitmap &bmp, wxCoord x, wxCoord y, bool useMask = FALSE);
    void DrawText(const wxString& text, wxCoord x, wxCoord y);
    void DrawRotatedText(const wxString& text, wxCoord x, wxCoord y, double angle);
    bool Blit(wxCoord xdest, wxCoord ydest, wxCoord width, wxCoord height,
              wxDC *source, wxCoord xsrc, wxCoord ysrc,
              int rop = wxCOPY, bool useMask = FALSE,
              wxCoord xsrcMask = -1, wxCoord ysrcMask = -1);



#else  // The new way
    
    %name(FloodFillXY) bool FloodFill(wxCoord x, wxCoord y, const wxColour& col, int style = wxFLOOD_SURFACE);
    bool FloodFill(const wxPoint& pt, const wxColour& col, int style = wxFLOOD_SURFACE);

    //%name(GetPixelXY) bool GetPixel(wxCoord x, wxCoord y, wxColour *col) const;
    //bool GetPixel(const wxPoint& pt, wxColour *col) const;
    %extend {
        wxColour GetPixelXY(wxCoord x, wxCoord y) {
            wxColour col;
            self->GetPixel(x, y, &col);
            return col;
        }
        wxColour GetPixel(const wxPoint& pt) {
            wxColour col;
            self->GetPixel(pt, &col);
            return col;
        }
    }
    %name(DrawLineXY) void DrawLine(wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2);
    void DrawLine(const wxPoint& pt1, const wxPoint& pt2);

    %name(CrossHairXY) void CrossHair(wxCoord x, wxCoord y);
    void CrossHair(const wxPoint& pt);

    %name(DrawArcXY) void DrawArc(wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2, wxCoord xc, wxCoord yc);
    void DrawArc(const wxPoint& pt1, const wxPoint& pt2, const wxPoint& centre);

    %name(DrawCheckMarkXY) void DrawCheckMark(wxCoord x, wxCoord y, wxCoord width, wxCoord height);
    void DrawCheckMark(const wxRect& rect);

    %name(DrawEllipticArcXY) void DrawEllipticArc(wxCoord x, wxCoord y, wxCoord w, wxCoord h, double sa, double ea);
    void DrawEllipticArc(const wxPoint& pt, const wxSize& sz, double sa, double ea);

    %name(DrawPointXY) void DrawPoint(wxCoord x, wxCoord y);
    void DrawPoint(const wxPoint& pt);

    %name(DrawRectangleXY) void DrawRectangle(wxCoord x, wxCoord y, wxCoord width, wxCoord height);
    void DrawRectangle(const wxPoint& pt, const wxSize& sz);
    %name(DrawRectangleRect) void DrawRectangle(const wxRect& rect);

    %name(DrawRoundedRectangleXY) void DrawRoundedRectangle(wxCoord x, wxCoord y, wxCoord width, wxCoord height, double radius);
    void DrawRoundedRectangle(const wxPoint& pt, const wxSize& sz, double radius);
    %name(DrawRoundedRectangleRect) void DrawRoundedRectangle(const wxRect& r, double radius);

    %name(DrawCircleXY) void DrawCircle(wxCoord x, wxCoord y, wxCoord radius);
    void DrawCircle(const wxPoint& pt, wxCoord radius);

    %name(DrawEllipseXY) void DrawEllipse(wxCoord x, wxCoord y, wxCoord width, wxCoord height);
    void DrawEllipse(const wxPoint& pt, const wxSize& sz);
    %name(DrawEllipseRect) void DrawEllipse(const wxRect& rect);

    %name(DrawIconXY) void DrawIcon(const wxIcon& icon, wxCoord x, wxCoord y);
    void DrawIcon(const wxIcon& icon, const wxPoint& pt);

    %name(DrawBitmapXY) void DrawBitmap(const wxBitmap &bmp, wxCoord x, wxCoord y, bool useMask = FALSE);
    void DrawBitmap(const wxBitmap &bmp, const wxPoint& pt, bool useMask = FALSE);

    %name(DrawTextXY) void DrawText(const wxString& text, wxCoord x, wxCoord y);
    void DrawText(const wxString& text, const wxPoint& pt);

    %name(DrawRotatedTextXY) void DrawRotatedText(const wxString& text, wxCoord x, wxCoord y, double angle);
    void DrawRotatedText(const wxString& text, const wxPoint& pt, double angle);

    
    %name(BlitXY) bool Blit(wxCoord xdest, wxCoord ydest, wxCoord width, wxCoord height,
                            wxDC *source, wxCoord xsrc, wxCoord ysrc,
                            int rop = wxCOPY, bool useMask = FALSE,
                            wxCoord xsrcMask = -1, wxCoord ysrcMask = -1);
    bool Blit(const wxPoint& destPt, const wxSize& sz,  
              wxDC *source, const wxPoint& srcPt,
              int rop = wxCOPY, bool useMask = FALSE,
              const wxPoint& srcPtMask = wxDefaultPosition);

#endif
    
    void DrawLines(int points, wxPoint* points_array, wxCoord xoffset = 0, wxCoord yoffset = 0);

    void DrawPolygon(int points, wxPoint* points_array,
                     wxCoord xoffset = 0, wxCoord yoffset = 0,
                     int fillStyle = wxODDEVEN_RULE);


    // this version puts both optional bitmap and the text into the given
    // rectangle and aligns is as specified by alignment parameter; it also
    // will emphasize the character with the given index if it is != -1 and
    // return the bounding rectangle if required
    void DrawLabel(const wxString& text, const wxRect& rect,
                   int alignment = wxALIGN_LEFT | wxALIGN_TOP,
                   int indexAccel = -1);
    %extend {
        wxRect DrawImageLabel(const wxString& text,
                              const wxBitmap& image,
                              const wxRect& rect,
                              int alignment = wxALIGN_LEFT | wxALIGN_TOP,
                              int indexAccel = -1) {
            wxRect rv;
            self->DrawLabel(text, image, rect, alignment, indexAccel, &rv);
            return rv;
        }
    }



    void DrawSpline(int points, wxPoint* points_array);


    
    // global DC operations
    // --------------------

    virtual void Clear();

    virtual bool StartDoc(const wxString& message);
    virtual void EndDoc();

    virtual void StartPage();
    virtual void EndPage();

    
    // set objects to use for drawing
    // ------------------------------

    virtual void SetFont(const wxFont& font);
    virtual void SetPen(const wxPen& pen);
    virtual void SetBrush(const wxBrush& brush);
    virtual void SetBackground(const wxBrush& brush);
    virtual void SetBackgroundMode(int mode);
    virtual void SetPalette(const wxPalette& palette);

    
    // clipping region
    // ---------------

    void SetClippingRegion(wxCoord x, wxCoord y, wxCoord width, wxCoord height);
    //void SetClippingRegion(const wxPoint& pt, const wxSize& sz)
    %name(SetClippingRect) void SetClippingRegion(const wxRect& rect);
    %name(SetClippingRegionAsRegion) void SetClippingRegion(const wxRegion& region);

    virtual void DestroyClippingRegion();

    void GetClippingBox(wxCoord *OUTPUT, wxCoord *OUTPUT, wxCoord *OUTPUT, wxCoord *OUTPUT) const;
    %extend {
        wxRect GetClippingRect() {
            wxRect rect;
            self->GetClippingBox(rect);
            return rect;
        }
    }
    

    
    // text extent
    // -----------

    virtual wxCoord GetCharHeight() const;
    virtual wxCoord GetCharWidth() const;

    // only works for single line strings
    void GetTextExtent(const wxString& string, wxCoord *OUTPUT, wxCoord *OUTPUT);
    %name(GetFullTextExtent)void GetTextExtent(const wxString& string,
                       wxCoord *OUTPUT, wxCoord *OUTPUT, wxCoord *OUTPUT, wxCoord* OUTPUT,
                       wxFont* font = NULL);

    // works for single as well as multi-line strings
    void GetMultiLineTextExtent(const wxString& text, wxCoord *OUTPUT, wxCoord *OUTPUT, wxCoord *OUTPUT,
                                wxFont *font = NULL);
    

    // size and resolution
    // -------------------

    // in device units
    %name(GetSizeTuple)void GetSize(int* OUTPUT, int* OUTPUT);
    wxSize GetSize();

    // in mm
    %name(GetSizeMMWH)void GetSizeMM(int* OUTPUT, int* OUTPUT) const;
    wxSize GetSizeMM() const;
    

    // coordinates conversions
    // -----------------------

    // This group of functions does actual conversion of the input, as you'd
    // expect.
    wxCoord DeviceToLogicalX(wxCoord x) const;
    wxCoord DeviceToLogicalY(wxCoord y) const;
    wxCoord DeviceToLogicalXRel(wxCoord x) const;
    wxCoord DeviceToLogicalYRel(wxCoord y) const;
    wxCoord LogicalToDeviceX(wxCoord x) const;
    wxCoord LogicalToDeviceY(wxCoord y) const;
    wxCoord LogicalToDeviceXRel(wxCoord x) const;
    wxCoord LogicalToDeviceYRel(wxCoord y) const;

    // query DC capabilities
    // ---------------------

    virtual bool CanDrawBitmap() const;
    virtual bool CanGetTextExtent() const;

    // colour depth
    virtual int GetDepth() const;

    // Resolution in Pixels per inch
    virtual wxSize GetPPI() const;

    virtual bool Ok() const;

    

    int GetBackgroundMode() const;
    const wxBrush&  GetBackground() const;
    const wxBrush&  GetBrush() const;
    const wxFont&   GetFont() const;
    const wxPen&    GetPen() const;
    const wxColour& GetTextBackground() const;
    const wxColour& GetTextForeground() const;

    virtual void SetTextForeground(const wxColour& colour);
    virtual void SetTextBackground(const wxColour& colour);

    int GetMapMode() const;
    virtual void SetMapMode(int mode);

    virtual void GetUserScale(double *OUTPUT, double *OUTPUT) const;
    virtual void SetUserScale(double x, double y);

    virtual void GetLogicalScale(double *OUTPUT, double *OUTPUT);
    virtual void SetLogicalScale(double x, double y);

    %name(GetLogicalOriginTuple) void GetLogicalOrigin(wxCoord *OUTPUT, wxCoord *OUTPUT) const;
    wxPoint GetLogicalOrigin() const;
    virtual void SetLogicalOrigin(wxCoord x, wxCoord y);

    
    %name(GetDeviceOriginTuple) void GetDeviceOrigin(wxCoord *OUTPUT, wxCoord *OUTPUT) const;
    wxPoint GetDeviceOrigin() const;
    virtual void SetDeviceOrigin(wxCoord x, wxCoord y);

    virtual void SetAxisOrientation(bool xLeftRight, bool yBottomUp);

    int GetLogicalFunction() const;
    virtual void SetLogicalFunction(int function);

    virtual void SetOptimization(bool opt);
    virtual bool GetOptimization();
    

    // bounding box
    // ------------

    virtual void CalcBoundingBox(wxCoord x, wxCoord y);
    void ResetBoundingBox();

    // Get the final bounding box of the PostScript or Metafile picture.
    wxCoord MinX() const;
    wxCoord MaxX() const;
    wxCoord MinY() const;
    wxCoord MaxY() const;


    %extend {
        void GetBoundingBox(int* OUTPUT, int* OUTPUT, int* OUTPUT, int* OUTPUT);
        // See below for implementation
    }    
    %pythoncode { def __nonzero__(self): return self.Ok() };



    %extend { // See drawlist.cpp for impplementaion of these...
        PyObject* _DrawPointList(PyObject* pyCoords, PyObject* pyPens, PyObject* pyBrushes)
        {
            return wxPyDrawXXXList(*self, wxPyDrawXXXPoint, pyCoords, pyPens, pyBrushes);
        }

        PyObject* _DrawLineList(PyObject* pyCoords, PyObject* pyPens, PyObject* pyBrushes)
        {
            return wxPyDrawXXXList(*self, wxPyDrawXXXLine, pyCoords, pyPens, pyBrushes);
        }

        PyObject* _DrawRectangleList(PyObject* pyCoords, PyObject* pyPens, PyObject* pyBrushes)
        {
            return wxPyDrawXXXList(*self, wxPyDrawXXXRectangle, pyCoords, pyPens, pyBrushes);
        }

        PyObject* _DrawEllipseList(PyObject* pyCoords, PyObject* pyPens, PyObject* pyBrushes)
        {
            return wxPyDrawXXXList(*self, wxPyDrawXXXEllipse, pyCoords, pyPens, pyBrushes);
        }

        PyObject* _DrawPolygonList(PyObject* pyCoords, PyObject* pyPens, PyObject* pyBrushes)
        {
            return wxPyDrawXXXList(*self, wxPyDrawXXXPolygon, pyCoords, pyPens, pyBrushes);
        }

        PyObject* _DrawTextList(PyObject* textList, PyObject* pyPoints,
                                PyObject* foregroundList, PyObject* backgroundList) {
            return wxPyDrawTextList(*self, textList, pyPoints, foregroundList, backgroundList);
        }
    }

    %pythoncode {
    def DrawPointList(self, points, pens=None):
        if pens is None:
           pens = []
        elif isinstance(pens, wx.Pen):
           pens = [pens]
        elif len(pens) != len(points):
           raise ValueError('points and pens must have same length')
        return self._DrawPointList(points, pens, [])


    def DrawLineList(self, lines, pens=None):
        if pens is None:
           pens = []
        elif isinstance(pens, wx.Pen):
           pens = [pens]
        elif len(pens) != len(lines):
           raise ValueError('lines and pens must have same length')
        return self._DrawLineList(lines, pens, [])


    def DrawRectangleList(self, rectangles, pens=None, brushes=None):
        if pens is None:
           pens = []
        elif isinstance(pens, wx.Pen):
           pens = [pens]
        elif len(pens) != len(rectangles):
           raise ValueError('rectangles and pens must have same length')
        if brushes is None:
           brushes = []
        elif isinstance(brushes, wx.Brush):
           brushes = [brushes]
        elif len(brushes) != len(rectangles):
           raise ValueError('rectangles and brushes must have same length')
        return self._DrawRectangleList(rectangles, pens, brushes)


    def DrawEllipseList(self, ellipses, pens=None, brushes=None):
        if pens is None:
           pens = []
        elif isinstance(pens, wx.Pen):
           pens = [pens]
        elif len(pens) != len(ellipses):
           raise ValueError('ellipses and pens must have same length')
        if brushes is None:
           brushes = []
        elif isinstance(brushes, wx.Brush):
           brushes = [brushes]
        elif len(brushes) != len(ellipses):
           raise ValueError('ellipses and brushes must have same length')
        return self._DrawEllipseList(ellipses, pens, brushes)


    def DrawPolygonList(self, polygons, pens=None, brushes=None):
        ## Note: This does not currently support fill style or offset
        ## you can always use the non-List version if need be.
        if pens is None:
           pens = []
        elif isinstance(pens, wx.Pen):
           pens = [pens]
        elif len(pens) != len(polygons):
           raise ValueError('polygons and pens must have same length')
        if brushes is None:
           brushes = []
        elif isinstance(brushes, wx.Brush):
           brushes = [brushes]
        elif len(brushes) != len(polygons):
           raise ValueError('polygons and brushes must have same length')
        return self._DrawPolygonList(polygons, pens, brushes)


    def DrawTextList(self, textList, coords, foregrounds = None, backgrounds = None, fonts = None):
        ## NOTE: this does not currently support changing the font
        ##       Make sure you set Background mode to wxSolid (DC.SetBackgroundMode)
        ##       If you want backgounds to do anything.
        if type(textList) == type(''):
           textList = [textList]
        elif len(textList) != len(coords):
           raise ValueError('textlist and coords must have same length')
        if foregrounds is None:
           foregrounds = []
        elif isinstance(foregrounds, wxColour):
           foregrounds = [foregrounds]
        elif len(foregrounds) != len(coords):
           raise ValueError('foregrounds and coords must have same length')
        if backgrounds is None:
           backgrounds = []
        elif isinstance(backgrounds, wxColour):
           backgrounds = [backgrounds]
        elif len(backgrounds) != len(coords):
           raise ValueError('backgrounds and coords must have same length')
        return  self._DrawTextList(textList, coords, foregrounds, backgrounds)
    }

};



%{
static void wxDC_GetBoundingBox(wxDC* dc, int* x1, int* y1, int* x2, int* y2) {
    *x1 = dc->MinX();
    *y1 = dc->MinY();
    *x2 = dc->MaxX();
    *y2 = dc->MaxY();
}
%}


//---------------------------------------------------------------------------
%newgroup

class wxMemoryDC : public wxDC {
public:
    wxMemoryDC();
    %name(MemoryDCFromDC) wxMemoryDC(wxDC* oldDC);
        
    void SelectObject(const wxBitmap& bitmap);
};

//---------------------------------------------------------------------------
%newgroup

class wxBufferedDC : public wxMemoryDC
{
public:
    %addtofunc wxBufferedDC( wxDC *dc, const wxBitmap &buffer ) 
        "self._dc = args[0] # save a ref so the other dc will not be deleted before self";
    %addtofunc wxBufferedDC( wxDC *dc, const wxSize &area ) 
        "val._dc = args[0] # save a ref so the other dc will not be deleted before self";

    // Construct a wxBufferedDC using a user supplied buffer.
    wxBufferedDC( wxDC *dc, const wxBitmap &buffer );

        // Construct a wxBufferedDC with an internal buffer of 'area'
        // (where area is usually something like the size of the window
        // being buffered)
    %name(BufferedDCInternalBuffer) wxBufferedDC( wxDC *dc, const wxSize &area );

        // Blits the buffer to the dc, and detaches the dc from
        // the buffer.  Usually called in the dtor or by the dtor
        // of derived classes if the BufferedDC must blit before
        // the derived class (which may own the dc it's blitting
        // to) is destroyed.
    void UnMask();
};


class wxBufferedPaintDC : public wxBufferedDC
{
public:
    wxBufferedPaintDC( wxWindow *window, const wxBitmap &buffer = wxNullBitmap );
};


//---------------------------------------------------------------------------
%newgroup

class wxScreenDC : public wxDC {
public:
    wxScreenDC();

    %name(StartDrawingOnTopWin) bool StartDrawingOnTop(wxWindow* window);
    bool StartDrawingOnTop(wxRect* rect = NULL);
    bool EndDrawingOnTop();
};

//---------------------------------------------------------------------------
%newgroup

class wxClientDC : public wxDC {
public:
      wxClientDC(wxWindow* win);
};

//---------------------------------------------------------------------------
%newgroup

class wxPaintDC : public wxDC {
public:
      wxPaintDC(wxWindow* win);
};

//---------------------------------------------------------------------------
%newgroup

class wxWindowDC : public wxDC {
public:
      wxWindowDC(wxWindow* win);
};

//---------------------------------------------------------------------------
%newgroup

class wxMirrorDC : public wxDC
{
public:
    // constructs a mirror DC associated with the given real DC
    //
    // if mirror parameter is true, all vertical and horizontal coordinates are
    // exchanged, otherwise this class behaves in exactly the same way as a
    // plain DC
    //
    wxMirrorDC(wxDC& dc, bool mirror);
};

//---------------------------------------------------------------------------
%newgroup

%{
#include <wx/dcps.h>
%}

class wxPostScriptDC : public wxDC {
public:
    wxPostScriptDC(const wxPrintData& printData);
//     %name(PostScriptDC2)wxPostScriptDC(const wxString& output,
//                                          bool interactive = TRUE,
//                                          wxWindow* parent = NULL);

    wxPrintData& GetPrintData();
    void SetPrintData(const wxPrintData& data);

    static void SetResolution(int ppi);
    static int GetResolution();
};

//---------------------------------------------------------------------------
%newgroup


#ifdef __WXMSW__

%{
#include <wx/metafile.h>
%}

class wxMetaFile : public wxObject {
public:
    wxMetaFile(const wxString& filename = wxPyEmptyString);
    ~wxMetaFile();

    bool Ok();
    bool SetClipboard(int width = 0, int height = 0);

    wxSize GetSize();
    int GetWidth();
    int GetHeight();

    const wxString& GetFileName() const;

    %pythoncode { def __nonzero__(self): return self.Ok() }
};

// bool wxMakeMetaFilePlaceable(const wxString& filename,
//                              int minX, int minY, int maxX, int maxY, float scale=1.0);


class wxMetaFileDC : public wxDC {
public:
    wxMetaFileDC(const wxString& filename = wxPyEmptyString,
                 int width = 0, int height = 0,
                 const wxString& description = wxPyEmptyString);
    wxMetaFile* Close();
};



#else  // Make some dummies for the other platforms

%{
class wxMetaFile : public wxObject {
public:
    wxMetaFile(const wxString&)
        { PyErr_SetNone(PyExc_NotImplementedError); }
};

class wxMetaFileDC : public wxClientDC {
public:
    wxMetaFileDC(const wxString&, int, int, const wxString&)
        { PyErr_SetNone(PyExc_NotImplementedError); }
};

%}

class wxMetaFile : public wxObject {
public:
    wxMetaFile(const wxString& filename = wxPyEmptyString);
};

class wxMetaFileDC : public wxDC {
public:
    wxMetaFileDC(const wxString& filename = wxPyEmptyString,
                 int width = 0, int height = 0,
                 const wxString& description = wxPyEmptyString);
};


#endif


//---------------------------------------------------------------------------

#if defined(__WXMSW__) || defined(__WXMAC__)

class  wxPrinterDC : public wxDC {
public:
    wxPrinterDC(const wxPrintData& printData);
//     %name(PrinterDC2) wxPrinterDC(const wxString& driver,
//                                     const wxString& device,
//                                     const wxString& output,
//                                     bool interactive = TRUE,
//                                     int orientation = wxPORTRAIT);
};

#else
%{
class  wxPrinterDC : public wxClientDC {
public:
    wxPrinterDC(const wxPrintData&)
        { PyErr_SetNone(PyExc_NotImplementedError); }
    
//     wxPrinterDC(const wxString&, const wxString&, const wxString&, bool, int)
//         { PyErr_SetNone(PyExc_NotImplementedError); }
};
%}
 
class  wxPrinterDC : public wxDC {
public:
    wxPrinterDC(const wxPrintData& printData);
//     %name(PrinterDC2) wxPrinterDC(const wxString& driver,
//                                     const wxString& device,
//                                     const wxString& output,
//                                     bool interactive = TRUE,
//                                     int orientation = wxPORTRAIT);
};
#endif

//---------------------------------------------------------------------------
