/////////////////////////////////////////////////////////////////////////////
// Name:        wx/fswatcher.h
// Purpose:     wxFileSystemWatcherBase
// Author:      Bartosz Bekier
// Created:     2009-05-23
// RCS-ID:      $Id$
// Copyright:   (c) 2009 Bartosz Bekier <bartosz.bekier@gmail.com>
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_FSWATCHER_BASE_H_
#define _WX_FSWATCHER_BASE_H_

#include "wx/defs.h"

#if wxUSE_FSWATCHER

#include "wx/event.h"
#include "wx/evtloop.h"
#include "wx/filename.h"
#include "wx/dir.h"
#include "wx/hashmap.h"

// ----------------------------------------------------------------------------
// wxFileSystemWatcherEventType & wxFileSystemWatcherEvent
// ----------------------------------------------------------------------------

#define wxTRACE_FSWATCHER "fswatcher"

/**
 * Possible types of file system events.
 * This is a subset that will work fine an all platforms (actually, we will
 * see how it works on Mac).
 *
 * We got 2 types of error events:
 * - warning: these are not fatal and further events can still be generated
 * - error: indicates fatal error and causes that no more events will happen
 */
enum
{
    wxFSW_EVENT_CREATE = 0x01,
    wxFSW_EVENT_DELETE = 0x02,
    wxFSW_EVENT_RENAME = 0x04,
    wxFSW_EVENT_MODIFY = 0x08,
    wxFSW_EVENT_ACCESS = 0x10,

    // error events
    wxFSW_EVENT_WARNING = 0x20,
    wxFSW_EVENT_ERROR = 0x40,

    wxFSW_EVENT_ALL = wxFSW_EVENT_CREATE | wxFSW_EVENT_DELETE |
                         wxFSW_EVENT_RENAME | wxFSW_EVENT_MODIFY |
                         wxFSW_EVENT_ACCESS |
                         wxFSW_EVENT_WARNING | wxFSW_EVENT_ERROR
};

/**
 * Event containing information about file system change.
 */
class WXDLLIMPEXP_FWD_BASE wxFileSystemWatcherEvent;
wxDECLARE_EXPORTED_EVENT(WXDLLIMPEXP_BASE, wxEVT_FSWATCHER,
                         wxFileSystemWatcherEvent);

class WXDLLIMPEXP_BASE wxFileSystemWatcherEvent: public wxEvent
{
public:
    wxFileSystemWatcherEvent(int changeType, int watchid = wxID_ANY) :
        wxEvent(watchid, wxEVT_FSWATCHER),
        m_changeType(changeType)
    {
    }

    wxFileSystemWatcherEvent(int changeType, const wxString& errorMsg,
							 int watchid = wxID_ANY) :
		wxEvent(watchid, wxEVT_FSWATCHER),
		m_changeType(changeType), m_errorMsg(errorMsg)
    {
    }

    wxFileSystemWatcherEvent(int changeType,
                             const wxFileName& path, const wxFileName& newPath,
                             int watchid = wxID_ANY) :
         wxEvent(watchid, wxEVT_FSWATCHER),
         m_changeType(changeType), m_path(path), m_newPath(newPath)
    {
    }

    /**
     * Returns the path at which the event occurred.
     */
    const wxFileName& GetPath() const
    {
        return m_path;
    }

    /**
     * Sets the path at which the event occurred
     */
    void SetPath(const wxFileName& path)
    {
        m_path = path;
    }

    /**
     * In case of rename(move?) events, returns the new path related to the
     * event. The "new" means newer in the sense of time. In case of other
     * events it returns the same path as GetPath().
     */
    const wxFileName& GetNewPath() const
    {
        return m_newPath;
    }

    /**
     * Sets the new path related to the event. See above.
     */
    void SetNewPath(const wxFileName& path)
    {
        m_newPath = path;
    }

    /**
     * Returns the type of file system event that occurred.
     */
    int GetChangeType() const
    {
        return m_changeType;
    }

    virtual wxEvent* Clone() const
    {
        return new wxFileSystemWatcherEvent(*this);
    }

    virtual wxEventCategory GetEventCategory() const
    {
        // TODO this has to be merged with "similiar" categories and changed
        return wxEVT_CATEGORY_UNKNOWN;
    }

    /**
     * Returns if this error is an error event
     */
    bool IsError() const
    {
        return m_changeType & (wxFSW_EVENT_ERROR | wxFSW_EVENT_WARNING);
    }

    wxString GetErrorDescription() const
    {
    	return m_errorMsg;
    }

    /**
     * Returns a wxString describing an event useful for debugging or testing
     */
    wxString ToString() const;

protected:
    int m_changeType;
    wxFileName m_path;
    wxFileName m_newPath;
    wxString m_errorMsg;
};

typedef void (wxEvtHandler::*wxFileSystemWatcherEventFunction)
												(wxFileSystemWatcherEvent&);

#define wxFileSystemWatcherEventHandler(func) \
    wxEVENT_HANDLER_CAST(wxFileSystemWatcherEventFunction, func)


// ----------------------------------------------------------------------------
// wxFileSystemWatcherBase: interface for wxFileSystemWatcher
// ----------------------------------------------------------------------------

/**
 * Simple container to store information about one watched file
 */
class wxFSWatchEntry;

WX_DECLARE_STRING_HASH_MAP(wxFSWatchEntry*, wxFSWatchEntries);

/**
 * Encapsulation of platform-specific file system event mechanism
 */
class wxFSWatcherService;

/**
 * Main entry point for clients interested in file system events.
 * Defines interface that can be used to receive that kind of events.
 */
class WXDLLIMPEXP_BASE wxFileSystemWatcherBase: public wxEvtHandler
{
public:
    wxFileSystemWatcherBase();

    virtual ~wxFileSystemWatcherBase();

    /**
     * Adds path to currently watched files. Any events concerning this
     * particular path will be sent to handler. Optionally a filter can be
     * specified to receive only events of particular type.
     *
     * Please note that when adding a dir, immediate children will be watched
     * as well.
     */
    virtual bool Add(const wxFileName& path, int events = wxFSW_EVENT_ALL);

    /**
     * Like above, but recursively adds every file/dir in the tree rooted in
     * path. Additionally a file mask can be specified to include only files
     * of particular type.
     */
    virtual bool AddTree(const wxFileName& path, int events = wxFSW_EVENT_ALL,
                        const wxString& filter = wxEmptyString) = 0;

    /**
     * Removes path from the list of watched paths.
     */
    virtual bool Remove(const wxFileName& path);

    /**
     * Same as above, but also removes every file belonging to the tree rooted
     * at path.
     */
    virtual bool RemoveTree(const wxFileName& path);

    /**
     * Stops watching all paths.
     */
    virtual bool RemoveAll();

    void SetOwner(wxEvtHandler* handler)
    {
        if (!handler)
            m_owner = this;
        else
            m_owner = handler;
    }

protected:

    /**
     * Event handlers. Override these if you are deriving file system watcher
     */
    virtual void OnChange(int changeType, const wxFileName& path,
                                        const wxFileName& newPath)
    {
        wxFileSystemWatcherEvent evt(changeType, path, newPath);
        m_owner->ProcessEvent(evt);
    }

    virtual void OnWarning(const wxString& errorMessage)
    {
        wxLogTrace(wxTRACE_FSWATCHER,
                    wxString::Format("Warning received: '%s'", errorMessage));
    }

    virtual void OnError(const wxString& errorMessage)
    {
        wxLogTrace(wxTRACE_FSWATCHER,
                    wxString::Format("Error received: '%s'", errorMessage));
    }

    /**
     * Function resposible for creating platform specific wxFSWatchEntry. This is
     * supposed to be simple factory function so it should never fail (i.e.
     * never return a NULL pointer)
     */
    virtual wxFSWatchEntry* CreateWatch(const wxFileName& path, int events) = 0;

    /**
     * Actual adding of wxFSWatchEntry to be watched for file system events
     */
    virtual bool DoAdd(const wxFSWatchEntry& watch) = 0;

    /**
	 * Actual removing of wxFSWatchEntry from list of watched paths
	 */
    virtual bool DoRemove(const wxFSWatchEntry& watch) = 0;

    wxFSWatchEntries m_watches;        // path=>wxFSWatchEntry* map
    wxFSWatcherService* m_service;     // file system events service
    wxEvtHandler* m_owner;             // handler for file system events

    friend class wxFSWatcherService;
};

// include the platform specific file defining wxFileSystemWatcher
// inheriting from wxFileSystemWatcherBase

#if HAVE_INOTIFY_H
    #include "wx/unix/fswatcher.h"
    #define wxFileSystemWatcher wxInotifyFileSystemWatcher
#elif defined(__WXMSW__)
    #include "wx/msw/fswatcher.h"
    #define wxFileSystemWatcher wxMswFileSystemWatcher
#elif defined(__WXCOCOA__)
    #include "wx/msw/fswatcher.h"
    #define wxFileSystemWatcher wxKqueueFileSystemWatcher
#else
    #include "wx/generic/fswatcher.h"
    #define wxFileSystemWatcher wxPollingFileSystemWatcher
#endif

#endif // wxUSE_FSWATCHER

#endif /* _WX_FSWATCHER_BASE_H_ */
