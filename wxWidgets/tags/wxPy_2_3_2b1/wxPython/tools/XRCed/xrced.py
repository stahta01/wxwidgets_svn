# Name:         xrced.py
# Purpose:      XRC editor, main module
# Author:       Roman Rolinsky <rolinsky@mema.ucl.ac.be>
# Created:      20.08.2001

from wxPython.wx import *
from wxPython.xrc import *
from wxPython.html import *
import wxPython.lib.wxpTag
from xml.dom import minidom
import os
import tempfile

import images

# String constants
htmlHeader = '<html><body bgcolor="#b0c4de">\n'
htmlFooter = '</body></html>\n'
progname = 'XRCed'
version = '0.0.3'

# Local modules
from xxx import *

# Globals
testWin = None
testWinPos = wxDefaultPosition

# 1 adds CMD command to Help menu
debug = 1

if debug:
    import traceback
    import time

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

# Properties panel
class Panel(wxHtmlWindow):
    def __init__(self, parent, id):
        wxHtmlWindow.__init__(self, parent, id)
        self.SetBorders(5)
        self.SetFonts('', '', [8, 10, 12, 14, 16, 19, 24])
        EVT_CHECKBOX(self, xxxObject.ID_CHECK_PARAMS, self.OnCheckParams)
        EVT_CHECKBOX(self, xxxChildContainer.ID_CHECK_PARAMS, self.OnCheckParams)
        self.modified = false

    def Clear(self):
        self.SetPage(htmlHeader + 'select a tree item on the left' + htmlFooter)

    def OnCheckParams(self, evt):
        selected = tree.GetSelection()
        xxx = tree.GetPyData(selected)
        if xxx.hasChild and evt.GetId() != xxxChildContainer.ID_CHECK_PARAMS:
            xxx = xxx.child
        # Set current object
        param = evt.GetEventObject().GetName()[6:]
        if xxx.hasChild:
            w = GetRegistered('_'+param)
        else:
            w = GetRegistered(param)
        elem = xxx.element
        if evt.IsChecked():
            # Ad  new text node in order of allParams
            w.SetValue('')
            textElem = tree.dom.createElement(param)
            textNode = tree.dom.createTextNode('')
            textElem.appendChild(textNode)
            # Find place to put new element: first present element after param
            found = false
            for p in xxx.allParams[xxx.allParams.index(param) + 1:]:
                # Content params don't have same type
                if xxx.params.has_key(p) and p != 'content':
                    found = true
                    break
            if found:
                nextTextElem = xxx.params[p].parentNode
                elem.insertBefore(textElem, nextTextElem)
            else:
                elem.appendChild(textElem)
            xxx.params[param] = textNode
        else:
            # Remove parameter element and following text node
            textElem = xxx.params[param].parentNode
            newline = textElem.nextSibling
            if newline and newline.nodeType == minidom.Node.TEXT_NODE:
                elem.removeChild(newline)
            elem.removeChild(textElem)
            del xxx.params[param]
            w.SetValue('')
        w.Enable(evt.IsChecked())
        # Set modified flas
        self.SetModified(true)

    # If some parameter was changed
    def IsModified(self):
        return self.modified
    def SetModified(self, value):
        self.modified = value

class HightLightBox:
    def __init__(self, pos, size):
        w = testWin.panel
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
        self.lines[0].SetDimensions(pos.x, pos.y, size.x, 2, wxSIZE_ALLOW_MINUS_ONE)
        self.lines[1].SetDimensions(pos.x, pos.y, 2, size.y, wxSIZE_ALLOW_MINUS_ONE)
        self.lines[2].SetDimensions(pos.x + size.x - 2, pos.y, 2, size.y,
                                    wxSIZE_ALLOW_MINUS_ONE)
        self.lines[3].SetDimensions(pos.x, pos.y + size.y - 2, size.x, 2,
                                    wxSIZE_ALLOW_MINUS_ONE)
    # Remove it
    def Remove(self):
        map(wxWindow.Destroy, self.lines)
        testWin.highLight = None

class MemoryFile:
    def __init__(self, name):
        self.name = name
        self.buffer = ''
    def write(self, data):
        self.buffer = self.buffer + data.encode()
    def close(self):
        f = open(self.name, 'w')
        f.write(self.buffer)
        f.close()
        # !!! memory FS will work someday
        #self.file = wxMemoryFSHandler_AddFile(self.name, self.buffer)

class XML_Tree(wxTreeCtrl):
    def __init__(self, parent, id):
        wxTreeCtrl.__init__(self, parent, id)
        self.SetBackgroundColour(wxColour(224, 248, 224))
        EVT_TREE_SEL_CHANGED(self, self.GetId(), self.OnSelChanged)
        EVT_TREE_ITEM_ACTIVATED(self, self.GetId(), self.OnItemActivated)
        EVT_RIGHT_DOWN(self, self.OnRightDown)
        self.needUpdate = false
        self.pendingHighLight = None
        self.ctrl = false
        self.dom = None
        # Create image list
        il = wxImageList(16, 16, true)
        xxxPanel.image = il.AddIcon( wxIconFromXPMData(images.getTreePanelData()) )
        xxxDialog.image = il.AddIcon( wxIconFromXPMData(images.getTreeDialogData()) )
        xxxFrame.image = il.AddIcon( wxIconFromXPMData(images.getTreeFrameData()) )
        xxxMenuBar.image = il.AddIcon( wxIconFromXPMData(images.getTreeMenuBarData()) )
        xxxMenu.image = il.AddIcon( wxIconFromXPMData(images.getTreeMenuData()) )
        xxxSizer.imageH = il.AddIcon( wxIconFromXPMData(images.getTreeSizerHData()) )
        xxxSizer.imageV = il.AddIcon( wxIconFromXPMData(images.getTreeSizerVData()) )
        xxxStaticBoxSizer.imageH = il.AddIcon( wxIconFromXPMData(images.getTreeStaticBoxSizerHData()) )
        xxxStaticBoxSizer.imageV = il.AddIcon( wxIconFromXPMData(images.getTreeStaticBoxSizerVData()) )
        xxxGridSizer.image = il.AddIcon( wxIconFromXPMData(images.getTreeSizerGridData()) )
        xxxFlexGridSizer.image = il.AddIcon( wxIconFromXPMData(images.getTreeSizerFlexGridData()) )
        self.il = il
        self.SetImageList(il)

    # !!! temporary solution for GetOldItem problem
    def Unselect(self):
        self.selection = wxTreeItemId()
        wxTreeCtrl.Unselect(self)
    def GetSelection(self):
        return self.selection

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

    def SetData(self, dom):
        self.dom = dom
        # Find 'resource' child, add it's children
        self.mainNode = dom.getElementsByTagName('resource')[0]
        nodes = self.mainNode.childNodes[:]
        for node in nodes:
            if IsObject(node):
                self.AddNode(self.GetRootItem(), None, node)
            else:
                self.mainNode.removeChild(node)
                node.unlink()
        self.Unselect()

    # Add tree item for given parent item if node is DOM element node with
    # 'object' tag. xxxParent is parent xxx object
    def AddNode(self, itemParent, xxxParent, node):
        # Set item data to current node
        xxx = MakeXXXFromDOM(xxxParent, node)
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


    # Remove leaf of tree, return it's data object
    def RemoveLeaf(self, leaf):
        xxx = self.GetPyData(leaf)
        node = xxx.element
        parent = node.parentNode
        parent.removeChild(node)
        self.Delete(leaf)
        # Update view?
        #if testWin and self.GetItemAncestor(leaf) == testWin.item:
        #    if testWin.highLight:
        #        testWin.highLight.Remove()
        #    self.needUpdate = true
        return node

    # Find position relative to the top-level window
    def FindNodePos(self, item):
        itemParent = self.GetItemParent(item)
        if itemParent == self.GetRootItem(): return wxPoint(0, 0)
        obj = self.FindNodeObject(item)
        # Find first ancestor which is a wxWindow (not a sizer)
        winParent = itemParent
        while self.GetPyData(winParent).isSizer:
            winParent = self.GetItemParent(winParent)
        parentPos = self.FindNodePos(winParent)
        return parentPos + obj.GetPosition()

    # Find window (or sizer) corresponding to a tree item.
    def FindNodeObject(self, item):
        itemParent = self.GetItemParent(item)
        # If top-level, return testWin (or panel if wxFrame)
        if itemParent == self.GetRootItem(): return testWin.panel
        xxx = self.GetPyData(item).treeObject()
        parentWin = self.FindNodeObject(itemParent)
        # Top-level sizer? return window's sizer
        if xxx.isSizer and isinstance(parentWin, wxWindowPtr):
            return parentWin.GetSizer()
        # Otherwise get parent's object and it's child
        n = 0                           # index of sibling
        prev = self.GetPrevSibling(item)
        while prev.IsOk():
            prev = self.GetPrevSibling(prev)
            n += 1
        child = parentWin.GetChildren()[n]
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
        # !!! problem with wxGTK
        #oldItem = evt.GetOldItem()
        oldItem = self.selection
        if oldItem.IsOk():
            xxx = self.GetPyData(oldItem)
            # If some data was modified, apply changes
            if xxx:
                if panel.IsModified():
                    self.Apply(xxx, oldItem)
                    #if conf.autoRefresh:
                    if testWin and testWin.highLight:
                        testWin.highLight.Remove()
                    self.needUpdate = true
        # Generate HTML view
        item = evt.GetItem()
        self.selection = item           # !!! fix
        xxx = self.GetPyData(item)
        html = htmlHeader
        # List of parameters tuples (parameter, isDefined)
        if not xxx:                     # root item
            html += 'this item has no properties' + htmlFooter
            panel.SetPage(html)
            if testWin and testWin.highLight:
                testWin.highLight.Remove()
            return
        # Normal nodes
        ClearRegister()                 # empty register
        html += xxx.generateHtml()
        html += htmlFooter
        panel.SetPage(html)
        # Set values, checkboxes to false, disable defaults
        if xxx.hasChild: prefix = '_'
        else: prefix = ''
        for param in xxx.allParams:
            if xxx.params.has_key(param):
                if param == 'content':
                    value = []
                    for text in xxx.params[param]:
                        value.append(str(text.data)) # convert from unicode
                else:
                    value = xxx.params[param].data
                GetRegistered(prefix + param).SetValue(value)
                if not param in xxx.required:
                    panel.FindWindowByName('check_' + param).SetValue(true)
            else:
                GetRegistered(prefix + param).Enable(false)
        # Same for the child of sizeritem
        if xxx.hasChild:
            xxx = xxx.child
            for param in xxx.allParams:
                if xxx.params.has_key(param):
                    if param == 'content':
                        value = []
                        for text in xxx.params[param]:
                            value.append(str(text.data)) # convert from unicode
                    else:
                        value = xxx.params[param].data
                    GetRegistered(param).SetValue(value)
                    if not param in xxx.required:
                        panel.FindWindowByName('check_' + param).SetValue(true)
                else:
                    GetRegistered(param).Enable(false)
        # Clear flag
        panel.SetModified(false)
        # Hightlighting is done in OnIdle
        tree.pendingHighLight = item

    # Find top-level parent
    def GetItemAncestor(self, item):
        while self.GetItemParent(item) != self.GetRootItem():
            item = self.GetItemParent(item)
        return item

    # Highlight selected item
    def HighLight(self, item):
        self.pendingHighLight = None
        if not testWin or self.GetPyData(testWin.item).className \
            not in ['wxDialog', 'wxPanel', 'wxFrame']:
            return
        # Top-level does not have highlight
        if item == testWin.item:
            if testWin.highLight: testWin.highLight.Remove()
            return
        # If a control from another window is selected, remove highlight
        if self.GetItemAncestor(item) != testWin.item and testWin.highLight:
            testWin.highLight.Remove()
            return
        # Get window/sizer object
        obj, pos = self.FindNodeObject(item), self.FindNodePos(item)
        size = obj.GetSize()
        # For notebook, select item's page.
        # For children of page, nothing happens (too much work)
        if isinstance(self.GetPyData(item).parent, xxxNotebook):
            notebook = self.FindNodeObject(self.GetItemParent(item))
            # Find position
            n = 0
            prev = self.GetPrevSibling(item)
            while prev.IsOk():
                n += 1
                prev = self.GetPrevSibling(prev)
            notebook.SetSelection(n)
        # Highlight
        try:                        # finally I use exceptions
            testWin.highLight.Replace(pos, size)
        except AttributeError:
            testWin.highLight = HightLightBox(pos, size)
        testWin.highLight.item = item

    # Double-click
    def OnItemActivated(self, evt):
        item = evt.GetItem()
        xxx = self.GetPyData(item)
        if not xxx: return              # if root selected, do nothing
        if panel.IsModified():
            self.Apply(xxx, item)       # apply changes
        self.CreateTestWin(item)

    # (re)create test window
    def CreateTestWin(self, node):
        global testWin
        # Create a window with this resource
        xxx = self.GetPyData(node).treeObject()
        if not xxx: return            # if root selected, do nothing
        # If noname element, display error
        if not xxx.hasName or not xxx.name:
            wxLogError("Can't display a noname element")
            return
        # Close old window, remember where it was
        highLight = None
        if testWin:
            pos = testWin.GetPosition()
            if node == testWin.item:
                # Remember highlight if same top-level window
                if testWin.highLight:
                    highLight = testWin.highLight.item
                # !!! if 0 is removed, refresh is broken (notebook not deleted?)
                if 0 and xxx.className == 'wxPanel':
                    if testWin.highLight:
                        testWin.pendingHighLight = highLight
                        testWin.highLight.Remove()
                    testWin.panel.Destroy()
                    testWin.panel = None
                else:
                    testWin.Destroy()
                    testWin = None
            else:
                testWin.Destroy()
                testWin = None
        else:
            pos = testWinPos
        # Save in temporary file before activating
        memFile = MemoryFile(tempfile.mktemp('xrc'))
        #memFile = MemoryFile('core.xrc') # to write debug file
        # Create partial XML file - faster for big files

        dom = minidom.Document()
        mainNode = dom.createElement('resource')
        dom.appendChild(mainNode)

        # Remove temporarily from old parent
        elem = xxx.element
        parent = elem.parentNode
        next = elem.nextSibling
        parent.replaceChild(self.dummyNode, elem)
        # Append to new DOM, write it
        mainNode.appendChild(elem)
        dom.writexml(memFile)
        # Put back in place
        mainNode.removeChild(elem)
        dom.unlink()
        parent.replaceChild(elem, self.dummyNode)

        memFile.close()                 # write to wxMemoryFS
        res = wxXmlResource('')
        res.Load(memFile.name)
        if xxx.className == 'wxFrame':
            # Create new frame
            testWin = wxPreFrame()
            res.LoadFrame(testWin, frame, xxx.name)
            testWin.panel = testWin
            testWin.SetPosition(pos)
            testWin.Show(true)
        elif xxx.className == 'wxPanel':
            # Create new frame
            if not testWin:
                testWin = wxFrame(frame, -1, 'Panel: ' + xxx.name, pos=pos)
            testWin.panel = res.LoadPanel(testWin, xxx.name)
            testWin.SetSize(testWin.panel.GetSize())
            testWin.Show(true)
        elif xxx.className == 'wxDialog':
            # Create new frame
            testWin = res.LoadDialog(None, xxx.name)
            testWin.panel = testWin
            testWin.SetPosition(pos)
            testWin.Show(true)
        elif xxx.className == 'wxMenuBar':
            testWin = wxFrame(frame, -1, 'MenuBar: ' + xxx.name, pos=pos)
            # Set status bar to display help
            testWin.CreateStatusBar()
            testWin.menuBar = res.LoadMenuBar(xxx.name)
            testWin.SetMenuBar(testWin.menuBar)
            testWin.Show(true)
        else:
            wxLogMessage('No view for this element yet')
            return
        os.unlink(memFile.name)         # remove tmp file
        testWin.item = node
        testWin.Connect(testWin.GetId(), -1, wxEVT_CLOSE_WINDOW, self.OnCloseTestWin)
        testWin.highLight = None
        if highLight and not tree.pendingHighLight:
            self.HighLight(highLight)

    def OnCloseTestWin(self, evt):
        global testWin, testWinPos
        testWinPos = testWin.GetPosition()
        testWin.Destroy()
        testWin = None
        evt.Skip()

    # True if next item should be inserted after current (vs. appended to it)
    def NeedInsert(self, item):
        xxx = self.GetPyData(item)
        if not xxx: return false        # root item
        if self.ctrl: return true       # if Ctrl pressed, always insert
        if xxx.hasChildren and not self.ItemHasChildren(item):
            return false
        return not (self.IsExpanded(item) and self.ItemHasChildren(item))

    # Pull-down
    def OnRightDown(self, evt):
        # Setup menu
        pullDownMenu.menu = wxMenu()
        item = self.GetSelection()
        if not item.IsOk():
            pullDownMenu.menu.Append(pullDownMenu.ID_EXPAND, 'Expand', 'Expand tree')
        else:
            self.ctrl = evt.ControlDown() # save Ctrl state
            m = wxMenu()                # create menu
            if item != self.GetRootItem(): needInsert = self.NeedInsert(item)
            if item == self.GetRootItem() or \
               self.GetItemParent(item) == self.GetRootItem() and needInsert:
                m.Append(pullDownMenu.ID_NEW_PANEL, 'Panel', 'Create panel')
                m.Append(pullDownMenu.ID_NEW_DIALOG, 'Dialog', 'Create dialog')
                m.Append(pullDownMenu.ID_NEW_FRAME, 'Frame', 'Create frame')
                m.AppendSeparator()
                m.Append(pullDownMenu.ID_NEW_MENU_BAR, 'MenuBar', 'Create menu bar')
                m.Append(pullDownMenu.ID_NEW_MENU, 'Menu', 'Create menu')
            else:
                xxx = self.GetPyData(item)
                if xxx.__class__ == xxxMenuBar:
                    m.Append(pullDownMenu.ID_NEW_MENU, 'Menu', 'Create menu')
                elif xxx.__class__ in [xxxMenu, xxxMenuItem]:
                    SetMenu(m, pullDownMenu.menuControls)
                else:
                    SetMenu(m, pullDownMenu.controls)
                    if not (xxx.isSizer or \
                            xxx.parent and xxx.parent.isSizer):
                        m.Enable(pullDownMenu.ID_NEW_SPACER, false)
            # Select correct label for create menu
            if item == self.GetRootItem():
                pullDownMenu.menu.AppendMenu(wxNewId(), 'Create', m, 'Create top-level object')
            else:
                if not needInsert:
                    pullDownMenu.menu.AppendMenu(wxNewId(), 'Create child', m,
                                                 'Create child object')
                else:
                    pullDownMenu.menu.AppendMenu(wxNewId(), 'Create Sibling', m,
                                                 'Create sibling of selected object')
            pullDownMenu.menu.AppendSeparator()
            pullDownMenu.menu.Append(wxID_CUT, 'Cut', 'Cut to the clipboard')
            pullDownMenu.menu.Append(wxID_COPY, 'Copy', 'Copy to the clipboard')
            pullDownMenu.menu.Append(wxID_PASTE, 'Paste', 'Paste from the clipboard')
            pullDownMenu.menu.Append(pullDownMenu.ID_DELETE,
                                     'Delete', 'Delete object')
            if item.IsOk() and self.ItemHasChildren(item):
                pullDownMenu.menu.AppendSeparator()
                pullDownMenu.menu.Append(pullDownMenu.ID_EXPAND, 'Expand', 'Expand subtree')
        self.PopupMenu(pullDownMenu.menu, evt.GetPosition())
        pullDownMenu.menu.Destroy()
        pullDownMenu.menu = None


    # Clear tree
    def Clear(self):
        self.DeleteAllItems()
        # Add minimal structure
        root = self.AddRoot('XML tree')
        self.Unselect()
        if self.dom: self.dom.unlink()
        self.dom = minidom.Document()
        self.dummyNode = self.dom.createComment('dummy node')
        # Create main node
        self.mainNode = self.dom.createElement('resource')
        self.dom.appendChild(self.mainNode)

    # Apply changes
    def Apply(self, xxx, item):
        if not xxx: return
        # !!! Save undo info
        if xxx.undo: xxx.undo.unlink()
        xxx.undo = xxx.element.cloneNode(false)
        if xxx.hasName:
            name = GetRegistered('name').GetValue()
            if xxx.name != name:
                xxx.name = name
                xxx.element.setAttribute('name', name)
                self.SetItemText(item, xxx.treeName())
        if xxx.hasChild: prefix = '_'
        else: prefix = ''
        for param, data in xxx.params.items():
            value = GetRegistered(prefix + param).GetValue()
            if param == 'content':
                # If number if items is not the same, recreate children
                if len(value) != len(data):
                    elem = xxx.element.getElementsByTagName('content')[0]
                    for n in elem.childNodes:
                        elem.removeChild(n)
                    data = []
                    for str in value:
                        itemElem = tree.dom.createElement('item')
                        itemText = tree.dom.createTextNode(str)
                        itemElem.appendChild(itemText)
                        elem.appendChild(itemElem)
                        data.append(itemText)
                    xxx.params[param] = data
                else:
                    for i in range(len(value)):
                        data[i].data = value[i]
            else:
                data.data = value
        if xxx.hasChild:
            self.Apply(xxx.child, item)
        else:
            # Change tree icon for sizers
            if isinstance(xxx, xxxBoxSizer):
                self.SetItemImage(item, xxx.treeImage())
            # Set global modified state
            frame.modified = true

class Frame(wxFrame):
    def __init__(self, size):
        wxFrame.__init__(self, None, -1, '', size=size)
        self.CreateStatusBar()

        # Make menus
        menuBar = wxMenuBar()

        menu = wxMenu()
        menu.Append(wxID_NEW, '&New\tCtrl-N', 'New file')
        menu.Append(wxID_OPEN, '&Open...\tCtrl-O', 'Open XRC file')
        menu.Append(wxID_SAVE, '&Save\tCtrl-S', 'Save XRC file')
        menu.Append(wxID_SAVEAS, 'Save &As...', 'Save XRC file under different name')
        menu.AppendSeparator()
        menu.Append(wxID_EXIT, '&Quit\tCtrl-Q', 'Exit application')
        menuBar.Append(menu, '&File')

        menu = wxMenu()
        menu.Append(wxID_UNDO, '&Undo\tCtrl-Z', 'Undo')
        menu.Append(wxID_REDO, '&Redo\tCtrl-R', 'Redo')
        menu.AppendSeparator()
        menu.Append(wxID_CUT, 'Cut\tCtrl-X', 'Cut to the clipboard')
        menu.Append(wxID_COPY, '&Copy\tCtrl-C', 'Copy to the clipboard')
        menu.Append(wxID_PASTE, '&Paste\tCtrl-V', 'Paste from the clipboard')
        self.ID_DELETE = wxNewId()
        menu.Append(self.ID_DELETE, '&Delete\tCtrl-D', 'Delete object')
        menuBar.Append(menu, '&Edit')

        menu = wxMenu()
        self.ID_REFRESH = wxNewId()
        menu.Append(self.ID_REFRESH, '&Refresh\tCtrl-R', 'Refresh view')
        self.ID_AUTO_REFRESH = wxNewId()
        menu.Append(self.ID_AUTO_REFRESH, '&Auto-refresh\tCtrl-A',
                    'Toggle auto-refresh mode', true)
        menu.Check(self.ID_AUTO_REFRESH, conf.autoRefresh)
        menuBar.Append(menu, '&View')

        menu = wxMenu()
        menu.Append(wxID_ABOUT, 'About...', 'About XCRed')
        if debug:
            self.ID_DEBUG_CMD = wxNewId()
            menu.Append(self.ID_DEBUG_CMD, 'CMD', 'Python command line')
            EVT_MENU(self, self.ID_DEBUG_CMD, self.OnDebugCMD)
        menuBar.Append(menu, '&Help')

        self.menuBar = menuBar
        self.SetMenuBar(menuBar)

        # Create toolbar
        tb = self.CreateToolBar()#wxTB_DOCKABLE | wxTB_FLAT)
        tb.SetToolBitmapSize((24,23))
        tb.AddSimpleTool(wxID_NEW, images.getNewBitmap(), 'New', 'New file')
        tb.AddSimpleTool(wxID_OPEN, images.getOpenBitmap(), 'Open', 'Open file')
        tb.AddSimpleTool(wxID_SAVE, images.getSaveBitmap(), 'Save', 'Save file')
        tb.AddSeparator()
        tb.AddSimpleTool(wxID_CUT, images.getCutBitmap(), 'Cut', 'Cut')
        tb.AddSimpleTool(wxID_COPY, images.getCopyBitmap(), 'Copy', 'Copy')
        tb.AddSimpleTool(wxID_PASTE, images.getPasteBitmap(), 'Paste', 'Paste')
        tb.AddSeparator()
        tb.AddSimpleTool(self.ID_REFRESH, images.getRefreshBitmap(),
                         'Refresh', 'Refresh view')
        tb.AddSimpleTool(self.ID_AUTO_REFRESH, images.getAutoRefreshBitmap(),
                         'Auto-refresh', 'Toggle auto-refresh mode', true)
        tb.ToggleTool(self.ID_AUTO_REFRESH, conf.autoRefresh)
        self.tb = tb
        tb.Realize()

        # File
        EVT_MENU(self, wxID_NEW, self.OnNew)
        EVT_MENU(self, wxID_OPEN, self.OnOpen)
        EVT_MENU(self, wxID_SAVE, self.OnSaveOrSaveAs)
        EVT_MENU(self, wxID_SAVEAS, self.OnSaveOrSaveAs)
        EVT_MENU(self, wxID_EXIT, self.OnExit)
        # Edit
        EVT_MENU(self, wxID_UNDO, self.OnUndo)
        EVT_MENU(self, wxID_REDO, self.OnRedo)
        EVT_MENU(self, wxID_CUT, self.OnCut)
        EVT_MENU(self, wxID_COPY, self.OnCopy)
        EVT_MENU(self, wxID_PASTE, self.OnPaste)
        EVT_MENU(self, self.ID_DELETE, self.OnDelete)
        # View
        EVT_MENU(self, self.ID_REFRESH, self.OnRefresh)
        EVT_MENU(self, self.ID_AUTO_REFRESH, self.OnAutoRefresh)
        # Help
        EVT_MENU(self, wxID_ABOUT, self.OnAbout)

        # Update events
        EVT_UPDATE_UI(self, wxID_CUT, self.OnUpdateUI)
        EVT_UPDATE_UI(self, wxID_COPY, self.OnUpdateUI)
        EVT_UPDATE_UI(self, wxID_PASTE, self.OnUpdateUI)
        EVT_UPDATE_UI(self, self.ID_DELETE, self.OnUpdateUI)

        # Build interface
        splitter = wxSplitterWindow(self, -1)
        # Create tree
        global tree
        tree = XML_Tree(splitter, -1)
        sys.modules['xxx'].tree = tree
        # Create panel for parameters
        global panel
        #panel = wxPanel(self, -1)
        # Sizer for static box
        #sizer = wxBoxSizer()
        panel = Panel(splitter, -1)
        sys.modules['params'].panel = panel
        #sizer.Add(panel, 1, wxEXPAND)
        #box = wxStaticBox(panel, -1, 'Parameters')
        #boxSizer = wxStaticBoxSizer(box)
        #boxSizer.Add(wxButton(panel, -1, 'BUTT ON'))
        #sizer.Add(boxSizer, 1, wxEXPAND | wxALL, 10)
        #panel.SetAutoLayout(true)
        #panel.SetSizer(sizer)
        # Set plitter windows
        splitter.SplitVertically(tree, panel, 200)
        #topSizer = wxBoxSizer()
        #topSizer.Add(splitter, 1, wxEXPAND)
        #self.SetAutoLayout(true)
        #self.SetSizer(topSizer)

        # Init pull-down menu data
        class MenuData: pass
        global pullDownMenu
        pullDownMenu = MenuData()
        pullDownMenu.menu = None
        pullDownMenu.ID_NEW_PANEL = wxNewId()
        pullDownMenu.ID_NEW_DIALOG = wxNewId()
        pullDownMenu.ID_NEW_FRAME = wxNewId()
        pullDownMenu.ID_NEW_MENU_BAR = wxNewId()
        pullDownMenu.ID_NEW_MENU = wxNewId()

        pullDownMenu.ID_NEW_STATIC_TEXT = wxNewId()
        pullDownMenu.ID_NEW_TEXT_CTRL = wxNewId()

        pullDownMenu.ID_NEW_BUTTON = wxNewId()
        pullDownMenu.ID_NEW_BITMAP_BUTTON = wxNewId()
        pullDownMenu.ID_NEW_RADIO_BUTTON = wxNewId()
        pullDownMenu.ID_NEW_SPIN_BUTTON = wxNewId()

        pullDownMenu.ID_NEW_STATIC_BOX = wxNewId()
        pullDownMenu.ID_NEW_CHECK_BOX = wxNewId()
        pullDownMenu.ID_NEW_RADIO_BOX = wxNewId()
        pullDownMenu.ID_NEW_COMBO_BOX = wxNewId()
        pullDownMenu.ID_NEW_LIST_BOX = wxNewId()

        pullDownMenu.ID_NEW_STATIC_LINE = wxNewId()
        pullDownMenu.ID_NEW_CHOICE = wxNewId()
        pullDownMenu.ID_NEW_SLIDER = wxNewId()
        pullDownMenu.ID_NEW_GAUGE = wxNewId()
        pullDownMenu.ID_NEW_SCROLL_BAR = wxNewId()
        pullDownMenu.ID_NEW_TREE_CTRL = wxNewId()
        pullDownMenu.ID_NEW_LIST_CTRL = wxNewId()
        pullDownMenu.ID_NEW_CHECK_LIST = wxNewId()
        pullDownMenu.ID_NEW_NOTEBOOK = wxNewId()
        pullDownMenu.ID_NEW_HTML_WINDOW = wxNewId()
        pullDownMenu.ID_NEW_CALENDAR = wxNewId()

        pullDownMenu.ID_NEW_BOX_SIZER = wxNewId()
        pullDownMenu.ID_NEW_STATIC_BOX_SIZER = wxNewId()
        pullDownMenu.ID_NEW_GRID_SIZER = wxNewId()
        pullDownMenu.ID_NEW_FLEX_GRID_SIZER = wxNewId()
        pullDownMenu.ID_NEW_SPACER = wxNewId()
        pullDownMenu.ID_NEW_MENU = wxNewId()
        pullDownMenu.ID_NEW_MENU_ITEM = wxNewId()
        pullDownMenu.ID_NEW_SEPARATOR = wxNewId()
        pullDownMenu.ID_NEW_LAST = wxNewId()
        pullDownMenu.ID_DELETE = self.ID_DELETE
        pullDownMenu.ID_EXPAND = wxNewId()
        EVT_MENU_RANGE(self, pullDownMenu.ID_NEW_PANEL,
                       pullDownMenu.ID_NEW_LAST, self.OnCreate)
        EVT_MENU(self, pullDownMenu.ID_EXPAND, self.OnExpand)
        # We connect to tree, but process in frame
        EVT_MENU_HIGHLIGHT_ALL(tree, self.OnPullDownHighlight)
        # Mapping from IDs to element names
        self.createMap = {
            pullDownMenu.ID_NEW_PANEL: 'wxPanel',
            pullDownMenu.ID_NEW_DIALOG: 'wxDialog',
            pullDownMenu.ID_NEW_FRAME: 'wxFrame',
            pullDownMenu.ID_NEW_MENU_BAR: 'wxMenuBar',
            pullDownMenu.ID_NEW_MENU: 'wxMenu',

            pullDownMenu.ID_NEW_STATIC_TEXT: 'wxStaticText',
            pullDownMenu.ID_NEW_TEXT_CTRL: 'wxTextCtrl',

            pullDownMenu.ID_NEW_BUTTON: 'wxButton',
            pullDownMenu.ID_NEW_BITMAP_BUTTON: 'wxBitmapButton',
            pullDownMenu.ID_NEW_RADIO_BUTTON: 'wxRadioButton',
            pullDownMenu.ID_NEW_SPIN_BUTTON: 'wxSpinButton',

            pullDownMenu.ID_NEW_STATIC_BOX: 'wxStaticBox',
            pullDownMenu.ID_NEW_CHECK_BOX: 'wxCheckBox',
            pullDownMenu.ID_NEW_RADIO_BOX: 'wxRadioBox',
            pullDownMenu.ID_NEW_COMBO_BOX: 'wxComboBox',
            pullDownMenu.ID_NEW_LIST_BOX: 'wxListBox',

            pullDownMenu.ID_NEW_STATIC_LINE: 'wxStaticLine',
            pullDownMenu.ID_NEW_CHOICE: 'wxChoice',
            pullDownMenu.ID_NEW_SLIDER: 'wxSlider',
            pullDownMenu.ID_NEW_GAUGE: 'wxGauge',
            pullDownMenu.ID_NEW_SCROLL_BAR: 'wxScrollBar',
            pullDownMenu.ID_NEW_TREE_CTRL: 'wxTreeCtrl',
            pullDownMenu.ID_NEW_LIST_CTRL: 'wxListCtrl',
            pullDownMenu.ID_NEW_CHECK_LIST: 'wxCheckList',
            pullDownMenu.ID_NEW_NOTEBOOK: 'wxNotebook',
            pullDownMenu.ID_NEW_HTML_WINDOW: 'wxHtmlWindow',
            pullDownMenu.ID_NEW_CALENDAR: 'wxCalendar',

            pullDownMenu.ID_NEW_BOX_SIZER: 'wxBoxSizer',
            pullDownMenu.ID_NEW_STATIC_BOX_SIZER: 'wxStaticBoxSizer',
            pullDownMenu.ID_NEW_GRID_SIZER: 'wxGridSizer',
            pullDownMenu.ID_NEW_FLEX_GRID_SIZER: 'wxFlexGridSizer',
            pullDownMenu.ID_NEW_SPACER: 'spacer',
            pullDownMenu.ID_NEW_MENU: 'wxMenu',
            pullDownMenu.ID_NEW_MENU_ITEM: 'wxMenuItem',
            pullDownMenu.ID_NEW_SEPARATOR: 'separator',
            }
        pullDownMenu.controls = [
            ['control', 'Various controls',
             (pullDownMenu.ID_NEW_STATIC_TEXT, 'Label', 'Create static label'),
             (pullDownMenu.ID_NEW_STATIC_LINE, 'Line', 'Create static line'),
             (pullDownMenu.ID_NEW_TEXT_CTRL, 'TextBox', 'Create text box control'),
             (pullDownMenu.ID_NEW_CHOICE, 'Choice', 'Create choice control'),
             (pullDownMenu.ID_NEW_SLIDER, 'Slider', 'Create slider control'),
             (pullDownMenu.ID_NEW_GAUGE, 'Gauge', 'Create gauge control'),
             (pullDownMenu.ID_NEW_SCROLL_BAR, 'ScrollBar', 'Create scroll bar'),
             (pullDownMenu.ID_NEW_TREE_CTRL, 'TreeCtrl', 'Create tree control'),
             (pullDownMenu.ID_NEW_LIST_CTRL, 'ListCtrl', 'Create list control'),
             (pullDownMenu.ID_NEW_CHECK_LIST, 'CheckList', 'Create check list control'),
             (pullDownMenu.ID_NEW_HTML_WINDOW, 'HtmlWindow', 'Create HTML window'),
             (pullDownMenu.ID_NEW_CALENDAR, 'Calendar', 'Create calendar control'),
             (pullDownMenu.ID_NEW_PANEL, 'Panel', 'Create panel'),
             (pullDownMenu.ID_NEW_NOTEBOOK, 'Notebook', 'Create notebook control'),
             ],
            ['button', 'Buttons',
             (pullDownMenu.ID_NEW_BUTTON, 'Button', 'Create button'),
             (pullDownMenu.ID_NEW_BITMAP_BUTTON, 'BitmapButton', 'Create bitmap button'),
             (pullDownMenu.ID_NEW_RADIO_BUTTON, 'RadioButton', 'Create radio button'),
             (pullDownMenu.ID_NEW_SPIN_BUTTON, 'SpinButton', 'Create spin button'),
             ],
            ['box', 'Boxes',
             (pullDownMenu.ID_NEW_STATIC_BOX, 'StaticBox', 'Create static box'),
             (pullDownMenu.ID_NEW_CHECK_BOX, 'CheckBox', 'Create check box'),
             (pullDownMenu.ID_NEW_RADIO_BOX, 'RadioBox', 'Create radio box'),
             (pullDownMenu.ID_NEW_COMBO_BOX, 'ComboBox', 'Create combo box'),
             (pullDownMenu.ID_NEW_LIST_BOX, 'ListBox', 'Create list box'),
             ],
            ['sizer', 'Sizers',
             (pullDownMenu.ID_NEW_BOX_SIZER, 'BoxSizer', 'Create box sizer'),
             (pullDownMenu.ID_NEW_STATIC_BOX_SIZER, 'StaticBoxSizer',
              'Create static box sizer'),
             (pullDownMenu.ID_NEW_GRID_SIZER, 'GridSizer', 'Create grid sizer'),
             (pullDownMenu.ID_NEW_FLEX_GRID_SIZER, 'FlexGridSizer',
              'Create flexgrid sizer'),
             (pullDownMenu.ID_NEW_SPACER, 'Spacer', 'Create spacer'),
             ]
            ]
        pullDownMenu.menuControls = [
            (pullDownMenu.ID_NEW_MENU, 'Menu', 'Create menu'),
            (pullDownMenu.ID_NEW_MENU_ITEM, 'MenuItem', 'Create menu item'),
            (pullDownMenu.ID_NEW_SEPARATOR, 'Separator', 'Create separator'),
            ]

        # Initialize
        self.Clear()

        # Other events
        EVT_IDLE(self, self.OnIdle)
        EVT_CLOSE(self, self.OnCloseWindow)

    def OnNew(self, evt):
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
            self.Open(path)
            wxEndBusyCursor()
            self.SetStatusText('Ready')
        dlg.Destroy()

    def OnSaveOrSaveAs(self, evt):
        if evt.GetId() == wxID_SAVEAS or not self.dataFile:
            if self.dataFile: defaultName = ''
            else: defaultName = 'UNTITLED.xrc'
            dlg = wxFileDialog(self, 'Save As', os.path.dirname(self.dataFile),
                               defaultName, '*.xrc',
                               wxSAVE | wxOVERWRITE_PROMPT | wxCHANGE_DIR)
            if dlg.ShowModal() == wxID_CANCEL: return
            path = dlg.GetPath()
            dlg.Destroy()
        else:
            path = self.dataFile
        self.SetStatusText('Saving...')
        wxYield()
        wxBeginBusyCursor()
        self.Save(path)
        self.dataFile = path
        wxEndBusyCursor()
        self.SetStatusText('Ready')

    def OnExit(self, evt):
        self.Close()

    def OnUndo(self, evt):
        print '*** being implemented'
        print self.lastOp, self.undo
        if self.lastOp == 'DELETE':
            parent, prev, elem = self.undo
            if prev.IsOk():
                xxx = MakeXXXFromDOM(tree.GetPyData(parent).treeObject(), elem)
                item = tree.InsertItem( parent, prev, xxx.treeObject().className,
                                        data=wxTreeItemData(xxx) )

    def OnRedo(self, evt):
        print '*** being implemented'

    def OnCut(self, evt):
        selected = tree.GetSelection()
        # Undo info
        self.lastOp = 'CUT'
        self.undo = [tree.GetItemParent(selected), tree.GetPrevSibling(selected)]
        # Delete testWin?
        global testWin
        if testWin:
            # If deleting top-level item, delete testWin
            if selected == testWin.item:
                testWin.Destroy()
                testWin = None
            else:
                # Remove highlight, update testWin
                if tree.GetItemAncestor(selected) == testWin.item:
                    if testWin.highLight: testWin.highLight.Remove()
                    tree.needUpdate = true
        self.clipboard = tree.RemoveLeaf(selected)
        tree.pendingHighLight = None
        tree.Unselect()
        panel.Clear()
        self.modified = true

    def OnCopy(self, evt):
        selected = tree.GetSelection()
        xxx = tree.GetPyData(selected)
        self.clipboard = xxx.element.cloneNode(true)

    def OnPaste(self, evt):
        selected = tree.GetSelection()
        appendChild = not tree.NeedInsert(selected)
        xxx = tree.GetPyData(selected)
        if not appendChild:
            # If has next item, insert, else append to parent
            nextItem = tree.GetNextSibling(selected)
            if nextItem.IsOk():
                # Insert before nextItem
                parentLeaf = tree.GetItemParent(selected)
            else:                   # last child: change selected to parent
                appendChild = true
                selected = tree.GetItemParent(selected)
        # Expanded container (must have children)
        elif tree.IsExpanded(selected) and tree.ItemHasChildren(selected):
            appendChild = false
            nextItem = tree.GetFirstChild(selected, 0)[0]
            parentLeaf = selected
        # Parent should be tree element or None
        if appendChild:
            parent = tree.GetPyData(selected)
        else:
            parent = tree.GetPyData(parentLeaf)
        if parent and parent.hasChild: parent = parent.child

        # Create a copy of clipboard element
        elem = self.clipboard.cloneNode(true)
        # Tempopary xxx object to test things
        xxx = MakeXXXFromDOM(parent, elem)
        className = xxx.treeObject().className
        # Check parent and child relationships
        # Parent is sizer or notebook, child is of wrong class or
        # parent is normal window, child is child container: detach child
        isChildContainer = isinstance(xxx, xxxChildContainer)
        if not parent and isChildContainer or \
           (parent.isSizer and not isinstance(xxx, xxxSizerItem)) or \
           (isinstance(parent, xxxNotebook) and not isinstance(xxx, xxxNotebookPage)) or \
           (not parent.isSizer and not isinstance(parent, xxxNotebook) and \
            isChildContainer):
            if isChildContainer:
                elem.removeChild(xxx.child.element) # detach child
                elem.unlink()           # delete child container
                elem = xxx.child.element # replace
                # This should help garbage collection (!!! maybe not needed?)
                xxx.child.parent = None
                xxx.child = None
        if parent:
            # Parent is sizer or notebook, child is not child container
            if parent.isSizer and not isChildContainer and \
               not isinstance(xxx, xxxSpacer):
                # Create sizer item element
                sizerItemElem = MakeEmptyDOM('sizeritem')
                sizerItemElem.appendChild(elem)
                elem = sizerItemElem
            elif isinstance(parent, xxxNotebook) and not isChildContainer:
                pageElem = MakeEmptyDOM('notebookpage')
                pageElem.appendChild(elem)
                elem = pageElem
        xxx = MakeXXXFromDOM(parent, elem)
        # Figure out if we must append a new child or sibling
        if appendChild:
            if parent: node = parent.element
            else: node = tree.mainNode
            node.appendChild(elem)
            newItem = tree.AppendItem(selected, xxx.treeName(), image=xxx.treeImage(),
                                      data=wxTreeItemData(xxx))
        else:
            node = tree.GetPyData(nextItem).element
            if parent:
                elemParent = parent.element
            else:
                elemParent = tree.mainNode
            elemParent.insertBefore(elem, node)
            # Inserting before is difficult, se we insert after or first child
            newItem = tree.InsertItem(parentLeaf, selected, xxx.treeName(),
                                      image=xxx.treeImage(), data=wxTreeItemData(xxx))
        # Add children items
        if xxx.hasChildren:
            treeObj = xxx.treeObject()
            for n in treeObj.element.childNodes:
                if IsObject(n):
                    tree.AddNode(newItem, treeObj, n)
        # Scroll to show new item
        tree.EnsureVisible(newItem)
        tree.SelectItem(newItem)
        if not tree.IsVisible(newItem):
            tree.ScrollTo(newItem)
            tree.Refresh()
        # Update view?
        if testWin and tree.GetItemAncestor(newItem) == testWin.item:
            if conf.autoRefresh:
                tree.needUpdate = true
                tree.pendingHighLight = newItem
            else:
                tree.pendingHighLight = None
        self.modified = true

    def OnDelete(self, evt):
        selected = tree.GetSelection()
        # Undo info
        self.lastOp = 'DELETE'
        self.undo = [tree.GetItemParent(selected), tree.GetPrevSibling(selected)]
        # Delete testWin?
        global testWin
        if testWin:
            # If deleting top-level item, delete testWin
            if selected == testWin.item:
                testWin.Destroy()
                testWin = None
            else:
                # Remove highlight, update testWin
                if tree.GetItemAncestor(selected) == testWin.item:
                    if testWin.highLight: testWin.highLight.Remove()
                    tree.needUpdate = true
        xnode = tree.RemoveLeaf(selected)
        self.undo.append(xnode.cloneNode(true))
        xnode.unlink()
        tree.pendingHighLight = None
        tree.Unselect()
        panel.Clear()
        self.modified = true

    def OnRefresh(self, evt):
        # If modified, apply first
        selection = tree.GetSelection()
        if selection.IsOk():
            xxx = tree.GetPyData(selection)
            if xxx and panel.IsModified():
                tree.Apply(xxx, selection)
        if testWin:
            # (re)create
            tree.CreateTestWin(testWin.item)
        tree.needUpdate = false

    def OnAutoRefresh(self, evt):
        conf.autoRefresh = evt.IsChecked()
        self.menuBar.Check(self.ID_AUTO_REFRESH, conf.autoRefresh)
        self.tb.ToggleTool(self.ID_AUTO_REFRESH, conf.autoRefresh)

    def OnAbout(self, evt):
        dlg = wxMessageDialog(self, '%s %s\n\nRoman Rolinsky <rolinsky@mema.ucl.ac.be>' % \
                        (progname, version),
                        'About %s' % progname, wxOK | wxCENTRE)
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
                msg =string.join(traceback.format_exception_only(etype, value)
                        +traceback.format_list(tblist))
                print msg

    def OnCreate(self, evt):
        selected = tree.GetSelection()
        appendChild = not tree.NeedInsert(selected)
        xxx = tree.GetPyData(selected)
        if not appendChild:
            # If has next item, insert, else append to parent
            nextItem = tree.GetNextSibling(selected)
            if nextItem.IsOk():
                # Insert before nextItem
                parentLeaf = tree.GetItemParent(selected)
            else:                   # last child: change selected to parent
                appendChild = true
                selected = tree.GetItemParent(selected)
        # Expanded container (must have children)
        elif tree.IsExpanded(selected) and tree.ItemHasChildren(selected):
            appendChild = false
            nextItem = tree.GetFirstChild(selected, 0)[0]
            parentLeaf = selected
        # Parent should be tree element or None
        if appendChild:
            parent = tree.GetPyData(selected)
        else:
            parent = tree.GetPyData(parentLeaf)
        if parent and parent.hasChild: parent = parent.child

        # Create element
        className = self.createMap[evt.GetId()]
        xxx = MakeEmptyXXX(parent, className)
        # Figure out if we must append a new child or sibling
        elem = xxx.element
        if appendChild:
            if parent: node = parent.element
            else: node = tree.mainNode
            # Insert newline for debug purposes
            node.appendChild(tree.dom.createTextNode('\n'))
            node.appendChild(elem)
            newItem = tree.AppendItem(selected, xxx.treeName(), image=xxx.treeImage(),
                                      data=wxTreeItemData(xxx))
        else:
            node = tree.GetPyData(nextItem).element
            if parent:
                elemParent = parent.element
            else:
                elemParent = tree.mainNode
            elemParent.insertBefore(tree.dom.createTextNode('\n'), node)
            elemParent.insertBefore(elem, node)
            # Inserting before is difficult, se we insert after or first child
            newItem = tree.InsertItem(parentLeaf, selected,
                                      xxx.treeName(), image=xxx.treeImage(),
                                      data=wxTreeItemData(xxx))
        tree.EnsureVisible(newItem)
        tree.SelectItem(newItem)
        if not tree.IsVisible(newItem):
            tree.ScrollTo(newItem)
            tree.Refresh()
        # Update view?
        if testWin and tree.GetItemAncestor(newItem) == testWin.item:
            if conf.autoRefresh:
                tree.needUpdate = true
                tree.pendingHighLight = newItem
            else:
                tree.pendingHighLight = None

    def OnExpand(self, evt):
        if tree.GetSelection().IsOk():
            tree.ExpandAll(tree.GetSelection())
        else:
            tree.ExpandAll(tree.GetRootItem())

    def OnPullDownHighlight(self, evt):
        menuId = evt.GetMenuId()
        help = ''
        if menuId != -1: help = pullDownMenu.GetHelpString(menuId)
        self.SetStatusText(help)

    def OnUpdateUI(self, evt):
        if evt.GetId() in [wxID_CUT, wxID_COPY, self.ID_DELETE]:
            enable = tree.GetSelection().IsOk() and \
                        tree.GetSelection() != tree.GetRootItem()
            evt.Enable(enable)
        elif evt.GetId() == wxID_PASTE:
            enable = self.clipboard != None
            evt.Enable(enable)

    def OnIdle(self, evt):
        if tree.needUpdate:
            if conf.autoRefresh:
                if testWin:
                    # (re)create
                    tree.CreateTestWin(testWin.item)
                tree.needUpdate = false
        elif tree.pendingHighLight:
            tree.HighLight(tree.pendingHighLight)
        evt.Skip()

    def OnCloseWindow(self, evt):
        if not self.AskSave(): return
        if testWin: testWin.Destroy()
        conf.width, conf.height = self.GetSize()
        evt.Skip()

    def Clear(self):
        self.dataFile = ''
        self.clipboard = None
        self.modified = false
        panel.SetModified(false)
        panel.Clear()
        tree.Clear()
        global testWin
        if testWin:
            testWin.Destroy()
            testWin = None
        self.SetTitle(progname)

    def Open(self, path):
        # Try to read the file
        try:
            open(path)
            self.Clear()
            # Build wx tree
            dom = minidom.parse(path)
            tree.SetData(dom)
            self.dataFile = path
            self.SetTitle(progname + ': ' + os.path.basename(path))
        except:
            wxLogError('Error reading file: ' + path)

    def Save(self, path):
        try:
            memFile = MemoryFile(path)
            tree.dom.writexml(memFile)
            memFile.close()
            self.modified = false
            panel.SetModified(false)
        except:
            wxLogError('Error writing file: ' + path)

    def AskSave(self):
        if not (self.modified or panel.IsModified()): return true
        flags = wxICON_EXCLAMATION | wxYES_NO | wxCANCEL | wxCENTRE
        dlg = wxMessageDialog( self, 'File is modified. Save before exit?',
                               'Save before too late?', flags )
        say = dlg.ShowModal()
        dlg.Destroy()
        if say == wxID_YES:
            self.OnSaveOrSaveAs(wxCommandEvent(wxID_SAVE))
            # If save was successful, modified flag is unset
            if not self.modified: return true
        elif say == wxID_NO:
            self.modified = false
            panel.SetModified(false)
            return true
        return false

class App(wxApp):
    def OnInit(self):
        self.SetAppName("xrced")
        # Settings
        global conf
        # !!! wxConfigBase_Get doesn't seem to work
        conf = wxConfig(style=wxCONFIG_USE_LOCAL_FILE)
        conf.autoRefresh = conf.ReadInt('autorefresh', true)
        size = conf.ReadInt('width', 800), conf.ReadInt('height', 600)
        # Add handlers
        wxFileSystem_AddHandler(wxMemoryFSHandler())
        wxInitAllImageHandlers()
        # Create main frame
        global frame
        frame = self.frame = Frame(size)
        self.frame.Show(true)
        # Load resources from XRC file (!!! should be transformed to .py later)
        sys.modules['params'].frame = frame
        frame.res = wxXmlResource('')
        frame.res.Load(os.path.join(sys.path[0], 'xrced.xrc'))
        return true

    def OnExit(self):
        # Write config
        wc = wxConfigBase_Get()
        wc.WriteInt('autorefresh', conf.autoRefresh)
        wc.WriteInt('width', conf.width)
        wc.WriteInt('height', conf.height)
        wc.Flush()

def main():
    app = App()
    app.MainLoop()
    app.OnExit()

if __name__ == '__main__':
    main()
