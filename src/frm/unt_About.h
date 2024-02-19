//---------------------------------------------------------------------------

#ifndef unt_AboutH
#define unt_AboutH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmAbout : public TForm
{
__published:	// IDE-managed Components
    TPaintBox *pbxBackground;
    TTimer *tmrInvalidate;
    void __fastcall pbxBackgroundPaint(TObject *Sender);
    void __fastcall pbxBackgroundClick(TObject *Sender);
    void __fastcall tmrInvalidateTimer(TObject *Sender);
private:	// User declarations
    inline void __fastcall Put (const String&, int =0);
    int Margin;
    mutable int x,y;
    const class cls_ThisExeFile* ExeInfo;
    int Transparency;
    bool Abort;

public:		// User declarations
// . . . Public methods
    __fastcall TfrmAbout(TControl*, const class cls_ThisExeFile*);
    //__fastcall ~TfrmAbout(); // Destructor

    void __fastcall FadeIn(const int); // Give fade time in [ms]
    void __fastcall FadeOut(const int);
    void __fastcall WaitClose();

// . . . Public attributes (!)
    int PaintTime; // [ms]

    // . . . Static members
    //cls_SCSlogo TfrmAbout::logo;
    static const int AlphaMax; // [0-255]
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#endif
