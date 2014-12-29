@echo off
echo Removing object and executable files.
if not exist source\makefile.vc4 goto direrror
if not exist %MSDEVDIR%\bin\cl.exe goto seterror
cd source
nmake -f makefile.vc4 clean
cd ..
goto end

:seterror
echo Error!
echo The environment variable MSDEVDIR does not
echo point to the root of the Microsoft Developer Studio directory
goto end

:direrror
echo Error!
echo Current directory is not the root directory of the
echo 3D Studio File Toolkit Source Distribution

:end
