/*
 * FILE : msw/glcanvas.cpp
 * 
 * This file was automatically generated by :
 * Simplified Wrapper and Interface Generator (SWIG)
 * Version 1.1 (Build 810)
 * 
 * Portions Copyright (c) 1995-1998
 * The University of Utah and The Regents of the University of California.
 * Permission is granted to distribute this file in any manner provided
 * this notice remains intact.
 * 
 * Do not make changes to this file--changes will be lost!
 *
 */


#define SWIGCODE
/* Implementation : PYTHON */

#define SWIGPYTHON
#include <string.h>
#include <stdlib.h>
/* Definitions for Windows/Unix exporting */
#if defined(__WIN32__)
#   if defined(_MSC_VER)
#	define SWIGEXPORT(a) __declspec(dllexport) a
#   else
#	if defined(__BORLANDC__)
#	    define SWIGEXPORT(a) a _export 
#	else
#	    define SWIGEXPORT(a) a 
#	endif
#   endif
#else
#   define SWIGEXPORT(a) a 
#endif

#ifdef __cplusplus
extern "C" {
#endif
#include "Python.h"
extern void SWIG_MakePtr(char *, void *, char *);
extern void SWIG_RegisterMapping(char *, char *, void *(*)(void *));
extern char *SWIG_GetPtr(char *, void **, char *);
extern char *SWIG_GetPtrObj(PyObject *, void **, char *);
extern void SWIG_addvarlink(PyObject *, char *, PyObject *(*)(void), int (*)(PyObject *));
extern PyObject *SWIG_newvarlink(void);
#ifdef __cplusplus
}
#endif
#define SWIG_init    initglcanvasc

#define SWIG_name    "glcanvasc"

#include "helpers.h"
#include <wx/glcanvas.h>

static PyObject* l_output_helper(PyObject* target, PyObject* o) {
    PyObject*   o2;
    PyObject*   o3;
    if (!target) {                   
        target = o;
    } else if (target == Py_None) {  
        Py_DECREF(Py_None);
        target = o;
    } else {                         
        if (!PyList_Check(target)) {
            o2 = target;
            target = PyList_New(0);
            PyList_Append(target, o2);
	    Py_XDECREF(o2);
        }
        PyList_Append(target,o);
	Py_XDECREF(o);
    }
    return target;
}

static PyObject* t_output_helper(PyObject* target, PyObject* o) {
    PyObject*   o2;
    PyObject*   o3;

    if (!target) {                   
        target = o;
    } else if (target == Py_None) {  
        Py_DECREF(Py_None);
        target = o;
    } else {                         
        if (!PyTuple_Check(target)) {
            o2 = target;
            target = PyTuple_New(1);
            PyTuple_SetItem(target, 0, o2);
        }
        o3 = PyTuple_New(1);            
        PyTuple_SetItem(o3, 0, o);      

        o2 = target;
        target = PySequence_Concat(o2, o3); 
        Py_DECREF(o2);                      
        Py_DECREF(o3);
    }
    return target;
}

static char* wxStringErrorMsg = "string type is required for parameter";

#if defined(__WXMSW__)
    static wxString wxPyEmptyStr("");
    static wxPoint  wxPyDefaultPosition(-1, -1);
    static wxSize   wxPyDefaultSize(-1, -1);
#endif
#ifdef __cplusplus
extern "C" {
#endif
#define new_wxGLContext(_swigarg0,_swigarg1,_swigarg2) (new wxGLContext(_swigarg0,_swigarg1,_swigarg2))
static PyObject *_wrap_new_wxGLContext(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxGLContext * _result;
    bool  _arg0;
    wxGLCanvas * _arg1;
    wxPalette * _arg2 = (wxPalette *) &wxNullPalette;
    int tempbool0;
    PyObject * _argo1 = 0;
    PyObject * _argo2 = 0;
    char *_kwnames[] = { "isRGB","win","palette", NULL };
    char _ptemp[128];

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"iO|O:new_wxGLContext",_kwnames,&tempbool0,&_argo1,&_argo2)) 
        return NULL;
    _arg0 = (bool ) tempbool0;
    if (_argo1) {
        if (_argo1 == Py_None) { _arg1 = NULL; }
        else if (SWIG_GetPtrObj(_argo1,(void **) &_arg1,"_wxGLCanvas_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 2 of new_wxGLContext. Expected _wxGLCanvas_p.");
        return NULL;
        }
    }
    if (_argo2) {
        if (_argo2 == Py_None) { _arg2 = NULL; }
        else if (SWIG_GetPtrObj(_argo2,(void **) &_arg2,"_wxPalette_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 3 of new_wxGLContext. Expected _wxPalette_p.");
        return NULL;
        }
    }
{
    wxPy_BEGIN_ALLOW_THREADS;
        _result = (wxGLContext *)new_wxGLContext(_arg0,_arg1,*_arg2);

    wxPy_END_ALLOW_THREADS;
}    if (_result) {
        SWIG_MakePtr(_ptemp, (char *) _result,"_wxGLContext_p");
        _resultobj = Py_BuildValue("s",_ptemp);
    } else {
        Py_INCREF(Py_None);
        _resultobj = Py_None;
    }
    return _resultobj;
}

#define delete_wxGLContext(_swigobj) (delete _swigobj)
static PyObject *_wrap_delete_wxGLContext(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxGLContext * _arg0;
    PyObject * _argo0 = 0;
    char *_kwnames[] = { "self", NULL };

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"O:delete_wxGLContext",_kwnames,&_argo0)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxGLContext_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of delete_wxGLContext. Expected _wxGLContext_p.");
        return NULL;
        }
    }
{
    wxPy_BEGIN_ALLOW_THREADS;
        delete_wxGLContext(_arg0);

    wxPy_END_ALLOW_THREADS;
}    Py_INCREF(Py_None);
    _resultobj = Py_None;
    return _resultobj;
}

#define wxGLContext_SetCurrent(_swigobj)  (_swigobj->SetCurrent())
static PyObject *_wrap_wxGLContext_SetCurrent(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxGLContext * _arg0;
    PyObject * _argo0 = 0;
    char *_kwnames[] = { "self", NULL };

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"O:wxGLContext_SetCurrent",_kwnames,&_argo0)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxGLContext_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxGLContext_SetCurrent. Expected _wxGLContext_p.");
        return NULL;
        }
    }
{
    wxPy_BEGIN_ALLOW_THREADS;
        wxGLContext_SetCurrent(_arg0);

    wxPy_END_ALLOW_THREADS;
}    Py_INCREF(Py_None);
    _resultobj = Py_None;
    return _resultobj;
}

#define wxGLContext_SetColour(_swigobj,_swigarg0)  (_swigobj->SetColour(_swigarg0))
static PyObject *_wrap_wxGLContext_SetColour(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxGLContext * _arg0;
    char * _arg1;
    PyObject * _argo0 = 0;
    char *_kwnames[] = { "self","colour", NULL };

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"Os:wxGLContext_SetColour",_kwnames,&_argo0,&_arg1)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxGLContext_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxGLContext_SetColour. Expected _wxGLContext_p.");
        return NULL;
        }
    }
{
    wxPy_BEGIN_ALLOW_THREADS;
        wxGLContext_SetColour(_arg0,_arg1);

    wxPy_END_ALLOW_THREADS;
}    Py_INCREF(Py_None);
    _resultobj = Py_None;
    return _resultobj;
}

#define wxGLContext_SwapBuffers(_swigobj)  (_swigobj->SwapBuffers())
static PyObject *_wrap_wxGLContext_SwapBuffers(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxGLContext * _arg0;
    PyObject * _argo0 = 0;
    char *_kwnames[] = { "self", NULL };

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"O:wxGLContext_SwapBuffers",_kwnames,&_argo0)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxGLContext_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxGLContext_SwapBuffers. Expected _wxGLContext_p.");
        return NULL;
        }
    }
{
    wxPy_BEGIN_ALLOW_THREADS;
        wxGLContext_SwapBuffers(_arg0);

    wxPy_END_ALLOW_THREADS;
}    Py_INCREF(Py_None);
    _resultobj = Py_None;
    return _resultobj;
}

#define wxGLContext_GetWindow(_swigobj)  (_swigobj->GetWindow())
static PyObject *_wrap_wxGLContext_GetWindow(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxWindow * _result;
    wxGLContext * _arg0;
    PyObject * _argo0 = 0;
    char *_kwnames[] = { "self", NULL };
    char _ptemp[128];

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"O:wxGLContext_GetWindow",_kwnames,&_argo0)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxGLContext_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxGLContext_GetWindow. Expected _wxGLContext_p.");
        return NULL;
        }
    }
{
    wxPy_BEGIN_ALLOW_THREADS;
        _result = (wxWindow *)wxGLContext_GetWindow(_arg0);

    wxPy_END_ALLOW_THREADS;
}    if (_result) {
        SWIG_MakePtr(_ptemp, (char *) _result,"_wxWindow_p");
        _resultobj = Py_BuildValue("s",_ptemp);
    } else {
        Py_INCREF(Py_None);
        _resultobj = Py_None;
    }
    return _resultobj;
}

static void *SwigwxGLCanvasTowxScrolledWindow(void *ptr) {
    wxGLCanvas *src;
    wxScrolledWindow *dest;
    src = (wxGLCanvas *) ptr;
    dest = (wxScrolledWindow *) src;
    return (void *) dest;
}

static void *SwigwxGLCanvasTowxPanel(void *ptr) {
    wxGLCanvas *src;
    wxPanel *dest;
    src = (wxGLCanvas *) ptr;
    dest = (wxPanel *) src;
    return (void *) dest;
}

static void *SwigwxGLCanvasTowxWindow(void *ptr) {
    wxGLCanvas *src;
    wxWindow *dest;
    src = (wxGLCanvas *) ptr;
    dest = (wxWindow *) src;
    return (void *) dest;
}

static void *SwigwxGLCanvasTowxEvtHandler(void *ptr) {
    wxGLCanvas *src;
    wxEvtHandler *dest;
    src = (wxGLCanvas *) ptr;
    dest = (wxEvtHandler *) src;
    return (void *) dest;
}

#define new_wxGLCanvas(_swigarg0,_swigarg1,_swigarg2,_swigarg3,_swigarg4,_swigarg5,_swigarg6,_swigarg7) (new wxGLCanvas(_swigarg0,_swigarg1,_swigarg2,_swigarg3,_swigarg4,_swigarg5,_swigarg6,_swigarg7))
static PyObject *_wrap_new_wxGLCanvas(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxGLCanvas * _result;
    wxWindow * _arg0;
    wxWindowID  _arg1 = (wxWindowID ) -1;
    wxPoint * _arg2 = (wxPoint *) &wxPyDefaultPosition;
    wxSize * _arg3 = (wxSize *) &wxPyDefaultSize;
    long  _arg4 = (long ) 0;
    char * _arg5 = (char *) "GLCanvas";
    int * _arg6 = (int *) 0;
    wxPalette * _arg7 = (wxPalette *) &wxNullPalette;
    PyObject * _argo0 = 0;
    wxPoint  temp;
    PyObject * _obj2 = 0;
    wxSize  temp0;
    PyObject * _obj3 = 0;
    PyObject * _argo6 = 0;
    PyObject * _argo7 = 0;
    char *_kwnames[] = { "parent","id","pos","size","style","name","attribList","palette", NULL };
    char _ptemp[128];

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"O|iOOlsOO:new_wxGLCanvas",_kwnames,&_argo0,&_arg1,&_obj2,&_obj3,&_arg4,&_arg5,&_argo6,&_argo7)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxWindow_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of new_wxGLCanvas. Expected _wxWindow_p.");
        return NULL;
        }
    }
    if (_obj2)
{
    _arg2 = &temp;
    if (! wxPoint_helper(_obj2, &_arg2))
        return NULL;
}
    if (_obj3)
{
    _arg3 = &temp0;
    if (! wxSize_helper(_obj3, &_arg3))
        return NULL;
}
    if (_argo6) {
        if (_argo6 == Py_None) { _arg6 = NULL; }
        else if (SWIG_GetPtrObj(_argo6,(void **) &_arg6,"_int_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 7 of new_wxGLCanvas. Expected _int_p.");
        return NULL;
        }
    }
    if (_argo7) {
        if (_argo7 == Py_None) { _arg7 = NULL; }
        else if (SWIG_GetPtrObj(_argo7,(void **) &_arg7,"_wxPalette_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 8 of new_wxGLCanvas. Expected _wxPalette_p.");
        return NULL;
        }
    }
{
    wxPy_BEGIN_ALLOW_THREADS;
        _result = (wxGLCanvas *)new_wxGLCanvas(_arg0,_arg1,*_arg2,*_arg3,_arg4,_arg5,_arg6,*_arg7);

    wxPy_END_ALLOW_THREADS;
}    if (_result) {
        SWIG_MakePtr(_ptemp, (char *) _result,"_wxGLCanvas_p");
        _resultobj = Py_BuildValue("s",_ptemp);
    } else {
        Py_INCREF(Py_None);
        _resultobj = Py_None;
    }
    return _resultobj;
}

#define wxGLCanvas_SetCurrent(_swigobj)  (_swigobj->SetCurrent())
static PyObject *_wrap_wxGLCanvas_SetCurrent(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxGLCanvas * _arg0;
    PyObject * _argo0 = 0;
    char *_kwnames[] = { "self", NULL };

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"O:wxGLCanvas_SetCurrent",_kwnames,&_argo0)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxGLCanvas_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxGLCanvas_SetCurrent. Expected _wxGLCanvas_p.");
        return NULL;
        }
    }
{
    wxPy_BEGIN_ALLOW_THREADS;
        wxGLCanvas_SetCurrent(_arg0);

    wxPy_END_ALLOW_THREADS;
}    Py_INCREF(Py_None);
    _resultobj = Py_None;
    return _resultobj;
}

#define wxGLCanvas_SetColour(_swigobj,_swigarg0)  (_swigobj->SetColour(_swigarg0))
static PyObject *_wrap_wxGLCanvas_SetColour(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxGLCanvas * _arg0;
    char * _arg1;
    PyObject * _argo0 = 0;
    char *_kwnames[] = { "self","colour", NULL };

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"Os:wxGLCanvas_SetColour",_kwnames,&_argo0,&_arg1)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxGLCanvas_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxGLCanvas_SetColour. Expected _wxGLCanvas_p.");
        return NULL;
        }
    }
{
    wxPy_BEGIN_ALLOW_THREADS;
        wxGLCanvas_SetColour(_arg0,_arg1);

    wxPy_END_ALLOW_THREADS;
}    Py_INCREF(Py_None);
    _resultobj = Py_None;
    return _resultobj;
}

#define wxGLCanvas_SwapBuffers(_swigobj)  (_swigobj->SwapBuffers())
static PyObject *_wrap_wxGLCanvas_SwapBuffers(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxGLCanvas * _arg0;
    PyObject * _argo0 = 0;
    char *_kwnames[] = { "self", NULL };

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"O:wxGLCanvas_SwapBuffers",_kwnames,&_argo0)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxGLCanvas_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxGLCanvas_SwapBuffers. Expected _wxGLCanvas_p.");
        return NULL;
        }
    }
{
    wxPy_BEGIN_ALLOW_THREADS;
        wxGLCanvas_SwapBuffers(_arg0);

    wxPy_END_ALLOW_THREADS;
}    Py_INCREF(Py_None);
    _resultobj = Py_None;
    return _resultobj;
}

#define wxGLCanvas_GetContext(_swigobj)  (_swigobj->GetContext())
static PyObject *_wrap_wxGLCanvas_GetContext(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxGLContext * _result;
    wxGLCanvas * _arg0;
    PyObject * _argo0 = 0;
    char *_kwnames[] = { "self", NULL };
    char _ptemp[128];

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"O:wxGLCanvas_GetContext",_kwnames,&_argo0)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxGLCanvas_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxGLCanvas_GetContext. Expected _wxGLCanvas_p.");
        return NULL;
        }
    }
{
    wxPy_BEGIN_ALLOW_THREADS;
        _result = (wxGLContext *)wxGLCanvas_GetContext(_arg0);

    wxPy_END_ALLOW_THREADS;
}    if (_result) {
        SWIG_MakePtr(_ptemp, (char *) _result,"_wxGLContext_p");
        _resultobj = Py_BuildValue("s",_ptemp);
    } else {
        Py_INCREF(Py_None);
        _resultobj = Py_None;
    }
    return _resultobj;
}

static PyMethodDef glcanvascMethods[] = {
	 { "wxGLCanvas_GetContext", (PyCFunction) _wrap_wxGLCanvas_GetContext, METH_VARARGS | METH_KEYWORDS },
	 { "wxGLCanvas_SwapBuffers", (PyCFunction) _wrap_wxGLCanvas_SwapBuffers, METH_VARARGS | METH_KEYWORDS },
	 { "wxGLCanvas_SetColour", (PyCFunction) _wrap_wxGLCanvas_SetColour, METH_VARARGS | METH_KEYWORDS },
	 { "wxGLCanvas_SetCurrent", (PyCFunction) _wrap_wxGLCanvas_SetCurrent, METH_VARARGS | METH_KEYWORDS },
	 { "new_wxGLCanvas", (PyCFunction) _wrap_new_wxGLCanvas, METH_VARARGS | METH_KEYWORDS },
	 { "wxGLContext_GetWindow", (PyCFunction) _wrap_wxGLContext_GetWindow, METH_VARARGS | METH_KEYWORDS },
	 { "wxGLContext_SwapBuffers", (PyCFunction) _wrap_wxGLContext_SwapBuffers, METH_VARARGS | METH_KEYWORDS },
	 { "wxGLContext_SetColour", (PyCFunction) _wrap_wxGLContext_SetColour, METH_VARARGS | METH_KEYWORDS },
	 { "wxGLContext_SetCurrent", (PyCFunction) _wrap_wxGLContext_SetCurrent, METH_VARARGS | METH_KEYWORDS },
	 { "delete_wxGLContext", (PyCFunction) _wrap_delete_wxGLContext, METH_VARARGS | METH_KEYWORDS },
	 { "new_wxGLContext", (PyCFunction) _wrap_new_wxGLContext, METH_VARARGS | METH_KEYWORDS },
	 { NULL, NULL }
};
#ifdef __cplusplus
}
#endif
/*
 * This table is used by the pointer type-checker
 */
static struct { char *n1; char *n2; void *(*pcnv)(void *); } _swig_mapping[] = {
    { "_wxAcceleratorTable","_class_wxAcceleratorTable",0},
    { "_wxEvent","_class_wxEvent",0},
    { "_class_wxActivateEvent","_wxActivateEvent",0},
    { "_signed_long","_long",0},
    { "_wxMenuEvent","_class_wxMenuEvent",0},
    { "_class_wxJPEGHandler","_wxJPEGHandler",0},
    { "_wxPyBitmapDataObject","_class_wxPyBitmapDataObject",0},
    { "_wxBitmapDataObject","_class_wxBitmapDataObject",0},
    { "_class_wxPyCommandEvent","_wxPyCommandEvent",0},
    { "_wxBMPHandler","_class_wxBMPHandler",0},
    { "_wxImage","_class_wxImage",0},
    { "_wxPrintQuality","_wxCoord",0},
    { "_wxPrintQuality","_int",0},
    { "_wxPrintQuality","_signed_int",0},
    { "_wxPrintQuality","_unsigned_int",0},
    { "_wxPrintQuality","_wxWindowID",0},
    { "_wxPrintQuality","_uint",0},
    { "_wxPrintQuality","_EBool",0},
    { "_wxPrintQuality","_size_t",0},
    { "_class_wxCustomDataObject","_wxCustomDataObject",0},
    { "_wxFontData","_class_wxFontData",0},
    { "_class_wxRegionIterator","_wxRegionIterator",0},
    { "_class_wxPyTextDropTarget","_wxPyTextDropTarget",0},
    { "_class_wxMenuBar","_wxMenuBar",0},
    { "_class_wxPyTreeItemData","_wxPyTreeItemData",0},
    { "_class_wxStaticBoxSizer","_wxStaticBoxSizer",0},
    { "_class_wxEvtHandler","_class_wxGLCanvas",SwigwxGLCanvasTowxEvtHandler},
    { "_class_wxEvtHandler","_wxGLCanvas",SwigwxGLCanvasTowxEvtHandler},
    { "_class_wxEvtHandler","_wxEvtHandler",0},
    { "_wxPaintEvent","_class_wxPaintEvent",0},
    { "_wxGIFHandler","_class_wxGIFHandler",0},
    { "_wxPySizer","_class_wxPySizer",0},
    { "_wxIndividualLayoutConstraint","_class_wxIndividualLayoutConstraint",0},
    { "_wxCursor","_class_wxCursor",0},
    { "_wxNotifyEvent","_class_wxNotifyEvent",0},
    { "_wxImageHandler","_class_wxImageHandler",0},
    { "_class_wxToolBarBase","_wxToolBarBase",0},
    { "_class_wxTreeCtrl","_wxTreeCtrl",0},
    { "_wxMask","_class_wxMask",0},
    { "_wxToolTip","_class_wxToolTip",0},
    { "_wxPNGHandler","_class_wxPNGHandler",0},
    { "_class_wxColourData","_wxColourData",0},
    { "_class_wxPageSetupDialogData","_wxPageSetupDialogData",0},
    { "_wxPrinter","_class_wxPrinter",0},
    { "_wxPen","_class_wxPen",0},
    { "_wxUpdateUIEvent","_class_wxUpdateUIEvent",0},
    { "_byte","_unsigned_char",0},
    { "_wxDataObject","_class_wxDataObject",0},
    { "_class_wxPyFontEnumerator","_wxPyFontEnumerator",0},
    { "_wxStaticBox","_class_wxStaticBox",0},
    { "_wxPyDataObjectSimple","_class_wxPyDataObjectSimple",0},
    { "_wxPyDropSource","_class_wxPyDropSource",0},
    { "_wxChoice","_class_wxChoice",0},
    { "_wxSlider","_class_wxSlider",0},
    { "_wxNotebookEvent","_class_wxNotebookEvent",0},
    { "_wxPyPrintout","_class_wxPyPrintout",0},
    { "_long","_wxDash",0},
    { "_long","_unsigned_long",0},
    { "_long","_signed_long",0},
    { "_wxImageList","_class_wxImageList",0},
    { "_wxDataObjectSimple","_class_wxDataObjectSimple",0},
    { "_wxDropFilesEvent","_class_wxDropFilesEvent",0},
    { "_wxBitmapButton","_class_wxBitmapButton",0},
    { "_wxSashWindow","_class_wxSashWindow",0},
    { "_class_wxSizer","_wxSizer",0},
    { "_class_wxTIFFHandler","_wxTIFFHandler",0},
    { "_class_wxPrintDialogData","_wxPrintDialogData",0},
    { "_wxGLContext","_class_wxGLContext",0},
    { "_class_wxAcceleratorTable","_wxAcceleratorTable",0},
    { "_class_wxClipboard","_wxClipboard",0},
    { "_class_wxGauge","_wxGauge",0},
    { "_class_wxSashEvent","_wxSashEvent",0},
    { "_wxDC","_class_wxDC",0},
    { "_wxSizerItem","_class_wxSizerItem",0},
    { "_class_wxBitmapDataObject","_wxBitmapDataObject",0},
    { "_wxListEvent","_class_wxListEvent",0},
    { "_class_wxSingleChoiceDialog","_wxSingleChoiceDialog",0},
    { "_wxProgressDialog","_class_wxProgressDialog",0},
    { "_class_wxBMPHandler","_wxBMPHandler",0},
    { "_wxPrintPreview","_class_wxPrintPreview",0},
    { "_wxSpinEvent","_class_wxSpinEvent",0},
    { "_wxSashLayoutWindow","_class_wxSashLayoutWindow",0},
    { "_size_t","_wxCoord",0},
    { "_size_t","_wxPrintQuality",0},
    { "_size_t","_unsigned_int",0},
    { "_size_t","_int",0},
    { "_size_t","_wxWindowID",0},
    { "_size_t","_uint",0},
    { "_class_wxRealPoint","_wxRealPoint",0},
    { "_wxNavigationKeyEvent","_class_wxNavigationKeyEvent",0},
    { "_wxPNMHandler","_class_wxPNMHandler",0},
    { "_wxPrinterDC","_class_wxPrinterDC",0},
    { "_wxWindowCreateEvent","_class_wxWindowCreateEvent",0},
    { "_class_wxMenuItem","_wxMenuItem",0},
    { "_class_wxPaintEvent","_wxPaintEvent",0},
    { "_wxSysColourChangedEvent","_class_wxSysColourChangedEvent",0},
    { "_class_wxStatusBar","_wxStatusBar",0},
    { "_class_wxGIFHandler","_wxGIFHandler",0},
    { "_class_wxPySizer","_wxPySizer",0},
    { "_wxPanel","_class_wxGLCanvas",SwigwxGLCanvasTowxPanel},
    { "_wxPanel","_wxGLCanvas",SwigwxGLCanvasTowxPanel},
    { "_wxPanel","_class_wxPanel",0},
    { "_wxInitDialogEvent","_class_wxInitDialogEvent",0},
    { "_wxCheckBox","_class_wxCheckBox",0},
    { "_wxPyEvent","_class_wxPyEvent",0},
    { "_wxTextCtrl","_class_wxTextCtrl",0},
    { "_class_wxMask","_wxMask",0},
    { "_wxTextDataObject","_class_wxTextDataObject",0},
    { "_class_wxKeyEvent","_wxKeyEvent",0},
    { "_class_wxToolTip","_wxToolTip",0},
    { "_class_wxPNGHandler","_wxPNGHandler",0},
    { "_wxColour","_class_wxColour",0},
    { "_class_wxDialog","_wxDialog",0},
    { "_wxBusyCursor","_class_wxBusyCursor",0},
    { "_wxPageSetupDialog","_class_wxPageSetupDialog",0},
    { "_class_wxPrinter","_wxPrinter",0},
    { "_class_wxFileDataObject","_wxFileDataObject",0},
    { "_wxIdleEvent","_class_wxIdleEvent",0},
    { "_class_wxUpdateUIEvent","_wxUpdateUIEvent",0},
    { "_wxToolBar","_class_wxToolBar",0},
    { "_class_wxDataObject","_wxDataObject",0},
    { "_wxCaret","_class_wxCaret",0},
    { "_wxStaticLine","_class_wxStaticLine",0},
    { "_class_wxLayoutAlgorithm","_wxLayoutAlgorithm",0},
    { "_wxBrush","_class_wxBrush",0},
    { "_wxMiniFrame","_class_wxMiniFrame",0},
    { "_class_wxNotebookEvent","_wxNotebookEvent",0},
    { "_class_wxPyPrintout","_wxPyPrintout",0},
    { "_wxDataFormat","_class_wxDataFormat",0},
    { "_class_wxDataObjectSimple","_wxDataObjectSimple",0},
    { "_class_wxSashWindow","_wxSashWindow",0},
    { "_wxShowEvent","_class_wxShowEvent",0},
    { "_uint","_wxCoord",0},
    { "_uint","_wxPrintQuality",0},
    { "_uint","_size_t",0},
    { "_uint","_unsigned_int",0},
    { "_uint","_int",0},
    { "_uint","_wxWindowID",0},
    { "_wxPyValidator","_class_wxPyValidator",0},
    { "_class_wxEvent","_wxEvent",0},
    { "_wxCheckListBox","_class_wxCheckListBox",0},
    { "_wxSplitterEvent","_class_wxSplitterEvent",0},
    { "_wxRect","_class_wxRect",0},
    { "_wxCommandEvent","_class_wxCommandEvent",0},
    { "_wxSizeEvent","_class_wxSizeEvent",0},
    { "_class_wxImage","_wxImage",0},
    { "_wxPoint","_class_wxPoint",0},
    { "_class_wxSashLayoutWindow","_wxSashLayoutWindow",0},
    { "_class_wxButton","_wxButton",0},
    { "_wxRadioBox","_class_wxRadioBox",0},
    { "_class_wxFontData","_wxFontData",0},
    { "_class_wxPNMHandler","_wxPNMHandler",0},
    { "_wxBoxSizer","_class_wxBoxSizer",0},
    { "_wxBitmap","_class_wxBitmap",0},
    { "_wxTaskBarIcon","_class_wxTaskBarIcon",0},
    { "_wxPrintDialog","_class_wxPrintDialog",0},
    { "_wxPyTimer","_class_wxPyTimer",0},
    { "_wxWindowDC","_class_wxWindowDC",0},
    { "_wxScrollBar","_class_wxScrollBar",0},
    { "_wxSpinButton","_class_wxSpinButton",0},
    { "_wxColourDialog","_class_wxColourDialog",0},
    { "_wxPrintData","_class_wxPrintData",0},
    { "_class_wxIndividualLayoutConstraint","_wxIndividualLayoutConstraint",0},
    { "_class_wxNotifyEvent","_wxNotifyEvent",0},
    { "_wxMessageDialog","_class_wxMessageDialog",0},
    { "_class_wxValidator","_wxValidator",0},
    { "_class_wxPyEvent","_wxPyEvent",0},
    { "_wxTextEntryDialog","_class_wxTextEntryDialog",0},
    { "_class_wxIconizeEvent","_wxIconizeEvent",0},
    { "_class_wxStaticBitmap","_wxStaticBitmap",0},
    { "_class_wxBusyCursor","_wxBusyCursor",0},
    { "_wxToolBarSimple","_class_wxToolBarSimple",0},
    { "_wxMDIChildFrame","_class_wxMDIChildFrame",0},
    { "_wxListItem","_class_wxListItem",0},
    { "_class_wxToolBar","_wxToolBar",0},
    { "_wxDropTarget","_class_wxDropTarget",0},
    { "_class_wxStaticLine","_wxStaticLine",0},
    { "_wxScrollEvent","_class_wxScrollEvent",0},
    { "_wxToolBarToolBase","_class_wxToolBarToolBase",0},
    { "_wxCalculateLayoutEvent","_class_wxCalculateLayoutEvent",0},
    { "_EBool","_wxCoord",0},
    { "_EBool","_wxPrintQuality",0},
    { "_EBool","_signed_int",0},
    { "_EBool","_int",0},
    { "_EBool","_wxWindowID",0},
    { "_class_wxRegion","_wxRegion",0},
    { "_class_wxDataFormat","_wxDataFormat",0},
    { "_class_wxDropFilesEvent","_wxDropFilesEvent",0},
    { "_wxWindowDestroyEvent","_class_wxWindowDestroyEvent",0},
    { "_class_wxPreviewFrame","_wxPreviewFrame",0},
    { "_wxStaticText","_class_wxStaticText",0},
    { "_wxFont","_class_wxFont",0},
    { "_class_wxPyDropTarget","_wxPyDropTarget",0},
    { "_wxCloseEvent","_class_wxCloseEvent",0},
    { "_class_wxSplitterEvent","_wxSplitterEvent",0},
    { "_wxNotebook","_class_wxNotebook",0},
    { "_unsigned_long","_wxDash",0},
    { "_unsigned_long","_long",0},
    { "_class_wxRect","_wxRect",0},
    { "_class_wxDC","_wxDC",0},
    { "_wxScrollWinEvent","_class_wxScrollWinEvent",0},
    { "_class_wxProgressDialog","_wxProgressDialog",0},
    { "_wxQueryNewPaletteEvent","_class_wxQueryNewPaletteEvent",0},
    { "_wxPyApp","_class_wxPyApp",0},
    { "_class_wxWindowCreateEvent","_wxWindowCreateEvent",0},
    { "_wxMDIParentFrame","_class_wxMDIParentFrame",0},
    { "_class_wxTreeEvent","_wxTreeEvent",0},
    { "_class_wxDirDialog","_wxDirDialog",0},
    { "_class_wxPyTimer","_wxPyTimer",0},
    { "_wxFocusEvent","_class_wxFocusEvent",0},
    { "_wxMaximizeEvent","_class_wxMaximizeEvent",0},
    { "_class_wxSpinButton","_wxSpinButton",0},
    { "_wxAcceleratorEntry","_class_wxAcceleratorEntry",0},
    { "_class_wxPanel","_class_wxGLCanvas",SwigwxGLCanvasTowxPanel},
    { "_class_wxPanel","_wxGLCanvas",SwigwxGLCanvasTowxPanel},
    { "_class_wxPanel","_wxPanel",0},
    { "_class_wxCheckBox","_wxCheckBox",0},
    { "_wxComboBox","_class_wxComboBox",0},
    { "_wxRadioButton","_class_wxRadioButton",0},
    { "_class_wxMessageDialog","_wxMessageDialog",0},
    { "_signed_int","_wxCoord",0},
    { "_signed_int","_wxPrintQuality",0},
    { "_signed_int","_EBool",0},
    { "_signed_int","_wxWindowID",0},
    { "_signed_int","_int",0},
    { "_class_wxTextCtrl","_wxTextCtrl",0},
    { "_wxLayoutConstraints","_class_wxLayoutConstraints",0},
    { "_wxMetaFileDC","_class_wxMetaFileDC",0},
    { "_class_wxTextDataObject","_wxTextDataObject",0},
    { "_wxMenu","_class_wxMenu",0},
    { "_class_wxMoveEvent","_wxMoveEvent",0},
    { "_wxListBox","_class_wxListBox",0},
    { "_wxScreenDC","_class_wxScreenDC",0},
    { "_class_wxToolBarSimple","_wxToolBarSimple",0},
    { "_class_wxMDIChildFrame","_wxMDIChildFrame",0},
    { "_WXTYPE","_short",0},
    { "_WXTYPE","_signed_short",0},
    { "_WXTYPE","_unsigned_short",0},
    { "_wxFileDialog","_class_wxFileDialog",0},
    { "_class_wxDropTarget","_wxDropTarget",0},
    { "_class_wxCaret","_wxCaret",0},
    { "_class_wxMDIClientWindow","_wxMDIClientWindow",0},
    { "_class_wxBrush","_wxBrush",0},
    { "_unsigned_short","_WXTYPE",0},
    { "_unsigned_short","_short",0},
    { "_class_wxWindow","_class_wxGLCanvas",SwigwxGLCanvasTowxWindow},
    { "_class_wxWindow","_wxGLCanvas",SwigwxGLCanvasTowxWindow},
    { "_class_wxWindow","_wxWindow",0},
    { "_wxSplitterWindow","_class_wxSplitterWindow",0},
    { "_class_wxStaticText","_wxStaticText",0},
    { "_wxPrintDialogData","_class_wxPrintDialogData",0},
    { "_class_wxFont","_wxFont",0},
    { "_wxClipboard","_class_wxClipboard",0},
    { "_class_wxPyValidator","_wxPyValidator",0},
    { "_class_wxCloseEvent","_wxCloseEvent",0},
    { "_wxSashEvent","_class_wxSashEvent",0},
    { "_wxBusyInfo","_class_wxBusyInfo",0},
    { "_class_wxMenuEvent","_wxMenuEvent",0},
    { "_wxPaletteChangedEvent","_class_wxPaletteChangedEvent",0},
    { "_class_wxPyBitmapDataObject","_wxPyBitmapDataObject",0},
    { "_wxClientDC","_class_wxClientDC",0},
    { "_wxMouseEvent","_class_wxMouseEvent",0},
    { "_wxListCtrl","_class_wxListCtrl",0},
    { "_wxSingleChoiceDialog","_class_wxSingleChoiceDialog",0},
    { "_wxGLCanvas","_class_wxGLCanvas",0},
    { "_class_wxPoint","_wxPoint",0},
    { "_wxRealPoint","_class_wxRealPoint",0},
    { "_class_wxRadioBox","_wxRadioBox",0},
    { "_class_wxBoxSizer","_wxBoxSizer",0},
    { "_signed_short","_WXTYPE",0},
    { "_signed_short","_short",0},
    { "_wxMemoryDC","_class_wxMemoryDC",0},
    { "_wxPyTextDataObject","_class_wxPyTextDataObject",0},
    { "_class_wxTaskBarIcon","_wxTaskBarIcon",0},
    { "_class_wxPrintDialog","_wxPrintDialog",0},
    { "_wxPaintDC","_class_wxPaintDC",0},
    { "_class_wxWindowDC","_wxWindowDC",0},
    { "_class_wxFocusEvent","_wxFocusEvent",0},
    { "_class_wxMaximizeEvent","_wxMaximizeEvent",0},
    { "_wxStatusBar","_class_wxStatusBar",0},
    { "_class_wxAcceleratorEntry","_wxAcceleratorEntry",0},
    { "_class_wxCursor","_wxCursor",0},
    { "_wxPyFileDropTarget","_class_wxPyFileDropTarget",0},
    { "_class_wxImageHandler","_wxImageHandler",0},
    { "_wxScrolledWindow","_class_wxGLCanvas",SwigwxGLCanvasTowxScrolledWindow},
    { "_wxScrolledWindow","_wxGLCanvas",SwigwxGLCanvasTowxScrolledWindow},
    { "_wxScrolledWindow","_class_wxScrolledWindow",0},
    { "_wxTreeItemId","_class_wxTreeItemId",0},
    { "_unsigned_char","_byte",0},
    { "_class_wxMetaFileDC","_wxMetaFileDC",0},
    { "_class_wxMenu","_wxMenu",0},
    { "_wxControl","_class_wxControl",0},
    { "_class_wxListBox","_wxListBox",0},
    { "_unsigned_int","_wxCoord",0},
    { "_unsigned_int","_wxPrintQuality",0},
    { "_unsigned_int","_size_t",0},
    { "_unsigned_int","_uint",0},
    { "_unsigned_int","_wxWindowID",0},
    { "_unsigned_int","_int",0},
    { "_wxIcon","_class_wxIcon",0},
    { "_wxDialog","_class_wxDialog",0},
    { "_class_wxListItem","_wxListItem",0},
    { "_class_wxPen","_wxPen",0},
    { "_class_wxFileDialog","_wxFileDialog",0},
    { "_wxQueryLayoutInfoEvent","_class_wxQueryLayoutInfoEvent",0},
    { "_short","_WXTYPE",0},
    { "_short","_unsigned_short",0},
    { "_short","_signed_short",0},
    { "_class_wxStaticBox","_wxStaticBox",0},
    { "_wxLayoutAlgorithm","_class_wxLayoutAlgorithm",0},
    { "_class_wxPyDataObjectSimple","_wxPyDataObjectSimple",0},
    { "_class_wxPyDropSource","_wxPyDropSource",0},
    { "_class_wxScrollEvent","_wxScrollEvent",0},
    { "_wxJoystickEvent","_class_wxJoystickEvent",0},
    { "_class_wxChoice","_wxChoice",0},
    { "_class_wxSlider","_wxSlider",0},
    { "_class_wxCalculateLayoutEvent","_wxCalculateLayoutEvent",0},
    { "_class_wxImageList","_wxImageList",0},
    { "_class_wxBitmapButton","_wxBitmapButton",0},
    { "_wxFrame","_class_wxFrame",0},
    { "_wxPCXHandler","_class_wxPCXHandler",0},
    { "_class_wxGLContext","_wxGLContext",0},
    { "_class_wxPaletteChangedEvent","_wxPaletteChangedEvent",0},
    { "_class_wxNotebook","_wxNotebook",0},
    { "_wxJPEGHandler","_class_wxJPEGHandler",0},
    { "_wxWindowID","_wxCoord",0},
    { "_wxWindowID","_wxPrintQuality",0},
    { "_wxWindowID","_size_t",0},
    { "_wxWindowID","_EBool",0},
    { "_wxWindowID","_uint",0},
    { "_wxWindowID","_int",0},
    { "_wxWindowID","_signed_int",0},
    { "_wxWindowID","_unsigned_int",0},
    { "_class_wxScrollWinEvent","_wxScrollWinEvent",0},
    { "_class_wxSizerItem","_wxSizerItem",0},
    { "_int","_wxCoord",0},
    { "_int","_wxPrintQuality",0},
    { "_int","_size_t",0},
    { "_int","_EBool",0},
    { "_int","_uint",0},
    { "_int","_wxWindowID",0},
    { "_int","_unsigned_int",0},
    { "_int","_signed_int",0},
    { "_class_wxMouseEvent","_wxMouseEvent",0},
    { "_wxPyCommandEvent","_class_wxPyCommandEvent",0},
    { "_class_wxListEvent","_wxListEvent",0},
    { "_class_wxPrintPreview","_wxPrintPreview",0},
    { "_class_wxSpinEvent","_wxSpinEvent",0},
    { "_class_wxQueryNewPaletteEvent","_wxQueryNewPaletteEvent",0},
    { "_class_wxNavigationKeyEvent","_wxNavigationKeyEvent",0},
    { "_wxButton","_class_wxButton",0},
    { "_class_wxPyApp","_wxPyApp",0},
    { "_wxSize","_class_wxSize",0},
    { "_wxRegionIterator","_class_wxRegionIterator",0},
    { "_class_wxPrinterDC","_wxPrinterDC",0},
    { "_class_wxPyTextDataObject","_wxPyTextDataObject",0},
    { "_class_wxMDIParentFrame","_wxMDIParentFrame",0},
    { "_wxPyTreeItemData","_class_wxPyTreeItemData",0},
    { "_wxStaticBoxSizer","_class_wxStaticBoxSizer",0},
    { "_class_wxPaintDC","_wxPaintDC",0},
    { "_class_wxSysColourChangedEvent","_wxSysColourChangedEvent",0},
    { "_class_wxPyFileDropTarget","_wxPyFileDropTarget",0},
    { "_class_wxInitDialogEvent","_wxInitDialogEvent",0},
    { "_class_wxComboBox","_wxComboBox",0},
    { "_class_wxRadioButton","_wxRadioButton",0},
    { "_wxValidator","_class_wxValidator",0},
    { "_wxToolBarBase","_class_wxToolBarBase",0},
    { "_class_wxTreeItemId","_wxTreeItemId",0},
    { "_wxTreeCtrl","_class_wxTreeCtrl",0},
    { "_class_wxLayoutConstraints","_wxLayoutConstraints",0},
    { "_wxIconizeEvent","_class_wxIconizeEvent",0},
    { "_class_wxControl","_wxControl",0},
    { "_wxStaticBitmap","_class_wxStaticBitmap",0},
    { "_class_wxIcon","_wxIcon",0},
    { "_class_wxColour","_wxColour",0},
    { "_class_wxScreenDC","_wxScreenDC",0},
    { "_class_wxPageSetupDialog","_wxPageSetupDialog",0},
    { "_wxPalette","_class_wxPalette",0},
    { "_class_wxIdleEvent","_wxIdleEvent",0},
    { "_wxCoord","_int",0},
    { "_wxCoord","_signed_int",0},
    { "_wxCoord","_unsigned_int",0},
    { "_wxCoord","_wxWindowID",0},
    { "_wxCoord","_uint",0},
    { "_wxCoord","_EBool",0},
    { "_wxCoord","_size_t",0},
    { "_wxCoord","_wxPrintQuality",0},
    { "_wxEraseEvent","_class_wxEraseEvent",0},
    { "_wxDataObjectComposite","_class_wxDataObjectComposite",0},
    { "_class_wxJoystickEvent","_wxJoystickEvent",0},
    { "_class_wxMiniFrame","_wxMiniFrame",0},
    { "_wxFontDialog","_class_wxFontDialog",0},
    { "_wxRegion","_class_wxRegion",0},
    { "_class_wxSplitterWindow","_wxSplitterWindow",0},
    { "_wxPreviewFrame","_class_wxPreviewFrame",0},
    { "_wxSizer","_class_wxSizer",0},
    { "_class_wxShowEvent","_wxShowEvent",0},
    { "_class_wxPCXHandler","_wxPCXHandler",0},
    { "_wxTIFFHandler","_class_wxTIFFHandler",0},
    { "_wxPyDropTarget","_class_wxPyDropTarget",0},
    { "_wxActivateEvent","_class_wxActivateEvent",0},
    { "_wxGauge","_class_wxGauge",0},
    { "_class_wxCheckListBox","_wxCheckListBox",0},
    { "_class_wxBusyInfo","_wxBusyInfo",0},
    { "_class_wxCommandEvent","_wxCommandEvent",0},
    { "_class_wxClientDC","_wxClientDC",0},
    { "_class_wxSizeEvent","_wxSizeEvent",0},
    { "_class_wxListCtrl","_wxListCtrl",0},
    { "_class_wxGLCanvas","_wxGLCanvas",0},
    { "_wxCustomDataObject","_class_wxCustomDataObject",0},
    { "_class_wxSize","_wxSize",0},
    { "_class_wxBitmap","_wxBitmap",0},
    { "_class_wxMemoryDC","_wxMemoryDC",0},
    { "_wxPyTextDropTarget","_class_wxPyTextDropTarget",0},
    { "_wxMenuBar","_class_wxMenuBar",0},
    { "_wxTreeEvent","_class_wxTreeEvent",0},
    { "_wxDirDialog","_class_wxDirDialog",0},
    { "_wxEvtHandler","_class_wxGLCanvas",SwigwxGLCanvasTowxEvtHandler},
    { "_wxEvtHandler","_wxGLCanvas",SwigwxGLCanvasTowxEvtHandler},
    { "_wxEvtHandler","_class_wxEvtHandler",0},
    { "_wxMenuItem","_class_wxMenuItem",0},
    { "_class_wxScrollBar","_wxScrollBar",0},
    { "_class_wxColourDialog","_wxColourDialog",0},
    { "_class_wxPrintData","_wxPrintData",0},
    { "_wxDash","_unsigned_long",0},
    { "_wxDash","_long",0},
    { "_class_wxScrolledWindow","_class_wxGLCanvas",SwigwxGLCanvasTowxScrolledWindow},
    { "_class_wxScrolledWindow","_wxGLCanvas",SwigwxGLCanvasTowxScrolledWindow},
    { "_class_wxScrolledWindow","_wxScrolledWindow",0},
    { "_class_wxTextEntryDialog","_wxTextEntryDialog",0},
    { "_wxKeyEvent","_class_wxKeyEvent",0},
    { "_wxMoveEvent","_class_wxMoveEvent",0},
    { "_wxColourData","_class_wxColourData",0},
    { "_wxPageSetupDialogData","_class_wxPageSetupDialogData",0},
    { "_class_wxPalette","_wxPalette",0},
    { "_wxFileDataObject","_class_wxFileDataObject",0},
    { "_class_wxQueryLayoutInfoEvent","_wxQueryLayoutInfoEvent",0},
    { "_class_wxEraseEvent","_wxEraseEvent",0},
    { "_wxPyFontEnumerator","_class_wxPyFontEnumerator",0},
    { "_wxMDIClientWindow","_class_wxMDIClientWindow",0},
    { "_class_wxDataObjectComposite","_wxDataObjectComposite",0},
    { "_class_wxToolBarToolBase","_wxToolBarToolBase",0},
    { "_class_wxFontDialog","_wxFontDialog",0},
    { "_wxWindow","_class_wxGLCanvas",SwigwxGLCanvasTowxWindow},
    { "_wxWindow","_wxGLCanvas",SwigwxGLCanvasTowxWindow},
    { "_wxWindow","_class_wxWindow",0},
    { "_class_wxWindowDestroyEvent","_wxWindowDestroyEvent",0},
    { "_class_wxFrame","_wxFrame",0},
{0,0,0}};

static PyObject *SWIG_globals;
#ifdef __cplusplus
extern "C" 
#endif
SWIGEXPORT(void) initglcanvasc() {
	 PyObject *m, *d;
	 SWIG_globals = SWIG_newvarlink();
	 m = Py_InitModule("glcanvasc", glcanvascMethods);
	 d = PyModule_GetDict(m);


    wxClassInfo::CleanUpClasses();
    wxClassInfo::InitializeClasses();

{
   int i;
   for (i = 0; _swig_mapping[i].n1; i++)
        SWIG_RegisterMapping(_swig_mapping[i].n1,_swig_mapping[i].n2,_swig_mapping[i].pcnv);
}
}
