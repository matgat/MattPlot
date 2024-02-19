//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "frm_SimpleEdit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TfrmSimpleEdit::TfrmSimpleEdit(TComponent* Owner, String filepath)
        : TForm(Owner)
{

// . . . Inputs
        File = filepath;

// . . . Initializations
        btnLoadClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TfrmSimpleEdit::btnLoadClick(TObject *Sender)
{
        txtEdit->Lines->LoadFromFile(File);
}
//---------------------------------------------------------------------------

void __fastcall TfrmSimpleEdit::btnSaveClick(TObject *Sender)
{
        txtEdit->Lines->SaveToFile(File);
        Close();
}
//---------------------------------------------------------------------------

void __fastcall TfrmSimpleEdit::pnlEditResize(TObject *Sender)
{ // magnify text at resize
        txtEdit->Font->Size = pnlEdit->Width/35;
}
//---------------------------------------------------------------------------

