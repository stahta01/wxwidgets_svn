/////////////////////////////////////////////////////////////////////////////
// Name:        helpers.h
// Purpose:     Helper functions/classes for the wxPython extenaion module
//
// Author:      Robin Dunn
//
// Created:     7/1/97
// RCS-ID:      $Id$
// Copyright:   (c) 1998 by Total Control Software
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifndef __wxp_helpers__
#define __wxp_helpers__

#include <wx/wx.h>


//----------------------------------------------------------------------

class wxPyApp: public wxApp
{
public:
    int  MainLoop(void);
    bool OnInit(void);
    void AfterMainLoop(void);
};

extern wxPyApp *wxPythonApp;

//----------------------------------------------------------------------

void      __wxPreStart();
PyObject* __wxStart(PyObject*, PyObject* args);

extern PyObject* wxPython_dict;
PyObject* __wxSetDictionary(PyObject*, PyObject* args);

extern wxHashTable*  wxPyWindows;  // keep track of all windows so we
                                   // don't accidentally delete them twice.

void wxPyEventThunker(wxObject*, wxEvent& event);

//----------------------------------------------------------------------


#ifndef SWIGCODE
extern "C" void SWIG_MakePtr(char *, void *, char *);
extern "C" char *SWIG_GetPtr(char *, void **, char *);
#endif


#ifdef _MSC_VER
# pragma warning(disable:4800)
#endif

typedef unsigned char byte;


// Non-const versions to keep SWIG happy.
extern wxPoint  wxPyDefaultPosition;
extern wxSize   wxPyDefaultSize;
extern wxString wxPyEmptyStr;

//----------------------------------------------------------------------

class wxPyCallback : public wxObject {
public:
    wxPyCallback(PyObject* func)    { m_func = func; Py_INCREF(m_func); }
    ~wxPyCallback()                 { Py_DECREF(m_func); }

    void EventThunker(wxEvent& event);

    PyObject*   m_func;
};

//---------------------------------------------------------------------------

class wxPyMenu : public wxMenu {
public:
    wxPyMenu(const wxString& title = "", PyObject* func=NULL);
    ~wxPyMenu();

private:
    static void MenuCallback(wxMenu& menu, wxCommandEvent& evt);
    PyObject*   func;
};


//---------------------------------------------------------------------------

class wxPyTimer : public wxTimer {
public:
    wxPyTimer(PyObject* callback);
    ~wxPyTimer();

    void        Notify();

private:
    PyObject*   func;
};

//---------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////
//
// $Log$
// Revision 1.6  1998/11/25 08:45:26  RD
// Added wxPalette, wxRegion, wxRegionIterator, wxTaskbarIcon
// Added events for wxGrid
// Other various fixes and additions
//
// Revision 1.5  1998/10/02 06:40:40  RD
//
// Version 0.4 of wxPython for MSW.
//
// Revision 1.4  1998/08/27 21:59:09  RD
// Some chicken-and-egg problems solved for wxPython on wxGTK
//
// Revision 1.3  1998/08/16 04:31:09  RD
// More wxGTK work.
//
// Revision 1.2  1998/08/14 23:36:37  RD
// Beginings of wxGTK compatibility
//
// Revision 1.1  1998/08/09 08:25:51  RD
// Initial version
//
//

#endif

