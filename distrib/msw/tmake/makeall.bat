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

echo Generating for Visual C++ 4.0...
%TM% -t vc wxwin.pro -o makefile.vc
echo Generating for Borland C++ (32 bits)...
%TM% -t b32 wxwin.pro -o makefile.b32
echo Generating for Visual C++ 1.52...
%TM% -t dos wxwin.pro -o makefile.dos
echo Generating for Borland C++ (16 bits)...
%TM% -t bcc wxwin.pro -o makefile.bcc
echo Generating for Cygwin/Mingw32
%TM% -t g95 wxwin.pro -o makefile.g95
echo Generating for Symantec C++...
%TM% -t sc wxwin.pro -o makefile.sc
echo Generating for Watcom C++...
%TM% -t wat wxwin.pro -o makefile.wat

:end
