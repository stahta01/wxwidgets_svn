/////////////////////////////////////////////////////////////////////////////
// Name:        htmltag.cpp
// Purpose:     wxHtmlTag class (represents single tag)
// Author:      Vaclav Slavik
// RCS-ID:      $Id$
// Copyright:   (c) 1999 Vaclav Slavik
// Licence:     wxWindows Licence
/////////////////////////////////////////////////////////////////////////////


#ifdef __GNUG__
#pragma implementation
#endif

#include "wx/wxprec.h"

#include "wx/defs.h"
#if wxUSE_HTML

#ifdef __BORDLANDC__
#pragma hdrstop
#endif

#ifndef WXPRECOMP
#include "wx/wx.h"
#endif

#include "wx/html/htmltag.h"
#include <stdio.h> // for vsscanf
#include <stdarg.h>




//-----------------------------------------------------------------------------
// wxHtmlTagsCache
//-----------------------------------------------------------------------------

IMPLEMENT_CLASS(wxHtmlTagsCache,wxObject)

#define CACHE_INCREMENT  64

wxHtmlTagsCache::wxHtmlTagsCache(const wxString& source)
{
    const wxChar *src = source.c_str();
    int i, tg, pos, stpos;
    int lng = source.Length();
    wxChar tagBuffer[256];

    m_Cache = NULL;
    m_CacheSize = 0;
    m_CachePos = 0;

    pos = 0;
    while (pos < lng) {
        if (src[pos] == wxT('<')) {  // tag found:
            if (m_CacheSize % CACHE_INCREMENT == 0)
                m_Cache = (sCacheItem*) realloc(m_Cache, (m_CacheSize + CACHE_INCREMENT) * sizeof(sCacheItem));
            tg = m_CacheSize++;
            m_Cache[tg].Key = stpos = pos++;

            int i;
            for ( i = 0;
                  pos < lng && i < (int)WXSIZEOF(tagBuffer) - 1 &&
                  src[pos] != wxT('>') && !wxIsspace(src[pos]);
                  i++, pos++ )
            {
                tagBuffer[i] = wxToupper(src[pos]);
            }
            tagBuffer[i] = _T('\0');

            m_Cache[tg].Name = new wxChar[i+1];
            memcpy(m_Cache[tg].Name, tagBuffer, (i+1)*sizeof(wxChar));

            while (src[pos] != wxT('>')) pos++;

            if (src[stpos+1] == wxT('/')) { // ending tag:
                m_Cache[tg].End1 = m_Cache[tg].End2 = -2;
                // find matching begin tag:
                for (i = tg; i >= 0; i--)
                    if ((m_Cache[i].End1 == -1) && (wxStrcmp(m_Cache[i].Name, tagBuffer+1) == 0)) {
                        m_Cache[i].End1 = stpos;
                        m_Cache[i].End2 = pos + 1;
                        break;
                    }
            }
            else {
                m_Cache[tg].End1 = m_Cache[tg].End2 = -1;
            }
        }

        pos++;
    }

    // ok, we're done, now we'll free .Name members of cache - we don't need it anymore:
    for (i = 0; i < m_CacheSize; i++) {
        delete[] m_Cache[i].Name;
        m_Cache[i].Name = NULL;
    }
}



void wxHtmlTagsCache::QueryTag(int at, int* end1, int* end2)
{
    if (m_Cache == NULL) return;
    if (m_Cache[m_CachePos].Key != at) {
        int delta = (at < m_Cache[m_CachePos].Key) ? -1 : 1;
        do {m_CachePos += delta;} while (m_Cache[m_CachePos].Key != at);
    }
    *end1 = m_Cache[m_CachePos].End1;
    *end2 = m_Cache[m_CachePos].End2;
}




//-----------------------------------------------------------------------------
// wxHtmlTag
//-----------------------------------------------------------------------------

IMPLEMENT_CLASS(wxHtmlTag,wxObject)

wxHtmlTag::wxHtmlTag(const wxString& source, int pos, int end_pos, wxHtmlTagsCache* cache) : wxObject()
{
    int i;
    char c;

    // fill-in name, params and begin pos:
    m_Name = m_Params = wxEmptyString;
    i = pos+1;
    if (source[i] == '/') { m_Ending = TRUE; i++; }
    else m_Ending = FALSE;

    // find tag's name and convert it to uppercase:
    while ((i < end_pos) && 
               ((c = source[i++]) != ' ' && c != '\r' && c != '\n' && c != '\t' &&
                c != '>')) 
    {
        if ((c >= 'a') && (c <= 'z')) c -= ('a' - 'A');
        m_Name += c;
    }

    // if the tag has parameters, read them and "normalize" them,
    // i.e. convert to uppercase, replace whitespaces by spaces and 
    // remove whitespaces around '=':
    if (source[i-1] != '>')
        while ((i < end_pos) && ((c = source[i++]) != '>')) 
        {
            if ((c >= 'a') && (c <= 'z')) c -= ('a' - 'A');
            if (c == '\r' || c == '\n' || c == '\t') c = ' '; // make future parsing a bit simpler
            m_Params += c;
            if (c == '"') 
            {
                // remove spaces around the '=' character:
                if (m_Params.Length() > 1 && 
                    m_Params[m_Params.Length()-2] == ' ')
                {
                    m_Params.RemoveLast();
                    while (m_Params.Length() > 0 && m_Params.Last() == ' ') 
                        m_Params.RemoveLast();
                    m_Params += '"';
                }
                while ((i < end_pos) && (source[i++] == ' ')) {}
                if (i < end_pos) i--;
            
                // ...and copy the value to m_Params:
                while ((i < end_pos) && ((c = source[i++]) != '"')) m_Params += c;
                m_Params += c;
            }
            else if (c == '\'') 
            {
                while ((i < end_pos) && ((c = source[i++]) != '\'')) m_Params += c;
                m_Params += c;
            }
        }
   m_Begin = i;

   cache->QueryTag(pos, &m_End1, &m_End2);
   if (m_End1 > end_pos) m_End1 = end_pos;
   if (m_End2 > end_pos) m_End2 = end_pos;
}

    

bool wxHtmlTag::HasParam(const wxString& par) const
{
    const wxChar *st = m_Params, *p = par;
    const wxChar *st2, *p2;
    const wxChar invalid = wxT('\001');

    if (*st == 0) return FALSE;
    if (*p == 0) return FALSE;
    for (st2 = st, p2 = p; ; st2++) {
        if (*p2 == 0 && (*st2 == wxT('=') || *st2 == wxT(' '))) return TRUE;
        if (*st2 == 0) return FALSE;
        if (*p2 != *st2) p2 = &invalid;
        if (*p2 == *st2) p2++;
        if (*st2 == ' ') p2 = p;
        else if (*st2 == '=') {
            p2 = p;
            while (*st2 != ' ') {
                if (*st2 == '"') {
                    st2++;
                    while (*st2 != '"') st2++;
                }
                st2++;
                if (*st2 == 0) return FALSE;
            }
        }
    }
}



wxString wxHtmlTag::GetParam(const wxString& par, bool with_commas) const
{
    const wxChar *st = m_Params, *p = par;
    const wxChar *st2, *p2;
    const wxChar invalid = wxT('\001');
    bool comma;
    char comma_char;

    if (*st == 0) return "";
    if (*p == 0) return "";
    for (st2 = st, p2 = p; ; st2++) {
        if (*p2 == 0 && (*st2 == wxT('=') || *st2 == wxT(' '))) { // found
            if (*st2 == wxT(' ')) return wxEmptyString;
            wxString fnd = "";
            st2++; // '=' character
            comma = FALSE;
            comma_char = '\0';
            if (!with_commas && (*(st2) == '"')) {
                st2++;
                comma = TRUE; 
                comma_char = '"';
            }
            else if (!with_commas && (*(st2) == '\'')) {
                st2++; 
                comma = TRUE;
                comma_char = '\'';
            }
            while (*st2 != 0) {
                if (comma && *st2 == comma_char) comma = FALSE;
                else if ((*st2 == ' ') && (!comma)) break;
                fnd += (*(st2++));
            }
            if (!with_commas && (*(st2-1) == comma_char)) fnd.RemoveLast();
            return fnd;
        }
        if (*st2 == 0) return "";
        if (*p2 != *st2) p2 = &invalid;
        if (*p2 == *st2) p2++;
        if (*st2 == ' ') p2 = p;
        else if (*st2 == '=') {
            p2 = p;
            while (*st2 != ' ') {
                if (*st2 == '"') {
                    st2++;
                    while (*st2 != '"') st2++;
                }
                else if (*st2 == '\'') {
                    st2++;
                    while (*st2 != '\'') st2++;
                }
                st2++;
            }
        }
    }
}



int wxHtmlTag::ScanParam(const wxString& par, wxChar *format, void *param) const
{
    wxString parval = GetParam(par);
    return wxSscanf((const wxChar*)parval, format, param);
}

#endif
