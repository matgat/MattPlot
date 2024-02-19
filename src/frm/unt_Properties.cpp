//---------------------------------------------------------------------------
#include <vcl.h>
#include "unt_MatCommon.h"
#pragma hdrstop
#include "unt_MPlotClasses.h"
#include "unt_MPlot.h"
#include "unt_PropertiesInterface.h"
#include "unt_OptionsSet.h" // Global options Opts (!) to store options
#include "unt_Properties.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//=============================== File scope ================================


//============================= Static Members ==============================


//======================= Form constructor/destructor =======================
//---------------------------------------------------------------------------
__fastcall TfrmProperties::TfrmProperties(TComponent* Owner, class cls_MPlot* p, class cls_FoilObject* fo)
    : TForm(Owner), Plot(p)
{ // Constructor
// . . . Defaults
    //Caption = Frame->Name + " - " + Frame->Title;
    ApplyChanges = false;
    PrevAutoApply = chkAutoApply->Checked;

// . . . Other
    UpdateTreeView(fo);
}

//---------------------------------------------------------------------------
__fastcall TfrmProperties::~TfrmProperties()
{ // Destructor
    ClearInterface(ApplyChanges);
}


//---------------------------------------------------------------------------


//================================= Utilities ===============================

//---------------------------------------------------------------------------
void __fastcall TfrmProperties::ClearInterface( const bool apply )
{ // Clear eventual opened interface

    for ( unsigned int i=0; i<Interfaces.size(); ++i )
        {
         if (apply) Interfaces[i]->Apply(); // Apply changes
         if ( chkSetAsDefault->Checked ) Interfaces[i]->AlterOptions(Opts); // Set options default
         delete Interfaces[i];
        }
    Interfaces.clear();
    while ( pagMain->PageCount ) delete pagMain->Pages[0];

    Plot->Invalidate();
    Application->ProcessMessages();
}

//---------------------------------------------------------------------------
void __fastcall TfrmProperties::UpdateTreeView(class cls_FoilObject* fo)
{ // Rebuild Plot TreeView

    Plot->FillTreeView(treePlot);
    treePlot->FullCollapse();
    // Select passed object entry
    treePlot->Selected = FindTreeObject(fo);
    /*
    if ( treePlot->Selected )
       {
        //treePlot->Selected->MakeVisible(); //Expand(true);
        //treePlot->Focused = treePlot->Selected;
       }
    */
}


//---------------------------------------------------------------------------
TTreeNode* TfrmProperties::FindTreeObject(class cls_FoilObject* fo)
{ // Find an object in TreeView

    TTreeNode* nCurr = treePlot->Items->GetFirstNode();
    while ( nCurr )
        {
         if ( fo == nCurr->Data ) return nCurr;
         else nCurr = nCurr->GetNext();
        }
    return 0;
}



//================================== Events =================================

//---------------------------------------------------------------------------
void __fastcall TfrmProperties::btnApplyClick(TObject *Sender)
{ // Apply changes

    ApplyChanges = true; // Work is done in destructor
    if ( chkSetAsDefault->Checked ) ModalResult = mrYes; // Got to store options
    else ModalResult = mrNo;
}

//---------------------------------------------------------------------------
void __fastcall TfrmProperties::btnDiscardClick(TObject *Sender)
{ // Discard changes

    ApplyChanges = false;
    if ( chkSetAsDefault->Checked ) ModalResult = mrYes; // Got to store options
    else ModalResult = mrNo;
}


//---------------------------------------------------------------------------
void __fastcall TfrmProperties::treePlotChange(TObject *Sender, TTreeNode *Node)
{
    //TreeView1->Selected->Text TreeView1->Selected->Index TreeView1->Selected->Level

// . . . Apply changes?
    // TODO 4: if modified...
    // if ( mat::MsgAsk("Do you want to apply changes?") )

// . . . Clear eventual opened interface
    ClearInterface(chkAutoApply->Checked);

// . . . See selected object
    cls_PropertiesInterface* ptmp; // Avoid W8030
    if ( Node->Level == 0 )  // The root node, group of frames
         {
          // Avoid automatic apply to contained objects
          if (chkAutoApply->Checked) {
                                      chkAutoApply->Checked = false;
                                      PrevAutoApply = true;
                                     }
          // Set frames common style
          TTabSheet* tabStyle = new TTabSheet(pagMain);
          tabStyle->PageControl = pagMain;
          tabStyle->Caption = "Set frames common style";
          ptmp = new cls_PlotFramesStyleEdit(tabStyle,reinterpret_cast<cls_PlotFramesStyleEdit::typ_FoilObjects*>(Node->Data));
          Interfaces.push_back(ptmp);
         }
    else if ( Node->Text == "Tracks" ) // Group of tracks
         {
          // Avoid automatic apply to contained objects
          if (chkAutoApply->Checked) {
                                      chkAutoApply->Checked = false;
                                      PrevAutoApply = true;
                                     }
          TTabSheet* tabSpecific = new TTabSheet(pagMain);
          tabSpecific->PageControl = pagMain;
          tabSpecific->Caption = "Contained tracks";
          ptmp = new cls_SimpleActionInterface(tabSpecific,reinterpret_cast<cls_Frame*>(Node->Parent->Data),"Reassign track colors");
          Interfaces.push_back(ptmp);
         }
    else if ( Node->Text == cls_Markers::Name ) // Group of markers
         {
          // Avoid automatic apply to contained objects
          if (chkAutoApply->Checked) {
                                      chkAutoApply->Checked = false;
                                      PrevAutoApply = true;
                                     }
          // Set markers common style
          TTabSheet* tabStyle = new TTabSheet(pagMain);
          tabStyle->PageControl = pagMain;
          tabStyle->Caption = "Set markers common style";
          ptmp = new cls_FoilObjectsEdit(tabStyle,reinterpret_cast<cls_FoilObjectsEdit::typ_FoilObjects*>(Node->Data));
          Interfaces.push_back(ptmp);
         }
    else if ( Node->Text == cls_Labels::Name ) // Group of text labels
         {
          // Avoid automatic apply to contained objects
          if (chkAutoApply->Checked) {
                                      chkAutoApply->Checked = false;
                                      PrevAutoApply = true;
                                     }
          // Set text labels common style
          TTabSheet* tabStyle = new TTabSheet(pagMain);
          tabStyle->PageControl = pagMain;
          tabStyle->Caption = "Set text labels common style";
          ptmp = new cls_FoilObjectsEdit(tabStyle,reinterpret_cast<cls_FoilObjectsEdit::typ_FoilObjects*>(Node->Data));
          Interfaces.push_back(ptmp);
         }
    else { // One object

          // Restore setting
          chkAutoApply->Checked = PrevAutoApply;

          cls_FoilObject* fo = reinterpret_cast<cls_FoilObject*>(Node->Data);

          // . . . Create interface
          if ( fo )
             {
              // Tab specific
              TTabSheet* tabSpecific = new TTabSheet(pagMain);
              tabSpecific->PageControl = pagMain;
              tabSpecific->Caption = "Specific";
              Interfaces.push_back( fo->CreateSpecInterface(tabSpecific) );

              // Tab style
              TTabSheet* tabStyle = new TTabSheet(pagMain);
              tabStyle->PageControl = pagMain;
              tabStyle->Caption = "Style";
              Interfaces.push_back( fo->CreateStyleInterface(tabStyle) );
             }
         }
}



//---------------------------------------------------------------------------
void __fastcall TfrmProperties::treePlotDragOver(TObject *Sender,TObject *Source, int X, int Y, TDragState State, bool &Accept)
{ // Decide if accept drop operation

    Accept = false; // Default
    //if ( Sender->ClassType() == __classid(TTreeView) ) ShowMessage("drag over sender is a TTreeView");
    //TTreeView* tree = dynamic_cast<TTreeView*>(Sender);

    // Must be internal drag and drop of 'treePlot'
    if ( treePlot == Sender && Sender == Source )
       { // Find involved nodes
        TTreeNode* DstNode = treePlot->GetNodeAt(X,Y);
        TTreeNode* SrcNode = treePlot->Selected;

        if ( DstNode && SrcNode )
           {
            // different parent (DstNode->Parent == SrcNode->Parent)
            Accept = ( (SrcNode->Level==1) && (DstNode->Level==1) ) || // Frame to frame
                     ( (SrcNode->Level==3) && (DstNode->Level==1) ) || // Track to frame
                     ( (SrcNode->Level==3) && (DstNode->Level==3) );   // Track to track
           }
       }
}

//---------------------------------------------------------------------------
void __fastcall TfrmProperties::treePlotDragDrop(TObject *Sender, TObject *Source, int X, int Y)
{ // Drop operation: if here, conditions in 'DragOver' are met

// . . . Get node datas
    TTreeView* tree = dynamic_cast<TTreeView*>(Sender);
    TTreeNode* DstNode = tree->GetNodeAt(X,Y);
    TTreeNode* SrcNode = tree->Selected;
    cls_FoilObject* o1 = reinterpret_cast<cls_FoilObject*>(SrcNode->Data);
    cls_FoilObject* o2 = reinterpret_cast<cls_FoilObject*>(DstNode->Data);
    tree->EndDrag(false);

// . . . Perform operation depending on objects
    cls_Frame* fr1 = dynamic_cast<cls_Frame*>(o1);
    cls_Frame* fr2 = dynamic_cast<cls_Frame*>(o2);
    cls_Track* tr1 = dynamic_cast<cls_Track*>(o1);
    cls_Track* tr2 = dynamic_cast<cls_Track*>(o2);
    if ( fr1 && fr2 )
       {
        Plot->MoveFrame(fr1,fr2);
        Plot->Invalidate();
        UpdateTreeView(o1);
       }
    else if ( tr1 && tr2 )
       {
        tr1->Owner->MoveTrack(tr1,tr2);
        Plot->Invalidate();
        UpdateTreeView(o1);
       }
    else if ( tr1 && fr2 )
       {
        tr1->Owner->MoveTrack(tr1,fr2);
        Plot->Invalidate();
        UpdateTreeView(o1);
       }
}

//---------------------------------------------------------------------------
void __fastcall TfrmProperties::chkAutoApplyClick(TObject *Sender)
{
    PrevAutoApply = chkAutoApply->Checked;
}

//=============================== End of File ===============================



