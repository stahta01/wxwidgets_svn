/////////////////////////////////////////////////////////////////////////////
// Name:        src/mac/carbon/utils.cpp
// Purpose:     Various utilities
// Author:      Stefan Csomor
// Modified by:
// Created:     1998-01-01
// RCS-ID:      $Id$
// Copyright:   (c) Stefan Csomor
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#include "wx/utils.h"

#ifndef WX_PRECOMP
    #include "wx/intl.h"
    #include "wx/app.h"
    #if wxUSE_GUI
        #include "wx/toplevel.h"
        #include "wx/font.h"
    #endif
#endif

#include "wx/apptrait.h"

#if wxUSE_GUI
    #include "wx/osx/uma.h"
#endif

#include <ctype.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

// #include "MoreFilesX.h"

#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_5
    #include <AudioToolbox/AudioServices.h>
#endif

#if wxUSE_GUI
    #include <CoreServices/CoreServices.h>
    #include <Carbon/Carbon.h>
    #include "wx/osx/private/timer.h"
#endif // wxUSE_GUI

#include "wx/evtloop.h"
#include "wx/osx/private.h"

#if defined(__MWERKS__) && wxUSE_UNICODE
#if __MWERKS__ < 0x4100
    #include <wtime.h>
#endif
#endif

#if wxUSE_BASE

// our OS version is the same in non GUI and GUI cases
wxOperatingSystemId wxGetOsVersion(int *majorVsn, int *minorVsn)
{
    SInt32 theSystem;
    Gestalt(gestaltSystemVersion, &theSystem);

    if ( majorVsn != NULL )
        *majorVsn = (theSystem >> 8);

    if ( minorVsn != NULL )
        *minorVsn = (theSystem & 0xFF);

#if defined( __DARWIN__ )
    return wxOS_MAC_OSX_DARWIN;
#else
    return wxOS_MAC_OS;
#endif
}

extern bool WXDLLEXPORT wxIsDebuggerRunning()
{
    // TODO : try to find out ...
    return false;
}

// Emit a beeeeeep
void wxBell()
{
#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_5
    if ( AudioServicesPlayAlertSound )
        AudioServicesPlayAlertSound(kUserPreferredAlert);
    else
#endif
#if MAC_OS_X_VERSION_MIN_REQUIRED < MAC_OS_X_VERSION_10_5
        SysBeep(30);
#else
    {
    }
#endif
}


#endif // wxUSE_BASE

#if wxUSE_GUI

wxPortId wxGUIAppTraits::GetToolkitVersion(int *verMaj, int *verMin) const
{
    // We suppose that toolkit version is the same as OS version under Mac
    wxGetOsVersion(verMaj, verMin);

    return wxPORT_MAC;
}

wxEventLoopBase* wxGUIAppTraits::CreateEventLoop()
{
    return new wxEventLoop;
}

wxTimerImpl* wxGUIAppTraits::CreateTimerImpl(wxTimer *timer)
{
    return new wxCarbonTimerImpl(timer);
}

int gs_wxBusyCursorCount = 0;
extern wxCursor    gMacCurrentCursor;
wxCursor        gMacStoredActiveCursor;

// Set the cursor to the busy cursor for all windows
void wxBeginBusyCursor(const wxCursor *cursor)
{
    if (gs_wxBusyCursorCount++ == 0)
    {
        gMacStoredActiveCursor = gMacCurrentCursor;
        cursor->MacInstall();

        wxSetCursor(*cursor);
    }
    //else: nothing to do, already set
}

// Restore cursor to normal
void wxEndBusyCursor()
{
    wxCHECK_RET( gs_wxBusyCursorCount > 0,
        wxT("no matching wxBeginBusyCursor() for wxEndBusyCursor()") );

    if (--gs_wxBusyCursorCount == 0)
    {
        gMacStoredActiveCursor.MacInstall();
        gMacStoredActiveCursor = wxNullCursor;

        wxSetCursor(wxNullCursor);
    }
}

// true if we're between the above two calls
bool wxIsBusy()
{
    return (gs_wxBusyCursorCount > 0);
}

#endif // wxUSE_GUI

#if wxUSE_BASE

wxString wxMacFindFolderNoSeparator( short        vol,
              OSType       folderType,
              Boolean      createFolder)
{
    FSRef fsRef;
    wxString strDir;

    if ( FSFindFolder( vol, folderType, createFolder, &fsRef) == noErr)
    {
        strDir = wxMacFSRefToPath( &fsRef );
    }

    return strDir;
}

wxString wxMacFindFolder( short        vol,
              OSType       folderType,
              Boolean      createFolder)
{
    return wxMacFindFolderNoSeparator(vol, folderType, createFolder) + wxFILE_SEP_PATH;
}

#endif // wxUSE_BASE

#if wxUSE_GUI

// Check whether this window wants to process messages, e.g. Stop button
// in long calculations.
bool wxCheckForInterrupt(wxWindow *WXUNUSED(wnd))
{
    // TODO
    return false;
}

void wxGetMousePosition( int* x, int* y )
{
#if wxMAC_USE_QUICKDRAW
    Point pt;
    GetGlobalMouse(&pt);
    *x = pt.h;
    *y = pt.v;
#else
    // TODO
#endif
};

// Return true if we have a colour display
bool wxColourDisplay()
{
    return true;
}

// Returns depth of screen
int wxDisplayDepth()
{
#if wxMAC_USE_QUICKDRAW
    int theDepth = (int) CGDisplayBitsPerPixel(CGMainDisplayID());
    Rect globRect;
    SetRect(&globRect, -32760, -32760, 32760, 32760);
    GDHandle    theMaxDevice;

    theMaxDevice = GetMaxDevice(&globRect);
    if (theMaxDevice != NULL)
        theDepth = (**(**theMaxDevice).gdPMap).pixelSize;

    return theDepth;
#else
    return 32; // TODO
#endif
}

// Get size of display
void wxDisplaySize(int *width, int *height)
{
    // TODO adapt for multi-displays
    CGRect bounds = CGDisplayBounds(CGMainDisplayID());
    if ( width )
        *width = (int)bounds.size.width ;
    if ( height )
        *height = (int)bounds.size.height;
}

void wxDisplaySizeMM(int *width, int *height)
{
    wxDisplaySize(width, height);
    // on mac 72 is fixed (at least now;-)
    float cvPt2Mm = 25.4 / 72;

    if (width != NULL)
        *width = int( *width * cvPt2Mm );

    if (height != NULL)
        *height = int( *height * cvPt2Mm );
}

void wxClientDisplayRect(int *x, int *y, int *width, int *height)
{
#if MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5
#if wxMAC_USE_QUICKDRAW

        HIRect bounds ;
        HIWindowGetAvailablePositioningBounds(kCGNullDirectDisplay,kHICoordSpace72DPIGlobal,
            &bounds);
    if ( x )
        *x = bounds.origin.x;
    if ( y )
        *y = bounds.origin.y;
    if ( width )
        *width = bounds.size.width;
    if ( height )
        *height = bounds.size.height;
#else
    int w, h;
    wxDisplaySize(&w,&h);
    if ( x )
        *x = 0;
    if ( y )
        *y = 24;
    if ( width )
        *width = w;
    if ( height )
        *height = h-24;
#endif
#else
    Rect r;
    GetAvailableWindowPositioningBounds( GetMainDevice() , &r );
    if ( x )
        *x = r.left;
    if ( y )
        *y = r.top;
    if ( width )
        *width = r.right - r.left;
    if ( height )
        *height = r.bottom - r.top;
#endif
}

wxWindow* wxFindWindowAtPoint(const wxPoint& pt)
{
    return wxGenericFindWindowAtPoint(pt);
}

#endif // wxUSE_GUI

#if wxUSE_BASE

#include <sys/utsname.h>

wxString wxGetOsDescription()
{
    struct utsname name;
    uname(&name);
    return wxString::Format(_T("Mac OS X (%s %s %s)"),
            wxString::FromAscii(name.sysname).c_str(),
            wxString::FromAscii(name.release).c_str(),
            wxString::FromAscii(name.machine).c_str());
}

#ifndef __DARWIN__
wxString wxGetUserHome (const wxString& user)
{
    // TODO
    return wxString();
}

bool wxGetDiskSpace(const wxString& path, wxDiskspaceSize_t *pTotal, wxDiskspaceSize_t *pFree)
{
    if ( path.empty() )
        return false;

    wxString p = path;
    if (p[0u] == ':' )
      p = wxGetCwd() + p;

    int pos = p.Find(':');
    if ( pos != wxNOT_FOUND )
      p = p.Mid(1,pos);

    p = p + wxT(":");

    OSErr err = noErr;

    FSRef fsRef;
    err = wxMacPathToFSRef( p , &fsRef );
    if ( noErr == err )
    {
        FSVolumeRefNum vRefNum;
        err = FSGetVRefNum( &fsRef , &vRefNum );
        if ( noErr == err )
        {
            UInt64 freeBytes , totalBytes;
            err = FSGetVInfo( vRefNum , NULL , &freeBytes , &totalBytes );
            if ( noErr == err )
            {
                if ( pTotal )
                    *pTotal = wxDiskspaceSize_t( totalBytes );
                if ( pFree )
                    *pFree = wxDiskspaceSize_t( freeBytes );
            }
        }
    }

    return err == noErr;
}
#endif // !__DARWIN__

//---------------------------------------------------------------------------
// wxMac Specific utility functions
//---------------------------------------------------------------------------

void wxMacStringToPascal( const wxString&from , StringPtr to )
{
    wxCharBuffer buf = from.mb_str( wxConvLocal );
    int len = strlen(buf);

    if ( len > 255 )
        len = 255;
    to[0] = len;
    memcpy( (char*) &to[1] , buf , len );
}

wxString wxMacMakeStringFromPascal( ConstStringPtr from )
{
    return wxString( (char*) &from[1] , wxConvLocal , from[0] );
}

// ----------------------------------------------------------------------------
// Common Event Support
// ----------------------------------------------------------------------------

void wxMacWakeUp()
{
    OSStatus err = noErr;

#if 0
    // lead sometimes to race conditions, although all calls used should be thread safe ...
    static wxMacCarbonEvent s_wakeupEvent;
    if ( !s_wakeupEvent.IsValid() )
    {
       err = s_wakeupEvent.Create( 'WXMC', 'WXMC', GetCurrentEventTime(),
                    kEventAttributeNone );
    }
    if ( err == noErr )
    {

        if ( IsEventInQueue( GetMainEventQueue() , s_wakeupEvent ) )
            return;
        s_wakeupEvent.SetCurrentTime();
        err = PostEventToQueue(GetMainEventQueue(), s_wakeupEvent,
                              kEventPriorityHigh );
    }
#else
    wxMacCarbonEvent wakeupEvent;
    wakeupEvent.Create( 'WXMC', 'WXMC', GetCurrentEventTime(),
                        kEventAttributeNone );
    err = PostEventToQueue(GetMainEventQueue(), wakeupEvent,
                           kEventPriorityHigh );
#endif
}

#endif // wxUSE_BASE

#if wxUSE_GUI

// ----------------------------------------------------------------------------
// Native Struct Conversions
// ----------------------------------------------------------------------------

void wxMacRectToNative( const wxRect *wx , Rect *n )
{
    n->left = wx->x;
    n->top = wx->y;
    n->right = wx->x + wx->width;
    n->bottom = wx->y + wx->height;
}

void wxMacNativeToRect( const Rect *n , wxRect* wx )
{
    wx->x = n->left;
    wx->y = n->top;
    wx->width = n->right - n->left;
    wx->height = n->bottom - n->top;
}

void wxMacPointToNative( const wxPoint* wx , Point *n )
{
    n->h = wx->x;
    n->v = wx->y;
}

void wxMacNativeToPoint( const Point *n , wxPoint* wx )
{
    wx->x = n->h;
    wx->y = n->v;
}

// ----------------------------------------------------------------------------
// Carbon Event Support
// ----------------------------------------------------------------------------

OSStatus wxMacCarbonEvent::GetParameter(EventParamName inName, EventParamType inDesiredType, UInt32 inBufferSize, void * outData)
{
    return ::GetEventParameter( m_eventRef , inName , inDesiredType , NULL , inBufferSize , NULL , outData );
}

OSStatus wxMacCarbonEvent::SetParameter(EventParamName inName, EventParamType inType, UInt32 inBufferSize, const void * inData)
{
    return ::SetEventParameter( m_eventRef , inName , inType , inBufferSize , inData );
}

// ----------------------------------------------------------------------------
// Control Access Support
// ----------------------------------------------------------------------------

#if wxMAC_USE_QUICKDRAW

IMPLEMENT_DYNAMIC_CLASS( wxMacControl , wxObject )

wxMacControl::wxMacControl()
{
    Init();
}

wxMacControl::wxMacControl(wxWindow* peer , bool isRootControl )
{
    Init();
    m_peer = peer;
    m_isRootControl = isRootControl;
}

wxMacControl::wxMacControl( wxWindow* peer , ControlRef control )
{
    Init();
    m_peer = peer;
    m_controlRef = control;
}

wxMacControl::wxMacControl( wxWindow* peer , WXWidget control )
{
    Init();
    m_peer = peer;
    m_controlRef = (ControlRef) control;
}

wxMacControl::~wxMacControl()
{
}

void wxMacControl::Init()
{
    m_peer = NULL;
    m_controlRef = NULL;
    m_needsFocusRect = false;
    m_isRootControl = false;
}

void wxMacControl::Dispose()
{
    wxASSERT_MSG( m_controlRef != NULL , wxT("Control Handle already NULL, Dispose called twice ?") );
    wxASSERT_MSG( IsValidControlHandle(m_controlRef) , wxT("Invalid Control Handle (maybe already released) in Dispose") );

    // we cannot check the ref count here anymore, as autorelease objects might delete their refs later
    // we can have situations when being embedded, where the control gets deleted behind our back, so only
    // CFRelease if we are safe
    if ( IsValidControlHandle(m_controlRef) )
        CFRelease(m_controlRef);
    m_controlRef = NULL;
}

void wxMacControl::SetReference( URefCon data )
{
    SetControlReference( m_controlRef , data );
}

OSStatus wxMacControl::GetData(ControlPartCode inPartCode , ResType inTag , Size inBufferSize , void * inOutBuffer , Size * outActualSize ) const
{
    return ::GetControlData( m_controlRef , inPartCode , inTag , inBufferSize , inOutBuffer , outActualSize );
}

OSStatus wxMacControl::GetDataSize(ControlPartCode inPartCode , ResType inTag , Size * outActualSize ) const
{
    return ::GetControlDataSize( m_controlRef , inPartCode , inTag , outActualSize );
}

OSStatus wxMacControl::SetData(ControlPartCode inPartCode , ResType inTag , Size inSize , const void * inData)
{
    return ::SetControlData( m_controlRef , inPartCode , inTag , inSize , inData );
}

OSStatus wxMacControl::SendEvent( EventRef event , OptionBits inOptions )
{
    return SendEventToEventTargetWithOptions( event,
        HIObjectGetEventTarget( (HIObjectRef) m_controlRef ), inOptions );
}

OSStatus wxMacControl::SendHICommand( HICommand &command , OptionBits inOptions )
{
    wxMacCarbonEvent event( kEventClassCommand , kEventCommandProcess );

    event.SetParameter<HICommand>(kEventParamDirectObject,command);

    return SendEvent( event , inOptions );
}

OSStatus wxMacControl::SendHICommand( UInt32 commandID , OptionBits inOptions  )
{
    HICommand command;

    memset( &command, 0 , sizeof(command) );
    command.commandID = commandID;
    return SendHICommand( command , inOptions );
}

void wxMacControl::Flash( ControlPartCode part , UInt32 ticks )
{
    unsigned long finalTicks;

    HiliteControl( m_controlRef , part );
    Delay( ticks , &finalTicks );
    HiliteControl( m_controlRef , kControlNoPart );
}

SInt32 wxMacControl::GetValue() const
{
    return ::GetControl32BitValue( m_controlRef );
}

SInt32 wxMacControl::GetMaximum() const
{
    return ::GetControl32BitMaximum( m_controlRef );
}

SInt32 wxMacControl::GetMinimum() const
{
    return ::GetControl32BitMinimum( m_controlRef );
}

void wxMacControl::SetValue( SInt32 v )
{
    ::SetControl32BitValue( m_controlRef , v );
}

void wxMacControl::SetMinimum( SInt32 v )
{
    ::SetControl32BitMinimum( m_controlRef , v );
}

void wxMacControl::SetMaximum( SInt32 v )
{
    ::SetControl32BitMaximum( m_controlRef , v );
}

void wxMacControl::SetValueAndRange( SInt32 value , SInt32 minimum , SInt32 maximum )
{
    ::SetControl32BitMinimum( m_controlRef , minimum );
    ::SetControl32BitMaximum( m_controlRef , maximum );
    ::SetControl32BitValue( m_controlRef , value );
}

OSStatus wxMacControl::SetFocus( ControlFocusPart focusPart )
{
    return SetKeyboardFocus( GetControlOwner( m_controlRef ), m_controlRef, focusPart );
}

bool wxMacControl::HasFocus() const
{
    ControlRef control;
    GetKeyboardFocus( GetUserFocusWindow() , &control );
    return control == m_controlRef;
}

void wxMacControl::SetNeedsFocusRect( bool needs )
{
    m_needsFocusRect = needs;
}

bool wxMacControl::NeedsFocusRect() const
{
    return m_needsFocusRect;
}

void wxMacControl::VisibilityChanged(bool WXUNUSED(shown))
{
}

void wxMacControl::SuperChangedPosition()
{
}

void wxMacControl::SetFont( const wxFont & font , const wxColour& foreground , long windowStyle )
{
    m_font = font;
#if wxMAC_USE_CORE_TEXT
    if ( UMAGetSystemVersion() >= 0x1050 )
    {
        HIViewPartCode part = 0;
        HIThemeTextHorizontalFlush flush = kHIThemeTextHorizontalFlushDefault;
        if ( ( windowStyle & wxALIGN_MASK ) & wxALIGN_CENTER_HORIZONTAL )
            flush = kHIThemeTextHorizontalFlushCenter;
        else if ( ( windowStyle & wxALIGN_MASK ) & wxALIGN_RIGHT )
            flush = kHIThemeTextHorizontalFlushRight;
        HIViewSetTextFont( m_controlRef , part , (CTFontRef) font.MacGetCTFont() );
        HIViewSetTextHorizontalFlush( m_controlRef, part, flush );

        if ( foreground != *wxBLACK )
        {
            ControlFontStyleRec fontStyle;
            foreground.GetRGBColor( &fontStyle.foreColor );
            fontStyle.flags = kControlUseForeColorMask;
            ::SetControlFontStyle( m_controlRef , &fontStyle );
        }

    }
#endif
#if wxMAC_USE_ATSU_TEXT
    ControlFontStyleRec fontStyle;
    if ( font.MacGetThemeFontID() != kThemeCurrentPortFont )
    {
        switch ( font.MacGetThemeFontID() )
        {
            case kThemeSmallSystemFont :
                fontStyle.font = kControlFontSmallSystemFont;
                break;

            case 109 : // mini font
                fontStyle.font = -5;
                break;

            case kThemeSystemFont :
                fontStyle.font = kControlFontBigSystemFont;
                break;

            default :
                fontStyle.font = kControlFontBigSystemFont;
                break;
        }

        fontStyle.flags = kControlUseFontMask;
    }
    else
    {
        fontStyle.font = font.MacGetFontNum();
        fontStyle.style = font.MacGetFontStyle();
        fontStyle.size = font.MacGetFontSize();
        fontStyle.flags = kControlUseFontMask | kControlUseFaceMask | kControlUseSizeMask;
    }

    fontStyle.just = teJustLeft;
    fontStyle.flags |= kControlUseJustMask;
    if ( ( windowStyle & wxALIGN_MASK ) & wxALIGN_CENTER_HORIZONTAL )
        fontStyle.just = teJustCenter;
    else if ( ( windowStyle & wxALIGN_MASK ) & wxALIGN_RIGHT )
        fontStyle.just = teJustRight;


    // we only should do this in case of a non-standard color, as otherwise 'disabled' controls
    // won't get grayed out by the system anymore

    if ( foreground != *wxBLACK )
    {
        foreground.GetRGBColor( &fontStyle.foreColor );
        fontStyle.flags |= kControlUseForeColorMask;
    }

    ::SetControlFontStyle( m_controlRef , &fontStyle );
#endif
}

void wxMacControl::SetBackgroundColour( const wxColour &WXUNUSED(col) )
{
//    HITextViewSetBackgroundColor( m_textView , color );
}

void wxMacControl::SetRange( SInt32 minimum , SInt32 maximum )
{
    ::SetControl32BitMinimum( m_controlRef , minimum );
    ::SetControl32BitMaximum( m_controlRef , maximum );
}

short wxMacControl::HandleKey( SInt16 keyCode,  SInt16 charCode, EventModifiers modifiers )
{
#ifndef __LP64__
    return HandleControlKey( m_controlRef , keyCode , charCode , modifiers );
#else
    return 0;
#endif
}

void wxMacControl::SetActionProc( ControlActionUPP   actionProc )
{
    SetControlAction( m_controlRef , actionProc );
}

void wxMacControl::SetViewSize( SInt32 viewSize )
{
    SetControlViewSize(m_controlRef , viewSize );
}

SInt32 wxMacControl::GetViewSize() const
{
    return GetControlViewSize( m_controlRef );
}

bool wxMacControl::IsVisible() const
{
    return IsControlVisible( m_controlRef );
}

void wxMacControl::SetVisibility( bool visible , bool redraw )
{
    SetControlVisibility( m_controlRef , visible , redraw );
}

bool wxMacControl::IsEnabled() const
{
    return IsControlEnabled( m_controlRef );
}

bool wxMacControl::IsActive() const
{
    return IsControlActive( m_controlRef );
}

void wxMacControl::Enable( bool enable )
{
    if ( enable )
        EnableControl( m_controlRef );
    else
        DisableControl( m_controlRef );
}

void wxMacControl::SetDrawingEnabled( bool enable )
{
    HIViewSetDrawingEnabled( m_controlRef , enable );
}

bool wxMacControl::GetNeedsDisplay() const
{
    return HIViewGetNeedsDisplay( m_controlRef );
}

void wxMacControl::SetNeedsDisplay( RgnHandle where )
{
    if ( !IsVisible() )
        return;

    HIViewSetNeedsDisplayInRegion( m_controlRef , where , true );
}

void wxMacControl::SetNeedsDisplay( Rect* where )
{
    if ( !IsVisible() )
        return;

    if ( where != NULL )
    {
        RgnHandle update = NewRgn();
        RectRgn( update , where );
        HIViewSetNeedsDisplayInRegion( m_controlRef , update , true );
        DisposeRgn( update );
    }
    else
        HIViewSetNeedsDisplay( m_controlRef , true );
}

void  wxMacControl::Convert( wxPoint *pt , wxMacControl *from , wxMacControl *to )
{
    HIPoint hiPoint;

    hiPoint.x = pt->x;
    hiPoint.y = pt->y;
    HIViewConvertPoint( &hiPoint , from->m_controlRef , to->m_controlRef  );
    pt->x = (int)hiPoint.x;
    pt->y = (int)hiPoint.y;
}

void wxMacControl::SetRect( Rect *r )
{
        //A HIRect is actually a CGRect on OSX - which consists of two structures -
        //CGPoint and CGSize, which have two floats each
        HIRect hir = { { r->left , r->top }, { r->right - r->left , r->bottom - r->top } };
        HIViewSetFrame ( m_controlRef , &hir );
        // eventuall we might have to do a SetVisibility( false , true );
        // before and a SetVisibility( true , true ); after
}

void wxMacControl::GetRect( Rect *r )
{
    GetControlBounds( m_controlRef , r );
}

void wxMacControl::GetRectInWindowCoords( Rect *r )
{
    GetControlBounds( m_controlRef , r ) ;

    WindowRef tlwref = GetControlOwner( m_controlRef ) ;

    wxNonOwnedWindow* tlwwx = wxFindWinFromMacWindow( tlwref ) ;
    if ( tlwwx != NULL )
    {
        ControlRef rootControl = tlwwx->GetPeer()->GetControlRef() ;
        HIPoint hiPoint = CGPointMake( 0 , 0 ) ;
        HIViewConvertPoint( &hiPoint , HIViewGetSuperview(m_controlRef) , rootControl ) ;
        OffsetRect( r , (short) hiPoint.x , (short) hiPoint.y ) ;
    }
}

void wxMacControl::GetBestRect( Rect *r )
{
    short   baselineoffset;

    GetBestControlRect( m_controlRef , r , &baselineoffset );
}

void wxMacControl::SetLabel( const wxString &title )
{
    wxFontEncoding encoding;

    if ( m_font.Ok() )
        encoding = m_font.GetEncoding();
    else
        encoding = wxFont::GetDefaultEncoding();

    SetControlTitleWithCFString( m_controlRef , wxCFStringRef( title , encoding ) );
}

void wxMacControl::GetFeatures( UInt32 * features )
{
    GetControlFeatures( m_controlRef , features );
}

OSStatus wxMacControl::GetRegion( ControlPartCode partCode , RgnHandle region )
{
    OSStatus err = GetControlRegion( m_controlRef , partCode , region );
    return err;
}

OSStatus wxMacControl::SetZOrder( bool above , wxMacControl* other )
{
    return HIViewSetZOrder( m_controlRef,above ? kHIViewZOrderAbove : kHIViewZOrderBelow,
       (other != NULL) ? other->m_controlRef : NULL);
}

// SetNeedsDisplay would not invalidate the children
static void InvalidateControlAndChildren( HIViewRef control )
{
    HIViewSetNeedsDisplay( control , true );
    UInt16 childrenCount = 0;
    OSStatus err = CountSubControls( control , &childrenCount );
    if ( err == errControlIsNotEmbedder )
        return;

    wxASSERT_MSG( err == noErr , wxT("Unexpected error when accessing subcontrols") );

    for ( UInt16 i = childrenCount; i >=1; --i )
    {
        HIViewRef child;

        err = GetIndexedSubControl( control , i , & child );
        if ( err == errControlIsNotEmbedder )
            return;

        InvalidateControlAndChildren( child );
    }
}

void wxMacControl::InvalidateWithChildren()
{
    InvalidateControlAndChildren( m_controlRef );
}

void wxMacControl::ScrollRect( wxRect *r , int dx , int dy )
{
    wxASSERT( r != NULL );

    HIRect scrollarea = CGRectMake( r->x , r->y , r->width , r->height);
    HIViewScrollRect ( m_controlRef , &scrollarea , dx ,dy );
}

OSType wxMacCreator = 'WXMC';
OSType wxMacControlProperty = 'MCCT';

void wxMacControl::SetReferenceInNativeControl()
{
    void * data = this;
    verify_noerr( SetControlProperty ( m_controlRef ,
        wxMacCreator,wxMacControlProperty, sizeof(data), &data ) );
}

wxMacControl* wxMacControl::GetReferenceFromNativeControl(ControlRef control)
{
    wxMacControl* ctl = NULL;
    ByteCount actualSize;
    if ( GetControlProperty( control ,wxMacCreator,wxMacControlProperty, sizeof(ctl) ,
        &actualSize , &ctl ) == noErr )
    {
        return ctl;
    }
    return NULL;
}

// ============================================================================
// DataBrowser Wrapper
// ============================================================================
//
// basing on DataBrowserItemIDs
//

IMPLEMENT_ABSTRACT_CLASS( wxMacDataBrowserControl , wxMacControl )

pascal void wxMacDataBrowserControl::DataBrowserItemNotificationProc(
    ControlRef browser,
    DataBrowserItemID itemID,
    DataBrowserItemNotification message,
    DataBrowserItemDataRef itemData )
{
    wxMacDataBrowserControl* ctl = wxDynamicCast(wxMacControl::GetReferenceFromNativeControl( browser ), wxMacDataBrowserControl);
    if ( ctl != 0 )
    {
        ctl->ItemNotification(itemID, message, itemData);
    }
}

pascal OSStatus wxMacDataBrowserControl::DataBrowserGetSetItemDataProc(
    ControlRef browser,
    DataBrowserItemID itemID,
    DataBrowserPropertyID property,
    DataBrowserItemDataRef itemData,
    Boolean changeValue )
{
    OSStatus err = errDataBrowserPropertyNotSupported;
    wxMacDataBrowserControl* ctl = wxDynamicCast(wxMacControl::GetReferenceFromNativeControl( browser ), wxMacDataBrowserControl);
    if ( ctl != 0 )
    {
        err = ctl->GetSetItemData(itemID, property, itemData, changeValue);
    }
    return err;
}

pascal Boolean wxMacDataBrowserControl::DataBrowserCompareProc(
    ControlRef browser,
    DataBrowserItemID itemOneID,
    DataBrowserItemID itemTwoID,
    DataBrowserPropertyID sortProperty)
{
    wxMacDataBrowserControl* ctl = wxDynamicCast(wxMacControl::GetReferenceFromNativeControl( browser ), wxMacDataBrowserControl);
    if ( ctl != 0 )
    {
        return ctl->CompareItems(itemOneID, itemTwoID, sortProperty);
    }
    return false;
}

DataBrowserItemDataUPP gDataBrowserItemDataUPP = NULL;
DataBrowserItemNotificationUPP gDataBrowserItemNotificationUPP = NULL;
DataBrowserItemCompareUPP gDataBrowserItemCompareUPP = NULL;

wxMacDataBrowserControl::wxMacDataBrowserControl( wxWindow* peer,
                                                  const wxPoint& pos,
                                                  const wxSize& size,
                                                  long WXUNUSED(style))
                       : wxMacControl( peer )
{
    Rect bounds = wxMacGetBoundsForControl( peer, pos, size );
    OSStatus err = ::CreateDataBrowserControl(
        MAC_WXHWND(peer->MacGetTopLevelWindowRef()),
        &bounds, kDataBrowserListView, &m_controlRef );
    SetReferenceInNativeControl();
    verify_noerr( err );
    if ( gDataBrowserItemCompareUPP == NULL )
        gDataBrowserItemCompareUPP = NewDataBrowserItemCompareUPP(DataBrowserCompareProc);
    if ( gDataBrowserItemDataUPP == NULL )
        gDataBrowserItemDataUPP = NewDataBrowserItemDataUPP(DataBrowserGetSetItemDataProc);
    if ( gDataBrowserItemNotificationUPP == NULL )
    {
        gDataBrowserItemNotificationUPP =
            (DataBrowserItemNotificationUPP) NewDataBrowserItemNotificationWithItemUPP(DataBrowserItemNotificationProc);
    }

    DataBrowserCallbacks callbacks;
    InitializeDataBrowserCallbacks( &callbacks, kDataBrowserLatestCallbacks );

    callbacks.u.v1.itemDataCallback = gDataBrowserItemDataUPP;
    callbacks.u.v1.itemCompareCallback = gDataBrowserItemCompareUPP;
    callbacks.u.v1.itemNotificationCallback = gDataBrowserItemNotificationUPP;
    SetCallbacks( &callbacks );

}

OSStatus wxMacDataBrowserControl::GetItemCount( DataBrowserItemID container,
    Boolean recurse,
    DataBrowserItemState state,
    ItemCount *numItems) const
{
    return GetDataBrowserItemCount( m_controlRef, container, recurse, state, numItems );
}

OSStatus wxMacDataBrowserControl::GetItems( DataBrowserItemID container,
    Boolean recurse,
    DataBrowserItemState state,
    Handle items) const
{
    return GetDataBrowserItems( m_controlRef, container, recurse, state, items );
}

OSStatus wxMacDataBrowserControl::SetSelectionFlags( DataBrowserSelectionFlags options )
{
    return SetDataBrowserSelectionFlags( m_controlRef, options );
}

OSStatus wxMacDataBrowserControl::AddColumn( DataBrowserListViewColumnDesc *columnDesc,
        DataBrowserTableViewColumnIndex position )
{
    return AddDataBrowserListViewColumn( m_controlRef, columnDesc, position );
}

OSStatus wxMacDataBrowserControl::GetColumnIDFromIndex( DataBrowserTableViewColumnIndex position, DataBrowserTableViewColumnID* id ){
    return GetDataBrowserTableViewColumnProperty( m_controlRef, position, id );
}

OSStatus wxMacDataBrowserControl::RemoveColumn( DataBrowserTableViewColumnIndex position )
{
    DataBrowserTableViewColumnID id;
    GetColumnIDFromIndex( position, &id );
    return RemoveDataBrowserTableViewColumn( m_controlRef, id );
}

OSStatus wxMacDataBrowserControl::AutoSizeColumns()
{
    return AutoSizeDataBrowserListViewColumns(m_controlRef);
}

OSStatus wxMacDataBrowserControl::SetHasScrollBars( bool horiz, bool vert )
{
    return SetDataBrowserHasScrollBars( m_controlRef, horiz, vert );
}

OSStatus wxMacDataBrowserControl::SetHiliteStyle( DataBrowserTableViewHiliteStyle hiliteStyle )
{
    return SetDataBrowserTableViewHiliteStyle( m_controlRef, hiliteStyle );
}

OSStatus wxMacDataBrowserControl::SetHeaderButtonHeight(UInt16 height)
{
    return SetDataBrowserListViewHeaderBtnHeight( m_controlRef, height );
}

OSStatus wxMacDataBrowserControl::GetHeaderButtonHeight(UInt16 *height)
{
    return GetDataBrowserListViewHeaderBtnHeight( m_controlRef, height );
}

OSStatus wxMacDataBrowserControl::SetCallbacks(const DataBrowserCallbacks *callbacks)
{
    return SetDataBrowserCallbacks( m_controlRef, callbacks );
}

OSStatus wxMacDataBrowserControl::UpdateItems(
    DataBrowserItemID container,
    UInt32 numItems,
    const DataBrowserItemID *items,
    DataBrowserPropertyID preSortProperty,
    DataBrowserPropertyID propertyID ) const
{
    return UpdateDataBrowserItems( m_controlRef, container, numItems, items, preSortProperty, propertyID );
}

bool wxMacDataBrowserControl::IsItemSelected( DataBrowserItemID item ) const
{
    return IsDataBrowserItemSelected( m_controlRef, item );
}

OSStatus wxMacDataBrowserControl::AddItems(
    DataBrowserItemID container,
    UInt32 numItems,
    const DataBrowserItemID *items,
    DataBrowserPropertyID preSortProperty )
{
    return AddDataBrowserItems( m_controlRef, container, numItems, items, preSortProperty );
}

OSStatus wxMacDataBrowserControl::RemoveItems(
    DataBrowserItemID container,
    UInt32 numItems,
    const DataBrowserItemID *items,
    DataBrowserPropertyID preSortProperty )
{
    return RemoveDataBrowserItems( m_controlRef, container, numItems, items, preSortProperty );
}

OSStatus wxMacDataBrowserControl::RevealItem(
    DataBrowserItemID item,
    DataBrowserPropertyID propertyID,
    DataBrowserRevealOptions options ) const
{
    return RevealDataBrowserItem( m_controlRef, item, propertyID, options );
}

OSStatus wxMacDataBrowserControl::SetSelectedItems(
    UInt32 numItems,
    const DataBrowserItemID *items,
    DataBrowserSetOption operation )
{
    return SetDataBrowserSelectedItems( m_controlRef, numItems, items, operation );
}

OSStatus wxMacDataBrowserControl::GetSelectionAnchor( DataBrowserItemID *first, DataBrowserItemID *last ) const
{
    return GetDataBrowserSelectionAnchor( m_controlRef, first, last );
}

OSStatus wxMacDataBrowserControl::GetItemID( DataBrowserTableViewRowIndex row, DataBrowserItemID * item ) const
{
    return GetDataBrowserTableViewItemID( m_controlRef, row, item );
}

OSStatus wxMacDataBrowserControl::GetItemRow( DataBrowserItemID item, DataBrowserTableViewRowIndex * row ) const
{
    return GetDataBrowserTableViewItemRow( m_controlRef, item, row );
}

OSStatus wxMacDataBrowserControl::SetDefaultRowHeight( UInt16 height )
{
    return SetDataBrowserTableViewRowHeight( m_controlRef , height );
}

OSStatus wxMacDataBrowserControl::GetDefaultRowHeight( UInt16 * height ) const
{
    return GetDataBrowserTableViewRowHeight( m_controlRef, height );
}

OSStatus wxMacDataBrowserControl::SetRowHeight( DataBrowserItemID item , UInt16 height)
{
    return SetDataBrowserTableViewItemRowHeight( m_controlRef, item , height );
}

OSStatus wxMacDataBrowserControl::GetRowHeight( DataBrowserItemID item , UInt16 *height) const
{
    return GetDataBrowserTableViewItemRowHeight( m_controlRef, item , height);
}

OSStatus wxMacDataBrowserControl::GetColumnWidth( DataBrowserPropertyID column , UInt16 *width ) const
{
    return GetDataBrowserTableViewNamedColumnWidth( m_controlRef , column , width );
}

OSStatus wxMacDataBrowserControl::SetColumnWidth( DataBrowserPropertyID column , UInt16 width )
{
    return SetDataBrowserTableViewNamedColumnWidth( m_controlRef , column , width );
}

OSStatus wxMacDataBrowserControl::GetDefaultColumnWidth( UInt16 *width ) const
{
    return GetDataBrowserTableViewColumnWidth( m_controlRef , width );
}

OSStatus wxMacDataBrowserControl::SetDefaultColumnWidth( UInt16 width )
{
    return SetDataBrowserTableViewColumnWidth( m_controlRef , width );
}

OSStatus wxMacDataBrowserControl::GetColumnCount(UInt32* numColumns) const
{
    return GetDataBrowserTableViewColumnCount( m_controlRef, numColumns);
}

OSStatus wxMacDataBrowserControl::GetColumnPosition( DataBrowserPropertyID column,
    DataBrowserTableViewColumnIndex *position) const
{
    return GetDataBrowserTableViewColumnPosition( m_controlRef , column , position);
}

OSStatus wxMacDataBrowserControl::SetColumnPosition( DataBrowserPropertyID column, DataBrowserTableViewColumnIndex position)
{
    return SetDataBrowserTableViewColumnPosition( m_controlRef , column , position);
}

OSStatus wxMacDataBrowserControl::GetScrollPosition( UInt32 *top , UInt32 *left ) const
{
    return GetDataBrowserScrollPosition( m_controlRef , top , left );
}

OSStatus wxMacDataBrowserControl::SetScrollPosition( UInt32 top , UInt32 left )
{
    return SetDataBrowserScrollPosition( m_controlRef , top , left );
}

OSStatus wxMacDataBrowserControl::GetSortProperty( DataBrowserPropertyID *column ) const
{
    return GetDataBrowserSortProperty( m_controlRef , column );
}

OSStatus wxMacDataBrowserControl::SetSortProperty( DataBrowserPropertyID column )
{
    return SetDataBrowserSortProperty( m_controlRef , column );
}

OSStatus wxMacDataBrowserControl::GetSortOrder( DataBrowserSortOrder *order ) const
{
    return GetDataBrowserSortOrder( m_controlRef , order );
}

OSStatus wxMacDataBrowserControl::SetSortOrder( DataBrowserSortOrder order )
{
    return SetDataBrowserSortOrder( m_controlRef , order );
}

OSStatus wxMacDataBrowserControl::GetPropertyFlags( DataBrowserPropertyID property,
    DataBrowserPropertyFlags *flags ) const
{
    return GetDataBrowserPropertyFlags( m_controlRef , property , flags );
}

OSStatus wxMacDataBrowserControl::SetPropertyFlags( DataBrowserPropertyID property,
    DataBrowserPropertyFlags flags )
{
    return SetDataBrowserPropertyFlags( m_controlRef , property , flags );
}

OSStatus wxMacDataBrowserControl::GetHeaderDesc( DataBrowserPropertyID property,
    DataBrowserListViewHeaderDesc *desc ) const
{
    return GetDataBrowserListViewHeaderDesc( m_controlRef , property , desc );
}

OSStatus wxMacDataBrowserControl::SetHeaderDesc( DataBrowserPropertyID property,
    DataBrowserListViewHeaderDesc *desc )
{
    return SetDataBrowserListViewHeaderDesc( m_controlRef , property , desc );
}

OSStatus wxMacDataBrowserControl::SetDisclosureColumn( DataBrowserPropertyID property ,
    Boolean expandableRows )
{
    return SetDataBrowserListViewDisclosureColumn( m_controlRef, property, expandableRows);
}

// ============================================================================
// Higher-level Databrowser
// ============================================================================
//
// basing on data item objects
//

wxMacDataItem::wxMacDataItem()
{
    m_data = NULL;

    m_order = 0;
    m_colId = kTextColumnId; // for compat with existing wx*ListBox impls.
}

wxMacDataItem::~wxMacDataItem()
{
}

void wxMacDataItem::SetOrder( SInt32 order )
{
    m_order = order;
}

SInt32 wxMacDataItem::GetOrder() const
{
    return m_order;
}

void wxMacDataItem::SetData( void* data)
{
    m_data = data;
}

void* wxMacDataItem::GetData() const
{
    return m_data;
}

short wxMacDataItem::GetColumn()
{
    return m_colId;
}

void wxMacDataItem::SetColumn( short col )
{
    m_colId = col;
}

void wxMacDataItem::SetLabel( const wxString& str)
{
    m_label = str;
    m_cfLabel = wxCFStringRef( str , wxLocale::GetSystemEncoding());
}

const wxString& wxMacDataItem::GetLabel() const
{
    return m_label;
}

bool wxMacDataItem::IsLessThan(wxMacDataItemBrowserControl *WXUNUSED(owner) ,
    const wxMacDataItem* rhs,
    DataBrowserPropertyID sortProperty) const
{
    const wxMacDataItem* otherItem = wx_const_cast(wxMacDataItem*,rhs);
    bool retval = false;

    if ( sortProperty == m_colId ){
        retval = m_label.CmpNoCase( otherItem->m_label) < 0;
    }

    else if ( sortProperty == kNumericOrderColumnId )
        retval = m_order < otherItem->m_order;

    return retval;
}

OSStatus wxMacDataItem::GetSetData( wxMacDataItemBrowserControl *WXUNUSED(owner) ,
    DataBrowserPropertyID property,
    DataBrowserItemDataRef itemData,
    bool changeValue )
{
    OSStatus err = errDataBrowserPropertyNotSupported;
    if ( !changeValue )
    {
        if ( property == m_colId ){
            err = ::SetDataBrowserItemDataText( itemData, m_cfLabel );
            err = noErr;
        }
        else if ( property == kNumericOrderColumnId ){
            err = ::SetDataBrowserItemDataValue( itemData, m_order );
            err = noErr;
        }
        else{
        }
    }
    else
    {
        switch (property)
        {
            // no editable props here
            default:
                break;
        }
    }

    return err;
}

void wxMacDataItem::Notification(wxMacDataItemBrowserControl *WXUNUSED(owner) ,
    DataBrowserItemNotification WXUNUSED(message),
    DataBrowserItemDataRef WXUNUSED(itemData) ) const
{
}

IMPLEMENT_DYNAMIC_CLASS( wxMacDataItemBrowserControl , wxMacDataBrowserControl )

wxMacDataItemBrowserControl::wxMacDataItemBrowserControl( wxWindow* peer , const wxPoint& pos, const wxSize& size, long style) :
    wxMacDataBrowserControl( peer, pos, size, style )
{
    m_suppressSelection = false;
    m_sortOrder = SortOrder_None;
    m_clientDataItemsType = wxClientData_None;
}

wxMacDataItem* wxMacDataItemBrowserControl::CreateItem()
{
    return new wxMacDataItem();
}

wxMacDataItemBrowserSelectionSuppressor::wxMacDataItemBrowserSelectionSuppressor(wxMacDataItemBrowserControl *browser)
{
    m_former = browser->SuppressSelection(true);
    m_browser = browser;
}

wxMacDataItemBrowserSelectionSuppressor::~wxMacDataItemBrowserSelectionSuppressor()
{
    m_browser->SuppressSelection(m_former);
}

bool  wxMacDataItemBrowserControl::SuppressSelection( bool suppress )
{
    bool former = m_suppressSelection;
    m_suppressSelection = suppress;

    return former;
}

Boolean wxMacDataItemBrowserControl::CompareItems(DataBrowserItemID itemOneID,
    DataBrowserItemID itemTwoID,
    DataBrowserPropertyID sortProperty)
{
    wxMacDataItem* itemOne = (wxMacDataItem*) itemOneID;
    wxMacDataItem* itemTwo = (wxMacDataItem*) itemTwoID;
    return CompareItems( itemOne , itemTwo , sortProperty );
}

Boolean wxMacDataItemBrowserControl::CompareItems(const wxMacDataItem*  itemOne,
    const wxMacDataItem*  itemTwo,
    DataBrowserPropertyID sortProperty)
{
    Boolean retval = false;
    if ( itemOne != NULL )
        retval = itemOne->IsLessThan( this , itemTwo , sortProperty);
    return retval;
}

OSStatus wxMacDataItemBrowserControl::GetSetItemData(
    DataBrowserItemID itemID,
    DataBrowserPropertyID property,
    DataBrowserItemDataRef itemData,
    Boolean changeValue )
{
    wxMacDataItem* item = (wxMacDataItem*) itemID;
    return GetSetItemData(item, property, itemData , changeValue );
}

OSStatus wxMacDataItemBrowserControl::GetSetItemData(
    wxMacDataItem* item,
    DataBrowserPropertyID property,
    DataBrowserItemDataRef itemData,
    Boolean changeValue )
{
    OSStatus err = errDataBrowserPropertyNotSupported;
    switch( property )
    {
        case kDataBrowserContainerIsClosableProperty :
        case kDataBrowserContainerIsSortableProperty :
        case kDataBrowserContainerIsOpenableProperty :
            // right now default behaviour on these
            break;
        default :

            if ( item != NULL ){
                err = item->GetSetData( this, property , itemData , changeValue );
            }
            break;

    }
    return err;
}

void wxMacDataItemBrowserControl::ItemNotification(
    DataBrowserItemID itemID,
    DataBrowserItemNotification message,
    DataBrowserItemDataRef itemData)
{
    wxMacDataItem* item = (wxMacDataItem*) itemID;
    ItemNotification( item , message, itemData);
}

void wxMacDataItemBrowserControl::ItemNotification(
    const wxMacDataItem* item,
    DataBrowserItemNotification message,
    DataBrowserItemDataRef itemData)
{
    if (item != NULL)
        item->Notification( this, message, itemData);
}

unsigned int wxMacDataItemBrowserControl::GetItemCount(const wxMacDataItem* container,
        bool recurse , DataBrowserItemState state) const
{
    ItemCount numItems = 0;
    verify_noerr( wxMacDataBrowserControl::GetItemCount( (DataBrowserItemID)container,
        recurse, state, &numItems ) );
    return numItems;
}

unsigned int wxMacDataItemBrowserControl::GetSelectedItemCount( const wxMacDataItem* container,
        bool recurse ) const
{
    return GetItemCount( container, recurse, kDataBrowserItemIsSelected );

}

void wxMacDataItemBrowserControl::GetItems(const wxMacDataItem* container,
    bool recurse , DataBrowserItemState state, wxArrayMacDataItemPtr &items) const
{
    Handle handle = NewHandle(0);
    verify_noerr( wxMacDataBrowserControl::GetItems( (DataBrowserItemID)container ,
        recurse , state, handle) );

    int itemCount = GetHandleSize(handle)/sizeof(DataBrowserItemID);
    HLock( handle );
    wxMacDataItemPtr* itemsArray = (wxMacDataItemPtr*) *handle;
    for ( int i = 0; i < itemCount; ++i)
    {
        items.Add(itemsArray[i]);
    }
    HUnlock( handle );
    DisposeHandle( handle );
}

unsigned int wxMacDataItemBrowserControl::GetLineFromItem(const wxMacDataItem* item) const
{
    DataBrowserTableViewRowIndex row;
    OSStatus err = GetItemRow( (DataBrowserItemID) item , &row);
    wxCHECK( err == noErr, (unsigned)-1 );
    return row;
}

wxMacDataItem*  wxMacDataItemBrowserControl::GetItemFromLine(unsigned int n) const
{
    DataBrowserItemID id;
    OSStatus err =  GetItemID( (DataBrowserTableViewRowIndex) n , &id);
    wxCHECK( err == noErr, NULL );
    return (wxMacDataItem*) id;
}

void wxMacDataItemBrowserControl::UpdateItem(const wxMacDataItem *container,
        const wxMacDataItem *item , DataBrowserPropertyID property) const
{
    verify_noerr( wxMacDataBrowserControl::UpdateItems((DataBrowserItemID)container, 1,
        (DataBrowserItemID*) &item, kDataBrowserItemNoProperty /* notSorted */, property ) );
}

void wxMacDataItemBrowserControl::UpdateItems(const wxMacDataItem *container,
        wxArrayMacDataItemPtr &itemArray , DataBrowserPropertyID property) const
{
    unsigned int noItems = itemArray.GetCount();
    DataBrowserItemID *items = new DataBrowserItemID[noItems];
    for ( unsigned int i = 0; i < noItems; ++i )
        items[i] = (DataBrowserItemID) itemArray[i];

    verify_noerr( wxMacDataBrowserControl::UpdateItems((DataBrowserItemID)container, noItems,
        items, kDataBrowserItemNoProperty /* notSorted */, property ) );
    delete [] items;
}

void wxMacDataItemBrowserControl::InsertColumn(int colId, DataBrowserPropertyType colType,
                                            const wxString& title, SInt16 just, int defaultWidth)
{
    DataBrowserListViewColumnDesc columnDesc;
    columnDesc.headerBtnDesc.titleOffset = 0;
    columnDesc.headerBtnDesc.version = kDataBrowserListViewLatestHeaderDesc;

    columnDesc.headerBtnDesc.btnFontStyle.flags =
        kControlUseFontMask | kControlUseJustMask;

    columnDesc.headerBtnDesc.btnContentInfo.contentType = kControlContentTextOnly;
    columnDesc.headerBtnDesc.btnFontStyle.just = just;
    columnDesc.headerBtnDesc.btnFontStyle.font = kControlFontViewSystemFont;
    columnDesc.headerBtnDesc.btnFontStyle.style = normal;

    // TODO: Why is m_font not defined when we enter wxLC_LIST mode, but is
    // defined for other modes?
    wxFontEncoding enc;
    if ( m_font.Ok() )
        enc = m_font.GetEncoding();
    else
        enc = wxLocale::GetSystemEncoding();
    wxCFStringRef cfTitle( title, enc );
    columnDesc.headerBtnDesc.titleString = cfTitle;

    columnDesc.headerBtnDesc.minimumWidth = 0;
    columnDesc.headerBtnDesc.maximumWidth = 30000;

    columnDesc.propertyDesc.propertyID = (kMinColumnId + colId);
    columnDesc.propertyDesc.propertyType = colType;
    columnDesc.propertyDesc.propertyFlags = kDataBrowserListViewSortableColumn;
    columnDesc.propertyDesc.propertyFlags |= kDataBrowserListViewTypeSelectColumn;
    columnDesc.propertyDesc.propertyFlags |= kDataBrowserListViewNoGapForIconInHeaderButton;

    verify_noerr( AddColumn( &columnDesc, kDataBrowserListViewAppendColumn ) );

    if (defaultWidth > 0){
        SetColumnWidth(colId, defaultWidth);
    }

}

void wxMacDataItemBrowserControl::SetColumnWidth(int colId, int width)
{
    DataBrowserPropertyID id;
    GetColumnIDFromIndex(colId, &id);
    verify_noerr( wxMacDataBrowserControl::SetColumnWidth(id, width));
}

int wxMacDataItemBrowserControl::GetColumnWidth(int colId)
{
    DataBrowserPropertyID id;
    GetColumnIDFromIndex(colId, &id);
    UInt16 result;
    verify_noerr( wxMacDataBrowserControl::GetColumnWidth(id, &result));
    return result;
}

void wxMacDataItemBrowserControl::AddItem(wxMacDataItem *container, wxMacDataItem *item)
{
    verify_noerr( wxMacDataBrowserControl::AddItems( (DataBrowserItemID)container, 1,
        (DataBrowserItemID*) &item, kDataBrowserItemNoProperty ) );
}

void wxMacDataItemBrowserControl::AddItems(wxMacDataItem *container, wxArrayMacDataItemPtr &itemArray )
{
    unsigned int noItems = itemArray.GetCount();
    DataBrowserItemID *items = new DataBrowserItemID[noItems];
    for ( unsigned int i = 0; i < noItems; ++i )
        items[i] = (DataBrowserItemID) itemArray[i];

    verify_noerr( wxMacDataBrowserControl::AddItems( (DataBrowserItemID)container, noItems,
        (DataBrowserItemID*) items, kDataBrowserItemNoProperty ) );
    delete [] items;
}

void wxMacDataItemBrowserControl::RemoveItem(wxMacDataItem *container, wxMacDataItem* item)
{
    OSStatus err = wxMacDataBrowserControl::RemoveItems( (DataBrowserItemID)container, 1,
        (DataBrowserItemID*) &item, kDataBrowserItemNoProperty );
    verify_noerr( err );
}

void wxMacDataItemBrowserControl::RemoveItems(wxMacDataItem *container, wxArrayMacDataItemPtr &itemArray)
{
    unsigned int noItems = itemArray.GetCount();
    DataBrowserItemID *items = new DataBrowserItemID[noItems];
    for ( unsigned int i = 0; i < noItems; ++i )
        items[i] = (DataBrowserItemID) itemArray[i];

    OSStatus err = wxMacDataBrowserControl::RemoveItems( (DataBrowserItemID)container, noItems,
        (DataBrowserItemID*) items, kDataBrowserItemNoProperty );
    verify_noerr( err );
    delete [] items;
}

void wxMacDataItemBrowserControl::RemoveAllItems(wxMacDataItem *container)
{
    OSStatus err = wxMacDataBrowserControl::RemoveItems( (DataBrowserItemID)container, 0 , NULL , kDataBrowserItemNoProperty );
    verify_noerr( err );
}

void wxMacDataItemBrowserControl::SetSelectedItem(wxMacDataItem* item , DataBrowserSetOption option)
{
    verify_noerr(wxMacDataBrowserControl::SetSelectedItems( 1, (DataBrowserItemID*) &item, option ));
}

void wxMacDataItemBrowserControl::SetSelectedAllItems(DataBrowserSetOption option)
{
    verify_noerr(wxMacDataBrowserControl::SetSelectedItems( 0 , NULL , option ));
}

void wxMacDataItemBrowserControl::SetSelectedItems(wxArrayMacDataItemPtr &itemArray , DataBrowserSetOption option)
{
    unsigned int noItems = itemArray.GetCount();
    DataBrowserItemID *items = new DataBrowserItemID[noItems];
    for ( unsigned int i = 0; i < noItems; ++i )
        items[i] = (DataBrowserItemID) itemArray[i];

    verify_noerr(wxMacDataBrowserControl::SetSelectedItems( noItems, (DataBrowserItemID*) items, option ));
    delete [] items;
}

Boolean wxMacDataItemBrowserControl::IsItemSelected( const wxMacDataItem* item) const
{
    return wxMacDataBrowserControl::IsItemSelected( (DataBrowserItemID) item);
}

void wxMacDataItemBrowserControl::RevealItem( wxMacDataItem* item, DataBrowserRevealOptions options)
{
    verify_noerr(wxMacDataBrowserControl::RevealItem( (DataBrowserItemID) item, kDataBrowserNoItem , options ) );
}

void wxMacDataItemBrowserControl::GetSelectionAnchor( wxMacDataItemPtr* first , wxMacDataItemPtr* last) const
{
    verify_noerr(wxMacDataBrowserControl::GetSelectionAnchor( (DataBrowserItemID*) first, (DataBrowserItemID*) last) );
}

wxClientDataType wxMacDataItemBrowserControl::GetClientDataType() const
{
     return m_clientDataItemsType;
}
void wxMacDataItemBrowserControl::SetClientDataType(wxClientDataType clientDataItemsType)
{
    m_clientDataItemsType = clientDataItemsType;
}

unsigned int wxMacDataItemBrowserControl::MacGetCount() const
{
    return GetItemCount(wxMacDataBrowserRootContainer,false,kDataBrowserItemAnyState);
}

void wxMacDataItemBrowserControl::MacDelete( unsigned int n )
{
    wxMacDataItem* item = (wxMacDataItem*)GetItemFromLine( n );
    RemoveItem( wxMacDataBrowserRootContainer, item );
}

void wxMacDataItemBrowserControl::MacInsert( unsigned int n,
                                             const wxArrayStringsAdapter& items,
                                             int column )
{
    size_t itemsCount = items.GetCount();
    if ( itemsCount == 0 )
        return;

    SInt32 frontLineOrder = 0;

    if ( m_sortOrder == SortOrder_None )
    {
        // increase the order of the lines to be shifted
        unsigned int lines = MacGetCount();
        for ( unsigned int i = n; i < lines; ++i)
        {
            wxMacDataItem* iter = (wxMacDataItem*) GetItemFromLine(i);
            iter->SetOrder( iter->GetOrder() + itemsCount );
        }
        if ( n > 0 )
        {
            wxMacDataItem* iter = (wxMacDataItem*) GetItemFromLine(n-1);
            frontLineOrder = iter->GetOrder();
        }
    }

    wxArrayMacDataItemPtr ids;
    ids.SetCount( itemsCount );

    for ( unsigned int i = 0; i < itemsCount; ++i )
    {
        wxMacDataItem* item = CreateItem();
        item->SetLabel( items[i]);
        if ( column != -1 )
            item->SetColumn( kMinColumnId + column );

        if ( m_sortOrder == SortOrder_None )
            item->SetOrder( frontLineOrder + 1 + i );

        ids[i] = item;
    }

    AddItems( wxMacDataBrowserRootContainer, ids );
}

int wxMacDataItemBrowserControl::MacAppend( const wxString& text)
{
    wxMacDataItem* item = CreateItem();
    item->SetLabel( text );
    if ( m_sortOrder == SortOrder_None )
    {
        unsigned int lines = MacGetCount();
        if ( lines == 0 )
            item->SetOrder( 1 );
        else
        {
            wxMacDataItem* frontItem = (wxMacDataItem*) GetItemFromLine(lines-1);
            item->SetOrder( frontItem->GetOrder() + 1 );
        }
    }
    AddItem( wxMacDataBrowserRootContainer, item );

    return GetLineFromItem(item);
}

void wxMacDataItemBrowserControl::MacClear()
{
    wxMacDataItemBrowserSelectionSuppressor suppressor(this);
    RemoveAllItems(wxMacDataBrowserRootContainer);
}

void wxMacDataItemBrowserControl::MacDeselectAll()
{
    wxMacDataItemBrowserSelectionSuppressor suppressor(this);
    SetSelectedAllItems( kDataBrowserItemsRemove );
}

void wxMacDataItemBrowserControl::MacSetSelection( unsigned int n, bool select, bool multi )
{
    wxMacDataItem* item = (wxMacDataItem*) GetItemFromLine(n);
    wxMacDataItemBrowserSelectionSuppressor suppressor(this);

    if ( IsItemSelected( item ) != select )
    {
        if ( select )
            SetSelectedItem( item, multi ? kDataBrowserItemsAdd : kDataBrowserItemsAssign );
        else
            SetSelectedItem( item, kDataBrowserItemsRemove );
    }

    MacScrollTo( n );
}

bool wxMacDataItemBrowserControl::MacIsSelected( unsigned int n ) const
{
    wxMacDataItem* item = (wxMacDataItem*) GetItemFromLine(n);
    return IsItemSelected( item );
}

int wxMacDataItemBrowserControl::MacGetSelection() const
{
    wxMacDataItemPtr first, last;
    GetSelectionAnchor( &first, &last );

    if ( first != NULL )
    {
        return GetLineFromItem( first );
    }

    return -1;
}

int wxMacDataItemBrowserControl::MacGetSelections( wxArrayInt& aSelections ) const
{
    aSelections.Empty();
    wxArrayMacDataItemPtr selectedItems;
    GetItems( wxMacDataBrowserRootContainer, false , kDataBrowserItemIsSelected, selectedItems);

    int count = selectedItems.GetCount();

    for ( int i = 0; i < count; ++i)
    {
        aSelections.Add(GetLineFromItem(selectedItems[i]));
    }

    return count;
}

void wxMacDataItemBrowserControl::MacSetString( unsigned int n, const wxString& text )
{
    // as we don't store the strings we only have to issue a redraw
    wxMacDataItem* item = (wxMacDataItem*) GetItemFromLine( n);
    item->SetLabel( text );
    UpdateItem( wxMacDataBrowserRootContainer, item , kTextColumnId );
}

wxString wxMacDataItemBrowserControl::MacGetString( unsigned int n ) const
{
    wxMacDataItem * item = (wxMacDataItem*) GetItemFromLine( n );
    return item->GetLabel();
}

void wxMacDataItemBrowserControl::MacSetClientData( unsigned int n, void * data)
{
    wxMacDataItem* item = (wxMacDataItem*) GetItemFromLine( n);
    item->SetData( data );
    // not displayed, therefore no Update infos to DataBrowser
}

void * wxMacDataItemBrowserControl::MacGetClientData( unsigned int n) const
{
    wxMacDataItem * item = (wxMacDataItem*) GetItemFromLine( n );
    return item->GetData();
}

void wxMacDataItemBrowserControl::MacScrollTo( unsigned int n )
{
    UInt32 top , left ;
    GetScrollPosition( &top , &left ) ;
    wxMacDataItem * item = (wxMacDataItem*) GetItemFromLine( n );

    // there is a bug in RevealItem that leads to situations
    // in large lists, where the item does not get scrolled
    // into sight, so we do a pre-scroll if necessary
    UInt16 height ;
    GetRowHeight( (DataBrowserItemID) item , &height ) ;
    UInt32 linetop = n * ((UInt32) height );
    UInt32 linebottom = linetop + height;
    Rect rect ;
    GetRect( &rect );

    if ( linetop < top || linebottom > (top + rect.bottom - rect.top ) )
        SetScrollPosition( wxMax( n-2, 0 ) * ((UInt32)height) , left ) ;

    RevealItem( item , kDataBrowserRevealWithoutSelecting );
}



//
// Tab Control
//

OSStatus wxMacControl::SetTabEnabled( SInt16 tabNo , bool enable )
{
    return ::SetTabEnabled( m_controlRef , tabNo , enable );
}

#endif

//
// Quartz Support
//

/*
    Return the generic RGB color space. This is a 'get' function and the caller should
    not release the returned value unless the caller retains it first. Usually callers
    of this routine will immediately use the returned colorspace with CoreGraphics
    so they typically do not need to retain it themselves.

    This function creates the generic RGB color space once and hangs onto it so it can
    return it whenever this function is called.
*/

CGColorSpaceRef wxMacGetGenericRGBColorSpace()
{
    static wxCFRef<CGColorSpaceRef> genericRGBColorSpace;

    if (genericRGBColorSpace == NULL)
    {
        genericRGBColorSpace.reset( CGColorSpaceCreateWithName( kCGColorSpaceGenericRGB ) );
    }

    return genericRGBColorSpace;
}

CGColorRef wxMacCreateCGColorFromHITheme( ThemeBrush brush )
{
    CGColorRef color ;
    HIThemeBrushCreateCGColor( brush, &color );
    return color;
}

#if wxMAC_USE_QUICKDRAW

static inline void PointFromHIPoint(const HIPoint& p, Point *pt)
{
    pt->h = wx_static_cast(short, p.x);
    pt->v = wx_static_cast(short, p.y);
}

void wxMacGlobalToLocal( WindowRef window , Point*pt )
{
    HIPoint p = CGPointMake( pt->h, pt->v );
    HIViewRef contentView ;
    // TODO check toolbar offset
    HIViewFindByID( HIViewGetRoot( window ), kHIViewWindowContentID , &contentView) ;
    HIPointConvert( &p, kHICoordSpace72DPIGlobal, NULL, kHICoordSpaceView, contentView );
    PointFromHIPoint(p, pt);
}

void wxMacLocalToGlobal( WindowRef window , Point*pt )
{
    HIPoint p = CGPointMake( pt->h, pt->v );
    HIViewRef contentView ;
    // TODO check toolbar offset
    HIViewFindByID( HIViewGetRoot( window ), kHIViewWindowContentID , &contentView) ;
    HIPointConvert( &p, kHICoordSpaceView, contentView, kHICoordSpace72DPIGlobal, NULL );
    PointFromHIPoint(p, pt);
}

#endif // wxMAC_USE_QUICKDRAW

#endif // wxUSE_GUI

#if wxUSE_BASE

#endif
