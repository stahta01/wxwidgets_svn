# This file was created automatically by SWIG.
# Don't modify this file, modify the SWIG interface instead.

import _calendar

import misc
import core
wx = core 
CAL_SUNDAY_FIRST = _calendar.CAL_SUNDAY_FIRST
CAL_MONDAY_FIRST = _calendar.CAL_MONDAY_FIRST
CAL_SHOW_HOLIDAYS = _calendar.CAL_SHOW_HOLIDAYS
CAL_NO_YEAR_CHANGE = _calendar.CAL_NO_YEAR_CHANGE
CAL_NO_MONTH_CHANGE = _calendar.CAL_NO_MONTH_CHANGE
CAL_SEQUENTIAL_MONTH_SELECTION = _calendar.CAL_SEQUENTIAL_MONTH_SELECTION
CAL_SHOW_SURROUNDING_WEEKS = _calendar.CAL_SHOW_SURROUNDING_WEEKS
CAL_HITTEST_NOWHERE = _calendar.CAL_HITTEST_NOWHERE
CAL_HITTEST_HEADER = _calendar.CAL_HITTEST_HEADER
CAL_HITTEST_DAY = _calendar.CAL_HITTEST_DAY
CAL_HITTEST_INCMONTH = _calendar.CAL_HITTEST_INCMONTH
CAL_HITTEST_DECMONTH = _calendar.CAL_HITTEST_DECMONTH
CAL_HITTEST_SURROUNDING_WEEK = _calendar.CAL_HITTEST_SURROUNDING_WEEK
CAL_BORDER_NONE = _calendar.CAL_BORDER_NONE
CAL_BORDER_SQUARE = _calendar.CAL_BORDER_SQUARE
CAL_BORDER_ROUND = _calendar.CAL_BORDER_ROUND
class CalendarDateAttr(object):
    """
    A set of customization attributes for a calendar date, which can be used to
    control the look of the Calendar object.
    """
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxCalendarDateAttr instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """
        __init__(Colour colText=wxNullColour, Colour colBack=wxNullColour, 
            Colour colBorder=wxNullColour, Font font=wxNullFont, 
            int border=CAL_BORDER_NONE) -> CalendarDateAttr

        Create a CalendarDateAttr.
        """
        newobj = _calendar.new_CalendarDateAttr(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
    def SetTextColour(*args, **kwargs):
        """SetTextColour(Colour colText)"""
        return _calendar.CalendarDateAttr_SetTextColour(*args, **kwargs)

    def SetBackgroundColour(*args, **kwargs):
        """SetBackgroundColour(Colour colBack)"""
        return _calendar.CalendarDateAttr_SetBackgroundColour(*args, **kwargs)

    def SetBorderColour(*args, **kwargs):
        """SetBorderColour(Colour col)"""
        return _calendar.CalendarDateAttr_SetBorderColour(*args, **kwargs)

    def SetFont(*args, **kwargs):
        """SetFont(Font font)"""
        return _calendar.CalendarDateAttr_SetFont(*args, **kwargs)

    def SetBorder(*args, **kwargs):
        """SetBorder(int border)"""
        return _calendar.CalendarDateAttr_SetBorder(*args, **kwargs)

    def SetHoliday(*args, **kwargs):
        """SetHoliday(bool holiday)"""
        return _calendar.CalendarDateAttr_SetHoliday(*args, **kwargs)

    def HasTextColour(*args, **kwargs):
        """HasTextColour() -> bool"""
        return _calendar.CalendarDateAttr_HasTextColour(*args, **kwargs)

    def HasBackgroundColour(*args, **kwargs):
        """HasBackgroundColour() -> bool"""
        return _calendar.CalendarDateAttr_HasBackgroundColour(*args, **kwargs)

    def HasBorderColour(*args, **kwargs):
        """HasBorderColour() -> bool"""
        return _calendar.CalendarDateAttr_HasBorderColour(*args, **kwargs)

    def HasFont(*args, **kwargs):
        """HasFont() -> bool"""
        return _calendar.CalendarDateAttr_HasFont(*args, **kwargs)

    def HasBorder(*args, **kwargs):
        """HasBorder() -> bool"""
        return _calendar.CalendarDateAttr_HasBorder(*args, **kwargs)

    def IsHoliday(*args, **kwargs):
        """IsHoliday() -> bool"""
        return _calendar.CalendarDateAttr_IsHoliday(*args, **kwargs)

    def GetTextColour(*args, **kwargs):
        """GetTextColour() -> Colour"""
        return _calendar.CalendarDateAttr_GetTextColour(*args, **kwargs)

    def GetBackgroundColour(*args, **kwargs):
        """GetBackgroundColour() -> Colour"""
        return _calendar.CalendarDateAttr_GetBackgroundColour(*args, **kwargs)

    def GetBorderColour(*args, **kwargs):
        """GetBorderColour() -> Colour"""
        return _calendar.CalendarDateAttr_GetBorderColour(*args, **kwargs)

    def GetFont(*args, **kwargs):
        """GetFont() -> Font"""
        return _calendar.CalendarDateAttr_GetFont(*args, **kwargs)

    def GetBorder(*args, **kwargs):
        """GetBorder() -> int"""
        return _calendar.CalendarDateAttr_GetBorder(*args, **kwargs)


class CalendarDateAttrPtr(CalendarDateAttr):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CalendarDateAttr
_calendar.CalendarDateAttr_swigregister(CalendarDateAttrPtr)

class CalendarEvent(core.CommandEvent):
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxCalendarEvent instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """__init__(CalendarCtrl cal, wxEventType type) -> CalendarEvent"""
        newobj = _calendar.new_CalendarEvent(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
    def GetDate(*args, **kwargs):
        """GetDate() -> DateTime"""
        return _calendar.CalendarEvent_GetDate(*args, **kwargs)

    def SetDate(*args, **kwargs):
        """SetDate(DateTime date)"""
        return _calendar.CalendarEvent_SetDate(*args, **kwargs)

    def SetWeekDay(*args, **kwargs):
        """SetWeekDay(int wd)"""
        return _calendar.CalendarEvent_SetWeekDay(*args, **kwargs)

    def GetWeekDay(*args, **kwargs):
        """GetWeekDay() -> int"""
        return _calendar.CalendarEvent_GetWeekDay(*args, **kwargs)


class CalendarEventPtr(CalendarEvent):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CalendarEvent
_calendar.CalendarEvent_swigregister(CalendarEventPtr)

wxEVT_CALENDAR_DOUBLECLICKED = _calendar.wxEVT_CALENDAR_DOUBLECLICKED
wxEVT_CALENDAR_SEL_CHANGED = _calendar.wxEVT_CALENDAR_SEL_CHANGED
wxEVT_CALENDAR_DAY_CHANGED = _calendar.wxEVT_CALENDAR_DAY_CHANGED
wxEVT_CALENDAR_MONTH_CHANGED = _calendar.wxEVT_CALENDAR_MONTH_CHANGED
wxEVT_CALENDAR_YEAR_CHANGED = _calendar.wxEVT_CALENDAR_YEAR_CHANGED
wxEVT_CALENDAR_WEEKDAY_CLICKED = _calendar.wxEVT_CALENDAR_WEEKDAY_CLICKED
EVT_CALENDAR =                 wx.PyEventBinder( wxEVT_CALENDAR_DOUBLECLICKED, 1)
EVT_CALENDAR_SEL_CHANGED =     wx.PyEventBinder( wxEVT_CALENDAR_SEL_CHANGED, 1)
EVT_CALENDAR_DAY =             wx.PyEventBinder( wxEVT_CALENDAR_DAY_CHANGED, 1)
EVT_CALENDAR_MONTH =           wx.PyEventBinder( wxEVT_CALENDAR_MONTH_CHANGED, 1)
EVT_CALENDAR_YEAR =            wx.PyEventBinder( wxEVT_CALENDAR_YEAR_CHANGED, 1)
EVT_CALENDAR_WEEKDAY_CLICKED = wx.PyEventBinder( wxEVT_CALENDAR_WEEKDAY_CLICKED, 1)

class CalendarCtrl(core.Control):
    """The calendar control allows the user to pick a date interactively."""
    def __repr__(self):
        return "<%s.%s; proxy of C++ wxCalendarCtrl instance at %s>" % (self.__class__.__module__, self.__class__.__name__, self.this,)
    def __init__(self, *args, **kwargs):
        """
        __init__(Window parent, int id, DateTime date=DefaultDateTime, 
            Point pos=DefaultPosition, Size size=DefaultSize, 
            long style=wxCAL_SHOW_HOLIDAYS|wxWANTS_CHARS, 
            String name=CalendarNameStr) -> CalendarCtrl

        Create and show a calendar control.
        """
        newobj = _calendar.new_CalendarCtrl(*args, **kwargs)
        self.this = newobj.this
        self.thisown = 1
        del newobj.thisown
        self._setOORInfo(self)

    def Create(*args, **kwargs):
        """
        Create(Window parent, int id, DateTime date=DefaultDateTime, 
            Point pos=DefaultPosition, Size size=DefaultSize, 
            long style=wxCAL_SHOW_HOLIDAYS|wxWANTS_CHARS, 
            String name=CalendarNameStr) -> bool

        Acutally create the GUI portion of the CalendarCtrl for 2-phase creation.
        """
        return _calendar.CalendarCtrl_Create(*args, **kwargs)

    def SetDate(*args, **kwargs):
        """
        SetDate(DateTime date)

        Sets the current date.
        """
        return _calendar.CalendarCtrl_SetDate(*args, **kwargs)

    def GetDate(*args, **kwargs):
        """
        GetDate() -> DateTime

        Gets the currently selected date.
        """
        return _calendar.CalendarCtrl_GetDate(*args, **kwargs)

    def SetLowerDateLimit(*args, **kwargs):
        """
        SetLowerDateLimit(DateTime date=DefaultDateTime) -> bool

        set the range in which selection can occur
        """
        return _calendar.CalendarCtrl_SetLowerDateLimit(*args, **kwargs)

    def SetUpperDateLimit(*args, **kwargs):
        """
        SetUpperDateLimit(DateTime date=DefaultDateTime) -> bool

        set the range in which selection can occur
        """
        return _calendar.CalendarCtrl_SetUpperDateLimit(*args, **kwargs)

    def GetLowerDateLimit(*args, **kwargs):
        """
        GetLowerDateLimit() -> DateTime

        get the range in which selection can occur
        """
        return _calendar.CalendarCtrl_GetLowerDateLimit(*args, **kwargs)

    def GetUpperDateLimit(*args, **kwargs):
        """
        GetUpperDateLimit() -> DateTime

        get the range in which selection can occur
        """
        return _calendar.CalendarCtrl_GetUpperDateLimit(*args, **kwargs)

    def SetDateRange(*args, **kwargs):
        """
        SetDateRange(DateTime lowerdate=DefaultDateTime, DateTime upperdate=DefaultDateTime) -> bool

        set the range in which selection can occur
        """
        return _calendar.CalendarCtrl_SetDateRange(*args, **kwargs)

    def EnableYearChange(*args, **kwargs):
        """
        EnableYearChange(bool enable=True)

        This function should be used instead of changing CAL_NO_YEAR_CHANGE
        style bit directly. It allows or disallows the user to change the year
        interactively.
        """
        return _calendar.CalendarCtrl_EnableYearChange(*args, **kwargs)

    def EnableMonthChange(*args, **kwargs):
        """
        EnableMonthChange(bool enable=True)

        This function should be used instead of changing CAL_NO_MONTH_CHANGE style
        bit. It allows or disallows the user to change the month interactively. Note
        that if the month can not be changed, the year can not be changed either.
        """
        return _calendar.CalendarCtrl_EnableMonthChange(*args, **kwargs)

    def EnableHolidayDisplay(*args, **kwargs):
        """
        EnableHolidayDisplay(bool display=True)

        This function should be used instead of changing CAL_SHOW_HOLIDAYS style
        bit directly. It enables or disables the special highlighting of the holidays.
        """
        return _calendar.CalendarCtrl_EnableHolidayDisplay(*args, **kwargs)

    def SetHeaderColours(*args, **kwargs):
        """
        SetHeaderColours(Colour colFg, Colour colBg)

        header colours are used for painting the weekdays at the top
        """
        return _calendar.CalendarCtrl_SetHeaderColours(*args, **kwargs)

    def GetHeaderColourFg(*args, **kwargs):
        """
        GetHeaderColourFg() -> Colour

        header colours are used for painting the weekdays at the top
        """
        return _calendar.CalendarCtrl_GetHeaderColourFg(*args, **kwargs)

    def GetHeaderColourBg(*args, **kwargs):
        """
        GetHeaderColourBg() -> Colour

        header colours are used for painting the weekdays at the top
        """
        return _calendar.CalendarCtrl_GetHeaderColourBg(*args, **kwargs)

    def SetHighlightColours(*args, **kwargs):
        """
        SetHighlightColours(Colour colFg, Colour colBg)

        highlight colour is used for the currently selected date
        """
        return _calendar.CalendarCtrl_SetHighlightColours(*args, **kwargs)

    def GetHighlightColourFg(*args, **kwargs):
        """
        GetHighlightColourFg() -> Colour

        highlight colour is used for the currently selected date
        """
        return _calendar.CalendarCtrl_GetHighlightColourFg(*args, **kwargs)

    def GetHighlightColourBg(*args, **kwargs):
        """
        GetHighlightColourBg() -> Colour

        highlight colour is used for the currently selected date
        """
        return _calendar.CalendarCtrl_GetHighlightColourBg(*args, **kwargs)

    def SetHolidayColours(*args, **kwargs):
        """
        SetHolidayColours(Colour colFg, Colour colBg)

        holiday colour is used for the holidays (if CAL_SHOW_HOLIDAYS style is used)
        """
        return _calendar.CalendarCtrl_SetHolidayColours(*args, **kwargs)

    def GetHolidayColourFg(*args, **kwargs):
        """
        GetHolidayColourFg() -> Colour

        holiday colour is used for the holidays (if CAL_SHOW_HOLIDAYS style is used)
        """
        return _calendar.CalendarCtrl_GetHolidayColourFg(*args, **kwargs)

    def GetHolidayColourBg(*args, **kwargs):
        """
        GetHolidayColourBg() -> Colour

        holiday colour is used for the holidays (if CAL_SHOW_HOLIDAYS style is used)
        """
        return _calendar.CalendarCtrl_GetHolidayColourBg(*args, **kwargs)

    def GetAttr(*args, **kwargs):
        """
        GetAttr(size_t day) -> CalendarDateAttr

        Returns the attribute for the given date (should be in the range 1...31).
        The returned value may be None
        """
        return _calendar.CalendarCtrl_GetAttr(*args, **kwargs)

    def SetAttr(*args, **kwargs):
        """
        SetAttr(size_t day, CalendarDateAttr attr)

        Associates the attribute with the specified date (in the range 1...31).
        If the attribute passed is None, the items attribute is cleared.
        """
        return _calendar.CalendarCtrl_SetAttr(*args, **kwargs)

    def SetHoliday(*args, **kwargs):
        """
        SetHoliday(size_t day)

        Marks the specified day as being a holiday in the current month.
        """
        return _calendar.CalendarCtrl_SetHoliday(*args, **kwargs)

    def ResetAttr(*args, **kwargs):
        """
        ResetAttr(size_t day)

        Clears any attributes associated with the given day (in the range 1...31).
        """
        return _calendar.CalendarCtrl_ResetAttr(*args, **kwargs)

    def HitTest(*args, **kwargs):
        """
        HitTest(Point pos) -> (result, date, weekday)

        Returns 3-tuple with information about the given position on the calendar
        control.  The first value of the tuple is a result code and determines the
        validity of the remaining two values.  The result codes are:

            CAL_HITTEST_NOWHERE:    hit outside of anything
            CAL_HITTEST_HEADER:     hit on the header, weekday is valid
            CAL_HITTEST_DAY:        hit on a day in the calendar, date is set.

        """
        return _calendar.CalendarCtrl_HitTest(*args, **kwargs)

    def GetMonthControl(*args, **kwargs):
        """
        GetMonthControl() -> Control

        get the currently shown control for month
        """
        return _calendar.CalendarCtrl_GetMonthControl(*args, **kwargs)

    def GetYearControl(*args, **kwargs):
        """
        GetYearControl() -> Control

        get the currently shown control for year
        """
        return _calendar.CalendarCtrl_GetYearControl(*args, **kwargs)


class CalendarCtrlPtr(CalendarCtrl):
    def __init__(self, this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = CalendarCtrl
_calendar.CalendarCtrl_swigregister(CalendarCtrlPtr)
cvar = _calendar.cvar
CalendarNameStr = cvar.CalendarNameStr

def PreCalendarCtrl(*args, **kwargs):
    """
    PreCalendarCtrl() -> CalendarCtrl

    Precreate a CalendarCtrl for 2-phase creation.
    """
    val = _calendar.new_PreCalendarCtrl(*args, **kwargs)
    val.thisown = 1
    return val


