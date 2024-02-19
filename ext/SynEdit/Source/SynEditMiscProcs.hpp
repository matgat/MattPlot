// Borland C++ Builder
// Copyright (c) 1995, 1999 by Borland International
// All rights reserved

// (DO NOT EDIT: machine generated header) 'SynEditMiscProcs.pas' rev: 5.00

#ifndef SynEditMiscProcsHPP
#define SynEditMiscProcsHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <Classes.hpp>	// Pascal unit
#include <Math.hpp>	// Pascal unit
#include <SynEditHighlighter.hpp>	// Pascal unit
#include <SynEditTypes.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Syneditmiscprocs
{
//-- type declarations -------------------------------------------------------
typedef int TIntArray[134217727];

typedef int *PIntArray;

typedef AnsiString __fastcall (*TConvertTabsProc)(const AnsiString Line, int TabWidth);

typedef AnsiString __fastcall (*TConvertTabsProcEx)(const AnsiString Line, int TabWidth, bool &HasTabs
	);

#pragma option push -b-
enum TStringType { stNone, stHalfNumAlpha, stHalfSymbol, stHalfKatakana, stWideNumAlpha, stWideSymbol, 
	stWideKatakana, stHiragana, stIdeograph, stControl, stKashida };
#pragma option pop

typedef bool __fastcall (__closure *THighlighterAttriProc)(Synedithighlighter::TSynCustomHighlighter* 
	Highlighter, Synedithighlighter::TSynHighlighterAttributes* Attri, AnsiString UniqueAttriName, const 
	void * * Params, const int Params_Size);

//-- var, const, procedure ---------------------------------------------------
extern PACKAGE int __fastcall MinMax(int x, int mi, int ma);
extern PACKAGE void __fastcall SwapInt(int &l, int &r);
extern PACKAGE Windows::TPoint __fastcall MaxPoint(const Windows::TPoint &P1, const Windows::TPoint 
	&P2);
extern PACKAGE Windows::TPoint __fastcall MinPoint(const Windows::TPoint &P1, const Windows::TPoint 
	&P2);
extern PACKAGE PIntArray __fastcall GetIntArray(unsigned Count, int InitialValue);
extern PACKAGE void __fastcall InternalFillRect(HDC dc, const Windows::TRect &rcPaint);
extern PACKAGE AnsiString __fastcall ConvertTabsEx(const AnsiString Line, int TabWidth, bool &HasTabs
	);
extern PACKAGE AnsiString __fastcall ConvertTabs(const AnsiString Line, int TabWidth);
extern PACKAGE TConvertTabsProc __fastcall GetBestConvertTabsProc(int TabWidth);
extern PACKAGE TConvertTabsProcEx __fastcall GetBestConvertTabsProcEx(int TabWidth);
extern PACKAGE int __fastcall GetExpandedLength(const AnsiString aStr, int aTabWidth);
extern PACKAGE int __fastcall CharIndex2CaretPos(int Index, int TabWidth, const AnsiString Line);
extern PACKAGE int __fastcall CaretPos2CharIndex(int Position, int TabWidth, const AnsiString Line, 
	bool &InsideTabChar);
extern PACKAGE int __fastcall StrScanForCharInSet(const AnsiString Line, int Start, const Synedittypes::TSynIdentChars 
	&AChars);
extern PACKAGE int __fastcall StrRScanForCharInSet(const AnsiString Line, int Start, const Synedittypes::TSynIdentChars 
	&AChars);
extern PACKAGE int __fastcall StrScanForMultiByteChar(const AnsiString Line, int Start);
extern PACKAGE int __fastcall StrRScanForMultiByteChar(const AnsiString Line, int Start);
extern PACKAGE char * __fastcall GetEOL(char * Line);
extern PACKAGE AnsiString __fastcall EncodeString(AnsiString s);
extern PACKAGE AnsiString __fastcall DecodeString(AnsiString s);
extern PACKAGE TStringType __fastcall IsStringType(Word Value);
extern PACKAGE bool __fastcall EnumHighlighterAttris(Synedithighlighter::TSynCustomHighlighter* Highlighter
	, bool SkipDuplicates, THighlighterAttriProc HighlighterAttriProc, const void * * Params, const int 
	Params_Size);
extern PACKAGE void __fastcall SynDrawGradient(const Graphics::TCanvas* ACanvas, const Graphics::TColor 
	AStartColor, const Graphics::TColor AEndColor, int ASteps, const Windows::TRect &ARect, const bool 
	AHorizontal);

}	/* namespace Syneditmiscprocs */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Syneditmiscprocs;
#endif
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// SynEditMiscProcs
