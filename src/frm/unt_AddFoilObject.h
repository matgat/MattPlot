//---------------------------------------------------------------------------
#ifndef unt_AddFoilObjectH
#define unt_AddFoilObjectH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmAddFoilObject : public TForm
{
__published:	// IDE-managed Components
    TPanel *pnlBottom;
    TBitBtn *btnApply;
    TBitBtn *btnDiscard;
    TPageControl *pagBody;
    TTabSheet *tabSpecific;
    TTabSheet *tabStyle;
    void __fastcall btnApplyClick(TObject *Sender);
    void __fastcall btnDiscardClick(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);
private:	// User declarations
    class cls_MPlot* Plot;
    class cls_FoilObject* FoilObj;
    class cls_PropertiesInterface *piSpec, *piStyle;
public:		// User declarations
    __fastcall TfrmAddFoilObject(TComponent* Owner, class cls_MPlot*, class cls_FoilObject*);
    __fastcall ~TfrmAddFoilObject();
};

//---------------------------------------------------------------------------
#endif
