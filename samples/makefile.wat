# =========================================================================
#     This makefile was generated by
#     Bakefile 0.1.1 (http://bakefile.sourceforge.net)
#     Do not modify, all changes will be overwritten!
# =========================================================================

!include ../build/config.wat

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

__console___depname =
!ifeq USE_GUI 0
__console___depname = console
!endif

### Variables: ###




### Targets: ###

all : .SYMBOLIC artprov $(__console___depname) dialogs image minimal

artprov : .SYMBOLIC 
	cd artprov
	wmake $(__MAKEOPTS__) all
	cd $(WATCOM_CWD)

clean : .SYMBOLIC 
	-if exist .\*.obj del .\*.obj
	-if exist .\*.res del .\*.res
	-if exist .\*.lbc del .\*.lbc
	-if exist .\*.ilk del .\*.ilk

!ifeq USE_GUI 0
console : .SYMBOLIC 
	cd console
	wmake $(__MAKEOPTS__) all
	cd $(WATCOM_CWD)
!endif

dialogs : .SYMBOLIC 
	cd dialogs
	wmake $(__MAKEOPTS__) all
	cd $(WATCOM_CWD)

image : .SYMBOLIC 
	cd image
	wmake $(__MAKEOPTS__) all
	cd $(WATCOM_CWD)

minimal : .SYMBOLIC 
	cd minimal
	wmake $(__MAKEOPTS__) all
	cd $(WATCOM_CWD)
