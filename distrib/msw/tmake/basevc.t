#!#############################################################################
#! File:    basevc.t
#! Purpose: tmake template file from which makebase.vc is generated by running
#!          tmake -t basevc wxwin.pro -o makebase.vc
#! Author:  Mark Johnson   
#! Created: 12.03.2001
#! Version: $Id$
#!#############################################################################
#${
    #! include the code which parses filelist.txt file and initializes
    #! %wxCommon, %wxGeneric and %wxMSW hashes.
    IncludeTemplate("filelist.t");

    #! now transform these hashes into $project tags
    foreach $file (sort keys %wxCommon) {
        next if $wxCommon{$file} !~ /\bB\b/;

        $file =~ s/cp?p?$/obj/;
        $project{"WXCOMMONOBJS"} .= "..\\common\\\$D\\" . $file . " "
    }

    foreach $file (sort keys %wxMSW) {
        next if $wxMSW{$file} !~ /\bB\b/;

        $file =~ s/cp?p?$/obj/;
        $project{"WXMSWOBJS"} .= "..\\msw\\\$D\\" . $file . " "
    }

    foreach $file (sort keys %wxBase) {
        $file =~ s/cp?p?$/obj/;
        $project{"WXCOMMONOBJS"} .= "..\\common\\\$D\\" . $file . " "
    }
#$}
# This file was automatically generated by tmake at #$ Now()
# DO NOT CHANGE THIS FILE, YOUR CHANGES WILL BE LOST! CHANGE BASEVC.T!

# File:     makebase.vc
# Author:   Mark Johnson
# Created:  12 March 2001
# Updated:
# Copyright: (c) 1997, Julian Smart
#
# "%W% %G%"
#
# Makefile : Builds wxWindows library wxBase(d).lib for VC++ (32-bit)
# Arguments:
#
# FINAL=1 argument to nmake to build version with no debugging info.
# 
# wxUSE_GUI=0 argument to build wxWindows version with no GUI (Console)
# - makevc.env changes lib name to wxBase(d).lib
#
wxUSE_GUI = 0
!include <../makevc.env>

THISDIR=$(WXWIN)\src\msw

LIBTARGET=$(WXLIB)
DUMMYOBJ=$D/dummy.obj

# This one overrides the others, to be consistent with the settings in setup.h
MINIMAL_WXWINDOWS_SETUP=0

PERIPH_LIBS=
PERIPH_TARGET=
PERIPH_CLEAN_TARGET=

# These are absolute paths, so that the compiler
# generates correct __FILE__ symbols for debugging.
# Otherwise you don't be able to double-click on a memory
# error to load that file.
COMMDIR=$(WXDIR)\src\common
MSWDIR=$(WXDIR)\src\msw

{../common}.cpp{../common/$D}.obj:
  cl @<<
$(CPPFLAGS) /Fo$@ /c /Tp $<
<<

{../common}.c{../common/$D}.obj:
  cl @<<
$(CPPFLAGS2) /Fo$@ /c /Tc $<
<<

{../msw}.cpp{../msw/$D}.obj:
  cl @<<
$(CPPFLAGS) /Fo$@ /c /Tp $<
<<

{../msw}.c{../msw/$D}.obj:
  cl @<<
$(CPPFLAGS2) /Fo$@ /c /Tc $<
<<

COMMONOBJS = \
    ..\common\$D\y_tab.obj \
    #$ ExpandList("WXCOMMONOBJS");

MSWOBJS = #$ ExpandList("WXMSWOBJS");

OBJECTS = $(COMMONOBJS) $(MSWOBJS) 

# Normal, static library
#all:    setuph dirs $(DUMMYOBJ) $(OBJECTS) $(PERIPH_TARGET) png zlib xpm jpeg tiff $(LIBTARGET)
all:    setuph dirs $(DUMMYOBJ) $(OBJECTS) $(PERIPH_TARGET) zlib $(LIBTARGET)

setuph:
    cd $(WXDIR)\include\wx\msw
    if not exist setup.h copy setup0.h setup.h
    cd $(WXDIR)\src\msw

dirs: $(MSWDIR)\$D $(COMMDIR)\$D 

$D:
    mkdir $D

$(COMMDIR)\$D:
    mkdir $(COMMDIR)\$D

$(MSWDIR)\$D:
    mkdir $(MSWDIR)\$D

### Static library

$(WXDIR)\lib\$(WXLIBNAME).lib:      $D\dummy.obj $(OBJECTS) 
  -erase $(LIBTARGET)
  $(implib) @<<
-out:$@
-machine:$(CPU)
$(OBJECTS) $D\dummy.obj $(PERIPH_LIBS)
<<

$D\dummy.obj: dummy.$(SRCSUFF) $(WXDIR)\include\wx\wx.h $(WXDIR)\include\wx\msw\setup.h
        cl $(CPPFLAGS) $(MAKEPRECOMP) /Fo$D\dummy.obj /c /Tp dummy.cpp
########################################################

..\common\$D\y_tab.obj:     ..\common\y_tab.c ..\common\lex_yy.c
        cl @<<
$(CPPFLAGS2) /c ..\common\y_tab.c -DUSE_DEFINE -DYY_USE_PROTOS /Fo$@
<<

..\common\y_tab.c:     ..\common\dosyacc.c
        copy "..\common"\dosyacc.c "..\common"\y_tab.c

..\common\lex_yy.c:    ..\common\doslex.c
    copy "..\common"\doslex.c "..\common"\lex_yy.c

$(OBJECTS): $(WXDIR)/include/wx/setup.h

..\common\$D\unzip.obj:     ..\common\unzip.c
        cl @<<
$(CPPFLAGS2) /c $(COMMDIR)\unzip.c /Fo$@
<<

png:
    cd $(WXDIR)\src\png
    nmake -f makefile.vc FINAL=$(FINAL) DLL=$(DLL) WXMAKINGDLL=$(WXMAKINGDLL) CRTFLAG=$(CRTFLAG)
    cd $(WXDIR)\src\msw

clean_png:
    cd $(WXDIR)\src\png
    nmake -f makefile.vc clean
    cd $(WXDIR)\src\msw

zlib:
    cd $(WXDIR)\src\zlib
    nmake -f makefile.vc FINAL=$(FINAL) DLL=$(DLL) WXMAKINGDLL=$(WXMAKINGDLL) CRTFLAG=$(CRTFLAG)
    cd $(WXDIR)\src\msw

clean_zlib:
    cd $(WXDIR)\src\zlib
    nmake -f makefile.vc clean
    cd $(WXDIR)\src\msw

jpeg:
    cd $(WXDIR)\src\jpeg
    nmake -f makefile.vc FINAL=$(FINAL) DLL=$(DLL) WXMAKINGDLL=$(WXMAKINGDLL) CRTFLAG=$(CRTFLAG) all
    cd $(WXDIR)\src\msw

clean_jpeg:
    cd $(WXDIR)\src\jpeg
    nmake -f makefile.vc clean
    cd $(WXDIR)\src\msw

tiff:
    cd $(WXDIR)\src\tiff
    nmake -f makefile.vc FINAL=$(FINAL) DLL=$(DLL) WXMAKINGDLL=$(WXMAKINGDLL) CRTFLAG=$(CRTFLAG) all
    cd $(WXDIR)\src\msw

clean_tiff:
    cd $(WXDIR)\src\tiff
    nmake -f makefile.vc clean
    cd $(WXDIR)\src\msw

xpm:
    cd $(WXDIR)\src\xpm
    nmake -f makefile.vc FINAL=$(FINAL) DLL=$(DLL) WXMAKINGDLL=$(WXMAKINGDLL) CRTFLAG=$(CRTFLAG)
    cd $(WXDIR)\src\msw

clean_xpm:
    cd $(WXDIR)\src\xpm
    nmake -f makefile.vc clean
    cd $(WXDIR)\src\msw

cleanall: clean clean_png clean_zlib clean_xpm clean_jpeg clean_tiff
        -erase ..\..\lib\wx$(WXVERSION)$(LIBEXT).lib
        -erase ..\..\lib\wx$(WXVERSION)$(LIBEXT).exp
        -erase ..\..\lib\wx$(WXVERSION)$(LIBEXT).pdb
        -erase ..\..\lib\wx$(WXVERSION)$(LIBEXT).ilk


clean: $(PERIPH_CLEAN_TARGET)
        -erase $(LIBTARGET)
        -erase $(COMMDIR)\$D\*.obj
        -erase $(COMMDIR)\$D\*.pdb
        -erase $(COMMDIR)\$D\*.sbr
        -erase $(COMMDIR)\$D\*.idb
        -erase $(COMMDIR)\$D\*.pch
        -erase $(COMMDIR)\y_tab.c
        -erase $(COMMDIR)\lex_yy.c
        -erase $(MSWDIR)\$D\*.idb
        -erase $(MSWDIR)\$D\*.pdb
        -erase $(MSWDIR)\$D\*.pch
        -erase $(MSWDIR)\$D\*.obj
        -erase $(MSWDIR)\$D\*.sbr
        -erase $(MSWDIR)\$D\*.pdb
        -rmdir $(D)
        -rmdir ..\common\$(D)

