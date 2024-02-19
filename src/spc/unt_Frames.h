//---------------------------------------------------------------------------
#ifndef unt_FramesH
#define unt_FramesH

        /* * * * * * * * * * * * * * * * * * * * * * * * * * * *
        *
        *        unt_Frames  - Matteo Gattanini
        *                ver: 095   (jan.2008)
        *
        *   cls_SizedObject        cls_FoilObject
        *                  \       /
        *                  cls_Frame
        *
        *        This unit defines frame class
        *
        * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*-------------------------- Geometric attributes ---------------------------
    -----> x
    ____________________________CANVAS________________________
 | |                    Parent->Margin                        |
 | |  o----------------------------------------------------o  |
 | |  | Left,Top            Margin               Right,Top |  |
 | |  | o- - - - - - - - - - - - - - - - - - - - - - - - o |  |
 V |  |M| left,top                             right,top |M|  |
 y |  |a|      .   .                             ...     |a|  |
   |  |r|     .  .  .     plot area     ..    ...        |r|  |
   |  |g|    .      .  ..            ..    . .           |g|  |
   |  |i|    .       .   .          .      ..            |i|  |
   |  |n|    .            .       .                      |n|  |
   |  | |   .              ......                        | |  |
   |  | o- - - - - - - - - - - - - - - - - - - - - - - - o |  |
   |  |  left,bottom        Margin           right,bottom  |  |
   |  o----------------------------------------------------o  |
   |   Left,Bottom      Parent->Margin         Right,Bottom   |
    ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
   Linear scale              Logarithmic scale
   Z = Kz z + Zo          Z = Kz Log(z) + Zo
   Kz = pixels per unit   Kz = pixels per decade
   Zo <=> 0               Zo <=> 1                                         */

//-------------------------------- Settings ---------------------------------
#ifdef _DEBUG
  #define FRM_DEBUG
#endif

//------------------------------ Extern files -------------------------------
#include <list> // for tracks collection
#include "unt_MPlotClasses.h" // MattPlot classes hierarchy
#include "unt_Tracks.h" // to inline


///////////////////////////////// cls_Frame /////////////////////////////////
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class cls_Frame : public cls_FoilObject, public cls_SizedObject { // Plot frame

public://                                                       ~~~~~~~~~~~~~

// ~ ~ ~ Types definitions
    typedef std::list<class cls_Track*> typ_Tracks;
    typedef typ_Tracks::iterator typ_TracksIt;
    typedef typ_Tracks::const_iterator typ_TracksConstIt;
    typedef typ_Tracks::reverse_iterator typ_TracksRevIt;

// . . . Public methods
    cls_Frame(class cls_MPlot* const);
    ~cls_Frame();

    // . . . Inherited (cls_FoilObject)
    virtual void __fastcall Draw(class cls_Foil*); // Render on parent's Canvas
    virtual bool __fastcall IsOver(const TPoint& P)  const {return mat::IsInRect(P,BoundsRect);} // Tell if a Canvas point is over
    virtual void __fastcall Reset(); // Reset operations, called on toggling 'Visible'
    virtual void __fastcall Remove() {;} // TODO: Remove request
    // Managing services
    virtual class cls_PropertiesInterface* CreateSpecInterface(class TWinControl*); // Create the edit properties interface
    virtual nms_MPlotClasses::en_FoilObjectType GetType(); // Get a enum type (deprecated)
    virtual typ_ActionIdxs* GetActionsIdxs() const {return &ActionsIdxs;} // Used to build popup menu
    //virtual void Assign(const class cls_FoilObject*);
    //virtual bool IsDifferentFrom( const class cls_FoilObject*);

    // View services
    void AssignScale (const cls_Frame*); // Assign frame scale
    void AssignScaleX(const cls_Frame*); // Assign frame X scale
    void AssignScaleY(const cls_Frame*); // Assign frame Y scale
    void FitScale(const bool); // Fit scale to all contained tracks (and axes)
    void FitScale (const cls_Track*); // Adapt scales to track space occupation
    void FitScaleX(const cls_Track*); // Adapt X scale to track x space occupation
    void FitScaleY(const cls_Track*); // Adapt Y scale to track y space occupation
    void ResetOriginX(); // Adapt space to view x axis
    void ResetOriginY(); // Adapt space to view y axis
    void ResetOrigin(); // Adapt space to view axes
    void Pan(const int&, const int&); // Modify mapped space offset given a pixel displacement
	void Zoom (const TRect&); // Modify space-transformation to map a new space
    // Auxiliary
    void ApplyOwnerOptions(); // Apply owner options
    bool IsXaxisVisible() {return (Yo >= -MarginY) && (Yo <= height+MarginY);} // Abscissa axis is visible
    bool IsYaxisVisible() {return (Xo >= -MarginX) && (Xo <= width+MarginX);} // Ordinate axis is visible

    // Space trasformation
    inline double Yphy (const int Y) const
            { // Anti-transform y canvas axis to physical space
             ASSERTING((Ky>0),"y scale must be >0!")
			 if ( YLogarithmic ) return std::pow(10,((bottom-Yo)-Y)/Ky); // y = 10^(- ( Yc - (bottom-Yor) ) / Ky)
             else return ((bottom-Yo)-Y)/Ky; // y = - ( Yc - (bottom-Yor) ) / Ky
            }
    inline int Ypix (const double y) const
            { // Transform y-axis value to canvas space
             if ( YLogarithmic )
                {
				 if (y>0) return -mat::round(Ky*std::log10(y)) + (bottom-Yo); // Yc = -Ky Log(y) +(bottom-Yor)
                 else return mat::MaxInt;
                }
             else return -mat::round(Ky*y) + (bottom-Yo); // Yc = -Ky (y) +(bottom-Yor)
            }
    inline double Xphy (const int X) const
            { // Anti-transform x canvas axis to physical space
             ASSERTING((Kx>0),"x scale must be >0!")
			 if ( XLogarithmic ) return std::pow(10,(X-left-Xo)/Kx); // x = 10^(( Xc - (left+Xor) ) / Kx)
             else return (X-left-Xo)/Kx; // x = ( Xc - (left+Xor) ) / Kx
            }
    inline int Xpix (const double x) const
            { // Transform y-axis value to canvas space
             if ( XLogarithmic )
                {
				 if (x>0) return mat::round(Kx*std::log10(x)) + left+Xo; // Xc = Kx Log(x) + X0r+left
                 else return -mat::MaxInt;
                }
             else return mat::round(Kx*x) + (left+Xo); // Xc = Kx x + X0r+left
            }
    // Rectangle version
    inline mat::stu_Rect Rphy(const TRect& R) const {return mat::stu_Rect(Xphy(R.left),Yphy(R.top),Xphy(R.right),Yphy(R.bottom));}
    // Delta transformations
	inline double DYphy (const int Y) const {if(YLogarithmic) return std::pow(10,Y/Ky); else return Y/Ky;}
	inline double DXphy (const int X) const {if(XLogarithmic) return std::pow(10,X/Kx); else return X/Kx;}
	inline int DYpix (const double y) const {if(YLogarithmic) return mat::round(Ky*std::log10(y)); else return mat::round(Ky*y);}
	inline int DXpix (const double x) const {if(XLogarithmic) return mat::round(Kx*std::log10(x)); else return mat::round(Kx*x);}

    // . . . Tracks collection
    // Status
    void CheckInvariant(); // Maintain tracks collection coherence
    bool Empty() const {return tracks.empty();} // Tell is is empty
    uint TracksCount() const {return tracks.size();} // Contained number
    //typ_TracksIt FindTrack(const class cls_Track*const pt) const {for (ctr=tracks.begin(); ctr!=tracks.end(); ++ctr) if (pt == *ctr) return ctr; return tracks.end();} // Find pointer
    typ_TracksIt FindTrack(const class cls_Track*const t) {for (tr=tracks.begin(); tr!=tracks.end(); ++tr) if (t == *tr) return tr; return tracks.end();} // Find pointer
    typ_TracksIt FindTrack(const String& nm) {for (tr=tracks.begin(); tr!=tracks.end(); ++tr) if ((*tr)->Name==nm) break; return tr;} // First matching-name track
    typ_TracksIt FindTrack(unsigned int pos) { for (tr=tracks.begin(); tr!=tracks.end() && pos>0; ++tr,--pos); return tr;} // Get track at position
    bool ContainsTrack (const class cls_Track*const t) {return (FindTrack(t) != tracks.end());}
    void GetTracksSpace(mat::stu_Rect&) const; // Get the Rectangle in physical space containing all tracks
    typ_TracksIt OverTrack (const TPoint& P) {for (tr=tracks.begin(); tr!=tracks.end(); ++tr) if((*tr)->IsOver(P)) break; return tr;} // Get a track that contains a screen point; returns 'tracks.end()' if not found
    //bool SetCurrentTrack (const TPoint& P) {tr=OverTrack(P); if (tr!=tracks.end()) {CurrentTrackIt=tr; return true;} else return false;}
    void FillStringList(class TStrings*) const; // Fill a stringlist with track names and pointers
    void DisplayTrackStatistics(); // Display statistics of selected track
    // Modifying
    void ReassignTrackColors(); // Reset color seed and reassign track colors
    void SelectNextTrack(const bool =false); // Select next track
    void SelectPrevTrack(const bool =false); // Select previous track
    void SelectFirstTrack(); // Select first track
    void SelectLastTrack(); // Select last track
    void PutCurrTrackInForeground(); // Put current track in foreground
    void SelectTrackByPos (const unsigned int pos) {tr=FindTrack(pos); if (tr!=tracks.end()) CurrentTrackIt=tr;} // Select track by position in list
    bool InsertTrack(class cls_Track*); // Add a track, managing name conflicts
    bool InsertTracks(typ_Tracks*); // Add a list of tracks
    void MoveTrack(class cls_Track* tr1, class cls_Track* tr2) {MoveTrack(FindTrack(tr1), FindTrack(tr2));} // Move an owned track near another track
    void MoveTrack (class cls_Track* t, class cls_Frame* f) {MoveTrack(FindTrack(t),f);} // Trasfer track to another frame
    void MoveTrack (cls_Frame* f) {MoveTrack(CurrentTrackIt,f);} // Move current track
    void MoveTracks (cls_Frame*);// Trasfer all tracks to another frame
    void RemoveTrack (typ_TracksIt); // Deallocate a track
    void RemoveTrack () { RemoveTrack(CurrentTrackIt); } // Deallocate current track
    void ClearTracks(); // Clear all tracks
    void Crop(const mat::stu_Rect&); // Crop all tracks abscissa domain
    // Import-Export
    bool ToText(const String&, const char =mat::InvalidChar); // Convert frame tracks to text file
    bool ToMfile(const String&); // Convert frame tracks to m file
    void TracksToColumns(typ_Tracks*, class cls_Columns*); // Convert a set of tracks to a set of data columns
    //void ColumnsToTracks(class cls_Columns*, typ_Tracks*); // Convert a set of data columns to a set of tracks
    bool ImportColumns(class cls_Columns*); // Read tracks from data columns
    void FillTrackBag(typ_Tracks*, const bool =false) const; // Fill a track bag with owned tracks sorted by size or not

    // . . . Other services
    //inline String FormatPoint_abs(const double y, const double x) const {return Owner->ToString(x) + String(" ; ") + Owner->ToString(y);} // Get the string representation of a point depending on general representation settings
    inline String FormatPoint_sca(const double y, const double x) const {return XToString(x) + String(" ; ") + YToString(y);} // Get the string representation of a point to precision of current scale
    inline String YToString (const double v) const {return mat::DoubleToString(v,Yfltprc,Yfltdgt);} // Convert a value to string according to precision of current y scale
    inline String XToString (const double v) const {return mat::DoubleToString(v,Xfltprc,Xfltdgt);} // Convert a value to string according to precision of current x scale
    void DrawVLine (class TCanvas* c, const int X) const {if(X<left||X>right) return; c->MoveTo(X,bottom); c->LineTo(X,top);} // Draw a vertical line through a frame
    void DrawHLine (class TCanvas* c, const int Y) const {if(Y<top||Y>bottom) return; c->MoveTo(left,Y); c->LineTo(right,Y);} // Draw a horizontal line through a frame
    // Auxiliary
    TColor GetNextTrackColor(); // Get a color to assign to a new track

// . . . Operators
    // operator+ with other frames?

// . . . Properties
    // Main
    __property String Name = { read=get_Name }; // Frame name
    __property String Title = { read=i_Title, write=i_Title }; // Frame title
    // State
    __property typ_TracksIt CurrentTrackIt = {read=i_CurrentTrackIt, write=i_CurrentTrackIt};
    __property class cls_Track* CurrentTrack = {read=get_CurrentTrack, write=set_CurrentTrack};
    //__property mat::cls_Color TrackCurrentColor = { read=i_TrackCurrentColor, write=i_TrackCurrentColor };
    // Scales
    __property double xrange = {read=i_xrange}; // X mapped space
    __property double xmin = {read=i_xmin};
    __property double xmax = {read=get_xmax};
    __property double xinf = {read=get_xinf}; // extremes
    __property double xsup = {read=get_xsup};
    __property double yrange = {read=i_yrange}; // Y mapped space
    __property double ymin = {read=i_ymin};
    __property double ymax = {read=get_ymax};
    __property TPoint O = {read=get_O}; // Axes origin position in Canvas
    __property int SampPerPix = {read=i_SampPerPix, write=i_SampPerPix}; // Undersampling option
    __property double PixelSizeX = {read=get_PixelSizeX}; // Pixel horizontal size [unit]
    __property double PixelSizeY = {read=get_PixelSizeY}; // Pixel vertical size [unit]
    __property bool YLogarithmic = {read=i_YLogarithmic, write=set_YLogarithmic}; // Ordinate logarithmic scale
    __property bool XLogarithmic = {read=i_XLogarithmic, write=set_XLogarithmic}; // Abscissa logarithmic scale
    // Tracks collection
    __property String LongestTrackName = { read=i_LongestTrackName }; // Used in Legend
    // Auxiliary
    __property int Row = {read=i_Row, write=i_Row}; // Layout facility
    __property int Tag = {read=i_Tag, write=i_Tag}; // A Tag

// . . . Public attributes (!)
    class cls_MPlot* Owner; // Parent plot
    static int Number; // Overall number

    // Tracks collection
    typ_Tracks tracks; // TODO 5: pass to private
    mutable typ_TracksIt tr; // track iterator
    mutable typ_TracksConstIt ctr; // const track iterator
    mutable typ_TracksRevIt rtr; // reverse track iterator

    // Owned objects
    class cls_Axes* Axes;
    class cls_Grid* Grid;
    class cls_Legend* Legend;
    cls_Markers Markers;
    cls_Labels Labels;

    // Default mapped space
    static const double xmin_def,xrange_def,ymin_def,yrange_def;

    // Default style
    static bool PlotAlsoOnMargins; // Extend plot area to margins
    static bool CenterTitle; // Center Title
    static int TitleMag; // Title size magnification
    static int DefaultMarginX,DefaultMarginY; // Frames margins
    static int DefaultBorder; // Frames border width
    static TColor DefaultColor; // Frame background

    // Managing
    static typ_ActionIdxs ActionsIdxs; // Used to build popup menu

private://                                                      ~~~~~~~~~~~~~

// . . . Private attributes

    // Space transformation
    double Kx,Ky; // Pixels per unit or decade
    int Xo,Yo; // Axes origin pixels coordinates measure respect (left,bottom)

    // Mapped space: set these values with set methods
    double i_yrange,i_xrange,i_ymin,i_xmin;

    // Auxiliary
    int w,h; // to detect dimensions modification
    int Xfltprc,Xfltdgt,Yfltprc,Yfltdgt; // Number string representation, dependent on current scale

// . . . Properties
    // Main
    String i_Title;
    String get_Name() {return String("Frame") + String(Tag);}
    // State
    typ_TracksIt i_CurrentTrackIt;
    class cls_Track* get_CurrentTrack() const {return (static_cast<typ_TracksConstIt>(CurrentTrackIt)!=tracks.end() ? *CurrentTrackIt : 0);}
    void __fastcall set_CurrentTrack(class cls_Track* t);
    mat::cls_Color i_TrackCurrentColor;
    // Scales
    TPoint get_O() const {return TPoint(left+Xo,bottom-Yo);}
    int i_SampPerPix;
    double get_PixelSizeX() const {if (i_XLogarithmic) return 1; else return (Kx)?(1/Kx):mat::Huge;}
    double get_PixelSizeY() const {if (i_YLogarithmic) return 1; else return (Ky)?(1/Ky):mat::Huge;}
    double get_xinf() const {return Xphy(Left);}
    double get_xsup() const {return Xphy(Right);}
    double get_xmax() const {return i_xmin+i_xrange;}
    double get_ymax() const {return i_ymin+i_yrange;}
    bool i_XLogarithmic, i_YLogarithmic;
    void set_YLogarithmic (const bool);
    void set_XLogarithmic (const bool);
    // Tracks collection
    String i_LongestTrackName;
    // Auxiliary
    int i_Row,i_Tag;

// . . . Private methods

    // Tracks collection
    void MoveTrack(typ_TracksIt, typ_TracksIt); // Move a Track before a destination
    void MoveTrack (typ_TracksIt, cls_Frame*); // Trasfer track to another frame
    void ShiftTracksFw(); // Shift forward all tracks of one position
    void ShiftTracksBw(); // Shift backward all tracks of one position
    // Scales
    inline void SetMappedX (const double, const double); // Set mapped x space
    inline void SetMappedY (const double, const double); // Set mapped y space
    inline void SetTransformX (const double, const int); // Set x-axis transformation coeffs
    inline void SetTransformY (const double, const int); // Set y-axis transformation coeffs
    inline void RecalculateMappedX(); // Recalculate x-axis mapped space
    inline void RecalculateMappedY(); // Recalculate y-axis mapped space
    inline void RecalculateTransformX(); // Recalculate x-axis transformation coeffs
    inline void RecalculateTransformY(); // Recalculate y-axis transformation coeffs
    inline void SetXvaluesRepresentation()
            { // Float precision and digits in string rep
             //Xfltdgt = 0; // Not used in ffGeneral
             // TODO 3: devo calcolarmi il numero di cifre decimali che mi servono
			 if ( std::fabs(PixelSizeX) >= mat::eps ) Xfltprc = std::ceil(std::fabs(std::log10(std::fabs(PixelSizeX))));
             else Xfltprc = 15;
			 if ( std::fabs(xmax) > 1 ) Xfltprc += std::floor(std::log10(std::fabs(xmax)));
			 //Xfltprc = ::ceil( 1 + std::log10(std::fabs(xmax)) - std::log10(std::fabs(PixelSizeX)) );
			 //Xfltprc = ::ceil( 1 + std::log10(std::fabs(PixelSizeX)) );
            }
    inline void SetYvaluesRepresentation()
            { // Float precision and digits in string rep
             //Yfltdgt = 0; // Not used in ffGeneral
			 if ( std::fabs(PixelSizeY) >= mat::eps ) Yfltprc = std::ceil(std::fabs(std::log10(std::fabs(PixelSizeY))));
             else Yfltprc = 15;
			 if ( std::fabs(ymax) > 1 ) Yfltprc += std::floor(std::log10(std::fabs(ymax)));
			 //Yfltprc = ::ceil( 1 + std::log10(std::fabs(ymax)) - std::log10(std::fabs(PixelSizeY)) );
			 //Yfltprc = ::ceil( -1 - std::log10(std::fabs(PixelSizeY)) );
            }
    // Low level
    inline double CalculateLinearScale (const double, const int);
    inline int CalculateLinearOrigin (const int, const double, const double);
    inline double CalculateLogarithmicScale (const double, const int);
    inline int CalculateLogarithmicOrigin (const int, const double, const double);

};//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//---------------------------------------------------------------------------
#endif
