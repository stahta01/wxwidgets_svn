/////////////////////////////////////////////////////////////////////////////
// Name:        calendar.i
// Purpose:     SWIG definitions for the wxCalendarCtrl
//
// Author:      Robin Dunn
//
// Created:     23-May-2000
// RCS-ID:      $Id$
// Copyright:   (c) 2000 by Total Control Software
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

%module calendar


%{
#include "wx/wxPython/wxPython.h"
#include "wx/wxPython/pyclasses.h"

#include <wx/calctrl.h>
%}

//----------------------------------------------------------------------

%import misc.i
%pythoncode { wx = core }

%include _calendar_rename.i

//---------------------------------------------------------------------------

enum {
    wxCAL_SUNDAY_FIRST,
    wxCAL_MONDAY_FIRST,
    wxCAL_SHOW_HOLIDAYS,
    wxCAL_NO_YEAR_CHANGE,
    wxCAL_NO_MONTH_CHANGE,
    wxCAL_SEQUENTIAL_MONTH_SELECTION,
    wxCAL_SHOW_SURROUNDING_WEEKS,
};


enum wxCalendarHitTestResult
{
    wxCAL_HITTEST_NOWHERE,      // outside of anything
    wxCAL_HITTEST_HEADER,       // on the header (weekdays)
    wxCAL_HITTEST_DAY,          // on a day in the calendar
    wxCAL_HITTEST_INCMONTH,
    wxCAL_HITTEST_DECMONTH,
    wxCAL_HITTEST_SURROUNDING_WEEK
};

// border types for a date
enum wxCalendarDateBorder
{
    wxCAL_BORDER_NONE,          // no border (default)
    wxCAL_BORDER_SQUARE,        // a rectangular border
    wxCAL_BORDER_ROUND          // a round border
};

//---------------------------------------------------------------------------


class wxCalendarDateAttr
{
public:
    // ctors
    wxCalendarDateAttr(const wxColour& colText,
                       const wxColour& colBack = wxNullColour,
                       const wxColour& colBorder = wxNullColour,
                       const wxFont& font = wxNullFont,
                       wxCalendarDateBorder border = wxCAL_BORDER_NONE);

    %name(CalendarDateAttrBorder)
        wxCalendarDateAttr(wxCalendarDateBorder border,
                           const wxColour& colBorder = wxNullColour);

    // setters
    void SetTextColour(const wxColour& colText);
    void SetBackgroundColour(const wxColour& colBack);
    void SetBorderColour(const wxColour& col);
    void SetFont(const wxFont& font);
    void SetBorder(wxCalendarDateBorder border);
    void SetHoliday(bool holiday);

    // accessors
    bool HasTextColour() const;
    bool HasBackgroundColour() const;
    bool HasBorderColour() const;
    bool HasFont() const;
    bool HasBorder() const;

    bool IsHoliday() const;

    wxColour GetTextColour() const;
    wxColour GetBackgroundColour() const;
    wxColour GetBorderColour() const;
    wxFont GetFont() const;
    wxCalendarDateBorder GetBorder() const;
};

//---------------------------------------------------------------------------

class wxCalendarCtrl;

class wxCalendarEvent : public wxCommandEvent
{
public:
    wxCalendarEvent(wxCalendarCtrl *cal, wxEventType type);

    const wxDateTime& GetDate() const;
    wxDateTime::WeekDay GetWeekDay() const;

};


%constant wxEventType wxEVT_CALENDAR_DOUBLECLICKED;
%constant wxEventType wxEVT_CALENDAR_SEL_CHANGED;
%constant wxEventType wxEVT_CALENDAR_DAY_CHANGED;
%constant wxEventType wxEVT_CALENDAR_MONTH_CHANGED;
%constant wxEventType wxEVT_CALENDAR_YEAR_CHANGED;
%constant wxEventType wxEVT_CALENDAR_WEEKDAY_CLICKED;


%pythoncode {
EVT_CALENDAR =                 wx.PyEventBinder( wxEVT_CALENDAR_DOUBLECLICKED, 1)
EVT_CALENDAR_SEL_CHANGED =     wx.PyEventBinder( wxEVT_CALENDAR_SEL_CHANGED, 1)
EVT_CALENDAR_DAY =             wx.PyEventBinder( wxEVT_CALENDAR_DAY_CHANGED, 1)
EVT_CALENDAR_MONTH =           wx.PyEventBinder( wxEVT_CALENDAR_MONTH_CHANGED, 1)
EVT_CALENDAR_YEAR =            wx.PyEventBinder( wxEVT_CALENDAR_YEAR_CHANGED, 1)
EVT_CALENDAR_WEEKDAY_CLICKED = wx.PyEventBinder( wxEVT_CALENDAR_WEEKDAY_CLICKED, 1)
}


//---------------------------------------------------------------------------

%{
    DECLARE_DEF_STRING(CalendarNameStr);
%}



class wxCalendarCtrl : public wxControl
{
public:
    %addtofunc wxCalendarCtrl      "self._setOORInfo(self)"
    %addtofunc wxCalendarCtrl()    ""

    wxCalendarCtrl(wxWindow *parent,
                   wxWindowID id,
                   const wxDateTime& date = wxDefaultDateTime,
                   const wxPoint& pos = wxDefaultPosition,
                   const wxSize& size = wxDefaultSize,
                   long style = wxCAL_SHOW_HOLIDAYS | wxWANTS_CHARS,
                   const wxString& name = wxPyCalendarNameStr);
    %name(PreCalendarCtrl)wxCalendarCtrl();

    bool Create(wxWindow *parent,
                wxWindowID id,
                const wxDateTime& date = wxDefaultDateTime,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxCAL_SHOW_HOLIDAYS | wxWANTS_CHARS,
                const wxString& name = wxPyCalendarNameStr);


    // set/get the current date
    // ------------------------

    void SetDate(const wxDateTime& date);
    const wxDateTime& GetDate() const;

    // set/get the range in which selection can occur
    // ---------------------------------------------

    bool SetLowerDateLimit(const wxDateTime& date = wxDefaultDateTime);
    const wxDateTime& GetLowerDateLimit() const;
    bool SetUpperDateLimit(const wxDateTime& date = wxDefaultDateTime);
    const wxDateTime& GetUpperDateLimit() const;

    bool SetDateRange(const wxDateTime& lowerdate = wxDefaultDateTime,
                      const wxDateTime& upperdate = wxDefaultDateTime);


    // calendar mode
    // -------------

    // some calendar styles can't be changed after the control creation by
    // just using SetWindowStyle() and Refresh() and the functions below
    // should be used instead for them

    // corresponds to wxCAL_NO_YEAR_CHANGE bit
    void EnableYearChange(bool enable = TRUE);

    // corresponds to wxCAL_NO_MONTH_CHANGE bit
    void EnableMonthChange(bool enable = TRUE);

    // corresponds to wxCAL_SHOW_HOLIDAYS bit
    void EnableHolidayDisplay(bool display = TRUE);

    // customization
    // -------------

    // header colours are used for painting the weekdays at the top
    void SetHeaderColours(const wxColour& colFg, const wxColour& colBg);
    wxColour GetHeaderColourFg() const;
    wxColour GetHeaderColourBg() const;

    // highlight colour is used for the currently selected date
    void SetHighlightColours(const wxColour& colFg, const wxColour& colBg);
    wxColour GetHighlightColourFg() const;
    wxColour GetHighlightColourBg() const;

    // holiday colour is used for the holidays (if style & wxCAL_SHOW_HOLIDAYS)
    void SetHolidayColours(const wxColour& colFg, const wxColour& colBg);
    wxColour GetHolidayColourFg() const;
    wxColour GetHolidayColourBg() const;

    // an item without custom attributes is drawn with the default colours and
    // font and without border, setting custom attributes allows to modify this
    //
    // the day parameter should be in 1..31 range, for days 29, 30, 31 the
    // corresponding attribute is just unused if there is no such day in the
    // current month

    wxCalendarDateAttr *GetAttr(size_t day) const;
    void SetAttr(size_t day, wxCalendarDateAttr *attr);

    void SetHoliday(size_t day);

    void ResetAttr(size_t day);

    // returns one of wxCAL_HITTEST_XXX constants and fills either date or wd
    // with the corresponding value (none for NOWHERE, the date for DAY and wd
    // for HEADER)
    wxCalendarHitTestResult HitTest(const wxPoint& pos,
                                    wxDateTime *date = NULL,
                                    wxDateTime::WeekDay *wd = NULL);


    bool Enable(bool enable = TRUE);
    bool Show(bool show = TRUE);

};


//---------------------------------------------------------------------------

%init %{
%}

//---------------------------------------------------------------------------

