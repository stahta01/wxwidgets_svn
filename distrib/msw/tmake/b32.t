#!#############################################################################
#! File:    b32.t
#! Purpose: tmake template file from which makefile.b32 is generated by running
#!          tmake -t b32 wxwin.pro
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
        next if $wxGeneric{$file} =~ /\b(PS|G|16|U)\b/;

        $file =~ s/cp?p?$/obj/;
        $project{"WXGENERICOBJS"} .= "\$(MSWDIR)\\" . $file . " "
    }

    foreach $file (sort keys %wxHTML) {
        next if $wxHTML{$file} =~ /\b16\b/;

        $file =~ s/cp?p?$/obj/;
        $project{"WXHTMLOBJS"} .= "\$(MSWDIR)\\" . $file . " "
    }

    foreach $file (sort keys %wxCommon) {
        next if $wxCommon{$file} =~ /\b(16|U)\b/;

        $isCFile = $file =~ /\.c$/;
        $file =~ s/cp?p?$/obj/;
        $obj = "\$(MSWDIR)\\" . $file . " ";
        $project{"WXCOMMONOBJS"} .= $obj;
        $project{"WXCOBJS"} .= $obj if $isCFile;
    }

    foreach $file (sort keys %wxMSW) {
        next if $wxMSW{$file} =~ /\b16\b/;

#!        if ( $file =~ /^automtn/ ) {
#!            #! comment in old makefile.b32 seems to imply that this file can not
#!            #! be compiled with Borland (leads to crash in oleauto sample)
#!            No longer true, at least for BC++ 5.2
#!            next;
#!        }

        $isCFile = $file =~ /\.c$/;

        my $isOleObj = $wxMSW{$file} =~ /\bO\b/;
        $file =~ s/cp?p?$/obj/;
        my $obj = "\$(MSWDIR)\\" . $file . " ";

        $project{"WXMSWOBJS"} .= $obj;
        if ( $isOleObj ) {
            #! remember that this file is in ole subdir
            $project{"WXOLEOBJS"} .= $obj;
        }
        $project{"WXCOBJS"} .= $obj if $isCFile;
    }
#$}

# This file was automatically generated by tmake 
# DO NOT CHANGE THIS FILE, YOUR CHANGES WILL BE LOST! CHANGE B32.T!

#
# File:     makefile.b32
# Author:   Julian Smart
# Created:  1998
# Updated:
# Copyright:
#
# "%W% %G%"
#
# Makefile : Builds wxWindows library wx.lib for MS Windows,
# and Borland C++ (32-bit).

# BCCDIR now defined in ../makeb32.env

!if "$(WXWIN)" == ""
!error You must define the WXWIN variable in autoexec.bat, e.g. WXWIN=c:\wx
!endif

WXDIR = $(WXWIN)
THISDIR = $(WXDIR)\src\msw

# Set all these to 1 if you want to build a dynamic library
!if "$(DLL)" == "1"
WXMAKINGDLL=1
WXBUILDDLL=1
!endif

!include $(WXDIR)\src\makeb32.env

# Please set these according to the settings in wx_setup.h, so we can include
# the appropriate libraries in wx.lib
USE_CTL3D=0

PERIPH_LIBS=
PERIPH_TARGET=
PERIPH_CLEAN_TARGET=

!if "$(USE_CTL3D)" == "1"
#Use WIN32S/WIN95 32 bit version ctl3d32.dll under win95 (Andre Beltman)
PERIPH_LIBS=$(WXDIR)\lib\ctl3d32.lib $(PERIPH_LIBS)
PERIPH_TARGET=ctl3d $(PERIPH_TARGET)
PERIPH_CLEAN_TARGET=clean_ctl3d $(PERIPH_CLEAN_TARGET)
!endif

#PERIPH_LIBS=$(WXDIR)\lib\zlib.lib $(WXDIR)\lib\winpng.lib $(WXDIR)\lib\jpeg.lib $(WXDIR)\lib\tiff.lib $(PERIPH_LIBS)
PERIPH_LIBS=
PERIPH_TARGET=zlib png jpeg tiff regex $(PERIPH_TARGET)
PERIPH_CLEAN_TARGET=clean_zlib clean_png clean_jpeg clean_tiff clean_regex $(PERIPH_CLEAN_TARGET)

!if "$(DLL)" == "0"
DUMMY=dummy
!else
DUMMY=dummydll
LIBS= cw32mti import32 ole2w32 odbc32 zlib winpng jpeg tiff regex
!endif

LIBTARGET=$(WXLIB)

GENDIR=..\generic
COMMDIR=..\common
HTMLDIR=..\html
OLEDIR=.\ole
MSWDIR=.

DOCDIR = $(WXDIR)\docs

GENERICOBJS= #$ ExpandList("WXGENERICOBJS");

# Not needed:
#  $(MSWDIR)\colrdlgg.obj \
#  $(MSWDIR)\fontdlgg.obj \
#  $(MSWDIR)\msgdlgg.obj \
#  $(MSWDIR)\printps.obj \
#  $(MSWDIR)\prntdlgg.obj \
#  $(MSWDIR)\listctrl.obj \
#  $(MSWDIR)\notebook.obj \
#  $(MSWDIR)\treectrl.obj

COMMONOBJS = \
		$(MSWDIR)\y_tab.obj \
		#$ ExpandList("WXCOMMONOBJS");

MSWOBJS = #$ ExpandList("WXMSWOBJS");

HTMLOBJS = #$ ExpandList("WXHTMLOBJS");

OBJECTS = $(COMMONOBJS) $(GENERICOBJS) $(MSWOBJS) $(HTMLOBJS)

default:	wx

wx:    $(ARCHINCDIR)\wx makesetuph makearchsetuph $(CFG) $(DUMMY).obj $(OBJECTS) $(PERIPH_TARGET) $(LIBTARGET)

all:	wx

# Copy the in-CVS setup0.h to setup.h if necessary
makesetuph:
     cd $(WXDIR)\include\wx\msw
     if not exist setup.h copy setup0.h setup.h
     cd $(WXDIR)\src\msw

# Copy include\wx\msw\setup.h to the architecture-specific location
makearchsetuph:
     copy $(WXDIR)\include\wx\msw\setup.h $(ARCHSETUPH)
     cd $(WXDIR)\src\msw

$(ARCHINCDIR)\wx:
    -mkdir $(ARCHINCDIR)
    -mkdir $(ARCHINCDIR)\wx

!if "$(DLL)" == "0"

$(LIBTARGET): $(DUMMY).obj $(OBJECTS)
        -erase $(LIBTARGET)
	tlib "$(LIBTARGET)" /P1024 @&&!
+$(OBJECTS:.obj =.obj +) +$(PERIPH_LIBS:.lib =.lib +)
!

!else

$(LIBTARGET): $(DUMMY).obj $(OBJECTS)
	-erase $(LIBTARGET)
	-erase $(WXLIBDIR)\wx.dll
        $(LINK) $(LINK_FLAGS) /L$(WXLIBDIR);$(BCCDIR)\lib;$(BCCDIR)\lib\psdk /v @&&!
c0d32.obj $(OBJECTS)
$(WXLIBDIR)\wx
nul
$(PERIPH_LIBS) $(LIBS)
wxb32
!
        implib -c $(LIBTARGET) $(WXLIBDIR)\wx.dll
!endif

dummy.obj: dummy.$(SRCSUFF) $(LOCALHEADERS) $(BASEHEADERS) $(WXDIR)\include\wx\wx.h
dummydll.obj: dummydll.$(SRCSUFF) $(LOCALHEADERS) $(BASEHEADERS) $(WXDIR)\include\wx\wx.h version.res

version.res: 
	brc32 -r -i$(WXDIR)\include\ $(MSWDIR)\version.rc

$(MSWDIR)\y_tab.obj:     $(COMMDIR)\y_tab.c $(COMMDIR)\lex_yy.c

#        cl @<<
# $(CPPFLAGS2) /c $*.c -DUSE_DEFINE -DYY_USE_PROTOS /Fo$@
# <<

$(COMMDIR)\y_tab.c:     $(COMMDIR)\dosyacc.c
        copy $(COMMDIR)\dosyacc.c $(COMMDIR)\y_tab.c

$(COMMDIR)\lex_yy.c:    $(COMMDIR)\doslex.c
    copy $(COMMDIR)\doslex.c $(COMMDIR)\lex_yy.c

# $(OBJECTS):	$(WXDIR)\include\wx\setup.h

#${
    $_ = $project{"WXMSWOBJS"};
    my @objs = split;
    foreach (@objs) {
        $text .= $_ . ": ";
        if ( $project{"WXOLEOBJS"} =~ /\Q$_/ ) { s/MSWDIR/OLEDIR/; }
        $suffix = $project{"WXCOBJS"} =~ /\Q$_/ ? "c" : '$(SRCSUFF)';
        s/obj$/$suffix/;
        $text .= $_ . "\n\n";
    }
#$}

########################################################
# Common objects (always compiled)

#${
    $_ = $project{"WXCOMMONOBJS"};
    my @objs = split;
    foreach (@objs) {
        $text .= $_ . ": ";
        $suffix = $project{"WXCOBJS"} =~ /\Q$_/ ? "c" : '$(SRCSUFF)';
        s/MSWDIR/COMMDIR/;
        s/obj$/$suffix/;
        $text .= $_ . "\n\n";
    }
#$}

########################################################
# Generic objects (not always compiled, depending on
# whether platforms have native implementations)

#${
    $_ = $project{"WXGENERICOBJS"};
    my @objs = split;
    foreach (@objs) {
        $text .= $_ . ": ";
        s/MSWDIR/GENDIR/;
        s/obj$/\$(SRCSUFF)/;
        $text .= $_ . "\n\n";
    }
#$}

########################################################
# HTML objects (always compiled)

#${
    $_ = $project{"WXHTMLOBJS"};
    my @objs = split;
    foreach (@objs) {
        $text .= $_ . ": ";
        s/MSWDIR/HTMLDIR/;
        s/obj$/\$(SRCSUFF)/;
        $text .= $_ . "\n\n";
    }
#$}


all_utils:
    cd $(WXDIR)\utils
    make -f makefile.b32
    cd $(WXDIR)\src\msw

all_samples:
    cd $(WXDIR)\samples
    make -f makefile.b32
    cd $(WXDIR)\src\msw

all_execs:
    cd $(WXDIR)\utils
    make -f makefile.b32 all_execs
    cd $(WXDIR)\src\msw

png:    $(CFG)
        cd $(WXDIR)\src\png
        make -f makefile.b32
        cd $(WXDIR)\src\msw

clean_png:
        cd $(WXDIR)\src\png
        make -f makefile.b32 clean
        cd $(WXDIR)\src\msw

zlib:   $(CFG)
        cd $(WXDIR)\src\zlib
        make -f makefile.b32 lib
        cd $(WXDIR)\src\msw

clean_zlib:
        cd $(WXDIR)\src\zlib
        make -f makefile.b32 clean
        cd $(WXDIR)\src\msw

jpeg:    $(CFG)
        cd $(WXDIR)\src\jpeg
        make -f makefile.b32
        cd $(WXDIR)\src\msw

clean_jpeg:
        cd $(WXDIR)\src\jpeg
        make -f makefile.b32 clean
        cd $(WXDIR)\src\msw

regex:   $(CFG)
        cd $(WXDIR)\src\regex
        make -f makefile.b32 lib
        cd $(WXDIR)\src\msw

clean_regex:
        cd $(WXDIR)\src\regex
        make -f makefile.b32 clean
        cd $(WXDIR)\src\msw

tiff:   $(CFG)
        cd $(WXDIR)\src\tiff
        make -f makefile.b32 lib
        cd $(WXDIR)\src\msw

clean_tiff:
        cd $(WXDIR)\src\tiff
        make -f makefile.b32 clean
        cd $(WXDIR)\src\msw

$(CFG): makefile.b32
	copy &&!
-Hc
-H=$(WXDIR)\src\msw\wx32.csm
-3
-d
-a1 # byte alignment
-R-
-X
-w-par
-w-aus
-w-hid # virtual function A hides virtual function B
-tWM

-I$(ARCHINCDIR);$(WXINC);$(BCCDIR)\include;$(WXDIR)/src/generic;$(WXDIR)/src/png;$(WXDIR)/src/jpeg;$(WXDIR)/src/zlib;$(WXDIR)/src/tiff
-I$(WXDIR)\include\wx\msw\gnuwin32

-L$(BCCDIR)\lib;$(BCCDIR)\lib\psdk
-D__WXWIN__
-D__WXMSW__
-D__WINDOWS__
-DWIN32
$(OPT)
$(DEBUG_FLAGS)
$(WIN95FLAG)
! $(CFG)

clean: $(PERIPH_CLEAN_TARGET)
    -erase $(LIBTARGET)
    -erase $(WXLIBDIR)\wx.dll
    -erase $(WXLIBDIR)\wx.tds
    -erase $(WXLIBDIR)\wx.il?
    -erase *.obj
    -erase *.pch
    -erase *.csm
    -erase *.cfg
    -erase "wx32.#??"
    -erase ..\common\y_tab.c
    -erase ..\common\lex_yy.c

cleanall: clean


# Making documents
docs:   allhlp allhtml allpdfrtf
alldocs: docs
hlp:    wxhlp portinghlp
wxhlp:  $(DOCDIR)/winhelp/wx.hlp
prophlp: $(DOCDIR)/winhelp/prop.hlp
refhlp: $(DOCDIR)/winhelp/techref.hlp
rtf:    $(DOCDIR)/winhelp/wx.rtf
proprtf: $(DOCDIR)/winhelp/prop.rtf
pdfrtf:    $(DOCDIR)/pdf/wx.rtf
proppdfrtf: $(DOCDIR)/pdf/prop.rtf
refpdfrtf: $(DOCDIR)/pdf/techref.rtf
html:	wxhtml portinghtml
wxhtml:	$(DOCDIR)\html\wx\wx.htm
htmlhelp: $(DOCDIR)\html\wx\wx.chm
prophtml: $(DOCDIR)\html\proplist\prop.htm
ps:     wxps referencps
wxps:	$(WXDIR)\docs\ps\wx.ps
propps:	$(WXDIR)\docs\ps\prop.ps
referencps:	$(WXDIR)\docs\ps\referenc.ps

portinghtml: $(DOCDIR)\html\porting\port.htm
portingrtf: $(DOCDIR)/winhelp/porting.rtf
portinghlp: $(DOCDIR)/winhelp/porting.hlp
portingpdfrtf: $(DOCDIR)/pdf/porting.rtf
portingps:	$(WXDIR)\docs\ps\porting.ps

allhlp: wxhlp portinghlp prophlp
        cd $(WXDIR)\utils\dialoged\src
        make -f makefile.b32 hlp
        cd $(THISDIR)

#        cd $(WXDIR)\utils\wxhelp\src
#        make -f makefile.b32 hlp
#        cd $(WXDIR)\utils\tex2rtf\src
#        make -f makefile.b32 hlp
#        cd $(WXDIR)\utils\wxgraph\src
#        make -f makefile.b32 hlp
#        cd $(WXDIR)\utils\wxchart\src
#        make -f makefile.b32 hlp
#        cd $(WXDIR)\utils\wxtree\src
#        make -f makefile.b32 hlp
#        cd $(WXDIR)\utils\wxbuild\src
#        make -f makefile.b32 hlp
#        cd $(WXDIR)\utils\wxgrid\src
#        make -f makefile.b32 hlp

allhtml: wxhtml portinghtml prophtml
        cd $(WXDIR)\utils\dialoged\src
        make -f makefile.b32 html
        cd $(THISDIR)

#        make -f makefile.b32 html
#        cd $(WXDIR)\utils\dialoged\src
#        make -f makefile.b32 html
#        cd $(WXDIR)\utils\hytext\src
#        make -f makefile.b32 html
#        cd $(WXDIR)\utils\wxhelp\src
#        make -f makefile.b32 html
#        cd $(WXDIR)\utils\tex2rtf\src
#        make -f makefile.b32 html
#        cd $(WXDIR)\utils\wxgraph\src
#        make -f makefile.b32 html
#        cd $(WXDIR)\utils\wxchart\src
#        make -f makefile.b32 html
#        cd $(WXDIR)\utils\wxtree\src
#        make -f makefile.b32 html

allps: wxps referencps portingps propps
        cd $(WXDIR)\utils\dialoged\src
        make -f makefile.b32 ps
        cd $(THISDIR)

allpdfrtf: pdfrtf portingpdfrtf proppdfrtf
        cd $(WXDIR)\utils\dialoged\src
        make -f makefile.b32 pdfrtf
        cd $(THISDIR)

#        cd $(WXDIR)\utils\wxhelp\src
#        make -f makefile.b32 ps
#        cd $(WXDIR)\utils\tex2rtf\src
#        make -f makefile.b32 ps
#        cd $(WXDIR)\utils\wxgraph\src
#        make -f makefile.b32 ps
#        cd $(WXDIR)\utils\wxchart\src
#        make -f makefile.b32 ps
#        cd $(WXDIR)\utils\wxtree\src
#        make -f makefile.b32 ps
#        cd $(THISDIR)

$(DOCDIR)/winhelp/wx.hlp:         $(DOCDIR)/latex/wx/wx.rtf $(DOCDIR)/latex/wx/wx.hpj
        cd $(DOCDIR)/latex/wx
        -erase wx.ph
        hc wx
        move wx.hlp $(DOCDIR)\winhelp\wx.hlp
        move wx.cnt $(DOCDIR)\winhelp\wx.cnt
        cd $(THISDIR)

$(DOCDIR)/winhelp/porting.hlp:         $(DOCDIR)/latex/porting/porting.rtf $(DOCDIR)/latex/porting/porting.hpj
        cd $(DOCDIR)/latex/porting
        -erase porting.ph
        hc porting
        move porting.hlp $(DOCDIR)\winhelp\porting.hlp
        move porting.cnt $(DOCDIR)\winhelp\porting.cnt
        cd $(THISDIR)

$(DOCDIR)/winhelp/prop.hlp:         $(DOCDIR)/latex/proplist/prop.rtf $(DOCDIR)/latex/proplist/prop.hpj
        cd $(DOCDIR)/latex/proplist
        -erase prop.ph
        hc prop
        move prop.hlp $(DOCDIR)\winhelp\prop.hlp
        move prop.cnt $(DOCDIR)\winhelp\prop.cnt
        cd $(THISDIR)

$(DOCDIR)/winhelp/techref.hlp:         $(DOCDIR)/latex/techref/techref.rtf $(DOCDIR)/latex/techref/techref.hpj
        cd $(DOCDIR)/latex/techref
        -erase techref.ph
        hc techref
        move techref.hlp $(DOCDIR)\winhelp\techref.hlp
        move techref.cnt $(DOCDIR)\winhelp\techref.cnt
        cd $(THISDIR)

$(DOCDIR)/latex/wx/wx.rtf:         $(DOCDIR)/latex/wx/classes.tex $(DOCDIR)/latex/wx/body.tex $(DOCDIR)/latex/wx/topics.tex $(DOCDIR)/latex/wx/manual.tex
        cd $(DOCDIR)\latex\wx
        -start $(WAITFLAG) tex2rtf $(DOCDIR)/latex/wx/manual.tex $(DOCDIR)/latex/wx/wx.rtf -twice -winhelp
        cd $(THISDIR)

$(DOCDIR)/latex/porting/porting.rtf:         $(DOCDIR)/latex/porting/porting.tex
        cd $(DOCDIR)\latex\porting
        -start $(WAITFLAG) tex2rtf $(DOCDIR)/latex/porting/porting.tex $(DOCDIR)/latex/porting/porting.rtf -twice -winhelp
        cd $(THISDIR)

$(DOCDIR)/latex/proplist/prop.rtf:         $(DOCDIR)/latex/proplist/prop.tex $(DOCDIR)/latex/proplist/body.tex $(DOCDIR)/latex/proplist/classes.tex $(DOCDIR)/latex/proplist/changes.tex
        cd $(DOCDIR)\latex\proplist
        -start $(WAITFLAG) tex2rtf $(DOCDIR)/latex/proplist/prop.tex $(DOCDIR)/latex/proplist/prop.rtf -twice -winhelp
        cd $(THISDIR)

$(DOCDIR)/latex/techref/techref.rtf:         $(DOCDIR)/latex/techref/techref.tex
        cd $(DOCDIR)\latex\techref
        -start $(WAITFLAG) tex2rtf $(DOCDIR)/latex/techref/techref.tex $(DOCDIR)/latex/techref/techref.rtf -twice -winhelp
        cd $(THISDIR)

$(DOCDIR)/pdf/wx.rtf:         $(DOCDIR)/latex/wx/classes.tex $(DOCDIR)/latex/wx/body.tex $(DOCDIR)/latex/wx/topics.tex $(DOCDIR)/latex/wx/manual.tex
        cd $(DOCDIR)\latex\wx
        -copy *.wmf $(DOCDIR)\pdf
        -copy *.bmp $(DOCDIR)\pdf
        -start $(WAITFLAG) tex2rtf $(DOCDIR)/latex/wx/manual.tex $(DOCDIR)/pdf/wx.rtf -twice -rtf
        cd $(THISDIR)

$(DOCDIR)/pdf/porting.rtf:         $(DOCDIR)/latex/porting/porting.tex
        cd $(DOCDIR)\latex\porting
        -copy *.wmf $(DOCDIR)\pdf
        -copy *.bmp $(DOCDIR)\pdf
        -start $(WAITFLAG) tex2rtf $(DOCDIR)/latex/porting/porting.tex $(DOCDIR)/pdf/porting.rtf -twice -rtf
        cd $(THISDIR)

$(DOCDIR)/pdf/prop.rtf:         $(DOCDIR)/latex/proplist/prop.tex $(DOCDIR)/latex/proplist/body.tex $(DOCDIR)/latex/proplist/classes.tex $(DOCDIR)/latex/proplist/changes.tex
        cd $(DOCDIR)\latex\proplist
        -copy *.wmf $(DOCDIR)\pdf
        -copy *.bmp $(DOCDIR)\pdf
        -start $(WAITFLAG) tex2rtf $(DOCDIR)/latex/proplist/prop.tex $(DOCDIR)/pdf/prop.rtf -twice -rtf
        cd $(THISDIR)

$(DOCDIR)/pdf/techref.rtf:         $(DOCDIR)/latex/techref/techref.tex
        cd $(DOCDIR)\latex\techref
        -copy *.wmf $(DOCDIR)\pdf
        -copy *.bmp $(DOCDIR)\pdf
        -start $(WAITFLAG) tex2rtf $(DOCDIR)/latex/techref/techref.tex $(DOCDIR)/pdf/techref.rtf -twice -rtf
        cd $(THISDIR)

$(DOCDIR)\html\wx\wx.htm:         $(DOCDIR)\latex\wx\classes.tex $(DOCDIR)\latex\wx\body.tex $(DOCDIR)/latex/wx/topics.tex $(DOCDIR)\latex\wx\manual.tex
        cd $(DOCDIR)\latex\wx
        -mkdir $(DOCDIR)\html\wx
        -start $(WAITFLAG) tex2rtf $(DOCDIR)\latex\wx\manual.tex $(DOCDIR)\html\wx\wx.htm -twice -html
        -erase $(DOCDIR)\html\wx\*.con
        -erase $(DOCDIR)\html\wx\*.ref
        -erase $(DOCDIR)\latex\wx\*.con
        -erase $(DOCDIR)\latex\wx\*.ref
         cd $(THISDIR)

$(DOCDIR)\html\wx\wx.chm : $(DOCDIR)\html\wx\wx.htm $(DOCDIR)\html\wx\wx.hhp
	cd $(DOCDIR)\html\wx
	-hhc wx.hhp
	cd $(THISDIR)


$(DOCDIR)\html\porting\port.htm:         $(DOCDIR)\latex\porting\porting.tex
        cd $(DOCDIR)\latex\porting
        -mkdir $(DOCDIR)\html\porting
        -start $(WAITFLAG) tex2rtf $(DOCDIR)\latex\porting\porting.tex $(DOCDIR)\html\porting\port.htm -twice -html
        -erase $(DOCDIR)\html\porting\*.con
        -erase $(DOCDIR)\html\porting\*.ref
        -erase $(DOCDIR)\latex\porting\*.con
        -erase $(DOCDIR)\latex\porting\*.ref
        cd $(THISDIR)

$(DOCDIR)\html\proplist\prop.htm:         $(DOCDIR)\latex\proplist\prop.tex $(DOCDIR)\latex\proplist\body.tex $(DOCDIR)\latex\proplist\classes.tex $(DOCDIR)\latex\proplist\changes.tex
        cd $(DOCDIR)\latex\proplist
        -mkdir $(DOCDIR)\html\proplist
        -start $(WAITFLAG) tex2rtf $(DOCDIR)\latex\proplist\prop.tex $(DOCDIR)\html\proplist\prop.htm -twice -html
        -erase $(DOCDIR)\html\proplist\*.con
        -erase $(DOCDIR)\html\proplist\*.ref
        -erase $(DOCDIR)\latex\proplist\*.con
        -erase $(DOCDIR)\latex\proplist\*.ref
        cd $(THISDIR)

$(WXDIR)\docs\latex\wx\manual.dvi:	$(DOCDIR)/latex/wx/body.tex $(DOCDIR)/latex/wx/manual.tex
	cd $(WXDIR)\docs\latex\wx
        -latex manual
        -latex manual
        -makeindx manual
        -bibtex manual
        -latex manual
        -latex manual
        cd $(THISDIR)

$(WXDIR)\docs\latex\porting\porting.dvi:	$(DOCDIR)/latex/porting/porting.tex
	cd $(WXDIR)\docs\latex\porting
        -latex porting
        -latex porting
        -makeindx porting
        -bibtex porting
        -latex porting
        -latex porting
        cd $(THISDIR)

$(WXDIR)\docs\ps\wx.ps:	$(WXDIR)\docs\latex\wx\manual.dvi
	cd $(WXDIR)\docs\latex\wx
        -dvips32 -o wx.ps manual
        move wx.ps $(WXDIR)\docs\ps\wx.ps
        cd $(THISDIR)

$(WXDIR)\docs\ps\porting.ps:	$(WXDIR)\docs\latex\porting\porting.dvi
	cd $(WXDIR)\docs\latex\porting
        -dvips32 -o porting.ps porting
        move porting.ps $(WXDIR)\docs\ps\porting.ps
        cd $(THISDIR)

$(WXDIR)\docs\latex\wx\referenc.dvi:	$(DOCDIR)/latex/wx/classes.tex $(DOCDIR)/latex/wx/topics.tex $(DOCDIR)/latex/wx/referenc.tex
	cd $(WXDIR)\docs\latex\wx
        -latex referenc
        -latex referenc
        -makeindx referenc
        -bibtex referenc
        -latex referenc
        -latex referenc
        cd $(THISDIR)

$(WXDIR)\docs\ps\referenc.ps:	$(WXDIR)\docs\latex\wx\referenc.dvi
	cd $(WXDIR)\docs\latex\wx
        -dvips32 -o referenc.ps referenc
        move referenc.ps $(WXDIR)\docs\ps\referenc.ps
        cd $(THISDIR)

# In order to force document reprocessing
touchmanual:
    -touch $(WXDIR)\docs\latex\wx\manual.tex

updatedocs: touchmanual alldocs

# Start Word, running the GeneratePDF macro. MakeManual.dot should be in the
# Office StartUp folder, and PDFMaker should be installed.
updatepdf:  # touchmanual pdfrtf
    start $(WAITFLAG) "winword d:\wx2\wxWindows\docs\latex\pdf\wx.rtf /mGeneratePDF"


MFTYPE=b32
# Can't use this or we'll have to distribute all tmake files with wxWindows
# makefile.$(MFTYPE) : $(WXWIN)\distrib\msw\tmake\filelist.txt $(WXWIN)\distrib\msw\tmake\$(MFTYPE).t

self:
	cd $(WXWIN)\distrib\msw\tmake
	tmake -t $(MFTYPE) wxwin.pro -o makefile.$(MFTYPE)
	copy makefile.$(MFTYPE) $(WXWIN)\src\msw

