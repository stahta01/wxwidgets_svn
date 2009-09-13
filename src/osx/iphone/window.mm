/////////////////////////////////////////////////////////////////////////////
// Name:        src/osx/iphone/window.mm
// Purpose:     widgets (non tlw) for iphone
// Author:      Stefan Csomor
// Modified by:
// Created:     2008-06-20
// RCS-ID:      $Id: window.mm 48805 2007-09-19 14:52:25Z SC $
// Copyright:   (c) Stefan Csomor
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#include "wx/osx/private.h"

#ifndef WX_PRECOMP
    #include "wx/nonownedwnd.h"
    #include "wx/frame.h"
    #include "wx/event.h"
    #include "wx/log.h"
#endif

#include <objc/runtime.h>

WXWidget wxWidgetImpl::FindFocus()
{
    UIView* focusedView = nil;
    UIWindow* keyWindow = [[UIApplication sharedApplication] keyWindow];
    if ( keyWindow != nil )
    {
    /*
        NSResponder* responder = [keyWindow firstResponder];
        if ( [responder isKindOfClass:[NSTextView class]] &&
            [keyWindow fieldEditor:NO forObject:nil] != nil )
        {
            focusedView = [(NSTextView*)responder delegate];
        }
        else
        {
            if ( [responder isKindOfClass:[NSView class]] )
                focusedView = (NSView*) responder;
        }
    */
    }
    return focusedView;
}

CGRect wxOSXGetFrameForControl( wxWindowMac* window , const wxPoint& pos , const wxSize &size , bool adjustForOrigin )
{
    int x, y, w, h ;

    window->MacGetBoundsForControl( pos , size , x , y, w, h , adjustForOrigin ) ;
    wxRect bounds(x,y,w,h);
    UIView* sv = (window->GetParent()->GetHandle() );

    return wxToNSRect( sv, bounds );
}

@interface wxUIView : UIView
{
}

@end // wxUIView

@interface wxUIView(PossibleMethods)
- (void)setTitle:(NSString *)title forState:(UIControlState)state;

- (void)drawRect: (CGRect) rect;

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)handleTouchEvent:(NSSet *)touches withEvent:(UIEvent *)event;

- (BOOL) becomeFirstResponder;
- (BOOL) resignFirstResponder;
@end

@interface wxUIContentView : wxUIView
{
}

@end

@interface wxUIContentViewController : UIViewController
{
}

@end

//
//
//

void SetupMouseEvent( wxMouseEvent &wxevent , NSSet* touches, UIEvent * nsEvent )
{
    UInt32 modifiers = 0 ;
    UITouch *touch = [touches anyObject];

    // these parameters are not given for all events
    UInt32 button = 0; // no secondary button
    UInt32 clickCount = [touch tapCount];
    UInt32 mouseChord = 0; // TODO does this exist for cocoa

    // will be overridden
    wxevent.m_x = 0;
    wxevent.m_y = 0;
    wxevent.m_shiftDown = 0;
    wxevent.m_controlDown = 0;
    wxevent.m_altDown = 0;
    wxevent.m_metaDown = 0;
    wxevent.m_clickCount = clickCount;
    wxevent.SetTimestamp( [touch timestamp] ) ;
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
    int eventType = [touch phase];
    switch (eventType)
    {
        case UITouchPhaseBegan :
            switch ( button )
            {
                case 0 :
                    wxevent.SetEventType( clickCount > 1 ? wxEVT_LEFT_DCLICK : wxEVT_LEFT_DOWN )  ;
                    break ;

                default:
                    break ;
            }
            break ;

        case UITouchPhaseEnded :
            switch ( button )
            {
                case 0 :
                    wxevent.SetEventType( wxEVT_LEFT_UP )  ;
                    break ;

                default:
                    break ;
            }
            break ;

        case UITouchPhaseMoved :
            wxevent.SetEventType( wxEVT_MOTION ) ;
            break;
        default :
            break ;
    }
}

@implementation wxUIView

+ (void)initialize
{
    static BOOL initialized = NO;
    if (!initialized)
    {
        initialized = YES;
        wxOSXIPhoneClassAddWXMethods( self );
    }
}

@end // wxUIView

/*
- (void)drawRect: (CGRect) rect
{
    wxWidgetIPhoneImpl* impl = (wxWidgetIPhoneImpl* ) wxWidgetImpl::FindFromWXWidget( self );
    if ( impl )
    {
        CGContextRef context = (CGContextRef) UIGraphicsGetCurrentContext();
        CGContextSaveGState( context );
        // draw background

        CGContextSetFillColorWithColor( context, impl->GetWXPeer()->GetBackgroundColour().GetCGColor());
        CGContextFillRect(context, rect );

        impl->GetWXPeer()->MacSetCGContextRef( context );

        impl->GetWXPeer()->GetUpdateRegion() =
            wxRegion(rect.origin.x,rect.origin.y,rect.size.width,rect.size.height) ;

        wxPaintEvent event;
        event.SetTimestamp(0); //  todo
        event.SetEventObject(impl->GetWXPeer());
        impl->GetWXPeer()->HandleWindowEvent(event);

        CGContextRestoreGState( context );
    }

}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    [self handleTouchEvent:touches withEvent:event];
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    [self handleTouchEvent:touches withEvent:event];
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    [self handleTouchEvent:touches withEvent:event];
}

-(void)handleTouchEvent:(NSSet *)touches withEvent:(UIEvent *)event
{
    wxWidgetIPhoneImpl* impl = (wxWidgetIPhoneImpl* ) wxWidgetImpl::FindFromWXWidget( self );
    CGPoint clickLocation;
    UITouch *touch = [touches anyObject];
    clickLocation = [touch locationInView:self];

    wxMouseEvent wxevent(wxEVT_LEFT_DOWN);
    SetupMouseEvent( wxevent , touches, event ) ;
    wxevent.m_x = clickLocation.x;
    wxevent.m_y = clickLocation.y;
    wxevent.SetEventObject( impl->GetWXPeer() ) ;
    wxevent.SetId( impl->GetWXPeer()->GetId() ) ;
    impl->GetWXPeer()->HandleWindowEvent(wxevent);
}

*/

void wxOSX_touchEvent(UIView* self, SEL _cmd, NSSet* touches, UIEvent *event )
{
    wxWidgetIPhoneImpl* impl = (wxWidgetIPhoneImpl* ) wxWidgetImpl::FindFromWXWidget( self );
    if (impl == NULL)
        return;

    impl->touchEvent(touches, event, self, _cmd);
}

BOOL wxOSX_becomeFirstResponder(UIView* self, SEL _cmd)
{
    wxWidgetIPhoneImpl* impl = (wxWidgetIPhoneImpl* ) wxWidgetImpl::FindFromWXWidget( self );
    if (impl == NULL)
        return NO;

    return impl->becomeFirstResponder(self, _cmd);
}

BOOL wxOSX_resignFirstResponder(UIView* self, SEL _cmd)
{
    wxWidgetIPhoneImpl* impl = (wxWidgetIPhoneImpl* ) wxWidgetImpl::FindFromWXWidget( self );
    if (impl == NULL)
        return NO;

    return impl->resignFirstResponder(self, _cmd);
}

void wxOSX_drawRect(UIView* self, SEL _cmd, CGRect rect)
{
    wxWidgetIPhoneImpl* impl = (wxWidgetIPhoneImpl* ) wxWidgetImpl::FindFromWXWidget( self );
    if (impl == NULL)
        return;

    return impl->drawRect(&rect, self, _cmd);
}


void wxOSXIPhoneClassAddWXMethods(Class c)
{
    class_addMethod(c, @selector(touchesBegan:withEvent:), (IMP) wxOSX_touchEvent, "v@:@@");
    class_addMethod(c, @selector(touchesMoved:withEvent:), (IMP) wxOSX_touchEvent, "v@:@@");
    class_addMethod(c, @selector(touchesEnded:withEvent:), (IMP) wxOSX_touchEvent, "v@:@@");
    class_addMethod(c, @selector(becomeFirstResponder), (IMP) wxOSX_becomeFirstResponder, "c@:" );
    class_addMethod(c, @selector(resignFirstResponder), (IMP) wxOSX_resignFirstResponder, "c@:" );
    class_addMethod(c, @selector(drawRect:), (IMP) wxOSX_drawRect, "v@:{_CGRect={_CGPoint=ff}{_CGSize=ff}}" );
}

@implementation wxUIContentView

@end

@implementation wxUIContentViewController

- (BOOL) shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation) interfaceOrientation
{
    return YES;
}

- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation
{
    CGRect fr = [self.view frame];
//    CGRect cv = [[self.view superview] frame];
//    CGRect bounds = CGRectMake( 0,0,fr.size.width, fr.size.height);
//    [[self.view superview] setFrame: fr ];
//    [self.view setFrame: bounds];
//    [self.view setNeedsDisplayInRect:bounds];
}

@end


IMPLEMENT_DYNAMIC_CLASS( wxWidgetIPhoneImpl , wxWidgetImpl )

wxWidgetIPhoneImpl::wxWidgetIPhoneImpl( wxWindowMac* peer , WXWidget w, bool isRootControl ) :
    wxWidgetImpl( peer, isRootControl ), m_osxView(w)
{
}

wxWidgetIPhoneImpl::wxWidgetIPhoneImpl()
{
}

void wxWidgetIPhoneImpl::Init()
{
    m_osxView = NULL;
}

wxWidgetIPhoneImpl::~wxWidgetIPhoneImpl()
{
    RemoveAssociations( this );

    if ( !IsRootControl() )
    {
        UIView *sv = [m_osxView superview];
        if ( sv != nil )
            [m_osxView removeFromSuperview];
    }
    [m_osxView release];
}

bool wxWidgetIPhoneImpl::IsVisible() const
{
    UIView* view = m_osxView;
    while ( view != nil && [view isHidden] == NO )
    {
        view = [view superview];
        if (view != nil && [view isHidden])
            NSLog(@"visible test failed for %@",view); 
    }
    return view == nil;
}

void wxWidgetIPhoneImpl::SetVisibility( bool visible )
{
    [m_osxView setHidden:(visible ? NO:YES)];
}

void wxWidgetIPhoneImpl::Raise()
{
    [[m_osxView superview] bringSubviewToFront:m_osxView];
}

void wxWidgetIPhoneImpl::Lower()
{
    [[m_osxView superview] sendSubviewToBack:m_osxView];
}

void wxWidgetIPhoneImpl::ScrollRect( const wxRect *rect, int dx, int dy )
{
    SetNeedsDisplay() ;
}

void wxWidgetIPhoneImpl::Move(int x, int y, int width, int height)
{
    CGRect r = CGRectMake( x, y, width, height) ;
    [m_osxView setFrame:r];
}

void wxWidgetIPhoneImpl::GetPosition( int &x, int &y ) const
{
    CGRect r = [m_osxView frame];
    x = r.origin.x;
    y = r.origin.y;
}

void wxWidgetIPhoneImpl::GetSize( int &width, int &height ) const
{
    CGRect rect = [m_osxView frame];
    width = rect.size.width;
    height = rect.size.height;
}

void wxWidgetIPhoneImpl::GetContentArea( int&left, int &top, int &width, int &height ) const
{
    left = top = 0;
    GetSize( width, height );
}

void wxWidgetIPhoneImpl::SetNeedsDisplay( const wxRect* where )
{
    if ( where )
    {
        CGRect r = CGRectMake( where->x, where->y, where->width, where->height) ;
        [m_osxView setNeedsDisplayInRect:r];
    }
    else
        [m_osxView setNeedsDisplay];
}

bool wxWidgetIPhoneImpl::GetNeedsDisplay() const
{
    return false;
//    return [m_osxView needsDisplay];
}

bool wxWidgetIPhoneImpl::CanFocus() const
{
    return [m_osxView canBecomeFirstResponder] == YES;
    // ? return [m_osxView isUserInteractionEnabled] == YES;
}

bool wxWidgetIPhoneImpl::HasFocus() const
{
    return [m_osxView isFirstResponder] == YES;
}

bool wxWidgetIPhoneImpl::SetFocus()
{
//    [m_osxView makeKeyWindow] ;
//  TODO
    return false;
}


void wxWidgetIPhoneImpl::RemoveFromParent()
{
    [m_osxView removeFromSuperview];
}

void wxWidgetIPhoneImpl::Embed( wxWidgetImpl *parent )
{
    UIView* container = parent->GetWXWidget() ;
    wxASSERT_MSG( container != NULL , wxT("No valid mac container control") ) ;
    [container addSubview:m_osxView];
}

void  wxWidgetImpl::Convert( wxPoint *pt , wxWidgetImpl *from , wxWidgetImpl *to )
{
    CGPoint p = CGPointMake( pt->x , pt->y );
    p = [from->GetWXWidget() convertPoint:p toView:to->GetWXWidget() ];
    pt->x = (int)p.x;
    pt->y = (int)p.y;
}

void wxWidgetIPhoneImpl::SetBackgroundColour( const wxColour &col )
{
    // m_osxView.backgroundColor = [[UIColor alloc] initWithCGColor:col.GetCGColor()];
}

void wxWidgetIPhoneImpl::SetLabel(const wxString& title, wxFontEncoding encoding)
{
    if ( [m_osxView respondsToSelector:@selector(setTitle:forState:) ] )
    {
        wxCFStringRef cf( title , encoding );
        [m_osxView setTitle:cf.AsNSString() forState:UIControlStateNormal ];
    }
    else if ( [m_osxView respondsToSelector:@selector(setStringValue:) ] )
    {
        wxCFStringRef cf( title , encoding );
        [m_osxView setStringValue:cf.AsNSString()];
    }
}


void wxWidgetIPhoneImpl::SetCursor( const wxCursor & cursor )
{
}

void wxWidgetIPhoneImpl::CaptureMouse()
{
}

void wxWidgetIPhoneImpl::ReleaseMouse()
{
}

wxInt32 wxWidgetIPhoneImpl::GetValue() const
{
}

void wxWidgetIPhoneImpl::SetValue( wxInt32 v )
{
}

void wxWidgetIPhoneImpl::SetBitmap( const wxBitmap& bitmap )
{
}

wxBitmap wxWidgetIPhoneImpl::GetBitmap() const
{
    wxBitmap bmp;
    return bmp;
}

void wxWidgetIPhoneImpl::SetBitmapPosition( wxDirection dir )
{
}

void wxWidgetIPhoneImpl::SetupTabs( const wxNotebook &notebook )
{
}

void wxWidgetIPhoneImpl::GetBestRect( wxRect *r ) const
{
    r->x = r->y = r->width = r->height = 0;

    if (  [m_osxView respondsToSelector:@selector(sizeToFit)] )
    {
        CGRect former = [m_osxView frame];
        [m_osxView sizeToFit];
        CGRect best = [m_osxView frame];
        [m_osxView setFrame:former];
        r->width = best.size.width;
        r->height = best.size.height;
    }
}

bool wxWidgetIPhoneImpl::IsEnabled() const
{
}

void wxWidgetIPhoneImpl::Enable( bool enable )
{
}

void wxWidgetIPhoneImpl::SetMinimum( wxInt32 v )
{
}

void wxWidgetIPhoneImpl::SetMaximum( wxInt32 v )
{
}

wxInt32 wxWidgetIPhoneImpl::GetMinimum() const
{
}

wxInt32 wxWidgetIPhoneImpl::GetMaximum() const
{
}

void wxWidgetIPhoneImpl::PulseGauge()
{
}

void wxWidgetIPhoneImpl::SetScrollThumb( wxInt32 value, wxInt32 thumbSize )
{
}

void wxWidgetIPhoneImpl::SetControlSize( wxWindowVariant variant )
{
}

void wxWidgetIPhoneImpl::SetFont( const wxFont & font , const wxColour& foreground , long windowStyle, bool ignoreBlack )
{
}

void wxWidgetIPhoneImpl::InstallEventHandler( WXWidget control )
{
    WXWidget c =  control ? control : (WXWidget) m_osxView;
    wxWidgetImpl::Associate( c, this ) ;

   if ([c isKindOfClass:[UIControl class] ])
    {
        UIControl* cc = (UIControl*) c;
        /*
        [cc addTarget:self action:@selector(touchUpInsideAction:event:) forControlEvents:UIControlEventTouchUpInside];
        */
    }
}

void wxWidgetIPhoneImpl::DoNotifyFocusEvent(bool receivedFocus, wxWidgetImpl* otherWindow)
{
    wxWindow* thisWindow = GetWXPeer();
    if ( thisWindow->MacGetTopLevelWindow() && NeedsFocusRect() )
    {
        thisWindow->MacInvalidateBorders();
    }

    if ( receivedFocus )
    {
        wxLogTrace(wxT("Focus"), wxT("focus set(%p)"), static_cast<void*>(thisWindow));
        wxChildFocusEvent eventFocus((wxWindow*)thisWindow);
        thisWindow->HandleWindowEvent(eventFocus);

#if wxUSE_CARET
        if ( thisWindow->GetCaret() )
            thisWindow->GetCaret()->OnSetFocus();
#endif

        wxFocusEvent event(wxEVT_SET_FOCUS, thisWindow->GetId());
        event.SetEventObject(thisWindow);
        if (otherWindow)
            event.SetWindow(otherWindow->GetWXPeer());
        thisWindow->HandleWindowEvent(event) ;
    }
    else // !receivedFocuss
    {
#if wxUSE_CARET
        if ( thisWindow->GetCaret() )
            thisWindow->GetCaret()->OnKillFocus();
#endif

        wxLogTrace(wxT("Focus"), wxT("focus lost(%p)"), static_cast<void*>(thisWindow));

        wxFocusEvent event( wxEVT_KILL_FOCUS, thisWindow->GetId());
        event.SetEventObject(thisWindow);
        if (otherWindow)
            event.SetWindow(otherWindow->GetWXPeer());
        thisWindow->HandleWindowEvent(event) ;
    }
}

typedef void (*wxOSX_DrawRectHandlerPtr)(UIView* self, SEL _cmd, CGRect rect);
typedef BOOL (*wxOSX_FocusHandlerPtr)(UIView* self, SEL _cmd);

bool wxWidgetIPhoneImpl::becomeFirstResponder(WXWidget slf, void *_cmd)
{
    wxOSX_FocusHandlerPtr superimpl = (wxOSX_FocusHandlerPtr) [[slf superclass] instanceMethodForSelector:(SEL)_cmd];
    // get the current focus before running becomeFirstResponder
    UIView* otherView = FindFocus();
    wxWidgetImpl* otherWindow = FindFromWXWidget(otherView);
    BOOL r = superimpl(slf, (SEL)_cmd);
    if ( r )
    {
        DoNotifyFocusEvent( true, otherWindow );
    }
    return r;
}

bool wxWidgetIPhoneImpl::resignFirstResponder(WXWidget slf, void *_cmd)
{
    wxOSX_FocusHandlerPtr superimpl = (wxOSX_FocusHandlerPtr) [[slf superclass] instanceMethodForSelector:(SEL)_cmd];
    BOOL r = superimpl(slf, (SEL)_cmd);
    // get the current focus after running resignFirstResponder
    UIView* otherView = FindFocus();
    wxWidgetImpl* otherWindow = FindFromWXWidget(otherView);
    // NSTextViews have an editor as true responder, therefore the might get the
    // resign notification if their editor takes over, don't trigger any event hen
    if ( r && otherWindow != this)
    {
        DoNotifyFocusEvent( false, otherWindow );
    }
    return r;
}

void wxWidgetIPhoneImpl::drawRect(CGRect* rect, WXWidget slf, void *WXUNUSED(_cmd))
{
    CGContextRef context = (CGContextRef) UIGraphicsGetCurrentContext();
    CGContextSaveGState( context );
    // draw background

    CGContextSetFillColorWithColor( context, GetWXPeer()->GetBackgroundColour().GetCGColor());
    CGContextFillRect(context, *rect );

    GetWXPeer()->MacSetCGContextRef( context );

    GetWXPeer()->GetUpdateRegion() =
        wxRegion(rect->origin.x,rect->origin.y,rect->size.width,rect->size.height) ;

    wxRegion updateRgn( wxFromNSRect( slf, *rect ) );

    wxWindow* wxpeer = GetWXPeer();
    wxpeer->GetUpdateRegion() = updateRgn;
    wxpeer->MacSetCGContextRef( context );

    bool handled = wxpeer->MacDoRedraw( 0 );

    CGContextRestoreGState( context );

    CGContextSaveGState( context );
    if ( !handled )
    {
        // call super
        SEL _cmd = @selector(drawRect:);
        wxOSX_DrawRectHandlerPtr superimpl = (wxOSX_DrawRectHandlerPtr) [[slf superclass] instanceMethodForSelector:_cmd];
        if ( superimpl != wxOSX_drawRect )
        {
            superimpl(slf, _cmd, *rect);
            CGContextRestoreGState( context );
            CGContextSaveGState( context );
        }
    }
    wxpeer->MacPaintChildrenBorders();
    wxpeer->MacSetCGContextRef( NULL );

    CGContextRestoreGState( context );
}

void wxWidgetIPhoneImpl::touchEvent(NSSet* touches, UIEvent *event, WXWidget slf, void *WXUNUSED(_cmd))
{
    CGPoint clickLocation;
    UITouch *touch = [touches anyObject];
    clickLocation = [touch locationInView:slf];
    wxPoint pt = wxFromNSPoint( m_osxView, clickLocation );

    wxMouseEvent wxevent(wxEVT_LEFT_DOWN);
    SetupMouseEvent( wxevent , touches, event ) ;
    wxevent.m_x = pt.x;
    wxevent.m_y = pt.y;
    wxevent.SetEventObject( GetWXPeer() ) ;
    //?wxevent.SetId( GetWXPeer()->GetId() ) ;

    GetWXPeer()->HandleWindowEvent(wxevent);
}

void wxWidgetIPhoneImpl::touchUpInsideAction(void* sender, WX_UIEvent evt, WXWidget slf, void* _cmd)
{
}

//
// Factory methods
//

wxWidgetImpl* wxWidgetImpl::CreateUserPane( wxWindowMac* wxpeer, wxWindowMac* WXUNUSED(parent),
    wxWindowID WXUNUSED(id), const wxPoint& pos, const wxSize& size,
    long WXUNUSED(style), long WXUNUSED(extraStyle))
{
    UIView* sv = (wxpeer->GetParent()->GetHandle() );

    CGRect r = CGRectMake( pos.x, pos.y, size.x, size.y) ;
    // Rect bounds = wxMacGetBoundsForControl( wxpeer, pos , size ) ;
    wxUIView* v = [[wxUIView alloc] initWithFrame:r];
    sv.clipsToBounds = YES;
    sv.contentMode =  UIViewContentModeRedraw;
    sv.clearsContextBeforeDrawing = NO;
    wxWidgetIPhoneImpl* c = new wxWidgetIPhoneImpl( wxpeer, v );
    return c;
}

wxWidgetImpl* wxWidgetImpl::CreateContentView( wxNonOwnedWindow* now )
{
    UIWindow* toplevelwindow = now->GetWXWindow();

    wxUIContentView* contentview = [[wxUIContentView alloc] initWithFrame:[toplevelwindow bounds]];
    contentview.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
    wxUIContentViewController* controller = [[wxUIContentViewController alloc] init];
    controller.view = contentview;
    /*
    UIInterfaceOrientation orientation = [[UIApplication sharedApplication] statusBarOrientation];
    // left orientation not yet implemented !
    if (orientation == UIInterfaceOrientationLandscapeRight )
    {
        CGAffineTransform transform = v.transform;

        // Use the status bar frame to determine the center point of the window's content area.
        CGRect statusBarFrame = [[UIApplication sharedApplication] statusBarFrame];
        CGRect bounds = CGRectMake(0, 0, statusBarFrame.size.height, statusBarFrame.origin.x);
        CGPoint center = CGPointMake(bounds.size.height / 2.0, bounds.size.width / 2.0);

        // Set the center point of the view to the center point of the window's content area.
        v.center = center;

        // Rotate the view 90 degrees around its new center point.
        transform = CGAffineTransformRotate(transform, ( M_PI / 2.0));
        v.transform = transform;
    }
    */
    wxWidgetIPhoneImpl* impl = new wxWidgetIPhoneImpl( now, contentview, true );
    impl->InstallEventHandler();
    [toplevelwindow addSubview:contentview];
    return impl;
}

