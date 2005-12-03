///////////////////////////////////////////////////////////////////////////////
// Name:        taskbar.cpp
// Purpose:     wxTaskBarIcon OSX Implementation
// Author:      Ryan Norton
// Modified by:
// Created:     09/25/2004
// RCS-ID:      $Id$
// Copyright:   (c) 2004 Ryan Norton
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

//=============================================================================
// Declarations
//=============================================================================

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "wx/wxprec.h"

#ifdef wxHAS_TASK_BAR_ICON

#include "wx/mac/private.h"

#include "wx/taskbar.h"
#include "wx/menu.h"
#include "wx/icon.h"
#include "wx/dcmemory.h"

//-----------------------------------------------------------------------------
//
//  wxTaskBarIconImpl
//
//  Superclass of wxTaskBarIcon implementations
//-----------------------------------------------------------------------------

class wxTaskBarIconImpl 
{
public:
    wxTaskBarIconImpl(wxTaskBarIcon* parent);
    virtual ~wxTaskBarIconImpl();

    virtual bool IsIconInstalled() const = 0;
    virtual bool SetIcon(const wxIcon& icon, const wxString& tooltip) = 0;
    virtual bool RemoveIcon() = 0;
    virtual bool PopupMenu(wxMenu *menu) = 0;
    
    wxMenu* CreatePopupMenu()
    { return m_parent->CreatePopupMenu(); }
    
    wxTaskBarIcon*       m_parent;
    class wxTaskBarIconWindow* m_menuEventWindow;
};

//-----------------------------------------------------------------------------
//
//  wxTaskBarIconWindow
//
//  Event handler for menus
//  NB: Since wxWindows in mac HAVE to have parents we need this to be
//  a top level window...
//-----------------------------------------------------------------------------

class wxTaskBarIconWindow : public wxTopLevelWindow
{
public:
    wxTaskBarIconWindow(wxTaskBarIconImpl* impl) 
        : wxTopLevelWindow(NULL, -1, wxT("")), m_impl(impl) 
    {
        Connect(-1, wxEVT_COMMAND_MENU_SELECTED, 
                wxCommandEventHandler(wxTaskBarIconWindow::OnMenuEvent)
               );
    }

    void OnMenuEvent(wxCommandEvent& event) 
    {
        m_impl->m_parent->ProcessEvent(event);
    }

private:
    wxTaskBarIconImpl* m_impl;
};

//-----------------------------------------------------------------------------
//
//  wxDockBarIconImpl
//
//-----------------------------------------------------------------------------

class wxDockTaskBarIcon : public wxTaskBarIconImpl
{
public:
    wxDockTaskBarIcon(wxTaskBarIcon* parent);
    virtual ~wxDockTaskBarIcon();
    
    virtual bool IsIconInstalled() const;
    virtual bool SetIcon(const wxIcon& icon, const wxString& tooltip);
    virtual bool RemoveIcon();
    virtual bool PopupMenu(wxMenu *menu);

    wxMenu* DoCreatePopupMenu();

    EventHandlerRef     m_eventHandlerRef;
    EventHandlerUPP     m_eventupp;
    wxMenu*             m_pMenu;
    MenuRef             m_theLastMenu;
    bool                m_iconAdded;
    wxWindow*           m_eventWindow;
};

// Forward declarations for utility functions for dock implementation
pascal OSStatus wxDockEventHandler( EventHandlerCallRef inHandlerCallRef,
                                    EventRef inEvent, void* pData);
wxMenu* wxDeepCopyMenu(wxMenu* menu);
                                    

//=============================================================================
//
// Implementation
//
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//  wxTaskBarIconImpl
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//-----------------------------------------------------------------------------
// wxTaskBarIconImpl Constructor
//
// Initializes members and creates the event window
//-----------------------------------------------------------------------------
wxTaskBarIconImpl::wxTaskBarIconImpl(wxTaskBarIcon* parent) 
    : m_parent(parent), m_menuEventWindow(new wxTaskBarIconWindow(this))
{
}

//-----------------------------------------------------------------------------
// wxTaskBarIconImpl Destructor
//
// Cleans up the event window
//-----------------------------------------------------------------------------
wxTaskBarIconImpl::~wxTaskBarIconImpl()
{
    delete m_menuEventWindow;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//  wxDockTaskBarIcon
//
//  OS X DOCK implementation of wxTaskBarIcon using carbon
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// wxDockEventHandler
//
// This is the global mac/carbon event handler for the dock.
// We need this for two reasons:
// 1) To handle wxTaskBarIcon menu events (see below for why)
// 2) To handle events from the dock when it requests a menu
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
pascal OSStatus wxDockEventHandler( EventHandlerCallRef inHandlerCallRef,
                                    EventRef inEvent, void* pData)
{
    // Get the parameters we want from the event
    wxDockTaskBarIcon* pTB = (wxDockTaskBarIcon*) pData;
    const UInt32 eventClass = GetEventClass(inEvent);
    const UInt32 eventKind = GetEventKind(inEvent);
            
    //
    // Handle wxTaskBar menu events (note that this is a global event handler
    // so it will actually get called by all commands/menus)
    //  
    if (eventClass == kEventClassCommand && eventKind == kEventCommandProcess) 
    {
        // if we have no taskbar menu quickly pass it back to wxApp
        if (! pTB->m_pMenu )
        {
            return eventNotHandledErr;
        }
                
        //
        // This is the real reason why we need this. Normally menus
        // get handled in wxMacAppEventHandler
        //
        // pascal OSStatus wxMacAppEventHandler(EventHandlerCallRef handler, 
        //                                      EventRef event, void *data)
        //
        // However, in the case of a taskbar menu call 
        // command.menu.menuRef IS NULL!
        // Which causes the wxApp handler just to skip it.
        //
        MenuRef taskbarMenuRef = MAC_WXHMENU(pTB->m_pMenu->GetHMenu());
        OSStatus result = eventNotHandledErr;
        OSErr err;
    
        // get the HICommand from the event
        HICommand command;
        err = GetEventParameter(inEvent, kEventParamDirectObject,
                                typeHICommand, NULL, 
                                sizeof(HICommand), NULL, &command);
        if (err == noErr)
        {       
            //
            // Obtain the REAL menuRef and the menuItemIndex in the real menuRef
            //
            // NOTE: menuRef is generally used here for submenus, as 
            // GetMenuItemRefCon could give an incorrect wxMenuItem if we pass 
            // just the top level wxTaskBar menu
            //
            MenuItemIndex menuItemIndex;
            MenuRef menuRef;

            err = GetIndMenuItemWithCommandID(taskbarMenuRef, 
                                              command.commandID, 
                                              1, &menuRef, &menuItemIndex);
            if (err == noErr)
            {
                MenuCommand id = command.commandID;
                wxMenuItem* item = NULL;
        
                if (id != 0) // get the wxMenuItem reference from the MenuRef
                    GetMenuItemRefCon(menuRef, menuItemIndex, (UInt32*) &item);
            
                if (item)
                {
                    // Handle items that are checkable
                    // FIXME: Doesn't work (at least on 10.2)!
                    if (item->IsCheckable())
                        item->Check( !item->IsChecked() ) ;

                    // send the wxEvent to the wxMenu
                    item->GetMenu()->SendEvent(id, 
                               item->IsCheckable() ? 
                               item->IsChecked() : -1 
                               );
                    err = noErr; // successfully handled the event
                }
            }
        } //end if noErr on getting HICommand from event        

        return err; // return whether we handled the event or not
    }

    // We better have a kEventClassApplication/kEventAppGetDockTileMenu combo here,
    // otherwise something is truly funky     
    wxASSERT(eventClass == kEventClassApplication && 
             eventKind == kEventAppGetDockTileMenu);

    // process the right click events
    // NB: This may result in double or even triple-creation of the menus
    // We need to do this for 2.4 compat, however
    wxTaskBarIconEvent downevt(wxEVT_TASKBAR_RIGHT_DOWN,NULL);
    pTB->m_parent->ProcessEvent(downevt);

    wxTaskBarIconEvent upevt(wxEVT_TASKBAR_RIGHT_UP,NULL);
    pTB->m_parent->ProcessEvent(upevt);

    //create popup menu
    wxMenu* menu = pTB->DoCreatePopupMenu();
    
    OSStatus err = noErr;

    if (menu)
    {
        //note to self - a MenuRef IS A MenuHandle
        MenuRef hMenu = MAC_WXHMENU(menu->GetHMenu());

        // When we call SetEventParameter it will decrement
        // the reference count of the menu - we need to make
        // sure it stays around in the wxMenu class here
        RetainMenu(hMenu);

        // set the actual dock menu
        err = SetEventParameter(inEvent, kEventParamMenuRef, 
                    typeMenuRef, sizeof(MenuRef), &hMenu);
        wxASSERT(err == noErr);

        return err;
    }
    else
        return eventNotHandledErr;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// wxDeepCopyMenu
//
// Performs a top-to-bottom copy of the input menu and all of its
// submenus.
//
// This is mostly needed for 2.4 compatability. However wxPython and others
// still use this way of setting the taskbarmenu.
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
wxMenu* wxDeepCopyMenu(wxMenu* menu)
{
    if (!menu)
        return NULL;

    //
    // NB:  Here we have to perform a deep copy of the menu,
    // copying each and every menu item from menu to m_pMenu.
    // Other implementations use wxWindow::PopupMenu here, 
    // which idle execution until the user selects something,
    // but since the mac handles this internally, we can't - 
    // and have no way at all to idle it while the dock menu
    // is being shown before menu goes out of scope (it may
    // not be on the heap, and may expire right after this function
    // is done - we need it to last until the carbon event is triggered - 
    // that's when the user right clicks).
    //
    // Also, since there is no equal (assignment) operator 
    // on either wxMenu or wxMenuItem, we have to do all the
    // dirty work ourselves.
    //

    // perform a deep copy of the menu
    wxMenuItemList& theList = menu->GetMenuItems();
    wxMenuItemList::compatibility_iterator theNode = theList.GetFirst();
    
    // create the main menu
    wxMenu* m_pMenu = new wxMenu(menu->GetTitle());

    while (theNode != NULL)
    {
        wxMenuItem* theItem = theNode->GetData();
        m_pMenu->Append(new wxMenuItem(m_pMenu, // parent menu
                                        theItem->GetId(), // id
                                        theItem->GetText(), // text label
                                        theItem->GetHelp(), // status bar help string
                                        theItem->GetKind(), // menu flags - checkable, separator, etc.
                                        wxDeepCopyMenu(theItem->GetSubMenu()) // submenu
                                        ));
        theNode = theNode->GetNext();
    }
    
    return m_pMenu;
}

//-----------------------------------------------------------------------------
// wxDockTaskBarIcon Constructor
//
// Initializes the dock implementation of wxTaskBarIcon.
//
// Here we create some mac-specific event handlers and UPPs.
//-----------------------------------------------------------------------------
wxDockTaskBarIcon::wxDockTaskBarIcon(wxTaskBarIcon* parent) 
    :   wxTaskBarIconImpl(parent),
        m_eventHandlerRef(NULL), m_pMenu(NULL), 
        m_theLastMenu(GetApplicationDockTileMenu()), m_iconAdded(false) 
{
    // register the events that will return the dock menu
    EventTypeSpec tbEventList[] = { { kEventClassCommand, kEventProcessCommand },
                                    { kEventClassApplication, kEventAppGetDockTileMenu } };
    
    m_eventupp = NewEventHandlerUPP(wxDockEventHandler);
    wxASSERT(m_eventupp != NULL);
                                              
#ifdef __WXDEBUG__
    OSStatus err =
#endif
    InstallApplicationEventHandler(
            m_eventupp,
            GetEventTypeCount(tbEventList), tbEventList, 
            this, &m_eventHandlerRef);
    wxASSERT(err == noErr);
}

//-----------------------------------------------------------------------------
// wxDockTaskBarIcon Destructor
//
// Cleans up mac events and restores the old icon to the dock
//-----------------------------------------------------------------------------
wxDockTaskBarIcon::~wxDockTaskBarIcon()
{
    // clean up event handler and event UPP
    RemoveEventHandler(m_eventHandlerRef);
    DisposeEventHandlerUPP(m_eventupp);

    // restore old icon and menu to the dock
    RemoveIcon();    
}

//-----------------------------------------------------------------------------
// wxDockTaskBarIcon::DoCreatePopupMenu
//
// Helper function that handles a request from the dock event handler 
// to get the menu for the dock
//-----------------------------------------------------------------------------
wxMenu* wxDockTaskBarIcon::DoCreatePopupMenu()
{
    // get the menu from the parent
    wxMenu* theNewMenu = CreatePopupMenu();
    
    if (theNewMenu)
    {
        if (m_pMenu)
            delete m_pMenu;
        m_pMenu = theNewMenu;
        m_pMenu->SetInvokingWindow(m_menuEventWindow);
    }
    
    // the return here can be one of three things 
    // (in order of priority):
    // 1) User passed a menu from CreatePopupMenu override
    // 2) menu sent to and copied from PopupMenu
    // 3) If neither (1) or (2), then NULL
    //
    return m_pMenu;
}

//-----------------------------------------------------------------------------
// wxDockTaskBarIcon::IsIconInstalled
//
// Returns whether or not the dock is not using the default image
//-----------------------------------------------------------------------------
bool wxDockTaskBarIcon::IsIconInstalled() const
{   
    return m_iconAdded; 
}

//-----------------------------------------------------------------------------
// wxDockTaskBarIcon::SetIcon
//
// Sets the icon for the dock CGImage functions and SetApplicationDockTileImage
//-----------------------------------------------------------------------------
bool wxDockTaskBarIcon::SetIcon(const wxIcon& icon, const wxString& tooltip)
{
    wxBitmap bmp( icon ) ;
    OSStatus err = noErr ;

    CGImageRef pImage;
    
#if 0 // is always available under OSX now -- crashes on 10.2 in CFRetain() - RN
    pImage = (CGImageRef) bmp.CGImageCreate() ;
#else
    WXHBITMAP iconport ;
    WXHBITMAP maskport ;
    iconport = bmp.GetHBITMAP( &maskport ) ;

    if (!maskport)
    {
        // Make a mask with no transparent pixels
        wxBitmap   mbmp(icon.GetWidth(), icon.GetHeight());
        wxMemoryDC dc;
        dc.SelectObject(mbmp);
        dc.SetBackground(*wxBLACK_BRUSH);
        dc.Clear();
        dc.SelectObject(wxNullBitmap);
        bmp.SetMask( new wxMask(mbmp, *wxWHITE) ) ;
        iconport = bmp.GetHBITMAP( &maskport ) ;
    } 
    
    //create the icon from the bitmap and mask bitmap contained within
    err = CreateCGImageFromPixMaps(
                                            GetGWorldPixMap(MAC_WXHBITMAP(iconport)),
                                            GetGWorldPixMap(MAC_WXHBITMAP(maskport)),
                                            &pImage
                                            );    
    wxASSERT(err == 0);
#endif

    wxASSERT(pImage != NULL);
    err = SetApplicationDockTileImage(pImage);
    
    wxASSERT(err == 0);
    
    if (pImage != NULL)
        CGImageRelease(pImage);
    
    return m_iconAdded = err == noErr;
}
    
//-----------------------------------------------------------------------------
// wxDockTaskBarIcon::RemoveIcon
//
// Restores the old image for the dock via RestoreApplicationDockTileImage
//-----------------------------------------------------------------------------
bool wxDockTaskBarIcon::RemoveIcon()
{
    if (m_pMenu)
    {
        delete m_pMenu;
        m_pMenu = NULL;
    }
    
    // restore old icon to the dock
    OSStatus err = RestoreApplicationDockTileImage();
    wxASSERT(err == noErr);
    
    // restore the old menu to the dock
    SetApplicationDockTileMenu(m_theLastMenu);

    return !(m_iconAdded = !(err == noErr));
}
    
//-----------------------------------------------------------------------------
// wxDockTaskBarIcon::PopupMenu
//
// 2.4 and wxPython method that "pops of the menu in the taskbar".
//
// In reality because of the way the dock menu works in carbon 
// we just save the menu, and if the user didn't override CreatePopupMenu
// return the menu passed here, thus sort of getting the same effect.
//-----------------------------------------------------------------------------
bool wxDockTaskBarIcon::PopupMenu(wxMenu *menu)
{
    wxASSERT(menu != NULL);

    if (m_pMenu)
        delete m_pMenu;
    
    //start copy of menu
    m_pMenu = wxDeepCopyMenu(menu);
   
    //finish up
    m_pMenu->SetInvokingWindow(m_menuEventWindow);

    return true;
}
    
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//  wxTaskBarIcon
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
IMPLEMENT_DYNAMIC_CLASS(wxTaskBarIcon, wxEvtHandler)
    
//-----------------------------------------------------------------------------
// wxTaskBarIcon Constructor
//
// Creates the backend
//
// Note that we only support DOCK currently as others require cocoa and
// also some require hacks and other such things. (MenuExtras are
// actually seperate programs that also require a special undocumented id 
// hack and other such fun stuff).
//-----------------------------------------------------------------------------
wxTaskBarIcon::wxTaskBarIcon(wxTaskBarIconType nType)
{
    wxASSERT_MSG(nType == DOCK, 
                       wxT("Only the DOCK implementation of wxTaskBarIcon")
                       wxT("on mac carbon is currently supported!"));
    m_impl = new wxDockTaskBarIcon(this);
}

//-----------------------------------------------------------------------------
// wxTaskBarIcon Destructor
//
// Destroys the backend
//-----------------------------------------------------------------------------
wxTaskBarIcon::~wxTaskBarIcon()
{
    delete m_impl;
}

//-----------------------------------------------------------------------------
// wxTaskBarIcon::SetIcon
// wxTaskBarIcon::RemoveIcon
// wxTaskBarIcon::PopupMenu
//
// Just calls the backend version of the said function.
//-----------------------------------------------------------------------------
bool wxTaskBarIcon::IsIconInstalled() const
{ return m_impl->IsIconInstalled(); }
bool wxTaskBarIcon::SetIcon(const wxIcon& icon, const wxString& tooltip)
{ return m_impl->SetIcon(icon, tooltip); }
bool wxTaskBarIcon::RemoveIcon()
{ return m_impl->RemoveIcon(); }    
bool wxTaskBarIcon::PopupMenu(wxMenu *menu)
{ return m_impl->PopupMenu(menu); }

#endif //wxHAS_TASK_BAR_ICON
