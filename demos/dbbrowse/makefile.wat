# =========================================================================
#     This makefile was generated by
#     Bakefile 0.1.3 (http://bakefile.sourceforge.net)
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
__RUNTIME_LIBS_5 =
!ifeq RUNTIME_LIBS dynamic
__RUNTIME_LIBS_5 = -br
!endif
!ifeq RUNTIME_LIBS static
__RUNTIME_LIBS_5 = 
!endif
__EXCEPTIONSFLAG_7 =
!ifeq USE_EXCEPTIONS 0
__EXCEPTIONSFLAG_7 = 
!endif
!ifeq USE_EXCEPTIONS 1
__EXCEPTIONSFLAG_7 = -xs
!endif
__WXLIB_DBGRID_p =
!ifeq MONOLITHIC 0
__WXLIB_DBGRID_p = &
	wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_dbgrid.lib
!endif
__WXLIB_ADV_p =
!ifeq MONOLITHIC 0
__WXLIB_ADV_p = &
	wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_adv.lib
!endif
__WXLIB_ODBC_p =
!ifeq MONOLITHIC 0
!ifeq USE_ODBC 1
__WXLIB_ODBC_p = wxbase25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_odbc.lib
!endif
!endif
__WXLIB_HTML_p =
!ifeq MONOLITHIC 0
__WXLIB_HTML_p = &
	wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_html.lib
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
LIBDIRNAME =
!ifeq SHARED 0
LIBDIRNAME = .\..\..\lib\wat_lib$(CFG)
!endif
!ifeq SHARED 1
LIBDIRNAME = .\..\..\lib\wat_dll$(CFG)
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
__DLLFLAG_p =
!ifeq SHARED 1
__DLLFLAG_p = -dWXUSINGDLL
!endif

### Variables: ###

OBJS = &
	wat_$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WXDLLFLAG)$(CFG)
SETUPHDIR = &
	$(LIBDIRNAME)\$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)
DBBROWSE_CXXFLAGS = $(__DEBUGINFO_0) $(__OPTIMIZEFLAG_2) -bm &
	$(__RUNTIME_LIBS_5) -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) &
	$(__UNICODE_DEFINE_p) -i=.\..\..\include -i=$(SETUPHDIR) -i=. $(__DLLFLAG_p) &
	-i=.\..\..\samples $(__EXCEPTIONSFLAG_7) $(CPPFLAGS) $(CXXFLAGS)
DBBROWSE_OBJECTS =  &
	$(OBJS)\dbbrowse_dbbrowse.obj &
	$(OBJS)\dbbrowse_doc.obj &
	$(OBJS)\dbbrowse_pgmctrl.obj &
	$(OBJS)\dbbrowse_tabpgwin.obj &
	$(OBJS)\dbbrowse_browsedb.obj &
	$(OBJS)\dbbrowse_dbtree.obj &
	$(OBJS)\dbbrowse_dbgrid.obj &
	$(OBJS)\dbbrowse_dlguser.obj


all : $(OBJS)
$(OBJS) :
	-if not exist $(OBJS) mkdir $(OBJS)

### Targets: ###

all : .SYMBOLIC $(OBJS)\dbbrowse.exe data_help_std data_help_png

clean : .SYMBOLIC 
	-if exist $(OBJS)\*.obj del $(OBJS)\*.obj
	-if exist $(OBJS)\*.res del $(OBJS)\*.res
	-if exist $(OBJS)\*.lbc del $(OBJS)\*.lbc
	-if exist $(OBJS)\*.ilk del $(OBJS)\*.ilk
	-if exist $(OBJS)\dbbrowse.exe del $(OBJS)\dbbrowse.exe

$(OBJS)\dbbrowse.exe :  $(DBBROWSE_OBJECTS) $(OBJS)\dbbrowse_dbbrowse.res
	@%create $(OBJS)\dbbrowse.lbc
	@%append $(OBJS)\dbbrowse.lbc option quiet
	@%append $(OBJS)\dbbrowse.lbc name $^@
	@%append $(OBJS)\dbbrowse.lbc option caseexact
	@%append $(OBJS)\dbbrowse.lbc $(LDFLAGS) $(__DEBUGINFO_1)  libpath $(LIBDIRNAME) system nt_win ref '_WinMain@16'
	@for %i in ($(DBBROWSE_OBJECTS)) do @%append $(OBJS)\dbbrowse.lbc file %i
	@for %i in ( $(__WXLIB_DBGRID_p)  $(__WXLIB_ADV_p)  $(__WXLIB_ODBC_p)  $(__WXLIB_HTML_p)  $(__WXLIB_CORE_p)  $(__WXLIB_BASE_p)  $(__WXLIB_MONO_p) $(__LIB_TIFF_p) $(__LIB_JPEG_p) $(__LIB_PNG_p) wxzlib$(WXDEBUGFLAG).lib  wxregex$(WXUNICODEFLAG)$(WXDEBUGFLAG).lib wxexpat$(WXDEBUGFLAG).lib   kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib ) do @%append $(OBJS)\dbbrowse.lbc library %i
	@%append $(OBJS)\dbbrowse.lbc option resource=$(OBJS)\dbbrowse_dbbrowse.res
	wlink @$(OBJS)\dbbrowse.lbc

data_help_std : .SYMBOLIC 
	if not exist $(OBJS)\help.std mkdir $(OBJS)\help.std
	for %f in (compiler.htm dbbrowse.hhc dbbrowse.hhk dbbrowse.hhp dbbrowse.htm getcol.htm getcolsh.htm icons.htm picgrams.htm problems.htm remstar.htm sample.htm startprm.htm todo.htm wxdb.htm wxtable.htm) do if not exist $(OBJS)\help.std\%f copy .\help.std\%f $(OBJS)\help.std

data_help_png : .SYMBOLIC 
	if not exist $(OBJS)\help.png mkdir $(OBJS)\help.png
	for %f in (col.png d_closed.png d_open.png dsn.png dsnclose.png dsnopen.png f_closed.png f_open.png gnu.png gtk.png imbau.png kde.png key.png keyf.png linux.png logo.png motif.png msvc.png odbc.png pgmctrl.png redh.png remstar.png server.png suse.png tab.png view.png winnt.png wins.png) do if not exist $(OBJS)\help.png\%f copy .\help.png\%f $(OBJS)\help.png

$(OBJS)\dbbrowse_dbbrowse.obj :  .AUTODEPEND .\dbbrowse.cpp
	$(CXX) -zq -fo=$^@ $(DBBROWSE_CXXFLAGS) $<

$(OBJS)\dbbrowse_doc.obj :  .AUTODEPEND .\doc.cpp
	$(CXX) -zq -fo=$^@ $(DBBROWSE_CXXFLAGS) $<

$(OBJS)\dbbrowse_pgmctrl.obj :  .AUTODEPEND .\pgmctrl.cpp
	$(CXX) -zq -fo=$^@ $(DBBROWSE_CXXFLAGS) $<

$(OBJS)\dbbrowse_tabpgwin.obj :  .AUTODEPEND .\tabpgwin.cpp
	$(CXX) -zq -fo=$^@ $(DBBROWSE_CXXFLAGS) $<

$(OBJS)\dbbrowse_browsedb.obj :  .AUTODEPEND .\browsedb.cpp
	$(CXX) -zq -fo=$^@ $(DBBROWSE_CXXFLAGS) $<

$(OBJS)\dbbrowse_dbtree.obj :  .AUTODEPEND .\dbtree.cpp
	$(CXX) -zq -fo=$^@ $(DBBROWSE_CXXFLAGS) $<

$(OBJS)\dbbrowse_dbgrid.obj :  .AUTODEPEND .\dbgrid.cpp
	$(CXX) -zq -fo=$^@ $(DBBROWSE_CXXFLAGS) $<

$(OBJS)\dbbrowse_dlguser.obj :  .AUTODEPEND .\dlguser.cpp
	$(CXX) -zq -fo=$^@ $(DBBROWSE_CXXFLAGS) $<

$(OBJS)\dbbrowse_dbbrowse.res :  .AUTODEPEND .\dbbrowse.rc
	wrc -q -ad -bt=nt -r -fo=$^@  -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) $(__UNICODE_DEFINE_p) -i=.\..\..\include -i=$(SETUPHDIR) -i=. $(__DLLFLAG_p) -i=.\..\..\samples $<
