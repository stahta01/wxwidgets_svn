#!#############################################################################
#! File:    vc6base.t
#! Purpose: tmake template file from which wxBase.dsp is generated by running
#!          tmake -t vc6base wxwin.pro -o wxBase.dsp
#! Author:  Vadim Zeitlin
#! Created: 27.11.99
#! Version: $Id$
#!#############################################################################
#${
    #! include the code which parses filelist.txt file and initializes
    #! %wxCommon, %wxGeneric and %wxMSW hashes.
    IncludeTemplate("filelist.t");

    #! now transform these hashes into $project tags
    foreach $file (sort keys %wxCommon) {
	next if $wxCommon{$file} !~ /\bB\b/;

	my $tag = $file =~ /\.c$/ ? "WXCSRCS" : "WXCOMMONSRCS";
	$project{$tag} .= $file . " "
    }

    foreach $file (sort keys %wxMSW) {
	next if $wxMSW{$file} !~ /\bB\b/;

	my $tag = $file =~ /\.c$/ ? "WXMSWCSRCS" : "WXMSWSRCS";
	$project{$tag} .= $file . " "
    }

    foreach $file (sort keys %wxBase) {
	my $tag = $file =~ /\.c$/ ? "WXCSRCS" : "WXCOMMONSRCS";
	$project{$tag} .= $file . " "
    }

    foreach $file (sort keys %wxWXINCLUDE) {
	next if $wxWXINCLUDE{$file} !~ /\bB\b/;
	next if $file =~ /setup.h/;
	next if $file =~ /[^.]*.cpp$/;
	$project{"WXHEADERS"} .= $file . " "
    }

    foreach $file (sort keys %wxMSWINCLUDE) {
	next if $wxMSWINCLUDE{$file} !~ /\bB\b/;
	next if $file =~ /setup0?.h/;
	$project{"WXMSWHEADERS"} .= $file . " "
    }

    $project{"WXVERSION"} = "232";
#$}
# Microsoft Developer Studio Project File - Name="wxBase" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104
# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=wxBase - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "wxBase.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "wxBase.mak" CFG="wxBase - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "wxBase - Win32 Release Unicode DLL" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "wxBase - Win32 Debug Unicode DLL" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "wxBase - Win32 Release Unicode" (based on "Win32 (x86) Static Library")
!MESSAGE "wxBase - Win32 Debug Unicode" (based on "Win32 (x86) Static Library")
!MESSAGE "wxBase - Win32 Release DLL" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "wxBase - Win32 Debug DLL" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "wxBase - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "wxBase - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "wxBase"
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "wxBase - Win32 Release Unicode DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "../lib"
# PROP BASE Intermediate_Dir "../BaseReleaseUnicodeDll"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../lib"
# PROP Intermediate_Dir "../BaseReleaseUnicodeDll"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W4 /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MT" /D "_USRDLL" /D "WXBASEDLL_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W4 /O2 /I "../lib/basedllu" /I "../include" /I "./regex" /I "./zlib" /D "NDEBUG" /D wxUSE_GUI=0 /D "WIN32" /D WINVER=0x400 /D "_MT" /D "WXMAKINGDLL" /D "_UNICODE" /D "UNICODE" /Yu"wx/wxprec.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /i "../include" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"../lib/wxbase#$ $text = $project{"WXVERSION"} . 'u.dll"'
# ADD LINK32 kernel32.lib user32.lib advapi32.lib wsock32.lib ../lib/zlib.lib ../lib/regex.lib /nologo /version:2.3 /dll /machine:I386 /out:"../lib/wxbase#$ $text = $project{"WXVERSION"} . 'u.dll"'

!ELSEIF  "$(CFG)" == "wxBase - Win32 Debug Unicode DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "../lib"
# PROP BASE Intermediate_Dir "../BaseDebugUnicodeDll"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../lib"
# PROP Intermediate_Dir "../BaseDebugUnicodeDll"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W4 /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MT" /D "_USRDLL" /D "WXBASEDLL_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MDd /W4 /Zi /Od /I "../lib/basedllud" /I "../include" /I "./regex" /I "./zlib" /D "_DEBUG" /D wxUSE_GUI=0 /D "WIN32" /D WINVER=0x400 /D "_MT" /D "WXMAKINGDLL" /D "_UNICODE" /D "UNICODE" /Yu"wx/wxprec.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /i "../include" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept /out:"../lib/wxbase#$ $text = $project{"WXVERSION"} . 'ud.dll"'
# ADD LINK32 kernel32.lib user32.lib advapi32.lib wsock32.lib ../lib/zlibd.lib ../lib/regexd.lib /nologo /version:2.2 /dll /debug /machine:I386 /pdbtype:sept /out:"../lib/wxbase#$ $text = $project{"WXVERSION"} . 'ud.dll"'

!ELSEIF  "$(CFG)" == "wxBase - Win32 Release Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "..\BaseReleaseUnicode"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "..\BaseReleaseUnicode"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W4 /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MT" /YX /FD /c
# ADD CPP /nologo /MD /W4 /O2 /I "../lib/baseu" /I "../include" /I "./regex" /I "./zlib" /D "NDEBUG" /D wxUSE_GUI=0 /D "WIN32" /D WINVER=0x400 /D "_MT" /D "_UNICODE" /D "UNICODE" /Yu"wx/wxprec.h" /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib\wxbaseu.lib"

!ELSEIF  "$(CFG)" == "wxBase - Win32 Debug Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "..\BaseDebugUnicode"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "..\BaseDebugUnicode"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W4 /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MT" /YX /FD /c
# ADD CPP /nologo /MDd /W4 /Zi /Od /I "../lib/baseud" /I "../include" /I "./regex" /I "./zlib" /D "_DEBUG" /D wxUSE_GUI=0 /D "WIN32" /D "__WXDEBUG__" /D WINVER=0x400 /D "_MT" /D "_UNICODE" /D "UNICODE" /Yu"wx/wxprec.h" /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"../lib/wxbase.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib\wxbaseud.lib"

!ELSEIF  "$(CFG)" == "wxBase - Win32 Release DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "../lib"
# PROP BASE Intermediate_Dir "../BaseReleaseDll"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../lib"
# PROP Intermediate_Dir "../BaseReleaseDll"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W4 /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MT" /D "_USRDLL" /D "WXBASEDLL_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W4 /O2 /I "../lib/basedll" /I "../include" /I "./regex" /I "./zlib" /D "NDEBUG" /D wxUSE_GUI=0 /D "WIN32" /D WINVER=0x400 /D "_MT" /D "WXMAKINGDLL" /Yu"wx/wxprec.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /i "../include" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"../lib/wxbase#$ $text = $project{"WXVERSION"} . '.dll"'
# ADD LINK32 kernel32.lib user32.lib advapi32.lib wsock32.lib ../lib/zlib.lib ../lib/regex.lib /nologo /version:2.3 /dll /machine:I386 /out:"../lib/wxbase#$ $text = $project{"WXVERSION"} . '.dll"'

!ELSEIF  "$(CFG)" == "wxBase - Win32 Debug DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "../lib"
# PROP BASE Intermediate_Dir "../BaseDebugDll"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../lib"
# PROP Intermediate_Dir "../BaseDebugDll"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W4 /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MT" /D "_USRDLL" /D "WXBASEDLL_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MDd /W4 /Zi /Od /I "../lib/basedlld" /I "../include" /I "./regex" /I "./zlib" /D "_DEBUG" /D wxUSE_GUI=0 /D "WIN32" /D WINVER=0x400 /D "_MT" /D "WXMAKINGDLL" /Yu"wx/wxprec.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /i "../include" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept /out:"../lib/wxbase#$ $text = $project{"WXVERSION"} . 'd.dll"'
# ADD LINK32 kernel32.lib user32.lib advapi32.lib wsock32.lib ../lib/zlibd.lib ../lib/regexd.lib /nologo /version:2.2 /dll /debug /machine:I386 /pdbtype:sept /out:"../lib/wxbase#$ $text = $project{"WXVERSION"} . 'd.dll"'

!ELSEIF  "$(CFG)" == "wxBase - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "..\BaseRelease"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "..\BaseRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W4 /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MT" /YX /FD /c
# ADD CPP /nologo /MD /W4 /O2 /I "../lib/base" /I "../include" /I "./regex" /I "./zlib" /D "NDEBUG" /D wxUSE_GUI=0 /D "WIN32" /D WINVER=0x400 /D "_MT" /Yu"wx/wxprec.h" /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib\wxbase.lib"

!ELSEIF  "$(CFG)" == "wxBase - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\lib"
# PROP BASE Intermediate_Dir "..\BaseDebug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\lib"
# PROP Intermediate_Dir "..\BaseDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W4 /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MT" /YX /FD /c
# ADD CPP /nologo /MDd /W4 /Zi /Od /I "../lib/based" /I "../include" /I "./regex" /I "./zlib" /D "_DEBUG" /D wxUSE_GUI=0 /D "WIN32" /D "__WXDEBUG__" /D WINVER=0x400 /D "_MT" /Yu"wx/wxprec.h" /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"../lib/wxbase.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib\wxbased.lib"

!ENDIF 

# Begin Target

# Name "wxBase - Win32 Release Unicode DLL"
# Name "wxBase - Win32 Debug Unicode DLL"
# Name "wxBase - Win32 Release Unicode"
# Name "wxBase - Win32 Debug Unicode"
# Name "wxBase - Win32 Release DLL"
# Name "wxBase - Win32 Debug DLL"
# Name "wxBase - Win32 Release"
# Name "wxBase - Win32 Debug"

# Begin Group "Common Files"

# PROP Default_Filter ""
#$ ExpandGlue("WXCOMMONSRCS", "# Begin Source File\n\nSOURCE=.\\common\\", "\n# End Source File\n# Begin Source File\n\nSOURCE=.\\common\\", "\n# End Source File\n");
#$ ExpandGlue("WXCSRCS", "# Begin Source File\n\nSOURCE=.\\common\\", "\n# SUBTRACT CPP /YX /Yc /Yu\n# End Source File\n# Begin Source File\n\nSOURCE=.\\common\\", "\n# SUBTRACT CPP /YX /Yc /Yu\n# End Source File\n");
# End Group
# Begin Group "MSW Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\msw\dummy.cpp
# ADD CPP /Yc"wx/wxprec.h"
# End Source File
#$ ExpandGlue("WXMSWSRCS", "# Begin Source File\n\nSOURCE=.\\msw\\", "\n# End Source File\n# Begin Source File\n\nSOURCE=.\\msw\\", "\n# End Source File\n");
#$ ExpandGlue("WXMSWCSRCS", "# Begin Source File\n\nSOURCE=.\\msw\\", "\n# SUBTRACT CPP /YX /Yc /Yu\n# End Source File\n# Begin Source File\n\nSOURCE=.\\msw\\", "\n# SUBTRACT CPP /YX /Yc /Yu\n# End Source File\n");
# End Group
# Begin Group "Headers"

# PROP Default_Filter ""
# Begin Group "Setup"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\wx\msw\setup0.h
!IF  "$(CFG)" == "wxBase - Win32 Release Unicode DLL"
# Begin Custom Build - Creating wx/setup.h from $(InputPath)
InputPath=..\include\wx\msw\setup0.h

"../lib/basedllu/wx/setup.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) ..\lib\basedllu\wx\setup.h

# End Custom Build
!ELSEIF  "$(CFG)" == "wxBase - Win32 Debug Unicode DLL"
# Begin Custom Build - Creating wx/setup.h from $(InputPath)
InputPath=..\include\wx\msw\setup0.h

"../lib/basedllud/wx/setup.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) ..\lib\basedllud\wx\setup.h
# End Custom Build
!ELSEIF  "$(CFG)" == "wxBase - Win32 Release Unicode"
# Begin Custom Build - Creating wx/setup.h from $(InputPath)
InputPath=..\include\wx\msw\setup0.h

"../lib/baseu/wx/setup.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) ..\lib\baseu\wx\setup.h

# End Custom Build
!ELSEIF  "$(CFG)" == "wxBase - Win32 Debug Unicode"
# Begin Custom Build - Creating wx/setup.h from $(InputPath)
InputPath=..\include\wx\msw\setup0.h

"../lib/baseud/wx/setup.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) ..\lib\baseud\wx\setup.h

# End Custom Build
!ELSEIF  "$(CFG)" == "wxBase - Win32 Release DLL"
# Begin Custom Build - Creating wx/setup.h from $(InputPath)
InputPath=..\include\wx\msw\setup0.h

"../lib/basedll/wx/setup.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) ..\lib\basedll\wx\setup.h

# End Custom Build
!ELSEIF  "$(CFG)" == "wxBase - Win32 Debug DLL"
# Begin Custom Build - Creating wx/setup.h from $(InputPath)
InputPath=..\include\wx\msw\setup0.h

"../lib/basedlld/wx/setup.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) ..\lib\basedlld\wx\setup.h
# End Custom Build
!ELSEIF  "$(CFG)" == "wxBase - Win32 Release"
# Begin Custom Build - Creating wx/setup.h from $(InputPath)
InputPath=..\include\wx\msw\setup0.h

"../lib/base/wx/setup.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) ..\lib\base\wx\setup.h

# End Custom Build
!ELSEIF  "$(CFG)" == "wxBase - Win32 Debug"
# Begin Custom Build - Creating wx/setup.h from $(InputPath)
InputPath=..\include\wx\msw\setup0.h

"../lib/based/wx/setup.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) ..\lib\based\wx\setup.h

# End Custom Build
!ENDIF 

# End Source File
# End Group
# Begin Group "Common"

# PROP Default_Filter ""
#$ ExpandGlue("WXHEADERS", "# Begin Source File\n\nSOURCE=..\\include\\wx\\", "\n# End Source File\n# Begin Source File\n\nSOURCE=..\\include\\wx\\", "\n# End Source File\n");
# End Group
# Begin Group "MSW"

# PROP Default_Filter ""
#$ ExpandGlue("WXMSWHEADERS", "# Begin Source File\n\nSOURCE=..\\include\\wx\\msw\\", "\n# End Source File\n# Begin Source File\n\nSOURCE=..\\include\\wx\\msw\\", "\n# End Source File\n");
# End Group
# End Group
# End Target
# End Project
#! vi: set sta ts=8 sw=4 noet nolist tw=0 ft=perl:
