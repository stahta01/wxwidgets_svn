///////////////////////////////////////////////////////////////////////////////
// Name:        wx/popupwin.h
// Purpose:     wxPopupWindow interface declaration
// Author:      Vadim Zeitlin
// Modified by:
// Created:     06.01.01
// RCS-ID:      $Id$
// Copyright:   (c) 2001 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// License:     wxWindows license
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_POPUPWIN_H_BASE_
#define _WX_POPUPWIN_H_BASE_

#ifdef __GNUG__
    #pragma interface "popupwinbase.h"
#endif

#include "wx/window.h"

#if wxUSE_POPUPWIN

// ----------------------------------------------------------------------------
// wxPopupWindow: a special kind of top level window used for popup menus,
// combobox popups and such.
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxPopupWindowBase : public wxWindow
{
public:
    wxPopupWindowBase() { }

    // create the popup window
    //
    // style may only contain border flags
    bool Create(wxWindow *parent, int style = wxBORDER_NONE);

    // move the popup window to the right position, i.e. such that it is
    // entirely visible
    //
    // the popup is positioned at ptOrigin + size if it opens below and to the
    // right (default), at ptOrigin - sizePopup if it opens above and to the
    // left &c
    //
    // the point must be given in screen coordinates!
    virtual void Position(const wxPoint& ptOrigin,
                          const wxSize& size);
};

// include the real class declaration
#ifdef __WXMSW__
    #include "wx/msw/popupwin.h"
#elif __WXGTK__
    #include "wx/gtk/popupwin.h"
#else
    #error "wxPopupWindow is not supported under this platform."
#endif

// ----------------------------------------------------------------------------
// wxPopupTransientWindow: a wxPopupWindow which disappears automatically
// when the user clicks mouse outside it or if it loses focus in any other way
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxPopupTransientWindow : public wxPopupWindow
{
public:
    // ctors
    wxPopupTransientWindow() { Init(); }
    wxPopupTransientWindow(wxWindow *parent);

    virtual ~wxPopupTransientWindow();

    // popup the window (this will show it too) and keep focus at winFocus
    // (or itself if it's NULL), dismiss the popup if we lose focus
    virtual void Popup(wxWindow *focus = NULL);

    // hide the window
    virtual void Dismiss();

    // called when a mouse is pressed while the popup is shown: return TRUE
    // from here to prevent its normal processing by the popup (which consists
    // in dismissing it if the mouse is cilcked outside it)
    virtual bool ProcessLeftDown(wxMouseEvent& event);

protected:
    // common part of all ctors
    void Init();

    // this is called when the popup is disappeared because of anything
    // else but direct call to Dismiss()
    virtual void OnDismiss();

    // dismiss and notify the derived class
    void DismissAndNotify();

    // remove our event handlers
    void PopHandlers();

    // the child of this popup if any
    wxWindow *m_child;

    // the window which has the focus while we're shown
    wxWindow *m_focus;

    friend class wxPopupWindowHandler;
    friend class wxPopupFocusHandler;
};

#if wxUSE_COMBOBOX && defined(__WXUNIVERSAL__)

// ----------------------------------------------------------------------------
// wxPopupComboWindow: wxPopupTransientWindow used by wxComboBox
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxComboBox;
class WXDLLEXPORT wxComboControl;

class WXDLLEXPORT wxPopupComboWindow : public wxPopupTransientWindow
{
public:
    wxPopupComboWindow(wxComboControl *parent);

    bool Create(wxComboControl *parent);

    // position the window correctly relatively to the combo
    void PositionNearCombo();

protected:
    // notify the combo here
    virtual void OnDismiss();

    // the parent combobox
    wxComboControl *m_combo;
};

#endif // wxUSE_COMBOBOX && defined(__WXUNIVERSAL__)

#endif // wxUSE_POPUPWIN

#endif // _WX_POPUPWIN_H_BASE_

