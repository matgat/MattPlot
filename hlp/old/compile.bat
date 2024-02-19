@echo off
title **** Compile MatPlot help sources ****
rem   ****     winXP cmd batch file     ****
rem   ****   2007 - Matteo Gattanini    ****
rem @if not "%OS%"=="Windows_NT" goto :EXIT

rem   ****      Internal arguments      ****
rem pay attention in dtd file external entity
set ignoreprfx=@
set initialtopic=1000
rem directories
set xmldir=.\xml
set htmdir=.\chm\html
rem addressed files
rem set hhc="%PROGRAMFILES%\HTML Help Workshop 14\hhc.exe"
set hhc=.\chm\bin\hhc.exe
set inst_saxon="%xmldir%\bin\saxon.exe"
set java_saxon=java -jar "%xmldir%\bin\saxon8.jar"
set dnet_saxon="%xmldir%\bin\dnet\Transform.exe"
set xslfile="%xmldir%\sty\xhelp2html.xsl"
set hhpfile=".\chm\MatPlot.hhp"
set chmfile="..\out\MatPlot.chm"
rem written files
set filelist="%xmldir%\%ignoreprfx%srcfiles.xml"
set alifile=".\chm\hlp_MatPlot.ali"
set hfile=".\chm\hlp_MatPlot.h"

rem   ****  What to do: ALL,XSL,HH ****
set task=ALL
goto %task%

:ALL
echo  **** Listing the xml source files ****
echo.
rem echo ^<^?xml version="1.0"^?^>  > %filelist%
echo ^<^!-- Created by compile.bat^: do not edit^! --^>  > %filelist%
echo ^<sourcefiles^>  >> %filelist%
type NUL > %alifile%
echo // Include me in program project to reference help topics > %hfile%

rem   ****     Preliminary settings     ****
setlocal EnableDelayedExpansion

rem   Take XML file names ignoring those
rem   with %ignoreprfx% as first letter
for %%I in (%xmldir%\*.xml) do (
set FN=%%~nI
set PRX=!FN:~0,1!
if NOT !PRX!==%ignoreprfx% (
echo ^	^<file name="!FN!" href="%%I"/^>  >> %filelist%
echo IDH_!FN!=html\!FN!.htm ; !FN! >> %alifile%
echo #define IDH_!FN! !initialtopic! >> %hfile%
set /a initialtopic+=1
)
)
echo ^</sourcefiles^>  >> %filelist% && echo [OK] created %filelist%

rem for /f "delims=" %%A in ('dir /B/A-d %xmldir%\*.xml' ) do set n=%%~nA & echo "%%~fA" "!n:~0,-3!%%~xA" 

if %task%==LIST goto END

:XSL
echo.
echo  **** Transform with instant Saxon ****
rem  saxon  [options]   source.xml   style.xsl   [params…]

rem   ****   Compile source xml files   ****
rem   **** except those with %ignoreprfx%  ****
for %%I in (%xmldir%\*.xml) do (
set FN=%%~nI
set PRX=!FN:~0,1!
rem ignore some files
if NOT !PRX!==%ignoreprfx% (
set H=%htmdir%\!FN!.htm
echo.

rem *** istant saxon
rem echo %instant_saxon% -o "!H!" "%%I" %xslfile%
%inst_saxon% -o "!H!" "%%I" %xslfile%
rem *** java saxon
rem %java_saxon% -t "%%I" %xslfile% >"!H!"
rem *** dotnet saxon
rem %dnet_saxon% -t "%%I" %xslfile% >"!H!"


if NOT errorlevel 1 (echo [OK] Created !H!) else pause
)
)

if %task%==XSL pause & goto END

rem   ****          Move file           ****
rem copy /Y ".\sty\faqs-style.css" /A ".\out\faqs-style.css" /A
rem move /Y %filelist% %xmldir%\filelist

:HH
echo.
echo  ****     Compiling Html Help      ****
echo hhc %hhpfile%
%hhc% %hhpfile%

if errorlevel 1 ( echo [OK] created %chmfile% ) else pause
if %task%==ALL hh %chmfile%
goto END

:END
rem   ****      Nothing more to do      ****
exit
rem   ****        End of file           ****
