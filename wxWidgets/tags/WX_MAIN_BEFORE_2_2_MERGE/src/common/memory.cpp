/////////////////////////////////////////////////////////////////////////////
// Name:        memory.cpp
// Purpose:     Memory checking implementation
// Author:      Arthur Seaton, Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "memory.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/defs.h"
#endif

#if (defined(__WXDEBUG__) && wxUSE_MEMORY_TRACING) || wxUSE_DEBUG_CONTEXT

#ifdef __GNUG__
// #pragma implementation
#endif

#ifndef WX_PRECOMP
#include "wx/utils.h"
#include "wx/app.h"
#endif

#include "wx/log.h"
#include <stdlib.h>

#include "wx/ioswrap.h"

#if wxUSE_IOSTREAMH
    #include <fstream.h>
#else
    #include <fstream>
#endif

#if !defined(__WATCOMC__) && !(defined(__VMS__) && ( __VMS_VER < 70000000 ) )\
     && !defined( __MWERKS__ ) && !defined(__SALFORDC__)
#include <memory.h>
#endif

#include <stdarg.h>
#include <string.h>

#ifdef __WXMSW__
#include <windows.h>

#ifdef GetClassInfo
#undef GetClassInfo
#endif

#ifdef GetClassName
#undef GetClassName
#endif

#endif

#include "wx/memory.h"

#ifdef new
#undef new
#endif

// wxDebugContext wxTheDebugContext;
/*
  Redefine new and delete so that we can pick up situations where:
        - we overwrite or underwrite areas of malloc'd memory.
        - we use uninitialise variables
  Only do this in debug mode.

  We change new to get enough memory to allocate a struct, followed
  by the caller's requested memory, followed by a tag. The struct
  is used to create a doubly linked list of these areas and also
  contains another tag. The tags are used to determine when the area
  has been over/under written.
*/


/*
  Values which are used to set the markers which will be tested for
  under/over write. There are 3 of these, one in the struct, one
  immediately after the struct but before the caller requested memory and
  one immediately after the requested memory.
*/
#define MemStartCheck  0x23A8
#define MemMidCheck  0xA328
#define MemEndCheck 0x8A32
#define MemFillChar 0xAF
#define MemStructId  0x666D

/*
  External interface for the wxMemStruct class. Others are
  defined inline within the class def. Here we only need to be able
  to add and delete nodes from the list and handle errors in some way.
*/

/*
  Used for internal "this shouldn't happen" type of errors.
*/
void wxMemStruct::ErrorMsg (const char * mesg)
{
  wxLogMessage(wxT("wxWindows memory checking error: %s"), mesg);
  PrintNode ();

//         << m_fileName << ' ' << m_lineNum << endl;
}

/*
  Used when we find an overwrite or an underwrite error.
*/
void wxMemStruct::ErrorMsg ()
{
  wxLogMessage(wxT("wxWindows over/underwrite memory error:"));
  PrintNode ();

//    cerr << m_fileName << ' ' << m_lineNum << endl;
}


/*
  We want to find out if pointers have been overwritten as soon as is
  possible, so test everything before we dereference it. Of course it's still
  quite possible that, if things have been overwritten, this function will
  fall over, but the only way of dealing with that would cost too much in terms
  of time.
*/
int wxMemStruct::AssertList ()
{
    if (wxDebugContext::GetHead () != 0 && ! (wxDebugContext::GetHead ())->AssertIt () ||
        wxDebugContext::GetTail () != 0 && ! wxDebugContext::GetTail ()->AssertIt ()) {
        ErrorMsg ("Head or tail pointers trashed");
        return 0;
    }
    return 1;
}


/*
  Check that the thing we're pointing to has the correct id for a wxMemStruct
  object and also that it's previous and next pointers are pointing at objects
  which have valid ids.
  This is definitely not perfect since we could fall over just trying to access
  any of the slots which we use here, but I think it's about the best that I
  can do without doing something like taking all new wxMemStruct pointers and
  comparing them against all known pointer within the list and then only
  doing this sort of check _after_ you've found the pointer in the list. That
  would be safer, but also much more time consuming.
*/
int wxMemStruct::AssertIt ()
{
    return (m_id == MemStructId &&
            (m_prev == 0 || m_prev->m_id == MemStructId) &&
            (m_next == 0 || m_next->m_id == MemStructId));
}


/*
  Additions are always at the tail of the list.
  Returns 0 on error, non-zero on success.
*/
int wxMemStruct::Append ()
{
    if (! AssertList ())
        return 0;

    if (wxDebugContext::GetHead () == 0) {
        if (wxDebugContext::GetTail () != 0) {
            ErrorMsg ("Null list should have a null tail pointer");
            return 0;
        }
        (void) wxDebugContext::SetHead (this);
        (void) wxDebugContext::SetTail (this);
    } else {
        wxDebugContext::GetTail ()->m_next = this;
        this->m_prev = wxDebugContext::GetTail ();
        (void) wxDebugContext::SetTail (this);
    }
    return 1;
}


/*
  Don't actually free up anything here as the space which is used
  by the node will be free'd up when the whole block is free'd.
  Returns 0 on error, non-zero on success.
*/
int wxMemStruct::Unlink ()
{
    if (! AssertList ())
        return 0;

    if (wxDebugContext::GetHead () == 0 || wxDebugContext::GetTail () == 0) {
        ErrorMsg ("Trying to remove node from empty list");
        return 0;
    }

    // Handle the part of the list before this node.
    if (m_prev == 0) {
        if (this != wxDebugContext::GetHead ()) {
            ErrorMsg ("No previous node for non-head node");
            return 0;
        }
        (void) wxDebugContext::SetHead (m_next);
    } else {
        if (! m_prev->AssertIt ()) {
            ErrorMsg ("Trashed previous pointer");
            return 0;
        }

        if (m_prev->m_next != this) {
            ErrorMsg ("List is inconsistent");
            return 0;
        }
        m_prev->m_next = m_next;
    }

    // Handle the part of the list after this node.
    if (m_next == 0) {
        if (this != wxDebugContext::GetTail ()) {
            ErrorMsg ("No next node for non-tail node");
            return 0;
        }
        (void) wxDebugContext::SetTail (m_prev);
    } else {
        if (! m_next->AssertIt ()) {
            ErrorMsg ("Trashed next pointer");
            return 0;
        }

        if (m_next->m_prev != this) {
            ErrorMsg ("List is inconsistent");
            return 0;
        }
        m_next->m_prev = m_prev;
    }

    return 1;
}



/*
  Checks a node and block of memory to see that the markers are still
  intact.
*/
int wxMemStruct::CheckBlock ()
{
    int nFailures = 0;

    if (m_firstMarker != MemStartCheck) {
        nFailures++;
        ErrorMsg ();
    }

    char * pointer = wxDebugContext::MidMarkerPos ((char *) this);
    if (* (wxMarkerType *) pointer != MemMidCheck) {
        nFailures++;
        ErrorMsg ();
    }

    pointer = wxDebugContext::EndMarkerPos ((char *) this, RequestSize ());
    if (* (wxMarkerType *) pointer != MemEndCheck) {
        nFailures++;
        ErrorMsg ();
    }

    return nFailures;
}


/*
  Check the list of nodes to see if they are all ok.
*/
int wxMemStruct::CheckAllPrevious ()
{
    int nFailures = 0;

    for (wxMemStruct * st = this->m_prev; st != 0; st = st->m_prev) {
        if (st->AssertIt ())
            nFailures += st->CheckBlock ();
        else
            return -1;
    }

    return nFailures;
}


/*
  When we delete a node we set the id slot to a specific value and then test
  against this to see if a nodes have been deleted previously. I don't
  just set the entire memory to the fillChar because then I'd be overwriting
  useful stuff like the vtbl which may be needed to output the error message
  including the file name and line numbers. Without this info the whole point
  of this class is lost!
*/
void wxMemStruct::SetDeleted ()
{
    m_id = MemFillChar;
}

int wxMemStruct::IsDeleted ()
{
    return (m_id == MemFillChar);
}


/*
  Print out a single node. There are many far better ways of doing this
  but this will suffice for now.
*/
void wxMemStruct::PrintNode ()
{
  if (m_isObject)
  {
    wxObject *obj = (wxObject *)m_actualData;
    wxClassInfo *info = obj->GetClassInfo();

    // Let's put this in standard form so IDEs can load the file at the appropriate
    // line
    wxString msg(wxT(""));

    if (m_fileName)
      msg.Printf(wxT("%s(%d): "), m_fileName, (int)m_lineNum);

    if (info && info->GetClassName())
      msg += info->GetClassName();
    else
      msg += wxT("object");

    wxString msg2;
    msg2.Printf(wxT(" at $%lX, size %d"), (long)GetActualData(), (int)RequestSize());
    msg += msg2;

    wxLogMessage(msg);
  }
  else
  {
    wxString msg("");

    if (m_fileName)
      msg.Printf(wxT("%s(%d): "), m_fileName, (int)m_lineNum);
    msg += wxT("non-object data");
    wxString msg2;
    msg2.Printf(wxT(" at $%lX, size %d\n"), (long)GetActualData(), (int)RequestSize());
    msg += msg2;

    wxLogMessage(msg);
  }
}

void wxMemStruct::Dump ()
{
  if (!ValidateNode()) return;

  if (m_isObject)
  {
    wxObject *obj = (wxObject *)m_actualData;

    wxString msg(wxT(""));
    if (m_fileName)
      msg.Printf(wxT("%s(%d): "), m_fileName, (int)m_lineNum);


    /* TODO: We no longer have a stream (using wxLogDebug) so we can't dump it.
     * Instead, do what wxObject::Dump does.
     * What should we do long-term, eliminate Dumping? Or specify
     * that MyClass::Dump should use wxLogDebug? Ugh.
    obj->Dump(wxDebugContext::GetStream());
     */

    if (obj->GetClassInfo() && obj->GetClassInfo()->GetClassName())
      msg += obj->GetClassInfo()->GetClassName();
    else
      msg += wxT("unknown object class");

    wxString msg2("");
    msg2.Printf(wxT(" at $%lX, size %d"), (long)GetActualData(), (int)RequestSize());
    msg += msg2;

    wxLogMessage(msg);
  }
  else
  {
    wxString msg(wxT(""));
    if (m_fileName)
      msg.Printf(wxT("%s(%d): "), m_fileName, (int)m_lineNum);

    wxString msg2("");
    msg2.Printf(wxT("non-object data at $%lX, size %d"), (long)GetActualData(), (int)RequestSize() );
    msg += msg2;
    wxLogMessage(msg);
  }
}


/*
  Validate a node. Check to see that the node is "clean" in the sense
  that nothing has over/underwritten it etc.
*/
int wxMemStruct::ValidateNode ()
{
    char * startPointer = (char *) this;
    if (!AssertIt ()) {
        if (IsDeleted ())
            ErrorMsg ("Object already deleted");
        else {
            // Can't use the error routines as we have no recognisable object.
#ifndef __WXGTK__
             wxLogMessage(wxT("Can't verify memory struct - all bets are off!"));
#endif
        }
        return 0;
    }

/*
    int i;
    for (i = 0; i < wxDebugContext::TotSize (requestSize ()); i++)
      cout << startPointer [i];
    cout << endl;
*/
    if (Marker () != MemStartCheck)
      ErrorMsg ();
    if (* (wxMarkerType *) wxDebugContext::MidMarkerPos (startPointer) != MemMidCheck)
      ErrorMsg ();
    if (* (wxMarkerType *) wxDebugContext::EndMarkerPos (startPointer,
                                              RequestSize ()) !=
        MemEndCheck)
      ErrorMsg ();

    // Back to before the extra buffer and check that
    // we can still read what we originally wrote.
    if (Marker () != MemStartCheck ||
        * (wxMarkerType *) wxDebugContext::MidMarkerPos (startPointer)
                         != MemMidCheck ||
        * (wxMarkerType *) wxDebugContext::EndMarkerPos (startPointer,
                                              RequestSize ()) != MemEndCheck)
    {
        ErrorMsg ();
        return 0;
    }

    return 1;
}

/*
  The wxDebugContext class.
*/

wxMemStruct *wxDebugContext::m_head = NULL;
wxMemStruct *wxDebugContext::m_tail = NULL;
// ostream *wxDebugContext::m_debugStream = NULL;
// streambuf *wxDebugContext::m_streamBuf = NULL;

// Must initialise these in wxEntry, and then delete them just before wxEntry exits
streambuf *wxDebugContext::m_streamBuf = NULL;
ostream *wxDebugContext::m_debugStream = NULL;

bool wxDebugContext::m_checkPrevious = FALSE;
int wxDebugContext::debugLevel = 1;
bool wxDebugContext::debugOn = TRUE;
wxMemStruct *wxDebugContext::checkPoint = NULL;

// For faster alignment calculation
static wxMarkerType markerCalc[2];
int wxDebugContext::m_balign = (int)((char *)&markerCalc[1] - (char*)&markerCalc[0]);
int wxDebugContext::m_balignmask = (int)((char *)&markerCalc[1] - (char*)&markerCalc[0]) - 1;

wxDebugContext::wxDebugContext(void)
{
//  m_streamBuf = new wxDebugStreamBuf;
//  m_debugStream = new ostream(m_streamBuf);
}

wxDebugContext::~wxDebugContext(void)
{
  SetStream(NULL, NULL);
}

/*
 * It's bizarre, but with BC++ 4.5, the value of str changes
 * between SetFile and SetStream.
 */

void wxDebugContext::SetStream(ostream *str, streambuf *buf)
{
  if (m_debugStream)
  {
    m_debugStream->flush();
    delete m_debugStream;
  }
  m_debugStream = NULL;

  // Not allowed in Watcom (~streambuf is protected).
  // Is this trying to say something significant to us??
#ifndef __WATCOMC__
  if (m_streamBuf)
  {
    streambuf* oldBuf = m_streamBuf;
    m_streamBuf = NULL;
    delete oldBuf;
  }
#endif
  m_streamBuf = buf;
  m_debugStream = str;
}

bool wxDebugContext::SetFile(const wxString& file)
{
  ofstream *str = new ofstream(file.fn_str());

  if (str->bad())
  {
    delete str;
    return FALSE;
  }
  else
  {
    SetStream(str);
    return TRUE;
  }
}

bool wxDebugContext::SetStandardError(void)
{
    // Obsolete
#if 0
#if !defined(_WINDLL)
  wxDebugStreamBuf *buf = new wxDebugStreamBuf;
  ostream *stream = new ostream(m_streamBuf);
  SetStream(stream, buf);
  return TRUE;
#else
  return FALSE;
#endif
#endif
  return FALSE;
}


/*
  Work out the positions of the markers by creating an array of 2 markers
  and comparing the addresses of the 2 elements. Use this number as the
  alignment for markers.
*/
size_t wxDebugContext::CalcAlignment ()
{
    wxMarkerType ar[2];
    return (char *) &ar[1] - (char *) &ar[0];
}


char * wxDebugContext::StructPos (const char * buf)
{
    return (char *) buf;
}

char * wxDebugContext::MidMarkerPos (const char * buf)
{
    return StructPos (buf) + PaddedSize (sizeof (wxMemStruct));
}

char * wxDebugContext::CallerMemPos (const char * buf)
{
    return MidMarkerPos (buf) + PaddedSize (sizeof(wxMarkerType));
}


char * wxDebugContext::EndMarkerPos (const char * buf, const size_t size)
{
    return CallerMemPos (buf) + PaddedSize (size);
}


/*
  Slightly different as this takes a pointer to the start of the caller
  requested region and returns a pointer to the start of the buffer.
  */
char * wxDebugContext::StartPos (const char * caller)
{
    return ((char *) (caller - wxDebugContext::PaddedSize (sizeof(wxMarkerType)) -
            wxDebugContext::PaddedSize (sizeof (wxMemStruct))));
}

/*
  We may need padding between various parts of the allocated memory.
  Given a size of memory, this returns the amount of memory which should
  be allocated in order to allow for alignment of the following object.

  I don't know how portable this stuff is, but it seems to work for me at
  the moment. It would be real nice if I knew more about this!

  // Note: this function is now obsolete (along with CalcAlignment)
  // because the calculations are done statically, for greater speed.
*/
size_t wxDebugContext::GetPadding (const size_t size)
{
    size_t pad = size % CalcAlignment ();
    return (pad) ? sizeof(wxMarkerType) - pad : 0;
}

size_t wxDebugContext::PaddedSize (const size_t size)
{
    // Added by Terry Farnham <TJRT@pacbell.net> to replace
    // slow GetPadding call.
	int padb;

	padb = size & m_balignmask;
	if(padb)
		return(size + m_balign - padb);
	else
		return(size);

// Old (slow) code
#if 0
    return size + GetPadding (size);
#endif
}

/*
  Returns the total amount of memory which we need to get from the system
  in order to satisfy a caller request. This includes space for the struct
  plus markers and the caller's memory as well.
*/
size_t wxDebugContext::TotSize (const size_t reqSize)
{
    return (PaddedSize (sizeof (wxMemStruct)) + PaddedSize (reqSize) +
            2 * sizeof(wxMarkerType));
}


/*
  Traverse the list of nodes executing the given function on each node.
*/
void wxDebugContext::TraverseList (PmSFV func, wxMemStruct *from)
{
  if (!from)
    from = wxDebugContext::GetHead ();

  for (wxMemStruct * st = from; st != 0; st = st->m_next)
  {
      void* data = st->GetActualData();
//      if ((data != (void*)m_debugStream) && (data != (void*) m_streamBuf))
      if (data != (void*) wxLog::GetActiveTarget())
      {
        (st->*func) ();
      }
  }
}


/*
  Print out the list.
  */
bool wxDebugContext::PrintList (void)
{
#ifdef __WXDEBUG__
//  if (!HasStream())
//    return FALSE;

  TraverseList ((PmSFV)&wxMemStruct::PrintNode, (checkPoint ? checkPoint->m_next : (wxMemStruct*)NULL));

  return TRUE;
#else
  return FALSE;
#endif
}

bool wxDebugContext::Dump(void)
{
#ifdef __WXDEBUG__
//  if (!HasStream())
//    return FALSE;

//  if (TRUE)
  {
    wxChar* appName = wxT("application");
    wxString appNameStr("");
    if (wxTheApp)
    {
        appNameStr = wxTheApp->GetAppName();
        appName = WXSTRINGCAST appNameStr;
        wxLogMessage(wxT("----- Memory dump of %s at %s -----"), appName, WXSTRINGCAST wxNow() );
    }
    else
    {
      wxLogMessage( wxT("----- Memory dump -----") );
    }
  }
  TraverseList ((PmSFV)&wxMemStruct::Dump, (checkPoint ? checkPoint->m_next : (wxMemStruct*)NULL));

  wxLogMessage( wxT("") );
  wxLogMessage( wxT("") );

  return TRUE;
#else
  return FALSE;
#endif
}

struct wxDebugStatsStruct
{
  long instanceCount;
  long totalSize;
  wxChar *instanceClass;
  wxDebugStatsStruct *next;
};

static wxDebugStatsStruct *FindStatsStruct(wxDebugStatsStruct *st, wxChar *name)
{
  while (st)
  {
    if (wxStrcmp(st->instanceClass, name) == 0)
      return st;
    st = st->next;
  }
  return NULL;
}

static wxDebugStatsStruct *InsertStatsStruct(wxDebugStatsStruct *head, wxDebugStatsStruct *st)
{
  st->next = head;
  return st;
}

bool wxDebugContext::PrintStatistics(bool detailed)
{
#ifdef __WXDEBUG__
//  if (!HasStream())
//    return FALSE;

//  if (TRUE)
  {
    wxChar* appName = wxT("application");
    wxString appNameStr(wxT(""));
    if (wxTheApp)
    {
        appNameStr = wxTheApp->GetAppName();
        appName = WXSTRINGCAST appNameStr;
        wxLogMessage(wxT("----- Memory statistics of %s at %s -----"), appName, WXSTRINGCAST wxNow() );
    }
    else
    {
      wxLogMessage( wxT("----- Memory statistics -----") );
    }
  }

  bool currentMode = GetDebugMode();
  SetDebugMode(FALSE);

  long noNonObjectNodes = 0;
  long noObjectNodes = 0;
  long totalSize = 0;

  wxDebugStatsStruct *list = NULL;

  wxMemStruct *from = (checkPoint ? checkPoint->m_next : (wxMemStruct*)NULL );
  if (!from)
    from = wxDebugContext::GetHead ();

  wxMemStruct *st;
  for (st = from; st != 0; st = st->m_next)
  {
    void* data = st->GetActualData();
//    if (detailed && (data != (void*)m_debugStream) && (data != (void*) m_streamBuf))
      if (detailed && (data != (void*) wxLog::GetActiveTarget()))
    {
      wxChar *className = wxT("nonobject");
      if (st->m_isObject && st->GetActualData())
      {
        wxObject *obj = (wxObject *)st->GetActualData();
        if (obj->GetClassInfo()->GetClassName())
          className = obj->GetClassInfo()->GetClassName();
      }
      wxDebugStatsStruct *stats = FindStatsStruct(list, className);
      if (!stats)
      {
        stats = (wxDebugStatsStruct *)malloc(sizeof(wxDebugStatsStruct));
        stats->instanceClass = className;
        stats->instanceCount = 0;
        stats->totalSize = 0;
        list = InsertStatsStruct(list, stats);
      }
      stats->instanceCount ++;
      stats->totalSize += st->RequestSize();
    }

//    if ((data != (void*)m_debugStream) && (data != (void*) m_streamBuf))
    if (data != (void*) wxLog::GetActiveTarget())
    {
        totalSize += st->RequestSize();
        if (st->m_isObject)
            noObjectNodes ++;
        else
            noNonObjectNodes ++;
    }
  }

  if (detailed)
  {
    while (list)
    {
      wxLogMessage(wxT("%ld objects of class %s, total size %ld"),
          list->instanceCount, list->instanceClass, list->totalSize);
      wxDebugStatsStruct *old = list;
      list = old->next;
      free((char *)old);
    }
    wxLogMessage(wxT(""));
  }

  SetDebugMode(currentMode);

  wxLogMessage(wxT("Number of object items: %ld"), noObjectNodes);
  wxLogMessage(wxT("Number of non-object items: %ld"), noNonObjectNodes);
  wxLogMessage(wxT("Total allocated size: %ld"), totalSize);
  wxLogMessage(wxT(""));
  wxLogMessage(wxT(""));

  return TRUE;
#else
  return FALSE;
#endif
}

bool wxDebugContext::PrintClasses(void)
{
//  if (!HasStream())
//    return FALSE;

//  if (TRUE)
  {
    wxChar* appName = wxT("application");
    wxString appNameStr(wxT(""));
    if (wxTheApp)
    {
        appNameStr = wxTheApp->GetAppName();
        appName = WXSTRINGCAST appNameStr;
        wxLogMessage(wxT("----- Classes in %s -----"), appName);
    }
  }

  int n = 0;
  wxNode *node;
  wxClassInfo *info;

  wxClassInfo::sm_classTable->BeginFind();
  node = wxClassInfo::sm_classTable->Next();
  while (node)
  {
    info = (wxClassInfo *)node->Data();
    if (info->GetClassName())
    {
        wxString msg(info->GetClassName());
        msg += wxT(" ");

        if (info->GetBaseClassName1() && !info->GetBaseClassName2())
        {
            msg += wxT("is a ");
            msg += info->GetBaseClassName1();
        }
        else if (info->GetBaseClassName1() && info->GetBaseClassName2())
        {
            msg += wxT("is a ");
            msg += info->GetBaseClassName1() ;
            msg += wxT(", ");
            msg += info->GetBaseClassName2() ;
        }
        if (info->GetConstructor())
            msg += wxT(": dynamic");

        wxLogMessage(msg);
    }
    node = wxClassInfo::sm_classTable->Next();
    n ++;
  }
  wxLogMessage(wxT(""));
  wxLogMessage(wxT("There are %d classes derived from wxObject."), n);
  wxLogMessage(wxT(""));
  wxLogMessage(wxT(""));
  return TRUE;
}

void wxDebugContext::SetCheckpoint(bool all)
{
  if (all)
    checkPoint = NULL;
  else
    checkPoint = m_tail;
}

// Checks all nodes since checkpoint, or since start.
int wxDebugContext::Check(bool checkAll)
{
  int nFailures = 0;

  wxMemStruct *from = (checkPoint ? checkPoint->m_next : (wxMemStruct*)NULL );
  if (!from || checkAll)
    from = wxDebugContext::GetHead ();

  for (wxMemStruct * st = from; st != 0; st = st->m_next)
  {
    if (st->AssertIt ())
      nFailures += st->CheckBlock ();
    else
      return -1;
  }

  return nFailures;
}

// Count the number of non-wxDebugContext-related objects
// that are outstanding
int wxDebugContext::CountObjectsLeft(bool sinceCheckpoint)
{
  int n = 0;

  wxMemStruct *from = NULL;
  if (sinceCheckpoint && checkPoint)
    from = checkPoint->m_next;
  else
    from = wxDebugContext::GetHead () ;

  for (wxMemStruct * st = from; st != 0; st = st->m_next)
  {
      void* data = st->GetActualData();
//      if ((data != (void*)m_debugStream) && (data != (void*) m_streamBuf))
      if (data != (void*) wxLog::GetActiveTarget())
          n ++;
  }

  return n ;
}

/*
  The global operator new used for everything apart from getting
  dynamic storage within this function itself.
*/

// We'll only do malloc and free for the moment: leave the interesting
// stuff for the wxObject versions.

#if defined(__WXDEBUG__) && wxUSE_GLOBAL_MEMORY_OPERATORS

#ifdef new
#undef new
#endif

// Seems OK all of a sudden. Maybe to do with linking with multithreaded library?
#if 0 // def __VISUALC__
#define NO_DEBUG_ALLOCATION
#endif

// Unfortunately ~wxDebugStreamBuf doesn't work (VC++ 5) when we enable the debugging
// code. I have no idea why. In BC++ 4.5, we have a similar problem the debug
// stream myseriously changing pointer address between being passed from SetFile to SetStream.
// See docs/msw/issues.txt.
void * operator new (size_t size, wxChar * fileName, int lineNum)
{
#ifdef NO_DEBUG_ALLOCATION
  return malloc(size);
#else
  return wxDebugAlloc(size, fileName, lineNum, FALSE, FALSE);
#endif
}

// Added JACS 25/11/98
void * operator new (size_t size)
{
#ifdef NO_DEBUG_ALLOCATION
  return malloc(size);
#else
  return wxDebugAlloc(size, NULL, 0, FALSE);
#endif
}

#if wxUSE_ARRAY_MEMORY_OPERATORS
void * operator new[] (size_t size)
{
#ifdef NO_DEBUG_ALLOCATION
  return malloc(size);
#else
  return wxDebugAlloc(size, NULL, 0, FALSE, TRUE);
#endif
}
#endif

#if wxUSE_ARRAY_MEMORY_OPERATORS
void * operator new[] (size_t size, wxChar * fileName, int lineNum)
{
#ifdef NO_DEBUG_ALLOCATION
  return malloc(size);
#else
  return wxDebugAlloc(size, fileName, lineNum, FALSE, TRUE);
#endif
}
#endif

#if !defined(__VISAGECPP__) // already defines this by default
void operator delete (void * buf)
{
#ifdef NO_DEBUG_ALLOCATION
  free((char*) buf);
#else
  wxDebugFree(buf);
#endif
}
#endif

// VC++ 6.0
#if defined(__VISUALC__) && (__VISUALC__ >= 1200)
void operator delete(void* pData, wxChar* /* fileName */, int /* lineNum */)
{
  wxDebugFree(pData, FALSE);
}
// New operator 21/11/1998
void operator delete[](void* pData, char* /* fileName */, int /* lineNum */)
{
  wxDebugFree(pData, TRUE);
}
#endif

#if wxUSE_ARRAY_MEMORY_OPERATORS

void operator delete[] (void * buf)
{
#ifdef NO_DEBUG_ALLOCATION
  free((char*) buf);
#else
  wxDebugFree(buf, TRUE);
#endif
}
#endif

#endif

// TODO: store whether this is a vector or not.
void * wxDebugAlloc(size_t size, wxChar * fileName, int lineNum, bool isObject, bool WXUNUSED(isVect) )
{
  // If not in debugging allocation mode, do the normal thing
  // so we don't leave any trace of ourselves in the node list.

  if (!wxDebugContext::GetDebugMode())
  {
    return (void *)malloc(size);
  }

    int totSize = wxDebugContext::TotSize (size);
    char * buf = (char *) malloc(totSize);
    if (!buf) {
        wxLogMessage(wxT("Call to malloc (%ld) failed."), (long)size);
        return 0;
    }
    wxMemStruct * st = (wxMemStruct *)buf;
    st->m_firstMarker = MemStartCheck;
    st->m_reqSize = size;
    st->m_fileName = fileName;
    st->m_lineNum = lineNum;
    st->m_id = MemStructId;
    st->m_prev = 0;
    st->m_next = 0;
    st->m_isObject = isObject;

    // Errors from Append() shouldn't really happen - but just in case!
    if (st->Append () == 0) {
        st->ErrorMsg ("Trying to append new node");
    }

    if (wxDebugContext::GetCheckPrevious ()) {
        if (st->CheckAllPrevious () < 0) {
            st->ErrorMsg ("Checking previous nodes");
        }
    }

    // Set up the extra markers at the middle and end.
    char * ptr = wxDebugContext::MidMarkerPos (buf);
    * (wxMarkerType *) ptr = MemMidCheck;
    ptr = wxDebugContext::EndMarkerPos (buf, size);
    * (wxMarkerType *) ptr = MemEndCheck;

    // pointer returned points to the start of the caller's
    // usable area.
    void *m_actualData = (void *) wxDebugContext::CallerMemPos (buf);
    st->m_actualData = m_actualData;

    return m_actualData;
}

// TODO: check whether was allocated as a vector
void wxDebugFree(void * buf, bool WXUNUSED(isVect) )
{
  if (!buf)
    return;

  // If not in debugging allocation mode, do the normal thing
  // so we don't leave any trace of ourselves in the node list.
  if (!wxDebugContext::GetDebugMode())
  {
    free((char *)buf);
    return;
  }

    // Points to the start of the entire allocated area.
    char * startPointer = wxDebugContext::StartPos ((char *) buf);
    // Find the struct and make sure that it's identifiable.
    wxMemStruct * st = (wxMemStruct *) wxDebugContext::StructPos (startPointer);

    if (! st->ValidateNode ())
        return;

    // If this is the current checkpoint, we need to
    // move the checkpoint back so it points to a valid
    // node.
    if (st == wxDebugContext::checkPoint)
      wxDebugContext::checkPoint = wxDebugContext::checkPoint->m_prev;

    if (! st->Unlink ())
    {
      st->ErrorMsg ("Unlinking deleted node");
    }

    // Now put in the fill char into the id slot and the caller requested
    // memory locations.
    st->SetDeleted ();
    (void) memset (wxDebugContext::CallerMemPos (startPointer), MemFillChar,
                   st->RequestSize ());

    // Don't allow delayed freeing of memory in this version
//    if (!wxDebugContext::GetDelayFree())
//    free((void *)st);
    free((char *)st);
}

// Trace: send output to the current debugging stream
void wxTrace(const wxChar *fmt ...)
{
  va_list ap;
  static wxChar buffer[512];

  va_start(ap, fmt);

#ifdef __WXMSW__
  wvsprintf(buffer,fmt,ap) ;
#else
  vsprintf(buffer,fmt,ap) ;
#endif

  va_end(ap);

  if (wxDebugContext::HasStream())
  {
    wxDebugContext::GetStream() << buffer;
    wxDebugContext::GetStream().flush();
  }
  else
#ifdef __WXMSW__
#ifdef __WIN32__
    OutputDebugString((LPCTSTR)buffer) ;
#else
    OutputDebugString((const char*) buffer) ;
#endif
#else
    fprintf(stderr, buffer);
#endif
}

// Trace with level
void wxTraceLevel(int level, const wxChar *fmt ...)
{
  if (wxDebugContext::GetLevel() < level)
    return;

  va_list ap;
  static wxChar buffer[512];

  va_start(ap, fmt);

#ifdef __WXMSW__
  wvsprintf(buffer,fmt,ap) ;
#else
  vsprintf(buffer,fmt,ap) ;
#endif

  va_end(ap);

  if (wxDebugContext::HasStream())
  {
    wxDebugContext::GetStream() << buffer;
    wxDebugContext::GetStream().flush();
  }
  else
#ifdef __WXMSW__
#ifdef __WIN32__
    OutputDebugString((LPCTSTR)buffer) ;
#else
    OutputDebugString((const char*) buffer) ;
#endif
#else
    fprintf(stderr, buffer);
#endif
}

#else // wxUSE_MEMORY_TRACING && defined(__WXDEBUG__)
void wxTrace(const char *WXUNUSED(fmt) ...)
{
}

void wxTraceLevel(int WXUNUSED(level), const char *WXUNUSED(fmt) ...)
{
}
#endif

