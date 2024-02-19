//---------------------------------------------------------------------------

#ifndef unt_OptionsH
#define unt_OptionsH

	/*******************************************************
	*                                                      *
	*      			cls_Options                    *
	*            ( gestione opzioni in file *.ini )        *
	*                                                      *
	*               2005 - Matteo Gattanini                *
	*                                                      *
	*******************************************************/

#include <inifiles.hpp>

//---------------------------------------------------------------------------
typedef void __fastcall( __closure *fastvoid)();
//---------------------------------------------------------------------------


/*
//---------------------------------------------------------------------------
class cls_OptionValue {

public:
	cls_OptionValue ();

// . . . Public methods

        // File I/O
        virtual String Read ( const class TIniFile* )=0;
        virtual String Write ( const class TIniFile* )=0;

private:
// . . . Private methods
        String ToDoubleLiteral ( String );

// . . . Private attributes

// . . . Properties

//-----------------------------------------------
};// end cls_OptionValue definition


//---------------------------------------------------------------------------
class cls_OptionString : public cls_OptionValue {

public:
	cls_OptionValue ();

// . . . Public methods

        // File I/O
        virtual String Read ( const class TIniFile* )=0;
        virtual String Write ( const class TIniFile* )=0;

private:
// . . . Private methods
        String ToDoubleLiteral ( String );

// . . . Private attributes

// . . . Properties

//-----------------------------------------------
};// end cls_OptionString definition

*/


//---------------------------------------------------------------------------
class cls_Options {

public:
	cls_Options ();

// . . . . . . . . . . . . . . . . . . . . . . . .
// . . . Public methods

        // File I/O
        String ReadFrom ( const String& );
        String StoreIn ( const String& );

// . . . On Read-Store Data event handlers
        __property fastvoid OnReadData = {read=ROnReadData, write=WOnReadData};
	__property fastvoid OnStoreData = {read=ROnStoreData, write=WOnStoreData};


protected:
// . . . Protected methods
        void Section (String); // set current section (controlling its existance)

        // Reading
        bool Read(String, bool); // reads a value (controlling its existance)
        int Read(String, int);
        String Read(String, String);
        double Read(String, double);

        // Writing
        void Write(String, bool); // write a value
        void Write(String, int);
        void Write(String, String);
        void Write(String, double);

        // Utilities
        String ToDoubleLiteral ( String );

// . . . Private attributes
        String section; // current section
        class TIniFile* ini; // current opened ini file
        int missingsects, missingvals; // counters

        String tmp; // temporary string

// . . . Events
        fastvoid iOnReadData; // internal pointer for event handler
        fastvoid ROnReadData (); // Get Handler pointer
        void WOnReadData (fastvoid); // Set Handler pointer

	fastvoid iOnStoreData; // internal pointer for event handler
        fastvoid ROnStoreData (); // Get Handler pointer
        void WOnStoreData (fastvoid); // Set Handler pointer

//-----------------------------------------------
};// end cls_Options definition


//---------------------------------------------------------------------------
#endif
