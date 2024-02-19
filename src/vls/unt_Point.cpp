//---------------------------------------------------------------------------
#include <vcl.h>
#include "unt_MatCommon.h"
#pragma hdrstop
#include "unt_Point.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


//------------------------ file scope declarations --------------------------




//============================= Public methods ==============================

//---------------------------------------------------------------------------
// Distance from line
//   ^       a x + b y+ c = 0         |a xp + b yp + c |
// y |       /                   d =  -----------------
//   |     /  ' d                       sqrt(a^2+b^2)
//   |   /      '   P(xp;yp)
//   | /          °              if P1(x1;y1) and P2(x2;y2) belong to a*x+b*y+c=0
//  -|--------------------->
//  o|                    x            |(x2-x1)(y1-yp) - (x1-xp)(y2-y1)|
//                                d = ----------------------------------
//                                       sqrt( (x2-x1)^2 + (y2-y1)^2 )



//---------------------------------------------------------------------------
// Coordinate tranformations
//   ^       _
// y |       /|X
//   |     /
//   |   /_
//   | /O  |alpha
//  -|-\------------------->
//  o|  \                x
//      _\|Y

/*
//---------------------------------------------------------------------------
TPoint cls_Point::ToXY ( const TPoint& O,
                         const double& kx,
                         const double& ky,
                         const int& alpha )
{ // direct coordinate transform
        TPoint XY;
        double a = alpha * mat::deg2rad; // to rad
        XY.x = kx*(::cos(a)*x - ::sin(a)*y) - O.x;
        XY.y = ky*(::cos(a)*y + ::sin(a)*x) - O.y;
        return XY;
}

//---------------------------------------------------------------------------
TPoint cls_Point::Toxy ( const TPoint& O,
                         const double& kx,
                         const double& ky,
                         const int& alpha )
{ // inverse coordinate transform
        TPoint xy;
        double a = alpha * mat::deg2rad; // to rad
        xy.x = (::cos(a)*x + ::sin(a)*y + O.x)/kx;
        xy.y = (::cos(a)*y - ::sin(a)*x + O.y)/ky;
        return xy;
}
*/



//=============================== Operators =================================

//---------------------------------------------------------------------------



//=============================== Properties ================================

//---------------------------------------------------------------------------


//================================= Events ==================================


//---------------------------------------------------------------------------



//============================= Private Methods =============================

//---------------------------------------------------------------------------
void cls_Point::Assign (const String& S)
{ // Given a string, try to retrieve a couple of coordinates

        // Retrieve separate Strings of values
        String xstr = "";
        String ystr = "";
        int i = 1;

        // Ignore non numerical characters
        while ( i <= S.Length() && !mat::IsNumber(S[i]) ) i++;

        // Now add only 123456789+-E
        while ( i <= S.Length() && mat::IsNumber(S[i]) ) xstr += S[i++];

        // Ignore non numerical characters
        while ( i <= S.Length() && !mat::IsNumber(S[i]) ) i++;

        // Now add only 123456789+-E
        while ( i <= S.Length() && mat::IsNumber(S[i]) ) ystr += S[i++];

        // Now assign values
        try { x = StrToFloat( xstr ); } catch(...) { x = 0; }
        try { y = StrToFloat( ystr ); } catch(...) { y = 0; }
}


//=============================== END OF FILE ===============================
