# =========================================================================
#     This configuration file was generated by
#     Bakefile 0.2.0 (http://bakefile.sourceforge.net)
#     Beware that all changes made to this file will be overwritten next
#     time you run Bakefile!
# =========================================================================


# -------------------------------------------------------------------------
# These are configurable options:
# -------------------------------------------------------------------------

# DOS32 mode/extender [X32VM,X32,PMODEW,CAUSEWAY,DOS32A,DOS4GW]
#   X32VM - FlashTek X-32VM DOS extender
#   X32 - FlashTek X-32 DOS extender
#   PMODEW - PMODE/W
#   CAUSEWAY - CauseWay DOS extender
#   DOS32A - DOS32/A
#   DOS4GW - Tenberry/Watcom 32-bit DOS extender
DOS32 = DOS4GW

# C compiler 
CC = wcc386

# C++ compiler 
CXX = wpp386

# Standard flags for CC 
CFLAGS = 

# Standard flags for C++ 
CXXFLAGS = 

# Standard preprocessor flags (common for CC and CXX) 
CPPFLAGS = 

# Standard linker flags 
LDFLAGS = 

# The C preprocessor 
CPP = $(CC) -p

# What type of library to build? [0,1]
#   1 - DLL
SHARED = 0

# Compile Unicode build of wxWidgets? [0,1]
#   1 - Unicode
UNICODE = 0

# Type of compiled binaries [debug,release]
BUILD = debug

# Should debugging info be included in the executables? The default value
# "default" means that debug info will be included if BUILD=debug
# and not included if BUILD=release. [0,1,default]
DEBUG_INFO = default

# Should __WXDEBUG__ be defined? The default value "default" means that it will
# be defined if BUILD=debug and not defined if BUILD=release. [0,1,default]
DEBUG_FLAG = default

# Multiple libraries or single huge monolithic one? [0,1]
#   0 - Multilib
#   1 - Monolithic
MONOLITHIC = 1

# Build GUI libraries? [0,1]
#   0 - Base
#   1 - GUI
USE_GUI = 1

# Build wxHTML library (USE_GUI must be 1)? [0,1]
USE_HTML = 1

# Build multimedia library (USE_GUI must be 1)? [0,1]
USE_MEDIA = 1

# Build wxXRC library (USE_GUI must be 1)? [0,1]
USE_XRC = 1

# Build OpenGL canvas library (USE_GUI must be 1)? [0,1]
USE_OPENGL = 0

# Build ODBC database classes (USE_GUI must be 1)? [0,1]
USE_ODBC = 0

# Build quality assurance classes library (USE_GUI must be 1)? [0,1]
USE_QA = 0

# Enable exceptions in compiled code. [0,1]
USE_EXCEPTIONS = 1

# Enable run-time type information (RTTI) in compiled code. [0,1]
USE_RTTI = 1

# Is this official build by wxWidgets developers? [0,1]
OFFICIAL_BUILD = 0

# Use this to name your customized DLLs differently 
VENDOR = custom

#  
WX_FLAVOUR = 

#  
WX_LIB_FLAVOUR = 

# Name of your custom configuration. This affects directory
# where object files are stored as well as the location of
# compiled .lib files and setup.h under the lib/ toplevel directory. 
CFG = 

# Compiler flags needed to compile test suite in tests directory. If you want
# to run the tests, set it so that the compiler can find CppUnit headers. 
CPPUNIT_CFLAGS = 

# Linker flags needed to link test suite in tests directory. If you want
# to run the tests, include CppUnit library here. 
CPPUNIT_LIBS = 

