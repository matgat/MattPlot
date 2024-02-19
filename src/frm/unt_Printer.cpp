//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "unt_Printer.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// TODO 5: Print dialog with margin and preview

//=============================== File scope ================================


/////////////////////////////////////////////////////////////////////////////
//================================= Dialogs =================================

//---------------------------------------------------------------------------
__fastcall TfrmChooseExtraPrnOps::TfrmChooseExtraPrnOps(TComponent* Owner, nms_MatPrint::cls_ExtraPrnOpts* po) : TForm(Owner)
{
    Opts = po;
}

//---------------------------------------------------------------------------
void __fastcall TfrmChooseExtraPrnOps::btnPriintScreenClick(TObject *Sender)
{
    Opts->Mode = nms_MatPrint::PR_SCREEN;
    Opts->NoBackground = chkNoBackground->Checked;
    ModalResult = mrOk;
}

//---------------------------------------------------------------------------
void __fastcall TfrmChooseExtraPrnOps::btnPrintPaperClick(TObject *Sender)
{
    Opts->Mode = nms_MatPrint::PR_PAPER;
    Opts->NoBackground = chkNoBackground->Checked;
    ModalResult = mrOk;
}

//---------------------------------------------------------------------------
void __fastcall TfrmChooseExtraPrnOps::WhenHelp (TMessage& Message)
{
    ModalResult = mrAbort;
}

//---------------------------------------------------------------------------
void __fastcall TfrmChooseExtraPrnOps::FormDblClick(TObject *Sender)
{
    ModalResult = mrRetry;
}

/*---------------------------------------------------------------------------
void __fastcall TfrmChoosePrint::WhenSysCommand (TMessage& Message)
{
    //DefWindowProc(handle, WM_SYSCOMMAND, SC_CONTEXTHELP, 0);
    //Message.Result=DLGC_WANTALLKEYS;}
    //ModalResult = nms_MatPrint::PR_HELP;
    //ShowMessage("sys command");
    //if ( Message.WParam == SC_CONTEXTHELP ) ShowMessage("whatis");
}*/

//---------------------------------------------------------------------------

//============================= Public methods ==============================



/////////////////////////////////////////////////////////////////////////////
//================================ Functions ================================

//---------------------------------------------------------------------------
TModalResult nms_MatPrint::ChooseExtraPrintOptions( class cls_ExtraPrnOpts* o)
{ // Show a dialog to choose print mode

    TForm* ModalChild = new TfrmChooseExtraPrnOps(Application->MainForm,o);
    TModalResult r = ModalChild->ShowModal();
    delete ModalChild;
    ModalChild = 0;
    return r;
}

//---------------------------------------------------------------------------
void nms_MatPrint::StretchDrawDIB( Graphics::TCanvas *DestCanvas, const TRect& DestRect,
                                   Graphics::TBitmap* SourceBitmap )
{ // Stretch a bitmap to fit in a destination rect in a canvas

// . . . Local variables
    int OriginalWidth = SourceBitmap->Width; // Save the original width of the bitmap
    HDC DC;
    bool IsPaletteDevice;
    bool IsDestPaletteDevice;
    int BitmapInfoSize;
    tagBITMAPINFO *lpBitmapInfo;
    HBITMAP hBm;
    HPALETTE hPal;
    HPALETTE OldPal;
    unsigned char *Bits;
    int NumPalEntries;
    Windows::TPaletteEntry *lPPalEntriesArray;
    HDC FCompHDC = GetDC(0); // use screen's DC
    
    // Are we a palette device?
    IsPaletteDevice = (::GetDeviceCaps(FCompHDC, RASTERCAPS) &  RC_PALETTE) == RC_PALETTE;
    
    // Give back the screen dc
    ::ReleaseDC(0, FCompHDC);
    
    //Allocate the BitmapInfo structure
    if (IsPaletteDevice == true) BitmapInfoSize = sizeof(TBitmapInfo) + (sizeof(RGBQUAD) * 255);
    else BitmapInfoSize = sizeof(TBitmapInfo);
    
    lpBitmapInfo  = (tagBITMAPINFO*) new char[BitmapInfoSize];

    // Zero out the BitmapInfo structure
    ::ZeroMemory (lpBitmapInfo, BitmapInfoSize);
    
    // Fill in the BitmapInfo structure
    lpBitmapInfo->bmiHeader.biSize = sizeof(TBitmapInfoHeader);
    lpBitmapInfo->bmiHeader.biWidth = OriginalWidth;
    lpBitmapInfo->bmiHeader.biHeight = SourceBitmap->Height;
    lpBitmapInfo->bmiHeader.biPlanes = 1;
    
    if ( IsPaletteDevice ) lpBitmapInfo->bmiHeader.biBitCount = 8;
    else lpBitmapInfo->bmiHeader.biBitCount = 24;
    
    lpBitmapInfo->bmiHeader.biCompression = BI_RGB;
    lpBitmapInfo->bmiHeader.biSizeImage = ((lpBitmapInfo->bmiHeader.biWidth * static_cast<int>(lpBitmapInfo->bmiHeader.biBitCount)) / 8) * lpBitmapInfo->bmiHeader.biHeight;
    
    lpBitmapInfo->bmiHeader.biXPelsPerMeter = 0;
    lpBitmapInfo->bmiHeader.biYPelsPerMeter = 0;
    
    if ( IsPaletteDevice )
         {
          lpBitmapInfo->bmiHeader.biClrUsed = 256;
          lpBitmapInfo->bmiHeader.biClrImportant = 256;
         }
    else {
          lpBitmapInfo->bmiHeader.biClrUsed = 0;
          lpBitmapInfo->bmiHeader.biClrImportant = 0;
         }
    
    //Take ownership of the bitmap handle and palette
    hBm = SourceBitmap->ReleaseHandle();
    hPal = SourceBitmap->ReleasePalette();
    
    //use screen's DC
    FCompHDC = GetDC(0);

    if ( IsPaletteDevice )
       {
        //If we are using a palette, it must be
        //selected into the dc during the conversion
        OldPal = SelectPalette(FCompHDC, hPal, true);
    
        //Realize the palette
        RealizePalette(FCompHDC);
       }
    
    //Tell GetDiBits to fill in the rest of the bitmap info structure
    ::GetDIBits(SourceBitmap->Canvas->Handle, hBm,
                0, lpBitmapInfo->bmiHeader.biHeight,
                NULL, lpBitmapInfo, DIB_RGB_COLORS);
    
    //Allocate memory for the Bits
    Bits = new unsigned char[lpBitmapInfo->bmiHeader.biSizeImage];

    //Get the bits
    GetDIBits(SourceBitmap->Canvas->Handle, hBm,
              0, lpBitmapInfo->bmiHeader.biHeight,
              Bits, lpBitmapInfo, DIB_RGB_COLORS);
    
    if ( IsPaletteDevice )
      {
       //Lets fix up the color table for buggy video drivers
       lPPalEntriesArray = new TPaletteEntry[256];
       NumPalEntries = GetSystemPaletteEntries(FCompHDC, 0, 256, lPPalEntriesArray);
    
       for (int i=0; i < NumPalEntries; i++)
         {
          lpBitmapInfo->bmiColors[i].rgbRed = lPPalEntriesArray[i].peRed;
          lpBitmapInfo->bmiColors[i].rgbGreen = lPPalEntriesArray[i].peGreen;
          lpBitmapInfo->bmiColors[i].rgbBlue = lPPalEntriesArray[i].peBlue;
         }

       delete lPPalEntriesArray;
      }
    
    if ( IsPaletteDevice )
       {
        //Select the old palette back in
        SelectPalette(FCompHDC, OldPal, true);
    
        //Realize the old palette
        RealizePalette(FCompHDC);
       }
    
    // Give back the screen dc
    ::ReleaseDC(0, FCompHDC);
    
    // Is the Dest dc a palette device?
    IsDestPaletteDevice = (::GetDeviceCaps(DestCanvas->Handle, RASTERCAPS) &  RC_PALETTE) == RC_PALETTE;

    if ( IsDestPaletteDevice )
       {
        //if we are using a palette, it must be selected into the dc during the conversion
        OldPal = ::SelectPalette(DestCanvas->Handle, hPal, true);
    
        //Realize the palette
        ::RealizePalette(DestCanvas->Handle);
       }

    // Do the blt
    ::StretchDIBits (
                     DestCanvas->Handle,
                     DestRect.Left, DestRect.Top, DestRect.Width(), DestRect.Height(),
                     0, 0, OriginalWidth, lpBitmapInfo->bmiHeader.biHeight,
                     Bits, lpBitmapInfo, DIB_RGB_COLORS, SRCCOPY
                    );

    if ( IsDestPaletteDevice )
       {
        //Select the old palette back in}
        SelectPalette(DestCanvas->Handle, OldPal, true);
    
        //Realize the old palette
        RealizePalette(DestCanvas->Handle);
       }

    // De-Allocate the Dib Bits
    delete [] Bits;
    
    // De-Allocate the BitmapInfo
    delete [] lpBitmapInfo;

    // Set the ownership of the bimap handles back to the bitmap
    SourceBitmap->Handle = hBm;
    SourceBitmap->Palette = hPal;
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

