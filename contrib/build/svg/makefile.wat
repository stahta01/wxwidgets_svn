# =========================================================================
#     This makefile was generated by
#     Bakefile 0.1.4 (http://bakefile.sourceforge.net)
#     Do not modify, all changes will be overwritten!
# =========================================================================

!include ../../../build/msw/config.wat

# -------------------------------------------------------------------------
# Do not modify the rest of this file!
# -------------------------------------------------------------------------

# Speed up compilation a bit:
!ifdef __LOADDLL__
!  loaddll wcc      wccd
!  loaddll wccaxp   wccdaxp
!  loaddll wcc386   wccd386
!  loaddll wpp      wppdi86
!  loaddll wppaxp   wppdaxp
!  loaddll wpp386   wppd386
!  loaddll wlink    wlink
!  loaddll wlib     wlibd
!endif

# We need these variables in some bakefile-made rules:
WATCOM_CWD = $+ $(%cdrive):$(%cwd) $-

### Conditionally set variables: ###

PORTNAME =
!ifeq USE_GUI 0
PORTNAME = base
!endif
!ifeq USE_GUI 1
PORTNAME = msw
!endif
VENDORTAG =
!ifeq OFFICIAL_BUILD 0
VENDORTAG = _$(VENDOR)
!endif
!ifeq OFFICIAL_BUILD 1
VENDORTAG = 
!endif
WXDEBUGFLAG =
!ifeq BUILD debug
!ifeq DEBUG_FLAG default
WXDEBUGFLAG = d
!endif
!endif
!ifeq DEBUG_FLAG 1
WXDEBUGFLAG = d
!endif
WXUNICODEFLAG =
!ifeq UNICODE 1
WXUNICODEFLAG = u
!endif
WXUNIVNAME =
!ifeq WXUNIV 1
WXUNIVNAME = univ
!endif
WXDLLFLAG =
!ifeq SHARED 1
WXDLLFLAG = dll
!endif
__svgdll___depname =
!ifeq SHARED 1
__svgdll___depname = &
	$(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)252$(WXUNICODEFLAG)$(WXDEBUGFLAG)_svg_wat$(VENDORTAG).dll
!endif
__DEBUGINFO_2 =
!ifeq BUILD debug
!ifeq DEBUG_INFO default
__DEBUGINFO_2 = debug all
!endif
!endif
!ifeq BUILD release
!ifeq DEBUG_INFO default
__DEBUGINFO_2 = 
!endif
!endif
!ifeq DEBUG_INFO 0
__DEBUGINFO_2 = 
!endif
!ifeq DEBUG_INFO 1
__DEBUGINFO_2 = debug all
!endif
__WXLIB_MONO_p =
!ifeq MONOLITHIC 1
__WXLIB_MONO_p = &
	wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG).lib
!endif
__LIB_TIFF_p =
!ifeq USE_GUI 1
__LIB_TIFF_p = wxtiff$(WXDEBUGFLAG).lib
!endif
__LIB_JPEG_p =
!ifeq USE_GUI 1
__LIB_JPEG_p = wxjpeg$(WXDEBUGFLAG).lib
!endif
__LIB_PNG_p =
!ifeq USE_GUI 1
__LIB_PNG_p = wxpng$(WXDEBUGFLAG).lib
!endif
__WXLIB_CORE_p =
!ifeq MONOLITHIC 0
__WXLIB_CORE_p = &
	wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_core.lib
!endif
__WXLIB_BASE_p =
!ifeq MONOLITHIC 0
__WXLIB_BASE_p = wxbase25$(WXUNICODEFLAG)$(WXDEBUGFLAG).lib
!endif
__svglib___depname =
!ifeq SHARED 0
__svglib___depname = &
	$(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_svg.lib
!endif
__DEBUGINFO_1 =
!ifeq BUILD debug
!ifeq DEBUG_INFO default
__DEBUGINFO_1 = -d2
!endif
!endif
!ifeq BUILD release
!ifeq DEBUG_INFO default
__DEBUGINFO_1 = -d0
!endif
!endif
!ifeq DEBUG_INFO 0
__DEBUGINFO_1 = -d0
!endif
!ifeq DEBUG_INFO 1
__DEBUGINFO_1 = -d2
!endif
__OPTIMIZEFLAG =
!ifeq BUILD debug
__OPTIMIZEFLAG = -od
!endif
!ifeq BUILD release
__OPTIMIZEFLAG = -ot -ox
!endif
__RUNTIME_LIBS =
!ifeq RUNTIME_LIBS dynamic
__RUNTIME_LIBS = -br
!endif
!ifeq RUNTIME_LIBS static
__RUNTIME_LIBS = 
!endif
__EXCEPTIONSFLAG =
!ifeq USE_EXCEPTIONS 0
__EXCEPTIONSFLAG = 
!endif
!ifeq USE_EXCEPTIONS 1
__EXCEPTIONSFLAG = -xs
!endif
__WXUNIV_DEFINE_p =
!ifeq WXUNIV 1
__WXUNIV_DEFINE_p = -d__WXUNIVERSAL__
!endif
__DEBUG_DEFINE_p =
!ifeq BUILD debug
!ifeq DEBUG_FLAG default
__DEBUG_DEFINE_p = -d__WXDEBUG__
!endif
!endif
!ifeq DEBUG_FLAG 1
__DEBUG_DEFINE_p = -d__WXDEBUG__
!endif
__UNICODE_DEFINE_p =
!ifeq UNICODE 1
__UNICODE_DEFINE_p = -d_UNICODE
!endif
LIBDIRNAME =
!ifeq SHARED 0
LIBDIRNAME = ..\..\src\svg\..\..\..\lib\wat_lib$(CFG)
!endif
!ifeq SHARED 1
LIBDIRNAME = ..\..\src\svg\..\..\..\lib\wat_dll$(CFG)
!endif

### Variables: ###

OBJS = &
	wat_$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WXDLLFLAG)$(CFG)
SETUPHDIR = &
	$(LIBDIRNAME)\$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)
SVGDLL_CXXFLAGS = -bd $(__DEBUGINFO_1) $(__OPTIMIZEFLAG) -bm $(__RUNTIME_LIBS) &
	-d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) $(__UNICODE_DEFINE_p) &
	-i=..\..\src\svg\..\..\..\include -i=$(SETUPHDIR) &
	-i=..\..\src\svg\..\..\include -dWXUSINGDLL -dWXMAKINGDLL_SVG &
	/fh=$(OBJS)\wxprec_svgdll.pch $(__EXCEPTIONSFLAG) $(CPPFLAGS) $(CXXFLAGS)
SVGDLL_OBJECTS =  &
	$(OBJS)\svgdll_dummy.obj &
	$(OBJS)\svgdll_dcsvg.obj
SVGLIB_CXXFLAGS = $(__DEBUGINFO_1) $(__OPTIMIZEFLAG) -bm $(__RUNTIME_LIBS) &
	-d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) $(__UNICODE_DEFINE_p) &
	-i=..\..\src\svg\..\..\..\include -i=$(SETUPHDIR) &
	-i=..\..\src\svg\..\..\include /fh=$(OBJS)\wxprec_svglib.pch &
	$(__EXCEPTIONSFLAG) $(CPPFLAGS) $(CXXFLAGS)
SVGLIB_OBJECTS =  &
	$(OBJS)\svglib_dummy.obj &
	$(OBJS)\svglib_dcsvg.obj


all : $(OBJS)
$(OBJS) :
	-if not exist $(OBJS) mkdir $(OBJS)

### Targets: ###

all : .SYMBOLIC $(__svgdll___depname) $(__svglib___depname)

clean : .SYMBOLIC 
	-if exist $(OBJS)\*.obj del $(OBJS)\*.obj
	-if exist $(OBJS)\*.res del $(OBJS)\*.res
	-if exist $(OBJS)\*.lbc del $(OBJS)\*.lbc
	-if exist $(OBJS)\*.ilk del $(OBJS)\*.ilk
	-if exist $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)252$(WXUNICODEFLAG)$(WXDEBUGFLAG)_svg_wat$(VENDORTAG).dll del $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)252$(WXUNICODEFLAG)$(WXDEBUGFLAG)_svg_wat$(VENDORTAG).dll
	-if exist $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_svg.lib del $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_svg.lib
	-if exist $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_svg.lib del $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_svg.lib

!ifeq SHARED 1
$(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)252$(WXUNICODEFLAG)$(WXDEBUGFLAG)_svg_wat$(VENDORTAG).dll :  $(SVGDLL_OBJECTS)
	@%create $(OBJS)\svgdll.lbc
	@%append $(OBJS)\svgdll.lbc option quiet
	@%append $(OBJS)\svgdll.lbc name $^@
	@%append $(OBJS)\svgdll.lbc option caseexact
	@%append $(OBJS)\svgdll.lbc $(LDFLAGS) $(__DEBUGINFO_2)  libpath $(LIBDIRNAME)
	@for %i in ($(SVGDLL_OBJECTS)) do @%append $(OBJS)\svgdll.lbc file %i
	@for %i in ( $(__WXLIB_MONO_p) $(__LIB_TIFF_p) $(__LIB_JPEG_p) $(__LIB_PNG_p) wxzlib$(WXDEBUGFLAG).lib  wxregex$(WXUNICODEFLAG)$(WXDEBUGFLAG).lib wxexpat$(WXDEBUGFLAG).lib   kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib  $(__WXLIB_CORE_p)  $(__WXLIB_BASE_p) ) do @%append $(OBJS)\svgdll.lbc library %i
	@%append $(OBJS)\svgdll.lbc
	@%append $(OBJS)\svgdll.lbc system nt_dll
	wlink @$(OBJS)\svgdll.lbc
	wlib -q -n -b $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_svg.lib +$^@
!endif

!ifeq SHARED 0
$(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_svg.lib :  $(SVGLIB_OBJECTS)
	@%create $(OBJS)\svglib.lbc
	@for %i in ($(SVGLIB_OBJECTS)) do @%append $(OBJS)\svglib.lbc +%i
	wlib -q -p4096 -n -b $^@ @$(OBJS)\svglib.lbc
!endif

$(OBJS)\svgdll_dummy.obj :  .AUTODEPEND ../../src/svg\..\..\..\src\msw\dummy.cpp
	$(CXX) -zq -fo=$^@ $(SVGDLL_CXXFLAGS) $<

$(OBJS)\svgdll_dcsvg.obj :  .AUTODEPEND ../../src/svg\dcsvg.cpp
	$(CXX) -zq -fo=$^@ $(SVGDLL_CXXFLAGS) $<

$(OBJS)\svglib_dummy.obj :  .AUTODEPEND ../../src/svg\..\..\..\src\msw\dummy.cpp
	$(CXX) -zq -fo=$^@ $(SVGLIB_CXXFLAGS) $<

$(OBJS)\svglib_dcsvg.obj :  .AUTODEPEND ../../src/svg\dcsvg.cpp
	$(CXX) -zq -fo=$^@ $(SVGLIB_CXXFLAGS) $<
