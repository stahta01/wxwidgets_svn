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

import wx.xrc

sys.modules['wxPython.wx'] = _wx
del sys, _wx


# Now assign all the reverse-renamed names:
WX_XMLRES_CURRENT_VERSION_MAJOR = wx.xrc.WX_XMLRES_CURRENT_VERSION_MAJOR
WX_XMLRES_CURRENT_VERSION_MINOR = wx.xrc.WX_XMLRES_CURRENT_VERSION_MINOR
WX_XMLRES_CURRENT_VERSION_RELEASE = wx.xrc.WX_XMLRES_CURRENT_VERSION_RELEASE
WX_XMLRES_CURRENT_VERSION_REVISION = wx.xrc.WX_XMLRES_CURRENT_VERSION_REVISION
wxXRC_USE_LOCALE = wx.xrc.XRC_USE_LOCALE
wxXRC_NO_SUBCLASSING = wx.xrc.XRC_NO_SUBCLASSING
wxEmptyXmlResource = wx.xrc.EmptyXmlResource
wxXmlResource_AddSubclassFactory = wx.xrc.XmlResource_AddSubclassFactory
wxXmlResource_GetXRCID = wx.xrc.XmlResource_GetXRCID
wxXmlResource_Get = wx.xrc.XmlResource_Get
wxXmlResource_Set = wx.xrc.XmlResource_Set
wxXmlResource = wx.xrc.XmlResource
wxXmlResourcePtr = wx.xrc.XmlResourcePtr
wxXmlSubclassFactory = wx.xrc.XmlSubclassFactory
wxXmlSubclassFactoryPtr = wx.xrc.XmlSubclassFactoryPtr
wxXML_ELEMENT_NODE = wx.xrc.XML_ELEMENT_NODE
wxXML_ATTRIBUTE_NODE = wx.xrc.XML_ATTRIBUTE_NODE
wxXML_TEXT_NODE = wx.xrc.XML_TEXT_NODE
wxXML_CDATA_SECTION_NODE = wx.xrc.XML_CDATA_SECTION_NODE
wxXML_ENTITY_REF_NODE = wx.xrc.XML_ENTITY_REF_NODE
wxXML_ENTITY_NODE = wx.xrc.XML_ENTITY_NODE
wxXML_PI_NODE = wx.xrc.XML_PI_NODE
wxXML_COMMENT_NODE = wx.xrc.XML_COMMENT_NODE
wxXML_DOCUMENT_NODE = wx.xrc.XML_DOCUMENT_NODE
wxXML_DOCUMENT_TYPE_NODE = wx.xrc.XML_DOCUMENT_TYPE_NODE
wxXML_DOCUMENT_FRAG_NODE = wx.xrc.XML_DOCUMENT_FRAG_NODE
wxXML_NOTATION_NODE = wx.xrc.XML_NOTATION_NODE
wxXML_HTML_DOCUMENT_NODE = wx.xrc.XML_HTML_DOCUMENT_NODE
wxXmlProperty = wx.xrc.XmlProperty
wxXmlPropertyPtr = wx.xrc.XmlPropertyPtr
wxXmlNodeEasy = wx.xrc.XmlNodeEasy
wxXmlNode = wx.xrc.XmlNode
wxXmlNodePtr = wx.xrc.XmlNodePtr
wxXmlDocumentFromStream = wx.xrc.XmlDocumentFromStream
wxEmptyXmlDocument = wx.xrc.EmptyXmlDocument
wxXmlDocument = wx.xrc.XmlDocument
wxXmlDocumentPtr = wx.xrc.XmlDocumentPtr
wxXmlResourceHandler = wx.xrc.XmlResourceHandler
wxXmlResourceHandlerPtr = wx.xrc.XmlResourceHandlerPtr
XRCID   = wx.xrc.XRCID
XRCCTRL = wx.xrc.XRCCTRL
wxTheXmlResource = wx.xrc.TheXmlResource


