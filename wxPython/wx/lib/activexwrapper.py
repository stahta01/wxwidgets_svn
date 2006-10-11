#----------------------------------------------------------------------
# Name:        wxPython.lib.activexwrapper
# Purpose:     a wxWindow derived class that can hold an ActiveX control
#
# Author:      Robin Dunn
#
# RCS-ID:      $Id$
# Copyright:   (c) 2000 by Total Control Software
# Licence:     wxWindows license
#----------------------------------------------------------------------
# 11/30/2003 - Jeff Grimmett (grimmtooth@softhome.net)
#
# o Updated for wx namespace
# o Tested with updated demo
# 

import  wx

try:
    import win32ui
    import pywin.mfc.activex
    import win32com.client
except ImportError:
    import sys
    if hasattr(sys, "frozen"):
        import os, win32api
        dllpath = os.path.join(win32api.GetSystemDirectory(), 'MFC71.DLL')
        if sys.version[:3] >= '2.4' and not os.path.exists(dllpath):
            message = "%s not found" % dllpath
        else:
            raise       # original error message
    else:
        message = "ActiveXWrapper requires PythonWin. Please install the PyWin32 package." 
    raise ImportError(message) 

##from win32con import WS_TABSTOP, WS_VISIBLE
WS_TABSTOP = 0x00010000
WS_VISIBLE = 0x10000000

#----------------------------------------------------------------------


def MakeActiveXClass(CoClass, eventClass=None, eventObj=None):
    """
    Dynamically construct a new class that derives from wxWindow, the
    ActiveX control and the appropriate COM classes.  This new class
    can be used just like the wxWindow class, but will also respond
    appropriately to the methods and properties of the COM object.  If
    this class, a derived class or a mix-in class has method names
    that match the COM object's event names, they will be called
    automatically.

    CoClass -- A COM control class from a module generated by
            makepy.py from a COM TypeLibrary.  Can also accept a
            CLSID.

    eventClass -- If given, this class will be added to the set of
            base classes that the new class is drived from.  It is
            good for mix-in classes for catching events.

    eventObj -- If given, this object will be searched for attributes
            by the new class's __getattr__ method, (like a mix-in
            object.)  This is useful if you want to catch COM
            callbacks in an existing object, (such as the parent
            window.)

    """


    if type(CoClass) == type(""):
        # use the CLSID to get the real class
        CoClass = win32com.client.CLSIDToClass(CoClass)

    # determine the base classes
    axEventClass = CoClass.default_source
    baseClasses = [pywin.mfc.activex.Control, wx.Window, CoClass, axEventClass]
    if eventClass:
        baseClasses.append(eventClass)
    baseClasses = tuple(baseClasses)

    # define the class attributes
    className = 'AXControl_'+CoClass.__name__
    classDict = { '__init__'    : axw__init__,
                  '__getattr__' : axw__getattr__,
                  'axw_OnSize'  : axw_OnSize,
                  'axw_OEB'     : axw_OEB,
                  '_name'       : className,
                  '_eventBase'  : axEventClass,
                  '_eventObj'   : eventObj,
                  'Cleanup'     : axw_Cleanup,
                  }

    # make a new class object
    import new
    classObj = new.classobj(className, baseClasses, classDict)
    return classObj




# These functions will be used as methods in the new class
def axw__init__(self, parent, ID=-1, pos=wx.DefaultPosition, size=wx.DefaultSize, style=0):
    
    # init base classes
    pywin.mfc.activex.Control.__init__(self)
    wx.Window.__init__( self, parent, -1, pos, size, style|wx.NO_FULL_REPAINT_ON_RESIZE)
    self.this.own(False)  # this should be set in wx.Window.__init__ when it calls _setOORInfo, but...
        
    win32ui.EnableControlContainer()
    self._eventObj = self._eventObj  # move from class to instance

    # create a pythonwin wrapper around this wxWindow
    handle = self.GetHandle()
    self._wnd = win32ui.CreateWindowFromHandle(handle)

    # create the control
    sz = self.GetSize()
    self.CreateControl(self._name, WS_TABSTOP | WS_VISIBLE,
                       (0, 0, sz.width, sz.height), self._wnd, ID)

    # init the ax events part of the object
    self._eventBase.__init__(self, self._dispobj_)

    # hook some wx events
    self.Bind(wx.EVT_SIZE, self.axw_OnSize)

def axw__getattr__(self, attr):
    try:
        return pywin.mfc.activex.Control.__getattr__(self, attr)
    except AttributeError:
        try:
            eo = self.__dict__['_eventObj']
            return getattr(eo, attr)
        except AttributeError:
            raise AttributeError('Attribute not found: %s' % attr)


def axw_OnSize(self, event):
    sz = self.GetClientSize()                        # get wxWindow size
    self.MoveWindow((0, 0, sz.width, sz.height), 1)  # move the AXControl


def axw_OEB(self, event):
    pass


def axw_Cleanup(self):
    #del self._wnd
    self.close()
    pass





