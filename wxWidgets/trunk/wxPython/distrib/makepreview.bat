rem @echo off

setlocal
set BASE=_distrib_zip\wxPython-%1

rem **** Make a directory to build up a distribution tree
mkdir _distrib_zip
mkdir %BASE%
mkdir %BASE%\wx
mkdir %BASE%\wxPython
mkdir %BASE%\demo
mkdir %BASE%\scripts

rem *** copy files
copy /s wx %BASE%\wx
copy /s wxPython %BASE%\wxPython
copy /s demo %BASE%\demo
copy /s scripts %BASE%\scripts
copy ..\BIN\*h_*.dll %BASE%\wx




rem *** remove unneeded files
cd _distrib_zip
ffind /A:D /SB CVS > del-files
cat del-files
del /sxzy @del-files
del del-files
cd ..



rem *** bundle it all up
cd _distrib_zip
tar cvf ../dist/wxPythonWIN32-preview-%1.tar wxPython-%1
gzip -9 ../dist/wxPythonWIN32-preview-%1.tar

rem *** cleanup
cd ..
del /sxzy _distrib_zip

endlocal



