// Borland C++ Builder
// Copyright (c) 1995, 1999 by Borland International
// All rights reserved

// (DO NOT EDIT: machine generated header) 'SynEditKbdHandler.pas' rev: 5.00

#ifndef SynEditKbdHandlerHPP
#define SynEditKbdHandlerHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <Classes.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <SynEditTypes.hpp>	// Pascal unit
#include <Forms.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Messages.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Syneditkbdhandler
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TKeyboardControl;
class PASCALIMPLEMENTATION TKeyboardControl : public Controls::TWinControl 
{
	typedef Controls::TWinControl inherited;
	
public:
	__property OnKeyDown ;
	__property OnKeyPress ;
	__property OnMouseDown ;
public:
	#pragma option push -w-inl
	/* TWinControl.Create */ inline __fastcall virtual TKeyboardControl(Classes::TComponent* AOwner) : 
		Controls::TWinControl(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TWinControl.CreateParented */ inline __fastcall TKeyboardControl(HWND ParentWindow) : Controls::TWinControl(
		ParentWindow) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TWinControl.Destroy */ inline __fastcall virtual ~TKeyboardControl(void) { }
	#pragma option pop
	
};


typedef void __fastcall (__closure *TMouseCursorEvent)(System::TObject* Sender, const Synedittypes::TBufferCoord 
	&aLineCharPos, Controls::TCursor &aCursor);

class DELPHICLASS TMethodList;
class PASCALIMPLEMENTATION TMethodList : public System::TObject 
{
	typedef System::TObject inherited;
	
private:
	Classes::TList* fData;
	Sysutils::TMethod __fastcall GetItem(int aIndex);
	int __fastcall GetCount(void);
	
public:
	__fastcall TMethodList(void);
	__fastcall virtual ~TMethodList(void);
	void __fastcall Add(const Sysutils::TMethod &aHandler);
	void __fastcall Remove(const Sysutils::TMethod &aHandler);
	__property Sysutils::TMethod Items[int aIndex] = {read=GetItem/*, default*/};
	__property int Count = {read=GetCount, nodefault};
};


class DELPHICLASS TSynEditKbdHandler;
class PASCALIMPLEMENTATION TSynEditKbdHandler : public System::TObject 
{
	typedef System::TObject inherited;
	
private:
	TMethodList* fKeyPressChain;
	TMethodList* fKeyDownChain;
	TMethodList* fKeyUpChain;
	TMethodList* fMouseDownChain;
	TMethodList* fMouseUpChain;
	TMethodList* fMouseCursorChain;
	bool fInKeyPress;
	bool fInKeyDown;
	bool fInKeyUp;
	bool fInMouseDown;
	bool fInMouseUp;
	bool fInMouseCursor;
	
public:
	__fastcall TSynEditKbdHandler(void);
	__fastcall virtual ~TSynEditKbdHandler(void);
	void __fastcall ExecuteKeyPress(System::TObject* Sender, char &Key);
	void __fastcall ExecuteKeyDown(System::TObject* Sender, Word &Key, Classes::TShiftState Shift);
	void __fastcall ExecuteKeyUp(System::TObject* Sender, Word &Key, Classes::TShiftState Shift);
	void __fastcall ExecuteMouseDown(System::TObject* Sender, Controls::TMouseButton Button, Classes::TShiftState 
		Shift, int X, int Y);
	void __fastcall ExecuteMouseUp(System::TObject* Sender, Controls::TMouseButton Button, Classes::TShiftState 
		Shift, int X, int Y);
	void __fastcall ExecuteMouseCursor(System::TObject* Sender, const Synedittypes::TBufferCoord &aLineCharPos
		, Controls::TCursor &aCursor);
	void __fastcall AddKeyDownHandler(Controls::TKeyEvent aHandler);
	void __fastcall RemoveKeyDownHandler(Controls::TKeyEvent aHandler);
	void __fastcall AddKeyUpHandler(Controls::TKeyEvent aHandler);
	void __fastcall RemoveKeyUpHandler(Controls::TKeyEvent aHandler);
	void __fastcall AddKeyPressHandler(Controls::TKeyPressEvent aHandler);
	void __fastcall RemoveKeyPressHandler(Controls::TKeyPressEvent aHandler);
	void __fastcall AddMouseDownHandler(Controls::TMouseEvent aHandler);
	void __fastcall RemoveMouseDownHandler(Controls::TMouseEvent aHandler);
	void __fastcall AddMouseUpHandler(Controls::TMouseEvent aHandler);
	void __fastcall RemoveMouseUpHandler(Controls::TMouseEvent aHandler);
	void __fastcall AddMouseCursorHandler(TMouseCursorEvent aHandler);
	void __fastcall RemoveMouseCursorHandler(TMouseCursorEvent aHandler);
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Syneditkbdhandler */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Syneditkbdhandler;
#endif
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// SynEditKbdHandler
