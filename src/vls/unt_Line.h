//---------------------------------------------------------------------------

#ifndef unt_LineH
#define unt_LineH

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
	*
	*	        cls_Line
	*	2006 - Matteo Gattanini
	*
	*	unt_Line (mar.2006)
	*		ver: 00    (07.mar.2008)
	*
	*	This unit defines a class that represents
	*	straight lines (coefficient in double);
	*	it uses cls_Point
	*
	*	Example of usage:
	*
	*	#include "unt_Line.h"
	*	cls_Line* line1 = new cls_Line();
	*	cls_Line* line1 = new cls_Line();
	*	cls_Point c
	*	
	*	
	*	
	*	
	*	//... and at the end:
	*	delete line1,line2;
	*
	* * * * * * * * * * * * * * * * * * * * * * * * * * * */

//--------------------------- Class Definition ------------------------------
class cls_Line {

public://							.............

// . . . Public methods
        cls_Line();
        cls_Line(const double&, const double&, const double&);
        cls_Line(const class cls_Point&, const class cls_Point&);
        cls_Line::cls_Line(const class cls_Point& , const cls_Line&);

        double At_x( const double& ) const;
        double At_y( const double& ) const;
        class cls_Point Interseption( const cls_Line& ) const;
        //double __fastcall Angle();

// . . . Operators

// . . . Properties

// . . . Public attributes (!)
	double A,B,C;
        
private://							.............

// . . . Private attributes
// . . . Properties
// . . . Private methods

};//-------------------------------------------------------------------------
#endif
 
