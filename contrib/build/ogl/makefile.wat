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
LIBDIRNAME = ..\..\src\ogl\..\..\..\lib\wat_lib$(CFG)
!endif
!ifeq SHARED 1
LIBDIRNAME = ..\..\src\ogl\..\..\..\lib\wat_dll$(CFG)
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
__ogldll___depname =
!ifeq SHARED 1
__ogldll___depname = &
	$(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)250$(WXUNICODEFLAG)$(WXDEBUGFLAG)_ogl_wat$(VENDORTAG).dll
!endif
__ogllib___depname =
!ifeq SHARED 0
__ogllib___depname = &
	$(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_ogl.lib
!endif

### Variables: ###

OBJS = &
	wat_$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WXDLLFLAG)$(CFG)
OGLDLL_CXXFLAGS = $(CPPFLAGS) -bd $(__DEBUGINFO) $(__OPTIMIZEFLAG) -bm &
	$(__RUNTIME_LIBS) -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) &
	$(__UNICODE_DEFINE_p) -i=..\..\src\ogl\..\..\..\include -i=$(SETUPHDIR) &
	-i=..\..\src\ogl\..\..\include -dWXUSINGDLL -dWXMAKINGDLL_OGL $(CXXFLAGS) &
	/fh=$(OBJS)\wxprec_ogldll.pch
OGLDLL_OBJECTS =  &
	$(OBJS)\ogldll_dummy.obj &
	$(OBJS)\ogldll_basic2.obj &
	$(OBJS)\ogldll_canvas.obj &
	$(OBJS)\ogldll_divided.obj &
	$(OBJS)\ogldll_mfutils.obj &
	$(OBJS)\ogldll_oglmisc.obj &
	$(OBJS)\ogldll_basic.obj &
	$(OBJS)\ogldll_composit.obj &
	$(OBJS)\ogldll_drawn.obj &
	$(OBJS)\ogldll_bmpshape.obj &
	$(OBJS)\ogldll_constrnt.obj &
	$(OBJS)\ogldll_lines.obj &
	$(OBJS)\ogldll_ogldiag.obj
OGLLIB_CXXFLAGS = $(CPPFLAGS) $(__DEBUGINFO) $(__OPTIMIZEFLAG) -bm &
	$(__RUNTIME_LIBS) -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) &
	$(__UNICODE_DEFINE_p) -i=..\..\src\ogl\..\..\..\include -i=$(SETUPHDIR) &
	-i=..\..\src\ogl\..\..\include $(CXXFLAGS) /fh=$(OBJS)\wxprec_ogllib.pch
OGLLIB_OBJECTS =  &
	$(OBJS)\ogllib_dummy.obj &
	$(OBJS)\ogllib_basic2.obj &
	$(OBJS)\ogllib_canvas.obj &
	$(OBJS)\ogllib_divided.obj &
	$(OBJS)\ogllib_mfutils.obj &
	$(OBJS)\ogllib_oglmisc.obj &
	$(OBJS)\ogllib_basic.obj &
	$(OBJS)\ogllib_composit.obj &
	$(OBJS)\ogllib_drawn.obj &
	$(OBJS)\ogllib_bmpshape.obj &
	$(OBJS)\ogllib_constrnt.obj &
	$(OBJS)\ogllib_lines.obj &
	$(OBJS)\ogllib_ogldiag.obj
SETUPHDIR = &
	$(LIBDIRNAME)\$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)


all : $(OBJS)
$(OBJS) :
	-if not exist $(OBJS) mkdir $(OBJS)

### Targets: ###

all : .SYMBOLIC $(__ogldll___depname) $(__ogllib___depname)

$(OBJS)\ogldll_basic.obj :  .AUTODEPEND ../../src/ogl\basic.cpp
	$(CXX) -zq -fo=$^@ $(OGLDLL_CXXFLAGS) $<

$(OBJS)\ogldll_basic2.obj :  .AUTODEPEND ../../src/ogl\basic2.cpp
	$(CXX) -zq -fo=$^@ $(OGLDLL_CXXFLAGS) $<

$(OBJS)\ogldll_bmpshape.obj :  .AUTODEPEND ../../src/ogl\bmpshape.cpp
	$(CXX) -zq -fo=$^@ $(OGLDLL_CXXFLAGS) $<

$(OBJS)\ogldll_canvas.obj :  .AUTODEPEND ../../src/ogl\canvas.cpp
	$(CXX) -zq -fo=$^@ $(OGLDLL_CXXFLAGS) $<

$(OBJS)\ogldll_composit.obj :  .AUTODEPEND ../../src/ogl\composit.cpp
	$(CXX) -zq -fo=$^@ $(OGLDLL_CXXFLAGS) $<

$(OBJS)\ogldll_constrnt.obj :  .AUTODEPEND ../../src/ogl\constrnt.cpp
	$(CXX) -zq -fo=$^@ $(OGLDLL_CXXFLAGS) $<

$(OBJS)\ogldll_divided.obj :  .AUTODEPEND ../../src/ogl\divided.cpp
	$(CXX) -zq -fo=$^@ $(OGLDLL_CXXFLAGS) $<

$(OBJS)\ogldll_drawn.obj :  .AUTODEPEND ../../src/ogl\drawn.cpp
	$(CXX) -zq -fo=$^@ $(OGLDLL_CXXFLAGS) $<

$(OBJS)\ogldll_dummy.obj :  .AUTODEPEND ../../src/ogl\..\..\..\src\msw\dummy.cpp
	$(CXX) -zq -fo=$^@ $(OGLDLL_CXXFLAGS) $<

$(OBJS)\ogldll_lines.obj :  .AUTODEPEND ../../src/ogl\lines.cpp
	$(CXX) -zq -fo=$^@ $(OGLDLL_CXXFLAGS) $<

$(OBJS)\ogldll_mfutils.obj :  .AUTODEPEND ../../src/ogl\mfutils.cpp
	$(CXX) -zq -fo=$^@ $(OGLDLL_CXXFLAGS) $<

$(OBJS)\ogldll_ogldiag.obj :  .AUTODEPEND ../../src/ogl\ogldiag.cpp
	$(CXX) -zq -fo=$^@ $(OGLDLL_CXXFLAGS) $<

$(OBJS)\ogldll_oglmisc.obj :  .AUTODEPEND ../../src/ogl\oglmisc.cpp
	$(CXX) -zq -fo=$^@ $(OGLDLL_CXXFLAGS) $<

$(OBJS)\ogllib_basic.obj :  .AUTODEPEND ../../src/ogl\basic.cpp
	$(CXX) -zq -fo=$^@ $(OGLLIB_CXXFLAGS) $<

$(OBJS)\ogllib_basic2.obj :  .AUTODEPEND ../../src/ogl\basic2.cpp
	$(CXX) -zq -fo=$^@ $(OGLLIB_CXXFLAGS) $<

$(OBJS)\ogllib_bmpshape.obj :  .AUTODEPEND ../../src/ogl\bmpshape.cpp
	$(CXX) -zq -fo=$^@ $(OGLLIB_CXXFLAGS) $<

$(OBJS)\ogllib_canvas.obj :  .AUTODEPEND ../../src/ogl\canvas.cpp
	$(CXX) -zq -fo=$^@ $(OGLLIB_CXXFLAGS) $<

$(OBJS)\ogllib_composit.obj :  .AUTODEPEND ../../src/ogl\composit.cpp
	$(CXX) -zq -fo=$^@ $(OGLLIB_CXXFLAGS) $<

$(OBJS)\ogllib_constrnt.obj :  .AUTODEPEND ../../src/ogl\constrnt.cpp
	$(CXX) -zq -fo=$^@ $(OGLLIB_CXXFLAGS) $<

$(OBJS)\ogllib_divided.obj :  .AUTODEPEND ../../src/ogl\divided.cpp
	$(CXX) -zq -fo=$^@ $(OGLLIB_CXXFLAGS) $<

$(OBJS)\ogllib_drawn.obj :  .AUTODEPEND ../../src/ogl\drawn.cpp
	$(CXX) -zq -fo=$^@ $(OGLLIB_CXXFLAGS) $<

$(OBJS)\ogllib_dummy.obj :  .AUTODEPEND ../../src/ogl\..\..\..\src\msw\dummy.cpp
	$(CXX) -zq -fo=$^@ $(OGLLIB_CXXFLAGS) $<

$(OBJS)\ogllib_lines.obj :  .AUTODEPEND ../../src/ogl\lines.cpp
	$(CXX) -zq -fo=$^@ $(OGLLIB_CXXFLAGS) $<

$(OBJS)\ogllib_mfutils.obj :  .AUTODEPEND ../../src/ogl\mfutils.cpp
	$(CXX) -zq -fo=$^@ $(OGLLIB_CXXFLAGS) $<

$(OBJS)\ogllib_ogldiag.obj :  .AUTODEPEND ../../src/ogl\ogldiag.cpp
	$(CXX) -zq -fo=$^@ $(OGLLIB_CXXFLAGS) $<

$(OBJS)\ogllib_oglmisc.obj :  .AUTODEPEND ../../src/ogl\oglmisc.cpp
	$(CXX) -zq -fo=$^@ $(OGLLIB_CXXFLAGS) $<

clean : .SYMBOLIC 
	-if exist $(OBJS)\*.obj del $(OBJS)\*.obj
	-if exist $(OBJS)\*.res del $(OBJS)\*.res
	-if exist $(OBJS)\*.lbc del $(OBJS)\*.lbc
	-if exist $(OBJS)\*.ilk del $(OBJS)\*.ilk
	-if exist $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)250$(WXUNICODEFLAG)$(WXDEBUGFLAG)_ogl_wat$(VENDORTAG).dll del $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)250$(WXUNICODEFLAG)$(WXDEBUGFLAG)_ogl_wat$(VENDORTAG).dll
	-if exist $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_ogl.lib del $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_ogl.lib
	-if exist $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_ogl.lib del $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_ogl.lib

!ifeq SHARED 1
$(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)250$(WXUNICODEFLAG)$(WXDEBUGFLAG)_ogl_wat$(VENDORTAG).dll :  $(OGLDLL_OBJECTS)
	@%create $(OBJS)\ogldll.lbc
	@%append $(OBJS)\ogldll.lbc option quiet
	@%append $(OBJS)\ogldll.lbc name $^@
	@%append $(OBJS)\ogldll.lbc option caseexact
	@%append $(OBJS)\ogldll.lbc $(LDFLAGS) $(__DEBUGINFO_1)  libpath $(LIBDIRNAME)
	@for %i in ($(OGLDLL_OBJECTS)) do @%append $(OBJS)\ogldll.lbc file %i
	@for %i in ( $(__WXLIB_MONO_p) $(__LIB_TIFF_p) $(__LIB_JPEG_p) $(__LIB_PNG_p) wxzlib$(WXDEBUGFLAG).lib  wxregex$(WXDEBUGFLAG).lib wxexpat$(WXDEBUGFLAG).lib   kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib  $(__WXLIB_CORE_p) $(__WXLIB_BASE_p)) do @%append $(OBJS)\ogldll.lbc library %i
	@%append $(OBJS)\ogldll.lbc
	@%append $(OBJS)\ogldll.lbc system nt_dll
	wlink @$(OBJS)\ogldll.lbc
	wlib -q -n -b $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_ogl.lib +$^@
!endif

!ifeq SHARED 0
$(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_ogl.lib :  $(OGLLIB_OBJECTS)
	@%create $(OBJS)\ogllib.lbc
	@for %i in ($(OGLLIB_OBJECTS)) do @%append $(OBJS)\ogllib.lbc +%i
	wlib -q -p2048 -n -b $^@ @$(OBJS)\ogllib.lbc
!endif
