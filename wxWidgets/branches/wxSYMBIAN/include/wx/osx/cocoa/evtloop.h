///////////////////////////////////////////////////////////////////////////////
// Name:        wx/osx/cocoa/evtloop.h
// Purpose:     declaration of wxGUIEventLoop for wxOSX/Cocoa
// Author:      Vadim Zeitlin
// Created:     2008-12-28
// RCS-ID:      $Id$
// Copyright:   (c) 2006 Vadim Zeitlin <vadim@wxwindows.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_OSX_COCOA_EVTLOOP_H_
#define _WX_OSX_COCOA_EVTLOOP_H_

class WXDLLIMPEXP_BASE wxGUIEventLoop : public wxCFEventLoop
{
public:
    wxGUIEventLoop();

    // implement/override base class pure virtual
    virtual bool Pending() const;
    virtual bool Dispatch();
    virtual int DispatchTimeout(unsigned long timeout);

    virtual void WakeUp();
    virtual bool YieldFor(long eventsToProcess);

protected:
    virtual CFRunLoopRef CFGetCurrentRunLoop() const;

private:
    double m_sleepTime;
};

#endif // _WX_OSX_COCOA_EVTLOOP_H_

