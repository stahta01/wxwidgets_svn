#!#############################################################################
#! File:    g95.t
#! Purpose: tmake template file from which makefile.g95 is generated by running
#!          tmake -t g95 wxwin.pro -o makefile.g95
#! Author:  Vadim Zeitlin
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
        next if $file =~ /^odbc\./;

        $file =~ s/cp?p?$/\$(OBJSUFF)/;
        $project{"WXCOMMONOBJS"} .= '$(COMMDIR)/' . $file . " "
    }

    foreach $file (sort keys %wxMSW) {
        #! Mingw32 doesn't have the OLE headers and has some troubles with
        #! socket code
        next if $wxMSW{$file} =~ /\b(O|16)\b/;

        #! native wxDirDlg can't be compiled due to GnuWin32/OLE limitations,
        next if $file =~ /^dirdlg\./;

        $file =~ s/cp?p?$/\$(OBJSUFF)/;
        $project{"WXMSWOBJS"} .= $file . " "
    }
#$}

# This file was automatically generated by tmake at #$ Now()
# DO NOT CHANGE THIS FILE, YOUR CHANGES WILL BE LOST! CHANGE G95.T!

#
# File:     makefile.unx
# Author:   Julian Smart
# Created:  1993
# Updated:
# Copyright:(c) 1993, AIAI, University of Edinburgh
#
# "@(#)makefile.unx 1.2 5/9/94"
#
# Makefile for libwx.a

# Replace this with your own path if necessary
WXDIR = ../..

# All common UNIX compiler flags and options are now in
# this central makefile.
include $(WXDIR)/src/makeg95.env

# IMPORTANT: if you want to link in subsidiary libraries,
# required for some optional parts of wxWindows, you must
# set EXTRATARGETS and EXTRAOBJS to match the settings in wx_setup.h.
# Having done this, you won't have to worry about linking them
# into each application.
#
# Here's the full list.
#   prologio: Required if using the wxWindows resource facility
#               USE_WX_RESOURCES should be 1
#   xmgauge:  Gauge class (required for Motif only)
#               USE_GAUGE should be 1
#   xpm:      XPM pixmap support
#               USE_XPM_IN_X should be 1
#   wxstring: string class
#               USE_GNU_WXSTRING should be 1
#
# Unfortunately, 'ar' doesn't like combining objects and libraries,
# so we must identify all the objects from the subordinate libraries. Yuck!
XPMSRC=../../contrib/wxxpm/objects$(GUISUFFIX)
XPMOBJS=$(XPMSRC)/crbuffri.$(OBJSUFF) $(XPMSRC)/crbuffrp.$(OBJSUFF) $(XPMSRC)/crdatfri.$(OBJSUFF) $(XPMSRC)/crdatfrp.$(OBJSUFF)\
  $(XPMSRC)/create.$(OBJSUFF) $(XPMSRC)/crifrbuf.$(OBJSUFF) $(XPMSRC)/crifrdat.$(OBJSUFF) $(XPMSRC)/crpfrbuf.$(OBJSUFF) $(XPMSRC)/crpfrdat.$(OBJSUFF)\
  $(XPMSRC)/data.$(OBJSUFF) $(XPMSRC)/hashtab.$(OBJSUFF) $(XPMSRC)/misc.$(OBJSUFF) $(XPMSRC)/parse.$(OBJSUFF) $(XPMSRC)/rdftodat.$(OBJSUFF)\
  $(XPMSRC)/rdftoi.$(OBJSUFF) $(XPMSRC)/rdftop.$(OBJSUFF) $(XPMSRC)/rgb.$(OBJSUFF) $(XPMSRC)/scan.$(OBJSUFF) $(XPMSRC)/simx.$(OBJSUFF)\
  $(XPMSRC)/wrffrdat.$(OBJSUFF) $(XPMSRC)/wrffri.$(OBJSUFF) $(XPMSRC)/wrffrp.$(OBJSUFF)

# Subordinate library possibilities

EXTRAOBJS=

GENDIR=../generic
COMMDIR=../common
XPMDIR=../xpm
OLEDIR=ole
MSWDIR=.

DOCDIR = $(WXDIR)\docs

GENERICOBJS= #$ ExpandList("WXGENERICOBJS");

COMMONOBJS = \
		$(COMMDIR)/y_tab.$(OBJSUFF) \
		#$ ExpandList("WXCOMMONOBJS");

MSWOBJS = #$ ExpandList("WXMSWOBJS");

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

OBJECTS = $(MSWOBJS) $(COMMONOBJS) $(GENERICOBJS) # $(XPMOBJECTS)

all:    $(OBJECTS) $(WXLIB)

base:
	cd $(WXDIR)/src/common; $(MAKE) -f makefile.g95 GUI=$(GUI) GUISUFFIX=$(GUISUFFIX) CC=$(CC)\
 OPTIONS='$(OPTIONS)' DEBUG='$(DEBUG)' DEBUGFLAGS='$(DEBUGFLAGS)' WARN='$(WARN)' XLIB='$(XLIB)' XINCLUDE='$(XINCLUDE)'


$(WXLIB): $(OBJECTS) $(EXTRAOBJS)
	ar $(AROPTIONS) $@ $(EXTRAOBJS) $(OBJECTS)
	$(RANLIB) $@

$(OBJECTS):	$(WXINC)/wx/defs.h $(WXINC)/wx/object.h $(WXINC)/wx/setup.h

$(COMMDIR)/y_tab.$(OBJSUFF):    $(COMMDIR)/y_tab.c $(COMMDIR)/lex_yy.c
	$(CCLEX) -c $(CPPFLAGS) -o $@ $(COMMDIR)/y_tab.c

# Replace lex with flex if you run into compilation
# problems with lex_yy.c. See also note about LEX_SCANNER
# above.
$(COMMDIR)/lex_yy.c:	$(COMMDIR)/lexer.l
	$(LEX) -L -o$(COMMDIR)/lex_yy.c $(COMMDIR)/lexer.l

# Try one of these if the above line doesn't work.
# Alternative syntax (1)
#	$(LEX) -t -L $(COMMDIR)/lexer.l > $(COMMDIR)/lex_yy.c
# Alternative syntax (2)
#	$(LEX) -L -o$(COMMDIR)/lex_yy.c $(COMMDIR)/lexer.l

#	sed -e "s/BUFSIZ/5000/g" < lex.yy.c | \
#	sed -e "s/yyoutput(c)/void yyoutput(c)/g" | \
#       sed -e "s/YYLMAX 200/YYLMAX 5000/g" > lex_yy.c
#	rm -f lex.yy.c

# Replace yacc with bison if you run into compilation
# problems with y_tab.c.

$(COMMDIR)/y_tab.c:	$(COMMDIR)/parser.y
	$(YACC) -o $(COMMDIR)/y_tab.c $(COMMDIR)/parser.y

# If you use e.g. gcc on Unix, uncomment these lines
# and comment out the above.

# $(COMMDIR)/y_tab.c:	$(COMMDIR)/parser.y
#	$(YACC) $(COMMDIR)/parser.y
#	mv y.tab.c $(COMMDIR)/y_tab.c

# Extra targets
rcparser:
	cd $(WXDIR)/utils/rcparser/src; $(MAKE)  -f makefile.g95
	cd $(WXDIR)/src/msw

clean_rcp:
	cd $(WXDIR)/utils/rcparser/src; $(MAKE) -f makefile.g95 clean
	cd $(WXDIR)/src/msw

clean:
	erase *.o
	erase ../common/y_tab.c
	erase ../common/lex_yy.c
	erase $(WXDIR)/lib/libwx$(GUISUFFIX).a
	erase core

cleanall: clean

