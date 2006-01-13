## This file reverse renames symbols in the wx package to give
## them their wx prefix again, for backwards compatibility.
##
## Generated by BuildRenamers in config.py

# This silly stuff here is so the wxPython.wx module doesn't conflict
# with the wx package.  We need to import modules from the wx package
# here, then we'll put the wxPython.wx entry back in sys.modules.
import sys
_wx = None
if sys.modules.has_key('wxPython.wx'):
    _wx = sys.modules['wxPython.wx']
    del sys.modules['wxPython.wx']

import wx._controls

sys.modules['wxPython.wx'] = _wx
del sys, _wx


# Now assign all the reverse-renamed names:
wxButtonNameStr = wx._controls.ButtonNameStr
wxBU_LEFT = wx._controls.BU_LEFT
wxBU_TOP = wx._controls.BU_TOP
wxBU_RIGHT = wx._controls.BU_RIGHT
wxBU_BOTTOM = wx._controls.BU_BOTTOM
wxBU_ALIGN_MASK = wx._controls.BU_ALIGN_MASK
wxBU_EXACTFIT = wx._controls.BU_EXACTFIT
wxBU_AUTODRAW = wx._controls.BU_AUTODRAW
wxButton = wx._controls.Button
wxButtonPtr = wx._controls.ButtonPtr
wxPreButton = wx._controls.PreButton
wxButton_GetDefaultSize = wx._controls.Button_GetDefaultSize
wxButton_GetClassDefaultAttributes = wx._controls.Button_GetClassDefaultAttributes
wxBitmapButton = wx._controls.BitmapButton
wxBitmapButtonPtr = wx._controls.BitmapButtonPtr
wxPreBitmapButton = wx._controls.PreBitmapButton
wxCheckBoxNameStr = wx._controls.CheckBoxNameStr
wxCHK_2STATE = wx._controls.CHK_2STATE
wxCHK_3STATE = wx._controls.CHK_3STATE
wxCHK_ALLOW_3RD_STATE_FOR_USER = wx._controls.CHK_ALLOW_3RD_STATE_FOR_USER
wxCHK_UNCHECKED = wx._controls.CHK_UNCHECKED
wxCHK_CHECKED = wx._controls.CHK_CHECKED
wxCHK_UNDETERMINED = wx._controls.CHK_UNDETERMINED
wxCheckBox = wx._controls.CheckBox
wxCheckBoxPtr = wx._controls.CheckBoxPtr
wxPreCheckBox = wx._controls.PreCheckBox
wxCheckBox_GetClassDefaultAttributes = wx._controls.CheckBox_GetClassDefaultAttributes
wxChoiceNameStr = wx._controls.ChoiceNameStr
wxChoice = wx._controls.Choice
wxChoicePtr = wx._controls.ChoicePtr
wxPreChoice = wx._controls.PreChoice
wxChoice_GetClassDefaultAttributes = wx._controls.Choice_GetClassDefaultAttributes
wxComboBoxNameStr = wx._controls.ComboBoxNameStr
wxComboBox = wx._controls.ComboBox
wxComboBoxPtr = wx._controls.ComboBoxPtr
wxPreComboBox = wx._controls.PreComboBox
wxComboBox_GetClassDefaultAttributes = wx._controls.ComboBox_GetClassDefaultAttributes
wxGaugeNameStr = wx._controls.GaugeNameStr
wxGA_HORIZONTAL = wx._controls.GA_HORIZONTAL
wxGA_VERTICAL = wx._controls.GA_VERTICAL
wxGA_SMOOTH = wx._controls.GA_SMOOTH
wxGA_PROGRESSBAR = wx._controls.GA_PROGRESSBAR
wxGauge = wx._controls.Gauge
wxGaugePtr = wx._controls.GaugePtr
wxPreGauge = wx._controls.PreGauge
wxGauge_GetClassDefaultAttributes = wx._controls.Gauge_GetClassDefaultAttributes
wxStaticBitmapNameStr = wx._controls.StaticBitmapNameStr
wxStaticBoxNameStr = wx._controls.StaticBoxNameStr
wxStaticTextNameStr = wx._controls.StaticTextNameStr
wxStaticBox = wx._controls.StaticBox
wxStaticBoxPtr = wx._controls.StaticBoxPtr
wxPreStaticBox = wx._controls.PreStaticBox
wxStaticBox_GetClassDefaultAttributes = wx._controls.StaticBox_GetClassDefaultAttributes
wxStaticLine = wx._controls.StaticLine
wxStaticLinePtr = wx._controls.StaticLinePtr
wxPreStaticLine = wx._controls.PreStaticLine
wxStaticLine_GetDefaultSize = wx._controls.StaticLine_GetDefaultSize
wxStaticLine_GetClassDefaultAttributes = wx._controls.StaticLine_GetClassDefaultAttributes
wxStaticText = wx._controls.StaticText
wxStaticTextPtr = wx._controls.StaticTextPtr
wxPreStaticText = wx._controls.PreStaticText
wxStaticText_GetClassDefaultAttributes = wx._controls.StaticText_GetClassDefaultAttributes
wxStaticBitmap = wx._controls.StaticBitmap
wxStaticBitmapPtr = wx._controls.StaticBitmapPtr
wxPreStaticBitmap = wx._controls.PreStaticBitmap
wxStaticBitmap_GetClassDefaultAttributes = wx._controls.StaticBitmap_GetClassDefaultAttributes
wxListBoxNameStr = wx._controls.ListBoxNameStr
wxListBox = wx._controls.ListBox
wxListBoxPtr = wx._controls.ListBoxPtr
wxPreListBox = wx._controls.PreListBox
wxListBox_GetClassDefaultAttributes = wx._controls.ListBox_GetClassDefaultAttributes
wxCheckListBox = wx._controls.CheckListBox
wxCheckListBoxPtr = wx._controls.CheckListBoxPtr
wxPreCheckListBox = wx._controls.PreCheckListBox
wxTextCtrlNameStr = wx._controls.TextCtrlNameStr
wxTE_NO_VSCROLL = wx._controls.TE_NO_VSCROLL
wxTE_AUTO_SCROLL = wx._controls.TE_AUTO_SCROLL
wxTE_READONLY = wx._controls.TE_READONLY
wxTE_MULTILINE = wx._controls.TE_MULTILINE
wxTE_PROCESS_TAB = wx._controls.TE_PROCESS_TAB
wxTE_LEFT = wx._controls.TE_LEFT
wxTE_CENTER = wx._controls.TE_CENTER
wxTE_RIGHT = wx._controls.TE_RIGHT
wxTE_CENTRE = wx._controls.TE_CENTRE
wxTE_RICH = wx._controls.TE_RICH
wxTE_PROCESS_ENTER = wx._controls.TE_PROCESS_ENTER
wxTE_PASSWORD = wx._controls.TE_PASSWORD
wxTE_AUTO_URL = wx._controls.TE_AUTO_URL
wxTE_NOHIDESEL = wx._controls.TE_NOHIDESEL
wxTE_DONTWRAP = wx._controls.TE_DONTWRAP
wxTE_CHARWRAP = wx._controls.TE_CHARWRAP
wxTE_WORDWRAP = wx._controls.TE_WORDWRAP
wxTE_BESTWRAP = wx._controls.TE_BESTWRAP
wxTE_LINEWRAP = wx._controls.TE_LINEWRAP
wxTE_RICH2 = wx._controls.TE_RICH2
wxTE_CAPITALIZE = wx._controls.TE_CAPITALIZE
wxTEXT_ALIGNMENT_DEFAULT = wx._controls.TEXT_ALIGNMENT_DEFAULT
wxTEXT_ALIGNMENT_LEFT = wx._controls.TEXT_ALIGNMENT_LEFT
wxTEXT_ALIGNMENT_CENTRE = wx._controls.TEXT_ALIGNMENT_CENTRE
wxTEXT_ALIGNMENT_CENTER = wx._controls.TEXT_ALIGNMENT_CENTER
wxTEXT_ALIGNMENT_RIGHT = wx._controls.TEXT_ALIGNMENT_RIGHT
wxTEXT_ALIGNMENT_JUSTIFIED = wx._controls.TEXT_ALIGNMENT_JUSTIFIED
wxTEXT_ATTR_TEXT_COLOUR = wx._controls.TEXT_ATTR_TEXT_COLOUR
wxTEXT_ATTR_BACKGROUND_COLOUR = wx._controls.TEXT_ATTR_BACKGROUND_COLOUR
wxTEXT_ATTR_FONT_FACE = wx._controls.TEXT_ATTR_FONT_FACE
wxTEXT_ATTR_FONT_SIZE = wx._controls.TEXT_ATTR_FONT_SIZE
wxTEXT_ATTR_FONT_WEIGHT = wx._controls.TEXT_ATTR_FONT_WEIGHT
wxTEXT_ATTR_FONT_ITALIC = wx._controls.TEXT_ATTR_FONT_ITALIC
wxTEXT_ATTR_FONT_UNDERLINE = wx._controls.TEXT_ATTR_FONT_UNDERLINE
wxTEXT_ATTR_FONT = wx._controls.TEXT_ATTR_FONT
wxTEXT_ATTR_ALIGNMENT = wx._controls.TEXT_ATTR_ALIGNMENT
wxTEXT_ATTR_LEFT_INDENT = wx._controls.TEXT_ATTR_LEFT_INDENT
wxTEXT_ATTR_RIGHT_INDENT = wx._controls.TEXT_ATTR_RIGHT_INDENT
wxTEXT_ATTR_TABS = wx._controls.TEXT_ATTR_TABS
wxTE_HT_UNKNOWN = wx._controls.TE_HT_UNKNOWN
wxTE_HT_BEFORE = wx._controls.TE_HT_BEFORE
wxTE_HT_ON_TEXT = wx._controls.TE_HT_ON_TEXT
wxTE_HT_BELOW = wx._controls.TE_HT_BELOW
wxTE_HT_BEYOND = wx._controls.TE_HT_BEYOND
wxOutOfRangeTextCoord = wx._controls.OutOfRangeTextCoord
wxInvalidTextCoord = wx._controls.InvalidTextCoord
wxTextAttr = wx._controls.TextAttr
wxTextAttrPtr = wx._controls.TextAttrPtr
wxTextAttr_Combine = wx._controls.TextAttr_Combine
wxTextCtrl = wx._controls.TextCtrl
wxTextCtrlPtr = wx._controls.TextCtrlPtr
wxPreTextCtrl = wx._controls.PreTextCtrl
wxTextCtrl_GetClassDefaultAttributes = wx._controls.TextCtrl_GetClassDefaultAttributes
wxEVT_COMMAND_TEXT_UPDATED = wx._controls.wxEVT_COMMAND_TEXT_UPDATED
wxEVT_COMMAND_TEXT_ENTER = wx._controls.wxEVT_COMMAND_TEXT_ENTER
wxEVT_COMMAND_TEXT_URL = wx._controls.wxEVT_COMMAND_TEXT_URL
wxEVT_COMMAND_TEXT_MAXLEN = wx._controls.wxEVT_COMMAND_TEXT_MAXLEN
wxTextUrlEvent = wx._controls.TextUrlEvent
wxTextUrlEventPtr = wx._controls.TextUrlEventPtr
wxScrollBarNameStr = wx._controls.ScrollBarNameStr
wxScrollBar = wx._controls.ScrollBar
wxScrollBarPtr = wx._controls.ScrollBarPtr
wxPreScrollBar = wx._controls.PreScrollBar
wxScrollBar_GetClassDefaultAttributes = wx._controls.ScrollBar_GetClassDefaultAttributes
wxSPIN_BUTTON_NAME = wx._controls.SPIN_BUTTON_NAME
wxSpinCtrlNameStr = wx._controls.SpinCtrlNameStr
wxSP_HORIZONTAL = wx._controls.SP_HORIZONTAL
wxSP_VERTICAL = wx._controls.SP_VERTICAL
wxSP_ARROW_KEYS = wx._controls.SP_ARROW_KEYS
wxSP_WRAP = wx._controls.SP_WRAP
wxSpinButton = wx._controls.SpinButton
wxSpinButtonPtr = wx._controls.SpinButtonPtr
wxPreSpinButton = wx._controls.PreSpinButton
wxSpinButton_GetClassDefaultAttributes = wx._controls.SpinButton_GetClassDefaultAttributes
wxSpinCtrl = wx._controls.SpinCtrl
wxSpinCtrlPtr = wx._controls.SpinCtrlPtr
wxPreSpinCtrl = wx._controls.PreSpinCtrl
wxSpinCtrl_GetClassDefaultAttributes = wx._controls.SpinCtrl_GetClassDefaultAttributes
wxSpinEvent = wx._controls.SpinEvent
wxSpinEventPtr = wx._controls.SpinEventPtr
wxEVT_COMMAND_SPINCTRL_UPDATED = wx._controls.wxEVT_COMMAND_SPINCTRL_UPDATED
wxRadioBoxNameStr = wx._controls.RadioBoxNameStr
wxRadioButtonNameStr = wx._controls.RadioButtonNameStr
wxRadioBox = wx._controls.RadioBox
wxRadioBoxPtr = wx._controls.RadioBoxPtr
wxPreRadioBox = wx._controls.PreRadioBox
wxRadioBox_GetClassDefaultAttributes = wx._controls.RadioBox_GetClassDefaultAttributes
wxRadioButton = wx._controls.RadioButton
wxRadioButtonPtr = wx._controls.RadioButtonPtr
wxPreRadioButton = wx._controls.PreRadioButton
wxRadioButton_GetClassDefaultAttributes = wx._controls.RadioButton_GetClassDefaultAttributes
wxSliderNameStr = wx._controls.SliderNameStr
wxSL_HORIZONTAL = wx._controls.SL_HORIZONTAL
wxSL_VERTICAL = wx._controls.SL_VERTICAL
wxSL_TICKS = wx._controls.SL_TICKS
wxSL_AUTOTICKS = wx._controls.SL_AUTOTICKS
wxSL_LABELS = wx._controls.SL_LABELS
wxSL_LEFT = wx._controls.SL_LEFT
wxSL_TOP = wx._controls.SL_TOP
wxSL_RIGHT = wx._controls.SL_RIGHT
wxSL_BOTTOM = wx._controls.SL_BOTTOM
wxSL_BOTH = wx._controls.SL_BOTH
wxSL_SELRANGE = wx._controls.SL_SELRANGE
wxSL_INVERSE = wx._controls.SL_INVERSE
wxSlider = wx._controls.Slider
wxSliderPtr = wx._controls.SliderPtr
wxPreSlider = wx._controls.PreSlider
wxSlider_GetClassDefaultAttributes = wx._controls.Slider_GetClassDefaultAttributes
wxToggleButtonNameStr = wx._controls.ToggleButtonNameStr
wxEVT_COMMAND_TOGGLEBUTTON_CLICKED = wx._controls.wxEVT_COMMAND_TOGGLEBUTTON_CLICKED
wxToggleButton = wx._controls.ToggleButton
wxToggleButtonPtr = wx._controls.ToggleButtonPtr
wxPreToggleButton = wx._controls.PreToggleButton
wxToggleButton_GetClassDefaultAttributes = wx._controls.ToggleButton_GetClassDefaultAttributes
wxNotebookNameStr = wx._controls.NotebookNameStr
wxBookCtrlBase = wx._controls.BookCtrlBase
wxBookCtrlBasePtr = wx._controls.BookCtrlBasePtr
wxBookCtrlBase_GetClassDefaultAttributes = wx._controls.BookCtrlBase_GetClassDefaultAttributes
wxBookCtrlBaseEvent = wx._controls.BookCtrlBaseEvent
wxBookCtrlBaseEventPtr = wx._controls.BookCtrlBaseEventPtr
wxNB_FIXEDWIDTH = wx._controls.NB_FIXEDWIDTH
wxNB_TOP = wx._controls.NB_TOP
wxNB_LEFT = wx._controls.NB_LEFT
wxNB_RIGHT = wx._controls.NB_RIGHT
wxNB_BOTTOM = wx._controls.NB_BOTTOM
wxNB_MULTILINE = wx._controls.NB_MULTILINE
wxNB_NOPAGETHEME = wx._controls.NB_NOPAGETHEME
wxNB_HITTEST_NOWHERE = wx._controls.NB_HITTEST_NOWHERE
wxNB_HITTEST_ONICON = wx._controls.NB_HITTEST_ONICON
wxNB_HITTEST_ONLABEL = wx._controls.NB_HITTEST_ONLABEL
wxNB_HITTEST_ONITEM = wx._controls.NB_HITTEST_ONITEM
wxNotebook = wx._controls.Notebook
wxNotebookPtr = wx._controls.NotebookPtr
wxPreNotebook = wx._controls.PreNotebook
wxNotebook_GetClassDefaultAttributes = wx._controls.Notebook_GetClassDefaultAttributes
wxNotebookEvent = wx._controls.NotebookEvent
wxNotebookEventPtr = wx._controls.NotebookEventPtr
wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED = wx._controls.wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED
wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGING = wx._controls.wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGING
wxLB_DEFAULT = wx._controls.LB_DEFAULT
wxLB_TOP = wx._controls.LB_TOP
wxLB_BOTTOM = wx._controls.LB_BOTTOM
wxLB_LEFT = wx._controls.LB_LEFT
wxLB_RIGHT = wx._controls.LB_RIGHT
wxLB_ALIGN_MASK = wx._controls.LB_ALIGN_MASK
wxListbook = wx._controls.Listbook
wxListbookPtr = wx._controls.ListbookPtr
wxPreListbook = wx._controls.PreListbook
wxListbookEvent = wx._controls.ListbookEvent
wxListbookEventPtr = wx._controls.ListbookEventPtr
wxEVT_COMMAND_LISTBOOK_PAGE_CHANGED = wx._controls.wxEVT_COMMAND_LISTBOOK_PAGE_CHANGED
wxEVT_COMMAND_LISTBOOK_PAGE_CHANGING = wx._controls.wxEVT_COMMAND_LISTBOOK_PAGE_CHANGING
wxCHB_DEFAULT = wx._controls.CHB_DEFAULT
wxCHB_TOP = wx._controls.CHB_TOP
wxCHB_BOTTOM = wx._controls.CHB_BOTTOM
wxCHB_LEFT = wx._controls.CHB_LEFT
wxCHB_RIGHT = wx._controls.CHB_RIGHT
wxCHB_ALIGN_MASK = wx._controls.CHB_ALIGN_MASK
wxChoicebook = wx._controls.Choicebook
wxChoicebookPtr = wx._controls.ChoicebookPtr
wxPreChoicebook = wx._controls.PreChoicebook
wxChoicebookEvent = wx._controls.ChoicebookEvent
wxChoicebookEventPtr = wx._controls.ChoicebookEventPtr
wxEVT_COMMAND_CHOICEBOOK_PAGE_CHANGED = wx._controls.wxEVT_COMMAND_CHOICEBOOK_PAGE_CHANGED
wxEVT_COMMAND_CHOICEBOOK_PAGE_CHANGING = wx._controls.wxEVT_COMMAND_CHOICEBOOK_PAGE_CHANGING
wxBookCtrlSizer = wx._controls.BookCtrlSizer
wxBookCtrlSizerPtr = wx._controls.BookCtrlSizerPtr
wxNotebookSizer = wx._controls.NotebookSizer
wxNotebookSizerPtr = wx._controls.NotebookSizerPtr
wxTOOL_STYLE_BUTTON = wx._controls.TOOL_STYLE_BUTTON
wxTOOL_STYLE_SEPARATOR = wx._controls.TOOL_STYLE_SEPARATOR
wxTOOL_STYLE_CONTROL = wx._controls.TOOL_STYLE_CONTROL
wxTB_HORIZONTAL = wx._controls.TB_HORIZONTAL
wxTB_VERTICAL = wx._controls.TB_VERTICAL
wxTB_3DBUTTONS = wx._controls.TB_3DBUTTONS
wxTB_FLAT = wx._controls.TB_FLAT
wxTB_DOCKABLE = wx._controls.TB_DOCKABLE
wxTB_NOICONS = wx._controls.TB_NOICONS
wxTB_TEXT = wx._controls.TB_TEXT
wxTB_NODIVIDER = wx._controls.TB_NODIVIDER
wxTB_NOALIGN = wx._controls.TB_NOALIGN
wxTB_HORZ_LAYOUT = wx._controls.TB_HORZ_LAYOUT
wxTB_HORZ_TEXT = wx._controls.TB_HORZ_TEXT
wxToolBarToolBase = wx._controls.ToolBarToolBase
wxToolBarToolBasePtr = wx._controls.ToolBarToolBasePtr
wxToolBarBase = wx._controls.ToolBarBase
wxToolBarBasePtr = wx._controls.ToolBarBasePtr
wxToolBar = wx._controls.ToolBar
wxToolBarPtr = wx._controls.ToolBarPtr
wxPreToolBar = wx._controls.PreToolBar
wxToolBar_GetClassDefaultAttributes = wx._controls.ToolBar_GetClassDefaultAttributes
wxListCtrlNameStr = wx._controls.ListCtrlNameStr
wxLC_VRULES = wx._controls.LC_VRULES
wxLC_HRULES = wx._controls.LC_HRULES
wxLC_ICON = wx._controls.LC_ICON
wxLC_SMALL_ICON = wx._controls.LC_SMALL_ICON
wxLC_LIST = wx._controls.LC_LIST
wxLC_REPORT = wx._controls.LC_REPORT
wxLC_ALIGN_TOP = wx._controls.LC_ALIGN_TOP
wxLC_ALIGN_LEFT = wx._controls.LC_ALIGN_LEFT
wxLC_AUTOARRANGE = wx._controls.LC_AUTOARRANGE
wxLC_VIRTUAL = wx._controls.LC_VIRTUAL
wxLC_EDIT_LABELS = wx._controls.LC_EDIT_LABELS
wxLC_NO_HEADER = wx._controls.LC_NO_HEADER
wxLC_NO_SORT_HEADER = wx._controls.LC_NO_SORT_HEADER
wxLC_SINGLE_SEL = wx._controls.LC_SINGLE_SEL
wxLC_SORT_ASCENDING = wx._controls.LC_SORT_ASCENDING
wxLC_SORT_DESCENDING = wx._controls.LC_SORT_DESCENDING
wxLC_MASK_TYPE = wx._controls.LC_MASK_TYPE
wxLC_MASK_ALIGN = wx._controls.LC_MASK_ALIGN
wxLC_MASK_SORT = wx._controls.LC_MASK_SORT
wxLIST_MASK_STATE = wx._controls.LIST_MASK_STATE
wxLIST_MASK_TEXT = wx._controls.LIST_MASK_TEXT
wxLIST_MASK_IMAGE = wx._controls.LIST_MASK_IMAGE
wxLIST_MASK_DATA = wx._controls.LIST_MASK_DATA
wxLIST_SET_ITEM = wx._controls.LIST_SET_ITEM
wxLIST_MASK_WIDTH = wx._controls.LIST_MASK_WIDTH
wxLIST_MASK_FORMAT = wx._controls.LIST_MASK_FORMAT
wxLIST_STATE_DONTCARE = wx._controls.LIST_STATE_DONTCARE
wxLIST_STATE_DROPHILITED = wx._controls.LIST_STATE_DROPHILITED
wxLIST_STATE_FOCUSED = wx._controls.LIST_STATE_FOCUSED
wxLIST_STATE_SELECTED = wx._controls.LIST_STATE_SELECTED
wxLIST_STATE_CUT = wx._controls.LIST_STATE_CUT
wxLIST_STATE_DISABLED = wx._controls.LIST_STATE_DISABLED
wxLIST_STATE_FILTERED = wx._controls.LIST_STATE_FILTERED
wxLIST_STATE_INUSE = wx._controls.LIST_STATE_INUSE
wxLIST_STATE_PICKED = wx._controls.LIST_STATE_PICKED
wxLIST_STATE_SOURCE = wx._controls.LIST_STATE_SOURCE
wxLIST_HITTEST_ABOVE = wx._controls.LIST_HITTEST_ABOVE
wxLIST_HITTEST_BELOW = wx._controls.LIST_HITTEST_BELOW
wxLIST_HITTEST_NOWHERE = wx._controls.LIST_HITTEST_NOWHERE
wxLIST_HITTEST_ONITEMICON = wx._controls.LIST_HITTEST_ONITEMICON
wxLIST_HITTEST_ONITEMLABEL = wx._controls.LIST_HITTEST_ONITEMLABEL
wxLIST_HITTEST_ONITEMRIGHT = wx._controls.LIST_HITTEST_ONITEMRIGHT
wxLIST_HITTEST_ONITEMSTATEICON = wx._controls.LIST_HITTEST_ONITEMSTATEICON
wxLIST_HITTEST_TOLEFT = wx._controls.LIST_HITTEST_TOLEFT
wxLIST_HITTEST_TORIGHT = wx._controls.LIST_HITTEST_TORIGHT
wxLIST_HITTEST_ONITEM = wx._controls.LIST_HITTEST_ONITEM
wxLIST_NEXT_ABOVE = wx._controls.LIST_NEXT_ABOVE
wxLIST_NEXT_ALL = wx._controls.LIST_NEXT_ALL
wxLIST_NEXT_BELOW = wx._controls.LIST_NEXT_BELOW
wxLIST_NEXT_LEFT = wx._controls.LIST_NEXT_LEFT
wxLIST_NEXT_RIGHT = wx._controls.LIST_NEXT_RIGHT
wxLIST_ALIGN_DEFAULT = wx._controls.LIST_ALIGN_DEFAULT
wxLIST_ALIGN_LEFT = wx._controls.LIST_ALIGN_LEFT
wxLIST_ALIGN_TOP = wx._controls.LIST_ALIGN_TOP
wxLIST_ALIGN_SNAP_TO_GRID = wx._controls.LIST_ALIGN_SNAP_TO_GRID
wxLIST_FORMAT_LEFT = wx._controls.LIST_FORMAT_LEFT
wxLIST_FORMAT_RIGHT = wx._controls.LIST_FORMAT_RIGHT
wxLIST_FORMAT_CENTRE = wx._controls.LIST_FORMAT_CENTRE
wxLIST_FORMAT_CENTER = wx._controls.LIST_FORMAT_CENTER
wxLIST_AUTOSIZE = wx._controls.LIST_AUTOSIZE
wxLIST_AUTOSIZE_USEHEADER = wx._controls.LIST_AUTOSIZE_USEHEADER
wxLIST_RECT_BOUNDS = wx._controls.LIST_RECT_BOUNDS
wxLIST_RECT_ICON = wx._controls.LIST_RECT_ICON
wxLIST_RECT_LABEL = wx._controls.LIST_RECT_LABEL
wxLIST_FIND_UP = wx._controls.LIST_FIND_UP
wxLIST_FIND_DOWN = wx._controls.LIST_FIND_DOWN
wxLIST_FIND_LEFT = wx._controls.LIST_FIND_LEFT
wxLIST_FIND_RIGHT = wx._controls.LIST_FIND_RIGHT
wxListItemAttr = wx._controls.ListItemAttr
wxListItemAttrPtr = wx._controls.ListItemAttrPtr
wxListItem = wx._controls.ListItem
wxListItemPtr = wx._controls.ListItemPtr
wxListEvent = wx._controls.ListEvent
wxListEventPtr = wx._controls.ListEventPtr
wxEVT_COMMAND_LIST_BEGIN_DRAG = wx._controls.wxEVT_COMMAND_LIST_BEGIN_DRAG
wxEVT_COMMAND_LIST_BEGIN_RDRAG = wx._controls.wxEVT_COMMAND_LIST_BEGIN_RDRAG
wxEVT_COMMAND_LIST_BEGIN_LABEL_EDIT = wx._controls.wxEVT_COMMAND_LIST_BEGIN_LABEL_EDIT
wxEVT_COMMAND_LIST_END_LABEL_EDIT = wx._controls.wxEVT_COMMAND_LIST_END_LABEL_EDIT
wxEVT_COMMAND_LIST_DELETE_ITEM = wx._controls.wxEVT_COMMAND_LIST_DELETE_ITEM
wxEVT_COMMAND_LIST_DELETE_ALL_ITEMS = wx._controls.wxEVT_COMMAND_LIST_DELETE_ALL_ITEMS
wxEVT_COMMAND_LIST_ITEM_SELECTED = wx._controls.wxEVT_COMMAND_LIST_ITEM_SELECTED
wxEVT_COMMAND_LIST_ITEM_DESELECTED = wx._controls.wxEVT_COMMAND_LIST_ITEM_DESELECTED
wxEVT_COMMAND_LIST_KEY_DOWN = wx._controls.wxEVT_COMMAND_LIST_KEY_DOWN
wxEVT_COMMAND_LIST_INSERT_ITEM = wx._controls.wxEVT_COMMAND_LIST_INSERT_ITEM
wxEVT_COMMAND_LIST_COL_CLICK = wx._controls.wxEVT_COMMAND_LIST_COL_CLICK
wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK = wx._controls.wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK
wxEVT_COMMAND_LIST_ITEM_MIDDLE_CLICK = wx._controls.wxEVT_COMMAND_LIST_ITEM_MIDDLE_CLICK
wxEVT_COMMAND_LIST_ITEM_ACTIVATED = wx._controls.wxEVT_COMMAND_LIST_ITEM_ACTIVATED
wxEVT_COMMAND_LIST_CACHE_HINT = wx._controls.wxEVT_COMMAND_LIST_CACHE_HINT
wxEVT_COMMAND_LIST_COL_RIGHT_CLICK = wx._controls.wxEVT_COMMAND_LIST_COL_RIGHT_CLICK
wxEVT_COMMAND_LIST_COL_BEGIN_DRAG = wx._controls.wxEVT_COMMAND_LIST_COL_BEGIN_DRAG
wxEVT_COMMAND_LIST_COL_DRAGGING = wx._controls.wxEVT_COMMAND_LIST_COL_DRAGGING
wxEVT_COMMAND_LIST_COL_END_DRAG = wx._controls.wxEVT_COMMAND_LIST_COL_END_DRAG
wxEVT_COMMAND_LIST_ITEM_FOCUSED = wx._controls.wxEVT_COMMAND_LIST_ITEM_FOCUSED
wxEVT_COMMAND_LIST_GET_INFO = wx._controls.wxEVT_COMMAND_LIST_GET_INFO
wxEVT_COMMAND_LIST_SET_INFO = wx._controls.wxEVT_COMMAND_LIST_SET_INFO
wxListCtrl = wx._controls.ListCtrl
wxListCtrlPtr = wx._controls.ListCtrlPtr
wxListCtrl = wx._controls.ListCtrl
wxPreListCtrl = wx._controls.PreListCtrl
wxListCtrl_GetClassDefaultAttributes = wx._controls.ListCtrl_GetClassDefaultAttributes
wxListView = wx._controls.ListView
wxListViewPtr = wx._controls.ListViewPtr
wxPreListView = wx._controls.PreListView
wxTreeCtrlNameStr = wx._controls.TreeCtrlNameStr
wxTR_NO_BUTTONS = wx._controls.TR_NO_BUTTONS
wxTR_HAS_BUTTONS = wx._controls.TR_HAS_BUTTONS
wxTR_NO_LINES = wx._controls.TR_NO_LINES
wxTR_LINES_AT_ROOT = wx._controls.TR_LINES_AT_ROOT
wxTR_SINGLE = wx._controls.TR_SINGLE
wxTR_MULTIPLE = wx._controls.TR_MULTIPLE
wxTR_EXTENDED = wx._controls.TR_EXTENDED
wxTR_HAS_VARIABLE_ROW_HEIGHT = wx._controls.TR_HAS_VARIABLE_ROW_HEIGHT
wxTR_EDIT_LABELS = wx._controls.TR_EDIT_LABELS
wxTR_HIDE_ROOT = wx._controls.TR_HIDE_ROOT
wxTR_ROW_LINES = wx._controls.TR_ROW_LINES
wxTR_FULL_ROW_HIGHLIGHT = wx._controls.TR_FULL_ROW_HIGHLIGHT
wxTR_DEFAULT_STYLE = wx._controls.TR_DEFAULT_STYLE
wxTR_TWIST_BUTTONS = wx._controls.TR_TWIST_BUTTONS
wxTR_MAC_BUTTONS = wx._controls.TR_MAC_BUTTONS
wxTR_AQUA_BUTTONS = wx._controls.TR_AQUA_BUTTONS
wxTreeItemIcon_Normal = wx._controls.TreeItemIcon_Normal
wxTreeItemIcon_Selected = wx._controls.TreeItemIcon_Selected
wxTreeItemIcon_Expanded = wx._controls.TreeItemIcon_Expanded
wxTreeItemIcon_SelectedExpanded = wx._controls.TreeItemIcon_SelectedExpanded
wxTreeItemIcon_Max = wx._controls.TreeItemIcon_Max
wxTREE_HITTEST_ABOVE = wx._controls.TREE_HITTEST_ABOVE
wxTREE_HITTEST_BELOW = wx._controls.TREE_HITTEST_BELOW
wxTREE_HITTEST_NOWHERE = wx._controls.TREE_HITTEST_NOWHERE
wxTREE_HITTEST_ONITEMBUTTON = wx._controls.TREE_HITTEST_ONITEMBUTTON
wxTREE_HITTEST_ONITEMICON = wx._controls.TREE_HITTEST_ONITEMICON
wxTREE_HITTEST_ONITEMINDENT = wx._controls.TREE_HITTEST_ONITEMINDENT
wxTREE_HITTEST_ONITEMLABEL = wx._controls.TREE_HITTEST_ONITEMLABEL
wxTREE_HITTEST_ONITEMRIGHT = wx._controls.TREE_HITTEST_ONITEMRIGHT
wxTREE_HITTEST_ONITEMSTATEICON = wx._controls.TREE_HITTEST_ONITEMSTATEICON
wxTREE_HITTEST_TOLEFT = wx._controls.TREE_HITTEST_TOLEFT
wxTREE_HITTEST_TORIGHT = wx._controls.TREE_HITTEST_TORIGHT
wxTREE_HITTEST_ONITEMUPPERPART = wx._controls.TREE_HITTEST_ONITEMUPPERPART
wxTREE_HITTEST_ONITEMLOWERPART = wx._controls.TREE_HITTEST_ONITEMLOWERPART
wxTREE_HITTEST_ONITEM = wx._controls.TREE_HITTEST_ONITEM
wxTreeItemId = wx._controls.TreeItemId
wxTreeItemIdPtr = wx._controls.TreeItemIdPtr
wxTreeItemData = wx._controls.TreeItemData
wxTreeItemDataPtr = wx._controls.TreeItemDataPtr
wxTreeItemData = wx._controls.TreeItemData
wxEVT_COMMAND_TREE_BEGIN_DRAG = wx._controls.wxEVT_COMMAND_TREE_BEGIN_DRAG
wxEVT_COMMAND_TREE_BEGIN_RDRAG = wx._controls.wxEVT_COMMAND_TREE_BEGIN_RDRAG
wxEVT_COMMAND_TREE_BEGIN_LABEL_EDIT = wx._controls.wxEVT_COMMAND_TREE_BEGIN_LABEL_EDIT
wxEVT_COMMAND_TREE_END_LABEL_EDIT = wx._controls.wxEVT_COMMAND_TREE_END_LABEL_EDIT
wxEVT_COMMAND_TREE_DELETE_ITEM = wx._controls.wxEVT_COMMAND_TREE_DELETE_ITEM
wxEVT_COMMAND_TREE_GET_INFO = wx._controls.wxEVT_COMMAND_TREE_GET_INFO
wxEVT_COMMAND_TREE_SET_INFO = wx._controls.wxEVT_COMMAND_TREE_SET_INFO
wxEVT_COMMAND_TREE_ITEM_EXPANDED = wx._controls.wxEVT_COMMAND_TREE_ITEM_EXPANDED
wxEVT_COMMAND_TREE_ITEM_EXPANDING = wx._controls.wxEVT_COMMAND_TREE_ITEM_EXPANDING
wxEVT_COMMAND_TREE_ITEM_COLLAPSED = wx._controls.wxEVT_COMMAND_TREE_ITEM_COLLAPSED
wxEVT_COMMAND_TREE_ITEM_COLLAPSING = wx._controls.wxEVT_COMMAND_TREE_ITEM_COLLAPSING
wxEVT_COMMAND_TREE_SEL_CHANGED = wx._controls.wxEVT_COMMAND_TREE_SEL_CHANGED
wxEVT_COMMAND_TREE_SEL_CHANGING = wx._controls.wxEVT_COMMAND_TREE_SEL_CHANGING
wxEVT_COMMAND_TREE_KEY_DOWN = wx._controls.wxEVT_COMMAND_TREE_KEY_DOWN
wxEVT_COMMAND_TREE_ITEM_ACTIVATED = wx._controls.wxEVT_COMMAND_TREE_ITEM_ACTIVATED
wxEVT_COMMAND_TREE_ITEM_RIGHT_CLICK = wx._controls.wxEVT_COMMAND_TREE_ITEM_RIGHT_CLICK
wxEVT_COMMAND_TREE_ITEM_MIDDLE_CLICK = wx._controls.wxEVT_COMMAND_TREE_ITEM_MIDDLE_CLICK
wxEVT_COMMAND_TREE_END_DRAG = wx._controls.wxEVT_COMMAND_TREE_END_DRAG
wxEVT_COMMAND_TREE_STATE_IMAGE_CLICK = wx._controls.wxEVT_COMMAND_TREE_STATE_IMAGE_CLICK
wxEVT_COMMAND_TREE_ITEM_GETTOOLTIP = wx._controls.wxEVT_COMMAND_TREE_ITEM_GETTOOLTIP
wxEVT_COMMAND_TREE_ITEM_MENU = wx._controls.wxEVT_COMMAND_TREE_ITEM_MENU
wxTreeEvent = wx._controls.TreeEvent
wxTreeEventPtr = wx._controls.TreeEventPtr
wxTreeCtrl = wx._controls.TreeCtrl
wxTreeCtrlPtr = wx._controls.TreeCtrlPtr
wxTreeCtrl = wx._controls.TreeCtrl
wxPreTreeCtrl = wx._controls.PreTreeCtrl
wxTreeCtrl_GetClassDefaultAttributes = wx._controls.TreeCtrl_GetClassDefaultAttributes
wxDirDialogDefaultFolderStr = wx._controls.DirDialogDefaultFolderStr
wxDIRCTRL_DIR_ONLY = wx._controls.DIRCTRL_DIR_ONLY
wxDIRCTRL_SELECT_FIRST = wx._controls.DIRCTRL_SELECT_FIRST
wxDIRCTRL_SHOW_FILTERS = wx._controls.DIRCTRL_SHOW_FILTERS
wxDIRCTRL_3D_INTERNAL = wx._controls.DIRCTRL_3D_INTERNAL
wxDIRCTRL_EDIT_LABELS = wx._controls.DIRCTRL_EDIT_LABELS
wxGenericDirCtrl = wx._controls.GenericDirCtrl
wxGenericDirCtrlPtr = wx._controls.GenericDirCtrlPtr
wxPreGenericDirCtrl = wx._controls.PreGenericDirCtrl
wxDirFilterListCtrl = wx._controls.DirFilterListCtrl
wxDirFilterListCtrlPtr = wx._controls.DirFilterListCtrlPtr
wxPreDirFilterListCtrl = wx._controls.PreDirFilterListCtrl
wxPyControl = wx._controls.PyControl
wxPyControlPtr = wx._controls.PyControlPtr
wxPrePyControl = wx._controls.PrePyControl
wxFRAME_EX_CONTEXTHELP = wx._controls.FRAME_EX_CONTEXTHELP
wxDIALOG_EX_CONTEXTHELP = wx._controls.DIALOG_EX_CONTEXTHELP
wxEVT_HELP = wx._controls.wxEVT_HELP
wxEVT_DETAILED_HELP = wx._controls.wxEVT_DETAILED_HELP
wxHelpEvent = wx._controls.HelpEvent
wxHelpEventPtr = wx._controls.HelpEventPtr
wxContextHelp = wx._controls.ContextHelp
wxContextHelpPtr = wx._controls.ContextHelpPtr
wxContextHelpButton = wx._controls.ContextHelpButton
wxContextHelpButtonPtr = wx._controls.ContextHelpButtonPtr
wxHelpProvider = wx._controls.HelpProvider
wxHelpProviderPtr = wx._controls.HelpProviderPtr
wxHelpProvider_Set = wx._controls.HelpProvider_Set
wxHelpProvider_Get = wx._controls.HelpProvider_Get
wxSimpleHelpProvider = wx._controls.SimpleHelpProvider
wxSimpleHelpProviderPtr = wx._controls.SimpleHelpProviderPtr
wxDragImage = wx._controls.DragImage
wxDragImagePtr = wx._controls.DragImagePtr
wxDragImage = wx._controls.DragImage
wxDragIcon = wx._controls.DragIcon
wxDragString = wx._controls.DragString
wxDragTreeItem = wx._controls.DragTreeItem
wxDragListItem = wx._controls.DragListItem
wxDatePickerCtrlNameStr = wx._controls.DatePickerCtrlNameStr
wxDP_DEFAULT = wx._controls.DP_DEFAULT
wxDP_SPIN = wx._controls.DP_SPIN
wxDP_DROPDOWN = wx._controls.DP_DROPDOWN
wxDP_SHOWCENTURY = wx._controls.DP_SHOWCENTURY
wxDP_ALLOWNONE = wx._controls.DP_ALLOWNONE
wxDatePickerCtrl = wx._controls.DatePickerCtrl
wxDatePickerCtrlPtr = wx._controls.DatePickerCtrlPtr
wxPreDatePickerCtrl = wx._controls.PreDatePickerCtrl


d = globals()
for k, v in wx._controls.__dict__.iteritems():
    if k.startswith('EVT'):
        d[k] = v
del d, k, v



