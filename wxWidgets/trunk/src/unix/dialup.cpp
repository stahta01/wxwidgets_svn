// -*- c++ -*- ///////////////////////////////////////////////////////////////
// Name:        unix/dialup.cpp
// Purpose:     Network related wxWindows classes and functions
// Author:      Karsten Ball�der
// Modified by:
// Created:     03.10.99
// RCS-ID:      $Id$
// Copyright:   (c) Karsten Ball�der
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/setup.h"

#ifdef __GNUG__
#   pragma implementation "dialup.h"
#endif

#if wxUSE_DIALUP_MANAGER

#ifndef  WX_PRECOMP
#   include "wx/defs.h"
#endif // !PCH

#include "wx/string.h"
#include "wx/event.h"
#include "wx/dialup.h"
#include "wx/timer.h"
#include "wx/filefn.h"
#include "wx/utils.h"
#include "wx/log.h"
#include "wx/file.h"
#include "wx/process.h"
#include "wx/intl.h"
#include "wx/app.h"
#include "wx/wxchar.h"

#include <stdlib.h>

#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#define __STRICT_ANSI__
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

// ----------------------------------------------------------------------------
// A class which groups functions dealing with connecting to the network from a
// workstation using dial-up access to the net. There is at most one instance
// of this class in the program accessed via GetDialUpManager().
// ----------------------------------------------------------------------------

/* TODO
 *
 * 1. more configurability for Unix: i.e. how to initiate the connection, how
 *    to check for online status, &c.
 * 2. add a "long Dial(long connectionId = -1)" function which asks the user
 *    about which connection to dial (this may be done using native dialogs
 *    under NT, need generic dialogs for all others) and returns the identifier
 *    of the selected connection (it's opaque to the application) - it may be
 *    reused later to dial the same connection later (or use strings instead of
 *    longs may be?)
 * 3. add an async version of dialing functions which notify the caller about
 *    the progress (or may be even start another thread to monitor it)
 * 4. the static creation/accessor functions are not MT-safe - but is this
 *    really crucial? I think we may suppose they're always called from the
 *    main thread?
 */

class WXDLLEXPORT wxDialUpManagerImpl : public wxDialUpManager
{
public:
   wxDialUpManagerImpl();
   ~wxDialUpManagerImpl();

   /** Could the dialup manager be initialized correctly? If this function
       returns FALSE, no other functions will work neither, so it's a good idea
       to call this function and check its result before calling any other
       wxDialUpManager methods.
   */
   virtual bool IsOk() const
      { return TRUE; }

   /** The simplest way to initiate a dial up: this function dials the given
       ISP (exact meaning of the parameter depends on the platform), returns
       TRUE on success or FALSE on failure and logs the appropriate error
       message in the latter case.
       @param nameOfISP optional paramater for dial program
       @param username unused
       @param password unused
   */
   virtual bool Dial(const wxString& nameOfISP,
                     const wxString& WXUNUSED(username),
                     const wxString& WXUNUSED(password),
                     bool async);

   /// Hang up the currently active dial up connection.
   virtual bool HangUp();

   // returns TRUE if the computer is connected to the network: under Windows,
   // this just means that a RAS connection exists, under Unix we check that
   // the "well-known host" (as specified by SetWellKnownHost) is reachable
   virtual bool IsOnline() const
      {
         if( (! m_timer) // we are not polling, so test now:
             || m_IsOnline < 0
            )
            CheckStatus();
         return m_IsOnline != 0;
      }

   /// do we have a constant net connection? -- GUESS!
   bool IsAlwaysOnline() const
      {
         ((wxDialUpManagerImpl *) this)->HangUp(); // brutal but necessary
         return IsOnline();
      }
   /// returns TRUE if (async) dialing is in progress
   inline virtual bool IsDialing() const
      { return m_DialProcess != NULL; }

   // cancel dialing the number initiated with Dial(async = TRUE)
   // NB: this won't result in DISCONNECTED event being sent
   virtual bool CancelDialing();

   size_t GetISPNames(class wxArrayString &) const
      { return 0; }
   
   // sometimes the built-in logic for determining the online status may fail,
   // so, in general, the user should be allowed to override it. This function
   // allows to forcefully set the online status - whatever our internal
   // algorithm may think about it.
   virtual void SetOnlineStatus(bool isOnline = TRUE)
      { m_IsOnline = isOnline; }

   // set misc wxDialUpManager options
   // --------------------------------

   // enable automatical checks for the connection status and sending of
   // wxEVT_DIALUP_CONNECTED/wxEVT_DIALUP_DISCONNECTED events. The interval
   // parameter is only for Unix where we do the check manually: under
   // Windows, the notification about the change of connection status is
   // instantenous.
   //
   // Returns FALSE if couldn't set up automatic check for online status.
   virtual bool EnableAutoCheckOnlineStatus(size_t nSeconds);

   // disable automatic check for connection status change - notice that the
   // wxEVT_DIALUP_XXX events won't be sent any more neither.
   virtual void DisableAutoCheckOnlineStatus();

   // under Unix, the value of well-known host is used to check whether we're
   // connected to the internet. It's unused under Windows, but this function
   // is always safe to call. The default value is www.yahoo.com.
   virtual void SetWellKnownHost(const wxString& hostname,
                                 int portno = 80);
   /** Sets the commands to start up the network and to hang up
       again. Used by the Unix implementations only.
   */
   virtual void SetConnectCommand(const wxString &command, const wxString &hupcmd)
      { m_ConnectCommand = command; m_HangUpCommand = hupcmd; }

private:
   /// -1: don�t know, 0 = no, 1 = yes
   int m_IsOnline;

   ///  Can we use ifconfig to list active devices?
   int m_CanUseIfconfig;
   /// The path to ifconfig
   wxString m_IfconfigPath;

   ///  Can we use ping to find hosts?
   int m_CanUsePing;
   /// The path to ping program
   wxString m_PingPath;
   
   /// beacon host:
   wxString m_BeaconHost;
   /// beacon host portnumber for connect:
   int m_BeaconPort;

   /// command to connect to network
   wxString m_ConnectCommand;
   /// command to hang up
   wxString m_HangUpCommand;
   /// name of ISP
   wxString m_ISPname;
   /// a timer for regular testing
   class AutoCheckTimer *m_timer;
   friend class AutoCheckTimer;

   /// a wxProcess for dialling in background
   class wxDialProcess *m_DialProcess;
   /// pid of dial process
   int m_DialPId;
   friend class wxDialProcess;

   /// determine status
   void CheckStatus(bool fromAsync = FALSE) const;

   /// real status check
   void CheckStatusInternal(void);

   /// Check output of ifconfig command for PPP/SLIP/PLIP devices
   int CheckIfconfig(void);
   /// Ping a host: 1 on success, -1 if it cannot be used, 0 if unreachable
   int CheckPing(void);
   /// Check by connecting to host on given port.
   int CheckConnect(void);
   
};


class AutoCheckTimer : public wxTimer
{
public:
   AutoCheckTimer(wxDialUpManagerImpl *dupman)
      {
         m_dupman = dupman;
         m_started = FALSE;
      }

   virtual bool Start( int millisecs = -1, bool WXUNUSED(one_shot) = FALSE )
      { m_started = TRUE; return wxTimer::Start(millisecs, FALSE); }

   virtual void Notify()
      { wxLogTrace(wxT("Checking dial up network status.")); m_dupman->CheckStatus(); }

   virtual void Stop()
      { if ( m_started ) wxTimer::Stop(); }
public:
   bool m_started;
   wxDialUpManagerImpl *m_dupman;
};

class wxDialProcess : public wxProcess
{
public:
   wxDialProcess(wxDialUpManagerImpl *dupman)
      {
         m_DupMan = dupman;
      }
   void Disconnect(void) { m_DupMan = NULL; }
   virtual void OnTerminate(int WXUNUSED(pid), int WXUNUSED(status))
      {
         if(m_DupMan)
         {
            m_DupMan->m_DialProcess = NULL;
            m_DupMan->CheckStatus(TRUE);
         }
      }
private:
      wxDialUpManagerImpl *m_DupMan;
};


wxDialUpManagerImpl::wxDialUpManagerImpl()
{
   m_IsOnline = -2; // -1 or -2, unknown
   m_DialProcess = NULL;
   m_timer = NULL;
   m_CanUseIfconfig = -1; // unknown
   m_CanUsePing = -1; // unknown
   m_BeaconHost = WXDIALUP_MANAGER_DEFAULT_BEACONHOST;
   m_BeaconPort = 80;
   SetConnectCommand("pon", "poff"); // default values for Debian/GNU linux
   wxChar * dial = wxGetenv(_T("WXDIALUP_DIALCMD"));
   wxChar * hup = wxGetenv(_T("WXDIALUP_HUPCMD"));
   if(dial || hup)
      SetConnectCommand(dial ? wxString(dial) : m_ConnectCommand,
                        hup ? wxString(hup) : m_HangUpCommand); 
}

wxDialUpManagerImpl::~wxDialUpManagerImpl()
{
   if(m_timer) delete m_timer;
   if(m_DialProcess)
   {
      m_DialProcess->Disconnect();
      m_DialProcess->Detach();
   }
}

bool
wxDialUpManagerImpl::Dial(const wxString &isp,
                          const wxString & WXUNUSED(username),
                          const wxString & WXUNUSED(password),
                          bool async)
{
   if(m_IsOnline == 1)
      return FALSE;
   m_IsOnline = -1;
   m_ISPname = isp;
   wxString cmd;
   if(m_ConnectCommand.Find(wxT("%s")))
      cmd.Printf(m_ConnectCommand,m_ISPname.c_str());
   else
      cmd = m_ConnectCommand;

   if ( async )
   {
      m_DialProcess = new wxDialProcess(this);
      m_DialPId = wxExecute(cmd, FALSE, m_DialProcess);
      if(m_DialPId == 0)
      {
         delete m_DialProcess;
         m_DialProcess = NULL;
         return FALSE;
      }
      else
         return TRUE;
   }
   else
       return wxExecute(cmd, /* sync */ TRUE) == 0;
}

bool
wxDialUpManagerImpl::HangUp(void)
{
   if(m_IsOnline == 0)
      return FALSE;
   if(IsDialing())
   {
      wxLogError(_("Already dialling ISP."));
      return FALSE;
   }
   m_IsOnline = -1;
   wxString cmd;
   if(m_HangUpCommand.Find(wxT("%s")))
      cmd.Printf(m_HangUpCommand,m_ISPname.c_str(), m_DialProcess);
   else
      cmd = m_HangUpCommand;
   return wxExecute(cmd, /* sync */ TRUE) == 0;
}


bool
wxDialUpManagerImpl::CancelDialing()
{
   if(! IsDialing())
      return FALSE;
   return kill(m_DialPId, SIGTERM) > 0;
}

bool
wxDialUpManagerImpl::EnableAutoCheckOnlineStatus(size_t nSeconds)
{
   DisableAutoCheckOnlineStatus();
   m_timer = new AutoCheckTimer(this);
   bool rc = m_timer->Start(nSeconds*1000);
   if(! rc)
   {
      delete m_timer;
      m_timer = NULL;
   }
   return rc;
}

void
wxDialUpManagerImpl::DisableAutoCheckOnlineStatus()
{
   if(m_timer != NULL)
   {
      m_timer->Stop();
      delete m_timer;
      m_timer = NULL;
   }
}


void
wxDialUpManagerImpl::SetWellKnownHost(const wxString& hostname, int portno)
{
   /// does hostname contain a port number?
   wxString port = hostname.After(wxT(':'));
   if(port.Length())
   {
      m_BeaconHost = hostname.Before(wxT(':'));
      m_BeaconPort = wxAtoi(port);
   }
   else
   {
      m_BeaconHost = hostname;
      m_BeaconPort = portno;
   }
}


void
wxDialUpManagerImpl::CheckStatus(bool fromAsync) const
{
   // This function calls the CheckStatusInternal() helper function
   // which is OS - specific and then sends the events.

   int oldIsOnline = m_IsOnline;
   ( /* non-const */ (wxDialUpManagerImpl *)this)->CheckStatusInternal();

   // now send the events as appropriate:
   if(m_IsOnline != oldIsOnline && m_IsOnline != -1 && oldIsOnline != -2) // -2: first time!
   {
      wxDialUpEvent event(m_IsOnline, ! fromAsync);
      (void)wxTheApp->ProcessEvent(event);
   }
}

/*
  We have three methods that we can use:

  1. test via /sbin/ifconfig and grep for "sl", "ppp", "pl"
     --> should be fast enough for regular polling
  2. test if we can reach the well known beacon host
     --> too slow for polling
  3. check /proc/net/dev on linux??
     This method should be preferred, if possible. Need to do more
     testing.

*/

void
wxDialUpManagerImpl::CheckStatusInternal(void)
{
   m_IsOnline = -1;

   int testResult;

   testResult = CheckConnect();
   if(testResult == -1)
      testResult = CheckIfconfig();
   if(testResult == -1)
      testResult = CheckPing();
      m_IsOnline = testResult;
}

int
wxDialUpManagerImpl::CheckConnect(void)
{
   // second method: try to connect to a well known host:
   // This can be used under Win 9x, too!
   struct hostent     *hp;
   struct sockaddr_in  serv_addr;

   if((hp = gethostbyname(m_BeaconHost.mb_str())) == NULL)
      return 0; // no DNS no net
   
   serv_addr.sin_family		= hp->h_addrtype;
   memcpy(&serv_addr.sin_addr,hp->h_addr, hp->h_length);
   serv_addr.sin_port		= htons(m_BeaconPort);

   int	sockfd;
   if( ( sockfd = socket(hp->h_addrtype, SOCK_STREAM, 0)) < 0) 
   {	
      return -1;  // no info
   }
   
   if( connect(sockfd, (struct sockaddr *) &serv_addr,
               sizeof(serv_addr)) >= 0)
   {
      close(sockfd);
      return 1; // we cant connect, so we have a network!
   }
   //connected!
   close(sockfd);
   if(errno == ENETUNREACH)
      return 0; // network is unreachable
   // connect failed, but don't know why
   return -1;
}

int
wxDialUpManagerImpl::CheckIfconfig(void)
{
   int rc = -1;
   // First time check for ifconfig location. We only use the variant
   // which does not take arguments, a la GNU.
   if(m_CanUseIfconfig == -1) // unknown
   {
      if(wxFileExists("/sbin/ifconfig"))
         m_IfconfigPath = "/sbin/ifconfig";
      else if(wxFileExists("/usr/sbin/ifconfig"))
         m_IfconfigPath = "/usr/sbin/ifconfig";
   }

   wxLogNull ln; // suppress all error messages
   // Let�s try the ifconfig method first, should be fastest:
   if(m_CanUseIfconfig != 0) // unknown or yes
   {
      wxASSERT(m_IfconfigPath.length());

      wxString tmpfile = wxGetTempFileName("_wxdialuptest");
      wxString cmd = "/bin/sh -c \'";
      cmd << m_IfconfigPath;
#if defined(__SOLARIS__) || defined (__SUNOS__)
      // need to add -a flag
      cmd << " -a";
#elif defined(__LINUX__) || defined (__FREEBSD__)
      // nothing to be added to ifconfig
#else
#   pragma warning "No ifconfig information for this OS."
      m_CanUseIfconfig = 0;
      return -1;
#endif
      cmd << " >" << tmpfile <<  '\'';
      /* I tried to add an option to wxExecute() to not close stdout,
         so we could let ifconfig write directly to the tmpfile, but
         this does not work. That should be faster, as it doesn�t call
         the shell first. I have no idea why. :-(  (KB) */
      if(wxExecute(cmd,TRUE /* sync */) == 0)
      {
         m_CanUseIfconfig = 1;
         wxFile file;
         if( file.Open(tmpfile) )
         {
            char *output = new char [file.Length()+1];
            output[file.Length()] = '\0';
            if(file.Read(output,file.Length()) == file.Length())
            {
               if(
#if defined(__SOLARIS__) || defined (__SUNOS__)
                  strstr(output,"ipdptp")   // dialup device
#elif defined(__LINUX__) || defined (__FREEBSD__)
                  strstr(output,"ppp")   // ppp
                  || strstr(output,"sl") // slip
                  || strstr(output,"pl") // plip
#else
                  FALSE
#endif
                  )
                  rc = 1;
               else
                  rc = 0;
            }
            file.Close();
            delete [] output;
         }
         // else rc remains -1 as we don't know for sure
      }
      else // could not run ifconfig correctly
         m_CanUseIfconfig = 0; // don�t try again
      (void) wxRemoveFile(tmpfile);
   }
      return rc;
}

int
wxDialUpManagerImpl::CheckPing(void)
{
   if(! m_CanUsePing)
      return -1;
   
   // First time check for ping location. We only use the variant
   // which does not take arguments, a la GNU.
   if(m_CanUsePing == -1) // unknown
   {
      if(wxFileExists("/bin/ping"))
         m_PingPath = "/bin/ping";
      else if(wxFileExists("/usr/sbin/ping"))
         m_PingPath = "/usr/sbin/ping";
      if(! m_PingPath)
      {
         m_CanUsePing = 0;
         return -1;
      }
   }

   wxLogNull ln; // suppress all error messages
   wxASSERT(m_PingPath.length());
   wxString cmd;
   cmd << m_PingPath << ' ';
#if defined(__SOLARIS__) || defined (__SUNOS__)
   // nothing to add to ping command
#elif defined(__LINUX__)
   cmd << "-c 1 "; // only ping once
#else
#   pragma warning "No Ping information for this OS."
   m_CanUsePing = 0;
   return -1;
#endif
   cmd << m_BeaconHost;
   if(wxExecute(cmd, TRUE /* sync */) == 0)
      return 1;
   else
      return 0;
}

/* static */
wxDialUpManager *
wxDialUpManager::Create(void)
{
   return new wxDialUpManagerImpl;
}

#endif // wxUSE_DIALUP_MANAGER
