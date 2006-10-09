# =========================================================================
#     This makefile was generated by
#     Bakefile 0.2.0 (http://bakefile.sourceforge.net)
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
LIBTYPE_SUFFIX =
!ifeq SHARED 0
LIBTYPE_SUFFIX = lib
!endif
!ifeq SHARED 1
LIBTYPE_SUFFIX = dll
!endif
EXTRALIBS_FOR_BASE =
!ifeq MONOLITHIC 0
EXTRALIBS_FOR_BASE = 
!endif
!ifeq MONOLITHIC 1
EXTRALIBS_FOR_BASE =  
!endif
__mmediadll___depname =
!ifeq SHARED 1
__mmediadll___depname = &
	$(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)$(WX_VERSION_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)_mmedia_wat$(VENDORTAG).dll
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
	wx$(PORTNAME)$(WXUNIVNAME)$(WX_RELEASE_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR).lib
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
__GDIPLUS_LIB_p =
!ifeq USE_GDIPLUS 1
__GDIPLUS_LIB_p = gdiplus.lib
!endif
__WXLIB_CORE_p =
!ifeq MONOLITHIC 0
__WXLIB_CORE_p = &
	wx$(PORTNAME)$(WXUNIVNAME)$(WX_RELEASE_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)_core.lib
!endif
__WXLIB_BASE_p =
!ifeq MONOLITHIC 0
__WXLIB_BASE_p = &
	wxbase$(WX_RELEASE_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR).lib
!endif
__mmedialib___depname =
!ifeq SHARED 0
__mmedialib___depname = &
	$(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)$(WX_RELEASE_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)_mmedia.lib
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
__THREADSFLAG =
!ifeq USE_THREADS 0
__THREADSFLAG = 
!endif
!ifeq USE_THREADS 1
__THREADSFLAG = -bm
!endif
__RUNTIME_LIBS =
!ifeq RUNTIME_LIBS dynamic
__RUNTIME_LIBS = -br
!endif
!ifeq RUNTIME_LIBS static
__RUNTIME_LIBS = 
!endif
__RTTIFLAG =
!ifeq USE_RTTI 0
__RTTIFLAG = 
!endif
!ifeq USE_RTTI 1
__RTTIFLAG = -xr
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
__EXCEPTIONS_DEFINE_p =
!ifeq USE_EXCEPTIONS 0
__EXCEPTIONS_DEFINE_p = -dwxNO_EXCEPTIONS
!endif
__RTTI_DEFINE_p =
!ifeq USE_RTTI 0
__RTTI_DEFINE_p = -dwxNO_RTTI
!endif
__THREAD_DEFINE_p =
!ifeq USE_THREADS 0
__THREAD_DEFINE_p = -dwxNO_THREADS
!endif
__UNICODE_DEFINE_p =
!ifeq UNICODE 1
__UNICODE_DEFINE_p = -d_UNICODE
!endif
__GFXCTX_DEFINE_p =
!ifeq USE_GDIPLUS 1
__GFXCTX_DEFINE_p = -dwxUSE_GRAPHICS_CONTEXT=1
!endif

### Variables: ###

WX_RELEASE_NODOT = 27
WX_VERSION_NODOT = $(WX_RELEASE_NODOT)1
OBJS = &
	wat_$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WXDLLFLAG)$(CFG)
LIBDIRNAME = ..\..\src\mmedia\..\..\..\lib\wat_$(LIBTYPE_SUFFIX)$(CFG)
SETUPHDIR = &
	$(LIBDIRNAME)\$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)
MMEDIADLL_CXXFLAGS = -bd $(__DEBUGINFO_1) $(__OPTIMIZEFLAG) $(__THREADSFLAG) &
	$(__RUNTIME_LIBS) -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) &
	$(__EXCEPTIONS_DEFINE_p) $(__RTTI_DEFINE_p) $(__THREAD_DEFINE_p) &
	$(__UNICODE_DEFINE_p) $(__GFXCTX_DEFINE_p) -i=$(SETUPHDIR) &
	-i=..\..\src\mmedia\..\..\..\include -wx -wcd=549 -wcd=656 -wcd=657 -wcd=667 &
	-i=..\..\src\mmedia\..\..\include -dWXUSINGDLL -dWXMAKINGDLL_MMEDIA &
	/fh=$(OBJS)\wxprec_mmediadll.pch $(__RTTIFLAG) $(__EXCEPTIONSFLAG) &
	$(CPPFLAGS) $(CXXFLAGS)
MMEDIADLL_OBJECTS =  &
	$(OBJS)\mmediadll_dummy.obj &
	$(OBJS)\mmediadll_cdwin.obj &
	$(OBJS)\mmediadll_sndwin.obj &
	$(OBJS)\mmediadll_vidwin.obj &
	$(OBJS)\mmediadll_cdbase.obj &
	$(OBJS)\mmediadll_g711.obj &
	$(OBJS)\mmediadll_g721.obj &
	$(OBJS)\mmediadll_g723_24.obj &
	$(OBJS)\mmediadll_g723_40.obj &
	$(OBJS)\mmediadll_g72x.obj &
	$(OBJS)\mmediadll_sndaiff.obj &
	$(OBJS)\mmediadll_sndbase.obj &
	$(OBJS)\mmediadll_sndcodec.obj &
	$(OBJS)\mmediadll_sndcpcm.obj &
	$(OBJS)\mmediadll_sndfile.obj &
	$(OBJS)\mmediadll_sndg72x.obj &
	$(OBJS)\mmediadll_sndmsad.obj &
	$(OBJS)\mmediadll_sndpcm.obj &
	$(OBJS)\mmediadll_sndulaw.obj &
	$(OBJS)\mmediadll_sndwav.obj &
	$(OBJS)\mmediadll_vidbase.obj
MMEDIALIB_CXXFLAGS = $(__DEBUGINFO_1) $(__OPTIMIZEFLAG) $(__THREADSFLAG) &
	$(__RUNTIME_LIBS) -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) &
	$(__EXCEPTIONS_DEFINE_p) $(__RTTI_DEFINE_p) $(__THREAD_DEFINE_p) &
	$(__UNICODE_DEFINE_p) $(__GFXCTX_DEFINE_p) -i=$(SETUPHDIR) &
	-i=..\..\src\mmedia\..\..\..\include -wx -wcd=549 -wcd=656 -wcd=657 -wcd=667 &
	-i=..\..\src\mmedia\..\..\include /fh=$(OBJS)\wxprec_mmedialib.pch &
	$(__RTTIFLAG) $(__EXCEPTIONSFLAG) $(CPPFLAGS) $(CXXFLAGS)
MMEDIALIB_OBJECTS =  &
	$(OBJS)\mmedialib_dummy.obj &
	$(OBJS)\mmedialib_cdwin.obj &
	$(OBJS)\mmedialib_sndwin.obj &
	$(OBJS)\mmedialib_vidwin.obj &
	$(OBJS)\mmedialib_cdbase.obj &
	$(OBJS)\mmedialib_g711.obj &
	$(OBJS)\mmedialib_g721.obj &
	$(OBJS)\mmedialib_g723_24.obj &
	$(OBJS)\mmedialib_g723_40.obj &
	$(OBJS)\mmedialib_g72x.obj &
	$(OBJS)\mmedialib_sndaiff.obj &
	$(OBJS)\mmedialib_sndbase.obj &
	$(OBJS)\mmedialib_sndcodec.obj &
	$(OBJS)\mmedialib_sndcpcm.obj &
	$(OBJS)\mmedialib_sndfile.obj &
	$(OBJS)\mmedialib_sndg72x.obj &
	$(OBJS)\mmedialib_sndmsad.obj &
	$(OBJS)\mmedialib_sndpcm.obj &
	$(OBJS)\mmedialib_sndulaw.obj &
	$(OBJS)\mmedialib_sndwav.obj &
	$(OBJS)\mmedialib_vidbase.obj


all : $(OBJS)
$(OBJS) :
	-if not exist $(OBJS) mkdir $(OBJS)

### Targets: ###

all : .SYMBOLIC $(__mmediadll___depname) $(__mmedialib___depname)

clean : .SYMBOLIC 
	-if exist $(OBJS)\*.obj del $(OBJS)\*.obj
	-if exist $(OBJS)\*.res del $(OBJS)\*.res
	-if exist $(OBJS)\*.lbc del $(OBJS)\*.lbc
	-if exist $(OBJS)\*.ilk del $(OBJS)\*.ilk
	-if exist $(OBJS)\*.pch del $(OBJS)\*.pch
	-if exist $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)$(WX_VERSION_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)_mmedia_wat$(VENDORTAG).dll del $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)$(WX_VERSION_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)_mmedia_wat$(VENDORTAG).dll
	-if exist $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)$(WX_RELEASE_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)_mmedia.lib del $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)$(WX_RELEASE_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)_mmedia.lib
	-if exist $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)$(WX_RELEASE_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)_mmedia.lib del $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)$(WX_RELEASE_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)_mmedia.lib

!ifeq SHARED 1
$(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)$(WX_VERSION_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)_mmedia_wat$(VENDORTAG).dll :  $(MMEDIADLL_OBJECTS) $(OBJS)\mmediadll_version.res
	@%create $(OBJS)\mmediadll.lbc
	@%append $(OBJS)\mmediadll.lbc option quiet
	@%append $(OBJS)\mmediadll.lbc name $^@
	@%append $(OBJS)\mmediadll.lbc option caseexact
	@%append $(OBJS)\mmediadll.lbc $(LDFLAGS) $(__DEBUGINFO_2)  libpath $(LIBDIRNAME)
	@for %i in ($(MMEDIADLL_OBJECTS)) do @%append $(OBJS)\mmediadll.lbc file %i
	@for %i in ( $(__WXLIB_MONO_p) $(__LIB_TIFF_p) $(__LIB_JPEG_p) $(__LIB_PNG_p)  wxzlib$(WXDEBUGFLAG).lib  wxregex$(WXUNICODEFLAG)$(WXDEBUGFLAG).lib wxexpat$(WXDEBUGFLAG).lib $(EXTRALIBS_FOR_BASE)  $(__GDIPLUS_LIB_p) kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib $(__WXLIB_CORE_p)  $(__WXLIB_BASE_p) ) do @%append $(OBJS)\mmediadll.lbc library %i
	@%append $(OBJS)\mmediadll.lbc option resource=$(OBJS)\mmediadll_version.res
	@%append $(OBJS)\mmediadll.lbc system nt_dll
	wlink @$(OBJS)\mmediadll.lbc
	wlib -q -n -b $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)$(WX_RELEASE_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)_mmedia.lib +$^@
!endif

!ifeq SHARED 0
$(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)$(WX_RELEASE_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)_mmedia.lib :  $(MMEDIALIB_OBJECTS)
	@%create $(OBJS)\mmedialib.lbc
	@for %i in ($(MMEDIALIB_OBJECTS)) do @%append $(OBJS)\mmedialib.lbc +%i
	wlib -q -p4096 -n -b $^@ @$(OBJS)\mmedialib.lbc
!endif

$(OBJS)\mmediadll_dummy.obj :  .AUTODEPEND ../../src/mmedia\..\..\..\src\common\dummy.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MMEDIADLL_CXXFLAGS) $<

$(OBJS)\mmediadll_version.res :  .AUTODEPEND ../../src/mmedia\..\..\..\src\msw\version.rc
	wrc -q -ad -bt=nt -r -fo=$^@   -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) $(__EXCEPTIONS_DEFINE_p) $(__RTTI_DEFINE_p) $(__THREAD_DEFINE_p) $(__UNICODE_DEFINE_p)  $(__GFXCTX_DEFINE_p) -i=$(SETUPHDIR) -i=..\..\src\mmedia\..\..\..\include -dWXDLLNAME=wx$(PORTNAME)$(WXUNIVNAME)$(WX_VERSION_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)_mmedia_wat$(VENDORTAG) $<

$(OBJS)\mmediadll_cdwin.obj :  .AUTODEPEND ../../src/mmedia\cdwin.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MMEDIADLL_CXXFLAGS) $<

$(OBJS)\mmediadll_sndwin.obj :  .AUTODEPEND ../../src/mmedia\sndwin.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MMEDIADLL_CXXFLAGS) $<

$(OBJS)\mmediadll_vidwin.obj :  .AUTODEPEND ../../src/mmedia\vidwin.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MMEDIADLL_CXXFLAGS) $<

$(OBJS)\mmediadll_cdbase.obj :  .AUTODEPEND ../../src/mmedia\cdbase.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MMEDIADLL_CXXFLAGS) $<

$(OBJS)\mmediadll_g711.obj :  .AUTODEPEND ../../src/mmedia\g711.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MMEDIADLL_CXXFLAGS) $<

$(OBJS)\mmediadll_g721.obj :  .AUTODEPEND ../../src/mmedia\g721.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MMEDIADLL_CXXFLAGS) $<

$(OBJS)\mmediadll_g723_24.obj :  .AUTODEPEND ../../src/mmedia\g723_24.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MMEDIADLL_CXXFLAGS) $<

$(OBJS)\mmediadll_g723_40.obj :  .AUTODEPEND ../../src/mmedia\g723_40.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MMEDIADLL_CXXFLAGS) $<

$(OBJS)\mmediadll_g72x.obj :  .AUTODEPEND ../../src/mmedia\g72x.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MMEDIADLL_CXXFLAGS) $<

$(OBJS)\mmediadll_sndaiff.obj :  .AUTODEPEND ../../src/mmedia\sndaiff.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MMEDIADLL_CXXFLAGS) $<

$(OBJS)\mmediadll_sndbase.obj :  .AUTODEPEND ../../src/mmedia\sndbase.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MMEDIADLL_CXXFLAGS) $<

$(OBJS)\mmediadll_sndcodec.obj :  .AUTODEPEND ../../src/mmedia\sndcodec.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MMEDIADLL_CXXFLAGS) $<

$(OBJS)\mmediadll_sndcpcm.obj :  .AUTODEPEND ../../src/mmedia\sndcpcm.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MMEDIADLL_CXXFLAGS) $<

$(OBJS)\mmediadll_sndfile.obj :  .AUTODEPEND ../../src/mmedia\sndfile.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MMEDIADLL_CXXFLAGS) $<

$(OBJS)\mmediadll_sndg72x.obj :  .AUTODEPEND ../../src/mmedia\sndg72x.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MMEDIADLL_CXXFLAGS) $<

$(OBJS)\mmediadll_sndmsad.obj :  .AUTODEPEND ../../src/mmedia\sndmsad.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MMEDIADLL_CXXFLAGS) $<

$(OBJS)\mmediadll_sndpcm.obj :  .AUTODEPEND ../../src/mmedia\sndpcm.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MMEDIADLL_CXXFLAGS) $<

$(OBJS)\mmediadll_sndulaw.obj :  .AUTODEPEND ../../src/mmedia\sndulaw.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MMEDIADLL_CXXFLAGS) $<

$(OBJS)\mmediadll_sndwav.obj :  .AUTODEPEND ../../src/mmedia\sndwav.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MMEDIADLL_CXXFLAGS) $<

$(OBJS)\mmediadll_vidbase.obj :  .AUTODEPEND ../../src/mmedia\vidbase.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MMEDIADLL_CXXFLAGS) $<

$(OBJS)\mmedialib_dummy.obj :  .AUTODEPEND ../../src/mmedia\..\..\..\src\common\dummy.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MMEDIALIB_CXXFLAGS) $<

$(OBJS)\mmedialib_cdwin.obj :  .AUTODEPEND ../../src/mmedia\cdwin.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MMEDIALIB_CXXFLAGS) $<

$(OBJS)\mmedialib_sndwin.obj :  .AUTODEPEND ../../src/mmedia\sndwin.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MMEDIALIB_CXXFLAGS) $<

$(OBJS)\mmedialib_vidwin.obj :  .AUTODEPEND ../../src/mmedia\vidwin.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MMEDIALIB_CXXFLAGS) $<

$(OBJS)\mmedialib_cdbase.obj :  .AUTODEPEND ../../src/mmedia\cdbase.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MMEDIALIB_CXXFLAGS) $<

$(OBJS)\mmedialib_g711.obj :  .AUTODEPEND ../../src/mmedia\g711.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MMEDIALIB_CXXFLAGS) $<

$(OBJS)\mmedialib_g721.obj :  .AUTODEPEND ../../src/mmedia\g721.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MMEDIALIB_CXXFLAGS) $<

$(OBJS)\mmedialib_g723_24.obj :  .AUTODEPEND ../../src/mmedia\g723_24.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MMEDIALIB_CXXFLAGS) $<

$(OBJS)\mmedialib_g723_40.obj :  .AUTODEPEND ../../src/mmedia\g723_40.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MMEDIALIB_CXXFLAGS) $<

$(OBJS)\mmedialib_g72x.obj :  .AUTODEPEND ../../src/mmedia\g72x.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MMEDIALIB_CXXFLAGS) $<

$(OBJS)\mmedialib_sndaiff.obj :  .AUTODEPEND ../../src/mmedia\sndaiff.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MMEDIALIB_CXXFLAGS) $<

$(OBJS)\mmedialib_sndbase.obj :  .AUTODEPEND ../../src/mmedia\sndbase.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MMEDIALIB_CXXFLAGS) $<

$(OBJS)\mmedialib_sndcodec.obj :  .AUTODEPEND ../../src/mmedia\sndcodec.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MMEDIALIB_CXXFLAGS) $<

$(OBJS)\mmedialib_sndcpcm.obj :  .AUTODEPEND ../../src/mmedia\sndcpcm.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MMEDIALIB_CXXFLAGS) $<

$(OBJS)\mmedialib_sndfile.obj :  .AUTODEPEND ../../src/mmedia\sndfile.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MMEDIALIB_CXXFLAGS) $<

$(OBJS)\mmedialib_sndg72x.obj :  .AUTODEPEND ../../src/mmedia\sndg72x.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MMEDIALIB_CXXFLAGS) $<

$(OBJS)\mmedialib_sndmsad.obj :  .AUTODEPEND ../../src/mmedia\sndmsad.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MMEDIALIB_CXXFLAGS) $<

$(OBJS)\mmedialib_sndpcm.obj :  .AUTODEPEND ../../src/mmedia\sndpcm.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MMEDIALIB_CXXFLAGS) $<

$(OBJS)\mmedialib_sndulaw.obj :  .AUTODEPEND ../../src/mmedia\sndulaw.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MMEDIALIB_CXXFLAGS) $<

$(OBJS)\mmedialib_sndwav.obj :  .AUTODEPEND ../../src/mmedia\sndwav.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MMEDIALIB_CXXFLAGS) $<

$(OBJS)\mmedialib_vidbase.obj :  .AUTODEPEND ../../src/mmedia\vidbase.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MMEDIALIB_CXXFLAGS) $<

