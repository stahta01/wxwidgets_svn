/////////////////////////////////////////////////////////////////////////////
// Name:        wx/os2/pen.h
// Purpose:     wxPen class
// Author:      David Webster
// Modified by:
// Created:     10/10/99
// RCS-ID:      $Id$
// Copyright:   (c) David Webster
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_PEN_H_
#define _WX_PEN_H_

#include "wx/gdiobj.h"
#include "wx/bitmap.h"

typedef long wxPMDash;

class WXDLLIMPEXP_FWD_CORE wxPen;

class WXDLLEXPORT wxPenRefData: public wxGDIRefData
{
    friend class WXDLLIMPEXP_FWD_CORE wxPen;
public:
    wxPenRefData();
    wxPenRefData(const wxPenRefData& rData);
    virtual ~wxPenRefData();

    bool operator==(const wxPenRefData& data) const
    {
        // we intentionally don't compare m_hPen fields here
        return m_nStyle == data.m_nStyle &&
               m_nWidth == data.m_nWidth &&
               m_nJoin == data.m_nJoin &&
               m_nCap == data.m_nCap &&
               m_vColour == data.m_vColour &&
               (m_nStyle != wxPENSTYLE_STIPPLE || m_vStipple.IsSameAs(data.m_vStipple)) &&
               (m_nStyle != wxPENSTYLE_USER_DASH ||
                (m_dash == data.m_dash &&
                    memcmp(m_dash, data.m_dash, m_nbDash*sizeof(wxDash)) == 0));
    }

protected:
    int                             m_nWidth;
    wxPenStyle                      m_nStyle;
    wxPenJoin                       m_nJoin;
    wxPenCap                        m_nCap;
    wxBitmap                        m_vStipple;
    int                             m_nbDash;
    wxDash *                        m_dash;
    wxColour                        m_vColour;
    WXHPEN                          m_hPen;// in OS/2 GPI this will be the PS the pen is associated with
};

#define M_PENDATA ((wxPenRefData *)m_refData)

// Pen
class WXDLLEXPORT wxPen : public wxPenBase
{
public:
    wxPen();
    wxPen( const wxColour& rColour
          ,int             nWidth = 1
          ,wxPenStyle      nStyle = wxPENSTYLE_SOLID
         );
#if FUTURE_WXWIN_COMPATIBILITY_3_0
    wxDEPRECATED_FUTURE( wxPen(const wxColour& col, int width, int style) );
#endif

    wxPen( const wxBitmap& rStipple
          ,int             nWidth
         );
    virtual ~wxPen();

    inline bool   operator == (const wxPen& rPen) const
    {
        const wxPenRefData *penData = (wxPenRefData *)rPen.m_refData;

        // an invalid pen is only equal to another invalid pen
        return m_refData ? penData && *M_PENDATA == *penData : !penData;
    }

    inline bool   operator != (const wxPen& rPen) const
        { return !(*this == rPen); }

    //
    // Override in order to recreate the pen
    //
    void SetColour(const wxColour& rColour);
    void SetColour(unsigned char cRed, unsigned char cGreen, unsigned char cBlue);

    void SetWidth(int nWidth);
    void SetStyle(wxPenStyle nStyle);
    void SetStipple(const wxBitmap& rStipple);
    void SetDashes( int           nNbDashes
                   ,const wxDash* pDash
                  );
    void SetJoin(wxPenJoin nJoin);
    void SetCap(wxPenCap nCap);
    void SetPS(HPS hPS);

    inline wxColour& GetColour(void) const { return (M_PENDATA ? M_PENDATA->m_vColour : wxNullColour); };
    inline int       GetWidth(void) const { return (M_PENDATA ? M_PENDATA->m_nWidth : -1); };
    inline wxPenStyle GetStyle(void) const { return (M_PENDATA ? M_PENDATA->m_nStyle : wxPENSTYLE_INVALID); };
    inline wxPenJoin  GetJoin(void) const { return (M_PENDATA ? M_PENDATA->m_nJoin : wxJOIN_INVALID); };
    inline wxPenCap   GetCap(void) const { return (M_PENDATA ? M_PENDATA->m_nCap : wxCAP_INVALID); };
    inline int       GetPS(void) const { return (M_PENDATA ? M_PENDATA->m_hPen : 0); };
    inline int       GetDashes(wxDash **ptr) const
    {
        *ptr = (M_PENDATA ? (wxDash*)M_PENDATA->m_dash : (wxDash*) NULL);
        return (M_PENDATA ? M_PENDATA->m_nbDash : -1);
    }
    inline wxDash*   GetDash() const { return (M_PENDATA ? (wxDash*)M_PENDATA->m_dash : (wxDash*)NULL); };
    inline int       GetDashCount() const { return (M_PENDATA ? M_PENDATA->m_nbDash : 0); };

    inline wxBitmap* GetStipple(void) const { return (M_PENDATA ? (& M_PENDATA->m_vStipple) : (wxBitmap*) NULL); };

    //
    // Implementation
    //

    //
    // Useful helper: create the brush resource
    //
    bool     RealizeResource(void);
    bool     FreeResource(bool bForce = false);
    virtual WXHANDLE GetResourceHandle(void) const;
    bool     IsFree(void) const;

private:
    LINEBUNDLE                     m_vLineBundle;
    AREABUNDLE                     m_vAreaBundle;

protected:
    virtual wxGDIRefData* CreateGDIRefData() const;
    virtual wxGDIRefData* CloneGDIRefData(const wxGDIRefData* data) const;

    // same as FreeResource() + RealizeResource()
    bool Recreate();

    DECLARE_DYNAMIC_CLASS(wxPen)
}; // end of CLASS wxPen

extern int wx2os2PenStyle(wxPenStyle nWxStyle);

#endif
    // _WX_PEN_H_
