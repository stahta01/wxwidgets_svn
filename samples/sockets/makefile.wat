# =========================================================================
#     This makefile was generated by
#     Bakefile 0.1.1 (http://bakefile.sourceforge.net)
#     Do not modify, all changes will be overwritten!
# =========================================================================

!include ../../build/config.wat

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
__DEBUGFLAG =
!ifeq BUILD debug
__DEBUGFLAG = -d2
!endif
!ifeq BUILD release
__DEBUGFLAG = -d0
!endif
__DEBUGFLAG_2 =
!ifeq BUILD debug
__DEBUGFLAG_2 = debug all
!endif
!ifeq BUILD release
__DEBUGFLAG_2 = 
!endif
__DEBUG_DEFINE_p =
!ifeq BUILD debug
__DEBUG_DEFINE_p = -d__WXDEBUG__
!endif
__DLLFLAG_p =
!ifeq SHARED 1
__DLLFLAG_p = -dWXUSINGDLL
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
__WXLIB_CORE_p = wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_core.lib
!endif
__WXLIB_MONO_p =
!ifeq MONOLITHIC 1
__WXLIB_MONO_p = wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG).lib
!endif
__WXUNIV_DEFINE_p =
!ifeq WXUNIV 1
__WXUNIV_DEFINE_p = -d__WXUNIVERSAL__
!endif

### Variables: ###

CLIENT_CXXFLAGS = $(CPPFLAGS) $(__DEBUGFLAG) $(__OPTIMIZEFLAG) -bm $(__RUNTIME_LIBS)  -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) $(__UNICODE_DEFINE_p) -i=.\..\..\include -i=.\..\..\lib\wat_$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WXDLLFLAG)$(CFG) -i=.\..\..\src\tiff -i=.\..\..\src\jpeg -i=.\..\..\src\png -i=.\..\..\src\zlib  -i=.\..\..\src\regex -i=.\..\..\src\expat\lib -i=. $(__DLLFLAG_p) $(CXXFLAGS) 
OBJS = wat_$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WXDLLFLAG)$(CFG)
SERVER_CXXFLAGS = $(CPPFLAGS) $(__DEBUGFLAG) $(__OPTIMIZEFLAG) -bm $(__RUNTIME_LIBS)  -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) $(__UNICODE_DEFINE_p) -i=.\..\..\include -i=.\..\..\lib\wat_$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WXDLLFLAG)$(CFG) -i=.\..\..\src\tiff -i=.\..\..\src\jpeg -i=.\..\..\src\png -i=.\..\..\src\zlib  -i=.\..\..\src\regex -i=.\..\..\src\expat\lib -i=. $(__DLLFLAG_p) $(CXXFLAGS) 



all : $(OBJS)
$(OBJS) :
	-if not exist $(OBJS) mkdir $(OBJS)

### Targets: ###

all : .SYMBOLIC $(OBJS)\client.exe $(OBJS)\server.exe

$(OBJS)\client_client.obj :  .AUTODEPEND .\client.cpp
	$(CXX) -zq -fo=$^@ $(CLIENT_CXXFLAGS) $<

$(OBJS)\client_client.res :  .AUTODEPEND .\client.rc
	wrc -q -ad -bt=nt -r -fo=$^@ -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) $(__UNICODE_DEFINE_p) -i=.\..\..\include -i=.\..\..\lib\wat_$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WXDLLFLAG)$(CFG) -i=.\..\..\src\tiff -i=.\..\..\src\jpeg -i=.\..\..\src\png -i=.\..\..\src\zlib  -i=.\..\..\src\regex -i=.\..\..\src\expat\lib -i=. $(__DLLFLAG_p) $<

$(OBJS)\server_server.obj :  .AUTODEPEND .\server.cpp
	$(CXX) -zq -fo=$^@ $(SERVER_CXXFLAGS) $<

$(OBJS)\server_server.res :  .AUTODEPEND .\server.rc
	wrc -q -ad -bt=nt -r -fo=$^@ -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) $(__UNICODE_DEFINE_p) -i=.\..\..\include -i=.\..\..\lib\wat_$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WXDLLFLAG)$(CFG) -i=.\..\..\src\tiff -i=.\..\..\src\jpeg -i=.\..\..\src\png -i=.\..\..\src\zlib  -i=.\..\..\src\regex -i=.\..\..\src\expat\lib -i=. $(__DLLFLAG_p) $<

clean : .SYMBOLIC 
	-if exist $(OBJS)\*.obj del $(OBJS)\*.obj
	-if exist $(OBJS)\*.res del $(OBJS)\*.res
	-if exist $(OBJS)\*.lbc del $(OBJS)\*.lbc
	-if exist $(OBJS)\*.ilk del $(OBJS)\*.ilk
	-if exist $(OBJS)\client.exe del $(OBJS)\client.exe
	-if exist $(OBJS)\server.exe del $(OBJS)\server.exe

$(OBJS)\client.exe :  $(OBJS)\client_client.obj $(OBJS)\client_client.res
	@%create $(OBJS)\client.lbc
	@%append $(OBJS)\client.lbc option quiet
	@%append $(OBJS)\client.lbc name $^@
	@%append $(OBJS)\client.lbc option incremental
	@%append $(OBJS)\client.lbc $(LDFLAGS) $(__DEBUGFLAG_2)  libpath .\..\..\lib\wat_$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WXDLLFLAG)$(CFG) system nt_win ref '_WinMain@16'
	@for %i in ( $(OBJS)\client_client.obj) do @%append $(OBJS)\client.lbc file %i
	@for %i in ( $(__WXLIB_CORE_p) $(__WXLIB_BASE_p) $(__WXLIB_MONO_p) wxtiff$(WXDEBUGFLAG).lib wxjpeg$(WXDEBUGFLAG).lib wxpng$(WXDEBUGFLAG).lib wxzlib$(WXDEBUGFLAG).lib  wxregex$(WXDEBUGFLAG).lib wxexpat$(WXDEBUGFLAG).lib  kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib ) do @%append $(OBJS)\client.lbc library %i
	@%append $(OBJS)\client.lbc option resource=$(OBJS)\client_client.res
	wlink @$(OBJS)\client.lbc

$(OBJS)\server.exe :  $(OBJS)\server_server.obj $(OBJS)\server_server.res
	@%create $(OBJS)\server.lbc
	@%append $(OBJS)\server.lbc option quiet
	@%append $(OBJS)\server.lbc name $^@
	@%append $(OBJS)\server.lbc option incremental
	@%append $(OBJS)\server.lbc $(LDFLAGS) $(__DEBUGFLAG_2)  libpath .\..\..\lib\wat_$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WXDLLFLAG)$(CFG) system nt_win ref '_WinMain@16'
	@for %i in ( $(OBJS)\server_server.obj) do @%append $(OBJS)\server.lbc file %i
	@for %i in ( $(__WXLIB_CORE_p) $(__WXLIB_BASE_p) $(__WXLIB_MONO_p) wxtiff$(WXDEBUGFLAG).lib wxjpeg$(WXDEBUGFLAG).lib wxpng$(WXDEBUGFLAG).lib wxzlib$(WXDEBUGFLAG).lib  wxregex$(WXDEBUGFLAG).lib wxexpat$(WXDEBUGFLAG).lib  kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib ) do @%append $(OBJS)\server.lbc library %i
	@%append $(OBJS)\server.lbc option resource=$(OBJS)\server_server.res
	wlink @$(OBJS)\server.lbc
