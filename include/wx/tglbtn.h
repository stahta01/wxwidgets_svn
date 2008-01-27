/////////////////////////////////////////////////////////////////////////////
// Name:        wx/tglbtn.h
// Purpose:     This dummy header includes the proper header file for the
//              system we're compiling under.
// Author:      John Norris, minor changes by Axel Schlueter
// Modified by:
// Created:     08.02.01
// RCS-ID:      $Id$
// Copyright:   (c) 2000 Johnny C. Norris II
// License:     wxWindows Licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_TOGGLEBUTTON_H_BASE_
#define _WX_TOGGLEBUTTON_H_BASE_

#include "wx/defs.h"

#if wxUSE_TOGGLEBTN

#include "wx/event.h"
#include "wx/control.h"     // base class

extern WXDLLIMPEXP_CORE const wxEventType wxEVT_COMMAND_TOGGLEBUTTON_CLICKED;

// ----------------------------------------------------------------------------
// wxToggleButtonBase
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxToggleButtonBase : public wxControl
{
public:
    wxToggleButtonBase() { }

    // Get/set the value
    virtual void SetValue(bool state) = 0;
    virtual bool GetValue() const = 0;

    void UpdateWindowUI(long flags)
    {
        wxControl::UpdateWindowUI(flags);

        if ( !IsShown() )
            return;

        wxWindow *tlw = wxGetTopLevelParent( this );
        if (tlw && wxPendingDelete.Member( tlw ))
           return;

        wxUpdateUIEvent event( GetId() );
        event.SetEventObject(this);

        if (GetEventHandler()->ProcessEvent(event) )
        {
            if ( event.GetSetChecked() )
                SetValue( event.GetChecked() );
        }
    }

    // Buttons on MSW can look bad if they are not native colours, because
    // then they become owner-drawn and not theme-drawn.  Disable it here
    // in wxToggleButtonBase to make it consistent.
    virtual bool ShouldInheritColours() const { return false; }

protected:
    // choose the default border for this window
    virtual wxBorder GetDefaultBorder() const { return wxBORDER_NONE; }

    DECLARE_NO_COPY_CLASS(wxToggleButtonBase)
};


#define EVT_TOGGLEBUTTON(id, fn) \
    wx__DECLARE_EVT1(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, id, wxCommandEventHandler(fn))

#if defined(__WXUNIVERSAL__)
    #include "wx/univ/tglbtn.h"
#elif defined(__WXMSW__)
    #include "wx/msw/tglbtn.h"
#elif defined(__WXGTK20__)
    #include "wx/gtk/tglbtn.h"
#elif defined(__WXGTK__)
    #include "wx/gtk1/tglbtn.h"
# elif defined(__WXMOTIF__)
    #include "wx/motif/tglbtn.h"
#elif defined(__WXMAC__)
    #include "wx/mac/tglbtn.h"
#elif defined(__WXPALMOS__)
    #include "wx/palmos/tglbtn.h"
#elif defined(__WXPM__)
    #include "wx/os2/tglbtn.h"
#endif

#endif // wxUSE_TOGGLEBTN

#endif // _WX_TOGGLEBUTTON_H_BASE_

