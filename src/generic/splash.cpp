/////////////////////////////////////////////////////////////////////////////
// Name:        splash.cpp
// Purpose:     wxSplashScreen class
// Author:      Julian Smart
// Modified by:
// Created:     28/6/2000
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "splash.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#if wxUSE_SPLASH

#ifndef WX_PRECOMP
#endif

#include "wx/splash.h"

/*
 * wxSplashScreen
 */

#define wxSPLASH_TIMER_ID 9999

BEGIN_EVENT_TABLE(wxSplashScreen, wxFrame)
    EVT_TIMER(wxSPLASH_TIMER_ID, wxSplashScreen::OnNotify)
    EVT_CLOSE(wxSplashScreen::OnCloseWindow)
END_EVENT_TABLE()

wxSplashScreen::wxSplashScreen(const wxBitmap& bitmap, long splashStyle, int milliseconds, wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style):
    wxFrame(parent, id, wxEmptyString, pos, size, style)
{
    m_window = NULL;
    m_splashStyle = splashStyle;
    m_milliseconds = milliseconds;

    m_window = new wxSplashScreenWindow(bitmap, this, -1, pos, size, wxNO_BORDER);

    // For some reason, we need to make the client size a couple of pixels
    // bigger for all of the bitmap to show.
    // Or do we?
#ifdef __WXMSW__
    int fudge = 0;
#else
    int fudge = 0;
#endif
    SetClientSize(bitmap.GetWidth()+fudge, bitmap.GetHeight()+fudge);

    if (m_splashStyle & wxSPLASH_CENTRE_ON_PARENT)
        CentreOnParent();
    else if (m_splashStyle & wxSPLASH_CENTRE_ON_SCREEN)
        CentreOnScreen();

    if (m_splashStyle & wxSPLASH_TIMEOUT)
    {
        m_timer.SetOwner(this, wxSPLASH_TIMER_ID);
        m_timer.Start(milliseconds, TRUE);
    }

    Show(TRUE);
    m_window->SetFocus();
    wxYield(); // Without this, you see a blank screen for an instant
}

wxSplashScreen::~wxSplashScreen()
{
    m_timer.Stop();
}

void wxSplashScreen::OnNotify(wxTimerEvent& WXUNUSED(event))
{
    Close(TRUE);
}

void wxSplashScreen::OnCloseWindow(wxCloseEvent& WXUNUSED(event))
{
    m_timer.Stop();
    this->Destroy();
}

/*
 * wxSplashScreenWindow
 */

BEGIN_EVENT_TABLE(wxSplashScreenWindow, wxWindow)
    //EVT_PAINT(wxSplashScreenWindow::OnPaint)
    EVT_ERASE_BACKGROUND(wxSplashScreenWindow::OnEraseBackground)
    EVT_CHAR(wxSplashScreenWindow::OnChar)
    EVT_MOUSE_EVENTS(wxSplashScreenWindow::OnMouseEvent)
END_EVENT_TABLE()

wxSplashScreenWindow::wxSplashScreenWindow(const wxBitmap& bitmap, wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style):
    wxWindow(parent, id, pos, size, style)
{
    m_bitmap = bitmap;
}

void wxSplashScreenWindow::OnPaint(wxPaintEvent& WXUNUSED(event))
{
    wxPaintDC dc(this);
    if (m_bitmap.Ok())
        dc.DrawBitmap(m_bitmap, 0, 0);
}

static void wxDrawSplashBitmap(wxDC& dc, const wxBitmap& bitmap, int WXUNUSED(x), int WXUNUSED(y))
{
    wxMemoryDC dcMem;

#ifndef __WXGTK__
    bool hiColour = (wxDisplayDepth() >= 16) ;

    if (bitmap.GetPalette() && !hiColour)
    {
        dc.SetPalette(* bitmap.GetPalette());
        dcMem.SetPalette(* bitmap.GetPalette());
    }
#endif

    dcMem.SelectObject(bitmap);
    dc.Blit(0, 0, bitmap.GetWidth(), bitmap.GetHeight(), & dcMem, 0, 0);
    dcMem.SelectObject(wxNullBitmap);

#ifndef __WXGTK__
    if (bitmap.GetPalette() && !hiColour)
    {
        dc.SetPalette(wxNullPalette);
        dcMem.SetPalette(wxNullPalette);
    }
#endif
}

void wxSplashScreenWindow::OnEraseBackground(wxEraseEvent& event)
{
    if (event.GetDC())
    {
        if (m_bitmap.Ok())
        {
            wxDrawSplashBitmap(* event.GetDC(), m_bitmap, 0, 0);
        }
    }
    else
    {
        wxClientDC dc(this);
        if (m_bitmap.Ok())
        {
            wxDrawSplashBitmap(dc, m_bitmap, 0, 0);
        }
    }
}

void wxSplashScreenWindow::OnMouseEvent(wxMouseEvent& event)
{
    if (event.LeftDown() || event.RightDown())
        GetParent()->Close(TRUE);
}

void wxSplashScreenWindow::OnChar(wxKeyEvent& WXUNUSED(event))
{
    GetParent()->Close(TRUE);
}

#endif // wxUSE_SPLASH
