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

import wx.windows

sys.modules['wxPython.wx'] = _wx
del sys, _wx


# Now assign all the reverse-renamed names:
wxPrePanel = wx.windows.PrePanel
wxPanel = wx.windows.Panel
wxPreScrolledWindow = wx.windows.PreScrolledWindow
wxScrolledWindow = wx.windows.ScrolledWindow
wxAcceleratorEntry = wx.windows.AcceleratorEntry
wxAcceleratorTable = wx.windows.AcceleratorTable
wxNullAcceleratorTable = wx.windows.NullAcceleratorTable
wxGetAccelFromString = wx.windows.GetAccelFromString
wxFULLSCREEN_NOMENUBAR = wx.windows.FULLSCREEN_NOMENUBAR
wxFULLSCREEN_NOTOOLBAR = wx.windows.FULLSCREEN_NOTOOLBAR
wxFULLSCREEN_NOSTATUSBAR = wx.windows.FULLSCREEN_NOSTATUSBAR
wxFULLSCREEN_NOBORDER = wx.windows.FULLSCREEN_NOBORDER
wxFULLSCREEN_NOCAPTION = wx.windows.FULLSCREEN_NOCAPTION
wxFULLSCREEN_ALL = wx.windows.FULLSCREEN_ALL
wxTOPLEVEL_EX_DIALOG = wx.windows.TOPLEVEL_EX_DIALOG
wxTopLevelWindow = wx.windows.TopLevelWindow
wxPreFrame = wx.windows.PreFrame
wxFrame = wx.windows.Frame
wxPreDialog = wx.windows.PreDialog
wxDialog = wx.windows.Dialog
wxPreMiniFrame = wx.windows.PreMiniFrame
wxMiniFrame = wx.windows.MiniFrame
wxSPLASH_CENTRE_ON_PARENT = wx.windows.SPLASH_CENTRE_ON_PARENT
wxSPLASH_CENTRE_ON_SCREEN = wx.windows.SPLASH_CENTRE_ON_SCREEN
wxSPLASH_NO_CENTRE = wx.windows.SPLASH_NO_CENTRE
wxSPLASH_TIMEOUT = wx.windows.SPLASH_TIMEOUT
wxSPLASH_NO_TIMEOUT = wx.windows.SPLASH_NO_TIMEOUT
wxSplashScreenWindow = wx.windows.SplashScreenWindow
wxSplashScreen = wx.windows.SplashScreen
wxPreStatusBar = wx.windows.PreStatusBar
wxStatusBar = wx.windows.StatusBar
wxSP_NOBORDER = wx.windows.SP_NOBORDER
wxSP_NOSASH = wx.windows.SP_NOSASH
wxSP_PERMIT_UNSPLIT = wx.windows.SP_PERMIT_UNSPLIT
wxSP_LIVE_UPDATE = wx.windows.SP_LIVE_UPDATE
wxSP_3DSASH = wx.windows.SP_3DSASH
wxSP_3DBORDER = wx.windows.SP_3DBORDER
wxSP_BORDER = wx.windows.SP_BORDER
wxSP_3D = wx.windows.SP_3D
wxEVT_COMMAND_SPLITTER_SASH_POS_CHANGED = wx.windows.wxEVT_COMMAND_SPLITTER_SASH_POS_CHANGED
wxEVT_COMMAND_SPLITTER_SASH_POS_CHANGING = wx.windows.wxEVT_COMMAND_SPLITTER_SASH_POS_CHANGING
wxEVT_COMMAND_SPLITTER_DOUBLECLICKED = wx.windows.wxEVT_COMMAND_SPLITTER_DOUBLECLICKED
wxEVT_COMMAND_SPLITTER_UNSPLIT = wx.windows.wxEVT_COMMAND_SPLITTER_UNSPLIT
wxSPLIT_HORIZONTAL = wx.windows.SPLIT_HORIZONTAL
wxSPLIT_VERTICAL = wx.windows.SPLIT_VERTICAL
wxSPLIT_DRAG_NONE = wx.windows.SPLIT_DRAG_NONE
wxSPLIT_DRAG_DRAGGING = wx.windows.SPLIT_DRAG_DRAGGING
wxSPLIT_DRAG_LEFT_DOWN = wx.windows.SPLIT_DRAG_LEFT_DOWN
wxPreSplitterWindow = wx.windows.PreSplitterWindow
wxSplitterWindow = wx.windows.SplitterWindow
wxSplitterEvent = wx.windows.SplitterEvent
wxSASH_DRAG_NONE = wx.windows.SASH_DRAG_NONE
wxSASH_DRAG_DRAGGING = wx.windows.SASH_DRAG_DRAGGING
wxSASH_DRAG_LEFT_DOWN = wx.windows.SASH_DRAG_LEFT_DOWN
wxSW_NOBORDER = wx.windows.SW_NOBORDER
wxSW_BORDER = wx.windows.SW_BORDER
wxSW_3DSASH = wx.windows.SW_3DSASH
wxSW_3DBORDER = wx.windows.SW_3DBORDER
wxSW_3D = wx.windows.SW_3D
wxSASH_TOP = wx.windows.SASH_TOP
wxSASH_RIGHT = wx.windows.SASH_RIGHT
wxSASH_BOTTOM = wx.windows.SASH_BOTTOM
wxSASH_LEFT = wx.windows.SASH_LEFT
wxSASH_NONE = wx.windows.SASH_NONE
wxPreSashWindow = wx.windows.PreSashWindow
wxSashWindow = wx.windows.SashWindow
wxSASH_STATUS_OK = wx.windows.SASH_STATUS_OK
wxSASH_STATUS_OUT_OF_RANGE = wx.windows.SASH_STATUS_OUT_OF_RANGE
wxSashEvent = wx.windows.SashEvent
wxEVT_SASH_DRAGGED = wx.windows.wxEVT_SASH_DRAGGED
wxLAYOUT_HORIZONTAL = wx.windows.LAYOUT_HORIZONTAL
wxLAYOUT_VERTICAL = wx.windows.LAYOUT_VERTICAL
wxLAYOUT_NONE = wx.windows.LAYOUT_NONE
wxLAYOUT_TOP = wx.windows.LAYOUT_TOP
wxLAYOUT_LEFT = wx.windows.LAYOUT_LEFT
wxLAYOUT_RIGHT = wx.windows.LAYOUT_RIGHT
wxLAYOUT_BOTTOM = wx.windows.LAYOUT_BOTTOM
wxLAYOUT_LENGTH_Y = wx.windows.LAYOUT_LENGTH_Y
wxLAYOUT_LENGTH_X = wx.windows.LAYOUT_LENGTH_X
wxLAYOUT_MRU_LENGTH = wx.windows.LAYOUT_MRU_LENGTH
wxLAYOUT_QUERY = wx.windows.LAYOUT_QUERY
wxEVT_QUERY_LAYOUT_INFO = wx.windows.wxEVT_QUERY_LAYOUT_INFO
wxEVT_CALCULATE_LAYOUT = wx.windows.wxEVT_CALCULATE_LAYOUT
wxQueryLayoutInfoEvent = wx.windows.QueryLayoutInfoEvent
wxCalculateLayoutEvent = wx.windows.CalculateLayoutEvent
wxPreSashLayoutWindow = wx.windows.PreSashLayoutWindow
wxSashLayoutWindow = wx.windows.SashLayoutWindow
wxLayoutAlgorithm = wx.windows.LayoutAlgorithm
wxPrePopupWindow = wx.windows.PrePopupWindow
wxPopupWindow = wx.windows.PopupWindow
wxPrePopupTransientWindow = wx.windows.PrePopupTransientWindow
wxPopupTransientWindow = wx.windows.PopupTransientWindow
wxTipWindow = wx.windows.TipWindow
wxPreVScrolledWindow = wx.windows.PreVScrolledWindow
wxVScrolledWindow = wx.windows.VScrolledWindow
wxPreVListBox = wx.windows.PreVListBox
wxVListBox = wx.windows.VListBox
wxPreHtmlListBox = wx.windows.PreHtmlListBox
wxHtmlListBox = wx.windows.HtmlListBox
wxTaskBarIcon = wx.windows.TaskBarIcon
wxTaskBarIconEvent = wx.windows.TaskBarIconEvent
wxEVT_TASKBAR_MOVE = wx.windows.wxEVT_TASKBAR_MOVE
wxEVT_TASKBAR_LEFT_DOWN = wx.windows.wxEVT_TASKBAR_LEFT_DOWN
wxEVT_TASKBAR_LEFT_UP = wx.windows.wxEVT_TASKBAR_LEFT_UP
wxEVT_TASKBAR_RIGHT_DOWN = wx.windows.wxEVT_TASKBAR_RIGHT_DOWN
wxEVT_TASKBAR_RIGHT_UP = wx.windows.wxEVT_TASKBAR_RIGHT_UP
wxEVT_TASKBAR_LEFT_DCLICK = wx.windows.wxEVT_TASKBAR_LEFT_DCLICK
wxEVT_TASKBAR_RIGHT_DCLICK = wx.windows.wxEVT_TASKBAR_RIGHT_DCLICK
wxColourData = wx.windows.ColourData
wxColourDialog = wx.windows.ColourDialog
wxDirDialog = wx.windows.DirDialog
wxFileDialog = wx.windows.FileDialog
wxCHOICEDLG_STYLE = wx.windows.CHOICEDLG_STYLE
wxMultiChoiceDialog = wx.windows.MultiChoiceDialog
wxSingleChoiceDialog = wx.windows.SingleChoiceDialog
wxTextEntryDialog = wx.windows.TextEntryDialog
wxFontData = wx.windows.FontData
wxFontDialog = wx.windows.FontDialog
wxMessageDialog = wx.windows.MessageDialog
wxProgressDialog = wx.windows.ProgressDialog
wxFR_DOWN = wx.windows.FR_DOWN
wxFR_WHOLEWORD = wx.windows.FR_WHOLEWORD
wxFR_MATCHCASE = wx.windows.FR_MATCHCASE
wxFR_REPLACEDIALOG = wx.windows.FR_REPLACEDIALOG
wxFR_NOUPDOWN = wx.windows.FR_NOUPDOWN
wxFR_NOMATCHCASE = wx.windows.FR_NOMATCHCASE
wxFR_NOWHOLEWORD = wx.windows.FR_NOWHOLEWORD
wxEVT_COMMAND_FIND = wx.windows.wxEVT_COMMAND_FIND
wxEVT_COMMAND_FIND_NEXT = wx.windows.wxEVT_COMMAND_FIND_NEXT
wxEVT_COMMAND_FIND_REPLACE = wx.windows.wxEVT_COMMAND_FIND_REPLACE
wxEVT_COMMAND_FIND_REPLACE_ALL = wx.windows.wxEVT_COMMAND_FIND_REPLACE_ALL
wxEVT_COMMAND_FIND_CLOSE = wx.windows.wxEVT_COMMAND_FIND_CLOSE
wxFindDialogEvent = wx.windows.FindDialogEvent
wxFindReplaceData = wx.windows.FindReplaceData
wxPreFindReplaceDialog = wx.windows.PreFindReplaceDialog
wxFindReplaceDialog = wx.windows.FindReplaceDialog
IDM_WINDOWTILE = wx.windows.IDM_WINDOWTILE
IDM_WINDOWTILEHOR = wx.windows.IDM_WINDOWTILEHOR
IDM_WINDOWCASCADE = wx.windows.IDM_WINDOWCASCADE
IDM_WINDOWICONS = wx.windows.IDM_WINDOWICONS
IDM_WINDOWNEXT = wx.windows.IDM_WINDOWNEXT
IDM_WINDOWTILEVERT = wx.windows.IDM_WINDOWTILEVERT
wxFIRST_MDI_CHILD = wx.windows.FIRST_MDI_CHILD
wxLAST_MDI_CHILD = wx.windows.LAST_MDI_CHILD
wxPreMDIParentFrame = wx.windows.PreMDIParentFrame
wxMDIParentFrame = wx.windows.MDIParentFrame
wxPreMDIChildFrame = wx.windows.PreMDIChildFrame
wxMDIChildFrame = wx.windows.MDIChildFrame
wxPreMDIClientWindow = wx.windows.PreMDIClientWindow
wxMDIClientWindow = wx.windows.MDIClientWindow
wxPyWindow = wx.windows.PyWindow
wxPyPanel = wx.windows.PyPanel
wxPRINT_MODE_NONE = wx.windows.PRINT_MODE_NONE
wxPRINT_MODE_PREVIEW = wx.windows.PRINT_MODE_PREVIEW
wxPRINT_MODE_FILE = wx.windows.PRINT_MODE_FILE
wxPRINT_MODE_PRINTER = wx.windows.PRINT_MODE_PRINTER
wxPrintData = wx.windows.PrintData
wxPageSetupDialogData = wx.windows.PageSetupDialogData
wxPageSetupDialog = wx.windows.PageSetupDialog
wxPrintDialogData = wx.windows.PrintDialogData
wxPrintDialog = wx.windows.PrintDialog
wxPRINTER_NO_ERROR = wx.windows.PRINTER_NO_ERROR
wxPRINTER_CANCELLED = wx.windows.PRINTER_CANCELLED
wxPRINTER_ERROR = wx.windows.PRINTER_ERROR
wxPrinter_GetLastError = wx.windows.Printer_GetLastError
wxPrinter = wx.windows.Printer
wxPrintout = wx.windows.Printout
wxPreviewCanvas = wx.windows.PreviewCanvas
wxPreviewFrame = wx.windows.PreviewFrame
wxPREVIEW_PRINT = wx.windows.PREVIEW_PRINT
wxPREVIEW_PREVIOUS = wx.windows.PREVIEW_PREVIOUS
wxPREVIEW_NEXT = wx.windows.PREVIEW_NEXT
wxPREVIEW_ZOOM = wx.windows.PREVIEW_ZOOM
wxPREVIEW_FIRST = wx.windows.PREVIEW_FIRST
wxPREVIEW_LAST = wx.windows.PREVIEW_LAST
wxPREVIEW_GOTO = wx.windows.PREVIEW_GOTO
wxPREVIEW_DEFAULT = wx.windows.PREVIEW_DEFAULT
wxID_PREVIEW_CLOSE = wx.windows.ID_PREVIEW_CLOSE
wxID_PREVIEW_NEXT = wx.windows.ID_PREVIEW_NEXT
wxID_PREVIEW_PREVIOUS = wx.windows.ID_PREVIEW_PREVIOUS
wxID_PREVIEW_PRINT = wx.windows.ID_PREVIEW_PRINT
wxID_PREVIEW_ZOOM = wx.windows.ID_PREVIEW_ZOOM
wxID_PREVIEW_FIRST = wx.windows.ID_PREVIEW_FIRST
wxID_PREVIEW_LAST = wx.windows.ID_PREVIEW_LAST
wxID_PREVIEW_GOTO = wx.windows.ID_PREVIEW_GOTO
wxPreviewControlBar = wx.windows.PreviewControlBar
wxPrintPreview = wx.windows.PrintPreview
wxPyPrintPreview = wx.windows.PyPrintPreview
wxPyPreviewFrame = wx.windows.PyPreviewFrame
wxPyPreviewControlBar = wx.windows.PyPreviewControlBar


d = globals()
for k, v in wx.windows.__dict__.iteritems():
    if k.startswith('EVT'):
        d[k] = v
    elif k.startswith('IDM'):
        d[k] = v
del d, k, v



