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

import wx.controls

sys.modules['wxPython.wx'] = _wx
del sys, _wx


# Now assign all the reverse-renamed names:
wxButtonNameStr = wx.controls.ButtonNameStr
wxBU_LEFT = wx.controls.BU_LEFT
wxBU_TOP = wx.controls.BU_TOP
wxBU_RIGHT = wx.controls.BU_RIGHT
wxBU_BOTTOM = wx.controls.BU_BOTTOM
wxBU_EXACTFIT = wx.controls.BU_EXACTFIT
wxBU_AUTODRAW = wx.controls.BU_AUTODRAW
wxButton = wx.controls.Button
wxButtonPtr = wx.controls.ButtonPtr
wxPreButton = wx.controls.PreButton
wxButton_GetDefaultSize = wx.controls.Button_GetDefaultSize
wxBitmapButton = wx.controls.BitmapButton
wxBitmapButtonPtr = wx.controls.BitmapButtonPtr
wxPreBitmapButton = wx.controls.PreBitmapButton
wxCheckBoxNameStr = wx.controls.CheckBoxNameStr
wxCHK_2STATE = wx.controls.CHK_2STATE
wxCHK_3STATE = wx.controls.CHK_3STATE
wxCHK_ALLOW_3RD_STATE_FOR_USER = wx.controls.CHK_ALLOW_3RD_STATE_FOR_USER
wxCHK_UNCHECKED = wx.controls.CHK_UNCHECKED
wxCHK_CHECKED = wx.controls.CHK_CHECKED
wxCHK_UNDETERMINED = wx.controls.CHK_UNDETERMINED
wxCheckBox = wx.controls.CheckBox
wxCheckBoxPtr = wx.controls.CheckBoxPtr
wxPreCheckBox = wx.controls.PreCheckBox
wxChoiceNameStr = wx.controls.ChoiceNameStr
wxChoice = wx.controls.Choice
wxChoicePtr = wx.controls.ChoicePtr
wxPreChoice = wx.controls.PreChoice
wxComboBoxNameStr = wx.controls.ComboBoxNameStr
wxComboBox = wx.controls.ComboBox
wxComboBoxPtr = wx.controls.ComboBoxPtr
wxPreComboBox = wx.controls.PreComboBox
wxGaugeNameStr = wx.controls.GaugeNameStr
wxGA_HORIZONTAL = wx.controls.GA_HORIZONTAL
wxGA_VERTICAL = wx.controls.GA_VERTICAL
wxGA_SMOOTH = wx.controls.GA_SMOOTH
wxGA_PROGRESSBAR = wx.controls.GA_PROGRESSBAR
wxGauge = wx.controls.Gauge
wxGaugePtr = wx.controls.GaugePtr
wxPreGauge = wx.controls.PreGauge
wxStaticBitmapNameStr = wx.controls.StaticBitmapNameStr
wxStaticBoxNameStr = wx.controls.StaticBoxNameStr
wxStaticTextNameStr = wx.controls.StaticTextNameStr
wxStaticBox = wx.controls.StaticBox
wxStaticBoxPtr = wx.controls.StaticBoxPtr
wxPreStaticBox = wx.controls.PreStaticBox
wxStaticLine = wx.controls.StaticLine
wxStaticLinePtr = wx.controls.StaticLinePtr
wxPreStaticLine = wx.controls.PreStaticLine
wxStaticLine_GetDefaultSize = wx.controls.StaticLine_GetDefaultSize
wxStaticText = wx.controls.StaticText
wxStaticTextPtr = wx.controls.StaticTextPtr
wxPreStaticText = wx.controls.PreStaticText
wxStaticBitmap = wx.controls.StaticBitmap
wxStaticBitmapPtr = wx.controls.StaticBitmapPtr
wxPreStaticBitmap = wx.controls.PreStaticBitmap
wxListBoxNameStr = wx.controls.ListBoxNameStr
wxListBox = wx.controls.ListBox
wxListBoxPtr = wx.controls.ListBoxPtr
wxPreListBox = wx.controls.PreListBox
wxCheckListBox = wx.controls.CheckListBox
wxCheckListBoxPtr = wx.controls.CheckListBoxPtr
wxPreCheckListBox = wx.controls.PreCheckListBox
wxTextCtrlNameStr = wx.controls.TextCtrlNameStr
wxTE_NO_VSCROLL = wx.controls.TE_NO_VSCROLL
wxTE_AUTO_SCROLL = wx.controls.TE_AUTO_SCROLL
wxTE_READONLY = wx.controls.TE_READONLY
wxTE_MULTILINE = wx.controls.TE_MULTILINE
wxTE_PROCESS_TAB = wx.controls.TE_PROCESS_TAB
wxTE_LEFT = wx.controls.TE_LEFT
wxTE_CENTER = wx.controls.TE_CENTER
wxTE_RIGHT = wx.controls.TE_RIGHT
wxTE_CENTRE = wx.controls.TE_CENTRE
wxTE_RICH = wx.controls.TE_RICH
wxTE_PROCESS_ENTER = wx.controls.TE_PROCESS_ENTER
wxTE_PASSWORD = wx.controls.TE_PASSWORD
wxTE_AUTO_URL = wx.controls.TE_AUTO_URL
wxTE_NOHIDESEL = wx.controls.TE_NOHIDESEL
wxTE_DONTWRAP = wx.controls.TE_DONTWRAP
wxTE_LINEWRAP = wx.controls.TE_LINEWRAP
wxTE_WORDWRAP = wx.controls.TE_WORDWRAP
wxTE_RICH2 = wx.controls.TE_RICH2
wxTEXT_ALIGNMENT_DEFAULT = wx.controls.TEXT_ALIGNMENT_DEFAULT
wxTEXT_ALIGNMENT_LEFT = wx.controls.TEXT_ALIGNMENT_LEFT
wxTEXT_ALIGNMENT_CENTRE = wx.controls.TEXT_ALIGNMENT_CENTRE
wxTEXT_ALIGNMENT_CENTER = wx.controls.TEXT_ALIGNMENT_CENTER
wxTEXT_ALIGNMENT_RIGHT = wx.controls.TEXT_ALIGNMENT_RIGHT
wxTEXT_ALIGNMENT_JUSTIFIED = wx.controls.TEXT_ALIGNMENT_JUSTIFIED
wxTEXT_ATTR_TEXT_COLOUR = wx.controls.TEXT_ATTR_TEXT_COLOUR
wxTEXT_ATTR_BACKGROUND_COLOUR = wx.controls.TEXT_ATTR_BACKGROUND_COLOUR
wxTEXT_ATTR_FONT_FACE = wx.controls.TEXT_ATTR_FONT_FACE
wxTEXT_ATTR_FONT_SIZE = wx.controls.TEXT_ATTR_FONT_SIZE
wxTEXT_ATTR_FONT_WEIGHT = wx.controls.TEXT_ATTR_FONT_WEIGHT
wxTEXT_ATTR_FONT_ITALIC = wx.controls.TEXT_ATTR_FONT_ITALIC
wxTEXT_ATTR_FONT_UNDERLINE = wx.controls.TEXT_ATTR_FONT_UNDERLINE
wxTEXT_ATTR_FONT = wx.controls.TEXT_ATTR_FONT
wxTEXT_ATTR_ALIGNMENT = wx.controls.TEXT_ATTR_ALIGNMENT
wxTEXT_ATTR_LEFT_INDENT = wx.controls.TEXT_ATTR_LEFT_INDENT
wxTEXT_ATTR_RIGHT_INDENT = wx.controls.TEXT_ATTR_RIGHT_INDENT
wxTEXT_ATTR_TABS = wx.controls.TEXT_ATTR_TABS
wxTextAttr = wx.controls.TextAttr
wxTextAttrPtr = wx.controls.TextAttrPtr
wxTextAttr_Combine = wx.controls.TextAttr_Combine
wxTextCtrl = wx.controls.TextCtrl
wxTextCtrlPtr = wx.controls.TextCtrlPtr
wxPreTextCtrl = wx.controls.PreTextCtrl
wxEVT_COMMAND_TEXT_UPDATED = wx.controls.wxEVT_COMMAND_TEXT_UPDATED
wxEVT_COMMAND_TEXT_ENTER = wx.controls.wxEVT_COMMAND_TEXT_ENTER
wxEVT_COMMAND_TEXT_URL = wx.controls.wxEVT_COMMAND_TEXT_URL
wxEVT_COMMAND_TEXT_MAXLEN = wx.controls.wxEVT_COMMAND_TEXT_MAXLEN
wxTextUrlEvent = wx.controls.TextUrlEvent
wxTextUrlEventPtr = wx.controls.TextUrlEventPtr
wxScrollBarNameStr = wx.controls.ScrollBarNameStr
wxScrollBar = wx.controls.ScrollBar
wxScrollBarPtr = wx.controls.ScrollBarPtr
wxPreScrollBar = wx.controls.PreScrollBar
wxSPIN_BUTTON_NAME = wx.controls.SPIN_BUTTON_NAME
wxSpinCtrlNameStr = wx.controls.SpinCtrlNameStr
wxSP_HORIZONTAL = wx.controls.SP_HORIZONTAL
wxSP_VERTICAL = wx.controls.SP_VERTICAL
wxSP_ARROW_KEYS = wx.controls.SP_ARROW_KEYS
wxSP_WRAP = wx.controls.SP_WRAP
wxSpinButton = wx.controls.SpinButton
wxSpinButtonPtr = wx.controls.SpinButtonPtr
wxPreSpinButton = wx.controls.PreSpinButton
wxSpinCtrl = wx.controls.SpinCtrl
wxSpinCtrlPtr = wx.controls.SpinCtrlPtr
wxPreSpinCtrl = wx.controls.PreSpinCtrl
wxEVT_COMMAND_SPINCTRL_UPDATED = wx.controls.wxEVT_COMMAND_SPINCTRL_UPDATED
wxRadioBoxNameStr = wx.controls.RadioBoxNameStr
wxRadioButtonNameStr = wx.controls.RadioButtonNameStr
wxRadioBox = wx.controls.RadioBox
wxRadioBoxPtr = wx.controls.RadioBoxPtr
wxPreRadioBox = wx.controls.PreRadioBox
wxRadioButton = wx.controls.RadioButton
wxRadioButtonPtr = wx.controls.RadioButtonPtr
wxPreRadioButton = wx.controls.PreRadioButton
wxSliderNameStr = wx.controls.SliderNameStr
wxSlider = wx.controls.Slider
wxSliderPtr = wx.controls.SliderPtr
wxPreSlider = wx.controls.PreSlider
wxToggleButtonNameStr = wx.controls.ToggleButtonNameStr
wxEVT_COMMAND_TOGGLEBUTTON_CLICKED = wx.controls.wxEVT_COMMAND_TOGGLEBUTTON_CLICKED
wxToggleButton = wx.controls.ToggleButton
wxToggleButtonPtr = wx.controls.ToggleButtonPtr
wxPreToggleButton = wx.controls.PreToggleButton
wxNOTEBOOK_NAME = wx.controls.NOTEBOOK_NAME
wxBookCtrl = wx.controls.BookCtrl
wxBookCtrlPtr = wx.controls.BookCtrlPtr
wxBookCtrlEvent = wx.controls.BookCtrlEvent
wxBookCtrlEventPtr = wx.controls.BookCtrlEventPtr
wxNB_FIXEDWIDTH = wx.controls.NB_FIXEDWIDTH
wxNB_TOP = wx.controls.NB_TOP
wxNB_LEFT = wx.controls.NB_LEFT
wxNB_RIGHT = wx.controls.NB_RIGHT
wxNB_BOTTOM = wx.controls.NB_BOTTOM
wxNB_MULTILINE = wx.controls.NB_MULTILINE
wxNB_HITTEST_NOWHERE = wx.controls.NB_HITTEST_NOWHERE
wxNB_HITTEST_ONICON = wx.controls.NB_HITTEST_ONICON
wxNB_HITTEST_ONLABEL = wx.controls.NB_HITTEST_ONLABEL
wxNB_HITTEST_ONITEM = wx.controls.NB_HITTEST_ONITEM
wxNotebook = wx.controls.Notebook
wxNotebookPtr = wx.controls.NotebookPtr
wxPreNotebook = wx.controls.PreNotebook
wxNotebookEvent = wx.controls.NotebookEvent
wxNotebookEventPtr = wx.controls.NotebookEventPtr
wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED = wx.controls.wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED
wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGING = wx.controls.wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGING
wxLB_DEFAULT = wx.controls.LB_DEFAULT
wxLB_TOP = wx.controls.LB_TOP
wxLB_BOTTOM = wx.controls.LB_BOTTOM
wxLB_LEFT = wx.controls.LB_LEFT
wxLB_RIGHT = wx.controls.LB_RIGHT
wxLB_ALIGN_MASK = wx.controls.LB_ALIGN_MASK
wxListbook = wx.controls.Listbook
wxListbookPtr = wx.controls.ListbookPtr
wxPreListbook = wx.controls.PreListbook
wxListbookEvent = wx.controls.ListbookEvent
wxListbookEventPtr = wx.controls.ListbookEventPtr
wxEVT_COMMAND_LISTBOOK_PAGE_CHANGED = wx.controls.wxEVT_COMMAND_LISTBOOK_PAGE_CHANGED
wxEVT_COMMAND_LISTBOOK_PAGE_CHANGING = wx.controls.wxEVT_COMMAND_LISTBOOK_PAGE_CHANGING
wxBookCtrlSizer = wx.controls.BookCtrlSizer
wxBookCtrlSizerPtr = wx.controls.BookCtrlSizerPtr
wxNotebookSizer = wx.controls.NotebookSizer
wxNotebookSizerPtr = wx.controls.NotebookSizerPtr
wxTOOL_STYLE_BUTTON = wx.controls.TOOL_STYLE_BUTTON
wxTOOL_STYLE_SEPARATOR = wx.controls.TOOL_STYLE_SEPARATOR
wxTOOL_STYLE_CONTROL = wx.controls.TOOL_STYLE_CONTROL
wxTB_HORIZONTAL = wx.controls.TB_HORIZONTAL
wxTB_VERTICAL = wx.controls.TB_VERTICAL
wxTB_3DBUTTONS = wx.controls.TB_3DBUTTONS
wxTB_FLAT = wx.controls.TB_FLAT
wxTB_DOCKABLE = wx.controls.TB_DOCKABLE
wxTB_NOICONS = wx.controls.TB_NOICONS
wxTB_TEXT = wx.controls.TB_TEXT
wxTB_NODIVIDER = wx.controls.TB_NODIVIDER
wxTB_NOALIGN = wx.controls.TB_NOALIGN
wxTB_HORZ_LAYOUT = wx.controls.TB_HORZ_LAYOUT
wxTB_HORZ_TEXT = wx.controls.TB_HORZ_TEXT
wxToolBarToolBase = wx.controls.ToolBarToolBase
wxToolBarToolBasePtr = wx.controls.ToolBarToolBasePtr
wxToolBarBase = wx.controls.ToolBarBase
wxToolBarBasePtr = wx.controls.ToolBarBasePtr
wxToolBar = wx.controls.ToolBar
wxToolBarPtr = wx.controls.ToolBarPtr
wxPreToolBar = wx.controls.PreToolBar
wxListCtrlNameStr = wx.controls.ListCtrlNameStr
wxLC_VRULES = wx.controls.LC_VRULES
wxLC_HRULES = wx.controls.LC_HRULES
wxLC_ICON = wx.controls.LC_ICON
wxLC_SMALL_ICON = wx.controls.LC_SMALL_ICON
wxLC_LIST = wx.controls.LC_LIST
wxLC_REPORT = wx.controls.LC_REPORT
wxLC_ALIGN_TOP = wx.controls.LC_ALIGN_TOP
wxLC_ALIGN_LEFT = wx.controls.LC_ALIGN_LEFT
wxLC_AUTOARRANGE = wx.controls.LC_AUTOARRANGE
wxLC_VIRTUAL = wx.controls.LC_VIRTUAL
wxLC_EDIT_LABELS = wx.controls.LC_EDIT_LABELS
wxLC_NO_HEADER = wx.controls.LC_NO_HEADER
wxLC_NO_SORT_HEADER = wx.controls.LC_NO_SORT_HEADER
wxLC_SINGLE_SEL = wx.controls.LC_SINGLE_SEL
wxLC_SORT_ASCENDING = wx.controls.LC_SORT_ASCENDING
wxLC_SORT_DESCENDING = wx.controls.LC_SORT_DESCENDING
wxLC_MASK_TYPE = wx.controls.LC_MASK_TYPE
wxLC_MASK_ALIGN = wx.controls.LC_MASK_ALIGN
wxLC_MASK_SORT = wx.controls.LC_MASK_SORT
wxLIST_MASK_STATE = wx.controls.LIST_MASK_STATE
wxLIST_MASK_TEXT = wx.controls.LIST_MASK_TEXT
wxLIST_MASK_IMAGE = wx.controls.LIST_MASK_IMAGE
wxLIST_MASK_DATA = wx.controls.LIST_MASK_DATA
wxLIST_SET_ITEM = wx.controls.LIST_SET_ITEM
wxLIST_MASK_WIDTH = wx.controls.LIST_MASK_WIDTH
wxLIST_MASK_FORMAT = wx.controls.LIST_MASK_FORMAT
wxLIST_STATE_DONTCARE = wx.controls.LIST_STATE_DONTCARE
wxLIST_STATE_DROPHILITED = wx.controls.LIST_STATE_DROPHILITED
wxLIST_STATE_FOCUSED = wx.controls.LIST_STATE_FOCUSED
wxLIST_STATE_SELECTED = wx.controls.LIST_STATE_SELECTED
wxLIST_STATE_CUT = wx.controls.LIST_STATE_CUT
wxLIST_STATE_DISABLED = wx.controls.LIST_STATE_DISABLED
wxLIST_STATE_FILTERED = wx.controls.LIST_STATE_FILTERED
wxLIST_STATE_INUSE = wx.controls.LIST_STATE_INUSE
wxLIST_STATE_PICKED = wx.controls.LIST_STATE_PICKED
wxLIST_STATE_SOURCE = wx.controls.LIST_STATE_SOURCE
wxLIST_HITTEST_ABOVE = wx.controls.LIST_HITTEST_ABOVE
wxLIST_HITTEST_BELOW = wx.controls.LIST_HITTEST_BELOW
wxLIST_HITTEST_NOWHERE = wx.controls.LIST_HITTEST_NOWHERE
wxLIST_HITTEST_ONITEMICON = wx.controls.LIST_HITTEST_ONITEMICON
wxLIST_HITTEST_ONITEMLABEL = wx.controls.LIST_HITTEST_ONITEMLABEL
wxLIST_HITTEST_ONITEMRIGHT = wx.controls.LIST_HITTEST_ONITEMRIGHT
wxLIST_HITTEST_ONITEMSTATEICON = wx.controls.LIST_HITTEST_ONITEMSTATEICON
wxLIST_HITTEST_TOLEFT = wx.controls.LIST_HITTEST_TOLEFT
wxLIST_HITTEST_TORIGHT = wx.controls.LIST_HITTEST_TORIGHT
wxLIST_HITTEST_ONITEM = wx.controls.LIST_HITTEST_ONITEM
wxLIST_NEXT_ABOVE = wx.controls.LIST_NEXT_ABOVE
wxLIST_NEXT_ALL = wx.controls.LIST_NEXT_ALL
wxLIST_NEXT_BELOW = wx.controls.LIST_NEXT_BELOW
wxLIST_NEXT_LEFT = wx.controls.LIST_NEXT_LEFT
wxLIST_NEXT_RIGHT = wx.controls.LIST_NEXT_RIGHT
wxLIST_ALIGN_DEFAULT = wx.controls.LIST_ALIGN_DEFAULT
wxLIST_ALIGN_LEFT = wx.controls.LIST_ALIGN_LEFT
wxLIST_ALIGN_TOP = wx.controls.LIST_ALIGN_TOP
wxLIST_ALIGN_SNAP_TO_GRID = wx.controls.LIST_ALIGN_SNAP_TO_GRID
wxLIST_FORMAT_LEFT = wx.controls.LIST_FORMAT_LEFT
wxLIST_FORMAT_RIGHT = wx.controls.LIST_FORMAT_RIGHT
wxLIST_FORMAT_CENTRE = wx.controls.LIST_FORMAT_CENTRE
wxLIST_FORMAT_CENTER = wx.controls.LIST_FORMAT_CENTER
wxLIST_AUTOSIZE = wx.controls.LIST_AUTOSIZE
wxLIST_AUTOSIZE_USEHEADER = wx.controls.LIST_AUTOSIZE_USEHEADER
wxLIST_RECT_BOUNDS = wx.controls.LIST_RECT_BOUNDS
wxLIST_RECT_ICON = wx.controls.LIST_RECT_ICON
wxLIST_RECT_LABEL = wx.controls.LIST_RECT_LABEL
wxLIST_FIND_UP = wx.controls.LIST_FIND_UP
wxLIST_FIND_DOWN = wx.controls.LIST_FIND_DOWN
wxLIST_FIND_LEFT = wx.controls.LIST_FIND_LEFT
wxLIST_FIND_RIGHT = wx.controls.LIST_FIND_RIGHT
wxListItemAttr = wx.controls.ListItemAttr
wxListItemAttrPtr = wx.controls.ListItemAttrPtr
wxListItem = wx.controls.ListItem
wxListItemPtr = wx.controls.ListItemPtr
wxListEvent = wx.controls.ListEvent
wxListEventPtr = wx.controls.ListEventPtr
wxEVT_COMMAND_LIST_BEGIN_DRAG = wx.controls.wxEVT_COMMAND_LIST_BEGIN_DRAG
wxEVT_COMMAND_LIST_BEGIN_RDRAG = wx.controls.wxEVT_COMMAND_LIST_BEGIN_RDRAG
wxEVT_COMMAND_LIST_BEGIN_LABEL_EDIT = wx.controls.wxEVT_COMMAND_LIST_BEGIN_LABEL_EDIT
wxEVT_COMMAND_LIST_END_LABEL_EDIT = wx.controls.wxEVT_COMMAND_LIST_END_LABEL_EDIT
wxEVT_COMMAND_LIST_DELETE_ITEM = wx.controls.wxEVT_COMMAND_LIST_DELETE_ITEM
wxEVT_COMMAND_LIST_DELETE_ALL_ITEMS = wx.controls.wxEVT_COMMAND_LIST_DELETE_ALL_ITEMS
wxEVT_COMMAND_LIST_GET_INFO = wx.controls.wxEVT_COMMAND_LIST_GET_INFO
wxEVT_COMMAND_LIST_SET_INFO = wx.controls.wxEVT_COMMAND_LIST_SET_INFO
wxEVT_COMMAND_LIST_ITEM_SELECTED = wx.controls.wxEVT_COMMAND_LIST_ITEM_SELECTED
wxEVT_COMMAND_LIST_ITEM_DESELECTED = wx.controls.wxEVT_COMMAND_LIST_ITEM_DESELECTED
wxEVT_COMMAND_LIST_KEY_DOWN = wx.controls.wxEVT_COMMAND_LIST_KEY_DOWN
wxEVT_COMMAND_LIST_INSERT_ITEM = wx.controls.wxEVT_COMMAND_LIST_INSERT_ITEM
wxEVT_COMMAND_LIST_COL_CLICK = wx.controls.wxEVT_COMMAND_LIST_COL_CLICK
wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK = wx.controls.wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK
wxEVT_COMMAND_LIST_ITEM_MIDDLE_CLICK = wx.controls.wxEVT_COMMAND_LIST_ITEM_MIDDLE_CLICK
wxEVT_COMMAND_LIST_ITEM_ACTIVATED = wx.controls.wxEVT_COMMAND_LIST_ITEM_ACTIVATED
wxEVT_COMMAND_LIST_CACHE_HINT = wx.controls.wxEVT_COMMAND_LIST_CACHE_HINT
wxEVT_COMMAND_LIST_COL_RIGHT_CLICK = wx.controls.wxEVT_COMMAND_LIST_COL_RIGHT_CLICK
wxEVT_COMMAND_LIST_COL_BEGIN_DRAG = wx.controls.wxEVT_COMMAND_LIST_COL_BEGIN_DRAG
wxEVT_COMMAND_LIST_COL_DRAGGING = wx.controls.wxEVT_COMMAND_LIST_COL_DRAGGING
wxEVT_COMMAND_LIST_COL_END_DRAG = wx.controls.wxEVT_COMMAND_LIST_COL_END_DRAG
wxEVT_COMMAND_LIST_ITEM_FOCUSED = wx.controls.wxEVT_COMMAND_LIST_ITEM_FOCUSED
wxListCtrl = wx.controls.ListCtrl
wxListCtrlPtr = wx.controls.ListCtrlPtr
wxPreListCtrl = wx.controls.PreListCtrl
wxListView = wx.controls.ListView
wxListViewPtr = wx.controls.ListViewPtr
wxPreListView = wx.controls.PreListView
wxTreeCtrlNameStr = wx.controls.TreeCtrlNameStr
wxTR_NO_BUTTONS = wx.controls.TR_NO_BUTTONS
wxTR_HAS_BUTTONS = wx.controls.TR_HAS_BUTTONS
wxTR_NO_LINES = wx.controls.TR_NO_LINES
wxTR_LINES_AT_ROOT = wx.controls.TR_LINES_AT_ROOT
wxTR_SINGLE = wx.controls.TR_SINGLE
wxTR_MULTIPLE = wx.controls.TR_MULTIPLE
wxTR_EXTENDED = wx.controls.TR_EXTENDED
wxTR_HAS_VARIABLE_ROW_HEIGHT = wx.controls.TR_HAS_VARIABLE_ROW_HEIGHT
wxTR_EDIT_LABELS = wx.controls.TR_EDIT_LABELS
wxTR_HIDE_ROOT = wx.controls.TR_HIDE_ROOT
wxTR_ROW_LINES = wx.controls.TR_ROW_LINES
wxTR_FULL_ROW_HIGHLIGHT = wx.controls.TR_FULL_ROW_HIGHLIGHT
wxTR_DEFAULT_STYLE = wx.controls.TR_DEFAULT_STYLE
wxTR_TWIST_BUTTONS = wx.controls.TR_TWIST_BUTTONS
wxTR_MAC_BUTTONS = wx.controls.TR_MAC_BUTTONS
wxTR_AQUA_BUTTONS = wx.controls.TR_AQUA_BUTTONS
wxTreeItemIcon_Normal = wx.controls.TreeItemIcon_Normal
wxTreeItemIcon_Selected = wx.controls.TreeItemIcon_Selected
wxTreeItemIcon_Expanded = wx.controls.TreeItemIcon_Expanded
wxTreeItemIcon_SelectedExpanded = wx.controls.TreeItemIcon_SelectedExpanded
wxTreeItemIcon_Max = wx.controls.TreeItemIcon_Max
wxTREE_HITTEST_ABOVE = wx.controls.TREE_HITTEST_ABOVE
wxTREE_HITTEST_BELOW = wx.controls.TREE_HITTEST_BELOW
wxTREE_HITTEST_NOWHERE = wx.controls.TREE_HITTEST_NOWHERE
wxTREE_HITTEST_ONITEMBUTTON = wx.controls.TREE_HITTEST_ONITEMBUTTON
wxTREE_HITTEST_ONITEMICON = wx.controls.TREE_HITTEST_ONITEMICON
wxTREE_HITTEST_ONITEMINDENT = wx.controls.TREE_HITTEST_ONITEMINDENT
wxTREE_HITTEST_ONITEMLABEL = wx.controls.TREE_HITTEST_ONITEMLABEL
wxTREE_HITTEST_ONITEMRIGHT = wx.controls.TREE_HITTEST_ONITEMRIGHT
wxTREE_HITTEST_ONITEMSTATEICON = wx.controls.TREE_HITTEST_ONITEMSTATEICON
wxTREE_HITTEST_TOLEFT = wx.controls.TREE_HITTEST_TOLEFT
wxTREE_HITTEST_TORIGHT = wx.controls.TREE_HITTEST_TORIGHT
wxTREE_HITTEST_ONITEMUPPERPART = wx.controls.TREE_HITTEST_ONITEMUPPERPART
wxTREE_HITTEST_ONITEMLOWERPART = wx.controls.TREE_HITTEST_ONITEMLOWERPART
wxTREE_HITTEST_ONITEM = wx.controls.TREE_HITTEST_ONITEM
wxTreeItemId = wx.controls.TreeItemId
wxTreeItemIdPtr = wx.controls.TreeItemIdPtr
wxTreeItemData = wx.controls.TreeItemData
wxTreeItemDataPtr = wx.controls.TreeItemDataPtr
wxEVT_COMMAND_TREE_BEGIN_DRAG = wx.controls.wxEVT_COMMAND_TREE_BEGIN_DRAG
wxEVT_COMMAND_TREE_BEGIN_RDRAG = wx.controls.wxEVT_COMMAND_TREE_BEGIN_RDRAG
wxEVT_COMMAND_TREE_BEGIN_LABEL_EDIT = wx.controls.wxEVT_COMMAND_TREE_BEGIN_LABEL_EDIT
wxEVT_COMMAND_TREE_END_LABEL_EDIT = wx.controls.wxEVT_COMMAND_TREE_END_LABEL_EDIT
wxEVT_COMMAND_TREE_DELETE_ITEM = wx.controls.wxEVT_COMMAND_TREE_DELETE_ITEM
wxEVT_COMMAND_TREE_GET_INFO = wx.controls.wxEVT_COMMAND_TREE_GET_INFO
wxEVT_COMMAND_TREE_SET_INFO = wx.controls.wxEVT_COMMAND_TREE_SET_INFO
wxEVT_COMMAND_TREE_ITEM_EXPANDED = wx.controls.wxEVT_COMMAND_TREE_ITEM_EXPANDED
wxEVT_COMMAND_TREE_ITEM_EXPANDING = wx.controls.wxEVT_COMMAND_TREE_ITEM_EXPANDING
wxEVT_COMMAND_TREE_ITEM_COLLAPSED = wx.controls.wxEVT_COMMAND_TREE_ITEM_COLLAPSED
wxEVT_COMMAND_TREE_ITEM_COLLAPSING = wx.controls.wxEVT_COMMAND_TREE_ITEM_COLLAPSING
wxEVT_COMMAND_TREE_SEL_CHANGED = wx.controls.wxEVT_COMMAND_TREE_SEL_CHANGED
wxEVT_COMMAND_TREE_SEL_CHANGING = wx.controls.wxEVT_COMMAND_TREE_SEL_CHANGING
wxEVT_COMMAND_TREE_KEY_DOWN = wx.controls.wxEVT_COMMAND_TREE_KEY_DOWN
wxEVT_COMMAND_TREE_ITEM_ACTIVATED = wx.controls.wxEVT_COMMAND_TREE_ITEM_ACTIVATED
wxEVT_COMMAND_TREE_ITEM_RIGHT_CLICK = wx.controls.wxEVT_COMMAND_TREE_ITEM_RIGHT_CLICK
wxEVT_COMMAND_TREE_ITEM_MIDDLE_CLICK = wx.controls.wxEVT_COMMAND_TREE_ITEM_MIDDLE_CLICK
wxEVT_COMMAND_TREE_END_DRAG = wx.controls.wxEVT_COMMAND_TREE_END_DRAG
wxEVT_COMMAND_TREE_STATE_IMAGE_CLICK = wx.controls.wxEVT_COMMAND_TREE_STATE_IMAGE_CLICK
wxEVT_COMMAND_TREE_ITEM_GETTOOLTIP = wx.controls.wxEVT_COMMAND_TREE_ITEM_GETTOOLTIP
wxTreeEvent = wx.controls.TreeEvent
wxTreeEventPtr = wx.controls.TreeEventPtr
wxTreeCtrl = wx.controls.TreeCtrl
wxTreeCtrlPtr = wx.controls.TreeCtrlPtr
wxPreTreeCtrl = wx.controls.PreTreeCtrl
wxDirDialogDefaultFolderStr = wx.controls.DirDialogDefaultFolderStr
wxDIRCTRL_DIR_ONLY = wx.controls.DIRCTRL_DIR_ONLY
wxDIRCTRL_SELECT_FIRST = wx.controls.DIRCTRL_SELECT_FIRST
wxDIRCTRL_SHOW_FILTERS = wx.controls.DIRCTRL_SHOW_FILTERS
wxDIRCTRL_3D_INTERNAL = wx.controls.DIRCTRL_3D_INTERNAL
wxDIRCTRL_EDIT_LABELS = wx.controls.DIRCTRL_EDIT_LABELS
wxGenericDirCtrl = wx.controls.GenericDirCtrl
wxGenericDirCtrlPtr = wx.controls.GenericDirCtrlPtr
wxPreGenericDirCtrl = wx.controls.PreGenericDirCtrl
wxDirFilterListCtrl = wx.controls.DirFilterListCtrl
wxDirFilterListCtrlPtr = wx.controls.DirFilterListCtrlPtr
wxPreDirFilterListCtrl = wx.controls.PreDirFilterListCtrl
wxPyControl = wx.controls.PyControl
wxPyControlPtr = wx.controls.PyControlPtr
wxFRAME_EX_CONTEXTHELP = wx.controls.FRAME_EX_CONTEXTHELP
wxDIALOG_EX_CONTEXTHELP = wx.controls.DIALOG_EX_CONTEXTHELP
wxEVT_HELP = wx.controls.wxEVT_HELP
wxEVT_DETAILED_HELP = wx.controls.wxEVT_DETAILED_HELP
wxHelpEvent = wx.controls.HelpEvent
wxHelpEventPtr = wx.controls.HelpEventPtr
wxContextHelp = wx.controls.ContextHelp
wxContextHelpPtr = wx.controls.ContextHelpPtr
wxContextHelpButton = wx.controls.ContextHelpButton
wxContextHelpButtonPtr = wx.controls.ContextHelpButtonPtr
wxHelpProvider = wx.controls.HelpProvider
wxHelpProviderPtr = wx.controls.HelpProviderPtr
wxHelpProvider_Set = wx.controls.HelpProvider_Set
wxHelpProvider_Get = wx.controls.HelpProvider_Get
wxSimpleHelpProvider = wx.controls.SimpleHelpProvider
wxSimpleHelpProviderPtr = wx.controls.SimpleHelpProviderPtr
wxDragImage = wx.controls.DragImage
wxDragImagePtr = wx.controls.DragImagePtr
wxDragIcon = wx.controls.DragIcon
wxDragString = wx.controls.DragString
wxDragTreeItem = wx.controls.DragTreeItem
wxDragListItem = wx.controls.DragListItem


d = globals()
for k, v in wx.controls.__dict__.iteritems():
    if k.startswith('EVT'):
        d[k] = v
del d, k, v



