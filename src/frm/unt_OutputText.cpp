//---------------------------------------------------------------------------
#include <vcl.h>
#include "unt_MatCommon.h"
#pragma hdrstop
#include "unt_OutputText.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"


//====================== Useful file scope declarations =====================
        //static const TColor clBorder = TColor(0xE37C27);
        //static const TColor clBackground = TColor (0xE7FCFF);
        //static const TColor clText = TColor (0x562C00);

//============================= Public methods ==============================

//---------------------------------------------------------------------------
__fastcall TfrmOutputText::TfrmOutputText(TComponent* Owner) : TForm(Owner)
{ // Constructor

// . . . Defaults
        DoubleBuffered = true;
        Text = "ciao";
        TForm* Form = dynamic_cast<TForm*>(Owner);
        if ( Form )
           {
            Font->Assign(Form->Font);
            Color = Form->Color;
           }

        pbxBackground->Canvas->Brush->Color = Color;
        //pbxBackground->Canvas->Font->Color = clText;
        //pbxBackground->Canvas->Brush->Style = bsClear;
        //pbxBackground->Canvas->Font->Size = Width/10;
        //pbxBackground->Canvas->Font->Name = "Century Gothic";
        //pbxBackground->Canvas->Font->Style = TFontStyles() >> fsItalic >> fsBold;

        // Dimensions
        Margin = pbxBackground->Canvas->TextWidth("X");

        // Set transparence
        mat::SetFormTransparence (this, 120);

        //Create an elliptical region
        //mat::SetFormRoundRectRegion(this,4*Margin);
}
//---------------------------------------------------------------------------
//__fastcall TfrmAbout::~TfrmAbout()
//{ // Destructor
//}

//-------------------------------- Events -----------------------------------

//---------------------------------------------------------------------------
void __fastcall TfrmOutputText::pbxBackgroundPaint(TObject *Sender)
{ // PaintBox drawing

        // Colors

// . . . Borders
        /* Ellipse background
        pbxBackground->Canvas->Brush->Style = bsSolid;
        pbxBackground->Canvas->Brush->Color = clBorder;
        pbxBackground->Canvas->Pen->Style = psClear;
        pbxBackground->Canvas->FillRect(pbxBackground->BoundsRect);
        pbxBackground->Canvas->Brush->Color = clBackground;
        pbxBackground->Canvas->Ellipse(Margin,Margin,pbxBackground->Width-Margin,pbxBackground->Height-Margin);
        */

// . . . Render text
        y = Margin;
        TSize TextSize = pbxBackground->Canvas->TextExtent(Text);

        Width = 2*Margin + TextSize.cx;
        Height = 2*Margin + TextSize.cy;

        Put( Text );
}

//---------------------------------------------------------------------------

void __fastcall TfrmOutputText::pbxBackgroundClick(TObject *Sender)
{
        Close();
}



//------------------------------ Utilities ----------------------------------

//---------------------------------------------------------------------------
inline void __fastcall TfrmOutputText::Put (String s)
{ // Put a string

        pbxBackground->Canvas->TextOut( Margin, y, s );
        y += pbxBackground->Canvas->TextHeight(s) + Margin;
}


//============================== END OF FILE ================================

