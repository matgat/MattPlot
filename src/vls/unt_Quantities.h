//---------------------------------------------------------------------------
#ifndef unt_QuantitiesH
#define unt_QuantitiesH
//---------------------------------------------------------------------------

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * *
    *
    *                unt_Quantities
    *        2006 - Matteo Gattanini
    *
    *        unt_Quantities (nov.2006)
    *                ver: 012    (26.nov.2006)
    *
    *       This unit defines some classes to represent
    *       physical scalar quantities (value+unit)
    *       cls_Quantity
    *                   \ cls_TimeQuantity
    *                   | cls_Length
    *
    *        Example of usage:
    *
    *       #include "unt_Quantities.h"
    *       cls_Quantity v = cls_Quantity(23.4,"mm/s");// a speed
    *       v *= 32.5;
    *       ShowMessage(v);
    *
    *       cls_Length L = cls_Length(33,1E-3); // a length of 33 mm 
    *       L.ScreenScale = 20/(3*L.UnitCoeff); // twenty pixels each three length units
    *       Shape1->Width = L.pix; // Render length on screen
    *
    * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "unt_MatCommon.h"

//========================== Class Definition ===============================
class cls_Quantity {

public://                                                       .............

// ~~~~~~~~~~~~~ Types definitions				~~~~~~~~~~~~~
    struct stu_Magnitude { double Value; char Label[64]; };
    typedef const struct stu_Magnitude* ptrMagnitude;

// . . . Public methods
    cls_Quantity( double =0, const String ="", double =1 );
    cls_Quantity( double, double, ptrMagnitude, ptrMagnitude );
    //~cls_Quantity();

    void SetRepresentation (int, int); // String representation digits

// . . . Operators

    cls_Quantity& operator= (const cls_Quantity& v)
        { // Assegnamento
            if (this == &v) return *this; // Handle self assignment
            k = v.k;
            unt = v.unt;
            mksVal = v.mksVal;
            return *this;
        }
    // Assegnamento e cast di `double' : un numero è inteso come uniformato mks
    cls_Quantity& operator= ( double v ) { mksVal = v; return *this;}
    operator double() const {return mksVal;}

    // Assegnamento e cast di `String'
    cls_Quantity& operator= ( const String& s ) { untStr = s; return *this;}
    operator String() const { return modStr; }

    // Operazioni
    double operator+ (cls_Quantity& v) {return mksVal+v.mksVal;}
    double operator- (cls_Quantity& v) {return mksVal-v.mksVal;}
    double operator* (cls_Quantity& v) {return mksVal*v.mksVal;}
    double operator/ (cls_Quantity& v) {return mksVal/v.mksVal;}

    //cls_Quantity& operator+ (cls_Quantity& v) {return cls_Quantity(mksVal+v.mksVal,k,unt);}
    //cls_Quantity& operator- (cls_Quantity& v) {return cls_Quantity(mksVal-v.mksVal,k,unt);}

// . . . Properties
    __property double mksVal = { read=i_mksVal, write=i_mksVal }; // Value in MKS system
    __property double UnitCoeff = { read=get_UnitCoeff, write=set_UnitCoeff }; // unit coefficient to MKS value
    __property String UnitLabel = { read=get_UnitLabel, write=unt }; // unit label

    __property double untVal = { read=get_untVal, write=set_untVal }; // Value in units
    __property String untStr = { read=get_untStr, write=set_untStr }; // String in units
    __property double modStr = { read=get_modStr }; // String in proper units
    __property double desStr = { read=get_desStr }; // String quantity description

    __property int pix = { read = get_pix, write = set_pix }; // screen pixels
    __property double ScreenScale = { read = pix_mks, write = set_pix_mks }; // screen pixels per meter

// . . . Public attributes (!)

protected://                                                    .............

// . . . Protected attributes
    ptrMagnitude OverUnits;
    ptrMagnitude UnderUnits;

    int fltprec,fltdgts; // precision and digits of displayed numbers

// . . . Properties
    double i_mksVal; // Il valore memorizzato nel sistema mks, usato per i calcoli
    double k; // To MKS value coefficient [mks/unit]
    String unt; // Unit string

    double get_UnitCoeff () const { return k; }
    void set_UnitCoeff (double u) { k = std::fabs(u); }
    String get_UnitLabel () const { return unt.Trim(); }

    double get_untVal () const {return mksVal/k;}
    void set_untVal (const double unit_val) { mksVal = unit_val * k; }
    String get_untStr () const { return FloatToStrF(untVal,ffGeneral,fltprec,fltdgts) + " " + unt;}
    void set_untStr (const String& unit_str)
        { // TODO : get unit
            untVal = unit_str.ToDouble();
        }
    String get_modStr () const;
    String get_desStr () const;

    int get_pix () const { return mksVal*pix_mks; }
    void set_pix (const int px) { mksVal = px/pix_mks; }
    void set_pix_mks (const double pu) { pix_mks = mat::EnsureNotZero(pu); }

// . . . Protected methods

private://                                                      .............

// . . . Private attributes
    static const struct stu_Magnitude UnderValueOrder[];
    static const struct stu_Magnitude OverValueOrder[];
    static const struct stu_Magnitude UnderPrefixes[];
    static const struct stu_Magnitude OverPrefixes[];
    static double pix_mks; // pixels per mks unit (screen scale)

// . . . Private methods
    inline ptrMagnitude GetIndex(double, int&, ptrMagnitude, ptrMagnitude) const; // Seek index of order of magnitude
    inline void SeekUnit(); // Set unit, seeking unit labels (specializations)
    inline void Init( double ); // Shared code in constructors

};// . . . end declaration of cls_Quantity




//========================== Class Definition ===============================
class cls_TimeQuantity : public cls_Quantity {

public://                                                       .............

// . . . Public methods
    cls_TimeQuantity(double t =0, double u =1):cls_Quantity(t,u,cls_TimeQuantity::UnderUnitsArray,cls_TimeQuantity::OverUnitsArray){;}
    //~cls_TimeQuantity();

    String hhmmss();

private://                                                       .............

// . . . Private attributes
    static const struct stu_Magnitude UnderUnitsArray[];
    static const struct stu_Magnitude OverUnitsArray[];

};// . . . end declaration of cls_TimeQuantity



//========================== Class Definition ===============================
class cls_Length : public cls_Quantity {

public://                                                       .............

    cls_Length(double l =0, double u =1):cls_Quantity(l,u,cls_Length::UnderUnitsArray,cls_Length::OverUnitsArray){;}
    //~cls_Length();

// . . . Public methods

// . . . Properties

// . . . Public attributes (!)

private://                                                      .............

// . . . Private attributes
    static const struct stu_Magnitude UnderUnitsArray[];
    static const struct stu_Magnitude OverUnitsArray[];

// . . . Properties

// . . . Private methods

};// . . . end declaration of cls_Length



//---------------------------------------------------------------------------
#endif
