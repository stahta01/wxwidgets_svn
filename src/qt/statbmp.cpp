/////////////////////////////////////////////////////////////////////////////
// Name:        src/qt/statbmp.cpp
// Author:      Peter Most
// Copyright:   (c) Peter Most
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/statbmp.h"

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
    Create( parent, id, label, pos, size, style, name );
}

bool wxStaticBitmap::Create( wxWindow *parent,
             wxWindowID id,
             const wxBitmap& label,
             const wxPoint& pos,
             const wxSize& size,
             long style,
             const wxString& name)
{
    m_qtWindow = m_qtLabel = new QLabel( parent->GetHandle() );
    SetBitmap( label );

    return QtCreateControl( parent, id, pos, size, style, wxDefaultValidator, name );
}

static void SetPixmap( QLabel *label, const QPixmap *pixMap )
{
    if ( pixMap != NULL )
        label->setPixmap( *pixMap );
}

void wxStaticBitmap::SetIcon(const wxIcon& icon)
{
    SetPixmap( m_qtLabel, icon.GetHandle() );
}

void wxStaticBitmap::SetBitmap(const wxBitmap& bitmap)
{
    SetPixmap( m_qtLabel, bitmap.GetHandle() );
}

wxBitmap wxStaticBitmap::GetBitmap() const
{
    return wxBitmap( *m_qtLabel->pixmap() );
}

wxIcon wxStaticBitmap::GetIcon() const
{
    wxIcon icon;
    icon.CopyFromBitmap( GetBitmap() );
    return icon;
}

QLabel *wxStaticBitmap::GetHandle() const
{
    return m_qtLabel;
}
