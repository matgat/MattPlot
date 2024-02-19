//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "unt_AddFoilObject.h"
#include "unt_MPlotClasses.h"
#include "unt_MPlot.h"
#include "unt_PropertiesInterface.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//cls_MarkerEdit* me;
cls_PropertiesInterface* me;

//---------------------------------------------------------------------------
__fastcall TfrmAddFoilObject::TfrmAddFoilObject(TComponent* Owner, cls_MPlot* p, cls_FoilObject* fo)
: TForm(Owner), Plot(p), FoilObj(fo)
{
    piSpec = FoilObj->CreateSpecInterface(tabSpecific);
    piStyle = FoilObj->CreateStyleInterface(tabStyle);

    FormResize(this);
    Caption = String("Add object to ") + Plot->CurrentFrame->Name;
}

//---------------------------------------------------------------------------
__fastcall TfrmAddFoilObject::~TfrmAddFoilObject()
{
    delete piSpec;
    delete piStyle;
}

//---------------------------------------------------------------------------
void __fastcall TfrmAddFoilObject::btnApplyClick(TObject *Sender)
{
    piSpec->Apply(FoilObj); // Apply changes
    piStyle->Apply(FoilObj); // Apply changes
    ModalResult = mrOk;
}

//---------------------------------------------------------------------------
void __fastcall TfrmAddFoilObject::btnDiscardClick(TObject *Sender)
{
    ModalResult = mrCancel;
}

//---------------------------------------------------------------------------
void __fastcall TfrmAddFoilObject::FormResize(TObject *Sender)
{ // Adapt buttons

    btnApply->Width = (pnlBottom->Width - 3*btnApply->Left) / 2;
    btnDiscard->Width = btnApply->Width;
    btnDiscard->Left = 2*btnApply->Left + btnApply->Width;
}
//---------------------------------------------------------------------------

