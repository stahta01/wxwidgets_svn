/////////////////////////////////////////////////////////////////////////////
// Name:        encconv.cpp
// Purpose:     wxEncodingConverter class for converting between different
//              font encodings
// Author:      Vaclav Slavik
// Copyright:   (c) 1999 Vaclav Slavik
// Licence:     wxWindows Licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "encconv.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
  #pragma hdrstop
#endif

#if wxUSE_FONTMAP

#include "wx/encconv.h"

#include <stdlib.h>

// conversion tables, generated by scripts in $(WXWIN)/misc/unictabl:
#if defined( __BORLANDC__ ) || defined(__DARWIN__)
#include "../common/unictabl.inc"
#else
#include "unictabl.inc"
#endif

#if wxUSE_WCHAR_T
typedef wchar_t tchar;
#else
typedef char tchar;
#endif

#ifdef __WXWINCE__
#undef LINKAGEMODE
#define LINKAGEMODE __cdecl
#endif

#ifdef __WXMAC__

#include <ATSUnicode.h>
#include <TextCommon.h>
#include <TextEncodingConverter.h>

#include "wx/fontutil.h"
#include "wx/mac/private.h"  // includes mac headers

wxUint16 gMacEncodings[wxFONTENCODING_MACMAX-wxFONTENCODING_MACMIN+1][128] ;
bool gMacEncodingsInited[wxFONTENCODING_MACMAX-wxFONTENCODING_MACMIN+1] ;

#endif

static wxUint16* LINKAGEMODE GetEncTable(wxFontEncoding enc)
{
#ifdef __WXMAC__
    if( enc >= wxFONTENCODING_MACMIN && enc <= wxFONTENCODING_MACMAX )
    {
        int i = enc-wxFONTENCODING_MACMIN ;
        if ( gMacEncodingsInited[i] == false )
        {
            TECObjectRef converter ;
            TextEncodingBase code = wxMacGetSystemEncFromFontEnc( enc ) ;
	        TextEncodingBase unicode = CreateTextEncoding(kTextEncodingUnicodeDefault,0,kUnicode16BitFormat) ;
    	    OSStatus status = TECCreateConverter(&converter,code,unicode);
    	    char s[2] ;
    	    s[1] = 0 ;
    	    ByteCount byteInLen, byteOutLen ;
    	    for( unsigned char c = 255 ; c >= 128 ; --c )
    	    {
    	        s[0] = c ;
                status = TECConvertText(converter, (ConstTextPtr) &s , 1, &byteInLen,
                (TextPtr) &gMacEncodings[i][c-128] , 2, &byteOutLen);
    	    }
            status = TECDisposeConverter(converter);
            gMacEncodingsInited[i]=true;	
        }
        return gMacEncodings[i] ;
    }
#endif

    for (int i = 0; encodings_list[i].table != NULL; i++)
    {
        if (encodings_list[i].encoding == enc)
            return encodings_list[i].table;
    }
    return NULL;
}

typedef struct {
    wxUint16 u;
    wxUint8  c;
} CharsetItem;

extern "C" int LINKAGEMODE CompareCharsetItems(const void *i1, const void *i2)
{
    return ( ((CharsetItem*)i1) -> u - ((CharsetItem*)i2) -> u );
}


static CharsetItem* LINKAGEMODE BuildReverseTable(wxUint16 *tbl)
{
    CharsetItem *rev = new CharsetItem[128];

    for (int i = 0; i < 128; i++)
        rev[i].c = 128 + i, rev[i].u = tbl[i];

    qsort(rev, 128, sizeof(CharsetItem), CompareCharsetItems);

    return rev;
}



wxEncodingConverter::wxEncodingConverter()
{
    m_Table = NULL;
    m_UnicodeInput = m_UnicodeOutput = FALSE;
    m_JustCopy = FALSE;
}



bool wxEncodingConverter::Init(wxFontEncoding input_enc, wxFontEncoding output_enc, int method)
{
    unsigned i;
    wxUint16 *in_tbl, *out_tbl = NULL;

    if (m_Table) {delete[] m_Table; m_Table = NULL;}

#if !wxUSE_WCHAR_T
    if (input_enc == wxFONTENCODING_UNICODE || output_enc == wxFONTENCODING_UNICODE) return FALSE;
#endif

    if (input_enc == output_enc) {m_JustCopy = TRUE; return TRUE;}

    m_UnicodeOutput = (output_enc == wxFONTENCODING_UNICODE);
    m_JustCopy = FALSE;

    if (input_enc == wxFONTENCODING_UNICODE)
    {
        if ((out_tbl = GetEncTable(output_enc)) == NULL) return FALSE;

        m_Table = new tchar[65536];
        for (i = 0; i < 128; i++)  m_Table[i] = (tchar)i; // 7bit ASCII
        for (i = 128; i < 65536; i++)  m_Table[i] = (tchar)'?';
                // FIXME - this should be character that means `unicode to charset' impossible, not '?'

        if (method == wxCONVERT_SUBSTITUTE)
        {
            for (i = 0; i < encoding_unicode_fallback_count; i++)
                m_Table[encoding_unicode_fallback[i].c] = (tchar) encoding_unicode_fallback[i].s;
        }

        for (i = 0; i < 128; i++)
            m_Table[out_tbl[i]] = (tchar)(128 + i);

        m_UnicodeInput = TRUE;
    }
    else // input !Unicode
    {
        if ((in_tbl = GetEncTable(input_enc)) == NULL) return FALSE;
        if (output_enc != wxFONTENCODING_UNICODE)
            if ((out_tbl = GetEncTable(output_enc)) == NULL) return FALSE;

        m_UnicodeInput = FALSE;

        m_Table = new tchar[256];
        for (i = 0; i < 128; i++)  m_Table[i] = (tchar)i; // 7bit ASCII

        if (output_enc == wxFONTENCODING_UNICODE)
        {
            for (i = 0; i < 128; i++)  m_Table[128 + i] = (tchar)in_tbl[i];
            return TRUE;
        }
        // FIXME: write a substitute for bsearch
#ifndef __WXWINCE__
        else // output !Unicode
        {
            CharsetItem *rev = BuildReverseTable(out_tbl);
            CharsetItem *item;
            CharsetItem key;

            for (i = 0; i < 128; i++)
            {
                key.u = in_tbl[i];
                item = (CharsetItem*) bsearch(&key, rev, 128, sizeof(CharsetItem), CompareCharsetItems);
                if (item == NULL && method == wxCONVERT_SUBSTITUTE)
                    item = (CharsetItem*) bsearch(&key, encoding_unicode_fallback,
                                encoding_unicode_fallback_count, sizeof(CharsetItem), CompareCharsetItems);
                if (item)
                    m_Table[128 + i] = (tchar)item -> c;
                else
#if wxUSE_WCHAR_T
                    m_Table[128 + i] = (wchar_t)(128 + i);
#else
                    m_Table[128 + i] = (char)(128 + i);
#endif
            }

            delete[] rev;
        }
#endif // !__WXWINCE__
    }

    return TRUE;
}



void wxEncodingConverter::Convert(const char* input, char* output) const
{
    wxASSERT_MSG(!m_UnicodeOutput, wxT("You cannot convert to unicode if output is const char*!"));
    wxASSERT_MSG(!m_UnicodeInput, wxT("You cannot convert from unicode if input is const char*!"));

    const char *i;
    char *o;

    if (m_JustCopy)
    {
        strcpy(output, input);
        return;
    }

    wxCHECK_RET(m_Table != NULL, wxT("You must call wxEncodingConverter::Init() before actually converting!"));

    for (i = input, o = output; *i != 0;)
        *(o++) = (char)(m_Table[(wxUint8)*(i++)]);
    *o = 0;
}


#if wxUSE_WCHAR_T

void wxEncodingConverter::Convert(const char* input, wchar_t* output) const
{
    wxASSERT_MSG(m_UnicodeOutput, wxT("You cannot convert to 8-bit if output is const wchar_t*!"));
    wxASSERT_MSG(!m_UnicodeInput, wxT("You cannot convert from unicode if input is const char*!"));

    const char *i;
    wchar_t *o;

    if (m_JustCopy)
    {
        for (i = input, o = output; *i != 0;)
            *(o++) = (wchar_t)(*(i++));
        *o = 0;
        return;
    }

    wxCHECK_RET(m_Table != NULL, wxT("You must call wxEncodingConverter::Init() before actually converting!"));

    for (i = input, o = output; *i != 0;)
        *(o++) = (wchar_t)(m_Table[(wxUint8)*(i++)]);
    *o = 0;
}



void wxEncodingConverter::Convert(const wchar_t* input, char* output) const
{
    wxASSERT_MSG(!m_UnicodeOutput, wxT("You cannot convert to unicode if output is const char*!"));
    wxASSERT_MSG(m_UnicodeInput, wxT("You cannot convert from 8-bit if input is const wchar_t*!"));

    const wchar_t *i;
    char *o;

    if (m_JustCopy)
    {
        for (i = input, o = output; *i != 0;)
            *(o++) = (char)(*(i++));
        *o = 0;
        return;
    }

    wxCHECK_RET(m_Table != NULL, wxT("You must call wxEncodingConverter::Init() before actually converting!"));

    for (i = input, o = output; *i != 0;)
        *(o++) = (char)(m_Table[(wxUint16)*(i++)]);
    *o = 0;
}



void wxEncodingConverter::Convert(const wchar_t* input, wchar_t* output) const
{
    wxASSERT_MSG(m_UnicodeOutput, wxT("You cannot convert to 8-bit if output is const wchar_t*!"));
    wxASSERT_MSG(m_UnicodeInput, wxT("You cannot convert from 8-bit if input is const wchar_t*!"));

    const wchar_t *i;
    wchar_t *o;

    if (m_JustCopy)
    {
        // wcscpy() is not guaranteed to exist
        for (i = input, o = output; *i != 0;)
            *(o++) = (*(i++));
        *o = 0;
        return;
    }

    wxCHECK_RET(m_Table != NULL, wxT("You must call wxEncodingConverter::Init() before actually converting!"));

    for (i = input, o = output; *i != 0;)
        *(o++) = (wchar_t)(m_Table[(wxUint8)*(i++)]);
    *o = 0;
}

#endif // wxUSE_WCHAR_T


wxString wxEncodingConverter::Convert(const wxString& input) const
{
    if (m_JustCopy) return input;

    wxString s;
    const wxChar *i;

    wxCHECK_MSG(m_Table != NULL, s,
                wxT("You must call wxEncodingConverter::Init() before actually converting!"));

    if (m_UnicodeInput)
    {
        for (i = input.c_str(); *i != 0; i++)
            s << (wxChar)(m_Table[(wxUint16)*i]);
    }
    else
    {
        for (i = input.c_str(); *i != 0; i++)
            s << (wxChar)(m_Table[(wxUint8)*i]);
    }

    return s;
}







// Following tables describe classes of encoding equivalence.
//

#define STOP wxFONTENCODING_SYSTEM

#define NUM_OF_PLATFORMS  4 /*must conform to enum wxPLATFORM_XXXX !!!*/
#define ENC_PER_PLATFORM  5
           // max no. of encodings for one language used on one platform
           // Anybody thinks 5 is not enough? ;-)

static wxFontEncoding
    EquivalentEncodings[][NUM_OF_PLATFORMS][ENC_PER_PLATFORM+1] = {

    // *** Please put more common encodings as first! ***

    // Western European
    {
        /* unix    */ {wxFONTENCODING_ISO8859_1, wxFONTENCODING_ISO8859_15, STOP},
        /* windows */ {wxFONTENCODING_CP1252, STOP},
        /* os2     */ {STOP},
        /* mac     */ {wxFONTENCODING_MACROMAN, STOP}
    },

    // Central European
    {
        /* unix    */ {wxFONTENCODING_ISO8859_2, STOP},
        /* windows */ {wxFONTENCODING_CP1250, STOP},
        /* os2     */ {STOP},
        /* mac     */ {wxFONTENCODING_MACCENTRALEUR, STOP}
    },

    // Baltic
    {
        /* unix    */ {wxFONTENCODING_ISO8859_13, wxFONTENCODING_ISO8859_4, STOP},
        /* windows */ {wxFONTENCODING_CP1257, STOP},
        /* os2     */ {STOP},
        /* mac     */ {STOP}
    },

    // Hebrew
    {
        /* unix    */ {wxFONTENCODING_ISO8859_8, STOP},
        /* windows */ {wxFONTENCODING_CP1255, STOP},
        /* os2     */ {STOP},
        /* mac     */ {wxFONTENCODING_MACHEBREW, STOP}
    },

    // Greek
    {
        /* unix    */ {wxFONTENCODING_ISO8859_7, STOP},
        /* windows */ {wxFONTENCODING_CP1253, STOP},
        /* os2     */ {STOP},
        /* mac     */ {wxFONTENCODING_MACGREEK, STOP}
    },

    // Arabic
    {
        /* unix    */ {wxFONTENCODING_ISO8859_6, STOP},
        /* windows */ {wxFONTENCODING_CP1256, STOP},
        /* os2     */ {STOP},
        /* mac     */ {wxFONTENCODING_MACARABIC, STOP}
    },

    // Turkish
    {
        /* unix    */ {wxFONTENCODING_ISO8859_9, STOP},
        /* windows */ {wxFONTENCODING_CP1254, STOP},
        /* os2     */ {STOP},
        /* mac     */ {wxFONTENCODING_MACTURKISH, STOP}
    },

    // Cyrillic
    {
        /* unix    */ {wxFONTENCODING_KOI8, wxFONTENCODING_ISO8859_5, STOP},
        /* windows */ {wxFONTENCODING_CP1251, STOP},
        /* os2     */ {STOP},
        /* mac     */ {wxFONTENCODING_MACCYRILLIC, STOP}
    },

    {{STOP},{STOP},{STOP},{STOP}} /* Terminator */
    /* no, _not_ Arnold! */
};


static bool FindEncoding(const wxFontEncodingArray& arr, wxFontEncoding f)
{
    for (wxFontEncodingArray::const_iterator it = arr.begin(), en = arr.end();
         it != en; ++it)
        if (*it == f)
            return true;
    return false;
}

wxFontEncodingArray wxEncodingConverter::GetPlatformEquivalents(wxFontEncoding enc, int platform)
{
    if (platform == wxPLATFORM_CURRENT)
    {
#if defined(__WXMSW__)
        platform = wxPLATFORM_WINDOWS;
#elif defined(__WXGTK__) || defined(__WXMOTIF__)
        platform = wxPLATFORM_UNIX;
#elif defined(__WXOS2__)
        platform = wxPLATFORM_OS2;
#elif defined(__WXMAC__)
        platform = wxPLATFORM_MAC;
#endif
    }

    int i, clas, e ;
    wxFontEncoding *f;
    wxFontEncodingArray arr;

    clas = 0;
    while (EquivalentEncodings[clas][0][0] != STOP)
    {
        for (i = 0; i < NUM_OF_PLATFORMS; i++)
            for (e = 0; EquivalentEncodings[clas][i][e] != STOP; e++)
                if (EquivalentEncodings[clas][i][e] == enc)
                {
                    for (f = EquivalentEncodings[clas][platform]; *f != STOP; f++)
                        if (*f == enc) arr.push_back(enc);
                    for (f = EquivalentEncodings[clas][platform]; *f != STOP; f++)
                        if (!FindEncoding(arr, *f)) arr.push_back(*f);
                    i = NUM_OF_PLATFORMS/*hack*/; break;
                }
        clas++;
    }

    return arr;
}



wxFontEncodingArray wxEncodingConverter::GetAllEquivalents(wxFontEncoding enc)
{
    int i, clas, e, j ;
    wxFontEncoding *f;
    wxFontEncodingArray arr;

    arr = GetPlatformEquivalents(enc); // we want them to be first items in array

    clas = 0;
    while (EquivalentEncodings[clas][0][0] != STOP)
    {
        for (i = 0; i < NUM_OF_PLATFORMS; i++)
            for (e = 0; EquivalentEncodings[clas][i][e] != STOP; e++)
                if (EquivalentEncodings[clas][i][e] == enc)
                {
                    for (j = 0; j < NUM_OF_PLATFORMS; j++)
                        for (f = EquivalentEncodings[clas][j]; *f != STOP; f++)
                            if (!FindEncoding(arr, *f)) arr.push_back(*f);
                    i = NUM_OF_PLATFORMS/*hack*/; break;
                }
        clas++;
    }

    return arr;
}

#endif // wxUSE_FONTMAP
