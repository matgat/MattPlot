//---------------------------------------------------------------------------
#ifndef unt_SynEditorH
#define unt_SynEditorH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ImgList.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <ExtCtrls.hpp>
#include <AppEvnts.hpp>
#include <Menus.hpp>
// TODO 5: move in cpp? SynEdit
#include "SynEdit.hpp"
//#include "SynMemo.hpp"
#include "SynEditMiscClasses.hpp"
#include "SynEditSearch.hpp"
#include "SynEditHighlighter.hpp"
#include "SynHighlighterGeneral.hpp"
#include "SynHighlighterIni.hpp"
#include "SynHighlighterXML.hpp"
// standard lib types
#include <vector>

//---------------------------------------------------------------------------

typedef Synedittypes::TBufferCoord TMemoPosition;

class TfrmSynEditor : public TForm
{
__published:	// IDE-managed Components
    TApplicationEvents *appEvents;
    TPanel *ctrPane;
    TPanel *ctrRightDock;
    TPanel *ctrBottomDock;
    TSplitter *splHorizontalDock;
    TSplitter *splVerticalDock;
    TMemo *ctrMemo;
    TEdit *edtSearchText;
    TToolBar *tlbMain;
    TToolButton *ToolButton1;
    TToolButton *ToolButton2;
    TToolButton *ToolSeparator0;
    TToolButton *ToolButton3;
    TToolButton *ToolButton4;
    TToolButton *ToolSeparator1;
    TToolButton *ToolButton5;
    TToolButton *ToolButton6;
    TToolButton *ToolSeparator3;
    TToolButton *ToolButton7;
    TToolButton *ToolButton8;
    TToolButton *ToolButton9;
    TToolButton *ToolSeparator4;
    TToolButton *ToolButton10;
    TToolButton *ToolButton11;
    TToolButton *ToolSeparator5;
    TToolButton *ToolButton12;
    TToolButton *ToolButton13;
    TToolButton *ToolButton14;
    TProgressBar *barProgress;
    TStatusBar *barStatus;
    TImageList *imlActions;
    TActionList *aclActions;
    TAction *actSave;
    TAction *actOpen;
    TAction *actUndo;
    TAction *actRedo;
    TAction *actApply;
    TAction *actDiscard;
    TAction *actShowPane;
    TAction *actSaveAs;
    TAction *actChangeFont;
    TAction *actDoSearch;
    TAction *actCopy;
    TAction *actCut;
    TAction *actPaste;
    TAction *actSelectAll;
    TAction *actCloseFile;
    TPopupMenu *popMenu;
    TMenuItem *Copy1;
    TMenuItem *Cut1;
    TMenuItem *Paste1;
    TMenuItem *Selectall1;
    TMenuItem *N1;
    void __fastcall actSaveExecute(TObject *Sender);
    void __fastcall actOpenExecute(TObject *Sender);
    void __fastcall actUndoExecute(TObject *Sender);
    void __fastcall actRedoExecute(TObject *Sender);
    void __fastcall actApplyExecute(TObject *Sender);
    void __fastcall actDiscardExecute(TObject *Sender);
    void __fastcall actShowPaneExecute(TObject *Sender);
    void __fastcall actChangeFontExecute(TObject *Sender);
    void __fastcall actSaveAsExecute(TObject *Sender);
    void __fastcall actDoSearchExecute(TObject *Sender);
    void __fastcall actCopyExecute(TObject *Sender);
    void __fastcall actCutExecute(TObject *Sender);
    void __fastcall actPasteExecute(TObject *Sender);
    void __fastcall actSelectAllExecute(TObject *Sender);
    void __fastcall actCloseFileExecute(TObject *Sender);
    void __fastcall aclActionsUpdate(TBasicAction *Action, bool &Handled);
    void __fastcall ctrMemoClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall ctrRightDockDockDrop(TObject *Sender, TDragDockObject *Source, int X, int Y);
    void __fastcall ctrRightDockUnDock(TObject *Sender, TControl *Client, TWinControl *NewTarget, bool &Allow);
    void __fastcall appEventsHint(TObject *Sender);
    void __fastcall appEventsIdle(TObject *Sender, bool &Done);
    void __fastcall edtSearchTextKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);

private:	// User declarations

    // . . . Types and constants
    enum en_StatusBarPanels { SP_POS=0, SP_MEX };
    //typedef enum {TYP_INI,TYP_XML,TYP_DATA} en_Type;

    // . . . Attributes
    String OpenedPath;
    TStream* OpenedStream;
    std::vector<TMemoPosition> Positions;
    bool get_Modified() {return synEditor->Modified;}
    // SynEdit objects
    class TSynEdit *synEditor;
    class TSynGeneralSyn *hghDat;
    class TSynIniSyn *hghIni;
    class TSynXMLSyn *hghXml;
    class TSynEditSearch *SynSearch;
    TSynSearchOptions SearchOptions;

    // . . . Utilities
    void __fastcall ToStatusBar(const String&, int=-1); // Write in statusbar
    String __fastcall FromStatusBar(int=-1); // Get from statusbar

    // . . . Services
    void __fastcall Init(); // Base constructor
    void __fastcall InitSynComponents();
    void ChooseStyle(String =""); // Assign Highlighter and other Editor properties
    void SpecificActions(); // Other specific actions depending on extension
    inline void EnableFileActions (const bool =true); // Enable/disable save action
    bool CanSave() const { return (!OpenedPath.IsEmpty() || OpenedStream); }

    // . . . Logistic
    void __fastcall Progress(System::TObject* Sender, int Size);
    void LongTask ( bool begin ); // Long task feedback
    void BeginLongTask(){LongTask(true);}
    void EndLongTask(){LongTask(false);}

    // . . . Docking
    void __fastcall DockOn (TWinControl*); // Resize docking host and show docked control
    void __fastcall UnDockFrom (TWinControl*); // Hide docking host if necessary

    // . . . synEditor events
    void __fastcall synEditorStatusChange(TObject *Sender, TSynStatusChanges Changes);
    void __fastcall synEditorDropFiles(TObject *Sender, int X, int Y, TStrings *AFiles);
    void __fastcall synEditorMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall synEditorReplaceText(TObject *Sender, const AnsiString ASearch, const AnsiString AReplace, int Line, int Column, TSynReplaceAction &Action);

// . . . Custom message handling
   //MESSAGE void __fastcall WhenDropFile(TWMDropFiles&); // Drag&drop file service routine
   //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ MESSAGE HANDLERS
   //BEGIN_MESSAGE_MAP
     //VCL_MESSAGE_HANDLER(WM_KEYDOWN, TWMKey, WhenKeyDown)
     //VCL_MESSAGE_HANDLER(WM_GETDLGCODE, TMessage, WhenGetDlgCode)
     //VCL_MESSAGE_HANDLER(CM_DIALOGKEY, TWMKey, WhenDialogKey)
     //VCL_MESSAGE_HANDLER(WM_DROPFILES, TWMDropFiles, WhenDropFile) // dropped files
   //END_MESSAGE_MAP(TForm)
   //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

public:		// User declarations
    __fastcall TfrmSynEditor(class TComponent* o) : TForm(o) {Init();};
    __fastcall TfrmSynEditor(class TComponent* o, class TStream* s) : TForm(o) { Init(); Open(s); };
    __fastcall TfrmSynEditor(class TComponent* o, const String& f) : TForm(o) { Init(); Open(f); };
    __fastcall ~TfrmSynEditor();

    // . . . I/O
    void Open( class TStream* ); // Open a stream
    void Save( class TStream* ); // Save to stream
    void Open( const String&  ); // Load from path
    void Save( const String&  ); // Save to path
    void CloseFile(); // Close current file

    // . . . SynEdit
    TStrings* synEditorLines() const {return synEditor->Lines;}
    void ListPosition (const String& Message, int Row, int Col); // Log an error
    void GoTo (const TMemoPosition&) const; // Go to position
    void HighlightLine( int Line );

    // . . . Properties
    __property bool Modified = { read=get_Modified };
    static int InstancesCount; // Counter of instances
};
//---------------------------------------------------------------------------
#endif
