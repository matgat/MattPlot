//---------------------------------------------------------------------------
#ifndef unt_MatLogoH
#define unt_MatLogoH
//---------------------------------------------------------------------------

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
	*
	*	        cls_MatLogo
	*	2006 - Matteo Gattanini
	*
	*	unt_Value (mar.2006)
	*		ver: 02    (25.apr.2008)
	*
	*	This unit defines a class that draws
	*	the SCS logo in a TPaintBox
	*
	*	Example of usage:
	*
	*	#include "unt_MatLogo.h"
	*	cls_MatLogo logo;
	*	logo.Draw(PaintBox1);
	*
	*
	*
	* * * * * * * * * * * * * * * * * * * * * * * * * * * */


//-------------------------- Types definition -------------------------------



//--------------------------- Class Definition ------------------------------
class cls_MatLogo {

public://							.............

// . . . Public methods
        __fastcall cls_MatLogo ();

        void __fastcall Draw( class TPaintBox* );
        void SetColors( TColor, TColor );
        String getColor() const {return LogoColor;}

// . . . Public attributes (!)
        double Scale; // [pix/unit]
        int BorderThickness;

private://							.............

// . . . Private attributes
        TColor BorderColor;
        mcl::cls_Color LogoColor;

// . . . Private methods
        void Fit ( const int, const int, const int, const int ); // Scale to fit
        inline TPoint AddPoints( const TPoint&, const TPoint& ) const;
        inline TPoint PolarToCart( const struct stu_PolarRelativeOp& );
        static inline TPoint AddRandom( const TPoint& );

};//-------------------------------------------------------------------------

#endif
