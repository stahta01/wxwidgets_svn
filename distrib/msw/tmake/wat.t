#!#############################################################################
#! File:    wat.t
#! Purpose: tmake template file from which makefile.wat is generated by running
#!          tmake -t wat wxwin.pro -o makefile.wat
#! Author:  Vadim Zeitlin
#! Created: 14.07.99
#! Version: $Id$
#! Changelist: 2003-02-25 - Juergen Ulbts - update from wxWindows 2.5.x/HEAD branch
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
    
    foreach $file (sort keys %wxHTML) {
        next if $wxHTML{$file} =~ /\b16\b/;

        $file =~ s/cp?p?$/obj/;
        $project{"WXHTMLOBJS"} .= $file . " "
    }

    foreach $file (sort keys %wxCommon) {
        next if $wxCommon{$file} =~ /\b(16|U)\b/;

        $isCFile = $file =~ /\.c$/;
        $file =~ s/cp?p?$/obj/;
        $project{"WXCOMMONOBJS"} .= $file . " ";
        $project{"WXCOBJS"} .= $file . " " if $isCFile;
    }

    foreach $file (sort keys %wxMSW) {
        #! these files don't compile
        next if $file =~ /^pnghand\./;

        next if $wxMSW{$file} =~ /\b16\b/;

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

# This file was automatically generated by tmake 
# DO NOT CHANGE THIS FILE, YOUR CHANGES WILL BE LOST! CHANGE WAT.T!

#
# File:     makefile.wat
# Author:   Julian Smart
# Created:  1998
#
# Makefile : Builds wxWindows library for Watcom C++, WIN32
!include ..\makewat.env

LIBTARGET   = $(WXDIR)\lib\$(LIBNAME).lib

EXTRATARGETS = png zlib jpeg tiff regex
EXTRATARGETSCLEAN = clean_png clean_zlib clean_jpeg clean_tiff clean_regex
GENDIR=$(WXDIR)\src\generic
COMMDIR=$(WXDIR)\src\common
JPEGDIR=$(WXDIR)\src\jpeg
TIFFDIR=$(WXDIR)\src\tiff
MSWDIR=$(WXDIR)\src\msw
OLEDIR=$(MSWDIR)\ole
HTMLDIR=$(WXDIR)\src\html

DOCDIR = $(WXDIR)\docs

GENERICOBJS= #$ ExpandGlue("WXGENERICOBJS", "\$(OUTPUTDIR)\\", " &\n\t\$(OUTPUTDIR)\\")

# These are generic things that don't need to be compiled on MSW,
# but sometimes it's useful to do so for testing purposes.
NONESSENTIALOBJS= #$ ExpandGlue("WXNONESSENTIALOBJS", "\$(OUTPUTDIR)\\", " &\n\t\$(OUTPUTDIR)\\")

COMMONOBJS = &
	$(OUTPUTDIR)\y_tab.obj &
	#$ ExpandGlue("WXCOMMONOBJS", "\$(OUTPUTDIR)\\", " &\n\t\$(OUTPUTDIR)\\")

MSWOBJS = #$ ExpandGlue("WXMSWOBJS", "\$(OUTPUTDIR)\\", " &\n\t\$(OUTPUTDIR)\\")

HTMLOBJS = #$ ExpandGlue("WXHTMLOBJS", "\$(OUTPUTDIR)\\", " &\n\t\$(OUTPUTDIR)\\")

# Add $(NONESSENTIALOBJS) if wanting generic dialogs, PostScript etc.
OBJECTS = $(COMMONOBJS) $(GENERICOBJS) $(MSWOBJS) $(HTMLOBJS)

SETUP_H=$(ARCHINCDIR)\wx\setup.h

all: $(SETUP_H) $(OUTPUTDIR) $(OBJECTS) $(LIBTARGET) $(EXTRATARGETS) .SYMBOLIC

$(ARCHINCDIR)\wx:
	mkdir $(ARCHINCDIR)
	mkdir $(ARCHINCDIR)\wx

$(OUTPUTDIR):
	@if not exist $^@ mkdir $^@

$(SETUP_H): $(WXDIR)\include\wx\msw\setup.h $(ARCHINCDIR)\wx
	copy $(WXDIR)\include\wx\msw\setup.h $@

LBCFILE=wx$(TOOLKIT).lbc
$(LIBTARGET) : $(OBJECTS)
    %create $(LBCFILE)
    @for %i in ( $(OBJECTS) ) do @%append $(LBCFILE) +%i
    wlib /b /c /n $(LIBPAGESIZE) $^@ @$(LBCFILE)


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
        $text .= "\$(OUTPUTDIR)\\" . $_ . ':     $(';
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
            $flags="CFLAGS";
        }
        else {
            $suffix = "cpp";
            $cc="CXX";
            $flags="CXXFLAGS";
        }
        $text .= $_ . ".$suffix\n" .
                 "  *\$($cc) \$($flags) \$<" . "\n\n";
    }
#$}

########################################################
# Common objects (always compiled)

#${
    $_ = $project{"WXCOMMONOBJS"};
    my @objs = split;
    foreach (@objs) {
        $text .= "\$(OUTPUTDIR)\\" . $_;
        s/\.obj$//;
        $text .= ':     $(COMMDIR)\\';
        my $suffix, $cc;
        if ( $project{"WXCOBJS"} =~ /\b\Q$_\E\b/ ) {
            $suffix = "c";
            $cc="CC";
            $flags="CFLAGS";
        }
        else {
            $suffix = "cpp";
            $cc="CXX";
            $flags="CXXFLAGS";
        }
        $text .= $_ . ".$suffix\n" .
                 "  *\$($cc) \$($flags) \$<" . "\n\n";
    }
#$}

$(OUTPUTDIR)\y_tab.obj:     $(COMMDIR)\y_tab.c $(COMMDIR)\lex_yy.c
  *$(CC) $(CFLAGS) -DUSE_DEFINE $(COMMDIR)\y_tab.c

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
        $text .= "\$(OUTPUTDIR)\\" . $_;
        s/\.obj$//;
        $text .= ':     $(GENDIR)\\';
        $text .= $_ . ".cpp\n" .
                 '  *$(CXX) $(CXXFLAGS) $<' . "\n\n";
    }
#$}


########################################################
# HTML objects (always compiled)

#${
    $_ = $project{"WXHTMLOBJS"};
    my @objs = split;
    foreach (@objs) {
        $text .= "\$(OUTPUTDIR)\\" . $_;
        s/\.obj$//;
        $text .= ':     $(HTMLDIR)\\';
        $text .= $_ . ".cpp\n" .
                 '  *$(CXX) $(CXXFLAGS) $<' . "\n\n";
    }
#$}

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

jpeg:    .SYMBOLIC
    cd $(WXDIR)\src\jpeg
    wmake -f makefile.wat all
    cd $(WXDIR)\src\msw

clean_jpeg:   .SYMBOLIC
    cd $(WXDIR)\src\jpeg
    wmake -f makefile.wat clean
    cd $(WXDIR)\src\msw

tiff:    .SYMBOLIC
    cd $(WXDIR)\src\tiff
    wmake -f makefile.wat all
    cd $(WXDIR)\src\msw

clean_tiff:   .SYMBOLIC
    cd $(WXDIR)\src\tiff
    wmake -f makefile.wat clean
    cd $(WXDIR)\src\msw

regex:    .SYMBOLIC
    cd $(WXDIR)\src\regex
    wmake -f makefile.wat all
    cd $(WXDIR)\src\msw

clean_regex:   .SYMBOLIC
    cd $(WXDIR)\src\regex
    wmake -f makefile.wat clean
    cd $(WXDIR)\src\msw

MFTYPE=wat
self : .SYMBOLIC $(WXDIR)\distrib\msw\tmake\filelist.txt $(WXDIR)\distrib\msw\tmake\$(MFTYPE).t
	cd $(WXDIR)\distrib\msw\tmake
	tmake -t $(MFTYPE) wxwin.pro -o makefile.$(MFTYPE)
	copy makefile.$(MFTYPE) $(WXDIR)\src\msw
