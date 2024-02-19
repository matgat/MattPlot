//---------------------------------------------------------------------------

#ifndef unt_Polynomial_3rdH
#define unt_Polynomial_3rdH


	/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
	*
	*	        cls_Polynomial_3rd
	*	2006 - Matteo Gattanini
	*
	*	unt_Polynomial_3rd (mar.2006)
	*		ver: 01    (11.mar.2008)
	*
	*	This unit defines a class that represents
	*	a third order polynomial
	*
	*	Example of usage:
	*
	*	#include "unt_Polynomial_3rd.h"
	*	cls_Polynomial_3rd p3 = cls_Polynomial_3rd(2,23.4,0,2);
	*	double r[3] = p3.roots();
	*	double D = p3.discriminant;
	*
	* * * * * * * * * * * * * * * * * * * * * * * * * * * */


//--------------------------- Types declaration -----------------------------



//--------------------------- Class Definition ------------------------------
class cls_Polynomial_3rd {

public://							.............

// . . . Public methods
        cls_Polynomial_3rd( double=0, double=0, double=0, double=0 );
        //~cls_Polynomial_3rd();

	void __fastcall CalcRoots (); // calculates the three roots
		
// . . . Operators
/*
        // Assegnamento
        cls_Value& operator= (const cls_Value& v)  {
                                                    if (this == &v) return *this; // Handle self assignment
                                                    k = v.k;
                                                    i_UnitLabel = v.i_UnitLabel;
                                                    mksVal = v.mksVal;
                                                    return *this;
                                                   }
	// Assegnamento e cast di `double' : un numero è inteso come uniformato mks
        cls_Value& operator= ( double v ) { mksVal = v; return *this;}
        operator double() const {return mksVal;}

	// Assegnamento e cast di `String'
        cls_Value& operator= ( String s ) { StrVal = s; return *this;}
        operator String() const {return StrVal;}

        // Operazioni
        double operator+ (cls_Value& v) {return mksVal+v.mksVal;} // Addizione
        //cls_Value& operator- (cls_Value& v) {} // Sottrazione
        //cls_Value& operator* (cls_Value& v) {} // Prodotto
        //cls_Value& operator/ (cls_Value& v) {} // Divisione
*/

// . . . Properties
        __property double discriminant = { read = get_discriminant }; // Discriminante

// . . . Public attributes (!)
        double roots[3];


protected://							.............

private://							.............

// . . . Private attributes
        double C[4]; // Coefficients

// . . . Properties
        double __fastcall get_discriminant (void) const;

// . . . Private methods


};//-------------------------------------------------------------------------

//---------------------------------------------------------------------------
#endif
