///////////////////////////////////////////////////////////////////////////////
// Name:        wx/string.h
// Purpose:     wxString and wxArrayString classes
// Author:      Vadim Zeitlin
// Modified by:
// Created:     29/01/98
// RCS-ID:      $Id$
// Copyright:   (c) 1998 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/*
    Efficient string class [more or less] compatible with MFC CString,
    wxWidgets version 1 wxString and std::string and some handy functions
    missing from string.h.
*/

#ifndef _WX_WXSTRINGH__
#define _WX_WXSTRINGH__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "string.h"
#endif

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include "wx/defs.h"        // everybody should include this

#if defined(__WXMAC__) || defined(__VISAGECPP__)
    #include <ctype.h>
#endif

#if defined(__VISAGECPP__) && __IBMCPP__ >= 400
   // problem in VACPP V4 with including stdlib.h multiple times
   // strconv includes it anyway
#  include <stdio.h>
#  include <string.h>
#  include <stdarg.h>
#  include <limits.h>
#else
#  include <string.h>
#  include <stdio.h>
#  include <stdarg.h>
#  include <limits.h>
#  include <stdlib.h>
#endif

#ifdef HAVE_STRCASECMP_IN_STRINGS_H
    #include <strings.h>    // for strcasecmp()
#endif // HAVE_STRCASECMP_IN_STRINGS_H

#include "wx/wxchar.h"      // for wxChar
#include "wx/buffer.h"      // for wxCharBuffer
#include "wx/strconv.h"     // for wxConvertXXX() macros and wxMBConv classes

// ---------------------------------------------------------------------------
// macros
// ---------------------------------------------------------------------------

// casts [unfortunately!] needed to call some broken functions which require
// "char *" instead of "const char *"
#define   WXSTRINGCAST (wxChar *)(const wxChar *)
#define   wxCSTRINGCAST (wxChar *)(const wxChar *)
#define   wxMBSTRINGCAST (char *)(const char *)
#define   wxWCSTRINGCAST (wchar_t *)(const wchar_t *)

// implementation only
#define   wxASSERT_VALID_INDEX(i) \
    wxASSERT_MSG( (size_t)(i) <= length(), _T("invalid index in wxString") )

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

#if defined(__VISAGECPP__) && __IBMCPP__ >= 400
// must define this static for VA or else you get multiply defined symbols everywhere
extern const unsigned int wxSTRING_MAXLEN;

#else
// maximum possible length for a string means "take all string" everywhere
//  (as sizeof(StringData) is unknown here, we substract 100)
const unsigned int wxSTRING_MAXLEN = UINT_MAX - 100;

#endif

// ----------------------------------------------------------------------------
// global data
// ----------------------------------------------------------------------------

// global pointer to empty string
extern WXDLLIMPEXP_DATA_BASE(const wxChar*) wxEmptyString;

// ---------------------------------------------------------------------------
// global functions complementing standard C string library replacements for
// strlen() and portable strcasecmp()
//---------------------------------------------------------------------------

// Use wxXXX() functions from wxchar.h instead! These functions are for
// backwards compatibility only.

// checks whether the passed in pointer is NULL and if the string is empty
inline bool IsEmpty(const char *p) { return (!p || !*p); }

// safe version of strlen() (returns 0 if passed NULL pointer)
inline size_t Strlen(const char *psz)
  { return psz ? strlen(psz) : 0; }

// portable strcasecmp/_stricmp
inline int Stricmp(const char *psz1, const char *psz2)
{
#if defined(__VISUALC__) && defined(__WXWINCE__)
  register char c1, c2;
  do {
    c1 = tolower(*psz1++);
    c2 = tolower(*psz2++);
  } while ( c1 && (c1 == c2) );

  return c1 - c2;
#elif defined(__VISUALC__) || ( defined(__MWERKS__) && defined(__INTEL__) )
  return _stricmp(psz1, psz2);
#elif defined(__SC__)
  return _stricmp(psz1, psz2);
#elif defined(__SALFORDC__)
  return stricmp(psz1, psz2);
#elif defined(__BORLANDC__)
  return stricmp(psz1, psz2);
#elif defined(__WATCOMC__)
  return stricmp(psz1, psz2);
#elif defined(__DJGPP__)
  return stricmp(psz1, psz2);
#elif defined(__EMX__)
  return stricmp(psz1, psz2);
#elif defined(__WXPM__)
  return stricmp(psz1, psz2);
#elif defined(HAVE_STRCASECMP_IN_STRING_H) || \
      defined(HAVE_STRCASECMP_IN_STRINGS_H) || \
      defined(__GNUWIN32__)
  return strcasecmp(psz1, psz2);
#elif defined(__MWERKS__) && !defined(__INTEL__)
  register char c1, c2;
  do {
    c1 = tolower(*psz1++);
    c2 = tolower(*psz2++);
  } while ( c1 && (c1 == c2) );

  return c1 - c2;
#else
  // almost all compilers/libraries provide this function (unfortunately under
  // different names), that's why we don't implement our own which will surely
  // be more efficient than this code (uncomment to use):
  /*
    register char c1, c2;
    do {
      c1 = tolower(*psz1++);
      c2 = tolower(*psz2++);
    } while ( c1 && (c1 == c2) );

    return c1 - c2;
  */

  #error  "Please define string case-insensitive compare for your OS/compiler"
#endif  // OS/compiler
}

// return an empty wxString
class WXDLLIMPEXP_BASE wxString; // not yet defined
inline const wxString& wxGetEmptyString() { return *(wxString *)&wxEmptyString; }

#if wxUSE_STL

#include "wx/beforestd.h"
#include <string>
#include "wx/afterstd.h"

#if wxUSE_UNICODE
    #ifdef HAVE_STD_WSTRING
        typedef std::wstring wxStringBase;
    #else
        typedef std::basic_string<wxChar> wxStringBase;
    #endif
#else
    typedef std::string wxStringBase;
#endif

#if (defined(__GNUG__) && (__GNUG__ < 3)) || \
    (defined(_MSC_VER) && (_MSC_VER <= 1200))
    #define wxSTRING_BASE_HASNT_CLEAR
#endif

#else // if !wxUSE_STL

#ifndef HAVE_STD_STRING_COMPARE
    #define HAVE_STD_STRING_COMPARE
#endif

// ---------------------------------------------------------------------------
// string data prepended with some housekeeping info (used by wxString class),
// is never used directly (but had to be put here to allow inlining)
// ---------------------------------------------------------------------------

struct WXDLLIMPEXP_BASE wxStringData
{
  int     nRefs;        // reference count
  size_t  nDataLength,  // actual string length
          nAllocLength; // allocated memory size

  // mimics declaration 'wxChar data[nAllocLength]'
  wxChar* data() const { return (wxChar*)(this + 1); }

  // empty string has a special ref count so it's never deleted
  bool  IsEmpty()   const { return (nRefs == -1); }
  bool  IsShared()  const { return (nRefs > 1);   }

  // lock/unlock
  void  Lock()   { if ( !IsEmpty() ) nRefs++;                    }

  // VC++ will refuse to inline Unlock but profiling shows that it is wrong
#if defined(__VISUALC__) && (__VISUALC__ >= 1200)
  __forceinline
#endif
  // VC++ free must take place in same DLL as allocation when using non dll
  // run-time library (e.g. Multithreaded instead of Multithreaded DLL)
#if defined(__VISUALC__) && defined(_MT) && !defined(_DLL)
  void  Unlock() { if ( !IsEmpty() && --nRefs == 0) Free();  }
  // we must not inline deallocation since allocation is not inlined
  void  Free();
#else
  void  Unlock() { if ( !IsEmpty() && --nRefs == 0) free(this);  }
#endif

  // if we had taken control over string memory (GetWriteBuf), it's
  // intentionally put in invalid state
  void  Validate(bool b)  { nRefs = (b ? 1 : 0); }
  bool  IsValid() const   { return (nRefs != 0); }
};

class WXDLLIMPEXP_BASE wxStringBase
{
#if !wxUSE_STL
friend class WXDLLIMPEXP_BASE wxArrayString;
#endif
public :
  // an 'invalid' value for string index, moved to this place due to a CW bug
  static const size_t npos;
protected:
  // points to data preceded by wxStringData structure with ref count info
  wxChar *m_pchData;

  // accessor to string data
  wxStringData* GetStringData() const { return (wxStringData*)m_pchData - 1; }

  // string (re)initialization functions
    // initializes the string to the empty value (must be called only from
    // ctors, use Reinit() otherwise)
  void Init() { m_pchData = (wxChar *)wxEmptyString; }
    // initializaes the string with (a part of) C-string
  void InitWith(const wxChar *psz, size_t nPos = 0, size_t nLen = npos);
    // as Init, but also frees old data
  void Reinit() { GetStringData()->Unlock(); Init(); }

  // memory allocation
    // allocates memory for string of length nLen
  bool AllocBuffer(size_t nLen);
    // copies data to another string
  bool AllocCopy(wxString&, int, int) const;
    // effectively copies data to string
  bool AssignCopy(size_t, const wxChar *);

  // append a (sub)string
  bool ConcatSelf(size_t nLen, const wxChar *src, size_t nMaxLen);
  bool ConcatSelf(size_t nLen, const wxChar *src)
    { return ConcatSelf(nLen, src, nLen); }

  // functions called before writing to the string: they copy it if there
  // are other references to our data (should be the only owner when writing)
  bool CopyBeforeWrite();
  bool AllocBeforeWrite(size_t);

    // compatibility with wxString
  bool Alloc(size_t nLen);
public:
  // standard types
  typedef wxChar value_type;
  typedef wxChar char_type;
  typedef size_t size_type;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef value_type *iterator;
  typedef const value_type *const_iterator;

  // constructors and destructor
    // ctor for an empty string
  wxStringBase() { Init(); }
    // copy ctor
  wxStringBase(const wxStringBase& stringSrc)
  {
    wxASSERT_MSG( stringSrc.GetStringData()->IsValid(),
                  _T("did you forget to call UngetWriteBuf()?") );

    if ( stringSrc.empty() ) {
      // nothing to do for an empty string
      Init();
    }
    else {
      m_pchData = stringSrc.m_pchData;            // share same data
      GetStringData()->Lock();                    // => one more copy
    }
  }
    // string containing nRepeat copies of ch
  wxStringBase(size_type nRepeat, wxChar ch);
    // ctor takes first nLength characters from C string
    // (default value of npos means take all the string)
  wxStringBase(const wxChar *psz)
      { InitWith(psz, 0, npos); }
  wxStringBase(const wxChar *psz, size_t nLength)
      { InitWith(psz, 0, nLength); }
  wxStringBase(const wxChar *psz, wxMBConv& WXUNUSED(conv), size_t nLength = npos)
      { InitWith(psz, 0, nLength); }
    // take nLen chars starting at nPos
  wxStringBase(const wxStringBase& str, size_t nPos, size_t nLen)
  {
    wxASSERT_MSG( str.GetStringData()->IsValid(),
                  _T("did you forget to call UngetWriteBuf()?") );
    Init();
    size_t strLen = str.length() - nPos; nLen = strLen < nLen ? strLen : nLen;
    InitWith(str.c_str(), nPos, nLen);
  }
    // take all characters from pStart to pEnd
  wxStringBase(const void *pStart, const void *pEnd);

    // dtor is not virtual, this class must not be inherited from!
  ~wxStringBase()
  {
#if defined(__VISUALC__) && (__VISUALC__ >= 1200)
      //RN - according to the above VC++ does indeed inline this,
      //even though it spits out two warnings
      #pragma warning (disable:4714)
#endif

      GetStringData()->Unlock();
  }

#if defined(__VISUALC__) && (__VISUALC__ >= 1200)
    //re-enable inlining warning
    #pragma warning (default:4714)
#endif
  // overloaded assignment
    // from another wxString
  wxStringBase& operator=(const wxStringBase& stringSrc);
    // from a character
  wxStringBase& operator=(wxChar ch);
    // from a C string
  wxStringBase& operator=(const wxChar *psz);

    // return the length of the string
  size_type size() const { return GetStringData()->nDataLength; }
    // return the length of the string
  size_type length() const { return size(); }
    // return the maximum size of the string
  size_type max_size() const { return wxSTRING_MAXLEN; }
    // resize the string, filling the space with c if c != 0
  void resize(size_t nSize, wxChar ch = wxT('\0'));
    // delete the contents of the string
  void clear() { erase(0, npos); }
    // returns true if the string is empty
  bool empty() const { return size() == 0; }
    // inform string about planned change in size
  void reserve(size_t sz) { Alloc(sz); }
  size_type capacity() const { return GetStringData()->nAllocLength; }

  // lib.string.access
    // return the character at position n
  value_type at(size_type n) const
    { wxASSERT_VALID_INDEX( n ); return m_pchData[n]; }
  value_type operator[](size_type n) const { return at(n); }
    // returns the writable character at position n
  reference at(size_type n)
    { wxASSERT_VALID_INDEX( n ); CopyBeforeWrite(); return m_pchData[n]; }
  reference operator[](size_type n)
    { wxASSERT_VALID_INDEX( n ); CopyBeforeWrite(); return m_pchData[n]; }

  // lib.string.modifiers
    // append elements str[pos], ..., str[pos+n]
  wxStringBase& append(const wxStringBase& str, size_t pos, size_t n)
  {
    wxASSERT(pos <= str.length());
    ConcatSelf(n, str.c_str() + pos, str.length() - pos);
    return *this;
  }
    // append a string
  wxStringBase& append(const wxStringBase& str)
    { ConcatSelf(str.length(), str.c_str()); return *this; }
    // append first n (or all if n == npos) characters of sz
  wxStringBase& append(const wxChar *sz)
    { ConcatSelf(wxStrlen(sz), sz); return *this; }
  wxStringBase& append(const wxChar *sz, size_t n)
    { ConcatSelf(n, sz); return *this; }
    // append n copies of ch
  wxStringBase& append(size_t n, wxChar ch);
    // append from first to last
  wxStringBase& append(const_iterator first, const_iterator last)
    { ConcatSelf(last - first, first); return *this; }

    // same as `this_string = str'
  wxStringBase& assign(const wxStringBase& str)
    { return *this = str; }
    // same as ` = str[pos..pos + n]
  wxStringBase& assign(const wxStringBase& str, size_t pos, size_t n)
    { clear(); return append(str, pos, n); }
    // same as `= first n (or all if n == npos) characters of sz'
  wxStringBase& assign(const wxChar *sz)
    { clear(); return append(sz, wxStrlen(sz)); }
  wxStringBase& assign(const wxChar *sz, size_t n)
    { clear(); return append(sz, n); }
    // same as `= n copies of ch'
  wxStringBase& assign(size_t n, wxChar ch)
    { clear(); return append(n, ch); }
    // assign from first to last
  wxStringBase& assign(const_iterator first, const_iterator last)
    { clear(); return append(first, last); }

    // first valid index position
  const_iterator begin() const { return m_pchData; }
    // position one after the last valid one
  const_iterator end() const { return m_pchData + length(); }

  // first valid index position
  iterator begin();
  // position one after the last valid one
  iterator end();

    // insert another string
  wxStringBase& insert(size_t nPos, const wxStringBase& str)
  {
    wxASSERT( str.GetStringData()->IsValid() );
    return insert(nPos, str.c_str(), str.length());
  }
    // insert n chars of str starting at nStart (in str)
  wxStringBase& insert(size_t nPos, const wxStringBase& str, size_t nStart, size_t n)
  {
    wxASSERT( str.GetStringData()->IsValid() );
    wxASSERT( nStart < str.length() );
    size_t strLen = str.length() - nStart;
    n = strLen < n ? strLen : n;
    return insert(nPos, str.c_str() + nStart, n);
  }
    // insert first n (or all if n == npos) characters of sz
  wxStringBase& insert(size_t nPos, const wxChar *sz, size_t n = npos);
    // insert n copies of ch
  wxStringBase& insert(size_t nPos, size_t n, wxChar ch)
    { return insert(nPos, wxStringBase(n, ch)); }
  iterator insert(iterator it, wxChar ch)
    { size_t idx = it - begin(); insert(idx, 1, ch); return begin() + idx; }
  void insert(iterator it, const_iterator first, const_iterator last)
    { insert(it - begin(), first, last - first); }
  void insert(iterator it, size_type n, wxChar ch)
    { insert(it - begin(), n, ch); }

    // delete characters from nStart to nStart + nLen
  wxStringBase& erase(size_type pos = 0, size_type n = npos);
  iterator erase(iterator first, iterator last)
  {
    size_t idx = first - begin();
    erase(idx, last - first);
    return begin() + idx;
  }
  iterator erase(iterator first);

  // explicit conversion to C string (use this with printf()!)
  const wxChar* c_str() const { return m_pchData; }
  const wxChar* data() const { return m_pchData; }

    // replaces the substring of length nLen starting at nStart
  wxStringBase& replace(size_t nStart, size_t nLen, const wxChar* sz);
    // replaces the substring of length nLen starting at nStart
  wxStringBase& replace(size_t nStart, size_t nLen, const wxStringBase& str)
    { return replace(nStart, nLen, str.c_str()); }
    // replaces the substring with nCount copies of ch
  wxStringBase& replace(size_t nStart, size_t nLen, size_t nCount, wxChar ch);
    // replaces a substring with another substring
  wxStringBase& replace(size_t nStart, size_t nLen,
                        const wxStringBase& str, size_t nStart2, size_t nLen2);
    // replaces the substring with first nCount chars of sz
  wxStringBase& replace(size_t nStart, size_t nLen,
                        const wxChar* sz, size_t nCount);
  wxStringBase& replace(iterator first, iterator last, const_pointer s)
    { return replace(first - begin(), last - first, s); }
  wxStringBase& replace(iterator first, iterator last, const_pointer s,
                        size_type n)
    { return replace(first - begin(), last - first, s, n); }
  wxStringBase& replace(iterator first, iterator last, const wxStringBase& s)
    { return replace(first - begin(), last - first, s); }
  wxStringBase& replace(iterator first, iterator last, size_type n, wxChar c)
    { return replace(first - begin(), last - first, n, c); }
  wxStringBase& replace(iterator first, iterator last,
                        const_iterator first1, const_iterator last1)
    { return replace(first - begin(), last - first, first1, last1 - first1); }

    // swap two strings
  void swap(wxStringBase& str);

    // All find() functions take the nStart argument which specifies the
    // position to start the search on, the default value is 0. All functions
    // return npos if there were no match.

    // find a substring
  size_t find(const wxStringBase& str, size_t nStart = 0) const;

  // VC++ 1.5 can't cope with this syntax.
#if !defined(__VISUALC__) || defined(__WIN32__)
    // find first n characters of sz
  size_t find(const wxChar* sz, size_t nStart = 0, size_t n = npos) const;
#endif // VC++ 1.5

    // find the first occurence of character ch after nStart
  size_t find(wxChar ch, size_t nStart = 0) const;

    // rfind() family is exactly like find() but works right to left

    // as find, but from the end
  size_t rfind(const wxStringBase& str, size_t nStart = npos) const;

  // VC++ 1.5 can't cope with this syntax.
    // as find, but from the end
  size_t rfind(const wxChar* sz, size_t nStart = npos,
               size_t n = npos) const;
    // as find, but from the end
  size_t rfind(wxChar ch, size_t nStart = npos) const;

    // find first/last occurence of any character in the set

    // as strpbrk() but starts at nStart, returns npos if not found
  size_t find_first_of(const wxStringBase& str, size_t nStart = 0) const
    { return find_first_of(str.c_str(), nStart); }
    // same as above
  size_t find_first_of(const wxChar* sz, size_t nStart = 0) const;
  size_t find_first_of(const wxChar* sz, size_t nStart, size_t n) const;
    // same as find(char, size_t)
  size_t find_first_of(wxChar c, size_t nStart = 0) const
    { return find(c, nStart); }
    // find the last (starting from nStart) char from str in this string
  size_t find_last_of (const wxStringBase& str, size_t nStart = npos) const
    { return find_last_of(str.c_str(), nStart); }
    // same as above
  size_t find_last_of (const wxChar* sz, size_t nStart = npos) const;
  size_t find_last_of(const wxChar* sz, size_t nStart, size_t n) const;
    // same as above
  size_t find_last_of(wxChar c, size_t nStart = npos) const
    { return rfind(c, nStart); }

    // find first/last occurence of any character not in the set

    // as strspn() (starting from nStart), returns npos on failure
  size_t find_first_not_of(const wxStringBase& str, size_t nStart = 0) const
    { return find_first_not_of(str.c_str(), nStart); }
    // same as above
  size_t find_first_not_of(const wxChar* sz, size_t nStart = 0) const;
  size_t find_first_not_of(const wxChar* sz, size_t nStart, size_t n) const;
    // same as above
  size_t find_first_not_of(wxChar ch, size_t nStart = 0) const;
    //  as strcspn()
  size_t find_last_not_of(const wxStringBase& str, size_t nStart = npos) const
    { return find_last_not_of(str.c_str(), nStart); }
    // same as above
  size_t find_last_not_of(const wxChar* sz, size_t nStart = npos) const;
  size_t find_last_not_of(const wxChar* sz, size_t nStart, size_t n) const;
    // same as above
  size_t find_last_not_of(wxChar ch, size_t nStart = npos) const;

    // All compare functions return -1, 0 or 1 if the [sub]string is less,
    // equal or greater than the compare() argument.

    // just like strcmp()
  int compare(const wxStringBase& str) const
    { return wxStrcmp(c_str(), str.c_str()); }
    // comparison with a substring
  int compare(size_t nStart, size_t nLen, const wxStringBase& str) const;
    // comparison of 2 substrings
  int compare(size_t nStart, size_t nLen,
              const wxStringBase& str, size_t nStart2, size_t nLen2) const;
    // just like strcmp()
  int compare(const wxChar* sz) const
    { return wxStrcmp(c_str(), sz); }
    // substring comparison with first nCount characters of sz
  int compare(size_t nStart, size_t nLen,
              const wxChar* sz, size_t nCount = npos) const;

  size_type copy(wxChar* s, size_type n, size_type pos = 0);

  // substring extraction
  wxStringBase substr(size_t nStart = 0, size_t nLen = npos) const;

      // string += string
  wxStringBase& operator+=(const wxStringBase& s) { return append(s); }
      // string += C string
  wxStringBase& operator+=(const wxChar *psz) { return append(psz); }
      // string += char
  wxStringBase& operator+=(wxChar ch) { return append(1, ch); }
};

#endif // !wxUSE_STL

// ---------------------------------------------------------------------------
// This is (yet another one) String class for C++ programmers. It doesn't use
// any of "advanced" C++ features (i.e. templates, exceptions, namespaces...)
// thus you should be able to compile it with practicaly any C++ compiler.
// This class uses copy-on-write technique, i.e. identical strings share the
// same memory as long as neither of them is changed.
//
// This class aims to be as compatible as possible with the new standard
// std::string class, but adds some additional functions and should be at
// least as efficient than the standard implementation.
//
// Performance note: it's more efficient to write functions which take "const
// String&" arguments than "const char *" if you assign the argument to
// another string.
//
// It was compiled and tested under Win32, Linux (libc 5 & 6), Solaris 5.5.
//
// To do:
//  - ressource support (string tables in ressources)
//  - more wide character (UNICODE) support
//  - regular expressions support
// ---------------------------------------------------------------------------

class WXDLLIMPEXP_BASE wxString : public wxStringBase
{
#if !wxUSE_STL
friend class WXDLLIMPEXP_BASE wxArrayString;
#endif

  // NB: special care was taken in arranging the member functions in such order
  //     that all inline functions can be effectively inlined, verify that all
  //     performace critical functions are still inlined if you change order!
private:
  // if we hadn't made these operators private, it would be possible to
  // compile "wxString s; s = 17;" without any warnings as 17 is implicitly
  // converted to char in C and we do have operator=(char)
  //
  // NB: we don't need other versions (short/long and unsigned) as attempt
  //     to assign another numeric type to wxString will now result in
  //     ambiguity between operator=(char) and operator=(int)
  wxString& operator=(int);

  // these methods are not implemented - there is _no_ conversion from int to
  // string, you're doing something wrong if the compiler wants to call it!
  //
  // try `s << i' or `s.Printf("%d", i)' instead
  wxString(int);

public:
  // constructors and destructor
    // ctor for an empty string
  wxString() : wxStringBase() { }
    // copy ctor
  wxString(const wxStringBase& stringSrc) : wxStringBase(stringSrc) { }
  wxString(const wxString& stringSrc) : wxStringBase(stringSrc) { }
    // string containing nRepeat copies of ch
  wxString(wxChar ch, size_t nRepeat = 1)
      : wxStringBase(nRepeat, ch) { }
  wxString(size_t nRepeat, wxChar ch)
      : wxStringBase(nRepeat, ch) { }
    // ctor takes first nLength characters from C string
    // (default value of npos means take all the string)
  wxString(const wxChar *psz)
      : wxStringBase(psz ? psz : wxT("")) { }
  wxString(const wxChar *psz, size_t nLength)
      : wxStringBase(psz, nLength) { }
  wxString(const wxChar *psz, wxMBConv& WXUNUSED(conv), size_t nLength = npos)
      : wxStringBase(psz, nLength == npos ? wxStrlen(psz) : nLength) { }

#if wxUSE_UNICODE
    // from multibyte string
  wxString(const char *psz, wxMBConv& conv, size_t nLength = npos);
    // from wxWCharBuffer (i.e. return from wxGetString)
  wxString(const wxWCharBuffer& psz) : wxStringBase(psz.data()) { }
#else // ANSI
    // from C string (for compilers using unsigned char)
  wxString(const unsigned char* psz, size_t nLength = npos)
      : wxStringBase((const char*)psz, nLength) { }

#if wxUSE_WCHAR_T
    // from wide (Unicode) string
  wxString(const wchar_t *pwz, wxMBConv& conv = wxConvLibc, size_t nLength = npos);
#endif // !wxUSE_WCHAR_T

    // from wxCharBuffer
  wxString(const wxCharBuffer& psz)
      : wxStringBase(psz, npos) { }
#endif // Unicode/ANSI

  // generic attributes & operations
    // as standard strlen()
  size_t Len() const { return length(); }
    // string contains any characters?
  bool IsEmpty() const { return empty(); }
    // empty string is "FALSE", so !str will return TRUE
  bool operator!() const { return IsEmpty(); }
    // truncate the string to given length
  wxString& Truncate(size_t uiLen);
    // empty string contents
  void Empty()
  {
    Truncate(0);

    wxASSERT_MSG( IsEmpty(), _T("string not empty after call to Empty()?") );
  }
    // empty the string and free memory
  void Clear()
  {
    wxString tmp(wxEmptyString);
    swap(tmp);
  }

  // contents test
    // Is an ascii value
  bool IsAscii() const;
    // Is a number
  bool IsNumber() const;
    // Is a word
  bool IsWord() const;

  // data access (all indexes are 0 based)
    // read access
    wxChar  GetChar(size_t n) const
      { return operator[](n); }
    // read/write access
    wxChar& GetWritableChar(size_t n)
      { return operator[](n); }
    // write access
    void  SetChar(size_t n, wxChar ch)
      { operator[](n) = ch; }

    // get last character
    wxChar  Last() const
      {
          wxASSERT_MSG( !IsEmpty(), _T("wxString: index out of bounds") );

          return operator[](length() - 1);
      }

    // get writable last character
    wxChar& Last()
      {
          wxASSERT_MSG( !IsEmpty(), _T("wxString: index out of bounds") );
          return operator[](length() - 1);
      }

    /*
        So why do we have all these overloaded operator[]s? A bit of history:
        initially there was only one of them, taking size_t. Then people
        started complaining because they wanted to use ints as indices (I
        wonder why) and compilers were giving warnings about it, so we had to
        add the operator[](int). Then it became apparent that you couldn't
        write str[0] any longer because there was ambiguity between two
        overloads and so you now had to write str[0u] (or, of course, use the
        explicit casts to either int or size_t but nobody did this).

        Finally, someone decided to compile wxWin on an Alpha machine and got
        a surprize: str[0u] didn't compile there because it is of type
        unsigned int and size_t is unsigned _long_ on Alpha and so there was
        ambiguity between converting uint to int or ulong. To fix this one we
        now add operator[](uint) for the machines where size_t is not already
        the same as unsigned int - hopefully this fixes the problem (for some
        time)

        The only real fix is, of course, to remove all versions but the one
        taking size_t...
     */

    // operator version of GetChar
    wxChar  operator[](int n) const
      { return wxStringBase::operator[](n); }
    wxChar& operator[](size_type n)
      { return wxStringBase::operator[](n); }
    wxChar operator[](size_type n) const
      { return wxStringBase::operator[](n); }
#ifndef wxSIZE_T_IS_UINT
    // operator version of GetChar
    wxChar operator[](unsigned int n) const
      { return wxStringBase::operator[](n); }

    // operator version of GetWriteableChar
    wxChar& operator[](unsigned int n)
      { return wxStringBase::operator[](n); }
#endif // size_t != unsigned int

    // implicit conversion to C string
    operator const wxChar*() const { return c_str(); }

    // identical to c_str(), for wxWin 1.6x compatibility
    const wxChar* wx_str()  const { return c_str(); }
    // identical to c_str(), for MFC compatibility
    const wxChar* GetData() const { return c_str(); }

    // conversion to/from plain (i.e. 7 bit) ASCII: this is useful for
    // converting numbers or strings which are certain not to contain special
    // chars (typically system functions, X atoms, environment variables etc.)
    //
    // the behaviour of these functions with the strings containing anything
    // else than 7 bit ASCII characters is undefined, use at your own risk.
#if wxUSE_UNICODE
    static wxString FromAscii(const char *ascii);  // string
    static wxString FromAscii(const char ascii);   // char
    const wxCharBuffer ToAscii() const;
#else // ANSI
    static wxString FromAscii(const char *ascii) { return wxString( ascii ); }
    static wxString FromAscii(const char ascii) { return wxString( ascii ); }
    const char *ToAscii() const { return c_str(); }
#endif // Unicode/!Unicode

    // conversions with (possible) format conversions: have to return a
    // buffer with temporary data
    //
    // the functions defined (in either Unicode or ANSI) mode are mb_str() to
    // return an ANSI (multibyte) string, wc_str() to return a wide string and
    // fn_str() to return a string which should be used with the OS APIs
    // accepting the file names. The return value is always the same, but the
    // type differs because a function may either return pointer to the buffer
    // directly or have to use intermediate buffer for translation.
#if wxUSE_UNICODE
    const wxCharBuffer mb_str(wxMBConv& conv = wxConvLibc) const
        { return conv.cWC2MB(c_str()); }

    const wxWX2MBbuf mbc_str() const { return mb_str(*wxConvCurrent); }

    const wxChar* wc_str() const { return c_str(); }

    // for compatibility with !wxUSE_UNICODE version
    const wxChar* wc_str(wxMBConv& WXUNUSED(conv)) const { return c_str(); }

#if wxMBFILES
    const wxCharBuffer fn_str() const { return mb_str(wxConvFile); }
#else // !wxMBFILES
    const wxChar* fn_str() const { return c_str(); }
#endif // wxMBFILES/!wxMBFILES
#else // ANSI
    const wxChar* mb_str() const { return c_str(); }

    // for compatibility with wxUSE_UNICODE version
    const wxChar* mb_str(wxMBConv& WXUNUSED(conv)) const { return c_str(); }

    const wxWX2MBbuf mbc_str() const { return mb_str(); }

#if wxUSE_WCHAR_T
    const wxWCharBuffer wc_str(wxMBConv& conv) const
        { return conv.cMB2WC(c_str()); }
#endif // wxUSE_WCHAR_T

    const wxChar* fn_str() const { return c_str(); }
#endif // Unicode/ANSI

  // overloaded assignment
    // from another wxString
  wxString& operator=(const wxStringBase& stringSrc)
    { return (wxString&)wxStringBase::operator=(stringSrc); }
    // from a character
  wxString& operator=(wxChar ch)
    { return (wxString&)wxStringBase::operator=(ch); }
    // from a C string
  wxString& operator=(const wxChar *psz)
    { return (wxString&)wxStringBase::operator=(psz); }
#if wxUSE_UNICODE
    // from wxWCharBuffer
  wxString& operator=(const wxWCharBuffer& psz)
    { (void) operator=((const wchar_t *)psz); return *this; }
#else // ANSI
    // from another kind of C string
  wxString& operator=(const unsigned char* psz);
#if wxUSE_WCHAR_T
    // from a wide string
  wxString& operator=(const wchar_t *pwz);
#endif
    // from wxCharBuffer
  wxString& operator=(const wxCharBuffer& psz)
    { (void) operator=((const char *)psz); return *this; }
#endif // Unicode/ANSI

  // string concatenation
    // in place concatenation
    /*
        Concatenate and return the result. Note that the left to right
        associativity of << allows to write things like "str << str1 << str2
        << ..." (unlike with +=)
     */
      // string += string
  wxString& operator<<(const wxString& s)
  {
#if !wxUSE_STL
    wxASSERT_MSG( s.GetStringData()->IsValid(),
                  _T("did you forget to call UngetWriteBuf()?") );
#endif

    append(s);
    return *this;
  }
      // string += C string
  wxString& operator<<(const wxChar *psz)
    { append(psz); return *this; }
      // string += char
  wxString& operator<<(wxChar ch) { append(1, ch); return *this; }

      // string += buffer (i.e. from wxGetString)
#if wxUSE_UNICODE
  wxString& operator<<(const wxWCharBuffer& s)
    { (void)operator<<((const wchar_t *)s); return *this; }
  void operator+=(const wxWCharBuffer& s)
    { (void)operator<<((const wchar_t *)s); }
#else // !wxUSE_UNICODE
  wxString& operator<<(const wxCharBuffer& s)
    { (void)operator<<((const char *)s); return *this; }
  void operator+=(const wxCharBuffer& s)
    { (void)operator<<((const char *)s); }
#endif // wxUSE_UNICODE/!wxUSE_UNICODE

    // string += C string
  wxString& Append(const wxString& s)
    {
        // test for IsEmpty() to share the string if possible
        if ( IsEmpty() )
            *this = s;
        else
            append(s);
        return *this;
    }
  wxString& Append(const wxChar* psz)
    { append(psz); return *this; }
    // append count copies of given character
  wxString& Append(wxChar ch, size_t count = 1u)
    { append(count, ch); return *this; }
  wxString& Append(const wxChar* psz, size_t nLen)
    { append(psz, nLen); return *this; }

    // prepend a string, return the string itself
  wxString& Prepend(const wxString& str)
    { *this = str + *this; return *this; }

    // non-destructive concatenation
      //
  friend wxString WXDLLIMPEXP_BASE operator+(const wxString& string1,  const wxString& string2);
      //
  friend wxString WXDLLIMPEXP_BASE operator+(const wxString& string, wxChar ch);
      //
  friend wxString WXDLLIMPEXP_BASE operator+(wxChar ch, const wxString& string);
      //
  friend wxString WXDLLIMPEXP_BASE operator+(const wxString& string, const wxChar *psz);
      //
  friend wxString WXDLLIMPEXP_BASE operator+(const wxChar *psz, const wxString& string);

  // stream-like functions
      // insert an int into string
  wxString& operator<<(int i)
    { return (*this) << Format(_T("%d"), i); }
      // insert an unsigned int into string
  wxString& operator<<(unsigned int ui)
    { return (*this) << Format(_T("%u"), ui); }
      // insert a long into string
  wxString& operator<<(long l)
    { return (*this) << Format(_T("%ld"), l); }
      // insert an unsigned long into string
  wxString& operator<<(unsigned long ul)
    { return (*this) << Format(_T("%lu"), ul); }
      // insert a float into string
  wxString& operator<<(float f)
    { return (*this) << Format(_T("%f"), f); }
      // insert a double into string
  wxString& operator<<(double d)
    { return (*this) << Format(_T("%g"), d); }

  // string comparison
    // case-sensitive comparison (returns a value < 0, = 0 or > 0)
  int Cmp(const wxChar *psz) const { return wxStrcmp(c_str(), psz); }
    // same as Cmp() but not case-sensitive
  int CmpNoCase(const wxChar *psz) const { return wxStricmp(c_str(), psz); }
    // test for the string equality, either considering case or not
    // (if compareWithCase then the case matters)
  bool IsSameAs(const wxChar *psz, bool compareWithCase = TRUE) const
    { return (compareWithCase ? Cmp(psz) : CmpNoCase(psz)) == 0; }
    // comparison with a signle character: returns TRUE if equal
  bool IsSameAs(wxChar c, bool compareWithCase = TRUE) const
    {
      return (length() == 1) && (compareWithCase ? GetChar(0u) == c
                              : wxToupper(GetChar(0u)) == wxToupper(c));
    }

  // simple sub-string extraction
      // return substring starting at nFirst of length nCount (or till the end
      // if nCount = default value)
  wxString Mid(size_t nFirst, size_t nCount = npos) const;

      // operator version of Mid()
  wxString  operator()(size_t start, size_t len) const
    { return Mid(start, len); }

      // check that the string starts with prefix and return the rest of the
      // string in the provided pointer if it is not NULL, otherwise return
      // FALSE
  bool StartsWith(const wxChar *prefix, wxString *rest = NULL) const;

      // get first nCount characters
  wxString Left(size_t nCount) const;
      // get last nCount characters
  wxString Right(size_t nCount) const;
      // get all characters before the first occurance of ch
      // (returns the whole string if ch not found)
  wxString BeforeFirst(wxChar ch) const;
      // get all characters before the last occurence of ch
      // (returns empty string if ch not found)
  wxString BeforeLast(wxChar ch) const;
      // get all characters after the first occurence of ch
      // (returns empty string if ch not found)
  wxString AfterFirst(wxChar ch) const;
      // get all characters after the last occurence of ch
      // (returns the whole string if ch not found)
  wxString AfterLast(wxChar ch) const;

    // for compatibility only, use more explicitly named functions above
  wxString Before(wxChar ch) const { return BeforeLast(ch); }
  wxString After(wxChar ch) const { return AfterFirst(ch); }

  // case conversion
      // convert to upper case in place, return the string itself
  wxString& MakeUpper();
      // convert to upper case, return the copy of the string
      // Here's something to remember: BC++ doesn't like returns in inlines.
  wxString Upper() const ;
      // convert to lower case in place, return the string itself
  wxString& MakeLower();
      // convert to lower case, return the copy of the string
  wxString Lower() const ;

  // trimming/padding whitespace (either side) and truncating
      // remove spaces from left or from right (default) side
  wxString& Trim(bool bFromRight = TRUE);
      // add nCount copies chPad in the beginning or at the end (default)
  wxString& Pad(size_t nCount, wxChar chPad = wxT(' '), bool bFromRight = TRUE);

  // searching and replacing
      // searching (return starting index, or -1 if not found)
  int Find(wxChar ch, bool bFromEnd = FALSE) const;   // like strchr/strrchr
      // searching (return starting index, or -1 if not found)
  int Find(const wxChar *pszSub) const;               // like strstr
      // replace first (or all of bReplaceAll) occurences of substring with
      // another string, returns the number of replacements made
  size_t Replace(const wxChar *szOld,
                 const wxChar *szNew,
                 bool bReplaceAll = TRUE);

    // check if the string contents matches a mask containing '*' and '?'
  bool Matches(const wxChar *szMask) const;

    // conversion to numbers: all functions return TRUE only if the whole
    // string is a number and put the value of this number into the pointer
    // provided, the base is the numeric base in which the conversion should be
    // done and must be comprised between 2 and 36 or be 0 in which case the
    // standard C rules apply (leading '0' => octal, "0x" => hex)
        // convert to a signed integer
    bool ToLong(long *val, int base = 10) const;
        // convert to an unsigned integer
    bool ToULong(unsigned long *val, int base = 10) const;
        // convert to a double
    bool ToDouble(double *val) const;

  // formated input/output
    // as sprintf(), returns the number of characters written or < 0 on error
    // (take 'this' into account in attribute parameter count)
  int Printf(const wxChar *pszFormat, ...) ATTRIBUTE_PRINTF_2;
    // as vprintf(), returns the number of characters written or < 0 on error
  int PrintfV(const wxChar* pszFormat, va_list argptr);

    // returns the string containing the result of Printf() to it
  static wxString Format(const wxChar *pszFormat, ...) ATTRIBUTE_PRINTF_1;
    // the same as above, but takes a va_list
  static wxString FormatV(const wxChar *pszFormat, va_list argptr);

  // raw access to string memory
    // ensure that string has space for at least nLen characters
    // only works if the data of this string is not shared
  bool Alloc(size_t nLen) { reserve(nLen); /*return capacity() >= nLen;*/ return true; }
    // minimize the string's memory
    // only works if the data of this string is not shared
  bool Shrink();
#if !wxUSE_STL
    // get writable buffer of at least nLen bytes. Unget() *must* be called
    // a.s.a.p. to put string back in a reasonable state!
  wxChar *GetWriteBuf(size_t nLen);
    // call this immediately after GetWriteBuf() has been used
  void UngetWriteBuf();
  void UngetWriteBuf(size_t nLen);
#endif

  // wxWidgets version 1 compatibility functions

  // use Mid()
  wxString SubString(size_t from, size_t to) const
      { return Mid(from, (to - from + 1)); }
    // values for second parameter of CompareTo function
  enum caseCompare {exact, ignoreCase};
    // values for first parameter of Strip function
  enum stripType {leading = 0x1, trailing = 0x2, both = 0x3};

  // use Printf()
  // (take 'this' into account in attribute parameter count)
  int sprintf(const wxChar *pszFormat, ...) ATTRIBUTE_PRINTF_2;

    // use Cmp()
  inline int CompareTo(const wxChar* psz, caseCompare cmp = exact) const
    { return cmp == exact ? Cmp(psz) : CmpNoCase(psz); }

    // use Len
  size_t Length() const { return length(); }
    // Count the number of characters
  int Freq(wxChar ch) const;
    // use MakeLower
  void LowerCase() { MakeLower(); }
    // use MakeUpper
  void UpperCase() { MakeUpper(); }
    // use Trim except that it doesn't change this string
  wxString Strip(stripType w = trailing) const;

    // use Find (more general variants not yet supported)
  size_t Index(const wxChar* psz) const { return Find(psz); }
  size_t Index(wxChar ch)         const { return Find(ch);  }
    // use Truncate
  wxString& Remove(size_t pos) { return Truncate(pos); }
  wxString& RemoveLast(size_t n = 1) { return Truncate(length() - n); }

  wxString& Remove(size_t nStart, size_t nLen)
      { return (wxString&)erase( nStart, nLen ); }

    // use Find()
  int First( const wxChar ch ) const { return Find(ch); }
  int First( const wxChar* psz ) const { return Find(psz); }
  int First( const wxString &str ) const { return Find(str); }
  int Last( const wxChar ch ) const { return Find(ch, TRUE); }
  bool Contains(const wxString& str) const { return Find(str) != -1; }

    // use IsEmpty()
  bool IsNull() const { return IsEmpty(); }

  // std::string compatibility functions

    // take nLen chars starting at nPos
  wxString(const wxString& str, size_t nPos, size_t nLen)
      : wxStringBase(str, nPos, nLen) { }
    // take all characters from pStart to pEnd
  wxString(const void *pStart, const void *pEnd)
      : wxStringBase((const wxChar*)pStart, (const wxChar*)pEnd) { }
#if wxUSE_STL
  wxString(const_iterator first, const_iterator last)
      : wxStringBase(first, last) { }
#endif

  // lib.string.modifiers
    // append elements str[pos], ..., str[pos+n]
  wxString& append(const wxString& str, size_t pos, size_t n)
    { return (wxString&)wxStringBase::append(str, pos, n); }
    // append a string
  wxString& append(const wxString& str)
    { return (wxString&)wxStringBase::append(str); }
    // append first n (or all if n == npos) characters of sz
  wxString& append(const wxChar *sz)
    { return (wxString&)wxStringBase::append(sz); }
  wxString& append(const wxChar *sz, size_t n)
    { return (wxString&)wxStringBase::append(sz, n); }
    // append n copies of ch
  wxString& append(size_t n, wxChar ch)
    { return (wxString&)wxStringBase::append(n, ch); }
    // append from first to last
  wxString& append(const_iterator first, const_iterator last)
    { return (wxString&)wxStringBase::append(first, last); }

    // same as `this_string = str'
  wxString& assign(const wxString& str)
    { return (wxString&)wxStringBase::assign(str); }
    // same as ` = str[pos..pos + n]
  wxString& assign(const wxString& str, size_t pos, size_t n)
    { return (wxString&)wxStringBase::assign(str, pos, n); }
    // same as `= first n (or all if n == npos) characters of sz'
  wxString& assign(const wxChar *sz)
    { return (wxString&)wxStringBase::assign(sz); }
  wxString& assign(const wxChar *sz, size_t n)
    { return (wxString&)wxStringBase::assign(sz, n); }
    // same as `= n copies of ch'
  wxString& assign(size_t n, wxChar ch)
    { return (wxString&)wxStringBase::assign(n, ch); }
    // assign from first to last
  wxString& assign(const_iterator first, const_iterator last)
    { return (wxString&)wxStringBase::assign(first, last); }

    // string comparison
#ifndef HAVE_STD_STRING_COMPARE
  int compare(const wxStringBase& str) const;
    // comparison with a substring
  int compare(size_t nStart, size_t nLen, const wxStringBase& str) const;
    // comparison of 2 substrings
  int compare(size_t nStart, size_t nLen,
              const wxStringBase& str, size_t nStart2, size_t nLen2) const;
    // just like strcmp()
  int compare(const wxChar* sz) const;
    // substring comparison with first nCount characters of sz
  int compare(size_t nStart, size_t nLen,
              const wxChar* sz, size_t nCount = npos) const;
#endif // !defined HAVE_STD_STRING_COMPARE

    // insert another string
  wxString& insert(size_t nPos, const wxString& str)
    { return (wxString&)wxStringBase::insert(nPos, str); }
    // insert n chars of str starting at nStart (in str)
  wxString& insert(size_t nPos, const wxString& str, size_t nStart, size_t n)
    { return (wxString&)wxStringBase::insert(nPos, str, nStart, n); }
    // insert first n (or all if n == npos) characters of sz
  wxString& insert(size_t nPos, const wxChar *sz)
    { return (wxString&)wxStringBase::insert(nPos, sz); }
  wxString& insert(size_t nPos, const wxChar *sz, size_t n)
    { return (wxString&)wxStringBase::insert(nPos, sz, n); }
    // insert n copies of ch
  wxString& insert(size_t nPos, size_t n, wxChar ch)
    { return (wxString&)wxStringBase::insert(nPos, n, ch); }
  iterator insert(iterator it, wxChar ch)
    { return wxStringBase::insert(it, ch); }
  void insert(iterator it, const_iterator first, const_iterator last)
    { wxStringBase::insert(it, first, last); }
  void insert(iterator it, size_type n, wxChar ch)
    { wxStringBase::insert(it, n, ch); }

    // delete characters from nStart to nStart + nLen
  wxString& erase(size_type pos = 0, size_type n = npos)
    { return (wxString&)wxStringBase::erase(pos, n); }
  iterator erase(iterator first, iterator last)
    { return wxStringBase::erase(first, last); }
  iterator erase(iterator first)
    { return wxStringBase::erase(first); }

#ifdef wxSTRING_BASE_HASNT_CLEAR
  void clear() { erase(); }
#endif

    // replaces the substring of length nLen starting at nStart
  wxString& replace(size_t nStart, size_t nLen, const wxChar* sz)
    { return (wxString&)wxStringBase::replace(nStart, nLen, sz); }
    // replaces the substring of length nLen starting at nStart
  wxString& replace(size_t nStart, size_t nLen, const wxString& str)
    { return (wxString&)wxStringBase::replace(nStart, nLen, str); }
    // replaces the substring with nCount copies of ch
  wxString& replace(size_t nStart, size_t nLen, size_t nCount, wxChar ch)
    { return (wxString&)wxStringBase::replace(nStart, nLen, nCount, ch); }
    // replaces a substring with another substring
  wxString& replace(size_t nStart, size_t nLen,
                    const wxString& str, size_t nStart2, size_t nLen2)
    { return (wxString&)wxStringBase::replace(nStart, nLen, str,
                                              nStart2, nLen2); }
     // replaces the substring with first nCount chars of sz
  wxString& replace(size_t nStart, size_t nLen,
                    const wxChar* sz, size_t nCount)
    { return (wxString&)wxStringBase::replace(nStart, nLen, sz, nCount); }
  wxString& replace(iterator first, iterator last, const_pointer s)
    { return (wxString&)wxStringBase::replace(first, last, s); }
  wxString& replace(iterator first, iterator last, const_pointer s,
                    size_type n)
    { return (wxString&)wxStringBase::replace(first, last, s, n); }
  wxString& replace(iterator first, iterator last, const wxString& s)
    { return (wxString&)wxStringBase::replace(first, last, s); }
  wxString& replace(iterator first, iterator last, size_type n, wxChar c)
    { return (wxString&)wxStringBase::replace(first, last, n, c); }
  wxString& replace(iterator first, iterator last,
                    const_iterator first1, const_iterator last1)
    { return (wxString&)wxStringBase::replace(first, last, first1, last1); }

      // string += string
  wxString& operator+=(const wxString& s)
    { return (wxString&)wxStringBase::operator+=(s); }
      // string += C string
  wxString& operator+=(const wxChar *psz)
    { return (wxString&)wxStringBase::operator+=(psz); }
      // string += char
  wxString& operator+=(wxChar ch)
    { return (wxString&)wxStringBase::operator+=(ch); }
};

// define wxArrayString, for compatibility
#if WXWIN_COMPATIBILITY_2_4 && !wxUSE_STL
    #include "wx/arrstr.h"
#endif

// ----------------------------------------------------------------------------
// wxStringBuffer: a tiny class allowing to get a writable pointer into string
// ----------------------------------------------------------------------------

#if wxUSE_STL

class WXDLLIMPEXP_BASE wxStringBuffer
{
public:
    wxStringBuffer(wxString& str, size_t lenWanted = 1024)
        : m_str(str), m_buf(lenWanted)
        { }

    ~wxStringBuffer() { m_str.assign(m_buf.data(), wxStrlen(m_buf.data())); }

    operator wxChar*() { return m_buf.data(); }

private:
    wxString& m_str;
#if wxUSE_UNICODE
    wxWCharBuffer m_buf;
#else
    wxCharBuffer m_buf;
#endif

    DECLARE_NO_COPY_CLASS(wxStringBuffer)
};

class WXDLLIMPEXP_BASE wxStringBufferLength
{
public:
    wxStringBufferLength(wxString& str, size_t lenWanted = 1024)
        : m_str(str), m_buf(lenWanted), m_len(0), m_lenSet(false)
        { }

    ~wxStringBufferLength()
    {
        wxASSERT(m_lenSet);
        m_str.assign(m_buf.data(), m_len);
    }

    operator wxChar*() { return m_buf.data(); }
    void SetLength(size_t length) { m_len = length; m_lenSet = true; }

private:
    wxString& m_str;
#if wxUSE_UNICODE
    wxWCharBuffer m_buf;
#else
    wxCharBuffer  m_buf;
#endif
    size_t        m_len;
    bool          m_lenSet;

    DECLARE_NO_COPY_CLASS(wxStringBufferLength)
};

#else // if !wxUSE_STL

class WXDLLIMPEXP_BASE wxStringBuffer
{
public:
    wxStringBuffer(wxString& str, size_t lenWanted = 1024)
        : m_str(str), m_buf(NULL)
        { m_buf = m_str.GetWriteBuf(lenWanted); }

    ~wxStringBuffer() { m_str.UngetWriteBuf(); }

    operator wxChar*() const { return m_buf; }

private:
    wxString& m_str;
    wxChar   *m_buf;

    DECLARE_NO_COPY_CLASS(wxStringBuffer)
};

class WXDLLIMPEXP_BASE wxStringBufferLength
{
public:
    wxStringBufferLength(wxString& str, size_t lenWanted = 1024)
        : m_str(str), m_buf(NULL), m_len(0), m_lenSet(false)
        { m_buf = m_str.GetWriteBuf(lenWanted); }

    ~wxStringBufferLength()
    {
        wxASSERT(m_lenSet);
        m_str.UngetWriteBuf(m_len);
    }

    operator wxChar*() const { return m_buf; }
    void SetLength(size_t length) { m_len = length; m_lenSet = true; }

private:
    wxString& m_str;
    wxChar   *m_buf;
    size_t    m_len;
    bool      m_lenSet;

    DECLARE_NO_COPY_CLASS(wxStringBufferLength)
};

#endif // !wxUSE_STL

// ---------------------------------------------------------------------------
// wxString comparison functions: operator versions are always case sensitive
// ---------------------------------------------------------------------------

#if wxUSE_STL

inline bool operator==(const wxString& s1, const wxString& s2)
    { return s1.compare(s2) == 0; }
inline bool operator==(const wxString& s1, const wxChar  * s2)
    { return s1.compare(s2) == 0; }
inline bool operator==(const wxChar  * s1, const wxString& s2)
    { return s2.compare(s1) == 0; }
inline bool operator!=(const wxString& s1, const wxString& s2)
    { return s1.compare(s2) != 0; }
inline bool operator!=(const wxString& s1, const wxChar  * s2)
    { return s1.compare(s2) != 0; }
inline bool operator!=(const wxChar  * s1, const wxString& s2)
    { return s2.compare(s1) != 0; }
inline bool operator< (const wxString& s1, const wxString& s2)
    { return s1.compare(s2) <  0; }
inline bool operator< (const wxString& s1, const wxChar  * s2)
    { return s1.compare(s2) <  0; }
inline bool operator< (const wxChar  * s1, const wxString& s2)
    { return s2.compare(s1) >  0; }
inline bool operator> (const wxString& s1, const wxString& s2)
    { return s1.compare(s2) >  0; }
inline bool operator> (const wxString& s1, const wxChar  * s2)
    { return s1.compare(s2) >  0; }
inline bool operator> (const wxChar  * s1, const wxString& s2)
    { return s2.compare(s1) <  0; }
inline bool operator<=(const wxString& s1, const wxString& s2)
    { return s1.compare(s2) <= 0; }
inline bool operator<=(const wxString& s1, const wxChar  * s2)
    { return s1.compare(s2) <= 0; }
inline bool operator<=(const wxChar  * s1, const wxString& s2)
    { return s2.compare(s1) >= 0; }
inline bool operator>=(const wxString& s1, const wxString& s2)
    { return s1.compare(s2) >= 0; }
inline bool operator>=(const wxString& s1, const wxChar  * s2)
    { return s1.compare(s2) >= 0; }
inline bool operator>=(const wxChar  * s1, const wxString& s2)
    { return s2.compare(s1) <= 0; }

#else // if !wxUSE_STL

inline bool operator==(const wxString& s1, const wxString& s2)
    { return (s1.Len() == s2.Len()) && (s1.Cmp(s2) == 0); }
inline bool operator==(const wxString& s1, const wxChar  * s2)
    { return s1.Cmp(s2) == 0; }
inline bool operator==(const wxChar  * s1, const wxString& s2)
    { return s2.Cmp(s1) == 0; }
inline bool operator!=(const wxString& s1, const wxString& s2)
    { return (s1.Len() != s2.Len()) || (s1.Cmp(s2) != 0); }
inline bool operator!=(const wxString& s1, const wxChar  * s2)
    { return s1.Cmp(s2) != 0; }
inline bool operator!=(const wxChar  * s1, const wxString& s2)
    { return s2.Cmp(s1) != 0; }
inline bool operator< (const wxString& s1, const wxString& s2)
    { return s1.Cmp(s2) < 0; }
inline bool operator< (const wxString& s1, const wxChar  * s2)
    { return s1.Cmp(s2) <  0; }
inline bool operator< (const wxChar  * s1, const wxString& s2)
    { return s2.Cmp(s1) >  0; }
inline bool operator> (const wxString& s1, const wxString& s2)
    { return s1.Cmp(s2) >  0; }
inline bool operator> (const wxString& s1, const wxChar  * s2)
    { return s1.Cmp(s2) >  0; }
inline bool operator> (const wxChar  * s1, const wxString& s2)
    { return s2.Cmp(s1) <  0; }
inline bool operator<=(const wxString& s1, const wxString& s2)
    { return s1.Cmp(s2) <= 0; }
inline bool operator<=(const wxString& s1, const wxChar  * s2)
    { return s1.Cmp(s2) <= 0; }
inline bool operator<=(const wxChar  * s1, const wxString& s2)
    { return s2.Cmp(s1) >= 0; }
inline bool operator>=(const wxString& s1, const wxString& s2)
    { return s1.Cmp(s2) >= 0; }
inline bool operator>=(const wxString& s1, const wxChar  * s2)
    { return s1.Cmp(s2) >= 0; }
inline bool operator>=(const wxChar  * s1, const wxString& s2)
    { return s2.Cmp(s1) <= 0; }

#endif // !wxUSE_STL

// comparison with char
inline bool operator==(wxChar c, const wxString& s) { return s.IsSameAs(c); }
inline bool operator==(const wxString& s, wxChar c) { return s.IsSameAs(c); }
inline bool operator!=(wxChar c, const wxString& s) { return !s.IsSameAs(c); }
inline bool operator!=(const wxString& s, wxChar c) { return !s.IsSameAs(c); }

#if wxUSE_UNICODE
inline bool operator==(const wxString& s1, const wxWCharBuffer& s2)
    { return (s1.Cmp((const wchar_t *)s2) == 0); }
inline bool operator==(const wxWCharBuffer& s1, const wxString& s2)
    { return (s2.Cmp((const wchar_t *)s1) == 0); }
inline bool operator!=(const wxString& s1, const wxWCharBuffer& s2)
    { return (s1.Cmp((const wchar_t *)s2) != 0); }
inline bool operator!=(const wxWCharBuffer& s1, const wxString& s2)
    { return (s2.Cmp((const wchar_t *)s1) != 0); }
#else // !wxUSE_UNICODE
inline bool operator==(const wxString& s1, const wxCharBuffer& s2)
    { return (s1.Cmp((const char *)s2) == 0); }
inline bool operator==(const wxCharBuffer& s1, const wxString& s2)
    { return (s2.Cmp((const char *)s1) == 0); }
inline bool operator!=(const wxString& s1, const wxCharBuffer& s2)
    { return (s1.Cmp((const char *)s2) != 0); }
inline bool operator!=(const wxCharBuffer& s1, const wxString& s2)
    { return (s2.Cmp((const char *)s1) != 0); }
#endif // wxUSE_UNICODE/!wxUSE_UNICODE

#if !wxUSE_STL

wxString WXDLLIMPEXP_BASE operator+(const wxString& string1,  const wxString& string2);
wxString WXDLLIMPEXP_BASE operator+(const wxString& string, wxChar ch);
wxString WXDLLIMPEXP_BASE operator+(wxChar ch, const wxString& string);
wxString WXDLLIMPEXP_BASE operator+(const wxString& string, const wxChar *psz);
wxString WXDLLIMPEXP_BASE operator+(const wxChar *psz, const wxString& string);

#endif // !wxUSE_STL

#if wxUSE_UNICODE
inline wxString operator+(const wxString& string, const wxWCharBuffer& buf)
    { return string + (const wchar_t *)buf; }
inline wxString operator+(const wxWCharBuffer& buf, const wxString& string)
    { return (const wchar_t *)buf + string; }
#else // !wxUSE_UNICODE
inline wxString operator+(const wxString& string, const wxCharBuffer& buf)
    { return string + (const char *)buf; }
inline wxString operator+(const wxCharBuffer& buf, const wxString& string)
    { return (const char *)buf + string; }
#endif // wxUSE_UNICODE/!wxUSE_UNICODE

// ---------------------------------------------------------------------------
// Implementation only from here until the end of file
// ---------------------------------------------------------------------------

// don't pollute the library user's name space
#undef wxASSERT_VALID_INDEX

#if wxUSE_STD_IOSTREAM

#include "wx/iosfwrap.h"

WXDLLIMPEXP_BASE wxSTD istream& operator>>(wxSTD istream&, wxString&);
WXDLLIMPEXP_BASE wxSTD ostream& operator<<(wxSTD ostream&, const wxString&);

#endif  // wxSTD_STRING_COMPATIBILITY

#endif  // _WX_WXSTRINGH__
