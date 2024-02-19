//---------------------------------------------------------------------------
#include <vcl.h>
#include "unt_MatCommon.h"
#pragma hdrstop
#include "unt_SynEditor.h"
#include "unt_FileDialog.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SynEditMiscClasses"
#pragma link "SynEditSearch"
#pragma resource "*.dfm"

// TODO 3: keys are captured by main form!!
// TODO 5: bookmarks

/*
  SynEdit1->SetBookMark(int BookMark, int X, int Y);
  SynEdit1->GotoBookMark(int BookMark);  SynEdit1->ClearBookMark(int BookMark);
  if ( mat::MsgAsk("cls_MPlot","Do you want to...?") )
  if (SynEditor->SelAvail && (SynEditor->BlockBegin.y == SynEditor->BlockEnd.y)) SynEditor->SelText;
  SynEditor->GetWordAtRowCol(SynEditor->CaretXY);

  if (Key==VK_RETURN)
     {
      synEditor->Font->Height = edtFontSize->Text.ToIntDef(0);
      synEditor->Font->Name = edtFont->Text;
     }
*/


//-------------------------------- File Scope -------------------------------
    const String Version         = "SynEditor 0.3";
    // TODO: common files definitions
    const String All_Files       = "All files|*";
    const String Ini_Files       = "Ini files (.ini)|*.ini";
    const String Data_Files      = "Data files (.txt,.out,.dat)|*.txt;*.out;*.dat";
    const String Xml_Files       = "Xml files (.xml)|*.xml";
    const String MattPlot_Files  = "MattPlot files (.mpl,.mplz)|*.mpl;*.mplz";

//----------------------------- Static members ------------------------------
    int TfrmSynEditor::InstancesCount = 0;

//------------------------------ Main methods -------------------------------

//---------------------------------------------------------------------------
void __fastcall TfrmSynEditor::Init()
{ // Base constructor

    ++InstancesCount;

// . . . Initializations
    OpenedPath = "";
    OpenedStream = 0;

// . . . SynEdit objects
    synEditor = new TSynEdit(this);
    hghDat = new TSynGeneralSyn(synEditor);
    hghIni = new TSynIniSyn(synEditor);
    hghXml = new TSynXMLSyn(synEditor);
    SynSearch = new TSynEditSearch(synEditor);
    SearchOptions = TSynSearchOptions() << ssoMatchCase
                                        >> ssoEntireScope
                                        >> ssoBackwards
                                        >> ssoSelectedOnly
                                        >> ssoWholeWord;
    InitSynComponents();

// . . . Other initializations
    Positions.clear();
    ctrMemo->Lines->Clear();
    // Actions
    EnableFileActions(false);
    // Caption
    Caption = Version;

// . . . Defaults
    ChooseStyle(); // A default style
}

//---------------------------------------------------------------------------
__fastcall TfrmSynEditor::~TfrmSynEditor()
{ // Destructor

    //delete SynSearch;
    //delete hghXml;
    //delete hghIni;
    //delete hghDat;
    delete synEditor;
    --InstancesCount;
}


//============================== Main services ==============================

//---------------------------------------------------------------------------
void TfrmSynEditor::Open( TStream* s )
{ // Open a stream

    s->Position = 0; // Go to start
    OpenedStream = s;

    BeginLongTask();
    try {
         synEditor->Lines->Clear();
         synEditor->Lines->LoadFromStream(s);
        }
    __finally { EndLongTask(); } // ° ° ° ° ° ° ° ° ° ° ° ° ° ° ° ° ° ° °

// . . . Other things
    // Actions
    EnableFileActions(true);
    // Set a caption
    Caption = Version + String(" - ") + synEditor->Highlighter->GetLanguageName();
    // Other things depending on opened stuff
    SpecificActions();
}

//---------------------------------------------------------------------------
void TfrmSynEditor::Open( const String& p )
{ // Open a path

// . . . Preliminary operations
    // Can choose style from extension
    String ext = ::ExtractFileExt(p);
    ChooseStyle( ext );

// . . . Fill lines
    BeginLongTask();
        //synEditor->Lines->Clear();
        //synEditor->Lines->LoadFromFile(p);
        try {
             OpenedStream = new TFileStream(p, fmOpenRead|fmShareDenyWrite);
             Open (OpenedStream);
             OpenedPath = p;
             actSave->Enabled = true;
            }
        __finally {
                   delete OpenedStream;
                   OpenedStream = 0;
                   EndLongTask();
                  }

// . . . Other things
    // Set a caption
    Caption +=  String(" - ") + OpenedPath;
}

//---------------------------------------------------------------------------
void TfrmSynEditor::Save( TStream* s )
{ // Save a stream

    s->Position = 0; // Go to start
    OpenedStream = s;

    BeginLongTask();
    try { synEditor->Lines->SaveToStream(s); }
    __finally { EndLongTask(); }

// . . . Other things
    // Actions
    EnableFileActions(true);
    // Set a caption
    Caption = Version + String(" - ") + synEditor->Highlighter->GetLanguageName();
}

//---------------------------------------------------------------------------
void TfrmSynEditor::Save( const String& p )
{ // Save to path

    // if ( ::FileExists(p) )
    BeginLongTask();
        //synEditor->Lines->SaveToFile(p);
        try {
             OpenedStream = new TFileStream(p, fmCreate | fmShareExclusive);
             Save (OpenedStream);
             OpenedPath = p;
             actSave->Enabled = true;
            }
        __finally {
                   delete OpenedStream;
                   OpenedStream = 0;
                   EndLongTask();
                  }

// . . . Other things
    // Set a caption
    Caption +=  String(" - ") + OpenedPath;
}

//---------------------------------------------------------------------------
void TfrmSynEditor::CloseFile()
{ // Close current file

// . . . Clear lines
    Positions.clear();
    ctrMemo->Lines->Clear();
    synEditor->Lines->Clear();

// . . . Other things
    // Opened
    OpenedPath = "";
    OpenedStream = 0;
    // Reset caption
    Caption = Version;
    // Actions
    EnableFileActions(false);
}

//---------------------------------------------------------------------------
void TfrmSynEditor::ListPosition (const String& Message, int Row, int Col)
{ // Log an error

    TMemoPosition p;
    p.Char = Col;
    p.Line = Row;
    Positions.push_back( TMemoPosition(p) );
    ctrMemo->Lines->Add( Message );
}

//---------------------------------------------------------------------------
void TfrmSynEditor::GoTo (const TMemoPosition& CharLine) const
{ // Go to position
    //synEditor->GotoLineAndCenter(Row);
    //synEditor->EnsureCursorPosVisible();
    //synEditor->EnsureCursorPosVisibleEx(true);
    synEditor->TopLine = CharLine.Line;
    synEditor->CaretXY = CharLine;
}

//---------------------------------------------------------------------------
void TfrmSynEditor::HighlightLine (int Line)
{ // Highlight a line
    //synEditor->SelectedColor = clRed;
    TMemoPosition p;
    p.Char = 0;
    p.Line = Line;
    synEditor->SelStart = synEditor->RowColToCharIndex(p);
    synEditor->SelEnd = 10;
}


//------------------------------- Utilities ---------------------------------

//---------------------------------------------------------------------------
void __fastcall TfrmSynEditor::ToStatusBar(const String& msg, int pnl)
{ // Write in statusbar
        if ( pnl<0 || pnl>=barStatus->Panels->Count ) pnl = barStatus->Panels->Count - 1;
        barStatus->Panels->Items[pnl]->Text = msg;
}
//---------------------------------------------------------------------------
String __fastcall TfrmSynEditor::FromStatusBar(int pnl)
{ // Get text in statusbar
        if ( pnl<0 || pnl>=barStatus->Panels->Count ) pnl = barStatus->Panels->Count - 1;
        return barStatus->Panels->Items[pnl]->Text;
}

//---------------------------------------------------------------------------
void TfrmSynEditor::ChooseStyle( String Ext )
{ // Assign Highlighter and other Editor properties

    Ext = Ext.UpperCase();
    if ( Ext == ".INI" ) // Ini syntax highlighter DefaultFilter = "ini files|*.ini";
         {
          //synEditor->Color = clBackground1;
          synEditor->Font->Size = 12;
          synEditor->Highlighter = hghIni;
         }
    else if ( Ext == ".XML" ) // XML syntax highlighter DefaultFilter = "xml files|*.xml";
         {
          //synEditor->Color = clBackground2;
          synEditor->Font->Size = 12;
          synEditor->Highlighter = hghXml;
         }
    else // Dat syntax highlighter  DefaultFilter = "data files|*.txt,*.out,*.dat";
         {
          //synEditor->Color = clBackground2;
          synEditor->Font->Size = 12;
          synEditor->Highlighter = hghDat;
         }
}

//---------------------------------------------------------------------------
void TfrmSynEditor::SpecificActions()
{ // Other specific actions depending on extension

    if ( synEditor->Highlighter == hghIni ) // Ini files
         {
          // . . . Show sections positions
          const char sect_char = '[';
          String l;
          for (int i=0; i<synEditor->Lines->Count; ++i)
              { // Searching section lines
               l = synEditor->Lines->Strings[i].Trim();
               if ( l.Length() >= 1 ) if ( l[1] == sect_char ) ListPosition(l, i+1, 1);
              }
          // Dock pane to right
          ctrPane->Width = Width/4;
          ctrPane->ManualDock(ctrRightDock,NULL,alClient);
         }
    else {
          // Dock pane bottom
          ctrPane->Height = Height/5;
          ctrPane->ManualDock(ctrBottomDock,NULL,alClient);
          //ctrPane->ManualFloat( TRect(Left+Width/2,Top+Height/2,Left+6*Width/5,Top+6*Height/5) );
         }
}

//---------------------------------------------------------------------------
inline void TfrmSynEditor::EnableFileActions ( const bool b )
{ // Enable/disable save action
    //actApply->Enabled = b;
    //actDiscard->Enabled = b;
    //actSaveAs->Enabled = b;
    //actCloseFile->Enabled = b;
    actSave->Enabled = b;
}




//------------------------------ Maintenance --------------------------------


//---------------------------------------------------------------------------
void TfrmSynEditor::LongTask ( bool begin )
{ // Long task feedback

    barProgress->Position = 0;
    barProgress->Visible = begin;
}

//---------------------------------------------------------------------------
void __fastcall TfrmSynEditor::Progress(System::TObject* Sender, int Size)
{ // A progress function

    barProgress->Position = Size;
}

//---------------------------------------------------------------------------
void __fastcall TfrmSynEditor::DockOn ( TWinControl* ctrDockHost )
{ // Resize docking host and show docked control

// . . . Settings
    const int DockedSizeFraction = 5;

// . . . See which DockSite is
    if ( ctrDockHost == ctrRightDock )
         {
          splVerticalDock->Visible = true;
          ctrDockHost->Width = ClientWidth / DockedSizeFraction;
          splVerticalDock->Left = ctrDockHost->Left - splVerticalDock->Width;
         }
    else { // Dealing with 'pagBottomDock'
          splHorizontalDock->Visible = true;
          ctrDockHost->Height = ClientHeight / DockedSizeFraction;
          barStatus->Top = ClientHeight - barStatus->Height;
          barProgress->Top = barStatus->Top - barProgress->Height;
          ctrDockHost->Top = barProgress->Top - ctrDockHost->Height;
          splHorizontalDock->Top = ctrDockHost->Top - splHorizontalDock->Height;
         }
}

//---------------------------------------------------------------------------
void __fastcall TfrmSynEditor::UnDockFrom ( TWinControl* ctrDockHost )
{ // Hide docking host if necessary

    // . . . See which DockSite is
    if ( ctrDockHost == ctrRightDock )
         {
          splVerticalDock->Visible = false;
          ctrDockHost->Width = 0;
         }
    else { // Dealing with 'BottomDock'
          splHorizontalDock->Visible = false;
          ctrDockHost->Height = 0;
         }
}


//------------------------------- Actions -----------------------------------

//---------------------------------------------------------------------------
void __fastcall TfrmSynEditor::actOpenExecute(TObject *Sender)
{ // Open action

// . . . Prepare
    cls_FileDialog FileDialog(this);
        FileDialog.Filter = All_Files+"|"+Ini_Files+"|"+Xml_Files+"|"+Data_Files+"|"+MattPlot_Files;
        FileDialog.FilterIndex = 1;
        FileDialog.DefaultExt = ""; // appended extension when missing
        //FileDialog.InitialDir = Opts->DefaultDir;
        FileDialog.Options >> cls_FileDialog::ofOverwritePrompt
                           >> cls_FileDialog::ofAllowMultiSelect
                           << cls_FileDialog::ofFileMustExist;

    if ( FileDialog.Open() ) Open( FileDialog.FileName );
    //else mat::MsgInfo("Open file","Aborted");
}

//---------------------------------------------------------------------------
void __fastcall TfrmSynEditor::actSaveExecute(TObject *Sender)
{ // Save
    if ( !OpenedPath.IsEmpty() && ::FileExists(OpenedPath) ) Save(OpenedPath);
    else if ( OpenedStream ) Save(OpenedStream);
    else actSaveAsExecute(Sender);
}


//---------------------------------------------------------------------------
void __fastcall TfrmSynEditor::actSaveAsExecute(TObject *Sender)
{ // Save as...

    cls_FileDialog FileDialog(this);
    FileDialog.Filter = All_Files+"|"+Ini_Files+"|"+Xml_Files+"|"+Data_Files+"|"+MattPlot_Files;
    FileDialog.FilterIndex = 1; // Default save with compression
    FileDialog.DefaultExt = ""; // appended extension when missing
    //FileDialog.InitialDir = Opts->DefaultDir;
    FileDialog.Options >> cls_FileDialog::ofCreatePrompt
                       << cls_FileDialog::ofOverwritePrompt;

    if ( FileDialog.Save() )
         {
          OpenedPath = FileDialog.FileName;
          OpenedStream = 0;
          Save(OpenedPath);
         }
}

//---------------------------------------------------------------------------
void __fastcall TfrmSynEditor::actCloseFileExecute(TObject *Sender)
{ // Close file

    if ( synEditor->Modified )
       {
        switch ( mat::MsgAskCancel("Closing file","Do you want to save your changes?") )
               {
                case IDYES :
                    actSaveExecute(Sender);
                    break;
                //case IDNO  :
               }
       }
    CloseFile();
}


//---------------------------------------------------------------------------
void __fastcall TfrmSynEditor::actApplyExecute(TObject *Sender)
{ // Apply changes

// . . . Update stream
    if ( synEditor->Modified && CanSave() ) actSaveExecute(Sender);
    ModalResult = mrYes; // file submitted
}

//---------------------------------------------------------------------------
void __fastcall TfrmSynEditor::actDiscardExecute(TObject *Sender)
{ // Discard changes

    ModalResult = mrNo; // file rejected
}

//---------------------------------------------------------------------------
void __fastcall TfrmSynEditor::actUndoExecute(TObject *Sender)
{ // Undo
    synEditor->Undo();
}

//---------------------------------------------------------------------------
void __fastcall TfrmSynEditor::actRedoExecute(TObject *Sender)
{ // Redo
    synEditor->Redo();
}

//---------------------------------------------------------------------------
void __fastcall TfrmSynEditor::actCopyExecute(TObject *Sender)
{ // Copy
    synEditor->CopyToClipboard();
}

//---------------------------------------------------------------------------
void __fastcall TfrmSynEditor::actCutExecute(TObject *Sender)
{ // Cut
    synEditor->CutToClipboard();
}

//---------------------------------------------------------------------------
void __fastcall TfrmSynEditor::actPasteExecute(TObject *Sender)
{ // Paste
    synEditor->PasteFromClipboard();
}

//---------------------------------------------------------------------------
void __fastcall TfrmSynEditor::actSelectAllExecute(TObject *Sender)
{ // Select all
    synEditor->SelectAll();
}

//---------------------------------------------------------------------------
void __fastcall TfrmSynEditor::actChangeFontExecute(TObject *Sender)
{ // Change font

    static const int totsizes = 4;
    static const int fsizes[totsizes] = { synEditor->Font->Size,
                                          1.3*synEditor->Font->Size,
                                          1.8*synEditor->Font->Size,
                                          0.8*synEditor->Font->Size };
    static int curridx = 0;
    synEditor->Font->Size = fsizes[ curridx=(curridx+1)%totsizes ];
}

//---------------------------------------------------------------------------
void __fastcall TfrmSynEditor::actShowPaneExecute(TObject *Sender)
{ // View Pane
    ctrPane->Visible = !ctrPane->Visible;
    // Manage dockhost size
    if ( ctrPane->Visible && (ctrPane->Parent->Parent==this) ) DockOn(ctrPane->Parent);
}

//---------------------------------------------------------------------------
void __fastcall TfrmSynEditor::actDoSearchExecute(TObject *Sender)
{ // Search for string

    if ( synEditor->SearchReplace(edtSearchText->Text,edtSearchText->Text, SearchOptions) )
         {
          edtSearchText->Color = TColor(0xD9EDD9);
          synEditor->SetFocus();
         }
    else {
          ::MessageBeep(MB_ICONASTERISK);
          edtSearchText->Color = TColor(0xD9D9ED);
          synEditor->CaretX = 0;
          synEditor->CaretY = 0;
         }
}

//---------------------------------------------------------------------------





//-------------------------------- Events -----------------------------------

//---------------------------------------------------------------------------
// On show
void __fastcall TfrmSynEditor::FormShow(TObject *Sender)
{
// . . . Position to first error position if existing
    if ( Positions.size() > 0 ) GoTo( Positions[0] );
    FocusControl(synEditor);
}

//---------------------------------------------------------------------------
// Display hints in statusbar. Sender is always 'Application'
void __fastcall TfrmSynEditor::appEventsHint(TObject *Sender)
{
// . . . Display Hint in Statusbar
    //PrevMessage = FromStatusBar(); // Keep the displayed message
    TApplication* a = dynamic_cast<TApplication*>(Sender);
    if (a) ToStatusBar( ::GetLongHint(a->Hint) );
}

//---------------------------------------------------------------------------
void __fastcall TfrmSynEditor::appEventsIdle(TObject *Sender, bool &Done)
{ // Idle tasks

// . . . Maintain docking site sizes: hide if has no more visible docked controls
    if ( ctrRightDock->VisibleDockClientCount < 1 ) UnDockFrom (ctrRightDock);
    if ( ctrBottomDock->VisibleDockClientCount < 1 ) UnDockFrom (ctrBottomDock);
}

//---------------------------------------------------------------------------
void __fastcall TfrmSynEditor::aclActionsUpdate(TBasicAction *Action, bool &Handled)
{ // Update task

        //actApply->Enabled = synEditor->Modified;
        //actDiscard->Enabled = synEditor->Modified;

        actPaste->Enabled = synEditor->CanPaste;
        actUndo->Enabled = synEditor->CanUndo;
        actRedo->Enabled = synEditor->CanRedo;

        actShowPane->Checked = ctrPane->Visible;
}

//---------------------------------------------------------------------------
void __fastcall TfrmSynEditor::synEditorDropFiles(TObject *Sender, int X, int Y, TStrings *AFiles)
{ // On Drop files

    if ( AFiles->Count > 1 ) ToStatusBar("Too many dropped files");
    else {
          CloseFile();
          Open( AFiles->Strings[0] );
         }
// . . . Perform operations on files
    //for ( int i=0; i<AFiles->Count; ++i ) Open( AFiles->Strings[i] );
}
//---------------------------------------------------------------------------
void __fastcall TfrmSynEditor::synEditorMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{ // On Mouse Down
    switch (Button)
        {
          case mbLeft :
              if ( Shift.Contains(ssAlt) )  synEditor->SelectionMode = smColumn;
              else synEditor->SelectionMode = smNormal;
              break;
          case mbRight:
              TPoint P = synEditor->ClientToScreen(TPoint(X,Y));
              popMenu->Popup(P.x,P.y);
              break;
          //case mbMiddle: ; break;
        }
}

//---------------------------------------------------------------------------
void __fastcall TfrmSynEditor::synEditorReplaceText(TObject *Sender,
      const AnsiString ASearch, const AnsiString AReplace, int Line,
      int Column, TSynReplaceAction &Action)
{ // On search string
    if (ASearch == AReplace) Action = Synedit::raSkip;
    else ToStatusBar("Replace not allowed");
}

//---------------------------------------------------------------------------
void __fastcall TfrmSynEditor::synEditorStatusChange(TObject *Sender, TSynStatusChanges Changes)
{ // Display caret position

    ToStatusBar(String(synEditor->CaretY) + " ; " + String(synEditor->CaretX),SP_POS);
}

//---------------------------------------------------------------------------
void __fastcall TfrmSynEditor::ctrMemoClick(TObject *Sender)
{
// . . . Get clicked line
    int l = ctrMemo->Perform(EM_LINEFROMCHAR,ctrMemo->SelStart,0);
    //barStatus->Panels->Items[SP_MEX]->Text = String(l);

// . . . Retrieved associated object
    if ( l >= 0 && l < ctrMemo->Lines->Count )
       {
        GoTo(Positions[l]);
        //HighlightLine (Positions[l].Line);
        FocusControl(synEditor);
       }
}

//---------------------------------------------------------------------------
void __fastcall TfrmSynEditor::edtSearchTextKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{ // Do search at enter
    if (Key == VK_RETURN) actDoSearchExecute(Sender);
}



//---------------------------------------------------------------------------
void __fastcall TfrmSynEditor::ctrRightDockDockDrop(TObject *Sender, TDragDockObject *Source, int X, int Y)
{ // On dropping control
    TWinControl* ctrDockHost = dynamic_cast<TWinControl*>(Sender);
    if ( ctrDockHost ) DockOn (ctrDockHost);
}

//---------------------------------------------------------------------------
void __fastcall TfrmSynEditor::ctrRightDockUnDock(TObject *Sender,
      TControl *Client, TWinControl *NewTarget, bool &Allow)
{  // On bringing out control
    TWinControl* ctrDockHost = dynamic_cast<TWinControl*>(Sender);
    // Hide only if has no more visible docked controls
    if ( ctrDockHost ) if ( ctrDockHost->VisibleDockClientCount <= 1 ) UnDockFrom (ctrDockHost);
}


//---------------------------------------------------------------------------
void __fastcall TfrmSynEditor::InitSynComponents()
{
// . . . Fill synEditor attributes
    synEditor->Parent = this;
    synEditor->Align = alClient;
    synEditor->OnMouseDown = synEditorMouseDown;
    synEditor->OnDropFiles = synEditorDropFiles;
    synEditor->OnReplaceText = synEditorReplaceText;
    synEditor->OnStatusChange = synEditorStatusChange;
    synEditor->SearchEngine = SynSearch;
    synEditor->Color = TColor(0xF1F9F9);
    synEditor->Font->Charset = DEFAULT_CHARSET;
    synEditor->Font->Color = clWindowText;
    synEditor->Font->Height = -13;
    synEditor->Font->Name = "Courier New";
    synEditor->Gutter->AutoSize = True;
    synEditor->Gutter->Cursor = crHandPoint;
    synEditor->Gutter->Font->Charset = DEFAULT_CHARSET;
    synEditor->Gutter->Font->Color = synEditor->Color;
    synEditor->Gutter->Font->Height = -12;
    synEditor->Gutter->Font->Name = "Courier New";
    synEditor->Gutter->Font->Style = TFontStyles() << fsBold;
    synEditor->Gutter->LeftOffset = 20;
    synEditor->Gutter->RightOffset = 5;
    synEditor->Gutter->ShowLineNumbers = true;
    synEditor->Gutter->Width = 50;
    synEditor->Gutter->Gradient = true;
    synEditor->Gutter->GradientStartColor = TColor(0xE7E5F1);
    synEditor->Gutter->GradientEndColor = TColor(0xD2AAB0);
    synEditor->Options = TSynEditorOptions() << eoAltSetsColumnMode << eoAutoIndent << eoDragDropEditing << eoDropFiles << eoEnhanceEndKey << eoGroupUndo << eoShowScrollHint << eoSmartTabDelete << eoTabIndent << eoTrimTrailingSpaces;
    synEditor->WantTabs = true;
    //synEditor->WordWrapGlyph->Glyph
    synEditor->WordWrapGlyph->MaskColor = clWhite;

// hghDat
// . . . Fill synEditor attributes
    hghDat->CommentAttri->Foreground = clGreen;
    hghDat->Comments = TCommentStyles() << csCStyle << csCPPStyle;
    hghDat->DetectPreprocessor = true;
    hghDat->IdentifierAttri->Foreground = clRed;
    hghDat->IdentifierAttri->Style = TFontStyles() << fsBold << fsStrikeOut;
    /*
    hghDat->IdentifierChars = "!\"#$%&\'#39\'()*+ <<-->/0123456789:;<=>?@"
                              "ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\;^_`"
                              "abcdefghijklmnopqrstuvwxyz{|}~€‚ƒ„…†‡ˆ‰Š‹ŒŽ‘’“”•–—˜™š›œžŸ"
                              "¡¢£¤¥¦§¨©ª«¬­®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞßà"
                              "áâãäåæçèéêëìíîïðñòóôõö÷øùúûüýþÿ";
    */
    hghDat->KeyAttri->Background = TColor(0xB3FFF0);
    hghDat->KeyAttri->Foreground = clBlack;
    hghDat->KeyAttri->Style = TFontStyles() << fsBold << fsUnderline;
    hghDat->NumberAttri->Foreground = clNavy;
    hghDat->PreprocessorAttri->Background = TColor(0xFFFEDF);
    hghDat->PreprocessorAttri->Foreground = TColor(0x8A2226);
    hghDat->PreprocessorAttri->Style = TFontStyles() << fsBold;
    hghDat->SpaceAttri->Foreground = TColor(0xE5E3D9);
    hghDat->SpaceAttri->Style = TFontStyles() << fsUnderline;
    hghDat->StringAttri->Foreground = clOlive;
    hghDat->StringAttri->Style = TFontStyles() << fsBold;
    hghDat->SymbolAttri->Foreground = TColor(0xCE00C7);
    hghDat->StringDelim = sdDoubleQuote;

// . . . Fill hghIni attributes
    hghIni->TextAttri->Background = TColor(0xCBFBF7);
    hghIni->TextAttri->Foreground = TColor(0xD00058);
    hghIni->TextAttri->Style = TFontStyles() << fsBold;
    hghIni->SectionAttri->Background = TColor(0xA8FFA3);
    hghIni->SectionAttri->Foreground = TColor(0x7B3B1D);
    hghIni->KeyAttri->Foreground = TColor(0x6AD7);
    hghIni->NumberAttri->Background = TColor(0xCBFBF7);
    hghIni->NumberAttri->Foreground = TColor(0xD20015);
    hghIni->NumberAttri->Style = TFontStyles() << fsBold;
    hghIni->SpaceAttri->Foreground = TColor(0xE5E3D9);
    hghIni->SpaceAttri->Style = TFontStyles() << fsBold << fsUnderline;
    hghIni->StringAttri->Background = TColor(0xCBFBF7);
    hghIni->StringAttri->Foreground = TColor(0xD20097);
    hghIni->StringAttri->Style = TFontStyles() << fsBold;
    hghIni->SymbolAttri->Foreground = TColor(0x404040);
    hghIni->SymbolAttri->Style = TFontStyles() << fsBold;

// . . . Fill hghXml attributes
    hghXml->ElementAttri->Foreground = TColor(0xF68B32);
    hghXml->AttributeAttri->Foreground = TColor(0x9CBF18);
    hghXml->AttributeValueAttri->Foreground = TColor(0xB93BB9);
    //hghXml->AttributeValueAttri->Style = TFontStyles() <<;
    hghXml->CDATAAttri->Foreground = clTeal;
    hghXml->EntityRefAttri->Foreground = TColor(0xE5338F);
    hghXml->EntityRefAttri->Style = TFontStyles() << fsBold << fsUnderline;
    hghXml->ProcessingInstructionAttri->Background = TColor(0xFFFADB);
    hghXml->ProcessingInstructionAttri->Foreground = TColor(0xFF0571);
    hghXml->ProcessingInstructionAttri->Style = TFontStyles() << fsBold;
    hghXml->CommentAttri->Foreground = TColor(0x8284);
    hghXml->CommentAttri->Style = TFontStyles() << fsItalic;
    hghXml->DocTypeAttri->Foreground = TColor(0xAA1053);
    hghXml->DocTypeAttri->Style = TFontStyles() << fsBold << fsItalic;
    hghXml->SymbolAttri->Foreground = clGreen;
    hghXml->WantBracesParsed = false;
}
//=============================== End of File ===============================
