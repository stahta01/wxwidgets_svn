/////////////////////////////////////////////////////////////////////////////
// Name:        imagepng.cpp
// Purpose:     wxImage PNG handler
// Author:      Robert Roebling
// RCS-ID:      $Id$
// Copyright:   (c) Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "image.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include "wx/image.h"
#include "wx/bitmap.h"
#include "wx/debug.h"
#include "wx/log.h"
#include "wx/app.h"
#if wxUSE_LIBPNG
#include "../png/png.h"
#endif
#include "wx/filefn.h"
#include "wx/wfstream.h"
#include "wx/intl.h"
#include "wx/module.h"

// For memcpy
#include <string.h>

#ifdef __SALFORDC__
#ifdef FAR
#undef FAR
#endif
#endif

#ifdef __WXMSW__
#include <windows.h>
#endif

//-----------------------------------------------------------------------------
// wxPNGHandler
//-----------------------------------------------------------------------------

#if wxUSE_LIBPNG

#if !USE_SHARED_LIBRARIES
IMPLEMENT_DYNAMIC_CLASS(wxPNGHandler,wxImageHandler)
#endif


#if wxUSE_STREAMS
static void _PNG_stream_reader( png_structp png_ptr, png_bytep data, png_size_t length )
{
    ((wxInputStream*) png_get_io_ptr( png_ptr )) -> Read(data, length);
}

static void _PNG_stream_writer( png_structp png_ptr, png_bytep data, png_size_t length )
{
    ((wxOutputStream*) png_get_io_ptr( png_ptr )) -> Write(data, length);
}

bool wxPNGHandler::LoadFile( wxImage *image, wxInputStream& stream )
{
    // VZ: as this function uses setjmp() the only fool proof error handling
    //     method is to use goto (setjmp is not really C++ dtors friendly...)
    
    unsigned char **lines = (unsigned char **) NULL;
    unsigned int i;
    png_infop info_ptr = (png_infop) NULL;
    
    image->Destroy();
    
    png_structp png_ptr = png_create_read_struct( PNG_LIBPNG_VER_STRING,
        (voidp) NULL,
        (png_error_ptr) NULL,
        (png_error_ptr) NULL );
    if (!png_ptr)
        goto error;
    
    info_ptr = png_create_info_struct( png_ptr );
    if (!info_ptr)
        goto error;
    
    if (setjmp(png_ptr->jmpbuf))
        goto error;
    
    if (info_ptr->color_type == PNG_COLOR_TYPE_RGB_ALPHA)
        goto error;
    
    png_set_read_fn( png_ptr, &stream, _PNG_stream_reader);
    
    png_uint_32 width,height;
    int bit_depth,color_type,interlace_type;
    
    png_read_info( png_ptr, info_ptr );
    png_get_IHDR( png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, (int*) NULL, (int*) NULL );
    
    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_expand( png_ptr );
    
    png_set_strip_16( png_ptr );
    png_set_packing( png_ptr );
    if (png_get_valid( png_ptr, info_ptr, PNG_INFO_tRNS))
        png_set_expand( png_ptr );
    png_set_filler( png_ptr, 0xff, PNG_FILLER_AFTER );
    
    image->Create( width, height );
    
    if (!image->Ok())
        goto error;
    
    lines = (unsigned char **)malloc( height * sizeof(unsigned char *) );
    if (lines == NULL)
        goto error;
    
    for (i = 0; i < height; i++)
    {
        if ((lines[i] = (unsigned char *)malloc(width * (sizeof(unsigned char) * 4))) == NULL)
        {
            for ( unsigned int n = 0; n < i; n++ )
                free( lines[n] );
            goto error;
        }
    }
    
    // loaded successfully!
    {
        int transp = 0;
        png_read_image( png_ptr, lines );
        png_destroy_read_struct( &png_ptr, &info_ptr, (png_infopp) NULL );
        unsigned char *ptr = image->GetData();
        if ((color_type == PNG_COLOR_TYPE_GRAY) ||
            (color_type == PNG_COLOR_TYPE_GRAY_ALPHA))
        {
            for (unsigned int y = 0; y < height; y++)
            {
                unsigned char *ptr2 = lines[y];
                for (unsigned int x = 0; x < width; x++)
                {
                    unsigned char r = *ptr2++;
                    unsigned char a = *ptr2++;
                    if (a < 128)
                    {
                        *ptr++ = 255;
                        *ptr++ = 0;
                        *ptr++ = 255;
                        transp = 1;
                    }
                    else
                    {
                        *ptr++ = r;
                        *ptr++ = r;
                        *ptr++ = r;
                    }
                }
            }
        }
        else
        {
            for (unsigned int y = 0; y < height; y++)
            {
                unsigned char *ptr2 = lines[y];
                for (unsigned int x = 0; x < width; x++)
                {
                    unsigned char r = *ptr2++;
                    unsigned char g = *ptr2++;
                    unsigned char b = *ptr2++;
                    unsigned char a = *ptr2++;
                    if (a < 128)
                    {
                        *ptr++ = 255;
                        *ptr++ = 0;
                        *ptr++ = 255;
                        transp = 1;
                    }
                    else
                    {
                        if ((r == 255) && (g == 0) && (b == 255)) r = 254;
                        *ptr++ = r;
                        *ptr++ = g;
                        *ptr++ = b;
                    }
                }
            }
        }
        
        for ( unsigned int j = 0; j < height; j++ )
            free( lines[j] );
        free( lines );
        
        if (transp)
        {
            image->SetMaskColour( 255, 0, 255 );
        }
        else
        {
            image->SetMask( FALSE );
        }
    }
    
    return TRUE;
    
error:
    wxLogError(_("Couldn't load a PNG image - probably file is corrupted."));
    
    if ( image->Ok() )
    {
        image->Destroy();
    }
    
    if ( lines )
    {
        free( lines );
    }
    
    if ( png_ptr )
    {
        if ( info_ptr )
        {
            png_destroy_read_struct( &png_ptr, &info_ptr, (png_infopp) NULL );
            free(info_ptr);
        }
        else
            png_destroy_read_struct( &png_ptr, (png_infopp) NULL, (png_infopp) NULL );
    }
    return FALSE;
}


bool wxPNGHandler::SaveFile( wxImage *image, wxOutputStream& stream )
{
    {
        png_structp png_ptr = png_create_write_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png_ptr)
        {
            return FALSE;
        }
        
        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (info_ptr == NULL)
        {
            png_destroy_write_struct( &png_ptr, (png_infopp)NULL );
            return FALSE;
        }
        
        if (setjmp(png_ptr->jmpbuf))
        {
            png_destroy_write_struct( &png_ptr, (png_infopp)NULL );
            return FALSE;
        }
        
        png_set_write_fn( png_ptr, &stream, _PNG_stream_writer, NULL);
        
        png_set_IHDR( png_ptr, info_ptr, image->GetWidth(), image->GetHeight(), 8,
            PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
        
        png_color_8 sig_bit;
        sig_bit.red = 8;
        sig_bit.green = 8;
        sig_bit.blue = 8;
        sig_bit.alpha = 8;
        png_set_sBIT( png_ptr, info_ptr, &sig_bit );
        png_write_info( png_ptr, info_ptr );
        png_set_shift( png_ptr, &sig_bit );
        png_set_packing( png_ptr );
        
        unsigned char *data = (unsigned char *)malloc( image->GetWidth()*4 );
        if (!data)
        {
            png_destroy_write_struct( &png_ptr, (png_infopp)NULL );
            return FALSE;
        }
        
        for (int y = 0; y < image->GetHeight(); y++)
        {
            unsigned char *ptr = image->GetData() + (y * image->GetWidth() * 3);
            for (int x = 0; x < image->GetWidth(); x++)
            {
                data[(x << 2) + 0] = *ptr++;
                data[(x << 2) + 1] = *ptr++;
                data[(x << 2) + 2] = *ptr++;
                if ((data[(x << 2) + 0] == image->GetMaskRed()) &&
                    (data[(x << 2) + 1] == image->GetMaskGreen()) &&
                    (data[(x << 2) + 2] == image->GetMaskBlue()))
                {
                    data[(x << 2) + 3] = 0;
                }
                else
                {
                    data[(x << 2) + 3] = 255;
                }
            }
            png_bytep row_ptr = data;
            png_write_rows( png_ptr, &row_ptr, 1 );
        }
        
        free(data);
        png_write_end( png_ptr, info_ptr );
        png_destroy_write_struct( &png_ptr, (png_infopp)&info_ptr );
    }
    return TRUE;
}
#endif // wxUSE_STREAMS

#endif

// wxUSE_LIBPNG

