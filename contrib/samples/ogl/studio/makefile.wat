# =========================================================================
#     This makefile was generated by
#     Bakefile 0.2.8 (http://www.bakefile.org)
#     Do not modify, all changes will be overwritten!
# =========================================================================

!include ../../../../build/os2/config.wat

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

PORTNAME =
!ifeq USE_GUI 0
PORTNAME = base
!endif
!ifeq USE_GUI 1
PORTNAME = pm
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
LIBTYPE_SUFFIX = pm_lib
!endif
!ifeq SHARED 1
LIBTYPE_SUFFIX = pm_dll
!endif
EXTRALIBS_FOR_BASE =
!ifeq MONOLITHIC 0
EXTRALIBS_FOR_BASE = 
!endif
!ifeq MONOLITHIC 1
EXTRALIBS_FOR_BASE =  
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
!ifeq UNICODE 1
__UNICODE_DEFINE_p = -d_UNICODE
!endif
__GFXCTX_DEFINE_p =
!ifeq USE_GDIPLUS 1
__GFXCTX_DEFINE_p = -dwxUSE_GRAPHICS_CONTEXT=1
!endif
____CAIRO_INCLUDEDIR_FILENAMES_p =
!ifeq USE_CAIRO 1
____CAIRO_INCLUDEDIR_FILENAMES_p = -i=$(CAIRO_ROOT)\include\cairo
!endif
__DLLFLAG_p =
!ifeq SHARED 1
__DLLFLAG_p = -dWXUSINGDLL
!endif
__WXLIB_ADV_p =
!ifeq MONOLITHIC 0
__WXLIB_ADV_p = &
	wx$(PORTNAME)$(WXUNIVNAME)$(WX_RELEASE_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)_adv.lib
!endif
__WXLIB_HTML_p =
!ifeq MONOLITHIC 0
__WXLIB_HTML_p = &
	wx$(PORTNAME)$(WXUNIVNAME)$(WX_RELEASE_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)_html.lib
!endif
__WXLIB_XML_p =
!ifeq MONOLITHIC 0
__WXLIB_XML_p = &
	wxbase$(WX_RELEASE_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)_xml.lib
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

### Variables: ###

WX_RELEASE_NODOT = 28
OBJS = &
	wat_$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WXDLLFLAG)$(CFG)
LIBDIRNAME = .\..\..\..\..\lib\wat_$(LIBTYPE_SUFFIX)$(CFG)
SETUPHDIR = &
	$(LIBDIRNAME)\$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)
STUDIO_CXXFLAGS = $(__DEBUGINFO_0) $(__OPTIMIZEFLAG_2) $(__THREADSFLAG_5) &
	$(__RUNTIME_LIBS_6) -d__WXPM__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) &
	$(__NDEBUG_DEFINE_p) $(__EXCEPTIONS_DEFINE_p) $(__RTTI_DEFINE_p) &
	$(__THREAD_DEFINE_p) $(__UNICODE_DEFINE_p) $(__GFXCTX_DEFINE_p) &
	-i=$(SETUPHDIR) -i=.\..\..\..\..\include &
	$(____CAIRO_INCLUDEDIR_FILENAMES_p) -wx -wcd=549 -wcd=656 -wcd=657 -wcd=667 &
	-i=. $(__DLLFLAG_p) -i=.\..\..\..\..\samples -dNOPCH -i=.\..\..\..\include &
	$(__RTTIFLAG_7) $(__EXCEPTIONSFLAG_8) $(CPPFLAGS) $(CXXFLAGS)
STUDIO_OBJECTS =  &
	$(OBJS)\studio_studio.obj &
	$(OBJS)\studio_doc.obj &
	$(OBJS)\studio_shapes.obj &
	$(OBJS)\studio_symbols.obj &
	$(OBJS)\studio_view.obj &
	$(OBJS)\studio_cspalette.obj &
	$(OBJS)\studio_mainfrm.obj &
	$(OBJS)\studio_project.obj &
	$(OBJS)\studio_dialogs.obj &
	$(OBJS)\studio_csprint.obj


all : $(OBJS)
$(OBJS) :
	-if not exist $(OBJS) mkdir $(OBJS)

### Targets: ###

all : .SYMBOLIC $(OBJS)\studio.exe data

clean : .SYMBOLIC 
	-if exist $(OBJS)\*.obj del $(OBJS)\*.obj
	-if exist $(OBJS)\*.res del $(OBJS)\*.res
	-if exist $(OBJS)\*.lbc del $(OBJS)\*.lbc
	-if exist $(OBJS)\*.ilk del $(OBJS)\*.ilk
	-if exist $(OBJS)\*.pch del $(OBJS)\*.pch
	-del $(OBJS)\studio.exe

$(OBJS)\studio.exe :  $(STUDIO_OBJECTS)
	@%create $(OBJS)\studio.lbc
	@%append $(OBJS)\studio.lbc option quiet
	@%append $(OBJS)\studio.lbc name $^@
	@%append $(OBJS)\studio.lbc option caseexact
	@%append $(OBJS)\studio.lbc  $(__DEBUGINFO_1)  libpath $(LIBDIRNAME) system os2v2_pm $(LDFLAGS)
	@for %i in ($(STUDIO_OBJECTS)) do @%append $(OBJS)\studio.lbc file %i
	@for %i in ( wx$(PORTNAME)$(WXUNIVNAME)$(WX_RELEASE_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)_ogl.lib  wx$(PORTNAME)$(WXUNIVNAME)$(WX_RELEASE_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)_deprecated.lib  $(__WXLIB_ADV_p)  $(__WXLIB_HTML_p)  $(__WXLIB_XML_p)  $(__WXLIB_CORE_p)  $(__WXLIB_BASE_p)  $(__WXLIB_MONO_p) $(__LIB_TIFF_p) $(__LIB_JPEG_p) $(__LIB_PNG_p)  wxzlib$(WXDEBUGFLAG).lib  wxregex$(WXUNICODEFLAG)$(WXDEBUGFLAG).lib wxexpat$(WXDEBUGFLAG).lib $(EXTRALIBS_FOR_BASE) upm32.lib) do @%append $(OBJS)\studio.lbc library %i
	@%append $(OBJS)\studio.lbc
	@for %i in () do @%append $(OBJS)\studio.lbc option stack=%i
	wlink @$(OBJS)\studio.lbc

data : .SYMBOLIC 
	if not exist $(OBJS) mkdir $(OBJS)
	for %f in (studio_resources.wxr) do if not exist $(OBJS)\%f copy .\%f $(OBJS)

$(OBJS)\studio_studio.obj :  .AUTODEPEND .\studio.cpp
	$(CXX) -bt=os2 -zq -fo=$^@ $(STUDIO_CXXFLAGS) $<

$(OBJS)\studio_doc.obj :  .AUTODEPEND .\doc.cpp
	$(CXX) -bt=os2 -zq -fo=$^@ $(STUDIO_CXXFLAGS) $<

$(OBJS)\studio_shapes.obj :  .AUTODEPEND .\shapes.cpp
	$(CXX) -bt=os2 -zq -fo=$^@ $(STUDIO_CXXFLAGS) $<

$(OBJS)\studio_symbols.obj :  .AUTODEPEND .\symbols.cpp
	$(CXX) -bt=os2 -zq -fo=$^@ $(STUDIO_CXXFLAGS) $<

$(OBJS)\studio_view.obj :  .AUTODEPEND .\view.cpp
	$(CXX) -bt=os2 -zq -fo=$^@ $(STUDIO_CXXFLAGS) $<

$(OBJS)\studio_cspalette.obj :  .AUTODEPEND .\cspalette.cpp
	$(CXX) -bt=os2 -zq -fo=$^@ $(STUDIO_CXXFLAGS) $<

$(OBJS)\studio_mainfrm.obj :  .AUTODEPEND .\mainfrm.cpp
	$(CXX) -bt=os2 -zq -fo=$^@ $(STUDIO_CXXFLAGS) $<

$(OBJS)\studio_project.obj :  .AUTODEPEND .\project.cpp
	$(CXX) -bt=os2 -zq -fo=$^@ $(STUDIO_CXXFLAGS) $<

$(OBJS)\studio_dialogs.obj :  .AUTODEPEND .\dialogs.cpp
	$(CXX) -bt=os2 -zq -fo=$^@ $(STUDIO_CXXFLAGS) $<

$(OBJS)\studio_csprint.obj :  .AUTODEPEND .\csprint.cpp
	$(CXX) -bt=os2 -zq -fo=$^@ $(STUDIO_CXXFLAGS) $<

