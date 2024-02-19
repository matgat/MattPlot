//---------------------------------------------------------------------------

#include <vcl.h>
#include "unt_MatCommon.h"
#pragma hdrstop

#include "unt_LogViewer.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//====================== Useful file scope declarations =====================
        TfrmLogViewer *frmLogViewer;

//---------------------------------------------------------------------------
__fastcall TfrmLogViewer::TfrmLogViewer(TForm* Owner) : TForm(Owner)
{
        frmOwner = Owner;
}

//---------------------------------------------------------------------------
void __fastcall TfrmLogViewer::RePosition()
{
        Top = frmOwner->Top + frmOwner->Height/4;
        Left = frmOwner->Left + frmOwner->Width/4;
        Width = 2*frmOwner->Width/3;
        Height = 3*frmOwner->Height/3;
}

//---------------------------------------------------------------------------
void __fastcall TfrmLogViewer::FormShow(TObject *Sender)
{
        // Reset things
        RePosition();
        trkTransparency->Position = 150;
}

//---------------------------------------------------------------------------

void __fastcall TfrmLogViewer::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
        if ( Key == VK_ESCAPE ) Close();
}

//---------------------------------------------------------------------------

void __fastcall TfrmLogViewer::trkTransparencyChange(TObject *Sender)
{
        mat::SetFormTransparence (this, trkTransparency->Position );
}

//---------------------------------------------------------------------------
