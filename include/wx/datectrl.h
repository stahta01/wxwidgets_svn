///////////////////////////////////////////////////////////////////////////////
// Name:        wx/datectrl.h
// Purpose:     implements wxDatePickerCtrl
// Author:      Vadim Zeitlin
// Modified by:
// Created:     2005-01-09
// RCS-ID:      $Id$
// Copyright:   (c) 2005 Vadim Zeitlin <vadim@wxwindows.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_DATECTRL_H_
#define _WX_DATECTRL_H_

#include "wx/control.h"         // the base class
#include "wx/datetime.h"

#define wxDatePickerCtrlNameStr _T("datectrl")

// wxDatePickerCtrl styles
enum
{
    // default style on this platform, either wxDP_SPIN or wxDP_DROPDOWN
    wxDP_DEFAULT = 0,

    // a spin control-like date picker (not supported in generic version)
    wxDP_SPIN = 1,

    // a combobox-like date picker (not supported in mac version)
    wxDP_DROPDOWN = 2,

    // always show century in the default date display (otherwise it depends on
    // the system date format which may include the century or not)
    wxDP_SHOWCENTURY = 4
};

// ----------------------------------------------------------------------------
// wxDatePickerCtrl: allow the user to enter the date
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_ADV wxDatePickerCtrlBase : public wxControl
{
public:
    /*
       The derived classes should implement ctor and Create() method with the
       following signature:

        bool Create(wxWindow *parent,
                    wxWindowID id,
                    const wxDateTime& dt = wxDefaultDateTime,
                    const wxPoint& pos = wxDefaultPosition,
                    const wxSize& size = wxDefaultSize,
                    long style = wxDP_DEFAULT | wxDP_SHOWCENTURY,
                    const wxValidator& validator = wxDefaultValidator,
                    const wxString& name = wxDatePickerCtrlNameStr);
     */

    // set/get the date
    virtual void SetValue(const wxDateTime& dt) = 0;
    virtual wxDateTime GetValue() const = 0;

    // set/get the allowed valid range for the dates, if either/both of them
    // are invalid, there is no corresponding limit and if neither is set
    // GetRange() returns false
    virtual void SetRange(const wxDateTime& dt1, const wxDateTime& dt2) = 0;
    virtual bool GetRange(wxDateTime *dt1, wxDateTime *dt2) const = 0;
};

#if defined(__WXMSW__) && !defined(__WXUNIVERSAL__)
    #include "wx/msw/datectrl.h"

    #define wxHAS_NATIVE_DATEPICKCTRL
#else
    #include "wx/generic/datectrl.h"

    class WXDLLIMPEXP_ADV wxDatePickerCtrl : public wxDatePickerCtrlGeneric
    {
    public:
        wxDatePickerCtrl() { }
        wxDatePickerCtrl(wxWindow *parent,
                         wxWindowID id,
                         const wxDateTime& date = wxDefaultDateTime,
                         const wxPoint& pos = wxDefaultPosition,
                         const wxSize& size = wxDefaultSize,
                         long style = wxDP_DEFAULT | wxDP_SHOWCENTURY,
                         const wxString& name = wxDatePickerCtrlNameStr)
            : wxDatePickerCtrlGeneric(parent, id, date, pos, size, style, name)
        {
        }

    private:
        DECLARE_DYNAMIC_CLASS(wxDatePickerCtrl)
        DECLARE_NO_COPY_CLASS(wxDatePickerCtrl)
    };
#endif

#endif // _WX_DATECTRL_H_

