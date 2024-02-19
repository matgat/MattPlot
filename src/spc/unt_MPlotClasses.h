//---------------------------------------------------------------------------

#ifndef unt_MPlotClassesH
#define unt_MPlotClassesH

        /* * * * * * * * * * * * * * * * * * * * * * * * * * * *
        *
        *        unt_MPlotClasses  - Matteo Gattanini
        *                ver: 1.0    (dec.2007)
        *
        *   cls_Foil, cls_VolatileDrawer
        *
        *   cls_SizedObject        cls_FoilObject
        *        |         \       /         \
        *        |         cls_Frame     cls_FrameObject
        *        |                        /      \
        *        |                      /         \
        *        |                    /            | cls_Track
        *      cls_FrameDraggableObject            | cls_Axes
        *                \                         | cls_Grid
        *                cls_Legend                | cls_Marker / cls_Markers
        *                                          | cls_Label  / cls_Labels
        *
        *        This unit defines a class tree related
        *        to `cls_MPlot' data plotting class
        *
        * * * * * * * * * * * * * * * * * * * * * * * * * * * */

//----------------------------- External files ------------------------------
//#include <StdCtrls.hpp> // for TLabel
#include <Graphics.hpp> // for style attributes
#include "unt_MatCommon.h"


//:::::::::::::::::::::::::::: Namespace content ::::::::::::::::::::::::::::
namespace nms_MPlotClasses
{
 //---------------------------- Types definition
    /*
    struct stu_Magnitude { double Value; char Label[4]; };
    const struct cls_Quantity::stu_Magnitude cls_Quantity::UnderValueOrder[] =
            {
             { 1     , "unità"},
             { 1E-1  , "decimi"},
             { 1E-2  , "centesimi"},
             { 1E-11 , "centimiliardesimi"},
             { 1E-12 , "inferiore ai centimiliardesimi"},
             { 0     , ""   } // terminator
            };
     //static const struct stu_Magnitude UnderValueOrder[];
    */

    // TODO 4: use more complex class for:
 
    //.........................................................
    // Point quantities
    typedef struct { double y, x;   // coordinates
                     double dy, dx; // increments to next point
                     double dydx;   // derivate
                    } stu_PointQuantities;

    typedef enum { PQ_y, PQ_x, PQ_dy, PQ_dx, PQ_dydx } en_PointQuantities;
    typedef union { stu_PointQuantities Names;
                    double Array[5];
                   } uni_PointQuantities;

    //.........................................................
    // Interval quantities
    typedef struct { double Dt, tmin, tmax; // independent variable
                     double Dy, DY; // ordinate increment and overall sweep
                     double n; // number of samples
                     double ymax, ymin; // ordinate max and min
                     double DyDt, Sy, avg, rms, std, stdp, varp; // incremental ratio, partial sum, average, root mean square, standard deviation
                     #ifdef MAD
                     double mad; // Mean Absolute Deviation
                     #endif
                     } stu_IntervalQuantities;
    typedef enum { IQ_Dt, IQ_tmin, IQ_tmax,
                   IQ_Dy, IQ_Dymax,
                   IQ_n,
                   IQ_ymax, IQ_ymin,
                   IQ_Sy, IQ_avg, IQ_rms, IQ_std,
                   #ifdef MAD
                   IQ_mad
                   #endif
                   } en_IntervalQuantities;
    typedef union { stu_IntervalQuantities Names;
                    double Array[13];
                   } uni_IntervalQuantities;

        //.........................................................
    // Foil object types
    typedef enum { OB_NONE,
                   OB_FRAME,
                   OB_TRACK,
                   OB_LEGEND,
                   OB_GRID,
                   OB_AXES,
                   OB_MARKER,
                   OB_LABEL } en_FoilObjectType;

    /*
    typedef struct { en_FoilObjectType Type;
                     uni_FoilObjectPtr Ptr;
                    } stu_FoilObject;
    */

}//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
namespace mpl = nms_MPlotClasses; // a short alias for this namespace


////////////////////////////////// cls_Foil /////////////////////////////////
//---------------------------------------------------------------------------
class cls_Foil { // A paper where paint

public://                                                       ~~~~~~~~~~~~~
// . . .  Types definitions

// . . . Public methods
	cls_Foil (TCanvas* c, const TRect& r, double ppp, bool rf =false)
            : i_Canvas(c),i_BoundsRect(r),i_PointsPerPix(ppp),i_RescaleFont(rf) {;}
    ~cls_Foil ()  {;}

// . . . Properties
    __property class TCanvas* Canvas = {read=i_Canvas};
	__property TRect BoundsRect = {read=i_BoundsRect, write=i_BoundsRect};
    __property int Top = {read=get_Top};
    __property int Left = {read=get_Left};
    __property int Width = {read=get_Width}; //, write=set_Width};
    __property int Height = {read=get_Height}; //, write=set_Height};
    __property double PointsPerPix = {read=i_PointsPerPix, write=i_PointsPerPix};
    __property bool RescaleFont = {read=i_RescaleFont, write=i_RescaleFont};

// . . . Public attributes (!)

private://                                                      ~~~~~~~~~~~~~

// . . . Private attributes

// . . . Properties
    class TCanvas*const i_Canvas;
	TRect i_BoundsRect;
    double i_PointsPerPix;
    bool i_RescaleFont;
    int get_Top () const {return i_BoundsRect.Top;}
    int get_Left () const {return i_BoundsRect.Left;}
    int get_Width () const {return i_BoundsRect.Width();}
    int get_Height () const {return i_BoundsRect.Height();}
    //void set_Width (int w) {w=(Width-w)/2; i_BoundsRect.Left+=w; i_BoundsRect.Right-=w;}
    //void set_Height(int h) {h=(Height-h)/2; i_BoundsRect.Top+=h; i_BoundsRect.Bottom-=h;}

// . . . Private methods
}; // end 'cls_Foil'



////////////////////////////// cls_VolatileText /////////////////////////////
//---------------------------------------------------------------------------
class cls_VolatileText { // A text rendered in Canvas with 'pmNotXor'

public://                                                       ~~~~~~~~~~~~~
// . . .  Types definitions
    enum en_HAlgn { AL_LEFT, AL_CENTER, AL_RIGHT };
    enum en_VAlgn { AL_TOP, AL_MIDDLE, AL_BOTTOM };
    typedef struct {int X, Y; en_HAlgn HAlgn; en_VAlgn VAlgn;} stu_Where;

// . . . Public methods
    cls_VolatileText ();
    //~cls_VolatileText() { delete i_Font; } TPoint

    inline void Render (class TCanvas*, const TColor); // Render in Canvas given background color
    inline void SetPlace (const int, const int, const en_VAlgn, const en_HAlgn); // Set place 1
    inline void Reset() {SetPlace(-1,-1,AL_TOP,AL_LEFT); i_Text="";} // Reset state

// . . . Properties
    __property String Text = {read=i_Text, write=i_Text};
    __property int Offset = {read=i_Offset, write=i_Offset}; // An offset respect place point

// . . . Public attributes (!)
    static TColor AlternateBkgColor;

private://                                                      ~~~~~~~~~~~~~

// . . . Private attributes
    stu_Where i_Place; // Place where to put text

// . . . Properties
    String i_Text;
    int i_Offset;

// . . . Private methods
}; // end 'cls_VolatileText'



///////////////////////////// cls_VolatileDrawer ////////////////////////////
//---------------------------------------------------------------------------
class cls_VolatileDrawer { // To draw temporary lines and marks on plot

public://                                                       ~~~~~~~~~~~~~
// . . .  Types definitions

// . . . Public methods
    cls_VolatileDrawer ( class cls_MPlot* );
    ~cls_VolatileDrawer ();

    // Dropping zone
	void MarkZone(const TRect&); // Fill the dropping zone
    void UnMarkZone(); // Unmark last marked zone
    // Continuous Pan direction
    void DrawDirection(const TPoint&, const TPoint&); // Draw direction during enhanced pan
    void EraseDirection(); // Erase last direction
	// Selection rectangle
	void DrawSelRect(const TRect&); // Draw zoom rectangle
    void EraseSelRect(); // Erase last zoom rectangle
    // Measure lines
    void DrawMeasureLines1 (bool persistent =false); // Draw 1st measure lines for a track
    void DrawMeasureLines2 (); // Draw 2nd measure lines for a track
    void EraseMeasureLines1(); // Erase 1st measure lines
    void EraseMeasureLines2(); // Erase 2nd measure lines
    // Pen services
    void SetPencil(); // Set Canvas for pencil stroke
    void SetHighlighter(); // Set Canvas for highlighter stroke
    void Stroke (const TPoint&, const TPoint&); // Draw with selected tool
    // Reset services
    void ResetZone() { ZoneRect=TRect(-1,-1,-1,-1); }
    void ResetDirection() { P_dir1=P_dir2=TPoint(-1,-1); }
    void ResetSelRect() { SelRect=TRect(-1,-1,-1,-1); }
    void ResetMeasure(TPoint& P, TPoint& S) { P=S=TPoint(-1,-1);
                                              OrdinateText->Reset();
                                              AbscissaText->Reset();
                                              SlopeText->Reset(); }
    void ResetMeasure() { ResetMeasure(mP1,mS1); ResetMeasure(mP2,mS2);}
    void Reset() { ResetZone(); ResetDirection(); ResetSelRect(); ResetMeasure(); } // Reset all
    // Other facilities
    inline bool IsDrawingInterval() const {return mP1.x>=0;}
    inline bool IsntDrawingInterval() const {return mP1.x<0;}

// . . . Properties
    // General
    __property class cls_MPlot* Owner = {read=i_Owner}; // Get owner
    __property class TCanvas* Canvas = {read=get_Canvas};
    // Marking zone service (dragging frames)
    __property TBrushStyle MarkZoneStyle = { read=i_MarkZoneStyle, write=i_MarkZoneStyle };
    __property TColor MarkColor = {read=i_MarkColor, write=i_MarkColor};
    // Selecting rectangle service (zoom and crop)
    __property TPenStyle SelRectLineStyle = { read=i_SelRectLineStyle, write=i_SelRectLineStyle };
    __property TColor SelRectLineColor = {read=i_SelRectLineColor, write=i_SelRectLineColor};
    __property TColor SelRectAreaColor = {read=i_SelRectAreaColor, write=i_SelRectAreaColor};
    __property bool IsSelRect = {read=get_IsSelRect};
    // Lines service (measuring lines)
    __property TColor MeasureLinesColor = {write=set_MeasureLinesColor};
    __property TPenStyle MeasureLinesStyle = { read=i_MeasureLinesStyle, write=i_MeasureLinesStyle };
    __property bool ViewVerticalLine = {read=i_ViewVerticalLine, write=i_ViewVerticalLine};
    __property bool ViewHorizontalLine = {read=i_ViewHorizontalLine, write=i_ViewHorizontalLine};
    __property bool ViewSlopeLine = {read=i_ViewSlopeLine, write=i_ViewSlopeLine};
    __property TColor HighlightInvBkgColor = {read=i_HighlightInvBkgColor, write=i_HighlightInvBkgColor};
    // Scribbling service (pen mode)
    __property TColor PencilColor = {read=i_PencilColor, write=i_PencilColor};
    __property int PencilSize = {read=i_PencilSize, write=i_PencilSize};
    __property TColor HighlighterColor = {read=i_HighlighterColor, write=i_HighlighterColor};
    __property int HighlighterSize = {read=get_HighlighterSize, write=set_HighlighterSize};

// . . . Public attributes (!)
    TRect ZoneRect; // Zone marking rectangle
    TRect SelRect; // Selection rectangle
    TPoint mP1,mP2,mS1,mS2; // Measure points and slope points

private://                                                      ~~~~~~~~~~~~~

// . . . Private attributes
    // General
    unsigned int CurrentRepaintTime;
    // Pan direction service
    TPoint P_dir1,P_dir2; // Direction extremes
    // Measure lines service
    class cls_VolatileText *AbscissaText, *AbscissaTextGrid;
    class cls_VolatileText *OrdinateText, *OrdinateTextGrid;
    class cls_VolatileText *SlopeText;
    double x1,y1,dydx1; // TODO: deprecated, use interval quantities
    short int Desaturation; // To distinguish measure lines
    // Auxiliary
    mutable class cls_Frame* CurrFrame;

// . . . Properties
    // General
    class cls_MPlot*const i_Owner;
    inline class TCanvas* get_Canvas ();
    // Marking zone service (dragging frames)
    TBrushStyle i_MarkZoneStyle;
    TColor i_MarkColor;
    // Selecting rectangle service (zoom and crop)
    TPenStyle i_SelRectLineStyle;
    TColor i_SelRectLineColor,i_SelRectAreaColor;
    bool get_IsSelRect() {return SelRect.bottom>0;}
    // Lines service (measuring lines)
    TColor i_MeasureLinesColor1, i_MeasureLinesColor2;
    void set_MeasureLinesColor(const TColor);
    TPenStyle i_MeasureLinesStyle;
    bool i_ViewVerticalLine, i_ViewHorizontalLine, i_ViewSlopeLine;
    TColor i_HighlightInvBkgColor;
    // Scribbling service (pen mode)
    TColor i_PencilColor, i_HighlighterColor;
    int i_PencilSize, i_HighlighterRadius;
    int get_HighlighterSize() const {return 2*i_HighlighterRadius;}
    void set_HighlighterSize (int s) {i_HighlighterRadius = s/2;}

// . . . Private methods
    inline void RenderMeasureLines (TPoint& P, TPoint& S); // Draw measure lines for a track
    inline void EraseMeasureLines(TPoint& P, TPoint& S); // Erase measure lines
    inline void RetrieveMeasuresPoints (TPoint& P ,TPoint& S); // Get screen ordinate and slope
}; // end 'cls_VolatileDrawer'




/////////////////////////////// cls_SizedObject /////////////////////////////
//---------------------------------------------------------------------------
class cls_SizedObject { // Object with dimensions anchored to its Top-Left point

public://                                                       ~~~~~~~~~~~~~
// . . .  Types definitions

// . . . Public methods
    cls_SizedObject() {i_Top=i_Left=i_Width=i_Height=i_PixMarginX=i_PixMarginY=i_MarginX=i_MarginY=0;}
    ~cls_SizedObject ()  {;}

    void RecalculateMargins(const double r)
         {
           i_MarginX = r*double(i_PixMarginX);
           if (width <= 0) i_MarginX = (r*Width)/6;
           i_MarginY = r*double(i_PixMarginY);
           if (height <= 0) i_MarginY = (r*Height)/6;
         }

    virtual bool __fastcall IsOver(const TPoint& P) const {return mat::IsInRect(P,BoundsRect);}
    virtual bool IsOverX(const int X) const {return (X>=Left && X<=Right);}
    virtual bool IsOverY(const int Y) const {return (Y>=Top && Y<=Bottom);}

// . . . Properties
    // Absolute margins
    __property int PixMargin = {read=i_MarginX, write=set_PixMargin}; // Spacing from border
    __property int PixMarginX = {read=i_PixMarginX, write=i_PixMarginX}; // Horizontal spacing
    __property int PixMarginY = {read=i_PixMarginY, write=i_PixMarginY}; // Vertical spacing
    // Rescaled margins
    __property int Margin = {read=get_Margin}; // Spacing from border
    __property int MarginX = {read=i_MarginX}; // Horizontal spacing
    __property int MarginY = {read=i_MarginY}; // Vertical spacing
    // Other
    __property TPoint C = {read=get_C}; // Center point
    // Outer dimensions
    __property int Top = {read=get_Top, write=set_Top};
    __property int Left = {read=get_Left, write=set_Left};
    __property int Bottom = {read=get_Bottom, write=set_Bottom};
    __property int Right = {read=get_Right, write=set_Right};
    __property int Width = {read=i_Width, write=i_Width};
    __property int Height = {read=i_Height, write=i_Height};
    __property TPoint TL = {read=get_TL}; // TopLeft point
    __property TPoint BR = {read=get_BR}; // BottomRight point
    __property TPoint CR = {read=get_CR}; // CenterRight point
    __property TRect BoundsRect = { read=get_BoundsRect, write=set_BoundsRect};
    // Inner dimensions
    __property int top = {read=get_top}; // , write=set_top
    __property int left = {read=get_left};  // , write=set_left
    __property int bottom = {read=get_bottom}; // , write=set_bottom
    __property int right = {read=get_right}; // , write=set_right
    __property int width = {read=get_width}; // , write=set_width
    __property int height = {read=get_height}; // , write=set_height
    __property TPoint tl = {read=get_tl}; // TopLeft point
    __property TPoint br = {read=get_br}; // BottomRight point
    __property TRect boundsRect = { read=get_boundsRect}; // , write=set_boundsRect

// . . . Public attributes (!)

private://                                                      ~~~~~~~~~~~~~

// . . . Private attributes

// . . . Properties
    int i_Top,i_Left,i_Width,i_Height; // Outer dimensions
    int i_MarginX,i_MarginY; // Margins (recalculated)
    int i_PixMarginX,i_PixMarginY; // Margins in pix (fixed)
    //Margins
    void set_PixMargin (const int m) {i_PixMarginX=i_PixMarginY=m;}
    int get_Margin() const {return (i_MarginX<i_MarginY)?i_MarginX:i_MarginY;}
    // Other
    TPoint get_C() const {return TPoint(Left+i_Width/2,Top+i_Height/2);} // Center point
    // Outer dimensions
    virtual int get_Top() const {return i_Top;} // Virtual for draggable objs
    virtual void set_Top(int t) {i_Top=t;}
    virtual int get_Left() const {return i_Left;}
    virtual void set_Left(int l) {i_Left=l;}
    int get_Bottom() const {return Top+i_Height;}
    void set_Bottom(int b) {i_Height=b-Top;}
    int get_Right() const {return Left+i_Width;}
    void set_Right(int r) {i_Width=r-Left;}
    TPoint get_TL() const {return TPoint(Left,Top);} // Top left point
    TPoint get_BR() const {return TPoint(Right,Bottom);}  // Bottom right point
    TPoint get_CR() const {return TPoint(Left+i_Width,Top+i_Height/2);} // Center right point
    TRect get_BoundsRect() const {return TRect(Left,Top,Right,Bottom);}
    void set_BoundsRect(TRect r) {Left=r.left;Top=r.top;Right=r.right;Bottom=r.bottom;}
    // Inner dimensions
    int get_top() const {return Top+i_MarginY;}
    //void set_top(int t) {MarginY=t-Top;}
    int get_left() const {return Left+i_MarginX;}
    //void set_left(int l) {MarginX=l-Left;}
    int get_bottom() const {return Bottom-i_MarginY;}
    //void set_bottom(int b) {MarginY=Bottom-b;}
    int get_right() const {return Right-i_MarginX;}
    //void set_right(int r) {MarginX=Right-r;}
    int get_width() const {return right-left;}
    //void set_width(int w) {MarginX=(Width-w)/2;}
    int get_height() const {return bottom-top;}
    //void set_height(int h) {MarginY=(Height-h)/2;}
    TPoint get_tl() const {return TPoint(left,top);}
    TPoint get_br() const {return TPoint(right,bottom);}
    TRect get_boundsRect() const {return TRect(left,top,right,bottom);}
    //void set_boundsRect(TRect r) {top=r.top;left=r.left;right=r.right;bottom=r.bottom;}

// . . . Private methods
}; // end 'cls_SizedObject'




////////////////////////////// cls_FoilObject /////////////////////////////
//---------------------------------------------------------------------------
class cls_FoilObject { // Something drawed in canvas

public://                                                       ~~~~~~~~~~~~~
// . . .  Types definitions
    typedef std::vector<int> typ_ActionIdxs;

// . . . Public methods
    cls_FoilObject();
    cls_FoilObject(const cls_FoilObject&); // Copy constructor
    ~cls_FoilObject(); // TODO: put virtual here?

    cls_FoilObject& operator=(const cls_FoilObject& fo) {AssignStyle(&fo); return *this;} // Copy assignment

    // . . . Managing services
    void AssignFont(class TFont* f) {i_Font->Assign(f); } // Assign font
    bool IsDifferentFrom(const class cls_FoilObject*); // A comparison function
    virtual class cls_PropertiesInterface* CreateSpecInterface(class TWinControl*); // Create the edit properties interface
    class cls_PropertiesInterface* CreateStyleInterface(class TWinControl*); // Only FoilObject properties
    virtual nms_MPlotClasses::en_FoilObjectType GetType(); // Get a enum type (deprecated)
    virtual typ_ActionIdxs* GetActionsIdxs() const {return &ActionsIdxs;} // Used to build popup menu

    // . . . Graphic services
    //-----------------------------------------------------------------------
    inline void SetPaintTools(class cls_Foil* f) // Set Pen, Brush and Font
       {
        Canvas = f->Canvas; // Memorize Canvas

        // . . . Here we can rescale sizes according to Foil
        Canvas->Pen->Width = mat::round(f->PointsPerPix * double(LineWeight));
        // Remark: Font is automatically rescaled according to 'PixelsPerInch'
        if (f->RescaleFont) Font->PixelsPerInch = double(Screen->PixelsPerInch) / f->PointsPerPix;
        else Font->PixelsPerInch = Screen->PixelsPerInch;

        // . . . Set paint tools
        Canvas->Font->Assign(Font);
        Canvas->Pen->Color = LineColor;
        Canvas->Brush->Color = AreaColor;
        Canvas->Pen->Mode = ColorMode;
        Canvas->Pen->Style = LineStyle;
        Canvas->Brush->Style = AreaStyle;

        // . . . Specific tasks
        //cls_SizedObject* sz = dynamic_cast<cls_SizedObject*>(this);
        //if (sz) sz->Margin = double(sz->Margin) * f->PointsPerPix;
       }
    inline void Paint(class cls_Foil* f) {if(Visible){SetPaintTools(f);Draw(f);}} // Set paint tools and draw
    virtual void __fastcall Draw(class cls_Foil*) =0; // Render on parent's Canvas (use 'Canvas')
    virtual bool __fastcall IsOver(const TPoint&)  const =0; // Tell if a Canvas point is over
    virtual void __fastcall Reset() =0; // Reset operations, called on toggling 'Visible'
    virtual void __fastcall Remove() =0; // Remove request

// . . . Properties
    //__property class cls_Foil* Foil = {read=i_Foil}; // Get foil
    __property bool Visible = {read=get_Visible, write=set_Visible}; // Draw or not
    // Graphic style
    __property TPenMode ColorMode = {read=i_ColorMode, write=i_ColorMode};
    __property int LineWeight = {read=i_LineWeight, write=i_LineWeight};
    __property TColor LineColor = {read=i_LineColor, write=i_LineColor};
    __property TPenStyle LineStyle = { read=i_LineStyle, write=i_LineStyle };
    __property TColor AreaColor = { read=i_AreaColor, write=i_AreaColor};
    __property TBrushStyle AreaStyle = {read=i_AreaStyle, write=i_AreaStyle};
    __property TFont* Font = {read=i_Font};

// . . . Public attributes (!)
    // Managing
    static typ_ActionIdxs ActionsIdxs; // Used to build popup menu

protected://                                                    ~~~~~~~~~~~~~

// . . . Protected attributes
    mutable TCanvas* Canvas; // Use it in 'Draw()' funct

// . . . Protected methods
    virtual void AssignStyle(const cls_FoilObject*); // Assign function

private://                                                      ~~~~~~~~~~~~~

// . . . Private attributes

// . . . Properties
    //class cls_Foil* i_Foil;
    bool i_Visible;
    int get_Visible() const {return i_Visible;}
    void set_Visible (const bool b){i_Visible=b; if(b) Reset();}
    // Graphic style
    TPenMode i_ColorMode;
    int i_LineWeight;
    TColor i_LineColor;
    TPenStyle i_LineStyle;
    TColor i_AreaColor;
    TBrushStyle i_AreaStyle;
    TFont* i_Font;

// . . . Private methods

}; // end 'cls_FoilObject'


/////////////////////////////// cls_FrameObject /////////////////////////////
//---------------------------------------------------------------------------
class cls_FrameObject : public cls_FoilObject { // Canvas object owned by a Frame

public://                                                       ~~~~~~~~~~~~~
// . . .  Types definitions

// . . . Public methods
    cls_FrameObject (class cls_Frame*);
    cls_FrameObject(const cls_FrameObject&); // Copy constructor
    virtual ~cls_FrameObject ()  {;}

    cls_FrameObject& operator=(const cls_FrameObject& fo) {AssignStyle(&fo); return *this;} // Copy assignment


// . . . Properties
    __property class cls_Frame* Owner = {read=i_Owner, write=i_Owner}; // Get owner frame

// . . . Public attributes (!)

protected://                                                    ~~~~~~~~~~~~~

// . . . Private attributes

// . . . Properties
    // Inherited
    // Specific
    class cls_Frame* i_Owner;

// . . . Private methods
}; // end 'cls_FrameObject'



/////////////////////////// cls_FrameDraggableObject ////////////////////////
//---------------------------------------------------------------------------
class cls_FrameDraggableObject : public cls_FrameObject, public cls_SizedObject { // A draggable frame object

public://                                                       ~~~~~~~~~~~~~

// . . . Public methods
    cls_FrameDraggableObject (class cls_Frame*);
    virtual ~cls_FrameDraggableObject ()  {;}

    virtual bool __fastcall IsOver(const TPoint& P) const {return Visible && mat::IsInRect(P,BoundsRect);} // Tell if a Canvas point is over

    // Dragging facilities
    void DragPoint(const TPoint& P) {DX=P.x-Left; DY=P.y-Top;} // Memorize the clicking point
    void DragPosition (const TPoint&); // Drag legend to a new position

// . . . Properties


// . . . Public attributes (!)

protected://                                                    ~~~~~~~~~~~~~

// . . . Protected attributes
    // position is relative to Frame's dimensions
    float TopRatio,LeftRatio;
    int DX,DY; // Relative position of drag point
    cls_SizedObject* sOwner; // Owner as sized object

private://                                                      ~~~~~~~~~~~~~

// . . . Properties
    int get_Top() const {return sOwner->Top+(TopRatio*sOwner->Height);}
    void set_Top(const int t){TopRatio=float(t-sOwner->Top)/sOwner->Height;}
    int get_Left() const {return sOwner->Left+(LeftRatio*sOwner->Width);}
    void set_Left(const int l){LeftRatio=float(l-sOwner->Left)/sOwner->Width;}

// . . . Private methods
}; // end 'cls_FrameDraggableObject'



////////////////////////////////// cls_Axes /////////////////////////////////
//---------------------------------------------------------------------------
class cls_Axes : public cls_FrameObject { // Frame axes

public://                                                       ~~~~~~~~~~~~~
// . . .  Types definitions

// . . . Public methods
    cls_Axes (class cls_Frame*);
    virtual ~cls_Axes ();

    // . . . Managing services
    virtual class cls_PropertiesInterface* CreateSpecInterface(class TWinControl*); // Create the edit properties interface
    virtual nms_MPlotClasses::en_FoilObjectType GetType(); // Get a enum type (deprecated)
    //virtual void AssignFont(class TFont* f); // Take into account the vertical font

    // . . . Graphic services
    virtual void __fastcall Draw(class cls_Foil*); // Render on parent's Canvas
    virtual bool __fastcall IsOver(const TPoint&) const; // Tell if a Canvas point is over
    virtual void __fastcall Reset() {;} // Reset operations, called on toggling 'Visible'
    virtual void __fastcall Remove() {Visible = false;} // Remove request

// . . . Properties
    __property String Xlabel = { read=i_Xlabel, write=i_Xlabel }; // X axis label
    __property String Ylabel = { read=i_Ylabel, write=i_Ylabel }; // Y axis label
    // Arrow dimension [pixels]
    __property int ArrowLength = {read=i_ArrowLength, write=i_ArrowLength};
    __property int ArrowWing = {read=i_ArrowWing, write=i_ArrowWing};

// . . . Public attributes (!)
    static const String Name;
    // Style deefaults
    static bool CenterLabels; // Center XY labels
    static TColor DefaultColor;

private://                                                      ~~~~~~~~~~~~~

// . . . Private attributes
    mutable TPoint O; // origin position
    //class TFont* i_VertFont;

// . . . Properties
    String i_Xlabel, i_Ylabel;
    int i_ArrowLength, i_ArrowWing;

// . . . Private methods
}; // end 'cls_Axes'



////////////////////////////////// cls_Label ////////////////////////////////
//---------------------------------------------------------------------------
class cls_Label : public cls_FrameObject { // A label in plot

public://                                                       ~~~~~~~~~~~~~
// . . .  Types definitions

// . . . Public methods
    cls_Label (class cls_Frame*); // const String txt, const double x, const double y
    virtual ~cls_Label ()  {;}

    // . . . Managing services
    virtual class cls_PropertiesInterface* CreateSpecInterface(class TWinControl*); // Create the edit properties interface
    virtual nms_MPlotClasses::en_FoilObjectType GetType(); // Get a enum type (deprecated)

    // . . . Graphic services
    virtual void __fastcall Draw(class cls_Foil*); // Render on parent's Canvas
    virtual bool __fastcall IsOver(const TPoint& P) const {return mat::IsInRect(P,i_LabelRect);} // Tell if a Canvas point is over
    virtual void __fastcall Reset() {;} // Reset operations, called on toggling 'Visible'
    virtual void __fastcall Remove() {;} // TODO: Remove request

// . . . Properties
    __property String Text = { read=i_Text, write=i_Text }; // String
    __property double x = { read=i_x, write=i_x }; // physical x position
    __property double y = { read=i_y, write=i_y }; // physical y position
    __property bool ShowCoords = { read=i_ShowCoords, write=i_ShowCoords }; // Show point coordinates

// . . . Public attributes (!)
    static const String Name;

private://                                                      ~~~~~~~~~~~~~

// . . . Private attributes
    TRect i_LabelRect; // A bag for displayed string rectangle

// . . . Properties
    String i_Text; // A bag for label
    double i_x, i_y;
    bool i_ShowCoords;

// . . . Private methods
}; // end 'cls_Label'


//////////////////////////////// cls_Labels ////////////////////////////////
//---------------------------------------------------------------------------
class cls_Labels : public std::vector<class cls_Label*> { // Label collection

public://                                                       ~~~~~~~~~~~~~
// . . .  Types definitions
    typedef std::vector<class cls_Label*> typ_Labels;
    typedef typ_Labels::iterator  typ_LabelsIt;
    typedef typ_Labels::const_iterator  typ_LabelsConstIt;

// . . . Public methods
    cls_Labels () {;}
    ~cls_Labels ()  { Clear(); }

    // TODO: do label remove request here
    void Paint(class cls_Foil* f) { for(l=begin();l!=end();++l) (*l)->Paint(f);} // Paint all
    void Clear() { for(l=begin();l!=end();++l) delete *l; clear();} // Clear all
    void Add(cls_Label* lp) {push_back(lp);}
    void Add(class cls_Frame* f, const String& txt, const double x, const double y, const bool sp){push_back(new cls_Label(f)); back()->Text=txt; back()->x=x; back()->y=y; back()->ShowCoords=sp;}
    void Remove(cls_Label* lp) {for(typ_LabelsIt li=begin();li!=end(); ++li) if((*li)==lp) {erase(li); break;}}

    cls_Label* IsOver(const TPoint& P) {for(l=begin();l!=end();++l){if((*l)->IsOver(P)) return *l;} return 0;}

// . . . Public attributes (!)
    static const String Name;

private://                                                      ~~~~~~~~~~~~~

// . . . Private attributes
    mutable typ_LabelsConstIt l;

// . . . Private methods
}; // end 'cls_Labels'

///////////////////////////////// cls_Marker ////////////////////////////////
//---------------------------------------------------------------------------
class cls_Marker : public cls_FrameObject { // Vertical/horizontal Marker

public://                                                       ~~~~~~~~~~~~~
// . . .  Types definitions
    typedef union {double Value; double* Address;} uni_Value;
    //typedef struct {uni_Value Value; bool Linked;} stu_Value;

// . . . Public methods
    cls_Marker(class cls_Frame*);
    virtual ~cls_Marker()  {;}

    // . . . Managing services
    virtual class cls_PropertiesInterface* CreateSpecInterface(class TWinControl*); // Create the edit properties interface
    virtual nms_MPlotClasses::en_FoilObjectType GetType(); // Get a enum type (deprecated)

    // . . . Graphic services
    virtual void __fastcall Draw(class cls_Foil*); // Render on parent's Canvas
    virtual bool __fastcall IsOver(const TPoint&) const; // Tell if a Canvas point is over
    virtual void __fastcall Reset() {;} // Reset operations, called on toggling 'Visible'
    virtual void __fastcall Remove() {;} // TODO: Remove request

    // Specific
    void SetValue (const double v) {i_Deferenced=false; i_Value.Value=v;}
    void SetAddress (double* const p) {i_Deferenced=true; i_Value.Address=p;}

// . . . Properties
    __property double Value = { read=get_Value }; // Quote
    __property String Label = { read=get_Label }; // Displayed string
    __property bool Vertical = { read=i_Vertical, write=i_Vertical }; // Orientation
    __property bool Deferenced = { read=i_Deferenced }; // // Value is deferenced
    __property String Prefix = { read=i_Prefix, write=i_Prefix }; // A prefix before value

// . . . Public attributes (!)
    static const String Name;
    // Style
    static int DefaultLineWeight;
    static TColor DefaultLineColor;
    static TPenStyle DefaultLineStyle;

private://                                                      ~~~~~~~~~~~~~

// . . . Private attributes
    int Q; // Pixel position
    String i_Label; // A bag for displayed string
    TRect i_LabelRect; // A bag for displayed string rectangle

// . . . Properties
    double get_Value() const {return i_Deferenced?*(i_Value.Address):i_Value.Value;}
    String get_Label() const {return i_Prefix + String(char('x'+!i_Vertical)) + String("=") + String(Value);}
    uni_Value i_Value;
    bool i_Vertical;
    bool i_Deferenced;
    String i_Prefix;

// . . . Private methods
}; // end 'cls_Marker'


//////////////////////////////// cls_Markers ////////////////////////////////
//---------------------------------------------------------------------------
class cls_Markers : public std::vector<class cls_Marker*> { // Markers collection

public://                                                       ~~~~~~~~~~~~~
// . . .  Types definitions
    typedef std::vector<class cls_Marker*> typ_Markers;
    typedef typ_Markers::iterator  typ_MarkersIt;
    typedef typ_Markers::const_iterator  typ_MarkersConstIt;

// . . . Public methods
    cls_Markers () {;}
    ~cls_Markers ()  { Clear(); }

    // TODO: do marker remove request here
    void Paint(class cls_Foil* f) { for(m=begin();m!=end();++m) (*m)->Paint(f);} // Paint all
    void RemoveDeferenced() {typ_MarkersIt i=begin(); while(i!=end()) {if ((*i)->Deferenced) {delete *i; i=erase(i);} else ++i;}} // Remove deferenced
    void Clear() { for(m=begin();m!=end();++m) delete *m; clear();} // Clear all
    void Add(cls_Marker* mp) {push_back(mp);}
    void Add(class cls_Frame* f, double v, bool vert) {push_back(new cls_Marker(f)); back()->SetValue(v); back()->Vertical = vert;}
    void Add(class cls_Frame* f, double* p, bool vert) {push_back(new cls_Marker(f)); back()->SetAddress(p); back()->Vertical = vert;}
    void Remove(cls_Marker* mp) {for(typ_MarkersIt mi=begin();mi!=end();++mi) if((*mi)==mp) {erase(mi); break;}}

    cls_Marker* IsOver(const TPoint& P) {for(m=begin();m!=end();++m){if((*m)->IsOver(P)) return *m;} return 0;}

// . . . Public attributes (!)
    static const String Name;

private://                                                      ~~~~~~~~~~~~~

// . . . Private attributes
    mutable typ_MarkersConstIt m;

// . . . Private methods
}; // end 'cls_Markers'




/////////////////////////////// cls_GridLines ///////////////////////////////
// Pixels -> Units -> Round: Dz=Divs[k]*10^n , Divs={1,2,5}
//
//   Dz  | 0.01 0.02 0.05 | 0.1 0.2 0.5 | 1 2 5 | 10 20 50 | 100 200 500 | ...
//Divs[k]|   1    2    5  |  1   2   5  | 1 2 5 |  1  2  5 |  1   2   5  | ...
//   k   |   0    1    2  |  0   1   2  | 0 1 2 |  0  1  2 |  0   1   2  | ...
//   n   |       -2       |     -1      |   0   |     1    |      2      | ...

//---------------------------------------------------------------------------
class cls_GridLines { // Regarding grid lines

public://                                                       ~~~~~~~~~~~~~
// . . .  Types definitions

// . . . Public methods
    //---------------------------------------------------------
    cls_GridLines ()
    { // Constructor
        i_Pixels = 20;
        i_Index = i_Offset = i_Order = 0;
    }

    //---------------------------------------------------------
    void Calculate (double z, bool tens)
    { // Calculate grid distance given a desired one (linear scale)
        //ASSERTING((z>0),"grid lines distance must be positive") // not using '::fabs'
        // decompose   z = k * 10^order  , 1<=k<10
        double order; // order of magnitude
        double Logk = std::modf(std::log10(std::fabs(z)),&order);
        if ( Logk<0 ) {--order; ++Logk;} // Ceil negative values
        double k = std::pow( 10, Logk );
        // Nearest rounded value
        i_Order = mat::round(order); // TODO 3: if(::abs(order)>308) deny;
        ASSERTING((std::abs(i_Order)<=308),"Grid lines delta out of range")
        if (tens) { i_Index = 0; i_Offset = 0; } // only power of tens
        else { // see grid divisions
              i_Index = Divs.size();
              while ( (i_Index>0) && (k<Divs[i_Index-1]) ) --i_Index;
             }
    }

    //---------------------------------------------------------
    String GetDivsString (const char d) const
    { // Get a string of divisions (given a delimiter)
        String ls = "";
        for (unsigned int i=0; i<Divs.size(); ++i) ls += String(Divs[i]) + String(d);
        return ls.SubString(1,ls.Length()-1);
    }

// . . . Properties
    // Options
    __property int Pixels = {read=i_Pixels, write=set_Pixels}; // Desired pixel distance
    __property double Delta = {read=get_Delta}; // Current physic distance between lines
    __property int Index = {read=i_Index}; // Current index of divisions array
    __property int Offset = {read=i_Offset, write=i_Offset}; // Current offset applied to index
    __property int Order = {read=i_Order}; // Current order of magnitude (ten power)

// . . . Public attributes (!)
	mat::numlist Divs;

private://                                                      ~~~~~~~~~~~~~

// . . . Private attributes

// . . . Properties
    int i_Pixels,i_Index,i_Offset,i_Order;
    void set_Pixels (const int p){i_Pixels=(p>0)?p:1;} // Set pixel distance
    //---------------------------------------------------------
    double get_Delta() const
    { // Choose physic distance
        // Antitransform (apply offset)
        int DivsSize = Divs.size();
        if ( DivsSize < 1 ) return 1.0;
        int N = i_Order*DivsSize + i_Index + i_Offset;
        // Transform
        int ord = N/DivsSize;
        int idx = N%DivsSize;
        if (idx<0) {--ord; idx+=DivsSize;}
        // Finally
        return std::pow10(ord) * Divs[idx];
    }

// . . . Private methods

}; // end 'cls_GridLines'

//---------------------------------------------------------------------------
class cls_Grid : public cls_FrameObject { // Frame grid

public://                                                       ~~~~~~~~~~~~~
// . . .  Types definitions

// . . . Public methods
    cls_Grid (class cls_Frame*);
    virtual ~cls_Grid ()  {;}

    // . . . Managing services
    virtual class cls_PropertiesInterface* CreateSpecInterface(class TWinControl*); // Create the edit properties interface
    virtual nms_MPlotClasses::en_FoilObjectType GetType(); // Get a enum type (deprecated)

    // . . . Graphic services
    virtual void __fastcall Draw(class cls_Foil*); // Render on parent's Canvas
    virtual bool __fastcall IsOver(const TPoint& P) const {return false;}
    virtual void __fastcall Reset() {Vlines.Offset=0; Hlines.Offset=0;} // Reset operations, called on toggling 'Visible'
    virtual void __fastcall Remove() {Visible = false;} // Remove request

    void RecalculateVlines(); // Calculate vertical grid increment
    void RecalculateHlines(); // Calculate horizontal grid increment
    void IncrementVOffset (const int); // Increment offset on grid thickness (vertical lines)
    void IncrementHOffset (const int); // Increment offset on grid thickness (horizontal lines)
    void IncrementOffset (const int); // Increment offset on grid thickness

// . . . Properties
    __property int VGridDist = {read=get_VGridDist, write=set_VGridDist}; // Desired vertical lines distance
    __property int HGridDist = {read=get_HGridDist, write=set_HGridDist}; // Desired horizontal lines distance
    __property String VGridDivs = {read=get_VGridDivs, write=set_VGridDivs}; // Vertical divisions string
    __property String HGridDivs = {read=get_HGridDivs, write=set_HGridDivs}; // Horizontal divisions string

// . . . Public attributes (!)
    static const String Name;
    int LabelSep; // Separation of text labels, written at every draw
    // Style defaults
    static String DefaultFont; // Grid labels
    static TPenStyle DefaultLinesStyle; // Grid lines style
    static TColor DefaultColor; // Grid lines color
    static String DefaultVGridDivs; // Divisions of grid lines
    static String DefaultHGridDivs;
    static int DefaultVGridDist; // Density of grid lines
    static int DefaultHGridDist;

private://                                                      ~~~~~~~~~~~~~

// . . . Private attributes
    cls_GridLines Vlines, Hlines;

// . . . Properties
    int get_VGridDist() const {return Vlines.Pixels;}
    void set_VGridDist(const int d) {Vlines.Pixels = d;}
    int get_HGridDist() const {return Hlines.Pixels;}
    void set_HGridDist(const int d) {Hlines.Pixels = d;}

    String get_VGridDivs() const {return Vlines.GetDivsString('|');}
    void set_VGridDivs(const String& s) { Vlines.Divs.clear(); mat::StringToNumlist(Vlines.Divs,s); Vlines.Offset=0; }
    String get_HGridDivs() const {return Hlines.GetDivsString('|');}
    void set_HGridDivs(const String& s) { Hlines.Divs.clear(); mat::StringToNumlist(Hlines.Divs,s); Hlines.Offset=0; }
 
// . . . Private methods

}; // end 'cls_Grid'


///////////////////////////////// cls_Legend ////////////////////////////////
//---------------------------------------------------------------------------
class cls_Legend : public cls_FrameDraggableObject { // Contained tracks legend

public://                                                       ~~~~~~~~~~~~~

// . . . Public methods
    cls_Legend (class cls_Frame*);
    virtual ~cls_Legend ()  {;}

    // . . . Managing services
    virtual class cls_PropertiesInterface* CreateSpecInterface(class TWinControl*); // Create the edit properties interface
    virtual nms_MPlotClasses::en_FoilObjectType GetType(); // Get a enum type (deprecated)

    // . . . Graphic services
    virtual void __fastcall Draw(class cls_Foil*); // Render on parent's Canvas
    //virtual bool __fastcall IsOver(const TPoint&) const; // Already defined
    virtual void __fastcall Reset(); // Reset position and dimension
    virtual void __fastcall Remove() {Visible = false;} // Remove request

    // . . . Specfic services
    void SelectTrack(const TPoint&); // Select track from position

// . . . Properties
    // Inherited 'Margin' is used for lines and border spacing
    __property int StrokeLength = {read=i_StrokeLength, write=i_StrokeLength}; // Length of track stroke [pix]

// . . . Public attributes (!)
    static const String Name;
    // Style defaults
    static int DefaultSpacing; // Lines and border spacing
    static int DefaultStrokeLength; // Length of track stroke
    static TColor DefaultColor; // Background

private://                                                      ~~~~~~~~~~~~~

// . . . Private attributes
    int i_LineHeight; // Height of line+spacing

// . . . Properties
    int i_StrokeLength;

// . . . Private methods
}; // end 'cls_Legend'



//---------------------------------------------------------------------------
#endif
