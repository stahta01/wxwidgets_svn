/////////////////////////////////////////////////////////////////////////////
// Name:        wx/msw/slider95.h
// Purpose:     wxSlider95 class
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_SLIDER95_H_
#define _WX_SLIDER95_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "slider95.h"
#endif

class WXDLLEXPORT wxSubwindows;

// Slider
class WXDLLEXPORT wxSlider95 : public wxSliderBase
{
public:
    wxSlider95() { Init(); }

    wxSlider95(wxWindow *parent,
               wxWindowID id,
               int value,
               int minValue,
               int maxValue,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = wxSL_HORIZONTAL,
               const wxValidator& validator = wxDefaultValidator,
               const wxString& name = wxSliderNameStr)
    {
        Init();

        (void)Create(parent, id, value, minValue, maxValue,
                     pos, size, style, validator, name);
    }

    bool Create(wxWindow *parent,
                wxWindowID id,
                int value,
                int minValue, int maxValue,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxSL_HORIZONTAL,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = wxSliderNameStr);

    virtual ~wxSlider95();

    // slider methods
    virtual int GetValue() const;
    virtual void SetValue(int);

    void SetRange(int minValue, int maxValue);

    int GetMin() const { return m_rangeMin; }
    int GetMax() const { return m_rangeMax; }

    // Win32-specific slider methods
    void SetTickFreq(int n, int pos);
    int GetTickFreq() const { return m_tickFreq; }
    void SetPageSize(int pageSize);
    int GetPageSize() const;
    void ClearSel();
    void ClearTicks();
    void SetLineSize(int lineSize);
    int GetLineSize() const;
    int GetSelEnd() const;
    int GetSelStart() const;
    void SetSelection(int minPos, int maxPos);
    void SetThumbLength(int len);
    int GetThumbLength() const;
    void SetTick(int tickPos);

    // implementation only from now on
    WXHWND GetStaticMin() const;
    WXHWND GetStaticMax() const;
    WXHWND GetEditValue() const;
    virtual bool ContainsHWND(WXHWND hWnd) const;

    void Command(wxCommandEvent& event);
    virtual bool MSWOnScroll(int orientation, WXWORD wParam,
                             WXWORD pos, WXHWND control);

    virtual bool Show(bool show = true);
    virtual bool Enable(bool show = true);
    virtual bool SetFont(const wxFont& font);

protected:
    // common part of all ctors
    void Init();

    // format an integer value as string
    static wxString Format(int n) { return wxString::Format(_T("%d"), n); }

	// Common processing to invert slider values based on wxSL_INVERSE 
    int MSWInvertOrNot(int value) const; 

    // get the boundig box for the slider and possible labels
    wxRect GetBoundingBox() const;

    // get the height and, if the pointer is not NULL, width of our labels
    int GetLabelsSize(int *width = NULL) const;


    // overridden base class virtuals
    virtual void DoGetPosition(int *x, int *y) const;
    virtual void DoGetSize(int *width, int *height) const;
    virtual void DoMoveWindow(int x, int y, int width, int height);
    virtual wxSize DoGetBestSize() const;

    virtual wxBorder GetDefaultBorder() const { return wxBORDER_NONE; }

    virtual WXDWORD MSWGetStyle(long flags, WXDWORD *exstyle = NULL) const;


    // the labels windows, if any
    wxSubwindows *m_labels;

    int           m_rangeMin;
    int           m_rangeMax;
    int           m_pageSize;
    int           m_lineSize;
    int           m_tickFreq;


    DECLARE_DYNAMIC_CLASS_NO_COPY(wxSlider95)
};

#endif // _WX_SLIDER95_H_

