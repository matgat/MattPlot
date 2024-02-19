//---------------------------------------------------------------------------
#include <vcl.h>
#include <math>
#pragma hdrstop

#include "unt_Polynomial_3rd.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)



//====================== Useful file scope declarations =====================

        const double epsilon = 1E-100;
        const double pi = 3.1415926535897932384626433832795;

//============================= Static members ==============================

//============================= Public methods ==============================

//---------------------------------------------------------------------------
cls_Polynomial_3rd::cls_Polynomial_3rd( double c3, double c2, double c1, double c0 )
{ // Class constructor
// . . . Coefficients
        C[3] = c3;
        C[2] = c2;
        C[1] = c1;
        C[0] = c0;

// . . . Defaults
// . . . Other initializations
}
//---------------------------------------------------------------------------
//cls_Polynomial_3rd::~cls_Polynomial_3rd()
//{ // Class destructor
//        //
//}


//---------------------------------------------------------------------------
void __fastcall cls_Polynomial_3rd::CalcRoots()
{ // Calculate polynomial roots

	if ( C[3] == 0 )
               {
                Application->MessageBox("The coefficient of the cube is 0!","cls_Polynomial_3rd",MB_OK|MB_ICONINFORMATION);
                return;
               }

	if ( C[0] == 0 )
	        Application->MessageBox("The 0-th coefficient is 0!","cls_Polynomial_3rd",MB_OK|MB_ICONINFORMATION);

	double b = C[2]/C[3];
	double c = C[1]/C[3];
	double d = C[0]/C[3];
	
	double q = (3.0*c - (b*b))/9.0;
	double r = -(27.0*d) + b*(9.0*c - 2.0*(b*b));
	r /= 54.0;
	double D = q*q*q + r*r; // Discriminant
        double term1 = (b/3.0);

        if ( D < 0 )
               { // all roots are real and unequal
		q = -q;
		double dum1 = q*q*q;
		dum1 = acos(r/sqrt(dum1));
		double r13 = 2.0*sqrt(q);
		roots[0] = -term1 + r13*cos(dum1/3.0);
		roots[1] = -term1 + r13*cos((dum1 + 2.0*pi)/3.0);
		roots[2] = -term1 + r13*cos((dum1 + 4.0*pi)/3.0);
               }
        else Application->MessageBox("Imaginary roots not yet supported!","cls_Polynomial_3rd",MB_OK|MB_ICONINFORMATION);

/*
	double p = (b - a*a/3) / 3;
	double q = ((a/3)*(b - a*a*2/9) - c) / 2;
	double p3 = p*p*p;
	double D = q*q + p3; // Discriminant

        if ( D < 0 )
               { // all roots are real and unequal
                double disp = - a/3;
                double rho = sqrt(-p3);

                double theta = acos(-q/rho)/3;
                //double theta = atan2(sqrt(-D),q);
                //if ( q<0 ) theta += pi; // II - III quadrante

		roots[0] = disp + rho * cos(theta/3.0);
		roots[1] = disp + rho * cos((theta + 2*pi)/3.0);
		roots[2] = disp + rho * cos((theta + 4*pi)/3.0);
               }
        else Application->MessageBox("Imaginary roots not yet supported!","cls_Polynomial_3rd",MB_OK|MB_ICONINFORMATION);
*/

/*
	if (discrim > 0) { // one root real, two are complex
	 s = r + Math.sqrt(discrim);
	 s = ((s < 0) ? -Math.pow(-s, (1.0/3.0)) : Math.pow(s, (1.0/3.0)));
	 t = r - Math.sqrt(discrim);
	 t = ((t < 0) ? -Math.pow(-t, (1.0/3.0)) : Math.pow(t, (1.0/3.0)));
	 dataForm.x1Re.value = -term1 + s + t;
	 term1 += (s + t)/2.0;
	 dataForm.x3Re.value = dataForm.x2Re.value = -term1;
	 term1 = Math.sqrt(3.0)*(-t + s)/2;
	 dataForm.x2Im.value = term1;
	 dataForm.x3Im.value = -term1;
	 return;
	} // End if (discrim > 0)

	// The remaining options are all real
	dataForm.x3Im.value = dataForm.x2Im.value = 0;

	if (discrim == 0){ // All roots real, at least two are equal.
	 r13 = ((r < 0) ? -Math.pow(-r,(1.0/3.0)) : Math.pow(r,(1.0/3.0)));
	 dataForm.x1Re.value = -term1 + 2.0*r13;
	 dataForm.x3Re.value = dataForm.x2Re.value = -(r13 + term1);
	 return;
	} // End if (discrim == 0)
*/
}


//=============================== Operators =================================

//============================== Properties =================================

//---------------------------------------------------------------------------
double __fastcall cls_Polynomial_3rd::get_discriminant(void) const
{ // Calculate polynomial discriminant
        Application->MessageBox("Calculate polynomial discriminant not yet supported!","cls_Polynomial_3rd",MB_OK|MB_ICONINFORMATION);
        return 0;
}

//================================ Events ===================================

//============================ Private Methods ==============================

//============================== END OF FILE ================================
