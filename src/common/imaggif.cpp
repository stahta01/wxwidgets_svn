/////////////////////////////////////////////////////////////////////////////
// Name:        imaggif.cpp
// Purpose:     wxGIFHandler
// Author:      Guillermo Rodriguez Garcia
//              wxWindows adaptation by Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "imaggif.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include <wx/wx.h>

#include <wx/image.h>
#include <wx/wfstream.h>
#include <wx/module.h>

#include <wx/imaggif.h>

/*

FOLLOWING CODE IS BY G.R.G. :
(except wxInputStream stuff)

*/

/************************************************************************
  READGIF.H - Leer un archivo GIF de 8 bits
 ------------------------------------------------------------------------
  Tratamiento Digital de la Imagen
 ------------------------------------------------------------------------
  Guillermo Rodriguez Garcia
  <guille@iies.es>

  Version: 2.0
*************************************************************************/

typedef struct
{
    int w;                  /* width */
    int h;                  /* height */
    unsigned char *p;       /* bitmap */
    unsigned char *pal;     /* palette */
} IMAGEN;


/************************************************************************
  READGIF.C - Lee un archivo GIF de 256 colores
 ------------------------------------------------------------------------
  Tratamiento Digital de la Imagen
 ------------------------------------------------------------------------
  Guillermo Rodriguez Garcia
  <guille@iies.es>

  Version: 2.0
*************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* error codes */
#define E_OK        0               /* everything was OK */
#define E_ARCHIVO   -1              /* error opening file */
#define E_FORMATO   -2              /* error in gif header */
#define E_MEMORIA   -3              /* error allocating memory */


/* This class binding is by VS, so all bugs in it are mine ;-) */

class gifDecoder
{
    private:
        /* globals */
        int            restbits;            /* remaining valid bits */
        unsigned int   restbyte;            /* remaining bytes in this block */
        unsigned int   lastbyte;            /* last byte read */

        unsigned char* file;                /* input file in memory */
        unsigned int   file_pos, file_size; /* position & size in it */

        int fgetc();
        void fread(void *ptr, size_t size, size_t nmemb);
        void fseekcur(int rel_pos);

    public:
        gifDecoder(unsigned char* mem, int sz) {file = mem; file_pos = 0; file_size = sz;}
        int getcode(int bits);
        int dgif(IMAGEN *img, int interl, int bits);
        int readgif(IMAGEN *img);
};


int gifDecoder::fgetc()
{
    if (file_pos < file_size) return file[file_pos++];
    else return EOF;
}

void gifDecoder::fread(void *ptr, size_t size, size_t nmemb)
{
    int todo = size * nmemb;
    if (todo + file_pos > file_size) todo = file_size - file_pos;
    memcpy(ptr, file + file_pos, todo);
    file_pos += todo;
}

void gifDecoder::fseekcur(int rel_pos)
{
    file_pos += rel_pos;
}

/* getcode:
 *  Reads the next code from the file, with size 'bits'
 *  v2.0 - changed to support 'bits' values < 8
 */
int gifDecoder::getcode(int bits)
{
    unsigned int    mask;           /* bit mask */
    unsigned int    code;           /* code (result) */


    /* get remaining bits from last byte read */
    mask = (1 << bits) - 1;
    code = (lastbyte >> (8 - restbits)) & mask;

    /* keep reading new bytes until needed */
    while (bits > restbits)
    {
        /* if no bytes left in this block, read the next block */
        if (restbyte == 0)
            restbyte = fgetc();

        /* read next byte and isolate the bits we need */
        lastbyte = fgetc();
        mask     = (1 << (bits - restbits)) - 1;
        code     = code + ((lastbyte & mask) << restbits);
        restbyte--;

        /* adjust total number of bits extracted from the buffer */
        restbits = restbits + 8;
    }

    /* find number of bits reamining for next code */
    restbits = (restbits - bits);

    return code;
}

/* dgif:
 *  GIF decoding function. The initial code size (aka root size)
 *  is 'bits'. Supports interlaced images (interl == 1).
 */
int gifDecoder::dgif(IMAGEN *img, int interl, int bits)
{
    int ab_prefix[4096];        /* alphabet (prefixes) */
    int ab_tail[4096];          /* alphabet (tails) */
    int stack[4096];            /* decompression stack */

    int ab_clr;                 /* clear code */
    int ab_fin;                 /* end of info code */
    int ab_bits;                /* actual symbol width, in bits */
    int ab_free;                /* first free position in alphabet */
    int ab_max;                 /* last possible character in alphabet */
    int pass;                   /* pass number in interlaced images */
    int pos;                    /* index into decompresion stack */
    int x, y;                   /* position in image buffer */

    int code, readcode, lastcode, abcabca;

    /* these won't change */
    ab_clr  = (1 << bits);
    ab_fin  = (1 << bits) + 1;

    /* these will change through the decompression proccess */
    ab_bits  = bits + 1;
    ab_free  = (1 << bits) + 2;
    ab_max   = (1 << ab_bits) - 1;
    lastcode = -1;
    abcabca  = -1;
    pass     = 1;
    pos = x = y = 0;

    /* reset static globals */
    restbits = 0;
    restbyte = 0;
    lastbyte = 0;

    do
    {
        /* get next code */
        readcode = code = getcode(ab_bits);

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

    return 0;
}

/* readgif:
 *  Reads a GIF image from the file with filename 'nombre' in the
 *  IMAGEN structure pointed by 'img'. Can read GIFs with any bit
 *  size (color depth), but the output image is always expanded
 *  to 8 bits per pixel. Also, the image palette always contains
 *  256 colors, although some of them may be unused. Returns E_OK
 *  (== 0) on success, or an error code if something fails. Error
 *  codes are E_ARCHIVO, E_FORMATO, E_MEMORIA (see above).
 */
int gifDecoder::readgif(IMAGEN *img)
{
    int             size, ncolors, bits, interl, i;
    unsigned char   pal[768];
    unsigned char   buf[16];


    /* read header and logical screen descriptor block (LSDB) */
    fread(buf, 1, 13);

    /* check GIF signature */
    if (memcmp(buf, "GIF", 3) != 0) return E_FORMATO;

    /* load global color map if available */
    if ((buf[10] & 0x80) == 0x80)
    {
        ncolors = 2 << (buf[10] & 0x07);
        fread(pal, 1, 3 * ncolors);
    }

    /* skip extensions */
    while (fgetc() == 0x21)            /* separator */
    {
        fgetc();                       /* function code */

        while ((i = fgetc()) != 0)     /* byte count */
            fseekcur(i);
    }

    /* read image descriptor block (IDB) */
    fread(buf, 1, 9);
    img->w = buf[4] + 256 * buf[5];

    img->h = buf[6] + 256 * buf[7];
    size   = img->w * img->h;
    interl = ((buf[8] & 0x40)? 1 : 0);

    /* load local color map if available */
    if ((buf[8] & 0x80) == 0x80)
    {
        ncolors = 2 << (buf[8] & 0x07);
        fread(pal, 1, 3 * ncolors);
    }

    /* get initial code size from first byte in raster data */
    bits = fgetc();

    /* allocate memory for image and palette */
    if ((img->p   = (unsigned char*) malloc(size)) == NULL) return E_MEMORIA;
    if ((img->pal = (unsigned char*) malloc(768))  == NULL) return E_MEMORIA;

    /* shift palette to fit VGA 6-bit format */
    for (i = 0; i < 768; i++)
        (img->pal)[i] = (unsigned char)pal[i] /* >> 2 not needed under wxWin */;

    /* decode GIF */
    dgif(img, interl, bits);

    /* finish successfully :-) */
    return E_OK;
}

/*

FOLLOWING CODE IS BY V.S. :

*/

//-----------------------------------------------------------------------------
// wxGIFHandler
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxGIFHandler,wxImageHandler)

bool wxGIFHandler::LoadFile( wxImage *image, wxInputStream& stream )
{
    unsigned char *ptr, *src, *pal;
    int file_size;
    unsigned char* file_con;
    IMAGEN igif;
    int i;
    gifDecoder *decod;

    image->Destroy();

    file_size = stream.StreamSize();
    file_con = (unsigned char*) malloc(file_size);
    stream.Read(file_con, file_size);
    decod = new gifDecoder(file_con, file_size);

    if (decod -> readgif(&igif) != E_OK) {
        wxLogDebug("Error reading GIF");
        free(file_con);
        delete decod;
        return FALSE;
    }
    free(file_con);
    delete decod;

    image->Create(igif.w, igif.h);
    if (!image->Ok()) {
        free(igif.pal);
        free(igif.p);
        return FALSE;
    }
    image->SetMask(FALSE);

    ptr = image->GetData();
    src = igif.p;
    pal = igif.pal;
    for (i = 0; i < igif.w * igif.h; i++, src++) {
        *(ptr++) = pal[3 * (*src) + 0];
        *(ptr++) = pal[3 * (*src) + 1];
        *(ptr++) = pal[3 * (*src) + 2];
    }

    free(igif.pal);
    free(igif.p);
    return TRUE;
}

bool wxGIFHandler::SaveFile( wxImage *image, wxOutputStream& stream )
{
    wxLogDebug("wxGIFHandler is read-only!!");
    return FALSE;
}

//////////////////// Module:

/* We haven't yet decided to go for the wxModule approach or
 * explicit handler-adding: assuming the latter for now -- JACS

class wxGIFModule : public wxModule
{
    DECLARE_DYNAMIC_CLASS(wxGIFModule)

    public:
        virtual bool OnInit()
        {
            wxImage::AddHandler(new wxGIFHandler);
            return TRUE;
        }
        virtual void OnExit() {}
};

IMPLEMENT_DYNAMIC_CLASS(wxGIFModule, wxModule)

*/


