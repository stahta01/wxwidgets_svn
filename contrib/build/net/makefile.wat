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
__UNICODE_DEFINE_p = -dwxUSE_UNICODE=1
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
__netutilsdll___depname =
!ifeq SHARED 1
__netutilsdll___depname = &
	$(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)250$(WXUNICODEFLAG)$(WXDEBUGFLAG)_netutils_wat$(VENDORTAG).dll
!endif
__netutilslib___depname =
!ifeq SHARED 0
__netutilslib___depname = &
	$(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_netutils.lib
!endif

### Variables: ###

LIBDIRNAME = &
	..\..\src\net\..\..\..\lib\wat_$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WXDLLFLAG)$(CFG)
NETUTILSDLL_CXXFLAGS = $(CPPFLAGS) -bd $(__DEBUGINFO) $(__OPTIMIZEFLAG) -bm &
	$(__RUNTIME_LIBS) -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) &
	$(__UNICODE_DEFINE_p) -i=..\..\src\net\..\..\..\include -i=$(LIBDIRNAME) &
	-i=..\..\src\net\..\..\..\src\tiff -i=..\..\src\net\..\..\..\src\jpeg &
	-i=..\..\src\net\..\..\..\src\png -i=..\..\src\net\..\..\..\src\zlib &
	-i=..\..\src\net\..\..\..\src\regex &
	-i=..\..\src\net\..\..\..\src\expat\lib -i=..\..\src\net\..\..\include &
	-dWXUSINGDLL -dWXMAKINGDLL_NETUTILS $(CXXFLAGS) &
	/fh=$(OBJS)\wxprec_netutilsdll.pch
NETUTILSDLL_OBJECTS =  &
	$(OBJS)\netutilsdll_dummy.obj &
	$(OBJS)\netutilsdll_email.obj &
	$(OBJS)\netutilsdll_smapi.obj &
	$(OBJS)\netutilsdll_web.obj
NETUTILSLIB_CXXFLAGS = $(CPPFLAGS) $(__DEBUGINFO) $(__OPTIMIZEFLAG) -bm &
	$(__RUNTIME_LIBS) -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) &
	$(__UNICODE_DEFINE_p) -i=..\..\src\net\..\..\..\include -i=$(LIBDIRNAME) &
	-i=..\..\src\net\..\..\..\src\tiff -i=..\..\src\net\..\..\..\src\jpeg &
	-i=..\..\src\net\..\..\..\src\png -i=..\..\src\net\..\..\..\src\zlib &
	-i=..\..\src\net\..\..\..\src\regex &
	-i=..\..\src\net\..\..\..\src\expat\lib -i=..\..\src\net\..\..\include &
	$(CXXFLAGS) /fh=$(OBJS)\wxprec_netutilslib.pch
NETUTILSLIB_OBJECTS =  &
	$(OBJS)\netutilslib_dummy.obj &
	$(OBJS)\netutilslib_email.obj &
	$(OBJS)\netutilslib_smapi.obj &
	$(OBJS)\netutilslib_web.obj
OBJS = &
	wat_$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WXDLLFLAG)$(CFG)

MAKEARGS = BUILD=$(BUILD) CFG=$(CFG) CPPFLAGS=$(CPPFLAGS) CXX=$(CXX) CXXFLAGS=$(CXXFLAGS) DEBUG_FLAG=$(DEBUG_FLAG) DEBUG_INFO=$(DEBUG_INFO) LDFLAGS=$(LDFLAGS) MONOLITHIC=$(MONOLITHIC) OFFICIAL_BUILD=$(OFFICIAL_BUILD) RUNTIME_LIBS=$(RUNTIME_LIBS) SHARED=$(SHARED) UNICODE=$(UNICODE) USE_GUI=$(USE_GUI) VENDOR=$(VENDOR) WXUNIV=$(WXUNIV)


all : $(OBJS)
$(OBJS) :
	-if not exist $(OBJS) mkdir $(OBJS)

### Targets: ###

all : .SYMBOLIC $(__netutilsdll___depname) $(__netutilslib___depname)

$(OBJS)\netutilsdll_dummy.obj :  .AUTODEPEND ../../src/net\..\..\..\src\msw\dummy.cpp
	$(CXX) -zq -fo=$^@ $(NETUTILSDLL_CXXFLAGS) $<

$(OBJS)\netutilsdll_email.obj :  .AUTODEPEND ../../src/net\email.cpp
	$(CXX) -zq -fo=$^@ $(NETUTILSDLL_CXXFLAGS) $<

$(OBJS)\netutilsdll_smapi.obj :  .AUTODEPEND ../../src/net\smapi.cpp
	$(CXX) -zq -fo=$^@ $(NETUTILSDLL_CXXFLAGS) $<

$(OBJS)\netutilsdll_web.obj :  .AUTODEPEND ../../src/net\web.cpp
	$(CXX) -zq -fo=$^@ $(NETUTILSDLL_CXXFLAGS) $<

$(OBJS)\netutilslib_dummy.obj :  .AUTODEPEND ../../src/net\..\..\..\src\msw\dummy.cpp
	$(CXX) -zq -fo=$^@ $(NETUTILSLIB_CXXFLAGS) $<

$(OBJS)\netutilslib_email.obj :  .AUTODEPEND ../../src/net\email.cpp
	$(CXX) -zq -fo=$^@ $(NETUTILSLIB_CXXFLAGS) $<

$(OBJS)\netutilslib_smapi.obj :  .AUTODEPEND ../../src/net\smapi.cpp
	$(CXX) -zq -fo=$^@ $(NETUTILSLIB_CXXFLAGS) $<

$(OBJS)\netutilslib_web.obj :  .AUTODEPEND ../../src/net\web.cpp
	$(CXX) -zq -fo=$^@ $(NETUTILSLIB_CXXFLAGS) $<

clean : .SYMBOLIC 
	-if exist $(OBJS)\*.obj del $(OBJS)\*.obj
	-if exist $(OBJS)\*.res del $(OBJS)\*.res
	-if exist $(OBJS)\*.lbc del $(OBJS)\*.lbc
	-if exist $(OBJS)\*.ilk del $(OBJS)\*.ilk
	-if exist $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)250$(WXUNICODEFLAG)$(WXDEBUGFLAG)_netutils_wat$(VENDORTAG).dll del $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)250$(WXUNICODEFLAG)$(WXDEBUGFLAG)_netutils_wat$(VENDORTAG).dll
	-if exist $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_netutils.lib del $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_netutils.lib
	-if exist $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_netutils.lib del $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_netutils.lib

!ifeq SHARED 1
$(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)250$(WXUNICODEFLAG)$(WXDEBUGFLAG)_netutils_wat$(VENDORTAG).dll :  $(NETUTILSDLL_OBJECTS)
	@%create $(OBJS)\netutilsdll.lbc
	@%append $(OBJS)\netutilsdll.lbc option quiet
	@%append $(OBJS)\netutilsdll.lbc name $^@
	@%append $(OBJS)\netutilsdll.lbc option incremental
	@%append $(OBJS)\netutilsdll.lbc $(LDFLAGS) $(__DEBUGINFO_1)  libpath $(LIBDIRNAME)
	@for %i in ($(NETUTILSDLL_OBJECTS)) do @%append $(OBJS)\netutilsdll.lbc file %i
	@for %i in ( $(__WXLIB_MONO_p) $(__LIB_TIFF_p) $(__LIB_JPEG_p) $(__LIB_PNG_p) wxzlib$(WXDEBUGFLAG).lib  wxregex$(WXDEBUGFLAG).lib wxexpat$(WXDEBUGFLAG).lib  kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib  $(__WXLIB_CORE_p) $(__WXLIB_BASE_p)) do @%append $(OBJS)\netutilsdll.lbc library %i
	@%append $(OBJS)\netutilsdll.lbc
	@%append $(OBJS)\netutilsdll.lbc system nt_dll
	wlink @$(OBJS)\netutilsdll.lbc
	wlib -q -n -b $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_netutils.lib +$^@
!endif

!ifeq SHARED 0
$(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_netutils.lib :  $(NETUTILSLIB_OBJECTS)
	@%create $(OBJS)\netutilslib.lbc
	@for %i in ($(NETUTILSLIB_OBJECTS)) do @%append $(OBJS)\netutilslib.lbc +%i
	wlib -q -p2048 -n -b $^@ @$(OBJS)\netutilslib.lbc
!endif
