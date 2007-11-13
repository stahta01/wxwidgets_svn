/////////////////////////////////////////////////////////////////////////////
// Name:        wx/hyperlink.h
// Purpose:     Hyperlink control
// Author:      David Norris <danorris@gmail.com>, Otto Wyss
// Modified by: Ryan Norton, Francesco Montorsi
// Created:     04/02/2005
// RCS-ID:      $Id$
// Copyright:   (c) 2005 David Norris
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_HYPERLINK_H_
#define _WX_HYPERLINK_H_

#include "wx/defs.h"

#if wxUSE_HYPERLINKCTRL

#include "wx/control.h"

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

#define wxHL_CONTEXTMENU        0x0001
#define wxHL_ALIGN_LEFT         0x0002
#define wxHL_ALIGN_RIGHT        0x0004
#define wxHL_ALIGN_CENTRE       0x0008
#define wxHL_DEFAULT_STYLE      (wxHL_CONTEXTMENU|wxNO_BORDER|wxHL_ALIGN_CENTRE)

extern WXDLLIMPEXP_DATA_ADV(const wxChar) wxHyperlinkCtrlNameStr[];


// ----------------------------------------------------------------------------
// wxHyperlinkCtrl
// ----------------------------------------------------------------------------

// A static text control that emulates a hyperlink. The link is displayed
// in an appropriate text style, derived from the control's normal font.
// When the mouse rolls over the link, the cursor changes to a hand and the
// link's color changes to the active color.
//
// Clicking on the link does not launch a web browser; instead, a
// HyperlinkEvent is fired. The event propagates upward until it is caught,
// just like a wxCommandEvent.
//
// Use the EVT_HYPERLINK() to catch link events.
class WXDLLIMPEXP_ADV wxHyperlinkCtrlBase : public wxControl
{
public:

    // get/set
    virtual wxColour GetHoverColour() const = 0;
    virtual void SetHoverColour(const wxColour &colour) = 0;

    virtual wxColour GetNormalColour() const = 0;
    virtual void SetNormalColour(const wxColour &colour) = 0;

    virtual wxColour GetVisitedColour() const = 0;
    virtual void SetVisitedColour(const wxColour &colour) = 0;

    virtual wxString GetURL() const = 0;
    virtual void SetURL (const wxString &url) = 0;

    virtual void SetVisited(bool visited = true) = 0;
    virtual bool GetVisited() const = 0;

    // NOTE: also wxWindow::Set/GetLabel, wxWindow::Set/GetBackgroundColour,
    //       wxWindow::Get/SetFont, wxWindow::Get/SetCursor are important !

protected:
    virtual wxBorder GetDefaultBorder() const { return wxBORDER_NONE; }

    // checks for validity some of the ctor/Create() function parameters
    void CheckParams(const wxString& label, const wxString& url, long style);

public:
    // not part of the public API but needs to be public as used by
    // GTK+ callbacks:
    void SendEvent();
};

#ifndef __WXDEBUG__
inline void wxHyperlinkCtrlBase::CheckParams(const wxString&, const wxString&, long) { }
#endif

// ----------------------------------------------------------------------------
// wxHyperlinkEvent
// ----------------------------------------------------------------------------

// Declare an event identifier.
BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_ADV, wxEVT_COMMAND_HYPERLINK, 3700)
END_DECLARE_EVENT_TYPES()

//
// An event fired when the user clicks on the label in a hyperlink control.
// See HyperlinkControl for details.
//
class WXDLLIMPEXP_ADV wxHyperlinkEvent : public wxCommandEvent
{
public:
    wxHyperlinkEvent() {}
    wxHyperlinkEvent(wxObject *generator, wxWindowID id, const wxString& url)
        : wxCommandEvent(wxEVT_COMMAND_HYPERLINK, id),
          m_url(url)
    {
        SetEventObject(generator);
    }

    // Returns the URL associated with the hyperlink control
    // that the user clicked on.
    wxString GetURL() const { return m_url; }
    void SetURL(const wxString &url) { m_url=url; }

    // default copy ctor, assignment operator and dtor are ok
    virtual wxEvent *Clone() const { return new wxHyperlinkEvent(*this); }

private:

    // URL associated with the hyperlink control that the used clicked on.
    wxString m_url;

    DECLARE_DYNAMIC_CLASS_NO_ASSIGN(wxHyperlinkEvent)
};


// ----------------------------------------------------------------------------
// event types and macros
// ----------------------------------------------------------------------------

typedef void (wxEvtHandler::*wxHyperlinkEventFunction)(wxHyperlinkEvent&);

#define wxHyperlinkEventHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(wxHyperlinkEventFunction, &func)

#define EVT_HYPERLINK(id, fn) \
    wx__DECLARE_EVT1(wxEVT_COMMAND_HYPERLINK, id, wxHyperlinkEventHandler(fn))

#ifdef _WX_DEFINE_DATE_EVENTS_
    DEFINE_EVENT_TYPE(wxEVT_COMMAND_HYPERLINK)

    IMPLEMENT_DYNAMIC_CLASS(wxHyperlinkEvent, wxCommandEvent)
#endif



#if defined(__WXGTK210__) && !defined(__WXUNIVERSAL__)
    #include "wx/gtk/hyperlink.h"
#else
    #include "wx/generic/hyperlink.h"
    #define wxHyperlinkCtrl     wxGenericHyperlinkCtrl
#endif


#endif // wxUSE_HYPERLINKCTRL

#endif // _WX_HYPERLINK_H_
