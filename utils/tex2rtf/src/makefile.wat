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
__tex2rtf_gui___depname =
!ifeq USE_GUI 1
__tex2rtf_gui___depname = $(OBJS)\tex2rtf_gui.exe
!endif
__WXLIB_HTML_p =
!ifeq MONOLITHIC 0
__WXLIB_HTML_p = &
	wx$(PORTNAME)$(WXUNIVNAME)$(WX_RELEASE_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)_html.lib
!endif
__WXLIB_CORE_p =
!ifeq MONOLITHIC 0
__WXLIB_CORE_p = &
	wx$(PORTNAME)$(WXUNIVNAME)$(WX_RELEASE_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)_core.lib
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
__DLLFLAG_p =
!ifeq SHARED 1
__DLLFLAG_p = -dWXUSINGDLL
!endif

### Variables: ###

WX_RELEASE_NODOT = 27
OBJS = &
	wat_$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WXDLLFLAG)$(CFG)
LIBDIRNAME = .\..\..\..\lib\wat_$(LIBTYPE_SUFFIX)$(CFG)
SETUPHDIR = &
	$(LIBDIRNAME)\$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)
TEX2RTF_CXXFLAGS = $(__DEBUGINFO) $(__OPTIMIZEFLAG) $(__THREADSFLAG) &
	$(__RUNTIME_LIBS) -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) &
	$(__EXCEPTIONS_DEFINE_p) $(__RTTI_DEFINE_p) $(__THREAD_DEFINE_p) &
	$(__UNICODE_DEFINE_p) -i=$(SETUPHDIR) -i=.\..\..\..\include -wx -wcd=549 &
	-wcd=656 -wcd=657 -wcd=667 -i=. $(__DLLFLAG_p) -dwxUSE_GUI=0 -dNO_GUI &
	$(__RTTIFLAG) $(__EXCEPTIONSFLAG) $(CPPFLAGS) $(CXXFLAGS)
TEX2RTF_OBJECTS =  &
	$(OBJS)\tex2rtf_tex2rtf.obj &
	$(OBJS)\tex2rtf_tex2any.obj &
	$(OBJS)\tex2rtf_texutils.obj &
	$(OBJS)\tex2rtf_rtfutils.obj &
	$(OBJS)\tex2rtf_xlputils.obj &
	$(OBJS)\tex2rtf_htmlutil.obj &
	$(OBJS)\tex2rtf_readshg.obj &
	$(OBJS)\tex2rtf_table.obj
TEX2RTF_GUI_CXXFLAGS = $(__DEBUGINFO) $(__OPTIMIZEFLAG) $(__THREADSFLAG) &
	$(__RUNTIME_LIBS) -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) &
	$(__EXCEPTIONS_DEFINE_p) $(__RTTI_DEFINE_p) $(__THREAD_DEFINE_p) &
	$(__UNICODE_DEFINE_p) -i=$(SETUPHDIR) -i=.\..\..\..\include -wx -wcd=549 &
	-wcd=656 -wcd=657 -wcd=667 -i=. $(__DLLFLAG_p) -i=.\..\..\..\samples -dNOPCH &
	$(__RTTIFLAG) $(__EXCEPTIONSFLAG) $(CPPFLAGS) $(CXXFLAGS)
TEX2RTF_GUI_OBJECTS =  &
	$(OBJS)\tex2rtf_gui_tex2rtf.obj &
	$(OBJS)\tex2rtf_gui_tex2any.obj &
	$(OBJS)\tex2rtf_gui_texutils.obj &
	$(OBJS)\tex2rtf_gui_rtfutils.obj &
	$(OBJS)\tex2rtf_gui_xlputils.obj &
	$(OBJS)\tex2rtf_gui_htmlutil.obj &
	$(OBJS)\tex2rtf_gui_readshg.obj &
	$(OBJS)\tex2rtf_gui_table.obj


all : $(OBJS)
$(OBJS) :
	-if not exist $(OBJS) mkdir $(OBJS)

### Targets: ###

all : .SYMBOLIC $(OBJS)\tex2rtf.exe $(__tex2rtf_gui___depname)

clean : .SYMBOLIC 
	-if exist $(OBJS)\*.obj del $(OBJS)\*.obj
	-if exist $(OBJS)\*.res del $(OBJS)\*.res
	-if exist $(OBJS)\*.lbc del $(OBJS)\*.lbc
	-if exist $(OBJS)\*.ilk del $(OBJS)\*.ilk
	-if exist $(OBJS)\*.pch del $(OBJS)\*.pch
	-if exist $(OBJS)\tex2rtf.exe del $(OBJS)\tex2rtf.exe
	-if exist $(OBJS)\tex2rtf_gui.exe del $(OBJS)\tex2rtf_gui.exe

$(OBJS)\tex2rtf.exe :  $(TEX2RTF_OBJECTS)
	@%create $(OBJS)\tex2rtf.lbc
	@%append $(OBJS)\tex2rtf.lbc option quiet
	@%append $(OBJS)\tex2rtf.lbc name $^@
	@%append $(OBJS)\tex2rtf.lbc option caseexact
	@%append $(OBJS)\tex2rtf.lbc $(LDFLAGS) $(__DEBUGINFO_1)  libpath $(LIBDIRNAME) system nt ref 'main_'
	@for %i in ($(TEX2RTF_OBJECTS)) do @%append $(OBJS)\tex2rtf.lbc file %i
	@for %i in ( $(__WXLIB_BASE_p)  $(__WXLIB_MONO_p) $(__LIB_TIFF_p) $(__LIB_JPEG_p) $(__LIB_PNG_p) wxzlib$(WXDEBUGFLAG).lib  wxregex$(WXUNICODEFLAG)$(WXDEBUGFLAG).lib wxexpat$(WXDEBUGFLAG).lib $(EXTRALIBS_FOR_BASE)  kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib) do @%append $(OBJS)\tex2rtf.lbc library %i
	@%append $(OBJS)\tex2rtf.lbc
	@for %i in () do @%append $(OBJS)\tex2rtf.lbc option stack=%i
	wlink @$(OBJS)\tex2rtf.lbc

!ifeq USE_GUI 1
$(OBJS)\tex2rtf_gui.exe :  $(TEX2RTF_GUI_OBJECTS) $(OBJS)\tex2rtf_gui_sample.res
	@%create $(OBJS)\tex2rtf_gui.lbc
	@%append $(OBJS)\tex2rtf_gui.lbc option quiet
	@%append $(OBJS)\tex2rtf_gui.lbc name $^@
	@%append $(OBJS)\tex2rtf_gui.lbc option caseexact
	@%append $(OBJS)\tex2rtf_gui.lbc $(LDFLAGS) $(__DEBUGINFO_1)  libpath $(LIBDIRNAME) system nt_win ref '_WinMain@16'
	@for %i in ($(TEX2RTF_GUI_OBJECTS)) do @%append $(OBJS)\tex2rtf_gui.lbc file %i
	@for %i in ( $(__WXLIB_HTML_p)  $(__WXLIB_CORE_p)  $(__WXLIB_BASE_p)  $(__WXLIB_MONO_p) $(__LIB_TIFF_p) $(__LIB_JPEG_p) $(__LIB_PNG_p) wxzlib$(WXDEBUGFLAG).lib  wxregex$(WXUNICODEFLAG)$(WXDEBUGFLAG).lib wxexpat$(WXDEBUGFLAG).lib $(EXTRALIBS_FOR_BASE)  kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib ) do @%append $(OBJS)\tex2rtf_gui.lbc library %i
	@%append $(OBJS)\tex2rtf_gui.lbc option resource=$(OBJS)\tex2rtf_gui_sample.res
	@for %i in () do @%append $(OBJS)\tex2rtf_gui.lbc option stack=%i
	wlink @$(OBJS)\tex2rtf_gui.lbc
!endif

$(OBJS)\tex2rtf_tex2rtf.obj :  .AUTODEPEND .\tex2rtf.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEX2RTF_CXXFLAGS) $<

$(OBJS)\tex2rtf_tex2any.obj :  .AUTODEPEND .\tex2any.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEX2RTF_CXXFLAGS) $<

$(OBJS)\tex2rtf_texutils.obj :  .AUTODEPEND .\texutils.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEX2RTF_CXXFLAGS) $<

$(OBJS)\tex2rtf_rtfutils.obj :  .AUTODEPEND .\rtfutils.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEX2RTF_CXXFLAGS) $<

$(OBJS)\tex2rtf_xlputils.obj :  .AUTODEPEND .\xlputils.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEX2RTF_CXXFLAGS) $<

$(OBJS)\tex2rtf_htmlutil.obj :  .AUTODEPEND .\htmlutil.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEX2RTF_CXXFLAGS) $<

$(OBJS)\tex2rtf_readshg.obj :  .AUTODEPEND .\readshg.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEX2RTF_CXXFLAGS) $<

$(OBJS)\tex2rtf_table.obj :  .AUTODEPEND .\table.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEX2RTF_CXXFLAGS) $<

$(OBJS)\tex2rtf_gui_sample.res :  .AUTODEPEND .\..\..\..\samples\sample.rc
	wrc -q -ad -bt=nt -r -fo=$^@   -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) $(__EXCEPTIONS_DEFINE_p) $(__RTTI_DEFINE_p) $(__THREAD_DEFINE_p) $(__UNICODE_DEFINE_p)  -i=$(SETUPHDIR) -i=.\..\..\..\include -i=. $(__DLLFLAG_p) -i=.\..\..\..\samples $<

$(OBJS)\tex2rtf_gui_tex2rtf.obj :  .AUTODEPEND .\tex2rtf.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEX2RTF_GUI_CXXFLAGS) $<

$(OBJS)\tex2rtf_gui_tex2any.obj :  .AUTODEPEND .\tex2any.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEX2RTF_GUI_CXXFLAGS) $<

$(OBJS)\tex2rtf_gui_texutils.obj :  .AUTODEPEND .\texutils.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEX2RTF_GUI_CXXFLAGS) $<

$(OBJS)\tex2rtf_gui_rtfutils.obj :  .AUTODEPEND .\rtfutils.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEX2RTF_GUI_CXXFLAGS) $<

$(OBJS)\tex2rtf_gui_xlputils.obj :  .AUTODEPEND .\xlputils.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEX2RTF_GUI_CXXFLAGS) $<

$(OBJS)\tex2rtf_gui_htmlutil.obj :  .AUTODEPEND .\htmlutil.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEX2RTF_GUI_CXXFLAGS) $<

$(OBJS)\tex2rtf_gui_readshg.obj :  .AUTODEPEND .\readshg.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEX2RTF_GUI_CXXFLAGS) $<

$(OBJS)\tex2rtf_gui_table.obj :  .AUTODEPEND .\table.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEX2RTF_GUI_CXXFLAGS) $<

