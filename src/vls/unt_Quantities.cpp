
//---------------------------------------------------------------------------
#include <Classes.hpp>
#pragma hdrstop
#include "unt_Quantities.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)



/////////////////////////////// cls_Quantity ////////////////////////////////

//============================= Static members ==============================

//---------------------------------------------------------------------------
const struct cls_Quantity::stu_Magnitude cls_Quantity::UnderValueOrder[] =
        {
         { 1     , "unità"},
         { 1E-1  , "decimi"},
         { 1E-2  , "centesimi"},
         { 1E-3  , "millesimi"},
         { 1E-4  , "decimillesimi"},
         { 1E-5  , "centimillesimi"},
         { 1E-6  , "milionesimi"},
         { 1E-7  , "decimilionesimi"},
         { 1E-8  , "centimilionesimi"},
         { 1E-9  , "miliardesimi"},
         { 1E-10 , "decimiliardesimi"},
         { 1E-11 , "centimiliardesimi"},
         { 1E-12 , "inferiore ai centimiliardesimi"},
         { 0     , ""   } // terminator
        };
const struct cls_Quantity::stu_Magnitude cls_Quantity::OverValueOrder[] =
        {
         {    1  , "unità"},
         { 1E+1  , "decine"},
         { 1E+2  , "centinaia"},
         { 1E+3  , "migliaia"},
         { 1E+4  , "decine di migliaia"},
         { 1E+5  , "centinaia di migliaia"},
         { 1E+6  , "milioni"},
         { 1E+7  , "decine di milioni"},
         { 1E+8  , "centinaia di milioni"},
         { 1E+9  , "miliardi"},
         { 1E+10 , "decine di miliardi"},
         { 1E+11 , "centinaia di miliardi"},
         { 1E+12 , "migliaia di miliardi"},
         { 1E+13 , "decine di migliaia di miliardi"},
         { 1E+14 , "centinaia di migliaia di miliardi"},
         { 1E+15 , "milioni di miliardi"},
         { 1E+16 , "decine di milioni di miliardi"},
         { 1E+17 , "centinaia di milioni di miliardi"},
         { 1E+18 , "miliardi di miliardi"},
         { 1E+19 , "decine di miliardi di miliardi"},
         { 1E+20 , "superiore alle centinaia di miliardi di miliardi"},
         {     0 , ""   } // terminator
        };

const struct cls_Quantity::stu_Magnitude cls_Quantity::UnderPrefixes[] =
       {
        {     1 , ""  }, // unit
        //{  1E-1 , "d" }, // deci
        //{  1E-2 , "c" }, // centi
        {  1E-3 , "m" }, // milli
        {  1E-6 , "u" }, // micro
        {  1E-9 , "n" }, // nano
        { 1E-12 , "p" }, // pico
        //{ 1E-15 , "f" }, // femto
        //{ 1E-18 , "a" }, // atto
        {     0 , ""   } // null
       };

const struct cls_Quantity::stu_Magnitude cls_Quantity::OverPrefixes[] =
       {
        {     1 , ""  }, // unit
        {   1E3 , "K" }, // kilo
        {   1E6 , "M" }, // mega
        {   1E9 , "G" }, // giga
        {  1E12 , "T" }, // tera
        //{  1E15 , "P" }, // peta
        //{  1E18 , "E" }, // exa
        {     0 , ""   } // null
       };

    double cls_Quantity::pix_mks = 1; // Scale to render on screen

//============================= Public methods ==============================

//---------------------------------------------------------------------------
cls_Quantity::cls_Quantity( double v, const String l, double u )
{ // Class constructor

// . . . Initializations
    UnderUnits = UnderPrefixes;
    OverUnits = OverPrefixes;
    Init(u);  // -->UnitCoeff
    UnitLabel = l;

// . . . In the end (only when UnitCoeff is ready)
    untVal = v;
}
cls_Quantity::cls_Quantity( double v, double u, ptrMagnitude un, ptrMagnitude ov)
            : UnderUnits(un), OverUnits(ov)
{ // Constructor for specializations

// . . . Initializations
    Init(u);    // -->UnitCoeff
    SeekUnit(); // -->UnitLabel

// . . . In the end (only when UnitCoeff is ready)
    untVal = v;
}

//---------------------------------------------------------------------------
//cls_Quantity::~cls_Quantity()
//{ // Class destructor
//        //
//}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void cls_Quantity::SetRepresentation (int p, int d)
{ // Set precision and digit representation in FloatToStrF
    fltprec = p;
    fltdgts = d;
}



//=============================== Operators =================================

//============================== Properties =================================

//---------------------------------------------------------------------------
String cls_Quantity::get_modStr () const
{ // generates a string of time value with a proper unit

    int i;
    ptrMagnitude Units = GetIndex(mksVal, i, UnderUnits, OverUnits);

    if ( Units )
       {
        String UnitSuffix = " " + String(Units[i].Label);
        if ( Units==cls_Quantity::OverPrefixes || Units==cls_Quantity::UnderPrefixes) UnitSuffix += UnitLabel;
        return ::FloatToStrF(mksVal/Units[i].Value,ffGeneral,fltprec,fltdgts) + UnitSuffix;
       }
    else return "0";
}

//---------------------------------------------------------------------------
String cls_Quantity::get_desStr () const
{ // Generates a descriptive string

    int i;
    ptrMagnitude Magnitude = GetIndex(untVal, i, cls_Quantity::UnderValueOrder, cls_Quantity::OverValueOrder);
    if ( !Magnitude ) return "valore nullo";
    if ( UnitLabel.IsEmpty() ) return String(Magnitude[i].Label);
    else return String(Magnitude[i].Label) + " di " + UnitLabel;
}

//============================ Private Methods ==============================

//---------------------------------------------------------------------------
inline void cls_Quantity::Init( double u )
{ // Class constructor

// . . . Defaults
    SetRepresentation(4,3); // precision, digits

// . . . Inputs
    UnitCoeff = u;

// . . . Other initializations
    // value range
    //std::numeric_limits<double> double_info;
    //Min = -double_info.max();
    //Max =  double_info.max();
}

//---------------------------------------------------------------------------
inline cls_Quantity::ptrMagnitude cls_Quantity::GetIndex(double v, int& i, ptrMagnitude UnderArray, ptrMagnitude OverArray ) const
{ // Seek index of order of magnitude

    v = std::fabs (v); // Absolute value
    if ( mat::IsZero(v) ) return 0;
    else {
          ptrMagnitude OrderArray;
          i=0;
          if ( v > 1 )
               {
                OrderArray = OverArray;
                while ( OrderArray[i].Value>0 && v>=OrderArray[i].Value) i++;
                i--; // previous
               }
          else {
                OrderArray = UnderArray;
                while ( OrderArray[i].Value>0 && v<OrderArray[i].Value) i++;
                if ( !OrderArray[i].Value ) i--; // previous if reached end
               }
          return OrderArray;
         }
}

//---------------------------------------------------------------------------
inline void cls_Quantity::SeekUnit()
{ // Seek chosen unit (specialization only)

    int i;
    ptrMagnitude Units = GetIndex(k, i, UnderUnits, OverUnits);
    if ( Units )
       {
        mksVal *= k / Units[i].Value; // Adjust value
        k = Units[i].Value; // New unit coefficient
        UnitLabel = String(Units[i].Label);
       }
}




/////////////////////////////// cls_TimeQuantity ////////////////////////////


//============================= Static members ==============================

const struct cls_Quantity::stu_Magnitude cls_TimeQuantity::UnderUnitsArray[] =
                    {
                     {        1 , "s"        }, // seconds
                     {     1E-3 , "ms"       }, // milliseconds
                     {     1E-6 , "us"       }, // microseconds
                     {     1E-9 , "ns"       }, // nanoseconds
                     {    1E-12 , "ps"       }, // picoseconds
                     {    1E-15 , "fs"       }, // femtoseconds
                     {    1E-18 , "as"       }, // attoseconds
                     {        0 , ""         }, // null
                    };

const struct cls_Quantity::stu_Magnitude cls_TimeQuantity::OverUnitsArray[] =
                    {
                     {        1 , "s"        }, // seconds
                     {       60 , "min"      }, // minutes
                     {    36E+2 , "h"        }, // hours
                     {   864E+2 , "days"     }, // days
                     {  2592E+3 , "months"   }, // months
                     { 31536E+3 , "years"    }, // years
                     { 31536E+5 , "centuries"}, // centuries
                     {        0 , ""         }, // null
                    };


//============================= Public methods ==============================

//---------------------------------------------------------------------------
String cls_TimeQuantity::hhmmss ()
{ // Format time in a String hh:mm:ss

    int T = mat::round( mksVal );
    unsigned int h = T / 3600;
    unsigned short m = (T % 3600) / 60;
    unsigned short s = (T % 60);
    return String(h)+":"+String(m)+":"+String(s);
}



///////////////////////////////// cls_Length ////////////////////////////////



//============================= Static members ==============================

const struct cls_Quantity::stu_Magnitude cls_Length::UnderUnitsArray[] =
                    {
                     {     1 , "m"  }, // meters
                     //{  1E-1 , "dm" }, // decimeters
                     {  1E-2 , "cm" }, // centimeters
                     {  1E-3 , "mm" }, // millimeters
                     {  1E-6 , "um" }, // micrometers
                     {  1E-9 , "nm" }, // nanometers
                     { 1E-12 , "pm" }, // picometers
                     {     0 , ""   }, // null
                    };

const struct cls_Quantity::stu_Magnitude cls_Length::OverUnitsArray[] =
                    {
                     {     1 , "m"  }, // meters
                     {   1E3 , "Km" }, // kilometers
                    };



//============================= Public methods ==============================



//////////////////////////////// END OF FILE ////////////////////////////////
