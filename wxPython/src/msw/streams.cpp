/*
 * FILE : src/msw/streams.cpp
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

#include "Python.h"

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
#define SWIG_init    initstreamsc

#define SWIG_name    "streamsc"

#include "helpers.h"
#include <wx/stream.h>
#include <wx/list.h>

static PyObject* l_output_helper(PyObject* target, PyObject* o) {
    PyObject*   o2;
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
  // C++
// definitions of wxStringPtrList and wxPyInputStream
#include <wx/listimpl.cpp>
WX_DEFINE_LIST(wxStringPtrList);


void wxPyInputStream::close() {
    /* do nothing */
}

void wxPyInputStream::flush() {
    /*do nothing*/
}

bool wxPyInputStream::eof() {
    if (wxi)
        return wxi->Eof();
    else
        return TRUE;
}

wxPyInputStream::~wxPyInputStream() {
    /*do nothing*/
}

wxString* wxPyInputStream::read(int size) {
    wxString* s = NULL;
    const int BUFSIZE = 1024;

    // check if we have a real wxInputStream to work with
    if (!wxi) {
        PyErr_SetString(PyExc_IOError,"no valid C-wxInputStream below");
        return NULL;
    }

    if (size < 0) {
        // init buffers
        char * buf = new char[BUFSIZE];
        if (!buf) {
            PyErr_NoMemory();
            return NULL;
        }

        s = new wxString();
        if (!s) {
            delete buf;
            PyErr_NoMemory();
            return NULL;
        }

        // read until EOF
        wxPy_BEGIN_ALLOW_THREADS;
        while (! wxi->Eof()) {
            wxi->Read(buf, BUFSIZE);
            //*s += wxString(buf, wxi->LastRead());
            s->Append(buf, wxi->LastRead());
        }
        delete buf;
        wxPy_END_ALLOW_THREADS;

        // error check
        if (wxi->LastError() == wxSTREAM_READ_ERROR) {
            delete s;
            PyErr_SetString(PyExc_IOError,"IOError in wxInputStream");
            return NULL;
        }

    } else {  // Read only size number of characters
        s = new wxString;
        if (!s) {
            PyErr_NoMemory();
            return NULL;
        }

        // read size bytes
        wxPy_BEGIN_ALLOW_THREADS;
        wxi->Read(s->GetWriteBuf(size+1), size);
        s->UngetWriteBuf(wxi->LastRead());
        wxPy_END_ALLOW_THREADS;

        // error check
        if (wxi->LastError() == wxSTREAM_READ_ERROR) {
            delete s;
            PyErr_SetString(PyExc_IOError,"IOError in wxInputStream");
            return NULL;
        }
    }
    return s;
}


wxString* wxPyInputStream::readline (int size) {
    // check if we have a real wxInputStream to work with
    if (!wxi) {
        PyErr_SetString(PyExc_IOError,"no valid C-wxInputStream below");
        return NULL;
    }

    // init buffer
    int i;
    char ch;
    wxString* s = new wxString;
    if (!s) {
        PyErr_NoMemory();
        return NULL;
    }

    // read until \n or byte limit reached
    wxPy_BEGIN_ALLOW_THREADS;
    for (i=ch=0; (ch != '\n') && (!wxi->Eof()) && ((size < 0) || (i < size)); i++) {
        *s += ch = wxi->GetC();
    }
    wxPy_END_ALLOW_THREADS;

    // errorcheck
    if (wxi->LastError() == wxSTREAM_READ_ERROR) {
        delete s;
        PyErr_SetString(PyExc_IOError,"IOError in wxInputStream");
        return NULL;
    }
    return s;
}


wxStringPtrList* wxPyInputStream::readlines (int sizehint) {
    // check if we have a real wxInputStream to work with
    if (!wxi) {
        PyErr_SetString(PyExc_IOError,"no valid C-wxInputStream below");
        return NULL;
    }

    // init list
    wxStringPtrList* l = new wxStringPtrList();
    if (!l) {
        PyErr_NoMemory();
        return NULL;
    }

    // read sizehint bytes or until EOF
    wxPy_BEGIN_ALLOW_THREADS;
    int i;
    for (i=0; (!wxi->Eof()) && ((sizehint < 0) || (i < sizehint));) {
        wxString* s = readline();
        if (s == NULL) {
            l->DeleteContents(TRUE);
            l->Clear();
            return NULL;
        }
        l->Append(s);
        i = i + s->Length();
    }
    wxPy_END_ALLOW_THREADS;

    // error check
    if (wxi->LastError() == wxSTREAM_READ_ERROR) {
        l->DeleteContents(TRUE);
        l->Clear();
        PyErr_SetString(PyExc_IOError,"IOError in wxInputStream");
        return NULL;
    }
    return l;
}


void wxPyInputStream::seek(int offset, int whence) {
    if (wxi)
        wxi->SeekI(offset, wxSeekMode(whence));
}

int wxPyInputStream::tell(){
    if (wxi)
        return wxi->TellI();
}



// wxInputStream which operates on a Python file-like object
class wxPyCBInputStream : public wxInputStream {
protected:
    PyObject* read;
    PyObject* seek;
    PyObject* tell;
    PyObject* py;

    virtual size_t OnSysRead(void *buffer, size_t bufsize) {
        if (bufsize == 0)
            return 0;

        bool doSave = wxPyRestoreThread();
        PyObject* arglist = Py_BuildValue("(i)", bufsize);
        PyObject* result = PyEval_CallObject(read, arglist);
        Py_DECREF(arglist);

        size_t o = 0;
        if ((result != NULL) && PyString_Check(result)) {
            o = PyString_Size(result);
            if (o == 0)
                m_lasterror = wxSTREAM_EOF;
            if (o > bufsize)
                o = bufsize;
            strncpy((char*)buffer, PyString_AsString(result), o);
            Py_DECREF(result);

        }
        else
            m_lasterror = wxSTREAM_READ_ERROR;
        wxPySaveThread(doSave);
        m_lastcount = o;
        return o;
    }

    virtual size_t OnSysWrite(const void *buffer, size_t bufsize){
        m_lasterror = wxSTREAM_WRITE_ERROR;
        return 0;
    }

    virtual off_t OnSysSeek(off_t off, wxSeekMode mode){
        bool doSave = wxPyRestoreThread();
        PyObject*arglist = Py_BuildValue("(ii)", off, mode);
        PyObject*result = PyEval_CallObject(seek, arglist);
        Py_DECREF(arglist);
        Py_XDECREF(result);
        wxPySaveThread(doSave);
        return OnSysTell();
    }

    virtual off_t OnSysTell() const{
        bool doSave = wxPyRestoreThread();
        PyObject* arglist = Py_BuildValue("()");
        PyObject* result = PyEval_CallObject(tell, arglist);
        Py_DECREF(arglist);
        off_t o = 0;
        if (result != NULL) {
            o = PyInt_AsLong(result);
            Py_DECREF(result);
        };
        wxPySaveThread(doSave);
        return o;
    }

    wxPyCBInputStream(PyObject *p, PyObject *r, PyObject *s, PyObject *t)
        : py(p), read(r), seek(s), tell(t)
        {}

public:
    ~wxPyCBInputStream() {
        bool doSave = wxPyRestoreThread();
        Py_XDECREF(py);
        Py_XDECREF(read);
        Py_XDECREF(seek);
        Py_XDECREF(tell);
        wxPySaveThread(doSave);
    }

    virtual size_t GetSize() {
        if (seek && tell) {
            off_t temp = OnSysTell();
            off_t ret = OnSysSeek(0, wxFromEnd);
            OnSysSeek(temp, wxFromStart);
            return ret;
        }
        else
            return 0;
    }

    static wxPyCBInputStream* create(PyObject *py) {
        PyObject* read;
        PyObject* seek;
        PyObject* tell;

        if (!PyInstance_Check(py) && !PyFile_Check(py)) {
            PyErr_SetString(PyExc_TypeError, "Not a file-like object");
            Py_XDECREF(py);
            return NULL;
        }
        read = getMethod(py, "read");
        seek = getMethod(py, "seek");
        tell = getMethod(py, "tell");

        if (!read) {
            PyErr_SetString(PyExc_TypeError, "Not a file-like object");
            Py_XDECREF(py);
            Py_XDECREF(read);
            Py_XDECREF(seek);
            Py_XDECREF(tell);
            return NULL;
        }
        return new wxPyCBInputStream(py, read, seek, tell);
    }

    static PyObject* getMethod(PyObject* py, char* name) {
        if (!PyObject_HasAttrString(py, name))
            return NULL;
        PyObject* o = PyObject_GetAttrString(py, name);
        if (!PyMethod_Check(o) && !PyCFunction_Check(o))
            return NULL;
        return o;
    }

protected:

};

#ifdef __cplusplus
extern "C" {
#endif
static wxPyInputStream *new_wxPyInputStream(PyObject *p) {
            wxInputStream* wxi = wxPyCBInputStream::create(p);
            if (wxi)
                return new wxPyInputStream(wxi);
            else
                return NULL;
        }

static PyObject *_wrap_new_wxInputStream(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxPyInputStream * _result;
    PyObject * _arg0;
    PyObject * _obj0 = 0;
    char *_kwnames[] = { "p", NULL };

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"O:new_wxInputStream",_kwnames,&_obj0)) 
        return NULL;
{
  _arg0 = _obj0;
}
{
        _result = (wxPyInputStream *)new_wxPyInputStream(_arg0);

}{
    char _ptemp[128];
    if (_result) {
        SWIG_MakePtr(_ptemp, (char *) _result,"_wxPyInputStream_p");
        _resultobj = Py_BuildValue("s",_ptemp);
    }
    else
        _resultobj=0;
}
    return _resultobj;
}

#define wxInputStream_close(_swigobj)  (_swigobj->close())
static PyObject *_wrap_wxInputStream_close(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxPyInputStream * _arg0;
    PyObject * _argo0 = 0;
    char *_kwnames[] = { "self", NULL };

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"O:wxInputStream_close",_kwnames,&_argo0)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxPyInputStream_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxInputStream_close. Expected _wxPyInputStream_p.");
        return NULL;
        }
    }
{
        wxInputStream_close(_arg0);

}    Py_INCREF(Py_None);
    _resultobj = Py_None;
    return _resultobj;
}

#define wxInputStream_flush(_swigobj)  (_swigobj->flush())
static PyObject *_wrap_wxInputStream_flush(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxPyInputStream * _arg0;
    PyObject * _argo0 = 0;
    char *_kwnames[] = { "self", NULL };

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"O:wxInputStream_flush",_kwnames,&_argo0)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxPyInputStream_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxInputStream_flush. Expected _wxPyInputStream_p.");
        return NULL;
        }
    }
{
        wxInputStream_flush(_arg0);

}    Py_INCREF(Py_None);
    _resultobj = Py_None;
    return _resultobj;
}

#define wxInputStream_eof(_swigobj)  (_swigobj->eof())
static PyObject *_wrap_wxInputStream_eof(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    bool  _result;
    wxPyInputStream * _arg0;
    PyObject * _argo0 = 0;
    char *_kwnames[] = { "self", NULL };

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"O:wxInputStream_eof",_kwnames,&_argo0)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxPyInputStream_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxInputStream_eof. Expected _wxPyInputStream_p.");
        return NULL;
        }
    }
{
        _result = (bool )wxInputStream_eof(_arg0);

}    _resultobj = Py_BuildValue("i",_result);
    return _resultobj;
}

#define wxInputStream_read(_swigobj,_swigarg0)  (_swigobj->read(_swigarg0))
static PyObject *_wrap_wxInputStream_read(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxString * _result;
    wxPyInputStream * _arg0;
    int  _arg1 = (int ) -1;
    PyObject * _argo0 = 0;
    char *_kwnames[] = { "self","size", NULL };

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"O|i:wxInputStream_read",_kwnames,&_argo0,&_arg1)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxPyInputStream_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxInputStream_read. Expected _wxPyInputStream_p.");
        return NULL;
        }
    }
{
        _result = (wxString *)wxInputStream_read(_arg0,_arg1);

}{
    if (_result) {
        _resultobj = PyString_FromStringAndSize(_result->c_str(), _result->Len());
        delete _result;
    }
    else
        _resultobj=0;
}
    return _resultobj;
}

#define wxInputStream_readline(_swigobj,_swigarg0)  (_swigobj->readline(_swigarg0))
static PyObject *_wrap_wxInputStream_readline(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxString * _result;
    wxPyInputStream * _arg0;
    int  _arg1 = (int ) -1;
    PyObject * _argo0 = 0;
    char *_kwnames[] = { "self","size", NULL };

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"O|i:wxInputStream_readline",_kwnames,&_argo0,&_arg1)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxPyInputStream_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxInputStream_readline. Expected _wxPyInputStream_p.");
        return NULL;
        }
    }
{
        _result = (wxString *)wxInputStream_readline(_arg0,_arg1);

}{
    if (_result) {
        _resultobj = PyString_FromStringAndSize(_result->c_str(), _result->Len());
        delete _result;
    }
    else
        _resultobj=0;
}
    return _resultobj;
}

#define wxInputStream_readlines(_swigobj,_swigarg0)  (_swigobj->readlines(_swigarg0))
static PyObject *_wrap_wxInputStream_readlines(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxStringPtrList * _result;
    wxPyInputStream * _arg0;
    int  _arg1 = (int ) -1;
    PyObject * _argo0 = 0;
    char *_kwnames[] = { "self","sizehint", NULL };

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"O|i:wxInputStream_readlines",_kwnames,&_argo0,&_arg1)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxPyInputStream_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxInputStream_readlines. Expected _wxPyInputStream_p.");
        return NULL;
        }
    }
{
        _result = (wxStringPtrList *)wxInputStream_readlines(_arg0,_arg1);

}{
    if (_result) {
        _resultobj = PyList_New(_result->GetCount());
        wxStringPtrList::Node *node = _result->GetFirst();
        for (int i=0; node; i++) {
            wxString *s = node->GetData();
            PyList_SetItem(_resultobj, i, PyString_FromStringAndSize(s->c_str(), s->Len()));
            node = node->GetNext();
            delete s;
        }
        delete _result;
    }
    else
        _resultobj=0;
}
    return _resultobj;
}

#define wxInputStream_seek(_swigobj,_swigarg0,_swigarg1)  (_swigobj->seek(_swigarg0,_swigarg1))
static PyObject *_wrap_wxInputStream_seek(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxPyInputStream * _arg0;
    int  _arg1;
    int  _arg2 = (int ) 0;
    PyObject * _argo0 = 0;
    char *_kwnames[] = { "self","offset","whence", NULL };

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"Oi|i:wxInputStream_seek",_kwnames,&_argo0,&_arg1,&_arg2)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxPyInputStream_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxInputStream_seek. Expected _wxPyInputStream_p.");
        return NULL;
        }
    }
{
        wxInputStream_seek(_arg0,_arg1,_arg2);

}    Py_INCREF(Py_None);
    _resultobj = Py_None;
    return _resultobj;
}

#define wxInputStream_tell(_swigobj)  (_swigobj->tell())
static PyObject *_wrap_wxInputStream_tell(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    int  _result;
    wxPyInputStream * _arg0;
    PyObject * _argo0 = 0;
    char *_kwnames[] = { "self", NULL };

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"O:wxInputStream_tell",_kwnames,&_argo0)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxPyInputStream_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxInputStream_tell. Expected _wxPyInputStream_p.");
        return NULL;
        }
    }
{
        _result = (int )wxInputStream_tell(_arg0);

}    _resultobj = Py_BuildValue("i",_result);
    return _resultobj;
}

static void  wxOutputStream_write(wxOutputStream *self,const wxString & str) {
            self->Write(str.c_str(), str.Length());
        }
static PyObject *_wrap_wxOutputStream_write(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxOutputStream * _arg0;
    wxString * _arg1;
    PyObject * _argo0 = 0;
    PyObject * _obj1 = 0;
    char *_kwnames[] = { "self","str", NULL };

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"OO:wxOutputStream_write",_kwnames,&_argo0,&_obj1)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxOutputStream_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxOutputStream_write. Expected _wxOutputStream_p.");
        return NULL;
        }
    }
{
    if (!PyString_Check(_obj1)) {
        PyErr_SetString(PyExc_TypeError, wxStringErrorMsg);
        return NULL;
    }
    _arg1 = new wxString(PyString_AsString(_obj1), PyString_Size(_obj1));
}
{
        wxOutputStream_write(_arg0,*_arg1);

}    Py_INCREF(Py_None);
    _resultobj = Py_None;
{
    if (_obj1)
        delete _arg1;
}
    return _resultobj;
}

static PyMethodDef streamscMethods[] = {
	 { "wxOutputStream_write", (PyCFunction) _wrap_wxOutputStream_write, METH_VARARGS | METH_KEYWORDS },
	 { "wxInputStream_tell", (PyCFunction) _wrap_wxInputStream_tell, METH_VARARGS | METH_KEYWORDS },
	 { "wxInputStream_seek", (PyCFunction) _wrap_wxInputStream_seek, METH_VARARGS | METH_KEYWORDS },
	 { "wxInputStream_readlines", (PyCFunction) _wrap_wxInputStream_readlines, METH_VARARGS | METH_KEYWORDS },
	 { "wxInputStream_readline", (PyCFunction) _wrap_wxInputStream_readline, METH_VARARGS | METH_KEYWORDS },
	 { "wxInputStream_read", (PyCFunction) _wrap_wxInputStream_read, METH_VARARGS | METH_KEYWORDS },
	 { "wxInputStream_eof", (PyCFunction) _wrap_wxInputStream_eof, METH_VARARGS | METH_KEYWORDS },
	 { "wxInputStream_flush", (PyCFunction) _wrap_wxInputStream_flush, METH_VARARGS | METH_KEYWORDS },
	 { "wxInputStream_close", (PyCFunction) _wrap_wxInputStream_close, METH_VARARGS | METH_KEYWORDS },
	 { "new_wxInputStream", (PyCFunction) _wrap_new_wxInputStream, METH_VARARGS | METH_KEYWORDS },
	 { NULL, NULL }
};
#ifdef __cplusplus
}
#endif
/*
 * This table is used by the pointer type-checker
 */
static struct { char *n1; char *n2; void *(*pcnv)(void *); } _swig_mapping[] = {
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
    { "_char","_wxChar",0},
    { "_EBool","_wxCoord",0},
    { "_EBool","_wxPrintQuality",0},
    { "_EBool","_signed_int",0},
    { "_EBool","_int",0},
    { "_EBool","_wxWindowID",0},
    { "_unsigned_long","_long",0},
    { "_wxPyInputStream","_class_wxPyInputStream",0},
    { "_class_wxOutputStream","_wxOutputStream",0},
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
    { "_class_wxPyInputStream","_wxPyInputStream",0},
    { "_signed_short","_WXTYPE",0},
    { "_signed_short","_short",0},
    { "_unsigned_char","_byte",0},
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
    { "_wxCoord","_int",0},
    { "_wxCoord","_signed_int",0},
    { "_wxCoord","_unsigned_int",0},
    { "_wxCoord","_wxWindowID",0},
    { "_wxCoord","_uint",0},
    { "_wxCoord","_EBool",0},
    { "_wxCoord","_size_t",0},
    { "_wxCoord","_time_t",0},
    { "_wxCoord","_wxPrintQuality",0},
    { "_wxOutputStream","_class_wxOutputStream",0},
{0,0,0}};

static PyObject *SWIG_globals;
#ifdef __cplusplus
extern "C" 
#endif
SWIGEXPORT(void) initstreamsc() {
	 PyObject *m, *d;
	 SWIG_globals = SWIG_newvarlink();
	 m = Py_InitModule("streamsc", streamscMethods);
	 d = PyModule_GetDict(m);
{
   int i;
   for (i = 0; _swig_mapping[i].n1; i++)
        SWIG_RegisterMapping(_swig_mapping[i].n1,_swig_mapping[i].n2,_swig_mapping[i].pcnv);
}
}
