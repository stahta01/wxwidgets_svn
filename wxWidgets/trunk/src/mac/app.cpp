/////////////////////////////////////////////////////////////////////////////
// Name:        app.cpp
// Purpose:     wxApp
// Author:      AUTHOR
// Modified by:
// Created:     ??/??/98
// RCS-ID:      $Id$
// Copyright:   (c) AUTHOR
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "app.h"
#endif

#include "wx/defs.h"

#include "wx/window.h"
#include "wx/frame.h"
#include "wx/button.h"
#include "wx/app.h"
#include "wx/utils.h"
#include "wx/gdicmn.h"
#include "wx/pen.h"
#include "wx/brush.h"
#include "wx/cursor.h"
#include "wx/intl.h"
#include "wx/icon.h"
#include "wx/palette.h"
#include "wx/dc.h"
#include "wx/dialog.h"
#include "wx/msgdlg.h"
#include "wx/log.h"
#include "wx/module.h"
#include "wx/memory.h"
#include "wx/tooltip.h"
#include "wx/textctrl.h"
#include "wx/menu.h"
#if wxUSE_WX_RESOURCES
#  include "wx/resource.h"
#endif

#include <string.h>

// mac

#ifndef __DARWIN__
  #if __option(profile)
    #include <profiler.h>
  #endif
#endif

#include "apprsrc.h"

#include "wx/mac/uma.h"
#include "wx/mac/macnotfy.h"

#ifdef __DARWIN__
#  include <CoreServices/CoreServices.h>
#else
#  include <Sound.h>
#  include <Threads.h>
#  include <ToolUtils.h>
#  include <DiskInit.h>
#  include <Devices.h>
#endif

extern char *wxBuffer;
extern wxList wxPendingDelete;
extern wxList *wxWinMacWindowList;
extern wxList *wxWinMacControlList;

wxApp *wxTheApp = NULL;

#if !USE_SHARED_LIBRARY
IMPLEMENT_DYNAMIC_CLASS(wxApp, wxEvtHandler)
BEGIN_EVENT_TABLE(wxApp, wxEvtHandler)
    EVT_IDLE(wxApp::OnIdle)
        EVT_END_SESSION(wxApp::OnEndSession)
        EVT_QUERY_END_SESSION(wxApp::OnQueryEndSession)
END_EVENT_TABLE()
#endif


const short    kMacMinHeap = (29 * 1024) ;
// platform specific static variables

const short kwxMacMenuBarResource = 1 ;
const short kwxMacAppleMenuId = 1 ;

WXHRGN            wxApp::s_macCursorRgn = NULL;
wxWindow*            wxApp::s_captureWindow = NULL ;
int                    wxApp::s_lastMouseDown = 0 ;
long                     wxApp::sm_lastMessageTime = 0;

bool    wxApp::s_macDefaultEncodingIsPC = true ;
bool wxApp::s_macSupportPCMenuShortcuts = true ;
long wxApp::s_macAboutMenuItemId = wxID_ABOUT ;
wxString wxApp::s_macHelpMenuTitleName = "&Help" ;

pascal OSErr AEHandleODoc( const AppleEvent *event , AppleEvent *reply , long refcon ) ;
pascal OSErr AEHandleOApp( const AppleEvent *event , AppleEvent *reply , long refcon ) ;
pascal OSErr AEHandlePDoc( const AppleEvent *event , AppleEvent *reply , long refcon ) ;
pascal OSErr AEHandleQuit( const AppleEvent *event , AppleEvent *reply , long refcon ) ;


pascal OSErr AEHandleODoc( const AppleEvent *event , AppleEvent *reply , long refcon )
{
    wxApp* app = (wxApp*) refcon ;
    return wxTheApp->MacHandleAEODoc( (AppleEvent*) event , reply) ;
}

pascal OSErr AEHandleOApp( const AppleEvent *event , AppleEvent *reply , long refcon )
{
    wxApp* app = (wxApp*) refcon ;
    return wxTheApp->MacHandleAEOApp( (AppleEvent*) event , reply ) ;
}

pascal OSErr AEHandlePDoc( const AppleEvent *event , AppleEvent *reply , long refcon )
{
    wxApp* app = (wxApp*) refcon ;
    return wxTheApp->MacHandleAEPDoc( (AppleEvent*) event , reply ) ;
}

pascal OSErr AEHandleQuit( const AppleEvent *event , AppleEvent *reply , long refcon )
{
    wxApp* app = (wxApp*) refcon ;
    return wxTheApp->MacHandleAEQuit( (AppleEvent*) event , reply) ;
}

short wxApp::MacHandleAEODoc(const WXEVENTREF event , WXEVENTREF reply)
{
    SysBeep(40) ;
    ProcessSerialNumber PSN ;
    PSN.highLongOfPSN = 0 ;
    PSN.lowLongOfPSN = kCurrentProcess ;
    SetFrontProcess( &PSN ) ;
    return noErr ;
}

short wxApp::MacHandleAEPDoc(const WXEVENTREF event , WXEVENTREF reply)
{
    return noErr ;
}

short wxApp::MacHandleAEOApp(const WXEVENTREF event , WXEVENTREF reply)
{
    return noErr ;
}

short wxApp::MacHandleAEQuit(const WXEVENTREF event , WXEVENTREF reply)
{
    wxWindow* win = GetTopWindow() ;
    if ( win )
    {
        win->Close(TRUE ) ;
    }
    else
    {
        ExitMainLoop() ;
    }
    return noErr ;
}

char StringMac[] =  "\x0d\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8a\x8b\x8c\x8d\x8e\x8f"
                    "\x90\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9a\x9b\x9c\x9d\x9e\x9f"
                    "\xa0\xa1\xa2\xa3\xa4\xa5\xa6\xa7\xa8\xa9\xaa\xab\xac\xae\xaf"
                    "\xb1\xb4\xb5\xb6\xbb\xbc\xbe\xbf"
                    "\xc0\xc1\xc2\xc4\xc7\xc8\xc9\xcb\xcc\xcd\xce\xcf"
                    "\xd0\xd1\xd2\xd3\xd4\xd5\xd6\xd8\xca\xdb" ;

char StringANSI[] = "\x0a\xC4\xC5\xC7\xC9\xD1\xD6\xDC\xE1\xE0\xE2\xE4\xE3\xE5\xE7\xE9\xE8"
                    "\xEA\xEB\xED\xEC\xEE\xEF\xF1\xF3\xF2\xF4\xF6\xF5\xFA\xF9\xFB\xFC"
                    "\x86\xBA\xA2\xA3\xA7\x95\xB6\xDF\xAE\xA9\x99\xB4\xA8\xC6\xD8"
                    "\xB1\xA5\xB5\xF0\xAA\xBA\xE6\xF8"
                    "\xBF\xA1\xAC\x83\xAB\xBB\x85\xC0\xC3\xD5\x8C\x9C"
                    "\x96\x97\x93\x94\x91\x92\xF7\xFF\xA0\x80" ;

void wxMacConvertFromPC( const char *from , char *to , int len )
{
    char *c ;
    if ( from == to )
    {
        for( int i = 0 ; i < len ; ++ i )
        {
            c = strchr( StringANSI , *from ) ;
            if ( c != NULL )
            {
                *to = StringMac[ c - StringANSI] ;
            }
            ++to ;
            ++from ;
        }
    }
    else
    {
        for( int i = 0 ; i < len ; ++ i )
        {
            c = strchr( StringANSI , *from ) ;
            if ( c != NULL )
            {
                *to = StringMac[ c - StringANSI] ;
            }
            else
            {
                *to = *from ;
            }
            ++to ;
            ++from ;
        }
    }
}

void wxMacConvertToPC( const char *from , char *to , int len )
{
    char *c ;
    if ( from == to )
    {
        for( int i = 0 ; i < len ; ++ i )
        {
            c = strchr( StringMac , *from ) ;
            if ( c != NULL )
            {
                *to = StringANSI[ c - StringMac] ;
            }
            ++to ;
            ++from ;
        }
    }
    else
    {
        for( int i = 0 ; i < len ; ++ i )
        {
            c = strchr( StringMac , *from ) ;
            if ( c != NULL )
            {
                *to = StringANSI[ c - StringMac] ;
            }
            else
            {
                *to = *from ;
            }
            ++to ;
            ++from ;
        }
    }
}

void wxMacConvertFromPC( char * p )
{
    char *ptr = p ;
    int len = strlen ( p ) ;

    wxMacConvertFromPC( ptr , ptr , len ) ;
}

void wxMacConvertFromPCForControls( char * p )
{
    char *ptr = p ;
    int len = strlen ( p ) ;

    wxMacConvertFromPC( ptr , ptr , len ) ;
    for ( int i = 0 ; i < strlen ( ptr ) ; i++ )
    {
        if ( ptr[i] == '&' && ptr[i]+1 != ' ' )
        {
            memmove( &ptr[i] , &ptr[i+1] , strlen( &ptr[i+1] ) + 1) ;
        }
    }
}

void wxMacConvertFromPC( unsigned char *p )
{
    char *ptr = (char*) p + 1 ;
    int len = p[0] ;

    wxMacConvertFromPC( ptr , ptr , len ) ;
}

extern char *wxBuffer ;

wxString wxMacMakeMacStringFromPC( const char * p )
{
    const char *ptr = p ;
    int len = strlen ( p ) ;
    char *buf = wxBuffer ;

    if ( len >= BUFSIZ + 512 )
    {
        buf = new char [len+1] ;
    }

    wxMacConvertFromPC( ptr , buf , len ) ;
    buf[len] = 0 ;
    wxString result( buf ) ;
    if ( buf != wxBuffer )
        delete buf ;
    return result ;
}


void wxMacConvertToPC( char * p )
{
    char *ptr = p ;
    int len = strlen ( p ) ;

    wxMacConvertToPC( ptr , ptr , len ) ;
}

void wxMacConvertToPC( unsigned char *p )
{
    char *ptr = (char*) p + 1 ;
    int len = p[0] ;

    wxMacConvertToPC( ptr , ptr , len ) ;
}

wxString wxMacMakePCStringFromMac( const char * p )
{
    const char *ptr = p ;
    int len = strlen ( p ) ;
    char *buf = wxBuffer ;

    if ( len >= BUFSIZ + 512 )
    {
        buf = new char [len+1] ;
    }

    wxMacConvertToPC( ptr , buf , len ) ;
    buf[len] = 0 ;

    wxString result( buf ) ;
    if ( buf != wxBuffer )
        delete buf ;
    return result ;
}

wxString wxMacMakeStringFromMacString( const char* from , bool mac2pcEncoding )
{
    if (mac2pcEncoding)
    {
      return wxMacMakePCStringFromMac( from ) ;
    }
    else
    {
      return wxString( from ) ;
    }
}

wxString wxMacMakeStringFromPascal( StringPtr from , bool mac2pcEncoding )
{
  // this is safe since a pascal string can never be larger than 256 bytes
  char s[256] ;
  CopyPascalStringToC( from , s ) ;
    if (mac2pcEncoding)
    {
      return wxMacMakePCStringFromMac( s ) ;
    }
    else
    {
      return wxString( s ) ;
    }
}

void wxMacStringToPascal( const char * from , StringPtr to , bool pc2macEncoding )
{
    if (pc2macEncoding)
    {
      CopyCStringToPascal( wxMacMakeMacStringFromPC( from ) , to ) ;
    }
    else
    {
      CopyCStringToPascal( from , to ) ;
    }
}
#ifdef WXMAKINGDLL
#ifndef __DARWIN__
// we know it's there ;-)
WXIMPORT char std::__throws_bad_alloc ;
#endif
#endif

bool wxApp::Initialize()
{
  int error = 0 ;

  // Mac-specific

  UMAInitToolbox( 4 ) ;
  SetEventMask( everyEvent ) ;
    UMAShowWatchCursor() ;

#if defined(UNIVERSAL_INTERFACES_VERSION) && (UNIVERSAL_INTERFACES_VERSION >= 0x0340)
    AEInstallEventHandler( kCoreEventClass , kAEOpenDocuments ,   NewAEEventHandlerUPP(AEHandleODoc) ,
                           (long) wxTheApp , FALSE ) ;
    AEInstallEventHandler( kCoreEventClass , kAEOpenApplication , NewAEEventHandlerUPP(AEHandleOApp) ,
                           (long) wxTheApp , FALSE ) ;
    AEInstallEventHandler( kCoreEventClass , kAEPrintDocuments ,  NewAEEventHandlerUPP(AEHandlePDoc) ,
                           (long) wxTheApp , FALSE ) ;
    AEInstallEventHandler( kCoreEventClass , kAEQuitApplication , NewAEEventHandlerUPP(AEHandleQuit) ,
                           (long) wxTheApp , FALSE ) ;
#else
    AEInstallEventHandler( kCoreEventClass , kAEOpenDocuments ,   NewAEEventHandlerProc(AEHandleODoc) ,
                   (long) wxTheApp , FALSE ) ;
    AEInstallEventHandler( kCoreEventClass , kAEOpenApplication , NewAEEventHandlerProc(AEHandleOApp) ,
                   (long) wxTheApp , FALSE ) ;
    AEInstallEventHandler( kCoreEventClass , kAEPrintDocuments ,  NewAEEventHandlerProc(AEHandlePDoc) ,
                   (long) wxTheApp , FALSE ) ;
    AEInstallEventHandler( kCoreEventClass , kAEQuitApplication , NewAEEventHandlerProc(AEHandleQuit) ,
                   (long) wxTheApp , FALSE ) ;
#endif


#ifndef __DARWIN__
  // test the minimal configuration necessary

    #if !TARGET_CARBON
    long theSystem ;
    long theMachine;

    if (Gestalt(gestaltMachineType, &theMachine) != noErr)
    {
        error = kMacSTRWrongMachine;
    }
    else if (theMachine < gestaltMacPlus)
    {
        error = kMacSTRWrongMachine;
    }
    else if (Gestalt(gestaltSystemVersion, &theSystem) != noErr )
    {
        error = kMacSTROldSystem  ;
    }
    else if ( theSystem < 0x0860 )
    {
        error = kMacSTROldSystem  ;
    }
    else if ((long)GetApplLimit() - (long)ApplicationZone() < kMacMinHeap)
    {
        error = kMacSTRSmallSize;
    }
    #endif
    /*
    else
    {
        if ( !UMAHasAppearance() )
        {
            error = kMacSTRNoPre8Yet ;
        }
    }
    */
#endif

    // if we encountered any problems so far, give the error code and exit immediately

  if ( error )
  {
        wxStAppResource resload ;
        short itemHit;
        Str255 message;

        GetIndString(message, 128, error);
        UMAShowArrowCursor() ;
        ParamText("\pFatal Error", message, (ConstStr255Param)"\p", (ConstStr255Param)"\p");
        itemHit = Alert(128, nil);
      return FALSE ;
  }

#ifndef __DARWIN__
  #if __option(profile)
    ProfilerInit( collectDetailed, bestTimeBase , 20000 , 40 ) ;
  #endif
#endif

  // now avoid exceptions thrown for new (bad_alloc)

#ifndef __DARWIN__
  std::__throws_bad_alloc = FALSE ;
#endif

    s_macCursorRgn = ::NewRgn() ;

#ifdef __WXMSW__
  wxBuffer = new char[1500];
#else
  wxBuffer = new char[BUFSIZ + 512];
#endif

  wxClassInfo::InitializeClasses();

#if wxUSE_RESOURCES
//    wxGetResource(wxT("wxWindows"), wxT("OsVersion"), &wxOsVersion);
#endif

#if wxUSE_THREADS
    wxPendingEventsLocker = new wxCriticalSection;
#endif
  wxTheColourDatabase = new wxColourDatabase(wxKEY_STRING);
  wxTheColourDatabase->Initialize();

#ifdef __WXDEBUG__
#if wxUSE_LOG
    // flush the logged messages if any and install a 'safer' log target: the
    // default one (wxLogGui) can't be used after the resources are freed just
    // below and the user suppliedo ne might be even more unsafe (using any
    // wxWindows GUI function is unsafe starting from now)
    wxLog::DontCreateOnDemand();

    // this will flush the old messages if any
    delete wxLog::SetActiveTarget(new wxLogStderr);
#endif // wxUSE_LOG
#endif

  wxWinMacWindowList = new wxList(wxKEY_INTEGER);
  wxWinMacControlList = new wxList(wxKEY_INTEGER);

  wxInitializeStockLists();
  wxInitializeStockObjects();

#if wxUSE_WX_RESOURCES
  wxInitializeResourceSystem();
#endif

  wxBitmap::InitStandardHandlers();

  wxModule::RegisterModules();
  if (!wxModule::InitializeModules()) {
     return FALSE;
  }

  wxMacCreateNotifierTable() ;

  UMAShowArrowCursor() ;

  return TRUE;
}

void wxApp::CleanUp()
{
    wxToolTip::RemoveToolTips() ;
#if wxUSE_LOG
    // flush the logged messages if any and install a 'safer' log target: the
    // default one (wxLogGui) can't be used after the resources are freed just
    // below and the user suppliedo ne might be even more unsafe (using any
    // wxWindows GUI function is unsafe starting from now)
    wxLog::DontCreateOnDemand();

    // this will flush the old messages if any
    delete wxLog::SetActiveTarget(new wxLogStderr);
#endif // wxUSE_LOG

    // One last chance for pending objects to be cleaned up
    wxTheApp->DeletePendingObjects();

  wxModule::CleanUpModules();

#if wxUSE_WX_RESOURCES
  wxCleanUpResourceSystem();
#endif

  wxDeleteStockObjects() ;

    // Destroy all GDI lists, etc.
    wxDeleteStockLists();

  delete wxTheColourDatabase;
  wxTheColourDatabase = NULL;

  wxBitmap::CleanUpHandlers();

  delete[] wxBuffer;
  wxBuffer = NULL;

  wxMacDestroyNotifierTable() ;
  if (wxWinMacWindowList)
    delete wxWinMacWindowList ;

  if (wxWinMacControlList)
    delete wxWinMacControlList ;

    delete wxPendingEvents;
#if wxUSE_THREADS
    delete wxPendingEventsLocker;
    // If we don't do the following, we get an apparent memory leak.
    ((wxEvtHandler&) wxDefaultValidator).ClearEventLocker();
#endif

  wxClassInfo::CleanUpClasses();

#ifndef __DARWIN__
  #if __option(profile)
  ProfilerDump( "\papp.prof" ) ;
  ProfilerTerm() ;
  #endif
#endif

  delete wxTheApp;
  wxTheApp = NULL;

#if (defined(__WXDEBUG__) && wxUSE_MEMORY_TRACING) || wxUSE_DEBUG_CONTEXT
    // At this point we want to check if there are any memory
    // blocks that aren't part of the wxDebugContext itself,
    // as a special case. Then when dumping we need to ignore
    // wxDebugContext, too.
    if (wxDebugContext::CountObjectsLeft(TRUE) > 0)
    {
        wxLogDebug(wxT("There were memory leaks."));
        wxDebugContext::Dump();
        wxDebugContext::PrintStatistics();
    }
    //  wxDebugContext::SetStream(NULL, NULL);
#endif

#if wxUSE_LOG
    // do it as the very last thing because everything else can log messages
    delete wxLog::SetActiveTarget(NULL);
#endif // wxUSE_LOG

    UMACleanupToolbox() ;
    if (s_macCursorRgn)
        ::DisposeRgn((RgnHandle)s_macCursorRgn);

    #if 0
        TerminateAE() ;
    #endif
}

//----------------------------------------------------------------------
// wxEntry
//----------------------------------------------------------------------

short gCurrentResource = -1 ;

wxStAppResource::wxStAppResource()
{
    m_currentRefNum = CurResFile() ;
    if ( gCurrentResource != -1 )
    {
        UseResFile( gCurrentResource ) ;
    }
}

wxStAppResource::~wxStAppResource()
{
    if ( m_currentRefNum != -1 )
    {
        UseResFile( m_currentRefNum ) ;
    }
}

#ifdef WXMAKINGDLL

// for shared libraries we have to manually get the correct resource ref num upon
// initializing and releasing when terminating, therefore the __wxinitialize and __wxterminate
// must be used

#ifdef __cplusplus
extern "C" {
#endif

void __sinit(void);	/*	(generated by linker)	*/
pascal OSErr __initialize(const CFragInitBlock *theInitBlock);
pascal void __terminate(void);

#ifdef __cplusplus
}
#endif

pascal OSErr __wxinitialize(const CFragInitBlock *theInitBlock)
{
   gCurrentResource = -1;
   
   if (theInitBlock->fragLocator.where == kDataForkCFragLocator) {
      gCurrentResource = 
         FSpOpenResFile(theInitBlock->fragLocator.u.onDisk.fileSpec, 
                              fsRdPerm);
   }
   return __initialize( theInitBlock ) ;
}

pascal void __wxterminate(void)
{
    if (gCurrentResource != -1)
      CloseResFile(gCurrentResource);
    __terminate() ;
}
#endif

int WXDLLEXPORT wxEntryStart( int argc, char *argv[] )
{
    return wxApp::Initialize();
}


int WXDLLEXPORT wxEntryInitGui()
{
    return wxTheApp->OnInitGui();
}


void WXDLLEXPORT wxEntryCleanup()
{
    wxApp::CleanUp();
}


int wxEntry( int argc, char *argv[] , bool enterLoop )
{
    gCurrentResource = CurResFile() ;
#ifdef __MWERKS__
#if (defined(__WXDEBUG__) && wxUSE_MEMORY_TRACING) || wxUSE_DEBUG_CONTEXT
    // This seems to be necessary since there are 'rogue'
    // objects present at this point (perhaps global objects?)
    // Setting a checkpoint will ignore them as far as the
    // memory checking facility is concerned.
    // Of course you may argue that memory allocated in globals should be
    // checked, but this is a reasonable compromise.
    wxDebugContext::SetCheckpoint();
#endif
#endif
    if (!wxEntryStart(argc, argv)) {
        return 0;
    }
   // create the application object or ensure that one already exists
    if (!wxTheApp)
    {
        // The app may have declared a global application object, but we recommend
        // the IMPLEMENT_APP macro is used instead, which sets an initializer
        // function for delayed, dynamic app object construction.
        wxCHECK_MSG( wxApp::GetInitializerFunction(), 0,
                     wxT("No initializer - use IMPLEMENT_APP macro.") );

        wxTheApp = (wxApp*) (*wxApp::GetInitializerFunction()) ();
    }

    wxCHECK_MSG( wxTheApp, 0, wxT("You have to define an instance of wxApp!") );

#ifndef __DARWIN__
    argc = 0 ; // currently we don't support files as parameters
#endif
    // we could try to get the open apple events here to adjust argc and argv better

    wxTheApp->argc = argc;
    wxTheApp->argv = argv;

    // GUI-specific initialization, such as creating an app context.
    wxEntryInitGui();


    // Here frames insert themselves automatically
    // into wxTopLevelWindows by getting created
    // in OnInit().

    int retValue = 0;

    if ( wxTheApp->OnInit() )
    {
        if ( enterLoop )
        {
            retValue = wxTheApp->OnRun();
        }
        else
            // We want to initialize, but not run or exit immediately.
            return 1;
    }
    //else: app initialization failed, so we skipped OnRun()

    wxWindow *topWindow = wxTheApp->GetTopWindow();
    if ( topWindow )
    {
        // Forcibly delete the window.
        if ( topWindow->IsKindOf(CLASSINFO(wxFrame)) ||
                topWindow->IsKindOf(CLASSINFO(wxDialog)) )
        {
            topWindow->Close(TRUE);
            wxTheApp->DeletePendingObjects();
        }
        else
        {
            delete topWindow;
            wxTheApp->SetTopWindow(NULL);
        }
    }

    wxTheApp->OnExit();

    wxEntryCleanup();

    return retValue;
}

// Static member initialization
wxAppInitializerFunction wxAppBase::m_appInitFn = (wxAppInitializerFunction) NULL;

wxApp::wxApp()
{
  m_topWindow = NULL;
  wxTheApp = this;

  m_wantDebugOutput = TRUE ;

  argc = 0;
  argv = NULL;

  m_printMode = wxPRINT_WINDOWS;
  m_exitOnFrameDelete = TRUE;
  m_auto3D = TRUE;
}

bool wxApp::Initialized()
{
  if (GetTopWindow())
    return TRUE;
  else
    return FALSE;
}

int wxApp::MainLoop()
{
  m_keepGoing = TRUE;

  while (m_keepGoing)
  {
        MacDoOneEvent() ;
  }

  return 0;
}

// Returns TRUE if more time is needed.
bool wxApp::ProcessIdle()
{
    wxIdleEvent event;
    event.SetEventObject(this);
    ProcessEvent(event);

    return event.MoreRequested();
}

void wxApp::ExitMainLoop()
{
  m_keepGoing = FALSE;
}

// Is a message/event pending?
bool wxApp::Pending()
{
    EventRecord event ;

  return EventAvail( everyEvent , &event ) ;
}

// Dispatch a message.
void wxApp::Dispatch()
{
    MacDoOneEvent() ;
}

void wxApp::OnIdle(wxIdleEvent& event)
{
   static bool s_inOnIdle = FALSE;

    // Avoid recursion (via ProcessEvent default case)
    if ( s_inOnIdle )
        return;


  s_inOnIdle = TRUE;

  // 'Garbage' collection of windows deleted with Close().
  DeletePendingObjects();

  // flush the logged messages if any
  wxLog *pLog = wxLog::GetActiveTarget();
  if ( pLog != NULL && pLog->HasPendingMessages() )
    pLog->Flush();

  // Send OnIdle events to all windows
  bool needMore = SendIdleEvents();

  if (needMore)
    event.RequestMore(TRUE);

    // If they are pending events, we must process them: pending events are
    // either events to the threads other than main or events posted with
    // wxPostEvent() functions
    wxMacProcessNotifierAndPendingEvents();

  s_inOnIdle = FALSE;
}

void wxWakeUpIdle()
{
    wxMacWakeUp() ;
}

// Send idle event to all top-level windows
bool wxApp::SendIdleEvents()
{
    bool needMore = FALSE;
    wxNode* node = wxTopLevelWindows.First();
    while (node)
    {
        wxWindow* win = (wxWindow*) node->Data();
        if (SendIdleEvents(win))
            needMore = TRUE;

        node = node->Next();
    }
    return needMore;
}

// Send idle event to window and all subwindows
bool wxApp::SendIdleEvents(wxWindow* win)
{
    bool needMore = FALSE;

    wxIdleEvent event;
    event.SetEventObject(win);
    win->ProcessEvent(event);

    if (event.MoreRequested())
        needMore = TRUE;

    wxNode* node = win->GetChildren().First();
    while (node)
    {
        wxWindow* win = (wxWindow*) node->Data();
        if (SendIdleEvents(win))
            needMore = TRUE;

        node = node->Next();
    }
    return needMore ;
}

void wxApp::DeletePendingObjects()
{
  wxNode *node = wxPendingDelete.First();
  while (node)
  {
    wxObject *obj = (wxObject *)node->Data();

    delete obj;

    if (wxPendingDelete.Member(obj))
      delete node;

    // Deleting one object may have deleted other pending
    // objects, so start from beginning of list again.
    node = wxPendingDelete.First();
  }
}

wxIcon
wxApp::GetStdIcon(int which) const
{
    switch(which)
    {
        case wxICON_INFORMATION:
            return wxIcon("wxICON_INFO");

        case wxICON_QUESTION:
            return wxIcon("wxICON_QUESTION");

        case wxICON_EXCLAMATION:
            return wxIcon("wxICON_WARNING");

        default:
            wxFAIL_MSG(wxT("requested non existent standard icon"));
            // still fall through

        case wxICON_HAND:
            return wxIcon("wxICON_ERROR");
    }
}

void wxExit()
{
    wxLogError(_("Fatal error: exiting"));

    wxApp::CleanUp();
    ::ExitToShell() ;
}

void wxApp::OnEndSession(wxCloseEvent& WXUNUSED(event))
{
    if (GetTopWindow())
        GetTopWindow()->Close(TRUE);
}

// Default behaviour: close the application with prompts. The
// user can veto the close, and therefore the end session.
void wxApp::OnQueryEndSession(wxCloseEvent& event)
{
    if (GetTopWindow())
    {
        if (!GetTopWindow()->Close(!event.CanVeto()))
            event.Veto(TRUE);
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
    static bool s_inYield = FALSE;

    if (s_inYield)
    {
        if ( !onlyIfNeeded )
        {
            wxFAIL_MSG( wxT("wxYield called recursively" ) );
        }

        return FALSE;
    }

    s_inYield = TRUE;

#if wxUSE_THREADS
    YieldToAnyThread() ;
#endif
    EventRecord event ;

    long sleepTime = 1 ; //::GetCaretTime();

    while ( !wxTheApp->IsExiting() && WaitNextEvent(everyEvent, &event,sleepTime, (RgnHandle) wxApp::s_macCursorRgn))
    {
        wxTheApp->MacHandleOneEvent( &event );
        if ( event.what != kHighLevelEvent )
            SetRectRgn( (RgnHandle) wxApp::s_macCursorRgn , event.where.h , event.where.v ,  event.where.h + 1 , event.where.v + 1 ) ;
    }

    wxMacProcessNotifierAndPendingEvents() ;

    s_inYield = FALSE;

    return TRUE;
}

// platform specifics

void wxApp::MacSuspend( bool convertClipboard )
{
    // we have to deactive the top level windows manually

    wxNode* node = wxTopLevelWindows.First();
    while (node)
    {
        wxTopLevelWindow* win = (wxTopLevelWindow*) node->Data();
        win->MacActivate( MacGetCurrentEvent() , false ) ;

        node = node->Next();
    }

     s_lastMouseDown = 0 ;
     if( convertClipboard )
     {
         MacConvertPrivateToPublicScrap() ;
     }

     ::HideFloatingWindows() ;
}

void wxApp::MacResume( bool convertClipboard )
{
        s_lastMouseDown = 0 ;
        if( convertClipboard )
        {
            MacConvertPublicToPrivateScrap() ;
        }

        ::ShowFloatingWindows() ;
}

void wxApp::MacConvertPrivateToPublicScrap()
{
}

void wxApp::MacConvertPublicToPrivateScrap()
{
}

void wxApp::MacDoOneEvent()
{
  EventRecord event ;

    long sleepTime = 1; // GetCaretTime() / 4 ;

    if (WaitNextEvent(everyEvent, &event, sleepTime, (RgnHandle) s_macCursorRgn))
    {
        MacHandleOneEvent( &event );
    }
    else
    {
        // idlers
        WindowPtr window = ::FrontWindow() ;
        if ( window )
            ::IdleControls( window ) ;

        wxTheApp->ProcessIdle() ;
    }
    if ( event.what != kHighLevelEvent )
        SetRectRgn( (RgnHandle) s_macCursorRgn , event.where.h , event.where.v ,  event.where.h + 1 , event.where.v + 1 ) ;

    // repeaters

    DeletePendingObjects() ;
    wxMacProcessNotifierAndPendingEvents() ;
}

void wxApp::MacHandleOneEvent( WXEVENTREF evr )
{
    EventRecord* ev = (EventRecord*) evr ;
    m_macCurrentEvent = ev ;

    wxApp::sm_lastMessageTime = ev->when ;

    switch (ev->what)
    {
        case mouseDown:
            MacHandleMouseDownEvent( ev ) ;
            if ( ev->modifiers & controlKey )
                s_lastMouseDown = 2;
            else
                s_lastMouseDown = 1;
            break;
        case mouseUp:
            if ( s_lastMouseDown == 2 )
            {
                ev->modifiers |= controlKey ;
            }
            else
            {
                ev->modifiers &= ~controlKey ;
            }
            MacHandleMouseUpEvent( ev ) ;
            s_lastMouseDown = 0;
            break;
        case activateEvt:
            MacHandleActivateEvent( ev ) ;
            break;
        case updateEvt:
            MacHandleUpdateEvent( ev ) ;
            break;
        case keyDown:
        case autoKey:
            MacHandleKeyDownEvent( ev ) ;
            break;
        case keyUp:
            MacHandleKeyUpEvent( ev ) ;
            break;
        case diskEvt:
            MacHandleDiskEvent( ev ) ;
            break;
        case osEvt:
            MacHandleOSEvent( ev ) ;
            break;
        case kHighLevelEvent:
            MacHandleHighLevelEvent( ev ) ;
            break;
        default:
            break;
    }
    wxMacProcessNotifierAndPendingEvents() ;
}

void wxApp::MacHandleHighLevelEvent( WXEVENTREF evr )
{
    EventRecord* ev = (EventRecord*) evr ;
    ::AEProcessAppleEvent( ev ) ;
}

bool s_macIsInModalLoop = false ;

void wxApp::MacHandleMouseDownEvent( WXEVENTREF evr )
{
    EventRecord* ev = (EventRecord*) evr ;
    wxToolTip::RemoveToolTips() ;

    WindowRef window;
    WindowRef frontWindow = ::FrontNonFloatingWindow() ;
    WindowAttributes frontWindowAttributes = NULL ;
    if ( frontWindow )
        ::GetWindowAttributes( frontWindow , &frontWindowAttributes ) ;

    short windowPart = ::FindWindow(ev->where, &window);
    wxTopLevelWindowMac* win = wxFindWinFromMacWindow( window ) ;
    if ( wxPendingDelete.Member(win) )
        return ;

    BitMap screenBits;
    GetQDGlobalsScreenBits( &screenBits );

    switch (windowPart)
    {
        case inMenuBar :
            if ( s_macIsInModalLoop )
            {
                SysBeep ( 30 ) ;
            }
            else
            {
                UInt32 menuresult = MenuSelect(ev->where) ;
                MacHandleMenuSelect( HiWord( menuresult ) , LoWord( menuresult ) );
                s_lastMouseDown = 0;
            }
            break ;
#if !TARGET_CARBON
        case inSysWindow :
            SystemClick( ev , window ) ;
            s_lastMouseDown = 0;
            break ;
#endif
        case inDrag :
            if ( window != frontWindow && s_macIsInModalLoop && !(ev->modifiers & cmdKey ) )
            {
                SysBeep ( 30 ) ;
            }
            else
            {
                DragWindow(window, ev->where, &screenBits.bounds);
                if (win)
                {
                    GrafPtr port ;
                    GetPort( &port ) ;
                    Point pt = { 0, 0 } ;
                    SetPortWindowPort(window) ;
                    LocalToGlobal( &pt ) ;
                    SetPort( port ) ;
                        win->SetSize( pt.h , pt.v , -1 ,
                            -1 , wxSIZE_USE_EXISTING);
                }
                s_lastMouseDown = 0;
            }
            break ;
        case inGoAway:
            if (TrackGoAway(window, ev->where))
            {
                if ( win )
                    win->Close() ;
            }
            s_lastMouseDown = 0;
            break;
        case inGrow:
          {
                int growResult = GrowWindow(window , ev->where, &screenBits.bounds);
                if (growResult != 0)
                {
                    int newWidth = LoWord(growResult);
                    int newHeight = HiWord(growResult);
                    int oldWidth, oldHeight;


                    if (win)
                    {
                        win->GetSize(&oldWidth, &oldHeight);
                        if (newWidth == 0)
                            newWidth = oldWidth;
                        if (newHeight == 0)
                            newHeight = oldHeight;
                        win->SetSize( -1, -1, newWidth, newHeight, wxSIZE_USE_EXISTING);
                    }
                }
                s_lastMouseDown = 0;
          }
            break;
        case inZoomIn:
        case inZoomOut:
                if (TrackBox(window, ev->where, windowPart))
                {
                    // TODO setup size event
                    ZoomWindow( window , windowPart , false ) ;
                    if (win)
                    {
                        Rect tempRect ;

                        GetWindowPortBounds(window, &tempRect ) ;
                        win->SetSize( -1, -1, tempRect.right-tempRect.left ,
                            tempRect.bottom-tempRect.top, wxSIZE_USE_EXISTING);
                    }
                }
            s_lastMouseDown = 0;
            break;
        case inCollapseBox :
                // TODO setup size event
            s_lastMouseDown = 0;
            break ;

        case inContent :
                {
                    GrafPtr port ;
                    GetPort( &port ) ;
                    SetPortWindowPort(window) ;
                    SetPort( port ) ;
                }
                if ( window != frontWindow && wxTheApp->s_captureWindow == NULL )
                {
                    if ( s_macIsInModalLoop )
                    {
                        SysBeep ( 30 ) ;
                    }
                    else if ( UMAIsWindowFloating( window ) )
                    {
                        if ( win )
                            win->MacMouseDown( ev , windowPart ) ;
                    }
                    else
                    {
                        if ( win )
                            win->MacMouseDown( ev , windowPart ) ;
                        ::SelectWindow( window ) ;
                    }
                }
                else
                {
                    if ( win )
                        win->MacMouseDown( ev , windowPart ) ;
                }
            break ;

        default:
            break;
    }
}

void wxApp::MacHandleMouseUpEvent( WXEVENTREF evr )
{
    EventRecord* ev = (EventRecord*) evr ;
    WindowRef window;

    short windowPart = inNoWindow ;
	if ( wxTheApp->s_captureWindow )
	{
		window = (WindowRef) s_captureWindow->MacGetRootWindow() ;
		windowPart = inContent ;
	} 
	else
	{
		windowPart = ::FindWindow(ev->where, &window) ;
	}

    switch (windowPart)
    {
        case inMenuBar :
            break ;
        case inSysWindow :
            break ;
        default:
            {
                wxTopLevelWindowMac* win = wxFindWinFromMacWindow( window ) ;
                if ( win )
                    win->MacMouseUp( ev , windowPart ) ;
            }
            break;
    }
}

long wxMacTranslateKey(unsigned char key, unsigned char code) ;
long wxMacTranslateKey(unsigned char key, unsigned char code)
{
    long retval = key ;
    switch (key)
    {
        case 0x01 :
                 retval = WXK_HOME;
          break;
        case 0x03 :
                 retval = WXK_RETURN;
          break;
        case 0x04 :
                 retval = WXK_END;
          break;
        case 0x05 :
                 retval = WXK_HELP;
          break;
        case 0x08 :
                 retval = WXK_BACK;
          break;
        case 0x09 :
                 retval = WXK_TAB;
          break;
        case 0x0b :
                 retval = WXK_PAGEUP;
          break;
        case 0x0c :
                 retval = WXK_PAGEDOWN;
          break;
        case 0x0d :
                 retval = WXK_RETURN;
          break;
            case 0x10 :
            {
                switch( code )
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
                }
            }
            break ;
            case 0x1b :
                retval = WXK_ESCAPE ;
            break ;
            case 0x1c :
                retval = WXK_LEFT ;
            break ;
            case 0x1d :
                retval = WXK_RIGHT ;
            break ;
            case 0x1e :
                retval = WXK_UP ;
            break ;
            case 0x1f :
                retval = WXK_DOWN ;
            break ;
            case 0x7F :
                retval = WXK_DELETE ;
             default:
            break ;
     } // end switch

    return retval;
}

void wxApp::MacHandleKeyDownEvent( WXEVENTREF evr )
{
    EventRecord* ev = (EventRecord*) evr ;
    wxToolTip::RemoveToolTips() ;

    UInt32 menuresult = UMAMenuEvent(ev) ;
    if ( HiWord( menuresult ) )
    {
        if ( !s_macIsInModalLoop )
        MacHandleMenuSelect( HiWord( menuresult ) , LoWord( menuresult ) ) ;
    }
    else
    {
        short keycode ;
        short keychar ;
        keychar = short(ev->message & charCodeMask);
        keycode = short(ev->message & keyCodeMask) >> 8 ;
        long keyval = wxMacTranslateKey(keychar, keycode) ;
        bool handled = false ;
        wxWindow* focus = wxWindow::FindFocus() ;
        if ( focus )
        {

            wxKeyEvent event(wxEVT_KEY_DOWN);
            event.m_shiftDown = ev->modifiers & shiftKey;
            event.m_controlDown = ev->modifiers & controlKey;
            event.m_altDown = ev->modifiers & optionKey;
            event.m_metaDown = ev->modifiers & cmdKey;
            event.m_keyCode = keyval;
            event.m_x = ev->where.h;
            event.m_y = ev->where.v;
            event.m_timeStamp = ev->when;
            event.SetEventObject(focus);
            handled = focus->GetEventHandler()->ProcessEvent( event ) ;
            if ( !handled )
            {
                #if wxUSE_ACCEL
                if (!handled)
                {
                    wxWindow *ancestor = focus;
                    /*
                    while (ancestor)
                    {
                        int command = ancestor->GetAcceleratorTable()->GetCommand( event );
                        if (command != -1)
                        {
                            wxCommandEvent command_event( wxEVT_COMMAND_MENU_SELECTED, command );
                            handled = ancestor->GetEventHandler()->ProcessEvent( command_event );
                            break;
                        }
                        if (ancestor->m_isFrame)
                            break;
                        ancestor = ancestor->GetParent();
                    }
                    */
                }
                #endif // wxUSE_ACCEL
            }
            if (!handled)
            {
                wxKeyEvent event(wxEVT_CHAR);
                event.m_shiftDown = ev->modifiers & shiftKey;
                event.m_controlDown = ev->modifiers & controlKey;
                event.m_altDown = ev->modifiers & optionKey;
                event.m_metaDown = ev->modifiers & cmdKey;
                event.m_keyCode = keyval;
                event.m_x = ev->where.h;
                event.m_y = ev->where.v;
                event.m_timeStamp = ev->when;
                event.SetEventObject(focus);
                handled = focus->GetEventHandler()->ProcessEvent( event ) ;
            }
            if ( !handled &&
                 (keyval == WXK_TAB) &&
                 (!focus->HasFlag(wxTE_PROCESS_TAB)) &&
                 (focus->GetParent()) &&
                 (focus->GetParent()->HasFlag( wxTAB_TRAVERSAL)) )
            {
                wxNavigationKeyEvent new_event;
                new_event.SetEventObject( focus );
                new_event.SetDirection( !event.ShiftDown() );
                /* CTRL-TAB changes the (parent) window, i.e. switch notebook page */
                new_event.SetWindowChange( event.ControlDown() );
                new_event.SetCurrentFocus( focus );
                handled = focus->GetEventHandler()->ProcessEvent( new_event );
            }
        }
        if ( !handled )
        {
            // if window is not having a focus still testing for default enter or cancel
            // TODO add the UMA version for ActiveNonFloatingWindow
          focus = wxFindWinFromMacWindow( FrontWindow() ) ;
          if ( focus )
          {
            if ( keyval == WXK_RETURN )
            {
                 wxButton *def = wxDynamicCast(focus->GetDefaultItem(),
                                                       wxButton);
                 if ( def && def->IsEnabled() )
                 {
                     wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, def->GetId() );
                     event.SetEventObject(def);
                     def->Command(event);
                     return ;
                }
            }
            /* generate wxID_CANCEL if command-. or <esc> has been pressed (typically in dialogs) */
            else if (keyval == WXK_ESCAPE || (keyval == '.' && ev->modifiers & cmdKey ) )
            {
                  wxCommandEvent new_event(wxEVT_COMMAND_BUTTON_CLICKED,wxID_CANCEL);
                  new_event.SetEventObject( focus );
                  handled = focus->GetEventHandler()->ProcessEvent( new_event );
            }
          }
        }
    }
}

void wxApp::MacHandleKeyUpEvent( WXEVENTREF evr )
{
    EventRecord* ev = (EventRecord*) evr ;
    wxToolTip::RemoveToolTips() ;

    UInt32 menuresult = UMAMenuEvent(ev) ;
    if ( HiWord( menuresult ) )
    {
    }
    else
    {
        short keycode ;
        short keychar ;
        keychar = short(ev->message & charCodeMask);
        keycode = short(ev->message & keyCodeMask) >> 8 ;

        wxWindow* focus = wxWindow::FindFocus() ;
        if ( focus )
        {
            long keyval = wxMacTranslateKey(keychar, keycode) ;

            wxKeyEvent event(wxEVT_KEY_UP);
            event.m_shiftDown = ev->modifiers & shiftKey;
            event.m_controlDown = ev->modifiers & controlKey;
            event.m_altDown = ev->modifiers & optionKey;
            event.m_metaDown = ev->modifiers & cmdKey;
            event.m_keyCode = keyval;
            event.m_x = ev->where.h;
            event.m_y = ev->where.v;
            event.m_timeStamp = ev->when;
            event.SetEventObject(focus);
            bool handled = focus->GetEventHandler()->ProcessEvent( event ) ;
        }
    }
}

void wxApp::MacHandleActivateEvent( WXEVENTREF evr )
{
    EventRecord* ev = (EventRecord*) evr ;
    WindowRef window = (WindowRef) ev->message ;
    if ( window )
    {
        bool activate = (ev->modifiers & activeFlag ) ;
        WindowClass wclass ;
        ::GetWindowClass ( window , &wclass ) ;
        if ( wclass == kFloatingWindowClass )
        {
            // if it is a floater we activate/deactivate the front non-floating window instead
            window = ::FrontNonFloatingWindow() ;
        }
        wxTopLevelWindowMac* win = wxFindWinFromMacWindow( window ) ;
        if ( win )
            win->MacActivate( ev , activate ) ;
    }
}

void wxApp::MacHandleUpdateEvent( WXEVENTREF evr )
{
    EventRecord* ev = (EventRecord*) evr ;
    WindowRef window = (WindowRef) ev->message ;
    wxTopLevelWindowMac * win = wxFindWinFromMacWindow( window ) ;
    if ( win )
    {
        if ( !wxPendingDelete.Member(win) )
            win->MacUpdate( ev->when ) ;
    }
    else
    {
        // since there is no way of telling this foreign window to update itself
        // we have to invalidate the update region otherwise we keep getting the same
        // event over and over again
        BeginUpdate( window ) ;
        EndUpdate( window ) ;
    }
}

void wxApp::MacHandleDiskEvent( WXEVENTREF evr )
{
    EventRecord* ev = (EventRecord*) evr ;
    if ( HiWord( ev->message ) != noErr )
  {
 #if !TARGET_CARBON
        OSErr err ;
        Point point ;
         SetPt( &point , 100 , 100 ) ;

          err = DIBadMount( point , ev->message ) ;
        wxASSERT( err == noErr ) ;
#endif
    }
}

void wxApp::MacHandleOSEvent( WXEVENTREF evr )
{
    EventRecord* ev = (EventRecord*) evr ;
    switch( ( ev->message & osEvtMessageMask ) >> 24 )
    {
        case suspendResumeMessage :
            {
                bool isResuming = ev->message & resumeFlag ;
#if !TARGET_CARBON
                bool convertClipboard = ev->message & convertClipboardFlag ;
#else
                bool convertClipboard = false;
#endif
                bool doesActivate = UMAGetProcessModeDoesActivateOnFGSwitch() ;
                if ( isResuming )
                {
                    WindowRef oldFrontWindow = NULL ;
                    WindowRef newFrontWindow = NULL ;

                    // in case we don't take care of activating ourselves, we have to synchronize
                    // our idea of the active window with the process manager's - which it already activated

                    if ( !doesActivate )
                        oldFrontWindow = ::FrontNonFloatingWindow() ;

                    MacResume( convertClipboard ) ;

                    newFrontWindow = ::FrontNonFloatingWindow() ;

                    if ( oldFrontWindow )
                    {
                        wxTopLevelWindowMac* win = wxFindWinFromMacWindow( oldFrontWindow ) ;
                        if ( win )
                            win->MacActivate( ev , false ) ;
                    }
                    if ( newFrontWindow )
                    {
                        wxTopLevelWindowMac* win = wxFindWinFromMacWindow( newFrontWindow ) ;
                        if ( win )
                            win->MacActivate( ev , true ) ;
                    }
                }
                else
                {
                    MacSuspend( convertClipboard ) ;

                    // in case this suspending did close an active window, another one might
                    // have surfaced -> lets deactivate that one

/* TODO : find out what to do on systems < 10 , perhaps FrontNonFloatingWindow
                    WindowRef newActiveWindow = ::ActiveNonFloatingWindow() ;
                    if ( newActiveWindow )
                    {
                        wxWindow* win = wxFindWinFromMacWindow( newActiveWindow ) ;
                        if ( win )
                            win->MacActivate( ev , false ) ;
                    }
*/
                }
            }
            break ;
        case mouseMovedMessage :
            {
                WindowRef window;

                wxWindow* currentMouseWindow = NULL ;

                wxWindow::MacGetWindowFromPoint( wxPoint( ev->where.h , ev->where.v ) ,
                                                 &currentMouseWindow ) ;
                if ( currentMouseWindow != wxWindow::s_lastMouseWindow )
                {
                    wxMouseEvent event ;

                    bool isDown = !(ev->modifiers & btnState) ; // 1 is for up
                    bool controlDown = ev->modifiers & controlKey ; // for simulating right mouse

                    event.m_leftDown = isDown && !controlDown;
                    event.m_middleDown = FALSE;
                    event.m_rightDown = isDown && controlDown;
                    event.m_shiftDown = ev->modifiers & shiftKey;
                    event.m_controlDown = ev->modifiers & controlKey;
                    event.m_altDown = ev->modifiers & optionKey;
                    event.m_metaDown = ev->modifiers & cmdKey;
                    event.m_x = ev->where.h;
                    event.m_y = ev->where.v;
                    event.m_timeStamp = ev->when;
                    event.SetEventObject(this);
                    
                    if ( wxWindow::s_lastMouseWindow )
                    {
                        wxMouseEvent eventleave(event);
                        eventleave.SetEventType( wxEVT_LEAVE_WINDOW );
                        wxWindow::s_lastMouseWindow->ScreenToClient( &eventleave.m_x, &eventleave.m_y );
                        
                        wxWindow::s_lastMouseWindow->GetEventHandler()->ProcessEvent(eventleave);
                    }
                    if ( currentMouseWindow )
                    {
                        wxMouseEvent evententer(event);
                        evententer.SetEventType( wxEVT_ENTER_WINDOW );
                        currentMouseWindow->ScreenToClient( &evententer.m_x, &evententer.m_y );
                        
                        currentMouseWindow->GetEventHandler()->ProcessEvent(evententer);
                    }
                    wxWindow::s_lastMouseWindow = currentMouseWindow ;
                }

                short windowPart = ::FindWindow(ev->where, &window);

                switch (windowPart)
                {
                    // fixes for setting the cursor back from dominic mazzoni
                    case inMenuBar :
                        UMAShowArrowCursor();
                        break ;
                    case inSysWindow :
                        UMAShowArrowCursor();
                        break ;
                    default:
                        {
                            // if ( s_lastMouseDown == 0 )
                            //     ev->modifiers |= btnState ;
                                
                            // Calling GetNextEvent with a zero event mask will always
                            // pass back a null event. However, it fills the EventRecord
                            // with the state of the modifier keys. This is needed since
                            // the modifier state returned by WaitForNextEvent often is
                            // wrong mouse move events. The attempt above to correct this
                            // didn't always work (under OS X at least).
            
                            EventRecord tmp;
                            ::GetNextEvent(0, &tmp);
                            ev->modifiers = tmp.modifiers;
                            
                            wxTopLevelWindowMac* win = wxFindWinFromMacWindow( window ) ;
                            if ( win )
                                win->MacMouseMoved( ev , windowPart ) ;
                            else
                                UMAShowArrowCursor();

                        }
                        break;
                }
            }
            break ;

    }
}

void wxApp::MacHandleMenuSelect( int macMenuId , int macMenuItemNum )
{
    if (macMenuId == 0)
         return; // no menu item selected

    if (macMenuId == kwxMacAppleMenuId && macMenuItemNum > 1)
    {
        #if ! TARGET_CARBON
        Str255        deskAccessoryName ;
        GrafPtr        savedPort ;

        GetMenuItemText(GetMenuHandle(kwxMacAppleMenuId), macMenuItemNum, deskAccessoryName);
        GetPort(&savedPort);
        OpenDeskAcc(deskAccessoryName);
        SetPort(savedPort);
        #endif
    }
    else
    {
        wxWindow* frontwindow = wxFindWinFromMacWindow( ::FrontWindow() )  ;
        if ( frontwindow && wxMenuBar::MacGetInstalledMenuBar() )
            wxMenuBar::MacGetInstalledMenuBar()->MacMenuSelect( frontwindow->GetEventHandler() , 0 , macMenuId , macMenuItemNum ) ;
    }
    HiliteMenu(0);
}

/*
long wxApp::MacTranslateKey(char key, int mods)
{
}

void wxApp::MacAdjustCursor()
{
}

*/
/*
void
wxApp::macAdjustCursor()
{
  if (ev->what != kHighLevelEvent)
  {
    wxWindow* theMacWxFrame = wxFrame::MacFindFrameOrDialog(::FrontWindow());
    if (theMacWxFrame)
    {
        if (!theMacWxFrame->MacAdjustCursor(ev->where))
        ::SetCursor(&(qd.arrow));
      }
  }
}
*/
