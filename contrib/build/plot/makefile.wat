# =========================================================================
#     This makefile was generated by
#     Bakefile 0.1.1 (http://bakefile.sourceforge.net)
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

LIBDIRNAME =
!ifeq SHARED 0
LIBDIRNAME = ..\..\src\plot\..\..\..\lib\wat_lib$(CFG)
!endif
!ifeq SHARED 1
LIBDIRNAME = ..\..\src\plot\..\..\..\lib\wat_dll$(CFG)
!endif
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
WXDLLFLAG =
!ifeq SHARED 1
WXDLLFLAG = dll
!endif
WXUNICODEFLAG =
!ifeq UNICODE 1
WXUNICODEFLAG = u
!endif
WXUNIVNAME =
!ifeq WXUNIV 1
WXUNIVNAME = univ
!endif
__DEBUGINFO =
!ifeq BUILD debug
!ifeq DEBUG_INFO default
__DEBUGINFO = -d2
!endif
!endif
!ifeq BUILD release
!ifeq DEBUG_INFO default
__DEBUGINFO = -d0
!endif
!endif
!ifeq DEBUG_INFO 0
__DEBUGINFO = -d0
!endif
!ifeq DEBUG_INFO 1
__DEBUGINFO = -d2
!endif
__DEBUGINFO_1 =
!ifeq BUILD debug
!ifeq DEBUG_INFO default
__DEBUGINFO_1 = debug all
!endif
!endif
!ifeq BUILD release
!ifeq DEBUG_INFO default
__DEBUGINFO_1 = 
!endif
!endif
!ifeq DEBUG_INFO 0
__DEBUGINFO_1 = 
!endif
!ifeq DEBUG_INFO 1
__DEBUGINFO_1 = debug all
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
__LIB_JPEG_p =
!ifeq USE_GUI 1
__LIB_JPEG_p = wxjpeg$(WXDEBUGFLAG).lib
!endif
__LIB_PNG_p =
!ifeq USE_GUI 1
__LIB_PNG_p = wxpng$(WXDEBUGFLAG).lib
!endif
__LIB_TIFF_p =
!ifeq USE_GUI 1
__LIB_TIFF_p = wxtiff$(WXDEBUGFLAG).lib
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
__UNICODE_DEFINE_p =
!ifeq UNICODE 1
__UNICODE_DEFINE_p = -d_UNICODE
!endif
__WXLIB_BASE_p =
!ifeq MONOLITHIC 0
__WXLIB_BASE_p = wxbase25$(WXUNICODEFLAG)$(WXDEBUGFLAG).lib
!endif
__WXLIB_CORE_p =
!ifeq MONOLITHIC 0
__WXLIB_CORE_p = &
	wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_core.lib
!endif
__WXLIB_MONO_p =
!ifeq MONOLITHIC 1
__WXLIB_MONO_p = &
	wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG).lib
!endif
__WXUNIV_DEFINE_p =
!ifeq WXUNIV 1
__WXUNIV_DEFINE_p = -d__WXUNIVERSAL__
!endif
__plotdll___depname =
!ifeq SHARED 1
__plotdll___depname = &
	$(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)251$(WXUNICODEFLAG)$(WXDEBUGFLAG)_plot_wat$(VENDORTAG).dll
!endif
__plotlib___depname =
!ifeq SHARED 0
__plotlib___depname = &
	$(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_plot.lib
!endif

### Variables: ###

OBJS = &
	wat_$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WXDLLFLAG)$(CFG)
PLOTDLL_CXXFLAGS = $(CPPFLAGS) -bd $(__DEBUGINFO) $(__OPTIMIZEFLAG) -bm &
	$(__RUNTIME_LIBS) -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) &
	$(__UNICODE_DEFINE_p) -i=..\..\src\plot\..\..\..\include -i=$(SETUPHDIR) &
	-i=..\..\src\plot\..\..\include -dWXUSINGDLL -dWXMAKINGDLL_PLOT $(CXXFLAGS) &
	/fh=$(OBJS)\wxprec_plotdll.pch
PLOTDLL_OBJECTS =  &
	$(OBJS)\plotdll_dummy.obj &
	$(OBJS)\plotdll_plot.obj
PLOTLIB_CXXFLAGS = $(CPPFLAGS) $(__DEBUGINFO) $(__OPTIMIZEFLAG) -bm &
	$(__RUNTIME_LIBS) -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) &
	$(__UNICODE_DEFINE_p) -i=..\..\src\plot\..\..\..\include -i=$(SETUPHDIR) &
	-i=..\..\src\plot\..\..\include $(CXXFLAGS) /fh=$(OBJS)\wxprec_plotlib.pch
PLOTLIB_OBJECTS =  &
	$(OBJS)\plotlib_dummy.obj &
	$(OBJS)\plotlib_plot.obj
SETUPHDIR = &
	$(LIBDIRNAME)\$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)


all : $(OBJS)
$(OBJS) :
	-if not exist $(OBJS) mkdir $(OBJS)

### Targets: ###

all : .SYMBOLIC $(__plotdll___depname) $(__plotlib___depname)

$(OBJS)\plotdll_dummy.obj :  .AUTODEPEND ../../src/plot\..\..\..\src\msw\dummy.cpp
	$(CXX) -zq -fo=$^@ $(PLOTDLL_CXXFLAGS) $<

$(OBJS)\plotdll_plot.obj :  .AUTODEPEND ../../src/plot\plot.cpp
	$(CXX) -zq -fo=$^@ $(PLOTDLL_CXXFLAGS) $<

$(OBJS)\plotlib_dummy.obj :  .AUTODEPEND ../../src/plot\..\..\..\src\msw\dummy.cpp
	$(CXX) -zq -fo=$^@ $(PLOTLIB_CXXFLAGS) $<

$(OBJS)\plotlib_plot.obj :  .AUTODEPEND ../../src/plot\plot.cpp
	$(CXX) -zq -fo=$^@ $(PLOTLIB_CXXFLAGS) $<

clean : .SYMBOLIC 
	-if exist $(OBJS)\*.obj del $(OBJS)\*.obj
	-if exist $(OBJS)\*.res del $(OBJS)\*.res
	-if exist $(OBJS)\*.lbc del $(OBJS)\*.lbc
	-if exist $(OBJS)\*.ilk del $(OBJS)\*.ilk
	-if exist $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)251$(WXUNICODEFLAG)$(WXDEBUGFLAG)_plot_wat$(VENDORTAG).dll del $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)251$(WXUNICODEFLAG)$(WXDEBUGFLAG)_plot_wat$(VENDORTAG).dll
	-if exist $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_plot.lib del $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_plot.lib
	-if exist $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_plot.lib del $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_plot.lib

!ifeq SHARED 1
$(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)251$(WXUNICODEFLAG)$(WXDEBUGFLAG)_plot_wat$(VENDORTAG).dll :  $(PLOTDLL_OBJECTS)
	@%create $(OBJS)\plotdll.lbc
	@%append $(OBJS)\plotdll.lbc option quiet
	@%append $(OBJS)\plotdll.lbc name $^@
	@%append $(OBJS)\plotdll.lbc option caseexact
	@%append $(OBJS)\plotdll.lbc $(LDFLAGS) $(__DEBUGINFO_1)  libpath $(LIBDIRNAME)
	@for %i in ($(PLOTDLL_OBJECTS)) do @%append $(OBJS)\plotdll.lbc file %i
	@for %i in ( $(__WXLIB_MONO_p) $(__LIB_TIFF_p) $(__LIB_JPEG_p) $(__LIB_PNG_p) wxzlib$(WXDEBUGFLAG).lib  wxregex$(WXDEBUGFLAG).lib wxexpat$(WXDEBUGFLAG).lib   kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib  $(__WXLIB_CORE_p)  $(__WXLIB_BASE_p) ) do @%append $(OBJS)\plotdll.lbc library %i
	@%append $(OBJS)\plotdll.lbc
	@%append $(OBJS)\plotdll.lbc system nt_dll
	wlink @$(OBJS)\plotdll.lbc
	wlib -q -n -b $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_plot.lib +$^@
!endif

!ifeq SHARED 0
$(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_plot.lib :  $(PLOTLIB_OBJECTS)
	@%create $(OBJS)\plotlib.lbc
	@for %i in ($(PLOTLIB_OBJECTS)) do @%append $(OBJS)\plotlib.lbc +%i
	wlib -q -p2048 -n -b $^@ @$(OBJS)\plotlib.lbc
!endif
