MattPlot 1.7
------------

Obsolete utility developed between 2004-2006 to visualize data.
Sources stored here just for historical reasons, one of my first c++ projects.

 
Compilation
-----------

- External librares: see in '.\ext\'
- Generate help stuff with 'buildhelp.bat'
- Compile with BCB5 opening 'prg_MattPlot.bpg'
  and doing Build All
- Compile with BDS2006 opening 'prg_MattPlot.bdsgroup'
  and doing Build All; if problems on 'gifimage.hpp'
  retry after compilating 'gifimage.pas'



Project folders tree
--------------------

```
..\ (main directory: project files, batch scripts)
   |
   |-prg_MattPlot.bpg (BCB5 project group)
   |-prj_MattPlot.bpr (MattPlot BCB5 project file)
   |-prg_MattPlot.bdsgroup (BDS2006 project group)
   |-prj_MattPlot.bdsproj (MattPlot BDS2006 project file)
   |-readme.txt (this file)
   |-Backup.bat (backup sources using 7z)
   |-clean.bat (delete temporary files)
   |-buildhelp.bat (build documentation, do it before building project)
   |-deploy.bat (calls upx and copy exe to destinations)
   |
   |-bin\ (other binaries)
   |-ext\ (external third-party code: libraries, headers,…)
   |     |
   |     |-GifImage\ (TGIFImage http://finn.mobilixnet.dk/delphi/)
   |     |-htmlhelp\ (Microsoft HtmlHelp library http://msdn.microsoft.com/library/en-us/htmlhelp/html/vsconHH1Start.asp)
   |     |-SynEdit\ (Syntax Editor component http://synedit.sourceforge.net/)
   |               |
   |               |-prj_Synedit.bpr\ (BCB5 static library project)
   |               |-Source\ (source folder directly from package distribution)
   |
   |-hlp\ (application help files)
   |-out\ (compilation output: exe, dll, ocx, lib, chm, …)
   |     |
   |     |-dst\ (distribution files (additional files to setup running environment) )
   |
   |-res\ (resource files: images, sounds,…)
   |-src\ (source code)
   |     |
   |     |-com\ (common resources for all classes)
   |     |-ext\ (classes linked to external third-party code)
   |     |-frm\ (forms and dialogs)
   |     |-i-o\ (input/output: files, serial port, XML,…)
   |     |-mot\ (motion related classes)
   |     |-msc\ (miscellaneous classes for various purposes)
   |     |-mth\ (mathematical calculations)
   |     |-old\ (obsolete code)
   |     |-res\ (other resources)
   |     |-spc\ (classes specific to current application)
   |     |-sys\ (classes especially related to operating system)
   |     |-tmp\ (not used code to be changed or deleted)
   |     |-vcl\ (classes especially related to the Visual Component Library)
   |     |-vls\ (classes implementing concrete types)
   |
   |-tmp\ (temporary stuff, intermediate output - beware units with same name!)
   |-tst\ (test projects)
```
