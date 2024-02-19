//---------------------------------------------------------------------------
#ifndef unt_MPlotH
#define unt_MPlotH

        /* * * * * * * * * * * * * * * * * * * * * * * * * * * *
        *
        *                cls_MPlot
        *        2006 - Matteo Gattanini
        *
        *        unt_MPlot ver: 08 (aug.2008)
        *
        *        This unit defines a class derived from
        *        Borland's VCL for data plotting
        *
        *        Example of usage:
        *
        *        #include "unt_MPlot.h"
        *        cls_MPlot* plot;
        *        plot = new cls_MPlot (Panel1,Log);
        *        //... and at the end:
        *        delete plot;
        *
        * * * * * * * * * * * * * * * * * * * * * * * * * * * */

//#include <ComCtrls.hpp> // 'TTreeView*'
//#include "unt_MPlotClasses.h"

/*-------------------------- Geometric attributes ---------------------------
    -----> x
    _________________CANVAS___________________________________
 | |                          Margin                          |
 | |  |¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|  |
 v |  | o--^---------------------------------------------o |  |
   |  | |  |    ...    .Frame           ..               | |  |
 y |  | |  | .      ..  plot area  ..      ..      ..    | |  |
   |M | |  | .               . ....           .....      | | M|
   |a | | -O- - - - - - - - - - - - - - - - - - - - - - -|>| a|
   |r | o------------------------------------------------o | r|
   |g |____________________________________________________| g|
   |i                         Margin                         i|
   |n |¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯| n|
   |  | o------------------------------------------------o |  |
   |  | |      ..         Frame     ....         ...     | |  |
   |  | |     .  ....     plot area     ..  .....   ...  | |  |
   |  | |   .        ...                  ..           . | |  |
   |  | o------------------------------------------------o |  |
   |  |____________________________________________________|  |
   |                          Margin                          |
    ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯*/

//------------------------------- Settings ----------------------------------
#ifdef _DEBUG
  #define PLT_DEBUG // Debug define
#endif

  #define USE_PLOT_GRCTRL
  
//----------------------------- External files ------------------------------
#include <list> // for Frame pointers
#include <vector> // for Frames Layout
#include "unt_Quantities.h" // For time measure quantity
#include "unt_Frames.h" // here for inlining


//--------------------------- Class Definition ------------------------------
// Substantially is a collection of frames, giving an interface to main
// application to control view (through CurrentFrame)
class cls_MPlot :
#ifdef USE_PLOT_GRCTRL
        public TGraphicControl {
#else
        public TCustomControl {
#endif

public: //                                                        .............

// ~ ~ ~ Types definitions
    typedef std::list<class cls_Frame*> typ_Frames;
    typedef typ_Frames::iterator        typ_FramesIt;
    typedef typ_Frames::const_iterator  typ_FramesConstIt;
    typedef std::vector<unsigned int>   typ_Layout;
    typedef typ_Layout::iterator        typ_LayoutIt;
    // enums
    typedef enum { MD_NONE=0, MD_SELECT, MD_PAN, MD_ZOOM, MD_MEASURE, MD_EDIT, MD_PEN } en_Modes; // Plot status modes
    typedef enum { ZN_OUTSIDE, ZN_FRAME, ZN_UPPERBAND, ZN_LEFTBORDER, ZN_RIGHTBORDER, ZN_LOWERBAND } en_Zone; // Plot zones
    typedef enum { PS_NONE=0, PS_DRAGGING, PS_RELEASING, PS_COASTING, PS_PULLING } en_PanStates; // Pan operation status
    // structs
    typedef struct { en_Zone Type; typ_FramesIt Iterator; } stu_PlotZone;
    typedef struct { mat::stu_Rect frm; mpl::stu_PointQuantities trk; } stu_FramePointQuantities;
    typedef union { class cls_Frame* Frame;
                    class cls_Track* Track;
                    class cls_Legend* Legend;
                    class cls_Grid* Grid;
                    class cls_Axes* Axes;
                    class cls_Marker* Marker;
                    class cls_Label* Label;
                    class cls_FoilObject* FoilObject;
                   } uni_FoilObjectPtr;
    // functions
    typedef void __fastcall (__closure *fFastVoid)(); // A plain function
    typedef void __fastcall (__closure *fPopupMenu)(const TPoint&); // A popup show function
    typedef void __fastcall (__closure *fProgress)(int,bool&); // A progress function
    //typedef void __fastcall (__closure *fOutString)(const String&, int); // An out string function

// . . . Public methods
    __fastcall cls_MPlot(Classes::TComponent*, class cls_Log*);
    __fastcall ~cls_MPlot();

    virtual void __fastcall Paint(); // OnPaint event
    inline void Clear() { ClearFrames(); } // Clear all

    // . . . Frames collection
    // Status
    unsigned int FramesCount() {return frames.size();}
    // Creation
    void AssureFrame(); // Choose or create a frame to put new tracks
    cls_Frame* PrepareNewFrame(const TPoint& =TPoint(-1,-1)); // Create a frame given a position in plot
    // Selection
    void SelectNextFrame(); // Select next frame in list
    void SelectPrevFrame(); // Select previous frame in list
    // Modifying
    void MergeFrames(typ_FramesIt,typ_FramesIt); // Merge two frames (in first)
    void MoveFrame(class cls_Frame* fr1, class cls_Frame* fr2) {MoveFrame(FindFrame(fr1), FindFrame(fr2));} // Move a Frame before a destination
    //void SwapFrames(class cls_Frame* fr1, class cls_Frame* fr2) {SwapFrames(FindFrame(fr1), FindFrame(fr2));}
    void RemoveFrame() {RemoveFrame(CurrentFrameIt);}// Remove current frame
    void ClearFrames(); // Clear all frames (deallocation and list emptying)
    // Misc
    void PlaceFrames(); // Arrange Frames position in CurrentFoil Canvas according to Layout
    void ApplyOptions(); // Pass to frames the style options
    void PaintTo(class cls_Frame*,class cls_Foil*); // Paint in a foil
    // Import-Export
    void FillStringList(class TStrings*) const; // Fill a stringlist with frame names and pointers
    void FillTreeView(class TTreeView*,bool =true); // Fill plot object treeview
    bool ToText(const String&, const char =mat::InvalidChar); // Convert all frames to text files
    bool ToMfile(const String&); // Convert all frames to m-files
    bool ToSVG(const String&); // Convert plot to Scalable Vector Graphics

    // Current track services
    void CycleTrack() {if (CurrentFrame) {(*CurrentFrameIt)->SelectNextTrack(true); (*CurrentFrameIt)->PutCurrTrackInForeground();} Invalidate();}
    void SelectFirstTrack() {if (CurrentFrame) (*CurrentFrameIt)->SelectFirstTrack(); Invalidate();}
    void SelectNextTrack_Rescale() {if (CurrentFrame) (*CurrentFrameIt)->SelectNextTrack(true); Invalidate();}
    void SelectPrevTrack_Rescale() {if (CurrentFrame) (*CurrentFrameIt)->SelectPrevTrack(true); Invalidate();}
    void SelectNextTrack() {if (CurrentFrame) (*CurrentFrameIt)->SelectNextTrack(); Invalidate();}
    void SelectPrevTrack() {if (CurrentFrame) (*CurrentFrameIt)->SelectPrevTrack(); Invalidate();}
    void SelectLastTrack() {if (CurrentFrame) (*CurrentFrameIt)->SelectLastTrack(); Invalidate();}
    void CurrentTrack_Remove(); // Remove current track
    void CurrentTrack_ToggleMarkPoints(); // Mark or not points of current track
    void CurrentTrack_ToggleStrokeLines(); // Stroke or not lines of current track
    void CurrentTrack_ToggleFillArea(); // Fill or not area of current track
    void CurrentTrack_ToggleDrawHistograms(); // Draw or not histograms of current track
    void CurrentTrack_ToggleShowCoords(); // Toggle show track point coordinates
    void CurrentTrack_DisplayStatistics(); // Display statistics of clicked/selected track

    // Viewing services
    void ResetOrigin();  // Reset axes origin
    void ToggleLogScaleX(); // Toggle linear/logarithmic scale for x axis
    void ToggleLogScaleY(); // Toggle linear/logarithmic scale for y axis
    void ToggleLegend(); // Toggle legend
    void ToggleGrid(); // Toggle grid
    void ExpandGrid(const int); // Modifying grid lines distance, use a negative value to shrink
    inline void Pan (const int,const int); // Perform a pan operation given displacement
    void DefaultZoom (const bool,const bool,const bool); // Perform a default zoom operation
    void Zoom(const TRect&); // Zoom in Rect
    void FitScale(const bool alltracks =true, const bool axesalso =true); // Reset current frame scale according to tracks
    // Editing services
    void CropTracks(const TRect&); // Crop current frame tracks abscissa domain

    // Other services
    inline String ToString(const double v) const {return mat::isNaN(v) ? mat::NaNstr : ::FloatToStrF(v,ffGeneral,FloatPrecision,FloatDigits);} // Use internal settings to display numbers
    double* __fastcall GetValuePointer (const String&); // Convert a string to its linked value
    void GetCanvasBitmap(TImage*, class cls_Frame* =0); // Draw Canvas in a TImage
    void SaveBitmap(const String&); // Save Canvas bitmap to path
    void Test(); // Arrange some frames and tracks just to test
    void Test2(); // Another test
    void Test_Experiments(); // Another test with experimental stuff
    void DrawRandomShapes(unsigned int); // Draw a random Rectangle
    void DrawRandomCurve(unsigned int); // Draw a random curve
    void TryColorGenerator(); // Try track colors generator

    // . . . Events
    void __fastcall OnKeyUp(TObject*, WORD&, TShiftState); // This is shared between modes
    // External handlers
    __property TKeyEvent OnKeyDown = {read=i_OnKeyDown, write=i_OnKeyDown}; // Used to pass keys from mmain form
    //__property TKeyEvent OnKeyUp = {read=i_OnKeyUp, write=i_OnKeyUp};
    __property fFastVoid OnUpdate = {read=i_OnUpdate, write=i_OnUpdate}; // a state variable changed
    __property fFastVoid OnChange = {read=i_OnChange, write=i_OnChange}; // on changing plot layout
    __property fFastVoid OnPaint = {read=i_OnPaint, write=i_OnPaint}; // just painted
    __property fPopupMenu OnPopupMenu = {read=i_OnPopupMenu, write=i_OnPopupMenu}; // right click handling
    __property TNotifyEvent OnProperties = {read=i_OnProperties, write=i_OnProperties}; // at request on object properties
    __property fProgress OnProgress = {read=i_OnProgress, write=i_OnProgress}; // lengthy task being executed
    //__property fFastVoid OnChange = {read=i_OnChange, write=i_OnChange}; // transform changed

// . . . Properties
    // State
    __property en_Modes Mode = {read=i_Mode, write=set_Mode};
    __property typ_FramesIt CurrentFrameIt = {read=i_CurrentFrameIt, write=i_CurrentFrameIt};
    __property class cls_Frame* CurrentFrame = {read=get_CurrentFrame, write=set_CurrentFrame};
    __property class cls_Track* CurrentTrack = {read=get_CurrentTrack, write=set_CurrentTrack};
    __property class cls_Foil* CurrentFoil = {read=i_CurrentFoil, write=i_CurrentFoil}; // Where is painted
    __property uni_FoilObjectPtr Selected = {read=i_Selected}; // Selected foil object
    // Measures
    __property String SelectedStuff = {read=get_SelectedStuff}; // Get a string with selected stuff
    __property String CurrentPoint = {read=i_CurrentPoint}; // Current point in physical space
    __property String PixelSize = {read=get_PixelSize}; // A Pixel size string
    __property bool Slow = {read=i_Slow, write=i_Slow}; // Refresh time over the threshold
    __property cls_TimeQuantity RefreshTime = {read=i_RefreshTime, write=i_RefreshTime};
    __property unsigned int Repaints = {read=i_Repaints}; // Repainting number
    // Options
    #ifdef INCLUDE_SOUNDS
      __property bool Noisy = {read=i_Noisy, write=i_Noisy}; // Play sounds
    #endif
    __property int SampPerPix = {read=i_SampPerPix, write=i_SampPerPix}; // Undersampling option
    __property unsigned int Timeout = {read=i_Timeout, write=i_Timeout}; // [s] Plot timeout
    __property unsigned int LagThreshold = {read=i_LagThreshold, write=i_LagThreshold}; // [ms] Refresh time slowness threshold
    __property bool LockX = {read=i_LockX, write=set_LockX}; // Lock X axes
    __property bool LockY = {read=i_LockY, write=set_LockY}; // Lock Y axes
    __property int FloatPrecision = {read=i_FloatPrecision, write=i_FloatPrecision}; // Precision of displayed numbers
    __property int FloatDigits = {read=i_FloatDigits, write=i_FloatDigits}; // Digits of displayed numbers
    __property int DefaultPan = {read=i_DefaultPan, write=i_DefaultPan}; // Key Pan default translation
    __property double CoastingTau  = {read=i_CoastingTau, write=i_CoastingTau}; // Frame coasting time constant [s]
    __property double ContinuousPan  = {read=i_ContinuousPan, write=i_ContinuousPan}; // Displacement proportional speed in continuous pan [1/s]
    __property bool SuddenZoom = {read=i_SuddenZoom, write=i_SuddenZoom}; // Zoom immediately after dragging rectangle
    __property int ZoomPerc = {read=i_ZoomPerc, write=i_ZoomPerc}; // [%] Default zoom in/out percentage
    // Style
    __property int PixMargin = {read=i_PixMargin, write=i_PixMargin}; // Geometry
    __property bool NoBackground = {read=i_NoBackground, write=i_NoBackground}; // Don't draw frames background
    __property bool MarkCurrentFrame = {read=i_MarkCurrentFrame, write=i_MarkCurrentFrame}; // Paint differently the CurrentFrame

    // Keys
    __property Word keyPanToggle = {read=i_keyPanToggle, write=i_keyPanToggle};
    __property Word keyZoomToggle = {read=i_keyZoomToggle, write=i_keyZoomToggle};
    __property Word keyReset = {read=i_keyReset, write=i_keyReset};
    __property Word keyDo = {read=i_keyDo, write=i_keyDo};
    __property Word keyCycle = {read=i_keyCycle, write=i_keyCycle};
    __property Word keyPrior = {read=i_keyPrior, write=i_keyPrior};
    __property Word keyNext = {read=i_keyNext, write=i_keyNext};
    __property Word keyMinus = {read=i_keyMinus, write=i_keyMinus};
    __property Word keyPlus = {read=i_keyPlus, write=i_keyPlus};
    __property Word keyStart = {read=i_keyStart, write=i_keyStart};
    __property Word keyEnd = {read=i_keyEnd, write=i_keyEnd};
    __property Word keyUp = {read=i_keyUp, write=i_keyUp};
    __property Word keyDown = {read=i_keyDown, write=i_keyDown};
    __property Word keyLeft = {read=i_keyLeft, write=i_keyLeft};
    __property Word keyRight = {read=i_keyRight, write=i_keyRight};
    // Access
    __property TFont* TextFont = {read=Font};

// . . . Public attributes (!)
    class cls_Log* Log;
    // Cursors
    static const TCursor crFrame,crTrack,crCancel,
                         crSelect,
                         crHandOpened,crHandClosed,
                         crZoom,crZoomP,crZoomX,
                         crMeasure,crSqCross,
                         crEdit,crCrop,
                         crPen,crHighlighter;

    class cls_VolatileDrawer* Drawer; // Here to have access in 'TfrmMplotMain::AfterReadOptions'

    // Current point measures: quite safe, because are continuously overwritten
    // Mouse
    TPoint P_clk,P_cur,P_pre; // Last clicked and current/prev point
    // Physical
    stu_FramePointQuantities p; // Current point quantities
    mpl::stu_IntervalQuantities q;  // Current interval quantities

private: //                                                     .............

// . . . Private attributes
    class TForm* Form;
    class cls_TimeCounter* TimeCounter;
    class cls_Foil *i_CurrentFoil, *ScreenFoil; // Paper on which paint
    class TTimer* timPan;

    // Frames collection
    typ_Frames frames; // Frames
    mutable typ_FramesIt fr;
    mutable typ_FramesConstIt cfr;
    typ_Layout Layout; // Canvas rows population of frames

    // . . . State
    bool IsDragDown,         // A drag operation could start
         IsMoving,           // Moving a frame or a track
         IsModeToggled,      // During mode temporarily toggled
         IsOverFrame,        // Cursor is over a frame
         IsOverCurrFrame,    // Cursor is over current frame
         IsCurrFrameChanged; // Current frame is changed
    en_PanStates PanStatus; // Enhanced pan status
    // Key handling
    TShortCut LastKeysPressed,KeysPressed;
    bool IsWaitingKeyUp; // A key is pressed
    // Other
    double VpanX, VpanY; // Pan speed [pix/s] (used as initial condition in coasting motion)
    int i_Margin; // Margin in points (given by PixMargin * Points/Pix)

// . . . Properties
    // State
    en_Modes i_Mode,PreviousMode;
    void set_Mode (en_Modes);
    typ_FramesIt i_CurrentFrameIt;
    class cls_Frame* get_CurrentFrame() const {return (static_cast<typ_FramesConstIt>(CurrentFrameIt)!=frames.end() ? (*CurrentFrameIt) : 0);}
    void __fastcall set_CurrentFrame(class cls_Frame* f) {if(f!=0 && f!=CurrentFrame) {typ_FramesIt fIt=FindFrame(f); if(fIt!=frames.end()) {CurrentFrameIt=fIt; Invalidate();}}}
    class cls_Track* get_CurrentTrack() const {return CurrentFrame ? CurrentFrame->CurrentTrack : 0;}
    void __fastcall set_CurrentTrack(class cls_Track* t) {if(t!=0 && t!=CurrentTrack) {CurrentFrame=t->Owner; CurrentFrame->CurrentTrack=t;} }
    uni_FoilObjectPtr i_Selected; // A bag for selected foil object
    // Measures
    String __fastcall get_SelectedStuff();
    String i_CurrentPoint;
    inline String get_PixelSize() const {return (static_cast<typ_FramesConstIt>(CurrentFrameIt)!=frames.end())?ToString((*CurrentFrameIt)->PixelSizeX)+String(" ; ")+ToString((*CurrentFrameIt)->PixelSizeY):String("NaN ; NaN");}
    cls_TimeQuantity i_RefreshTime;
    bool i_Slow;
    unsigned int i_Repaints;
    // Options
    #ifdef INCLUDE_SOUNDS
      bool i_Noisy;
    #endif
    int i_SampPerPix;
    unsigned int i_Timeout, i_LagThreshold;
    bool i_LockX,i_LockY;
    void set_LockX (bool);
    void set_LockY (bool);
    int i_FloatPrecision, i_FloatDigits;
    int i_DragThreshold;
    bool i_FitAtSelect;
    int i_DefaultPan;
    double i_CoastingTau, i_ContinuousPan;
    bool i_SuddenZoom;
    int i_ZoomPerc;
    // Styles: Plot
    int i_PixMargin;
    bool i_NoBackground;
    bool i_MarkCurrentFrame;
    // Keys
    Word i_keyPanToggle,
         i_keyZoomToggle,
         i_keyReset,
         i_keyDo,
         i_keyCycle,
         i_keyPrior,
         i_keyNext,
         i_keyMinus,
         i_keyPlus,
         i_keyStart,
         i_keyEnd,
         i_keyUp,
         i_keyDown,
         i_keyLeft,
         i_keyRight;

    // . . . Events
    TKeyEvent i_OnKeyDown;
    fFastVoid i_OnUpdate,i_OnChange,i_OnPaint;
    fProgress i_OnProgress;
    fPopupMenu i_OnPopupMenu;
    TNotifyEvent i_OnProperties;

    inline bool CommonMouseDown(TMouseButton,TShiftState,int,int);
    inline void CommonMouseMove(TShiftState,int,int); // Common operations before mouse move
    inline void CommonAfterMouseMove(); // Common operations after mouse move
    inline void CommonAfterMouseUp(TMouseButton,TShiftState,int,int);
    inline bool CommonMouseWheel(TShiftState, int, const TPoint&, bool&);
    inline bool CommonKeyDown(WORD, TShiftState); // Returns true if key repeated

    void __fastcall OnMouseDownSelect(TObject*,TMouseButton,TShiftState,int,int);
    void __fastcall OnMouseMoveSelect(TObject*, TShiftState,int,int);
    void __fastcall OnMouseUpSelect(TObject*,TMouseButton ,TShiftState,int,int);
    void __fastcall OnMouseWheelSelect(TObject*,TShiftState,int,const TPoint&,bool&);
    void __fastcall OnKeyDownSelect(TObject*,WORD&,TShiftState);

    void __fastcall OnMouseDownEdit(TObject*,TMouseButton,TShiftState,int,int);
    void __fastcall OnMouseMoveEdit(TObject*, TShiftState,int,int);
    void __fastcall OnMouseUpEdit(TObject*,TMouseButton ,TShiftState,int,int);
    void __fastcall OnMouseWheelEdit(TObject*,TShiftState,int,const TPoint&,bool&);
    void __fastcall OnKeyDownEdit(TObject*,WORD&,TShiftState);

    void __fastcall OnMouseDownPan(TObject*,TMouseButton,TShiftState,int,int);
    void __fastcall OnMouseMovePan(TObject*, TShiftState,int,int);
    void __fastcall OnMouseUpPan(TObject*,TMouseButton ,TShiftState,int,int);
    void __fastcall OnMouseWheelPan(TObject*,TShiftState,int,const TPoint&,bool&);
    void __fastcall OnKeyDownPan(TObject*,WORD&,TShiftState);

    void __fastcall OnMouseDownZoom(TObject*,TMouseButton ,TShiftState,int,int);
    void __fastcall OnMouseMoveZoom(TObject*,TShiftState,int,int);
    void __fastcall OnMouseUpZoom(TObject*,TMouseButton ,TShiftState,int,int);
    void __fastcall OnMouseWheelZoom(TObject*,TShiftState,int,const TPoint&,bool&);
    void __fastcall OnKeyDownZoom(TObject*,WORD&,TShiftState);

    void __fastcall OnMouseDownMeasure(TObject*,TMouseButton,TShiftState,int,int);
    void __fastcall OnMouseMoveMeasure(TObject*,TShiftState,int,int);
    void __fastcall OnMouseUpMeasure(TObject*,TMouseButton ,TShiftState,int,int);
    //void __fastcall OnMouseWheelMeasure(TObject*,TShiftState,int,const TPoint&,bool&);
    void __fastcall OnKeyDownMeasure(TObject*,WORD&,TShiftState);

    void __fastcall OnMouseDownPen(TObject*,TMouseButton,TShiftState,int,int);
    void __fastcall OnMouseMovePen(TObject*,TShiftState,int,int);
    void __fastcall OnMouseUpPen(TObject*,TMouseButton ,TShiftState,int,int);
    //void __fastcall OnMouseWheelPen(TObject*,TShiftState,int,const TPoint&,bool&);
    //void __fastcall OnKeyDownPen(TObject*,WORD&,TShiftState);

// . . . Private methods

    // Frames collection
    inline void CheckInvariant(); //  Ensure Frames Layout coherence
    class cls_Frame* CreateFrame(const stu_PlotZone&); // Create and insert a new Frame, updating current frame
    class cls_Frame* CreateFrame(); // Create a new frame below
    void RemoveFrame(typ_FramesIt); // Remove Frame given an iterator
    void MoveFrame(typ_FramesIt, typ_FramesIt); // Move a Frame before a destination
    void InsertFrame(class cls_Frame*, const stu_PlotZone&); // Insert a Frame pointer
    void SwapFrames(typ_FramesIt,typ_FramesIt); // Swap two frames
    typ_FramesIt FindFrame (class cls_Frame* f) {for (fr=frames.begin(); fr!=frames.end(); ++fr) if (*fr==f) break; return fr;} // Find a frame in frames; returns 'frames.end()' if not found
    typ_FramesIt OverFrame (const TPoint& P) {for(fr=frames.begin(); fr!=frames.end(); ++fr) if ((*fr)->IsOver(P)) break;return fr;} // Get a frame that contains a screen point; returns 'frames.end()' if not found
    void CheckCurrentFrame (const TPoint& P, bool set) {IsCurrFrameChanged=false; fr=OverFrame(P); IsOverFrame=(fr!=frames.end()); IsOverCurrFrame=IsOverFrame&&(CurrentFrameIt==fr); if(IsOverFrame&&set&&(CurrentFrameIt!=fr)){CurrentFrameIt=fr; IsOverCurrFrame=IsCurrFrameChanged=true;}}
    //bool SetCurrentFrame (const TPoint& P) {fr=OverFrame(P); if (fr!=frames.end()) {CurrentFrameIt=fr; return true;} else return false;}
    //bool SetNewCurrentFrame (const TPoint& P) {fr=OverFrame(P); if (CurrentFrameIt!=fr && fr!=frames.end()) {CurrentFrameIt=fr; return true;} else return false;}
    TRect GetZoneRect(const TPoint&); // Get the zone rectangle (used by marker)
    stu_PlotZone GetInsertionZone(const TPoint&); // Get zone by point coordinates
    // Other
    inline void UpdateTrackPointQuantities(); // Update p (mat::NaN if invalid)
    inline void UpdateTrackIntervalQuantities(); // Update q (mat::NaN if invalid)
    inline void DefaultZoom(class cls_Frame*, const TPoint&, double, bool, bool); // Do default Zoom
    inline void ToggleToMode(const en_Modes); // Temporarily toggle to mode
    inline bool UntoggleMode(); // Restore previous mode
    #ifdef INCLUDE_SOUNDS
      inline void Sound(const unsigned short); // Play a sound from resource
    #endif
    inline bool IsStartedDragging() {return (::abs(P_cur.x-P_clk.x)>Mouse->DragThreshold || ::abs(P_cur.y-P_clk.y)>Mouse->DragThreshold);}
    //inline void ChangeCursorTo(TCursor c){Cursor = c; Screen->Cursor = crDefault;}
    inline void MeasurePanSpeed(); // Measure Pan speed basing on actual frame position
    inline void MeasurePanSpeed(const int, const int); // Measure Pan speed from intercurrent displacement
    void __fastcall PhysicPan(System::TObject* Sender);

// . . . Custom message handling
   /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ MESSAGE HANDLERS
   void __fastcall WhenKeyDown (TWMKey& Message) {if(OnKeyDown) OnKeyDown(this,WORD('k'),TShiftState());}
   void __fastcall WhenMouseWheel(TWMMouseWheel& Message) {if(OnMouseWheel) OnMouseWheel();}
   BEGIN_MESSAGE_MAP
     VCL_MESSAGE_HANDLER(WM_KEYDOWN, TWMKey, WhenKeyDown)
     //VCL_MESSAGE_HANDLER(WM_MOUSEWHEEL, TWMMouseWheel, WhenMouseWheel)
   END_MESSAGE_MAP(inherited)
   //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
};//-------------------------------------------------------------------------



//---------------------------------------------------------------------------
#endif
