/////////////////////////////////////////////////////////////////////////////
// Name:        stattext.cpp
// Purpose:     wxStaticText
// Author:      Stefan Csomor
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id$
// Copyright:   (c) Stefan Csomor
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#if wxUSE_STATTEXT

#include "wx/app.h"
#include "wx/stattext.h"
#include "wx/notebook.h"
#include "wx/tabctrl.h"
#include "wx/dc.h"
#include "wx/dcclient.h"
#include "wx/utils.h"
#include "wx/settings.h"

#include "wx/mac/uma.h"

#include <stdio.h>

IMPLEMENT_DYNAMIC_CLASS(wxStaticText, wxControl)


bool wxStaticText::Create( wxWindow *parent,
    wxWindowID id,
    const wxString& label,
    const wxPoint& pos,
    const wxSize& size,
    long style,
    const wxString& name )
{
    m_macIsUserPane = false;

    m_label = wxStripMenuCodes( label );

    if ( !wxControl::Create( parent, id, pos, size, style, wxDefaultValidator, name ) )
        return false;

    Rect bounds = wxMacGetBoundsForControl( this, pos, size );
    wxMacCFStringHolder str( m_label, m_font.GetEncoding() );

    m_peer = new wxMacControl( this );
    OSStatus err = CreateStaticTextControl(
        MAC_WXHWND(parent->MacGetTopLevelWindowRef()),
        &bounds, str, NULL, m_peer->GetControlRefAddr() );
    verify_noerr( err );

	if (  ( style & wxST_DOTS_END ) || ( style & wxST_DOTS_MIDDLE ) )
    {
        TruncCode tCode = truncEnd ;
        if ( style & wxST_DOTS_MIDDLE )
            tCode = truncMiddle ;
        
        err = m_peer->SetData(kControlStaticTextTruncTag,tCode) ;
        err = m_peer->SetData(kControlStaticTextIsMultilineTag,(Boolean)0) ;
    }
    
    MacPostControlCreate( pos, size );

    return true;
}

wxSize wxStaticText::DoGetBestSize() const
{
    ControlFontStyleRec controlFont;
    OSStatus err = m_peer->GetData<ControlFontStyleRec>( kControlEntireControl, kControlFontStyleTag, &controlFont );
    verify_noerr( err );

    Point bounds;
    SInt16 baseline;
    wxMacCFStringHolder str( m_label,  m_font.GetEncoding() );

    if ( m_font.MacGetThemeFontID() != kThemeCurrentPortFont )
    {
        err = GetThemeTextDimensions(
            (m_label.Length() > 0 ? (CFStringRef)str : CFSTR(" ")),
            m_font.MacGetThemeFontID(), kThemeStateActive, false, &bounds, &baseline );
        verify_noerr( err );
    }
    else
    {
        wxMacWindowStateSaver sv( this );
        ::TextFont( m_font.MacGetFontNum() );
        ::TextSize( (short)(m_font.MacGetFontSize()) );
        ::TextFace( m_font.MacGetFontStyle() );

        err = GetThemeTextDimensions(
            (m_label.Length() > 0 ? (CFStringRef)str : CFSTR(" ")),
            kThemeCurrentPortFont, kThemeStateActive, false, &bounds, &baseline );
        verify_noerr( err );
    }

    if ( m_label.Length() == 0 )
        bounds.h = 0 ;

    bounds.h += MacGetLeftBorderSize() + MacGetRightBorderSize();
    bounds.v += MacGetTopBorderSize() + MacGetBottomBorderSize();

    return wxSize( bounds.h, bounds.v );
}

void wxStaticText::SetLabel( const wxString& st )
{
    m_label = wxStripMenuCodes( st );

    wxMacCFStringHolder str( m_label, m_font.GetEncoding() );
    CFStringRef ref = str;
    OSStatus err = m_peer->SetData<CFStringRef>(kControlEntireControl, kControlStaticTextCFStringTag, ref );
    verify_noerr( err );

    if ( !(GetWindowStyle() & wxST_NO_AUTORESIZE) )
    {
        InvalidateBestSize();
        SetSize( GetBestSize() );
    }

    Refresh();

    // we shouldn't need forced updates
    // Update() ;
}

bool wxStaticText::SetFont(const wxFont& font)
{
    bool ret = wxControl::SetFont( font );

    if ( ret )
    {
        if ( !(GetWindowStyle() & wxST_NO_AUTORESIZE) )
        {
            InvalidateBestSize();
            SetSize( GetBestSize() );
        }
    }

    return ret;
}

#endif //if wxUSE_STATTEXT

