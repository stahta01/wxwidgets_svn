#Digital Mars (was Symantec) C++ makefile 
WXDIR = $(WXWIN)
EXTRALIBS=
TARGET=keyboard
OBJECTS = $(TARGET).obj
include $(WXDIR)\src\makeprog.sc
