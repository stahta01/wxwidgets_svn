# This file was created automatically by SWIG.
# Don't modify this file, modify the SWIG interface instead.

"""
`GLCanvas` provides an OpenGL Context on a `wx.Window`.
"""

import _glcanvas

def _swig_setattr_nondynamic(self,class_type,name,value,static=1):
    if (name == "this"):
        if isinstance(value, class_type):
            self.__dict__[name] = value.this
            if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
            del value.thisown
            return
    method = class_type.__swig_setmethods__.get(name,None)
    if method: return method(self,value)
    if (not static) or hasattr(self,name) or (name == "thisown"):
        self.__dict__[name] = value
    else:
        raise AttributeError("You cannot add attributes to %s" % self)

def _swig_setattr(self,class_type,name,value):
    return _swig_setattr_nondynamic(self,class_type,name,value,0)

def _swig_getattr(self,class_type,name):
    method = class_type.__swig_getmethods__.get(name,None)
    if method: return method(self)
    raise AttributeError,name

import types
try:
    _object = types.ObjectType
    _newclass = 1
except AttributeError:
    class _object : pass
    _newclass = 0
del types


def _swig_setattr_nondynamic_method(set):
    def set_attr(self,name,value):
        if hasattr(self,name) or (name in ("this", "thisown")):
            set(self,name,value)
        else:
            raise AttributeError("You cannot add attributes to %s" % self)
    return set_attr


import _core
wx = _core 
__docfilter__ = wx.__DocFilter(globals()) 
class GLContext(_core.Object):
    """Proxy of C++ GLContext class"""
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxGLContext instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """
        __init__(self, bool isRGB, GLCanvas win, wxPalette palette=wxNullPalette, 
            GLContext other=None) -> GLContext
        """
        newobj = _glcanvas.new_GLContext(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
    def __del__(self, destroy=_glcanvas.delete_GLContext):
        """__del__(self)"""
        try:
            if self.thisown: destroy(self)
        except: pass

    def SetCurrent(*args, **kwargs):
        """SetCurrent(self)"""
        return _glcanvas.GLContext_SetCurrent(*args, **kwargs)

    def SetColour(*args, **kwargs):
        """SetColour(self, String colour)"""
        return _glcanvas.GLContext_SetColour(*args, **kwargs)

    def SwapBuffers(*args, **kwargs):
        """SwapBuffers(self)"""
        return _glcanvas.GLContext_SwapBuffers(*args, **kwargs)

    def GetWindow(*args, **kwargs):
        """GetWindow(self) -> Window"""
        return _glcanvas.GLContext_GetWindow(*args, **kwargs)


class GLContextPtr(GLContext):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = GLContext
_glcanvas.GLContext_swigregister(GLContextPtr)
cvar = _glcanvas.cvar
GLCanvasNameStr = cvar.GLCanvasNameStr

WX_GL_RGBA = _glcanvas.WX_GL_RGBA
WX_GL_BUFFER_SIZE = _glcanvas.WX_GL_BUFFER_SIZE
WX_GL_LEVEL = _glcanvas.WX_GL_LEVEL
WX_GL_DOUBLEBUFFER = _glcanvas.WX_GL_DOUBLEBUFFER
WX_GL_STEREO = _glcanvas.WX_GL_STEREO
WX_GL_AUX_BUFFERS = _glcanvas.WX_GL_AUX_BUFFERS
WX_GL_MIN_RED = _glcanvas.WX_GL_MIN_RED
WX_GL_MIN_GREEN = _glcanvas.WX_GL_MIN_GREEN
WX_GL_MIN_BLUE = _glcanvas.WX_GL_MIN_BLUE
WX_GL_MIN_ALPHA = _glcanvas.WX_GL_MIN_ALPHA
WX_GL_DEPTH_SIZE = _glcanvas.WX_GL_DEPTH_SIZE
WX_GL_STENCIL_SIZE = _glcanvas.WX_GL_STENCIL_SIZE
WX_GL_MIN_ACCUM_RED = _glcanvas.WX_GL_MIN_ACCUM_RED
WX_GL_MIN_ACCUM_GREEN = _glcanvas.WX_GL_MIN_ACCUM_GREEN
WX_GL_MIN_ACCUM_BLUE = _glcanvas.WX_GL_MIN_ACCUM_BLUE
WX_GL_MIN_ACCUM_ALPHA = _glcanvas.WX_GL_MIN_ACCUM_ALPHA
class GLCanvas(_core.Window):
    """Proxy of C++ GLCanvas class"""
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxGLCanvas instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """
        __init__(self, Window parent, int id=-1, Point pos=DefaultPosition, 
            Size size=DefaultSize, long style=0, String name=GLCanvasNameStr, 
            int attribList=None, wxPalette palette=wxNullPalette) -> GLCanvas
        """
        newobj = _glcanvas.new_GLCanvas(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
        self._setOORInfo(self)

    def SetCurrent(*args, **kwargs):
        """SetCurrent(self)"""
        return _glcanvas.GLCanvas_SetCurrent(*args, **kwargs)

    def SetColour(*args, **kwargs):
        """SetColour(self, String colour)"""
        return _glcanvas.GLCanvas_SetColour(*args, **kwargs)

    def SwapBuffers(*args, **kwargs):
        """SwapBuffers(self)"""
        return _glcanvas.GLCanvas_SwapBuffers(*args, **kwargs)

    def GetContext(*args, **kwargs):
        """GetContext(self) -> GLContext"""
        return _glcanvas.GLCanvas_GetContext(*args, **kwargs)


class GLCanvasPtr(GLCanvas):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = GLCanvas
_glcanvas.GLCanvas_swigregister(GLCanvasPtr)

def GLCanvasWithContext(*args, **kwargs):
    """
    GLCanvasWithContext(Window parent, GLContext shared=None, int id=-1, Point pos=DefaultPosition, 
        Size size=DefaultSize, 
        long style=0, String name=GLCanvasNameStr, 
        int attribList=None, wxPalette palette=wxNullPalette) -> GLCanvas
    """
    val = _glcanvas.new_GLCanvasWithContext(*args, **kwargs)
    val.thisown = 1
    val._setOORInfo(val)
    return val


