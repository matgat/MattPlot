// Borland C++ Builder
// Copyright (c) 1995, 1999 by Borland International
// All rights reserved

// (DO NOT EDIT: machine generated header) 'SynEditSearch.pas' rev: 5.00

#ifndef SynEditSearchHPP
#define SynEditSearchHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <Classes.hpp>	// Pascal unit
#include <SynEditMiscClasses.hpp>	// Pascal unit
#include <SynEditTypes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Syneditsearch
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TSynEditSearch;
class PASCALIMPLEMENTATION TSynEditSearch : public Syneditmiscclasses::TSynEditSearchCustom 
{
	typedef Syneditmiscclasses::TSynEditSearchCustom inherited;
	
private:
	bool CompTableSensitive;
	Byte CompTable[256];
	bool DelimTable[256];
	char *Run;
	char *Origin;
	char *TheEnd;
	AnsiString Pat;
	int fCount;
	int fTextLen;
	int Look_At;
	int PatLen;
	int PatLenPlus;
	int Shift[256];
	bool fSensitive;
	bool fWhole;
	Classes::TList* fResults;
	bool fShiftInitialized;
	bool __fastcall GetFinished(void);
	void __fastcall InitShiftTable(void);
	void __fastcall SetSensitive(const bool Value);
	void __fastcall MakeCompTable(bool Sensitive);
	void __fastcall MakeDelimiterTable(void);
	
protected:
	bool __fastcall TestWholeWord(void);
	virtual void __fastcall SetPattern(const AnsiString Value);
	virtual AnsiString __fastcall GetPattern(void);
	virtual int __fastcall GetLength(int aIndex);
	virtual int __fastcall GetResult(int Index);
	virtual int __fastcall GetResultCount(void);
	virtual void __fastcall SetOptions(const Synedittypes::TSynSearchOptions Value);
	
public:
	__fastcall virtual TSynEditSearch(Classes::TComponent* aOwner);
	__fastcall virtual ~TSynEditSearch(void);
	virtual int __fastcall FindAll(const AnsiString NewText);
	virtual AnsiString __fastcall Replace(const AnsiString aOccurrence, const AnsiString aReplacement);
		
	int __fastcall FindFirst(const AnsiString NewText);
	void __fastcall FixResults(int First, int Delta);
	int __fastcall Next(void);
	__property int Count = {read=fCount, write=fCount, nodefault};
	__property bool Finished = {read=GetFinished, nodefault};
	__property Pattern  = {read=Pat};
	__property bool Sensitive = {read=fSensitive, write=SetSensitive, nodefault};
	__property bool Whole = {read=fWhole, write=fWhole, nodefault};
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Syneditsearch */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Syneditsearch;
#endif
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// SynEditSearch
