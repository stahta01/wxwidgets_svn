/////////////////////////////////////////////////////////////////////////////
// Name:        wxp.i
// Purpose:     SWIG interface file for a python wxWindows module
//
// Author:      Robin Dunn
//
// Created:     5/22/98
// RCS-ID:      $Id$
// Copyright:   (c) 1998 by Total Control Software
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

%module wxp


%{
#include "helpers.h"
%}

//----------------------------------------------------------------------
// This is where we include the other wrapper definition files for SWIG
//----------------------------------------------------------------------

%include typemaps.i
%include my_typemaps.i
%include _defs.i

%import misc.i
%import windows.i
%import events.i
%import gdi.i
%import mdi.i
%import controls.i
%import controls2.i
%import windows2.i
%import cmndlgs.i
%import stattool.i
%import frames.i



%native(_wxStart)           __wxStart;
%native(_wxSetDictionary)   __wxSetDictionary;

//---------------------------------------------------------------------------

#define __version__ "0.4.1"

wxPoint     wxPyDefaultPosition;
wxSize      wxPyDefaultSize;

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

class wxPyApp : public wxEvtHandler {
public:
    %addmethods {
        wxPyApp() {
            wxPythonApp = new wxPyApp();
            return wxPythonApp;
        }
    }


    wxString GetAppName();
#ifdef __WXMSW__
    bool GetAuto3D();
#endif
    wxString GetClassName();
    bool GetExitOnFrameDelete();
    int GetPrintMode();
    wxWindow * GetTopWindow();
    wxString GetVendorName();

    void Dispatch();
    void ExitMainLoop();
    bool Initialized();
    int  MainLoop();
    bool Pending();

    void SetAppName(const wxString& name);
#ifdef __WXMSW__
    void SetAuto3D(bool auto3D);
#endif
    void SetClassName(const wxString& name);
    void SetExitOnFrameDelete(bool flag);
    void SetPrintMode(int mode);
    void SetTopWindow(wxWindow* window);
    void SetVendorName(const wxString& name);

    // This one is wxPython specific.  If you override MainLoop,
    // call this when done.
    void AfterMainLoop();
};



//----------------------------------------------------------------------
// This code gets added to the module initialization function
//----------------------------------------------------------------------

%{
extern "C" SWIGEXPORT(void,initwindowsc)();
extern "C" SWIGEXPORT(void,initwindows2c)();
extern "C" SWIGEXPORT(void,initeventsc)();
extern "C" SWIGEXPORT(void,initmiscc)();
extern "C" SWIGEXPORT(void,initgdic)();
extern "C" SWIGEXPORT(void,initmdic)();
extern "C" SWIGEXPORT(void,initcontrolsc)();
extern "C" SWIGEXPORT(void,initcontrols2c)();
extern "C" SWIGEXPORT(void,initcmndlgsc)();
extern "C" SWIGEXPORT(void,initstattoolc)();
extern "C" SWIGEXPORT(void,initframesc)();

%}

%init %{

    __wxPreStart();     // initialize the GUI toolkit, if needed.

//    wxPyWindows = new wxHashTable(wxKEY_INTEGER, 100);

        // Since these modules are all linked together, initialize them now
        // because python won't be able to find their shared library files,
        // (since there isn't any.)
    initwindowsc();
    initwindows2c();
    initeventsc();
    initmiscc();
    initgdic();
    initmdic();
    initcontrolsc();
    initcontrols2c();
    initcmndlgsc();
    initstattoolc();
    initframesc();

%}

//----------------------------------------------------------------------
// And this gets appended to the shadow class file.
//----------------------------------------------------------------------

%pragma(python) include="_extras.py";


/////////////////////////////////////////////////////////////////////////////
//
// $Log$
// Revision 1.8  1998/10/07 07:34:35  RD
// Version 0.4.1 for wxGTK
//
// Revision 1.7  1998/10/02 06:40:44  RD
//
// Version 0.4 of wxPython for MSW.
//
// Revision 1.6  1998/08/27 21:59:10  RD
// Some chicken-and-egg problems solved for wxPython on wxGTK
//
// Revision 1.5  1998/08/27 00:00:28  RD
// - more tweaks
// - have discovered some problems but not yet discovered solutions...
//
// Revision 1.4  1998/08/18 19:48:20  RD
// more wxGTK compatibility things.
//
// It builds now but there are serious runtime problems...
//
// Revision 1.3  1998/08/15 07:36:53  RD
// - Moved the header in the .i files out of the code that gets put into
// the .cpp files.  It caused CVS conflicts because of the RCS ID being
// different each time.
//
// - A few minor fixes.
//
// Revision 1.2  1998/08/14 23:36:49  RD
// Beginings of wxGTK compatibility
//
// Revision 1.1  1998/08/09 08:25:53  RD
// Initial version
//
//


