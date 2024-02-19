#include <vcl.h>
#pragma hdrstop
#include "unt_TrackTransform.h"
#include "unt_MPlot.h"
#include "unt_MainForm.h"
#include ".\\hlp\\chm\\hlp_MattPlot.h" // Help topics IDs
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constructor
__fastcall TfrmTrackTransform::TfrmTrackTransform(TComponent* Owner, class cls_MPlot* p)
     : TForm(Owner), Plot(p)
{
    //pagOperators->ActivePageIndex = 0;
    cbxBinaryOp->ItemIndex = 0;
    Message = &(barStatus->Panels->Items[0]->Text);
}

//---------------------------------------------------------------------------
// Destructor
__fastcall TfrmTrackTransform::~TfrmTrackTransform()
{
}

//---------------------------------------------------------------------------
// Fill a frames combobox
void __fastcall TfrmTrackTransform::UpdateShownFrames(cls_MPlot* plt, TComboBox* cbx, bool selcurr)
{
    cbx->Items->Clear();
    plt->FillStringList(cbx->Items);

// . Select CurrentFrame
    if(selcurr)
       {
        for (int i=0; i<cbx->Items->Count; ++i)
            {
             if ( reinterpret_cast<class cls_Frame*>(cbx->Items->Objects[i]) == plt->CurrentFrame )
                {
                 cbx->ItemIndex = i;
                 break;
                }
            }
       }
    cbx->OnChange(cbx); // Force tracks refresh
}

//---------------------------------------------------------------------------
// Fill a track listbox
void __fastcall TfrmTrackTransform::UpdateShownTracks(cls_Frame* frm, TListBox* lst)
{
    lst->Items->Clear();
    frm->FillStringList(lst->Items);

// . Select Frames's CurrentTrack
    for (int i=0; i<lst->Items->Count; ++i)
        {
         if ( reinterpret_cast<cls_Track*>(lst->Items->Objects[i]) == frm->CurrentTrack )
            {
             lst->ItemIndex = i;
             if (lst->MultiSelect) lst->Selected[i] = true;
             break;
            }
        }
}

//----------------------------------- Events --------------------------------

/*---------------------------------------------------------------------------
void __fastcall TfrmTrackTransform::WhenHelp (TMessage& Message)
{ // Pass help messages to main form
    //::SendMessage(Application->MainForm->Handle,WM_HELP,0,22);
    bool CallHelp;
    frmMplotMain->appEventsHelp(HELP_CONTEXT, IDH_Transforming, CallHelp);
}*/

//---------------------------------------------------------------------------
// Do unary operation
void __fastcall TfrmTrackTransform::DoUnaryOp(TObject *Sender)
{
// . Settings
    // TODO 4: progress handler passed to elab functions
    cls_Frame* fp; // A bag for current frame
    cls_Track* tp; // A bag

// . Get values
    double k =1, o =0;
    double pe =2;
    if ( pagUnaryOperators->ActivePageIndex == tabIndepVar->PageIndex )
         {
          try {k = edtScale_t->Text.ToDouble();}
          catch (EConvertError& e) {edtScale_t->Text=String(k); *Message = "Invalid scale"; return;}
          try {o = edtOffset_t->Text.ToDouble();}
          catch (EConvertError& e) {edtOffset_t->Text=String(o); barStatus->Panels->Items[0]->Text = "Invalid offset"; return;}
         }
    else if ( pagUnaryOperators->ActivePageIndex == tabDepVar->PageIndex )
         {
          try {k = edtScale_xy->Text.ToDouble();}
          catch (EConvertError& e) {edtScale_xy->Text=String(k); *Message = "Invalid scale"; return;}
          try {o = edtOffset_xy->Text.ToDouble();}
          catch (EConvertError& e) {edtOffset_xy->Text=String(o); barStatus->Panels->Items[0]->Text = "Invalid offset"; return;}
          try {pe = edtPowExponent->Text.ToDouble();}
          catch (EConvertError& e) {edtPowExponent->Text=String(pe); *Message = "Invalid pow exponent"; return;}
         }

// . Get sources
    std::vector<cls_Track*> SelectedTracks;
    std::vector<cls_Track*>::iterator ti;
    for (int i=0; i<lstTracks->Items->Count; ++i)
        {
         if ( lstTracks->Selected[i] )
            {
             tp = reinterpret_cast<cls_Track*>(lstTracks->Items->Objects[i]);
             if (tp) SelectedTracks.push_back(tp);
            }
        }
    if ( SelectedTracks.empty() )
       {
        *Message = "You should select one or more tracks";
        return;
       }

// . Prepare destination
    bool newtracks = (grpDestination->ItemIndex > 0);
    bool newframe = (grpDestination->ItemIndex == 2);

    if (newframe) fp = Plot->PrepareNewFrame();
    else fp = Plot->CurrentFrame;

// . Apply to selected tracks
    for ( ti=SelectedTracks.begin(); ti!=SelectedTracks.end(); ++ti )
        {
         *Message = "Computing track " + (*ti)->Name + " ...";
         Application->ProcessMessages();

         // . See frame
         if (!newframe) fp = (*ti)->Owner;

         // . Prepare destination track
         if ( newtracks )
            {
             tp = (*ti)->Clone();
             if (tp) {
                      tp->LineColor = fp->GetNextTrackColor();
                      fp->InsertTrack( tp );
                     }
             else    {
                      *Message = "Unable to create new track";
                      return;
                     }
            }
         else tp = *ti;

         // . See operator
         if ( pagUnaryOperators->ActivePageIndex == tabIndepVar->PageIndex )
              {
               switch ( rdgIndepVarOps->ItemIndex )
                      {
                       case 0: // Rescale/Shift
                           if (newtracks) tp->Name = tp->Name + "_rescaledt";
                           tp->RescaleShift(k,o,false);
                           break;
                       case 1: // Reverse
                           if (newtracks) tp->Name = tp->Name + "_rev";
                           tp->ReverseIndepVar();
                           break;
                      } // end 'switch indep var op'
              }
         else if ( pagUnaryOperators->ActivePageIndex == tabDepVar->PageIndex )
              {
               switch ( rdgDepVarOps->ItemIndex )
                      {
                       case 0: // Rescale/Shift
                           if (newtracks) tp->Name = tp->Name + "_rescaled";
                           tp->RescaleShift(k,o,true);
                           break;
                       case 1: // Absolute value
                           if (newtracks) tp->Name = tp->Name + "_abs";
                           tp->abs();
                           break;
                       case 2: // Power
                           if (newtracks) tp->Name = tp->Name + "_pow";
                           tp->pow(pe);
                           break;
                       case 3: // Natural exponential
                           if (newtracks) tp->Name = tp->Name + "_exp";
                           tp->exp();
                           break;
                       case 4: // Natural logarithm
                           if (newtracks) tp->Name = tp->Name + "_log";
                           tp->log();
                           break;
                       case 5: // Base2 logarithm
                           if (newtracks) tp->Name = tp->Name + "_log2";
                           tp->log2();
                           break;
                       case 6: // Mask 16bit
                           if (newtracks) tp->Name = tp->Name + "_m16";
                           tp->mask16();
                           break;
                       case 7: // Cast to unsigned int 16bit
                           if (newtracks) tp->Name = tp->Name + "_u16";
                           tp->cast_unsigned16();
                           break;
                      } // end 'switch NonlinOp'
              }
         else if ( pagUnaryOperators->ActivePageIndex == tabFilter->PageIndex )
              {
               if (newtracks) tp->Name = tp->Name + "_filtered";
               int fltnum = edtAveragedSamples->Text.ToIntDef(10);
               edtAveragedSamples->Text = String(fltnum);
               tp->Filter(fltnum);
              }
         else if ( pagUnaryOperators->ActivePageIndex == tabTransform->PageIndex )
              {
               switch ( rdgTransformations->ItemIndex )
                      {
                       case 0: // Spectrum (fft)
                           if (newframe) {
                                          fp->Title = "Spectrum";
                                          fp->Axes->Xlabel = "f [Hz]";
                                         }
                           if (newtracks) tp->Name = tp->Name + "_spect";
                           // Stems
                           tp->PointStyle = 'd';
                           tp->LineWeight = 1;
                           tp->AreaColor = fp->GetNextTrackColor();
                           tp->MarkPoints = true;
                           tp->StrokeLines = false;
                           tp->FillArea = false;
                           tp->DrawHistograms = true;
                           tp->HistWidthPerc = 0;
                           tp->AreaStyle = bsSolid;

                           if ( !tp->IsEvenlySpaced() ) ::ShowMessage("Track data not evenly spaced, spectrum will be approximate");

                           tp->Spect(true);
                           break;

                       case 1: // Spectrum (dft)
                           if (newframe) {
                                          fp->Title = "Spectrum";
                                          fp->Axes->Xlabel = "f [Hz]";
                                         }
                           if (newtracks) tp->Name = tp->Name + "_spect";
                           // Stems
                           tp->PointStyle = 'd';
                           tp->LineWeight = 1;
                           tp->AreaColor = fp->GetNextTrackColor();
                           tp->MarkPoints = true;
                           tp->StrokeLines = false;
                           tp->FillArea = false;
                           tp->DrawHistograms = true;
                           tp->HistWidthPerc = 0;
                           tp->AreaStyle = bsSolid;

                           tp->Spect(false);
                           break;

                       case 2: // Derivative
                           if (newframe) {
                                          fp->Title = "Derivative";
                                         }
                           if (newtracks) tp->Name = tp->Name + "\'";
                           tp->Derivative();
                           break;

                       case 3: // Integrate
                           if (newframe) {
                                          fp->Title = "Integral";
                                         }
                           if (newtracks) tp->Name = tp->Name + "_int";
                           tp->Integral(0);
                           break;
                      } // end 'switch Transformations'
              }
        } // end 'for SelectedTracks'

// . Finally
    UpdateShownFrames(Plot, cbxFrames, chkUpdateCombo->Checked);
    *Message = "Done";
    fp->FitScale(true);
    Plot->Invalidate();
}

//---------------------------------------------------------------------------
// Do binary operation
void __fastcall TfrmTrackTransform::DoBinaryOp(TObject *Sender)
{

// . Settings
    // TODO 4: progress handler passed to elab functions
    cls_Frame* fp; // A bag for current frame
    cls_Track *t1 =0, *t2 =0, *tr =0; // Pointers for operands and result

// . Get track1 and track2
    for (int i=0; i<lstTrack1->Items->Count; ++i)
        {
         if ( lstTrack1->Selected[i] ) t1 = reinterpret_cast<cls_Track*>(lstTrack1->Items->Objects[i]);
        }
    for (int i=0; i<lstTrack2->Items->Count; ++i)
        {
         if ( lstTrack2->Selected[i] ) t2 = reinterpret_cast<cls_Track*>(lstTrack2->Items->Objects[i]);
        }

    if ( !t1 || !t2 )
       {
        *Message = "You should select two operands";
        return;
       }

// . Prepare destination frame
    bool newtracks = (grpDestination->ItemIndex > 0);
    bool newframe = (grpDestination->ItemIndex == 2);

    if (newframe) fp = Plot->PrepareNewFrame();
    else fp = t1->Owner;
    //Plot->CurrentFrame = fp;

// . Do operation
    *Message = "Computing " + t1->Name + cbxBinaryOp->Text + t2->Name;
    Application->ProcessMessages();

    // . Prepare destination track
    if ( newtracks )
        {
         tr = t1->Clone();
         if (tr) {
                  tr->Name = tr->Name;
                  tr->LineColor = fp->GetNextTrackColor();
                  fp->InsertTrack( tr );
                 }
         else    {
                  *Message = "Unable to create new track";
                  return;
                 }
        }
    else tr = t1;

    // . See operator
    switch ( cbxBinaryOp->ItemIndex )
        {
         case 0 : //______________________________ +
             if (newtracks) tr->Name = tr->Name + "+" + t2->Name;
             tr->Sum(*t2);
             break;

         case 1 : //______________________________ -
             if (newtracks) tr->Name = tr->Name + "-" + t2->Name;
             tr->Subtract(*t2);
             break;

         case 2 : //______________________________ *
             if (newtracks) tr->Name = tr->Name + "·" + t2->Name;
             tr->Multiply(*t2);
             break;

         case 3 : //______________________________ /
             if (newtracks) tr->Name = tr->Name + "/" + t2->Name;
             tr->Divide(*t2);
             break;

         case 4 : //______________________________ |x|
             if (newtracks) tr->Name = "V("+tr->Name + "²+" + t2->Name+"²)";
             tr->CombineMod(*t2);
             break;

         case 5 : //______________________________ atan
             if (newtracks) tr->Name = "atan("+tr->Name+"/"+t2->Name+")";
             tr->Atan(*t2);
             break;

         case 6 : //______________________________ Custom1
             if (newtracks) tr->Name = "F1("+tr->Name+","+t2->Name+")";
             tr->Custom1(*t2);
             break;
         // TODO: case x : //______________________________ + j·
         // combine as cplx
        } // end 'switch binary op'

// . Finally
    UpdateShownFrames(Plot, cbxFrameTrack1, chkUpdateCombo->Checked);
    UpdateShownFrames(Plot, cbxFrameTrack2, chkUpdateCombo->Checked);
    *Message = "Done";
    fp->FitScale(true);
    Plot->Invalidate();
}

//---------------------------------------------------------------------------
void __fastcall TfrmTrackTransform::tabUnaryShow(TObject *Sender)
{
    btnDo->OnClick = DoUnaryOp;
    UpdateShownFrames(Plot, cbxFrames, chkUpdateCombo->Checked);
}

//---------------------------------------------------------------------------
void __fastcall TfrmTrackTransform::tabBinaryShow(TObject *Sender)
{
    btnDo->OnClick = DoBinaryOp;
    UpdateShownFrames(Plot, cbxFrameTrack1, chkUpdateCombo->Checked);
    UpdateShownFrames(Plot, cbxFrameTrack2, chkUpdateCombo->Checked);
}

//---------------------------------------------------------------------------
void __fastcall TfrmTrackTransform::cbxFramesChange(TObject *Sender)
{ // See selected frame and its tracks

    cls_Frame* selFrame = reinterpret_cast<cls_Frame*>(cbxFrames->Items->Objects[cbxFrames->ItemIndex]);
    if ( selFrame ) UpdateShownTracks(selFrame, lstTracks);
}

//---------------------------------------------------------------------------
void __fastcall TfrmTrackTransform::cbxFrameTrack1Change(TObject *Sender)
{ // See selected frame and its tracks

    cls_Frame* selFrame = reinterpret_cast<cls_Frame*>(cbxFrameTrack1->Items->Objects[cbxFrameTrack1->ItemIndex]);
    if ( selFrame ) UpdateShownTracks(selFrame, lstTrack1);
}

//---------------------------------------------------------------------------
void __fastcall TfrmTrackTransform::cbxFrameTrack2Change(TObject *Sender)
{ // See selected frame and its tracks

    cls_Frame* selFrame = reinterpret_cast<cls_Frame*>(cbxFrameTrack2->Items->Objects[cbxFrameTrack2->ItemIndex]);
    if ( selFrame ) UpdateShownTracks(selFrame, lstTrack2);
}


//---------------------------------------------------------------------------
void __fastcall TfrmTrackTransform::btnDoneClick(TObject *Sender)
{
    //ModalResult = mrOk;
    Close();
}

//---------------------------------------------------------------------------
void __fastcall TfrmTrackTransform::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
    if(Key==VK_ESCAPE) Close();
}
//---------------------------------------------------------------------------

