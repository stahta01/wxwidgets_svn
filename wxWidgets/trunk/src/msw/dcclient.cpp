/////////////////////////////////////////////////////////////////////////////
// Name:        dcclient.cpp
// Purpose:     wxClientDC class
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ===========================================================================
// declarations
// ===========================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#ifdef __GNUG__
    #pragma implementation "dcclient.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/string.h"
#include "wx/log.h"
#include "wx/window.h"

#include "wx/msw/private.h"

#include "wx/dcclient.h"

// ----------------------------------------------------------------------------
// array/list types
// ----------------------------------------------------------------------------

struct WXDLLEXPORT wxPaintDCInfo
{
    wxPaintDCInfo(wxWindow *win, wxDC *dc)
    {
        hwnd = win->GetHWND();
        hdc = dc->GetHDC();
        count = 1;
    }

    WXHWND    hwnd;       // window for this DC
    WXHDC     hdc;        // the DC handle
    size_t    count;      // usage count
};

#include "wx/arrimpl.cpp"

WX_DEFINE_OBJARRAY(wxArrayDCInfo);

// ----------------------------------------------------------------------------
// macros
// ----------------------------------------------------------------------------

#if !USE_SHARED_LIBRARY
    IMPLEMENT_DYNAMIC_CLASS(wxWindowDC, wxDC)
    IMPLEMENT_DYNAMIC_CLASS(wxClientDC, wxWindowDC)
    IMPLEMENT_DYNAMIC_CLASS(wxPaintDC, wxWindowDC)
#endif

// ----------------------------------------------------------------------------
// global variables
// ----------------------------------------------------------------------------

static PAINTSTRUCT g_paintStruct;

#ifdef __WXDEBUG__
    // a global variable which we check to verify that wxPaintDC are only
    // created in resopnse to WM_PAINT message - doing this from elsewhere is a
    // common programming error among wxWindows programmers and might lead to
    // very subtle and difficult to debug refresh/repaint bugs.
    extern bool g_isPainting = FALSE;
#endif // __WXDEBUG__

// ===========================================================================
// implementation
// ===========================================================================

// ----------------------------------------------------------------------------
// wxWindowDC
// ----------------------------------------------------------------------------

wxWindowDC::wxWindowDC()
{
  m_canvas = NULL;
}

wxWindowDC::wxWindowDC(wxWindow *the_canvas)
{
  m_canvas = the_canvas;
  m_hDC = (WXHDC) ::GetWindowDC(GetWinHwnd(the_canvas) );
  m_hDCCount++;

  SetBackground(wxBrush(m_canvas->GetBackgroundColour(), wxSOLID));
}

wxWindowDC::~wxWindowDC()
{
  if (m_canvas && m_hDC)
  {
    SelectOldObjects(m_hDC);

    ::ReleaseDC(GetWinHwnd(m_canvas), GetHdc());
    m_hDC = 0;
  }

  m_hDCCount--;
}

// ----------------------------------------------------------------------------
// wxClientDC
// ----------------------------------------------------------------------------

wxClientDC::wxClientDC()
{
  m_canvas = NULL;
}

wxClientDC::wxClientDC(wxWindow *the_canvas)
{
  m_canvas = the_canvas;
  m_hDC = (WXHDC) ::GetDC(GetWinHwnd(the_canvas));

  SetBackground(wxBrush(m_canvas->GetBackgroundColour(), wxSOLID));
}

wxClientDC::~wxClientDC()
{
  if ( m_canvas && GetHdc() )
  {
    SelectOldObjects(m_hDC);

    ::ReleaseDC(GetWinHwnd(m_canvas), GetHdc());
    m_hDC = 0;
  }
}

// ----------------------------------------------------------------------------
// wxPaintDC
// ----------------------------------------------------------------------------

// VZ: initial implementation (by JACS) only remembered the last wxPaintDC
//     created and tried to reuse - this was supposed to take care of a
//     situation when a derived class OnPaint() calls base class OnPaint()
//     because in this case ::BeginPaint() shouldn't be called second time.
//
//     I'm not sure how useful this is, however we must remember the HWND
//     associated with the last HDC as well - otherwise we may (and will!) try
//     to reuse the HDC for another HWND which is a nice recipe for disaster.
//
//     So we store a list of windows for which we already have the DC and not
//     just one single hDC. This seems to work, but I'm really not sure about
//     the usefullness of the whole idea - IMHO it's much better to not call
//     base class OnPaint() at all, or, if we realyl want to allow it, add a
//     "wxPaintDC *" parameter to wxPaintEvent which should be used if it's
//     !NULL instead of creating a new DC.

wxArrayDCInfo wxPaintDC::ms_cache;

wxPaintDC::wxPaintDC()
{
    m_canvas = NULL;
    m_hDC = 0;
}

wxPaintDC::wxPaintDC(wxWindow *canvas)
{
    wxCHECK_RET( canvas, "NULL canvas in wxPaintDC ctor" );

#ifdef __WXDEBUG__
    if ( !g_isPainting )
    {
        wxFAIL_MSG( _T("wxPaintDC may be created only in EVT_PAINT handler!") );

        return;
    }
#endif // __WXDEBUG__

    m_canvas = canvas;

    // do we have a DC for this window in the cache?
    wxPaintDCInfo *info = FindInCache();
    if ( info )
    {
        m_hDC = info->hdc;
        info->count++;
    }
    else // not in cache, create a new one
    {
        m_hDC = (WXHDC)::BeginPaint(GetWinHwnd(m_canvas), &g_paintStruct);
        ms_cache.Add(new wxPaintDCInfo(m_canvas, this));
    }

    SetBackground(wxBrush(m_canvas->GetBackgroundColour(), wxSOLID));
}

wxPaintDC::~wxPaintDC()
{
    if ( m_hDC )
    {
        size_t index;
        wxPaintDCInfo *info = FindInCache(&index);

        wxCHECK_RET( info, _T("existing DC should have a cache entry") );

        if ( !--info->count )
        {
            ::EndPaint(GetWinHwnd(m_canvas), &g_paintStruct);

            ms_cache.Remove(index);
        }
        //else: cached DC entry is still in use
    }
}

wxPaintDCInfo *wxPaintDC::FindInCache(size_t *index) const
{
    wxPaintDCInfo *info = NULL;
    size_t nCache = ms_cache.GetCount();
    for ( size_t n = 0; n < nCache; n++ )
    {
        info = &ms_cache[n];
        if ( info->hwnd == m_canvas->GetHWND() )
        {
            if ( index )
                *index = n;
            break;
        }
    }

    return info;
}
