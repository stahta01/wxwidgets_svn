## This file reverse renames symbols in the wx package to give
## them their wx prefix again, for backwards compatibility.
##
## Generated by ./distrib/build_renamers.py

# This silly stuff here is so the wxPython.wx module doesn't conflict
# with the wx package.  We need to import modules from the wx package
# here, then we'll put the entry back in sys.modules.
import sys
_wx = None
if sys.modules.has_key('wxPython.wx'):
    _wx = sys.modules['wxPython.wx']
    del sys.modules['wxPython.wx']

import wx.core

sys.modules['wxPython.wx']
del sys, _wx


# Now assign all the reverse-renamed names:
wxNOT_FOUND = wx.core.NOT_FOUND
wxVSCROLL = wx.core.VSCROLL
wxHSCROLL = wx.core.HSCROLL
wxCAPTION = wx.core.CAPTION
wxDOUBLE_BORDER = wx.core.DOUBLE_BORDER
wxSUNKEN_BORDER = wx.core.SUNKEN_BORDER
wxRAISED_BORDER = wx.core.RAISED_BORDER
wxBORDER = wx.core.BORDER
wxSIMPLE_BORDER = wx.core.SIMPLE_BORDER
wxSTATIC_BORDER = wx.core.STATIC_BORDER
wxTRANSPARENT_WINDOW = wx.core.TRANSPARENT_WINDOW
wxNO_BORDER = wx.core.NO_BORDER
wxUSER_COLOURS = wx.core.USER_COLOURS
wxNO_3D = wx.core.NO_3D
wxTAB_TRAVERSAL = wx.core.TAB_TRAVERSAL
wxWANTS_CHARS = wx.core.WANTS_CHARS
wxPOPUP_WINDOW = wx.core.POPUP_WINDOW
wxCENTER_FRAME = wx.core.CENTER_FRAME
wxCENTRE_ON_SCREEN = wx.core.CENTRE_ON_SCREEN
wxCENTER_ON_SCREEN = wx.core.CENTER_ON_SCREEN
wxSTAY_ON_TOP = wx.core.STAY_ON_TOP
wxICONIZE = wx.core.ICONIZE
wxMINIMIZE = wx.core.MINIMIZE
wxMAXIMIZE = wx.core.MAXIMIZE
wxCLOSE_BOX = wx.core.CLOSE_BOX
wxTHICK_FRAME = wx.core.THICK_FRAME
wxSYSTEM_MENU = wx.core.SYSTEM_MENU
wxMINIMIZE_BOX = wx.core.MINIMIZE_BOX
wxMAXIMIZE_BOX = wx.core.MAXIMIZE_BOX
wxTINY_CAPTION_HORIZ = wx.core.TINY_CAPTION_HORIZ
wxTINY_CAPTION_VERT = wx.core.TINY_CAPTION_VERT
wxRESIZE_BOX = wx.core.RESIZE_BOX
wxRESIZE_BORDER = wx.core.RESIZE_BORDER
wxDIALOG_MODAL = wx.core.DIALOG_MODAL
wxDIALOG_MODELESS = wx.core.DIALOG_MODELESS
wxDIALOG_NO_PARENT = wx.core.DIALOG_NO_PARENT
wxDEFAULT_FRAME_STYLE = wx.core.DEFAULT_FRAME_STYLE
wxDEFAULT_DIALOG_STYLE = wx.core.DEFAULT_DIALOG_STYLE
wxFRAME_TOOL_WINDOW = wx.core.FRAME_TOOL_WINDOW
wxFRAME_FLOAT_ON_PARENT = wx.core.FRAME_FLOAT_ON_PARENT
wxFRAME_NO_WINDOW_MENU = wx.core.FRAME_NO_WINDOW_MENU
wxFRAME_NO_TASKBAR = wx.core.FRAME_NO_TASKBAR
wxFRAME_SHAPED = wx.core.FRAME_SHAPED
wxED_CLIENT_MARGIN = wx.core.ED_CLIENT_MARGIN
wxED_BUTTONS_BOTTOM = wx.core.ED_BUTTONS_BOTTOM
wxED_BUTTONS_RIGHT = wx.core.ED_BUTTONS_RIGHT
wxED_STATIC_LINE = wx.core.ED_STATIC_LINE
wxEXT_DIALOG_STYLE = wx.core.EXT_DIALOG_STYLE
wxCLIP_CHILDREN = wx.core.CLIP_CHILDREN
wxCLIP_SIBLINGS = wx.core.CLIP_SIBLINGS
wxRETAINED = wx.core.RETAINED
wxBACKINGSTORE = wx.core.BACKINGSTORE
wxCOLOURED = wx.core.COLOURED
wxFIXED_LENGTH = wx.core.FIXED_LENGTH
wxLB_NEEDED_SB = wx.core.LB_NEEDED_SB
wxLB_ALWAYS_SB = wx.core.LB_ALWAYS_SB
wxLB_SORT = wx.core.LB_SORT
wxLB_SINGLE = wx.core.LB_SINGLE
wxLB_MULTIPLE = wx.core.LB_MULTIPLE
wxLB_EXTENDED = wx.core.LB_EXTENDED
wxLB_OWNERDRAW = wx.core.LB_OWNERDRAW
wxLB_HSCROLL = wx.core.LB_HSCROLL
wxPROCESS_ENTER = wx.core.PROCESS_ENTER
wxPASSWORD = wx.core.PASSWORD
wxCB_SIMPLE = wx.core.CB_SIMPLE
wxCB_DROPDOWN = wx.core.CB_DROPDOWN
wxCB_SORT = wx.core.CB_SORT
wxCB_READONLY = wx.core.CB_READONLY
wxRA_HORIZONTAL = wx.core.RA_HORIZONTAL
wxRA_VERTICAL = wx.core.RA_VERTICAL
wxRA_SPECIFY_ROWS = wx.core.RA_SPECIFY_ROWS
wxRA_SPECIFY_COLS = wx.core.RA_SPECIFY_COLS
wxRB_GROUP = wx.core.RB_GROUP
wxRB_SINGLE = wx.core.RB_SINGLE
wxSL_HORIZONTAL = wx.core.SL_HORIZONTAL
wxSL_VERTICAL = wx.core.SL_VERTICAL
wxSL_AUTOTICKS = wx.core.SL_AUTOTICKS
wxSL_LABELS = wx.core.SL_LABELS
wxSL_LEFT = wx.core.SL_LEFT
wxSL_TOP = wx.core.SL_TOP
wxSL_RIGHT = wx.core.SL_RIGHT
wxSL_BOTTOM = wx.core.SL_BOTTOM
wxSL_BOTH = wx.core.SL_BOTH
wxSL_SELRANGE = wx.core.SL_SELRANGE
wxSB_HORIZONTAL = wx.core.SB_HORIZONTAL
wxSB_VERTICAL = wx.core.SB_VERTICAL
wxST_SIZEGRIP = wx.core.ST_SIZEGRIP
wxST_NO_AUTORESIZE = wx.core.ST_NO_AUTORESIZE
wxFLOOD_SURFACE = wx.core.FLOOD_SURFACE
wxFLOOD_BORDER = wx.core.FLOOD_BORDER
wxODDEVEN_RULE = wx.core.ODDEVEN_RULE
wxWINDING_RULE = wx.core.WINDING_RULE
wxTOOL_TOP = wx.core.TOOL_TOP
wxTOOL_BOTTOM = wx.core.TOOL_BOTTOM
wxTOOL_LEFT = wx.core.TOOL_LEFT
wxTOOL_RIGHT = wx.core.TOOL_RIGHT
wxOK = wx.core.OK
wxYES_NO = wx.core.YES_NO
wxCANCEL = wx.core.CANCEL
wxYES = wx.core.YES
wxNO = wx.core.NO
wxNO_DEFAULT = wx.core.NO_DEFAULT
wxYES_DEFAULT = wx.core.YES_DEFAULT
wxICON_EXCLAMATION = wx.core.ICON_EXCLAMATION
wxICON_HAND = wx.core.ICON_HAND
wxICON_QUESTION = wx.core.ICON_QUESTION
wxICON_INFORMATION = wx.core.ICON_INFORMATION
wxICON_STOP = wx.core.ICON_STOP
wxICON_ASTERISK = wx.core.ICON_ASTERISK
wxICON_MASK = wx.core.ICON_MASK
wxICON_WARNING = wx.core.ICON_WARNING
wxICON_ERROR = wx.core.ICON_ERROR
wxFORWARD = wx.core.FORWARD
wxBACKWARD = wx.core.BACKWARD
wxRESET = wx.core.RESET
wxHELP = wx.core.HELP
wxMORE = wx.core.MORE
wxSETUP = wx.core.SETUP
wxSIZE_AUTO_WIDTH = wx.core.SIZE_AUTO_WIDTH
wxSIZE_AUTO_HEIGHT = wx.core.SIZE_AUTO_HEIGHT
wxSIZE_AUTO = wx.core.SIZE_AUTO
wxSIZE_USE_EXISTING = wx.core.SIZE_USE_EXISTING
wxSIZE_ALLOW_MINUS_ONE = wx.core.SIZE_ALLOW_MINUS_ONE
wxPORTRAIT = wx.core.PORTRAIT
wxLANDSCAPE = wx.core.LANDSCAPE
wxPRINT_QUALITY_HIGH = wx.core.PRINT_QUALITY_HIGH
wxPRINT_QUALITY_MEDIUM = wx.core.PRINT_QUALITY_MEDIUM
wxPRINT_QUALITY_LOW = wx.core.PRINT_QUALITY_LOW
wxPRINT_QUALITY_DRAFT = wx.core.PRINT_QUALITY_DRAFT
wxID_ANY = wx.core.ID_ANY
wxID_SEPARATOR = wx.core.ID_SEPARATOR
wxID_LOWEST = wx.core.ID_LOWEST
wxID_OPEN = wx.core.ID_OPEN
wxID_CLOSE = wx.core.ID_CLOSE
wxID_NEW = wx.core.ID_NEW
wxID_SAVE = wx.core.ID_SAVE
wxID_SAVEAS = wx.core.ID_SAVEAS
wxID_REVERT = wx.core.ID_REVERT
wxID_EXIT = wx.core.ID_EXIT
wxID_UNDO = wx.core.ID_UNDO
wxID_REDO = wx.core.ID_REDO
wxID_HELP = wx.core.ID_HELP
wxID_PRINT = wx.core.ID_PRINT
wxID_PRINT_SETUP = wx.core.ID_PRINT_SETUP
wxID_PREVIEW = wx.core.ID_PREVIEW
wxID_ABOUT = wx.core.ID_ABOUT
wxID_HELP_CONTENTS = wx.core.ID_HELP_CONTENTS
wxID_HELP_COMMANDS = wx.core.ID_HELP_COMMANDS
wxID_HELP_PROCEDURES = wx.core.ID_HELP_PROCEDURES
wxID_HELP_CONTEXT = wx.core.ID_HELP_CONTEXT
wxID_CLOSE_ALL = wx.core.ID_CLOSE_ALL
wxID_PREFERENCES = wx.core.ID_PREFERENCES
wxID_CUT = wx.core.ID_CUT
wxID_COPY = wx.core.ID_COPY
wxID_PASTE = wx.core.ID_PASTE
wxID_CLEAR = wx.core.ID_CLEAR
wxID_FIND = wx.core.ID_FIND
wxID_DUPLICATE = wx.core.ID_DUPLICATE
wxID_SELECTALL = wx.core.ID_SELECTALL
wxID_DELETE = wx.core.ID_DELETE
wxID_REPLACE = wx.core.ID_REPLACE
wxID_REPLACE_ALL = wx.core.ID_REPLACE_ALL
wxID_PROPERTIES = wx.core.ID_PROPERTIES
wxID_VIEW_DETAILS = wx.core.ID_VIEW_DETAILS
wxID_VIEW_LARGEICONS = wx.core.ID_VIEW_LARGEICONS
wxID_VIEW_SMALLICONS = wx.core.ID_VIEW_SMALLICONS
wxID_VIEW_LIST = wx.core.ID_VIEW_LIST
wxID_VIEW_SORTDATE = wx.core.ID_VIEW_SORTDATE
wxID_VIEW_SORTNAME = wx.core.ID_VIEW_SORTNAME
wxID_VIEW_SORTSIZE = wx.core.ID_VIEW_SORTSIZE
wxID_VIEW_SORTTYPE = wx.core.ID_VIEW_SORTTYPE
wxID_FILE1 = wx.core.ID_FILE1
wxID_FILE2 = wx.core.ID_FILE2
wxID_FILE3 = wx.core.ID_FILE3
wxID_FILE4 = wx.core.ID_FILE4
wxID_FILE5 = wx.core.ID_FILE5
wxID_FILE6 = wx.core.ID_FILE6
wxID_FILE7 = wx.core.ID_FILE7
wxID_FILE8 = wx.core.ID_FILE8
wxID_FILE9 = wx.core.ID_FILE9
wxID_OK = wx.core.ID_OK
wxID_CANCEL = wx.core.ID_CANCEL
wxID_APPLY = wx.core.ID_APPLY
wxID_YES = wx.core.ID_YES
wxID_NO = wx.core.ID_NO
wxID_STATIC = wx.core.ID_STATIC
wxID_FORWARD = wx.core.ID_FORWARD
wxID_BACKWARD = wx.core.ID_BACKWARD
wxID_DEFAULT = wx.core.ID_DEFAULT
wxID_MORE = wx.core.ID_MORE
wxID_SETUP = wx.core.ID_SETUP
wxID_RESET = wx.core.ID_RESET
wxID_CONTEXT_HELP = wx.core.ID_CONTEXT_HELP
wxID_YESTOALL = wx.core.ID_YESTOALL
wxID_NOTOALL = wx.core.ID_NOTOALL
wxID_ABORT = wx.core.ID_ABORT
wxID_RETRY = wx.core.ID_RETRY
wxID_IGNORE = wx.core.ID_IGNORE
wxID_HIGHEST = wx.core.ID_HIGHEST
wxOPEN = wx.core.OPEN
wxSAVE = wx.core.SAVE
wxHIDE_READONLY = wx.core.HIDE_READONLY
wxOVERWRITE_PROMPT = wx.core.OVERWRITE_PROMPT
wxFILE_MUST_EXIST = wx.core.FILE_MUST_EXIST
wxMULTIPLE = wx.core.MULTIPLE
wxCHANGE_DIR = wx.core.CHANGE_DIR
wxACCEL_ALT = wx.core.ACCEL_ALT
wxACCEL_CTRL = wx.core.ACCEL_CTRL
wxACCEL_SHIFT = wx.core.ACCEL_SHIFT
wxACCEL_NORMAL = wx.core.ACCEL_NORMAL
wxPD_AUTO_HIDE = wx.core.PD_AUTO_HIDE
wxPD_APP_MODAL = wx.core.PD_APP_MODAL
wxPD_CAN_ABORT = wx.core.PD_CAN_ABORT
wxPD_ELAPSED_TIME = wx.core.PD_ELAPSED_TIME
wxPD_ESTIMATED_TIME = wx.core.PD_ESTIMATED_TIME
wxPD_REMAINING_TIME = wx.core.PD_REMAINING_TIME
wxDD_NEW_DIR_BUTTON = wx.core.DD_NEW_DIR_BUTTON
wxDD_DEFAULT_STYLE = wx.core.DD_DEFAULT_STYLE
wxMENU_TEAROFF = wx.core.MENU_TEAROFF
wxMB_DOCKABLE = wx.core.MB_DOCKABLE
wxNO_FULL_REPAINT_ON_RESIZE = wx.core.NO_FULL_REPAINT_ON_RESIZE
wxLI_HORIZONTAL = wx.core.LI_HORIZONTAL
wxLI_VERTICAL = wx.core.LI_VERTICAL
wxWS_EX_VALIDATE_RECURSIVELY = wx.core.WS_EX_VALIDATE_RECURSIVELY
wxWS_EX_BLOCK_EVENTS = wx.core.WS_EX_BLOCK_EVENTS
wxWS_EX_TRANSIENT = wx.core.WS_EX_TRANSIENT
wxWS_EX_THEMED_BACKGROUND = wx.core.WS_EX_THEMED_BACKGROUND
wxWS_EX_PROCESS_IDLE = wx.core.WS_EX_PROCESS_IDLE
wxWS_EX_PROCESS_UI_UPDATES = wx.core.WS_EX_PROCESS_UI_UPDATES
wxMM_TEXT = wx.core.MM_TEXT
wxMM_LOMETRIC = wx.core.MM_LOMETRIC
wxMM_HIMETRIC = wx.core.MM_HIMETRIC
wxMM_LOENGLISH = wx.core.MM_LOENGLISH
wxMM_HIENGLISH = wx.core.MM_HIENGLISH
wxMM_TWIPS = wx.core.MM_TWIPS
wxMM_ISOTROPIC = wx.core.MM_ISOTROPIC
wxMM_ANISOTROPIC = wx.core.MM_ANISOTROPIC
wxMM_POINTS = wx.core.MM_POINTS
wxMM_METRIC = wx.core.MM_METRIC
wxCENTRE = wx.core.CENTRE
wxCENTER = wx.core.CENTER
wxHORIZONTAL = wx.core.HORIZONTAL
wxVERTICAL = wx.core.VERTICAL
wxBOTH = wx.core.BOTH
wxLEFT = wx.core.LEFT
wxRIGHT = wx.core.RIGHT
wxUP = wx.core.UP
wxDOWN = wx.core.DOWN
wxTOP = wx.core.TOP
wxBOTTOM = wx.core.BOTTOM
wxNORTH = wx.core.NORTH
wxSOUTH = wx.core.SOUTH
wxWEST = wx.core.WEST
wxEAST = wx.core.EAST
wxALL = wx.core.ALL
wxALIGN_NOT = wx.core.ALIGN_NOT
wxALIGN_CENTER_HORIZONTAL = wx.core.ALIGN_CENTER_HORIZONTAL
wxALIGN_CENTRE_HORIZONTAL = wx.core.ALIGN_CENTRE_HORIZONTAL
wxALIGN_LEFT = wx.core.ALIGN_LEFT
wxALIGN_TOP = wx.core.ALIGN_TOP
wxALIGN_RIGHT = wx.core.ALIGN_RIGHT
wxALIGN_BOTTOM = wx.core.ALIGN_BOTTOM
wxALIGN_CENTER_VERTICAL = wx.core.ALIGN_CENTER_VERTICAL
wxALIGN_CENTRE_VERTICAL = wx.core.ALIGN_CENTRE_VERTICAL
wxALIGN_CENTER = wx.core.ALIGN_CENTER
wxALIGN_CENTRE = wx.core.ALIGN_CENTRE
wxALIGN_MASK = wx.core.ALIGN_MASK
wxSTRETCH_NOT = wx.core.STRETCH_NOT
wxSHRINK = wx.core.SHRINK
wxGROW = wx.core.GROW
wxEXPAND = wx.core.EXPAND
wxSHAPED = wx.core.SHAPED
wxADJUST_MINSIZE = wx.core.ADJUST_MINSIZE
wxTILE = wx.core.TILE
wxBORDER_DEFAULT = wx.core.BORDER_DEFAULT
wxBORDER_NONE = wx.core.BORDER_NONE
wxBORDER_STATIC = wx.core.BORDER_STATIC
wxBORDER_SIMPLE = wx.core.BORDER_SIMPLE
wxBORDER_RAISED = wx.core.BORDER_RAISED
wxBORDER_SUNKEN = wx.core.BORDER_SUNKEN
wxBORDER_DOUBLE = wx.core.BORDER_DOUBLE
wxBORDER_MASK = wx.core.BORDER_MASK
wxDEFAULT = wx.core.DEFAULT
wxDECORATIVE = wx.core.DECORATIVE
wxROMAN = wx.core.ROMAN
wxSCRIPT = wx.core.SCRIPT
wxSWISS = wx.core.SWISS
wxMODERN = wx.core.MODERN
wxTELETYPE = wx.core.TELETYPE
wxVARIABLE = wx.core.VARIABLE
wxFIXED = wx.core.FIXED
wxNORMAL = wx.core.NORMAL
wxLIGHT = wx.core.LIGHT
wxBOLD = wx.core.BOLD
wxITALIC = wx.core.ITALIC
wxSLANT = wx.core.SLANT
wxSOLID = wx.core.SOLID
wxDOT = wx.core.DOT
wxLONG_DASH = wx.core.LONG_DASH
wxSHORT_DASH = wx.core.SHORT_DASH
wxDOT_DASH = wx.core.DOT_DASH
wxUSER_DASH = wx.core.USER_DASH
wxTRANSPARENT = wx.core.TRANSPARENT
wxSTIPPLE = wx.core.STIPPLE
wxBDIAGONAL_HATCH = wx.core.BDIAGONAL_HATCH
wxCROSSDIAG_HATCH = wx.core.CROSSDIAG_HATCH
wxFDIAGONAL_HATCH = wx.core.FDIAGONAL_HATCH
wxCROSS_HATCH = wx.core.CROSS_HATCH
wxHORIZONTAL_HATCH = wx.core.HORIZONTAL_HATCH
wxVERTICAL_HATCH = wx.core.VERTICAL_HATCH
wxJOIN_BEVEL = wx.core.JOIN_BEVEL
wxJOIN_MITER = wx.core.JOIN_MITER
wxJOIN_ROUND = wx.core.JOIN_ROUND
wxCAP_ROUND = wx.core.CAP_ROUND
wxCAP_PROJECTING = wx.core.CAP_PROJECTING
wxCAP_BUTT = wx.core.CAP_BUTT
wxCLEAR = wx.core.CLEAR
wxXOR = wx.core.XOR
wxINVERT = wx.core.INVERT
wxOR_REVERSE = wx.core.OR_REVERSE
wxAND_REVERSE = wx.core.AND_REVERSE
wxCOPY = wx.core.COPY
wxAND = wx.core.AND
wxAND_INVERT = wx.core.AND_INVERT
wxNO_OP = wx.core.NO_OP
wxNOR = wx.core.NOR
wxEQUIV = wx.core.EQUIV
wxSRC_INVERT = wx.core.SRC_INVERT
wxOR_INVERT = wx.core.OR_INVERT
wxNAND = wx.core.NAND
wxOR = wx.core.OR
wxSET = wx.core.SET
WXK_BACK = wx.core.WXK_BACK
WXK_TAB = wx.core.WXK_TAB
WXK_RETURN = wx.core.WXK_RETURN
WXK_ESCAPE = wx.core.WXK_ESCAPE
WXK_SPACE = wx.core.WXK_SPACE
WXK_DELETE = wx.core.WXK_DELETE
WXK_START = wx.core.WXK_START
WXK_LBUTTON = wx.core.WXK_LBUTTON
WXK_RBUTTON = wx.core.WXK_RBUTTON
WXK_CANCEL = wx.core.WXK_CANCEL
WXK_MBUTTON = wx.core.WXK_MBUTTON
WXK_CLEAR = wx.core.WXK_CLEAR
WXK_SHIFT = wx.core.WXK_SHIFT
WXK_ALT = wx.core.WXK_ALT
WXK_CONTROL = wx.core.WXK_CONTROL
WXK_MENU = wx.core.WXK_MENU
WXK_PAUSE = wx.core.WXK_PAUSE
WXK_CAPITAL = wx.core.WXK_CAPITAL
WXK_PRIOR = wx.core.WXK_PRIOR
WXK_NEXT = wx.core.WXK_NEXT
WXK_END = wx.core.WXK_END
WXK_HOME = wx.core.WXK_HOME
WXK_LEFT = wx.core.WXK_LEFT
WXK_UP = wx.core.WXK_UP
WXK_RIGHT = wx.core.WXK_RIGHT
WXK_DOWN = wx.core.WXK_DOWN
WXK_SELECT = wx.core.WXK_SELECT
WXK_PRINT = wx.core.WXK_PRINT
WXK_EXECUTE = wx.core.WXK_EXECUTE
WXK_SNAPSHOT = wx.core.WXK_SNAPSHOT
WXK_INSERT = wx.core.WXK_INSERT
WXK_HELP = wx.core.WXK_HELP
WXK_NUMPAD0 = wx.core.WXK_NUMPAD0
WXK_NUMPAD1 = wx.core.WXK_NUMPAD1
WXK_NUMPAD2 = wx.core.WXK_NUMPAD2
WXK_NUMPAD3 = wx.core.WXK_NUMPAD3
WXK_NUMPAD4 = wx.core.WXK_NUMPAD4
WXK_NUMPAD5 = wx.core.WXK_NUMPAD5
WXK_NUMPAD6 = wx.core.WXK_NUMPAD6
WXK_NUMPAD7 = wx.core.WXK_NUMPAD7
WXK_NUMPAD8 = wx.core.WXK_NUMPAD8
WXK_NUMPAD9 = wx.core.WXK_NUMPAD9
WXK_MULTIPLY = wx.core.WXK_MULTIPLY
WXK_ADD = wx.core.WXK_ADD
WXK_SEPARATOR = wx.core.WXK_SEPARATOR
WXK_SUBTRACT = wx.core.WXK_SUBTRACT
WXK_DECIMAL = wx.core.WXK_DECIMAL
WXK_DIVIDE = wx.core.WXK_DIVIDE
WXK_F1 = wx.core.WXK_F1
WXK_F2 = wx.core.WXK_F2
WXK_F3 = wx.core.WXK_F3
WXK_F4 = wx.core.WXK_F4
WXK_F5 = wx.core.WXK_F5
WXK_F6 = wx.core.WXK_F6
WXK_F7 = wx.core.WXK_F7
WXK_F8 = wx.core.WXK_F8
WXK_F9 = wx.core.WXK_F9
WXK_F10 = wx.core.WXK_F10
WXK_F11 = wx.core.WXK_F11
WXK_F12 = wx.core.WXK_F12
WXK_F13 = wx.core.WXK_F13
WXK_F14 = wx.core.WXK_F14
WXK_F15 = wx.core.WXK_F15
WXK_F16 = wx.core.WXK_F16
WXK_F17 = wx.core.WXK_F17
WXK_F18 = wx.core.WXK_F18
WXK_F19 = wx.core.WXK_F19
WXK_F20 = wx.core.WXK_F20
WXK_F21 = wx.core.WXK_F21
WXK_F22 = wx.core.WXK_F22
WXK_F23 = wx.core.WXK_F23
WXK_F24 = wx.core.WXK_F24
WXK_NUMLOCK = wx.core.WXK_NUMLOCK
WXK_SCROLL = wx.core.WXK_SCROLL
WXK_PAGEUP = wx.core.WXK_PAGEUP
WXK_PAGEDOWN = wx.core.WXK_PAGEDOWN
WXK_NUMPAD_SPACE = wx.core.WXK_NUMPAD_SPACE
WXK_NUMPAD_TAB = wx.core.WXK_NUMPAD_TAB
WXK_NUMPAD_ENTER = wx.core.WXK_NUMPAD_ENTER
WXK_NUMPAD_F1 = wx.core.WXK_NUMPAD_F1
WXK_NUMPAD_F2 = wx.core.WXK_NUMPAD_F2
WXK_NUMPAD_F3 = wx.core.WXK_NUMPAD_F3
WXK_NUMPAD_F4 = wx.core.WXK_NUMPAD_F4
WXK_NUMPAD_HOME = wx.core.WXK_NUMPAD_HOME
WXK_NUMPAD_LEFT = wx.core.WXK_NUMPAD_LEFT
WXK_NUMPAD_UP = wx.core.WXK_NUMPAD_UP
WXK_NUMPAD_RIGHT = wx.core.WXK_NUMPAD_RIGHT
WXK_NUMPAD_DOWN = wx.core.WXK_NUMPAD_DOWN
WXK_NUMPAD_PRIOR = wx.core.WXK_NUMPAD_PRIOR
WXK_NUMPAD_PAGEUP = wx.core.WXK_NUMPAD_PAGEUP
WXK_NUMPAD_NEXT = wx.core.WXK_NUMPAD_NEXT
WXK_NUMPAD_PAGEDOWN = wx.core.WXK_NUMPAD_PAGEDOWN
WXK_NUMPAD_END = wx.core.WXK_NUMPAD_END
WXK_NUMPAD_BEGIN = wx.core.WXK_NUMPAD_BEGIN
WXK_NUMPAD_INSERT = wx.core.WXK_NUMPAD_INSERT
WXK_NUMPAD_DELETE = wx.core.WXK_NUMPAD_DELETE
WXK_NUMPAD_EQUAL = wx.core.WXK_NUMPAD_EQUAL
WXK_NUMPAD_MULTIPLY = wx.core.WXK_NUMPAD_MULTIPLY
WXK_NUMPAD_ADD = wx.core.WXK_NUMPAD_ADD
WXK_NUMPAD_SEPARATOR = wx.core.WXK_NUMPAD_SEPARATOR
WXK_NUMPAD_SUBTRACT = wx.core.WXK_NUMPAD_SUBTRACT
WXK_NUMPAD_DECIMAL = wx.core.WXK_NUMPAD_DECIMAL
WXK_NUMPAD_DIVIDE = wx.core.WXK_NUMPAD_DIVIDE
WXK_WINDOWS_LEFT = wx.core.WXK_WINDOWS_LEFT
WXK_WINDOWS_RIGHT = wx.core.WXK_WINDOWS_RIGHT
WXK_WINDOWS_MENU = wx.core.WXK_WINDOWS_MENU
wxPAPER_NONE = wx.core.PAPER_NONE
wxPAPER_LETTER = wx.core.PAPER_LETTER
wxPAPER_LEGAL = wx.core.PAPER_LEGAL
wxPAPER_A4 = wx.core.PAPER_A4
wxPAPER_CSHEET = wx.core.PAPER_CSHEET
wxPAPER_DSHEET = wx.core.PAPER_DSHEET
wxPAPER_ESHEET = wx.core.PAPER_ESHEET
wxPAPER_LETTERSMALL = wx.core.PAPER_LETTERSMALL
wxPAPER_TABLOID = wx.core.PAPER_TABLOID
wxPAPER_LEDGER = wx.core.PAPER_LEDGER
wxPAPER_STATEMENT = wx.core.PAPER_STATEMENT
wxPAPER_EXECUTIVE = wx.core.PAPER_EXECUTIVE
wxPAPER_A3 = wx.core.PAPER_A3
wxPAPER_A4SMALL = wx.core.PAPER_A4SMALL
wxPAPER_A5 = wx.core.PAPER_A5
wxPAPER_B4 = wx.core.PAPER_B4
wxPAPER_B5 = wx.core.PAPER_B5
wxPAPER_FOLIO = wx.core.PAPER_FOLIO
wxPAPER_QUARTO = wx.core.PAPER_QUARTO
wxPAPER_10X14 = wx.core.PAPER_10X14
wxPAPER_11X17 = wx.core.PAPER_11X17
wxPAPER_NOTE = wx.core.PAPER_NOTE
wxPAPER_ENV_9 = wx.core.PAPER_ENV_9
wxPAPER_ENV_10 = wx.core.PAPER_ENV_10
wxPAPER_ENV_11 = wx.core.PAPER_ENV_11
wxPAPER_ENV_12 = wx.core.PAPER_ENV_12
wxPAPER_ENV_14 = wx.core.PAPER_ENV_14
wxPAPER_ENV_DL = wx.core.PAPER_ENV_DL
wxPAPER_ENV_C5 = wx.core.PAPER_ENV_C5
wxPAPER_ENV_C3 = wx.core.PAPER_ENV_C3
wxPAPER_ENV_C4 = wx.core.PAPER_ENV_C4
wxPAPER_ENV_C6 = wx.core.PAPER_ENV_C6
wxPAPER_ENV_C65 = wx.core.PAPER_ENV_C65
wxPAPER_ENV_B4 = wx.core.PAPER_ENV_B4
wxPAPER_ENV_B5 = wx.core.PAPER_ENV_B5
wxPAPER_ENV_B6 = wx.core.PAPER_ENV_B6
wxPAPER_ENV_ITALY = wx.core.PAPER_ENV_ITALY
wxPAPER_ENV_MONARCH = wx.core.PAPER_ENV_MONARCH
wxPAPER_ENV_PERSONAL = wx.core.PAPER_ENV_PERSONAL
wxPAPER_FANFOLD_US = wx.core.PAPER_FANFOLD_US
wxPAPER_FANFOLD_STD_GERMAN = wx.core.PAPER_FANFOLD_STD_GERMAN
wxPAPER_FANFOLD_LGL_GERMAN = wx.core.PAPER_FANFOLD_LGL_GERMAN
wxPAPER_ISO_B4 = wx.core.PAPER_ISO_B4
wxPAPER_JAPANESE_POSTCARD = wx.core.PAPER_JAPANESE_POSTCARD
wxPAPER_9X11 = wx.core.PAPER_9X11
wxPAPER_10X11 = wx.core.PAPER_10X11
wxPAPER_15X11 = wx.core.PAPER_15X11
wxPAPER_ENV_INVITE = wx.core.PAPER_ENV_INVITE
wxPAPER_LETTER_EXTRA = wx.core.PAPER_LETTER_EXTRA
wxPAPER_LEGAL_EXTRA = wx.core.PAPER_LEGAL_EXTRA
wxPAPER_TABLOID_EXTRA = wx.core.PAPER_TABLOID_EXTRA
wxPAPER_A4_EXTRA = wx.core.PAPER_A4_EXTRA
wxPAPER_LETTER_TRANSVERSE = wx.core.PAPER_LETTER_TRANSVERSE
wxPAPER_A4_TRANSVERSE = wx.core.PAPER_A4_TRANSVERSE
wxPAPER_LETTER_EXTRA_TRANSVERSE = wx.core.PAPER_LETTER_EXTRA_TRANSVERSE
wxPAPER_A_PLUS = wx.core.PAPER_A_PLUS
wxPAPER_B_PLUS = wx.core.PAPER_B_PLUS
wxPAPER_LETTER_PLUS = wx.core.PAPER_LETTER_PLUS
wxPAPER_A4_PLUS = wx.core.PAPER_A4_PLUS
wxPAPER_A5_TRANSVERSE = wx.core.PAPER_A5_TRANSVERSE
wxPAPER_B5_TRANSVERSE = wx.core.PAPER_B5_TRANSVERSE
wxPAPER_A3_EXTRA = wx.core.PAPER_A3_EXTRA
wxPAPER_A5_EXTRA = wx.core.PAPER_A5_EXTRA
wxPAPER_B5_EXTRA = wx.core.PAPER_B5_EXTRA
wxPAPER_A2 = wx.core.PAPER_A2
wxPAPER_A3_TRANSVERSE = wx.core.PAPER_A3_TRANSVERSE
wxPAPER_A3_EXTRA_TRANSVERSE = wx.core.PAPER_A3_EXTRA_TRANSVERSE
wxDUPLEX_SIMPLEX = wx.core.DUPLEX_SIMPLEX
wxDUPLEX_HORIZONTAL = wx.core.DUPLEX_HORIZONTAL
wxDUPLEX_VERTICAL = wx.core.DUPLEX_VERTICAL
wxITEM_SEPARATOR = wx.core.ITEM_SEPARATOR
wxITEM_NORMAL = wx.core.ITEM_NORMAL
wxITEM_CHECK = wx.core.ITEM_CHECK
wxITEM_RADIO = wx.core.ITEM_RADIO
wxITEM_MAX = wx.core.ITEM_MAX
wxHT_NOWHERE = wx.core.HT_NOWHERE
wxHT_SCROLLBAR_FIRST = wx.core.HT_SCROLLBAR_FIRST
wxHT_SCROLLBAR_ARROW_LINE_1 = wx.core.HT_SCROLLBAR_ARROW_LINE_1
wxHT_SCROLLBAR_ARROW_LINE_2 = wx.core.HT_SCROLLBAR_ARROW_LINE_2
wxHT_SCROLLBAR_ARROW_PAGE_1 = wx.core.HT_SCROLLBAR_ARROW_PAGE_1
wxHT_SCROLLBAR_ARROW_PAGE_2 = wx.core.HT_SCROLLBAR_ARROW_PAGE_2
wxHT_SCROLLBAR_THUMB = wx.core.HT_SCROLLBAR_THUMB
wxHT_SCROLLBAR_BAR_1 = wx.core.HT_SCROLLBAR_BAR_1
wxHT_SCROLLBAR_BAR_2 = wx.core.HT_SCROLLBAR_BAR_2
wxHT_SCROLLBAR_LAST = wx.core.HT_SCROLLBAR_LAST
wxHT_WINDOW_OUTSIDE = wx.core.HT_WINDOW_OUTSIDE
wxHT_WINDOW_INSIDE = wx.core.HT_WINDOW_INSIDE
wxHT_WINDOW_VERT_SCROLLBAR = wx.core.HT_WINDOW_VERT_SCROLLBAR
wxHT_WINDOW_HORZ_SCROLLBAR = wx.core.HT_WINDOW_HORZ_SCROLLBAR
wxHT_WINDOW_CORNER = wx.core.HT_WINDOW_CORNER
wxHT_MAX = wx.core.HT_MAX
wxMOD_NONE = wx.core.MOD_NONE
wxMOD_ALT = wx.core.MOD_ALT
wxMOD_CONTROL = wx.core.MOD_CONTROL
wxMOD_SHIFT = wx.core.MOD_SHIFT
wxMOD_WIN = wx.core.MOD_WIN
wxUPDATE_UI_NONE = wx.core.UPDATE_UI_NONE
wxUPDATE_UI_RECURSE = wx.core.UPDATE_UI_RECURSE
wxUPDATE_UI_FROMIDLE = wx.core.UPDATE_UI_FROMIDLE
wxObject = wx.core.Object
wxBITMAP_TYPE_INVALID = wx.core.BITMAP_TYPE_INVALID
wxBITMAP_TYPE_BMP = wx.core.BITMAP_TYPE_BMP
wxBITMAP_TYPE_BMP_RESOURCE = wx.core.BITMAP_TYPE_BMP_RESOURCE
wxBITMAP_TYPE_RESOURCE = wx.core.BITMAP_TYPE_RESOURCE
wxBITMAP_TYPE_ICO = wx.core.BITMAP_TYPE_ICO
wxBITMAP_TYPE_ICO_RESOURCE = wx.core.BITMAP_TYPE_ICO_RESOURCE
wxBITMAP_TYPE_CUR = wx.core.BITMAP_TYPE_CUR
wxBITMAP_TYPE_CUR_RESOURCE = wx.core.BITMAP_TYPE_CUR_RESOURCE
wxBITMAP_TYPE_XBM = wx.core.BITMAP_TYPE_XBM
wxBITMAP_TYPE_XBM_DATA = wx.core.BITMAP_TYPE_XBM_DATA
wxBITMAP_TYPE_XPM = wx.core.BITMAP_TYPE_XPM
wxBITMAP_TYPE_XPM_DATA = wx.core.BITMAP_TYPE_XPM_DATA
wxBITMAP_TYPE_TIF = wx.core.BITMAP_TYPE_TIF
wxBITMAP_TYPE_TIF_RESOURCE = wx.core.BITMAP_TYPE_TIF_RESOURCE
wxBITMAP_TYPE_GIF = wx.core.BITMAP_TYPE_GIF
wxBITMAP_TYPE_GIF_RESOURCE = wx.core.BITMAP_TYPE_GIF_RESOURCE
wxBITMAP_TYPE_PNG = wx.core.BITMAP_TYPE_PNG
wxBITMAP_TYPE_PNG_RESOURCE = wx.core.BITMAP_TYPE_PNG_RESOURCE
wxBITMAP_TYPE_JPEG = wx.core.BITMAP_TYPE_JPEG
wxBITMAP_TYPE_JPEG_RESOURCE = wx.core.BITMAP_TYPE_JPEG_RESOURCE
wxBITMAP_TYPE_PNM = wx.core.BITMAP_TYPE_PNM
wxBITMAP_TYPE_PNM_RESOURCE = wx.core.BITMAP_TYPE_PNM_RESOURCE
wxBITMAP_TYPE_PCX = wx.core.BITMAP_TYPE_PCX
wxBITMAP_TYPE_PCX_RESOURCE = wx.core.BITMAP_TYPE_PCX_RESOURCE
wxBITMAP_TYPE_PICT = wx.core.BITMAP_TYPE_PICT
wxBITMAP_TYPE_PICT_RESOURCE = wx.core.BITMAP_TYPE_PICT_RESOURCE
wxBITMAP_TYPE_ICON = wx.core.BITMAP_TYPE_ICON
wxBITMAP_TYPE_ICON_RESOURCE = wx.core.BITMAP_TYPE_ICON_RESOURCE
wxBITMAP_TYPE_ANI = wx.core.BITMAP_TYPE_ANI
wxBITMAP_TYPE_IFF = wx.core.BITMAP_TYPE_IFF
wxBITMAP_TYPE_MACCURSOR = wx.core.BITMAP_TYPE_MACCURSOR
wxBITMAP_TYPE_MACCURSOR_RESOURCE = wx.core.BITMAP_TYPE_MACCURSOR_RESOURCE
wxBITMAP_TYPE_ANY = wx.core.BITMAP_TYPE_ANY
wxCURSOR_NONE = wx.core.CURSOR_NONE
wxCURSOR_ARROW = wx.core.CURSOR_ARROW
wxCURSOR_RIGHT_ARROW = wx.core.CURSOR_RIGHT_ARROW
wxCURSOR_BULLSEYE = wx.core.CURSOR_BULLSEYE
wxCURSOR_CHAR = wx.core.CURSOR_CHAR
wxCURSOR_CROSS = wx.core.CURSOR_CROSS
wxCURSOR_HAND = wx.core.CURSOR_HAND
wxCURSOR_IBEAM = wx.core.CURSOR_IBEAM
wxCURSOR_LEFT_BUTTON = wx.core.CURSOR_LEFT_BUTTON
wxCURSOR_MAGNIFIER = wx.core.CURSOR_MAGNIFIER
wxCURSOR_MIDDLE_BUTTON = wx.core.CURSOR_MIDDLE_BUTTON
wxCURSOR_NO_ENTRY = wx.core.CURSOR_NO_ENTRY
wxCURSOR_PAINT_BRUSH = wx.core.CURSOR_PAINT_BRUSH
wxCURSOR_PENCIL = wx.core.CURSOR_PENCIL
wxCURSOR_POINT_LEFT = wx.core.CURSOR_POINT_LEFT
wxCURSOR_POINT_RIGHT = wx.core.CURSOR_POINT_RIGHT
wxCURSOR_QUESTION_ARROW = wx.core.CURSOR_QUESTION_ARROW
wxCURSOR_RIGHT_BUTTON = wx.core.CURSOR_RIGHT_BUTTON
wxCURSOR_SIZENESW = wx.core.CURSOR_SIZENESW
wxCURSOR_SIZENS = wx.core.CURSOR_SIZENS
wxCURSOR_SIZENWSE = wx.core.CURSOR_SIZENWSE
wxCURSOR_SIZEWE = wx.core.CURSOR_SIZEWE
wxCURSOR_SIZING = wx.core.CURSOR_SIZING
wxCURSOR_SPRAYCAN = wx.core.CURSOR_SPRAYCAN
wxCURSOR_WAIT = wx.core.CURSOR_WAIT
wxCURSOR_WATCH = wx.core.CURSOR_WATCH
wxCURSOR_BLANK = wx.core.CURSOR_BLANK
wxCURSOR_DEFAULT = wx.core.CURSOR_DEFAULT
wxCURSOR_ARROWWAIT = wx.core.CURSOR_ARROWWAIT
wxCURSOR_MAX = wx.core.CURSOR_MAX
wxSize = wx.core.Size
wxRealPoint = wx.core.RealPoint
wxPoint = wx.core.Point
wxRectPP = wx.core.RectPP
wxRectPS = wx.core.RectPS
wxRect = wx.core.Rect
wxIntersectRect = wx.core.IntersectRect
wxPoint2DCopy = wx.core.Point2DCopy
wxPoint2DFromPoint = wx.core.Point2DFromPoint
wxPoint2D = wx.core.Point2D
wxDefaultPosition = wx.core.DefaultPosition
wxDefaultSize = wx.core.DefaultSize
wxFromStart = wx.core.FromStart
wxFromCurrent = wx.core.FromCurrent
wxFromEnd = wx.core.FromEnd
wxInputStream = wx.core.InputStream
wxOutputStream = wx.core.OutputStream
wxFSFile = wx.core.FSFile
wxCPPFileSystemHandler = wx.core.CPPFileSystemHandler
wxFileSystemHandler = wx.core.FileSystemHandler
wxFileSystem = wx.core.FileSystem
wxFileSystem_URLToFileName = wx.core.FileSystem_URLToFileName
wxInternetFSHandler = wx.core.InternetFSHandler
wxZipFSHandler = wx.core.ZipFSHandler
__wxMemoryFSHandler_AddFile_wxImage = wx.core.__wxMemoryFSHandler_AddFile_wxImage
__wxMemoryFSHandler_AddFile_wxBitmap = wx.core.__wxMemoryFSHandler_AddFile_wxBitmap
__wxMemoryFSHandler_AddFile_Data = wx.core.__wxMemoryFSHandler_AddFile_Data
wxMemoryFSHandler = wx.core.MemoryFSHandler
wxImageHandler = wx.core.ImageHandler
wxImageHistogram = wx.core.ImageHistogram
wxImageFromMime = wx.core.ImageFromMime
wxImageFromStream = wx.core.ImageFromStream
wxImageFromStreamMime = wx.core.ImageFromStreamMime
wxEmptyImage = wx.core.EmptyImage
wxImageFromBitmap = wx.core.ImageFromBitmap
wxImageFromData = wx.core.ImageFromData
wxImage = wx.core.Image
wxInitAllImageHandlers = wx.core.InitAllImageHandlers
wxNullImage = wx.core.NullImage
wxIMAGE_OPTION_BMP_FORMAT = wx.core.IMAGE_OPTION_BMP_FORMAT
wxIMAGE_OPTION_CUR_HOTSPOT_X = wx.core.IMAGE_OPTION_CUR_HOTSPOT_X
wxIMAGE_OPTION_CUR_HOTSPOT_Y = wx.core.IMAGE_OPTION_CUR_HOTSPOT_Y
wxBMP_24BPP = wx.core.BMP_24BPP
wxBMP_8BPP = wx.core.BMP_8BPP
wxBMP_8BPP_GREY = wx.core.BMP_8BPP_GREY
wxBMP_8BPP_GRAY = wx.core.BMP_8BPP_GRAY
wxBMP_8BPP_RED = wx.core.BMP_8BPP_RED
wxBMP_8BPP_PALETTE = wx.core.BMP_8BPP_PALETTE
wxBMP_4BPP = wx.core.BMP_4BPP
wxBMP_1BPP = wx.core.BMP_1BPP
wxBMP_1BPP_BW = wx.core.BMP_1BPP_BW
wxBMPHandler = wx.core.BMPHandler
wxICOHandler = wx.core.ICOHandler
wxCURHandler = wx.core.CURHandler
wxANIHandler = wx.core.ANIHandler
wxPNGHandler = wx.core.PNGHandler
wxGIFHandler = wx.core.GIFHandler
wxPCXHandler = wx.core.PCXHandler
wxJPEGHandler = wx.core.JPEGHandler
wxPNMHandler = wx.core.PNMHandler
wxXPMHandler = wx.core.XPMHandler
wxTIFFHandler = wx.core.TIFFHandler
wxEvtHandler = wx.core.EvtHandler
wxEVENT_PROPAGATE_NONE = wx.core.EVENT_PROPAGATE_NONE
wxEVENT_PROPAGATE_MAX = wx.core.EVENT_PROPAGATE_MAX
wxNewEventType = wx.core.NewEventType
wxEVT_NULL = wx.core.wxEVT_NULL
wxEVT_FIRST = wx.core.wxEVT_FIRST
wxEVT_USER_FIRST = wx.core.wxEVT_USER_FIRST
wxEVT_COMMAND_BUTTON_CLICKED = wx.core.wxEVT_COMMAND_BUTTON_CLICKED
wxEVT_COMMAND_CHECKBOX_CLICKED = wx.core.wxEVT_COMMAND_CHECKBOX_CLICKED
wxEVT_COMMAND_CHOICE_SELECTED = wx.core.wxEVT_COMMAND_CHOICE_SELECTED
wxEVT_COMMAND_LISTBOX_SELECTED = wx.core.wxEVT_COMMAND_LISTBOX_SELECTED
wxEVT_COMMAND_LISTBOX_DOUBLECLICKED = wx.core.wxEVT_COMMAND_LISTBOX_DOUBLECLICKED
wxEVT_COMMAND_CHECKLISTBOX_TOGGLED = wx.core.wxEVT_COMMAND_CHECKLISTBOX_TOGGLED
wxEVT_COMMAND_MENU_SELECTED = wx.core.wxEVT_COMMAND_MENU_SELECTED
wxEVT_COMMAND_TOOL_CLICKED = wx.core.wxEVT_COMMAND_TOOL_CLICKED
wxEVT_COMMAND_SLIDER_UPDATED = wx.core.wxEVT_COMMAND_SLIDER_UPDATED
wxEVT_COMMAND_RADIOBOX_SELECTED = wx.core.wxEVT_COMMAND_RADIOBOX_SELECTED
wxEVT_COMMAND_RADIOBUTTON_SELECTED = wx.core.wxEVT_COMMAND_RADIOBUTTON_SELECTED
wxEVT_COMMAND_SCROLLBAR_UPDATED = wx.core.wxEVT_COMMAND_SCROLLBAR_UPDATED
wxEVT_COMMAND_VLBOX_SELECTED = wx.core.wxEVT_COMMAND_VLBOX_SELECTED
wxEVT_COMMAND_COMBOBOX_SELECTED = wx.core.wxEVT_COMMAND_COMBOBOX_SELECTED
wxEVT_COMMAND_TOOL_RCLICKED = wx.core.wxEVT_COMMAND_TOOL_RCLICKED
wxEVT_COMMAND_TOOL_ENTER = wx.core.wxEVT_COMMAND_TOOL_ENTER
wxEVT_SOCKET = wx.core.wxEVT_SOCKET
wxEVT_TIMER = wx.core.wxEVT_TIMER
wxEVT_LEFT_DOWN = wx.core.wxEVT_LEFT_DOWN
wxEVT_LEFT_UP = wx.core.wxEVT_LEFT_UP
wxEVT_MIDDLE_DOWN = wx.core.wxEVT_MIDDLE_DOWN
wxEVT_MIDDLE_UP = wx.core.wxEVT_MIDDLE_UP
wxEVT_RIGHT_DOWN = wx.core.wxEVT_RIGHT_DOWN
wxEVT_RIGHT_UP = wx.core.wxEVT_RIGHT_UP
wxEVT_MOTION = wx.core.wxEVT_MOTION
wxEVT_ENTER_WINDOW = wx.core.wxEVT_ENTER_WINDOW
wxEVT_LEAVE_WINDOW = wx.core.wxEVT_LEAVE_WINDOW
wxEVT_LEFT_DCLICK = wx.core.wxEVT_LEFT_DCLICK
wxEVT_MIDDLE_DCLICK = wx.core.wxEVT_MIDDLE_DCLICK
wxEVT_RIGHT_DCLICK = wx.core.wxEVT_RIGHT_DCLICK
wxEVT_SET_FOCUS = wx.core.wxEVT_SET_FOCUS
wxEVT_KILL_FOCUS = wx.core.wxEVT_KILL_FOCUS
wxEVT_CHILD_FOCUS = wx.core.wxEVT_CHILD_FOCUS
wxEVT_MOUSEWHEEL = wx.core.wxEVT_MOUSEWHEEL
wxEVT_NC_LEFT_DOWN = wx.core.wxEVT_NC_LEFT_DOWN
wxEVT_NC_LEFT_UP = wx.core.wxEVT_NC_LEFT_UP
wxEVT_NC_MIDDLE_DOWN = wx.core.wxEVT_NC_MIDDLE_DOWN
wxEVT_NC_MIDDLE_UP = wx.core.wxEVT_NC_MIDDLE_UP
wxEVT_NC_RIGHT_DOWN = wx.core.wxEVT_NC_RIGHT_DOWN
wxEVT_NC_RIGHT_UP = wx.core.wxEVT_NC_RIGHT_UP
wxEVT_NC_MOTION = wx.core.wxEVT_NC_MOTION
wxEVT_NC_ENTER_WINDOW = wx.core.wxEVT_NC_ENTER_WINDOW
wxEVT_NC_LEAVE_WINDOW = wx.core.wxEVT_NC_LEAVE_WINDOW
wxEVT_NC_LEFT_DCLICK = wx.core.wxEVT_NC_LEFT_DCLICK
wxEVT_NC_MIDDLE_DCLICK = wx.core.wxEVT_NC_MIDDLE_DCLICK
wxEVT_NC_RIGHT_DCLICK = wx.core.wxEVT_NC_RIGHT_DCLICK
wxEVT_CHAR = wx.core.wxEVT_CHAR
wxEVT_CHAR_HOOK = wx.core.wxEVT_CHAR_HOOK
wxEVT_NAVIGATION_KEY = wx.core.wxEVT_NAVIGATION_KEY
wxEVT_KEY_DOWN = wx.core.wxEVT_KEY_DOWN
wxEVT_KEY_UP = wx.core.wxEVT_KEY_UP
wxEVT_HOTKEY = wx.core.wxEVT_HOTKEY
wxEVT_SET_CURSOR = wx.core.wxEVT_SET_CURSOR
wxEVT_SCROLL_TOP = wx.core.wxEVT_SCROLL_TOP
wxEVT_SCROLL_BOTTOM = wx.core.wxEVT_SCROLL_BOTTOM
wxEVT_SCROLL_LINEUP = wx.core.wxEVT_SCROLL_LINEUP
wxEVT_SCROLL_LINEDOWN = wx.core.wxEVT_SCROLL_LINEDOWN
wxEVT_SCROLL_PAGEUP = wx.core.wxEVT_SCROLL_PAGEUP
wxEVT_SCROLL_PAGEDOWN = wx.core.wxEVT_SCROLL_PAGEDOWN
wxEVT_SCROLL_THUMBTRACK = wx.core.wxEVT_SCROLL_THUMBTRACK
wxEVT_SCROLL_THUMBRELEASE = wx.core.wxEVT_SCROLL_THUMBRELEASE
wxEVT_SCROLL_ENDSCROLL = wx.core.wxEVT_SCROLL_ENDSCROLL
wxEVT_SCROLLWIN_TOP = wx.core.wxEVT_SCROLLWIN_TOP
wxEVT_SCROLLWIN_BOTTOM = wx.core.wxEVT_SCROLLWIN_BOTTOM
wxEVT_SCROLLWIN_LINEUP = wx.core.wxEVT_SCROLLWIN_LINEUP
wxEVT_SCROLLWIN_LINEDOWN = wx.core.wxEVT_SCROLLWIN_LINEDOWN
wxEVT_SCROLLWIN_PAGEUP = wx.core.wxEVT_SCROLLWIN_PAGEUP
wxEVT_SCROLLWIN_PAGEDOWN = wx.core.wxEVT_SCROLLWIN_PAGEDOWN
wxEVT_SCROLLWIN_THUMBTRACK = wx.core.wxEVT_SCROLLWIN_THUMBTRACK
wxEVT_SCROLLWIN_THUMBRELEASE = wx.core.wxEVT_SCROLLWIN_THUMBRELEASE
wxEVT_SIZE = wx.core.wxEVT_SIZE
wxEVT_MOVE = wx.core.wxEVT_MOVE
wxEVT_CLOSE_WINDOW = wx.core.wxEVT_CLOSE_WINDOW
wxEVT_END_SESSION = wx.core.wxEVT_END_SESSION
wxEVT_QUERY_END_SESSION = wx.core.wxEVT_QUERY_END_SESSION
wxEVT_ACTIVATE_APP = wx.core.wxEVT_ACTIVATE_APP
wxEVT_POWER = wx.core.wxEVT_POWER
wxEVT_ACTIVATE = wx.core.wxEVT_ACTIVATE
wxEVT_CREATE = wx.core.wxEVT_CREATE
wxEVT_DESTROY = wx.core.wxEVT_DESTROY
wxEVT_SHOW = wx.core.wxEVT_SHOW
wxEVT_ICONIZE = wx.core.wxEVT_ICONIZE
wxEVT_MAXIMIZE = wx.core.wxEVT_MAXIMIZE
wxEVT_MOUSE_CAPTURE_CHANGED = wx.core.wxEVT_MOUSE_CAPTURE_CHANGED
wxEVT_PAINT = wx.core.wxEVT_PAINT
wxEVT_ERASE_BACKGROUND = wx.core.wxEVT_ERASE_BACKGROUND
wxEVT_NC_PAINT = wx.core.wxEVT_NC_PAINT
wxEVT_PAINT_ICON = wx.core.wxEVT_PAINT_ICON
wxEVT_MENU_OPEN = wx.core.wxEVT_MENU_OPEN
wxEVT_MENU_CLOSE = wx.core.wxEVT_MENU_CLOSE
wxEVT_MENU_HIGHLIGHT = wx.core.wxEVT_MENU_HIGHLIGHT
wxEVT_CONTEXT_MENU = wx.core.wxEVT_CONTEXT_MENU
wxEVT_SYS_COLOUR_CHANGED = wx.core.wxEVT_SYS_COLOUR_CHANGED
wxEVT_DISPLAY_CHANGED = wx.core.wxEVT_DISPLAY_CHANGED
wxEVT_SETTING_CHANGED = wx.core.wxEVT_SETTING_CHANGED
wxEVT_QUERY_NEW_PALETTE = wx.core.wxEVT_QUERY_NEW_PALETTE
wxEVT_PALETTE_CHANGED = wx.core.wxEVT_PALETTE_CHANGED
wxEVT_DROP_FILES = wx.core.wxEVT_DROP_FILES
wxEVT_DRAW_ITEM = wx.core.wxEVT_DRAW_ITEM
wxEVT_MEASURE_ITEM = wx.core.wxEVT_MEASURE_ITEM
wxEVT_COMPARE_ITEM = wx.core.wxEVT_COMPARE_ITEM
wxEVT_INIT_DIALOG = wx.core.wxEVT_INIT_DIALOG
wxEVT_IDLE = wx.core.wxEVT_IDLE
wxEVT_UPDATE_UI = wx.core.wxEVT_UPDATE_UI
wxEVT_SIZING = wx.core.wxEVT_SIZING
wxEVT_MOVING = wx.core.wxEVT_MOVING
wxEVT_COMMAND_LEFT_CLICK = wx.core.wxEVT_COMMAND_LEFT_CLICK
wxEVT_COMMAND_LEFT_DCLICK = wx.core.wxEVT_COMMAND_LEFT_DCLICK
wxEVT_COMMAND_RIGHT_CLICK = wx.core.wxEVT_COMMAND_RIGHT_CLICK
wxEVT_COMMAND_RIGHT_DCLICK = wx.core.wxEVT_COMMAND_RIGHT_DCLICK
wxEVT_COMMAND_SET_FOCUS = wx.core.wxEVT_COMMAND_SET_FOCUS
wxEVT_COMMAND_KILL_FOCUS = wx.core.wxEVT_COMMAND_KILL_FOCUS
wxEVT_COMMAND_ENTER = wx.core.wxEVT_COMMAND_ENTER
wxEvent = wx.core.Event
wxPropagationDisabler = wx.core.PropagationDisabler
wxPropagateOnce = wx.core.PropagateOnce
wxCommandEvent = wx.core.CommandEvent
wxNotifyEvent = wx.core.NotifyEvent
wxScrollEvent = wx.core.ScrollEvent
wxScrollWinEvent = wx.core.ScrollWinEvent
wxMOUSE_BTN_ANY = wx.core.MOUSE_BTN_ANY
wxMOUSE_BTN_NONE = wx.core.MOUSE_BTN_NONE
wxMOUSE_BTN_LEFT = wx.core.MOUSE_BTN_LEFT
wxMOUSE_BTN_MIDDLE = wx.core.MOUSE_BTN_MIDDLE
wxMOUSE_BTN_RIGHT = wx.core.MOUSE_BTN_RIGHT
wxMouseEvent = wx.core.MouseEvent
wxSetCursorEvent = wx.core.SetCursorEvent
wxKeyEvent = wx.core.KeyEvent
wxSizeEvent = wx.core.SizeEvent
wxMoveEvent = wx.core.MoveEvent
wxPaintEvent = wx.core.PaintEvent
wxNcPaintEvent = wx.core.NcPaintEvent
wxEraseEvent = wx.core.EraseEvent
wxFocusEvent = wx.core.FocusEvent
wxChildFocusEvent = wx.core.ChildFocusEvent
wxActivateEvent = wx.core.ActivateEvent
wxInitDialogEvent = wx.core.InitDialogEvent
wxMenuEvent = wx.core.MenuEvent
wxCloseEvent = wx.core.CloseEvent
wxShowEvent = wx.core.ShowEvent
wxIconizeEvent = wx.core.IconizeEvent
wxMaximizeEvent = wx.core.MaximizeEvent
wxDropFilesEvent = wx.core.DropFilesEvent
wxUPDATE_UI_PROCESS_ALL = wx.core.UPDATE_UI_PROCESS_ALL
wxUPDATE_UI_PROCESS_SPECIFIED = wx.core.UPDATE_UI_PROCESS_SPECIFIED
wxUpdateUIEvent = wx.core.UpdateUIEvent
wxSysColourChangedEvent = wx.core.SysColourChangedEvent
wxMouseCaptureChangedEvent = wx.core.MouseCaptureChangedEvent
wxDisplayChangedEvent = wx.core.DisplayChangedEvent
wxPaletteChangedEvent = wx.core.PaletteChangedEvent
wxQueryNewPaletteEvent = wx.core.QueryNewPaletteEvent
wxNavigationKeyEvent = wx.core.NavigationKeyEvent
wxWindowCreateEvent = wx.core.WindowCreateEvent
wxWindowDestroyEvent = wx.core.WindowDestroyEvent
wxContextMenuEvent = wx.core.ContextMenuEvent
wxIDLE_PROCESS_ALL = wx.core.IDLE_PROCESS_ALL
wxIDLE_PROCESS_SPECIFIED = wx.core.IDLE_PROCESS_SPECIFIED
wxIdleEvent = wx.core.IdleEvent
wxPyEvent = wx.core.PyEvent
wxPyCommandEvent = wx.core.PyCommandEvent
wxPYAPP_ASSERT_SUPPRESS = wx.core.PYAPP_ASSERT_SUPPRESS
wxPYAPP_ASSERT_EXCEPTION = wx.core.PYAPP_ASSERT_EXCEPTION
wxPYAPP_ASSERT_DIALOG = wx.core.PYAPP_ASSERT_DIALOG
wxPYAPP_ASSERT_LOG = wx.core.PYAPP_ASSERT_LOG
wxPRINT_WINDOWS = wx.core.PRINT_WINDOWS
wxPRINT_POSTSCRIPT = wx.core.PRINT_POSTSCRIPT
wxPyApp = wx.core.PyApp
wxExit = wx.core.Exit
wxYield = wx.core.Yield
wxYieldIfNeeded = wx.core.YieldIfNeeded
wxSafeYield = wx.core.SafeYield
wxWakeUpIdle = wx.core.WakeUpIdle
wxPostEvent = wx.core.PostEvent
wxApp_CleanUp = wx.core.App_CleanUp
wxGetApp = wx.core.GetApp
wxPreWindow = wx.core.PreWindow
wxWindow = wx.core.Window
wxFindWindowById = wx.core.FindWindowById
wxFindWindowByName = wx.core.FindWindowByName
wxFindWindowByLabel = wx.core.FindWindowByLabel
wxValidator = wx.core.Validator
wxPyValidator = wx.core.PyValidator
wxDefaultValidator = wx.core.DefaultValidator
wxMenu = wx.core.Menu
wxMenuBar = wx.core.MenuBar
wxMenuItem = wx.core.MenuItem
wxPreControl = wx.core.PreControl
wxControl = wx.core.Control
wxItemContainer = wx.core.ItemContainer
wxControlWithItems = wx.core.ControlWithItems
wxSizerItem = wx.core.SizerItem
wxSizer = wx.core.Sizer
wxPySizer = wx.core.PySizer
wxBoxSizer = wx.core.BoxSizer
wxStaticBoxSizer = wx.core.StaticBoxSizer
wxGridSizer = wx.core.GridSizer
wxFLEX_GROWMODE_NONE = wx.core.FLEX_GROWMODE_NONE
wxFLEX_GROWMODE_SPECIFIED = wx.core.FLEX_GROWMODE_SPECIFIED
wxFLEX_GROWMODE_ALL = wx.core.FLEX_GROWMODE_ALL
wxFlexGridSizer = wx.core.FlexGridSizer
wxLeft = wx.core.Left
wxTop = wx.core.Top
wxRight = wx.core.Right
wxBottom = wx.core.Bottom
wxWidth = wx.core.Width
wxHeight = wx.core.Height
wxCentre = wx.core.Centre
wxCenter = wx.core.Center
wxCentreX = wx.core.CentreX
wxCentreY = wx.core.CentreY
wxUnconstrained = wx.core.Unconstrained
wxAsIs = wx.core.AsIs
wxPercentOf = wx.core.PercentOf
wxAbove = wx.core.Above
wxBelow = wx.core.Below
wxLeftOf = wx.core.LeftOf
wxRightOf = wx.core.RightOf
wxSameAs = wx.core.SameAs
wxAbsolute = wx.core.Absolute
wxIndividualLayoutConstraint = wx.core.IndividualLayoutConstraint
wxLayoutConstraints = wx.core.LayoutConstraints
wxPyOnDemandOutputWindow = wx.core.PyOnDemandOutputWindow
wxApp = wx.core.App
wxPySimpleApp = wx.core.PySimpleApp
wxPyWidgetTester = wx.core.PyWidgetTester
wxApp_GetMacDefaultEncodingIsPC = wx.core.App_GetMacDefaultEncodingIsPC
wxApp_GetMacSupportPCMenuShortcuts = wx.core.App_GetMacSupportPCMenuShortcuts
wxApp_GetMacAboutMenuItemId = wx.core.App_GetMacAboutMenuItemId
wxApp_GetMacPreferencesMenuItemId = wx.core.App_GetMacPreferencesMenuItemId
wxApp_GetMacExitMenuItemId = wx.core.App_GetMacExitMenuItemId
wxApp_GetMacHelpMenuTitleName = wx.core.App_GetMacHelpMenuTitleName
wxApp_SetMacDefaultEncodingIsPC = wx.core.App_SetMacDefaultEncodingIsPC
wxApp_SetMacSupportPCMenuShortcuts = wx.core.App_SetMacSupportPCMenuShortcuts
wxApp_SetMacAboutMenuItemId = wx.core.App_SetMacAboutMenuItemId
wxApp_SetMacPreferencesMenuItemId = wx.core.App_SetMacPreferencesMenuItemId
wxApp_SetMacExitMenuItemId = wx.core.App_SetMacExitMenuItemId
wxApp_SetMacHelpMenuTitleName = wx.core.App_SetMacHelpMenuTitleName
wxPlatform = wx.core.Platform
wxUSE_UNICODE = wx.core.USE_UNICODE
wxVERSION_STRING = wx.core.VERSION_STRING
wxMAJOR_VERSION = wx.core.MAJOR_VERSION
wxMINOR_VERSION = wx.core.MINOR_VERSION
wxRELEASE_VERSION = wx.core.RELEASE_VERSION
wxSUBREL_VERSION = wx.core.SUBREL_VERSION
wxVERSION = wx.core.VERSION
wxRELEASE_NUMBER = wx.core.RELEASE_NUMBER
wxPyDeadObjectError = wx.core.PyDeadObjectError
wxCallAfter = wx.core.CallAfter
wxFutureCall = wx.core.FutureCall
wxNotebookPage = wx.core.NotebookPage
wxPyEventBinder = wx.core.PyEventBinder
wxDLG_PNT = wx.core.DLG_PNT
wxDLG_SZE = wx.core.DLG_SZE
wxPyAssertionError = wx.core.PyAssertionError
wxMemoryFSHandler_AddFile = wx.core.MemoryFSHandler_AddFile


d = globals()
for k, v in wx.core.__dict__.iteritems():
    if k.startswith('EVT'):
        d[k] = v
    elif k.startswith('WXK'):
        d[k] = v
del d, k, v



