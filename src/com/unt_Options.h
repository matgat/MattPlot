#ifndef unt_OptionsH
#define unt_OptionsH

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * *
    *
    *       unt_Options
    *
    *   cls_Options
    *       ver: 0.11    (26.may.2006)
    *       2006 - Matteo Gattanini
    *   This unit contains a base class to manage
    *   ini options files
    *
    *   Example of usage:
    *   #include "unt_Options.h"
    *   cls_Options* o = new cls_Options();
    *   o->ReadFromFile("C:\\file.ini");
    *   o->StoreToFile("C:\\file.ini");
    *   delete o;
    *
    * * * * * * * * * * * * * * * * * * * * * * * * * * * */


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class cls_Options {

public://                           ~~~~~~~~~~~~~
// ~~~~~~~~~~~~~ Types definitions              ~~~~~~~~~~~~~
    typedef void __fastcall (__closure (*typ_FastVoidFunct))(); // A member function

// . . . Public methods
    __fastcall cls_Options (class cls_Log*);
    //__fastcall ~cls_Options ();

    // Main services
    void ReadFromIniFile (class cls_OptionValue**,const String&);
    void StoreToIniFile (class cls_OptionValue**,const String&);
    void SetDefaults (class cls_OptionValue**);

// . . . Public attributes (!)
    typ_FastVoidFunct OnRead; // Called after read
    typ_FastVoidFunct OnStore; // Called before store
    static class cls_OptionValue*const SKIP_ADDR;

private://                          ~~~~~~~~~~~~~

// . . . Private attributes
    class cls_Log* Log;

// . . . Private methods

};//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#endif
