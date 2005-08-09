# Name:         xrced.py
# Purpose:      XRC editor, main module
# Author:       Roman Rolinsky <rolinsky@mema.ucl.ac.be>
# Created:      20.08.2001
# RCS-ID:       $Id$

"""

xrced -- Simple resource editor for XRC format used by wxWindows/wxPython
         GUI toolkit.

Usage:

    xrced [ -h ] [ -v ] [ XRC-file ]

Options:

    -h          output short usage info and exit

    -v          output version info and exit
"""

from globals import *
import os, sys, getopt, re, traceback, tempfile, shutil

# Local modules
from tree import *                      # imports xxx which imports params
from panel import *
from tools import *
# Cleanup recursive import sideeffects, otherwise we can't create undoMan
import undo
undo.ParamPage = ParamPage
undoMan = g.undoMan = UndoManager()

# Set application path for loading resources
if __name__ == '__main__':
    basePath = os.path.dirname(sys.argv[0])
else:
    basePath = os.path.dirname(__file__)

# 1 adds CMD command to Help menu
debug = 0

g.helpText = """\
<HTML><H2>Welcome to XRC<font color="blue">ed</font></H2><H3><font color="green">DON'T PANIC :)</font></H3>
Read this note before clicking on anything!<P>
To start select tree root, then popup menu with your right mouse button,
select "Append Child", and then any command.<P>
Or just press one of the buttons on the tools palette.<P>
Enter XML ID, change properties, create children.<P>
To test your interface select Test command (View menu).<P>
Consult README file for the details.</HTML>
"""

defaultIDs = {xxxPanel:'PANEL', xxxDialog:'DIALOG', xxxFrame:'FRAME',
              xxxMenuBar:'MENUBAR', xxxMenu:'MENU', xxxToolBar:'TOOLBAR',
              xxxWizard:'WIZARD'}

defaultName = 'UNTITLED.xrc'

################################################################################

# ScrolledMessageDialog - modified from wxPython lib to set fixed-width font
class ScrolledMessageDialog(wxDialog):
    def __init__(self, parent, msg, caption, pos = wxDefaultPosition, size = (500,300)):
        from wxPython.lib.layoutf import Layoutf
        wxDialog.__init__(self, parent, -1, caption, pos, size)
        text = wxTextCtrl(self, -1, msg, wxDefaultPosition,
                             wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY)
        text.SetFont(g.modernFont())
        dc = wxWindowDC(text)
        # !!! possible bug - GetTextExtent without font returns sysfont dims
        w, h = dc.GetFullTextExtent(' ', g.modernFont())[:2]
        ok = wxButton(self, wxID_OK, "OK")
        text.SetConstraints(Layoutf('t=t5#1;b=t5#2;l=l5#1;r=r5#1', (self,ok)))
        text.SetSize((w * 80 + 30, h * 40))
        text.ShowPosition(1)
        ok.SetConstraints(Layoutf('b=b5#1;x%w50#1;w!80;h!25', (self,)))
        self.SetAutoLayout(True)
        self.Fit()
        self.CenterOnScreen(wxBOTH)

################################################################################

# Event handler for using during location
class Locator(wxEvtHandler):
    def ProcessEvent(self, evt):
        print evt

class Frame(wxFrame):
    def __init__(self, pos, size):
        wxFrame.__init__(self, None, -1, '', pos, size)
        global frame
        frame = g.frame = self
        bar = self.CreateStatusBar(2)
        bar.SetStatusWidths([-1, 40])
        self.SetIcon(images.getIconIcon())

        # Idle flag
        self.inIdle = False

        # Load our own resources
        self.res = wxXmlResource('')
        # !!! Blocking of assert failure occurring in older unicode builds
        try:
            self.res.Load(os.path.join(basePath, 'xrced.xrc'))
        except wx._core.PyAssertionError:
            print 'PyAssertionError was ignored'

        # Make menus
        menuBar = wxMenuBar()

        menu = wxMenu()
        menu.Append(wxID_NEW, '&New\tCtrl-N', 'New file')
        menu.AppendSeparator()
        menu.Append(wxID_OPEN, '&Open...\tCtrl-O', 'Open XRC file')
        self.recentMenu = wxMenu()
        self.AppendRecent(self.recentMenu)
        menu.AppendMenu(-1, 'Open Recent', self.recentMenu, 'Open a recent file')
        menu.AppendSeparator()
        menu.Append(wxID_SAVE, '&Save\tCtrl-S', 'Save XRC file')
        menu.Append(wxID_SAVEAS, 'Save &As...', 'Save XRC file under different name')
        menu.AppendSeparator()
        menu.Append(wxID_EXIT, '&Quit\tCtrl-Q', 'Exit application')

        menuBar.Append(menu, '&File')

        menu = wxMenu()
        menu.Append(wxID_UNDO, '&Undo\tCtrl-Z', 'Undo')
        menu.Append(wxID_REDO, '&Redo\tCtrl-Y', 'Redo')
        menu.AppendSeparator()
        menu.Append(wxID_CUT, 'Cut\tCtrl-X', 'Cut to the clipboard')
        menu.Append(wxID_COPY, '&Copy\tCtrl-C', 'Copy to the clipboard')
        menu.Append(wxID_PASTE, '&Paste\tCtrl-V', 'Paste from the clipboard')
        self.ID_DELETE = wxNewId()
        menu.Append(self.ID_DELETE, '&Delete\tCtrl-D', 'Delete object')
        menu.AppendSeparator()
        self.ID_LOCATE = wxNewId()
        self.ID_TOOL_LOCATE = wxNewId()
        self.ID_TOOL_PASTE = wxNewId()
        menu.Append(self.ID_LOCATE, '&Locate\tCtrl-L', 'Locate control in test window and select it')
        menuBar.Append(menu, '&Edit')

        menu = wxMenu()
        self.ID_EMBED_PANEL = wxNewId()
        menu.Append(self.ID_EMBED_PANEL, '&Embed Panel',
                    'Toggle embedding properties panel in the main window', True)
        menu.Check(self.ID_EMBED_PANEL, conf.embedPanel)
        self.ID_SHOW_TOOLS = wxNewId()
        menu.Append(self.ID_SHOW_TOOLS, 'Show &Tools', 'Toggle tools', True)
        menu.Check(self.ID_SHOW_TOOLS, conf.showTools)
        menu.AppendSeparator()
        self.ID_TEST = wxNewId()
        menu.Append(self.ID_TEST, '&Test\tF5', 'Show test window')
        self.ID_REFRESH = wxNewId()
        menu.Append(self.ID_REFRESH, '&Refresh\tCtrl-R', 'Refresh test window')
        self.ID_AUTO_REFRESH = wxNewId()
        menu.Append(self.ID_AUTO_REFRESH, '&Auto-refresh\tCtrl-A',
                    'Toggle auto-refresh mode', True)
        menu.Check(self.ID_AUTO_REFRESH, conf.autoRefresh)
        self.ID_TEST_HIDE = wxNewId()
        menu.Append(self.ID_TEST_HIDE, '&Hide\tCtrl-H', 'Close test window')
        menuBar.Append(menu, '&View')

        menu = wxMenu()
        menu.Append(wxID_ABOUT, '&About...', 'About XCRed')
        self.ID_README = wxNewId()
        menu.Append(self.ID_README, '&Readme...', 'View the README file')
        if debug:
            self.ID_DEBUG_CMD = wxNewId()
            menu.Append(self.ID_DEBUG_CMD, 'CMD', 'Python command line')
            EVT_MENU(self, self.ID_DEBUG_CMD, self.OnDebugCMD)
        menuBar.Append(menu, '&Help')

        self.menuBar = menuBar
        self.SetMenuBar(menuBar)

        # Create toolbar
        tb = self.CreateToolBar(wxTB_HORIZONTAL | wxNO_BORDER | wxTB_FLAT)
        tb.SetToolBitmapSize((24,24))
        new_bmp  = wx.ArtProvider.GetBitmap(wx.ART_NORMAL_FILE, wx.ART_TOOLBAR)
        open_bmp = wx.ArtProvider.GetBitmap(wx.ART_FILE_OPEN, wx.ART_TOOLBAR)
        save_bmp = wx.ArtProvider.GetBitmap(wx.ART_FILE_SAVE, wx.ART_TOOLBAR)
        undo_bmp = wx.ArtProvider.GetBitmap(wx.ART_UNDO, wx.ART_TOOLBAR)
        redo_bmp = wx.ArtProvider.GetBitmap(wx.ART_REDO, wx.ART_TOOLBAR)
        cut_bmp  = wx.ArtProvider.GetBitmap(wx.ART_CUT, wx.ART_TOOLBAR)
        copy_bmp = wx.ArtProvider.GetBitmap(wx.ART_COPY, wx.ART_TOOLBAR)
        paste_bmp= wx.ArtProvider.GetBitmap(wx.ART_PASTE, wx.ART_TOOLBAR)
        
        tb.AddSimpleTool(wxID_NEW, new_bmp, 'New', 'New file')
        tb.AddSimpleTool(wxID_OPEN, open_bmp, 'Open', 'Open file')
        tb.AddSimpleTool(wxID_SAVE, save_bmp, 'Save', 'Save file')
        tb.AddControl(wxStaticLine(tb, -1, size=(-1,23), style=wxLI_VERTICAL))
        tb.AddSimpleTool(wxID_UNDO, undo_bmp, 'Undo', 'Undo')
        tb.AddSimpleTool(wxID_REDO, redo_bmp, 'Redo', 'Redo')
        tb.AddControl(wxStaticLine(tb, -1, size=(-1,23), style=wxLI_VERTICAL))
        tb.AddSimpleTool(wxID_CUT, cut_bmp, 'Cut', 'Cut')
        tb.AddSimpleTool(wxID_COPY, copy_bmp, 'Copy', 'Copy')
        tb.AddSimpleTool(self.ID_TOOL_PASTE, paste_bmp, 'Paste', 'Paste')
        tb.AddControl(wxStaticLine(tb, -1, size=(-1,23), style=wxLI_VERTICAL))
        tb.AddSimpleTool(self.ID_TOOL_LOCATE,
                        images.getLocateBitmap(), #images.getLocateArmedBitmap(),
                        'Locate', 'Locate control in test window and select it', True)
        tb.AddControl(wxStaticLine(tb, -1, size=(-1,23), style=wxLI_VERTICAL))
        tb.AddSimpleTool(self.ID_TEST, images.getTestBitmap(), 'Test', 'Test window')
        tb.AddSimpleTool(self.ID_REFRESH, images.getRefreshBitmap(),
                         'Refresh', 'Refresh view')
        tb.AddSimpleTool(self.ID_AUTO_REFRESH, images.getAutoRefreshBitmap(),
                         'Auto-refresh', 'Toggle auto-refresh mode', True)
        if wxPlatform == '__WXGTK__':
            tb.AddSeparator()   # otherwise auto-refresh sticks in status line
        tb.ToggleTool(self.ID_AUTO_REFRESH, conf.autoRefresh)
        tb.Realize()
 
        self.tb = tb
        self.minWidth = tb.GetSize()[0] # minimal width is the size of toolbar

        # File
        EVT_MENU(self, wxID_NEW, self.OnNew)
        EVT_MENU(self, wxID_OPEN, self.OnOpen)
        EVT_MENU(self, wxID_SAVE, self.OnSaveOrSaveAs)
        EVT_MENU(self, wxID_SAVEAS, self.OnSaveOrSaveAs)
        EVT_MENU(self, wxID_EXIT, self.OnExit)
        # Edit
        EVT_MENU(self, wxID_UNDO, self.OnUndo)
        EVT_MENU(self, wxID_REDO, self.OnRedo)
        EVT_MENU(self, wxID_CUT, self.OnCutDelete)
        EVT_MENU(self, wxID_COPY, self.OnCopy)
        EVT_MENU(self, wxID_PASTE, self.OnPaste)
        EVT_MENU(self, self.ID_TOOL_PASTE, self.OnPaste)
        EVT_MENU(self, self.ID_DELETE, self.OnCutDelete)
        EVT_MENU(self, self.ID_LOCATE, self.OnLocate)
        EVT_MENU(self, self.ID_TOOL_LOCATE, self.OnLocate)
        # View
        EVT_MENU(self, self.ID_EMBED_PANEL, self.OnEmbedPanel)
        EVT_MENU(self, self.ID_SHOW_TOOLS, self.OnShowTools)
        EVT_MENU(self, self.ID_TEST, self.OnTest)
        EVT_MENU(self, self.ID_REFRESH, self.OnRefresh)
        EVT_MENU(self, self.ID_AUTO_REFRESH, self.OnAutoRefresh)
        EVT_MENU(self, self.ID_TEST_HIDE, self.OnTestHide)
        # Help
        EVT_MENU(self, wxID_ABOUT, self.OnAbout)
        EVT_MENU(self, self.ID_README, self.OnReadme)

        # Update events
        EVT_UPDATE_UI(self, wxID_SAVE, self.OnUpdateUI)
        EVT_UPDATE_UI(self, wxID_CUT, self.OnUpdateUI)
        EVT_UPDATE_UI(self, wxID_COPY, self.OnUpdateUI)
        EVT_UPDATE_UI(self, wxID_PASTE, self.OnUpdateUI)
        EVT_UPDATE_UI(self, self.ID_LOCATE, self.OnUpdateUI)
        EVT_UPDATE_UI(self, self.ID_TOOL_LOCATE, self.OnUpdateUI)
        EVT_UPDATE_UI(self, self.ID_TOOL_PASTE, self.OnUpdateUI)
        EVT_UPDATE_UI(self, wxID_UNDO, self.OnUpdateUI)
        EVT_UPDATE_UI(self, wxID_REDO, self.OnUpdateUI)
        EVT_UPDATE_UI(self, self.ID_DELETE, self.OnUpdateUI)
        EVT_UPDATE_UI(self, self.ID_TEST, self.OnUpdateUI)
        EVT_UPDATE_UI(self, self.ID_REFRESH, self.OnUpdateUI)

        # Build interface
        sizer = wxBoxSizer(wxVERTICAL)
        sizer.Add(wxStaticLine(self, -1), 0, wxEXPAND)
        # Horizontal sizer for toolbar and splitter
        self.toolsSizer = sizer1 = wxBoxSizer()
        splitter = wxSplitterWindow(self, -1, style=wxSP_3DSASH)
        self.splitter = splitter
        splitter.SetMinimumPaneSize(100)
        # Create tree
        global tree
        g.tree = tree = XML_Tree(splitter, -1)

        # Init pull-down menu data
        global pullDownMenu
        g.pullDownMenu = pullDownMenu = PullDownMenu(self)

        # Vertical toolbar for GUI buttons
        g.tools = tools = Tools(self)
        tools.Show(conf.showTools)
        if conf.showTools: sizer1.Add(tools, 0, wxEXPAND)

        tree.RegisterKeyEvents()

        # !!! frame styles are broken
        # Miniframe for not embedded mode
        miniFrame = wxFrame(self, -1, 'Properties & Style',
                            (conf.panelX, conf.panelY),
                            (conf.panelWidth, conf.panelHeight))
        self.miniFrame = miniFrame
        sizer2 = wxBoxSizer()
        miniFrame.SetAutoLayout(True)
        miniFrame.SetSizer(sizer2)
        EVT_CLOSE(self.miniFrame, self.OnCloseMiniFrame)
        # Create panel for parameters
        global panel
        if conf.embedPanel:
            panel = Panel(splitter)
            # Set plitter windows
            splitter.SplitVertically(tree, panel, conf.sashPos)
        else:
            panel = Panel(miniFrame)
            sizer2.Add(panel, 1, wxEXPAND)
            miniFrame.Show(True)
            splitter.Initialize(tree)
        sizer1.Add(splitter, 1, wxEXPAND)
        sizer.Add(sizer1, 1, wxEXPAND)
        self.SetAutoLayout(True)
        self.SetSizer(sizer)

        # Initialize
        self.clipboard = None
        self.Clear()

        # Other events
        EVT_IDLE(self, self.OnIdle)
        EVT_CLOSE(self, self.OnCloseWindow)
        EVT_KEY_DOWN(self, tools.OnKeyDown)
        EVT_KEY_UP(self, tools.OnKeyUp)
    
    def AppendRecent(self, menu):
        # add recently used files to the menu
        for id,name in conf.recentfiles.iteritems():
            menu.Append(id,name)
            EVT_MENU(self,id,self.OnRecentFile)
        return 
        
    def OnRecentFile(self,evt):
        # open recently used file
        if not self.AskSave(): return
        wxBeginBusyCursor()
        try:
            path=conf.recentfiles[evt.GetId()]
            if self.Open(path):
                self.SetStatusText('Data loaded')
            else:
                self.SetStatusText('Failed')
        except KeyError:
            self.SetStatusText('No such file')
        wxEndBusyCursor()

    def OnNew(self, evt):
        if not self.AskSave(): return
        self.Clear()

    def OnOpen(self, evt):
        if not self.AskSave(): return
        dlg = wxFileDialog(self, 'Open', os.path.dirname(self.dataFile),
                           '', '*.xrc', wxOPEN | wxCHANGE_DIR)
        if dlg.ShowModal() == wxID_OK:
            path = dlg.GetPath()
            self.SetStatusText('Loading...')
            wxYield()
            wxBeginBusyCursor()
            try:
                if self.Open(path):
                    self.SetStatusText('Data loaded')
                else:
                    self.SetStatusText('Failed')
                self.SaveRecent(path)
            finally:
                wxEndBusyCursor()
        dlg.Destroy()

    def OnSaveOrSaveAs(self, evt):
        if evt.GetId() == wxID_SAVEAS or not self.dataFile:
            if self.dataFile: name = ''
            else: name = defaultName
            dirname = os.path.abspath(os.path.dirname(self.dataFile))
            dlg = wxFileDialog(self, 'Save As', dirname, name, '*.xrc',
                               wxSAVE | wxOVERWRITE_PROMPT | wxCHANGE_DIR)
            if dlg.ShowModal() == wxID_OK:
                path = dlg.GetPath()
                dlg.Destroy()
            else:
                dlg.Destroy()
                return
        else:
            path = self.dataFile
        self.SetStatusText('Saving...')
        wxYield()
        wxBeginBusyCursor()
        try:
            try:
                tmpFile,tmpName = tempfile.mkstemp(prefix='xrced-')
                os.close(tmpFile)
                self.Save(tmpName) # save temporary file first
                shutil.move(tmpName, path)
                self.dataFile = path
                self.SetStatusText('Data saved')
                self.SaveRecent(path)
            except IOError:
                self.SetStatusText('Failed')
        finally:
            wxEndBusyCursor()        

    def SaveRecent(self,path):
        # append to recently used files
        if path not in conf.recentfiles.values():
            newid = wxNewId()
            self.recentMenu.Append(newid, path)
            EVT_MENU(self, newid, self.OnRecentFile)
            conf.recentfiles[newid] = path

    def OnExit(self, evt):
        self.Close()

    def OnUndo(self, evt):
        # Extra check to not mess with idle updating
        if undoMan.CanUndo():
            undoMan.Undo()

    def OnRedo(self, evt):
        if undoMan.CanRedo():
            undoMan.Redo()

    def OnCopy(self, evt):
        selected = tree.selection
        if not selected: return         # key pressed event
        xxx = tree.GetPyData(selected)
        self.clipboard = xxx.element.cloneNode(True)
        self.SetStatusText('Copied')

    def OnPaste(self, evt):
        selected = tree.selection
        if not selected: return         # key pressed event
        # For pasting with Ctrl pressed
        appendChild = True
        if evt.GetId() == pullDownMenu.ID_PASTE_SIBLING: appendChild = False
        elif evt.GetId() == self.ID_TOOL_PASTE:
            if g.tree.ctrl: appendChild = False
            else: appendChild = not tree.NeedInsert(selected)
        else: appendChild = not tree.NeedInsert(selected)
        xxx = tree.GetPyData(selected)
        if not appendChild:
            # If has next item, insert, else append to parent
            nextItem = tree.GetNextSibling(selected)
            parentLeaf = tree.GetItemParent(selected)
        # Expanded container (must have children)
        elif tree.IsExpanded(selected) and tree.GetChildrenCount(selected, False):
            # Insert as first child
            nextItem = tree.GetFirstChild(selected)[0]
            parentLeaf = selected
        else:
            # No children or unexpanded item - appendChild stays True
            nextItem = wxTreeItemId()   # no next item
            parentLeaf = selected
        parent = tree.GetPyData(parentLeaf).treeObject()

        # Create a copy of clipboard element
        elem = self.clipboard.cloneNode(True)
        # Tempopary xxx object to test things
        xxx = MakeXXXFromDOM(parent, elem)

        # Check compatibility
        error = False
        # Top-level
        x = xxx.treeObject()
        if x.__class__ in [xxxDialog, xxxFrame, xxxMenuBar, xxxWizard]:
            # Top-level classes
            if parent.__class__ != xxxMainNode: error = True
        elif x.__class__ == xxxToolBar:
            # Toolbar can be top-level of child of panel or frame
            if parent.__class__ not in [xxxMainNode, xxxPanel, xxxFrame] and \
               not parent.isSizer: error = True
        elif x.__class__ == xxxPanel and parent.__class__ == xxxMainNode:
            pass
        elif x.__class__ == xxxSpacer:
            if not parent.isSizer: error = True
        elif x.__class__ == xxxSeparator:
            if not parent.__class__ in [xxxMenu, xxxToolBar]: error = True
        elif x.__class__ == xxxTool:
            if parent.__class__ != xxxToolBar: error = True
        elif x.__class__ == xxxMenu:
            if not parent.__class__ in [xxxMainNode, xxxMenuBar, xxxMenu]: error = True
        elif x.__class__ == xxxMenuItem:
            if not parent.__class__ in [xxxMenuBar, xxxMenu]: error = True
        elif x.isSizer and parent.__class__ == xxxNotebook: error = True
        else:                           # normal controls can be almost anywhere
            if parent.__class__ == xxxMainNode or \
               parent.__class__ in [xxxMenuBar, xxxMenu]: error = True
        if error:
            if parent.__class__ == xxxMainNode: parentClass = 'root'
            else: parentClass = parent.className
            wxLogError('Incompatible parent/child: parent is %s, child is %s!' %
                       (parentClass, x.className))
            return

        # Check parent and child relationships.
        # If parent is sizer or notebook, child is of wrong class or
        # parent is normal window, child is child container then detach child.
        isChildContainer = isinstance(xxx, xxxChildContainer)
        if isChildContainer and \
           ((parent.isSizer and not isinstance(xxx, xxxSizerItem)) or \
            (isinstance(parent, xxxNotebook) and not isinstance(xxx, xxxNotebookPage)) or \
           not (parent.isSizer or isinstance(parent, xxxNotebook))):
            elem.removeChild(xxx.child.element) # detach child
            elem.unlink()           # delete child container
            elem = xxx.child.element # replace
            # This may help garbage collection
            xxx.child.parent = None
            isChildContainer = False
        # Parent is sizer or notebook, child is not child container
        if parent.isSizer and not isChildContainer and not isinstance(xxx, xxxSpacer):
            # Create sizer item element
            sizerItemElem = MakeEmptyDOM(parent.itemTag)
            sizerItemElem.appendChild(elem)
            elem = sizerItemElem
        elif isinstance(parent, xxxNotebook) and not isChildContainer:
            pageElem = MakeEmptyDOM('notebookpage')
            pageElem.appendChild(elem)
            elem = pageElem
        # Insert new node, register undo
        newItem = tree.InsertNode(parentLeaf, parent, elem, nextItem)
        undoMan.RegisterUndo(UndoPasteCreate(parentLeaf, parent, newItem, selected))
        # Scroll to show new item (!!! redundant?)
        tree.EnsureVisible(newItem)
        tree.SelectItem(newItem)
        if not tree.IsVisible(newItem):
            tree.ScrollTo(newItem)
            tree.Refresh()
        # Update view?
        if g.testWin and tree.IsHighlatable(newItem):
            if conf.autoRefresh:
                tree.needUpdate = True
                tree.pendingHighLight = newItem
            else:
                tree.pendingHighLight = None
        self.SetModified()
        self.SetStatusText('Pasted')

    def OnCutDelete(self, evt):
        selected = tree.selection
        if not selected: return         # key pressed event
        # Undo info
        if evt.GetId() == wxID_CUT:
            self.lastOp = 'CUT'
            status = 'Removed to clipboard'
        else:
            self.lastOp = 'DELETE'
            status = 'Deleted'
        # Delete testWin?
        if g.testWin:
            # If deleting top-level item, delete testWin
            if selected == g.testWin.item:
                g.testWin.Destroy()
                g.testWin = None
            else:
                # Remove highlight, update testWin
                if g.testWin.highLight:
                    g.testWin.highLight.Remove()
                tree.needUpdate = True
        # Prepare undo data
        panel.Apply()
        index = tree.ItemFullIndex(selected)
        parent = tree.GetPyData(tree.GetItemParent(selected)).treeObject()
        elem = tree.RemoveLeaf(selected)
        undoMan.RegisterUndo(UndoCutDelete(index, parent, elem))
        if evt.GetId() == wxID_CUT:
            if self.clipboard: self.clipboard.unlink()
            self.clipboard = elem.cloneNode(True)
        tree.pendingHighLight = None
        tree.UnselectAll()
        tree.selection = None
        # Update tools
        g.tools.UpdateUI()
        panel.Clear()
        self.SetModified()
        self.SetStatusText(status)

    def OnSubclass(self, evt):
        selected = tree.selection
        xxx = tree.GetPyData(selected).treeObject()
        elem = xxx.element
        subclass = xxx.subclass
        dlg = wxTextEntryDialog(self, 'Subclass:', defaultValue=subclass)
        if dlg.ShowModal() == wxID_OK:
            subclass = dlg.GetValue()
            if subclass:
                elem.setAttribute('subclass', subclass)
            elif elem.hasAttribute('subclass'):
                elem.removeAttribute('subclass')
            self.SetModified()
            xxx.subclass = elem.getAttribute('subclass')
            tree.SetItemText(selected, xxx.treeName())
            panel.pages[0].box.SetLabel(xxx.panelName())
        dlg.Destroy()

    def OnEmbedPanel(self, evt):
        conf.embedPanel = evt.IsChecked()
        if conf.embedPanel:
            # Remember last dimentions
            conf.panelX, conf.panelY = self.miniFrame.GetPosition()
            conf.panelWidth, conf.panelHeight = self.miniFrame.GetSize()
            size = self.GetSize()
            pos = self.GetPosition()
            sizePanel = panel.GetSize()
            panel.Reparent(self.splitter)
            self.miniFrame.GetSizer().Remove(panel)
            wxYield()
            # Widen
            self.SetDimensions(pos.x, pos.y, size.width + sizePanel.width, size.height)
            self.splitter.SplitVertically(tree, panel, conf.sashPos)
            self.miniFrame.Show(False)
        else:
            conf.sashPos = self.splitter.GetSashPosition()
            pos = self.GetPosition()
            size = self.GetSize()
            sizePanel = panel.GetSize()
            self.splitter.Unsplit(panel)
            sizer = self.miniFrame.GetSizer()
            panel.Reparent(self.miniFrame)
            panel.Show(True)
            sizer.Add(panel, 1, wxEXPAND)
            self.miniFrame.Show(True)
            self.miniFrame.SetDimensions(conf.panelX, conf.panelY,
                                         conf.panelWidth, conf.panelHeight)
            wxYield()
            # Reduce width
            self.SetDimensions(pos.x, pos.y,
                               max(size.width - sizePanel.width, self.minWidth), size.height)

    def OnShowTools(self, evt):
        conf.showTools = evt.IsChecked()
        g.tools.Show(conf.showTools)
        if conf.showTools:
            self.toolsSizer.Prepend(g.tools, 0, wxEXPAND)
        else:
            self.toolsSizer.Remove(g.tools)
        self.toolsSizer.Layout()
        
    def OnTest(self, evt):
        if not tree.selection: return   # key pressed event
        tree.ShowTestWindow(tree.selection)

    def OnTestHide(self, evt):
        tree.CloseTestWindow()

    # Find object by relative position
    def FindObject(self, item, obj):
        # We simply perform depth-first traversal, sinse it's too much
        # hassle to deal with all sizer/window combinations
        w = tree.FindNodeObject(item)
        if w == obj:
            return item
        if tree.ItemHasChildren(item):
            child = tree.GetFirstChild(item)[0]
            while child:
                found = self.FindObject(child, obj)
                if found: return found
                child = tree.GetNextSibling(child)
        return None

    def OnTestWinLeftDown(self, evt):
        pos = evt.GetPosition()
        self.SetHandler(g.testWin)
        g.testWin.Disconnect(wxID_ANY, wxID_ANY, wxEVT_LEFT_DOWN)
        item = self.FindObject(g.testWin.item, evt.GetEventObject())
        if item:
            tree.SelectItem(item)
        self.tb.ToggleTool(self.ID_TOOL_LOCATE, False)
        if item:
            self.SetStatusText('Selected %s' % tree.GetItemText(item))
        else:
            self.SetStatusText('Locate failed!')

    def SetHandler(self, w, h=None):
        if h:
            w.SetEventHandler(h)
            w.SetCursor(wxCROSS_CURSOR)
        else:
            w.SetEventHandler(w)
            w.SetCursor(wxNullCursor)
        for ch in w.GetChildren():
            self.SetHandler(ch, h)

    def OnLocate(self, evt):
        if g.testWin:
            if evt.GetId() == self.ID_LOCATE or \
               evt.GetId() == self.ID_TOOL_LOCATE and evt.IsChecked():
                self.SetHandler(g.testWin, g.testWin)
                g.testWin.Connect(wxID_ANY, wxID_ANY, wxEVT_LEFT_DOWN, self.OnTestWinLeftDown)
                if evt.GetId() == self.ID_LOCATE:
                    self.tb.ToggleTool(self.ID_TOOL_LOCATE, True)
            elif evt.GetId() == self.ID_TOOL_LOCATE and not evt.IsChecked():
                self.SetHandler(g.testWin, None)
                g.testWin.Disconnect(wxID_ANY, wxID_ANY, wxEVT_LEFT_DOWN)
            self.SetStatusText('Click somewhere in your test window now')

    def OnRefresh(self, evt):
        # If modified, apply first
        selection = tree.selection
        if selection:
            xxx = tree.GetPyData(selection)
            if xxx and panel.IsModified():
                tree.Apply(xxx, selection)
        if g.testWin:
            # (re)create
            tree.CreateTestWin(g.testWin.item)
        panel.modified = False
        tree.needUpdate = False

    def OnAutoRefresh(self, evt):
        conf.autoRefresh = evt.IsChecked()
        self.menuBar.Check(self.ID_AUTO_REFRESH, conf.autoRefresh)
        self.tb.ToggleTool(self.ID_AUTO_REFRESH, conf.autoRefresh)

    def OnAbout(self, evt):
        str = '''\
XRCed version %s

(c) Roman Rolinsky <rollrom@users.sourceforge.net>
Homepage: http://xrced.sourceforge.net\
''' % version
        dlg = wxMessageDialog(self, str, 'About XRCed', wxOK | wxCENTRE)
        dlg.ShowModal()
        dlg.Destroy()

    def OnReadme(self, evt):
        text = open(os.path.join(basePath, 'README.txt'), 'r').read()
        dlg = ScrolledMessageDialog(self, text, "XRCed README")
        dlg.ShowModal()
        dlg.Destroy()

    # Simple emulation of python command line
    def OnDebugCMD(self, evt):
        while 1:
            try:
                exec raw_input('C:\> ')
            except EOFError:
                print '^D'
                break
            except:
                (etype, value, tb) =sys.exc_info()
                tblist =traceback.extract_tb(tb)[1:]
                msg =' '.join(traceback.format_exception_only(etype, value)
                        +traceback.format_list(tblist))
                print msg

    def OnCreate(self, evt):
        selected = tree.selection
        if tree.ctrl: appendChild = False
        else: appendChild = not tree.NeedInsert(selected)
        xxx = tree.GetPyData(selected)
        if not appendChild:
            # If insert before
            if tree.shift:
                # If has previous item, insert after it, else append to parent
                nextItem = selected
                parentLeaf = tree.GetItemParent(selected)
            else:
                # If has next item, insert, else append to parent
                nextItem = tree.GetNextSibling(selected)
                parentLeaf = tree.GetItemParent(selected)
        # Expanded container (must have children)
        elif tree.shift and tree.IsExpanded(selected) \
           and tree.GetChildrenCount(selected, False):
            nextItem = tree.GetFirstChild(selected)[0]
            parentLeaf = selected
        else:
            nextItem = wxTreeItemId()
            parentLeaf = selected
        parent = tree.GetPyData(parentLeaf)
        if parent.hasChild: parent = parent.child

        # Create object_ref?
        if evt.GetId() == ID_NEW.REF:
            ref = wxGetTextFromUser('Create reference to:', 'Create reference')
            if not ref: return
            xxx = MakeEmptyRefXXX(parent, ref)
        else:
            # Create empty element
            className = pullDownMenu.createMap[evt.GetId()]
            xxx = MakeEmptyXXX(parent, className)

        # Set default name for top-level windows
        if parent.__class__ == xxxMainNode:
            cl = xxx.treeObject().__class__
            frame.maxIDs[cl] += 1
            xxx.setTreeName('%s%d' % (defaultIDs[cl], frame.maxIDs[cl]))
        # And for some other standard controls
        elif parent.__class__ == xxxStdDialogButtonSizer:
            xxx.setTreeName(pullDownMenu.stdButtonIDs[evt.GetId()][0])
            # We can even set label
            obj = xxx.treeObject()
            elem = g.tree.dom.createElement('label')
            elem.appendChild(g.tree.dom.createTextNode(pullDownMenu.stdButtonIDs[evt.GetId()][1]))
            obj.params['label'] = xxxParam(elem)
            xxx.treeObject().element.appendChild(elem)

        # Insert new node, register undo
        elem = xxx.element
        newItem = tree.InsertNode(parentLeaf, parent, elem, nextItem)
        undoMan.RegisterUndo(UndoPasteCreate(parentLeaf, parent, newItem, selected))
        tree.EnsureVisible(newItem)
        tree.SelectItem(newItem)
        if not tree.IsVisible(newItem):
            tree.ScrollTo(newItem)
            tree.Refresh()
        # Update view?
        if g.testWin and tree.IsHighlatable(newItem):
            if conf.autoRefresh:
                tree.needUpdate = True
                tree.pendingHighLight = newItem
            else:
                tree.pendingHighLight = None
        tree.SetFocus()
        self.SetModified()


    # Replace one object with another
    def OnReplace(self, evt):
        selected = tree.selection
        xxx = tree.GetPyData(selected).treeObject()
        elem = xxx.element
        parent = elem.parentNode
        parentXXX = xxx.parent
        # New class
        className = pullDownMenu.createMap[evt.GetId() - 1000]
        # Create temporary empty node (with default values)
        dummy = MakeEmptyDOM(className)
        xxxClass = xxxDict[className]
        # Remove non-compatible children
        if tree.ItemHasChildren(selected) and not xxxClass.hasChildren:
            tree.DeleteChildren(selected)
        nodes = elem.childNodes[:]
        tags = []
        for node in nodes:
            if node.nodeType != minidom.Node.ELEMENT_NODE: continue
            remove = False
            tag = node.tagName
            if tag == 'object':
                if not xxxClass.hasChildren:
                    remove = True
            elif tag not in xxxClass.allParams and \
                     (not xxxClass.hasStyle or tag not in xxxClass.styles):
                remove = True
            else:
                tags.append(tag)
            if remove:
                elem.removeChild(node)
                node.unlink()
        
        # Copy parameters present in dummy but not in elem
        for node in dummy.childNodes:
            tag = node.tagName
            if tag not in tags:
                elem.appendChild(node.cloneNode(True))
        dummy.unlink()
        # Change class name
        elem.setAttribute('class', className)        
        if elem.hasAttribute('subclass'):
            elem.removeAttribute('subclass') # clear subclassing
        # Re-create xxx element
        xxx = MakeXXXFromDOM(parentXXX, elem)
        # Update parent in child objects
        if tree.ItemHasChildren(selected):
            i, cookie = tree.GetFirstChild(selected)
            while i.IsOk():
                x = tree.GetPyData(i)
                x.parent = xxx
                if x.hasChild: x.child.parent = xxx
                i, cookie = tree.GetNextChild(selected, cookie)
    
        # Update tree
        if tree.GetPyData(selected).hasChild: # child container
            container = tree.GetPyData(selected)
            container.child = xxx
            container.hasChildren = xxx.hasChildren
            container.isSizer = xxx.isSizer
        else:
            tree.SetPyData(selected, xxx)
        tree.SetItemText(selected, xxx.treeName())
        tree.SetItemImage(selected, xxx.treeImage())

        # Set default name for top-level windows
        if parent.__class__ == xxxMainNode:
            cl = xxx.treeObject().__class__
            frame.maxIDs[cl] += 1
            xxx.setTreeName('%s%d' % (defaultIDs[cl], frame.maxIDs[cl]))

        # Update panel
        g.panel.SetData(xxx)
        # Update tools
        g.tools.UpdateUI()

        #undoMan.RegisterUndo(UndoPasteCreate(parentLeaf, parent, newItem, selected))
        # Update view?
        if g.testWin and tree.IsHighlatable(selected):
            if conf.autoRefresh:
                tree.needUpdate = True
                tree.pendingHighLight = selected
            else:
                tree.pendingHighLight = None
        tree.SetFocus()
        self.SetModified()

    # Expand/collapse subtree
    def OnExpand(self, evt):
        if tree.selection: tree.ExpandAll(tree.selection)
        else: tree.ExpandAll(tree.root)
    def OnCollapse(self, evt):
        if tree.selection: tree.CollapseAll(tree.selection)
        else: tree.CollapseAll(tree.root)

    def OnPullDownHighlight(self, evt):
        menuId = evt.GetMenuId()
        if menuId != -1:
            menu = evt.GetEventObject()
            help = menu.GetHelpString(menuId)
            self.SetStatusText(help)
        else:
            self.SetStatusText('')

    def OnUpdateUI(self, evt):
        if evt.GetId() in [wxID_CUT, wxID_COPY, self.ID_DELETE]:
            evt.Enable(tree.selection is not None and tree.selection != tree.root)
        elif evt.GetId() == wxID_SAVE:
            evt.Enable(self.modified)
        elif evt.GetId() in [wxID_PASTE, self.ID_TOOL_PASTE]:
            evt.Enable((self.clipboard and tree.selection) != None)
        elif evt.GetId() == self.ID_TEST:
            evt.Enable(tree.selection is not None and tree.selection != tree.root)
        elif evt.GetId() in [self.ID_LOCATE, self.ID_TOOL_LOCATE]:
            evt.Enable(g.testWin is not None)
        elif evt.GetId() == wxID_UNDO:  evt.Enable(undoMan.CanUndo())
        elif evt.GetId() == wxID_REDO:  evt.Enable(undoMan.CanRedo())

    def OnIdle(self, evt):
        if self.inIdle: return          # Recursive call protection
        self.inIdle = True
        if tree.needUpdate:
            if conf.autoRefresh:
                if g.testWin:
                    self.SetStatusText('Refreshing test window...')
                    # (re)create
                    tree.CreateTestWin(g.testWin.item)
                    wxYield()
                    self.SetStatusText('')
                tree.needUpdate = False
        elif tree.pendingHighLight:
            tree.HighLight(tree.pendingHighLight)
        else:
            evt.Skip()
        self.inIdle = False

    # We don't let close panel window
    def OnCloseMiniFrame(self, evt):
        return

    def OnCloseWindow(self, evt):
        if not self.AskSave(): return
        if g.testWin: g.testWin.Destroy()
        if not panel.GetPageCount() == 2:
            panel.page2.Destroy()
        else:
            # If we don't do this, page does not get destroyed (a bug?)
            panel.RemovePage(1)
        if not self.IsIconized():
            conf.x, conf.y = self.GetPosition()
            conf.width, conf.height = self.GetSize()
            if conf.embedPanel:
                conf.sashPos = self.splitter.GetSashPosition()
            else:
                conf.panelX, conf.panelY = self.miniFrame.GetPosition()
                conf.panelWidth, conf.panelHeight = self.miniFrame.GetSize()
        evt.Skip()

    def Clear(self):
        self.dataFile = ''
        if self.clipboard:
            self.clipboard.unlink()
            self.clipboard = None
        undoMan.Clear()
        self.SetModified(False)
        tree.Clear()
        panel.Clear()
        if g.testWin:
            g.testWin.Destroy()
            g.testWin = None
        # Numbers for new controls
        self.maxIDs = {}
        self.maxIDs[xxxPanel] = self.maxIDs[xxxDialog] = self.maxIDs[xxxFrame] = \
        self.maxIDs[xxxMenuBar] = self.maxIDs[xxxMenu] = self.maxIDs[xxxToolBar] = \
        self.maxIDs[xxxWizard] = 0

    def SetModified(self, state=True):
        self.modified = state
        name = os.path.basename(self.dataFile)
        if not name: name = defaultName
        if state:
            self.SetTitle(progname + ': ' + name + ' *')
        else:
            self.SetTitle(progname + ': ' + name)

    def Open(self, path):
        if not os.path.exists(path):
            wxLogError('File does not exists: %s' % path)
            return False
        # Try to read the file
        try:
            f = open(path)
            self.Clear()
            dom = minidom.parse(f)
            f.close()
            # Set encoding global variable and default encoding
            if dom.encoding:
                g.currentEncoding = dom.encoding
                wx.SetDefaultPyEncoding(g.currentEncoding.encode())
            else:
                g.currentEncoding = ''
            # Change dir
            self.dataFile = path = os.path.abspath(path)
            dir = os.path.dirname(path)
            if dir: os.chdir(dir)
            tree.SetData(dom)
            self.SetTitle(progname + ': ' + os.path.basename(path))
        except:
            # Nice exception printing
            inf = sys.exc_info()
            wxLogError(traceback.format_exception(inf[0], inf[1], None)[-1])
            wxLogError('Error reading file: %s' % path)
            if debug: raise
            return False
        return True

    def Indent(self, node, indent = 0):
        # Copy child list because it will change soon
        children = node.childNodes[:]
        # Main node doesn't need to be indented
        if indent:
            text = self.domCopy.createTextNode('\n' + ' ' * indent)
            node.parentNode.insertBefore(text, node)
        if children:
            # Append newline after last child, except for text nodes
            if children[-1].nodeType == minidom.Node.ELEMENT_NODE:
                text = self.domCopy.createTextNode('\n' + ' ' * indent)
                node.appendChild(text)
            # Indent children which are elements
            for n in children:
                if n.nodeType == minidom.Node.ELEMENT_NODE:
                    self.Indent(n, indent + 2)

    def Save(self, path):
        try:
            import codecs
            # Apply changes
            if tree.selection and panel.IsModified():
                self.OnRefresh(wxCommandEvent())
            if g.currentEncoding:
                f = codecs.open(path, 'wt', g.currentEncoding)
            else:
                f = codecs.open(path, 'wt')
            # Make temporary copy for formatting it
            # !!! We can't clone dom node, it works only once
            #self.domCopy = tree.dom.cloneNode(True)
            self.domCopy = MyDocument()
            mainNode = self.domCopy.appendChild(tree.mainNode.cloneNode(True))
            self.Indent(mainNode)
            self.domCopy.writexml(f, encoding = g.currentEncoding)
            f.close()
            self.domCopy.unlink()
            self.domCopy = None
            self.SetModified(False)
            panel.SetModified(False)
        except:
            inf = sys.exc_info()
            wxLogError(traceback.format_exception(inf[0], inf[1], None)[-1])
            wxLogError('Error writing file: %s' % path)
            raise

    def AskSave(self):
        if not (self.modified or panel.IsModified()): return True
        flags = wxICON_EXCLAMATION | wxYES_NO | wxCANCEL | wxCENTRE
        dlg = wxMessageDialog( self, 'File is modified. Save before exit?',
                               'Save before too late?', flags )
        say = dlg.ShowModal()
        dlg.Destroy()
        if say == wxID_YES:
            self.OnSaveOrSaveAs(wxCommandEvent(wxID_SAVE))
            # If save was successful, modified flag is unset
            if not self.modified: return True
        elif say == wxID_NO:
            self.SetModified(False)
            panel.SetModified(False)
            return True
        return False

    def SaveUndo(self):
        pass                            # !!!

################################################################################

def usage():
    print >> sys.stderr, 'usage: xrced [-dhiv] [file]'

class App(wxApp):
    def OnInit(self):
        global debug
        # Process comand-line
        opts = args = None
        try:
            opts, args = getopt.getopt(sys.argv[1:], 'dhiv')
            for o,a in opts:
                if o == '-h':
                    usage()
                    sys.exit(0)
                elif o == '-d':
                    debug = True
                elif o == '-v':
                    print 'XRCed version', version
                    sys.exit(0)
            
        except getopt.GetoptError:
            if wxPlatform != '__WXMAC__': # macs have some extra parameters
                print >> sys.stderr, 'Unknown option'
                usage()
                sys.exit(1)

        self.SetAppName('xrced')
        # Settings
        global conf
        conf = g.conf = wxConfig(style = wxCONFIG_USE_LOCAL_FILE)
        conf.autoRefresh = conf.ReadInt('autorefresh', True)
        pos = conf.ReadInt('x', -1), conf.ReadInt('y', -1)
        size = conf.ReadInt('width', 800), conf.ReadInt('height', 600)
        conf.embedPanel = conf.ReadInt('embedPanel', True)
        conf.showTools = conf.ReadInt('showTools', True)
        conf.sashPos = conf.ReadInt('sashPos', 200)
        # read recently used files
        recentfiles=conf.Read('recentFiles','')
        conf.recentfiles={}
        if recentfiles:
            for fil in recentfiles.split('|'):
                conf.recentfiles[wxNewId()]=fil
        if not conf.embedPanel:
            conf.panelX = conf.ReadInt('panelX', -1)
            conf.panelY = conf.ReadInt('panelY', -1)
        else:
            conf.panelX = conf.panelY = -1
        conf.panelWidth = conf.ReadInt('panelWidth', 200)
        conf.panelHeight = conf.ReadInt('panelHeight', 200)
        conf.panic = not conf.HasEntry('nopanic')
        # Add handlers
        wxFileSystem_AddHandler(wxMemoryFSHandler())
        wxInitAllImageHandlers()
        # Create main frame
        frame = Frame(pos, size)
        frame.Show(True)

        # Load file after showing
        if args:
            conf.panic = False
            frame.open = frame.Open(args[0])

        return True

    def OnExit(self):
        # Write config
        global conf
        wc = wxConfigBase_Get()
        wc.WriteInt('autorefresh', conf.autoRefresh)
        wc.WriteInt('x', conf.x)
        wc.WriteInt('y', conf.y)
        wc.WriteInt('width', conf.width)
        wc.WriteInt('height', conf.height)
        wc.WriteInt('embedPanel', conf.embedPanel)
        wc.WriteInt('showTools', conf.showTools)
        if not conf.embedPanel:
            wc.WriteInt('panelX', conf.panelX)
            wc.WriteInt('panelY', conf.panelY)
        wc.WriteInt('sashPos', conf.sashPos)
        wc.WriteInt('panelWidth', conf.panelWidth)
        wc.WriteInt('panelHeight', conf.panelHeight)
        wc.WriteInt('nopanic', True)
        wc.Write('recentFiles', '|'.join(conf.recentfiles.values()[-5:]))
        wc.Flush()

def main():
    app = App(0, useBestVisual=False)
    #app.SetAssertMode(wxPYAPP_ASSERT_LOG)
    app.MainLoop()
    app.OnExit()
    global conf
    del conf

if __name__ == '__main__':
    main()
