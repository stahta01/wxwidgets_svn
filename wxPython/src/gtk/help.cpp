/*
 * FILE : src/gtk/help.cpp
 * 
 * This file was automatically generated by :
 * Simplified Wrapper and Interface Generator (SWIG)
 * Version 1.1 (Build 883)
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
#include "Python.h"

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
extern void SWIG_MakePtr(char *, void *, char *);
extern void SWIG_RegisterMapping(char *, char *, void *(*)(void *));
extern char *SWIG_GetPtr(char *, void **, char *);
extern char *SWIG_GetPtrObj(PyObject *, void **, char *);
extern void SWIG_addvarlink(PyObject *, char *, PyObject *(*)(void), int (*)(PyObject *));
extern PyObject *SWIG_newvarlink(void);
#ifdef __cplusplus
}
#endif
#define SWIG_init    inithelpc

#define SWIG_name    "helpc"

#include "wxPython.h"
#include <wx/cshelp.h>


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
#ifdef __cplusplus
extern "C" {
#endif
static void *SwigwxHelpEventTowxCommandEvent(void *ptr) {
    wxHelpEvent *src;
    wxCommandEvent *dest;
    src = (wxHelpEvent *) ptr;
    dest = (wxCommandEvent *) src;
    return (void *) dest;
}

static void *SwigwxHelpEventTowxEvent(void *ptr) {
    wxHelpEvent *src;
    wxEvent *dest;
    src = (wxHelpEvent *) ptr;
    dest = (wxEvent *) src;
    return (void *) dest;
}

static void *SwigwxHelpEventTowxObject(void *ptr) {
    wxHelpEvent *src;
    wxObject *dest;
    src = (wxHelpEvent *) ptr;
    dest = (wxObject *) src;
    return (void *) dest;
}

#define new_wxHelpEvent(_swigarg0,_swigarg1,_swigarg2) (new wxHelpEvent(_swigarg0,_swigarg1,_swigarg2))
static PyObject *_wrap_new_wxHelpEvent(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxHelpEvent * _result;
    wxEventType  _arg0 = (wxEventType ) wxEVT_NULL;
    wxWindowID  _arg1 = (wxWindowID ) 0;
    wxPoint * _arg2 = (wxPoint *) &wxDefaultPosition;
    wxPoint  temp;
    PyObject * _obj2 = 0;
    char *_kwnames[] = { "type","id","pt", NULL };
    char _ptemp[128];

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"|iiO:new_wxHelpEvent",_kwnames,&_arg0,&_arg1,&_obj2)) 
        return NULL;
    if (_obj2)
{
    _arg2 = &temp;
    if (! wxPoint_helper(_obj2, &_arg2))
        return NULL;
}
{
    PyThreadState* __tstate = wxPyBeginAllowThreads();
    _result = (wxHelpEvent *)new_wxHelpEvent(_arg0,_arg1,*_arg2);

    wxPyEndAllowThreads(__tstate);
    if (PyErr_Occurred()) return NULL;
}    if (_result) {
        SWIG_MakePtr(_ptemp, (char *) _result,"_wxHelpEvent_p");
        _resultobj = Py_BuildValue("s",_ptemp);
    } else {
        Py_INCREF(Py_None);
        _resultobj = Py_None;
    }
    return _resultobj;
}

#define wxHelpEvent_GetPosition(_swigobj)  (_swigobj->GetPosition())
static PyObject *_wrap_wxHelpEvent_GetPosition(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxPoint * _result;
    wxHelpEvent * _arg0;
    PyObject * _argo0 = 0;
    char *_kwnames[] = { "self", NULL };
    char _ptemp[128];

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"O:wxHelpEvent_GetPosition",_kwnames,&_argo0)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxHelpEvent_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxHelpEvent_GetPosition. Expected _wxHelpEvent_p.");
        return NULL;
        }
    }
{
    PyThreadState* __tstate = wxPyBeginAllowThreads();
    const wxPoint & _result_ref = wxHelpEvent_GetPosition(_arg0);
    _result = (wxPoint *) &_result_ref;

    wxPyEndAllowThreads(__tstate);
    if (PyErr_Occurred()) return NULL;
}    if (_result) {
        SWIG_MakePtr(_ptemp, (char *) _result,"_wxPoint_p");
        _resultobj = Py_BuildValue("s",_ptemp);
    } else {
        Py_INCREF(Py_None);
        _resultobj = Py_None;
    }
    return _resultobj;
}

#define wxHelpEvent_SetPosition(_swigobj,_swigarg0)  (_swigobj->SetPosition(_swigarg0))
static PyObject *_wrap_wxHelpEvent_SetPosition(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxHelpEvent * _arg0;
    wxPoint * _arg1;
    PyObject * _argo0 = 0;
    wxPoint  temp;
    PyObject * _obj1 = 0;
    char *_kwnames[] = { "self","pos", NULL };

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"OO:wxHelpEvent_SetPosition",_kwnames,&_argo0,&_obj1)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxHelpEvent_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxHelpEvent_SetPosition. Expected _wxHelpEvent_p.");
        return NULL;
        }
    }
{
    _arg1 = &temp;
    if (! wxPoint_helper(_obj1, &_arg1))
        return NULL;
}
{
    PyThreadState* __tstate = wxPyBeginAllowThreads();
    wxHelpEvent_SetPosition(_arg0,*_arg1);

    wxPyEndAllowThreads(__tstate);
    if (PyErr_Occurred()) return NULL;
}    Py_INCREF(Py_None);
    _resultobj = Py_None;
    return _resultobj;
}

#define wxHelpEvent_GetLink(_swigobj)  (_swigobj->GetLink())
static PyObject *_wrap_wxHelpEvent_GetLink(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxString * _result;
    wxHelpEvent * _arg0;
    PyObject * _argo0 = 0;
    char *_kwnames[] = { "self", NULL };

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"O:wxHelpEvent_GetLink",_kwnames,&_argo0)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxHelpEvent_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxHelpEvent_GetLink. Expected _wxHelpEvent_p.");
        return NULL;
        }
    }
{
    PyThreadState* __tstate = wxPyBeginAllowThreads();
    const wxString & _result_ref = wxHelpEvent_GetLink(_arg0);
    _result = (wxString *) &_result_ref;

    wxPyEndAllowThreads(__tstate);
    if (PyErr_Occurred()) return NULL;
}{
#if wxUSE_UNICODE
    _resultobj = PyUnicode_FromWideChar(_result->c_str(), _result->Len());
#else
    _resultobj = PyString_FromStringAndSize(_result->c_str(), _result->Len());
#endif
}
    return _resultobj;
}

#define wxHelpEvent_SetLink(_swigobj,_swigarg0)  (_swigobj->SetLink(_swigarg0))
static PyObject *_wrap_wxHelpEvent_SetLink(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxHelpEvent * _arg0;
    wxString * _arg1;
    PyObject * _argo0 = 0;
    PyObject * _obj1 = 0;
    char *_kwnames[] = { "self","link", NULL };

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"OO:wxHelpEvent_SetLink",_kwnames,&_argo0,&_obj1)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxHelpEvent_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxHelpEvent_SetLink. Expected _wxHelpEvent_p.");
        return NULL;
        }
    }
{
    _arg1 = wxString_in_helper(_obj1);
    if (_arg1 == NULL)
        return NULL;
}
{
    PyThreadState* __tstate = wxPyBeginAllowThreads();
    wxHelpEvent_SetLink(_arg0,*_arg1);

    wxPyEndAllowThreads(__tstate);
    if (PyErr_Occurred()) return NULL;
}    Py_INCREF(Py_None);
    _resultobj = Py_None;
{
    if (_obj1)
        delete _arg1;
}
    return _resultobj;
}

#define wxHelpEvent_GetTarget(_swigobj)  (_swigobj->GetTarget())
static PyObject *_wrap_wxHelpEvent_GetTarget(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxString * _result;
    wxHelpEvent * _arg0;
    PyObject * _argo0 = 0;
    char *_kwnames[] = { "self", NULL };

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"O:wxHelpEvent_GetTarget",_kwnames,&_argo0)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxHelpEvent_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxHelpEvent_GetTarget. Expected _wxHelpEvent_p.");
        return NULL;
        }
    }
{
    PyThreadState* __tstate = wxPyBeginAllowThreads();
    const wxString & _result_ref = wxHelpEvent_GetTarget(_arg0);
    _result = (wxString *) &_result_ref;

    wxPyEndAllowThreads(__tstate);
    if (PyErr_Occurred()) return NULL;
}{
#if wxUSE_UNICODE
    _resultobj = PyUnicode_FromWideChar(_result->c_str(), _result->Len());
#else
    _resultobj = PyString_FromStringAndSize(_result->c_str(), _result->Len());
#endif
}
    return _resultobj;
}

#define wxHelpEvent_SetTarget(_swigobj,_swigarg0)  (_swigobj->SetTarget(_swigarg0))
static PyObject *_wrap_wxHelpEvent_SetTarget(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxHelpEvent * _arg0;
    wxString * _arg1;
    PyObject * _argo0 = 0;
    PyObject * _obj1 = 0;
    char *_kwnames[] = { "self","target", NULL };

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"OO:wxHelpEvent_SetTarget",_kwnames,&_argo0,&_obj1)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxHelpEvent_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxHelpEvent_SetTarget. Expected _wxHelpEvent_p.");
        return NULL;
        }
    }
{
    _arg1 = wxString_in_helper(_obj1);
    if (_arg1 == NULL)
        return NULL;
}
{
    PyThreadState* __tstate = wxPyBeginAllowThreads();
    wxHelpEvent_SetTarget(_arg0,*_arg1);

    wxPyEndAllowThreads(__tstate);
    if (PyErr_Occurred()) return NULL;
}    Py_INCREF(Py_None);
    _resultobj = Py_None;
{
    if (_obj1)
        delete _arg1;
}
    return _resultobj;
}

static void *SwigwxContextHelpTowxObject(void *ptr) {
    wxContextHelp *src;
    wxObject *dest;
    src = (wxContextHelp *) ptr;
    dest = (wxObject *) src;
    return (void *) dest;
}

#define new_wxContextHelp(_swigarg0,_swigarg1) (new wxContextHelp(_swigarg0,_swigarg1))
static PyObject *_wrap_new_wxContextHelp(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxContextHelp * _result;
    wxWindow * _arg0 = (wxWindow *) NULL;
    bool  _arg1 = (bool ) TRUE;
    PyObject * _argo0 = 0;
    int tempbool1 = (int) TRUE;
    char *_kwnames[] = { "window","doNow", NULL };
    char _ptemp[128];

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"|Oi:new_wxContextHelp",_kwnames,&_argo0,&tempbool1)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxWindow_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of new_wxContextHelp. Expected _wxWindow_p.");
        return NULL;
        }
    }
    _arg1 = (bool ) tempbool1;
{
    PyThreadState* __tstate = wxPyBeginAllowThreads();
    _result = (wxContextHelp *)new_wxContextHelp(_arg0,_arg1);

    wxPyEndAllowThreads(__tstate);
    if (PyErr_Occurred()) return NULL;
}    if (_result) {
        SWIG_MakePtr(_ptemp, (char *) _result,"_wxContextHelp_p");
        _resultobj = Py_BuildValue("s",_ptemp);
    } else {
        Py_INCREF(Py_None);
        _resultobj = Py_None;
    }
    return _resultobj;
}

#define delete_wxContextHelp(_swigobj) (delete _swigobj)
static PyObject *_wrap_delete_wxContextHelp(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxContextHelp * _arg0;
    PyObject * _argo0 = 0;
    char *_kwnames[] = { "self", NULL };

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"O:delete_wxContextHelp",_kwnames,&_argo0)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxContextHelp_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of delete_wxContextHelp. Expected _wxContextHelp_p.");
        return NULL;
        }
    }
{
    PyThreadState* __tstate = wxPyBeginAllowThreads();
    delete_wxContextHelp(_arg0);

    wxPyEndAllowThreads(__tstate);
    if (PyErr_Occurred()) return NULL;
}    Py_INCREF(Py_None);
    _resultobj = Py_None;
    return _resultobj;
}

#define wxContextHelp_BeginContextHelp(_swigobj,_swigarg0)  (_swigobj->BeginContextHelp(_swigarg0))
static PyObject *_wrap_wxContextHelp_BeginContextHelp(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    bool  _result;
    wxContextHelp * _arg0;
    wxWindow * _arg1 = (wxWindow *) NULL;
    PyObject * _argo0 = 0;
    PyObject * _argo1 = 0;
    char *_kwnames[] = { "self","window", NULL };

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"O|O:wxContextHelp_BeginContextHelp",_kwnames,&_argo0,&_argo1)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxContextHelp_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxContextHelp_BeginContextHelp. Expected _wxContextHelp_p.");
        return NULL;
        }
    }
    if (_argo1) {
        if (_argo1 == Py_None) { _arg1 = NULL; }
        else if (SWIG_GetPtrObj(_argo1,(void **) &_arg1,"_wxWindow_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 2 of wxContextHelp_BeginContextHelp. Expected _wxWindow_p.");
        return NULL;
        }
    }
{
    PyThreadState* __tstate = wxPyBeginAllowThreads();
    _result = (bool )wxContextHelp_BeginContextHelp(_arg0,_arg1);

    wxPyEndAllowThreads(__tstate);
    if (PyErr_Occurred()) return NULL;
}    _resultobj = Py_BuildValue("i",_result);
    return _resultobj;
}

#define wxContextHelp_EndContextHelp(_swigobj)  (_swigobj->EndContextHelp())
static PyObject *_wrap_wxContextHelp_EndContextHelp(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    bool  _result;
    wxContextHelp * _arg0;
    PyObject * _argo0 = 0;
    char *_kwnames[] = { "self", NULL };

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"O:wxContextHelp_EndContextHelp",_kwnames,&_argo0)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxContextHelp_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxContextHelp_EndContextHelp. Expected _wxContextHelp_p.");
        return NULL;
        }
    }
{
    PyThreadState* __tstate = wxPyBeginAllowThreads();
    _result = (bool )wxContextHelp_EndContextHelp(_arg0);

    wxPyEndAllowThreads(__tstate);
    if (PyErr_Occurred()) return NULL;
}    _resultobj = Py_BuildValue("i",_result);
    return _resultobj;
}

static void *SwigwxContextHelpButtonTowxBitmapButton(void *ptr) {
    wxContextHelpButton *src;
    wxBitmapButton *dest;
    src = (wxContextHelpButton *) ptr;
    dest = (wxBitmapButton *) src;
    return (void *) dest;
}

static void *SwigwxContextHelpButtonTowxButton(void *ptr) {
    wxContextHelpButton *src;
    wxButton *dest;
    src = (wxContextHelpButton *) ptr;
    dest = (wxButton *) src;
    return (void *) dest;
}

static void *SwigwxContextHelpButtonTowxControl(void *ptr) {
    wxContextHelpButton *src;
    wxControl *dest;
    src = (wxContextHelpButton *) ptr;
    dest = (wxControl *) src;
    return (void *) dest;
}

static void *SwigwxContextHelpButtonTowxWindow(void *ptr) {
    wxContextHelpButton *src;
    wxWindow *dest;
    src = (wxContextHelpButton *) ptr;
    dest = (wxWindow *) src;
    return (void *) dest;
}

static void *SwigwxContextHelpButtonTowxEvtHandler(void *ptr) {
    wxContextHelpButton *src;
    wxEvtHandler *dest;
    src = (wxContextHelpButton *) ptr;
    dest = (wxEvtHandler *) src;
    return (void *) dest;
}

static void *SwigwxContextHelpButtonTowxObject(void *ptr) {
    wxContextHelpButton *src;
    wxObject *dest;
    src = (wxContextHelpButton *) ptr;
    dest = (wxObject *) src;
    return (void *) dest;
}

#define new_wxContextHelpButton(_swigarg0,_swigarg1,_swigarg2,_swigarg3,_swigarg4) (new wxContextHelpButton(_swigarg0,_swigarg1,_swigarg2,_swigarg3,_swigarg4))
static PyObject *_wrap_new_wxContextHelpButton(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxContextHelpButton * _result;
    wxWindow * _arg0;
    wxWindowID  _arg1 = (wxWindowID ) (wxID_CONTEXT_HELP);
    wxPoint * _arg2 = (wxPoint *) &wxDefaultPosition;
    wxSize * _arg3 = (wxSize *) &wxDefaultSize;
    long  _arg4 = (long ) wxBU_AUTODRAW;
    PyObject * _argo0 = 0;
    wxPoint  temp;
    PyObject * _obj2 = 0;
    wxSize  temp0;
    PyObject * _obj3 = 0;
    char *_kwnames[] = { "parent","id","pos","size","style", NULL };
    char _ptemp[128];

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"O|iOOl:new_wxContextHelpButton",_kwnames,&_argo0,&_arg1,&_obj2,&_obj3,&_arg4)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxWindow_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of new_wxContextHelpButton. Expected _wxWindow_p.");
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
{
    PyThreadState* __tstate = wxPyBeginAllowThreads();
    _result = (wxContextHelpButton *)new_wxContextHelpButton(_arg0,_arg1,*_arg2,*_arg3,_arg4);

    wxPyEndAllowThreads(__tstate);
    if (PyErr_Occurred()) return NULL;
}    if (_result) {
        SWIG_MakePtr(_ptemp, (char *) _result,"_wxContextHelpButton_p");
        _resultobj = Py_BuildValue("s",_ptemp);
    } else {
        Py_INCREF(Py_None);
        _resultobj = Py_None;
    }
    return _resultobj;
}

static PyObject *_wrap_wxHelpProvider_Set(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxHelpProvider * _result;
    wxHelpProvider * _arg0;
    PyObject * _argo0 = 0;
    char *_kwnames[] = { "helpProvider", NULL };
    char _ptemp[128];

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"O:wxHelpProvider_Set",_kwnames,&_argo0)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxHelpProvider_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxHelpProvider_Set. Expected _wxHelpProvider_p.");
        return NULL;
        }
    }
{
    PyThreadState* __tstate = wxPyBeginAllowThreads();
    _result = (wxHelpProvider *)wxHelpProvider::Set(_arg0);

    wxPyEndAllowThreads(__tstate);
    if (PyErr_Occurred()) return NULL;
}    if (_result) {
        SWIG_MakePtr(_ptemp, (char *) _result,"_wxHelpProvider_p");
        _resultobj = Py_BuildValue("s",_ptemp);
    } else {
        Py_INCREF(Py_None);
        _resultobj = Py_None;
    }
    return _resultobj;
}

static PyObject *_wrap_wxHelpProvider_Get(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxHelpProvider * _result;
    char *_kwnames[] = {  NULL };
    char _ptemp[128];

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,":wxHelpProvider_Get",_kwnames)) 
        return NULL;
{
    PyThreadState* __tstate = wxPyBeginAllowThreads();
    _result = (wxHelpProvider *)wxHelpProvider::Get();

    wxPyEndAllowThreads(__tstate);
    if (PyErr_Occurred()) return NULL;
}    if (_result) {
        SWIG_MakePtr(_ptemp, (char *) _result,"_wxHelpProvider_p");
        _resultobj = Py_BuildValue("s",_ptemp);
    } else {
        Py_INCREF(Py_None);
        _resultobj = Py_None;
    }
    return _resultobj;
}

#define wxHelpProvider_GetHelp(_swigobj,_swigarg0)  (_swigobj->GetHelp(_swigarg0))
static PyObject *_wrap_wxHelpProvider_GetHelp(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxString * _result;
    wxHelpProvider * _arg0;
    wxWindow * _arg1;
    PyObject * _argo0 = 0;
    PyObject * _argo1 = 0;
    char *_kwnames[] = { "self","window", NULL };

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"OO:wxHelpProvider_GetHelp",_kwnames,&_argo0,&_argo1)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxHelpProvider_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxHelpProvider_GetHelp. Expected _wxHelpProvider_p.");
        return NULL;
        }
    }
    if (_argo1) {
        if (_argo1 == Py_None) { _arg1 = NULL; }
        else if (SWIG_GetPtrObj(_argo1,(void **) &_arg1,"_wxWindow_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 2 of wxHelpProvider_GetHelp. Expected _wxWindow_p.");
        return NULL;
        }
    }
{
    PyThreadState* __tstate = wxPyBeginAllowThreads();
    _result = new wxString (wxHelpProvider_GetHelp(_arg0,_arg1));

    wxPyEndAllowThreads(__tstate);
    if (PyErr_Occurred()) return NULL;
}{
#if wxUSE_UNICODE
    _resultobj = PyUnicode_FromWideChar(_result->c_str(), _result->Len());
#else
    _resultobj = PyString_FromStringAndSize(_result->c_str(), _result->Len());
#endif
}
{
    delete _result;
}
    return _resultobj;
}

#define wxHelpProvider_ShowHelp(_swigobj,_swigarg0)  (_swigobj->ShowHelp(_swigarg0))
static PyObject *_wrap_wxHelpProvider_ShowHelp(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    bool  _result;
    wxHelpProvider * _arg0;
    wxWindowBase * _arg1;
    PyObject * _argo0 = 0;
    PyObject * _argo1 = 0;
    char *_kwnames[] = { "self","window", NULL };

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"OO:wxHelpProvider_ShowHelp",_kwnames,&_argo0,&_argo1)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxHelpProvider_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxHelpProvider_ShowHelp. Expected _wxHelpProvider_p.");
        return NULL;
        }
    }
    if (_argo1) {
        if (_argo1 == Py_None) { _arg1 = NULL; }
        else if (SWIG_GetPtrObj(_argo1,(void **) &_arg1,"_wxWindowBase_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 2 of wxHelpProvider_ShowHelp. Expected _wxWindowBase_p.");
        return NULL;
        }
    }
{
    PyThreadState* __tstate = wxPyBeginAllowThreads();
    _result = (bool )wxHelpProvider_ShowHelp(_arg0,_arg1);

    wxPyEndAllowThreads(__tstate);
    if (PyErr_Occurred()) return NULL;
}    _resultobj = Py_BuildValue("i",_result);
    return _resultobj;
}

#define wxHelpProvider_AddHelp(_swigobj,_swigarg0,_swigarg1)  (_swigobj->AddHelp(_swigarg0,_swigarg1))
static PyObject *_wrap_wxHelpProvider_AddHelp(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxHelpProvider * _arg0;
    wxWindowBase * _arg1;
    wxString * _arg2;
    PyObject * _argo0 = 0;
    PyObject * _argo1 = 0;
    PyObject * _obj2 = 0;
    char *_kwnames[] = { "self","window","text", NULL };

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"OOO:wxHelpProvider_AddHelp",_kwnames,&_argo0,&_argo1,&_obj2)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxHelpProvider_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxHelpProvider_AddHelp. Expected _wxHelpProvider_p.");
        return NULL;
        }
    }
    if (_argo1) {
        if (_argo1 == Py_None) { _arg1 = NULL; }
        else if (SWIG_GetPtrObj(_argo1,(void **) &_arg1,"_wxWindowBase_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 2 of wxHelpProvider_AddHelp. Expected _wxWindowBase_p.");
        return NULL;
        }
    }
{
    _arg2 = wxString_in_helper(_obj2);
    if (_arg2 == NULL)
        return NULL;
}
{
    PyThreadState* __tstate = wxPyBeginAllowThreads();
    wxHelpProvider_AddHelp(_arg0,_arg1,*_arg2);

    wxPyEndAllowThreads(__tstate);
    if (PyErr_Occurred()) return NULL;
}    Py_INCREF(Py_None);
    _resultobj = Py_None;
{
    if (_obj2)
        delete _arg2;
}
    return _resultobj;
}

#define wxHelpProvider_AddHelpById(_swigobj,_swigarg0,_swigarg1)  (_swigobj->AddHelp(_swigarg0,_swigarg1))
static PyObject *_wrap_wxHelpProvider_AddHelpById(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxHelpProvider * _arg0;
    wxWindowID  _arg1;
    wxString * _arg2;
    PyObject * _argo0 = 0;
    PyObject * _obj2 = 0;
    char *_kwnames[] = { "self","id","text", NULL };

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"OiO:wxHelpProvider_AddHelpById",_kwnames,&_argo0,&_arg1,&_obj2)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxHelpProvider_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxHelpProvider_AddHelpById. Expected _wxHelpProvider_p.");
        return NULL;
        }
    }
{
    _arg2 = wxString_in_helper(_obj2);
    if (_arg2 == NULL)
        return NULL;
}
{
    PyThreadState* __tstate = wxPyBeginAllowThreads();
    wxHelpProvider_AddHelpById(_arg0,_arg1,*_arg2);

    wxPyEndAllowThreads(__tstate);
    if (PyErr_Occurred()) return NULL;
}    Py_INCREF(Py_None);
    _resultobj = Py_None;
{
    if (_obj2)
        delete _arg2;
}
    return _resultobj;
}

static void  wxHelpProvider_Destroy(wxHelpProvider *self) { delete self; }
static PyObject *_wrap_wxHelpProvider_Destroy(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxHelpProvider * _arg0;
    PyObject * _argo0 = 0;
    char *_kwnames[] = { "self", NULL };

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"O:wxHelpProvider_Destroy",_kwnames,&_argo0)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxHelpProvider_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxHelpProvider_Destroy. Expected _wxHelpProvider_p.");
        return NULL;
        }
    }
{
    PyThreadState* __tstate = wxPyBeginAllowThreads();
    wxHelpProvider_Destroy(_arg0);

    wxPyEndAllowThreads(__tstate);
    if (PyErr_Occurred()) return NULL;
}    Py_INCREF(Py_None);
    _resultobj = Py_None;
    return _resultobj;
}

static void *SwigwxSimpleHelpProviderTowxHelpProvider(void *ptr) {
    wxSimpleHelpProvider *src;
    wxHelpProvider *dest;
    src = (wxSimpleHelpProvider *) ptr;
    dest = (wxHelpProvider *) src;
    return (void *) dest;
}

#define new_wxSimpleHelpProvider() (new wxSimpleHelpProvider())
static PyObject *_wrap_new_wxSimpleHelpProvider(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxSimpleHelpProvider * _result;
    char *_kwnames[] = {  NULL };
    char _ptemp[128];

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,":new_wxSimpleHelpProvider",_kwnames)) 
        return NULL;
{
    PyThreadState* __tstate = wxPyBeginAllowThreads();
    _result = (wxSimpleHelpProvider *)new_wxSimpleHelpProvider();

    wxPyEndAllowThreads(__tstate);
    if (PyErr_Occurred()) return NULL;
}    if (_result) {
        SWIG_MakePtr(_ptemp, (char *) _result,"_wxSimpleHelpProvider_p");
        _resultobj = Py_BuildValue("s",_ptemp);
    } else {
        Py_INCREF(Py_None);
        _resultobj = Py_None;
    }
    return _resultobj;
}

static PyMethodDef helpcMethods[] = {
	 { "new_wxSimpleHelpProvider", (PyCFunction) _wrap_new_wxSimpleHelpProvider, METH_VARARGS | METH_KEYWORDS },
	 { "wxHelpProvider_Destroy", (PyCFunction) _wrap_wxHelpProvider_Destroy, METH_VARARGS | METH_KEYWORDS },
	 { "wxHelpProvider_AddHelpById", (PyCFunction) _wrap_wxHelpProvider_AddHelpById, METH_VARARGS | METH_KEYWORDS },
	 { "wxHelpProvider_AddHelp", (PyCFunction) _wrap_wxHelpProvider_AddHelp, METH_VARARGS | METH_KEYWORDS },
	 { "wxHelpProvider_ShowHelp", (PyCFunction) _wrap_wxHelpProvider_ShowHelp, METH_VARARGS | METH_KEYWORDS },
	 { "wxHelpProvider_GetHelp", (PyCFunction) _wrap_wxHelpProvider_GetHelp, METH_VARARGS | METH_KEYWORDS },
	 { "wxHelpProvider_Get", (PyCFunction) _wrap_wxHelpProvider_Get, METH_VARARGS | METH_KEYWORDS },
	 { "wxHelpProvider_Set", (PyCFunction) _wrap_wxHelpProvider_Set, METH_VARARGS | METH_KEYWORDS },
	 { "new_wxContextHelpButton", (PyCFunction) _wrap_new_wxContextHelpButton, METH_VARARGS | METH_KEYWORDS },
	 { "wxContextHelp_EndContextHelp", (PyCFunction) _wrap_wxContextHelp_EndContextHelp, METH_VARARGS | METH_KEYWORDS },
	 { "wxContextHelp_BeginContextHelp", (PyCFunction) _wrap_wxContextHelp_BeginContextHelp, METH_VARARGS | METH_KEYWORDS },
	 { "delete_wxContextHelp", (PyCFunction) _wrap_delete_wxContextHelp, METH_VARARGS | METH_KEYWORDS },
	 { "new_wxContextHelp", (PyCFunction) _wrap_new_wxContextHelp, METH_VARARGS | METH_KEYWORDS },
	 { "wxHelpEvent_SetTarget", (PyCFunction) _wrap_wxHelpEvent_SetTarget, METH_VARARGS | METH_KEYWORDS },
	 { "wxHelpEvent_GetTarget", (PyCFunction) _wrap_wxHelpEvent_GetTarget, METH_VARARGS | METH_KEYWORDS },
	 { "wxHelpEvent_SetLink", (PyCFunction) _wrap_wxHelpEvent_SetLink, METH_VARARGS | METH_KEYWORDS },
	 { "wxHelpEvent_GetLink", (PyCFunction) _wrap_wxHelpEvent_GetLink, METH_VARARGS | METH_KEYWORDS },
	 { "wxHelpEvent_SetPosition", (PyCFunction) _wrap_wxHelpEvent_SetPosition, METH_VARARGS | METH_KEYWORDS },
	 { "wxHelpEvent_GetPosition", (PyCFunction) _wrap_wxHelpEvent_GetPosition, METH_VARARGS | METH_KEYWORDS },
	 { "new_wxHelpEvent", (PyCFunction) _wrap_new_wxHelpEvent, METH_VARARGS | METH_KEYWORDS },
	 { NULL, NULL }
};
#ifdef __cplusplus
}
#endif
/*
 * This table is used by the pointer type-checker
 */
static struct { char *n1; char *n2; void *(*pcnv)(void *); } _swig_mapping[] = {
    { "_wxEvent","_wxHelpEvent",SwigwxHelpEventTowxEvent},
    { "_signed_long","_long",0},
    { "_wxPrintQuality","_wxCoord",0},
    { "_wxPrintQuality","_int",0},
    { "_wxPrintQuality","_signed_int",0},
    { "_wxPrintQuality","_unsigned_int",0},
    { "_wxPrintQuality","_wxWindowID",0},
    { "_wxPrintQuality","_uint",0},
    { "_wxPrintQuality","_EBool",0},
    { "_wxPrintQuality","_size_t",0},
    { "_wxPrintQuality","_time_t",0},
    { "_byte","_unsigned_char",0},
    { "_long","_unsigned_long",0},
    { "_long","_signed_long",0},
    { "_wxBitmapButton","_wxContextHelpButton",SwigwxContextHelpButtonTowxBitmapButton},
    { "_size_t","_wxCoord",0},
    { "_size_t","_wxPrintQuality",0},
    { "_size_t","_time_t",0},
    { "_size_t","_unsigned_int",0},
    { "_size_t","_int",0},
    { "_size_t","_wxWindowID",0},
    { "_size_t","_uint",0},
    { "_uint","_wxCoord",0},
    { "_uint","_wxPrintQuality",0},
    { "_uint","_time_t",0},
    { "_uint","_size_t",0},
    { "_uint","_unsigned_int",0},
    { "_uint","_int",0},
    { "_uint","_wxWindowID",0},
    { "_wxChar","_char",0},
    { "_wxCommandEvent","_wxHelpEvent",SwigwxHelpEventTowxCommandEvent},
    { "_char","_wxChar",0},
    { "_wxHelpProvider","_wxSimpleHelpProvider",SwigwxSimpleHelpProviderTowxHelpProvider},
    { "_struct_wxNativeFontInfo","_wxNativeFontInfo",0},
    { "_EBool","_wxCoord",0},
    { "_EBool","_wxPrintQuality",0},
    { "_EBool","_signed_int",0},
    { "_EBool","_int",0},
    { "_EBool","_wxWindowID",0},
    { "_unsigned_long","_long",0},
    { "_wxNativeFontInfo","_struct_wxNativeFontInfo",0},
    { "_signed_int","_wxCoord",0},
    { "_signed_int","_wxPrintQuality",0},
    { "_signed_int","_EBool",0},
    { "_signed_int","_wxWindowID",0},
    { "_signed_int","_int",0},
    { "_WXTYPE","_short",0},
    { "_WXTYPE","_signed_short",0},
    { "_WXTYPE","_unsigned_short",0},
    { "_unsigned_short","_WXTYPE",0},
    { "_unsigned_short","_short",0},
    { "_wxObject","_wxContextHelpButton",SwigwxContextHelpButtonTowxObject},
    { "_wxObject","_wxContextHelp",SwigwxContextHelpTowxObject},
    { "_wxObject","_wxHelpEvent",SwigwxHelpEventTowxObject},
    { "_signed_short","_WXTYPE",0},
    { "_signed_short","_short",0},
    { "_unsigned_char","_byte",0},
    { "_wxControl","_wxContextHelpButton",SwigwxContextHelpButtonTowxControl},
    { "_unsigned_int","_wxCoord",0},
    { "_unsigned_int","_wxPrintQuality",0},
    { "_unsigned_int","_time_t",0},
    { "_unsigned_int","_size_t",0},
    { "_unsigned_int","_uint",0},
    { "_unsigned_int","_wxWindowID",0},
    { "_unsigned_int","_int",0},
    { "_short","_WXTYPE",0},
    { "_short","_unsigned_short",0},
    { "_short","_signed_short",0},
    { "_wxWindowID","_wxCoord",0},
    { "_wxWindowID","_wxPrintQuality",0},
    { "_wxWindowID","_time_t",0},
    { "_wxWindowID","_size_t",0},
    { "_wxWindowID","_EBool",0},
    { "_wxWindowID","_uint",0},
    { "_wxWindowID","_int",0},
    { "_wxWindowID","_signed_int",0},
    { "_wxWindowID","_unsigned_int",0},
    { "_int","_wxCoord",0},
    { "_int","_wxPrintQuality",0},
    { "_int","_time_t",0},
    { "_int","_size_t",0},
    { "_int","_EBool",0},
    { "_int","_uint",0},
    { "_int","_wxWindowID",0},
    { "_int","_unsigned_int",0},
    { "_int","_signed_int",0},
    { "_time_t","_wxCoord",0},
    { "_time_t","_wxPrintQuality",0},
    { "_time_t","_unsigned_int",0},
    { "_time_t","_int",0},
    { "_time_t","_wxWindowID",0},
    { "_time_t","_uint",0},
    { "_time_t","_size_t",0},
    { "_wxButton","_wxContextHelpButton",SwigwxContextHelpButtonTowxButton},
    { "_wxCoord","_int",0},
    { "_wxCoord","_signed_int",0},
    { "_wxCoord","_unsigned_int",0},
    { "_wxCoord","_wxWindowID",0},
    { "_wxCoord","_uint",0},
    { "_wxCoord","_EBool",0},
    { "_wxCoord","_size_t",0},
    { "_wxCoord","_time_t",0},
    { "_wxCoord","_wxPrintQuality",0},
    { "_wxEvtHandler","_wxContextHelpButton",SwigwxContextHelpButtonTowxEvtHandler},
    { "_wxWindow","_wxContextHelpButton",SwigwxContextHelpButtonTowxWindow},
{0,0,0}};

static PyObject *SWIG_globals;
#ifdef __cplusplus
extern "C" 
#endif
SWIGEXPORT(void) inithelpc() {
	 PyObject *m, *d;
	 SWIG_globals = SWIG_newvarlink();
	 m = Py_InitModule("helpc", helpcMethods);
	 d = PyModule_GetDict(m);
	 PyDict_SetItemString(d,"wxFRAME_EX_CONTEXTHELP", PyInt_FromLong((long) wxFRAME_EX_CONTEXTHELP));
	 PyDict_SetItemString(d,"wxDIALOG_EX_CONTEXTHELP", PyInt_FromLong((long) wxDIALOG_EX_CONTEXTHELP));
	 PyDict_SetItemString(d,"wxID_CONTEXT_HELP", PyInt_FromLong((long) wxID_CONTEXT_HELP));
	 PyDict_SetItemString(d,"wxEVT_HELP", PyInt_FromLong((long) wxEVT_HELP));
	 PyDict_SetItemString(d,"wxEVT_DETAILED_HELP", PyInt_FromLong((long) wxEVT_DETAILED_HELP));
{
   int i;
   for (i = 0; _swig_mapping[i].n1; i++)
        SWIG_RegisterMapping(_swig_mapping[i].n1,_swig_mapping[i].n2,_swig_mapping[i].pcnv);
}
}
