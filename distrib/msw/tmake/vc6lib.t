#!#############################################################################
#! File:    vc6lib.t
#! Purpose: tmake template file from which the VC++ 6.0 project file for
#!          building static wxWindows library wx.dsp is generated by running
#!                  tmake -t vc6lib wxwin.pro -o wx.dsp
#! Author:  Vadim Zeitlin
#! Created: 29.09.01
#! Version: $Id$
#!#############################################################################
#${
    #! include the code which parses filelist.txt file and initializes
    #! %wxCommon, %wxGeneric and %wxMSW hashes.
    IncludeTemplate("filelist.t");

    #! now transform these hashes into $project tags
    foreach $file (sort keys %wxGeneric) {
        next if $wxGeneric{$file} =~ /\b(PS|G|16|U)\b/;
        $project{"WXGENERICSRCS"} .= $file . " "
    }

    foreach $file (sort keys %wxCommon) {
        next if $wxCommon{$file} =~ /\b16\b/;

        my $tag = $file =~ /\.c$/ ? "WXCSRCS" : "WXCOMMONSRCS";
        $project{$tag} .= $file . " "
    }

    foreach $file (sort keys %wxMSW) {
        next if $wxMSW{$file} =~ /\b16\b/;

        my $tag;
        if ( $wxMSW{$file} =~ /\bO\b/ ) { $tag = "WXOLESRCS" }
        else { $tag = $file =~ /\.c$/ ? "WXMSWCSRCS" : "WXMSWSRCS" }
        $project{$tag} .= $file . " "
    }

    foreach $file (sort keys %wxHTML) {
        next if $wxHTML{$file} =~ /\b16\b/;
        $project{"WXHTMLSRCS"} .= $file . " "
    }

    foreach $file (sort keys %wxWXINCLUDE) {
        next if $file =~ /setup.h/;
        next if $file =~ /[^.]*.cpp$/;
        $project{"WXHEADERS"} .= $file . " "
    }

    foreach $file (sort keys %wxGENERICINCLUDE) {
        next if $wxGeneric{$file} =~ /\b(PS|G|16|U)\b/;
        $project{"WXGENERICHEADERS"} .= $file . " "
    }

    foreach $file (sort keys %wxMSWINCLUDE) {
        next if $file =~ /setup0?.h/;
        $project{"WXMSWHEADERS"} .= $file . " "
    }

    foreach $file (sort keys %wxHTMLINCLUDE) {
        $project{"WXHTMLHEADERS"} .= $file . " "
    }
#$}
# Microsoft Developer Studio Project File - Name="wx" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=wx - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "wx.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "wx.mak" CFG="wx - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "wx - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "wx - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "wx"
# PROP Scc_LocalPath ".."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "wx - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "../Release"
# PROP BASE Intermediate_Dir "../Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Release"
# PROP Intermediate_Dir "../Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W4 /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MT" /YX /FD /c
# ADD CPP /nologo /MD /W4 /O2 /I "../lib/msw" /I "../include" /I "./zlib" /I "./jpeg" /I "./png" /I "./regex" /I "./tiff" /D "WIN32" /D "NDEBUG" /D WINVER=0x0400 /D "STRICT" /Yu"wx/wxprec.h" /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib\wxmsw.lib"

!ELSEIF  "$(CFG)" == "wx - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "../Debug"
# PROP BASE Intermediate_Dir "../Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../Debug"
# PROP Intermediate_Dir "../Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W4 /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /W4 /Zi /Od /I "../lib/mswd" /I "../include" /I "./zlib" /I "./jpeg" /I "./png" /I "./regex" /I "./tiff" /D "WIN32" /D "_DEBUG" /D "__WXDEBUG__" /D WINVER=0x0400 /D "STRICT" /Yu"wx/wxprec.h" /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib\wxmswd.lib"

!ENDIF 

# Begin Target

# Name "wx - Win32 Release"
# Name "wx - Win32 Debug"
# Begin Group "Common Files"

# PROP Default_Filter ""
#$ ExpandGlue("WXCOMMONSRCS", "# Begin Source File\n\nSOURCE=.\\common\\", "\n# End Source File\n# Begin Source File\n\nSOURCE=.\\common\\", "\n# End Source File\n");
#$ ExpandGlue("WXCSRCS", "# Begin Source File\n\nSOURCE=.\\common\\", "\n# SUBTRACT CPP /YX /Yc /Yu\n# End Source File\n# Begin Source File\n\nSOURCE=.\\common\\", "\n# SUBTRACT CPP /YX /Yc /Yu\n# End Source File\n");
# Begin Source File

SOURCE=.\common\y_tab.c
# ADD CPP /W1 /D "USE_DEFINE"
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# End Group
# Begin Group "Generic Files"

# PROP Default_Filter ""
#$ ExpandGlue("WXGENERICSRCS", "# Begin Source File\n\nSOURCE=.\\generic\\", "\n# End Source File\n# Begin Source File\n\nSOURCE=.\\generic\\", "\n# End Source File\n");
# End Group
# Begin Group "wxHTML Files"

# PROP Default_Filter ""
#$ ExpandGlue("WXHTMLSRCS", "# Begin Source File\n\nSOURCE=.\\html\\", "\n# End Source File\n# Begin Source File\n\nSOURCE=.\\html\\", "\n# End Source File\n");
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
# Begin Group "OLE Files"

# PROP Default_Filter ""
#$ ExpandGlue("WXOLESRCS", "# Begin Source File\n\nSOURCE=.\\msw\\ole\\", "\n# End Source File\n# Begin Source File\n\nSOURCE=.\\msw\\ole\\", "\n# End Source File\n");
# End Group
# Begin Group "Headers"

# PROP Default_Filter ""
# Begin Group "Setup"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\wx\msw\setup0.h
!IF  "$(CFG)" == "wx - Win32 Release"
# Begin Custom Build - Creating wx/setup.h from $(InputPath)
InputPath=..\include\wx\msw\setup0.h

"../lib/msw/wx/setup.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) ..\lib\msw\wx\setup.h

# End Custom Build
!ELSEIF  "$(CFG)" == "wx - Win32 Debug"
# Begin Custom Build - Creating wx/setup.h from $(InputPath)
InputPath=..\include\wx\msw\setup0.h

"../lib/mswd/wx/setup.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) ..\lib\mswd\wx\setup.h

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
# Begin Group "Generic"

# PROP Default_Filter ""
#$ ExpandGlue("WXGENERICHEADERS", "# Begin Source File\n\nSOURCE=..\\include\\wx\\generic\\", "\n# End Source File\n# Begin Source File\n\nSOURCE=..\\include\\wx\\generic\\", "\n# End Source File\n");
# End Group
# Begin Group "HTML"

# PROP Default_Filter ""
#$ ExpandGlue("WXHTMLHEADERS", "# Begin Source File\n\nSOURCE=..\\include\\wx\\html\\", "\n# End Source File\n# Begin Source File\n\nSOURCE=..\\include\\wx\\html\\", "\n# End Source File\n");
# End Group
# End Group
# End Target
# End Project
#! vi: set sta ts=8 sw=4 noet nolist tw=0 ft=make:
