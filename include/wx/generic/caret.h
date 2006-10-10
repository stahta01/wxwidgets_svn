///////////////////////////////////////////////////////////////////////////////
// Name:        generic/caret.h
// Purpose:     generic wxCaret class
// Author:      Vadim Zeitlin (original code by Robert Roebling)
// Modified by:
// Created:     25.05.99
// RCS-ID:      $Id$
// Copyright:   (c) wxWidgets team
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_CARET_H_
#define _WX_CARET_H_

#include "wx/timer.h"

class WXDLLIMPEXP_CORE wxCaret;

class WXDLLEXPORT wxCaretTimer : public wxTimer
{
public:
    wxCaretTimer(wxCaret *caret);
    virtual void Notify();

private:
    wxCaret *m_caret;
};

#ifndef wxUSE_OVERLAY
    #if defined(wxMAC_USE_CORE_GRAPHICS) && wxMAC_USE_CORE_GRAPHICS
        #define wxUSE_OVERLAY 1
    #else
        #define wxUSE_OVERLAY 0
    #endif
#endif

#if wxUSE_OVERLAY
    #include "wx/dc.h"
#endif

class WXDLLIMPEXP_CORE wxCaret : public wxCaretBase
{
public:
    // ctors
    // -----
        // default - use Create()
    wxCaret() : m_timer(this) { InitGeneric(); }
        // creates a block caret associated with the given window
    wxCaret(wxWindowBase *window, int width, int height)
        : wxCaretBase(window, width, height), m_timer(this) { InitGeneric(); }
    wxCaret(wxWindowBase *window, const wxSize& size)
        : wxCaretBase(window, size), m_timer(this) { InitGeneric(); }

    virtual ~wxCaret();

    // implementation
    // --------------

    // called by wxWindow (not using the event tables)
    virtual void OnSetFocus();
    virtual void OnKillFocus();

    // called by wxCaretTimer
    void OnTimer();

protected:
    virtual void DoShow();
    virtual void DoHide();
    virtual void DoMove();
    virtual void DoSize();

    // blink the caret once
    void Blink();

    // refresh the caret
    void Refresh();

    // draw the caret on the given DC
    void DoDraw(wxDC *dc);

private:
    // GTK specific initialization
    void InitGeneric();

#if wxUSE_OVERLAY
    // the overlay for displaying the caret
    wxOverlay   m_overlay;
#else
    // the bitmap holding the part of window hidden by the caret when it was
    // at (m_xOld, m_yOld)
    wxBitmap      m_bmpUnderCaret;
    int           m_xOld,
                  m_yOld;
#endif

    wxCaretTimer  m_timer;
    bool          m_blinkedOut,     // true => caret hidden right now
                  m_hasFocus;       // true => our window has focus
};

#endif // _WX_CARET_H_
