#include <vcl.h>
#include <mmsystem.h> // Win32 multimedia (PlaySound)
#include "unt_MatCommon.h"
#include "unt_Log.h"
#pragma hdrstop
//---------------------------------------------------------------------------
#include <Clipbrd.hpp> // Clipboard()
#include <FileCtrl.hpp> // 'ForceDirectories' and other
#include <memory> // std::auto_ptr
#include "unt_MainForm.h"
#include "res_MainForm.h"
#include "unt_OptionsSet.h"  // Global options (!)
#include "unt_ThisExeFile.h"
#include "unt_Properties.h"
#include "unt_About.h"
#include "unt_FileDialog.h" // 'cls_FileDialog'
#include "unt_AddFoilObject.h" // Add label/marker dialogs
#include "unt_SynEditor.h"
#include "unt_GraphicFile.h"
#include "unt_TimeCounter.h"
#include "unt_Quantities.h"
#include ".\\hlp\\chm\\hlp_MattPlot.h" // Help topics IDs
#include "unt_HtmlHelp.h"
#include "unt_Printer.h" // My print functions
#include "unt_StreamDataParser.h"
#include "unt_Tracks.h"
#include "unt_MPlot.h"
#include "unt_TrackTransform.h"
#include "unt_ImportExport.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// TODO 1: use actions Checked to not repeat Execute
// TODO 1: TrackTransform as tool, not modal
// TODO 4: TWinControl instead of TGraphicControl? remove pnlPlotArea
// TODO 4: TFileStream descendant to speed up Read() character (read bigger blocks)
// TODO 4: cls_StreamParser hierarchy: text,mfile,xml; fill columns or directly frames?
// TODO 5: '.src\tmp\unt_Progress.h'

/*
------------- Bugs
*) scale fails (see data file)
*) Main form actions fired on keydown event of other forms, must capture them

------------- Wishlist
*) Stems
*) riscalatura rispetto fondoscala grandezze a 16bit?
*) Option for saving and resuming last plot at restart
*) Elaboration/transformations:  unary complex: fft, rotation e^jw
                                                binary: convolution
                                 unary: apply only to abscissa selection (EditMode)
                                        saturate (limit values), also with crop
                                        eliminate spikes
                                 Multi: max/min of a set of tracks
*) Eliminate Plot keys, fixed meaning
*) Markers: set from clipboard, add with dragndrop from ctrGrid
*) Save dialog: Checkbox "current frame only", unify export
*) X interval in statusbar
*) a structure for measured quantities (name,bool vertical,value)
*) consider to discard the xml format and use a saving text format:
   .each frame is transformed to a text file, in case of compressed,
    one archive with more files
   .introduce directives for frame and track style

------------- Optimization
*) for optimization: evaluate end expr once
           for ( int y=0, yend=Bdst->Height; y<yend; ++y )

*) ShortCut = ShortCut(LOBYTE(VkKeyScan('+')), TShiftState() << ssCtrl);
*) std::auto_ptr<TStringList> sl(new TStringList());
*) W8027 'functions containing some return statements are not expanded inline' : return by reference
*) smart track quantities calculation: keep first interval point iterator already calculated
*) my own fast data container in place of map?
*) use directly GDI winAPI for drawing: VolatileText, drawing functions; PaintWindow(HDC DC), PaintTo
   ::FrameRect(Canvas->Handle, &TheRect, ::CreateSolidBrush(0x00000000));
------------- Help
*) popups.txt in xml help; HTML help popups - Application->HelpCommand(HELP_CONTEXTPOPUP, IDH_Welcome);
*/


//========================== File scope variables ===========================

#define CHECKABORT_BREAK {if(UserAbort){UserAbort=false;Log->Warn("UserAbort","Operation aborted by user");break;}}
#define CHECKABORT_RETURN {if(UserAbort){UserAbort=false;Log->Warn("UserAbort","Operation aborted by user");return;}}


    // . Constants
    // Other const strings
    const String NotYetImplemented = "Not yet implemented, check for updates";

    // . Classes
    TfrmMplotMain *frmMplotMain;
    cls_ThisExeFile ThisExe;
    cls_Log *Log =0;

    // . Aux
    //String PrevMessage ="";  // Keep a StatusBar message after Hint


//============================= Static Members ==============================
    //const TCursor TfrmMplotMain::crFuck = TCursor(13);
    //const String TfrmMplotMain::AppName = "MattPlot";


//======================= Form constructor/destructor =======================


//---------------------------------------------------------------------------
__fastcall TfrmMplotMain::TfrmMplotMain(TComponent* Owner) : TForm(Owner)
{ // Costruttore

    // . Name control
    #ifndef _DEBUG
    String CallingName = mat::ExtractFileName( ParamStr(0) );
    if ( Application->Title.LowerCase() != CallingName.LowerCase() )
       {
        mat::MsgError(CallingName,"Invalid CRC");
        Application->Terminate();
       }
    #endif

    // . Defaults
    DoubleBuffered = true;
    Closing = Busy = UserAbort = false;
    DropPoint.x = DropPoint.y = -1;
    ModalChild = 0;

    // . Log instance
    // Viewer
    // TODO 5: TMemo* memLog instantiated at runtime?
    /*memLog: TMemo
      Left = 0
      Top = 0
      Width = 62
      Height = 56
      Align = alClient
      Color = clBlack
      DragKind = dkDock
      Font.Charset = ANSI_CHARSET
      Font.Color = clLime
      Font.Height = -13
      Font.Name = 'Lucida Sans Unicode'
      Font.Style = []
      ParentFont = False
      ScrollBars = ssBoth
      TabOrder = 0
      WordWrap = False    */

    // Log
    Log = new cls_Log(ThisExe.Name,memLog->Lines);
    // Eventi Log
    Log->OnEntry = DisplayMessage;
    Log->Entry("Init","Log ready, prosecuting initializations"); Log->StartTime();

    // . Resources
    // Interface objects
    //bmpTrack = new Graphics::TBitmap();
    //bmpFrame = new Graphics::TBitmap();
    //bmpTrack->Transparent = true;
    //bmpFrame->Transparent = true;
    //mat::LoadBitmap(bmpTrack,ID_BMP_TRACK);
    //mat::LoadBitmap(bmpFrame,ID_BMP_FRAME);

    // Additional informations on this application
    // ThisExe.Date.LoadStr(ID_STR_DATE);
    ThisExe.Date = __DATE__;
    ThisExe.AuthorMail = mat::LoadString(ID_STR_MAIL);
    ThisExe.WebSite = mat::LoadString(ID_STR_SITE);
    // Cursors
    //mat::LoadCursor(crFuck,ID_CUR_FUCK);

    Log->SubEntry("Resources loaded in " + Log->GetTime()); Log->StartTime(1);

    // . Help settings

    // Hints
    Application->HintShortCuts = true;

    // Help file (using "unt_HtmlHelp.h")
    HH = new cls_HtmlHelp();
    if ( !HH->ForceNoAPI )
         {
          Log->Success("Microsoft™ HTML Help Library loaded");
         }
    else {
          Log->Warn("Microsoft™ HTML Help Library may not be properly installed");
          Log->Append( HH->hhctrlMaintenanceMessage());
         }

    // TODO 5: Help context mapping (see 'hlp_MattPlot.h')
    HelpContext = IDH_Welcome; // Main Form Help ID
    tobMain->HelpContext = IDH_Interface; // Toolbar help context
    memLog->HelpContext = IDH_Log; // Log help context
    grdPane->HelpContext = IDH_PaneGrid; // Display grid help context
    treePlot->HelpContext = IDH_PlotObjects;

    // . Form object explicit instantiations
  /*treePlot: TTreeView
    Left = 0
    Top = 29
    Width = 169
    Height = 126
    Align = alLeft
    DragMode = dmAutomatic
    HideSelection = False
    Indent = 19
    ReadOnly = True
    TabOrder = 8               */

    // . Specific classes instances
    // Main options
    Opts = new cls_OptionsSet(Log);
    Opts->OnRead = AfterReadOptions; // Update task after read options
    Opts->OnStore = PrepareStoreOptions; // Prepare storing options
    // Plot
    // Remark: without pnlPlotArea no mousewheel event!!
    Plot = new cls_MPlot(pnlPlotArea,Log);
    Plot->OnProgress = Progress;
    Plot->OnPopupMenu = OnPlotPopup;
    Plot->OnPaint = OnPlotPaint;
    Plot->OnUpdate = OnPlotUpdate;
    Plot->OnChange = OnPlotChange;
    Plot->OnProperties = actPropertiesExecute;
    //ActiveControl = dynamic_cast<TWinControl*>(Plot); // No: Plot is a less expensive TGraphicControl

    Log->SubEntry("Classes instanced in " + Log->GetTime(1));

    // . Settings from options file
    actReadOptionsExecute(this); // Do this after all classes are instanced

    // . Other initializations
    Caption = ThisExe.Name + "  " + ThisExe.Version + "  [" + ThisExe.Date + "]"; // Form's caption
    AddActionNamesToHints(); // Build Action Hints
    // Quantities combobox cbxQuantities->Items->SetText( "y\nx\ndy\ndx\dydx\n" );
    grdPane->Canvas->Font->Assign(Font);
    grdPane->Selection.Bottom = 0;
    grdPane->Selection.Top = 0;

        // . System settings
    ::DragAcceptFiles(Handle,true); // for drag&drop file

    // . Finally
    Log->Entry("Init","End initialization in " + Log->GetTime());
    Application->OnIdle = OneShotThings;
}
//---------------------------------------------------------------------------
__fastcall TfrmMplotMain::~TfrmMplotMain()
{ // Distruttore

    Closing = true;
    try { // Be robust on further exception
         // In here ctrLog does not exists any more!
         Log->UnLink();
         // Delete temporary files on exit
         if ( Opts->DeleteTemp ) mat::DeleteFiles(GetTmpPath(),"*.*");
         ClearToolButtons(tobMain);
        }
    catch(...){}

    delete Plot;
    delete Opts;
    delete Log;
    delete HH;
}

//---------------------------------------------------------------------------




//================================= Utilities ===============================

//---------------------------------------------------------------------------
String TfrmMplotMain::GetHomePath()
{ // Get user home directory

    return ::IncludeTrailingBackslash( mat::GetHomePath() );
}

//---------------------------------------------------------------------------
String TfrmMplotMain::GetTmpPath()
{ // Get temporary dir path
    static String tmpdir = "";

    if ( tmpdir.IsEmpty() )
       { // Must retrieve temporary directory

        Log->Entry("GetTmpDirPath","Retrieving temporary folder");

            // . Check option
        String opt = String(Opts->TempDir).Trim();
        char c = 'n'; // a char for the 'switch' statement
        if (  opt.LowerCase() == "sys" ) c = 'l'; // in system temp
        else if ( opt.LowerCase() == "exe" ) c = 'e'; // in exe dir
        else c = 'n'; // An explicit path

            // . Operations
        switch ( c )
          {
           case 'n' :
               tmpdir = ::ExtractFilePath(::IncludeTrailingBackslash(opt));
               if ( ::ExtractFileDrive(tmpdir).Length() > 0 )
                    {
                     try {
                          if ( ::ForceDirectories(tmpdir) ) break; // Ok
                          else Log->Error("Cannot create path: " + tmpdir);
                         }
                     catch (Exception& e)
                         {
                          Log->Error("Invalid option TempDir: " + opt);
                         }
                    }
               else Log->Error("Invalid option TempDir: " + opt);
               // ...Otherwise go ahead
           case 'l' :
               tmpdir = mat::GetTmpPath(); // mat::GetEnvironmentVar("%TEMP%");
               if ( ::DirectoryExists(tmpdir) )
                    {
                     if ( ::ForceDirectories(tmpdir) ) break; // Ok
                     else Log->Error("Cannot create path: " + tmpdir);
                    }
               else Log->Error("Invalid %TEMP% variable: " + tmpdir);
               // ...Otherwise go ahead
           case 'e' :
               tmpdir = ThisExe.Directory;
               // Surely this directory exists!!
               if ( ::ForceDirectories(tmpdir) ) break; // Ok
               else {
                     Log->Error("Cannot create path: " + tmpdir);
                     tmpdir = "C:\\";
                     Log->Warn("Ok, I\'m desperate, setting temp dir to: " + tmpdir);
                    }
          }
            // . Final tasks
        // This is VERY important, force a subdirectory for sure!
        ASSERTING((Application->Title.Length()>0),"Application must have a name")
        tmpdir = ::IncludeTrailingBackslash(::IncludeTrailingBackslash(tmpdir)+String("~")+Application->Title);

        // This is the first time: delete previous temp files if requested or ensure folder
        if ( ::DirectoryExists(tmpdir) ) {if (Opts->DeleteTemp) mat::DeleteFiles(tmpdir,"*.*");}
        else ::ForceDirectories(tmpdir); // Ensure folder

        Log->SubEntry("I\'ll assume that temporary folder is " + tmpdir);
        Log->SubEntry("You should have write rights there!");
       }
    else ::ForceDirectories(tmpdir); // Ensure folder
    return tmpdir;
}

//---------------------------------------------------------------------------
inline String TfrmMplotMain::GetOptionFilePath()
{ // Get option file path
    return ThisExe.Directory + Application->Title + ".ini";
}


//---------------------------------------------------------------------------
inline String TfrmMplotMain::GetHelpFilePath()
{ // Get help file path
    return GetTmpPath() + Application->HelpFile;
}



//---------------------------------------------------------------------------
String TfrmMplotMain::EnsureHelpFile( const String& pth )
{ // Ensure help file presence extracting in resource if necessary

    if ( ::FileExists(pth) ) return pth;
    else {
          Log->Entry("EnsureHelpFile","Help file not found, I\'ll recreate it");
          try {
               TResourceStream *strRes = new TResourceStream(int(::HInstance), ID_RAW_EMBEDDED_HELPFILE, RT_RCDATA);
               ::ForceDirectories( ::ExtractFileDir(pth) ); // Ensure folder
               strRes->SaveToFile(pth);
               delete strRes;
               return pth;
              }
          catch (...)
              {
               Log->Error("Unable to recreate help file");
               return "";
              }
         }
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::BuildGridPane(TStringGrid* gr)
{ // Set grid cells value pointers

    ASSERTING((gr->ColCount==2),"grpPane must have two cols")
    gr->Cols[1]->Clear();

/* . . . Remove empty rows
    for (int row=0; row<gr->RowCount; ++row)
        {
         if ( gr->Cells[0][row].IsEmpty() )
            {
             // Pull up the rest
             for (int r=row+1; r<gr->RowCount; ++r) gr->Cells[0][r-1] = gr->Cells[0][r];
             // Then remove last row
             gr->RowCount -= 1; // Remove row
            }
        }  */

    // . Map cell strings with value pointers
    for ( int i=0; i<gr->RowCount; ++i )
        {
         gr->Objects[1][i] = reinterpret_cast<TObject*>( Plot->GetValuePointer(gr->Cells[0][i]) );
         if ( gr->Objects[1][i] == 0 ) Log->Error("Pane grid","Invalid quantity " + gr->Cells[0][i]); // Not recognized
        }
}

//---------------------------------------------------------------------------
void TfrmMplotMain::ProcessArguments(const mat::stringlist& args)
{ // Application arguments passed by command line
  // Process files when the plot GraphicControl receives
  // pixel dimensions (when Form is built), so do in OnShow
  // Strings preceded by 'optchar' are options, 'addchar' appended lines,
  // the rest are files
  // MattPlot.exe [-opt1] [-opt2] [[+addtofile1] c:\file1] [[+addtofile2] c:\file2]

    Log->Entry("Args","Reading command line arguments");
    #ifdef _DEBUG
      Log->SubEntry( args );
    #endif

    // . Settings
    const char CHR_OPT = '-'; // Option argument
    const char CHR_ADD = '+'; // Add line to file
    // Default options
    typ_FileOpenOpts FileOpenOpts;

    // . Some feedback
    if ( args.size() > 0 ) Log->SubEntry("Passed " + String(args.size()) + " strings");
    else {
          Log->SubEntry("No arguments passed");
          return;
         }

    // . Process all passed strings
    String AppendedLines = ""; // Bag for appended lines
    for ( unsigned int n=0, nmax=args.size(); n<nmax; ++n )
          {
           CHECKABORT_BREAK

           // Get argument string and check length
           String PassedStr = args[n].Trim();
           if ( PassedStr.Length() < 2 )
              {
               Log->Error(PassedStr + " is not a valid argument, check command line args");
               continue;
              }

           // Ignore 'no further instances' char
           if ( PassedStr[1] == '@' ) PassedStr = mat::After(PassedStr,1);

           // Analyze argument
           switch ( PassedStr[1] )
             {
              case CHR_OPT : // **** An option ****
                  PassedStr = mat::After(PassedStr,1); // remove opt char
                  Log->SubEntry("Option " +String(n)+  " : " + PassedStr,1);

                  if ( PassedStr.UpperCase() == "X" ) FileOpenOpts << oo_ClearAll;
                  else if ( PassedStr.UpperCase() == "F" ) FileOpenOpts << oo_SameFrame;
                  else if ( PassedStr.UpperCase() == "T" ) if(!actStayOnTop->Checked) actStayOnTopExecute(0);
                  else if ( PassedStr == "?" || PassedStr.UpperCase() == "H" ) HH->Open(EnsureHelpFile(GetHelpFilePath()), IDH_ProgramArguments);
                  else Log->Error("Unknown option, check command line args");
                  break;
              case CHR_ADD : // **** A line to append ****
                  PassedStr = mat::After(PassedStr,1); // remove add char
                  Log->SubEntry("A line to append: " + PassedStr,1);
                  AppendedLines += PassedStr + "\n";
                  break;
              default : // **** A file path ****
                  Log->SubEntry("A file path: " + PassedStr,1);
                  ProcessFile(PassedStr,AppendedLines,FileOpenOpts);
                  AppendedLines = ""; // Reset lines to append
             }// end 'switch arg first char'
          }// end 'for all args'
}


//---------------------------------------------------------------------------
// Process an opened file
void TfrmMplotMain::ProcessFile( const String& pth, const String& AddedLines, const typ_FileOpenOpts& FileOpenOpts )
{
    mat::cls_Scope LongTask(&DoLongTask, &EndLongTask);
    
    // . Preliminar check
    //bool a = ::DirectoryExists(pth);

    // . Recognize file type
    if ( mat::IsDirectory(pth) )
         { // I'll process all files in this directory
          DropPoint.x = DropPoint.y = -1; // Drop point information no more useful
          Log->SubEntry("Processing a directory, I\'ll see all contained files");
          std::auto_ptr<TStringList> sl(new TStringList());
          // TODO: do it recursively
          mat::ListFiles( sl.get(), pth, "*", true);
          for ( int i=0; i<sl->Count; ++i )
              {
               CHECKABORT_BREAK
               ProcessFile( sl->Strings[i], AddedLines );
              }
         }
    else { // A file
          if ( !::FileExists(pth) ) throw EInOutError("File not found: " + pth);
          // See extension
          String ext = ::ExtractFileExt(pth).LowerCase();

          /*if ( ext == MattPlot_Files_Ext || ext == Compressed_MattPlot_Files_Ext )
                { // A plot xml file
                 Log->Entry("ProcessFiles","Opening a MattPlot XML file");
                 Log->SubEntry(pth);

                 if ( Plot->FramesCount() ) if ( !mat::MsgAsk("Opening plot","This will clear current plot, do you want to continue?") ) return;
                 Plot->Clear();

                 // TODO 5: deal with MattPlot XML format
                 Log->Entry("MattPlot XML file",NotYetImplemented);*/
          if ( ext == iex::IniFileType.Extension() )
               { // An option file
                Log->Entry("ProcessFiles","Opening an ini file, I\'ll try to retrieve options from it");
                Opts->ReadAllFrom( pth ); // TODO 5: don't set default
               }
          // TODO: See if archive type
          // else if ( ext == iex::SevenZipFileType.Extension() )
          // TODO 2: support compressed files by searching dll and extracting in temp folder
          else { // A data file

                Log->Entry("ProcessFiles","Opening a data file");
                Log->SubEntry(pth);

                    // . Should I replace all existing plots?
                if ( FileOpenOpts.Contains(oo_ClearAll) ) Plot->Clear();
                
                    // . Prepare the frame where put tracks
                if ( FileOpenOpts.Contains(oo_SameFrame) )
                     {
                      Log->SubEntry("I\'ll put it in an existing frame");
                      Plot->AssureFrame();
                     }
                else {
                      Log->SubEntry("I\'ll put it in a new frame");
                      Plot->PrepareNewFrame( Plot->ScreenToClient(this->ClientToScreen(DropPoint)) );
                     }
                    // . Open the file stream and parse it
                //::ForceDirectories( ::ExtractFileDir(pth) ); // Ensure folder
                bool retry = false;
                bool jobdone = false;
                do {
                    // TODO 3: support compressed files by implementing a TStream descendant
                    std::auto_ptr<TStream> DataStream(new TFileStream(pth, fmOpenRead|fmShareDenyWrite));

                    // Parser instantiation
                    cls_StreamDataParser *p = 0;
                    if (ext == iex::CsvFileType.Extension() ) p = new cls_StreamCsvParser(Log);
                    else p = new cls_StreamTextTableParser(Log);
                    std::auto_ptr<cls_StreamDataParser> DataCols(p);
                    DataCols->ExpectedDelimiters = Opts->ExpectedDelimiters;
                    DataCols->OnProgress = Progress;

                    try {
                         // . Perform parsing
                         jobdone = DataCols->Read(DataStream.get(),AddedLines);

                         // If here parsing ended successfully: data are ready in 'DataCols.Columns'
                         jobdone = Plot->CurrentFrame->ImportColumns( &(DataCols->Columns) );
                        }
                    catch ( mat::EParsingError& e )
                        {
                         Log->Error("Parser", e.Message);
                         if ( !ModalChild )
                              {
                               // std::auto_ptr<TfrmSynEditor> frmEditor(new TfrmSynEditor(Application->MainForm));
                               ModalChild = new TfrmSynEditor(this, DataStream.get());
                               dynamic_cast<TfrmSynEditor*>(ModalChild)->ListPosition (e.Message, e.Row, e.Col);
                               retry = ModalChild->ShowModal() == mrYes;
                               delete ModalChild;
                               ModalChild = 0;
                              }
                         else {
                               Log->Warn("SynEditor already instanced");
                               retry = false;
                              }
                        }
                   }
                while ( retry && !jobdone );

                    // . Delete frame if empty and failed import
                if ( !jobdone && Plot->CurrentFrame->Empty() ) Plot->RemoveFrame();
               }// end 'a data file'
         } // end 'a file'
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::PrepareStoreOptions()
{ // Preprocessing of options values to write

    // . Get local variables linked to options

    // . Preprocessing of values to write
    // Substitution of default directory
    // Executable directory 'exe'
    if ( String(Opts->DefaultDir) == ThisExe.Directory )  Opts->DefaultDir = "exe";
    // User directory
    else if ( String(Opts->DefaultDir) == GetHomePath() )  Opts->DefaultDir = "usr";
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::AfterReadOptions()
{ // Postprocessing of read options, update program values

    // . Preliminary substitutions
    // Substitution of default directory
    // Executable directory 'exe'
    if ( String(Opts->DefaultDir).Trim().LowerCase() == "exe" ) Opts->DefaultDir = ThisExe.Directory;
	else if ( String(Opts->DefaultDir).Trim().LowerCase() == "usr" ) Opts->DefaultDir = GetHomePath();

    // Handling of DecimalSeparator and ThousandSeparator
    // TODO 5: DecimalSeparator should be read before reading options!!!! (for 'double' strings)
    if ( Opts->DecimalSeparator == '?' || Opts->ThousandSeparator == '?' )
         {
          Application->UpdateFormatSettings = true;
          ::PostMessage(Application->Handle,WM_SETTINGCHANGE,NULL,NULL);
          // System values of '::DecimalSeparator' and '::ThousandSeparator' are used
          ::Sleep(10);
         }
    else {
          Application->UpdateFormatSettings = false;
         }
    if ( Opts->DecimalSeparator != '?' ) ::DecimalSeparator = Opts->DecimalSeparator;
    if ( Opts->ThousandSeparator != '?' ) ::ThousandSeparator = Opts->ThousandSeparator;
    // A check
    if ( ::DecimalSeparator == ::ThousandSeparator ) Log->Error("Decimal and thousand separators are identical! (" + String(::DecimalSeparator) + ") this will lead to number equivocations");

    // . Perform local actions after read options

        // . Main form
    // Choose First Mode
	TBasicAction* act = GetActionFromName( String(Opts->FirstMode).Trim() );
    if (act) act->OnExecute(this);
	else Log->Warn("FirstMode \'" + Opts->FirstMode + "\' is not a valid action");
    frmMplotMain->Menu = (Opts->EnableMainMenu) ? mnuMain : 0;
    if ( int(Opts->FormColor) != int(Color&0x00FFFFFF) ) Color = TColor(Opts->FormColor);
    grdPane->Color = TColor(Opts->ToolsColor);
    treePlot->Color = TColor(Opts->ToolsColor);
    Plot->Drawer->HighlightInvBkgColor = TColor(mat::Invert(TColor(Opts->HighlightColor)));
    // Toolbar
    frmMplotMain->tobMain->Visible = false; // BeginUpdate
    mat::stringlist actnames;
    mat::Tokenize(actnames, String(Opts->Toolbar).c_str(), ',');
    BuildToolbar(actnames); // Build toolbar
    frmMplotMain->tobMain->Visible = Opts->EnableToolbar; // EndUpdate
    // Context menus customization
    actnames.clear();
    mat::Tokenize(actnames, String(Opts->ObjectMenu).c_str(), ',');
    ActionNamesToIndexes (actnames, &cls_FoilObject::ActionsIdxs);
    actnames.clear();
    mat::Tokenize(actnames, String(Opts->FrameMenu).c_str(), ',');
    ActionNamesToIndexes (actnames, &cls_Frame::ActionsIdxs);
    actnames.clear();
    mat::Tokenize(actnames, String(Opts->TrackMenu).c_str(), ',');
    ActionNamesToIndexes (actnames, &cls_Track::ActionsIdxs);
    // Resize status panels
    barStatus->Height = 2.2*barStatus->Font->Size;
    barStatus->Panels->Items[0]->Width = 22 * barStatus->Font->Size;
    barStatus->Panels->Items[1]->Width = 20 * barStatus->Font->Size;
    barStatus->Panels->Items[2]->Width = 9 * barStatus->Font->Size;
    #ifdef INCLUDE_SOUNDS
      Plot->Noisy = Opts->PlaySound;
    #endif
    #ifdef _DEBUG
      Opts->DevelopMode = true;
    #endif
    Plot->FloatPrecision = Opts->FloatPrecision;
    Plot->FloatDigits = Opts->FloatDigits;
    actExperimentalStuff->Enabled = Opts->DevelopMode;

        // . Tools
    //TfrmMplotMain->ctrPane->Width = Opts->PaneWidth;
    //'Opts->AutoPane' is managed in this form
    // Fill pane quantities from 'Opts->PaneQuantities'
      grdPane->Cols[0]->Clear();
      mat::stringlist qn;
      mat::Tokenize(qn, String(Opts->PaneQuantities).c_str(), ',');
      grdPane->RowCount = qn.size();
      for ( unsigned int row=0; row<qn.size(); ++row ) grdPane->Cells[0][row] = qn[row];
      BuildGridPane(grdPane);

        // . Plot
    Plot->SampPerPix = Opts->SampPerPix;
    mat::StringToTFont(Opts->PlotFont,Plot->TextFont);
    Plot->PixMargin = Opts->PlotMargin;
    Plot->LagThreshold = Opts->LagThreshold;
    Plot->Timeout = Opts->Timeout;
    // TODO: Plot->NoBackground , Plot->MarkCurrentFrame
        // . Frame
    cls_Frame::PlotAlsoOnMargins = Opts->PlotAlsoOnMargins;
    cls_Frame::CenterTitle = Opts->CenterTitle;
    cls_Frame::TitleMag = Opts->TitleMag;
    cls_Frame::DefaultBorder = Opts->FrameBorder;
    cls_Frame::DefaultMarginX = Opts->FrameMarginX;
    cls_Frame::DefaultMarginY = Opts->FrameMarginY;
    cls_Frame::DefaultColor = TColor(Opts->FrameColor);
        // . Axes
    cls_Axes::CenterLabels = Opts->CenterAxesLabels;
    cls_Axes::DefaultColor = TColor(Opts->AxesColor);
    // Legend
    cls_Legend::DefaultSpacing = Opts->LegendSpacing;
    cls_Legend::DefaultStrokeLength = Opts->LegendStrokeLength;
    cls_Legend::DefaultColor = TColor(Opts->LegendColor);
        // . Grid
    cls_Grid::DefaultVGridDist = ::abs(Opts->VGridDist);
    cls_Grid::DefaultHGridDist = ::abs(Opts->HGridDist);
    // Grid divisions, Ensure increasing order
    //std::sort(mat::numlist(Opts->VGridDivs).begin(), mat::numlist(Opts->VGridDivs).end());
    //std::sort(mat::numlist(Opts->HGridDivs).begin(), mat::numlist(Opts->HGridDivs).end());
    cls_Grid::DefaultVGridDivs = Opts->VGridDivs;
    cls_Grid::DefaultHGridDivs = Opts->HGridDivs;
    cls_Grid::DefaultLinesStyle = TPenStyle(Opts->GridLinesStyle);
    cls_Grid::DefaultColor = TColor(Opts->GridColor);
    cls_Grid::DefaultFont= Opts->LabelFont;
        // . Tracks
    cls_Track::DefaultLineWeight = Opts->TrackLineWeight;
    cls_Track::DefaultLineColor = TColor(Opts->TrackLineColorSeed);
    cls_Track::DefaultLineStyle = TPenStyle(Opts->TrackLineStyle);
    cls_Track::DefaultPointStyle = Opts->TrackPointStyle;
    cls_Track::DefaultPointSizeCoeff = Opts->TrackPointSize;
    cls_Track::DefaultMarkPoints = Opts->TrackMarkPoints;
    cls_Track::DefaultStrokeLines = Opts->TrackStrokeLines;
    cls_Track::DefaultFillArea = Opts->TrackFillArea;
    cls_Track::DefaultDrawHistograms = Opts->TrackDrawHistograms;
    cls_Track::DefaultTrackShowCoords = Opts->TrackShowCoords;
    cls_Track::DefaultTrackCoordsDist = Opts->TrackCoordsDist;
        // . Markers
    cls_Marker::DefaultLineWeight = Opts->MarkerLineWeight;
    cls_Marker::DefaultLineColor = TColor(Opts->MarkerLineColor);
    cls_Marker::DefaultLineStyle = TPenStyle(Opts->MarkerLineStyle);
        // . EditMode
    Plot->Drawer->MarkColor = TColor(Opts->MarkColor);
    Plot->Drawer->MarkZoneStyle = TBrushStyle(Opts->MarkZoneStyle);
        // . PanMode
    Plot->DefaultPan = Opts->DefaultPan;
    Plot->CoastingTau = double(Opts->CoastingTau) / 1000.0;
    Plot->ContinuousPan = double(Opts->ContinuousPan) * 0.03;
        // . ZoomMode
    Plot->SuddenZoom = Opts->SuddenZoom;
    Plot->ZoomPerc = Opts->ZoomPerc;
    Plot->Drawer->SelRectLineStyle = TPenStyle(Opts->SelRectLineStyle);
    Plot->Drawer->SelRectLineColor = TColor(Opts->SelRectLineColor);
    Plot->Drawer->SelRectAreaColor = TColor(Opts->SelRectAreaColor);
        // . MeasureMode
    Plot->Drawer->MeasureLinesColor = TColor(Opts->MeasureColor);
    Plot->Drawer->MeasureLinesStyle = TPenStyle(Opts->MeasureLinesStyle);
    Plot->Drawer->ViewVerticalLine = Opts->ViewAbscissaLine;
    Plot->Drawer->ViewHorizontalLine = Opts->ViewOrdinateLine;
    Plot->Drawer->ViewSlopeLine = Opts->ViewSlopeLine;
    //mat::StringToTFont(Opts->VolatileFont,Plot->Drawer->VolatileFont);
    //::ShowMessage(::IntToHex(int(Plot->Drawer->VolatileFont->Color),8));
        // . PenMode
    Plot->Drawer->PencilColor = TColor(Opts->PencilColor);
    Plot->Drawer->PencilSize = Opts->PencilSize;
    Plot->Drawer->HighlighterSize = Opts->HighlighterSize;
    Plot->Drawer->HighlighterColor = TColor(Opts->HighlighterColor);
        // . Plot keys
    Plot->keyZoomToggle = Opts->ZoomToggle;
    Plot->keyPanToggle = Opts->PanToggle;
    Plot->keyReset = Opts->Reset;
    Plot->keyDo = Opts->Do;
    Plot->keyCycle = Opts->Cycle;
    Plot->keyPrior = Opts->Prior;
    Plot->keyNext = Opts->Next;
    Plot->keyMinus = Opts->Minus;
    Plot->keyPlus = Opts->Plus;
    Plot->keyStart = Opts->Start;
    Plot->keyEnd = Opts->End;
    Plot->keyUp = Opts->Up;
    Plot->keyDown = Opts->Down;
    Plot->keyLeft = Opts->Left;
    Plot->keyRight = Opts->Right;
        // . Shortcuts
    mat::SetShortCut(actEditOptions,Opts->EditOptions);
    mat::SetShortCut(actStoreOptions,Opts->StoreOptions);
    mat::SetShortCut(actSnapshot,Opts->Snapshot);
    mat::SetShortCut(actRefresh,Opts->Refresh);
    mat::SetShortCut(actAbort,Opts->Abort);
    mat::SetShortCut(actExecMacro,Opts->ExecMacro);
    mat::SetShortCut(actLockX,Opts->ToggleLockAxes);
    mat::SetShortCut(actLockY,Opts->ToggleLockAxes);
    mat::SetShortCut(actShowTree,Opts->ShowTree);
    mat::SetShortCut(actClearAll,Opts->ClearAll);
    mat::SetShortCut(actSelectMode,Opts->SelectMode);
    mat::SetShortCut(actEditMode,Opts->EditMode);
    mat::SetShortCut(actPanMode,Opts->PanMode);
    mat::SetShortCut(actZoomMode,Opts->ZoomMode);
    mat::SetShortCut(actMeasureMode,Opts->MeasureMode);
    mat::SetShortCut(actPenMode,Opts->PenMode);
    mat::SetShortCut(actAddFrame,Opts->AddFrame);
    mat::SetShortCut(actRemoveFrame,Opts->RemoveFrame);
    mat::SetShortCut(actFrameProperties,Opts->FrameProperties);
    mat::SetShortCut(actExportFrameToText,Opts->ExportFrameToText);
    mat::SetShortCut(actExportFrameToMatlab,Opts->ExportFrameToMatlab);
    mat::SetShortCut(actFitToAxes,Opts->FitToAxes);
    mat::SetShortCut(actFitToTracks,Opts->FitToTracks);
    mat::SetShortCut(actFitToTrack,Opts->FitToTrack);
    mat::SetShortCut(actLegendToggle,Opts->LegendToggle);
    mat::SetShortCut(actGridToggle,Opts->GridToggle);
    mat::SetShortCut(actGridMore,Opts->GridMore);
    mat::SetShortCut(actGridLess,Opts->GridLess);
    mat::SetShortCut(actAddText,Opts->AddText);
    mat::SetShortCut(actAddMarker,Opts->AddMarker);
    mat::SetShortCut(actAddTrack,Opts->AddTrack);
    mat::SetShortCut(actRemoveTrack,Opts->RemoveTrack);
    mat::SetShortCut(actTrackProperties,Opts->TrackProperties);
    mat::SetShortCut(actTrackStatistics,Opts->TrackStatistics);
    mat::SetShortCut(actTrackTransform,Opts->TrackTransform);
    mat::SetShortCut(actMarkPoints,Opts->MarkPoints);
    mat::SetShortCut(actShowCoords,Opts->ShowCoords);
    mat::SetShortCut(actTrackData,Opts->TrackData);
    mat::SetShortCut(actShowLog,Opts->ShowLog);
    mat::SetShortCut(actShowPane,Opts->ShowPane);
    mat::SetShortCut(actCompute,Opts->Compute);
    mat::SetShortCut(actDemo,Opts->Demo);
    mat::SetShortCut(actExit,Opts->Exit);
    mat::SetShortCut(actTrySomething,VK_SPACE,TShiftState() << ssAlt);
    mat::SetShortCut(actEasterEgg,VK_LWIN,TShiftState() << ssAlt);
    mat::SetShortCut(actPrint,VK_PRINT); // Print shortcut

    // . Finally
    Plot->ApplyOptions();
    Plot->Invalidate();
}


//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::ToStatusBar(const String& msg, int pnl)
{ // Write in statusbar
    if ( pnl<0 || pnl>=barStatus->Panels->Count ) pnl = barStatus->Panels->Count - 1;
    barStatus->Panels->Items[pnl]->Text = msg;
}
//---------------------------------------------------------------------------
String __fastcall TfrmMplotMain::FromStatusBar(int pnl)
{ // Get text in statusbar
    if ( pnl<0 || pnl>=barStatus->Panels->Count ) pnl = barStatus->Panels->Count - 1;
    return barStatus->Panels->Items[pnl]->Text;
}


//---------------------------------------------------------------------------
// Feedback during waiting
void __fastcall TfrmMplotMain::DoLongTask()
{
    // . Manage status flags
    Busy = true;

    // . Progress bar
    barProgress->Visible = Busy;
    barProgress->Position = 0;
    barProgress->Hint = Log->LastEntry;
    //Remark: toolbar contains actions, not needed: tobMain->Enabled = !Busy;
    //        see also 'aclMainUpdate' that calls 'EnableAllActions'

    // . Status bar
    //PrevMsg = FromStatusBar();
    //SetStatusImage( actAbort->ImageIndex );
    Log->Entry("Starting long task","To Abort press " + ::ShortCutToText(Opts->Abort));

    // . Cursor
    Screen->Cursor = crHourGlass;

    // . Other
    Repaint();
    Application->ProcessMessages();
}

//---------------------------------------------------------------------------
// Ending a long task (feedback)
void __fastcall TfrmMplotMain::EndLongTask()
{
    // . Manage status flags
    Busy = false;

    // . Progress bar
    barProgress->Visible = Busy;
    barProgress->Position = 0;
    barProgress->Hint = Log->LastEntry;

    // . Status bar
    //ClearStatusImage();

    // . Cursor
    Screen->Cursor = crDefault;

    // . Other
    Repaint();
    Application->ProcessMessages();
}

//----------------------------------------------------------------------------
// Progress bar control; transfers abort action to using object
void __fastcall TfrmMplotMain::Progress( int PercentDone, bool& abort )
{
    //if (!Busy) DoLongTask(); // Force longtask interface
    barProgress->Position = PercentDone;
    Application->ProcessMessages();
    abort = UserAbort;
    if ( UserAbort ) UserAbort = false; // Eat user abort request
}

//----------------------------------------------------------------------------
// Image processing progress
void __fastcall TfrmMplotMain::ImageProgress ( System::TObject* Sender,
											   TProgressStage Stage,
											   Byte PercentDone,
											   bool RedrawNow,
											   const TRect &R,
                                               const AnsiString Msg )
{
    if ( Stage == psEnding ) barProgress->Position = 0;
    else barProgress->Position = PercentDone;

    if ( !Msg.IsEmpty() ) ToStatusBar(Msg);
}

//----------------------------------------------------------------------------
// Right click plot event
void __fastcall TfrmMplotMain::OnPlotPopup(const TPoint& P)
{
    switch ( Plot->Mode )
       {
        case cls_MPlot::MD_SELECT  : CreatePlotPopup(P); break;
        //case cls_MPlot::MD_PAN     :  // Continuous pan
        //case cls_MPlot::MD_ZOOM    :  // Default zoom out
        case cls_MPlot::MD_MEASURE : popMeasure->Popup(P.x,P.y); break;
        case cls_MPlot::MD_EDIT    : popEdit->Popup(P.x,P.y); break;
       }
}

//----------------------------------------------------------------------------
// Plot properties popupmenu creator
void __fastcall TfrmMplotMain::CreatePlotPopup(const TPoint& P)
{
    ClearPopMenu(popFoilObject);
	BuildPopMenu (popFoilObject, Plot->Selected.FoilObject->GetActionsIdxs());
    popFoilObject->Popup(P.x,P.y);
}

//---------------------------------------------------------------------------
// Save Plot bitmap
bool TfrmMplotMain::SavePlotBitmap (const String& path, TImage* img)
{ 
    bool jobdone = false;
    bool skip = false;
    if ( Opts->PromptOverwrite && ::FileExists(path) ) skip = !mat::MsgAsk(String("Do you want to overwrite:\n") + path);

    if ( !skip )
       {
        // Graphic file handler
        cls_GraphicFile* GrFile = new cls_GraphicFile(img);
        try {
             GrFile->InitialDir = Opts->DefaultDir;
             GrFile->OnProgress = ImageProgress;
             if ( GrFile->Save(path) )
                  {
                   jobdone = true;
                   Log->Success(GrFile->Result);
                  }
             else Log->Error(GrFile->Result);
            }
        __finally { delete GrFile; }
       }
    return jobdone;
}

/*---------------------------------------------------------------------------
void TfrmMplotMain::DrawPlot (class TCanvas* c, const TRect& R)
{ // Draw Plot in a Canvas via Metafile

        // . Create MetaFile and its Canvas
    TMetafile *mf = new TMetafile;
    mf->Width  = R.Width();
    mf->Height = R.Height();
    //mf->MMWidth  = (2540 * mf->Width) / Screen->PixelsPerInch;
    //mf->MMHeight = (2540 * mf->Height) / Screen->PixelsPerInch;
    TMetafileCanvas* mc = new TMetafileCanvas(mf, 0, Application->Title, "Plot");
    cls_Foil MetaFoil = cls_Foil(mc,R,double(R.Width())/double(Plot->Width),true);

        // . Set again dimensions
    mf->Width  = R.Width();
    mf->Height = R.Height();

        // . Draw
    Plot->PaintTo(0,&MetaFoil);
    delete mc;

        // . Here can use metafile
    c->Draw(0,0,mf);

    // . Finally
    delete mf;
} */

//---------------------------------------------------------------------------
bool TfrmMplotMain::GetPlotMetafile (class cls_Frame* fr, const String& path, const bool clpcopy) const
{ // Build Metafile from Plot

    // . Checks
        // . Dimensions
    int mfWidth  = Opts->MetafileMagnification * ((fr) ? fr->Width : Plot->Width);
    int mfHeight = Opts->MetafileMagnification * ((fr) ? fr->Height : Plot->Height);

        // . Create MetaFile and its Canvas
    TMetafile *mf = new TMetafile;
    mf->Width  = mfWidth;
    mf->Height = mfHeight;
    //mf->MMWidth  = (2540 * mf->Width) / Screen->PixelsPerInch;
    //mf->MMHeight = (2540 * mf->Height) / Screen->PixelsPerInch;
    TMetafileCanvas* mc = new TMetafileCanvas(mf, 0, Application->Title, "Plot");
    cls_Foil MetaFoil = cls_Foil(mc,TRect(0,0,mfWidth,mfHeight),Opts->MetafileMagnification,true);

        // . Set again dimensions
    mf->Width  = mfWidth;
    mf->Height = mfHeight;

        // . Draw
    Plot->PaintTo(fr, &MetaFoil);
    delete mc;

        // . Here can use metafile
    //PaintBox1->Canvas->Draw(0,0,mf);
    if ( path.Length() > 1 ) mf->SaveToFile(path);
    if ( clpcopy ) Clipboard()->Assign(mf);

    // . Finally
    delete mf;
    return true;
}

//---------------------------------------------------------------------------
// TODO: move Export plot or current frame
bool TfrmMplotMain::Export(const String& path, cls_Frame* fr, TImage* img)
{

    // . Local vars
    mat::cls_Scope LongTask(&DoLongTask, &EndLongTask);
    bool jobdone = false;
    String Subj = (fr) ? fr->Name : String("Plot");

    Log->Entry("Export",Subj + " exporting requested");

    // . See which filter
    String ext = ::ExtractFileExt(path).Trim().LowerCase();
    if ( ext == ".mpl" || ext == ".mplz" ) // MattPlot multisession text files (.mpl)
         {
          jobdone = (fr) ? fr->ToText(path) : Plot->ToText(path);
         }
    else if (ext == ".txt") // Plain text data file (.txt)
         {
          jobdone = (fr) ? fr->ToText(path, Opts->DataDelimiter) : Plot->ToText(path, Opts->DataDelimiter);
         }
    else if (ext == ".m") // m-files (.m)
         {
          jobdone = (fr) ? fr->ToMfile(path) : Plot->ToMfile(path);
         }
    else if (ext == ".bmp" || ext == ".gif") // Bitmap (.bmp) / Graphics Interchange Format (.gif)
         {
          jobdone = SavePlotBitmap(path, img);
         }
    else if (ext == ".emf") // Enhanced Meta File (.emf)
         {
          jobdone = GetPlotMetafile(fr, path, false);
          if (fr) Log->Warn("Better export to EMF the entire plot");
         }
    else if (ext == ".svg") // Scalable Vector Graphics (.svg)
         {
          //jobdone = Plot->ToSVG(path);
          Log->Warn("Export to svg",NotYetImplemented);
         }
    else {
          //Log->Error("Unknown file filter, export aborted");
          Log->Error("Format \'" + ext + "\' not supported, use a valid extension");
         }

    // . Finally
    Log->Entry( Subj + " exported to",::ExtractFileName(path));
    return jobdone;
}



//--------------------------- Action utilities ------------------------------

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::BuildToolbar (const mat::stringlist& actnames)
{ // Build toolbar from list of action names

        // . Clear toolbar
    //tobMain->BeginUpdate();
    Log->SubEntry("Deleting toolbar");
    ClearToolButtons(tobMain);

        // . Rebuild
    Log->SubEntry("Rebuilding toolbar");
    String ActName;
    for ( int i=actnames.size()-1; i>=0; --i )
        {
         ActName = actnames[i].Trim();
		 TBasicAction* act = GetActionFromName( ActName );
         if ( act )
              { // Got an action

               // A check: not all action can stay in toolbar...
               if ( act == actEditOptions ||
                    act == actReadOptions ||
                    act == actStoreOptions  ) {
                                               Log->Warn("Action \'" + act->Name + "\' is not allowed to stay in toolbar");
                                               continue;
                                              }

               TToolButton* tbt = new TToolButton(tobMain);
               tbt->Parent = tobMain;

               // Handle special switch-to-mode actions
               if ( act == actSelectMode  ||
                    act == actEditMode    ||
                    act == actPanMode     ||
                    act == actZoomMode    ||
                    act == actMeasureMode ||
                    act == actPenMode       ) {
                                               tbt->Style = tbsCheck;
                                               tbt->Grouped = true;
                                               tbt->AllowAllUp = false;
                                              }
               else tbt->Style = tbsButton;
               tbt->Action = act;
              }
         else if ( ActName.UpperCase() == "SEP" || ActName.UpperCase() == "SEPARATOR")
              {
               TToolButton* tbt = new TToolButton(tobMain);
               tbt->Parent = tobMain;
               tbt->Style = tbsSeparator;
               tbt->Width = 8;
              }
         else if ( ActName.UpperCase() == "DIV" || ActName.UpperCase() == "DDIVIDERIV" )
              {
               TToolButton* tbt = new TToolButton(tobMain);
               tbt->Parent = tobMain;
               tbt->Style = tbsDivider;
               tbt->Width = 8;
              }
         else if ( ActName.IsEmpty() )
              {
               Log->Warn("Ignoring an empty string in toolbar declaration");
              }
         else {
               Log->Warn("Unknown action \'" + ActName + "\', check spell and case");
              }
        }
    //tobMain->EndUpdate();
    //tobMain->Invalidate();
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::ClearToolButtons(TToolBar* tob)
{ // Clear toolbuttons from a toolbar

    while ( tob->ButtonCount )
          {
           TToolButton* tbt = tob->Buttons[0];
           tbt->Parent = 0;
           delete tbt;
          }
}

//---------------------------------------------------------------------------
TBasicAction* TfrmMplotMain::GetActionFromName (const String& s)
{ // Get action pointer from name (case sensitive)

    for ( int i=0; i<aclMain->ActionCount; ++i )
        {
		 if ( GetNameFromAction(aclMain->Actions[i]) == s ) return aclMain->Actions[i];
        }
    return 0;
}

//---------------------------------------------------------------------------
int TfrmMplotMain::GetActionIdx (String s)
{ // Get action index from name (case sensitive)

    s = s.Trim();
    for ( int i=0; i<aclMain->ActionCount; ++i )
        {
		 if ( GetNameFromAction(aclMain->Actions[i]) == s ) return i;
        }
    return -2;
}

//---------------------------------------------------------------------------
void TfrmMplotMain::ActionNamesToIndexes (const mat::stringlist& actnames, mat::intlist* idxs)
{ // Build an indexes vector from action names

    // . Preliminary things
    idxs->clear();

    // . Build
    String ActName;
    for ( int i=actnames.size()-1; i>=0; --i )
        {
         ActName = actnames[i].Trim();

         if ( ActName == "SEP" || ActName == "DIV" || ActName == "SEPARATOR" || ActName == "DIVIDER")
              { // A separator
               idxs->push_back(-1);
              }
         else if ( ActName.IsEmpty() )
              {
               Log->Warn("Ignoring an empty action name");
               idxs->push_back(-1);
              }
         else {
               idxs->push_back( GetActionIdx(ActName) );
               if ( idxs->back() < 0 ) Log->Warn("Unknown action \'" + ActName + "\', check spell and case");
              }
        }
}

//---------------------------------------------------------------------------
inline String TfrmMplotMain::GetNameFromAction(TContainedAction* a)
{ // Remove 'act' prefix
    return mat::After(a->Name,3);
}


//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::AddActionNamesToHints ()
{ // Add action names to their hints

    for ( int i=0; i<aclMain->ActionCount; ++i )
        {
         TCustomAction* a = dynamic_cast<TCustomAction*>(aclMain->Actions[i]);
		 a->Hint = GetNameFromAction( aclMain->Actions[i] ) + ": " + a->Hint;
        }
}


//---------------------------------------------------------------------------
void TfrmMplotMain::BuildPopMenu (TPopupMenu* pop, const mat::intlist* idxs)
{ // Build pop menu from action indexes of aclMain

    // . Build
    for ( int i=idxs->size()-1; i>=0; --i )
        {
         int idx = (*idxs)[i];
         if ( idx > 0 ) // An action
              {
               TBasicAction* act = aclMain->Actions[idx];
               if (act)
                  {
                   TMenuItem* mit = new TMenuItem(popFoilObject);
                   mit->Action = act;
                   popFoilObject->Items->Add(mit);
                  }
              }
         else if ( idx == -1 ) // A separator
              {
               TMenuItem* mit = new TMenuItem(popFoilObject);
               mit->Caption = "-";
               pop->Items->Add(mit);
              }
         // else Skip unknown action
        }
}


//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::ClearPopMenu (TPopupMenu* pop)
{ // Clear pop menu

    // Delete all top level items on the popup menu}
    while ( pop->Items->Count )
       {
        TMenuItem *mit = pop->Items->Items[0];
        pop->Items->Delete(0);
        delete mit;
       }
}





//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::EnableNormalActions(const bool b)
{ // Enable/Disable an action subset (excluding abort etc)
  // Contract: depends on Tag value!

    for ( int i=0; i<aclMain->ActionCount; ++i )
        {
         TCustomAction* a = dynamic_cast<TCustomAction*>(aclMain->Actions[i]);
         if ( a->Tag ) a->Enabled = b;
        }
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::aclMainUpdate(TBasicAction *Action, bool &Handled)
{ // Update interface status (put Handled to true to stop updating)

/* Iteration by tag/category is possible, but there would be more overhead
    for ( int i=0; i<aclMain->ActionCount; ++i )
        {
         TCustomAction* a = dynamic_cast<TCustomAction*>(aclMain->Actions[i]);
         if (a && a->Tag==3) a->Enabled = tabCams->Visible;
        }
*/

    if ( Busy )
         {
          EnableNormalActions(false);
         }
    else {
              // . Retrieve current status
          bool AtLeastOneFrame = Plot->FramesCount();
          bool MoreThanOneFrame = Plot->FramesCount() > 1;
          bool ThereIsACurrentFrame = Plot->CurrentFrame;
          bool ThereIsACurrentTrack = Plot->CurrentTrack;
          //bool CanEditCurrentFrame = WeAreInEditMode && ThereIsACurrentFrame;
          bool GridVisible = ThereIsACurrentFrame ? Plot->CurrentFrame->Grid->Visible : false;

              // . Actions inhibition
              // . Edit
          //actCopyGrid->Enabled = grdPane->Selection.Bottom-grdPane->Selection.Top>0;
              // . Frame set access
          actClearAll->Enabled = AtLeastOneFrame;
          //actCompute->Enabled = AtLeastOneTrack;
              // . Plot access
          actProperties->Enabled = (Plot->Selected.FoilObject != 0);
          // . . Frame access
          actLockX->Enabled = MoreThanOneFrame;
          actLockY->Enabled = MoreThanOneFrame;
          actRemoveFrame->Enabled = ThereIsACurrentFrame;
          actClearFrame->Enabled = ThereIsACurrentFrame;
          actFrameProperties->Enabled = ThereIsACurrentFrame;
          actAddMarker->Enabled = ThereIsACurrentFrame;
          actAddText->Enabled = ThereIsACurrentFrame;
          actExportFrameToText->Enabled = ThereIsACurrentFrame;
          actExportFrameToMatlab->Enabled = ThereIsACurrentFrame;
          actFitToAxes->Enabled = ThereIsACurrentFrame;
          actFitToTracks->Enabled = ThereIsACurrentFrame;
          actFitToTrack->Enabled = ThereIsACurrentFrame;
          actGotoOrigin->Enabled = ThereIsACurrentFrame;
          actToggleScaleX->Enabled = ThereIsACurrentFrame;
          actToggleScaleY->Enabled = ThereIsACurrentFrame;
          actLegendToggle->Enabled = ThereIsACurrentFrame;
          actGridToggle->Enabled = ThereIsACurrentFrame;
          actGridMore->Enabled = GridVisible;
          actGridLess->Enabled = GridVisible;
          actZoomIn->Enabled = ThereIsACurrentFrame;
          actZoomOut->Enabled = ThereIsACurrentFrame;
          // . . Track access
          actRemoveTrack->Enabled = ThereIsACurrentTrack;
          actTrackProperties->Enabled = ThereIsACurrentTrack;
          actTrackData->Enabled = ThereIsACurrentTrack;
          actMarkPoints->Enabled = ThereIsACurrentTrack;
          actStrokeLines->Enabled = ThereIsACurrentTrack;
          actFillArea->Enabled = ThereIsACurrentTrack;
          actDrawHistograms->Enabled = ThereIsACurrentTrack;
          actShowCoords->Enabled = ThereIsACurrentTrack;
          actTrackStatistics->Enabled = ThereIsACurrentTrack;
          actTrackTransform->Enabled = ThereIsACurrentTrack;

              // . Checked state
              // . Frame
          if ( ThereIsACurrentFrame )
             {
                  // . Plot state
              // TODO 5: in logarithmic, calculated slope is wrong
              if ( Plot->CurrentFrame->XLogarithmic ||
                   Plot->CurrentFrame->YLogarithmic   ) Plot->Drawer->ViewSlopeLine = false;

              // Actions
              actGridToggle->Checked = GridVisible;
              actLegendToggle->Checked = Plot->CurrentFrame->Legend->Visible;
              actToggleScaleX->Checked = Plot->CurrentFrame->XLogarithmic;
              actToggleScaleY->Checked = Plot->CurrentFrame->YLogarithmic;
             }

              // . Track
          if ( ThereIsACurrentTrack )
             {
              actMarkPoints->Checked = Plot->CurrentFrame->CurrentTrack->MarkPoints;
              actStrokeLines->Checked = Plot->CurrentFrame->CurrentTrack->StrokeLines;
              actFillArea->Checked = Plot->CurrentFrame->CurrentTrack->FillArea;
              actDrawHistograms->Checked = Plot->CurrentFrame->CurrentTrack->DrawHistograms;
              actShowCoords->Checked = Plot->CurrentFrame->CurrentTrack->ShowCoords;
             }

              // . Plot
          actSelectMode->Checked = (Plot->Mode == cls_MPlot::MD_SELECT);
          actEditMode->Checked = (Plot->Mode == cls_MPlot::MD_EDIT);
          actPanMode->Checked = (Plot->Mode == cls_MPlot::MD_PAN);
          actZoomMode->Checked = (Plot->Mode == cls_MPlot::MD_ZOOM);
          actMeasureMode->Checked = (Plot->Mode == cls_MPlot::MD_MEASURE);
          actPenMode->Checked = (Plot->Mode == cls_MPlot::MD_PEN);
          actLockX->Checked = Plot->LockX;
          actLockY->Checked = Plot->LockY;
          actMeasureAbscissa->Checked = Plot->Drawer->ViewVerticalLine;
          actMeasureOrdinate->Checked = Plot->Drawer->ViewHorizontalLine;
          actMeasureSlope->Checked = Plot->Drawer->ViewSlopeLine;
          //actViewMeasureMarkers->Checked;
          //actViewMeasureMarkers->Caption = String("View ") + Opts->MeasureMarkers;

              // . Other
          actShowTree->Checked = ctrTree->Visible;
          actShowPane->Checked = ctrPane->Visible;
          actShowLog->Checked = ctrLog->Visible;
         }
}



//---------------------------------------------------------------------------




//================================= Actions =================================


//--------------------------------- File ------------------------------------


//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actOpenExecute(TObject *Sender)
{ // Open a plot or a datafile

    // . Prepare
    cls_FileDialog FileDialog(this);
    // TODO: see supported file types
    FileDialog.Filter = iex::FL_ALL_FILES+"|"+iex::MPlotTextFileType.Filter()+"|"+iex::FL_ALL_TEXTFILES;
    FileDialog.FilterIndex = 1;
    FileDialog.DefaultExt = ""; // appended extension when missing
    FileDialog.InitialDir = Opts->DefaultDir;
    FileDialog.Options >> cls_FileDialog::ofOverwritePrompt
                       << cls_FileDialog::ofAllowMultiSelect
                       << cls_FileDialog::ofFileMustExist;

    //if ( mat::MsgAsk("Do you want to...?") )
    if ( FileDialog.Open() )
         {
          // Perform operations on files
          for ( int i=0; i<FileDialog.Files->Count; ++i)
              {
               CHECKABORT_BREAK
               ProcessFile( FileDialog.Files->Strings[i] );
              }
         }
    else Log->Error(FileDialog.DialogErrorString());
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actSaveExecute(TObject *Sender)
{ // TODO 5: Save the plot

#ifdef _DEBUG
    cls_FileDialog FileDialog(this);
    FileDialog.Filter = iex::MPlotXmlFileType.Filter();
    FileDialog.FilterIndex = 1; // Default filter
    FileDialog.DefaultExt = iex::MPlotXmlFileType.Extension(); // appended extension when missing
    FileDialog.InitialDir = Opts->DefaultDir;
    FileDialog.Options >> cls_FileDialog::ofCreatePrompt
                       >> cls_FileDialog::ofAllowMultiSelect
                       >> cls_FileDialog::ofFileMustExist;
    if ( Opts->PromptOverwrite ) FileDialog.Options << cls_FileDialog::ofOverwritePrompt;
    else FileDialog.Options >> cls_FileDialog::ofOverwritePrompt;

    if ( FileDialog.Save() )
         {
          //trace->Sequence->Save(dlgSave->FileName);
          // If text files abort and actExportToTextExecute(TObject *Sender)
          ToStatusBar("Plot saved in " + ::ExtractFileName(FileDialog.FileName));
         }
    else { // an error occurred
          Log->Entry(FileDialog.DialogErrorString());
         }
#else
    mat::MsgInfo(NotYetImplemented);
#endif
}


//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actExportExecute(TObject *Sender)
{ // Export plot to...

    // . Local vars
    static int FilterIndex = 2;
    static String FileName = String(Opts->DefaultDir) + String("Plot");
    bool jobdone = false;
    cls_Frame* CurrFrame = 0; // Overall plot or current frame only

    // . Overall plot or current frame only?
    // TODO 5: Option for asking?
    CurrFrame = 0;
    if ( Plot->FramesCount()>1 ) if ( mat::MsgAsk("Exporting plot","Do you want to export current frame only?") ) CurrFrame = Plot->CurrentFrame;

    // . Preliminary operations (before trackbar)
        // . Get bitmap
    Log->SubEntry("Capturing Canvas bitmap immediately");
    std::auto_ptr<TImage> PlotImage(new TImage(this));
    ::Sleep(10); // Let the menu disappear
    // Could get both images, then decide later
    Plot->GetCanvasBitmap(PlotImage.get(), CurrFrame);

    // . Visualize save dialog
    cls_GraphicFile::BuildFilters(); // Build 'cls_GraphicFile::Filters'
    cls_FileDialog FileDialog(this);
    // TODO: see available files
    FileDialog.Filter = iex::MPlotTextFileType.Filter()+ "|" +iex::TextFileType.Filter()+ "|" +iex::MPlotMFileType.Filter()+ "|" +iex::EmfFileType.Filter()+ "|" +cls_GraphicFile::Filters+ "|" +iex::SvgFileType.Filter();
    //FileDialog.Filter = String("MattPlot files (.mpl,.mplz)|*.mpl;*.mplz")+ "|" +"Text files (.txt)|*.txt"+ "|" +"Matlab scripts (.m)|*.m"+ "|" +"Enhanced metafiles (.emf)|*.emf"+ "|" +cls_GraphicFile::Filters+ "|" +"Scalable Vector Graphics (.svg)|*.svg";

    FileDialog.FilterIndex = FilterIndex; // Default filter
    //FileDialog.DefaultExt = ""; // appended extension when missing
    FileDialog.InitialDir = Opts->DefaultDir;
    // TODO 5: if exporting frame, use its title as file name
    FileDialog.FileName = FileName;
    FileDialog.Options >> cls_FileDialog::ofCreatePrompt
                       >> cls_FileDialog::ofAllowMultiSelect
                       >> cls_FileDialog::ofFileMustExist;
    if ( Opts->PromptOverwrite ) FileDialog.Options << cls_FileDialog::ofOverwritePrompt;
    else FileDialog.Options >> cls_FileDialog::ofOverwritePrompt;

    if ( FileDialog.Save() )
         {
          FileName = FileDialog.FileName;
          //String FileExt = ::ExtractFileExt(FileDialog.FileName).LowerCase();
          FilterIndex = FileDialog.FilterIndex;
          jobdone = Export(FileDialog.FileName, CurrFrame, PlotImage.get());
         }
    else { // an error occurred
          Log->Entry(FileDialog.DialogErrorString());
         }

    // . Finally
    if ( jobdone )
       {
        Log->Entry("Plot exported to ", FileName);
       }
}


/*
//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actExportFrameExecute(TObject *Sender)
{ // Export current canvas as bitmap image

    // . Check
    cls_Frame* CurrFrame = Plot->CurrentFrame;
    if ( !CurrFrame ) return;

    // . Local vars
    static int FilterIndex = 2;
    String FileName = String(Opts->DefaultDir) + CurrFrame->Name;
    bool jobdone = false;

    Log->Entry("ExportFrameTo...","Frame exporting requested");

    // . Visualize save dialog
    mat::cls_Scope LongTask(&DoLongTask, &EndLongTask);
    cls_FileDialog FileDialog(this);
    FileDialog.Filter = FL_MPLOT_TXTFILES+ "|" +FL_TEXTFILES+ "|" +FL_METAFILES+ "|" +FL_SVGFILES+ "|" +FL_MFILES;
    FileDialog.FilterIndex = FilterIndex; // Default filter
    //FileDialog.DefaultExt = ""; // appended extension when missing
    FileDialog.InitialDir = Opts->DefaultDir;
    FileDialog.FileName = FileName;
    FileDialog.Options >> cls_FileDialog::ofCreatePrompt
                       << cls_FileDialog::ofOverwritePrompt
                       >> cls_FileDialog::ofAllowMultiSelect
                       >> cls_FileDialog::ofFileMustExist;
    if ( Opts->PromptOverwrite ) FileDialog.Options << cls_FileDialog::ofOverwritePrompt;
    else FileDialog.Options >> cls_FileDialog::ofOverwritePrompt;

    if ( FileDialog.Save() )
         {
          //String FileExt = ::ExtractFileExt(FileDialog.FileName).LowerCase();
          FilterIndex = FileDialog.FilterIndex;
          switch ( FilterIndex )
              {
                case 1 : // MattPlot multisession text files (.mpl)
                    jobdone = CurrFrame->ToText(FileDialog.FileName);
                    break;
                case 2 : // Plain text data file (.txt)
                    jobdone = CurrFrame->ToText(FileDialog.FileName, Opts->DataDelimiter);
                    break;
                case 3 : // Enhanced Meta File (.emf)
                    Log->Warn("Export frame to emf","Better export entire plot");
                    jobdone = GetPlotMetafile(true, FileDialog.FileName, false);
                    break;
                case 4 : // Scalable Vector Graphics (.svg)
                    Log->Entry("Export to svg",NotYetImplemented);
                    break;
                case 5 : // m-files (.m)
                    jobdone = CurrFrame->ToMfile(FileDialog.FileName);
                    break;
                default:
                    Log->Error("Unknown file filter, export aborted");
                    // Log->Error("Format \'" + FileExt + "\' not supported, use a valid extension");
              }
         }
    else { // A 'FileDialog' error occurred
          Log->Error(FileDialog.DialogErrorString());
         }

    // . Finally
    if ( jobdone )
       {
        Log->Entry(CurrFrame->Name + " exported to",::ExtractFileName(FileDialog.FileName));
       }
}
*/

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actExportFrameToTextExecute(TObject *Sender)
{ // Export CurrentFrame to plain ASCII text

    // . Check
    cls_Frame* CurrFrame = Plot->CurrentFrame;
    if ( !CurrFrame ) return;

    // . Task
    String TgtName = String(Opts->DefaultDir) + CurrFrame->Name + String(".txt");
    bool skip = false;
    if ( Opts->PromptOverwrite && ::FileExists(TgtName) ) skip = !mat::MsgAsk(String("Do you want to overwrite:\n") + TgtName);

    if ( !skip )
       {
        bool jobdone = CurrFrame->ToText(TgtName, Opts->DataDelimiter);
        if (jobdone) Log->Success("ExportFrame",CurrFrame->Name + " exported to " + ::ExtractFileName(TgtName));
       }
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actExportFrameToMatlabExecute(TObject *Sender)
{ // Pass current frame tracks to Matlab (m-file)

    // . Check
    cls_Frame* CurrFrame = Plot->CurrentFrame;
    if ( !CurrFrame ) return;

    // . Task
    String TgtName = String(Opts->DefaultDir) + CurrFrame->Name + String(".m");
    bool skip = false;
    if ( Opts->PromptOverwrite && ::FileExists(TgtName) ) skip = !mat::MsgAsk(String("Do you want to overwrite:\n") + TgtName);

    if ( !skip )
       {
        bool jobdone = CurrFrame->ToMfile(TgtName);
        if (jobdone) Log->Success("ExportFrame",CurrFrame->Name + " exported to " + ::ExtractFileName(TgtName));
       }
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actPrintExecute(TObject *Sender)
{ // Print current plot

    mat::cls_Scope LongTask(&DoLongTask, &EndLongTask);

    // . Prepare the print dialog
    //TODO 5: put these in the printer unit
    if (actPrint->Checked) return;
    actPrint->Checked = true;

        // . Save print related options of plot
    bool prev_NoBackground = Plot->NoBackground;
    bool prev_MarkCurrentFrame = Plot->MarkCurrentFrame;

    TPrintDialog* dlgPrint = new TPrintDialog(this);
    dlgPrint->Options << poWarning  << poHelp >> poPageNums >> poSelection;
    Graphics::TBitmap* BmpToPrn = new Graphics::TBitmap;

    try { //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    Progress(10,UserAbort); //° ° ° ° ° ° ° ° ° ° ° ° ° ° ° ° ° ° ° ° ° ° ° °

    // . Get bitmap first
    BmpToPrn->Width = Plot->Width;
    BmpToPrn->Height = Plot->Height;
    BmpToPrn->Canvas->CopyRect(Plot->BoundsRect, Plot->CurrentFoil->Canvas, Plot->BoundsRect);

    Progress(20,UserAbort); //° ° ° ° ° ° ° ° ° ° ° ° ° ° ° ° ° ° ° ° ° ° ° °

    // . Decide extra print options
    prn::cls_ExtraPrnOpts ExtraPrnOpts;
    switch ( prn::ChooseExtraPrintOptions( &ExtraPrnOpts ) )
      {
       case mrOk    : break; // Go ahead
       case mrAbort : // Print help requested
                      HH->Open(EnsureHelpFile(GetHelpFilePath()), IDH_Printing);
       case mrRetry : // Double clicked in form
                      Log->SubEntry("Printer aborted by double-clicking");
       default      : // Print revoked
                      throw Exception("Printing aborted by user");
      } // end 'switch' of ModalResult

    // . Print dialog
    if ( dlgPrint->Execute() )
       {
        Log->Entry("MainForm","Printing canvas");

            // . Choose printer
        TPrinter* prn = ::Printer();
        Log->SubEntry("Using printer: " + prn->Printers->Strings[prn->PrinterIndex]);

            // . Set printer properties
        prn->Title = Application->Title + " print " + Now().FormatString("hh.nn dd.mm.yyyy"); // Now().DateTimeString()
        if ( prn->Capabilities.Contains(pcCopies) )
           {
            prn->Copies = dlgPrint->Copies;
            Log->SubEntry("Printer supports copies; set " + String(prn->Copies) + " copies");
           }
        //if ( prn->Capabilities.Contains(pcCollation) ) dlgPrint->Collate;

            // . Dimensions
        // Printable space
        int PrnMargin = (Opts->PrintMargin * mat::min(prn->PageWidth,prn->PageHeight))/100;
        TRect PrintArea;
        if ( ExtraPrnOpts.Mode == prn::PR_SCREEN )
             { // Preserve aspect ratio

                  // . Page orientation
              double GraphAR = double(BmpToPrn->Width)/double(BmpToPrn->Height); // Bitmap Aspect Ratio
              if ( prn->Capabilities.Contains(pcOrientation) )
                 { // Choose dynamically orientation
                  if ( GraphAR <= 1 )
                       {
                        Log->SubEntry("Choosing portrait orientation");
                        prn->Orientation = poPortrait;
                       }
                  else {
                        Log->SubEntry("Choosing landscape orientation");
                        prn->Orientation = poLandscape;
                       }
                 }
                  // . PrintArea
              double PageAR = double(prn->PageWidth)/double(prn->PageHeight); // Page Aspect Ratio
              if ( GraphAR >= PageAR )
                   PrintArea = TRect( PrnMargin,                            // Left
                                      PrnMargin,                            // Top
                                      prn->PageWidth-PrnMargin,             // Right
                                      (prn->PageWidth-2*PrnMargin)/GraphAR  // Bottom
                                     );
              else PrintArea = TRect( PrnMargin,                            // Left
                                      PrnMargin,                            // Top
                                      (prn->PageHeight-2*PrnMargin)*GraphAR,// Right
                                      prn->PageHeight-PrnMargin             // Bottom
                                     );
             }
        else { // Use all print area

                  // . Page orientation
              if ( prn->Capabilities.Contains(pcOrientation) )
                 { // Choose dynamically orientation
                  prn->Orientation = poLandscape;
                  Log->SubEntry("Forcing landscape orientation");
                 }
                  // . PrintArea
              PrintArea = TRect( PrnMargin,                  // Left
                                 PrnMargin,                  // Top
                                 prn->PageWidth-PrnMargin,   // Right
                                 prn->PageHeight-PrnMargin   // Bottom
                                );

                  // . Background
              Plot->NoBackground = ExtraPrnOpts.NoBackground;
             }

        Progress(30,UserAbort); //° ° ° ° ° ° ° ° ° ° ° ° ° ° ° ° ° ° ° ° ° °

            // . Begin printing task
        try {
             //prn->Aborted
             //prn->Printing
             //prn->Fonts
             prn->BeginDoc();
                 // . Title (using default printer font)
             if ( Opts->PrintInfoText ) prn->Canvas->TextOut( PrnMargin, PrnMargin/2, prn->Title );

             Progress(50,UserAbort); // ° ° ° ° ° ° ° ° ° ° ° ° ° ° ° ° ° ° °

                 // . Print
             cls_Foil PrintFoil = cls_Foil(prn->Canvas,PrintArea,double(PrintArea.Width())/double(Screen->Width));
             switch ( ExtraPrnOpts.Mode )
               {
                case prn::PR_PAPER : // Repaint Canvas to have maximum quality
                     Plot->CurrentFoil = &PrintFoil;
                     Plot->Paint();
                     //DrawPlot (prn->Canvas, PrintArea); // Use metafile
                     break;
                case prn::PR_SCREEN : // Just draw current plot screen bitmap
                     //prn->Canvas->Draw(GraphRect.left,GraphRect.top,BmpToPrn); // Simple draw
                     //prn->Canvas->Draw((prn->PageWidth-BmpToPrn->Width)/2,(prn->PageHeight-BmpToPrn->Height)/2,BmpToPrn); // Center on page
                     //prn->Canvas->StretchDraw(GraphRect,BmpToPrn); // (!) can cause problems!
                     prn::StretchDrawDIB(prn->Canvas, PrintArea, BmpToPrn); // Avoid StretchDraw problems
                     break;
               } // end 'switch'
             prn->EndDoc();
            }
        catch (...) {
                     if ( Printer()->Printing ) Printer()->Abort();
                     Log->Error("Printing error");
                    }

        Progress(80,UserAbort); //° ° ° ° ° ° ° ° ° ° ° ° ° ° ° ° ° ° ° ° ° °
       } // end 'dlgPrint->Execute()'
    else Log->SubEntry("Printing aborted by user");
    } //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    __finally {
               Progress(90,UserAbort); // ° ° ° ° ° ° ° ° ° ° ° ° ° ° ° ° ° °
               delete BmpToPrn;
               delete dlgPrint;
               actPrint->Checked = false;

                   // . Restore print related options of plot
               Plot->NoBackground = prev_NoBackground;
               Plot->MarkCurrentFrame = prev_MarkCurrentFrame;
              }

    Log->SubEntry("Printing completed");
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actEditOptionsExecute(TObject *Sender)
{ // Edit options

    // . Get path
    String FilePath = GetOptionFilePath();

    // . Create one if not exists
    if ( !::FileExists(FilePath) )
       {
        Log->Warn("Edit options","INI file does not exists, I\'ll create a new one");
        actStoreOptionsExecute( actEditOptions );
       }

    // . Open file stream and create the editor
    if ( ModalChild ) return;
    ModalChild = new TfrmSynEditor(this,FilePath); // TfrmSynEditor* frmEditor
    try {
         if ( ModalChild->ShowModal() == mrYes ) actReadOptionsExecute(Sender); // Read modified options
        }
    __finally {
               delete ModalChild;
               ModalChild = 0;
              }
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actReadOptionsExecute(TObject *Sender)
{ // Reload options from MattPlot ini file

    // . Read from file in Opts
    // Warning: if activated by toolbutton, there's anexception caused by
    // the fact that 'BuildToolbar' deletes current ToolButton.
    Opts->ReadAllFrom( GetOptionFilePath() );
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actStoreOptionsExecute(TObject *Sender)
{ // Save options in MattPlot ini file

    // . Write Opts to file
        Opts->WriteAllTo( GetOptionFilePath() );

    // . Now re-read all
        Opts->ReadAllFrom( GetOptionFilePath() );
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actSetDefaultsExecute(TObject *Sender)
{ // Set defaults
    Opts->SetDefaults();
    AfterReadOptions();

}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actSetStylePlainExecute(TObject *Sender)
{ // Set a palin style, good for printing

    // [General]
    Opts->FloatPrecision = 12;
    Opts->FloatDigits = 2;
    //Opts->LagThreshold = 200;
    //Opts->DevelopMode = false;

    // [MainForm]
    Opts->FormColor = TColor(0xD8E9EC);
    Opts->ToolsColor = clWhite;
    Opts->HighlightColor = clYellow;
    Opts->EnableMainMenu = false;
    Opts->EnableToolbar = true;
    Opts->Toolbar = "Open,Export,Print,SEP,SEP,FitToAxes,SEP,SEP,PanMode,ZoomMode,MeasureMode,SEP,SEP,StayOnTop,Exit";
    Opts->ObjectMenu = "Properties,SEP,Remove";
    Opts->FrameMenu = "RemoveFrame,SEP,LegendToggle,GridToggle";
    Opts->TrackMenu = "RemoveTrack";
    Opts->LogSite = "Bottom";
    Opts->PaneSite = "Right";
    Opts->AutoPane = false;
    //Opts->PaneQuantities= "";

    // [Exporter]
    Opts->PrintMargin = 3;

    // [Plot]
    Opts->FirstMode = "ZoomMode";
    Opts->PlotMargin = 5;
    Opts->PlotFont = "Verdana,12,0x0";

    // [Frames]
    Opts->PlotAlsoOnMargins = false;
    Opts->CenterTitle = true;
    Opts->TitleMag = 120;
    Opts->FrameBorder = 1;
    Opts->FrameMarginX = 70;
    Opts->FrameMarginY = 40;
    Opts->FrameColor = clWhite;

    // [Axes]
    Opts->AxesColor = clBlack;
    Opts->CenterAxesLabels = true;

    // [Legend]
    Opts->LegendSpacing = 5;
    Opts->LegendStrokeLength = 30;
    Opts->LegendColor = clWhite;

    // [Grid]
    Opts->VGridDist = 40;
    Opts->HGridDist = 25;
    Opts->VGridDivs = "1\f2\f5";
    Opts->HGridDivs = "1\f2\f5";
    Opts->GridLinesStyle = "...";
    Opts->GridColor = clGray;
    Opts->LabelFont = "Small Fonts,6,0x0";

    // [Tracks]
    Opts->TrackLineWeight = 1;
    Opts->TrackLineColorSeed = clRed;
    Opts->TrackLineStyle = "___";
    Opts->TrackPointStyle = '+';
    Opts->TrackMarkPoints = false;
    Opts->TrackStrokeLines = true;
    Opts->TrackFillArea = false;
    Opts->TrackDrawHistograms = false;
    Opts->TrackShowCoords = false;
    Opts->TrackCoordsDist = 80;

    // [EditMode]
    Opts->MarkColor = clGray;
    Opts->MarkZoneStyle = "xx";

    // [PanMode]
    Opts->DefaultPan = 10;
    Opts->CoastingTau = 0;
    Opts->ContinuousPan = 7;

    // [ZoomMode]
    Opts->SuddenZoom = false;
    Opts->ZoomPerc = 50;
    Opts->SelRectLineStyle = "...";
    Opts->SelRectLineColor = clGray;
    Opts->SelRectAreaColor = clGray;

    // [MeasureMode]
    Opts->ViewAbscissaLine = true;
    Opts->ViewOrdinateLine = true;
    Opts->ViewSlopeLine = false;
    Opts->MeasureMarkers = "ymin,ymax,avg";
    Opts->MeasureColor = clGray;
    Opts->MeasureLinesStyle = "_ _";

    // [PenMode]
    Opts->PencilColor = clGray;
    Opts->PencilSize = 1;
    Opts->HighlighterColor = clYellow;
    Opts->HighlighterSize = 40;

    // . Finally
    AfterReadOptions();
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actSetStylePSpiceExecute(TObject *Sender)
{ // set a PSpice style

    // [General]
    //Opts->DefaultDir = "exe";
    Opts->FloatPrecision = 12;
    Opts->FloatDigits = 2;
    //Opts->LagThreshold = 200;
    //Opts->DevelopMode = false;

    // [MainForm]
    Opts->FormColor = TColor(0xD8E9EC);
    Opts->ToolsColor = clBtnFace;
    Opts->HighlightColor = TColor(0x70060D); //TColor(0x525552);
    Opts->EnableMainMenu = true;
    Opts->EnableToolbar = true;
    Opts->Toolbar = "Open,Export,Print,SEP,SEP,SEP,LockX,LockY,FitToAxes,SEP,SEP,SEP,SelectMode,EditMode,PanMode,ZoomMode,MeasureMode,PenMode";
    Opts->ObjectMenu = "Properties,SEP,Remove";
    Opts->FrameMenu = "RemoveFrame,AddMarker,AddText,SEP,LegendToggle,GridToggle";
    Opts->TrackMenu = "RemoveTrack,SEP,MarkPoints,StrokeLines";
    Opts->LogSite = "Right";
    Opts->PaneSite = "Bottom";
    Opts->AutoPane = false;
    //Opts->PaneQuantities= "";

    // [Exporter]
    Opts->PrintMargin = 5;

    // [Plot]
    Opts->FirstMode = "PanMode";
    Opts->PlotMargin = 5;
    Opts->PlotFont = "Fixedsys,8,0xADAAAD";

    // [Frames]
    //Opts->CenterTitle = true;
    Opts->FrameBorder = 1;
    Opts->FrameMarginX = 50;
    Opts->FrameMarginY = 35;
    Opts->FrameColor = clBlack;

    // [Axes]
    Opts->AxesColor = RGB(82,85,82);
    //Opts->CenterAxesLabels = true;

    // [Legend]
    Opts->LegendSpacing = 5;
    Opts->LegendStrokeLength = 30;
    Opts->LegendColor = TColor(0xADAAAD);

    // [Grid]
    Opts->VGridDist = 30;
    Opts->HGridDist = 15;
    Opts->VGridDivs = "1\f2\f5";
    Opts->HGridDivs = "1\f2\f5";
    Opts->GridLinesStyle = "...";
    Opts->GridColor = RGB(173,170,173);
    Opts->LabelFont = "Fixedsys,8,0xADAAAD";

    // [Tracks]
    Opts->TrackLineWeight = 1;
    Opts->TrackLineColorSeed = RGB(255,85,82);
    Opts->TrackLineStyle = "___";
    Opts->TrackPointStyle = 'q';
    Opts->TrackMarkPoints = false;
    Opts->TrackStrokeLines = true;
    Opts->TrackFillArea = false;
    Opts->TrackDrawHistograms = false;
    Opts->TrackShowCoords = false;
    Opts->TrackCoordsDist = 80;

    // [EditMode]
    Opts->MarkColor = RGB(132,156,132);
    Opts->MarkZoneStyle = "xx";

    // [PanMode]
    Opts->DefaultPan = 10;
    Opts->CoastingTau = 0;
    Opts->ContinuousPan = 7;

    // [ZoomMode]
    Opts->SuddenZoom = true;
    Opts->ZoomPerc = 50;
    Opts->SelRectLineStyle = "...";
    Opts->SelRectLineColor = RGB(132,123,167);
    Opts->SelRectAreaColor = RGB(240,240,198);

    // [MeasureMode]
    Opts->ViewAbscissaLine = true;
    Opts->ViewOrdinateLine = true;
    Opts->ViewSlopeLine = false;
    Opts->MeasureMarkers = "tmin,tmax,avg,rms";
    Opts->MeasureColor = RGB(132,25,233);
    Opts->MeasureLinesStyle = "_ _";

    // [PenMode]
    Opts->PencilColor = RGB(255,255,0);
    Opts->PencilSize = 3;
    Opts->HighlighterColor = RGB(165,165,255);
    Opts->HighlighterSize = 50;

    // . Finally
    AfterReadOptions();
}


//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actExitExecute(TObject *Sender)
{ // Exit application
    UserAbort = true;
    ::Sleep(1);
    Close();
}



//-------------------------------- Plot -------------------------------------


//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actCopyGridExecute(TObject *Sender)
{ // grpPane Clipboard utility

    //if ( Clipboard()->HasFormat(CF_TEXT) )
    // Build string
    String cltxt = "";
    //for ( int i=0; i<grdPane->RowCount; ++i )
    for ( int i=grdPane->Selection.Top; i<=grdPane->Selection.Bottom; ++i )
        {
         cltxt += grdPane->Cells[0][i] + "\t" + grdPane->Cells[1][i] + "\r\n";
        }
    Clipboard()->AsText = cltxt;
}


//-------------------------------- Plot -------------------------------------


//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actSelectModeExecute(TObject *Sender)
{ // Edit mode
    Plot->Mode = cls_MPlot::MD_SELECT;
    if ( Opts->AutoPane && ctrPane->Visible ) actShowPaneExecute(Sender);
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actPanModeExecute(TObject *Sender)
{ // Pan mode
    Plot->Mode = cls_MPlot::MD_PAN;
    if ( Opts->AutoPane && ctrPane->Visible ) actShowPaneExecute(Sender);
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actZoomModeExecute(TObject *Sender)
{ // Zoom mode
    Plot->Mode = cls_MPlot::MD_ZOOM;
    if ( Opts->AutoPane && ctrPane->Visible ) actShowPaneExecute(Sender);
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actMeasureModeExecute(TObject *Sender)
{ // Measure mode
    Plot->Mode = cls_MPlot::MD_MEASURE;
    if ( Opts->AutoPane && !ctrPane->Visible ) actShowPaneExecute(Sender);
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actEditModeExecute(TObject *Sender)
{ // Edit mode
    Plot->Mode = cls_MPlot::MD_EDIT;
    if ( Opts->AutoPane && ctrPane->Visible ) actShowPaneExecute(Sender);
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actPenModeExecute(TObject *Sender)
{ // Pen mode
    Plot->Mode = cls_MPlot::MD_PEN;
    if ( Opts->AutoPane && ctrPane->Visible ) actShowPaneExecute(Sender);
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actRefreshExecute(TObject *Sender)
{ // Refresh plot
    Plot->Invalidate();
    OnPlotUpdate();
    OnPlotChange();
}


//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actSnapshotExecute(TObject *Sender)
{ // Snapshot action

    mat::cls_Scope LongTask(&DoLongTask, &EndLongTask);

    // . Settings
    const String path = String(Opts->DefaultDir) + "Snapshot.gif";

    // . Get bitmap first
    std::auto_ptr<TImage> PlotImage(new TImage(this));
    ::Sleep(10); // Let the menu disappear
    Plot->GetCanvasBitmap( PlotImage.get() );

    if ( Opts->SnapshotAction & 1 )
       {
        SavePlotBitmap(path, PlotImage.get());
        try { mat::ShellExecute(path,""); }
        catch(Exception& e) {Log->Error(e.Message);}
       }

    if ( Opts->SnapshotAction & 2 )
       {
        GetPlotMetafile(0, "", true);
       }
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actStayOnTopExecute(TObject *Sender)
{ // Toggle normal/always in foreground

    actStayOnTop->Checked = !actStayOnTop->Checked;
    HWND hWndInsertAfter = actStayOnTop->Checked ? HWND_TOPMOST : HWND_NOTOPMOST;
    ::SetWindowPos(Application->MainForm->Handle,hWndInsertAfter,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actMeasureAbscissaExecute(TObject *Sender)
{ // View abscissa line in measure mode
    Plot->Drawer->ViewVerticalLine = !Plot->Drawer->ViewVerticalLine;
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actMeasureOrdinateExecute(TObject *Sender)
{ // Toggle track ordinate measure
    Plot->Drawer->ViewHorizontalLine = !Plot->Drawer->ViewHorizontalLine;
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actMeasureSlopeExecute(TObject *Sender)
{ // Toggle track slope measure
    Plot->Drawer->ViewSlopeLine = !Plot->Drawer->ViewSlopeLine;
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actViewMeasureMarkersExecute(TObject *Sender)
{ // Toggle Measure Markers

    if ( Plot->CurrentFrame )
       {
        if ( actViewMeasureMarkers->Checked )
             { // Remove measure markers
              Plot->CurrentFrame->Markers.RemoveDeferenced();
              actViewMeasureMarkers->Checked = false;
             }
        else {
              mat::stringlist qs;
              mat::Tokenize(qs, String(Opts->MeasureMarkers).c_str(), ',');
              for ( unsigned int i=0; i<qs.size(); ++i )
                  {
                   double* q = Plot->GetValuePointer(qs[i]);
                   if ( q == 0 ) Log->Error("MeasureMarkers","Unknown quantity " + Opts->MeasureMarkers);
                   else {
                         // TODO: Discriminate Vertical
                         bool vert = (qs[i][1]=='t')? true : false;
                         Plot->CurrentFrame->Markers.Add(Plot->CurrentFrame,q,vert);
                         Plot->CurrentFrame->Markers.back()->Prefix = qs[i] + String(": ");
                        }
                  }
              actViewMeasureMarkers->Checked = true;
             }
       }
    Plot->Invalidate();
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actPropertiesExecute(TObject *Sender)
{ // Foil object properties

    if ( ModalChild || Plot->Selected.FoilObject==0 ) return;
    ModalChild = new TfrmProperties(this,Plot,Plot->Selected.FoilObject);
    if ( ModalChild->ShowModal() == mrYes ) Opts->WriteAllTo( GetOptionFilePath() );
    delete ModalChild;
    ModalChild = 0;
    Plot->Invalidate();
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actRemoveExecute(TObject *Sender)
{ // Remove clicked object
    if ( Plot->Selected.FoilObject ) Plot->Selected.FoilObject->Remove();
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actExecMacroExecute(TObject *Sender)
{
    // TODO 3: Opts->Macro
    mat::MsgInfo(NotYetImplemented);
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actAbortExecute(TObject *Sender)
{ // Abort current operation
    UserAbort = true;
}



//-------------------------------- Frame ------------------------------------


//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actAddFrameExecute(TObject *Sender)
{ // Add frame
    Plot->PrepareNewFrame();
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actRemoveFrameExecute(TObject *Sender)
{ // Remove frame
    Plot->RemoveFrame(); // Remove selected frame
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actClearFrameExecute(TObject *Sender)
{ // Clear frame
    if ( Plot->CurrentFrame ) Plot->CurrentFrame->ClearTracks();
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actFramePropertiesExecute(TObject *Sender)
{ // Edit frame appearance (frame)

    if ( ModalChild ) return;
    ModalChild = new TfrmProperties(this,Plot,Plot->CurrentFrame);
    if ( ModalChild->ShowModal() == mrYes ) Opts->WriteAllTo( GetOptionFilePath() );
    delete ModalChild;
    ModalChild = 0;
    Plot->Invalidate();
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actClearAllExecute(TObject *Sender)
{ // Clear Plot
    Plot->ClearFrames();
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actLockXExecute(TObject *Sender)
{ // Toggle lock X
    Plot->LockX = !Plot->LockX;
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actLockYExecute(TObject *Sender)
{ // Toggle lock Y
    Plot->LockY = !Plot->LockY;
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actFitToAxesExecute(TObject *Sender)
{ // Adapt scale to fit contained tracks and axes
    Plot->FitScale(true,true);
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actFitToTracksExecute(TObject *Sender)
{ // Adapt selected frame scale according to contained tracks
    Plot->FitScale(true,false);
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actFitToTrackExecute(TObject *Sender)
{ // Adapt selected frame scale according to current track
    Plot->FitScale(false,false);
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actGotoOriginExecute(TObject *Sender)
{ // Force scale to view frame axes (frame)
    Plot->ResetOrigin();
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actToggleScaleXExecute(TObject *Sender)
{ // Switch linear/logarithmic scale for X axis
    Plot->ToggleLogScaleX();
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actToggleScaleYExecute(TObject *Sender)
{ // Switch linear/logarithmic scale for Y axis
    Plot->ToggleLogScaleY();
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actLegendToggleExecute(TObject *Sender)
{ // Toggle frame legend
    Plot->ToggleLegend();
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actGridToggleExecute(TObject *Sender)
{ // Toggle grid (frame)
    Plot->ToggleGrid();
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actGridMoreExecute(TObject *Sender)
{ // Thickening grid lines (frame)
    Plot->ExpandGrid(-1);
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actGridLessExecute(TObject *Sender)
{ // Thinning grid lines
    Plot->ExpandGrid(1);
}


//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actZoomInExecute(TObject *Sender)
{ // Default Zoom In
    Plot->DefaultZoom(true,false,false);
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actZoomOutExecute(TObject *Sender)
{ // Default Zoom Out
    Plot->DefaultZoom(false,false,false);
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actAddMarkerExecute(TObject *Sender)
{ // Add marker to current frame

    cls_Marker* newmk = new cls_Marker(Plot->CurrentFrame);
    ModalChild = new TfrmAddFoilObject(this,Plot,newmk);
    if ( ModalChild->ShowModal() == mrOk )
         {
          Plot->CurrentFrame->Markers.Add(newmk);
         }
    else {
          delete newmk;
         }

    delete ModalChild;
    ModalChild = 0;
    Plot->Invalidate();
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actAddTextExecute(TObject *Sender)
{ // Add a text label in a given position

    cls_Label* newlb = new cls_Label(Plot->CurrentFrame);
    ModalChild = new TfrmAddFoilObject(this,Plot,newlb);
    if ( ModalChild->ShowModal() == mrOk )
         {
          Plot->CurrentFrame->Labels.Add(newlb);
         }
    else {
          delete newlb;
         }

    delete ModalChild;
    ModalChild = 0;

    //Plot->CurrentFrame->Labels.Add(Plot->CurrentFrame,"new text", Plot->CurrentFrame->xmin+Plot->CurrentFrame->xrange/2, Plot->CurrentFrame->ymin+Plot->CurrentFrame->yrange/2, false);
    Plot->Invalidate();
}




//-------------------------------- Track ------------------------------------


//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actAddTrackExecute(TObject *Sender)
{ // TODO 4: Add track

    mat::MsgInfo(NotYetImplemented);
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actRemoveTrackExecute(TObject *Sender)
{ // Remove track

    Plot->CurrentTrack_Remove();
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actTrackPropertiesExecute(TObject *Sender)
{ // Edit track appearance (track)

    if ( ModalChild ) return;
    ModalChild = new TfrmProperties(this,Plot,Plot->CurrentTrack);
    if ( ModalChild->ShowModal() == mrYes ) Opts->WriteAllTo( GetOptionFilePath() );
    delete ModalChild;
    ModalChild = 0;
    Plot->Invalidate();
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actTrackDataExecute(TObject *Sender)
{ // Edit the selected track data (track)

    if ( ModalChild ) return;
    ModalChild = new TfrmSynEditor(Application->MainForm);
    TStrings* lines = dynamic_cast<TfrmSynEditor*>(ModalChild)->synEditorLines();
    try {
         Plot->CurrentTrack->DataToStrings( const_cast<TStrings*>(lines) );
         if ( ModalChild->ShowModal() == mrYes )
            {
             // TODO 2: undo when exception
             Plot->CurrentTrack->StringsToData(lines);
            }
        } 
    __finally {
               delete ModalChild;
               ModalChild = 0;
              }
}


//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actMarkPointsExecute(TObject *Sender)
{ // Mark selected track point
    Plot->CurrentTrack_ToggleMarkPoints();
}

//---------------------------------------------------------------------------

void __fastcall TfrmMplotMain::actStrokeLinesExecute(TObject *Sender)
{ // Stroke selected track lines
    Plot->CurrentTrack_ToggleStrokeLines();
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actFillAreaExecute(TObject *Sender)
{ // Fill selected track area
    Plot->CurrentTrack_ToggleFillArea();
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actDrawHistogramsExecute(TObject *Sender)
{ // Draw selected track histograms
    Plot->CurrentTrack_ToggleDrawHistograms();
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actShowCoordsExecute(TObject *Sender)
{ // Show track point coordinates
    Plot->CurrentTrack_ToggleShowCoords();
}


//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actTrackStatisticsExecute(TObject *Sender)
{ // Calculate and display selected interval statistics (track)

    Plot->CurrentTrack_DisplayStatistics();
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actTrackTransformExecute(TObject *Sender)
{ // Transform track

    if (ModalChild) return;
    mat::cls_Scope LongTask(&DoLongTask, &EndLongTask);

    ModalChild = new TfrmTrackTransform(this,Plot);
    tobMain->HelpContext = IDH_Transforming; // Toolbar help context
    // TODO 3: TrtrForm->OnProgress = Progress;

    try       {
               ModalChild->ShowModal();
              }
    __finally {
               delete ModalChild;
               ModalChild = 0;
              }
    Plot->Invalidate();
}


//-------------------------------- Tools ------------------------------------

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actShowTreeExecute(TObject *Sender)
{ // View plot frames and tracks tree

    static bool FirstTime = true;
    if ( FirstTime )
         { // First time dock in initial position
          ctrTree->Tag = ClientWidth / 5;
          ctrTree->Width = ctrTree->Tag;
          ctrTree->Height = ClientHeight;
          ctrTree->ManualDock(ctrLeftDock,NULL,alClient);
          // Populate
          Plot->FillTreeView(treePlot,false);
          ctrTree->Visible = true;
          FirstTime = false;
         }
    else { // Toggle
          if ( ctrTree->Visible )
               {
                // Remember size
                ctrTree->Tag = ctrTree->Width<ctrTree->Height ? ctrTree->Width : ctrTree->Height;
                ctrTree->Visible = false;
                treePlot->Items->Clear();
               }
          else {
                // Manage dockhost size
                if ( ctrTree->Parent->Parent == this ) DockOn(ctrTree->Parent, ctrTree);
                // Populate
                Plot->FillTreeView(treePlot,false);
                ctrTree->Visible = true;
               }
         }
}


//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actShowPaneExecute(TObject *Sender)
{ // Show display: is fixes to right panel

    static bool FirstTime = true;
    if ( FirstTime )
         { // First time dock in initial position
          ctrPane->Tag = ClientWidth / 5;
          if ( String(Opts->PaneSite).LowerCase() == "bottom" )
               {
                ctrPane->Width = ClientWidth;
                ctrPane->Height = ctrPane->Tag;
                ctrPane->ManualDock(ctrBottomDock,NULL,alClient);
               }
          else if ( String(Opts->PaneSite).LowerCase() == "right" )
               {
                ctrPane->Width = ctrPane->Tag;
                ctrPane->Height = ClientHeight;
                ctrPane->ManualDock(ctrRightDock,NULL,alClient);
               }
          else {
                ctrPane->ManualFloat( TRect(Left+Width,Top,Left+5*(Width/4),Top+Height) );
               }
          ctrPane->Visible = true;
          FirstTime = false;
         }
    else { // Normal operations
          if ( ctrPane->Visible )
               {
                // Remember size
                ctrPane->Tag = ctrPane->Width<ctrPane->Height ? ctrPane->Width : ctrPane->Height;
                ctrPane->Visible = false;
                grdPane->Cols[1]->Clear();
               }
          else {
                // Manage dockhost size
                if ( ctrPane->Parent->Parent == this ) DockOn(ctrPane->Parent, ctrPane);
                // Adjust columns width
                grdPane->ColWidths[0] = 15 + grdPane->Canvas->TextWidth("DYmax");
                grdPane->ColWidths[1] = grdPane->Width - grdPane->ColWidths[0] - 8;
                // Re-read symbols
                BuildGridPane(grdPane);
                ctrPane->Visible = true;
               }
         }
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actShowLogExecute(TObject *Sender)
{ // View Log

    static bool FirstTime = true;
    if ( FirstTime )
         { // First time dock in initial position
          // Floating caption
          ctrLog->Caption = Log->Title; //memLog->Lines->Strings[0] = String("\t") + Log->Title;
          // Dock
          ctrLog->Tag = ClientHeight / 5;
          if ( String(Opts->LogSite).LowerCase() == "bottom" )
               {
                ctrLog->Width = ClientWidth;
                ctrLog->Height = ctrLog->Tag;
                ctrLog->ManualDock(ctrBottomDock,NULL,alClient);
               }
          else if ( String(Opts->LogSite).LowerCase() == "right" )
               {
                ctrLog->Width = ctrLog->Tag;
                ctrLog->Height = ClientHeight;
                ctrLog->ManualDock(ctrRightDock,NULL,alClient);
               }
          else {
                ctrLog->ManualFloat( TRect(Left+Width/2,Top+Height/2,Left+6*Width/5,Top+6*Height/5) );
               }
          ctrLog->Visible = true;
          FirstTime = false;
         }
    else { // Normal operations
          if ( ctrLog->Visible )
               {
                // Remember size
                ctrLog->Tag = ctrLog->Width<ctrLog->Height ? ctrLog->Width : ctrLog->Height;
                ctrLog->Visible = false;
               }
          else {
                // Manage dockhost size
                if ( ctrLog->Parent->Parent == this ) DockOn(ctrLog->Parent, ctrLog);
                ctrLog->Visible = true;
               }
         }
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actTextEditorExecute(TObject *Sender)
{ // Open an instance of text editor

    // TODO 4: disable main form shortcut keys
    TfrmSynEditor* frmEditor = new TfrmSynEditor(Application->MainForm);
    frmEditor->Show();
}


//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actComputeExecute(TObject *Sender)
{ // TODO 4: Tracks elaboration form

    mat::MsgInfo(NotYetImplemented);
}



//-------------------------------- Help -------------------------------------

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actUpdateExecute(TObject *Sender)
{ // Update

    try { mat::ShellExecute( ThisExe.WebSite ); }
    catch(Exception& e) {Log->Error(e.Message);}
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actDemoExecute(TObject *Sender)
{ // A demonstration of MattPlot

    Log->Entry("Action","Demo requested");

    static int k = 0;
    if ( k++%2 ) Plot->Test2();
    else Plot->Test();
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actExperimentalStuffExecute(TObject *Sender)
{ // Experimental stuff

    Plot->Test_Experiments();
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actHelpContentsExecute(TObject *Sender)
{ // Show help
    Application->HelpContext(HelpContext);
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actWhatsNewExecute(TObject *Sender)
{ // What's new
    Application->HelpContext(IDH_WhatsNew);
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actAboutExecute(TObject *Sender)
{ // Informations about this program

    if ( actAbout->Checked ) return;
    actAbout->Checked = true;
    //::PlaySound(MAKEINTRESOURCE(ID_WAV_JINGLE),HInstance,SND_ASYNC|SND_LOOP|SND_RESOURCE|SND_NOWAIT|SND_NODEFAULT );
    TfrmAbout* frmAbout = new TfrmAbout(this,&ThisExe);
    try {
         //frmAbout->ShowModal();
         frmAbout->FadeIn(200);
         frmAbout->WaitClose();
        }
    __finally {
               delete frmAbout;
               frmAbout = 0;
               //::PlaySound(NULL,0,0);
               actAbout->Checked = false;
              }
}




//-------------------------------- Hidden -----------------------------------

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actTrySomethingExecute(TObject *Sender)
{
    Plot->TryColorGenerator();
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::actEasterEggExecute(TObject *Sender)
{// An easter egg...  TODO: decide here

    static unsigned int n = 0;
    switch ( n %= 2 )
       {
        case 0 : Plot->DrawRandomShapes(100); break;
        case 1 : Plot->DrawRandomCurve(100); break;
       }
    n++;
}

//---------------------------------------------------------------------------



//============================== Low-level uts ==============================

/*---------------------------------------------------------------------------
inline int TfrmMplotMain::VisibleControlCount(TWinControl *wctr)
{ // Count visible controls contained in a wincontrol
    int VisCtrls = 0;
    for (int i; i<wctr->ControlCount; ++i) if (wctr->Controls[i]->Visible) ++VisCtrls;
    return VisCtrls;
}*/

/*---------------------------------------------------------------------------
inline void TfrmMplotMain::PutMemoryStatus(en_StatusBarPanels pnl)
{ // Put used memory to statusbar; pnl should be 'SP_SYS'
        // . Use win32API to retrieve system memory informations
    static MEMORYSTATUS mems;
    ::ZeroMemory(&mems,sizeof (MEMORYSTATUS));
    mems.dwLength = sizeof (MEMORYSTATUS);
    ::GlobalMemoryStatus (&mems);
    ToStatusBar(String(mems.dwMemoryLoad)+"% used",pnl);
    //"Installed RAM: " + String( ceil(mems.dwTotalPhys/mat::OneMegaByte) ) + "Mb, " + String(mems.dwMemoryLoad) + "% in use" + " (" + String( ceil(mems.dwAvailPhys/OneMega) ) + "Mb free)";
    //"Paging File: " + String( ceil(mems.dwTotalPageFile/mat::OneMegaByte) ) + "Mb (" + FloatToStrF ( 100 * float (mems.dwAvailPageFile) / mems.dwTotalPageFile,ffGeneral,5,1) + "% free)";

}*/




//================================== Events =================================


//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::OnPlotUpdate()
{ // Plot's updated: update interface

    // . Statusbar
    ToStatusBar(Plot->SelectedStuff,SP_SEL); // Put selected stuff
    if (Plot->Mode == cls_MPlot::MD_ZOOM) ToStatusBar(Plot->PixelSize,SP_PNT); // Put pixel size
    else ToStatusBar(Plot->CurrentPoint,SP_PNT); // Put current point

    // . Output grid (values monitor)
      for (int i=0; i<grdPane->RowCount; ++i) grdPane->Cells[1][i] = (grdPane->Objects[1][i]) ? Plot->ToString(*(reinterpret_cast<double*>(grdPane->Objects[1][i]))) : mat::NaNstr;
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::OnPlotChange()
{ // Plot's layout changed

    // . Update plot frame/tracks tree
    if ( ctrTree && ctrTree->Visible && !Closing )
       {
        Plot->FillTreeView(treePlot,false);
       }
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::OnPlotPaint()
{ // Plot just painted

    // . Refresh time
    ToStatusBar(Plot->RefreshTime, SP_SYS);
    if ( Plot->Slow ) ToStatusBar("Paint is slow: try to reduce plotted tracks!",SP_MEX);
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::FormDeactivate(TObject *Sender)
{ // When loosing focus to another application form, avoid measure lines
    Plot->Invalidate();
}

//---------------------------------------------------------------------------
// First run tasks
void __fastcall TfrmMplotMain::OneShotThings(TObject *Sender, bool &Done)
{
        // . Before processing cmd arguments
    Application->OnIdle = 0;
    Repaint(); // Paint form before processing files

        // . Command line arguments handling (now to have a visible form)
    // Build passed arguments
    mat::stringlist passedargs;
    for (int i=1; i<=ParamCount(); ++i) passedargs.push_back(ParamStr(i));

    // Process them
    ProcessArguments(passedargs);

    Done = true;
    Application->OnIdle = IdleStuff;
}

//---------------------------------------------------------------------------
// Idle stuff
void __fastcall TfrmMplotMain::IdleStuff(TObject *Sender, bool &Done)
{ 
    // . StatusBar
    //if ( !PrevMessage.IsEmpty() ) ToStatusBar(PrevMessage); // Restore message cancelled by an Hint
    //PrevMessage = "";
    // if ( !Plot->IsOverFrame )
    ToStatusBar(Plot->SelectedStuff,SP_SEL); // for sure

    // . Maintain docking site sizes: hide if has no more visible docked controls
    if ( ctrRightDock->VisibleDockClientCount < 1 ) UnDockFrom(ctrRightDock);
    if ( ctrLeftDock->VisibleDockClientCount < 1 ) UnDockFrom(ctrLeftDock);
    if ( ctrBottomDock->VisibleDockClientCount < 1 ) UnDockFrom(ctrBottomDock);
    //for (int i=0; i<ctrBottomDock->DockClientCount; ++i) ctrBottomDock->DockClients[i]->Height = Height/5;

    // focus on plot
    //ActiveControl = pnlPlotArea;

// TODO 4: resize statusbar panels according to content
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::appEventsHint(TObject *Sender)
{ // Display hints in statusbar. Sender is always 'Application'

    // . Display Hint in Statusbar
    //PrevMessage = FromStatusBar(); // Keep the displayed message
    TApplication* a = dynamic_cast<TApplication*>(Sender);
    if (a) ToStatusBar( ::GetLongHint(a->Hint) );

    // . See on which Frame/Track action will be applied
    ToStatusBar(Plot->SelectedStuff,SP_SEL);
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::appEventsException(TObject *Sender, Exception *E)
{ // Generic Exception Handler

    try { // Be robust on further exception
         Log->Error(E->Message);
         //Application->ShowException(E);

         if ( Application->Terminated ) Log->Save(); // Crash log
        }
    catch(...){}
    //Application->Terminate();
}

//---------------------------------------------------------------------------
bool __fastcall TfrmMplotMain::appEventsHelp(WORD Command, int Data, bool &CallHelp)
{ // Help event handler:
  // Winhelp context calls come here and get diverted to HTML Help Context call

    CallHelp = false; // VCL should not call WinHelp
    if ( Application->Terminated ) return true;

    #ifdef HELP_DEBUG
      ToStatusBar("Help Command: " + cls_HtmlHelp::WinHelpCmdToStr(Command) + " ("+String(Command) + ")   Data: " + String(Data));
    #endif

    switch ( Command )
       {
        case HELP_CONTEXT :
            {
             // Displays the topic identified by the specified context identifier defined in the [MAP] section
             // Data: Unsigned long integer containing the context identifier for the topic
             HH->Open(EnsureHelpFile(GetHelpFilePath()), Data);
             //HH->Open(EnsureHelpFile(GetHelpFilePath()), "Topic1.htm");

             //HH->ToForeground();
             return true;
            }
        case HELP_CONTEXTPOPUP :
            {
             // Displays, in a pop-up window, the topic identified by the specified context identifier defined in the [MAP] section
             // Data: Unsigned long integer containing the context identifier for a topic
             HH->Popup(EnsureHelpFile(GetHelpFilePath()), Data);
             return true;
            }
        default:
            {
             // No mapped command, simply execute generic help action
             actHelpContentsExecute(this);
             return true;
            }
       }
/*  WinHelp commands
HELP_COMMAND    Executes a Help macro or macro string.  Address of a string that specifies the name of the Help macro(s) to execute. If the string specifies multiple macros names, the names must be separated by semicolons. You must use the short form of the macro name for some macros because Help does not support the long name.
HELP_CONTENTS   Displays the topic specified by the Contents option in the [OPTIONS] section of the .HPJ file. This is for backward compatibility. New applications should provide a .CNT file and use the HELP_FINDER command. Ignored, set to 0.
HELP_FORCEFILE  Ensures that WinHelp is displaying the correct help file. If the incorrect help file is being displayed, WinHelp opens the correct one; otherwise, there is no action.  Ignored, set to 0.
HELP_HELPONHELP Displays help on how to use Windows Help, if the WINHELP.HLP file is available. Ignored, set to 0.
HELP_INDEX  Displays the Index in the Help Topics dialog box. This command is for backward compatibility. New applications should use the HELP_FINDER command.  Ignored, set to 0.
HELP_KEY    Displays the topic in the keyword table that matches the specified keyword, if there is an exact match. If there is more than one match, displays the Index with the topics listed in the Topics Found list box.    Address of a keyword string.
HELP_MULTIKEY   Displays the topic specified by a keyword in an alternative keyword table.  Address of a MULTIKEYHELP structure that specifies a table footnote character and a keyword. 
HELP_PARTIALKEY Displays the topic in the keyword table that matches the specified keyword, if there is an exact match. If there is more than one match, displays the Index tab. To display the Index without passing a keyword (the third result), you should use a pointer to an empty string.    Address of a keyword  string.
HELP_QUIT   Informs the Help application that it is no longer needed. If no other applications have asked for Help, Windows closes the Help application.    Ignored, set to 0.
HELP_SETCONTENTS    Specifies the Contents topic. The Help application displays this topic when the user clicks the Contents button.    Unsigned long integer containing the context identifier for the Contents topic.
HELP_SETINDEX   Specifies a keyword table to be displayed in the Index of the Help Topics dialog box.   Unsigned long integer containing the context identifier for the Index topic.
HELP_SETWINPOS  Displays the Help window, if it is minimized or in memory, and sets its size and position as specified.     Address of a HELPWININFO structure that specifies the size and position of either a primary or secondary Help window.
*/
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::appEventsActivate(TObject *Sender)
{ // When a modal child is active, ensure not hidden by the main form
    if ( ModalChild ) ModalChild->SetFocus();
}

//---------------------------------------------------------------------------
// On deactivate, check if must be always on top
// (FormStyle==fsStayOnTop works only at design time)
void __fastcall TfrmMplotMain::appEventsDeactivate(TObject *Sender)
{
    //if(actStayOnTop->Checked) ::SetWindowPos(Application->MainForm->Handle,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
    //else ::SetWindowPos(Application->MainForm->Handle,HWND_NOTOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::DockOn ( TWinControl* ctrDockHost, TControl* ctrDocked )
{ // Resize docking host and show docked control

    // . See which DockSite is
    if ( ctrDockHost == ctrRightDock )
         {
          splRightDock->Visible = true;
          // TODO: ctrDockHost->Width = ctrDocked->Tag;
          ctrDockHost->Width = ClientWidth/5;
          splRightDock->Left = ctrDockHost->Left - splRightDock->Width;
         }
    else if ( ctrDockHost == ctrLeftDock )
         {
          splLeftDock->Visible = true;
          // ctrDockHost->Width = ctrDocked->Tag;
          ctrDockHost->Width = ClientWidth/5;
          splLeftDock->Left = ctrDockHost->Left + ctrDockHost->Width;
         }
    else { // Dealing with 'pagBottomDock'
          splBottomDock->Visible = true;
          // ctrDockHost->Height = ctrDocked->Tag;
          ctrDockHost->Height = ClientHeight/5;
          // Bottom controls stack
          barStatus->Top = ClientHeight - barStatus->Height;
          barProgress->Top = barStatus->Top - barProgress->Height;
          ctrDockHost->Top = barProgress->Top - ctrDockHost->Height;
          splBottomDock->Top = ctrDockHost->Top - splBottomDock->Height;
         }
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::UnDockFrom ( TWinControl* ctrDockHost )
{ // Hide docking host if necessary

        // . See which DockSite is
    if ( ctrDockHost == ctrRightDock )
         {
          splRightDock->Visible = false;
          ctrDockHost->Width = 0;
         }
    else if ( ctrDockHost == ctrLeftDock )
         {
          splLeftDock->Visible = false;
          ctrDockHost->Width = 0;
         }
    else { // Dealing with 'BottomDock'
          splBottomDock->Visible = false;
          ctrDockHost->Height = 0;
         }
}


//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::WhenDropFile(TWMDropFiles& Message)
{ // Drag&drop file service routine

     // Prepare
     mat::stringlist paths;
     TCHAR strbuf[MAX_PATH];
     HDROP hDrop = HDROP(Message.Drop);

     // How many dropped files?
     UINT FileCount = ::DragQueryFile(hDrop, 0xFFFFFFFF, NULL, MAX_PATH-1);

     // Where they've been dropped? (if outside the client area force a value)
     if ( !::DragQueryPoint(hDrop,&DropPoint) ) DropPoint.x = DropPoint.y = -1;

     // Notify informations
     Log->Entry("Dropped", String(FileCount)+ " files in (" +String(DropPoint.x)+ "," +String(DropPoint.y)+ ")");

     // Get file paths
     for (UINT i=0; i<FileCount; i++)
         {
          //FileName.SetLength(DragQueryFile((HDROP)message.Drop, index,FileName.c_str(), MAX_PATH));
          UINT PathLength = ::DragQueryFile(hDrop, i, strbuf, MAX_PATH-1);
          if ( PathLength >= sizeof(strbuf) ) Log->Warn("Path may be clipped");
          paths.push_back( String(strbuf) );
          Log->SubEntry(paths.back());
         }
     ::DragFinish(hDrop); // tell the OS operation finished

    // . Perform operations on files
     for ( unsigned int i=0; i<paths.size(); ++i )
         {
          CHECKABORT_BREAK
          ProcessFile( paths.at(i) );
         }

    // . Finally
    DropPoint.x = DropPoint.y = -1; // Reset drop point
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::WhenCopyData(TWMCopyData& Message)
{ // Get the command line from another instance

    HWND Sender = Message.From;
    Log->Entry("Wow","0x" + ::IntToHex(int(Sender),8) + " has sent to me a WM_COPYDATA message");

    // . Put in foreground
    ::SetForegroundWindow(Handle);
    ::SetWindowPos(Application->MainForm->Handle,HWND_TOP,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW);

    // . Autenticate operation
    if ( Sender==0 && Message.CopyDataStruct->dwData == 0xFF )
         {
              // . Get data from message
          String strargs = String( static_cast<char*>(Message.CopyDataStruct->lpData) );
          Log->Success("A further application instance was suppressed");
          Log->SubEntry(String("I\'ll handle its args: ") + strargs);

              // . Get command line args from message
          mat::stringlist args;
          mat::DecomposeArgs(args, strargs.c_str());

              // . Process them
          ProcessArguments(args);
         }

    // . Finally, let them know we processed the message...
    Message.Result = 1;
    ::ReplyMessage(Message.Result); // Allow ::SendMessage thread to go on
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::WhenTrainingCard (TMessage& Message)
{ // Messages from HTMLHELP

    //TForm::Dispatch(&Message);
    Log->Entry("HtlHelp",String(Message.WParam) + String("  ") + String(Message.LParam));
}



//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{ // Generic main form key down handler

    //if ( ActiveControl == dynamic_cast<TWinControl*>(grdPane) ) return;

    #ifdef USE_PLOT_GRCTRL
      // Send catched key message to MattPlot if no other control has focus
      if ( Active && Plot->OnKeyDown ) Plot->OnKeyDown(Sender,Key,Shift);
    #else
    #endif

    // IsShortCut
}


//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift)
{ // Generic main form key up handler

    #ifdef USE_PLOT_GRCTRL
      // Send catched key message to MattPlot
      if ( Active ) Plot->OnKeyUp(Sender, Key, Shift);
    #else
    #endif
}


/*---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::barMainDrawPanel(TStatusBar *StatusBar, TStatusPanel *Panel, const TRect &Rect)
{ // Update StatusBar psOwnerDraw panels

        switch ( Panel->Index )
           {
            case 0 : {
                      //StatusBar->Canvas->Draw(Rect.Left,Rect.Top,bmpTrack);
                      //StatusBar->Canvas->TextRect(Rect.Left,Rect.Top,bmpFrame);
                     } break;
           }
}
*/

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::barStatusClick(TObject *Sender)
{ // Open log
    actShowLogExecute(Sender);
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::ctrRightDockDockDrop(TObject *Sender, TDragDockObject *Source, int X, int Y)
{ // On dropping control
    TWinControl* ctrDockHost = dynamic_cast<TWinControl*>(Sender);
    if ( ctrDockHost ) DockOn(ctrDockHost, Source->Control);
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::ctrRightDockUnDock(TObject *Sender, TControl *Client, TWinControl *NewTarget, bool &Allow)
{ // On bringing out control
    TWinControl* ctrDockHost = dynamic_cast<TWinControl*>(Sender);

    Client->Tag = Client->Width<Client->Height ? Client->Width : Client->Height; // Rimember size
    // Hide only if has no more visible docked controls
    if ( ctrDockHost ) if ( ctrDockHost->VisibleDockClientCount <= 1 ) UnDockFrom(ctrDockHost);
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Some custom cell styles
void __fastcall TfrmMplotMain::grdPaneDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State)
{
    // . Settings
    const int sepx = 4;
    const int sepy = 2;
    //grd->Canvas->Font->Assign(grd->Font);
    //TRect InRect = TRect(Rect.Left+sep,Rect.Top+sep,Rect.Right-sep,Rect.Bottom-sep);
    //const UINT DrawTextFlags = DT_SINGLELINE | DT_VCENTER | DT_NOCLIP;

    // . Focus
    //if ( State.Contains(gdFocused) ) grdPane->Canvas->DrawFocusRect(Rect);
    //if ( State.Contains(gdFixed) ) { grd->Canvas->Brush->Color = grd->FixedColor; grd->Canvas->FillRect( Rect ); ::Frame3D( grd->Canvas, Rect, clBtnHighlight, clBtnShadow, 1 ); }
    // Custom selection style (highlighter)
    //if ( State.Contains( gdSelected ) && !State.Contains( gdFocused ) ) { grd->Canvas->Brush->Color = clHighlight; grd->Canvas->Font->Color = clHighlightText; }
    if ( State.Contains(gdSelected) ) {grdPane->Canvas->Brush->Color = TColor(Opts->HighlightColor); grdPane->Canvas->Font->Color = clWindowText;}

    // . Cells
        // . Red background when not recognized
    if ( ACol==0 && grdPane->Objects[1][ARow]==0 ) grdPane->Canvas->Brush->Color = TColor(0x6060F0);

        // . Common
    grdPane->Canvas->FillRect(Rect);
    grdPane->Canvas->TextRect(Rect,Rect.Left+sepx,Rect.Top+sepy,grdPane->Cells[ACol][ARow]);
    //::DrawText(grd->Canvas->Handle,grd->Cells[ACol][ARow].c_str(),grd->Cells[ACol][ARow].Length(),&InRect,DrawTextFlags|DT_CENTER); // Centered
    //::DrawText(grd->Canvas->Handle,grd->Cells[ACol][ARow].c_str(),grd->Cells[ACol][ARow].Length(),&InRect,DrawTextFlags|DT_LEFT);

    // . Focus
    //if (State.Contains(gdFocused)) grd->Canvas->DrawFocusRect(Rect);
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::grdPaneMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
    //TStringGrid* gr = dynamic_cast<TStringGrid*>(Sender);
    int ACol,ARow;
    grdPane->MouseToCell(X, Y, ACol, ARow);

        // . Adjust columns width
    grdPane->ColWidths[0] = 15 + grdPane->Canvas->TextWidth("DYmax");
    grdPane->ColWidths[1] = grdPane->Width - grdPane->ColWidths[0] - 8;

    // . Perform actions according to mouse button
    switch ( Button )
           {
            case mbLeft :
                // Get text to clipboard
                //if ( ACol && Clipboard()->HasFormat(CF_TEXT)) Clipboard()->AsText = grdPane->Cells[ACol][ARow];
                break;

            case mbRight :
                // Custom manage of popup menu
                grdPane->Tag = ARow; // Use tag to put selected row
                TPoint P = grdPane->ClientToScreen(TPoint(X,Y));
                popPaneGrid->Popup(P.x,P.y);
                break;

            //case mbMiddle :
           }
    DefocusControl(grdPane, true);
}

/*   Editable grpPane with combobox

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::grdPaneEnter(TObject *Sender)
{ // Set cell value pointers

    BuildGridPane( grdPane );
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::grdPaneKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{ // When pressed enter read and map cell strings
    if ( Key == VK_RETURN )
       {
        grdPaneEnter(Sender);
        pnlPlotArea->SetFocus();
       }
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::grdPaneMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{ // Add a row if clicked on last

    TStringGrid* gr = dynamic_cast<TStringGrid*>(Sender);

        // . Adjust columns width
    gr->ColWidths[0] = 15 + gr->Canvas->TextWidth("DYmax");
    gr->ColWidths[1] = gr->Width - gr->ColWidths[0] - 8;

    // . Perform actions according to mouse button
    switch ( Button )
           {
            case mbLeft :

                int ACol,ARow;
                gr->MouseToCell(X, Y, ACol, ARow);
                //gr->Cells[ACol][ARow] = "";
                //if ( (ARow+1) == gr->RowCount ) gr->RowCount += 1;  // If clicked on last row, add a new one

                // Get text to clipboard
                if ( ACol && Clipboard()->HasFormat(CF_TEXT)) Clipboard()->AsText = gr->Cells[ACol][ARow];

                    // . Manage custom inplace editor
                if ( ACol == 0 && cbxQuantities->Tag!=gr->Row )
                     {
                      PlaceComboEditor(cbxQuantities, gr, ACol, ARow);
                     }
                else {
                      pnlPlotArea->SetFocus();
                      cbxQuantities->Tag = 0; // Now can be placed
                      cbxQuantities->Visible = false;
                     }
                break;
            case mbRight :
                // Already done with popup menu
                //TPoint P = gr->ClientToScreen(TPoint(X,Y));
                //popGrid->Popup(P.x,P.y);
                break;
            //case mbMiddle :
           }
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::grdPaneTopLeftChanged(TObject *Sender)
{
    //TStringGrid* pGrid = dynamic_cast<TStringGrid*>(Sender);
    if ( cbxQuantities->Visible ) PlaceComboEditor(cbxQuantities, grdPane, 0, cbxQuantities->Tag);
    //grdPane->Invalidate();
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::cbxQuantitiesChange(TObject *Sender)
{ // Quantity change

    if ( cbxQuantities->Tag>0 ) grdPane->Cells[0][cbxQuantities->Tag] = cbxQuantities->Text;
    pnlPlotArea->SetFocus();
}


void __fastcall PlaceComboEditor(TComboBox* cbx, TStringGrid* grd, int ACol, int ARow); // StringGrid In-Place ComboEditor

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::PlaceComboEditor(TComboBox* cbx, TStringGrid* grd, int ACol, int ARow)
{ // StringGrid In-Place ComboEditor

    grd->Col = ACol;
    grd->Row = ARow;
    cbx->Tag = ARow;
    TRect Rect = grd->CellRect(ACol,ARow);
    cbx->Top = grd->Top;
    cbx->Left = grd->Left;
    if (cbx->Items->IndexOf(grd->Cells[ACol][ARow]) >= 0) cbx->ItemIndex = cbx->Items->IndexOf(grd->Cells[ACol][ARow]);
    else cbx->Text = grd->Cells[ACol][ARow];
    cbx->Top = cbx->Top + Rect.Top + grd->GridLineWidth;
    cbx->Left = cbx->Left + Rect.Left + grd->GridLineWidth+1;
    cbx->Height = (Rect.Bottom - Rect.Top) + 1;
    cbx->Width = (Rect.Right - Rect.Left) + 0;
    cbx->Visible = true;
    cbx->SetFocus();
}

*/


//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::treePlotDragOver(TObject *Sender, TObject *Source, int X, int Y, TDragState State, bool &Accept)
{
    Accept = false; // Default
    //if ( Sender->ClassType() == __classid(TTreeView) ) ShowMessage("drag over sender is a TTreeView");
    //TTreeView* tree = dynamic_cast<TTreeView*>(Sender);

    // Must be internal drag and drop of 'treePlot'
    if ( treePlot == Sender && Sender == Source )
       { // Find involved nodes
        TTreeNode* DstNode = treePlot->GetNodeAt(X,Y);
        TTreeNode* SrcNode = treePlot->Selected;

        if ( DstNode && SrcNode )
           {
            // different parent (DstNode->Parent == SrcNode->Parent)
            Accept = ( (SrcNode->Level==1) && (DstNode->Level==1) ) || // Frame to frame
                     ( (SrcNode->Level==2) && (DstNode->Level==1) && (SrcNode->Parent!=DstNode) ) || // Track to frame
                     ( (SrcNode->Level==2) && (DstNode->Level==2) && (SrcNode->Parent==DstNode->Parent) );   // Track to track (same frame)
           }
       }
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::treePlotDragDrop(TObject *Sender, TObject *Source, int X, int Y)
{ // Drop operation: if here, conditions in 'DragOver' are met

    // . Get node datas
    TTreeView* tree = dynamic_cast<TTreeView*>(Sender);
    TTreeNode* DstNode = tree->GetNodeAt(X,Y);
    TTreeNode* SrcNode = tree->Selected;
    cls_FoilObject* o1 = reinterpret_cast<cls_FoilObject*>(SrcNode->Data);
    cls_FoilObject* o2 = reinterpret_cast<cls_FoilObject*>(DstNode->Data);
    tree->EndDrag(false);

    // . Perform plot operation depending on objects
    cls_Frame* fr1 = dynamic_cast<cls_Frame*>(o1);
    cls_Frame* fr2 = dynamic_cast<cls_Frame*>(o2);
    cls_Track* tr1 = dynamic_cast<cls_Track*>(o1);
    cls_Track* tr2 = dynamic_cast<cls_Track*>(o2);

    if ( fr1 && fr2 ) Plot->MoveFrame(fr1,fr2);
    else if ( tr1 && tr2 ) tr1->Owner->MoveTrack(tr1,tr2);
    else if ( tr1 && fr2 ) tr1->Owner->MoveTrack(tr1,fr2);

    // . Finally
    Plot->Invalidate();
    Plot->FillTreeView(treePlot,false);
}

//---------------------------------------------------------------------------
void __fastcall TfrmMplotMain::treePlotMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{ // Select/Open object

    // . Get involved objects
    TTreeView* tree = dynamic_cast<TTreeView*>(Sender);
    if (!tree) return; // TreeView only
    // Ensure clicked node
    tree->Selected = tree->GetNodeAt(X,Y); // Force selection with right click also
    if (!tree->Selected) return;
    cls_FoilObject* fo = reinterpret_cast<cls_FoilObject*>(tree->Selected->Data);
    if ( !fo || tree->Selected->Level<1 ) return;

    // . Change clicked object
    Plot->Selected.FoilObject = fo;
    cls_Frame* f = dynamic_cast<cls_Frame*>(Plot->Selected.FoilObject);
    if (f) Plot->CurrentFrame = f;
    else {
          cls_Track* t = dynamic_cast<cls_Track*>(Plot->Selected.FoilObject);
          if (t) Plot->CurrentTrack = t;
         }

    // . Perform actions according to mouse button
    switch ( Button )
           {
            case mbLeft :

                if ( Shift.Contains(ssDouble) ) actPropertiesExecute(treePlot);
                break;

            case mbRight :

                CreatePlotPopup( tree->ClientToScreen(TPoint(X,Y)) );
                break;
           }
}

/*
//---------------------------------------------------------------------------
void __fastcall Tfrm_GateConfMain::treeIncomingMouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
{ // Manage TTreeNodes hints

    // . Preliminar things
    TTreeView* tree = dynamic_cast<TTreeView*>(Sender);
    if (!tree) return; // On a TreeView only

    // . Manage TTreeNodes hints
    //Log->SubEntry(Sender->ClassName());
    TTreeNode *HintNode = tree->GetNodeAt(X, Y);
    if ( HintNode )
       {
        // . See node type
        switch ( HintNode->ImageIndex )
               {
                case IM_BLOCK   : tree->Hint = String("Block mapped in p") + String(NODE_ADDR(HintNode)); break;
                default         : tree->Hint = HintNode->Text;
               }
       }
}

//---------------------------------------------------------------------------
void __fastcall Tfrm_GateConfMain::treeIncomingExpanding(TObject *Sender, TTreeNode *Node, bool &AllowExpansion)
{ // Avoid double-click collapsing/expanding

    TTreeView* tree = dynamic_cast<TTreeView*>(Sender); if (!tree) return; // TreeView only
    POINT p; ::GetCursorPos(&p); p = tree->ScreenToClient(p);
    THitTests ht = tree->GetHitTestInfoAt(p.x, p.y);
    AllowExpansion = ht.Contains(htOnButton) || AllowExpand;
}
*/


//---------------------------------------------------------------------------



//=============================== End of File ===============================



