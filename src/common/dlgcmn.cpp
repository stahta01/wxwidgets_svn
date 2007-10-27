/////////////////////////////////////////////////////////////////////////////
// Name:        src/common/dlgcmn.cpp
// Purpose:     common (to all ports) wxDialog functions
// Author:      Vadim Zeitlin
// Modified by:
// Created:     28.06.99
// RCS-ID:      $Id$
// Copyright:   (c) Vadim Zeitlin
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/dialog.h"

#ifndef WX_PRECOMP
    #include "wx/app.h"
    #include "wx/button.h"
    #include "wx/dcclient.h"
    #include "wx/intl.h"
    #include "wx/settings.h"
    #include "wx/stattext.h"
    #include "wx/sizer.h"
    #include "wx/containr.h"
#endif

#include "wx/statline.h"
#include "wx/sysopt.h"
#include "wx/private/stattext.h"


// ----------------------------------------------------------------------------
// wxDialogBase
// ----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(wxDialogBase, wxTopLevelWindow)
    EVT_BUTTON(wxID_ANY, wxDialogBase::OnButton)

    EVT_CLOSE(wxDialogBase::OnCloseWindow)

    EVT_CHAR_HOOK(wxDialogBase::OnCharHook)
END_EVENT_TABLE()

void wxDialogBase::Init()
{
    m_returnCode = 0;
    m_affirmativeId = wxID_OK;
    m_escapeId = wxID_ANY;

    // the dialogs have this flag on by default to prevent the events from the
    // dialog controls from reaching the parent frame which is usually
    // undesirable and can lead to unexpected and hard to find bugs
    SetExtraStyle(GetExtraStyle() | wxWS_EX_BLOCK_EVENTS);
}

// helper of GetParentForModalDialog()
static bool CanBeUsedAsParent(wxWindow *parent)
{
    extern WXDLLIMPEXP_DATA_CORE(wxList) wxPendingDelete;

    return !parent->HasExtraStyle(wxWS_EX_TRANSIENT) &&
                parent->IsShownOnScreen() &&
                    !wxPendingDelete.Member(parent) &&
                        !parent->IsBeingDeleted();
}

wxWindow *wxDialogBase::GetParentForModalDialog(wxWindow *parent) const
{
    // creating a parent-less modal dialog will result (under e.g. wxGTK2)
    // in an unfocused dialog, so try to find a valid parent for it:
    if ( parent )
        parent = wxGetTopLevelParent(parent);

    if ( !parent || !CanBeUsedAsParent(parent) )
        parent = wxTheApp->GetTopWindow();

    if ( parent && !CanBeUsedAsParent(parent) )
    {
        // can't use this one, it's going to disappear
        parent = NULL;
    }

    return parent;
}

#if wxUSE_STATTEXT

class wxTextSizerWrapper : public wxTextWrapper
{
public:
    wxTextSizerWrapper(wxWindow *win)
    {
        m_win = win;
        m_hLine = 0;
    }

    wxSizer *CreateSizer(const wxString& text, int widthMax)
    {
        m_sizer = new wxBoxSizer(wxVERTICAL);
        Wrap(m_win, text, widthMax);
        return m_sizer;
    }

protected:
    virtual void OnOutputLine(const wxString& line)
    {
        if ( !line.empty() )
        {
            m_sizer->Add(new wxStaticText(m_win, wxID_ANY, line));
        }
        else // empty line, no need to create a control for it
        {
            if ( !m_hLine )
                m_hLine = m_win->GetCharHeight();

            m_sizer->Add(5, m_hLine);
        }
    }

private:
    wxWindow *m_win;
    wxSizer *m_sizer;
    int m_hLine;
};

wxSizer *wxDialogBase::CreateTextSizer(const wxString& message)
{
    // I admit that this is complete bogus, but it makes
    // message boxes work for pda screens temporarily..
    int widthMax = -1;
    const bool is_pda = wxSystemSettings::GetScreenType() <= wxSYS_SCREEN_PDA;
    if (is_pda)
    {
        widthMax = wxSystemSettings::GetMetric( wxSYS_SCREEN_X ) - 25;
    }

    // '&' is used as accel mnemonic prefix in the wxWidgets controls but in
    // the static messages created by CreateTextSizer() (used by wxMessageBox,
    // for example), we don't want this special meaning, so we need to quote it
    wxString text(message);
    text.Replace(_T("&"), _T("&&"));

    wxTextSizerWrapper wrapper(this);

    return wrapper.CreateSizer(text, widthMax);
}

#endif // wxUSE_STATTEXT

wxSizer *wxDialogBase::CreateButtonSizer(long flags)
{
    wxSizer *sizer = NULL;

#ifdef __SMARTPHONE__
    wxDialog* dialog = (wxDialog*) this;
    if ( flags & wxOK )
        dialog->SetLeftMenu(wxID_OK);

    if ( flags & wxCANCEL )
        dialog->SetRightMenu(wxID_CANCEL);

    if ( flags & wxYES )
        dialog->SetLeftMenu(wxID_YES);

    if ( flags & wxNO )
        dialog->SetRightMenu(wxID_NO);
#else // !__SMARTPHONE__

#if wxUSE_BUTTON

#ifdef __POCKETPC__
    // PocketPC guidelines recommend for Ok/Cancel dialogs to use OK button
    // located inside caption bar and implement Cancel functionality through
    // Undo outside dialog. As native behaviour this will be default here but
    // can be replaced with real wxButtons by setting the option below to 1
    if ( (flags & ~(wxCANCEL|wxNO_DEFAULT)) != wxOK ||
            wxSystemOptions::GetOptionInt(wxT("wince.dialog.real-ok-cancel")) )
#endif // __POCKETPC__
    {
        sizer = CreateStdDialogButtonSizer(flags);
    }
#endif // wxUSE_BUTTON

#endif // __SMARTPHONE__/!__SMARTPHONE__

    return sizer;
}

wxSizer *wxDialogBase::CreateSeparatedButtonSizer(long flags)
{
    wxSizer *sizer = CreateButtonSizer(flags);
    if ( !sizer )
        return NULL;

    // Mac Human Interface Guidelines recommend not to use static lines as
    // grouping elements
#if wxUSE_STATLINE && !defined(__WXMAC__)
    wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);
    topsizer->Add(new wxStaticLine(this),
                   wxSizerFlags().Expand().DoubleBorder(wxBOTTOM));
    topsizer->Add(sizer, wxSizerFlags().Expand());
    sizer = topsizer;
#endif // wxUSE_STATLINE

    return sizer;
}

#if wxUSE_BUTTON

wxStdDialogButtonSizer *wxDialogBase::CreateStdDialogButtonSizer( long flags )
{
    wxStdDialogButtonSizer *sizer = new wxStdDialogButtonSizer();

    wxButton *ok = NULL;
    wxButton *yes = NULL;
    wxButton *no = NULL;

    if (flags & wxOK)
    {
        ok = new wxButton(this, wxID_OK);
        sizer->AddButton(ok);
    }

    if (flags & wxCANCEL)
    {
        wxButton *cancel = new wxButton(this, wxID_CANCEL);
        sizer->AddButton(cancel);
    }

    if (flags & wxYES)
    {
        yes = new wxButton(this, wxID_YES);
        sizer->AddButton(yes);
    }

    if (flags & wxNO)
    {
        no = new wxButton(this, wxID_NO);
        sizer->AddButton(no);
    }

    if (flags & wxAPPLY)
    {
        wxButton *apply = new wxButton(this, wxID_APPLY);
        sizer->AddButton(apply);
    }

    if (flags & wxCLOSE)
    {
        wxButton *close = new wxButton(this, wxID_CLOSE);
        sizer->AddButton(close);
    }

    if (flags & wxHELP)
    {
        wxButton *help = new wxButton(this, wxID_HELP);
        sizer->AddButton(help);
    }

    if (flags & wxNO_DEFAULT)
    {
        if (no)
        {
            no->SetDefault();
            no->SetFocus();
        }
    }
    else
    {
        if (ok)
        {
            ok->SetDefault();
            ok->SetFocus();
        }
        else if (yes)
        {
            yes->SetDefault();
            yes->SetFocus();
        }
    }

    if (flags & wxOK)
        SetAffirmativeId(wxID_OK);
    else if (flags & wxYES)
        SetAffirmativeId(wxID_YES);

    sizer->Realize();

    return sizer;
}

#endif // wxUSE_BUTTON

// ----------------------------------------------------------------------------
// standard buttons handling
// ----------------------------------------------------------------------------

void wxDialogBase::EndDialog(int rc)
{
    if ( IsModal() )
        EndModal(rc);
    else
        Hide();
}

void wxDialogBase::AcceptAndClose()
{
    if ( Validate() && TransferDataFromWindow() )
    {
        EndDialog(m_affirmativeId);
    }
}

void wxDialogBase::SetAffirmativeId(int affirmativeId)
{
    m_affirmativeId = affirmativeId;
}

void wxDialogBase::SetEscapeId(int escapeId)
{
    m_escapeId = escapeId;
}

bool wxDialogBase::EmulateButtonClickIfPresent(int id)
{
#if wxUSE_BUTTON
    wxButton *btn = wxDynamicCast(FindWindow(id), wxButton);

    if ( !btn || !btn->IsEnabled() || !btn->IsShown() )
        return false;

    wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, id);
    event.SetEventObject(btn);
    btn->GetEventHandler()->ProcessEvent(event);

    return true;
#else // !wxUSE_BUTTON
    wxUnusedVar(id);
    return false;
#endif // wxUSE_BUTTON/!wxUSE_BUTTON
}

bool wxDialogBase::IsEscapeKey(const wxKeyEvent& event)
{
    // for most platforms, Esc key is used to close the dialogs
    return event.GetKeyCode() == WXK_ESCAPE &&
                event.GetModifiers() == wxMOD_NONE;
}

void wxDialogBase::OnCharHook(wxKeyEvent& event)
{
    if ( event.GetKeyCode() == WXK_ESCAPE )
    {
        int idCancel = GetEscapeId();
        switch ( idCancel )
        {
            case wxID_NONE:
                // don't handle Esc specially at all
                break;

            case wxID_ANY:
                // this value is special: it means translate Esc to wxID_CANCEL
                // but if there is no such button, then fall back to wxID_OK
                if ( EmulateButtonClickIfPresent(wxID_CANCEL) )
                    return;
                idCancel = GetAffirmativeId();
                // fall through

            default:
                // translate Esc to button press for the button with given id
                if ( EmulateButtonClickIfPresent(idCancel) )
                    return;
        }
    }

    event.Skip();
}

void wxDialogBase::OnButton(wxCommandEvent& event)
{
    const int id = event.GetId();
    if ( id == GetAffirmativeId() )
    {
        AcceptAndClose();
    }
    else if ( id == wxID_APPLY )
    {
        if ( Validate() )
            TransferDataFromWindow();

        // TODO: disable the Apply button until things change again
    }
    else if ( id == GetEscapeId() ||
                (id == wxID_CANCEL && GetEscapeId() == wxID_ANY) )
    {
        EndDialog(wxID_CANCEL);
    }
    else // not a standard button
    {
        event.Skip();
    }
}

// ----------------------------------------------------------------------------
// other event handlers
// ----------------------------------------------------------------------------

void wxDialogBase::OnCloseWindow(wxCloseEvent& WXUNUSED(event))
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

void wxDialogBase::OnSysColourChanged(wxSysColourChangedEvent& event)
{
#ifndef __WXGTK__
    SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE));
    Refresh();
#endif

    event.Skip();
}
