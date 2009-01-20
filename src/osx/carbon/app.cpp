/////////////////////////////////////////////////////////////////////////////
// Name:        src/osx/carbon/app.cpp
// Purpose:     wxApp
// Author:      Stefan Csomor
// Modified by:
// Created:     1998-01-01
// RCS-ID:      $Id$
// Copyright:   (c) Stefan Csomor
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#include "wx/app.h"

#ifndef WX_PRECOMP
    #include "wx/intl.h"
    #include "wx/log.h"
    #include "wx/utils.h"
    #include "wx/window.h"
    #include "wx/frame.h"
    #include "wx/dc.h"
    #include "wx/button.h"
    #include "wx/menu.h"
    #include "wx/pen.h"
    #include "wx/brush.h"
    #include "wx/palette.h"
    #include "wx/icon.h"
    #include "wx/cursor.h"
    #include "wx/dialog.h"
    #include "wx/msgdlg.h"
    #include "wx/textctrl.h"
    #include "wx/memory.h"
    #include "wx/gdicmn.h"
    #include "wx/module.h"
#endif

#include "wx/tooltip.h"
#include "wx/docview.h"
#include "wx/filename.h"
#include "wx/link.h"
#include "wx/thread.h"
#include "wx/evtloop.h"

#include <string.h>

// mac
#if wxOSX_USE_CARBON
#include "wx/osx/uma.h"
#else
#include "wx/osx/private.h"
#endif

#if defined(WXMAKINGDLL_CORE)
#   include <mach-o/dyld.h>
#endif

// Keep linker from discarding wxStockGDIMac
wxFORCE_LINK_MODULE(gdiobj)

IMPLEMENT_DYNAMIC_CLASS(wxApp, wxEvtHandler)
BEGIN_EVENT_TABLE(wxApp, wxEvtHandler)
    EVT_IDLE(wxApp::OnIdle)
    EVT_END_SESSION(wxApp::OnEndSession)
    EVT_QUERY_END_SESSION(wxApp::OnQueryEndSession)
END_EVENT_TABLE()


// platform specific static variables
static const short kwxMacAppleMenuId = 1 ;

wxWindow* wxApp::s_captureWindow = NULL ;
long      wxApp::s_lastModifiers = 0 ;

long      wxApp::s_macAboutMenuItemId = wxID_ABOUT ;
long      wxApp::s_macPreferencesMenuItemId = wxID_PREFERENCES ;
long      wxApp::s_macExitMenuItemId = wxID_EXIT ;
wxString  wxApp::s_macHelpMenuTitleName = wxT("&Help") ;

bool      wxApp::sm_isEmbedded = false; // Normally we're not a plugin

#if wxOSX_USE_CARBON

//----------------------------------------------------------------------
// Core Apple Event Support
//----------------------------------------------------------------------

AEEventHandlerUPP sODocHandler = NULL ;
AEEventHandlerUPP sGURLHandler = NULL ;
AEEventHandlerUPP sOAppHandler = NULL ;
AEEventHandlerUPP sPDocHandler = NULL ;
AEEventHandlerUPP sRAppHandler = NULL ;
AEEventHandlerUPP sQuitHandler = NULL ;

pascal OSErr AEHandleODoc( const AppleEvent *event , AppleEvent *reply , SRefCon refcon ) ;
pascal OSErr AEHandleOApp( const AppleEvent *event , AppleEvent *reply , SRefCon refcon ) ;
pascal OSErr AEHandlePDoc( const AppleEvent *event , AppleEvent *reply , SRefCon refcon ) ;
pascal OSErr AEHandleQuit( const AppleEvent *event , AppleEvent *reply , SRefCon refcon ) ;
pascal OSErr AEHandleRApp( const AppleEvent *event , AppleEvent *reply , SRefCon refcon ) ;
pascal OSErr AEHandleGURL( const AppleEvent *event , AppleEvent *reply , SRefCon refcon ) ;

pascal OSErr AEHandleODoc( const AppleEvent *event , AppleEvent *reply , SRefCon WXUNUSED(refcon) )
{
    return wxTheApp->MacHandleAEODoc( (AppleEvent*) event , reply) ;
}

pascal OSErr AEHandleOApp( const AppleEvent *event , AppleEvent *reply , SRefCon WXUNUSED(refcon) )
{
    return wxTheApp->MacHandleAEOApp( (AppleEvent*) event , reply ) ;
}

pascal OSErr AEHandlePDoc( const AppleEvent *event , AppleEvent *reply , SRefCon WXUNUSED(refcon) )
{
    return wxTheApp->MacHandleAEPDoc( (AppleEvent*) event , reply ) ;
}

pascal OSErr AEHandleQuit( const AppleEvent *event , AppleEvent *reply , SRefCon WXUNUSED(refcon) )
{
    return wxTheApp->MacHandleAEQuit( (AppleEvent*) event , reply) ;
}

pascal OSErr AEHandleRApp( const AppleEvent *event , AppleEvent *reply , SRefCon WXUNUSED(refcon) )
{
    return wxTheApp->MacHandleAERApp( (AppleEvent*) event , reply) ;
}

pascal OSErr AEHandleGURL( const AppleEvent *event , AppleEvent *reply , SRefCon WXUNUSED(refcon) )
{
    return wxTheApp->MacHandleAEGURL((WXEVENTREF *)event , reply) ;
}


// AEODoc Calls MacOpenFile on each of the files passed

short wxApp::MacHandleAEODoc(const WXEVENTREF event, WXEVENTREF WXUNUSED(reply))
{
    AEDescList docList;
    AEKeyword keywd;
    DescType returnedType;
    Size actualSize;
    long itemsInList;
    OSErr err;
    short i;

    err = AEGetParamDesc((AppleEvent *)event, keyDirectObject, typeAEList,&docList);
    if (err != noErr)
        return err;

    err = AECountItems(&docList, &itemsInList);
    if (err != noErr)
        return err;

    ProcessSerialNumber PSN ;
    PSN.highLongOfPSN = 0 ;
    PSN.lowLongOfPSN = kCurrentProcess ;
    SetFrontProcess( &PSN ) ;

    wxString fName ;
    FSRef theRef ;

    for (i = 1; i <= itemsInList; i++)
    {
        AEGetNthPtr(
            &docList, i, typeFSRef, &keywd, &returnedType,
            (Ptr)&theRef, sizeof(theRef), &actualSize);
        fName = wxMacFSRefToPath( &theRef ) ;

        MacOpenFile(fName);
    }

    return noErr;
}

// AEODoc Calls MacOpenURL on the url passed

short wxApp::MacHandleAEGURL(const WXEVENTREF event, WXEVENTREF WXUNUSED(reply))
{
    DescType returnedType;
    Size actualSize;
    char url[255];
    OSErr err = AEGetParamPtr((AppleEvent *)event, keyDirectObject, typeChar,
                              &returnedType, url, sizeof(url)-1,
                              &actualSize);
    if (err != noErr)
        return err;

    url[actualSize] = '\0';    // Terminate the C string

    ProcessSerialNumber PSN ;
    PSN.highLongOfPSN = 0 ;
    PSN.lowLongOfPSN = kCurrentProcess ;
    SetFrontProcess( &PSN ) ;

    MacOpenURL(wxString(url, wxConvUTF8));

    return noErr;
}

// AEPDoc Calls MacPrintFile on each of the files passed

short wxApp::MacHandleAEPDoc(const WXEVENTREF event , WXEVENTREF WXUNUSED(reply))
{
    AEDescList docList;
    AEKeyword keywd;
    DescType returnedType;
    Size actualSize;
    long itemsInList;
    OSErr err;
    short i;

    err = AEGetParamDesc((AppleEvent *)event, keyDirectObject, typeAEList,&docList);
    if (err != noErr)
        return err;

    err = AECountItems(&docList, &itemsInList);
    if (err != noErr)
        return err;

    ProcessSerialNumber PSN ;
    PSN.highLongOfPSN = 0 ;
    PSN.lowLongOfPSN = kCurrentProcess ;
    SetFrontProcess( &PSN ) ;

    wxString fName ;
    FSRef theRef ;

    for (i = 1; i <= itemsInList; i++)
    {
        AEGetNthPtr(
            &docList, i, typeFSRef, &keywd, &returnedType,
            (Ptr)&theRef, sizeof(theRef), &actualSize);
        fName = wxMacFSRefToPath( &theRef ) ;

        MacPrintFile(fName);
    }

    return noErr;
}

// AEOApp calls MacNewFile

short wxApp::MacHandleAEOApp(const WXEVENTREF WXUNUSED(event) , WXEVENTREF WXUNUSED(reply))
{
    MacNewFile() ;
    return noErr ;
}

// AEQuit attempts to quit the application

short wxApp::MacHandleAEQuit(const WXEVENTREF WXUNUSED(event) , WXEVENTREF WXUNUSED(reply))
{
    wxWindow* win = GetTopWindow() ;
    if ( win )
    {
        wxCommandEvent exitEvent(wxEVT_COMMAND_MENU_SELECTED, s_macExitMenuItemId);
        if (!win->ProcessEvent(exitEvent))
            win->Close(true) ;
    }
    else
    {
        ExitMainLoop() ;
    }

    return noErr ;
}

// AEROApp calls MacReopenApp

short wxApp::MacHandleAERApp(const WXEVENTREF WXUNUSED(event) , WXEVENTREF WXUNUSED(reply))
{
    MacReopenApp() ;

    return noErr ;
}

#endif

//----------------------------------------------------------------------
// Support Routines linking the Mac...File Calls to the Document Manager
//----------------------------------------------------------------------

void wxApp::MacOpenFile(const wxString & fileName )
{
#if wxUSE_DOC_VIEW_ARCHITECTURE
    wxDocManager* dm = wxDocManager::GetDocumentManager() ;
    if ( dm )
        dm->CreateDocument(fileName , wxDOC_SILENT ) ;
#endif
}

void wxApp::MacOpenURL(const wxString & WXUNUSED(url) )
{
}

void wxApp::MacPrintFile(const wxString & fileName )
{
#if wxUSE_DOC_VIEW_ARCHITECTURE

#if wxUSE_PRINTING_ARCHITECTURE
    wxDocManager* dm = wxDocManager::GetDocumentManager() ;
    if ( dm )
    {
        wxDocument *doc = dm->CreateDocument(fileName , wxDOC_SILENT ) ;
        if ( doc )
        {
            wxView* view = doc->GetFirstView() ;
            if ( view )
            {
                wxPrintout *printout = view->OnCreatePrintout();
                if (printout)
                {
                    wxPrinter printer;
                    printer.Print(view->GetFrame(), printout, true);
                    delete printout;
                }
            }

            if (doc->Close())
            {
                doc->DeleteAllViews();
                dm->RemoveDocument(doc) ;
            }
        }
    }
#endif //print

#endif //docview
}



void wxApp::MacNewFile()
{
}

void wxApp::MacReopenApp()
{
    // HIG says :
    // if there is no open window -> create a new one
    // if all windows are hidden -> show the first
    // if some windows are not hidden -> do nothing

    wxWindowList::compatibility_iterator node = wxTopLevelWindows.GetFirst();
    if ( node == NULL )
    {
        MacNewFile() ;
    }
    else
    {
        wxTopLevelWindow* firstIconized = NULL ;
        wxTopLevelWindow* firstHidden = NULL ;
        while (node)
        {
            wxTopLevelWindow* win = (wxTopLevelWindow*) node->GetData();
            if ( !win->IsShown() )
            {
                // make sure we don't show 'virtual toplevel windows' like wxTaskBarIconWindow
                if ( firstHidden == NULL && ( wxDynamicCast( win, wxFrame ) || wxDynamicCast( win, wxDialog ) ) )
                   firstHidden = win ;
            }
            else if ( win->IsIconized() )
            {
                if ( firstIconized == NULL )
                    firstIconized = win ;
            }
            else
            {
                // we do have a visible, non-iconized toplevelwindow -> do nothing
                return;
            }

            node = node->GetNext();
        }

        if ( firstIconized )
            firstIconized->Iconize( false ) ;
        else if ( firstHidden )
            firstHidden->Show( true );
    }
}

//----------------------------------------------------------------------
// Macintosh CommandID support - converting between native and wx IDs
//----------------------------------------------------------------------

// if no native match they just return the passed-in id

#if wxOSX_USE_CARBON

struct IdPair
{
    UInt32 macId ;
    int wxId ;
} ;

IdPair gCommandIds [] =
{
    { kHICommandCut ,           wxID_CUT } ,
    { kHICommandCopy ,          wxID_COPY } ,
    { kHICommandPaste ,         wxID_PASTE } ,
    { kHICommandSelectAll ,     wxID_SELECTALL } ,
    { kHICommandClear ,         wxID_CLEAR } ,
    { kHICommandUndo ,          wxID_UNDO } ,
    { kHICommandRedo ,          wxID_REDO } ,
} ;

int wxMacCommandToId( UInt32 macCommandId )
{
    int wxid = 0 ;

    switch ( macCommandId )
    {
        case kHICommandPreferences :
            wxid = wxApp::s_macPreferencesMenuItemId ;
            break ;

        case kHICommandQuit :
            wxid = wxApp::s_macExitMenuItemId ;
            break ;

        case kHICommandAbout :
            wxid = wxApp::s_macAboutMenuItemId ;
            break ;

        default :
            {
                for ( size_t i = 0 ; i < WXSIZEOF(gCommandIds) ; ++i )
                {
                    if ( gCommandIds[i].macId == macCommandId )
                    {
                        wxid = gCommandIds[i].wxId ;
                        break ;
                    }
                }
            }
            break ;
    }

    if ( wxid == 0 )
        wxid = (int) macCommandId ;

    return wxid ;
}

UInt32 wxIdToMacCommand( int wxId )
{
    UInt32 macId = 0 ;

    if ( wxId == wxApp::s_macPreferencesMenuItemId )
        macId = kHICommandPreferences ;
    else if (wxId == wxApp::s_macExitMenuItemId)
        macId = kHICommandQuit ;
    else if (wxId == wxApp::s_macAboutMenuItemId)
        macId = kHICommandAbout ;
    else
    {
        for ( size_t i = 0 ; i < WXSIZEOF(gCommandIds) ; ++i )
        {
            if ( gCommandIds[i].wxId == wxId )
            {
                macId = gCommandIds[i].macId ;
                break ;
            }
        }
    }

    if ( macId == 0 )
        macId = (int) wxId ;

    return macId ;
}

wxMenu* wxFindMenuFromMacCommand( const HICommand &command , wxMenuItem* &item )
{
    wxMenu* itemMenu = NULL ;
#ifndef __WXUNIVERSAL__
    int id = 0 ;

    // for 'standard' commands which don't have a wx-menu
    if ( command.commandID == kHICommandPreferences || command.commandID == kHICommandQuit || command.commandID == kHICommandAbout )
    {
        id = wxMacCommandToId( command.commandID ) ;

        wxMenuBar* mbar = wxMenuBar::MacGetInstalledMenuBar() ;
        if ( mbar )
            item = mbar->FindItem( id , &itemMenu ) ;
    }
    else if ( command.commandID != 0 && command.menu.menuRef != 0 && command.menu.menuItemIndex != 0 )
    {
        id = wxMacCommandToId( command.commandID ) ;
        // make sure it is one of our own menus, or of the 'synthetic' apple and help menus , otherwise don't touch
        MenuItemIndex firstUserHelpMenuItem ;
        static MenuHandle helpMenuHandle = NULL ;
        if ( helpMenuHandle == NULL )
        {
            if ( UMAGetHelpMenuDontCreate( &helpMenuHandle , &firstUserHelpMenuItem) != noErr )
                helpMenuHandle = NULL ;
        }

        // is it part of the application or the Help menu, then look for the id directly
        if ( ( GetMenuHandle( kwxMacAppleMenuId ) != NULL && command.menu.menuRef == GetMenuHandle( kwxMacAppleMenuId ) ) ||
             ( helpMenuHandle != NULL && command.menu.menuRef == helpMenuHandle ) ||
             wxMenuBar::MacGetWindowMenuHMenu() != NULL && command.menu.menuRef == wxMenuBar::MacGetWindowMenuHMenu() )
        {
            wxMenuBar* mbar = wxMenuBar::MacGetInstalledMenuBar() ;
            if ( mbar )
                item = mbar->FindItem( id , &itemMenu ) ;
        }
        else
        {
            URefCon refCon = NULL ;

            GetMenuItemRefCon( command.menu.menuRef , command.menu.menuItemIndex , &refCon ) ;
            itemMenu = wxFindMenuFromMacMenu( command.menu.menuRef ) ;
            if ( itemMenu != NULL && refCon != 0)
                item = ((wxMenuItemImpl*) refCon)->GetWXPeer() ;
        }
    }
#endif
    return itemMenu ;
}

#endif

//----------------------------------------------------------------------
// Carbon Event Handler
//----------------------------------------------------------------------

#if wxOSX_USE_CARBON

static const EventTypeSpec eventList[] =
{
    { kEventClassCommand, kEventProcessCommand } ,
    { kEventClassCommand, kEventCommandUpdateStatus } ,

    { kEventClassMenu, kEventMenuOpening },
    { kEventClassMenu, kEventMenuClosed },
    { kEventClassMenu, kEventMenuTargetItem },

    { kEventClassApplication , kEventAppActivated } ,
    { kEventClassApplication , kEventAppDeactivated } ,
    // handling the quit event is not recommended by apple
    // rather using the quit apple event - which we do

    { kEventClassAppleEvent , kEventAppleEvent } ,

    { kEventClassMouse , kEventMouseDown } ,
    { kEventClassMouse , kEventMouseMoved } ,
    { kEventClassMouse , kEventMouseUp } ,
    { kEventClassMouse , kEventMouseDragged } ,
    { 'WXMC' , 'WXMC' }
} ;

static pascal OSStatus
wxMacAppMenuEventHandler( EventHandlerCallRef WXUNUSED(handler),
                          EventRef event,
                          void *WXUNUSED(data) )
{
    wxMacCarbonEvent cEvent( event ) ;
    MenuRef menuRef = cEvent.GetParameter<MenuRef>(kEventParamDirectObject) ;
#ifndef __WXUNIVERSAL__
    wxMenu* menu = wxFindMenuFromMacMenu( menuRef ) ;

    if ( menu )
    {
        switch (GetEventKind(event))
        {
            case kEventMenuOpening:
                menu->HandleMenuOpened();
                break;

            case kEventMenuClosed:
                menu->HandleMenuClosed();
                break;

            case kEventMenuTargetItem:
                {
                    HICommand command ;

                    command.menu.menuRef = menuRef;
                    command.menu.menuItemIndex = cEvent.GetParameter<MenuItemIndex>(kEventParamMenuItemIndex,typeMenuItemIndex) ;
                    command.commandID = cEvent.GetParameter<MenuCommand>(kEventParamMenuCommand,typeMenuCommand) ;
                    if (command.commandID != 0)
                    {
                        wxMenuItem* item = NULL ;
                        wxMenu* itemMenu = wxFindMenuFromMacCommand( command , item ) ;
                        if ( itemMenu && item )
                            itemMenu->HandleMenuItemHighlighted( item );
                    }
                }
                break;

            default:
                wxFAIL_MSG(wxT("Unexpected menu event kind"));
                break;
        }

    }
#endif
    return eventNotHandledErr;
}

static pascal OSStatus
wxMacAppCommandEventHandler( EventHandlerCallRef WXUNUSED(handler) ,
                             EventRef event ,
                             void *WXUNUSED(data) )
{
    OSStatus result = eventNotHandledErr ;

    HICommand command ;

    wxMacCarbonEvent cEvent( event ) ;
    cEvent.GetParameter<HICommand>(kEventParamDirectObject,typeHICommand,&command) ;

    wxMenuItem* item = NULL ;
    wxMenu* itemMenu = wxFindMenuFromMacCommand( command , item ) ;

    if ( item )
    {
        wxASSERT( itemMenu != NULL ) ;

        switch ( cEvent.GetKind() )
        {
            case kEventProcessCommand :
                if ( itemMenu->HandleCommandProcess( item ) )
                    result = noErr;
            break ;

        case kEventCommandUpdateStatus:
            if ( itemMenu->HandleCommandUpdateStatus( item ) )
                    result = noErr;
            break ;

        default :
            break ;
        }
    }
    return result ;
}

static pascal OSStatus
wxMacAppApplicationEventHandler( EventHandlerCallRef WXUNUSED(handler) ,
                                 EventRef event ,
                                 void *WXUNUSED(data) )
{
    OSStatus result = eventNotHandledErr ;
    switch ( GetEventKind( event ) )
    {
        case kEventAppActivated :
            if ( wxTheApp )
                wxTheApp->SetActive( true , NULL ) ;
            result = noErr ;
            break ;

        case kEventAppDeactivated :
            if ( wxTheApp )
                wxTheApp->SetActive( false , NULL ) ;
            result = noErr ;
            break ;

        default :
            break ;
    }

    return result ;
}

pascal OSStatus wxMacAppEventHandler( EventHandlerCallRef handler , EventRef event , void *data )
{
    EventRef formerEvent = (EventRef) wxTheApp->MacGetCurrentEvent() ;
    EventHandlerCallRef formerEventHandlerCallRef = (EventHandlerCallRef) wxTheApp->MacGetCurrentEventHandlerCallRef() ;
    wxTheApp->MacSetCurrentEvent( event , handler ) ;

    OSStatus result = eventNotHandledErr ;
    switch ( GetEventClass( event ) )
    {
#ifndef __LP64__
        case kEventClassCommand :
            result = wxMacAppCommandEventHandler( handler , event , data ) ;
            break ;
#endif
        case kEventClassApplication :
            result = wxMacAppApplicationEventHandler( handler , event , data ) ;
            break ;
#ifndef __LP64__
        case kEventClassMenu :
            result = wxMacAppMenuEventHandler( handler , event , data ) ;
            break ;

        case kEventClassMouse :
            {
                wxMacCarbonEvent cEvent( event ) ;

                WindowRef window ;
                Point screenMouseLocation = cEvent.GetParameter<Point>(kEventParamMouseLocation) ;
                ::FindWindow(screenMouseLocation, &window);
                // only send this event in case it had not already been sent to a tlw, as we get
                // double events otherwise (in case event.skip) was called
                if ( window == NULL )
                    result = wxMacTopLevelMouseEventHandler( handler , event , NULL ) ;
            }
            break ;
#endif
        case kEventClassAppleEvent :
            {
#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_5
                if ( AEProcessEvent != NULL )
                {
                    result = AEProcessEvent(event);
                }
#endif
#if MAC_OS_X_VERSION_MIN_REQUIRED < MAC_OS_X_VERSION_10_5
                {
                    EventRecord rec ;

                    wxMacConvertEventToRecord( event , &rec ) ;
                    result = AEProcessAppleEvent( &rec ) ;
                }
#endif
            }
            break ;

        default :
            break ;
    }

    wxTheApp->MacSetCurrentEvent( formerEvent, formerEventHandlerCallRef ) ;

    return result ;
}

DEFINE_ONE_SHOT_HANDLER_GETTER( wxMacAppEventHandler )
#endif

#if defined( __WXDEBUG__ ) && wxOSX_USE_COCOA_OR_CARBON

pascal static void
wxMacAssertOutputHandler(OSType WXUNUSED(componentSignature),
                         UInt32 WXUNUSED(options),
                         const char *assertionString,
                         const char *exceptionLabelString,
                         const char *errorString,
                         const char *fileName,
                         long lineNumber,
                         void *value,
                         ConstStr255Param WXUNUSED(outputMsg))
{
    // flow into assert handling
    wxString fileNameStr ;
    wxString assertionStr ;
    wxString exceptionStr ;
    wxString errorStr ;

#if wxUSE_UNICODE
    fileNameStr = wxString(fileName, wxConvLocal);
    assertionStr = wxString(assertionString, wxConvLocal);
    exceptionStr = wxString((exceptionLabelString!=0) ? exceptionLabelString : "", wxConvLocal) ;
    errorStr = wxString((errorString!=0) ? errorString : "", wxConvLocal) ;
#else
    fileNameStr = fileName;
    assertionStr = assertionString;
    exceptionStr = (exceptionLabelString!=0) ? exceptionLabelString : "" ;
    errorStr = (errorString!=0) ? errorString : "" ;
#endif

#if 1
    // flow into log
    wxLogDebug( wxT("AssertMacros: %s %s %s file: %s, line: %ld (value %p)\n"),
        assertionStr.c_str() ,
        exceptionStr.c_str() ,
        errorStr.c_str(),
        fileNameStr.c_str(), lineNumber ,
        value ) ;
#else

    wxOnAssert(fileNameStr, lineNumber , assertionStr ,
        wxString::Format( wxT("%s %s value (%p)") , exceptionStr, errorStr , value ) ) ;
#endif
}

#endif //__WXDEBUG__

extern "C" void macPostedEventCallback(void *WXUNUSED(unused))
{
    wxTheApp->ProcessPendingEvents();
}

bool wxApp::Initialize(int& argc, wxChar **argv)
{
    // Mac-specific

#if defined( __WXDEBUG__ ) && wxOSX_USE_COCOA_OR_CARBON
    InstallDebugAssertOutputHandler( NewDebugAssertOutputHandlerUPP( wxMacAssertOutputHandler ) );
#endif

    // Mac OS X passes a process serial number command line argument when
    // the application is launched from the Finder. This argument must be
    // removed from the command line arguments before being handled by the
    // application (otherwise applications would need to handle it)
    if ( argc > 1 )
    {
        static const wxChar *ARG_PSN = _T("-psn_");
        if ( wxStrncmp(argv[1], ARG_PSN, wxStrlen(ARG_PSN)) == 0 )
        {
            // remove this argument
            --argc;
            memmove(argv + 1, argv + 2, argc * sizeof(char *));
        }
    }

    if ( !wxAppBase::Initialize(argc, argv) )
        return false;

#if wxUSE_INTL
    wxFont::SetDefaultEncoding(wxLocale::GetSystemEncoding());
#endif

    // these might be the startup dirs, set them to the 'usual' dir containing the app bundle
    wxString startupCwd = wxGetCwd() ;
    if ( startupCwd == wxT("/") || startupCwd.Right(15) == wxT("/Contents/MacOS") )
    {
        CFURLRef url = CFBundleCopyBundleURL(CFBundleGetMainBundle() ) ;
        CFURLRef urlParent = CFURLCreateCopyDeletingLastPathComponent( kCFAllocatorDefault , url ) ;
        CFRelease( url ) ;
        CFStringRef path = CFURLCopyFileSystemPath ( urlParent , kCFURLPOSIXPathStyle ) ;
        CFRelease( urlParent ) ;
        wxString cwd = wxCFStringRef(path).AsString(wxLocale::GetSystemEncoding());
        wxSetWorkingDirectory( cwd ) ;
    }

    /* connect posted events to common-mode run loop so that wxPostEvent events
       are handled even while we're in the menu or on a scrollbar */
       /*
    CFRunLoopSourceContext event_posted_context = {0};
    event_posted_context.perform = macPostedEventCallback;
    m_macEventPosted = CFRunLoopSourceCreate(NULL,0,&event_posted_context);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), m_macEventPosted, kCFRunLoopCommonModes);
	// run loop takes ownership
	CFRelease(m_macEventPosted);
        */
    return true;
}

bool wxApp::CallOnInit()
{
    wxMacAutoreleasePool autoreleasepool;
    return OnInit();
}

bool wxApp::OnInitGui()
{
    if ( !wxAppBase::OnInitGui() )
        return false ;

    if ( !DoInitGui() )
        return false;

    return true ;
}

bool wxApp::ProcessIdle()
{
    wxMacAutoreleasePool autoreleasepool;
    return wxAppBase::ProcessIdle();
}

#if wxOSX_USE_CARBON
bool wxApp::DoInitGui()
{
    InstallStandardEventHandler( GetApplicationEventTarget() ) ;
    if (!sm_isEmbedded)
    {
        InstallApplicationEventHandler(
            GetwxMacAppEventHandlerUPP(),
            GetEventTypeCount(eventList), eventList, wxTheApp, (EventHandlerRef *)&(wxTheApp->m_macEventHandler));
    }

    if (!sm_isEmbedded)
    {
        sODocHandler = NewAEEventHandlerUPP(AEHandleODoc) ;
        sGURLHandler = NewAEEventHandlerUPP(AEHandleGURL) ;
        sOAppHandler = NewAEEventHandlerUPP(AEHandleOApp) ;
        sPDocHandler = NewAEEventHandlerUPP(AEHandlePDoc) ;
        sRAppHandler = NewAEEventHandlerUPP(AEHandleRApp) ;
        sQuitHandler = NewAEEventHandlerUPP(AEHandleQuit) ;

        AEInstallEventHandler( kCoreEventClass , kAEOpenDocuments ,
                               sODocHandler , 0 , FALSE ) ;
        AEInstallEventHandler( kInternetEventClass, kAEGetURL,
                               sGURLHandler , 0 , FALSE ) ;
        AEInstallEventHandler( kCoreEventClass , kAEOpenApplication ,
                               sOAppHandler , 0 , FALSE ) ;
        AEInstallEventHandler( kCoreEventClass , kAEPrintDocuments ,
                               sPDocHandler , 0 , FALSE ) ;
        AEInstallEventHandler( kCoreEventClass , kAEReopenApplication ,
                               sRAppHandler , 0 , FALSE ) ;
        AEInstallEventHandler( kCoreEventClass , kAEQuitApplication ,
                               sQuitHandler , 0 , FALSE ) ;
    }

    if ( !wxMacInitCocoa() )
        return false;

    return true;
}

void wxApp::DoCleanUp()
{
    if (!sm_isEmbedded)
        RemoveEventHandler( (EventHandlerRef)(wxTheApp->m_macEventHandler) );

    if (!sm_isEmbedded)
    {
        AERemoveEventHandler( kCoreEventClass , kAEOpenDocuments ,
                               sODocHandler , FALSE ) ;
        AERemoveEventHandler( kInternetEventClass, kAEGetURL,
                               sGURLHandler , FALSE ) ;
        AERemoveEventHandler( kCoreEventClass , kAEOpenApplication ,
                               sOAppHandler , FALSE ) ;
        AERemoveEventHandler( kCoreEventClass , kAEPrintDocuments ,
                               sPDocHandler , FALSE ) ;
        AERemoveEventHandler( kCoreEventClass , kAEReopenApplication ,
                               sRAppHandler , FALSE ) ;
        AERemoveEventHandler( kCoreEventClass , kAEQuitApplication ,
                               sQuitHandler , FALSE ) ;

        DisposeAEEventHandlerUPP( sODocHandler ) ;
        DisposeAEEventHandlerUPP( sGURLHandler ) ;
        DisposeAEEventHandlerUPP( sOAppHandler ) ;
        DisposeAEEventHandlerUPP( sPDocHandler ) ;
        DisposeAEEventHandlerUPP( sRAppHandler ) ;
        DisposeAEEventHandlerUPP( sQuitHandler ) ;
    }
}

#endif

void wxApp::CleanUp()
{
#if wxUSE_TOOLTIPS
    wxToolTip::RemoveToolTips() ;
#endif

    if (m_macEventPosted)
	{
		CFRunLoopRemoveSource(CFRunLoopGetCurrent(), m_macEventPosted, kCFRunLoopCommonModes);
		m_macEventPosted = NULL;
	}

    DoCleanUp();

    wxAppBase::CleanUp();
}

//----------------------------------------------------------------------
// misc initialization stuff
//----------------------------------------------------------------------

#if wxOSX_USE_CARBON && MAC_OS_X_VERSION_MIN_REQUIRED < MAC_OS_X_VERSION_10_5
bool wxMacConvertEventToRecord( EventRef event , EventRecord *rec)
{
    OSStatus err = noErr ;
    bool converted = ConvertEventRefToEventRecord( event, rec) ;

    if ( !converted )
    {
        switch ( GetEventClass( event ) )
        {
            case kEventClassKeyboard :
            {
                converted = true ;
                switch ( GetEventKind(event) )
                {
                    case kEventRawKeyDown :
                        rec->what = keyDown ;
                        break ;

                    case kEventRawKeyRepeat :
                        rec->what = autoKey ;
                        break ;

                    case kEventRawKeyUp :
                        rec->what = keyUp ;
                        break ;

                    case kEventRawKeyModifiersChanged :
                        rec->what = nullEvent ;
                        break ;

                    default :
                        converted = false ;
                        break ;
                }

                if ( converted )
                {
                    UInt32 keyCode ;
                    unsigned char charCode ;
                    UInt32 modifiers ;
                    GetMouse( &rec->where) ;
                    err = GetEventParameter(event, kEventParamKeyModifiers, typeUInt32, NULL, 4, NULL, &modifiers);
                    err = GetEventParameter(event, kEventParamKeyCode, typeUInt32, NULL, 4, NULL, &keyCode);
                    err = GetEventParameter(event, kEventParamKeyMacCharCodes, typeChar, NULL, 1, NULL, &charCode);
                    rec->modifiers = modifiers ;
                    rec->message = (keyCode << 8 ) + charCode ;
                }
            }
            break ;

            case kEventClassTextInput :
            {
                switch ( GetEventKind( event ) )
                {
                    case kEventTextInputUnicodeForKeyEvent :
                        {
                            EventRef rawEvent ;
                            err = GetEventParameter(
                                event, kEventParamTextInputSendKeyboardEvent, typeEventRef, NULL,
                                sizeof(rawEvent), NULL, &rawEvent ) ;
                            converted = true ;

                            {
                                UInt32 keyCode, modifiers;
                                unsigned char charCode ;
                                GetMouse( &rec->where) ;
                                rec->what = keyDown ;
                                err = GetEventParameter(rawEvent, kEventParamKeyModifiers, typeUInt32, NULL, 4, NULL, &modifiers);
                                err = GetEventParameter(rawEvent, kEventParamKeyCode, typeUInt32, NULL, 4, NULL, &keyCode);
                                err = GetEventParameter(rawEvent, kEventParamKeyMacCharCodes, typeChar, NULL, 1, NULL, &charCode);
                                rec->modifiers = modifiers ;
                                rec->message = (keyCode << 8 ) + charCode ;
                            }
                       }
                       break ;

                    default :
                        break ;
                }
            }
            break ;

            default :
                break ;
        }
    }

    return converted ;
}
#endif

wxApp::wxApp()
{
    m_printMode = wxPRINT_WINDOWS;

    m_macCurrentEvent = NULL ;
    m_macCurrentEventHandlerCallRef = NULL ;
    m_macEventPosted = NULL ;
}

CFMutableArrayRef GetAutoReleaseArray()
{
    static CFMutableArrayRef array = 0;
    if ( array == 0)
        array= CFArrayCreateMutable(kCFAllocatorDefault,0,&kCFTypeArrayCallBacks);
    return array;
}

void wxApp::MacAddToAutorelease( void* cfrefobj )
{
    CFArrayAppendValue( GetAutoReleaseArray(), cfrefobj );
}

void wxApp::OnIdle(wxIdleEvent& WXUNUSED(event))
{
    // If they are pending events, we must process them: pending events are
    // either events to the threads other than main or events posted with
    // wxPostEvent() functions
#ifndef __WXUNIVERSAL__
#if wxUSE_MENUS
  if (!wxMenuBar::MacGetInstalledMenuBar() && wxMenuBar::MacGetCommonMenuBar())
    wxMenuBar::MacGetCommonMenuBar()->MacInstallMenuBar();
#endif
#endif
    CFArrayRemoveAllValues( GetAutoReleaseArray() );
}

void wxApp::WakeUpIdle()
{
    if (m_macEventPosted)
    {
        CFRunLoopSourceSignal(m_macEventPosted);
    }

    wxMacWakeUp() ;
}

void wxApp::OnEndSession(wxCloseEvent& WXUNUSED(event))
{
    if (GetTopWindow())
        GetTopWindow()->Close(true);
}

// Default behaviour: close the application with prompts. The
// user can veto the close, and therefore the end session.
void wxApp::OnQueryEndSession(wxCloseEvent& event)
{
    if (GetTopWindow())
    {
        if (!GetTopWindow()->Close(!event.CanVeto()))
            event.Veto(true);
    }
}

extern "C" void wxCYield() ;
void wxCYield()
{
    wxYield() ;
}

// Yield to other processes

bool wxApp::Yield(bool onlyIfNeeded)
{
#if wxUSE_THREADS
    // Yielding from a non-gui thread needs to bail out, otherwise we end up
    // possibly sending events in the thread too.
    if ( !wxThread::IsMain() )
    {
        return true;
    }
#endif // wxUSE_THREADS

    if (m_isInsideYield)
    {
        if ( !onlyIfNeeded )
        {
            wxFAIL_MSG( wxT("wxYield called recursively" ) );
        }

        return false;
    }

    m_isInsideYield = true;

#if wxUSE_LOG
    // disable log flushing from here because a call to wxYield() shouldn't
    // normally result in message boxes popping up &c
    wxLog::Suspend();
#endif // wxUSE_LOG

    wxEventLoop * const
        loop = static_cast<wxEventLoop *>(wxEventLoop::GetActive());
    if ( loop )
    {
        // process all pending events:
        while ( loop->Pending() )
            loop->Dispatch();
    }

    // it's necessary to call ProcessIdle() to update the frames sizes which
    // might have been changed (it also will update other things set from
    // OnUpdateUI() which is a nice (and desired) side effect)
    while ( ProcessIdle() ) {}

#if wxUSE_LOG
    wxLog::Resume();
#endif // wxUSE_LOG
    m_isInsideYield = false;

    return true;
}

// virtual
void wxApp::MacHandleUnhandledEvent( WXEVENTREF WXUNUSED(evr) )
{
    // Override to process unhandled events as you please
}

#if wxOSX_USE_CARBON

long wxMacTranslateKey(unsigned char key, unsigned char code)
{
    long retval = key ;
    switch (key)
    {
        case kHomeCharCode :
            retval = WXK_HOME;
            break;

        case kEnterCharCode :
            retval = WXK_RETURN;
            break;
        case kEndCharCode :
            retval = WXK_END;
            break;

        case kHelpCharCode :
            retval = WXK_HELP;
            break;

        case kBackspaceCharCode :
            retval = WXK_BACK;
            break;

        case kTabCharCode :
            retval = WXK_TAB;
            break;

        case kPageUpCharCode :
            retval = WXK_PAGEUP;
            break;

        case kPageDownCharCode :
            retval = WXK_PAGEDOWN;
            break;

        case kReturnCharCode :
            retval = WXK_RETURN;
            break;

        case kFunctionKeyCharCode :
        {
            switch ( code )
            {
                case 0x7a :
                    retval = WXK_F1 ;
                    break;

                case 0x78 :
                    retval = WXK_F2 ;
                    break;

                case 0x63 :
                    retval = WXK_F3 ;
                    break;

                case 0x76 :
                    retval = WXK_F4 ;
                    break;

                case 0x60 :
                    retval = WXK_F5 ;
                    break;

                case 0x61 :
                    retval = WXK_F6 ;
                    break;

                case 0x62:
                    retval = WXK_F7 ;
                    break;

                case 0x64 :
                    retval = WXK_F8 ;
                    break;

                case 0x65 :
                    retval = WXK_F9 ;
                    break;

                case 0x6D :
                    retval = WXK_F10 ;
                    break;

                case 0x67 :
                    retval = WXK_F11 ;
                    break;

                case 0x6F :
                    retval = WXK_F12 ;
                    break;

                case 0x69 :
                    retval = WXK_F13 ;
                    break;

                case 0x6B :
                    retval = WXK_F14 ;
                    break;

                case 0x71 :
                    retval = WXK_F15 ;
                    break;

                default:
                    break;
            }
        }
        break ;

        case kEscapeCharCode :
            retval = WXK_ESCAPE ;
            break ;

        case kLeftArrowCharCode :
            retval = WXK_LEFT ;
            break ;

        case kRightArrowCharCode :
            retval = WXK_RIGHT ;
            break ;

        case kUpArrowCharCode :
            retval = WXK_UP ;
            break ;

        case kDownArrowCharCode :
            retval = WXK_DOWN ;
            break ;

        case kDeleteCharCode :
            retval = WXK_DELETE ;
            break ;

        default:
            break ;
     } // end switch

    return retval;
}

int wxMacKeyCodeToModifier(wxKeyCode key)
{
    switch (key)
    {
    case WXK_START:
    case WXK_MENU:
        return cmdKey;

    case WXK_SHIFT:
        return shiftKey;

    case WXK_CAPITAL:
        return alphaLock;

    case WXK_ALT:
        return optionKey;

    case WXK_CONTROL:
        return controlKey;

    default:
        return 0;
    }
}
#endif

wxMouseState wxGetMouseState()
{
    wxMouseState ms;

    wxPoint pt = wxGetMousePosition();
    ms.SetX(pt.x);
    ms.SetY(pt.y);

#if wxOSX_USE_CARBON
    UInt32 buttons = GetCurrentButtonState();
    ms.SetLeftDown( (buttons & 0x01) != 0 );
    ms.SetMiddleDown( (buttons & 0x04) != 0 );
    ms.SetRightDown( (buttons & 0x02) != 0 );

    UInt32 modifiers = GetCurrentKeyModifiers();
    ms.SetControlDown(modifiers & controlKey);
    ms.SetShiftDown(modifiers & shiftKey);
    ms.SetAltDown(modifiers & optionKey);
    ms.SetMetaDown(modifiers & cmdKey);
#else
    // TODO
#endif
    return ms;
}

// TODO : once the new key/char handling is tested, move all the code to wxWindow

bool wxApp::MacSendKeyDownEvent( wxWindow* focus , long keymessage , long modifiers , long when , short wherex , short wherey , wxChar uniChar )
{
    if ( !focus )
        return false ;

    wxKeyEvent event(wxEVT_KEY_DOWN) ;
    MacCreateKeyEvent( event, focus , keymessage , modifiers , when , wherex , wherey , uniChar ) ;

    return focus->HandleKeyEvent(event);
}

bool wxApp::MacSendKeyUpEvent( wxWindow* focus , long keymessage , long modifiers , long when , short wherex , short wherey , wxChar uniChar )
{
    if ( !focus )
        return false ;

    bool handled;
    wxKeyEvent event( wxEVT_KEY_UP ) ;
    MacCreateKeyEvent( event, focus , keymessage , modifiers , when , wherex , wherey , uniChar ) ;

    return focus->HandleKeyEvent(event) ;
}

bool wxApp::MacSendCharEvent( wxWindow* focus , long keymessage , long modifiers , long when , short wherex , short wherey , wxChar uniChar )
{
    if ( !focus )
        return false ;
    wxKeyEvent event(wxEVT_CHAR) ;
    MacCreateKeyEvent( event, focus , keymessage , modifiers , when , wherex , wherey , uniChar ) ;

    bool handled = false ;

#if wxOSX_USE_CARBON
    long keyval = event.m_keyCode ;
    wxNonOwnedWindow *tlw = focus->MacGetTopLevelWindow() ;

    if (tlw)
    {
        event.SetEventType( wxEVT_CHAR_HOOK );
        handled = tlw->HandleWindowEvent( event );
        if ( handled && event.GetSkipped() )
            handled = false ;
    }

    if ( !handled )
    {
        event.SetEventType( wxEVT_CHAR );
        event.Skip( false ) ;
        handled = focus->HandleWindowEvent( event ) ;
    }

    if ( !handled && (keyval == WXK_TAB) )
    {
        wxWindow* iter = focus->GetParent() ;
        while ( iter && !handled )
        {
            if ( iter->HasFlag( wxTAB_TRAVERSAL ) )
            {
                wxNavigationKeyEvent new_event;
                new_event.SetEventObject( focus );
                new_event.SetDirection( !event.ShiftDown() );
                /* CTRL-TAB changes the (parent) window, i.e. switch notebook page */
                new_event.SetWindowChange( event.ControlDown() );
                new_event.SetCurrentFocus( focus );
                handled = focus->GetParent()->HandleWindowEvent( new_event );
                if ( handled && new_event.GetSkipped() )
                    handled = false ;
            }

            iter = iter->GetParent() ;
        }
    }

    // backdoor handler for default return and command escape
    if ( !handled && (!focus->IsKindOf(CLASSINFO(wxControl) ) || !focus->AcceptsFocus() ) )
    {
        // if window is not having a focus still testing for default enter or cancel
        // TODO: add the UMA version for ActiveNonFloatingWindow
#ifndef __LP64__
        wxWindow* focus = wxNonOwnedWindow::GetFromWXWindow( (WXWindow) FrontWindow() ) ;
        if ( focus )
        {
            if ( keyval == WXK_RETURN || keyval == WXK_NUMPAD_ENTER )
            {
                wxTopLevelWindow *tlw = wxDynamicCast(wxGetTopLevelParent(focus), wxTopLevelWindow);
                if ( tlw && tlw->GetDefaultItem() )
                {
                    wxButton *def = wxDynamicCast(tlw->GetDefaultItem(), wxButton);
                    if ( def && def->IsEnabled() )
                    {
                        wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, def->GetId() );
                        event.SetEventObject(def);
                        def->Command(event);

                        return true ;
                    }
                }
            }
            else if (keyval == WXK_ESCAPE || (keyval == '.' && modifiers & cmdKey ) )
            {
                // generate wxID_CANCEL if command-. or <esc> has been pressed (typically in dialogs)
                wxCommandEvent new_event(wxEVT_COMMAND_BUTTON_CLICKED,wxID_CANCEL);
                new_event.SetEventObject( focus );
                handled = focus->HandleWindowEvent( new_event );
            }
        }
#endif
    }
#endif
    return handled ;
}

// This method handles common code for SendKeyDown, SendKeyUp, and SendChar events.
void wxApp::MacCreateKeyEvent( wxKeyEvent& event, wxWindow* focus , long keymessage , long modifiers , long when , short wherex , short wherey , wxChar uniChar )
{
#if wxOSX_USE_CARBON
    short keycode, keychar ;

    keychar = short(keymessage & charCodeMask);
    keycode = short(keymessage & keyCodeMask) >> 8 ;
    if ( !(event.GetEventType() == wxEVT_CHAR) && (modifiers & (controlKey | shiftKey | optionKey) ) )
    {
        // control interferes with some built-in keys like pgdown, return etc. therefore we remove the controlKey modifier
        // and look at the character after
#ifdef __LP64__
		// TODO new implementation using TextInputSources
#else
        UInt32 state = 0;
        UInt32 keyInfo = KeyTranslate((Ptr)GetScriptManagerVariable(smKCHRCache), ( modifiers & (~(controlKey | shiftKey | optionKey))) | keycode, &state);
        keychar = short(keyInfo & charCodeMask);
#endif
    }

    long keyval = wxMacTranslateKey(keychar, keycode) ;
    if ( keyval == keychar && ( event.GetEventType() == wxEVT_KEY_UP || event.GetEventType() == wxEVT_KEY_DOWN ) )
        keyval = wxToupper( keyval ) ;

    // Check for NUMPAD keys.  For KEY_UP/DOWN events we need to use the
    // WXK_NUMPAD constants, but for the CHAR event we want to use the
    // standard ascii values
    if ( event.GetEventType() != wxEVT_CHAR )
    {
        if (keyval >= '0' && keyval <= '9' && keycode >= 82 && keycode <= 92)
        {
            keyval = (keyval - '0') + WXK_NUMPAD0;
        }
        else if (keycode >= 65 && keycode <= 81)
        {
            switch (keycode)
            {
                case 76 :
                    keyval = WXK_NUMPAD_ENTER;
                    break;

                case 81:
                    keyval = WXK_NUMPAD_EQUAL;
                    break;

                case 67:
                    keyval = WXK_NUMPAD_MULTIPLY;
                    break;

                case 75:
                    keyval = WXK_NUMPAD_DIVIDE;
                    break;

                case 78:
                    keyval = WXK_NUMPAD_SUBTRACT;
                    break;

                case 69:
                    keyval = WXK_NUMPAD_ADD;
                    break;

                case 65:
                    keyval = WXK_NUMPAD_DECIMAL;
                    break;
                default:
                    break;
            }
        }
    }

    event.m_shiftDown = modifiers & shiftKey;
    event.m_controlDown = modifiers & controlKey;
    event.m_altDown = modifiers & optionKey;
    event.m_metaDown = modifiers & cmdKey;
    event.m_keyCode = keyval ;
#if wxUSE_UNICODE
    event.m_uniChar = uniChar ;
#endif

    event.m_rawCode = keymessage;
    event.m_rawFlags = modifiers;
    event.m_x = wherex;
    event.m_y = wherey;
    event.SetTimestamp(when);
    event.SetEventObject(focus);
#endif
}


void wxApp::MacHideApp()
{
#if wxOSX_USE_CARBON
    wxMacCarbonEvent event( kEventClassCommand , kEventCommandProcess );
    HICommand command;
    memset( &command, 0 , sizeof(command) );
    command.commandID = kHICommandHide ;
    event.SetParameter<HICommand>(kEventParamDirectObject, command );
    SendEventToApplication( event );
#endif
}
