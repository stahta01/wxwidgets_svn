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

import wx.grid

sys.modules['wxPython.wx'] = _wx
del sys, _wx


# Now assign all the reverse-renamed names:
wxDateTimeFormatStr = wx.grid.DateTimeFormatStr
wxGRID_VALUE_STRING = wx.grid.GRID_VALUE_STRING
wxGRID_VALUE_BOOL = wx.grid.GRID_VALUE_BOOL
wxGRID_VALUE_NUMBER = wx.grid.GRID_VALUE_NUMBER
wxGRID_VALUE_FLOAT = wx.grid.GRID_VALUE_FLOAT
wxGRID_VALUE_CHOICE = wx.grid.GRID_VALUE_CHOICE
wxGRID_VALUE_TEXT = wx.grid.GRID_VALUE_TEXT
wxGRID_VALUE_LONG = wx.grid.GRID_VALUE_LONG
wxGRID_VALUE_CHOICEINT = wx.grid.GRID_VALUE_CHOICEINT
wxGRID_VALUE_DATETIME = wx.grid.GRID_VALUE_DATETIME
wxGridNoCellCoords = wx.grid.GridNoCellCoords
wxGridNoCellRect = wx.grid.GridNoCellRect
wxGRID_DEFAULT_NUMBER_ROWS = wx.grid.GRID_DEFAULT_NUMBER_ROWS
wxGRID_DEFAULT_NUMBER_COLS = wx.grid.GRID_DEFAULT_NUMBER_COLS
wxGRID_DEFAULT_ROW_HEIGHT = wx.grid.GRID_DEFAULT_ROW_HEIGHT
wxGRID_DEFAULT_COL_WIDTH = wx.grid.GRID_DEFAULT_COL_WIDTH
wxGRID_DEFAULT_COL_LABEL_HEIGHT = wx.grid.GRID_DEFAULT_COL_LABEL_HEIGHT
wxGRID_DEFAULT_ROW_LABEL_WIDTH = wx.grid.GRID_DEFAULT_ROW_LABEL_WIDTH
wxGRID_LABEL_EDGE_ZONE = wx.grid.GRID_LABEL_EDGE_ZONE
wxGRID_MIN_ROW_HEIGHT = wx.grid.GRID_MIN_ROW_HEIGHT
wxGRID_MIN_COL_WIDTH = wx.grid.GRID_MIN_COL_WIDTH
wxGRID_DEFAULT_SCROLLBAR_WIDTH = wx.grid.GRID_DEFAULT_SCROLLBAR_WIDTH
wxGridCellRenderer = wx.grid.GridCellRenderer
wxGridCellRendererPtr = wx.grid.GridCellRendererPtr
wxPyGridCellRenderer = wx.grid.PyGridCellRenderer
wxPyGridCellRendererPtr = wx.grid.PyGridCellRendererPtr
wxGridCellStringRenderer = wx.grid.GridCellStringRenderer
wxGridCellStringRendererPtr = wx.grid.GridCellStringRendererPtr
wxGridCellNumberRenderer = wx.grid.GridCellNumberRenderer
wxGridCellNumberRendererPtr = wx.grid.GridCellNumberRendererPtr
wxGridCellFloatRenderer = wx.grid.GridCellFloatRenderer
wxGridCellFloatRendererPtr = wx.grid.GridCellFloatRendererPtr
wxGridCellBoolRenderer = wx.grid.GridCellBoolRenderer
wxGridCellBoolRendererPtr = wx.grid.GridCellBoolRendererPtr
wxGridCellDateTimeRenderer = wx.grid.GridCellDateTimeRenderer
wxGridCellDateTimeRendererPtr = wx.grid.GridCellDateTimeRendererPtr
wxGridCellEnumRenderer = wx.grid.GridCellEnumRenderer
wxGridCellEnumRendererPtr = wx.grid.GridCellEnumRendererPtr
wxGridCellAutoWrapStringRenderer = wx.grid.GridCellAutoWrapStringRenderer
wxGridCellAutoWrapStringRendererPtr = wx.grid.GridCellAutoWrapStringRendererPtr
wxGridCellEditor = wx.grid.GridCellEditor
wxGridCellEditorPtr = wx.grid.GridCellEditorPtr
wxPyGridCellEditor = wx.grid.PyGridCellEditor
wxPyGridCellEditorPtr = wx.grid.PyGridCellEditorPtr
wxGridCellTextEditor = wx.grid.GridCellTextEditor
wxGridCellTextEditorPtr = wx.grid.GridCellTextEditorPtr
wxGridCellNumberEditor = wx.grid.GridCellNumberEditor
wxGridCellNumberEditorPtr = wx.grid.GridCellNumberEditorPtr
wxGridCellFloatEditor = wx.grid.GridCellFloatEditor
wxGridCellFloatEditorPtr = wx.grid.GridCellFloatEditorPtr
wxGridCellBoolEditor = wx.grid.GridCellBoolEditor
wxGridCellBoolEditorPtr = wx.grid.GridCellBoolEditorPtr
wxGridCellChoiceEditor = wx.grid.GridCellChoiceEditor
wxGridCellChoiceEditorPtr = wx.grid.GridCellChoiceEditorPtr
wxGridCellEnumEditor = wx.grid.GridCellEnumEditor
wxGridCellEnumEditorPtr = wx.grid.GridCellEnumEditorPtr
wxGridCellAutoWrapStringEditor = wx.grid.GridCellAutoWrapStringEditor
wxGridCellAutoWrapStringEditorPtr = wx.grid.GridCellAutoWrapStringEditorPtr
wxGridCellAttr = wx.grid.GridCellAttr
wxGridCellAttrPtr = wx.grid.GridCellAttrPtr
wxGridCellAttrProvider = wx.grid.GridCellAttrProvider
wxGridCellAttrProviderPtr = wx.grid.GridCellAttrProviderPtr
wxPyGridCellAttrProvider = wx.grid.PyGridCellAttrProvider
wxPyGridCellAttrProviderPtr = wx.grid.PyGridCellAttrProviderPtr
wxGridTableBase = wx.grid.GridTableBase
wxGridTableBasePtr = wx.grid.GridTableBasePtr
wxPyGridTableBase = wx.grid.PyGridTableBase
wxPyGridTableBasePtr = wx.grid.PyGridTableBasePtr
wxGridStringTable = wx.grid.GridStringTable
wxGridStringTablePtr = wx.grid.GridStringTablePtr
wxGRIDTABLE_REQUEST_VIEW_GET_VALUES = wx.grid.GRIDTABLE_REQUEST_VIEW_GET_VALUES
wxGRIDTABLE_REQUEST_VIEW_SEND_VALUES = wx.grid.GRIDTABLE_REQUEST_VIEW_SEND_VALUES
wxGRIDTABLE_NOTIFY_ROWS_INSERTED = wx.grid.GRIDTABLE_NOTIFY_ROWS_INSERTED
wxGRIDTABLE_NOTIFY_ROWS_APPENDED = wx.grid.GRIDTABLE_NOTIFY_ROWS_APPENDED
wxGRIDTABLE_NOTIFY_ROWS_DELETED = wx.grid.GRIDTABLE_NOTIFY_ROWS_DELETED
wxGRIDTABLE_NOTIFY_COLS_INSERTED = wx.grid.GRIDTABLE_NOTIFY_COLS_INSERTED
wxGRIDTABLE_NOTIFY_COLS_APPENDED = wx.grid.GRIDTABLE_NOTIFY_COLS_APPENDED
wxGRIDTABLE_NOTIFY_COLS_DELETED = wx.grid.GRIDTABLE_NOTIFY_COLS_DELETED
wxGridTableMessage = wx.grid.GridTableMessage
wxGridTableMessagePtr = wx.grid.GridTableMessagePtr
wxGridCellCoords = wx.grid.GridCellCoords
wxGridCellCoordsPtr = wx.grid.GridCellCoordsPtr
wxGrid = wx.grid.Grid
wxGridPtr = wx.grid.GridPtr
wxPreGrid = wx.grid.PreGrid
wxGrid_GetClassDefaultAttributes = wx.grid.Grid_GetClassDefaultAttributes
wxGridEvent = wx.grid.GridEvent
wxGridEventPtr = wx.grid.GridEventPtr
wxGridSizeEvent = wx.grid.GridSizeEvent
wxGridSizeEventPtr = wx.grid.GridSizeEventPtr
wxGridRangeSelectEvent = wx.grid.GridRangeSelectEvent
wxGridRangeSelectEventPtr = wx.grid.GridRangeSelectEventPtr
wxGridEditorCreatedEvent = wx.grid.GridEditorCreatedEvent
wxGridEditorCreatedEventPtr = wx.grid.GridEditorCreatedEventPtr
wxEVT_GRID_CELL_LEFT_CLICK = wx.grid.wxEVT_GRID_CELL_LEFT_CLICK
wxEVT_GRID_CELL_RIGHT_CLICK = wx.grid.wxEVT_GRID_CELL_RIGHT_CLICK
wxEVT_GRID_CELL_LEFT_DCLICK = wx.grid.wxEVT_GRID_CELL_LEFT_DCLICK
wxEVT_GRID_CELL_RIGHT_DCLICK = wx.grid.wxEVT_GRID_CELL_RIGHT_DCLICK
wxEVT_GRID_LABEL_LEFT_CLICK = wx.grid.wxEVT_GRID_LABEL_LEFT_CLICK
wxEVT_GRID_LABEL_RIGHT_CLICK = wx.grid.wxEVT_GRID_LABEL_RIGHT_CLICK
wxEVT_GRID_LABEL_LEFT_DCLICK = wx.grid.wxEVT_GRID_LABEL_LEFT_DCLICK
wxEVT_GRID_LABEL_RIGHT_DCLICK = wx.grid.wxEVT_GRID_LABEL_RIGHT_DCLICK
wxEVT_GRID_ROW_SIZE = wx.grid.wxEVT_GRID_ROW_SIZE
wxEVT_GRID_COL_SIZE = wx.grid.wxEVT_GRID_COL_SIZE
wxEVT_GRID_RANGE_SELECT = wx.grid.wxEVT_GRID_RANGE_SELECT
wxEVT_GRID_CELL_CHANGE = wx.grid.wxEVT_GRID_CELL_CHANGE
wxEVT_GRID_SELECT_CELL = wx.grid.wxEVT_GRID_SELECT_CELL
wxEVT_GRID_EDITOR_SHOWN = wx.grid.wxEVT_GRID_EDITOR_SHOWN
wxEVT_GRID_EDITOR_HIDDEN = wx.grid.wxEVT_GRID_EDITOR_HIDDEN
wxEVT_GRID_EDITOR_CREATED = wx.grid.wxEVT_GRID_EDITOR_CREATED
wxEVT_GRID_CELL_BEGIN_DRAG = wx.grid.wxEVT_GRID_CELL_BEGIN_DRAG


d = globals()
for k, v in wx.grid.__dict__.iteritems():
    if k.startswith('EVT'):
        d[k] = v
del d, k, v



