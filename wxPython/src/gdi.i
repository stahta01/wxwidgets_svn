/////////////////////////////////////////////////////////////////////////////
// Name:        gdi.i
// Purpose:     SWIG interface file for wxDC, wxBrush, wxPen, wxFont, etc.
//
// Author:      Robin Dunn
//
// Created:     7/7/97
// RCS-ID:      $Id$
// Copyright:   (c) 1998 by Total Control Software
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////


%module gdi

%{
#include "helpers.h"
#include <wx/metafile.h>
#include <wx/imaglist.h>
#ifndef __WXMSW__
#include <wx/dcps.h>
#endif
%}

//----------------------------------------------------------------------

%include typemaps.i
%include my_typemaps.i

// Import some definitions of other classes, etc.
%import _defs.i
%import misc.i

%{
    static wxString wxPyEmptyStr("");
%}

//---------------------------------------------------------------------------

class wxGDIObject : public wxObject {
public:
    wxGDIObject();
    ~wxGDIObject();

    bool GetVisible();
    void SetVisible( bool visible );

    bool IsNull();

};

//---------------------------------------------------------------------------

class wxBitmap : public wxGDIObject
{
public:
    wxBitmap(const wxString& name, wxBitmapType type);
    ~wxBitmap();

    wxPalette* GetPalette();
    wxMask* GetMask();
    bool LoadFile(const wxString& name, long flags);
    bool SaveFile(const wxString& name, int type, wxPalette* palette = NULL);
    void SetMask(wxMask* mask);
#ifdef __WXMSW__
    void SetPalette(wxPalette& palette);
#endif

    // wxGDIImage methods
#ifdef __WXMSW__
    long GetHandle();
    void SetHandle(long handle);
#endif
    bool Ok();
    int GetWidth();
    int GetHeight();
    int GetDepth();
    void SetWidth(int w);
    void SetHeight(int h);
    void SetDepth(int d);
#ifdef __WXMSW__
    void SetSize(const wxSize& size);
#endif

    wxBitmap GetSubBitmap( const wxRect& rect );
#ifdef __WXMSW__
    bool CopyFromIcon(const wxIcon& icon);
    bool CopyFromCursor(const wxCursor& cursor);
    int GetQuality();
    void SetQuality(int q);
#endif

    %pragma(python) addtoclass = "
    def __del__(self,gdic=gdic):
        try:
            if self.thisown == 1 :
                gdic.delete_wxBitmap(self)
        except:
            pass
"
};


// Declarations of some alternate "constructors"
%new wxBitmap* wxEmptyBitmap(int width, int height, int depth=-1);
%new wxBitmap* wxBitmapFromXPMData(PyObject* listOfStrings);
%new wxBitmap* wxBitmapFromIcon(const wxIcon& icon);

#ifdef __WXMSW__
%new wxBitmap* wxBitmapFromData(PyObject* data, long type,
                                int width, int height, int depth = 1);
#endif



%{ // Implementations of some alternate "constructors"

    wxBitmap* wxEmptyBitmap(int width, int height, int depth=-1) {
        return new wxBitmap(width, height, depth);
    }

    static char** ConvertListOfStrings(PyObject* listOfStrings) {
        char**    cArray = NULL;
        int       count;

        if (!PyList_Check(listOfStrings)) {
            PyErr_SetString(PyExc_TypeError, "Expected a list of strings.");
            return NULL;
        }
        count = PyList_Size(listOfStrings);
        cArray = new char*[count];

        for(int x=0; x<count; x++) {
            // TODO: Need some validation and error checking here
            cArray[x] = PyString_AsString(PyList_GET_ITEM(listOfStrings, x));
        }
        return cArray;
    }

    wxBitmap* wxBitmapFromXPMData(PyObject* listOfStrings) {
        char**    cArray = NULL;
        wxBitmap* bmp;

        cArray = ConvertListOfStrings(listOfStrings);
        if (! cArray)
            return NULL;
        bmp = new wxBitmap(cArray);
        delete [] cArray;
        return bmp;
    }


    wxBitmap* wxBitmapFromIcon(const wxIcon& icon) {
        return new wxBitmap(icon);
    }



#ifdef __WXMSW__
    wxBitmap* wxBitmapFromData(PyObject* data, long type,
                               int width, int height, int depth = 1) {
        if (! PyString_Check(data)) {
            PyErr_SetString(PyExc_TypeError, "Expected string object");
            return NULL;
        }

        return new wxBitmap((void*)PyString_AsString(data), type, width, height, depth);
    }
#endif
%}

//---------------------------------------------------------------------------

class wxMask : public wxObject {
public:
    wxMask(const wxBitmap& bitmap);
    //~wxMask();

    %addmethods { void Destroy() { delete self; } }
};

%new wxMask* wxMaskColour(const wxBitmap& bitmap, const wxColour& colour);
%{
    wxMask* wxMaskColour(const wxBitmap& bitmap, const wxColour& colour) {
        return new wxMask(bitmap, colour);
    }
%}


//---------------------------------------------------------------------------


class wxIcon : public wxGDIObject
{
public:
    wxIcon(const wxString& name, long flags,
           int desiredWidth = -1, int desiredHeight = -1);
    ~wxIcon();

    bool LoadFile(const wxString& name, long flags);

    // wxGDIImage methods
#ifdef __WXMSW__
    long GetHandle();
    void SetHandle(long handle);
#endif
    bool Ok();
    int GetWidth();
    int GetHeight();
    int GetDepth();
    void SetWidth(int w);
    void SetHeight(int h);
    void SetDepth(int d);
#ifdef __WXMSW__
    void SetSize(const wxSize& size);
#endif
    void CopyFromBitmap(const wxBitmap& bmp);

    %pragma(python) addtoclass = "
    def __del__(self,gdic=gdic):
        try:
            if self.thisown == 1 :
                gdic.delete_wxIcon(self)
        except:
            pass
"
};


// Declarations of some alternate "constructors"
%new wxIcon* wxEmptyIcon();
%new wxIcon* wxIconFromXPMData(PyObject* listOfStrings);

%{ // Implementations of some alternate "constructors"
    wxIcon* wxEmptyIcon() {
        return new wxIcon();
    }

    wxIcon* wxIconFromXPMData(PyObject* listOfStrings) {
        char**  cArray = NULL;
        wxIcon* icon;

        cArray = ConvertListOfStrings(listOfStrings);
        if (! cArray)
            return NULL;
        icon = new wxIcon(cArray);
        delete [] cArray;
        return icon;
    }
%}

//---------------------------------------------------------------------------

class wxCursor : public wxGDIObject
{
public:
#ifdef __WXMSW__
    wxCursor(const wxString& cursorName, long flags, int hotSpotX=0, int hotSpotY=0);
#endif
    ~wxCursor();

    // wxGDIImage methods
#ifdef __WXMSW__
    long GetHandle();
    void SetHandle(long handle);
#endif
    bool Ok();
#ifdef __WXMSW__
    int GetWidth();
    int GetHeight();
    int GetDepth();
    void SetWidth(int w);
    void SetHeight(int h);
    void SetDepth(int d);
    void SetSize(const wxSize& size);
#endif
};

%name(wxStockCursor) %new wxCursor* wxPyStockCursor(int id);
%{                              // Alternate 'constructor'
    wxCursor* wxPyStockCursor(int id) {
        return new wxCursor(id);
    }
%}

//----------------------------------------------------------------------


enum wxFontEncoding
{
    wxFONTENCODING_SYSTEM = -1,     // system default
    wxFONTENCODING_DEFAULT,         // current default encoding

    // ISO8859 standard defines a number of single-byte charsets
    wxFONTENCODING_ISO8859_1,       // West European (Latin1)
    wxFONTENCODING_ISO8859_2,       // Central and East European (Latin2)
    wxFONTENCODING_ISO8859_3,       // Esperanto (Latin3)
    wxFONTENCODING_ISO8859_4,       // Baltic languages (Estonian) (Latin4)
    wxFONTENCODING_ISO8859_5,       // Cyrillic
    wxFONTENCODING_ISO8859_6,       // Arabic
    wxFONTENCODING_ISO8859_7,       // Greek
    wxFONTENCODING_ISO8859_8,       // Hebrew
    wxFONTENCODING_ISO8859_9,       // Turkish (Latin5)
    wxFONTENCODING_ISO8859_10,      // Variation of Latin4 (Latin6)
    wxFONTENCODING_ISO8859_11,      // Thai
    wxFONTENCODING_ISO8859_12,      // doesn't exist currently, but put it
                                    // here anyhow to make all ISO8859
                                    // consecutive numbers
    wxFONTENCODING_ISO8859_13,      // Latin7
    wxFONTENCODING_ISO8859_14,      // Latin8
    wxFONTENCODING_ISO8859_15,      // Latin9 (a.k.a. Latin0, includes euro)

    // Cyrillic charset soup (see http://czyborra.com/charsets/cyrillic.html)
    wxFONTENCODING_KOI8,            // we don't support any of KOI8 variants
    wxFONTENCODING_ALTERNATIVE,     // same as MS-DOS CP866
    wxFONTENCODING_BULGARIAN,       // used under Linux in Bulgaria

    // what would we do without Microsoft? They have their own encodings
        // for DOS
    wxFONTENCODING_CP437,           // original MS-DOS codepage
    wxFONTENCODING_CP850,           // CP437 merged with Latin1
    wxFONTENCODING_CP852,           // CP437 merged with Latin2
    wxFONTENCODING_CP855,           // another cyrillic encoding
    wxFONTENCODING_CP866,           // and another one
        // and for Windows
    wxFONTENCODING_CP1250,          // WinLatin2
    wxFONTENCODING_CP1251,          // WinCyrillic
    wxFONTENCODING_CP1252,          // WinLatin1

    wxFONTENCODING_MAX
};


class wxFont : public wxGDIObject {
public:
    wxFont( int pointSize, int family, int style, int weight,
            int underline=FALSE, char* faceName = "",
            wxFontEncoding encoding=wxFONTENCODING_DEFAULT);
    ~wxFont();

    bool Ok();
    wxString GetFaceName();
    int GetFamily();
#ifdef __WXMSW__
    int GetFontId();
#endif
    int GetPointSize();
    int GetStyle();
    bool GetUnderlined();
    int GetWeight();
    wxFontEncoding GetEncoding();
    void SetFaceName(const wxString& faceName);
    void SetFamily(int family);
    void SetPointSize(int pointSize);
    void SetStyle(int style);
    void SetUnderlined(bool underlined);
    void SetWeight(int weight);
    void SetEncoding(wxFontEncoding encoding);
    wxString GetFamilyString();
    wxString GetStyleString();
    wxString GetWeightString();
};

%inline %{
    wxFontEncoding wxFont_GetDefaultEncoding() {
        return wxFont::GetDefaultEncoding();
    }

    void wxFont_SetDefaultEncoding(wxFontEncoding encoding) {
        wxFont::SetDefaultEncoding(encoding);
    }
%}


class wxFontList : public wxObject {
public:

    void AddFont(wxFont* font);
    wxFont * FindOrCreateFont(int point_size, int family, int style, int weight,
                              bool underline = FALSE, const char* facename = NULL,
                              wxFontEncoding encoding = wxFONTENCODING_DEFAULT);
    void RemoveFont(wxFont *font);
};


//----------------------------------------------------------------------

class wxColour : public wxObject {
public:
    wxColour(unsigned char red=0, unsigned char green=0, unsigned char blue=0);
    ~wxColour();
    unsigned char Red();
    unsigned char Green();
    unsigned char Blue();
    bool Ok();
    void Set(unsigned char red, unsigned char green, unsigned char blue);
    %addmethods {
        PyObject* Get() {
            PyObject* rv = PyTuple_New(3);
            PyTuple_SetItem(rv, 0, PyInt_FromLong(self->Red()));
            PyTuple_SetItem(rv, 1, PyInt_FromLong(self->Green()));
            PyTuple_SetItem(rv, 2, PyInt_FromLong(self->Blue()));
            return rv;
        }
    }
    %pragma(python) addtoclass = "asTuple = Get"
    %pragma(python) addtoclass = "def __str__(self): return str(self.asTuple())"
    %pragma(python) addtoclass = "def __repr__(self): return str(self.asTuple())"

};

%new wxColour* wxNamedColour(const wxString& colorName);

%{                                      // Alternate 'constructor'
    wxColour* wxNamedColour(const wxString& colorName) {
        return new wxColour(colorName);
    }
%}



class wxColourDatabase : public wxObject {
public:

    wxColour *FindColour(const wxString& colour);
    wxString FindName(const wxColour& colour) const;

    %addmethods {
        void Append(const wxString& name, int red, int green, int blue) {
            self->Append(name.c_str(), new wxColour(red, green, blue));
        }
    }
};


//----------------------------------------------------------------------


class wxPen : public wxGDIObject {
public:
    wxPen(wxColour& colour, int width=1, int style=wxSOLID);
    ~wxPen();

    int GetCap();
    wxColour& GetColour();

    int GetJoin();
    int GetStyle();
    int GetWidth();
    bool Ok();
    void SetCap(int cap_style);
    void SetColour(wxColour& colour);
    void SetJoin(int join_style);
    void SetStyle(int style);
    void SetWidth(int width);

            // **** This one needs to return a list of ints (wxDash)
    int GetDashes(wxDash **dashes);
    void SetDashes(int LCOUNT, wxDash* choices);

#ifdef __WXMSW__
    wxBitmap* GetStipple();
    void SetStipple(wxBitmap& stipple);
#endif
};


class wxPenList : public wxObject {
public:

    void AddPen(wxPen* pen);
    wxPen* FindOrCreatePen(const wxColour& colour, int width, int style);
    void RemovePen(wxPen* pen);
};



//----------------------------------------------------------------------

class wxBrush : public wxGDIObject {
public:
    wxBrush(const wxColour& colour, int style=wxSOLID);
    ~wxBrush();

    wxColour& GetColour();
    wxBitmap * GetStipple();
    int GetStyle();
    bool Ok();
    void SetColour(wxColour &colour);
    void SetStipple(wxBitmap& bitmap);
    void SetStyle(int style);
};


class wxBrushList {
public:

    void AddBrush(wxBrush *brush);
    wxBrush * FindOrCreateBrush(const wxColour& colour, int style);
    void RemoveBrush(wxBrush *brush);
};

//----------------------------------------------------------------------



class wxDC : public wxObject {
public:
//    wxDC(); **** abstract base class, can't instantiate.
    ~wxDC();

    void BeginDrawing();
//  %name(BlitXY)
    bool Blit(long xdest, long ydest,
              long width, long height,
              wxDC *source, long xsrc, long ysrc,
              int logicalFunc = wxCOPY, int useMask = FALSE);
//      bool Blit(const wxPoint& destPt, const wxSize& sz,
//                wxDC *source, const wxPoint& srcPt,
//                int logicalFunc = wxCOPY, int useMask = FALSE);

    void Clear();
    void CrossHair(long x, long y);
    void DestroyClippingRegion();
    long DeviceToLogicalX(long x);
    long DeviceToLogicalXRel(long x);
    long DeviceToLogicalY(long y);
    long DeviceToLogicalYRel(long y);
    void DrawArc(long x1, long y1, long x2, long y2, long xc, long yc);
    void DrawCircle(long x, long y, long radius);
    void DrawEllipse(long x, long y, long width, long height);
    void DrawEllipticArc(long x, long y, long width, long height, long start, long end);
    void DrawIcon(const wxIcon& icon, long x, long y);
    void DrawLine(long x1, long y1, long x2, long y2);
    void DrawLines(int PCOUNT, wxPoint* points, long xoffset=0, long yoffset=0);
    void DrawPolygon(int PCOUNT, wxPoint* points, long xoffset=0, long yoffset=0,
                     int fill_style=wxODDEVEN_RULE);
    void DrawPoint(long x, long y);
    void DrawRectangle(long x, long y, long width, long height);
    void DrawRotatedText(const wxString& text, wxCoord x, wxCoord y, double angle);
    void DrawRoundedRectangle(long x, long y, long width, long height, long radius=20);
    void DrawSpline(int PCOUNT, wxPoint* points);
    void DrawText(const wxString& text, long x, long y);
    void EndDoc();
    void EndDrawing();
    void EndPage();
    void FloodFill(long x, long y, const wxColour& colour, int style=wxFLOOD_SURFACE);
    wxBrush&  GetBackground();
    wxBrush&  GetBrush();
    long GetCharHeight();
    long GetCharWidth();
    void GetClippingBox(long *OUTPUT, long *OUTPUT,
                        long *OUTPUT, long *OUTPUT);
    wxFont& GetFont();
    int GetLogicalFunction();
    void GetLogicalScale(double *OUTPUT, double *OUTPUT);
    int GetMapMode();
    bool GetOptimization();
    wxPen& GetPen();
    %addmethods {
        %new wxColour* GetPixel(long x, long y) {
            wxColour* wc = new wxColour();
            self->GetPixel(x, y, wc);
            return wc;
        }
    }
    %name(GetSizeTuple)void GetSize(int* OUTPUT, int* OUTPUT);
    wxSize GetSize();
    wxSize GetSizeMM();
    wxColour& GetTextBackground();
    void GetTextExtent(const wxString& string, long *OUTPUT, long *OUTPUT);
    %name(GetFullTextExtent)void GetTextExtent(const wxString& string,
                       long *OUTPUT, long *OUTPUT, long *OUTPUT, long* OUTPUT,
                       const wxFont* font = NULL);
    wxColour& GetTextForeground();
    void GetUserScale(double *OUTPUT, double *OUTPUT);
    long LogicalToDeviceX(long x);
    long LogicalToDeviceXRel(long x);
    long LogicalToDeviceY(long y);
    long LogicalToDeviceYRel(long y);
    long MaxX();
    long MaxY();
    long MinX();
    long MinY();
    bool Ok();
    void SetDeviceOrigin(long x, long y);
    void SetBackground(const wxBrush& brush);
    void SetBackgroundMode(int mode);
    void SetClippingRegion(long x, long y, long width, long height);
    void SetPalette(const wxPalette& colourMap);
    void SetBrush(const wxBrush& brush);
    void SetFont(const wxFont& font);
    void SetLogicalFunction(int function);
    void SetLogicalScale(double x, double y);
    void SetMapMode(int mode);
    void SetOptimization(bool optimize);
    void SetPen(const wxPen& pen);
    void SetTextBackground(const wxColour& colour);
    void SetTextForeground(const wxColour& colour);
    void SetUserScale(double x_scale, double y_scale);
    bool StartDoc(const wxString& message);
    void StartPage();



    void DrawBitmap(const wxBitmap& bitmap, long x, long y,
                    int useMask = FALSE);

    bool CanDrawBitmap();
    bool CanGetTextExtent();
    int  GetDepth();
    wxSize GetPPI();

    void GetLogicalOrigin(int *OUTPUT, int *OUTPUT);
    void SetLogicalOrigin(int x, int y);
    void GetDeviceOrigin(int *OUTPUT, int *OUTPUT);
    void SetAxisOrientation(bool xLeftRight, bool yBottomUp);

    void CalcBoundingBox(int x, int y);
    void ResetBoundingBox();
};


//----------------------------------------------------------------------

class wxMemoryDC : public wxDC {
public:
    wxMemoryDC();

    void SelectObject(const wxBitmap& bitmap);
}

%new wxMemoryDC* wxMemoryDCFromDC(wxDC* oldDC);
%{                                      // Alternate 'constructor'
    wxMemoryDC* wxMemoryDCFromDC(wxDC* oldDC) {
        return new wxMemoryDC(oldDC);
    }
%}


//---------------------------------------------------------------------------

class wxScreenDC : public wxDC {
public:
    wxScreenDC();

    %name(StartDrawingOnTopWin) bool StartDrawingOnTop(wxWindow* window);
    bool StartDrawingOnTop(wxRect* rect = NULL);
    bool EndDrawingOnTop();
};

//---------------------------------------------------------------------------

class wxClientDC : public wxDC {
public:
      wxClientDC(wxWindow* win);
};

//---------------------------------------------------------------------------

class wxPaintDC : public wxDC {
public:
      wxPaintDC(wxWindow* win);
};

//---------------------------------------------------------------------------

class wxWindowDC : public wxDC {
public:
      wxWindowDC(wxWindow* win);
};

//---------------------------------------------------------------------------

#ifndef __WXMSW__
class wxPostScriptDC : public wxDC {
public:
      wxPostScriptDC(const wxString& output, bool interactive = TRUE, wxWindow* win = NULL);
};
#endif

//---------------------------------------------------------------------------


#ifdef __WXMSW__
class wxMetaFileDC : public wxDC {
public:
    wxMetaFileDC(const wxString& filename = wxPyEmptyStr);
    wxMetaFile* Close();
};
#endif

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------


%readonly
%{
#if 0
%}
extern wxFont *wxNORMAL_FONT;
extern wxFont *wxSMALL_FONT;
extern wxFont *wxITALIC_FONT;
extern wxFont *wxSWISS_FONT;

extern wxPen *wxRED_PEN;
extern wxPen *wxCYAN_PEN;
extern wxPen *wxGREEN_PEN;
extern wxPen *wxBLACK_PEN;
extern wxPen *wxWHITE_PEN;
extern wxPen *wxTRANSPARENT_PEN;
extern wxPen *wxBLACK_DASHED_PEN;
extern wxPen *wxGREY_PEN;
extern wxPen *wxMEDIUM_GREY_PEN;
extern wxPen *wxLIGHT_GREY_PEN;

extern wxBrush *wxBLUE_BRUSH;
extern wxBrush *wxGREEN_BRUSH;
extern wxBrush *wxWHITE_BRUSH;
extern wxBrush *wxBLACK_BRUSH;
extern wxBrush *wxTRANSPARENT_BRUSH;
extern wxBrush *wxCYAN_BRUSH;
extern wxBrush *wxRED_BRUSH;
extern wxBrush *wxGREY_BRUSH;
extern wxBrush *wxMEDIUM_GREY_BRUSH;
extern wxBrush *wxLIGHT_GREY_BRUSH;

extern wxColour *wxBLACK;
extern wxColour *wxWHITE;
extern wxColour *wxRED;
extern wxColour *wxBLUE;
extern wxColour *wxGREEN;
extern wxColour *wxCYAN;
extern wxColour *wxLIGHT_GREY;

extern wxCursor *wxSTANDARD_CURSOR;
extern wxCursor *wxHOURGLASS_CURSOR;
extern wxCursor *wxCROSS_CURSOR;

extern wxBitmap wxNullBitmap;
extern wxIcon   wxNullIcon;
extern wxCursor wxNullCursor;
extern wxPen    wxNullPen;
extern wxBrush  wxNullBrush;
extern wxPalette wxNullPalette;
extern wxFont   wxNullFont;
extern wxColour wxNullColour;


extern wxFontList*       wxTheFontList;
extern wxPenList*        wxThePenList;
extern wxBrushlist*      wxTheBrushList;
extern wxColourDatabase* wxTheColourDatabase;


%readwrite
%{
#endif
%}

//---------------------------------------------------------------------------

class wxPalette : public wxGDIObject {
public:
    wxPalette(int LCOUNT, byte* choices, byte* choices, byte* choices);
    ~wxPalette();

    int GetPixel(byte red, byte green, byte blue);
    bool GetRGB(int pixel, byte* OUTPUT, byte* OUTPUT, byte* OUTPUT);
    bool Ok();
};

//---------------------------------------------------------------------------

enum {
    wxIMAGELIST_DRAW_NORMAL ,
    wxIMAGELIST_DRAW_TRANSPARENT,
    wxIMAGELIST_DRAW_SELECTED,
    wxIMAGELIST_DRAW_FOCUSED,
    wxIMAGE_LIST_NORMAL,
    wxIMAGE_LIST_SMALL,
    wxIMAGE_LIST_STATE
};

class wxImageList : public wxObject {
public:
    wxImageList(int width, int height, int mask=TRUE, int initialCount=1);
    ~wxImageList();

    int Add(const wxBitmap& bitmap, const wxBitmap& mask = wxNullBitmap);
    %name(AddWithColourMask)int Add(const wxBitmap& bitmap, const wxColour& maskColour);
    %name(AddIcon)int Add(const wxIcon& icon);
#ifdef __WXMSW__
    bool Replace(int index, const wxBitmap& bitmap, const wxBitmap& mask = wxNullBitmap);
#else
//      %name(ReplaceIcon)bool Replace(int index, const wxIcon& icon);
//      int Add(const wxBitmap& bitmap);
    bool Replace(int index, const wxBitmap& bitmap);
#endif

    bool Draw(int index, wxDC& dc, int x, int x, int flags = wxIMAGELIST_DRAW_NORMAL,
              const bool solidBackground = FALSE);

    int GetImageCount();
    bool Remove(int index);
    bool RemoveAll();
    void GetSize(int index, int& OUTPUT, int& OUTPUT);
};


//---------------------------------------------------------------------------
// Regions, etc.

enum wxRegionContain {
	wxOutRegion, wxPartRegion, wxInRegion
};


class wxRegion : public wxGDIObject {
public:
    wxRegion(long x=0, long y=0, long width=0, long height=0);
    ~wxRegion();

    void Clear();
    wxRegionContain Contains(long x, long y);
    %name(ContainsPoint)wxRegionContain Contains(const wxPoint& pt);
    %name(ContainsRect)wxRegionContain Contains(const wxRect& rect);
    %name(ContainsRectDim)wxRegionContain Contains(long x, long y, long w, long h);

    wxRect GetBox();

    bool Intersect(long x, long y, long width, long height);
    %name(IntersectRect)bool Intersect(const wxRect& rect);
    %name(IntersectRegion)bool Intersect(const wxRegion& region);

    bool IsEmpty();

    bool Union(long x, long y, long width, long height);
    %name(UnionRect)bool Union(const wxRect& rect);
    %name(UnionRegion)bool Union(const wxRegion& region);

    bool Subtract(long x, long y, long width, long height);
    %name(SubtractRect)bool Subtract(const wxRect& rect);
    %name(SubtractRegion)bool Subtract(const wxRegion& region);

    bool Xor(long x, long y, long width, long height);
    %name(XorRect)bool Xor(const wxRect& rect);
    %name(XorRegion)bool Xor(const wxRegion& region);
};



class wxRegionIterator : public wxObject {
public:
    wxRegionIterator(const wxRegion& region);
    ~wxRegionIterator();

    long GetX();
    long GetY();
    long GetW();
    long GetWidth();
    long GetH();
    long GetHeight();
    wxRect GetRect();
    bool HaveRects();
    void Reset();

    %addmethods {
        void Next() {
            (*self) ++;
        }
    };
};


//---------------------------------------------------------------------------

