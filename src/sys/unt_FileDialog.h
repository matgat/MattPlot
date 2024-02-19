//---------------------------------------------------------------------------
#ifndef unt_FileDialogH
#define unt_FileDialogH

        /* * * * * * * * * * * * * * * * * * * * * * * * * * * *
        *
        *    unt_FileDialog
        *
        *    cls_FileDialog
        *    2008 - Matteo Gattanini
        *
        *    Derived from code by: * Cotta
        *
        *    Overview
        *    --------------------------------------------
        *    Wraps up the API code to open/save files
        *
        *    Licenses
        *    --------------------------------------------
        *    Use and modify freely
        *
        *    Release history
        *    --------------------------------------------
        *    ver 0.3 (dec.2008)
        *        .Using exceptions
        *    ver 0.2a    (11.nov.2006)
        *        .First release
        *
        *    Example of usage:
        *    --------------------------------------------
        *    #include "unt_FileDialog.h"
        *    // ...
        *    cls_FileDialog FileDialog(this);
        *    FileDialog.Filter = "backup files|*.bak;*.sav|all files|*");
        *    FileDialog.AddFilter("documents","*.txt;*.doc;*.rtf");
        *    FileDialog.AddFilter("all files","*");
        *    FileDialog.FilterIndex = 2;
        *    FileDialog.DefaultExt = "txt"; // appended extension when missing
        *    FileDialog.InitialDir = "c:\\data"; // default is the exe dir
        *    // Flags
        *    FileDialog.Options >> cls_FileDialog::ofCreatePrompt
        *                       << cls_FileDialog::ofOverwritePrompt;
        *
        *     if ( FileDialog.Open() )
        *          {
        *           Memo1->Lines->LoadFromFile(FileDialog.FileName);
        *          }
        *     else { // an error occurred
        *           ShowMessage(FileDialog.DialogErrorString());
        *          }
        *     if ( FileDialog.Save() )
        *          {
        *           Memo1->Lines->SaveToFile(FileDialog.FileName);
        *          }
        *     else { // an error occurred
        *           ShowMessage(FileDialog.DialogErrorString());
        *          }
        *
        *     // Or use the free store:
        *    cls_FileDialog* FileDialog = new cls_FileDialog(this);
        *    // ...
        *    delete FileDialog;
        *
        *    Remarks
        *    --------------------------------------------
        *    *) TODO: checkboxes
        *
        *
        * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <windows.h>
#include <Classes.hpp>
//---------------------------------------------------------------------------
class cls_FileDialog {

public://                                                       ~~~~~~~~~~~~~
// . . .  Types definitions
        enum en_Actions { en_Open, en_Save };
        enum en_Styles { en_Win95, en_Win98, en_WinXP };

        // Option flags
        enum en_FileDialogFlag { ofReadOnly,
                                 ofOverwritePrompt, // confirm file overwriting
                                 ofHideReadOnly,
                                 ofNoChangeDir,   // prevent changing directory
                                 ofShowHelp,
                                 ofNoValidate,
                                 ofAllowMultiSelect,   // allow multiple files
                                 ofExtensionDifferent,
                                 ofPathMustExist, // deny non-existing paths
                                 ofFileMustExist, // deny non-existing files
                                 ofCreatePrompt, // confirm when getting a non-existing path
                                 ofShareAware,
                                 ofNoReadOnlyReturn,
                                 ofNoTestFileCreate,
                                 ofNoNetworkButton,
                                 ofNoLongNames,
                                 ofOldStyleDialog,
                                 ofNoDereferenceLinks,
                                 ofEnableIncludeNotify,
                                 ofEnableSizing
                                };

        typedef Set<en_FileDialogFlag, ofReadOnly, ofEnableSizing> typ_Options;

// . . . Public methods
        // Costructors
        cls_FileDialog () { Parent=0; Init(); }
        cls_FileDialog (HWND H) { Parent=H; Init(); }
        cls_FileDialog (TWinControl* W)  { Parent=W->Handle; Init(); }
        cls_FileDialog::~cls_FileDialog () {delete Files;}

        bool __fastcall Open(); // Execute open file dialog
        bool __fastcall Save(); // Execute save file dialog
        static String DialogErrorString(); // Get dialog error string
        void AddFilter (const String& descr, const String& filt); // Add filter facility

// . . . Properties
        __property String FileName = { read=i_FileName, write=i_FileName }; // The retrieved file path

        __property String InitialDir = { read=i_InitialDir, write=i_InitialDir }; // The initial directory
        __property String DefaultExt = { read=i_DefaultExt, write=i_DefaultExt }; // Default extension appended to filename (without period)
        __property String DialogTitle = { read=i_DialogTitle, write=i_DialogTitle }; // The dialog title
        __property String Filter = { read=i_Filter, write=i_Filter }; // File filter: "documents|*.txt;*.doc;*.rtf|backup files|*.bak"
        __property int FilterIndex = { read=get_FilterIndex, write=i_FilterIndex }; // Default file filter index
        __property en_Styles Style = { read=i_Style, write=i_Style }; // Dialog style
        __property HWND Parent = { read=get_Parent, write=i_Parent }; // Parent's handle
        // Option flags
        __property typ_Options Options = {read=i_Options, write=i_Options};
        __property unsigned int Flags = { read=get_Flags };

// . . . Public attributes (!)
        class TStrings* Files;

private://                                                      ~~~~~~~~~~~~~

// . . . Private attributes

// . . . Private methods

        // Functions that deal directly with API stuff (translate class members to Windows)
        static bool Execute ( HWND parent, // parent window's handle
                              const String& filters, // filters string
                              int& filter_index, // selected filter
                              String& file_name, // where store selected file / file editbox initialization
                              class TStrings* files, // where store selected files
                              const char* initial_dir, // initial directory
                              const char* dialog_title, // dialog titlebar caption
                              unsigned int flags, // dialog options
                              const char* default_ext, // default appended extension
                              en_Styles style, // dialog style (os)
                              en_Actions action // open or save
                             );

        void Init (); // Shared construction code
        static unsigned int GetFlagMask (en_FileDialogFlag); // Translate class enum to windows flags

// . . . Properties
        String i_InitialDir,i_DefaultExt,i_DialogTitle,i_Filter,i_FileName;

        en_Styles i_Style;

        HWND i_Parent;
        HWND get_Parent () const;

        int i_FilterIndex;
        int get_FilterIndex() const { if ( i_FilterIndex<0 && !Filter.IsEmpty() ) return 0; else return i_FilterIndex;}

        typ_Options i_Options;
        unsigned int get_Flags() const;
};//-------------------------------------------------------------------------
#endif
