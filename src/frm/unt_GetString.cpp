//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "unt_GetString.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
void __fastcall TfrmGetString::edtGetStringKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
        if ( Key == VK_RETURN )
           {
            *InString = edtGetString->Text;
            Close();
           }
}
//---------------------------------------------------------------------------
