///////////////////////////////////////////////////////////////////////////////
// Name:        config.cpp
// Purpose:     implementation of wxConfig class
// Author:      Vadim Zeitlin
// Modified by:
// Created:     07.04.98
// RCS-ID:      $Id$
// Copyright:   (c) 1997 Karsten Ball�der   Ballueder@usa.net
//                       Vadim Zeitlin      <zeitlin@dptmaths.ens-cachan.fr>
// Licence:     wxWindows license
///////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------
#ifdef __GNUG__
  #pragma implementation "config.h"
#endif

#include  "wx/wxprec.h"

#ifdef    __BORLANDC__
  #pragma hdrstop
#endif  //__BORLANDC__

#ifndef WX_PRECOMP
  #include  <wx/string.h>
  #include  <wx/intl.h>
#endif //WX_PRECOMP

#include  <wx/app.h>
#include  <wx/file.h>
#include  <wx/log.h>
#include  <wx/textfile.h>
#include  <wx/config.h>
#include  <wx/fileconf.h>
#include <stdlib.h>

#include  <ctype.h>       // for isalnum()

// ----------------------------------------------------------------------------
// global and class static variables
// ----------------------------------------------------------------------------

wxConfig *wxConfig::ms_pConfig = NULL;

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxConfig
// ----------------------------------------------------------------------------
wxConfig::~wxConfig()
{
}

wxConfig *wxConfig::Set(wxConfig *pConfig)
{
  wxConfig *pOld = ms_pConfig;
  ms_pConfig = pConfig;
  return pOld;
}

void wxConfig::Create()
{
  ms_pConfig = wxTheApp->CreateConfig();
}

const char *wxConfig::Read(const char *szKey, const char *szDefault) const
{
  static char s_szBuf[1024];
  wxString s;
  Read(&s, szKey, szDefault);
  strncpy(s_szBuf, s, WXSIZEOF(s_szBuf));

  return s_szBuf;
}

// ----------------------------------------------------------------------------
// Config::PathChanger
// ----------------------------------------------------------------------------

wxConfig::PathChanger::PathChanger(const wxConfig *pContainer,
                                 const wxString& strEntry)
{
  m_pContainer = (wxConfig *)pContainer;
  wxString strPath = strEntry.Before(APPCONF_PATH_SEPARATOR);

  // special case of "/keyname" when there is nothing before "/"
  if ( strPath.IsEmpty() && strEntry[0] == APPCONF_PATH_SEPARATOR )
    strPath = APPCONF_PATH_SEPARATOR;

  if ( !strPath.IsEmpty() ) {
    // do change the path
    m_bChanged = TRUE;
    m_strName = strEntry.Right(APPCONF_PATH_SEPARATOR);
    m_strOldPath = m_pContainer->GetPath();
    m_strOldPath += APPCONF_PATH_SEPARATOR;
    m_pContainer->SetPath(strPath);
  }
  else {
    // it's a name only, without path - nothing to do
    m_bChanged = FALSE;
    m_strName = strEntry;
  }
}

wxConfig::PathChanger::~PathChanger()
{
  // only restore path if it was changed
  if ( m_bChanged ) {
    m_pContainer->SetPath(m_strOldPath);
  }
}

// ----------------------------------------------------------------------------
// static & global functions
// ----------------------------------------------------------------------------

// understands both Unix and Windows (but only under Windows) environment
// variables expansion: i.e. $var, $(var) and ${var} are always understood
// and in addition under Windows %var% is also.
wxString ExpandEnvVars(const wxString& str)
{
  wxString strResult;
  strResult.Alloc(str.Len());

  // don't change the values the enum elements: they must be equal
  // to the matching [closing] delimiter.
  enum Bracket
  {
    Bracket_None,
    Bracket_Normal  = ')',
    Bracket_Curly   = '}',
#ifdef  __WXMSW__
    Bracket_Windows = '%'     // yeah, Windows people are a bit strange ;-)
#endif
  };

  uint m;
  for ( uint n = 0; n < str.Len(); n++ ) {
    switch ( str[n] ) {
#ifdef  __WXMSW__
      case '%':
#endif  //WINDOWS
      case '$':
        {
          Bracket bracket;
          #ifdef  __WXMSW__
            if ( str[n] == '%' )
              bracket = Bracket_Windows;
            else
          #endif  //WINDOWS
          if ( n == str.Len() - 1 ) {
            bracket = Bracket_None;
          }
          else {
            switch ( str[n + 1] ) {
              case '(':
                bracket = Bracket_Normal;
                n++;                   // skip the bracket
                break;

              case '{':
                bracket = Bracket_Curly;
                n++;                   // skip the bracket
                break;

              default:
                bracket = Bracket_None;
            }
          }

          m = n + 1;

          while ( m < str.Len() && (isalnum(str[m]) || str[m] == '_') )
            m++;

          wxString strVarName(str.c_str() + n + 1, m - n - 1);

          const char *pszValue = getenv(strVarName);
          if ( pszValue != NULL ) {
            strResult += pszValue;
          }
          else {
            // variable doesn't exist => don't change anything
            #ifdef  __WXMSW__
              if ( bracket != Bracket_Windows )
            #endif
                if ( bracket != Bracket_None )
                  strResult << str[n - 1];
            strResult << str[n] << strVarName;
          }

          // check the closing bracket
          if ( bracket != Bracket_None ) {
            if ( m == str.Len() || str[m] != (char)bracket ) {
              wxLogWarning("missing '%c' at position %d in '%s'.",
                           (char)bracket, m + 1, str.c_str());
            }
            else {
              // skip closing bracket
              if ( pszValue == NULL )
                strResult << (char)bracket;
              m++;
            }
          }

          n = m - 1;  // skip variable name
        }
        break;

      default:
        strResult += str[n];
    }
  }

  return strResult;
}

// this function is used to properly interpret '..' in path
void SplitPath(wxArrayString& aParts, const char *sz)
{
  aParts.Empty();

  wxString strCurrent;
  const char *pc = sz;
  for ( ;; ) {
    if ( *pc == '\0' || *pc == APPCONF_PATH_SEPARATOR ) {
      if ( strCurrent == "." ) {
        // ignore
      }
      else if ( strCurrent == ".." ) {
        // go up one level
        if ( aParts.IsEmpty() )
          wxLogWarning("'%s' has extra '..', ignored.", sz);
        else
          aParts.Remove(aParts.Count() - 1);

        strCurrent.Empty();
      }
      else if ( !strCurrent.IsEmpty() ) {
        aParts.Add(strCurrent);
        strCurrent.Empty();
      }
      //else:
        // could log an error here, but we prefer to ignore extra '/'

      if ( *pc == '\0' )
        return;
    }
    else
      strCurrent += *pc;

    pc++;
  }
}

