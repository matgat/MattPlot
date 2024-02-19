//---------------------------------------------------------------------------
#ifndef unt_TimeQuantityH
#define unt_TimeQuantityH
//---------------------------------------------------------------------------
#include "unt_Quantity.h"

        /* * * * * * * * * * * * * * * * * * * * * * * * * * * *
        *
        *                cls_TimeQuantity
        *        2006 - Matteo Gattanini
        *
        *        unt_TimeQuantity (mar.2006)
        *                ver: 02    (10.nov.2008)
        *
        *        This unit defines a class that represents
        *        a time quantity (double) with unit
        *
        *        Example of usage:
        *
        *        #include "unt_TimeQuantity.h"
        *        cls_TimeQuantity t;
        *        
        *        
        *        
        *
        * * * * * * * * * * * * * * * * * * * * * * * * * * * */



//========================== Class Definition ===============================
class cls_TimeQuantity : public cls_Quantity {

public://                                                        .............

        cls_TimeQuantity(double t =0, double u =1):cls_Quantity(t,u,cls_TimeQuantity::UnderUnitsArray,cls_TimeQuantity::OverUnitsArray){;}
        //~cls_TimeQuantity();

// . . . Public methods
        String __fastcall hhmmss();

private://                                                        .............

// . . . Private attributes
        static const struct stu_Quantity UnderUnitsArray[];
        static const struct stu_Quantity OverUnitsArray[];

// . . . Properties

// . . . Private methods

};//.........................................end declaration of cls_TimeQuantity


//---------------------------------------------------------------------------
#endif
