//---------------------------------------------------------------------------
//#include <vcl.h>
#include "unt_MatCommon.h"
#pragma hdrstop
#include "unt_TimeQuantity.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------


//====================== Useful file scope declarations =====================

//============================= Static members ==============================

       const stu_Quantity cls_TimeQuantity::UnderUnitsArray[] =
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

       const stu_Quantity cls_TimeQuantity::OverUnitsArray[] =
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
String __fastcall cls_TimeQuantity::hhmmss ()
{ // Format time in a String hh:mm:ss

        int T = mat::round( mksVal );
        unsigned int hou = T / 3600;
        unsigned short min = (T % 3600) / 60;
        unsigned short sec = (T % 60);
        return String(hou)+":"+String(min)+":"+String(sec);
}



//=============================== Operators =================================

//=============================== Properties ================================

//============================ Private Methods ==============================

//============================== END OF FILE ================================
