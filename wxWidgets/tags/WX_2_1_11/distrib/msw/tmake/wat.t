#!#############################################################################
#! File:    wat.t
#! Purpose: tmake template file from which makefile.wat is generated by running
#!          tmake -t wat wxwin.pro -o makefile.wat
#!          TODO:
#!            - extended.c, unzip.c must be compiled with $(CC), not $(CCC).
#!            - extended.c, unzip.c targets must be as per b32.t etc.
#!            - OLE files not generated correctly (need 'ole/' directory)
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
        my $tag = "";
        if ( $wxGeneric{$file} =~ /\b(PS|G|16|U)\b/ ) {
            $tag = "WXNONESSENTIALOBJS";
        }
        else {
            $tag = "WXGENERICOBJS";
        }

        $file =~ s/cp?p?$/obj/;
        $project{$tag} .= $file . " "
    }

    foreach $file (sort keys %wxCommon) {
        #! doesn't compile, apparently
        next if $file =~ /^imagjpeg\./;

        $isCFile = $file =~ /\.c$/;
        $file =~ s/cp?p?$/obj/;
        $project{"WXCOMMONOBJS"} .= $file . " ";
        $project{"WXCOBJS"} .= $file . " " if $isCFile;
    }

    foreach $file (sort keys %wxMSW) {
        #! these files don't compile
        next if $file =~ /^pnghand\./;

        next if $wxGeneric{$file} =~ /\b16\b/;

        my $isOleObj = $wxMSW{$file} =~ /\bO\b/;
        my $isCFile = $file =~ /\.c$/;
        $file =~ s/cp?p?$/obj/;
        $project{"WXMSWOBJS"} .= $file . " ";
        $project{"WXCOBJS"} .= $file . " " if $isCFile;
        $project{"WXOLEOBJS"} .= $file . " " if $isOleObj
    }
#$}
#! an attempt to embed '#' directly in the string somehow didn't work...
#$ $text = chr(35) . '!/binb/wmake.exe';

# This file was automatically generated by tmake at #$ Now()
# DO NOT CHANGE THIS FILE, YOUR CHANGES WILL BE LOST! CHANGE WAT.T!

#
# File:     makefile.wat
# Author:   Julian Smart
# Created:  1998
#
# Makefile : Builds wxWindows library for Watcom C++, WIN32
#
# NOTE: This file is generated from wat.t by tmake, but not all bugs have
# been removed from this process. If wxWindows doesn't compile,
# check the following and edit this makefile accordingly:
#
# - OLE-related files such as oleutils.cpp should have 'ole\' prepended
#   to the path.
# - extended.c, gsocket.c, unzip.c must be compiled using $(CC), not $(CCC).
#   They may also be wrongly specified as extended.cpp, etc.

WXDIR = ..\..

!include $(WXDIR)\src\makewat.env

WXLIB = $(WXDIR)\lib

LIBTARGET   = $(WXLIB)\wx.lib
DUMMY=dummydll
# ODBCLIB     = ..\..\contrib\odbc\odbc32.lib

EXTRATARGETS = xpm png zlib
EXTRATARGETSCLEAN = clean_xpm clean_png clean_zlib
GENDIR=$(WXDIR)\src\generic
COMMDIR=$(WXDIR)\src\common
XPMDIR=$(WXDIR)\src\xpm
MSWDIR=$(WXDIR)\src\msw
OLEDIR=$(MSWDIR)\ole

DOCDIR = $(WXDIR)\docs

GENERICOBJS= #$ ExpandGlue("WXGENERICOBJS", "", " &\n\t")

# These are generic things that don't need to be compiled on MSW,
# but sometimes it's useful to do so for testing purposes.
NONESSENTIALOBJS= #$ ExpandGlue("WXNONESSENTIALOBJS", "", " &\n\t")

COMMONOBJS = &
	y_tab.obj &
	#$ ExpandGlue("WXCOMMONOBJS", "", " &\n\t")

MSWOBJS = #$ ExpandGlue("WXMSWOBJS", "", " &\n\t")

# Add $(NONESSENTIALOBJS) if wanting generic dialogs, PostScript etc.
OBJECTS = $(COMMONOBJS) $(GENERICOBJS) $(MSWOBJS)

all:        $(OBJECTS) $(LIBTARGET) $(EXTRATARGETS)

$(LIBTARGET) : $(OBJECTS)
    %create tmp.lbc
    @for %i in ( $(OBJECTS) ) do @%append tmp.lbc +%i
    wlib /b /c /n /p=512 $^@ @tmp.lbc

#test : $(OBJECTS)
#    %create tmp.lbc
#    @for %i in ( $(OBJECTS) ) do @%append tmp.lbc +%i
#    wlib /b /c /n /p=512 $^@ @tmp.lbc


clean:   .SYMBOLIC $(EXTRATARGETSCLEAN)
    -erase *.obj
    -erase $(LIBTARGET)
    -erase *.pch
    -erase *.err
    -erase *.lbc

cleanall:   clean

#${
    $_ = $project{"WXMSWOBJS"};
    my @objs = split;
    foreach (@objs) {
        $text .= $_ . ':     $(';
        s/\.obj$//;
        if ( $project{"WXOLEOBJS"} =~ /\b\Q$_\E\b/ ) {
            $text .= 'OLEDIR)\\';
        } else {
            $text .= 'MSWDIR)\\';
        }
        my $suffix, $cc;
        if ( $project{"WXCOBJS"} =~ /\b\Q$_\E\b/ ) {
            $suffix = "c";
            $cc="CC";
        }
        else {
            $suffix = "cpp";
            $cc="CCC";
        }
        $text .= $_ . ".$suffix\n" .
                 "  *\$($cc) \$(CPPFLAGS) \$(IFLAGS) \$<" . "\n\n";
    }
#$}

########################################################
# Common objects (always compiled)

#${
    $_ = $project{"WXCOMMONOBJS"};
    my @objs = split;
    foreach (@objs) {
        $text .= $_;
        s/\.obj$//;
        $text .= ':     $(COMMDIR)\\';
        my $suffix, $cc;
        if ( $project{"WXCOBJS"} =~ /\b\Q$_\E\b/ ) {
            $suffix = "c";
            $cc="CC";
        }
        else {
            $suffix = "cpp";
            $cc="CCC";
        }
        $text .= $_ . ".$suffix\n" .
                 "  *\$($cc) \$(CPPFLAGS) \$(IFLAGS) \$<" . "\n\n";
    }
#$}

y_tab.obj:     $(COMMDIR)\y_tab.c $(COMMDIR)\lex_yy.c
  *$(CC) $(CPPFLAGS) $(IFLAGS) -DUSE_DEFINE $(COMMDIR)\y_tab.c

#  *$(CC) $(CPPFLAGS) $(IFLAGS) -DUSE_DEFINE -DYY_USE_PROTOS $(COMMDIR)\y_tab.c

$(COMMDIR)\y_tab.c:     $(COMMDIR)\dosyacc.c
        copy $(COMMDIR)\dosyacc.c $(COMMDIR)\y_tab.c

$(COMMDIR)\lex_yy.c:    $(COMMDIR)\doslex.c
    copy $(COMMDIR)\doslex.c $(COMMDIR)\lex_yy.c

########################################################
# Generic objects (not always compiled, depending on
# whether platforms have native implementations)

#${
    $_ = $project{"WXGENERICOBJS"};
    my @objs = split;
    foreach (@objs) {
        $text .= $_;
        s/\.obj$//;
        $text .= ':     $(GENDIR)\\';
        $text .= $_ . ".cpp\n" .
                 '  *$(CCC) $(CPPFLAGS) $(IFLAGS) $<' . "\n\n";
    }
#$}

crbuffri.obj: $(XPMDIR)\crbuffri.c
  *$(CC) $(CPPFLAGS) $(IFLAGS) $<

crbuffrp.obj: $(XPMDIR)\crbuffrp.c
  *$(CC) $(CPPFLAGS) $(IFLAGS) $<

crdatfri.obj: $(XPMDIR)\crdatfri.c
  *$(CC) $(CPPFLAGS) $(IFLAGS) $<

crdatfrp.obj: $(XPMDIR)\crdatfrp.c
  *$(CC) $(CPPFLAGS) $(IFLAGS) $<

create.obj: $(XPMDIR)\create.c
  *$(CC) $(CPPFLAGS) $(IFLAGS) $<

crifrbuf.obj: $(XPMDIR)\crifrbuf.c
  *$(CC) $(CPPFLAGS) $(IFLAGS) $<

crifrdat.obj: $(XPMDIR)\crifrdat.c
  *$(CC) $(CPPFLAGS) $(IFLAGS) $<

crpfrbuf.obj: $(XPMDIR)\crpfrbuf.c
  *$(CC) $(CPPFLAGS) $(IFLAGS) $<

crpfrdat.obj: $(XPMDIR)\crpfrdat.c
  *$(CC) $(CPPFLAGS) $(IFLAGS) $<

# TODO: what to do about this clash of filename????
#data.obj: $(XPMDIR)\data.c
#  *$(CC) $(CPPFLAGS) $(IFLAGS) $<

hashtab.obj: $(XPMDIR)\hashtab.c
  *$(CC) $(CPPFLAGS) $(IFLAGS) $<

misc.obj: $(XPMDIR)\misc.c
  *$(CC) $(CPPFLAGS) $(IFLAGS) $<

parse.obj: $(XPMDIR)\parse.c
  *$(CC) $(CPPFLAGS) $(IFLAGS) $<

rdftodat.obj: $(XPMDIR)\rdftodat.c
  *$(CC) $(CPPFLAGS) $(IFLAGS) $<

rdftoi.obj: $(XPMDIR)\rdftoi.c
  *$(CC) $(CPPFLAGS) $(IFLAGS) $<

rdftop.obj: $(XPMDIR)\rdftop.c
  *$(CC) $(CPPFLAGS) $(IFLAGS) $<

rgb.obj: $(XPMDIR)\rgb.c
  *$(CC) $(CPPFLAGS) $(IFLAGS) $<

scan.obj: $(XPMDIR)\scan.c
  *$(CC) $(CPPFLAGS) $(IFLAGS) $<

simx.obj: $(XPMDIR)\simx.c
  *$(CC) $(CPPFLAGS) $(IFLAGS) $<

wrffrdat.obj: $(XPMDIR)\wrffrdat.c
  *$(CC) $(CPPFLAGS) $(IFLAGS) $<

wrffri.obj: $(XPMDIR)\wrffri.c
  *$(CC) $(CPPFLAGS) $(IFLAGS) $<

wrffrp.obj: $(XPMDIR)\wrffrp.c
  *$(CC) $(CPPFLAGS) $(IFLAGS) $<

OBJ1 = adler32$(O) compress$(O) crc32$(O) gzio$(O) uncompr$(O) deflate$(O) \
  trees$(O)
OBJ2 = zutil$(O) inflate$(O) infblock$(O) inftrees$(O) infcodes$(O) \
  infutil$(O) inffast$(O)

adler32.obj: adler32.c zutil.h zlib.h zconf.h
	$(CC) -c $(CFLAGS) $*.c

compress.obj: compress.c zlib.h zconf.h
	$(CC) -c $(CFLAGS) $*.c

crc32.obj: crc32.c zutil.h zlib.h zconf.h
	$(CC) -c $(CFLAGS) $*.c

deflate.obj: deflate.c deflate.h zutil.h zlib.h zconf.h
	$(CC) -c $(CFLAGS) $*.c

gzio.obj: gzio.c zutil.h zlib.h zconf.h
	$(CC) -c $(CFLAGS) $*.c

infblock.obj: infblock.c zutil.h zlib.h zconf.h infblock.h inftrees.h\
   infcodes.h infutil.h
	$(CC) -c $(CFLAGS) $*.c

infcodes.obj: infcodes.c zutil.h zlib.h zconf.h inftrees.h infutil.h\
   infcodes.h inffast.h
	$(CC) -c $(CFLAGS) $*.c

inflate.obj: inflate.c zutil.h zlib.h zconf.h infblock.h
	$(CC) -c $(CFLAGS) $*.c

inftrees.obj: inftrees.c zutil.h zlib.h zconf.h inftrees.h
	$(CC) -c $(CFLAGS) $*.c

infutil.obj: infutil.c zutil.h zlib.h zconf.h inftrees.h infutil.h
	$(CC) -c $(CFLAGS) $*.c

inffast.obj: inffast.c zutil.h zlib.h zconf.h inftrees.h infutil.h inffast.h
	$(CC) -c $(CFLAGS) $*.c

trees.obj: trees.c deflate.h zutil.h zlib.h zconf.h
	$(CC) -c $(CFLAGS) $*.c

uncompr.obj: uncompr.c zlib.h zconf.h
	$(CC) -c $(CFLAGS) $*.c

zutil.obj: zutil.c zutil.h zlib.h zconf.h
	$(CC) -c $(CFLAGS) $*.c

xpm:   .SYMBOLIC
    cd $(WXDIR)\src\xpm
    wmake -f makefile.wat all
    cd $(WXDIR)\src\msw

clean_xpm:   .SYMBOLIC
    cd $(WXDIR)\src\xpm
    wmake -f makefile.wat clean
    cd $(WXDIR)\src\msw

png:   .SYMBOLIC
    cd $(WXDIR)\src\png
    wmake -f makefile.wat all
    cd $(WXDIR)\src\msw

clean_png:   .SYMBOLIC
    cd $(WXDIR)\src\png
    wmake -f makefile.wat clean
    cd $(WXDIR)\src\msw

zlib:   .SYMBOLIC
    cd $(WXDIR)\src\zlib
    wmake -f makefile.wat all
    cd $(WXDIR)\src\msw

clean_zlib:   .SYMBOLIC
    cd $(WXDIR)\src\zlib
    wmake -f makefile.wat clean
    cd $(WXDIR)\src\msw

MFTYPE=wat
makefile.$(MFTYPE) : $(WXWIN)\distrib\msw\tmake\filelist.txt $(WXWIN)\distrib\msw\tmake\$(MFTYPE).t
	cd $(WXWIN)\distrib\msw\tmake
	tmake -t $(MFTYPE) wxwin.pro -o makefile.$(MFTYPE)
	copy makefile.$(MFTYPE) $(WXWIN)\src\msw
