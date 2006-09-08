/////////////////////////////////////////////////////////////////////////////
// Name:        src/mac/carbon/toolbar.cpp
// Purpose:     wxToolBar
// Author:      Stefan Csomor
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id$
// Copyright:   (c) Stefan Csomor
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#if wxUSE_TOOLBAR

#include "wx/toolbar.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wx/mac/uma.h"
#include "wx/geometry.h"


#ifdef __WXMAC_OSX__
const short kwxMacToolBarToolDefaultWidth = 16;
const short kwxMacToolBarToolDefaultHeight = 16;
const short kwxMacToolBarTopMargin = 4;
const short kwxMacToolBarLeftMargin =  4;
const short kwxMacToolBorder = 0;
const short kwxMacToolSpacing = 6;
#else
const short kwxMacToolBarToolDefaultWidth = 24;
const short kwxMacToolBarToolDefaultHeight = 22;
const short kwxMacToolBarTopMargin = 2;
const short kwxMacToolBarLeftMargin = 2;
const short kwxMacToolBorder = 4;
const short kwxMacToolSpacing = 0;
#endif


IMPLEMENT_DYNAMIC_CLASS(wxToolBar, wxControl)

BEGIN_EVENT_TABLE(wxToolBar, wxToolBarBase)
    EVT_PAINT( wxToolBar::OnPaint )
END_EVENT_TABLE()


#pragma mark -
#pragma mark Tool Implementation


// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

// We have a dual implementation for each tool, ControlRef and HIToolbarItemRef

class wxToolBarTool : public wxToolBarToolBase
{
public:
    wxToolBarTool(
        wxToolBar *tbar,
        int id,
        const wxString& label,
        const wxBitmap& bmpNormal,
        const wxBitmap& bmpDisabled,
        wxItemKind kind,
        wxObject *clientData,
        const wxString& shortHelp,
        const wxString& longHelp );

    wxToolBarTool(wxToolBar *tbar, wxControl *control)
        : wxToolBarToolBase(tbar, control)
    {
        Init();
        if (control != NULL)
            SetControlHandle( (ControlRef) control->GetHandle() );
    }

    virtual ~wxToolBarTool()
    {
        ClearControl();

#if wxMAC_USE_NATIVE_TOOLBAR
        if ( m_toolbarItemRef )
            CFRelease( m_toolbarItemRef );
#endif
    }

    WXWidget GetControlHandle()
    {
        return (WXWidget) m_controlHandle;
    }

    void SetControlHandle( ControlRef handle )
    {
        m_controlHandle = handle;
    }

    void SetPosition( const wxPoint& position );

    void ClearControl()
    {
        m_control = NULL;
        if ( m_controlHandle )
        {
            if ( !IsControl() )
                DisposeControl( m_controlHandle );
            else
            {
                // the embedded control is not under the responsibility of the tool
            }
            m_controlHandle = NULL ;
        }

#if wxMAC_USE_NATIVE_TOOLBAR
        m_toolbarItemRef = NULL;
#endif
    }

    wxSize GetSize() const
    {
        wxSize curSize;

        if ( IsControl() )
        {
            curSize = GetControl()->GetSize();
        }
        else if ( IsButton() )
        {
            curSize = GetToolBar()->GetToolSize();
        }
        else
        {
            // separator size
            curSize = GetToolBar()->GetToolSize();
            if ( GetToolBar()->GetWindowStyleFlag() & wxTB_VERTICAL )
                curSize.y /= 4;
            else
                curSize.x /= 4;
        }

        return curSize;
    }

    wxPoint GetPosition() const
    {
        return wxPoint( m_x, m_y );
    }

    bool DoEnable( bool enable );

    void UpdateToggleImage( bool toggle );

#if wxMAC_USE_NATIVE_TOOLBAR
    void SetToolbarItemRef( HIToolbarItemRef ref )
    {
        if ( m_controlHandle )
            HideControl( m_controlHandle );
        if ( m_toolbarItemRef )
            CFRelease( m_toolbarItemRef );

        m_toolbarItemRef = ref;
        if ( m_toolbarItemRef )
        {
            HIToolbarItemSetHelpText(
                m_toolbarItemRef,
                wxMacCFStringHolder( GetShortHelp(), GetToolBar()->GetFont().GetEncoding() ),
                wxMacCFStringHolder( GetLongHelp(), GetToolBar()->GetFont().GetEncoding() ) );
        }
    }

    HIToolbarItemRef GetToolbarItemRef() const
    {
        return m_toolbarItemRef;
    }

    void SetIndex( CFIndex idx )
    {
        m_index = idx;
    }

    CFIndex GetIndex() const
    {
        return m_index;
    }
#endif

private:
    void Init()
    {
        m_controlHandle = NULL;

#if wxMAC_USE_NATIVE_TOOLBAR
        m_toolbarItemRef = NULL;
        m_index = -1;
#endif
    }

    ControlRef m_controlHandle;
    wxCoord     m_x;
    wxCoord     m_y;

#if wxMAC_USE_NATIVE_TOOLBAR
    HIToolbarItemRef m_toolbarItemRef;
    // position in its toolbar, -1 means not inserted
    CFIndex m_index;
#endif
};

static const EventTypeSpec eventList[] =
{
    { kEventClassControl, kEventControlHit },
#ifdef __WXMAC_OSX__
    { kEventClassControl, kEventControlHitTest },
#endif
};

static pascal OSStatus wxMacToolBarToolControlEventHandler( EventHandlerCallRef handler, EventRef event, void *data )
{
    OSStatus result = eventNotHandledErr;
    ControlRef controlRef;
    wxMacCarbonEvent cEvent( event );

    cEvent.GetParameter( kEventParamDirectObject, &controlRef );

    switch ( GetEventKind( event ) )
    {
        case kEventControlHit:
            {
                wxToolBarTool *tbartool = (wxToolBarTool*)data;
                wxToolBar *tbar = tbartool != NULL ? (wxToolBar*) (tbartool->GetToolBar()) : NULL;
                if ((tbartool != NULL) && tbartool->CanBeToggled())
                {
                    bool    shouldToggle;

#ifdef __WXMAC_OSX__
                    shouldToggle = !tbartool->IsToggled();
#else
                    shouldToggle = (GetControl32BitValue( (ControlRef)(tbartool->GetControlHandle()) ) != 0);
#endif

                    tbar->ToggleTool( tbartool->GetId(), shouldToggle );
                }

                if (tbartool != NULL)
                    tbar->OnLeftClick( tbartool->GetId(), tbartool->IsToggled() );
                result = noErr;
            }
            break;

#ifdef __WXMAC_OSX__
        case kEventControlHitTest:
            {
                HIPoint pt = cEvent.GetParameter<HIPoint>(kEventParamMouseLocation);
                HIRect rect;
                HIViewGetBounds( controlRef, &rect );

                ControlPartCode pc = kControlNoPart;
                if ( CGRectContainsPoint( rect, pt ) )
                    pc = kControlIconPart;
                cEvent.SetParameter( kEventParamControlPart, typeControlPartCode, pc );
                result = noErr;
            }
            break;
#endif

        default:
            break;
    }

    return result;
}

static pascal OSStatus wxMacToolBarToolEventHandler( EventHandlerCallRef handler, EventRef event, void *data )
{
    OSStatus result = eventNotHandledErr;

    switch ( GetEventClass( event ) )
    {
        case kEventClassControl:
            result = wxMacToolBarToolControlEventHandler( handler, event, data );
            break;

        default:
            break;
    }

    return result;
}

DEFINE_ONE_SHOT_HANDLER_GETTER( wxMacToolBarToolEventHandler )

#if wxMAC_USE_NATIVE_TOOLBAR

static const EventTypeSpec toolBarEventList[] =
{
    { kEventClassToolbarItem, kEventToolbarItemPerformAction },
};

static pascal OSStatus wxMacToolBarCommandEventHandler( EventHandlerCallRef handler, EventRef event, void *data )
{
    OSStatus result = eventNotHandledErr;

    switch ( GetEventKind( event ) )
    {
        case kEventToolbarItemPerformAction:
            {
                wxToolBarTool* tbartool = (wxToolBarTool*) data;
                if ( tbartool != NULL )
                {
                    wxToolBar *tbar = (wxToolBar*)(tbartool->GetToolBar());
                    int toolID = tbartool->GetId();

                    if ( tbartool->CanBeToggled() )
                    {
                        if ( tbar != NULL )
                            tbar->ToggleTool(toolID, !tbartool->IsToggled() );
                    }

                    if ( tbar != NULL )
                        tbar->OnLeftClick( toolID, tbartool->IsToggled() );
                    result = noErr;
                }
            }
            break;

        default:
            break;
    }

    return result;
}

static pascal OSStatus wxMacToolBarEventHandler( EventHandlerCallRef handler, EventRef event, void *data )
{
    OSStatus result = eventNotHandledErr;

    switch ( GetEventClass( event ) )
    {
        case kEventClassToolbarItem:
            result = wxMacToolBarCommandEventHandler( handler, event, data );
            break;

        default:
            break;
    }

    return result;
}

DEFINE_ONE_SHOT_HANDLER_GETTER( wxMacToolBarEventHandler )

#endif

bool wxToolBarTool::DoEnable( bool enable )
{
    if ( IsControl() )
    {
        GetControl()->Enable( enable );
    }
    else if ( IsButton() )
    {
#if wxMAC_USE_NATIVE_TOOLBAR
        if ( m_toolbarItemRef != NULL )
            HIToolbarItemSetEnabled( m_toolbarItemRef, enable );
#endif

        if ( m_controlHandle != NULL )
        {
#if TARGET_API_MAC_OSX
            if ( enable )
                EnableControl( m_controlHandle );
            else
                DisableControl( m_controlHandle );
#else
            if ( enable )
                ActivateControl( m_controlHandle );
            else
                DeactivateControl( m_controlHandle );
#endif
        }
    }

    return true;
}

void wxToolBarTool::SetPosition( const wxPoint& position )
{
    m_x = position.x;
    m_y = position.y;

    int x, y;
    x = y = 0;
    int mac_x = position.x;
    int mac_y = position.y;

    if ( IsButton() )
    {
        Rect contrlRect;
        GetControlBounds( m_controlHandle, &contrlRect );
        int former_mac_x = contrlRect.left;
        int former_mac_y = contrlRect.top;
        GetToolBar()->GetToolSize();

        if ( mac_x != former_mac_x || mac_y != former_mac_y )
        {
            UMAMoveControl( m_controlHandle, mac_x, mac_y );
        }
    }
    else if ( IsControl() )
    {
        // embedded native controls are moved by the OS
#if wxMAC_USE_NATIVE_TOOLBAR
        if ( ((wxToolBar*)GetToolBar())->MacWantsNativeToolbar() == false )
#endif
        {
            GetControl()->Move( position );
        }
    }
    else
    {
        // separator
#ifdef __WXMAC_OSX__
        Rect contrlRect;
        GetControlBounds( m_controlHandle, &contrlRect );
        int former_mac_x = contrlRect.left;
        int former_mac_y = contrlRect.top;

        if ( mac_x != former_mac_x || mac_y != former_mac_y )
            UMAMoveControl( m_controlHandle, mac_x, mac_y );
#endif
    }
}

void wxToolBarTool::UpdateToggleImage( bool toggle )
{
#if wxMAC_USE_NATIVE_TOOLBAR

#if MAC_OS_X_VERSION_MAX_ALLOWED < MAC_OS_X_VERSION_10_4
#define kHIToolbarItemSelected (1 << 7)
#endif

    // FIXME: this should be a OSX v10.4 runtime check
    if (m_toolbarItemRef != NULL)
    {
        OptionBits addAttrs, removeAttrs;
        OSStatus result;

        if (toggle)
        {
            addAttrs = kHIToolbarItemSelected;
            removeAttrs = kHIToolbarItemNoAttributes;
        }
        else
        {
            addAttrs = kHIToolbarItemNoAttributes;
            removeAttrs = kHIToolbarItemSelected;
        }

        result = HIToolbarItemChangeAttributes( m_toolbarItemRef, addAttrs, removeAttrs );
    }
#endif

#ifdef __WXMAC_OSX__
    if ( toggle )
    {
        int w = m_bmpNormal.GetWidth();
        int h = m_bmpNormal.GetHeight();
        wxBitmap bmp( w, h );
        wxMemoryDC dc;

        dc.SelectObject( bmp );
        dc.SetPen( wxNullPen );
        dc.SetBackground( *wxWHITE );
        dc.DrawRectangle( 0, 0, w, h );
        dc.DrawBitmap( m_bmpNormal, 0, 0, true );
        dc.SelectObject( wxNullBitmap );
        ControlButtonContentInfo info;
        wxMacCreateBitmapButton( &info, bmp );
        SetControlData( m_controlHandle, 0, kControlIconContentTag, sizeof(info), (Ptr)&info );
        wxMacReleaseBitmapButton( &info );
    }
    else
    {
        ControlButtonContentInfo info;
        wxMacCreateBitmapButton( &info, m_bmpNormal );
        SetControlData( m_controlHandle, 0, kControlIconContentTag, sizeof(info), (Ptr)&info );
        wxMacReleaseBitmapButton( &info );
    }

    IconTransformType transform = toggle ? kTransformSelected : kTransformNone;
    SetControlData(
        m_controlHandle, 0, kControlIconTransformTag,
        sizeof(transform), (Ptr)&transform );
    HIViewSetNeedsDisplay( m_controlHandle, true );

#else
    ::SetControl32BitValue( m_controlHandle, toggle );
#endif
}

wxToolBarTool::wxToolBarTool(
    wxToolBar *tbar,
    int id,
    const wxString& label,
    const wxBitmap& bmpNormal,
    const wxBitmap& bmpDisabled,
    wxItemKind kind,
    wxObject *clientData,
    const wxString& shortHelp,
    const wxString& longHelp )
    :
    wxToolBarToolBase(
        tbar, id, label, bmpNormal, bmpDisabled, kind,
        clientData, shortHelp, longHelp )
{
    Init();
}

#pragma mark -
#pragma mark Toolbar Implementation

wxToolBarToolBase *wxToolBar::CreateTool(
    int id,
    const wxString& label,
    const wxBitmap& bmpNormal,
    const wxBitmap& bmpDisabled,
    wxItemKind kind,
    wxObject *clientData,
    const wxString& shortHelp,
    const wxString& longHelp )
{
    return new wxToolBarTool(
        this, id, label, bmpNormal, bmpDisabled, kind,
        clientData, shortHelp, longHelp );
}

wxToolBarToolBase * wxToolBar::CreateTool( wxControl *control )
{
    return new wxToolBarTool( this, control );
}

void wxToolBar::Init()
{
    m_maxWidth = -1;
    m_maxHeight = -1;
    m_defaultWidth = kwxMacToolBarToolDefaultWidth;
    m_defaultHeight = kwxMacToolBarToolDefaultHeight;

#if wxMAC_USE_NATIVE_TOOLBAR
    m_macHIToolbarRef = NULL;
    m_macUsesNativeToolbar = false;
#endif
}

#define kControlToolbarItemClassID		CFSTR( "org.wxwidgets.controltoolbaritem" )

const EventTypeSpec kEvents[] = 
{
	{ kEventClassHIObject, kEventHIObjectConstruct },
	{ kEventClassHIObject, kEventHIObjectInitialize },
	{ kEventClassHIObject, kEventHIObjectDestruct },
	
	{ kEventClassToolbarItem, kEventToolbarItemCreateCustomView }
};

const EventTypeSpec kViewEvents[] = 
{ 
    { kEventClassControl, kEventControlGetSizeConstraints } 
};

struct ControlToolbarItem 
{ 
    HIToolbarItemRef    toolbarItem; 
    HIViewRef           viewRef;
    wxSize              lastValidSize ;
}; 

static pascal OSStatus ControlToolbarItemHandler( EventHandlerCallRef inCallRef, EventRef inEvent, void* inUserData )
{
	OSStatus			result = eventNotHandledErr;
	ControlToolbarItem*	object = (ControlToolbarItem*)inUserData;

	switch ( GetEventClass( inEvent ) )
	{
		case kEventClassHIObject:
			switch ( GetEventKind( inEvent ) )
			{
				case kEventHIObjectConstruct:
					{
						HIObjectRef			toolbarItem;
						ControlToolbarItem*	item;
						
						GetEventParameter( inEvent, kEventParamHIObjectInstance, typeHIObjectRef, NULL,
                            sizeof( HIObjectRef ), NULL, &toolbarItem );
						
                        item = (ControlToolbarItem*) malloc(sizeof(ControlToolbarItem)) ;
                        item->toolbarItem = toolbarItem ;
                        item->viewRef = NULL ;
                        
						SetEventParameter( inEvent, kEventParamHIObjectInstance, typeVoidPtr, sizeof( void * ), &item );
                        
                        result = noErr ;
					}
					break;
 
                case kEventHIObjectInitialize:
                    result = CallNextEventHandler( inCallRef, inEvent );
					if ( result == noErr )
                    {
                        CFDataRef           data;
                        GetEventParameter( inEvent, kEventParamToolbarItemConfigData, typeCFTypeRef, NULL,
                            sizeof( CFTypeRef ), NULL, &data );
					
                        HIViewRef viewRef ;
                        
                        wxASSERT_MSG( CFDataGetLength( data ) == sizeof( viewRef ) , wxT("Illegal Data passed") ) ;
                        memcpy( &viewRef , CFDataGetBytePtr( data ) , sizeof( viewRef ) ) ;
                    
                        object->viewRef = (HIViewRef) viewRef ;

						result = noErr ;
					}
                    break;

				case kEventHIObjectDestruct:
                    free( object ) ;
					result = noErr;
					break;
			}
			break;
		
		case kEventClassToolbarItem:
			switch ( GetEventKind( inEvent ) )
			{				
				case kEventToolbarItemCreateCustomView:
				{
                    HIViewRef viewRef = object->viewRef ;

                    HIViewRemoveFromSuperview( viewRef ) ;
                    HIViewSetVisible(viewRef, true) ;
                    InstallEventHandler( GetControlEventTarget( viewRef ), ControlToolbarItemHandler,
                                            GetEventTypeCount( kViewEvents ), kViewEvents, object, NULL );
                    
                    result = SetEventParameter( inEvent, kEventParamControlRef, typeControlRef, sizeof( HIViewRef ), &viewRef );
				}
				break;
			}
			break;
		
		case kEventClassControl:
			switch ( GetEventKind( inEvent ) )
			{
				case kEventControlGetSizeConstraints:
				{
                    wxWindow* wxwindow = wxFindControlFromMacControl(object->viewRef ) ;
                    if ( wxwindow )
                    {
                        wxSize sz = wxwindow->GetSize() ;
                        sz.x -= wxwindow->MacGetLeftBorderSize() + wxwindow->MacGetRightBorderSize();
                        sz.y -= wxwindow->MacGetTopBorderSize() + wxwindow->MacGetBottomBorderSize();
                        // during toolbar layout the native window sometimes gets negative sizes
                        // so we always keep the last valid size here, to make sure we survive the
                        // shuffle ...
                        if ( sz.x > 0 && sz.y > 0 )
                            object->lastValidSize = sz ;
                        else
                            sz = object->lastValidSize ;
                            
                        HISize min, max;
                        min.width = max.width = sz.x ;
                        min.height = max.height = sz.y ;
                        
                        result = SetEventParameter( inEvent, kEventParamMinimumSize, typeHISize,
                                                        sizeof( HISize ), &min );
                        
                        result = SetEventParameter( inEvent, kEventParamMaximumSize, typeHISize,
                                                        sizeof( HISize ), &max );
                        result = noErr ;
                    }
				}
				break;
			}
			break;
	}
	
	return result;
}

void RegisterControlToolbarItemClass()
{
	static bool sRegistered;
	
	if ( !sRegistered )
	{
		HIObjectRegisterSubclass( kControlToolbarItemClassID, kHIToolbarItemClassID, 0,
				ControlToolbarItemHandler, GetEventTypeCount( kEvents ), kEvents, 0, NULL );
		
		sRegistered = true;
	}
}

HIToolbarItemRef CreateControlToolbarItem(CFStringRef inIdentifier, CFTypeRef inConfigData)
{
	RegisterControlToolbarItemClass();
    
	OSStatus			err;
	EventRef			event;
	UInt32				options = kHIToolbarItemAllowDuplicates;
	HIToolbarItemRef	result = NULL;
	
	err = CreateEvent( NULL, kEventClassHIObject, kEventHIObjectInitialize, GetCurrentEventTime(), 0, &event );
	require_noerr( err, CantCreateEvent );
	
	SetEventParameter( event, kEventParamAttributes, typeUInt32, sizeof( UInt32 ), &options );
	SetEventParameter( event, kEventParamToolbarItemIdentifier, typeCFStringRef, sizeof( CFStringRef ), &inIdentifier );
	
	if ( inConfigData )
		SetEventParameter( event, kEventParamToolbarItemConfigData, typeCFTypeRef, sizeof( CFTypeRef ), &inConfigData );
	
	err = HIObjectCreate( kControlToolbarItemClassID, event, (HIObjectRef*)&result );
	check_noerr( err );
	
	ReleaseEvent( event );
CantCreateEvent :	
	return result ;
}

static const EventTypeSpec kToolbarEvents[] =
{
	{ kEventClassToolbar, kEventToolbarGetDefaultIdentifiers },
	{ kEventClassToolbar, kEventToolbarGetAllowedIdentifiers },
	{ kEventClassToolbar, kEventToolbarCreateItemWithIdentifier },
};

static OSStatus ToolbarDelegateHandler( EventHandlerCallRef inCallRef, EventRef inEvent, void* inUserData )
{
	OSStatus result = eventNotHandledErr;
    // Not yet needed
    // wxToolBar* toolbar = (wxToolBar*) inUserData ;
	CFMutableArrayRef	array;

	switch ( GetEventKind( inEvent ) )
	{
		case kEventToolbarGetDefaultIdentifiers:
            {
                GetEventParameter( inEvent, kEventParamMutableArray, typeCFMutableArrayRef, NULL,
					sizeof( CFMutableArrayRef ), NULL, &array );
                // not implemented yet
                // GetToolbarDefaultItems( array );
                result = noErr;
            }
			break;
			
		case kEventToolbarGetAllowedIdentifiers:
            {
                GetEventParameter( inEvent, kEventParamMutableArray, typeCFMutableArrayRef, NULL,
					sizeof( CFMutableArrayRef ), NULL, &array );
                // not implemented yet
                // GetToolbarAllowedItems( array );
                result = noErr;
            }
			break;
		case kEventToolbarCreateItemWithIdentifier:
			{
				HIToolbarItemRef		item = NULL;
				CFTypeRef				data = NULL;
                CFStringRef             identifier = NULL ;
				
				GetEventParameter( inEvent, kEventParamToolbarItemIdentifier, typeCFStringRef, NULL,
						sizeof( CFStringRef ), NULL, &identifier );
				
				GetEventParameter( inEvent, kEventParamToolbarItemConfigData, typeCFTypeRef, NULL,
						sizeof( CFTypeRef ), NULL, &data );
					
                if ( CFStringCompare( kControlToolbarItemClassID, identifier, kCFCompareBackwards ) == kCFCompareEqualTo )
                {
                    item = CreateControlToolbarItem( kControlToolbarItemClassID, data );
                    if ( item )
                    {
                        SetEventParameter( inEvent, kEventParamToolbarItem, typeHIToolbarItemRef,
                            sizeof( HIToolbarItemRef ), &item );
                        result = noErr;
                    }
				}
                
			}
			break;
    }
    return result ;
}

// also for the toolbar we have the dual implementation:
// only when MacInstallNativeToolbar is called is the native toolbar set as the window toolbar

bool wxToolBar::Create(
    wxWindow *parent,
    wxWindowID id,
    const wxPoint& pos,
    const wxSize& size,
    long style,
    const wxString& name )
{
    if ( !wxToolBarBase::Create( parent, id, pos, size, style, wxDefaultValidator, name ) )
        return false;

    OSStatus err = noErr;

#if wxMAC_USE_NATIVE_TOOLBAR
    wxString labelStr = wxString::Format( wxT("%xd"), (int)this );
    err = HIToolbarCreate(
        wxMacCFStringHolder( labelStr, wxFont::GetDefaultEncoding() ), 0,
        (HIToolbarRef*) &m_macHIToolbarRef );

    if (m_macHIToolbarRef != NULL)
    {
		InstallEventHandler( HIObjectGetEventTarget((HIToolbarRef)m_macHIToolbarRef ), ToolbarDelegateHandler,
				GetEventTypeCount( kToolbarEvents ), kToolbarEvents, this, NULL );

        HIToolbarDisplayMode mode = kHIToolbarDisplayModeDefault;
        HIToolbarDisplaySize displaySize = kHIToolbarDisplaySizeSmall;

        if ( style & wxTB_NOICONS )
            mode = kHIToolbarDisplayModeLabelOnly;
        else if ( style & wxTB_TEXT )
            mode = kHIToolbarDisplayModeIconAndLabel;
        else
            mode = kHIToolbarDisplayModeIconOnly;

        HIToolbarSetDisplayMode( (HIToolbarRef) m_macHIToolbarRef, mode );
        HIToolbarSetDisplaySize( (HIToolbarRef) m_macHIToolbarRef, displaySize );
    }
#endif

    return (err == noErr);
}

wxToolBar::~wxToolBar()
{
#if wxMAC_USE_NATIVE_TOOLBAR
    if (m_macHIToolbarRef != NULL)
    {
        // if this is the installed toolbar, then deinstall it
        if (m_macUsesNativeToolbar)
            MacInstallNativeToolbar( false );

        CFRelease( (HIToolbarRef)m_macHIToolbarRef );
        m_macHIToolbarRef = NULL;
    }
#endif
}

bool wxToolBar::Show( bool show )
{
    WindowRef tlw = MAC_WXHWND(MacGetTopLevelWindowRef());
    bool bResult = (tlw != NULL);

    if (bResult)
    {
#if wxMAC_USE_NATIVE_TOOLBAR
        bool ownToolbarInstalled = false;
        MacTopLevelHasNativeToolbar( &ownToolbarInstalled );
        if (ownToolbarInstalled)
        {
            bResult = (IsWindowToolbarVisible( tlw ) != show);
            if ( bResult )
                ShowHideWindowToolbar( tlw, show, false );
        }
        else
            bResult = wxToolBarBase::Show( show );
#else

        bResult = wxToolBarBase::Show( show );
#endif
    }

    return bResult;
}

bool wxToolBar::IsShown() const
{
    bool bResult;

#if wxMAC_USE_NATIVE_TOOLBAR
    bool ownToolbarInstalled;

    MacTopLevelHasNativeToolbar( &ownToolbarInstalled );
    if (ownToolbarInstalled)
    {
        WindowRef tlw = MAC_WXHWND(MacGetTopLevelWindowRef());
        bResult = IsWindowToolbarVisible( tlw );
    }
    else
        bResult = wxToolBarBase::IsShown();
#else

    bResult = wxToolBarBase::IsShown();
#endif

    return bResult;
}

void wxToolBar::DoGetSize( int *width, int *height ) const
{
#if wxMAC_USE_NATIVE_TOOLBAR
    Rect    boundsR;
    bool    ownToolbarInstalled;

    MacTopLevelHasNativeToolbar( &ownToolbarInstalled );
    if ( ownToolbarInstalled )
    {
        // TODO: is this really a control ?
        GetControlBounds( (ControlRef) m_macHIToolbarRef, &boundsR );
        if ( width != NULL )
            *width = boundsR.right - boundsR.left;
        if ( height != NULL )
            *height = boundsR.bottom - boundsR.top;
    }
    else
        wxToolBarBase::DoGetSize( width, height );

#else
    wxToolBarBase::DoGetSize( width, height );
#endif
}

wxSize wxToolBar::DoGetBestSize() const
{
    int width, height;

    DoGetSize( &width, &height );

    return wxSize( width, height );
}

void wxToolBar::SetWindowStyleFlag( long style )
{
    wxToolBarBase::SetWindowStyleFlag( style );

#if wxMAC_USE_NATIVE_TOOLBAR
    if (m_macHIToolbarRef != NULL)
    {
        HIToolbarDisplayMode mode = kHIToolbarDisplayModeDefault;

        if ( style & wxTB_NOICONS )
            mode = kHIToolbarDisplayModeLabelOnly;
        else if ( style & wxTB_TEXT )
            mode = kHIToolbarDisplayModeIconAndLabel;
        else
            mode = kHIToolbarDisplayModeIconOnly;

       HIToolbarSetDisplayMode( (HIToolbarRef) m_macHIToolbarRef, mode );
    }
#endif
}

#if wxMAC_USE_NATIVE_TOOLBAR
bool wxToolBar::MacWantsNativeToolbar()
{
    return m_macUsesNativeToolbar;
}

bool wxToolBar::MacTopLevelHasNativeToolbar(bool *ownToolbarInstalled) const
{
    bool bResultV = false;

    if (ownToolbarInstalled != NULL)
        *ownToolbarInstalled = false;

    WindowRef tlw = MAC_WXHWND(MacGetTopLevelWindowRef());
    if (tlw != NULL)
    {
        HIToolbarRef curToolbarRef = NULL;
        OSStatus err = GetWindowToolbar( tlw, &curToolbarRef );
        bResultV = ((err == noErr) && (curToolbarRef != NULL));
        if (bResultV && (ownToolbarInstalled != NULL))
            *ownToolbarInstalled = (curToolbarRef == m_macHIToolbarRef);
    }

    return bResultV;
}

bool wxToolBar::MacInstallNativeToolbar(bool usesNative)
{
    bool bResult = false;

    if (usesNative && (m_macHIToolbarRef == NULL))
        return bResult;

    if (usesNative && ((GetWindowStyleFlag() & wxTB_VERTICAL) != 0))
        return bResult;

    WindowRef tlw = MAC_WXHWND(MacGetTopLevelWindowRef());
    if (tlw == NULL)
        return bResult;

    // check the existing toolbar
    HIToolbarRef curToolbarRef = NULL;
    OSStatus err = GetWindowToolbar( tlw, &curToolbarRef );
    if (err != noErr)
        curToolbarRef = NULL;

    m_macUsesNativeToolbar = usesNative;

    if (m_macUsesNativeToolbar)
    {
        // only install toolbar if there isn't one installed already
        if (curToolbarRef == NULL)
        {
            bResult = true;

            SetWindowToolbar( tlw, (HIToolbarRef) m_macHIToolbarRef );
            ShowHideWindowToolbar( tlw, true, false );
            ChangeWindowAttributes( tlw, kWindowToolbarButtonAttribute, 0 );
            SetAutomaticControlDragTrackingEnabledForWindow( tlw, true );

            Rect r = { 0, 0, 0, 0 };
            m_peer->SetRect( &r );
            SetSize( wxSIZE_AUTO_WIDTH, 0 );
            m_peer->SetVisibility( false, true );
            wxToolBarBase::Show( false );
        }
    }
    else
    {
        // only deinstall toolbar if this is the installed one
        if (m_macHIToolbarRef == curToolbarRef)
        {
            bResult = true;

            ShowHideWindowToolbar( tlw, false, false );
            ChangeWindowAttributes( tlw, 0, kWindowToolbarButtonAttribute );
            SetWindowToolbar( tlw, NULL );

            m_peer->SetVisibility( true, true );
        }
    }

    if (bResult)
        InvalidateBestSize();

// wxLogDebug( wxT("    --> [%lx] - result [%s]"), (long)this, bResult ? wxT("T") : wxT("F") );
    return bResult;
}
#endif

bool wxToolBar::Realize()
{
    if (m_tools.GetCount() == 0)
        return false;

    int maxWidth = 0;
    int maxHeight = 0;

    int maxToolWidth = 0;
    int maxToolHeight = 0;

    int x = m_xMargin + kwxMacToolBarLeftMargin;
    int y = m_yMargin + kwxMacToolBarTopMargin;

    int tw, th;
    GetSize( &tw, &th );

    // find the maximum tool width and height
    wxToolBarTool *tool;
    wxToolBarToolsList::compatibility_iterator node = m_tools.GetFirst();
    while ( node != NULL )
    {
        tool = (wxToolBarTool *) node->GetData();
        if ( tool != NULL )
        {
            wxSize  sz = tool->GetSize();

            if ( sz.x > maxToolWidth )
                maxToolWidth = sz.x;
            if ( sz.y > maxToolHeight )
                maxToolHeight = sz.y;
        }

        node = node->GetNext();
    }

    bool lastIsRadio = false;
    bool curIsRadio = false;
    bool setChoiceInGroup = false;

#if wxMAC_USE_NATIVE_TOOLBAR
    CFIndex currentPosition = 0;
    bool insertAll = false;
#endif

    node = m_tools.GetFirst();
    while ( node != NULL )
    {
        tool = (wxToolBarTool*) node->GetData();
        if ( tool == NULL )
        {
            node = node->GetNext();
            continue;
        }

        // set tool position:
        // for the moment just perform a single row/column alignment
        wxSize  cursize = tool->GetSize();
        if ( x + cursize.x > maxWidth )
            maxWidth = x + cursize.x;
        if ( y + cursize.y > maxHeight )
            maxHeight = y + cursize.y;

        if ( GetWindowStyleFlag() & wxTB_VERTICAL )
        {
            int x1 = x + ( maxToolWidth - cursize.x ) / 2;
            tool->SetPosition( wxPoint(x1, y) );
        }
        else
        {
            int y1 = y + ( maxToolHeight - cursize.y ) / 2;
            tool->SetPosition( wxPoint(x, y1) );
        }

        // update the item positioning state
        if ( GetWindowStyleFlag() & wxTB_VERTICAL )
            y += cursize.y + kwxMacToolSpacing;
        else
            x += cursize.x + kwxMacToolSpacing;

#if wxMAC_USE_NATIVE_TOOLBAR
        // install in native HIToolbar
        if ( m_macHIToolbarRef != NULL )
        {
            HIToolbarItemRef    hiItemRef = tool->GetToolbarItemRef();
            if ( hiItemRef != NULL )
            {
                if ( insertAll || (tool->GetIndex() != currentPosition) )
                {
                    OSStatus err = noErr;
                    if ( !insertAll )
                    {
                        insertAll = true;

                        // if this is the first tool that gets newly inserted or repositioned
                        // first remove all 'old' tools from here to the right, because of this
                        // all following tools will have to be reinserted (insertAll). i = 100 because there's
                        // no way to determine how many there are in a toolbar, so just a high number :-(
                        for ( CFIndex i = 100; i >= currentPosition; --i )
                        {
                            err = HIToolbarRemoveItemAtIndex( (HIToolbarRef) m_macHIToolbarRef, i );
                        }

                        if (err != noErr)
                        {
                            wxString errMsg = wxString::Format( wxT("HIToolbarRemoveItemAtIndex failed [%ld]"), (long)err );
                            wxFAIL_MSG( errMsg.c_str() );
                        }
                    }

                    err = HIToolbarInsertItemAtIndex( (HIToolbarRef) m_macHIToolbarRef, hiItemRef, currentPosition );
                    if (err != noErr)
                    {
                        wxString errMsg = wxString::Format( wxT("HIToolbarInsertItemAtIndex failed [%ld]"), (long)err );
                        wxFAIL_MSG( errMsg.c_str() );
                    }

                    tool->SetIndex( currentPosition );
                }

                currentPosition++;
            }
        }
#endif

        // update radio button (and group) state
        lastIsRadio = curIsRadio;
        curIsRadio = ( tool->IsButton() && (tool->GetKind() == wxITEM_RADIO) );

        if ( !curIsRadio )
        {
            if ( tool->IsToggled() )
                DoToggleTool( tool, true );

            setChoiceInGroup = false;
        }
        else
        {
            if ( !lastIsRadio )
            {
                if ( tool->Toggle( true ) )
                {
                    DoToggleTool( tool, true );
                    setChoiceInGroup = true;
                }
            }
            else if ( tool->IsToggled() )
            {
                if ( tool->IsToggled() )
                    DoToggleTool( tool, true );

                wxToolBarToolsList::compatibility_iterator  nodePrev = node->GetPrevious();
                while ( nodePrev != NULL )
                {
                    wxToolBarToolBase   *toggleTool = nodePrev->GetData();
                    if ( (toggleTool == NULL) || !toggleTool->IsButton() || (toggleTool->GetKind() != wxITEM_RADIO) )
                        break;

                    if ( toggleTool->Toggle( false ) )
                        DoToggleTool( toggleTool, false );

                    nodePrev = nodePrev->GetPrevious();
                }
            }
        }

        node = node->GetNext();
    }

    if ( GetWindowStyleFlag() & wxTB_HORIZONTAL )
    {
        // if not set yet, only one row
        if ( m_maxRows <= 0 )
            SetRows( 1 );

        m_minWidth = maxWidth;
        maxWidth = tw;
        maxHeight += m_yMargin + kwxMacToolBarTopMargin;
        m_minHeight = m_maxHeight = maxHeight;
    }
    else
    {
        // if not set yet, have one column
        if ( (GetToolsCount() > 0) && (m_maxRows <= 0) )
            SetRows( GetToolsCount() );

        m_minHeight = maxHeight;
        maxHeight = th;
        maxWidth += m_xMargin + kwxMacToolBarLeftMargin;
        m_minWidth = m_maxWidth = maxWidth;
    }

#if 0
    // FIXME: should this be OSX-only?
    {
        bool wantNativeToolbar, ownToolbarInstalled;

        // attempt to install the native toolbar
        wantNativeToolbar = ((GetWindowStyleFlag() & wxTB_VERTICAL) == 0);
        MacInstallNativeToolbar( wantNativeToolbar );
        (void)MacTopLevelHasNativeToolbar( &ownToolbarInstalled );
        if (!ownToolbarInstalled)
        {
           SetSize( maxWidth, maxHeight );
           InvalidateBestSize();
        }
    }
#else
    SetSize( maxWidth, maxHeight );
    InvalidateBestSize();
#endif

    SetBestFittingSize();

    return true;
}

void wxToolBar::SetToolBitmapSize(const wxSize& size)
{
    m_defaultWidth = size.x + kwxMacToolBorder;
    m_defaultHeight = size.y + kwxMacToolBorder;

#if wxMAC_USE_NATIVE_TOOLBAR
    if (m_macHIToolbarRef != NULL)
    {
        int maxs = wxMax( size.x, size.y );
        HIToolbarDisplaySize sizeSpec;
        if ( maxs > 32 )
            sizeSpec = kHIToolbarDisplaySizeNormal;
        else if ( maxs > 24 )
            sizeSpec = kHIToolbarDisplaySizeDefault;
        else
            sizeSpec = kHIToolbarDisplaySizeSmall;

        HIToolbarSetDisplaySize( (HIToolbarRef) m_macHIToolbarRef, sizeSpec );
    }
#endif
}

// The button size is bigger than the bitmap size
wxSize wxToolBar::GetToolSize() const
{
    return wxSize(m_defaultWidth + kwxMacToolBorder, m_defaultHeight + kwxMacToolBorder);
}

void wxToolBar::SetRows(int nRows)
{
    // avoid resizing the frame uselessly
    if ( nRows != m_maxRows )
        m_maxRows = nRows;
}

void wxToolBar::MacSuperChangedPosition()
{
    wxWindow::MacSuperChangedPosition();

#if wxMAC_USE_NATIVE_TOOLBAR
    if (! m_macUsesNativeToolbar )
        Realize();
#else

    Realize();
#endif
}

wxToolBarToolBase *wxToolBar::FindToolForPosition(wxCoord x, wxCoord y) const
{
    wxToolBarTool *tool;
    wxToolBarToolsList::compatibility_iterator node = m_tools.GetFirst();
    while ( node != NULL )
    {
        tool = (wxToolBarTool *)node->GetData();
        if (tool != NULL)
        {
            wxRect2DInt r( tool->GetPosition(), tool->GetSize() );
            if ( r.Contains( wxPoint( x, y ) ) )
                return tool;
        }

        node = node->GetNext();
    }

    return (wxToolBarToolBase*)NULL;
}

wxString wxToolBar::MacGetToolTipString( wxPoint &pt )
{
    wxToolBarToolBase *tool = FindToolForPosition( pt.x, pt.y );
    if ( tool != NULL )
        return tool->GetShortHelp();

    return wxEmptyString;
}

void wxToolBar::DoEnableTool(wxToolBarToolBase *t, bool enable)
{
    if ( t != NULL )
        ((wxToolBarTool*)t)->DoEnable( enable );
}

void wxToolBar::DoToggleTool(wxToolBarToolBase *t, bool toggle)
{
    wxToolBarTool *tool = (wxToolBarTool *)t;
    if ( ( tool != NULL ) && tool->IsButton() )
        tool->UpdateToggleImage( toggle );
}

bool wxToolBar::DoInsertTool(size_t WXUNUSED(pos), wxToolBarToolBase *toolBase)
{
    wxToolBarTool *tool = wx_static_cast( wxToolBarTool*, toolBase );
    if (tool == NULL)
        return false;

    WindowRef window = (WindowRef) MacGetTopLevelWindowRef();
    wxSize toolSize = GetToolSize();
    Rect toolrect = { 0, 0, toolSize.y, toolSize.x };
    ControlRef controlHandle = NULL;
    OSStatus err = 0;

    switch (tool->GetStyle())
    {
        case wxTOOL_STYLE_SEPARATOR:
            {
                wxASSERT( tool->GetControlHandle() == NULL );
                toolSize.x /= 4;
                toolSize.y /= 4;
                if ( GetWindowStyleFlag() & wxTB_VERTICAL )
                    toolrect.bottom = toolSize.y;
                else
                    toolrect.right = toolSize.x;

#ifdef __WXMAC_OSX__
                // in flat style we need a visual separator
#if wxMAC_USE_NATIVE_TOOLBAR
                HIToolbarItemRef item;
                err = HIToolbarItemCreate(
                    kHIToolbarSeparatorIdentifier,
                    kHIToolbarItemCantBeRemoved | kHIToolbarItemIsSeparator | kHIToolbarItemAllowDuplicates,
                    &item );
                if (err == noErr)
                    tool->SetToolbarItemRef( item );
#endif

                CreateSeparatorControl( window, &toolrect, &controlHandle );
                tool->SetControlHandle( controlHandle );
#endif
            }
            break;

        case wxTOOL_STYLE_BUTTON:
            {
                wxASSERT( tool->GetControlHandle() == NULL );
                ControlButtonContentInfo info;
                wxMacCreateBitmapButton( &info, tool->GetNormalBitmap(), kControlContentIconRef );

                if ( UMAGetSystemVersion() >= 0x1000)
                {
                    CreateIconControl( window, &toolrect, &info, false, &controlHandle );
                }
                else
                {
                    SInt16 behaviour = kControlBehaviorOffsetContents;
                    if ( tool->CanBeToggled() )
                        behaviour |= kControlBehaviorToggles;
                    err = CreateBevelButtonControl( window,
                        &toolrect, CFSTR(""), kControlBevelButtonNormalBevel,
                        behaviour, &info, 0, 0, 0, &controlHandle );
                }

#if wxMAC_USE_NATIVE_TOOLBAR
                HIToolbarItemRef item;
                wxString labelStr = wxString::Format(wxT("%xd"), (int)tool);
                err = HIToolbarItemCreate(
                    wxMacCFStringHolder(labelStr, wxFont::GetDefaultEncoding()),
                    kHIToolbarItemCantBeRemoved | kHIToolbarItemAnchoredLeft | kHIToolbarItemAllowDuplicates, &item );
                if (err  == noErr)
                {
                    InstallEventHandler(
                        HIObjectGetEventTarget(item), GetwxMacToolBarEventHandlerUPP(),
                        GetEventTypeCount(toolBarEventList), toolBarEventList, tool, NULL );
                    HIToolbarItemSetLabel( item, wxMacCFStringHolder(tool->GetLabel(), m_font.GetEncoding()) );
                    HIToolbarItemSetIconRef( item, info.u.iconRef );
                    HIToolbarItemSetCommandID( item, kHIToolbarCommandPressAction );
                    tool->SetToolbarItemRef( item );
                }
#endif

                wxMacReleaseBitmapButton( &info );

#if 0
                SetBevelButtonTextPlacement( m_controlHandle, kControlBevelButtonPlaceBelowGraphic );
                UMASetControlTitle( m_controlHandle, label, wxFont::GetDefaultEncoding() );
#endif

                InstallControlEventHandler(
                    (ControlRef) controlHandle, GetwxMacToolBarToolEventHandlerUPP(),
                    GetEventTypeCount(eventList), eventList, tool, NULL );

                tool->SetControlHandle( controlHandle );
            }
            break;

        case wxTOOL_STYLE_CONTROL:

#if wxMAC_USE_NATIVE_TOOLBAR
            {
	            wxASSERT( tool->GetControl() != NULL );
                HIToolbarItemRef    item;
                HIViewRef viewRef = (HIViewRef) tool->GetControl()->GetHandle() ;
                // as this control now is part of both the wxToolBar children and the native toolbar, we have to increase the
                // reference count to make sure we are not dealing with zombie controls after the native toolbar has released its views
                CFRetain( viewRef ) ;
                CFDataRef data = CFDataCreate( kCFAllocatorDefault , (UInt8*) &viewRef , sizeof(viewRef) ) ;
                 err = HIToolbarCreateItemWithIdentifier((HIToolbarRef) m_macHIToolbarRef,kControlToolbarItemClassID,
                   data , &item ) ;

                if (err  == noErr)
                {
                    tool->SetToolbarItemRef( item );
                }
                CFRelease( data ) ;
           }

#else
                // right now there's nothing to do here
#endif
                break;

        default:
            break;
    }

    if ( err == noErr )
    {
        if ( controlHandle )
        {
            ControlRef container = (ControlRef) GetHandle();
            wxASSERT_MSG( container != NULL, wxT("No valid Mac container control") );

            UMAShowControl( controlHandle );
            ::EmbedControl( controlHandle, container );
        }

        if ( tool->CanBeToggled() && tool->IsToggled() )
            tool->UpdateToggleImage( true );

        // nothing special to do here - we relayout in Realize() later
        tool->Attach( this );
        InvalidateBestSize();
    }
    else
    {
        wxString errMsg = wxString::Format( wxT("wxToolBar::DoInsertTool - failure [%ld]"), (long)err );
        wxFAIL_MSG( errMsg.c_str() );
    }

    return (err == noErr);
}

void wxToolBar::DoSetToggle(wxToolBarToolBase *WXUNUSED(tool), bool WXUNUSED(toggle))
{
    wxFAIL_MSG( wxT("not implemented") );
}

bool wxToolBar::DoDeleteTool(size_t WXUNUSED(pos), wxToolBarToolBase *toolbase)
{
    wxToolBarTool* tool = wx_static_cast( wxToolBarTool*, toolbase );
    wxToolBarToolsList::compatibility_iterator node;
    for ( node = m_tools.GetFirst(); node; node = node->GetNext() )
    {
        wxToolBarToolBase *tool2 = node->GetData();
        if ( tool2 == tool )
        {
            // let node point to the next node in the list
            node = node->GetNext();

            break;
        }
    }

    wxSize sz = ((wxToolBarTool*)tool)->GetSize();

    tool->Detach();

#if wxMAC_USE_NATIVE_TOOLBAR
    CFIndex removeIndex = tool->GetIndex();
#endif

    switch ( tool->GetStyle() )
    {
        case wxTOOL_STYLE_CONTROL:
            {
                tool->GetControl()->Destroy();
                tool->ClearControl();
            }
            break;

        case wxTOOL_STYLE_BUTTON:
        case wxTOOL_STYLE_SEPARATOR:
            if ( tool->GetControlHandle() )
            {
#if wxMAC_USE_NATIVE_TOOLBAR
                if ( removeIndex != -1 && m_macHIToolbarRef )
                {
                    HIToolbarRemoveItemAtIndex( (HIToolbarRef) m_macHIToolbarRef, removeIndex );
                    tool->SetIndex( -1 );
                }
#endif

                tool->ClearControl();
            }
            break;

        default:
            break;
    }

    // and finally reposition all the controls after this one

    for ( /* node -> first after deleted */; node; node = node->GetNext() )
    {
        wxToolBarTool *tool2 = (wxToolBarTool*) node->GetData();
        wxPoint pt = tool2->GetPosition();

        if ( GetWindowStyleFlag() & wxTB_VERTICAL )
            pt.y -= sz.y;
        else
            pt.x -= sz.x;

        tool2->SetPosition( pt );

#if wxMAC_USE_NATIVE_TOOLBAR
        if ( removeIndex != -1 && tool2->GetIndex() > removeIndex )
            tool2->SetIndex( tool2->GetIndex() - 1 );
#endif
    }

    InvalidateBestSize();

    return true;
}

void wxToolBar::OnPaint(wxPaintEvent& event)
{
#if wxMAC_USE_NATIVE_TOOLBAR
    if ( m_macUsesNativeToolbar )
    {
        event.Skip(true);
        return;
    }
#endif

    wxPaintDC dc(this);

    int w, h;
    GetSize( &w, &h );

    bool drawMetalTheme = MacGetTopLevelWindow()->MacGetMetalAppearance();
    bool minimumUmaAvailable = (UMAGetSystemVersion() >= 0x1030);

#if wxMAC_USE_CORE_GRAPHICS && MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_3
    if ( !drawMetalTheme && minimumUmaAvailable )
    {
        HIThemePlacardDrawInfo info;
        memset( &info, 0, sizeof(info) );
        info.version = 0;
        info.state = IsEnabled() ? kThemeStateActive : kThemeStateInactive;

        CGContextRef cgContext = (CGContextRef) MacGetCGContextRef();
        HIRect rect = CGRectMake( 0, 0, w, h );
        HIThemeDrawPlacard( &rect, &info, cgContext, kHIThemeOrientationNormal );
    }
    else
    {
        // leave the background as it is (striped or metal)
    }

#else

    const bool drawBorder = true;

    if (drawBorder)
    {
        wxMacPortSetter helper( &dc );

        if ( !drawMetalTheme || !minimumUmaAvailable )
        {
            Rect toolbarrect = { dc.YLOG2DEVMAC(0), dc.XLOG2DEVMAC(0),
                dc.YLOG2DEVMAC(h), dc.XLOG2DEVMAC(w) };

#if 0
            if ( toolbarrect.left < 0 )
                toolbarrect.left = 0;
            if ( toolbarrect.top < 0 )
                toolbarrect.top = 0;
#endif

            UMADrawThemePlacard( &toolbarrect, IsEnabled() ? kThemeStateActive : kThemeStateInactive );
        }
        else
        {
#if TARGET_API_MAC_OSX
            HIRect hiToolbarrect = CGRectMake(
                dc.YLOG2DEVMAC(0), dc.XLOG2DEVMAC(0),
                dc.YLOG2DEVREL(h), dc.XLOG2DEVREL(w) );
            CGContextRef cgContext;
            Rect bounds;

            GetPortBounds( (CGrafPtr) dc.m_macPort, &bounds );
            QDBeginCGContext( (CGrafPtr) dc.m_macPort, &cgContext );

            CGContextTranslateCTM( cgContext, 0, bounds.bottom - bounds.top );
            CGContextScaleCTM( cgContext, 1, -1 );

            HIThemeBackgroundDrawInfo drawInfo;
            drawInfo.version = 0;
            drawInfo.state = kThemeStateActive;
            drawInfo.kind = kThemeBackgroundMetal;
            HIThemeApplyBackground( &hiToolbarrect, &drawInfo, cgContext, kHIThemeOrientationNormal );

            QDEndCGContext( (CGrafPtr) dc.m_macPort, &cgContext );
#endif
        }
    }
#endif

    event.Skip();
}

#endif // wxUSE_TOOLBAR
