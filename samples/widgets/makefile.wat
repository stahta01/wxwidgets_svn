# =========================================================================
#     This makefile was generated by
#     Bakefile 0.2.8 (http://www.bakefile.org)
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

PORTNAME =
!ifeq USE_GUI 0
PORTNAME = base
!endif
!ifeq USE_GUI 1
PORTNAME = msw
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
__WXLIB_ADV_p =
!ifeq MONOLITHIC 0
__WXLIB_ADV_p = &
	wx$(PORTNAME)$(WXUNIVNAME)$(WX_RELEASE_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)_adv.lib
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
	$(COMPILER_PREFIX)_$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WXDLLFLAG)$(CFG)
LIBDIRNAME = .\..\..\lib\$(COMPILER_PREFIX)_$(LIBTYPE_SUFFIX)$(CFG)
SETUPHDIR = &
	$(LIBDIRNAME)\$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)
WIDGETS_CXXFLAGS = $(__DEBUGINFO_0) $(__OPTIMIZEFLAG_2) $(__THREADSFLAG_5) &
	$(__RUNTIME_LIBS_6) -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) &
	$(__NDEBUG_DEFINE_p) $(__EXCEPTIONS_DEFINE_p) $(__RTTI_DEFINE_p) &
	$(__THREAD_DEFINE_p) $(__UNICODE_DEFINE_p) -i=$(SETUPHDIR) &
	-i=.\..\..\include $(____CAIRO_INCLUDEDIR_FILENAMES) -wx -wcd=549 -wcd=656 &
	-wcd=657 -wcd=667 -i=. $(__DLLFLAG_p) -i=.\..\..\samples -dNOPCH &
	$(__RTTIFLAG_7) $(__EXCEPTIONSFLAG_8) $(CPPFLAGS) $(CXXFLAGS)
WIDGETS_OBJECTS =  &
	$(OBJS)\widgets_bmpcombobox.obj &
	$(OBJS)\widgets_button.obj &
	$(OBJS)\widgets_checkbox.obj &
	$(OBJS)\widgets_choice.obj &
	$(OBJS)\widgets_clrpicker.obj &
	$(OBJS)\widgets_combobox.obj &
	$(OBJS)\widgets_datepick.obj &
	$(OBJS)\widgets_dirctrl.obj &
	$(OBJS)\widgets_dirpicker.obj &
	$(OBJS)\widgets_editlbox.obj &
	$(OBJS)\widgets_filectrl.obj &
	$(OBJS)\widgets_filepicker.obj &
	$(OBJS)\widgets_fontpicker.obj &
	$(OBJS)\widgets_gauge.obj &
	$(OBJS)\widgets_hyperlnk.obj &
	$(OBJS)\widgets_itemcontainer.obj &
	$(OBJS)\widgets_listbox.obj &
	$(OBJS)\widgets_notebook.obj &
	$(OBJS)\widgets_odcombobox.obj &
	$(OBJS)\widgets_radiobox.obj &
	$(OBJS)\widgets_searchctrl.obj &
	$(OBJS)\widgets_slider.obj &
	$(OBJS)\widgets_spinbtn.obj &
	$(OBJS)\widgets_static.obj &
	$(OBJS)\widgets_statbmp.obj &
	$(OBJS)\widgets_textctrl.obj &
	$(OBJS)\widgets_toggle.obj &
	$(OBJS)\widgets_widgets.obj


all : $(OBJS)
$(OBJS) :
	-if not exist $(OBJS) mkdir $(OBJS)

### Targets: ###

all : .SYMBOLIC $(OBJS)\widgets.exe

clean : .SYMBOLIC 
	-if exist $(OBJS)\*.obj del $(OBJS)\*.obj
	-if exist $(OBJS)\*.res del $(OBJS)\*.res
	-if exist $(OBJS)\*.lbc del $(OBJS)\*.lbc
	-if exist $(OBJS)\*.ilk del $(OBJS)\*.ilk
	-if exist $(OBJS)\*.pch del $(OBJS)\*.pch
	-if exist $(OBJS)\widgets.exe del $(OBJS)\widgets.exe

$(OBJS)\widgets.exe :  $(WIDGETS_OBJECTS) $(OBJS)\widgets_sample.res
	@%create $(OBJS)\widgets.lbc
	@%append $(OBJS)\widgets.lbc option quiet
	@%append $(OBJS)\widgets.lbc name $^@
	@%append $(OBJS)\widgets.lbc option caseexact
	@%append $(OBJS)\widgets.lbc  $(__DEBUGINFO_1)  libpath $(LIBDIRNAME) system nt_win ref '_WinMain@16' $(____CAIRO_LIBDIR_FILENAMES_p) $(LDFLAGS)
	@for %i in ($(WIDGETS_OBJECTS)) do @%append $(OBJS)\widgets.lbc file %i
	@for %i in ( $(__WXLIB_ADV_p)  $(__WXLIB_CORE_p)  $(__WXLIB_BASE_p)  $(__WXLIB_MONO_p) $(__LIB_TIFF_p) $(__LIB_JPEG_p) $(__LIB_PNG_p)  wxzlib$(WXDEBUGFLAG).lib wxregex$(WXUNICODEFLAG)$(WXDEBUGFLAG).lib wxexpat$(WXDEBUGFLAG).lib $(EXTRALIBS_FOR_BASE)  $(__CAIRO_LIB_p) kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib wininet.lib) do @%append $(OBJS)\widgets.lbc library %i
	@%append $(OBJS)\widgets.lbc option resource=$(OBJS)\widgets_sample.res
	@for %i in () do @%append $(OBJS)\widgets.lbc option stack=%i
	wlink @$(OBJS)\widgets.lbc

$(OBJS)\widgets_bmpcombobox.obj :  .AUTODEPEND .\bmpcombobox.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WIDGETS_CXXFLAGS) $<

$(OBJS)\widgets_button.obj :  .AUTODEPEND .\button.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WIDGETS_CXXFLAGS) $<

$(OBJS)\widgets_checkbox.obj :  .AUTODEPEND .\checkbox.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WIDGETS_CXXFLAGS) $<

$(OBJS)\widgets_choice.obj :  .AUTODEPEND .\choice.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WIDGETS_CXXFLAGS) $<

$(OBJS)\widgets_clrpicker.obj :  .AUTODEPEND .\clrpicker.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WIDGETS_CXXFLAGS) $<

$(OBJS)\widgets_combobox.obj :  .AUTODEPEND .\combobox.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WIDGETS_CXXFLAGS) $<

$(OBJS)\widgets_datepick.obj :  .AUTODEPEND .\datepick.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WIDGETS_CXXFLAGS) $<

$(OBJS)\widgets_dirctrl.obj :  .AUTODEPEND .\dirctrl.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WIDGETS_CXXFLAGS) $<

$(OBJS)\widgets_dirpicker.obj :  .AUTODEPEND .\dirpicker.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WIDGETS_CXXFLAGS) $<

$(OBJS)\widgets_editlbox.obj :  .AUTODEPEND .\editlbox.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WIDGETS_CXXFLAGS) $<

$(OBJS)\widgets_filectrl.obj :  .AUTODEPEND .\filectrl.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WIDGETS_CXXFLAGS) $<

$(OBJS)\widgets_filepicker.obj :  .AUTODEPEND .\filepicker.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WIDGETS_CXXFLAGS) $<

$(OBJS)\widgets_fontpicker.obj :  .AUTODEPEND .\fontpicker.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WIDGETS_CXXFLAGS) $<

$(OBJS)\widgets_gauge.obj :  .AUTODEPEND .\gauge.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WIDGETS_CXXFLAGS) $<

$(OBJS)\widgets_hyperlnk.obj :  .AUTODEPEND .\hyperlnk.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WIDGETS_CXXFLAGS) $<

$(OBJS)\widgets_itemcontainer.obj :  .AUTODEPEND .\itemcontainer.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WIDGETS_CXXFLAGS) $<

$(OBJS)\widgets_listbox.obj :  .AUTODEPEND .\listbox.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WIDGETS_CXXFLAGS) $<

$(OBJS)\widgets_notebook.obj :  .AUTODEPEND .\notebook.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WIDGETS_CXXFLAGS) $<

$(OBJS)\widgets_odcombobox.obj :  .AUTODEPEND .\odcombobox.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WIDGETS_CXXFLAGS) $<

$(OBJS)\widgets_radiobox.obj :  .AUTODEPEND .\radiobox.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WIDGETS_CXXFLAGS) $<

$(OBJS)\widgets_searchctrl.obj :  .AUTODEPEND .\searchctrl.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WIDGETS_CXXFLAGS) $<

$(OBJS)\widgets_slider.obj :  .AUTODEPEND .\slider.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WIDGETS_CXXFLAGS) $<

$(OBJS)\widgets_spinbtn.obj :  .AUTODEPEND .\spinbtn.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WIDGETS_CXXFLAGS) $<

$(OBJS)\widgets_static.obj :  .AUTODEPEND .\static.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WIDGETS_CXXFLAGS) $<

$(OBJS)\widgets_statbmp.obj :  .AUTODEPEND .\statbmp.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WIDGETS_CXXFLAGS) $<

$(OBJS)\widgets_textctrl.obj :  .AUTODEPEND .\textctrl.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WIDGETS_CXXFLAGS) $<

$(OBJS)\widgets_toggle.obj :  .AUTODEPEND .\toggle.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WIDGETS_CXXFLAGS) $<

$(OBJS)\widgets_widgets.obj :  .AUTODEPEND .\widgets.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WIDGETS_CXXFLAGS) $<

$(OBJS)\widgets_sample.res :  .AUTODEPEND .\..\sample.rc
	wrc -q -ad -bt=nt -r -fo=$^@    -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) $(__NDEBUG_DEFINE_p) $(__EXCEPTIONS_DEFINE_p) $(__RTTI_DEFINE_p) $(__THREAD_DEFINE_p) $(__UNICODE_DEFINE_p)  -i=$(SETUPHDIR) -i=.\..\..\include $(____CAIRO_INCLUDEDIR_FILENAMES) -i=. $(__DLLFLAG_p) -i=.\..\..\samples -dNOPCH $<

