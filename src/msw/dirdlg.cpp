/////////////////////////////////////////////////////////////////////////////
// Name:        dirdlg.cpp
// Purpose:     wxDirDialog
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------
#ifdef __GNUG__
    #pragma implementation "dirdlg.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/utils.h"
    #include "wx/dialog.h"
    #include "wx/dirdlg.h"
    #include "wx/log.h"
#endif

#include "wx/msw/private.h"

#if defined(__WIN95__) && \
    (!defined(__GNUWIN32__) || defined(wxUSE_NORLANDER_HEADERS))
    #define CAN_COMPILE_DIRDLG
//#else: we provide a stub version which doesn't do anything
#endif

#ifdef CAN_COMPILE_DIRDLG
    #include "shlobj.h" // Win95 shell
#endif

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

#ifndef MAX_PATH
    #define MAX_PATH 4096      // be generuous
#endif

// ----------------------------------------------------------------------------
// wxWindows macros
// ----------------------------------------------------------------------------

#if !USE_SHARED_LIBRARY
    IMPLEMENT_CLASS(wxDirDialog, wxDialog)
#endif

// ----------------------------------------------------------------------------
// private functions prototypes
// ----------------------------------------------------------------------------

// free the parameter
static void ItemListFree(LPITEMIDLIST pidl);

// the callback proc for the dir dlg
static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lp,
                                       LPARAM pData);

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxDirDialog
// ----------------------------------------------------------------------------

wxDirDialog::wxDirDialog(wxWindow *parent,
                         const wxString& message,
                         const wxString& defaultPath,
                         long WXUNUSED(style),
                         const wxPoint& WXUNUSED(pos))
{
    m_message = message;
    m_parent = parent;
    m_path = defaultPath;
}

int wxDirDialog::ShowModal()
{
#ifdef CAN_COMPILE_DIRDLG
    BROWSEINFO bi;
    bi.hwndOwner      = m_parent ? GetHwndOf(m_parent) : NULL;
    bi.pidlRoot       = NULL;
    bi.pszDisplayName = NULL;
    bi.lpszTitle      = m_message.c_str();
    bi.ulFlags        = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
    bi.lpfn           = BrowseCallbackProc;
    bi.lParam         = (LPARAM)m_path.c_str();    // param for the callback

    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

    if ( bi.pidlRoot )
    {
        ItemListFree((LPITEMIDLIST)bi.pidlRoot);
    }

    if ( !pidl )
    {
        // Cancel button pressed
        return wxID_CANCEL;
    }

    BOOL ok = SHGetPathFromIDList(pidl, m_path.GetWriteBuf(MAX_PATH));
    m_path.UngetWriteBuf();

    ItemListFree(pidl);

    if ( !ok )
    {
        wxLogLastError("SHGetPathFromIDList");

        return wxID_CANCEL;
    }

    return wxID_OK;
#else // !CAN_COMPILE_DIRDLG
    return wxID_CANCEL;
#endif // CAN_COMPILE_DIRDLG/!CAN_COMPILE_DIRDLG
}

// ----------------------------------------------------------------------------
// private functions
// ----------------------------------------------------------------------------

static int CALLBACK
BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lp, LPARAM pData)
{
    switch(uMsg)
    {
        case BFFM_INITIALIZED:
            // sent immediately after initialisation and so we may set the
            // initial selection here
            //
            // wParam = TRUE => lParam is a string and not a PIDL
            SendMessage(hwnd, BFFM_SETSELECTION, TRUE, pData);
            break;

        case BFFM_SELCHANGED:
            {
                // Set the status window to the currently selected path.
                TCHAR szDir[MAX_PATH];
                if ( SHGetPathFromIDList((LPITEMIDLIST)lp, szDir) )
                {
                    SendMessage(hwnd, BFFM_SETSTATUSTEXT, 0, (LPARAM)szDir);
                }
            }
            break;

        //case BFFM_VALIDATEFAILED: -- might be used to provide custom message
        //                             if the user types in invalid dir name
    }

    return 0;
}


static void ItemListFree(LPITEMIDLIST pidl)
{
    if ( pidl )
    {
        LPMALLOC pMalloc;
        SHGetMalloc(&pMalloc);
        if ( pMalloc )
        {
            pMalloc->Free(pidl);
            pMalloc->Release();
        }
        else
        {
            wxLogLastError("SHGetMalloc");
        }
    }
}

