@echo off
title Backup 1.3
rem   ****     winXP cmd batch file     ****
rem   ****   2007 - Matteo Gattanini    ****
:: Avvertenze:
:: non viene usato il timestamp e viene sovrascritto il backup giornaliero
:: controllare le impostazioni internazionali del PC :
::  data sia:  dd/mm/yyyy 
::attenzione nel sovrascrivere la destinazione (comunque grazie al timestamp non dovrebbe mai accadere)
:: per cambiare l'estensione dell'archivio

rem   ****      Settings     ****
set sourcedir=..\
set sourcename=MattPlot
set backupdir=:\Backup\
set stbckdrive=E
set ndbckdrive=F
:: archiver configuration
set archiverpath=%programfiles%\7-Zip\
set archivername=7z.exe
set archivetype=7z
set archiveropts=a -t%archivetype% -xr0!*.~* -xr0!*.tmp -xr0!*\.svn -xr0!*.tds -xr0!*.obj

rem   ****  Preliminary things ****
:: build a date string
for /f "tokens=1,2,3,4 delims=/-. " %%x in ("%date%") do set d=%%z%%y%%x
set d=%d: =0%
:: build a time string
::set t=%time%
goto ARCHIVE


rem   **** Archive ****
:ARCHIVE
set path=%path%;%archiverpath%
set source="%sourcedir%%sourcename%"
set archivedfilename=%d%_%sourcename%.%archivetype%
set destination="%stbckdrive%%backupdir%%archivedfilename%"


:DOARCHIVE
echo  -  Using:
echo %archivername%
echo  -  source directory:
echo %source%
echo  -  destination file:
echo %destination%
echo.
if EXIST %destination% del /S %destination%
echo %archivername% %archiveropts% %destination% %source%
%archivername% %archiveropts% %destination% %source%
if errorlevel 1 goto ERROR
echo ...Ok
goto COPY

:COPY
if EXIST %ndbckdrive%:\ (
echo copying also to %ndbckdrive%%backupdir%%archivedfilename%
if EXIST %ndbckdrive%%backupdir%%archivedfilename% del /S F%backupdir%%archivedfilename%
copy /y /z /b %destination% "%ndbckdrive%%backupdir%%archivedfilename%"
)
if errorlevel 1 goto ERROR
echo ...Ok
goto END

:ERROR
echo (!) Something was wrong, check and retry!
rem sendmail.exe -t < error_mail.txt
pause
exit

rem   ****      Nothing more to do     ****
:END
exit
rem   ****        End of file           ****


setlocal EnableDelayedExpansion
for /f "tokens=1-3 delims=," %%x in ("%backupdrives%") do (
for %%d in (%%x,%%y,%%z) do (
set destination=%%d%backupdir%
if exist !destination! (
set destination="!destination!%archivedfilename%"