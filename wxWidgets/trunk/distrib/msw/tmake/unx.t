#!################################################################################
#! File:    unx.t
#! Purpose: tmake template file from which makefile.unx.in is generated by running
#!          tmake -t unx wxwin.pro -o makefile.unx.in
#! Author:  Vadim Zeitlin, Robert Roebling, Julian Smart
#! Created: 14.07.99
#! Version: $Id$
#!################################################################################

#${
    #! include the code which parses filelist.txt file and initializes
    #! %wxCommon, %wxGeneric, %wxHtml, %wxUnix and %wxGTK hashes.
    IncludeTemplate("filelist.t");

    #! now transform these hashes into $project tags
    foreach $file (sort keys %wxGeneric) {
        next if $wxGeneric{$file} =~ /\bR\b/;

        $file =~ s/cp?p?$/\o/;
        $project{"WXGENERICOBJS"} .= $file . " "
    }

    foreach $file (sort keys %wxCommon) {
        next if $wxCommon{$file} =~ /\bR\b/;

        $file =~ s/cp?p?$/\o/;
        $project{"WXCOMMONOBJS"} .= $file . " "
    }

    foreach $file (sort keys %wxGTK) {
        $file =~ s/cp?p?$/\o/;
        $project{"WXGTKOBJS"} .= $file . " "
    }

    foreach $file (sort keys %wxHTML) {
        $file =~ s/cp?p?$/\o/;
        $project{"WXHTMLOBJS"} .= $file . " "
    }

    foreach $file (sort keys %wxUNIX) {
        $file =~ s/cp?p?$/\o/;
        $project{"WXUNIXOBJS"} .= $file . " "
    }
#$}
# This file was automatically generated by tmake at #$ Now()
# DO NOT CHANGE THIS FILE, YOUR CHANGES WILL BE LOST! CHANGE UNX.T!

#
# File:     makefile.unx
# Author:   Julian Smart, Robert Roebling, Vadim Zeitlin
# Created:  1993
# Updated:  1999
# Copyright:(c) 1993, AIAI, University of Edinburgh,
# Copyright:(c) 1999, Vadim Zeitlin
# Copyright:(c) 1999, Robert Roebling
#
# Makefile for libwx_gtk.a, libwx_motif.a and libwx_msw.a

########################### VERSION #################################

LIBS = @LIBS@

TOOLKIT = @TOOLKIT@

WXLIB = @WX_LIBRARY@

########################### VERSION #################################

WX_MAJOR_VERSION_NUMBER = @WX_MAJOR_VERSION_NUMBER@
WX_MINOR_VERSION_NUMBER = @WX_MINOR_VERSION_NUMBER@
WX_RELEASE_NUMBER = @WX_RELEASE_NUMBER@

########################### Misc #################################

SHELL = @SHELL@

########################### Paths #################################

srcdir = @srcdir@/src/gtk

top_srcdir = @top_srcdir@
prefix = @prefix@
exec_prefix = @exec_prefix@

bindir = @bindir@
sbindir = @sbindir@
libexecdir = @libexecdir@
datadir = @datadir@
sysconfdir = @sysconfdir@
sharedstatedir = @sharedstatedir@
localstatedir = @localstatedir@
libdir = @libdir@
infodir = @infodir@
mandir = @mandir@
includedir = @includedir@
oldincludedir = /usr/include

DESTDIR =

pkgdatadir = $(datadir)/@PACKAGE@
pkglibdir = $(libdir)/@PACKAGE@
pkgincludedir = $(includedir)/@PACKAGE@

top_builddir = .

INSTALL = @INSTALL@
INSTALL_PROGRAM = @INSTALL_PROGRAM@
INSTALL_DATA = @INSTALL_DATA@
INSTALL_SCRIPT = @INSTALL_SCRIPT@
transform = @program_transform_name@

NORMAL_INSTALL = :
PRE_INSTALL = :
POST_INSTALL = :
NORMAL_UNINSTALL = :
PRE_UNINSTALL = :
POST_UNINSTALL = :
build_alias = @build_alias@
build_triplet = @build@
host_alias = @host_alias@
host_triplet = @host@
target_alias = @target_alias@
target_triplet = @target@

EXTRA_VPATH = @EXTRA_VPATH_MF@

VPATH = .:${srcdir}:${srcdir}/src/common:${srcdir}/src/generic:${srcdir}/src/html:${EXTRA_VPATH}

########################### Programs #################################

# C++ compiler
CC          = @CXX@
CCPP        = @CXXCPP@

# C compiler
CCC         = @CC@
CCCPP       = @CPP@

# Compiler for lex/yacc .c programs
CCLEX       = @CC@

LEX         = @LEX@
YACC        = @YACC@
AR          = @AR@
AS          = @AS@
NM          = @NM@
LN_S        = @LN_S@
STRIP       = @STRIP@
MAKE        = make
AROPTIONS   = ruv
RANLIB      = @RANLIB@
LD          = @LD@
MAKEINFO    = @MAKEINFO@

########################### Flags #################################

CFLAGS = @TOOLKIT_DEF@ @WXDEBUG_DEFINE@ @CFLAGS@
CPPFLAGS = @TOOLKIT_DEF@ @WXDEBUG_DEFINE@ @CXXFLAGS@

########################### Rules #################################

# Clears all default suffixes
.SUFFIXES:	.o .cpp .c

.c.o :
	$(CCC) -c $(CFLAGS) -o $@ $<

.cpp.o :
	$(CC) -c $(CPPFLAGS) -o $@ $<

########################### Files #################################

WXDIR = $(srcdir)/../..

# Subordinate library possibilities

GENDIR  = $(WXDIR)/src/generic
COMMDIR = $(WXDIR)/src/common
HTMLDIR = $(WXDIR)/src/html
UNIXDIR = $(WXDIR)/src/unix
PNGDIR  = $(WXDIR)/src/png
JPEGDIR = $(WXDIR)/src/jpeg
ZLIBDIR = $(WXDIR)/src/zlib
GTKDIR  = $(WXDIR)/src/gtk

DOCDIR = $(WXDIR)\docs

GENERICOBJS = \
                #$ ExpandList("WXGENERICOBJS");

COMMONOBJS  = \
		parser.o \
		#$ ExpandList("WXCOMMONOBJS");

HTMLOBJS = \
                #$ ExpandList("WXHTMLOBJS");

GTKOBJS     = \
		#$ ExpandList("WXGTKOBJS");

UNIXOBJS     = \
		#$ ExpandList("WXUNIXOBJS");

ZLIBOBJS    = \
		adler32.o \
		compress.o \
		crc32.o \
		gzio.o \
		uncompr.o \
		deflate.o \
		trees.o \
		zutil.o \
		inflate.o \
		infblock.o \
		inftrees.o \
		infcodes.o \
		infutil.o \
		inffast.o

PNGOBJS     = \
		png.o \
		pngread.o \
		pngrtran.o \
		pngrutil.o \
		pngpread.o \
		pngtrans.o \
		pngwrite.o \
		pngwtran.o \
		pngwutil.o \
		pngerror.o \
		pngmem.o \
		pngwio.o \
		pngrio.o \
		pngget.o \
		pngset.o


JPEGOBJS    = \
		jcomapi.o \
		jutils.o \
		jerror.o \
		jmemmgr.o \
		jmemnobs.o \
		jcapimin.o \
		jcapistd.o \
		jctrans.o \
		jcparam.o \
		jdatadst.o \
		jcinit.o \
		jcmaster.o \
		jcmarker.o \
		jcmainct.o \
		jcprepct.o \
		jccoefct.o \
		jccolor.o \
		jcsample.o \
		jchuff.o \
		jcphuff.o \
		jcdctmgr.o \
		jfdctfst.o \
		jfdctflt.o \
		jfdctint.o \
		jdapimin.o \
		jdapistd.o \
		jdtrans.o \
		jdatasrc.o \
		jdmaster.o \
		jdinput.o \
		jdmarker.o \
		jdhuff.o \
		jdphuff.o \
		jdmainct.o \
		jdcoefct.o \
		jdpostct.o \
		jddctmgr.o \
		jidctfst.o \
		jidctflt.o \
		jidctint.o \
		jidctred.o \
		jdsample.o \
		jdcolor.o \
		jquant1.o \
		jquant2.o \
		jdmerge.o

OBJECTS = $(GTKOBJS) $(COMMONOBJS) $(GENERICOBJS) $(HTMLOBJ) $(UNIXOBJS) \
	  $(JPEGOBJS) $(PNGOBJS) $(ZLIBOBJS)

all:    $(OBJECTS) $(WXLIB)

$(WXLIB): $(OBJECTS)
	$AR) $(AROPTIONS) $@ $(OBJECTS)
	$(RANLIB) $@

$(OBJECTS):	$(WXDIR)/include/wx/defs.h $(WXDIR)/include/wx/object.h $(WXDIR)/include/wx/setup.h

parser.o:    parser.c lexer.c
	$(CCLEX) -c $(CFLAGS) -o $@ parser.c

parser.c:	$(COMMDIR)/parser.y lexer.c
	$(YACC) $(COMMDIR)/parser.y
	@sed -e "s;$(COMMDIR)/y.tab.c;parser.y;g" < y.tab.c | \
	sed -e "s/BUFSIZ/5000/g"            | \
	sed -e "s/YYLMAX 200/YYLMAX 5000/g" | \
	sed -e "s/yy/PROIO_yy/g"            | \
	sed -e "s/input/PROIO_input/g"      | \
	sed -e "s/unput/PROIO_unput/g"      > parser.c
	@$(RM) y.tab.c

lexer.c:	$(COMMDIR)/lexer.l
	$(LEX) $(COMMDIR)/lexer.l
	@sed -e "s;$(COMMDIR)/lex.yy.c;lexer.l;g" < lex.yy.c | \
	sed -e "s/yy/PROIO_yy/g"            | \
	sed -e "s/input/PROIO_input/g"      | \
	sed -e "s/unput/PROIO_unput/g"      > lexer.c
	@$(RM) lex.yy.c

clean:
	rm -f *.o
	rm -f *.lo
	rm -f parser.c
	rm -f lexer.c
	rm -f *.a
	rm -f *.la

cleanall: clean
