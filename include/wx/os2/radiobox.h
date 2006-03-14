/////////////////////////////////////////////////////////////////////////////
// Name:        wx/os2/radiobox.h
// Purpose:     wxRadioBox class
// Author:      David Webster
// Modified by:
// Created:     10/12/99
// RCS-ID:      $Id$
// Copyright:   (c) David Webster
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_RADIOBOX_H_
#define _WX_RADIOBOX_H_

// List box item
class WXDLLEXPORT wxBitmap ;

class WXDLLEXPORT wxRadioBox: public wxControl, public wxRadioBoxBase
{
public:
    wxRadioBox();

    inline wxRadioBox( wxWindow*          pParent
                      ,wxWindowID         vId
                      ,const wxString&    rsTitle
                      ,const wxPoint&     rPos = wxDefaultPosition
                      ,const wxSize&      rSize = wxDefaultSize
                      ,int                nNum = 0
                      ,const wxString     asChoices[] = NULL
                      ,int                nMajorDim = 0
                      ,long               lStyle = wxRA_HORIZONTAL
                      ,const wxValidator& rVal = wxDefaultValidator
                      ,const wxString&    rsName = wxRadioBoxNameStr
                     )
    {
        Create( pParent
               ,vId
               ,rsTitle
               ,rPos
               ,rSize
               ,nNum
               ,asChoices
               ,nMajorDim
               ,lStyle
               ,rVal
               ,rsName
              );
    }

    inline wxRadioBox( wxWindow*            pParent
                      ,wxWindowID           vId
                      ,const wxString&      rsTitle
                      ,const wxPoint&       rPos
                      ,const wxSize&        rSize
                      ,const wxArrayString& asChoices
                      ,int                  nMajorDim = 0
                      ,long                 lStyle = wxRA_HORIZONTAL
                      ,const wxValidator&   rVal = wxDefaultValidator
                      ,const wxString&      rsName = wxRadioBoxNameStr
                     )
    {
        Create( pParent
               ,vId
               ,rsTitle
               ,rPos
               ,rSize
               ,asChoices
               ,nMajorDim
               ,lStyle
               ,rVal
               ,rsName
              );
    }

    ~wxRadioBox();

    bool Create( wxWindow*          pParent
                ,wxWindowID         vId
                ,const wxString&    rsTitle
                ,const wxPoint&     rPos = wxDefaultPosition
                ,const wxSize&      rSize = wxDefaultSize
                ,int                nNum = 0
                ,const wxString     asChoices[] = NULL
                ,int                nMajorDim = 0
                ,long               lStyle = wxRA_HORIZONTAL
                ,const wxValidator& rVal = wxDefaultValidator
                ,const wxString&    rsName = wxRadioBoxNameStr
               );

    bool Create( wxWindow*            pParent
                ,wxWindowID           vId
                ,const wxString&      rsTitle
                ,const wxPoint&       rPos
                ,const wxSize&        rSize
                ,const wxArrayString& asChoices
                ,int                  nMajorDim = 0
                ,long                 lStyle = wxRA_HORIZONTAL
                ,const wxValidator&   rVal = wxDefaultValidator
                ,const wxString&      rsName = wxRadioBoxNameStr
               );

    void Command(wxCommandEvent& rEvent);
    bool ContainsHWND(WXHWND hWnd) const;
    virtual bool Enable(bool bEnable = true);
    virtual bool Enable(int  nItem, bool bEnable = true);

    virtual WXHBRUSH OnCtlColor( WXHDC    hDC
                                ,WXHWND   hWnd
                                ,WXUINT   uCtlColor
                                ,WXUINT   uMessage
                                ,WXWPARAM wParam
                                ,WXLPARAM lParam
                               );
    virtual bool     OS2Command( WXUINT uParam
                                ,WXWORD wId
                               );
    void             SendNotificationEvent(void);
    virtual bool     Show(int  nItem, bool bShow = true);
    virtual bool     Show(bool bShow = true);
    MRESULT          WindowProc( WXUINT   uMsg
                                ,WXWPARAM wParam
                                ,WXLPARAM lParam
                               );




           virtual size_t   GetCount() const;
    inline         WXHWND*  GetRadioButtons(void) const { return m_ahRadioButtons; }
                   int      GetSelection(void) const;
                   void     GetSize( int* pnX
                                    ,int* pnY
                                   ) const;
    inline         int      GetSizeFlags(void) const { return m_nSizeFlags; }
                   wxString GetString(int nIndex) const;
           virtual wxString GetStringSelection(void) const;

    inline         void     SetButtonFont(const wxFont& rFont) { SetFont(rFont); }
                   void     SetFocus(void);
           virtual bool     SetFont(const wxFont& rFont);
    inline         void     SetLabelFont(const wxFont& WXUNUSED(font)) {};
                   void     SetSelection(int nIndex);
           virtual void     SetString( int             nNum
                                      ,const wxString& rsLabel
                                     );
    virtual bool SetStringSelection(const wxString& rsStr);

    virtual void SetLabel(const wxString& rsLabel)
        { wxControl::SetLabel(rsLabel); }
    virtual wxString GetLabel() const
        { return wxControl::GetLabel(); }

    void SetLabel( int nItem, const wxString& rsLabel );
    void SetLabel( int item, wxBitmap* pBitmap );
    wxString GetLabel(int nItem) const;

protected:
    virtual wxSize DoGetBestSize(void) const;
    virtual void   DoSetSize( int nX
                             ,int nY
                             ,int nWidth
                             ,int nHeight
                             ,int nSizeFlags = wxSIZE_AUTO
                            );
    wxSize GetMaxButtonSize(void) const;
    wxSize GetTotalButtonSize(const wxSize& rSizeBtn) const;
    void   SubclassRadioButton(WXHWND hWndBtn);


    WXHWND* m_ahRadioButtons;
    int*    m_pnRadioWidth;  // for bitmaps
    int*    m_pnRadioHeight;
    int     m_nSelectedButton;
    int     m_nSizeFlags;

private:

    size_t  m_nNoItems;

    DECLARE_DYNAMIC_CLASS(wxRadioBox)
}; // end of wxRadioBox

#endif // _WX_RADIOBOX_H_
