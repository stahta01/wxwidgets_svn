#
# Tag Image File Format Library
#
# Copyright (c) 1995 Sam Leffler
# Copyright (c) 1995 Silicon Graphics, Inc.
# 
# Permission to use, copy, modify, distribute, and sell this software and 
# its documentation for any purpose is hereby granted without fee, provided
# that (i) the above copyright notices and this permission notice appear in
# all copies of the software and related documentation, and (ii) the names of
# Sam Leffler and Silicon Graphics may not be used in any advertising or
# publicity relating to the software without the specific, prior written
# permission of Sam Leffler and Silicon Graphics.
# 
# THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND, 
# EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
# WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  
# 
# IN NO EVENT SHALL SAM LEFFLER OR SILICON GRAPHICS BE LIABLE FOR
# ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
# OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
# WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF 
# LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE 
# OF THIS SOFTWARE.
#
DEPTH= ::
SRCDIR= :

NULL	=
CC	= C
AR	= Lib
AROPTS	= 
RM= delete -y

IPATH	=  -I {DEPTH} -I {SRCDIR}
COPTS	= 
OPTIMIZER=
CFLAGS	=  {COPTS} {OPTIMIZER} {IPATH}

CFILES	= 
OBJECTS	= getopt.c.o
TARGETS	= libport.o

.c.o %c4 .c
	{CC} -model far {COptions} {CFLAGS} -s {Default} {DepDir}{Default}.c -o {TargDir}{Default}.c.o

all %c4  {TARGETS}

libport.o %c4  {OBJECTS}
	{AR} {OBJECTS} -o libport.o
	
clean %c4
	{RM} {TARGETS} {OBJECTS} || set status 0
