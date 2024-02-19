//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
#include "unt_Length.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------



//======================= Useful file scope declarations ====================

	const double epsilon = 1E-100;




//============================= Static members ==============================

	const stu_Quantity cls_Length::LengthUnits[] =
	{
			 {   1E3 , "Km" }, // kilometers
                         {     1 , "m"  }, // meters
                         {  1E-1 , "dm" }, // decimeters
                         {  1E-2 , "cm" }, // centimeters
                         {  1E-3 , "mm" }, // millimeters
                         {  1E-6 , "um" }, // micrometers
                         {  1E-9 , "nm" }, // nanometers
                         { 1E-12 , "pm" }, // picometers
                         {     0 , ""   }, // null
			};
	//const int Units_size = sizeof(Units) / sizeof(Units[0]);

        double cls_Length::pix_u = 1;




//============================= Public methods ==============================

//---------------------------------------------------------------------------
cls_Length::cls_Length(double l, double ucoeff) : cls_Quantity (l, ucoeff)
{ // Class constructor

// . . . Initializations and defaults
        Units = LengthUnits;
// . . . Other things
        SetUnit (ucoeff);
// . . . Other initializations
}

//---------------------------------------------------------------------------
//cls_Length::~cls_Length()
//{ // Class destructor
//        //
//}
//---------------------------------------------------------------------------



//=============================== Operators =================================
//=============================== Properties ================================

String __fastcall cls_Length::get_StrRep (void) const { return ToProperUnit();}

//=============================== Events ====================================
//============================ Private Methods ==============================

//============================== END OF FILE ================================
