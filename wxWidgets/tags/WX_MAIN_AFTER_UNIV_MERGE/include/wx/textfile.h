///////////////////////////////////////////////////////////////////////////////
// Name:        textfile.h
// Purpose:     class wxTextFile to work with text files of _small_ size
//              (file is fully loaded in memory) and which understands CR/LF
//              differences between platforms.
// Author:      Vadim Zeitlin
// Modified by:
// Created:     03.04.98
// RCS-ID:      $Id$
// Copyright:   (c) 1998 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// Licence:     wxWindows license
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_TEXTFILE_H
#define _WX_TEXTFILE_H

#ifdef __GNUG__
    #pragma interface "textfile.h"
#endif

#include "wx/defs.h"

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// NB: this is always defined, even if !wxUSE_TEXTFILE

// the line termination type
enum wxTextFileType
{
    wxTextFileType_None,  // incomplete (the last line of the file only)
    wxTextFileType_Unix,  // line is terminated with 'LF' = 0xA = 10 = '\n'
    wxTextFileType_Dos,   //                         'CR' 'LF'
    wxTextFileType_Mac,   //                         'CR' = 0xD = 13 = '\r'
    wxTextFileType_Os2    //                         'CR' 'LF'
};

#if wxUSE_TEXTFILE

#include "wx/string.h"
#include "wx/file.h"
#include "wx/dynarray.h"

// ----------------------------------------------------------------------------
// wxTextFile
// ----------------------------------------------------------------------------

WX_DEFINE_EXPORTED_ARRAY(wxTextFileType, ArrayFileType);

class WXDLLEXPORT wxTextFile
{
public:
  // constants and static functions
    // default type for current platform (determined at compile time)
  static const wxTextFileType typeDefault;

    // this function returns a string which is identical to "text" passed in
    // except that the line terminator characters are changed to correspond the
    // given type. Called with the default argument, the function translates
    // the string to the native format (Unix for Unix, DOS for Windows, ...).
  static wxString Translate(const wxString& text,
                            wxTextFileType type = typeDefault);

    // get the file termination string
  static const wxChar *GetEOL(wxTextFileType type = typeDefault);

  // ctors
    // def ctor, use Open(string)
  wxTextFile() { }
    //
  wxTextFile(const wxString& strFile);

  // file operations
    // file exists?
  bool Exists() const;
    // create the file if it doesn't already exist
  bool Create();
    // same as Create() but with (another) file name
  bool Create(const wxString& strFile);
    // Open() also loads file in memory on success
  bool Open();
    // same as Open() but with (another) file name
  bool Open(const wxString& strFile);
    // closes the file and frees memory, losing all changes
  bool Close();
    // is file currently opened?
  bool IsOpened() const { return m_isOpened; }

  // accessors
    // get the number of lines in the file
  size_t GetLineCount() const { return m_aLines.Count(); }
    // the returned line may be modified (but don't add CR/LF at the end!)
  wxString& GetLine(size_t n)    const { return m_aLines[n]; }
  wxString& operator[](size_t n) const { return m_aLines[n]; }

    // the current line has meaning only when you're using
    // GetFirstLine()/GetNextLine() functions, it doesn't get updated when
    // you're using "direct access" i.e. GetLine()
  size_t GetCurrentLine() const { return m_nCurLine; }
  void GoToLine(size_t n) { m_nCurLine = n; }
  bool Eof() const { return (m_aLines.Count() == 0 || m_nCurLine == m_aLines.Count() - 1); }

    // these methods allow more "iterator-like" traversal of the list of
    // lines, i.e. you may write something like:
    //  for ( str = GetFirstLine(); !Eof(); str = GetNextLine() ) { ... }

    // NB: const is commented out because not all compilers understand
    //     'mutable' keyword yet (m_nCurLine should be mutable)
  wxString& GetFirstLine() /* const */ { return m_aLines[m_nCurLine = 0]; }
  wxString& GetNextLine()  /* const */ { return m_aLines[++m_nCurLine];   }
  wxString& GetPrevLine()  /* const */
    { wxASSERT(m_nCurLine > 0); return m_aLines[--m_nCurLine];   }
  wxString& GetLastLine() /* const */
    { return m_aLines[m_nCurLine = m_aLines.Count() - 1]; }

    // get the type of the line (see also GetEOL)
  wxTextFileType GetLineType(size_t n) const { return m_aTypes[n]; }
    // guess the type of file (m_file is supposed to be opened)
  wxTextFileType GuessType() const;
    // get the name of the file
  const wxChar *GetName() const { return m_strFile.c_str(); }

  // add/remove lines
    // add a line to the end
  void AddLine(const wxString& str, wxTextFileType type = typeDefault)
    { m_aLines.Add(str); m_aTypes.Add(type); }
    // insert a line before the line number n
  void InsertLine(const wxString& str,
                  size_t n,
                  wxTextFileType type = typeDefault)
    { m_aLines.Insert(str, n); m_aTypes.Insert(type, n); }
    // delete one line
  void RemoveLine(size_t n) { m_aLines.Remove(n); m_aTypes.Remove(n); }

  // change the file on disk (default argument means "don't change type")
  // possibly in another format
  bool Write(wxTextFileType typeNew = wxTextFileType_None);

  // dtor
  ~wxTextFile();

private:
  // copy ctor/assignment operator not implemented
  wxTextFile(const wxTextFile&);
  wxTextFile& operator=(const wxTextFile&);

  // read the file in memory (m_file is supposed to be just opened)
  bool Read();

  wxFile        m_file;     // current file

  ArrayFileType m_aTypes;   // type of each line
  wxArrayString m_aLines;   // lines of file

  size_t        m_nCurLine; // number of current line in the file

  bool          m_isOpened; // was the file successfully opened the last time?

  wxString      m_strFile;  // name of the file
};

#else // !wxUSE_TEXTFILE

// these static wxTextFile methods are used internally by wxWindows, so should
// be defined even if we're compiling without wxTextFile at all.

class WXDLLEXPORT wxTextFile
{
public:
    // default type for current platform (determined at compile time)
    static const wxTextFileType typeDefault;

    // this function returns a string which is identical to "text" passed in
    // except that the line terminator characters are changed to correspond the
    // given type. Called with the default argument, the function translates
    // the string to the native format (Unix for Unix, DOS for Windows, ...).
    static wxString Translate(const wxString& text,
                              wxTextFileType type = typeDefault);

    // get the file termination string
    static const wxChar *GetEOL(wxTextFileType type = typeDefault);

private:
    // copy ctor/assignment operator not implemented
    wxTextFile(const wxTextFile&);
    wxTextFile& operator=(const wxTextFile&);

    // suppress the gcc warning: 'class defines only private constructors and
    // has no friends'
#ifdef __GNUG__
    friend class wxTextFileDummyFriend;
#endif // gcc
};

#endif // wxUSE_TEXTFILE

#endif // _WX_TEXTFILE_H

