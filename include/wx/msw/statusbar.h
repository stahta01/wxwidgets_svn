///////////////////////////////////////////////////////////////////////////////
// Name:        wx/msw/statbr.h
// Purpose:     native implementation of wxStatusBar
// Author:      Vadim Zeitlin
// Modified by:
// Created:     04.04.98
// RCS-ID:      $Id$
// Copyright:   (c) 1998 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_MSW_STATUSBAR_H_
#define _WX_MSW_STATUSBAR_H_

#if wxUSE_NATIVE_STATUSBAR

#include "wx/vector.h"
#include "wx/tooltip.h"

class WXDLLIMPEXP_FWD_CORE wxClientDC;

class WXDLLIMPEXP_CORE wxStatusBar : public wxStatusBarBase
{
public:
    // ctors and such
    wxStatusBar();
    wxStatusBar(wxWindow *parent,
                wxWindowID id = wxID_ANY,
                long style = wxSTB_DEFAULT_STYLE,
                const wxString& name = wxStatusBarNameStr)
    {
        m_pDC = NULL;
        (void)Create(parent, id, style, name);
    }

    bool Create(wxWindow *parent,
                wxWindowID id = wxID_ANY,
                long style = wxSTB_DEFAULT_STYLE,
                const wxString& name = wxStatusBarNameStr);

    virtual ~wxStatusBar();

    // implement base class methods
    virtual void SetFieldsCount(int number = 1, const int *widths = NULL);
    virtual void SetStatusWidths(int n, const int widths_field[]);
    virtual void SetStatusStyles(int n, const int styles[]);
    virtual void SetMinHeight(int height);
    virtual bool GetFieldRect(int i, wxRect& rect) const;

    virtual int GetBorderX() const;
    virtual int GetBorderY() const;

    // override some wxWindow virtual methods too
    virtual bool SetFont(const wxFont& font);

    virtual WXLRESULT MSWWindowProc(WXUINT nMsg,
                                    WXWPARAM wParam,
                                    WXLPARAM lParam);

protected:
    // implement base class pure virtual method
    virtual void DoUpdateStatusText(int number);

    // override some base class virtuals
    virtual wxSize DoGetBestSize() const;
    virtual void DoMoveWindow(int x, int y, int width, int height);
#if wxUSE_TOOLTIPS
    virtual bool MSWProcessMessage(WXMSG* pMsg);
    virtual bool MSWOnNotify(int idCtrl, WXLPARAM lParam, WXLPARAM* result);
#endif

    // implementation of the public SetStatusWidths()
    void MSWUpdateFieldsWidths();

    // used by DoUpdateStatusText()
    wxClientDC *m_pDC;

    // the tooltips used when wxSTB_SHOW_TIPS is given
    wxVector<wxToolTip*> m_tooltips;

private:
    DECLARE_DYNAMIC_CLASS_NO_COPY(wxStatusBar)
};

#endif  // wxUSE_NATIVE_STATUSBAR

#endif // _WX_MSW_STATUSBAR_H_
