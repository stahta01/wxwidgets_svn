/////////////////////////////////////////////////////////////////////////////
// Name:      rotate.cpp
// Purpose:   Image rotation test
// Author:    Carlos Moreno
// Modified by:
// Created:   6/2/2000
// RCS-ID:    $Id$
// Copyright: (c) 2000
// Licence:   wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/image.h"

#include <math.h>       // M_PI

class MyApp: public wxApp
{
public:
    virtual bool OnInit();

    const wxImage& GetImage() const { return m_image; }

private:
    wxImage m_image;
};


class MyFrame: public wxFrame
{
public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

    void OnQuit (wxCommandEvent &);
    void OnAngle(wxCommandEvent &);
    void OnMouseLeftUp (wxMouseEvent & event);
    void OnMouseRightUp (wxMouseEvent & event);

private:
    double  m_angle;

    DECLARE_EVENT_TABLE()
};

enum
{
    ID_Quit = 1,
    ID_Angle
};

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU (ID_Quit, MyFrame::OnQuit)
    EVT_MENU (ID_Angle, MyFrame::OnAngle)
    EVT_LEFT_UP (MyFrame::OnMouseLeftUp)
    EVT_RIGHT_UP (MyFrame::OnMouseRightUp)
END_EVENT_TABLE()

IMPLEMENT_APP(MyApp)


bool MyApp::OnInit()
{
    m_image = wxImage("kclub.bmp", wxBITMAP_TYPE_BMP);
    if ( !m_image.Ok() )
    {
        wxLogError("Can't load the test image, please copy it to the "
                   "program directory");
        return FALSE;
    }

    MyFrame *frame = new MyFrame ("wxWindows rotate sample",
                                  wxPoint(20,20), wxSize(600,450));

    frame->SetBackgroundColour (wxColour (0,80,60));

    frame->Show (TRUE);
    SetTopWindow (frame);
    return TRUE;
}

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame((wxFrame *)NULL, -1, title, pos, size)
{
    m_angle = 0.1;

    wxMenu *menuFile = new wxMenu;
    menuFile->Append (ID_Angle, "Set &angle\tCtrl-A");
    menuFile->AppendSeparator();
    menuFile->Append (ID_Quit, "E&xit\tAlt-X");

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append (menuFile, "&File");

    SetMenuBar (menuBar);
}

void MyFrame::OnAngle (wxCommandEvent &)
{
    long degrees = (long)((180*m_angle)/M_PI);
    degrees = wxGetNumberFromUser("Change the image rotation angle",
                                  "Angle in degrees:",
                                  "wxWindows rotate sample",
                                  degrees,
                                  -180, +180,
                                  this);
    m_angle = (degrees * M_PI) / 180.0;
}

void MyFrame::OnQuit (wxCommandEvent &)
{
    Close (TRUE);
}


// Rotate with interpolation and with offset correction
void MyFrame::OnMouseLeftUp (wxMouseEvent & event)
{
    wxPoint offset;
    const wxImage& img = wxGetApp().GetImage();
    wxImage img2 = img.Rotate(m_angle, wxPoint(img.GetWidth()/2, img.GetHeight()/2), TRUE, &offset);

    wxBitmap bmp = img2.ConvertToBitmap ();

    wxClientDC dc (this);
    dc.DrawBitmap (img2.ConvertToBitmap(), event.m_x + offset.x, event.m_y + offset.y);
}

// without interpolation, and without offset correction
void MyFrame::OnMouseRightUp (wxMouseEvent & event)
{
    const wxImage& img = wxGetApp().GetImage();
    wxImage img2 = img.Rotate(m_angle, wxPoint(img.GetWidth()/2, img.GetHeight()/2), FALSE);

    wxBitmap bmp = img2.ConvertToBitmap ();

    wxClientDC dc (this);
    dc.DrawBitmap (bmp, event.m_x, event.m_y);
}
