@echo off
rem Zip up an external, generic + Windows distribution of wxWindows 2.0
set src=%wxwin
set dest=%src\deliver
set wise=0

Rem Set this to the required version
set version=2_1_14

if "%src" == "" goto usage
if "%dest" == "" goto usage
if "%1" == "-help" goto usage
if "%1" == "--help" goto usage
if "%1" == "/?" goto usage
if "%1" == "wiseonly" goto dounzip
if "%1" == "wise" set wise=1
echo About to archive an external wxWindows distribution:
echo   From   %src
echo   To     %dest
if "%wise" == "1" echo with WISE setup creation.
echo CTRL-C if this is not correct.
pause

erase %dest\wx*.zip
erase %dest\ogl3.zip
erase %dest\tex2rtf2.zip
erase %dest\jpeg.zip
erase %dest\tiff.zip
erase %dest\dialoged.zip
erase %dest\utils.zip

if direxist %dest\wx deltree /Y %dest\wx

cd %src
echo Zipping...

zip32 -@ %dest\wx%version%_gen.zip < %src\distrib\msw\generic.rsp
zip32 -@ -u %dest\wx%version%_gen.zip < %src\distrib\msw\makefile.rsp
zip32 -@ %dest\wx%version%_msw.zip < %src\distrib\msw\msw.rsp
zip32 -@ -u %dest\wx%version%_msw.zip < %src\distrib\msw\makefile.rsp
zip32 -@ %dest\wx%version%_gtk.zip < %src\distrib\msw\gtk.rsp
zip32 -@ -u %dest\wx%version%_gtk.zip < %src\distrib\msw\makefile.rsp
zip32 -@ %dest\wx%version%_stubs.zip < %src\distrib\msw\stubs.rsp
zip32 -@ %dest\wx%version%_mot.zip < %src\distrib\msw\motif.rsp
zip32 -@ -u %dest\wx%version%_mot.zip < %src\distrib\msw\makefile.rsp
Rem (Obsolete) zip32 -@ %dest\wx%version%_user.zip < %src\distrib\msw\user.rsp

zip32 -@ %dest\wx%version%_doc.zip < %src\distrib\msw\docsrc.rsp
zip32 -@ %dest\wx%version%_hlp.zip < %src\distrib\msw\wx_hlp.rsp
zip32 -@ %dest\wx%version%_htm.zip < %src\distrib\msw\wx_html.rsp
zip32 -@ %dest\wx%version%_pdf.zip < %src\distrib\msw\wx_pdf.rsp
zip32 -@ %dest\wx%version%_wrd.zip < %src\distrib\msw\wx_word.rsp

rem VC++ project files
zip32 -@ %dest\wx%version%_vc.zip < %src\distrib\msw\vc.rsp

rem BC++ project files
zip32 -@ %dest\wx%version%_bc.zip < %src\distrib\msw\bc.rsp

rem CodeWarrior project files
zip32 -@ %dest\wx%version%_cw.zip < %src\distrib\msw\cw.rsp

rem OGL 3
zip32 -@ %dest\ogl3.zip < %src\distrib\msw\ogl.rsp

rem MMedia
zip32 -@ %dest\mmedia.zip < %src\distrib\msw\mmedia.rsp

rem STC (Scintilla widget)
zip32 -@ %dest\stc.zip < %src\distrib\msw\stc.rsp

rem GLCanvas: obsolete, now in main library
rem zip32 -@ %dest\glcanvas.zip < %src\distrib\msw\glcanvas.rsp

rem Tex2RTF
zip32 -@ %dest\tex2rtf2.zip < %src\distrib\msw\tex2rtf.rsp

rem JPEG source
zip32 -@ %dest\jpeg.zip < %src\distrib\msw\jpeg.rsp

rem TIFF source
zip32 -@ %dest\tiff.zip < %src\distrib\msw\tiff.rsp

rem Dialog Editor source and binary
erase %dest\dialoged_source.zip
zip32 -@ %dest\dialoged_source.zip < %src\distrib\msw\dialoged.rsp
zip32 -j %dest\dialoged.zip %dest\dialoged_source.zip %src\bin\dialoged.exe %src\bin\dialoged.hlp %src\bin\dialoged.cnt
erase %dest\dialoged_source.zip

rem Misc. utils not in the main distribution
zip32 -@ %dest\utils.zip < %src\distrib\msw\utils.rsp
zip32 -@ -u %dest\utils.zip < %src\distrib\msw\utilmake.rsp

copy %src\docs\changes.txt %dest
copy %src\docs\msw\install.txt %dest\install_msw.txt
copy %src\docs\motif\install.txt %dest\install_motif.txt
copy %src\docs\gtk\install.txt %dest\install_gtk.txt
copy %src\docs\readme.txt %dest
copy %src\docs\release.txt %dest
copy %src\docs\motif\makewxmotif %dest
copy %src\docs\gtk\makewxgtk %dest

Rem Skip WISE setup if wise is 0.
if "%wise" == "0" goto end

:dounzip

cd %dest

rem Unzip the Windows files into 'wx'
mkdir %dest\wx

Rem After this change of directory, we're in the
Rem temporary 'wx' directory and not acting on
Rem the source wxWindows directory.
cd %dest\wx
unzip32 -o ..\wx%version%_msw.zip
unzip32 -o ..\wx%version%_gen.zip
unzip32 -o ..\wx%version%_vc.zip
unzip32 -o ..\wx%version%_bc.zip
unzip32 -o ..\wx%version%_hlp.zip
Rem Need Word file, for Remstar DB classes
unzip32 -o ..\wx%version%_wrd.zip
unzip32 -o ..\ogl3.zip
unzip32 -o ..\jpeg.zip
unzip32 -o ..\tiff.zip
unzip32 -o ..\tex2rtf2.zip

rem Now delete a few files that are unnecessary
attrib -R *
erase /Y BuildCVS.txt *.in *.spec *.guess *.sub mkinstalldirs modules install-sh *.sh descrip.mms
erase /Y configure samples\configure samples\*.in demos\configure demos\*.in contrib\configure contrib\*.in
erase /Y setup.h.in setup.h_vms
erase /SY Makefile.in
rem erase /Y docs\pdf\ogl.pdf
rem deltree /Y docs\html\ogl

rem Now copy some binary files to 'bin'
if not isdir bin mkdir bin
copy %src\bin\dialoged.exe bin
copy %src\bin\dbgview.* bin
copy %src\bin\life.exe bin
copy %src\docs\winhelp\dialoged.hlp %src\docs\winhelp\dialoged.cnt bin

rem Time to regenerate the WISE install script, wxwin2.wse.
rem NB: if you've changed wxwin2.wse using WISE, call splitwise.exe
rem from within distrib\msw, to split off wisetop.txt and wisebott.txt.
echo Calling 'makewise' to generate wxwin2.wse...
call %WXWIN\distrib\msw\makewise.bat

erase /Y %dest\setup.*

rem Now invoke WISE install on the new wxwin2.wse
set wisecmd="c:\Program Files\wise\wise32.exe" /C %WXWIN\distrib\msw\wxwin2.wse
echo Invoking %wisecmd...
start %wisecmd
pause Press any key to continue with the wxWindows distribution...
Rem ren %WXWIN\deliver\setup.EXE %WXWIN\deliver\setup_%version%.exe

cd %dest

Rem tidy up capitalisation of filenames
ren setup.EXE s
ren s setup.exe

ren setup.w02 s
ren s setup.w02

ren setup.w03 s
ren s setup.w03

ren setup.w04 s
ren s setup.w04

ren setup.w05 s
ren s setup.w05

ren setup.w06 s
ren s setup.w06

ren setup.w07 s
ren s setup.w07

rem Put all the setup files into a single zip archive.
zip32 wx%version%_setup.zip readme.txt setup.*

echo wxWindows archived.

goto end

:usage
echo DOS wxWindows distribution. Zips up all GTK/Motif/MSW/doc files,
echo and optionally makes a deliver\wx directory and a setup.exe
echo if you specify 'wise' (skipping the zipping if you use 'wiseonly').
echo.
echo Usage: zipdist [wise | wiseonly]

:end
