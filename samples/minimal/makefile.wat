#
# Makefile for WATCOM
#
# Created by Julian Smart, January 1999
# 
#

OUTPUTDIR = Watcom\

PROGRAM = minimal
OBJECTS = $(OUTPUTDIR)$(PROGRAM).obj

!include $(%WXWIN)\src\makeprog.wat


