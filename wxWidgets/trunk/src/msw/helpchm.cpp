/////////////////////////////////////////////////////////////////////////////
// Name:        helpchm.cpp
// Purpose:     Help system: MS HTML Help implementation
// Author:      Julian Smart
// Modified by:
// Created:     16/04/2000
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "helpchm.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/defs.h"
#endif

#include "wx/filefn.h"

#if wxUSE_HELP && wxUSE_MS_HTML_HELP && defined(__WIN95__)
#include "wx/msw/helpchm.h"

#include "wx/dynlib.h"

#ifndef WX_PRECOMP
#include <windows.h>
#endif

// This is found in the HTML Help Workshop installation,
// along with htmlhelp.lib.
#include <htmlhelp.h>

#include <time.h>

#ifdef __WXMSW__
#include "wx/msw/private.h"
#endif

#include <string.h>

// utility functions to manage the loading/unloading
// of hhctrl.ocx
#ifndef UNICODE
typedef HWND ( WINAPI * HTMLHELP )( HWND, LPCSTR, UINT, DWORD );
#define HTMLHELP_NAME "HtmlHelpA"
#else
typedef HWND ( WINAPI * HTMLHELP )( HWND, LPCWSTR, UINT, DWORD );
#define HTMLHELP_NAME "HtmlHelpW"
#endif
// dll handle/reference count
static HTMLHELP gs_htmlHelp = 0;
static wxDllType gs_dllHandle = 0;
static int gs_dllCount = 0;

static bool LoadHtmlHelpLibrary()
{
    if( !gs_dllCount )
    {
        gs_dllHandle = wxDllLoader::LoadLibrary( "hhctrl.ocx" );
        if( !gs_dllHandle )
        {
            wxLogError(_("MS HTML Help functions are unavailable because the MS HTML Help library is not installed on this machine. Please install it."));
            return FALSE;
        }
        else
        {
            gs_dllCount = 1;
            gs_htmlHelp = (HTMLHELP)wxDllLoader::GetSymbol( gs_dllHandle, HTMLHELP_NAME );

            if( !gs_htmlHelp )
            {
                wxLogError(_("Failed to initialize MS HTML Help."));

                wxDllLoader::UnloadLibrary(gs_dllHandle);
                gs_dllHandle = 0;
                gs_dllCount = 0;

                return FALSE ;
            }
        }
    }
    else
    {
        ++gs_dllCount;
    }

    return TRUE;
}

static void UnloadHtmlHelpLibrary()
{
    if( gs_dllCount != 0 && !--gs_dllCount )
    {
        wxDllLoader::UnloadLibrary( gs_dllHandle );
        gs_dllHandle = 0;
        gs_htmlHelp = 0;
    }
}

static HWND GetSuitableHWND()
{
    if (wxTheApp->GetTopWindow())
        return (HWND) wxTheApp->GetTopWindow()->GetHWND();
    else
        return GetDesktopWindow();
}

IMPLEMENT_DYNAMIC_CLASS(wxCHMHelpController, wxHelpControllerBase)

bool wxCHMHelpController::Initialize(const wxString& filename)
{
    // warn on failure
    if( !LoadHtmlHelpLibrary() )
        return FALSE;

    m_helpFile = filename;
    return TRUE;
}

bool wxCHMHelpController::LoadFile(const wxString& file)
{
    if (!file.IsEmpty())
        m_helpFile = file;
    return TRUE;
}

bool wxCHMHelpController::DisplayContents()
{
    if (m_helpFile.IsEmpty()) return FALSE;

    wxString str = GetValidFilename(m_helpFile);

    gs_htmlHelp(GetSuitableHWND(), (const wxChar*) str, HH_HELP_FINDER, 0L);
    return TRUE;
}

// Use topic or HTML filename
bool wxCHMHelpController::DisplaySection(const wxString& section)
{
    if (m_helpFile.IsEmpty()) return FALSE;

    wxString str = GetValidFilename(m_helpFile);

    // Is this an HTML file or a keyword?
    bool isFilename = (section.Find(wxT(".htm")) != -1);

    if (isFilename)
        gs_htmlHelp(GetSuitableHWND(), (const wxChar*) str, HH_DISPLAY_TOPIC, (DWORD) (const wxChar*) section);
    else
        KeywordSearch(section);
    return TRUE;
}

// Use context number
bool wxCHMHelpController::DisplaySection(int section)
{
    if (m_helpFile.IsEmpty()) return FALSE;

    wxString str = GetValidFilename(m_helpFile);

    gs_htmlHelp(GetSuitableHWND(), (const wxChar*) str, HH_HELP_CONTEXT, (DWORD)section);
    return TRUE;
}

bool wxCHMHelpController::DisplayContextPopup(int contextId)
{
    if (m_helpFile.IsEmpty()) return FALSE;

    wxString str = GetValidFilename(m_helpFile);

    // We also have to specify the popups file (default is cshelp.txt).
    // str += wxT("::/cshelp.txt");

    HH_POPUP popup;
    popup.cbStruct = sizeof(popup);
    popup.hinst = (HINSTANCE) wxGetInstance();
    popup.idString = contextId ;

    GetCursorPos(& popup.pt);
    popup.clrForeground = (COLORREF)-1;
    popup.clrBackground = (COLORREF)-1;
    popup.rcMargins.top = popup.rcMargins.left = popup.rcMargins.right = popup.rcMargins.bottom = -1;
    popup.pszFont = NULL;
    popup.pszText = NULL;

    gs_htmlHelp(GetSuitableHWND(), (const wxChar*) str, HH_DISPLAY_TEXT_POPUP, (DWORD) & popup);
    return TRUE;
}

bool wxCHMHelpController::DisplayTextPopup(const wxString& text, const wxPoint& pos)
{
    HH_POPUP popup;
    popup.cbStruct = sizeof(popup);
    popup.hinst = (HINSTANCE) wxGetInstance();
    popup.idString = 0 ;
    popup.pt.x = pos.x; popup.pt.y = pos.y;
    popup.clrForeground = (COLORREF)-1;
    popup.clrBackground = (COLORREF)-1;
    popup.rcMargins.top = popup.rcMargins.left = popup.rcMargins.right = popup.rcMargins.bottom = -1;
    popup.pszFont = NULL;
    popup.pszText = (const wxChar*) text;

    gs_htmlHelp(GetSuitableHWND(), NULL, HH_DISPLAY_TEXT_POPUP, (DWORD) & popup);
    return TRUE;
}

bool wxCHMHelpController::DisplayBlock(long block)
{
    return DisplaySection(block);
}

bool wxCHMHelpController::KeywordSearch(const wxString& k)
{
    if (m_helpFile.IsEmpty()) return FALSE;

    wxString str = GetValidFilename(m_helpFile);

    HH_AKLINK link;
    link.cbStruct =     sizeof(HH_AKLINK) ;
    link.fReserved =    FALSE ;
    link.pszKeywords =  k.c_str() ;
    link.pszUrl =       NULL ;
    link.pszMsgText =   NULL ;
    link.pszMsgTitle =  NULL ;
    link.pszWindow =    NULL ;
    link.fIndexOnFail = TRUE ;

    gs_htmlHelp(GetSuitableHWND(), (const wxChar*) str, HH_KEYWORD_LOOKUP, (DWORD)& link);
    return TRUE;
}

bool wxCHMHelpController::Quit()
{
    gs_htmlHelp(GetSuitableHWND(), 0, HH_CLOSE_ALL, 0L);
    UnloadHtmlHelpLibrary();

    return TRUE;
}

// Append extension if necessary.
wxString wxCHMHelpController::GetValidFilename(const wxString& file) const
{
    wxString path, name, ext;
    wxSplitPath(file, & path, & name, & ext);

    wxString fullName;
    if (path.IsEmpty())
        fullName = name + wxT(".chm");
    else if (path.Last() == wxT('\\'))
        fullName = path + name + wxT(".chm");
    else
        fullName = path + wxT("\\") + name + wxT(".chm");
    return fullName;
}

#endif // wxUSE_HELP
