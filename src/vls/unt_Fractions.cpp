//#include <vcl.h>
#pragma hdrstop
#include "unt_Fractions.h" // 'cls_Fraction'
//---------------------------------------------------------------------------
#pragma package(smart_init)



//============================= Static members ==============================
    const cls_Fraction::typ_Real cls_Fraction::x_max = static_cast<cls_Fraction::typ_Real>(std::numeric_limits<cls_Fraction::typ_Int>::max());
    const cls_Fraction::typ_Real cls_Fraction::eps = std::numeric_limits<cls_Fraction::typ_Real>::min();


//============================= Private methods =============================


//---------------------------------------------------------------------------
// Find the approssimating rational using 'continuous fraction' method
void cls_Fraction::RationalApprox_ContFrac(typ_Real x, const typ_Int M, const typ_Real& e)
{
    bool neg = x<0;
    x = std::fabs(x);
    typ_Int ak, Nk=1, Dk=0, Nk_1=0, Dk_1=1, Nk_2, Dk_2;
    typ_Real xki, xkf=x, ek=2*x, ek_1;
    do {
        // . Separate integer and fractional parts
        xkf = std::modf(xkf, &xki);
        if(xki<=x_max) ak = xki; // Continuous fraction expansion term
        else throw xki; // too big

        // .Calculate convergents
        Nk_2 = Nk_1;             Dk_2 = Dk_1;
        Nk_1 = Nk;               Dk_1 = Dk;
        Nk = ak*Nk_1 + Nk_2;     Dk = ak*Dk_1 + Dk_2;
        // If outside maximum, roll back and stop
        if(Nk>M || Dk>M) {
                          Nk = Nk_1;
                          Dk = Dk_1;
                          break;
                         }
        // . Check approximation
        ek_1 = ek;
        ek = std::fabs(x-(typ_Real(Nk)/typ_Real(Dk)));
        // If error isn't decreasing, roll back and stop
        if (ek >= ek_1) {
                         Nk = Nk_1;
                         Dk = Dk_1;
                         break;
                        }
        // . Next term of continuous fraction expansion
        if(xkf<=eps) break; // TODO: truncate in a better way
        xkf = 1/xkf;
       }
    while (ek > e);

    // Finally
    i_N = neg ? -Nk : Nk;
    i_D = Dk;
} // end 'RationalApprox_ContFrac'


/*
//---------------------------------------------------------------------------
// Find the approssimating rational using 'Farey sequence' method
void cls_Fraction::RationalApprox_Farey(typ_Real x, const typ_Int M, const typ_Real& e)
{
    // . Prepare
    bool neg = x<0;
    x = std::fabs(x);
    if(x>x_max) throw x; // x too big
    typ_Int N1 = static_cast<typ_Int>(x);
    typ_Int N2 = N1+1;
    typ_Int D1=1, D2=1;
    typ_Int N3=N1, D3=D1;
    typ_Real v;

    // . Iterative approximation
    do {
        typ_Int Ntmp = N1 + N2;
        typ_Int Dtmp = D1 + D2;
        if(Ntmp>M || Dtmp>M) {i_N=neg?-N3:N3; i_D=D3; break;}
        else {N3=Ntmp; D3=Dtmp;}
        v = typ_Real(N3)/typ_Real(D3);
        if(std::fabs(x-v)<e) {i_N=neg?-N3:N3; i_D=D3; break;}
        else {if(v<x){N1=N3; D1=D3;} else{N2=N3; D2=D3;}}
       }
    while(true);
} // end 'RationalApprox_Farey'

*/

