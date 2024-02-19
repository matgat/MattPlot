//---------------------------------------------------------------------------

#ifndef frm_SimpleEditH
#define frm_SimpleEditH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmSimpleEdit : public TForm
{
__published:	// IDE-managed Components
        TPanel *pnlEdit;
        TPanel *pnlButtons;
        TBitBtn *btnLoad;
        TBitBtn *btnSave;
        TMemo *txtEdit;
        TPanel *pnlLeft;
        TPanel *pnlRight;
        void __fastcall btnLoadClick(TObject *Sender);
        void __fastcall btnSaveClick(TObject *Sender);
        void __fastcall pnlEditResize(TObject *Sender);
private:	// User declarations
        String File;
public:		// User declarations
        __fastcall TfrmSimpleEdit(TComponent* , String);
};
//---------------------------------------------------------------------------
#endif
