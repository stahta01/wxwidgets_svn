#!#############################################################################
#! File:    g95.t
#! Purpose: tmake template file from which makefile.g95 is generated by running
#!          tmake -t g95 wxwin.pro -o makefile.g95
#! Author:  Vadim Zeitlin, Robert Roebling, Julian Smart
#! Created: 14.07.99
#! Version: $Id$
#!#############################################################################
#${
    #! include the code which parses filelist.txt file and initializes
    #! %wxCommon, %wxGeneric and %wxMSW hashes.
    IncludeTemplate("filelist.t");

    #! now transform these hashes into $project tags
    foreach $file (sort keys %wxGeneric) {
        #! native wxDirDlg can't be compiled due to GnuWin32/OLE limitations,
        #! so take the generic version
        if ( $wxGeneric{$file} =~ /\b(PS|G|U|16)\b/ ) {
            next unless $file =~ /^dirdlgg\./;
        }

        $file =~ s/cp?p?$/\$(OBJSUFF)/;
        $project{"WXGENERICOBJS"} .= '$(GENDIR)/' . $file . " "
    }

    foreach $file (sort keys %wxCommon) {
        next if $wxCommon{$file} =~ /\b(16)\b/;

        #! needs extra files (sql*.h) so not compiled by default.
        #! next if $file =~ /^odbc\./;

        if ( $file =~ /^odbc\./ )
        {
          $file =~ s/cp?p?$/\$(OBJSUFF)/;
          $project{"ADVANCEDOBJS"} .= '$(COMMDIR)/' . $file . " "
        }
        else
        {
          $file =~ s/cp?p?$/\$(OBJSUFF)/;
          $project{"WXCOMMONOBJS"} .= '$(COMMDIR)/' . $file . " "
        }
    }

    foreach $file (sort keys %wxMSW) {
        #! native wxDirDlg can't be compiled due to GnuWin32/OLE limitations,
        next if $file =~ /^dirdlg\./;

        next if $wxMSW{$file} =~ /\b(16)\b/;

        #! Mingw32 doesn't have the OLE headers and has some troubles with
        #! socket code, so put in  ADVANCEDOBJS
        if ( $wxMSW{$file} =~ /\b(O)\b/ )
        {
          $file =~ s/cp?p?$/\$(OBJSUFF)/;
          $project{"ADVANCEDOBJS"} .= '$(MSWDIR)/ole/' . $file . " "
        }
        else
        {
          $file =~ s/cp?p?$/\$(OBJSUFF)/;
          $project{"WXMSWOBJS"} .= '$(MSWDIR)/' . $file . " "
        }

    }

    foreach $file (sort keys %wxHTML) {
        $file =~ s/cp?p?$/\$(OBJSUFF)/;
        $project{"WXHTMLOBJS"} .= '$(HTMLDIR)/' . $file . " "
    }

#$}
# This file was automatically generated by tmake at #$ Now()
# DO NOT CHANGE THIS FILE, YOUR CHANGES WILL BE LOST! CHANGE G95.T!

#
# File:     makefile.g95
# Author:   Julian Smart, Robert Roebling, Vadim Zeitlin
# Created:  1993
# Updated:  1999
# Copyright:(c) 1993, AIAI, University of Edinburgh,
# Copyright:(c) 1999, Vadim Zeitlin
# Copyright:(c) 1999, Robert Roebling
#
# Makefile for libwx.a

# Replace this with your own path if necessary
WXDIR = ../..

# All common compiler flags and options are now in
# this central makefile.
include $(WXDIR)/src/makeg95.env

# Subordinate library possibilities

EXTRAOBJS=

GENDIR  = $(WXDIR)/src/generic
COMMDIR = $(WXDIR)/src/common
HTMLDIR = $(WXDIR)/src/html
XPMDIR  = $(WXDIR)/src/xpm
PNGDIR  = $(WXDIR)/src/png
JPEGDIR = $(WXDIR)/src/jpeg
ZLIBDIR = $(WXDIR)/src/zlib
OLEDIR  = $(WXDIR)/src/msw/ole
MSWDIR  = $(WXDIR)/src/msw

DOCDIR = $(WXDIR)\docs

GENERICOBJS = \
                #$ ExpandList("WXGENERICOBJS");

COMMONOBJS  = \
		$(COMMDIR)/y_tab.$(OBJSUFF) \
		#$ ExpandList("WXCOMMONOBJS");

HTMLOBJS = \
                #$ ExpandList("WXHTMLOBJS");

MSWOBJS     = \
		#$ ExpandList("WXMSWOBJS");

ADVANCEDOBJS     = \
		#$ ExpandList("ADVANCEDOBJS");

ZLIBOBJS    = \
		$(ZLIBDIR)/adler32.$(OBJSUFF) \
		$(ZLIBDIR)/compress.$(OBJSUFF) \
		$(ZLIBDIR)/crc32.$(OBJSUFF) \
		$(ZLIBDIR)/gzio.$(OBJSUFF) \
		$(ZLIBDIR)/uncompr.$(OBJSUFF) \
		$(ZLIBDIR)/deflate.$(OBJSUFF) \
		$(ZLIBDIR)/trees.$(OBJSUFF) \
		$(ZLIBDIR)/zutil.$(OBJSUFF) \
		$(ZLIBDIR)/inflate.$(OBJSUFF) \
		$(ZLIBDIR)/infblock.$(OBJSUFF) \
		$(ZLIBDIR)/inftrees.$(OBJSUFF) \
		$(ZLIBDIR)/infcodes.$(OBJSUFF) \
		$(ZLIBDIR)/infutil.$(OBJSUFF) \
		$(ZLIBDIR)/inffast.$(OBJSUFF)

PNGOBJS     = \
		$(PNGDIR)/png.$(OBJSUFF) \
		$(PNGDIR)/pngread.$(OBJSUFF) \
		$(PNGDIR)/pngrtran.$(OBJSUFF) \
		$(PNGDIR)/pngrutil.$(OBJSUFF) \
		$(PNGDIR)/pngpread.$(OBJSUFF) \
		$(PNGDIR)/pngtrans.$(OBJSUFF) \
		$(PNGDIR)/pngwrite.$(OBJSUFF) \
		$(PNGDIR)/pngwtran.$(OBJSUFF) \
		$(PNGDIR)/pngwutil.$(OBJSUFF) \
		$(PNGDIR)/pngerror.$(OBJSUFF) \
		$(PNGDIR)/pngmem.$(OBJSUFF) \
		$(PNGDIR)/pngwio.$(OBJSUFF) \
		$(PNGDIR)/pngrio.$(OBJSUFF) \
		$(PNGDIR)/pngget.$(OBJSUFF) \
		$(PNGDIR)/pngset.$(OBJSUFF)


JPEGOBJS    = \
		$(JPEGDIR)/jcomapi.$(OBJSUFF) \
		$(JPEGDIR)/jutils.$(OBJSUFF) \
		$(JPEGDIR)/jerror.$(OBJSUFF) \
		$(JPEGDIR)/jmemmgr.$(OBJSUFF) \
		$(JPEGDIR)/jmemnobs.$(OBJSUFF) \
		$(JPEGDIR)/jcapimin.$(OBJSUFF) \
		$(JPEGDIR)/jcapistd.$(OBJSUFF) \
		$(JPEGDIR)/jctrans.$(OBJSUFF) \
		$(JPEGDIR)/jcparam.$(OBJSUFF) \
		$(JPEGDIR)/jdatadst.$(OBJSUFF) \
		$(JPEGDIR)/jcinit.$(OBJSUFF) \
		$(JPEGDIR)/jcmaster.$(OBJSUFF) \
		$(JPEGDIR)/jcmarker.$(OBJSUFF) \
		$(JPEGDIR)/jcmainct.$(OBJSUFF) \
		$(JPEGDIR)/jcprepct.$(OBJSUFF) \
		$(JPEGDIR)/jccoefct.$(OBJSUFF) \
		$(JPEGDIR)/jccolor.$(OBJSUFF) \
		$(JPEGDIR)/jcsample.$(OBJSUFF) \
		$(JPEGDIR)/jchuff.$(OBJSUFF) \
		$(JPEGDIR)/jcphuff.$(OBJSUFF) \
		$(JPEGDIR)/jcdctmgr.$(OBJSUFF) \
		$(JPEGDIR)/jfdctfst.$(OBJSUFF) \
		$(JPEGDIR)/jfdctflt.$(OBJSUFF) \
		$(JPEGDIR)/jfdctint.$(OBJSUFF) \
		$(JPEGDIR)/jdapimin.$(OBJSUFF) \
		$(JPEGDIR)/jdapistd.$(OBJSUFF) \
		$(JPEGDIR)/jdtrans.$(OBJSUFF) \
		$(JPEGDIR)/jdatasrc.$(OBJSUFF) \
		$(JPEGDIR)/jdmaster.$(OBJSUFF) \
		$(JPEGDIR)/jdinput.$(OBJSUFF) \
		$(JPEGDIR)/jdmarker.$(OBJSUFF) \
		$(JPEGDIR)/jdhuff.$(OBJSUFF) \
		$(JPEGDIR)/jdphuff.$(OBJSUFF) \
		$(JPEGDIR)/jdmainct.$(OBJSUFF) \
		$(JPEGDIR)/jdcoefct.$(OBJSUFF) \
		$(JPEGDIR)/jdpostct.$(OBJSUFF) \
		$(JPEGDIR)/jddctmgr.$(OBJSUFF) \
		$(JPEGDIR)/jidctfst.$(OBJSUFF) \
		$(JPEGDIR)/jidctflt.$(OBJSUFF) \
		$(JPEGDIR)/jidctint.$(OBJSUFF) \
		$(JPEGDIR)/jidctred.$(OBJSUFF) \
		$(JPEGDIR)/jdsample.$(OBJSUFF) \
		$(JPEGDIR)/jdcolor.$(OBJSUFF) \
		$(JPEGDIR)/jquant1.$(OBJSUFF) \
		$(JPEGDIR)/jquant2.$(OBJSUFF) \
		$(JPEGDIR)/jdmerge.$(OBJSUFF)

XPMOBJECTS = 	$(XPMDIR)/crbuffri.o\
		$(XPMDIR)/crdatfri.o\
		$(XPMDIR)/create.o $(XPMDIR)/crifrbuf.o\
		$(XPMDIR)/crifrdat.o\
		$(XPMDIR)/data.o\
		$(XPMDIR)/hashtab.o $(XPMDIR)/misc.o\
		$(XPMDIR)/parse.o $(XPMDIR)/rdftodat.o\
		$(XPMDIR)/rdftoi.o\
		$(XPMDIR)/rgb.o $(XPMDIR)/scan.o\
		$(XPMDIR)/simx.o $(XPMDIR)/wrffrdat.o\
		$(XPMDIR)/wrffrp.o $(XPMDIR)/wrffri.o

OBJECTS = $(MSWOBJS) $(COMMONOBJS) $(GENERICOBJS) $(HTMLOBJS) \
	  $(JPEGOBJS) $(PNGOBJS) $(ZLIBOBJS) # $(ADVANCEDOBJS) # $(XPMOBJECTS)

ifeq ($(MINGW32),1)
  ifeq ($(MINGW32VERSION),2.95)
    OBJECTS = $(MSWOBJS) $(COMMONOBJS) $(GENERICOBJS) $(HTMLOBJS) \
	  $(JPEGOBJS) $(PNGOBJS) $(ZLIBOBJS) $(ADVANCEDOBJS) # $(XPMOBJECTS)   
  else
    OBJECTS = $(MSWOBJS) $(COMMONOBJS) $(GENERICOBJS) $(HTMLOBJS) \
	  $(JPEGOBJS) $(PNGOBJS) $(ZLIBOBJS) # $(XPMOBJECTS)
  endif
else
  OBJECTS = $(MSWOBJS) $(COMMONOBJS) $(GENERICOBJS) $(HTMLOBJS) \
	  $(JPEGOBJS) $(PNGOBJS) $(ZLIBOBJS) # $(XPMOBJECTS)
endif

all:    $(OBJECTS) $(WXLIB)

$(WXLIB): $(OBJECTS) $(EXTRAOBJS)
	ar $(AROPTIONS) $@ $(EXTRAOBJS) $(OBJECTS)
	$(RANLIB) $@

$(OBJECTS):	$(WXINC)/wx/defs.h $(WXINC)/wx/object.h $(WXINC)/wx/setup.h

$(COMMDIR)/y_tab.$(OBJSUFF):    $(COMMDIR)/y_tab.c $(COMMDIR)/lex_yy.c
	$(CCLEX) -c $(CPPFLAGS) -DUSE_DEFINE -DYY_USE_PROTOS -o $@ $(COMMDIR)/y_tab.c

$(COMMDIR)/y_tab.c:     $(COMMDIR)/dosyacc.c
	copy ..\common\dosyacc.c ..\common\y_tab.c

$(COMMDIR)/lex_yy.c:    $(COMMDIR)/doslex.c
	copy ..\common\doslex.c ..\common\lex_yy.c

# Replace lex with flex if you run into compilation
# problems with lex_yy.c. See also note about LEX_SCANNER
# above.
# $(COMMDIR)/lex_yy.c:	$(COMMDIR)/lexer.l
#	$(LEX) -L -o$(COMMDIR)/lex_yy.c $(COMMDIR)/lexer.l
#
# Try one of these if the above line doesn't work.
# Alternative syntax (1)
#	$(LEX) -t -L $(COMMDIR)/lexer.l > $(COMMDIR)/lex_yy.c
# Alternative syntax (2)
#	$(LEX) -L -o$(COMMDIR)/lex_yy.c $(COMMDIR)/lexer.l
#
#	sed -e "s/BUFSIZ/5000/g" < lex.yy.c | \
#	sed -e "s/yyoutput(c)/void yyoutput(c)/g" | \
#       sed -e "s/YYLMAX 200/YYLMAX 5000/g" > lex_yy.c
#	rm -f lex.yy.c
#
# Replace yacc with bison if you run into compilation
# problems with y_tab.c.
#
# $(COMMDIR)/y_tab.c:	$(COMMDIR)/parser.y
#	$(YACC) -o $(COMMDIR)/y_tab.c $(COMMDIR)/parser.y
#
# If you use e.g. gcc on Unix, uncomment these lines
# and comment out the above.
#
# $(COMMDIR)/y_tab.c:	$(COMMDIR)/parser.y
#	$(YACC) $(COMMDIR)/parser.y
#	mv y.tab.c $(COMMDIR)/y_tab.c

clean:
	rm -f *.o
	rm -f *.bak
	rm -f core
	rm -f ..\common\y_tab.c
	rm -f ..\common\lex_yy.c
	rm -f ..\common\*.o
	rm -f ..\common\*.bak
	rm -f ..\generic\*.o
	rm -f ..\generic\*.bak
	rm -f ..\html\*.o
	rm -f ..\png\*.o
	rm -f ..\png\*.bak
	rm -f ..\zlib\*.o
	rm -f ..\zlib\*.bak
	rm -f ..\jpeg\*.o
	rm -f ..\..\lib\libwx.a

cleanall: clean
