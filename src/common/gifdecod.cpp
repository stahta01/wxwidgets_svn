/////////////////////////////////////////////////////////////////////////////
// Name:        gifdecod.cpp
// Purpose:     wxGIFDecoder, GIF reader for wxImage and wxAnimation
// Author:      Guillermo Rodriguez Garcia <guille@iies.es>
// Version:     3.04
// RCS-ID:      $Id$
// Copyright:   (c) Guillermo Rodriguez Garcia
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "gifdecod.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#  pragma hdrstop
#endif

#ifndef WX_PRECOMP
#  include "wx/defs.h"
#endif

#if wxUSE_STREAMS && wxUSE_GIF

#include <stdlib.h>
#include <string.h>
#include "wx/gifdecod.h"


//---------------------------------------------------------------------------
// wxGIFDecoder constructor and destructor
//---------------------------------------------------------------------------

wxGIFDecoder::wxGIFDecoder(wxInputStream *s, bool anim)
{
    m_f    = s;
    m_anim = anim;

    m_background = -1;
    m_screenw = 0;
    m_screenh = 0;

    m_pimage  = NULL;
    m_pfirst  = NULL;
    m_plast   = NULL;
    m_image   = 0;
    m_nimages = 0;
}

wxGIFDecoder::~wxGIFDecoder()
{
    Destroy();
}

void wxGIFDecoder::Destroy()
{
    GIFImage *pimg, *paux;

    pimg = m_pfirst;

    while (pimg != NULL)
    {
        paux = pimg->next;
        free(pimg->p);
        free(pimg->pal);
        delete pimg;
        pimg = paux;
    }

    m_pimage  = NULL;
    m_pfirst  = NULL;
    m_plast   = NULL;
    m_image   = 0;
    m_nimages = 0;
}


//---------------------------------------------------------------------------
// Convert this image to a wxImage object
//---------------------------------------------------------------------------

// This function was designed by Vaclav Slavik

bool wxGIFDecoder::ConvertToImage(wxImage *image) const
{
    unsigned char *src, *dst, *pal;
    unsigned long i;
    int      transparent;

    /* just in case... */
    image->Destroy();

    /* create the image */
    image->Create(GetWidth(), GetHeight());

    if (!image->Ok())
        return FALSE;

    pal = GetPalette();
    src = GetData();
    dst = image->GetData();
    transparent = GetTransparentColour();

    /* set transparent colour mask */
    if (transparent != -1)
    {
        for (i = 0; i < 256; i++)
        {
            if ((pal[3 * i + 0] == 255) &&
                (pal[3 * i + 1] == 0) &&
                (pal[3 * i + 2] == 255))
            {
                pal[3 * i + 2] = 254;
            }
        }

        pal[3 * transparent + 0] = 255,
        pal[3 * transparent + 1] = 0,
        pal[3 * transparent + 2] = 255;

        image->SetMaskColour(255, 0, 255);
    }
    else
        image->SetMask(FALSE);

    if (pal)
    {
        unsigned char* r = new unsigned char[256];
        unsigned char* g = new unsigned char[256];
        unsigned char* b = new unsigned char[256];
        for (i = 0; i < 256; i++)
        {
            r[i] = pal[3*i + 0];
            g[i] = pal[3*i + 1];
            b[i] = pal[3*i + 2];
        }
        image->SetPalette(wxPalette(256, r, g, b));
        delete[] r; delete[] g; delete[] b;
    }

    /* copy image data */
    for (i = 0; i < (GetWidth() * GetHeight()); i++, src++)
    {
        *(dst++) = pal[3 * (*src) + 0];
        *(dst++) = pal[3 * (*src) + 1];
        *(dst++) = pal[3 * (*src) + 2];
    }

    return TRUE;
}


//---------------------------------------------------------------------------
// Data accessors
//---------------------------------------------------------------------------

// Get data for current frame

int wxGIFDecoder::GetFrameIndex() const         { return m_image; }
unsigned char* wxGIFDecoder::GetData() const    { return (m_pimage->p); }
unsigned char* wxGIFDecoder::GetPalette() const { return (m_pimage->pal); }
unsigned int wxGIFDecoder::GetWidth() const     { return (m_pimage->w); }
unsigned int wxGIFDecoder::GetHeight() const    { return (m_pimage->h); }
unsigned int wxGIFDecoder::GetTop() const       { return (m_pimage->top); }
unsigned int wxGIFDecoder::GetLeft() const      { return (m_pimage->left); }
int wxGIFDecoder::GetTransparentColour() const  { return (m_pimage->transparent); }
int wxGIFDecoder::GetDisposalMethod() const     { return (m_pimage->disposal); }
long wxGIFDecoder::GetDelay() const             { return (m_pimage->delay); }

// Get global data

unsigned int wxGIFDecoder::GetLogicalScreenWidth() const    { return m_screenw; }
unsigned int wxGIFDecoder::GetLogicalScreenHeight() const   { return m_screenh; }
int wxGIFDecoder::GetBackgroundColour() const   { return m_background; }
int wxGIFDecoder::GetNumberOfFrames() const     { return m_nimages; }
bool wxGIFDecoder::IsAnimation() const          { return (m_nimages > 1); }


//---------------------------------------------------------------------------
// Functions to move through the animation
//---------------------------------------------------------------------------

bool wxGIFDecoder::GoFirstFrame()
{
    if (!IsAnimation())
        return FALSE;

    m_image = 1;
    m_pimage = m_pfirst;
    return TRUE;
}

bool wxGIFDecoder::GoLastFrame()
{
    if (!IsAnimation())
        return FALSE;

    m_image = m_nimages;
    m_pimage = m_plast;
    return TRUE;
}

bool wxGIFDecoder::GoNextFrame(bool cyclic)
{
    if (!IsAnimation())
        return FALSE;

    if ((m_image < m_nimages) || (cyclic))
    {
        m_pimage = m_pimage->next;
        m_image++;

        if (!m_pimage)
        {
            m_image = 1;
            m_pimage = m_pfirst;
        }

        return TRUE;
    }
    else
        return FALSE;
}

bool wxGIFDecoder::GoPrevFrame(bool cyclic)
{
    if (!IsAnimation())
        return FALSE;

    if ((m_image > 1) || (cyclic))
    {
        m_pimage = m_pimage->prev;
        m_image--;

        if (!m_pimage)
        {
            m_image = m_nimages;
            m_pimage = m_plast;
        }

        return TRUE;
    }
    else
        return FALSE;
}

bool wxGIFDecoder::GoFrame(int which)
{
    int i;

    if (!IsAnimation())
        return FALSE;

    if ((which >= 1) && (which <= m_nimages))
    {
        m_pimage = m_pfirst;

        for (i = 1; i < which; i++)
            m_pimage = m_pimage->next;

        return TRUE;
    }
    else
        return FALSE;
}


//---------------------------------------------------------------------------
// GIF reading and decoding
//---------------------------------------------------------------------------

// getcode:
//  Reads the next code from the file stream, with size 'bits'
//
int wxGIFDecoder::getcode(int bits, int ab_fin)
{
    unsigned int mask;          /* bit mask */
    unsigned int code;          /* code (result) */


    /* get remaining bits from last byte read */
    mask = (1 << bits) - 1;
    code = (m_lastbyte >> (8 - m_restbits)) & mask;

    /* keep reading new bytes while needed */
    while (bits > m_restbits)
    {
        /* if no bytes left in this block, read the next block */
        if (m_restbyte == 0)
        {
            m_restbyte = (unsigned char)m_f->GetC();

            /* Some encoders are a bit broken: instead of issuing
             * an end-of-image symbol (ab_fin) they come up with
             * a zero-length subblock!! We catch this here so
             * that the decoder sees an ab_fin code.
             */
            if (m_restbyte == 0)
            {
                code = ab_fin;
                break;
            }

            /* prefetch data */
            m_f->Read((void *) m_buffer, m_restbyte);
            m_bufp = m_buffer;
        }

        /* read next byte and isolate the bits we need */
        m_lastbyte = (unsigned char) (*m_bufp++);
        mask       = (1 << (bits - m_restbits)) - 1;
        code       = code + ((m_lastbyte & mask) << m_restbits);
        m_restbyte--;

        /* adjust total number of bits extracted from the buffer */
        m_restbits = m_restbits + 8;
    }

    /* find number of bits remaining for next code */
    m_restbits = (m_restbits - bits);

    return code;
}


// dgif:
//  GIF decoding function. The initial code size (aka root size)
//  is 'bits'. Supports interlaced images (interl == 1).
//
int wxGIFDecoder::dgif(GIFImage *img, int interl, int bits)
{
#ifdef __WXMAC__
    int *ab_prefix = new int[4096];        /* alphabet (prefixes) */
    int *ab_tail = new int[4096];          /* alphabet (tails) */
    int *stack = new int[4096];            /* decompression stack */
#else
    int ab_prefix[4096];        /* alphabet (prefixes) */
    int ab_tail[4096];          /* alphabet (tails) */
    int stack[4096];            /* decompression stack */
#endif
    int ab_clr;                 /* clear code */
    int ab_fin;                 /* end of info code */
    int ab_bits;                /* actual symbol width, in bits */
    int ab_free;                /* first free position in alphabet */
    int ab_max;                 /* last possible character in alphabet */
    int pass;                   /* pass number in interlaced images */
    int pos;                    /* index into decompresion stack */
    unsigned int x, y;          /* position in image buffer */

    int code, readcode, lastcode, abcabca;

    /* these won't change */
    ab_clr = (1 << bits);
    ab_fin = (1 << bits) + 1;

    /* these will change through the decompression proccess */
    ab_bits  = bits + 1;
    ab_free  = (1 << bits) + 2;
    ab_max   = (1 << ab_bits) - 1;
    lastcode = -1;
    abcabca  = -1;
    pass     = 1;
    pos = x = y = 0;

    /* reset decoder vars */
    m_restbits = 0;
    m_restbyte = 0;
    m_lastbyte = 0;

    do
    {
        /* get next code */
        readcode = code = getcode(ab_bits, ab_fin);

        /* end of image? */
        if (code == ab_fin) break;

        /* reset alphabet? */
        if (code == ab_clr)
        {
            /* reset main variables */
            ab_bits  = bits + 1;
            ab_free  = (1 << bits) + 2;
            ab_max   = (1 << ab_bits) - 1;
            lastcode = -1;
            abcabca  = -1;

            /* skip to next code */
            continue;
        }

        /* unknown code: special case (like in ABCABCA) */
        if (code >= ab_free)
        {
            code = lastcode;            /* take last string */
            stack[pos++] = abcabca;     /* add first character */
        }

        /* build the string for this code in the stack */
        while (code > ab_clr)
        {
            stack[pos++] = ab_tail[code];
            code         = ab_prefix[code];
        }
        stack[pos] = code;              /* push last code into the stack */
        abcabca    = code;              /* save for special case */

        /* make new entry in alphabet (only if NOT just cleared) */
        if (lastcode != -1)
        {
            ab_prefix[ab_free] = lastcode;
            ab_tail[ab_free]   = code;
            ab_free++;

            if ((ab_free > ab_max) && (ab_bits < 12))
            {
                ab_bits++;
                ab_max = (1 << ab_bits) - 1;
            }
        }

        /* dump stack data to the buffer */
        while (pos >= 0)
        {
            (img->p)[x + (y * (img->w))] = (char)stack[pos--];

            if (++x >= (img->w))
            {
                x = 0;

                if (interl)
                {
                    /* support for interlaced images */
                    switch (pass)
                    {
                        case 1: y += 8; break;
                        case 2: y += 8; break;
                        case 3: y += 4; break;
                        case 4: y += 2; break;
                    }
                    if (y >= (img->h))
                    {
                        switch (++pass)
                        {
                            case 2: y = 4; break;
                            case 3: y = 2; break;
                            case 4: y = 1; break;
                        }
                    }
                }
                else
                {
                    /* non-interlaced */
                    y++;
                }
            }
        }

        pos = 0;
        lastcode = readcode;
    }
    while (code != ab_fin);

#ifdef __WXMAC__
    delete [] ab_prefix ;       
    delete [] ab_tail ;       
    delete [] stack ;       
#endif
    return 0;
}


// CanRead:
//  Returns TRUE if the file looks like a valid GIF, FALSE otherwise.
//
bool wxGIFDecoder::CanRead()
{
    unsigned char buf[3];

    m_f->Read(buf, 3);
    m_f->SeekI(-3, wxFromCurrent);

    return (memcmp(buf, "GIF", 3) == 0);
}


// ReadGIF:
//  Reads and decodes one or more GIF images, depending on whether
//  animated GIF support is enabled. Can read GIFs with any bit
//  size (color depth), but the output images are always expanded
//  to 8 bits per pixel. Also, the image palettes always contain
//  256 colors, although some of them may be unused. Returns wxGIF_OK
//  (== 0) on success, or an error code if something fails (see
//  header file for details)
//
int wxGIFDecoder::ReadGIF()
{
    int           ncolors, bits, interl, transparent, disposal, i;
    long          size;
    long          delay;
    unsigned char type = 0;
    unsigned char pal[768];
    unsigned char buf[16];
    GIFImage      **ppimg, *pimg, *pprev;

    /* check GIF signature */
    if (!CanRead())
        return wxGIF_INVFORMAT;

    /* check for animated GIF support (ver. >= 89a) */
    m_f->Read(buf, 6);

    if (memcmp(buf + 3, "89a", 3) < 0)
        m_anim = FALSE;

    /* read logical screen descriptor block (LSDB) */
    m_f->Read(buf, 7);
    m_screenw = buf[0] + 256 * buf[1];
    m_screenh = buf[2] + 256 * buf[3];

    /* load global color map if available */
    if ((buf[4] & 0x80) == 0x80)
    {
        m_background = buf[5];

        ncolors = 2 << (buf[4] & 0x07);
        m_f->Read(pal, 3 * ncolors);
    }

    /* transparent colour, disposal method and delay default to unused */
    transparent = -1;
    disposal = -1;
    delay = -1;

    /* read images */
    ppimg = &m_pfirst;
    pprev = NULL;
    pimg  = NULL;

    bool done = FALSE;

    while(!done)
    {
        type = (unsigned char)m_f->GetC();

        /* end of data? */
        if (type == 0x3B)
        {
            done = TRUE;
        }
        else
        /* extension block? */
        if (type == 0x21)
        {
            if (((unsigned char)m_f->GetC()) == 0xF9)
            /* graphics control extension, parse it */
            {
                m_f->Read(buf, 6);

                /* read delay and convert from 1/100 of a second to ms */
                delay = 10 * (buf[2] + 256 * buf[3]);

                /* read transparent colour index, if used */
                if (buf[1] & 0x01)
                    transparent = buf[4];

                /* read disposal method */
                disposal = (buf[1] & 0x1C) - 1;
            }
            else
            /* other extension, skip */
            {
                while ((i = (unsigned char)m_f->GetC()) != 0)
                {
                    m_f->SeekI(i, wxFromCurrent);
                }
            }
        }
        else
        /* image descriptor block? */
        if (type == 0x2C)
        {
            /* allocate memory for IMAGEN struct */
            pimg = (*ppimg) = new GIFImage();

            if (pimg == NULL)
            {
                Destroy();
                return wxGIF_MEMERR;
            }

            /* fill in the data */
            m_f->Read(buf, 9);
            pimg->left = buf[4] + 256 * buf[5];
            pimg->top = buf[4] + 256 * buf[5];
            pimg->w = buf[4] + 256 * buf[5];
            pimg->h = buf[6] + 256 * buf[7];
            interl = ((buf[8] & 0x40)? 1 : 0);
            size = pimg->w * pimg->h;

            pimg->transparent = transparent;
            pimg->disposal = disposal;
            pimg->delay = delay;
            pimg->next = NULL;
            pimg->prev = pprev;
            pprev = pimg;
            ppimg = &pimg->next;

            /* allocate memory for image and palette */
            pimg->p   = (unsigned char *) malloc((size_t)size);
            pimg->pal = (unsigned char *) malloc(768);

            if ((!pimg->p) || (!pimg->pal))
            {
                Destroy();
                return wxGIF_MEMERR;
            }

            /* load local color map if available, else use global map */
            if ((buf[8] & 0x80) == 0x80)
            {
                ncolors = 2 << (buf[8] & 0x07);
                m_f->Read(pimg->pal, 3 * ncolors);
            }
            else
                memcpy(pimg->pal, pal, 768);

            /* get initial code size from first byte in raster data */
            bits = (unsigned char)m_f->GetC();

            /* decode image */
            dgif(pimg, interl, bits);
            m_nimages++;

            /* if this is not an animated GIF, exit after first image */
            if (!m_anim)
                done = TRUE;
        }
    }

    /* setup image pointers */
    if (m_nimages != 0)
    {
        m_image = 1;
        m_plast = pimg;
        m_pimage = m_pfirst;
    }

    /* try to read to the end of the stream */
    while (type != 0x3B)
    {
        type = (unsigned char)m_f->GetC();

        if (type == 0x21)
        {
            /* extension type */
            (void) m_f->GetC();

            /* skip all data */
            while ((i = (unsigned char)m_f->GetC()) != 0)
            {
                m_f->SeekI(i, wxFromCurrent);
            }
        }
        else if (type == 0x2C)
        {
            /* image descriptor block */
            m_f->Read(buf, 9);

            /* local color map */
            if ((buf[8] & 0x80) == 0x80)
            {
                ncolors = 2 << (buf[8] & 0x07);
                m_f->SeekI(3 * ncolors, wxFromCurrent);
            }

            /* initial code size */
            (void) m_f->GetC();

            /* skip all data */
            while ((i = (unsigned char)m_f->GetC()) != 0)
            {
                m_f->SeekI(i, wxFromCurrent);
            }
        }
        else if ((type != 0x3B) && (type != 00)) /* testing */
        {
            /* images are OK, but couldn't read to the end of the stream */
            return wxGIF_TRUNCATED;
        }
    }

    return wxGIF_OK;
}

#endif // wxUSE_STREAMS && wxUSE_GIF
