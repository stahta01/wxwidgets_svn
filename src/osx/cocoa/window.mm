/////////////////////////////////////////////////////////////////////////////
// Name:        src/osx/cocoa/window.mm
// Purpose:     widgets (non tlw) for cocoa
// Author:      Stefan Csomor
// Modified by:
// Created:     2008-06-20
// RCS-ID:      $Id: window.mm 48805 2007-09-19 14:52:25Z SC $
// Copyright:   (c) Stefan Csomor
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/nonownedwnd.h"
#endif

#ifdef __WXMAC__
#include "wx/osx/private.h"
#endif

NSRect wxOSXGetFrameForControl( wxWindowMac* window , const wxPoint& pos , const wxSize &size , bool adjustForOrigin )
{
    int x, y, w, h ;

    window->MacGetBoundsForControl( pos , size , x , y, w, h , adjustForOrigin ) ;
    wxRect bounds(x,y,w,h);
    NSView* sv = (window->GetParent()->GetHandle() );

    return wxToNSRect( sv, bounds );
}

@interface wxNSView : NSView
{
    wxWidgetCocoaImpl* impl;
}

- (void)drawRect: (NSRect) rect;

WXCOCOAIMPL_COMMON_MOUSE_INTERFACE

- (void)keyDown:(NSEvent *)event;
- (void)keyUp:(NSEvent *)event;
- (void)flagsChanged:(NSEvent *)event;
- (void)handleKeyEvent:(NSEvent *)event;

- (void)setImplementation: (wxWidgetCocoaImpl *) theImplementation;
- (wxWidgetCocoaImpl*) implementation;
- (BOOL) isFlipped;
- (BOOL) becomeFirstResponder;
- (BOOL) resignFirstResponder;
- (BOOL) canBecomeKeyView;

@end // wxNSView

long wxOSXTranslateCocoaKey(unsigned short code, int unichar )
{
    long retval = code;
    switch( unichar )
    {
        case NSUpArrowFunctionKey :
            retval = WXK_UP;
            break;
        case NSDownArrowFunctionKey :
            retval = WXK_DOWN;
            break;
        case NSLeftArrowFunctionKey :
            retval = WXK_LEFT;
            break;
        case NSRightArrowFunctionKey :
            retval = WXK_RIGHT;
            break;
        case NSInsertFunctionKey  :
            retval = WXK_INSERT;
            break;
        case NSDeleteFunctionKey  :
            retval = WXK_DELETE;
            break;
        case NSHomeFunctionKey  :
            retval = WXK_HOME;
            break;
//        case NSBeginFunctionKey  :
//            retval = WXK_BEGIN;
//            break;
        case NSEndFunctionKey  :
            retval = WXK_END;
            break;
        case NSPageUpFunctionKey  :
            retval = WXK_PAGEUP;
            break;
       case NSPageDownFunctionKey  :
            retval = WXK_PAGEDOWN;
            break;
       case NSHelpFunctionKey  :
            retval = WXK_HELP;
            break;

        default :
            if ( unichar >= NSF1FunctionKey && unichar >= NSF24FunctionKey )
                retval = WXK_F1 + (unichar - NSF1FunctionKey );
            break;
    }
    return retval;
}

void SetupKeyEvent( wxKeyEvent &wxevent , NSEvent * nsEvent )
{
    UInt32 modifiers = [nsEvent modifierFlags] ;
    int eventType = [nsEvent type];

    wxevent.m_shiftDown = modifiers & NSShiftKeyMask;
    wxevent.m_controlDown = modifiers & NSControlKeyMask;
    wxevent.m_altDown = modifiers & NSAlternateKeyMask;
    wxevent.m_metaDown = modifiers & NSCommandKeyMask;

    wxString chars;
    if ( eventType != NSFlagsChanged )
    {
        NSString* nschars = [nsEvent characters];
        if ( nschars )
        {
            wxCFStringRef cfchars((CFStringRef)[nschars retain]);
            chars = cfchars.AsString();
        }
    }
    
    int unichar = chars.Length() > 0 ? chars[0] : 0;
    
#if wxUSE_UNICODE
    wxevent.m_uniChar = unichar;
#endif
    wxevent.m_keyCode = wxOSXTranslateCocoaKey( [nsEvent keyCode], unichar ) ;
//    wxevent.m_rawCode = keymessage;
    wxevent.m_rawFlags = modifiers;
    
    wxevent.SetTimestamp( [nsEvent timestamp] * 1000.0 ) ;
    switch (eventType)
    {
        case NSKeyDown :
            wxevent.SetEventType( wxEVT_KEY_DOWN )  ;
            break;
        case NSKeyUp :
            wxevent.SetEventType( wxEVT_KEY_UP )  ;
            break;
        case NSFlagsChanged :
            // setup common code here
            break;
        default :
            break ;
    }
}

void SetupMouseEvent( wxMouseEvent &wxevent , NSEvent * nsEvent )
{
    UInt32 modifiers = [nsEvent modifierFlags] ;
    wxPoint screenMouseLocation = wxFromNSPoint( NULL, [nsEvent locationInWindow]);

    // these parameters are not given for all events
    UInt32 button = [nsEvent buttonNumber];
    UInt32 clickCount = [nsEvent clickCount];
    UInt32 mouseChord = 0; // TODO does this exist for cocoa

    wxevent.m_x = screenMouseLocation.x;
    wxevent.m_y = screenMouseLocation.y;
    wxevent.m_shiftDown = modifiers & NSShiftKeyMask;
    wxevent.m_controlDown = modifiers & NSControlKeyMask;
    wxevent.m_altDown = modifiers & NSAlternateKeyMask;
    wxevent.m_metaDown = modifiers & NSCommandKeyMask;
    wxevent.m_clickCount = clickCount;
    wxevent.SetTimestamp( [nsEvent timestamp] * 1000.0 ) ;
/*
    // a control click is interpreted as a right click
    bool thisButtonIsFakeRight = false ;
    if ( button == kEventMouseButtonPrimary && (modifiers & controlKey) )
    {
        button = kEventMouseButtonSecondary ;
        thisButtonIsFakeRight = true ;
    }

    // otherwise we report double clicks by connecting a left click with a ctrl-left click
    if ( clickCount > 1 && button != g_lastButton )
        clickCount = 1 ;
    // we must make sure that our synthetic 'right' button corresponds in
    // mouse down, moved and mouse up, and does not deliver a right down and left up

    if ( cEvent.GetKind() == kEventMouseDown )
    {
        g_lastButton = button ;
        g_lastButtonWasFakeRight = thisButtonIsFakeRight ;
    }

    if ( button == 0 )
    {
        g_lastButton = 0 ;
        g_lastButtonWasFakeRight = false ;
    }
    else if ( g_lastButton == kEventMouseButtonSecondary && g_lastButtonWasFakeRight )
        button = g_lastButton ;

    // Adjust the chord mask to remove the primary button and add the
    // secondary button.  It is possible that the secondary button is
    // already pressed, e.g. on a mouse connected to a laptop, but this
    // possibility is ignored here:
    if( thisButtonIsFakeRight && ( mouseChord & 1U ) )
        mouseChord = ((mouseChord & ~1U) | 2U);

    if(mouseChord & 1U)
                wxevent.m_leftDown = true ;
    if(mouseChord & 2U)
                wxevent.m_rightDown = true ;
    if(mouseChord & 4U)
                wxevent.m_middleDown = true ;

*/
    // translate into wx types
    int eventType = [nsEvent type];
    switch (eventType)
    {
        case NSLeftMouseDown :
        case NSRightMouseDown :
        case NSOtherMouseDown :
            switch ( button )
            {
                case 0 :
                    wxevent.SetEventType( clickCount > 1 ? wxEVT_LEFT_DCLICK : wxEVT_LEFT_DOWN )  ;
                    break ;

                case 1 :
                    wxevent.SetEventType( clickCount > 1 ? wxEVT_RIGHT_DCLICK : wxEVT_RIGHT_DOWN ) ;
                    break ;

                case 2 :
                    wxevent.SetEventType( clickCount > 1 ? wxEVT_MIDDLE_DCLICK : wxEVT_MIDDLE_DOWN ) ;
                    break ;

                default:
                    break ;
            }
            break ;

        case NSLeftMouseUp :
        case NSRightMouseUp :
        case NSOtherMouseUp :
            switch ( button )
            {
                case 0 :
                    wxevent.SetEventType( wxEVT_LEFT_UP )  ;
                    break ;

                case 1 :
                    wxevent.SetEventType( wxEVT_RIGHT_UP ) ;
                    break ;

                case 2 :
                    wxevent.SetEventType( wxEVT_MIDDLE_UP ) ;
                    break ;

                default:
                    break ;
            }
            break ;

     case NSScrollWheel :
        {
            wxevent.SetEventType( wxEVT_MOUSEWHEEL ) ;
            /*
            EventMouseWheelAxis axis = cEvent.GetParameter<EventMouseWheelAxis>(kEventParamMouseWheelAxis, typeMouseWheelAxis) ;
            SInt32 delta = cEvent.GetParameter<SInt32>(kEventParamMouseWheelDelta, typeSInt32) ;
            */
            wxevent.m_wheelRotation = 10; // delta;
            wxevent.m_wheelDelta = 1;
            wxevent.m_linesPerAction = 1;
            if ( 0 /* axis == kEventMouseWheelAxisX*/ )
                wxevent.m_wheelAxis = 1;
        }
        break ;

        case NSMouseEntered :
        case NSMouseExited :
        case NSLeftMouseDragged :
        case NSRightMouseDragged :
        case NSOtherMouseDragged :
        case NSMouseMoved :
            wxevent.SetEventType( wxEVT_MOTION ) ;
            break;
        default :
            break ;
    }
}

@implementation wxNSView

#define OSX_DEBUG_DRAWING 0

- (void)drawRect: (NSRect) rect
{
    if ( impl )
    {
        CGContextRef context = (CGContextRef) [[NSGraphicsContext currentContext] graphicsPort];
        CGContextSaveGState( context );
#if OSX_DEBUG_DRAWING
        CGContextBeginPath( context );
        CGContextMoveToPoint(context, 0, 0);
        NSRect bounds = [self bounds];
        CGContextAddLineToPoint(context, 10, 0);
        CGContextMoveToPoint(context, 0, 0);
        CGContextAddLineToPoint(context, 0, 10);
        CGContextMoveToPoint(context, bounds.size.width, bounds.size.height);
        CGContextAddLineToPoint(context, bounds.size.width, bounds.size.height-10);
        CGContextMoveToPoint(context, bounds.size.width, bounds.size.height);
        CGContextAddLineToPoint(context, bounds.size.width-10, bounds.size.height);
        CGContextClosePath( context );
        CGContextStrokePath(context);
#endif

        if ( [ self isFlipped ] == NO )
        {
            CGContextTranslateCTM( context, 0,  [self bounds].size.height );
            CGContextScaleCTM( context, 1, -1 );
        }
        
        wxRegion updateRgn;
        const NSRect *rects;
        NSInteger count;

        [self getRectsBeingDrawn:&rects count:&count];
        for ( int i = 0 ; i < count ; ++i )
        {
            updateRgn.Union(wxFromNSRect(self, rects[i]) );
        }

        wxWindow* wxpeer = impl->GetWXPeer();
        wxpeer->GetUpdateRegion() = updateRgn;
        wxpeer->MacSetCGContextRef( context );
        
        wxPaintEvent event;
        event.SetTimestamp(0); //  todo
        event.SetEventObject(wxpeer);
        wxpeer->HandleWindowEvent(event);
                
        CGContextRestoreGState( context );
    }
}

WXCOCOAIMPL_COMMON_MOUSE_IMPLEMENTATION

- (void)keyDown:(NSEvent *)event
{
    [self handleKeyEvent:event];
}

- (void)keyUp:(NSEvent *)event
{
    [self handleKeyEvent:event];
}

- (void)flagsChanged:(NSEvent *)event
{
    [self handleKeyEvent:event];
}

- (void)handleKeyEvent:(NSEvent *)event
{
    wxKeyEvent wxevent(wxEVT_KEY_DOWN);
    SetupKeyEvent( wxevent, event );
    impl->GetWXPeer()->HandleWindowEvent(wxevent);
}


- (void)setImplementation: (wxWidgetCocoaImpl *) theImplementation
{
    impl = theImplementation;
}

- (wxWidgetCocoaImpl*) implementation
{
    return impl;
}

- (BOOL) isFlipped
{
    return YES;
}

- (BOOL) becomeFirstResponder
{
    BOOL r = [super becomeFirstResponder];
    if ( r )
    {
    }
    return r;
}

- (BOOL) resignFirstResponder
{
    BOOL r = [super resignFirstResponder];
    if ( r )
    {
    }
    return r;
}

- (BOOL) canBecomeKeyView
{
    return YES;
}

@end // wxNSView

IMPLEMENT_DYNAMIC_CLASS( wxWidgetCocoaImpl , wxWidgetImpl )

wxWidgetCocoaImpl::wxWidgetCocoaImpl( wxWindowMac* peer , WXWidget w, bool isRootControl ) :
    wxWidgetImpl( peer, isRootControl ), m_osxView(w)
{
}

wxWidgetCocoaImpl::wxWidgetCocoaImpl() 
{
}

void wxWidgetCocoaImpl::Init()
{
    m_osxView = NULL;
}

wxWidgetCocoaImpl::~wxWidgetCocoaImpl()
{
    if ( [m_osxView respondsToSelector:@selector(setImplementation:) ] )
        [m_osxView setImplementation:NULL];
    if ( !IsRootControl() )
    {
        NSView *sv = [m_osxView superview];
        if ( sv != nil )
            [m_osxView removeFromSuperview];
    }
    [m_osxView release];
}
    
bool wxWidgetCocoaImpl::IsVisible() const 
{
    return [m_osxView isHiddenOrHasHiddenAncestor] == NO;
}

void wxWidgetCocoaImpl::SetVisibility( bool visible )
{
    [m_osxView setHidden:(visible ? NO:YES)];
}

void wxWidgetCocoaImpl::Raise()
{
}
    
void wxWidgetCocoaImpl::Lower()
{
}

void wxWidgetCocoaImpl::ScrollRect( const wxRect *rect, int dx, int dy )
{
}

void wxWidgetCocoaImpl::Move(int x, int y, int width, int height)
{
    NSRect r = wxToNSRect( [m_osxView superview], wxRect(x,y,width, height) );
    [m_osxView setFrame:r];
}

void wxWidgetCocoaImpl::GetPosition( int &x, int &y ) const
{
    wxRect r = wxFromNSRect( [m_osxView superview], [m_osxView frame] );
    x = r.GetLeft();
    y = r.GetTop();
}

void wxWidgetCocoaImpl::GetSize( int &width, int &height ) const
{
    NSRect rect = [m_osxView frame];
    width = rect.size.width;
    height = rect.size.height;
}

void wxWidgetCocoaImpl::GetContentArea( int&left, int &top, int &width, int &height ) const
{
    left = top = 0;
    GetSize( width, height );
}

void wxWidgetCocoaImpl::SetNeedsDisplay( const wxRect* where )
{
    if ( where )
        [m_osxView setNeedsDisplayInRect:wxToNSRect(m_osxView, *where )];
    else
        [m_osxView setNeedsDisplay:YES];
}

bool wxWidgetCocoaImpl::GetNeedsDisplay() const
{
    return [m_osxView needsDisplay];
}

bool wxWidgetCocoaImpl::CanFocus() const
{
    return [m_osxView canBecomeKeyView] == YES;
}

bool wxWidgetCocoaImpl::HasFocus() const
{
    return ( [[m_osxView window] firstResponder] == m_osxView );
}

bool wxWidgetCocoaImpl::SetFocus() 
{
    if ( [m_osxView canBecomeKeyView] == NO )
        return false;
        
    [[m_osxView window] makeFirstResponder: m_osxView] ;
    return true;
}


void wxWidgetCocoaImpl::RemoveFromParent()
{
    [m_osxView removeFromSuperview];
}

void wxWidgetCocoaImpl::Embed( wxWidgetImpl *parent )
{
    NSView* container = parent->GetWXWidget() ;
    wxASSERT_MSG( container != NULL , wxT("No valid mac container control") ) ;
    [container addSubview:m_osxView];
}

void wxWidgetCocoaImpl::SetBackgroundColour( const wxColour &WXUNUSED(col) )
{
    // m_osxView.backgroundColor = [[UIColor alloc] initWithCGColor:col.GetCGColor()];
}

void wxWidgetCocoaImpl::SetLabel( const wxString& title, wxFontEncoding encoding )
{
    if ( [m_osxView respondsToSelector:@selector(setTitle:) ] )
    {
        wxCFStringRef cf( title , m_wxPeer->GetFont().GetEncoding() );
        [m_osxView setTitle:cf.AsNSString()];
    }
}
    

void  wxWidgetImpl::Convert( wxPoint *pt , wxWidgetImpl *from , wxWidgetImpl *to )
{
    NSPoint p = wxToNSPoint( from->GetWXWidget(), *pt );
    p = [from->GetWXWidget() convertPoint:p toView:to->GetWXWidget() ]; 
    *pt = wxFromNSPoint( to->GetWXWidget(), p );
}

wxInt32 wxWidgetCocoaImpl::GetValue() const 
{
    return [(NSControl*)m_osxView intValue];
}

void wxWidgetCocoaImpl::SetValue( wxInt32 v ) 
{
    if (  [m_osxView respondsToSelector:@selector(setIntValue:)] )
    {
        [m_osxView setIntValue:v];
    }
    else if (  [m_osxView respondsToSelector:@selector(setFloatValue:)] )
    {
        [m_osxView setFloatValue:(double)v];
    }
    else if (  [m_osxView respondsToSelector:@selector(setDoubleValue:)] )
    {
        [m_osxView setDoubleValue:(double)v];
    }
}

void wxWidgetCocoaImpl::SetMinimum( wxInt32 v ) 
{
    if (  [m_osxView respondsToSelector:@selector(setMinValue:)] )
    {
        [m_osxView setMinValue:(double)v];
    }
}

void wxWidgetCocoaImpl::SetMaximum( wxInt32 v ) 
{
    if (  [m_osxView respondsToSelector:@selector(setMaxValue:)] )
    {
        [m_osxView setMaxValue:(double)v];
    }
}

void wxWidgetCocoaImpl::SetBitmap( const wxBitmap& bitmap )
{
    if (  [m_osxView respondsToSelector:@selector(setImage:)] )
    {
        [m_osxView setImage:bitmap.GetNSImage()];
    }
}

void wxWidgetCocoaImpl::SetupTabs( const wxNotebook& notebook)
{
    // implementation in subclass
}

void wxWidgetCocoaImpl::GetBestRect( wxRect *r ) const
{
    r->x = r->y = r->width = r->height = 0;
//    if (  [m_osxView isKindOfClass:[NSControl class]] )
    if (  [m_osxView respondsToSelector:@selector(sizeToFit)] )
    {
        NSRect former = [m_osxView frame];
        [m_osxView sizeToFit];
        NSRect best = [m_osxView frame];
        [m_osxView setFrame:former];
        r->width = best.size.width;
        r->height = best.size.height;
    }
}

bool wxWidgetCocoaImpl::IsEnabled() const
{
    if ( [m_osxView respondsToSelector:@selector(isEnabled) ] )
        return [m_osxView isEnabled];
    return true;
}

void wxWidgetCocoaImpl::Enable( bool enable )
{
    if ( [m_osxView respondsToSelector:@selector(setEnabled:) ] )
        [m_osxView setEnabled:enable];
}

void wxWidgetCocoaImpl::PulseGauge()
{
}

void wxWidgetCocoaImpl::SetScrollThumb( wxInt32 val, wxInt32 view )
{
}

void wxWidgetCocoaImpl::SetControlSize( wxWindowVariant variant ) 
{
    NSControlSize size = NSRegularControlSize;
    
    switch ( variant )
    {
        case wxWINDOW_VARIANT_NORMAL :
            size = NSRegularControlSize;
            break ;

        case wxWINDOW_VARIANT_SMALL :
            size = NSSmallControlSize;
            break ;

        case wxWINDOW_VARIANT_MINI :
            size = NSMiniControlSize;
            break ;

        case wxWINDOW_VARIANT_LARGE :
            size = NSRegularControlSize;
            break ;

        default:
            wxFAIL_MSG(_T("unexpected window variant"));
            break ;
    }
    if ( [m_osxView respondsToSelector:@selector(setControlSize:)] )
        [m_osxView setControlSize:size];
}

void wxWidgetCocoaImpl::SetFont(wxFont const&, wxColour const&, long, bool)
{
    // TODO
}

void wxWidgetCocoaImpl::InstallEventHandler( WXWidget control )
{
}

void wxWidgetCocoaImpl::DoHandleMouseEvent(NSEvent *event)
{
    NSPoint clickLocation; 
    clickLocation = [m_osxView convertPoint:[event locationInWindow] fromView:nil]; 
    wxPoint pt = wxFromNSPoint( m_osxView, clickLocation );
    wxMouseEvent wxevent(wxEVT_LEFT_DOWN);
    SetupMouseEvent( wxevent , event ) ;
    wxevent.m_x = pt.x;
    wxevent.m_y = pt.y;
    GetWXPeer()->HandleWindowEvent(wxevent);
}


//
// Factory methods
//

wxWidgetImpl* wxWidgetImpl::CreateUserPane( wxWindowMac* wxpeer, wxWindowMac* parent, wxWindowID id, const wxPoint& pos, const wxSize& size,
                            long style, long extraStyle)
{
    NSView* sv = (wxpeer->GetParent()->GetHandle() );
    
    NSRect r = wxOSXGetFrameForControl( wxpeer, pos , size ) ;
    wxNSView* v = [[wxNSView alloc] initWithFrame:r];
    [sv addSubview:v];
    wxWidgetCocoaImpl* c = new wxWidgetCocoaImpl( wxpeer, v );
    [v setImplementation:c];
    return c;
}

wxWidgetImpl* wxWidgetImpl::CreateContentView( wxNonOwnedWindow* now ) 
{
    NSWindow* tlw = now->GetWXWindow();
    wxNSView* v = [[wxNSView alloc] initWithFrame:[[tlw contentView] frame]];
    wxWidgetCocoaImpl* c = new wxWidgetCocoaImpl( now, v, true );
    [v setImplementation:c];
    [tlw setContentView:v];
    return c;
}
