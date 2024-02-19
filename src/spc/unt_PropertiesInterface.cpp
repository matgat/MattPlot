//---------------------------------------------------------------------------
#include <vcl.h>
#include "unt_MatCommon.h"
#pragma hdrstop
#include "unt_Frames.h" //   > MattPlot classes
#include "unt_Tracks.h" //  /
#include "unt_PropertiesInterface.h"
#include "unt_OptionsSet.h" // store options funct
#include "unt_CreateCtrls.h" // To ease controls positioning
#include <Dialogs.hpp> // color dialog
//---------------------------------------------------------------------------
#pragma package(smart_init)

//=============================== File scope ================================


//////////////////////////// cls_FoilObjectEdit /////////////////////////////

//---------------------------------------------------------------------------
__fastcall cls_FoilObjectEdit::cls_FoilObjectEdit (TWinControl* p, cls_FoilObject* fo) : cls_PropertiesInterface(p,fo)
{ // Constructor

    if (!i_FoilObject)
       {
        TLabel* lblEmpty = ccl::CreateControl<TLabel>(p,TRect(Margin,Margin,Margin+width,2*Margin));
        lblEmpty->Caption = "Empty";
        return;
       }
// . . . Interface instances (They're deleted by owner)
    // . . . Fixed size panel
    pnlFixed = new TPanel(Parent);
    pnlFixed->Parent = Parent;
    //pnlFixed->BevelInner = bvNone;
    //pnlFixed->BevelOuter = bvNone;
    pnlFixed->Align = alLeft;
    pnlFixed->Width = (6*pnlFixed->Parent->Width)/10;
    int first_col_wdt = 5*pnlFixed->Width/10;
    int vspace = Margin;
    int hspace = Margin;

    // . . . Style paintbox
    pbxStyle = new TPaintBox(Parent);
    pbxStyle->Parent = Parent;
    pbxStyle->Align = alClient;
    pbxStyle->OnPaint = PaintPreview;

    // . . . LineWeight
    cbxLineWeight = ccl::CreateControl<TComboBox>( pnlFixed, TRect(first_col_wdt,vspace,pnlFixed->Width-hspace,0) );
    cbxLineWeight->Items->SetText("0\n1\n2\n3\n4\n5\n6\n7\n8\n9");
    cbxLineWeight->OnChange = Update;
    TLabel *lblLineWeight = ccl::CreateControl<TLabel>(cbxLineWeight, -hspace, first_col_wdt-hspace );
    lblLineWeight->Caption = "Line weight";

    // . . . Margin
    cbxMarginX = ccl::CreateControl<TComboBox>( cbxLineWeight, vspace );
    cbxMarginX->Width = cbxMarginX->Width/2;
    cbxMarginX->Enabled = bool(dynamic_cast<cls_SizedObject*>(i_FoilObject));
    cbxMarginX->Items->SetText("0\n1\n5\n10\n20\n30\n40\n50\n60\n70\n80\n90");
    cbxMarginX->OnChange = Update;
    cbxMarginY = ccl::CreateControl<TComboBox>( cbxMarginX, 0, cbxMarginX->Width );
    cbxMarginY->Top = cbxMarginX->Top;
    cbxMarginY->Enabled = cbxMarginX->Enabled;
    cbxMarginY->Items->SetText("0\n1\n5\n10\n20\n30\n40\n50\n60\n70\n80\n90");
    cbxMarginY->OnChange = cbxMarginX->OnChange;
    TLabel *lblMargin = ccl::CreateControl<TLabel>(cbxMarginX, -hspace, first_col_wdt-hspace );
    lblMargin->Caption = "Margins";
    lblMargin->Enabled = cbxMarginX->Enabled;

    // . . . ColorMode
    cbxColorMode = ccl::CreateControl<TComboBox>( cbxMarginX, vspace );
    cbxColorMode->Width = cbxLineWeight->Width;
    cbxColorMode->Items->SetText("pmBlack\npmWhite\npmNop\npmNot\npmCopy\npmNotCopy\npmMergePenNot\npmMaskPenNot\npmMergeNotPen\npmMaskNotPen\npmMerge\npmNotMerge\npmMask\npmNotMask\npmXor\npmNotXor");
    cbxColorMode->Style = Stdctrls::csDropDownList;
    cbxColorMode->OnChange = Update;
    TLabel *lblColorMode = ccl::CreateControl<TLabel>(cbxColorMode, -hspace, first_col_wdt-hspace );
    lblColorMode->Caption = "Color mode";

    // . . . LineStyle
    cbxLineStyle = ccl::CreateControl<TComboBox>( cbxColorMode, vspace );
    cbxLineStyle->Items->SetText("psSolid\npsDash\npsDot\npsDashDot\npsDashDotDot\npsClear\npsInsideFrame");
    cbxLineStyle->Style = Stdctrls::csDropDownList;
    cbxLineStyle->OnChange = Update;
    TLabel *lblLineStyle = ccl::CreateControl<TLabel>(cbxLineStyle, -hspace, first_col_wdt-hspace );
    lblLineStyle->Caption = "Line style";

    // . . . AreaStyle
    cbxAreaStyle = ccl::CreateControl<TComboBox>( cbxLineStyle, vspace );
    cbxAreaStyle->Items->SetText("bsSolid\nbsClear\nbsHorizontal\nbsVertical\nbsFDiagonal\nbsBDiagonal\nbsCross\nbsDiagCross");
    cbxAreaStyle->Style = Stdctrls::csDropDownList;
    cbxAreaStyle->OnChange = Update;
    TLabel *lblAreaStyle = ccl::CreateControl<TLabel>(cbxAreaStyle, -hspace, first_col_wdt-hspace );
    lblAreaStyle->Caption = "Area style";

    // . . . LineColor
    shpLineColor = ccl::CreateControl<TShape>( cbxAreaStyle, vspace );
    //shpLineColor->Height = 2*lblLineColor->Height;
    shpLineColor->OnMouseDown = ShapeMouseDown;
    shpLineColor->Cursor = crHandPoint;
    TLabel *lblLineColor = ccl::CreateControl<TLabel>(shpLineColor, -hspace, first_col_wdt-hspace );
    lblLineColor->Caption = "Line color";

    // . . . AreaColor
    shpAreaColor = ccl::CreateControl<TShape>( shpLineColor, vspace );
    //shpAreaColor->Height = 2*lblLineColor->Height;
    shpAreaColor->OnMouseDown = ShapeMouseDown;
    shpAreaColor->Cursor = crHandPoint;
    TLabel *lblAreaColor = ccl::CreateControl<TLabel>(shpAreaColor, -hspace, first_col_wdt-hspace );
    lblAreaColor->Caption = "Area color";

    // . . . Font
    btnFont = ccl::CreateControl<TLabel>( shpAreaColor, vspace );
    btnFont->Caption = "ABCabc123";
    btnFont->OnMouseDown = FontMouseDown;
    btnFont->Cursor = crHandPoint;
    TLabel *lblFont = ccl::CreateControl<TLabel>(btnFont, -hspace, first_col_wdt-hspace );
    lblFont->Caption = "Text font";

    // . . . Visible
    chkVisible = ccl::CreateControl<TCheckBox>(lblFont, vspace );
    chkVisible->Caption = "Visible";
    chkVisible->Width = first_col_wdt;
    chkVisible->Alignment = taLeftJustify;

// . . . Finally
    Revert(i_FoilObject); // Initial values
}

//---------------------------------------------------------------------------
void cls_FoilObjectEdit::Apply(cls_FoilObject* fo)
{ // Apply changes to linked foil object

    if (!fo) return;
    try {
         fo->Visible = chkVisible->Checked;
         fo->LineWeight = cbxLineWeight->Tag;
         fo->LineColor = shpLineColor->Brush->Color;
         fo->LineStyle = TPenStyle(cbxLineStyle->ItemIndex);
         fo->AreaColor = shpAreaColor->Brush->Color;
         fo->AreaStyle = TBrushStyle(cbxAreaStyle->ItemIndex);
         fo->ColorMode = TPenMode(cbxColorMode->ItemIndex);
         fo->AssignFont(btnFont->Font);

         cls_SizedObject *so = dynamic_cast<cls_SizedObject*>(fo);
         if (so) {
                  so->PixMarginX = cbxMarginX->Tag;
                  so->PixMarginY = cbxMarginY->Tag;
                 }
        }
    catch (Exception& e) { mat::MsgError(e.Message); }
}

//---------------------------------------------------------------------------
void cls_FoilObjectEdit::Revert(cls_FoilObject* fo)
{ // Revert values to those of linked foil object

    if (!fo) return;
    chkVisible->Checked = fo->Visible;
    cbxLineWeight->Text = String(fo->LineWeight);
    cbxColorMode->ItemIndex = int(fo->ColorMode);
    cbxLineStyle->ItemIndex = int(fo->LineStyle);
    cbxAreaStyle->ItemIndex = int(fo->AreaStyle);
    shpLineColor->Brush->Color = fo->LineColor;
    shpAreaColor->Brush->Color = fo->AreaColor;
    btnFont->Font->Assign(fo->Font);
    cls_SizedObject *so = dynamic_cast<cls_SizedObject*>(fo);
    if (so) {
             cbxMarginX->Text = String(so->PixMarginX);
             cbxMarginY->Text = String(so->PixMarginY);
            }
    Update(Parent);
}

//============================= Private methods =============================

//---------------------------------------------------------------------------
void __fastcall cls_FoilObjectEdit::Update(TObject *Sender)
{ // Generic control change event

    if (!i_FoilObject) return;
    cbxLineWeight->Tag = cbxLineWeight->Text.Trim().ToIntDef(i_FoilObject->LineWeight);
    cbxLineWeight->Text = String(cbxLineWeight->Tag);
    cls_SizedObject *so = dynamic_cast<cls_SizedObject*>(i_FoilObject);
    if (so) {
             cbxMarginX->Tag = cbxMarginX->Text.Trim().ToIntDef(so->PixMarginX);
             cbxMarginY->Tag = cbxMarginY->Text.Trim().ToIntDef(so->PixMarginY);
             cbxMarginX->Text = String(cbxMarginX->Tag);
             cbxMarginY->Text = String(cbxMarginY->Tag);
            }
    pbxStyle->Invalidate();
}

//---------------------------------------------------------------------------
void __fastcall cls_FoilObjectEdit::PaintPreview(TObject *Sender)
{ // Paint style

    if ( chkVisible->Checked )
       {
        // Set tools
        pbxStyle->Canvas->Pen->Width = cbxLineWeight->Tag;
        pbxStyle->Canvas->Pen->Color = shpLineColor->Brush->Color;
        pbxStyle->Canvas->Pen->Style = TPenStyle(cbxLineStyle->ItemIndex);
        pbxStyle->Canvas->Brush->Color = shpAreaColor->Brush->Color;
        pbxStyle->Canvas->Brush->Style = TBrushStyle(cbxAreaStyle->ItemIndex);
        pbxStyle->Canvas->Pen->Mode = TPenMode(cbxColorMode->ItemIndex);
        pbxStyle->Canvas->Font->Assign(btnFont->Font);
        // Draw
        pbxStyle->Canvas->Rectangle( TRect(Margin,Margin,pbxStyle->Width-Margin,pbxStyle->Height-Margin) );
        String TextString = "Text out";
        int X = (pbxStyle->Width - pbxStyle->Canvas->TextWidth(TextString))/2;
        int Y = (pbxStyle->Height - pbxStyle->Canvas->TextHeight(TextString))/2;
        pbxStyle->Canvas->TextOut( X, Y, TextString );
       }
}



//---------------------------------------------------------------------------
void __fastcall cls_FoilObjectEdit::ShapeMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{ // Modify shape color

    TShape* shp = dynamic_cast<TShape*>(Sender);
    if ( shp ) {
                TColorDialog *dlgColor = new TColorDialog(Parent);
                dlgColor->Color = shp->Brush->Color;
                dlgColor->Options = TColorDialogOptions() << cdFullOpen;
                if ( dlgColor->Execute() ) shp->Brush->Color = dlgColor->Color;
                delete dlgColor;
                Update(Sender);
               }
}

//---------------------------------------------------------------------------
void __fastcall cls_FoilObjectEdit::FontMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{ // Modify Font

    TFontDialog *dlgFont = new TFontDialog(Parent);
    dlgFont->Font->Assign(i_FoilObject->Font);
    //dlgFont->Options = TFontDialogOptions() << ;
    if ( dlgFont->Execute() ) btnFont->Font->Assign(dlgFont->Font);
    delete dlgFont;
    Update(Sender);
}


/////////////////////////////// cls_FrameEdit ///////////////////////////////

//---------------------------------------------------------------------------
__fastcall cls_FrameEdit::cls_FrameEdit (TWinControl* p, cls_Frame* f)
  : cls_PropertiesInterface(p,f), i_Frame(f)
{ // Constructor

// . . . Interface instances (They're deleted by owner)
    int vspace = Margin / 2;

    // . . . Title
    TLabel* lblFrameTitle = ccl::CreateControl<TLabel>(p,TRect(Margin,Margin,Margin+width,Margin+2*vspace));
    lblFrameTitle->Caption = "Title";
    edtFrameTitle = ccl::CreateControl<TEdit>(lblFrameTitle, vspace);
    edtFrameTitle->Width = width;
    edtFrameTitle->Anchors = TAnchors() << akTop << akLeft << akRight;

    // . . . SampPerPix
    TLabel* lblSampPerPix = ccl::CreateControl<TLabel>( edtFrameTitle, Margin);
    lblSampPerPix->Caption = "Samples per Pixel";
    edtSampPerPix = ccl::CreateControl<TEdit>(lblSampPerPix, vspace);
    edtSampPerPix->Width = width;
    edtSampPerPix->Anchors = TAnchors() << akTop << akLeft << akRight;

// . . . Finally
    Revert(i_Frame); // Initial values
}

//---------------------------------------------------------------------------
void cls_FrameEdit::Apply(cls_FoilObject* fo)
{ // Apply changes to linked foil object

    i_Frame->Title = edtFrameTitle->Text;
    i_Frame->SampPerPix = edtSampPerPix->Text.ToInt();
}

//---------------------------------------------------------------------------
void cls_FrameEdit::Revert(cls_FoilObject* fo)
{ // Revert values to those of linked foil object

    edtFrameTitle->Text = i_Frame->Title;
    edtSampPerPix->Text = String(i_Frame->SampPerPix);
}

//---------------------------------------------------------------------------
void cls_FrameEdit::AlterOptions(cls_OptionsSet* o)
{ // Alter options

    o->FrameBorder = i_Frame->LineWeight;
    o->FrameMarginX = i_Frame->PixMarginX;
    o->FrameMarginY = i_Frame->PixMarginY;
    o->PlotFont = mat::TFontToString(i_Frame->Font);
    o->FrameColor = i_Frame->AreaColor;
    o->AxesColor = i_Frame->Axes->LineColor;

    o->GridColor = i_Frame->Grid->LineColor;
    o->LabelFont = mat::TFontToString(i_Frame->Grid->Font);
    //o->VGridDist;
    //o->HGridDist;
    o->GridLinesStyle = i_Frame->Grid->LineStyle;

    o->LegendSpacing = i_Frame->Legend->PixMargin;
    //Opts->LegendStrokeLength;

    o->SampPerPix = i_Frame->SampPerPix;
}




/////////////////////////////// cls_TrackEdit ///////////////////////////////
//---------------------------------------------------------------------------
__fastcall cls_TrackEdit::cls_TrackEdit (TWinControl* p, cls_Track* t)
  : cls_PropertiesInterface(p,t), i_Track(t)
{ // Constructor

// . . . Interface instances (They're deleted by owner)
    //Margin = 2 * Screen->MenuFont->Size;
    int vspace = Margin / 2;

    // . . . Name
    TLabel* lblTrackName = ccl::CreateControl<TLabel>(p,TRect(Margin,Margin,Margin+width,Margin+2*vspace));
    lblTrackName->Caption = "Name";
    edtTrackName = ccl::CreateControl<TEdit>(lblTrackName, vspace);
    edtTrackName->Width = width;
    edtTrackName->Anchors = TAnchors() << akTop << akLeft << akRight;

    // . . . Markpoints
    chkMarkPoints = ccl::CreateControl<TCheckBox>(edtTrackName,Margin);
    chkMarkPoints->Width = width/3;
    chkMarkPoints->Caption = "Mark points";

    // . . . PointStyle and size
    cbxPointStyle = ccl::CreateControl<TComboBox>( chkMarkPoints, Margin, 50 );
    cbxPointStyle->Style = Stdctrls::csDropDownList;
    for (int i=1; i<=cls_Track::PointStyles.Length(); ++i) cbxPointStyle->Items->Add(String(cls_Track::PointStyles[i]));
    edtPointSize = ccl::CreateControl<TEdit>(cbxPointStyle, Margin, 50);
    edtPointSize->Anchors = TAnchors() << akTop << akLeft << akRight;
    edtPointSize->Hint = "Point size coefficient";
    edtPointSize->ShowHint = true;

    // . . . StrokeLines
    chkStrokeLines = ccl::CreateControl<TCheckBox>(chkMarkPoints,Margin);
    chkStrokeLines->Width = width/2;
    chkStrokeLines->Caption = "Stroke lines";

    // . . . FillArea
    chkFillArea = ccl::CreateControl<TCheckBox>(chkStrokeLines,Margin);
    chkFillArea->Width = width/2;
    chkFillArea->Caption = "Fill area";

    // . . . DrawHistograms / Stems
    chkDrawHistograms = ccl::CreateControl<TCheckBox>(chkFillArea,Margin);
    chkDrawHistograms->Width = width/2;
    chkDrawHistograms->Caption = "Draw histograms";
    edtHistWidthPerc = ccl::CreateControl<TEdit>(chkDrawHistograms, Margin, 50);
    edtHistWidthPerc->Anchors = TAnchors() << akTop << akLeft << akRight;
    edtHistWidthPerc->Hint = "Histograms width percentage 0÷100%";
    edtHistWidthPerc->ShowHint = true;

    // . . . ShowCoords and CoordsDist
    chkShowCoords = ccl::CreateControl<TCheckBox>(chkDrawHistograms,Margin);
    chkShowCoords->Width = width/2;
    chkShowCoords->Caption = "Show coordinates";
    edtCoordsDist = ccl::CreateControl<TEdit>(chkShowCoords, Margin, 50);
    edtCoordsDist->Anchors = TAnchors() << akTop << akLeft << akRight;
    edtCoordsDist->Hint = "Hiding distance threshold [pix]";
    edtCoordsDist->ShowHint = true;

// . . . Finally
    Revert(i_Track); // Initial values
}

//---------------------------------------------------------------------------
void cls_TrackEdit::Apply(cls_FoilObject* fo)
{ // Apply changes to linked foil object

    i_Track->Name = edtTrackName->Text;
    i_Track->MarkPoints = chkMarkPoints->Checked;
    if ( cbxPointStyle->Text.Length() > 0 ) i_Track->PointStyle = cbxPointStyle->Text[1];
    i_Track->PointSizeCoeff = edtPointSize->Text.ToIntDef(i_Track->PointSize);
    i_Track->StrokeLines = chkStrokeLines->Checked;
    i_Track->FillArea = chkFillArea->Checked;
    i_Track->DrawHistograms = chkDrawHistograms->Checked;
    i_Track->HistWidthPerc = edtHistWidthPerc->Text.ToIntDef(i_Track->HistWidthPerc);
    i_Track->ShowCoords = chkShowCoords->Checked;
    i_Track->CoordsDist = edtCoordsDist->Text.ToIntDef(i_Track->CoordsDist);
}

//---------------------------------------------------------------------------
void cls_TrackEdit::Revert(cls_FoilObject* fo)
{ // Revert values to those of linked foil object

    edtTrackName->Text = i_Track->Name;
    chkMarkPoints->Checked = i_Track->MarkPoints;
    for(int i=0; i<cbxPointStyle->Items->Count; ++i)
        if (cbxPointStyle->Items->Strings[i][1]==i_Track->PointStyle) {cbxPointStyle->ItemIndex=i; break;}
    edtPointSize->Text = String(i_Track->PointSizeCoeff);
    chkStrokeLines->Checked = i_Track->StrokeLines;
    chkFillArea->Checked = i_Track->FillArea;
    chkDrawHistograms->Checked = i_Track->DrawHistograms;
    edtHistWidthPerc->Text = String(i_Track->HistWidthPerc);
    chkShowCoords->Checked = i_Track->ShowCoords;
    edtCoordsDist->Text = String(i_Track->CoordsDist);
}

//---------------------------------------------------------------------------
void cls_TrackEdit::AlterOptions(cls_OptionsSet* o)
{ // Alter options

    o->TrackLineWeight = i_Track->LineWeight;
    o->TrackLineColorSeed = i_Track->LineColor;
    o->TrackLineStyle = i_Track->LineStyle;

    o->TrackPointStyle = i_Track->PointStyle;
    o->TrackPointSize = i_Track->PointSizeCoeff;
    o->TrackMarkPoints = i_Track->MarkPoints;
    o->TrackStrokeLines = i_Track->StrokeLines;
    o->TrackFillArea = i_Track->FillArea;
    o->TrackDrawHistograms = i_Track->DrawHistograms;
    o->TrackShowCoords = i_Track->ShowCoords;
    o->TrackCoordsDist = i_Track->CoordsDist;
}



//////////////////////////////// cls_AxesEdit ///////////////////////////////
//---------------------------------------------------------------------------
__fastcall cls_AxesEdit::cls_AxesEdit (TWinControl* p, cls_Axes* a)
  : cls_PropertiesInterface(p,a), i_Axes(a)
{ // Constructor

// . . . Interface instances (They're deleted by owner)
    int vspace = Margin / 2;

    // . . . Xlabel, Ylabel
    TLabel* lblXlabel = ccl::CreateControl<TLabel>(p,TRect(Margin,Margin,Margin+width,Margin+2*vspace));
    lblXlabel->Caption = "X label";
    edtXlabel = ccl::CreateControl<TEdit>(lblXlabel, vspace);
    edtXlabel->Anchors = TAnchors() << akTop << akLeft << akRight;
    edtXlabel->Width = width;

    TLabel* lblYlabel = ccl::CreateControl<TLabel>(edtXlabel, Margin);
    lblYlabel->Caption = "Y label";
    edtYlabel = ccl::CreateControl<TEdit>(lblYlabel, vspace);
    edtYlabel->Anchors = TAnchors() << akTop << akLeft << akRight;
    edtYlabel->Width = width;

// . . . Finally
    Revert(i_Axes); // Initial values
}

//---------------------------------------------------------------------------
void cls_AxesEdit::Apply(cls_FoilObject* fo)
{ // Apply changes to linked foil object

    i_Axes->Xlabel = edtXlabel->Text;
    i_Axes->Ylabel = edtYlabel->Text;
}

//---------------------------------------------------------------------------
void cls_AxesEdit::Revert(cls_FoilObject* fo)
{ // Revert values to those of linked foil object

    edtXlabel->Text = i_Axes->Xlabel;
    edtYlabel->Text = i_Axes->Ylabel;
}

//---------------------------------------------------------------------------
void cls_AxesEdit::AlterOptions(cls_OptionsSet* o)
{ // Alter options

    o->AxesColor = i_Axes->LineColor;
}



//////////////////////////////// cls_GridEdit ///////////////////////////////
//---------------------------------------------------------------------------
__fastcall cls_GridEdit::cls_GridEdit (TWinControl* p, cls_Grid* g)
  : cls_PropertiesInterface(p,g), i_Grid(g)
{ // Constructor

// . . . Interface instances (They're deleted by owner)
    int vspace = Margin / 2;

    // . . . Vertical grid divisions
    TLabel* lblVGrid = ccl::CreateControl<TLabel>( p, TRect(Margin,Margin,Margin+width,Margin+2*vspace) );
    lblVGrid->Caption = "Vertical lines distance/divisions";
    edtVGridDist = ccl::CreateControl<TEdit>(lblVGrid, vspace);
    edtVGridDist->Width = (width-Margin)/3;
    edtVGridDist->Anchors = TAnchors() << akTop << akLeft;
    edtVGridDist->OnChange = Update;
    edtVGridDivs = ccl::CreateControl<TEdit>(edtVGridDist, Margin, width-edtVGridDist->Width-Margin);
    edtVGridDivs->Anchors = TAnchors() << akTop << akLeft << akRight;

// . . . Horizontal grid divisions
    TLabel* lblHGrid = ccl::CreateControl<TLabel>( edtVGridDist, 2*vspace );
    lblHGrid->Caption = "Horizontal lines distance/divisions";
    edtHGridDist = ccl::CreateControl<TEdit>(lblHGrid, vspace);
    edtHGridDist->Width = edtVGridDist->Width;
    edtHGridDist->Anchors = TAnchors() << akTop << akLeft;
    edtHGridDist->OnChange = Update;
    edtHGridDivs = ccl::CreateControl<TEdit>(edtHGridDist, Margin, edtVGridDivs->Width);
    edtHGridDivs->Anchors = TAnchors() << akTop << akLeft << akRight;

// . . . Finally
    Revert(i_Grid); // Initial values
}

//---------------------------------------------------------------------------
void cls_GridEdit::Apply(cls_FoilObject* fo)
{ // Apply changes to linked foil object

    i_Grid->VGridDist = edtVGridDist->Tag;
    i_Grid->VGridDivs = edtVGridDivs->Text;
    i_Grid->HGridDist = edtHGridDist->Tag;
    i_Grid->HGridDivs = edtHGridDivs->Text;
}

//---------------------------------------------------------------------------
void cls_GridEdit::Revert(cls_FoilObject* fo)
{ // Revert values to those of linked foil object

    edtVGridDist->Text = String(i_Grid->VGridDist);
    edtVGridDivs->Text = i_Grid->VGridDivs;
    edtHGridDist->Text = String(i_Grid->HGridDist);
    edtHGridDivs->Text = i_Grid->HGridDivs;
}

//---------------------------------------------------------------------------
void cls_GridEdit::AlterOptions(cls_OptionsSet* o)
{ // Alter options

    o->VGridDist = i_Grid->VGridDist;
    o->HGridDist = i_Grid->HGridDist;
    o->VGridDivs = i_Grid->VGridDivs;
    o->HGridDivs = i_Grid->HGridDivs;
    o->GridLinesStyle = i_Grid->LineStyle;
    o->GridColor = i_Grid->LineColor;
    //o->LabelFont;
}

//============================= Private methods =============================

//---------------------------------------------------------------------------
void __fastcall cls_GridEdit::Update(TObject *Sender)
{ // Generic control change event

    edtVGridDist->Tag = edtVGridDist->Text.Trim().ToIntDef(i_Grid->VGridDist);
    edtVGridDist->Text = String(edtVGridDist->Tag);
    edtHGridDist->Tag = edtHGridDist->Text.Trim().ToIntDef(i_Grid->HGridDist);
    edtHGridDist->Text = String(edtHGridDist->Tag);
}



/////////////////////////////// cls_LegendEdit //////////////////////////////
//---------------------------------------------------------------------------
__fastcall cls_LegendEdit::cls_LegendEdit (TWinControl* p, cls_Legend* l)
  : cls_PropertiesInterface(p,l), i_Legend(l)
{ // Constructor

// . . . Interface instances (They're deleted by owner)
    int vspace = Margin / 2;

    // . . . Stroke length
    TLabel* lblStrokeLength = ccl::CreateControl<TLabel>( p, TRect(Margin,Margin,Margin+width,Margin+2*vspace));
    lblStrokeLength->Caption = "Stroke length";
    edtStrokeLength = ccl::CreateControl<TEdit>(lblStrokeLength, vspace);
    edtStrokeLength->Width = width;
    edtStrokeLength->Anchors = TAnchors() << akTop << akLeft << akRight;

// . . . Finally
    Revert(i_Legend); // Initial values
}

//---------------------------------------------------------------------------
void cls_LegendEdit::Apply(cls_FoilObject* fo)
{ // Apply changes to linked foil object

    i_Legend->StrokeLength = edtStrokeLength->Text.ToIntDef(10);
}

//---------------------------------------------------------------------------
void cls_LegendEdit::Revert(cls_FoilObject* fo)
{ // Revert values to those of linked foil object

    edtStrokeLength->Text = String(i_Legend->StrokeLength);
}

//---------------------------------------------------------------------------
void cls_LegendEdit::AlterOptions(cls_OptionsSet* o)
{ // Alter options

    o->LegendSpacing = i_Legend->PixMargin;
    o->LegendColor = i_Legend->AreaColor;
    o->LegendStrokeLength = i_Legend->StrokeLength;
}


/////////////////////////////// cls_MarkerEdit //////////////////////////////
//---------------------------------------------------------------------------
__fastcall cls_MarkerEdit::cls_MarkerEdit (TWinControl* p, cls_Marker* m)
  : cls_PropertiesInterface(p,m), i_Marker(m)
{ // Constructor

// . . . Interface instances (They're deleted by owner)
    int vspace = Margin;

    // . . . Value
    TLabel* lblValue = ccl::CreateControl<TLabel>( p, TRect(Margin,Margin,Margin+width,Margin+2*vspace));
    lblValue->Caption = "Value";
    edtValue = ccl::CreateControl<TEdit>(lblValue, vspace);
    edtValue->Width = width;
    edtValue->Anchors = TAnchors() << akTop << akLeft << akRight;

    // . . . Prefix
    TLabel* lblPrefix = ccl::CreateControl<TLabel>(edtValue, vspace);
    lblPrefix->Caption = "Prefix";
    edtPrefix = ccl::CreateControl<TEdit>(lblPrefix, vspace);
    edtPrefix->Width = width;
    edtPrefix->Anchors = TAnchors() << akTop << akLeft << akRight;

    // . . . Vertical or horizontal?
    chkVertical = ccl::CreateControl<TCheckBox>(edtPrefix, vspace);
    chkVertical->Caption = "Vertical";
    chkVertical->Width = width/2;
    chkVertical->Alignment = taRightJustify;

// . . . Finally
    Revert(i_Marker); // Initial values
}

//---------------------------------------------------------------------------
void cls_MarkerEdit::Apply(cls_FoilObject* fo)
{ // Apply changes to linked foil object

    i_Marker->SetValue(edtValue->Text.ToDouble());
    i_Marker->Prefix = edtPrefix->Text;
    i_Marker->Vertical = chkVertical->Checked;
}

//---------------------------------------------------------------------------
void cls_MarkerEdit::Revert(cls_FoilObject* fo)
{ // Revert values to those of linked foil object

    edtValue->Text = String(i_Marker->Value);
    edtPrefix->Text = i_Marker->Prefix;
    chkVertical->Checked = i_Marker->Vertical;
}

//---------------------------------------------------------------------------
void cls_MarkerEdit::AlterOptions(cls_OptionsSet* o)
{ // Alter options
}


//////////////////////////////// cls_LabelEdit //////////////////////////////
//---------------------------------------------------------------------------
__fastcall cls_LabelEdit::cls_LabelEdit (TWinControl* p, cls_Label* l)
  : cls_PropertiesInterface(p,l), i_Label(l)
{ // Constructor

// . . . Interface instances (They're deleted by owner)
    int vspace = Margin / 2;

    // . . . Text
    TLabel* lblText = ccl::CreateControl<TLabel>( p, TRect(Margin,Margin,Margin+width,Margin+2*vspace));
    lblText->Caption = "Text";
    edtText = ccl::CreateControl<TEdit>(lblText, vspace);
    edtText->Width = width;
    edtText->Anchors = TAnchors() << akTop << akLeft << akRight;

    // . . . Coordinates
    TLabel* lblX = ccl::CreateControl<TLabel>(edtText, vspace);
    lblX->Caption = "X coordinate";
    edtX = ccl::CreateControl<TEdit>(lblX, vspace);
    edtX->Width = width;
    edtX->Anchors = TAnchors() << akTop << akLeft << akRight;

    TLabel* lblY = ccl::CreateControl<TLabel>(edtX, vspace);
    lblY->Caption = "Y coordinate";
    edtY = ccl::CreateControl<TEdit>(lblY, vspace);
    edtY->Width = width;
    edtY->Anchors = TAnchors() << akTop << akLeft << akRight;

    // . . . Vertical or horizontal?
    chkShowCoords = ccl::CreateControl<TCheckBox>(edtY, vspace);
    chkShowCoords->Caption = "Show coordinates";
    chkShowCoords->Width = width/2;
    chkShowCoords->Alignment = taRightJustify;

// . . . Finally
    Revert(i_Label); // Initial values
}

//---------------------------------------------------------------------------
void cls_LabelEdit::Apply(cls_FoilObject* fo)
{ // Apply changes to linked foil object

    i_Label->Text = edtText->Text;
    i_Label->x = edtX->Text.Trim().ToDouble();
    i_Label->y = edtY->Text.Trim().ToDouble();
    i_Label->ShowCoords = chkShowCoords->Checked;
}

//---------------------------------------------------------------------------
void cls_LabelEdit::Revert(cls_FoilObject* fo)
{ // Revert values to those of linked foil object

    edtText->Text = i_Label->Text;
    edtX->Text = String(i_Label->x);
    edtY->Text = String(i_Label->y);
    chkShowCoords->Checked = i_Label->ShowCoords;
}

//---------------------------------------------------------------------------
void cls_LabelEdit::AlterOptions(cls_OptionsSet* o)
{ // Alter options
}



/////////////////////////// cls_SimpleActionInterface ///////////////////////
//---------------------------------------------------------------------------
__fastcall cls_SimpleActionInterface::cls_SimpleActionInterface (TWinControl* p, cls_Frame* f, const String& c)
       : cls_PropertiesInterface(p,f), i_Frame(f)
{ // Constructor

// . . . Interface instances (They're deleted by owner)
    int vspace = 3*Margin;

    // . . . Operation button
    btnDo = ccl::CreateControl<TButton>(p, TRect(Margin,Margin,Margin+width,Margin+2*vspace));
    btnDo->Caption = c;
    btnDo->OnClick = OnClick; // TNotifyEvent h
    btnDo->Anchors = TAnchors() << akTop << akLeft << akRight;
}
__fastcall cls_SimpleActionInterface::~cls_SimpleActionInterface() {delete btnDo;}

//---------------------------------------------------------------------------
void __fastcall cls_SimpleActionInterface::OnClick (TObject* Sender)
{ // Fixed: Reassign colors
    i_Frame->ReassignTrackColors();
}


///////////////////////////// cls_FoilObjectsEdit ///////////////////////////
//---------------------------------------------------------------------------
void cls_FoilObjectsEdit::Apply(cls_FoilObject* fo)
{ // Apply changes to linked foil object

    for (typ_FoilObjectsIt fi=i_PObjects->begin(); fi!=i_PObjects->end(); ++fi)
        {
         cls_FoilObjectEdit::Apply( *fi );
        }
}

//---------------------------------------------------------------------------
void cls_FoilObjectsEdit::Revert(cls_FoilObject* fo)
{ // Revert values to those of linked foil object

    for (typ_FoilObjectsIt fi=i_PObjects->begin(); fi!=i_PObjects->end(); ++fi)
        {
         cls_FoilObjectEdit::Revert( *fi );
        }
}

/////////////////////////// cls_PlotFramesStyleEdit /////////////////////////
//---------------------------------------------------------------------------
void cls_PlotFramesStyleEdit::Apply(cls_FoilObject* fo)
{ // Apply changes to linked foil object

    for (typ_FoilObjectsIt fi=i_PObjects->begin(); fi!=i_PObjects->end(); ++fi)
        {
         cls_FoilObjectEdit::Apply( *fi );
        }
}

//---------------------------------------------------------------------------
void cls_PlotFramesStyleEdit::Revert(cls_FoilObject* fo)
{ // Revert values to those of linked foil object

    for (typ_FoilObjectsIt fi=i_PObjects->begin(); fi!=i_PObjects->end(); ++fi)
        {
         cls_FoilObjectEdit::Revert( *fi );
        }
}


//=============================== End of File ===============================



