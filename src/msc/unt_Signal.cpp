//---------------------------------------------------------------------------

#include <vcl.h>
#include <math.h>
#include <stdlib.h>

#pragma hdrstop

#include "unt_Signal.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//===========================================================================
//                       Constructor, Destructor

//---------------------------------------------------------------------------
cls_Signal::cls_Signal( short stype, double a, double t )
{ // Class constructor

// . . . Get inputs
        switch ( stype )
               {
                case 0 : CalculateValue = Sine; break;
                case 1 : CalculateValue = Ramp; break;
                case 2 : CalculateValue = Trapezoidal; break;
                case 3 : CalculateValue = Random; break;
                default : CalculateValue = Random;
               }

        if ( a ) A = a;
        else
           {
            ShowMessage("cls_Signal - A must be nonzero; 1 substituted");
            A = 1;
           }

        if ( t ) T = t;
        else
           {
            ShowMessage("cls_Signal - T must be nonzero; 1 substituted");
            T = 1;
           }

// . . . Initializations and defaults
        n = 0;
}


//---------------------------------------------------------------------------
cls_Signal::cls_Signal( CALCFUNCT Funct )
{ // Class constructor

// . . . Get inputs

        if ( Funct ) CalculateValue = Funct;
        else
           {
            ShowMessage("cls_Signal - undefined function; sine substituted");
            CalculateValue = Sine;
           }

// . . . Initializations and defaults
        n = 0;
}


//---------------------------------------------------------------------------
//cls_Signal::~cls_Signal()
//{ // Class destructor
//        //
//}
//---------------------------------------------------------------------------


//===========================================================================
//                              Methods
//---------------------------------------------------------------------------
double cls_Signal::GetValue(double t)
{ // Get a value
        n++;
        return CalculateValue(t);
}
//---------------------------------------------------------------------------


//===========================================================================
//                              Property A
//---------------------------------------------------------------------------
int cls_Signal::Rn ()
{ // Get n
        return in;
}
//---------------------------------------------------------------------------
void cls_Signal::Wn (int N)
{ // Set n
        in = N;
}
//---------------------------------------------------------------------------



//===========================================================================
//                              Event
//---------------------------------------------------------------------------
CALCFUNCT cls_Signal::RFunct ()
{ // Get Handler pointer
        //if ( !iFunct ) ShowMessage("Warning: Handler not defined");
        return iFunct;
}
//---------------------------------------------------------------------------
void cls_Signal::WFunct (CALCFUNCT F)
{ // Set Handler pointer
        iFunct = F;
}
//---------------------------------------------------------------------------



//===========================================================================
//                              Private Methods
//---------------------------------------------------------------------------
double cls_Signal::Sine ( double t )
{ // Sine

        return ( A * sin( (6.2831853072 * t / T ) ) );
}

//---------------------------------------------------------------------------
double cls_Signal::Ramp ( double t )
{ // Ramp

        return ( A * t / T );
}

//---------------------------------------------------------------------------
double cls_Signal::Trapezoidal ( double t )
{ // Trapezoidal

        static double tr = 0.15 * T; // tempo lati obliqui

        double tloc = int(t) % int(T);

        if ( tloc < tr ) return tloc*A/tr;
        else if ( tloc > ( T-tr ) ) return -tloc*A/T;
        else return A;
}


//---------------------------------------------------------------------------
double cls_Signal::Random (double t)
{ // Random

        randomize();
        return (rand() % int(A));
}
//---------------------------------------------------------------------------


//===========================================================================
