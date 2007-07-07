///////////////////////////////////////////////////////////////////////////////
// Name:        wx/msgout.h
// Purpose:     wxMessageOutput class. Shows a message to the user
// Author:      Mattia Barbon
// Modified by:
// Created:     17.07.02
// RCS-ID:      $Id$
// Copyright:   (c) Mattia Barbon
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_MSGOUT_H_
#define _WX_MSGOUT_H_

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include "wx/defs.h"
#include "wx/chartype.h"
#include "wx/strvararg.h"

// ----------------------------------------------------------------------------
// wxMessageOutput is a class abstracting formatted output target, i.e.
// something you can printf() to
// ----------------------------------------------------------------------------

// NB: VC6 has a bug that causes linker errors if you have template methods
//     in a class using __declspec(dllimport). The solution is to split such
//     class into two classes, one that contains the template methods and does
//     *not* use WXDLLIMPEXP_BASE and another class that contains the rest
//     (with DLL linkage).
class wxMessageOutputBase
{
public:
    virtual ~wxMessageOutputBase() { }

    // show a message to the user
    // void Printf(const wxString& format, ...) = 0;
    WX_DEFINE_VARARG_FUNC_VOID(Printf, 1, (const wxFormatString&),
                               DoPrintfWchar, DoPrintfUtf8)
#ifdef __WATCOMC__
    // workaround for http://bugzilla.openwatcom.org/show_bug.cgi?id=351
    WX_VARARG_WATCOM_WORKAROUND(void, Printf, 1, (const wxString&),
                                (wxFormatString(f1)));
    WX_VARARG_WATCOM_WORKAROUND(void, Printf, 1, (const wxCStrData&),
                                (wxFormatString(f1)));
    WX_VARARG_WATCOM_WORKAROUND(void, Printf, 1, (const char*),
                                (wxFormatString(f1)));
    WX_VARARG_WATCOM_WORKAROUND(void, Printf, 1, (const wchar_t*),
                                (wxFormatString(f1)));
#endif

protected:
    // NB: this is pure virtual so that it can be implemented in dllexported
    //     wxMessagOutput class
#if !wxUSE_UTF8_LOCALE_ONLY
    virtual void DoPrintfWchar(const wxChar *format, ...) = 0;
#endif
#if wxUSE_UNICODE_UTF8
    virtual void DoPrintfUtf8(const char *format, ...) = 0;
#endif

    // called by DoPrintf() to output formatted string
    virtual void Output(const wxString& str) = 0;
};

#ifdef __VISUALC__
    // "non dll-interface class 'wxStringPrintfMixin' used as base interface
    // for dll-interface class 'wxString'" -- this is OK in our case
    #pragma warning (disable:4275)
#endif

class WXDLLIMPEXP_BASE wxMessageOutput : public wxMessageOutputBase
{
public:
    virtual ~wxMessageOutput() { }

    // gets the current wxMessageOutput object (may be NULL during
    // initialization or shutdown)
    static wxMessageOutput* Get();

    // sets the global wxMessageOutput instance; returns the previous one
    static wxMessageOutput* Set(wxMessageOutput* msgout);

protected:
#if !wxUSE_UTF8_LOCALE_ONLY
    virtual void DoPrintfWchar(const wxChar *format, ...);
#endif
#if wxUSE_UNICODE_UTF8
    virtual void DoPrintfUtf8(const char *format, ...);
#endif
    virtual void Output(const wxString& str) = 0;

private:
    static wxMessageOutput* ms_msgOut;
};

#ifdef __VISUALC__
    #pragma warning (default:4275)
#endif

// ----------------------------------------------------------------------------
// implementation showing the message to the user in "best" possible way: uses
// native message box if available (currently only under Windows) and stderr
// otherwise; unlike wxMessageOutputMessageBox this class is always safe to use
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_BASE wxMessageOutputBest : public wxMessageOutput
{
public:
    wxMessageOutputBest() { }

protected:
    virtual void Output(const wxString& str);
};

// ----------------------------------------------------------------------------
// implementation which sends output to stderr
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_BASE wxMessageOutputStderr : public wxMessageOutput
{
public:
    wxMessageOutputStderr() { }

protected:
    virtual void Output(const wxString& str);
};

// ----------------------------------------------------------------------------
// implementation which shows output in a message box
// ----------------------------------------------------------------------------

#if wxUSE_GUI

class WXDLLIMPEXP_CORE wxMessageOutputMessageBox : public wxMessageOutput
{
public:
    wxMessageOutputMessageBox() { }

protected:
    virtual void Output(const wxString& str);
};

#endif // wxUSE_GUI

// ----------------------------------------------------------------------------
// implementation using the native way of outputting debug messages
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_BASE wxMessageOutputDebug : public wxMessageOutput
{
public:
    wxMessageOutputDebug() { }

protected:
    virtual void Output(const wxString& str);
};

// ----------------------------------------------------------------------------
// implementation using wxLog (mainly for backwards compatibility)
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_BASE wxMessageOutputLog : public wxMessageOutput
{
public:
    wxMessageOutputLog() { }

protected:
    virtual void Output(const wxString& str);
};

#endif
    // _WX_MSGOUT_H_
