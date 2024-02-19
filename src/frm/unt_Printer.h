//---------------------------------------------------------------------------
#ifndef unt_PrinterH
#define unt_PrinterH

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * *
    *
    *        unt_Printer   2007 Matteo Gattanini
    *                ver: 02    (18.dic.2007)
    *
    *        This unit defines some facilities for printing
    *
    *        Example of usage:
    *        #include "unt_Printer.h"
    *
    * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <SysUtils.hpp>
#include <Graphics.hpp> // for TBitmap
#include <Printers.hpp> // for TPrinter
#include <Windows.hpp>
#include <Buttons.hpp>


//---------------------------------------------------------------------------
namespace nms_MatPrint
{//:::::::::::::::::::::::::::: Namespace content :::::::::::::::::::::::::::


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Types definition
    //.........................................................
    // . . . Print types
    typedef enum
       {
        PR_NONE       =  0,
        PR_SCREEN     =  1,
        PR_PAPER      =  2
       } E_PRINTMODE;


    //-------------------------------------------------------------------
    class cls_ExtraPrnOpts {
    public://                                        . . . Public methods
      cls_ExtraPrnOpts() {
                          Mode = PR_NONE;
                          NoBackground = false;
                         }
      //~cls_ExtraPrnOpts();

      // . . . Public attributes (!)
      E_PRINTMODE Mode;
      bool NoBackground;

    private://                                   . . . Private attributes
      // . . . Private methods
    };//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



//---------------------------- Functions prototypes

    TModalResult ChooseExtraPrintOptions(class cls_ExtraPrnOpts*); // Show a dialog to choose print mode

    //~~~~~~~~~~~~~~~~~~~~~~~~ Printing bitmaps
    void StretchDrawDIB(Graphics::TCanvas* DestCanvas, // Stretch a bitmap to fit in a destination tect in acanvas
                        const TRect& DestRect,
                        Graphics::TBitmap* SourceBitmap);

/*
//---------------------------- Classes
////////////////////////////// cls_ /////////////////////////////
//---------------------------------------------------------------------------
class cls_ : public cls_ { //

public://                                                       ~~~~~~~~~~~~~
// . . .  Types definitions

// . . . Public methods
        cls_ () : cls_() {}

// . . . Properties

// . . . Public attributes (!)

protected://                                                    ~~~~~~~~~~~~~

// . . . Protected methods

private://                                                      ~~~~~~~~~~~~~

// . . . Private attributes
// . . . Properties
// . . . Private methods

};//-------------------------------------------------------------------------
*/

}//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
namespace prn = nms_MatPrint; // a short alias for this namespace


/////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
class TfrmChooseExtraPrnOps : public TForm
{
__published:	// IDE-managed Components
    TBitBtn *btnPriintScreen;
    TBitBtn *btnPrintPaper;
    TCheckBox *chkNoBackground;
    void __fastcall btnPriintScreenClick(TObject *Sender);
    void __fastcall btnPrintPaperClick(TObject *Sender);
    void __fastcall FormDblClick(TObject *Sender);
private:	// User declarations

    // Print options container
    class nms_MatPrint::cls_ExtraPrnOpts* Opts;

    MESSAGE void __fastcall WhenHelp (TMessage& Message);
    //MESSAGE void __fastcall WhenSysCommand (TMessage& Message);
   //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ MESSAGE HANDLERS
   BEGIN_MESSAGE_MAP
     VCL_MESSAGE_HANDLER(WM_HELP, TMessage, WhenHelp)
     //VCL_MESSAGE_HANDLER(WM_SYSCOMMAND, TMessage, WhenSysCommand)
   END_MESSAGE_MAP(TForm)
   //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

public:		// User declarations
    __fastcall TfrmChooseExtraPrnOps(TComponent* Owner,nms_MatPrint::cls_ExtraPrnOpts*);
};

//---------------------------------------------------------------------------
#endif
