/////////////////////////////////////////////////////////////////////////////
// Name:        src/mac/carbon/stattext.cpp
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

#include "wx/stattext.h"

#ifndef WX_PRECOMP
    #include "wx/app.h"
    #include "wx/utils.h"
    #include "wx/dc.h"
    #include "wx/dcclient.h"
    #include "wx/settings.h"
#endif // WX_PRECOMP

#include "wx/notebook.h"
#include "wx/tabctrl.h"

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

    if ( !wxControl::Create( parent, id, pos, size, style, wxDefaultValidator, name ) )
        return false;

    Rect bounds = wxMacGetBoundsForControl( this, pos, size );

    m_peer = new wxMacControl( this );
    OSStatus err = CreateStaticTextControl(
        MAC_WXHWND(parent->MacGetTopLevelWindowRef()),
        &bounds, NULL, NULL, m_peer->GetControlRefAddr() );
    verify_noerr( err );

    if ( ( style & wxST_ELLIPSIZE_END ) || ( style & wxST_ELLIPSIZE_MIDDLE ) )
    {
        TruncCode tCode = truncEnd;
        if ( style & wxST_ELLIPSIZE_MIDDLE )
            tCode = truncMiddle;

        err = m_peer->SetData( kControlStaticTextTruncTag, tCode );
        err = m_peer->SetData( kControlStaticTextIsMultilineTag, (Boolean)0 );
    }

    MacPostControlCreate( pos, size );

    SetLabel(label);

    return true;
}

wxSize wxStaticText::DoGetBestSize() const
{
    Rect bestsize = { 0 , 0 , 0 , 0 } ;
    Point bounds;
    
    // try the built-in best size if available
    Boolean former = m_peer->GetData<Boolean>( kControlStaticTextIsMultilineTag);
    m_peer->SetData( kControlStaticTextIsMultilineTag, (Boolean)0 );
    m_peer->GetBestRect( &bestsize ) ;
    m_peer->SetData( kControlStaticTextIsMultilineTag, former );
    if ( !EmptyRect( &bestsize ) )
    {
        bounds.h = bestsize.right - bestsize.left ;
        bounds.v = bestsize.bottom - bestsize.top ;
    }
    else
    {
        ControlFontStyleRec controlFont;
        OSStatus err = m_peer->GetData<ControlFontStyleRec>( kControlEntireControl, kControlFontStyleTag, &controlFont );
        verify_noerr( err );

        wxCFStringRef str( m_label,  GetFont().GetEncoding() );

#if wxMAC_USE_ATSU_TEXT
        SInt16 baseline;
        if ( m_font.MacGetThemeFontID() != kThemeCurrentPortFont )
        {
            err = GetThemeTextDimensions(
                (!m_label.empty() ? (CFStringRef)str : CFSTR(" ")),
                m_font.MacGetThemeFontID(), kThemeStateActive, false, &bounds, &baseline );
            verify_noerr( err );
        }
        else
#endif
        {
            wxClientDC dc(const_cast<wxStaticText*>(this));
            wxCoord width, height ;
            dc.GetTextExtent( m_label , &width, &height);
            bounds.h = width;
            bounds.v = height;
        }

        if ( m_label.empty() )
            bounds.h = 0;
    }
    bounds.h += MacGetLeftBorderSize() + MacGetRightBorderSize();
    bounds.v += MacGetTopBorderSize() + MacGetBottomBorderSize();

    return wxSize( bounds.h, bounds.v );
}

void wxStaticText::SetLabel(const wxString& label)
{
    m_labelOrig = label;

    // middle/end ellipsization is handled by the OS:
    if ( HasFlag(wxST_ELLIPSIZE_END) || HasFlag(wxST_ELLIPSIZE_MIDDLE) )
    {
        // remove markup
        wxString str(label);
        if (HasFlag(wxST_MARKUP))
            str = RemoveMarkup(label);

        // and leave ellipsization to the OS
        DoSetLabel(str);
    }
    else // not supported natively
    {
        DoSetLabel(GetEllipsizedLabelWithoutMarkup());
    }

    if ( !(GetWindowStyle() & wxST_NO_AUTORESIZE) &&
         !IsEllipsized() )  // don't resize if we adjust to current size
    {
        InvalidateBestSize();
        SetSize( GetBestSize() );
    }

    Refresh();

    // we shouldn't need forced updates
    // Update();
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


// for wxST_ELLIPSIZE_* support:

void wxStaticText::DoSetLabel(const wxString& label)
{
    m_labelOrig = label;
    m_label = RemoveMnemonics(label);

    wxCFStringRef str( m_label, GetFont().GetEncoding() );
    OSStatus err = m_peer->SetData<CFStringRef>(kControlEntireControl, kControlStaticTextCFStringTag, str);
    verify_noerr( err );
}

wxString wxStaticText::DoGetLabel() const
{
    return m_label;
}

/*
   FIXME: UpdateLabel() should be called on size events when wxST_ELLIPSIZE_START is set
          to allow correct dynamic ellipsizing of the label
*/

#endif //if wxUSE_STATTEXT
