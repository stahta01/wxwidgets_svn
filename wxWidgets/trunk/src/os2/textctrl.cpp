/////////////////////////////////////////////////////////////////////////////
// Name:        textctrl.cpp
// Purpose:     wxTextCtrl
// Author:      David Webster
// Modified by:
// Created:     10/17/99
// RCS-ID:      $Id$
// Copyright:   (c) David Webster
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
    #include "wx/textctrl.h"
    #include "wx/settings.h"
    #include "wx/brush.h"
    #include "wx/utils.h"
    #include "wx/log.h"
#endif

#if wxUSE_CLIPBOARD
    #include "wx/app.h"
    #include "wx/clipbrd.h"
#endif

#include "wx/textfile.h"

#include "wx/os2/private.h"

#include <string.h>
#include <stdlib.h>
#include <sys/types.h>

#if wxUSE_IOSTREAMH
#   include <fstream.h>
#else
#   include <fstream>
#endif


// ----------------------------------------------------------------------------
// event tables and other macros
// ----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxTextCtrl, wxControl)

BEGIN_EVENT_TABLE(wxTextCtrl, wxControl)
    EVT_CHAR(wxTextCtrl::OnChar)
    EVT_DROP_FILES(wxTextCtrl::OnDropFiles)

    EVT_MENU(wxID_CUT, wxTextCtrl::OnCut)
    EVT_MENU(wxID_COPY, wxTextCtrl::OnCopy)
    EVT_MENU(wxID_PASTE, wxTextCtrl::OnPaste)
    EVT_MENU(wxID_UNDO, wxTextCtrl::OnUndo)
    EVT_MENU(wxID_REDO, wxTextCtrl::OnRedo)

    EVT_UPDATE_UI(wxID_CUT, wxTextCtrl::OnUpdateCut)
    EVT_UPDATE_UI(wxID_COPY, wxTextCtrl::OnUpdateCopy)
    EVT_UPDATE_UI(wxID_PASTE, wxTextCtrl::OnUpdatePaste)
    EVT_UPDATE_UI(wxID_UNDO, wxTextCtrl::OnUpdateUndo)
    EVT_UPDATE_UI(wxID_REDO, wxTextCtrl::OnUpdateRedo)
END_EVENT_TABLE()


// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// creation
// ----------------------------------------------------------------------------

wxTextCtrl::wxTextCtrl()
{
}

bool wxTextCtrl::Create(
  wxWindow*                         pParent
, wxWindowID                        vId
, const wxString&                   rsValue
, const wxPoint&                    rPos
, const wxSize&                     rSize
, long                              lStyle
#if wxUSE_VALIDATORS
, const wxValidator&                rValidator
#endif
, const wxString&                   rsName
)
{
    //
    // Base initialization
    //
    if ( !CreateBase( pParent
                     ,vId
                     ,rPos
                     ,rSize
                     ,lStyle
#if wxUSE_VALIDATORS
                     ,rValidator
#endif
                     ,rsName
                    ))
        return FALSE;

    if (pParent )
        pParent->AddChild(this);

    m_windowStyle = lStyle;

    long                            lSstyle = WS_VISIBLE | WS_TABSTOP;

    //
    // Single and multiline edit fields are two different controls in PM
    //
    if ( m_windowStyle & wxTE_MULTILINE )
    {
        m_bIsMLE = TRUE;
        m_windowStyle |= wxTE_PROCESS_ENTER;

        if ((m_windowStyle & wxTE_NO_VSCROLL) == 0)
            lSstyle |= MLS_VSCROLL;
        if (m_windowStyle & wxHSCROLL)
            lSstyle |= MLS_HSCROLL;
        if (m_windowStyle & wxTE_READONLY)
            lSstyle |= MLS_READONLY;
    }
    else
    {
        lSstyle |= ES_LEFT;

        if (m_windowStyle & wxHSCROLL)
            lSstyle |=  ES_AUTOSCROLL;
        if (m_windowStyle & wxTE_READONLY)
            lSstyle |= ES_READONLY;
        if (m_windowStyle & wxTE_PASSWORD) // hidden input
            lSstyle |= ES_UNREADABLE;
    }
    if (m_bIsMLE)
    {
        m_hWnd = (WXHWND)::WinCreateWindow( (HWND)GetHwndOf(pParent) // Parent window handle
                                           ,WC_MLE                   // Window class
                                           ,(PSZ)rsValue.c_str()     // Initial Text
                                           ,(ULONG)lSstyle           // Style flags
                                           ,(LONG)rPos.x             // X pos of origin
                                           ,(LONG)rPos.y             // Y pos of origin
                                           ,(LONG)rSize.x            // field width
                                           ,(LONG)rSize.y            // field height
                                           ,(HWND)GetHwndOf(pParent) // owner window handle (same as parent
                                           ,HWND_TOP                 // initial z position
                                           ,(ULONG)vId               // Window identifier
                                           ,NULL                     // no control data
                                           ,NULL                     // no Presentation parameters
                                          );
    }
    else
    {
        m_hWnd = (WXHWND)::WinCreateWindow( (HWND)GetHwndOf(pParent) // Parent window handle
                                           ,WC_ENTRYFIELD            // Window class
                                           ,(PSZ)rsValue.c_str()     // Initial Text
                                           ,(ULONG)lSstyle           // Style flags
                                           ,(LONG)rPos.x             // X pos of origin
                                           ,(LONG)rPos.y             // Y pos of origin
                                           ,(LONG)rSize.x            // field width
                                           ,(LONG)rSize.y            // field height
                                           ,(HWND)GetHwndOf(pParent) // owner window handle (same as parent
                                           ,HWND_TOP                 // initial z position
                                           ,(ULONG)vId               // Window identifier
                                           ,NULL                     // no control data
                                           ,NULL                     // no Presentation parameters
                                          );
    }

    if (m_hWnd == 0)
    {
        return FALSE;
    }

    SubclassWin(GetHWND());

    //
    // Set font, position, size and initial value
    //
    wxFont&                         vFontParent = pParent->GetFont();

    if (vFontParent.Ok())
    {
        SetFont(vFontParent);
    }
    else
    {
        SetFont(wxSystemSettings::GetSystemFont(wxSYS_SYSTEM_FONT));
    }
    if (!rsValue.IsEmpty())
    {
        SetValue(rsValue);
    }
    SetupColours();
    SetSize( rPos.x
            ,rPos.y
            ,rSize.x
            ,rSize.y
           );
    return TRUE;
} // end of wxTextCtrl::Create

//
// Make sure the window style (etc.) reflects the HWND style (roughly)
//
void wxTextCtrl::AdoptAttributesFromHWND()
{
    HWND                            hWnd = GetHwnd();
    LONG                            lStyle = ::WinQueryWindowULong(hWnd, QWL_STYLE);

    wxWindow::AdoptAttributesFromHWND();

    if (m_bIsMLE)
    {
        m_windowStyle |= wxTE_MULTILINE;
        if (lStyle & MLS_READONLY)
            m_windowStyle |= wxTE_READONLY;
    }
    else
    {
        if (lStyle & ES_UNREADABLE)
            m_windowStyle |= wxTE_PASSWORD;
        if (lStyle & ES_READONLY)
            m_windowStyle |= wxTE_READONLY;
    }
} // end of wxTextCtrl::AdoptAttributesFromHWND

void wxTextCtrl::SetupColours()
{
    wxColour                        vBkgndColour;

    vBkgndColour = wxSystemSettings::GetSystemColour(wxSYS_COLOUR_WINDOW);
    SetBackgroundColour(vBkgndColour);
    SetForegroundColour(GetParent()->GetForegroundColour());
} // end of wxTextCtrl::SetupColours

// ----------------------------------------------------------------------------
// set/get the controls text
// ----------------------------------------------------------------------------

wxString wxTextCtrl::GetValue() const
{
    wxString                        sStr = wxGetWindowText(GetHWND());
    char*                           zStr = (char*)sStr.c_str();

    for ( ; *zStr; zStr++ )
    {
        //
        // this will replace \r\n with just \n
        //
        if (*zStr == '\n')
            *zStr = '\0';
        if (*zStr == '\r')
            *zStr = '\n';
    }
    sStr = zStr;
    return sStr;
} // end of wxTextCtrl::GetValue

void wxTextCtrl::SetValue(
  const wxString&                   rsValue
)
{
    //
    // If the text is long enough, it's faster to just set it instead of first
    // comparing it with the old one (chances are that it will be different
    // anyhow, this comparison is there to avoid flicker for small single-line
    // edit controls mostly)
    //
    if ((rsValue.length() > 0x400) || (rsValue != GetValue()))
    {
        ::WinSetWindowText(GetHwnd(), rsValue.c_str());
        AdjustSpaceLimit();
    }
} // end of wxTextCtrl::SetValue

void wxTextCtrl::WriteText(
  const wxString&                   rsValue
)
{
    ::WinSetWindowText(GetHwnd(), rsValue.c_str());
    AdjustSpaceLimit();
} // end of wxTextCtrl::WriteText

void wxTextCtrl::AppendText(
  const wxString&                   rsText
)
{
    SetInsertionPointEnd();
    WriteText(rsText);
} // end of wxTextCtrl::AppendText

void wxTextCtrl::Clear()
{
    ::WinSetWindowText(GetHwnd(), "");
} // end of wxTextCtrl::Clear

// ----------------------------------------------------------------------------
// Clipboard operations
// ----------------------------------------------------------------------------

void wxTextCtrl::Copy()
{
    if (CanCopy())
    {
        HWND hWnd = GetHwnd();
        if (m_bIsMLE)
            ::WinSendMsg(hWnd, MLM_COPY, 0, 0);
        else
            ::WinSendMsg(hWnd, EM_COPY, 0, 0);
    }
} // end of wxTextCtrl::Copy

void wxTextCtrl::Cut()
{
    if (CanCut())
    {
        HWND hWnd = GetHwnd();

        if (m_bIsMLE)
            ::WinSendMsg(hWnd, MLM_CUT, 0, 0);
        else
            ::WinSendMsg(hWnd, EM_CUT, 0, 0);
    }
} // end of wxTextCtrl::Cut

void wxTextCtrl::Paste()
{
    if (CanPaste())
    {
        HWND                        hWnd = GetHwnd();

        ::WinSendMsg(hWnd, EM_PASTE, 0, 0);
    }
} // end of wxTextCtrl::Paste

bool wxTextCtrl::CanCopy() const
{
    //
    // Can copy if there's a selection
    //
    long                            lFrom = 0L;
    long                            lTo = 0L;

    GetSelection(&lFrom, &lTo);
    return (lFrom != lTo);
} // end of wxTextCtrl::CanCopy

bool wxTextCtrl::CanCut() const
{
    //
    // Can cut if there's a selection
    //
    long                            lFrom = 0L;
    long                            lTo = 0L;

    GetSelection(&lFrom, &lTo);
    return (lFrom != lTo);
} // end of wxTextCtrl::CanCut

bool wxTextCtrl::CanPaste() const
{
    bool                            bIsTextAvailable = FALSE;

    if (!IsEditable())
        return FALSE;

    //
    // Check for straight text on clipboard
    //
    if (::WinOpenClipbrd(vHabmain))
    {
        bIsTextAvailable = (::WinQueryClipbrdData(vHabmain, CF_TEXT) != 0);
        ::WinCloseClipbrd(vHabmain);
    }
    return bIsTextAvailable;
} // end of wxTextCtrl::CanPaste

// ----------------------------------------------------------------------------
// Accessors
// ----------------------------------------------------------------------------

void wxTextCtrl::SetEditable(
  bool                              bEditable
)
{
    HWND                            hWnd = GetHwnd();

    if (m_bIsMLE)
        ::WinSendMsg(hWnd, MLM_SETREADONLY, MPFROMLONG(!bEditable), (MPARAM)0);
    else
        ::WinSendMsg(hWnd, EM_SETREADONLY, MPFROMLONG(!bEditable), (MPARAM)0);
} // end of wxTextCtrl::SetEditable

void wxTextCtrl::SetInsertionPoint(
  long                              lPos
)
{
    HWND                            hWnd = GetHwnd();

    if (m_bIsMLE)
        ::WinSendMsg(hWnd, MLM_SETSEL, (MPARAM)lPos, (MPARAM)lPos);
    else
        ::WinSendMsg(hWnd, EM_SETSEL, MPFROM2SHORT((USHORT)lPos, (USHORT)lPos), (MPARAM)0);
} // end of wxTextCtrl::SetInsertionPoint

void wxTextCtrl::SetInsertionPointEnd()
{
    long                            lPos = GetLastPosition();

    SetInsertionPoint(lPos);
} // end of wxTextCtrl::SetInsertionPointEnd

long wxTextCtrl::GetInsertionPoint() const
{
    WXDWORD                         dwPos = 0L;

    if (m_bIsMLE)
        dwPos = (WXDWORD)::WinSendMsg(GetHwnd(), MLM_QUERYSEL, (MPARAM)MLFQS_MINSEL, 0);
    else
    {
        dwPos = (WXDWORD)::WinSendMsg(GetHwnd(), EM_QUERYSEL, 0, 0);
        dwPos = SHORT1FROMMP((MPARAM)dwPos);  // the first 16 bit value is the min pos
    }
    return (dwPos & 0xFFFF);
} // end of wxTextCtrl::GetInsertionPoint

long wxTextCtrl::GetLastPosition() const
{
    HWND                            hWnd = GetHwnd();
    long                            lCharIndex;
    long                            lLineLength;

    if (m_bIsMLE)
    {
        lCharIndex = 0;

        //
        // This just gets the total text length.  The last will be this value
        //
        lLineLength = (long)::WinSendMsg(hWnd, MLM_QUERYTEXTLENGTH, 0, 0);
    }
    else
    {
        WNDPARAMS                   vParams;

        lCharIndex = 0;
        vParams.fsStatus = WPM_CCHTEXT;
        if (::WinSendMsg( GetHwnd()
                         ,WM_QUERYWINDOWPARAMS
                         ,&vParams
                         ,0
                        ))
        {
            lLineLength = (long)vParams.cchText;
        }
        else
            lLineLength = 0;
    }
    return(lCharIndex + lLineLength);
} // end of wxTextCtrl::GetLastPosition

// If the return values from and to are the same, there is no
// selection.
void wxTextCtrl::GetSelection(
  long*                             plFrom
, long*                             plTo
) const
{
    WXDWORD                         dwPos;

    if (m_bIsMLE)
        dwPos = (WXDWORD)::WinSendMsg(GetHwnd(), MLM_QUERYSEL, (MPARAM)MLFQS_MINSEL, 0);
    else
    {
        dwPos = (WXDWORD)::WinSendMsg(GetHwnd(), EM_QUERYSEL, 0, 0);
    }
    *plFrom = SHORT1FROMMP((MPARAM)dwPos);  // the first 16 bit value is the min pos
    *plTo = SHORT2FROMMP((MPARAM)dwPos);  // the first 16 bit value is the min pos
} // end of wxTextCtrl::GetSelection

bool wxTextCtrl::IsEditable() const
{
    if (m_bIsMLE)
        return((bool)LONGFROMMR(::WinSendMsg(GetHwnd(), MLM_QUERYREADONLY, 0, 0)));
    else
        return((bool)LONGFROMMR(::WinSendMsg(GetHwnd(), EM_QUERYREADONLY, 0, 0)));
} // end of wxTextCtrl::IsEditable

// ----------------------------------------------------------------------------
// Editing
// ----------------------------------------------------------------------------

void wxTextCtrl::Replace(
  long                              lFrom
, long                              lTo
, const wxString&                   rsValue
)
{
#if wxUSE_CLIPBOARD
    HWND                            hWnd      = GetHwnd();
    long                            lFromChar = lFrom;
    long                            lToChar   = lTo;

    //
    // Set selection and remove it
    //
    if (m_bIsMLE)
    {
        ::WinSendMsg(hWnd, MLM_SETSEL, MPFROM2SHORT((USHORT)lFrom, (USHORT)lTo), 0);
        ::WinSendMsg(hWnd, MLM_CUT, 0, 0);
    }
    else
    {
        ::WinSendMsg(hWnd, EM_SETSEL, MPFROM2SHORT((USHORT)lFrom, (USHORT)lTo), 0);
        ::WinSendMsg(hWnd, EM_CUT, 0, 0);
    }

    //
    // Now replace with 'value', by pasting.
    //
    wxSetClipboardData(wxDF_TEXT, (wxObject *) (const wxChar *)rsValue, 0, 0);

    // Paste into edit control
    if (m_bIsMLE)
        ::WinSendMsg(hWnd, MLM_PASTE, (MPARAM)0, (MPARAM)0);
    else
        ::WinSendMsg(hWnd, EM_PASTE, (MPARAM)0, (MPARAM)0);
#else
    wxFAIL_MSG("wxTextCtrl::Replace not implemented if wxUSE_CLIPBOARD is 0.");
#endif
}  // end of wxTextCtrl::Replace

void wxTextCtrl::Remove(
  long                              lFrom
, long                              lTo
)
{
    HWND                            hWnd      = GetHwnd();
    long                            lFromChar = lFrom;
    long                            lToChar   = lTo;

    if (m_bIsMLE)
    {
        ::WinSendMsg(hWnd, MLM_SETSEL, MPFROM2SHORT((USHORT)lFrom, (USHORT)lTo), 0);
        ::WinSendMsg(hWnd, MLM_CUT, 0, 0);
    }
    else
    {
        ::WinSendMsg(hWnd, EM_SETSEL, MPFROM2SHORT((USHORT)lFrom, (USHORT)lTo), 0);
        ::WinSendMsg(hWnd, EM_CUT, 0, 0);
    }
} // end of wxTextCtrl::Remove

void wxTextCtrl::SetSelection(
  long                              lFrom
, long                              lTo
)
{
    HWND                            hWnd = GetHwnd();
    long                            lFromChar = lFrom;
    long                            lToChar = lTo;

    //
    // If from and to are both -1, it means (in wxWindows) that all text should
    // be selected. Translate into Windows convention
    //
    if ((lFrom == -1L) && (lTo == -1L))
    {
        lFromChar = 0L;
        lToChar   = -1L;
    }
    if (m_bIsMLE)
        ::WinSendMsg(hWnd, MLM_SETSEL, (MPARAM)lFromChar, (MPARAM)lToChar);
    else
        ::WinSendMsg(hWnd, EM_SETSEL, MPFROM2SHORT((USHORT)lFromChar, (USHORT)lToChar), (MPARAM)0);
} // end of wxTextCtrl::SetSelection

bool wxTextCtrl::LoadFile(
  const wxString&                   rsFile
)
{
    if ( wxTextCtrlBase::LoadFile(rsFile) )
    {
        //
        // Update the size limit if needed
        //
        AdjustSpaceLimit();
        return TRUE;
    }
    return FALSE;
} // end of wxTextCtrl::LoadFile

bool wxTextCtrl::IsModified() const
{
    bool                            bRc;

    if (m_bIsMLE)
        bRc = (bool)LONGFROMMR(::WinSendMsg(GetHwnd(), MLM_QUERYCHANGED, 0, 0));
    else
        bRc = (bool)LONGFROMMR(::WinSendMsg(GetHwnd(), EM_QUERYCHANGED, 0, 0));
    return bRc;
} // end of wxTextCtrl::IsModified

//
// Makes 'unmodified'
//
void wxTextCtrl::DiscardEdits()
{
    if (m_bIsMLE)
        ::WinSendMsg(GetHwnd(), MLM_SETCHANGED, MPFROMLONG(FALSE), 0);
    else
        //
        // EM controls do not have a SETCHANGED but issuing a query should reset it
        //
        ::WinSendMsg(GetHwnd(), EM_QUERYCHANGED, 0, 0);
} // end of wxTextCtrl::DiscardEdits

int wxTextCtrl::GetNumberOfLines() const
{
    int                             nNumLines;

    if (m_bIsMLE)
        nNumLines = (int)::WinSendMsg(GetHwnd(), MLM_QUERYLINECOUNT, 0, 0);
    else
        nNumLines = 1;
    return nNumLines;
} // end of wxTextCtrl::GetNumberOfLines

long wxTextCtrl::XYToPosition(
  long                              lX
, long                              lY
) const
{
    HWND                            hWnd = GetHwnd();
    long                            lCharIndex = 0L;
    long                            lLen;

    if (m_bIsMLE)
    {
        lLen = (long)::WinSendMsg(GetHwnd(), MLM_QUERYLINELENGTH, 0, 0);
        lCharIndex = ((lLen * lY) + lX);
    }
    else
        lCharIndex = lX;
    return lCharIndex;
} // end of wxTextCtrl::XYToPosition

bool wxTextCtrl::PositionToXY(
  long                              lPos
, long*                             plX
, long*                             plY
) const
{
    HWND                            hWnd = GetHwnd();
    long                            nLineNo = -1;
    long                            lCharIndex = 0;

    if (m_bIsMLE)
        nLineNo = (long)::WinSendMsg(hWnd, MLM_LINEFROMCHAR, (MPARAM)lPos, 0);
    else
        nLineNo = 0;

    if (nLineNo == -1)
    {
        // no such line
        return FALSE;
    }

    //
    // This gets the char index for the _beginning_ of this line
    //
    long                            lLineWidth;

    if (m_bIsMLE)
    {
        lLineWidth = (long)::WinSendMsg(hWnd, MLM_QUERYLINELENGTH, (MPARAM)0, (MPARAM)0);
        lCharIndex = (nLineNo + 1) * lLineWidth;
    }
    else
    {
        WNDPARAMS                   vParams;

        vParams.fsStatus = WPM_CCHTEXT;
        if (::WinSendMsg( hWnd
                         ,WM_QUERYWINDOWPARAMS
                         ,&vParams
                         ,0
                        ))
        {
            lCharIndex = vParams.cchText;
        }
        else
            lCharIndex = 32;
    }

    if (lCharIndex == -1)
    {
        return FALSE;
    }

    //
    // The X position must therefore be the difference between pos and charIndex
    //
    if (plX)
        *plX = lPos - lCharIndex;
    if (plY)
        *plY = nLineNo;

    return TRUE;
} // end of wxTextCtrl::PositionToXY

void wxTextCtrl::ShowPosition(
  long                              lPos
)
{
    HWND                            hWnd = GetHwnd();
    long                            lCurrentLineLineNo = 0L;

    // To scroll to a position, we pass the number of lines and characters
    // to scroll *by*. This means that we need to:
    // (1) Find the line position of the current line.
    // (2) Find the line position of pos.
    // (3) Scroll by (pos - current).
    // For now, ignore the horizontal scrolling.

    //
    // Is this where scrolling is relative to - the line containing the caret?
    // Or is the first visible line??? Try first visible line.
    //
    if (m_bIsMLE)
    {
        //
        // In PM this is the actual char position
        //
        lCurrentLineLineNo = (long)::WinSendMsg(hWnd, MLM_QUERYFIRSTCHAR, (MPARAM)0, (MPARAM)0);

        //
        // This will cause a scroll to the selected position
        //
        ::WinSendMsg(hWnd, MLM_SETSEL, (MPARAM)lCurrentLineLineNo, (MPARAM)lCurrentLineLineNo);
    }
} // end of wxTextCtrl::ShowPosition

int wxTextCtrl::GetLineLength(
  long                              lLineNo
) const
{
    long                            lLen = 0L;

    if (m_bIsMLE)
        lLen = (long)::WinSendMsg(GetHwnd(), MLM_QUERYLINELENGTH, 0, 0);
    else
    {
        WNDPARAMS                   vParams;

        vParams.fsStatus = WPM_CCHTEXT;
        if (::WinSendMsg( GetHwnd()
                         ,WM_QUERYWINDOWPARAMS
                         ,&vParams
                         ,0
                        ))
        {
            lLen = vParams.cchText;
        }
        else
            lLen = 32;
    }
    return lLen;
} // end ofwxTextCtrl::GetLineLength

wxString wxTextCtrl::GetLineText(
  long                              lLineNo
) const
{
    long                            lLen = (long)GetLineLength((long)lLineNo) + 1;
    wxString                        sStr;
    char*                           zBuf;

    //
    // There must be at least enough place for the length WORD in the
    // buffer
    //
    lLen += sizeof(WORD);
    zBuf = new char[lLen];
    if (m_bIsMLE)
    {
        long                        lIndex;
        long                        lBuflen;
        long                        lCopied;

        lLen = (long)::WinSendMsg(GetHwnd(), MLM_QUERYLINELENGTH, 0, 0);
        lIndex = lLen * lLineNo;

        ::WinSendMsg(GetHwnd(), MLM_SETSEL, (MPARAM)lIndex, (MPARAM)lIndex);
        ::WinSendMsg(GetHwnd(), MLM_SETIMPORTEXPORT, MPFROMP(zBuf), MPFROMSHORT((USHORT)sizeof(zBuf)));
        lBuflen = (long)::WinSendMsg(GetHwnd(), MLM_QUERYFORMATTEXTLENGTH, MPFROMLONG(lIndex), MPFROMLONG(-1));
        lCopied = (long)::WinSendMsg(GetHwnd(), MLM_EXPORT, MPFROMP(&lIndex), MPFROMP(&lBuflen));
        zBuf[lCopied] = '\0';
    }
    else
    {
        WNDPARAMS                   vParams;

        vParams.fsStatus = WPM_CCHTEXT;
        if (::WinSendMsg( GetHwnd()
                         ,WM_QUERYWINDOWPARAMS
                         ,&vParams
                         ,0
                        ))
         memcpy(zBuf, vParams.pszText, vParams.cchText);
         zBuf[vParams.cchText] = '\0';
     }
     sStr = zBuf;
     delete [] zBuf;
     return sStr;
} // end of wxTextCtrl::GetLineText

// ----------------------------------------------------------------------------
// Undo/redo
// ----------------------------------------------------------------------------

void wxTextCtrl::Undo()
{
    if (CanUndo())
    {
        if (m_bIsMLE)
            ::WinSendMsg(GetHwnd(), MLM_UNDO, 0, 0);
        // Simple entryfields cannot be undone
    }
} // end of wxTextCtrl::Undo

void wxTextCtrl::Redo()
{
    if (CanRedo())
    {
        if (m_bIsMLE)
            ::WinSendMsg(GetHwnd(), MLM_UNDO, 0, 0);
        // Simple entryfields cannot be undone
    }
} // end of wxTextCtrl::Redo

bool wxTextCtrl::CanUndo() const
{
    bool                            bOk;

    if (m_bIsMLE)
        bOk = (::WinSendMsg(GetHwnd(), MLM_QUERYUNDO, 0, 0) != 0);
    else
        bOk = FALSE; // can't undo regular edit fields in PM
    return bOk;
} // end of wxTextCtrl::CanUndo

bool wxTextCtrl::CanRedo() const
{
    bool                            bOk;

    if (m_bIsMLE)
        bOk = (::WinSendMsg(GetHwnd(), MLM_QUERYUNDO, 0, 0) != 0);
    else
        bOk = FALSE; // can't undo regular edit fields in PM
    return bOk;
} // end of wxTextCtrl::CanRedo

// ----------------------------------------------------------------------------
// implemenation details
// ----------------------------------------------------------------------------

void wxTextCtrl::Command(
  wxCommandEvent&                   rEvent
)
{
    SetValue(rEvent.GetString());
    ProcessCommand (rEvent);
} // end of wxTextCtrl::Command

void wxTextCtrl::OnDropFiles(
  wxDropFilesEvent&                 rEvent
)
{
    // By default, load the first file into the text window.
    if (rEvent.GetNumberOfFiles() > 0)
    {
        LoadFile(rEvent.GetFiles()[0]);
    }
} // end of wxTextCtrl::OnDropFiles

WXHBRUSH wxTextCtrl::OnCtlColor(
  WXHDC                             hWxDC
, WXHWND                            hWnd
, WXUINT                            uCtlColor
, WXUINT                            uMessage
, WXWPARAM                          wParam
, WXLPARAM                          lParam
)
{
    HPS                             hPS = (HPS)hWxDC;
    wxBrush*                        pBrush = NULL;
    wxColour                        vColBack = GetBackgroundColour();
    wxColour                        vColFore = GetForegroundColour();
    wxBrush*                        pBackgroundBrush = wxTheBrushList->FindOrCreateBrush( GetBackgroundColour()
                                                                                         ,wxSOLID
                                                                                        );

    if (m_bUseCtl3D)
    {
        HBRUSH                      hBrush = NULLHANDLE;

        return hBrush;
    }
    if (GetParent()->GetTransparentBackground())
        ::GpiSetBackMix(hPS, BM_LEAVEALONE);
    else
        ::GpiSetBackMix(hPS, BM_OVERPAINT);
    if (!IsEnabled() && (GetWindowStyle() & wxTE_MULTILINE) == 0)
        vColBack = wxSystemSettings::GetSystemColour(wxSYS_COLOUR_3DFACE);
    ::GpiSetBackColor(hPS, vColBack.GetPixel());
    ::GpiSetColor(hPS, vColFore.GetPixel());
    return (WXHBRUSH)pBackgroundBrush->GetResourceHandle();
} // end of wxTextCtrl::OnCtlColor

void wxTextCtrl::OnChar(
  wxKeyEvent&                       rEvent
)
{
    switch (rEvent.KeyCode())
    {
        case WXK_RETURN:
            if ( !(m_windowStyle & wxTE_MULTILINE) )
            {
                wxCommandEvent      vEvent(wxEVT_COMMAND_TEXT_ENTER, m_windowId);

                vEvent.SetEventObject(this);
                if ( GetEventHandler()->ProcessEvent(vEvent))
                    return;
            }
            //else: multiline controls need Enter for themselves

            break;

        case WXK_TAB:
            // always produce navigation event - even if we process TAB
            // ourselves the fact that we got here means that the user code
            // decided to skip processing of this TAB - probably to let it
            // do its default job.
            //
            // NB: Notice that Ctrl-Tab is handled elsewhere and Alt-Tab is
            //     handled by Windows
            {
                wxNavigationKeyEvent    vEventNav;

                vEventNav.SetDirection(!rEvent.ShiftDown());
                vEventNav.SetWindowChange(FALSE);
                vEventNav.SetEventObject(this);

                if ( GetEventHandler()->ProcessEvent(vEventNav) )
                    return;
            }
            break;
    }
    rEvent.Skip();
} // end of wxTextCtrl::OnChar

bool wxTextCtrl::OS2Command(
  WXUINT                            uParam
, WXWORD                            WXUNUSED(vId)
)
{
    switch (uParam)
    {
        case EN_SETFOCUS:
        case EN_KILLFOCUS:
            {
                wxFocusEvent        vEvent( uParam == EN_KILLFOCUS ? wxEVT_KILL_FOCUS
                                                                   : wxEVT_SET_FOCUS
                                           ,m_windowId
                                          );

                vEvent.SetEventObject(this);
                GetEventHandler()->ProcessEvent(vEvent);
            }
            break;

        case EN_CHANGE:
            {
                wxCommandEvent      vEvent( wxEVT_COMMAND_TEXT_UPDATED
                                           ,m_windowId
                                          );

                InitCommandEvent(vEvent);
                vEvent.SetString((char*)GetValue().c_str());
                ProcessCommand(vEvent);
            }
            break;

        case EN_OVERFLOW:
            //
            // The text size limit has been hit - increase it
            //
            AdjustSpaceLimit();
            break;

        case EN_SCROLL:
        case EN_INSERTMODETOGGLE:
        case EN_MEMERROR:
            return FALSE;
        default:
            return FALSE;
    }

    //
    // Processed
    //
    return TRUE;
} // end of wxTextCtrl::OS2Command

void wxTextCtrl::AdjustSpaceLimit()
{
    unsigned int                    uLen = 0;
    unsigned int                    uLimit = 0;

    uLen   = ::WinQueryWindowTextLength(GetHwnd());
    if (m_bIsMLE)
    {
        uLimit = (unsigned int)::WinSendMsg( GetHwnd()
                                            ,MLM_QUERYTEXTLIMIT
                                            ,0
                                            ,0
                                           );
    }
    else
    {
        ENTRYFDATA*                 pEfd;
        WNDPARAMS                   vParams;

        vParams.fsStatus = WPM_CBCTLDATA;
        vParams.cbCtlData = sizeof(ENTRYFDATA);

        if (::WinSendMsg( GetHwnd()
                         ,WM_QUERYWINDOWPARAMS
                         ,&vParams
                         ,0
                        ))
        {
            pEfd = (ENTRYFDATA*)vParams.pCtlData;
            uLimit = (unsigned int)pEfd->cchEditLimit;
        }
        else
            uLimit = 32; //PM's default
    }
    if (uLen >= uLimit)
    {
        uLimit = uLen + 0x8000;    // 32Kb
        if (uLimit > 0xffff)
        {
            uLimit = 0L;
        }
        if (m_bIsMLE)
            ::WinSendMsg(GetHwnd(), MLM_SETTEXTLIMIT, MPFROMLONG(uLimit), 0);
        else
            ::WinSendMsg(GetHwnd(), EM_SETTEXTLIMIT, MPFROMLONG(uLimit), 0);
    }
} // end of wxTextCtrl::AdjustSpaceLimit

bool wxTextCtrl::AcceptsFocus() const
{
    //
    // We don't want focus if we can't be edited
    //
    return IsEditable() && wxControl::AcceptsFocus();
} // end of wxTextCtrl::Command

wxSize wxTextCtrl::DoGetBestSize() const
{
    int                             nCx;
    int                             nCy;

    wxGetCharSize(GetHWND(), &nCx, &nCy, (wxFont*)&GetFont());

    int                             wText = DEFAULT_ITEM_WIDTH;
    int                             hText = EDIT_HEIGHT_FROM_CHAR_HEIGHT(nCy);

    if (m_windowStyle & wxTE_MULTILINE)
    {
        hText *= wxMin(GetNumberOfLines(), 5);
    }
    //else: for single line control everything is ok
    return wxSize(wText, hText);
} // end of wxTextCtrl::DoGetBestSize

// ----------------------------------------------------------------------------
// standard handlers for standard edit menu events
// ----------------------------------------------------------------------------

void wxTextCtrl::OnCut(
  wxCommandEvent&                   rEvent
)
{
    Cut();
} // end of wxTextCtrl::OnCut

void wxTextCtrl::OnCopy(
  wxCommandEvent&                   rEvent
)
{
    Copy();
} // end of wxTextCtrl::OnCopy

void wxTextCtrl::OnPaste(
  wxCommandEvent&                   rEvent
)
{
    Paste();
} // end of wxTextCtrl::OnPaste

void wxTextCtrl::OnUndo(
  wxCommandEvent&                   rEvent
)
{
    Undo();
} // end of wxTextCtrl::OnUndo

void wxTextCtrl::OnRedo(
  wxCommandEvent&                   rEvent
)
{
    Redo();
} // end of wxTextCtrl::OnRedo

void wxTextCtrl::OnUpdateCut(
  wxUpdateUIEvent&                  rEvent
)
{
    rEvent.Enable(CanCut());
} // end of wxTextCtrl::OnUpdateCut

void wxTextCtrl::OnUpdateCopy(
  wxUpdateUIEvent&                  rEvent
)
{
    rEvent.Enable(CanCopy());
} // end of wxTextCtrl::OnUpdateCopy

void wxTextCtrl::OnUpdatePaste(
  wxUpdateUIEvent&                  rEvent
)
{
    rEvent.Enable(CanPaste());
} // end of wxTextCtrl::OnUpdatePaste

void wxTextCtrl::OnUpdateUndo(
  wxUpdateUIEvent&                  rEvent
)
{
    rEvent.Enable(CanUndo());
} // end of wxTextCtrl::OnUpdateUndo

void wxTextCtrl::OnUpdateRedo(
  wxUpdateUIEvent&                  rEvent
)
{
    rEvent.Enable(CanRedo());
} // end of wxTextCtrl::OnUpdateRedo

bool wxTextCtrl::SetBackgroundColour(
  const wxColour&                   rColour
)
{
    if (m_bIsMLE)
        ::WinSendMsg(GetHwnd(), MLM_SETBACKCOLOR, (MPARAM)rColour.GetPixel(), MLE_INDEX);
    return TRUE;
} // end of wxTextCtrl::SetBackgroundColour

bool wxTextCtrl::SetForegroundColour(
  const wxColour&                   rColour
)
{
    if (m_bIsMLE)
        ::WinSendMsg(GetHwnd(), MLM_SETTEXTCOLOR, (MPARAM)rColour.GetPixel(), MLE_INDEX);
    return TRUE;
} // end of wxTextCtrl::SetForegroundColour

