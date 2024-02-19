//---------------------------------------------------------------------------
#include <vcl.h>
#include "unt_MatCommon.h"
#pragma hdrstop
#include "unt_MatLogo.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


//====================== Useful file scope declarations =====================


//-------------------------- Types definition -------------------------------
        enum enu_Operation {MOVE, LINE, ARCV, ARCX, FILL};
        struct stu_PolarRelativeOp{double r; // Distance in pixels
                                   double a; // Angle in degrees
                                   int idx; // Start point index
                                   enu_Operation op;
                                   };
//---------------------------------------------------------------------------
                
	/* Logo shape
  M1.._  M2      M4    _, M5
    \  `--.      ,.--''/
     \     `.  ,'     /
      \      `'      /
       \     M3     /
   M14 /            , M6
      /   M11    M9  \
     /     /\ ,'\     \
    /     /  `   \     \
   /     .'  M10 '.     \
M13'--._ |        `.,--'' M7
        ''M12      M8    */


        const int LogoWidth = 600;
        const int LogoHeight = 500;

    /* From M1
	const stu_PolarRelativeOp DrawOp[] =
		{
         { 100  , -30 ,  0, MOVE }, // ( 1)  M1
		 { 155  , -20 ,  1, LINE }, // ( 2)  M1 -> M2
		 { 130  , -60 ,  2, ARCX }, // ( 3)  M2 -> M3
		 { 130  ,  60 ,  3, ARCX }, // ( 4)  M3 -> M4
		 { 155  ,  20 ,  4, LINE }, // ( 5)  M4 -> M5
         { 180  ,-100 ,  5, ARCV }, // ( 6)  M5 -> M6
         { 230  , -80 ,  6, ARCV }, // ( 7)  M6 -> M7
         { 100  ,-150 ,  7, ARCV }, // ( 8)  M7 -> M8
         { 280  , 100 ,  8, ARCV }, // ( 9)  M8 -> M9
         { 168  ,-120 ,  9, ARCV }, // (10)  M9 -> M10
         { 168  , 120 , 10, ARCV }, // (11)  M10-> M11
         { 280  ,-100 , 11, ARCV }, // (12)  M11-> M12
         { 100  , 150 , 12, ARCV }, // (13)  M12-> M13
         { 230  ,  80 , 13, ARCV }, // (14)  M13-> M14
         {   0  ,   0 ,  1, ARCV }, // (15)  M14-> M1
         {  20  , -92 ,  2, FILL }  // Fill M
		};     */
    const stu_PolarRelativeOp DrawOp[] =
		{
         { 480  , -51 ,  0, MOVE }, // M10
         { 168  , 120 ,  1, ARCV }, // M10-> M11
         { 280  ,-100 ,  2, ARCV }, // M11-> M12
         { 100  , 150 ,  3, ARCV }, // M12-> M13
         { 230  ,  80 ,  4, ARCV }, // M13-> M14
         { 180  , 100 ,  5, ARCV }, // M14-> M1
         { 155  , -20 ,  6, LINE }, // M1 -> M2
		 { 130  , -60 ,  7, ARCX }, // M2 -> M3
		 { 130  ,  60 ,  8, ARCX }, // M3 -> M4
		 { 155  ,  20 ,  9, LINE }, // M4 -> M5
         { 180  ,-100 , 10, ARCV }, // M5 -> M6
         { 230  , -80 , 11, ARCV }, // M6 -> M7
         { 100  ,-150 , 12, ARCV }, // M7 -> M8
         { 280  , 100 , 13, ARCV }, // M8 -> M9
         {   0  ,   0 ,  1, ARCV }, // M9 -> M10
         {  20  , -90 ,  8, FILL }  // Fill M
		};

	const int N = sizeof(DrawOp) / sizeof(DrawOp[0]);

        // Cartesian pixel points (respect canvas origin)
        std::vector <TPoint> Points (N); // Points stack
        TPoint P;

//---------------------------------------------------------------------------




//============================== Public methods==============================



//---------------------------------------------------------------------------
__fastcall cls_MatLogo::cls_MatLogo()
{ // Contructor
// . . . Defaults
    SetColors( TColor(0xE8E3D0), TColor(0xE8E3D0) );
    BorderThickness = 5;
}

//---------------------------------------------------------------------------
void __fastcall cls_MatLogo::Draw ( class TPaintBox* board )
{ // Draws logo in a canvas

    // Rescale dimensions to fit in canvas
    Fit(
        board->Width,
        board->Height,
        board->Width/20, // left margin
        board->Height/20 // top margin
        );

    // Prepare style
    LogoColor.H += 5.123;

    //BorderColor += mat::GetRandomNumber(0xF); // A random color
    //static int DeltaColor = 3;
    //static int prevR;
    //prevR = GetRValue(BorderColor);
    //if ( (prevR+DeltaColor)<=0 || (prevR+DeltaColor)>=0xFF ) DeltaColor = -DeltaColor;
    //BorderColor += DeltaColor;

    board->Canvas->Brush->Style = bsSolid;
    board->Canvas->Brush->Color = TColor(LogoColor);
    board->Canvas->Pen->Style = psSolid;
    board->Canvas->Pen->Color = TColor(LogoColor);
    board->Canvas->Pen->Width = BorderThickness;

// . . . Draw

    for ( int i=0; i<N; i++) // S clockwise
        {
         switch ( DrawOp[i].op )
           {
            case MOVE : {
                         Points.push_back( AddPoints(Points.at(DrawOp[i].idx),PolarToCart(DrawOp[i])) );
                         board->Canvas->MoveTo(Points.back().x, Points.back().y);
                        }; break;
            case LINE : {
                         Points.push_back( AddPoints(Points.at(DrawOp[i].idx),PolarToCart(DrawOp[i])) );
                         if ( i>=N-2 ) P = Points.back();
                         else P = AddRandom(Points.back());
                         board->Canvas->LineTo(P.x, P.y);
                        }; break;
            case ARCV : { // Concave arc
                         TPoint Pprev = board->Canvas->PenPos;
                         Points.push_back( AddPoints(Points.at(DrawOp[i].idx),PolarToCart(DrawOp[i])) );
                         if ( i>=N-2 ) P = Points.back();
                         else P = AddRandom(Points.back());
                         int Dx = (P.x-Pprev.x);
                         int Dy = (P.y-Pprev.y);
                         if ( (Dx>0&&Dy>0) || (Dx<0&&Dy<0) ) board->Canvas->Arc(Pprev.x,Pprev.y-Dy,P.x+Dx,P.y,Pprev.x,Pprev.y,P.x,P.y);
                         else board->Canvas->Arc(Pprev.x-Dx,Pprev.y,P.x,P.y+Dy,Pprev.x,Pprev.y,P.x,P.y);
                         board->Canvas->MoveTo(P.x, P.y);
                        }; break;
            case ARCX : { // Convex arc
                         TPoint Pprev = board->Canvas->PenPos;
                         Points.push_back( AddPoints(Points.at(DrawOp[i].idx),PolarToCart(DrawOp[i])) );
                         if ( i>=N-2 ) P = Points.back();
                         else P = AddRandom(Points.back());
                         int Dx = (P.x-Pprev.x);
                         int Dy = (P.y-Pprev.y);
                         if ( (Dx>0&&Dy>0) || (Dx<0&&Dy<0) ) board->Canvas->Arc(Pprev.x-Dx,Pprev.y,P.x,P.y+Dy,P.x,Points.back().y,Pprev.x,Pprev.y);
                         else board->Canvas->Arc(Pprev.x,Pprev.y-Dy,P.x+Dx,P.y,P.x,P.y,Pprev.x,Pprev.y);
                         board->Canvas->MoveTo(P.x, P.y);
                        }; break;
            case FILL : {
                         TPoint Pfill = AddPoints(Points.at(DrawOp[i].idx),PolarToCart(DrawOp[i]));
                         board->Canvas->FloodFill(Pfill.x,Pfill.y,TColor(LogoColor),fsBorder);
                         //board->Canvas->LineTo(Pfill.x, Pfill.y);
                        }; break;
           }
        }
}

//---------------------------------------------------------------------------
void cls_MatLogo::SetColors( TColor logocl, TColor bordercl )
{ // Set logo and logoborder colors
    LogoColor = logocl;
    BorderColor = bordercl;
}




//============================= Private methods =============================


//---------------------------------------------------------------------------
void cls_MatLogo::Fit( const int W, const int H, const int LeftMargin, const int TopMargin )
{ // Initialize Scale and starting point
	// Scale
		Scale = mat::min( double(W - 2*LeftMargin)/LogoWidth,
						  double(H - 2*TopMargin)/LogoHeight );
	// Starting point
        Points.clear();
        Points.push_back( TPoint(LeftMargin,TopMargin) );
}


//---------------------------------------------------------------------------
inline TPoint cls_MatLogo::AddPoints( const TPoint& Pp, const TPoint& dP ) const
{
        return TPoint(Pp.x + dP.x, Pp.y + dP.y);
}

//---------------------------------------------------------------------------
inline TPoint cls_MatLogo::PolarToCart( const struct stu_PolarRelativeOp& D )
{
        double angle = -D.a * 0.017453292519943295769236907684886;

		return TPoint( mat::round(Scale*D.r*std::cos(angle)),
                       mat::round(Scale*D.r*std::sin(angle)) );
}

//---------------------------------------------------------------------------
inline TPoint cls_MatLogo::AddRandom( const TPoint& Pp )
{ // Add a random factor
        static int r = 10;
        int DeltaX = mat::GetRandomNumber(r,-r);
        int DeltaY = mat::GetRandomNumber(r,-r);
        return TPoint(Pp.x + DeltaX, Pp.y + DeltaY);
}


//============================== END OF FILE ================================




