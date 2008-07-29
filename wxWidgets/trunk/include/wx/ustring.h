/////////////////////////////////////////////////////////////////////////////
// Name:        wx/ustring.h
// Purpose:     32-bit string (UCS-4)
// Author:      Robert Roebling
// Copyright:   (c) Robert Roebling
// RCS-ID:      $Id: tab.h 37400 2006-02-09 00:28:34Z VZ $
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_USTRING_H_BASE_
#define _WX_USTRING_H_BASE_

#include "wx/defs.h"
#include "wx/string.h"

WXDLLIMPEXP_TEMPLATE_INSTANCE_BASE( wxCharTypeBuffer<wxChar32> )

#if SIZEOF_WCHAR_T == 2

typedef wxWCharBuffer wxU16CharBuffer;

#else

class WXDLLIMPEXP_BASE wxU16CharBuffer : public wxCharTypeBuffer<wxChar16>
{
public:
    typedef wxCharTypeBuffer<wxChar16> wxCharTypeBufferBase;

    wxU16CharBuffer(const wxCharTypeBufferBase& buf)
        : wxCharTypeBufferBase(buf) {}

    wxU16CharBuffer(const CharType *str = NULL) : wxCharTypeBufferBase(str) {}
    wxU16CharBuffer(size_t len) : wxCharTypeBufferBase(len) {}
};

#endif



#if SIZEOF_WCHAR_T == 2

class WXDLLIMPEXP_BASE wxU32CharBuffer : public wxCharTypeBuffer<wxChar32>
{
public:
    typedef wxCharTypeBuffer<wxChar32> wxCharTypeBufferBase;

    wxU32CharBuffer(const wxCharTypeBufferBase& buf)
        : wxCharTypeBufferBase(buf) {}

    wxU32CharBuffer(const CharType *str = NULL) : wxCharTypeBufferBase(str) {}
    wxU32CharBuffer(size_t len) : wxCharTypeBufferBase(len) {}
};

#else

typedef wxWCharBuffer wxU32CharBuffer;

#endif

class WXDLLIMPEXP_BASE wxUString: public std::basic_string<wxChar32>
{
public: 
    wxUString() { }
    
    wxUString( const wxChar32 *str )                            { assign(str); }
    wxUString( const wxUString &str )                           { assign(str); }
    wxUString( const wxU32CharBuffer &buf )                     { assign(buf); }
    
    wxUString( const char *str )                                { assign(str); }
    wxUString( const wxCharBuffer &buf )                        { assign(buf); }
    wxUString( const char *str, const wxMBConv &conv )          { assign(str,conv); }
    wxUString( const wxCharBuffer &buf, const wxMBConv &conv )  { assign(buf,conv); }

    wxUString( const wxChar16 *str )                            { assign(str); }
    wxUString( const wxU16CharBuffer &buf )                     { assign(buf); }
    
    wxUString( const wxCStrData *cstr )                         { assign(cstr); }
    wxUString( const wxString &str )                            { assign(str); }
    
    wxUString( char ch )                                        { assign(ch); }
    wxUString( wxChar16 ch )                                    { assign(ch); }
    wxUString( wxChar32 ch )                                    { assign(ch); }
    wxUString( wxUniChar ch )                                   { assign(ch); }
    wxUString( wxUniCharRef ch )                                { assign(ch); }
    wxUString( size_type n, char ch )                           { assign(n,ch); }
    wxUString( size_type n, wxChar16 ch )                       { assign(n,ch); }
    wxUString( size_type n, wxChar32 ch )                       { assign(n,ch); }
    wxUString( size_type n, wxUniChar ch )                      { assign(n,ch); }
    wxUString( size_type n, wxUniCharRef ch )                   { assign(n,ch); }
    
    // static construction
    
    static wxUString FromAscii( const char *str, size_type n )
    {
        wxUString ret;
        ret.assignFromAscii( str, n );
        return ret;
    }
    
    static wxUString FromAscii( const char *str )
    {
        wxUString ret;
        ret.assignFromAscii( str );
        return ret;
    }
    
    static wxUString FromUTF8( const char *str, size_type n )
    {
        wxUString ret;
        ret.assignFromUTF8( str, n );
        return ret;
    }
    
    static wxUString FromUTF8( const char *str )
    {
        wxUString ret;
        ret.assignFromUTF8( str );
        return ret;
    }
    
    static wxUString FromUTF16( const wxChar16 *str, size_type n )
    {
        wxUString ret;
        ret.assignFromUTF16( str, n );
        return ret;
    }
    
    static wxUString FromUTF16( const wxChar16 *str )
    {
        wxUString ret;
        ret.assignFromUTF16( str );
        return ret;
    }
    
    // assign from encoding
    
    wxUString &assignFromAscii( const char *str );
    wxUString &assignFromAscii( const char *str, size_type n );
    wxUString &assignFromUTF8( const char *str );
    wxUString &assignFromUTF8( const char *str, size_type n );
    wxUString &assignFromUTF16( const wxChar16* str );
    wxUString &assignFromUTF16( const wxChar16* str, size_type n );
    wxUString &assignFromCString( const char* str );
    wxUString &assignFromCString( const char* str, const wxMBConv &conv );
 
    // conversions
    
    wxCharBuffer utf8_str() const;
    wxU16CharBuffer utf16_str() const;
    
#if SIZEOF_WCHAR_T == 2
    wxWCharBuffer wc_str() const
    {
       return utf16_str();
    }
#else
    wchar_t *wc_str() const
    {
        return (wchar_t*) c_str();
    }
#endif

    operator wxString() const
    {
#if wxUSE_UNICODE_UTF8
        return wxString::FromUTF8( utf8_str() );
#else
#if SIZEOF_WCHAR_T == 2
        return wxString( utf16_str() );
#else
        return wxString( c_str() );
#endif
#endif
    }

#if wxUSE_UNICODE_UTF8
    wxCharBuffer wx_str()
    {
        return utf8_str();
    }
#else
#if SIZEOF_WCHAR_T == 2
    wxWCharBuffer wx_str()
    {
        return utf16_str();
    }
#else
    wchar_t* wx_str()
    {
        return c_str();
    }
#endif
#endif

    // assign
    
    inline wxUString &assign( const wxChar32* str )
    {
        std::basic_string<wxChar32> *base = this;
        return (wxUString &) base->assign( str );
    }
     
    inline wxUString &assign( const wxChar32* str, size_type n )
    {
        std::basic_string<wxChar32> *base = this;
        return (wxUString &) base->assign( str, n );
    }
     
    inline wxUString &assign( const wxUString &str )
    {
        std::basic_string<wxChar32> *base = this;
        return (wxUString &) base->assign( str );
    }
     
    inline wxUString &assign( const wxUString &str, size_type pos, size_type n )
    {
        std::basic_string<wxChar32> *base = this;
        return (wxUString &) base->assign( str, pos, n );
    }
     
    inline wxUString &assign( wxChar32 ch )
    {
        std::basic_string<wxChar32> *base = this;
        return (wxUString &) base->assign( (size_type) 1, ch );
    }
    
    inline wxUString &assign( size_type n, wxChar32 ch )
    {
        std::basic_string<wxChar32> *base = this;
        return (wxUString &) base->assign( n, ch );
    }
    
    wxUString &assign( const wxU32CharBuffer &buf )
    {
        return assign( buf.data() );
    }
     
    wxUString &assign( const char *str )
    {
        return assignFromCString( str );
    }
    
    wxUString &assign( const wxCharBuffer &buf )
    { 
        return assignFromCString( buf.data() );
    }
    
    wxUString &assign( const char *str, const wxMBConv &conv )
    { 
        return assignFromCString( str, conv );
    }
    
    wxUString &assign( const wxCharBuffer &buf, const wxMBConv &conv )
    { 
        return assignFromCString( buf.data(), conv );
    }
    
    wxUString &assign( const wxChar16 *str )
    {
        return assignFromUTF16( str );
    }
    
    wxUString &assign( const wxU16CharBuffer &buf )
    {
        return assignFromUTF16( buf.data() );
    }
    
    wxUString &assign( const wxCStrData *cstr )
    {
#if SIZEOF_WCHAR_T == 2
        return assignFromUTF16( cstr->AsWChar() );
#else
        return assign( cstr->AsWChar() );
#endif
    }
    
    wxUString &assign( const wxString &str )
    {
#if wxUSE_UNICODE_UTF8
        return assignFromUTF8( str.wx_str() );
#else
   #if SIZEOF_WCHAR_T == 2
        return assignFromUTF16( str.wc_str() );
   #else
        return assign( wc_str() );
   #endif
#endif
    }
    
    wxUString &assign( char ch )
    {
        char buf[2];
        buf[0] = ch;
        buf[1] = 0;
        return assignFromCString( buf );
    }
    
    wxUString &assign( size_type n, char ch )
    {  
        wxCharBuffer buffer(n);
        char *p = buffer.data();
        size_type i;
        for (i = 0; i < n; i++)
        {
           *p = ch;
           p++;
        }
        return assignFromCString( buffer.data() );
    }
    
    wxUString &assign( wxChar16 ch )
    {
        wxChar16 buf[2];
        buf[0] = ch;
        buf[1] = 0;
        return assignFromUTF16( buf );
    }
    
    wxUString &assign( size_type n, wxChar16 ch )
    {
        wxU16CharBuffer buffer(n);
        wxChar16 *p = buffer.data();
        size_type i;
        for (i = 0; i < n; i++)
        {
           *p = ch;
           p++;
        }
        return assignFromUTF16( buffer.data() );
    }
    
    wxUString &assign( wxUniChar ch )
    {
        return assign( (const wxChar32) ch.GetValue() );
    }
    
    wxUString &assign( size_type n, wxUniChar ch )
    {
        return assign( n, (const wxChar32) ch.GetValue() );
    }
    
    wxUString &assign( wxUniCharRef ch )
    {
        return assign( (const wxChar32) ch.GetValue() );
    }
    
    wxUString &assign( size_type n, wxUniCharRef ch )
    {
        return assign( n, (const wxChar32) ch.GetValue() );
    }
    
    // append [STL overload]
    
    inline wxUString &append( const wxUString &s )
    {
        std::basic_string<wxChar32> *base = this;
        return (wxUString &) base->append( s );
    }
    
    inline wxUString &append( const wxUString &s, size_type pos, size_type n )
    {
        std::basic_string<wxChar32> *base = this;
        return (wxUString &) base->append( s, pos, n );
    }
    
    inline wxUString &append( const wxChar32* s )
    {
        std::basic_string<wxChar32> *base = this;
        return (wxUString &) base->append( s );
    }

    inline wxUString &append( const wxChar32* s, size_type n )
    {
        std::basic_string<wxChar32> *base = this;
        return (wxUString &) base->append( s, n );
    }

    inline wxUString &append( size_type n, wxChar32 c )
    {
        std::basic_string<wxChar32> *base = this;
        return (wxUString &) base->append( n, c );
    }

    inline wxUString &append( wxChar32 c )
    {
        std::basic_string<wxChar32> *base = this;
        return (wxUString &) base->append( 1, c );
    }

    // append [wx overload]

    wxUString &append( const wxU16CharBuffer &buf )
    {
        return append( buf.data() );
    }

    wxUString &append( const wxU32CharBuffer &buf )
    {
        return append( buf.data() );
    }

    wxUString &append( const char *str )
    {
        return append( wxUString( str ) );
    }

    wxUString &append( const wxCharBuffer &buf )
    {
        return append( wxUString( buf ) );
    }

    wxUString &append( const wxChar16 *str )
    {
        return append( wxUString( str ) );
    }
    
    wxUString &append( const wxString &str )
    {
        return append( wxUString( str ) );
    }
    
    wxUString &append( const wxCStrData *cstr )
    {
        return append( wxUString( cstr ) );
    }
    
    wxUString &append( char ch )
    {
        char buf[2];
        buf[0] = ch;
        buf[1] = 0;
        return append( buf );
    }
    
    wxUString &append( wxChar16 ch )
    {
        wxChar16 buf[2];
        buf[0] = ch;
        buf[1] = 0;
        return append( buf );
    }
    
    wxUString &append( wxUniChar ch )
    {
        return append( (size_type) 1, (wxChar32) ch.GetValue() );
    }

    wxUString &append( wxUniCharRef ch )
    {
        return append( (size_type) 1, (wxChar32) ch.GetValue() );
    }

    
    // insert [STL overloads]
    
    inline wxUString &insert( size_type pos, const wxUString &s )
    {
        std::basic_string<wxChar32> *base = this;
        return (wxUString &) base->insert( pos, s );
    }

    inline wxUString &insert( size_type pos, const wxUString &s, size_type pos1, size_type n )
    {
        std::basic_string<wxChar32> *base = this;
        return (wxUString &) base->insert( pos, s, pos1, n );
    }

    inline wxUString &insert( size_type pos, const wxChar32 *s )
    {
        std::basic_string<wxChar32> *base = this;
        return (wxUString &) base->insert( pos, s );
    }

    inline wxUString &insert( size_type pos, const wxChar32 *s, size_type n )
    {
        std::basic_string<wxChar32> *base = this;
        return (wxUString &) base->insert( pos, s, n );
    }

    inline wxUString &insert( size_type pos, size_type n, wxChar32 c )
    {
        std::basic_string<wxChar32> *base = this;
        return (wxUString &) base->insert( pos, n, c );
    }


    // insert [STL overloads]

    wxUString &insert( size_type n, const char *s )
    {
        return insert( n, wxUString( s ) );
    }

    wxUString &insert( size_type n, const wxChar16 *s )
    {
        return insert( n, wxUString( s ) );
    }

    wxUString &insert( size_type n, const wxCharBuffer &buf )
    {
        return insert( n, wxUString( buf ) );
    }

    wxUString &insert( size_type n, const wxU16CharBuffer &buf )
    {
        return insert( n, wxUString( buf ) );
    }

    wxUString &insert( size_type n, const wxU32CharBuffer &buf )
    {
        return insert( n, buf.data() );
    }

    wxUString &insert( size_type n, const wxString &s )
    {
        return insert( n, wxUString( s ) );
    }
    
    wxUString &insert( size_type n, const wxCStrData *cstr )
    {
        return insert( n, wxUString( cstr ) );
    }
    
    wxUString &insert( size_type n, char ch )
    {
        char buf[2];
        buf[0] = ch;
        buf[1] = 0;
        return insert( n, buf );
    }
    
    wxUString &insert( size_type n, wchar_t ch )
    {
        wchar_t buf[2];
        buf[0] = ch;
        buf[1] = 0;
        return insert( n, buf );
    }
    
    // insert iterator
    
    iterator insert( iterator it, wxChar32 ch )
    {
        std::basic_string<wxChar32> *base = this;
        return base->insert( it, ch );
    }
    
    void insert(iterator it, const_iterator first, const_iterator last)
    {
        std::basic_string<wxChar32> *base = this;
        base->insert( it, first, last );
    }


    // operator =
    inline wxUString& operator=(const wxUString& s)
        { return assign( s ); }
    inline wxUString& operator=(const wxString& s)
        { return assign( s ); }
    inline wxUString& operator=(const wxCStrData* s)
        { return assign( s ); }
    inline wxUString& operator=(const char *s)
        { return assign( s ); }
    inline wxUString& operator=(const wxChar16 *s)
        { return assign( s ); }
    inline wxUString& operator=(const wxChar32 *s)
        { return assign( s ); }
    inline wxUString& operator=(const wxCharBuffer &s)
        { return assign( s ); }
    inline wxUString& operator=(const wxU16CharBuffer &s)
        { return assign( s ); }
    inline wxUString& operator=(const wxU32CharBuffer &s)
        { return assign( s ); }
    inline wxUString& operator=(const char ch)
        { return assign( ch ); }
    inline wxUString& operator=(const wxChar16 ch)
        { return assign( ch ); }
    inline wxUString& operator=(const wxChar32 ch)
        { return assign( ch ); }
    inline wxUString& operator=(const wxUniChar ch)
        { return assign( ch ); }
    inline wxUString& operator=(const wxUniCharRef ch)
        { return assign( ch ); }
    
    // operator +=
    inline wxUString& operator+=(const wxUString& s)
        { return append( s ); }
    inline wxUString& operator+=(const wxString& s)
        { return append( s ); }
    inline wxUString& operator+=(const wxCStrData* s)
        { return append( s ); }
    inline wxUString& operator+=(const char *s)
        { return append( s ); }
    inline wxUString& operator+=(const wxChar16 *s)
        { return append( s ); }
    inline wxUString& operator+=(const wxChar32 *s)
        { return append( s ); }
    inline wxUString& operator+=(const wxCharBuffer &s)
        { return append( s ); }
    inline wxUString& operator+=(const wxU16CharBuffer &s)
        { return append( s ); }
    inline wxUString& operator+=(const wxU32CharBuffer &s)
        { return append( s ); }
    inline wxUString& operator+=(const char ch)
        { return append( ch ); }
    inline wxUString& operator+=(const wxChar16 ch)
        { return append( ch ); }
    inline wxUString& operator+=(const wxChar32 ch)
        { return append( ch ); }
    inline wxUString& operator+=(const wxUniChar ch)
        { return append( ch ); }
    inline wxUString& operator+=(const wxUniCharRef ch)
        { return append( ch ); }
    
};

inline wxUString operator+(const wxUString &s1, const wxUString &s2)
    { wxUString ret( s1 ); ret.append( s2 ); return ret; }
inline wxUString operator+(const wxUString &s1, const char *s2)
    { return s1 + wxUString(s2); }
inline wxUString operator+(const wxUString &s1, const wxString &s2)
    { return s1 + wxUString(s2); }
inline wxUString operator+(const wxUString &s1, const wxCStrData *s2)
    { return s1 + wxUString(s2); }
inline wxUString operator+(const wxUString &s1, const wxChar16* s2)
    { return s1 + wxUString(s2); }
inline wxUString operator+(const wxUString &s1, const wxChar32 *s2)
    { return s1 + wxUString(s2); }
inline wxUString operator+(const wxUString &s1, const wxCharBuffer &s2)
    { return s1 + wxUString(s2); }
inline wxUString operator+(const wxUString &s1, const wxU16CharBuffer &s2)
    { return s1 + wxUString(s2); }
inline wxUString operator+(const wxUString &s1, const wxU32CharBuffer &s2)
    { return s1 + wxUString(s2); }
inline wxUString operator+(const wxUString &s1, char s2)
    { return s1 + wxUString(s2); }
inline wxUString operator+(const wxUString &s1, wxChar32 s2)
    { wxUString ret( s1 ); ret.append( s2 ); return ret; }
inline wxUString operator+(const wxUString &s1, wxChar16 s2)
    { wxUString ret( s1 ); ret.append( (wxChar32) s2 ); return ret; }
inline wxUString operator+(const wxUString &s1, wxUniChar s2)
    { wxUString ret( s1 ); ret.append( (wxChar32) s2.GetValue() ); return ret; }
inline wxUString operator+(const wxUString &s1, wxUniCharRef s2)
    { wxUString ret( s1 ); ret.append( (wxChar32) s2.GetValue() ); return ret; }

inline wxUString operator+(const char *s1, const wxUString &s2)
    { return wxUString(s1) + s2; }
inline wxUString operator+(const wxString &s1, const wxUString &s2)
    { return wxUString(s1) + s2; }
inline wxUString operator+(const wxCStrData *s1, const wxUString &s2)
    { return wxUString(s1) + s2; }
inline wxUString operator+(const wxChar16* s1, const wxUString &s2)
    { return wxUString(s1) + s2; }
inline wxUString operator+(const wxChar32 *s1, const wxUString &s2)
    { return wxUString(s1) + s2; }
inline wxUString operator+(const wxCharBuffer &s1, const wxUString &s2)
    { return wxUString(s1) + s2; }
inline wxUString operator+(const wxU16CharBuffer &s1, const wxUString &s2)
    { return wxUString(s1) + s2; }
inline wxUString operator+(const wxU32CharBuffer &s1, const wxUString &s2)
    { return wxUString(s1) + s2; }
inline wxUString operator+(char s1, const wxUString &s2)
    { return wxUString(s1) + s2; }
inline wxUString operator+(wxChar32 s1, const wxUString &s2 )
    { return wxUString(s1) + s2; }
inline wxUString operator+(wxChar16 s1, const wxUString &s2)
    { return wxUString(s1) + s2; }
inline wxUString operator+(wxUniChar s1, const wxUString &s2)
    { return wxUString(s1) + s2; }
inline wxUString operator+(wxUniCharRef s1, const wxUString &s2)
    { return wxUString(s1) + s2; }


inline bool operator==(const wxUString& s1, const wxUString& s2)
    { return s1.compare( s2 ) == 0; }
inline bool operator!=(const wxUString& s1, const wxUString& s2)
    { return s1.compare( s2 ) != 0; }
inline bool operator< (const wxUString& s1, const wxUString& s2)
    { wxPrintf( "test\n"); return s1.compare( s2 ) < 0; }
inline bool operator> (const wxUString& s1, const wxUString& s2)
    { return s1.compare( s2 ) > 0; }
inline bool operator<=(const wxUString& s1, const wxUString& s2)
    { return s1.compare( s2 ) <= 0; }
inline bool operator>=(const wxUString& s1, const wxUString& s2)
    { return s1.compare( s2 ) >= 0; }

#define wxUSTRING_COMP_OPERATORS( T )                       \
inline bool operator==(const wxUString& s1, T s2)           \
    { return s1.compare( wxUString(s2) ) == 0; }            \
inline bool operator!=(const wxUString& s1, T s2)           \
    { return s1.compare( wxUString(s2) ) != 0; }            \
inline bool operator< (const wxUString& s1, T s2)           \
    { return s1.compare( wxUString(s2) ) < 0; }             \
inline bool operator> (const wxUString& s1, T s2)           \
    { return s1.compare( wxUString(s2) ) > 0; }             \
inline bool operator<=(const wxUString& s1, T s2)           \
    { return s1.compare( wxUString(s2) ) <= 0; }            \
inline bool operator>=(const wxUString& s1, T s2)           \
    { return s1.compare( wxUString(s2) ) >= 0; }            \
\
inline bool operator==(T s2, const wxUString& s1)           \
    { return s1.compare( wxUString(s2) ) == 0; }            \
inline bool operator!=(T s2, const wxUString& s1)           \
    { return s1.compare( wxUString(s2) ) != 0; }            \
inline bool operator< (T s2, const wxUString& s1)           \
    { return s1.compare( wxUString(s2) ) > 0; }             \
inline bool operator> (T s2, const wxUString& s1)           \
    { return s1.compare( wxUString(s2) ) < 0; }             \
inline bool operator<=(T s2, const wxUString& s1)           \
    { return s1.compare( wxUString(s2) ) >= 0; }            \
inline bool operator>=(T s2, const wxUString& s1)           \
    { return s1.compare( wxUString(s2) ) <= 0; }

wxUSTRING_COMP_OPERATORS( const wxString & )
wxUSTRING_COMP_OPERATORS( const char * )
wxUSTRING_COMP_OPERATORS( const wxChar16 * )
wxUSTRING_COMP_OPERATORS( const wxChar32 * )
wxUSTRING_COMP_OPERATORS( const wxCharBuffer & )
wxUSTRING_COMP_OPERATORS( const wxU16CharBuffer & )
wxUSTRING_COMP_OPERATORS( const wxU32CharBuffer & )
wxUSTRING_COMP_OPERATORS( const wxCStrData * )

#endif
    // _WX_USTRING_H_BASE_
