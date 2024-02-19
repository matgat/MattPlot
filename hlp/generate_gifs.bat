@echo off
title **** Batch image conversion ****
rem settings
set bmpdir=..\res\Bitmaps\16
set gifdir=.\chm\html\files\images\actions
set conv=.\bin\convert

echo *************************
echo converting bmp files in %bmpdir% to gif in %gifdir%
setlocal EnableDelayedExpansion
for %%I in (%bmpdir%\*.bmp) do (
set FN=%%~nI
%conv% %%I %gifdir%\!FN!.gif
if NOT errorlevel 1 (echo !FN!.gif : OK) else (echo !FN!.gif: ERROR)
)