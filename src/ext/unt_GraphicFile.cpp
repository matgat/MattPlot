//---------------------------------------------------------------------------
#include <SysUtils.hpp>  // Exceptions
//#include "unt_MatCommon.h"
#pragma hdrstop
//---------------------------------------------------------------------------
#include "unt_GraphicFile.h"
#include "extdlgs.hpp"  // for open/save picture dialog

// . . . Image classes

#ifdef ENVISION_LIB
    #include "EnBmpGr.hpp";
    #include "EnWmfGr.hpp";
    #include "EnPngGr.hpp";
    #include "EnEpsGr.hpp"; // Open only
    #include "EnPdfGr.hpp";
    #include "EnJpgGr.hpp";
    #pragma link "CollectObj_c5.lib"
#else
	//#include <jpeg.hpp> // TJPEGImage
	#include "GIFImage.hpp" // Anders Melander's TGifImage class
    //USEUNIT("..\..\ext\GifImage\GIFImage.pas");
#endif

    // TODO 5: use TPicture and RegisterFileFormat
    //Image1.Picture.RegisterFileFormat('img','Bitmap file',TBitmap);
    //TPicture::RegisterFileFormat(__classid(TJpeg), "JPG", "JPEG Image files", __classid(TJpeg));

#pragma package(smart_init)
//---------------------------------------------------------------------------


//============================== File scope =================================
        // Beware: these are visible globally in this file!

enum en_GrExtensions {EX_BMP=1, EX_GIF, EX_PNG, EX_JPG, EX_EMF, EX_EPS, EX_PDF};

#ifdef ENVISION_LIB
    const struct {int Index; char Extension[6]; char Description[256];}
    GrFilters[] = {
                  {EX_BMP,".bmp","Bitmap (*.bmp)"},
                  //{EX_GIF,".gif","Graphics Interchange Format (*.gif)"},
                  {EX_PNG,".png","Portable Network Graphics (*.png)"},
                  {EX_JPG,".jpg;.jpeg","Joint Photographic Experts Group (*.jpg;*.jpeg)"}
                  //{EX_EMF,".emf","Enhanced Meta File (*.emf)"},
                  //{EX_EPS,".eps","Encapsulated PostScript (*.eps)"},
                  {EX_PDF,".pdf","Portable Document Format (*.pdf)"},
                 };
#else
    const struct {int Index; char Extension[6]; char Description[256];}
    GrFilters[] = {
                   {EX_BMP,".bmp","Bitmap (*.bmp)"},
                   {EX_GIF,".gif","Graphics Interchange Format (*.gif)"}
                  };
#endif

const int GrFiltersSize = sizeof(GrFilters)/sizeof(GrFilters[0]);

/*
//enum TProgressStage {psStarting, psRunning, psEnding};
//typedef void __fastcall (__closure *TProgressEvent)(System::TObject* Sender, TProgressStage Stage, Byte PercentDone, bool RedrawNow, const TRect &R, const AnsiString Msg);
void __fastcall TFormMain::ImageProgress(System::TObject* Sender,
                                           TProgressStage Stage,
                                           Byte PercentDone,
                                           bool RedrawNow,
                                           const TRect &R,
                                           const AnsiString Msg)
{ // Image processing progress
        if (Stage == psEnding) ProgressBar->Position = 0;
        else ProgressBar->Position = PercentDone;
}
*/

//============================= Static Members ==============================
String cls_GraphicFile::Filters = "";


//---------------------------------------------------------------------------
void cls_GraphicFile::BuildFilters()
{ // Build filters facility
    if ( Filters.IsEmpty() )
       {
        for (int i=0; i<GrFiltersSize; i++)
            AddFilter( String(GrFilters[i].Description), String("*")+String(GrFilters[i].Extension) );
       }
}


//============================= Public methods ==============================

//---------------------------------------------------------------------------
cls_GraphicFile::cls_GraphicFile (TImage* img) : Image(img), Owner(img->Owner)
{ // Constructor

// . . . Default values
    i_Result = "";
    //InitialDir = ExtractFileDir(Application->ExeName); // Starting in exe directory
    OnProgress = 0;

// . . . Recreate Filter String
    //Filters = GraphicFilter(__classid(Graphics::TGraphic));
    BuildFilters();
}

//---------------------------------------------------------------------------
bool cls_GraphicFile::Open (String FilePath)
{ // Open from file; returns true if ok

// . . . Prepare things
    bool ok = false;

    try {
         Image->Picture->LoadFromFile(FilePath);
         Image->Width  = Image->Picture->Graphic->Width;
         Image->Height = Image->Picture->Graphic->Height;
         i_Result = "Image opened from " + FilePath;
         ok = true;
        }
    catch(EInvalidGraphic& e){i_Result = "Invalid file format:\n" + e.Message;}
    catch(Exception& e){i_Result = e.Message;}

// . . . Finally
    //Caption = Image->Picture->Graphic->ClassName();
    return ok;
}

//---------------------------------------------------------------------------
bool cls_GraphicFile::Open ()
{ // Execute open file dialog; returns true if ok

// . . . Prepare dialog
    TOpenPictureDialog* FileDialog = new TOpenPictureDialog(Owner);
    FileDialog->Filter = "All files|*|" + Filters;
    FileDialog->Title = "Open Image";
    FileDialog->FilterIndex = 0;
    FileDialog->InitialDir = InitialDir;
    FileDialog->Options = FileDialog->Options >> ofAllowMultiSelect;

    bool ok = false;
    if ( FileDialog->Execute() ) ok = Open(FileDialog->FileName);

// . . . Finally
    delete FileDialog;
    return ok;
}

//---------------------------------------------------------------------------
bool cls_GraphicFile::Save (String FilePath)
{ // Save to file; returns true if ok

// . . . Prepare things
    bool ok = false;

    #ifdef ENVISION_LIB
        TDibGraphic* img;
    #else
        TGraphic* img;
    #endif

    String FileExt = ::ExtractFileExt(FilePath).UpperCase();
    try {
         try {
              if ( FileExt == ".BMP" )
                   {
                    #ifdef ENVISION_LIB
                        img = new TBitmapGraphic();
                    #else
                        img = new Graphics::TBitmap();
                    #endif
                   }
              else if ( FileExt == ".GIF" )
                   {
                    img = new TGIFImage();

                    // Adjust some bad defaults
                    TGIFImage* gif = dynamic_cast<TGIFImage*>(img);
                    gif->ColorReduction = rmQuantizeWindows;
                    gif->ReductionBits = 8;
                    gif->DitherMode = dmFloydSteinberg;
                    //gif->Compression = gcLZW;
                   }
              //else if ( FileExt == ".PNG" ) img = new TPngGraphic(); break;
              //else if ( FileExt == ".JPG" ) img = new TJpegGraphic(); break; // TJPEGImage *jp = new TJPEGImage();
              //else if ( FileExt == ".WMF" ) img = new TMetaFileGraphic(); break;
              //else if ( FileExt == ".PDF" ) img = new TPdfGraphic(); break;
              else {
                    throw Exception("Save graphic: format not supported, select another filter");
                   }
              if (OnProgress) img->OnProgress = OnProgress;
              img->Assign(Image->Picture->Bitmap);
              img->SaveToFile(FilePath);
              i_Result = String(Image->Picture->Graphic->ClassName()) + " saved in " + FilePath;
              ok = true;
             }
         catch ( Exception& e )
            {
             i_Result = String(e.ClassName()) + " " + e.Message;
            }
        }
    __finally {
               delete img;
              }

// . . . Finally
    return ok;
}

//---------------------------------------------------------------------------
bool cls_GraphicFile::Save ()
{ // Execute save file dialog; returns true if ok

// . . . Prepare dialog
    TSavePictureDialog* FileDialog = new TSavePictureDialog(Owner);
    FileDialog->Filter = Filters; // GraphicFilter(__classid(Graphics::TGraphic));
    FileDialog->Title = "Save Image As";
    FileDialog->FilterIndex = EX_GIF; // EX_PNG
    //FileDialog->DefaultExt = "bmp";
    FileDialog->InitialDir = InitialDir;
    FileDialog->Options = FileDialog->Options >> ofAllowMultiSelect << ofOverwritePrompt >> ofCreatePrompt;

    bool ok;

    if ( FileDialog->Execute() )
       {
        // Force extension of chosen filter
        String FilePath = ::ChangeFileExt(FileDialog->FileName, GrFilters[FileDialog->FilterIndex-1].Extension);
        ok = Save(FilePath);
       }
    else i_Result = "Save picture aborted by user";

// . . . Finally
    delete FileDialog;
    return ok;
}


//---------------------------------------------------------------------------
void cls_GraphicFile::AddFilter (const String& descr, const String& filt)
{ // Add filter facility
    if ( !Filters.IsEmpty() ) Filters += "|";
    Filters += descr + " ("+filt+")" + "|" + filt;
}



//============================= Private Methods =============================




//------------------------------- Properties --------------------------------



//=============================== END OF FILE ===============================
