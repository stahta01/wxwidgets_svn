/////////////////////////////////////////////////////////////////////////////
// Name:        variant.h
// Purpose:     wxVariant class, container for any type
// Author:      Julian Smart
// Modified by:
// Created:     10/09/98
// RCS-ID:      $Id$
// Copyright:   (c)
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_VARIANT_H_
#define _WX_VARIANT_H_

#ifdef __GNUG__
#pragma interface "variant.h"
#endif

#include "wx/defs.h"
#include "wx/object.h"
#include "wx/string.h"
#include "wx/list.h"

#if USE_IOSTREAMH
#include <iostream.h>
#else
#include <iostream>
#endif

/*
 * wxVariantData stores the actual data in a wxVariant object,
 * to allow it to store any type of data.
 * Derive from this to provide custom data handling.
 *
 * TODO: in order to replace wxPropertyValue, we would need
 * to consider adding constructors that take pointers to C++ variables,
 * or removing that functionality from the wxProperty library.
 * Essentially wxPropertyValue takes on some of the wxValidator functionality
 * by storing pointers and not just actual values, allowing update of C++ data
 * to be handled automatically. Perhaps there's another way of doing this without
 * overloading wxVariant with unnecessary functionality.
 */

class WXDLLEXPORT wxVariantData: public wxObject
{
DECLARE_ABSTRACT_CLASS(wxVariantData)
public:

// Construction & destruction
    wxVariantData() {};

// Override these to provide common functionality
    // Copy to data
    virtual void Copy(wxVariantData& data) = 0;
    virtual bool Eq(wxVariantData& data) const = 0;
    virtual bool Write(ostream& str) const = 0;
    virtual bool Write(wxString& str) const = 0;
    virtual bool Read(istream& str) = 0;
    virtual bool Read(wxString& str) = 0;
    // What type is it? Return a string name.
    virtual wxString GetType() const = 0;
};

/*
 * wxVariant can store any kind of data, but has some basic types
 * built in.
 * NOTE: this eventually should have a reference-counting implementation.
 * PLEASE, if you change it to ref-counting, make sure it doesn't involve bloating
 * this class too much.
 */

class WXDLLEXPORT wxVariant: public wxObject
{
DECLARE_DYNAMIC_CLASS(wxVariant)
public:

// Construction & destruction
    wxVariant();
    wxVariant(double val);
    wxVariant(long val);
    wxVariant(bool val);
    wxVariant(char val);
    wxVariant(const wxString& val);
    wxVariant(const char* val); // Necessary or VC++ assumes bool!
/* Causes ambiguity
    wxVariant(const wxStringList& val);
*/
    wxVariant(const wxList& val); // List of variants
    wxVariant(const wxVariant& variant);
    wxVariant(wxVariantData* data); // User-defined data
    ~wxVariant();

// Generic operators
    // Assignment
    void operator= (const wxVariant& variant);

    // Assignment using data, e.g.
    // myVariant = new wxStringVariantData("hello");
    void operator= (wxVariantData* variantData);
    bool operator== (const wxVariant& variant) const;
    bool operator!= (const wxVariant& variant) const;

// Specific operators
    bool operator== (double value) const;
    bool operator!= (double value) const;
    void operator= (double value) ;
    bool operator== (long value) const;
    bool operator!= (long value) const;
    void operator= (long value) ;
    bool operator== (char value) const;
    bool operator!= (char value) const;
    void operator= (char value) ;
    bool operator== (bool value) const;
    bool operator!= (bool value) const;
    void operator= (bool value) ;
    bool operator== (const wxString& value) const;
    bool operator!= (const wxString& value) const;
    void operator= (const wxString& value) ;
    void operator= (const char* value) ; // Necessary or VC++ assumes bool!
    bool operator== (const wxStringList& value) const;
    bool operator!= (const wxStringList& value) const;
    void operator= (const wxStringList& value) ;
    bool operator== (const wxList& value) const;
    bool operator!= (const wxList& value) const;
    void operator= (const wxList& value) ;

    // Treat a list variant as an array
    wxVariant operator[] (size_t idx) const;
    wxVariant& operator[] (size_t idx) ;

    // Implicit conversion to a wxString
    inline operator wxString () const {  return MakeString(); }
    wxString MakeString() const;

    // Other implicit conversions
    inline operator double () const {  return GetDouble(); }
    inline operator long () const {  return GetLong(); }
    inline operator bool () const {  return GetBool(); }

// Accessors
    // Tests whether there is data
    inline bool IsNull() const { return (m_data == (wxVariantData*) NULL); }

    wxVariantData* GetData() const { return m_data; }
    void SetData(wxVariantData* data) ;

    // Returns a string representing the type of the variant,
    // e.g. "string", "bool", "stringlist", "list", "double", "long"
    wxString GetType() const;

    bool IsType(const wxString& type) const;

    // Return the number of elements in a list
    int GetCount() const;

// Value accessors
    double GetReal() const ;
    inline double GetDouble() const { return GetReal(); };
    long GetInteger() const ;
    inline long GetLong() const { return GetInteger(); };
    char GetChar() const ;
    bool GetBool() const ;
    wxString GetString() const ;
    wxList& GetList() const ;
    wxStringList& GetStringList() const ;

// Operations
    // Make NULL (i.e. delete the data)
    void MakeNull();

    // Make empty list
    void NullList();

    // Append to list
    void Append(const wxVariant& value);

    // Insert at front of list
    void Insert(const wxVariant& value);

    // Returns TRUE if the variant is a member of the list
    bool Member(const wxVariant& value) const;

    // Deletes the nth element of the list
    bool Delete(int item);

    // Clear list
    void ClearList();

// Type conversion
    bool Convert(long* value) const;
    bool Convert(bool* value) const;
    bool Convert(double* value) const;
    bool Convert(wxString* value) const;
    bool Convert(char* value) const;

// Attributes
protected:
    wxVariantData*  m_data;
};

#endif
    // _WX_VARIANT_H_
