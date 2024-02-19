//---------------------------------------------------------------------------

#ifndef unt_OutputTextH
#define unt_OutputTextH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmOutputText : public TForm
{
__published:	// IDE-managed Components
        TPaintBox *pbxBackground;
        void __fastcall pbxBackgroundPaint(TObject *Sender);
        void __fastcall pbxBackgroundClick(TObject *Sender);
private:	// User declarations

        int Margin;
        mutable int y;
        inline void __fastcall Put (String);
        const class cls_ThisExeFile* ExeInfo;

public:		// User declarations
        __fastcall TfrmOutputText(TComponent*);
        //__fastcall ~TfrmAbout(); // Destructor

        String Text;
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#endif
