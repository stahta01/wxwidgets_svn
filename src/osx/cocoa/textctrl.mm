/////////////////////////////////////////////////////////////////////////////
// Name:        src/osx/cocoa/textctrl.mm
// Purpose:     wxTextCtrl
// Author:      Stefan Csomor
// Modified by: Ryan Norton (MLTE GetLineLength and GetLineText)
// Created:     1998-01-01
// RCS-ID:      $Id: textctrl.cpp 54820 2008-07-29 20:04:11Z SC $
// Copyright:   (c) Stefan Csomor
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#if wxUSE_TEXTCTRL

#include "wx/textctrl.h"

#ifndef WX_PRECOMP
    #include "wx/intl.h"
    #include "wx/app.h"
    #include "wx/utils.h"
    #include "wx/dc.h"
    #include "wx/button.h"
    #include "wx/menu.h"
    #include "wx/settings.h"
    #include "wx/msgdlg.h"
    #include "wx/toplevel.h"
#endif

#ifdef __DARWIN__
    #include <sys/types.h>
    #include <sys/stat.h>
#else
    #include <stat.h>
#endif

#if wxUSE_STD_IOSTREAM
    #if wxUSE_IOSTREAMH
        #include <fstream.h>
    #else
        #include <fstream>
    #endif
#endif

#include "wx/filefn.h"
#include "wx/sysopt.h"
#include "wx/thread.h"

#include "wx/osx/private.h"
#include "wx/osx/carbon/private/mactext.h"


@implementation wxNSTextField

- (void)setImplementation: (wxWidgetImpl *) theImplementation
{
    impl = theImplementation;
}

- (wxWidgetImpl*) implementation
{
    return impl;
}

- (BOOL) isFlipped
{
    return YES;
}

// use our common calls
- (void) setTitle:(NSString *) title
{
    [self setStringValue: title];
}

@end

class wxNSTextFieldControl : public wxMacTextControl
{
public :
    wxNSTextFieldControl( wxTextCtrl *wxPeer, WXWidget w ) : wxMacTextControl(wxPeer, w)
    {
    }
    virtual ~wxNSTextFieldControl()
    {
    }

    virtual void VisibilityChanged(bool shown){}
    virtual wxString GetStringValue() const 
    {
        wxCFStringRef cf( (CFStringRef) [[(wxNSTextField*) m_osxView stringValue] retain] );
        return cf.AsString(m_wxPeer->GetFont().GetEncoding());
    }
    virtual void SetStringValue( const wxString &str) 
    {
        [(wxNSTextField*) m_osxView setStringValue: wxCFStringRef( str , m_wxPeer->GetFont().GetEncoding() ).AsNSString()];
    }
    virtual void Copy() {}
    virtual void Cut() {}
    virtual void Paste() {}
    virtual bool CanPaste() const { return false;}
    virtual void SetEditable(bool editable) {}
    virtual void GetSelection( long* from, long* to) const {}
    virtual void SetSelection( long from , long to ){}
    virtual void WriteText(const wxString& str) 
    {
        // temp hack to get logging working early
        wxString former = GetStringValue();
        SetStringValue( former + str );
    }
};

wxWidgetImplType* wxWidgetImpl::CreateTextControl( wxTextCtrl* wxpeer, 
                                    wxWindowMac* parent, 
                                    wxWindowID id, 
                                    const wxString& str,
                                    const wxPoint& pos, 
                                    const wxSize& size,
                                    long style, 
                                    long extraStyle)
{
    NSView* sv = (wxpeer->GetParent()->GetHandle() );
    
    NSRect r = wxOSXGetFrameForControl( wxpeer, pos , size ) ;
    wxNSTextField* v = [[wxNSTextField alloc] initWithFrame:r];
    [sv addSubview:v];

    //[v setBezeled:NO];
    //[v setEditable:NO];
    //[v setDrawsBackground:NO];
    
    wxWidgetCocoaImpl* c = new wxNSTextFieldControl( wxpeer, v );
    [v setImplementation:c];
    return c;
}


#endif // wxUSE_TEXTCTRL
