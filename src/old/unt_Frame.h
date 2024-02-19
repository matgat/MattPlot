//---------------------------------------------------------------------------

#ifndef unt_FrameH
#define unt_FrameH


        /* * * * * * * * * * * * * * * * * * * * * * * * * * * *
        *
        *                cls_Frame (nov.2006)
        *        2006 - Matteo Gattanini
        *
        *        unt_Frame
        *                ver: 04    (26.nov.2006)
        *
        *        This unit defines a class representing a frame
        *        of a `cls_MatPlot' class (data plotting canvas)
        *
        * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*-------------------------- Geometric attributes ---------------------------
    -----> x
    ____________________________CANVAS________________________
 | |                    Parent->Border                        |
 | |  o----------------------------------------------------o  |
 | |  | Left,Top            Margin               Right,Top |  |
 | |  | o- - - - - - - - - - - - - - - - - - - - - - - - o |  |
 V |  |M| left,top                             right,top |M|  |
 y |B |a|      .   .                             ...     |a| B|
   |o |r|     .  .  .     plot area     ..    ...        |r| o|
   |r |g|    .      .  ..            ..    . .           |g| r|
   |d |i|    .       .   .          .      ..            |i| d|
   |e |n|    .            .       .                      |n| e|
   |r | |   .              ......                        | | r|
   |  | o- - - - - - - - - - - - - - - - - - - - - - - - o |  |
   |  |  left,bottom        Margin           right,bottom  |  |
   |  o----------------------------------------------------o  |
   |   Left,Bottom      Parent->Border         Right,Bottom   |
    ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
   Linear scale              Logarithmic scale
   X = Kx x + Xo             X = Kx Log(x) + Xo
   Y = Ky y + Yo             Y = Ky Log(y) + Yo
   Kx,Ky = pixels per unit   Kx,Ky = pixels per decade
   (Xo,Yo) <=>  (0,0)        (Xo,Yo) <=> (1,1)
*/

#include <set> // for Tracks pointers
 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Class Definition ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class cls_Frame {

public://                                                        ~~~~~~~~~~~~~

// ~ ~ ~ Types definitions
        typedef std::set<class cls_Track*> typ_Tracks;
        typedef typ_Tracks::iterator typ_TracksIt;

// . . . Public methods
        cls_Frame(class cls_MatPlot*,int =10,TColor =clWhite);
        //~cls_Frame();

        void __fastcall Plot (TCanvas*);

        // Scale services
        void AdaptScale (const cls_Track*); // Adapt scale to visualize track space occupation
        void AdaptScale() { if(SelectedTrack) AdaptScale(SelectedTrack); else AdaptScale(0); }
        void ResetOrigin(); // Adapt space to view axes
        void Pan(const int&, const int&); // Modify mapped space offset given a pixel displacement
        void Zoom (const Windows::TRect&); // Modify space-transformation to map a new space

        // Space trasformation
        inline double Yphy (const int Y) const
                { // Anti-transform y canvas axis to physical space
                 if ( YLogarithmic ) return ::pow10(((bottom-Yo)-Y)/Ky); // y = 10^(- ( Yc - (bottom-Yor) ) / Ky)
                 else return ((bottom-Yo)-Y)/Ky; // y = - ( Yc - (bottom-Yor) ) / Ky
                }
        inline int Ypix (const double y) const
                { // Transform  y-axis value to canvas space
                 if ( YLogarithmic ) return -mat::round(Ky*::log10(y)) + (bottom-Yo); // Yc = -Ky Log(y) +(bottom-Yor)
                 else return -mat::round(Ky*y) + (bottom-Yo); // Yc = -Ky (y) +(bottom-Yor)
                }
        inline double Xphy (const int X) const
                { // Anti-transform  x canvas axis to physical space
                 if ( XLogarithmic ) return ::pow10((X-left-Xo)/Kx); // x = 10^(( Xc - (left+Xor) ) / Kx)
                 else return (X-left-Xo)/Kx; // x = ( Xc - (left+Xor) ) / Kx
                }
        inline int Xpix (const double x) const
                { // Transform  y-axis value to canvas space
                 if ( XLogarithmic ) return mat::round(Kx*::log10(x)) + left+Xo; // Xc = Kx Log(x) + X0r+left
                 else return mat::round(Kx*x) + left+Xo; // Xc = Kx x + X0r+left
                }

        // Contained Tracks services
        void InsertTrack (class cls_Track*); // Add a track reference to be plotted
        void InsertTracks (class cls_Frame*); // Add track pointers of another frame
        void AddToOwnedTracks (class cls_Frame*); // Add another owner to owned tracks
        void RemoveTrack (class cls_Track*); // Remove track from plotted tracks
        bool Contains (class cls_Track*); // Tell if contains a track
        void RemoveTracks (); // Remove all tracks
        class cls_Track* OverTrack (const TPoint&); // Clicked track

        // Drawing services (see also in private methods)
        inline void DrawVLine (class TCanvas* Canvas, const int& X) const {Canvas->MoveTo(X,bottom);Canvas->LineTo(X,top);}
        inline void DrawHLine (class TCanvas* Canvas, const int& Y) const {Canvas->MoveTo(left,Y);Canvas->LineTo(right,Y);}

// . . . Operators

// . . . Properties
        __property class cls_Track* SelectedTrack = {read=i_SelectedTrack, write=i_SelectedTrack};
        __property double PixelSizeX = {read=get_PixelSizeX}; // Pixel horizontal size
        __property double PixelSizeY = {read=get_PixelSizeY}; // Pixel vertical size
        // Options
        __property int Margin = {read=i_Margin, write=set_Margin}; // Plot area margin
        __property bool DrawGrid = {read=i_DrawGrid, write=i_DrawGrid}; // Draw the grid
        __property bool DrawLegend = {read=i_DrawLegend, write=i_DrawLegend}; // Draw the legend
        __property uint VGridDist = {read=i_VGridDist, write=set_VGridDist}; // Vertical grid pixel distance (wished)
        __property uint HGridDist = {read=i_HGridDist, write=set_HGridDist}; // Horizontal grid pixel distance (wished)
        __property bool YLogarithmic = {read=i_YLogarithmic, write=set_YLogarithmic}; // Ordinate logarithmic scale
        __property bool XLogarithmic = {read=i_XLogarithmic, write=set_XLogarithmic}; // Abscissa logarithmic scale
        // Colors
        __property TColor BackgroundColor = {read=i_BackgroundColor, write=i_BackgroundColor};
        __property TColor BorderColor = {read=i_BorderColor, write=i_BorderColor};
        __property TColor AxesColor = {read=i_AxesColor, write=i_AxesColor};
        __property TColor GridColor = {read=i_GridColor, write=i_GridColor};
        __property TColor LabelColor = {read=i_LabelColor, write=i_LabelColor};
        // Inner dimensions
        __property int top = {read=i_top, write=i_top};
        __property int left = {read=i_left, write=i_left};
        __property int bottom = {read=get_bottom, write=set_bottom};
        __property int right = {read=get_right, write=set_right};
        __property int width = {read=i_width, write=i_width};
        __property int height = {read=i_height, write=i_height};
        __property TPoint tl = {read=get_tl}; // TopLeft point
        __property TPoint br = {read=get_br}; // BottomRight point
        __property TRect boundsRect = { read=get_boundsRect, write=set_boundsRect};
        // Outer dimensions
        __property int Top = {read=get_Top, write=set_Top};
        __property int Left = {read=get_Left, write=set_Left};
        __property int Bottom = {read=get_Bottom, write=set_Bottom};
        __property int Right = {read=get_Right, write=set_Right};
        __property int Width = {read=get_Width, write=set_Width};
        __property int Height = {read=get_Height, write=set_Height};
        __property TPoint TL = {read=get_TL}; // TopLeft point
        __property TPoint BR = {read=get_BR}; // BottomRight point
        __property TPoint C = {read=get_C}; // Center point
        __property TRect BoundsRect = { read=get_BoundsRect, write=set_BoundsRect};
        // Auxiliary
        __property int Row = {read=i_Row, write=i_Row}; // Layout facility
        __property int Tag = {read=i_Tag, write=i_Tag}; // Layout facility

// . . . Public attributes (!)
        static const String Name;
        // Default mapped space
        static const double xmin_def,xrange_def,ymin_def,yrange_def;

protected://                                                        ~~~~~~~~~~~~~

// . . . Protected attributes

// . . . Protected methods
        // . . . Properties

private://                                                        ~~~~~~~~~~~~~

// . . . Private attributes
        class cls_MatPlot* Parent;
        typ_Tracks Tracks; typ_TracksIt tr; // Tracks

        // Mapped space: set these values with set methods
        double yrange,xrange,ymin,xmin;
        // Space transformation
        double Kx,Ky; // Pixels per unit or decade
        int Xo,Yo; // Axes origin pixels coordinate

        // Static members
        static int Number;
        static int ArrowLength,ArrowWing;

        // Auxiliary

// . . . Properties
        class cls_Track* i_SelectedTrack;
        double get_PixelSizeX() const {return (Kx)?(1/Kx):mat::huge;}
        double get_PixelSizeY() const {return (Ky)?(1/Ky):mat::huge;}
        // Options
        int i_Margin;
        void set_Margin (const int);
        bool i_DrawGrid,i_DrawLegend;
        uint i_VGridDist,i_HGridDist;
        void set_VGridDist (uint);
        void set_HGridDist (uint);
        bool i_XLogarithmic,i_YLogarithmic;
        void set_YLogarithmic (bool);
        void set_XLogarithmic (bool);
        // Colors
        TColor i_BackgroundColor,
               i_BorderColor,
               i_AxesColor,
               i_GridColor,
               i_LabelColor;
        // Dimensions
        int i_top,i_left,i_width,i_height;
        int get_bottom() const {return top+height;}
        void set_bottom(int b) {height=b-top;}
        int get_right() const {return left+width;}
        void set_right(int r) {width=r-left;}
        TPoint get_tl() const {return TPoint(left,top);}
        TPoint get_br() const {return TPoint(right,bottom);}
        TRect get_boundsRect() const {return TRect(left,top,right,bottom);}
        void set_boundsRect(TRect r) {top=r.top;left=r.left;right=r.right;bottom=r.bottom;}
        int get_Width() const {return width+2*Margin;}
        void set_Width(int w) {width=w-2*Margin;}
        int get_Height() const {return height+2*Margin;}
        void set_Height(int h) {height=h-2*Margin;}
        int get_Top() const {return top-Margin;}
        void set_Top(int t) {top=t+Margin;}
        int get_Left() const {return left-Margin;}
        void set_Left(int l) {left=l+Margin;}
        int get_Bottom() const {return Top+Height;}
        void set_Bottom(int b) {Height=b-Top;}
        int get_Right() const {return Left+Width;}
        void set_Right(int r) {Width=r-Left;}
        TPoint get_TL() const {return TPoint(Left,Top);}
        TPoint get_BR() const {return TPoint(Left+Width,Top+Height);}
        TPoint get_C() const {return TPoint(left+width/2,top+height/2);}
        TRect get_BoundsRect() const {return TRect(Left,Top,Left+Width,Top+Height);}
        void set_BoundsRect(TRect r) {Top=r.top;Left=r.left;Width=r.right-r.left;Height=r.bottom-r.top;}
        // Auxiliary
        int i_Row,i_Tag;

// . . . Private methods
        inline void DrawTheAxes (TCanvas*);
        inline void DrawTheGrid (TCanvas*);
        inline void DrawTheLegend (TCanvas*);
        // Scales
        inline void SetMappedX (const double&, const double&); // Set mapped x space
        inline void SetMappedY (const double&, const double&); // Set mapped y space
        inline void SetTransformX (const double&, const int&); // Set x-axis transformation coeffs
        inline void SetTransformY (const double&, const int&); // Set y-axis transformation coeffs
        inline void RecalculateMappedX(); // Recalculate x-axis mapped space
        inline void RecalculateMappedY(); // Recalculate y-axis mapped space
        inline void RecalculateTransformX(); // Recalculate x-axis transformation coeffs
        inline void RecalculateTransformY(); // Recalculate y-axis transformation coeffs
        // Low level
        inline double CalculateLinearScale (double, const int);
        inline int CalculateLinearOrigin (const double, const int, const double);
        inline double CalculateLogarithmicScale (double, const int);
        inline int CalculateLogarithmicOrigin (const double, const int, const double);

};//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//---------------------------------------------------------------------------
#endif
