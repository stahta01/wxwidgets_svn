@echo off
rem Zip up an external, generic + Windows distribution of wxWindows 2
rem using Inno Setup + ScriptMaker
set src=%wxwin
set dest=%src\deliver
set webfiles=c:\wx2dev\wxWebSite
set inno=0

Rem Set this to the required version
set version=2.3.2

if "%src" == "" goto usage
if "%dest" == "" goto usage
if "%1" == "-help" goto usage
if "%1" == "--help" goto usage
if "%1" == "/?" goto usage
if "%1" == "innoonly" goto dounzip
if "%1" == "inno" set inno=1
if "%1" == "tidyup" goto tidyup
echo About to archive an external wxWindows distribution:
echo   From   %src
echo   To     %dest
if "%inno" == "1" echo with Inno Setup creation.
echo CTRL-C if this is not correct.
pause

erase %dest\wx*.zip
erase %dest\*.htb
erase %dest\ogl3*.zip
erase %dest\contrib*.zip
erase %dest\tex2rtf2*.zip
erase %dest\mmedia*.zip
erase %dest\jpeg*.zip
erase %dest\tiff*.zip
erase %dest\dialoged*.zip
erase %dest\utils*.zip
erase %dest\extradoc*.zip
erase %dest\stc*.zip
erase %dest\*-win32*.zip
erase %dest\setup*.*
erase %dest\*.txt
erase %dest\make*

if direxist %dest\wx erase /sxyz %dest\wx\
if not direxist %dest mkdir %dest

# Copy FAQ from wxWebSite CVS
if not direxist %webfiles% echo Error - %webfiles% does not exist
if not direxist %webfiles% goto end
echo Copying FAQ and other files from %webfiles
copy %webfiles%\site\faq*.htm %src\docs\html
copy %webfiles%\site\platform.htm %src\docs\html

cd %src
echo Zipping...

Rem Zip up the complete wxMSW-xxx.zip file
Rem NO, do it by unarchiving component files, deleting/copying some files, then
Rem rearchiving.
Rem zip32 -@ %dest\wxMSW-%version%.zip < %src\distrib\msw\generic.rsp
Rem zip32 -@ -u %dest\wxMSW-%version%.zip < %src\distrib\msw\msw.rsp
Rem zip32 -@ -u %dest\wxMSW-%version%.zip < %src\distrib\msw\wx_chm.rsp
Rem zip32 -@ -u %dest\wxMSW-%version%.zip  < %src\distrib\msw\jpeg.rsp
Rem zip32 -@ -u %dest\wxMSW-%version%.zip < %src\distrib\msw\tiff.rsp

echo Zipping wxMac distribution
zip32 -@ %dest%\wxMac-%version%.zip < %src%\distrib\msw\generic.rsp
zip32 -u -@ %dest%\wxMac-%version%.zip < %src%\distrib\msw\mac.rsp
zip32 -u -@ %dest%\wxMac-%version%.zip < %src%\distrib\msw\cw.rsp
zip32 -u -@ %dest%\wxMac-%version%.zip < %src%\distrib\msw\tiff.rsp
zip32 -u -@ %dest%\wxMac-%version%.zip < %src%\distrib\msw\jpeg.rsp
zip32 -u -@ %dest%\wxMac-%version%.zip < %src%\distrib\msw\tex2rtf.rsp
zip32 -u -@ %dest%\wxMac-%version%.zip < %src%\distrib\msw\dialoged.rsp
zip32 -u -@ %dest%\wxMac-%version%.zip < %src%\distrib\msw\ogl.rsp
zip32 -u -@ %dest%\wxMac-%version%.zip < %src%\distrib\msw\xml.rsp
zip32 -u -@ %dest%\wxMac-%version%.zip < %src%\distrib\msw\contrib.rsp
zip32 -u -@ %dest%\wxMac-%version%.zip < %src%\distrib\msw\makefile.rsp

Rem Below is the old-style separated-out format. This is retained only
Rem for local use, and for creating wxMSW-xxx.zip.
echo Zipping individual components
zip32 -@ %dest\wxWindows-%version%-gen.zip < %src\distrib\msw\generic.rsp
zip32 -@ -u %dest\wxWindows-%version%-gen.zip < %src\distrib\msw\makefile.rsp
zip32 -@ %dest\wxWindows-%version%-msw.zip < %src\distrib\msw\msw.rsp
zip32 -@ -u %dest\wxWindows-%version%-msw.zip < %src\distrib\msw\makefile.rsp
zip32 -@ %dest\wxWindows-%version%-gtk.zip < %src\distrib\msw\gtk.rsp
zip32 -@ -u %dest\wxWindows-%version%-gtk.zip < %src\distrib\msw\makefile.rsp
Rem zip32 -@ %dest\wxWindows-%version%-stubs.zip < %src\distrib\msw\stubs.rsp
zip32 -@ %dest\wxWindows-%version%-mot.zip < %src\distrib\msw\motif.rsp
zip32 -@ -u %dest\wxWindows-%version%-mot.zip < %src\distrib\msw\makefile.rsp
Rem (Obsolete) zip32 -@ %dest\wxWindows-%version%-user.zip < %src\distrib\msw\user.rsp

zip32 -@ %dest\wxWindows-%version%-DocSource.zip < %src\distrib\msw\docsrc.rsp
zip32 -@ %dest\wxWindows-%version%-WinHelp.zip < %src\distrib\msw\wx_hlp.rsp
zip32 -@ %dest\wxWindows-%version%-HTML.zip < %src\distrib\msw\wx_html.rsp
zip32 -@ %dest\wxWindows-%version%-PDF.zip < %src\distrib\msw\wx_pdf.rsp
zip32 -@ %dest\wxWindows-%version%-Word.zip < %src\distrib\msw\wx_word.rsp
zip32 -@ %dest\wxWindows-%version%-HTB.zip < %src\distrib\msw\wx_htb.rsp
zip32 -@ %dest\wxWindows-%version%-HTMLHelp.zip < %src\distrib\msw\wx_chm.rsp

Rem PDF/HTML docs that should go into the Windows setup because
Rem there are no WinHelp equivalents
zip32 -@ %dest\extradoc-%version%.zip < %src\distrib\msw\extradoc.rsp

rem VC++ project files
zip32 -@ %dest\wxWindows-%version%-vc.zip < %src\distrib\msw\vc.rsp

rem BC++ project files
zip32 -@ %dest\wxWindows-%version%-bc.zip < %src\distrib\msw\bc.rsp

rem CodeWarrior project files
zip32 -@ %dest\wxWindows-%version%-cw.zip < %src\distrib\msw\cw.rsp

rem OGL 3
zip32 -@ %dest\ogl3-%version%.zip < %src\distrib\msw\ogl.rsp

rem MMedia
zip32 -@ %dest\mmedia-%version%.zip < %src\distrib\msw\mmedia.rsp

rem STC (Scintilla widget)
zip32 -@ %dest\stc-%version%.zip < %src\distrib\msw\stc.rsp

rem GLCanvas: obsolete, now in main library
rem zip32 -@ %dest\glcanvas.zip < %src\distrib\msw\glcanvas.rsp

rem Tex2RTF
zip32 -@ %dest\tex2rtf2-%version%.zip < %src\distrib\msw\tex2rtf.rsp

rem JPEG source
zip32 -@ %dest\jpeg-%version%.zip < %src\distrib\msw\jpeg.rsp

rem TIFF source
zip32 -@ %dest\tiff-%version%.zip < %src\distrib\msw\tiff.rsp

zip32 -@ %dest\wxxrc-%version%.zip < %src\distrib\msw\xml.rsp

zip32 -@ %dest\contrib-%version%.zip < %src\distrib\msw\contrib.rsp

rem Dialog Editor source and binary
erase %dest\dialoged_source.zip
zip32 -@ %dest\dialoged_source.zip < %src\distrib\msw\dialoged.rsp
zip32 -j %dest\dialoged-%version%.zip %dest\dialoged_source.zip %src\bin\dialoged.exe %src\docs\winhelp\dialoged.hlp %src\docs\winhelp\dialoged.cnt
erase %dest\dialoged_source.zip

rem Misc. utils not in the main distribution
zip32 -@ %dest\utils-%version%.zip < %src\distrib\msw\utils.rsp
zip32 -@ -u %dest\utils-%version%.zip < %src\distrib\msw\utilmake.rsp

copy %src\docs\changes.txt %dest\changes-%version%.txt
copy %src\docs\msw\install.txt %dest\install_msw-%version%.txt
copy %src\docs\mac\install.txt %dest\install_mac-%version%.txt
copy %src\docs\motif\install.txt %dest\install_motif-%version%.txt
copy %src\docs\gtk\install.txt %dest\install_gtk-%version%.txt
copy %src\docs\readme.txt %dest\readme-%version%.txt
copy %src\docs\motif\readme.txt %dest\readme_motif-%version%.txt
copy %src\docs\gtk\readme.txt %dest\readme_gtk-%version%.txt
copy %src\docs\msw\readme.txt %dest\readme_msw-%version%.txt
copy %src\docs\mac\readme.txt %dest\readme_mac-%version%.txt
copy %src\docs\readme_vms.txt %dest\readme_vms-%version%.txt
copy %src\docs\motif\makewxmotif %dest\makewxmotif-%version%
copy %src\docs\gtk\makewxgtk %dest\makewxgtk-%version%

Rem Skip Inno Setup if inno is 0.
if "%inno" == "0" goto end

:dounzip

Rem Make dialoged-win32.zip and tex2rtf-win32.zip

cd %src\bin

zip32 %dest\dialoged-win32-%version%.zip dialoged.*
zip32 %dest\tex2rtf-win32-%version%.zip tex2rtf.*

cd %dest

rem Unzip the Windows files into 'wx'
mkdir %dest\wx

Rem After this change of directory, we're in the
Rem temporary 'wx' directory and not acting on
Rem the source wxWindows directory.
cd %dest\wx
unzip32 -o ..\wxWindows-%version%-msw.zip
unzip32 -o ..\wxWindows-%version%-gen.zip
unzip32 -o ..\wxWindows-%version%-vc.zip
unzip32 -o ..\wxWindows-%version%-bc.zip
unzip32 -o ..\wxWindows-%version%-HTMLHelp.zip
unzip32 -o ..\extradoc-%version%.zip
Rem Need Word file, for Remstar DB classes
unzip32 -o ..\wxWindows-%version%-Word.zip
unzip32 -o ..\ogl3-%version%.zip
unzip32 -o ..\mmedia-%version%.zip
unzip32 -o ..\stc-%version%.zip
unzip32 -o ..\jpeg-%version%.zip
unzip32 -o ..\tiff-%version%.zip
unzip32 -o ..\wxxrc-%version%.zip
unzip32 -o ..\contrib-%version%.zip
unzip32 -o ..\tex2rtf2-%version%.zip

rem Now delete a few files that are unnecessary
attrib -R *
erase /Y BuildCVS.txt *.in *.spec *.guess *.sub mkinstalldirs modules install-sh *.sh descrip.mms
erase /Y configure samples\configure samples\*.in demos\configure demos\*.in contrib\configure contrib\*.in
erase /Y setup.h.in setup.h_vms
erase /SY Makefile.in
erase /Y docs\html\wxbook.htm docs\html\roadmap.htm
Rem erase /Y contrib\docs\winhelp\mmedia.*
Rem erase /Y contrib\docs\winhelp\stc.*
Rem erase /Y contrib\docs\htmlhelp\mmedia.*
Rem erase /Y contrib\docs\htmlhelp\stc.*
Rem erase /Y contrib\docs\pdf\*.*
erase /sxzy contrib\docs\latex\ogl
erase /Y src\mingegcs.bat
erase /Y distrib

rem Now copy some binary files to 'bin'
if not isdir bin mkdir bin
copy %src\bin\dialoged.exe bin
copy %src\bin\tex2rtf.exe bin
copy %src\bin\dbgview.* bin
copy %src\bin\life.exe bin
copy %src\demos\life\breeder.lif bin
copy %src\docs\winhelp\dialoged.hlp %src\docs\winhelp\dialoged.cnt bin
copy %src\docs\winhelp\tex2rtf.hlp %src\docs\winhelp\tex2rtf.cnt bin

Rem Make wxMSW-xxx.zip
zip32 -r ..\wxMSW-%version%.zip *

rem Time to regenerate the Inno Setup script, wxwin2.iss.
rem NB: if you've changed wxwin2.iss using ScriptMaker, call splitinno.exe
rem from within distrib\msw, to split off innotop.txt and innobott.txt.
rem (NOT YET IMPLEMENTED)
echo Calling 'makeinno' to generate wxwin2.iss...
call %WXWIN\distrib\msw\makeinno.bat

erase /Y %dest\setup.*

rem Now invoke Inno Setup on the new wxwin2.iss
set innocmd="C:\Program Files\Inno Setup 1.3\compil32.exe" /cc %WXWIN\distrib\msw\wxwin2.iss
echo Invoking %innocmd...
start "Inno Setup" /w %innocmd%
pause Press any key to continue with the wxWindows distribution...
Rem ren %WXWIN\deliver2\setup.EXE %WXWIN\deliver2\wxMSW-%version%-setup.exe

:tidyup
cd %dest

rem Put all the setup files into a single zip archive.
zip32 wxMSW-%version%-setup.zip readme-%version%.txt setup.*

echo wxWindows archived.

goto end

:usage
echo DOS wxWindows distribution. Zips up all GTK/Motif/MSW/doc files,
echo and optionally makes a deliver2\wx directory and a setup.exe
echo if you specify 'inno' (skipping the zipping if you use 'innoonly').
echo.
echo "Usage: zipdist [inno | innoonly]"

:end
