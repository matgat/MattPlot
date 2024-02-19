//---------------------------------------------------------------------------
#ifndef unt_GraphicFileH
#define unt_GraphicFileH

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    *
    *        unt_GraphicFile
    *
    *        cls_GraphicFile 10.2006 Matteo Gattanini
    *        ver: 0.3    (02.dec.2007)
    *
    *    Manage graphic files using external libraries
    *
    *    Example of usage:
    *    #include "unt_GraphicFile.h"
    *    // ...
    *       ProgressBar->Visible = true;
    *       cls_GraphicFile* gr = new cls_GraphicFile(Image);
    *       gr->InitialDir = ExtractFileDir(Application->ExeName);
    *       gr->OnProgress = ImageProgress;
    *       if ( gr->SaveAs() ) StatusBar1->SimpleText = gr->Result;
    *       else ShowMessage("Error: " + gr->Result);
    *       delete gr;
    *       ProgressBar->Visible = false;
    *       // Where
    *       //----------------------------------------
    *       void __fastcall TForm1::ImageProgress(System::TObject* Sender,
    *                                             TProgressStage Stage,
    *                                             Byte PercentDone,
    *                                             bool RedrawNow,
    *                                             const Windows::TRect &R,
    *                                             const AnsiString Msg)
    *       { // Image processing progress
    *               if (Stage == psEnding) ProgressBar->Position = 0;
    *               else ProgressBar->Position = PercentDone;
    *       }
    *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <Classes.hpp>
#include <ExtCtrls.hpp> // TImage
//---------------------------------------------------------------------------
class cls_GraphicFile {

public://                                                       ~~~~~~~~~~~~~
// . . .  Types definitions
    //enum en_Actions { en_Open, en_Save };

// . . . Public methods
    // Costructors
    cls_GraphicFile (TImage*);
    // Destructor
    cls_GraphicFile::~cls_GraphicFile ()  {;}

    bool Open (String FilePath); // Open from file; returns true if ok
    bool Open (); // Execute open file dialog; returns true if ok
    bool Save (String FilePath); // Save to file; returns true if ok
    bool Save (); // Execute save file dialog; returns true if ok
    static void AddFilter (const String& descr, const String& filt); // Add filter facility
    static void BuildFilters(); // Build filters facility

// . . . Properties
    __property String InitialDir = { read=i_InitialDir, write=i_InitialDir }; // The initial directory

    // Diagnostic
    __property String Result = { read=i_Result}; // A simple diagnostic string

    // Events
    __property TProgressEvent OnProgress = { read=i_OnProgress, write=set_OnProgress };

// . . . Public attributes (!)
    static String Filters; // File filter: "documents|*.txt;*.doc;*.rtf|backup files|*.bak"

private://                                                      ~~~~~~~~~~~~~

// . . . Private attributes
    TComponent* Owner;
    TImage* Image;

// . . . Private methods

// . . . Properties
    String i_InitialDir,i_Result;

    TProgressEvent i_OnProgress;
    void set_OnProgress (TProgressEvent f) {Image->OnProgress = i_OnProgress = f;}
        
};//-------------------------------------------------------------------------
#endif
