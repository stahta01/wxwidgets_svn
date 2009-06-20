/////////////////////////////////////////////////////////////////////////////
// Name:        src/osx/carbon/toolbar.cpp
// Purpose:     wxToolBar
// Author:      Stefan Csomor
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id: toolbar.cpp 54954 2008-08-03 11:27:03Z VZ $
// Copyright:   (c) Stefan Csomor
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#if wxUSE_TOOLBAR

#include "wx/toolbar.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wx/app.h"
#include "wx/osx/private.h"
#include "wx/geometry.h"
#include "wx/sysopt.h"

const short kwxMacToolBarToolDefaultWidth = 16;
const short kwxMacToolBarToolDefaultHeight = 16;
const short kwxMacToolBarTopMargin = 4;
const short kwxMacToolBarLeftMargin =  4;
const short kwxMacToolBorder = 0;
const short kwxMacToolSpacing = 6;

BEGIN_EVENT_TABLE(wxToolBar, wxToolBarBase)
    EVT_PAINT( wxToolBar::OnPaint )
END_EVENT_TABLE()


#pragma mark -
#pragma mark Tool Implementation

// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

// We have a dual implementation for each tool, WXWidget and NSToolbarItem*

// when embedding native controls in the native toolbar we must make sure the
// control does not get deleted behind our backs, so the retain count gets increased
// (after creation it is 1), first be the creation of the custom NSToolbarItem wrapper
// object, and second by the code 'creating' the custom HIView (which is the same as the
// already existing native control, therefore we just increase the ref count)
// when this view is removed from the native toolbar its count gets decremented again
// and when the HITooolbarItem wrapper object gets destroyed it is decremented as well
// so in the end the control lives with a refcount of one and can be disposed of by the
// wxControl code. For embedded controls on a non-native toolbar this ref count is less
// so we can only test against a range, not a specific value of the refcount.

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

    wxToolBarTool(wxToolBar *tbar, wxControl *control, const wxString& label)
        : wxToolBarToolBase(tbar, control, label)
    {
        Init();
        if (control != NULL)
            SetControlHandle( (WXWidget) control->GetHandle() );
    }

    virtual ~wxToolBarTool()
    {
        ClearControl();
    }

    WXWidget GetControlHandle()
    {
        return (WXWidget) m_controlHandle;
    }

    void SetControlHandle( WXWidget handle )
    {
        m_controlHandle = handle;
    }

    void SetPosition( const wxPoint& position );

    void ClearControl()
    {
        if ( m_controlHandle )
        {
            if ( !IsControl() )
            {
                [m_controlHandle retain];
            }
            else
            {
                // the embedded control is not under the responsibility of the tool, it gets disposed of in the
                // proper wxControl destructor
            }
            m_controlHandle = NULL ;
        }

#if wxOSX_USE_NATIVE_TOOLBAR
        if ( m_toolbarItem )
        {
            [m_toolbarItem release];
            m_toolbarItem = NULL;
        }
#endif // wxOSX_USE_NATIVE_TOOLBAR
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
            if ( GetToolBar()->GetWindowStyleFlag() & (wxTB_LEFT|wxTB_RIGHT) )
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

    void UpdateImages();
    
    void UpdateToggleImage( bool toggle );
    
    void UpdateLabel()
    {
#if wxOSX_USE_NATIVE_TOOLBAR
       if ( m_toolbarItem )
        {
            // strip mnemonics from the label for compatibility with the usual
            // labels in wxStaticText sense
            wxString labelStr = wxStripMenuCodes(m_label);
            wxCFStringRef l(labelStr, GetToolBarFontEncoding());

            [m_toolbarItem setLabel:l.AsNSString()];
            
            wxCFStringRef sh( GetShortHelp(), GetToolBarFontEncoding() );
            [m_toolbarItem setToolTip:sh.AsNSString()];
        }
#endif
    }

    void Action()
    {
        wxToolBar *tbar = (wxToolBar*) GetToolBar();
        if (CanBeToggled())
        {
            bool    shouldToggle;

            shouldToggle = !IsToggled();
            tbar->ToggleTool( GetId(), shouldToggle );
        }

        tbar->OnLeftClick( GetId(), IsToggled() );
    }
    
#if wxOSX_USE_NATIVE_TOOLBAR
    void SetToolbarItemRef( NSToolbarItem* ref )
    {
        if ( m_controlHandle )
            [m_controlHandle setHidden:YES];
        if ( m_toolbarItem )
            [m_toolbarItem release];

        m_toolbarItem = ref;
    }

    NSToolbarItem* GetToolbarItemRef() const
    {
        return m_toolbarItem;
    }

    void SetIndex( CFIndex idx )
    {
        m_index = idx;
    }

    CFIndex GetIndex() const
    {
        return m_index;
    }

    virtual void SetLabel(const wxString& label)
    {
        wxToolBarToolBase::SetLabel(label);
        UpdateLabel();
     }
#endif // wxOSX_USE_NATIVE_TOOLBAR

private:
#if wxOSX_USE_NATIVE_TOOLBAR
    wxFontEncoding GetToolBarFontEncoding() const
    {
        wxFont f;
        if ( GetToolBar() )
            f = GetToolBar()->GetFont();
        return f.IsOk() ? f.GetEncoding() : wxFont::GetDefaultEncoding();
    }
#endif // wxOSX_USE_NATIVE_TOOLBAR

    void Init()
    {
        m_controlHandle = NULL;

#if wxOSX_USE_NATIVE_TOOLBAR
        m_toolbarItem = NULL;
        m_index = -1;
#endif
    }

    WXWidget m_controlHandle;
    wxCoord     m_x;
    wxCoord     m_y;
    wxBitmap    m_alternateBitmap;

#if wxOSX_USE_NATIVE_TOOLBAR
    NSToolbarItem* m_toolbarItem;
    // position in its toolbar, -1 means not inserted
    CFIndex m_index;
#endif
};

#if wxOSX_USE_NATIVE_TOOLBAR

@interface wxNSToolbarItem : NSToolbarItem
{
    wxToolBarTool* impl;
}

- (id) initWithItemIdentifier: (NSString*) identifier;
- (void)setImplementation: (wxToolBarTool *) theImplementation;
- (wxToolBarTool*) implementation;
- (void) clickedAction: (id) sender;

@end


@interface wxNSToolbarDelegate : NSObject
{
}

- (NSToolbarItem *)toolbar:(NSToolbar *)toolbar itemForItemIdentifier:(NSString *)itemIdentifier willBeInsertedIntoToolbar:(BOOL)flag;
    
- (NSArray *)toolbarDefaultItemIdentifiers:(NSToolbar*)toolbar;

- (NSArray *)toolbarAllowedItemIdentifiers:(NSToolbar*)toolbar;

- (NSArray *)toolbarSelectableItemIdentifiers:(NSToolbar *)toolbar;


@end

#endif


@interface wxNSToolBarButton : NSButton
{
    wxToolBarTool* impl;
}

- (id)initWithFrame:(NSRect)frame;
- (void) clickedAction: (id) sender;
- (void)setImplementation: (wxToolBarTool *) theImplementation;
- (wxToolBarTool*) implementation;
- (BOOL) isFlipped;

@end

#if wxOSX_USE_NATIVE_TOOLBAR

@implementation wxNSToolbarItem

- (id)initWithItemIdentifier: (NSString*) identifier
{
    [super initWithItemIdentifier:identifier];
    impl = NULL;
    [self setTarget: self];
    [self setAction: @selector(clickedAction:)];
    return self;
}

- (void) clickedAction: (id) sender
{
    wxUnusedVar(sender);
    if ( impl )
    {
        impl->Action();
    }
}

- (void)setImplementation: (wxToolBarTool *) theImplementation
{
    impl = theImplementation;
}

- (wxToolBarTool*) implementation
{
    return impl;
}

@end

@implementation wxNSToolbarDelegate

- (NSArray *)toolbarDefaultItemIdentifiers:(NSToolbar*)toolbar
{
    wxUnusedVar(toolbar);
    return nil;
}

- (NSArray *)toolbarAllowedItemIdentifiers:(NSToolbar*)toolbar
{
    wxUnusedVar(toolbar);
    return nil;
}

- (NSArray *)toolbarSelectableItemIdentifiers:(NSToolbar *)toolbar
{
    wxUnusedVar(toolbar);
    return nil;
}

- (NSToolbarItem*) toolbar:(NSToolbar*) toolbar itemForItemIdentifier:(NSString*) itemIdentifier willBeInsertedIntoToolbar:(BOOL) flag
{
    wxUnusedVar(toolbar);
#ifdef __LP64__
    wxToolBarTool* tool = (wxToolBarTool*) [itemIdentifier longLongValue];
#else
    wxToolBarTool* tool = (wxToolBarTool*) [itemIdentifier intValue];
#endif
    if ( tool )
    {
        wxNSToolbarItem* item = (wxNSToolbarItem*) tool->GetToolbarItemRef();
        if ( flag && tool->IsControl() )
        {
            NSView* view = tool->GetControl()->GetHandle();
            [view removeFromSuperview];
            [item setView:view];
            wxSize sz = tool->GetControl()->GetSize();
            NSSize size = NSMakeSize((float)sz.x, (float)sz.y);
            [item setMaxSize:size];
            [item setMinSize:size];
            [view setHidden:NO];
        }
        return item;
    }
    return nil;
}

@end

#endif

@implementation wxNSToolBarButton

- (id)initWithFrame:(NSRect)frame
{
    [super initWithFrame:frame];
    impl = NULL;
    [self setTarget: self];
    [self setAction: @selector(clickedAction:)];
    return self;
}

- (void) clickedAction: (id) sender
{
    wxUnusedVar(sender);
    if ( impl )
    {
        impl->Action();
    }
}

- (void)setImplementation: (wxToolBarTool *) theImplementation
{
    impl = theImplementation;
}

- (wxToolBarTool*) implementation
{
    return impl;
}

- (BOOL) isFlipped
{
    return YES;
}

@end

bool wxToolBarTool::DoEnable( bool enable )
{
    if ( IsControl() )
    {
        GetControl()->Enable( enable );
    }
    else if ( IsButton() )
    {
#if wxOSX_USE_NATIVE_TOOLBAR
        if ( m_toolbarItem != NULL )
            [m_toolbarItem setEnabled:enable];
#endif

        if ( m_controlHandle != NULL )
            [(NSControl*)m_controlHandle setEnabled:enable];
    }

    return true;
}

void wxToolBarTool::SetPosition( const wxPoint& position )
{
    m_x = position.x;
    m_y = position.y;

    int mac_x = position.x;
    int mac_y = position.y;

    if ( IsButton() )
    {
        NSRect frame = [m_controlHandle frame];
        if ( frame.origin.x != mac_x || frame.origin.y != mac_y )
        {
            frame.origin.x = mac_x;
            frame.origin.y = mac_y;
            [m_controlHandle setFrame:frame];
        }
    }
    else if ( IsControl() )
    {
        // embedded native controls are moved by the OS
#if wxOSX_USE_NATIVE_TOOLBAR
        if ( ((wxToolBar*)GetToolBar())->MacWantsNativeToolbar() == false )
#endif
        {
            GetControl()->Move( position );
        }
    }
    else
    {
        NSRect frame = [m_controlHandle frame];
        if ( frame.origin.x != mac_x || frame.origin.y != mac_y )
        {
            frame.origin.x = mac_x;
            frame.origin.y = mac_y;
            [m_controlHandle setFrame:frame];
        }
    }    
}

void wxToolBarTool::UpdateImages()
{
    [(NSButton*) m_controlHandle setImage:m_bmpNormal.GetNSImage()];

    if ( CanBeToggled() )
    {
        int w = m_bmpNormal.GetWidth();
        int h = m_bmpNormal.GetHeight();
        m_alternateBitmap = wxBitmap( w, h );
        wxMemoryDC dc;

        dc.SelectObject( m_alternateBitmap );
        dc.SetPen( wxPen(*wxBLACK) );
        dc.SetBrush( wxBrush( *wxLIGHT_GREY ));
        dc.DrawRectangle( 0, 0, w, h );
        dc.DrawBitmap( m_bmpNormal, 0, 0, true );
        dc.SelectObject( wxNullBitmap );
        
        [(NSButton*) m_controlHandle setAlternateImage:m_alternateBitmap.GetNSImage()];
    }
    UpdateToggleImage( CanBeToggled() && IsToggled() );
}

void wxToolBarTool::UpdateToggleImage( bool toggle )
{
#if wxOSX_USE_NATIVE_TOOLBAR
    if (m_toolbarItem != NULL )
    {
        // the native toolbar item only has a 'selected' state (one for one toolbar)
        // so we emulate the toggle here
        if ( CanBeToggled() && toggle )
            [m_toolbarItem setImage:m_alternateBitmap.GetNSImage()];
        else
            [m_toolbarItem setImage:m_bmpNormal.GetNSImage()];
    }
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

wxToolBarToolBase *
wxToolBar::CreateTool(wxControl *control, const wxString& label)
{
    return new wxToolBarTool(this, control, label);
}

void wxToolBar::Init()
{
    m_maxWidth = -1;
    m_maxHeight = -1;
    m_defaultWidth = kwxMacToolBarToolDefaultWidth;
    m_defaultHeight = kwxMacToolBarToolDefaultHeight;

#if wxOSX_USE_NATIVE_TOOLBAR
    m_macToolbar = NULL;
    m_macUsesNativeToolbar = false;
#endif
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

    FixupStyle();

    OSStatus err = noErr;

#if wxOSX_USE_NATIVE_TOOLBAR

    if (parent->IsKindOf(CLASSINFO(wxFrame)) && wxSystemOptions::GetOptionInt(wxT("mac.toolbar.no-native")) != 1)
    {
        static wxNSToolbarDelegate* controller = nil;
        
        if ( controller == nil )
            controller = [[wxNSToolbarDelegate alloc] init];
        wxString identifier = wxString::Format( wxT("%p"), this );
        wxCFStringRef cfidentifier(identifier);
        NSToolbar* tb =  [[NSToolbar alloc] initWithIdentifier:cfidentifier.AsNSString()];
        
        m_macToolbar = tb ;
        
        if (m_macToolbar != NULL)
        {
            [tb setDelegate:controller];
            
            NSToolbarDisplayMode mode = NSToolbarDisplayModeDefault;
            NSToolbarSizeMode displaySize = NSToolbarSizeModeSmall;

            if ( style & wxTB_NOICONS )
                mode = NSToolbarDisplayModeLabelOnly;
            else if ( style & wxTB_TEXT )
                mode = NSToolbarDisplayModeIconAndLabel;
            else
                mode = NSToolbarDisplayModeIconOnly;

            [tb setDisplayMode:mode];
            [tb setSizeMode:displaySize];
       }
    }
#endif // wxOSX_USE_NATIVE_TOOLBAR

    return (err == noErr);
}

wxToolBar::~wxToolBar()
{
#if wxOSX_USE_NATIVE_TOOLBAR
    if (m_macToolbar != NULL)
    {
        // if this is the installed toolbar, then deinstall it
        if (m_macUsesNativeToolbar)
            MacInstallNativeToolbar( false );

        [(NSToolbar*)m_macToolbar setDelegate:nil];
        [(NSToolbar*)m_macToolbar release];
        m_macToolbar = NULL;
    }
#endif
}

bool wxToolBar::Show( bool show )
{
    WXWindow tlw = MacGetTopLevelWindowRef();
    bool bResult = (tlw != NULL);

    if (bResult)
    {
#if wxOSX_USE_NATIVE_TOOLBAR
        bool ownToolbarInstalled = false;
        MacTopLevelHasNativeToolbar( &ownToolbarInstalled );
        if (ownToolbarInstalled)
        {
            bResult = ([(NSToolbar*)m_macToolbar isVisible] != show);
            if ( bResult )
                [(NSToolbar*)m_macToolbar setVisible:show];
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

#if wxOSX_USE_NATIVE_TOOLBAR
    bool ownToolbarInstalled;

    MacTopLevelHasNativeToolbar( &ownToolbarInstalled );
    if (ownToolbarInstalled)
    {
        bResult = [(NSToolbar*)m_macToolbar isVisible];
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
#if wxOSX_USE_NATIVE_TOOLBAR
    bool    ownToolbarInstalled;

    MacTopLevelHasNativeToolbar( &ownToolbarInstalled );
    if ( ownToolbarInstalled )
    {
        WXWindow tlw = MacGetTopLevelWindowRef();
        float toolbarHeight = 0.0;
        NSRect windowFrame = NSMakeRect(0, 0, 0, 0);
 
        if(m_macToolbar && [(NSToolbar*)m_macToolbar isVisible])
        {
            windowFrame = [NSWindow contentRectForFrameRect:[tlw frame]
                                styleMask:[tlw styleMask]];
            toolbarHeight = NSHeight(windowFrame)
                        - NSHeight([[tlw contentView] frame]);
        }
 
        if ( width != NULL )
            *width = (int)windowFrame.size.width;
        if ( height != NULL )
            *height = (int)toolbarHeight;
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

#if wxOSX_USE_NATIVE_TOOLBAR
    if (m_macToolbar != NULL)
    {
        NSToolbarDisplayMode mode = NSToolbarDisplayModeDefault;

        if ( style & wxTB_NOICONS )
            mode = NSToolbarDisplayModeLabelOnly;
        else if ( style & wxTB_TEXT )
            mode = NSToolbarDisplayModeIconAndLabel;
        else
            mode = NSToolbarDisplayModeIconOnly;

        [(NSToolbar*) m_macToolbar setDisplayMode:mode];
    }
#endif
}

#if wxOSX_USE_NATIVE_TOOLBAR
bool wxToolBar::MacWantsNativeToolbar()
{
    return m_macUsesNativeToolbar;
}

bool wxToolBar::MacTopLevelHasNativeToolbar(bool *ownToolbarInstalled) const
{
    bool bResultV = false;

    if (ownToolbarInstalled != NULL)
        *ownToolbarInstalled = false;

    WXWindow tlw = MacGetTopLevelWindowRef();
    if (tlw != NULL)
    {
        NSToolbar* curToolbarRef = [tlw toolbar];
        bResultV = (curToolbarRef != NULL);
        if (bResultV && (ownToolbarInstalled != NULL))
            *ownToolbarInstalled = (curToolbarRef == m_macToolbar);
    }

    return bResultV;
}

bool wxToolBar::MacInstallNativeToolbar(bool usesNative)
{
    bool bResult = false;

    if (usesNative && (m_macToolbar == NULL))
        return bResult;

    if (usesNative && ((GetWindowStyleFlag() & (wxTB_LEFT|wxTB_RIGHT|wxTB_BOTTOM)) != 0))
        return bResult;

    WXWindow tlw = MacGetTopLevelWindowRef();
    if (tlw == NULL)
        return bResult;

    // check the existing toolbar
    NSToolbar* curToolbarRef = [tlw toolbar];

    m_macUsesNativeToolbar = usesNative;

    if (m_macUsesNativeToolbar)
    {
        // only install toolbar if there isn't one installed already
        if (curToolbarRef == NULL)
        {
            bResult = true;
            [tlw setToolbar:(NSToolbar*) m_macToolbar];
            [(NSToolbar*) m_macToolbar setVisible:YES];
            
            m_peer->Move(0,0,0,0 );
            SetSize( wxSIZE_AUTO_WIDTH, 0 );
            m_peer->SetVisibility( false );
            wxToolBarBase::Show( false );
        }
    }
    else
    {
        // only deinstall toolbar if this is the installed one
        if (m_macToolbar == curToolbarRef)
        {
            bResult = true;
            [(NSToolbar*) m_macToolbar setVisible:NO];
            [tlw setToolbar:nil];
            m_peer->SetVisibility( true );
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
    while ( node )
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

#if wxOSX_USE_NATIVE_TOOLBAR
    CFIndex currentPosition = 0;
    bool insertAll = false;

    NSToolbar* refTB = (NSToolbar*)m_macToolbar;
    wxFont f;
    wxFontEncoding enc;
    f = GetFont();
    if ( f.IsOk() )
        enc = f.GetEncoding();
    else
        enc = wxFont::GetDefaultEncoding();
#endif

    node = m_tools.GetFirst();
    while ( node )
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

        if ( GetWindowStyleFlag() & (wxTB_LEFT|wxTB_RIGHT) )
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
        if ( GetWindowStyleFlag() & (wxTB_LEFT|wxTB_RIGHT) )
            y += cursize.y + kwxMacToolSpacing;
        else
            x += cursize.x + kwxMacToolSpacing;

#if wxOSX_USE_NATIVE_TOOLBAR
        // install in native NSToolbar
        if ( refTB )
        {
            NSToolbarItem* hiItemRef = tool->GetToolbarItemRef();
            if ( hiItemRef != NULL )
            {
                // since setting the help texts is non-virtual we have to update
                // the strings now
                wxCFStringRef sh( tool->GetShortHelp(), enc);
                [hiItemRef setToolTip:sh.AsNSString()];

                if ( insertAll || (tool->GetIndex() != currentPosition) )
                {
                    if ( !insertAll )
                    {
                        insertAll = true;

                        // if this is the first tool that gets newly inserted or repositioned
                        // first remove all 'old' tools from here to the right, because of this
                        // all following tools will have to be reinserted (insertAll).
                        for ( wxToolBarToolsList::compatibility_iterator node2 = m_tools.GetLast();
                              node2 != node;
                              node2 = node2->GetPrevious() )
                        {
                            wxToolBarTool *tool2 = (wxToolBarTool*) node2->GetData();

                            const long idx = tool2->GetIndex();
                            if ( idx != -1 )
                            {
                                [refTB removeItemAtIndex:idx];
                                tool2->SetIndex(-1);
                            }
                        }
                    }
                    wxString identifier = wxString::Format( wxT("%ld"), (long) tool );
                    wxCFStringRef cfidentifier(identifier);

                    [refTB insertItemWithItemIdentifier:cfidentifier.AsNSString() atIndex:currentPosition];
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
        }
        else
        {
            if ( !lastIsRadio )
            {
                if ( tool->Toggle( true ) )
                {
                    DoToggleTool( tool, true );
                }
            }
            else if ( tool->IsToggled() )
            {
                if ( tool->IsToggled() )
                    DoToggleTool( tool, true );

                wxToolBarToolsList::compatibility_iterator  nodePrev = node->GetPrevious();
                while ( nodePrev )
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

    if ( GetWindowStyleFlag() & (wxTB_TOP|wxTB_BOTTOM) )
    {
        // if not set yet, only one row
        if ( m_maxRows <= 0 )
            SetRows( 1 );

        m_minWidth = maxWidth;
        // maxHeight = th;
        maxHeight += m_yMargin + kwxMacToolBarTopMargin;
        m_minHeight = m_maxHeight = maxHeight;
    }
    else
    {
        // if not set yet, have one column
        if ( (GetToolsCount() > 0) && (m_maxRows <= 0) )
            SetRows( GetToolsCount() );

        m_minHeight = maxHeight;
        // maxWidth = tw;
        maxWidth += m_xMargin + kwxMacToolBarLeftMargin;
        m_minWidth = m_maxWidth = maxWidth;
    }

#if 0
    // FIXME: should this be OSX-only?
    {
        bool wantNativeToolbar, ownToolbarInstalled;

        // attempt to install the native toolbar
        wantNativeToolbar = ((GetWindowStyleFlag() & (wxTB_LEFT|wxTB_BOTTOM|wxTB_RIGHT)) == 0);
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

    SetInitialSize();

    return true;
}

void wxToolBar::SetToolBitmapSize(const wxSize& size)
{
    m_defaultWidth = size.x + kwxMacToolBorder;
    m_defaultHeight = size.y + kwxMacToolBorder;

#if wxOSX_USE_NATIVE_TOOLBAR
    if (m_macToolbar != NULL)
    {
        int maxs = wxMax( size.x, size.y );
        NSToolbarSizeMode sizeSpec;
        if ( maxs > 32 )
            sizeSpec = NSToolbarSizeModeRegular;
        else if ( maxs > 24 )
            sizeSpec = NSToolbarSizeModeDefault;
        else
            sizeSpec = NSToolbarSizeModeSmall;

        [(NSToolbar*) m_macToolbar setSizeMode:sizeSpec ];
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

#if wxOSX_USE_NATIVE_TOOLBAR
    if (! m_macUsesNativeToolbar )
        Realize();
#else

    Realize();
#endif
}

void wxToolBar::SetToolNormalBitmap( int id, const wxBitmap& bitmap )
{
    wxToolBarTool* tool = static_cast<wxToolBarTool*>(FindById(id));
    if ( tool )
    {
        wxCHECK_RET( tool->IsButton(), wxT("Can only set bitmap on button tools."));

        tool->SetNormalBitmap(bitmap);

        // a side-effect of the UpdateToggleImage function is that it always changes the bitmap used on the button.
        tool->UpdateImages();
    }
}

void wxToolBar::SetToolDisabledBitmap( int id, const wxBitmap& bitmap )
{
    wxToolBarTool* tool = static_cast<wxToolBarTool*>(FindById(id));
    if ( tool )
    {
        wxCHECK_RET( tool->IsButton(), wxT("Can only set bitmap on button tools."));

        tool->SetDisabledBitmap(bitmap);

        // TODO:  what to do for this one?
    }
}

wxToolBarToolBase *wxToolBar::FindToolForPosition(wxCoord x, wxCoord y) const
{
    wxToolBarTool *tool;
    wxToolBarToolsList::compatibility_iterator node = m_tools.GetFirst();
    while ( node )
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

    return NULL;
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
    wxToolBarTool *tool = static_cast< wxToolBarTool*>(toolBase );
    if (tool == NULL)
        return false;

    wxSize toolSize = GetToolSize();
    WXWidget controlHandle = NULL;
    NSRect toolrect = NSMakeRect(0, 0, toolSize.x, toolSize.y );

#if wxOSX_USE_NATIVE_TOOLBAR
    wxString label = tool->GetLabel();
    if (m_macToolbar && !label.empty() )
    {
        // strip mnemonics from the label for compatibility
        // with the usual labels in wxStaticText sense
        label = wxStripMenuCodes(label);
    }
#endif // wxOSX_USE_NATIVE_TOOLBAR

    switch (tool->GetStyle())
    {
        case wxTOOL_STYLE_SEPARATOR:
            {
                wxASSERT( tool->GetControlHandle() == NULL );
                toolSize.x /= 4;
                toolSize.y /= 4;
                if ( GetWindowStyleFlag() & (wxTB_LEFT|wxTB_RIGHT) )
                    toolrect.size.height = toolSize.y;
                else
                    toolrect.size.width = toolSize.x;

                // in flat style we need a visual separator
#if wxOSX_USE_NATIVE_TOOLBAR
                if (m_macToolbar != NULL)
                {
                    NSToolbarItem* item = [[NSToolbarItem alloc] initWithItemIdentifier:NSToolbarSeparatorItemIdentifier];
                    tool->SetToolbarItemRef( item );
                }
#endif // wxOSX_USE_NATIVE_TOOLBAR

                NSBox* box = [[NSBox alloc] initWithFrame:toolrect];
                [box setBoxType:NSBoxSeparator];
                controlHandle = box;
                tool->SetControlHandle( controlHandle );
            }
            break;

        case wxTOOL_STYLE_BUTTON:
            {
                wxASSERT( tool->GetControlHandle() == NULL );

                wxNSToolBarButton* v = [[wxNSToolBarButton alloc] initWithFrame:toolrect];

                [v setBezelStyle:NSRegularSquareBezelStyle];
                [v setBordered:NO];
                [v setButtonType: ( tool->CanBeToggled() ? NSOnOffButton : NSMomentaryPushInButton )];
                [v setImplementation:tool];
                
                controlHandle = v;
                
#if wxOSX_USE_NATIVE_TOOLBAR
                if (m_macToolbar != NULL)
                {
                    wxString identifier = wxString::Format(wxT("%ld"), (long) tool);
                    wxCFStringRef cfidentifier( identifier, wxFont::GetDefaultEncoding() );
                    wxNSToolbarItem* item = [[wxNSToolbarItem alloc] initWithItemIdentifier:cfidentifier.AsNSString() ];
                    [item setImplementation:tool];
                    tool->SetToolbarItemRef( item );
                }

#endif // wxOSX_USE_NATIVE_TOOLBAR
                tool->SetControlHandle( controlHandle );
                tool->UpdateImages();
                tool->UpdateLabel();
#if 0
                SetBevelButtonTextPlacement( m_controlHandle, kControlBevelButtonPlaceBelowGraphic );
                SetControlTitleWithCFString( m_controlHandle , wxCFStringRef( label, wxFont::GetDefaultEncoding() );
#endif
#if 0
                InstallControlEventHandler(
                    (WXWidget) controlHandle, GetwxMacToolBarToolEventHandlerUPP(),
                    GetEventTypeCount(eventList), eventList, tool, NULL );
#endif
            }
            break;

        case wxTOOL_STYLE_CONTROL:

#if wxOSX_USE_NATIVE_TOOLBAR
            if (m_macToolbar != NULL)
            {
                WXWidget view = (WXWidget) tool->GetControl()->GetHandle() ;
                wxCHECK_MSG( view, false, _T("control must be non-NULL") );

                wxString identifier = wxString::Format(wxT("%ld"), (long) tool);
                wxCFStringRef cfidentifier( identifier, wxFont::GetDefaultEncoding() );
                wxNSToolbarItem* item = [[wxNSToolbarItem alloc] initWithItemIdentifier:cfidentifier.AsNSString() ];
                [item setImplementation:tool];
                tool->SetToolbarItemRef( item );
           }
#else
            // right now there's nothing to do here
#endif
            tool->UpdateLabel();
            break;

        default:
            break;
    }

    if ( controlHandle )
    {
        WXWidget container = (WXWidget) GetHandle();
        wxASSERT_MSG( container != NULL, wxT("No valid Mac container control") );

//        SetControlVisibility( controlHandle, true, true );
        [container addSubview:controlHandle];
    }

    // nothing special to do here - we relayout in Realize() later
    InvalidateBestSize();

    return true;

}

void wxToolBar::DoSetToggle(wxToolBarToolBase *WXUNUSED(tool), bool WXUNUSED(toggle))
{
    wxFAIL_MSG( wxT("not implemented") );
}

bool wxToolBar::DoDeleteTool(size_t WXUNUSED(pos), wxToolBarToolBase *toolbase)
{
    wxToolBarTool* tool = static_cast< wxToolBarTool*>(toolbase );
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

#if wxOSX_USE_NATIVE_TOOLBAR
    CFIndex removeIndex = tool->GetIndex();
#endif

#if wxOSX_USE_NATIVE_TOOLBAR
    if (m_macToolbar != NULL)
    {
        if ( removeIndex != -1 && m_macToolbar )
        {
            [(NSToolbar*) m_macToolbar removeItemAtIndex:removeIndex];
            tool->SetIndex( -1 );
        }
    }
#endif

    tool->ClearControl();

    // and finally reposition all the controls after this one

    for ( /* node -> first after deleted */; node; node = node->GetNext() )
    {
        wxToolBarTool *tool2 = (wxToolBarTool*) node->GetData();
        wxPoint pt = tool2->GetPosition();

        if ( GetWindowStyleFlag() & (wxTB_LEFT|wxTB_RIGHT) )
            pt.y -= sz.y;
        else
            pt.x -= sz.x;

        tool2->SetPosition( pt );

#if wxOSX_USE_NATIVE_TOOLBAR
        if (m_macToolbar != NULL)
        {
            if ( removeIndex != -1 && tool2->GetIndex() > removeIndex )
                tool2->SetIndex( tool2->GetIndex() - 1 );
        }
#endif
    }

    InvalidateBestSize();

    return true;
}

#include <Carbon/Carbon.h>

void wxToolBar::OnPaint(wxPaintEvent& event)
{
#if wxOSX_USE_NATIVE_TOOLBAR
    if ( m_macUsesNativeToolbar )
    {
        event.Skip(true);
        return;
    }
#endif

    wxPaintDC dc(this);

    int w, h;
    GetSize( &w, &h );

    bool drawMetalTheme = MacGetTopLevelWindow()->GetExtraStyle() & wxFRAME_EX_METAL;

    if ( !drawMetalTheme  )
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

    event.Skip();
}

#endif // wxUSE_TOOLBAR
