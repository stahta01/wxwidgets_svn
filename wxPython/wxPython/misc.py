## This file reverse renames symbols in the wx package to give
## them their wx prefix again, for backwards compatibility.
##
## Generated by ./distrib/build_renamers.py

# This silly stuff here is so the wxPython.wx module doesn't conflict
# with the wx package.  We need to import modules from the wx package
# here, then we'll put the wxPython.wx entry back in sys.modules.
import sys
_wx = None
if sys.modules.has_key('wxPython.wx'):
    _wx = sys.modules['wxPython.wx']
    del sys.modules['wxPython.wx']

import wx.misc

sys.modules['wxPython.wx'] = _wx
del sys, _wx


# Now assign all the reverse-renamed names:
wxSYS_OEM_FIXED_FONT = wx.misc.SYS_OEM_FIXED_FONT
wxSYS_ANSI_FIXED_FONT = wx.misc.SYS_ANSI_FIXED_FONT
wxSYS_ANSI_VAR_FONT = wx.misc.SYS_ANSI_VAR_FONT
wxSYS_SYSTEM_FONT = wx.misc.SYS_SYSTEM_FONT
wxSYS_DEVICE_DEFAULT_FONT = wx.misc.SYS_DEVICE_DEFAULT_FONT
wxSYS_DEFAULT_PALETTE = wx.misc.SYS_DEFAULT_PALETTE
wxSYS_SYSTEM_FIXED_FONT = wx.misc.SYS_SYSTEM_FIXED_FONT
wxSYS_DEFAULT_GUI_FONT = wx.misc.SYS_DEFAULT_GUI_FONT
wxSYS_ICONTITLE_FONT = wx.misc.SYS_ICONTITLE_FONT
wxSYS_COLOUR_SCROLLBAR = wx.misc.SYS_COLOUR_SCROLLBAR
wxSYS_COLOUR_BACKGROUND = wx.misc.SYS_COLOUR_BACKGROUND
wxSYS_COLOUR_DESKTOP = wx.misc.SYS_COLOUR_DESKTOP
wxSYS_COLOUR_ACTIVECAPTION = wx.misc.SYS_COLOUR_ACTIVECAPTION
wxSYS_COLOUR_INACTIVECAPTION = wx.misc.SYS_COLOUR_INACTIVECAPTION
wxSYS_COLOUR_MENU = wx.misc.SYS_COLOUR_MENU
wxSYS_COLOUR_WINDOW = wx.misc.SYS_COLOUR_WINDOW
wxSYS_COLOUR_WINDOWFRAME = wx.misc.SYS_COLOUR_WINDOWFRAME
wxSYS_COLOUR_MENUTEXT = wx.misc.SYS_COLOUR_MENUTEXT
wxSYS_COLOUR_WINDOWTEXT = wx.misc.SYS_COLOUR_WINDOWTEXT
wxSYS_COLOUR_CAPTIONTEXT = wx.misc.SYS_COLOUR_CAPTIONTEXT
wxSYS_COLOUR_ACTIVEBORDER = wx.misc.SYS_COLOUR_ACTIVEBORDER
wxSYS_COLOUR_INACTIVEBORDER = wx.misc.SYS_COLOUR_INACTIVEBORDER
wxSYS_COLOUR_APPWORKSPACE = wx.misc.SYS_COLOUR_APPWORKSPACE
wxSYS_COLOUR_HIGHLIGHT = wx.misc.SYS_COLOUR_HIGHLIGHT
wxSYS_COLOUR_HIGHLIGHTTEXT = wx.misc.SYS_COLOUR_HIGHLIGHTTEXT
wxSYS_COLOUR_BTNFACE = wx.misc.SYS_COLOUR_BTNFACE
wxSYS_COLOUR_3DFACE = wx.misc.SYS_COLOUR_3DFACE
wxSYS_COLOUR_BTNSHADOW = wx.misc.SYS_COLOUR_BTNSHADOW
wxSYS_COLOUR_3DSHADOW = wx.misc.SYS_COLOUR_3DSHADOW
wxSYS_COLOUR_GRAYTEXT = wx.misc.SYS_COLOUR_GRAYTEXT
wxSYS_COLOUR_BTNTEXT = wx.misc.SYS_COLOUR_BTNTEXT
wxSYS_COLOUR_INACTIVECAPTIONTEXT = wx.misc.SYS_COLOUR_INACTIVECAPTIONTEXT
wxSYS_COLOUR_BTNHIGHLIGHT = wx.misc.SYS_COLOUR_BTNHIGHLIGHT
wxSYS_COLOUR_BTNHILIGHT = wx.misc.SYS_COLOUR_BTNHILIGHT
wxSYS_COLOUR_3DHIGHLIGHT = wx.misc.SYS_COLOUR_3DHIGHLIGHT
wxSYS_COLOUR_3DHILIGHT = wx.misc.SYS_COLOUR_3DHILIGHT
wxSYS_COLOUR_3DDKSHADOW = wx.misc.SYS_COLOUR_3DDKSHADOW
wxSYS_COLOUR_3DLIGHT = wx.misc.SYS_COLOUR_3DLIGHT
wxSYS_COLOUR_INFOTEXT = wx.misc.SYS_COLOUR_INFOTEXT
wxSYS_COLOUR_INFOBK = wx.misc.SYS_COLOUR_INFOBK
wxSYS_COLOUR_LISTBOX = wx.misc.SYS_COLOUR_LISTBOX
wxSYS_COLOUR_HOTLIGHT = wx.misc.SYS_COLOUR_HOTLIGHT
wxSYS_COLOUR_GRADIENTACTIVECAPTION = wx.misc.SYS_COLOUR_GRADIENTACTIVECAPTION
wxSYS_COLOUR_GRADIENTINACTIVECAPTION = wx.misc.SYS_COLOUR_GRADIENTINACTIVECAPTION
wxSYS_COLOUR_MENUHILIGHT = wx.misc.SYS_COLOUR_MENUHILIGHT
wxSYS_COLOUR_MENUBAR = wx.misc.SYS_COLOUR_MENUBAR
wxSYS_COLOUR_MAX = wx.misc.SYS_COLOUR_MAX
wxSYS_MOUSE_BUTTONS = wx.misc.SYS_MOUSE_BUTTONS
wxSYS_BORDER_X = wx.misc.SYS_BORDER_X
wxSYS_BORDER_Y = wx.misc.SYS_BORDER_Y
wxSYS_CURSOR_X = wx.misc.SYS_CURSOR_X
wxSYS_CURSOR_Y = wx.misc.SYS_CURSOR_Y
wxSYS_DCLICK_X = wx.misc.SYS_DCLICK_X
wxSYS_DCLICK_Y = wx.misc.SYS_DCLICK_Y
wxSYS_DRAG_X = wx.misc.SYS_DRAG_X
wxSYS_DRAG_Y = wx.misc.SYS_DRAG_Y
wxSYS_EDGE_X = wx.misc.SYS_EDGE_X
wxSYS_EDGE_Y = wx.misc.SYS_EDGE_Y
wxSYS_HSCROLL_ARROW_X = wx.misc.SYS_HSCROLL_ARROW_X
wxSYS_HSCROLL_ARROW_Y = wx.misc.SYS_HSCROLL_ARROW_Y
wxSYS_HTHUMB_X = wx.misc.SYS_HTHUMB_X
wxSYS_ICON_X = wx.misc.SYS_ICON_X
wxSYS_ICON_Y = wx.misc.SYS_ICON_Y
wxSYS_ICONSPACING_X = wx.misc.SYS_ICONSPACING_X
wxSYS_ICONSPACING_Y = wx.misc.SYS_ICONSPACING_Y
wxSYS_WINDOWMIN_X = wx.misc.SYS_WINDOWMIN_X
wxSYS_WINDOWMIN_Y = wx.misc.SYS_WINDOWMIN_Y
wxSYS_SCREEN_X = wx.misc.SYS_SCREEN_X
wxSYS_SCREEN_Y = wx.misc.SYS_SCREEN_Y
wxSYS_FRAMESIZE_X = wx.misc.SYS_FRAMESIZE_X
wxSYS_FRAMESIZE_Y = wx.misc.SYS_FRAMESIZE_Y
wxSYS_SMALLICON_X = wx.misc.SYS_SMALLICON_X
wxSYS_SMALLICON_Y = wx.misc.SYS_SMALLICON_Y
wxSYS_HSCROLL_Y = wx.misc.SYS_HSCROLL_Y
wxSYS_VSCROLL_X = wx.misc.SYS_VSCROLL_X
wxSYS_VSCROLL_ARROW_X = wx.misc.SYS_VSCROLL_ARROW_X
wxSYS_VSCROLL_ARROW_Y = wx.misc.SYS_VSCROLL_ARROW_Y
wxSYS_VTHUMB_Y = wx.misc.SYS_VTHUMB_Y
wxSYS_CAPTION_Y = wx.misc.SYS_CAPTION_Y
wxSYS_MENU_Y = wx.misc.SYS_MENU_Y
wxSYS_NETWORK_PRESENT = wx.misc.SYS_NETWORK_PRESENT
wxSYS_PENWINDOWS_PRESENT = wx.misc.SYS_PENWINDOWS_PRESENT
wxSYS_SHOW_SOUNDS = wx.misc.SYS_SHOW_SOUNDS
wxSYS_SWAP_BUTTONS = wx.misc.SYS_SWAP_BUTTONS
wxSYS_CAN_DRAW_FRAME_DECORATIONS = wx.misc.SYS_CAN_DRAW_FRAME_DECORATIONS
wxSYS_CAN_ICONIZE_FRAME = wx.misc.SYS_CAN_ICONIZE_FRAME
wxSYS_SCREEN_NONE = wx.misc.SYS_SCREEN_NONE
wxSYS_SCREEN_TINY = wx.misc.SYS_SCREEN_TINY
wxSYS_SCREEN_PDA = wx.misc.SYS_SCREEN_PDA
wxSYS_SCREEN_SMALL = wx.misc.SYS_SCREEN_SMALL
wxSYS_SCREEN_DESKTOP = wx.misc.SYS_SCREEN_DESKTOP
wxSystemSettings = wx.misc.SystemSettings
wxSystemSettingsPtr = wx.misc.SystemSettingsPtr
wxSystemSettings_GetColour = wx.misc.SystemSettings_GetColour
wxSystemSettings_GetFont = wx.misc.SystemSettings_GetFont
wxSystemSettings_GetMetric = wx.misc.SystemSettings_GetMetric
wxSystemSettings_HasFeature = wx.misc.SystemSettings_HasFeature
wxSystemSettings_GetScreenType = wx.misc.SystemSettings_GetScreenType
wxSystemSettings_SetScreenType = wx.misc.SystemSettings_SetScreenType
wxSystemOptions = wx.misc.SystemOptions
wxSystemOptionsPtr = wx.misc.SystemOptionsPtr
wxSystemOptions_SetOption = wx.misc.SystemOptions_SetOption
wxSystemOptions_SetOptionInt = wx.misc.SystemOptions_SetOptionInt
wxSystemOptions_GetOption = wx.misc.SystemOptions_GetOption
wxSystemOptions_GetOptionInt = wx.misc.SystemOptions_GetOptionInt
wxSystemOptions_HasOption = wx.misc.SystemOptions_HasOption
wxFileSelectorPromptStr = wx.misc.FileSelectorPromptStr
wxFileSelectorDefaultWildcardStr = wx.misc.FileSelectorDefaultWildcardStr
wxDirSelectorPromptStr = wx.misc.DirSelectorPromptStr
wxNewId = wx.misc.NewId
wxRegisterId = wx.misc.RegisterId
wxGetCurrentId = wx.misc.GetCurrentId
wxBell = wx.misc.Bell
wxEndBusyCursor = wx.misc.EndBusyCursor
wxGetElapsedTime = wx.misc.GetElapsedTime
wxGetMousePosition = wx.misc.GetMousePosition
wxIsBusy = wx.misc.IsBusy
wxNow = wx.misc.Now
wxShell = wx.misc.Shell
wxStartTimer = wx.misc.StartTimer
wxGetOsVersion = wx.misc.GetOsVersion
wxGetOsDescription = wx.misc.GetOsDescription
wxGetFreeMemory = wx.misc.GetFreeMemory
wxSHUTDOWN_POWEROFF = wx.misc.SHUTDOWN_POWEROFF
wxSHUTDOWN_REBOOT = wx.misc.SHUTDOWN_REBOOT
wxShutdown = wx.misc.Shutdown
wxSleep = wx.misc.Sleep
wxUsleep = wx.misc.Usleep
wxEnableTopLevelWindows = wx.misc.EnableTopLevelWindows
wxStripMenuCodes = wx.misc.StripMenuCodes
wxGetEmailAddress = wx.misc.GetEmailAddress
wxGetHostName = wx.misc.GetHostName
wxGetFullHostName = wx.misc.GetFullHostName
wxGetUserId = wx.misc.GetUserId
wxGetUserName = wx.misc.GetUserName
wxGetHomeDir = wx.misc.GetHomeDir
wxGetUserHome = wx.misc.GetUserHome
wxGetProcessId = wx.misc.GetProcessId
wxTrap = wx.misc.Trap
wxFileSelector = wx.misc.FileSelector
wxLoadFileSelector = wx.misc.LoadFileSelector
wxSaveFileSelector = wx.misc.SaveFileSelector
wxDirSelector = wx.misc.DirSelector
wxGetTextFromUser = wx.misc.GetTextFromUser
wxGetPasswordFromUser = wx.misc.GetPasswordFromUser
wxGetSingleChoice = wx.misc.GetSingleChoice
wxGetSingleChoiceIndex = wx.misc.GetSingleChoiceIndex
wxMessageBox = wx.misc.MessageBox
wxGetNumberFromUser = wx.misc.GetNumberFromUser
wxColourDisplay = wx.misc.ColourDisplay
wxDisplayDepth = wx.misc.DisplayDepth
wxGetDisplayDepth = wx.misc.GetDisplayDepth
wxDisplaySize = wx.misc.DisplaySize
wxGetDisplaySize = wx.misc.GetDisplaySize
wxDisplaySizeMM = wx.misc.DisplaySizeMM
wxGetDisplaySizeMM = wx.misc.GetDisplaySizeMM
wxClientDisplayRect = wx.misc.ClientDisplayRect
wxGetClientDisplayRect = wx.misc.GetClientDisplayRect
wxSetCursor = wx.misc.SetCursor
wxBeginBusyCursor = wx.misc.BeginBusyCursor
wxGetActiveWindow = wx.misc.GetActiveWindow
wxGenericFindWindowAtPoint = wx.misc.GenericFindWindowAtPoint
wxFindWindowAtPoint = wx.misc.FindWindowAtPoint
wxGetTopLevelParent = wx.misc.GetTopLevelParent
wxGetKeyState = wx.misc.GetKeyState
wxWakeUpMainThread = wx.misc.WakeUpMainThread
wxMutexGuiEnter = wx.misc.MutexGuiEnter
wxMutexGuiLeave = wx.misc.MutexGuiLeave
wxMutexGuiLocker = wx.misc.MutexGuiLocker
wxMutexGuiLockerPtr = wx.misc.MutexGuiLockerPtr
wxThread_IsMain = wx.misc.Thread_IsMain
wxToolTip = wx.misc.ToolTip
wxToolTipPtr = wx.misc.ToolTipPtr
wxToolTip_Enable = wx.misc.ToolTip_Enable
wxToolTip_SetDelay = wx.misc.ToolTip_SetDelay
wxCaret = wx.misc.Caret
wxCaretPtr = wx.misc.CaretPtr
wxCaret_GetBlinkTime = wx.misc.Caret_GetBlinkTime
wxCaret_SetBlinkTime = wx.misc.Caret_SetBlinkTime
wxBusyCursor = wx.misc.BusyCursor
wxBusyCursorPtr = wx.misc.BusyCursorPtr
wxWindowDisabler = wx.misc.WindowDisabler
wxWindowDisablerPtr = wx.misc.WindowDisablerPtr
wxBusyInfo = wx.misc.BusyInfo
wxBusyInfoPtr = wx.misc.BusyInfoPtr
wxStopWatch = wx.misc.StopWatch
wxStopWatchPtr = wx.misc.StopWatchPtr
wxFileHistory = wx.misc.FileHistory
wxFileHistoryPtr = wx.misc.FileHistoryPtr
wxSingleInstanceChecker = wx.misc.SingleInstanceChecker
wxSingleInstanceCheckerPtr = wx.misc.SingleInstanceCheckerPtr
wxPreSingleInstanceChecker = wx.misc.PreSingleInstanceChecker
wxDrawWindowOnDC = wx.misc.DrawWindowOnDC
wxTipProvider = wx.misc.TipProvider
wxTipProviderPtr = wx.misc.TipProviderPtr
wxPyTipProvider = wx.misc.PyTipProvider
wxPyTipProviderPtr = wx.misc.PyTipProviderPtr
wxShowTip = wx.misc.ShowTip
wxCreateFileTipProvider = wx.misc.CreateFileTipProvider
wxTIMER_CONTINUOUS = wx.misc.TIMER_CONTINUOUS
wxTIMER_ONE_SHOT = wx.misc.TIMER_ONE_SHOT
wxEVT_TIMER = wx.misc.wxEVT_TIMER
wxTimer = wx.misc.Timer
wxTimerPtr = wx.misc.TimerPtr
wxTimerEvent = wx.misc.TimerEvent
wxTimerEventPtr = wx.misc.TimerEventPtr
wxTimerRunner = wx.misc.TimerRunner
wxTimerRunnerPtr = wx.misc.TimerRunnerPtr
wxLOG_FatalError = wx.misc.LOG_FatalError
wxLOG_Error = wx.misc.LOG_Error
wxLOG_Warning = wx.misc.LOG_Warning
wxLOG_Message = wx.misc.LOG_Message
wxLOG_Status = wx.misc.LOG_Status
wxLOG_Info = wx.misc.LOG_Info
wxLOG_Debug = wx.misc.LOG_Debug
wxLOG_Trace = wx.misc.LOG_Trace
wxLOG_Progress = wx.misc.LOG_Progress
wxLOG_User = wx.misc.LOG_User
wxLOG_Max = wx.misc.LOG_Max
wxTRACE_MemAlloc = wx.misc.TRACE_MemAlloc
wxTRACE_Messages = wx.misc.TRACE_Messages
wxTRACE_ResAlloc = wx.misc.TRACE_ResAlloc
wxTRACE_RefCount = wx.misc.TRACE_RefCount
wxTRACE_OleCalls = wx.misc.TRACE_OleCalls
wxTraceMemAlloc = wx.misc.TraceMemAlloc
wxTraceMessages = wx.misc.TraceMessages
wxTraceResAlloc = wx.misc.TraceResAlloc
wxTraceRefCount = wx.misc.TraceRefCount
wxTraceOleCalls = wx.misc.TraceOleCalls
wxLog = wx.misc.Log
wxLogPtr = wx.misc.LogPtr
wxLog_IsEnabled = wx.misc.Log_IsEnabled
wxLog_EnableLogging = wx.misc.Log_EnableLogging
wxLog_OnLog = wx.misc.Log_OnLog
wxLog_FlushActive = wx.misc.Log_FlushActive
wxLog_GetActiveTarget = wx.misc.Log_GetActiveTarget
wxLog_SetActiveTarget = wx.misc.Log_SetActiveTarget
wxLog_Suspend = wx.misc.Log_Suspend
wxLog_Resume = wx.misc.Log_Resume
wxLog_SetVerbose = wx.misc.Log_SetVerbose
wxLog_SetLogLevel = wx.misc.Log_SetLogLevel
wxLog_DontCreateOnDemand = wx.misc.Log_DontCreateOnDemand
wxLog_SetTraceMask = wx.misc.Log_SetTraceMask
wxLog_AddTraceMask = wx.misc.Log_AddTraceMask
wxLog_RemoveTraceMask = wx.misc.Log_RemoveTraceMask
wxLog_ClearTraceMasks = wx.misc.Log_ClearTraceMasks
wxLog_GetTraceMasks = wx.misc.Log_GetTraceMasks
wxLog_SetTimestamp = wx.misc.Log_SetTimestamp
wxLog_GetVerbose = wx.misc.Log_GetVerbose
wxLog_GetTraceMask = wx.misc.Log_GetTraceMask
wxLog_IsAllowedTraceMask = wx.misc.Log_IsAllowedTraceMask
wxLog_GetLogLevel = wx.misc.Log_GetLogLevel
wxLog_GetTimestamp = wx.misc.Log_GetTimestamp
wxLog_TimeStamp = wx.misc.Log_TimeStamp
wxLogStderr = wx.misc.LogStderr
wxLogStderrPtr = wx.misc.LogStderrPtr
wxLogTextCtrl = wx.misc.LogTextCtrl
wxLogTextCtrlPtr = wx.misc.LogTextCtrlPtr
wxLogGui = wx.misc.LogGui
wxLogGuiPtr = wx.misc.LogGuiPtr
wxLogWindow = wx.misc.LogWindow
wxLogWindowPtr = wx.misc.LogWindowPtr
wxLogChain = wx.misc.LogChain
wxLogChainPtr = wx.misc.LogChainPtr
wxSysErrorCode = wx.misc.SysErrorCode
wxSysErrorMsg = wx.misc.SysErrorMsg
wxLogFatalError = wx.misc.LogFatalError
wxLogError = wx.misc.LogError
wxLogWarning = wx.misc.LogWarning
wxLogMessage = wx.misc.LogMessage
wxLogInfo = wx.misc.LogInfo
wxLogDebug = wx.misc.LogDebug
wxLogVerbose = wx.misc.LogVerbose
wxLogStatus = wx.misc.LogStatus
wxLogStatusFrame = wx.misc.LogStatusFrame
wxLogSysError = wx.misc.LogSysError
wxLogTrace = wx.misc.LogTrace
wxLogTrace = wx.misc.LogTrace
wxLogGeneric = wx.misc.LogGeneric
wxSafeShowMessage = wx.misc.SafeShowMessage
wxLogNull = wx.misc.LogNull
wxLogNullPtr = wx.misc.LogNullPtr
wxPyLog = wx.misc.PyLog
wxPyLogPtr = wx.misc.PyLogPtr
wxPROCESS_DEFAULT = wx.misc.PROCESS_DEFAULT
wxPROCESS_REDIRECT = wx.misc.PROCESS_REDIRECT
wxKILL_OK = wx.misc.KILL_OK
wxKILL_BAD_SIGNAL = wx.misc.KILL_BAD_SIGNAL
wxKILL_ACCESS_DENIED = wx.misc.KILL_ACCESS_DENIED
wxKILL_NO_PROCESS = wx.misc.KILL_NO_PROCESS
wxKILL_ERROR = wx.misc.KILL_ERROR
wxSIGNONE = wx.misc.SIGNONE
wxSIGHUP = wx.misc.SIGHUP
wxSIGINT = wx.misc.SIGINT
wxSIGQUIT = wx.misc.SIGQUIT
wxSIGILL = wx.misc.SIGILL
wxSIGTRAP = wx.misc.SIGTRAP
wxSIGABRT = wx.misc.SIGABRT
wxSIGIOT = wx.misc.SIGIOT
wxSIGEMT = wx.misc.SIGEMT
wxSIGFPE = wx.misc.SIGFPE
wxSIGKILL = wx.misc.SIGKILL
wxSIGBUS = wx.misc.SIGBUS
wxSIGSEGV = wx.misc.SIGSEGV
wxSIGSYS = wx.misc.SIGSYS
wxSIGPIPE = wx.misc.SIGPIPE
wxSIGALRM = wx.misc.SIGALRM
wxSIGTERM = wx.misc.SIGTERM
wxProcess = wx.misc.Process
wxProcessPtr = wx.misc.ProcessPtr
wxProcess_Kill = wx.misc.Process_Kill
wxProcess_Exists = wx.misc.Process_Exists
wxProcess_Open = wx.misc.Process_Open
wxProcessEvent = wx.misc.ProcessEvent
wxProcessEventPtr = wx.misc.ProcessEventPtr
wxEVT_END_PROCESS = wx.misc.wxEVT_END_PROCESS
wxEXEC_ASYNC = wx.misc.EXEC_ASYNC
wxEXEC_SYNC = wx.misc.EXEC_SYNC
wxEXEC_NOHIDE = wx.misc.EXEC_NOHIDE
wxEXEC_MAKE_GROUP_LEADER = wx.misc.EXEC_MAKE_GROUP_LEADER
wxExecute = wx.misc.Execute
wxJOYSTICK1 = wx.misc.JOYSTICK1
wxJOYSTICK2 = wx.misc.JOYSTICK2
wxJOY_BUTTON_ANY = wx.misc.JOY_BUTTON_ANY
wxJOY_BUTTON1 = wx.misc.JOY_BUTTON1
wxJOY_BUTTON2 = wx.misc.JOY_BUTTON2
wxJOY_BUTTON3 = wx.misc.JOY_BUTTON3
wxJOY_BUTTON4 = wx.misc.JOY_BUTTON4
wxJoystick = wx.misc.Joystick
wxJoystickPtr = wx.misc.JoystickPtr
wxEVT_JOY_BUTTON_DOWN = wx.misc.wxEVT_JOY_BUTTON_DOWN
wxEVT_JOY_BUTTON_UP = wx.misc.wxEVT_JOY_BUTTON_UP
wxEVT_JOY_MOVE = wx.misc.wxEVT_JOY_MOVE
wxEVT_JOY_ZMOVE = wx.misc.wxEVT_JOY_ZMOVE
wxJoystickEvent = wx.misc.JoystickEvent
wxJoystickEventPtr = wx.misc.JoystickEventPtr
wxSOUND_SYNC = wx.misc.SOUND_SYNC
wxSOUND_ASYNC = wx.misc.SOUND_ASYNC
wxSOUND_LOOP = wx.misc.SOUND_LOOP
wxSound = wx.misc.Sound
wxSoundPtr = wx.misc.SoundPtr
wxSound_PlaySound = wx.misc.Sound_PlaySound
wxSound_Stop = wx.misc.Sound_Stop
wxMAILCAP_STANDARD = wx.misc.MAILCAP_STANDARD
wxMAILCAP_NETSCAPE = wx.misc.MAILCAP_NETSCAPE
wxMAILCAP_KDE = wx.misc.MAILCAP_KDE
wxMAILCAP_GNOME = wx.misc.MAILCAP_GNOME
wxMAILCAP_ALL = wx.misc.MAILCAP_ALL
wxFileTypeInfo = wx.misc.FileTypeInfo
wxFileTypeInfoPtr = wx.misc.FileTypeInfoPtr
wxFileTypeInfoSequence = wx.misc.FileTypeInfoSequence
wxNullFileTypeInfo = wx.misc.NullFileTypeInfo
wxFileType = wx.misc.FileType
wxFileTypePtr = wx.misc.FileTypePtr
wxFileType_ExpandCommand = wx.misc.FileType_ExpandCommand
wxTheMimeTypesManager = wx.misc.TheMimeTypesManager
wxMimeTypesManager = wx.misc.MimeTypesManager
wxMimeTypesManagerPtr = wx.misc.MimeTypesManagerPtr
wxMimeTypesManager_IsOfType = wx.misc.MimeTypesManager_IsOfType
wxART_TOOLBAR = wx.misc.ART_TOOLBAR
wxART_MENU = wx.misc.ART_MENU
wxART_FRAME_ICON = wx.misc.ART_FRAME_ICON
wxART_CMN_DIALOG = wx.misc.ART_CMN_DIALOG
wxART_HELP_BROWSER = wx.misc.ART_HELP_BROWSER
wxART_MESSAGE_BOX = wx.misc.ART_MESSAGE_BOX
wxART_OTHER = wx.misc.ART_OTHER
wxART_ADD_BOOKMARK = wx.misc.ART_ADD_BOOKMARK
wxART_DEL_BOOKMARK = wx.misc.ART_DEL_BOOKMARK
wxART_HELP_SIDE_PANEL = wx.misc.ART_HELP_SIDE_PANEL
wxART_HELP_SETTINGS = wx.misc.ART_HELP_SETTINGS
wxART_HELP_BOOK = wx.misc.ART_HELP_BOOK
wxART_HELP_FOLDER = wx.misc.ART_HELP_FOLDER
wxART_HELP_PAGE = wx.misc.ART_HELP_PAGE
wxART_GO_BACK = wx.misc.ART_GO_BACK
wxART_GO_FORWARD = wx.misc.ART_GO_FORWARD
wxART_GO_UP = wx.misc.ART_GO_UP
wxART_GO_DOWN = wx.misc.ART_GO_DOWN
wxART_GO_TO_PARENT = wx.misc.ART_GO_TO_PARENT
wxART_GO_HOME = wx.misc.ART_GO_HOME
wxART_FILE_OPEN = wx.misc.ART_FILE_OPEN
wxART_PRINT = wx.misc.ART_PRINT
wxART_HELP = wx.misc.ART_HELP
wxART_TIP = wx.misc.ART_TIP
wxART_REPORT_VIEW = wx.misc.ART_REPORT_VIEW
wxART_LIST_VIEW = wx.misc.ART_LIST_VIEW
wxART_NEW_DIR = wx.misc.ART_NEW_DIR
wxART_FOLDER = wx.misc.ART_FOLDER
wxART_GO_DIR_UP = wx.misc.ART_GO_DIR_UP
wxART_EXECUTABLE_FILE = wx.misc.ART_EXECUTABLE_FILE
wxART_NORMAL_FILE = wx.misc.ART_NORMAL_FILE
wxART_TICK_MARK = wx.misc.ART_TICK_MARK
wxART_CROSS_MARK = wx.misc.ART_CROSS_MARK
wxART_ERROR = wx.misc.ART_ERROR
wxART_QUESTION = wx.misc.ART_QUESTION
wxART_WARNING = wx.misc.ART_WARNING
wxART_INFORMATION = wx.misc.ART_INFORMATION
wxART_MISSING_IMAGE = wx.misc.ART_MISSING_IMAGE
wxArtProvider = wx.misc.ArtProvider
wxArtProviderPtr = wx.misc.ArtProviderPtr
wxArtProvider_PushProvider = wx.misc.ArtProvider_PushProvider
wxArtProvider_PopProvider = wx.misc.ArtProvider_PopProvider
wxArtProvider_RemoveProvider = wx.misc.ArtProvider_RemoveProvider
wxArtProvider_GetBitmap = wx.misc.ArtProvider_GetBitmap
wxArtProvider_GetIcon = wx.misc.ArtProvider_GetIcon
wxCONFIG_USE_LOCAL_FILE = wx.misc.CONFIG_USE_LOCAL_FILE
wxCONFIG_USE_GLOBAL_FILE = wx.misc.CONFIG_USE_GLOBAL_FILE
wxCONFIG_USE_RELATIVE_PATH = wx.misc.CONFIG_USE_RELATIVE_PATH
wxCONFIG_USE_NO_ESCAPE_CHARACTERS = wx.misc.CONFIG_USE_NO_ESCAPE_CHARACTERS
wxConfigBase = wx.misc.ConfigBase
wxConfigBasePtr = wx.misc.ConfigBasePtr
wxConfigBase_Set = wx.misc.ConfigBase_Set
wxConfigBase_Get = wx.misc.ConfigBase_Get
wxConfigBase_Create = wx.misc.ConfigBase_Create
wxConfigBase_DontCreateOnDemand = wx.misc.ConfigBase_DontCreateOnDemand
wxConfigPathChanger = wx.misc.ConfigPathChanger
wxConfigPathChangerPtr = wx.misc.ConfigPathChangerPtr
wxConfig = wx.misc.Config
wxConfigPtr = wx.misc.ConfigPtr
wxFileConfig = wx.misc.FileConfig
wxFileConfigPtr = wx.misc.FileConfigPtr
wxExpandEnvVars = wx.misc.ExpandEnvVars
wxDateFormatStr = wx.misc.DateFormatStr
wxTimeSpanFormatStr = wx.misc.TimeSpanFormatStr
wxDateTime = wx.misc.DateTime
wxDateTimePtr = wx.misc.DateTimePtr
wxDateTime_SetCountry = wx.misc.DateTime_SetCountry
wxDateTime_GetCountry = wx.misc.DateTime_GetCountry
wxDateTime_IsWestEuropeanCountry = wx.misc.DateTime_IsWestEuropeanCountry
wxDateTime_GetCurrentYear = wx.misc.DateTime_GetCurrentYear
wxDateTime_ConvertYearToBC = wx.misc.DateTime_ConvertYearToBC
wxDateTime_GetCurrentMonth = wx.misc.DateTime_GetCurrentMonth
wxDateTime_IsLeapYear = wx.misc.DateTime_IsLeapYear
wxDateTime_GetCentury = wx.misc.DateTime_GetCentury
wxDateTime_GetNumberOfDaysinYear = wx.misc.DateTime_GetNumberOfDaysinYear
wxDateTime_GetNumberOfDaysInMonth = wx.misc.DateTime_GetNumberOfDaysInMonth
wxDateTime_GetMonthName = wx.misc.DateTime_GetMonthName
wxDateTime_GetWeekDayName = wx.misc.DateTime_GetWeekDayName
wxDateTime_GetAmPmStrings = wx.misc.DateTime_GetAmPmStrings
wxDateTime_IsDSTApplicable = wx.misc.DateTime_IsDSTApplicable
wxDateTime_GetBeginDST = wx.misc.DateTime_GetBeginDST
wxDateTime_GetEndDST = wx.misc.DateTime_GetEndDST
wxDateTime_Now = wx.misc.DateTime_Now
wxDateTime_UNow = wx.misc.DateTime_UNow
wxDateTime_Today = wx.misc.DateTime_Today
wxDateTimeFromTimeT = wx.misc.DateTimeFromTimeT
wxDateTimeFromJDN = wx.misc.DateTimeFromJDN
wxDateTimeFromHMS = wx.misc.DateTimeFromHMS
wxDateTimeFromDMY = wx.misc.DateTimeFromDMY
wxTimeSpan = wx.misc.TimeSpan
wxTimeSpanPtr = wx.misc.TimeSpanPtr
wxTimeSpan_Seconds = wx.misc.TimeSpan_Seconds
wxTimeSpan_Second = wx.misc.TimeSpan_Second
wxTimeSpan_Minutes = wx.misc.TimeSpan_Minutes
wxTimeSpan_Minute = wx.misc.TimeSpan_Minute
wxTimeSpan_Hours = wx.misc.TimeSpan_Hours
wxTimeSpan_Hour = wx.misc.TimeSpan_Hour
wxTimeSpan_Days = wx.misc.TimeSpan_Days
wxTimeSpan_Day = wx.misc.TimeSpan_Day
wxTimeSpan_Weeks = wx.misc.TimeSpan_Weeks
wxTimeSpan_Week = wx.misc.TimeSpan_Week
wxDateSpan = wx.misc.DateSpan
wxDateSpanPtr = wx.misc.DateSpanPtr
wxDateSpan_Days = wx.misc.DateSpan_Days
wxDateSpan_Day = wx.misc.DateSpan_Day
wxDateSpan_Weeks = wx.misc.DateSpan_Weeks
wxDateSpan_Week = wx.misc.DateSpan_Week
wxDateSpan_Months = wx.misc.DateSpan_Months
wxDateSpan_Month = wx.misc.DateSpan_Month
wxDateSpan_Years = wx.misc.DateSpan_Years
wxDateSpan_Year = wx.misc.DateSpan_Year
wxGetLocalTime = wx.misc.GetLocalTime
wxGetUTCTime = wx.misc.GetUTCTime
wxGetCurrentTime = wx.misc.GetCurrentTime
wxGetLocalTimeMillis = wx.misc.GetLocalTimeMillis
wxDF_INVALID = wx.misc.DF_INVALID
wxDF_TEXT = wx.misc.DF_TEXT
wxDF_BITMAP = wx.misc.DF_BITMAP
wxDF_METAFILE = wx.misc.DF_METAFILE
wxDF_SYLK = wx.misc.DF_SYLK
wxDF_DIF = wx.misc.DF_DIF
wxDF_TIFF = wx.misc.DF_TIFF
wxDF_OEMTEXT = wx.misc.DF_OEMTEXT
wxDF_DIB = wx.misc.DF_DIB
wxDF_PALETTE = wx.misc.DF_PALETTE
wxDF_PENDATA = wx.misc.DF_PENDATA
wxDF_RIFF = wx.misc.DF_RIFF
wxDF_WAVE = wx.misc.DF_WAVE
wxDF_UNICODETEXT = wx.misc.DF_UNICODETEXT
wxDF_ENHMETAFILE = wx.misc.DF_ENHMETAFILE
wxDF_FILENAME = wx.misc.DF_FILENAME
wxDF_LOCALE = wx.misc.DF_LOCALE
wxDF_PRIVATE = wx.misc.DF_PRIVATE
wxDF_HTML = wx.misc.DF_HTML
wxDF_MAX = wx.misc.DF_MAX
wxDataFormat = wx.misc.DataFormat
wxDataFormatPtr = wx.misc.DataFormatPtr
wxCustomDataFormat = wx.misc.CustomDataFormat
wxFormatInvalid = wx.misc.FormatInvalid
wxDataObject = wx.misc.DataObject
wxDataObjectPtr = wx.misc.DataObjectPtr
wxDataObjectSimple = wx.misc.DataObjectSimple
wxDataObjectSimplePtr = wx.misc.DataObjectSimplePtr
wxPyDataObjectSimple = wx.misc.PyDataObjectSimple
wxPyDataObjectSimplePtr = wx.misc.PyDataObjectSimplePtr
wxDataObjectComposite = wx.misc.DataObjectComposite
wxDataObjectCompositePtr = wx.misc.DataObjectCompositePtr
wxTextDataObject = wx.misc.TextDataObject
wxTextDataObjectPtr = wx.misc.TextDataObjectPtr
wxPyTextDataObject = wx.misc.PyTextDataObject
wxPyTextDataObjectPtr = wx.misc.PyTextDataObjectPtr
wxBitmapDataObject = wx.misc.BitmapDataObject
wxBitmapDataObjectPtr = wx.misc.BitmapDataObjectPtr
wxPyBitmapDataObject = wx.misc.PyBitmapDataObject
wxPyBitmapDataObjectPtr = wx.misc.PyBitmapDataObjectPtr
wxFileDataObject = wx.misc.FileDataObject
wxFileDataObjectPtr = wx.misc.FileDataObjectPtr
wxCustomDataObject = wx.misc.CustomDataObject
wxCustomDataObjectPtr = wx.misc.CustomDataObjectPtr
wxURLDataObject = wx.misc.URLDataObject
wxURLDataObjectPtr = wx.misc.URLDataObjectPtr
wxMetafileDataObject = wx.misc.MetafileDataObject
wxMetafileDataObjectPtr = wx.misc.MetafileDataObjectPtr
wxDrag_CopyOnly = wx.misc.Drag_CopyOnly
wxDrag_AllowMove = wx.misc.Drag_AllowMove
wxDrag_DefaultMove = wx.misc.Drag_DefaultMove
wxDragError = wx.misc.DragError
wxDragNone = wx.misc.DragNone
wxDragCopy = wx.misc.DragCopy
wxDragMove = wx.misc.DragMove
wxDragLink = wx.misc.DragLink
wxDragCancel = wx.misc.DragCancel
wxIsDragResultOk = wx.misc.IsDragResultOk
wxDropSource = wx.misc.DropSource
wxDropSourcePtr = wx.misc.DropSourcePtr
wxDropTarget = wx.misc.DropTarget
wxDropTargetPtr = wx.misc.DropTargetPtr
wxTextDropTarget = wx.misc.TextDropTarget
wxTextDropTargetPtr = wx.misc.TextDropTargetPtr
wxFileDropTarget = wx.misc.FileDropTarget
wxFileDropTargetPtr = wx.misc.FileDropTargetPtr
wxClipboard = wx.misc.Clipboard
wxClipboardPtr = wx.misc.ClipboardPtr
wxTheClipboard = wx.misc.TheClipboard
wxClipboardLocker = wx.misc.ClipboardLocker
wxClipboardLockerPtr = wx.misc.ClipboardLockerPtr
wxPyTimer = wx.misc.PyTimer
wxPyDropTarget = wx.misc.PyDropTarget


