# =========================================================================
#     This makefile was generated by
#     Bakefile 0.2.0 (http://bakefile.sourceforge.net)
#     Do not modify, all changes will be overwritten!
# =========================================================================

!include ../build/msw/config.wat

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
__WXLIB_NET_p =
!ifeq MONOLITHIC 0
__WXLIB_NET_p = &
	wxbase$(WX_RELEASE_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)_net.lib
!endif
__test_gui___depname =
!ifeq USE_GUI 1
__test_gui___depname = $(OBJS)\test_gui.exe
!endif
__WXLIB_CORE_p =
!ifeq MONOLITHIC 0
__WXLIB_CORE_p = &
	wx$(PORTNAME)$(WXUNIVNAME)$(WX_RELEASE_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)_core.lib
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
LIBDIRNAME = .\..\lib\wat_$(LIBTYPE_SUFFIX)$(CFG)
SETUPHDIR = &
	$(LIBDIRNAME)\$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)
TEST_CXXFLAGS = $(__DEBUGINFO) $(__OPTIMIZEFLAG) $(__THREADSFLAG) &
	$(__RUNTIME_LIBS) -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) &
	$(__EXCEPTIONS_DEFINE_p) $(__RTTI_DEFINE_p) $(__THREAD_DEFINE_p) &
	$(__UNICODE_DEFINE_p) -i=$(SETUPHDIR) -i=.\..\include -wx -wcd=549 -wcd=656 &
	-wcd=657 -wcd=667 -i=. $(__DLLFLAG_p) -dwxUSE_GUI=0 $(CPPUNIT_CFLAGS) &
	/fh=$(OBJS)\testprec_test.pch $(__RTTIFLAG) $(__EXCEPTIONSFLAG) $(CPPFLAGS) &
	$(CXXFLAGS)
TEST_OBJECTS =  &
	$(OBJS)\test_dummy.obj &
	$(OBJS)\test_test.obj &
	$(OBJS)\test_archivetest.obj &
	$(OBJS)\test_ziptest.obj &
	$(OBJS)\test_arrays.obj &
	$(OBJS)\test_datetimetest.obj &
	$(OBJS)\test_fileconftest.obj &
	$(OBJS)\test_filekind.obj &
	$(OBJS)\test_filenametest.obj &
	$(OBJS)\test_filesystest.obj &
	$(OBJS)\test_fontmaptest.obj &
	$(OBJS)\test_formatconvertertest.obj &
	$(OBJS)\test_hashes.obj &
	$(OBJS)\test_lists.obj &
	$(OBJS)\test_longlongtest.obj &
	$(OBJS)\test_convautotest.obj &
	$(OBJS)\test_mbconvtest.obj &
	$(OBJS)\test_regextest.obj &
	$(OBJS)\test_wxregextest.obj &
	$(OBJS)\test_scopeguardtest.obj &
	$(OBJS)\test_strings.obj &
	$(OBJS)\test_stdstrings.obj &
	$(OBJS)\test_tokenizer.obj &
	$(OBJS)\test_unicode.obj &
	$(OBJS)\test_crt.obj &
	$(OBJS)\test_vsnprintf.obj &
	$(OBJS)\test_bstream.obj &
	$(OBJS)\test_datastreamtest.obj &
	$(OBJS)\test_ffilestream.obj &
	$(OBJS)\test_filestream.obj &
	$(OBJS)\test_largefile.obj &
	$(OBJS)\test_memstream.obj &
	$(OBJS)\test_sstream.obj &
	$(OBJS)\test_tempfile.obj &
	$(OBJS)\test_textstreamtest.obj &
	$(OBJS)\test_zlibstream.obj &
	$(OBJS)\test_textfiletest.obj &
	$(OBJS)\test_uris.obj
TEST_GUI_CXXFLAGS = $(__DEBUGINFO) $(__OPTIMIZEFLAG) $(__THREADSFLAG) &
	$(__RUNTIME_LIBS) -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) &
	$(__EXCEPTIONS_DEFINE_p) $(__RTTI_DEFINE_p) $(__THREAD_DEFINE_p) &
	$(__UNICODE_DEFINE_p) -i=$(SETUPHDIR) -i=.\..\include -wx -wcd=549 -wcd=656 &
	-wcd=657 -wcd=667 -i=. $(__DLLFLAG_p) -i=.\..\samples -dNOPCH &
	$(CPPUNIT_CFLAGS) /fh=$(OBJS)\testprec_test_gui.pch $(__RTTIFLAG) &
	$(__EXCEPTIONSFLAG) $(CPPFLAGS) $(CXXFLAGS)
TEST_GUI_OBJECTS =  &
	$(OBJS)\test_gui_dummy.obj &
	$(OBJS)\test_gui_test.obj &
	$(OBJS)\test_gui_rect.obj &
	$(OBJS)\test_gui_size.obj &
	$(OBJS)\test_gui_point.obj


all : $(OBJS)
$(OBJS) :
	-if not exist $(OBJS) mkdir $(OBJS)

### Targets: ###

all : .SYMBOLIC $(OBJS)\test.exe $(__test_gui___depname) data

clean : .SYMBOLIC 
	-if exist $(OBJS)\*.obj del $(OBJS)\*.obj
	-if exist $(OBJS)\*.res del $(OBJS)\*.res
	-if exist $(OBJS)\*.lbc del $(OBJS)\*.lbc
	-if exist $(OBJS)\*.ilk del $(OBJS)\*.ilk
	-if exist $(OBJS)\*.pch del $(OBJS)\*.pch
	-if exist $(OBJS)\test.exe del $(OBJS)\test.exe
	-if exist $(OBJS)\test_gui.exe del $(OBJS)\test_gui.exe

$(OBJS)\test.exe :  $(TEST_OBJECTS)
	@%create $(OBJS)\test.lbc
	@%append $(OBJS)\test.lbc option quiet
	@%append $(OBJS)\test.lbc name $^@
	@%append $(OBJS)\test.lbc option caseexact
	@%append $(OBJS)\test.lbc $(LDFLAGS) $(__DEBUGINFO_1)  libpath $(LIBDIRNAME) system nt ref 'main_' $(CPPUNIT_LIBS)
	@for %i in ($(TEST_OBJECTS)) do @%append $(OBJS)\test.lbc file %i
	@for %i in ( $(__WXLIB_NET_p)  $(__WXLIB_BASE_p)  $(__WXLIB_MONO_p) wxzlib$(WXDEBUGFLAG).lib  wxregex$(WXUNICODEFLAG)$(WXDEBUGFLAG).lib wxexpat$(WXDEBUGFLAG).lib $(EXTRALIBS_FOR_BASE)  kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib) do @%append $(OBJS)\test.lbc library %i
	@%append $(OBJS)\test.lbc
	@for %i in () do @%append $(OBJS)\test.lbc option stack=%i
	wlink @$(OBJS)\test.lbc

!ifeq USE_GUI 1
$(OBJS)\test_gui.exe :  $(TEST_GUI_OBJECTS) $(OBJS)\test_gui_sample.res
	@%create $(OBJS)\test_gui.lbc
	@%append $(OBJS)\test_gui.lbc option quiet
	@%append $(OBJS)\test_gui.lbc name $^@
	@%append $(OBJS)\test_gui.lbc option caseexact
	@%append $(OBJS)\test_gui.lbc $(LDFLAGS) $(__DEBUGINFO_1)  libpath $(LIBDIRNAME) $(CPPUNIT_LIBS) system nt ref 'main_'
	@for %i in ($(TEST_GUI_OBJECTS)) do @%append $(OBJS)\test_gui.lbc file %i
	@for %i in ( $(__WXLIB_CORE_p)  $(__WXLIB_BASE_p)  $(__WXLIB_MONO_p) $(__LIB_TIFF_p) $(__LIB_JPEG_p) $(__LIB_PNG_p)  wxzlib$(WXDEBUGFLAG).lib  wxregex$(WXUNICODEFLAG)$(WXDEBUGFLAG).lib wxexpat$(WXDEBUGFLAG).lib $(EXTRALIBS_FOR_BASE)  kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib) do @%append $(OBJS)\test_gui.lbc library %i
	@%append $(OBJS)\test_gui.lbc option resource=$(OBJS)\test_gui_sample.res
	@for %i in () do @%append $(OBJS)\test_gui.lbc option stack=%i
	wlink @$(OBJS)\test_gui.lbc
!endif

data : .SYMBOLIC 
	if not exist $(OBJS) mkdir $(OBJS)
	for %f in (testdata.fc) do if not exist $(OBJS)\%f copy .\%f $(OBJS)

$(OBJS)\test_dummy.obj :  .AUTODEPEND .\dummy.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_test.obj :  .AUTODEPEND .\test.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_archivetest.obj :  .AUTODEPEND .\archive\archivetest.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_ziptest.obj :  .AUTODEPEND .\archive\ziptest.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_arrays.obj :  .AUTODEPEND .\arrays\arrays.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_datetimetest.obj :  .AUTODEPEND .\datetime\datetimetest.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_fileconftest.obj :  .AUTODEPEND .\fileconf\fileconftest.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_filekind.obj :  .AUTODEPEND .\filekind\filekind.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_filenametest.obj :  .AUTODEPEND .\filename\filenametest.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_filesystest.obj :  .AUTODEPEND .\filesys\filesystest.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_fontmaptest.obj :  .AUTODEPEND .\fontmap\fontmaptest.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_formatconvertertest.obj :  .AUTODEPEND .\formatconverter\formatconvertertest.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_hashes.obj :  .AUTODEPEND .\hashes\hashes.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_lists.obj :  .AUTODEPEND .\lists\lists.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_longlongtest.obj :  .AUTODEPEND .\longlong\longlongtest.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_convautotest.obj :  .AUTODEPEND .\mbconv\convautotest.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_mbconvtest.obj :  .AUTODEPEND .\mbconv\mbconvtest.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_regextest.obj :  .AUTODEPEND .\regex\regextest.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_wxregextest.obj :  .AUTODEPEND .\regex\wxregextest.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_scopeguardtest.obj :  .AUTODEPEND .\scopeguard\scopeguardtest.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_strings.obj :  .AUTODEPEND .\strings\strings.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_stdstrings.obj :  .AUTODEPEND .\strings\stdstrings.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_tokenizer.obj :  .AUTODEPEND .\strings\tokenizer.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_unicode.obj :  .AUTODEPEND .\strings\unicode.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_crt.obj :  .AUTODEPEND .\strings\crt.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_vsnprintf.obj :  .AUTODEPEND .\strings\vsnprintf.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_bstream.obj :  .AUTODEPEND .\streams\bstream.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_datastreamtest.obj :  .AUTODEPEND .\streams\datastreamtest.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_ffilestream.obj :  .AUTODEPEND .\streams\ffilestream.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_filestream.obj :  .AUTODEPEND .\streams\filestream.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_largefile.obj :  .AUTODEPEND .\streams\largefile.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_memstream.obj :  .AUTODEPEND .\streams\memstream.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_sstream.obj :  .AUTODEPEND .\streams\sstream.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_tempfile.obj :  .AUTODEPEND .\streams\tempfile.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_textstreamtest.obj :  .AUTODEPEND .\streams\textstreamtest.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_zlibstream.obj :  .AUTODEPEND .\streams\zlibstream.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_textfiletest.obj :  .AUTODEPEND .\textfile\textfiletest.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_uris.obj :  .AUTODEPEND .\uris\uris.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_gui_sample.res :  .AUTODEPEND .\..\samples\sample.rc
	wrc -q -ad -bt=nt -r -fo=$^@   -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) $(__EXCEPTIONS_DEFINE_p) $(__RTTI_DEFINE_p) $(__THREAD_DEFINE_p) $(__UNICODE_DEFINE_p)  -i=$(SETUPHDIR) -i=.\..\include -i=. $(__DLLFLAG_p) -i=.\..\samples $<

$(OBJS)\test_gui_dummy.obj :  .AUTODEPEND .\dummy.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEST_GUI_CXXFLAGS) $<

$(OBJS)\test_gui_test.obj :  .AUTODEPEND .\test.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEST_GUI_CXXFLAGS) $<

$(OBJS)\test_gui_rect.obj :  .AUTODEPEND .\geometry\rect.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEST_GUI_CXXFLAGS) $<

$(OBJS)\test_gui_size.obj :  .AUTODEPEND .\geometry\size.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEST_GUI_CXXFLAGS) $<

$(OBJS)\test_gui_point.obj :  .AUTODEPEND .\geometry\point.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(TEST_GUI_CXXFLAGS) $<

