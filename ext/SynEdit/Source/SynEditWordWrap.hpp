// Borland C++ Builder
// Copyright (c) 1995, 1999 by Borland International
// All rights reserved

// (DO NOT EDIT: machine generated header) 'SynEditWordWrap.pas' rev: 5.00

#ifndef SynEditWordWrapHPP
#define SynEditWordWrapHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <Classes.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <SynEdit.hpp>	// Pascal unit
#include <SynEditTextBuffer.hpp>	// Pascal unit
#include <SynEditTypes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Syneditwordwrap
{
//-- type declarations -------------------------------------------------------
typedef int TLineIndex;

typedef int TRowIndex;

typedef Byte TRowLength;

typedef TRowIndex TRowIndexArray[134217728];

typedef TRowIndex *PRowIndexArray;

typedef Byte TRowLengthArray[134217728];

typedef Byte *PRowLengthArray;

class DELPHICLASS TSynWordWrapPlugin;
class PASCALIMPLEMENTATION TSynWordWrapPlugin : public System::TInterfacedObject 
{
	typedef System::TInterfacedObject inherited;
	
private:
	TRowIndex *fLineOffsets;
	Byte *fRowLengths;
	int fLineCapacity;
	int fRowCapacity;
	int fLineCount;
	Synedit::TCustomSynEdit* fEditor;
	Byte fMinRowLength;
	Byte fMaxRowLength;
	Sysutils::TSysCharSet fBreakChars;
	void __fastcall GrowLines(int aMinSize);
	void __fastcall MoveLines(TLineIndex aStart, int aMoveBy);
	void __fastcall GrowRows(int aMinSize);
	void __fastcall MoveRows(TRowIndex aStart, int aMoveBy);
	void __fastcall SetEmpty(void);
	
protected:
	void __fastcall WrapLines(void);
	int __fastcall ReWrapLine(TLineIndex aIndex);
	void __fastcall TrimArrays(void);
	__property PRowIndexArray LineOffsets = {read=fLineOffsets};
	__property PRowLengthArray RowLengths = {read=fRowLengths};
	__property Synedit::TCustomSynEdit* Editor = {read=fEditor};
	
public:
	__fastcall TSynWordWrapPlugin(Synedit::TCustomSynEdit* aOwner);
	__fastcall virtual ~TSynWordWrapPlugin(void);
	__property int LineCount = {read=fLineCount, nodefault};
	Synedittypes::TDisplayCoord __fastcall BufferToDisplayPos(const Synedittypes::TBufferCoord &aPos);
	Synedittypes::TBufferCoord __fastcall DisplayToBufferPos(const Synedittypes::TDisplayCoord &aPos);
	int __fastcall RowCount(void);
	int __fastcall GetRowLength(int aRow);
	int __fastcall LinesInserted(int aIndex, int aCount);
	int __fastcall LinesDeleted(int aIndex, int aCount);
	int __fastcall LinesPutted(int aIndex, int aCount);
	void __fastcall Reset(void);
	void __fastcall DisplayChanged(void);
private:
	void *__ISynEditBufferPlugin;	/* Synedit::ISynEditBufferPlugin */
	
public:
	operator ISynEditBufferPlugin*(void) { return (ISynEditBufferPlugin*)&__ISynEditBufferPlugin; }
	
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE bool OldWhitespaceBehaviour;

}	/* namespace Syneditwordwrap */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Syneditwordwrap;
#endif
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// SynEditWordWrap
