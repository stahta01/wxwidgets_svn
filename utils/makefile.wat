# =========================================================================
#     This makefile was generated by
#     Bakefile 0.2.9 (http://www.bakefile.org)
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


### Variables: ###

MAKEARGS = CC="$(CC)" CXX="$(CXX)" CFLAGS="$(CFLAGS)" CXXFLAGS="$(CXXFLAGS)" &
	CPPFLAGS="$(CPPFLAGS)" LDFLAGS="$(LDFLAGS)" CPP="$(CPP)" SHARED="$(SHARED)" &
	WXUNIV="$(WXUNIV)" UNICODE="$(UNICODE)" BUILD="$(BUILD)" &
	DEBUG_INFO="$(DEBUG_INFO)" DEBUG_FLAG="$(DEBUG_FLAG)" &
	MONOLITHIC="$(MONOLITHIC)" USE_GUI="$(USE_GUI)" USE_HTML="$(USE_HTML)" &
	USE_MEDIA="$(USE_MEDIA)" USE_XRC="$(USE_XRC)" USE_AUI="$(USE_AUI)" &
	USE_RIBBON="$(USE_RIBBON)" USE_PROPGRID="$(USE_PROPGRID)" &
	USE_RICHTEXT="$(USE_RICHTEXT)" USE_STC="$(USE_STC)" &
	USE_OPENGL="$(USE_OPENGL)" USE_QA="$(USE_QA)" &
	USE_EXCEPTIONS="$(USE_EXCEPTIONS)" USE_RTTI="$(USE_RTTI)" &
	USE_THREADS="$(USE_THREADS)" USE_CAIRO="$(USE_CAIRO)" &
	OFFICIAL_BUILD="$(OFFICIAL_BUILD)" VENDOR="$(VENDOR)" &
	WX_FLAVOUR="$(WX_FLAVOUR)" WX_LIB_FLAVOUR="$(WX_LIB_FLAVOUR)" CFG="$(CFG)" &
	CPPUNIT_CFLAGS="$(CPPUNIT_CFLAGS)" CPPUNIT_LIBS="$(CPPUNIT_LIBS)" &
	RUNTIME_LIBS="$(RUNTIME_LIBS)"


### Targets: ###

all : .SYMBOLIC emulator execmon helpview hhp2cached ifacecheck screenshotgen wxrc

clean : .SYMBOLIC 
	-if exist .\*.obj del .\*.obj
	-if exist .\*.res del .\*.res
	-if exist .\*.lbc del .\*.lbc
	-if exist .\*.ilk del .\*.ilk
	-if exist .\*.pch del .\*.pch
	cd emulator\src
	wmake $(__MAKEOPTS__) -f makefile.wat $(MAKEARGS) clean
	cd $(WATCOM_CWD)
	cd execmon
	wmake $(__MAKEOPTS__) -f makefile.wat $(MAKEARGS) clean
	cd $(WATCOM_CWD)
	cd helpview\src
	wmake $(__MAKEOPTS__) -f makefile.wat $(MAKEARGS) clean
	cd $(WATCOM_CWD)
	cd hhp2cached
	wmake $(__MAKEOPTS__) -f makefile.wat $(MAKEARGS) clean
	cd $(WATCOM_CWD)
	cd ifacecheck\src
	wmake $(__MAKEOPTS__) -f makefile.wat $(MAKEARGS) clean
	cd $(WATCOM_CWD)
	cd screenshotgen\src
	wmake $(__MAKEOPTS__) -f makefile.wat $(MAKEARGS) clean
	cd $(WATCOM_CWD)
	cd wxrc
	wmake $(__MAKEOPTS__) -f makefile.wat $(MAKEARGS) clean
	cd $(WATCOM_CWD)

emulator : .SYMBOLIC 
	cd emulator\src
	wmake $(__MAKEOPTS__) -f makefile.wat $(MAKEARGS) all
	cd $(WATCOM_CWD)

execmon : .SYMBOLIC 
	cd execmon
	wmake $(__MAKEOPTS__) -f makefile.wat $(MAKEARGS) all
	cd $(WATCOM_CWD)

helpview : .SYMBOLIC 
	cd helpview\src
	wmake $(__MAKEOPTS__) -f makefile.wat $(MAKEARGS) all
	cd $(WATCOM_CWD)

hhp2cached : .SYMBOLIC 
	cd hhp2cached
	wmake $(__MAKEOPTS__) -f makefile.wat $(MAKEARGS) all
	cd $(WATCOM_CWD)

ifacecheck : .SYMBOLIC 
	cd ifacecheck\src
	wmake $(__MAKEOPTS__) -f makefile.wat $(MAKEARGS) all
	cd $(WATCOM_CWD)

screenshotgen : .SYMBOLIC 
	cd screenshotgen\src
	wmake $(__MAKEOPTS__) -f makefile.wat $(MAKEARGS) all
	cd $(WATCOM_CWD)

wxrc : .SYMBOLIC 
	cd wxrc
	wmake $(__MAKEOPTS__) -f makefile.wat $(MAKEARGS) all
	cd $(WATCOM_CWD)

