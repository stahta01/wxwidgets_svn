/////////////////////////////////////////////////////////////////////////////
// Name:        gauge.h
// Purpose:     wxGauge class
// Author:      David Webster
// Modified by:
// Created:     10/06/99
// RCS-ID:      $Id$
// Copyright:   (c) David Webster
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_GAUGE_H_
#define _WX_GAUGE_H_

#include "wx/control.h"

WXDLLEXPORT_DATA(extern const char*) wxGaugeNameStr;

class WXDLLEXPORT wxGauge: public wxControl
{
public:
    inline wxGauge() { m_nRangeMax = 0; m_nGaugePos = 0; }

    inline wxGauge( wxWindow*          pParent
                   ,wxWindowID         vId
                   ,int                nRange
                   ,const wxPoint&     rPos = wxDefaultPosition
                   ,const wxSize&      rSize = wxDefaultSize
                   ,long               lStyle = wxGA_HORIZONTAL
#if wxUSE_VALIDATORS
                   ,const wxValidator& rValidator = wxDefaultValidator
#endif
                   ,const wxString&    rsName = wxGaugeNameStr
                  )
    {
        Create( pParent
               ,vId
               ,nRange
               ,rPos
               ,rSize
               ,lStyle
#if wxUSE_VALIDATORS
               ,rValidator
#endif
               ,rsName
              );
    }

    bool Create( wxWindow*          pParent
                ,wxWindowID         vId
                ,int                nRange
                ,const wxPoint&     rPos = wxDefaultPosition
                ,const wxSize&      rSize = wxDefaultSize
                ,long               lStyle = wxGA_HORIZONTAL
#if wxUSE_VALIDATORS
                ,const wxValidator& rValidator = wxDefaultValidator
#endif
                ,const wxString&    rsName = wxGaugeNameStr
               );

    int  GetShadowWidth(void) const;
    int  GetBezelFace(void) const;
    int  GetRange(void) const;
    int  GetValue(void) const;

    bool SetBackgroundColour(const wxColour& rColour);
    void SetBezelFace(int nWidth);
    bool SetForegroundColour(const wxColour& rColour);
    void SetRange(int nRange);
    void SetShadowWidth(int nWidth);
    void SetValue(int nPos);

    inline virtual bool AcceptsFocus(void) const { return FALSE; }
    inline virtual void Command(wxCommandEvent& WXUNUSED(rEvent)) {};

protected:
    int                             m_nRangeMax;
    int                             m_nGaugePos;

private:
    DECLARE_DYNAMIC_CLASS(wxGauge)
}; // end of CLASS wxGauge

#endif // _WX_GAUGE_H_

