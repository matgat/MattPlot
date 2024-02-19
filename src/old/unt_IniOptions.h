#ifndef unt_OptionsH
#define unt_OptionsH

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
	*
	*		unt_Options
	*
	*	cls_Options
	*		ver: 0.11    (26.may.2006)
	*		2006 - Matteo Gattanini
	*	This unit contains a base class to manage
	*	ini options files
	*
	*	Example of usage:
	*	#include "unt_Options.h"
	*	cls_Options* o = new cls_Options();
	*	o->ReadFromFile("C:\\file.ini");
	*	o->StoreToFile("C:\\file.ini");
	*	delete o;
	*
	* * * * * * * * * * * * * * * * * * * * * * * * * * * */


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class cls_Options {

public://							~~~~~~~~~~~~~
// ~~~~~~~~~~~~~ Types definitions				~~~~~~~~~~~~~
	typedef void __fastcall (__closure *FastVoidFunct)(); // A member function

// . . . Public methods
	__fastcall cls_Options (class cls_OptionValue** v);
	//__fastcall ~cls_Options ();

	// File I/O
	void __fastcall ReadFromFile (const String&);
	void __fastcall StoreToFile (const String&);

// . . . Events
	__property FastVoidFunct OnRead = {read=get_OnRead, write=set_OnRead};
	__property FastVoidFunct OnStore = {read=get_OnStore, write=set_OnStore};

// . . . Public attributes (!)

protected://							~~~~~~~~~~~~~
// . . . Protected methods

// . . . Protected attributes

// . . . Events
	FastVoidFunct iOnStore,iOnRead;
	FastVoidFunct __fastcall get_OnRead () const {return iOnRead;}
	void __fastcall set_OnRead (FastVoidFunct F) {iOnRead=F;}
	FastVoidFunct __fastcall get_OnStore (){return iOnStore;}
	void __fastcall set_OnStore (FastVoidFunct F) {iOnStore=F;}

private://							~~~~~~~~~~~~~
// . . . Private methods

// . . . Private attributes
        class cls_OptionValue** Values;
        class cls_IniFile *ini;
	class cls_Log* Log;
	String CurrentSection; // current section
	int MissingSects, MissingVals; // counters
};//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#endif
