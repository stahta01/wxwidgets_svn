/////////////////////////////////////////////////////////////////////////////
// Name:        canvas.h
// Author:      Robert Roebling
// Created:     XX/XX/XX
// Copyright:   2000 (c) Robert Roebling
// Licence:     wxWindows Licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __WXCANVAS_H__
#define __WXCANVAS_H__

#ifdef __GNUG__
    #pragma interface "canvas.cpp"
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wx/image.h"
#include "wx/txtstrm.h"
#include "wx/geometry.h"


//----------------------------------------------------------------------------
// decls
//----------------------------------------------------------------------------

#define IMAGE_CANVAS 0

class wxCanvas;

//----------------------------------------------------------------------------
// wxCanvasObject
//----------------------------------------------------------------------------

class wxCanvasObject: public wxEvtHandler
{
public:
    wxCanvasObject();

    // Area occupied by object. Used for clipping, intersection,
    // mouse enter etc. Screen coordinates
    void SetArea( int x, int y, int width, int height );
    void SetArea( wxRect rect );

    // These are for screen output only therefore use
    // int as coordinates.
    virtual bool IsHit( int x, int y, int margin = 0 );
    virtual void Render(int xabs, int yabs, int clip_x, int clip_y, int clip_width, int clip_height );

    // use doubles later
    virtual void Move( int x, int y );

    // Once we have world coordinates in doubles, this will get
    // called for every object if the world coordinate system
    // changes (zooming).
    virtual void Recreate();

    // Later...
    virtual void WriteSVG( wxTextOutputStream &stream );

    wxCanvas *GetOwner()              { return m_owner; }
    virtual void SetOwner( wxCanvas *owner )  { m_owner = owner; }

    bool        IsControl()     { return m_isControl; }
    bool        IsVector()      { return m_isVector; }
    bool        IsImage()       { return m_isImage; }
    inline int  GetX()          { return m_area.x; }
    inline int  GetY()          { return m_area.y; }
    inline int  GetWidth()      { return m_area.width; }
    inline int  GetHeight()     { return m_area.height; }

    void CaptureMouse();
    void ReleaseMouse();
    bool IsCapturedMouse();

protected:
    wxCanvas   *m_owner;
    bool        m_isControl;
    bool        m_isVector;
    bool        m_isImage;

    //relative boundingbox in parent in pixels
    wxRect      m_area;

    friend class wxCanvas;
};

//----------------------------------------------------------------------------
// wxCanvasObjectGroup
//----------------------------------------------------------------------------

class wxCanvasObjectGroup
{
public:
    wxCanvasObjectGroup();
    virtual ~wxCanvasObjectGroup();

    void SetOwner(wxCanvas* canvas);
    wxCanvas *GetOwner()              { return m_owner; }

    virtual void Prepend( wxCanvasObject* obj );
    virtual void Append( wxCanvasObject* obj );
    virtual void Insert( size_t before, wxCanvasObject* obj );
    virtual void Remove( wxCanvasObject* obj );

    virtual void Recreate();
            void DeleteContents( bool );
    virtual void Render(int xabs, int yabs,int x, int y, int width, int height );
    virtual void WriteSVG( wxTextOutputStream &stream );
    virtual bool IsHit( int x, int y, int margin );
    virtual wxCanvasObject* IsHitObject( int x, int y, int margin );

    void ExtendArea(double x, double y);

    inline int  GetXMin()     { return m_minx; }
    inline int  GetYMin()     { return m_miny; }
    inline int  GetXMax()     { return m_maxx; }
    inline int  GetYMax()     { return m_maxy; }

protected:
    wxCanvas   *m_owner;

    //bounding box
    double        m_minx;
    double        m_miny;
    double        m_maxx;
    double        m_maxy;
    bool          m_validbounds;

    wxList        m_objects;

    friend class wxCanvas;
};

//----------------------------------------------------------------------------
// wxCanvasObjectGroupRef
//----------------------------------------------------------------------------

class wxCanvasObjectGroupRef: public wxCanvasObject
{
public:
    wxCanvasObjectGroupRef(double x, double y,wxCanvasObjectGroup* group);

    void SetOwner(wxCanvas* canvas);

    virtual void Recreate();
    virtual void Render(int xabs, int yabs,int x, int y, int width, int height );
    virtual void WriteSVG( wxTextOutputStream &stream );
    virtual bool IsHit( int x, int y, int margin );
    void Move( int x, int y );

    inline int  GetPosX()          { return m_x; }
    inline int  GetPosY()          { return m_y; }

    void ExtendArea(double x, double y);
    virtual wxCanvasObject* IsHitObject( int x, int y, int margin );

protected:
    //position of the group
    double        m_x;
    double        m_y;

    //reference to the group
    wxCanvasObjectGroup*        m_group;

    //bounding box
    double        m_minx;
    double        m_miny;
    double        m_maxx;
    double        m_maxy;
    bool          m_validbounds;

};

//----------------------------------------------------------------------------
// wxCanvasPolygon
//----------------------------------------------------------------------------

class wxCanvasPolygon: public wxCanvasObject
{
public:
    wxCanvasPolygon( int n, wxPoint2DDouble points[] );
    ~wxCanvasPolygon();
    void SetBrush(wxBrush& brush)  { m_brush = brush; };
    void SetPen(wxPen& pen)        { m_pen = pen; };

    virtual void Recreate();

    virtual void Render(int xabs, int yabs, int clip_x, int clip_y, int clip_width, int clip_height );
    virtual void WriteSVG( wxTextOutputStream &stream );

private:
    void ExtendArea(double x, double y);

    wxBrush       m_brush;
    wxPen         m_pen;

    int           m_n;
    wxPoint2DDouble* m_points;

    //bounding box
    double        m_minx;
    double        m_miny;
    double        m_maxx;
    double        m_maxy;
    bool          m_validbounds;

};

//----------------------------------------------------------------------------
// wxCanvasPolyline
//----------------------------------------------------------------------------

class wxCanvasPolyline: public wxCanvasObject
{
public:
    wxCanvasPolyline(int n, wxPoint2DDouble points[]);
    ~wxCanvasPolyline();
    void SetPen(wxPen& pen)    { m_pen = pen; };

    virtual void Recreate();

    virtual void Render(int xabs, int yabs, int clip_x, int clip_y, int clip_width, int clip_height );
    virtual void WriteSVG( wxTextOutputStream &stream );

private:
    void ExtendArea(double x, double y);

    wxPen         m_pen;

    int           m_n;
    wxPoint2DDouble* m_points;

    //bounding box
    double        m_minx;
    double        m_miny;
    double        m_maxx;
    double        m_maxy;
    bool          m_validbounds;

};



//----------------------------------------------------------------------------
// wxCanvasRect
//----------------------------------------------------------------------------

class wxCanvasRect: public wxCanvasObject
{
public:
    wxCanvasRect( double x, double y, double w, double h );
    void SetBrush(wxBrush& brush)  { m_brush = brush; };
    void SetPen(wxPen& pen)        { m_pen = pen; };

    virtual void Recreate();

    virtual void Render(int xabs, int yabs, int clip_x, int clip_y, int clip_width, int clip_height );
    virtual void WriteSVG( wxTextOutputStream &stream );

private:
    wxPen         m_pen;
    wxBrush       m_brush;

    double        m_x;
    double        m_y;
    double        m_width;
    double        m_height;
};

//----------------------------------------------------------------------------
// wxCanvasLine
//----------------------------------------------------------------------------

class wxCanvasLine: public wxCanvasObject
{
public:
    wxCanvasLine( double x1, double y1, double x2, double y2 );
    void SetPen(wxPen& pen)    { m_pen = pen; };

    virtual void Recreate();

    virtual void Render(int xabs, int yabs, int clip_x, int clip_y, int clip_width, int clip_height );
    virtual void WriteSVG( wxTextOutputStream &stream );
    
private:
    wxPen         m_pen;

    double        m_x1;
    double        m_y1;
    double        m_x2;
    double        m_y2;
};

//----------------------------------------------------------------------------
// wxCanvasImage
//----------------------------------------------------------------------------

class wxCanvasImage: public wxCanvasObject
{
public:
    wxCanvasImage( const wxImage &image, double x, double y, double w, double h );
    
    virtual void Recreate();
    
    virtual void Render(int xabs, int yabs, int clip_x, int clip_y, int clip_width, int clip_height );
    virtual void WriteSVG( wxTextOutputStream &stream );
    
private:
    double      m_x;
    double      m_y;
    double      m_width;
    double      m_height;
    
    wxImage     m_image;
#if IMAGE_CANVAS
    wxImage     m_tmp;
#else
    wxBitmap    m_tmp;
#endif
};

//----------------------------------------------------------------------------
// wxCanvasControl
//----------------------------------------------------------------------------

class wxCanvasControl: public wxCanvasObject
{
public:
    wxCanvasControl( wxWindow *control );
    ~wxCanvasControl();
    
    virtual void Recreate();
    
    virtual void Move( int x, int y );
    
private:
    wxWindow     *m_control;
};

//----------------------------------------------------------------------------
// wxCanvasText
//----------------------------------------------------------------------------

class wxCanvasText: public wxCanvasObject
{
public:
    wxCanvasText( const wxString &text, double x, double y, const wxString &foneFile, int size );
    ~wxCanvasText();

    void Recreate();

    virtual void Render(int xabs, int yabs, int clip_x, int clip_y, int clip_width, int clip_height );
    virtual void WriteSVG( wxTextOutputStream &stream );
    
    void SetRGB( unsigned char red, unsigned char green, unsigned char blue );
    void SetFlag( int flag );
    int GetFlag()              { return m_flag; }

private:
    wxString        m_text;
    double          m_x;
    double          m_y;
    unsigned char  *m_alpha;
    void           *m_faceData;
    int             m_flag;
    int             m_red;
    int             m_green;
    int             m_blue;
    wxString        m_fontFileName;
    int             m_size;
};

//----------------------------------------------------------------------------
// wxCanvas
//----------------------------------------------------------------------------

class wxCanvas: public wxScrolledWindow
{
public:
    // constructors and destructors
    wxCanvas( wxWindow *parent, wxWindowID id = -1,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxScrolledWindowStyle );
    virtual ~wxCanvas();

    virtual void SetArea( int width, int height );
    virtual void SetColour( unsigned char red, unsigned char green, unsigned char blue );
    virtual void Update( int x, int y, int width, int height, bool blit = TRUE );
    virtual void UpdateNow();

    virtual void Freeze();
    virtual void Thaw();

    virtual void Prepend( wxCanvasObject* obj );
    virtual void Append( wxCanvasObject* obj );
    virtual void Insert( size_t before, wxCanvasObject* obj );
    virtual void Remove( wxCanvasObject* obj );

    // override these to change your coordiate system ...
    virtual int GetDeviceX( double x );
    virtual int GetDeviceY( double y );
    virtual int GetDeviceWidth( double width );
    virtual int GetDeviceHeight( double height );

    // ... and call this to tell all objets to recreate then
    virtual void Recreate();

#if IMAGE_CANVAS
    inline wxImage *GetBuffer()  { return &m_buffer; }
#else
    inline wxBitmap *GetBuffer() { return &m_buffer; }
    inline wxMemoryDC *GetDC()   { return m_renderDC; }
#endif
    inline int GetBufferX()      { return m_bufferX; }
    inline int GetBufferY()      { return m_bufferY; }
    inline int GetBufferWidth()  { return m_buffer.GetWidth(); }
    inline int GetBufferHeight() { return m_buffer.GetHeight(); }
    
    bool NeedUpdate()            { return m_needUpdate; }
    bool IsFrozen()              { return m_frozen; }

    void BlitBuffer( wxDC &dc );

    void SetCaptureMouse( wxCanvasObject *obj );

    virtual void ScrollWindow( int dx, int dy,
                               const wxRect* rect = (wxRect *) NULL );

private:
#if IMAGE_CANVAS
    wxImage          m_buffer;
#else
    wxBitmap         m_buffer;
    wxMemoryDC      *m_renderDC;
#endif
    int              m_bufferX;
    int              m_bufferY;
    bool             m_needUpdate;
    wxList           m_updateRects;
    wxCanvasObjectGroup* m_root;

    unsigned char    m_green,m_red,m_blue;
    bool             m_frozen;
    wxCanvasObject  *m_lastMouse;
    wxCanvasObject  *m_captureMouse;
    
    int              m_oldDeviceX,m_oldDeviceY;

    friend class wxCanvasObject;

private:
    void OnChar( wxKeyEvent &event );
    void OnPaint( wxPaintEvent &event );
    void OnMouse( wxMouseEvent &event );
    void OnSize( wxSizeEvent &event );
    void OnIdle( wxIdleEvent &event );
    void OnSetFocus( wxFocusEvent &event );
    void OnKillFocus( wxFocusEvent &event );
    void OnEraseBackground( wxEraseEvent &event );

private:
    DECLARE_CLASS(wxCanvas)
    DECLARE_EVENT_TABLE()
};


#endif
    // WXCANVAS

