///////////////////////////////////////////////////////////////////////////////
// Name:        tests/strings/strings.cpp
// Purpose:     wxString unit test
// Author:      Vadim Zeitlin, Wlodzimierz ABX Skiba
// Created:     2004-04-19
// RCS-ID:      $Id$
// Copyright:   (c) 2004 Vadim Zeitlin, Wlodzimierz Skiba
///////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include "testprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif // WX_PRECOMP

// ----------------------------------------------------------------------------
// test class
// ----------------------------------------------------------------------------

class StringTestCase : public CppUnit::TestCase
{
public:
    StringTestCase();

private:
    CPPUNIT_TEST_SUITE( StringTestCase );
        CPPUNIT_TEST( String );
        CPPUNIT_TEST( PChar );
        CPPUNIT_TEST( Format );
        CPPUNIT_TEST( Constructors );
        CPPUNIT_TEST( StaticConstructors );
        CPPUNIT_TEST( Extraction );
        CPPUNIT_TEST( Trim );
        CPPUNIT_TEST( Find );
        CPPUNIT_TEST( Replace );
        CPPUNIT_TEST( Match );
        CPPUNIT_TEST( CaseChanges );
        CPPUNIT_TEST( Compare );
        CPPUNIT_TEST( CompareNoCase );
        CPPUNIT_TEST( Contains );
        CPPUNIT_TEST( ToLong );
        CPPUNIT_TEST( ToULong );
#ifdef wxLongLong_t
        CPPUNIT_TEST( ToLongLong );
        CPPUNIT_TEST( ToULongLong );
#endif // wxLongLong_t
        CPPUNIT_TEST( ToDouble );
        CPPUNIT_TEST( StringBuf );
        CPPUNIT_TEST( UTF8Buf );
        CPPUNIT_TEST( CStrDataTernaryOperator );
        CPPUNIT_TEST( CStrDataOperators );
        CPPUNIT_TEST( CStrDataImplicitConversion );
        CPPUNIT_TEST( ExplicitConversion );
    CPPUNIT_TEST_SUITE_END();

    void String();
    void PChar();
    void Format();
    void Constructors();
    void StaticConstructors();
    void Extraction();
    void Trim();
    void Find();
    void Replace();
    void Match();
    void CaseChanges();
    void Compare();
    void CompareNoCase();
    void Contains();
    void ToLong();
    void ToULong();
#ifdef wxLongLong_t
    void ToLongLong();
    void ToULongLong();
#endif // wxLongLong_t
    void ToDouble();
    void StringBuf();
    void UTF8Buf();
    void CStrDataTernaryOperator();
    void DoCStrDataTernaryOperator(bool cond);
    void CStrDataOperators();
    void CStrDataImplicitConversion();
    void ExplicitConversion();

    DECLARE_NO_COPY_CLASS(StringTestCase)
};

// register in the unnamed registry so that these tests are run by default
CPPUNIT_TEST_SUITE_REGISTRATION( StringTestCase );

// also include in it's own registry so that these tests can be run alone
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( StringTestCase, "StringTestCase" );

StringTestCase::StringTestCase()
{
}

void StringTestCase::String()
{
    wxString a, b, c;

    a.reserve (128);
    b.reserve (128);
    c.reserve (128);

    for (int i = 0; i < 2; ++i)
    {
        a = _T("Hello");
        b = _T(" world");
        c = _T("! How'ya doin'?");
        a += b;
        a += c;
        c = _T("Hello world! What's up?");
        CPPUNIT_ASSERT( c != a );
    }
}

void StringTestCase::PChar()
{
    wxChar a [128];
    wxChar b [128];
    wxChar c [128];

    for (int i = 0; i < 2; ++i)
    {
        wxStrcpy (a, _T("Hello"));
        wxStrcpy (b, _T(" world"));
        wxStrcpy (c, _T("! How'ya doin'?"));
        wxStrcat (a, b);
        wxStrcat (a, c);
        wxStrcpy (c, _T("Hello world! What's up?"));
        CPPUNIT_ASSERT( wxStrcmp (c, a) != 0 );
    }
}

void StringTestCase::Format()
{
    wxString s1,s2;
    s1.Printf(_T("%03d"), 18);
    CPPUNIT_ASSERT( s1 == wxString::Format(_T("%03d"), 18) );
    s2.Printf(_T("Number 18: %s\n"), s1.c_str());
    CPPUNIT_ASSERT( s2 == wxString::Format(_T("Number 18: %s\n"), s1.c_str()) );

    static const size_t lengths[] = { 1, 512, 1024, 1025, 2048, 4096, 4097 };
    for ( size_t n = 0; n < WXSIZEOF(lengths); n++ )
    {
        const size_t len = lengths[n];

        wxString s(_T('Z'), len);
        CPPUNIT_ASSERT_EQUAL( len, wxString::Format(_T("%s"), s.c_str()).length());
    }
}

void StringTestCase::Constructors()
{
    CPPUNIT_ASSERT_EQUAL( "", wxString('Z', 0) );
    CPPUNIT_ASSERT_EQUAL( "Z", wxString('Z') );
    CPPUNIT_ASSERT_EQUAL( "ZZZZ", wxString('Z', 4) );
    CPPUNIT_ASSERT_EQUAL( "Hell", wxString("Hello", 4) );
    CPPUNIT_ASSERT_EQUAL( "Hello", wxString("Hello", 5) );

#if wxUSE_UNICODE
    CPPUNIT_ASSERT_EQUAL( L"", wxString(L'Z', 0) );
    CPPUNIT_ASSERT_EQUAL( L"Z", wxString(L'Z') );
    CPPUNIT_ASSERT_EQUAL( L"ZZZZ", wxString(L'Z', 4) );
    CPPUNIT_ASSERT_EQUAL( L"Hell", wxString(L"Hello", 4) );
    CPPUNIT_ASSERT_EQUAL( L"Hello", wxString(L"Hello", 5) );
#endif // wxUSE_UNICODE

    static const char *s = "?really!";
    const char *start = wxStrchr(s, 'r');
    const char *end = wxStrchr(s, '!');
    CPPUNIT_ASSERT_EQUAL( "really", wxString(start, end) );

    // test if creating string from NULL C pointer works:
    CPPUNIT_ASSERT_EQUAL( "", wxString((const char *)NULL) );
}

void StringTestCase::StaticConstructors()
{
    CPPUNIT_ASSERT_EQUAL( "", wxString::FromAscii("") );
    CPPUNIT_ASSERT_EQUAL( "", wxString::FromAscii("Hello", 0) );
    CPPUNIT_ASSERT_EQUAL( "Hell", wxString::FromAscii("Hello", 4) );
    CPPUNIT_ASSERT_EQUAL( "Hello", wxString::FromAscii("Hello", 5) );
    CPPUNIT_ASSERT_EQUAL( "Hello", wxString::FromAscii("Hello") );

    // FIXME: this doesn't work currently but should!
    //CPPUNIT_ASSERT_EQUAL( 1, wxString::FromAscii("", 1).length() );


    CPPUNIT_ASSERT_EQUAL( "", wxString::FromUTF8("") );
    CPPUNIT_ASSERT_EQUAL( "", wxString::FromUTF8("Hello", 0) );
    CPPUNIT_ASSERT_EQUAL( "Hell", wxString::FromUTF8("Hello", 4) );
    CPPUNIT_ASSERT_EQUAL( "Hello", wxString::FromUTF8("Hello", 5) );
    CPPUNIT_ASSERT_EQUAL( "Hello", wxString::FromUTF8("Hello") );

    //CPPUNIT_ASSERT_EQUAL( 1, wxString::FromUTF8("", 1).length() );
}

void StringTestCase::Extraction()
{
    wxString s(_T("Hello, world!"));

    CPPUNIT_ASSERT( wxStrcmp( s.c_str() , _T("Hello, world!") ) == 0 );
    CPPUNIT_ASSERT( wxStrcmp( s.Left(5).c_str() , _T("Hello") ) == 0 );
    CPPUNIT_ASSERT( wxStrcmp( s.Right(6).c_str() , _T("world!") ) == 0 );
    CPPUNIT_ASSERT( wxStrcmp( s(3, 5).c_str() , _T("lo, w") ) == 0 );
    CPPUNIT_ASSERT( wxStrcmp( s.Mid(3).c_str() , _T("lo, world!") ) == 0 );
    CPPUNIT_ASSERT( wxStrcmp( s.substr(3, 5).c_str() , _T("lo, w") ) == 0 );
    CPPUNIT_ASSERT( wxStrcmp( s.substr(3).c_str() , _T("lo, world!") ) == 0 );

#if wxUSE_UNICODE
    static const char *germanUTF8 = "Oberfl\303\244che";
    wxString strUnicode(wxString::FromUTF8(germanUTF8));

    CPPUNIT_ASSERT( strUnicode.Mid(0, 10) == strUnicode );
    CPPUNIT_ASSERT( strUnicode.Mid(7, 2) == "ch" );
#endif // wxUSE_UNICODE

    wxString rest;

    #define TEST_STARTS_WITH(prefix, correct_rest, result)                    \
        CPPUNIT_ASSERT_EQUAL(result, s.StartsWith(prefix, &rest));            \
        if ( result )                                                         \
            CPPUNIT_ASSERT_EQUAL(correct_rest, rest)

    TEST_STARTS_WITH( _T("Hello"),           _T(", world!"),      true  );
    TEST_STARTS_WITH( _T("Hello, "),         _T("world!"),        true  );
    TEST_STARTS_WITH( _T("Hello, world!"),   _T(""),              true  );
    TEST_STARTS_WITH( _T("Hello, world!!!"), _T(""),              false );
    TEST_STARTS_WITH( _T(""),                _T("Hello, world!"), true  );
    TEST_STARTS_WITH( _T("Goodbye"),         _T(""),              false );
    TEST_STARTS_WITH( _T("Hi"),              _T(""),              false );

    #undef TEST_STARTS_WITH

    rest = "Hello world";
    CPPUNIT_ASSERT( rest.StartsWith("Hello ", &rest) );
    CPPUNIT_ASSERT_EQUAL("world", rest);

    #define TEST_ENDS_WITH(suffix, correct_rest, result)                      \
        CPPUNIT_ASSERT_EQUAL(result, s.EndsWith(suffix, &rest));              \
        if ( result )                                                         \
            CPPUNIT_ASSERT_EQUAL(correct_rest, rest)

    TEST_ENDS_WITH( _T(""),                 _T("Hello, world!"), true  );
    TEST_ENDS_WITH( _T("!"),                _T("Hello, world"),  true  );
    TEST_ENDS_WITH( _T(", world!"),         _T("Hello"),         true  );
    TEST_ENDS_WITH( _T("ello, world!"),     _T("H"),             true  );
    TEST_ENDS_WITH( _T("Hello, world!"),    _T(""),              true  );
    TEST_ENDS_WITH( _T("very long string"), _T(""),              false );
    TEST_ENDS_WITH( _T("?"),                _T(""),              false );
    TEST_ENDS_WITH( _T("Hello, world"),     _T(""),              false );
    TEST_ENDS_WITH( _T("Gello, world!"),    _T(""),              false );

    #undef TEST_ENDS_WITH
}

void StringTestCase::Trim()
{
    #define TEST_TRIM( str , dir , result )  \
        CPPUNIT_ASSERT( wxString(str).Trim(dir) == result )

    TEST_TRIM( _T("  Test  "),  true, _T("  Test") );
    TEST_TRIM( _T("    "),      true, _T("")       );
    TEST_TRIM( _T(" "),         true, _T("")       );
    TEST_TRIM( _T(""),          true, _T("")       );

    TEST_TRIM( _T("  Test  "),  false, _T("Test  ") );
    TEST_TRIM( _T("    "),      false, _T("")       );
    TEST_TRIM( _T(" "),         false, _T("")       );
    TEST_TRIM( _T(""),          false, _T("")       );

    #undef TEST_TRIM
}

void StringTestCase::Find()
{
    #define TEST_FIND( str , start , result )  \
        CPPUNIT_ASSERT( wxString(str).find(_T("ell"), start) == result );

    TEST_FIND( _T("Well, hello world"),  0, 1              );
    TEST_FIND( _T("Well, hello world"),  6, 7              );
    TEST_FIND( _T("Well, hello world"),  9, wxString::npos );

    #undef TEST_FIND
}

void StringTestCase::Replace()
{
    #define TEST_REPLACE( original , pos , len , replacement , result ) \
        { \
            wxString s = original; \
            s.replace( pos , len , replacement ); \
            CPPUNIT_ASSERT_EQUAL( result, s ); \
        }

    TEST_REPLACE( _T("012-AWORD-XYZ"), 4, 5, _T("BWORD"),  _T("012-BWORD-XYZ") );
    TEST_REPLACE( _T("increase"),      0, 2, _T("de"),     _T("decrease")      );
    TEST_REPLACE( _T("wxWindow"),      8, 0, _T("s"),      _T("wxWindows")     );
    TEST_REPLACE( _T("foobar"),        3, 0, _T("-"),      _T("foo-bar")       );
    TEST_REPLACE( _T("barfoo"),        0, 6, _T("foobar"), _T("foobar")        );


    #define TEST_NULLCHARREPLACE( o , olen, pos , len , replacement , r, rlen ) \
        { \
            wxString s(o,olen); \
            s.replace( pos , len , replacement ); \
            CPPUNIT_ASSERT( s == wxString(r,rlen) ); \
        }

    TEST_NULLCHARREPLACE( _T("null\0char"), 9, 5, 1, _T("d"),
                          _T("null\0dhar"), 9 );

    #define TEST_WXREPLACE( o , olen, olds, news, all, r, rlen ) \
        { \
            wxString s(o,olen); \
            s.Replace( olds, news, all ); \
            CPPUNIT_ASSERT( s == wxString(r,rlen) ); \
        }

    TEST_WXREPLACE( _T("null\0char"), 9, _T("c"), _T("de"), true,
                          _T("null\0dehar"), 10 );

    TEST_WXREPLACE( _T("null\0dehar"), 10, _T("de"), _T("c"), true,
                          _T("null\0char"), 9 );

    #undef TEST_WXREPLACE
    #undef TEST_NULLCHARREPLACE
    #undef TEST_REPLACE
}

void StringTestCase::Match()
{
    #define TEST_MATCH( s1 , s2 , result ) \
        CPPUNIT_ASSERT( wxString(s1).Matches(s2) == result )

    TEST_MATCH( _T("foobar"),    _T("foo*"),   true  );
    TEST_MATCH( _T("foobar"),    _T("*oo*"),   true  );
    TEST_MATCH( _T("foobar"),    _T("*bar"),   true  );
    TEST_MATCH( _T("foobar"),    _T("??????"), true  );
    TEST_MATCH( _T("foobar"),    _T("f??b*"),  true  );
    TEST_MATCH( _T("foobar"),    _T("f?b*"),   false );
    TEST_MATCH( _T("foobar"),    _T("*goo*"),  false );
    TEST_MATCH( _T("foobar"),    _T("*foo"),   false );
    TEST_MATCH( _T("foobarfoo"), _T("*foo"),   true  );
    TEST_MATCH( _T(""),          _T("*"),      true  );
    TEST_MATCH( _T(""),          _T("?"),      false );

    #undef TEST_MATCH
}


void StringTestCase::CaseChanges()
{
    wxString s1(_T("Hello!"));
    wxString s1u(s1);
    wxString s1l(s1);
    s1u.MakeUpper();
    s1l.MakeLower();

    CPPUNIT_ASSERT_EQUAL( _T("HELLO!"), s1u );
    CPPUNIT_ASSERT_EQUAL( _T("hello!"), s1l );

    wxString s2u, s2l;
    s2u.MakeUpper();
    s2l.MakeLower();

    CPPUNIT_ASSERT_EQUAL( "", s2u );
    CPPUNIT_ASSERT_EQUAL( "", s2l );


    wxString s3("good bye");
    CPPUNIT_ASSERT_EQUAL( "Good bye", s3.Capitalize() );
    s3.MakeCapitalized();
    CPPUNIT_ASSERT_EQUAL( "Good bye", s3 );

    CPPUNIT_ASSERT_EQUAL( "Abc", wxString("ABC").Capitalize() );

    CPPUNIT_ASSERT_EQUAL( "", wxString().Capitalize() );
}

void StringTestCase::Compare()
{
    wxString s1 = wxT("AHH");
    wxString eq = wxT("AHH");
    wxString neq1 = wxT("HAH");
    wxString neq2 = wxT("AH");
    wxString neq3 = wxT("AHHH");
    wxString neq4 = wxT("AhH");

    CPPUNIT_ASSERT( s1 == eq );
    CPPUNIT_ASSERT( s1 != neq1 );
    CPPUNIT_ASSERT( s1 != neq2 );
    CPPUNIT_ASSERT( s1 != neq3 );
    CPPUNIT_ASSERT( s1 != neq4 );

    CPPUNIT_ASSERT( s1 == wxT("AHH") );
    CPPUNIT_ASSERT( s1 != wxT("no") );
    CPPUNIT_ASSERT( s1 < wxT("AZ") );
    CPPUNIT_ASSERT( s1 <= wxT("AZ") );
    CPPUNIT_ASSERT( s1 <= wxT("AHH") );
    CPPUNIT_ASSERT( s1 > wxT("AA") );
    CPPUNIT_ASSERT( s1 >= wxT("AA") );
    CPPUNIT_ASSERT( s1 >= wxT("AHH") );

    // test comparison with C strings in Unicode build (must work in ANSI as
    // well, of course):
    CPPUNIT_ASSERT( s1 == "AHH" );
    CPPUNIT_ASSERT( s1 != "no" );
    CPPUNIT_ASSERT( s1 < "AZ" );
    CPPUNIT_ASSERT( s1 <= "AZ" );
    CPPUNIT_ASSERT( s1 <= "AHH" );
    CPPUNIT_ASSERT( s1 > "AA" );
    CPPUNIT_ASSERT( s1 >= "AA" );
    CPPUNIT_ASSERT( s1 >= "AHH" );

//    wxString _s1 = wxT("A\0HH");
//    wxString _eq = wxT("A\0HH");
//    wxString _neq1 = wxT("H\0AH");
//    wxString _neq2 = wxT("A\0H");
//    wxString _neq3 = wxT("A\0HHH");
//    wxString _neq4 = wxT("A\0hH");
    s1.insert(1,1,'\0');
    eq.insert(1,1,'\0');
    neq1.insert(1,1,'\0');
    neq2.insert(1,1,'\0');
    neq3.insert(1,1,'\0');
    neq4.insert(1,1,'\0');

    CPPUNIT_ASSERT( s1 == eq );
    CPPUNIT_ASSERT( s1 != neq1 );
    CPPUNIT_ASSERT( s1 != neq2 );
    CPPUNIT_ASSERT( s1 != neq3 );
    CPPUNIT_ASSERT( s1 != neq4 );
}

void StringTestCase::CompareNoCase()
{
    wxString s1 = wxT("AHH");
    wxString eq = wxT("AHH");
    wxString eq2 = wxT("AhH");
    wxString eq3 = wxT("ahh");
    wxString neq = wxT("HAH");
    wxString neq2 = wxT("AH");
    wxString neq3 = wxT("AHHH");

    #define CPPUNIT_CNCEQ_ASSERT(s1, s2) CPPUNIT_ASSERT( s1.CmpNoCase(s2) == 0)
    #define CPPUNIT_CNCNEQ_ASSERT(s1, s2) CPPUNIT_ASSERT( s1.CmpNoCase(s2) != 0)

    CPPUNIT_CNCEQ_ASSERT( s1, eq );
    CPPUNIT_CNCEQ_ASSERT( s1, eq2 );
    CPPUNIT_CNCEQ_ASSERT( s1, eq3 );

    CPPUNIT_CNCNEQ_ASSERT( s1, neq );
    CPPUNIT_CNCNEQ_ASSERT( s1, neq2 );
    CPPUNIT_CNCNEQ_ASSERT( s1, neq3 );


//    wxString _s1 = wxT("A\0HH");
//    wxString _eq = wxT("A\0HH");
//    wxString _eq2 = wxT("A\0hH");
//    wxString _eq3 = wxT("a\0hh");
//    wxString _neq = wxT("H\0AH");
//    wxString _neq2 = wxT("A\0H");
//    wxString _neq3 = wxT("A\0HHH");

    s1.insert(1,1,'\0');
    eq.insert(1,1,'\0');
    eq2.insert(1,1,'\0');
    eq3.insert(1,1,'\0');
    neq.insert(1,1,'\0');
    neq2.insert(1,1,'\0');
    neq3.insert(1,1,'\0');

    CPPUNIT_CNCEQ_ASSERT( s1, eq );
    CPPUNIT_CNCEQ_ASSERT( s1, eq2 );
    CPPUNIT_CNCEQ_ASSERT( s1, eq3 );

    CPPUNIT_CNCNEQ_ASSERT( s1, neq );
    CPPUNIT_CNCNEQ_ASSERT( s1, neq2 );
    CPPUNIT_CNCNEQ_ASSERT( s1, neq3 );
}

void StringTestCase::Contains()
{
    static const struct ContainsData
    {
        const wxChar *hay;
        const wxChar *needle;
        bool contains;
    } containsData[] =
    {
        { _T(""),       _T(""),         true  },
        { _T(""),       _T("foo"),      false },
        { _T("foo"),    _T(""),         true  },
        { _T("foo"),    _T("f"),        true  },
        { _T("foo"),    _T("o"),        true  },
        { _T("foo"),    _T("oo"),       true  },
        { _T("foo"),    _T("ooo"),      false },
        { _T("foo"),    _T("oooo"),     false },
        { _T("foo"),    _T("fooo"),     false },
    };

    for ( size_t n = 0; n < WXSIZEOF(containsData); n++ )
    {
        const ContainsData& cd = containsData[n];
        CPPUNIT_ASSERT_EQUAL( cd.contains, wxString(cd.hay).Contains(cd.needle) );
    }
}

// flags used in ToLongData.flags
enum
{
    Number_Ok       = 0,
    Number_Invalid  = 1,
    Number_Unsigned = 2,    // if not specified, works for signed conversion
    Number_Signed   = 4,    // if not specified, works for unsigned
    Number_LongLong = 8,    // only for long long tests
    Number_Long     = 16,   // only for long tests
};

static const struct ToLongData
{
    const wxChar *str;
#ifdef wxLongLong_t
    wxLongLong_t value;
#else
    long value;
#endif // wxLongLong_t
    int flags;

    long LValue() const { return value; }
    unsigned long ULValue() const { return value; }
#ifdef wxLongLong_t
    wxLongLong_t LLValue() const { return value; }
    wxULongLong_t ULLValue() const { return (wxULongLong_t)value; }
#endif // wxLongLong_t

    bool IsOk() const { return !(flags & Number_Invalid); }
} longData[] =
{
    { _T("1"), 1, Number_Ok },
    { _T("0"), 0, Number_Ok },
    { _T("a"), 0, Number_Invalid },
    { _T("12345"), 12345, Number_Ok },
    { _T("--1"), 0, Number_Invalid },

    { _T("-1"), -1, Number_Signed | Number_Long },
    // this is surprizing but consistent with strtoul() behaviour
    { _T("-1"), ULONG_MAX, Number_Unsigned | Number_Long },

    // this must overflow, even with 64 bit long
    { _T("922337203685477580711"), 0, Number_Invalid },

#ifdef wxLongLong_t
    { _T("2147483648"), wxLL(2147483648), Number_LongLong },
    { _T("-2147483648"), wxLL(-2147483648), Number_LongLong | Number_Signed },
    { _T("9223372036854775808"), wxULL(9223372036854775808), Number_LongLong |
                                                             Number_Unsigned },
#endif // wxLongLong_t
};

void StringTestCase::ToLong()
{
    long l;
    for ( size_t n = 0; n < WXSIZEOF(longData); n++ )
    {
        const ToLongData& ld = longData[n];

        if ( ld.flags & (Number_LongLong | Number_Unsigned) )
            continue;

        CPPUNIT_ASSERT_EQUAL( ld.IsOk(), wxString(ld.str).ToLong(&l) );
        if ( ld.IsOk() )
            CPPUNIT_ASSERT_EQUAL( ld.LValue(), l );
    }
}

void StringTestCase::ToULong()
{
    unsigned long ul;
    for ( size_t n = 0; n < WXSIZEOF(longData); n++ )
    {
        const ToLongData& ld = longData[n];

        if ( ld.flags & (Number_LongLong | Number_Signed) )
            continue;

        CPPUNIT_ASSERT_EQUAL( ld.IsOk(), wxString(ld.str).ToULong(&ul) );
        if ( ld.IsOk() )
            CPPUNIT_ASSERT_EQUAL( ld.ULValue(), ul );
    }
}

#ifdef wxLongLong_t

void StringTestCase::ToLongLong()
{
    wxLongLong_t l;
    for ( size_t n = 0; n < WXSIZEOF(longData); n++ )
    {
        const ToLongData& ld = longData[n];

        if ( ld.flags & (Number_Long | Number_Unsigned) )
            continue;

        CPPUNIT_ASSERT_EQUAL( ld.IsOk(), wxString(ld.str).ToLongLong(&l) );
        if ( ld.IsOk() )
            CPPUNIT_ASSERT_EQUAL( ld.LLValue(), l );
    }
}

void StringTestCase::ToULongLong()
{
    wxULongLong_t ul;
    for ( size_t n = 0; n < WXSIZEOF(longData); n++ )
    {
        const ToLongData& ld = longData[n];

        if ( ld.flags & (Number_Long | Number_Signed) )
            continue;

        CPPUNIT_ASSERT_EQUAL( ld.IsOk(), wxString(ld.str).ToULongLong(&ul) );
        if ( ld.IsOk() )
            CPPUNIT_ASSERT_EQUAL( ld.ULLValue(), ul );
    }
}

#endif // wxLongLong_t

void StringTestCase::ToDouble()
{
    double d;
    static const struct ToDoubleData
    {
        const wxChar *str;
        double value;
        bool ok;
    } doubleData[] =
    {
        { _T("1"), 1, true },
        { _T("1.23"), 1.23, true },
        { _T(".1"), .1, true },
        { _T("1."), 1, true },
        { _T("1.."), 0, false },
        { _T("0"), 0, true },
        { _T("a"), 0, false },
        { _T("12345"), 12345, true },
        { _T("-1"), -1, true },
        { _T("--1"), 0, false },
    };

    // we need to use decimal point, not comma or whatever is its value for the
    // current locale
    wxSetlocale(LC_ALL, "C");

    size_t n;
    for ( n = 0; n < WXSIZEOF(doubleData); n++ )
    {
        const ToDoubleData& ld = doubleData[n];
        CPPUNIT_ASSERT_EQUAL( ld.ok, wxString(ld.str).ToDouble(&d) );
        if ( ld.ok )
            CPPUNIT_ASSERT_EQUAL( ld.value, d );
    }
}

void StringTestCase::StringBuf()
{
    // check that buffer can be used to write into the string
    wxString s;
    wxStrcpy(wxStringBuffer(s, 10), _T("foo"));

    CPPUNIT_ASSERT_EQUAL(3, s.length());
    CPPUNIT_ASSERT(_T('f') == s[0u]);
    CPPUNIT_ASSERT(_T('o') == s[1]);
    CPPUNIT_ASSERT(_T('o') == s[2]);

    {
        // also check that the buffer initially contains the original string
        // contents
        wxStringBuffer buf(s, 10);
        CPPUNIT_ASSERT_EQUAL( _T('f'), buf[0] );
        CPPUNIT_ASSERT_EQUAL( _T('o'), buf[1] );
        CPPUNIT_ASSERT_EQUAL( _T('o'), buf[2] );
        CPPUNIT_ASSERT_EQUAL( _T('\0'), buf[3] );
    }

    {
        wxStringBufferLength buf(s, 10);
        CPPUNIT_ASSERT_EQUAL( _T('f'), buf[0] );
        CPPUNIT_ASSERT_EQUAL( _T('o'), buf[1] );
        CPPUNIT_ASSERT_EQUAL( _T('o'), buf[2] );
        CPPUNIT_ASSERT_EQUAL( _T('\0'), buf[3] );

        // and check that it can be used to write only the specified number of
        // characters to the string
        wxStrcpy(buf, _T("barrbaz"));
        buf.SetLength(4);
    }

    CPPUNIT_ASSERT_EQUAL(4, s.length());
    CPPUNIT_ASSERT(_T('b') == s[0u]);
    CPPUNIT_ASSERT(_T('a') == s[1]);
    CPPUNIT_ASSERT(_T('r') == s[2]);
    CPPUNIT_ASSERT(_T('r') == s[3]);

    // check that creating buffer of length smaller than string works, i.e. at
    // least doesn't crash (it would if we naively copied the entire original
    // string contents in the buffer)
    *wxStringBuffer(s, 1) = '!';
}

void StringTestCase::UTF8Buf()
{
#if wxUSE_UNICODE
    // "czech" in Czech ("cestina"):
    static const char *textUTF8 = "\304\215e\305\241tina";
    static const wchar_t textUTF16[] = {0x10D, 0x65, 0x161, 0x74, 0x69, 0x6E, 0x61, 0};

    wxString s;
    wxStrcpy(wxUTF8StringBuffer(s, 9), textUTF8);
    CPPUNIT_ASSERT(s == textUTF16);

    {
        wxUTF8StringBufferLength buf(s, 20);
        wxStrcpy(buf, textUTF8);
        buf.SetLength(5);
    }
    CPPUNIT_ASSERT(s == wxString(textUTF16, 0, 3));
#endif // wxUSE_UNICODE
}



void StringTestCase::CStrDataTernaryOperator()
{
    DoCStrDataTernaryOperator(true);
    DoCStrDataTernaryOperator(false);
}

template<typename T> bool CheckStr(const wxString& expected, T s)
{
    return expected == wxString(s);
}

void StringTestCase::DoCStrDataTernaryOperator(bool cond)
{
    // test compilation of wxCStrData when used with operator?: (the asserts
    // are not very important, we're testing if the code compiles at all):

    wxString s("foo");

    const wchar_t *wcStr = L"foo";
    CPPUNIT_ASSERT( CheckStr(s, (cond ? s.c_str() : wcStr)) );
    CPPUNIT_ASSERT( CheckStr(s, (cond ? s.c_str() : L"foo")) );
    CPPUNIT_ASSERT( CheckStr(s, (cond ? wcStr : s.c_str())) );
    CPPUNIT_ASSERT( CheckStr(s, (cond ? L"foo" : s.c_str())) );

    const char *mbStr = "foo";
    CPPUNIT_ASSERT( CheckStr(s, (cond ? s.c_str() : mbStr)) );
    CPPUNIT_ASSERT( CheckStr(s, (cond ? s.c_str() : "foo")) );
    CPPUNIT_ASSERT( CheckStr(s, (cond ? mbStr : s.c_str())) );
    CPPUNIT_ASSERT( CheckStr(s, (cond ? "foo" : s.c_str())) );

    wxString empty("");
    CPPUNIT_ASSERT( CheckStr(empty, (cond ? empty.c_str() : wxEmptyString)) );
    CPPUNIT_ASSERT( CheckStr(empty, (cond ? wxEmptyString : empty.c_str())) );
}

void StringTestCase::CStrDataOperators()
{
    wxString s("hello");

    CPPUNIT_ASSERT( s.c_str()[0] == 'h' );
    CPPUNIT_ASSERT( s.c_str()[1] == 'e' );
    CPPUNIT_ASSERT( s.c_str()[5] == '\0' );

    CPPUNIT_ASSERT( *s.c_str() == 'h' );
    CPPUNIT_ASSERT( *(s.c_str() + 2) == 'l' );
    CPPUNIT_ASSERT( *(s.c_str() + 5) == '\0' );
}

bool CheckStrChar(const wxString& expected, char *s)
    { return CheckStr(expected, s); }
bool CheckStrWChar(const wxString& expected, wchar_t *s)
    { return CheckStr(expected, s); }
bool CheckStrConstChar(const wxString& expected, const char *s)
    { return CheckStr(expected, s); }
bool CheckStrConstWChar(const wxString& expected, const wchar_t *s)
    { return CheckStr(expected, s); }

void StringTestCase::CStrDataImplicitConversion()
{
    wxString s("foo");

    CPPUNIT_ASSERT( CheckStrConstWChar(s, s.c_str()) );
    CPPUNIT_ASSERT( CheckStrConstChar(s, s.c_str()) );

    // implicit conversion of wxString is not available in STL build
#if !wxUSE_STL
    CPPUNIT_ASSERT( CheckStrConstWChar(s, s) );
    CPPUNIT_ASSERT( CheckStrConstChar(s, s) );
#endif
}

void StringTestCase::ExplicitConversion()
{
    wxString s("foo");

    CPPUNIT_ASSERT( CheckStr(s, s.mb_str()) );
    CPPUNIT_ASSERT( CheckStrConstChar(s, s.mb_str()) );
    CPPUNIT_ASSERT( CheckStrChar(s, s.char_str()) );

    CPPUNIT_ASSERT( CheckStr(s, s.wc_str()) );
    CPPUNIT_ASSERT( CheckStrConstWChar(s, s.wc_str()) );
    CPPUNIT_ASSERT( CheckStrWChar(s, s.wchar_str()) );
}
