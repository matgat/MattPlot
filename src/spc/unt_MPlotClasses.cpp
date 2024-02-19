//---------------------------------------------------------------------------
#include <vcl.h>
#include "unt_MatCommon.h"
#pragma hdrstop
#include "unt_MPlot.h" // ToString()
#include "unt_MPlotClasses.h"
#include "unt_Frames.h" // to access frame properties
#include "unt_Tracks.h" // to draw measure lines
#include "unt_PropertiesInterface.h" // To instantiate interface
#pragma package(smart_init)
//---------------------------------------------------------------------------



////////////////////////////// cls_VolatileText /////////////////////////////

//============================= Public methods ==============================

//---------------------------------------------------------------------------
cls_VolatileText::cls_VolatileText ()
{ // Constructor

// . . . Main
    Reset();

// . . . Other
    i_Offset = 0;
}

//---------------------------------------------------------------------------
// Set places
inline void cls_VolatileText::SetPlace (const int x, const int y, const en_VAlgn va, const en_HAlgn ha)
{
    i_Place.X = x;
    i_Place.Y = y;
    i_Place.VAlgn = va;
    i_Place.HAlgn = ha;
}


//---------------------------------------------------------------------------
// Render in Canvas, given eventual inverse background color
inline void cls_VolatileText::Render (class TCanvas* Canvas, const TColor BkgInv)
{
    // Render only for valid positions
    if ( i_Place.X<=0 || i_Place.Y<=0 ) return;

// . . . Xoring text using temporary memory bitmap (VCL)
    // . . . Prepare bitmap
    Graphics::TBitmap* bmp = new Graphics::TBitmap();
    try {
         // Size
         bmp->Width = Canvas->TextWidth(i_Text);
         bmp->Height = Canvas->TextHeight(i_Text);
         TRect R1 = Rect(0, 0, bmp->Width, bmp->Height);
         // >>> Transparent (TColor(mat::Invert(Canvas->Brush->Color))) or highlighted background
         bmp->Canvas->Brush->Color = BkgInv;
         // Ensure background (uncomment if problems)
         //bmp->Canvas->Brush->Style = bsSolid;
         //bmp->Canvas->FillRect(R1); bmp->Canvas->Brush->Style = bsClear;
         // Text Font
         bmp->Canvas->Font->Assign( Canvas->Font );
         // >>> Transparent text if Highlight color is the same of inverted font color
         //TColor clInvFont = TColor(mat::Invert(Canvas->Font->Color));
         //bmp->Canvas->Font->Color = (clInvFont != bmp->Canvas->Brush->Color) ? clInvFont : clInvBackground; //Canvas->Font->Color;
         // Use Plot Font color
         bmp->Canvas->Font->Color = TColor(mat::Invert(Canvas->Font->Color));
         // Out text
         bmp->Canvas->TextOut(0, 0, i_Text);
         // Now bitmap is ready, can copy it in inverse mode
         Canvas->CopyMode = cmSrcInvert;
         // . . . Copying bitmap
         // Prepare where
         int x = i_Place.X;
         int y = i_Place.Y;
         if ( i_Place.HAlgn == AL_CENTER ) x -= Canvas->TextWidth(i_Text)/2;
         else if ( i_Place.HAlgn == AL_RIGHT ) x -= Canvas->TextWidth(i_Text) + i_Offset;
         else x += i_Offset; // AL_LEFT
         if ( i_Place.VAlgn == AL_MIDDLE ) y -= Canvas->TextHeight(i_Text)/2;
         else if ( i_Place.VAlgn == AL_BOTTOM ) y -= Canvas->TextHeight(i_Text) + i_Offset;
         else y += i_Offset; // AL_TOP
         // Put text
         Canvas->CopyRect(Rect(x, y, x + bmp->Width, y + bmp->Height), bmp->Canvas, R1);
        } // end 'try'
    __finally {
               delete bmp;
              }
}




///////////////////////////// cls_VolatileDrawer ////////////////////////////

//============================= Static Members ==============================

//============================= Public methods ==============================

//---------------------------------------------------------------------------
cls_VolatileDrawer::cls_VolatileDrawer (class cls_MPlot* o) : i_Owner(o)
{ // Constructor

// . . . Ptoperties
    // Marking zone service (dragging frames)
    i_MarkZoneStyle  = bsDiagCross;
    i_MarkColor = TColor(0x849C84);
    // Selecting rectangle service (zoom and crop)
    i_SelRectLineStyle = psDot;
    i_SelRectLineColor = TColor(0xA77B84);
    i_SelRectAreaColor = TColor(0xA77B84);
    // Lines service (measuring lines)
    MeasureLinesColor = TColor(0xE91984); // sets i_MeasureLinesColor1 and i_MeasureLinesColor2;
    i_MeasureLinesStyle = psDashDot;
    i_ViewVerticalLine = i_ViewHorizontalLine = i_ViewSlopeLine = false;
    i_HighlightInvBkgColor = clWhite;
    // Scribbling service (pen mode)
    i_PencilColor = clDkGray;
    i_HighlighterColor = clYellow;
    i_PencilSize = 1;
    i_HighlighterRadius = 10;

// . . . Contained objects
    // Volatile text
    AbscissaText = new cls_VolatileText();
    AbscissaTextGrid = new cls_VolatileText();
    OrdinateText = new cls_VolatileText();
    OrdinateTextGrid = new cls_VolatileText();
    SlopeText = new cls_VolatileText();

    AbscissaText->Offset = 2;
    OrdinateText->Offset = AbscissaText->Offset;
    SlopeText->Offset = AbscissaText->Offset;

// . . . Other initializations
    CurrentRepaintTime = 0;
    Reset();
    Desaturation = 90;
}

//---------------------------------------------------------------------------
cls_VolatileDrawer::~cls_VolatileDrawer ()
{
    delete SlopeText;
    delete OrdinateTextGrid;
    delete OrdinateText;
    delete AbscissaTextGrid;
    delete AbscissaText;
}

//---------------------------------------------------------------------------
void cls_VolatileDrawer::MarkZone (const TRect& R)
{ // Fill the dropping zone

// . . . First update repaint time and zone rectangle
    CurrentRepaintTime = Owner->Repaints;
    ZoneRect = R;

// . . . Then set canvas tools
    Canvas->Pen->Width = 2;
    Canvas->Pen->Style = psClear;
    Canvas->Pen->Mode = pmNotXor;
    Canvas->Brush->Color = MarkColor;
    Canvas->Brush->Style = MarkZoneStyle;

// . . . Finally do paint operations
    Owner->CurrentFoil->Canvas->Rectangle(ZoneRect);
}


//---------------------------------------------------------------------------
void cls_VolatileDrawer::UnMarkZone()
{ // Unmark last marked zone

// . . . Erase last mark if Canvas not repainted   && ZoneRect.bottom>0
    if ( CurrentRepaintTime==Owner->Repaints ) MarkZone(ZoneRect);

// . . . Reset state
    ResetZone();
}

//---------------------------------------------------------------------------
void cls_VolatileDrawer::DrawDirection(const TPoint& P1, const TPoint& P2)
{ // Draw a spring during enhanced pan

// . . . First update repaint time and points
    CurrentRepaintTime = Owner->Repaints;
    P_dir1 = P1;
    P_dir2 = P2;

// . . . Then set canvas tools
    Canvas->Pen->Width = 1;
    Canvas->Pen->Color = SelRectLineColor;
    Canvas->Pen->Mode = pmNotXor;
    //Canvas->Brush->Style = bsClear;

// . . . Finally do paint operations
    Canvas->Pen->Style = SelRectLineStyle;
    Canvas->MoveTo(P_dir1.x,P_dir1.y);
    Canvas->LineTo(P_dir2.x,P_dir2.y);
    Canvas->Pen->Style = psSolid;
    Canvas->Ellipse(P_dir1.x-4,P_dir1.y-4,P_dir1.x+4,P_dir1.y+4);
}

//---------------------------------------------------------------------------
void cls_VolatileDrawer::EraseDirection()
{ // Erase last spring

// . . . Erase last spring if Canvas not repainted
    if ( CurrentRepaintTime==Owner->Repaints ) DrawDirection(P_dir1,P_dir2);

// . . . Reset state
    ResetDirection ();
}

//---------------------------------------------------------------------------
void cls_VolatileDrawer::DrawSelRect (const TRect& R)
{ // Draw a rubber band Rectangle

// . . . First update repaint time and rectangle
    CurrentRepaintTime = Owner->Repaints;
    SelRect = R;

// . . . Then set canvas tools
    Canvas->Pen->Width = 1;
    Canvas->Pen->Style = SelRectLineStyle;
    Canvas->Pen->Color = SelRectLineColor;
    if ( SelRectAreaColor != SelRectLineColor )
         { // Solid area
          Canvas->Brush->Style = bsSolid;
          Canvas->Brush->Color = SelRectAreaColor;
         }
    else { // Transparent area
          Canvas->Brush->Style = bsClear;
         }
    Canvas->Pen->Mode = pmNotXor;

// . . . Finally do paint operations
    Canvas->Rectangle(SelRect);
}

//---------------------------------------------------------------------------
void cls_VolatileDrawer::EraseSelRect()
{ // Erase last selection rectangle

// . . . Erase last rectangle if Canvas not repainted
    if ( CurrentRepaintTime==Owner->Repaints ) DrawSelRect( SelRect );

// . . . Reset state
    ResetSelRect();
}


//---------------------------------------------------------------------------
void cls_VolatileDrawer::DrawMeasureLines1 ( bool persistent )
{ // Draw 1st measure lines for a track

// . . . Specific ops
    Canvas->Pen->Color = i_MeasureLinesColor1;

// . . . Store fixed measure quantities for interval calculations
    // TODO: use interval quantities instead!
    x1 = Owner->p.trk.x;
    y1 = Owner->p.trk.y;
    dydx1 = Owner->p.trk.dydx;

// . . . Get measure points
    RetrieveMeasuresPoints(mP1,mS1);

// . . . Render lines (and text)
    if ( IsDrawingInterval() ) RenderMeasureLines(mP1,mS1);
    if ( persistent ) ResetMeasure();
}

//---------------------------------------------------------------------------
void cls_VolatileDrawer::DrawMeasureLines2 ()
{ // Draw 2nd measure lines for a track
/*
    Points description
        punctual measure:
            mP2 is the point, mS2 is the displacement of tangent extreme
        interval measure:
            mP1 and mP2 are the first and second point
            mS1 and mS2 are the displacements of extremes of tangents
*/
// . . . Specific ops
    Canvas->Pen->Color = i_MeasureLinesColor2;
    CurrFrame = Owner->CurrentFrame;
    // Adapt grid text
    AbscissaTextGrid->Offset = CurrFrame->Grid->LabelSep;
    OrdinateTextGrid->Offset = AbscissaTextGrid->Offset;
    // For font size calculations
    Canvas->Font->Assign(Owner->TextFont);

// . . . Get measure points
    RetrieveMeasuresPoints(mP2,mS2);

// . . . Set text to be rendered (where and what)
    if ( IsntDrawingInterval() )
         {// . . . Point infos

          // Current abscissa on both sides
          AbscissaTextGrid->Text = CurrFrame->XToString(Owner->p.trk.x);
          AbscissaTextGrid->SetPlace(mP2.x, CurrFrame->bottom, cls_VolatileText::AL_TOP, cls_VolatileText::AL_CENTER);
          AbscissaText->Text = AbscissaTextGrid->Text;  // String("x: ") +
          AbscissaText->SetPlace(mP2.x, CurrFrame->top, cls_VolatileText::AL_BOTTOM, cls_VolatileText::AL_CENTER);

          // Current ordinate on both sides
          if ( ViewHorizontalLine )
             {
              OrdinateTextGrid->Text = CurrFrame->Owner->ToString(Owner->p.trk.y);
              OrdinateTextGrid->SetPlace(CurrFrame->left, mP2.y, cls_VolatileText::AL_MIDDLE, cls_VolatileText::AL_RIGHT);
              OrdinateText->Text = OrdinateTextGrid->Text;  // String("y: ") +
              OrdinateText->SetPlace(CurrFrame->Right, mP2.y, cls_VolatileText::AL_MIDDLE, cls_VolatileText::AL_RIGHT);
             }  // end 'if ViewHorizontalLine'

          // Slope
          if ( ViewSlopeLine )
             {
              SlopeText->Text = String("y’: ") + CurrFrame->Owner->ToString(Owner->p.trk.dydx);
              SlopeText->SetPlace(mP2.x+mS2.x, mP2.y-mS2.y, cls_VolatileText::AL_TOP, cls_VolatileText::AL_LEFT);
             }  // end 'if ViewSlopeLine'
         } // end 'if punctual measure'
    else {// . . . Interval infos

          // Current abscissa
          AbscissaTextGrid->Text = CurrFrame->XToString(Owner->p.trk.x);
          AbscissaTextGrid->SetPlace(mP2.x, CurrFrame->bottom, cls_VolatileText::AL_TOP, cls_VolatileText::AL_CENTER);
          // Abscissa displacement: detect when interval can contain the label
          AbscissaText->Text = CurrFrame->XToString(Owner->p.trk.x - x1); // String("Dt: ") +
          int D = mP2.x - mP1.x;
          bool IsWide = std::abs(D) > (2*Canvas->TextWidth(AbscissaText->Text) + AbscissaText->Offset);
          AbscissaText->SetPlace(mP1.x+(D)/2, CurrFrame->top, IsWide?cls_VolatileText::AL_BOTTOM:cls_VolatileText::AL_TOP, cls_VolatileText::AL_CENTER);
          //AbscissaText->SetPlace(mP2.x, mP2.y + Canvas->Font->Size + 5, cls_VolatileText::AL_TOP, cls_VolatileText::AL_LEFT);

          // Ordinate
          if ( ViewHorizontalLine )
             {
              // Current ordinate
              OrdinateTextGrid->Text = CurrFrame->Owner->ToString(Owner->p.trk.y);
              OrdinateTextGrid->SetPlace(CurrFrame->left, mP2.y, cls_VolatileText::AL_MIDDLE, cls_VolatileText::AL_RIGHT);
              // Ordinate displacement: detect when interval can contain the label
              OrdinateText->Text = CurrFrame->Owner->ToString(Owner->p.trk.y - y1); // String("Dy: ") +
              int D = mP2.y - mP1.y;
              bool IsWide = std::abs(D) > (2*Canvas->TextHeight(OrdinateText->Text) + OrdinateText->Offset);
              OrdinateText->SetPlace(IsWide?CurrFrame->Right:CurrFrame->right, mP1.y+(D)/2, cls_VolatileText::AL_MIDDLE, cls_VolatileText::AL_RIGHT);
              //OrdinateText->SetPlace(mP2.x, mP2.y, cls_VolatileText::AL_TOP, cls_VolatileText::AL_LEFT);
             }  // end 'if ViewHorizontalLine'

          // Slope
          if ( ViewSlopeLine )
             {
              // Derivate (tangent)
              SlopeText->Text = String("Dy’: ") + CurrFrame->Owner->ToString(Owner->p.trk.dydx - dydx1);
              SlopeText->SetPlace(mP2.x+mS2.x, mP2.y-mS2.y, cls_VolatileText::AL_TOP, cls_VolatileText::AL_LEFT);
              // // Incremental derivate
              //SlopeText->Text = String("Dy/Dt: ") + CurrFrame->Owner->ToString(Owner->p.trk.DyDt);
              //SlopeText->SetPlace(mP2.x, mP2.y, cls_VolatileText::AL_TOP, cls_VolatileText::AL_LEFT);
             } // end 'if ViewSlopeLine'
         } // end 'interval measure'

// . . . Render lines (and text)
    if ( mP2.x >= 0 ) RenderMeasureLines(mP2,mS2);
}

//---------------------------------------------------------------------------
void cls_VolatileDrawer::EraseMeasureLines1()
{ // Erase 1st measure lines

    Canvas->Pen->Color = i_MeasureLinesColor1;
    EraseMeasureLines(mP1,mS1);
}

//---------------------------------------------------------------------------
void cls_VolatileDrawer::EraseMeasureLines2()
{ // Erase 2nd measure lines

    Canvas->Pen->Color = i_MeasureLinesColor2;
    EraseMeasureLines(mP2,mS2);
}

//---------------------------------------------------------------------------
void cls_VolatileDrawer::SetPencil()
{ // Set Canvas for pencil stroke

    Canvas->Pen->Color = PencilColor;
    Canvas->Pen->Width = PencilSize;
    Canvas->Pen->Style = psSolid;
    Canvas->Pen->Mode = pmCopy;
}

//---------------------------------------------------------------------------
void cls_VolatileDrawer::SetHighlighter()
{ // Set Canvas for highlighter stroke

    Canvas->Pen->Style = psClear;
    Canvas->Brush->Color = HighlighterColor;
    Canvas->Brush->Style = bsSolid;
    Canvas->Pen->Mode = pmNotXor;
}

//---------------------------------------------------------------------------
void cls_VolatileDrawer::Stroke (const TPoint& P1, const TPoint& P2)
{ // Draw with selected tool
        
    if ( Canvas->Pen->Style == psSolid )
       { Canvas->MoveTo(P1.x,P1.y);
         Canvas->LineTo(P2.x,P2.y); }
    else Canvas->Ellipse( P2.x - i_HighlighterRadius,
                          P2.y - i_HighlighterRadius,
                          P2.x + i_HighlighterRadius,
                          P2.y + i_HighlighterRadius );
}




//============================= Private methods =============================


//---------------------------------------------------------------------------
inline void cls_VolatileDrawer::RenderMeasureLines (TPoint& P, TPoint& S)
{ // Draw measure lines for a track

// . . . First update repaint time
    CurrentRepaintTime = Owner->Repaints;

// . . . Paint lines
    // . . . Set canvas tools for lines
    Canvas->Pen->Width = 1;
    //Canvas->Pen->Color = MeasureLinesColor; // Differentiate colors
    Canvas->Pen->Style = MeasureLinesStyle;
    Canvas->Pen->Mode = pmNotXor;
    Canvas->Brush->Style = bsClear;
    // ...and paint them
    // Vertical line
    if ( ViewVerticalLine ) Owner->CurrentFrame->DrawVLine(Canvas, P.x);
    // Horizontal line
    if ( ViewHorizontalLine )
       {
        Owner->CurrentFrame->DrawHLine(Canvas, P.y);
        // Draw the connecting line
        //if ( IsDrawingInterval() ) {Canvas->MoveTo(mP1.x,mP1.y); Canvas->LineTo(P.x,P.y);}
       }
    // Slope line
    if ( ViewSlopeLine && S.x>-1 )
       {
        Canvas->MoveTo(P.x-S.x,P.y+S.y);
        Canvas->LineTo(P.x+S.x,P.y-S.y);
       }

// . . . Render the volatile text
    // Warning: (!)Don't draw lines anymore, since Render alters Canvas props
    // Plot text
    Canvas->Font->Assign(Owner->TextFont);
    if ( ViewVerticalLine ) AbscissaText->Render(Canvas,i_HighlightInvBkgColor);
    if ( ViewHorizontalLine ) OrdinateText->Render(Canvas,i_HighlightInvBkgColor);
    if ( ViewSlopeLine && S.x>-1 ) SlopeText->Render(Canvas,i_HighlightInvBkgColor);
    // Grid text
    Canvas->Font->Assign(Owner->CurrentFrame->Grid->Font);
    AbscissaTextGrid->Render(Canvas, TColor(mat::Invert(Canvas->Brush->Color)));
    if ( ViewHorizontalLine ) OrdinateTextGrid->Render(Canvas, TColor(mat::Invert(Canvas->Brush->Color)));
}


//---------------------------------------------------------------------------
inline void cls_VolatileDrawer::EraseMeasureLines (TPoint& P, TPoint& S)
{ // Erase last measure lines if existing

// . . . Erase last rectangle if Canvas not repainted
    if ( CurrentRepaintTime==Owner->Repaints ) RenderMeasureLines(P,S);

// . . . Reset state
    ResetMeasure(P,S);
}

//---------------------------------------------------------------------------
void cls_VolatileDrawer::RetrieveMeasuresPoints ( TPoint& P, TPoint& S )
{// Get screen ordinate and slope (-1,-1 if invalid)

    ASSERTING((Owner->CurrentFrame),"A frame must be selected!")

// . . . Local variables
    static int L; // Length of half slope line [pix]
    static double a; // Screen angle of slope line

// . . . Initializations
    CurrFrame = Owner->CurrentFrame;
    S = TPoint(-1,-1);

    // Current track abscissa in screen space
    P.x = (!mat::isNaN(Owner->p.trk.x)) ? CurrFrame->Xpix(Owner->p.trk.x) : -1;
    // Current track ordinate in screen space
    P.y = (!mat::isNaN(Owner->p.trk.y)) ? CurrFrame->Ypix(Owner->p.trk.y) : -1;
    if ( P.y<CurrFrame->Top || P.y>CurrFrame->Bottom ) P.y = -1; // Don't draw if outside frame

    // Current track slope point in screen space
    if ( ViewSlopeLine && !mat::isNaN(Owner->p.trk.dydx) && P.y>=0 )
       {
        // Screen angle of slope line (not valid in Logarithmic scale)
		a = std::atan( Owner->p.trk.dydx * (CurrFrame->PixelSizeX/CurrFrame->PixelSizeY) );
        // Length of half slope line [pix]
        L = mat::min(
					 mat::min(std::abs(int(CurrFrame->Bottom - P.y)),std::abs(int(P.y - CurrFrame->Top))),
					 mat::min(std::abs(int(CurrFrame->Right - P.x)),std::abs(int(P.x - CurrFrame->Left)))
                    );
        // Calculate slope segment extremity displacement
		S = TPoint( L*std::cos(a) , L*std::sin(a) );
       }
}



//=============================== Properties ================================
//---------------------------------------------------------------------------
void cls_VolatileDrawer::set_MeasureLinesColor(const TColor C)
{
        i_MeasureLinesColor2 = C;
        i_MeasureLinesColor1 = TColor(mat::Desaturate(C, Desaturation));
}
//---------------------------------------------------------------------------
inline class TCanvas* cls_VolatileDrawer::get_Canvas () {return Owner->CurrentFoil->Canvas;}





/////////////////////////////// cls_FoilObject ////////////////////////////
//============================= Static Members ==============================

    cls_FoilObject::typ_ActionIdxs cls_FoilObject::ActionsIdxs; // Used to build popup menu

//============================= Public methods ==============================

//---------------------------------------------------------------------------
cls_FoilObject::cls_FoilObject()
{ // Constructor

// . . . Default values
    //i_Foil = 0;
    i_Visible = true; // warning: Visible calls pure virtual funtion Reset()
    // Graphic style
    ColorMode = pmCopy;
    LineWeight = 1;
    LineColor = clBlack;
    LineStyle = psSolid;
    AreaColor = clWhite;
    AreaStyle = bsClear;

// . . . Class instances
    i_Font = new TFont();
}

//---------------------------------------------------------------------------
cls_FoilObject::cls_FoilObject(const cls_FoilObject& fo)
{ // Copy constructor

// . . . Class instances
    i_Font = new TFont();

// . . . Default values
    AssignStyle(&fo);
}

//---------------------------------------------------------------------------
cls_FoilObject::~cls_FoilObject ()
{ // Destructor
    delete i_Font;
}

//---------------------------------------------------------------------------
void cls_FoilObject::AssignStyle( const cls_FoilObject* fo )
{ // Assign Canvas style

    if ( this!=fo && fo )
       {
        i_Visible = fo->Visible;
        AssignFont(fo->Font);
        i_LineWeight = fo->LineWeight;
        i_LineColor = fo->LineColor;
        i_AreaColor = fo->LineColor;
        i_ColorMode = fo->ColorMode;
        i_AreaStyle = fo->AreaStyle;
        i_LineStyle = fo->LineStyle;
       }
}

//---------------------------------------------------------------------------
bool cls_FoilObject::IsDifferentFrom( const class cls_FoilObject* fo )
{ // A comparison function

    return mat::AreDifferent(Font,fo->Font) ||
           ( LineWeight != fo->LineWeight ) ||
           ( LineColor != fo->LineColor )   ||
           ( AreaColor != fo->LineColor )   ||
           ( ColorMode != fo->ColorMode )   ||
           ( AreaStyle != fo->AreaStyle )   ||
           ( LineStyle != fo->LineStyle );
}


//---------------------------------------------------------------------------
// Create the edit properties interface
cls_PropertiesInterface* cls_FoilObject::CreateSpecInterface(class TWinControl* p)
{
    return new cls_FoilObjectEdit(p,this);
}

//---------------------------------------------------------------------------
// Create the edit FoilObject properties interface
cls_PropertiesInterface* cls_FoilObject::CreateStyleInterface(class TWinControl* p)
{
    return new cls_FoilObjectEdit(p,this);
}

//---------------------------------------------------------------------------
// Get a enum type (deprecated)
nms_MPlotClasses::en_FoilObjectType cls_FoilObject::GetType()
{
    return nms_MPlotClasses::OB_NONE;
}




//============================= Private methods =============================






/////////////////////////////// cls_FrameObject /////////////////////////////
//============================= Static Members ==============================
//============================= Public methods ==============================
//---------------------------------------------------------------------------
cls_FrameObject::cls_FrameObject (class cls_Frame* f)
: i_Owner(f)
{ // Constructor
// . . . Default
    // Inherited
    if (i_Owner) AssignStyle(i_Owner);
}

//---------------------------------------------------------------------------
cls_FrameObject::cls_FrameObject(const cls_FrameObject& fro)
: cls_FoilObject(fro), i_Owner(fro.Owner)
{ // Copy constructor
}

//=============================== Properties ================================
//---------------------------------------------------------------------------






/////////////////////////// cls_FrameDraggableObject ////////////////////////
//============================= Static Members ==============================
//============================= Public methods ==============================

//---------------------------------------------------------------------------
cls_FrameDraggableObject::cls_FrameDraggableObject (class cls_Frame* f) : cls_FrameObject(f)
{ // Constructor

    sOwner = dynamic_cast<cls_SizedObject*>(Owner);
    ASSERTING((sOwner),"Failed a dynamic cast")

// . . . Default values
    TopRatio = LeftRatio = 0.1;
    DX = DY = 0;
}

//---------------------------------------------------------------------------
void cls_FrameDraggableObject::DragPosition (const TPoint& P)
{ // Drag legend to a new position
    // I know relative position of dragging point: DX,DY
    // so the position where the legend should go:
    int FutureLeft = P.x - DX;
    int FutureTop = P.y - DY;
    // Don't drag outside frame
    if ( FutureLeft > Owner->Left && FutureLeft+Width < Owner->Right ) Left = FutureLeft;
    if ( FutureTop > Owner->Top && FutureTop+Height < Owner->Bottom )  Top = FutureTop;
}


//============================= Private methods =============================






///////////////////////////////// cls_Axes //////////////////////////////////
//============================= Static Members ==============================
    const String cls_Axes::Name = "Axes";
    bool cls_Axes::CenterLabels = false;
    TColor cls_Axes::DefaultColor = TColor(0);

//============================= Public methods ==============================

//---------------------------------------------------------------------------
cls_Axes::cls_Axes (class cls_Frame* f) : cls_FrameObject(f)
{ // Constructor

// . . . Default values
    // Inherited
    AreaStyle = bsSolid;
    ColorMode = pmNotXor;
    LineWeight = 2;
    LineColor = DefaultColor;
    AreaColor = LineColor;
    // Specific
    i_Xlabel = i_Ylabel = "";
    i_ArrowLength = 7;
    i_ArrowWing = 3;

// . . . Other operations
}

//---------------------------------------------------------------------------
cls_Axes::~cls_Axes ()
{ // Destructor
}

//---------------------------------------------------------------------------
// Create the edit properties interface
cls_PropertiesInterface* cls_Axes::CreateSpecInterface(class TWinControl* p)
{
    return new cls_AxesEdit(p,this);
}

//---------------------------------------------------------------------------
// Get a enum type (deprecated)
nms_MPlotClasses::en_FoilObjectType cls_Axes::GetType()
{
    return nms_MPlotClasses::OB_AXES;
}


//---------------------------------------------------------------------------
void __fastcall cls_Axes::Draw(class cls_Foil* Foil)
{ // Render on parent's Canvas

// . . . Local variables
	static TPoint A[3]; // Arrow vertexes

// . . . Rescale lengths according to Foil
    // Arrow dimensions
    ArrowLength = mat::min(int(15*Foil->PointsPerPix),(95*mat::min(Owner->MarginX,Owner->MarginY) )/100);
    ArrowWing = 0.3 * ArrowLength;

// . . . Origin position
    O = Owner->O;

// . . . Axes lines
    // . . . X axis ( if visible )
	if ( Owner->IsXaxisVisible() )
	   {
	    Canvas->MoveTo(Owner->Left,O.y);
	    Canvas->LineTo(Owner->right,O.y);
	    // arrow
        A[0] = Point(Owner->right,O.y+ArrowWing);
        A[1] = Point(Owner->right+ArrowLength,O.y);
        A[2] = Point(Owner->right,O.y-ArrowWing);
        Canvas->Polygon(A, 2);
	   }

    // . . . Y axis ( if visible )
	if ( Owner->IsYaxisVisible() )
	   {
	    Canvas->MoveTo(O.x,Owner->Bottom);
	    Canvas->LineTo(O.x,Owner->top);
	    // arrow
	    A[0] = Point(O.x-ArrowWing,Owner->top);
	    A[1] = Point(O.x,Owner->top-ArrowLength);
        A[2] = Point(O.x+ArrowWing,Owner->top);
	    Canvas->Polygon(A, 2);
	   }

// . . . Labels
    Canvas->Brush->Style = bsClear;
    // Limit labels into margins
    if (Font->Size >= Owner->Margin) Canvas->Font->Size = Owner->Margin;
    // . . . X label
    cls_Track* CurrentTrack = Owner->CurrentTrack;
    String Txt = ( Xlabel.IsEmpty() && CurrentTrack ) ? CurrentTrack->XLabel() : Xlabel;
    int tmp = (CenterLabels) ? Owner->Left+(Owner->Width - Canvas->TextWidth(Txt))/2 : Owner->right - Canvas->TextWidth(Txt);
    Canvas->TextOut(tmp,Owner->Bottom - Canvas->TextHeight(Txt) - 3, Txt);
    // . . . Y label
    Txt = ( Ylabel.IsEmpty() && CurrentTrack ) ? CurrentTrack->YLabel() : Ylabel;
    tmp = (CenterLabels) ? Owner->Bottom-(Owner->Height - Canvas->TextWidth(Txt))/2 : Owner->top + Canvas->TextWidth(Txt);
    mat::TextOutEx(Canvas,Owner->Left,tmp,Txt,90);
}

//---------------------------------------------------------------------------
bool __fastcall cls_Axes::IsOver(const TPoint& P) const
{ // Tell if a Canvas point is over

    O = Owner->O;
    return (::abs(P.x - O.x)<LineWeight) || (::abs(P.y - O.y)<LineWeight);
}

//---------------------------------------------------------------------------


////////////////////////////////// cls_Label ////////////////////////////////
//============================= Static Members ==============================
    const String cls_Label::Name = "Label";
    const String cls_Labels::Name = "Labels";

//============================= Public methods ==============================
//---------------------------------------------------------------------------
cls_Label::cls_Label (class cls_Frame* f) : cls_FrameObject(f)
{ // Constructor

// . . . Default values
    // Inherited
    ColorMode = pmCopy;
    AreaStyle = bsClear;
    // Specific
    i_Text = "";
    i_x = i_y = 0;
    i_LabelRect = TRect(0,0,0,0);
    i_ShowCoords = false;
}

//---------------------------------------------------------------------------
// Create the edit properties interface
cls_PropertiesInterface* cls_Label::CreateSpecInterface(class TWinControl* p)
{
    return new cls_LabelEdit(p,this);
}

//---------------------------------------------------------------------------
// Get a enum type (deprecated)
nms_MPlotClasses::en_FoilObjectType cls_Label::GetType()
{
    return nms_MPlotClasses::OB_LABEL;
}

//---------------------------------------------------------------------------
void __fastcall cls_Label::Draw(class cls_Foil* Foil)
{ // Render on parent's Canvas

// . . . Update containing rectangle (here because depends on current Canvas!)
    int X = Owner->Xpix(i_x);
    int Y = Owner->Ypix(i_y);

// . . . Draw
    if ( i_ShowCoords )
         {
          //String Coords = String("(") + Owner->XToString(i_x) + String("(") + Owner->YToString(i_y) + String(")");
          // Owner->FormatPoint(i_y,i_x);
          String Coords = String(" (") + Owner->Owner->ToString(i_x) + String(";") + Owner->Owner->ToString(i_y) + String(")");
          int r = Canvas->Font->Size; // 2.0 * Foil->PointsPerPix;
          int d = 2*r;
          i_LabelRect = TRect( X, Y, X+Canvas->TextWidth(i_Text), Y+2*Canvas->TextHeight(i_Text));
          Canvas->Ellipse(i_LabelRect.left-r,i_LabelRect.top-r,i_LabelRect.left+r,i_LabelRect.top+r);
          Canvas->MoveTo(i_LabelRect.left,i_LabelRect.top-d);
          Canvas->LineTo(i_LabelRect.left,i_LabelRect.top+d);
          Canvas->MoveTo(i_LabelRect.left-d,i_LabelRect.top);
          Canvas->LineTo(i_LabelRect.left+d,i_LabelRect.top);
          Canvas->TextOut(i_LabelRect.left,i_LabelRect.top,Coords);
          Canvas->TextOut(i_LabelRect.left,i_LabelRect.top + i_LabelRect.Height()/2,i_Text);
         }
    else {
          i_LabelRect = TRect( X, Y, X+Canvas->TextWidth(i_Text), Y+Canvas->TextHeight(i_Text));
          Canvas->TextOut(i_LabelRect.left,i_LabelRect.top,i_Text);
         }
}

//============================= Private methods =============================



///////////////////////////////// cls_Marker ////////////////////////////////
//============================= Static Members ==============================
    const String cls_Marker::Name = "Marker";
    const String cls_Markers::Name = "Markers";

    int cls_Marker::DefaultLineWeight = 1;
    TColor cls_Marker::DefaultLineColor = clRed;
    TPenStyle cls_Marker::DefaultLineStyle = psDash;


//============================= Public methods ==============================

//---------------------------------------------------------------------------
cls_Marker::cls_Marker (class cls_Frame* f) : cls_FrameObject(f)
{ // Constructor

// . . . Default values
    // Inherited
    ColorMode = pmCopy;
    LineWeight = DefaultLineWeight;
    LineColor = DefaultLineColor;
    LineStyle = DefaultLineStyle;
    AreaStyle = bsClear;
    // Specific
    i_Deferenced = false;
    i_Value.Value = 0;
    i_Vertical = false;
    i_Prefix = "";
    i_LabelRect = TRect(0,0,0,0);

// . . . Other operations
}

//---------------------------------------------------------------------------
// Create the edit properties interface
cls_PropertiesInterface* cls_Marker::CreateSpecInterface(class TWinControl* p)
{
    return new cls_MarkerEdit(p,this);
}

//---------------------------------------------------------------------------
// Get a enum type (deprecated)
nms_MPlotClasses::en_FoilObjectType cls_Marker::GetType()
{
    return nms_MPlotClasses::OB_MARKER;
}

//---------------------------------------------------------------------------
void __fastcall cls_Marker::Draw(class cls_Foil* Foil)
{ // Render on parent's Canvas

    if ( mat::isNaN(Value) ) return;

    i_Label = i_Prefix + Owner->Owner->ToString(Value);
	if ( i_Vertical )
         { // Vertical line
          // Pixel position
          Q = Owner->Xpix(Value);
          if ( Owner->IsOverX(Q) )
             {
              Canvas->MoveTo(Q,Owner->bottom);
              Canvas->LineTo(Q,Owner->top);

              //i_Label = i_Prefix + Owner->XToString(Value);
              i_LabelRect = TRect( Q,
                                   Owner->top,
                                   Q + Canvas->TextWidth(i_Label),
                                   Owner->top + Canvas->TextHeight(i_Label));
              Canvas->TextOut(i_LabelRect.Left,i_LabelRect.Top,i_Label);
             }
         }
    else { // Horizontal line
          // Pixel position
          Q = Owner->Ypix(Value);
          if ( Owner->IsOverY(Q) )
             {
              Canvas->MoveTo(Owner->left,Q);
              Canvas->LineTo(Owner->right,Q);

              //i_Label = i_Prefix + Owner->YToString(Value);
              i_LabelRect = TRect(Owner->right - Canvas->TextWidth(i_Label),
                                  Q,
                                  Owner->right,
                                  Q + Canvas->TextHeight(i_Label));
              Canvas->TextOut(i_LabelRect.Left,i_LabelRect.Top,i_Label);
             }
         }
}

//---------------------------------------------------------------------------
bool __fastcall cls_Marker::IsOver(const TPoint& P) const
{ // Tell if a Canvas point is over

    return mat::IsInRect(P,i_LabelRect) || // On label
          (( i_Vertical ) ? (::abs(P.x - Q)<LineWeight) : (::abs(P.y - Q)<LineWeight)); // On line
}

//============================= Private methods =============================




///////////////////////////////// cls_Grid //////////////////////////////////

//============================= Static Members ==============================
    const String cls_Grid::Name = "Grid";
    String cls_Grid::DefaultFont = "Microsoft Sans Serif";
    TPenStyle cls_Grid::DefaultLinesStyle = psDot;
    TColor cls_Grid::DefaultColor = clBlue;
    String cls_Grid::DefaultVGridDivs = "1";
    String cls_Grid::DefaultHGridDivs = "1";
    int cls_Grid::DefaultVGridDist = 100;
    int cls_Grid::DefaultHGridDist = 100;


//============================= Public methods ==============================

//---------------------------------------------------------------------------
cls_Grid::cls_Grid (class cls_Frame* f) : cls_FrameObject(f)
{ // Constructor

// . . . Default values
    // Style
    AreaStyle = bsClear;
    ColorMode = pmCopy;
    LineWeight = 1;

// . . . Retrieve grid divisions
    VGridDivs = DefaultVGridDivs;
    HGridDivs = DefaultHGridDivs;
    VGridDist = DefaultVGridDist;
    HGridDist = DefaultHGridDist;

// . . . Other operations
    Visible = true; // This resets grid line's density
}

//---------------------------------------------------------------------------
// Create the edit properties interface
cls_PropertiesInterface* cls_Grid::CreateSpecInterface(class TWinControl* p)
{
    return new cls_GridEdit(p,this);
}

//---------------------------------------------------------------------------
// Get a enum type (deprecated)
nms_MPlotClasses::en_FoilObjectType cls_Grid::GetType()
{
    return nms_MPlotClasses::OB_GRID;
}

//---------------------------------------------------------------------------
void __fastcall cls_Grid::Draw(class cls_Foil* Foil)
{ // Render on parent's Canvas

// . . . Some local auxiliary variables
    double z,zmax; // line coordinates [phys]
    double Delta; // Current increment
    int Z; // current line coordinate [Canvas pix]
    int O; // Position of axes [Canvas pix]
    String lbl; // Number labels
    int n; // Iteration number

// . . . Rescale lengths according to Foil
    //RecalculateMargins(Foil->PointsPerPix);
    LabelSep = 3 * Foil->PointsPerPix;

// . . . Vertical lines
    zmax = Owner->xmax; // maximum line phys coordinate
    O = Owner->O.x; // Position of axes
    n = 0;

    if ( Owner->XLogarithmic )
         {
          Delta = std::pow10(std::floor(std::log10(Owner->xmin)));
          z = Delta * std::ceil(Owner->xmin/Delta); // first line phys coordinate
         }
    else {
          Delta = Vlines.Delta;
          z = Delta * std::ceil(Owner->xmin/Delta); // first line phys coordinate
         }

    while ( z <= zmax && ++n < Owner->width )
          {
           Z = Owner->Xpix(z);
           if ( Z != O ) // Not over y axys
              {
                Owner->DrawVLine(Canvas,Z);
                lbl = Owner->XToString(z); // TODO 2: Number label
                Canvas->TextOut( Z-(Canvas->TextWidth(lbl)/2),Owner->bottom+LabelSep,lbl);
              }

           // Get next line coordinate
           if ( Owner->XLogarithmic && (z/Delta) > 9 ) Delta *= 10;
           z += Delta;
          }

// . . . Horizontal lines
    zmax = Owner->ymax; // maximum line phys coordinate
    O = Owner->O.y; // Position of axes
    n = 0;
    int TextHeight2 = Canvas->TextHeight("1")/2; // to center labels

    if ( Owner->YLogarithmic )
         {
          Delta = std::pow10(std::floor(std::log10(Owner->ymin)));
		  z = Delta * std::ceil(Owner->ymin/Delta); // first line phys coordinate
         }
    else {
          Delta = Hlines.Delta;
          z = Delta * std::ceil(Owner->ymin/Delta); // first line phys coordinate
         }

    while ( z <= zmax && ++n < Owner->height )
          {
           Z = Owner->Ypix(z);
           if ( Z != O ) // Not over y axys
              {
               Owner->DrawHLine(Canvas,Z);
               lbl = Owner->YToString(z); // TODO 2: Number label
               Canvas->TextOut(Owner->left-Canvas->TextWidth(lbl)-LabelSep,Z-TextHeight2,lbl);
              }

           // Get next line coordinate
           if ( Owner->YLogarithmic && (z/Delta) > 9) Delta *= 10;
           z += Delta;
          }

    /* #ifdef FRM_DEBUG
      Canvas->Font->Assign(Owner->Font);
      Canvas->Brush->Style = bsSolid;
      Canvas->TextOut(Owner->left+100,Owner->top+100,"V Range: " + String(Owner->xrange));
      Canvas->TextOut(Owner->left+100,Owner->top+120,"V Pixels/Delta: " + String(Vlines.Pixels) + " / " + String(Vlines.Delta));
      Canvas->TextOut(Owner->left+100,Owner->top+140,"H Range: " + String(Owner->yrange));
      Canvas->TextOut(Owner->left+100,Owner->top+160,"H Pixels/Delta: " + String(Hlines.Pixels) + " / " + String(Hlines.Delta));
    #endif */
}

//---------------------------------------------------------------------------
void cls_Grid::IncrementVOffset (const int i)
{ // Increment offset (thickness) of vertical lines
    // Limit offset by screen visualization
    if ( i < 0 )
         { // A request to thinning
          if ( Vlines.Delta > (2*Font->Size*Owner->PixelSizeX) ) Vlines.Offset += i;
         }
    else { // A request to thickening
          if ( Vlines.Delta < Owner->xrange ) Vlines.Offset += i;
         }
}

//---------------------------------------------------------------------------
void cls_Grid::IncrementHOffset (const int i)
{ // Increment offset (thickness) of horizontal lines
    // Limit offset by screen visualization
    if ( i < 0 )
         { // A request to thinning
          if ( Hlines.Delta > (2*Font->Size*Owner->PixelSizeY) ) Hlines.Offset += i;
         }
    else { // A request to thickening
          if ( Hlines.Delta < Owner->yrange ) Hlines.Offset += i;
         }
}

//---------------------------------------------------------------------------
void cls_Grid::IncrementOffset (const int i)
{ // Increment offset on grid thickness
    // Limit offset by screen visualization
    if ( i < 0 )
         { // A request to thinning
          if ( Vlines.Delta > (2*Font->Size*Owner->PixelSizeX) ) Vlines.Offset += i;
          if ( Hlines.Delta > (2*Font->Size*Owner->PixelSizeY) ) Hlines.Offset += i;
         }
    else { // A request to thickening
          if ( Vlines.Delta < Owner->xrange ) Vlines.Offset += i;
          if ( Hlines.Delta < Owner->yrange ) Hlines.Offset += i;
         }
}

//---------------------------------------------------------------------------
void cls_Grid::RecalculateVlines()
{ // Calculate vertical grid increment

// . . . Rescale lengths according to Foil
    int Points = Owner->Owner->CurrentFoil->PointsPerPix * double(Vlines.Pixels);

    Vlines.Calculate( Owner->DXphy(Points), Owner->XLogarithmic );
}

//---------------------------------------------------------------------------
void cls_Grid::RecalculateHlines()
{ // Calculate horizontal grid increment

// . . . Rescale lengths according to Foil
    int Points = Owner->Owner->CurrentFoil->PointsPerPix * double(Hlines.Pixels);

    Hlines.Calculate( Owner->DYphy(Points), Owner->YLogarithmic );
}


//============================= Private methods =============================




///////////////////////////////// cls_Legend /////////////////////////////////

//============================= Static Members ==============================
    const String cls_Legend::Name = "Legend";
    int cls_Legend::DefaultSpacing = 10;
    int cls_Legend::DefaultStrokeLength = 30;
    TColor cls_Legend::DefaultColor = clWhite;

//============================= Public methods ==============================

//---------------------------------------------------------------------------
cls_Legend::cls_Legend (class cls_Frame* f) : cls_FrameDraggableObject(f)
{ // Constructor

// . . . Default values
    i_StrokeLength = DefaultStrokeLength; // Length of track stroke

// . . . Other operations
    Visible = true; // This resets legend's position
}

//---------------------------------------------------------------------------
// Create the edit properties interface
cls_PropertiesInterface* cls_Legend::CreateSpecInterface(class TWinControl* p)
{
    return new cls_LegendEdit(p,this);
}

//---------------------------------------------------------------------------
// Get a enum type (deprecated)
nms_MPlotClasses::en_FoilObjectType cls_Legend::GetType()
{
    return nms_MPlotClasses::OB_LEGEND;
}


//---------------------------------------------------------------------------
void __fastcall cls_Legend::Draw(class cls_Foil* Foil)
{ // Render on parent's Canvas

// . . . Rescale lengths according to Foil
    RecalculateMargins(Foil->PointsPerPix);
    int stroke_length = double(StrokeLength) * Foil->PointsPerPix;

// . . . Legend size calculation
    //Canvas->Font->Name = "Arial";
    Canvas->Font->Size = 6;
    i_LineHeight = Canvas->TextHeight("X") + MarginY;
    int stroke_offset = i_LineHeight/2;
    Width = 4*MarginX + stroke_length + Canvas->TextWidth( Owner->LongestTrackName );
    Height = Owner->TracksCount()*i_LineHeight + 2*MarginY + 1;

// . . . Draw
    // Draw a background rectangle
    //if ( Owner->Empty() ) return;
    Canvas->Rectangle( BoundsRect );
    TPoint P1, P2;
    cls_Frame::typ_TracksIt tr = Owner->tracks.begin();
    for ( int i=0; tr != Owner->tracks.end(); ++i,++tr )
        {
         P1 = TPoint(Left+MarginX, Top+MarginY+stroke_offset+i*i_LineHeight);
         P2 = TPoint(P1.x+stroke_length, P1.y);
         (*tr)->Stroke(Foil,P1,P2);
         if ( tr == Owner->CurrentTrackIt ) Canvas->Font->Style = Canvas->Font->Style << fsUnderline;
         Canvas->Brush->Style = bsClear;
         Canvas->TextOut(P2.x+2*MarginX,P2.y-(2*stroke_offset)/3,(*tr)->Name);
         Canvas->Font->Style = Canvas->Font->Style >> fsUnderline;
        }
}

//---------------------------------------------------------------------------
void cls_Legend::SelectTrack(const TPoint& P)
{ // Determine and track from position. Geometry assumption: ladder

    Owner->SelectTrackByPos( (P.y-top)/i_LineHeight );
}

//---------------------------------------------------------------------------
void __fastcall cls_Legend::Reset()
{ // Reset position

    // TODO 5: Legend default position option
    // top left
    TopRatio = Owner->Height ? 0.5 * float(Owner->MarginY)/(Owner->Height) : 0.1;
    LeftRatio = Owner->Width ? 1.2 * float(Owner->MarginX)/(Owner->Width) : 0.1;
    // Default dimensions
    Width = Owner->Width;
    Height = Owner->Height;

    Owner->CheckInvariant(); // Update LongestTrackName
}



