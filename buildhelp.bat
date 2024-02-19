@echo off
title *** Build help ***

cd ".\hlp"
call clean.bat
call generate_gifs.bat
call compile.bat -no