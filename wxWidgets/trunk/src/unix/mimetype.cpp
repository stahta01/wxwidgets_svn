/////////////////////////////////////////////////////////////////////////////
// Name:        unix/mimetype.cpp
// Purpose:     classes and functions to manage MIME types
// Author:      Vadim Zeitlin
// Modified by:
// Created:     23.09.98
// RCS-ID:      $Id$
// Copyright:   (c) 1998 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// Licence:     wxWindows license (part of wxExtra library)
/////////////////////////////////////////////////////////////////////////////

#ifdef    __GNUG__
#pragma implementation "mimetype.h"
#endif

// for compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
  #pragma hdrstop
#endif

#ifndef WX_PRECOMP
  #include "wx/defs.h"
#endif

#if (wxUSE_FILE && wxUSE_TEXTFILE) || defined(__WXMSW__)

#ifndef WX_PRECOMP
  #include "wx/string.h"
  #if wxUSE_GUI
    #include "wx/icon.h"
  #endif
#endif //WX_PRECOMP


#include "wx/log.h"
#include "wx/file.h"
#include "wx/intl.h"
#include "wx/dynarray.h"
#include "wx/confbase.h"

#include "wx/ffile.h"
#include "wx/textfile.h"
#include "wx/dir.h"
#include "wx/utils.h"
#include "wx/tokenzr.h"

#include "wx/unix/mimetype.h"

// other standard headers
#include <ctype.h>

// in case we're compiling in non-GUI mode
class WXDLLEXPORT wxIcon;

// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------


// this class uses both mailcap and mime.types to gather information about file
// types.
//
// The information about mailcap file was extracted from metamail(1) sources and
// documentation.
//
// Format of mailcap file: spaces are ignored, each line is either a comment
// (starts with '#') or a line of the form <field1>;<field2>;...;<fieldN>.
// A backslash can be used to quote semicolons and newlines (and, in fact,
// anything else including itself).
//
// The first field is always the MIME type in the form of type/subtype (see RFC
// 822) where subtype may be '*' meaning "any". Following metamail, we accept
// "type" which means the same as "type/*", although I'm not sure whether this
// is standard.
//
// The second field is always the command to run. It is subject to
// parameter/filename expansion described below.
//
// All the following fields are optional and may not be present at all. If
// they're present they may appear in any order, although each of them should
// appear only once. The optional fields are the following:
//  * notes=xxx is an uninterpreted string which is silently ignored
//  * test=xxx is the command to be used to determine whether this mailcap line
//    applies to our data or not. The RHS of this field goes through the
//    parameter/filename expansion (as the 2nd field) and the resulting string
//    is executed. The line applies only if the command succeeds, i.e. returns 0
//    exit code.
//  * print=xxx is the command to be used to print (and not view) the data of
//    this type (parameter/filename expansion is done here too)
//  * edit=xxx is the command to open/edit the data of this type
//  * needsterminal means that a new console must be created for the viewer
//  * copiousoutput means that the viewer doesn't interact with the user but
//    produces (possibly) a lof of lines of output on stdout (i.e. "cat" is a
//    good example), thus it might be a good idea to use some kind of paging
//    mechanism.
//  * textualnewlines means not to perform CR/LF translation (not honored)
//  * compose and composetyped fields are used to determine the program to be
//    called to create a new message pert in the specified format (unused).
//
// Parameter/filename xpansion:
//  * %s is replaced with the (full) file name
//  * %t is replaced with MIME type/subtype of the entry
//  * for multipart type only %n is replaced with the nnumber of parts and %F is
//    replaced by an array of (content-type, temporary file name) pairs for all
//    message parts (TODO)
//  * %{parameter} is replaced with the value of parameter taken from
//    Content-type header line of the message.
//
// FIXME any docs with real descriptions of these files??
//
// There are 2 possible formats for mime.types file, one entry per line (used
// for global mime.types) and "expanded" format where an entry takes multiple
// lines (used for users mime.types).
//
// For both formats spaces are ignored and lines starting with a '#' are
// comments. Each record has one of two following forms:
//  a) for "brief" format:
//      <mime type>  <space separated list of extensions>
//  b) for "expanded" format:
//      type=<mime type> \ desc="<description>" \ exts="ext"
//
// We try to autodetect the format of mime.types: if a non-comment line starts
// with "type=" we assume the second format, otherwise the first one.

// there may be more than one entry for one and the same mime type, to
// choose the right one we have to run the command specified in the test
// field on our data.
class MailCapEntry
{
public:
    // ctor
    MailCapEntry(const wxString& openCmd,
                 const wxString& printCmd,
                 const wxString& testCmd)
        : m_openCmd(openCmd), m_printCmd(printCmd), m_testCmd(testCmd)
    {
        m_next = NULL;
    }

    // accessors
    const wxString& GetOpenCmd()  const { return m_openCmd;  }
    const wxString& GetPrintCmd() const { return m_printCmd; }
    const wxString& GetTestCmd()  const { return m_testCmd;  }

    MailCapEntry *GetNext() const { return m_next; }

    // operations
        // prepend this element to the list
    void Prepend(MailCapEntry *next) { m_next = next; }
        // insert into the list at given position
    void Insert(MailCapEntry *next, size_t pos)
    {
        // FIXME slooow...
        MailCapEntry *cur;
        size_t n = 0;
        for ( cur = next; cur != NULL; cur = cur->m_next, n++ ) {
            if ( n == pos )
                break;
        }

        wxASSERT_MSG( n == pos, wxT("invalid position in MailCapEntry::Insert") );

        m_next = cur->m_next;
        cur->m_next = this;
    }
        // append this element to the list
    void Append(MailCapEntry *next)
    {
        wxCHECK_RET( next != NULL, wxT("Append()ing to what?") );

        // FIXME slooow...
        MailCapEntry *cur;
        for ( cur = next; cur->m_next != NULL; cur = cur->m_next )
            ;

        cur->m_next = this;

        wxASSERT_MSG( !m_next, wxT("Append()ing element already in the list?") );
    }

private:
    wxString m_openCmd,         // command to use to open/view the file
             m_printCmd,        //                     print
             m_testCmd;         // only apply this entry if test yields
                                // true (i.e. the command returns 0)

    MailCapEntry *m_next;       // in the linked list
};


// the base class which may be used to find an icon for the MIME type
class wxMimeTypeIconHandler
{
public:
    virtual bool GetIcon(const wxString& mimetype, wxIcon *icon) = 0;

    // this function fills manager with MIME types information gathered
    // (as side effect) when searching for icons. This may be particularly
    // useful if mime.types is incomplete (e.g. RedHat distributions).
    virtual void GetMimeInfoRecords(wxMimeTypesManagerImpl *manager) = 0;
};


// the icon handler which uses GNOME MIME database
class wxGNOMEIconHandler : public wxMimeTypeIconHandler
{
public:
    virtual bool GetIcon(const wxString& mimetype, wxIcon *icon);
    virtual void GetMimeInfoRecords(wxMimeTypesManagerImpl *manager);

private:
    void Init();
    void LoadIconsFromKeyFile(const wxString& filename);
    void LoadKeyFilesFromDir(const wxString& dirbase);

    void LoadMimeTypesFromMimeFile(const wxString& filename, wxMimeTypesManagerImpl *manager);
    void LoadMimeFilesFromDir(const wxString& dirbase, wxMimeTypesManagerImpl *manager);

    static bool m_inited;

    static wxSortedArrayString ms_mimetypes;
    static wxArrayString       ms_icons;
};

// the icon handler which uses KDE MIME database
class wxKDEIconHandler : public wxMimeTypeIconHandler
{
public:
    virtual bool GetIcon(const wxString& mimetype, wxIcon *icon);
    virtual void GetMimeInfoRecords(wxMimeTypesManagerImpl *manager);

private:
    void LoadLinksForMimeSubtype(const wxString& dirbase,
                                 const wxString& subdir,
                                 const wxString& filename,
                                 const wxArrayString& icondirs);
    void LoadLinksForMimeType(const wxString& dirbase,
                              const wxString& subdir,
                              const wxArrayString& icondirs);
    void LoadLinkFilesFromDir(const wxString& dirbase,
                              const wxArrayString& icondirs);
    void Init();

    static bool m_inited;

    static wxSortedArrayString ms_mimetypes;
    static wxArrayString       ms_icons;

    static wxArrayString       ms_infoTypes;
    static wxArrayString       ms_infoDescriptions;
    static wxArrayString       ms_infoExtensions;
};



// ----------------------------------------------------------------------------
// various statics
// ----------------------------------------------------------------------------

static wxGNOMEIconHandler gs_iconHandlerGNOME;
static wxKDEIconHandler gs_iconHandlerKDE;

bool wxGNOMEIconHandler::m_inited = FALSE;
wxSortedArrayString wxGNOMEIconHandler::ms_mimetypes;
wxArrayString       wxGNOMEIconHandler::ms_icons;

bool wxKDEIconHandler::m_inited = FALSE;
wxSortedArrayString wxKDEIconHandler::ms_mimetypes;
wxArrayString       wxKDEIconHandler::ms_icons;

wxArrayString       wxKDEIconHandler::ms_infoTypes;
wxArrayString       wxKDEIconHandler::ms_infoDescriptions;
wxArrayString       wxKDEIconHandler::ms_infoExtensions;


ArrayIconHandlers wxMimeTypesManagerImpl::ms_iconHandlers;

// ----------------------------------------------------------------------------
// wxGNOMEIconHandler
// ----------------------------------------------------------------------------

// GNOME stores the info we're interested in in several locations:
//  1. xxx.keys files under /usr/share/mime-info
//  2. xxx.keys files under ~/.gnome/mime-info
//
// The format of xxx.keys file is the following:
//
// mimetype/subtype:
//      field=value
//
// with blank lines separating the entries and indented lines starting with
// TABs. We're interested in the field icon-filename whose value is the path
// containing the icon.

void wxGNOMEIconHandler::LoadIconsFromKeyFile(const wxString& filename)
{
    wxTextFile textfile(filename);
    if ( !textfile.Open() )
        return;

    // values for the entry being parsed
    wxString curMimeType, curIconFile;

    const wxChar *pc;
    size_t nLineCount = textfile.GetLineCount();
    for ( size_t nLine = 0; ; nLine++ )
    {
        if ( nLine < nLineCount )
        {
            pc = textfile[nLine].c_str();
            if ( *pc == _T('#') )
            {
                // skip comments
                continue;
            }
        }
        else
        {
            // so that we will fall into the "if" below
            pc = NULL;
        }

        if ( !pc || !*pc )
        {
            // end of the entry
            if ( !!curMimeType && !!curIconFile )
            {
                // do we already know this mimetype?
                int i = ms_mimetypes.Index(curMimeType);
                if ( i == wxNOT_FOUND )
                {
                    // add a new entry
                    size_t n = ms_mimetypes.Add(curMimeType);
                    ms_icons.Insert(curIconFile, n);
                }
                else
                {
                    // replace the existing one (this means that the directories
                    // should be searched in order of increased priority!)
                    ms_icons[(size_t)i] = curIconFile;
                }
            }

            if ( !pc )
            {
                // the end - this can only happen if nLine == nLineCount
                break;
            }

            curIconFile.Empty();

            continue;
        }

        // what do we have here?
        if ( *pc == _T('\t') )
        {
            // this is a field=value ling
            pc++; // skip leading TAB

            static const int lenField = 13; // strlen("icon-filename")
            if ( wxStrncmp(pc, _T("icon-filename"), lenField) == 0 )
            {
                // skip '=' which follows and take everything left until the end
                // of line
                curIconFile = pc + lenField + 1;
            }
            //else: some other field, we don't care
        }
        else
        {
            // this is the start of the new section
            curMimeType.Empty();

            while ( *pc != _T(':') && *pc != _T('\0') )
            {
                curMimeType += *pc++;
            }
        }
    }
}

void wxGNOMEIconHandler::LoadKeyFilesFromDir(const wxString& dirbase)
{
    wxASSERT_MSG( !!dirbase && !wxEndsWithPathSeparator(dirbase),
                  _T("base directory shouldn't end with a slash") );

    wxString dirname = dirbase;
    dirname << _T("/mime-info");

    if ( !wxDir::Exists(dirname) )
        return;

    wxDir dir(dirname);
    if ( !dir.IsOpened() )
        return;

    // we will concatenate it with filename to get the full path below
    dirname += _T('/');

    wxString filename;
    bool cont = dir.GetFirst(&filename, _T("*.keys"), wxDIR_FILES);
    while ( cont )
    {
        LoadIconsFromKeyFile(dirname + filename);

        cont = dir.GetNext(&filename);
    }
}


void wxGNOMEIconHandler::LoadMimeTypesFromMimeFile(const wxString& filename, wxMimeTypesManagerImpl *manager)
{
    wxTextFile textfile(filename);
    if ( !textfile.Open() )
        return;

    // values for the entry being parsed
    wxString curMimeType, curExtList;

    const wxChar *pc;
    size_t nLineCount = textfile.GetLineCount();
    for ( size_t nLine = 0; ; nLine++ )
    {
        if ( nLine < nLineCount )
        {
            pc = textfile[nLine].c_str();
            if ( *pc == _T('#') )
            {
                // skip comments
                continue;
            }
        }
        else
        {
            // so that we will fall into the "if" below
            pc = NULL;
        }

        if ( !pc || !*pc )
        {
            // end of the entry
            if ( !!curMimeType && !!curExtList )
            {
                 manager -> AddMimeTypeInfo(curMimeType, curExtList, wxEmptyString);
            }

            if ( !pc )
            {
                // the end - this can only happen if nLine == nLineCount
                break;
            }

            curExtList.Empty();

            continue;
        }

        // what do we have here?
        if ( *pc == _T('\t') )
        {
            // this is a field=value ling
            pc++; // skip leading TAB

            static const int lenField = 4; // strlen("ext:")
            if ( wxStrncmp(pc, _T("ext:"), lenField) == 0 )
            {
                // skip ' ' which follows and take everything left until the end
                // of line
                curExtList = pc + lenField + 1;
            }
            //else: some other field, we don't care
        }
        else
        {
            // this is the start of the new section
            curMimeType.Empty();

            while ( *pc != _T(':') && *pc != _T('\0') )
            {
                curMimeType += *pc++;
            }
        }
    }
}


void wxGNOMEIconHandler::LoadMimeFilesFromDir(const wxString& dirbase, wxMimeTypesManagerImpl *manager)
{
    wxASSERT_MSG( !!dirbase && !wxEndsWithPathSeparator(dirbase),
                  _T("base directory shouldn't end with a slash") );

    wxString dirname = dirbase;
    dirname << _T("/mime-info");

    if ( !wxDir::Exists(dirname) )
        return;

    wxDir dir(dirname);
    if ( !dir.IsOpened() )
        return;

    // we will concatenate it with filename to get the full path below
    dirname += _T('/');

    wxString filename;
    bool cont = dir.GetFirst(&filename, _T("*.mime"), wxDIR_FILES);
    while ( cont )
    {
        LoadMimeTypesFromMimeFile(dirname + filename, manager);

        cont = dir.GetNext(&filename);
    }
}


void wxGNOMEIconHandler::Init()
{
    wxArrayString dirs;
    dirs.Add(_T("/usr/share"));
    dirs.Add(_T("/usr/local/share"));

    wxString gnomedir;
    wxGetHomeDir( &gnomedir );
    gnomedir += _T("/.gnome");
    dirs.Add( gnomedir );

    size_t nDirs = dirs.GetCount();
    for ( size_t nDir = 0; nDir < nDirs; nDir++ )
    {
        LoadKeyFilesFromDir(dirs[nDir]);
    }

    m_inited = TRUE;
}


void wxGNOMEIconHandler::GetMimeInfoRecords(wxMimeTypesManagerImpl *manager)
{
    if ( !m_inited )
    {
        Init();
    }
    
    wxArrayString dirs;
    dirs.Add(_T("/usr/share"));
    dirs.Add(_T("/usr/local/share"));

    wxString gnomedir;
    wxGetHomeDir( &gnomedir );
    gnomedir += _T("/.gnome");
    dirs.Add( gnomedir );

    size_t nDirs = dirs.GetCount();
    for ( size_t nDir = 0; nDir < nDirs; nDir++ )
    {
        LoadMimeFilesFromDir(dirs[nDir], manager);
    }
}


bool wxGNOMEIconHandler::GetIcon(const wxString& mimetype, wxIcon *icon)
{
    if ( !m_inited )
    {
        Init();
    }

    int index = ms_mimetypes.Index(mimetype);
    if ( index == wxNOT_FOUND )
        return FALSE;

    wxString iconname = ms_icons[(size_t)index];

#if wxUSE_GUI
    wxLogNull nolog;
    wxIcon icn;
    if (iconname.Right(4).MakeUpper() == _T(".XPM"))
        icn = wxIcon(iconname);
    else
        icn = wxIcon(iconname, wxBITMAP_TYPE_ANY);
    if (icn.Ok()) *icon = icn;
    else return FALSE;
#else
    // helpful for testing in console mode
    wxLogDebug(_T("Found GNOME icon for '%s': '%s'\n"),
               mimetype.c_str(), iconname.c_str());
#endif

    return TRUE;
}

// ----------------------------------------------------------------------------
// wxKDEIconHandler
// ----------------------------------------------------------------------------

// KDE stores the icon info in its .kdelnk files. The file for mimetype/subtype
// may be found in either of the following locations
//
//  1. $KDEDIR/share/mimelnk/mimetype/subtype.kdelnk
//  2. ~/.kde/share/mimelnk/mimetype/subtype.kdelnk
//
// The format of a .kdelnk file is almost the same as the one used by
// wxFileConfig, i.e. there are groups, comments and entries. The icon is the
// value for the entry "Type"

void wxKDEIconHandler::LoadLinksForMimeSubtype(const wxString& dirbase,
                                               const wxString& subdir,
                                               const wxString& filename,
                                               const wxArrayString& icondirs)
{
    wxFFile file(dirbase + filename);
    if ( !file.IsOpened() )
        return;

    // construct mimetype from the directory name and the basename of the
    // file (it always has .kdelnk extension)
    wxString mimetype;
    mimetype << subdir << _T('/') << filename.BeforeLast(_T('.'));

    // these files are small, slurp the entire file at once
    wxString text;
    if ( !file.ReadAll(&text) )
        return;

    int pos;
    const wxChar *pc;

    // before trying to find an icon, grab mimetype information
    // (because BFU's machine would hardly have well-edited mime.types but (s)he might
    // have edited it in control panel...)

    wxString mime_extension, mime_desc;

    pos = wxNOT_FOUND;
    if (wxGetLocale() != NULL)
        mime_desc = _T("Comment[") + wxGetLocale()->GetName() + _T("]=");
    if (pos == wxNOT_FOUND) mime_desc = _T("Comment=");
    pos = text.Find(mime_desc);
    if (pos == wxNOT_FOUND) mime_desc = wxEmptyString;
    else
    {
        pc = text.c_str() + pos + mime_desc.Length();
        mime_desc = wxEmptyString;
        while ( *pc && *pc != _T('\n') ) mime_desc += *pc++;
    }

    pos = text.Find(_T("Patterns="));
    if (pos != wxNOT_FOUND)
    {
        wxString exts;
        pc = text.c_str() + pos + 9;
        while ( *pc && *pc != _T('\n') ) exts += *pc++;
        wxStringTokenizer tokenizer(exts, _T(";"));
        wxString e;

        while (tokenizer.HasMoreTokens())
        {
            e = tokenizer.GetNextToken();
            if (e.Left(2) != _T("*.")) continue; // don't support too difficult patterns
            mime_extension << e.Mid(2);
            mime_extension << _T(' ');
        }
        mime_extension.RemoveLast();
    }

    ms_infoTypes.Add(mimetype);
    ms_infoDescriptions.Add(mime_desc);
    ms_infoExtensions.Add(mime_extension);

    // ok, now we can take care of icon:

    pos = text.Find(_T("Icon="));
    if ( pos == wxNOT_FOUND )
    {
        // no icon info
        return;
    }

    wxString icon;

    pc = text.c_str() + pos + 5;  // 5 == strlen("Icon=")
    while ( *pc && *pc != _T('\n') )
    {
        icon += *pc++;
    }

    if ( !!icon )
    {
        // we must check if the file exists because it may be stored
        // in many locations, at least ~/.kde and $KDEDIR
        size_t nDir, nDirs = icondirs.GetCount();
        for ( nDir = 0; nDir < nDirs; nDir++ )
            if (wxFileExists(icondirs[nDir] + icon))
            {
                icon.Prepend(icondirs[nDir]);
                break;
            }
        if (nDir == nDirs) return; //does not exist

        // do we already have this MIME type?
        int i = ms_mimetypes.Index(mimetype);
        if ( i == wxNOT_FOUND )
        {
            // add it
            size_t n = ms_mimetypes.Add(mimetype);
            ms_icons.Insert(icon, n);
        }
        else
        {
            // replace the old value
            ms_icons[(size_t)i] = icon;
        }
    }
}

void wxKDEIconHandler::LoadLinksForMimeType(const wxString& dirbase,
                                            const wxString& subdir,
                                            const wxArrayString& icondirs)
{
    wxString dirname = dirbase;
    dirname += subdir;
    wxDir dir(dirname);
    if ( !dir.IsOpened() )
        return;

    dirname += _T('/');

    wxString filename;
    bool cont = dir.GetFirst(&filename, _T("*.kdelnk"), wxDIR_FILES);
    while ( cont )
    {
        LoadLinksForMimeSubtype(dirname, subdir, filename, icondirs);

        cont = dir.GetNext(&filename);
    }
}

void wxKDEIconHandler::LoadLinkFilesFromDir(const wxString& dirbase,
                                            const wxArrayString& icondirs)
{
    wxASSERT_MSG( !!dirbase && !wxEndsWithPathSeparator(dirbase),
                  _T("base directory shouldn't end with a slash") );

    wxString dirname = dirbase;
    dirname << _T("/mimelnk");

    if ( !wxDir::Exists(dirname) )
        return;

    wxDir dir(dirname);
    if ( !dir.IsOpened() )
        return;

    // we will concatenate it with dir name to get the full path below
    dirname += _T('/');

    wxString subdir;
    bool cont = dir.GetFirst(&subdir, wxEmptyString, wxDIR_DIRS);
    while ( cont )
    {
        LoadLinksForMimeType(dirname, subdir, icondirs);

        cont = dir.GetNext(&subdir);
    }
}

void wxKDEIconHandler::Init()
{
    wxArrayString dirs;
    wxArrayString icondirs;

    // settings in ~/.kde have maximal priority
    dirs.Add(wxGetHomeDir() + _T("/.kde/share"));
    icondirs.Add(wxGetHomeDir() + _T("/.kde/share/icons/"));

    // the variable KDEDIR is set when KDE is running
    const char *kdedir = getenv("KDEDIR");
    if ( kdedir )
    {
        dirs.Add(wxString(kdedir) + _T("/share"));
        icondirs.Add(wxString(kdedir) + _T("/share/icons/"));
    }
    else
    {
        // try to guess KDEDIR
        dirs.Add(_T("/usr/share"));
        dirs.Add(_T("/opt/kde/share"));
        icondirs.Add(_T("/usr/share/icons/"));
        icondirs.Add(_T("/opt/kde/share/icons/"));
    }

    size_t nDirs = dirs.GetCount();
    for ( size_t nDir = 0; nDir < nDirs; nDir++ )
    {
        LoadLinkFilesFromDir(dirs[nDir], icondirs);
    }

    m_inited = TRUE;
}

bool wxKDEIconHandler::GetIcon(const wxString& mimetype, wxIcon *icon)
{
    if ( !m_inited )
    {
        Init();
    }

    int index = ms_mimetypes.Index(mimetype);
    if ( index == wxNOT_FOUND )
        return FALSE;

    wxString iconname = ms_icons[(size_t)index];

#if wxUSE_GUI
    wxLogNull nolog;
    wxIcon icn;
    if (iconname.Right(4).MakeUpper() == _T(".XPM"))
        icn = wxIcon(iconname);
    else
        icn = wxIcon(iconname, wxBITMAP_TYPE_ANY);
    if (icn.Ok()) *icon = icn;
    else return FALSE;
#else
    // helpful for testing in console mode
    wxLogDebug(_T("Found KDE icon for '%s': '%s'\n"),
               mimetype.c_str(), iconname.c_str());
#endif

    return TRUE;
}


void wxKDEIconHandler::GetMimeInfoRecords(wxMimeTypesManagerImpl *manager)
{
    if ( !m_inited ) Init();

    size_t cnt = ms_infoTypes.GetCount();
    for (unsigned i = 0; i < cnt; i++)
        manager -> AddMimeTypeInfo(ms_infoTypes[i], ms_infoExtensions[i], ms_infoDescriptions[i]);
}


// ----------------------------------------------------------------------------
// wxFileTypeImpl (Unix)
// ----------------------------------------------------------------------------

MailCapEntry *
wxFileTypeImpl::GetEntry(const wxFileType::MessageParameters& params) const
{
    wxString command;
    MailCapEntry *entry = m_manager->m_aEntries[m_index[0]];
    while ( entry != NULL ) {
        // notice that an empty command would always succeed (it's ok)
        command = wxFileType::ExpandCommand(entry->GetTestCmd(), params);

        if ( command.IsEmpty() || (wxSystem(command) == 0) ) {
            // ok, passed
            wxLogTrace(wxT("Test '%s' for mime type '%s' succeeded."),
                       command.c_str(), params.GetMimeType().c_str());
            break;
        }
        else {
            wxLogTrace(wxT("Test '%s' for mime type '%s' failed."),
                       command.c_str(), params.GetMimeType().c_str());
        }

        entry = entry->GetNext();
    }

    return entry;
}

bool wxFileTypeImpl::GetIcon(wxIcon *icon) const
{
    wxArrayString mimetypes;
    GetMimeTypes(mimetypes);

    ArrayIconHandlers& handlers = m_manager->GetIconHandlers();
    size_t count = handlers.GetCount();
    size_t counttypes = mimetypes.GetCount();
    for ( size_t n = 0; n < count; n++ )
    {
        for ( size_t n2 = 0; n2 < counttypes; n2++ )
        {
            if ( handlers[n]->GetIcon(mimetypes[n2], icon) )
                return TRUE;
        }
    }

    return FALSE;
}


bool 
wxFileTypeImpl::GetMimeTypes(wxArrayString& mimeTypes) const
{
    mimeTypes.Clear();
    for (size_t i = 0; i < m_index.GetCount(); i++)
        mimeTypes.Add(m_manager->m_aTypes[m_index[i]]);
    return TRUE;
}


bool
wxFileTypeImpl::GetExpandedCommand(wxString *expandedCmd,
                                   const wxFileType::MessageParameters& params,
                                   bool open) const
{
    MailCapEntry *entry = GetEntry(params);
    if ( entry == NULL ) {
        // all tests failed...
        return FALSE;
    }

    wxString cmd = open ? entry->GetOpenCmd() : entry->GetPrintCmd();
    if ( cmd.IsEmpty() ) {
        // may happen, especially for "print"
        return FALSE;
    }

    *expandedCmd = wxFileType::ExpandCommand(cmd, params);
    return TRUE;
}

bool wxFileTypeImpl::GetExtensions(wxArrayString& extensions)
{
    wxString strExtensions = m_manager->GetExtension(m_index[0]);
    extensions.Empty();

    // one extension in the space or comma delimitid list
    wxString strExt;
    for ( const wxChar *p = strExtensions; ; p++ ) {
        if ( *p == wxT(' ') || *p == wxT(',') || *p == wxT('\0') ) {
            if ( !strExt.IsEmpty() ) {
                extensions.Add(strExt);
                strExt.Empty();
            }
            //else: repeated spaces (shouldn't happen, but it's not that
            //      important if it does happen)

            if ( *p == wxT('\0') )
                break;
        }
        else if ( *p == wxT('.') ) {
            // remove the dot from extension (but only if it's the first char)
            if ( !strExt.IsEmpty() ) {
                strExt += wxT('.');
            }
            //else: no, don't append it
        }
        else {
            strExt += *p;
        }
    }

    return TRUE;
}

// ----------------------------------------------------------------------------
// wxMimeTypesManagerImpl (Unix)
// ----------------------------------------------------------------------------

/* static */
ArrayIconHandlers& wxMimeTypesManagerImpl::GetIconHandlers()
{
    if ( ms_iconHandlers.GetCount() == 0 )
    {
        ms_iconHandlers.Add(&gs_iconHandlerGNOME);
        ms_iconHandlers.Add(&gs_iconHandlerKDE);
    }

    return ms_iconHandlers;
}

// read system and user mailcaps (TODO implement mime.types support)
wxMimeTypesManagerImpl::wxMimeTypesManagerImpl()
{
    // directories where we look for mailcap and mime.types by default
    // (taken from metamail(1) sources)
    static const wxChar *aStandardLocations[] =
    {
        wxT("/etc"),
        wxT("/usr/etc"),
        wxT("/usr/local/etc"),
        wxT("/etc/mail"),
        wxT("/usr/public/lib")
    };

    // first read the system wide file(s)
    size_t n;
    for ( n = 0; n < WXSIZEOF(aStandardLocations); n++ ) {
        wxString dir = aStandardLocations[n];

        wxString file = dir + wxT("/mailcap");
        if ( wxFile::Exists(file) ) {
            ReadMailcap(file);
        }

        file = dir + wxT("/mime.types");
        if ( wxFile::Exists(file) ) {
            ReadMimeTypes(file);
        }
    }

    wxString strHome = wxGetenv(wxT("HOME"));

    // and now the users mailcap
    wxString strUserMailcap = strHome + wxT("/.mailcap");
    if ( wxFile::Exists(strUserMailcap) ) {
        ReadMailcap(strUserMailcap);
    }

    // read the users mime.types
    wxString strUserMimeTypes = strHome + wxT("/.mime.types");
    if ( wxFile::Exists(strUserMimeTypes) ) {
        ReadMimeTypes(strUserMimeTypes);
    }

    // read KDE/GNOME tables
    ArrayIconHandlers& handlers = GetIconHandlers();
    size_t count = handlers.GetCount();
    for ( size_t hn = 0; hn < count; hn++ )
        handlers[hn]->GetMimeInfoRecords(this);
}

wxFileType *
wxMimeTypesManagerImpl::GetFileTypeFromExtension(const wxString& ext)
{
    wxFileType *fileType = NULL;  
    size_t count = m_aExtensions.GetCount();
    for ( size_t n = 0; n < count; n++ ) {
        wxString extensions = m_aExtensions[n];
        while ( !extensions.IsEmpty() ) {
            wxString field = extensions.BeforeFirst(wxT(' '));
            extensions = extensions.AfterFirst(wxT(' '));

            // consider extensions as not being case-sensitive
            if ( field.IsSameAs(ext, FALSE /* no case */) ) {
                // found
                if (fileType == NULL) fileType = new wxFileType;
                fileType->m_impl->Init(this, n);
                     // adds this mime type to _list_ of mime types with this extension
            }
        }
    }

    return fileType;
}

wxFileType *
wxMimeTypesManagerImpl::GetFileTypeFromMimeType(const wxString& mimeType)
{
    // mime types are not case-sensitive
    wxString mimetype(mimeType);
    mimetype.MakeLower();

    // first look for an exact match
    int index = m_aTypes.Index(mimetype);
    if ( index == wxNOT_FOUND ) {
        // then try to find "text/*" as match for "text/plain" (for example)
        // NB: if mimeType doesn't contain '/' at all, BeforeFirst() will return
        //     the whole string - ok.
        wxString strCategory = mimetype.BeforeFirst(wxT('/'));

        size_t nCount = m_aTypes.Count();
        for ( size_t n = 0; n < nCount; n++ ) {
            if ( (m_aTypes[n].BeforeFirst(wxT('/')) == strCategory ) &&
                 m_aTypes[n].AfterFirst(wxT('/')) == wxT("*") ) {
                    index = n;
                    break;
            }
        }
    }

    if ( index != wxNOT_FOUND ) {
        wxFileType *fileType = new wxFileType;
        fileType->m_impl->Init(this, index);

        return fileType;
    }
    else {
        // not found...
        return NULL;
    }
}

void wxMimeTypesManagerImpl::AddFallback(const wxFileTypeInfo& filetype)
{
    wxString extensions;
    const wxArrayString& exts = filetype.GetExtensions();
    size_t nExts = exts.GetCount();
    for ( size_t nExt = 0; nExt < nExts; nExt++ ) {
        if ( nExt > 0 ) {
            extensions += wxT(' ');
        }
        extensions += exts[nExt];
    }

    AddMimeTypeInfo(filetype.GetMimeType(),
                    extensions,
                    filetype.GetDescription());

    AddMailcapInfo(filetype.GetMimeType(),
                   filetype.GetOpenCommand(),
                   filetype.GetPrintCommand(),
                   wxT(""),
                   filetype.GetDescription());
}

void wxMimeTypesManagerImpl::AddMimeTypeInfo(const wxString& strMimeType,
                                             const wxString& strExtensions,
                                             const wxString& strDesc)
{
    int index = m_aTypes.Index(strMimeType);
    if ( index == wxNOT_FOUND ) {
        // add a new entry
        m_aTypes.Add(strMimeType);
        m_aEntries.Add(NULL);
        m_aExtensions.Add(strExtensions);
        m_aDescriptions.Add(strDesc);
    }
    else {
        // modify an existing one
        if ( !strDesc.IsEmpty() ) {
            m_aDescriptions[index] = strDesc;   // replace old value
        }
        m_aExtensions[index] += ' ' + strExtensions;
    }
}

void wxMimeTypesManagerImpl::AddMailcapInfo(const wxString& strType,
                                            const wxString& strOpenCmd,
                                            const wxString& strPrintCmd,
                                            const wxString& strTest,
                                            const wxString& strDesc)
{
    MailCapEntry *entry = new MailCapEntry(strOpenCmd, strPrintCmd, strTest);

    int nIndex = m_aTypes.Index(strType);
    if ( nIndex == wxNOT_FOUND ) {
        // new file type
        m_aTypes.Add(strType);

        m_aEntries.Add(entry);
        m_aExtensions.Add(wxT(""));
        m_aDescriptions.Add(strDesc);
    }
    else {
        // always append the entry in the tail of the list - info added with
        // this function can only come from AddFallbacks()
        MailCapEntry *entryOld = m_aEntries[nIndex];
        if ( entryOld )
            entry->Append(entryOld);
        else
            m_aEntries[nIndex] = entry;
    }
}

bool wxMimeTypesManagerImpl::ReadMimeTypes(const wxString& strFileName)
{
    wxLogTrace(wxT("--- Parsing mime.types file '%s' ---"), strFileName.c_str());

    wxTextFile file(strFileName);
    if ( !file.Open() )
        return FALSE;

    // the information we extract
    wxString strMimeType, strDesc, strExtensions;

    size_t nLineCount = file.GetLineCount();
    const wxChar *pc = NULL;
    for ( size_t nLine = 0; nLine < nLineCount; nLine++ ) {
        if ( pc == NULL ) {
            // now we're at the start of the line
            pc = file[nLine].c_str();
        }
        else {
            // we didn't finish with the previous line yet
            nLine--;
        }

        // skip whitespace
        while ( wxIsspace(*pc) )
            pc++;

        // comment or blank line?
        if ( *pc == wxT('#') || !*pc ) {
            // skip the whole line
            pc = NULL;
            continue;
        }

        // detect file format
        const wxChar *pEqualSign = wxStrchr(pc, wxT('='));
        if ( pEqualSign == NULL ) {
            // brief format
            // ------------

            // first field is mime type
            for ( strMimeType.Empty(); !wxIsspace(*pc) && *pc != wxT('\0'); pc++ ) {
                strMimeType += *pc;
            }

            // skip whitespace
            while ( wxIsspace(*pc) )
                pc++;

            // take all the rest of the string
            strExtensions = pc;

            // no description...
            strDesc.Empty();
        }
        else {
            // expanded format
            // ---------------

            // the string on the left of '=' is the field name
            wxString strLHS(pc, pEqualSign - pc);

            // eat whitespace
            for ( pc = pEqualSign + 1; wxIsspace(*pc); pc++ )
                ;

            const wxChar *pEnd;
            if ( *pc == wxT('"') ) {
                // the string is quoted and ends at the matching quote
                pEnd = wxStrchr(++pc, wxT('"'));
                if ( pEnd == NULL ) {
                    wxLogWarning(_("Mime.types file %s, line %d: unterminated "
                                   "quoted string."),
                                 strFileName.c_str(), nLine + 1);
                }
            }
            else {
                // unquoted string ends at the first space
                for ( pEnd = pc; !wxIsspace(*pEnd); pEnd++ )
                    ;
            }

            // now we have the RHS (field value)
            wxString strRHS(pc, pEnd - pc);

            // check what follows this entry
            if ( *pEnd == wxT('"') ) {
                // skip this quote
                pEnd++;
            }

            for ( pc = pEnd; wxIsspace(*pc); pc++ )
                ;

            // if there is something left, it may be either a '\\' to continue
            // the line or the next field of the same entry
            bool entryEnded = *pc == wxT('\0'),
                 nextFieldOnSameLine = FALSE;
            if ( !entryEnded ) {
                nextFieldOnSameLine = ((*pc != wxT('\\')) || (pc[1] != wxT('\0')));
            }

            // now see what we got
            if ( strLHS == wxT("type") ) {
                strMimeType = strRHS;
            }
            else if ( strLHS == wxT("desc") ) {
                strDesc = strRHS;
            }
            else if ( strLHS == wxT("exts") ) {
                strExtensions = strRHS;
            }
            else {
                wxLogWarning(_("Unknown field in file %s, line %d: '%s'."),
                             strFileName.c_str(), nLine + 1, strLHS.c_str());
            }

            if ( !entryEnded ) {
                if ( !nextFieldOnSameLine )
                    pc = NULL;
                //else: don't reset it

                // as we don't reset strMimeType, the next field in this entry
                // will be interpreted correctly.

                continue;
            }
        }

        // although it doesn't seem to be covered by RFCs, some programs
        // (notably Netscape) create their entries with several comma
        // separated extensions (RFC mention the spaces only)
        strExtensions.Replace(wxT(","), wxT(" "));

        // also deal with the leading dot
        if ( !strExtensions.IsEmpty() && strExtensions[0u] == wxT('.') )
        {
            strExtensions.erase(0, 1);
        }

        AddMimeTypeInfo(strMimeType, strExtensions, strDesc);

        // finished with this line
        pc = NULL;
    }

    // check our data integriry
    wxASSERT( m_aTypes.Count() == m_aEntries.Count() &&
              m_aTypes.Count() == m_aExtensions.Count() &&
              m_aTypes.Count() == m_aDescriptions.Count() );

    return TRUE;
}

bool wxMimeTypesManagerImpl::ReadMailcap(const wxString& strFileName,
                                         bool fallback)
{
    wxLogTrace(wxT("--- Parsing mailcap file '%s' ---"), strFileName.c_str());

    wxTextFile file(strFileName);
    if ( !file.Open() )
        return FALSE;

    // see the comments near the end of function for the reason we need these
    // variables (search for the next occurence of them)
        // indices of MIME types (in m_aTypes) we already found in this file
    wxArrayInt aEntryIndices;
        // aLastIndices[n] is the index of last element in
        // m_aEntries[aEntryIndices[n]] from this file
    wxArrayInt aLastIndices;

    size_t nLineCount = file.GetLineCount();
    for ( size_t nLine = 0; nLine < nLineCount; nLine++ ) {
        // now we're at the start of the line
        const wxChar *pc = file[nLine].c_str();

        // skip whitespace
        while ( wxIsspace(*pc) )
            pc++;

        // comment or empty string?
        if ( *pc == wxT('#') || *pc == wxT('\0') )
            continue;

        // no, do parse

        // what field are we currently in? The first 2 are fixed and there may
        // be an arbitrary number of other fields -- currently, we are not
        // interested in any of them, but we should parse them as well...
        enum
        {
            Field_Type,
            Field_OpenCmd,
            Field_Other
        } currentToken = Field_Type;

        // the flags and field values on the current line
        bool needsterminal = FALSE,
             copiousoutput = FALSE;
        wxString strType,
                 strOpenCmd,
                 strPrintCmd,
                 strTest,
                 strDesc,
                 curField; // accumulator
        for ( bool cont = TRUE; cont; pc++ ) {
            switch ( *pc ) {
                case wxT('\\'):
                    // interpret the next character literally (notice that
                    // backslash can be used for line continuation)
                    if ( *++pc == wxT('\0') ) {
                        // fetch the next line.

                        // pc currently points to nowhere, but after the next
                        // pc++ in the for line it will point to the beginning
                        // of the next line in the file
                        pc = file[++nLine].c_str() - 1;
                    }
                    else {
                        // just a normal character
                        curField += *pc;
                    }
                    break;

                case wxT('\0'):
                    cont = FALSE;   // end of line reached, exit the loop

                    // fall through

                case wxT(';'):
                    // store this field and start looking for the next one

                    // trim whitespaces from both sides
                    curField.Trim(TRUE).Trim(FALSE);

                    switch ( currentToken ) {
                        case Field_Type:
                            strType = curField;
                            if ( strType.Find(wxT('/')) == wxNOT_FOUND ) {
                                // we interpret "type" as "type/*"
                                strType += wxT("/*");
                            }

                            currentToken = Field_OpenCmd;
                            break;

                        case Field_OpenCmd:
                            strOpenCmd = curField;

                            currentToken = Field_Other;
                            break;

                        case Field_Other:
                            {
                                // "good" mailcap entry?
                                bool ok = TRUE;

                                // is this something of the form foo=bar?
                                const wxChar *pEq = wxStrchr(curField, wxT('='));
                                if ( pEq != NULL ) {
                                    wxString lhs = curField.BeforeFirst(wxT('=')),
                                             rhs = curField.AfterFirst(wxT('='));

                                    lhs.Trim(TRUE);     // from right
                                    rhs.Trim(FALSE);    // from left

                                    if ( lhs == wxT("print") )
                                        strPrintCmd = rhs;
                                    else if ( lhs == wxT("test") )
                                        strTest = rhs;
                                    else if ( lhs == wxT("description") ) {
                                        // it might be quoted
                                        if ( rhs[0u] == wxT('"') &&
                                             rhs.Last() == wxT('"') ) {
                                            strDesc = wxString(rhs.c_str() + 1,
                                                               rhs.Len() - 2);
                                        }
                                        else {
                                            strDesc = rhs;
                                        }
                                    }
                                    else if ( lhs == wxT("compose") ||
                                              lhs == wxT("composetyped") ||
                                              lhs == wxT("notes") ||
                                              lhs == wxT("edit") )
                                        ;   // ignore
                                    else
                                        ok = FALSE;

                                }
                                else {
                                    // no, it's a simple flag
                                    // TODO support the flags:
                                    //  1. create an xterm for 'needsterminal'
                                    //  2. append "| $PAGER" for 'copiousoutput'
                                    if ( curField == wxT("needsterminal") )
                                        needsterminal = TRUE;
                                    else if ( curField == wxT("copiousoutput") )
                                        copiousoutput = TRUE;
                                    else if ( curField == wxT("textualnewlines") )
                                        ;   // ignore
                                    else
                                        ok = FALSE;
                                }

                                if ( !ok )
                                {
                                    // we don't understand this field, but
                                    // Netscape stores info in it, so don't warn
                                    // about it
                                    if ( curField.Left(16u) != "x-mozilla-flags=" )
                                    {
                                        // don't flood the user with error
                                        // messages if we don't understand
                                        // something in his mailcap, but give
                                        // them in debug mode because this might
                                        // be useful for the programmer
                                        wxLogDebug
                                        (
                                          wxT("Mailcap file %s, line %d: "
                                              "unknown field '%s' for the "
                                              "MIME type '%s' ignored."),
                                              strFileName.c_str(),
                                              nLine + 1,
                                              curField.c_str(),
                                              strType.c_str()
                                        );
                                    }
                                }
                            }

                            // it already has this value
                            //currentToken = Field_Other;
                            break;

                        default:
                            wxFAIL_MSG(wxT("unknown field type in mailcap"));
                    }

                    // next token starts immediately after ';'
                    curField.Empty();
                    break;

                default:
                    curField += *pc;
            }
        }

        // check that we really read something reasonable
        if ( currentToken == Field_Type || currentToken == Field_OpenCmd ) {
            wxLogWarning(_("Mailcap file %s, line %d: incomplete entry "
                           "ignored."),
                         strFileName.c_str(), nLine + 1);
        }
        else {
            MailCapEntry *entry = new MailCapEntry(strOpenCmd,
                                                   strPrintCmd,
                                                   strTest);

            // NB: because of complications below (we must get entries priority
            //     right), we can't use AddMailcapInfo() here, unfortunately.
            strType.MakeLower();
            int nIndex = m_aTypes.Index(strType);
            if ( nIndex == wxNOT_FOUND ) {
                // new file type
                m_aTypes.Add(strType);

                m_aEntries.Add(entry);
                m_aExtensions.Add(wxT(""));
                m_aDescriptions.Add(strDesc);
            }
            else {
                // modify the existing entry: the entries in one and the same
                // file are read in top-to-bottom order, i.e. the entries read
                // first should be tried before the entries below. However,
                // the files read later should override the settings in the
                // files read before (except if fallback is TRUE), thus we
                // Insert() the new entry to the list if it has already
                // occured in _this_ file, but Prepend() it if it occured in
                // some of the previous ones and Append() to it in the
                // fallback case

                if ( fallback ) {
                    // 'fallback' parameter prevents the entries from this
                    // file from overriding the other ones - always append
                    MailCapEntry *entryOld = m_aEntries[nIndex];
                    if ( entryOld )
                        entry->Append(entryOld);
                    else
                        m_aEntries[nIndex] = entry;
                }
                else {
                    int entryIndex = aEntryIndices.Index(nIndex);
                    if ( entryIndex == wxNOT_FOUND ) {
                        // first time in this file
                        aEntryIndices.Add(nIndex);
                        aLastIndices.Add(0);

                        entry->Prepend(m_aEntries[nIndex]);
                        m_aEntries[nIndex] = entry;
                    }
                    else {
                        // not the first time in _this_ file
                        size_t nEntryIndex = (size_t)entryIndex;
                        MailCapEntry *entryOld = m_aEntries[nIndex];
                        if ( entryOld )
                            entry->Insert(entryOld, aLastIndices[nEntryIndex]);
                        else
                            m_aEntries[nIndex] = entry;

                        // the indices were shifted by 1
                        aLastIndices[nEntryIndex]++;
                    }
                }

                if ( !strDesc.IsEmpty() ) {
                    // replace the old one - what else can we do??
                    m_aDescriptions[nIndex] = strDesc;
                }
            }
        }

        // check our data integriry
        wxASSERT( m_aTypes.Count() == m_aEntries.Count() &&
                  m_aTypes.Count() == m_aExtensions.Count() &&
                  m_aTypes.Count() == m_aDescriptions.Count() );
    }

    return TRUE;
}

size_t wxMimeTypesManagerImpl::EnumAllFileTypes(wxArrayString& mimetypes)
{
    mimetypes.Empty();

    wxString type;
    size_t count = m_aTypes.GetCount();
    for ( size_t n = 0; n < count; n++ )
    {
        // don't return template types from here (i.e. anything containg '*')
        type = m_aTypes[n];
        if ( type.Find(_T('*')) == wxNOT_FOUND )
        {
            mimetypes.Add(type);
        }
    }

    return mimetypes.GetCount();
}

#endif
  // wxUSE_FILE && wxUSE_TEXTFILE

