/////////////////////////////////////////////////////////////////////////////
// Name:        wx/datetime.h
// Purpose:     declarations of time/date related classes (wxDateTime,
//              wxTimeSpan)
// Author:      Vadim Zeitlin
// Modified by:
// Created:     10.02.99
// RCS-ID:      $Id$
// Copyright:   (c) 1998 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_TIME_H
#define _WX_TIME_H

#ifdef __GNUG__
    #pragma interface "datetime.h"
#endif

#include <time.h>

#include "wx/longlong.h"

class WXDLLEXPORT wxDateTime;
class WXDLLEXPORT wxTimeSpan;
class WXDLLEXPORT wxDateSpan;

/*
 * TODO Well, everything :-)
 *
 * 1. Time zones with minutes (make wxTimeZone a class)
 * 2. getdate() function like under Solaris
 * 3. text conversion for wxDateSpan
 */

/*
  The three classes declared in this header represent:

  1. An absolute moment in the time (wxDateTime)
  2. A difference between two moments in the time, positive or negative
     (wxTimeSpan)
  3. A logical difference between two dates expressed in
     years/months/weeks/days (wxDateSpan)

  The following arithmetic operations are permitted (all others are not):

  addition
  --------

  wxDateTime + wxTimeSpan = wxDateTime
  wxDateTime + wxDateSpan = wxDateTime
  wxTimeSpan + wxTimeSpan = wxTimeSpan
  wxDateSpan + wxDateSpan = wxDateSpan

  substraction
  ------------
  wxDateTime - wxDateTime = wxTimeSpan
  wxTimeSpan - wxTimeSpan = wxTimeSpan
  wxDateSpan - wxDateSpan = wxDateSpan

  multiplication
  --------------
  wxTimeSpan * number = wxTimeSpan
  wxDateSpan * number = wxDateSpan

  unitary minus
  -------------
  -wxTimeSpan = wxTimeSpan
  -wxDateSpan = wxDateSpan
*/

// ----------------------------------------------------------------------------
// This class represents an absolute moment in the time
// ----------------------------------------------------------------------------
class WXDLLEXPORT wxDateTime
{
public:
    // types
    // ------------------------------------------------------------------------

        // a small unsigned integer type for storing things like seconds, days
        // of the week, &c. It should be at least short (i.e. not char) to
        // contain the number of milliseconds - it may also be 'int' because
        // there is no size penalty associated with it in our code, we don't
        // store any data in this format
    typedef unsigned short wxDateTime_t;

        // the timezones
    enum TimeZone
    {
        // the time in the current time zone
        Local,

        // zones from GMT (= Greenwhich Mean Time): they're guaranteed to be
        // consequent numbers, so writing something like `GMT0 + offset' is
        // safe if abs(offset) <= 12

        // underscore stands for minus
        GMT_12, GMT_11, GMT_10, GMT_9, GMT_8, GMT_7,
        GMT_6, GMT_5, GMT_4, GMT_3, GMT_2, GMT_1,
        GMT0,
        GMT1, GMT2, GMT3, GMT4, GMT5, GMT6,
        GMT7, GMT8, GMT9, GMT10, GMT11, GMT12,
        // Note that GMT12 and GMT_12 are not the same: there is a difference
        // of exactly one day between them

        // Universal Coordinated Time
        UTC = GMT0

        // TODO add symbolic names for TZ (EST, MET, ...)?
    };

        // the calendar systems we know about: notice that it's valid (for
        // this classes purpose anyhow) to work with any of these calendars
        // even with the dates before the historical appearance of the
        // calendar
    enum Calendar
    {
        Gregorian,  // current calendar
        Julian      // calendar in use since -45 until the 1582 (or later)

        // TODO Hebrew, Chinese, Maya, ... (just kidding) (or then may be not?)
    };

        // these values only are used to identify the different dates of
        // adoption of the Gregorian calendar (see IsGregorian())
        //
        // All data and comments taken verbatim from "The Calendar FAQ (v 2.0)"
        // by Claus T�ndering, http://www.pip.dknet.dk/~c-t/calendar.html
        // except for the comments "we take".
        //
        // Symbol "->" should be read as "was followed by" in the comments
        // which follow.
    enum GregorianAdoption
    {
        Gr_Unknown,    // no data for this country or it's too uncertain to use
        Gr_Standard,   // on the day 0 of Gregorian calendar: 15 Oct 1582

        Gr_Alaska,             // Oct 1867 when Alaska became part of the USA
        Gr_Albania,            // Dec 1912

        Gr_Austria = Gr_Unknown,    // Different regions on different dates
        Gr_Austria_Brixen,          // 5 Oct 1583 -> 16 Oct 1583
        Gr_Austria_Salzburg = Gr_Austria_Brixen,
        Gr_Austria_Tyrol = Gr_Austria_Brixen,
        Gr_Austria_Carinthia,       // 14 Dec 1583 -> 25 Dec 1583
        Gr_Austria_Styria = Gr_Austria_Carinthia,

        Gr_Belgium,            // Then part of the Netherlands

        Gr_Bulgaria = Gr_Unknown, // Unknown precisely (from 1915 to 1920)
        Gr_Bulgaria_1,         //      18 Mar 1916 -> 1 Apr 1916
        Gr_Bulgaria_2,         //      31 Mar 1916 -> 14 Apr 1916
        Gr_Bulgaria_3,         //      3 Sep 1920 -> 17 Sep 1920

        Gr_Canada = Gr_Unknown,   // Different regions followed the changes in
                               // Great Britain or France

        Gr_China = Gr_Unknown,    // Different authorities say:
        Gr_China_1,            //      18 Dec 1911 -> 1 Jan 1912
        Gr_China_2,            //      18 Dec 1928 -> 1 Jan 1929

        Gr_Czechoslovakia,     // (Bohemia and Moravia) 6 Jan 1584 -> 17 Jan 1584
        Gr_Denmark,            // (including Norway) 18 Feb 1700 -> 1 Mar 1700
        Gr_Egypt,              // 1875
        Gr_Estonia,            // 1918
        Gr_Finland,            // Then part of Sweden

        Gr_France,             // 9 Dec 1582 -> 20 Dec 1582
        Gr_France_Alsace,      //      4 Feb 1682 -> 16 Feb 1682
        Gr_France_Lorraine,    //      16 Feb 1760 -> 28 Feb 1760
        Gr_France_Strasbourg,  // February 1682

        Gr_Germany = Gr_Unknown,  // Different states on different dates:
        Gr_Germany_Catholic,   //      1583-1585 (we take 1584)
        Gr_Germany_Prussia,    //      22 Aug 1610 -> 2 Sep 1610
        Gr_Germany_Protestant, //      18 Feb 1700 -> 1 Mar 1700

        Gr_GreatBritain,       // 2 Sep 1752 -> 14 Sep 1752 (use 'cal(1)')

        Gr_Greece,             // 9 Mar 1924 -> 23 Mar 1924
        Gr_Hungary,            // 21 Oct 1587 -> 1 Nov 1587
        Gr_Ireland = Gr_GreatBritain,
        Gr_Italy = Gr_Standard,

        Gr_Japan = Gr_Unknown,    // Different authorities say:
        Gr_Japan_1,            //      19 Dec 1872 -> 1 Jan 1873
        Gr_Japan_2,            //      19 Dec 1892 -> 1 Jan 1893
        Gr_Japan_3,            //      18 Dec 1918 -> 1 Jan 1919

        Gr_Latvia,             // 1915-1918 (we take 1915)
        Gr_Lithuania,          // 1915
        Gr_Luxemburg,          // 14 Dec 1582 -> 25 Dec 1582
        Gr_Netherlands = Gr_Belgium, // (including Belgium) 1 Jan 1583

        // this is too weird to take into account: the Gregorian calendar was
        // introduced twice in Groningen, first time 28 Feb 1583 was followed
        // by 11 Mar 1583, then it has gone back to Julian in the summer of
        // 1584 and then 13 Dec 1700 -> 12 Jan 1701 - which is
        // the date we take here
        Gr_Netherlands_Groningen,  // 13 Dec 1700 -> 12 Jan 1701
        Gr_Netherlands_Gelderland, // 30 Jun 1700 -> 12 Jul 1700
        Gr_Netherlands_Utrecht,    // (and Overijssel) 30 Nov 1700->12 Dec 1700
        Gr_Netherlands_Friesland,  // (and Drenthe) 31 Dec 1700 -> 12 Jan 1701

        Gr_Norway = Gr_Denmark,       // Then part of Denmark
        Gr_Poland = Gr_Standard,
        Gr_Portugal = Gr_Standard,
        Gr_Romania,                // 31 Mar 1919 -> 14 Apr 1919
        Gr_Russia,                 // 31 Jan 1918 -> 14 Feb 1918
        Gr_Scotland = Gr_GreatBritain,
        Gr_Spain = Gr_Standard,

        // Sweden has a curious history. Sweden decided to make a gradual
        // change from the Julian to the Gregorian calendar. By dropping every
        // leap year from 1700 through 1740 the eleven superfluous days would
        // be omitted and from 1 Mar 1740 they would be in sync with the
        // Gregorian calendar. (But in the meantime they would be in sync with
        // nobody!)
        //
        // So 1700 (which should have been a leap year in the Julian calendar)
        // was not a leap year in Sweden. However, by mistake 1704 and 1708
        // became leap years. This left Sweden out of synchronisation with
        // both the Julian and the Gregorian world, so they decided to go back
        // to the Julian calendar. In order to do this, they inserted an extra
        // day in 1712, making that year a double leap year! So in 1712,
        // February had 30 days in Sweden.
        //
        // Later, in 1753, Sweden changed to the Gregorian calendar by
        // dropping 11 days like everyone else.
        Gr_Sweden = Gr_Finland,       // 17 Feb 1753 -> 1 Mar 1753

        Gr_Switzerland = Gr_Unknown,// Different cantons used different dates
        Gr_Switzerland_Catholic,    //      1583, 1584 or 1597 (we take 1584)
        Gr_Switzerland_Protestant,  //      31 Dec 1700 -> 12 Jan 1701

        Gr_Turkey,                 // 1 Jan 1927
        Gr_USA = Gr_GreatBritain,
        Gr_Wales = Gr_GreatBritain,
        Gr_Yugoslavia              // 1919
    };

        // the country parameter is used so far for calculating the start and
        // the end of DST period and for deciding whether the date is a work
        // day or not
        //
        // TODO move this to intl.h
    enum Country
    {
        Country_Unknown, // no special information for this country
        Country_Default, // set the default country with SetCountry() method

        // TODO add more countries (for this we must know about DST and/or
        //      holidays for this country)
        France,
        USA
    };

        // symbolic names for the months
    enum Month
    {
        Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec, Inv_Month
    };

        // symbolic names for the weekdays
    enum WeekDay
    {
        Sun, Mon, Tue, Wed, Thu, Fri, Sat, Inv_WeekDay
    };

        // invalid value for the year
    enum Year
    {
        Inv_Year = INT_MIN
    };

    // static methods
    // ------------------------------------------------------------------------

        // set the current country
    static void SetCountry(Country country);
        // get the current country
    static Country GetCountry();

        // return the current year
    static int GetCurrentYear(Calendar cal = Gregorian);

        // convert the year as returned by wxDateTime::GetYear() to a year
        // suitable for BC/AD notation. The difference is that BC year 1
        // corresponds to the year 0 (while BC year 0 didn't exist) and AD
        // year N is just year N.
    static int ConvertYearToBC(int year);

        // return the current month
    static Month GetCurrentMonth(Calendar cal = Gregorian);

        // returns TRUE if the given year is a leap year in the given calendar
    static bool IsLeapYear(int year, Calendar cal = Gregorian);

        // returns the number of days in this year (356 or 355 typically :-)
    static wxDateTime_t GetNumberOfDays(int year, Calendar cal = Gregorian);

        // get the number of the days in the given month (default value for
        // the year means the current one)
    static wxDateTime_t GetNumberOfDays(Month month,
                                        int year = Inv_Year,
                                        Calendar cal = Gregorian);

        // get the month name in the current locale
    static wxString GetMonthName(Month month);

        // get the weekday name in the current locale
    static wxString GetDayOfWeekName(WeekDay weekday);

        // get the beginning of DST for this year, will return invalid object
        // if no DST applicable in this year. The default value of the
        // parameter means to take the current year.
    static wxDateTime GetBeginDST(int year = Inv_Year);
        // get the end of DST for this year, will return invalid object
        // if no DST applicable in this year. The default value of the
        // parameter means to take the current year.
    static wxDateTime GetEndDST(int year = Inv_Year);

    // constructors: you should test whether the constructor succeeded with
    // IsValid() function. The values Inv_Month and Inv_Year for the
    // parameters mean take current month and/or year values.
    // ------------------------------------------------------------------------

        // default ctor does not initialize the object, use Set()!
    wxDateTime() { }

        // from time_t: seconds since the Epoch 00:00:00 UTC, Jan 1, 1970)
    inline wxDateTime(time_t timet);
        // from broken down time/date
    inline wxDateTime(const struct tm& tm);

        // from separate values for each component, date set to today
    inline wxDateTime(wxDateTime_t hour,
                      wxDateTime_t minute = 0,
                      wxDateTime_t second = 0,
                      wxDateTime_t millisec = 0);
        // from separate values for each component with explicit date
    inline wxDateTime(wxDateTime_t day,
                      Month        month = Inv_Month,
                      int          year = Inv_Year, // 1999, not 99 please!
                      wxDateTime_t hour = 0,
                      wxDateTime_t minute = 0,
                      wxDateTime_t second = 0,
                      wxDateTime_t millisec = 0);

        // default copy ctor ok

        // no dtor

    // assignment operators and Set() functions: all non const methods return
    // the reference to this object. IsValid() should be used to test whether
    // the function succeeded.
    // ------------------------------------------------------------------------

        // set to the current time
    inline wxDateTime& SetToCurrent();

        // set to given time_t value
    inline wxDateTime& Set(time_t timet);

        // set to given broken down time/date
    wxDateTime& Set(const struct tm& tm);

        // set to given time, date = today
    wxDateTime& Set(wxDateTime_t hour,
                    wxDateTime_t minute = 0,
                    wxDateTime_t second = 0,
                    wxDateTime_t millisec = 0);

        // from separate values for each component with explicit date
        // (defaults for month and year are the current values)
    wxDateTime& Set(wxDateTime_t day,
                    Month        month = Inv_Month,
                    int          year = Inv_Year, // 1999, not 99 please!
                    wxDateTime_t hour = 0,
                    wxDateTime_t minute = 0,
                    wxDateTime_t second = 0,
                    wxDateTime_t millisec = 0);

        // resets time to 00:00:00, doesn't change the date
    wxDateTime& ResetTime();

        // the following functions don't change the values of the other
        // fields, i.e. SetMinute() won't change either hour or seconds value

        // set the year
    wxDateTime& SetYear(int year);
        // set the month
    wxDateTime& SetMonth(Month month);
        // set the day of the month
    wxDateTime& SetDay(wxDateTime_t day);
        // set hour
    wxDateTime& SetHour(wxDateTime_t hour);
        // set minute
    wxDateTime& SetMinute(wxDateTime_t minute);
        // set second
    wxDateTime& SetSecond(wxDateTime_t second);
        // set millisecond
    wxDateTime& SetMillisecond(wxDateTime_t millisecond);

        // assignment operator from time_t
    wxDateTime& operator=(time_t timet) { return Set(timet); }

        // assignment operator from broken down time/date
    wxDateTime& operator=(const struct tm& tm) { return Set(tm); }

        // default assignment operator is ok

    // calendar calculations (functions which set the date only leave the time
    // unchanged, e.g. don't explictly zero it)
    // ------------------------------------------------------------------------

        // set to Nth occurence of given weekday in the given month of the
        // given year (time is set to 0), return TRUE on success and FALSE on
        // failure. n may be positive (1..5) or negative to count from the end
        // of the month (see helper function SetToLastWeekDay())
    bool SetToWeekDay(WeekDay weekday,
                      int n = 1,
                      wxDateTime_t month = Inv_Month,
                      int year = Inv_Year);

        // sets to the last weekday in the given month, year
    inline bool SetToLastWeekDay(WeekDay weekday,
                                 wxDateTime_t month = Inv_Month,
                                 int year = Inv_Year);

        // sets the date to the given day of the given week in the year,
        // returns TRUE on success and FALSE if given date doesn't exist (e.g.
        // numWeek is > 53)
    bool SetToTheWeek(wxDateTime_t numWeek, WeekDay weekday = Mon);

        // get the century (19 for 1999, 20 for 2000 and -5 for 492 BC)
    int GetCentury() const;

        // The definitions below were taken verbatim from
        //
        //      http://www.capecod.net/~pbaum/date/date0.htm
        //
        // (Peter Baum's home page)
        //
        // definition: The Julian Day Number, Julian Day, or JD of a
        // particular instant of time is the number of days and fractions of a
        // day since 12 hours Universal Time (Greenwich mean noon) on January
        // 1 of the year -4712, where the year is given in the Julian
        // proleptic calendar. The idea of using this reference date was
        // originally proposed by Joseph Scalizer in 1582 to count years but
        // it was modified by 19th century astronomers to count days. One
        // could have equivalently defined the reference time to be noon of
        // November 24, -4713 if were understood that Gregorian calendar rules
        // were applied. Julian days are Julian Day Numbers and are not to be
        // confused with Julian dates.
        //
        // definition: The Rata Die number is a date specified as the number
        // of days relative to a base date of December 31 of the year 0. Thus
        // January 1 of the year 1 is Rata Die day 1.

        // get the Julian Day number
    long GetJulianDay() const;

        // get the Rata Die number
    long GetRataDie() const;

        // TODO algorithms for calculating some important dates, such as
        //      religious holidays (Easter...) or moon/solar eclipses? Some
        //      algorithms can be found in the calendar FAQ

    // accessors: many of them take the timezone parameter which indicates the
    // timezone for which to make the calculations and the default value means
    // to do it for the current timezone of this machine (even if the function
    // only operates with the date it's necessary because a date may wrap as
    // result of timezone shift)
    // ------------------------------------------------------------------------

        // is the date valid (FALSE for uninitialized objects as well as after
        // the functions which failed to convert the date to supported range)
    inline bool IsValid() const { return this != &ms_InvDateTime; }

        // get the year (returns Inv_Year if date is invalid)
    int GetYear(TimeZone zone = Local) const;
        // get the month (Inv_Month if date is invalid)
    Month GetMonth(TimeZone zone = Local) const;
        // get the month day (in 1..31 range, 0 if date is invalid)
    wxDateTime_t GetDay(TimeZone zone = Local) const;
        // get the day of the week (Inv_WeekDay if date is invalid)
    WeekDay GetDayOfWeek(TimeZone zone = Local) const;
        // get the hour of the day
    wxDateTime_t GetHour(TimeZone zone = Local) const;
        // get the minute
    wxDateTime_t GetMinute(TimeZone zone = Local) const;
        // get the second
    wxDateTime_t GetSecond(TimeZone zone = Local) const;
        // get milliseconds
    wxDateTime_t GetMillisecond(TimeZone zone = Local) const;

        // get the day since the year start (1..366, 0 if date is invalid)
    wxDateTime_t GetDayOfYear() const;
        // get the week number since the year start (1..52, 0 if date is
        // invalid)
    wxDateTime_t GetWeekOfYear() const;

        // is this date a work day? This depends on a country, of course,
        // because the holidays are different in different countries
    bool IsWorkDay(Country country = Current, TimeZone zone = local) const;

        // is this date later than Gregorian calendar introduction for the
        // given country (see enum GregorianAdoption)?
        //
        // NB: this function shouldn't be considered as absolute authoiruty in
        //     the matter. Besides, for some countries the exact date of
        //     adoption of the Gregorian calendar is simply unknown.
    bool IsGregorianDate(GregorianAdoption country = Gregorian_Standard) const;

        // is daylight savings time in effect at this moment?
        //
        // Return value is > 0 if DST is in effect, 0 if it is not and -1 if
        // the information is not available (this is compatible with ANSI C)
    int IsDST(Country country = Country_Default, TimeZone zone = local) const;

    // comparison (see also functions below for operator versions)
    // ------------------------------------------------------------------------

        // returns TRUE if the date is strictly earlier than the given one
    bool IsEarlierThan(const wxDateTime& datetime) const;

        // returns TRUE if the date is strictly later than the given one
    bool IsLaterThan(const wxDateTime& datetime) const;

        // returns TRUE if the date is strictly in the given range
    bool IsStrictlyBetween(const wxDateTime& t1, const wxDateTime& t2) const;
        // returns TRUE if the date is in the given range
    bool IsBetween(const wxDateTime& t1, const wxDateTime& t2) const;

    // date operations: for the non-const methods, the return value is this
    // object itself (see also functions below for operator versions)
    // ------------------------------------------------------------------------

        // add a time span (positive or negative)
    wxDateTime& Add(const wxTimeSpan& diff);
        // add a time span (positive or negative)
    wxDateTime& operator+=(const wxTimeSpan& diff) { return Add(diff); }

        // substract a time span (positive or negative)
    inline wxDateTime& Substract(const wxTimeSpan& diff);
        // substract a time span (positive or negative)
    inline wxDateTime& operator-=(const wxTimeSpan& diff);

        // add a date span (positive or negative)
    wxDateTime& Add(const wxDateSpan& diff);
        // add a date span (positive or negative)
    wxDateTime& operator+=(const wxDateSpan& diff) { return Add(diff); }

        // substract a date span (positive or negative)
    inline wxDateTime& Substract(const wxDateSpan& diff);
        // substract a date span (positive or negative)
    inline wxDateTime& operator-=(const wxDateSpan& diff);

        // substract a date (may result in positive or negative time span)
    wxTimeSpan Substract(const wxDateTime& datetime) const;
        // substract a date (may result in positive or negative time span)
    wxTimeSpan operator-(const wxDateTime& datetime) const;

    // conversion to/from text: all conversions from text return TRUE on
    // success or FALSE if the date is malformed/out of supported range
    // ------------------------------------------------------------------------

        // parse a string in RFC 822 format (found e.g. in mail headers and
        // having the form "Wed, 10 Feb 1999 19:07:07 +0100")
    bool ParseRfc822Date(const wxString& date);
        // parse a date/time in the given format (see strptime(3))
    bool ParseFormat(const wxString& date, const char *format = "%c");
        // parse a string containing the date/time in "free" format, this
        // function will try to make an educated guess at the string contents
        // (and return FALSE if it fails)
    bool ParseDateTime(const wxString& date);

        // this function accepts strftime()-like format string (default
        // argument corresponds to the preferred date and time representation
        // for the current locale) and returns the string containing the
        // resulting text representation
    wxString Format(const char *format = "%c") const;
        // preferred date representation for the current locale
    wxString FormatDate() const { return Format("%x"); }
        // preferred time representation for the current locale
    wxString FormatTime() const { return Format("%X"); }

    // implementation
    // ------------------------------------------------------------------------

        // get the internal representation
    inline wxLongLong GetValue() const;

private:
    // a helper function to get the current time_t
    static inline time_t GetTimeNow() { return time((time_t *)NULL); }

    // the current country - as it's the same for all program objects (unless
    // it runs on a _really_ big load balancing system), this is a static
    // member: see SetCountry() and GetCountry()
    static Country ms_country;

    // the internal representation of the time is the amount of milliseconds
    // elapsed since the origin which is set by convention to the UNIX/C epoch
    // value: the midnight of on January 1, 1970 (UTC)
    wxLongLong m_time;

    // this constant is used to transform a time_t value to the internal
    // representation, as time_t is in seconds and we use milliseconds it's
    // fixed to 1000
    static const int TIME_T_FACTOR;

    // invalid wxDateTime object - returned by all functions which return
    // "wxDateTime &" on failure
    static wxDateTime ms_InvDateTime;
};

// ----------------------------------------------------------------------------
// This class contains a difference between 2 wxDateTime values, so it makes
// sense to add it to wxDateTime and it is the result of substraction of 2
// objects of that class. See also wxDateSpan.
// ----------------------------------------------------------------------------
class WXDLLEXPORT wxTimeSpan
{
public:
    // constructors
    // ------------------------------------------------------------------------

        // default ctor constructs the 0 time span
    wxTimeSpan() { }

        // from separate values for each component, date set to 0 (hours are
        // not restricted to 0..24 range, neither are minutes, seconds or
        // milliseconds)
    wxTimeSpan(int hours,
               int minutes = 0,
               int seconds = 0,
               int milliseconds = 0);
        // from separate values for each component with explicit date (none of
        // the parameters isn't restricted to any range)
    wxTimeSpan(int years,
               int months,
               int days,
               int hours = 0,
               int minutes = 0,
               int seconds = 0,
               int milliseconds = 0);

        // from internal representation
    wxTimeSpan(wxLongLong diff) : m_diff(diff) { }

        // default copy ctor is ok

        // no dtor

    // arithmetics with time spans
    // ------------------------------------------------------------------------

        // add two timespans together
    inline wxTimeSpan& Add(const wxTimeSpan& diff);
        // add two timespans together
    wxTimeSpan& operator+=(const wxTimeSpan& diff) { return Add(diff); }
        // add two timespans together
    inline wxTimeSpan operator+(const wxTimeSpan& ts1, const wxTimeSpan& ts2);

        // substract another timespan
    inline wxTimeSpan& Substract(const wxTimeSpan& diff);
        // substract another timespan
    wxTimeSpan& operator-=(const wxTimeSpan& diff) { return Substract(diff); }
        // substract two timespans
    inline wxTimeSpan operator-(const wxTimeSpan& ts1, const wxTimeSpan& ts2);

        // multiply timespan by a scalar
    inline wxTimeSpan& Multiply(int n);
        // multiply timespan by a scalar
    wxTimeSpan& operator*=(int n) { return Multiply(n); }
        // multiply timespan by a scalar
    inline wxTimeSpan operator*(int n) const;

        // return this timespan with inversed sign
    wxTimeSpan Negate() { return wxTimeSpan(-GetValue()); }
        // negate the value of the timespan
    wxTimeSpan& Neg() { m_diff = -GetValue(); return *this; }
        // negate the value of the timespan
    wxTimeSpan& operator-() { return Neg(); }

        // return the absolute value of the timespan: does _not_ modify the
        // object
    inline wxTimeSpan Abs() const;

        // there is intentionally no division because we don't want to
        // introduce rounding errors in time calculations

    // comparaison (see also operator versions below)
    // ------------------------------------------------------------------------

        // is the timespan null?
    bool IsNull() const { return m_diff == 0; }
        // returns true if the timespan is null
    bool operator!() const { return !IsNull(); }

        // is the timespan positive?
    bool IsPositive() const { return m_diff > 0; }

        // is the timespan negative?
    bool IsNegative() const { return m_diff < 0; }

        // are two timespans equal?
    inline bool IsEqualTo(const wxTimeSpan& ts) const;
        // compare two timestamps: works with the absolute values, i.e. -2
        // hours is longer than 1 hour. Also, it will return FALSE if the
        // timespans are equal in absolute value.
    inline bool IsLongerThan(const wxTimeSpan& ts) const;
        // compare two timestamps: works with the absolute values, i.e. 1
        // hour is shorter than -2 hours. Also, it will return FALSE if the
        // timespans are equal in absolute value.
    bool IsShorterThan(const wxTimeSpan& t) const { return !IsLongerThan(t); }

    // breaking into years, ..., days, ..., seconds: all these functions
    // behave like GetYears() which returns 1 for the timespan of 1 year and 1
    // day, but 0 (and not -1) for the negative timespan of 1 year without 1
    // day. IOW, (ts - wxTimeSpan(ts.GetYears())).GetYears() is always 0.
    // ------------------------------------------------------------------------

        // get the max number of years in this timespan
    inline int GetYears() const;
        // get the max number of months in this timespan
    inline int GetMonths() const;
        // get the max number of weeks in this timespan
    inline int GetWeeks() const;
        // get the max number of days in this timespan
    inline int GetDays() const;
        // get the max number of hours in this timespan
    inline int GetHours() const;
        // get the max number of minutes in this timespan
    inline int GetMinutes() const;
        // get the max number of seconds in this timespan
    inline int GetSeconds() const;
        // get the number of milliseconds in this timespan
    int GetMilliseconds() const { return m_diff; }

    // conversion to text
    // ------------------------------------------------------------------------

        // this function accepts strftime()-like format string (default
        // argument corresponds to the preferred date and time representation
        // for the current locale) and returns the string containing the
        // resulting text representation. Notice that only some of format
        // specifiers valid for wxDateTime are valid for wxTimeSpan: hours,
        // minutes and seconds make sense, but not "PM/AM" string for example.
    wxString Format(const char *format = "%c") const;
        // preferred date representation for the current locale
    wxString FormatDate() const { return Format("%x"); }
        // preferred time representation for the current locale
    wxString FormatTime() const { return Format("%X"); }

    // implementation
    // ------------------------------------------------------------------------

        // get the internal representation
    wxLongLong GetValue() const { return m_diff; }

private:
    // the (signed) time span in milliseconds
    wxLongLong m_diff;
};

// ----------------------------------------------------------------------------
// This class is a "logical time span" and is useful for implementing program
// logic for such things as "add one month to the date" which, in general,
// doesn't mean to add 60*60*24*31 seconds to it, but to take the same date
// the next month (to understand that this is indeed different consider adding
// one month to Feb, 15 - we want to get Mar, 15, of course).
//
// When adding a month to the date, all lesser components (days, hours, ...)
// won't be changed.
//
// wxDateSpan can be either positive or negative. They may be
// multiplied by scalars which multiply all deltas by the scalar: i.e. 2*(1
// month and 1 day) is 2 months and 2 days. They can be added together and
// with wxDateTime or wxTimeSpan, but the type of result is different for each
// case.
//
// Beware about weeks: if you specify both weeks and days, the total number of
// days added will be 7*weeks + days! See also GetTotalDays() function.
//
// Finally, notice that for adding hours, minutes &c you don't need this
// class: wxTimeSpan will do the job because there are no subtleties
// associated with those.
// ----------------------------------------------------------------------------
class WXDLLEXPORT wxDateSpan
{
public:
    // constructors
    // ------------------------------------------------------------------------

        // this many years/months/weeks/days
    wxDateSpan(int years, int months, int weeks, int days);

        // default copy ctor is ok

        // no dtor

    // accessors (all SetXXX() return the (modified) wxDateSpan object)
    // ------------------------------------------------------------------------

        // set number of years
    wxDateSpan& SetYears(int n) { m_years = n; return this; }
        // set number of months
    wxDateSpan& SetMonths(int n) { m_months = n; return this; }
        // set number of weeks
    wxDateSpan& SetWeeks(int n) { m_weeks = n; return this; }
        // set number of days
    wxDateSpan& SetDays(int n) { m_days = n; return this; }

        // get number of years
    int GetYears() const { return m_years; }
        // get number of months
    int GetMonths() const { return m_months; }
        // get number of weeks
    int GetWeeks() const { return m_weeks; }
        // get number of days
    int GetDays() const { return m_days; }
        // returns 7*GetWeeks() + GetDays()
    int GetTotalDays() const { return 7*m_weeks + m_days; }

    // arithmetics
    // ------------------------------------------------------------------------

        // add another wxDateSpan to us
    inline wxDateSpan& Add(const wxDateSpan& other);
        // add another wxDateSpan to us
    inline wxDateSpan& operator+=(const wxDateSpan& other);

        // substract another wxDateSpan from us
    inline wxDateSpan& Substract(const wxDateSpan& other);
        // substract another wxDateSpan from us
    inline wxDateSpan& operator-=(const wxDateSpan& other);

        // return a copy of this time span with changed sign
    inline wxDateSpan Negate() const;
        // inverse the sign of this timespan
    inline wxDateSpan& Neg();
        // inverse the sign of this timespan
    wxDateSpan& operator-() { return Neg(); }

        // multiply all components by a (signed) number
    inline wxDateSpan& operator*=(int factor);

private:
    int m_years,
        m_months,
        m_weeks,
        m_days;
};

WXDLLEXPORT_DATA(extern wxDateSpan) wxYear;
WXDLLEXPORT_DATA(extern wxDateSpan) wxMonth;
WXDLLEXPORT_DATA(extern wxDateSpan) wxWeek;
WXDLLEXPORT_DATA(extern wxDateSpan) wxDay;

// ============================================================================
// binary operators
// ============================================================================

// ----------------------------------------------------------------------------
// wxDateTime operators
// ----------------------------------------------------------------------------

inline bool WXDLLEXPORT operator<(const wxDateTime &t1, const wxDateTime &t2)
{
    wxASSERT_MSG( t1.IsValid() && t2.IsValid(), "invalid wxDateTime" );

    return t1.GetValue() < t2.GetValue()
}

inline bool WXDLLEXPORT operator<=(const wxDateTime &t1, const wxDateTime &t2)
{
    wxASSERT_MSG( t1.IsValid() && t2.IsValid(), "invalid wxDateTime" );

    return t1.GetValue() <= t2.GetValue()
}

inline bool WXDLLEXPORT operator>(const wxDateTime &t1, const wxDateTime &t2)
{
    wxASSERT_MSG( t1.IsValid() && t2.IsValid(), "invalid wxDateTime" );

    return t1.GetValue() > t2.GetValue()
}

inline bool WXDLLEXPORT operator>=(const wxDateTime &t1, const wxDateTime &t2)
{
    wxASSERT_MSG( t1.IsValid() && t2.IsValid(), "invalid wxDateTime" );

    return t1.GetValue() >= t2.GetValue()
}

inline bool WXDLLEXPORT operator==(const wxDateTime &t1, const wxDateTime &t2)
{
    wxASSERT_MSG( t1.IsValid() && t2.IsValid(), "invalid wxDateTime" );

    return t1.GetValue() == t2.GetValue()
}

inline bool WXDLLEXPORT operator!=(const wxDateTime &t1, const wxDateTime &t2)
{
    wxASSERT_MSG( t1.IsValid() && t2.IsValid(), "invalid wxDateTime" );

    return t1.GetValue() != t2.GetValue()
}

inline wxTimeSpan WXDLLEXPORT operator-(const wxDateTime &t1,
                                        const wxDateTime &t2)
{
    wxASSERT_MSG( t1.IsValid() && t2.IsValid(), "invalid wxDateTime" );

    return wxTimeSpan(t1.GetValue() - t2.GetValue());
}

// ----------------------------------------------------------------------------
// wxTimeSpan operators
// ----------------------------------------------------------------------------

inline bool WXDLLEXPORT operator<(const wxTimeSpan &t1, const wxTimeSpan &t2)
{
    return t1.GetValue() < t2.GetValue()
}

inline bool WXDLLEXPORT operator<=(const wxTimeSpan &t1, const wxTimeSpan &t2)
{
    return t1.GetValue() <= t2.GetValue()
}

inline bool WXDLLEXPORT operator>(const wxTimeSpan &t1, const wxTimeSpan &t2)
{
    return t1.GetValue() > t2.GetValue()
}

inline bool WXDLLEXPORT operator>=(const wxTimeSpan &t1, const wxTimeSpan &t2)
{
    return t1.GetValue() >= t2.GetValue()
}

inline bool WXDLLEXPORT operator==(const wxTimeSpan &t1, const wxTimeSpan &t2)
{
    return t1.GetValue() == t2.GetValue()
}

inline bool WXDLLEXPORT operator!=(const wxTimeSpan &t1, const wxTimeSpan &t2)
{
    return t1.GetValue() != t2.GetValue();
}

// ----------------------------------------------------------------------------
// wxDateSpan
// ----------------------------------------------------------------------------

inline WXDLLEXPORT wxDateSpan operator+(const wxDateSpan& rt1,
                                        const wxDateSpan& rt2)
{
    return wxDateSpan(rt1.m_years + rt2.m_years,
                      rt1.m_months + rt2.m_months,
                      rt1.m_weeks + rt2.m_weeks,
                      rt1.m_days + rt2.m_days);
}

// ============================================================================
// inline functions implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxDateTime
// ----------------------------------------------------------------------------

const int wxDateTime::TIME_T_FACTOR = 1000;

wxDateTime& Set(time_t timet)
{
    m_time = timet * wxDateTime::TIME_T_FACTOR;

    return *this;
}

wxDateTime& SetToCurrent()
{
    return Set(GetTimeNow());
}

wxDateTime::wxDateTime(time_t timet)
{
    Set(timet);
}

wxDateTime::wxDateTime(const struct tm& tm)
{
    Set(tm);
}

wxDateTime::wxDateTime(wxDateTime_t hour,
                       wxDateTime_t minute,
                       wxDateTime_t second,
                       wxDateTime_t millisec)
{
    Set(hour, minute, second, millisec);
}

wxDateTime::wxDateTime(wxDateTime_t day,
                       Month        month,
                       int          year,
                       wxDateTime_t hour,
                       wxDateTime_t minute,
                       wxDateTime_t second,
                       wxDateTime_t millisec)
{
    Set(day, month, year, hour, minute, second, millisec);
}

wxLongLong wxDateTime::GetValue() const
{
    wxASSERT_MSG( IsValid(), "invalid wxDateTime");

    return m_time;
}

bool wxDateTime::SetToLastWeekDay(WeekDay weekday,
                                  wxDateTime_t month,
                                  int year)
{
    SetToWeekDay(weekday, -1, month, year);
}

wxDateTime& wxDateTime::Substract(const wxTimeSpan& diff)
{
    return Add(diff.Negate());
}

wxDateTime& wxDateTime::operator-=(const wxTimeSpan& diff)
{
    return Add(diff.Negate());
}

wxDateTime& wxDateTime::Substract(const wxDateSpan& diff)
{
    return Add(diff.Negate());
}

wxDateTime& wxDateTime::operator-=(const wxDateSpan& diff)
{
    return Add(diff.Negate());
}

wxTimeSpan wxDateTime::operator-(const wxDateTime& datetime) const
{
    return Substract(datetime);
}

// ----------------------------------------------------------------------------
// wxTimeSpan
// ----------------------------------------------------------------------------

wxTimeSpan& wxTimeSpan::Add(const wxTimeSpan& diff)
{
    m_diff += diff.GetValue();

    return *this;
}

wxTimeSpan& wxTimeSpan::Substract(const wxTimeSpan& diff)
{
    m_diff -= diff.GetValue();

    return *this;
}

wxTimeSpan& wxTimeSpan::Multiply(int n)
{
    m_diff *= n;

    return *this;
}

wxTimeSpan wxTimeSpan::operator*(int n) const
{
    wxTimeSpan result(*this);
    result.Multiply(n);

    return result;
}

wxTimeSpan wxTimeSpan::Abs() const
{
    return wxTimeSpan(GetValue().Abs());
}

bool wxTimeSpan::IsEqualTo(const wxTimeSpan& ts) const
{
    return GetValue() == ts.GetValue();
}

bool wxTimeSpan::IsLongerThan(const wxTimeSpan& ts) const
{
    return Abs() > ts.Abs();
}

// ----------------------------------------------------------------------------
// wxDateSpan
// ----------------------------------------------------------------------------

wxDateSpan&
wxDateSpan::operator+=(const wxDateSpan& other)
{
    m_years += other.m_years;
    m_months += other.m_months;
    m_weeks += other.m_weeks;
    m_days += other.m_days;

    return *this;
}

wxDateSpan& wxDateSpan::operator*=(int factor)
{
    m_years *= m_years;
    m_months *= m_months;
    m_weeks *= m_weeks;
    m_days *= m_days;

    return *this;
}

wxDateSpan Negate() const
{
    return wxDateSpan(-m_years, -m_months, -m_weeks, -m_days);
}

wxDateSpan& Neg()
{
    m_years = -m_years;
    m_months = -m_months;
    m_weeks = -m_weeks;
    m_days = -m_days;

    return *this;
}

#endif // _WX_TIME_H
