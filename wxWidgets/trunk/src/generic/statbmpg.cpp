///////////////////////////////////////////////////////////////////////////////
// Name:        src/generic/statbmp.cpp
// Purpose:     wxGenericStaticBitmap 
// Author:      Marcin Wojdyr, Stefan Csomor
// Created:     2008-06-16
// RCS-ID:      $Id$
// Copyright:   wxWidgets developers
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#if wxUSE_STATBMP

#include "wx/generic/statbmpg.h"


IMPLEMENT_DYNAMIC_CLASS(wxGenericStaticBitmap, wxStaticBitmapBase)

bool wxGenericStaticBitmap::Create(wxWindow *parent, wxWindowID id, 
                                   const wxBitmap& bitmap,
                                   const wxPoint& pos, const wxSize& size,
                                   long style, const wxString& name)
{
    if (! wxControl::Create(parent, id, pos, size, style, 
                            wxDefaultValidator, name))
        return false;
    SetBitmap(bitmap);
    Connect(wxEVT_PAINT, wxPaintEventHandler(wxGenericStaticBitmap::OnPaint));
    return true;
}

void wxGenericStaticBitmap::OnPaint(wxPaintEvent& WXUNUSED(event))
{
    wxPaintDC dc(this);
    PrepareDC(dc);
    if (m_bitmap.Ok())
        dc.DrawBitmap(m_bitmap, 0, 0, true);
}

#endif // wxUSE_STATBMP

