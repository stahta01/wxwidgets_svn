/*
 * Program: image
 *
 * Author: Robert Roebling
 *
 * Copyright: (C) 1998, Robert Roebling
 *
 */

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/image.h"

#include "wx/file.h"

#include "smile.xbm"
#include "smile.xpm"

// derived classes

class MyFrame;
class MyApp;

// MyCanvas

class MyCanvas: public wxScrolledWindow
{
public:
    MyCanvas() {};
    MyCanvas( wxWindow *parent, wxWindowID, const wxPoint &pos, const wxSize &size );
    ~MyCanvas();
    void OnPaint( wxPaintEvent &event );
    void CreateAntiAliasedBitmap();

    wxBitmap  *my_horse_png;
    wxBitmap  *my_horse_jpeg;
    wxBitmap  *my_horse_gif;
    wxBitmap  *my_horse_bmp;
    wxBitmap  *my_horse_pcx;
    wxBitmap  *my_horse_pnm;
    wxBitmap  *my_horse_tiff;
    wxBitmap  *my_smile_xbm;
    wxBitmap  *my_square;
    wxBitmap  *my_anti;

protected:
    wxBitmap m_bmpSmileXpm;
    wxIcon   m_iconSmileXpm;

private:
    DECLARE_DYNAMIC_CLASS(MyCanvas)
    DECLARE_EVENT_TABLE()
};

// MyFrame

class MyFrame: public wxFrame
{
public:
    MyFrame();

    void OnAbout( wxCommandEvent &event );
    void OnQuit( wxCommandEvent &event );

    MyCanvas         *m_canvas;

private:
    DECLARE_DYNAMIC_CLASS(MyFrame)
    DECLARE_EVENT_TABLE()
};

// MyApp

class MyApp: public wxApp
{
public:
    virtual bool OnInit();
};

// main program

IMPLEMENT_APP(MyApp)

// MyCanvas

IMPLEMENT_DYNAMIC_CLASS(MyCanvas, wxScrolledWindow)

BEGIN_EVENT_TABLE(MyCanvas, wxScrolledWindow)
  EVT_PAINT(MyCanvas::OnPaint)
END_EVENT_TABLE()

MyCanvas::MyCanvas( wxWindow *parent, wxWindowID id,
                    const wxPoint &pos, const wxSize &size )
        : wxScrolledWindow( parent, id, pos, size, wxSUNKEN_BORDER )
#if !defined(__WINDOWS__) || wxUSE_XPM_IN_MSW
          , m_bmpSmileXpm((const char **) smile_xpm)
          , m_iconSmileXpm((const char **) smile_xpm)
#endif
{
    my_horse_png = (wxBitmap*) NULL;
    my_horse_jpeg = (wxBitmap*) NULL;
    my_horse_gif = (wxBitmap*) NULL;
    my_horse_bmp = (wxBitmap*) NULL;
    my_horse_pcx = (wxBitmap*) NULL;
    my_horse_pnm = (wxBitmap*) NULL;
    my_horse_tiff = (wxBitmap*) NULL;
    my_smile_xbm = (wxBitmap*) NULL;
    my_square = (wxBitmap*) NULL;
    my_anti = (wxBitmap*) NULL;

    SetBackgroundColour(* wxWHITE);

    wxBitmap bitmap( 100, 100 );

    wxMemoryDC dc;
    dc.SelectObject( bitmap );
    dc.SetBrush( wxBrush( "orange", wxSOLID ) );
    dc.SetPen( *wxBLACK_PEN );
    dc.DrawRectangle( 0, 0, 100, 100 );
    dc.SetBrush( *wxWHITE_BRUSH );
    dc.DrawRectangle( 20, 20, 60, 60 );
    dc.SelectObject( wxNullBitmap );

    // try to find the directory with our images
    wxString dir;
    if ( wxFile::Exists("./horse.png") )
        dir = "./";
    else if ( wxFile::Exists("../horse.png") )
        dir = "../";
    else
        wxLogWarning("Can't find image files in either '.' or '..'!");

    wxImage image( bitmap );

#if wxUSE_LIBPNG
    if ( !image.SaveFile( dir + wxString("test.png"), wxBITMAP_TYPE_PNG ) )
        wxLogError("Can't save file");

    if ( !image.LoadFile( dir + wxString("horse.png"), wxBITMAP_TYPE_PNG ) )
        wxLogError("Can't load PNG image");
    else
        my_horse_png = new wxBitmap( image.ConvertToBitmap() );
#endif // wxUSE_LIBPNG

#if wxUSE_LIBJPEG
    if ( !image.LoadFile( dir + wxString("horse.jpg") ) )
        wxLogError("Can't load JPG image");
    else
        my_horse_jpeg = new wxBitmap( image.ConvertToBitmap() );
#endif // wxUSE_LIBJPEG

#if wxUSE_GIF
    if ( !image.LoadFile( dir + wxString("horse.gif") ) )
        wxLogError("Can't load GIF image");
    else
        my_horse_gif = new wxBitmap( image.ConvertToBitmap() );
#endif

#if wxUSE_PCX
    if ( !image.LoadFile( dir + wxString("horse.pcx"), wxBITMAP_TYPE_PCX ) )
        wxLogError("Can't load PCX image");
    else
        my_horse_pcx = new wxBitmap( image.ConvertToBitmap() );

    image.LoadFile( dir + wxString("test.pcx") );
    my_square = new wxBitmap( image.ConvertToBitmap() );
#endif

    if ( !image.LoadFile( dir + wxString("horse.bmp"), wxBITMAP_TYPE_BMP ) )
        wxLogError("Can't load BMP image");
    else
        my_horse_bmp = new wxBitmap( image.ConvertToBitmap() );

#if wxUSE_PNM
    if ( !image.LoadFile( dir + wxString("horse.pnm"), wxBITMAP_TYPE_PNM ) )
        wxLogError("Can't load PNM image");
    else
        my_horse_pnm = new wxBitmap( image.ConvertToBitmap() );
#endif

#if wxUSE_LIBTIFF
    if ( !image.LoadFile( dir + wxString("horse.tif"), wxBITMAP_TYPE_TIF ) )
        wxLogError("Can't load TIFF image");
    else
        my_horse_tiff = new wxBitmap( image.ConvertToBitmap() );
#endif

    CreateAntiAliasedBitmap();

    my_smile_xbm = new wxBitmap( (const char*)smile_bits, smile_width,
                                 smile_height, 1 );

#if !defined(__WINDOWS__) || wxUSE_XPM_IN_MSW
    // demonstrates XPM automatically using the mask when saving
    if ( m_bmpSmileXpm.Ok() )
        m_bmpSmileXpm.SaveFile("saved.xpm", wxBITMAP_TYPE_XPM);
#endif
}

MyCanvas::~MyCanvas()
{
    delete my_horse_pnm;
    delete my_horse_png;
    delete my_horse_jpeg;
    delete my_horse_gif;
    delete my_horse_bmp;
    delete my_horse_pcx;
    delete my_horse_tiff;
    delete my_smile_xbm;
    delete my_square;
    delete my_anti;
}

void MyCanvas::OnPaint( wxPaintEvent &WXUNUSED(event) )
{
    wxPaintDC dc( this );
    PrepareDC( dc );

    dc.DrawText( "Loaded image", 30, 10 );
    if (my_square && my_square->Ok()) dc.DrawBitmap( *my_square, 30, 30 );

    dc.DrawText( "Drawn directly", 150, 10 );
    dc.SetBrush( wxBrush( "orange", wxSOLID ) );
    dc.SetPen( *wxBLACK_PEN );
    dc.DrawRectangle( 150, 30, 100, 100 );
    dc.SetBrush( *wxWHITE_BRUSH );
    dc.DrawRectangle( 170, 50, 60, 60 );

    if (my_anti && my_anti->Ok())
        dc.DrawBitmap( *my_anti, 280, 30 );

    dc.DrawText( "PNG handler", 30, 135 );
    if (my_horse_png && my_horse_png->Ok())
    {
        dc.DrawBitmap( *my_horse_png, 30, 150 );
        wxRect rect(0,0,100,100);
        wxBitmap sub( my_horse_png->GetSubBitmap(rect) );
        dc.DrawText( "GetSubBitmap()", 280, 190 );
        dc.DrawBitmap( sub, 280, 210 );
    }

    dc.DrawText( "JPEG handler", 30, 365 );
    if (my_horse_jpeg && my_horse_jpeg->Ok())
        dc.DrawBitmap( *my_horse_jpeg, 30, 380 );

    dc.DrawText( "GIF handler", 30, 595 );
    if (my_horse_gif && my_horse_gif->Ok())
        dc.DrawBitmap( *my_horse_gif, 30, 610 );

    dc.DrawText( "PCX handler", 30, 825 );
    if (my_horse_pcx && my_horse_pcx->Ok())
        dc.DrawBitmap( *my_horse_pcx, 30, 840 );

    dc.DrawText( "BMP handler", 30, 1055 );
    if (my_horse_bmp && my_horse_bmp->Ok())
        dc.DrawBitmap( *my_horse_bmp, 30, 1070 );

    dc.DrawText( "PNM handler", 30, 1285 );
    if (my_horse_pnm && my_horse_pnm->Ok())
        dc.DrawBitmap( *my_horse_pnm, 30, 1300 );

    dc.DrawText( "TIFF handler", 30, 1515 );
    if (my_horse_tiff && my_horse_tiff->Ok())
        dc.DrawBitmap( *my_horse_tiff, 30, 1530 );

    if (my_smile_xbm && my_smile_xbm->Ok())
    {
        dc.DrawText( "XBM bitmap", 30, 1745 );
        dc.SetTextForeground( "RED" );
        dc.SetTextBackground( "GREEN" );
        dc.DrawBitmap( *my_smile_xbm, 30, 1760 );

        dc.DrawText( "After wxImage conversion", 150, 1745 );
        wxImage i( *my_smile_xbm );
        i.SetMaskColour( 0,0,0 );
        i.Replace( 255,255,255,
               wxRED_PEN->GetColour().Red(),
               wxRED_PEN->GetColour().Green(),
               wxRED_PEN->GetColour().Blue() );
        dc.DrawBitmap( i.ConvertToBitmap(), 150, 1760, TRUE );
    }
    dc.SetTextForeground( "BLACK" );

    wxBitmap mono( 60,50,1 );
    wxMemoryDC memdc;
    memdc.SelectObject( mono );
    memdc.SetPen( *wxBLACK_PEN );
    memdc.SetBrush( *wxWHITE_BRUSH );
    memdc.DrawRectangle( 0,0,60,50 );
    memdc.SetTextForeground( *wxBLACK );
    memdc.DrawText( "Hi!", 5, 5 );
    memdc.SetBrush( *wxBLACK_BRUSH );
    memdc.DrawRectangle( 33,5,20,20 );
    memdc.SetPen( *wxRED_PEN );
    memdc.DrawLine( 5, 42, 50, 42 );
    memdc.SelectObject( wxNullBitmap );
    if (mono.Ok())
    {
        dc.DrawText( "Mono bitmap", 30, 1845 );
        dc.SetTextForeground( "RED" );
        dc.SetTextBackground( "GREEN" );
        dc.DrawBitmap( mono, 30, 1860 );

        dc.SetTextForeground( "BLACK" );
        dc.DrawText( "After wxImage conversion", 150, 1845 );
        dc.SetTextForeground( "RED" );
        wxImage i( mono );
        i.SetMaskColour( 255,255,255 );
        i.Replace( 0,0,0,
               wxRED_PEN->GetColour().Red(),
               wxRED_PEN->GetColour().Green(),
               wxRED_PEN->GetColour().Blue() );
        dc.DrawBitmap( i.ConvertToBitmap(), 150, 1860, TRUE );
        dc.SetTextForeground( "BLACK" );
    }

    dc.DrawText("XPM bitmap", 30, 1950);
    if ( m_bmpSmileXpm.Ok() )
    {
        dc.DrawBitmap(m_bmpSmileXpm, 30, 1975, TRUE);
    }

    dc.DrawText("XPM icon", 150, 1950);
    if ( m_iconSmileXpm.Ok() )
    {
        dc.DrawIcon(m_iconSmileXpm, 150, 1975);
    }
}

void MyCanvas::CreateAntiAliasedBitmap()
{
  wxBitmap bitmap( 300, 300 );

  wxMemoryDC dc;

  dc.SelectObject( bitmap );

  dc.Clear();

  dc.SetFont( wxFont( 24, wxDECORATIVE, wxNORMAL, wxNORMAL) );
  dc.SetTextForeground( "RED" );
  dc.DrawText( "This is anti-aliased Text.", 20, 20 );
  dc.DrawText( "And a Rectangle.", 20, 60 );

  dc.SetBrush( *wxRED_BRUSH );
  dc.SetPen( *wxTRANSPARENT_PEN );
  dc.DrawRoundedRectangle( 20, 100, 200, 180, 20 );

  wxImage original( bitmap );
  wxImage anti( 150, 150 );

  /* This is quite slow, but safe. Use wxImage::GetData() for speed instead. */

  for (int y = 1; y < 149; y++)
    for (int x = 1; x < 149; x++)
    {
       int red = original.GetRed( x*2, y*2 ) +
                 original.GetRed( x*2-1, y*2 ) +
                 original.GetRed( x*2, y*2+1 ) +
                 original.GetRed( x*2+1, y*2+1 );
       red = red/4;

       int green = original.GetGreen( x*2, y*2 ) +
                   original.GetGreen( x*2-1, y*2 ) +
                   original.GetGreen( x*2, y*2+1 ) +
                   original.GetGreen( x*2+1, y*2+1 );
       green = green/4;

       int blue = original.GetBlue( x*2, y*2 ) +
                  original.GetBlue( x*2-1, y*2 ) +
                  original.GetBlue( x*2, y*2+1 ) +
                  original.GetBlue( x*2+1, y*2+1 );
       blue = blue/4;
       anti.SetRGB( x, y, red, green, blue );
    }
  my_anti = new wxBitmap( anti.ConvertToBitmap() );
}

// MyFrame

const int ID_QUIT  = 108;
const int ID_ABOUT = 109;

IMPLEMENT_DYNAMIC_CLASS( MyFrame, wxFrame )

BEGIN_EVENT_TABLE(MyFrame,wxFrame)
  EVT_MENU    (ID_ABOUT, MyFrame::OnAbout)
  EVT_MENU    (ID_QUIT,  MyFrame::OnQuit)
END_EVENT_TABLE()

MyFrame::MyFrame()
       : wxFrame( (wxFrame *)NULL, -1, "wxImage sample",
                  wxPoint(20,20), wxSize(470,360) )
{
  wxMenu *file_menu = new wxMenu();
  file_menu->Append( ID_ABOUT, "&About...");
  file_menu->Append( ID_QUIT, "E&xit");

  wxMenuBar *menu_bar = new wxMenuBar();
  menu_bar->Append(file_menu, "&File");

  SetMenuBar( menu_bar );

  CreateStatusBar(2);
  int widths[] = { -1, 100 };
  SetStatusWidths( 2, widths );

  m_canvas = new MyCanvas( this, -1, wxPoint(0,0), wxSize(10,10) );

  // 500 width * 2100 height
  m_canvas->SetScrollbars( 10, 10, 50, 210 );
}

void MyFrame::OnQuit( wxCommandEvent &WXUNUSED(event) )
{
  Close( TRUE );
}

void MyFrame::OnAbout( wxCommandEvent &WXUNUSED(event) )
{
  (void)wxMessageBox( "wxImage demo\n"
                      "Robert Roebling (c) 1998,2000",
                      "About wxImage Demo", wxICON_INFORMATION | wxOK );
}

//-----------------------------------------------------------------------------
// MyApp
//-----------------------------------------------------------------------------

bool MyApp::OnInit()
{
#if wxUSE_LIBPNG
  wxImage::AddHandler( new wxPNGHandler );
#endif

#if wxUSE_LIBJPEG
  wxImage::AddHandler( new wxJPEGHandler );
#endif

#if wxUSE_LIBTIFF
  wxImage::AddHandler( new wxTIFFHandler );
#endif

#if wxUSE_GIF
  wxImage::AddHandler( new wxGIFHandler );
#endif

#if wxUSE_PCX
  wxImage::AddHandler( new wxPCXHandler );
#endif

#if wxUSE_PNM
  wxImage::AddHandler( new wxPNMHandler );
#endif

  wxFrame *frame = new MyFrame();
  frame->Show( TRUE );

  return TRUE;
}

