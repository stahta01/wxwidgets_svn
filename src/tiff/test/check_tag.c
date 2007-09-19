/* $Id$ */

/*
 * Copyright (c) 2004, Andrey Kiselev  <dron@ak4719.spb.edu>
 *
 * Permission to use, copy, modify, distribute, and sell this software and 
 * its documentation for any purpose is hereby granted without fee, provided
 * that (i) the above copyright notices and this permission notice appear in
 * all copies of the software and related documentation, and (ii) the names of
 * Sam Leffler and Silicon Graphics may not be used in any advertising or
 * publicity relating to the software without the specific, prior written
 * permission of Sam Leffler and Silicon Graphics.
 * 
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 * IN NO EVENT SHALL SAM LEFFLER OR SILICON GRAPHICS BE LIABLE FOR
 * ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
 * OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF 
 * LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE 
 * OF THIS SOFTWARE.
 */

/*
 * TIFF Library
 *
 * Module to test LONG tags read/write functions.
 */

#include "tiffio.h"

int
CheckShortField(TIFF *tif, ttag_t field, uint16 value)
{
	uint16 tmp = 0;

	if (!TIFFGetField(tif, field, &tmp)) {
		fprintf (stderr, "Problem fetching tag %lu.\n",
			 (unsigned long) field);
		return -1;
	}
	if (tmp != value) {
		fprintf (stderr, "Wrong SHORT value fetched for tag %lu.\n",
			 (unsigned long) field);
		return -1;
	}

	return 0;
}

int
CheckLongField(TIFF *tif, ttag_t field, uint32 value)
{
	uint32 tmp = 0;

	if (!TIFFGetField(tif, field, &tmp)) {
		fprintf (stderr, "Problem fetching tag %lu.\n",
			 (unsigned long) field);
		return -1;
	}
	if (tmp != value) {
		fprintf (stderr, "Wrong LONG value fetched for tag %lu.\n",
			 (unsigned long) field);
		return -1;
	}

	return 0;
}


