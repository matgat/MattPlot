//---------------------------------------------------------------------------

#ifndef unt_MainFormH
#define unt_MainFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ActnList.hpp>
#include <Menus.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <StdActns.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
#include <AppEvnts.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class TfrmMplotMain : public TForm//                                  ...........
{
__published:    // IDE-managed Components
    TPanel *pnlPlotArea;
    TImageList *imlActions;
    TToolBar *tobMain;
    TStatusBar *barStatus;
    TProgressBar *barProgress;
    TApplicationEvents *appEvents;
    TActionList *aclMain;
    TAction *actExit;
    TAction *actOpen;
    TAction *actSave;
    TAction *actAddFrame;
    TAction *actAddTrack;
    TAction *actAbout;
    TAction *actEditOptions;
    TAction *actTrackData;
    TAction *actTrackProperties;
    TAction *actFrameProperties;
    TAction *actTrackStatistics;
    TAction *actFitToTracks;
    TAction *actGridMore;
    TAction *actGridLess;
    TAction *actShowTree;
    TAction *actGotoOrigin;
    TAction *actPanMode;
    TAction *actMeasureMode;
    TAction *actZoomMode;
    TAction *actGridToggle;
    TAction *actClearAll;
    TAction *actExport;
    TAction *actSelectMode;
    TAction *actShowLog;
    TAction *actRefresh;
    TAction *actCompute;
    TAction *actStoreOptions;
    TAction *actReadOptions;
    TAction *actLockX;
    TAction *actEasterEgg;
    TAction *actExportFrameToText;
    TAction *actRemoveFrame;
    TAction *actRemoveTrack;
    TAction *actAbort;
    TAction *actLegendToggle;
    TAction *actPrint;
    TAction *actDemo;
    TAction *actZoomIn;
    TAction *actZoomOut;
    TAction *actToggleScaleX;
    TAction *actToggleScaleY;
    TAction *actMarkPoints;
    TAction *actStrokeLines;
    TAction *actFillArea;
    TAction *actDrawHistograms;
    TAction *actShowCoords;
    TAction *actTrackTransform;
    TAction *actPenMode;
    TAction *actExportFrameToMatlab;
    TAction *actMeasureOrdinate;
    TAction *actMeasureSlope;
    TAction *actSnapshot;
    TAction *actMeasureAbscissa;
    TAction *actShowPane;
    TAction *actExperimentalStuff;
    TAction *actUpdate;
    TAction *actAddMarker;
    TAction *actTextEditor;
    TAction *actProperties;
    TAction *actLockY;
    TAction *actStayOnTop;
    TAction *actFitToAxes;
    TAction *actClearFrame;
    TAction *actSetStylePlain;
    TAction *actSetStylePSpice;
    TAction *actSetDefaults;
    TAction *actEditMode;
    TAction *actFitToTrack;
    TAction *actViewMeasureMarkers;
    TAction *actTrySomething;
    TAction *actCopyGrid;
    TAction *actAddText;
    THelpContents *actHelpContents;
    TMainMenu *mnuMain;
    TMenuItem *mnuFile;
    TMenuItem *mnuTrack;
    TMenuItem *mnuPlot;
    TMenuItem *mnuExport;
    TMenuItem *mnuTools;
    TMenuItem *mnuHelp;
    TMenuItem *mnuOpen;
    TMenuItem *mnuSep00;
    TMenuItem *mnuSave;
    TMenuItem *mnuSep01;
    TMenuItem *mnuExit;
    TMenuItem *mnuTrackData;
    TMenuItem *mnuSep11;
    TMenuItem *mnuSep20;
    TMenuItem *mnuTrackStyle;
    TMenuItem *mnuFrameStyle;
    TMenuItem *mnuEditOptions;
    TMenuItem *mnuResetView;
    TMenuItem *mnuSep23;
    TMenuItem *mnuViewAxes;
    TMenuItem *mnuTogglegrid;
    TMenuItem *mnuGridThick;
    TMenuItem *mnuGridThin;
    TMenuItem *mnuSep30;
    TMenuItem *mnuShowTree;
    TMenuItem *mnuPanMode;
    TMenuItem *mnuZoomMode;
    TMenuItem *mnuMeasureMode;
    TMenuItem *mnuSep40;
    TMenuItem *mnuStatistics;
    TMenuItem *mnuHelpContents;
    TMenuItem *mnuSep50;
    TMenuItem *mnuAbout;
    TMenuItem *mnuClearAll;
    TMenuItem *mnuShowLog;
    TMenuItem *mnuEditMode;
    TMenuItem *mnuElaborate;
    TMenuItem *mnuMode;
    TMenuItem *mnuSep10;
    TMenuItem *mnuSaveOptions;
    TMenuItem *mnuLoadOptions;
    TMenuItem *mnuPlainText;
    TMenuItem *mnuAddTracks;
    TMenuItem *mnuAddtrack;
    TMenuItem *mnuRefresh;
    TMenuItem *mnuSep21;
    TMenuItem *mnuRemoveframe;
    TMenuItem *mnuRemovetrack;
    TMenuItem *mnuToggleLegend;
    TMenuItem *mnuPrint;
    TMenuItem *mnuDemo;
    TMenuItem *mnuAbort;
    TMenuItem *mnuFitToAxes;
    TMenuItem *mnuZoomP;
    TMenuItem *mnuZoomM;
    TMenuItem *mnuToggleXScale;
    TMenuItem *mnuToggleYScale;
    TMenuItem *mnuFrame;
    TMenuItem *mnuSyncX;
    TMenuItem *mnuPenMode;
    TMenuItem *mnuToMatlab;
    TMenuItem *pitMeasureOrdinate;
    TMenuItem *pitMeasureSlope;
    TMenuItem *mnuSep24;
    TMenuItem *mnuScreenShoot;
    TMenuItem *pitMeasureAbscissa;
    TMenuItem *mnuSaveBitmapAs;
    TMenuItem *mnuShowdisplay;
    TMenuItem *mnuTrackPlottingStyle;
    TMenuItem *mnuMarkPoints;
    TMenuItem *mnuStrokeLines;
    TMenuItem *mniFillArea;
    TMenuItem *mnuDrawHistograms;
    TMenuItem *mnuSep31;
    TMenuItem *mnuTrackTransform;
    TMenuItem *mnuMeasureLines;
    TMenuItem *mnuMeasureAbscissa;
    TMenuItem *mnuMeasureOrdinate;
    TMenuItem *mnuMeasureSlope;
    TMenuItem *mnuExperimentalStuff;
    TMenuItem *mnuUpdate;
    TMenuItem *mnuAddToFrame;
    TMenuItem *mnuAddMarker;
    TMenuItem *mnuTextEditor;
    TMenuItem *mnuStayOnTop;
    TMenuItem *mnuSyncY;
    TMenuItem *mnuClearFrame;
    TMenuItem *mnySetStyles;
    TMenuItem *mnuSetStylePlain;
    TMenuItem *mnuSetStylePSpice;
    TMenuItem *mnuSetDefaults;
    TMenuItem *mnuSep25;
    TMenuItem *mnuFitToTrack;
    TMenuItem *pitMeasureMarkers;
    TMenuItem *mnuMeasureMarkers;
    TMenuItem *pitCompute;
    TMenuItem *pitTrackTransform;
    TMenuItem *mnuPlotAdd;
    TMenuItem *mnuPlotAddFrame;
    TMenuItem *mnuFrameAddTrack;
    TMenuItem *mnuSep12;
    TMenuItem *mnuProperties;
    TMenuItem *pitCopyGridAll;
    TMenuItem *mnuAddText;
    TMenuItem *mnuShowCoords;
    TMenuItem *mnuExecMacro;
    TPopupMenu *popPaneGrid;
    TImageList *imlTree;
    TPopupMenu *popEdit;
    TPopupMenu *popFoilObject;
    TPopupMenu *popMeasure;
    TPanel *ctrRightDock;
    TPanel *ctrBottomDock;
    TPanel *ctrLeftDock;
    TSplitter *splBottomDock;
    TSplitter *splRightDock;
    TSplitter *splLeftDock;
    TPanel *ctrTree;
    TTreeView *treePlot;
    TPanel *ctrPane;
    TStringGrid *grdPane;
    TPanel *ctrLog;
    TMemo *memLog;
    TAction *actExecMacro;
    TMenuItem *mnuSep41;
    TAction *actRemove;
    TAction *actWhatsNew;
    TMenuItem *mnuObject;
    TMenuItem *mnuRemove;
    TMenuItem *mnuWhatsnew;
    void __fastcall actOpenExecute(TObject *Sender);
    void __fastcall actAddFrameExecute(TObject *Sender);
    void __fastcall actAddTrackExecute(TObject *Sender);
    void __fastcall actSaveExecute(TObject *Sender);
    void __fastcall actExitExecute(TObject *Sender);
    void __fastcall actTrackDataExecute(TObject *Sender);
    void __fastcall actTrackPropertiesExecute(TObject *Sender);
    void __fastcall actFramePropertiesExecute(TObject *Sender);
    void __fastcall actEditOptionsExecute(TObject *Sender);
    void __fastcall actFitToTracksExecute(TObject *Sender);
    void __fastcall actGotoOriginExecute(TObject *Sender);
    void __fastcall actGridMoreExecute(TObject *Sender);
    void __fastcall actGridLessExecute(TObject *Sender);
    void __fastcall actShowTreeExecute(TObject *Sender);
    void __fastcall actPanModeExecute(TObject *Sender);
    void __fastcall actZoomModeExecute(TObject *Sender);
    void __fastcall actMeasureModeExecute(TObject *Sender);
    void __fastcall actTrackStatisticsExecute(TObject *Sender);
    void __fastcall actHelpContentsExecute(TObject *Sender);
    void __fastcall actAboutExecute(TObject *Sender);
    void __fastcall actGridToggleExecute(TObject *Sender);
    void __fastcall actExportExecute(TObject *Sender);
    void __fastcall actClearAllExecute(TObject *Sender);
    void __fastcall actSelectModeExecute(TObject *Sender);
    void __fastcall actShowLogExecute(TObject *Sender);
    void __fastcall aclMainUpdate(TBasicAction *Action, bool &Handled);
    void __fastcall actReadOptionsExecute(TObject *Sender);
    void __fastcall actStoreOptionsExecute(TObject *Sender);
    void __fastcall actLockXExecute(TObject *Sender);
    void __fastcall actEasterEggExecute(TObject *Sender);
    void __fastcall appEventsException(TObject *Sender, Exception *E);
    bool __fastcall appEventsHelp(WORD Command, int Data, bool &CallHelp);
    void __fastcall actExportFrameToTextExecute(TObject *Sender);
    void __fastcall actRefreshExecute(TObject *Sender);
    void __fastcall actRemoveFrameExecute(TObject *Sender);
    void __fastcall actRemoveTrackExecute(TObject *Sender);
    void __fastcall actAbortExecute(TObject *Sender);
    void __fastcall actLegendToggleExecute(TObject *Sender);
    void __fastcall actPrintExecute(TObject *Sender);
    void __fastcall actDemoExecute(TObject *Sender);
    void __fastcall actToggleScaleXExecute(TObject *Sender);
    void __fastcall actToggleScaleYExecute(TObject *Sender);
    void __fastcall actMarkPointsExecute(TObject *Sender);
    void __fastcall actStrokeLinesExecute(TObject *Sender);
    void __fastcall actFillAreaExecute(TObject *Sender);
    void __fastcall actDrawHistogramsExecute(TObject *Sender);
    void __fastcall actShowCoordsExecute(TObject *Sender);
    void __fastcall actZoomOutExecute(TObject *Sender);
    void __fastcall actZoomInExecute(TObject *Sender);
    void __fastcall appEventsHint(TObject *Sender);
    void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
    void __fastcall FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
    void __fastcall actPenModeExecute(TObject *Sender);
    void __fastcall actExportFrameToMatlabExecute(TObject *Sender);
    void __fastcall actComputeExecute(TObject *Sender);
    void __fastcall actMeasureOrdinateExecute(TObject *Sender);
    void __fastcall actMeasureSlopeExecute(TObject *Sender);
    void __fastcall actSnapshotExecute(TObject *Sender);
    void __fastcall actMeasureAbscissaExecute(TObject *Sender);
    void __fastcall ctrRightDockDockDrop(TObject *Sender, TDragDockObject *Source, int X, int Y);
    void __fastcall ctrRightDockUnDock(TObject *Sender, TControl *Client, TWinControl *NewTarget, bool &Allow);
    void __fastcall FormDeactivate(TObject *Sender);
    void __fastcall actShowPaneExecute(TObject *Sender);
    void __fastcall actTrackTransformExecute(TObject *Sender);
    void __fastcall actExperimentalStuffExecute(TObject *Sender);
    void __fastcall actUpdateExecute(TObject *Sender);
    void __fastcall actAddMarkerExecute(TObject *Sender);
    void __fastcall actTextEditorExecute(TObject *Sender);
    void __fastcall barStatusClick(TObject *Sender);
    void __fastcall actPropertiesExecute(TObject *Sender);
    void __fastcall actLockYExecute(TObject *Sender);
    void __fastcall actStayOnTopExecute(TObject *Sender);
    void __fastcall actFitToAxesExecute(TObject *Sender);
    void __fastcall appEventsDeactivate(TObject *Sender);
    void __fastcall actClearFrameExecute(TObject *Sender);
    void __fastcall actSetStylePlainExecute(TObject *Sender);
    void __fastcall actSetStylePSpiceExecute(TObject *Sender);
    void __fastcall actSetDefaultsExecute(TObject *Sender);
    void __fastcall actEditModeExecute(TObject *Sender);
    void __fastcall actFitToTrackExecute(TObject *Sender);
    void __fastcall actViewMeasureMarkersExecute(TObject *Sender);
    void __fastcall actTrySomethingExecute(TObject *Sender);
    void __fastcall grdPaneDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State);
    void __fastcall actCopyGridExecute(TObject *Sender);
    void __fastcall grdPaneMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall actAddTextExecute(TObject *Sender);
    void __fastcall treePlotDragOver(TObject *Sender, TObject *Source, int X, int Y, TDragState State, bool &Accept);
    void __fastcall treePlotDragDrop(TObject *Sender, TObject *Source, int X, int Y);
    void __fastcall treePlotMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall actExecMacroExecute(TObject *Sender);
    void __fastcall actRemoveExecute(TObject *Sender);
    void __fastcall actWhatsNewExecute(TObject *Sender);
    void __fastcall appEventsActivate(TObject *Sender);

private://                                                         ...........

// . . . Types
    enum en_FileOpenOpts { oo_ClearAll, oo_SameFrame };
    typedef Set<en_FileOpenOpts, oo_ClearAll, oo_SameFrame> typ_FileOpenOpts;

// . . . Private user attributes
    class TForm* ModalChild;
    class cls_MPlot* Plot;
    class cls_HtmlHelp* HH;
    //Graphics::TBitmap *bmpTrack,*bmpFrame;
    bool Closing; // Destroying main form
    bool Busy; // Busy state
    bool UserAbort; // set by abort action
    POINT DropPoint; // Coordinates of drop operation

// . . . Private user methods
    // Utilities
    String GetHomePath(); // Get user home directory
    String GetTmpPath(); // Get temporary dir path
    void ProcessArguments(const mat::stringlist&);
    void ProcessFile(const String& path, const String& ="", const typ_FileOpenOpts& =typ_FileOpenOpts());
    void __fastcall AfterReadOptions(); // Option events
    void __fastcall PrepareStoreOptions(); // Option events
    // Actions utilities
	inline String GetNameFromAction(TContainedAction*); // Remove 'act' prefix
	TBasicAction* GetActionFromName(const String&); // Get action from name
    int GetActionIdx(String); // Get action index from name
    void ActionNamesToIndexes (const mat::stringlist&, mat::intlist*); // Build an indexes vector from action names
    void __fastcall AddActionNamesToHints(); // Add action names to their hints
    void __fastcall EnableNormalActions(const bool =true); // Enable/Disable all actions
    void __fastcall BuildToolbar (const mat::stringlist&); // Build toolbar from list of action names
    void __fastcall ClearToolButtons(TToolBar*); // Clear toolbuttons from a toolbar
    void BuildPopMenu (TPopupMenu*, const mat::intlist*); // Build pop menu from action indexes of aclMain
    void __fastcall ClearPopMenu(TPopupMenu*); // Clear pop menu
    void __fastcall DeleteMenuItem(TMenuItem*); // Delete a menu item

    // Services
    void __fastcall OneShotThings(TObject *Sender, bool &Done);
    void __fastcall IdleStuff(TObject *Sender, bool &Done);
    enum en_StatusBarPanels { SP_PNT=0, SP_SEL=1, SP_SYS=2, SP_MEX=3 }; // StatusBar panels indexes
    void __fastcall ToStatusBar(const String&, int=-1); // Write in statusbar
    String __fastcall FromStatusBar(int=-1); // Get from statusbar
    void __fastcall DisplayMessage(const String& msg) {ToStatusBar(msg);} // Assigned to Log->OnEntry
    inline String GetOptionFilePath(); // Get option file path
    inline String GetHelpFilePath(); // Get help file path
    String EnsureHelpFile(const String&); // Ensure help file presence extracting in resource if necessary
    void __fastcall BuildGridPane(TStringGrid*); // Set grid cells value pointers

    // Long tasks feedback
    void __fastcall DoLongTask(); // Feedback during waiting
    void __fastcall EndLongTask(); // Ending a long task (feedback)
    void __fastcall Progress (int,bool&);
    void __fastcall ImageProgress(System::TObject*,TProgressStage,Byte,bool,const TRect&,const AnsiString);

    // Plot services
    void __fastcall OnPlotUpdate(); // Plot's updated: update interface
    void __fastcall OnPlotChange(); // Plot's layout changed: update tree
    void __fastcall OnPlotPaint(); // Plot just painted
    //void __fastcall OnPlotChange(); // Plot's changed: update interface
    void __fastcall OnPlotPopup(const TPoint&); // Right click plot event
    void __fastcall CreatePlotPopup(const TPoint&); // Plot properties popupmenu creator
    bool SavePlotBitmap (const String& path, class TImage*); // Save Plot bitmap
    bool GetPlotMetafile (class cls_Frame*, const String&, const bool) const; // Build Metafile from Plot
    //void DrawPlot (class TCanvas*,const TRect&); // Draw Plot in a Canvas via Metafile
    bool Export(const String&, class cls_Frame*, TImage*); // Export plot or current frame

    // Dock uts
    //void __fastcall ResizeDockArea (TWinControl*, TControl*); // Resize docking host
    void __fastcall DockOn (TWinControl*, TControl*); // Resize docking host and show docked control
    void __fastcall UnDockFrom (TWinControl*); // Hide docking host if necessary
    // Low level
    //inline int VisibleControlCount(TWinControl*); // Count visible controls contained in a wincontrol

// . . . Custom message handling
   void __fastcall WhenDialogKey (TWMKey& Message)
   { // Handle special keys (like TAB)
        //if (Message.CharCode != VK_TAB) { TForm::Dispatch((void *) &Message);
        if(OnKeyDown) OnKeyDown(this,Message.CharCode,::KeyDataToShiftState(Message.KeyData));
        //inherited::Dispatch(&Message);
   }
   //MESSAGE void __fastcall WhenGetDlgCode (TMessage& Message) {Message.Result=DLGC_WANTALLKEYS;}
   MESSAGE void __fastcall WhenDropFile(TWMDropFiles&); // Drag&drop file service routine
   MESSAGE void __fastcall WhenCopyData(TWMCopyData&); // Get the command line from another instance
   MESSAGE void __fastcall WhenTrainingCard (TMessage& Message); // Messages from HTMLHELP

   //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ MESSAGE HANDLERS
   BEGIN_MESSAGE_MAP
     //VCL_MESSAGE_HANDLER(WM_KEYDOWN, TWMKey, WhenKeyDown)
     //VCL_MESSAGE_HANDLER(WM_GETDLGCODE, TMessage, WhenGetDlgCode)
     VCL_MESSAGE_HANDLER(CM_DIALOGKEY, TWMKey, WhenDialogKey)
     VCL_MESSAGE_HANDLER(WM_DROPFILES, TWMDropFiles, WhenDropFile)
     VCL_MESSAGE_HANDLER(WM_TCARD, TMessage, WhenTrainingCard)
     VCL_MESSAGE_HANDLER(WM_COPYDATA, TWMCopyData, WhenCopyData)
   END_MESSAGE_MAP(TForm)
   //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

public://                                                         ...........
// . . . User public methods
        __fastcall TfrmMplotMain(TComponent* Owner);
        __fastcall ~TfrmMplotMain();

// . . . Public attributes (!)
        //static const TCursor crFuck;
        //static const String AppName;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMplotMain *frmMplotMain;
//---------------------------------------------------------------------------
#endif
