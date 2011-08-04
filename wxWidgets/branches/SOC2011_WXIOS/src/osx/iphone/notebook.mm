/////////////////////////////////////////////////////////////////////////////
// Name:        src/osx/iphone/notebook.mm
// Purpose:     wxNotebook
// Author:      Linas Valiukas
// Modified by:
// Created:     2011-06-27
// RCS-ID:      $Id$
// Copyright:   (c) Linas Valiukas
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#if wxUSE_NOTEBOOK

#include "wx/notebook.h"

#ifndef WX_PRECOMP
#include "wx/string.h"
#include "wx/log.h"
#include "wx/app.h"
#include "wx/image.h"
#endif // WX_PRECOMP

#include "wx/string.h"
#include "wx/imaglist.h"
#include "wx/osx/private.h"
#include "wx/osx/iphone/private/notebookimpl.h"

#include <stdio.h>


#pragma mark -
#pragma mark Cocoa class

@implementation wxUITabBarController

- (BOOL)tabBarController:(UITabBarController *)tabBarController shouldSelectViewController:(UIViewController *)viewController {
    return YES;
}

- (void)tabBarController:(UITabBarController *)tabBarController didSelectViewController:(UIViewController *)viewController {
    wxUnusedVar(viewController);
    
    wxWidgetIPhoneImpl* viewimpl = (wxWidgetIPhoneImpl* ) wxWidgetImpl::FindFromWXWidget( tabBarController.view );
    if ( viewimpl )
    {
        wxNotebook* wxpeer = (wxNotebook*) viewimpl->GetWXPeer();
        wxpeer->OSXHandleClicked(0);
    }
    
#ifdef __WXDEBUG__
    NSLog(@"tab changed:");
    NSLog(@"%@", (NSString *)[viewController.view performSelector:@selector(recursiveDescription)]);
#endif

}

@end


#pragma mark -
#pragma mark Peer implementation

wxNotebookIPhoneImpl::wxNotebookIPhoneImpl( wxWindowMac* peer , wxUITabBarController *tabBarController, WXWidget w ) : wxWidgetIPhoneImpl(peer, w)
{
    m_tabBarController = tabBarController;
}
    
// Ignore attempts to set position/size
void wxNotebookIPhoneImpl::Move(int x, int y, int width, int height)
{
    
}

// Don't know nothing about labels too
void wxNotebookIPhoneImpl::SetLabel(const wxString& title, wxFontEncoding encoding)
{
    
}

void wxNotebookIPhoneImpl::GetContentArea( int &left , int &top , int &width , int &height ) const
{
    CGRect r = m_osxView.bounds;
    left = (int)r.origin.x;
    top = (int)r.origin.y;
    width = (int)r.size.width;
    height = (int)r.size.height;
}

void wxNotebookIPhoneImpl::SetValue( wxInt32 value )
{
    // avoid 'changed' events when setting the tab programmatically
    [m_tabBarController setDelegate:nil];
    [m_tabBarController setSelectedIndex:(value-1)];
    [m_tabBarController setDelegate:m_tabBarController];
}

wxInt32 wxNotebookIPhoneImpl::GetValue() const
{
    return [m_tabBarController selectedIndex]+1;
}

void wxNotebookIPhoneImpl::SetMaximum( wxInt32 maximum )
{
    int tabCount = [[m_tabBarController viewControllers] count];
    
    if (maximum != tabCount) {
    
        NSMutableArray *controllers = [NSMutableArray arrayWithArray:[m_tabBarController viewControllers]];
        
        // avoid 'changed' events when setting the tab programmatically
        [m_tabBarController setDelegate:nil];
        
        if ( maximum > tabCount )
        {
            for ( int i = tabCount ; i < maximum ; ++i )
            {
                UIViewController *viewController = [[UIViewController alloc] init];
                [viewController setTitle:@"-"];
                [controllers addObject:viewController];
                [viewController release];
            }
        }
        else if ( maximum < tabCount )
        {
            NSRange removeRange;
            removeRange.location = maximum;
            removeRange.length = tabCount-maximum;
            [controllers removeObjectsInRange:removeRange];
        }
        
        [m_tabBarController setViewControllers:controllers];
        [m_tabBarController setDelegate:m_tabBarController];
    }
}

// Set a text badge for the given item
bool wxNotebookIPhoneImpl::SetBadge(int item, const wxString& badge)
{
    if ( badge.empty() ) {
        return false;
    }
    
    if ( [[m_tabBarController viewControllers] count] < (unsigned int)(item+1) ) {
        return false;
    }
    
    UIViewController *viewController = [[m_tabBarController viewControllers] objectAtIndex:item];
    UITabBarItem *tabBarItem = [viewController tabBarItem];
    
    if ( !tabBarItem ) {
        return false;
    }
    
    wxCFStringRef cf( badge );
    @try {
        [tabBarItem setBadgeValue:cf.AsNSString()];
    }
    @catch (NSException *e) {
        // FIXME raises exception:
        // 'failed setting a badge: CALayerInvalidGeometry CALayer position contains NaN: [20 nan]'
        NSLog(@"failed setting a badge: %@ %@", [e name], [e reason]);
        wxASSERT_MSG(0, "failed setting a badge");
        return false;
    }    
    
    return true;
}

// Get the text badge for the given item
wxString wxNotebookIPhoneImpl::GetBadge(int item) const
{
    if ( [[m_tabBarController viewControllers] count] < (unsigned int)(item+1) ) {
        return wxEmptyString;
    }
    
    UIViewController *viewController = [[m_tabBarController viewControllers] objectAtIndex:item];
    UITabBarItem *tabBarItem = [viewController tabBarItem];
    
    if ( !tabBarItem ) {
        return wxEmptyString;
    }
    
    NSString *badgeValue = [tabBarItem badgeValue];
    if ( !badgeValue || [badgeValue isEqualToString:@""] ) {
        return wxEmptyString;
    }
    
    return wxString([badgeValue cStringUsingEncoding:[NSString defaultCStringEncoding]]);
}

wxUITabBarController* wxNotebookIPhoneImpl::GetTabBarController()
{
    return m_tabBarController;
}


void wxNotebookIPhoneImpl::SetupTabs( const wxNotebook& notebook )
{
    int pageCount = notebook.GetPageCount();
    
    SetMaximum( pageCount );
    
    // avoid 'changed' events when setting the tab programmatically
    [m_tabBarController setDelegate:nil];
    
    for ( int i = 0 ; i < pageCount ; ++i )
    {
        wxNotebookPage* page = notebook.GetPage(i);
        UIViewController *controller = [[m_tabBarController viewControllers] objectAtIndex:i];
        
        wxCFStringRef cf( page->GetLabel() , notebook.GetFont().GetEncoding() );
        NSString *tabTitle = cf.AsNSString();
        int pageImage = notebook.GetPageImage(i);
        
        if (pageImage != -1) {  // System item
            UITabBarSystemItem systemItem = UITabBarSystemItemHistory;
            
            switch (pageImage) {
                case wxID_FAVORITES:    systemItem = UITabBarSystemItemFavorites;   break;
                case wxID_FEATURED:     systemItem = UITabBarSystemItemFeatured;    break;
                case wxID_TOPRATED:     systemItem = UITabBarSystemItemTopRated;    break;
                case wxID_RECENTS:      systemItem = UITabBarSystemItemRecents;     break;
                case wxID_CONTACTS:     systemItem = UITabBarSystemItemContacts;    break;
                case wxID_HISTORY:      systemItem = UITabBarSystemItemHistory;     break;
                case wxID_BOOKMARKS:    systemItem = UITabBarSystemItemBookmarks;   break;
                case wxID_SEARCH:       systemItem = UITabBarSystemItemSearch;      break;
                case wxID_DOWNLOADS:    systemItem = UITabBarSystemItemDownloads;   break;
                case wxID_MOSTRECENT:   systemItem = UITabBarSystemItemMostRecent;  break;
                case wxID_MOSTVIEWED:   systemItem = UITabBarSystemItemMostViewed;  break;
                case wxID_MORE:         systemItem = UITabBarSystemItemMore;        break;
                default:                systemItem = UITabBarSystemItemHistory;     break;
            };
            
            controller.tabBarItem = [[UITabBarItem alloc] initWithTabBarSystemItem:systemItem
                                                                               tag:i];
        } else {
            controller.tabBarItem = [[UITabBarItem alloc] initWithTitle:tabTitle
                                                                  image:nil // FIXME
                                                                    tag:i];
        }
        
        [controller setView:page->GetPeer()->GetWXWidget()];
        [controller setTitle:tabTitle];
    }
    
    [m_tabBarController setDelegate:m_tabBarController];
}



wxWidgetImplType* wxWidgetImpl::CreateTabView(wxWindowMac* wxpeer,
                                              wxWindowMac* parent,
                                              wxWindowID WXUNUSED(id),
                                              const wxPoint& pos,
                                              const wxSize& size,
                                              long style,
                                              long WXUNUSED(extraStyle))
{
    wxUITabBarController* controller = [[wxUITabBarController alloc] init];
    [controller setDelegate:controller];

    // Adjust to parent's size
    if (parent) {
        UIView *parentView = parent->GetPeer()->GetWXWidget();
        if (parentView) {
            CGRect parentViewFrame = parentView.frame;
            parentViewFrame.origin.x = 0;
            parentViewFrame.origin.y = 0;
            [controller.view setFrame:parentViewFrame];
        }        
    }
            
    wxWidgetIPhoneImpl* c = new wxNotebookIPhoneImpl( wxpeer, controller, controller.view );
    return c;
}

#pragma mark -
#pragma mark wxNotebook implementation bits

bool wxNotebook::SetBadge(int item, const wxString& badge)
{
    wxNotebookIPhoneImpl *peer = (wxNotebookIPhoneImpl *)GetPeer();
    peer->SetBadge(item, badge);
}

// Gets the UITabBar text badge for the given item
wxString wxNotebook::GetBadge(int item) const
{
    wxNotebookIPhoneImpl *peer = (wxNotebookIPhoneImpl *)GetPeer();
    return peer->GetBadge(item);
}

#endif //if wxUSE_NOTEBOOK
