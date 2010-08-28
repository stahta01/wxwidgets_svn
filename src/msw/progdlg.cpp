/////////////////////////////////////////////////////////////////////////////
// Name:        src/msw/progdlg.cpp
// Purpose:     wxProgressDialog
// Author:      Rickard Westerlund
// Created:     2010-07-22
// RCS-ID:      $Id$
// Copyright:   (c) 2010 wxWidgets team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// Declarations
// ============================================================================

// ----------------------------------------------------------------------------
// Headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_PROGRESSDLG && wxUSE_THREADS

#include "wx/msw/private/msgdlg.h"
#include "wx/progdlg.h"

using namespace wxMSWMessageDialog;

#ifdef wxHAS_MSW_TASKDIALOG

// ----------------------------------------------------------------------------
// Constants
// ----------------------------------------------------------------------------

namespace
{

// Notification values of wxProgressDialogSharedData::m_notifications
const int wxSPDD_VALUE_CHANGED     = 0x0001;
const int wxSPDD_RANGE_CHANGED     = 0x0002;
const int wxSPDD_PBMARQUEE_CHANGED = 0x0004;
const int wxSPDD_TITLE_CHANGED     = 0x0008;
const int wxSPDD_MESSAGE_CHANGED   = 0x0010;
const int wxSPDD_EXPINFO_CHANGED   = 0x0020;
const int wxSPDD_ENABLE_SKIP       = 0x0040;
const int wxSPDD_ENABLE_ABORT      = 0x0080;
const int wxSPDD_DISABLE_SKIP      = 0x0100;
const int wxSPDD_DISABLE_ABORT     = 0x0200;
const int wxSPDD_FINISHED          = 0x0400;
const int wxSPDD_DESTROYED         = 0x0800;

const int Id_SkipBtn = wxID_HIGHEST + 1;

} // anonymous namespace

// ============================================================================
// Helper classes
// ============================================================================

// Class used to share data between the main thread and the task dialog runner.
class wxProgressDialogSharedData
{
public:
    wxProgressDialogSharedData()
    {
        m_hwnd = 0;
        m_value = 0;
        m_progressBarMarquee = false;
        m_skipped = false;
        m_notifications = 0;
    }

    wxCriticalSection m_cs;

    HWND m_hwnd;            // Task dialog handler
    long m_style;           // wxProgressDialog style
    int m_value;
    int m_range;
    wxString m_title;
    wxString m_message;
    wxString m_expandedInformation;
    wxString m_labelCancel; // Privately used by callback.
    unsigned long m_timeStop;

    wxGenericProgressDialog::ProgressDialogState m_state;
    bool m_progressBarMarquee;
    bool m_skipped;

    // Bit field that indicates fields that have been modified by the
    // main thread so the task dialog runner knows what to update.
    int m_notifications;
};

// Runner thread that takes care of displaying and updating the
// task dialog.
class wxProgressDialogTaskRunner : public wxThread
{
public:
    wxProgressDialogTaskRunner(wxWindow* parent)
        : wxThread(wxTHREAD_JOINABLE),
          m_parent(parent)
        { }

    wxProgressDialogSharedData* GetSharedDataObject()
        { return &m_sharedData; }

private:
    wxWindow* m_parent;
    wxProgressDialogSharedData m_sharedData;

    virtual void* Entry();

    static HRESULT CALLBACK TaskDialogCallbackProc(HWND hwnd,
                                                   UINT uNotification,
                                                   WPARAM wParam,
                                                   LPARAM lParam,
                                                   LONG_PTR dwRefData);
};

// ============================================================================
// Helper functions
// ============================================================================

namespace
{

bool UsesCloseButtonOnly(long style)
{
    return !((style & wxPD_CAN_ABORT) || (style & wxPD_AUTO_HIDE));
}

BOOL CALLBACK DisplayCloseButton(HWND hwnd, LPARAM lParam)
{
    wxProgressDialogSharedData *sharedData =
        (wxProgressDialogSharedData *) lParam;

    if ( wxGetWindowText( hwnd ) == sharedData->m_labelCancel )
    {
        sharedData->m_labelCancel = _("Close");
        SendMessage( hwnd, WM_SETTEXT, 0,
                     (LPARAM) sharedData->m_labelCancel.wx_str() );

        return FALSE;
    }

    return TRUE;
}

void PerformNotificationUpdates(HWND hwnd,
                                wxProgressDialogSharedData *sharedData)
{
    // Update the appropriate dialog fields.
    if ( sharedData->m_notifications & wxSPDD_RANGE_CHANGED )
    {
        ::SendMessage( hwnd,
                       TDM_SET_PROGRESS_BAR_RANGE,
                       0,
                       MAKELPARAM(0, sharedData->m_range) );
    }

    if ( sharedData->m_notifications & wxSPDD_VALUE_CHANGED )
    {
        ::SendMessage( hwnd,
                       TDM_SET_PROGRESS_BAR_POS,
                       sharedData->m_value,
                       0 );
    }

    if ( sharedData->m_notifications & wxSPDD_PBMARQUEE_CHANGED )
    {
        BOOL val = sharedData->m_progressBarMarquee ? TRUE : FALSE;
        ::SendMessage( hwnd,
                       TDM_SET_MARQUEE_PROGRESS_BAR,
                       val,
                       0 );
        ::SendMessage( hwnd,
                       TDM_SET_PROGRESS_BAR_MARQUEE,
                       val,
                       0 );
    }

    if ( sharedData->m_notifications & wxSPDD_TITLE_CHANGED )
        ::SetWindowText( hwnd, sharedData->m_title.wx_str() );

    if ( sharedData->m_notifications & wxSPDD_MESSAGE_CHANGED )
    {
        // Split the message in the title string and the rest if it has
        // multiple lines.
        wxString
            title = sharedData->m_message,
            body;

        const size_t posNL = title.find('\n');
        if ( posNL != wxString::npos )
        {
            // There can an extra new line between the first and subsequent
            // lines to separate them as it looks better with the generic
            // version -- but in this one, they're already separated by the use
            // of different dialog elements, so suppress the extra new line.
            int numNLs = 1;
            if ( posNL < title.length() - 1 && title[posNL + 1] == '\n' )
                numNLs++;

            body.assign(title, posNL + numNLs, wxString::npos);
            title.erase(posNL);
        }

        ::SendMessage( hwnd,
                       TDM_SET_ELEMENT_TEXT,
                       TDE_MAIN_INSTRUCTION,
                       (LPARAM) title.wx_str() );

        ::SendMessage( hwnd,
                       TDM_SET_ELEMENT_TEXT,
                       TDE_CONTENT,
                       (LPARAM) body.wx_str() );
    }

    if ( sharedData->m_notifications & wxSPDD_EXPINFO_CHANGED )
    {
        const wxString& expandedInformation =
            sharedData->m_expandedInformation;
        if ( !expandedInformation.empty() )
        {
            ::SendMessage( hwnd,
                           TDM_SET_ELEMENT_TEXT,
                           TDE_EXPANDED_INFORMATION,
                           (LPARAM) expandedInformation.wx_str() );
        }
    }

    if ( sharedData->m_notifications & wxSPDD_ENABLE_SKIP )
        ::SendMessage( hwnd, TDM_ENABLE_BUTTON, Id_SkipBtn, TRUE );

    if ( sharedData->m_notifications & wxSPDD_ENABLE_ABORT )
        ::SendMessage( hwnd, TDM_ENABLE_BUTTON, IDCANCEL, TRUE );

    if ( sharedData->m_notifications & wxSPDD_DISABLE_SKIP )
        ::SendMessage( hwnd, TDM_ENABLE_BUTTON, Id_SkipBtn, FALSE );

    if ( sharedData->m_notifications & wxSPDD_DISABLE_ABORT )
        ::SendMessage( hwnd, TDM_ENABLE_BUTTON, IDCANCEL, FALSE );

    // Is the progress finished?
    if ( sharedData->m_notifications & wxSPDD_FINISHED )
    {
        sharedData->m_state = wxGenericProgressDialog::Finished;

        if ( !(sharedData->m_style & wxPD_AUTO_HIDE) )
        {
            // Change Cancel into Close and activate the button.
            ::SendMessage( hwnd, TDM_ENABLE_BUTTON, Id_SkipBtn, FALSE );
            ::SendMessage( hwnd, TDM_ENABLE_BUTTON, IDCANCEL, TRUE );
            ::EnumChildWindows( hwnd, DisplayCloseButton,
                                (LPARAM) sharedData );
        }
    }
}

} // anonymous namespace

#endif // wxHAS_MSW_TASKDIALOG

// ============================================================================
// wxProgressDialog implementation
// ============================================================================

wxProgressDialog::wxProgressDialog( const wxString& title,
                                    const wxString& message,
                                    int maximum,
                                    wxWindow *parent,
                                    int style )
    : wxGenericProgressDialog(parent, maximum, style),
      m_taskDialogRunner(NULL),
      m_sharedData(NULL),
      m_message(message),
      m_title(title)
{
#ifdef wxHAS_MSW_TASKDIALOG
    if ( HasNativeProgressDialog() )
    {
        Show();
        DisableOtherWindows();

        return;
    }
#endif // wxHAS_MSW_TASKDIALOG

    Create(title, message, maximum, parent, style);
}

wxProgressDialog::~wxProgressDialog()
{
#ifdef wxHAS_MSW_TASKDIALOG
    if ( !m_taskDialogRunner )
        return;

    if ( m_sharedData )
    {
        wxCriticalSectionLocker locker(m_sharedData->m_cs);
        m_sharedData->m_notifications |= wxSPDD_DESTROYED;
    }

    m_taskDialogRunner->Wait();

    delete m_taskDialogRunner;

    ReenableOtherWindows();

    if ( GetTopParent() )
        GetTopParent()->Raise();
#endif // wxHAS_MSW_TASKDIALOG
}

bool wxProgressDialog::Update(int value, const wxString& newmsg, bool *skip)
{
#ifdef wxHAS_MSW_TASKDIALOG
    if ( HasNativeProgressDialog() )
    {
        wxCriticalSectionLocker locker(m_sharedData->m_cs);

        // Do nothing in canceled state.
        if ( !DoNativeBeforeUpdate(skip) )
            return false;

        value /= m_factor;

        wxASSERT_MSG( value <= m_maximum, wxT("invalid progress value") );

        m_sharedData->m_value = value;
        m_sharedData->m_notifications |= wxSPDD_VALUE_CHANGED;

        if ( !newmsg.empty() )
        {
            m_message = newmsg;
            m_sharedData->m_message = newmsg;
            m_sharedData->m_notifications |= wxSPDD_MESSAGE_CHANGED;
        }

        if ( m_sharedData->m_progressBarMarquee )
        {
            m_sharedData->m_progressBarMarquee = false;
            m_sharedData->m_notifications |= wxSPDD_PBMARQUEE_CHANGED;
        }

        UpdateExpandedInformation( value );

        // Has the progress bar finished?
        if ( value == m_maximum )
        {
            if ( m_state == Finished )
                return true;

            m_state = Finished;
            m_sharedData->m_state = Finished;
            m_sharedData->m_notifications |= wxSPDD_FINISHED;
            if( !HasFlag(wxPD_AUTO_HIDE) && newmsg.empty() )
            {
                // Provide the finishing message if the application didn't.
                m_message = _("Done.");
                m_sharedData->m_message = m_message;
                m_sharedData->m_notifications |= wxSPDD_MESSAGE_CHANGED;
            }
        }

        return m_sharedData->m_state != Canceled;
    }
#endif // wxHAS_MSW_TASKDIALOG

    return wxGenericProgressDialog::Update( value, newmsg, skip );
}

bool wxProgressDialog::Pulse(const wxString& newmsg, bool *skip)
{
#ifdef wxHAS_MSW_TASKDIALOG
    if ( HasNativeProgressDialog() )
    {
        wxCriticalSectionLocker locker(m_sharedData->m_cs);

        // Do nothing in canceled state.
        if ( !DoNativeBeforeUpdate(skip) )
            return false;

        if ( !m_sharedData->m_progressBarMarquee )
        {
            m_sharedData->m_progressBarMarquee = true;
            m_sharedData->m_notifications |= wxSPDD_PBMARQUEE_CHANGED;
        }

        if ( !newmsg.empty() )
        {
            m_message = newmsg;
            m_sharedData->m_message = newmsg;
            m_sharedData->m_notifications |= wxSPDD_MESSAGE_CHANGED;
        }

        // The value passed here doesn't matter, only elapsed time makes sense
        // in indeterminate mode anyhow.
        UpdateExpandedInformation(0);

        return m_sharedData->m_state != Canceled;
    }
#endif // wxHAS_MSW_TASKDIALOG

    return wxGenericProgressDialog::Pulse( newmsg, skip );
}

bool wxProgressDialog::DoNativeBeforeUpdate(bool *skip)
{
#ifdef wxHAS_MSW_TASKDIALOG
    if ( HasNativeProgressDialog() )
    {
        if ( m_sharedData->m_skipped  )
        {
            if ( skip && !*skip )
            {
                *skip = true;
                m_sharedData->m_skipped = false;
                m_sharedData->m_notifications |= wxSPDD_ENABLE_SKIP;
            }
        }

        if ( m_sharedData->m_state == Canceled )
            m_timeStop = m_sharedData->m_timeStop;

        return m_sharedData->m_state != Canceled;
    }
#endif // wxHAS_MSW_TASKDIALOG

    wxUnusedVar(skip);
    wxFAIL_MSG( "unreachable" );

    return false;
}

void wxProgressDialog::Resume()
{
    wxGenericProgressDialog::Resume();

#ifdef wxHAS_MSW_TASKDIALOG
    if ( HasNativeProgressDialog() )
    {
        HWND hwnd;

        {
            wxCriticalSectionLocker locker(m_sharedData->m_cs);
            m_sharedData->m_state = m_state;

            // "Skip" was disabled when "Cancel" had been clicked, so re-enable
            // it now.
            m_sharedData->m_notifications |= wxSPDD_ENABLE_SKIP;

            if ( !UsesCloseButtonOnly(m_windowStyle) )
                m_sharedData->m_notifications |= wxSPDD_ENABLE_ABORT;

            hwnd = m_sharedData->m_hwnd;
        } // Unlock m_cs, we can't call any function operating on a dialog with
          // it locked as it can result in a deadlock if the dialog callback is
          // called by Windows.

        // After resuming we need to bring the window on top of the Z-order as
        // it could be hidden by another window shown from the main thread,
        // e.g. a confirmation dialog asking whether the user really wants to
        // abort.
        //
        // Notice that this must be done from the main thread as it owns the
        // currently active window and attempts to do this from the task dialog
        // thread would simply fail.
        ::BringWindowToTop(hwnd);
    }
#endif // wxHAS_MSW_TASKDIALOG
}

int wxProgressDialog::GetValue() const
{
#ifdef wxHAS_MSW_TASKDIALOG
    if ( HasNativeProgressDialog() )
    {
        wxCriticalSectionLocker locker(m_sharedData->m_cs);
        return m_sharedData->m_value;
    }
#endif // wxHAS_MSW_TASKDIALOG

    return wxGenericProgressDialog::GetValue();
}

wxString wxProgressDialog::GetMessage() const
{
#ifdef wxHAS_MSW_TASKDIALOG
    if ( HasNativeProgressDialog() )
        return m_message;
#endif // wxHAS_MSW_TASKDIALOG

    return wxGenericProgressDialog::GetMessage();
}

void wxProgressDialog::SetRange(int maximum)
{
    wxGenericProgressDialog::SetRange( maximum );

#ifdef wxHAS_MSW_TASKDIALOG
    if ( HasNativeProgressDialog() )
    {
        wxCriticalSectionLocker locker(m_sharedData->m_cs);

        m_sharedData->m_range = maximum;
        m_sharedData->m_notifications |= wxSPDD_RANGE_CHANGED;
    }
#endif // wxHAS_MSW_TASKDIALOG
}

bool wxProgressDialog::WasSkipped() const
{
#ifdef wxHAS_MSW_TASKDIALOG
    if ( HasNativeProgressDialog() )
    {
        if ( !m_sharedData )
        {
            // Couldn't be skipped before being shown.
            return false;
        }

        wxCriticalSectionLocker locker(m_sharedData->m_cs);
        return m_sharedData->m_skipped;
    }
#endif // wxHAS_MSW_TASKDIALOG

    return wxGenericProgressDialog::WasSkipped();
}

bool wxProgressDialog::WasCancelled() const
{
#ifdef wxHAS_MSW_TASKDIALOG
    if ( HasNativeProgressDialog() )
    {
        wxCriticalSectionLocker locker(m_sharedData->m_cs);
        return m_sharedData->m_state == Canceled;
    }
#endif // wxHAS_MSW_TASKDIALOG

    return wxGenericProgressDialog::WasCancelled();
}

void wxProgressDialog::SetTitle(const wxString& title)
{
#ifdef wxHAS_MSW_TASKDIALOG
    if ( HasNativeProgressDialog() )
    {
        m_title = title;

        if ( m_sharedData )
        {
            wxCriticalSectionLocker locker(m_sharedData->m_cs);
            m_sharedData->m_title = title;
            m_sharedData->m_notifications = wxSPDD_TITLE_CHANGED;
        }
    }
#endif // wxHAS_MSW_TASKDIALOG

    wxGenericProgressDialog::SetTitle(title);
}

wxString wxProgressDialog::GetTitle() const
{
#ifdef wxHAS_MSW_TASKDIALOG
    if ( HasNativeProgressDialog() )
        return m_title;
#endif // wxHAS_MSW_TASKDIALOG

    return wxGenericProgressDialog::GetTitle();
}

bool wxProgressDialog::Show(bool show)
{
#ifdef wxHAS_MSW_TASKDIALOG
    if ( HasNativeProgressDialog() )
    {
        // The dialog can't be hidden at all and showing it again after it had
        // been shown before doesn't do anything.
        if ( !show || m_taskDialogRunner )
            return false;

        // We're showing the dialog for the first time, create the thread that
        // will manage it.
        m_taskDialogRunner = new wxProgressDialogTaskRunner(GetParent());
        m_sharedData = m_taskDialogRunner->GetSharedDataObject();

        // Initialize shared data.
        m_sharedData->m_title = m_title;
        m_sharedData->m_message = m_message;
        m_sharedData->m_range = m_maximum;
        m_sharedData->m_state = Uncancelable;
        m_sharedData->m_style = m_windowStyle;

        if ( HasFlag(wxPD_CAN_ABORT) )
        {
            m_sharedData->m_state = Continue;
            m_sharedData->m_labelCancel = _("Cancel");
        }
        else if ( !HasFlag(wxPD_AUTO_HIDE) )
        {
            m_sharedData->m_labelCancel = _("Close");
        }

        if ( m_windowStyle & (wxPD_ELAPSED_TIME
                              | wxPD_ESTIMATED_TIME
                              | wxPD_REMAINING_TIME) )
        {
            // Use a non-empty string just to have the collapsible pane shown.
            m_sharedData->m_expandedInformation = " ";
        }

        // Do launch the thread.
        if ( m_taskDialogRunner->Create() != wxTHREAD_NO_ERROR )
        {
            wxLogError( "Unable to create thread!" );
            return false;
        }

        if ( m_taskDialogRunner->Run() != wxTHREAD_NO_ERROR )
        {
            wxLogError( "Unable to start thread!" );
            return false;
        }

        if ( !HasFlag(wxPD_APP_MODAL) )
            GetParent()->Disable();
        //else: otherwise all windows will be disabled by m_taskDialogRunner

        // Do not show the underlying dialog.
        return false;
    }
#endif // wxHAS_MSW_TASKDIALOG

    return wxGenericProgressDialog::Show( show );
}

bool wxProgressDialog::HasNativeProgressDialog() const
{
#ifdef wxHAS_MSW_TASKDIALOG
    // For a native implementation task dialogs are required, which
    // also require at least one button to be present so the flags needs
    // to be checked as well to see if this is the case.
    return HasNativeTaskDialog()
           && ((m_windowStyle & (wxPD_CAN_SKIP | wxPD_CAN_ABORT))
               || !(m_windowStyle & wxPD_AUTO_HIDE));
#else // !wxHAS_MSW_TASKDIALOG
    // This shouldn't be even called in !wxHAS_MSW_TASKDIALOG case but as we
    // still must define the function as returning something, return false.
    return false;
#endif // wxHAS_MSW_TASKDIALOG/!wxHAS_MSW_TASKDIALOG
}

void wxProgressDialog::UpdateExpandedInformation(int value)
{
#ifdef wxHAS_MSW_TASKDIALOG
    unsigned long elapsedTime;
    unsigned long estimatedTime;
    unsigned long remainingTime;
    UpdateTimeEstimates(value, elapsedTime, estimatedTime, remainingTime);

    int realEstimatedTime = estimatedTime,
        realRemainingTime = remainingTime;
    if ( m_sharedData->m_progressBarMarquee )
    {
        // In indeterminate mode we don't have any estimation neither for the
        // remaining nor for estimated time.
        realEstimatedTime =
        realRemainingTime = -1;
    }

    wxString expandedInformation;

    // Calculate the three different timing values.
    if ( m_windowStyle & wxPD_ELAPSED_TIME )
    {
        expandedInformation << GetElapsedLabel()
                            << " "
                            << GetFormattedTime(elapsedTime);
    }

    if ( m_windowStyle & wxPD_ESTIMATED_TIME )
    {
        if ( !expandedInformation.empty() )
            expandedInformation += "\n";

        expandedInformation << GetEstimatedLabel()
                            << " "
                            << GetFormattedTime(realEstimatedTime);
    }

    if ( m_windowStyle & wxPD_REMAINING_TIME )
    {
        if ( !expandedInformation.empty() )
            expandedInformation += "\n";

        expandedInformation << GetRemainingLabel()
                            << " "
                            << GetFormattedTime(realRemainingTime);
    }

    // Update with new timing information.
    if ( expandedInformation != m_sharedData->m_expandedInformation )
    {
        m_sharedData->m_expandedInformation = expandedInformation;
        m_sharedData->m_notifications |= wxSPDD_EXPINFO_CHANGED;
    }
#else // !wxHAS_MSW_TASKDIALOG
    wxUnusedVar(value);
#endif // wxHAS_MSW_TASKDIALOG/!wxHAS_MSW_TASKDIALOG
}

// ----------------------------------------------------------------------------
// wxProgressDialogTaskRunner and related methods
// ----------------------------------------------------------------------------

#ifdef wxHAS_MSW_TASKDIALOG

void* wxProgressDialogTaskRunner::Entry()
{
    WinStruct<TASKDIALOGCONFIG> tdc;
    wxMSWTaskDialogConfig wxTdc;

    {
        wxCriticalSectionLocker locker(m_sharedData.m_cs);

        wxTdc.caption = m_sharedData.m_title.wx_str();
        wxTdc.message = m_sharedData.m_message.wx_str();

        wxTdc.MSWCommonTaskDialogInit( tdc );
        tdc.pfCallback = TaskDialogCallbackProc;
        tdc.lpCallbackData = (LONG_PTR) &m_sharedData;

        // Undo some of the effects of MSWCommonTaskDialogInit().
        tdc.dwFlags &= ~TDF_EXPAND_FOOTER_AREA; // Expand in content area.
        tdc.dwCommonButtons = 0; // Don't use common buttons.

        wxTdc.useCustomLabels = true;

        if ( m_sharedData.m_style & wxPD_CAN_SKIP )
            wxTdc.AddTaskDialogButton( tdc, Id_SkipBtn, 0, _("Skip") );

        // Use a Cancel button when requested or use a Close button when
        // the dialog does not automatically hide.
        if ( (m_sharedData.m_style & wxPD_CAN_ABORT)
             || !(m_sharedData.m_style & wxPD_AUTO_HIDE) )
        {
            wxTdc.AddTaskDialogButton( tdc, IDCANCEL, 0,
                                       m_sharedData.m_labelCancel );
        }

        tdc.dwFlags |= TDF_CALLBACK_TIMER
                       | TDF_SHOW_PROGRESS_BAR
                       | TDF_SHOW_MARQUEE_PROGRESS_BAR;

        if ( !m_sharedData.m_expandedInformation.empty() )
        {
            tdc.pszExpandedInformation =
                m_sharedData.m_expandedInformation.wx_str();
        }
    }

    TaskDialogIndirect_t taskDialogIndirect = GetTaskDialogIndirectFunc();
    if ( !taskDialogIndirect )
        return NULL;

    int msAns;
    HRESULT hr = taskDialogIndirect(&tdc, &msAns, NULL, NULL);
    if ( FAILED(hr) )
        wxLogApiError( "TaskDialogIndirect", hr );

    return NULL;
}

// static
HRESULT CALLBACK
wxProgressDialogTaskRunner::TaskDialogCallbackProc
                            (
                                HWND hwnd,
                                UINT uNotification,
                                WPARAM wParam,
                                LPARAM WXUNUSED(lParam),
                                LONG_PTR dwRefData
                            )
{
    wxProgressDialogSharedData * const sharedData =
        (wxProgressDialogSharedData *) dwRefData;

    wxCriticalSectionLocker locker(sharedData->m_cs);

    switch ( uNotification )
    {
        case TDN_CREATED:
            // Store the HWND for the main thread use.
            sharedData->m_hwnd = hwnd;

            // Set the maximum value and disable Close button.
            ::SendMessage( hwnd,
                           TDM_SET_PROGRESS_BAR_RANGE,
                           0,
                           MAKELPARAM(0, sharedData->m_range) );

            if ( UsesCloseButtonOnly(sharedData->m_style) )
                ::SendMessage( hwnd, TDM_ENABLE_BUTTON, IDCANCEL, FALSE );
            break;

        case TDN_BUTTON_CLICKED:
            switch ( wParam )
            {
                case Id_SkipBtn:
                    ::SendMessage(hwnd, TDM_ENABLE_BUTTON, Id_SkipBtn, FALSE);
                    sharedData->m_skipped = true;
                    return TRUE;

                case IDCANCEL:
                    if ( sharedData->m_state
                            == wxGenericProgressDialog::Finished )
                    {
                        return FALSE;
                    }

                    // Close button on the window triggers an IDCANCEL press,
                    // don't allow it when it should only be possible to close
                    // a finished dialog.
                    if ( !UsesCloseButtonOnly(sharedData->m_style) )
                    {
                        wxCHECK_MSG( sharedData->m_state ==
                                        wxGenericProgressDialog::Continue,
                                    TRUE,
                                    "Dialog not in a cancelable state!");

                        ::SendMessage(hwnd, TDM_ENABLE_BUTTON, Id_SkipBtn, FALSE);
                        ::SendMessage(hwnd, TDM_ENABLE_BUTTON, IDCANCEL, FALSE);

                        sharedData->m_timeStop = wxGetCurrentTime();
                        sharedData->m_state = wxGenericProgressDialog::Canceled;
                    }

                    return TRUE;
            }
            break;

        case TDN_TIMER:
            PerformNotificationUpdates(hwnd, sharedData);

            // End dialog in three different cases:
            // 1. Progress finished and dialog should automatically hide.
            // 2. The wxProgressDialog object was destructed and should
            //    automatically hide.
            // 3. The dialog was canceled and wxProgressDialog object
            //    was destroyed.
            bool isCanceled =
                sharedData->m_state == wxGenericProgressDialog::Canceled;
            bool isFinished =
                sharedData->m_state == wxGenericProgressDialog::Finished;
            bool wasDestroyed =
                (sharedData->m_notifications & wxSPDD_DESTROYED) != 0;
            bool shouldAutoHide = (sharedData->m_style & wxPD_AUTO_HIDE) != 0;

            if ( (shouldAutoHide && (isFinished || wasDestroyed))
                 || (wasDestroyed && isCanceled) )
            {
                ::EndDialog( hwnd, IDCLOSE );
            }

            sharedData->m_notifications = 0;

            return TRUE;
    }

    // Return anything.
    return 0;
}

#endif // wxHAS_MSW_TASKDIALOG

#endif // wxUSE_PROGRESSDLG && wxUSE_THREADS
