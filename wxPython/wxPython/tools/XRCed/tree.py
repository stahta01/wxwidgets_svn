# Name:         tree.py
# Purpose:      XRC editor, XML_tree class
# Author:       Roman Rolinsky <rolinsky@mema.ucl.ac.be>
# Created:      02.12.2002
# RCS-ID:       $Id$

from xxx import *                       # xxx imports globals and params

# Icons
import images

class MemoryFile:
    def __init__(self, name):
        self.name = name
        self.buffer = ''
    def write(self, data):
        self.buffer += data.encode()
    def close(self):
        wxMemoryFSHandler_AddFile(self.name, self.buffer)

################################################################################

# Redefine writing to include encoding
class MyDocument(minidom.Document):
    def __init__(self):
        minidom.Document.__init__(self)
        self.encoding = ''
    def writexml(self, writer, indent="", addindent="", newl="", encoding=""):
        if encoding: encdstr = 'encoding="%s"' % encoding
        else: encdstr = ''
        writer.write('<?xml version="1.0" %s?>\n' % encdstr)
        for node in self.childNodes:
            node.writexml(writer, indent, addindent, newl)

################################################################################

class PullDownMenu:
    ID_NEW_PANEL = wxNewId()
    ID_NEW_DIALOG = wxNewId()
    ID_NEW_FRAME = wxNewId()
    ID_NEW_TOOL_BAR = wxNewId()
    ID_NEW_TOOL = wxNewId()
    ID_NEW_MENU_BAR = wxNewId()
    ID_NEW_MENU = wxNewId()

    ID_NEW_STATIC_TEXT = wxNewId()
    ID_NEW_TEXT_CTRL = wxNewId()

    ID_NEW_BUTTON = wxNewId()
    ID_NEW_BITMAP_BUTTON = wxNewId()
    ID_NEW_RADIO_BUTTON = wxNewId()
    ID_NEW_SPIN_BUTTON = wxNewId()

    ID_NEW_STATIC_BOX = wxNewId()
    ID_NEW_CHECK_BOX = wxNewId()
    ID_NEW_RADIO_BOX = wxNewId()
    ID_NEW_COMBO_BOX = wxNewId()
    ID_NEW_LIST_BOX = wxNewId()

    ID_NEW_STATIC_LINE = wxNewId()
    ID_NEW_STATIC_BITMAP = wxNewId()
    ID_NEW_CHOICE = wxNewId()
    ID_NEW_SLIDER = wxNewId()
    ID_NEW_GAUGE = wxNewId()
    ID_NEW_SCROLL_BAR = wxNewId()
    ID_NEW_TREE_CTRL = wxNewId()
    ID_NEW_LIST_CTRL = wxNewId()
    ID_NEW_CHECK_LIST = wxNewId()
    ID_NEW_NOTEBOOK = wxNewId()
    ID_NEW_HTML_WINDOW = wxNewId()
    ID_NEW_CALENDAR_CTRL = wxNewId()
    ID_NEW_GENERIC_DIR_CTRL = wxNewId()
    ID_NEW_SPIN_CTRL = wxNewId()
    ID_NEW_UNKNOWN = wxNewId()

    ID_NEW_BOX_SIZER = wxNewId()
    ID_NEW_STATIC_BOX_SIZER = wxNewId()
    ID_NEW_GRID_SIZER = wxNewId()
    ID_NEW_FLEX_GRID_SIZER = wxNewId()
    ID_NEW_SPACER = wxNewId()
    ID_NEW_TOOL_BAR = wxNewId()
    ID_NEW_TOOL = wxNewId()
    ID_NEW_MENU = wxNewId()
    ID_NEW_MENU_ITEM = wxNewId()
    ID_NEW_SEPARATOR = wxNewId()
    ID_NEW_LAST = wxNewId()
    ID_EXPAND = wxNewId()
    ID_COLLAPSE = wxNewId()
    ID_PASTE_SIBLING = wxNewId()

    def __init__(self, parent):
        self.ID_DELETE = parent.ID_DELETE
        EVT_MENU_RANGE(parent, self.ID_NEW_PANEL,
                       self.ID_NEW_LAST, parent.OnCreate)
        EVT_MENU(parent, self.ID_COLLAPSE, parent.OnCollapse)
        EVT_MENU(parent, self.ID_EXPAND, parent.OnExpand)
        EVT_MENU(parent, self.ID_PASTE_SIBLING, parent.OnPaste)
        # We connect to tree, but process in frame
        EVT_MENU_HIGHLIGHT_ALL(g.tree, parent.OnPullDownHighlight)

################################################################################

# Set menu to list items.
# Each menu command is a tuple (id, label, help)
# submenus are lists [id, label, help, submenu]
# and separators are any other type
def SetMenu(m, list):
    for l in list:
        if type(l) == types.TupleType:
            apply(m.Append, l)
        elif type(l) == types.ListType:
            subMenu = wxMenu()
            SetMenu(subMenu, l[2:])
            m.AppendMenu(wxNewId(), l[0], subMenu, l[1])
        else:                           # separator
            m.AppendSeparator()

################################################################################

class HighLightBox:
    def __init__(self, pos, size):
        if size.x == -1: size.x = 0
        if size.y == -1: size.y = 0
        w = g.testWin.panel
        l1 = wxWindow(w, -1, pos, wxSize(size.x, 2))
        l1.SetBackgroundColour(wxRED)
        l2 = wxWindow(w, -1, pos, wxSize(2, size.y))
        l2.SetBackgroundColour(wxRED)
        l3 = wxWindow(w, -1, wxPoint(pos.x + size.x - 2, pos.y), wxSize(2, size.y))
        l3.SetBackgroundColour(wxRED)
        l4 = wxWindow(w, -1, wxPoint(pos.x, pos.y + size.y - 2), wxSize(size.x, 2))
        l4.SetBackgroundColour(wxRED)
        self.lines = [l1, l2, l3, l4]
    # Move highlight to a new position
    def Replace(self, pos, size):
        if size.x == -1: size.x = 0
        if size.y == -1: size.y = 0
        self.lines[0].SetDimensions(pos.x, pos.y, size.x, 2)
        self.lines[1].SetDimensions(pos.x, pos.y, 2, size.y)
        self.lines[2].SetDimensions(pos.x + size.x - 2, pos.y, 2, size.y)
        self.lines[3].SetDimensions(pos.x, pos.y + size.y - 2, size.x, 2)
    # Remove it
    def Remove(self):
        map(wxWindow.Destroy, self.lines)
        g.testWin.highLight = None

################################################################################

class XML_Tree(wxTreeCtrl):
    # Constant to define standart window name
    stdName = '_XRCED_T_W'
    def __init__(self, parent, id):
        wxTreeCtrl.__init__(self, parent, id, style = wxTR_HAS_BUTTONS)
        self.SetBackgroundColour(wxColour(224, 248, 224))
        EVT_TREE_SEL_CHANGED(self, self.GetId(), self.OnSelChanged)
        # One works on Linux, another on Windows
        if wxPlatform == '__WXGTK__':
            EVT_TREE_ITEM_ACTIVATED(self, self.GetId(), self.OnItemActivated)
        else:
            EVT_LEFT_DCLICK(self, self.OnDClick)
        EVT_RIGHT_DOWN(self, self.OnRightDown)

        self.needUpdate = False
        self.pendingHighLight = None
        self.ctrl = self.shift = False
        self.dom = None
        # Create image list
        il = wxImageList(16, 16, True)
        self.rootImage = il.AddIcon(images.getTreeRootIcon())
        xxxObject.image = il.AddIcon(images.getTreeDefaultIcon())
        xxxPanel.image = il.AddIcon(images.getTreePanelIcon())
        xxxDialog.image = il.AddIcon(images.getTreeDialogIcon())
        xxxFrame.image = il.AddIcon(images.getTreeFrameIcon())
        xxxMenuBar.image = il.AddIcon(images.getTreeMenuBarIcon())
        xxxToolBar.image = il.AddIcon(images.getTreeToolBarIcon())
        xxxMenu.image = il.AddIcon(images.getTreeMenuIcon())
        xxxSizer.imageH = il.AddIcon(images.getTreeSizerHIcon())
        xxxSizer.imageV = il.AddIcon(images.getTreeSizerVIcon())
        xxxStaticBoxSizer.imageH = il.AddIcon(images.getTreeStaticBoxSizerHIcon())
        xxxStaticBoxSizer.imageV = il.AddIcon(images.getTreeStaticBoxSizerVIcon())
        xxxGridSizer.image = il.AddIcon(images.getTreeSizerGridIcon())
        xxxFlexGridSizer.image = il.AddIcon(images.getTreeSizerFlexGridIcon())
        self.il = il
        self.SetImageList(il)

    def Unselect(self):
        self.selection = None
        wxTreeCtrl.Unselect(self)

    def ExpandAll(self, item):
        if self.ItemHasChildren(item):
            self.Expand(item)
            i, cookie = self.GetFirstChild(item, 0)
            children = []
            while i.IsOk():
                children.append(i)
                i, cookie = self.GetNextChild(item, cookie)
            for i in children:
                self.ExpandAll(i)
    def CollapseAll(self, item):
        if self.ItemHasChildren(item):
            i, cookie = self.GetFirstChild(item, 0)
            children = []
            while i.IsOk():
                children.append(i)
                i, cookie = self.GetNextChild(item, cookie)
            for i in children:
                self.CollapseAll(i)
            self.Collapse(item)

    # Clear tree
    def Clear(self):
        self.DeleteAllItems()
        # Add minimal structure
        if self.dom: self.dom.unlink()
        self.dom = MyDocument()
        self.dummyNode = self.dom.createComment('dummy node')
        # Create main node
        self.mainNode = self.dom.createElement('resource')
        self.dom.appendChild(self.mainNode)
        self.rootObj = xxxMainNode(self.dom)
        self.root = self.AddRoot('XML tree', self.rootImage,
                                 data=wxTreeItemData(self.rootObj))
        self.SetItemHasChildren(self.root)
        self.Expand(self.root)
        self.Unselect()

    # Clear old data and set new
    def SetData(self, dom):
        self.DeleteAllItems()
        # Add minimal structure
        if self.dom: self.dom.unlink()
        self.dom = dom
        self.dummyNode = self.dom.createComment('dummy node')
        # Find 'resource' child, add it's children
        self.mainNode = dom.documentElement
        self.rootObj = xxxMainNode(self.dom)
        self.root = self.AddRoot('XML tree', self.rootImage,
                                 data=wxTreeItemData(self.rootObj))
        self.SetItemHasChildren(self.root)
        nodes = self.mainNode.childNodes[:]
        for node in nodes:
            if IsObject(node):
                self.AddNode(self.root, None, node)
            else:
                self.mainNode.removeChild(node)
                node.unlink()
        self.Expand(self.root)
        self.Unselect()

    # Add tree item for given parent item if node is DOM element node with
    # 'object' tag. xxxParent is parent xxx object
    def AddNode(self, itemParent, xxxParent, node):
        # Set item data to current node
        try:
            xxx = MakeXXXFromDOM(xxxParent, node)
        except:
            print 'ERROR: MakeXXXFromDom(%s, %s)' % (xxxParent, node)
            raise
        treeObj = xxx.treeObject()
        # Append tree item
        item = self.AppendItem(itemParent, treeObj.treeName(),
                               image=treeObj.treeImage(),
                               data=wxTreeItemData(xxx))
        # Try to find children objects
        if treeObj.hasChildren:
            nodes = treeObj.element.childNodes[:]
            for n in nodes:
                if IsObject(n):
                    self.AddNode(item, treeObj, n)
                elif n.nodeType != minidom.Node.ELEMENT_NODE:
                    treeObj.element.removeChild(n)
                    n.unlink()
    # Insert new item at specific position
    def InsertNode(self, itemParent, parent, elem, nextItem):
        # Insert in XML tree and wxWin
        xxx = MakeXXXFromDOM(parent, elem)
        # If nextItem is None, we append to parent, otherwise insert before it
        if nextItem.IsOk():
            node = self.GetPyData(nextItem).element
            parent.element.insertBefore(elem, node)
            # Inserting before is difficult, se we insert after or first child
            index = self.ItemIndex(nextItem)
            newItem = self.InsertItemBefore(itemParent, index,
                        xxx.treeName(), image=xxx.treeImage())
            self.SetPyData(newItem, xxx)
        else:
            parent.element.appendChild(elem)
            newItem = self.AppendItem(itemParent, xxx.treeName(), image=xxx.treeImage(),
                                      data=wxTreeItemData(xxx))
        # Add children items
        if xxx.hasChildren:
            treeObj = xxx.treeObject()
            for n in treeObj.element.childNodes:
                if IsObject(n):
                    self.AddNode(newItem, treeObj, n)
        return newItem

    # Remove leaf of tree, return it's data object
    def RemoveLeaf(self, leaf):
        xxx = self.GetPyData(leaf)
        node = xxx.element
        parent = node.parentNode
        parent.removeChild(node)
        self.Delete(leaf)
        # Reset selection object
        self.selection = None
        return node
    # Find position relative to the top-level window
    def FindNodePos(self, item):
        # Root at (0,0)
        if item == g.testWin.item: return wxPoint(0, 0)
        itemParent = self.GetItemParent(item)
        # Select NB page
        obj = self.FindNodeObject(item)
        if self.GetPyData(itemParent).treeObject().__class__ == xxxNotebook:
            notebook = self.FindNodeObject(itemParent)
            # Find position
            for i in range(notebook.GetPageCount()):
                if notebook.GetPage(i) == obj:
                    if notebook.GetSelection() != i: notebook.SetSelection(i)
                    break
        # Find first ancestor which is a wxWindow (not a sizer)
        winParent = itemParent
        while self.GetPyData(winParent).isSizer:
            winParent = self.GetItemParent(winParent)
        parentPos = self.FindNodePos(winParent)
        # Position (-1,-1) is really (0,0)
        pos = obj.GetPosition()
        if pos == (-1,-1): pos = (0,0)
        return parentPos + pos
    # Find window (or sizer) corresponding to a tree item.
    def FindNodeObject(self, item):
        testWin = g.testWin
        # If top-level, return testWin (or panel its panel)
        if item == testWin.item: return testWin.panel
        itemParent = self.GetItemParent(item)
        xxx = self.GetPyData(item).treeObject()
        parentWin = self.FindNodeObject(itemParent)
        # Top-level sizer? return window's sizer
        if xxx.isSizer and isinstance(parentWin, wxWindowPtr):
            return parentWin.GetSizer()
        # Otherwise get parent's object and it's child
        child = parentWin.GetChildren()[self.ItemIndex(item)]
        # Return window or sizer for sizer items
        if child.GetClassName() == 'wxSizerItem':
            if child.IsWindow(): child = child.GetWindow()
            elif child.IsSizer():
                child = child.GetSizer()
                # Test for notebook sizers
                if isinstance(child, wxNotebookSizerPtr):
                    child = child.GetNotebook()
        return child
    def OnSelChanged(self, evt):
        # Apply changes
        # !!! problem with wxGTK - GetOldItem is Ok if nothing selected
        #oldItem = evt.GetOldItem()
        status = ''
        oldItem = self.selection
        if oldItem:
            xxx = self.GetPyData(oldItem)
            # If some data was modified, apply changes
            if g.panel.IsModified():
                self.Apply(xxx, oldItem)
                #if conf.autoRefresh:
                if g.testWin:
                    if g.testWin.highLight:
                        g.testWin.highLight.Remove()
                    self.needUpdate = True
                status = 'Changes were applied'
        g.frame.SetStatusText(status)
        # Generate view
        self.selection = evt.GetItem()
        if not self.selection.IsOk():
            self.selection = None
            return
        xxx = self.GetPyData(self.selection)
        # Update panel
        g.panel.SetData(xxx)
        # Hightlighting is done in OnIdle
        self.pendingHighLight = self.selection
    # Check if item is in testWin subtree
    def IsHighlatable(self, item):
        if item == g.testWin.item: return False
        while item != self.root:
            item = self.GetItemParent(item)
            if item == g.testWin.item: return True
        return False
    # Highlight selected item
    def HighLight(self, item):
        self.pendingHighLight = None
        # Can highlight only with some top-level windows
        if not g.testWin or self.GetPyData(g.testWin.item).treeObject().__class__ \
            not in [xxxDialog, xxxPanel, xxxFrame]:
            return
        # If a control from another window is selected, remove highlight
        if not self.IsHighlatable(item):
            if g.testWin.highLight: g.testWin.highLight.Remove()
            return
        # Get window/sizer object
        obj, pos = self.FindNodeObject(item), self.FindNodePos(item)
        size = obj.GetSize()
        # Highlight
        # Nagative positions are not working wuite well
        if g.testWin.highLight:
            g.testWin.highLight.Replace(pos, size)
        else:
            g.testWin.highLight = HighLightBox(pos, size)
        g.testWin.highLight.item = item
    def ShowTestWindow(self, item):
        xxx = self.GetPyData(item)
        if g.panel.IsModified():
            self.Apply(xxx, item)       # apply changes
        treeObj = xxx.treeObject()
        if treeObj.className not in ['wxFrame', 'wxPanel', 'wxDialog',
                                     'wxMenuBar', 'wxToolBar']:
            wxLogMessage('No view for this element (yet)')
            return
        # Show item in bold
        if g.testWin:     # Reset old
            self.SetItemBold(g.testWin.item, False)
        self.CreateTestWin(item)
        # Maybe an error occured, so we need to test
        if g.testWin: self.SetItemBold(g.testWin.item)
    # Double-click on Linux
    def OnItemActivated(self, evt):
        if evt.GetItem() != self.root:
            self.ShowTestWindow(evt.GetItem())
    # Double-click on Windows
    def OnDClick(self, evt):
        item, flags = self.HitTest(evt.GetPosition())
        if flags in [wxTREE_HITTEST_ONITEMBUTTON, wxTREE_HITTEST_ONITEMLABEL]:
            if item != self.root: self.ShowTestWindow(item)
        else:
            evt.Skip()
    # (re)create test window
    def CreateTestWin(self, item):
        testWin = g.testWin
        wxBeginBusyCursor()
        # Create a window with this resource
        xxx = self.GetPyData(item).treeObject()
        # Close old window, remember where it was
        highLight = None
        if testWin:
            pos = testWin.GetPosition()
            if item == testWin.item:
                # Remember highlight if same top-level window
                if testWin.highLight:
                    highLight = testWin.highLight.item
                if xxx.className == 'wxPanel':
                    if testWin.highLight:
                        testWin.pendingHighLight = highLight
                        testWin.highLight.Remove()
                    testWin.panel.Destroy()
                    testWin.panel = None
                else:
                    testWin.Destroy()
                    testWin = g.testWin = None
            else:
                testWin.Destroy()
                testWin = g.testWin = None
        else:
            pos = g.testWinPos
        # Save in memory FS
        memFile = MemoryFile('xxx.xrc')
        # Create partial XML file - faster for big files

        dom = MyDocument()
        mainNode = dom.createElement('resource')
        dom.appendChild(mainNode)

        # Remove temporarily from old parent
        elem = xxx.element
        # Change window id to _XRCED_T_W. This gives some name for
        # unnamed windows, and for named gives the possibility to
        # write sawfish scripts.
        if not xxx.name:
            name = 'noname'
        else:
            name = xxx.name
        elem.setAttribute('name', self.stdName)
        parent = elem.parentNode
        next = elem.nextSibling
        parent.replaceChild(self.dummyNode, elem)
        # Append to new DOM, write it
        mainNode.appendChild(elem)
        dom.writexml(memFile, encoding=self.rootObj.params['encoding'].value())
        # Put back in place
        mainNode.removeChild(elem)
        dom.unlink()
        parent.replaceChild(elem, self.dummyNode)
        # Remove temporary name or restore changed
        if not xxx.name:
            elem.removeAttribute('name')
        else:
            elem.setAttribute('name', xxx.name)
        memFile.close()                 # write to wxMemoryFS
        res = wxXmlResource('', g.xmlFlags)
        res.Load('memory:xxx.xrc')
        if xxx.__class__ == xxxFrame:
            # Frame can't have many children,
            # but it's first child possibly can...
            child = self.GetFirstChild(item, 0)[0]
            if child.IsOk() and self.GetPyData(child).__class__ == xxxPanel:
                # Clean-up before recursive call or error
                wxMemoryFSHandler_RemoveFile('xxx.xrc')
                wxEndBusyCursor()
                self.CreateTestWin(child)
                return
            # This currently works under GTK, but not under MSW
            testWin = g.testWin = wxPreFrame()
            res.LoadOnFrame(testWin, g.frame, self.stdName)
            # Create status bar
            testWin.CreateStatusBar()
            testWin.panel = testWin
            testWin.SetPosition(pos)
            testWin.Show(True)
        elif xxx.__class__ == xxxPanel:
            # Create new frame
            if not testWin:
                testWin = g.testWin = wxFrame(g.frame, -1, 'Panel: ' + name, pos=pos)
            testWin.panel = res.LoadPanel(testWin, self.stdName)
            testWin.SetClientSize(testWin.panel.GetSize())
            testWin.Show(True)
        elif xxx.__class__ == xxxDialog:
            testWin = g.testWin = res.LoadDialog(None, self.stdName)
            testWin.panel = testWin
            testWin.Layout()
            testWin.SetPosition(pos)
            testWin.Show(True)
        elif xxx.__class__ == xxxMenuBar:
            testWin = g.testWin = wxFrame(g.frame, -1, 'MenuBar: ' + name, pos=pos)
            testWin.panel = None
            # Set status bar to display help
            testWin.CreateStatusBar()
            testWin.menuBar = res.LoadMenuBar(self.stdName)
            testWin.SetMenuBar(testWin.menuBar)
            testWin.Show(True)
        elif xxx.__class__ == xxxToolBar:
            testWin = g.testWin = wxFrame(g.frame, -1, 'ToolBar: ' + name, pos=pos)
            testWin.panel = None
            # Set status bar to display help
            testWin.CreateStatusBar()
            testWin.toolBar = res.LoadToolBar(testWin, self.stdName)
            testWin.SetToolBar(testWin.toolBar)
            testWin.Show(True)
        wxMemoryFSHandler_RemoveFile('xxx.xrc')
        testWin.item = item
        EVT_CLOSE(testWin, self.OnCloseTestWin)
        EVT_BUTTON(testWin, wxID_OK, self.OnCloseTestWin)
        EVT_BUTTON(testWin, wxID_CANCEL, self.OnCloseTestWin)
        testWin.highLight = None
        if highLight and not self.pendingHighLight:
            self.HighLight(highLight)
        wxEndBusyCursor()

    def OnCloseTestWin(self, evt):
        self.SetItemBold(g.testWin.item, False)
        g.testWinPos = g.testWin.GetPosition()
        g.testWin.Destroy()
        g.testWin = None

    # Return item index in parent
    def ItemIndex(self, item):
        n = 0                           # index of sibling
        prev = self.GetPrevSibling(item)
        while prev.IsOk():
            prev = self.GetPrevSibling(prev)
            n += 1
        return n

    # Full tree index of an item - list of positions
    def ItemFullIndex(self, item):
        if not item.IsOk(): return None
        l = []
        while item != self.root:
            l.insert(0, self.ItemIndex(item))
            item = self.GetItemParent(item)
        return l
    # Get item position from full index
    def ItemAtFullIndex(self, index):
        if index is None: return wxTreeItemId()
        item = self.root
        for i in index:
            item = self.GetFirstChild(item, 0)[0]
            for k in range(i): item = self.GetNextSibling(item)
        return item

    # True if next item should be inserted after current (vs. appended to it)
    def NeedInsert(self, item):
        xxx = self.GetPyData(item)
        if item == self.root: return False        # root item
        if xxx.hasChildren and not self.GetChildrenCount(item, False):
            return False
        return not (self.IsExpanded(item) and self.GetChildrenCount(item, False))

    # Pull-down
    def OnRightDown(self, evt):
        pullDownMenu = g.pullDownMenu
        # select this item
        pt = evt.GetPosition();
        item, flags = self.HitTest(pt)
        if item.Ok() and flags & wxTREE_HITTEST_ONITEM:
            self.SelectItem(item)

        # Setup menu
        menu = wxMenu()

        item = self.selection
        if not item:
            menu.Append(g.pullDownMenu.ID_EXPAND, 'Expand', 'Expand tree')
            menu.Append(g.pullDownMenu.ID_COLLAPSE, 'Collapse', 'Collapse tree')
        else:
            self.ctrl = evt.ControlDown() # save Ctrl state
            self.shift = evt.ShiftDown()  # and Shift too
            m = wxMenu()                  # create menu
            if self.ctrl:
                needInsert = True
            else:
                needInsert = self.NeedInsert(item)
            if item == self.root or needInsert and self.GetItemParent(item) == self.root:
                m.Append(pullDownMenu.ID_NEW_PANEL, 'Panel', 'Create panel')
                m.Append(pullDownMenu.ID_NEW_DIALOG, 'Dialog', 'Create dialog')
                m.Append(pullDownMenu.ID_NEW_FRAME, 'Frame', 'Create frame')
                m.AppendSeparator()
                m.Append(pullDownMenu.ID_NEW_TOOL_BAR, 'ToolBar', 'Create toolbar')
                m.Append(pullDownMenu.ID_NEW_MENU_BAR, 'MenuBar', 'Create menubar')
                m.Append(pullDownMenu.ID_NEW_MENU, 'Menu', 'Create menu')
            else:
                xxx = self.GetPyData(item).treeObject()
                # Check parent for possible child nodes if inserting sibling
                if needInsert: xxx = xxx.parent
                if xxx.__class__ == xxxMenuBar:
                    m.Append(pullDownMenu.ID_NEW_MENU, 'Menu', 'Create menu')
                elif xxx.__class__ in [xxxToolBar, xxxTool] or \
                     xxx.__class__ == xxxSeparator and xxx.parent.__class__ == xxxToolBar:
                    SetMenu(m, pullDownMenu.toolBarControls)
                elif xxx.__class__ in [xxxMenu, xxxMenuItem]:
                    SetMenu(m, pullDownMenu.menuControls)
                else:
                    SetMenu(m, pullDownMenu.controls)
                    if xxx.__class__ == xxxNotebook:
                        m.Enable(m.FindItem('sizer'), False)
                    elif not (xxx.isSizer or xxx.parent and xxx.parent.isSizer):
                        m.Enable(pullDownMenu.ID_NEW_SPACER, False)
            # Select correct label for create menu
            if not needInsert:
                if self.shift:
                    menu.AppendMenu(wxNewId(), 'Insert Child', m,
                                    'Create child object as the first child')
                else:
                    menu.AppendMenu(wxNewId(), 'Append Child', m,
                                    'Create child object as the last child')
            else:
                if self.shift:
                    menu.AppendMenu(wxNewId(), 'Create Sibling', m,
                                    'Create sibling before selected object')
                else:
                    menu.AppendMenu(wxNewId(), 'Create Sibling', m,
                                    'Create sibling after selected object')
            menu.AppendSeparator()
            # Not using standart IDs because we don't want to show shortcuts
            menu.Append(wxID_CUT, 'Cut', 'Cut to the clipboard')
            menu.Append(wxID_COPY, 'Copy', 'Copy to the clipboard')
            if self.ctrl and item != self.root:
                menu.Append(pullDownMenu.ID_PASTE_SIBLING, 'Paste Sibling',
                            'Paste from the clipboard as a sibling')
            else:
                menu.Append(wxID_PASTE, 'Paste', 'Paste from the clipboard')
            menu.Append(pullDownMenu.ID_DELETE,
                                'Delete', 'Delete object')
            if self.ItemHasChildren(item):
                menu.AppendSeparator()
                menu.Append(pullDownMenu.ID_EXPAND, 'Expand', 'Expand subtree')
                menu.Append(pullDownMenu.ID_COLLAPSE, 'Collapse', 'Collapse subtree')
        self.PopupMenu(menu, evt.GetPosition())
        menu.Destroy()

    # Apply changes
    def Apply(self, xxx, item):
        g.panel.Apply()
        # Update tree view
        xxx = xxx.treeObject()
        if xxx.hasName and self.GetItemText(item) != xxx.name:
            self.SetItemText(item, xxx.treeName())
        # Change tree icon for sizers
        if isinstance(xxx, xxxBoxSizer):
            self.SetItemImage(item, xxx.treeImage())
        # Set global modified state
        g.frame.modified = True

