/////////////////////////////////////////////////////////////////////////////
// Program:     wxWindows Widgets Sample
// Name:        slider.cpp
// Purpose:     Part of the widgets sample showing wxSlider
// Author:      Vadim Zeitlin
// Created:     16.04.01
// Id:          $Id$
// Copyright:   (c) 2001 Vadim Zeitlin
// License:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// for compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers
#ifndef WX_PRECOMP
    #include "wx/log.h"

    #include "wx/button.h"
    #include "wx/checkbox.h"
    #include "wx/radiobox.h"
    #include "wx/slider.h"
    #include "wx/statbox.h"
    #include "wx/textctrl.h"
#endif

#if wxUSE_TOOLTIPS
    #include "wx/tooltip.h"
#endif

#include "wx/sizer.h"

#include "widgets.h"
#include "icons/slider.xpm"

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// control ids
enum
{
    SliderPage_Reset = wxID_HIGHEST,
    SliderPage_Clear,
    SliderPage_SetValue,
    SliderPage_SetMinAndMax,
    SliderPage_SetTickFreq,
    SliderPage_SetThumbLen,
    SliderPage_CurValueText,
    SliderPage_ValueText,
    SliderPage_MinText,
    SliderPage_MaxText,
    SliderPage_TickFreqText,
    SliderPage_ThumbLenText,
    SliderPage_RadioSides,
    SliderPage_BothSides,
    SliderPage_Slider
};

// sides radiobox values
enum
{
    StaticSides_Top,
    StaticSides_Bottom,
    StaticSides_Left,
    StaticSides_Right
};

// ----------------------------------------------------------------------------
// SliderWidgetsPage
// ----------------------------------------------------------------------------

class SliderWidgetsPage : public WidgetsPage
{
public:
    SliderWidgetsPage(wxNotebook *notebook, wxImageList *imaglist);
    virtual ~SliderWidgetsPage();

protected:
    // event handlers
    void OnButtonReset(wxCommandEvent& event);
    void OnButtonClear(wxCommandEvent& event);
    void OnButtonSetValue(wxCommandEvent& event);
    void OnButtonSetMinAndMax(wxCommandEvent& event);
    void OnButtonSetTickFreq(wxCommandEvent& event);
    void OnButtonSetThumbLen(wxCommandEvent& event);

    void OnCheckOrRadioBox(wxCommandEvent& event);

    void OnSlider(wxScrollEvent& event);

    void OnUpdateUIValueButton(wxUpdateUIEvent& event);
    void OnUpdateUIMinMaxButton(wxUpdateUIEvent& event);
    void OnUpdateUITickFreq(wxUpdateUIEvent& event);
    void OnUpdateUIThumbLen(wxUpdateUIEvent& event);
    void OnUpdateUIRadioSides(wxUpdateUIEvent& event);
    void OnUpdateUIBothSides(wxUpdateUIEvent& event);

    void OnUpdateUIResetButton(wxUpdateUIEvent& event);

    void OnUpdateUICurValueText(wxUpdateUIEvent& event);

    // reset the slider parameters
    void Reset();

    // (re)create the slider
    void CreateSlider();

    // set the tick frequency from the text field value
    void DoSetTickFreq();

    // set the thumb len from the text field value
    void DoSetThumbLen();

    // is this slider value in range?
    bool IsValidValue(int val) const
        { return (val >= m_min) && (val <= m_max); }

    // the slider range
    int m_min, m_max;

    // the controls
    // ------------

    // the check/radio boxes for styles
    wxCheckBox *m_chkLabels,
               *m_chkVert,
               *m_chkTicks,
               *m_chkBothSides;

    wxRadioBox *m_radioSides;

    // the slider itself and the sizer it is in
    wxSlider *m_slider;
    wxSizer *m_sizerSlider;

    // the text entries for set value/range
    wxTextCtrl *m_textValue,
               *m_textMin,
               *m_textMax,
               *m_textTickFreq,
               *m_textThumbLen;

private:
    DECLARE_EVENT_TABLE()
    DECLARE_WIDGETS_PAGE(SliderWidgetsPage)
};

// ----------------------------------------------------------------------------
// event tables
// ----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(SliderWidgetsPage, WidgetsPage)
    EVT_BUTTON(SliderPage_Reset, SliderWidgetsPage::OnButtonReset)
    EVT_BUTTON(SliderPage_SetValue, SliderWidgetsPage::OnButtonSetValue)
    EVT_BUTTON(SliderPage_SetMinAndMax, SliderWidgetsPage::OnButtonSetMinAndMax)
    EVT_BUTTON(SliderPage_SetTickFreq, SliderWidgetsPage::OnButtonSetTickFreq)
    EVT_BUTTON(SliderPage_SetThumbLen, SliderWidgetsPage::OnButtonSetThumbLen)

    EVT_UPDATE_UI(SliderPage_SetValue, SliderWidgetsPage::OnUpdateUIValueButton)
    EVT_UPDATE_UI(SliderPage_SetMinAndMax, SliderWidgetsPage::OnUpdateUIMinMaxButton)
    EVT_UPDATE_UI(SliderPage_SetTickFreq, SliderWidgetsPage::OnUpdateUITickFreq)
    EVT_UPDATE_UI(SliderPage_SetThumbLen, SliderWidgetsPage::OnUpdateUIThumbLen)
    EVT_UPDATE_UI(SliderPage_TickFreqText, SliderWidgetsPage::OnUpdateUITickFreq)
    EVT_UPDATE_UI(SliderPage_RadioSides, SliderWidgetsPage::OnUpdateUIRadioSides)
    EVT_UPDATE_UI(SliderPage_BothSides, SliderWidgetsPage::OnUpdateUIBothSides)

    EVT_UPDATE_UI(SliderPage_Reset, SliderWidgetsPage::OnUpdateUIResetButton)

    EVT_UPDATE_UI(SliderPage_CurValueText, SliderWidgetsPage::OnUpdateUICurValueText)

    EVT_COMMAND_SCROLL(SliderPage_Slider, SliderWidgetsPage::OnSlider)

    EVT_CHECKBOX(-1, SliderWidgetsPage::OnCheckOrRadioBox)
    EVT_RADIOBOX(-1, SliderWidgetsPage::OnCheckOrRadioBox)
END_EVENT_TABLE()

// ============================================================================
// implementation
// ============================================================================

IMPLEMENT_WIDGETS_PAGE(SliderWidgetsPage, _T("Slider"));

SliderWidgetsPage::SliderWidgetsPage(wxNotebook *notebook,
                                       wxImageList *imaglist)
                  : WidgetsPage(notebook)
{
    imaglist->Add(wxBitmap(slider_xpm));

    // init everything
    m_min = 0;
    m_max = 100;

    m_chkVert =
    m_chkTicks =
    m_chkLabels =
    m_chkBothSides = (wxCheckBox *)NULL;

    m_radioSides = (wxRadioBox *)NULL;

    m_slider = (wxSlider *)NULL;
    m_sizerSlider = (wxSizer *)NULL;

    wxSizer *sizerTop = new wxBoxSizer(wxHORIZONTAL);

    // left pane
    wxStaticBox *box = new wxStaticBox(this, -1, _T("&Set style"));
    wxSizer *sizerLeft = new wxStaticBoxSizer(box, wxVERTICAL);

    m_chkVert = CreateCheckBoxAndAddToSizer(sizerLeft, _T("&Vertical"));
    m_chkTicks = CreateCheckBoxAndAddToSizer(sizerLeft, _T("Show &ticks"));
    m_chkLabels = CreateCheckBoxAndAddToSizer(sizerLeft, _T("Show &labels"));
    static const wxString sides[] =
    {
        _T("top"),
        _T("bottom"),
        _T("left"),
        _T("right"),
    };
    m_radioSides = new wxRadioBox(this, SliderPage_RadioSides, _T("&Ticks/Labels"),
                                 wxDefaultPosition, wxDefaultSize,
                                 WXSIZEOF(sides), sides,
                                 1, wxRA_SPECIFY_COLS);
    sizerLeft->Add(m_radioSides, 0, wxGROW | wxALL, 5);
    m_chkBothSides = CreateCheckBoxAndAddToSizer
                     (sizerLeft, _T("&Both sides"), SliderPage_BothSides);
#if wxUSE_TOOLTIPS
    m_chkBothSides->SetToolTip( _T("\"Both sides\" is only supported \nin Win95 and Universal") );
#endif // wxUSE_TOOLTIPS

    sizerLeft->Add(5, 5, 0, wxGROW | wxALL, 5); // spacer

    wxButton *btn = new wxButton(this, SliderPage_Reset, _T("&Reset"));
    sizerLeft->Add(btn, 0, wxALIGN_CENTRE_HORIZONTAL | wxALL, 15);

    // middle pane
    wxStaticBox *box2 = new wxStaticBox(this, -1, _T("&Change slider value"));
    wxSizer *sizerMiddle = new wxStaticBoxSizer(box2, wxVERTICAL);

    wxTextCtrl *text;
    wxSizer *sizerRow = CreateSizerWithTextAndLabel(_T("Current value"),
                                                    SliderPage_CurValueText,
                                                    &text);
    text->SetEditable(FALSE);

    sizerMiddle->Add(sizerRow, 0, wxALL | wxGROW, 5);

    sizerRow = CreateSizerWithTextAndButton(SliderPage_SetValue,
                                            _T("Set &value"),
                                            SliderPage_ValueText,
                                            &m_textValue);
    sizerMiddle->Add(sizerRow, 0, wxALL | wxGROW, 5);

    sizerRow = CreateSizerWithTextAndButton(SliderPage_SetMinAndMax,
                                            _T("&Min and max"),
                                            SliderPage_MinText,
                                            &m_textMin);

    m_textMax = new wxTextCtrl(this, SliderPage_MaxText, _T(""));
    sizerRow->Add(m_textMax, 1, wxLEFT | wxALIGN_CENTRE_VERTICAL, 5);

    m_textMin->SetValue( wxString::Format(_T("%d"), m_min) );
    m_textMax->SetValue( wxString::Format(_T("%d"), m_max) );

    sizerMiddle->Add(sizerRow, 0, wxALL | wxGROW, 5);

    sizerRow = CreateSizerWithTextAndButton(SliderPage_SetTickFreq,
                                            _T("Tick &frequency"),
                                            SliderPage_TickFreqText,
                                            &m_textTickFreq);

    m_textTickFreq->SetValue(_T("10"));

    sizerMiddle->Add(sizerRow, 0, wxALL | wxGROW, 5);

    sizerRow = CreateSizerWithTextAndButton(SliderPage_SetThumbLen,
                                            _T("Thumb &lenght"),
                                            SliderPage_ThumbLenText,
                                            &m_textThumbLen);

    sizerMiddle->Add(sizerRow, 0, wxALL | wxGROW, 5);

    // right pane
    wxSizer *sizerRight = new wxBoxSizer(wxHORIZONTAL);
    sizerRight->SetMinSize(150, 40);
    m_sizerSlider = sizerRight; // save it to modify it later

    Reset();
    CreateSlider();

    // the 3 panes panes compose the window
    sizerTop->Add(sizerLeft, 0, wxGROW | (wxALL & ~wxLEFT), 10);
    sizerTop->Add(sizerMiddle, 0, wxGROW | wxALL, 10);
    sizerTop->Add(sizerRight, 1, wxGROW | (wxALL & ~wxRIGHT), 10);

    // final initializations
    SetAutoLayout(TRUE);
    SetSizer(sizerTop);

    sizerTop->Fit(this);
}

SliderWidgetsPage::~SliderWidgetsPage()
{
}

// ----------------------------------------------------------------------------
// operations
// ----------------------------------------------------------------------------

void SliderWidgetsPage::Reset()
{
    m_chkVert->SetValue(FALSE);
    m_chkTicks->SetValue(TRUE);
    m_chkLabels->SetValue(TRUE);
    m_chkBothSides->SetValue(FALSE);

    m_radioSides->SetSelection(StaticSides_Top);
}

void SliderWidgetsPage::CreateSlider()
{
    int flags = 0;

    bool isVert = m_chkVert->GetValue();
    if ( isVert )
        flags |= wxSL_VERTICAL;
    else
        flags |= wxSL_HORIZONTAL;

    if ( m_chkLabels->GetValue() )
    {
        flags |= wxSL_LABELS;
    }

    if ( m_chkTicks->GetValue() )
    {
        flags |= wxSL_AUTOTICKS;
    }

    switch ( m_radioSides->GetSelection() )
    {
        case StaticSides_Top:
            flags |= wxSL_TOP;
            break;
        case StaticSides_Left:
            flags |= wxSL_LEFT;
            break;
        case StaticSides_Bottom:
            flags |= wxSL_BOTTOM;
            break;
        case StaticSides_Right:
            flags |= wxSL_RIGHT;
            break;
        default:
            wxFAIL_MSG(_T("unexpected radiobox selection"));
            // fall through
    }

    if ( m_chkBothSides->GetValue() )
    {
        flags |= wxSL_BOTH;
    }

    int val = m_min;
    if ( m_slider )
    {
        int valOld = m_slider->GetValue();
        if ( !IsValidValue(valOld) )
        {
            val = valOld;
        }

        m_sizerSlider->Detach( m_slider );

        if ( m_sizerSlider->GetChildren().GetCount() )
        {
            // we have 2 spacers, remove them too
            m_sizerSlider->Remove( 0u );
            m_sizerSlider->Remove( 0u );
        }

        delete m_slider;
    }

    m_slider = new wxSlider(this, SliderPage_Slider,
                            val, m_min, m_max,
                            wxDefaultPosition, wxDefaultSize,
                            flags);

    if ( isVert )
    {
        m_sizerSlider->Add(0, 0, 1);
        m_sizerSlider->Add(m_slider, 0, wxGROW | wxALL, 5);
        m_sizerSlider->Add(0, 0, 1);
    }
    else
    {
        m_sizerSlider->Add(m_slider, 1, wxCENTRE | wxALL, 5);
    }

    if ( m_chkTicks->GetValue() )
    {
        DoSetTickFreq();
    }

    m_sizerSlider->Layout();
}

void SliderWidgetsPage::DoSetTickFreq()
{
    long freq;
    if ( !m_textTickFreq->GetValue().ToLong(&freq) )
    {
        wxLogWarning(_T("Invalid slider tick frequency"));

        return;
    }

    m_slider->SetTickFreq(freq, 0 /* unused */);
}

void SliderWidgetsPage::DoSetThumbLen()
{
    long len;
    if ( !m_textThumbLen->GetValue().ToLong(&len) )
    {
        wxLogWarning(_T("Invalid slider thumb lenght"));

        return;
    }

    m_slider->SetThumbLength(len);
}

// ----------------------------------------------------------------------------
// event handlers
// ----------------------------------------------------------------------------

void SliderWidgetsPage::OnButtonReset(wxCommandEvent& WXUNUSED(event))
{
    Reset();

    CreateSlider();
}

void SliderWidgetsPage::OnButtonSetTickFreq(wxCommandEvent& WXUNUSED(event))
{
    DoSetTickFreq();
}

void SliderWidgetsPage::OnButtonSetThumbLen(wxCommandEvent& WXUNUSED(event))
{
    DoSetThumbLen();
}

void SliderWidgetsPage::OnButtonSetMinAndMax(wxCommandEvent& WXUNUSED(event))
{
    long minNew,
         maxNew = 0; // init to suppress compiler warning
    if ( !m_textMin->GetValue().ToLong(&minNew) ||
         !m_textMax->GetValue().ToLong(&maxNew) ||
         minNew >= maxNew )
    {
        wxLogWarning(_T("Invalid min/max values for the slider."));

        return;
    }

    m_min = minNew;
    m_max = maxNew;

    m_slider->SetRange(minNew, maxNew);
}

void SliderWidgetsPage::OnButtonSetValue(wxCommandEvent& WXUNUSED(event))
{
    long val;
    if ( !m_textValue->GetValue().ToLong(&val) || !IsValidValue(val) )
    {
        wxLogWarning(_T("Invalid slider value."));

        return;
    }

    m_slider->SetValue(val);
}

void SliderWidgetsPage::OnUpdateUIValueButton(wxUpdateUIEvent& event)
{
    long val;
    event.Enable( m_textValue->GetValue().ToLong(&val) && IsValidValue(val) );
}

void SliderWidgetsPage::OnUpdateUITickFreq(wxUpdateUIEvent& event)
{
    long freq;
    event.Enable( m_chkTicks->GetValue() &&
                  m_textTickFreq->GetValue().ToLong(&freq) &&
                  (freq > 0) && (freq <= m_max - m_min) );
}

void SliderWidgetsPage::OnUpdateUIThumbLen(wxUpdateUIEvent& event)
{
    long val;
    event.Enable( m_textThumbLen->GetValue().ToLong(&val));
}

void SliderWidgetsPage::OnUpdateUIMinMaxButton(wxUpdateUIEvent& event)
{
    long mn, mx;
    event.Enable( m_textMin->GetValue().ToLong(&mn) &&
                  m_textMax->GetValue().ToLong(&mx) &&
                  mn < mx);
}

void SliderWidgetsPage::OnUpdateUIResetButton(wxUpdateUIEvent& event)
{
    event.Enable( m_chkVert->GetValue() ||
                  !m_chkTicks->GetValue() ||
                  !m_chkLabels->GetValue() ||
                  m_chkBothSides->GetValue() );
}

void SliderWidgetsPage::OnCheckOrRadioBox(wxCommandEvent& event)
{
    CreateSlider();
}

void SliderWidgetsPage::OnUpdateUICurValueText(wxUpdateUIEvent& event)
{
    event.SetText( wxString::Format(_T("%d"), m_slider->GetValue()) );
}

void SliderWidgetsPage::OnUpdateUIRadioSides(wxUpdateUIEvent& event)
{
    event.Enable( m_chkLabels->GetValue() || m_chkTicks->GetValue() );
}

void SliderWidgetsPage::OnUpdateUIBothSides(wxUpdateUIEvent& event)
{
#if defined(__WIN95__) || defined(__WXUNIVERSAL__)
    event.Enable( m_chkTicks->GetValue() );
#else
    event.Enable( FALSE );
#endif // defined(__WIN95__) || defined(__WXUNIVERSAL__)
}

void SliderWidgetsPage::OnSlider(wxScrollEvent& event)
{
    wxASSERT_MSG( event.GetInt() == m_slider->GetValue(),
                  wxT("slider value should be the same") );

    wxEventType eventType = event.GetEventType();

    /*
        This array takes the EXACT order of the declarations in
        include/wx/event.h
        (section "wxScrollBar and wxSlider event identifiers")
    */
    static const wxChar *eventNames[] =
    {
        wxT("wxEVT_SCROLL_TOP"),
        wxT("wxEVT_SCROLL_BOTTOM"),
        wxT("wxEVT_SCROLL_LINEUP"),
        wxT("wxEVT_SCROLL_LINEDOWN"),
        wxT("wxEVT_SCROLL_PAGEUP"),
        wxT("wxEVT_SCROLL_PAGEDOWN"),
        wxT("wxEVT_SCROLL_THUMBTRACK"),
        wxT("wxEVT_SCROLL_THUMBRELEASE"),
        wxT("wxEVT_SCROLL_ENDSCROLL")
    };

    int index = eventType - wxEVT_SCROLL_TOP;

    /*
        If this assert is triggered, there is an unknown slider event which
        should be added to the above eventNames array.
    */
    wxASSERT_MSG(index >= 0 && (size_t)index < WXSIZEOF(eventNames),
                 wxT("Unknown slider event") );


    static int s_numSliderEvents = 0;

    wxLogMessage(wxT("Slider event #%d: %s (pos = %d)"),
                 s_numSliderEvents++,
                 eventNames[index],
                 event.GetPosition());
}

