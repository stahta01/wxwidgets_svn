/////////////////////////////////////////////////////////////////////////////
// Name:        wx/filename.h
// Purpose:     wxFileName - encapsulates a file path
// Author:      Robert Roebling, Vadim Zeitlin
// Modified by:
// Created:     28.12.00
// RCS-ID:      $Id$
// Copyright:   (c) 2000 Robert Roebling
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifndef   _WX_FILENAME_H_
#define   _WX_FILENAME_H_

#ifdef __GNUG__
    #pragma interface "filename.h"
#endif

#ifndef WX_PRECOMP
    #include  "wx/string.h"
#endif

/*
    TODO:

    1. support for drives under Windows
    2. more file operations:
        a) chmod()
        b) [acm]time() - get and set
        c) file size
        d) file permissions with readable accessors for most common bits
           such as IsReadable() &c
        e) rename()?
    3. SameFileAs() function to compare inodes under Unix
 */

// ridiculously enough, this will replace DirExists with wxDirExists etc
#include "wx/filefn.h"
#include "wx/datetime.h"

class WXDLLEXPORT wxFile;

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// the various values for the path format: this mainly affects the path
// separator but also whether or not the path has the drive part (as under
// Windows)
enum wxPathFormat
{
    wxPATH_NATIVE = 0,      // the path format for the current platform
    wxPATH_UNIX,
    wxPATH_MAC,
    wxPATH_DOS,
    wxPATH_VMS,

    wxPATH_BEOS = wxPATH_UNIX,
    wxPATH_WIN = wxPATH_DOS,
    wxPATH_OS2 = wxPATH_DOS
};

// the kind of normalization to do with the file name: these values can be
// or'd together to perform several operations at once
enum wxPathNormalize
{
    wxPATH_NORM_ENV_VARS = 0x0001,  // replace env vars with their values
    wxPATH_NORM_DOTS     = 0x0002,  // squeeze all .. and . and prepend cwd
    wxPATH_NORM_TILDE    = 0x0004,  // Unix only: replace ~ and ~user
    wxPATH_NORM_CASE     = 0x0008,  // if case insensitive => tolower
    wxPATH_NORM_ABSOLUTE = 0x0010,  // make the path absolute
    wxPATH_NORM_LONG =     0x0020,  // make the path the long form
    wxPATH_NORM_ALL      = 0x003f
};

// ----------------------------------------------------------------------------
// wxFileName: encapsulates a file path
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxFileName
{
public:
    // constructors and assignment

        // the usual stuff
    wxFileName() { }
    wxFileName( const wxFileName &filepath ) { Assign(filepath); }

        // from a full filename: if it terminates with a '/', a directory path
        // is contructed (the name will be empty), otherwise a file name and
        // extension are extracted from it
    wxFileName( const wxString& fullpath, wxPathFormat format = wxPATH_NATIVE )
        { Assign( fullpath, format ); }

        // from a directory name and a file name
    wxFileName(const wxString& path,
               const wxString& name,
               wxPathFormat format = wxPATH_NATIVE)
        { Assign(path, name, format); }

        // from a volume, directory name, file base name and extension
    wxFileName(const wxString& volume,
               const wxString& path,
               const wxString& name,
               const wxString& ext,
               wxPathFormat format = wxPATH_NATIVE)
        { Assign(volume, path, name, ext, format); }

        // from a directory name, file base name and extension
    wxFileName(const wxString& path,
               const wxString& name,
               const wxString& ext,
               wxPathFormat format = wxPATH_NATIVE)
        { Assign(path, name, ext, format); }

        // the same for delayed initialization

    void Assign(const wxFileName& filepath);

    void Assign(const wxString& fullpath,
                wxPathFormat format = wxPATH_NATIVE);

    void Assign(const wxString& volume,
                const wxString& path,
                const wxString& name,
                const wxString& ext,
                wxPathFormat format = wxPATH_NATIVE);

    void Assign(const wxString& path,
                const wxString& name,
                wxPathFormat format = wxPATH_NATIVE);

    void Assign(const wxString& path,
                const wxString& name,
                const wxString& ext,
                wxPathFormat format = wxPATH_NATIVE)
    {
        // empty volume
        Assign(_T(""), path, name, ext, format);
    }

    void AssignDir(const wxString& dir, wxPathFormat format = wxPATH_NATIVE);

        // assorted assignment operators

    wxFileName& operator=(const wxFileName& filename)
        { Assign(filename); return *this; }

    wxFileName& operator=(const wxString& filename)
        { Assign(filename); return *this; }

        // reset all components to default, uninitialized state
    void Clear();

        // static pseudo constructors
    static wxFileName FileName(const wxString& file);
    static wxFileName DirName(const wxString& dir);

    // file tests

        // is the filename valid at all?
    bool IsOk() const { return !m_dirs.IsEmpty() || !m_name.IsEmpty(); }

        // does the file with this name exists?
    bool FileExists();
    static bool FileExists( const wxString &file );

        // does the directory with this name exists?
    bool DirExists();
    static bool DirExists( const wxString &dir );

        // VZ: also need: IsDirWritable(), IsFileExecutable() &c (TODO)

    // time functions

        // set the file creation and last access/mod times
        // (any of the pointers may be NULL)
    bool SetTimes(const wxDateTime *dtCreate,
                  const wxDateTime *dtAccess,
                  const wxDateTime *dtMod);

        // set the access and modification times to the current moment
    bool Touch();

        // return the last access, last modification and last change times
        // (any of the pointers may be NULL)
    bool GetTimes(wxDateTime *dtAccess,
                  wxDateTime *dtMod,
                  wxDateTime *dtChange) const;

        // convenience wrapper: get just the last mod time of the file
    wxDateTime GetModificationTime() const
    {
        wxDateTime dtMod;
        (void)GetTimes(NULL, &dtMod, NULL);
        return dtMod;
    }

#ifdef __WXMAC__
    bool MacSetTypeAndCreator( wxUint32 type , wxUint32 creator ) ;
    bool MacGetTypeAndCreator( wxUint32 *type , wxUint32 *creator ) ;
    // gets the 'common' type and creator for a certain extension
    static bool MacFindDefaultTypeAndCreator( const wxString& ext , wxUint32 *type , wxUint32 *creator ) ;
    // registers application defined extensions and their default type and creator
    static void MacRegisterDefaultTypeAndCreator( const wxString& ext , wxUint32 type , wxUint32 creator ) ;
    // looks up the appropriate type and creator from the registration and then sets
    bool MacSetDefaultTypeAndCreator() ;
#endif
    // various file/dir operations

        // retrieve the value of the current working directory
    void AssignCwd(const wxString& volume = wxEmptyString);
    static wxString GetCwd(const wxString& volume = wxEmptyString);

        // change the current working directory
    bool SetCwd();
    static bool SetCwd( const wxString &cwd );

        // get the value of user home (Unix only mainly)
    void AssignHomeDir();
    static wxString GetHomeDir();

        // get a temp file name starting with the specified prefix and open the
        // file passed to us using this name for writing (atomically if
        // possible)
    void AssignTempFileName(const wxString& prefix, wxFile *fileTemp = NULL);
    static wxString CreateTempFileName(const wxString& prefix,
                                       wxFile *fileTemp = NULL);

    // directory creation and removal.
    // if full is TRUE, will try to make each directory in the path.
    bool Mkdir( int perm = 0777, bool full = FALSE);
    static bool Mkdir( const wxString &dir, int perm = 0777, bool full = FALSE );

    bool Rmdir();
    static bool Rmdir( const wxString &dir );

    // operations on the path

        // normalize the path: with the default flags value, the path will be
        // made absolute, without any ".." and "." and all environment
        // variables will be expanded in it
        //
        // this may be done using another (than current) value of cwd
    bool Normalize(wxPathNormalize flags = wxPATH_NORM_ALL,
                   const wxString& cwd = wxEmptyString,
                   wxPathFormat format = wxPATH_NATIVE);

        // get a path path relative to the given base directory, i.e. opposite
        // of Normalize
        //
        // pass an empty string to get a path relative to the working directory
        //
        // returns TRUE if the file name was modified, FALSE if we failed to do
        // anything with it (happens when the file is on a different volume,
        // for example)
    bool MakeRelativeTo(const wxString& pathBase = _T(""),
                        wxPathFormat format = wxPATH_NATIVE);


    // Comparison

        // compares with the rules of this platform
    bool SameAs(const wxFileName &filepath,
                wxPathFormat format = wxPATH_NATIVE);

        // uses the current platform settings
    bool operator==(const wxFileName& filename) { return SameAs(filename); }
    bool operator==(const wxString& filename)
        { return *this == wxFileName(filename); }

        // are the file names of this type cases sensitive?
    static bool IsCaseSensitive( wxPathFormat format = wxPATH_NATIVE );

        // is this filename absolute?
    bool IsAbsolute() const
        { return !m_relative; }

        // is this filename relative?
    bool IsRelative() const
        { return m_relative; }
        
        // forcibly set the flag
    void SetAbsolute()
        { m_relative = FALSE; }
    void SetRelative()
        { m_relative = TRUE; }

    // Information about path format

    // get the string separating the volume from the path for this format
    static wxString GetVolumeSeparator(wxPathFormat format = wxPATH_NATIVE);

    // get the string of path separators for this format
    static wxString GetPathSeparators(wxPathFormat format = wxPATH_NATIVE);

    // is the char a path separator for this format?
    static bool IsPathSeparator(wxChar ch, wxPathFormat format = wxPATH_NATIVE);

    // FIXME: what exactly does this do?
    bool IsWild( wxPathFormat format = wxPATH_NATIVE );

    // Dir accessors
    void AppendDir( const wxString &dir );
    void PrependDir( const wxString &dir );
    void InsertDir( int before, const wxString &dir );
    void RemoveDir( int pos );
    size_t GetDirCount() const { return m_dirs.GetCount(); }

    // Other accessors
    void SetExt( const wxString &ext )          { m_ext = ext; }
    wxString GetExt() const                     { return m_ext; }
    bool HasExt() const                         { return !m_ext.empty(); }

    void SetName( const wxString &name )        { m_name = name; }
    wxString GetName() const                    { return m_name; }
    bool HasName() const                        { return !m_name.empty(); }

    void SetVolume( const wxString &volume )    { m_volume = volume; }
    wxString GetVolume() const                  { return m_volume; }
    bool HasVolume() const                      { return !m_volume.empty(); }

    // full name is the file name + extension (but without the path)
    void SetFullName(const wxString& fullname);
    wxString GetFullName() const;

    const wxArrayString& GetDirs() const        { return m_dirs; }

    // Construct path only - possibly with the trailing separator
    wxString GetPath( bool add_separator = FALSE,
                      wxPathFormat format = wxPATH_NATIVE ) const;
    // Replace current path with this one
    void SetPath( const wxString &path, wxPathFormat format = wxPATH_NATIVE );

    // more readable synonym
    wxString GetPathWithSep(wxPathFormat format = wxPATH_NATIVE ) const
        { return GetPath(TRUE /* add separator */, format); }

    // Construct full path with name and ext
    wxString GetFullPath( wxPathFormat format = wxPATH_NATIVE ) const;

    // Return the short form of the path (returns identity on non-Windows platforms)
    wxString GetShortPath() const;

    // Return the long form of the path (returns identity on non-Windows platforms)
    wxString GetLongPath() const;

    // various helpers

        // get the canonical path format for this platform
    static wxPathFormat GetFormat( wxPathFormat format = wxPATH_NATIVE );

        // split a fullpath into the volume, path, (base) name and extension
        // (all of the pointers can be NULL)
    static void SplitPath(const wxString& fullpath,
                          wxString *volume,
                          wxString *path,
                          wxString *name,
                          wxString *ext,
                          wxPathFormat format = wxPATH_NATIVE);

        // compatibility version
    static void SplitPath(const wxString& fullpath,
                          wxString *path,
                          wxString *name,
                          wxString *ext,
                          wxPathFormat format = wxPATH_NATIVE);

private:
    // the drive/volume/device specification (always empty for Unix)
    wxString        m_volume;

    // the path components of the file
    wxArrayString   m_dirs;

    // the file name and extension (empty for directories)
    wxString        m_name,
                    m_ext;
                    
    // is the path relative
    bool            m_relative;
};

#endif // _WX_FILENAME_H_

