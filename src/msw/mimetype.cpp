/////////////////////////////////////////////////////////////////////////////
// Name:        msw/mimetype.cpp
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

// this is Win32 only code
#ifndef __WIN16__

#ifndef WX_PRECOMP
    #include "wx/string.h"
    #if wxUSE_GUI
        #include "wx/icon.h"
        #include "wx/msgdlg.h"
    #endif
#endif //WX_PRECOMP

#include "wx/log.h"
#include "wx/file.h"
#include "wx/intl.h"
#include "wx/dynarray.h"
#include "wx/confbase.h"

#ifdef __WXMSW__
    #include "wx/msw/registry.h"
    #include "windows.h"
#endif // OS

#include "wx/msw/mimetype.h"

// other standard headers
#include <ctype.h>

// in case we're compiling in non-GUI mode
class WXDLLEXPORT wxIcon;

// These classes use Windows registry to retrieve the required information.
//
// Keys used (not all of them are documented, so it might actually stop working
// in future versions of Windows...):
//  1. "HKCR\MIME\Database\Content Type" contains subkeys for all known MIME
//     types, each key has a string value "Extension" which gives (dot preceded)
//     extension for the files of this MIME type.
//
//  2. "HKCR\.ext" contains
//   a) unnamed value containing the "filetype"
//   b) value "Content Type" containing the MIME type
//
// 3. "HKCR\filetype" contains
//   a) unnamed value containing the description
//   b) subkey "DefaultIcon" with single unnamed value giving the icon index in
//      an icon file
//   c) shell\open\command and shell\open\print subkeys containing the commands
//      to open/print the file (the positional parameters are introduced by %1,
//      %2, ... in these strings, we change them to %s ourselves)

// although I don't know of any official documentation which mentions this
// location, uses it, so it isn't likely to change
static const wxChar *MIME_DATABASE_KEY = wxT("MIME\\Database\\Content Type\\");

void wxFileTypeImpl::Init(const wxString& strFileType, const wxString& ext)
{
    // VZ: does it? (FIXME)
    wxCHECK_RET( !ext.IsEmpty(), _T("needs an extension") );

    if ( ext[0u] != wxT('.') ) {
        m_ext = wxT('.');
    }
    m_ext << ext;

    m_strFileType = strFileType;
    if ( !strFileType ) {
        m_strFileType = m_ext.AfterFirst('.') + "_auto_file";
    }
}

wxString wxFileTypeImpl::GetVerbPath(const wxString& verb) const
{
    wxString path;
    path << m_strFileType << _T("\\shell\\") << verb << _T("\\command");
    return path;
}

size_t wxFileTypeImpl::GetAllCommands(wxArrayString *verbs,
                                      wxArrayString *commands,
                                      const wxFileType::MessageParameters& params) const
{
    wxCHECK_MSG( !m_ext.IsEmpty(), 0, _T("GetAllCommands() needs an extension") );

    if ( m_strFileType.IsEmpty() )
    {
        // get it from the registry
        wxFileTypeImpl *self = wxConstCast(this, wxFileTypeImpl);
        wxRegKey rkey(wxRegKey::HKCR, m_ext);
        if ( !rkey.Exists() || !rkey.QueryValue(_T(""), self->m_strFileType) )
        {
            wxLogDebug(_T("Can't get the filetype for extension '%s'."),
                       m_ext.c_str());

            return 0;
        }
    }

    // enum all subkeys of HKCR\filetype\shell
    size_t count = 0;
    wxRegKey rkey(wxRegKey::HKCR, m_strFileType  + _T("\\shell"));
    long dummy;
    wxString verb;
    bool ok = rkey.GetFirstKey(verb, dummy);
    while ( ok )
    {
        wxString command = wxFileType::ExpandCommand(GetCommand(verb), params);

        // we want the open bverb to eb always the first

        if ( verb.CmpNoCase(_T("open")) == 0 )
        {
            if ( verbs )
                verbs->Insert(verb, 0);
            if ( commands )
                commands->Insert(command, 0);
        }
        else // anything else than "open"
        {
            if ( verbs )
                verbs->Add(verb);
            if ( commands )
                commands->Add(command);
        }

        count++;

        ok = rkey.GetNextKey(verb, dummy);
    }

    return count;
}

// ----------------------------------------------------------------------------
// modify the registry database
// ----------------------------------------------------------------------------

bool wxFileTypeImpl::EnsureExtKeyExists()
{
    wxRegKey rkey(wxRegKey::HKCR, m_ext);
    if ( !rkey.Exists() )
    {
        if ( !rkey.Create() || !rkey.SetValue(_T(""), m_strFileType) )
        {
            wxLogError(_("Failed to create registry entry for '%s' files."),
                       m_ext.c_str());
            return FALSE;
        }
    }

    return TRUE;
}

// ----------------------------------------------------------------------------
// get the command to use
// ----------------------------------------------------------------------------

wxString wxFileTypeImpl::GetCommand(const wxChar *verb) const
{
    // suppress possible error messages
    wxLogNull nolog;
    wxString strKey;

    if ( wxRegKey(wxRegKey::HKCR, m_ext + _T("\\shell")).Exists() )
        strKey = m_ext;
    if ( wxRegKey(wxRegKey::HKCR, m_strFileType + _T("\\shell")).Exists() )
        strKey = m_strFileType;

    if ( !strKey )
    {
        // no info
        return wxEmptyString;
    }

    strKey << wxT("\\shell\\") << verb;
    wxRegKey key(wxRegKey::HKCR, strKey + _T("\\command"));
    wxString command;
    if ( key.Open() ) {
        // it's the default value of the key
        if ( key.QueryValue(wxT(""), command) ) {
            // transform it from '%1' to '%s' style format string (now also
            // test for %L - apparently MS started using it as well for the
            // same purpose)

            // NB: we don't make any attempt to verify that the string is valid,
            //     i.e. doesn't contain %2, or second %1 or .... But we do make
            //     sure that we return a string with _exactly_ one '%s'!
            bool foundFilename = FALSE;
            size_t len = command.Len();
            for ( size_t n = 0; (n < len) && !foundFilename; n++ ) {
                if ( command[n] == wxT('%') &&
                     (n + 1 < len) &&
                     (command[n + 1] == wxT('1') ||
                      command[n + 1] == wxT('L')) ) {
                    // replace it with '%s'
                    command[n + 1] = wxT('s');

                    foundFilename = TRUE;
                }
            }

#if wxUSE_IPC
            // look whether we must issue some DDE requests to the application
            // (and not just launch it)
            strKey += _T("\\DDEExec");
            wxRegKey keyDDE(wxRegKey::HKCR, strKey);
            if ( keyDDE.Open() ) {
                wxString ddeCommand, ddeServer, ddeTopic;
                keyDDE.QueryValue(_T(""), ddeCommand);
                ddeCommand.Replace(_T("%1"), _T("%s"));

                wxRegKey(wxRegKey::HKCR, strKey + _T("\\Application")).
                    QueryValue(_T(""), ddeServer);
                wxRegKey(wxRegKey::HKCR, strKey + _T("\\Topic")).
                    QueryValue(_T(""), ddeTopic);

                // HACK: we use a special feature of wxExecute which exists
                //       just because we need it here: it will establish DDE
                //       conversation with the program it just launched
                command.Prepend(_T("WX_DDE#"));
                command << _T('#') << ddeServer
                        << _T('#') << ddeTopic
                        << _T('#') << ddeCommand;
            }
            else
#endif // wxUSE_IPC
                if ( !foundFilename ) {
                // we didn't find any '%1' - the application doesn't know which
                // file to open (note that we only do it if there is no DDEExec
                // subkey)
                //
                // HACK: append the filename at the end, hope that it will do
                command << wxT(" %s");
            }
        }
    }
    //else: no such file type or no value, will return empty string

    return command;
}

bool
wxFileTypeImpl::GetOpenCommand(wxString *openCmd,
                               const wxFileType::MessageParameters& params)
                               const
{
    wxString cmd = GetCommand(wxT("open"));

    *openCmd = wxFileType::ExpandCommand(cmd, params);

    return !openCmd->IsEmpty();
}

bool
wxFileTypeImpl::GetPrintCommand(wxString *printCmd,
                                const wxFileType::MessageParameters& params)
                                const
{
    wxString cmd = GetCommand(wxT("print"));

    *printCmd = wxFileType::ExpandCommand(cmd, params);

    return !printCmd->IsEmpty();
}

// ----------------------------------------------------------------------------
// getting other stuff
// ----------------------------------------------------------------------------

// TODO this function is half implemented
bool wxFileTypeImpl::GetExtensions(wxArrayString& extensions)
{
    if ( m_ext.IsEmpty() ) {
        // the only way to get the list of extensions from the file type is to
        // scan through all extensions in the registry - too slow...
        return FALSE;
    }
    else {
        extensions.Empty();
        extensions.Add(m_ext);

        // it's a lie too, we don't return _all_ extensions...
        return TRUE;
    }
}

bool wxFileTypeImpl::GetMimeType(wxString *mimeType) const
{
    // suppress possible error messages
    wxLogNull nolog;
    wxRegKey key(wxRegKey::HKCR, m_ext);

    return key.Open() && key.QueryValue(wxT("Content Type"), *mimeType);
}

bool wxFileTypeImpl::GetMimeTypes(wxArrayString& mimeTypes) const
{
    wxString s;

    if ( !GetMimeType(&s) )
    {
        return FALSE;
    }

    mimeTypes.Clear();
    mimeTypes.Add(s);
    return TRUE;
}


bool wxFileTypeImpl::GetIcon(wxIcon *icon,
                             wxString *iconFile,
                             int *iconIndex) const
{
#if wxUSE_GUI
    wxString strIconKey;
    strIconKey << m_strFileType << wxT("\\DefaultIcon");

    // suppress possible error messages
    wxLogNull nolog;
    wxRegKey key(wxRegKey::HKCR, strIconKey);

    if ( key.Open() ) {
        wxString strIcon;
        // it's the default value of the key
        if ( key.QueryValue(wxT(""), strIcon) ) {
            // the format is the following: <full path to file>, <icon index>
            // NB: icon index may be negative as well as positive and the full
            //     path may contain the environment variables inside '%'
            wxString strFullPath = strIcon.BeforeLast(wxT(',')),
            strIndex = strIcon.AfterLast(wxT(','));

            // index may be omitted, in which case BeforeLast(',') is empty and
            // AfterLast(',') is the whole string
            if ( strFullPath.IsEmpty() ) {
                strFullPath = strIndex;
                strIndex = wxT("0");
            }

            wxString strExpPath = wxExpandEnvVars(strFullPath);
            // here we need C based counting!
            int nIndex = wxAtoi(strIndex);

            HICON hIcon = ExtractIcon(GetModuleHandle(NULL), strExpPath, nIndex);
            switch ( (int)hIcon ) {
                case 0: // means no icons were found
                case 1: // means no such file or it wasn't a DLL/EXE/OCX/ICO/...
                    wxLogDebug(wxT("incorrect registry entry '%s': no such icon."),
                               key.GetName().c_str());
                    break;

                default:
                    icon->SetHICON((WXHICON)hIcon);
                    if ( iconIndex )
                        *iconIndex = nIndex;
                    if ( iconFile )
                        *iconFile = strFullPath;
                    return TRUE;
            }
        }
    }

    // no such file type or no value or incorrect icon entry
#endif // wxUSE_GUI

    return FALSE;
}

bool wxFileTypeImpl::GetDescription(wxString *desc) const
{
    // suppress possible error messages
    wxLogNull nolog;
    wxRegKey key(wxRegKey::HKCR, m_strFileType);

    if ( key.Open() ) {
        // it's the default value of the key
        if ( key.QueryValue(wxT(""), *desc) ) {
            return TRUE;
        }
    }

    return FALSE;
}

// helper function
wxFileType *
wxMimeTypesManagerImpl::CreateFileType(const wxString& filetype, const wxString& ext)
{
    wxFileType *fileType = new wxFileType;
    fileType->m_impl->Init(filetype, ext);
    return fileType;
}

// extension -> file type
wxFileType *
wxMimeTypesManagerImpl::GetFileTypeFromExtension(const wxString& ext)
{
    // add the leading point if necessary
    wxString str;
    if ( ext[0u] != wxT('.') ) {
        str = wxT('.');
    }
    str << ext;

    // suppress possible error messages
    wxLogNull nolog;

    bool knownExtension = FALSE;

    wxString strFileType;
    wxRegKey key(wxRegKey::HKCR, str);
    if ( key.Open() ) {
        // it's the default value of the key
        if ( key.QueryValue(wxT(""), strFileType) ) {
            // create the new wxFileType object
            return CreateFileType(strFileType, ext);
        }
        else {
            // this extension doesn't have a filetype, but it's known to the
            // system and may be has some other useful keys (open command or
            // content-type), so still return a file type object for it
            knownExtension = TRUE;
        }
    }

    if ( !knownExtension )
    {
        // unknown extension
        return NULL;
    }

    return CreateFileType(wxEmptyString, ext);
}

/*
wxFileType *
wxMimeTypesManagerImpl::GetOrAllocateFileTypeFromExtension(const wxString& ext)
{
    wxFileType *fileType = GetFileTypeFromExtension(ext);
    if ( !fileType )
    {
        fileType = CreateFileType(wxEmptyString, ext);
    }

    return fileType;
}
*/

// MIME type -> extension -> file type
wxFileType *
wxMimeTypesManagerImpl::GetFileTypeFromMimeType(const wxString& mimeType)
{
    wxString strKey = MIME_DATABASE_KEY;
    strKey << mimeType;

    // suppress possible error messages
    wxLogNull nolog;

    wxString ext;
    wxRegKey key(wxRegKey::HKCR, strKey);
    if ( key.Open() ) {
        if ( key.QueryValue(wxT("Extension"), ext) ) {
            return GetFileTypeFromExtension(ext);
        }
    }

    // unknown MIME type
    return NULL;
}

size_t wxMimeTypesManagerImpl::EnumAllFileTypes(wxArrayString& mimetypes)
{
    // enumerate all keys under MIME_DATABASE_KEY
    wxRegKey key(wxRegKey::HKCR, MIME_DATABASE_KEY);

    wxString type;
    long cookie;
    bool cont = key.GetFirstKey(type, cookie);
    while ( cont )
    {
        mimetypes.Add(type);

        cont = key.GetNextKey(type, cookie);
    }

    return mimetypes.GetCount();
}

// ----------------------------------------------------------------------------
// create a new association
// ----------------------------------------------------------------------------

wxFileType *wxMimeTypesManagerImpl::Associate(const wxFileTypeInfo& ftInfo)
{
    wxCHECK_MSG( !ftInfo.GetExtensions().IsEmpty(), NULL,
                 _T("Associate() needs extension") );

    bool ok = FALSE ;
    int iExtCount = 0 ;
    wxString filetype;
    wxString extWithDot;

    wxString ext = ftInfo.GetExtensions()[iExtCount];

    wxCHECK_MSG( !ext.empty(), NULL,
                 _T("Associate() needs non empty extension") );

    if ( ext[0u] != _T('.') )
        extWithDot = _T('.');
    extWithDot += ext;

    // start by setting the HKCR\\.ext entries
    // default is filetype; content type is mimetype
    const wxString& filetypeOrig = ftInfo.GetShortDesc();

    wxRegKey key(wxRegKey::HKCR, extWithDot);
    if ( !key.Exists() )
    {
        // create the mapping from the extension to the filetype
        ok = key.Create();
        if ( ok )
        {

            if ( filetypeOrig.empty() )
            {
                // make it up from the extension
                filetype << extWithDot.c_str() + 1 << _T("_file");
            }
            else
            {
                // just use the provided one
                filetype = filetypeOrig;
            }

            ok = key.SetValue(_T(""), filetype);
        }
        }
        else
        {
            // key already exists, maybe we want to change it ??
            if (!filetypeOrig.empty())
                {
                    filetype = filetypeOrig;
                    ok = key.SetValue(_T(""), filetype);
                }
            else
                {
                    ok = key.QueryValue(_T(""), filetype);
                }
        }
        // now set a mimetypeif we have it, but ignore it if none
        const wxString& mimetype = ftInfo.GetMimeType();
        if ( !mimetype.empty() )
        {
            // set the MIME type
            ok = key.SetValue(_T("Content Type"), mimetype);

            if ( ok )
            {
                // create the MIME key
                wxString strKey = MIME_DATABASE_KEY;
                strKey << mimetype;
                wxRegKey keyMIME(wxRegKey::HKCR, strKey);
                ok = keyMIME.Create();

                if ( ok )
                {
                    // and provide a back link to the extension
                    ok = keyMIME.SetValue(_T("Extension"), extWithDot);
                }
            }
        }


    // now make other extensions have the same filetype
    
    for (iExtCount=1; iExtCount < ftInfo.GetExtensionsCount(); iExtCount++ )
        {
            ext = ftInfo.GetExtensions()[iExtCount];
            if ( ext[0u] != _T('.') )
               extWithDot = _T('.');
            extWithDot += ext;

            wxRegKey key(wxRegKey::HKCR, extWithDot);
            if ( !key.Exists() ) ok = key.Create();
            ok = key.SetValue(_T(""), filetype);

        // now set any mimetypes we may have, but ignore it if none
        const wxString& mimetype = ftInfo.GetMimeType();
        if ( !mimetype.empty() )
        {
            // set the MIME type
            ok = key.SetValue(_T("Content Type"), mimetype);

        if ( ok )
        {
                // create the MIME key
                wxString strKey = MIME_DATABASE_KEY;
                strKey << mimetype;
                wxRegKey keyMIME(wxRegKey::HKCR, strKey);
                ok = keyMIME.Create();

        if ( ok )
        {
                    // and provide a back link to the extension
                    ok = keyMIME.SetValue(_T("Extension"), extWithDot);
        }
        }
    }


    } // end of for loop; all extensions now point to HKCR\.ext\Default

    // create the filetype key itself (it will be empty for now, but
    // SetCommand(), SetDefaultIcon() &c will use it later)
    wxRegKey keyFT(wxRegKey::HKCR, filetype);
    ok = keyFT.Create();
    
    wxFileType *ft = NULL; 
    ft = CreateFileType(filetype, extWithDot);

    if (ft)
    {
            if (! ftInfo.GetOpenCommand ().IsEmpty() ) ft->SetCommand (ftInfo.GetOpenCommand (), wxT("open"  ) );
            if (! ftInfo.GetPrintCommand().IsEmpty() ) ft->SetCommand (ftInfo.GetPrintCommand(), wxT("print" ) );
            // chris: I don't like the ->m_impl-> here FIX this ??
            if (! ftInfo.GetDescription ().IsEmpty() ) ft->m_impl->SetDescription (ftInfo.GetDescription ()) ;
            if (! ftInfo.GetIconFile().IsEmpty() ) ft->SetDefaultIcon (ftInfo.GetIconFile(), ftInfo.GetIconIndex() );

        }
    return ft;
}

bool wxFileTypeImpl::SetCommand(const wxString& cmd,
                                const wxString& verb,
                                bool overwriteprompt)
{
    wxCHECK_MSG( !m_ext.IsEmpty() && !verb.IsEmpty(), FALSE,
                 _T("SetCommand() needs an extension and a verb") );

    if ( !EnsureExtKeyExists() )
        return FALSE;

    wxRegKey rkey(wxRegKey::HKCR, GetVerbPath(verb));
#if 0
    if ( rkey.Exists() && overwriteprompt )
    {
#if wxUSE_GUI
        wxString old;
        rkey.QueryValue(wxT(""), old);
        if ( wxMessageBox
             (
                wxString::Format(
                    _("Do you want to overwrite the command used to %s "
                      "files with extension \"%s\" ?\nCurrent value is \n%s, "
                      "\nNew value is \n%s %1"), // bug here FIX need %1 ??
                    verb.c_str(),
                    m_ext.c_str(),
                    old.c_str(),
                    cmd.c_str()),
                _("Confirm registry update"),
                wxYES_NO | wxICON_QUESTION
             ) != wxYES )
#endif // wxUSE_GUI
        {
            // cancelled by user
            return FALSE;
        }
    }
#endif
    // TODO:
    // 1. translate '%s' to '%1' instead of always adding it
    // 2. create DDEExec value if needed (undo GetCommand)
    return rkey.Create() && rkey.SetValue(_T(""), cmd + _T(" \"%1\"") );
}

/* // no longer used
bool wxFileTypeImpl::SetMimeType(const wxString& mimeTypeOrig)
{
    wxCHECK_MSG( !m_ext.IsEmpty(), FALSE, _T("SetMimeType() needs extension") );

    if ( !EnsureExtKeyExists() )
        return FALSE;

    // VZ: is this really useful? (FIXME)
    wxString mimeType;
    if ( !mimeTypeOrig )
    {
        // make up a default value for it
        wxString cmd;
        wxSplitPath(GetCommand(_T("open")), NULL, &cmd, NULL);
        mimeType << _T("application/x-") << cmd;
    }
    else
    {
        mimeType = mimeTypeOrig;
    }

    wxRegKey rkey(wxRegKey::HKCR, m_ext);
    return rkey.Create() && rkey.SetValue(_T("Content Type"), mimeType);
}
*/

bool wxFileTypeImpl::SetDefaultIcon(const wxString& cmd, int index)
{
    wxCHECK_MSG( !m_ext.IsEmpty(), FALSE, _T("SetDefaultIcon() needs extension") );
    wxCHECK_MSG( !m_strFileType.IsEmpty(), FALSE, _T("File key not found") );
//    the next line fails on a SMBshare, I think because it is case mangled
//    wxCHECK_MSG( !wxFileExists(cmd), FALSE, _T("Icon file not found.") );

    if ( !EnsureExtKeyExists() )
        return FALSE;

    wxRegKey rkey(wxRegKey::HKCR, m_strFileType + _T("\\DefaultIcon"));

    return rkey.Create() &&
           rkey.SetValue(_T(""),
                         wxString::Format(_T("%s,%d"), cmd.c_str(), index));
}

bool wxFileTypeImpl::SetDescription (const wxString& desc)
{
    wxCHECK_MSG( !m_strFileType.IsEmpty(), FALSE, _T("File key not found") );
    wxCHECK_MSG( !desc.IsEmpty(), FALSE, _T("No file description supplied") );

    if ( !EnsureExtKeyExists() )
        return FALSE;

    wxRegKey rkey(wxRegKey::HKCR, m_strFileType );

    return rkey.Create() &&
           rkey.SetValue(_T(""), desc);
}

// ----------------------------------------------------------------------------
// remove file association
// ----------------------------------------------------------------------------

bool wxFileTypeImpl::Unassociate()
{
    bool result = TRUE;
    if ( !RemoveOpenCommand() )
        result = FALSE;
    if ( !RemoveDefaultIcon() )
        result = FALSE;
    if ( !RemoveMimeType() )
        result = FALSE;
   if ( !RemoveDescription() )
        result = FALSE;

/*
    //this might hold other keys, eg some have CSLID keys
    if ( result )
    {
        // delete the root key
        wxRegKey key(wxRegKey::HKCR, m_ext);
        if ( key.Exists() )
            result = key.DeleteSelf();
    }
*/
    return result;
}

bool wxFileTypeImpl::RemoveOpenCommand()
{
   return RemoveCommand(_T("open"));
}

bool wxFileTypeImpl::RemoveCommand(const wxString& verb)
{
    wxCHECK_MSG( !m_ext.IsEmpty() && !verb.IsEmpty(), FALSE,
                 _T("RemoveCommand() needs an extension and a verb") );

    wxString  sKey = m_strFileType;
    wxRegKey rkey(wxRegKey::HKCR, GetVerbPath(verb));

    // if the key already doesn't exist, it's a success
    return !rkey.Exists() || rkey.DeleteSelf();
}

bool wxFileTypeImpl::RemoveMimeType()
{
    wxCHECK_MSG( !m_ext.IsEmpty(), FALSE, _T("RemoveMimeType() needs extension") );

    wxRegKey rkey(wxRegKey::HKCR, m_ext);
    return !rkey.Exists() || rkey.DeleteSelf();
}

bool wxFileTypeImpl::RemoveDefaultIcon()
{
    wxCHECK_MSG( !m_ext.IsEmpty(), FALSE,
                 _T("RemoveDefaultIcon() needs extension") );

    wxRegKey rkey (wxRegKey::HKCR, m_strFileType  + _T("\\DefaultIcon"));
    return !rkey.Exists() || rkey.DeleteSelf();
}

bool wxFileTypeImpl::RemoveDescription()
{
    wxCHECK_MSG( !m_ext.IsEmpty(), FALSE,
                 _T("RemoveDescription() needs extension") );

    wxRegKey rkey (wxRegKey::HKCR, m_strFileType );
    return !rkey.Exists() || rkey.DeleteSelf();
}

#endif
  // __WIN16__
