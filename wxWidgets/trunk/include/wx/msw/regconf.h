///////////////////////////////////////////////////////////////////////////////
// Name:        msw/regconf.h  
// Purpose:     Registry based implementation of wxConfig
// Author:      Vadim Zeitlin
// Modified by: 
// Created:     27.04.98
// RCS-ID:      $Id$
// Copyright:   (c) 1998 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// Licence:     wxWindows license
///////////////////////////////////////////////////////////////////////////////

#ifndef   _REGCONF_H
#define   _REGCONF_H

#ifdef __GNUG__
#pragma interface "regconf.h"
#endif

// ----------------------------------------------------------------------------
// wxRegConfig
// ----------------------------------------------------------------------------

class wxRegConfig : public wxConfig
{
public:
  // ctor & dtor
    // will store data in HKLM\strRegHive and HKCU\strRegHive
  wxRegConfig(const wxString& strRegHive);
    // dtor will save unsaved data
  virtual ~wxRegConfig();

  // implement inherited pure virtual functions
  // ------------------------------------------

  // path management
  virtual void SetPath(const wxString& strPath);
  virtual const wxString& GetPath() const { return m_strPath; }

  // enum
  virtual bool GetFirstGroup(wxString& str, long& lIndex);
  virtual bool GetNextGroup (wxString& str, long& lIndex);
  virtual bool GetFirstEntry(wxString& str, long& lIndex);
  virtual bool GetNextEntry (wxString& str, long& lIndex);

  // tests for existence
  virtual bool HasGroup(const wxString& strName) const;
  virtual bool HasEntry(const wxString& strName) const;

    // get number of entries/subgroups in the current group, with or without
    // it's subgroups
  virtual uint GetNumberOfEntries(bool bRecursive = FALSE) const = 0;
  virtual uint GetNumberOfGroups(bool bRecursive = FALSE) const = 0;

  // read/write
  virtual bool Read(wxString&, const char *, const char * = 0) const;
  virtual bool Read(long&, const char *, long = 0) const;
  virtual bool Write(const char *szKey, const char *szValue);
  virtual bool Write(const char *szKey, long Value);
  virtual bool Flush(bool /* bCurrentOnly = FALSE */ ) { return TRUE; }

  // delete
  virtual bool DeleteEntry(const char *szKey, bool bGroupIfEmptyAlso);
  virtual bool DeleteGroup(const char *szKey);
  virtual bool DeleteAll();

private:
  // these keys are opened during all lifetime of wxRegConfig object
  wxRegKey  m_keyLocalRoot,  m_keyLocal,
            m_keyGlobalRoot, m_keyGlobal;

  // current path (not '/' terminated)
  wxString  m_strPath;
};

#endif  //_REGCONF_H
