//---------------------------------------------------------------------------
#include <vcl.h>
#include "unt_MatCommon.h"
#pragma hdrstop

#include "unt_MatPlot.h"
#include "unt_Tracks.h"
#include "unt_Frame.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------



//========================== File scope variables ===========================
#define FRM_DEBUG //###

//============================= Static Members ==============================
        const String cls_Frame::Name = "cls_Frame";
        int cls_Frame::Number = 0; // Number of instantiated frames

        // Arrow dimension [pixels]       //    |
        int cls_Frame::ArrowLength = 7;   // ===  >
        int cls_Frame::ArrowWing = 3;     //    |

        // Default mapped space
        const double cls_Frame::xmin_def = 0.1;
        const double cls_Frame::xrange_def = 100;
        const double cls_Frame::ymin_def = 0.1;
        const double cls_Frame::yrange_def = 100;


//============================= Public methods ==============================


//---------------------------------------------------------------------------
cls_Frame::cls_Frame(class cls_MatPlot* parent, int brd, TColor col) : Parent(parent)
{ // Costruttore

// . . . Inputs
        Margin = brd;

// . . . Valori di default / stato iniziale
        // Properties
        i_SelectedTrack = 0;
        DrawGrid = true;
        DrawLegend = true;
        VGridDist = 100;
        HGridDist = 100;
        YLogarithmic = false;
        XLogarithmic = false;
        // Colors
        BackgroundColor = col;
        BorderColor = TColor(mat::Darken(BackgroundColor,20)); // Border default
        AxesColor = clBlack;
        GridColor = BorderColor;
        LabelColor = clBlack;
        // Inner dimensions
        top = 1;
        left = 1;
        width = 16;
        height = 16;
        // Set mapped space and space transformation
        SetMappedX (xmin_def, xrange_def);
        SetMappedY (ymin_def, yrange_def);

// . . . Altre operazioni
        Tag = Number;
        ++Number;
}

//---------------------------------------------------------------------------
//cls_Frame::~cls_Frame()
//{ // Destructor
//        --Number;
//}


//----------------------------- Drawing services ----------------------------

//---------------------------------------------------------------------------
void __fastcall cls_Frame::Plot (TCanvas* Canvas)
{ // Operazioni di disegno

// . . . Pen style
        Canvas->Pen->Mode = pmCopy;
        Canvas->Pen->Style = psSolid;
        Canvas->Pen->Width = 1;

// . . . Background and border
        Canvas->Brush->Style = bsSolid;
        Canvas->Brush->Color = BackgroundColor;
        Canvas->Pen->Color = BorderColor;
        //Canvas->FillRect(BoundsRect);
        //Canvas->FrameRect(BoundsRect);
        Canvas->Rectangle(BoundsRect);

        #ifdef FRM_DEBUG
          Canvas->Font->Name = Parent->TextFontName;
          Canvas->Font->Size = Parent->TextFontSize;
          int px = left+10;
          int py = top+10;
          Canvas->Font->Style = TFontStyles() << fsBold;
          Canvas->TextOut(px,py,String("Frame: ") + String(Tag) + String("  Row: ") + String(Row));
          Canvas->Font->Style = TFontStyles() >> fsBold;
          Canvas->TextOut(px,py+20,"Top: " + String(Top) + "  Left: " + String(Left) + "  Width: " + String(Width) + "  Height: " + String(Height));
          Canvas->TextOut(px,py+40,"Kx: " + String(Kx) + " Xo: " + String(Xo) + " mapped x: " + String(width/Kx) + " xm: " + String(Xphy(left)) + " xM: " + String(Xphy(right)));
          Canvas->TextOut(px,py+60,"Ky: " + String(Ky) + " Yo: " + String(Yo) + " mapped y: " + String(height/Ky) + " ym: " + String(Yphy(bottom)) + " yM: " + String(Yphy(top)));
          Canvas->TextOut(px,py+80,"xrange: " + String(xrange) + " xmin: " + String(xmin) + " xmax: " + String(xmin+xrange) );
          Canvas->TextOut(px,py+100,"yrange: " + String(yrange) + " ymin: " + String(ymin) + " ymax: " + String(ymin+yrange) );
          if (SelectedTrack) Canvas->TextOut(px,py+120,"track: " + SelectedTrack->Name);

          Canvas->Brush->Style = bsClear;
          Canvas->Pen->Color = clYellow;
          Canvas->Pen->Mode = pmCopy;
          Canvas->Rectangle(boundsRect);
          Canvas->MoveTo(Left,Top);
          Canvas->Pen->Color = clRed;
          Canvas->LineTo(Left + Width, Top + Height);
          Canvas->Pen->Color = clGreen;
          Canvas->MoveTo(Left + Width, Top);
          Canvas->LineTo(Left, Top + Height);
          Canvas->LineTo(left, bottom);
          Canvas->TextOut(Canvas->PenPos.x,Canvas->PenPos.y,String(Canvas->PenPos.x) + "," + String(Canvas->PenPos.y));
        #endif

// . . . Detect Frame pixels dimension modification
        static int w = 0;
        if ( w!=width ) // Frame width is changed
           {
            // Recalculate abscissa scale
            RecalculateTransformX();

            // Recalculate vertical grid

            // Store new dimension
            w = width;
           }

        // ordinate
        static int h = 0;
        if ( h != height) // Frame height is changed
           {
            // Recalculate ordinate scale
            RecalculateTransformY();

            // Recalculate horizontal grid

            // Store new dimension
            h = height;
           }

// . . . Grid lines and axes
        DrawTheAxes(Canvas);
        if ( DrawGrid ) DrawTheGrid(Canvas);
        if ( DrawLegend ) DrawTheLegend(Canvas);

// . . . Plot owned tracks
        for ( tr=Tracks.begin(); tr!=Tracks.end(); ++tr ) (*tr)->Plot(this,Canvas);
}

//---------------------------------------------------------------------------
inline void cls_Frame::DrawTheGrid (TCanvas* Canvas)
{ // Draw grid

// . . . Set pen for grid
        Canvas->Pen->Color = GridColor;
        Canvas->Pen->Mode = pmCopy;
        Canvas->Pen->Style = psDot;
        Canvas->Pen->Width = 1;
        Canvas->Pen->Color = LabelColor;
        Canvas->Font->Name = Parent->LabelFontName;
        Canvas->Font->Size = Parent->LabelFontSize;

/*
// . . . Grid lines
        // . . vertical
	ngr=0; // temp coordinates and counter
	lgr = lgrmax = floor( (LeadingTrack->Xmax-LeadingTrack->Xmin) / DVgr) * DVgr; // max grid line value
	Lgr = O.x + mat::round(kx*lgr); // pixel value of current line
	
        while ( (Lgr >= F_bl.x) && (ngr < 100) )
              {
	       if ( Lgr != O.x )
		    { // Not y axys
		     Lbl = FloatToStrF(lgr,ffGeneral,10,2);
		     Canvas->MoveTo(Lgr,F_bl.y);
		     Canvas->LineTo(Lgr,F_tr.y);
		     Canvas->TextOut( Lgr-(Canvas->TextWidth(Lbl)/2),
					      F_bl.y,
					      Lbl);
		    }
	       else { // on y axys do nothing
		    }
		
	       lgr = lgrmax - ( ngr * DVgr ); // current grid line ordinate value
	       Lgr = O.x + mat::round(kx*lgr);// ordinate of current line
               ngr++;
              }

	// . . horizontal
        ngr=0; // temp coordinates and counter
	lgr = lgrmax = floor( Amax / DHgr ) * DHgr; // max grid line value
	Lgr = O.y - mat::round(ky*lgr); // ordinate of current line
	
        while ( (Lgr >= F_tr.y) && (ngr < 100) )
              {
	       if ( Lgr != O.y )
		    { // Not x axys
		     Lbl = FloatToStrF(lgr,ffGeneral,10,2);
		     Canvas->MoveTo(F_bl.x,Lgr);
		     Canvas->LineTo(F_tr.x,Lgr);
		     Canvas->TextOut( F_bl.x,
					      Lgr-fixV,
					      Lbl);
		    }
	       else { // on x axys only label `0´
		     Lbl = "0";
		     Canvas->TextOut( F_bl.x - Canvas->TextWidth(Lbl) - 2,
					      O.y-fixV,
					      Lbl);
		    }
		
	       lgr = lgrmax - ( ngr * DHgr ); // current grid line ordinate value
	       Lgr = O.y - mat::round(ky*lgr); // ordinate of current line
               ngr++;
              }
*/
}

//---------------------------------------------------------------------------
inline void cls_Frame::DrawTheAxes (TCanvas* Canvas)
{ // Draw axes if visible

// . . . Set Pen
        //Canvas->Pen->Style = psSolid;
	Canvas->Pen->Width = 2;
        Canvas->Pen->Color = AxesColor;

// . . . X axis
	if ( Yo >= -Margin && Yo <= height+Margin ) // if visible
	   {
            static int Yco; Yco = bottom-Yo;
	    Canvas->MoveTo(Left,Yco);
	    Canvas->LineTo(right,Yco);
	    // arrow
	    Canvas->LineTo(right,Yco+ArrowWing);
	    Canvas->LineTo(right+ArrowLength,Yco);
            Canvas->LineTo(right,Yco-ArrowWing);
	    Canvas->LineTo(right,Yco);
	   }
// . . . Y axis
	if ( Xo >= -Margin && Xo <= width+Margin ) // if visible
	   {
	    Canvas->MoveTo(left+Xo,Bottom);
	    Canvas->LineTo(left+Xo,top);
	    // arrow
	    Canvas->LineTo(left+Xo-ArrowWing,top);
	    Canvas->LineTo(left+Xo,top-ArrowLength);
            Canvas->LineTo(left+Xo+ArrowWing,top);
	    Canvas->LineTo(left+Xo,top);
	   }
}

//---------------------------------------------------------------------------
inline void cls_Frame::DrawTheLegend (TCanvas* Canvas)
{ // Draw legend visible

// . . . Legend
        Canvas->Pen->Style = psSolid;
        Canvas->Font->Name = Parent->TextFontName;
        Canvas->Font->Size = Parent->TextFontSize;

        unsigned int i=0;
        for ( tr=Tracks.begin(); tr!=Tracks.end(); ++tr )
            {
             /*
             if (tr == LeadingTrack) Canvas->Font->Style = TFontStyles() << fsBold << fsUnderline;
             else Canvas->Font->Style = TFontStyles() >> fsBold >> fsUnderline;
             TPoint P = TPoint ( Width-15-Target->Canvas->TextWidth("xxxxxxxxxxx"), 3 * i * fixV );
             Canvas->Pen->Color = tr->PlotColor;
             Canvas->Pen->Width = 3;
             Canvas->MoveTo(P.x, P.y+fixV);
             Canvas->LineTo(P.x+15, P.y+fixV);
             P.x += 17;
             String lbl;
             //if ( !tr->Points.empty() ) lbl = tr->Label + " = " + FloatToStrF(tr->Points.back().y, ffGeneral,6,2)+ tr->Unit;
             lbl = tr->Label;
             Canvas->TextOut( P.x, P.y, lbl);
             i++;
             */
            }
}



//-------------------------- Space transformations --------------------------

//---------------------------------------------------------------------------
void cls_Frame::AdaptScale (const cls_Track* trackptr)
{ // Adapt scale to visualize track space occupation / to view axes

        if ( trackptr )
             {
              SetMappedX(trackptr->Xmin,trackptr->Xrange);
              SetMappedY(trackptr->Ymin,trackptr->Yrange);
             }
        else {
              SetMappedX(xmin_def,xrange_def);
              SetMappedY(ymin_def,yrange_def);
             }
}

//---------------------------------------------------------------------------
void cls_Frame::ResetOrigin()
{ // Adapt space to view axes

        SetTransformX(Kx,0);
        SetTransformY(Ky,0);
}

//---------------------------------------------------------------------------
void cls_Frame::Pan (const int& DX, const int& DY)
{ // Modify mapped space offset given a pixel displacement

        Xo += DX; Yo -= DY;
        xmin = Xphy(left);
        ymin = Yphy(bottom);
}

//---------------------------------------------------------------------------
void cls_Frame::Zoom ( const Windows::TRect& ZR )
{ // Modify space-transformation to map a new space

        Windows::TRect R = ZR;
        mat::Normalize(R);

        SetMappedX (Xphy(R.left), Xphy(R.right)-Xphy(R.left));
        SetMappedY (Yphy(R.bottom), Yphy(R.top)-Yphy(R.bottom));
}



//------------------------------ Tracks services ----------------------------

//---------------------------------------------------------------------------
void cls_Frame::InsertTrack (cls_Track* trackptr)
{ // Add a track pointer to plotted track set

        // Set operation
        Tracks.insert(trackptr);
        // Update tracks owners list
        trackptr->InsertOwner(this);
        // Update Selected Track
        i_SelectedTrack = trackptr;
}

//---------------------------------------------------------------------------
void cls_Frame::InsertTracks (cls_Frame* frameptr)
{ // Add track pointers of another frame

        // Set operations
        if ( !frameptr->Tracks.empty() ) Tracks.insert( frameptr->Tracks.begin(), --frameptr->Tracks.end() );
        // Update tracks owners list
        frameptr->AddToOwnedTracks(this);
}

//---------------------------------------------------------------------------
void cls_Frame::AddToOwnedTracks (cls_Frame* frameptr)
{ // Add an owner to owned tracks

        for ( tr=Tracks.begin(); tr!=Tracks.end(); ++tr ) (*tr)->InsertOwner(frameptr);
}


//---------------------------------------------------------------------------
void cls_Frame::RemoveTrack (cls_Track* trackptr)
{ // Remove track from plotted tracks

        // Set operation
        Tracks.erase( trackptr );
        // Update tracks owners list
        trackptr->RemoveOwner(this);
        // Update track selection
        if ( Tracks.empty() ) i_SelectedTrack = 0;
        else if ( trackptr == SelectedTrack ) i_SelectedTrack = *(Tracks.rend());
}

//---------------------------------------------------------------------------
void cls_Frame::RemoveTracks ()
{ // Remove all tracks

        // Set operation
        Tracks.clear();
        // Update tracks owners list
        for ( tr=Tracks.begin(); tr!=Tracks.end(); ++tr ) (*tr)->RemoveOwner(this);
        // Update track selection
        i_SelectedTrack = 0;
}

//---------------------------------------------------------------------------
bool cls_Frame::Contains (cls_Track* trackptr)
{ // Tell if contains a track

        return Tracks.count(trackptr);
}

//---------------------------------------------------------------------------
cls_Track* cls_Frame::OverTrack (const TPoint& P)
{ // Clicked Track

        for ( tr=Tracks.begin(); tr!=Tracks.end(); ++tr ) if ( (*tr)->Contains(P,this) ) return *tr;
        return 0;
}




//=============================== Properties ================================

//---------------------------------------------------------------------------
void cls_Frame::set_Margin (const int m)
{ // Set margin and adjust axes arrow dimensions

        i_Margin = m;
        ArrowLength = .7 * i_Margin;
        ArrowWing = .4 * ArrowLength;
}

//---------------------------------------------------------------------------
void cls_Frame::set_VGridDist (uint D)
{ // Set vertical grid pixel distance

        if ( D > 0 ) i_VGridDist = D;
        else i_VGridDist = 2;
}

//---------------------------------------------------------------------------
void cls_Frame::set_HGridDist (uint D)
{ // Set horizontal grid pixel distance

        if ( D > 0 ) i_HGridDist = D;
        else i_HGridDist = 2;
}

//---------------------------------------------------------------------------
void cls_Frame::set_YLogarithmic (bool b)
{ // Set y logarithmic scale

        if ( b && ymin <= 0 ) ymin = mat::epsilon; // Logarithmic scale domain control
        i_YLogarithmic = b;
        RecalculateTransformY();
        AdaptScale();
}

//---------------------------------------------------------------------------
void cls_Frame::set_XLogarithmic (bool b)
{ // Set x logarithmic scale

        if ( b && xmin <= 0 ) xmin = mat::epsilon; // Logarithmic scale domain control
        i_XLogarithmic = b;
        RecalculateTransformX();
        AdaptScale();
}


//============================= Private methods =============================


//---------------------------------------------------------------------------
inline void cls_Frame::SetMappedX (const double& minval, const double& range)
{ // Set mapped x space

        if ( XLogarithmic && minval<=0 ) xmin = mat::epsilon; // Logarithmic scale domain check
        else xmin = minval;
        xrange = range;
        RecalculateTransformX();
}

//---------------------------------------------------------------------------
inline void cls_Frame::SetMappedY (const double& minval, const double& range)
{ // Set mapped y space

        if ( YLogarithmic && minval<=0 ) ymin = mat::epsilon; // Logarithmic scale domain check
        else ymin = minval;
        yrange = range;
        RecalculateTransformY();
}

//---------------------------------------------------------------------------
inline void cls_Frame::SetTransformX (const double& k, const int& o)
{ // Set x-axis transformation coeffs

        Kx = k;
        Xo = o;
        RecalculateMappedX();
}

//---------------------------------------------------------------------------
inline void cls_Frame::SetTransformY (const double& k, const int& o)
{ // Set y-axis transformation coeffs

        Ky = k;
        Yo = o;
        RecalculateMappedY();
}

//---------------------------------------------------------------------------
inline void cls_Frame::RecalculateMappedX()
{ // Recalculate x-axis mapped space from transformation coeffs and dimension

        xmin = Xphy(left);
        xrange = Xphy(right) - xmin;
}

//---------------------------------------------------------------------------
inline void cls_Frame::RecalculateMappedY()
{ // Recalculate y-axis mapped space from transformation coeffs and dimension

        ymin = Yphy(bottom);
        yrange = Yphy(top) - ymin;
}


//---------------------------------------------------------------------------
inline void cls_Frame::RecalculateTransformX()
{ // Recalculate x-axis transformation coeffs from mapped space and dimensions

        if ( XLogarithmic )
             { // Logarithmic (X = Kx Log(x) + Xo)
              Kx = CalculateLogarithmicScale((xrange+xmin)/xmin,width);
              Xo = CalculateLogarithmicOrigin(xmin,0,Kx);
             }
        else { // Linear (X = Kx x + Xo)
              Kx = CalculateLinearScale(xrange,width);
              Xo = CalculateLinearOrigin(xmin,0,Kx);
             }
}

//---------------------------------------------------------------------------
inline void cls_Frame::RecalculateTransformY()
{ // Recalculate y-axis transformation coeffs from mapped space and dimensions

        if ( YLogarithmic )
             { // Logarithmic (Y = Ky Log(y) + Yo)
              Ky = CalculateLogarithmicScale((yrange+ymin)/ymin,height);
              Yo = CalculateLogarithmicOrigin(ymin,0,Ky);
             }
        else { // Linear (Y = Ky y + Yo)
              Ky = CalculateLinearScale(yrange,height);
              Yo = CalculateLinearOrigin(ymin,0,Ky);
             }
}


//---------------------------------------------------------------------------
inline double cls_Frame::CalculateLinearScale (double range, const int pix)
{ // Calculate axis linear scale to fit a range=max-min

        // Low level check? ###
        //if ( mat::IsZero(range) ) range = 1;
        return pix / range; // K=(pix length)/(max-min)
}

//---------------------------------------------------------------------------
inline int cls_Frame::CalculateLinearOrigin (const double z, const int Z, const double K)
{ // Calculate linear axis pixel origin given two points

        return (Z - mat::round(K*z) ); // Zo = Z - K z
}

//---------------------------------------------------------------------------
inline double cls_Frame::CalculateLogarithmicScale (double range, const int pix)
{ // Calculate axis logarithmic scale to fit a range=max/min

        // Low level check? ###
        //if ( range <= 1+mat::epsilon ) range = 10;
        return pix / ::log10(range); // K = (pix length)/[Log(max)-Log(min)]
}

//---------------------------------------------------------------------------
inline int cls_Frame::CalculateLogarithmicOrigin (const double z, const int Z, const double K)
{ // Calculate logarithmic axis pixel origin given two points

        // Low level check? ###
        //if ( z <= mat::epsilon ) z = 1;
        return ( Z - mat::round ( K*::log10(z) ) ); // Zo = Z - K Log(z)
}


/*
//---------------------------------------------------------------------------
inline void cls_Frame::VerticalGridIncrement ()
{ // Calculate vertical grid increment

        //VGridDensity = Width/VGridPixDist;
}

//---------------------------------------------------------------------------
inline void cls_Frame::HorizontalGridIncrement ()
{ // Calculate horizontal grid increment

        //HGridDensity = Height/HGridPixDist;
}
*/


//---------------------------------------------------------------------------
double Rounded( double v, int d =1 )
{ // Lower rounded number (0.1,0.2,0.5,1,2,5,10,...) density d>0

        const double Log5 = 0.69897000433601880478626110527551;
        const double Log2 = 0.30102999566398119521373889472449;
        double ex = ::log10( ::fabs( v/d ) );
        double ans = ::pow10( ::floor(ex) );
        ex -= ::floor(ex); // Fractional part of exponential
        if ( ex >= Log5 ) ans *= 5;
        else if ( ex >= Log2 ) ans *= 2;
        return (v<0) ? -ans : ans;
}

//=============================== End of File ===============================
