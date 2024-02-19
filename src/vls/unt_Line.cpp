//---------------------------------------------------------------------------

#include <vcl.h>
#include <math>
#include "Math.hpp"
#pragma hdrstop

#include "unt_Line.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
// . . . File scope
	const double pi = 3.1415926535897932384626433832795;
	const double pimezzi = 1.5707963267948966192313216916398;
	const double RadperDeg = 0.017453292519943295769236907684886;
	inline bool IsNumber (char c) { return ( ( c>='0' && c<='9' ) ||
						   c=='+' || c=='-' || c=='E' ||
						   c==DecimalSeparator ); }
	const double Huge = 1E10;
//---------------------------------------------------------------------------





//============================= Public methods ==============================

//---------------------------------------------------------------------------
cls_Line::cls_Line() : A(0), B(0), C(0) {}
cls_Line::cls_Line(const double& a, const double& b, const double& c) : A(a),B(b),C(c) {}
cls_Line::cls_Line(const cls_Point& p1, const cls_Point& p2)
{ // Straight line passing through p1 and p2

        double det = (p1.x * p2.y) - (p2.x * p1.y);

        if ( det == 0 ) // Passes through origin
           {
            C = 0;
            if ( p2.x != 0 )
               {
                B = 1;
                A = - p2.y / p2.x;
               }
            else if ( p1.x != 0 )
               {
                B = 1;
                A = - p1.y / p1.x;
               }
            else if ( p2.y != 0 )
               {
                A = 1;
                B = - p2.x / p2.y;
               }
            else if ( p1.y != 0 )
               {
                A = 1;
                B = - p1.x / p1.y;
               }
            else
               {
                A = B = C = 0;
               }

           }
        else
           {
            C = 1;
            A = (p2.y - p1.y) / det;
            B = (p1.x - p2.x) / det;
           }
}

cls_Line::cls_Line(const class cls_Point& p, const cls_Line& l)
{ // Straight line passing through p and perpendicular to l

        A = l.B;
        B = -l.A;
        C = A*p.x + B*p.y;
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
double cls_Line::At_x( const double& x ) const
{ // Value at abscissa

        if ( B == 0 ) return Huge;
        else return (C - A*x)/B;
}

//---------------------------------------------------------------------------
double cls_Line::At_y( const double& y ) const
{ // Value at abscissa

        if ( A == 0 ) return Huge;
        else return (C - B*y)/A;
}

//---------------------------------------------------------------------------
cls_Point cls_Line::Interseption( const cls_Line& l ) const
{ // Calculate Interseption point with another line

        double det = (A * l.B) - (B * l.A);
        if ( det == 0 ) return cls_Point (Huge,Huge); // parallel lines

        double x = ( l.B*C - B*l.C ) / det;
        double y = ( A*l.C - l.A*C ) / det;
        return cls_Point(x,y);
}

//=============================== Operators =================================

//---------------------------------------------------------------------------


//=============================== Properties ================================


//---------------------------------------------------------------------------


//=============================== Events ====================================


//---------------------------------------------------------------------------



//============================ Private Methods ==============================


//---------------------------------------------------------------------------


//============================== END OF FILE ================================
