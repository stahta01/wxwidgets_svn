/////////////////////////////////////////////////////////////////////////////
// Name:        src/msw/dialog.cpp
// Purpose:     wxDialog class
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "dialog.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/dialog.h"
    #include "wx/utils.h"
    #include "wx/frame.h"
    #include "wx/app.h"
    #include "wx/settings.h"
    #include "wx/intl.h"
    #include "wx/log.h"
#endif

#include "wx/msw/private.h"
#include "wx/log.h"
#include "wx/evtloop.h"
#include "wx/ptr_scpd.h"

#if wxUSE_COMMON_DIALOGS && !defined(__WXMICROWIN__)
    #include <commdlg.h>
#endif

// ----------------------------------------------------------------------------
// wxWin macros
// ----------------------------------------------------------------------------

#if wxUSE_EXTENDED_RTTI
WX_DEFINE_FLAGS( wxDialogStyle )

wxBEGIN_FLAGS( wxDialogStyle )
    // new style border flags, we put them first to
    // use them for streaming out
    wxFLAGS_MEMBER(wxBORDER_SIMPLE)
    wxFLAGS_MEMBER(wxBORDER_SUNKEN)
    wxFLAGS_MEMBER(wxBORDER_DOUBLE)
    wxFLAGS_MEMBER(wxBORDER_RAISED)
    wxFLAGS_MEMBER(wxBORDER_STATIC)
    wxFLAGS_MEMBER(wxBORDER_NONE)

    // old style border flags
    wxFLAGS_MEMBER(wxSIMPLE_BORDER)
    wxFLAGS_MEMBER(wxSUNKEN_BORDER)
    wxFLAGS_MEMBER(wxDOUBLE_BORDER)
    wxFLAGS_MEMBER(wxRAISED_BORDER)
    wxFLAGS_MEMBER(wxSTATIC_BORDER)
    wxFLAGS_MEMBER(wxNO_BORDER)

    // standard window styles
    wxFLAGS_MEMBER(wxTAB_TRAVERSAL)
    wxFLAGS_MEMBER(wxCLIP_CHILDREN)

    // dialog styles
    wxFLAGS_MEMBER(wxWS_EX_VALIDATE_RECURSIVELY)
    wxFLAGS_MEMBER(wxSTAY_ON_TOP)
    wxFLAGS_MEMBER(wxCAPTION)
    wxFLAGS_MEMBER(wxTHICK_FRAME)
    wxFLAGS_MEMBER(wxSYSTEM_MENU)
    wxFLAGS_MEMBER(wxRESIZE_BORDER)
    wxFLAGS_MEMBER(wxRESIZE_BOX)
    wxFLAGS_MEMBER(wxCLOSE_BOX)
    wxFLAGS_MEMBER(wxMAXIMIZE_BOX)
    wxFLAGS_MEMBER(wxMINIMIZE_BOX)
wxEND_FLAGS( wxDialogStyle )

IMPLEMENT_DYNAMIC_CLASS_XTI(wxDialog, wxTopLevelWindow,"wx/dialog.h")

wxBEGIN_PROPERTIES_TABLE(wxDialog)
    wxPROPERTY( Title, wxString, SetTitle, GetTitle, wxString() , 0 /*flags*/ , wxT("Helpstring") , wxT("group"))
    wxPROPERTY_FLAGS( WindowStyle , wxDialogStyle , long , SetWindowStyleFlag , GetWindowStyleFlag , EMPTY_MACROVALUE , 0 /*flags*/ , wxT("Helpstring") , wxT("group")) // style
wxEND_PROPERTIES_TABLE()

wxBEGIN_HANDLERS_TABLE(wxDialog)
wxEND_HANDLERS_TABLE()

wxCONSTRUCTOR_6( wxDialog , wxWindow* , Parent , wxWindowID , Id , wxString , Title , wxPoint , Position , wxSize , Size , long , WindowStyle)

#else
IMPLEMENT_DYNAMIC_CLASS(wxDialog, wxTopLevelWindow)
#endif

BEGIN_EVENT_TABLE(wxDialog, wxDialogBase)
    EVT_BUTTON(wxID_OK, wxDialog::OnOK)
    EVT_BUTTON(wxID_APPLY, wxDialog::OnApply)
    EVT_BUTTON(wxID_CANCEL, wxDialog::OnCancel)

    EVT_SYS_COLOUR_CHANGED(wxDialog::OnSysColourChanged)

    EVT_CLOSE(wxDialog::OnCloseWindow)
END_EVENT_TABLE()

// ----------------------------------------------------------------------------
// wxDialogModalData
// ----------------------------------------------------------------------------

// this is simply a container for any data we need to implement modality which
// allows us to avoid changing wxDialog each time the implementation changes
class wxDialogModalData
{
public:
    wxDialogModalData(wxDialog *dialog) : m_evtLoop(dialog) { }

    void RunLoop()
    {
        m_evtLoop.Run();
    }

    void ExitLoop()
    {
        m_evtLoop.Exit();
    }

private:
    wxModalEventLoop m_evtLoop;
};

wxDEFINE_TIED_SCOPED_PTR_TYPE(wxDialogModalData);

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxDialog construction
// ----------------------------------------------------------------------------

void wxDialog::Init()
{
    m_oldFocus = (wxWindow *)NULL;
    m_isShown = false;
    m_modalData = NULL;
    m_endModalCalled = false;
}

bool wxDialog::Create(wxWindow *parent,
                      wxWindowID id,
                      const wxString& title,
                      const wxPoint& pos,
                      const wxSize& size,
                      long style,
                      const wxString& name)
{
    SetExtraStyle(GetExtraStyle() | wxTOPLEVEL_EX_DIALOG);

    // save focus before doing anything which can potentially change it
    m_oldFocus = FindFocus();

    // All dialogs should really have this style
    style |= wxTAB_TRAVERSAL;

    if ( !wxTopLevelWindow::Create(parent, id, title, pos, size, style, name) )
        return false;

    if ( !m_hasFont )
        SetFont(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT));

    SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE));

    return true;
}

// deprecated ctor
wxDialog::wxDialog(wxWindow *parent,
                   const wxString& title,
                   bool WXUNUSED(modal),
                   int x,
                   int y,
                   int w,
                   int h,
                   long style,
                   const wxString& name)
{
    Init();

    Create(parent, -1, title, wxPoint(x, y), wxSize(w, h), style, name);
}

void wxDialog::SetModal(bool WXUNUSED(flag))
{
    // nothing to do, obsolete method
}

wxDialog::~wxDialog()
{
    m_isBeingDeleted = true;

    // this will also reenable all the other windows for a modal dialog
    Show(false);
}

// ----------------------------------------------------------------------------
// showing the dialogs
// ----------------------------------------------------------------------------

bool wxDialog::IsModalShowing() const
{
    return IsModal();
}

wxWindow *wxDialog::FindSuitableParent() const
{
    // first try to use the currently active window
    HWND hwndFg = ::GetForegroundWindow();
    wxWindow *parent = hwndFg ? wxFindWinFromHandle((WXHWND)hwndFg)
                              : NULL;
    if ( !parent )
    {
        // next try the main app window
        parent = wxTheApp->GetTopWindow();
    }

    // finally, check if the parent we found is really suitable
    if ( !parent || parent == (wxWindow *)this || !parent->IsShown() )
    {
        // don't use this one
        parent = NULL;
    }

    return parent;
}

bool wxDialog::Show(bool show)
{
    if ( show == IsShown() )
        return false;

    if ( !show && m_modalData )
    {
        // we need to do this before calling wxDialogBase version because if we
        // had disabled other app windows, they must be reenabled right now as
        // if they stay disabled Windows will activate another window (one
        // which is enabled, anyhow) when we're hidden in the base class Show()
        // and we will lose activation
        m_modalData->ExitLoop();
    }

    if ( show )
    {
        // this usually will result in TransferDataToWindow() being called
        // which will change the controls values so do it before showing as
        // otherwise we could have some flicker
        InitDialog();
    }

    wxDialogBase::Show(show);

    if ( show )
    {
        // dialogs don't get WM_SIZE message after creation unlike most (all?)
        // other windows and so could start their life non laid out correctly
        // if we didn't call Layout() from here
        //
        // NB: normally we should call it just the first time but doing it
        //     every time is simpler than keeping a flag
        Layout();
    }

    return true;
}

void wxDialog::Raise()
{
    ::SetForegroundWindow(GetHwnd());
}

// show dialog modally
int wxDialog::ShowModal()
{
    wxASSERT_MSG( !IsModal(), _T("wxDialog::ShowModal() reentered?") );

    m_endModalCalled = false;

    Show();

    // EndModal may have been called from InitDialog handler (called from
    // inside Show()), which would cause an infinite loop if we didn't take it
    // into account
    if ( !m_endModalCalled )
    {
        // modal dialog needs a parent window, so try to find one
        wxWindow *parent = GetParent();
        if ( !parent )
        {
            parent = FindSuitableParent();
        }

        // remember where the focus was
        wxWindow *oldFocus = m_oldFocus;
        if ( !oldFocus )
        {
            // VZ: do we really want to do this?
            oldFocus = parent;
        }

        // We have to remember the HWND because we need to check
        // the HWND still exists (oldFocus can be garbage when the dialog
        // exits, if it has been destroyed)
        HWND hwndOldFocus = oldFocus ? GetHwndOf(oldFocus) : NULL;


        // enter and run the modal loop
        {
            wxDialogModalDataTiedPtr modalData(&m_modalData,
                                               new wxDialogModalData(this));
            modalData->RunLoop();
        }


        // and restore focus
        // Note that this code MUST NOT access the dialog object's data
        // in case the object has been deleted (which will be the case
        // for a modal dialog that has been destroyed before calling EndModal).
        if ( oldFocus && (oldFocus != this) && ::IsWindow(hwndOldFocus))
        {
            // This is likely to prove that the object still exists
            if (wxFindWinFromHandle((WXHWND) hwndOldFocus) == oldFocus)
                oldFocus->SetFocus();
        }
    }

    return GetReturnCode();
}

void wxDialog::EndModal(int retCode)
{
    wxASSERT_MSG( IsModal(), _T("EndModal() called for non modal dialog") );

    m_endModalCalled = true;
    SetReturnCode(retCode);

    Show(false);
}

// ----------------------------------------------------------------------------
// wxWin event handlers
// ----------------------------------------------------------------------------

// Standard buttons
void wxDialog::OnOK(wxCommandEvent& WXUNUSED(event))
{
  if ( Validate() && TransferDataFromWindow() )
  {
      EndModal(wxID_OK);
  }
}

void wxDialog::OnApply(wxCommandEvent& WXUNUSED(event))
{
    if ( Validate() )
        TransferDataFromWindow();

    // TODO probably need to disable the Apply button until things change again
}

void wxDialog::OnCancel(wxCommandEvent& WXUNUSED(event))
{
    EndModal(wxID_CANCEL);
}

void wxDialog::OnCloseWindow(wxCloseEvent& WXUNUSED(event))
{
    // We'll send a Cancel message by default, which may close the dialog.
    // Check for looping if the Cancel event handler calls Close().

    // Note that if a cancel button and handler aren't present in the dialog,
    // nothing will happen when you close the dialog via the window manager, or
    // via Close(). We wouldn't want to destroy the dialog by default, since
    // the dialog may have been created on the stack. However, this does mean
    // that calling dialog->Close() won't delete the dialog unless the handler
    // for wxID_CANCEL does so. So use Destroy() if you want to be sure to
    // destroy the dialog. The default OnCancel (above) simply ends a modal
    // dialog, and hides a modeless dialog.

    // VZ: this is horrible and MT-unsafe. Can't we reuse some of these global
    //     lists here? don't dare to change it now, but should be done later!
    static wxList closing;

    if ( closing.Member(this) )
        return;

    closing.Append(this);

    wxCommandEvent cancelEvent(wxEVT_COMMAND_BUTTON_CLICKED, wxID_CANCEL);
    cancelEvent.SetEventObject( this );
    GetEventHandler()->ProcessEvent(cancelEvent); // This may close the dialog

    closing.DeleteObject(this);
}

void wxDialog::OnSysColourChanged(wxSysColourChangedEvent& WXUNUSED(event))
{
#if wxUSE_CTL3D
    Ctl3dColorChange();
#else
    SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE));
    Refresh();
#endif
}

// ---------------------------------------------------------------------------
// dialog window proc
// ---------------------------------------------------------------------------

WXLRESULT wxDialog::MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam)
{
    WXLRESULT rc = 0;
    bool processed = false;

    switch ( message )
    {
#ifdef __WXWINCE__
        // react to pressing the OK button in the title
        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK)
            {
                wxButton *btn = wxDynamicCast(FindWindow(wxID_CANCEL), wxButton);
                if ( btn && btn->IsEnabled() )
                {
                    // if we do have a cancel button, do press it
                    btn->MSWCommand(BN_CLICKED, 0 /* unused */);
                    processed = true;
                    break;
                }
            }
            break;
#endif
        case WM_CLOSE:
            // if we can't close, tell the system that we processed the
            // message - otherwise it would close us
            processed = !Close();
            break;

        case WM_SIZE:
            // the Windows dialogs unfortunately are not meant to be resizeable
            // at all and their standard class doesn't include CS_[VH]REDRAW
            // styles which means that the window is not refreshed properly
            // after the resize and no amount of WS_CLIPCHILDREN/SIBLINGS can
            // help with it - so we have to refresh it manually which certainly
            // creates flicker but at least doesn't show garbage on the screen
            rc = wxWindow::MSWWindowProc(message, wParam, lParam);
            processed = true;
            if ( HasFlag(wxFULL_REPAINT_ON_RESIZE) )
            {
                ::InvalidateRect(GetHwnd(), NULL, false /* erase bg */);
            }
            break;

#ifndef __WXMICROWIN__
        case WM_SETCURSOR:
            // we want to override the busy cursor for modal dialogs:
            // typically, wxBeginBusyCursor() is called and then a modal dialog
            // is shown, but the modal dialog shouldn't have hourglass cursor
            if ( IsModal() && wxIsBusy() )
            {
                // set our cursor for all windows (but see below)
                wxCursor cursor = m_cursor;
                if ( !cursor.Ok() )
                    cursor = wxCURSOR_ARROW;

                ::SetCursor(GetHcursorOf(cursor));

                // in any case, stop here and don't let wxWindow process this
                // message (it would set the busy cursor)
                processed = true;

                // but return false to tell the child window (if the event
                // comes from one of them and not from ourselves) that it can
                // set its own cursor if it has one: thus, standard controls
                // (e.g. text ctrl) still have correct cursors in a dialog
                // invoked while wxIsBusy()
                rc = false;
            }
            break;
#endif // __WXMICROWIN__
    }

    if ( !processed )
        rc = wxWindow::MSWWindowProc(message, wParam, lParam);

    return rc;
}

#if wxUSE_CTL3D

// Define for each class of dialog and control
WXHBRUSH wxDialog::OnCtlColor(WXHDC WXUNUSED(pDC),
                              WXHWND WXUNUSED(pWnd),
                              WXUINT WXUNUSED(nCtlColor),
                              WXUINT message,
                              WXWPARAM wParam,
                              WXLPARAM lParam)
{
    return (WXHBRUSH)Ctl3dCtlColorEx(message, wParam, lParam);
}

#endif // wxUSE_CTL3D

