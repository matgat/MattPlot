#ifndef unt_TrackTransformH
#define unt_TrackTransformH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <CheckLst.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class TfrmTrackTransform : public TForm
{
__published:	// IDE-managed Components
    TStatusBar *barStatus;
    TPanel *pnlBottom;
    TPageControl *pagOperators;
    TButton *btnDo;
    TRadioGroup *grpDestination;
    TTabSheet *tabUnary;
    TTabSheet *tabBinary;
    TPanel *pnlTracks;
    TGroupBox *grpSource;
    TComboBox *cbxFrames;
    TListBox *lstTracks;
    TPanel *pnlOperation;
    TGroupBox *grpOperation;
    TPageControl *pagUnaryOperators;
    TTabSheet *tabIndepVar;
    TLabel *lblReascaleDescription;
    TTabSheet *tabDepVar;
    TRadioGroup *rdgDepVarOps;
    TTabSheet *tabFilter;
    TLabel *lblFilterDescription;
    TLabel *lnlN;
    TImage *imgMeanFilterFormula;
    TEdit *edtAveragedSamples;
    TPanel *pnlTrack1;
    TGroupBox *grpTrack1;
    TComboBox *cbxFrameTrack1;
    TListBox *lstTrack1;
    TPanel *pnlTrack2;
    TGroupBox *grpTrack2;
    TComboBox *cbxFrameTrack2;
    TListBox *lstTrack2;
    TPanel *pnlBinaryOperator;
    TGroupBox *grpBinaryOperator;
    TComboBox *cbxBinaryOp;
    TTabSheet *tabTransform;
    TRadioGroup *rdgTransformations;
    TEdit *edtPowExponent;
    TLabel *Label1;
    TRadioGroup *rdgIndepVarOps;
    TEdit *edtOffset_t;
    TLabel *lblRescaleFormula;
    TLabel *Label2;
    TEdit *edtOffset_xy;
    TComboBox *edtScale_t;
    TComboBox *edtScale_xy;
    TButton *btnDone;
    TCheckBox *chkUpdateCombo;
    void __fastcall cbxFramesChange(TObject *Sender);
    void __fastcall tabUnaryShow(TObject *Sender);
    void __fastcall tabBinaryShow(TObject *Sender);
    void __fastcall cbxFrameTrack1Change(TObject *Sender);
    void __fastcall cbxFrameTrack2Change(TObject *Sender);
    void __fastcall btnDoneClick(TObject *Sender);
    void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
private:	// User declarations

    // Types
    enum en_UnaryOps {OP_RESCALE, OP_ABS, OP_FILTER};

    // Private attributes
    String *Message; // Output messages
    class cls_MPlot* Plot;

    // Services
    static void __fastcall UpdateShownFrames(class cls_MPlot*, class TComboBox*, bool); // Fill a frames combobox
    static void __fastcall UpdateShownTracks(class cls_Frame*, class TListBox*); // Fill a tracks listbox
    void __fastcall DoUnaryOp(TObject*);
    void __fastcall DoBinaryOp(TObject*);

public:		// User declarations
    __fastcall TfrmTrackTransform(TComponent*, class cls_MPlot*);
    __fastcall ~TfrmTrackTransform();

   // Message handling
   //void __fastcall WhenHelp (TMessage& Message);
   //void __fastcall WhenSysCommand (TMessage& Message);
   //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ MESSAGE HANDLERS
   //BEGIN_MESSAGE_MAP
     //VCL_MESSAGE_HANDLER(WM_HELP, TMessage, WhenHelp)
     //VCL_MESSAGE_HANDLER(WM_SYSCOMMAND, TMessage, WhenSysCommand)
   //END_MESSAGE_MAP(TForm)
   //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
};
//---------------------------------------------------------------------------
#endif
