# =========================================================================
#     This makefile was generated by
#     Bakefile 0.1.4 (http://bakefile.sourceforge.net)
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
LIBDIRNAME = .\..\lib\wat_lib$(CFG)
!endif
!ifeq SHARED 1
LIBDIRNAME = .\..\lib\wat_dll$(CFG)
!endif

### Variables: ###

OBJS = &
	wat_$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WXDLLFLAG)$(CFG)
SETUPHDIR = &
	$(LIBDIRNAME)\$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)
TEST_CXXFLAGS = $(__DEBUGINFO_0) $(__OPTIMIZEFLAG_2) -bm $(__RUNTIME_LIBS_5) &
	-d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) $(__UNICODE_DEFINE_p) &
	-i=.\..\include -i=$(SETUPHDIR) -i=. $(__DLLFLAG_p) $(CPPUNIT_CFLAGS) &
	$(__EXCEPTIONSFLAG_7) $(CPPFLAGS) $(CXXFLAGS)
TEST_OBJECTS =  &
	$(OBJS)\test_test.obj &
	$(OBJS)\test_main.obj &
	$(OBJS)\test_formatconverter.obj &
	$(OBJS)\test_regex.obj &
	$(OBJS)\test_wxregex.obj &
	$(OBJS)\test_filename.obj &
	$(OBJS)\test_filesys.obj &
	$(OBJS)\test_arrays.obj &
	$(OBJS)\test_hashes.obj &
	$(OBJS)\test_longlong.obj &
	$(OBJS)\test_strings.obj &
	$(OBJS)\test_stdstrings.obj &
	$(OBJS)\test_unicode.obj &
	$(OBJS)\test_crt.obj &
	$(OBJS)\test_bstream.obj &
	$(OBJS)\test_ffilestream.obj &
	$(OBJS)\test_filestream.obj &
	$(OBJS)\test_memstream.obj &
	$(OBJS)\test_zlibstream.obj &
	$(OBJS)\test_fontmap.obj &
	$(OBJS)\test_datetime.obj


all : $(OBJS)
$(OBJS) :
	-if not exist $(OBJS) mkdir $(OBJS)

### Targets: ###

all : .SYMBOLIC $(OBJS)\test.exe data

clean : .SYMBOLIC 
	-if exist $(OBJS)\*.obj del $(OBJS)\*.obj
	-if exist $(OBJS)\*.res del $(OBJS)\*.res
	-if exist $(OBJS)\*.lbc del $(OBJS)\*.lbc
	-if exist $(OBJS)\*.ilk del $(OBJS)\*.ilk
	-if exist $(OBJS)\test.exe del $(OBJS)\test.exe

$(OBJS)\test.exe :  $(TEST_OBJECTS)
	@%create $(OBJS)\test.lbc
	@%append $(OBJS)\test.lbc option quiet
	@%append $(OBJS)\test.lbc name $^@
	@%append $(OBJS)\test.lbc option caseexact
	@%append $(OBJS)\test.lbc $(LDFLAGS) $(__DEBUGINFO_1)  libpath $(LIBDIRNAME) system nt ref 'main_' $(CPPUNIT_LIBS)
	@for %i in ($(TEST_OBJECTS)) do @%append $(OBJS)\test.lbc file %i
	@for %i in ( $(__WXLIB_BASE_p)  $(__WXLIB_MONO_p) $(__LIB_TIFF_p) $(__LIB_JPEG_p) $(__LIB_PNG_p) wxzlib$(WXDEBUGFLAG).lib  wxregex$(WXUNICODEFLAG)$(WXDEBUGFLAG).lib wxexpat$(WXDEBUGFLAG).lib   kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib) do @%append $(OBJS)\test.lbc library %i
	@%append $(OBJS)\test.lbc
	wlink @$(OBJS)\test.lbc

data : .SYMBOLIC 
	if not exist $(OBJS) mkdir $(OBJS)
	for %f in (testdata.fc) do if not exist $(OBJS)\%f copy .\%f $(OBJS)

$(OBJS)\test_test.obj :  .AUTODEPEND .\test.cpp
	$(CXX) -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_main.obj :  .AUTODEPEND .\mbconv\main.cpp
	$(CXX) -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_formatconverter.obj :  .AUTODEPEND .\formatconverter\formatconverter.cpp
	$(CXX) -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_regex.obj :  .AUTODEPEND .\regex\regex.cpp
	$(CXX) -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_wxregex.obj :  .AUTODEPEND .\regex\wxregex.cpp
	$(CXX) -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_filename.obj :  .AUTODEPEND .\filename\filename.cpp
	$(CXX) -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_filesys.obj :  .AUTODEPEND .\filesys\filesys.cpp
	$(CXX) -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_arrays.obj :  .AUTODEPEND .\arrays\arrays.cpp
	$(CXX) -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_hashes.obj :  .AUTODEPEND .\hashes\hashes.cpp
	$(CXX) -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_longlong.obj :  .AUTODEPEND .\longlong\longlong.cpp
	$(CXX) -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_strings.obj :  .AUTODEPEND .\strings\strings.cpp
	$(CXX) -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_stdstrings.obj :  .AUTODEPEND .\strings\stdstrings.cpp
	$(CXX) -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_unicode.obj :  .AUTODEPEND .\strings\unicode.cpp
	$(CXX) -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_crt.obj :  .AUTODEPEND .\strings\crt.cpp
	$(CXX) -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_bstream.obj :  .AUTODEPEND .\streams\bstream.cpp
	$(CXX) -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_ffilestream.obj :  .AUTODEPEND .\streams\ffilestream.cpp
	$(CXX) -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_filestream.obj :  .AUTODEPEND .\streams\filestream.cpp
	$(CXX) -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_memstream.obj :  .AUTODEPEND .\streams\memstream.cpp
	$(CXX) -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_zlibstream.obj :  .AUTODEPEND .\streams\zlibstream.cpp
	$(CXX) -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_fontmap.obj :  .AUTODEPEND .\fontmap\fontmap.cpp
	$(CXX) -zq -fo=$^@ $(TEST_CXXFLAGS) $<

$(OBJS)\test_datetime.obj :  .AUTODEPEND .\datetime\datetime.cpp
	$(CXX) -zq -fo=$^@ $(TEST_CXXFLAGS) $<
