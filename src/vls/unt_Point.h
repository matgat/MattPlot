//---------------------------------------------------------------------------

#ifndef unt_PointH
#define unt_PointH


	/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
	*
	*	cls_Point
	*	2006 - Matteo Gattanini
	*
	*	unt_Point (mar.2006)
	*		ver: 03    (may.2007)
	*
	*	This unit defines a class that represents
	*	a point in two-dimension planes
	*
	*	Example of usage:
	*
	*	#include "unt_Point.h"
	*	cls_Point P1 = cls_Point(23.4, 3.1);
	*	cls_Point* P2 = new cls_Point(0.1,30);
	*	cls_Point P3 = P1 + *P2;
	*	
	*	//... and at the end:
	*	delete P2;
	*
	* * * * * * * * * * * * * * * * * * * * * * * * * * * */

//#include <cmath>

//--------------------------- Class Definition ------------------------------
class cls_Point { // Point with coordinates in double

public://							                            .............

// . . . Public methods
        cls_Point(const double& px =0, const double& py =0) {x=px; y=py; Init();}
        cls_Point(const TPoint& pt) {x=pt.x; y=pt.y; Init();}
        cls_Point(const String S) { Assign(S); Init();}
        //~cls_Point(); // Class destructor

        // Some utils
        double dist (const cls_Point& p) const {return ::sqrt( (x-p.x)*(x-p.x) + (y-p.y)*(y-p.y) );} // distanza tra due punti
        cls_Point Middle ( const cls_Point& p) const {return cls_Point( p.x+(x-p.x)/2, p.y+(y-p.y)/2 );} // Calculate middle point
        bool IsNear (const cls_Point& p, const double& d) const {return dist(p) <= d;} // if point is near

        // Direct and inverse coordinate transformations
        //TPoint ToXY( const TPoint&, const double&, const double&, const int&);
        //TPoint Toxy( const TPoint&, const double&, const double&, const int&);

        // Conversions
        void SetRepresentation( const int xp, const int xd, const int yp, const int yd) {Xfltprc=xp; Yfltprc=yp; Xfltdgt=xd; Yfltdgt=yd;}
        String ToString () const {return "("+mat::ToString(x,Xfltprc,Xfltdgt)+" ; "+mat::ToString(y,Yfltprc,Yfltdgt)+")";} // Transform to a String to be viewed


// . . . Operators
        cls_Point operator+ (const cls_Point& p) {return cls_Point(x+p.x, y+p.y);} // Addizione
        cls_Point operator- (const cls_Point& p) {return cls_Point(x-p.x, y-p.y);} // Sottrazione
        cls_Point operator* (const double h) {return cls_Point(h*x, h*y);} // Moltiplica per costante
        cls_Point operator/ (const double h) {return cls_Point(x/h, y/h);} // Divisione per costante
        // Casts
        operator TPoint() const {return TPoint ( mat::round(x), mat::round(y) );} // cast to TPoint
        operator String() const {return ToString();} // cast to String
        // Assignment
        cls_Point& operator= (const cls_Point& p) { if (&p!=this) {x=p.x; y=p.y;} return *this; }
        cls_Point& operator= (const String& s) { Assign(s); return *this; }

// . . . Properties
        __property double x = {read=_x, write=_x}; // Abscissa
        __property double y = {read=_y, write=_y}; // Ordinate
        __property double rho = {read=get_rho}; // Magnitude (distance from origin)
        __property double phi = {read=get_phi}; // Phase (angle respect abscissa axis)
        __property double Phi = {read=get_Phi}; // Phase (0:2pi)
        
// . . . Public attributes (!)
        int Xfltprc,Xfltdgt,Yfltprc,Yfltdgt; // Number string representation

private://							                            .............

// . . . Private attributes

// . . . Properties
        double _x,_y;
        double get_rho() {return ::sqrt(_x*_x + _y*_y);}
        double get_phi() {return ::atan2(_y,_x);} // (-pi:+pi)
        double get_Phi() {double a=phi; return (a<0) ? a+mat::duepi : a;} // (0:2pi)

// . . . Private methods
        void Init() { Xfltprc=Yfltprc=6; Xfltdgt=Yfltdgt=3; };
        void Assign (const String&); // Given a string, try to retrieve a couple of coordinates

};//-------------------------------------------------------------------------
#endif
 
