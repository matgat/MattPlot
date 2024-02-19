#include <vcl.h>
#include "unt_Log.h"
#include "unt_MatCommon.h"
#pragma hdrstop
//---------------------------------------------------------------------------
#include <ComCtrls.hpp> // 'TTreeView'
#include <FileCtrl.hpp> // 'ForceDirectories' and other
// #include <algorithm> // std::swap
#include <mmsystem.h> // Win32 multimedia (PlaySound)
//#include "unt_OptionsSet.h" // Global options Opts (!)
#include "unt_TimeCounter.h" // 'cls_TimeCounter'
#include "unt_MPlotClasses.h"
//#include "unt_Frames.h" // in header for inlining
#include "unt_Tracks.h"
#include "res_MPlot.h"
#include "unt_MPlot.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------
// TODO 5: pen mode: vector of physpoints->coloredpixels to memorize points
// Select frame sound: Sound(ID_WAV_SELFRAME); 


//========================== File scope variables ===========================
    enum en_MeasuredTimes { MT_PAINT=0, MT_PAN=1 }; // Up to four


//============================= Static Members ==============================

        // Cursors: map in VCL indexes
        const TCursor cls_MPlot::crFrame       = TCursor(1);
        const TCursor cls_MPlot::crTrack       = TCursor(2);
        const TCursor cls_MPlot::crCancel      = TCursor(3);
        const TCursor cls_MPlot::crSelect      = TCursor(4);
        const TCursor cls_MPlot::crHandOpened  = TCursor(5);
        const TCursor cls_MPlot::crHandClosed  = TCursor(6);
        const TCursor cls_MPlot::crZoom        = TCursor(7);
        const TCursor cls_MPlot::crZoomP       = TCursor(8);
        const TCursor cls_MPlot::crZoomX       = TCursor(9);
        const TCursor cls_MPlot::crMeasure     = TCursor(10);
        const TCursor cls_MPlot::crSqCross     = TCursor(11);
        const TCursor cls_MPlot::crEdit        = TCursor(12);
        const TCursor cls_MPlot::crCrop        = TCursor(13);
        const TCursor cls_MPlot::crPen         = TCursor(14);
        const TCursor cls_MPlot::crHighlighter = TCursor(15);



//============================= Public methods ==============================


//---------------------------------------------------------------------------
__fastcall cls_MPlot::cls_MPlot(class TComponent* c, cls_Log* l)
#ifdef USE_PLOT_GRCTRL
        : TGraphicControl(c), Log(l)
#else
        : TCustomControl(c), Log(l)
#endif
{ // Constructor

// . . . Inherited
    if ( Owner->InheritsFrom(__classid(TWinControl)) ) Parent = dynamic_cast<TWinControl*>(Owner);
    else mat::MsgError("cls_MPlot must be placed in a TWinControl!");
    Parent->DoubleBuffered = true;

    // Try to link to Form MouseWheel event
    Form = dynamic_cast<TForm*>(Parent->Owner);
    if ( !Mouse->WheelPresent ) Log->Warn("cls_MPlot::","Wow, it seems you do not have mouse wheel!");
    else if ( !Form ) Log->Warn("cls_MPlot::","Unable to get Form owner, mouse wheel disabled");

    //Parent = Panel; // Safe downcast from TPanel* to TObject*
    //dynamic_cast<TControl *>(Temp)
    //if ( InheritsFrom(Panel->ClassType(), TCustomPanel().ClassType()) ) ...
    //Panel->FullRepaint = true;
    ParentFont = true;
    ParentColor = true;
    //ShowHint = true;

// . . . Inherited from TGraphicControl
    Width = 100;
    Height = 100;
    Align = alClient;
    // Stile controllo VCL  csReflector << csFramed << csReplicatable
    ControlStyle = TControlStyle() << csReplicatable << csCaptureMouse << csClickEvents << csDoubleClicks >> csNoStdEvents >> csDisplayDragImage << csOpaque;

// . . . Other operations
    // Load cursors from resource
    mat::LoadCursor(crFrame,ID_CUR_FRAME);
    mat::LoadCursor(crTrack,ID_CUR_TRACK);
    mat::LoadCursor(crCancel,ID_CUR_CANCEL);
    mat::LoadCursor(crSelect,ID_CUR_SELECT);
    mat::LoadCursor(crHandOpened,ID_CUR_HAND_OP);
    mat::LoadCursor(crHandClosed,ID_CUR_HAND_CL);
    mat::LoadCursor(crZoom,ID_CUR_ZOOM);
    mat::LoadCursor(crZoomP,ID_CUR_ZOOM_P);
    mat::LoadCursor(crZoomX,ID_CUR_ZOOM_X);
    mat::LoadCursor(crMeasure,ID_CUR_MEASURE);
    mat::LoadCursor(crSqCross,ID_CUR_SQCROSS);
    mat::LoadCursor(crEdit,ID_CUR_EDIT);
    mat::LoadCursor(crCrop,ID_CUR_CROP); 
    mat::LoadCursor(crPen,ID_CUR_PEN);
    mat::LoadCursor(crHighlighter,ID_CUR_HIGHLIGHTER);

// . . . Other attributes instances
    ScreenFoil = new cls_Foil(Canvas,BoundsRect,1.0);
    TimeCounter = new cls_TimeCounter();
    Drawer = new cls_VolatileDrawer( this );

// . . . Properties Default
    // State
    PreviousMode = i_Mode = MD_NONE;
    i_CurrentFrameIt = frames.end();
    i_CurrentFoil = ScreenFoil;
    i_Selected.FoilObject = 0;
    // Measures
    i_CurrentPoint = "";
    i_Slow = false;
    i_Repaints = 0;
    // Options
    #ifdef INCLUDE_SOUNDS
      i_Noisy = false;
    #endif
    i_SampPerPix = 1;
    i_Timeout = 10;
    i_LagThreshold = 500;
    i_LockX = i_LockY = false;
    i_FloatPrecision = 12;
    i_FloatDigits = 3;
    i_FitAtSelect = false;
    i_SuddenZoom = false;
    i_ZoomPerc = 50;
    i_DefaultPan = 10;
    i_CoastingTau = 1;
    i_ContinuousPan = 1;
    // Styles: Plot
    i_PixMargin = 10;
    i_NoBackground = false;
    i_MarkCurrentFrame = true;
    // Keys
    i_keyPanToggle = ::ShortCut(VK_SHIFT,TShiftState());
    i_keyZoomToggle = ::ShortCut(VK_CONTROL,TShiftState());
    i_keyReset = ::ShortCut(VK_SPACE,TShiftState());
    i_keyDo = ::ShortCut(VK_RETURN,TShiftState());
    i_keyCycle = ::ShortCut(VK_TAB,TShiftState());
    i_keyPrior = ::ShortCut(VK_PRIOR,TShiftState());
    i_keyNext = ::ShortCut(VK_NEXT,TShiftState());
    i_keyMinus = ::ShortCut(VK_SUBTRACT,TShiftState());
    i_keyPlus = ::ShortCut(VK_ADD,TShiftState());
    i_keyStart = ::ShortCut(VK_HOME,TShiftState());
    i_keyEnd = ::ShortCut(VK_END,TShiftState());
    i_keyUp = ::ShortCut(VK_UP,TShiftState());
    i_keyDown = ::ShortCut(VK_DOWN,TShiftState());
    i_keyLeft = ::ShortCut(VK_LEFT,TShiftState());
    i_keyRight = ::ShortCut(VK_RIGHT,TShiftState());
    // Events
    i_OnKeyDown = 0;
    i_OnPaint = 0;
    i_OnUpdate = 0;
    i_OnChange = 0;
    i_OnProgress = 0;
    i_OnProperties = 0;
    i_OnPopupMenu = 0;

// . . . Auxiliary attributes Default
    // . . . State
    IsDragDown = IsMoving = IsModeToggled = false;
    PanStatus = PS_NONE;
    IsOverFrame = IsOverCurrFrame = IsCurrFrameChanged = false; // Maintained by 'CheckCurrentFrame'
    // Point quantities
    P_pre = P_cur = P_clk = TPoint(0,0);
    p.frm = 0;
    p.trk.y = p.trk.x = p.trk.dy = p.trk.dx = p.trk.dydx = 0;
    // Key handling
    KeysPressed = LastKeysPressed = 0;
    IsWaitingKeyUp = false;
    // Other
    VpanX = VpanY = 0;
    i_Margin = 0;

// . . . Other
    // Coasting pan timer
    timPan = new TTimer(this);
    timPan->Enabled = false;
    timPan->Interval = 100;
    timPan->OnTimer = PhysicPan;

// . . . Finally
    Mode = MD_SELECT; // This sets mouse/key events
}

//---------------------------------------------------------------------------
__fastcall cls_MPlot::~cls_MPlot()
{ // Destructor

    ClearFrames(); // Clear contained frames

    // Free directly-allocated objects
    delete timPan;
    delete TimeCounter;
    delete Drawer;
    delete ScreenFoil;
    //delete frmOutputText;
}



//----------------------------- Drawing services ----------------------------

//---------------------------------------------------------------------------
void __fastcall cls_MPlot::Paint()
{ // Draw in the current foil

// . . . First operations
    TimeCounter->StartTime(MT_PAINT);
    // Reset state
    Drawer->Reset();

// . . . Manage foil resize
    if ( i_CurrentFoil != ScreenFoil )
         {// Printing on an external Canvas
          // Force next time rearrange frames
          ScreenFoil->BoundsRect = TRect(0,0,0,0);
          // . . . Rescale Plot lengths according to Foil (CurrentFoil->PixelsPerInch/ScreenFoil->PixelsPerInch)
          i_Margin = PixMargin * i_CurrentFoil->PointsPerPix;
          PlaceFrames(); // Rearrange frames
         }
    else {// Painting on screen
          // . . . Restore Plot lengths for screen
          i_Margin = PixMargin;
          // Follow plot size if form resized
          if ( i_CurrentFoil->BoundsRect != BoundsRect )
             {
              i_CurrentFoil->BoundsRect = BoundsRect; // Follow size
              PlaceFrames(); // Rearrange frames
             }
         }

// . . . Contract
    //if (i_CurrentFoil->Width<1 || i_CurrentFoil->Height<1) return;
    ASSERTING((i_CurrentFoil->Width>0 || i_CurrentFoil->Height>0),"Foil has size zero")

// . . . Background
    //CurrentFoil->Canvas->Brush->Style = bsSolid;
    //CurrentFoil->Canvas->Brush->Color = Color;
    //CurrentFoil->Canvas->FillRect(CurrentFoil->BoundsRect);

// . . . Paint each frame
    double rtime = 0;
    for ( fr=frames.begin(); fr!=frames.end(); ++fr )
        {
         // Paint frame if not exceeding Timeout
         if ( rtime < Timeout )
              {
               // TODO: do frame remove request here
               (*fr)->Paint(i_CurrentFoil);
               rtime += TimeCounter->GetTime(MT_PAINT);  // [s]
              }
         else { // Too much time, stop plotting
               Log->Warn("Forcing draw termination, too much time!");
               break;
              }
        }

// . . . Debug
    #ifdef PLT_DEBUG
      //CurrentFoil->Canvas->Brush->Style = bsSolid;
      //CurrentFoil->Canvas->Pen->Mode = pmCopy;
      //CurrentFoil->Canvas->Pen->Style = psSolid;
      //CurrentFoil->Canvas->Pen->Width = 3;
      //String l ="| "; for (unsigned int i=0; i<Layout.size(); i++) l+= String(Layout[i]) + " | ";
      //CurrentFoil->Canvas->TextOut(1,1," Layout: "+l);
    #endif

// . . . Final operations
    // . . . Update refresh time
    RefreshTime = cls_TimeQuantity(rtime); // cls_TimeQuantity(TimeCounter->GetTime(MT_PAINT));
    // . . . Detect slow refresh
    Slow = (1E3*RefreshTime) > LagThreshold;
    // Increment repaints counters
    ++i_Repaints;
    // . . . Put plot hint?
    //Hint = RefreshTime;

    // . . . Restore normal Foil
    i_CurrentFoil = ScreenFoil;

    // . . . Just painted
    if ( OnPaint ) OnPaint();
}




//----------------------------- Viewing services ----------------------------

//---------------------------------------------------------------------------
void cls_MPlot::ResetOrigin()
{ // Reset axes origin (selected frame)

    #ifdef INCLUDE_SOUNDS
      Sound(ID_WAV_RESET);
    #endif

    if ( CurrentFrame )
       {
        (*CurrentFrameIt)->ResetOrigin();
        if ( LockX ) for ( fr=frames.begin(); fr!=frames.end(); ++fr ) if (fr!=CurrentFrameIt) (*fr)->ResetOriginX();
        if ( LockY ) for ( fr=frames.begin(); fr!=frames.end(); ++fr ) if (fr!=CurrentFrameIt) (*fr)->ResetOriginY();
       }
    // Finally
    Invalidate();
}


//---------------------------------------------------------------------------
void cls_MPlot::ToggleLogScaleX()
{ // Toggle linear/logarithmic scale for x axis

    if ( LockX )
       {
        static bool All_XLogarithmic; All_XLogarithmic = (CurrentFrame) ? !((*CurrentFrameIt)->XLogarithmic) : false;
        for ( fr=frames.begin(); fr!=frames.end(); ++fr ) (*fr)->XLogarithmic = All_XLogarithmic;
       }
    else if ( CurrentFrame ) (*CurrentFrameIt)->XLogarithmic = !((*CurrentFrameIt)->XLogarithmic);

    // Finally
    Invalidate();
}

//---------------------------------------------------------------------------
void cls_MPlot::ToggleLogScaleY()
{ // Toggle linear/logarithmic scale for y axis

    if ( LockY )
       {
        static bool All_YLogarithmic; All_YLogarithmic = (CurrentFrame) ? !((*CurrentFrameIt)->YLogarithmic) : false;
        for ( fr=frames.begin(); fr!=frames.end(); ++fr ) (*fr)->YLogarithmic = All_YLogarithmic;
       }
    else if ( CurrentFrame ) (*CurrentFrameIt)->YLogarithmic = !((*CurrentFrameIt)->YLogarithmic);

    // Finally
    Invalidate();
}


//---------------------------------------------------------------------------
void cls_MPlot::ToggleLegend()
{ // Toggle legend

    if ( LockX || LockY )
         {
          static bool DrawLegends; DrawLegends = (CurrentFrame) ? !((*CurrentFrameIt)->Legend->Visible) : false;
          for ( fr=frames.begin(); fr!=frames.end(); ++fr ) (*fr)->Legend->Visible = DrawLegends;
         }
    else if ( CurrentFrame ) (*CurrentFrameIt)->Legend->Visible = !((*CurrentFrameIt)->Legend->Visible);

    // Finally
    Invalidate();
}


//---------------------------------------------------------------------------
void cls_MPlot::ToggleGrid()
{ // Toggle grid

    if ( LockX || LockY )
         {
          static bool DrawGrids; DrawGrids = (CurrentFrame) ? !((*CurrentFrameIt)->Grid->Visible) : false;
          for ( fr=frames.begin(); fr!=frames.end(); ++fr ) (*fr)->Grid->Visible = DrawGrids;
         }
    else if ( CurrentFrame ) (*CurrentFrameIt)->Grid->Visible = !((*CurrentFrameIt)->Grid->Visible);

    // Finally
    Invalidate();
}


//---------------------------------------------------------------------------
void cls_MPlot::ExpandGrid( const int D )
{ // Modifying grid lines distance, use a negative value to shrink (selected frame)

    if ( LockX || LockY )
         {
          for ( fr=frames.begin(); fr!=frames.end(); ++fr )
              {
               (*fr)->Grid->IncrementOffset(D);
              }
         }
    else if ( CurrentFrame )
         {
          (*CurrentFrameIt)->Grid->IncrementOffset(D);
         }

    // Finally
    Invalidate();
}

//---------------------------------------------------------------------------
inline void cls_MPlot::Pan (const int DX, const int DY)
{ // Perform a pan operation given displacement

    if ( CurrentFrame )
       {
        (*CurrentFrameIt)->Pan(DX,DY);
        if ( LockX ) for ( fr=frames.begin(); fr!=frames.end(); ++fr ) if (fr!=CurrentFrameIt) (*fr)->Pan(DX,0);
        if ( LockY ) for ( fr=frames.begin(); fr!=frames.end(); ++fr ) if (fr!=CurrentFrameIt) (*fr)->Pan(0,DY);
       }

    // . . . Finally
    if ( OnUpdate ) OnUpdate();
    Invalidate();
}

//---------------------------------------------------------------------------
void cls_MPlot::DefaultZoom ( const bool in, const bool x_only, const bool y_only )
{ // Perform a default zoom operation: CurrentFrame might be 0

    int sign = in ? -1 : 1;

    // Choose zoom point
    TPoint ZP = TPoint(-1,-1);
    CheckCurrentFrame(P_cur,true);
    if (IsOverCurrFrame) ZP = P_cur;
    else if (CurrentFrame) ZP = (*CurrentFrameIt)->C;

    if ( ZP.x >= 0 ) DefaultZoom(*CurrentFrameIt,ZP,sign*ZoomPerc/100.0,x_only,y_only);
}

//---------------------------------------------------------------------------
void cls_MPlot::Zoom (const TRect& R)
{ // Zoom in SelRect: if here I must have clicked on a frame

    if (CurrentFrame)
       {
        #ifdef INCLUDE_SOUNDS
          Sound(ID_WAV_ZOOM);
        #endif

        // Common operation
        (*CurrentFrameIt)->Zoom( R );

        if ( LockX ) for ( fr=frames.begin(); fr!=frames.end(); ++fr ) if (fr!=CurrentFrameIt) (*fr)->AssignScaleX(*CurrentFrameIt);
        if ( LockY ) for ( fr=frames.begin(); fr!=frames.end(); ++fr ) if (fr!=CurrentFrameIt) (*fr)->AssignScaleY(*CurrentFrameIt);
       }

    // Finally
    if ( OnUpdate ) OnUpdate(); // Maintain pixel size output
    Invalidate();
}


//---------------------------------------------------------------------------
void cls_MPlot::FitScale( const bool alltracks, const bool axesalso )
{ // Adapt view according to contained tracks (current frame)

    if ( CurrentFrame )
       {
        // Feedback
        #ifdef INCLUDE_SOUNDS
          Sound(ID_WAV_RESET);
        #endif

        if (alltracks) (*CurrentFrameIt)->FitScale(axesalso);
        else (*CurrentFrameIt)->FitScale((*CurrentFrameIt)->CurrentTrack);

        if ( LockX ) for ( fr=frames.begin(); fr!=frames.end(); ++fr ) if (fr!=CurrentFrameIt) (*fr)->AssignScaleX( *CurrentFrameIt ); // (*fr)->FitScaleX( (*fr)->CurrentTrack );
        if ( LockY ) for ( fr=frames.begin(); fr!=frames.end(); ++fr ) if (fr!=CurrentFrameIt) (*fr)->AssignScaleY( *CurrentFrameIt ); // (*fr)->FitScaleY( (*fr)->CurrentTrack );
       }

    // Finally
    Invalidate();
}


//------------------------- Actions on current track ------------------------


//---------------------------------------------------------------------------
void cls_MPlot::CurrentTrack_Remove()
{ // Remove current track

    if ( CurrentFrame )
       {
        #ifdef INCLUDE_SOUNDS
         Sound(ID_WAV_DELTRACK);
        #endif
        CurrentFrame->RemoveTrack();

        // Finally
        Invalidate();
       }
}


//---------------------------------------------------------------------------
void cls_MPlot::CurrentTrack_ToggleMarkPoints()
{ // Mark or not points of clicked/selected track

    if ( CurrentTrack )
       {
        CurrentTrack->MarkPoints = !CurrentTrack->MarkPoints;

        // Finally
        Invalidate();
       }
}

//---------------------------------------------------------------------------
void cls_MPlot::CurrentTrack_ToggleStrokeLines()
{ // Stroke or not lines of clicked/selected track

    if ( CurrentTrack )
       {
        CurrentTrack->StrokeLines = !CurrentTrack->StrokeLines;

        // Finally
        Invalidate();
       }
}

//---------------------------------------------------------------------------
void cls_MPlot::CurrentTrack_ToggleFillArea()
{ // Fill or not area of clicked/selected track

    if ( CurrentTrack )
       {
        CurrentTrack->FillArea = !CurrentTrack->FillArea;

        // Finally
        Invalidate();
       }
}

//---------------------------------------------------------------------------
void cls_MPlot::CurrentTrack_ToggleDrawHistograms()
{ // Draw or not histograms of clicked/selected track

    if ( CurrentTrack )
       {
        CurrentTrack->DrawHistograms = !CurrentTrack->DrawHistograms;

        // Finally
        Invalidate();
       }
}

//---------------------------------------------------------------------------
void cls_MPlot::CurrentTrack_ToggleShowCoords()
{ // Toggle show track point coordinates

    if ( CurrentTrack )
       {
        CurrentTrack->ShowCoords = !CurrentTrack->ShowCoords;

        // Finally
        Invalidate();
       }
}



//---------------------------------------------------------------------------
void cls_MPlot::CurrentTrack_DisplayStatistics()
{ // Display statistics of clicked/selected track

    if ( CurrentFrame ) CurrentFrame->DisplayTrackStatistics();
}




//----------------------------- frames collection ---------------------------

//---------------------------------------------------------------------------
void cls_MPlot::PlaceFrames()
{ // Arrange frames position in CurrentFoil Canvas according to Layout
  // This shares geometry assumption with 'GetZoneRect' and 'GetInsertionZone'

    if ( !Layout.empty() )
       {
        // Layout vector represents canvas rows population
        unsigned int row = 0; // current row
        unsigned int col; // current column
        int CommonHeight = (CurrentFoil->Height - (Layout.size()+1)*i_Margin) / Layout.size();
        int WidthInRow;
        fr = frames.begin();
        while ( fr != frames.end() )
            {
             WidthInRow = (CurrentFoil->Width - (Layout[row]+1)*i_Margin) / Layout[row];
             col = 0;
             do  {
                  (*fr)->Height = CommonHeight;
                  (*fr)->Top = CurrentFoil->Top + i_Margin + row*(CommonHeight+i_Margin);
                  (*fr)->Width = WidthInRow;
                  (*fr)->Left = CurrentFoil->Left + i_Margin + col*(WidthInRow+i_Margin);
                  (*fr)->Row = row; // belonging row
                 }
             while ( ++fr!=frames.end() && ++col<Layout[row] );
             ++row; // next row
            }
       }

// . . . Finally
}

//---------------------------------------------------------------------------
void cls_MPlot::AssureFrame ()
{ // Assure that exists frame where put tracks, creating one only if necessary

    // Create a new frame if no frames (assigning CurrentFrame)
    if ( frames.empty() ) CreateFrame();
    // Assure a CurrentFrame
    if ( !CurrentFrame ) CurrentFrameIt = --frames.end();

// . . . Some feedback
    if ( CurrentFrame ) Log->Success(CurrentFrame->Name + " ready");
    else Log->Error("Cannot create a new frame and none is available");

// . . . Finally
    //Invalidate();
}

//---------------------------------------------------------------------------
cls_Frame* cls_MPlot::PrepareNewFrame ( const TPoint& P )
{ // Create a frame given a position in plot

// . . . Get zone
    stu_PlotZone zone = GetInsertionZone(P);
    // Decide which frame
    switch ( zone.Type )
        {
         case ZN_FRAME : // Dropped on an existing frame: no create
            CurrentFrameIt = zone.Iterator;
            break;
         case ZN_OUTSIDE : // Not dropped at all, create a Frame in deafult pos
            CreateFrame(); // This also sets CurrentFrame
            break;
         default: // Dropped outside frames
            CreateFrame( zone ); // This also sets CurrentFrame
        }

// . . . Some feedback
    if ( CurrentFrame ) Log->Success(CurrentFrame->Name + " ready");
    else Log->Error("Ehmm, no current frame: this may cause problems");

// . . . Finally
    // Reposition legends after frames resizing
    for( fr=frames.begin(); fr!=frames.end(); ++fr ) (*fr)->Legend->Reset();
    Invalidate();
    return CurrentFrame;
}

//---------------------------------------------------------------------------
void cls_MPlot::ClearFrames()
{ // Clear all frames

    // . . . Feedback
    #ifdef INCLUDE_SOUNDS
      Sound(ID_WAV_DELFRAME);
    #endif

    // . . . Frames deallocation
    for ( fr=frames.begin(); fr!=frames.end(); ++fr ) if ( (*fr) ) delete (*fr);

    // . . . FrameList/Layout update
    Layout.clear();
    frames.clear();

    // . . . Update current frame
    CurrentFrameIt = frames.end();

    // . . . Finally
    CheckInvariant();
    Invalidate();
}

//---------------------------------------------------------------------------
void cls_MPlot::MergeFrames(typ_FramesIt frameit_dst, typ_FramesIt frameit_src)
{ // Merge two frames (in first)

    // . . . Feedback
    #ifdef INCLUDE_SOUNDS
      Sound(ID_WAV_MERGE);
    #endif

    // . . . FrameList/Layout update
    (*frameit_src)->MoveTracks( *frameit_dst );
    RemoveFrame( frameit_src ); // calls 'CheckInvariant();'
    // Set destination as current frame
    CurrentFrameIt = frameit_dst;
}

//---------------------------------------------------------------------------
void cls_MPlot::SwapFrames(typ_FramesIt frameit1, typ_FramesIt frameit2)
{ // Swap two frames

    // . . . Feedback
    #ifdef INCLUDE_SOUNDS
      Sound(ID_WAV_DROPFRAME);
    #endif

    if ( frameit1!=frames.end() && frameit2!=frames.end() )
       {
        // . . . FrameList/Layout update
        std::swap(*frameit1, *frameit2);

        // . . . Finally
        CheckInvariant();
        PlaceFrames(); // Reassign frames positions
       }
}



//---------------------------------------------------------------------------
void cls_MPlot::SelectNextFrame ()
{ // Select next frame in list

    if ( frames.empty() ) CurrentFrameIt = frames.end();
    else if ( CurrentFrameIt == frames.end() ) CurrentFrameIt = frames.begin();
    else if ( ++CurrentFrameIt == frames.end() ) CurrentFrameIt = frames.begin();
}

//---------------------------------------------------------------------------
void cls_MPlot::SelectPrevFrame ()
{ // Select previous frame in list

    if ( frames.empty() ) CurrentFrameIt = frames.end();
    else if ( CurrentFrameIt == frames.begin() || CurrentFrameIt == frames.end() ) CurrentFrameIt = --frames.end(); // rbegin()
    else --CurrentFrameIt;
}

//---------------------------------------------------------------------------
void cls_MPlot::ApplyOptions()
{ // Pass to frames the style options

    // . . . Frames and FrameObjects
    for ( fr=frames.begin(); fr!=frames.end(); ++fr ) (*fr)->ApplyOwnerOptions();

    // . . . Other objects
}

//---------------------------------------------------------------------------
void cls_MPlot::PaintTo( class cls_Frame* frptr, class cls_Foil* f )
{ // Paint current frame in a foil

    i_CurrentFoil = f;
    if ( frptr )
         {
          frptr->BoundsRect = f->BoundsRect;
          frptr->Paint(f);
          PlaceFrames(); // Reassign frames positions
         }
    else {
          Paint();
         }
    i_CurrentFoil = ScreenFoil;
}




//------------------------------- Import-Export -----------------------------

//---------------------------------------------------------------------------
void cls_MPlot::FillStringList( class TStrings* sl ) const
{ // Fill a stringlist with frame names and pointers

    for ( cfr=frames.begin(); cfr!=frames.end(); ++cfr )
        {
         sl->AddObject( (*cfr)->Name, reinterpret_cast<TObject*>(*cfr) );
        }
}

//---------------------------------------------------------------------------
void cls_MPlot::FillTreeView(class TTreeView* tree, bool full)
{ // Fill plot object treeview

// . . . Preparation
    tree->Items->Clear();
    tree->Items->BeginUpdate();
    TTreeNode *nRoot, *nFrame, *nLast, *nChild;

    // . . . Root
    nRoot = tree->Items->AddObject(NULL, "Plot", &frames);
    nRoot->ImageIndex = nRoot->Level;
    nRoot->SelectedIndex = nRoot->ImageIndex;

    // . . . Frames
    if ( full )
         { // Complete tree
          for ( cfr=frames.begin(); cfr!=frames.end(); ++cfr )
              {
               nFrame = tree->Items->AddChildObject(nRoot, (*cfr)->Name, *cfr);
               nFrame->ImageIndex = nFrame->Level;
               nFrame->SelectedIndex = nFrame->ImageIndex;
               // . . . Axes
               nLast = tree->Items->AddChildObject(nFrame, cls_Axes::Name, (*cfr)->Axes);
               nLast->ImageIndex = nFrame->ImageIndex + 1;
               nLast->SelectedIndex = nLast->ImageIndex;
               // . . . Grid
               nLast = tree->Items->AddChildObject(nFrame, cls_Grid::Name, (*cfr)->Grid);
               nLast->ImageIndex = nFrame->ImageIndex + 2;
               nLast->SelectedIndex = nLast->ImageIndex;
               // . . . Legend
               nLast = tree->Items->AddChildObject(nFrame, cls_Legend::Name, (*cfr)->Legend);
               nLast->ImageIndex = nFrame->ImageIndex + 3;
               nLast->SelectedIndex = nLast->ImageIndex;
               // . . . Markers
               nLast = tree->Items->AddChildObject(nFrame, cls_Markers::Name, &((*cfr)->Markers));
               nLast->ImageIndex = nFrame->ImageIndex + 4;
               nLast->SelectedIndex = nLast->ImageIndex;
               for (cls_Markers::typ_MarkersConstIt m=(*cfr)->Markers.begin(); m!=(*cfr)->Markers.end(); ++m)
                   {
                    nChild = tree->Items->AddChildObject(nLast, (*m)->Label, *m);
                    nChild->ImageIndex = nLast->ImageIndex;
                    nChild->SelectedIndex = nChild->ImageIndex;
                   }
               // . . . Labels
               nLast = tree->Items->AddChildObject(nFrame, cls_Labels::Name, &((*cfr)->Labels));
               nLast->ImageIndex = nFrame->ImageIndex + 5;
               nLast->SelectedIndex = nLast->ImageIndex;
               for (cls_Labels::typ_LabelsConstIt l=(*cfr)->Labels.begin(); l!=(*cfr)->Labels.end(); ++l)
                   {
                    nChild = tree->Items->AddChildObject(nLast, (*l)->Text, *l);
                    nChild->ImageIndex = nLast->ImageIndex;
                    nChild->SelectedIndex = nChild->ImageIndex;
                   }
               // . . . Tracks
               nLast = tree->Items->AddChildObject(nFrame, "Tracks", &((*cfr)->tracks));
               nLast->ImageIndex = nFrame->ImageIndex + 6;
               nLast->SelectedIndex = nLast->ImageIndex;
               for (cls_Frame::typ_TracksIt t=(*cfr)->tracks.begin(); t!=(*cfr)->tracks.end(); ++t)
                   {
                    nChild = tree->Items->AddChildObject(nLast, (*t)->Name, *t);
                    nChild->ImageIndex = nLast->ImageIndex;
                    nChild->SelectedIndex = nChild->ImageIndex;
                   }
              }
         }
    else { // Only frames and tracks
          for ( cfr=frames.begin(); cfr!=frames.end(); ++cfr )
              {
               nLast = tree->Items->AddChildObject(nRoot, (*cfr)->Name, *cfr);
               nLast->ImageIndex = nLast->Level;
               nLast->SelectedIndex = nLast->ImageIndex;
               // . . . Tracks
               for (cls_Frame::typ_TracksIt t=(*cfr)->tracks.begin(); t!=(*cfr)->tracks.end(); ++t)
                   {
                    nChild = tree->Items->AddChildObject(nLast, (*t)->Name, *t);
                    nChild->ImageIndex = nLast->ImageIndex + 1;
                    nChild->SelectedIndex = nChild->ImageIndex;
                   }
              }
          tree->FullExpand();
         }
    tree->Items->EndUpdate();
}

//---------------------------------------------------------------------------
bool cls_MPlot::ToText(const String& pth, const char delm)
{ // Convert all frames to text files

    if ( frames.size() == 1 ) return frames.back()->ToText(pth, delm);
    else {
          bool res = true;
          String fname = mat::ExtractFileName(pth);
          String fext = ::ExtractFileExt(pth);
          String fdir = ::IncludeTrailingBackslash(::IncludeTrailingBackslash(::ExtractFileDir(pth)) + fname);

          if ( !::ForceDirectories(fdir) ) return false;
          for ( fr=frames.begin(); fr!=frames.end(); ++fr ) res &= (*fr)->ToText(fdir+(*fr)->Name+fext, delm);
          return res;
         }
}

//---------------------------------------------------------------------------
bool cls_MPlot::ToMfile(const String& pth)
{ // Convert all frames to text files

    if ( frames.size() == 1 ) return frames.back()->ToMfile(pth);
    else {
          bool res = true;
          String fname = mat::ExtractFileName(pth);
          String fext = ::ExtractFileExt(pth);
          String fdir = ::IncludeTrailingBackslash(::IncludeTrailingBackslash(::ExtractFileDir(pth)) + fname);

          if ( !::ForceDirectories(fdir) ) return false;
          for ( fr=frames.begin(); fr!=frames.end(); ++fr ) res &= (*fr)->ToMfile(fdir+(*fr)->Name+fext);
          return res;
         }
}



//---------------------------------------------------------------------------
bool cls_MPlot::ToSVG(const String& pth)
{ // Convert plot to Scalable Vector Graphics

    return false;
}



//------------------------------ Editing services ---------------------------

//---------------------------------------------------------------------------
void cls_MPlot::CropTracks(const TRect& R)
{ // Crop current frame tracks abscissa domain

    if ( CurrentFrame )
       {
        // Feedback
        #ifdef INCLUDE_SOUNDS
          Sound(ID_WAV_CROP);
        #endif

        Log->Entry("Cropping","Cropping all data tracks to current rectangle abscissa domain");

        mat::stu_Rect r = mat::stu_Rect((*CurrentFrameIt)->Xphy(R.left),
                                        (*CurrentFrameIt)->Yphy(R.top),
                                        (*CurrentFrameIt)->Xphy(R.right),
                                        (*CurrentFrameIt)->Yphy(R.bottom) );
        (*CurrentFrameIt)->Crop(r);

         if ( LockX ) for ( fr=frames.begin(); fr!=frames.end(); ++fr ) if (fr!=CurrentFrameIt) (*fr)->Crop(r);
       }

    // Finally
    FitScale(true,false);
}


//------------------------------- Other services ----------------------------

//---------------------------------------------------------------------------
double* __fastcall cls_MPlot::GetValuePointer (const String& valstr)
{ // Convert a string to its linked value

    // Point quantities
         if ( valstr == "y" ) return &(p.trk.y);
    else if ( valstr == "x" ) return &(p.trk.x);
    else if ( valstr == "dy" ) return &(p.trk.dy);
    else if ( valstr == "dx" ) return &(p.trk.dx);
    else if ( valstr == "dydx" ) return &(p.trk.dydx);
    // Interval quantities
    else if ( valstr == "n" ) return &(q.n);
    else if ( valstr == "Dt" ) return &(q.Dt);
    else if ( valstr == "tmin" ) return &(q.tmin);
    else if ( valstr == "tmax" ) return &(q.tmax);
    else if ( valstr == "Dy" ) return &(q.Dy);
    else if ( valstr == "DY" ) return &(q.DY);
    else if ( valstr == "ymin" ) return &(q.ymin);
    else if ( valstr == "ymax" ) return &(q.ymax);
    else if ( valstr == "DyDt" ) return &(q.DyDt);
    else if ( valstr == "Sy" ) return &(q.Sy);
    else if ( valstr == "avg" ) return &(q.avg);
    else if ( valstr == "rms" ) return &(q.rms);
    else if ( valstr == "std" ) return &(q.std);
    else if ( valstr == "std%" ) return &(q.stdp);
    else if ( valstr == "var%" ) return &(q.varp);
    #ifdef MAD
    else if ( valstr == "mad" ) return &(q.mad);
    #endif
    else return 0;
}

//---------------------------------------------------------------------------
void cls_MPlot::GetCanvasBitmap(TImage *img, cls_Frame* fr)
{ // Get Canvas bitmap image

    try  {
          TControl * ThisCtrl = dynamic_cast<TControl*>(this);
          TRect srcRect,dstRect;
          // . . . See which region
          if ( fr )
               { // Frame only
                img->Height = fr->Height;
                img->Width = fr->Width;
                dstRect = Rect(0,0,img->Width,img->Height);
                srcRect = fr->BoundsRect;
               }
          else { // Overall Plot control
                img->Height = ThisCtrl->Height;
                img->Width = ThisCtrl->Width;
                dstRect = Rect(0,0,ThisCtrl->Width,ThisCtrl->Height);
                if ( ThisCtrl->InheritsFrom(__classid(TWinControl)) ) srcRect = dstRect;
                else srcRect = ThisCtrl->BoundsRect;
               }

          img->Canvas->CopyRect(dstRect, Canvas, srcRect);
          struct {TLogPalette lPal; TPaletteEntry dummy[256];} SysPal;
          SysPal.lPal.palVersion = 0x300;
          SysPal.lPal.palNumEntries = 256;
          ::GetSystemPaletteEntries(Canvas->Handle,0,256,SysPal.lPal.palPalEntry);
          img->Picture->Bitmap->Palette = CreatePalette(&SysPal.lPal);
        }
    catch( Exception& e )
        {
         Log->Error("Unable to get canvas bitmap: " + e.Message);
        }
}


//---------------------------------------------------------------------------
void cls_MPlot::SaveBitmap(const String& path)
{ // Save Canvas as bitmap

    TImage *image2save = new TImage(0); //(TComponent*)Parent
    GetCanvasBitmap(image2save);
    image2save->Picture->SaveToFile(path);
    delete image2save;
}

//---------------------------------------------------------------------------
void cls_MPlot::Test()
{ // Arrange some frames and tracks just to test

    // Local vars
    cls_Frame* newframe;
    cls_Track* newtrack;
    cls_DataFnTrack* dattrack;

// . . . Frame 1
    newframe = PrepareNewFrame();
    if (newframe)
       {
        newframe->Title = "Square wave harmonics";
        double A = 12; // Square wave ampl
        double A0 = 4 * A / M_PI;
        double w0 = 2 * M_PI * 3;
        double dt = 0.001;
        dattrack = new cls_DataFnTrack("Harmonic 1");
        for (double t=0; t<1; t+=dt ) dattrack->AddPoint(t, A0*std::sin(w0*t));
        newframe->InsertTrack( dattrack );

        dattrack = new cls_DataFnTrack("Harmonic 2");
        for (double t=0; t<1; t+=dt ) dattrack->AddPoint(t, A0*std::sin(3*w0*t)/3);
        newframe->InsertTrack( dattrack );

        dattrack = new cls_DataFnTrack("Harmonic 3");
        for (double t=0; t<1; t+=dt ) dattrack->AddPoint(t, A0*std::sin(5*w0*t)/5);
        newframe->InsertTrack( dattrack );

        dattrack = new cls_DataFnTrack("Sum");
        dattrack->LineWeight = 3;
        for (double t=0; t<1; t+=dt ) dattrack->AddPoint(t,A0*(std::sin(w0*t)     +
                                                               std::sin(3*w0*t)/3 +
                                                               std::sin(5*w0*t)/5) );
        newframe->InsertTrack( dattrack );
        // Add some other things
        newframe->Markers.Add(newframe,A,false);
        newframe->Markers.Add(newframe,-A,false);
        newframe->Labels.Add(newframe,"Peak n=3",(1.5*M_PI)/(3*w0),-A0/3,true);
        newframe->FitScale(true);
       }

// . . . Frame 2
    newframe = PrepareNewFrame();
    if (newframe)
       {
        newframe->Title = "Three-phase with noise";
        newframe->Grid->VGridDivs = "30|60|90|120|180|360";
        newframe->Grid->IncrementVOffset(-11);
        newframe->Grid->HGridDist = 20;
        //newframe->GetNextTrackColor();

        // . . . sin a
        dattrack = new cls_DataFnTrack("V_1");
        dattrack->YQuantity.UnitLabel = "V";
        dattrack->XName = "Theta";
        dattrack->XQuantity.UnitLabel = "deg";
        for (double x=-2*180; x<6*180; ++x) dattrack->AddPoint(x,std::sin((M_PI/180)*x)+mat::GetRandomNumber(0.15,-0.15,0.0001));
        newframe->InsertTrack( dattrack );

        // . . . sin b
        dattrack = new cls_DataFnTrack("V_2");
        dattrack->XName = "Theta";
        dattrack->YQuantity.UnitLabel = "V";
        dattrack->XQuantity.UnitLabel = "deg";
        for (double x=-2*180; x<6*180; ++x) dattrack->AddPoint(x,std::sin((M_PI/180)*x+2*M_PI/3)+mat::GetRandomNumber(0.15,-0.15,0.0001));
        newframe->InsertTrack( dattrack );

        // . . . sin c
        dattrack = new cls_DataFnTrack("V_3");
        dattrack->XName = "Theta";
        dattrack->YQuantity.UnitLabel = "V";
        dattrack->XQuantity.UnitLabel = "deg";
        for (double x=-2*180; x<6*180; ++x) dattrack->AddPoint(x,std::sin((M_PI/180)*x-2*M_PI/3)+mat::GetRandomNumber(0.15,-0.15,0.0001));
        newframe->InsertTrack( dattrack );

        // Add some other things
        newframe->Markers.Add(newframe,-30,true);
        newframe->Labels.Add(newframe,"U",90,.8,false);
        newframe->Labels.Add(newframe,"V",210,.8,false);
        newframe->Labels.Add(newframe,"W",330,.8,false);
        newframe->SelectFirstTrack();
        newframe->FitScale(true);
       }
}

//---------------------------------------------------------------------------
void cls_MPlot::Test2()
{ // Arrange some frames and tracks just to test

    // Local vars
    cls_Frame* newframe;
    cls_Track* newtrack;
    cls_DataFnTrack* dattrack;

// . . . Frame 1
    newframe = PrepareNewFrame();
    if (newframe)
       {
        newframe->Title = "Rectified 50Hz three-phase";

        double A = 10;
        double w = 2 * M_PI * 50;
        double T = 0.04;
        double dt = (2*M_PI / w) / 1000;
        // . . . sin a
        dattrack = new cls_DataFnTrack("V_1");
        dattrack->YQuantity.UnitLabel = "V";
        dattrack->XName = "t";
        dattrack->XQuantity.UnitLabel = "s";
        for(double t=0; t<T; t+=dt) dattrack->AddPoint(t,A*std::sin(w*t));
        newframe->InsertTrack( dattrack );

        // . . . sin b
        dattrack = new cls_DataFnTrack("V_2");
        dattrack->XName = "t";
        dattrack->YQuantity.UnitLabel = "V";
        dattrack->XQuantity.UnitLabel = "s";
        for(double t=0; t<T; t+=dt) dattrack->AddPoint(t,A*std::sin(w*t+2*M_PI/3));
        newframe->InsertTrack( dattrack );

        // . . . sin c
        dattrack = new cls_DataFnTrack("V_3");
        dattrack->XName = "t";
        dattrack->YQuantity.UnitLabel = "V";
        dattrack->XQuantity.UnitLabel = "s";
        for(double t=0; t<T; t+=dt) dattrack->AddPoint(t,A*std::sin(w*t-2*M_PI/3));
        newframe->InsertTrack( dattrack );

        // . . . Rectified three-pase
        dattrack = new cls_DataFnTrack("r_3");
        dattrack->XName = "t";
        dattrack->YQuantity.UnitLabel = "V";
        dattrack->XQuantity.UnitLabel = "s";
        for(double t=0; t<T; t+=dt) dattrack->AddPoint(t, A*mat::max( std::fabs(std::sin(w*t)),
                                                                      std::fabs(std::sin(w*t+2*M_PI/3)),
                                                                      std::fabs(std::sin(w*t-2*M_PI/3))) );
        dattrack->LineWeight = 3;
        newframe->InsertTrack( dattrack );

        // Add some other things
        newframe->SelectFirstTrack();
        newframe->FitScale(true);
       }

// . . . Frame 2
    newframe = PrepareNewFrame();
    if (newframe)
       {
        newframe->Title = "Square waves";

        double A = 1;
        double T = 1;
        double Ttot = 4*T;
        double dt = Ttot / 1000.0;

        // . . . 25%
        dattrack = new cls_DataFnTrack("d=25%");
        //dattrack->YQuantity.UnitLabel = "V";
        dattrack->XName = "time";
        dattrack->XQuantity.UnitLabel = "s";
        for(double t=0; t<Ttot; t+=dt) dattrack->AddPoint(t,A*mat::SquareWave(t, T, 0.25));
        newframe->InsertTrack( dattrack );

        // . . . 50%
        dattrack = new cls_DataFnTrack("d=50%");
        dattrack->XName = "time";
        //dattrack->YQuantity.UnitLabel = "V";
        dattrack->XQuantity.UnitLabel = "s";
        for(double t=0; t<Ttot; t+=dt) dattrack->AddPoint(t,A*mat::SquareWave(t, T, 0.5));
        newframe->InsertTrack( dattrack );

        // . . . 75%
        dattrack = new cls_DataFnTrack("d=75%");
        dattrack->XName = "time";
        //dattrack->YQuantity.UnitLabel = "V";
        dattrack->XQuantity.UnitLabel = "s";
        for(double t=0; t<Ttot; t+=dt) dattrack->AddPoint(t,A*mat::SquareWave(t, T, 0.75));
        newframe->InsertTrack( dattrack );

        // Add some other things
        newframe->Markers.Add(newframe,T,true);
        //newframe->Labels.Add(newframe,"U",90,.8,false);
        newframe->SelectFirstTrack();
        newframe->FitScale(true);
       }

// . . . Frame 3
    newframe = PrepareNewFrame();
    if (newframe)
       {
        newframe->Title = "Sine";

        double T = 4*M_PI;
        double dt = T / 100;

        // . . . sine
        dattrack = new cls_DataFnTrack("Sine");
        for(double t=0; t<T; t+=dt) dattrack->AddPoint(t,std::sin(t));
        newframe->InsertTrack( dattrack );
        newframe->FitScale(true);
       }
}

//---------------------------------------------------------------------------
void cls_MPlot::Test_Experiments ()
{ // Another test with experimental stuff

// . . . Local vars
    cls_Frame* newframe;
    cls_Track* newtrack;
    cls_DataFnTrack* dattr;

// . . . Frame 1
    newframe = PrepareNewFrame();
    if (newframe)
       {
        newframe->Title = "Some functions";
        // A polinome
        newtrack = new cls_PolynomialTrack("x^2");
        newframe->InsertTrack( newtrack );
        // A sine
        newtrack = new cls_HarmonicTrack("sin(x)");
        newframe->InsertTrack( newtrack );
        // An exponential
        newtrack = new cls_ExponentialTrack("e^x");
        newframe->InsertTrack( newtrack );
        // An hyperbola
        newtrack = new cls_HyperbolaTrack("1/x");
        newframe->InsertTrack( newtrack );

        newframe->SelectFirstTrack();
       }

// . . . Frame 2
    newframe = PrepareNewFrame();
    if (newframe)
       {
        newframe->Title = "Points";

        // . . . Some hystograms
        newtrack = new cls_DataFnTrack("histograms");
        newtrack->LineColor = clFuchsia;
        newtrack->AreaColor = TColor(0xF7D6BF);
        newtrack->AreaStyle = bsSolid;
        newtrack->LineWeight = 4;
        newtrack->MarkPoints = false;
        newtrack->DrawHistograms = true;
        newtrack->StrokeLines = false;
        newtrack->ShowCoords = false;
        // Add some points
        dattr = dynamic_cast<cls_DataFnTrack*>(newtrack);
        ASSERTING((dattr),"invalid cast, check track classes")
        for (double d=0; d<10; ++d ) dattr->AddPoint(d,mat::GetRandomNumber(5,1,.5));
        // Finally, insert it
        newframe->InsertTrack( newtrack );

        // . . . Some lines
        newtrack = new cls_DataFnTrack("graph");
        newtrack->LineColor = clRed;
        newtrack->AreaColor = TColor(mat::Darken(newtrack->LineColor,-25));
        newtrack->LineWeight = 2;
        // Add some points
        dattr = dynamic_cast<cls_DataFnTrack*>(newtrack);
        ASSERTING((dattr),"invalid cast, check track classes")
        for (double d=0; d<10; ++d ) dattr->AddPoint(d,mat::GetRandomNumber(10,-5,1));
        // Finally, insert it
        newframe->InsertTrack( newtrack );

        // . . . Some points
        newtrack = new cls_DataFnTrack("somepoints");
        newtrack->LineColor = TColor(0xC1CD19);
        newtrack->AreaColor = TColor(mat::Darken(newtrack->LineColor,-25));
        newtrack->LineWeight = 5;
        newtrack->MarkPoints = true;
        newtrack->DrawHistograms = false;
        newtrack->StrokeLines = false;
        newtrack->ShowCoords = false;
        newtrack->PointStyle = 'o';
        newtrack->AreaStyle = bsClear;
        // Add some points
        dattr = dynamic_cast<cls_DataFnTrack*>(newtrack);
        ASSERTING((dattr),"invalid cast, check track classes")
        for (double d=0; d<10; ++d ) dattr->AddPoint(d,mat::GetRandomNumber(10,1,1));
        // Finally, insert it
        newframe->InsertTrack( newtrack );

        // . . . Finally
        newframe->SelectFirstTrack();
       }

// . . . Frame 3
    newframe = PrepareNewFrame(CurrentFrame->CR); // Put beside
    if (newframe)
       {
        newframe->Title = "Areas";
        newframe->YLogarithmic = true;

        // An hyperbola
        newtrack = new cls_HyperbolaTrack("1/x");
        newtrack->LineWeight = 2;
        newtrack->FillArea = true;
        newtrack->AreaColor = TColor(0xD6FAF0);
        // Finally, insert it
        newframe->InsertTrack( newtrack );

        // An exponential
        newtrack = new cls_ExponentialTrack("e^x");
        newtrack->LineStyle = psDashDot;
        // Finally, insert it
        newframe->InsertTrack( newtrack );

        // A polinome
        newtrack = new cls_PolynomialTrack("x^2");
        newtrack->LineStyle = psDash;
        newtrack->FillArea = true;
        newtrack->AreaColor = TColor(0xEDE5D9);
        // Finally, insert it
        newframe->InsertTrack( newtrack );

        // A sine
        newtrack = new cls_HarmonicTrack("sin(x)");
        newtrack->FillArea = true;
        newtrack->LineStyle = psDot;
        newtrack->AreaColor = TColor(0xBFC0EF);
        // Finally, insert it
        newframe->InsertTrack( newtrack );

        // Finally
        newframe->SelectFirstTrack();
       }
}

//---------------------------------------------------------------------------
void cls_MPlot::DrawRandomShapes (unsigned int N)
{ // Draw a random Shape

    static bool FirstCall = true;
    if (FirstCall) { ::randomize(); FirstCall=false; } // call once

    int w,h,x,y;
    for (unsigned int i=0; i<N; ++i)
        {
         w = ::random(Width/3);
         h = ::random(Height/3);
         x = ::random(Width - w);
         y = ::random(Height - h);

         mat::PickRandomPen(Canvas);
         mat::PickRandomBrush(Canvas);

         switch ( ::random(3) )
            {
             case 1  : Canvas->RoundRect(x, y, x+w, y+h, w/5, h/5); break;
             case 2  : Canvas->Ellipse(x, y, x+w, y+h); break;
             default : Canvas->Rectangle(x, y, x+w, y+h); break;
            }
        }
}

//---------------------------------------------------------------------------
void cls_MPlot::DrawRandomCurve (unsigned int N)
{ // Draw a random curve given a number of nodes

    static bool FirstCall = true;
    if (FirstCall) { ::randomize(); FirstCall=false; } // call once

    mat::PickRandomPen(Canvas);
    mat::PickRandomBrush(Canvas);

	TPoint* nodes = new TPoint[N];
    for (unsigned int i=0; i<N; ++i) nodes[i] = Point(::random(Width),::random(Height));
    //Canvas->Polyline(nodes,N-1);
    Canvas->PolyBezier(nodes,N-1);
    delete[] nodes;
}


//---------------------------------------------------------------------------
void cls_MPlot::TryColorGenerator()
{ // Try color generator

    AssureFrame();
    // . . . Set Canvas tools
    Canvas->Pen->Width = 30;
    Canvas->Pen->Style = psSolid;
    Canvas->Pen->Mode = pmCopy;
    // . . . Initial coordinates
    int X = CurrentFrame->left;
    int Y1 = CurrentFrame->top;
    int Y2 = CurrentFrame->bottom;
    // . . . Draw
    do {
        Canvas->MoveTo(X,Y1);
        Canvas->Pen->Color = CurrentFrame->GetNextTrackColor();
        Canvas->LineTo(X,Y2);
        X += Canvas->Pen->Width;
       }
    while ( ::abs(X) <= ::abs(CurrentFrame->right) );
}


//---------------------------------------------------------------------------



//=============================== Properties ================================


//---------------------------------------------------------------------------
void cls_MPlot::set_Mode(cls_MPlot::en_Modes mode)
{ // Set Property

// . . . Checks
    //if ( mode == i_Mode ) return; // Don't set an existing mode
    if ( IsModeToggled ) // Do nothing if mode temporarily toggled
       {
        Log->Warn("Unable to change mode because is temporarily toggled");
        return;
       }

// . . . OnExit mode operations
    if ( i_Mode == MD_SELECT )
         {
          Drawer->UnMarkZone();
         }
    else if ( i_Mode == MD_PAN )
         {
          VpanX = VpanY = 0; // Avoid coasting
          PanStatus = PS_NONE;
          Drawer->EraseDirection();
         }
    else if ( i_Mode == MD_ZOOM )
         {
          Drawer->EraseSelRect();
         }
    else if ( i_Mode == MD_MEASURE )
         {
          Drawer->EraseMeasureLines1();
          Drawer->EraseMeasureLines2();
          //frmOutputText->Hide();
         }
    // else if ( i_Mode == MD_EDIT )

// . . . OnEnter mode operations
    switch ( mode )
           {
            case MD_SELECT  : {// Select mode
                               #ifdef INCLUDE_SOUNDS
                                 Sound(ID_WAV_SELECT);
                               #endif
                               Screen->Cursor = crSelect;
                               Cursor = crSelect;
                               Screen->Cursor = crDefault;
                               OnMouseDown = OnMouseDownSelect;
                               OnMouseMove = OnMouseMoveSelect;
                               OnMouseUp = OnMouseUpSelect;
                               if (Form) Form->OnMouseWheel = OnMouseWheelSelect;
                               OnKeyDown = OnKeyDownSelect;
                              } break;
            case MD_PAN     : {// Pan mode
                               #ifdef INCLUDE_SOUNDS
                                 Sound(ID_WAV_PAN);
                               #endif
                               Screen->Cursor = crHandOpened;
                               Cursor = crHandOpened;
                               Screen->Cursor = crDefault;
                               OnMouseDown = OnMouseDownPan;
                               OnMouseMove = OnMouseMovePan;
                               OnMouseUp = OnMouseUpPan;
                               if (Form) Form->OnMouseWheel = OnMouseWheelPan;
                               OnKeyDown = OnKeyDownPan;
                              } break;
            case MD_ZOOM    : {// Zoom mode
                               #ifdef INCLUDE_SOUNDS
                                 Sound(ID_WAV_ZOOM);
                               #endif
                               Screen->Cursor = crZoom;
                               Cursor = crZoom;
                               Screen->Cursor = crDefault;
                               OnMouseDown = OnMouseDownZoom;
                               OnMouseMove = OnMouseMoveZoom;
                               OnMouseUp = OnMouseUpZoom;
                               if (Form) Form->OnMouseWheel = OnMouseWheelZoom;
                               OnKeyDown = OnKeyDownZoom;
                              } break;
            case MD_MEASURE : {// Measure mode
                               #ifdef INCLUDE_SOUNDS
                                 Sound(ID_WAV_MEASURE);
                               #endif
                               Screen->Cursor = crMeasure;
                               Cursor = crMeasure;
                               Screen->Cursor = crDefault;
                               OnMouseDown = OnMouseDownMeasure;
                               OnMouseMove = OnMouseMoveMeasure;
                               OnMouseUp = OnMouseUpMeasure;
                               //if (Form) Form->OnMouseWheel = OnMouseWheelMeasure;
                               if (Form) Form->OnMouseWheel = OnMouseWheelSelect;
                               OnKeyDown = OnKeyDownMeasure;
                              } break;
            case MD_EDIT  : {// Edit mode
                               #ifdef INCLUDE_SOUNDS
                                 Sound(ID_WAV_EDIT);
                               #endif
                               Screen->Cursor = crEdit;
                               Cursor = crEdit;
                               Screen->Cursor = crDefault;
                               OnMouseDown = OnMouseDownEdit;
                               OnMouseMove = OnMouseMoveEdit;
                               OnMouseUp = OnMouseUpEdit;
                               if (Form) Form->OnMouseWheel = OnMouseWheelEdit;
                               OnKeyDown = OnKeyDownEdit;
                              } break;
            case MD_PEN     : {// Pen mode
                               #ifdef INCLUDE_SOUNDS
                                 Sound(ID_WAV_PEN);
                               #endif
                               Screen->Cursor = crPen;
                               Cursor = crPen;
                               Screen->Cursor = crDefault;
                               OnMouseDown = OnMouseDownPen;
                               OnMouseMove = OnMouseMovePen;
                               OnMouseUp = OnMouseUpPen;
                               if (Form) Form->OnMouseWheel = OnMouseWheelPan;
                               OnKeyDown = OnKeyDownPan;
                              } break;
            default         : {// Invalid mode
                               mat::MsgError("cls_MPlot","Gassp! An invalid mode??");
                               return;
                              }
           }// end switch

// . . . Finally
    PreviousMode = i_Mode; // Keep track of previous mode
    i_Mode = mode; // Now can set
    Invalidate();
}

//---------------------------------------------------------------------------
void cls_MPlot::set_LockX (bool b)
{ // Set Sync frame axes property

    #ifdef INCLUDE_SOUNDS
      Sound(ID_WAV_SYNC);
    #endif
    i_LockX = b;
}

//---------------------------------------------------------------------------
void cls_MPlot::set_LockY (bool b)
{ // Set Sync frame axes property

    #ifdef INCLUDE_SOUNDS
      Sound(ID_WAV_SYNC);
    #endif
    i_LockY = b;
}

//---------------------------------------------------------------------------
String __fastcall cls_MPlot::get_SelectedStuff()
{ // Get a string with selected stuff

    static String mex;
    if ( CurrentFrame )
         {
          mex = (*CurrentFrameIt)->Name;
          // Append frame title also
          //if ( !(*CurrentFrameIt)->Title.IsEmpty() ) mex += String("(") + ( (*CurrentFrameIt)->Title.Length()>12  ? ((*CurrentFrameIt)->Title).SubString(1,12) + String("…") : (*CurrentFrameIt)->Title ) + String(")");
          if ( CurrentTrack ) mex = mex + "." + CurrentTrack->Name;
         }
    else mex = String("No selection");
    return mex;
}



//================================= Events ==================================


/*
    // Test key
    //TShortCut KeysWanted = ZoomToggle;
    //Word KeyWanted;
    //TShiftState ShiftWanted;
    //ShortCutToKey(KeysWanted, KeyWanted, ShiftWanted);
    //Log->SubEntry("down: " +String(KeysPressed)+" "+String(int(Key))+" "+mat::ToString(Shift));
    //Log->SubEntry("wanted: " +String(KeysWanted)+" "+String(int(KeyWanted))+" "+mat::ToString(ShiftWanted));
*/

//---------------------------------------------------------------------------
inline bool cls_MPlot::CommonMouseDown(TMouseButton Button,TShiftState Shift,int X,int Y)
{ // Common mouse down operations; returns true if there's a valid CurrentFrame

// . . . Maintain clicked point 'P_clk' and cursor position 'P_cur'
    P_clk = P_cur = TPoint(X,Y); // Last clicked point

// . . . Check frame
    CheckCurrentFrame (P_clk, true);
    // Remark: this sets 'IsOverFrame', 'IsOverCurrFrame',
    //         'IsCurrFrameChanged' and 'CurrentFrameIt' if 'true'

// . . . Common operations on CurrentFrame
    if ( IsOverCurrFrame )
       {
        // . . . Maintain point quantities subset
        // set first cursor point, held when 'IsDragDown'
        p.frm.top = (*CurrentFrameIt)->Yphy(P_cur.y);
        p.frm.left = (*CurrentFrameIt)->Xphy(P_cur.x);

        // . . . Finally
        return true;
       }
    else return false;
}

//---------------------------------------------------------------------------
inline void cls_MPlot::CommonMouseMove(TShiftState Shift, int X, int Y)
{ // Common operations before mouse move

// . . . Maintain cursor position 'P_pre','P_cur'
    P_pre = P_cur;
    P_cur = TPoint(X,Y);

// . . . Detect Out Of Bounds (except in select mode)
    if ( CurrentFrame && IsDragDown && Mode!=MD_SELECT )
       {
        if ( !mat::IsInRect(P_cur,CurrentFrame->BoundsRect) ) Mouse->CursorPos = ClientToScreen(P_cur=P_pre);
        if ( !mat::IsInRect(P_cur,BoundsRect) ) Screen->Cursor = crDefault;
       }

// . . . Check frame
    CheckCurrentFrame (P_cur, false);
    // Remark: this sets 'IsOverFrame', 'IsOverCurrFrame',
    //         'IsCurrFrameChanged' and 'CurrentFrameIt' if 'true'

// . . . Common operations on CurrentFrame
    if ( IsOverCurrFrame )
         {
          // . . . Maintain point quantities subset
          // current cursor point in physical space
          p.frm.bottom = (*CurrentFrameIt)->Yphy(P_cur.y);
          p.frm.right = (*CurrentFrameIt)->Xphy(P_cur.x);
          if ( !IsDragDown ) {p.frm.left=p.frm.right; p.frm.top=p.frm.bottom;} // first point follows second when not dragging
          //i_CurrentPoint = (*CurrentFrameIt)->YToString(p.frm.y2) + String(" ; ") + (*CurrentFrameIt)->XToString(p.frm.x2);

          i_CurrentPoint = (*CurrentFrameIt)->FormatPoint_sca(p.frm.bottom,p.frm.right);
         }
    else {
          // . . . Maintain point quantities subset
          // current cursor point in physical space
          p.frm = mat::NaN;
          i_CurrentPoint = String("NaN;NaN");
         }
}

//---------------------------------------------------------------------------
inline void cls_MPlot::CommonAfterMouseMove()
{ // Common operations after mouse move

// . . . Current point changed: feedback
    if ( OnUpdate ) OnUpdate();
}

//---------------------------------------------------------------------------
inline void cls_MPlot::CommonAfterMouseUp(TMouseButton Button,TShiftState Shift,int X,int Y)
{ // Common mouse up operations

// . . . Maintain cursor position 'P_cur'
    P_cur = TPoint(X,Y);

// . . . If here dragging is over
    IsDragDown = false;

// . . . Reset eventual drag cursor
    Screen->Cursor = TCursor(crDefault);

// . . . Maintain selected stuff
    if ( OnUpdate ) OnUpdate();
}

//---------------------------------------------------------------------------
inline bool cls_MPlot::CommonMouseWheel(TShiftState Shift, int WheelDelta, const TPoint &MousePos, bool &Handled)
{ // Common mouse wheel operations; returns true if there's a valid CurrentFrame

// . . . Maintain cursor position 'P_cur'
    P_cur = ScreenToClient(MousePos);
    Handled = true;

// . . . Check frame
    CheckCurrentFrame (P_cur, true);
    // Remark: this sets 'IsOverFrame', 'IsOverCurrFrame',
    //         'IsCurrFrameChanged' and 'CurrentFrameIt' if 'true'

// . . . Common operations on CurrentFrame
    if ( IsOverCurrFrame )
       {
        // Do something?

        // . . . Maintain current cursor point in physical space 'p_cur'
        //(*CurrentFrameIt)->ScreenToPhysSpace(P_cur,&p_cur);

        // . . . Maintain track fixed measures 'y', 'dydx' from 'p_cur'
        //CalcCurrentTrackFixedMeasures();

        // . . . Finally
        return true;
       }
    else return false;
}

//---------------------------------------------------------------------------
inline bool cls_MPlot::CommonKeyDown(WORD Key, TShiftState Shift)
{ // Returns true if key repeated

    KeysPressed = ::ShortCut(Key,Shift); // Get pressed keys
    if ( IsWaitingKeyUp && (KeysPressed==LastKeysPressed) ) return true; // Wait KeyUp or a change
    else {
          LastKeysPressed = KeysPressed; // Update last pressed keys
          IsWaitingKeyUp = true; // Lock

          // . . . Handle Plot Keys Fixed Tasks Here
          if ( Key == keyZoomToggle ) ToggleToMode(MD_ZOOM);
          else if ( Key == keyPanToggle ) ToggleToMode(MD_PAN);
          else if ( Key == keyCycle ) CycleTrack();
          else if ( Key == keyPrior ) SelectPrevTrack_Rescale();
          else if ( Key == keyNext  ) SelectNextTrack_Rescale();
          return false;
         }
}

//---------------------------------------------------------------------------
void __fastcall cls_MPlot::OnKeyUp(TObject *Sender, WORD &Key, TShiftState Shift)
{ // Common key up operations
    //TShortCut KeysReleased = ShortCut(Key,Shift);

// . . . Handle Toggle tasks (monostable states)
    if ( Key == mat::GetKey(keyZoomToggle) || Key == mat::GetKey(keyPanToggle) ) UntoggleMode();

// . . . Now unlock KeyDown handler
    IsWaitingKeyUp = false;

// . . . Maintain selected stuff
    if ( OnUpdate ) OnUpdate();
}



//---------------------------------------------------------------------------
void __fastcall cls_MPlot::OnMouseDownSelect(TObject *Sender,TMouseButton Button,TShiftState Shift,int X,int Y)
{ // OnMouseDown event handler

// . . . Local variables
    static cls_Frame::typ_TracksIt ClickedTrackIt;

// . . . Common operations
    // Select frame in any case
    if ( !CommonMouseDown(Button,Shift,X,Y) ) return;

// . . . What has been clicked? Update state
    if ( !IsOverCurrFrame )
       { // Clicked outside frames
        i_Selected.FoilObject = 0;
        return; // Nothing to do if clicked outside
       }

// . . . Clicked in a frame: what has been clicked? Update state
    // 'CurrentFrameIt' is already set by 'CheckCurrentFrame'
    #ifdef PLT_DEBUG
        Log->SubEntry("Clicked in " + CurrentFrame->Name);
    #endif

// . . . Set clicked object
    // Surely a frame
    i_Selected.FoilObject = *CurrentFrameIt;

    // . . . Now see frame subobjects
    // . . . Track
    ClickedTrackIt = (*CurrentFrameIt)->OverTrack( P_clk );
    if ( ClickedTrackIt != (*CurrentFrameIt)->tracks.end()  )
       {
        #ifdef PLT_DEBUG
            Log->SubEntry("Clicked on track " + (*ClickedTrackIt)->Name);
        #endif

        // Feedback
        #ifdef INCLUDE_SOUNDS
          Sound(ID_WAV_SELTRACK);
        #endif

        // . . . Set selected object
        i_Selected.FoilObject = *ClickedTrackIt;

        // . . . Set current track also
        (*CurrentFrameIt)->CurrentTrackIt = ClickedTrackIt;
       }

    // . . . Legend
    if ( (*CurrentFrameIt)->Legend->IsOver( P_clk ) )
       {
        #ifdef PLT_DEBUG
            Log->SubEntry("Clicked on legend");
        #endif

        // . . . Set selected object
        i_Selected.FoilObject = (*CurrentFrameIt)->Legend;
       }

    // . . . Markers
    cls_Marker* m = (*CurrentFrameIt)->Markers.IsOver( P_clk );
    if (m) i_Selected.FoilObject = m;

    // . . . Labels
    cls_Label* l = (*CurrentFrameIt)->Labels.IsOver( P_clk );
    if (l) i_Selected.FoilObject = l;

    // . . . Axes
    if ( (*CurrentFrameIt)->Axes->IsOver( P_clk ) ) i_Selected.FoilObject = (*CurrentFrameIt)->Axes;

    // . . . Grid
    //if ( (*CurrentFrameIt)->Grid->IsOver( P_clk ) ) i_Selected.FoilObject = (*CurrentFrameIt)->Grid;


// . . . Specific mouse button operations
    switch ( Button )
           {
            case mbLeft :
                {
                 // Common operations
                 IsDragDown = true; // Set this for drag operation
                 //if ( Shift.Contains(ssShift) )
                 // Open properties with double click
                 if ( Shift.Contains(ssDouble) ) if (OnProperties) OnProperties(this);

                 // Specific object operations
                 cls_Legend* lg = dynamic_cast<cls_Legend*>(i_Selected.FoilObject);
                 if (lg) {
                          lg->SelectTrack(P_clk); // select track from legend
                          lg->DragPoint(P_clk); // set drag point
                         }

                 /*
                 switch ( i_Selected.FoilObject->GetType() )
                        {
                         case mpl::OB_LEGEND : // Select track / Drag legend

                             break;
                         //case mpl::OB_GRID : ; break;
                         //case mpl::OB_AXES : ; break;
                         //case mpl::OB_MARKER : ; break;
                         //case mpl::OB_LABEL : ; break;
                         //case mpl::OB_TRACK : ; break;
                         //case mpl::OB_FRAME : ; break;
                         //case OB_NONE :
                         //default :
                        }
                 */
                } break;

            case mbRight :
                {
                 // Request context popup menu
                 if ( OnPopupMenu ) OnPopupMenu( ClientToScreen(P_clk) );
                } break;

            case mbMiddle :
                {
                 // TODO 5: do something with middle click
                } break;
           }

// . . . Finally
    Invalidate(); // View selection
    // Maintain selected stuff
    if ( OnUpdate ) OnUpdate();
}

//---------------------------------------------------------------------------
void __fastcall cls_MPlot::OnMouseMoveSelect(TObject *Sender, TShiftState Shift, int X, int Y)
{ // OnMouseMove event handler

// . . . Common operations
    CommonMouseMove(Shift, X, Y);

// . . . Specific operations

    // If Temporary switched over legend, return to previous mode
    if ( IsOverCurrFrame )
        if ( !((*CurrentFrameIt)->Legend->IsOver(P_cur)) )
            if ( UntoggleMode() ) return;

    // Selection and moving
    if ( IsDragDown && IsStartedDragging() ) // requires P_cur and P_clk
       {
        // Decide what to do according to clicked object
        //cls_Legend* lg = dynamic_cast<cls_Legend*>(i_Selected.FoilObject);
        //cls_Frame* fr = dynamic_cast<cls_Frame*>(i_Selected.FoilObject);
        //cls_Track* tr = dynamic_cast<cls_Track*>(i_Selected.FoilObject);

        switch ( i_Selected.FoilObject->GetType() )
           {
            case mpl::OB_LEGEND : // Dragging the legend
                IsMoving = true;
                i_Selected.Legend->DragPosition(P_cur);
                if ( mat::IsInRect(P_cur,BoundsRect) ) Screen->Cursor = crDefault;
                else Screen->Cursor = crCancel;
                Invalidate(); // Feedback during drag
                break;

            case mpl::OB_TRACK : // Dragging a Track
                // Set state
                IsMoving = true;
                if ( mat::IsInRect(P_cur,BoundsRect) ) Screen->Cursor = crTrack;
                else Screen->Cursor = crCancel;
                break;

            case mpl::OB_FRAME : // Dragging a frame
                // Set state
                IsMoving = true;
                // In any case unmark preceding zone
                Drawer->UnMarkZone();

                if ( mat::IsInRect(P_cur,BoundsRect) )
                     { // Inside plot
                      Screen->Cursor = crFrame;
                      Drawer->MarkZone ( GetZoneRect(P_cur) );
                     }
                else { // Outside plot
                      Screen->Cursor = crCancel;
                     }
                break;

            case mpl::OB_MARKER :
                IsMoving = true;
                if ( mat::IsInRect(P_cur,BoundsRect) )
                   {
                    Screen->Cursor = crDrag;
                    i_Selected.Marker->SetValue( i_Selected.Marker->Vertical ? (*CurrentFrameIt)->Xphy(P_cur.x) : (*CurrentFrameIt)->Yphy(P_cur.y) );
                    Invalidate(); // Feedback during drag
                   }
                else Screen->Cursor = crCancel;
                break;

            case mpl::OB_LABEL :
                IsMoving = true;
                if ( mat::IsInRect(P_cur,BoundsRect) )
                   {
                    Screen->Cursor = crDrag;
                    i_Selected.Label->x = (*CurrentFrameIt)->Xphy(P_cur.x);
                    i_Selected.Label->y = (*CurrentFrameIt)->Yphy(P_cur.y);
                    Invalidate(); // Feedback during drag
                   }
                else Screen->Cursor = crCancel;
                break;
                
            //case mpl::OB_GRID :
            //case mpl::OB_AXES :
            //default :
           } // end 'switch ( selected object type )'
       }

// . . . Common operations
    CommonAfterMouseMove();
}

//---------------------------------------------------------------------------
void __fastcall cls_MPlot::OnMouseUpSelect(TObject *Sender,TMouseButton Button,TShiftState Shift,int X,int Y)
{ // OnMouseDown event handler

// . . . Detect, if something was dragged, where it was released
    if ( IsMoving )
       {
        IsMoving = false;

        // Check where it was released
        stu_PlotZone Zone = GetInsertionZone(P_cur);

        switch ( i_Selected.FoilObject->GetType() )
           {
            case mpl::OB_LEGEND : // Dropping the legend
                (*CurrentFrameIt)->Legend->DragPosition(P_cur);
                // Hide if dropped outside
                if ( Zone.Type == ZN_OUTSIDE ) (*CurrentFrameIt)->Legend->Visible = false;
                break;

            case mpl::OB_TRACK : // Dropping a Track...
                if ( Zone.Type == ZN_FRAME && Zone.Iterator != CurrentFrameIt )
                     { // ...A Track released on another Frame: move
                      Log->Entry("Edit mode","Moving Track " + (*CurrentFrameIt)->CurrentTrack->Name  + " from " + (*CurrentFrameIt)->Name + " to " + (*(Zone.Iterator))->Name);
                      #ifdef INCLUDE_SOUNDS
                        Sound(ID_WAV_DROPTRACK);
                      #endif
                      (*CurrentFrameIt)->MoveTrack( *(Zone.Iterator) );
                     }
                else if ( Zone.Type == ZN_FRAME )
                     { // ...A Track released in its same frame
                      Log->Entry("Edit mode","Track \'" + (*CurrentFrameIt)->CurrentTrack->Name + "\' released on its same frame: I\'ll do nothing");
                     }
                else if ( Zone.Type == ZN_OUTSIDE )
                     { // ...A Track released outside
                      Log->Entry("Edit mode","Deleting Track " + (*CurrentFrameIt)->CurrentTrack->Name);
                      (*CurrentFrameIt)->RemoveTrack();
                     }
                else { // ...A Track released on a border
                      Log->Entry("Edit mode","Track \'" + (*CurrentFrameIt)->CurrentTrack->Name + "\' released on plot border: I\'ll do nothing");
                     }
                break;

            case mpl::OB_FRAME : // Dropping a frame...

                Drawer->UnMarkZone();

                switch ( Zone.Type )
                   {
                    case ZN_FRAME :
                        {
                         if ( Zone.Iterator != CurrentFrameIt )
                              { // A frame released on another frame : merge

                               if ( mat::MsgAsk("Do you want to merge these frames?\n\nYes: Merge\nNo: Just swap them") )
                                     {
                                      Log->Entry("Edit mode","Merging " + (*CurrentFrameIt)->Name + " with " + (*(Zone.Iterator))->Name);
                                      MergeFrames(Zone.Iterator,CurrentFrameIt);
                                     }
                               else  {
                                      Log->Entry("Edit mode","Swapping " + (*CurrentFrameIt)->Name + " with " + (*(Zone.Iterator))->Name);
                                      SwapFrames(CurrentFrameIt,Zone.Iterator);
                                     }
                              }
                         else { // A frame released on itself : do nothing
                               Log->Entry("Edit mode","End dragging " + (*CurrentFrameIt)->Name);
                              }
                        } break;
                    case ZN_UPPERBAND:
                    case ZN_LEFTBORDER :
                    case ZN_RIGHTBORDER :
                    case ZN_LOWERBAND :
                        {
                         #ifdef INCLUDE_SOUNDS
                           Sound(ID_WAV_DROPFRAME);
                         #endif
                         Log->Entry("Edit mode","Moving " + (*CurrentFrameIt)->Name);
                         InsertFrame( *CurrentFrameIt , Zone );
                        } break;
                    case ZN_OUTSIDE : // A frame released outside the plot: delete
                        {
                         Log->Entry("Edit mode","Deleting " + (*CurrentFrameIt)->Name);
                         RemoveFrame( CurrentFrameIt );
                        } break;
                   } // end 'switch ( Zone.Type )'
                break;
            case mpl::OB_MARKER :
                if ( Zone.Type == ZN_OUTSIDE ) (*CurrentFrameIt)->Markers.Remove(i_Selected.Marker);
                break;
            case mpl::OB_LABEL :
                if ( Zone.Type == ZN_OUTSIDE ) (*CurrentFrameIt)->Labels.Remove(i_Selected.Label);
                break;
            //case mpl::OB_GRID :
            //case mpl::OB_AXES :
            //default :
           } // end 'switch ( selected object type )'
        Screen->Cursor = crDefault;
       } // end 'if ( IsMoving )'

// . . . Common operations
    CommonAfterMouseUp(Button,Shift,X,Y);

// . . . Finally
    Invalidate(); // Update plot
}

//---------------------------------------------------------------------------
void __fastcall cls_MPlot::OnMouseWheelSelect(TObject *Sender, TShiftState Shift,
                                int WheelDelta, const TPoint &MousePos, bool &Handled)
{ // OnMouseWheel event handler

// . . . Common operations
    if ( CommonMouseWheel(Shift,WheelDelta,MousePos,Handled) )
         {// In frame: cycle Tracks
          if ( WheelDelta < 0 ) SelectNextTrack();
          else SelectPrevTrack();
         }
    else {// Outside: cycle frames
          if ( WheelDelta < 0 ) SelectNextFrame();
          else SelectPrevFrame();
         }

// . . . Finally
    // Maintain selected stuff
    if ( OnUpdate ) OnUpdate();
}

//---------------------------------------------------------------------------
void __fastcall cls_MPlot::OnKeyDownSelect(TObject *Sender, WORD &Key, TShiftState Shift)
{ // OnKeyDown handler, externally called

// . . . Avoid key repeat (sets KeysPressed and)
    if ( CommonKeyDown(Key,Shift) ) return;

// . . . Handle Plot Keys Tasks
    if      ( Key == keyReset ) SelectFirstTrack();
    else if ( Key == keyDo    ) SelectNextTrack();
    //else if ( Key == keyCycle ) SelectNextTrack();
    //else if ( Key == keyPrior ) SelectPrevTrack();
    //else if ( Key == keyNext  ) electNextTrack();
    else if ( Key == keyMinus ) SelectPrevTrack();
    else if ( Key == keyPlus  ) SelectNextTrack();
    else if ( Key == keyStart ) SelectFirstTrack();
    else if ( Key == keyEnd   ) SelectLastTrack();
    else if ( Key == keyUp    ) SelectPrevTrack();
    else if ( Key == keyDown  ) SelectNextTrack();
    else if ( Key == keyLeft  ) SelectPrevTrack();
    else if ( Key == keyRight ) SelectNextTrack();
}


//---------------------------------------------------------------------------
void __fastcall cls_MPlot::OnMouseDownEdit(TObject *Sender,TMouseButton Button,TShiftState Shift,int X,int Y)
{ // OnMouseDown event handler

// . . . Common operations: Do nothing if clicked in invalid frame
    if ( !CommonMouseDown(Button,Shift,X,Y) ) return;

// . . . Specific mouse button operations
    switch ( Button )
           {
            case mbLeft :
                {
                 if ( Drawer->IsSelRect )
                      { // A Crop rectangle is ready
                       if ( mat::IsInRect(P_clk,Drawer->SelRect) )
                            {
                             if ( Shift.Contains(ssDouble) )
                                { // Perform crop operation
                                 CropTracks( Drawer->SelRect );
                                 // Erase crop rect
                                 Drawer->EraseSelRect();
                                }
                            }
                       else {
                             // Erase crop rect
                             Drawer->EraseSelRect();
                            }
                      }
                 else { // No crop rectangle
                       // Start dragging rectangle
                       IsDragDown = true;
                       //Screen->Cursor = crCrop;
                      }
                } break;
            case mbRight :
                { // Reset dragging rectangle
                 IsDragDown = false;
                 if ( Drawer->IsSelRect )
                      { // A Zoom rectangle is ready: abort crop
                       Drawer->EraseSelRect();
                      }
                 else { // No zoom rectangle: request context popup menu
                       if ( OnPopupMenu ) OnPopupMenu( ClientToScreen(P_clk) );
                      }
                } break;
            case mbMiddle :
                {
                 // TODO 5: empty middle click
                } break;
           }

// . . . Finally
    // Maintain selected stuff
    if ( OnUpdate ) OnUpdate();
}

//---------------------------------------------------------------------------
void __fastcall cls_MPlot::OnMouseMoveEdit(TObject *Sender, TShiftState Shift, int X, int Y)
{ // OnMouseMove event handler

// . . . Common operations
    CommonMouseMove(Shift, X, Y);

// . . . Specific operations
    if ( IsOverFrame )
         {
          // . . . Force frame selection (warning: assume fr valorized by 'CommonMouseMove')
          //if ( CurrentFrameIt != fr ) {CurrentFrameIt = fr; Invalidate();}

          // . . . Drawing crop rectangle
          if ( IsDragDown && IsStartedDragging() ) // requires 'P_cur' and 'P_clk'
               { // Draw crop rectangle
                if ( Screen->Cursor != crCrop ) Screen->Cursor = crCrop;
                Drawer->EraseSelRect();
                Drawer->DrawSelRect(TRect(P_clk,P_cur));
               }
          else { // Set cursors, erase crop rect if outside
                if ( Drawer->IsSelRect )
                   { // A Crop rectangle is waiting
                    if ( mat::IsInRect(P_cur,Drawer->SelRect) ) Screen->Cursor = crCrop;
                    else if ( mat::IsInRect(P_cur,CurrentFrame->BoundsRect) ) Screen->Cursor = crEdit;
                    else { Screen->Cursor = crDefault; Drawer->EraseSelRect(); }
                   }
               }
         }

// . . . Common operations
    CommonAfterMouseMove();
}

//---------------------------------------------------------------------------
void __fastcall cls_MPlot::OnMouseUpEdit(TObject *Sender,TMouseButton Button,TShiftState Shift,int X,int Y)
{ // OnMouseDown event handler

// . . . Perform actions according to mouse button
    switch ( Button )
           {
            case mbLeft :
                 // TODO: set edit sel rect
				 mat::stu_Rect SelRect = (*CurrentFrameIt)->Rphy(Drawer->SelRect);
                break;
            //case mbRight : break;
           }

// . . . Common operations
    CommonAfterMouseUp(Button,Shift,X,Y);
}

//---------------------------------------------------------------------------
void __fastcall cls_MPlot::OnMouseWheelEdit(TObject *Sender, TShiftState Shift,
                                int WheelDelta, const TPoint &MousePos, bool &Handled)
{ // OnMouseWheel event handler

// . . . Common operations
    if ( CommonMouseWheel(Shift,WheelDelta,MousePos,Handled) )
         {// In frame: cycle Tracks
          if ( WheelDelta < 0 ) (*CurrentFrameIt)->SelectNextTrack();
          else (*CurrentFrameIt)->SelectPrevTrack();
         }
    else {// Outside: cycle frames
          if ( WheelDelta < 0 ) SelectNextFrame();
          else SelectPrevFrame();
         }

// . . . Finally
    // Maintain selected stuff
    if ( OnUpdate ) OnUpdate();
}

//---------------------------------------------------------------------------
void __fastcall cls_MPlot::OnKeyDownEdit(TObject *Sender, WORD &Key, TShiftState Shift)
{ // OnKeyDown handler, externally called

// . . . Avoid key repeat (sets KeysPressed and)
    if ( CommonKeyDown(Key,Shift) ) return;

// . . . Handle Plot Keys Tasks
    if      ( Key == keyReset ) SelectFirstTrack();
    else if ( Key == keyDo    ) SelectNextTrack();
    //else if ( Key == keyCycle ) SelectNextTrack();
    //else if ( Key == keyPrior ) SelectPrevTrack();
    //else if ( Key == keyNext  ) SelectNextTrack();
    else if ( Key == keyMinus ) SelectPrevTrack();
    else if ( Key == keyPlus  ) SelectNextTrack();
    else if ( Key == keyStart ) SelectFirstTrack();
    else if ( Key == keyEnd   ) SelectLastTrack();
    else if ( Key == keyUp    ) SelectPrevTrack();
    else if ( Key == keyDown  ) SelectNextTrack();
    else if ( Key == keyLeft  ) SelectPrevTrack();
    else if ( Key == keyRight ) SelectNextTrack();
}



//---------------------------------------------------------------------------
void __fastcall cls_MPlot::OnMouseDownPan(TObject *Sender,TMouseButton Button,TShiftState Shift,int X,int Y)
{ // OnMouseDown event handler

// . . . Common operations: Do nothing if clicked in invalid frame
    if ( !CommonMouseDown(Button,Shift,X,Y) ) return;

// . . . Perform actions according to mouse button
    switch ( Button )
           {
            case mbLeft :
                {
                 if ( Shift.Contains(ssDouble) )
                      { // Go to origin
                       VpanX = VpanY = 0; // Avoid coasting
                       ResetOrigin();
                      }
                 else {
                       IsDragDown = true;
                       PanStatus = PS_DRAGGING;
                       Screen->Cursor = crHandClosed;
                      }
                } break;
            case mbRight :
                {
                 // Manage context popup menu?
                 // Temporary switch to measure mode? ToggleToMode(MD_MEASURE);

                 // Enhanced Pan
                 IsDragDown = true;
                 PanStatus = PS_PULLING;
                 timPan->Enabled = true; // Start pulling
                 Screen->Cursor = crSizeAll;
                } break;
            case mbMiddle :
                {
                 // Go to origin
                 VpanX = VpanY = 0; // Avoid coasting
                 ResetOrigin();
                } break;
           }
}

//---------------------------------------------------------------------------
void __fastcall cls_MPlot::OnMouseMovePan(TObject *Sender, TShiftState Shift, int X, int Y)
{ // OnMouseMove event handler

// . . . Common operations
    CommonMouseMove(Shift, X, Y);

// . . . Specific operations: do pan
    if ( IsOverFrame )
         {
          // . . . Different pan modes operations
          switch ( PanStatus )
           {
            case PS_DRAGGING:

              // . . . Pan speed measuring
              static int DX, DY; // Displacements [pix]
              DX = X-P_pre.x; DY = Y-P_pre.y; // Get new displacements
              MeasurePanSpeed(DX, DY);
              // . . . Do Pan
              Pan(DX, DY);
            break;

            case PS_PULLING:
              // . . . Just some feedback
              Drawer->EraseDirection();
              Drawer->DrawDirection(P_clk,P_cur);
            break;
           }
         }

// . . . Common operations
    CommonAfterMouseMove();
}

//---------------------------------------------------------------------------
void __fastcall cls_MPlot::OnMouseUpPan(TObject *Sender,TMouseButton Button,TShiftState Shift,int X,int Y)
{ // OnMouseDown event handler

// . . . Specific operations
    if ( PanStatus == PS_PULLING ) Drawer->EraseDirection();
    PanStatus = PS_RELEASING;
    timPan->Enabled = true; // Starts free evolution

    #ifdef PLT_DEBUG
      Log->Entry("Pan","X: " + String(P_cur.x-P_clk.x) + " Y: " + String(P_cur.y-P_clk.y));
    #endif

// . . . Common operations
    CommonAfterMouseUp(Button,Shift,X,Y);
}

//---------------------------------------------------------------------------
void __fastcall cls_MPlot::OnMouseWheelPan(TObject *Sender, TShiftState Shift,
                                int WheelDelta, const TPoint &MousePos, bool &Handled)
{ // OnMouseWheel event handler

// . . . Common operations
    if ( CommonMouseWheel(Shift,WheelDelta,MousePos,Handled) )
          { // On a valid frame
           // . . . Do Pan
           if ( Shift.Contains(ssRight) )
                { // Y axis
                 Pan(0,-WheelDelta);
                }
           else { // X axis
                 Pan(WheelDelta,0);
                }
          }
}

//---------------------------------------------------------------------------
void __fastcall cls_MPlot::OnKeyDownPan(TObject *Sender, WORD &Key, TShiftState Shift)
{ // OnKeyDown handler, externally called

// . . . Avoid key repeat (sets KeysPressed and)
    if ( CommonKeyDown(Key,Shift) ) return;

// . . . Handle Plot Keys Tasks
    if      ( Key == keyReset ) ResetOrigin();
    else if ( Key == keyDo    ) ResetOrigin();
    //else if ( Key == keyCycle ) CurrentFrame->SelectNextTrack();
    //else if ( Key == keyPrior ) CurrentFrame->SelectPrevTrack();
    //else if ( Key == keyNext  ) CurrentFrame->SelectNextTrack();
    else if ( Key == keyMinus ) {if (CurrentFrame) Pan((*CurrentFrameIt)->width,0);}
    else if ( Key == keyPlus  ) {if (CurrentFrame) Pan(-(*CurrentFrameIt)->width,0);}
    // TODO 5: go to current track start or end
    else if ( Key == keyStart ) SelectFirstTrack();
    else if ( Key == keyEnd   ) SelectLastTrack();
    else if ( Key == keyUp    ) {if (CurrentFrame) Pan(0,(*CurrentFrameIt)->height/DefaultPan);}
    else if ( Key == keyDown  ) {if (CurrentFrame) Pan(0,-(*CurrentFrameIt)->height/DefaultPan);}
    else if ( Key == keyLeft  ) {if (CurrentFrame) Pan((*CurrentFrameIt)->width/DefaultPan,0);}
    else if ( Key == keyRight ) {if (CurrentFrame) Pan(-(*CurrentFrameIt)->width/DefaultPan,0);}
}



//---------------------------------------------------------------------------
void __fastcall cls_MPlot::OnMouseDownZoom(TObject *Sender,TMouseButton Button,TShiftState Shift,int X,int Y)
{ // OnMouseDown event handler

// . . . Common operations: Do nothing if clicked in invalid frame
    if ( !CommonMouseDown(Button,Shift,X,Y) ) return;

// . . . Perform actions according to mouse button
    switch ( Button )
           {
            case mbLeft :
                {
                 if ( Drawer->IsSelRect )
                      { // A Zoom rectangle is ready
                       if ( mat::IsInRect(P_clk,Drawer->SelRect) )
                            { // Perform Zoom operation
                             Zoom( Drawer->SelRect );
                             #ifdef PLT_DEBUG
                               Log->Entry("ZoomRect",String(Drawer->SelRect.Width()) + " x " + String(Drawer->SelRect.Height()));
                             #endif
                            }
                       // Erase zoom rect
                       Drawer->EraseSelRect();
                       // Set a flag for default left click zoom
                       IsDragDown = false;
                      }
                 else { // No zoom rectangle
                       // Double click does nothing
                       //if ( Shift.Contains(ssDouble) ) ; // FitScale();
                       // Otherwise, start dragging zoom rect
                       IsDragDown = true;
                      }
                } break;
            case mbRight :
                {
                 IsDragDown = false; // Reset dragging rectangle

                 if ( Drawer->IsSelRect )
                      { // A Zoom rectangle is ready: abort
                       Drawer->EraseSelRect();
                      }
                 else { // No zoom rectangle: perform generic Zoom Out
                       if (Shift.Contains(ssLeft)) DefaultZoom(*CurrentFrameIt,P_cur,ZoomPerc/100.0,true,false);
                       else DefaultZoom(*CurrentFrameIt,P_cur,ZoomPerc/100.0,false,false);
                      }
                } break;
            case mbMiddle :
                {
                 // Perform generic Zoom In, create a default zoom rectangle
                 //DefaultZoom(*CurrentFrameIt,P_cur,-ZoomPerc/100.0);
                 FitScale();
                } break;
           }

// . . . Finally
    // Maintain selected stuff
    if ( OnUpdate ) OnUpdate();
}

//---------------------------------------------------------------------------
void __fastcall cls_MPlot::OnMouseMoveZoom(TObject *Sender, TShiftState Shift, int X, int Y)
{ // OnMouseMove event handler

// . . . Common operations
    CommonMouseMove(Shift, X, Y);

// . . . Specific operations
    if ( IsOverFrame )
         {
          // . . . Force frame selection (warning: assume fr valorized by 'CommonMouseMove')
          //if ( CurrentFrameIt != fr ) {CurrentFrameIt = fr; Invalidate();}

          // . . . Drawing zoom rectangle
          if ( IsDragDown && IsStartedDragging() ) // requires 'P_cur' and 'P_clk'
               { // Draw
                //Screen->Cursor = crDefault;
                Drawer->EraseSelRect();
                Drawer->DrawSelRect(TRect(P_clk,P_cur));
               }
          else { // Set cursors, erase zoomrect if outside
                if ( Drawer->IsSelRect )
                   { // A Zoom rectangle is waiting
                    if ( mat::IsInRect(P_cur,Drawer->SelRect) ) Screen->Cursor = crZoomP;
                    else if ( mat::IsInRect(P_cur,CurrentFrame->BoundsRect) ) Screen->Cursor = crZoomX;
                    else { Screen->Cursor = crDefault; Drawer->EraseSelRect(); }
                   }
               }
         }

// . . . Common operations
    CommonAfterMouseMove();
}



//---------------------------------------------------------------------------
void __fastcall cls_MPlot::OnMouseUpZoom(TObject *Sender,TMouseButton Button,TShiftState Shift,int X,int Y)
{ // OnMouseUp event handler

// . . . Perform actions according to mouse button
    switch ( Button )
           {
            case mbLeft :
                {
                 if ( Drawer->IsSelRect && SuddenZoom )
                      { // A rectangle is drawn; if sudden zoom, perform zoom immediately

                       // Do Zoom
                       Zoom( Drawer->SelRect );
                       Drawer->EraseSelRect(); // Redundant, since there will be a redraw
                      }
                 else if ( !Drawer->IsSelRect && IsDragDown )
                      { // If no zoom rectangle was done, default zoom in
                       if (Shift.Contains(ssRight)) DefaultZoom(*CurrentFrameIt,P_cur,-ZoomPerc/100.0,true,false);
                       else DefaultZoom(*CurrentFrameIt,P_cur,-ZoomPerc/100.0,false,false);
                      }
                } break;
            //case mbRight :
           }

// . . . Common operations
    CommonAfterMouseUp(Button,Shift,X,Y);
}

//---------------------------------------------------------------------------
void __fastcall cls_MPlot::OnMouseWheelZoom(TObject *Sender, TShiftState Shift,
                                int WheelDelta, const TPoint &MousePos, bool &Handled)
{ // OnMouseWheel event handler

// . . . Common operations
    if ( CommonMouseWheel(Shift,WheelDelta,MousePos,Handled) )
          { // On a valid frame
           // . . . Do default zoom
           if ( Shift.Contains(ssRight) ) DefaultZoom(*CurrentFrameIt,P_cur,(ZoomPerc/200.0)*(WheelDelta/::abs(WheelDelta)),true,false);
           else if (Shift.Contains(ssLeft)) DefaultZoom(*CurrentFrameIt,P_cur,(ZoomPerc/200.0)*(WheelDelta/::abs(WheelDelta)),false,true);
           else DefaultZoom(*CurrentFrameIt,P_cur,(ZoomPerc/200.0)*(WheelDelta/::abs(WheelDelta)),false,false);
          }
}

//---------------------------------------------------------------------------
void __fastcall cls_MPlot::OnKeyDownZoom(TObject *Sender, WORD &Key, TShiftState Shift)
{ // OnKeyDown handler, externally called

// . . . Avoid key repeat (sets KeysPressed and)
    if ( CommonKeyDown(Key,Shift) ) return;

// . . . Handle Plot Keys Tasks
    if      ( Key == keyReset ) FitScale();
    else if ( Key == keyDo    ) DefaultZoom (true,false,false);
    //else if ( Key == keyCycle ) {if (CurrentFrame) CurrentFrame->SelectNextTrack();}
    //else if ( Key == keyPrior ) {if (CurrentFrame) CurrentFrame->SelectPrevTrack();}
    //else if ( Key == keyNext  ) {if (CurrentFrame) CurrentFrame->SelectNextTrack();}
    else if ( Key == keyMinus ) DefaultZoom (false,false,false);
    else if ( Key == keyPlus  ) DefaultZoom (true,false,false);
    else if ( Key == keyStart ) FitScale();
    else if ( Key == keyEnd   ) FitScale();
    else if ( Key == keyUp    ) DefaultZoom (false,false,true);
    else if ( Key == keyDown  ) DefaultZoom (true,false,true);
    else if ( Key == keyLeft  ) DefaultZoom (false,true,false);
    else if ( Key == keyRight ) DefaultZoom (true,true,false);
}



//---------------------------------------------------------------------------
void __fastcall cls_MPlot::OnMouseDownMeasure(TObject *Sender,TMouseButton Button,TShiftState Shift,int X,int Y)
{ // OnMouseDown event handler

// . . . Common operations: Do nothing if clicked in invalid frame
    if ( !CommonMouseDown(Button,Shift,X,Y) ) return;

// . . . Perform actions according to mouse button
    switch ( Button )
           {
            case mbLeft :
                { // Select first measure point of selected track
                 if ( Shift.Contains(ssDouble) )
                      { // Add current point to log
                       Log->Add(CurrentPoint);
                      }
                 else { // Request interval measure
                       #ifdef INCLUDE_SOUNDS
                         Sound(ID_WAV_SET);
                       #endif
                       IsDragDown = true;
                       Screen->Cursor = crSqCross;
                       Drawer->DrawMeasureLines1();
                      }
                } break;
            case mbRight :
                {
                 // Request context popup menu
                 if ( OnPopupMenu ) OnPopupMenu( ClientToScreen(P_clk) );
                 Drawer->ResetMeasure();
                 Invalidate();
                } break;
            case mbMiddle :
                { // Set a persisting marker on point
                 #ifdef INCLUDE_SOUNDS
                   Sound(ID_WAV_SET);
                 #endif
                 Drawer->DrawMeasureLines1(true);
                } break;
           }
}

//---------------------------------------------------------------------------
void __fastcall cls_MPlot::OnMouseMoveMeasure(TObject *Sender, TShiftState Shift, int X, int Y)
{ // OnMouseMove event handler

// . . . Common operations
    CommonMouseMove(Shift, X, Y);

// . . . Common operations
    if ( IsOverFrame )
         {
          // . . . Force frame selection (warning: assume fr valorized by 'CommonMouseMove')
          if ( CurrentFrameIt != fr )
             {
              CurrentFrameIt = fr;
              Invalidate(); // Reflect change
             }

          // . . . If dragging
          if ( IsDragDown )
             {
              UpdateTrackIntervalQuantities();
             }

          // . . . Temporary switch to select mode if over legend
          if ( (*CurrentFrameIt)->Legend->IsOver(P_cur) ) {ToggleToMode(MD_SELECT); return;}

          // . . . Maintain track point quantities
          UpdateTrackPointQuantities();

          // . . . What is doing?
          //if ( IsDragDown && IsStartedDragging() ) // requires 'P_cur' and 'P_clk'
          // Measuring interval
          // . . . Draw second measure lines if on a frame
          Drawer->EraseMeasureLines2(); // Erase previous lines if existing
          Drawer->DrawMeasureLines2();
         } // end 'is over frame'
    else {
          // . . . Gone outside frames: erase all
          Drawer->EraseMeasureLines1();
          Drawer->EraseMeasureLines2();
          Invalidate();
         }

// . . . Common operations
    CommonAfterMouseMove();
}

//---------------------------------------------------------------------------
void __fastcall cls_MPlot::OnMouseUpMeasure(TObject *Sender,TMouseButton Button,TShiftState Shift,int X,int Y)
{ // OnMouseUp event handler

// . . . Check frame
    CheckCurrentFrame (P_cur, false); // This sets 'IsOverFrame','IsOverCurrFrame','IsCurrFrameChanged' and 'CurrentFrameIt' if 'true'

// . . . Specific mouse up operations
    // . . . Perform actions according to mouse button
    switch ( Button )
           {
            case mbLeft :
                {
                 if ( IsDragDown )
                    { // Returning from interval measure: erase first measure point
                     if ( IsOverCurrFrame ) Drawer->EraseMeasureLines1();
                     Screen->Cursor = crDefault;
                    }
                 Invalidate();
                } break;
            case mbRight :
                { // Returning from popup menu
                 if ( OnPopupMenu ) OnPopupMenu( ClientToScreen(P_clk) );
                 Invalidate();
                } break;
            case mbMiddle :
                {
                 // TODO 5: empty middle click
                } break;
           }

// . . . Common operations
    CommonAfterMouseUp(Button,Shift,X,Y);

// . . . Finally
    // May be toggled from other modes
    //if ( UntoggleMode() ) Drawer->EraseMeasureLines2();
}

/*
//---------------------------------------------------------------------------
void __fastcall cls_MPlot::OnMouseWheelMeasure(TObject *Sender, TShiftState Shift,
                                int WheelDelta, const TPoint &MousePos, bool &Handled)
{ // OnMouseWheel event handler

// . . . Common operations
    if ( CommonMouseWheel(Shift,WheelDelta,MousePos,Handled) )
          { // On a valid frame
           // TODO: go to next point WheelDelta/::abs(WheelDelta)
          }

    #ifdef PLT_DEBUG
      Log->SubEntry("Measure wheel " + String(WheelDelta));
    #endif
}
*/

//---------------------------------------------------------------------------
void __fastcall cls_MPlot::OnKeyDownMeasure(TObject *Sender, WORD &Key, TShiftState Shift)
{ // OnKeyDown handler, externally called

// . . . Avoid key repeat (sets KeysPressed and)
    if ( CommonKeyDown(Key,Shift) ) return;

// . . . Handle Plot Keys Tasks
    if      ( Key == keyReset ) SelectFirstTrack();
    else if ( Key == keyDo    ) SelectNextTrack();
    //else if ( Key == keyCycle ) SelectNextTrack();
    //else if ( Key == keyPrior ) SelectPrevTrack();
    //else if ( Key == keyNext  ) SelectNextTrack();
    else if ( Key == keyMinus ) SelectPrevTrack();
    else if ( Key == keyPlus  ) SelectNextTrack();
    else if ( Key == keyStart ) SelectFirstTrack();
    else if ( Key == keyEnd   ) SelectLastTrack();
    else if ( Key == keyUp    ) SelectPrevTrack();
    else if ( Key == keyDown  ) SelectNextTrack();
    else if ( Key == keyLeft  ) Mouse->CursorPos = TPoint(Mouse->CursorPos.x-1,Mouse->CursorPos.y); // ClientToScreen // SelectPrevTrack();
    else if ( Key == keyRight ) Mouse->CursorPos = TPoint(Mouse->CursorPos.x+1,Mouse->CursorPos.y); //SelectNextTrack();
}


//---------------------------------------------------------------------------
void __fastcall cls_MPlot::OnMouseDownPen(TObject *Sender,TMouseButton Button,TShiftState Shift,int X,int Y)
{ // OnMouseDown event handler

// . . . Common operations: Do nothing if clicked in invalid frame
    if ( !CommonMouseDown(Button,Shift,X,Y) ) return;

// . . . Specific operations
    // . . . Perform actions according to mouse button
    switch ( Button )
           {
            case mbLeft :
                 // Select a pencil style
                 if ( Shift.Contains(ssDouble) ) Invalidate();
                 else {
                       Screen->Cursor = crPen;
                       IsDragDown = true;
                       Drawer->SetPencil();
                      }
                 break;
            case mbRight :
                 // Select another style (highlighter)
                 Screen->Cursor = crHighlighter;
                 IsDragDown = true;
                 Drawer->SetHighlighter();
                 break;
            case mbMiddle :
                 Invalidate();
                 break;
           }
}

//---------------------------------------------------------------------------
void __fastcall cls_MPlot::OnMouseMovePen(TObject *Sender, TShiftState Shift, int X, int Y)
{ // OnMouseMove event handler

// . . . Common operations
    CommonMouseMove(Shift, X, Y);

// . . . Specific operations: color pixels
    if ( IsDragDown && IsOverFrame ) Drawer->Stroke(P_pre,P_cur);

// . . . Common operations
    CommonAfterMouseMove();
}

//---------------------------------------------------------------------------
void __fastcall cls_MPlot::OnMouseUpPen(TObject *Sender,TMouseButton Button,TShiftState Shift,int X,int Y)
{ // OnMouseUp event handler

// . . . Common operations
    CommonAfterMouseUp(Button,Shift,X,Y);
}



//---------------------------------------------------------------------------




//============================= Private methods =============================



//---------------------------- frames collection ----------------------------

//---------------------------------------------------------------------------
inline void cls_MPlot::CheckInvariant()
{ // Invariant check : ensure frames Layout coherence

    unsigned int sum=0;
    typ_LayoutIt l = Layout.begin();
    while ( l != Layout.end() )
          {
           if ( (*l) > 0 ) { sum += (*l); ++l; }
           else l = Layout.erase(l); // delete empty rows
          }
    // Check Layout consistency
    if ( sum != frames.size() )
       { // Layout inconsistency!
        String L ="| ";
        for (uint i=0; i<Layout.size(); ++i) L += String(Layout[i]) + " | ";
        L += "= " + String(sum);
        mat::MsgError("cls_MPlot","Warning: inconsistent Layout!\nThere are " + String(frames.size()) + " frames with Layout: " + L);
       }
    // Plot layout change event
    if ( frames.empty() ) cls_Frame::Number = 0; // Reset frames overall number
    if (OnChange) OnChange();
}


//---------------------------------------------------------------------------
cls_Frame* cls_MPlot::CreateFrame( const stu_PlotZone& zone )
{ // Create and insert a Frame object, updating current frame

// . . . Don't create if too much
    if ( !frames.empty() )
       {
        cls_Frame* f = frames.back();
        if ( f->height<=(2*f->MarginY) || f->width<=(2*f->MarginX) )
           {
            Log->Warn("I will not create a new frame: too much density");
            return 0;
           }
       }

// . . . Create frame
    cls_Frame* newfrptr = new cls_Frame(this);
    if ( !newfrptr )
       {
        Log->Error("Unable to create a new Frame");
        return 0;
       }

// . . . Add to frame collection and calculate new dimensions
    newfrptr->RecalculateMargins(CurrentFoil->PointsPerPix);
    InsertFrame( newfrptr, zone );

// . . . Now set size dependent properties
    newfrptr->Grid->Reset();
    newfrptr->RecalculateMargins(CurrentFoil->PointsPerPix);
    newfrptr->Legend->RecalculateMargins(CurrentFoil->PointsPerPix);
    newfrptr->Legend->Reset();

// . . . Finally
    return newfrptr;
}

//---------------------------------------------------------------------------
cls_Frame* cls_MPlot::CreateFrame()
{ // Create and insert a Frame below

    stu_PlotZone zone;
    zone.Type = ZN_LOWERBAND;
    zone.Iterator = --frames.end();
    return CreateFrame (zone);
}


//---------------------------------------------------------------------------
void cls_MPlot::MoveFrame( typ_FramesIt src, typ_FramesIt dst )
{ // Move a Frame before a destination

    if ( src != frames.end() && dst != frames.end() )
       {
        if ( dst == --frames.end() ) frames.insert( frames.end(), *src ); // queue back if on last
        else frames.insert( dst, *src ); // insert before
        frames.erase( src );
        CheckInvariant();
        PlaceFrames();
       }
}

//---------------------------------------------------------------------------
void cls_MPlot::InsertFrame( class cls_Frame* frameptr, const stu_PlotZone& zone )
{ // Move/insert a Frame in a zone

    // Check if is a new or already existing Frame
    typ_FramesIt frameit = FindFrame(frameptr);
    bool exists = ( frameit != frames.end() );

    // Remove frame from list if existing (support moving)
    // Now remove only from Layout
    if (exists) --Layout.at( (*frameit)->Row ); // (now to avoid iterator invalidation)

    switch ( zone.Type )
        {
         case ZN_FRAME :
             { // Put aside
              #ifdef PLT_DEBUG
                Log->SubEntry((*(zone.Iterator))->Name+" ("+String(P_cur.x)+","+String(P_cur.y)+")");
              #endif
              ++Layout.at( (*(zone.Iterator))->Row );
              CurrentFrameIt = frames.insert( zone.Iterator, frameptr );
             } break;
         case ZN_UPPERBAND:
             { // Insert a new row
              #ifdef PLT_DEBUG
                Log->SubEntry("Upper band of "+ (*(zone.Iterator))->Name+" ("+String(P_cur.x)+","+String(P_cur.y)+")");
              #endif
              Layout.insert( Layout.begin() + (*(zone.Iterator))->Row, 1 );
              CurrentFrameIt = frames.insert( zone.Iterator, frameptr );
             } break;
         case ZN_LEFTBORDER :
             { // Put aside
              #ifdef PLT_DEBUG
                Log->SubEntry("Left border of "+(*(zone.Iterator))->Name+" ("+String(P_cur.x)+","+String(P_cur.y)+")");
              #endif
              ++Layout.at( (*(zone.Iterator))->Row );
              CurrentFrameIt = frames.insert( zone.Iterator, frameptr );
             } break;
         case ZN_RIGHTBORDER :
             { // Put aside
              #ifdef PLT_DEBUG
                Log->SubEntry("Right border of "+ (*(zone.Iterator))->Name+" ("+String(P_cur.x)+","+String(P_cur.y)+")");
              #endif
              ++Layout.at( (*(zone.Iterator))->Row );
              fr = zone.Iterator;
              CurrentFrameIt = frames.insert( ++fr, frameptr );
             } break;
         case ZN_LOWERBAND :
             { // Append to tail
              #ifdef PLT_DEBUG
                Log->SubEntry("Lower band ("+String(P_cur.x)+","+String(P_cur.y)+")");
              #endif
              Layout.push_back(1);
              CurrentFrameIt = frames.insert(frames.end(), frameptr );
             } break;
         case ZN_OUTSIDE :
             { // Do nothing
              #ifdef PLT_DEBUG
                Log->SubEntry("Outside ("+String(P_cur.x)+","+String(P_cur.y)+")");
              #endif
             } break;
        }

    // Finish removing existing frame from its previous position
    if (exists) frames.erase( frameit ); // Finish remove

// . . . Finally
    CheckInvariant();
    PlaceFrames(); // Reassign frames positions
}

//---------------------------------------------------------------------------
void cls_MPlot::RemoveFrame( typ_FramesIt frameit )
{ // Deallocate a Frame object

    if ( frameit != frames.end() && (*frameit) )
         {
          // Freedback
          #ifdef INCLUDE_SOUNDS
            Sound(ID_WAV_DELFRAME);
          #endif

          // Layout update
          --Layout.at( (*frameit)->Row ); // Layout update

          // Update frame tracks
          (*frameit)->ClearTracks();

          // Memory deallocation
          delete (*frameit);

          // Plot frame-list update
          CurrentFrameIt = frames.erase( frameit );
          if ( CurrentFrameIt == frames.end() ) CurrentFrameIt = frames.begin();

          // Finally
          CheckInvariant();
          PlaceFrames(); // Reassign frames positions
          Invalidate();
         }
    else {
          Log->Warn("RemoveFrame","Attempt to remove a null pointer");
         }
}

/*
//---------------------------------------------------------------------------
TRect cls_MPlot::GetZoneRect( const TPoint& P )
{ // Get the zone rectangle (used only by marker to mark zone)
  // This shares geometry assumption with 'GetInsertionZone' and 'PlaceFrames'
  // Original one (effective rects)
        TRect R;
        unsigned int Band = i_Margin;
        unsigned int QuadHeight = (Height - Band) / Layout.size();

        // detect the row
        unsigned int row = P.y / QuadHeight;
        unsigned int rest = P.y % QuadHeight;
        //if ( row >= Layout.size() ) R = TRect(0,Height-i_Margin,Width,Height); // LOWERBAND
        if ( rest <= Band ) R = TRect(0,row*QuadHeight,Width,row*QuadHeight+i_Margin); // HORZBAND
        else { // Got row, now detect col
              unsigned int QuadWidth = (Width - Band) / Layout.at(row);
              unsigned int col = P.x / QuadWidth;
              rest = P.x % QuadWidth;

              if ( col >= Layout.at(row) ) R = TRect(Width-i_Margin,(row*QuadHeight)+i_Margin,Width,(row+1)*QuadHeight); // RIGHTBORDER
              else if ( rest <= Band ) R = TRect(col*QuadWidth,row*QuadHeight+i_Margin,col*QuadWidth+i_Margin,(row+1)*QuadHeight); // LEFTBORDER
              //else R = TRect(col*QuadWidth+i_Margin,row*QuadHeight+i_Margin,(col+1)*QuadWidth,(row+1)*QuadHeight); // FRAME: get frame rect
              else R = TRect(-1,-1,-1,-1); // FRAME: don't mark frame area
             }
        return R;
}
*/

//---------------------------------------------------------------------------
TRect cls_MPlot::GetZoneRect( const TPoint& P )
{ // Get the zone rectangle (used only by marker to mark zone)
  // This shares geometry assumption with 'GetInsertionZone' and 'PlaceFrames'
        static TRect R;
        unsigned int QuadWidth, rest, row, col;

        // . . . Assign lengths
        unsigned int Before = i_Margin + cls_Frame::DefaultMarginY;
        unsigned int QuadHeight = (Height - i_Margin) / Layout.size();
        unsigned int After = QuadHeight - cls_Frame::DefaultMarginY;

        // detect the row
        row = P.y / QuadHeight;
        rest = P.y % QuadHeight;
        if ( rest <= Before ) R = TRect(0,row*QuadHeight,Width,row*QuadHeight+i_Margin); // previous HORZBAND
        else if ( rest >= After ) R = TRect(0,(row+1)*QuadHeight,Width,(row+1)*QuadHeight+i_Margin); // next HORZBAND
        else { // Got row, now detect col
              QuadWidth = (Width - i_Margin) / Layout.at(row);
              After = QuadWidth - cls_Frame::DefaultMarginX;
              col = P.x / QuadWidth;
              rest = P.x % QuadWidth;

              if ( rest <= Before ) R = TRect(col*QuadWidth,row*QuadHeight+i_Margin,col*QuadWidth+i_Margin,(row+1)*QuadHeight); // previous VERTMARGIN
              else if ( rest >= After ) R = TRect((col+1)*QuadWidth,row*QuadHeight+i_Margin,(col+1)*QuadWidth+i_Margin,(row+1)*QuadHeight); // next VERTMARGIN
              else { // FRAME: get frame rect
                    R = mat::Shrink(TRect(col*QuadWidth+i_Margin,row*QuadHeight+i_Margin,(col+1)*QuadWidth,(row+1)*QuadHeight),90); // Shrink rectangle to reduce marking work
                   }
             }
        return R;
}

//---------------------------------------------------------------------------
cls_MPlot::stu_PlotZone cls_MPlot::GetInsertionZone (const TPoint& P)
{ // Get plot zone by point coordinates
  // This shares geometry assumption with 'GetZoneRect' and 'PlaceFrames'

        stu_PlotZone zone;
        zone.Iterator = frames.begin();

        if ( !mat::IsInRect(P,BoundsRect) )
           { // Outside plot
            zone.Type = ZN_OUTSIDE;
            return zone;
           }

        unsigned int row = 0; // current row
        unsigned int col; // current column
        while ( zone.Iterator != frames.end() )
             { // Consider all frames
              col = 0;
              if ( P.y < (*zone.Iterator)->top )
                   { // Upper band
                    zone.Type = ZN_UPPERBAND;
                    return zone;
                   }
              else if ( P.y < (*zone.Iterator)->bottom )
                   { // Is in this row
                    do  {
                          if ( P.x < (*zone.Iterator)->left )
                             { // Left border
                              zone.Type = ZN_LEFTBORDER;
                              return zone;
                             }
                          else if ( P.x < (*zone.Iterator)->right )
                             { // In frame
                              zone.Type = ZN_FRAME;
                              return zone;
                             }
                        }
                    while ( ++zone.Iterator!=frames.end() && ++col<Layout[row] );
                    // If here is in the row right border
                    zone.Type = ZN_RIGHTBORDER;
                    --zone.Iterator; // Get the preceding frame
                    return zone;
                   }
              else { // Skip this row
                    while ( ++zone.Iterator!=frames.end() && ++col<Layout[row] ) {;}
                   }
              ++row;
             }
        // If here no frames, or lower band
        zone.Type = ZN_LOWERBAND;
        return zone;
}



//----------------------------- Other methods  ------------------------------

//---------------------------------------------------------------------------
inline void cls_MPlot::UpdateTrackPointQuantities()
{// Calculate ordinate and slope (mat::NaN if invalid)

// . . . Initializations
    cls_Track* tr = CurrentTrack;


// . . . Update values
    if ( tr )
         {
          // TODO 5: in general independent variable does not coincide with x
          /*
            physical         independent                track
            coordinates  =>   variable         =>     coordinates
             (x,y)           t = fx^-1(x)               y=fy(t)
                        if more than one, see fy^-1     x=fx(t)
          */  /*
          double t; // independent variable
          cls_FunctionTrack* ftrack = dynamic_cast<const cls_FunctionTrack*>(CurrentTrack);
          cls_ParametricTrack* ptrack = dynamic_cast<const cls_ParametricTrack*>(CurrentTrack);
          if ( ftrack ) t = p.frm_x; // independent variable
          else if ( ptrack )
             {
              mat::numlist nl = fx_inv(p.frm_x); // independent variable
              if ( nl.size() > 1 ) // get all y
              t = ...
             }
          */
          tr->UpdatePointQuantities( p.frm.right, &(p.trk) ); // t = p.frm.x2
         }
    else {
          p.trk.y = p.trk.x = p.trk.dy = p.trk.dx = p.trk.dydx = mat::NaN;
         }
}

//---------------------------------------------------------------------------
inline void cls_MPlot::UpdateTrackIntervalQuantities()
{// Calculate interval quantities (mat::NaN if invalid)

// . . . Initializations
    cls_Track* tr = CurrentTrack;

// . . . Update values
    if ( tr )
         {
          // TODO 5: in general independent variable does not coincide with x
          //if ( ::fabs(p.frm.x1 - p.frm.x2) > 4 ) int b = 0;
          tr->UpdateIntervalQuantities (p.frm.left, p.frm.right, &q);
         }
    else {
          q.n = 0;
          q.Dt = q.tmin = q.tmax = q.Dy = q.DY = q.ymin = q.ymax = q.DyDt = q.Sy = q.avg = q.rms = q.std = q.stdp = q.varp = mat::NaN;
          #ifdef MAD
          q.mad = mat::NaN;
          #endif
         }
}


//---------------------------------------------------------------------------
inline void cls_MPlot::DefaultZoom(cls_Frame* fr, const TPoint& P, double r, bool x_only, bool y_only)
{ // Do default zoom

    // Build a default ZoomRect
    r = ( 1 + r ) / 2; // ratio
    int w2 = y_only ? fr->width/2 : mat::round( r * fr->width );
    int h2 = x_only ? fr->height/2 : mat::round( r * fr->height );
    TRect R = TRect(P.x-w2, P.y-h2, P.x+w2, P.y+h2);

    // A temp rect
    //Drawer->DrawSelRect( R );

    // Main operation
    Zoom(R);

    // Finally
    Mouse->CursorPos = ClientToScreen(fr->C); // Move cursor in the transformed zoom point
    Drawer->ResetSelRect(); // Actually not needed, already erased by refresh in zoom op
}

//---------------------------------------------------------------------------
inline void cls_MPlot::ToggleToMode (const en_Modes m)
{ // Temporarily toggle to mode
    if ( !IsModeToggled )
       {
        Mode = m;
        IsModeToggled = true;
       }
}

//---------------------------------------------------------------------------
inline bool cls_MPlot::UntoggleMode()
{ // Restore previous mode
    if (IsModeToggled)
       {
        IsModeToggled = false;
        Mode = PreviousMode;
        return true;
       }
    else return false;
}

#ifdef INCLUDE_SOUNDS
//---------------------------------------------------------------------------
inline void cls_MPlot::Sound(const unsigned short resid)
{ // Play a sound from resource
    if(Noisy)::PlaySound(MAKEINTRESOURCE(resid),HInstance,SND_ASYNC|SND_RESOURCE|SND_NOWAIT|SND_NODEFAULT);
}
#endif

//---------------------------------------------------------------------------
inline void cls_MPlot::MeasurePanSpeed()
{ // Measure Pan speed basing on actual frame position

    ASSERTING((CurrentFrame),"CurrentFrame must exists to MeasurePanSpeed")
    
// . . . Local variables
    static TPoint O1 = (*CurrentFrameIt)->O , O2; // Positions [pix]
    static double Dt; // Time interval [s]

// . . . Measure speed
    O2 = (*CurrentFrameIt)->O; // Get new position
    MeasurePanSpeed(O2.x-O1.x, O2.y-O1.y);
    O1 = O2; // Update old position
}

//---------------------------------------------------------------------------
inline void cls_MPlot::MeasurePanSpeed(const int DX, const int DY)
{ // Measure Pan speed from intercurrent displacement

// . . . Local variables
    static double Dt; // Time interval [s]

// . . . Pan speed measuring
    Dt = TimeCounter->GetTime(MT_PAN); // Get time (first call gets a big time)
    // Avoid big speeds, limit them
    if(Dt<0.008) Dt = 0.008;
    // Average with last measure: better no
    //VpanX = ((double(DX) / Dt) + VpanX) / 2;
    //VpanY = ((double(DY) / Dt) + VpanY) / 2;
    // No averaging TODO: filter somehow
    VpanX = double(DX) / Dt;
    VpanY = double(DY) / Dt;
    TimeCounter->StartTime(MT_PAN);

// . . . Finally
    #ifdef PLT_DEBUG
      Log->SubEntry("Pan speed: "+ToString(VpanX)+" ; "+ToString(VpanY));
    #endif
}

//---------------------------------------------------------------------------
void __fastcall cls_MPlot::PhysicPan(System::TObject* Sender)
{ // Pan according to speed given by mouse displacement respect P_clk

// . . . Local variables
    // Coasting
    static double t; // Time [s]
    static double dt; // Time interval [s]

// . . . Update interval
    dt = double(timPan->Interval) / 1E3;

// . . . Actions
    if ( CurrentFrame ) // IsOverFrame
       {
        switch ( PanStatus )
         {
          case PS_COASTING: // Do coasting motion
            // . . . Local variables
            static double vx, vy; // Coasting speed [pix/s]
            static double TransientFactor;
            // . . . Calculations
			TransientFactor = std::exp(-t/CoastingTau);
            vx = VpanX * TransientFactor;
            vy = VpanY * TransientFactor;
            // Check for end coasting motion
            if ( Slow || (std::fabs(vx)<1 && std::fabs(vy)<1) )
                 { // Coasting ended
                  PanStatus = PS_NONE;
                 }
            else {
                  // Do coasting
                  Pan( mat::round(vx*dt), mat::round(vy*dt) );
                  t += dt; // Update time
                 }
          break;

          case PS_PULLING: // Pull

            // . . . Pan speed measuring
            MeasurePanSpeed();

            // Recalculate frame transformation
            Pan( mat::round( ContinuousPan * double(P_cur.x - P_clk.x) ),
                 mat::round( ContinuousPan * double(P_cur.y - P_clk.y) ) );
            // Just some feedback
            // Drawer->EraseDirection();
            Drawer->DrawDirection(P_clk,P_cur);
          break;

          case PS_RELEASING: // Start free coasting motion

            // Reset time
            t = 0;
            // What to do
            if ( !mat::IsZero(CoastingTau) ) PanStatus = PS_COASTING;
            else PanStatus = PS_NONE;
          break;

          default: // Deactivate
            timPan->Enabled = false;
         }
       }

// . . . Other things
    // Self regulation of pan timer interval [ms]
    int newinterval = mat::max(30,mat::round(1E3*RefreshTime));
    // Change only if difference is significative
    if ( std::abs(int(newinterval - timPan->Interval)) > 10 ) timPan->Interval = newinterval;
}


//=============================== End of File ===============================


