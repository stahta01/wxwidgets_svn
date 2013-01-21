# =========================================================================
#     This makefile was generated by
#     Bakefile 0.2.9 (http://www.bakefile.org)
#     Do not modify, all changes will be overwritten!
# =========================================================================

!include ../../build/msw/config.wat

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
! if $(__VERSION__) >= 1280
!  loaddll wlink    wlinkd
! else
!  loaddll wlink    wlink
! endif
!  loaddll wlib     wlibd
!endif

# We need these variables in some bakefile-made rules:
WATCOM_CWD = $+ $(%cdrive):$(%cwd) $-

### Conditionally set variables: ###

WIN32_TOOLKIT_LOWERCASE =
!ifeq TOOLKIT GTK
WIN32_TOOLKIT_LOWERCASE = gtk
!endif
!ifeq TOOLKIT MSW
WIN32_TOOLKIT_LOWERCASE = msw
!endif
PORTNAME =
!ifeq USE_GUI 0
PORTNAME = base
!endif
!ifeq USE_GUI 1
PORTNAME = $(WIN32_TOOLKIT_LOWERCASE)$(TOOLKIT_VERSION)
!endif
WXBASEPORT =
!ifeq TOOLKIT MAC
WXBASEPORT = _carbon
!endif
COMPILER_VERSION =
!ifeq OFFICIAL_BUILD 1
COMPILER_VERSION = ERROR-COMPILER-VERSION-MUST-BE-SET-FOR-OFFICIAL-BUILD
!endif
WXDEBUGFLAG =
!ifeq BUILD debug
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
LIB_GTK =
!ifeq TOOLKIT GTK
!ifeq TOOLKIT_VERSION 2
LIB_GTK = gtk-win32-2.0.lib gdk-win32-2.0.lib pangocairo-1.0.lib &
	gdk_pixbuf-2.0.lib cairo.lib pango-1.0.lib gobject-2.0.lib gthread-2.0.lib &
	glib-2.0.lib
!endif
!endif
__DEBUGINFO_0 =
!ifeq BUILD debug
!ifeq DEBUG_INFO default
__DEBUGINFO_0 = -d2
!endif
!endif
!ifeq BUILD release
!ifeq DEBUG_INFO default
__DEBUGINFO_0 = -d0
!endif
!endif
!ifeq DEBUG_INFO 0
__DEBUGINFO_0 = -d0
!endif
!ifeq DEBUG_INFO 1
__DEBUGINFO_0 = -d2
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
__OPTIMIZEFLAG_2 =
!ifeq BUILD debug
__OPTIMIZEFLAG_2 = -od
!endif
!ifeq BUILD release
__OPTIMIZEFLAG_2 = -ot -ox
!endif
__THREADSFLAG_5 =
!ifeq USE_THREADS 0
__THREADSFLAG_5 = 
!endif
!ifeq USE_THREADS 1
__THREADSFLAG_5 = -bm
!endif
__RUNTIME_LIBS_6 =
!ifeq RUNTIME_LIBS dynamic
__RUNTIME_LIBS_6 = -br
!endif
!ifeq RUNTIME_LIBS static
__RUNTIME_LIBS_6 = 
!endif
__RTTIFLAG_7 =
!ifeq USE_RTTI 0
__RTTIFLAG_7 = 
!endif
!ifeq USE_RTTI 1
__RTTIFLAG_7 = -xr
!endif
__EXCEPTIONSFLAG_8 =
!ifeq USE_EXCEPTIONS 0
__EXCEPTIONSFLAG_8 = 
!endif
!ifeq USE_EXCEPTIONS 1
__EXCEPTIONSFLAG_8 = -xs
!endif
__WXLIB_CORE_p =
!ifeq MONOLITHIC 0
__WXLIB_CORE_p = &
	wx$(PORTNAME)$(WXUNIVNAME)$(WX_RELEASE_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)_core.lib
!endif
__WXLIB_BASE_p =
!ifeq MONOLITHIC 0
__WXLIB_BASE_p = &
	wxbase$(WXBASEPORT)$(WX_RELEASE_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR).lib
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
__CAIRO_LIB_p =
!ifeq USE_CAIRO 1
__CAIRO_LIB_p = cairo.lib
!endif
____CAIRO_LIBDIR_FILENAMES_p =
!ifeq USE_CAIRO 1
____CAIRO_LIBDIR_FILENAMES_p = libpath $(CAIRO_ROOT)\lib
!endif
__WXUNIV_DEFINE_p =
!ifeq WXUNIV 1
__WXUNIV_DEFINE_p = -d__WXUNIVERSAL__
!endif
__DEBUG_DEFINE_p =
!ifeq DEBUG_FLAG 0
__DEBUG_DEFINE_p = -dwxDEBUG_LEVEL=0
!endif
__NDEBUG_DEFINE_p =
!ifeq BUILD release
__NDEBUG_DEFINE_p = -dNDEBUG
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
!ifeq UNICODE 0
__UNICODE_DEFINE_p = -dwxUSE_UNICODE=0
!endif
!ifeq UNICODE 1
__UNICODE_DEFINE_p = -d_UNICODE
!endif
____CAIRO_INCLUDEDIR_FILENAMES =
!ifeq USE_CAIRO 1
____CAIRO_INCLUDEDIR_FILENAMES = -i=$(CAIRO_ROOT)\include\cairo
!endif
__DLLFLAG_p =
!ifeq SHARED 1
__DLLFLAG_p = -dWXUSINGDLL
!endif

### Variables: ###

WX_RELEASE_NODOT = 29
COMPILER_PREFIX = wat
OBJS = &
	$(COMPILER_PREFIX)$(COMPILER_VERSION)_$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WXDLLFLAG)$(CFG)
LIBDIRNAME = &
	.\..\..\lib\$(COMPILER_PREFIX)$(COMPILER_VERSION)_$(LIBTYPE_SUFFIX)$(CFG)
SETUPHDIR = &
	$(LIBDIRNAME)\$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)
STATBAR_CXXFLAGS = $(__DEBUGINFO_0) $(__OPTIMIZEFLAG_2) $(__THREADSFLAG_5) &
	$(__RUNTIME_LIBS_6) -d__WX$(TOOLKIT)__ $(__WXUNIV_DEFINE_p) &
	$(__DEBUG_DEFINE_p) $(__NDEBUG_DEFINE_p) $(__EXCEPTIONS_DEFINE_p) &
	$(__RTTI_DEFINE_p) $(__THREAD_DEFINE_p) $(__UNICODE_DEFINE_p) &
	-i=$(SETUPHDIR) -i=.\..\..\include $(____CAIRO_INCLUDEDIR_FILENAMES) -wx &
	-wcd=549 -wcd=656 -wcd=657 -wcd=667 -i=. $(__DLLFLAG_p) -i=.\..\..\samples &
	-dNOPCH $(__RTTIFLAG_7) $(__EXCEPTIONSFLAG_8) $(CPPFLAGS) $(CXXFLAGS)
STATBAR_OBJECTS =  &
	$(OBJS)\statbar_statbar.obj


all : $(OBJS)
$(OBJS) :
	-if not exist $(OBJS) mkdir $(OBJS)

### Targets: ###

all : .SYMBOLIC $(OBJS)\statbar.exe

clean : .SYMBOLIC 
	-if exist $(OBJS)\*.obj del $(OBJS)\*.obj
	-if exist $(OBJS)\*.res del $(OBJS)\*.res
	-if exist $(OBJS)\*.lbc del $(OBJS)\*.lbc
	-if exist $(OBJS)\*.ilk del $(OBJS)\*.ilk
	-if exist $(OBJS)\*.pch del $(OBJS)\*.pch
	-if exist $(OBJS)\statbar.exe del $(OBJS)\statbar.exe

$(OBJS)\statbar.exe :  $(STATBAR_OBJECTS) $(OBJS)\statbar_sample.res
	@%create $(OBJS)\statbar.lbc
	@%append $(OBJS)\statbar.lbc option quiet
	@%append $(OBJS)\statbar.lbc name $^@
	@%append $(OBJS)\statbar.lbc option caseexact
	@%append $(OBJS)\statbar.lbc  $(__DEBUGINFO_1)  libpath $(LIBDIRNAME) system nt_win ref '_WinMain@16' $(____CAIRO_LIBDIR_FILENAMES_p) $(LDFLAGS)
	@for %i in ($(STATBAR_OBJECTS)) do @%append $(OBJS)\statbar.lbc file %i
	@for %i in ( $(__WXLIB_CORE_p)  $(__WXLIB_BASE_p)  $(__WXLIB_MONO_p) $(__LIB_TIFF_p) $(__LIB_JPEG_p) $(__LIB_PNG_p) $(LIB_GTK)  wxzlib$(WXDEBUGFLAG).lib wxregex$(WXUNICODEFLAG)$(WXDEBUGFLAG).lib wxexpat$(WXDEBUGFLAG).lib $(EXTRALIBS_FOR_BASE)  $(__CAIRO_LIB_p) kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib wininet.lib) do @%append $(OBJS)\statbar.lbc library %i
	@%append $(OBJS)\statbar.lbc option resource=$(OBJS)\statbar_sample.res
	@for %i in () do @%append $(OBJS)\statbar.lbc option stack=%i
	wlink @$(OBJS)\statbar.lbc

$(OBJS)\statbar_sample.res :  .AUTODEPEND .\..\..\samples\sample.rc
	wrc -q -ad -bt=nt -r -fo=$^@    -d__WX$(TOOLKIT)__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) $(__NDEBUG_DEFINE_p) $(__EXCEPTIONS_DEFINE_p) $(__RTTI_DEFINE_p) $(__THREAD_DEFINE_p) $(__UNICODE_DEFINE_p)  -i=$(SETUPHDIR) -i=.\..\..\include $(____CAIRO_INCLUDEDIR_FILENAMES) -i=. $(__DLLFLAG_p) -i=.\..\..\samples -dNOPCH $<

$(OBJS)\statbar_statbar.obj :  .AUTODEPEND .\statbar.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(STATBAR_CXXFLAGS) $<

