# sheet.py
# CSheet - A wxPython spreadsheet class.
# This is free software.  Feel free to adapt it as you like.
# Author: Mark F. Russo (russomf@hotmail.com) 2002/01/31

from wxPython.wx import *
from wxPython.grid import *
from string import *

#---------------------------------------------------------------------------
class CTextCellEditor(wxTextCtrl):
    """ Custom text control for cell editing """
    def __init__(self, parent, id, grid):
        wxTextCtrl.__init__(self, parent, id, "", style=wxNO_BORDER)
        self._grid = grid                           # Save grid reference
        EVT_CHAR(self, self.OnChar)

    def OnChar(self, evt):                          # Hook OnChar for custom behavior
        """Customizes char events """
        key = evt.GetKeyCode()
        if   key == WXK_DOWN:
            self._grid.DisableCellEditControl()     # Commit the edit
            self._grid.MoveCursorDown(false)        # Change the current cell
        elif key == WXK_UP:
            self._grid.DisableCellEditControl()     # Commit the edit
            self._grid.MoveCursorUp(false)          # Change the current cell
        elif key == WXK_LEFT:
            self._grid.DisableCellEditControl()     # Commit the edit
            self._grid.MoveCursorLeft(false)        # Change the current cell
        elif key == WXK_RIGHT:
            self._grid.DisableCellEditControl()     # Commit the edit
            self._grid.MoveCursorRight(false)       # Change the current cell
            
        evt.Skip()                                  # Continue event
        
#---------------------------------------------------------------------------
class CCellEditor(wxPyGridCellEditor):
    """ Custom cell editor """
    def __init__(self, grid):
        wxPyGridCellEditor.__init__(self)
        self._grid = grid                           # Save a reference to the grid
        
    def Create(self, parent, id, evtHandler):
        """ Create the actual edit control.  Must derive from wxControl.
            Must Override
        """
        self._tc = CTextCellEditor(parent, id, self._grid)
        self._tc.SetInsertionPoint(0)
        self.SetControl(self._tc)
        if evtHandler:
            self._tc.PushEventHandler(evtHandler)

    def SetSize(self, rect):
        """ Position/size the edit control within the cell rectangle. """
        # Size text control to exactly overlay in-cell editing
        self._tc.SetDimensions(rect.x+3, rect.y+3, rect.width-2, rect.height-2)

    def Show(self, show, attr):
        """ Show or hide the edit control.  Use the attr (if not None)
            to set colors or fonts for the control.
        """
        self.base_Show(show, attr)

    def PaintBackground(self, rect, attr):
        """ Draws the part of the cell not occupied by the edit control.  The
            base class version just fills it with background colour from the
            attribute.
        """
        # Call base class method.
        self.base_PaintBackground(self, rect, attr)
    
    def BeginEdit(self, row, col, grid):
        """ Fetch the value from the table and prepare edit control to begin editing.
            Set the focus to the edit control.  Must Override.
        """
        self._startValue = grid.GetTable().GetValue(row, col)
        self._tc.SetValue(self._startValue)
        self._tc.SetFocus()
        
        # Select the text when initiating an edit so that subsequent typing
        # replaces the contents.
        self._tc.SetSelection(0, self._tc.GetLastPosition())
        
    def EndEdit(self, row, col, grid):
        """ Commit editing the current cell. Returns true if the value has changed.
            If necessary, the control may be destroyed. Must Override.
        """
        changed = false                             # Assume value not changed
        val = self._tc.GetValue()                   # Get value in edit control
        if val != self._startValue:                 # Compare
            changed = true                          # If different then changed is true
            grid.GetTable().SetValue(row, col, val) # Update the table
        self._startValue = ''                       # Clear the class' start value
        self._tc.SetValue('')                       # Clear contents of the edit control

        return changed

    def Reset(self):
        """ Reset the value in the control back to its starting value. Must Override. """
        self._tc.SetValue(self._startValue)
        self._tc.SetInsertionPointEnd()

    def IsAcceptedKey(self, evt):
        """ Return true to allow the given key to start editing.  The base class
            version only checks that the event has no modifiers.  F2 is special
            and will always start the editor.
        """
        return (not (evt.ControlDown() or evt.AltDown())
                and  evt.GetKeyCode() != WXK_SHIFT)

    def StartingKey(self, evt):
        """ If the editor is enabled by pressing keys on the grid, this will be
            called to let the editor react to that first key.
        """
        key = evt.GetKeyCode()              # Get the key code
        ch = None                           # Handle num pad keys
        if key in [WXK_NUMPAD0, WXK_NUMPAD1, WXK_NUMPAD2, WXK_NUMPAD3, WXK_NUMPAD4,
                   WXK_NUMPAD5, WXK_NUMPAD6, WXK_NUMPAD7, WXK_NUMPAD8, WXK_NUMPAD9]:
            ch = chr(ord('0') + key - WXK_NUMPAD0)
        
        elif key == WXK_BACK:               # Empty text control when init w/ back key
            ch = ""
                                            # Handle normal keys
        elif key < 256 and key >= 0 and chr(key) in string.printable:
            ch = chr(key)
            if not evt.ShiftDown():
                ch = string.lower(ch)
        
        if ch is not None:                  # If are at this point with a key,
            self._tc.SetValue(ch)           # replace the contents of the text control.
            self._tc.SetInsertionPointEnd() # Move to the end so that subsequent keys are appended
        else:
            evt.Skip()

    def StartingClick(self):
        """ If the editor is enabled by clicking on the cell, this method will be
            called to allow the editor to simulate the click on the control.
        """
        pass

    def Destroy(self):
        """ Final cleanup """
        self.base_Destroy()

    def Clone(self):
        """ Create a new object which is the copy of this one. Must Override. """
        return CCellEditor()

#---------------------------------------------------------------------------
class CSheet(wxGrid):
    def __init__(self, parent):
        wxGrid.__init__(self, parent, -1)
        
        # Init variables
        self._lastCol = -1              # Init last cell column clicked
        self._lastRow = -1              # Init last cell row clicked
        self._selected = None           # Init range currently selected
                                        # Map string datatype to default renderer/editor
        self.RegisterDataType(wxGRID_VALUE_STRING,
                              wxGridCellStringRenderer(),
                              CCellEditor(self))
        
        self.CreateGrid(4, 3)           # By default start with a 4 x 3 grid
        self.SetColLabelSize(18)        # Default sizes and alignment
        self.SetRowLabelSize(50)
        self.SetRowLabelAlignment(wxALIGN_RIGHT, wxALIGN_BOTTOM)
        self.SetColSize(0, 75)          # Default column sizes
        self.SetColSize(1, 75)
        self.SetColSize(2, 75)
        
        # Sink events        
        EVT_GRID_CELL_LEFT_CLICK(  self, self.OnLeftClick)
        EVT_GRID_CELL_RIGHT_CLICK( self, self.OnRightClick)
        EVT_GRID_CELL_LEFT_DCLICK( self, self.OnLeftDoubleClick)
        EVT_GRID_RANGE_SELECT(     self, self.OnRangeSelect)
        EVT_GRID_ROW_SIZE(         self, self.OnRowSize)
        EVT_GRID_COL_SIZE(         self, self.OnColSize)
        EVT_GRID_CELL_CHANGE(      self, self.OnCellChange)
        EVT_GRID_SELECT_CELL(      self, self.OnGridSelectCell)

    def OnGridSelectCell(self, event):
        """ Track cell selections """
        # Save the last cell coordinates
        self._lastRow, self._lastCol = event.GetRow(), event.GetCol()
        event.Skip()
        
    def OnRowSize(self, event):
        event.Skip()

    def OnColSize(self, event):
        event.Skip()

    def OnCellChange(self, event):
        event.Skip()
    
    def OnLeftClick(self, event):
        """ Override left-click behavior to prevent left-click edit initiation """
        # Save the cell clicked
        currCell = (event.GetRow(), event.GetCol())
        
        # Suppress event if same cell clicked twice in a row.
        # This prevents a single-click from initiating an edit.
        if currCell != (self._lastRow, self._lastCol): event.Skip()
        
    def OnRightClick(self, event):
        """ Move grid cursor when a cell is right-clicked """
        self.SetGridCursor( event.GetRow(), event.GetCol() )
        event.Skip()
        
    def OnLeftDoubleClick(self, event):
        """ Initiate the cell editor on a double-click """
        # Move grid cursor to double-clicked cell
        if self.CanEnableCellControl():
            self.SetGridCursor( event.GetRow(), event.GetCol() )
            self.EnableCellEditControl(true)    # Show the cell editor
        event.Skip()

    def OnRangeSelect(self, event):
        """ Track which cells are selected so that copy/paste behavior can be implemented """
        # If a single cell is selected, then Selecting() returns false (0)
        # and range coords are entire grid.  In this case cancel previous selection.
        # If more than one cell is selected, then Selecting() is true (1)
        # and range accurately reflects selected cells.  Save them.
        # If more cells are added to a selection, selecting remains true (1)
        self._selected = None
        if event.Selecting():
            self._selected = ((event.GetTopRow(), event.GetLeftCol()),
                              (event.GetBottomRow(), event.GetRightCol()))
        event.Skip()
        
    def Copy(self):
        """ Copy the currently selected cells to the clipboard """
        # TODO: raise an error when there are no cells selected?
        if self._selected == None: return
        ((r1, c1), (r2, c2)) = self._selected
        
        # Build a string to put on the clipboard
        # (Is there a faster way to do this in Python?)
        crlf = chr(13) + chr(10)
        tab = chr(9)
        s = ""
        for row in range(r1, r2+1):
            for col in range(c1, c2):
                s += self.GetCellValue(row,col)
                s += tab
            s += self.GetCellValue(row, c2)
            s += crlf
        
        # Put the string on the clipboard
        if wxTheClipboard.Open():
            wxTheClipboard.Clear()
            wxTheClipboard.SetData(wxTextDataObject(s))
            wxTheClipboard.Close()

    def Paste(self):
        """ Paste the contents of the clipboard into the currently selected cells """
        # (Is there a better way to do this?)
        if wxTheClipboard.Open():
            td = wxTextDataObject()
            success = wxTheClipboard.GetData(td)
            wxTheClipboard.Close()
            if not success: return              # Exit on failure
            s = td.GetText()                    # Get the text
            
            crlf = chr(13) + chr(10)            # CrLf characters
            tab = chr(9)                        # Tab character
            
            rows = split(s, crlf)               # split into rows
            rows = rows[0:-1]                   # leave out last element, which is always empty
            for i in range(0, len(rows)):       # split rows into elements
                rows[i] = split(rows[i], tab)
            
            # Get the starting and ending cell range to paste into
            if self._selected == None:          # If no cells selected...
                r1 = self.GetGridCursorRow()    # Start the paste at the current location
                c1 = self.GetGridCursorCol()
                r2 = self.GetNumberRows()-1     # Go to maximum row and col extents
                c2 = self.GetNumberCols()-1
            else:                               # If cells selected, only paste there
                ((r1, c1), (r2, c2)) = self._selected
            
            # Enter data into spreadsheet cells one at a time
            r = r1                              # Init row and column counters
            c = c1
            for row in rows:                    # Loop over all rows
                for element in row:             # Loop over all row elements
                    self.SetCellValue(r, c, str(element))   # Set cell value
                    c += 1                      # Increment the column counter
                    if c > c2: break            # Do not exceed maximum column
                r += 1
                if r > r2: break                # Do not exceed maximum row
                c = c1

    def Clear(self):
        """ Clear the currently selected cells """
        if self._selected == None:              # If no selection...
            r = self.GetGridCursorRow()         # clear only current cell
            c = self.GetGridCursorCol()
            self.SetCellValue(r, c, "")
        else:                                   # Otherwise clear selected cells
            ((r1, c1), (r2, c2)) = self._selected
            for r in range(r1, r2+1):
                for c in range(c1, c2+1):
                    self.SetCellValue(r, c, "")

    def SetNumberRows(self, numRows=1):
        """ Set the number of rows in the sheet """
        # Check for non-negative number
        if numRows < 0:  return false
        
        # Adjust number of rows
        curRows = self.GetNumberRows()
        if curRows < numRows:
            self.AppendRows(numRows - curRows)
        elif curRows > numRows:
            self.DeleteRows(numRows, curRows - numRows)

        return true

    def SetNumberCols(self, numCols=1):
        """ Set the number of columns in the sheet """
        # Check for non-negative number
        if numCols < 0:  return false
        
        # Adjust number of rows
        curCols = self.GetNumberCols()
        if curCols < numCols:
            self.AppendCols(numCols - curCols)
        elif curCols > numCols:
            self.DeleteCols(numCols, curCols - numCols)

        return true
