//---------------------------------------------------------------------------

#ifndef unt_app_settingsH
#define unt_app_settingsH
//---------------------------------------------------------------------------

#include "unt_Options.h"

//---------------------------------------------------------------------------
//			    cls_App_settings
//		     ( opzioni dell'applicazione )

class cls_App_settings : public cls_Options {
//-----------------------------------------------
public:
	cls_App_settings ();

//. . . . . . . . . . . . . . . . . . . . . . . .
//. . . . Public attributes

//. . . [general]
        int dt; // velocity-position request interval [ms]
        bool DragEnabled;
        // Enable choosing movement
        bool ChooseMovement;
        
//-----------------------------------------------
private:
	void __fastcall ReadData ();
	void __fastcall StoreData ();

//-----------------------------------------------
};// end cls_App_settings definition

//---------------------------------------------------------------------------
extern cls_App_settings* opts;  // global pointer
//---------------------------------------------------------------------------
#endif
