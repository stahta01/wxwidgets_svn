# Microsoft Developer Studio Project File - Name="animate" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102
# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=animate - Win32 DLL Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "animate.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "animate.mak" CFG="animate - Win32 DLL Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "animate - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "animate - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "animate - Win32 Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "animate - Win32 Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE "animate - Win32 Universal Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "animate - Win32 Universal Release" (based on "Win32 (x86) Static Library")
!MESSAGE "animate - Win32 Universal Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "animate - Win32 Universal Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE "animate - Win32 DLL Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "animate - Win32 DLL Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "animate - Win32 DLL Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "animate - Win32 DLL Unicode Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "animate - Win32 DLL Universal Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "animate - Win32 DLL Universal Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "animate - Win32 DLL Universal Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "animate - Win32 DLL Universal Unicode Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "animate - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\src\animate\..\..\..\lib\vc_lib"
# PROP BASE Intermediate_Dir "vc_mswd\animate"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\src\animate\..\..\..\lib\vc_lib"
# PROP Intermediate_Dir "vc_mswd\animate"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /W4 /MDd /Od /I "..\..\src\animate\..\..\..\include" /I "..\..\src\animate\..\..\..\lib\vc_lib\mswd" /Zi /Gm /GZ /Fd..\..\src\animate\..\..\..\lib\vc_lib\wxmsw25d_animate.pdb /Yu"wx/wxprec.h" /Fp"vc_mswd\wxprec_animatelib.pch" /I "..\..\src\animate\..\..\include" /D "WIN32" /D "_LIB" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /c
# ADD CPP /nologo /FD /W4 /MDd /Od /I "..\..\src\animate\..\..\..\include" /I "..\..\src\animate\..\..\..\lib\vc_lib\mswd" /Zi /Gm /GZ /Fd..\..\src\animate\..\..\..\lib\vc_lib\wxmsw25d_animate.pdb /Yu"wx/wxprec.h" /Fp"vc_mswd\wxprec_animatelib.pch" /I "..\..\src\animate\..\..\include" /D "WIN32" /D "_LIB" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /c
# ADD BASE RSC /l 0x405
# ADD RSC /l 0x405
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\src\animate\..\..\..\lib\vc_lib\wxmsw25d_animate.lib"
# ADD LIB32 /nologo /out:"..\..\src\animate\..\..\..\lib\vc_lib\wxmsw25d_animate.lib"

!ELSEIF  "$(CFG)" == "animate - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\src\animate\..\..\..\lib\vc_lib"
# PROP BASE Intermediate_Dir "vc_msw\animate"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\src\animate\..\..\..\lib\vc_lib"
# PROP Intermediate_Dir "vc_msw\animate"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /W4 /MD /O1 /I "..\..\src\animate\..\..\..\include" /I "..\..\src\animate\..\..\..\lib\vc_lib\msw" /Fd..\..\src\animate\..\..\..\lib\vc_lib\wxmsw25_animate.pdb /Yu"wx/wxprec.h" /Fp"vc_msw\wxprec_animatelib.pch" /I "..\..\src\animate\..\..\include" /D "WIN32" /D "_LIB" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /W4 /MD /O1 /I "..\..\src\animate\..\..\..\include" /I "..\..\src\animate\..\..\..\lib\vc_lib\msw" /Fd..\..\src\animate\..\..\..\lib\vc_lib\wxmsw25_animate.pdb /Yu"wx/wxprec.h" /Fp"vc_msw\wxprec_animatelib.pch" /I "..\..\src\animate\..\..\include" /D "WIN32" /D "_LIB" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x405
# ADD RSC /l 0x405
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\src\animate\..\..\..\lib\vc_lib\wxmsw25_animate.lib"
# ADD LIB32 /nologo /out:"..\..\src\animate\..\..\..\lib\vc_lib\wxmsw25_animate.lib"

!ELSEIF  "$(CFG)" == "animate - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\src\animate\..\..\..\lib\vc_lib"
# PROP BASE Intermediate_Dir "vc_mswud\animate"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\src\animate\..\..\..\lib\vc_lib"
# PROP Intermediate_Dir "vc_mswud\animate"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /W4 /MDd /Od /I "..\..\src\animate\..\..\..\include" /I "..\..\src\animate\..\..\..\lib\vc_lib\mswud" /Zi /Gm /GZ /Fd..\..\src\animate\..\..\..\lib\vc_lib\wxmsw25ud_animate.pdb /Yu"wx/wxprec.h" /Fp"vc_mswud\wxprec_animatelib.pch" /I "..\..\src\animate\..\..\include" /D "WIN32" /D "_LIB" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D wxUSE_UNICODE=1 /c
# ADD CPP /nologo /FD /W4 /MDd /Od /I "..\..\src\animate\..\..\..\include" /I "..\..\src\animate\..\..\..\lib\vc_lib\mswud" /Zi /Gm /GZ /Fd..\..\src\animate\..\..\..\lib\vc_lib\wxmsw25ud_animate.pdb /Yu"wx/wxprec.h" /Fp"vc_mswud\wxprec_animatelib.pch" /I "..\..\src\animate\..\..\include" /D "WIN32" /D "_LIB" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D wxUSE_UNICODE=1 /c
# ADD BASE RSC /l 0x405
# ADD RSC /l 0x405
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\src\animate\..\..\..\lib\vc_lib\wxmsw25ud_animate.lib"
# ADD LIB32 /nologo /out:"..\..\src\animate\..\..\..\lib\vc_lib\wxmsw25ud_animate.lib"

!ELSEIF  "$(CFG)" == "animate - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\src\animate\..\..\..\lib\vc_lib"
# PROP BASE Intermediate_Dir "vc_mswu\animate"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\src\animate\..\..\..\lib\vc_lib"
# PROP Intermediate_Dir "vc_mswu\animate"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /W4 /MD /O1 /I "..\..\src\animate\..\..\..\include" /I "..\..\src\animate\..\..\..\lib\vc_lib\mswu" /Fd..\..\src\animate\..\..\..\lib\vc_lib\wxmsw25u_animate.pdb /Yu"wx/wxprec.h" /Fp"vc_mswu\wxprec_animatelib.pch" /I "..\..\src\animate\..\..\include" /D "WIN32" /D "_LIB" /D "__WXMSW__" /D wxUSE_UNICODE=1 /c
# ADD CPP /nologo /FD /W4 /MD /O1 /I "..\..\src\animate\..\..\..\include" /I "..\..\src\animate\..\..\..\lib\vc_lib\mswu" /Fd..\..\src\animate\..\..\..\lib\vc_lib\wxmsw25u_animate.pdb /Yu"wx/wxprec.h" /Fp"vc_mswu\wxprec_animatelib.pch" /I "..\..\src\animate\..\..\include" /D "WIN32" /D "_LIB" /D "__WXMSW__" /D wxUSE_UNICODE=1 /c
# ADD BASE RSC /l 0x405
# ADD RSC /l 0x405
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\src\animate\..\..\..\lib\vc_lib\wxmsw25u_animate.lib"
# ADD LIB32 /nologo /out:"..\..\src\animate\..\..\..\lib\vc_lib\wxmsw25u_animate.lib"

!ELSEIF  "$(CFG)" == "animate - Win32 Universal Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\src\animate\..\..\..\lib\vc_lib"
# PROP BASE Intermediate_Dir "vc_mswunivd\animate"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\src\animate\..\..\..\lib\vc_lib"
# PROP Intermediate_Dir "vc_mswunivd\animate"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /W4 /MDd /Od /I "..\..\src\animate\..\..\..\include" /I "..\..\src\animate\..\..\..\lib\vc_lib\mswunivd" /Zi /Gm /GZ /Fd..\..\src\animate\..\..\..\lib\vc_lib\wxmswuniv25d_animate.pdb /Yu"wx/wxprec.h" /Fp"vc_mswunivd\wxprec_animatelib.pch" /I "..\..\src\animate\..\..\include" /D "WIN32" /D "_LIB" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WXDEBUG__" /c
# ADD CPP /nologo /FD /W4 /MDd /Od /I "..\..\src\animate\..\..\..\include" /I "..\..\src\animate\..\..\..\lib\vc_lib\mswunivd" /Zi /Gm /GZ /Fd..\..\src\animate\..\..\..\lib\vc_lib\wxmswuniv25d_animate.pdb /Yu"wx/wxprec.h" /Fp"vc_mswunivd\wxprec_animatelib.pch" /I "..\..\src\animate\..\..\include" /D "WIN32" /D "_LIB" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WXDEBUG__" /c
# ADD BASE RSC /l 0x405
# ADD RSC /l 0x405
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\src\animate\..\..\..\lib\vc_lib\wxmswuniv25d_animate.lib"
# ADD LIB32 /nologo /out:"..\..\src\animate\..\..\..\lib\vc_lib\wxmswuniv25d_animate.lib"

!ELSEIF  "$(CFG)" == "animate - Win32 Universal Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\src\animate\..\..\..\lib\vc_lib"
# PROP BASE Intermediate_Dir "vc_mswuniv\animate"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\src\animate\..\..\..\lib\vc_lib"
# PROP Intermediate_Dir "vc_mswuniv\animate"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /W4 /MD /O1 /I "..\..\src\animate\..\..\..\include" /I "..\..\src\animate\..\..\..\lib\vc_lib\mswuniv" /Fd..\..\src\animate\..\..\..\lib\vc_lib\wxmswuniv25_animate.pdb /Yu"wx/wxprec.h" /Fp"vc_mswuniv\wxprec_animatelib.pch" /I "..\..\src\animate\..\..\include" /D "WIN32" /D "_LIB" /D "__WXMSW__" /D "__WXUNIVERSAL__" /c
# ADD CPP /nologo /FD /W4 /MD /O1 /I "..\..\src\animate\..\..\..\include" /I "..\..\src\animate\..\..\..\lib\vc_lib\mswuniv" /Fd..\..\src\animate\..\..\..\lib\vc_lib\wxmswuniv25_animate.pdb /Yu"wx/wxprec.h" /Fp"vc_mswuniv\wxprec_animatelib.pch" /I "..\..\src\animate\..\..\include" /D "WIN32" /D "_LIB" /D "__WXMSW__" /D "__WXUNIVERSAL__" /c
# ADD BASE RSC /l 0x405
# ADD RSC /l 0x405
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\src\animate\..\..\..\lib\vc_lib\wxmswuniv25_animate.lib"
# ADD LIB32 /nologo /out:"..\..\src\animate\..\..\..\lib\vc_lib\wxmswuniv25_animate.lib"

!ELSEIF  "$(CFG)" == "animate - Win32 Universal Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\src\animate\..\..\..\lib\vc_lib"
# PROP BASE Intermediate_Dir "vc_mswunivud\animate"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\src\animate\..\..\..\lib\vc_lib"
# PROP Intermediate_Dir "vc_mswunivud\animate"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /W4 /MDd /Od /I "..\..\src\animate\..\..\..\include" /I "..\..\src\animate\..\..\..\lib\vc_lib\mswunivud" /Zi /Gm /GZ /Fd..\..\src\animate\..\..\..\lib\vc_lib\wxmswuniv25ud_animate.pdb /Yu"wx/wxprec.h" /Fp"vc_mswunivud\wxprec_animatelib.pch" /I "..\..\src\animate\..\..\include" /D "WIN32" /D "_LIB" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WXDEBUG__" /D wxUSE_UNICODE=1 /c
# ADD CPP /nologo /FD /W4 /MDd /Od /I "..\..\src\animate\..\..\..\include" /I "..\..\src\animate\..\..\..\lib\vc_lib\mswunivud" /Zi /Gm /GZ /Fd..\..\src\animate\..\..\..\lib\vc_lib\wxmswuniv25ud_animate.pdb /Yu"wx/wxprec.h" /Fp"vc_mswunivud\wxprec_animatelib.pch" /I "..\..\src\animate\..\..\include" /D "WIN32" /D "_LIB" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WXDEBUG__" /D wxUSE_UNICODE=1 /c
# ADD BASE RSC /l 0x405
# ADD RSC /l 0x405
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\src\animate\..\..\..\lib\vc_lib\wxmswuniv25ud_animate.lib"
# ADD LIB32 /nologo /out:"..\..\src\animate\..\..\..\lib\vc_lib\wxmswuniv25ud_animate.lib"

!ELSEIF  "$(CFG)" == "animate - Win32 Universal Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\src\animate\..\..\..\lib\vc_lib"
# PROP BASE Intermediate_Dir "vc_mswunivu\animate"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\src\animate\..\..\..\lib\vc_lib"
# PROP Intermediate_Dir "vc_mswunivu\animate"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /W4 /MD /O1 /I "..\..\src\animate\..\..\..\include" /I "..\..\src\animate\..\..\..\lib\vc_lib\mswunivu" /Fd..\..\src\animate\..\..\..\lib\vc_lib\wxmswuniv25u_animate.pdb /Yu"wx/wxprec.h" /Fp"vc_mswunivu\wxprec_animatelib.pch" /I "..\..\src\animate\..\..\include" /D "WIN32" /D "_LIB" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D wxUSE_UNICODE=1 /c
# ADD CPP /nologo /FD /W4 /MD /O1 /I "..\..\src\animate\..\..\..\include" /I "..\..\src\animate\..\..\..\lib\vc_lib\mswunivu" /Fd..\..\src\animate\..\..\..\lib\vc_lib\wxmswuniv25u_animate.pdb /Yu"wx/wxprec.h" /Fp"vc_mswunivu\wxprec_animatelib.pch" /I "..\..\src\animate\..\..\include" /D "WIN32" /D "_LIB" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D wxUSE_UNICODE=1 /c
# ADD BASE RSC /l 0x405
# ADD RSC /l 0x405
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\src\animate\..\..\..\lib\vc_lib\wxmswuniv25u_animate.lib"
# ADD LIB32 /nologo /out:"..\..\src\animate\..\..\..\lib\vc_lib\wxmswuniv25u_animate.lib"

!ELSEIF  "$(CFG)" == "animate - Win32 DLL Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\src\animate\..\..\..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vc_mswddll\animate"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\src\animate\..\..\..\lib\vc_dll"
# PROP Intermediate_Dir "vc_mswddll\animate"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /W4 /MDd /Od /I "..\..\src\animate\..\..\..\include" /I "..\..\src\animate\..\..\..\lib\vc_dll\mswd" /Yu"wx/wxprec.h" /Fp"vc_mswddll\wxprec_animatedll.pch" /Zi /Gm /GZ /Fd..\..\src\animate\..\..\..\lib\vc_dll\wxmsw250d_animate_vc_custom.pdb /I "..\..\src\animate\..\..\include" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "WXUSINGDLL" /D "WXMAKINGDLL_ANIMATE" /c
# ADD CPP /nologo /FD /W4 /MDd /Od /I "..\..\src\animate\..\..\..\include" /I "..\..\src\animate\..\..\..\lib\vc_dll\mswd" /Yu"wx/wxprec.h" /Fp"vc_mswddll\wxprec_animatedll.pch" /Zi /Gm /GZ /Fd..\..\src\animate\..\..\..\lib\vc_dll\wxmsw250d_animate_vc_custom.pdb /I "..\..\src\animate\..\..\include" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "WXUSINGDLL" /D "WXMAKINGDLL_ANIMATE" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "WXUSINGDLL" /D "WXMAKINGDLL_ANIMATE" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "WXUSINGDLL" /D "WXMAKINGDLL_ANIMATE" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /d "_DEBUG" /d "__WXMSW__" /d "__WXDEBUG__" /i "..\..\src\animate\..\..\..\include" /i "..\..\src\animate\..\..\..\lib\vc_dll\mswd" /i "..\..\src\animate\..\..\include" /d "WXUSINGDLL" /d WXMAKINGDLL_ANIMATE
# ADD RSC /l 0x405 /d "_DEBUG" /d "__WXMSW__" /d "__WXDEBUG__" /i "..\..\src\animate\..\..\..\include" /i "..\..\src\animate\..\..\..\lib\vc_dll\mswd" /i "..\..\src\animate\..\..\include" /d "WXUSINGDLL" /d WXMAKINGDLL_ANIMATE
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib wxmsw25d_core.lib wxbase25d.lib /nologo /dll /machine:i386 /out:"..\..\src\animate\..\..\..\lib\vc_dll\wxmsw250d_animate_vc_custom.dll" /libpath:"..\..\src\animate\..\..\..\lib\vc_dll" /implib:"..\..\src\animate\..\..\..\lib\vc_dll\wxmsw25d_animate.lib" /debug
# ADD LINK32 wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib wxmsw25d_core.lib wxbase25d.lib /nologo /dll /machine:i386 /out:"..\..\src\animate\..\..\..\lib\vc_dll\wxmsw250d_animate_vc_custom.dll" /libpath:"..\..\src\animate\..\..\..\lib\vc_dll" /implib:"..\..\src\animate\..\..\..\lib\vc_dll\wxmsw25d_animate.lib" /debug

!ELSEIF  "$(CFG)" == "animate - Win32 DLL Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\src\animate\..\..\..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vc_mswdll\animate"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\src\animate\..\..\..\lib\vc_dll"
# PROP Intermediate_Dir "vc_mswdll\animate"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /W4 /MD /O1 /I "..\..\src\animate\..\..\..\include" /I "..\..\src\animate\..\..\..\lib\vc_dll\msw" /Yu"wx/wxprec.h" /Fp"vc_mswdll\wxprec_animatedll.pch" /Fd..\..\src\animate\..\..\..\lib\vc_dll\wxmsw250_animate_vc_custom.pdb /I "..\..\src\animate\..\..\include" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "__WXMSW__" /D "WXUSINGDLL" /D "WXMAKINGDLL_ANIMATE" /c
# ADD CPP /nologo /FD /W4 /MD /O1 /I "..\..\src\animate\..\..\..\include" /I "..\..\src\animate\..\..\..\lib\vc_dll\msw" /Yu"wx/wxprec.h" /Fp"vc_mswdll\wxprec_animatedll.pch" /Fd..\..\src\animate\..\..\..\lib\vc_dll\wxmsw250_animate_vc_custom.pdb /I "..\..\src\animate\..\..\include" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "__WXMSW__" /D "WXUSINGDLL" /D "WXMAKINGDLL_ANIMATE" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "__WXMSW__" /D "WXUSINGDLL" /D "WXMAKINGDLL_ANIMATE" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "__WXMSW__" /D "WXUSINGDLL" /D "WXMAKINGDLL_ANIMATE" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /d "__WXMSW__" /i "..\..\src\animate\..\..\..\include" /i "..\..\src\animate\..\..\..\lib\vc_dll\msw" /i "..\..\src\animate\..\..\include" /d "WXUSINGDLL" /d WXMAKINGDLL_ANIMATE
# ADD RSC /l 0x405 /d "__WXMSW__" /i "..\..\src\animate\..\..\..\include" /i "..\..\src\animate\..\..\..\lib\vc_dll\msw" /i "..\..\src\animate\..\..\include" /d "WXUSINGDLL" /d WXMAKINGDLL_ANIMATE
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib wxmsw25_core.lib wxbase25.lib /nologo /dll /machine:i386 /out:"..\..\src\animate\..\..\..\lib\vc_dll\wxmsw250_animate_vc_custom.dll" /libpath:"..\..\src\animate\..\..\..\lib\vc_dll" /implib:"..\..\src\animate\..\..\..\lib\vc_dll\wxmsw25_animate.lib"
# ADD LINK32 wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib wxmsw25_core.lib wxbase25.lib /nologo /dll /machine:i386 /out:"..\..\src\animate\..\..\..\lib\vc_dll\wxmsw250_animate_vc_custom.dll" /libpath:"..\..\src\animate\..\..\..\lib\vc_dll" /implib:"..\..\src\animate\..\..\..\lib\vc_dll\wxmsw25_animate.lib"

!ELSEIF  "$(CFG)" == "animate - Win32 DLL Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\src\animate\..\..\..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vc_mswuddll\animate"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\src\animate\..\..\..\lib\vc_dll"
# PROP Intermediate_Dir "vc_mswuddll\animate"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /W4 /MDd /Od /I "..\..\src\animate\..\..\..\include" /I "..\..\src\animate\..\..\..\lib\vc_dll\mswud" /Yu"wx/wxprec.h" /Fp"vc_mswuddll\wxprec_animatedll.pch" /Zi /Gm /GZ /Fd..\..\src\animate\..\..\..\lib\vc_dll\wxmsw250ud_animate_vc_custom.pdb /I "..\..\src\animate\..\..\include" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D wxUSE_UNICODE=1 /D "WXUSINGDLL" /D "WXMAKINGDLL_ANIMATE" /c
# ADD CPP /nologo /FD /W4 /MDd /Od /I "..\..\src\animate\..\..\..\include" /I "..\..\src\animate\..\..\..\lib\vc_dll\mswud" /Yu"wx/wxprec.h" /Fp"vc_mswuddll\wxprec_animatedll.pch" /Zi /Gm /GZ /Fd..\..\src\animate\..\..\..\lib\vc_dll\wxmsw250ud_animate_vc_custom.pdb /I "..\..\src\animate\..\..\include" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D wxUSE_UNICODE=1 /D "WXUSINGDLL" /D "WXMAKINGDLL_ANIMATE" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D wxUSE_UNICODE=1 /D "WXUSINGDLL" /D "WXMAKINGDLL_ANIMATE" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D wxUSE_UNICODE=1 /D "WXUSINGDLL" /D "WXMAKINGDLL_ANIMATE" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /d "_DEBUG" /d "__WXMSW__" /d "__WXDEBUG__" /d wxUSE_UNICODE=1 /i "..\..\src\animate\..\..\..\include" /i "..\..\src\animate\..\..\..\lib\vc_dll\mswud" /i "..\..\src\animate\..\..\include" /d "WXUSINGDLL" /d WXMAKINGDLL_ANIMATE
# ADD RSC /l 0x405 /d "_DEBUG" /d "__WXMSW__" /d "__WXDEBUG__" /d wxUSE_UNICODE=1 /i "..\..\src\animate\..\..\..\include" /i "..\..\src\animate\..\..\..\lib\vc_dll\mswud" /i "..\..\src\animate\..\..\include" /d "WXUSINGDLL" /d WXMAKINGDLL_ANIMATE
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib wxmsw25ud_core.lib wxbase25ud.lib /nologo /dll /machine:i386 /out:"..\..\src\animate\..\..\..\lib\vc_dll\wxmsw250ud_animate_vc_custom.dll" /libpath:"..\..\src\animate\..\..\..\lib\vc_dll" /implib:"..\..\src\animate\..\..\..\lib\vc_dll\wxmsw25ud_animate.lib" /debug
# ADD LINK32 wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib wxmsw25ud_core.lib wxbase25ud.lib /nologo /dll /machine:i386 /out:"..\..\src\animate\..\..\..\lib\vc_dll\wxmsw250ud_animate_vc_custom.dll" /libpath:"..\..\src\animate\..\..\..\lib\vc_dll" /implib:"..\..\src\animate\..\..\..\lib\vc_dll\wxmsw25ud_animate.lib" /debug

!ELSEIF  "$(CFG)" == "animate - Win32 DLL Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\src\animate\..\..\..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vc_mswudll\animate"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\src\animate\..\..\..\lib\vc_dll"
# PROP Intermediate_Dir "vc_mswudll\animate"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /W4 /MD /O1 /I "..\..\src\animate\..\..\..\include" /I "..\..\src\animate\..\..\..\lib\vc_dll\mswu" /Yu"wx/wxprec.h" /Fp"vc_mswudll\wxprec_animatedll.pch" /Fd..\..\src\animate\..\..\..\lib\vc_dll\wxmsw250u_animate_vc_custom.pdb /I "..\..\src\animate\..\..\include" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "__WXMSW__" /D wxUSE_UNICODE=1 /D "WXUSINGDLL" /D "WXMAKINGDLL_ANIMATE" /c
# ADD CPP /nologo /FD /W4 /MD /O1 /I "..\..\src\animate\..\..\..\include" /I "..\..\src\animate\..\..\..\lib\vc_dll\mswu" /Yu"wx/wxprec.h" /Fp"vc_mswudll\wxprec_animatedll.pch" /Fd..\..\src\animate\..\..\..\lib\vc_dll\wxmsw250u_animate_vc_custom.pdb /I "..\..\src\animate\..\..\include" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "__WXMSW__" /D wxUSE_UNICODE=1 /D "WXUSINGDLL" /D "WXMAKINGDLL_ANIMATE" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "__WXMSW__" /D wxUSE_UNICODE=1 /D "WXUSINGDLL" /D "WXMAKINGDLL_ANIMATE" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "__WXMSW__" /D wxUSE_UNICODE=1 /D "WXUSINGDLL" /D "WXMAKINGDLL_ANIMATE" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /d "__WXMSW__" /d wxUSE_UNICODE=1 /i "..\..\src\animate\..\..\..\include" /i "..\..\src\animate\..\..\..\lib\vc_dll\mswu" /i "..\..\src\animate\..\..\include" /d "WXUSINGDLL" /d WXMAKINGDLL_ANIMATE
# ADD RSC /l 0x405 /d "__WXMSW__" /d wxUSE_UNICODE=1 /i "..\..\src\animate\..\..\..\include" /i "..\..\src\animate\..\..\..\lib\vc_dll\mswu" /i "..\..\src\animate\..\..\include" /d "WXUSINGDLL" /d WXMAKINGDLL_ANIMATE
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib wxmsw25u_core.lib wxbase25u.lib /nologo /dll /machine:i386 /out:"..\..\src\animate\..\..\..\lib\vc_dll\wxmsw250u_animate_vc_custom.dll" /libpath:"..\..\src\animate\..\..\..\lib\vc_dll" /implib:"..\..\src\animate\..\..\..\lib\vc_dll\wxmsw25u_animate.lib"
# ADD LINK32 wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib wxmsw25u_core.lib wxbase25u.lib /nologo /dll /machine:i386 /out:"..\..\src\animate\..\..\..\lib\vc_dll\wxmsw250u_animate_vc_custom.dll" /libpath:"..\..\src\animate\..\..\..\lib\vc_dll" /implib:"..\..\src\animate\..\..\..\lib\vc_dll\wxmsw25u_animate.lib"

!ELSEIF  "$(CFG)" == "animate - Win32 DLL Universal Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\src\animate\..\..\..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vc_mswunivddll\animate"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\src\animate\..\..\..\lib\vc_dll"
# PROP Intermediate_Dir "vc_mswunivddll\animate"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /W4 /MDd /Od /I "..\..\src\animate\..\..\..\include" /I "..\..\src\animate\..\..\..\lib\vc_dll\mswunivd" /Yu"wx/wxprec.h" /Fp"vc_mswunivddll\wxprec_animatedll.pch" /Zi /Gm /GZ /Fd..\..\src\animate\..\..\..\lib\vc_dll\wxmswuniv250d_animate_vc_custom.pdb /I "..\..\src\animate\..\..\include" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WXDEBUG__" /D "WXUSINGDLL" /D "WXMAKINGDLL_ANIMATE" /c
# ADD CPP /nologo /FD /W4 /MDd /Od /I "..\..\src\animate\..\..\..\include" /I "..\..\src\animate\..\..\..\lib\vc_dll\mswunivd" /Yu"wx/wxprec.h" /Fp"vc_mswunivddll\wxprec_animatedll.pch" /Zi /Gm /GZ /Fd..\..\src\animate\..\..\..\lib\vc_dll\wxmswuniv250d_animate_vc_custom.pdb /I "..\..\src\animate\..\..\include" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WXDEBUG__" /D "WXUSINGDLL" /D "WXMAKINGDLL_ANIMATE" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WXDEBUG__" /D "WXUSINGDLL" /D "WXMAKINGDLL_ANIMATE" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WXDEBUG__" /D "WXUSINGDLL" /D "WXMAKINGDLL_ANIMATE" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /d "_DEBUG" /d "__WXMSW__" /d "__WXUNIVERSAL__" /d "__WXDEBUG__" /i "..\..\src\animate\..\..\..\include" /i "..\..\src\animate\..\..\..\lib\vc_dll\mswunivd" /i "..\..\src\animate\..\..\include" /d "WXUSINGDLL" /d WXMAKINGDLL_ANIMATE
# ADD RSC /l 0x405 /d "_DEBUG" /d "__WXMSW__" /d "__WXUNIVERSAL__" /d "__WXDEBUG__" /i "..\..\src\animate\..\..\..\include" /i "..\..\src\animate\..\..\..\lib\vc_dll\mswunivd" /i "..\..\src\animate\..\..\include" /d "WXUSINGDLL" /d WXMAKINGDLL_ANIMATE
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib wxmswuniv25d_core.lib wxbase25d.lib /nologo /dll /machine:i386 /out:"..\..\src\animate\..\..\..\lib\vc_dll\wxmswuniv250d_animate_vc_custom.dll" /libpath:"..\..\src\animate\..\..\..\lib\vc_dll" /implib:"..\..\src\animate\..\..\..\lib\vc_dll\wxmswuniv25d_animate.lib" /debug
# ADD LINK32 wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib wxmswuniv25d_core.lib wxbase25d.lib /nologo /dll /machine:i386 /out:"..\..\src\animate\..\..\..\lib\vc_dll\wxmswuniv250d_animate_vc_custom.dll" /libpath:"..\..\src\animate\..\..\..\lib\vc_dll" /implib:"..\..\src\animate\..\..\..\lib\vc_dll\wxmswuniv25d_animate.lib" /debug

!ELSEIF  "$(CFG)" == "animate - Win32 DLL Universal Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\src\animate\..\..\..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vc_mswunivdll\animate"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\src\animate\..\..\..\lib\vc_dll"
# PROP Intermediate_Dir "vc_mswunivdll\animate"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /W4 /MD /O1 /I "..\..\src\animate\..\..\..\include" /I "..\..\src\animate\..\..\..\lib\vc_dll\mswuniv" /Yu"wx/wxprec.h" /Fp"vc_mswunivdll\wxprec_animatedll.pch" /Fd..\..\src\animate\..\..\..\lib\vc_dll\wxmswuniv250_animate_vc_custom.pdb /I "..\..\src\animate\..\..\include" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "WXUSINGDLL" /D "WXMAKINGDLL_ANIMATE" /c
# ADD CPP /nologo /FD /W4 /MD /O1 /I "..\..\src\animate\..\..\..\include" /I "..\..\src\animate\..\..\..\lib\vc_dll\mswuniv" /Yu"wx/wxprec.h" /Fp"vc_mswunivdll\wxprec_animatedll.pch" /Fd..\..\src\animate\..\..\..\lib\vc_dll\wxmswuniv250_animate_vc_custom.pdb /I "..\..\src\animate\..\..\include" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "WXUSINGDLL" /D "WXMAKINGDLL_ANIMATE" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "WXUSINGDLL" /D "WXMAKINGDLL_ANIMATE" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "WXUSINGDLL" /D "WXMAKINGDLL_ANIMATE" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /d "__WXMSW__" /d "__WXUNIVERSAL__" /i "..\..\src\animate\..\..\..\include" /i "..\..\src\animate\..\..\..\lib\vc_dll\mswuniv" /i "..\..\src\animate\..\..\include" /d "WXUSINGDLL" /d WXMAKINGDLL_ANIMATE
# ADD RSC /l 0x405 /d "__WXMSW__" /d "__WXUNIVERSAL__" /i "..\..\src\animate\..\..\..\include" /i "..\..\src\animate\..\..\..\lib\vc_dll\mswuniv" /i "..\..\src\animate\..\..\include" /d "WXUSINGDLL" /d WXMAKINGDLL_ANIMATE
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib wxmswuniv25_core.lib wxbase25.lib /nologo /dll /machine:i386 /out:"..\..\src\animate\..\..\..\lib\vc_dll\wxmswuniv250_animate_vc_custom.dll" /libpath:"..\..\src\animate\..\..\..\lib\vc_dll" /implib:"..\..\src\animate\..\..\..\lib\vc_dll\wxmswuniv25_animate.lib"
# ADD LINK32 wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib wxmswuniv25_core.lib wxbase25.lib /nologo /dll /machine:i386 /out:"..\..\src\animate\..\..\..\lib\vc_dll\wxmswuniv250_animate_vc_custom.dll" /libpath:"..\..\src\animate\..\..\..\lib\vc_dll" /implib:"..\..\src\animate\..\..\..\lib\vc_dll\wxmswuniv25_animate.lib"

!ELSEIF  "$(CFG)" == "animate - Win32 DLL Universal Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\src\animate\..\..\..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vc_mswunivuddll\animate"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\src\animate\..\..\..\lib\vc_dll"
# PROP Intermediate_Dir "vc_mswunivuddll\animate"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /W4 /MDd /Od /I "..\..\src\animate\..\..\..\include" /I "..\..\src\animate\..\..\..\lib\vc_dll\mswunivud" /Yu"wx/wxprec.h" /Fp"vc_mswunivuddll\wxprec_animatedll.pch" /Zi /Gm /GZ /Fd..\..\src\animate\..\..\..\lib\vc_dll\wxmswuniv250ud_animate_vc_custom.pdb /I "..\..\src\animate\..\..\include" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WXDEBUG__" /D wxUSE_UNICODE=1 /D "WXUSINGDLL" /D "WXMAKINGDLL_ANIMATE" /c
# ADD CPP /nologo /FD /W4 /MDd /Od /I "..\..\src\animate\..\..\..\include" /I "..\..\src\animate\..\..\..\lib\vc_dll\mswunivud" /Yu"wx/wxprec.h" /Fp"vc_mswunivuddll\wxprec_animatedll.pch" /Zi /Gm /GZ /Fd..\..\src\animate\..\..\..\lib\vc_dll\wxmswuniv250ud_animate_vc_custom.pdb /I "..\..\src\animate\..\..\include" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WXDEBUG__" /D wxUSE_UNICODE=1 /D "WXUSINGDLL" /D "WXMAKINGDLL_ANIMATE" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WXDEBUG__" /D wxUSE_UNICODE=1 /D "WXUSINGDLL" /D "WXMAKINGDLL_ANIMATE" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WXDEBUG__" /D wxUSE_UNICODE=1 /D "WXUSINGDLL" /D "WXMAKINGDLL_ANIMATE" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /d "_DEBUG" /d "__WXMSW__" /d "__WXUNIVERSAL__" /d "__WXDEBUG__" /d wxUSE_UNICODE=1 /i "..\..\src\animate\..\..\..\include" /i "..\..\src\animate\..\..\..\lib\vc_dll\mswunivud" /i "..\..\src\animate\..\..\include" /d "WXUSINGDLL" /d WXMAKINGDLL_ANIMATE
# ADD RSC /l 0x405 /d "_DEBUG" /d "__WXMSW__" /d "__WXUNIVERSAL__" /d "__WXDEBUG__" /d wxUSE_UNICODE=1 /i "..\..\src\animate\..\..\..\include" /i "..\..\src\animate\..\..\..\lib\vc_dll\mswunivud" /i "..\..\src\animate\..\..\include" /d "WXUSINGDLL" /d WXMAKINGDLL_ANIMATE
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib wxmswuniv25ud_core.lib wxbase25ud.lib /nologo /dll /machine:i386 /out:"..\..\src\animate\..\..\..\lib\vc_dll\wxmswuniv250ud_animate_vc_custom.dll" /libpath:"..\..\src\animate\..\..\..\lib\vc_dll" /implib:"..\..\src\animate\..\..\..\lib\vc_dll\wxmswuniv25ud_animate.lib" /debug
# ADD LINK32 wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib wxmswuniv25ud_core.lib wxbase25ud.lib /nologo /dll /machine:i386 /out:"..\..\src\animate\..\..\..\lib\vc_dll\wxmswuniv250ud_animate_vc_custom.dll" /libpath:"..\..\src\animate\..\..\..\lib\vc_dll" /implib:"..\..\src\animate\..\..\..\lib\vc_dll\wxmswuniv25ud_animate.lib" /debug

!ELSEIF  "$(CFG)" == "animate - Win32 DLL Universal Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\src\animate\..\..\..\lib\vc_dll"
# PROP BASE Intermediate_Dir "vc_mswunivudll\animate"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\src\animate\..\..\..\lib\vc_dll"
# PROP Intermediate_Dir "vc_mswunivudll\animate"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /W4 /MD /O1 /I "..\..\src\animate\..\..\..\include" /I "..\..\src\animate\..\..\..\lib\vc_dll\mswunivu" /Yu"wx/wxprec.h" /Fp"vc_mswunivudll\wxprec_animatedll.pch" /Fd..\..\src\animate\..\..\..\lib\vc_dll\wxmswuniv250u_animate_vc_custom.pdb /I "..\..\src\animate\..\..\include" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D wxUSE_UNICODE=1 /D "WXUSINGDLL" /D "WXMAKINGDLL_ANIMATE" /c
# ADD CPP /nologo /FD /W4 /MD /O1 /I "..\..\src\animate\..\..\..\include" /I "..\..\src\animate\..\..\..\lib\vc_dll\mswunivu" /Yu"wx/wxprec.h" /Fp"vc_mswunivudll\wxprec_animatedll.pch" /Fd..\..\src\animate\..\..\..\lib\vc_dll\wxmswuniv250u_animate_vc_custom.pdb /I "..\..\src\animate\..\..\include" /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D wxUSE_UNICODE=1 /D "WXUSINGDLL" /D "WXMAKINGDLL_ANIMATE" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D wxUSE_UNICODE=1 /D "WXUSINGDLL" /D "WXMAKINGDLL_ANIMATE" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_USRDLL" /D "DLL_EXPORTS" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D wxUSE_UNICODE=1 /D "WXUSINGDLL" /D "WXMAKINGDLL_ANIMATE" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /d "__WXMSW__" /d "__WXUNIVERSAL__" /d wxUSE_UNICODE=1 /i "..\..\src\animate\..\..\..\include" /i "..\..\src\animate\..\..\..\lib\vc_dll\mswunivu" /i "..\..\src\animate\..\..\include" /d "WXUSINGDLL" /d WXMAKINGDLL_ANIMATE
# ADD RSC /l 0x405 /d "__WXMSW__" /d "__WXUNIVERSAL__" /d wxUSE_UNICODE=1 /i "..\..\src\animate\..\..\..\include" /i "..\..\src\animate\..\..\..\lib\vc_dll\mswunivu" /i "..\..\src\animate\..\..\include" /d "WXUSINGDLL" /d WXMAKINGDLL_ANIMATE
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib wxmswuniv25u_core.lib wxbase25u.lib /nologo /dll /machine:i386 /out:"..\..\src\animate\..\..\..\lib\vc_dll\wxmswuniv250u_animate_vc_custom.dll" /libpath:"..\..\src\animate\..\..\..\lib\vc_dll" /implib:"..\..\src\animate\..\..\..\lib\vc_dll\wxmswuniv25u_animate.lib"
# ADD LINK32 wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib wxmswuniv25u_core.lib wxbase25u.lib /nologo /dll /machine:i386 /out:"..\..\src\animate\..\..\..\lib\vc_dll\wxmswuniv250u_animate_vc_custom.dll" /libpath:"..\..\src\animate\..\..\..\lib\vc_dll" /implib:"..\..\src\animate\..\..\..\lib\vc_dll\wxmswuniv25u_animate.lib"

!ENDIF

# Begin Target

# Name "animate - Win32 Debug"
# Name "animate - Win32 Release"
# Name "animate - Win32 Unicode Debug"
# Name "animate - Win32 Unicode Release"
# Name "animate - Win32 Universal Debug"
# Name "animate - Win32 Universal Release"
# Name "animate - Win32 Universal Unicode Debug"
# Name "animate - Win32 Universal Unicode Release"
# Name "animate - Win32 DLL Debug"
# Name "animate - Win32 DLL Release"
# Name "animate - Win32 DLL Unicode Debug"
# Name "animate - Win32 DLL Unicode Release"
# Name "animate - Win32 DLL Universal Debug"
# Name "animate - Win32 DLL Universal Release"
# Name "animate - Win32 DLL Universal Unicode Debug"
# Name "animate - Win32 DLL Universal Unicode Release"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=../../src/animate\animate.cpp
# End Source File
# Begin Source File

SOURCE=../../src/animate\..\..\..\src\msw\dummy.cpp
# ADD BASE CPP /Yc"wx/wxprec.h"
# ADD CPP /Yc"wx/wxprec.h"
# End Source File
# End Group
# End Target
# End Project

