/////////////////////////////////////////////////////////////////////////////
// Name:        webkit.h
// Purpose:     wxWebKitCtrl - embeddable web kit control
// Author:      Jethro Grassie / Kevin Ollivier
// Modified by:
// Created:     2004-4-16
// RCS-ID:      $Id$
// Copyright:   (c) Jethro Grassie / Kevin Ollivier
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_WEBKIT_H
#define _WX_WEBKIT_H

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "webkit.h"
#endif

#if wxUSE_WEBKIT

#if !defined(__WXMAC__) && !defined(__WXCOCOA__)
#error "wxWebKitCtrl not implemented for this platform"
#endif

#ifdef __WXCOCOA
#include <WebKit/WebKit.h>
#endif
#include "wx/control.h"

// ----------------------------------------------------------------------------
// Web Kit Control
// ----------------------------------------------------------------------------

class wxWebKitCtrl : public wxControl
{
public:
    DECLARE_DYNAMIC_CLASS(wxWebKitCtrl)

    wxWebKitCtrl() {};
    wxWebKitCtrl(wxWindow *parent,
                    wxWindowID winID,
                    const wxString& strURL,
                    const wxPoint& pos = wxDefaultPosition,
                    const wxSize& size = wxDefaultSize, long style = 0,
                    const wxValidator& validator = wxDefaultValidator,
                    const wxString& name = wxT("webkitctrl"))
    {
        Create(parent, winID, strURL, pos, size, style, validator, name);
    };
    bool Create(wxWindow *parent,
                wxWindowID winID,
                const wxString& strURL,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize, long style = 0,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = wxT("webkitctrl"));
    virtual ~wxWebKitCtrl();
    
    void LoadURL(const wxString &url);
    
    bool CanGoBack();
    bool CanGoForward();
    bool GoBack();
    bool GoForward();
    void Reload(); 
    void Stop();
    bool CanGetPageSource();
    wxString GetPageSource();
    void SetPageSource(wxString& source, const wxString& baseUrl = wxEmptyString);
    
    //we need to resize the webview when the control size changes
    void OnSize(wxSizeEvent &event);
protected:
    DECLARE_EVENT_TABLE()
    void MacVisibilityChanged();

private:
    wxWindow *m_parent;
    wxWindowID m_windowID;
    wxString m_currentURL;
    wxString m_pageTitle;
    void* m_webView;
    //It should be WebView, but WebView is Cocoa only, so any class which included
    //this header would have to link to Cocoa, so for now use void* instead.
};

// ----------------------------------------------------------------------------
// Web Kit Events
// ----------------------------------------------------------------------------

enum {
    wxWEBKIT_STATE_START = 1,
    wxWEBKIT_STATE_NEGOTIATING = 2,
    wxWEBKIT_STATE_REDIRECTING = 4,
    wxWEBKIT_STATE_TRANSFERRING = 8,
    wxWEBKIT_STATE_STOP = 16,
        wxWEBKIT_STATE_FAILED = 32
};

class wxWebKitStateChangedEvent : public wxCommandEvent
{
    DECLARE_DYNAMIC_CLASS( wxWebKitStateChangedEvent )

public:
    int GetState() { return m_state; }
    void SetState(const int state) { m_state = state; }
    wxString GetURL() { return m_url; }
    void SetURL(const wxString& url) { m_url = url; }

    wxWebKitStateChangedEvent( wxWindow* win = (wxWindow*) NULL );
    wxEvent *Clone(void) const { return new wxWebKitStateChangedEvent(*this); }

protected:
    int m_state;
    wxString m_url;
};

typedef void (wxEvtHandler::*wxWebKitStateChangedEventFunction)(wxWebKitStateChangedEvent&);

BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_LOCAL_EVENT_TYPE(wxEVT_WEBKIT_STATE_CHANGED, -1)
END_DECLARE_EVENT_TYPES()

#define EVT_WEBKIT_STATE_CHANGED(func) \
            DECLARE_EVENT_TABLE_ENTRY( wxEVT_WEBKIT_STATE_CHANGED, \
                            -1,                       \
                            -1,                       \
                            (wxObjectEventFunction)   \
                            (wxWebKitStateChangedEventFunction) & func, \
                            (wxObject *) NULL ),

#endif // wxUSE_WEBKIT

#endif // _WX_WEBKIT_H_
