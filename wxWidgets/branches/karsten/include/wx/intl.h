/////////////////////////////////////////////////////////////////////////////
// Name:        intl.h
// Purpose:     Internationalization and localisation for wxWindows
// Author:      Vadim Zeitlin
// Modified by:
// Created:     29/01/98
// RCS-ID:      $Id$
// Copyright:   (c) 1998 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// Licence:   	wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifndef   __INTLH__
#define   __INTLH__

#ifdef __GNUG__
#pragma interface "intl.h"
#endif

#include "wx/defs.h"
#include "wx/string.h"

// ============================================================================
// global decls
// ============================================================================

// ----------------------------------------------------------------------------
// simple types
// ----------------------------------------------------------------------------

// # adjust if necessary
typedef unsigned char uint8;
typedef unsigned long uint32;

// ----------------------------------------------------------------------------
// macros
// ----------------------------------------------------------------------------

// gettext() style macro
#define   _(str)  wxGetTranslation(str)

// ----------------------------------------------------------------------------
// forward decls
// ----------------------------------------------------------------------------
class WXDLLEXPORT wxLocale;
class WXDLLEXPORT wxMsgCatalog;

// ----------------------------------------------------------------------------
// global functions
// ----------------------------------------------------------------------------
extern wxLocale* WXDLLEXPORT wxGetLocale();
inline const char* wxGetTranslation(const char *sz);

// ============================================================================
// locale support
// ============================================================================

// ----------------------------------------------------------------------------
// wxLocale: encapsulates all language dependent settings, including current
//           message catalogs, date, time and currency formats (#### to do) &c
// ----------------------------------------------------------------------------
class WXDLLEXPORT wxLocale
{
public:
  // ctor & dtor
    // the ctor has a side effect of changing current locale
    wxLocale(const char *szName,              // name (for messages)
             const char *szShort = NULL,      // dir prefix (for msg files)
             const char *szLocale = NULL,     // locale (for setlocale)
             bool bLoadDefault = TRUE);       // preload wxstd.mo?
    // restores old locale
 ~wxLocale();

  // returns locale name
  const char *GetLocale() const { return m_strLocale; }

  // add a catalog: it's searched for in standard places (current directory 
  // first, system one after). It will be used for message lookup by
  // GetString().
  //
  // Returns 'true' if it was successfully loaded
  bool AddCatalog(const char *szDomain);
  
  // check if the given catalog is loaded
  bool IsLoaded(const char *szDomain) const;
  
  // retrieve the translation for a string in all loaded domains unless
  // the szDomain parameter is specified (and then only this domain is
  // searched)
  //
  // return original string if translation is not available
  // (in this case an error message is generated the first time
  //  a string is not found; use wxLogNull to suppress it)
  //
  // domains are searched in the last to first order, i.e. catalogs
  // added later override those added before.
  const char *GetString(const char *szOrigString, 
                        const char *szDomain = NULL) const;

private:
  // find catalog by name in a linked list, return NULL if !found
  wxMsgCatalog  *FindCatalog(const char *szDomain) const;

  wxString       m_strLocale,     // this locale name
                 m_strShort;      // short name for the locale
  
  const char    *m_pszOldLocale;  // previous locale from setlocale()
  wxLocale      *m_pOldLocale;    // previous wxLocale
  
  wxMsgCatalog  *m_pMsgCat;       // pointer to linked list of catalogs
};

// ============================================================================
// optional features
// ============================================================================

// ----------------------------------------------------------------------------
// wxTString: automatically translates strings to current language
// ----------------------------------------------------------------------------

// this feature should be enabled by defining WX_USE_AUTOTRANS, if it's not
// done no automatic translation is performed
#if  USE_AUTOTRANS
  class WXDLLEXPORT wxTString
  {
  public:
    // NB: different ctors do different things!
      // does translation
    wxTString(const char *sz) : m_pcsz(wxGetTranslation(sz)) { }
      // no translation
    wxTString(const wxString& s) : m_pcsz(s) { }

    // NB: no copy ctor, it must be a POD so that we can pass it
    //     to vararg functions (and it's not needed anyhow)
    
    // implicit conversion
    operator const char *() const { return m_pcsz; }

  private:
    const char *m_pcsz;
  };
#else   //!USE_AUTOTRANS
  #define wxTString   wxString
#endif  //USE_AUTOTRANS

#define TRANSSTRING_DEFINED
  
// ----------------------------------------------------------------------------
// inline functions
// ----------------------------------------------------------------------------

// get the translation of the string in the current locale  
inline const char *wxGetTranslation(const char *sz)
{
  wxLocale *pLoc = wxGetLocale();
  return pLoc == NULL ? sz : pLoc->GetString(sz);
}

#endif
	// __INTLH__
