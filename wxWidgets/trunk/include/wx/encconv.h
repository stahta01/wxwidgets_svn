/////////////////////////////////////////////////////////////////////////////
// Name:        encconv.h
// Purpose:     wxEncodingConverter class for converting between different
//              font encodings
// Author:      Vaclav Slavik
// Copyright:   (c) 1999 Vaclav Slavik
// Licence:     wxWindows Licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __ENCCONV_H__
#define __ENCCONV_H__

#ifdef __GNUG__
#pragma interface "encconv.h"
#endif

#include "wx/defs.h"
#include "wx/font.h"
#include "wx/dynarray.h"



enum {
    wxCONVERT_STRICT,
    wxCONVERT_SUBSTITUTE
};


enum {
    wxPLATFORM_CURRENT = -1,
    
    wxPLATFORM_UNIX = 0,
    wxPLATFORM_WINDOWS,
    wxPLATFORM_OS2,
    wxPLATFORM_MAC,
};

WX_DEFINE_ARRAY(wxFontEncoding, wxFontEncodingArray);


//--------------------------------------------------------------------------------
// wxEncodingConverter
//                  This class is capable of converting strings between any two
//                  8bit encodings/charsets. It can also convert from/to Unicode
//--------------------------------------------------------------------------------

class WXDLLEXPORT wxEncodingConverter : public wxObject
{
    public:
    
            wxEncodingConverter();
            ~wxEncodingConverter() { if (m_Table) delete[] m_Table; }
    
            // Initialize convertion. Both output or input encoding may
            // be wxFONTENCODING_UNICODE, but only if wxUSE_ENCODING is set to 1.
            //
            // All subsequent calls to Convert() will interpret it's argument
            // as a string in input_enc encoding and will output string in
            // output_enc encoding.
            //
            // You must call this method before calling Convert. You may call 
            // it more than once in order to switch to another conversion
            //
            // Method affects behaviour of Convert() in case input character
            // cannot be converted because it does not exist in output encoding:
            //     wxCONVERT_STRICT -- 
            //              follow behaviour of GNU Recode - just copy unconvertable 
            //              characters to output and don't change them (it's integer 
            //              value will stay the same)
            //     wxCONVERT_SUBSTITUTE --
            //              try some (lossy) substitutions - e.g. replace 
            //              unconvertable latin capitals with acute by ordinary
            //              capitals, replace en-dash or em-dash by '-' etc.
            //     both modes gurantee that output string will have same length
            //     as input string
            //
            // Returns FALSE if given conversion is impossible, TRUE otherwise
            // (conversion may be impossible either if you try to convert
            // to Unicode with non-Unicode build of wxWindows or if input
            // or output encoding is not supported.)
            bool Init(wxFontEncoding input_enc, wxFontEncoding output_enc, int method = wxCONVERT_STRICT);
            
            // Convert input string according to settings passed to Init.
            // Note that you must call Init before using Convert!
            wxString Convert(const wxString& input);
            void Convert(const wxChar* input, wxChar* output);
            void Convert(wxChar* str) { Convert(str, str); }
            
            // Return equivalent(s) for given font that are used
            // under given platform. wxPLATFORM_CURRENT means the plaform
            // this binary was compiled for
            //
            // Examples:
            //     current platform          enc    returned value
            // -----------------------------------------------------
            //     unix                   CP1250         {ISO8859_2}
            //     unix                ISO8859_2                  {}
            //     windows             ISO8859_2            {CP1250}
            //
            // Equivalence is defined in terms of convertibility:
            // 2 encodings are equivalent if you can convert text between
            // then without loosing information (it may - and will - happen
            // that you loose special chars like quotation marks or em-dashes
            // but you shouldn't loose any diacritics and language-specific
            // characters when converting between equivalent encodings).
            // 
            // Convert() method is not limited to converting between 
            // equivalent encodings, it can convert between arbitrary
            // two encodings!
            //
            // Remember that this function does _NOT_ check for presence of
            // fonts in system. It only tells you what are most suitable
            // encodings. (It usually returns only one encoding)
            //
            // Note that argument enc itself may be present in returned array!
            // (so that you can -- as a side effect -- detect whether the
            // encoding is native for this platform or not)
            static wxFontEncodingArray GetPlatformEquivalents(wxFontEncoding enc, int platform = wxPLATFORM_CURRENT);

            // Similar to GetPlatformEquivalent, but this one will return ALL 
            // equivalent encodings, regardless the platform, including itself.
            static wxFontEncodingArray GetAllEquivalents(wxFontEncoding enc);

    private:
    
            wxChar *m_Table;
            bool m_UnicodeInput;
            bool m_JustCopy;
            
};


#endif  // __ENCCONV_H__

















