/////////////////////////////////////////////////////////////////////////////
// Name:        progdlgg.h
// Purpose:     wxProgressDialog class
// Author:      Karsten Ball�der
// Modified by:
// Created:     09.05.1999
// RCS-ID:      $Id$
// Copyright:   (c) Karsten Ball�der
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "progdlgg.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_PROGRESSDLG

#ifndef WX_PRECOMP
    #include "wx/utils.h"
    #include "wx/frame.h"
    #include "wx/button.h"
    #include "wx/stattext.h"
    #include "wx/sizer.h"
    #include "wx/event.h"
    #include "wx/gauge.h"
    #include "wx/intl.h"
    #include "wx/settings.h"
    #include "wx/dcclient.h"
    #include "wx/timer.h"
#endif

#include "wx/generic/progdlgg.h"

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

#define LAYOUT_MARGIN 8

// ----------------------------------------------------------------------------
// private functions
// ----------------------------------------------------------------------------

// update the label to show the given time (in seconds)
static void SetTimeLabel(unsigned long val, wxStaticText *label);

// ----------------------------------------------------------------------------
// event tables
// ----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(wxProgressDialog, wxDialog)
    EVT_BUTTON(wxID_CANCEL, wxProgressDialog::OnCancel)

    EVT_CLOSE(wxProgressDialog::OnClose)
END_EVENT_TABLE()

IMPLEMENT_CLASS(wxProgressDialog, wxDialog)

// ============================================================================
// wxProgressDialog implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxProgressDialog creation
// ----------------------------------------------------------------------------

wxProgressDialog::wxProgressDialog(wxString const &title,
                                   wxString const &message,
                                   int maximum,
                                   wxWindow *parent,
                                   int style)
                : wxDialog(parent, wxID_ANY, title),
                  m_delay(3)
{
    // we may disappear at any moment, let the others know about it
    SetExtraStyle(GetExtraStyle() | wxWS_EX_TRANSIENT);

    m_windowStyle |= style;

    bool hasAbortButton = (style & wxPD_CAN_ABORT) != 0;

#if defined(__WXMSW__) && !defined(__WXUNIVERSAL__)
    // we have to remove the "Close" button from the title bar then as it is
    // confusing to have it - it doesn't work anyhow
    //
    // FIXME: should probably have a (extended?) window style for this
    if ( !hasAbortButton )
    {
        EnableCloseButton(false);
    }
#endif // wxMSW

    m_state = hasAbortButton ? Continue : Uncancelable;
    m_maximum = maximum;

#if defined(__WXMSW__) || defined(__WXPM__)
    // we can't have values > 65,536 in the progress control under Windows, so
    // scale everything down
    m_factor = m_maximum / 65536 + 1;
    m_maximum /= m_factor;
#endif // __WXMSW__

    m_parentTop = wxGetTopLevelParent(parent);

    wxClientDC dc(this);
    dc.SetFont(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT));
    long widthText;
    dc.GetTextExtent(message, &widthText, NULL, NULL, NULL, NULL);

    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	
    m_msg = new wxStaticText(this, wxID_ANY, message);
    sizer->Add(m_msg, 0, wxLEFT | wxTOP, 2*LAYOUT_MARGIN);

    wxSize sizeDlg,
           sizeLabel = m_msg->GetSize();
    sizeDlg.y = 2*LAYOUT_MARGIN + sizeLabel.y;

    if ( maximum > 0 )
    {
        // note that we can't use wxGA_SMOOTH because it happens to
        // cause the dialog to be modal. Have an extra
        // style argument to wxProgressDialog, perhaps.
        m_gauge = new wxGauge(this, wxID_ANY, m_maximum,
                              wxDefaultPosition, wxDefaultSize,
                              wxGA_HORIZONTAL);

        sizer->Add(m_gauge, 0, wxLEFT | wxRIGHT | wxTOP | wxEXPAND, 2*LAYOUT_MARGIN);
        m_gauge->SetValue(0);

        wxSize sizeGauge = m_gauge->GetSize();
        sizeDlg.y += 2*LAYOUT_MARGIN + sizeGauge.y;
    }
    else
        m_gauge = (wxGauge *)NULL;

    // create the estimated/remaining/total time zones if requested
    m_elapsed = m_estimated = m_remaining = (wxStaticText*)NULL;
    m_display_estimated = m_last_timeupdate = m_break = 0;
    m_ctdelay = 0;

    // if we are going to have at least one label, remmeber it in this var
    wxStaticText *label = NULL;

    // also count how many labels we really have
    size_t nTimeLabels = 0;

    if ( style & wxPD_ELAPSED_TIME )
    {
        nTimeLabels++;

        label =
        m_elapsed = CreateLabel(_("Elapsed time : "), sizer);
    }

    if ( style & wxPD_ESTIMATED_TIME )
    {
        nTimeLabels++;

        label =
        m_estimated = CreateLabel(_("Estimated time : "), sizer);
    }

    if ( style & wxPD_REMAINING_TIME )
    {
        nTimeLabels++;

        label =
        m_remaining = CreateLabel(_("Remaining time : "), sizer);
    }

    if ( nTimeLabels > 0 )
    {
        // set it to the current time
        m_timeStart = wxGetCurrentTime();
        sizeDlg.y += nTimeLabels * (label->GetSize().y + LAYOUT_MARGIN);
    }

    if ( hasAbortButton )
    {
        m_btnAbort = new wxButton(this, wxID_CANCEL);

        // Windows dialogs usually have buttons in the lower right corner
#if defined(__WXMSW__) || defined(__WXPM__)
        sizer->Add(m_btnAbort, 0, wxALIGN_RIGHT | wxALL, 2*LAYOUT_MARGIN);
#else // !MSW
        sizer->Add(m_btnAbort, 0, wxALIGN_CENTER_HORIZONTAL | wxBOTTOM | wxTOP, 2*LAYOUT_MARGIN);
#endif // MSW/!MSW
        sizeDlg.y += 2*LAYOUT_MARGIN + wxButton::GetDefaultSize().y;
    }
    else // no "Cancel" button
    {
        m_btnAbort = (wxButton *)NULL;
    }

    SetSizerAndFit(sizer);

    sizeDlg.y += 2*LAYOUT_MARGIN;

    // try to make the dialog not square but rectangular of reasonabel width
    sizeDlg.x = (wxCoord)wxMax(widthText, 4*sizeDlg.y/3);
    sizeDlg.x *= 3;
    sizeDlg.x /= 2;
    SetClientSize(sizeDlg);

    Centre(wxCENTER_FRAME | wxBOTH);

    if ( style & wxPD_APP_MODAL )
    {
        m_winDisabler = new wxWindowDisabler(this);
    }
    else
    {
        if ( m_parentTop )
            m_parentTop->Disable();
        m_winDisabler = NULL;
    }

    Show();
    Enable();

    // this one can be initialized even if the others are unknown for now
    //
    // NB: do it after calling Layout() to keep the labels correctly aligned
    if ( m_elapsed )
    {
        SetTimeLabel(0, m_elapsed);
    }

    Update();
}

wxStaticText *wxProgressDialog::CreateLabel(const wxString& text,
                                            wxSizer *sizer)
{
    wxBoxSizer *locsizer = new wxBoxSizer(wxHORIZONTAL);

    wxStaticText *dummy = new wxStaticText(this, -1, text);
    wxStaticText *label = new wxStaticText(this, wxID_ANY, _("unknown"));

    // VZ: I like the labels be centered - if the others don't mind, you may
    //     remove "#ifdef __WXMSW__" and use it for all ports
#if defined(__WXMSW__) || defined(__WXPM__) || defined(__WXMAC__)
    locsizer->Add(dummy, 1, wxALIGN_RIGHT);
    locsizer->Add(label, 1, wxALIGN_LEFT | wxLEFT, LAYOUT_MARGIN);
    sizer->Add(locsizer, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, LAYOUT_MARGIN);
#else // !MSW
    sizer->Add(locsizer, 0, wxALIGN_RIGHT | wxRIGHT | wxTOP, LAYOUT_MARGIN);
    locsizer->Add(dummy);
    locsizer->Add(label, 0, wxLEFT, LAYOUT_MARGIN);
#endif // MSW/!MSW

    return label;
}

// ----------------------------------------------------------------------------
// wxProgressDialog operations
// ----------------------------------------------------------------------------

bool
wxProgressDialog::Update(int value, const wxString& newmsg)
{
    wxASSERT_MSG( value == -1 || m_gauge, wxT("cannot update non existent dialog") );

#ifdef __WXMSW__
    value /= m_factor;
#endif // __WXMSW__

    wxASSERT_MSG( value <= m_maximum, wxT("invalid progress value") );

    // fill up the gauge if value == maximum because this means that the dialog
    // is going to close and the gauge shouldn't be partly empty in this case
    if ( m_gauge && value <= m_maximum )
    {
        m_gauge->SetValue(value == m_maximum ? value : value + 1);
    }

    if ( !newmsg.IsEmpty() )
    {
        m_msg->SetLabel(newmsg);

        wxYieldIfNeeded() ;
    }

    if ( (m_elapsed || m_remaining || m_estimated) && (value != 0) )
    {
        unsigned long elapsed = wxGetCurrentTime() - m_timeStart;
        if (    m_last_timeupdate < elapsed
             || value == m_maximum
           )
        {
            m_last_timeupdate = elapsed;
            unsigned long estimated = m_break +
                  (unsigned long)(( (double) (elapsed-m_break) * m_maximum ) / ((double)value)) ;
            if (    estimated > m_display_estimated
                 && m_ctdelay >= 0
               )
            {
                ++m_ctdelay;
            }
            else if (    estimated < m_display_estimated
                      && m_ctdelay <= 0
                    )
            {
                --m_ctdelay;
            }
            else
            {
                m_ctdelay = 0;
            }
            if (    m_ctdelay >= m_delay          // enough confirmations for a higher value
                 || m_ctdelay <= (m_delay*-1)     // enough confirmations for a lower value
                 || value == m_maximum            // to stay consistent
                 || elapsed > m_display_estimated // to stay consistent
                 || ( elapsed > 0 && elapsed < 4 ) // additional updates in the beginning
               )
            {
                m_display_estimated = estimated;
                m_ctdelay = 0;
            }
        }

        unsigned long display_remaining = m_display_estimated - elapsed;
        if ( display_remaining < 0 )
        {
            display_remaining = 0;
        }

        SetTimeLabel(elapsed, m_elapsed);
        SetTimeLabel(m_display_estimated, m_estimated);
        SetTimeLabel(display_remaining, m_remaining);
    }

    if ( value == m_maximum )
    {
        if ( m_state == Finished )
        {
            // ignore multiple calls to Update(m_maximum): it may sometimes be
            // troublesome to ensure that Update() is not called twice with the
            // same value (e.g. because of the rounding errors) and if we don't
            // return now we're going to generate asserts below
            return true;
        }

        // so that we return true below and that out [Cancel] handler knew what
        // to do
        m_state = Finished;
        if( !(GetWindowStyle() & wxPD_AUTO_HIDE) )
        {
            if ( m_btnAbort )
            {
                // tell the user what he should do...
                m_btnAbort->SetLabel(_("Close"));
            }
#if defined(__WXMSW__) && !defined(__WXUNIVERSAL__)
            else // enable the button to give the user a way to close the dlg
            {
                EnableCloseButton();
            }
#endif // __WXMSW__

            if ( !newmsg )
            {
                // also provide the finishing message if the application didn't
                m_msg->SetLabel(_("Done."));
            }

            wxYieldIfNeeded() ;

            (void)ShowModal();
        }
        else // auto hide
        {
            // reenable other windows before hiding this one because otherwise
            // Windows wouldn't give the focus back to the window which had
            // been previously focused because it would still be disabled
            ReenableOtherWindows();

            Hide();
        }
    }
    else
    {
        // we have to yield because not only we want to update the display but
        // also to process the clicks on the cancel button
        wxYieldIfNeeded() ;
    }

    // update the display in case yielding above didn't do it
    Update();

    return m_state != Canceled;
}

void wxProgressDialog::Resume()
{
    m_state = Continue;
    m_ctdelay = m_delay; // force an update of the elapsed/estimated/remaining time
    m_break += wxGetCurrentTime()-m_timeStop;

    // it may have been disabled by OnCancel(), so enable it back to let the
    // user interrupt us again if needed
    m_btnAbort->Enable();
}

bool wxProgressDialog::Show( bool show )
{
    // reenable other windows before hiding this one because otherwise
    // Windows wouldn't give the focus back to the window which had
    // been previously focused because it would still be disabled
    if(!show)
        ReenableOtherWindows();

    return wxDialog::Show(show);
}

// ----------------------------------------------------------------------------
// event handlers
// ----------------------------------------------------------------------------

void wxProgressDialog::OnCancel(wxCommandEvent& event)
{
    if ( m_state == Finished )
    {
        // this means that the count down is already finished and we're being
        // shown as a modal dialog - so just let the default handler do the job
        event.Skip();
    }
    else
    {
        // request to cancel was received, the next time Update() is called we
        // will handle it
        m_state = Canceled;

        // update the button state immediately so that the user knows that the
        // request has been noticed
        m_btnAbort->Disable();

        // save the time when the dialog was stopped
        m_timeStop = wxGetCurrentTime();
    }
}

void wxProgressDialog::OnClose(wxCloseEvent& event)
{
    if ( m_state == Uncancelable )
    {
        // can't close this dialog
        event.Veto();
    }
    else if ( m_state == Finished )
    {
        // let the default handler close the window as we already terminated
        event.Skip();
    }
    else
    {
        // next Update() will notice it
        m_state = Canceled;
        m_btnAbort->Disable();
        m_timeStop = wxGetCurrentTime();
    }
}

// ----------------------------------------------------------------------------
// destruction
// ----------------------------------------------------------------------------

wxProgressDialog::~wxProgressDialog()
{
    // normally this should have been already done, but just in case
    ReenableOtherWindows();
}

void wxProgressDialog::ReenableOtherWindows()
{
    if ( GetWindowStyle() & wxPD_APP_MODAL )
    {
        delete m_winDisabler;
        m_winDisabler = (wxWindowDisabler *)NULL;
    }
    else
    {
        if ( m_parentTop )
            m_parentTop->Enable();
    }
}

// ----------------------------------------------------------------------------
// private functions
// ----------------------------------------------------------------------------

static void SetTimeLabel(unsigned long val, wxStaticText *label)
{
    if ( label )
    {
        wxString s;
        unsigned long hours = val / 3600;
        unsigned long minutes = (val % 3600) / 60;
        unsigned long seconds = val % 60;
        s.Printf(wxT("%lu:%02lu:%02lu"), hours, minutes, seconds);

        if ( s != label->GetLabel() )
            label->SetLabel(s);
    }
}

#endif // wxUSE_PROGRESSDLG
