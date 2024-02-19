title *** clean Borland's aux files ***

rem *** borland tmp files 
    del /S "*.obj"
    del /S "*.dcu"
    del /S "*.tds"
    del /S "*.~h"
    del /S "*.~???"
    del /S "*.cgl"

rem *** Help files ***
    call ".\hlp\clean.bat"

pause
