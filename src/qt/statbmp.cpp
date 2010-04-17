/////////////////////////////////////////////////////////////////////////////
// Name:        src/qt/statbmp.cpp
// Author:      Peter Most
// Id:          $Id$
// Copyright:   (c) Peter Most
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/statbmp.h"

IMPLEMENT_DYNAMIC_CLASS( wxStaticBitmap, wxStaticBitmapBase )

wxStaticBitmap::wxStaticBitmap()
{
}

wxStaticBitmap::wxStaticBitmap( wxWindow *parent,
                wxWindowID id,
                const wxBitmap& label,
                const wxPoint& pos,
                const wxSize& size,
                long style,
                const wxString& name)
{
}

bool wxStaticBitmap::Create( wxWindow *parent,
             wxWindowID id,
             const wxBitmap& label,
             const wxPoint& pos,
             const wxSize& size,
             long style,
             const wxString& name)
{
    return false;
}

void wxStaticBitmap::SetIcon(const wxIcon& icon)
{
}

void wxStaticBitmap::SetBitmap(const wxBitmap& bitmap)
{
}

wxBitmap wxStaticBitmap::GetBitmap() const
{
    return wxBitmap();
}

wxIcon wxStaticBitmap::GetIcon() const
{
    return wxIcon();
}

