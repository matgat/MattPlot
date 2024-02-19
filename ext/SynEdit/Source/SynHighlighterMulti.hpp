// Borland C++ Builder
// Copyright (c) 1995, 1999 by Borland International
// All rights reserved

// (DO NOT EDIT: machine generated header) 'SynHighlighterMulti.pas' rev: 5.00

#ifndef SynHighlighterMultiHPP
#define SynHighlighterMultiHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <Classes.hpp>	// Pascal unit
#include <SynEditHighlighter.hpp>	// Pascal unit
#include <SynEditTypes.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Synhighlightermulti
{
//-- type declarations -------------------------------------------------------
typedef void __fastcall (__closure *TOnCheckMarker)(System::TObject* Sender, int &StartPos, int &MarkerLen
	, AnsiString &MarkerText, int Line);

class DELPHICLASS TScheme;
class PASCALIMPLEMENTATION TScheme : public Classes::TCollectionItem 
{
	typedef Classes::TCollectionItem inherited;
	
private:
	AnsiString fEndExpr;
	AnsiString fStartExpr;
	Synedithighlighter::TSynCustomHighlighter* fHighlighter;
	Synedithighlighter::TSynHighlighterAttributes* fMarkerAttri;
	AnsiString fSchemeName;
	bool fCaseSensitive;
	TOnCheckMarker fOnCheckStartMarker;
	TOnCheckMarker fOnCheckEndMarker;
	AnsiString __fastcall ConvertExpression(const AnsiString Value);
	void __fastcall MarkerAttriChanged(System::TObject* Sender);
	void __fastcall SetMarkerAttri(const Synedithighlighter::TSynHighlighterAttributes* Value);
	void __fastcall SetHighlighter(const Synedithighlighter::TSynCustomHighlighter* Value);
	void __fastcall SetEndExpr(const AnsiString Value);
	void __fastcall SetStartExpr(const AnsiString Value);
	void __fastcall SetCaseSensitive(const bool Value);
	
protected:
	virtual AnsiString __fastcall GetDisplayName(void);
	virtual void __fastcall SetDisplayName(const AnsiString Value);
	
public:
	__fastcall virtual TScheme(Classes::TCollection* Collection);
	__fastcall virtual ~TScheme(void);
	
__published:
	__property bool CaseSensitive = {read=fCaseSensitive, write=SetCaseSensitive, default=1};
	__property AnsiString StartExpr = {read=fStartExpr, write=SetStartExpr};
	__property AnsiString EndExpr = {read=fEndExpr, write=SetEndExpr};
	__property Synedithighlighter::TSynCustomHighlighter* Highlighter = {read=fHighlighter, write=SetHighlighter
		};
	__property Synedithighlighter::TSynHighlighterAttributes* MarkerAttri = {read=fMarkerAttri, write=SetMarkerAttri
		};
	__property AnsiString SchemeName = {read=fSchemeName, write=fSchemeName};
	__property TOnCheckMarker OnCheckStartMarker = {read=fOnCheckStartMarker, write=fOnCheckStartMarker
		};
	__property TOnCheckMarker OnCheckEndMarker = {read=fOnCheckEndMarker, write=fOnCheckEndMarker};
};


typedef TMetaClass*TgmSchemeClass;

class DELPHICLASS TSchemes;
class DELPHICLASS TSynMultiSyn;
#pragma option push -b-
enum TRangeOperation { roGet, roSet };
#pragma option pop

typedef void __fastcall (__closure *TRangeProc)(TRangeOperation Operation, unsigned &Range);

class DELPHICLASS TMarker;
class PASCALIMPLEMENTATION TMarker : public System::TObject 
{
	typedef System::TObject inherited;
	
protected:
	int fScheme;
	int fStartPos;
	int fMarkerLen;
	AnsiString fMarkerText;
	bool fIsOpenMarker;
	
public:
	__fastcall TMarker(int aScheme, int aStartPos, int aMarkerLen, bool aIsOpenMarker, const AnsiString 
		aMarkerText);
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TMarker(void) { }
	#pragma option pop
	
};


typedef void __fastcall (__closure *TCustomRangeEvent)(TSynMultiSyn* Sender, TRangeOperation Operation
	, void * &Range);

class PASCALIMPLEMENTATION TSynMultiSyn : public Synedithighlighter::TSynCustomHighlighter 
{
	typedef Synedithighlighter::TSynCustomHighlighter inherited;
	
private:
	TRangeProc fRangeProc;
	AnsiString fDefaultLanguageName;
	Classes::TList* fMarkers;
	TMarker* fMarker;
	int fNextMarker;
	int fCurrScheme;
	AnsiString fTmpLine;
	void *fTmpRange;
	TCustomRangeEvent fOnCustomRange;
	void __fastcall SetDefaultHighlighter(const Synedithighlighter::TSynCustomHighlighter* Value);
	TMarker* __fastcall GetMarkers(int aIndex);
	__property TMarker* Markers[int aIndex] = {read=GetMarkers};
	void __fastcall DoCheckMarker(TScheme* Scheme, int StartPos, int MarkerLen, const AnsiString MarkerText
		, bool Start, int Line);
	void __fastcall SetOnCustomRange(const TCustomRangeEvent Value);
	
protected:
	TSchemes* fSchemes;
	Synedithighlighter::TSynCustomHighlighter* fDefaultHighlighter;
	AnsiString fLine;
	int fLineNumber;
	int fTokenPos;
	int fRun;
	AnsiString fSampleSource;
	virtual void __fastcall Loaded(void);
	void __fastcall SetSchemes(const TSchemes* Value);
	void __fastcall ClearMarkers(void);
	virtual Synedittypes::TSynIdentChars __fastcall GetIdentChars(void);
	virtual Synedithighlighter::TSynHighlighterAttributes* __fastcall GetDefaultAttribute(int Index);
	virtual int __fastcall GetAttribCount(void);
	virtual Synedithighlighter::TSynHighlighterAttributes* __fastcall GetAttribute(int idx);
	void __fastcall HookHighlighter(Synedithighlighter::TSynCustomHighlighter* aHL);
	void __fastcall UnhookHighlighter(Synedithighlighter::TSynCustomHighlighter* aHL);
	virtual void __fastcall Notification(Classes::TComponent* aComp, Classes::TOperation aOp);
	virtual AnsiString __fastcall GetSampleSource(void);
	virtual void __fastcall SetSampleSource(AnsiString Value);
	void __fastcall OldRangeProc(TRangeOperation Operation, unsigned &Range);
	void __fastcall NewRangeProc(TRangeOperation Operation, unsigned &Range);
	void __fastcall UserRangeProc(TRangeOperation Operation, unsigned &Range);
	
public:
	#pragma option push -w-inl
	/* virtual class method */ virtual AnsiString __fastcall GetLanguageName() { return GetLanguageName(__classid(TSynMultiSyn)
		); }
	#pragma option pop
	/*         class method */ static AnsiString __fastcall GetLanguageName(TMetaClass* vmt);
	__fastcall virtual TSynMultiSyn(Classes::TComponent* AOwner);
	__fastcall virtual ~TSynMultiSyn(void);
	virtual bool __fastcall GetEol(void);
	virtual void * __fastcall GetRange(void);
	virtual AnsiString __fastcall GetToken(void);
	virtual Synedithighlighter::TSynHighlighterAttributes* __fastcall GetTokenAttribute(void);
	virtual int __fastcall GetTokenKind(void);
	virtual int __fastcall GetTokenPos(void);
	virtual void __fastcall Next(void);
	virtual void __fastcall SetLine(AnsiString NewValue, int LineNumber);
	virtual void __fastcall SetRange(void * Value);
	virtual void __fastcall ResetRange(void);
	bool __fastcall UpdateRangeProcs(void);
	__property int CurrScheme = {read=fCurrScheme, write=fCurrScheme, nodefault};
	__property AnsiString CurrLine = {read=fLine};
	virtual bool __fastcall LoadFromRegistry(HKEY RootKey, AnsiString Key);
	virtual bool __fastcall SaveToRegistry(HKEY RootKey, AnsiString Key);
	
__published:
	__property TSchemes* Schemes = {read=fSchemes, write=SetSchemes};
	__property Synedithighlighter::TSynCustomHighlighter* DefaultHighlighter = {read=fDefaultHighlighter
		, write=SetDefaultHighlighter};
	__property AnsiString DefaultLanguageName = {read=fDefaultLanguageName, write=fDefaultLanguageName}
		;
	__property TCustomRangeEvent OnCustomRange = {read=fOnCustomRange, write=SetOnCustomRange};
};


class PASCALIMPLEMENTATION TSchemes : public Classes::TCollection 
{
	typedef Classes::TCollection inherited;
	
private:
	TSynMultiSyn* fOwner;
	TScheme* __fastcall GetItems(int Index);
	void __fastcall SetItems(int Index, const TScheme* Value);
	
protected:
	DYNAMIC Classes::TPersistent* __fastcall GetOwner(void);
	virtual void __fastcall Update(Classes::TCollectionItem* Item);
	
public:
	__fastcall TSchemes(TSynMultiSyn* aOwner);
	__property TScheme* Items[int aIndex] = {read=GetItems, write=SetItems/*, default*/};
public:
	#pragma option push -w-inl
	/* TCollection.Destroy */ inline __fastcall virtual ~TSchemes(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Synhighlightermulti */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Synhighlightermulti;
#endif
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// SynHighlighterMulti
