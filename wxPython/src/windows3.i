/////////////////////////////////////////////////////////////////////////////
// Name:        windows3.i
// Purpose:     SWIG definitions of MORE window classes
//
// Author:      Robin Dunn
//
// Created:     22-Dec-1998
// RCS-ID:      $Id$
// Copyright:   (c) 1998 by Total Control Software
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

%module windows3

%{
#include "helpers.h"
#include <wx/sashwin.h>
#include <wx/laywin.h>
#ifndef __WXMAC__
#include <wx/popupwin.h>
#endif
%}

//----------------------------------------------------------------------

%include typemaps.i
%include my_typemaps.i

// Import some definitions of other classes, etc.
%import _defs.i
%import misc.i
%import gdi.i
%import windows.i
%import windows2.i
%import mdi.i
%import events.i

%pragma(python) code = "import wx"

//---------------------------------------------------------------------------

enum wxSashEdgePosition {
    wxSASH_TOP = 0,
    wxSASH_RIGHT,
    wxSASH_BOTTOM,
    wxSASH_LEFT,
    wxSASH_NONE = 100
};

enum {
    wxEVT_SASH_DRAGGED,
    wxSW_3D,
    wxSW_3DSASH,
    wxSW_3DBORDER,
    wxSW_BORDER
};

enum wxSashDragStatus
{
    wxSASH_STATUS_OK,
    wxSASH_STATUS_OUT_OF_RANGE
};


class wxSashEvent : public wxCommandEvent {
public:
    wxSashEvent(int id = 0, wxSashEdgePosition edge = wxSASH_NONE);

    void SetEdge(wxSashEdgePosition edge);
    wxSashEdgePosition GetEdge();
    void SetDragRect(const wxRect& rect);
    wxRect GetDragRect();
    void SetDragStatus(wxSashDragStatus status);
    wxSashDragStatus GetDragStatus();
};



class wxSashWindow: public wxWindow {
public:
    wxSashWindow(wxWindow* parent, wxWindowID id,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = wxCLIP_CHILDREN | wxSW_3D,
                 const char* name = "sashWindow");
    %name(wxPreSashWindow)wxSashWindow();

    bool Create(wxWindow* parent, wxWindowID id,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = wxCLIP_CHILDREN | wxSW_3D,
                 const char* name = "sashWindow");

    %pragma(python) addtomethod = "__init__:self._setOORInfo(self)"
    %pragma(python) addtomethod = "wxPreSashWindow:val._setOORInfo(val)"

    bool GetSashVisible(wxSashEdgePosition edge);
    int GetDefaultBorderSize();
    int GetEdgeMargin(wxSashEdgePosition edge);
    int GetExtraBorderSize();
    int GetMaximumSizeX();
    int GetMaximumSizeY();
    int GetMinimumSizeX();
    int GetMinimumSizeY();
    bool HasBorder(wxSashEdgePosition edge);
    void SetDefaultBorderSize(int width);
    void SetExtraBorderSize(int width);
    void SetMaximumSizeX(int min);
    void SetMaximumSizeY(int min);
    void SetMinimumSizeX(int min);
    void SetMinimumSizeY(int min);
    void SetSashVisible(wxSashEdgePosition edge, bool visible);
    void SetSashBorder(wxSashEdgePosition edge, bool hasBorder);

};


//---------------------------------------------------------------------------

enum wxLayoutOrientation {
    wxLAYOUT_HORIZONTAL,
    wxLAYOUT_VERTICAL
};

enum wxLayoutAlignment {
    wxLAYOUT_NONE,
    wxLAYOUT_TOP,
    wxLAYOUT_LEFT,
    wxLAYOUT_RIGHT,
    wxLAYOUT_BOTTOM,
};


enum {
     wxEVT_QUERY_LAYOUT_INFO,
     wxEVT_CALCULATE_LAYOUT,
};


class wxQueryLayoutInfoEvent: public wxEvent {
public:
    wxQueryLayoutInfoEvent(wxWindowID id = 0);

    void SetRequestedLength(int length);
    int GetRequestedLength();
    void SetFlags(int flags);
    int GetFlags();
    void SetSize(const wxSize& size);
    wxSize GetSize();
    void SetOrientation(wxLayoutOrientation orient);
    wxLayoutOrientation GetOrientation();
    void SetAlignment(wxLayoutAlignment align);
    wxLayoutAlignment GetAlignment();
};



class wxCalculateLayoutEvent: public wxEvent {
public:
    wxCalculateLayoutEvent(wxWindowID id = 0);

    void SetFlags(int flags);
    int GetFlags();
    void SetRect(const wxRect& rect);
    wxRect GetRect();
};


class wxSashLayoutWindow: public wxSashWindow {
public:
    wxSashLayoutWindow(wxWindow* parent, wxWindowID id,
                       const wxPoint& pos = wxDefaultPosition,
                       const wxSize& size = wxDefaultSize,
                       long style = wxCLIP_CHILDREN | wxSW_3D,
                       const char* name = "layoutWindow");
    %name(wxPreSashLayoutWindow)wxSashLayoutWindow();

    bool Create(wxWindow* parent, wxWindowID id,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxCLIP_CHILDREN | wxSW_3D,
                const char* name = "layoutWindow");

    %pragma(python) addtomethod = "__init__:self._setOORInfo(self)"
    %pragma(python) addtomethod = "wxPreSashLayoutWindow:val._setOORInfo(val)"

    wxLayoutAlignment GetAlignment();
    wxLayoutOrientation GetOrientation();
    void SetAlignment(wxLayoutAlignment alignment);
    void SetDefaultSize(const wxSize& size);
    void SetOrientation(wxLayoutOrientation orientation);
};

//---------------------------------------------------------------------------

class wxLayoutAlgorithm : public wxObject {
public:
    wxLayoutAlgorithm();
    ~wxLayoutAlgorithm();

    bool LayoutMDIFrame(wxMDIParentFrame* frame, wxRect* rect = NULL);
    bool LayoutFrame(wxFrame* frame, wxWindow* mainWindow = NULL);
    bool LayoutWindow(wxWindow* parent, wxWindow* mainWindow = NULL);
};


//---------------------------------------------------------------------------
#ifndef __WXMAC__

// wxPopupWindow: a special kind of top level window used for popup menus,
// combobox popups and such.
class wxPopupWindow : public wxWindow {
public:
    wxPopupWindow(wxWindow *parent, int flags = wxBORDER_NONE);
    %name(wxPrePopupWindow)wxPopupWindow();

    %pragma(python) addtomethod = "__init__:self._setOORInfo(self)"
    %pragma(python) addtomethod = "wxPrePopupWindow:val._setOORInfo(val)"

    bool Create(wxWindow *parent, int flags = wxBORDER_NONE);

    void Position(const wxPoint& ptOrigin,
                  const wxSize& size);
};


%{
class wxPyPopupTransientWindow : public wxPopupTransientWindow
{
public:
    wxPyPopupTransientWindow() : wxPopupTransientWindow() {}
    wxPyPopupTransientWindow(wxWindow* parent, int style = wxBORDER_NONE)
        : wxPopupTransientWindow(parent, style) {}

    DEC_PYCALLBACK_BOOL_ME(ProcessLeftDown);
    PYPRIVATE;
};

IMP_PYCALLBACK_BOOL_ME(wxPyPopupTransientWindow, wxPopupTransientWindow, ProcessLeftDown);

%}



// wxPopupTransientWindow: a wxPopupWindow which disappears automatically
// when the user clicks mouse outside it or if it loses focus in any other way
%name(wxPopupTransientWindow) class wxPyPopupTransientWindow : public wxPopupWindow
{
public:
    wxPyPopupTransientWindow(wxWindow *parent, int style = wxBORDER_NONE);
    %name(wxPrePopupTransientWindow)wxPyPopupTransientWindow();

    void _setCallbackInfo(PyObject* self, PyObject* _class);
    %pragma(python) addtomethod = "__init__:self._setCallbackInfo(self, wxPopupTransientWindow)"

    %pragma(python) addtomethod = "__init__:self._setOORInfo(self)"
    %pragma(python) addtomethod = "wxPrePopupTransientWindow:val._setOORInfo(val)"

    // popup the window (this will show it too) and keep focus at winFocus
    // (or itself if it's NULL), dismiss the popup if we lose focus
    virtual void Popup(wxWindow *focus = NULL);

    // hide the window
    virtual void Dismiss();
};

#endif
//---------------------------------------------------------------------------

