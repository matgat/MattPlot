//---------------------------------------------------------------------------
#ifndef unt_OptionsSetH
#define unt_OptionsSetH
//---------------------------------------------------------------------------
#include "unt_OptionValues.h"
#include "unt_Options.h"

        /* * * * * * * * * * * * * * * * * * * * * * * * * * * *
        *
        *    unt_OptionSet    ver: 0.1    (feb.2009)
        *    2009 - matteo.gattanini@gmail.com
        *
        *    Overview
        *    --------------------------------------------
        *    This unit manages the applications options
        *
        *    Licenses
        *    --------------------------------------------
        *    Use and modify freely
        *
        *    Release history
        *    --------------------------------------------
        *
        *    Example of usage:
        *    --------------------------------------------
        *     #include "unt_OptionSet.h"
        *     cls_OptionSet* o = new cls_OptionSet(Log);
        *     o->ReadAll("C:\\file.ini");
        *     o->WriteAll("C:\\file.ini");
        *     delete o;
        *
        * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define OPT_SIZE 148 /* Update this when adding new options! */

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class cls_OptionsSet : public cls_Options {

public://							~~~~~~~~~~~~~
// ~~~~~~~~~~~~~ Types definitions				~~~~~~~~~~~~~
// . . . Public methods
	__fastcall cls_OptionsSet (class cls_Log*);
	//__fastcall ~cls_OptionsSet ();

    void ReadAllFrom (const String& path) {ReadFromIniFile(Values,path);}
    void WriteAllTo (const String& path) {StoreToIniFile(Values,path);}
    void SetDefaults () { cls_Options::SetDefaults(Values); }

// . . . Public attributes (!)
    const int Size;
    // General
    cls_OptionString DefaultDir;
    cls_OptionString TempDir;
    cls_OptionBool PromptOverwrite;
    cls_OptionBool DeleteTemp;
    cls_OptionBool PlaySound;
    cls_OptionInt FloatPrecision;
    cls_OptionInt FloatDigits;
    cls_OptionString Macro;
    cls_OptionBool DevelopMode;
    // MainForm
    cls_OptionTColor FormColor;
    cls_OptionTColor ToolsColor;
    cls_OptionTColor HighlightColor;
    cls_OptionBool EnableMainMenu;
    cls_OptionBool EnableToolbar;
    cls_OptionString Toolbar,
                     ObjectMenu,
                     FrameMenu,
                     TrackMenu;
    cls_OptionString LogSite;
    cls_OptionString PaneSite;
    //PaneWidth;
    // Importer
    cls_OptionChar DecimalSeparator;
    cls_OptionChar ThousandSeparator;
    cls_OptionString ExpectedDelimiters;
    // Exporter
    cls_OptionChar DataDelimiter;
    cls_OptionBool PrintInfoText;
    cls_OptionInt PrintMargin;
    cls_OptionInt MetafileMagnification;
    cls_OptionInt SnapshotAction;
    // Plot
    cls_OptionString FirstMode;
    cls_OptionInt SampPerPix;
    cls_OptionInt Timeout;
    cls_OptionInt LagThreshold;
    cls_OptionInt PlotMargin;
    cls_OptionString PlotFont;
    cls_OptionBool MarkCurrentFrame;
    // Frame
    cls_OptionBool PlotAlsoOnMargins;
    cls_OptionBool CenterTitle;
    cls_OptionInt TitleMag;
    cls_OptionInt FrameBorder;
    cls_OptionInt FrameMarginX;
    cls_OptionInt FrameMarginY;
    cls_OptionTColor FrameColor;
    // Axes
    cls_OptionTColor AxesColor;
    cls_OptionBool CenterAxesLabels;
    // Legend
    cls_OptionInt LegendSpacing;
    cls_OptionInt LegendStrokeLength;
    cls_OptionTColor LegendColor;
    // Grid
    cls_OptionInt VGridDist;
    cls_OptionInt HGridDist;
    cls_OptionNumlist VGridDivs;
    cls_OptionNumlist HGridDivs;
    cls_OptionTPenStyle GridLinesStyle;
    cls_OptionTColor GridColor;
    cls_OptionString LabelFont;
    // Tracks
    cls_OptionInt TrackLineWeight;
    cls_OptionTColor TrackLineColorSeed;
    cls_OptionTPenStyle TrackLineStyle;
    cls_OptionChar TrackPointStyle;
    cls_OptionInt TrackPointSize;
    cls_OptionBool TrackMarkPoints;
    cls_OptionBool TrackShowCoords;
    cls_OptionInt TrackCoordsDist;
    cls_OptionBool TrackStrokeLines;
    cls_OptionBool TrackFillArea;
    cls_OptionBool TrackDrawHistograms;
    // Markers
    cls_OptionInt MarkerLineWeight;
    cls_OptionTColor MarkerLineColor;
    cls_OptionTPenStyle MarkerLineStyle;
    // Edit Mode
    cls_OptionTColor MarkColor;
    cls_OptionTBrushStyle MarkZoneStyle;
    //cls_OptionBool FitAtSelect;
    // Pan Mode
    cls_OptionInt DefaultPan;
    cls_OptionInt CoastingTau;
    cls_OptionInt ContinuousPan;
    // Zoom Mode
    cls_OptionBool SuddenZoom;
    cls_OptionInt ZoomPerc;
    cls_OptionTPenStyle SelRectLineStyle;
    cls_OptionTColor SelRectLineColor;
    cls_OptionTColor SelRectAreaColor;
    // Measure Mode
    cls_OptionBool ViewAbscissaLine;
    cls_OptionBool ViewOrdinateLine;
    cls_OptionBool ViewSlopeLine;
    cls_OptionTColor MeasureColor;
    cls_OptionTPenStyle MeasureLinesStyle;
    cls_OptionString MeasureMarkers;
    cls_OptionBool AutoPane;
    cls_OptionString PaneQuantities;
    // Pen Mode
    cls_OptionTColor PencilColor;
    cls_OptionInt PencilSize;
    cls_OptionTColor HighlighterColor;
    cls_OptionInt HighlighterSize;
    // PlotKeys
    cls_OptionTShortCut PanToggle,
                        ZoomToggle,
                        Reset,
                        Do,
                        Cycle,
                        Prior,
                        Next,
                        Minus,
                        Plus,
                        Start,
                        End,
                        Up,
                        Down,
                        Left,
                        Right;
    // ShortCuts
    cls_OptionTShortCut EditOptions,
                        StoreOptions,
                        Snapshot,
                        Refresh,
                        Abort,
                        ExecMacro,
                        ToggleLockAxes,
                        ShowTree,
                        ClearAll,
                        SelectMode,
                        EditMode,
                        PanMode,
                        ZoomMode,
                        MeasureMode,
                        PenMode,
                        AddFrame,
                        RemoveFrame,
                        FrameProperties,
                        ExportFrameToText,
                        ExportFrameToMatlab,
                        FitToAxes,
                        FitToTracks,
                        FitToTrack,
                        LegendToggle,
                        GridToggle,
                        GridMore,
                        GridLess,
                        AddText,
                        AddMarker,
                        AddTrack,
                        RemoveTrack,
                        TrackProperties,
                        TrackStatistics,
                        TrackTransform,
                        MarkPoints,
                        ShowCoords,
                        TrackData,
                        ShowLog,
                        ShowPane,
                        Compute,
                        Demo,
                        Exit;
private://							~~~~~~~~~~~~~
// . . . Private methods
// . . . Private attributes
    class cls_OptionValue* Values[OPT_SIZE];
};//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Global pointer
    extern cls_OptionsSet *Opts;
//---------------------------------------------------------------------------
#endif
