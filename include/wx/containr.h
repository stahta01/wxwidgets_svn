///////////////////////////////////////////////////////////////////////////////
// Name:        wx/containr.h
// Purpose:     wxControlContainer class declration: a "mix-in" class which
//              implements the TAB navigation between the controls
// Author:      Vadim Zeitlin
// Modified by:
// Created:     06.08.01
// RCS-ID:      $Id$
// Copyright:   (c) 2001 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_CONTAINR_H_
#define _WX_CONTAINR_H_

#ifdef __GNUG__
    #pragma interface "containr.h"
#endif

class WXDLLEXPORT wxFocusEvent;
class WXDLLEXPORT wxNavigationKeyEvent;
class WXDLLEXPORT wxWindow;
class WXDLLEXPORT wxWindowBase;

/*
   Implementation note: wxControlContainer is not a real mix-in but rather
   a class meant to be agregated with (and not inherited from). Although
   logically it should be a mix-in, doing it like this has no advantage from
   the point of view of the existing code but does have some problems (we'd
   need to play tricks with event handlers which may be difficult to do
   safely). The price we pay for this simplicity is the ugly macros below.
 */

// ----------------------------------------------------------------------------
// wxControlContainer
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxControlContainer
{
public:
    // ctors and such
    wxControlContainer(wxWindow *winParent);

    wxWindow *GetDefaultItem() const { return m_winDefault; }
    wxWindow *SetDefaultItem(wxWindow *win)
        { wxWindow *winOld = m_winDefault; m_winDefault = win; return winOld; }

    void SetLastFocus(wxWindow *win);

    // the methods to be called from the window event handlers
    void HandleOnNavigationKey(wxNavigationKeyEvent& event);
    void HandleOnFocus(wxFocusEvent& event);
    void HandleOnWindowDestroy(wxWindowBase *child);

    // should be called from SetFocus()
    void DoSetFocus();

protected:
    // set the focus to the child which had it the last time
    bool SetFocusToChild();

    // the parent window we manage the children for
    wxWindow *m_winParent;

    // the child which had the focus last time this panel was activated
    wxWindow *m_winLastFocused;

    // a default window (e.g. a button) or NULL
    wxWindow *m_winDefault;
};

// this function is for wxWindows internal use only
extern bool wxSetFocusToChild(wxWindow *win, wxWindow **child);

// ----------------------------------------------------------------------------
// macros which may be used by the classes wishing to implement TAB navigation
// among their children
// ----------------------------------------------------------------------------

// declare the methods to be forwarded
#define WX_DECLARE_CONTROL_CONTAINER() \
    void OnNavigationKey(wxNavigationKeyEvent& event); \
    void OnFocus(wxFocusEvent& event); \
    virtual void OnChildFocus(wxChildFocusEvent& event); \
    virtual void SetFocus(); \
    virtual void RemoveChild(wxWindowBase *child); \
    virtual wxWindow *GetDefaultItem() const; \
    virtual wxWindow *SetDefaultItem(wxWindow *child) \

// implement the event table entries for wxControlContainer
#define WX_EVENT_TABLE_CONTROL_CONTAINER(classname) \
    EVT_SET_FOCUS(classname::OnFocus) \
    EVT_CHILD_FOCUS(classname::OnChildFocus) \
    EVT_NAVIGATION_KEY(classname::OnNavigationKey)

// implement the methods forwarding to the wxControlContainer
#define WX_DELEGATE_TO_CONTROL_CONTAINER(classname, container)  \
wxWindow *classname::SetDefaultItem(wxWindow *child) \
{ \
    return container->SetDefaultItem(child); \
} \
 \
wxWindow *classname::GetDefaultItem() const \
{ \
    return container->GetDefaultItem(); \
} \
 \
void classname::OnNavigationKey( wxNavigationKeyEvent& event ) \
{ \
    container->HandleOnNavigationKey(event); \
} \
 \
void classname::RemoveChild(wxWindowBase *child) \
{ \
    container->HandleOnWindowDestroy(child); \
 \
    wxWindow::RemoveChild(child); \
} \
 \
void classname::SetFocus() \
{ \
    container->DoSetFocus(); \
} \
 \
void classname::OnChildFocus(wxChildFocusEvent& event) \
{ \
    container->SetLastFocus(event.GetWindow()); \
} \
 \
void classname::OnFocus(wxFocusEvent& event) \
{ \
    container->HandleOnFocus(event); \
}


#endif // _WX_CONTAINR_H_
