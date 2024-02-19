//---------------------------------------------------------------------------
#ifndef unt_PropertiesH
#define unt_PropertiesH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <vector> // for interface pointers

/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
class TfrmProperties : public TForm
{
__published:	// IDE-managed Components
    TPanel *pnlBottom;
    TBitBtn *btnApply;
    TBitBtn *btnDiscard;
    TPageControl *pagMain;
    TTreeView *treePlot;
    TSplitter *splVertical;
    TImageList *imlTree;
    TCheckBox *chkAutoApply;
    TCheckBox *chkSetAsDefault;
    void __fastcall btnDiscardClick(TObject *Sender);
    void __fastcall btnApplyClick(TObject *Sender);
    void __fastcall treePlotChange(TObject *Sender, TTreeNode *Node);
    void __fastcall treePlotDragOver(TObject *Sender, TObject *Source, int X, int Y, TDragState State, bool &Accept);
    void __fastcall treePlotDragDrop(TObject *Sender, TObject *Source, int X, int Y);
    void __fastcall chkAutoApplyClick(TObject *Sender);
private:	// User declarations
    // . . . Objects
    class cls_MPlot* Plot;
    bool ApplyChanges; // When leaving interface
    bool PrevAutoApply; // When restoring chkAutoApply
    std::vector<class cls_PropertiesInterface*> Interfaces;

    // . . . Utilities
    void __fastcall ClearInterface(const bool); // Clear eventual opened interface
    void __fastcall UpdateTreeView(class cls_FoilObject*); // Rebuild Plot TreeView
    TTreeNode* FindTreeObject(class cls_FoilObject*); // Find an object in TreeView

public:		// User declarations
    __fastcall TfrmProperties(TComponent*, class cls_MPlot*, class cls_FoilObject*);
    __fastcall ~TfrmProperties();
};
//---------------------------------------------------------------------------
#endif
