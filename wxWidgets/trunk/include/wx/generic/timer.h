/////////////////////////////////////////////////////////////////////////////
// Name:        timer.h
// Purpose:     Generic implementation of wxTimer class
// Author:      Vaclav Slavik
// Id:          $Id$
// Copyright:   (c) Vaclav Slavik
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////


#ifndef __WX_TIMER_H__
#define __WX_TIMER_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "timer.h"
#endif

//-----------------------------------------------------------------------------
// wxTimer
//-----------------------------------------------------------------------------

class wxTimerDesc;

class WXDLLEXPORT wxTimer : public wxTimerBase
{
public:
    wxTimer() { Init(); }
    wxTimer(wxEvtHandler *owner, int timerid = -1) : wxTimerBase(owner, timerid)
        { Init(); }
    ~wxTimer();

    virtual bool Start(int millisecs = -1, bool oneShot = FALSE);
    virtual void Stop();

    virtual bool IsRunning() const;

    // implementation
    static void NotifyTimers();

protected:
    void Init();

private:
    wxTimerDesc *m_desc;
    
    DECLARE_ABSTRACT_CLASS(wxTimer)
};

#endif // __WX_TIMER_H__
