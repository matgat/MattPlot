//---------------------------------------------------------------------------
#include <vcl.h>
#include "unt_MatCommon.h"
#pragma hdrstop
#include "unt_TimeCounter.h"
#include "unt_ThisExeFile.h"
#include "unt_MatLogo.h"
#include "unt_About.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"


//====================== Useful file scope declarations =====================

    cls_MatLogo logo;
    const int TfrmAbout::AlphaMax = 200; // [0-255]

//============================= Public methods ==============================

//---------------------------------------------------------------------------
__fastcall TfrmAbout::TfrmAbout(TControl* Owner, const class cls_ThisExeFile* exe)
    : TForm(Owner), ExeInfo(exe)
{ // Constructor

// . . . Defaults
    DoubleBuffered = true;
    // Dimensions
    Width = 0.65 * Owner->Width;
    Height = 0.85 * Width;
    Margin = Width/20;
    Abort = false;
    PaintTime = 50;
    // Other
    tmrInvalidate->Interval = 100;
    tmrInvalidate->Enabled = true;

    // Elliptical region and alpha channel
    Transparency = AlphaMax;
    mat::SetFormTransparence (this, Transparency);
    mat::SetFormEllipticRegion(this);
}
//---------------------------------------------------------------------------
//__fastcall TfrmAbout::~TfrmAbout()
//{ // Destructor
//}

//---------------------------------------------------------------------------
void __fastcall TfrmAbout::FadeIn(const int FadeTime)
{ // Fade in

    tmrInvalidate->Enabled = false;
    Transparency = 0;
    int FadeRate;
    Show();
    while ( Transparency < AlphaMax ) // Wait until completion
          {
           FadeRate = (double(AlphaMax) * PaintTime) / FadeTime;
           mat::SetFormTransparence (this, Transparency);
           Repaint();
           Transparency += FadeRate;
          }
    tmrInvalidate->Enabled = true;
}

//---------------------------------------------------------------------------
void __fastcall TfrmAbout::FadeOut(const int FadeTime)
{ // Fade out

    tmrInvalidate->Enabled = false;
    int FadeRate;
    while ( Transparency > 0 ) // Wait until completion
          {
           FadeRate = (double(AlphaMax) * PaintTime) / FadeTime;
           Transparency -= FadeRate;
           mat::SetFormTransparence (this, Transparency);
           Repaint();
          }
    Close();
}

//---------------------------------------------------------------------------
void __fastcall TfrmAbout::WaitClose()
{ // Fade out

    while (!Abort) Application->ProcessMessages(); // Wait until close
}


//-------------------------------- Events -----------------------------------

//---------------------------------------------------------------------------
void __fastcall TfrmAbout::tmrInvalidateTimer(TObject *Sender)
{ // Image refresh

    pbxBackground->Invalidate();
}

//---------------------------------------------------------------------------
void __fastcall TfrmAbout::pbxBackgroundPaint(TObject *Sender)
{ // PaintBox drawing

// . . . Measure
    static cls_TimeCounter TimeCounter;
    TimeCounter.StartTime();

// . . . Settings
    static const TColor clBackground = TColor (0xE7FCFF);
    static const TColor clText = TColor (0x562C00);
    static const TColor clBorder = TColor(0xE37C27);

// . . . Borders
    /*  Rectangular border
    pbxBackground->Canvas->Brush->Style = bsClear;
    //pbxBackground->Canvas->Pen->Mode = pmCopy;
    pbxBackground->Canvas->Pen->Style = psSolid;
    pbxBackground->Canvas->Pen->Width = Margin;
    pbxBackground->Canvas->Pen->Color = clBorder;
    //pbxBackground->Canvas->FrameRect(pbxBackground->BoundsRect);
    pbxBackground->Canvas->Rectangle(pbxBackground->BoundsRect);
    */
    // Ellipse background
    pbxBackground->Canvas->Brush->Style = bsSolid;
    pbxBackground->Canvas->Brush->Color = clBorder;
    pbxBackground->Canvas->Pen->Style = psClear;
    pbxBackground->Canvas->FillRect(pbxBackground->BoundsRect);
    pbxBackground->Canvas->Brush->Color = clBackground;
    pbxBackground->Canvas->Ellipse(Margin,Margin,pbxBackground->Width-Margin,pbxBackground->Height-Margin);

// . . . Logo
    logo.Draw(pbxBackground); // Measured task

// . . . Informations
    pbxBackground->Canvas->Font->Color = clText;
    pbxBackground->Canvas->Brush->Color = clBackground;
    pbxBackground->Canvas->Brush->Style = bsClear;
    y = pbxBackground->Height / 8;

    pbxBackground->Canvas->Font->Size = Width/6;
    pbxBackground->Canvas->Font->Name = "Impact";
    pbxBackground->Canvas->Font->Style = TFontStyles() >> fsItalic >> fsBold;
    Put( ExeInfo->Name, -pbxBackground->Canvas->TextHeight("X")/2 );

    pbxBackground->Canvas->Font->Size = pbxBackground->Width/20;
    pbxBackground->Canvas->Font->Name = "Franklin Gothic Medium";
    Put( ExeInfo->Version );

    pbxBackground->Canvas->Font->Size = pbxBackground->Width/40;
    pbxBackground->Canvas->Font->Name = "Arial";
    pbxBackground->Canvas->Font->Style = TFontStyles() >> fsItalic >> fsBold;
    #ifdef _DEBUG
      Put("Debug build!");
    #else
      Put("Pre-release build");
    #endif

    pbxBackground->Canvas->Font->Size = pbxBackground->Width/30;
    pbxBackground->Canvas->Font->Style = TFontStyles() << fsItalic >> fsBold;
    pbxBackground->Canvas->Font->Name = "Century Gothic";
    Put( ExeInfo->MetaData.FileDescription );

    pbxBackground->Canvas->Font->Size = pbxBackground->Width/30;
    pbxBackground->Canvas->Font->Name = "Arial";
    pbxBackground->Canvas->Font->Style = TFontStyles() >> fsItalic << fsBold;
    Put(ExeInfo->MetaData.Comments, pbxBackground->Canvas->TextHeight("X"));

    pbxBackground->Canvas->Font->Size = pbxBackground->Width/40;
    pbxBackground->Canvas->Font->Style = TFontStyles() >> fsItalic >> fsBold;
    Put( ExeInfo->AuthorMail, pbxBackground->Canvas->TextHeight("X") );

    pbxBackground->Canvas->Font->Size = pbxBackground->Width/55;
    pbxBackground->Canvas->Font->Name = "Times New Roman";
    pbxBackground->Canvas->Font->Style = TFontStyles() << fsItalic >> fsBold;
    Put( ExeInfo->MetaData.LegalCopyright );

// . . . Finally
    PaintTime = std::ceil(1000.0 * TimeCounter.GetTime());
}

//---------------------------------------------------------------------------
void __fastcall TfrmAbout::pbxBackgroundClick(TObject *Sender)
{
    FadeOut(200);
    Abort = true;
    //Close();
}



//------------------------------ Utilities ----------------------------------

//---------------------------------------------------------------------------
inline void __fastcall TfrmAbout::Put (const String& s, int vspace)
{ // Put a string

    x = (pbxBackground->Width - pbxBackground->Canvas->TextWidth(s)) / 2;
    pbxBackground->Canvas->TextOut( x, y, s );
    y += 1.3 * pbxBackground->Canvas->TextHeight(s) + vspace;
}


//============================== END OF FILE ================================
