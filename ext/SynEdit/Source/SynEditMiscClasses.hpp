// Borland C++ Builder
// Copyright (c) 1995, 1999 by Borland International
// All rights reserved

// (DO NOT EDIT: machine generated header) 'SynEditMiscClasses.pas' rev: 5.00

#ifndef SynEditMiscClassesHPP
#define SynEditMiscClassesHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <SysUtils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Math.hpp>	// Pascal unit
#include <SynEditKeyConst.hpp>	// Pascal unit
#include <SynEditTypes.hpp>	// Pascal unit
#include <Registry.hpp>	// Pascal unit
#include <Menus.hpp>	// Pascal unit
#include <StdCtrls.hpp>	// Pascal unit
#include <Forms.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Messages.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <Consts.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Syneditmiscclasses
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TSynSelectedColor;
class PASCALIMPLEMENTATION TSynSelectedColor : public Classes::TPersistent 
{
	typedef Classes::TPersistent inherited;
	
private:
	Graphics::TColor fBG;
	Graphics::TColor fFG;
	Classes::TNotifyEvent fOnChange;
	void __fastcall SetBG(Graphics::TColor Value);
	void __fastcall SetFG(Graphics::TColor Value);
	
public:
	__fastcall TSynSelectedColor(void);
	virtual void __fastcall Assign(Classes::TPersistent* Source);
	
__published:
	__property Graphics::TColor Background = {read=fBG, write=SetBG, default=-2147483635};
	__property Graphics::TColor Foreground = {read=fFG, write=SetFG, default=-2147483634};
	__property Classes::TNotifyEvent OnChange = {read=fOnChange, write=fOnChange};
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TSynSelectedColor(void) { }
	#pragma option pop
	
};


#pragma option push -b-
enum TSynGutterBorderStyle { gbsNone, gbsMiddle, gbsRight };
#pragma option pop

class DELPHICLASS TSynGutter;
class PASCALIMPLEMENTATION TSynGutter : public Classes::TPersistent 
{
	typedef Classes::TPersistent inherited;
	
private:
	Graphics::TFont* fFont;
	Graphics::TColor fColor;
	Graphics::TColor fBorderColor;
	int fWidth;
	bool fShowLineNumbers;
	int fDigitCount;
	bool fLeadingZeros;
	bool fZeroStart;
	int fLeftOffset;
	int fRightOffset;
	Classes::TNotifyEvent fOnChange;
	Controls::TCursor fCursor;
	bool fVisible;
	bool fUseFontStyle;
	bool fAutoSize;
	int fAutoSizeDigitCount;
	TSynGutterBorderStyle fBorderStyle;
	int fLineNumberStart;
	bool fGradient;
	Graphics::TColor fGradientStartColor;
	Graphics::TColor fGradientEndColor;
	int fGradientSteps;
	void __fastcall SetAutoSize(const bool Value);
	void __fastcall SetBorderColor(const Graphics::TColor Value);
	void __fastcall SetColor(const Graphics::TColor Value);
	void __fastcall SetDigitCount(int Value);
	void __fastcall SetLeadingZeros(const bool Value);
	void __fastcall SetLeftOffset(int Value);
	void __fastcall SetRightOffset(int Value);
	void __fastcall SetShowLineNumbers(const bool Value);
	void __fastcall SetUseFontStyle(bool Value);
	void __fastcall SetVisible(bool Value);
	void __fastcall SetWidth(int Value);
	void __fastcall SetZeroStart(const bool Value);
	void __fastcall SetFont(Graphics::TFont* Value);
	void __fastcall OnFontChange(System::TObject* Sender);
	void __fastcall SetBorderStyle(const TSynGutterBorderStyle Value);
	void __fastcall SetLineNumberStart(const int Value);
	void __fastcall SetGradient(const bool Value);
	void __fastcall SetGradientStartColor(const Graphics::TColor Value);
	void __fastcall SetGradientEndColor(const Graphics::TColor Value);
	void __fastcall SetGradientSteps(const int Value);
	
public:
	__fastcall TSynGutter(void);
	__fastcall virtual ~TSynGutter(void);
	virtual void __fastcall Assign(Classes::TPersistent* Source);
	void __fastcall AutoSizeDigitCount(int LinesCount);
	AnsiString __fastcall FormatLineNumber(int Line);
	int __fastcall RealGutterWidth(int CharWidth);
	
__published:
	__property bool AutoSize = {read=fAutoSize, write=SetAutoSize, default=0};
	__property TSynGutterBorderStyle BorderStyle = {read=fBorderStyle, write=SetBorderStyle, default=1}
		;
	__property Graphics::TColor Color = {read=fColor, write=SetColor, default=-2147483633};
	__property Graphics::TColor BorderColor = {read=fBorderColor, write=SetBorderColor, default=-2147483643
		};
	__property Controls::TCursor Cursor = {read=fCursor, write=fCursor, default=0};
	__property int DigitCount = {read=fDigitCount, write=SetDigitCount, default=4};
	__property Graphics::TFont* Font = {read=fFont, write=SetFont};
	__property bool LeadingZeros = {read=fLeadingZeros, write=SetLeadingZeros, default=0};
	__property int LeftOffset = {read=fLeftOffset, write=SetLeftOffset, default=16};
	__property int RightOffset = {read=fRightOffset, write=SetRightOffset, default=2};
	__property bool ShowLineNumbers = {read=fShowLineNumbers, write=SetShowLineNumbers, default=0};
	__property bool UseFontStyle = {read=fUseFontStyle, write=SetUseFontStyle, default=1};
	__property bool Visible = {read=fVisible, write=SetVisible, default=1};
	__property int Width = {read=fWidth, write=SetWidth, default=30};
	__property bool ZeroStart = {read=fZeroStart, write=SetZeroStart, default=0};
	__property int LineNumberStart = {read=fLineNumberStart, write=SetLineNumberStart, default=1};
	__property bool Gradient = {read=fGradient, write=SetGradient, default=0};
	__property Graphics::TColor GradientStartColor = {read=fGradientStartColor, write=SetGradientStartColor
		, default=-2147483643};
	__property Graphics::TColor GradientEndColor = {read=fGradientEndColor, write=SetGradientEndColor, 
		default=-2147483633};
	__property int GradientSteps = {read=fGradientSteps, write=SetGradientSteps, default=48};
	__property Classes::TNotifyEvent OnChange = {read=fOnChange, write=fOnChange};
};


class DELPHICLASS TSynBookMarkOpt;
class PASCALIMPLEMENTATION TSynBookMarkOpt : public Classes::TPersistent 
{
	typedef Classes::TPersistent inherited;
	
private:
	Controls::TImageList* fBookmarkImages;
	bool fDrawBookmarksFirst;
	bool fEnableKeys;
	bool fGlyphsVisible;
	int fLeftMargin;
	Classes::TComponent* fOwner;
	int fXoffset;
	Classes::TNotifyEvent fOnChange;
	void __fastcall SetBookmarkImages(const Controls::TImageList* Value);
	void __fastcall SetDrawBookmarksFirst(bool Value);
	void __fastcall SetGlyphsVisible(bool Value);
	void __fastcall SetLeftMargin(int Value);
	void __fastcall SetXOffset(int Value);
	
public:
	__fastcall TSynBookMarkOpt(Classes::TComponent* AOwner);
	virtual void __fastcall Assign(Classes::TPersistent* Source);
	
__published:
	__property Controls::TImageList* BookmarkImages = {read=fBookmarkImages, write=SetBookmarkImages};
	__property bool DrawBookmarksFirst = {read=fDrawBookmarksFirst, write=SetDrawBookmarksFirst, default=1
		};
	__property bool EnableKeys = {read=fEnableKeys, write=fEnableKeys, default=1};
	__property bool GlyphsVisible = {read=fGlyphsVisible, write=SetGlyphsVisible, default=1};
	__property int LeftMargin = {read=fLeftMargin, write=SetLeftMargin, default=2};
	__property int Xoffset = {read=fXoffset, write=SetXOffset, default=12};
	__property Classes::TNotifyEvent OnChange = {read=fOnChange, write=fOnChange};
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TSynBookMarkOpt(void) { }
	#pragma option pop
	
};


class DELPHICLASS TSynGlyph;
class PASCALIMPLEMENTATION TSynGlyph : public Classes::TPersistent 
{
	typedef Classes::TPersistent inherited;
	
private:
	bool fVisible;
	Graphics::TBitmap* fInternalGlyph;
	Graphics::TBitmap* fGlyph;
	Graphics::TColor fInternalMaskColor;
	Graphics::TColor fMaskColor;
	Classes::TNotifyEvent fOnChange;
	void __fastcall SetGlyph(Graphics::TBitmap* Value);
	void __fastcall GlyphChange(System::TObject* Sender);
	void __fastcall SetMaskColor(Graphics::TColor Value);
	void __fastcall SetVisible(bool Value);
	int __fastcall GetWidth(void);
	int __fastcall GetHeight(void);
	
public:
	__fastcall TSynGlyph(unsigned aModule, const AnsiString aName, Graphics::TColor aMaskColor);
	__fastcall virtual ~TSynGlyph(void);
	virtual void __fastcall Assign(Classes::TPersistent* aSource);
	void __fastcall Draw(Graphics::TCanvas* aCanvas, int aX, int aY, int aLineHeight);
	__property int Width = {read=GetWidth, nodefault};
	__property int Height = {read=GetHeight, nodefault};
	
__published:
	__property Graphics::TBitmap* Glyph = {read=fGlyph, write=SetGlyph};
	__property Graphics::TColor MaskColor = {read=fMaskColor, write=SetMaskColor, default=536870911};
	__property bool Visible = {read=fVisible, write=SetVisible, default=1};
	__property Classes::TNotifyEvent OnChange = {read=fOnChange, write=fOnChange};
};


class DELPHICLASS ESynMethodChain;
class PASCALIMPLEMENTATION ESynMethodChain : public Sysutils::Exception 
{
	typedef Sysutils::Exception inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall ESynMethodChain(const AnsiString Msg) : Sysutils::Exception(
		Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall ESynMethodChain(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : Sysutils::Exception(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall ESynMethodChain(int Ident)/* overload */ : Sysutils::Exception(
		Ident) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall ESynMethodChain(int Ident, const System::TVarRec * Args
		, const int Args_Size)/* overload */ : Sysutils::Exception(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall ESynMethodChain(const AnsiString Msg, int AHelpContext
		) : Sysutils::Exception(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall ESynMethodChain(const AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : Sysutils::Exception(Msg, Args, Args_Size, AHelpContext
		) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall ESynMethodChain(int Ident, int AHelpContext)/* overload */
		 : Sysutils::Exception(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall ESynMethodChain(System::PResStringRec ResStringRec
		, const System::TVarRec * Args, const int Args_Size, int AHelpContext)/* overload */ : Sysutils::Exception(
		ResStringRec, Args, Args_Size, AHelpContext) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~ESynMethodChain(void) { }
	#pragma option pop
	
};


typedef void __fastcall (__closure *TSynExceptionEvent)(System::TObject* Sender, Sysutils::Exception* 
	E, bool &DoContinue);

class DELPHICLASS TSynMethodChain;
class PASCALIMPLEMENTATION TSynMethodChain : public System::TObject 
{
	typedef System::TObject inherited;
	
private:
	Classes::TList* FNotifyProcs;
	TSynExceptionEvent FExceptionHandler;
	
protected:
	virtual void __fastcall DoFire(const Sysutils::TMethod &AEvent) = 0 ;
	virtual bool __fastcall DoHandleException(Sysutils::Exception* E);
	__property TSynExceptionEvent ExceptionHandler = {read=FExceptionHandler, write=FExceptionHandler};
		
	
public:
	__fastcall virtual TSynMethodChain(void);
	__fastcall virtual ~TSynMethodChain(void);
	void __fastcall Add(const Sysutils::TMethod &AEvent);
	void __fastcall Remove(const Sysutils::TMethod &AEvent);
	void __fastcall Fire(void);
};


class DELPHICLASS TSynNotifyEventChain;
class PASCALIMPLEMENTATION TSynNotifyEventChain : public TSynMethodChain 
{
	typedef TSynMethodChain inherited;
	
private:
	System::TObject* FSender;
	
protected:
	virtual void __fastcall DoFire(const Sysutils::TMethod &AEvent);
	
public:
	__fastcall TSynNotifyEventChain(System::TObject* ASender);
	HIDESBASE void __fastcall Add(Classes::TNotifyEvent AEvent);
	HIDESBASE void __fastcall Remove(Classes::TNotifyEvent AEvent);
	__property ExceptionHandler ;
	__property System::TObject* Sender = {read=FSender, write=FSender};
public:
	#pragma option push -w-inl
	/* TSynMethodChain.Create */ inline __fastcall virtual TSynNotifyEventChain(void) : TSynMethodChain(
		) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TSynMethodChain.Destroy */ inline __fastcall virtual ~TSynNotifyEventChain(void) { }
	#pragma option pop
	
};


class DELPHICLASS TSynInternalImage;
class PASCALIMPLEMENTATION TSynInternalImage : public System::TObject 
{
	typedef System::TObject inherited;
	
private:
	Graphics::TBitmap* fImages;
	int fWidth;
	int fHeight;
	int fCount;
	Graphics::TBitmap* __fastcall CreateBitmapFromInternalList(unsigned aModule, const AnsiString Name)
		;
	void __fastcall FreeBitmapFromInternalList(void);
	
public:
	__fastcall TSynInternalImage(unsigned aModule, const AnsiString Name, int Count);
	__fastcall virtual ~TSynInternalImage(void);
	void __fastcall Draw(Graphics::TCanvas* ACanvas, int Number, int X, int Y, int LineHeight);
	void __fastcall DrawTransparent(Graphics::TCanvas* ACanvas, int Number, int X, int Y, int LineHeight
		, Graphics::TColor TransparentColor);
};


typedef Forms::TFormBorderStyle TSynBorderStyle;

#pragma option push -b-
enum THKModifier { hkShift, hkCtrl, hkAlt };
#pragma option pop

typedef Set<THKModifier, hkShift, hkAlt>  THKModifiers;

#pragma option push -b-
enum THKInvalidKey { hcNone, hcShift, hcCtrl, hcAlt, hcShiftCtrl, hcShiftAlt, hcCtrlAlt, hcShiftCtrlAlt 
	};
#pragma option pop

typedef Set<THKInvalidKey, hcNone, hcShiftCtrlAlt>  THKInvalidKeys;

class DELPHICLASS TSynHotKey;
class PASCALIMPLEMENTATION TSynHotKey : public Controls::TCustomControl 
{
	typedef Controls::TCustomControl inherited;
	
private:
	Forms::TFormBorderStyle FBorderStyle;
	Classes::TShortCut FHotKey;
	THKInvalidKeys FInvalidKeys;
	THKModifiers FModifiers;
	bool FPressedOnlyModifiers;
	void __fastcall SetBorderStyle(const Forms::TBorderStyle Value);
	void __fastcall SetHotKey(const Classes::TShortCut Value);
	void __fastcall SetInvalidKeys(const THKInvalidKeys Value);
	void __fastcall SetModifiers(const THKModifiers Value);
	MESSAGE void __fastcall WMGetDlgCode(Messages::TMessage &Message);
	HIDESBASE MESSAGE void __fastcall WMKillFocus(Messages::TWMKillFocus &Msg);
	HIDESBASE MESSAGE void __fastcall WMSetFocus(Messages::TWMSetFocus &Msg);
	
protected:
	virtual void __fastcall CreateParams(Controls::TCreateParams &Params);
	DYNAMIC void __fastcall DoExit(void);
	DYNAMIC void __fastcall KeyDown(Word &Key, Classes::TShiftState Shift);
	DYNAMIC void __fastcall KeyUp(Word &Key, Classes::TShiftState Shift);
	DYNAMIC void __fastcall MouseDown(Controls::TMouseButton Button, Classes::TShiftState Shift, int X, 
		int Y);
	virtual void __fastcall Paint(void);
	
public:
	__fastcall virtual TSynHotKey(Classes::TComponent* AOwner);
	
__published:
	__property Forms::TBorderStyle BorderStyle = {read=FBorderStyle, write=SetBorderStyle, default=1};
	__property Classes::TShortCut HotKey = {read=FHotKey, write=SetHotKey, default=65};
	__property THKInvalidKeys InvalidKeys = {read=FInvalidKeys, write=SetInvalidKeys, default=3};
	__property THKModifiers Modifiers = {read=FModifiers, write=SetModifiers, default=4};
public:
	#pragma option push -w-inl
	/* TCustomControl.Destroy */ inline __fastcall virtual ~TSynHotKey(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TWinControl.CreateParented */ inline __fastcall TSynHotKey(HWND ParentWindow) : Controls::TCustomControl(
		ParentWindow) { }
	#pragma option pop
	
};


class DELPHICLASS TSynEditSearchCustom;
class PASCALIMPLEMENTATION TSynEditSearchCustom : public Classes::TComponent 
{
	typedef Classes::TComponent inherited;
	
protected:
	virtual AnsiString __fastcall GetPattern(void) = 0 ;
	virtual void __fastcall SetPattern(const AnsiString Value) = 0 ;
	virtual int __fastcall GetLength(int aIndex) = 0 ;
	virtual int __fastcall GetResult(int aIndex) = 0 ;
	virtual int __fastcall GetResultCount(void) = 0 ;
	virtual void __fastcall SetOptions(const Synedittypes::TSynSearchOptions Value) = 0 ;
	
public:
	virtual int __fastcall FindAll(const AnsiString NewText) = 0 ;
	virtual AnsiString __fastcall Replace(const AnsiString aOccurrence, const AnsiString aReplacement) = 0 
		;
	__property AnsiString Pattern = {read=GetPattern, write=SetPattern};
	__property int ResultCount = {read=GetResultCount, nodefault};
	__property int Results[int aIndex] = {read=GetResult};
	__property int Lengths[int aIndex] = {read=GetLength};
	__property Synedittypes::TSynSearchOptions Options = {write=SetOptions, nodefault};
public:
	#pragma option push -w-inl
	/* TComponent.Create */ inline __fastcall virtual TSynEditSearchCustom(Classes::TComponent* AOwner)
		 : Classes::TComponent(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TComponent.Destroy */ inline __fastcall virtual ~TSynEditSearchCustom(void) { }
	#pragma option pop
	
};


typedef TRegistry TBetterRegistry;
;

class DELPHICLASS TSynEditMark;
class PASCALIMPLEMENTATION TSynEditMark : public System::TObject 
{
	typedef System::TObject inherited;
	
protected:
	Classes::TNotifyEvent fOnChange;
	int fLine;
	int fChar;
	int fImage;
	bool fVisible;
	bool fInternalImage;
	int fBookmarkNum;
	virtual void __fastcall SetChar(const int Value);
	virtual void __fastcall SetImage(const int Value);
	virtual void __fastcall SetLine(const int Value);
	void __fastcall SetVisible(const bool Value);
	void __fastcall SetInternalImage(const bool Value);
	bool __fastcall GetIsBookmark(void);
	
public:
	__fastcall TSynEditMark(void);
	__property int Line = {read=fLine, write=SetLine, nodefault};
	__property int Char = {read=fChar, write=SetChar, nodefault};
	__property int ImageIndex = {read=fImage, write=SetImage, nodefault};
	__property int BookmarkNumber = {read=fBookmarkNum, write=fBookmarkNum, nodefault};
	__property bool Visible = {read=fVisible, write=SetVisible, nodefault};
	__property bool InternalImage = {read=fInternalImage, write=SetInternalImage, nodefault};
	__property bool IsBookmark = {read=GetIsBookmark, nodefault};
	__property Classes::TNotifyEvent OnChange = {read=fOnChange, write=fOnChange};
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TSynEditMark(void) { }
	#pragma option pop
	
};


typedef TSynEditMark* TSynEditLineMarks[17];

class DELPHICLASS TSynEditMarkList;
class PASCALIMPLEMENTATION TSynEditMarkList : public System::TObject 
{
	typedef System::TObject inherited;
	
private:
	Classes::TList* fItems;
	Classes::TNotifyEvent fOnChange;
	void __fastcall DoChange(void);
	TSynEditMark* __fastcall GetItem(int Index);
	int __fastcall GetCount(void);
	void __fastcall InternalDelete(int Index);
	
public:
	__fastcall TSynEditMarkList(void);
	__fastcall virtual ~TSynEditMarkList(void);
	int __fastcall Add(TSynEditMark* Item);
	int __fastcall Remove(TSynEditMark* Item);
	void __fastcall ClearLine(int line);
	void __fastcall Clear(void);
	void __fastcall GetMarksForLine(int line, /* out */ TSynEditMark* * Marks);
	__property TSynEditMark* Items[int Index] = {read=GetItem/*, default*/};
	__property int Count = {read=GetCount, nodefault};
	__property Classes::TNotifyEvent OnChange = {read=fOnChange, write=fOnChange};
};


//-- var, const, procedure ---------------------------------------------------
static const Shortint BorderWidth = 0x0;

}	/* namespace Syneditmiscclasses */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Syneditmiscclasses;
#endif
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// SynEditMiscClasses
