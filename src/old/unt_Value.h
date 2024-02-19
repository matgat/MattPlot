//---------------------------------------------------------------------------
#ifndef unt_ValueH
#define unt_ValueH
//---------------------------------------------------------------------------

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
	*
	*	        cls_Value
	*	2006 - Matteo Gattanini
	*
	*	unt_Value (mar.2006)
	*		ver: 01    (11.mar.2008)
	*
	*	This unit defines a class that represents
	*	a scalar quantity (double)
	*
	*	Example of usage:
	*
	*	#include "unt_Value.h"
	*	cls_Value v = cls_Value(23.4);
	*	v *= 32.5;
	*	ShowMessage(v);
	*
	* * * * * * * * * * * * * * * * * * * * * * * * * * * */


//---------------------------- Macro definition -----------------------------

//--------------------------- Types declaration -----------------------------


//--------------------------- Class Definition ------------------------------
class cls_Value {

public://							.............

// . . . Public methods
        cls_Value( double=0 );
        //~cls_Value();
		
// . . . Operators
        // Assegnamento
        cls_Value& operator= (const cls_Value& v)  {
                                                    if (this == &v) return *this; // Handle self assignment
                                                   }
	// Assegnamento e cast di `double' : un numero è inteso come uniformato mks
        cls_Value& operator= ( double v ) { return *this;}
        operator double() const {return Val;}

	// Assegnamento e cast di `String'
        cls_Value& operator= ( String s ) { return *this;}
        operator String() const {return Val;}

        // Operazioni
        double operator+ (cls_Value& v) {return mksVal+v.mksVal;} // Addizione
        //cls_Value& operator- (cls_Value& v) {} // Sottrazione
        //cls_Value& operator* (cls_Value& v) {} // Prodotto
        //cls_Value& operator/ (cls_Value& v) {} // Divisione

// . . . Properties
        __property double Val = { read = get_Val, write = set_Val }; // MKS value

// . . . Public attributes (!)
	// Force string-double conversion if fails
        // Estremi dell'intervallo consentito
	//double MinVal,MaxVal;

protected://							.............

// . . . Protected attributes
	double v; // To MKS value coefficient [mks/unit]

// . . . Properties
        double __fastcall get_Val (void) const {return v;}
        void __fastcall set_Val (const double d) { v=d; }

// . . . Protected methods

private://							.............

// . . . Private attributes

// . . . Private methods


};//-------------------------------------------------------------------------




//---------------------------------------------------------------------------
#endif
