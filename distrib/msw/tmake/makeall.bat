@echo off
rem File:      makeall.bat
rem Purpose:   create wxWindows makefiles for all compilers
rem Author:    Vadim Zeitlin
rem Created:   14.07.99
rem Copyright: (c) 1999 Vadim Zeitlin
rem Version:   $Id$
rem
rem Makefile to create the makefiles for all compilers from the templates using
rem tmake. The environment variable WX or WXWIN should be defined and contain
rem the root directory of wxWindows installation. TMAKE program should be in
rem path or, alternatively, TMAKE environment variable should be set.

rem set tmakecmd=E:\Perl\5.00471\bin\MSWin32-x86\perl.exe g:\Unix\tmake\bin\tmake
set tmakecmd=d:\programs\tmake\bin\tmake

if "x%WX%" == "x" goto skip
set WXDIR=%WX%
goto ok

:skip
if "x%WXWIN%" == "x" goto no_wx
set WXDIR=%WXWIN%
goto ok

:no_wx
echo "Please set WX environment variable!"
goto end

:ok
set TM=tmake
if "x%TMAKE%" == "x" goto skip2
set TM=%TMAKE%

:skip2

rem this loop just doesn't want to work under command.com and cmd.exe and
rem 4nt.exe, so I preferred to unroll it.
rem for %%c in (b32 bcc dos g95 sc vc wat) %TM% -t %c% wxwin.pro -o %WXDIR%\src\msw\makefile.%c%

echo Generating for for Visual C++ 6.0 wxBase C++ (32 bits)...
%tmakecmd% -t basevc wxwin.pro -o %WXDIR%\src\msw\makebase.vc
echo Generating for for Visual C++ 6.0 wxWindows.dsp
%tmakecmd% -t vc6msw wxwin.pro -o %WXDIR%\src\wxWindows.dsp
echo Generating for for Visual C++ 6.0 wxUniv.dsp
%tmakecmd% -t vc6univ wxwin.pro -o %WXDIR%\src\wxUniv.dsp
echo Generating for for Visual C++ 6.0 wxBase.dsp
%tmakecmd% -t vc6base wxwin.pro -o %WXDIR%\src\wxBase.dsp
echo Generating for Visual C++ 4.0...
%tmakecmd% -t vc wxwin.pro -o %WXDIR%\src\msw\makefile.vc
echo Generating for Borland C++ (32 bits)...
%tmakecmd% -t b32 wxwin.pro -o %WXDIR%\src\msw\makefile.b32
echo Generating for Visual C++ 1.52...
%tmakecmd% -t dos wxwin.pro -o %WXDIR%\src\msw\makefile.dos
echo Generating for Borland C++ (16 bits)...
%tmakecmd% -t bcc wxwin.pro -o %WXDIR%\src\msw\makefile.bcc
echo Generating for Cygwin/Mingw32
%tmakecmd% -t g95 wxwin.pro -o %WXDIR%\src\msw\makefile.g95
echo Generating for Symantec C++...
%tmakecmd% -t sc wxwin.pro -o %WXDIR%\src\msw\makefile.sc
echo Generating for Watcom C++...
%tmakecmd% -t wat wxwin.pro -o %WXDIR%\src\msw\makefile.wat
echo Generating for Unix and Configure...
%tmakecmd% -t gtk wxwin.pro -o %WXDIR%\src\gtk\files.lst
%tmakecmd% -t univ wxwin.pro -o %WXDIR%\src\univ\files.lst
%tmakecmd% -t msw wxwin.pro -o %WXDIR%\src\msw\files.lst
%tmakecmd% -t mac wxwin.pro -o %WXDIR%\src\mac\files.lst
%tmakecmd% -t motif wxwin.pro -o %WXDIR%\src\motif\files.lst
%tmakecmd% -t os2 wxwin.pro -o %WXDIR%\src\os2\files.lst
:end
