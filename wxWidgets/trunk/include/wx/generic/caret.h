///////////////////////////////////////////////////////////////////////////////
// Name:        generic/caret.h
// Purpose:     generic wxCaret class
// Author:      Vadim Zeitlin (original code by Robert Roebling)
// Modified by:
// Created:     25.05.99
// RCS-ID:      $Id$
// Copyright:   (c) wxWindows team
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#if 0 //def __GNUG__
    #pragma implementation "caret.h"
#endif

#ifndef _WX_CARET_H_
#define _WX_CARET_H_

#include "wx/timer.h"

class wxCaret : public wxCaretBase
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

    // blink the caret once
    void Blink();

protected:
    virtual void DoShow();
    virtual void DoHide();
    virtual void DoMove();

    // draw the caret on the given DC
    void DoDraw(wxDC *dc);

private:
    // GTK specific initialization
    void InitGeneric();

    class CaretTimer : public wxTimer
    {
    public:
        CaretTimer(wxCaret *caret) { m_caret = caret; }

        virtual void Notify() { m_caret->Blink(); }

    private:
        wxCaret *m_caret;
    } m_timer;

    bool    m_blinkedOut;   // TRUE => caret hidden right now
};

#endif // _WX_CARET_H_
