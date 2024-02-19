#include <vcl.h>
#include "unt_MatCommon.h"
#include "unt_Log.h"
#pragma hdrstop

#include "unt_MPlot.h" // For 'CurrentFoil'
#include "unt_MPlotClasses.h"
//#include "unt_Tracks.h"
#include "unt_Frames.h"
#include "unt_DataColumns.h"
#include "unt_StreamParser.h" // for 'cls_Columns::' in 'AddTracks'
//#include <algorithm> // for 'std::sort'
#include "unt_PropertiesInterface.h" // To instantiate interface
#pragma package(smart_init)
//---------------------------------------------------------------------------



//========================== File scope variables ===========================
// TODO 3: zoom abscissa/ordinate only
// TODO 5: special y scale: rescale track to frame dimension (digital signals)
// TODO 5: use 'cls_Quantities' for displaying values



///////////////////////////////// cls_Frame /////////////////////////////////

//============================= Static Members ==============================

        int cls_Frame::Number = 0; // Number of instantiated frames

        // Default mapped space
        const double cls_Frame::xmin_def = 0.1;
        const double cls_Frame::xrange_def = 10;
        const double cls_Frame::ymin_def = 0.1;
        const double cls_Frame::yrange_def = 10;

        // Physical space limits
        //const double cls_Frame::Zmin_lin = -E+308;
        //const double cls_Frame::Zmax_lin = +E+308;
        //const double cls_Frame::Zmin_log = log10arg_min;
        //const double cls_Frame::Zmax_log = mat::SupDouble;

        // Styles
        bool cls_Frame::PlotAlsoOnMargins = false;
        bool cls_Frame::CenterTitle = false;
        int cls_Frame::TitleMag = 110;
        int cls_Frame::DefaultMarginX = 10;
        int cls_Frame::DefaultMarginY = 10;
        int cls_Frame::DefaultBorder = 1;
        TColor cls_Frame::DefaultColor = clYellow;

        cls_FoilObject::typ_ActionIdxs cls_Frame::ActionsIdxs; // Used to build popup menu


//============================= Public methods ==============================

//---------------------------------------------------------------------------
cls_Frame::cls_Frame( class cls_MPlot* const mp ) : Owner(mp)
{ // Constructor

// . . . Inherited
    LineStyle = psSolid;
    AreaStyle = bsSolid;
    // see 'ApplyOwnerOptions()'

// . . . Properties
    // Main
    i_Title;
    // State
    i_CurrentTrackIt = tracks.end();
    // Style
    // see 'ApplyOwnerOptions()'
    // Scales
    i_yrange = yrange_def;
    i_xrange = xrange_def;
    i_ymin = ymin_def;
    i_xmin = xmin_def;
    i_YLogarithmic = false;
    i_XLogarithmic = false;
    i_SampPerPix = 1;
    // Tracks collection
    i_LongestTrackName = "";
    // Auxiliary
    i_Row = 0;
    i_Tag = ++Number;

// . . . Other initializations
    Kx = Ky = 1;
    Xo = Yo = 0;
    Xfltprc=Yfltprc=12; Xfltdgt=Yfltdgt=3;
    w = 0; h = 0; // to detect dimensions modification

// . . . Frame objects
    Axes = new cls_Axes(this);
    Grid = new cls_Grid(this);
    Legend = new cls_Legend(this);
    // Markers pointers are in vector 'Markers'
    // Labels pointers are in vector 'Labels'
    //Labels.Add(this, "ciao", 10, 5.6);

// . . . Other operations
    ApplyOwnerOptions(); // Apply owner options
}

//---------------------------------------------------------------------------
cls_Frame::~cls_Frame()
{ // Destructor

// . . . Maintain IDs
        //--Number;

// . . . Free objects
        ClearTracks(); // Clear contained Tracks
        delete Legend;
        delete Grid;
        delete Axes;
}
//------------------------------- Main services -----------------------------


//---------------------------------------------------------------------------
void __fastcall cls_Frame::Draw (class cls_Foil* Foil)
{ // Inherited method Draw (paint on parent's Canvas)

// . . . Rescale lengths according to Foil
    RecalculateMargins(Foil->PointsPerPix);

// . . . Check
    if ( width<1 || height<1 ) return;

// . . . Set clip region
    HRGN ClipRegion;
    ClipRegion = ::CreateRectRgn(Left,Top,Right,Bottom);
    ::SelectClipRgn(Canvas->Handle,ClipRegion);

// . . . Background and border
    // . . . Background
    if ( this==Owner->CurrentFrame && Owner->MarkCurrentFrame )
       { // Frame selection
        //Canvas->Pen->Color = mat::Darken(LineColor,5);
        Canvas->Pen->Color = Axes->LineColor;
        //Canvas->Brush->Color = mat::Darken(AreaColor,-1);
       }
    if ( Owner->NoBackground || AreaColor==Font->Color ) Canvas->Brush->Style = bsClear;
    // Draw
    //Canvas->FrameRect(BoundsRect);
    //Canvas->FillRect(BoundsRect);
    Canvas->Rectangle(BoundsRect);

    // . . . Borders
    Canvas->Pen->Color = LineColor;
    Canvas->Brush->Style = bsClear;
    // Internal Plot Area
    Canvas->Rectangle(boundsRect);

// . . . Title
    // Set local settings
    int prev_size, new_size;
    prev_size = Canvas->Font->Size;
    Canvas->Font->Style = TFontStyles() << fsBold;
    new_size = (prev_size * TitleMag ) / 100;
    if (new_size >= Margin) new_size = Margin; // Limit into margin
    Canvas->Font->Size = new_size;
    // Calculate position
    int tmp = (CenterTitle) ? Left+(Width-Canvas->TextWidth(Title))/2 : left;
    Canvas->TextOut(tmp,Top+1,Title);
    // Restore previous settings
    Canvas->Font->Size = prev_size;
    Canvas->Font->Style = TFontStyles() >> fsBold;

// . . . Detect Frame pixels dimension modification
    if ( w != width ) // Frame width is changed
       {
        RecalculateTransformX(); // Recalculate abscissa scale
        w = width; // Store new dimension
       }

    // ordinate
    if ( h != height ) // Frame height is changed
       {
        RecalculateTransformY(); // Recalculate ordinate scale
        h = height; // Store new dimension
       }

// . . . Grid lines and axes (order means layers position)
    Axes->Paint(Foil);
    Grid->Paint(Foil);

// . . . Plot owned tracks
    // TODO: do track remove request here
    for( rtr=tracks.rbegin(); rtr!=tracks.rend(); ++rtr ) (*rtr)->Paint(Foil);

// . . . Markers
    Markers.Paint(Foil);

// . . . Labels
    Labels.Paint(Foil);

// . . . Legend
    Legend->Paint(Foil);

// . . . Reset clip region
    ::SelectClipRgn(Canvas->Handle,NULL);
    ::DeleteObject(ClipRegion);

// . . . Debug
/*  #ifdef FRM_DEBUG
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
      if (CurrentTrack) Canvas->TextOut(px,py+120,"track: " + CurrentTrack->Name);
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
    #endif  */
}


//---------------------------------------------------------------------------
void __fastcall cls_Frame::Reset()
{ // Inherited method Reset (reset operations (toggling Visible))

        Owner->PlaceFrames();
        Axes->Reset();
        Grid->Reset();
        Legend->Reset();
}

//---------------------------------------------------------------------------
// Create the edit properties interface
cls_PropertiesInterface* cls_Frame::CreateSpecInterface(class TWinControl* p)
{
    return new cls_FrameEdit(p,this);
}

//---------------------------------------------------------------------------
// Get a enum type (deprecated)
nms_MPlotClasses::en_FoilObjectType cls_Frame::GetType()
{
    return nms_MPlotClasses::OB_FRAME;
}


//-------------------------- Space transformations --------------------------

//---------------------------------------------------------------------------
void cls_Frame::AssignScale (const cls_Frame* frameptr)
{ // Assign a frame scale

    AssignScaleX (frameptr);
    AssignScaleY (frameptr);
}

//---------------------------------------------------------------------------
void cls_Frame::AssignScaleX (const cls_Frame* frameptr)
{ // // Assign a frame X scale

    if ( frameptr ) SetMappedX(frameptr->xmin,frameptr->xrange);
    else SetMappedX(xmin_def,xrange_def);
}

//---------------------------------------------------------------------------
void cls_Frame::AssignScaleY (const cls_Frame* frameptr)
{ // Assign a frame Y scale

    if ( frameptr ) SetMappedY(frameptr->ymin,frameptr->yrange);
    else SetMappedY(ymin_def,yrange_def);
}

//---------------------------------------------------------------------------
void cls_Frame::FitScale( const bool axesalso )
{ // Fit scale to all contained tracks

// . . . See all tracks
    mat::stu_Rect r;
    GetTracksSpace(r);
    if ( mat::IsZero(r.right-r.left) )
       {
        r.left = r.left - xrange_def/2;
        r.right = r.left + xrange_def;
       }
    if ( mat::IsZero(r.bottom-r.top) )
       {
        r.top = r.top - yrange_def/2;
        r.bottom = r.top + yrange_def;
       }

// . . . Include axes if requested
    if ( axesalso )
       {
        double xo_pos = i_XLogarithmic ? 1.0 : 0.0;
        double yo_pos = i_YLogarithmic ? 1.0 : 0.0;
        r.left = mat::min(xo_pos,r.left);
        r.right = mat::max(xo_pos,r.right);
        r.top = mat::min(yo_pos,r.top);
        r.bottom = mat::max(yo_pos,r.bottom);
       }

// . . . Correct in case of logarithmic
    if ( i_XLogarithmic )
       {
        if ( r.left < mat::log_argmin ) r.left = mat::log_argmin;
        if ( r.right <= r.left  ) r.right = r.left + mat::log_argmin;
       }
    if ( i_YLogarithmic )
       {
        if ( r.top < mat::log_argmin ) r.top = mat::log_argmin;
        if ( r.bottom <= r.top  ) r.bottom = r.top + mat::log_argmin;
       }

// . . . Finally set scale
    SetMappedX(r.left, r.right-r.left);
    SetMappedY(r.top, r.bottom-r.top);
}

//---------------------------------------------------------------------------
void cls_Frame::FitScale (const cls_Track* trackptr)
{ // Adapt scales to track space occupation

    FitScaleX (trackptr);
    FitScaleY (trackptr);
}

//---------------------------------------------------------------------------
void cls_Frame::FitScaleX (const cls_Track* trackptr)
{ // Adapt X scale to track x space occupation

// . . . See space
    double mn, rng;
    if ( trackptr )
         {
          mn = trackptr->xmin;
          rng = trackptr->xrange;
         }
    else {
          mn = xmin_def;
          rng = xrange_def;
         }

// . . . Correct in case of logarithmic
    if ( i_XLogarithmic )
       {
        if ( mn < mat::log_argmin ) mn = mat::log_argmin;
        if ( rng <= mn  ) rng = mn + mat::log_argmin;
       }

// . . . Finally
    SetMappedX(mn,rng);
}

//---------------------------------------------------------------------------
void cls_Frame::FitScaleY (const cls_Track* trackptr)
{ // Adapt Y scale to track y space occupation
// . . . See space
    double mn, rng;
    if ( trackptr )
         {
          mn = trackptr->ymin;
          rng = trackptr->yrange;
         }
    else {
          mn = ymin_def;
          rng = yrange_def;
         }

// . . . Correct in case of logarithmic
    if ( i_YLogarithmic )
       {
        if ( mn < mat::log_argmin ) mn = mat::log_argmin;
        if ( rng <= mn  ) rng = mn + mat::log_argmin;
       }

// . . . Finally
    SetMappedY(mn,rng);
}

//---------------------------------------------------------------------------
void cls_Frame::ResetOriginX()
{ // Adapt space to view x axis

    SetTransformX(Kx,0);
}

//---------------------------------------------------------------------------
void cls_Frame::ResetOriginY()
{ // Adapt space to view y axis

    SetTransformY(Ky,0);
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

    // TODO 2: do a range control here!!!
    // se xmin<min o xmax>max non fare niente

    if ( XLogarithmic )
         { // TODO 5: logarithmic changes range also
          //SetTransformX (Kx, Xo+DX);
          Xo += DX;
          SetMappedX (Xphy(left), Xphy(right)-Xphy(left));
         }
    else { // linear
          Xo += DX;
          // faster way ('xrange' remains constant)
          i_xmin = Xphy(left);
          // safer way: SetMappedX (Xphy(left), xrange);
         }

    if ( YLogarithmic )
         { // TODO 5: logarithmic changes range also
          //SetTransformY (Ky, Yo-DY);
          Yo -= DY;
          SetMappedY (Yphy(bottom), Yphy(top)-Yphy(bottom));
         }
    else { // linear
          Yo -= DY;
          // faster way ('yrange' remains constant)
          i_ymin = Yphy(bottom);
          // safer way: SetMappedY (Yphy(bottom), yrange);
         }
}

//---------------------------------------------------------------------------
void cls_Frame::Zoom ( const TRect& ZR )
{ // Modify space-transformation to map a new space

    // TODO 2: do a range control here!!!
    // se k<min o k>max non fare niente

    TRect R = mat::Normalize(ZR);

    SetMappedX (Xphy(R.left), Xphy(R.right)-Xphy(R.left));
    SetMappedY (Yphy(R.bottom), Yphy(R.top)-Yphy(R.bottom));
}



//---------------------------- Tracks collection ----------------------------

//---------------------------------------------------------------------------
void __fastcall cls_Frame::set_CurrentTrack(class cls_Track* t)
{
    if ( t!=0 && t!=CurrentTrack )
       {
        tr = FindTrack(t);
        if ( tr != tracks.end() )
           {
            CurrentTrackIt = tr;
            Owner->Invalidate();
           }
       }
}

//---------------------------------------------------------------------------
void cls_Frame::CheckInvariant()
{ // Maintain coherence

    bool NoSelectedTrack = true;
    i_LongestTrackName = "";

    for ( tr=tracks.begin(); tr!=tracks.end(); ++tr )
        {
         // Becomes false only if encounter 'Selected' pointer
         NoSelectedTrack = ( tr != CurrentTrackIt ) && NoSelectedTrack;

         // Maintain longest name property
         if ( ((*tr)->Name).Length() > i_LongestTrackName.Length() ) i_LongestTrackName = (*tr)->Name;
        }

    // Reset color seed if empty
    if ( Empty() ) i_TrackCurrentColor = cls_Track::DefaultLineColor;

    // Maintain selected track
    if ( NoSelectedTrack ) SelectPrevTrack();
    // Plot layout change event
    if (Owner->OnChange) Owner->OnChange();
}


//---------------------------------------------------------------------------
void cls_Frame::FillStringList( class TStrings* sl ) const
{ // Fill a stringlist with track names and pointers

    for ( ctr=tracks.begin(); ctr!=tracks.end(); ++ctr )
        {
         sl->AddObject( (*ctr)->Name, reinterpret_cast<TObject*>(*ctr) );
        }
}

//---------------------------------------------------------------------------
void cls_Frame::DisplayTrackStatistics()
{ // Display statistics of selected track

    if ( CurrentTrack )
       {
        String s;
        CurrentTrack->Statistics(s);
        mat::MsgInfo( "Overall statistics of track " + CurrentTrack->Name + "\n\n" + s) ;
       }
}

//---------------------------------------------------------------------------
void cls_Frame::GetTracksSpace(mat::stu_Rect& r) const
{ // Get the Rectangle in physical space containing all tracks

// . . . Find max and min coordinates in physical space
    for ( ctr=tracks.begin(); ctr!=tracks.end(); ++ctr )
        {
         if ( ctr == tracks.begin() )
              {
               r.left = (*ctr)->xmin;
               r.right = (*ctr)->xmax;
               r.top = (*ctr)->ymin;
               r.bottom = (*ctr)->ymax;
              }
         else {
               if ( r.left > (*ctr)->xmin ) r.left = (*ctr)->xmin;
               if ( r.right < (*ctr)->xmax ) r.right = (*ctr)->xmax;
               if ( r.top > (*ctr)->ymin ) r.top = (*ctr)->ymin;
               if ( r.bottom < (*ctr)->ymax ) r.bottom = (*ctr)->ymax;
              }
        }
}


//---------------------------------------------------------------------------
void cls_Frame::ReassignTrackColors()
{ // Reset color seed and reassign track colors

    i_TrackCurrentColor = cls_Track::DefaultLineColor;
    for ( tr=tracks.begin(); tr!=tracks.end(); ++tr )
        {
         (*tr)->LineColor = GetNextTrackColor();
         (*tr)->AreaColor = TColor(mat::Darken((*tr)->LineColor,-25));
        }
}

//---------------------------------------------------------------------------
void cls_Frame::SelectNextTrack (const bool fitscale)
{ // Select next track

    // Null 'Selected' pointer if no tracks
    if ( tracks.empty() ) CurrentTrackIt = tracks.end();
    else if ( CurrentTrackIt == tracks.end() ) CurrentTrackIt = tracks.begin();
    else if ( ++CurrentTrackIt == tracks.end() ) CurrentTrackIt = tracks.begin();
    // Fit scale if requested
    if (fitscale) FitScale(CurrentTrack);
}

//---------------------------------------------------------------------------
void cls_Frame::SelectPrevTrack (const bool fitscale)
{ // Select previous track

    // Null 'Selected' pointer if no tracks
    if ( tracks.empty() ) CurrentTrackIt = tracks.end();
    else if ( CurrentTrackIt == tracks.begin() || CurrentTrackIt == tracks.end() ) CurrentTrackIt = --tracks.end(); // rbegin()
    else --CurrentTrackIt;
    // Fit scale if requested
    if (fitscale) FitScale(CurrentTrack);
}

//---------------------------------------------------------------------------
void cls_Frame::SelectFirstTrack()
{ // Select first track
    CurrentTrackIt = tracks.begin();
}

//---------------------------------------------------------------------------
void cls_Frame::SelectLastTrack()
{ // Select last track
    CurrentTrackIt = (tracks.empty()) ? tracks.end() : --tracks.end();
}


//---------------------------------------------------------------------------
bool cls_Frame::InsertTrack ( class cls_Track* trackptr )
{ // Add a track to owned ones, managing name conflicts

    // . . . Checks
    ASSERTING((trackptr),"Attempt to add an invalid track pointer")
    ASSERTING((!ContainsTrack(trackptr)),"Trying to re-add an existing track!")

    // . . . Avoid duplicate names
    static unsigned int k = 2; // duplicate value
    if ( FindTrack(trackptr->Name) != tracks.end() )
         { // Manage name conflicts
          if ( k<=2 ) trackptr->Name += String("_") + String(k++);
          else trackptr->Name = mat::Before(trackptr->Name,"_") + "_" + String(k++);
          return InsertTrack(trackptr); // Call itself recursively
         }
    else { // Can add track, do insert operations
          k = 2; // reset counter

          // Set attributes
          trackptr->Owner = this; // Update track's owner
          trackptr->AssignFont(Font);
          if (trackptr->LineColor == cls_Track::DefaultLineColor)
             {
              trackptr->LineColor = GetNextTrackColor();
              // Set also area color
              trackptr->AreaColor = TColor(mat::Darken(trackptr->LineColor,-25));
             }

          // Insert pointer
          //std::pair<typ_TracksIt, bool> result = tracks.insert( trackptr );
          CurrentTrackIt = tracks.insert( tracks.end(), trackptr );

          // Here could fit to inserted track

          #ifdef FRM_DEBUG
            Owner->Log->SubEntry(String("Assigning track ") + trackptr->Name + String(" to ") + Name);
            Owner->Log->SubEntry(String("xmin: ") + String(trackptr->xmin) );
            Owner->Log->SubEntry(String("xrange: ") + String(trackptr->xrange) );
            Owner->Log->SubEntry(String("dtmin: ") + String(trackptr->dtmin) );
            Owner->Log->SubEntry(String("ymin: ") + String(trackptr->ymin) );
            Owner->Log->SubEntry(String("yrange: ") + String(trackptr->yrange) );
          #endif

          // . . . Maintain coherence
          CheckInvariant();
          return true;
         }
}

//---------------------------------------------------------------------------
bool cls_Frame::InsertTracks ( typ_Tracks* trackptr )
{ // Add a list of tracks

    bool ok = true;
    for ( tr=trackptr->begin(); tr!=trackptr->end(); ++tr )
        {
         ok = ok || InsertTrack( (*tr) ); // Add to frame track-set
        }
    return ok;
}


//---------------------------------------------------------------------------
void cls_Frame::MoveTracks ( cls_Frame* frameptr )
{ // Trasfer all tracks to another frame

    for (tr=tracks.begin(); tr!=tracks.end(); ++tr) MoveTrack(tr,frameptr);
}

//---------------------------------------------------------------------------
void cls_Frame::PutCurrTrackInForeground()
{ // Put current track in foreground

    if ( CurrentTrackIt != tracks.end() )
       {
        //MoveTrack(CurrentTrackIt,tracks.begin());
        // Better shift all tracks to preserve sequence
        // and to allow cycling all tracks
        cls_Track* currtr = *CurrentTrackIt;
        while ( currtr != tracks.front() ) ShiftTracksBw();
        CurrentTrackIt = tracks.begin();
       }
    CheckInvariant();
}


//---------------------------------------------------------------------------
void cls_Frame::RemoveTrack ( typ_TracksIt trackit )
{ // Deallocate and erase a track

    // Memory deallocation
    if ( *trackit ) delete *trackit;

    // List operation
    CurrentTrackIt = tracks.erase( trackit );
    if ( CurrentTrackIt == tracks.end() ) CurrentTrackIt = tracks.begin();

    // Maintain coherence
    CheckInvariant();
}


//---------------------------------------------------------------------------
void cls_Frame::ClearTracks()
{ // Clear all tracks

    // Deallocate memory
    for ( tr=tracks.begin(); tr!=tracks.end(); ++tr ) if ((*tr)) delete (*tr);
    //mat::ViewAddress((*tr));
    tracks.clear();

    // . . . Maintain coherence
    CheckInvariant();
}

//---------------------------------------------------------------------------
void cls_Frame::Crop(const mat::stu_Rect& r)
{ // Crop all tracks abscissa domain

    // . . . For all tracks
    for( tr=tracks.begin(); tr!=tracks.end(); ++tr )
       {
        (*tr)->Crop(r);
        if((*tr)->Size<=0) RemoveTrack(tr);
        /*
        cls_DataFnTrack *t = dynamic_cast<cls_DataFnTrack*>(*tr);
        if(!t)
             {
              // Owner->Log->Warn("Skipping crop of track " + (*tr)->Name + ", not a data track");
              // TODO: convert parametric track to data track
              (*tr)->Crop(r);
              if((*tr)->Size<=0) RemoveTrack(tr);
             }
        else {
              t->Crop();
              if(t->Size<=0) RemoveTrack(tr);
             }
        */
       }
}


//------------------------------ Import-Export ------------------------------

//---------------------------------------------------------------------------
bool cls_Frame::ToText ( const String& pth, const char delm )
{ // Convert frame tracks to text file

// . . . Internal variables
    cls_Columns cols; // Data columns bag

// . . . Fill track bag
    cls_Frame::typ_Tracks tracks;
    // When a valid column delimiter is provided sort tracks by size
    FillTrackBag(&tracks, (delm != mat::InvalidChar) );

// . . . Conversion
    TracksToColumns(&tracks, &cols);

// . . . Save to file operation
    TStringList* f = new TStringList();
    try       {
               cols.ToStrings(f,delm);
               f->SaveToFile(pth);
              }
    __finally {
               delete f;
              }

// . . . Finally
    return true;
}

//---------------------------------------------------------------------------
bool cls_Frame::ToMfile ( const String& pth )
{ // Convert frame tracks to text file

// . . . Internal variables
    cls_Columns cols; // Data columns bag
    // m-file string bags
    String PlotTitle = Name;
    if ( !Title.IsEmpty() ) PlotTitle += String(" (") + Title + String(")");
    String ArrayDef;
    String PlotArgs = "";
    String PlotLegend = "";
    String FunctName = String("Load") + Name;
    String DataName = mat::ForceToBeIdentifier(Name);
    String ColName, ColName_x;

// . . . Fill track bag
    cls_Frame::typ_Tracks tracks;
    FillTrackBag( &tracks );

// . . . Conversion
    TracksToColumns(&tracks, &cols);

// . . . Save to file operation
    TStringList* f = new TStringList();
    try       {
               // . . . Start to fill lines
               f->BeginUpdate();
               f->Add("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");
               f->Add("%");
               f->Add("% " + Application->Title + " auto-generated m-file (" + ::Date().FormatString("dd.mm.yyyy") +String(" - ")+ ::Time().FormatString("hh':'nn':'ss'.'zzz") +String(")"));
               f->Add("% This function loads in workspace " + Name + " tracks data");
               if (!Title.IsEmpty()) f->Add("% Frame title is: " + Title);
               f->Add("% Input: a figure handle, put 0 to disable plot");
               f->Add("% Output: a struct containing data");
               f->Add("%");
               f->Add("% Usage:");
               f->Add("% >>" + DataName + " = " + FunctName + "();");
               f->Add("% Specifying an existing figure handle for plot:");
               f->Add("% >>" + DataName + " = " + FunctName + "(2);");
               f->Add("% To disable plot:");
               f->Add("% >>" + DataName + " = " + FunctName + "(0);");
               f->Add("% Access data through struct syntax \'" + DataName + ".fieldname\'");
               f->Add("% >> myvar = " + DataName + ".zz;");
               f->Add("% >> plot(" + DataName + ".xx, " + DataName + ".yy" + ");");
               f->Add("% To see actual available fields, just type:");
               f->Add("% >> " + DataName);
               f->Add("%");
               f->Add("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");
               f->Add("function " + DataName + " = " + FunctName + "(figID)");
               f->Add("");
               f->Add("%----------------------------------------------------------");
               f->Add("% -- Input arguments, default values");
               f->Add(String("if ( nargin < 1 ) figID = ") + String(Tag) + ";");
               f->Add("elseif ( nargin > 1 )");
               f->Add("    disp(\'Usage: " + DataName + " = " + FunctName + "(figID)\');");
               f->Add("    return");
               f->Add("end");
               f->Add("");
               f->Add("%----------------------------------------------------------");
               f->Add("% -- Data section");
               f->Add("disp(\'Loading " + Application->Title + " " + Name + " datas...\')");
               // . . . Build data part
               // Iterate all columns
               for ( cls_Columns::typ_it c=cols.begin(); c!=cols.end(); ++c )
                   {
                    // Set columns labels (name and indep var label)
                    ColName = DataName + "." + mat::ForceToBeIdentifier((*c)->Label);
                    ColName_x = DataName + "." + mat::ForceToBeIdentifier((*c)->IndepVar.Label);
                    if ( ColName_x.IsEmpty() ) ColName_x = ColName + String("_x");

                    // Fill data part
                    f->Add("");
                    if ( (*c)->IsIndepVar() )
                         { // An independent variable data column
                          f->Add("% This is an independent variable " + (*c)->Scaling.GetUnitString());
                         }
                    else { // A dependent variable
                          // Resolve eventual self contained independent variable infos
                          if ( (*c)->IndepVar.IsSelfContained )
                               { // Must introduce a further variable for col indep var

                                // Rather than 'ColName_x' for now I use:
                                ColName_x = ColName + "_x";

                                // Build the eavenly spaced array
                                f->Add(String("% This is the independent variable of ") + ColName + String(" ") + (*c)->IndepVar.Scaling.GetUnitString());
                                f->Add( ColName_x + String(" = ") + mat::BuildMatlabArrayDef((*c)->IndepVar.Scaling.Offset, (*c)->IndepVar.Scaling.Scale, (*c)->IndepVar.Scaling.Offset + ((*c)->IndepVar.Scaling.Scale * ((*c)->size()-1))) + String("';"));
                               }
                          else { // This dependent column is linked to an existing indep var
                               }

                          f->Add("% This is the track " + (*c)->Label + String(" ") + (*c)->Scaling.GetUnitString() + " linked to " + (*c)->IndepVar.Label);

                          // Prepare also plot argument
                          PlotArgs += ColName_x + String(",") + ColName + String(",");
                          PlotLegend += String("\'") + (*c)->Label + String("\',");
                         }
                    // Common part: column data
                    ArrayDef = ColName + String(" = ");
                    (*c)->BuildArrayDef (ArrayDef, ';');
                    ArrayDef += String(";");
                    f->Add(ArrayDef);
                   }
               // Final operations on auxiliary strings
               mat::DropLastChar(PlotArgs);
               mat::DropLastChar(PlotLegend);
               // . . . Continue to fill lines
               f->Add("");
               f->Add("disp(\'...Done\')");
               f->Add("");
               f->Add("%----------------------------------------------------------");
               f->Add("% -- Plot section");
               f->Add("if ( figID > 0 )");
               f->Add("    disp(\'Preparing plots...\')");
               f->Add("    figure(figID);");
               f->Add("    grid on;");
               f->Add("    hold on;");
               f->Add("    plot(" + PlotArgs + ");");
               f->Add("    h = legend(" + PlotLegend + ");");
               f->Add("    set(h,\'Interpreter\',\'none\');");
               f->Add("    h = title(\'" + PlotTitle + "\');");
               f->Add("    set(h,\'Interpreter\',\'none\');");
               if (!Axes->Xlabel.IsEmpty()) f->Add("    xlabel(\'" + Axes->Xlabel + "\');");
               if (!Axes->Ylabel.IsEmpty()) f->Add("    ylabel(\'" + Axes->Ylabel + "\');");
               f->Add("    hold off;");
               f->Add("    disp(\'...Done\')");
               f->Add("end");

               // . . . Stringlist complete
               f->EndUpdate();
               //cols.ToStrings(f,delm);
               f->SaveToFile(pth);
              }
    __finally {
               delete f;
              }
// . . . Finally
    return true;
}


//---------------------------------------------------------------------------
// Convert a set of tracks into a set of data columns
void cls_Frame::TracksToColumns ( typ_Tracks* trks, class cls_Columns* cols )
{

// . . . Preparation
    cols->reserve( 2 * trks->size() );

// . . . Overall infos
    // . . . Frame title and axes labels
    cols->Title = Title;
    cols->Xlabel = Axes->Xlabel;
    cols->Ylabel = Axes->Ylabel;
    // . . . Value markers
    for (cls_Markers::typ_MarkersConstIt m=Markers.begin(); m!=Markers.end(); ++m)
        {
         if ( (*m)->Visible ) // Exclude measuring makers, normally hidden
            {
             cls_Columns::stu_Marker M;
             M.Value = (*m)->Value;
             M.Vertical = (*m)->Vertical;
             cols->Markers.push_back(M);
            }
        }
    // . . . Text labels
    for (cls_Labels::typ_LabelsConstIt l=Labels.begin(); l!=Labels.end(); ++l)
        {
         if ( (*l)->Visible ) // Exclude hidden labels
            {
             cls_Columns::stu_Label L;
             L.x = (*l)->x;
             L.y = (*l)->y;
             L.Text = (*l)->Text;
             if ( (*l)->ShowCoords ) L.Text = String("+") + L.Text;
             cols->Labels.push_back(L);
            }
        }

// . . . For each track analyze independent and dependent vars
    for ( typ_TracksIt tr=trks->begin(); tr!=trks->end(); ++tr )
        {
         // . . . Dependent variable
         // TODO 4: cplx track TYP_REALPART,TYP_IMGPART
         // Allocate column
         cls_Column* ycol = new cls_Column( (*tr)->Size);

         // Attributes
         ycol->Label = dcol::PutBracketsEscapeChar( (*tr)->Name );
         // Check if already exists a column having this name...
         for ( cls_Columns::typ_it c=cols->begin(); c!=cols->end(); ++c )
             {
              if ( (*c)->Label == ycol->Label )
                 {
                  ycol->Label += "_";
                  c = cols->begin();
                  continue;
                 }
             }

         ycol->IndepVar.Label = (*tr)->XName;
         ycol->Type = cls_ColumnDeclaration::TYP_DEPVAR;
         ycol->Scaling.Unit = (*tr)->YQuantity.UnitLabel; // Default values for other members of mat::cls_ScaleUnitOffset;
         // Data
         (*tr)->CopyDepVarTo( ycol );
         // Remains to define 'st_Abscissa' attribute

         // . . . Independent variable
         // > Is arithmetic progression?
         double k, o; // scale, offset
         (*tr)->GetIndepVarProgression(&k,&o);

         // > Is the same of an already processed track?
         String tlbl = ""; // label of same independent var column
         for ( cls_Columns::typ_it c=cols->begin(); c!=cols->end(); ++c )
             {
              if ( (*c)->Type == cls_ColumnDeclaration::TYP_INDEPVAR )
                if ( (*tr)->SameIndepVar(*c) )
                   {
                    tlbl = (*c)->Label;
                    break;
                   }
             }

         // What to do
         if ( k )
              { // Got an arithmetic progression
               ycol->IndepVar.IsSelfContained = true;
               ycol->IndepVar.Scaling.Scale = k;
               ycol->IndepVar.Scaling.Offset = o;
               ycol->IndepVar.Scaling.Unit = (*tr)->XQuantity.UnitLabel;
              }
         else if ( tlbl.Length() > 0 )
              { // Indep var column already exists
               ycol->IndepVar.IsSelfContained = false;
               ycol->IndepVar.Label = tlbl;
              }
         else { // A new independent variable
               cols->push_back( new cls_Column( (*tr)->Size) ); // will be deleted by 'cols'
               // Attributes
               cols->back()->Label = ((*tr)->XName.IsEmpty()) ? (*tr)->Name + "_x" : (*tr)->XName;
               cols->back()->Type = cls_ColumnDeclaration::TYP_INDEPVAR;
               cols->back()->Scaling.Unit = (*tr)->XQuantity.UnitLabel;
               // Now can set link
               ycol->IndepVar.Label = cols->back()->Label;
               ycol->IndepVar.IsSelfContained = false;
               // Data
               (*tr)->CopyIndepVarTo( cols->back() );
              }

         // . . . Finally
         // Push dependent variable column
         cols->push_back( ycol ); // will be deleted by 'cols'
        }
}


//---------------------------------------------------------------------------
bool cls_Frame::ImportColumns ( class cls_Columns* cols )
{ // Read tracks from data columns struct

// . . . First of all
    bool jobdone = false;
    bool WasEmpty = Empty();

// . . . Overall infos
    // . . . Frame title and axes labels
    if ( Title.IsEmpty() ) Title = cols->Title;
    if ( Axes->Xlabel.IsEmpty() ) Axes->Xlabel = cols->Xlabel;
    if ( Axes->Ylabel.IsEmpty() ) Axes->Ylabel = cols->Ylabel;
    // . . . Value markers
    for (cls_Columns::typ_MarkersConstIt m=cols->Markers.begin(); m!=cols->Markers.end(); ++m)
        {
         Markers.Add(this,m->Value,m->Vertical);
        }
    // . . . Text labels
    for (cls_Columns::typ_LabelsConstIt l=cols->Labels.begin(); l!=cols->Labels.end(); ++l)
        {
         // Detect 'ShowCoords' char
         String Text = l->Text.Trim();
         bool MustShowCoords = (Text[1] == '+');
         if ( MustShowCoords ) Text = mat::After(Text,1); // Remove char
         Labels.Add(this,Text,l->x,l->y,MustShowCoords);
        }

// . . . Build tracks from data columns
    for ( cls_Columns::typ_it c=cols->begin(); c!=cols->end(); ++c )
        {
         if ( !(*c)->IsIndepVar() )
            {
             cls_Track* newtr = 0;
             if ( (*c)->IsOrdinate() )
                {
                 // Remember the column attributes:
                 //(*c)->{Label,Abscissa.(Label|Index|Scale|Unit|Offset),Unit,Scale,Offset,Type}
                 // Get column
                 cls_Column* yc = (*c);
                 // . . . Create the new track
                 cls_DataFnTrack* dattr = new cls_DataFnTrack ( yc->Label );
                 // TODO 2: unit for tracks's cls_Quantity (y and x)
                 dattr->YQuantity.UnitLabel = yc->Scaling.Unit;

                 // . . . Populate points
                 // TODO 4: Progress here?
                 // if ( !yc->IndepVar.IsSelfContained && yc->IndepVar.Index < 0 ) something wrong
                 if ( yc->IndepVar.Index < 0 )
                      { // Indexed ordinate
                       dattr->XQuantity.UnitLabel = yc->IndepVar.Scaling.Unit;
                       dattr->XName = yc->IndepVar.Label;
                       for ( unsigned int i=0; i<(*c)->size(); ++i ) dattr->AddPoint( yc->TransformIndex(i), (*yc)[i] );
                      }
                 else { // Linked ordinate
                       cls_Column* xc = cols->at( yc->IndepVar.Index ); // Get the abscissa column
                       dattr->XQuantity.UnitLabel = xc->Scaling.Unit;
                       dattr->XName = xc->Label; // Equal to yc->Abscissa.Label;
                       for ( unsigned int i=0; i<yc->size() && i<xc->size(); ++i ) dattr->AddPoint( (*xc)[i], (*yc)[i] );
                      }
                 newtr = dattr;
                }
             else if ( (*c)->IsRealPart() )
                  {
                   // TODO 5: deal with polar tracks
                   Owner->Log->Warn("Complex tracks not yet supported, skipping");
                   //cls_PolarTrack* cplxtr = new cls_PolarTrack();
                   // Remember that index of real part links to img part, whose index links to abscissa
                   //newtr = cplxtr;
                  }
             //else if ( (*c)->IsImgPart() )

             // . . . A track was created
             if ( newtr )
                {
                 // . . . Assign some other attributes

                 // . . . Add Track to Frame
                 InsertTrack( newtr );
                }
            }// end 'is not abscissa'
        }

// . . . Finally
    SelectFirstTrack();
    // Don't rescale if frame is not empty
    if ( WasEmpty ) FitScale(true);
    jobdone = true; // Job done
    return jobdone;
}

//---------------------------------------------------------------------------
void cls_Frame::FillTrackBag ( typ_Tracks* trs, const bool sorted ) const
{ // Fill a track bag with owned tracks sorted by size

// . . . Get tracks
    for ( ctr=tracks.begin(); ctr!=tracks.end(); ++ctr ) trs->push_back( *ctr );

    // TODO 2: sort by size
    //if (sorted) std::sort(trs->begin(), trs->end(), cls_CompareSize_Track() );
}



//------------------------------ Other services -----------------------------

//---------------------------------------------------------------------------
void cls_Frame::ApplyOwnerOptions()
{ // Apply owner options

// . . . Frame
    PixMarginX = DefaultMarginX;
    PixMarginY = DefaultMarginY;
    LineWeight = DefaultBorder; // Border
    AreaColor = DefaultColor;
    LineColor = TColor(mat::Darken(AreaColor,20)); // Border color
    AssignFont(Owner->TextFont);
    i_SampPerPix = Owner->SampPerPix;
    i_TrackCurrentColor = cls_Track::DefaultLineColor;

// . . . Axes
    Axes->LineColor = cls_Axes::DefaultColor;
    Axes->AreaColor = Axes->LineColor;
    Axes->AssignFont(Font);

// . . . Grid
    Grid->LineColor = cls_Grid::DefaultColor; // LineColor
    Grid->LineStyle = cls_Grid::DefaultLinesStyle;
    Grid->VGridDist = cls_Grid::DefaultVGridDist;
    Grid->HGridDist = cls_Grid::DefaultHGridDist;
    // Warning: the following resets current divs!
    Grid->VGridDivs = cls_Grid::DefaultVGridDivs;
    Grid->HGridDivs = cls_Grid::DefaultHGridDivs;
    mat::StringToTFont(cls_Grid::DefaultFont,Grid->Font);
    Grid->AssignFont(Grid->Font);

// . . . Legend
    Legend->LineWeight = LineWeight;
    Legend->LineColor = LineColor;
    Legend->AreaColor = cls_Legend::DefaultColor;
    Legend->AreaStyle = (Legend->AreaColor==Font->Color)? bsClear : bsSolid;
    Legend->PixMargin = cls_Legend::DefaultSpacing;
    Legend->StrokeLength = cls_Legend::DefaultStrokeLength;
    Legend->AssignFont(Font);

// . . . Markers
    for (cls_Markers::typ_MarkersConstIt m=Markers.begin(); m!=Markers.end(); ++m)
        {
         (*m)->LineWeight = cls_Marker::DefaultLineWeight;
         (*m)->LineColor = cls_Marker::DefaultLineColor;
         (*m)->LineStyle = cls_Marker::DefaultLineStyle;
        }

// . . . Labels
    for (cls_Labels::typ_LabelsConstIt l=Labels.begin(); l!=Labels.end(); ++l)
        {
		 (*l)->AssignFont(Font);
        }

// . . . Existing tracks
    for ( tr=tracks.begin(); tr!=tracks.end(); ++tr )
        {
         (*tr)->AssignFont(Font);
         //(*tr)->LineWeight = cls_Track::DefaultLineWeight;
        }
}



//=============================== Properties ================================



//---------------------------------------------------------------------------
void cls_Frame::set_YLogarithmic (const bool b)
{ // Set y logarithmic scale

    i_YLogarithmic = b;
    if ( YLogarithmic )
         {
          if ( i_ymin < mat::log_argmin ) i_ymin = mat::log_argmin; // Logarithmic scale domain check
         }

    RecalculateTransformY();
}

//---------------------------------------------------------------------------
void cls_Frame::set_XLogarithmic (const bool b)
{ // Set x logarithmic scale

    i_XLogarithmic = b;
    if ( XLogarithmic )
         {
          if ( i_xmin < mat::log_argmin ) i_xmin = mat::log_argmin; // Logarithmic scale domain check
         }

    RecalculateTransformX();
}





//============================= Private methods =============================


//---------------------------- Tracks collection ----------------------------

//---------------------------------------------------------------------------
void cls_Frame::MoveTrack( typ_TracksIt src, typ_TracksIt dst )
{ // Move a Track before a destination

    if ( src != tracks.end() && dst != tracks.end() )
       {
        if ( dst == --tracks.end() ) CurrentTrackIt = tracks.insert( tracks.end(), *src ); // queue back if on last
        else CurrentTrackIt = tracks.insert( dst, *src ); // insert before
        /*CurrentTrackIt =*/ tracks.erase( src );
        CheckInvariant();
       }
}

//---------------------------------------------------------------------------
void cls_Frame::MoveTrack ( typ_TracksIt trackit, cls_Frame* frameptr )
{ // Trasfer track to another frame

    if ( trackit != tracks.end() && frameptr != this )
       {
        if ( frameptr->InsertTrack( *trackit ) )
           {
            // Remove track pointer from this collection (no deallocation)
            CurrentTrackIt = tracks.erase( trackit );
            CheckInvariant();
           }
        else Owner->Log->Error(frameptr->Name + ": unable to insert track");
       }
}

//---------------------------------------------------------------------------
void cls_Frame::ShiftTracksFw()
{ // Shift forward all tracks of one position

// . . . Store last track
    cls_Track* lst;
    rtr = tracks.rbegin();
    if ( rtr != tracks.rend() ) lst = *rtr;
    else return; // No tracks

// . . . Shift operation
    typ_TracksRevIt nxt = rtr; // next slot (reversed previous)
    while ( ++rtr != tracks.rend() )
          {
           *nxt = *rtr; // overwrite next slot
           nxt = rtr; // update next slot
          }
// . . . Shift also last
    *nxt = lst;
}

//---------------------------------------------------------------------------
void cls_Frame::ShiftTracksBw()
{ // Shift backward all tracks of one position

// . . . Store last track
    cls_Track* fst;
    tr = tracks.begin();
    if ( tr != tracks.end() ) fst = *tr;
    else return; // No tracks

// . . . Shift operation
    typ_TracksIt prv = tr; // previous slot
    while ( ++tr != tracks.end() )
          {
           *prv = *tr; // overwrite prev slot
           prv = tr; // update prev slot
          }
// . . . Shift also first
    *prv = fst;
}


//--------------------------------- Scale -----------------------------------

//---------------------------------------------------------------------------
inline void cls_Frame::SetMappedX (const double minval, const double range)
{ // Set mapped x space (range check is unnecessary here)

    ASSERTING((range>0),"x range must be >0!")
// . . . Assignment
    i_xrange = range;
    i_xmin = minval;

// . . . Recalculate tranformation
    RecalculateTransformX(); // Recalculate Y scale
}

//---------------------------------------------------------------------------
inline void cls_Frame::SetMappedY (const double minval, const double range)
{ // Set mapped y space

    ASSERTING((range>0),"y range must be >0!")
// . . . Assignment
    i_yrange = range;
    i_ymin = minval;

// . . . Recalculate tranformation
    RecalculateTransformY(); // Recalculate Y scale
}

//---------------------------------------------------------------------------
inline void cls_Frame::SetTransformX (const double k, const int o)
{ // Set x-axis transformation coeffs

    ASSERTING((Kx>0),"x scale must be >0!")
// . . . Assign
    Kx = k;
    Xo = o;

// . . . Recalculate mapped space
    RecalculateMappedX(); // X space
}

//---------------------------------------------------------------------------
inline void cls_Frame::SetTransformY (const double k, const int o)
{ // Set y-axis transformation coeffs

    ASSERTING((Ky>0),"y scale must be >0!")
// . . . Assign
    Ky = k;
    Yo = o;

// . . . Recalculate mapped space
    RecalculateMappedY(); // Y space
}

//---------------------------------------------------------------------------
inline void cls_Frame::RecalculateMappedX()
{ // Recalculate x-axis mapped space from transformation coeffs and dimension

    i_xmin = Xphy(left);
    i_xrange = Xphy(right) - i_xmin;

// . . . Update also
    Grid->RecalculateVlines(); // Recalculate horizontal grid
    SetXvaluesRepresentation(); // Float precision and digits in string rep
}

//---------------------------------------------------------------------------
inline void cls_Frame::RecalculateMappedY()
{ // Recalculate y-axis mapped space from transformation coeffs and dimension

    i_ymin = Yphy(bottom);
    i_yrange = Yphy(top) - i_ymin;

// . . . Update also
    Grid->RecalculateHlines(); // Recalculate vertical grid
    SetYvaluesRepresentation(); // Float precision and digits in string rep
}


//---------------------------------------------------------------------------
inline void cls_Frame::RecalculateTransformX()
{ // Recalculate x-axis transformation coeffs from mapped space and dimensions

    if ( XLogarithmic )
         { // Logarithmic (X = Kx Log(x) + Xo)
          Kx = CalculateLogarithmicScale((i_xrange+i_xmin)/i_xmin,width);
          Xo = CalculateLogarithmicOrigin(0,i_xmin,Kx);
         }
    else { // Linear (X = Kx x + Xo)
          Kx = CalculateLinearScale(i_xrange,width);
          Xo = CalculateLinearOrigin(0,i_xmin,Kx);
         }

// . . . Update also
    Grid->RecalculateVlines(); // Recalculate vertical grid
    SetXvaluesRepresentation(); // Float precision and digits in string rep
}

//---------------------------------------------------------------------------
inline void cls_Frame::RecalculateTransformY()
{ // Recalculate y-axis transformation coeffs from mapped space and dimensions

    if ( YLogarithmic )
         { // Logarithmic (Y = Ky Log(y) + Yo)
          Ky = CalculateLogarithmicScale((i_yrange+i_ymin)/i_ymin,height);
          Yo = CalculateLogarithmicOrigin(0,i_ymin,Ky);
         }
    else { // Linear (Y = Ky y + Yo)
          Ky = CalculateLinearScale(i_yrange,height);
          Yo = CalculateLinearOrigin(0,i_ymin,Ky);
         }

// . . . Update also
    Grid->RecalculateHlines(); // Recalculate horizontal grid
    SetYvaluesRepresentation(); // Float precision and digits in string rep
}


//---------------------------------------------------------------------------
inline double cls_Frame::CalculateLinearScale (const double range, const int pix)
{ // Calculate axis linear scale to fit a range=max-min

    return double(pix) / range; // K=(pix length)/(max-min)
}

//---------------------------------------------------------------------------
inline int cls_Frame::CalculateLinearOrigin (const int Z, const double z, const double K)
{ // Calculate linear axis pixel origin given two points

    return (Z - mat::round(K*z) ); // Zo = Z - K z
}

//---------------------------------------------------------------------------
inline double cls_Frame::CalculateLogarithmicScale (const double ratio, const int pix)
{ // Calculate axis logarithmic scale to fit a range=max/min

    ASSERTING((ratio>0),"range must be in positive semiaxis")
    ASSERTING((ratio!=1),"range cannot be null")
	return double(pix) / std::log10(ratio); // K = (pix length)/[Log(max)-Log(min)]
}

//---------------------------------------------------------------------------
inline int cls_Frame::CalculateLogarithmicOrigin (const int Z, const double z, const double K)
{ // Calculate logarithmic axis pixel origin given two points

    ASSERTING((z>0),"point must be in positive semiaxis")
    return ( Z - mat::round( K*std::log10(z) ) ); // Zo = Z - K Log(z)
}

//-------------------------------- Auxiliary --------------------------------

//---------------------------------------------------------------------------
TColor cls_Frame::GetNextTrackColor()
{ // Get a color to assign to a new track

    //static const double DH = 123 * (1 + 32.62599469496021220159151193634/360.0); // old value
    //static const double DH = 360*((6 + 0.65)/(6*6)); // (360+h)/N , h<360/N
    static const double DH = 360*((5 + 0.76)/(5*5)); // (360+h)/N , h<360/N
    TColor c = TColor(i_TrackCurrentColor);
    i_TrackCurrentColor.H += DH;
    return c;
}


//=============================== End of File ===============================





