///////////////////////////////////////////////////////////////////////////////
// Name:        statbar.cpp
// Purpose:     native implementation of wxStatusBar (optional)
// Author:      Stefan Csomor
// Modified by: 
// Created:     1998-01-01
// RCS-ID:      $Id$
// Copyright:   (c) 1998 Stefan Csomor
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "statbrma.h"
#endif

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include "wx/statusbr.h"
#include "wx/dc.h"
#include "wx/dcclient.h"

BEGIN_EVENT_TABLE(wxStatusBarMac, wxStatusBarGeneric)
    EVT_PAINT(wxStatusBarMac::OnPaint)
END_EVENT_TABLE()

#ifdef __WXMAC__
#include "wx/mac/private.h"
#endif

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxStatusBarMac class
// ----------------------------------------------------------------------------

wxStatusBarMac::wxStatusBarMac()
{
    SetParent(NULL);
}

wxStatusBarMac::~wxStatusBarMac()
{
}

bool wxStatusBarMac::Create(wxWindow *parent, wxWindowID id,
                            long style ,
                            const wxString& name)
{
    if( !wxStatusBarGeneric::Create( parent , id , style , name ) )
        return FALSE ;
    
    m_macBackgroundBrush.MacSetTheme( kThemeBrushDialogBackgroundActive ) ;    
    
    return TRUE ;
}

void wxStatusBarMac::DrawFieldText(wxDC& dc, int i)
{
    int leftMargin = 2;
    int w, h ;
    GetSize( &w , &h ) ;
    wxRect rect;
    GetFieldRect(i, rect);
    
    if ( !IsWindowHilited( MAC_WXHWND( MacGetTopLevelWindowRef() ) ) )
    {
        dc.SetTextForeground( wxColour( 0x80 , 0x80 , 0x80 ) ) ;
    }
    
    wxString text(GetStatusText(i));
    
    long x, y;
    
    dc.GetTextExtent(text, &x, &y);
    
    int xpos = rect.x + leftMargin + 1 ;
    int ypos = 1 ;
    
    dc.SetClippingRegion(rect.x, 0, rect.width, h);
    
    dc.DrawText(text, xpos, ypos);
    
    dc.DestroyClippingRegion();
}

void wxStatusBarMac::DrawField(wxDC& dc, int i)
{
    DrawFieldText(dc, i);
}

void wxStatusBarMac::SetStatusText(const wxString& text, int number)
{
    wxCHECK_RET( (number >= 0) && (number < m_nFields),
        _T("invalid status bar field index") );
    
    m_statusStrings[number] = text;
    wxRect rect;
    GetFieldRect(number, rect);
    int w, h ;
    GetSize( &w , &h ) ;
    rect.y=0;
    rect.height = h ;
    Refresh( TRUE , &rect ) ;
    Update();
}

void wxStatusBarMac::OnPaint(wxPaintEvent& WXUNUSED(event) )
{
  	wxPaintDC dc(this);
  	dc.Clear() ;

    int major,minor;
    wxGetOsVersion( &major, &minor );
    int w, h ;
    GetSize( &w , &h ) ;

	if ( IsWindowHilited( MAC_WXHWND( MacGetTopLevelWindowRef() ) ) )
	{
		wxPen white( wxWHITE , 1 , wxSOLID ) ;
        if (major >= 10) 
        {
            //Finder statusbar border color: (Project builder similar is 9B9B9B)
            dc.SetPen(wxPen(wxColour(0xB1,0xB1,0xB1),1,wxSOLID));  
        }
        else
        {
            wxPen black( wxBLACK , 1 , wxSOLID ) ;
            dc.SetPen(black);
    	}
		dc.DrawLine(0, 0 ,
		       w , 0);
		dc.SetPen(white);
		dc.DrawLine(0, 1 ,
		       w , 1);
	}
	else
	{
        if (major >= 10) 
            //Finder statusbar border color: (Project builder similar is 9B9B9B)
            dc.SetPen(wxPen(wxColour(0xB1,0xB1,0xB1),1,wxSOLID)); 
        else
            dc.SetPen(wxPen(wxColour(0x80,0x80,0x80),1,wxSOLID));

		dc.DrawLine(0, 0 ,
		       w , 0);
	}

	int i;
	if ( GetFont().Ok() )
		dc.SetFont(GetFont());
	dc.SetBackgroundMode(wxTRANSPARENT);

	for ( i = 0; i < m_nFields; i ++ )
		DrawField(dc, i);
}
