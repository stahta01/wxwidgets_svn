///////////////////////////////////////////////////////////////////////////////
// Name:        wx/unix/apptrait.h
// Purpose:     standard implementations of wxAppTraits for Unix
// Author:      Vadim Zeitlin
// Modified by:
// Created:     23.06.2003
// RCS-ID:      $Id$
// Copyright:   (c) 2003 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_UNIX_APPTRAIT_H_
#define _WX_UNIX_APPTRAIT_H_

// ----------------------------------------------------------------------------
// wxGUI/ConsoleAppTraits: must derive from wxAppTraits, not wxAppTraitsBase
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxConsoleAppTraits : public wxConsoleAppTraitsBase
{
public:
#if wxUSE_CONSOLE_EVENTLOOP
    virtual wxEventLoopBase *CreateEventLoop();
#endif // wxUSE_CONSOLE_EVENTLOOP
    virtual bool CreateEndProcessPipe(wxExecuteData& execData);
    virtual bool IsWriteFDOfEndProcessPipe(wxExecuteData& execData, int fd);
    virtual void DetachWriteFDOfEndProcessPipe(wxExecuteData& execData);
    virtual int WaitForChild(wxExecuteData& execData);
#if wxUSE_TIMER
    virtual wxTimerImpl *CreateTimerImpl(wxTimer *timer);
#endif
};

#if wxUSE_GUI

class WXDLLEXPORT wxGUIAppTraits : public wxGUIAppTraitsBase
{
public:
    virtual wxEventLoopBase *CreateEventLoop();
    virtual bool CreateEndProcessPipe(wxExecuteData& execData);
    virtual bool IsWriteFDOfEndProcessPipe(wxExecuteData& execData, int fd);
    virtual void DetachWriteFDOfEndProcessPipe(wxExecuteData& execData);
    virtual int WaitForChild(wxExecuteData& execData);
#if wxUSE_TIMER
    virtual wxTimerImpl *CreateTimerImpl(wxTimer *timer);
#endif

#if (defined(__WXMAC__) || defined(__WXCOCOA__)) && wxUSE_STDPATHS
    virtual wxStandardPathsBase& GetStandardPaths();
#endif
    virtual wxPortId GetToolkitVersion(int *majVer, int *minVer) const;

#if defined(__WXGTK__) && wxUSE_INTL
    virtual void SetLocale();
#endif // __WXGTK__

#ifdef __WXGTK20__
    virtual wxString GetDesktopEnvironment() const;
    virtual wxString GetStandardCmdLineOptions(wxArrayString& names,
                                               wxArrayString& desc) const;
#endif // __WXGTK20____

#if defined(__WXDEBUG__) && defined(__WXGTK20__)
    virtual bool ShowAssertDialog(const wxString& msg);
#endif
};

#endif // wxUSE_GUI

#endif // _WX_UNIX_APPTRAIT_H_

