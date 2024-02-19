@echo off
rem Settings
set dstexe=MattPlot.exe
set outdir=.\out
set upxexe="%PROGRAMFILES%\Bin\upx.exe"
set srcexe=prj_%dstexe%
set dests="%PROGRAMFILES%\Bin\%dstexe%" "K:\Exes\Engineering\%dstexe%" "%USERPROFILE%\Documenti\Subversion\Website\html\files\%dstexe%"

title *** Deploy %dstexe% ***


:PREPARE
set dst="%outdir%\%dstexe%"
rem if exist %dst% (del /S %dst%)
copy /y /z /b "%outdir%\%srcexe%" %dst%

:COMPRESS
echo.
rem Compress with UPX
%upxexe% --version
rem  **UPX options **
rem (other opts) set UPX=--compress-icons#0 --compress-resources=0 --strip-relocs=0
rem set UPX=-9 --compress-exports=0 --crp-ms#999999
rem set UPX=--brute --compress-exports=0
%upxexe% %dst%
echo %dst% ready!
echo.

:COPY
for %%i in (%dests%) do (
rem echo %%i
echo.
echo Copying to %%i
copy /y /z /b %dst% %%i
)
echo.
if errorlevel 0 (echo All done) else ( pause )
