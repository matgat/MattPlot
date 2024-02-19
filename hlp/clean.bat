@echo off
title **** Clean MatPlot help out compile files ****
rem   ****     winXP cmd batch file     ****
rem   ****   2006 - Matteo Gattanini    ****

rem   ****      Internal arguments      ****
set outdir=.\chm\html
set hfile=.\chm\hlp_MattPlot.h
set alifile=.\chm\hlp_MattPlot.ali
set tocfile=.\chm\TOC.hhc
set gifdir=.\chm\html\files\images\actions

@echo on
rem   ****  Main task ****
    del "%outdir%\*.htm*"
    del "%hfile%"
	del "%alifile%"
	del "%tocfile%"
rem Deleting gif files in %gifdir%
    del "%gifdir%\*.gif"

@echo off
rem   ****  Finally ****
