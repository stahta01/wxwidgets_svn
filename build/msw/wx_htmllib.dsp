# Microsoft Developer Studio Project File - Name="htmllib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=htmllib - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "wx_htmllib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "wx_htmllib.mak" CFG="htmllib - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "htmllib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "htmllib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "htmllib - Win32 Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "htmllib - Win32 Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE "htmllib - Win32 Universal Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "htmllib - Win32 Universal Release" (based on "Win32 (x86) Static Library")
!MESSAGE "htmllib - Win32 Universal Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "htmllib - Win32 Universal Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "htmllib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\lib\vc_mswd"
# PROP BASE Intermediate_Dir "vc_mswd\htmllib"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\lib\vc_mswd"
# PROP Intermediate_Dir "vc_mswd\htmllib"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /W4 /MDd /Od /I "..\..\include" /I "..\..\lib\vc_mswd" /I "..\..\src\tiff" /I "..\..\src\jpeg" /I "..\..\src\png" /I "..\..\src\zlib" /I "..\..\src\regex" /I "..\..\src\expat\lib" /Zi /Gm /GZ /Fd..\..\lib\vc_mswd\wxmsw25d_html.pdb /Yu"wx/wxprec.h" /Fp"vc_mswd\wxprec_htmllib.pch" /D "WIN32" /D "_LIB" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /c
# ADD CPP /nologo /FD /W4 /MDd /Od /I "..\..\include" /I "..\..\lib\vc_mswd" /I "..\..\src\tiff" /I "..\..\src\jpeg" /I "..\..\src\png" /I "..\..\src\zlib" /I "..\..\src\regex" /I "..\..\src\expat\lib" /Zi /Gm /GZ /Fd..\..\lib\vc_mswd\wxmsw25d_html.pdb /Yu"wx/wxprec.h" /Fp"vc_mswd\wxprec_htmllib.pch" /D "WIN32" /D "_LIB" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /c
# ADD BASE RSC /l 0x405
# ADD RSC /l 0x405
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\lib\vc_mswd\wxmsw25d_html.lib"
# ADD LIB32 /nologo /out:"..\..\lib\vc_mswd\wxmsw25d_html.lib"

!ELSEIF  "$(CFG)" == "htmllib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\lib\vc_msw"
# PROP BASE Intermediate_Dir "vc_msw\htmllib"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\lib\vc_msw"
# PROP Intermediate_Dir "vc_msw\htmllib"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /W4 /MD /O1 /I "..\..\include" /I "..\..\lib\vc_msw" /I "..\..\src\tiff" /I "..\..\src\jpeg" /I "..\..\src\png" /I "..\..\src\zlib" /I "..\..\src\regex" /I "..\..\src\expat\lib" /Fd..\..\lib\vc_msw\wxmsw25_html.pdb /Yu"wx/wxprec.h" /Fp"vc_msw\wxprec_htmllib.pch" /D "WIN32" /D "_LIB" /D "__WXMSW__" /c
# ADD CPP /nologo /FD /W4 /MD /O1 /I "..\..\include" /I "..\..\lib\vc_msw" /I "..\..\src\tiff" /I "..\..\src\jpeg" /I "..\..\src\png" /I "..\..\src\zlib" /I "..\..\src\regex" /I "..\..\src\expat\lib" /Fd..\..\lib\vc_msw\wxmsw25_html.pdb /Yu"wx/wxprec.h" /Fp"vc_msw\wxprec_htmllib.pch" /D "WIN32" /D "_LIB" /D "__WXMSW__" /c
# ADD BASE RSC /l 0x405
# ADD RSC /l 0x405
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\lib\vc_msw\wxmsw25_html.lib"
# ADD LIB32 /nologo /out:"..\..\lib\vc_msw\wxmsw25_html.lib"

!ELSEIF  "$(CFG)" == "htmllib - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\lib\vc_mswud"
# PROP BASE Intermediate_Dir "vc_mswud\htmllib"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\lib\vc_mswud"
# PROP Intermediate_Dir "vc_mswud\htmllib"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /W4 /MDd /Od /I "..\..\include" /I "..\..\lib\vc_mswud" /I "..\..\src\tiff" /I "..\..\src\jpeg" /I "..\..\src\png" /I "..\..\src\zlib" /I "..\..\src\regex" /I "..\..\src\expat\lib" /Zi /Gm /GZ /Fd..\..\lib\vc_mswud\wxmsw25ud_html.pdb /Yu"wx/wxprec.h" /Fp"vc_mswud\wxprec_htmllib.pch" /D "WIN32" /D "_LIB" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D wxUSE_UNICODE=1 /c
# ADD CPP /nologo /FD /W4 /MDd /Od /I "..\..\include" /I "..\..\lib\vc_mswud" /I "..\..\src\tiff" /I "..\..\src\jpeg" /I "..\..\src\png" /I "..\..\src\zlib" /I "..\..\src\regex" /I "..\..\src\expat\lib" /Zi /Gm /GZ /Fd..\..\lib\vc_mswud\wxmsw25ud_html.pdb /Yu"wx/wxprec.h" /Fp"vc_mswud\wxprec_htmllib.pch" /D "WIN32" /D "_LIB" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D wxUSE_UNICODE=1 /c
# ADD BASE RSC /l 0x405
# ADD RSC /l 0x405
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\lib\vc_mswud\wxmsw25ud_html.lib"
# ADD LIB32 /nologo /out:"..\..\lib\vc_mswud\wxmsw25ud_html.lib"

!ELSEIF  "$(CFG)" == "htmllib - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\lib\vc_mswu"
# PROP BASE Intermediate_Dir "vc_mswu\htmllib"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\lib\vc_mswu"
# PROP Intermediate_Dir "vc_mswu\htmllib"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /W4 /MD /O1 /I "..\..\include" /I "..\..\lib\vc_mswu" /I "..\..\src\tiff" /I "..\..\src\jpeg" /I "..\..\src\png" /I "..\..\src\zlib" /I "..\..\src\regex" /I "..\..\src\expat\lib" /Fd..\..\lib\vc_mswu\wxmsw25u_html.pdb /Yu"wx/wxprec.h" /Fp"vc_mswu\wxprec_htmllib.pch" /D "WIN32" /D "_LIB" /D "__WXMSW__" /D wxUSE_UNICODE=1 /c
# ADD CPP /nologo /FD /W4 /MD /O1 /I "..\..\include" /I "..\..\lib\vc_mswu" /I "..\..\src\tiff" /I "..\..\src\jpeg" /I "..\..\src\png" /I "..\..\src\zlib" /I "..\..\src\regex" /I "..\..\src\expat\lib" /Fd..\..\lib\vc_mswu\wxmsw25u_html.pdb /Yu"wx/wxprec.h" /Fp"vc_mswu\wxprec_htmllib.pch" /D "WIN32" /D "_LIB" /D "__WXMSW__" /D wxUSE_UNICODE=1 /c
# ADD BASE RSC /l 0x405
# ADD RSC /l 0x405
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\lib\vc_mswu\wxmsw25u_html.lib"
# ADD LIB32 /nologo /out:"..\..\lib\vc_mswu\wxmsw25u_html.lib"

!ELSEIF  "$(CFG)" == "htmllib - Win32 Universal Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\lib\vc_mswunivd"
# PROP BASE Intermediate_Dir "vc_mswunivd\htmllib"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\lib\vc_mswunivd"
# PROP Intermediate_Dir "vc_mswunivd\htmllib"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /W4 /MDd /Od /I "..\..\include" /I "..\..\lib\vc_mswunivd" /I "..\..\src\tiff" /I "..\..\src\jpeg" /I "..\..\src\png" /I "..\..\src\zlib" /I "..\..\src\regex" /I "..\..\src\expat\lib" /Zi /Gm /GZ /Fd..\..\lib\vc_mswunivd\wxmswuniv25d_html.pdb /Yu"wx/wxprec.h" /Fp"vc_mswunivd\wxprec_htmllib.pch" /D "WIN32" /D "_LIB" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WXDEBUG__" /c
# ADD CPP /nologo /FD /W4 /MDd /Od /I "..\..\include" /I "..\..\lib\vc_mswunivd" /I "..\..\src\tiff" /I "..\..\src\jpeg" /I "..\..\src\png" /I "..\..\src\zlib" /I "..\..\src\regex" /I "..\..\src\expat\lib" /Zi /Gm /GZ /Fd..\..\lib\vc_mswunivd\wxmswuniv25d_html.pdb /Yu"wx/wxprec.h" /Fp"vc_mswunivd\wxprec_htmllib.pch" /D "WIN32" /D "_LIB" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WXDEBUG__" /c
# ADD BASE RSC /l 0x405
# ADD RSC /l 0x405
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\lib\vc_mswunivd\wxmswuniv25d_html.lib"
# ADD LIB32 /nologo /out:"..\..\lib\vc_mswunivd\wxmswuniv25d_html.lib"

!ELSEIF  "$(CFG)" == "htmllib - Win32 Universal Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\lib\vc_mswuniv"
# PROP BASE Intermediate_Dir "vc_mswuniv\htmllib"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\lib\vc_mswuniv"
# PROP Intermediate_Dir "vc_mswuniv\htmllib"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /W4 /MD /O1 /I "..\..\include" /I "..\..\lib\vc_mswuniv" /I "..\..\src\tiff" /I "..\..\src\jpeg" /I "..\..\src\png" /I "..\..\src\zlib" /I "..\..\src\regex" /I "..\..\src\expat\lib" /Fd..\..\lib\vc_mswuniv\wxmswuniv25_html.pdb /Yu"wx/wxprec.h" /Fp"vc_mswuniv\wxprec_htmllib.pch" /D "WIN32" /D "_LIB" /D "__WXMSW__" /D "__WXUNIVERSAL__" /c
# ADD CPP /nologo /FD /W4 /MD /O1 /I "..\..\include" /I "..\..\lib\vc_mswuniv" /I "..\..\src\tiff" /I "..\..\src\jpeg" /I "..\..\src\png" /I "..\..\src\zlib" /I "..\..\src\regex" /I "..\..\src\expat\lib" /Fd..\..\lib\vc_mswuniv\wxmswuniv25_html.pdb /Yu"wx/wxprec.h" /Fp"vc_mswuniv\wxprec_htmllib.pch" /D "WIN32" /D "_LIB" /D "__WXMSW__" /D "__WXUNIVERSAL__" /c
# ADD BASE RSC /l 0x405
# ADD RSC /l 0x405
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\lib\vc_mswuniv\wxmswuniv25_html.lib"
# ADD LIB32 /nologo /out:"..\..\lib\vc_mswuniv\wxmswuniv25_html.lib"

!ELSEIF  "$(CFG)" == "htmllib - Win32 Universal Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\..\lib\vc_mswunivud"
# PROP BASE Intermediate_Dir "vc_mswunivud\htmllib"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\lib\vc_mswunivud"
# PROP Intermediate_Dir "vc_mswunivud\htmllib"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /W4 /MDd /Od /I "..\..\include" /I "..\..\lib\vc_mswunivud" /I "..\..\src\tiff" /I "..\..\src\jpeg" /I "..\..\src\png" /I "..\..\src\zlib" /I "..\..\src\regex" /I "..\..\src\expat\lib" /Zi /Gm /GZ /Fd..\..\lib\vc_mswunivud\wxmswuniv25ud_html.pdb /Yu"wx/wxprec.h" /Fp"vc_mswunivud\wxprec_htmllib.pch" /D "WIN32" /D "_LIB" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WXDEBUG__" /D wxUSE_UNICODE=1 /c
# ADD CPP /nologo /FD /W4 /MDd /Od /I "..\..\include" /I "..\..\lib\vc_mswunivud" /I "..\..\src\tiff" /I "..\..\src\jpeg" /I "..\..\src\png" /I "..\..\src\zlib" /I "..\..\src\regex" /I "..\..\src\expat\lib" /Zi /Gm /GZ /Fd..\..\lib\vc_mswunivud\wxmswuniv25ud_html.pdb /Yu"wx/wxprec.h" /Fp"vc_mswunivud\wxprec_htmllib.pch" /D "WIN32" /D "_LIB" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WXDEBUG__" /D wxUSE_UNICODE=1 /c
# ADD BASE RSC /l 0x405
# ADD RSC /l 0x405
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\lib\vc_mswunivud\wxmswuniv25ud_html.lib"
# ADD LIB32 /nologo /out:"..\..\lib\vc_mswunivud\wxmswuniv25ud_html.lib"

!ELSEIF  "$(CFG)" == "htmllib - Win32 Universal Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\..\lib\vc_mswunivu"
# PROP BASE Intermediate_Dir "vc_mswunivu\htmllib"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\lib\vc_mswunivu"
# PROP Intermediate_Dir "vc_mswunivu\htmllib"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /W4 /MD /O1 /I "..\..\include" /I "..\..\lib\vc_mswunivu" /I "..\..\src\tiff" /I "..\..\src\jpeg" /I "..\..\src\png" /I "..\..\src\zlib" /I "..\..\src\regex" /I "..\..\src\expat\lib" /Fd..\..\lib\vc_mswunivu\wxmswuniv25u_html.pdb /Yu"wx/wxprec.h" /Fp"vc_mswunivu\wxprec_htmllib.pch" /D "WIN32" /D "_LIB" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D wxUSE_UNICODE=1 /c
# ADD CPP /nologo /FD /W4 /MD /O1 /I "..\..\include" /I "..\..\lib\vc_mswunivu" /I "..\..\src\tiff" /I "..\..\src\jpeg" /I "..\..\src\png" /I "..\..\src\zlib" /I "..\..\src\regex" /I "..\..\src\expat\lib" /Fd..\..\lib\vc_mswunivu\wxmswuniv25u_html.pdb /Yu"wx/wxprec.h" /Fp"vc_mswunivu\wxprec_htmllib.pch" /D "WIN32" /D "_LIB" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D wxUSE_UNICODE=1 /c
# ADD BASE RSC /l 0x405
# ADD RSC /l 0x405
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\lib\vc_mswunivu\wxmswuniv25u_html.lib"
# ADD LIB32 /nologo /out:"..\..\lib\vc_mswunivu\wxmswuniv25u_html.lib"

!ENDIF

# Begin Target

# Name "htmllib - Win32 Debug"
# Name "htmllib - Win32 Release"
# Name "htmllib - Win32 Unicode Debug"
# Name "htmllib - Win32 Unicode Release"
# Name "htmllib - Win32 Universal Debug"
# Name "htmllib - Win32 Universal Release"
# Name "htmllib - Win32 Universal Unicode Debug"
# Name "htmllib - Win32 Universal Unicode Release"
# Begin Group "Common Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\include\wx\html\forcelnk.h
# End Source File
# Begin Source File

SOURCE=..\..\include\wx\html\helpctrl.h
# End Source File
# Begin Source File

SOURCE=..\..\include\wx\html\helpdata.h
# End Source File
# Begin Source File

SOURCE=..\..\include\wx\html\helpfrm.h
# End Source File
# Begin Source File

SOURCE=..\..\include\wx\html\htmlcell.h
# End Source File
# Begin Source File

SOURCE=..\..\include\wx\html\htmldefs.h
# End Source File
# Begin Source File

SOURCE=..\..\include\wx\html\htmlfilt.h
# End Source File
# Begin Source File

SOURCE=..\..\include\wx\html\htmlpars.h
# End Source File
# Begin Source File

SOURCE=..\..\include\wx\html\htmlproc.h
# End Source File
# Begin Source File

SOURCE=..\..\include\wx\html\htmltag.h
# End Source File
# Begin Source File

SOURCE=..\..\include\wx\html\htmlwin.h
# End Source File
# Begin Source File

SOURCE=..\..\include\wx\html\htmprint.h
# End Source File
# Begin Source File

SOURCE=..\..\include\wx\html\m_templ.h
# End Source File
# Begin Source File

SOURCE=..\..\include\wx\html\winpars.h
# End Source File
# Begin Source File

SOURCE=..\..\include\wx\htmllbox.h
# End Source File
# Begin Source File

SOURCE=..\..\include\wx\wxhtml.h
# End Source File
# End Group
# Begin Group "wxHTML Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\html\helpctrl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\html\helpdata.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\html\helpfrm.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\html\htmlcell.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\html\htmlfilt.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\html\htmlpars.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\html\htmltag.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\html\htmlwin.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\html\htmprint.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\html\m_dflist.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\html\m_fonts.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\html\m_hline.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\html\m_image.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\html\m_layout.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\html\m_links.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\html\m_list.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\html\m_pre.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\html\m_style.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\html\m_tables.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\html\winpars.cpp
# End Source File
# End Group
# Begin Group "Generic Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\generic\htmllbox.cpp
# End Source File
# End Group
# Begin Group "MSW Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\msw\dummy.cpp
# ADD BASE CPP /Yc"wx/wxprec.h"
# ADD CPP /Yc"wx/wxprec.h"
# End Source File
# Begin Source File

SOURCE=..\..\src\msw\helpbest.cpp
# End Source File
# End Group
# End Target
# End Project

