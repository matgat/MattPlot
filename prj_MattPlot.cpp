#include <vcl.h>
#include <float.h>
#pragma hdrstop
USE("prj_MattPlot.todo", ToDo);
USERES("prj_MattPlot.res");
USEFORM("src\frm\unt_MainForm.cpp", frmMplotMain);
USERC("res\res_MainForm.rc");
USE("res\res_MainForm.h", File);
USEUNIT("src\spc\unt_MPlot.cpp");
USEUNIT("src\com\unt_Colors.cpp");
USEUNIT("src\com\unt_MatCommon.cpp");
USEUNIT("src\com\unt_Log.cpp");
USEUNIT("src\spc\unt_Tracks.cpp");
USEUNIT("src\spc\unt_Frames.cpp");
USEFORM("src\frm\unt_About.cpp", frmAbout);
USEUNIT("src\msc\unt_MatLogo.cpp");
USEUNIT("src\com\unt_ThisExeFile.cpp");
USEUNIT("src\com\unt_FileMetaData.cpp");
USEUNIT("src\com\unt_IniFile.cpp");
USEUNIT("src\com\unt_OptionValues.cpp");
USEUNIT("src\com\unt_Options.cpp");
USEUNIT("src\spc\unt_OptionsSet.cpp");
USEUNIT("src\ext\unt_HtmlHelp.cpp");
USEUNIT("src\sys\unt_FileDialog.cpp");
USEUNIT("src\ext\unt_GraphicFile.cpp");
USEUNIT("src\vls\unt_Quantities.cpp");
USEUNIT("src\sys\unt_TimeCounter.cpp");
USEUNIT("ext\GifImage\GIFImage.pas");
USEUNIT("src\spc\unt_MPlotClasses.cpp");
USEFORM("src\ext\unt_SynEditor.cpp", frmSynEditor);
USEUNIT("src\com\unt_CreateCtrls.cpp");
USEFORM("src\frm\unt_Printer.cpp", frmChooseExtraPrnOps);
USEUNIT("src\i-o\unt_DataColumns.cpp");
USEUNIT("src\i-o\unt_StreamParser.cpp");
USEFORM("src\frm\unt_TrackTransform.cpp", frmTrackTransform);
USEUNIT("src\spc\unt_PropertiesInterface.cpp");
USEFORM("src\frm\unt_Properties.cpp", frmProperties);
USEUNIT("src\mth\unt_Fourier.cpp");
USE("res\res_MPlot.h", File);
USERC("res\res_MPlot.rc");
USEUNIT("src\i-o\unt_StreamDataParser.cpp");
USEUNIT("src\i-o\unt_ImportExport.cpp");
USELIB("ext\SynEdit\Source\prj_Synedit.lib");
USEFORM("src\frm\unt_AddFoilObject.cpp", frmAddFoilObject);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR cmdarg, int)
{ // The main function
        _control87(MCW_EM,MCW_EM); // TODO 2: mask floating-point errors
        try
        {
         // . Further calls are handled by main instance
         // Try to open the mutex.
         HANDLE hMutex = ::OpenMutex(MUTEX_ALL_ACCESS, 0, "Global\\MattPlot.Mutex");
         if ( !hMutex ) hMutex = ::CreateMutex(0, 0, "Global\\MattPlot.Mutex");
         else { // A further instance
               if ( ::strlen(cmdarg) > 0 )
                  { // A second instance with argument
                   if ( cmdarg[0] == '@' )
                      { // if first argument is a particular char, use main instance
                       // Main instance to the top
                       HWND hWnd = ::FindWindow("TfrmMplotMain", NULL);
                       if (hWnd) {
                                  ::SetForegroundWindow(hWnd);
                                  // Send command line args in a WM_COPYDATA message.
                                  COPYDATASTRUCT cds;
                                  cds.dwData = 0xFF;
                                  cds.cbData = ::strlen(cmdarg) + 1;
                                  cds.lpData = cmdarg;
                                  ::SendMessage(hWnd, WM_COPYDATA, 0, (LPARAM)&cds);
                                 } 
                       return 0; // Now quit further instance
                      }// end 'argument begins witha particular char'
                  }// end 'a further instance with argument'
              }// end 'a further instance'

         // . Normally execute
         Application->Initialize();
         Application->Title = "MattPlot";
         Application->HelpFile = "MattPlot.chm";
         Application->CreateForm(__classid(TfrmMplotMain), &frmMplotMain);
         Application->Run();
        }
        catch (Exception &exception)
        {
         Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
