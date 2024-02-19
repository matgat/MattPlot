#include <vcl.h>
#include "unt_MatCommon.h"
#pragma hdrstop
#include "unt_MPlot.h" // ToString()
#include "unt_Frames.h"
#include "unt_Tracks.h"
#include "unt_Fourier.h" // For 'Spect()'
//#include <memory> // std::auto_ptr
#include "unt_PropertiesInterface.h" // To instantiate interface
#pragma package(smart_init)
//---------------------------------------------------------------------------

//============================== File scope =================================
// TODO 5: Initialize function tracks with strings (parsers)
// TODO 4: Export current viewed domain (frame view) to text

//#define BYPASS_VCL_GDI_WRAPPER
//#define DRAWTRACK_PIXELSINCREMENTS



///////////////////////////////// cls_Track /////////////////////////////////
//============================= Static Members ==============================
    const String cls_Track::PointStyles = "o+qdx";
    // Track defaults
    int cls_Track::DefaultLineWeight = 1;
    TColor cls_Track::DefaultLineColor = clRed;
    TPenStyle cls_Track::DefaultLineStyle = psSolid;
    char cls_Track::DefaultPointStyle = 'o';
    int cls_Track::DefaultPointSizeCoeff = 2;
    bool cls_Track::DefaultMarkPoints = false;
    bool cls_Track::DefaultStrokeLines = true;
    bool cls_Track::DefaultFillArea = false;
    bool cls_Track::DefaultDrawHistograms = false;
    int cls_Track::DefaultHistWidthPerc = 75;
    bool cls_Track::DefaultTrackShowCoords = false;
    int cls_Track::DefaultTrackCoordsDist = 80;

    cls_FoilObject::typ_ActionIdxs cls_Track::ActionsIdxs; // Used to build popup menu


//============================= Public methods ==============================

//---------------------------------------------------------------------------
cls_Track::cls_Track (const String& n, class cls_Frame* f) : cls_FrameObject(f)
{ // Constructor

// . Inputs
    i_Name = (n.IsEmpty())? String("y") : n;
    i_XName = "";

// . Defaults
    // Style (inherited)
    ColorMode = pmCopy;
    LineWeight = DefaultLineWeight;
    LineColor = DefaultLineColor;
    LineStyle = DefaultLineStyle;
    AreaStyle = bsClear; // bsSolid

    // Options
    i_MarkPoints = DefaultMarkPoints;
    i_PointStyle = DefaultPointStyle;
    i_PointSizeCoeff = DefaultPointSizeCoeff;
    i_StrokeLines = DefaultStrokeLines;
    i_FillArea = DefaultFillArea;
    i_DrawHistograms = DefaultDrawHistograms;
    i_HistWidthPerc = DefaultHistWidthPerc;
    i_ShowCoords = DefaultTrackShowCoords;
    CoordsDist = DefaultTrackCoordsDist; // (call set method for this)

// . Initializations
    i_ymax = i_xmax = i_tmax = 0; // mat::NaN;
    i_ymin = i_xmin = i_tmin = 0; // mat::NaN;

    i_x = i_y = i_dx = i_dy = 0;
    i_X = i_Y = 0;
    #ifdef DRAWTRACK_PIXELSINCREMENTS
    i_DX = i_DY = 0;
    #endif
}


//---------------------------------------------------------------------------
// Create the edit properties interface
cls_PropertiesInterface* cls_Track::CreateSpecInterface(class TWinControl* p)
{
    return new cls_TrackEdit(p,this);
}

//---------------------------------------------------------------------------
// Get a enum type (deprecated)
nms_MPlotClasses::en_FoilObjectType cls_Track::GetType()
{
    return nms_MPlotClasses::OB_TRACK;
}


//---------------------------------------------------------------------------
void cls_Track::Stroke (class cls_Foil* f, const TPoint& P1, const TPoint& P2)
{ // Stroke a tract (used in legend)

// . Set Pen and Brush
    SetPaintTools(f);

// . Stroke
    if ( FillArea || DrawHistograms )
       {
        f->Canvas->Pen->Color = AreaColor;
        f->Canvas->Pen->Style = psSolid;
        int inc = (P2.x-P1.x)/3;
        for (int i=P1.x; i<P2.x; i+=inc)
            {
             f->Canvas->MoveTo(i,P1.y);
             f->Canvas->LineTo(i,P2.y+10*f->PointsPerPix);
            }
        f->Canvas->Pen->Color = LineColor;
        f->Canvas->Pen->Style = LineStyle;
       }
    if ( StrokeLines )
       {
        f->Canvas->MoveTo(P1.x,P1.y);
        f->Canvas->LineTo(P2.x,P2.y);
       }
    if ( MarkPoints )
       {
        DoMarkPoint(P1.x, P1.y, PointSize*f->PointsPerPix);
        DoMarkPoint(P2.x, P2.y, PointSize*f->PointsPerPix);
       }
}




//============================ Protected Methods ============================

//---------------------------------------------------------------------------
inline void cls_Track::DoMarkPoint(const int X, const int Y, const int P_2)
{ // Mark a point; P_2 is the mark half size

    switch ( PointStyle )
        {
         case 'o' :
                Canvas->Ellipse(X-P_2,Y-P_2,X+P_2,Y+P_2);
                break;
         case '+' :
                Canvas->MoveTo(X-P_2,Y); Canvas->LineTo(X+P_2,Y);
                Canvas->MoveTo(X,Y-P_2); Canvas->LineTo(X,Y+P_2);
                Canvas->MoveTo(X,Y);
                break;
         case 'q' :
                Canvas->Rectangle(X-P_2,Y-P_2,X+P_2,Y+P_2);
                break;
         case 'd' :
                Canvas->MoveTo(X-P_2,Y);
                Canvas->LineTo(X,Y+P_2);
                Canvas->LineTo(X+P_2,Y);
                Canvas->LineTo(X,Y-P_2);
                Canvas->LineTo(X-P_2,Y);
                Canvas->MoveTo(X,Y);
                break;
         case 'x' :
                Canvas->MoveTo(X-P_2,Y-P_2); Canvas->LineTo(X+P_2,Y+P_2);
                Canvas->MoveTo(X+P_2,Y-P_2); Canvas->LineTo(X-P_2,Y+P_2);
                Canvas->MoveTo(X,Y);
                break;
         //case '.' : Canvas->Pixels[X][Y] = AreaColor; break;
        }
}

//---------------------------------------------------------------------------
// Show current point coordinates
inline void cls_Track::DoShowCoords(const int d2)
{

    static int i_LastX=0, i_LastY=0; // Last point done

    int iDX = i_X - i_LastX;
    int iDY = i_Y - i_LastY;

    if ( (iDX*iDX + iDY*iDY) > d2 )
       {
        #ifdef BYPASS_VCL_GDI_WRAPPER
        //String s = Owner->FormatPoint_abs(i_y,i_x);
        String s = Owner->Owner->ToString(i_x) + String(";") + Owner->Owner->ToString(i_y);
        ::TextOut(Canvas->Handle,i_X,i_Y,s.c_str(),s.Length());
        #else
        Canvas->TextOut(i_X+PointSize,i_Y+PointSize,Owner->Owner->ToString(i_x)+String(";")+Owner->Owner->ToString(i_y));
        Canvas->MoveTo(i_X,i_Y);
        #endif
        i_LastY = i_Y;
        i_LastX = i_X;
       }
}

//---------------------------------------------------------------------------
inline void cls_Track::DoFillArea (const int X, const int Y, const int DW)
{ // Fill area subtended by point
    ASSERTING((Owner),"Here \'Owner\' should be defined!")
	static TPoint CurPenPos;

// . Set tools
    CurPenPos = Canvas->PenPos;
    Canvas->Pen->Mode = pmNotXor; //pmMask;
    Canvas->Pen->Color = AreaColor;
    Canvas->Pen->Width = DW;
// . Do the work
    Canvas->MoveTo(X,Owner->O.y);
    Canvas->LineTo(X,Y+LineWeight);
// . Restore
    Canvas->PenPos = CurPenPos; // //Canvas->MoveTo(X,Y);
    Canvas->Pen->Mode = ColorMode;
    Canvas->Pen->Color = LineColor;
    Canvas->Pen->Width = LineWeight;
}

//---------------------------------------------------------------------------
inline void cls_Track::DoDrawHistogram (const int X, const int Y, int DX_2)
{ // Draw an histogram for point; DX_2 is the half width
    ASSERTING((Owner),"Here \'Owner\' should be defined!")

    DX_2 = (DX_2 * i_HistWidthPerc) / 100;
    if (DX_2>0) Canvas->Rectangle(X-DX_2,Y,X+DX_2,Owner->O.y);
    else { // Just a line
          Canvas->MoveTo(X,Owner->O.y);
          Canvas->LineTo(X,Y);
         }
}


//============================= Protected Methods ===========================

//---------------------------------------------------------------------------
void cls_Track::Assign(const cls_Track* t2)
{ // Copy in other track

    if ( this!=t2 && t2 )
       {
        // Inherited stuff
        cls_FoilObject::AssignStyle(t2);
        // My stuff
        i_Name = t2->i_Name;
        i_XName = t2->i_XName;
        i_YQuantity = t2->i_YQuantity;
        i_XQuantity = t2->i_XQuantity;
        i_tmin = t2->i_tmin;
        i_tmax = t2->i_tmax;
        i_xmin = t2->i_xmin;
        i_xmax = t2->i_xmax;
        i_ymin = t2->i_ymin;
        i_ymax = t2->i_ymax;
        i_MarkPoints = t2->i_MarkPoints;
        i_StrokeLines = t2->i_StrokeLines;
        i_FillArea = t2->i_FillArea;
        i_DrawHistograms = t2->i_DrawHistograms;
        i_ShowCoords = t2->i_ShowCoords;
        i_PointStyle = t2->i_PointStyle;
        i_PointSizeCoeff = t2->PointSizeCoeff;
        i_HistWidthPerc = t2->i_HistWidthPerc;
       }
}

//============================== Private Methods ============================

//---------------------------------------------------------------------------
void cls_Track::set_Name(String s)
{
    i_Name = s;
    if (Owner) Owner->CheckInvariant();
}




///////////////////////////// cls_FunctionTrack /////////////////////////////

//============================= Static Members ==============================

//============================= Public methods ==============================

//---------------------------------------------------------------------------
bool __fastcall cls_FunctionTrack::IsOver(const TPoint& P) const
{ // A point is over track
/*                   |        _,,-
         ------------+------,---------- Y-tol
                     | ,.-'
                    _o'----------------  Y
                _,-' |
         -----.'-----+----------------- Y+tol
            ,'       |
          ,'         |
         /          P.x                              */
    // Determine where is track point on screen
    ASSERTING((Owner),"Here \'Owner\' should be defined!")
    i_x = Owner->Xphy(P.x);
    if ( IsInDomain(i_x) )
       {
        return ::abs(P.y - Owner->Ypix(operator()(i_x))) <= (LineWeight + 5); // tolerance
       }
    else return false;
}


//============================= Private Methods =============================

//------------------------------- Properties --------------------------------




//////////////////////////// cls_ParametricTrack ////////////////////////////

//============================= Static Members ==============================

//============================= Public methods ==============================

//---------------------------------------------------------------------------
void cls_ParametricTrack::Statistics( String& s ) const
{ // Get overall statistics

    s = "  Number of samples: " + String(Size);
    if (IsEvenlySpaced()) s += " evenly spaced";
    else s += " not evenly spaced";
    s += "\n";
    s += "\n  Independent parameter increment: " + String(dtmin) + String("÷") + String(dtmax);
    s += "\n  Independent variable range: " + String(trange) + ", from " + String(tmin) + " to " + String(tmax);
    s += "\n";
    s += "\n  Abscissa range: " + String(xrange) + ", from " + String(xmin) + " to " + String(xmax);
    s += "\n";
    s += "\n  Ordinate range: " + String(yrange) + ", from " + String(ymin) + " to " + String(ymax);
}

//============================= Private Methods =============================

//------------------------------- Properties --------------------------------





//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%% Specializations of function tracks %%%%%%%%%%%%%%%%%%%



////////////////////////////// cls_DataFnTrack //////////////////////////////
//============================= Static Members ==============================

//============================= Public methods ==============================

//---------------------------------------------------------------------------
cls_DataFnTrack::cls_DataFnTrack(const String& n) : cls_FunctionTrack(n)
{ // Constructor

// . Initializations
    i_dtmin = i_dtmax = 0; // mat::NaN;
}


//---------------------------------------------------------------------------
void cls_DataFnTrack::Statistics( String& s ) const
{ // Get overall statistics

    String IndepVarIncr;
    s = "  Number of samples: " + String(Size);
    if ( IsEvenlySpaced() )
         {
          s += " evenly spaced";
          IndepVarIncr = String(" (constant): ") + String(dtmin);
         }
    else {
          s += " not evenly spaced";
          IndepVarIncr = String(": ") + String(dtmin) + String("÷") + String(dtmax);
         }
    s += "\n";
    s += "\n  Independent variable increment" + IndepVarIncr;
    s += "\n  Independent variable range: " + String(trange) + ", from " + String(tmin) + " to " + String(tmax);
    //s += "\n  Abscissa range: " + String(xrange) + ", from " + String(xmin) + " to " + String(xmax);
    s += "\n";
    s += "\n  Ordinate range: " + String(yrange) + ", from " + String(ymin) + " to " + String(ymax);
}



//---------------------------------------------------------------------------
void __fastcall cls_DataFnTrack::Draw (class cls_Foil* Foil)
{ // Plot a data track
    ASSERTING((Owner),"Here \'Owner\' should be defined!")

// . Initializations
    i_x = i_y = 0; // = i_dx = i_dy = 0;
    i_X = i_Y = 0; // = i_DX = i_DY = 0;
    px = Owner->PixelSizeX / Owner->SampPerPix;
    py = Owner->PixelSizeY / Owner->SampPerPix;
    // Some drawing quantities
    int DW = Owner->DXpix(dtmin) - 2; // Space between points (Used as lineweight in 'DoFillArea')
    if (DW<1) DW = 1;
    int DH_2 = (DW - LineWeight)/2; // Histograms half width (used in 'DoDrawHistograms')
    int P_2 = PointSize * Foil->PointsPerPix;

// . Move to first visible point
    ci = data.lower_bound( ((cls_Frame::PlotAlsoOnMargins)?Owner->xinf:Owner->xmin) );
    if ( ci == data.end() ) return; // Out of bounds
    else if ( ci != data.begin() ) --ci; // Previous, assuring no internal crop
    ci_end = data.lower_bound( ((cls_Frame::PlotAlsoOnMargins)?Owner->xsup:Owner->xmax) );
    if ( ci_end != data.end() ) ++ci_end; // Next, assuring no internal crop

    // Avoidable?
    Canvas->MoveTo(Owner->Xpix(ci->first),Owner->Ypix(ci->second));

// . Iterate all visible points
    while ( ci != ci_end )
          {
           // . Smart udersampling
           // TODO 5: also for logarithmic sclae
           // Update increments
           i_dy = ci->second - i_y;
           i_dx = ci->first - i_x;
           if ( std::fabs(i_dx)<px && std::fabs(i_dy)<py ) { ++ci; continue; } // This point can be skipped

           // . Update last physic point not skipped
           i_y = ci->second;
           i_x = ci->first;
           // Transform to screen (log scale errors are handled in transformation)
           #ifdef DRAWTRACK_PIXELSINCREMENTS
           i_DY = i_Y;
           i_DX = i_X;
           #endif
           i_Y = Owner->Ypix(i_y);
           i_X = Owner->Xpix(i_x);
           #ifdef DRAWTRACK_PIXELSINCREMENTS
           i_DY = i_Y - i_DY;
           i_DX = i_X - i_DX;
           #endif

           // . Draw
           if (FillArea) DoFillArea(i_X,i_Y,DW);
           if (StrokeLines) {
                             #ifdef BYPASS_VCL_GDI_WRAPPER
                              ::LineTo(Canvas->Handle,i_X,i_Y);
                             #else
                              Canvas->LineTo(i_X,i_Y);
                             #endif

                            }
           if (DrawHistograms) DoDrawHistogram(i_X,i_Y,DH_2);
           else Canvas->MoveTo(i_X,i_Y);
           if(MarkPoints) DoMarkPoint(i_X,i_Y,P_2);
           if(ShowCoords) DoShowCoords(i_CoordsDist2);

           // . Finally
           ++ci;
          }
}

//---------------------------------------------------------------------------
void __fastcall cls_DataFnTrack::UpdatePointQuantities (const typ_t t, mpl::stu_PointQuantities* p)
{// Calculate point quantities

    ci = data.upper_bound(t);
    if ( ci != data.end() )
         { // In domain
          p->y = ci->second;
          p->x = ci->first;

          //p->y = lininterp(t);
          //p->x = t;

          // Increments
          if ( ci != data.begin() )
               {
                --ci;
                p->dy = p->y - ci->second;
                p->dx = p->x - ci->first;
                p->dydx = mat::IsZero(p->dx) ? mat::NaN : p->dy/p->dx;
               }
          else {
                p->dy = p->dx = p->dydx = mat::NaN;
               }
         }
    else {
          p->y = p->x = p->dy = p->dx = p->dydx = mat::NaN;
         }
}

//---------------------------------------------------------------------------
void __fastcall cls_DataFnTrack::UpdateIntervalQuantities (typ_t t1, typ_t t2, mpl::stu_IntervalQuantities* q)
{// Calculate interval quantities

// . Find range iterators
    // First ensure a right order
    if ( t1 > t2 ) mat::swap(t1,t2);
    // Proceed to find iterators
    ci = data.upper_bound(t1);
    ci_end = ++( data.upper_bound(t2) );
    //if ( --ci == data.begin() ) ci = data.begin(); // previous sample also
    //if ( ci_end != data.end() ) ++ci_end; // next sample also
    if ( ci == ci_end )
       { // Null interval or out of bounds
        q->tmax = t2;
        q->tmin = t1;
        q->Dt = q->tmax - q->tmin;
        q->Dy = q->DY = q->n = 0;
        q->ymax = q->ymin = q->DyDt = q->Sy = q->avg = q->rms = q->std = q->stdp = q->varp = mat::NaN;
        #ifdef MAD
          q->mad = mat::NaN;
        #endif
        return;
       }

// . If here there is at leat one sample
    // Initialize values
    i_y = ci->second;
    i_x = ci->first;
    q->Dt = q->tmin = i_x;
    q->Dy = i_y;
    q->DY = 0;
    q->n = 1;
    q->ymax = q->ymin = i_y;
    q->avg = i_y;
    q->rms = i_y*i_y;
    q->Sy = q->std = 0;

    // Must have at least two samples for interval measures
    ++ci;

// . Iterate all points in interval
    while ( ci != ci_end )
          {
           // current increments
           i_dx = ci->first - i_x;
           i_dy = ci->second - i_y;
           // current coords
           i_y = ci->second;
           i_x = ci->first;
           // number of samples
           ++(q->n);
           // ordinate
           if ( q->ymax < ci->second ) q->ymax = ci->second;
           else if ( q->ymin > ci->second ) q->ymin = ci->second;
           //p->dydx = mat::IsZero(p->dx) ? mat::NaN : p->dy/p->dx;

           // Sum, mean, rms, standard deviation
           q->Sy += i_y * i_dx;
           q->avg += i_y;
           q->rms += i_y * i_y;

           // . Finally
           ++ci;
          }

// . Post calculations
    q->tmax = i_x;
    // Increments
    q->Dt = q->tmax - q->tmin;
    q->Dy = i_y - q->Dy;
    q->DY = q->ymax - q->ymin;
    q->DyDt = mat::IsZero(q->Dt) ? mat::NaN : q->Dy/q->Dt;
    // Mean, rms, standard deviation
    q->avg /= q->n; // average
    q->rms = std::sqrt(q->rms / q->n); // root mean square
    // TODO 4: Inspect why in some cases sqrt arg is negative
    q->std = std::sqrt( std::fabs((q->rms*q->rms)-(q->avg*q->avg)) ); // standard deviation

    // Maximum percentual variation respect mean
    if( !mat::IsZero(q->avg) )
       {
        q->stdp = mat::round(257.6 * q->std/q->avg, 0.1); // 99% (at least 85% for generic distributions) band
        q->varp = mat::round(100.0 * std::fabs(mat::max(q->ymax-q->avg,q->avg-q->ymin)/q->avg),0.1);
       }
    #ifdef MAD
      // Mean Absolute Deviation
      q->mad = 0;
      for ( ci=upper_bound(t1); ci!= ci_end; ++ci ) q->mad += std::fabs(ci->second - q->avg);
      q->mad /= q->n;
    #endif
}

//--------------------------- Modifying services ----------------------------


//---------------------------------------------------------------------------
cls_Track& cls_DataFnTrack::RescaleShift(const double k, const double o, bool dep)
{ // Linear transformation (Rescale/Shift)

    if ( dep )
         {
          for (typ_YtIt i=data.begin(); i!=data.end(); ++i) i->second = k*i->second + o;
          UpdateStats();
         }
    else {
          unsigned int N = data.size();
          numlist_xy y; numlist_t t; Fill(t,y);
          data.clear();
          for (unsigned int i=0; i<N; ++i) AddPoint(k*t[i]+o, y[i]);
         }

// . Finally
    return *this;
}

//---------------------------------------------------------------------------
cls_Track& cls_DataFnTrack::ReverseIndepVar()
{ // Reverse independent variable

    unsigned int N = data.size();
    numlist_xy y; numlist_t t; Fill(t,y);
    data.clear();
    for (unsigned int i=0; i<N; ++i) AddPoint(t[N-1-i], y[i]);

// . Finally
    return *this;
}

//---------------------------------------------------------------------------
cls_Track& cls_DataFnTrack::abs()
{ // Absolute value

    for (typ_YtIt i=data.begin(); i!=data.end(); ++i) i->second = std::fabs(i->second);
// . Finally
    UpdateStats();
    return *this;
}

//---------------------------------------------------------------------------
cls_Track& cls_DataFnTrack::pow(const double e)
{ // Absolute value

    //for (typ_YtIt i=data.begin(); i!=data.end(); ++i) i->second = );
    //UpdateStats();
// . Skip invalid values
    unsigned int N = data.size();
    numlist_xy y; numlist_t t; Fill(t,y);
    data.clear();
    bool fract_exp = (e != std::floor(e)); // must discard neg operands
    bool neg_exp = (e == std::floor(e)); // must discard null operands

    if ( fract_exp ) for (unsigned int i=0; i<N; ++i)
                         {
                          if ( neg_exp && mat::IsZero(y[i]) ) continue; // skip
                          AddPoint(t[i], mat::sign(y[i]) * std::pow(std::fabs(y[i]),e));
                         }
    else             for (unsigned int i=0; i<N; ++i)
                         {
                          if ( neg_exp && mat::IsZero(y[i]) ) continue; // skip
                          AddPoint(t[i], std::pow(y[i],e));
                         }
    return *this;
}


//---------------------------------------------------------------------------
// Exponential
cls_Track& cls_DataFnTrack::exp()
{
    for (typ_YtIt i=data.begin(); i!=data.end(); ++i) i->second = std::exp(i->second);
// . Finally
    UpdateStats();
    return *this;
}

//---------------------------------------------------------------------------
// Natural logarithm
cls_Track& cls_DataFnTrack::log()
{
    //for (typ_YtIt i=data.begin(); i!=data.end(); ++i) i->second = std::log(i->second);
    //UpdateStats();

// . Skip invalid values
    unsigned int N = data.size();
    numlist_xy y; numlist_t t; Fill(t,y);
    data.clear();
    for (unsigned int i=0; i<N; ++i)
        {
         if(y[i]<=0) continue; // skip
         AddPoint(t[i], std::log(y[i]));
        }

// . Finally
    return *this;
} // end 'log'

//---------------------------------------------------------------------------
// Base2 logarithm
cls_Track& cls_DataFnTrack::log2()
{
// . Skip invalid values
    unsigned int N = data.size();
    numlist_xy y; numlist_t t; Fill(t,y);
    data.clear();
    const double ln2_inv = 1.4426950408889634073599246810019;
    for (unsigned int i=0; i<N; ++i)
        {
         if(y[i]<=0) continue; // skip
         AddPoint(t[i], std::log(y[i])*ln2_inv);
        }

// . Finally
    return *this;
} // end 'log2'

//---------------------------------------------------------------------------
// Mask to 16bit
cls_Track& cls_DataFnTrack::mask16()
{
    for (typ_YtIt i=data.begin(); i!=data.end(); ++i) i->second = mat::round(i->second) & int(0xFFFF);
// . Finally
    UpdateStats();
    return *this;
}

//---------------------------------------------------------------------------
// Cast to unsigned int 16bit
cls_Track& cls_DataFnTrack::cast_unsigned16()
{
    for (typ_YtIt i=data.begin(); i!=data.end(); ++i) i->second = static_cast<unsigned int>(mat::round(i->second)) & 0xFFFF;
// . Finally
    UpdateStats();
    return *this;
}


//---------------------------------------------------------------------------
cls_Track& cls_DataFnTrack::Filter(int N)
{ // Averaging filter; on borders order decreases

// . Check
    N = std::abs(N);
    double Naveraged = N + 1;
    if ( data.size() < Naveraged ) return *this;

// . A buffer
    numlist_xy filtered;
    filtered.reserve( data.size() );

// . Prepare iterators delimiting inner parts
    int before = mat::floor(double(N)/2.0);
    int after = mat::ceil(double(N)/2.0);
    typ_YtIt start = data.begin();
    typ_YtIt curr = start;
    for (int i=0; i<before; ++i) ++curr;
    typ_YtIt stop = start;
    for (int i=0; i<=after; ++i) ++stop;

// . Operation
    double sum; // A bag
    // Starting border
    typ_YtIt brd = start;
    while ( brd != curr )
          {
           sum = 0;
           int N = 0;
           for (typ_YtIt i=start; i!=stop; ++i) {sum+=i->second; ++N;}
           //brd->second = sum / N; // don't write track itself
           filtered.push_back( sum/N );
           ++brd; ++stop;
          }
    // Inner part
    while ( stop != data.end() )
          {
           sum = 0;
           for (typ_YtIt i=start; i!=stop; ++i) sum += i->second;
           //curr->second = sum / Naveraged; // don't write track itself
           filtered.push_back( sum/Naveraged );
           ++start; ++curr; ++stop;
          }
    // Ending border
    while ( curr != data.end() )
          {
           sum = 0;
           int N = 0;
           for (typ_YtIt i=start; i!=data.end(); ++i) {sum+=i->second; ++N;}
           //curr->second = sum / N; // don't write track itself
           filtered.push_back( sum/N );
           ++start; ++curr;
          }

// . Finally
    AssignDepVar(filtered);
    return *this;
}

//---------------------------------------------------------------------------
// Crop abscissa domain and ordinates
cls_Track& cls_DataFnTrack::Crop(const mat::stu_Rect& r)
{
     String a = Name;
// . Perform operation
    // Abscissa crop
    data.erase(data.begin(), data.lower_bound(r.left));
    data.erase(data.upper_bound(r.right), data.end());
    // Ordinate crop
    for (typ_YtIt i=data.begin(); i!=data.end(); )
        {
         if ( i->second > r.top) data.erase(i++);
         else if ( i->second < r.bottom) data.erase(i++);
         else ++i;
        }

// . Update statistics
    UpdateStats();

    // TODO 5: maintain optimization flags SameAbscissa or IndexedAbscissa

    return *this;
}

//---------------------------------------------------------------------------
// Compute spectrum
cls_Track& cls_DataFnTrack::Spect(const bool usefft)
{

// . Interface with 'cls_fft'
    int N = data.size();
    int N_2 = N/2;
    double fs_N = 1.0/(dtmin*N);

    // . New
    typ_xy* y = new typ_xy[N];
    ci = data.begin();
    for ( int i=0; i<N; ++i ) y[i]=(ci++)->second;

    // . Do
    //typ_xy* S = new typ_xy[N];
    typ_xy* S = new typ_xy[N_2+1];
    if (usefft) cls_fft::spect_fft(N, y, S);
    else cls_fft::spect_dft(N, y, S);
    delete[] y;

    // . Copy into
    data.clear();
    for (int i=0; i<=N_2; ++i ) AddPoint(i*fs_N, S[i]);
    delete[] S;

// . Finally
    return *this;
}

//---------------------------------------------------------------------------
// Derivative
cls_Track& cls_DataFnTrack::Derivative()
{
// . Skip null increments
    unsigned int N_1 = data.size() - 1;
    numlist_xy y; numlist_t t; Fill(t,y);
    typ_xy ii_dt; // bag
    data.clear();
    for (unsigned int i=0; i<N_1; ++i)
        {
         // Get increment
         ii_dt = t[i+1] - t[i];

         // Put derivative
         if ( !mat::IsZero(ii_dt) ) AddPoint(t[i], (y[i+1]-y[i])/ii_dt);
        }

// . Finally
    return *this;

/*
// . Prepare iterators
    typ_YtIt i = data.begin();
    if ( i == data.end() ) return *this; // If empty do nothing
    typ_YtIt i_end = --(data.end());

// . All samples
    typ_xy ii_dt, ii_dy; // Bags
    typ_YtIt i_prev = i;
    while ( ++i != i_end )
          {
           // Get increments
           ii_dt = i->first - i_prev->first;
           ii_dy = i->second - i_prev->second;

           // Calculate derivative and substitute point
           if ( mat::IsZero(ii_dt) )
                {
                 i_prev = data.erase(i_prev);
                }
           else {
                 i_prev->second = ii_dy/ii_dt;
                }

           // Update
           ++i_prev;
          }

// . Finally
    i_end->second = i_prev->second;
    UpdateStats();
    return *this;
*/
}

//---------------------------------------------------------------------------
// Integration
cls_Track& cls_DataFnTrack::Integral(const typ_xy C)
{
// . Prepare iterators
    typ_YtIt i = data.begin();
    if ( i == data.end() ) return *this; // If empty do nothing
    typ_xy dA, prev_val = i->second; // bag

// . Integration constant
    if ( !mat::isNaN(C) ) i->second = C;
    // else constant integration is equal first sample

// . For all next samples
    typ_YtIt i_prev = i;
    while ( ++i != data.end() )
          {
           // Sum (use trapezoid)
           dA = ((i->second+prev_val)/2) * (i->first - i_prev->first);
           prev_val = i->second;
           i->second = i_prev->second + dA;
           // Update
           ++i_prev;
          }

// . Finally
    UpdateStats();
    return *this;
}



//-------------------- Arithmetic operators with scalar ---------------------

//---------------------------------------------------------------------------
cls_Track& cls_DataFnTrack::operator+ (const typ_xy k)
{
    if ( !mat::isNaN(k) )
       {
        for ( typ_YtIt i=data.begin(); i!=data.end(); ++i ) i->second += k;
        UpdateStats();
       }
    return *this;
}

//---------------------------------------------------------------------------
cls_Track& cls_DataFnTrack::operator* (const typ_xy k)
{
    double kmax = absymax;
    kmax = (k>1) ? mat::SupDouble/k : mat::SupDouble;
    if ( !mat::isNaN(k) && k<kmax)
       {
        for ( typ_YtIt i=data.begin(); i!=data.end(); ++i ) i->second *= k;
        UpdateStats();
       }
    return *this;
}

//---------------------------------------------------------------------------
cls_Track& cls_DataFnTrack::operator/ (const typ_xy k)
{
    if ( !mat::isNaN(k) && std::fabs(k)>(absymax/mat::SupDouble))
       {
        for ( typ_YtIt i=data.begin(); i!=data.end(); ++i ) i->second /= k;
        UpdateStats();
       }
    return *this;
}



//-------------------- Arithmetic operators with track ----------------------

//---------------------------------------------------------------------------
cls_Track& cls_DataFnTrack::Sum(const cls_Track& t2)
{
    for ( typ_YtIt i=data.begin(); i!=data.end(); ++i )
        {
         i_y = t2.lininterp(i->first);
         if ( !mat::isNaN(i_y) ) i->second += i_y;
        }

    UpdateStats();
    return *this;
}

//---------------------------------------------------------------------------
cls_Track& cls_DataFnTrack::Subtract(const cls_Track& t2)
{
    for ( typ_YtIt i=data.begin(); i!=data.end(); ++i )
        {
         i_y = t2.lininterp(i->first);
         if ( !mat::isNaN(i_y) ) i->second -= i_y;
        }

    UpdateStats();
    return *this;
}

//---------------------------------------------------------------------------
cls_Track& cls_DataFnTrack::Multiply(const cls_Track& t2)
{
    for ( typ_YtIt i=data.begin(); i!=data.end(); ++i )
        {
         i_y = t2.lininterp(i->first);
         if ( !mat::isNaN(i_y) ) i->second *= i_y;
        }

    UpdateStats();
    return *this;
}

//---------------------------------------------------------------------------
cls_Track& cls_DataFnTrack::Divide(const cls_Track& t2)
{
    for ( typ_YtIt i=data.begin(); i!=data.end(); ++i )
        {
         i_y = t2.lininterp(i->first);
         if ( !mat::isNaN(i_y) )
            {
             if (mat::IsZero(i_y)) i->second = mat::sign(i->second) * mat::sign(i_y) * mat::SupDouble;
             else i->second /= i_y;
            }
        }

    UpdateStats();
    return *this;
}

//---------------------------------------------------------------------------
cls_Track& cls_DataFnTrack::CombineMod(const cls_Track& t2)
{
    for ( typ_YtIt i=data.begin(); i!=data.end(); ++i )
        {
         i_y = t2.lininterp(i->first);
         i->second = std::sqrt(i->second*i->second + i_y*i_y);
        }

    UpdateStats();
    return *this;
}

//---------------------------------------------------------------------------
cls_Track& cls_DataFnTrack::Atan(const cls_Track& t2)
{
    for ( typ_YtIt i=data.begin(); i!=data.end(); ++i )
        {
         i_y = t2.lininterp(i->first);
         i->second = std::atan2(i->second, i_y);
        }

    UpdateStats();
    return *this;
}

//---------------------------------------------------------------------------
cls_Track& cls_DataFnTrack::Custom1(const cls_Track& t2)
{
    for ( typ_YtIt i=data.begin(); i!=data.end(); ++i )
        {
         i_y = t2.lininterp(i->first);
         if ( !mat::isNaN(i_y) ) i->second = std::sqrt(i->second*i->second + i_y*i_y) * std::sin(0.78539816339744830961566084581988-std::atan2(i->second, i_y));
        }

    UpdateStats();
    return *this;
}

//--------------------------- Exporting services ----------------------------


//---------------------------------------------------------------------------
void cls_DataFnTrack::CopyIndepVarTo( mat::numlist* v ) const
{ // Fills a vector of doubles with independent variable

    v->reserve( data.size() );
    for ( ci=data.begin(); ci!=data.end(); ++ci ) v->push_back( ci->first );
}

//---------------------------------------------------------------------------
void cls_DataFnTrack::CopyDepVarTo( mat::numlist* v ) const
{ // Fills a vector of doubles with dependent variable

    v->reserve( data.size() );
    for ( ci=data.begin(); ci!=data.end(); ++ci ) v->push_back( ci->second );
}

//---------------------------------------------------------------------------
void cls_DataFnTrack::Fill(typ_t* t, typ_xy* y) const
{ // Fill the double arrays with data

    ci = data.begin();
    for ( unsigned int i=0, imax=data.size(); i<imax; ++i )
        {
         t[i] = ci->first;
         y[i] = ci->second;
         ++ci;
        }
}

//---------------------------------------------------------------------------
void cls_DataFnTrack::Fill(numlist_xy& t, numlist_xy& y) const
{ // Fill the double arrays with data

    t.reserve( data.size() );
    y.reserve( data.size() );
    for ( ci=data.begin(); ci!=data.end(); ++ci )
        {
         t.push_back( ci->first );
         y.push_back( ci->second );
        }
}

//---------------------------------------------------------------------------
void cls_DataFnTrack::CopyIndepVarTo( String& s, const char delm) const
{ // Fills a string with independent variable

    String Delm = String(delm);
    //s.SetLength( s.Length() + 16*size() );
    for ( ci=data.begin(); ci!=data.end(); ++ci ) s += ::FloatToStr(ci->first) + Delm;
    s = s.SubString(1,s.Length()-1); // Drop last delimiter
}

//---------------------------------------------------------------------------
void cls_DataFnTrack::CopyDepVarTo( String& s, const char delm) const
{ // Fills a string with dependent variable

    String Delm = String(delm);
    //s.SetLength( s.Length() + 16*size() );
    for ( ci=data.begin(); ci!=data.end(); ++ci ) s += ::FloatToStr(ci->second) + Delm;
    s = s.SubString(1,s.Length()-1); // Drop last delimiter
}

//---------------------------------------------------------------------------
void cls_DataFnTrack::DataToStrings (class TStrings* sl) const
{ // Save data to a stringlist with rows composed by x<tab>y

// . Auxiliary variables
    sl->BeginUpdate();
    sl->Capacity = data.size();
// . For all points
    for ( ci=data.begin(); ci!=data.end(); ++ci )
        {
         sl->Add( ::FloatToStr(ci->first) + String('\t') + ::FloatToStr(ci->second) );
        }
// . Finally
    sl->EndUpdate();
}

//---------------------------------------------------------------------------
void cls_DataFnTrack::StringsToData (class TStrings* sl)
{ // Retrieve data from a stringlist with rows composed by x<tab>y

// . Preliminary operations
    data.clear();
    //reserve(sl->Capacity);
    int d;
    String Line, Dep, Indep;

// . For all points
    for ( int i=0, imax=sl->Count; i<imax; ++i )
        {
         // . Ignore if empty
         Line = sl->Strings[i].Trim();
         if ( Line.IsEmpty() ) continue;
         // . Get indep var and dep strings
         d = Line.LastDelimiter("\t");
         Indep = (Line.SubString(1,d-1)).Trim();
         Dep = (Line.SubString(d+1,Line.Length()-d)).Trim();
         try {
              AddPoint (Indep.ToDouble(), Dep.ToDouble());
             }
         catch (Exception& e)
             {
              throw mat::EParsingError(e.Message, i, 0);
             }
        }
// . Finally
}

//---------------------------------------------------------------------------
bool cls_DataFnTrack::SameIndepVar( const mat::numlist* nl ) const
{ // Tells if the independent variable coincide with given numlist

    if ( data.size() == nl->size() )
       {
        unsigned int i = 0;
        for ( ci=data.begin(); ci!=data.end(); ++ci )
            {
             if ( !mat::IsZero( ci->first - (*nl)[i++] ) ) return false;
            }
        return true;
       }
    else return false;
}

//---------------------------------------------------------------------------
void cls_DataFnTrack::GetIndepVarProgression (double* k, double* o) const
{ // Retrieve eventual indep var arithmetic progression

    *k = 0;
    if ( data.size() < 3 ) return;
    ci = data.begin();
    *o = (ci++)->first;
    *k = ci->first - *o;
    ci_end = ci;
    while ( ++ci != data.end() )
        {
         if ( !mat::IsZero( *k - (ci->first - ci_end->first) ) ) { *k = 0; return; }
         ci_end = ci;
        }
}


//============================= Protected Methods ===========================


//============================= Private Methods =============================

//---------------------------------------------------------------------------
void cls_DataFnTrack::Assign(const cls_DataFnTrack* t2)
{ // Copy in other track

    if ( this!=t2 && t2 )
       {
        // Inherited stuff
        cls_Track::operator=(*t2);
        // My stuff
        data = t2->data;
        i_dtmin = t2->i_dtmin;
        i_dtmax = t2->i_dtmax;
       }
}

//---------------------------------------------------------------------------
void cls_DataFnTrack::AssignDepVar(const numlist_xy& v)
{ // Copy in dependent variable from array

    unsigned int N = (data.size() < v.size()) ? data.size() : v.size();
    typ_YtIt i = data.begin();
    for (unsigned int n=0; n<N; ++n) (i++)->second = v[n];
    UpdateStats();
}


//---------------------------------------------------------------------------
void cls_DataFnTrack::AssignDepVar(const typ_xy*const v, unsigned int N)
{ // Copy in dependent variable from array

    if ( N > data.size() ) N = data.size();
    typ_YtIt i = data.begin();
    for (unsigned int n=0; n<N; ++n) (i++)->second = v[n];
    UpdateStats();
}

//---------------------------------------------------------------------------
void cls_DataFnTrack::UpdateStats()
{ // Recalculate samples statistics, see also 'AddPoint'

    if ( data.empty() )
         {
          i_ymin = i_ymax = mat::NaN;
          i_xmax = i_tmax = i_xmin = i_tmin = mat::NaN;
          i_dtmax = i_dtmin = mat::NaN;
         }
    else {
          i_ymin = i_ymax = data.begin()->second;
          i_xmax = i_tmax = i_xmin = i_tmin = data.begin()->first;
          i_dtmax = 0;
          i_dtmin = mat::SupDouble;

          for ( ci=data.begin(); ci!=data.end(); ++ci ) AddToStats(ci->first, ci->second);
         }
}
//------------------------------- Properties --------------------------------




///////////////////////////// cls_AnalyticFnTrack ///////////////////////////

//============================= Static Members ==============================

//============================= Public methods ==============================

//---------------------------------------------------------------------------
cls_AnalyticFnTrack::cls_AnalyticFnTrack (const String& n) : cls_FunctionTrack(n)
{
    i_tmax = mat::Huge;
    i_tmin = -mat::Huge;
    i_xmin=0;
    i_xmax=10;
    i_ymin=0;
    i_ymax=10;

    i_ky = 1;
    i_oy = 0;
    i_kt = 1;
    i_ot = 0;
}



//---------------------------------------------------------------------------
void cls_AnalyticFnTrack::Statistics( String& s ) const
{ // Get overall statistics

    s = "  Analytic function track";
    s += "\n";
    s += "\n  Independent variable increment: " + String(dtmin) + String("÷") + String(dtmax);
    s += "\n  Independent variable range: " + String(trange) + ", from " + String(tmin) + " to " + String(tmax);
    //s += "\n  Abscissa range: " + String(xrange) + ", from " + String(xmin) + " to " + String(xmax);
    s += "\n";
    s += "\n  Ordinate range: " + String(yrange) + ", from " + String(ymin) + " to " + String(ymax);
}

//---------------------------------------------------------------------------
void __fastcall cls_AnalyticFnTrack::Draw(class cls_Foil* Foil)
{ // TODO: Plot a function track
    ASSERTING((Owner),"Here \'Owner\' should be defined!")

// . Initializations
    static bool skipping; // skipping point state
    skipping = false;
    int kn = 1; // 1 / Owner->SampPerPix;
    // Some drawing quantities
    int DW = Owner->DXpix(dtmin) - 2; // Space between points (Used as lineweight in 'DoFillArea')
    if (DW<1) DW = 1;
    int DH_2 = (DW - LineWeight)/2; // Histograms half width (used in 'DoDrawHistograms')
    int P_2 = PointSize;

// . Move to first existing point
    i_X = (cls_Frame::PlotAlsoOnMargins) ? Owner->Left : Owner->left;
    int Xend = (cls_Frame::PlotAlsoOnMargins) ? Owner->Right : Owner->right;
    //int _Ytop = (cls_Frame::PlotAlsoOnMargins) ? Owner->Top : Owner->top;
    //int _Ybtm = (cls_Frame::PlotAlsoOnMargins) ? Owner->Bottom : Owner->bottom;
    do {
        i_y = operator()( Owner->Xphy(i_X) );
        i_X += kn;
       }
    while ( mat::isNaN(i_y) && i_X < Xend );
    Canvas->MoveTo(i_X,Owner->Ypix(i_y));

// . Iterate all visible points
    while ( i_X < Xend ) // consider frame x pixels
        {
         // Calculate ordinate
         i_y = operator()( Owner->Xphy(i_X) );

         // Skip point if not existing
         if ( mat::isNaN(i_y) )
            {
             skipping = true;
             i_X += kn; // next abscissa
             continue;
            }

         // Skip point if negative and in logarithmic scale
         if ( Owner->YLogarithmic && i_y < mat::log_argmin ) {i_X += kn; continue;}

         // If here we are in domain (ordinate exists)
         i_Y = Owner->Ypix(i_y); // Pixel ordinate

         // Do operations
         if ( skipping )
              { // If preceding point was skipped. move to
               Canvas->MoveTo(i_X,i_Y);
               skipping = false; // See next point
              }
         else { // Draw
               //Canvas->Pen->Color = TrackColor;
               if (FillArea) DoFillArea(i_X,i_Y,DW);
               if (DrawHistograms) DoDrawHistogram(i_X,i_Y,DH_2);
               if (StrokeLines) Canvas->LineTo(i_X,i_Y); // Draw point to point line
               else Canvas->MoveTo(i_X,i_Y);
               if (MarkPoints) DoMarkPoint(i_X,i_Y,P_2);
              }

         i_X += kn; // next abscissa
        }
}

//---------------------------------------------------------------------------
void __fastcall cls_AnalyticFnTrack::UpdatePointQuantities (const typ_t t, mpl::stu_PointQuantities* p)
{// Calculate all point quantities

    if ( IsInDomain(t) )
         {
          p->x = t;
          p->y = operator()(t);
          p->dx = dtmin;
          p->dy = operator()(t+dtmin) - p->y;
          p->dydx = mat::IsZero(p->dx) ? mat::NaN : p->dy/p->dx;
         }
    else {
          p->y = p->x = p->dy = p->dx = p->dydx = mat::NaN;
         }
}

//---------------------------------------------------------------------------
void __fastcall cls_AnalyticFnTrack::UpdateIntervalQuantities (typ_t t1, typ_t t2, mpl::stu_IntervalQuantities* q)
{// Calculate interval quantities

    q->n = 0;
    q->Dt = q->tmin = q->tmax = q->Dy = q->DY = q->ymin = q->ymax = q->DyDt = q->Sy = q->avg = q->rms = q->std = q->stdp = q->varp = mat::NaN;
}




//============================= Protected Methods ===========================

/*
//---------------------------------------------------------------------------
cls_Track& cls_AnalyticFnTrack::Assign(const cls_Track& t2)
{ // TODO 4: Copy in other track

    if ( this == &t2 ) return *this;
    // cls_FoilObject::Assign

    return *this;
}
*/


//============================= Private Methods =============================

//---------------------------------------------------------------------------
cls_Track::typ_t cls_AnalyticFnTrack::get_dtmin() const
{
    ASSERTING((Owner),"Here \'Owner\' should be defined!")
    return Owner->PixelSizeX;
    // TODO 5: return Owner->Xphy(1); // more general (logarithmic scale)
}

//---------------------------------------------------------------------------
cls_Track::typ_t cls_AnalyticFnTrack::get_dtmax() const
{ // Ehmm, this should not be used

    ASSERTING((Owner),"Here \'Owner\' should be defined!")
    return Owner->Xphy(1);
}

/*---------------------------------------------------------------------------
cls_Track::typ_xy cls_AnalyticFnTrack::get_dymin() const
{
    ASSERTING((Owner),"Here \'Owner\' should be defined!")
    return Owner->PixelSizeY;
    // TODO 5: return Owner->Yphy(1); // more general (logarithmic scale)
}*/





//============================== END OF FILE ================================

