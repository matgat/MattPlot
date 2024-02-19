//---------------------------------------------------------------------------

#ifndef unt_LogViewerH
#define unt_LogViewerH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmLogViewer : public TForm
{
__published:	// IDE-managed Components
        TMemo *memLog;
        TPanel *pnlBottom;
        TTrackBar *trkTransparency;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall trkTransparencyChange(TObject *Sender);
        void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
private:	// User declarations
        void __fastcall RePosition();
        TForm* frmOwner;
public:		// User declarations
        __fastcall TfrmLogViewer(TForm* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmLogViewer *frmLogViewer;
//---------------------------------------------------------------------------
#endif
