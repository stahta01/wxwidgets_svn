///////////////////////////////////////////////////////////////////////////////
// Name:        config.h
// Purpose:     declaration of the base class of all config implementations
//              (see also: fileconf.h and msw/regconf.h)
// Author:      Karsten Ball�der & Vadim Zeitlin
// Modified by: 
// Created:     07.04.98 (adapted from appconf.h)
// RCS-ID:      $Id$
// Copyright:   (c) 1997 Karsten Ball�der   Ballueder@usa.net  
//                       Vadim Zeitlin      <zeitlin@dptmaths.ens-cachan.fr>
// Licence:     wxWindows license
///////////////////////////////////////////////////////////////////////////////

#ifndef   _APPCONF_H
#define   _APPCONF_H

#ifdef __GNUG__
#pragma interface "config.h"
#endif

// ----------------------------------------------------------------------------
// compile options
// ----------------------------------------------------------------------------

// it won't compile without it anyhow
#ifndef USE_WXCONFIG
  #error "Please define USE_WXCONFIG or remove config.cpp from your makefile"
#endif // USE_WXCONFIG

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

/// shall we be case sensitive in parsing variable names?
#ifndef APPCONF_CASE_SENSITIVE
  #define  APPCONF_CASE_SENSITIVE       FALSE
#endif

/// separates group and entry names
#ifndef APPCONF_PATH_SEPARATOR
  #define   APPCONF_PATH_SEPARATOR     '/'
#endif

/// introduces immutable entries
#ifndef APPCONF_IMMUTABLE_PREFIX
  #define   APPCONF_IMMUTABLE_PREFIX   '!'
#endif

/// should we use registry instead of configuration files under Win32?
#ifndef   APPCONF_WIN32_NATIVE
  #define APPCONF_WIN32_NATIVE          TRUE
#endif

// ----------------------------------------------------------------------------
// various helper global functions
// ----------------------------------------------------------------------------

/*
  Replace environment variables ($SOMETHING) with their values. The format is
  $VARNAME or ${VARNAME} where VARNAME contains alphanumeric characters and
  '_' only. '$' must be escaped ('\$') in order to be taken literally.
 */
extern wxString wxExpandEnvVars(const char *sz);

/*
  Split path into parts removing '..' in progress
 */
extern void wxSplitPath(wxArrayString& aParts, const char *sz);

// ----------------------------------------------------------------------------
// abstract base class wxConfig which defines the interface for derived classes
//
// wxConfig organizes the items in a tree-like structure (modeled after the 
// Unix/Dos filesystem). There are groups (directories) and keys (files). 
// There is always one current group given by the current path.
//
// Keys are pairs "key_name = value" where value may be of string or integer
// (long) type (@@@ doubles and other types such as wxDate coming soon).
// ----------------------------------------------------------------------------
class wxConfig
{
public:
  // static functions
    // sets the config object, returns the previous pointer
  static wxConfig *Set(wxConfig *pConfig);
    // get the config object, creates it on demand
  static wxConfig *Get() { if ( !ms_pConfig ) Create(); return ms_pConfig; }
    // create a new config object
  static void Create();

  // ctor & virtual dtor
  wxConfig() { m_bExpandEnvVars = TRUE; }
  virtual ~wxConfig();

  // path management
    // set current path: if the first character is '/', it's the absolute path,
    // otherwise it's a relative path. '..' is supported. If the strPath 
    // doesn't exist it is created.
  virtual void SetPath(const wxString& strPath) = 0;
    // retrieve the current path (always as absolute path)
  virtual const wxString& GetPath() const = 0;

  // enumeration: all functions here return false when there are no more items.
  // you must pass the same lIndex to GetNext and GetFirst (don't modify it)
    // enumerate subgroups
  virtual bool GetFirstGroup(wxString& str, long& lIndex) = 0;
  virtual bool GetNextGroup (wxString& str, long& lIndex) = 0;
    // enumerate entries
  virtual bool GetFirstEntry(wxString& str, long& lIndex) = 0;
  virtual bool GetNextEntry (wxString& str, long& lIndex) = 0;
    // get number of entries/subgroups in the current group, with or without
    // it's subgroups
  virtual uint GetNumberOfEntries(bool bRecursive = FALSE) const = 0;
  virtual uint GetNumberOfGroups(bool bRecursive = FALSE) const = 0;

  // tests of existence
    // returns TRUE if the group by this name exists
  virtual bool HasGroup(const wxString& strName) const = 0;
    // same as above, but for an entry
  virtual bool HasEntry(const wxString& strName) const = 0;
    // returns TRUE if either a group or an entry with a given name exist
  bool Exists(const wxString& strName) const
    { return HasGroup(strName) || HasEntry(strName); }

  // key access: returns TRUE if value was really read, FALSE if default used
  // (and if the key is not found the default value is returned.)
    // read a string from the key
  virtual bool Read(wxString *pStr, const char *szKey,
                    const char *szDefault = NULL) const = 0;
    // another version using statis buffer - it means it will be overwritten
    // after each call to this function!
  virtual const char *Read(const char *szKey,
                           const char *szDefault = NULL) const;
    // the same for longs
  virtual long Read(const char *szKey, long lDefault) const
    { long l; Read(&l, szKey, lDefault); return l; }
    // and another version: returns true if default value is returned
  virtual bool Read(long *pl, const char *szKey, long lDefault = 0) const = 0;

    // write the value (return true on success)
  virtual bool Write(const char *szKey, const char *szValue) = 0;
  virtual bool Write(const char *szKey, long lValue) = 0;
    // permanently writes all changes
  virtual bool Flush(bool bCurrentOnly = FALSE) = 0;

  // delete entries/groups
    // deletes the specified entry and the group it belongs to if
    // it was the last key in it and the second parameter is true
  virtual bool DeleteEntry(const char *szKey,
                           bool bDeleteGroupIfEmpty = TRUE) = 0;
    // delete the group (with all subgroups)
  virtual bool DeleteGroup(const char *szKey) = 0;
    // delete the whole underlying object (disk file, registry key, ...)
    // primarly for use by desinstallation routine.
  virtual bool DeleteAll() = 0;

  // options
    // we can automatically expand environment variables in the config entries
    // (this option is on by default, you can turn it on/off at any time)
  bool IsExpandingEnvVars() const { return m_bExpandEnvVars; }
  void SetExpandEnvVars(bool bDoIt = TRUE) { m_bExpandEnvVars = bDoIt; }
    // does expansion only if needed
  wxString ExpandEnvVars(const wxString& str) const
    { return IsExpandingEnvVars() ? wxExpandEnvVars(str) : str; }

protected:
  static bool IsImmutable(const char *szKey) 
    { return *szKey == APPCONF_IMMUTABLE_PREFIX; }

  // a handy little class which changes current path to the path of given entry
  // and restores it in dtor: so if you declare a local variable of this type,
  // you work in the entry directory and the path is automatically restored
  // when the function returns
  class PathChanger
  {
  public:
    // ctor/dtor do path changing/restorin
    PathChanger(const wxConfig *pContainer, const wxString& strEntry);
   ~PathChanger();

    // get the key name
   const wxString& Name() const { return m_strName; }

  private:
    wxConfig *m_pContainer;   // object we live in
    wxString  m_strName,      // name of entry (i.e. name only)
              m_strOldPath;   // saved path
    bool      m_bChanged;     // was the path changed?
  };

private:
  // are we doing automatic environment variable expansion?
  bool m_bExpandEnvVars;

  // static variables
  static wxConfig *ms_pConfig;
};

#endif  //_APPCONF_H

