# Name:         xxx.py ('xxx' is easy to distinguish from 'wx' :) )
# Purpose:      XML interface classes
# Author:       Roman Rolinsky <rolinsky@mema.ucl.ac.be>
# Created:      22.08.2001
# RCS-ID:       $Id$

from wxPython.wx import *
from wxPython.xrc import *
from xml.dom import minidom
import wxPython.lib.wxpTag

from params import *

currentEncoding = wxLocale_GetSystemEncodingName()

# Base class for interface parameter classes
class xxxNode:
    def __init__(self, node):
        self.node = node
    def remove(self):
        self.node.parentNode.removeChild(self.node)
        self.node.unlink()

# Generic (text) parameter class
class xxxParam(xxxNode):
    # Standard use: for text nodes
    def __init__(self, node):
        xxxNode.__init__(self, node)
        if not node.hasChildNodes():
            # If does not have child nodes, create empty text node
            text = tree.dom.createTextNode('')
            node.appendChild(text)
        else:
            text = node.childNodes[0] # first child must be text node
            assert text.nodeType == minidom.Node.TEXT_NODE
        # Use convertion from unicode to current encoding
        self.textNode = text
    # Value returns string
    if wxUSE_UNICODE:   # no conversion is needed
        def value(self):
            return self.textNode.data
        def update(self, value):
            self.textNode.data = value
    else:
        def value(self):
            return self.textNode.data.encode(currentEncoding)
        def update(self, value):
            self.textNode.data = unicode(value, currentEncoding)

# Integer parameter
class xxxParamInt(xxxParam):
    # Standard use: for text nodes
    def __init__(self, node):
        xxxParam.__init__(self, node)
    # Value returns string
    def value(self):
        try:
            return int(self.textNode.data)
        except ValueError:
            return -1                   # invalid value
    def update(self, value):
        self.textNode.data = str(value)

# Content parameter
class xxxParamContent(xxxNode):
    def __init__(self, node):
        xxxNode.__init__(self, node)
        data, l = [], []                # data is needed to quicker value retrieval
        nodes = node.childNodes[:]      # make a copy of the child list
        for n in nodes:
            if n.nodeType == minidom.Node.ELEMENT_NODE:
                assert n.tagName == 'item', 'bad content content'
                if not n.hasChildNodes():
                    # If does not have child nodes, create empty text node
                    text = tree.dom.createTextNode('')
                    node.appendChild(text)
                else:
                    # !!! normalize?
                    text = n.childNodes[0] # first child must be text node
                    assert text.nodeType == minidom.Node.TEXT_NODE
                l.append(text)
                data.append(str(text.data))
            else:                       # remove other
                node.removeChild(n)
                n.unlink()
        self.l, self.data = l, data
    def value(self):
        return self.data
    def update(self, value):
        # If number if items is not the same, recreate children
        if len(value) != len(self.l):   # remove first if number of items has changed
            childNodes = self.node.childNodes[:]
            for n in childNodes:
                self.node.removeChild(n)
            l = []
            for str in value:
                itemElem = tree.dom.createElement('item')
                itemText = tree.dom.createTextNode(str)
                itemElem.appendChild(itemText)
                self.node.appendChild(itemElem)
                l.append(itemText)
            self.l = l
        else:
            for i in range(len(value)):
                self.l[i].data = value[i]
        self.data = value

# Content parameter for checklist
class xxxParamContentCheckList(xxxNode):
    def __init__(self, node):
        xxxNode.__init__(self, node)
        data, l = [], []                # data is needed to quicker value retrieval
        nodes = node.childNodes[:]      # make a copy of the child list
        for n in nodes:
            if n.nodeType == minidom.Node.ELEMENT_NODE:
                assert n.tagName == 'item', 'bad content content'
                checked = n.getAttribute('checked')
                if not checked: checked = 0
                if not n.hasChildNodes():
                    # If does not have child nodes, create empty text node
                    text = tree.dom.createTextNode('')
                    node.appendChild(text)
                else:
                    # !!! normalize?
                    text = n.childNodes[0] # first child must be text node
                    assert text.nodeType == minidom.Node.TEXT_NODE
                l.append((text, n))
                data.append((str(text.data), int(checked)))
            else:                       # remove other
                node.removeChild(n)
                n.unlink()
        self.l, self.data = l, data
    def value(self):
        return self.data
    def update(self, value):
        # If number if items is not the same, recreate children
        if len(value) != len(self.l):   # remove first if number of items has changed
            childNodes = self.node.childNodes[:]
            for n in childNodes:
                self.node.removeChild(n)
            l = []
            for s,ch in value:
                itemElem = tree.dom.createElement('item')
                # Add checked only if true
                if ch: itemElem.setAttribute('checked', '1')
                itemText = tree.dom.createTextNode(s)
                itemElem.appendChild(itemText)
                self.node.appendChild(itemElem)
                l.append((itemText, itemElem))
            self.l = l
        else:
            for i in range(len(value)):
                self.l[i][0].data = value[i][0]
                self.l[i][1].setAttribute('checked', str(value[i][1]))
        self.data = value

################################################################################

# Classes to interface DOM objects
class xxxObject:
    # Default behavior
    hasChildren = false                 # has children elements?
    hasStyle = true                     # almost everyone
    hasName = true                      # has name attribute?
    isSizer = hasChild = false
    allParams = None                    # Some nodes have no parameters
    # Style parameters (all optional)
    styles = ['fg', 'bg', 'font', 'enabled', 'focused', 'hidden', 'tooltip']
    # Special parameters
    specials = []
    # Required paremeters: none by default
    required = []
    # Default parameters with default values
    default = {}
    # Parameter types
    paramDict = {}
    # Window styles and extended styles
    winStyles = []
    # Tree icon index
    #image = -1
    # Construct a new xxx object from DOM element
    # parent is parent xxx object (or None if none), element is DOM element object
    def __init__(self, parent, element):
        self.parent = parent
        self.element = element
        self.undo = None
        # Get attributes
        self.className = element.getAttribute('class')
        if self.hasName: self.name = element.getAttribute('name')
        # Set parameters (text element children)
        self.params = {}
        nodes = element.childNodes[:]
        for node in nodes:
            if node.nodeType == minidom.Node.ELEMENT_NODE:
                tag = node.tagName
                if tag == 'object':
                    continue            # do nothing for object children here
                if tag not in self.allParams and tag not in self.styles:
                    print 'WARNING: unknown parameter for %s: %s' % \
                          (self.className, tag)
                elif tag in self.specials:
                    self.special(tag, node)
                elif tag == 'content':
                    if self.className == 'wxCheckList':
                        self.params[tag] = xxxParamContentCheckList(node)
                    else:
                        self.params[tag] = xxxParamContent(node)
                elif tag == 'font': # has children
                    self.params[tag] = xxxParamFont(element, node)
                else:                   # simple parameter
                    self.params[tag] = xxxParam(node)
            else:
                # Remove all other nodes
                element.removeChild(node)
                node.unlink()
    # Returns real tree object
    def treeObject(self):
        if self.hasChild: return self.child
        return self
    # Returns tree image index
    def treeImage(self):
        if self.hasChild: return self.child.treeImage()
        return self.image
    # Class name plus wx name
    def treeName(self):
        if self.hasChild: return self.child.treeName()
        if self.hasName and self.name: return self.className + ' "' + self.name + '"'
        return self.className

################################################################################

# This is a little special: it is both xxxObject and xxxNode
class xxxParamFont(xxxObject, xxxNode):
    allParams = ['size', 'style', 'weight', 'family', 'underlined',
                 'face', 'encoding']
    def __init__(self, parent, element):
        xxxObject.__init__(self, parent, element)
        xxxNode.__init__(self, element)
        self.parentNode = parent       # required to behave similar to DOM node
        v = []
        for p in self.allParams:
            try:
                v.append(str(self.params[p].value()))
            except KeyError:
                v.append('')
        self.data = v
    def update(self, value):
        # `value' is a list of strings corresponding to all parameters
        elem = self.element
        # Remove old elements first
        childNodes = elem.childNodes[:]
        for node in childNodes: elem.removeChild(node)
        i = 0
        self.params.clear()
        v = []
        for param in self.allParams:
            if value[i]:
                fontElem = tree.dom.createElement(param)
                textNode = tree.dom.createTextNode(value[i])
                self.params[param] = textNode
                fontElem.appendChild(textNode)
                elem.appendChild(fontElem)
            v.append(value[i])
            i += 1
        self.data = v
    def value(self):
        return self.data

################################################################################

class xxxContainer(xxxObject):
    hasChildren = true

# Simulate normal parameter for encoding
class xxxEncoding:
    def __init__(self, val):
        self.encd = val
    def value(self):
        return self.encd
    def update(self, val):
        self.encd = val

# Special class for root node
class xxxMainNode(xxxContainer):
    allParams = ['encoding']
    required = ['encoding']
    hasStyle = hasName = false
    def __init__(self, dom):
        xxxContainer.__init__(self, None, dom.documentElement)
        self.className = 'XML tree'
        self.params['encoding'] = xxxEncoding(dom.encoding)

################################################################################
# Top-level windwows

class xxxPanel(xxxContainer):
    allParams = ['pos', 'size', 'style']
    styles = ['fg', 'bg', 'font', 'enabled', 'focused', 'hidden', 'exstyle',
              'tooltip']
    winStyles = ['wxNO_3D', 'wxTAB_TRAVERSAL', 'wxCLIP_CHILDREN']
    exStyles = ['wxWS_EX_VALIDATE_RECURSIVELY']

class xxxDialog(xxxContainer):
    allParams = ['title', 'centered', 'pos', 'size', 'style']
    paramDict = {'centered': ParamBool}
    required = ['title']
    winStyles = ['wxDEFAULT_DIALOG_STYLE', 'wxSTAY_ON_TOP',
                 'wxDIALOG_MODAL', 'wxDIALOG_MODELESS',
                 'wxCAPTION', 'wxSYSTEM_MENU', 'wxRESIZE_BORDER', 'wxRESIZE_BOX',
                 'wxTHICK_FRAME', 
                 'wxNO_3D', 'wxTAB_TRAVERSAL', 'wxCLIP_CHILDREN']
    styles = ['fg', 'bg', 'font', 'enabled', 'focused', 'hidden', 'exstyle',
              'tooltip']
    exStyles = ['wxWS_EX_VALIDATE_RECURSIVELY']

class xxxFrame(xxxContainer):
    allParams = ['title', 'centered', 'pos', 'size', 'style']
    paramDict = {'centered': ParamBool}
    required = ['title']
    winStyles = ['wxDEFAULT_FRAME_STYLE', 'wxDEFAULT_DIALOG_STYLE',
                 'wxSTAY_ON_TOP', 
                 'wxCAPTION', 'wxSYSTEM_MENU', 'wxRESIZE_BORDER',
                 'wxRESIZE_BOX', 'wxMINIMIZE_BOX', 'wxMAXIMIZE_BOX',
                 'wxFRAME_FLOAT_ON_PARENT', 'wxFRAME_TOOL_WINDOW',
                 'wxNO_3D', 'wxTAB_TRAVERSAL', 'wxCLIP_CHILDREN']
    styles = ['fg', 'bg', 'font', 'enabled', 'focused', 'hidden', 'exstyle',
              'tooltip']
    exStyles = ['wxWS_EX_VALIDATE_RECURSIVELY']

class xxxTool(xxxObject):
    allParams = ['bitmap', 'bitmap2', 'toggle', 'tooltip', 'longhelp']
    paramDict = {'bitmap2': ParamFile}
    hasStyle = false

class xxxToolBar(xxxContainer):
    allParams = ['bitmapsize', 'margins', 'packing', 'separation', 
                 'pos', 'size', 'style']
    hasStyle = false
    paramDict = {'bitmapsize': ParamPosSize, 'margins': ParamPosSize,
                 'packing': ParamInt, 'separation': ParamInt,
                 'style': ParamNonGenericStyle}
    winStyles = ['wxTB_FLAT', 'wxTB_DOCKABLE', 'wxTB_VERTICAL', 'wxTB_HORIZONTAL']

################################################################################
# Bitmap, Icon

class xxxBitmap(xxxObject):
    allParams = ['bitmap']
    required = ['bitmap']

class xxxIcon(xxxObject):
    allParams = ['icon']
    required = ['icon']

################################################################################
# Controls

class xxxStaticText(xxxObject):
    allParams = ['label', 'pos', 'size', 'style']
    required = ['label']
    winStyles = ['wxALIGN_LEFT', 'wxALIGN_RIGHT', 'wxALIGN_CENTRE', 'wxST_NO_AUTORESIZE']

class xxxStaticLine(xxxObject):
    allParams = ['pos', 'size', 'style']
    winStyles = ['wxLI_HORIZONTAL', 'wxLI_VERTICAL']

class xxxStaticBitmap(xxxObject):
    allParams = ['bitmap', 'pos', 'size', 'style']
    required = ['bitmap']

class xxxTextCtrl(xxxObject):
    allParams = ['value', 'pos', 'size', 'style']
    winStyles = ['wxTE_PROCESS_ENTER', 'wxTE_PROCESS_TAB', 'wxTE_MULTILINE',
              'wxTE_PASSWORD', 'wxTE_READONLY', 'wxHSCROLL']

class xxxChoice(xxxObject):
    allParams = ['content', 'selection', 'pos', 'size', 'style']
    required = ['content']
    winStyles = ['wxCB_SORT']

class xxxSlider(xxxObject):
    allParams = ['value', 'min', 'max', 'pos', 'size', 'style',
                 'tickfreq', 'pagesize', 'linesize', 'thumb', 'tick',
                 'selmin', 'selmax']
    paramDict = {'value': ParamInt, 'tickfreq': ParamInt, 'pagesize': ParamInt,
                 'linesize': ParamInt, 'thumb': ParamInt, 'thumb': ParamInt,
                 'tick': ParamInt, 'selmin': ParamInt, 'selmax': ParamInt}
    required = ['value', 'min', 'max']
    winStyles = ['wxSL_HORIZONTAL', 'wxSL_VERTICAL', 'wxSL_AUTOTICKS', 'wxSL_LABELS',
                 'wxSL_LEFT', 'wxSL_RIGHT', 'wxSL_TOP', 'wxSL_BOTTOM',
                 'wxSL_BOTH', 'wxSL_SELRANGE']

class xxxGauge(xxxObject):
    allParams = ['range', 'pos', 'size', 'style', 'value', 'shadow', 'bezel']
    paramDict = {'range': ParamInt, 'value': ParamInt, 
                 'shadow': ParamInt, 'bezel': ParamInt}
    winStyles = ['wxGA_HORIZONTAL', 'wxGA_VERTICAL', 'wxGA_PROGRESSBAR', 'wxGA_SMOOTH']

class xxxScrollBar(xxxObject):
    allParams = ['pos', 'size', 'style', 'value', 'thumbsize', 'range', 'pagesize']
    paramDict = {'value': ParamInt, 'range': ParamInt, 'thumbsize': ParamInt,
                 'pagesize': ParamInt}
    winStyles = ['wxSB_HORIZONTAL', 'wxSB_VERTICAL']

class xxxListCtrl(xxxObject):
    allParams = ['pos', 'size', 'style']
    winStyles = ['wxLC_LIST', 'wxLC_REPORT', 'wxLC_ICON', 'wxLC_SMALL_ICON',
              'wxLC_ALIGN_TOP', 'wxLC_ALIGN_LEFT', 'wxLC_AUTOARRANGE',
              'wxLC_USER_TEXT', 'wxLC_EDIT_LABELS', 'wxLC_NO_HEADER',
              'wxLC_SINGLE_SEL', 'wxLC_SORT_ASCENDING', 'wxLC_SORT_DESCENDING']

class xxxTreeCtrl(xxxObject):
    allParams = ['pos', 'size', 'style']
    winStyles = ['wxTR_HAS_BUTTONS', 'wxTR_NO_LINES', 'wxTR_LINES_AT_ROOT',
              'wxTR_EDIT_LABELS', 'wxTR_MULTIPLE']

class xxxHtmlWindow(xxxObject):
    allParams = ['pos', 'size', 'style', 'borders', 'url', 'htmlcode']
    paramDict = {'borders': ParamInt}
    winStyles = ['wxHW_SCROLLBAR_NEVER', 'wxHW_SCROLLBAR_AUTO']

class xxxCalendarCtrl(xxxObject):
    allParams = ['pos', 'size', 'style']

class xxxNotebook(xxxContainer):
    allParams = ['usenotebooksizer', 'pos', 'size', 'style']
    paramDict = {'usenotebooksizer': ParamBool}
    winStyles = ['wxNB_FIXEDWIDTH', 'wxNB_LEFT', 'wxNB_RIGHT', 'wxNB_BOTTOM']

class xxxGenericDirCtrl(xxxObject):
    allParams = ['defaultfolder', 'filter', 'defaultfilter', 'pos', 'size', 'style']
    paramDict = {'defaultfilter': ParamInt}
    winStyles = ['wxDIRCTRL_DIR_ONLY', 'wxDIRCTRL_3D_INTERNAL', 'wxDIRCTRL_SELECT_FIRST',
                 'wxDIRCTRL_SHOW_FILTERS', 'wxDIRCTRL_EDIT_LABELS']

################################################################################
# Buttons

class xxxButton(xxxObject):
    allParams = ['label', 'default', 'pos', 'size', 'style']
    paramDict = {'default': ParamBool}
    required = ['label']
    winStyles = ['wxBU_LEFT', 'wxBU_TOP', 'wxBU_RIGHT', 'wxBU_BOTTOM']

class xxxBitmapButton(xxxObject):
    allParams = ['bitmap', 'selected', 'focus', 'disabled', 'default',
                 'pos', 'size', 'style']
    required = ['bitmap']
    winStyles = ['wxBU_AUTODRAW', 'wxBU_LEFT', 'wxBU_TOP',
                 'wxBU_RIGHT', 'wxBU_BOTTOM']

class xxxRadioButton(xxxObject):
    allParams = ['label', 'value', 'pos', 'size', 'style']
    paramDict = {'value': ParamBool}
    required = ['label']
    winStyles = ['wxRB_GROUP']

class xxxSpinButton(xxxObject):
    allParams = ['value', 'min', 'max', 'pos', 'size', 'style']
    paramDict = {'value': ParamInt}
    winStyles = ['wxSP_HORIZONTAL', 'wxSP_VERTICAL', 'wxSP_ARROW_KEYS', 'wxSP_WRAP']

class xxxSpinCtrl(xxxObject):
    allParams = ['value', 'min', 'max', 'pos', 'size', 'style']
    paramDict = {'value': ParamInt}
    winStyles = ['wxSP_HORIZONTAL', 'wxSP_VERTICAL', 'wxSP_ARROW_KEYS', 'wxSP_WRAP']

################################################################################
# Boxes

class xxxStaticBox(xxxObject):
    allParams = ['label', 'pos', 'size', 'style']
    required = ['label']

class xxxRadioBox(xxxObject):
    allParams = ['label', 'content', 'selection', 'dimension', 'pos', 'size', 'style']
    paramDict = {'dimension': ParamInt}
    required = ['label', 'content']
    winStyles = ['wxRA_SPECIFY_ROWS', 'wxRA_SPECIFY_COLS']

class xxxCheckBox(xxxObject):
    allParams = ['label', 'checked', 'pos', 'size', 'style']
    paramDict = {'checked': ParamBool}
    required = ['label']

class xxxComboBox(xxxObject):
    allParams = ['content', 'selection', 'value', 'pos', 'size', 'style']
    required = ['content']
    winStyles = ['wxCB_SIMPLE', 'wxCB_SORT', 'wxCB_READONLY', 'wxCB_DROPDOWN']

class xxxListBox(xxxObject):
    allParams = ['content', 'selection', 'pos', 'size', 'style']
    required = ['content']
    winStyles = ['wxLB_SINGLE', 'wxLB_MULTIPLE', 'wxLB_EXTENDED', 'wxLB_HSCROLL',
              'wxLB_ALWAYS_SB', 'wxLB_NEEDED_SB', 'wxLB_SORT']

class xxxCheckList(xxxObject):
    allParams = ['content', 'pos', 'size', 'style']
    required = ['content']
    winStyles = ['wxLC_LIST', 'wxLC_REPORT', 'wxLC_ICON', 'wxLC_SMALL_ICON',
              'wxLC_ALIGN_TOP', 'wxLC_ALIGN_LEFT', 'wxLC_AUTOARRANGE',
              'wxLC_USER_TEXT', 'wxLC_EDIT_LABELS', 'wxLC_NO_HEADER',
              'wxLC_SINGLE_SEL', 'wxLC_SORT_ASCENDING', 'wxLC_SORT_DESCENDING']
    paramDict = {'content': ParamContentCheckList}
    
################################################################################
# Sizers

class xxxSizer(xxxContainer):
    hasName = hasStyle = false
    paramDict = {'orient': ParamOrient}
    isSizer = true

class xxxBoxSizer(xxxSizer):
    allParams = ['orient']
    required = ['orient']
    default = {'orient': 'wxVERTICAL'}
    # Tree icon depends on orientation
    def treeImage(self):
        if self.params['orient'].value() == 'wxHORIZONTAL': return self.imageH
        else: return self.imageV

class xxxStaticBoxSizer(xxxBoxSizer):
    allParams = ['label', 'orient']
    required = ['label', 'orient']
    default = {'orient': 'wxVERTICAL'}

class xxxGridSizer(xxxSizer):
    allParams = ['cols', 'rows', 'vgap', 'hgap']
    required = ['cols']
    default = {'cols': '2', 'rows': '2'}

# For repeated parameters
class xxxParamMulti:
    def __init__(self):
        self.l, self.data = [], []
    def append(self, param):
        self.l.append(param)
        self.data.append(param.value())
    def value(self):
        return self.data
    def remove(self):
        for param in self.l:
            param.remove()
        self.l, self.data = [], []

class xxxFlexGridSizer(xxxGridSizer):
    specials = ['growablecols', 'growablerows']
    allParams = ['cols', 'rows', 'vgap', 'hgap'] + specials
    paramDict = {'growablecols':ParamIntList, 'growablerows':ParamIntList}
    # Special processing for growable* parameters
    # (they are represented by several nodes)
    def special(self, tag, node):
        if not self.params.has_key(tag):
            self.params[tag] = xxxParamMulti()
        self.params[tag].append(xxxParamInt(node))
    def setSpecial(self, param, value):
        # Straightforward implementation: remove, add again
        self.params[param].remove()
        del self.params[param]
        for i in value:
            node = tree.dom.createElement(param)
            text = tree.dom.createTextNode(str(i))
            node.appendChild(text)
            self.element.appendChild(node)
            self.special(param, node)

# Container with only one child.
# Not shown in tree.
class xxxChildContainer(xxxObject):
    hasName = hasStyle = false
    hasChild = true
    def __init__(self, parent, element):
        xxxObject.__init__(self, parent, element)
        # Must have one child with 'object' tag, but we don't check it
        nodes = element.childNodes[:]   # create copy
        for node in nodes:
            if node.nodeType == minidom.Node.ELEMENT_NODE:
                if node.tagName == 'object':
                    # Create new xxx object for child node
                    self.child = MakeXXXFromDOM(self, node)
                    self.child.parent = parent
                    # Copy hasChildren and isSizer attributes
                    self.hasChildren = self.child.hasChildren
                    self.isSizer = self.child.isSizer
                    return              # success
            else:
                element.removeChild(node)
                node.unlink()
        assert 0, 'no child found'

class xxxSizerItem(xxxChildContainer):
    allParams = ['option', 'flag', 'border']
    paramDict = {'option': ParamInt}
    def __init__(self, parent, element):
        xxxChildContainer.__init__(self, parent, element)
        # Remove pos parameter - not needed for sizeritems
        if 'pos' in self.child.allParams:
            self.child.allParams = self.child.allParams[:]
            self.child.allParams.remove('pos')

class xxxNotebookPage(xxxChildContainer):
    allParams = ['label', 'selected']
    paramDict = {'selected': ParamBool}
    required = ['label']
    def __init__(self, parent, element):
        xxxChildContainer.__init__(self, parent, element)
        # pos and size dont matter for notebookpages
        if 'pos' in self.child.allParams:
            self.child.allParams = self.child.allParams[:]
            self.child.allParams.remove('pos')
        if 'size' in self.child.allParams:
            self.child.allParams = self.child.allParams[:]
            self.child.allParams.remove('size')

class xxxSpacer(xxxObject):
    hasName = hasStyle = false
    allParams = ['size', 'option', 'flag', 'border']
    paramDict = {'option': ParamInt}
    default = {'size': '0,0'}

class xxxMenuBar(xxxContainer):
    allParams = ['style']
    paramDict = {'style': ParamNonGenericStyle}    # no generic styles
    winStyles = ['wxMB_DOCKABLE']

class xxxMenu(xxxContainer):
    allParams = ['label', 'help', 'style']
    default = {'label': ''}
    paramDict = {'style': ParamNonGenericStyle}    # no generic styles
    winStyles = ['wxMENU_TEAROFF']

class xxxMenuItem(xxxObject):
    allParams = ['label', 'bitmap', 'accel', 'help',
                 'checkable', 'radio', 'enabled', 'checked']
    default = {'label': ''}
    hasStyle = false

class xxxSeparator(xxxObject):
    hasName = hasStyle = false

################################################################################
# Unknown control

class xxxUnknown(xxxObject):
    allParams = ['pos', 'size', 'style']
    paramDict = {'style': ParamNonGenericStyle}    # no generic styles

################################################################################

xxxDict = {
    'wxPanel': xxxPanel,
    'wxDialog': xxxDialog,
    'wxFrame': xxxFrame,
    'tool': xxxTool,
    'wxToolBar': xxxToolBar,
    
    'wxBitmap': xxxBitmap,
    'wxIcon': xxxIcon,

    'wxButton': xxxButton,
    'wxBitmapButton': xxxBitmapButton,
    'wxRadioButton': xxxRadioButton,
    'wxSpinButton': xxxSpinButton,

    'wxStaticBox': xxxStaticBox,
    'wxStaticBitmap': xxxStaticBitmap,
    'wxRadioBox': xxxRadioBox,
    'wxComboBox': xxxComboBox,
    'wxCheckBox': xxxCheckBox,
    'wxListBox': xxxListBox,

    'wxStaticText': xxxStaticText,
    'wxStaticLine': xxxStaticLine,
    'wxTextCtrl': xxxTextCtrl,
    'wxChoice': xxxChoice,
    'wxSlider': xxxSlider,
    'wxGauge': xxxGauge,
    'wxScrollBar': xxxScrollBar,
    'wxTreeCtrl': xxxTreeCtrl,
    'wxListCtrl': xxxListCtrl,
    'wxCheckList': xxxCheckList,
    'wxNotebook': xxxNotebook,
    'notebookpage': xxxNotebookPage,
    'wxHtmlWindow': xxxHtmlWindow,
    'wxCalendarCtrl': xxxCalendarCtrl,
    'wxGenericDirCtrl': xxxGenericDirCtrl,
    'wxSpinCtrl': xxxSpinCtrl,
    
    'wxBoxSizer': xxxBoxSizer,
    'wxStaticBoxSizer': xxxStaticBoxSizer,
    'wxGridSizer': xxxGridSizer,
    'wxFlexGridSizer': xxxFlexGridSizer,
    'sizeritem': xxxSizerItem,
    'spacer': xxxSpacer,

    'wxMenuBar': xxxMenuBar,
    'wxMenu': xxxMenu,
    'wxMenuItem': xxxMenuItem,
    'separator': xxxSeparator,

    'unknown': xxxUnknown,
    }

# Create IDs for all parameters of all classes
paramIDs = {'fg': wxNewId(), 'bg': wxNewId(), 'exstyle': wxNewId(), 'font': wxNewId(),
            'enabled': wxNewId(), 'focused': wxNewId(), 'hidden': wxNewId(),
            'tooltip': wxNewId(), 'encoding': wxNewId()
            }
for cl in xxxDict.values():
    if cl.allParams:
        for param in cl.allParams + cl.paramDict.keys():
            if not paramIDs.has_key(param):
                paramIDs[param] = wxNewId()

################################################################################
# Helper functions

# Test for object elements
def IsObject(node):
    return node.nodeType == minidom.Node.ELEMENT_NODE and node.tagName == 'object'

# Make XXX object from some DOM object, selecting correct class
def MakeXXXFromDOM(parent, element):
    try:
        klass = xxxDict[element.getAttribute('class')]
    except KeyError:
        # Verify that it's not recursive exception
        print 'ERROR: unknown class:', element.getAttribute('class')
        raise
    return klass(parent, element)

# Make empty DOM element
def MakeEmptyDOM(className): 
    elem = tree.dom.createElement('object')
    elem.setAttribute('class', className)
    # Set required and default parameters
    xxxClass = xxxDict[className]
    defaultNotRequired = filter(lambda x, l=xxxClass.required: x not in l,
                                xxxClass.default.keys())
    for param in xxxClass.required + defaultNotRequired:
        textElem = tree.dom.createElement(param)
        try:
            textNode = tree.dom.createTextNode(xxxClass.default[param])
        except KeyError:
            textNode = tree.dom.createTextNode('')
        textElem.appendChild(textNode)
        elem.appendChild(textElem)
    return elem

# Make empty XXX object
def MakeEmptyXXX(parent, className):
    # Make corresponding DOM object first
    elem = MakeEmptyDOM(className)
    # If parent is a sizer, we should create sizeritem object, except for spacers
    if parent:
        if parent.isSizer and className != 'spacer':
            sizerItemElem = MakeEmptyDOM('sizeritem')
            sizerItemElem.appendChild(elem)
            elem = sizerItemElem
        elif isinstance(parent, xxxNotebook):
            pageElem = MakeEmptyDOM('notebookpage')
            pageElem.appendChild(elem)
            elem = pageElem
    # Now just make object
    return MakeXXXFromDOM(parent, elem)

