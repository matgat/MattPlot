//---------------------------------------------------------------------------
#ifndef unt_MainOptionsH
#define unt_MainOptionsH
//---------------------------------------------------------------------------

#include "unt_IniOptions.h"

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
	*
	*		unt_MainOptions
	*
	*	cls_MainOptions
	*		ver: 0.0    (31.jul.2006)
	*		2006 - Matteo Gattanini
	*	This unit manages the applications options
	*
	*	Example of usage:
	*	#include "unt_MainOptions.h"
	*	cls_MainOptions* o = new cls_MainOptions();
	*	o->ReadFromFile("C:\\file.ini");
	*	o->StoreToFile("C:\\file.ini");
	*	delete o;
	*
	* * * * * * * * * * * * * * * * * * * * * * * * * * * */


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class cls_MainOptions : public cls_IniOptions {

public://							~~~~~~~~~~~~~
// ~~~~~~~~~~~~~ Types definitions				~~~~~~~~~~~~~
// . . . Public methods
	__fastcall cls_MainOptions ();
	//__fastcall ~cls_MainOptions ();

// . . . Public attributes (!)
        bool PlaySound;

private://							~~~~~~~~~~~~~
// . . . Private methods
// . . . Private attributes

};//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#endif
