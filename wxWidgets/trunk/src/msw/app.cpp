/////////////////////////////////////////////////////////////////////////////
// Name:        app.cpp
// Purpose:     wxApp
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
  #pragma implementation "app.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#if defined(__BORLANDC__)
  #pragma hdrstop
#endif

#ifndef WX_PRECOMP
  #include "wx/frame.h"
  #include "wx/app.h"
  #include "wx/utils.h"
  #include "wx/gdicmn.h"
  #include "wx/pen.h"
  #include "wx/brush.h"
  #include "wx/cursor.h"
  #include "wx/icon.h"
  #include "wx/palette.h"
  #include "wx/dc.h"
  #include "wx/dialog.h"
  #include "wx/msgdlg.h"
#endif

#include "wx/msw/private.h"
#include "wx/postscrp.h"
#include "wx/log.h"
#include "wx/module.h"

#if wxUSE_WX_RESOURCES
  #include "wx/resource.h"
#endif

#include <string.h>
#include <ctype.h>

#if defined(__WIN95__) && !defined(__GNUWIN32__)
  #include <commctrl.h>
#endif

// use debug CRT functions for memory leak detections in VC++
/* This still doesn't work for me, Vadim.
#if defined(__WXDEBUG__) && defined(_MSC_VER)
  // VC++ uses this macro as debug/release mode indicator
  #ifndef _DEBUG
    #define _DEBUG
  #endif

  #include <crtdbg.h>
#endif
*/

extern char *wxBuffer;
extern char *wxOsVersion;
extern wxList *wxWinHandleList;
extern wxList wxPendingDelete;
extern void wxSetKeyboardHook(bool doIt);
extern wxCursor *g_globalCursor;

HINSTANCE wxhInstance = 0;
static MSG s_currentMsg;
wxApp *wxTheApp = NULL;

// @@ why not const? and not static?
char wxFrameClassName[]         = "wxFrameClass";
char wxMDIFrameClassName[]      = "wxMDIFrameClass";
char wxMDIChildFrameClassName[] = "wxMDIChildFrameClass";
char wxPanelClassName[]         = "wxPanelClass";
char wxCanvasClassName[]        = "wxCanvasClass";

HICON wxSTD_FRAME_ICON = NULL;
HICON wxSTD_MDICHILDFRAME_ICON = NULL;
HICON wxSTD_MDIPARENTFRAME_ICON = NULL;

HICON wxDEFAULT_FRAME_ICON = NULL;
HICON wxDEFAULT_MDICHILDFRAME_ICON = NULL;
HICON wxDEFAULT_MDIPARENTFRAME_ICON = NULL;

HBRUSH wxDisableButtonBrush = 0;

LRESULT APIENTRY wxWndProc(HWND, UINT, WPARAM, LPARAM);

#if !USE_SHARED_LIBRARY
  IMPLEMENT_DYNAMIC_CLASS(wxApp, wxEvtHandler)

  BEGIN_EVENT_TABLE(wxApp, wxEvtHandler)
      EVT_IDLE(wxApp::OnIdle)
  END_EVENT_TABLE()
#endif

long wxApp::sm_lastMessageTime = 0;

#ifdef __WIN95__
  static HINSTANCE gs_hRichEdit = NULL;
#endif

//// Initialize

bool wxApp::Initialize()
{
  wxBuffer = new char[1500];

/*
  #if defined(__WXDEBUG__) && defined(_MSC_VER)
    // do check for memory leaks on program exit
    // (another useful flag is _CRTDBG_DELAY_FREE_MEM_DF which doesn't free
    //  deallocated memory which may be used to simulate low-memory condition)
    _CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
  #endif // debug build under MS VC++
*/

  #if (WXDEBUG && wxUSE_MEMORY_TRACING) || wxUSE_DEBUG_CONTEXT
    #if defined(_WINDLL)
      streambuf* sBuf = NULL;
    #else  // EXE
      streambuf* sBuf = new wxDebugStreamBuf;
    #endif // DLL

    ostream* oStr = new ostream(sBuf) ;
    wxDebugContext::SetStream(oStr, sBuf);
  #endif  // wxUSE_MEMORY_TRACING

  wxClassInfo::InitializeClasses();

  #if wxUSE_RESOURCES
    wxGetResource("wxWindows", "OsVersion", &wxOsVersion);
  #endif

  wxTheColourDatabase = new wxColourDatabase(wxKEY_STRING);
  wxTheColourDatabase->Initialize();

  wxInitializeStockLists();
  wxInitializeStockObjects();

  #if wxUSE_WX_RESOURCES
    wxInitializeResourceSystem();
  #endif

  // For PostScript printing
  #if wxUSE_POSTSCRIPT
    wxInitializePrintSetupData();
    wxThePrintPaperDatabase = new wxPrintPaperDatabase;
    wxThePrintPaperDatabase->CreateDatabase();
  #endif

  wxBitmap::InitStandardHandlers();

  #if defined(__WIN95__)
    InitCommonControls();
    gs_hRichEdit = LoadLibrary("RICHED32.DLL");

    if (gs_hRichEdit == NULL)
    {
      wxMessageBox("Could not initialise Rich Edit DLL");
    }
  #endif

  #if  defined(WX_DRAG_DROP)
    // we need to initialize OLE library
    if ( FAILED(::OleInitialize(NULL)) )
      wxFatalError(_("Cannot initialize OLE"));
  #endif

  #if CTL3D
    if (!Ctl3dRegister(wxhInstance))
      wxFatalError("Cannot register CTL3D");

    Ctl3dAutoSubclass(wxhInstance);
  #endif

  g_globalCursor = new wxCursor;

  wxSTD_FRAME_ICON = LoadIcon(wxhInstance, "wxSTD_FRAME");
  wxSTD_MDIPARENTFRAME_ICON = LoadIcon(wxhInstance, "wxSTD_MDIPARENTFRAME");
  wxSTD_MDICHILDFRAME_ICON = LoadIcon(wxhInstance, "wxSTD_MDICHILDFRAME");

  wxDEFAULT_FRAME_ICON = LoadIcon(wxhInstance, "wxDEFAULT_FRAME");
  wxDEFAULT_MDIPARENTFRAME_ICON = LoadIcon(wxhInstance, "wxDEFAULT_MDIPARENTFRAME");
  wxDEFAULT_MDICHILDFRAME_ICON = LoadIcon(wxhInstance, "wxDEFAULT_MDICHILDFRAME");

  RegisterWindowClasses();

  // Create the brush for disabling bitmap buttons

  LOGBRUSH lb ;
  lb.lbStyle = BS_PATTERN;
  lb.lbHatch = (int)LoadBitmap( wxhInstance, "wxDISABLE_BUTTON_BITMAP" ) ;
  wxDisableButtonBrush = ::CreateBrushIndirect( & lb ) ;
  ::DeleteObject( (HGDIOBJ)lb.lbHatch ) ;

  #if wxUSE_PENWINDOWS
    wxRegisterPenWin();
  #endif

  wxWinHandleList = new wxList(wxKEY_INTEGER);

  // This is to foil optimizations in Visual C++ that
  // throw out dummy.obj.
  #if (_MSC_VER >= 800) && !defined(WXMAKINGDLL)
    extern char wxDummyChar;
    if (wxDummyChar) wxDummyChar++;
  #endif

  wxSetKeyboardHook(TRUE);

  wxModule::RegisterModules();
  if (!wxModule::InitializeModules())
    return FALSE;
  return TRUE;
}

//// RegisterWindowClasses

bool wxApp::RegisterWindowClasses()
{
///////////////////////////////////////////////////////////////////////
// Register the frame window class.
  WNDCLASS wndclass;   // Structure used to register Windows class.

  wndclass.style         = CS_HREDRAW | CS_VREDRAW;
  wndclass.lpfnWndProc   = (WNDPROC)wxWndProc;
  wndclass.cbClsExtra    = 0;
  wndclass.cbWndExtra    = sizeof( DWORD ); // was 4
  wndclass.hInstance     = wxhInstance;
  wndclass.hIcon         = NULL;        // wxSTD_FRAME_ICON;
  wndclass.hCursor       = LoadCursor( NULL, IDC_ARROW );
  wndclass.hbrBackground =  (HBRUSH)(COLOR_APPWORKSPACE+1) ;
//  wndclass.hbrBackground = GetStockObject( WHITE_BRUSH );
  wndclass.lpszMenuName  = NULL;
#ifdef _MULTIPLE_INSTANCES
  sprintf( wxFrameClassName,"wxFrameClass%d", wxhInstance );
#endif
  wndclass.lpszClassName = wxFrameClassName;

  if (!RegisterClass( &wndclass ))
  {
    // wxFatalError("Can't register Frame Window class");
  }

///////////////////////////////////////////////////////////////////////
// Register the MDI frame window class.
  WNDCLASS wndclass1;   // Structure used to register Windows class.

  wndclass1.style         = CS_HREDRAW | CS_VREDRAW;
  wndclass1.lpfnWndProc   = (WNDPROC)wxWndProc;
  wndclass1.cbClsExtra    = 0;
  wndclass1.cbWndExtra    = sizeof( DWORD ); // was 4
  wndclass1.hInstance     = wxhInstance;
  wndclass1.hIcon         = NULL; // wxSTD_MDIPARENTFRAME_ICON;
  wndclass1.hCursor       = LoadCursor( NULL, IDC_ARROW );
//  wndclass1.hbrBackground =  (HBRUSH)(COLOR_APPWORKSPACE+1) ;
  wndclass1.hbrBackground = NULL;
  wndclass1.lpszMenuName  = NULL;

  wndclass1.lpszClassName = wxMDIFrameClassName;
  if (!RegisterClass( &wndclass1 ))
  {
//    wxFatalError("Can't register MDI Frame window class");
//  return FALSE;
  }

///////////////////////////////////////////////////////////////////////
// Register the MDI child frame window class.
  WNDCLASS wndclass4;   // Structure used to register Windows class.

  wndclass4.style         = CS_HREDRAW | CS_VREDRAW;
  wndclass4.lpfnWndProc   = (WNDPROC)wxWndProc;
  wndclass4.cbClsExtra    = 0;
  wndclass4.cbWndExtra    = sizeof( DWORD ); // was 4
  wndclass4.hInstance     = wxhInstance;
  wndclass4.hIcon         = NULL;       // wxSTD_MDICHILDFRAME_ICON;
  wndclass4.hCursor       = LoadCursor( NULL, IDC_ARROW );
  // TODO: perhaps this should be NULL so that Windows doesn't
  // paint the background itself (would OnEraseBackground duplicate
  // this?)
  wndclass4.hbrBackground =  (HBRUSH)(COLOR_WINDOW+1) ;
//  wndclass4.hbrBackground = NULL;
  wndclass4.lpszMenuName  = NULL;
  wndclass4.lpszClassName = wxMDIChildFrameClassName;

  if (!RegisterClass( &wndclass4 ))
  {
//   wxFatalError("Can't register MDI child frame window class");
//   return FALSE;
  }

///////////////////////////////////////////////////////////////////////
// Register the panel window class.
  WNDCLASS wndclass2;   // Structure used to register Windows class.
  memset(&wndclass2, 0, sizeof(WNDCLASS));   // start with NULL defaults
  // Use CS_OWNDC to avoid messing about restoring the context
  // for every graphic operation.
  wndclass2.style         = CS_HREDRAW | CS_VREDRAW;
  wndclass2.lpfnWndProc   = (WNDPROC)wxWndProc;
  wndclass2.cbClsExtra    = 0;
  wndclass2.cbWndExtra    = sizeof( DWORD ); // was 4
  wndclass2.hInstance     = wxhInstance;
  wndclass2.hIcon         = NULL;
  wndclass2.hCursor       = NULL;
//  wndclass2.hbrBackground = (HBRUSH)(COLOR_BTNFACE+1) ;
  wndclass2.hbrBackground = (HBRUSH) GetStockObject( LTGRAY_BRUSH );
  wndclass2.lpszMenuName  = NULL;
  wndclass2.lpszClassName = wxPanelClassName;
  if (!RegisterClass( &wndclass2 ))
  {
//   wxFatalError("Can't register Panel Window class");
//   return FALSE;
  }

///////////////////////////////////////////////////////////////////////
// Register the canvas and textsubwindow class name
  WNDCLASS wndclass3;   // Structure used to register Windows class.
  memset(&wndclass3, 0, sizeof(WNDCLASS));   // start with NULL defaults
  // Use CS_OWNDC to avoid messing about restoring the context
  // for every graphic operation.
//  wndclass3.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS ;
  // wxWin 2.0
  wndclass3.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS ;
  wndclass3.lpfnWndProc   = (WNDPROC)wxWndProc;
  wndclass3.cbClsExtra    = 0;
  wndclass3.cbWndExtra    = sizeof( DWORD ); // was 4
  wndclass3.hInstance     = wxhInstance;
  wndclass3.hIcon         = NULL;
  wndclass3.hCursor       = NULL;
//  wndclass3.hbrBackground = (HBRUSH)(COLOR_WINDOW+1) ;
  wndclass3.hbrBackground = NULL;
  wndclass3.lpszMenuName  = NULL;
  wndclass3.lpszClassName = wxCanvasClassName;
  if (!RegisterClass( &wndclass3))
  {
//   wxFatalError("Can't register Canvas class");
//   return FALSE;
  }

  return TRUE;
}

//// Convert Windows to argc, argv style

// FIXME this code should be rewritten (use wxArrayString instead...)
void wxApp::ConvertToStandardCommandArgs(char* lpCmdLine)
{
  // Split command line into tokens, as in usual main(argc, argv)
  char **command = new char*[50]; // VZ: sure? why not 25 or 73 and a half??

  int count = 0;
  char *buf = new char[strlen(lpCmdLine) + 1];

  /* Model independent strcpy */
  int i;
  for (i = 0; (buf[i] = lpCmdLine[i]) != 0; i++)
  {
    /* loop */;
  }

  // Get application name
  char name[260]; // 260 is MAX_PATH value from windef.h
  ::GetModuleFileName(wxhInstance, name, WXSIZEOF(name));

  // Is it only 16-bit Borland that already copies the program name
  // to the first argv index?
#if !defined(__GNUWIN32__)
// #if ! (defined(__BORLANDC__) && !defined(__WIN32__))
  command[count++] = copystring(name);
// #endif
#endif

  strcpy(name, wxFileNameFromPath(name));
  wxStripExtension(name);
  wxTheApp->SetAppName(name);

  /* Break up string */
  // Treat strings enclosed in double-quotes as single arguments
  char* str = buf;
  while (*str)
  {
    if ( count == WXSIZEOF(command) )
    {
      wxFAIL_MSG("too many command line args.");
      break;
    }

    while ( *str && isspace(*str) )  // skip whitespace
      str++;

    if (*str == '"')
    {
      str++;
      command[count++] = str;
      while (*str && *str != '"')
        str++;
    }
    else if (*str)
    {
      command[count++] = str;
      while (*str && !isspace(*str))
        str++;
    }
    if (*str)
      *str++ = '\0';
  }

  wxTheApp->argv = new char*[count + 1];
  wxTheApp->argv[count] = NULL; /* argv[] is NULL terminated list! */
  wxTheApp->argc = count;

  for (i = 0; i < count; i++)
  {
    wxTheApp->argv[i] = copystring(command[i]);

    delete [] command[i];
  }

  delete [] command;
  delete [] buf;
}

//// Cleans up any wxWindows internal structures left lying around

void wxApp::CleanUp()
{
  //// COMMON CLEANUP
  wxModule::CleanUpModules();

#if wxUSE_WX_RESOURCES
  wxCleanUpResourceSystem();

//  wxDefaultResourceTable->ClearTable();
#endif

  // Indicate that the cursor can be freed,
  // so that cursor won't be deleted by deleting
  // the bitmap list before g_globalCursor goes out
  // of scope (double deletion of the cursor).
  wxSetCursor(wxNullCursor);
  delete g_globalCursor;

  wxDeleteStockObjects() ;

  // Destroy all GDI lists, etc.
  wxDeleteStockLists();

  delete wxTheColourDatabase;
  wxTheColourDatabase = NULL;

#if wxUSE_POSTSCRIPT
  wxInitializePrintSetupData(FALSE);
  delete wxThePrintPaperDatabase;
  wxThePrintPaperDatabase = NULL;
#endif

  wxBitmap::CleanUpHandlers();

  delete[] wxBuffer;
  wxBuffer = NULL;

  //// WINDOWS-SPECIFIC CLEANUP

  wxSetKeyboardHook(FALSE);

#ifdef __WIN95__
  if (gs_hRichEdit != NULL)
    FreeLibrary(gs_hRichEdit);
#endif

#if wxUSE_PENWINDOWS
  wxCleanUpPenWin();
#endif

  if (wxSTD_FRAME_ICON)
    DestroyIcon(wxSTD_FRAME_ICON);
  if (wxSTD_MDICHILDFRAME_ICON)
    DestroyIcon(wxSTD_MDICHILDFRAME_ICON);
  if (wxSTD_MDIPARENTFRAME_ICON)
    DestroyIcon(wxSTD_MDIPARENTFRAME_ICON);

  if (wxDEFAULT_FRAME_ICON)
    DestroyIcon(wxDEFAULT_FRAME_ICON);
  if (wxDEFAULT_MDICHILDFRAME_ICON)
    DestroyIcon(wxDEFAULT_MDICHILDFRAME_ICON);
  if (wxDEFAULT_MDIPARENTFRAME_ICON)
    DestroyIcon(wxDEFAULT_MDIPARENTFRAME_ICON);

  if ( wxDisableButtonBrush )
    ::DeleteObject( wxDisableButtonBrush ) ;

#if     defined(WX_DRAG_DROP)
  ::OleUninitialize();
#endif

#if CTL3D
  Ctl3dUnregister(wxhInstance);
#endif

  if (wxWinHandleList)
    delete wxWinHandleList ;

  wxClassInfo::CleanUpClasses();

  // do it as the very last thing because everything else can log messages
  wxLog::DontCreateOnDemand();
  delete wxLog::SetActiveTarget(NULL);
}

#if !defined(_WINDLL) || (defined(_WINDLL) && defined(WXMAKINGDLL))

//// Main wxWindows entry point
int wxEntry(WXHINSTANCE hInstance,
            WXHINSTANCE WXUNUSED(hPrevInstance),
            char *lpCmdLine,
            int nCmdShow,
            bool enterLoop)
{
#ifndef __WXDEBUG__ // take everything into a try-except block in release build
  __try {
#endif

  wxhInstance = (HINSTANCE) hInstance;

  if (!wxApp::Initialize())
    return 0;

  // create the application object or ensure that one already exists
  if (!wxTheApp)
  {
    // The app may have declared a global application object, but we recommend
    // the IMPLEMENT_APP macro is used instead, which sets an initializer
    // function for delayed, dynamic app object construction.
    wxCHECK_MSG( wxApp::GetInitializerFunction(), 0,
                 "No initializer - use IMPLEMENT_APP macro." );

    wxTheApp = (*wxApp::GetInitializerFunction()) ();
  }

  wxCHECK_MSG( wxTheApp, 0, "You have to define an instance of wxApp!" );

  // save the WinMain() parameters
  wxTheApp->ConvertToStandardCommandArgs(lpCmdLine);
  wxTheApp->m_nCmdShow = nCmdShow;

  // GUI-specific initialisation. In fact on Windows we don't have any,
  // but this call is provided for compatibility across platforms.
  wxTheApp->OnInitGui() ;

  int retValue = 0;

  if ( wxTheApp->OnInit() )
  {
      if ( enterLoop )
      {
          retValue = wxTheApp->OnRun();
      }
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
  wxApp::CleanUp();

  delete wxTheApp;
  wxTheApp = NULL;

#if (WXDEBUG && wxUSE_MEMORY_TRACING) || wxUSE_DEBUG_CONTEXT
  // At this point we want to check if there are any memory
  // blocks that aren't part of the wxDebugContext itself,
  // as a special case. Then when dumping we need to ignore
  // wxDebugContext, too.
  if (wxDebugContext::CountObjectsLeft() > 0)
  {
      wxTrace("There were memory leaks.\n");
      wxDebugContext::Dump();
      wxDebugContext::PrintStatistics();
  }
  wxDebugContext::SetStream(NULL, NULL);
#endif

  return retValue;
#ifndef __WXDEBUG__ // catch exceptions only in release build
  }
  __except ( EXCEPTION_EXECUTE_HANDLER ) {
    /*
    if ( wxTheApp )
      wxTheApp->OnFatalException();
    */

    ::ExitProcess(3); // the same exit code as abort()
  }
#endif //debug
}

#else /*  _WINDLL  */

//// Entry point for DLLs

int wxEntry(WXHINSTANCE hInstance)
{
  wxhInstance = (HINSTANCE) hInstance;
  wxApp::Initialize();

  // The app may have declared a global application object, but we recommend
  // the IMPLEMENT_APP macro is used instead, which sets an initializer function
  // for delayed, dynamic app object construction.

  if (!wxTheApp)
  {
    if (!wxApp::GetInitializerFunction())
    {
      MessageBox(NULL, "No initializer - use IMPLEMENT_APP macro.", "wxWindows Error", MB_APPLMODAL | MB_ICONSTOP | MB_OK);
    return 0;
    }

    wxTheApp = (* wxApp::GetInitializerFunction()) ();
  }

  if (!wxTheApp) {
    MessageBox(NULL, "You have to define an instance of wxApp!", "wxWindows Error", MB_APPLMODAL | MB_ICONSTOP | MB_OK);
    return 0;
  }

  wxTheApp->argc = 0;
  wxTheApp->argv = NULL;

  wxTheApp->OnInitGui();

  wxTheApp->OnInit();

  if (wxTheApp->GetTopWindow() && wxTheApp->GetTopWindow()->GetHWND()) {
    wxTheApp->GetTopWindow()->Show(TRUE);
  }

  return 1;
}
#endif // _WINDLL

//// Static member initialization

wxAppInitializerFunction wxApp::m_appInitFn = (wxAppInitializerFunction) NULL;

wxApp::wxApp()
{
  m_topWindow = NULL;
  wxTheApp = this;
  m_className = "";
  m_wantDebugOutput = TRUE ;
  m_appName = "";
  argc = 0;
  argv = NULL;
#ifdef __WXMSW__
  m_printMode = wxPRINT_WINDOWS;
#else
  m_printMode = wxPRINT_POSTSCRIPT;
#endif
  m_exitOnFrameDelete = TRUE;
  m_auto3D = TRUE;
}

wxApp::~wxApp()
{
  // Delete command-line args
  int i;
  for (i = 0; i < argc; i++)
  {
    delete[] argv[i];
  }
  delete argv;
}

bool wxApp::Initialized()
{
#ifndef _WINDLL
  if (GetTopWindow())
    return TRUE;
  else
    return FALSE;
#endif
#ifdef _WINDLL // Assume initialized if DLL (no way of telling)
  return TRUE;
#endif
}

/*
 * Get and process a message, returning FALSE if WM_QUIT
 * received.
 *
 */
bool wxApp::DoMessage()
{
  if (!::GetMessage(&s_currentMsg, (HWND) NULL, 0, 0))
  {
    return FALSE;
  }

  // Process the message
  if (!ProcessMessage((WXMSG *)&s_currentMsg))
  {
    ::TranslateMessage(&s_currentMsg);
    wxApp::sm_lastMessageTime = s_currentMsg.time; /* MATTHEW: timeStamp impl. */
    ::DispatchMessage(&s_currentMsg);
  }
  return TRUE;
}

/*
 * Keep trying to process messages until WM_QUIT
 * received.
 *
 * If there are messages to be processed, they will all be
 * processed and OnIdle will not be called.
 * When there are no more messages, OnIdle is called.
 * If OnIdle requests more time,
 * it will be repeatedly called so long as there are no pending messages.
 * A 'feature' of this is that once OnIdle has decided that no more processing
 * is required, then it won't get processing time until further messages
 * are processed (it'll sit in DoMessage).
 */

int wxApp::MainLoop()
{
  m_keepGoing = TRUE;
  while (m_keepGoing)
  {
    while (!::PeekMessage(&s_currentMsg, 0, 0, 0, PM_NOREMOVE) &&
           ProcessIdle()) {}
    if (!DoMessage())
      m_keepGoing = FALSE;
  }

  return s_currentMsg.wParam;
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

bool wxApp::Pending()
{
  return (::PeekMessage(&s_currentMsg, 0, 0, 0, PM_NOREMOVE) != 0) ;
}

void wxApp::Dispatch()
{
    if (!DoMessage())
      m_keepGoing = FALSE;
}

/*
 * Give all windows a chance to preprocess
 * the message. Some may have accelerator tables, or have
 * MDI complications.
 */
bool wxApp::ProcessMessage(WXMSG *Msg)
{
  MSG *msg = (MSG *)Msg;

  HWND hWnd;

  // Try translations first; find the youngest window with
  // a translation table.
  for (hWnd = msg->hwnd; hWnd != NULL; hWnd = ::GetParent(hWnd))
  {
    wxWindow *wnd = wxFindWinFromHandle((WXHWND) hWnd);
    if (wnd)
    {
       if (wnd->MSWTranslateMessage(Msg))
         return TRUE;
    }
  }

  // Anyone for a non-translation message? Try youngest descendants first.
  for (hWnd = msg->hwnd; hWnd != NULL; hWnd = ::GetParent(hWnd))
  {
    wxWindow *wnd = wxFindWinFromHandle((WXHWND) hWnd);
    if (wnd)
    {
       if (wnd->MSWProcessMessage(Msg))
         return TRUE;
    }
  }
  return FALSE;
}

void wxApp::OnIdle(wxIdleEvent& event)
{
  static bool inOnIdle = FALSE;

  // Avoid recursion (via ProcessEvent default case)
  if (inOnIdle)
    return;

  inOnIdle = TRUE;

  // 'Garbage' collection of windows deleted with Close().
  DeletePendingObjects();

  // flush the logged messages if any
  wxLog *pLog = wxLog::GetActiveTarget();
  if ( pLog != NULL && pLog->HasPendingMessages() )
    pLog->Flush();

  // Send OnIdle events to all windows
  bool needMore = SendIdleEvents();
//  bool needMore = FALSE;

  if (needMore)
    event.RequestMore(TRUE);

  inOnIdle = FALSE;
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

  wxNode* node = win->GetChildren()->First();
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

wxLog* wxApp::CreateLogTarget()
{
    return new wxLogGui;
}

wxWindow* wxApp::GetTopWindow() const
{
    if (m_topWindow)
        return m_topWindow;
    else if (wxTopLevelWindows.Number() > 0)
        return (wxWindow*) wxTopLevelWindows.First()->Data();
    else
        return NULL;
}

int wxApp::GetComCtl32Version() const
{
    // have we loaded COMCTL32 yet?
    HMODULE theModule = ::GetModuleHandle("COMCTL32");
    int version = 0;

    // if so, then we can check for the version
    if (theModule)
    {
        // InitCommonControlsEx is unique to 4.7 and later
        FARPROC theProc = ::GetProcAddress(theModule, "InitCommonControlsEx");

        if (! theProc)
        {                    // not found, must be 4.00
      version = 400;
        }
        else
        {
      // The following symbol are unique to 4.71
      //   DllInstall
      //   FlatSB_EnableScrollBar FlatSB_GetScrollInfo FlatSB_GetScrollPos
      //   FlatSB_GetScrollProp FlatSB_GetScrollRange FlatSB_SetScrollInfo
      //   FlatSB_SetScrollPos FlatSB_SetScrollProp FlatSB_SetScrollRange
      //   FlatSB_ShowScrollBar
      //   _DrawIndirectImageList _DuplicateImageList
      //   InitializeFlatSB
      //   UninitializeFlatSB
      // we could check for any of these - I chose DllInstall
      FARPROC theProc = ::GetProcAddress(theModule, "DllInstall");
      if (! theProc)
      {
        // not found, must be 4.70
        version = 470;
      }
      else
      {                         // found, must be 4.71
        version = 471;
      }
        }
    }
    return version;
}

void wxExit()
{
  wxApp::CleanUp();
  FatalAppExit(0, "Fatal error: exiting");
}

// Yield to incoming messages
bool wxYield()
{
  MSG msg;
  // We want to go back to the main message loop
  // if we see a WM_QUIT. (?)
  while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) && msg.message != WM_QUIT)
  {
    if (!wxTheApp->DoMessage())
      break;
  }

  return TRUE;
}

HINSTANCE wxGetInstance()
{
  return wxhInstance;
}

// For some reason, with MSVC++ 1.5, WinMain isn't linked in properly
// if in a separate file. So include it here to ensure it's linked.
#if (defined(_MSC_VER) && !defined(__WIN32__)) || defined(__GNUWIN32__)
  #include "main.cpp"
#endif
