#ifndef unt_FractionsH
#define unt_FractionsH
/*      ---------------------------------------------
        unt_Fractions
        2008-2011 matteo.gattanini@gmail.com

        OVERVIEW
        ---------------------------------------------
        This unit defines a class to handle rational
        numbers

        RELEASE HISTORY
        ---------------------------------------------
        0.8 (sep2011)
          .Macotec work
        0.5 (mar2008)
          .SCS work

        LICENSES
        ---------------------------------------------
        Use and modify freely

        EXAMPLE OF USAGE:
        ---------------------------------------------
        #include <cmath>
        #include "unt_Fractions.h"
        // Try fractions
        double v = std::sqrt(2);
        cls_Fraction f(v);
        String s = String(v) + " ~ " + f + " = " + String(f.Ratio()) + "\n";
        v = 12345.6789;
        f = v;
        s += String(v) + " ~ " + f + " = " + String(f.Ratio()) + "\n";
        v = 0.3333333333333333333333333;
        f = v;
        s += String(v) + " ~ " + f + " = " + String(f.Ratio()) + "\n";
        ::ShowMessage(s);

        DEPENDENCIES:
        ---------------------------------------------    */
        //#include "unt_MatCommon.h"
        #include <cmath> // std::fabs
        #include <limits>
        #ifdef __BORLANDC__
          #include <Classes.hpp> // 'String', 'TStream'
        #endif

        


/////////////////////////////////////////////////////////////////////////////
////////////////////////////// cls_Fraction /////////////////////////////////
class cls_Fraction
{
public:
// . Types
    typedef double typ_Real;
    typedef int typ_Int; // __int64

// . Public methods
    cls_Fraction(const typ_Int n =0, const typ_Int d =1) : i_N(n), i_D(d) {Simplify();}
    cls_Fraction(const typ_Real& v, const typ_Int& M =std::numeric_limits<typ_Int>::max(),const typ_Real e=2*std::numeric_limits<typ_Real>::epsilon()) { RationalApprox_ContFrac(v,M,e); }
    cls_Fraction(const cls_Fraction& f) : i_N(f.i_N), i_D(f.i_D) {;}
    //~cls_Fraction(); // Class destructor
    
    // . Main facilities
    typ_Int Numerator() const {return i_N;}
    typ_Int Denominator() const {return i_D;}
    void Numerator(typ_Int n) {i_N=n; Simplify();}
    void Denominator(typ_Int d) {i_D=d; Simplify();}

    cls_Fraction& Simplify()
       { // Reduces the fraction and fix signs
        typ_Int GCD = gcd(i_N,i_D); // (gcd will always get positive values)
        i_N /= GCD;
        i_D /= GCD;
        if(i_D<0) {i_N=-i_N; i_D=-i_D;}
        return *this;
       }

    // std::numeric_limits<typ_Real>::has_infinity
    typ_Real Ratio() const {if(i_D!=0) return typ_Real(i_N)/typ_Real(i_D); else return (i_N>0) ? std::numeric_limits<typ_Real>::infinity() : -std::numeric_limits<typ_Real>::infinity();} // Get fraction ratio

    // . Operators
    cls_Fraction operator-() const {return cls_Fraction(-i_N,i_D);}
    cls_Fraction operator+(const cls_Fraction& f) const {return cls_Fraction(i_N*f.i_D+i_D*f.i_N, i_D*f.i_D);}
    cls_Fraction operator-(const cls_Fraction& f) const {return cls_Fraction(i_N*f.i_D-i_D*f.i_N, i_D*f.i_D);}
    cls_Fraction operator*(const cls_Fraction& f) const {return cls_Fraction(i_N*f.i_N          , i_D*f.i_D);}
    cls_Fraction operator/(const cls_Fraction& f) const {return cls_Fraction(i_N*f.i_D          , i_D*f.i_N);}
    // Real numbers
    cls_Fraction operator+(const typ_Real& v) {return (*this)+cls_Fraction(v);}
    cls_Fraction operator-(const typ_Real& v) {return (*this)-cls_Fraction(v);}
    cls_Fraction operator*(const typ_Real& v) {return (*this)*cls_Fraction(v);}
    cls_Fraction operator/(const typ_Real& v) {return (*this)/cls_Fraction(v);}
    // Integer numbers
    cls_Fraction operator+(const typ_Int& v) {return cls_Fraction(v*i_D+i_N, i_D);}
    cls_Fraction operator-(const typ_Int& v) {return cls_Fraction(i_N-v*i_D, i_D);}
    cls_Fraction operator*(const typ_Int& v) {return cls_Fraction(v*i_N, i_D);}
    cls_Fraction operator/(const typ_Int& v) {return cls_Fraction(i_N, v*i_D);}
    
    // . Inport/Export
    //friend ostream& operator<<(ostream& out, const cls_Fraction& f) { out << f.Numerator() << Sep << f.Denominator(); return out;}
    //friend istream& operator>>(istream& in, cls_Fraction& f) {typ_Int n, d; char dlm; while( !(in >> n >> dlm >> n)) in.ignore(INT_MAX, '\n'); in.clear(); f=cls_Fraction(n,d); return in;}
    
    // Casts
    //operator typ_Real() const {return Ratio();} // cast to real
    #ifdef __BORLANDC__
      operator String() const {String s=String(i_N); if(i_D!=1)s+=String(Sep)+String(i_D); if(i_N<0)s="("+s+")"; return s;} // cast to String
      cls_Fraction& operator= (const String& s) {int l=s.LastDelimiter(Sep); if(l>0){i_N=s.SubString(1,l-1).ToInt(); i_D=s.SubString(l+1,s.Length()).ToInt();} else{i_N=s.ToInt(); i_D=1;} return *this;}
    #endif
    // Assignments
    cls_Fraction& operator= (const cls_Fraction& f) {if(&f!=this){i_N=f.i_N; i_D=f.i_D;} return *this;}
    cls_Fraction& operator= (const typ_Real& v) {RationalApprox_ContFrac(v,std::numeric_limits<typ_Int>::max(),2*std::numeric_limits<typ_Real>::epsilon()); return *this;}

    // . General facilities
    static typ_Int gcd(typ_Int n1, typ_Int n2)
       { // Calculates the Greatest Common Divisor of two integers
        n1 = std::abs(n1);
        n2 = std::abs(n2);
        while(true)
           {
            n1 = n1%n2; if(n1==0) return n2;
            n2 = n2%n1; if(n2==0) return n1;
           }
       }

    // . Public attributes (!)
    static const char Sep = '/';
    static const typ_Real x_max;
    static const typ_Real eps;

private:

    // . Private attributes
    typ_Int i_N, i_D; // Numerator and denominator

    // . Private methods
    void RationalApprox_ContFrac(typ_Real,const typ_Int,const typ_Real&); // Find the approssimating rational using 'continuous fraction' method

}; // end 'cls_Fraction'


//---------------------------------------------------------------------------
#endif
 
