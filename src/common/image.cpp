/////////////////////////////////////////////////////////////////////////////
// Name:        image.cpp
// Purpose:     wxImage
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
#include "../png/png.h"
#include "wx/filefn.h"

#ifdef __WXGTK__
#include "gdk/gdkprivate.h"
#include "gdk/gdkx.h"
#endif

//-----------------------------------------------------------------------------
// wxImage
//-----------------------------------------------------------------------------

class wxImageRefData: public wxObjectRefData
{

public:
  wxImageRefData(void);
  ~wxImageRefData(void);
  
  int             m_width;
  int             m_height;
  unsigned char  *m_data;
  bool            m_hasMask;
  unsigned char   m_maskRed,m_maskGreen,m_maskBlue;
  bool            m_ok;
};

wxImageRefData::wxImageRefData(void)
{
    m_width = 0;
    m_height = 0;
    m_data = (unsigned char*) NULL;
    m_ok = FALSE;
    m_maskRed = 0;
    m_maskGreen = 0;
    m_maskBlue = 0;
    m_hasMask = FALSE;
}

wxImageRefData::~wxImageRefData(void)
{
    if (m_data) free( m_data );
}

wxList wxImage::sm_handlers;

//-----------------------------------------------------------------------------

#define M_IMGDATA ((wxImageRefData *)m_refData)

#if !USE_SHARED_LIBRARIES
IMPLEMENT_DYNAMIC_CLASS(wxImage, wxObject)
#endif

wxImage::wxImage()
{
}

wxImage::wxImage( int width, int height )
{
    Create( width, height );
}

wxImage::wxImage( const wxString& name, long type )
{
    LoadFile( name, type );
}

wxImage::wxImage( const wxImage& image )  
{  
    Ref(image); 
}

wxImage::wxImage( const wxImage* image )  
{ 
    if (image) Ref(*image); 
}

void wxImage::Create( int width, int height )
{
    m_refData = new wxImageRefData();
  
    M_IMGDATA->m_data = (unsigned char *) malloc( width*height*3 );
    if (M_IMGDATA->m_data)
    {
        for (int l = 0; l < width*height*3; l++) M_IMGDATA->m_data[l] = 0;
  
        M_IMGDATA->m_width = width;
        M_IMGDATA->m_height = height;
        M_IMGDATA->m_ok = TRUE;
    }
    else
    {
        UnRef();
    }
}

void wxImage::Destroy()
{
    UnRef();
}

wxImage wxImage::Scale( int width, int height )
{
    wxImage image;
    
    wxCHECK_MSG( Ok(), image, "invlaid image" );
    
    wxCHECK_MSG( (width > 0) && (height > 0), image, "invalid image size" );
    
    image.Create( width, height );
    
    char unsigned *data = image.GetData();
    
    wxCHECK_MSG( data, image, "unable to create image" );
    
    if (M_IMGDATA->m_hasMask)
        image.SetMaskColour( M_IMGDATA->m_maskRed, M_IMGDATA->m_maskGreen, M_IMGDATA->m_maskBlue );
    
    double xscale = (double)width / (double)M_IMGDATA->m_width;
    double yscale = (double)height / (double)M_IMGDATA->m_height;
    
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
	{
	    int new_pos = 3*(j*width + i);
	    int old_pos = 3*((long)(j/yscale)*M_IMGDATA->m_width + (long)(i/xscale));
	    data[ new_pos   ] = M_IMGDATA->m_data[ old_pos   ];
	    data[ new_pos+1 ] = M_IMGDATA->m_data[ old_pos+1 ];
	    data[ new_pos+2 ] = M_IMGDATA->m_data[ old_pos+2 ];
	}
    }
    
    return image;
}
  
void wxImage::SetRGB( int x, int y, unsigned char r, unsigned char g, unsigned char b )
{
    wxCHECK_RET( Ok(), "invalid image" );
    
    int w = M_IMGDATA->m_width;
    int h = M_IMGDATA->m_height;
    
    wxCHECK_RET( (x>=0) && (y>=0) && (x<w) && (y<h), "invalid image index" );
    
    long pos = (y * w + x) * 3;
    
    M_IMGDATA->m_data[ pos   ] = r;
    M_IMGDATA->m_data[ pos+1 ] = g;
    M_IMGDATA->m_data[ pos+2 ] = b;
}

unsigned char wxImage::GetRed( int x, int y )
{
    wxCHECK_MSG( Ok(), 0, "invalid image" );
    
    int w = M_IMGDATA->m_width;
    int h = M_IMGDATA->m_height;
    
    wxCHECK_MSG( (x>=0) && (y>=0) && (x<w) && (y<h), 0, "invalid image index" );
    
    long pos = (y * w + x) * 3;
    
    return M_IMGDATA->m_data[pos];
}

unsigned char wxImage::GetGreen( int x, int y )
{
    wxCHECK_MSG( Ok(), 0, "invalid image" );
    
    int w = M_IMGDATA->m_width;
    int h = M_IMGDATA->m_height;
    
    wxCHECK_MSG( (x>=0) && (y>=0) && (x<w) && (y<h), 0, "invalid image index" );
    
    long pos = (y * w + x) * 3;
    
    return M_IMGDATA->m_data[pos+1];
}

unsigned char wxImage::GetBlue( int x, int y )
{
    wxCHECK_MSG( Ok(), 0, "invalid image" );
    
    int w = M_IMGDATA->m_width;
    int h = M_IMGDATA->m_height;
    
    wxCHECK_MSG( (x>=0) && (y>=0) && (x<w) && (y<h), 0, "invalid image index" );
    
    long pos = (y * w + x) * 3;
    
    return M_IMGDATA->m_data[pos+2];
}
  
bool wxImage::Ok() const 
{ 
    return (M_IMGDATA && M_IMGDATA->m_ok); 
}

char unsigned *wxImage::GetData() const
{
    wxCHECK_MSG( Ok(), (char unsigned *)NULL, "invalid image" );
  
    return M_IMGDATA->m_data;
}

void wxImage::SetData( char unsigned *WXUNUSED(data) )
{
    wxCHECK_RET( Ok(), "invalid image" );
}

void wxImage::SetMaskColour( unsigned char r, unsigned char g, unsigned char b )
{
    wxCHECK_RET( Ok(), "invalid image" );
  
    M_IMGDATA->m_maskRed = r;
    M_IMGDATA->m_maskGreen = g;
    M_IMGDATA->m_maskBlue = b;
    M_IMGDATA->m_hasMask = TRUE;
}

unsigned char wxImage::GetMaskRed() const
{
    wxCHECK_MSG( Ok(), 0, "invalid image" );
  
    return M_IMGDATA->m_maskRed;
}

unsigned char wxImage::GetMaskGreen() const
{
    wxCHECK_MSG( Ok(), 0, "invalid image" );
   
    return M_IMGDATA->m_maskGreen;
}

unsigned char wxImage::GetMaskBlue() const
{
    wxCHECK_MSG( Ok(), 0, "invalid image" );
  
    return M_IMGDATA->m_maskBlue;
}
  
void wxImage::SetMask( bool mask )
{
    wxCHECK_RET( Ok(), "invalid image" );
  
    M_IMGDATA->m_hasMask = mask;
}

bool wxImage::HasMask() const
{
    wxCHECK_MSG( Ok(), FALSE, "invalid image" );
  
    return M_IMGDATA->m_hasMask;
}

int wxImage::GetWidth() const 
{ 
    wxCHECK_MSG( Ok(), 0, "invalid image" );
    
    return M_IMGDATA->m_width; 
}

int wxImage::GetHeight() const 
{ 
    wxCHECK_MSG( Ok(), 0, "invalid image" );
    
    return M_IMGDATA->m_height; 
}

bool wxImage::LoadFile( const wxString& filename, long type )
{
    UnRef();
  
    if (!wxFileExists(filename))
    {
        wxLogWarning( "Image file does not exist." );

        return FALSE;
    }

    m_refData = new wxImageRefData;

    wxImageHandler *handler = FindHandler(type);

    if (handler == NULL) 
    {
        wxLogWarning( "No image handler for type %d defined.", type );

        return FALSE;
    }

    return handler->LoadFile( this, filename );
}

bool wxImage::SaveFile( const wxString& filename, int type )
{
    wxCHECK_MSG( Ok(), FALSE, "invalid image" );
    
    wxImageHandler *handler = FindHandler(type);

    if (handler == NULL) 
    {
      wxLogWarning( "No image handler for type %d defined.", type );

      return FALSE;
    }

    return handler->SaveFile( this, filename );
}

void wxImage::AddHandler( wxImageHandler *handler )
{
    sm_handlers.Append( handler );
}

void wxImage::InsertHandler( wxImageHandler *handler )
{
    sm_handlers.Insert( handler );
}

bool wxImage::RemoveHandler( const wxString& name )
{
    wxImageHandler *handler = FindHandler(name);
    if (handler)
    {
        sm_handlers.DeleteObject(handler);
        return TRUE;
    }
    else
        return FALSE;
}

wxImageHandler *wxImage::FindHandler( const wxString& name )
{
    wxNode *node = sm_handlers.First();
    while (node)
    {
        wxImageHandler *handler = (wxImageHandler*)node->Data();
        if (handler->GetName() == name) return handler;
        node = node->Next();
    }
    return (wxImageHandler *)NULL;
}

wxImageHandler *wxImage::FindHandler( const wxString& extension, long bitmapType )
{
    wxNode *node = sm_handlers.First();
    while (node)
    {
        wxImageHandler *handler = (wxImageHandler*)node->Data();
        if ( handler->GetExtension() == extension &&
            (bitmapType == -1 || handler->GetType() == bitmapType) )
        return handler;
        node = node->Next();
    }
    return (wxImageHandler*)NULL;
}

wxImageHandler *wxImage::FindHandler( long bitmapType )
{
    wxNode *node = sm_handlers.First();
    while (node)
    {
        wxImageHandler *handler = (wxImageHandler *)node->Data();
        if (handler->GetType() == bitmapType) return handler;
        node = node->Next();
    }
    return NULL;
}

void wxImage::InitStandardHandlers()
{
    AddHandler( new wxBMPHandler );
    AddHandler( new wxPNGHandler );
}

void wxImage::CleanUpHandlers()
{
    wxNode *node = sm_handlers.First();
    while (node)
    {
        wxImageHandler *handler = (wxImageHandler *)node->Data();
        wxNode *next = node->Next();
        delete handler;
        delete node;
        node = next;
    }
}

//-----------------------------------------------------------------------------
// wxImageHandler
//-----------------------------------------------------------------------------

#if !USE_SHARED_LIBRARIES
IMPLEMENT_DYNAMIC_CLASS(wxImageHandler,wxObject)
#endif

bool wxImageHandler::LoadFile( wxImage *WXUNUSED(image), const wxString& WXUNUSED(name) )
{
    return FALSE;
}

bool wxImageHandler::SaveFile( wxImage *WXUNUSED(image), const wxString& WXUNUSED(name) )
{
    return FALSE;
}

//-----------------------------------------------------------------------------
// wxPNGHandler
//-----------------------------------------------------------------------------

#if !USE_SHARED_LIBRARIES
IMPLEMENT_DYNAMIC_CLASS(wxPNGHandler,wxImageHandler)
#endif
  
bool wxPNGHandler::LoadFile( wxImage *image, const wxString& name )
{
   FILE               *f;
   png_structp         png_ptr;
   png_infop           info_ptr;
   unsigned char      *ptr, **lines, *ptr2;
   int                 transp,bit_depth,color_type,interlace_type;
   png_uint_32         width, height;
   unsigned int	       i;

   image->Destroy();
   
   transp = 0;
   png_ptr = png_create_read_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );
   if (!png_ptr) return FALSE;

   info_ptr = png_create_info_struct( png_ptr );
   if (!info_ptr)
   {
     png_destroy_read_struct( &png_ptr, NULL, NULL );
     return FALSE;
   }

   if (setjmp(png_ptr->jmpbuf))
   {
     png_destroy_read_struct( &png_ptr, &info_ptr, NULL );
     return FALSE;
   }

   if (info_ptr->color_type == PNG_COLOR_TYPE_RGB_ALPHA)
   {
     png_destroy_read_struct( &png_ptr, &info_ptr, NULL );
     return FALSE;
   }
   
   f = fopen( name, "rb" );
   png_init_io( png_ptr, f );
   
   png_read_info( png_ptr, info_ptr );
   png_get_IHDR( png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, NULL, NULL );
   
   if (color_type == PNG_COLOR_TYPE_PALETTE) png_set_expand( png_ptr );
   
   png_set_strip_16( png_ptr );
   png_set_packing( png_ptr );
   if (png_get_valid( png_ptr, info_ptr, PNG_INFO_tRNS)) png_set_expand( png_ptr );
   png_set_filler( png_ptr, 0xff, PNG_FILLER_AFTER );
   
   image->Create( width, height );
   
   if (!image->Ok())
   {
     png_destroy_read_struct( &png_ptr, &info_ptr, NULL );
     return FALSE;
   }
   
   lines = (unsigned char **)malloc( height * sizeof(unsigned char *) );
   if (lines == NULL)
   {
     image->Destroy();
     png_destroy_read_struct( &png_ptr, &info_ptr, NULL );
     return FALSE;
   }
   
   for (i = 0; i < height; i++)
   {
     if ((lines[i] = (unsigned char *)malloc(width * (sizeof(unsigned char) * 4))) == NULL)
     {
       image->Destroy();
       for (unsigned int n = 0; n < i; n++) free( lines[n] );
       free( lines );
       png_destroy_read_struct( &png_ptr, &info_ptr, NULL );
       return FALSE;
     }
  }
     
   png_read_image( png_ptr, lines );
   png_destroy_read_struct( &png_ptr, &info_ptr, NULL );
   ptr = image->GetData();
   if ((color_type == PNG_COLOR_TYPE_GRAY) ||
       (color_type == PNG_COLOR_TYPE_GRAY_ALPHA))
     {
	for (unsigned int y = 0; y < height; y++)
	  {
	     ptr2 = lines[y];
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
	     ptr2 = lines[y];
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
   for (i = 0; i < height; i++) free( lines[i] );
   free( lines );
   if (transp)
     image->SetMaskColour( 255, 0, 255 );
   else
     image->SetMask( FALSE );
   
   return TRUE;
}


bool wxPNGHandler::SaveFile( wxImage *image, const wxString& name )
{
  FILE *f = fopen( name, "wb" );
  if (f)
  {
    png_structp png_ptr = png_create_write_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) 
    { 
      fclose( f ); 
      return FALSE; 
    }
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL)
    {
      fclose(f);
      png_destroy_write_struct( &png_ptr, (png_infopp)NULL );
      return FALSE;
    }
    
    if (setjmp(png_ptr->jmpbuf))
    {
      fclose( f );
      png_destroy_write_struct( &png_ptr, (png_infopp)NULL );
      return FALSE;
    }
    
    png_init_io( png_ptr, f );
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
      fclose( f );
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
          data[(x << 2) + 3] = 0;
	else
	  data[(x << 2) + 3] = 255;
      }
      png_bytep row_ptr = data;
      png_write_rows( png_ptr, &row_ptr, 1 );
    }
    free(data);
    png_write_end( png_ptr, info_ptr );
    png_destroy_write_struct( &png_ptr, (png_infopp)NULL );

    fclose(f);
  }
  return TRUE;
}

//-----------------------------------------------------------------------------
// wxBMPHandler
//-----------------------------------------------------------------------------

#if !USE_SHARED_LIBRARIES
IMPLEMENT_DYNAMIC_CLASS(wxBMPHandler,wxImageHandler)
#endif
  
bool wxBMPHandler::LoadFile( wxImage *image, const wxString& name )
{
   FILE               *file;
   unsigned char      *data, *ptr;
   int                 done, i, bpp, planes, comp, ncolors, line, column,
                       linesize, linepos, rshift = 0, gshift = 0, bshift = 0;
   unsigned char       aByte;
   short int           word;
   long int            dbuf[4], dword, rmask = 0, gmask = 0, bmask = 0, offset,
                       size;
   signed char         bbuf[4];
   struct _cmap
     {
	unsigned char       r, g, b;
     }
                      *cmap = NULL;
#ifndef BI_RGB
#define BI_RGB       0
#define BI_RLE8      1
#define BI_RLE4      2
#endif

#ifndef BI_BITFIELDS
#define BI_BITFIELDS 3
#endif

  image->Destroy();

   file = fopen(name, "r");
   if (!file)
      return NULL;

   done = 0;
   /* 
    * Reading the bmp header 
    */

   fread(&bbuf, 1, 2, file);

   fread(dbuf, 4, 4, file);

   size = dbuf[0];
   offset = dbuf[2];

   fread(dbuf, 4, 2, file);
   int width = (int)dbuf[0];
   int height = (int)dbuf[1];
   if (width > 32767)
     {
	fprintf(stderr, "IMLIB ERROR: Image width > 32767 pixels for file\n");
	fclose(file);
	return FALSE;
     }
   if (height > 32767)
     {
	fprintf(stderr, "IMLIB ERROR: Image height > 32767 pixels for file\n");
	fclose(file);
	return FALSE;
     }
   fread(&word, 2, 1, file);
   planes = (int)word;
   fread(&word, 2, 1, file);
   bpp = (int)word;
   if (bpp != 1 && bpp != 4 && bpp != 8 && bpp && 16 && bpp != 24 && bpp != 32)
     {
	fprintf(stderr, "IMLIB ERROR: unknown bitdepth in file\n");
	fclose(file);
	return FALSE;
     }
   fread(dbuf, 4, 4, file);
   comp = (int)dbuf[0];
   if (comp != BI_RGB && comp != BI_RLE4 && comp != BI_RLE8 && comp != BI_BITFIELDS)
     {
	fprintf(stderr, "IMLIB ERROR: unknown encoding in Windows BMP file\n");
	fclose(file);
	return FALSE;
     }
   fread(dbuf, 4, 2, file);
   ncolors = (int)dbuf[0];
   if (ncolors == 0)
      ncolors = 1 << bpp;
   /* some more sanity checks */
   if (((comp == BI_RLE4) && (bpp != 4)) || ((comp == BI_RLE8) && (bpp != 8)) || ((comp == BI_BITFIELDS) && (bpp != 16 && bpp != 32)))
     {
	fprintf(stderr, "IMLIB ERROR: encoding of BMP doesn't match bitdepth\n");
	fclose(file);
	return FALSE;
     }
   if (bpp < 16)
     {
	cmap = (struct _cmap *)malloc(sizeof(struct _cmap) * ncolors);

	if (!cmap)
	  {
	     fprintf(stderr, "IMLIB ERROR: Cannot allocate RAM for color map in BMP file\n");
	     fclose(file);
	     return FALSE;
	  }
     }
   else
      cmap = NULL;
      
   image->Create( width, height );      
   ptr = image->GetData();
   if (!ptr)
     {
	fprintf(stderr, "IMLIB ERROR: Cannot allocate RAM for RGB data in file\n");
	fclose(file);
	if (cmap)
	   free(cmap);
	return FALSE;
     }

   /*
    * Reading the palette, if it exists.
    */
   if (bpp < 16 && ncolors != 0)
     {
	for (i = 0; i < ncolors; i++)
	  {
	     fread(bbuf, 1, 4, file);
	     cmap[i].b = bbuf[0];
	     cmap[i].g = bbuf[1];
	     cmap[i].r = bbuf[2];
	  }
     }
   else if (bpp == 16 || bpp == 32)
     {
	if (comp == BI_BITFIELDS)
	  {
	     int                 bit = 0;

	     fread(dbuf, 4, 3, file);
	     bmask = dbuf[0];
	     gmask = dbuf[1];
	     rmask = dbuf[2];
	     /* find shift amount.. ugly, but i can't think of a better way */
	     for (bit = 0; bit < bpp; bit++)
	       {
		  if (bmask & (1 << bit))
		     bshift = bit;
		  if (gmask & (1 << bit))
		     gshift = bit;
		  if (rmask & (1 << bit))
		     rshift = bit;
	       }
	  }
	else if (bpp == 16)
	  {
	     rmask = 0x7C00;
	     gmask = 0x03E0;
	     bmask = 0x001F;
	     rshift = 10;
	     gshift = 5;
	     bshift = 0;
	  }
	else if (bpp == 32)
	  {
	     rmask = 0x00FF0000;
	     gmask = 0x0000FF00;
	     bmask = 0x000000FF;
	     rshift = 16;
	     gshift = 8;
	     bshift = 0;
	  }
     }

   /*
    * REading the image data
    */
   fseek(file, offset, SEEK_SET);
   data = ptr;

   /* set the whole image to the background color */
   if (bpp < 16 && (comp == BI_RLE4 || comp == BI_RLE8))
     {
	for (i = 0; i < width * height; i++)
	  {
	     *ptr++ = cmap[0].r;
	     *ptr++ = cmap[0].g;
	     *ptr++ = cmap[0].b;
	  }
	ptr = data;
     }
   line = 0;
   column = 0;
#define poffset (line * width * 3 + column * 3)

   /*
    * BMPs are stored upside down... hmmmmmmmmmm....
    */

   linesize = ((width * bpp + 31) / 32) * 4;
   for (line = (height - 1); line >= 0; line--)
     {
	linepos = 0;
	for (column = 0; column < width;)
	  {
	     if (bpp < 16)
	       {
		  int                 index;

		  linepos++;
		  aByte = getc(file);
		  if (bpp == 1)
		    {
		       int                 bit = 0;

		       for (bit = 0; bit < 8; bit++)
			 {
			    index = ((aByte & (0x80 >> bit)) ? 1 : 0);
			    ptr[poffset] = cmap[index].r;
			    ptr[poffset + 1] = cmap[index].g;
			    ptr[poffset + 2] = cmap[index].b;
			    column++;
			 }
		    }
		  else if (bpp == 4)
		    {
		       if (comp == BI_RLE4)
			 {
			    fprintf(stderr, "can't deal with 4bit encoded yet.\n");
			    image->Destroy();
			    free(cmap);
			    return FALSE;
			 }
		       else
			 {
			    int                 nibble = 0;

			    for (nibble = 0; nibble < 2; nibble++)
			      {
				 index = ((aByte & (0xF0 >> nibble * 4)) >> (!nibble * 4));
				 if (index >= 16)
				    index = 15;
				 ptr[poffset] = cmap[index].r;
				 ptr[poffset + 1] = cmap[index].g;
				 ptr[poffset + 2] = cmap[index].b;
				 column++;
			      }
			 }
		    }
		  else if (bpp == 8)
		    {
		       if (comp == BI_RLE8)
			 {
			    unsigned char       first;

			    first = aByte;
			    aByte = getc(file);
			    if (first == 0)
			      {
				 if (aByte == 0)
				   {
/*                                    column = width; */
				   }
				 else if (aByte == 1)
				   {
				      column = width;
				      line = -1;
				   }
				 else if (aByte == 2)
				   {
				      aByte = getc(file);
				      column += aByte;
				      linepos = column * bpp / 8;
				      aByte = getc(file);
				      line += aByte;
				   }
				 else
				   {
				      int                 absolute = aByte;

				      for (i = 0; i < absolute; i++)
					{
					   linepos++;
					   aByte = getc(file);
					   ptr[poffset] = cmap[aByte].r;
					   ptr[poffset + 1] = cmap[aByte].g;
					   ptr[poffset + 2] = cmap[aByte].b;
					   column++;
					}
				      if (absolute & 0x01)
					 aByte = getc(file);
				   }
			      }
			    else
			      {
				 for (i = 0; i < first; i++)
				   {
				      ptr[poffset] = cmap[aByte].r;
				      ptr[poffset + 1] = cmap[aByte].g;
				      ptr[poffset + 2] = cmap[aByte].b;
				      column++;
				      linepos++;
				   }
			      }
			 }
		       else
			 {
			    ptr[poffset] = cmap[aByte].r;
			    ptr[poffset + 1] = cmap[aByte].g;
			    ptr[poffset + 2] = cmap[aByte].b;
			    column++;
			    linepos += size;
			 }
		    }
	       }
	     else if (bpp == 24)
	       {
		  linepos += fread(&bbuf, 1, 3, file);
		  ptr[poffset] = (unsigned char)bbuf[2];
		  ptr[poffset + 1] = (unsigned char)bbuf[1];
		  ptr[poffset + 2] = (unsigned char)bbuf[0];
		  column++;
	       }
	     else if (bpp == 16)
	       {
		  unsigned char       temp;

		  linepos += fread(&word, 2, 1, file);
		  temp = (word & rmask) >> rshift;
		  ptr[poffset] = temp;
		  temp = (word & gmask) >> gshift;
		  ptr[poffset + 1] = temp;
		  temp = (word & bmask) >> gshift;
		  ptr[poffset + 2] = temp;
		  column++;
	       }
	     else
	       {
		  unsigned char       temp;

		  linepos += fread(&dword, 4, 1, file);
		  temp = (dword & rmask) >> rshift;
		  ptr[poffset] = temp;
		  temp = (dword & gmask) >> gshift;
		  ptr[poffset + 1] = temp;
		  temp = (dword & bmask) >> bshift;
		  ptr[poffset + 2] = temp;
		  column++;
	       }
	  }
	while ((linepos < linesize) && (comp != 1) && (comp != 2))
	  {
	     int                 temp = fread(&aByte, 1, 1, file);

	     linepos += temp;
	     if (!temp)
		break;
	  }
     }
   if (cmap) free(cmap);
      
   image->SetMask( FALSE );
   
   fclose(file);
   return TRUE;
}

#ifdef __WXMSW__

wxBitmap wxImage::ConvertToBitmap() const
{
  
  wxBitmap bitmap;
  wxCHECK_MSG( Ok(), bitmap, "invalid image" );
  int width = GetWidth();
  int height = GetHeight();
  bitmap.SetWidth( width );
  bitmap.SetHeight( height );
  bitmap.SetDepth( wxDisplayDepth() );

  int headersize = sizeof(BITMAPINFOHEADER);
  LPBITMAPINFO lpDIBh = (BITMAPINFO *) malloc( headersize );
  wxCHECK_MSG( lpDIBh, bitmap, "could not allocate memory for DIB header" );

// Fill in the DIB header
  lpDIBh->bmiHeader.biSize = headersize;
  lpDIBh->bmiHeader.biWidth = width;
  lpDIBh->bmiHeader.biHeight = -height;
  lpDIBh->bmiHeader.biSizeImage = width * height * 3;

  lpDIBh->bmiHeader.biPlanes = 1;
  lpDIBh->bmiHeader.biBitCount = 24;
  lpDIBh->bmiHeader.biCompression = BI_RGB;
  lpDIBh->bmiHeader.biClrUsed = 0;

// These seem not needed for our purpose here.
//  lpDIBh->bmiHeader.biClrImportant = 0;
//  lpDIBh->bmiHeader.biXPelsPerMeter = 0;
//  lpDIBh->bmiHeader.biYPelsPerMeter = 0;

  unsigned char *lpBits = (unsigned char *) malloc( width*height*3 );
  if( !lpBits )
  {
      wxFAIL_MSG( "could not allocate memory for DIB" );
      free( lpDIBh );
      return bitmap;
  }

  unsigned char *data = GetData();

  unsigned char *ptdata = data, *ptbits = lpBits;
  for( int i=0; i<width*height; i++ )
  {
    *(ptbits++) = *(ptdata+2);
    *(ptbits++) = *(ptdata+1);
    *(ptbits++) = *(ptdata  );
    ptdata += 3;
  }
 
  HDC hdc = ::GetDC(NULL);

  HBITMAP hbitmap;
  hbitmap = CreateDIBitmap( hdc, &(lpDIBh->bmiHeader), CBM_INIT, lpBits, lpDIBh, DIB_RGB_COLORS );
    
// The above line is equivalent to the following two lines.
//    hbitmap = ::CreateCompatibleBitmap( hdc, width, height );
//    ::SetDIBits( hdc, hbitmap, 0, height, lpBits, lpDIBh, DIB_RGB_COLORS);
// or the following lines
//    hbitmap = ::CreateCompatibleBitmap( hdc, width, height );
//    HDC memdc = ::CreateCompatibleDC( hdc );
//    ::SelectObject( memdc, hbitmap); 
//    ::SetDIBitsToDevice( memdc, 0, 0, width, height,
//	    0, 0, 0, height, (void *)lpBits, lpDIBh, DIB_RGB_COLORS);
//    ::SelectObject( memdc, 0 ); 
//    ::DeleteDC( memdc ); 

  bitmap.SetHBITMAP( (WXHBITMAP) hbitmap );

  if( HasMask() )
  {
    unsigned char r = GetMaskRed();
    unsigned char g = GetMaskGreen();
    unsigned char b = GetMaskBlue();
    unsigned char zero = 0, one = 255;
    ptdata = data;
    ptbits = lpBits;
    for( int i=0; i<width*height; i++ )
    {
      if( (*(ptdata++)!=r) | (*(ptdata++)!=g) | (*(ptdata++)!=b) )
      {
        *(ptbits++) = one;
        *(ptbits++) = one;
        *(ptbits++) = one;
      }
      else
      {
        *(ptbits++) = zero;
        *(ptbits++) = zero;
        *(ptbits++) = zero;
      }
    }
    hbitmap = ::CreateBitmap( (WORD)width, (WORD)height, 1, 1, NULL );
    ::SetDIBits( hdc, hbitmap, 0, (WORD)height, lpBits, lpDIBh, DIB_RGB_COLORS);
    wxMask *mask = new wxMask();
    mask->SetMaskBitmap( (WXHBITMAP) hbitmap );
    bitmap.SetMask( mask );

/* The following can also be used but is slow to run
    wxColour colour( GetMaskRed(), GetMaskGreen(), GetMaskBlue());
    wxMask *mask = new wxMask( bitmap, colour );
    bitmap.SetMask( mask );
*/
  }

  ::ReleaseDC(NULL, hdc);   
  free(lpDIBh);
  free(lpBits);

  if( bitmap.GetHBITMAP() )
    bitmap.SetOk( TRUE );
  else
    bitmap.SetOk( FALSE );
   
  return bitmap;
}


wxImage::wxImage( const wxBitmap &bitmap )
{
  if( !bitmap.Ok() )
  {
      wxFAIL_MSG( "invalid bitmap" );
      return;
  }

  int width = bitmap.GetWidth();
  int height = bitmap.GetHeight();
  Create( width, height ); 
  unsigned char *data = GetData();
  if( !data )
  {
      wxFAIL_MSG( "could not allocate data for image" );
      return;
  }
    
  int headersize = sizeof(BITMAPINFOHEADER);
  LPBITMAPINFO lpDIBh = (BITMAPINFO *) malloc( headersize );
  if( !lpDIBh )
  {
      wxFAIL_MSG( "could not allocate data for DIB header" );
      free( data );
      return;
  }

// Fill in the DIB header
  lpDIBh->bmiHeader.biSize = headersize;
  lpDIBh->bmiHeader.biWidth = width;
  lpDIBh->bmiHeader.biHeight = -height;
  lpDIBh->bmiHeader.biSizeImage = width * height * 3;

  lpDIBh->bmiHeader.biPlanes = 1;
  lpDIBh->bmiHeader.biBitCount = 24;
  lpDIBh->bmiHeader.biCompression = BI_RGB;
  lpDIBh->bmiHeader.biClrUsed = 0;

// These seem not needed for our purpose here.
//    lpDIBh->bmiHeader.biClrImportant = 0;
//    lpDIBh->bmiHeader.biXPelsPerMeter = 0;
//    lpDIBh->bmiHeader.biYPelsPerMeter = 0;

  unsigned char *lpBits = (unsigned char *) malloc( width*height*3 );
  if( !lpBits )
  {
      wxFAIL_MSG( "could not allocate data for DIB" );
      free( data );
      free( lpDIBh );
      return;
  }
    
  HBITMAP hbitmap;
  hbitmap = (HBITMAP) bitmap.GetHBITMAP();
  HDC hdc = ::GetDC(NULL);
  ::GetDIBits( hdc, hbitmap, 0, height, lpBits, lpDIBh, DIB_RGB_COLORS );
	
  unsigned char *ptdata = data, *ptbits = lpBits;
  for( int i=0; i<width*height; i++ )
  {
    *(ptdata++) = *(ptbits+2);
    *(ptdata++) = *(ptbits+1);
    *(ptdata++) = *(ptbits  );
    ptbits += 3;
  }	
      
  if( bitmap.GetMask() && bitmap.GetMask()->GetMaskBitmap() )
  {
    hbitmap = (HBITMAP) bitmap.GetMask()->GetMaskBitmap();
    HDC memdc = ::CreateCompatibleDC( hdc );
    ::SetTextColor( memdc, RGB( 0, 0, 0 ) );
    ::SetBkColor( memdc, RGB( 255, 255, 255 ) );
    ::GetDIBits( memdc, hbitmap, 0, height, lpBits, lpDIBh, DIB_RGB_COLORS );
    ::DeleteDC( memdc ); 
    unsigned char r=16, g=16, b=16;  // background set to RGB(16,16,16)
    ptdata = data;
    ptbits = lpBits;
    for( int i=0; i<width*height; i++ )
    {
      if( *ptbits != 0 )
      {
        *(ptdata++)  = r;
        *(ptdata++)  = g;
        *(ptdata++)  = b;
      }
      ptbits += 3;
    }       
    SetMaskColour( r, g, b );
  }	
      
  ::ReleaseDC(NULL, hdc);   
  free(lpDIBh);
  free(lpBits);
}

#endif

#ifdef __WXGTK__

wxBitmap wxImage::ConvertToBitmap() const
{
    wxBitmap bitmap;

    wxCHECK_MSG( Ok(), bitmap, "invalid image" );

    int width = GetWidth();
    int height = GetHeight();

    bitmap.SetHeight( height );
    bitmap.SetWidth( width );

    // Create picture

    GdkImage *data_image =
      gdk_image_new( GDK_IMAGE_FASTEST, gdk_visual_get_system(), width, height );

    bitmap.SetPixmap( gdk_pixmap_new( (GdkWindow*)&gdk_root_parent, width, height, -1 ) );

    // Create mask

    GdkImage *mask_image = (GdkImage*) NULL;

    if (HasMask())
    {
        unsigned char *mask_data = (unsigned char*)malloc( ((width >> 3)+8) * height );

        mask_image =  gdk_image_new_bitmap( gdk_visual_get_system(), mask_data, width, height );

	wxMask *mask = new wxMask();
	mask->m_bitmap = gdk_pixmap_new( (GdkWindow*)&gdk_root_parent, width, height, 1 );

	bitmap.SetMask( mask );
    }

    // Retrieve depth

    GdkVisual *visual = gdk_window_get_visual( bitmap.GetPixmap() );
    if (visual == NULL) visual = gdk_window_get_visual( (GdkWindow*) &gdk_root_parent );
    int bpp = visual->depth;
    if ((bpp == 16) && (visual->red_mask != 0xf800)) bpp = 15;
    if (bpp < 8) bpp = 8;

    // Render

    enum byte_order { RGB, RBG, BRG, BGR, GRB, GBR };
    byte_order b_o = RGB;

    if (bpp >= 24)
    {
        GdkVisual *visual = gdk_visual_get_system();
        if ((visual->red_mask > visual->green_mask) && (visual->green_mask > visual->blue_mask))      b_o = RGB;
        else if ((visual->red_mask > visual->blue_mask) && (visual->blue_mask > visual->green_mask))  b_o = RGB;
        else if ((visual->blue_mask > visual->red_mask) && (visual->red_mask > visual->green_mask))   b_o = BRG;
        else if ((visual->blue_mask > visual->green_mask) && (visual->green_mask > visual->red_mask)) b_o = BGR;
        else if ((visual->green_mask > visual->red_mask) && (visual->red_mask > visual->blue_mask))   b_o = GRB;
        else if ((visual->green_mask > visual->blue_mask) && (visual->blue_mask > visual->red_mask))  b_o = GBR;
    }

    int r_mask = GetMaskRed();
    int g_mask = GetMaskGreen();
    int b_mask = GetMaskBlue();

    unsigned char* data = GetData();

    int index = 0;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int r = data[index];
	    index++;
            int g = data[index];
	    index++;
            int b = data[index];
	    index++;

	    if (HasMask())
	    {
	        if ((r == r_mask) && (b == b_mask) && (g == g_mask))
	            gdk_image_put_pixel( mask_image, x, y, 1 );
	        else
	            gdk_image_put_pixel( mask_image, x, y, 0 );
	    }

	    switch (bpp)
	    {
	        case 8:
	        {
	            GdkColormap *cmap = gtk_widget_get_default_colormap();
                    GdkColor *colors = cmap->colors;
                    int max = 3 * (65536);
                    int index = -1;

                    for (int i = 0; i < cmap->size; i++)
                    {
                        int rdiff = (r << 8) - colors[i].red;
                        int gdiff = (g << 8) - colors[i].green;
                        int bdiff = (b << 8) - colors[i].blue;
                        int sum = ABS (rdiff) + ABS (gdiff) + ABS (bdiff);
                        if (sum < max) { index = i; max = sum; }
                    }

	            gdk_image_put_pixel( data_image, x, y, index );

	            break;
	        }
	        case 15:
	        {
	            guint32 pixel = ((r & 0xf8) << 7) | ((g & 0xf8) << 2) | ((b & 0xf8) >> 3);
	            gdk_image_put_pixel( data_image, x, y, pixel );
	            break;
	        }
	        case 16:
	        {
	            guint32 pixel = ((r & 0xf8) << 8) | ((g & 0xfc) << 3) | ((b & 0xf8) >> 3);
	            gdk_image_put_pixel( data_image, x, y, pixel );
	            break;
	        }
	        case 32:
	        case 24:
	        {
	            guint32 pixel = 0;
	            switch (b_o)
	            {
	                case RGB: pixel = (r << 16) | (g << 8) | b; break;
	                case RBG: pixel = (r << 16) | (b << 8) | g; break;
	                case BRG: pixel = (b << 16) | (r << 8) | g; break;
	                case BGR: pixel = (b << 16) | (g << 8) | r; break;
	                case GRB: pixel = (g << 16) | (r << 8) | b; break;
	                case GBR: pixel = (g << 16) | (b << 8) | r; break;
	            }
	            gdk_image_put_pixel( data_image, x, y, pixel );
	        }
	        default: break;
	    }
        } // for
    }  // for

    // Blit picture

    GdkGC *data_gc = gdk_gc_new( bitmap.GetPixmap() );

    gdk_draw_image( bitmap.GetPixmap(), data_gc, data_image, 0, 0, 0, 0, width, height );

    gdk_image_destroy( data_image );
    gdk_gc_unref( data_gc );

    // Blit mask

    if (HasMask())
    {
        GdkGC *mask_gc = gdk_gc_new( bitmap.GetMask()->GetBitmap() );

        gdk_draw_image( bitmap.GetMask()->GetBitmap(), mask_gc, mask_image, 0, 0, 0, 0, width, height );

        gdk_image_destroy( mask_image );
        gdk_gc_unref( mask_gc );
    }

    return bitmap;
}

wxImage::wxImage( const wxBitmap &bitmap )
{
    wxCHECK_RET( bitmap.Ok(), "invalid bitmap" );

    GdkImage *gdk_image = gdk_image_get( bitmap.GetPixmap(),
                                         0, 0,
					 bitmap.GetWidth(), bitmap.GetHeight() );

    wxCHECK_RET( gdk_image, "couldn't create image" );

    Create( bitmap.GetWidth(), bitmap.GetHeight() );
    char unsigned *data = GetData();

    if (!data)
    {
        gdk_image_destroy( gdk_image );
        wxFAIL_MSG( "couldn't create image" );
	return;
    }

    GdkImage *gdk_image_mask = (GdkImage*) NULL;
    if (bitmap.GetMask())
    {
        gdk_image_mask = gdk_image_get( bitmap.GetMask()->GetBitmap(),
	                                0, 0,
					bitmap.GetWidth(), bitmap.GetHeight() );

	SetMaskColour( 16, 16, 16 );  // anything unlikely and dividable
    }

    GdkVisual *visual = gdk_window_get_visual( bitmap.GetPixmap() );
    if (visual == NULL) visual = gdk_window_get_visual( (GdkWindow*) &gdk_root_parent );
    int bpp = visual->depth;
    if ((bpp == 16) && (visual->red_mask != 0xf800)) bpp = 15;

    GdkColormap *cmap = gtk_widget_get_default_colormap();

    long pos = 0;
    for (int j = 0; j < bitmap.GetHeight(); j++)
    {
        for (int i = 0; i < bitmap.GetWidth(); i++)
        {
            int pixel = gdk_image_get_pixel( gdk_image, i, j );
            if (bpp <= 8)
            {
                data[pos] = cmap->colors[pixel].red >> 8;
                data[pos+1] = cmap->colors[pixel].green >> 8;
                data[pos+2] = cmap->colors[pixel].blue >> 8;
            } else if (bpp == 15)
            {
                data[pos] = (pixel >> 7) & 0xf8;
                data[pos+1] = (pixel >> 2) & 0xf8;
                data[pos+2] = (pixel << 3) & 0xf8;
            } else if (bpp == 16)
            {
                data[pos] = (pixel >> 8) & 0xf8;
                data[pos+1] = (pixel >> 3) & 0xfc;
                data[pos+2] = (pixel << 3) & 0xf8;
            } else
            {
                data[pos] = (pixel >> 16) & 0xff;
                data[pos+1] = (pixel >> 8) & 0xff;
                data[pos+2] = pixel & 0xff;
            }

	    if (gdk_image_mask)
	    {
	        int mask_pixel = gdk_image_get_pixel( gdk_image_mask, i, j );
	        if (mask_pixel == 0)
	        {
                    data[pos] = 16;
                    data[pos+1] = 16;
                    data[pos+2] = 16;
	       }
	    }

            pos += 3;
        }
    }

    gdk_image_destroy( gdk_image );
    if (gdk_image_mask) gdk_image_destroy( gdk_image_mask );
}

#endif
