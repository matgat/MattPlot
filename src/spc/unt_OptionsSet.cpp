//---------------------------------------------------------------------------
#include <vcl.h>
#include "unt_Log.h"
#include "unt_MatCommon.h"
#pragma hdrstop

#include "unt_OptionsSet.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//============================ Extern variables =============================
cls_OptionsSet *Opts = 0;

//========================== File scope variables ===========================

//============================= Static Members ==============================
//static const unsigned int cls_OptionsSet::Size = 2;


//============================= Public methods ==============================


//---------------------------------------------------------------------------
__fastcall cls_OptionsSet::cls_OptionsSet(cls_Log* l) : cls_Options(l), Size(OPT_SIZE),
// -- General
  DefaultDir(cls_OptionString("General","DefaultDir","Default directory (`usr´ for home dir, `exe´ for exe dir, or explicit path)","usr")),
  TempDir(cls_OptionString("General","TempDir","Temporary files directory (`sys´ for local system, `exe´ for exe dir)","sys")),
  PromptOverwrite(cls_OptionBool("General","PromptOverwrite","Ask when writing on an existing file",true)),
  DeleteTemp(cls_OptionBool("General","DeleteTemp","Delete temp files on enter and exit",true)),
  PlaySound(cls_OptionBool("General","PlaySound","Enable/disable sound effects",true)),
  FloatPrecision(cls_OptionInt("General","FloatPrecision","Default precision of displayed numbers",6)),
  FloatDigits(cls_OptionInt("General","FloatDigits","Default digits of displayed numbers",0)),
  Macro(cls_OptionString("General","Macro","A custom operation executed with ExecMacro","")),
  DevelopMode(cls_OptionBool("General","DevelopMode","Enable work-in-progress features",false)),
// -- MainForm
  FormColor(cls_OptionTColor("MainForm","FormColor","Colors",TColor(0xD8E9EC))),  //clBtnFace
  ToolsColor(cls_OptionTColor("MainForm","ToolsColor","Colors",TColor(0xF2F6F8))),
  HighlightColor(cls_OptionTColor("MainForm","HighlightColor","Color used to highlight",TColor(0x8FF9F2))),
  EnableMainMenu(cls_OptionBool("MainForm","EnableMainMenu","Application main menu visible or not",true)),
  EnableToolbar(cls_OptionBool("MainForm","EnableToolbar","Application toolbar visible or not",true)),
  Toolbar(cls_OptionString("MainForm","Toolbar","Toolbar buttons (use `SEP´ or `DIV´ to put a separator)","Open,Export,Print,SEP,LockX,LockY,FitToAxes,FitToTrack,SEP,ShowTree,LegendToggle,GridToggle,GridMore,GridLess,SEP,MarkPoints,ShowCoords,StrokeLines,FillArea,DrawHistograms,SEP,SelectMode,EditMode,PanMode,ZoomMode,MeasureMode,PenMode,SEP,StayOnTop,Exit")),
  ObjectMenu(cls_OptionString("MainForm","ObjectMenu","General object contextual menu (use `SEP´ or `DIV´ to put a separator)","Properties,SEP,Remove")),
  FrameMenu(cls_OptionString("MainForm","FrameMenu","Frames contextual menu (use `SEP´ or `DIV´ to put a separator)","FrameProperties,RemoveFrame,AddMarker,AddText,SEP,LegendToggle,GridToggle,SEP,FitToAxes,FitToTracks,FitToTrack,SEP,ToggleScaleX,ToggleScaleY")),
  TrackMenu(cls_OptionString("MainForm","TrackMenu","Tracks contextual menu (use `SEP´ or `DIV´ to put a separator)","TrackProperties,RemoveTrack,SEP,TrackData,TrackStatistics,SEP,MarkPoints,ShowCoords,StrokeLines,FillArea,DrawHistograms")),
  LogSite(cls_OptionString("MainForm","LogSite","Initial position of Log (Bottom,Right,Floating)","Floating")),
  PaneSite(cls_OptionString("MainForm","PaneSite","Initial position of display grid (Bottom,Right,Floating)","Right")),
  //PaneWidth(cls_OptionInt("MainForm","PaneWidth","Initial width of display grid [pix]",200)),
// -- Importer
  DecimalSeparator(cls_OptionChar("Importer","DecimalSeparator","DecimalSeparator char in float literals (type `?´ for locale settings)",'.')),
  ThousandSeparator(cls_OptionChar("Importer","ThousandSeparator","ThousandSeparator char in float literals (type `?´ for locale settings)",'\a')),
  ExpectedDelimiters(cls_OptionString("Importer","ExpectedDelimiters","Expected data delimiters other than space characters",":;|")),
// -- Exporter
  DataDelimiter(cls_OptionChar("Exporter","DataDelimiter","Columns delimiter when exporting to text",'\t')),
  PrintInfoText(cls_OptionBool("Exporter","PrintInfoText","Print an info string in page header",true)),
  PrintMargin(cls_OptionInt("Exporter","PrintMargin","Print page margin [% of page dimensions]",5)),
  MetafileMagnification(cls_OptionInt("Exporter","MetafileMagnification","Resolution of metafile respect screen",5)),
  SnapshotAction(cls_OptionInt("Exporter","SnapshotAction","Choose between `1´=save as gif, `2´=copy emf to clipboard, `3´=both",3)),
// -- Plot
  FirstMode(cls_OptionString("Plot","FirstMode","Initial plot mode","SelectMode")),
  SampPerPix(cls_OptionInt("Plot","SampPerPix","Undersampling rate of rendered tracks",32)),
  Timeout(cls_OptionInt("Plot","Timeout","If draw takes too much time, abort [s]",5)),
  LagThreshold(cls_OptionInt("Plot","LagThreshold","Refresh time slowness threshold [ms]",200)),
  PlotMargin(cls_OptionInt("Plot","PlotMargin","Margin between frames [pix]",5)),
  PlotFont(cls_OptionString("Plot","PlotFont","Font in plot","Verdana,10,0x0")),
  MarkCurrentFrame(cls_OptionBool("Plot","MarkCurrentFrame","Paint current frame slightly differently",true)),
// -- Frames
  PlotAlsoOnMargins(cls_OptionBool("Frames","PlotAlsoOnMargins","Extend plot also on margins",true)),
  CenterTitle(cls_OptionBool("Frames","CenterTitle","Center title",true)),
  TitleMag(cls_OptionInt("Frames","TitleMag","Title font magnification [%]",120)),
  FrameBorder(cls_OptionInt("Frames","FrameBorder","Border width of frames [pix]",1)),
  FrameMarginX(cls_OptionInt("Frames","FrameMarginX","Horizontal margin of frames [pix]",60)),
  FrameMarginY(cls_OptionInt("Frames","FrameMarginY","Vertical margin of frames [pix]",35)),
  FrameColor(cls_OptionTColor("Frames","FrameColor","Default background color",TColor(0xF0EAED))),
// -- Axes
  AxesColor(cls_OptionTColor("Axes","AxesColor","Colors",TColor(0x0))),
  CenterAxesLabels(cls_OptionBool("Axes","CenterAxesLabels","Center axes labels",false)),
// -- Legend
  LegendSpacing(cls_OptionInt("Legend","LegendSpacing","Lines and border spacing [pix]",5)),
  LegendStrokeLength(cls_OptionInt("Legend","LegendStrokeLength","Length of track stroke [pix]",30)),
  LegendColor(cls_OptionTColor("Legend","LegendColor","Legend background: for transparency put the PlotFont color",TColor(0xF2EEF0))),
// -- Grid
  VGridDist(cls_OptionInt("Grid","VGridDist","Desired vertical grid lines distance [pix]",100)),
  HGridDist(cls_OptionInt("Grid","HGridDist","Desired horizontal grid lines distance [pix]",50)),
  VGridDivs(cls_OptionNumlist("Grid","VGridDivs","Vertical grid divisions (increasing order)",mat::StringToNumlist("1\f2\f5"))),
  HGridDivs(cls_OptionNumlist("Grid","HGridDivs","Horizontal grid divisions (increasing order)",mat::StringToNumlist("1\f2\f5"))),
  GridLinesStyle(cls_OptionTPenStyle("Grid","GridLinesStyle","Style of grid lines (`___´,`_ _´,`...´,`_._´,`_..´)",psDot)),
  GridColor(cls_OptionTColor("Grid","GridColor","Color of lines",TColor(mat::Darken(0xF0EAED,10)))),
  LabelFont(cls_OptionString("Grid","LabelFont","Font of number labels","Small Fonts,6,0x341B00")),
// -- Tracks
  TrackLineWeight(cls_OptionInt("Tracks","TrackLineWeight","Thickness of lines connecting points [pix]",1)),
  TrackLineColorSeed(cls_OptionTColor("Tracks","TrackLineColorSeed","Line color of first track",TColor(0x1D1DC9))),
  TrackLineStyle(cls_OptionTPenStyle("Tracks","TrackLineStyle","Style of track lines (`___´,`_ _´,`...´,`_._´,`_..´)",psSolid)),
  TrackPointStyle(cls_OptionChar("Tracks","TrackPointStyle","Shape of point marks (`x´,`o´,`+´,'q')",'o')),
  TrackPointSize(cls_OptionInt("Tracks","TrackPointSize","Points markers size [k*TrackLineWeight+2]",2)),
  TrackMarkPoints(cls_OptionBool("Tracks","TrackMarkPoints","Mark each point",false)),
  TrackShowCoords(cls_OptionBool("Tracks","TrackShowCoords","Show point coordinates",false)),
  TrackCoordsDist(cls_OptionInt("Tracks","TrackCoordsDist","Show point coordinates threshold [pix]",80)),
  TrackStrokeLines(cls_OptionBool("Tracks","TrackStrokeLines","Draw lines between points",true)),
  TrackFillArea(cls_OptionBool("Tracks","TrackFillArea","Fill subtended area",false)),
  TrackDrawHistograms(cls_OptionBool("Tracks","TrackDrawHistograms","Draw a rectangle for each point",false)),
// -- Markers
  MarkerLineWeight(cls_OptionInt("Markers","MarkerLineWeight","Marker line thickness",1)),
  MarkerLineColor(cls_OptionTColor("Markers","MarkerLineColor","Line color of markers",clRed)),
  MarkerLineStyle(cls_OptionTPenStyle("Markers","MarkerLineStyle","Style of marker lines (`___´,`_ _´,`...´,`_._´,`_..´)",psDash)),
// -- Select Mode
  MarkColor(cls_OptionTColor("SelectMode","MarkColor","Color of zone marker",TColor(0x849C84))),
  MarkZoneStyle(cls_OptionTBrushStyle("SelectMode","MarkZoneStyle","How filling zones while dragging frames (`##´,`´,`//´,`\\\\´,`xx´,`==´,`||´,`++´)",bsDiagCross)),
  //FitAtSelect(cls_OptionBool("SelectMode","FitAtSelect","Fit scale at track selection",true)),
// -- Edit Mode
// -- Pan Mode
  DefaultPan(cls_OptionInt("PanMode","DefaultPan","Default pan when using cursor keys [pix]",10)),
  CoastingTau(cls_OptionInt("PanMode","CoastingTau","Frame coasting motion time constant (inertia/friction) [ms]",80)),
  ContinuousPan(cls_OptionInt("PanMode","ContinuousPan","Pan proportionality vs displacement",7)),
// -- Zoom Mode
  SuddenZoom(cls_OptionBool("ZoomMode","SuddenZoom","Zoom immediately after dragging rectangle",false)),
  ZoomPerc(cls_OptionInt("ZoomMode","ZoomPerc","Default zoom percentage ratio [%]",50)),
  SelRectLineStyle(cls_OptionTPenStyle("ZoomMode","SelRectLineStyle","Style of selection rectangle lines (`___´,`_ _´,`...´,`_._´,`_..´)",psDot)),
  SelRectLineColor(cls_OptionTColor("ZoomMode","SelRectLineColor","Color of selection rectangle lines",TColor(0xA77B84))),
  SelRectAreaColor(cls_OptionTColor("ZoomMode","SelRectAreaColor","Color of selection rectangle area",TColor(0xE5E5E2))),
// -- Measure Mode
  ViewAbscissaLine(cls_OptionBool("MeasureMode","ViewAbscissaLine","View measuring abscissa line",true)),
  ViewOrdinateLine(cls_OptionBool("MeasureMode","ViewOrdinateLine","View measuring ordinate line",true)),
  ViewSlopeLine(cls_OptionBool("MeasureMode","ViewSlopeLine","View measuring slope line",false)),
  MeasureColor(cls_OptionTColor("MeasureMode","MeasureLinesColor","Color of measure lines",TColor(0x82446B))),
  MeasureLinesStyle(cls_OptionTPenStyle("MeasureMode","MeasureLinesStyle","Style of measuring lines (`___´,`_ _´,`...´,`_._´,`_..´)",psDash)),
  MeasureMarkers(cls_OptionString("MeasureMode","MeasureMarkers","Quantities linked to measure markers","tmin,tmax,avg")),
  AutoPane(cls_OptionBool("MeasureMode","AutoPane","Auto show pane when measure",true)),
  PaneQuantities(cls_OptionString("MeasureMode","PaneQuantities","Displayed quantities in pane","y,x,dy,dx,dydx,n,Dt,tmin,tmax,Dy,DY,ymin,ymax,DyDt,Sy,avg,rms,std,std%")),
// -- Pen Mode
  PencilColor(cls_OptionTColor("PenMode","PencilColor","Color of pencil",TColor(0x494F54))),
  PencilSize(cls_OptionInt("PenMode","PencilSize","Pencil line weight [pix]",3)),
  HighlighterColor(cls_OptionTColor("PenMode","HighlighterColor","Color of highlighter",TColor(0xEFFFF))),
  HighlighterSize(cls_OptionInt("PenMode","HighlighterSize","Highlighter size [pix]",30)),
// -- PlotKeys
  PanToggle(cls_OptionTShortCut("PlotKeys","PanToggle","",::ShortCut(VK_SHIFT,TShiftState()))),
  ZoomToggle(cls_OptionTShortCut("PlotKeys","ZoomToggle","",::ShortCut(VK_CONTROL,TShiftState()))),
  Reset(cls_OptionTShortCut("PlotKeys","Reset","",::ShortCut(VK_SPACE,TShiftState()))),
  Do(cls_OptionTShortCut("PlotKeys","Do","",::ShortCut(VK_RETURN,TShiftState()))),
  Cycle(cls_OptionTShortCut("PlotKeys","Cycle","",::ShortCut(VK_TAB,TShiftState()))),
  Prior(cls_OptionTShortCut("PlotKeys","Prior","",::ShortCut(VK_PRIOR,TShiftState()))),
  Next(cls_OptionTShortCut("PlotKeys","Next","",::ShortCut(VK_NEXT,TShiftState()))),
  Minus(cls_OptionTShortCut("PlotKeys","Minus","",::ShortCut(VK_SUBTRACT,TShiftState()))), // +80
  Plus(cls_OptionTShortCut("PlotKeys","Plus","",::ShortCut(VK_ADD,TShiftState()))), // +80
  Start(cls_OptionTShortCut("PlotKeys","Start","",::ShortCut(VK_HOME,TShiftState()))),
  End(cls_OptionTShortCut("PlotKeys","End","",::ShortCut(VK_END,TShiftState()))),
  Up(cls_OptionTShortCut("PlotKeys","Up","",::ShortCut(VK_UP,TShiftState()))),
  Down(cls_OptionTShortCut("PlotKeys","Down","",::ShortCut(VK_DOWN,TShiftState()))),
  Left(cls_OptionTShortCut("PlotKeys","Left","",::ShortCut(VK_LEFT,TShiftState()))),
  Right(cls_OptionTShortCut("PlotKeys","Right","",::ShortCut(VK_RIGHT,TShiftState()))),
// -- ShortCuts
  EditOptions(cls_OptionTShortCut("ShortCuts","EditOptions","",::ShortCut('E',TShiftState()<<ssAlt))),
  StoreOptions(cls_OptionTShortCut("ShortCuts","StoreOptions","",::ShortCut('S',TShiftState()<<ssAlt))),
  Snapshot(cls_OptionTShortCut("ShortCuts","Snapshot","",::ShortCut(VK_F2,TShiftState()))), // VK_SNAPSHOT
  Refresh(cls_OptionTShortCut("ShortCuts","Refresh","",::ShortCut(VK_F5,TShiftState()))),
  Abort(cls_OptionTShortCut("ShortCuts","Abort","",::ShortCut(VK_ESCAPE,TShiftState()))),
  ExecMacro(cls_OptionTShortCut("ShortCuts","ExecMacro","",::ShortCut(VK_F12,TShiftState()))),
  ToggleLockAxes(cls_OptionTShortCut("ShortCuts","ToggleLockAxes","",::ShortCut('\0',TShiftState()))),
  ShowTree(cls_OptionTShortCut("ShortCuts","ShowTree","",::ShortCut(VK_F9,TShiftState()))),
  ClearAll(cls_OptionTShortCut("ShortCuts","ClearAll","",::ShortCut(VK_DELETE,TShiftState()<<ssAlt))),
  SelectMode(cls_OptionTShortCut("ShortCuts","SelectMode","",::ShortCut(VK_NUMPAD1,TShiftState()))),
  EditMode(cls_OptionTShortCut("ShortCuts","EditMode","",::ShortCut(VK_NUMPAD2,TShiftState()))),
  PanMode(cls_OptionTShortCut("ShortCuts","PanMode","",::ShortCut(VK_NUMPAD3,TShiftState()))),
  ZoomMode(cls_OptionTShortCut("ShortCuts","ZoomMode","",::ShortCut(VK_NUMPAD4,TShiftState()))),
  MeasureMode(cls_OptionTShortCut("ShortCuts","MeasureMode","",::ShortCut(VK_NUMPAD5,TShiftState()))),
  PenMode(cls_OptionTShortCut("ShortCuts","PenMode","",::ShortCut(VK_NUMPAD6,TShiftState()))),
  AddFrame(cls_OptionTShortCut("ShortCuts","AddFrame","",::ShortCut('F',TShiftState()<<ssAlt))),
  RemoveFrame(cls_OptionTShortCut("ShortCuts","RemoveFrame","",::ShortCut(VK_DELETE,TShiftState()))),
  FrameProperties(cls_OptionTShortCut("ShortCuts","FrameProperties","",::ShortCut('G',TShiftState()<<ssAlt))),
  ExportFrameToText(cls_OptionTShortCut("ShortCuts","ExportFrameToText","",::ShortCut('K',TShiftState()<<ssAlt))),
  ExportFrameToMatlab(cls_OptionTShortCut("ShortCuts","ExportFrameToMatlab","",::ShortCut('N',TShiftState()<<ssAlt))),
  FitToAxes(cls_OptionTShortCut("ShortCuts","FitToAxes","",::ShortCut(VK_F6,TShiftState()))),
  FitToTracks(cls_OptionTShortCut("ShortCuts","FitToTracks","",::ShortCut(VK_F7,TShiftState()))),
  FitToTrack(cls_OptionTShortCut("ShortCuts","FitToTrack","",::ShortCut(VK_F8,TShiftState()))),
  LegendToggle(cls_OptionTShortCut("ShortCuts","LegendToggle","",::ShortCut(VK_F3,TShiftState()))),
  GridToggle(cls_OptionTShortCut("ShortCuts","GridToggle","",::ShortCut(VK_F4,TShiftState()))),
  GridMore(cls_OptionTShortCut("ShortCuts","GridMore","",::ShortCut('Z',TShiftState()<<ssAlt))),
  GridLess(cls_OptionTShortCut("ShortCuts","GridLess","",::ShortCut('X',TShiftState()<<ssAlt))),
  AddText(cls_OptionTShortCut("ShortCuts","AddText","",::ShortCut('L',TShiftState()<<ssAlt))),
  AddMarker(cls_OptionTShortCut("ShortCuts","AddMarker","",::ShortCut('M',TShiftState()<<ssAlt))),
  AddTrack(cls_OptionTShortCut("ShortCuts","AddTrack","",::ShortCut('T',TShiftState()<<ssAlt))),
  RemoveTrack(cls_OptionTShortCut("ShortCuts","RemoveTrack","",::ShortCut(VK_BACK,TShiftState()))),
  TrackProperties(cls_OptionTShortCut("ShortCuts","TrackProperties","",::ShortCut('Y',TShiftState()<<ssAlt))),
  TrackStatistics(cls_OptionTShortCut("ShortCuts","TrackStatistics","",::ShortCut(VK_NUMPAD0,TShiftState()))),
  TrackTransform(cls_OptionTShortCut("ShortCuts","TrackTransform","",::ShortCut(VK_F11,TShiftState()))),
  MarkPoints(cls_OptionTShortCut("ShortCuts","MarkPoints","",::ShortCut('P',TShiftState()<<ssAlt))),
  ShowCoords(cls_OptionTShortCut("ShortCuts","ShowCoords","",::ShortCut('P',TShiftState()<<ssAlt<<ssShift))),
  TrackData(cls_OptionTShortCut("ShortCuts","TrackData","",::ShortCut('\0',TShiftState()))),
  ShowLog(cls_OptionTShortCut("ShortCuts","ShowLog","",::ShortCut('\0',TShiftState()))),
  ShowPane(cls_OptionTShortCut("ShortCuts","ShowPane","",::ShortCut(VK_F10,TShiftState()))),
  Compute(cls_OptionTShortCut("ShortCuts","Compute","",::ShortCut('\0',TShiftState()))),
  Demo(cls_OptionTShortCut("ShortCuts","Demo","",::ShortCut('D',TShiftState()<<ssAlt))),
  Exit(cls_OptionTShortCut("ShortCuts","Exit","",::ShortCut('Q',TShiftState()<<ssAlt)))
// VK_LWIN VK_RWIN VK_ZOOM

/* To Add a new options add a line over here, then the pointer in     *
 *  the array under here then add the declaration in the header file, *
 *  and finally adjust the size 'OPT_SIZE' of the declared array of   *
 *  pointers 'Values'.                                                */

{ // Body of constructor
    int p = 0; // position index
// General
        Values[p++] = &DefaultDir;
        Values[p++] = &TempDir;
        Values[p++] = &PromptOverwrite;
        Values[p++] = &DeleteTemp;
        Values[p++] = &PlaySound;
        Values[p++] = &FloatPrecision;
        Values[p++] = &FloatDigits;
        Values[p++] = &Macro;
        Values[p++] = &DevelopMode;
// MainForm
        Values[p++] = &FormColor;
        Values[p++] = &ToolsColor;
        Values[p++] = &HighlightColor;
        Values[p++] = &EnableMainMenu;
        Values[p++] = &EnableToolbar;
        Values[p++] = &Toolbar;
        Values[p++] = &ObjectMenu;
        Values[p++] = &FrameMenu;
        Values[p++] = &TrackMenu;
        Values[p++] = &LogSite;
        Values[p++] = &PaneSite;
        //Values[p++] = &PaneWidth;
// Importer
        Values[p++] = &DecimalSeparator;
        Values[p++] = &ThousandSeparator;
        Values[p++] = &ExpectedDelimiters;
// Exporter
        Values[p++] = &DataDelimiter;
        Values[p++] = &PrintInfoText;
        Values[p++] = &PrintMargin;
        Values[p++] = &MetafileMagnification;
        Values[p++] = &SnapshotAction;
// Plot
        Values[p++] = &FirstMode;
        Values[p++] = &SampPerPix;
        Values[p++] = &Timeout;
        Values[p++] = &LagThreshold;
        Values[p++] = &PlotMargin;
        Values[p++] = &PlotFont;
        Values[p++] = &MarkCurrentFrame;
// Frame
        Values[p++] = &CenterTitle;
        Values[p++] = &PlotAlsoOnMargins;
        Values[p++] = &TitleMag;
        Values[p++] = &FrameBorder;
        Values[p++] = &FrameMarginX;
        Values[p++] = &FrameMarginY;
        Values[p++] = &FrameColor;
// Axes
        Values[p++] = &AxesColor;
        Values[p++] = &CenterAxesLabels;
// Legend
        Values[p++] = &LegendSpacing;
        Values[p++] = &LegendStrokeLength;
        Values[p++] = &LegendColor;
// Grid
        Values[p++] = &VGridDist;
        Values[p++] = &HGridDist;
        Values[p++] = &VGridDivs;
        Values[p++] = &HGridDivs;
        Values[p++] = &GridLinesStyle;
        Values[p++] = &GridColor;
        Values[p++] = &LabelFont;
// Tracks
        Values[p++] = &TrackLineWeight;
        Values[p++] = &TrackLineColorSeed;
        Values[p++] = &TrackLineStyle;
        Values[p++] = &TrackPointStyle;
        Values[p++] = &TrackPointSize;
        Values[p++] = &TrackMarkPoints;
        Values[p++] = &TrackShowCoords;
        Values[p++] = &TrackCoordsDist;
        Values[p++] = &TrackStrokeLines;
        Values[p++] = &TrackFillArea;
        Values[p++] = &TrackDrawHistograms;
// Markers
        Values[p++] = &MarkerLineWeight;
        Values[p++] = &MarkerLineColor;
        Values[p++] = &MarkerLineStyle;

// Select Mode
        Values[p++] = &MarkColor;
        Values[p++] = &MarkZoneStyle;
// Pan Mode
        Values[p++] = &DefaultPan;
        Values[p++] = &CoastingTau;
        Values[p++] = &ContinuousPan;
// Zoom Mode
        Values[p++] = &SuddenZoom;
        Values[p++] = &ZoomPerc;
        Values[p++] = &SelRectLineStyle;
        Values[p++] = &SelRectLineColor;
        Values[p++] = &SelRectAreaColor;
// Measure Mode
        Values[p++] = &ViewAbscissaLine;
        Values[p++] = &ViewOrdinateLine;
        Values[p++] = &ViewSlopeLine;
        Values[p++] = &MeasureColor;
        Values[p++] = &MeasureLinesStyle;
        Values[p++] = &MeasureMarkers;
        Values[p++] = &AutoPane;
        Values[p++] = &PaneQuantities;
// Edit Mode
// Pen Mode
        Values[p++] = &PencilColor;
        Values[p++] = &PencilSize;
        Values[p++] = &HighlighterColor;
        Values[p++] = &HighlighterSize;
// PlotKeys
        Values[p++] = &PanToggle;
        Values[p++] = &ZoomToggle;
        Values[p++] = &Reset;
        Values[p++] = &Do;
        Values[p++] = &Cycle;
        Values[p++] = &Prior;
        Values[p++] = &Next;
        Values[p++] = &Minus;
        Values[p++] = &Plus;
        Values[p++] = &Start;
        Values[p++] = &End;
        Values[p++] = &Up;
        Values[p++] = &Down;
        Values[p++] = &Left;
        Values[p++] = &Right;
// ShortCuts
        Values[p++] = &EditOptions;
        Values[p++] = &StoreOptions;
        Values[p++] = &Snapshot;
        Values[p++] = &Refresh;
        Values[p++] = &Abort;
        Values[p++] = &ExecMacro;
        Values[p++] = &ToggleLockAxes;
        Values[p++] = &ShowTree;
        Values[p++] = &ClearAll;
        Values[p++] = &SelectMode;
        Values[p++] = &EditMode;
        Values[p++] = &PanMode;
        Values[p++] = &ZoomMode;
        Values[p++] = &MeasureMode;
        Values[p++] = &PenMode;
        Values[p++] = &AddFrame;
        Values[p++] = &RemoveFrame;
        Values[p++] = &FrameProperties;
        Values[p++] = &ExportFrameToText;
        Values[p++] = &ExportFrameToMatlab;
        Values[p++] = &FitToAxes;
        Values[p++] = &FitToTracks;
        Values[p++] = &FitToTrack;
        Values[p++] = &LegendToggle;
        Values[p++] = &GridToggle;
        Values[p++] = &GridMore;
        Values[p++] = &GridLess;
        Values[p++] = &AddText;
        Values[p++] = &AddMarker;
        Values[p++] = &AddTrack;
        Values[p++] = &RemoveTrack;
        Values[p++] = &TrackProperties;
        Values[p++] = &TrackStatistics;
        Values[p++] = &TrackTransform;
        Values[p++] = &MarkPoints;
        Values[p++] = &ShowCoords;
        Values[p++] = &TrackData;
        Values[p++] = &ShowLog;
        Values[p++] = &ShowPane;
        Values[p++] = &Compute;
        Values[p++] = &Demo;
        Values[p++] = &Exit;
        Values[p++] = 0; // termination

    ASSERTING((p==Size),"Option index does not match Size")
}

//---------------------------------------------------------------------------
//__fastcall cls_OptionsSet::~cls_OptionsSet ()
//{ // Destructor
//}

//============================ Protected Methods ============================


//=============================== END OF FILE ===============================
