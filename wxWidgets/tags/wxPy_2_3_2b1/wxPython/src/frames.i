/////////////////////////////////////////////////////////////////////////////
// Name:        frames.i
// Purpose:     SWIG definitions of various window classes
//
// Author:      Robin Dunn
//
// Created:     8/27/98
// RCS-ID:      $Id$
// Copyright:   (c) 1998 by Total Control Software
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////


%module frames

%{
#include "helpers.h"
#include <wx/minifram.h>
#include <wx/tipwin.h>
%}

//----------------------------------------------------------------------

%include typemaps.i
%include my_typemaps.i

// Import some definitions of other classes, etc.
%import _defs.i
%import misc.i
%import gdi.i
%import windows.i
%import stattool.i

%pragma(python) code = "import wx"

//----------------------------------------------------------------------

enum {
    wxFULLSCREEN_NOMENUBAR,
    wxFULLSCREEN_NOTOOLBAR,
    wxFULLSCREEN_NOSTATUSBAR,
    wxFULLSCREEN_NOBORDER,
    wxFULLSCREEN_NOCAPTION,
    wxFULLSCREEN_ALL
};


class wxFrame : public wxWindow {
public:
    wxFrame(wxWindow* parent, const wxWindowID id, const wxString& title,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxDEFAULT_FRAME_STYLE,
            char* name = "frame");
    %name(wxPreFrame)wxFrame();

    bool Create(wxWindow* parent, const wxWindowID id, const wxString& title,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxDEFAULT_FRAME_STYLE,
                char* name = "frame");

    void Centre(int direction = wxBOTH);
    wxStatusBar* CreateStatusBar(int number = 1,
                                 long style = wxST_SIZEGRIP,
                                 wxWindowID id = -1,
                                 char* name = "statusBar");
    wxToolBar* CreateToolBar(long style = wxNO_BORDER|wxTB_HORIZONTAL|wxTB_FLAT,
                             wxWindowID id = -1,
                             char* name = "toolBar");

    const wxIcon& GetIcon();
    wxMenuBar* GetMenuBar();
    wxStatusBar* GetStatusBar();
    wxString GetTitle();
    wxToolBar* GetToolBar();
    void Iconize(bool iconize);
    bool IsIconized();
    void Maximize(bool maximize);
    bool IsMaximized();
    void Restore();
    void SetAcceleratorTable(const wxAcceleratorTable& accel);
    void SetIcon(const wxIcon& icon);
    void SetMenuBar(wxMenuBar* menuBar);
    void SetStatusBar(wxStatusBar *statusBar);
    void SetStatusText(const wxString& text, int number = 0);
    void SetStatusWidths(int LCOUNT, int* choices); // uses typemap
    void SetTitle(const wxString& title);
    void SetToolBar(wxToolBar* toolbar);
    void MakeModal(bool modal = TRUE);
    wxPoint GetClientAreaOrigin() const;
    bool Command(int id);
    bool ProcessCommand(int id);
    bool ShowFullScreen(bool show, long style = wxFULLSCREEN_ALL);
    bool IsFullScreen();
};

//---------------------------------------------------------------------------

class wxMiniFrame : public wxFrame {
public:
    wxMiniFrame(wxWindow* parent, const wxWindowID id, const wxString& title,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxDEFAULT_FRAME_STYLE,
                char* name = "frame");
    %name(wxPreMiniFrame)wxMiniFrame();

    bool Create(wxWindow* parent, const wxWindowID id, const wxString& title,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxDEFAULT_FRAME_STYLE,
                char* name = "frame");
};


//---------------------------------------------------------------------------

class wxTipWindow : public wxFrame
{
public:
    wxTipWindow(wxWindow *parent,
                const wxString& text,
                wxCoord maxLength = 100);
};


//---------------------------------------------------------------------------





