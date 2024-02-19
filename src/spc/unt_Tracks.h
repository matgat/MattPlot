//---------------------------------------------------------------------------

#ifndef unt_TracksH
#define unt_TracksH

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * *
    *
    *    unt_Tracks (2008 - Matteo Gattanini)
    *        ver: 09    (08.aug.2008)
    *
    *    This unit defines a class hierarchy to represent
    *    tracks for data plotting
    *
    *    (cls_TrackCollection)
    *     cls_Track
    *        \cls_ParametricTrack
    *        |          \  cls_DataParmTrack
    *        |           | cls_AnalyticParmTrack
    *        |
    *         cls_FunctionTrack
    *                  \  cls_DataFnTrack
    *                   | cls_AnalyticFnTrack
    *                             \  cls_PolynomialTrack
    *                              | cls_HyperbolaTrack
    *                              | cls_ExponentialTrack
    *                              | cls_HarmonicTrack
    *                              | ...
    *    Example of usage:
    *    #include "unt_Tracks.h"
    *
    * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <Graphics.hpp> // for style attributes
//#include <SysUtils.hpp> // for EMathError
#include <vector> // for numlist
#include <map> // for data points
#include "unt_MPlotClasses.h" // MPlot classes hierarchy
#include "unt_Quantities.h" // Axes quantities


//-------------------------------- Settings ---------------------------------
#ifdef _DEBUG
  #define TRK_DEBUG
#endif

// TODO 1: cls_CplxTrack, cls_RealTrack, discard the current hierarchy
// TODO 1: cls_CplxTrack, cls_RealTrack, fried functions for sum etc...
// TODO 2: operators, uniform interface: ForEachPoint, getNext, endReached



///////////////////////////////// cls_Track /////////////////////////////////
//---------------------------------------------------------------------------
class cls_Track : public cls_FrameObject { // Track interface class

public://                                                       ~~~~~~~~~~~~~

// ~ ~ ~ Types definitions
    //enum en_PlotStyles { PS_LINES=1, PS_LINESPOINTS, PS_POINTS, PS_AREA, PS_HISTOGRAMS };
    typedef unsigned int typ_i; // type for value indexes or data size
    typedef double typ_t; // type of independent variable
    typedef double typ_xy; // type of dependent variables

// . . . Public methods
    cls_Track (const String&, class cls_Frame*);
    cls_Track (const cls_Track& t2) : cls_FrameObject(t2.Owner) {Assign(&t2);}; // Copy constructor
    virtual ~cls_Track ()  {;}

    virtual cls_Track& operator=(const cls_Track& t2) {Assign(&t2); return *this;} // Copy assignment
    virtual cls_Track* Clone() =0; // Clone in new track - Warning: user is responsible to free

    // . . . Interface services
    // (inherited from 'cls_FrameObject')
    // Remark: 'Draw' and 'IsOver' are left to subclasses
    virtual void __fastcall Reset() {;} // Reset operations, called on toggling 'Visible'
    virtual void __fastcall Remove() {;} // TODO: Remove request
    // Managing services
    virtual class cls_PropertiesInterface* CreateSpecInterface(class TWinControl*); // Create the edit properties interface
    virtual nms_MPlotClasses::en_FoilObjectType GetType(); // Get a enum type (deprecated)
    virtual typ_ActionIdxs* GetActionsIdxs() const {return &ActionsIdxs;} // Used to build popup menu
    // (other)
    void Stroke (class cls_Foil*, const TPoint&, const TPoint&); // Stroke a tract (used in legend)
    String YLabel() {return (YQuantity.UnitLabel.IsEmpty()) ? Name : Name + String(" [") + YQuantity.UnitLabel + String("]");}
    String XLabel() {return (XQuantity.UnitLabel.IsEmpty()) ? XName : XName + String(" [") + XQuantity.UnitLabel + String("]");}

    // . . . Statistics services
    virtual void Statistics(String&) const =0; // Get overall statistics
    virtual void __fastcall UpdatePointQuantities(const typ_t, mpl::stu_PointQuantities*) =0; // Update point quantities
    virtual void __fastcall UpdateIntervalQuantities (typ_t, typ_t, mpl::stu_IntervalQuantities*) =0; // Update interval quantities
     // TODO:  use mat::stu_Range??  ------------------  ^      ^

    // . . . Contained data services
    // > > > Non modifying
    // Accessing
    virtual typ_xy operator()(typ_t) const =0; // Get nearest dependent value
    virtual typ_xy lininterp(typ_t) const =0; // Interpolate linearly
    virtual typ_xy operator[](typ_i) const =0; // Get value at given index
    virtual bool IsInDomain (const typ_t t) const =0; // tells if independent var is mapped
    bool IsEvenlySpaced() const {return mat::IsZero(dtmax-dtmin);};
    bool operator<(const cls_Track& t2)const { return Size < t2.Size; } // Compare size

    // > > > Modifying
    // - - -  Unary operators
    virtual cls_Track& RescaleShift(const double, const double, const bool) =0; // Linear transformation (Rescale/Shift) on indep/dep var
    virtual cls_Track& ReverseIndepVar() =0; // Reverse independent variable
    virtual cls_Track& abs() =0; // Absolute value
    virtual cls_Track& pow(const double) =0; // Power
    virtual cls_Track& exp() =0; // Exponential
    virtual cls_Track& log() =0; // Natural logarithm
    virtual cls_Track& log2() =0; // Base2 logarithm
    virtual cls_Track& mask16() =0; // mask 16bit
    virtual cls_Track& cast_unsigned16() =0; // Cast to unsigned int 16bit
    virtual cls_Track& Filter(const int) =0; // Averaging filter
    virtual cls_Track& Crop(const mat::stu_Rect&) =0; // Crop by domain
    virtual cls_Track& Spect(const bool) =0; // Transform to spectrum (a real track)
    virtual cls_Track& Derivative() =0; // Derivative
    virtual cls_Track& Integral(const typ_xy C =0) =0; // Integral given the integration constant

    // - - - Binary operators
    // Arithmetic operators with scalar
    virtual cls_Track& operator+ (const typ_xy) =0;
    virtual cls_Track& operator- (const typ_xy) =0;
    virtual cls_Track& operator* (const typ_xy) =0;
    virtual cls_Track& operator/ (const typ_xy) =0;
    // Arithmetic operators with track
    //virtual cls_Track& operator+ (const cls_Track&) =0;
    //virtual cls_Track& operator- (const cls_Track&) =0;
    //virtual cls_Track& operator* (const cls_Track&) =0;
    //virtual cls_Track& operator/ (const cls_Track&) =0;

    virtual cls_Track& Sum(const cls_Track&) =0;
    virtual cls_Track& Subtract(const cls_Track&) =0;
    virtual cls_Track& Multiply(const cls_Track&) =0;
    virtual cls_Track& Divide(const cls_Track&) =0;
    virtual cls_Track& CombineMod(const cls_Track&) =0;
    virtual cls_Track& Atan(const cls_Track&) =0;
    virtual cls_Track& Custom1(const cls_Track&) =0;

    // . . . Exporting services
    virtual void CopyIndepVarTo(mat::numlist*) const =0; // fills a vector of doubles with independent variable
    virtual void CopyDepVarTo(mat::numlist*) const =0; // fills a vector of doubles with dependent variable
    virtual void CopyIndepVarTo(String&, const char) const =0; // fills a string with independent variable
    virtual void CopyDepVarTo(String&, const char) const =0; // fills a string with dependent variable
    virtual void DataToStrings (class TStrings*) const =0; // Fill a stringlist with data
    virtual void StringsToData (class TStrings*) =0; // retrieve data from a stringlist with rows composed by x<tab>y
    virtual bool SameIndepVar( const mat::numlist*) const =0; // Tells if the independent variable coincide with given numlist
    virtual void GetIndepVarProgression(double*,double*) const =0; // Retrieve eventual indep var arithmetic progression

// . . . Properties
    // . . . General attributes
    __property String Name = { read=i_Name, write=set_Name }; // Track name (unique)
    __property String XName = { read=i_XName, write=i_XName }; // Indep var name (unique)
    __property cls_Quantity YQuantity = { read=i_YQuantity, write=i_YQuantity }; // A track value quantity, used to deal with measure units
    __property cls_Quantity XQuantity = { read=i_XQuantity, write=i_XQuantity }; // An indep quantity, used to deal with measure units
    // . . . Contained data
    __property typ_i Size = { read=get_Size }; // Data size
    __property typ_t dtmin = { read=get_dtmin }; // Minimum indep var increment
    __property typ_t dtmax = { read=get_dtmax }; // Maximum indep var increment
    __property typ_t tmax = { read=i_tmax }; // Domain
    __property typ_t tmin = { read=i_tmin };
    __property typ_t trange = { read=get_trange };
    __property typ_xy xmax = { read=i_xmax }; // Abscissa codomain
    __property typ_xy xmin = { read=i_xmin };
    __property typ_xy xrange = { read=get_xrange };
    __property typ_xy ymax = { read=i_ymax }; // Ordinate codomain
    __property typ_xy ymin = { read=i_ymin };
    __property typ_xy yrange = { read=get_yrange }; // Codomain size
    __property typ_xy absymax = { read=get_absymax }; // Maximum absolute ordinate
    // . . . Render options
    __property bool MarkPoints = { read=i_MarkPoints, write=i_MarkPoints };
    __property char PointStyle = { read=i_PointStyle, write=i_PointStyle };
    __property int PointSizeCoeff = { read=i_PointSizeCoeff, write=i_PointSizeCoeff }; // Point size = PointSizeCoeff * LineWeight + 2
    __property int PointSize= { read=get_PointSize };
    __property bool StrokeLines = { read=i_StrokeLines, write=i_StrokeLines };
    __property bool FillArea = { read=i_FillArea, write=i_FillArea };
    __property bool DrawHistograms = { read=i_DrawHistograms, write=i_DrawHistograms };
    __property int HistWidthPerc = { read=i_HistWidthPerc, write=i_HistWidthPerc }; // Histograms width percentage (0 for stems)
    __property bool ShowCoords = { read=i_ShowCoords, write=i_ShowCoords };
    __property int CoordsDist = { read=get_CoordsDist, write=set_CoordsDist };

// . . . Public attributes (!)
    static const String PointStyles; // chars of point styles
    // Style: default
    static int DefaultLineWeight;
    static TColor DefaultLineColor;
    static TPenStyle DefaultLineStyle;
    static char DefaultPointStyle;
    static int DefaultPointSizeCoeff;
    static bool DefaultMarkPoints;
    static bool DefaultStrokeLines;
    static bool DefaultFillArea;
    static bool DefaultDrawHistograms;
    static int DefaultHistWidthPerc;
    static bool DefaultTrackShowCoords;
    static int DefaultTrackCoordsDist;
    // Managing
    static typ_ActionIdxs ActionsIdxs; // Used to build popup menu

protected://                                                    ~~~~~~~~~~~~~

// . . . Protected attributes
    // Auxiliary
    mutable typ_xy i_y, i_x, i_dx, i_dy; // coordinates and other auxiliary for draw
    mutable int i_X, i_Y; // pixel coordinates
    #ifdef DRAWTRACK_PIXELSINCREMENTS
    mutable int i_DX, i_DY; // pixel increments
    #endif
    mutable typ_t i_dt; // Used in 'lininterp'

// . . . Properties
    // General
    String i_Name, i_XName;
    void set_Name(String);
    cls_Quantity i_YQuantity, i_XQuantity;
    // Contained data
    virtual typ_i get_Size() const =0;
    virtual typ_t get_dtmin() const =0;
    virtual typ_t get_dtmax() const =0;
    //virtual typ_xy get_dymin() const =0;
    typ_t i_tmin, i_tmax;
    typ_t get_trange() const {return mat::EnsureNotZero(i_tmax-i_tmin);}
    typ_xy i_xmin, i_xmax;
    typ_xy i_ymin, i_ymax;
    typ_xy get_xrange() const {return mat::EnsureNotZero(i_xmax-i_xmin);}
    typ_xy get_yrange() const {return mat::EnsureNotZero(ymax-ymin);}
    typ_xy get_absymax() const { return mat::max(std::fabs(ymax),std::fabs(ymin)); }
    // Style
    bool i_MarkPoints;
    bool i_StrokeLines;
    bool i_FillArea;
    bool i_DrawHistograms;
    int i_HistWidthPerc;
    char i_PointStyle;
    int i_PointSizeCoeff;
    int get_PointSize() const {return i_PointSizeCoeff*LineWeight + 2;}
    bool i_ShowCoords;
    int i_CoordsDist2;
	int get_CoordsDist() const {return std::sqrt(double(i_CoordsDist2));}
    void set_CoordsDist(const int d) {i_CoordsDist2 = d*d;}

// . . . Protected methods
    // Rendering services
    inline void DoMarkPoint(const int, const int, const int); // Mark a point
    inline void DoShowCoords(const int); // Show current point coordinates
    inline void DoFillArea (const int, const int, const int); // Fill area under point
    inline void DoDrawHistogram (const int, const int, int); // Draw an histogram for point

private://                                                      ~~~~~~~~~~~~~
// . . . Private attributes

// . . . Private methods
    void Assign(const cls_Track*); // Copy in other track

}; // end 'cls_Track'

// TODO: remove
//---------------------------------------------------------------------------
class cls_FunctionTrack : public cls_Track { // a real function of one real variable
/*
     /  y = f(t)
    <               =>   y = f(x)
     \  x = t
*/
public://                                                       ~~~~~~~~~~~~~
// . . .  Types definitions

// . . . Public methods
    cls_FunctionTrack (const String& n) : cls_Track(n,0) {;}
    cls_FunctionTrack (const cls_FunctionTrack& t2) : cls_Track(t2){;} // Copy constructor
    virtual ~cls_FunctionTrack ()  {;}

    // Inherited
    // Remark: Draw is left to subclasses
    // TODO 3: see if can be moved to cls_Track
    virtual bool __fastcall IsOver(const TPoint&) const; // A point is over track

};//-------------------------------------------------------------------------


//////////////////////////// cls_ParametricTrack ////////////////////////////
//---------------------------------------------------------------------------
class cls_ParametricTrack : public cls_Track { // a complex function of one real variable
/*
     /  y = fy(t)
    <               2d coordinates
     \  x = fx(t)
*/
public://                                                       ~~~~~~~~~~~~~
// . . .  Types definitions

// . . . Public methods
    cls_ParametricTrack (const String& n) : cls_Track(n,0) {;}
    virtual ~cls_ParametricTrack ()  {;}

    // Inherited
    //virtual void __fastcall Draw(class cls_Foil*); // Render on paren't Camvas
    //virtual bool __fastcall IsOver(const TPoint&) const; // A point is over track

    // . . . Statistics services
    virtual void Statistics(String&) const; // Get overall statistics

// . . . Operators
// . . . Properties
// . . . Public attributes (!)

protected://                                                    ~~~~~~~~~~~~~

// . . . Protected methods

private://                                                      ~~~~~~~~~~~~~

// . . . Private attributes
// . . . Properties
// . . . Private methods

};//-------------------------------------------------------------------------




//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%% Specializations of function tracks %%%%%%%%%%%%%%%%%%%


////////////////////////////// cls_DataFnTrack //////////////////////////////
//---------------------------------------------------------------------------
class cls_DataFnTrack : public cls_FunctionTrack  { // a set of points
//                        public std::map<cls_Track::typ_t,cls_Track::typ_xy> { // a set of points
/*
     /  y = correspondingto(x)
    <                           2d coordinates
     \  x = nearest(t)
*/
public://                                                       ~~~~~~~~~~~~~
// . . .  Types definitions
    typedef std::vector<cls_Track::typ_xy> numlist_xy;
    typedef std::vector<cls_Track::typ_t> numlist_t;
    typedef std::map<cls_Track::typ_t,cls_Track::typ_xy> typ_Yt;
    typedef typ_Yt::iterator typ_YtIt;
    typedef typ_Yt::const_iterator typ_YtConstIt;
    typedef std::pair<typ_YtConstIt,typ_YtConstIt> typ_ci_pair;
    //typedef std::pair<typ_t,typ_xy> typ_datapoint;

// . . . Public methods
    cls_DataFnTrack(const String&);
    cls_DataFnTrack (const cls_DataFnTrack& t2) : cls_FunctionTrack(t2) {Assign(&t2);} // Copy constructor

    virtual ~cls_DataFnTrack ()  {;}

    // Copy
    //virtual cls_Track& operator=(const cls_DataFnTrack& t2) {Assign(&t2); return *this;} // Copy assignment
    virtual cls_Track* Clone() {return new cls_DataFnTrack(*this);} // Clone in new track - Warning: user is responsible to free

    // . . . Interface services
    virtual void __fastcall Draw(class cls_Foil*);

    // . . . Statistics services
    virtual void Statistics(String&) const; // Get overall statistics
    virtual void __fastcall UpdatePointQuantities (const typ_t, mpl::stu_PointQuantities*);
    virtual void __fastcall UpdateIntervalQuantities (typ_t, typ_t, mpl::stu_IntervalQuantities*);

    // . . . Contained data services
    // > > > Non modifying
    // Accessing
    virtual typ_xy operator()(typ_t t) const {return data.lower_bound(t)->second;}
    virtual typ_xy lininterp(typ_t t) const {
                                             typ_ci_pair ri = data.equal_range(t);
                                             if ( ri.second==data.end() ) return ri.first->second; // mat::NaN;
                                             else if ( ri.second==ri.first && ri.first!=data.begin() )
                                                { // Must interpolate
                                                 --ri.first; // Prev value
                                                 i_dt = ri.second->first - ri.first->first;
                                                 i_dy = ri.second->second - ri.first->second;
                                                 return mat::IsZero(i_dt) ? ri.first->second : ri.first->second + (t-ri.first->first)*(i_dy/i_dt);
                                                }
                                             else return ri.first->second;
                                            }
    virtual typ_xy operator[](typ_i k) const {ci=data.begin(); for(typ_i i=0; i<k; ++i)++ci; return ci->second;}
    virtual bool IsInDomain (const typ_t t) const {return (t>=i_tmin)&&(t<=i_tmax);}
    // > > > Modifying
    virtual cls_Track& RescaleShift(const double, const double,const bool);
    virtual cls_Track& ReverseIndepVar();
    virtual cls_Track& abs(); // Absolute value
    virtual cls_Track& pow(const double); // Power
    virtual cls_Track& exp(); // Exponential
    virtual cls_Track& log(); // Natural logarithm
    virtual cls_Track& log2(); // Base2 logarithm
    virtual cls_Track& mask16(); // mask 16bit
    virtual cls_Track& cast_unsigned16(); // Cast to unsigned int 16bit
    virtual cls_Track& Filter(int); // Averaging filter
    virtual cls_Track& Crop(const mat::stu_Rect&); // Crop by abscissa domain
    virtual cls_Track& Spect(const bool); // Transform to spectrum
    virtual cls_Track& Derivative(); // Derivative
    virtual cls_Track& Integral(const typ_xy =0); // Integral given the integration constant

    // Arithmetic operators with scalar
    virtual cls_Track& operator+ (const typ_xy);
    virtual cls_Track& operator- (const typ_xy k) {return operator+(-k);}
    virtual cls_Track& operator* (const typ_xy);
    virtual cls_Track& operator/ (const typ_xy);
    // Arithmetic operators with track
    //virtual cls_Track& operator+ (const cls_Track& t2) {return Sum(t2);}
    //virtual cls_Track& operator- (const cls_Track& t2) {return Subtract(t2);}
    //virtual cls_Track& operator* (const cls_Track& t2) {return Multiply(t2);}
    //virtual cls_Track& operator/ (const cls_Track& t2) {return Sum(Divide);}
    virtual cls_Track& Sum(const cls_Track&);
    virtual cls_Track& Subtract(const cls_Track&);
    virtual cls_Track& Multiply(const cls_Track&);
    virtual cls_Track& Divide(const cls_Track&);
    virtual cls_Track& CombineMod(const cls_Track&);
    virtual cls_Track& Atan(const cls_Track&);
    virtual cls_Track& Custom1(const cls_Track&);

    // . . . Exporting services
    virtual void CopyIndepVarTo(mat::numlist*) const; // fills a vector of doubles with independent variable
    virtual void CopyDepVarTo(mat::numlist*) const; // fills a vector of doubles with dependent variable
    virtual void CopyIndepVarTo(String&, const char) const; // fills a string with independent variable
    virtual void CopyDepVarTo(String&, const char) const; // fills a string with dependent variable
    virtual void DataToStrings (class TStrings*) const; // Fill a stringlist with data
    virtual void StringsToData (class TStrings*); // retrieve data from a stringlist with rows composed by x<tab>y
    virtual bool SameIndepVar( const mat::numlist*) const; // Tells if the independent variable coincide with given numlist
    virtual void GetIndepVarProgression(double*,double*) const; // Retrieve eventual indep var arithmetic progression

    // . . . Specific services
    //-----------------------------------------------------
    void __fastcall AddToStats (const typ_t t, const typ_xy v)
        { // Maintain samples statistics
         // !Warning: in this way dx calculation fails when fill is not ordered
         // last_t should be the 'previous' t, from lower_bound
         if ( data.empty() )
              { // Initialize
               i_xmax = i_tmax = i_xmin = i_tmin = last_t = t;
               i_ymin = i_ymax = v;
               i_dtmax = 0;
               i_dtmin = mat::SupDouble;
              }
         else {
               if ( i_tmin > t ) i_xmin = i_tmin = t;
               else if ( i_tmax < t ) i_xmax = i_tmax = t;
               if ( i_ymin > v ) i_ymin = v;
               else if ( i_ymax < v ) i_ymax = v;

			   last_t = std::fabs(t-last_t); // Ehmm, now 'last_t' is 'curr_dt'
               if ( i_dtmin > last_t ) i_dtmin = last_t;
               if ( i_dtmax < last_t ) i_dtmax = last_t;
               last_t = t;
              }
        }
    //-----------------------------------------------------
    void __fastcall AddPoint (const typ_t t, const typ_xy v)
        { // Add a point to track
         AddToStats(t,v);
         // Add to container
         data[t] = v; //data.insert(data.rbegin(),typ_datapoint(px,py));
        }

// . . . Properties

// . . . Public attributes (!)

protected://                                                    ~~~~~~~~~~~~~

// . . . Protected methods
    // Inherited from 'cls_Track'
    // Inherited from 'cls_FunctionTrack'

// . . . Properties
    // Contained data
    typ_Yt data;
    virtual typ_i get_Size() const { return data.size(); }
    typ_t i_dtmin, i_dtmax;
    virtual typ_t get_dtmin() const { return i_dtmin; }
    virtual typ_t get_dtmax() const { return i_dtmax; }
    //virtual typ_xy get_dymin() const { return i_dymin; }

private://                                                      ~~~~~~~~~~~~~
// . . . Private attributes
    mutable typ_YtConstIt ci,ci_end;
    mutable typ_t last_t;
    mutable double px,py; // skip inner points in Draw

// . . . Private methods
    void Assign(const cls_DataFnTrack*); // Copy in other track
    void AssignDepVar(const numlist_xy&); // Copy in dependent variable from array
    void AssignDepVar(const typ_xy*const, unsigned int);

    void UpdateStats(); // Recalculate samples statistics
    void Fill(typ_t*, typ_xy*) const; // Fill the arrays with data
    void Fill(numlist_xy&, numlist_xy&) const;

}; // end 'cls_DataFnTrack'



///////////////////////////// cls_AnalyticFnTrack ///////////////////////////
//---------------------------------------------------------------------------
class cls_AnalyticFnTrack : public cls_FunctionTrack { // a function
/*
     /  y = f(x)
    <             2d coordinates
     \  x = t
*/
public://                                                       ~~~~~~~~~~~~~
// . . .  Types definitions

// . . . Public methods
    cls_AnalyticFnTrack (const String&);
    cls_AnalyticFnTrack (const cls_AnalyticFnTrack& t2) : cls_FunctionTrack(t2) {Assign(&t2);} // Copy constructor
    virtual ~cls_AnalyticFnTrack ()  {;}

    virtual cls_Track* Clone() {return 0;} // TODO 3: Clone in new track - Warning: user is responsible to free

    // . . . Interface services
    virtual void __fastcall Draw(class cls_Foil*); // Render on paren't Camvas

    // . . . Statistics services
    virtual void Statistics(String&) const; // Get overall statistics
    virtual void __fastcall UpdatePointQuantities (const typ_t, mpl::stu_PointQuantities*); // Calculate all point quantities
    virtual void __fastcall UpdateIntervalQuantities (typ_t, typ_t, mpl::stu_IntervalQuantities*); // Calculate interval quantities

    // . . . Contained data services
    // > > > Non modifying
    // Accessing
    virtual typ_xy operator()(typ_t t) const {return IsInDomain(t) ? i_ky*fn(i_kt*t+i_ot)+i_oy : mat::NaN;}
    virtual typ_xy lininterp(typ_t t) const {return operator()(t);}
    virtual typ_xy operator[] (typ_i k) const {return operator()(k*dtmin);}
    // > > > Modifying
    virtual cls_Track& RescaleShift(const double k, const double o, const bool b) {if(b){i_ky*=k; i_oy+=o;} else{i_kt*=k; i_ot+=o;} return *this;}
    virtual cls_Track& ReverseIndepVar() {return *this;}
    virtual cls_Track& abs() {return *this;}
    virtual cls_Track& pow(const double e) {return *this;} // Power
    virtual cls_Track& exp() {return *this;} // Exponential
    virtual cls_Track& log() {return *this;} // Natural logarithm
    virtual cls_Track& log2() {return *this;} // Base2 logarithm
    virtual cls_Track& mask16() {return *this;} // mask 16bit
    virtual cls_Track& cast_unsigned16() {return *this;} // Cast to unsigned int 16bit
    virtual cls_Track& Filter(const int) {return *this;} // Averaging filter
    virtual cls_Track& Crop(const mat::stu_Rect&) {return *this;} // Crop by abscissa domain
    virtual cls_Track& Spect(const bool) {return *this;} // Transform to spectrum
    virtual cls_Track& Derivative(){return *this;} // TODO: Derivative
    virtual cls_Track& Integral(const typ_xy){return *this;} // TODO: Integral given the integration constant
    // Arithmetic operators with scalar
    virtual cls_Track& operator+ (const typ_xy) {return *this;}
    virtual cls_Track& operator- (const typ_xy) {return *this;}
    virtual cls_Track& operator* (const typ_xy) {return *this;}
    virtual cls_Track& operator/ (const typ_xy) {return *this;}
    // Arithmetic operators with track
    //virtual cls_Track& operator+ (const cls_Track&) {return *this;}
    //virtual cls_Track& operator- (const cls_Track&) {return *this;}
    //virtual cls_Track& operator* (const cls_Track&) {return *this;}
    //virtual cls_Track& operator/ (const cls_Track&) {return *this;}

    virtual cls_Track& Sum(const cls_Track&) {return *this;}
    virtual cls_Track& Subtract(const cls_Track&) {return *this;}
    virtual cls_Track& Multiply(const cls_Track&) {return *this;}
    virtual cls_Track& Divide(const cls_Track&) {return *this;}
    virtual cls_Track& CombineMod(const cls_Track&) {return *this;}
    virtual cls_Track& Atan(const cls_Track&) {return *this;}
    virtual cls_Track& Custom1(const cls_Track&) {return *this;}

    // . . . Exporting services
    virtual void CopyIndepVarTo(mat::numlist*) const {;} // fills a vector of doubles with independent variable
    virtual void CopyDepVarTo(mat::numlist*) const {;} // fills a vector of doubles with dependent variable
    virtual void CopyIndepVarTo(String&, const char) const {;} // fills a string with independent variable
    virtual void CopyDepVarTo(String&, const char) const {;} // fills a string with dependent variable
    virtual void DataToStrings (class TStrings*) const {;} // Fill a stringlist with data
    virtual void StringsToData (class TStrings*) {;} // retrieve data from a stringlist with rows composed by x<tab>y
    virtual bool SameIndepVar( const mat::numlist*) const {return false;} // Tells if the independent variable coincide with given numlist
    virtual void GetIndepVarProgression(double*,double*) const {;} // Retrieve eventual indep var arithmetic progression


// . . . Operators
// . . . Properties
// . . . Public attributes (!)

protected://                                                    ~~~~~~~~~~~~~

// . . . Protected methods
    virtual typ_xy fn (const typ_xy) const =0; // defined in specializations

// . . . Properties
    // Global data
    virtual typ_i get_Size() const { return trange/dtmin; }
    virtual typ_t get_dtmin() const;
    virtual typ_t get_dtmax() const;
    //virtual typ_xy get_dymin() const;

private://                                                      ~~~~~~~~~~~~~

// . . . Private attributes
    typ_xy i_ky, i_oy; // dependent variable scale and offset
    typ_t i_kt, i_ot; // independent variable scale and offset
// . . . Properties
// . . . Private methods
    void Assign(const cls_AnalyticFnTrack*) {;} // Copy in other track

};//-------------------------------------------------------------------------


//////////////////////////// cls_PolynomialTrack ////////////////////////////
//---------------------------------------------------------------------------
class cls_PolynomialTrack : public cls_AnalyticFnTrack { // polynome

public://                                                       ~~~~~~~~~~~~~
// . . .  Types definitions

// . . . Public methods
    cls_PolynomialTrack (const String& n) : cls_AnalyticFnTrack(n) {;}
    virtual bool IsInDomain (const typ_t t) const {return true;} // tells if independent var is mapped

protected://                                                    ~~~~~~~~~~~~~

// . . . Protected methods
    virtual typ_xy fn(const typ_xy w) const {return w*w-w;} // function

private://                                                      ~~~~~~~~~~~~~
};//-------------------------------------------------------------------------


///////////////////////////// cls_HyperbolaTrack ////////////////////////////
//---------------------------------------------------------------------------
class cls_HyperbolaTrack : public cls_AnalyticFnTrack { // hyperbola

public://                                                       ~~~~~~~~~~~~~
// . . .  Types definitions

// . . . Public methods
    cls_HyperbolaTrack (const String& n) : cls_AnalyticFnTrack(n)
        {
         i_xmin = -10; i_xmax = 10;
         i_ymin = -10; i_ymax = 10;
        }
	virtual bool IsInDomain (const typ_t t) const {return std::fabs(t)>mat::eps;}

// . . . Properties
// . . . Public attributes (!)

protected://                                                    ~~~~~~~~~~~~~

// . . . Protected methods
    virtual typ_xy fn(const typ_xy w) const {return 1/w;} // function

private://                                                      ~~~~~~~~~~~~~
};//-------------------------------------------------------------------------


//////////////////////////// cls_ExponentialTrack ///////////////////////////
//---------------------------------------------------------------------------
class cls_ExponentialTrack : public cls_AnalyticFnTrack { // exponential

public://                                                       ~~~~~~~~~~~~~
// . . .  Types definitions

// . . . Public methods
    cls_ExponentialTrack (const String& n) : cls_AnalyticFnTrack(n)
        {
         i_xmin = -10; i_xmax = 10;
         i_ymin = -10; i_ymax = 20;
        }
	virtual bool IsInDomain (const typ_t t) const {return std::fabs(t)<std::log(mat::SupDouble);}

protected://                                                    ~~~~~~~~~~~~~

// . . . Protected methods
	virtual typ_xy fn(const typ_xy w) const {return std::exp(w);} // function ::Power(M_E,x);

private://                                                      ~~~~~~~~~~~~~
};//-------------------------------------------------------------------------


////////////////////////////// cls_HarmonicTrack ////////////////////////////
//---------------------------------------------------------------------------
class cls_HarmonicTrack : public cls_AnalyticFnTrack { // sin

public://                                                       ~~~~~~~~~~~~~
// . . .  Types definitions

// . . . Public methods
    cls_HarmonicTrack (const String& n) : cls_AnalyticFnTrack(n)
        {
         i_ymin = -1.1; i_ymax = 1.1;
         i_xmin = -3.14; i_xmax = 6.28;
        }
    virtual bool IsInDomain (const typ_t t) const {return true;}
protected://                                                    ~~~~~~~~~~~~~

// . . . Protected methods
    virtual typ_xy fn(const typ_xy w) const {return std::sin(w);} // function

private://                                                      ~~~~~~~~~~~~~
};//-------------------------------------------------------------------------


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%% Specializations of parametric tracks %%%%%%%%%%%%%%%%%%%



//------------------------------- Utilities ---------------------------------


//---------------------------------------------------------------------------
class cls_CompareSize_Track { // Functional to sort tracks by size
public:
    bool operator() (const class cls_Track* t1, const class cls_Track* t2) const {return t1->Size < t2->Size;}
};


#endif //--------------------------------------------------------------------

