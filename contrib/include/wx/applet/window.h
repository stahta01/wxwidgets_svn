/****************************************************************************
*
*                       wxWindows HTML Applet Package
*
*               Copyright (C) 1991-2001 SciTech Software, Inc.
*                            All rights reserved.
*
*  ========================================================================
*
*    The contents of this file are subject to the wxWindows License
*    Version 3.0 (the "License"); you may not use this file except in
*    compliance with the License. You may obtain a copy of the License at
*    http://www.wxwindows.org/licence3.txt
*
*    Software distributed under the License is distributed on an
*    "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
*    implied. See the License for the specific language governing
*    rights and limitations under the License.
*
*  ========================================================================
*
* Language:     ANSI C++
* Environment:  Any
*
* Description:  Header file for the wxHtmlAppletWindow class
*
****************************************************************************/

#ifndef __WX_APPLET_WINDOW_H
#define __WX_APPLET_WINDOW_H

#include "wx/html/htmlwin.h"

// Forward declare
class wxApplet;
class wxLoadPageEvent;
class wxPageLoadedEvent;
class wxIncludePrep;

// Declare a linked list of wxApplet pointers
WX_DECLARE_LIST(wxApplet, wxAppletList);

/*--------------------------- Class Definitions ---------------------------*/

/****************************************************************************
REMARKS:
Defines the class for virtual-link data types
****************************************************************************/
class VirtualData : public wxObject {
private:
    wxString m_name;
    wxString m_group;
    wxString m_href;

public:
            // Ctors
            VirtualData(
                wxString& name,
                wxString& group,
                wxString& href );

            // Gets
            wxString GetName(){ return m_name;};
            wxString GetGroup(){ return m_group;};
            wxString GetHref(){ return m_href;};

            // Sets
            void SetName (wxString& s){ m_name = s; };
            void SetGroup(wxString& s){ m_group = s; };
            void SetHref (wxString& s){ m_href = s; };
    };

/****************************************************************************
REMARKS:
Defines the class for wxAppletWindow. This class is derived from the
wxHtmlWindow class and extends it with functionality to handle embedded
wxApplet's on the HTML pages.
****************************************************************************/
class wxHtmlAppletWindow : public wxHtmlWindow {
private:
    DECLARE_CLASS(wxHtmlAppletWindow);
    DECLARE_EVENT_TABLE();

    bool m_mutexLock;
    wxIncludePrep *incPreprocessor;  // deleted by list it is added too in constructor
protected:
	wxAppletList		m_AppletList;		
	static wxHashTable	m_Cookies;
    wxToolBarBase       *m_NavBar;
    int                 m_NavBackId;
    int                 m_NavForwardId;
		
public:
            // Constructor
            wxHtmlAppletWindow(
                wxWindow *parent,
                wxWindowID id = -1,
                wxToolBarBase *navBar = NULL,
                int navBackId = -1,
                int navForwardId = -1,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxHW_SCROLLBAR_AUTO,
                const wxString& name = "htmlAppletWindow");

            // Destructor
            ~wxHtmlAppletWindow();

            // Create an instance of an applet based on it's class name
            wxApplet *CreateApplet(
                const wxString& classId,
                const wxString& iName,
                const wxHtmlTag &params,
                const wxSize& size);

            // Find an instance of an applet based on it's class name
            wxApplet *FindApplet(const wxString& className);

            // Remove an applet from the window. Called during applet destruction
            bool RemoveApplet(const wxApplet *applet);

            // Load a new URL page
    virtual bool LoadPage(const wxString& location);

            // Called when users clicked on hypertext link.
    virtual void OnLinkClicked(const wxHtmlLinkInfo& link);

            // Handles forward navigation within the HTML stack
            bool HistoryForward();

            // Handles backwards navigation within the HTML stack
            bool HistoryBack();

            // Broadcast a message to all applets on the page
            void SendMessage(wxEvent& msg);

            // Register a cookie of data in the applet manager
            bool RegisterCookie(const wxString& name,wxObject *cookie);

            // UnRegister a cookie of data in the applet manager
            bool UnRegisterCookie(const wxString& name);

            // Find a cookie of data given it's public name
            wxObject *FindCookie(const wxString& name);
			
            // Event handlers to load a new page	
			void OnLoadPage(wxLoadPageEvent &event);

            // Event handlers to load a new page	
    		void OnPageLoaded(wxPageLoadedEvent &event);

            // LoadPage mutex locks
            void Lock() { m_mutexLock = true;};
            void UnLock() { m_mutexLock = false;};

            // Returns TRUE if the mutex is locked, FALSE otherwise.
            bool IsLocked() { return m_mutexLock;};

            // Tries to lock the mutex. If it can't, returns immediately with false.
            bool TryLock();

    };

#endif // __WX_APPLET_WINDOW_H

