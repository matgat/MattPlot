//---------------------------------------------------------------------------

#ifndef unt_GetStringH
#define unt_GetStringH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TfrmGetString : public TForm
{
__published:	// IDE-managed Components
        TEdit *edtGetString;
        void __fastcall edtGetStringKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
private:	// User declarationsù
        String* InString;
public:		// User declarations
        __fastcall TfrmGetString(TComponent* Owner, String* instr) : InString(instr),TForm(Owner)
                   {Left=Mouse->CursorPos.x;Top=Mouse->CursorPos.y;edtGetString->Text=*instr;}
};
//---------------------------------------------------------------------------
#endif
