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
wxPanel = wx.windows.Panel
wxPanelPtr = wx.windows.PanelPtr
wxPrePanel = wx.windows.PrePanel
wxScrolledWindow = wx.windows.ScrolledWindow
wxScrolledWindowPtr = wx.windows.ScrolledWindowPtr
wxPreScrolledWindow = wx.windows.PreScrolledWindow
wxFrameNameStr = wx.windows.FrameNameStr
wxDialogNameStr = wx.windows.DialogNameStr
wxStatusLineNameStr = wx.windows.StatusLineNameStr
wxToolBarNameStr = wx.windows.ToolBarNameStr
wxFULLSCREEN_NOMENUBAR = wx.windows.FULLSCREEN_NOMENUBAR
wxFULLSCREEN_NOTOOLBAR = wx.windows.FULLSCREEN_NOTOOLBAR
wxFULLSCREEN_NOSTATUSBAR = wx.windows.FULLSCREEN_NOSTATUSBAR
wxFULLSCREEN_NOBORDER = wx.windows.FULLSCREEN_NOBORDER
wxFULLSCREEN_NOCAPTION = wx.windows.FULLSCREEN_NOCAPTION
wxFULLSCREEN_ALL = wx.windows.FULLSCREEN_ALL
wxTOPLEVEL_EX_DIALOG = wx.windows.TOPLEVEL_EX_DIALOG
wxTopLevelWindow = wx.windows.TopLevelWindow
wxTopLevelWindowPtr = wx.windows.TopLevelWindowPtr
wxFrame = wx.windows.Frame
wxFramePtr = wx.windows.FramePtr
wxPreFrame = wx.windows.PreFrame
wxDialog = wx.windows.Dialog
wxDialogPtr = wx.windows.DialogPtr
wxPreDialog = wx.windows.PreDialog
wxMiniFrame = wx.windows.MiniFrame
wxMiniFramePtr = wx.windows.MiniFramePtr
wxPreMiniFrame = wx.windows.PreMiniFrame
wxSPLASH_CENTRE_ON_PARENT = wx.windows.SPLASH_CENTRE_ON_PARENT
wxSPLASH_CENTRE_ON_SCREEN = wx.windows.SPLASH_CENTRE_ON_SCREEN
wxSPLASH_NO_CENTRE = wx.windows.SPLASH_NO_CENTRE
wxSPLASH_TIMEOUT = wx.windows.SPLASH_TIMEOUT
wxSPLASH_NO_TIMEOUT = wx.windows.SPLASH_NO_TIMEOUT
wxSplashScreenWindow = wx.windows.SplashScreenWindow
wxSplashScreenWindowPtr = wx.windows.SplashScreenWindowPtr
wxSplashScreen = wx.windows.SplashScreen
wxSplashScreenPtr = wx.windows.SplashScreenPtr
wxStatusBar = wx.windows.StatusBar
wxStatusBarPtr = wx.windows.StatusBarPtr
wxPreStatusBar = wx.windows.PreStatusBar
wxSplitterNameStr = wx.windows.SplitterNameStr
wxSP_NOBORDER = wx.windows.SP_NOBORDER
wxSP_NOSASH = wx.windows.SP_NOSASH
wxSP_PERMIT_UNSPLIT = wx.windows.SP_PERMIT_UNSPLIT
wxSP_LIVE_UPDATE = wx.windows.SP_LIVE_UPDATE
wxSP_3DSASH = wx.windows.SP_3DSASH
wxSP_3DBORDER = wx.windows.SP_3DBORDER
wxSP_BORDER = wx.windows.SP_BORDER
wxSP_3D = wx.windows.SP_3D
wxSPLIT_HORIZONTAL = wx.windows.SPLIT_HORIZONTAL
wxSPLIT_VERTICAL = wx.windows.SPLIT_VERTICAL
wxSPLIT_DRAG_NONE = wx.windows.SPLIT_DRAG_NONE
wxSPLIT_DRAG_DRAGGING = wx.windows.SPLIT_DRAG_DRAGGING
wxSPLIT_DRAG_LEFT_DOWN = wx.windows.SPLIT_DRAG_LEFT_DOWN
wxSplitterWindow = wx.windows.SplitterWindow
wxSplitterWindowPtr = wx.windows.SplitterWindowPtr
wxPreSplitterWindow = wx.windows.PreSplitterWindow
wxSplitterEvent = wx.windows.SplitterEvent
wxSplitterEventPtr = wx.windows.SplitterEventPtr
wxEVT_COMMAND_SPLITTER_SASH_POS_CHANGED = wx.windows.wxEVT_COMMAND_SPLITTER_SASH_POS_CHANGED
wxEVT_COMMAND_SPLITTER_SASH_POS_CHANGING = wx.windows.wxEVT_COMMAND_SPLITTER_SASH_POS_CHANGING
wxEVT_COMMAND_SPLITTER_DOUBLECLICKED = wx.windows.wxEVT_COMMAND_SPLITTER_DOUBLECLICKED
wxEVT_COMMAND_SPLITTER_UNSPLIT = wx.windows.wxEVT_COMMAND_SPLITTER_UNSPLIT
wxSashNameStr = wx.windows.SashNameStr
wxSashLayoutNameStr = wx.windows.SashLayoutNameStr
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
wxSashWindow = wx.windows.SashWindow
wxSashWindowPtr = wx.windows.SashWindowPtr
wxPreSashWindow = wx.windows.PreSashWindow
wxSASH_STATUS_OK = wx.windows.SASH_STATUS_OK
wxSASH_STATUS_OUT_OF_RANGE = wx.windows.SASH_STATUS_OUT_OF_RANGE
wxSashEvent = wx.windows.SashEvent
wxSashEventPtr = wx.windows.SashEventPtr
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
wxQueryLayoutInfoEventPtr = wx.windows.QueryLayoutInfoEventPtr
wxCalculateLayoutEvent = wx.windows.CalculateLayoutEvent
wxCalculateLayoutEventPtr = wx.windows.CalculateLayoutEventPtr
wxSashLayoutWindow = wx.windows.SashLayoutWindow
wxSashLayoutWindowPtr = wx.windows.SashLayoutWindowPtr
wxPreSashLayoutWindow = wx.windows.PreSashLayoutWindow
wxLayoutAlgorithm = wx.windows.LayoutAlgorithm
wxLayoutAlgorithmPtr = wx.windows.LayoutAlgorithmPtr
wxPopupWindow = wx.windows.PopupWindow
wxPopupWindowPtr = wx.windows.PopupWindowPtr
wxPrePopupWindow = wx.windows.PrePopupWindow
wxPopupTransientWindow = wx.windows.PopupTransientWindow
wxPopupTransientWindowPtr = wx.windows.PopupTransientWindowPtr
wxPrePopupTransientWindow = wx.windows.PrePopupTransientWindow
wxTipWindow = wx.windows.TipWindow
wxTipWindowPtr = wx.windows.TipWindowPtr
wxVScrolledWindow = wx.windows.VScrolledWindow
wxVScrolledWindowPtr = wx.windows.VScrolledWindowPtr
wxPreVScrolledWindow = wx.windows.PreVScrolledWindow
wxVListBoxNameStr = wx.windows.VListBoxNameStr
wxVListBox = wx.windows.VListBox
wxVListBoxPtr = wx.windows.VListBoxPtr
wxPreVListBox = wx.windows.PreVListBox
wxHtmlListBox = wx.windows.HtmlListBox
wxHtmlListBoxPtr = wx.windows.HtmlListBoxPtr
wxPreHtmlListBox = wx.windows.PreHtmlListBox
wxTaskBarIcon = wx.windows.TaskBarIcon
wxTaskBarIconPtr = wx.windows.TaskBarIconPtr
wxTaskBarIconEvent = wx.windows.TaskBarIconEvent
wxTaskBarIconEventPtr = wx.windows.TaskBarIconEventPtr
wxEVT_TASKBAR_MOVE = wx.windows.wxEVT_TASKBAR_MOVE
wxEVT_TASKBAR_LEFT_DOWN = wx.windows.wxEVT_TASKBAR_LEFT_DOWN
wxEVT_TASKBAR_LEFT_UP = wx.windows.wxEVT_TASKBAR_LEFT_UP
wxEVT_TASKBAR_RIGHT_DOWN = wx.windows.wxEVT_TASKBAR_RIGHT_DOWN
wxEVT_TASKBAR_RIGHT_UP = wx.windows.wxEVT_TASKBAR_RIGHT_UP
wxEVT_TASKBAR_LEFT_DCLICK = wx.windows.wxEVT_TASKBAR_LEFT_DCLICK
wxEVT_TASKBAR_RIGHT_DCLICK = wx.windows.wxEVT_TASKBAR_RIGHT_DCLICK
wxFileSelectorPromptStr = wx.windows.FileSelectorPromptStr
wxDirSelectorPromptStr = wx.windows.DirSelectorPromptStr
wxDirDialogNameStr = wx.windows.DirDialogNameStr
wxFileSelectorDefaultWildcardStr = wx.windows.FileSelectorDefaultWildcardStr
wxGetTextFromUserPromptStr = wx.windows.GetTextFromUserPromptStr
wxMessageBoxCaptionStr = wx.windows.MessageBoxCaptionStr
wxColourData = wx.windows.ColourData
wxColourDataPtr = wx.windows.ColourDataPtr
wxColourDialog = wx.windows.ColourDialog
wxColourDialogPtr = wx.windows.ColourDialogPtr
wxDirDialog = wx.windows.DirDialog
wxDirDialogPtr = wx.windows.DirDialogPtr
wxFileDialog = wx.windows.FileDialog
wxFileDialogPtr = wx.windows.FileDialogPtr
wxCHOICEDLG_STYLE = wx.windows.CHOICEDLG_STYLE
wxMultiChoiceDialog = wx.windows.MultiChoiceDialog
wxMultiChoiceDialogPtr = wx.windows.MultiChoiceDialogPtr
wxSingleChoiceDialog = wx.windows.SingleChoiceDialog
wxSingleChoiceDialogPtr = wx.windows.SingleChoiceDialogPtr
wxTextEntryDialog = wx.windows.TextEntryDialog
wxTextEntryDialogPtr = wx.windows.TextEntryDialogPtr
wxFontData = wx.windows.FontData
wxFontDataPtr = wx.windows.FontDataPtr
wxFontDialog = wx.windows.FontDialog
wxFontDialogPtr = wx.windows.FontDialogPtr
wxMessageDialog = wx.windows.MessageDialog
wxMessageDialogPtr = wx.windows.MessageDialogPtr
wxProgressDialog = wx.windows.ProgressDialog
wxProgressDialogPtr = wx.windows.ProgressDialogPtr
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
wxFindDialogEventPtr = wx.windows.FindDialogEventPtr
wxFindReplaceData = wx.windows.FindReplaceData
wxFindReplaceDataPtr = wx.windows.FindReplaceDataPtr
wxFindReplaceDialog = wx.windows.FindReplaceDialog
wxFindReplaceDialogPtr = wx.windows.FindReplaceDialogPtr
wxPreFindReplaceDialog = wx.windows.PreFindReplaceDialog
IDM_WINDOWTILE = wx.windows.IDM_WINDOWTILE
IDM_WINDOWTILEHOR = wx.windows.IDM_WINDOWTILEHOR
IDM_WINDOWCASCADE = wx.windows.IDM_WINDOWCASCADE
IDM_WINDOWICONS = wx.windows.IDM_WINDOWICONS
IDM_WINDOWNEXT = wx.windows.IDM_WINDOWNEXT
IDM_WINDOWTILEVERT = wx.windows.IDM_WINDOWTILEVERT
wxFIRST_MDI_CHILD = wx.windows.FIRST_MDI_CHILD
wxLAST_MDI_CHILD = wx.windows.LAST_MDI_CHILD
wxMDIParentFrame = wx.windows.MDIParentFrame
wxMDIParentFramePtr = wx.windows.MDIParentFramePtr
wxPreMDIParentFrame = wx.windows.PreMDIParentFrame
wxMDIChildFrame = wx.windows.MDIChildFrame
wxMDIChildFramePtr = wx.windows.MDIChildFramePtr
wxPreMDIChildFrame = wx.windows.PreMDIChildFrame
wxMDIClientWindow = wx.windows.MDIClientWindow
wxMDIClientWindowPtr = wx.windows.MDIClientWindowPtr
wxPreMDIClientWindow = wx.windows.PreMDIClientWindow
wxPyWindow = wx.windows.PyWindow
wxPyWindowPtr = wx.windows.PyWindowPtr
wxPyPanel = wx.windows.PyPanel
wxPyPanelPtr = wx.windows.PyPanelPtr
wxPrintoutTitleStr = wx.windows.PrintoutTitleStr
wxPreviewCanvasNameStr = wx.windows.PreviewCanvasNameStr
wxPRINT_MODE_NONE = wx.windows.PRINT_MODE_NONE
wxPRINT_MODE_PREVIEW = wx.windows.PRINT_MODE_PREVIEW
wxPRINT_MODE_FILE = wx.windows.PRINT_MODE_FILE
wxPRINT_MODE_PRINTER = wx.windows.PRINT_MODE_PRINTER
wxPRINT_MODE_STREAM = wx.windows.PRINT_MODE_STREAM
wxPrintData = wx.windows.PrintData
wxPrintDataPtr = wx.windows.PrintDataPtr
wxPageSetupDialogData = wx.windows.PageSetupDialogData
wxPageSetupDialogDataPtr = wx.windows.PageSetupDialogDataPtr
wxPageSetupDialog = wx.windows.PageSetupDialog
wxPageSetupDialogPtr = wx.windows.PageSetupDialogPtr
wxPrintDialogData = wx.windows.PrintDialogData
wxPrintDialogDataPtr = wx.windows.PrintDialogDataPtr
wxPrintDialog = wx.windows.PrintDialog
wxPrintDialogPtr = wx.windows.PrintDialogPtr
wxPRINTER_NO_ERROR = wx.windows.PRINTER_NO_ERROR
wxPRINTER_CANCELLED = wx.windows.PRINTER_CANCELLED
wxPRINTER_ERROR = wx.windows.PRINTER_ERROR
wxPrinter = wx.windows.Printer
wxPrinterPtr = wx.windows.PrinterPtr
wxPrinter_GetLastError = wx.windows.Printer_GetLastError
wxPrintout = wx.windows.Printout
wxPrintoutPtr = wx.windows.PrintoutPtr
wxPreviewCanvas = wx.windows.PreviewCanvas
wxPreviewCanvasPtr = wx.windows.PreviewCanvasPtr
wxPreviewFrame = wx.windows.PreviewFrame
wxPreviewFramePtr = wx.windows.PreviewFramePtr
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
wxPreviewControlBarPtr = wx.windows.PreviewControlBarPtr
wxPrintPreview = wx.windows.PrintPreview
wxPrintPreviewPtr = wx.windows.PrintPreviewPtr
wxPyPrintPreview = wx.windows.PyPrintPreview
wxPyPrintPreviewPtr = wx.windows.PyPrintPreviewPtr
wxPyPreviewFrame = wx.windows.PyPreviewFrame
wxPyPreviewFramePtr = wx.windows.PyPreviewFramePtr
wxPyPreviewControlBar = wx.windows.PyPreviewControlBar
wxPyPreviewControlBarPtr = wx.windows.PyPreviewControlBarPtr


d = globals()
for k, v in wx.windows.__dict__.iteritems():
    if k.startswith('EVT'):
        d[k] = v
    elif k.startswith('IDM'):
        d[k] = v
del d, k, v



