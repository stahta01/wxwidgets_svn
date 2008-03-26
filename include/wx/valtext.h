/////////////////////////////////////////////////////////////////////////////
// Name:        valtext.h
// Purpose:     wxTextValidator class
// Author:      Julian Smart
// Modified by:
// Created:     29/01/98
// RCS-ID:      $Id$
// Copyright:   (c) 1998 Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_VALTEXT_H_
#define _WX_VALTEXT_H_

#include "wx/defs.h"

#if wxUSE_VALIDATORS && (wxUSE_TEXTCTRL || wxUSE_COMBOBOX)

class WXDLLIMPEXP_FWD_CORE wxTextEntry;

#include "wx/validate.h"

#define wxFILTER_NONE           0x0000
#define wxFILTER_ASCII          0x0001
#define wxFILTER_ALPHA          0x0002
#define wxFILTER_ALPHANUMERIC   0x0004
#define wxFILTER_NUMERIC        0x0008
#define wxFILTER_INCLUDE_LIST   0x0010
#define wxFILTER_EXCLUDE_LIST   0x0020
#define wxFILTER_INCLUDE_CHAR_LIST 0x0040
#define wxFILTER_EXCLUDE_CHAR_LIST 0x0080

class WXDLLIMPEXP_CORE wxTextValidator: public wxValidator
{
DECLARE_DYNAMIC_CLASS(wxTextValidator)
public:

    wxTextValidator(long style = wxFILTER_NONE, wxString *val = 0);
    wxTextValidator(const wxTextValidator& val);

    virtual ~wxTextValidator(){}

    // Make a clone of this validator (or return NULL) - currently necessary
    // if you're passing a reference to a validator.
    // Another possibility is to always pass a pointer to a new validator
    // (so the calling code can use a copy constructor of the relevant class).
    virtual wxObject *Clone() const { return new wxTextValidator(*this); }
    bool Copy(const wxTextValidator& val);

    // Called when the value in the window must be validated.
    // This function can pop up an error message.
    virtual bool Validate(wxWindow *parent);

    // Called to transfer data to the window
    virtual bool TransferToWindow();

    // Called to transfer data from the window
    virtual bool TransferFromWindow();

    // ACCESSORS
    inline long GetStyle() const { return m_validatorStyle; }
    inline void SetStyle(long style) { m_validatorStyle = style; }

    wxTextEntry *GetTextEntry();

    void SetIncludes(const wxArrayString& includes) { m_includes = includes; }
    inline wxArrayString& GetIncludes() { return m_includes; }

    void SetExcludes(const wxArrayString& excludes) { m_excludes = excludes; }
    inline wxArrayString& GetExcludes() { return m_excludes; }

    bool IsInCharIncludes(const wxString& val);
    bool IsNotInCharExcludes(const wxString& val);

    // Filter keystrokes
    void OnChar(wxKeyEvent& event);

DECLARE_EVENT_TABLE()

protected:
    long            m_validatorStyle;
    wxString *      m_stringValue;
    wxArrayString   m_includes;
    wxArrayString   m_excludes;

private:
// Cannot use
//  DECLARE_NO_COPY_CLASS(wxTextValidator)
// because copy constructor is explicitly declared above;
// but no copy assignment operator is defined, so declare
// it private to prevent the compiler from defining it:
    wxTextValidator& operator=(const wxTextValidator&);
};

#endif
  // wxUSE_VALIDATORS && (wxUSE_TEXTCTRL || wxUSE_COMBOBOX)

#endif // _WX_VALTEXT_H_
