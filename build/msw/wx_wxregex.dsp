# Microsoft Developer Studio Project File - Name="wxregex" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=wxregex - Win32 DLL Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "wx_wxregex.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "wx_wxregex.mak" CFG="wxregex - Win32 DLL Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "wxregex - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "wxregex - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "wxregex - Win32 Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "wxregex - Win32 Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE "wxregex - Win32 Universal Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "wxregex - Win32 Universal Release" (based on "Win32 (x86) Static Library")
!MESSAGE "wxregex - Win32 Universal Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "wxregex - Win32 Universal Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE "wxregex - Win32 DLL Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "wxregex - Win32 DLL Release" (based on "Win32 (x86) Static Library")
!MESSAGE "wxregex - Win32 DLL Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "wxregex - Win32 DLL Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE "wxregex - Win32 DLL Universal Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "wxregex - Win32 DLL Universal Release" (based on "Win32 (x86) Static Library")
!MESSAGE "wxregex - Win32 DLL Universal Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "wxregex - Win32 DLL Universal Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "wxregex - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\lib\vc_lib"
# PROP BASE Intermediate_Dir "vc_mswd\wxregex"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\lib\vc_lib"
# PROP Intermediate_Dir "vc_mswd\wxregex"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /GR /GX /MDd /W1 /Od /Zi /Gm /GZ /Fd..\..\lib\vc_lib\wxregexd.pdb /D "WIN32" /D "_LIB" /D "_DEBUG" /c
# ADD CPP /nologo /FD /GR /GX /MDd /W1 /Od /Zi /Gm /GZ /Fd..\..\lib\vc_lib\wxregexd.pdb /D "WIN32" /D "_LIB" /D "_DEBUG" /c
# ADD BASE RSC /l 0x405
# ADD RSC /l 0x405
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\lib\vc_lib\wxregexd.lib"
# ADD LIB32 /nologo /out:"..\..\lib\vc_lib\wxregexd.lib"

!ELSEIF  "$(CFG)" == "wxregex - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\lib\vc_lib"
# PROP BASE Intermediate_Dir "vc_msw\wxregex"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\lib\vc_lib"
# PROP Intermediate_Dir "vc_msw\wxregex"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /GR /GX /MD /W1 /O1 /Fd..\..\lib\vc_lib\wxregex.pdb /D "WIN32" /D "_LIB" /c
# ADD CPP /nologo /FD /GR /GX /MD /W1 /O1 /Fd..\..\lib\vc_lib\wxregex.pdb /D "WIN32" /D "_LIB" /c
# ADD BASE RSC /l 0x405
# ADD RSC /l 0x405
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\lib\vc_lib\wxregex.lib"
# ADD LIB32 /nologo /out:"..\..\lib\vc_lib\wxregex.lib"

!ELSEIF  "$(CFG)" == "wxregex - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\lib\vc_lib"
# PROP BASE Intermediate_Dir "vc_mswud\wxregex"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\lib\vc_lib"
# PROP Intermediate_Dir "vc_mswud\wxregex"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /GR /GX /MDd /W1 /Od /Zi /Gm /GZ /Fd..\..\lib\vc_lib\wxregexd.pdb /D "WIN32" /D "_LIB" /D "_DEBUG" /c
# ADD CPP /nologo /FD /GR /GX /MDd /W1 /Od /Zi /Gm /GZ /Fd..\..\lib\vc_lib\wxregexd.pdb /D "WIN32" /D "_LIB" /D "_DEBUG" /c
# ADD BASE RSC /l 0x405
# ADD RSC /l 0x405
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\lib\vc_lib\wxregexd.lib"
# ADD LIB32 /nologo /out:"..\..\lib\vc_lib\wxregexd.lib"

!ELSEIF  "$(CFG)" == "wxregex - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\lib\vc_lib"
# PROP BASE Intermediate_Dir "vc_mswu\wxregex"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\lib\vc_lib"
# PROP Intermediate_Dir "vc_mswu\wxregex"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /GR /GX /MD /W1 /O1 /Fd..\..\lib\vc_lib\wxregex.pdb /D "WIN32" /D "_LIB" /c
# ADD CPP /nologo /FD /GR /GX /MD /W1 /O1 /Fd..\..\lib\vc_lib\wxregex.pdb /D "WIN32" /D "_LIB" /c
# ADD BASE RSC /l 0x405
# ADD RSC /l 0x405
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\lib\vc_lib\wxregex.lib"
# ADD LIB32 /nologo /out:"..\..\lib\vc_lib\wxregex.lib"

!ELSEIF  "$(CFG)" == "wxregex - Win32 Universal Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\lib\vc_lib"
# PROP BASE Intermediate_Dir "vc_mswunivd\wxregex"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\lib\vc_lib"
# PROP Intermediate_Dir "vc_mswunivd\wxregex"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /GR /GX /MDd /W1 /Od /Zi /Gm /GZ /Fd..\..\lib\vc_lib\wxregexd.pdb /D "WIN32" /D "_LIB" /D "_DEBUG" /c
# ADD CPP /nologo /FD /GR /GX /MDd /W1 /Od /Zi /Gm /GZ /Fd..\..\lib\vc_lib\wxregexd.pdb /D "WIN32" /D "_LIB" /D "_DEBUG" /c
# ADD BASE RSC /l 0x405
# ADD RSC /l 0x405
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\lib\vc_lib\wxregexd.lib"
# ADD LIB32 /nologo /out:"..\..\lib\vc_lib\wxregexd.lib"

!ELSEIF  "$(CFG)" == "wxregex - Win32 Universal Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\lib\vc_lib"
# PROP BASE Intermediate_Dir "vc_mswuniv\wxregex"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\lib\vc_lib"
# PROP Intermediate_Dir "vc_mswuniv\wxregex"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /GR /GX /MD /W1 /O1 /Fd..\..\lib\vc_lib\wxregex.pdb /D "WIN32" /D "_LIB" /c
# ADD CPP /nologo /FD /GR /GX /MD /W1 /O1 /Fd..\..\lib\vc_lib\wxregex.pdb /D "WIN32" /D "_LIB" /c
# ADD BASE RSC /l 0x405
# ADD RSC /l 0x405
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\lib\vc_lib\wxregex.lib"
# ADD LIB32 /nologo /out:"..\..\lib\vc_lib\wxregex.lib"

!ELSEIF  "$(CFG)" == "wxregex - Win32 Universal Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\lib\vc_lib"
# PROP BASE Intermediate_Dir "vc_mswunivud\wxregex"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\lib\vc_lib"
# PROP Intermediate_Dir "vc_mswunivud\wxregex"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /GR /GX /MDd /W1 /Od /Zi /Gm /GZ /Fd..\..\lib\vc_lib\wxregexd.pdb /D "WIN32" /D "_LIB" /D "_DEBUG" /c
# ADD CPP /nologo /FD /GR /GX /MDd /W1 /Od /Zi /Gm /GZ /Fd..\..\lib\vc_lib\wxregexd.pdb /D "WIN32" /D "_LIB" /D "_DEBUG" /c
# ADD BASE RSC /l 0x405
# ADD RSC /l 0x405
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\lib\vc_lib\wxregexd.lib"
# ADD LIB32 /nologo /out:"..\..\lib\vc_lib\wxregexd.lib"

!ELSEIF  "$(CFG)" == "wxregex - Win32 Universal Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\lib\vc_lib"
# PROP BASE Intermediate_Dir "vc_mswunivu\wxregex"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\lib\vc_lib"
# PROP Intermediate_Dir "vc_mswunivu\wxregex"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /GR /GX /MD /W1 /O1 /Fd..\..\lib\vc_lib\wxregex.pdb /D "WIN32" /D "_LIB" /c
# ADD CPP /nologo /FD /GR /GX /MD /W1 /O1 /Fd..\..\lib\vc_lib\wxregex.pdb /D "WIN32" /D "_LIB" /c
# ADD BASE RSC /l 0x405
# ADD RSC /l 0x405
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\lib\vc_lib\wxregex.lib"
# ADD LIB32 /nologo /out:"..\..\lib\vc_lib\wxregex.lib"

!ELSEIF  "$(CFG)" == "wxregex - Win32 DLL Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vc_mswddll\wxregex"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\lib\vc_dll"
# PROP Intermediate_Dir "vc_mswddll\wxregex"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /GR /GX /MDd /W1 /Od /Zi /Gm /GZ /Fd..\..\lib\vc_dll\wxregexd.pdb /D "WIN32" /D "_LIB" /D "_DEBUG" /c
# ADD CPP /nologo /FD /GR /GX /MDd /W1 /Od /Zi /Gm /GZ /Fd..\..\lib\vc_dll\wxregexd.pdb /D "WIN32" /D "_LIB" /D "_DEBUG" /c
# ADD BASE RSC /l 0x405
# ADD RSC /l 0x405
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\lib\vc_dll\wxregexd.lib"
# ADD LIB32 /nologo /out:"..\..\lib\vc_dll\wxregexd.lib"

!ELSEIF  "$(CFG)" == "wxregex - Win32 DLL Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vc_mswdll\wxregex"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\lib\vc_dll"
# PROP Intermediate_Dir "vc_mswdll\wxregex"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /GR /GX /MD /W1 /O1 /Fd..\..\lib\vc_dll\wxregex.pdb /D "WIN32" /D "_LIB" /c
# ADD CPP /nologo /FD /GR /GX /MD /W1 /O1 /Fd..\..\lib\vc_dll\wxregex.pdb /D "WIN32" /D "_LIB" /c
# ADD BASE RSC /l 0x405
# ADD RSC /l 0x405
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\lib\vc_dll\wxregex.lib"
# ADD LIB32 /nologo /out:"..\..\lib\vc_dll\wxregex.lib"

!ELSEIF  "$(CFG)" == "wxregex - Win32 DLL Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vc_mswuddll\wxregex"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\lib\vc_dll"
# PROP Intermediate_Dir "vc_mswuddll\wxregex"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /GR /GX /MDd /W1 /Od /Zi /Gm /GZ /Fd..\..\lib\vc_dll\wxregexd.pdb /D "WIN32" /D "_LIB" /D "_DEBUG" /c
# ADD CPP /nologo /FD /GR /GX /MDd /W1 /Od /Zi /Gm /GZ /Fd..\..\lib\vc_dll\wxregexd.pdb /D "WIN32" /D "_LIB" /D "_DEBUG" /c
# ADD BASE RSC /l 0x405
# ADD RSC /l 0x405
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\lib\vc_dll\wxregexd.lib"
# ADD LIB32 /nologo /out:"..\..\lib\vc_dll\wxregexd.lib"

!ELSEIF  "$(CFG)" == "wxregex - Win32 DLL Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vc_mswudll\wxregex"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\lib\vc_dll"
# PROP Intermediate_Dir "vc_mswudll\wxregex"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /GR /GX /MD /W1 /O1 /Fd..\..\lib\vc_dll\wxregex.pdb /D "WIN32" /D "_LIB" /c
# ADD CPP /nologo /FD /GR /GX /MD /W1 /O1 /Fd..\..\lib\vc_dll\wxregex.pdb /D "WIN32" /D "_LIB" /c
# ADD BASE RSC /l 0x405
# ADD RSC /l 0x405
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\lib\vc_dll\wxregex.lib"
# ADD LIB32 /nologo /out:"..\..\lib\vc_dll\wxregex.lib"

!ELSEIF  "$(CFG)" == "wxregex - Win32 DLL Universal Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vc_mswunivddll\wxregex"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\lib\vc_dll"
# PROP Intermediate_Dir "vc_mswunivddll\wxregex"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /GR /GX /MDd /W1 /Od /Zi /Gm /GZ /Fd..\..\lib\vc_dll\wxregexd.pdb /D "WIN32" /D "_LIB" /D "_DEBUG" /c
# ADD CPP /nologo /FD /GR /GX /MDd /W1 /Od /Zi /Gm /GZ /Fd..\..\lib\vc_dll\wxregexd.pdb /D "WIN32" /D "_LIB" /D "_DEBUG" /c
# ADD BASE RSC /l 0x405
# ADD RSC /l 0x405
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\lib\vc_dll\wxregexd.lib"
# ADD LIB32 /nologo /out:"..\..\lib\vc_dll\wxregexd.lib"

!ELSEIF  "$(CFG)" == "wxregex - Win32 DLL Universal Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vc_mswunivdll\wxregex"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\lib\vc_dll"
# PROP Intermediate_Dir "vc_mswunivdll\wxregex"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /GR /GX /MD /W1 /O1 /Fd..\..\lib\vc_dll\wxregex.pdb /D "WIN32" /D "_LIB" /c
# ADD CPP /nologo /FD /GR /GX /MD /W1 /O1 /Fd..\..\lib\vc_dll\wxregex.pdb /D "WIN32" /D "_LIB" /c
# ADD BASE RSC /l 0x405
# ADD RSC /l 0x405
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\lib\vc_dll\wxregex.lib"
# ADD LIB32 /nologo /out:"..\..\lib\vc_dll\wxregex.lib"

!ELSEIF  "$(CFG)" == "wxregex - Win32 DLL Universal Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vc_mswunivuddll\wxregex"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\lib\vc_dll"
# PROP Intermediate_Dir "vc_mswunivuddll\wxregex"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /GR /GX /MDd /W1 /Od /Zi /Gm /GZ /Fd..\..\lib\vc_dll\wxregexd.pdb /D "WIN32" /D "_LIB" /D "_DEBUG" /c
# ADD CPP /nologo /FD /GR /GX /MDd /W1 /Od /Zi /Gm /GZ /Fd..\..\lib\vc_dll\wxregexd.pdb /D "WIN32" /D "_LIB" /D "_DEBUG" /c
# ADD BASE RSC /l 0x405
# ADD RSC /l 0x405
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\lib\vc_dll\wxregexd.lib"
# ADD LIB32 /nologo /out:"..\..\lib\vc_dll\wxregexd.lib"

!ELSEIF  "$(CFG)" == "wxregex - Win32 DLL Universal Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vc_mswunivudll\wxregex"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\lib\vc_dll"
# PROP Intermediate_Dir "vc_mswunivudll\wxregex"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /GR /GX /MD /W1 /O1 /Fd..\..\lib\vc_dll\wxregex.pdb /D "WIN32" /D "_LIB" /c
# ADD CPP /nologo /FD /GR /GX /MD /W1 /O1 /Fd..\..\lib\vc_dll\wxregex.pdb /D "WIN32" /D "_LIB" /c
# ADD BASE RSC /l 0x405
# ADD RSC /l 0x405
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\lib\vc_dll\wxregex.lib"
# ADD LIB32 /nologo /out:"..\..\lib\vc_dll\wxregex.lib"

!ENDIF

# Begin Target

# Name "wxregex - Win32 Debug"
# Name "wxregex - Win32 Release"
# Name "wxregex - Win32 Unicode Debug"
# Name "wxregex - Win32 Unicode Release"
# Name "wxregex - Win32 Universal Debug"
# Name "wxregex - Win32 Universal Release"
# Name "wxregex - Win32 Universal Unicode Debug"
# Name "wxregex - Win32 Universal Unicode Release"
# Name "wxregex - Win32 DLL Debug"
# Name "wxregex - Win32 DLL Release"
# Name "wxregex - Win32 DLL Unicode Debug"
# Name "wxregex - Win32 DLL Unicode Release"
# Name "wxregex - Win32 DLL Universal Debug"
# Name "wxregex - Win32 DLL Universal Release"
# Name "wxregex - Win32 DLL Universal Unicode Debug"
# Name "wxregex - Win32 DLL Universal Unicode Release"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\regex\regcomp.c
# End Source File
# Begin Source File

SOURCE=..\..\src\regex\regerror.c
# End Source File
# Begin Source File

SOURCE=..\..\src\regex\regexec.c
# End Source File
# Begin Source File

SOURCE=..\..\src\regex\regfree.c
# End Source File
# End Group
# End Target
# End Project

