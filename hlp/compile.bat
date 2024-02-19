@echo off
title **** Compile MattPlot help sources ****
rem   ****     winXP cmd batch file     ****
rem   ****   2007 - Matteo Gattanini    ****
rem @if not "%OS%"=="Windows_NT" goto :EXIT

rem   ****      Internal arguments      ****
rem directories
set prgname=MattPlot
set bindir=.\bin
set xmldir=.\xml
set chmdir=.\chm
set outdir=%chmdir%\html
rem keep synchronized to the path in 'res_MainForm.rc'
set resdir=..\res\Other
rem used programs
set hhc="%PROGRAMFILES%\HTML Help Workshop\hhc.exe"
set inst_saxon=%bindir%\saxon\saxon.exe
set java_saxon=java -jar %bindir%\saxon\saxon9.jar
set dnet_saxon=%bindir%\saxon\dnet\Transform.exe
rem addressed files
set xmlfile="%xmldir%\main.xml"
set xslfile="%xmldir%\sty\xhelp2html.xsl"
set hhpfile="%chmdir%\%prgname%.hhp"
rem written files
set outfile="%outdir%\main.html"
::set filelist="%xmldir%\%ignoreprfx%srcfiles.xml"
set alifile="%chmdir%\hlp_%prgname%.ali"
set hfile="%chmdir%\hlp_%prgname%.h"
set chmfile="%resdir%\%prgname%.chm"

rem   ****  What to do: XSL,HH ****
set task=JAVASAX
echo.
goto %task%

:INSTSAX
echo  **** Transform with instant Saxon ****
rem  saxon  [options]   source.xml   style.xsl   [params…]
echo %inst_saxon% -o %outfile% %xmlfile% %xslfile%
echo tranforming...
%inst_saxon% -o %outfile% %xmlfile% %xslfile%
goto ENDTRANSFORM

:JAVASAX
echo  **** Transform with Java Saxon ****
rem  java -jar saxon.jar -t source.xml style.xsl >output.html
echo %java_saxon% -t %xmlfile% %xslfile% >%outfile%
echo tranforming...
%java_saxon% -t %xmlfile% %xslfile% >%outfile%
goto ENDTRANSFORM

:DNETSAX
echo  **** Transform with dotnet Saxon ****
rem Transform.exe -t source.xml style.xsl >output.html
echo %dnet_saxon% -t %xmlfile% %xslfile% >%outfile%
echo tranforming...
%dnet_saxon% -t %xmlfile% %xslfile% >%outfile%
goto ENDTRANSFORM

:ENDTRANSFORM
if NOT errorlevel 1 (echo [OK] Created %outfile%) else pause
goto HH

:HH
echo.
echo  ****     Compiling Html Help      ****
echo hhc %hhpfile%
%hhc% %hhpfile%

if errorlevel 1 ( echo [OK] created %chmfile% ) else pause
::Open file, only if no script argument "-no"
IF /I NOT "%1"=="-no" ( 
echo hh %chmfile%
hh %chmfile%
)
goto END

:END
rem   ****      Nothing more to do      ****
exit
rem   ****        End of file           ****
