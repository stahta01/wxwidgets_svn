/////////////////////////////////////////////////////////////////////////
// File:        taskbar.cpp
// Purpose:	    Implements wxTaskBarIcon class for manipulating icons on
//              the Windows task bar.
// Author:      Julian Smart
// Modified by:
// Created:     24/3/98
// RCS-ID:      $Id$
// Copyright:   (c)
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "taskbar.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/defs.h"
#endif

#ifdef __WIN95__

#include <wx/msw/taskbar.h>
#include <string.h>
#include <windows.h>
#include <wx/msw/private.h>

#ifdef __GNUWIN32__
#include <wx/msw/gnuwin32/extra.h>
#endif

LRESULT APIENTRY _EXPORT wxTaskBarIconWindowProc( HWND hWnd, unsigned msg,
				     UINT wParam, LONG lParam );

char *wxTaskBarWindowClass = "wxTaskBarWindowClass";

wxList wxTaskBarIcon::sm_taskBarIcons;
bool   wxTaskBarIcon::sm_registeredClass = FALSE;
UINT   wxTaskBarIcon::sm_taskbarMsg = 0;

wxTaskBarIcon::wxTaskBarIcon(void)
{
    m_hWnd = 0;
    m_iconAdded = FALSE;

    AddObject(this);

    if (RegisterWindowClass())
        m_hWnd = CreateTaskBarWindow();
}

wxTaskBarIcon::~wxTaskBarIcon(void)
{
    RemoveObject(this);

    if (m_iconAdded)
    {
        RemoveIcon();
    }

    if (m_hWnd)
    {
        ::DestroyWindow((HWND) m_hWnd);
        m_hWnd = 0;
    }
}

// Operations
bool wxTaskBarIcon::SetIcon(const wxIcon& icon, const wxString& tooltip)
{
    if (!IsOK())
        return FALSE;

    NOTIFYICONDATA notifyData;

    memset(&notifyData, 0, sizeof(notifyData));
	notifyData.cbSize = sizeof(notifyData);
	notifyData.hWnd = (HWND) m_hWnd;
	notifyData.uCallbackMessage = sm_taskbarMsg;
	notifyData.uFlags = NIF_MESSAGE ;
	if (icon.Ok())
    {
		notifyData.uFlags |= NIF_ICON;
	    notifyData.hIcon = (HICON) icon.GetHICON();
    }

    if (((const char*) tooltip != NULL) && (tooltip != ""))
    {
        notifyData.uFlags |= NIF_TIP ;
		lstrcpyn(notifyData.szTip, (char*) (const char*) tooltip, sizeof(notifyData.szTip));
    }

    notifyData.uID = 99;

    if (m_iconAdded)
        return (Shell_NotifyIcon(NIM_MODIFY, & notifyData) != 0);
    else
    {
        m_iconAdded = (Shell_NotifyIcon(NIM_ADD, & notifyData) != 0);
        return m_iconAdded;
    }
}

bool wxTaskBarIcon::RemoveIcon(void)
{
    if (!m_iconAdded)
        return FALSE;

    NOTIFYICONDATA notifyData;

    memset(&notifyData, 0, sizeof(notifyData));
	notifyData.cbSize = sizeof(notifyData);
	notifyData.hWnd = (HWND) m_hWnd;
	notifyData.uCallbackMessage = sm_taskbarMsg;
	notifyData.uFlags = NIF_MESSAGE;
	notifyData.hIcon = 0 ; // hIcon;
    notifyData.uID = 99;
    m_iconAdded = FALSE;

    return (Shell_NotifyIcon(NIM_DELETE, & notifyData) != 0);
}

// Overridables
void wxTaskBarIcon::OnMouseMove(void)
{
}

void wxTaskBarIcon::OnLButtonDown(void)
{
}

void wxTaskBarIcon::OnLButtonUp(void)
{
}

void wxTaskBarIcon::OnRButtonDown(void)
{
}

void wxTaskBarIcon::OnRButtonUp(void)
{
}

void wxTaskBarIcon::OnLButtonDClick(void)
{
}

void wxTaskBarIcon::OnRButtonDClick(void)
{
}

wxTaskBarIcon* wxTaskBarIcon::FindObjectForHWND(WXHWND hWnd)
{
    wxNode*node = sm_taskBarIcons.First();
    while (node)
    {
        wxTaskBarIcon* obj = (wxTaskBarIcon*) node->Data();
        if (obj->GetHWND() == hWnd)
            return obj;
        node = node->Next();
    }
    return NULL;
}

void wxTaskBarIcon::AddObject(wxTaskBarIcon* obj)
{
    sm_taskBarIcons.Append(obj);
}

void wxTaskBarIcon::RemoveObject(wxTaskBarIcon* obj)
{
    sm_taskBarIcons.DeleteObject(obj);
}

bool wxTaskBarIcon::RegisterWindowClass()
{
    if (sm_registeredClass)
        return TRUE;

    // Also register the taskbar message here
    sm_taskbarMsg = ::RegisterWindowMessage("wxTaskBarIconMessage");

    WNDCLASS	wc;
    bool	rc;

    HINSTANCE hInstance = GetModuleHandle(NULL);

    /*
     * set up and register window class
     */
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) wxTaskBarIconWindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = 0;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = wxTaskBarWindowClass ;
    rc = (::RegisterClass( &wc ) != 0);

    sm_registeredClass = (rc != 0);

    return( (rc != 0) );
}

WXHWND wxTaskBarIcon::CreateTaskBarWindow()
{
    HINSTANCE hInstance = GetModuleHandle(NULL);

    HWND hWnd = CreateWindowEx (0, wxTaskBarWindowClass,
            "wxTaskBarWindow",
            WS_OVERLAPPED,
            0,
            0,
            10,
            10,
            NULL,
            (HMENU) 0,
            hInstance,
            NULL);

    return (WXHWND) hWnd;
}

long wxTaskBarIcon::WindowProc( WXHWND hWnd, unsigned int msg, unsigned int wParam, long lParam )
{
    if (msg != sm_taskbarMsg)
        return DefWindowProc((HWND) hWnd, msg, wParam, lParam);

    switch (lParam)
    {
	case WM_LBUTTONDOWN:
		OnLButtonDown();
		break;

	case WM_LBUTTONUP:
		OnLButtonUp();
		break;

	case WM_RBUTTONDOWN:
		OnRButtonDown();
		break;

	case WM_RBUTTONUP:
		OnRButtonUp();
		break;

	case WM_LBUTTONDBLCLK:
		OnLButtonDClick();
		break;

	case WM_RBUTTONDBLCLK:
		OnRButtonDClick();
		break;

	case WM_MOUSEMOVE:
		OnMouseMove();
		break;

	default:
        break;
	}
    return 0;
}

LRESULT APIENTRY _EXPORT wxTaskBarIconWindowProc( HWND hWnd, unsigned msg,
				     UINT wParam, LONG lParam )
{
    wxTaskBarIcon* obj = wxTaskBarIcon::FindObjectForHWND((WXHWND) hWnd);
    if (obj)
        return obj->WindowProc((WXHWND) hWnd, msg, wParam, lParam);
    else
        return DefWindowProc(hWnd, msg, wParam, lParam);
}

#endif
    // __WIN95__
