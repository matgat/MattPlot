// Borland C++ Builder
// Copyright (c) 1995, 1999 by Borland International
// All rights reserved

// (DO NOT EDIT: machine generated header) 'SynEdit.pas' rev: 5.00

#ifndef SynEditHPP
#define SynEditHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <Menus.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Math.hpp>	// Pascal unit
#include <SynEditKbdHandler.hpp>	// Pascal unit
#include <SynEditHighlighter.hpp>	// Pascal unit
#include <SynEditKeyCmds.hpp>	// Pascal unit
#include <SynEditTextBuffer.hpp>	// Pascal unit
#include <SynEditMiscClasses.hpp>	// Pascal unit
#include <SynEditMiscProcs.hpp>	// Pascal unit
#include <SynEditKeyConst.hpp>	// Pascal unit
#include <SynEditTypes.hpp>	// Pascal unit
#include <SynTextDrawer.hpp>	// Pascal unit
#include <Imm.hpp>	// Pascal unit
#include <Messages.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <ExtCtrls.hpp>	// Pascal unit
#include <StdCtrls.hpp>	// Pascal unit
#include <Forms.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Synedit
{
//-- type declarations -------------------------------------------------------
typedef Forms::TFormBorderStyle TSynBorderStyle;

typedef Synedittypes::TBufferCoord  TBufferCoord;

typedef Synedittypes::TDisplayCoord  TDisplayCoord;

typedef TSynEditMark TSynEditMark;
;

#pragma option push -b-
enum TSynReplaceAction { raCancel, raSkip, raReplace, raReplaceAll };
#pragma option pop

class DELPHICLASS ESynEditError;
class PASCALIMPLEMENTATION ESynEditError : public Sysutils::Exception 
{
	typedef Sysutils::Exception inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall ESynEditError(const AnsiString Msg) : Sysutils::Exception(
		Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall ESynEditError(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : Sysutils::Exception(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall ESynEditError(int Ident)/* overload */ : Sysutils::Exception(
		Ident) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall ESynEditError(int Ident, const System::TVarRec * Args
		, const int Args_Size)/* overload */ : Sysutils::Exception(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall ESynEditError(const AnsiString Msg, int AHelpContext) : 
		Sysutils::Exception(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall ESynEditError(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : Sysutils::Exception(Msg, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall ESynEditError(int Ident, int AHelpContext)/* overload */
		 : Sysutils::Exception(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall ESynEditError(System::PResStringRec ResStringRec
		, const System::TVarRec * Args, const int Args_Size, int AHelpContext)/* overload */ : Sysutils::Exception(
		ResStringRec, Args, Args_Size, AHelpContext) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~ESynEditError(void) { }
	#pragma option pop
	
};


typedef void __fastcall (__closure *TDropFilesEvent)(System::TObject* Sender, int X, int Y, Classes::TStrings* 
	AFiles);

typedef void __fastcall (__closure *THookedCommandEvent)(System::TObject* Sender, bool AfterProcessing
	, bool &Handled, Syneditkeycmds::TSynEditorCommand &Command, char &AChar, void * Data, void * HandlerData
	);

typedef void __fastcall (__closure *TPaintEvent)(System::TObject* Sender, Graphics::TCanvas* ACanvas
	);

typedef void __fastcall (__closure *TProcessCommandEvent)(System::TObject* Sender, Syneditkeycmds::TSynEditorCommand 
	&Command, char &AChar, void * Data);

typedef void __fastcall (__closure *TReplaceTextEvent)(System::TObject* Sender, const AnsiString ASearch
	, const AnsiString AReplace, int Line, int Column, TSynReplaceAction &Action);

typedef void __fastcall (__closure *TSpecialLineColorsEvent)(System::TObject* Sender, int Line, bool 
	&Special, Graphics::TColor &FG, Graphics::TColor &BG);

#pragma option push -b-
enum TTransientType { ttBefore, ttAfter };
#pragma option pop

typedef void __fastcall (__closure *TPaintTransient)(System::TObject* Sender, Graphics::TCanvas* Canvas
	, TTransientType TransientType);

typedef void __fastcall (__closure *TScrollEvent)(System::TObject* Sender, Forms::TScrollBarKind ScrollBar
	);

typedef void __fastcall (__closure *TGutterGetTextEvent)(System::TObject* Sender, int aLine, AnsiString 
	&aText);

typedef void __fastcall (__closure *TGutterPaintEvent)(System::TObject* Sender, int aLine, int X, int 
	Y);

#pragma option push -b-
enum TSynEditCaretType { ctVerticalLine, ctHorizontalLine, ctHalfBlock, ctBlock };
#pragma option pop

#pragma option push -b-
enum TSynStateFlag { sfCaretChanged, sfScrollbarChanged, sfLinesChanging, sfIgnoreNextChar, sfCaretVisible, 
	sfDblClicked, sfPossibleGutterClick, sfWaitForDragging, sfGutterDragging };
#pragma option pop

typedef Set<TSynStateFlag, sfCaretChanged, sfGutterDragging>  TSynStateFlags;

#pragma option push -b-
enum TScrollHintFormat { shfTopLineOnly, shfTopToBottom };
#pragma option pop

#pragma option push -b-
enum TSynEditorOption { eoAltSetsColumnMode, eoAutoIndent, eoAutoSizeMaxScrollWidth, eoDisableScrollArrows, 
	eoDragDropEditing, eoDropFiles, eoEnhanceHomeKey, eoEnhanceEndKey, eoGroupUndo, eoHalfPageScroll, eoHideShowScrollbars, 
	eoKeepCaretX, eoNoCaret, eoNoSelection, eoRightMouseMovesCursor, eoScrollByOneLess, eoScrollHintFollows, 
	eoScrollPastEof, eoScrollPastEol, eoShowScrollHint, eoShowSpecialChars, eoSmartTabDelete, eoSmartTabs, 
	eoSpecialLineDefaultFg, eoTabIndent, eoTabsToSpaces, eoTrimTrailingSpaces };
#pragma option pop

typedef Set<TSynEditorOption, eoAltSetsColumnMode, eoTrimTrailingSpaces>  TSynEditorOptions;

#pragma option push -b-
enum TSynStatusChange { scAll, scCaretX, scCaretY, scLeftChar, scTopLine, scInsertMode, scModified, 
	scSelection, scReadOnly };
#pragma option pop

typedef Set<TSynStatusChange, scAll, scReadOnly>  TSynStatusChanges;

typedef void __fastcall (__closure *TContextHelpEvent)(System::TObject* Sender, AnsiString word);

typedef void __fastcall (__closure *TStatusChangeEvent)(System::TObject* Sender, TSynStatusChanges Changes
	);

typedef void __fastcall (__closure *TMouseCursorEvent)(System::TObject* Sender, const Synedittypes::TBufferCoord 
	&aLineCharPos, Controls::TCursor &aCursor);

typedef void __fastcall (__closure *TPlaceMarkEvent)(System::TObject* Sender, Syneditmiscclasses::TSynEditMark* 
	&Mark);

typedef void __fastcall (__closure *TGutterClickEvent)(System::TObject* Sender, Controls::TMouseButton 
	Button, int X, int Y, int Line, Syneditmiscclasses::TSynEditMark* Mark);

__interface ISynEditBufferPlugin;
typedef System::DelphiInterface<ISynEditBufferPlugin> _di_ISynEditBufferPlugin;
__interface ISynEditBufferPlugin  : public IUnknown 
{
	
public:
	virtual Synedittypes::TDisplayCoord __fastcall BufferToDisplayPos(const Synedittypes::TBufferCoord 
		&aPos) = 0 ;
	virtual Synedittypes::TBufferCoord __fastcall DisplayToBufferPos(const Synedittypes::TDisplayCoord 
		&aPos) = 0 ;
	virtual int __fastcall RowCount(void) = 0 ;
	virtual int __fastcall GetRowLength(int aRow) = 0 ;
	virtual int __fastcall LinesInserted(int aIndex, int aCount) = 0 ;
	virtual int __fastcall LinesDeleted(int aIndex, int aCount) = 0 ;
	virtual int __fastcall LinesPutted(int aIndex, int aCount) = 0 ;
	virtual void __fastcall DisplayChanged(void) = 0 ;
	virtual void __fastcall Reset(void) = 0 ;
};

class DELPHICLASS TSynEditPlugin;
class DELPHICLASS TCustomSynEdit;
class PASCALIMPLEMENTATION TCustomSynEdit : public Controls::TCustomControl 
{
	typedef Controls::TCustomControl inherited;
	
private:
	HIDESBASE MESSAGE void __fastcall WMCancelMode(Messages::TMessage &Message);
	MESSAGE void __fastcall WMCaptureChanged(Messages::TMessage &Msg);
	MESSAGE void __fastcall WMClear(Messages::TMessage &Msg);
	MESSAGE void __fastcall WMCopy(Messages::TMessage &Message);
	MESSAGE void __fastcall WMCut(Messages::TMessage &Message);
	MESSAGE void __fastcall WMDropFiles(Messages::TMessage &Msg);
	HIDESBASE MESSAGE void __fastcall WMEraseBkgnd(Messages::TMessage &Msg);
	MESSAGE void __fastcall WMGetDlgCode(Messages::TWMNoParams &Msg);
	MESSAGE void __fastcall WMGetText(Messages::TWMGetText &Msg);
	MESSAGE void __fastcall WMGetTextLength(Messages::TWMNoParams &Msg);
	HIDESBASE MESSAGE void __fastcall WMHScroll(Messages::TWMScroll &Msg);
	MESSAGE void __fastcall WMPaste(Messages::TMessage &Message);
	MESSAGE void __fastcall WMSetText(Messages::TWMSetText &Msg);
	MESSAGE void __fastcall WMImeComposition(Messages::TMessage &Msg);
	MESSAGE void __fastcall WMImeNotify(Messages::TMessage &Msg);
	HIDESBASE MESSAGE void __fastcall WMKillFocus(Messages::TWMKillFocus &Msg);
	HIDESBASE MESSAGE void __fastcall WMSetCursor(Messages::TWMSetCursor &Msg);
	HIDESBASE MESSAGE void __fastcall WMSetFocus(Messages::TWMSetFocus &Msg);
	HIDESBASE MESSAGE void __fastcall WMSize(Messages::TWMSize &Msg);
	MESSAGE void __fastcall WMUndo(Messages::TMessage &Msg);
	HIDESBASE MESSAGE void __fastcall WMVScroll(Messages::TWMScroll &Msg);
	HIDESBASE MESSAGE void __fastcall WMMouseWheel(Messages::TMessage &Msg);
	bool fAlwaysShowCaret;
	Synedittypes::TBufferCoord fBlockBegin;
	Synedittypes::TBufferCoord fBlockEnd;
	int fCaretX;
	int fLastCaretX;
	int fCaretY;
	int fCharsInWindow;
	int fCharWidth;
	Graphics::TFont* fFontDummy;
	int fImeCount;
	bool fMBCSStepAside;
	bool fInserting;
	Classes::TStrings* fLines;
	Classes::TStrings* fOrigLines;
	Synedittextbuffer::TSynEditUndoList* fOrigUndoList;
	Synedittextbuffer::TSynEditUndoList* fOrigRedoList;
	int fLinesInWindow;
	int fLeftChar;
	int fMaxScrollWidth;
	int fPaintLock;
	bool fReadOnly;
	int fRightEdge;
	Graphics::TColor fRightEdgeColor;
	Graphics::TColor fScrollHintColor;
	TScrollHintFormat fScrollHintFormat;
	Stdctrls::TScrollStyle FScrollBars;
	int fTextHeight;
	int fTextOffset;
	int fTopLine;
	Synedithighlighter::TSynCustomHighlighter* fHighlighter;
	Syneditmiscclasses::TSynSelectedColor* fSelectedColor;
	Graphics::TColor fActiveLineColor;
	Synedittextbuffer::TSynEditUndoList* fUndoList;
	Synedittextbuffer::TSynEditUndoList* fRedoList;
	Syneditmiscclasses::TSynEditMark* fBookMarks[10];
	int fMouseDownX;
	int fMouseDownY;
	Syneditmiscclasses::TSynBookMarkOpt* fBookMarkOpt;
	Forms::TFormBorderStyle fBorderStyle;
	bool fHideSelection;
	int fMouseWheelAccumulator;
	TSynEditCaretType fOverwriteCaret;
	TSynEditCaretType fInsertCaret;
	Windows::TPoint fCaretOffset;
	Syneditkeycmds::TSynEditKeyStrokes* fKeyStrokes;
	bool fModified;
	Syneditmiscclasses::TSynEditMarkList* fMarkList;
	int fExtraLineSpacing;
	Synedittypes::TSynSelectionMode fSelectionMode;
	Synedittypes::TSynSelectionMode fActiveSelectionMode;
	bool fWantReturns;
	bool fWantTabs;
	_di_ISynEditBufferPlugin fWordWrapPlugin;
	Syneditmiscclasses::TSynGlyph* fWordWrapGlyph;
	bool fCaretAtEOL;
	Syneditmiscclasses::TSynGutter* fGutter;
	int fTabWidth;
	Syntextdrawer::TheTextDrawer* fTextDrawer;
	Windows::TRect fInvalidateRect;
	TSynStateFlags fStateFlags;
	TSynEditorOptions fOptions;
	TSynStatusChanges fStatusChanges;
	Word fLastKey;
	Classes::TShiftState fLastShiftState;
	Syneditmiscclasses::TSynEditSearchCustom* fSearchEngine;
	Classes::TList* fHookedCommandHandlers;
	Syneditkbdhandler::TSynEditKbdHandler* fKbdHandler;
	Classes::TList* fFocusList;
	Classes::TList* fPlugins;
	Extctrls::TTimer* fScrollTimer;
	int fScrollDeltaX;
	int fScrollDeltaY;
	Classes::TNotifyEvent fOnChange;
	TPlaceMarkEvent fOnClearMark;
	TProcessCommandEvent fOnCommandProcessed;
	TDropFilesEvent fOnDropFiles;
	TGutterClickEvent fOnGutterClick;
	TMouseCursorEvent fOnMouseCursor;
	TPaintEvent fOnPaint;
	TPlaceMarkEvent fOnPlaceMark;
	TProcessCommandEvent fOnProcessCommand;
	TProcessCommandEvent fOnProcessUserCommand;
	TReplaceTextEvent fOnReplaceText;
	TSpecialLineColorsEvent fOnSpecialLineColors;
	TContextHelpEvent fOnContextHelp;
	TPaintTransient fOnPaintTransient;
	TScrollEvent fOnScroll;
	TGutterGetTextEvent fOnGutterGetText;
	TGutterPaintEvent fOnGutterPaint;
	TStatusChangeEvent fOnStatusChange;
	bool fShowSpecChar;
	int FPaintTransientLock;
	bool FIsScrolling;
	Classes::TNotifyEvent fChainListCleared;
	Synedittextbuffer::TStringListChangeEvent fChainListDeleted;
	Synedittextbuffer::TStringListChangeEvent fChainListInserted;
	Synedittextbuffer::TStringListChangeEvent fChainListPutted;
	Classes::TNotifyEvent fChainLinesChanging;
	Classes::TNotifyEvent fChainLinesChanged;
	TCustomSynEdit* fChainedEditor;
	Classes::TNotifyEvent fChainUndoAdded;
	Classes::TNotifyEvent fChainRedoAdded;
	void __fastcall BookMarkOptionsChanged(System::TObject* Sender);
	void __fastcall ComputeCaret(int X, int Y);
	void __fastcall ComputeScroll(int X, int Y);
	void __fastcall DoBlockIndent(void);
	void __fastcall DoBlockUnindent(void);
	void __fastcall DoHomeKey(bool Selection);
	void __fastcall DoEndKey(bool aSelection);
	void __fastcall DoLinesDeleted(int FirstLine, int Count);
	void __fastcall DoLinesInserted(int FirstLine, int Count);
	void __fastcall DoShiftTabKey(void);
	void __fastcall DoTabKey(void);
	void __fastcall DoCaseChange(const Syneditkeycmds::TSynEditorCommand Cmd);
	int __fastcall FindHookedCmdEvent(THookedCommandEvent AHandlerProc);
	void __fastcall SynFontChanged(System::TObject* Sender);
	Synedittypes::TBufferCoord __fastcall GetBlockBegin(void);
	Synedittypes::TBufferCoord __fastcall GetBlockEnd(void);
	bool __fastcall GetCanPaste(void);
	bool __fastcall GetCanRedo(void);
	bool __fastcall GetCanUndo(void);
	Synedittypes::TBufferCoord __fastcall GetCaretXY(void);
	int __fastcall GetDisplayX(void);
	int __fastcall GetDisplayY(void);
	Synedittypes::TDisplayCoord __fastcall GetDisplayXY(void);
	int __fastcall GetDisplayLineCount(void);
	Graphics::TFont* __fastcall GetFont(void);
	int __fastcall GetHookedCommandHandlersCount(void);
	AnsiString __fastcall GetLineText(void);
	int __fastcall GetMaxUndo(void);
	TSynEditorOptions __fastcall GetOptions(void);
	bool __fastcall GetSelAvail(void);
	bool __fastcall GetSelTabBlock(void);
	bool __fastcall GetSelTabLine(void);
	AnsiString __fastcall GetSelText(void);
	AnsiString __fastcall SynGetText(void);
	AnsiString __fastcall GetWordAtCursor(void);
	AnsiString __fastcall GetWordAtMouse(void);
	bool __fastcall GetWordWrap(void);
	void __fastcall GutterChanged(System::TObject* Sender);
	void __fastcall InsertBlock(const Synedittypes::TBufferCoord &BB, const Synedittypes::TBufferCoord 
		&BE, char * ChangeStr, bool AddToUndoList);
	int __fastcall LeftSpaces(const AnsiString Line);
	int __fastcall LeftSpacesEx(const AnsiString Line, bool WantTabs);
	AnsiString __fastcall GetLeftSpacing(int CharCount, bool WantTabs);
	void __fastcall LinesChanging(System::TObject* Sender);
	void __fastcall MoveCaretAndSelection(const Synedittypes::TBufferCoord &ptBefore, const Synedittypes::TBufferCoord 
		&ptAfter, bool SelectionCommand);
	void __fastcall MoveCaretHorz(int DX, bool SelectionCommand);
	void __fastcall MoveCaretVert(int DY, bool SelectionCommand);
	void __fastcall PluginsAfterPaint(Graphics::TCanvas* ACanvas, const Windows::TRect &AClip, int FirstLine
		, int LastLine);
	void __fastcall ReadAddedKeystrokes(Classes::TReader* Reader);
	void __fastcall ReadRemovedKeystrokes(Classes::TReader* Reader);
	int __fastcall ScanFrom(int Index);
	void __fastcall ScrollTimerHandler(System::TObject* Sender);
	void __fastcall SelectedColorsChanged(System::TObject* Sender);
	void __fastcall SetBlockBegin(const Synedittypes::TBufferCoord &Value);
	void __fastcall SetBlockEnd(const Synedittypes::TBufferCoord &Value);
	void __fastcall SetBorderStyle(Forms::TBorderStyle Value);
	void __fastcall SetCaretX(int Value);
	void __fastcall SetCaretY(int Value);
	void __fastcall InternalSetCaretX(int Value);
	void __fastcall InternalSetCaretY(int Value);
	void __fastcall SetInternalDisplayXY(const Synedittypes::TDisplayCoord &aPos);
	void __fastcall SetActiveLineColor(Graphics::TColor Value);
	void __fastcall SetExtraLineSpacing(const int Value);
	HIDESBASE void __fastcall SetFont(const Graphics::TFont* Value);
	void __fastcall SetGutter(const Syneditmiscclasses::TSynGutter* Value);
	void __fastcall SetGutterWidth(int Value);
	void __fastcall SetHideSelection(const bool Value);
	void __fastcall SetHighlighter(const Synedithighlighter::TSynCustomHighlighter* Value);
	void __fastcall SetInsertCaret(const TSynEditCaretType Value);
	void __fastcall SetInsertMode(const bool Value);
	void __fastcall SetKeystrokes(const Syneditkeycmds::TSynEditKeyStrokes* Value);
	void __fastcall SetLeftChar(int Value);
	void __fastcall SetLines(Classes::TStrings* Value);
	void __fastcall SetLineText(AnsiString Value);
	void __fastcall SetMaxScrollWidth(int Value);
	void __fastcall SetMaxUndo(const int Value);
	void __fastcall SetModified(bool Value);
	void __fastcall SetOptions(TSynEditorOptions Value);
	void __fastcall SetOverwriteCaret(const TSynEditCaretType Value);
	void __fastcall SetRightEdge(int Value);
	void __fastcall SetRightEdgeColor(Graphics::TColor Value);
	void __fastcall SetScrollBars(const Stdctrls::TScrollStyle Value);
	void __fastcall SetSearchEngine(Syneditmiscclasses::TSynEditSearchCustom* Value);
	void __fastcall SetSelectionMode(const Synedittypes::TSynSelectionMode Value);
	void __fastcall SetActiveSelectionMode(const Synedittypes::TSynSelectionMode Value);
	void __fastcall SetSelTextExternal(const AnsiString Value);
	void __fastcall SetTabWidth(int Value);
	void __fastcall SynSetText(const AnsiString Value);
	void __fastcall SetTopLine(int Value);
	void __fastcall SetWordWrap(const bool Value);
	void __fastcall SetWordWrapGlyph(const Syneditmiscclasses::TSynGlyph* Value);
	void __fastcall WordWrapGlyphChange(System::TObject* Sender);
	void __fastcall SizeOrFontChanged(bool bFont);
	void __fastcall ProperSetLine(int ALine, const AnsiString ALineText);
	void __fastcall UpdateModifiedStatus(void);
	void __fastcall UndoRedoAdded(System::TObject* Sender);
	void __fastcall UpdateLastCaretX(void);
	void __fastcall UpdateScrollBars(void);
	void __fastcall WriteAddedKeystrokes(Classes::TWriter* Writer);
	void __fastcall WriteRemovedKeystrokes(Classes::TWriter* Writer);
	
protected:
	virtual void __fastcall CreateParams(Controls::TCreateParams &Params);
	virtual void __fastcall CreateWnd(void);
	virtual void __fastcall DestroyWnd(void);
	virtual void __fastcall InvalidateRect(const Windows::TRect &aRect, bool aErase);
	DYNAMIC void __fastcall DblClick(void);
	void __fastcall DecPaintLock(void);
	virtual void __fastcall DefineProperties(Classes::TFiler* Filer);
	virtual void __fastcall DoChange(void);
	DYNAMIC void __fastcall DragCanceled(void);
	DYNAMIC void __fastcall DragOver(System::TObject* Source, int X, int Y, Controls::TDragState State, 
		bool &Accept);
	virtual bool __fastcall GetReadOnly(void);
	void __fastcall HighlighterAttrChanged(System::TObject* Sender);
	void __fastcall IncPaintLock(void);
	void __fastcall InitializeCaret(void);
	DYNAMIC void __fastcall KeyUp(Word &Key, Classes::TShiftState Shift);
	DYNAMIC void __fastcall KeyDown(Word &Key, Classes::TShiftState Shift);
	DYNAMIC void __fastcall KeyPress(char &Key);
	virtual void __fastcall LinesChanged(System::TObject* Sender);
	void __fastcall ListCleared(System::TObject* Sender);
	void __fastcall ListDeleted(System::TObject* Sender, int aIndex, int aCount);
	void __fastcall ListInserted(System::TObject* Sender, int Index, int aCount);
	void __fastcall ListPutted(System::TObject* Sender, int Index, int aCount);
	void __fastcall ChainListCleared(System::TObject* Sender);
	void __fastcall ChainListDeleted(System::TObject* Sender, int aIndex, int aCount);
	void __fastcall ChainListInserted(System::TObject* Sender, int aIndex, int aCount);
	void __fastcall ChainListPutted(System::TObject* Sender, int aIndex, int aCount);
	void __fastcall ChainLinesChanging(System::TObject* Sender);
	void __fastcall ChainLinesChanged(System::TObject* Sender);
	void __fastcall ChainUndoRedoAdded(System::TObject* Sender);
	void __fastcall ScanRanges(void);
	virtual void __fastcall Loaded(void);
	void __fastcall MarkListChange(System::TObject* Sender);
	void __fastcall MBCSGetSelRangeInLineWhenColumnSelectionMode(const AnsiString s, int &ColFrom, int 
		&ColTo);
	DYNAMIC void __fastcall MouseDown(Controls::TMouseButton Button, Classes::TShiftState Shift, int X, 
		int Y);
	DYNAMIC void __fastcall MouseMove(Classes::TShiftState Shift, int X, int Y);
	DYNAMIC void __fastcall MouseUp(Controls::TMouseButton Button, Classes::TShiftState Shift, int X, int 
		Y);
	virtual void __fastcall NotifyHookedCommandHandlers(bool AfterProcessing, Syneditkeycmds::TSynEditorCommand 
		&Command, char &AChar, void * Data);
	virtual void __fastcall Paint(void);
	virtual void __fastcall PaintGutter(const Windows::TRect &AClip, const int aFirstRow, const int aLastRow
		);
	virtual void __fastcall PaintTextLines(const Windows::TRect &AClip, const int aFirstRow, const int 
		aLastRow, const int FirstCol, const int LastCol);
	void __fastcall RecalcCharExtent(void);
	void __fastcall RedoItem(void);
	virtual void __fastcall InternalSetCaretXY(const Synedittypes::TBufferCoord &Value);
	virtual void __fastcall SetCaretXY(const Synedittypes::TBufferCoord &Value);
	virtual void __fastcall SetCaretXYEx(bool CallEnsureCursorPos, const Synedittypes::TBufferCoord &Value
		);
	virtual void __fastcall SetName(const AnsiString Value);
	virtual void __fastcall SetReadOnly(bool Value);
	void __fastcall SetWantReturns(bool Value);
	void __fastcall SetSelTextPrimitive(const AnsiString aValue);
	void __fastcall SetSelTextPrimitiveEx(Synedittypes::TSynSelectionMode PasteMode, char * Value, bool 
		AddToUndoList);
	void __fastcall SetWantTabs(bool Value);
	void __fastcall StatusChanged(TSynStatusChanges AChanges);
	Syneditkeycmds::TSynEditorCommand __fastcall TranslateKeyCode(Word Code, Classes::TShiftState Shift
		, void * &Data);
	void __fastcall UndoItem(void);
	virtual void __fastcall UpdateMouseCursor(void);
	int fGutterWidth;
	Syneditmiscclasses::TSynInternalImage* fInternalImage;
	void __fastcall HideCaret(void);
	void __fastcall ShowCaret(void);
	virtual void __fastcall DoOnClearBookmark(Syneditmiscclasses::TSynEditMark* &Mark);
	virtual void __fastcall DoOnCommandProcessed(Syneditkeycmds::TSynEditorCommand Command, char AChar, 
		void * Data);
	virtual void __fastcall DoOnGutterClick(Controls::TMouseButton Button, int X, int Y);
	virtual void __fastcall DoOnPaint(void);
	virtual void __fastcall DoOnPaintTransientEx(TTransientType TransientType, bool Lock);
	virtual void __fastcall DoOnPaintTransient(TTransientType TransientType);
	virtual void __fastcall DoOnPlaceMark(Syneditmiscclasses::TSynEditMark* &Mark);
	virtual void __fastcall DoOnProcessCommand(Syneditkeycmds::TSynEditorCommand &Command, char &AChar, 
		void * Data);
	virtual TSynReplaceAction __fastcall DoOnReplaceText(const AnsiString ASearch, const AnsiString AReplace
		, int Line, int Column);
	virtual bool __fastcall DoOnSpecialLineColors(int Line, Graphics::TColor &Foreground, Graphics::TColor 
		&Background);
	virtual void __fastcall DoOnStatusChange(TSynStatusChanges Changes);
	int __fastcall GetSelEnd(void);
	int __fastcall GetSelStart(void);
	int __fastcall GetSelLength(void);
	void __fastcall SetSelEnd(const int Value);
	void __fastcall SetSelStart(const int Value);
	void __fastcall SetSelLength(const int Value);
	void __fastcall SetAlwaysShowCaret(const bool Value);
	void __fastcall PrepareIdentChars(Synedittypes::TSynIdentChars &IdentChars, Synedittypes::TSynIdentChars 
		&WhiteChars);
	void __fastcall LinesHookChanged(void);
	__property int InternalCaretX = {write=InternalSetCaretX, nodefault};
	__property int InternalCaretY = {write=InternalSetCaretY, nodefault};
	__property Synedittypes::TBufferCoord InternalCaretXY = {write=InternalSetCaretXY};
	
public:
	__fastcall virtual TCustomSynEdit(Classes::TComponent* AOwner);
	__fastcall virtual ~TCustomSynEdit(void);
	__property Canvas ;
	__property int SelStart = {read=GetSelStart, write=SetSelStart, nodefault};
	__property int SelEnd = {read=GetSelEnd, write=SetSelEnd, nodefault};
	__property bool AlwaysShowCaret = {read=fAlwaysShowCaret, write=SetAlwaysShowCaret, nodefault};
	void __fastcall UpdateCaret(void);
	void __fastcall AddKey(Syneditkeycmds::TSynEditorCommand Command, Word Key1, Classes::TShiftState SS1
		, Word Key2, Classes::TShiftState SS2);
	void __fastcall BeginUndoBlock(void);
	void __fastcall BeginUpdate(void);
	bool __fastcall CaretInView(void);
	Synedittypes::TBufferCoord __fastcall CharIndexToRowCol(int Index);
	void __fastcall Clear(void);
	void __fastcall ClearAll(void);
	void __fastcall ClearBookMark(int BookMark);
	void __fastcall ClearSelection(void);
	virtual void __fastcall CommandProcessor(Syneditkeycmds::TSynEditorCommand Command, char AChar, void * 
		Data);
	void __fastcall ClearUndo(void);
	void __fastcall CopyToClipboard(void);
	void __fastcall CutToClipboard(void);
	void __fastcall DoCopyToClipboard(const AnsiString SText);
	DYNAMIC void __fastcall DragDrop(System::TObject* Source, int X, int Y);
	void __fastcall EndUndoBlock(void);
	void __fastcall EndUpdate(void);
	void __fastcall EnsureCursorPosVisible(void);
	void __fastcall EnsureCursorPosVisibleEx(bool ForceToMiddle);
	virtual void __fastcall FindMatchingBracket(void);
	virtual Synedittypes::TBufferCoord __fastcall GetMatchingBracket(void);
	virtual Synedittypes::TBufferCoord __fastcall GetMatchingBracketEx(const Synedittypes::TBufferCoord 
		&APoint);
	DYNAMIC bool __fastcall ExecuteAction(Classes::TBasicAction* Action);
	virtual void __fastcall ExecuteCommand(Syneditkeycmds::TSynEditorCommand Command, char AChar, void * 
		Data);
	bool __fastcall GetBookMark(int BookMark, int &X, int &Y);
	bool __fastcall GetHighlighterAttriAtRowCol(const Synedittypes::TBufferCoord &XY, AnsiString &Token
		, Synedithighlighter::TSynHighlighterAttributes* &Attri);
	bool __fastcall GetHighlighterAttriAtRowColEx(const Synedittypes::TBufferCoord &XY, AnsiString &Token
		, int &TokenType, int &Start, Synedithighlighter::TSynHighlighterAttributes* &Attri);
	bool __fastcall GetPositionOfMouse(/* out */ Synedittypes::TBufferCoord &aPos);
	AnsiString __fastcall GetWordAtRowCol(const Synedittypes::TBufferCoord &XY);
	void __fastcall GotoBookMark(int BookMark);
	void __fastcall GotoLineAndCenter(int ALine);
	Synedittypes::TSynIdentChars __fastcall IdentChars(void);
	void __fastcall InvalidateGutter(void);
	void __fastcall InvalidateGutterLine(int aLine);
	void __fastcall InvalidateGutterLines(int FirstLine, int LastLine);
	void __fastcall InvalidateLine(int Line);
	void __fastcall InvalidateLines(int FirstLine, int LastLine);
	void __fastcall InvalidateSelection(void);
	bool __fastcall IsBookmark(int BookMark);
	bool __fastcall IsPointInSelection(const Synedittypes::TBufferCoord &Value);
	void __fastcall LockUndo(void);
	Synedittypes::TDisplayCoord __fastcall BufferToDisplayPos(const Synedittypes::TBufferCoord &p);
	Synedittypes::TBufferCoord __fastcall DisplayToBufferPos(const Synedittypes::TDisplayCoord &p);
	int __fastcall LineToRow(int aLine);
	int __fastcall RowToLine(int aRow);
	virtual Synedittypes::TBufferCoord __fastcall NextWordPos(void);
	virtual Synedittypes::TBufferCoord __fastcall NextWordPosEx(const Synedittypes::TBufferCoord &XY);
	virtual void __fastcall Notification(Classes::TComponent* AComponent, Classes::TOperation Operation
		);
	void __fastcall PasteFromClipboard(void);
	virtual Synedittypes::TBufferCoord __fastcall WordStart(void);
	virtual Synedittypes::TBufferCoord __fastcall WordStartEx(const Synedittypes::TBufferCoord &XY);
	virtual Synedittypes::TBufferCoord __fastcall WordEnd(void);
	virtual Synedittypes::TBufferCoord __fastcall WordEndEx(const Synedittypes::TBufferCoord &XY);
	virtual Synedittypes::TBufferCoord __fastcall PrevWordPos(void);
	virtual Synedittypes::TBufferCoord __fastcall PrevWordPosEx(const Synedittypes::TBufferCoord &XY);
	Synedittypes::TDisplayCoord __fastcall PixelsToRowColumn(int aX, int aY);
	Synedittypes::TDisplayCoord __fastcall PixelsToNearestRowColumn(int aX, int aY);
	void __fastcall Redo(void);
	void __fastcall RegisterCommandHandler(const THookedCommandEvent AHandlerProc, void * AHandlerData)
		;
	Windows::TPoint __fastcall RowColumnToPixels(const Synedittypes::TDisplayCoord &RowCol);
	int __fastcall RowColToCharIndex(const Synedittypes::TBufferCoord &RowCol);
	int __fastcall SearchReplace(const AnsiString ASearch, const AnsiString AReplace, Synedittypes::TSynSearchOptions 
		AOptions);
	void __fastcall SelectAll(void);
	void __fastcall SetBookMark(int BookMark, int X, int Y);
	void __fastcall SetCaretAndSelection(const Synedittypes::TBufferCoord &ptCaret, const Synedittypes::TBufferCoord 
		&ptBefore, const Synedittypes::TBufferCoord &ptAfter);
	virtual void __fastcall SetDefaultKeystrokes(void);
	void __fastcall SetSelWord(void);
	void __fastcall SetWordBlock(const Synedittypes::TBufferCoord &Value);
	void __fastcall Undo(void);
	void __fastcall UnlockUndo(void);
	void __fastcall UnregisterCommandHandler(THookedCommandEvent AHandlerProc);
	DYNAMIC bool __fastcall UpdateAction(Classes::TBasicAction* Action);
	virtual void __fastcall SetFocus(void);
	void __fastcall AddKeyUpHandler(Controls::TKeyEvent aHandler);
	void __fastcall RemoveKeyUpHandler(Controls::TKeyEvent aHandler);
	void __fastcall AddKeyDownHandler(Controls::TKeyEvent aHandler);
	void __fastcall RemoveKeyDownHandler(Controls::TKeyEvent aHandler);
	void __fastcall AddKeyPressHandler(Controls::TKeyPressEvent aHandler);
	void __fastcall RemoveKeyPressHandler(Controls::TKeyPressEvent aHandler);
	void __fastcall AddFocusControl(Controls::TWinControl* aControl);
	void __fastcall RemoveFocusControl(Controls::TWinControl* aControl);
	void __fastcall AddMouseDownHandler(Controls::TMouseEvent aHandler);
	void __fastcall RemoveMouseDownHandler(Controls::TMouseEvent aHandler);
	void __fastcall AddMouseUpHandler(Controls::TMouseEvent aHandler);
	void __fastcall RemoveMouseUpHandler(Controls::TMouseEvent aHandler);
	void __fastcall AddMouseCursorHandler(TMouseCursorEvent aHandler);
	void __fastcall RemoveMouseCursorHandler(TMouseCursorEvent aHandler);
	virtual void __fastcall WndProc(Messages::TMessage &Msg);
	void __fastcall SetLinesPointer(TCustomSynEdit* ASynEdit);
	void __fastcall RemoveLinesPointer(void);
	void __fastcall HookTextBuffer(Synedittextbuffer::TSynEditStringList* aBuffer, Synedittextbuffer::TSynEditUndoList* 
		aUndo, Synedittextbuffer::TSynEditUndoList* aRedo);
	void __fastcall UnHookTextBuffer(void);
	__property Synedittypes::TBufferCoord BlockBegin = {read=GetBlockBegin, write=SetBlockBegin};
	__property Synedittypes::TBufferCoord BlockEnd = {read=GetBlockEnd, write=SetBlockEnd};
	__property bool CanPaste = {read=GetCanPaste, nodefault};
	__property bool CanRedo = {read=GetCanRedo, nodefault};
	__property bool CanUndo = {read=GetCanUndo, nodefault};
	__property int CaretX = {read=fCaretX, write=SetCaretX, nodefault};
	__property int CaretY = {read=fCaretY, write=SetCaretY, nodefault};
	__property Synedittypes::TBufferCoord CaretXY = {read=GetCaretXY, write=SetCaretXY};
	__property Graphics::TColor ActiveLineColor = {read=fActiveLineColor, write=SetActiveLineColor, default=536870911
		};
	__property int DisplayX = {read=GetDisplayX, nodefault};
	__property int DisplayY = {read=GetDisplayY, nodefault};
	__property Synedittypes::TDisplayCoord DisplayXY = {read=GetDisplayXY};
	__property int DisplayLineCount = {read=GetDisplayLineCount, nodefault};
	__property int CharsInWindow = {read=fCharsInWindow, nodefault};
	__property int CharWidth = {read=fCharWidth, nodefault};
	__property Color ;
	__property Graphics::TFont* Font = {read=GetFont, write=SetFont};
	__property int GutterWidth = {read=fGutterWidth, nodefault};
	__property Synedithighlighter::TSynCustomHighlighter* Highlighter = {read=fHighlighter, write=SetHighlighter
		};
	__property int LeftChar = {read=fLeftChar, write=SetLeftChar, nodefault};
	__property int LineHeight = {read=fTextHeight, nodefault};
	__property int LinesInWindow = {read=fLinesInWindow, nodefault};
	__property AnsiString LineText = {read=GetLineText, write=SetLineText};
	__property Classes::TStrings* Lines = {read=fLines, write=SetLines};
	__property Syneditmiscclasses::TSynEditMarkList* Marks = {read=fMarkList};
	__property int MaxScrollWidth = {read=fMaxScrollWidth, write=SetMaxScrollWidth, default=1024};
	__property bool Modified = {read=fModified, write=SetModified, nodefault};
	__property int PaintLock = {read=fPaintLock, nodefault};
	__property bool ReadOnly = {read=GetReadOnly, write=SetReadOnly, default=0};
	__property Syneditmiscclasses::TSynEditSearchCustom* SearchEngine = {read=fSearchEngine, write=SetSearchEngine
		};
	__property bool SelAvail = {read=GetSelAvail, nodefault};
	__property int SelLength = {read=GetSelLength, write=SetSelLength, nodefault};
	__property bool SelTabBlock = {read=GetSelTabBlock, nodefault};
	__property bool SelTabLine = {read=GetSelTabLine, nodefault};
	__property AnsiString SelText = {read=GetSelText, write=SetSelTextExternal};
	__property TSynStateFlags StateFlags = {read=fStateFlags, nodefault};
	__property AnsiString Text = {read=SynGetText, write=SynSetText};
	__property int TopLine = {read=fTopLine, write=SetTopLine, nodefault};
	__property AnsiString WordAtCursor = {read=GetWordAtCursor};
	__property AnsiString WordAtMouse = {read=GetWordAtMouse};
	__property Synedittextbuffer::TSynEditUndoList* UndoList = {read=fUndoList};
	__property Synedittextbuffer::TSynEditUndoList* RedoList = {read=fRedoList};
	__property TProcessCommandEvent OnProcessCommand = {read=fOnProcessCommand, write=fOnProcessCommand
		};
	__property Syneditmiscclasses::TSynBookMarkOpt* BookMarkOptions = {read=fBookMarkOpt, write=fBookMarkOpt
		};
	__property Forms::TBorderStyle BorderStyle = {read=fBorderStyle, write=SetBorderStyle, default=1};
	__property int ExtraLineSpacing = {read=fExtraLineSpacing, write=SetExtraLineSpacing, default=0};
	__property Syneditmiscclasses::TSynGutter* Gutter = {read=fGutter, write=SetGutter};
	__property bool HideSelection = {read=fHideSelection, write=SetHideSelection, default=0};
	__property TSynEditCaretType InsertCaret = {read=fInsertCaret, write=SetInsertCaret, default=0};
	__property bool InsertMode = {read=fInserting, write=SetInsertMode, default=1};
	__property bool IsScrolling = {read=FIsScrolling, nodefault};
	__property Syneditkeycmds::TSynEditKeyStrokes* Keystrokes = {read=fKeyStrokes, write=SetKeystrokes, 
		stored=false};
	__property int MaxUndo = {read=GetMaxUndo, write=SetMaxUndo, default=1024};
	__property TSynEditorOptions Options = {read=GetOptions, write=SetOptions, default=40632722};
	__property TSynEditCaretType OverwriteCaret = {read=fOverwriteCaret, write=SetOverwriteCaret, default=3
		};
	__property int RightEdge = {read=fRightEdge, write=SetRightEdge, default=80};
	__property Graphics::TColor RightEdgeColor = {read=fRightEdgeColor, write=SetRightEdgeColor, default=12632256
		};
	__property Graphics::TColor ScrollHintColor = {read=fScrollHintColor, write=fScrollHintColor, default=-2147483624
		};
	__property TScrollHintFormat ScrollHintFormat = {read=fScrollHintFormat, write=fScrollHintFormat, default=0
		};
	__property Stdctrls::TScrollStyle ScrollBars = {read=FScrollBars, write=SetScrollBars, default=3};
	__property Syneditmiscclasses::TSynSelectedColor* SelectedColor = {read=fSelectedColor, write=fSelectedColor
		};
	__property Synedittypes::TSynSelectionMode SelectionMode = {read=fSelectionMode, write=SetSelectionMode
		, default=0};
	__property Synedittypes::TSynSelectionMode ActiveSelectionMode = {read=fActiveSelectionMode, write=
		SetActiveSelectionMode, stored=false, nodefault};
	__property int TabWidth = {read=fTabWidth, write=SetTabWidth, default=8};
	__property bool WantReturns = {read=fWantReturns, write=SetWantReturns, default=1};
	__property bool WantTabs = {read=fWantTabs, write=SetWantTabs, default=0};
	__property bool WordWrap = {read=GetWordWrap, write=SetWordWrap, default=0};
	__property Syneditmiscclasses::TSynGlyph* WordWrapGlyph = {read=fWordWrapGlyph, write=SetWordWrapGlyph
		};
	__property Classes::TNotifyEvent OnChange = {read=fOnChange, write=fOnChange};
	__property TPlaceMarkEvent OnClearBookmark = {read=fOnClearMark, write=fOnClearMark};
	__property TProcessCommandEvent OnCommandProcessed = {read=fOnCommandProcessed, write=fOnCommandProcessed
		};
	__property TContextHelpEvent OnContextHelp = {read=fOnContextHelp, write=fOnContextHelp};
	__property TDropFilesEvent OnDropFiles = {read=fOnDropFiles, write=fOnDropFiles};
	__property TGutterClickEvent OnGutterClick = {read=fOnGutterClick, write=fOnGutterClick};
	__property TGutterGetTextEvent OnGutterGetText = {read=fOnGutterGetText, write=fOnGutterGetText};
	__property TGutterPaintEvent OnGutterPaint = {read=fOnGutterPaint, write=fOnGutterPaint};
	__property TMouseCursorEvent OnMouseCursor = {read=fOnMouseCursor, write=fOnMouseCursor};
	__property TPaintEvent OnPaint = {read=fOnPaint, write=fOnPaint};
	__property TPlaceMarkEvent OnPlaceBookmark = {read=fOnPlaceMark, write=fOnPlaceMark};
	__property TProcessCommandEvent OnProcessUserCommand = {read=fOnProcessUserCommand, write=fOnProcessUserCommand
		};
	__property TReplaceTextEvent OnReplaceText = {read=fOnReplaceText, write=fOnReplaceText};
	__property TSpecialLineColorsEvent OnSpecialLineColors = {read=fOnSpecialLineColors, write=fOnSpecialLineColors
		};
	__property TStatusChangeEvent OnStatusChange = {read=fOnStatusChange, write=fOnStatusChange};
	__property TPaintTransient OnPaintTransient = {read=fOnPaintTransient, write=fOnPaintTransient};
	__property TScrollEvent OnScroll = {read=fOnScroll, write=fOnScroll};
	
__published:
	__property Cursor ;
public:
	#pragma option push -w-inl
	/* TWinControl.CreateParented */ inline __fastcall TCustomSynEdit(HWND ParentWindow) : Controls::TCustomControl(
		ParentWindow) { }
	#pragma option pop
	
};


class PASCALIMPLEMENTATION TSynEditPlugin : public System::TObject 
{
	typedef System::TObject inherited;
	
private:
	TCustomSynEdit* fOwner;
	
protected:
	virtual void __fastcall AfterPaint(Graphics::TCanvas* ACanvas, const Windows::TRect &AClip, int FirstLine
		, int LastLine) = 0 ;
	virtual void __fastcall LinesInserted(int FirstLine, int Count) = 0 ;
	virtual void __fastcall LinesDeleted(int FirstLine, int Count) = 0 ;
	__property TCustomSynEdit* Editor = {read=fOwner};
	
public:
	__fastcall TSynEditPlugin(TCustomSynEdit* AOwner);
	__fastcall virtual ~TSynEditPlugin(void);
};


class DELPHICLASS TSynEdit;
class PASCALIMPLEMENTATION TSynEdit : public TCustomSynEdit 
{
	typedef TCustomSynEdit inherited;
	
__published:
	__property Align ;
	__property Anchors ;
	__property Constraints ;
	__property Color ;
	__property ActiveLineColor ;
	__property Ctl3D ;
	__property ParentCtl3D ;
	__property Enabled ;
	__property Font ;
	__property Height ;
	__property Name ;
	__property ParentColor ;
	__property ParentFont ;
	__property ParentShowHint ;
	__property PopupMenu ;
	__property ShowHint ;
	__property TabOrder ;
	__property TabStop ;
	__property Visible ;
	__property Width ;
	__property OnClick ;
	__property OnDblClick ;
	__property OnDragDrop ;
	__property OnDragOver ;
	__property OnEndDock ;
	__property OnStartDock ;
	__property OnEndDrag ;
	__property OnEnter ;
	__property OnExit ;
	__property OnKeyDown ;
	__property OnKeyPress ;
	__property OnKeyUp ;
	__property OnMouseDown ;
	__property OnMouseMove ;
	__property OnMouseUp ;
	__property OnStartDrag ;
	__property BookMarkOptions ;
	__property BorderStyle ;
	__property ExtraLineSpacing ;
	__property Gutter ;
	__property HideSelection ;
	__property Highlighter ;
	__property InsertCaret ;
	__property InsertMode ;
	__property Keystrokes ;
	__property Lines ;
	__property MaxScrollWidth ;
	__property MaxUndo ;
	__property Options ;
	__property OverwriteCaret ;
	__property ReadOnly ;
	__property RightEdge ;
	__property RightEdgeColor ;
	__property ScrollHintColor ;
	__property ScrollHintFormat ;
	__property ScrollBars ;
	__property SearchEngine ;
	__property SelectedColor ;
	__property SelectionMode ;
	__property TabWidth ;
	__property WantReturns ;
	__property WantTabs ;
	__property WordWrap ;
	__property WordWrapGlyph ;
	__property OnChange ;
	__property OnClearBookmark ;
	__property OnCommandProcessed ;
	__property OnContextHelp ;
	__property OnDropFiles ;
	__property OnGutterClick ;
	__property OnGutterGetText ;
	__property OnGutterPaint ;
	__property OnMouseCursor ;
	__property OnPaint ;
	__property OnPlaceBookmark ;
	__property OnProcessCommand ;
	__property OnProcessUserCommand ;
	__property OnReplaceText ;
	__property OnScroll ;
	__property OnSpecialLineColors ;
	__property OnStatusChange ;
	__property OnPaintTransient ;
public:
	#pragma option push -w-inl
	/* TCustomSynEdit.Create */ inline __fastcall virtual TSynEdit(Classes::TComponent* AOwner) : TCustomSynEdit(
		AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TCustomSynEdit.Destroy */ inline __fastcall virtual ~TSynEdit(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TWinControl.CreateParented */ inline __fastcall TSynEdit(HWND ParentWindow) : TCustomSynEdit(ParentWindow
		) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------
static const Word MAX_SCROLL = 0x7fff;
static const Shortint MAX_MARKS = 0x10;
#define SYNEDIT_CLIPBOARD_FORMAT "SynEdit Control Block Type"
extern PACKAGE unsigned SynEditClipboardFormat;
#define SYNEDIT_DEFAULT_OPTIONS (System::Set<TSynEditorOption, eoAltSetsColumnMode, eoTrimTrailingSpaces> () \
	)

}	/* namespace Synedit */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Synedit;
#endif
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// SynEdit
