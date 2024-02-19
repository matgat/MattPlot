//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "unt_SynEditor.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
TfrmSynEditor::TfrmSynEditor(TComponent* Owner, String pth) : TForm(Owner)
{ // Constructor

// . . . Initializations
        Caption = pth;

// . . . Finally
        Open(pth);
}


//------------------------------- Utilities ---------------------------------


//---------------------------------------------------------------------------
void TfrmSynEditor::Open( const String& p )
{ // Open a path

            synEditor->Lines->Clear();
            synEditor->Lines->LoadFromFile(p);
}

//---------------------------------------------------------------------------
void TfrmSynEditor::Save( const String& p )
{ // Save to path

            synEditor->Lines->SaveToFile(p);
}