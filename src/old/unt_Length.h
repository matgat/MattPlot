//---------------------------------------------------------------------------
#ifndef unt_LengthH
#define unt_LengthH
//---------------------------------------------------------------------------
#include "unt_Value.h"

        /* * * * * * * * * * * * * * * * * * * * * * * * * * * *
        *
        *                cls_Length
        *        2006 - Matteo Gattanini
        *
        *        unt_Length (mar.2006)
        *                ver: 01    (11.mar.2008)
        *
        *        This unit defines a class that represents
        *        a length (double) with unit
        *
        *        Example of usage:
        *
        *        #include "unt_Length.h"
        *       , visualized with 4 pix/mm
        *        cls_Length l = cls_Length(33,1E-3); // Create a length of 33 mm 
        *        l.SetScreenScale(20/3); // twenty pixels each three length units
        *        
        *        
        *
        * * * * * * * * * * * * * * * * * * * * * * * * * * * */


//========================== Class Definition ===============================
class cls_Length : public cls_Quantity {

public://                                                        .............

        cls_Length( double=0, double=1 );
        //~cls_Length();

// . . . Public methods


// . . . Operators (già in cls_Quantity)
        // Assegnmento
        cls_Length& operator= (const cls_Length& v)  {
                                                    if (this == &v) return *this; // Handle self assignment
                                                    k = v.k;
                                                    i_UnitLabel = v.i_UnitLabel;
                                                    mksVal = v.mksVal;
                                                    return *this;
                                                   }

        // Assegnamento e cast di `double' : un numero è inteso come uniformato mks
        cls_Length& operator= ( double v ) { mksVal = v; return *this;}
        operator double() const {return mksVal;}

        // Assegnamento e cast di `String'
        cls_Length& operator= ( String s ) { StrVal = s; return *this;}
        operator String() const {return StrVal;}

// . . . Properties
        __property String StrRep = { read = get_StrRep }; // string representation with proper unit
        __property int pix = { read = get_pix, write = set_pix }; // MKS value

// . . . Public attributes (!)
        static double pix_u; // pixels per units (to screen scale)

private://                                                        .............

// . . . Private attributes
        static const struct stu_Quantity LengthUnits[];

// . . . Properties
        String __fastcall get_StrRep (void) const;
        int __fastcall get_pix (void) const {return Val*pix_u;}
        void __fastcall set_pix (const int newpix) { Val = newpix/pix_u; }
// . . . Private methods

};//.........................................end declaration of cls_Length


//---------------------------------------------------------------------------
#endif
