## This file reverse renames symbols in the wx package to give
## them their wx prefix again, for backwards compatibility.
##
## Generated by ./distrib/build_renamers.py

# This silly stuff here is so the wxPython.wx module doesn't conflict
# with the wx package.  We need to import modules from the wx package
# here, then we'll put the wxPython.wx entry back in sys.modules.
import sys
_wx = None
if sys.modules.has_key('wxPython.wx'):
    _wx = sys.modules['wxPython.wx']
    del sys.modules['wxPython.wx']

import wx.calendar

sys.modules['wxPython.wx'] = _wx
del sys, _wx


# Now assign all the reverse-renamed names:
wxCAL_SUNDAY_FIRST = wx.calendar.CAL_SUNDAY_FIRST
wxCAL_MONDAY_FIRST = wx.calendar.CAL_MONDAY_FIRST
wxCAL_SHOW_HOLIDAYS = wx.calendar.CAL_SHOW_HOLIDAYS
wxCAL_NO_YEAR_CHANGE = wx.calendar.CAL_NO_YEAR_CHANGE
wxCAL_NO_MONTH_CHANGE = wx.calendar.CAL_NO_MONTH_CHANGE
wxCAL_SEQUENTIAL_MONTH_SELECTION = wx.calendar.CAL_SEQUENTIAL_MONTH_SELECTION
wxCAL_SHOW_SURROUNDING_WEEKS = wx.calendar.CAL_SHOW_SURROUNDING_WEEKS
wxCAL_HITTEST_NOWHERE = wx.calendar.CAL_HITTEST_NOWHERE
wxCAL_HITTEST_HEADER = wx.calendar.CAL_HITTEST_HEADER
wxCAL_HITTEST_DAY = wx.calendar.CAL_HITTEST_DAY
wxCAL_HITTEST_INCMONTH = wx.calendar.CAL_HITTEST_INCMONTH
wxCAL_HITTEST_DECMONTH = wx.calendar.CAL_HITTEST_DECMONTH
wxCAL_HITTEST_SURROUNDING_WEEK = wx.calendar.CAL_HITTEST_SURROUNDING_WEEK
wxCAL_BORDER_NONE = wx.calendar.CAL_BORDER_NONE
wxCAL_BORDER_SQUARE = wx.calendar.CAL_BORDER_SQUARE
wxCAL_BORDER_ROUND = wx.calendar.CAL_BORDER_ROUND
wxCalendarDateAttr = wx.calendar.CalendarDateAttr
wxCalendarDateAttrPtr = wx.calendar.CalendarDateAttrPtr
wxCalendarDateAttrBorder = wx.calendar.CalendarDateAttrBorder
wxCalendarEvent = wx.calendar.CalendarEvent
wxCalendarEventPtr = wx.calendar.CalendarEventPtr
wxEVT_CALENDAR_DOUBLECLICKED = wx.calendar.wxEVT_CALENDAR_DOUBLECLICKED
wxEVT_CALENDAR_SEL_CHANGED = wx.calendar.wxEVT_CALENDAR_SEL_CHANGED
wxEVT_CALENDAR_DAY_CHANGED = wx.calendar.wxEVT_CALENDAR_DAY_CHANGED
wxEVT_CALENDAR_MONTH_CHANGED = wx.calendar.wxEVT_CALENDAR_MONTH_CHANGED
wxEVT_CALENDAR_YEAR_CHANGED = wx.calendar.wxEVT_CALENDAR_YEAR_CHANGED
wxEVT_CALENDAR_WEEKDAY_CLICKED = wx.calendar.wxEVT_CALENDAR_WEEKDAY_CLICKED
wxCalendarNameStr = wx.calendar.CalendarNameStr
wxCalendarCtrl = wx.calendar.CalendarCtrl
wxCalendarCtrlPtr = wx.calendar.CalendarCtrlPtr
wxPreCalendarCtrl = wx.calendar.PreCalendarCtrl


d = globals()
for k, v in wx.calendar.__dict__.iteritems():
    if k.startswith('EVT'):
        d[k] = v
del d, k, v



