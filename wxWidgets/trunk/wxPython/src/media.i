/////////////////////////////////////////////////////////////////////////////
// Name:        media.i
// Purpose:     SWIG definitions for the wxMediaCtrl
//
// Author:      Robin Dunn
//
// Created:     23-Nov-2004
// RCS-ID:      $Id$
// Copyright:   (c) 2004 by Total Control Software
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

%define DOCSTRING
"Classes for a media player control"
%enddef

%module(package="wx", docstring=DOCSTRING) media


%{
#include "wx/wxPython/wxPython.h"
#include "wx/wxPython/pyclasses.h"

#include <wx/mediactrl.h>
#include <wx/uri.h>
%}

//----------------------------------------------------------------------

%import core.i
%pythoncode { wx = _core }
%pythoncode { __docfilter__ = wx.__DocFilter(globals()) }

%include _media_rename.i

//---------------------------------------------------------------------------

%typemap(in) wxLongLong {
    $1 = PyLong_AsLongLong($input);
}


%typemap(out) wxLongLong {
    $result = PyLong_FromLongLong($1.GetValue());
}

//---------------------------------------------------------------------------
// Make a stubbed out class for platforms that don't have wxMediaCtrl
%{
#if !wxUSE_MEDIACTRL
enum wxMediaState
{
    wxMEDIASTATE_STOPPED=0,
    wxMEDIASTATE_PAUSED=0,
    wxMEDIASTATE_PLAYING=0
};


class wxMediaEvent : public wxNotifyEvent
{
public:
    wxMediaEvent(wxEventType, int )    { wxPyRaiseNotImplemented(); }
};

class wxMediaCtrl : public wxControl
{
public:
    wxMediaCtrl()    { wxPyRaiseNotImplemented(); }

    wxMediaCtrl(wxWindow* , wxWindowID ,
                const wxString& ,
                const wxPoint& ,
                const wxSize& ,
                long style ,
                const wxString& ,
                const wxValidator& ,
                const wxString& ) { wxPyRaiseNotImplemented(); }

    bool Create(wxWindow* , wxWindowID ,
                const wxString& ,
                const wxPoint& ,
                const wxSize& ,
                long style ,
                const wxString& ,
                const wxValidator& ,
                const wxString& ) { return false; }

    bool Play() { return false; }
    bool Pause() { return false; }
    bool Stop() { return false; }

    bool Load(const wxString& fileName) { return false; }
    bool Load(const wxURI& location) { return false; }

    wxMediaState GetState() { return wxMEDIASTATE_STOPPED; }

    double GetPlaybackRate()  { return 0.0; }
    bool SetPlaybackRate(double dRate) { return false; }

    wxFileOffset Seek(wxFileOffset where, wxSeekMode mode = wxFromStart)
    { return 0; }
    
    wxFileOffset Tell()    { return 0; }
    wxFileOffset Length()    { return 0; }
};

const wxEventType wxEVT_MEDIA_FINISHED = 0;
const wxEventType wxEVT_MEDIA_STOP = 0;

#endif
%}



//---------------------------------------------------------------------------

enum wxMediaState
{
    wxMEDIASTATE_STOPPED,
    wxMEDIASTATE_PAUSED,
    wxMEDIASTATE_PLAYING
};



// MAKE_CONST_WXSTRING(MEDIABACKEND_DIRECTSHOW);
// MAKE_CONST_WXSTRING(MEDIABACKEND_MCI       );
// MAKE_CONST_WXSTRING(MEDIABACKEND_QUICKTIME );

//---------------------------------------------------------------------------

class wxMediaEvent : public wxNotifyEvent
{
public:
    wxMediaEvent(wxEventType commandType = wxEVT_NULL, int id = 0);
};


//---------------------------------------------------------------------------

MAKE_CONST_WXSTRING_NOSWIG(EmptyString);
MAKE_CONST_WXSTRING2(MediaCtrlNameStr, wxT("mediaCtrl"));
MustHaveApp(wxMediaCtrl);


class wxMediaCtrl : public wxControl
{
public:
    %pythonAppend wxMediaCtrl      "self._setOORInfo(self)"
    %pythonAppend wxMediaCtrl()    ""


    wxMediaCtrl(wxWindow* parent,
                wxWindowID id=-1,
                const wxString& fileName = wxPyEmptyString,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0,
                const wxString& szBackend = wxPyEmptyString,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = wxPyMediaCtrlNameStr);
    
    %RenameCtor(PreMediaCtrl,  wxMediaCtrl());

    bool Create(wxWindow* parent,
                wxWindowID id=-1,
                const wxString& fileName = wxPyEmptyString,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0,
                const wxString& szBackend = wxPyEmptyString,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = wxPyMediaCtrlNameStr);


    bool Play();
    bool Pause();
    bool Stop();

    bool Load(const wxString& fileName);
    %extend {
        bool LoadFromURI(const wxString& location) {
            return self->Load(wxURI(location));
        }
    }

    wxMediaState GetState();

    double GetPlaybackRate();
    bool SetPlaybackRate(double dRate);

    wxFileOffset Seek(wxFileOffset where, wxSeekMode mode = wxFromStart);    
    wxFileOffset Tell();
    wxFileOffset Length();
};



%constant wxEventType wxEVT_MEDIA_FINISHED;
%constant wxEventType wxEVT_MEDIA_STOP;

%pythoncode {
EVT_MEDIA_FINISHED = wx.PyEventBinder( wxEVT_MEDIA_FINISHED, 1)
EVT_MEDIA_STOP     = wx.PyEventBinder( wxEVT_MEDIA_STOP, 1)
}    

//---------------------------------------------------------------------------

%init %{
%}

//---------------------------------------------------------------------------

