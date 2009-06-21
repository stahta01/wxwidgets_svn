/////////////////////////////////////////////////////////////////////////////
// Name:        src/osx/cocoa/button.mm
// Purpose:     wxButton
// Author:      Stefan Csomor
// Modified by:
// Created:     1998-01-01
// RCS-ID:      $Id: button.cpp 54845 2008-07-30 14:52:41Z SC $
// Copyright:   (c) Stefan Csomor
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#include "wx/button.h"

#ifndef WX_PRECOMP
    #include "wx/panel.h"
    #include "wx/toplevel.h"
    #include "wx/dcclient.h"
#endif

#include "wx/stockitem.h"

#include "wx/osx/private.h"

wxSize wxButton::DoGetBestSize() const
{
    if ( GetId() == wxID_HELP )
        return wxSize( 23 , 23 ) ;

    wxSize sz = GetDefaultSize() ;

    switch (GetWindowVariant())
    {
        case wxWINDOW_VARIANT_NORMAL:
        case wxWINDOW_VARIANT_LARGE:
            sz.y = 23 ;
            break;

        case wxWINDOW_VARIANT_SMALL:
            sz.y = 17 ;
            break;

        case wxWINDOW_VARIANT_MINI:
            sz.y = 15 ;
            break;

        default:
            break;
    }

    wxRect r ;

    m_peer->GetBestRect(&r);

    if ( r.GetWidth() == 0 && r.GetHeight() == 0 )
    {
    }
    sz.x = r.GetWidth();
    sz.y = r.GetHeight();

    int wBtn = 96;

    if ((wBtn > sz.x) || ( GetWindowStyle() & wxBU_EXACTFIT))
        sz.x = wBtn;

#if wxOSX_USE_CARBON
    Rect    bestsize = { 0 , 0 , 0 , 0 } ;
    m_peer->GetBestRect( &bestsize ) ;

    int wBtn;
    if ( EmptyRect( &bestsize ) || ( GetWindowStyle() & wxBU_EXACTFIT) )
    {
        Point bounds;

        ControlFontStyleRec controlFont;
        OSStatus err = m_peer->GetData<ControlFontStyleRec>( kControlEntireControl, kControlFontStyleTag, &controlFont );
        verify_noerr( err );

        wxCFStringRef str( m_label,  GetFont().GetEncoding() );

#if wxOSX_USE_ATSU_TEXT
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
            wxClientDC dc(const_cast<wxButton*>(this));
            wxCoord width, height ;
            dc.GetTextExtent( m_label , &width, &height);
            bounds.h = width;
            bounds.v = height;
        }

        wBtn = bounds.h + sz.y;
    }
    else
    {
        wBtn = bestsize.right - bestsize.left ;
        // non 'normal' window variants don't return the correct height
        // sz.y = bestsize.bottom - bestsize.top ;
    }
    if ((wBtn > sz.x) || ( GetWindowStyle() & wxBU_EXACTFIT))
        sz.x = wBtn;
#endif

    return sz ;
}

wxSize wxButton::GetDefaultSize()
{
    int wBtn = 70 ;
    int hBtn = 20 ;

    return wxSize(wBtn, hBtn);
}

@implementation wxNSButton

+ (void)initialize
{
    static BOOL initialized = NO;
    if (!initialized)
    {
        initialized = YES;
        wxOSXCocoaClassAddWXMethods( self );
    }
}

- (int) intValue
{
    switch ( [self state] )
    {
        case NSOnState:
            return 1;
        case NSMixedState:
            return 2;
        default:
            return 0;
    }
}

- (void) setIntValue: (int) v
{
    switch( v )
    {
        case 2:
            [self setState:NSMixedState];
            break;
        case 1:
            [self setState:NSOnState];
            break;
        default :
            [self setState:NSOffState];
            break;
    }
}

@end

namespace
{

class wxButtonCocoaImpl : public wxWidgetCocoaImpl
{
public:
    wxButtonCocoaImpl(wxWindowMac *wxpeer, wxNSButton *v)
        : wxWidgetCocoaImpl(wxpeer, v)
    {
    }

    virtual void SetBitmap(const wxBitmap& bitmap)
    {
        [GetNSButton() setBezelStyle:bitmap.IsOk() ? NSRegularSquareBezelStyle
                                                   : NSRoundedBezelStyle];

        wxWidgetCocoaImpl::SetBitmap(bitmap);
    }

private:
    NSButton *GetNSButton() const
    {
        wxASSERT( [m_osxView isKindOfClass:[NSButton class]] );

        return static_cast<NSButton *>(m_osxView);
    }
};

} // anonymous namespace

wxWidgetImplType* wxWidgetImpl::CreateButton( wxWindowMac* wxpeer,
                                    wxWindowMac* WXUNUSED(parent),
                                    wxWindowID id,
                                    const wxString& WXUNUSED(label),
                                    const wxPoint& pos,
                                    const wxSize& size,
                                    long WXUNUSED(style),
                                    long WXUNUSED(extraStyle))
{
    NSRect r = wxOSXGetFrameForControl( wxpeer, pos , size ) ;
    wxNSButton* v = [[wxNSButton alloc] initWithFrame:r];

    if ( id == wxID_HELP )
    {
        [v setBezelStyle:NSHelpButtonBezelStyle];
    }
    else
    {
        [v setBezelStyle:NSRoundedBezelStyle];
    }

    [v setButtonType:NSMomentaryPushInButton];
    return new wxButtonCocoaImpl( wxpeer, v );
}

void wxWidgetCocoaImpl::SetDefaultButton( bool isDefault )
{
    if ( isDefault && [m_osxView isKindOfClass:[NSButton class]] )
        // NOTE: setKeyEquivalent: nil will trigger an assert
        // instead do not call in that case.
        [(NSButton*)m_osxView setKeyEquivalent: @"\r" ];
}

void wxWidgetCocoaImpl::PerformClick()
{
}

//
// wxDisclosureButton implementation
//

@interface wxDisclosureNSButton : NSButton
{

    BOOL isOpen;
}

- (void) updateImage;

- (void) toggle;

+ (NSImage *)rotateImage: (NSImage *)image;

@end

static const char * disc_triangle_xpm[] = {
"10 9 4 1",
"   c None",
".  c #737373",
"+  c #989898",
"-  c #c6c6c6",
" .-       ",
" ..+-     ",
" ....+    ",
" ......-  ",
" .......- ",
" ......-  ",
" ....+    ",
" ..+-     ",
" .-       ",
};

@implementation wxDisclosureNSButton

+ (void)initialize
{
    static BOOL initialized = NO;
    if (!initialized)
    {
        initialized = YES;
        wxOSXCocoaClassAddWXMethods( self );
    }
}

- (id) initWithFrame:(NSRect) frame
{
    self = [super initWithFrame:frame];
    [self setBezelStyle:NSSmallSquareBezelStyle];
    isOpen = NO;
    [self setImagePosition:NSImageLeft];
    [self updateImage];
    return self;
}

- (int) intValue
{
    return isOpen ? 1 : 0;
}

- (void) setIntValue: (int) v
{
    isOpen = ( v != 0 );
    [self updateImage];
}

- (void) toggle
{
    isOpen = !isOpen;
    [self updateImage];
}

wxCFRef<NSImage*> downArray ;

- (void) updateImage
{
    static wxBitmap trianglebm(disc_triangle_xpm);
    if ( downArray.get() == NULL )
    {
        downArray.reset( [wxDisclosureNSButton rotateImage:trianglebm.GetNSImage()] );
    }

    if ( isOpen )
        [self setImage:(NSImage*)downArray.get()];
    else
        [self setImage:trianglebm.GetNSImage()];
}

+ (NSImage *)rotateImage: (NSImage *)image
{
    NSSize imageSize = [image size];
    NSSize newImageSize = NSMakeSize(imageSize.height, imageSize.width);
    NSImage* newImage = [[NSImage alloc] initWithSize: newImageSize];

    [newImage lockFocus];

    NSAffineTransform* tm = [NSAffineTransform transform];
    [tm translateXBy:newImageSize.width/2 yBy:newImageSize.height/2];
    [tm rotateByDegrees:-90];
    [tm translateXBy:-newImageSize.width/2 yBy:-newImageSize.height/2];
    [tm concat];


    [image drawInRect:NSMakeRect(0,0,newImageSize.width, newImageSize.height)
        fromRect:NSZeroRect operation:NSCompositeCopy fraction:1.0];

    [newImage unlockFocus];
    return newImage;
}

@end

class wxDisclosureTriangleCocoaImpl : public wxWidgetCocoaImpl
{
public :
    wxDisclosureTriangleCocoaImpl(wxWindowMac* peer , WXWidget w) :
        wxWidgetCocoaImpl(peer, w)
    {
    }

    ~wxDisclosureTriangleCocoaImpl()
    {
    }

    virtual void controlAction(WXWidget slf, void* _cmd, void *sender)
    {
        wxDisclosureNSButton* db = (wxDisclosureNSButton*)m_osxView;
        [db toggle];
        wxWidgetCocoaImpl::controlAction(slf, _cmd, sender );
    }
};

wxWidgetImplType* wxWidgetImpl::CreateDisclosureTriangle( wxWindowMac* wxpeer,
                                    wxWindowMac* WXUNUSED(parent),
                                    wxWindowID WXUNUSED(id),
                                    const wxString& label,
                                    const wxPoint& pos,
                                    const wxSize& size,
                                    long WXUNUSED(style),
                                    long WXUNUSED(extraStyle))
{
    NSRect r = wxOSXGetFrameForControl( wxpeer, pos , size ) ;
    wxDisclosureNSButton* v = [[wxDisclosureNSButton alloc] initWithFrame:r];
    [v setTitle:wxCFStringRef( label).AsNSString()];
    wxDisclosureTriangleCocoaImpl* c = new wxDisclosureTriangleCocoaImpl( wxpeer, v );
    return c;
}
