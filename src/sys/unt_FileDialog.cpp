//---------------------------------------------------------------------------
#include <Forms.hpp>  // Application, Screen
//#include <Sysutils.hpp>
//#include "unt_MatCommon.h"
#pragma hdrstop
#include "unt_FileDialog.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------


//============================== File scope =================================
    // Beware: these are visible globally in this file!

//-------------------------------- Settings ---------------------------------
    const int MAX_SELECTED_FILES = 32; // max multiple selected files


//---------------------------------------------------------------------------
UINT APIENTRY OFNHookProc
        (
         HWND hdlg,     // handle to the dialog box window
         UINT uiMsg,    // message identifier
         WPARAM wParam, // message parameter
         LPARAM lParam  // message parameter
        )
{ // A dummy function
        return 0;
}

//============================= Static Members ==============================


//---------------------------------------------------------------------------
// Build win dialog structure and call the API; return true if file selected and pressed OK
bool cls_FileDialog::Execute ( HWND parent, // parent window's handle
                               const String& filters, // filters string
                               int& filter_index, // selected filter
                               String& file_name, // where store selected file / file editbox initialization
                               TStrings* files, // where store selected files
                               const char* initial_dir, // initial directory
                               const char* dialog_title, // dialog titlebar caption
                               unsigned int flags, // dialog options
                               const char* default_ext, // default appended extension
                               en_Styles style, // dialog style (os)
                               en_Actions action // open or save
                              )
{

// . . . Specific API arguments are intantiated and filled locally
        OPENFILENAME OFN;
        ::ZeroMemory(&OFN, sizeof(OFN)); // Avoid not initialized fields problem

        const int BufSize = MAX_SELECTED_FILES * ( MAX_PATH + 1 );

        // . . . Handles
        OFN.hwndOwner = parent; // Parent's handle
        OFN.hInstance = 0; // Is used for dialog templates
        
        // . . . File filters
        char bufFilter[BufSize];
        ::StrLCopy(bufFilter, filters.c_str(), BufSize-2);
        // translate filters string: '|'->'\0' and finally double '\0'
        int lastchar = BufSize-2;
        if ( lastchar > filters.Length() ) lastchar = filters.Length();

        for (int i=0; i<lastchar; ++i) if ( bufFilter[i]=='|' ) bufFilter[i]='\0';
        // must terminate with two null characters
        bufFilter[lastchar] = '\0'; // This should be yet to 0
        bufFilter[lastchar+1] = '\0';
        OFN.lpstrFilter = bufFilter;
        char custom_filter[BufSize] = "";
        OFN.lpstrCustomFilter = custom_filter; // user-defined filter patterns
        OFN.nMaxCustFilter = BufSize;
        OFN.nFilterIndex = filter_index; // currently selected filter

        // . . . File name
        char bufFileName[BufSize];
        ::StrLCopy(bufFileName, file_name.c_str(), BufSize-1);
        OFN.lpstrFile = bufFileName; // buffer that containing the filename (initializes also the edit control)
        OFN.nMaxFile = BufSize; // size of buffer
        OFN.lpstrFileTitle = 0; // name and extension of the selected file
        OFN.nMaxFileTitle = 0;

        // . . . Directory and title
        OFN.lpstrInitialDir = initial_dir;
        OFN.lpstrTitle = dialog_title; // title bar string of the dialog box

        // . . . Flags
        OFN.Flags = flags;

        // . . . Utilities
        OFN.nFileOffset = 0; // gives the file name position in the path string
        OFN.nFileExtension = 0; // gives the file extension position in the path string
        OFN.lpstrDefExt = default_ext; // default appended extension

        // . . . Hook
        OFN.lCustData = 0;
        //OFN.lpfnHook = 0;
        OFN.lpTemplateName = 0;

        // . . . OS dependent settings
        switch (style)
           {
            case en_Win95 : // Windows 95
                     OFN.Flags = OFN.Flags | OFN_ENABLEHOOK & ~OFN_EXPLORER;
                     OFN.lpfnHook = OFNHookProc;
                     break;
            case en_Win98 : // Windows 98
                     OFN.Flags = OFN.Flags | OFN_ENABLEHOOK | OFN_EXPLORER;
                     OFN.lpfnHook = OFNHookProc;
                     break;
            case en_WinXP : // Windows XP
                     OFN.Flags = OFN.Flags & ~OFN_ENABLEHOOK | OFN_EXPLORER;
                     OFN.lpfnHook = 0;
                     break;
           }
        
        // . . . Size of the structure
        //if ( _WIN32_WINNT >= 0x0500 ) OFN.lStructSize = sizeof(OFN);
        //else OFN.lStructSize = OPENFILENAME_SIZE_VERSION_400;
        OFN.lStructSize = sizeof(OFN);

// . . . Create dialog
        bool result = false;
        switch ( action )
           {
            case en_Open : result = ::GetOpenFileName(&OFN); break;
            case en_Save : result = ::GetSaveFileName(&OFN); break;
           }

// . . . Take selected file and return operation result
        files->Clear();
        file_name = "";
        if ( result )
           {
            // Selected filter
            filter_index = OFN.nFilterIndex; // currently selected filter
            
            // . . . Get file names
            // If the OFN_ALLOWMULTISELECT flag is set and the user selects multiple files,
            // the buffer contains the current directory followed by the filenames of the
            // selected files. For Explorer-style dialog boxes, the directory and filename
            // strings are NULL separated, with an extra NULL character after the last filename

            if ( flags & OFN_ALLOWMULTISELECT )
                 {
                  char endchar = '\0';
                  //if ( !(flags & OFN_EXPLORER) ) endchar = ' '; // old style interface
                  // Fill common directory
                  String Dir = "";
                  unsigned int i=0; // init char counter
                  while ( OFN.lpstrFile[i] != endchar && i<OFN.nMaxFile ) Dir += OFN.lpstrFile[i++];
                  Dir = ::IncludeTrailingBackslash(Dir); // Ensure path delimiter
                  // A check
                  //if ( i != OFN.nFileOffset ) does not match
                  // Fill files
                  String CurrentFile = "";
                  while ( i < (OFN.nMaxFile-1) )
                        {
                         if ( OFN.lpstrFile[i] == endchar )
                              { // File name complete
                               files->Add( ::ExcludeTrailingBackslash(Dir+CurrentFile) );
                               CurrentFile = "";

                               // Check max allowed selected files
                               if ( files->Count >= MAX_SELECTED_FILES )
                                  {
                                   // Allowed max selected files reached!
                                   break;
                                  }

                               // Check if finished
                               if ( OFN.lpstrFile[i+1] == endchar ) break; // end
                              }
                         else {
                               CurrentFile += OFN.lpstrFile[i];
                              }
                         ++i; // next char
                        }

                  // Set also 'file_name' to the first selection
                  if ( files->Count > 0 ) file_name = files->Strings[0];
                 }
            else {
                  file_name = String(OFN.lpstrFile);
                  files->Add(file_name);
                 }
           }

// . . . Finally
        return result;
}

//---------------------------------------------------------------------------
unsigned int cls_FileDialog::GetFlagMask (en_FileDialogFlag flag)
{ // Translate my enum to windows flags defined in 'commdlg.h'

    switch ( flag )
        {
         case ofReadOnly             : return OFN_READONLY;
         case ofOverwritePrompt      : return OFN_OVERWRITEPROMPT;
         case ofHideReadOnly         : return OFN_HIDEREADONLY;
         case ofNoChangeDir          : return OFN_NOCHANGEDIR;
         case ofShowHelp             : return OFN_SHOWHELP;
         case ofNoValidate           : return OFN_NOVALIDATE;
         case ofAllowMultiSelect     : return OFN_ALLOWMULTISELECT;
         case ofExtensionDifferent   : return OFN_EXTENSIONDIFFERENT;
         case ofPathMustExist        : return OFN_PATHMUSTEXIST;
         case ofFileMustExist        : return OFN_FILEMUSTEXIST;
         case ofCreatePrompt         : return OFN_CREATEPROMPT;
         case ofShareAware           : return OFN_SHAREAWARE;
         case ofNoReadOnlyReturn     : return OFN_NOREADONLYRETURN;
         case ofNoTestFileCreate     : return OFN_NOTESTFILECREATE;
         case ofNoNetworkButton      : return OFN_NONETWORKBUTTON;
         case ofNoLongNames          : return OFN_NOLONGNAMES;
         case ofOldStyleDialog       : return 0;
         case ofNoDereferenceLinks   : return OFN_NODEREFERENCELINKS;
         case ofEnableIncludeNotify  : return 0;
         case ofEnableSizing         : return 0;
                                    /*
                                     : return OFN_ENABLEHOOK
                                     : return OFN_ENABLETEMPLATE
                                     : return OFN_ENABLETEMPLATEHANDLE
                                     : return OFN_EXPLORER
                                     : return OFN_LONGNAMES
                                    */
         default                     : return 0;
        }
}


//---------------------------------------------------------------------------
String cls_FileDialog::DialogErrorString()
{ // Get dialog error string
    String Msg;
    switch ( ::CommDlgExtendedError() )
       {
        // Common
        case CDERR_DIALOGFAILURE   : Msg = "The dialog box could not be created (invalid parent\'s handle?)"; break;
        case CDERR_FINDRESFAILURE  : Msg = "The common dialog box function failed to find a specified resource"; break;
        case CDERR_INITIALIZATION  : Msg = "The common dialog box function failed during initialization (not enough memory?)"; break;
        case CDERR_LOCKRESFAILURE  : Msg = "The common dialog box function failed to lock a specified resource"; break;
        case CDERR_LOADRESFAILURE  : Msg = "The common dialog box function failed to load a specified resource"; break;
        case CDERR_LOADSTRFAILURE  : Msg = "The common dialog box function failed to load a specified string"; break;
        case CDERR_MEMALLOCFAILURE : Msg = "The common dialog box function was unable to allocate memory for internal structures"; break;
        case CDERR_MEMLOCKFAILURE  : Msg = "The common dialog box function was unable to lock the memory associated with a handle"; break;
        case CDERR_NOHINSTANCE     : Msg = "The ENABLETEMPLATE flag was set, but you failed to provide a corresponding instance handle"; break;
        case CDERR_NOHOOK          : Msg = "The ENABLEHOOK flag was set, but you failed to provide a pointer to a corresponding hook procedure"; break;
        case CDERR_NOTEMPLATE      : Msg = "The ENABLETEMPLATE flag was set, but you failed to provide a corresponding template"; break;
        case CDERR_STRUCTSIZE      : Msg = "The lStructSize member of the initialization structure for the corresponding common dialog box is invalid"; break;
        case CDERR_REGISTERMSGFAIL : Msg = "The RegisterWindowMessage function returned an error code when it was called by the common dialog box function"; break;
        //  GetOpenFileName and GetSaveFileName functions
        case FNERR_BUFFERTOOSMALL  : Msg = "The buffer pointed to by the lpstrFile member of the OPENFILENAME structure is too small. The first two bytes of the lpstrFile buffer contain an integer value specifying the size, in bytes (ANSI version) or characters (Unicode version), required to receive the full name"; break;
        case FNERR_INVALIDFILENAME : Msg = "A filename is invalid"; break;
        case FNERR_SUBCLASSFAILURE : Msg = "An attempt to subclass a list box failed because sufficient memory was not available"; break;
        //
        default                    : Msg = "User abort or unknown failure";
       }
       return Msg;
}



//============================= Public methods ==============================


//---------------------------------------------------------------------------
bool __fastcall cls_FileDialog::Open()
{ // Execute open file dialog: returns true if ok

// . . . Prepare specific action datas
    String Title = DialogTitle.Trim();
    if ( Title.IsEmpty() ) Title = "Open file";

    // Flags
    // Options >> ofOverwritePrompt << ofFileMustExist;

// . . . Call API function wrapper
    return Execute(Parent,Filter,i_FilterIndex,i_FileName,Files,InitialDir.c_str(),Title.c_str(),Flags,DefaultExt.c_str(),Style,en_Open);
}

//---------------------------------------------------------------------------
bool __fastcall cls_FileDialog::Save()
{ // Execute save file dialog: returns true if ok

// . . . Prepare specific action datas
    String Title = DialogTitle.Trim();
    if ( Title.IsEmpty() ) Title = "Save file";

    // Flags
    // Options >> ofCreatePrompt << ofOverwritePrompt;

// . . . Call API function wrapper
    return Execute(Parent,Filter,i_FilterIndex,i_FileName,Files,InitialDir.c_str(),Title.c_str(),Flags,DefaultExt.c_str(),Style,en_Save);
}

//---------------------------------------------------------------------------
void cls_FileDialog::AddFilter (const String& descr, const String& filt)
{ // Add filter facility
    if ( !Filter.IsEmpty() ) Filter += "|";
    Filter += descr + " ("+filt+")" + "|" + filt;
}










//============================== Private Methods ============================


//---------------------------------------------------------------------------
void cls_FileDialog::Init ()
{ // Shared contruction code

// . . . Dialog attributes
        DialogTitle = "";
        Filter = "";
        FilterIndex = -1;
        DefaultExt = "";
        InitialDir = ExtractFileDir(Application->ExeName); // Starting in exe directory

        // Flags
        Options >> ofNoChangeDir;

// . . . Check for windows version to set dialog style
        ::OSVERSIONINFO osvi;
        DWORD dwPlatformId;
        osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
        if ( ::GetVersionEx(&osvi) ) dwPlatformId = osvi.dwPlatformId;
        else dwPlatformId = (WM_USER+3426); // BADPLATFORM
        switch ( dwPlatformId )
               {
                case VER_PLATFORM_WIN32_NT      : Style = en_WinXP; break;
                case VER_PLATFORM_WIN32s        : Style = en_Win98; break;
                case VER_PLATFORM_WIN32_WINDOWS : Style = en_Win95; break;
                default                         : Style = en_Win98;
               }

// . . . Other initializations
        i_FileName = "";
        Files = new TStringList();
}




//------------------------------ Properties ---------------------------------


//---------------------------------------------------------------------------
HWND cls_FileDialog::get_Parent () const
{ // Get Parent's form handle
    if (i_Parent) return i_Parent;
    else if (Application->MainForm) return  Application->MainForm->Handle;
    else if (Screen->ActiveForm) return  Screen->ActiveForm->Handle;
    else return 0;
}

//---------------------------------------------------------------------------
unsigned int cls_FileDialog::get_Flags () const
{ // Build flags dword

        unsigned int flags = 0;
        /*
        while () flags |= GetFlagMask(  );
        */
        if ( Options.Contains(ofReadOnly) )            flags |= OFN_READONLY;
        if ( Options.Contains(ofOverwritePrompt) )     flags |= OFN_OVERWRITEPROMPT;
        if ( Options.Contains(ofHideReadOnly) )        flags |= OFN_HIDEREADONLY;
        if ( Options.Contains(ofNoChangeDir) )         flags |= OFN_NOCHANGEDIR;
        if ( Options.Contains(ofShowHelp) )            flags |= OFN_SHOWHELP;
        if ( Options.Contains(ofNoValidate) )          flags |= OFN_NOVALIDATE;
        if ( Options.Contains(ofAllowMultiSelect) )    flags |= OFN_ALLOWMULTISELECT;
        if ( Options.Contains(ofExtensionDifferent) )  flags |= OFN_EXTENSIONDIFFERENT;
        if ( Options.Contains(ofPathMustExist) )       flags |= OFN_PATHMUSTEXIST;
        if ( Options.Contains(ofFileMustExist) )       flags |= OFN_FILEMUSTEXIST;
        if ( Options.Contains(ofCreatePrompt) )        flags |= OFN_CREATEPROMPT;
        if ( Options.Contains(ofShareAware) )          flags |= OFN_SHAREAWARE;
        if ( Options.Contains(ofNoReadOnlyReturn) )    flags |= OFN_NOREADONLYRETURN;
        if ( Options.Contains(ofNoTestFileCreate) )    flags |= OFN_NOTESTFILECREATE;
        if ( Options.Contains(ofNoNetworkButton) )     flags |= OFN_NONETWORKBUTTON;
        if ( Options.Contains(ofNoLongNames) )         flags |= OFN_NOLONGNAMES;
        //if ( Options.Contains(ofOldStyleDialog) )      flags |= 0;
        if ( Options.Contains(ofNoDereferenceLinks) )  flags |= OFN_NODEREFERENCELINKS;
        //if ( Options.Contains(ofEnableIncludeNotify) ) flags |= 0;
        //if ( Options.Contains(ofEnableSizing) )        flags |= 0;

        return flags;
}


//=============================== END OF FILE ===============================
