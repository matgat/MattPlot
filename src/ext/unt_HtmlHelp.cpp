//---------------------------------------------------------------------------
#include <vcl.h>
#ifndef NO_MATCOMMON
  #include "unt_MatCommon.h"  // for 'ShellExecute'
#endif
#pragma hdrstop
#include "unt_HtmlHelp.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


//========================== File scope variables ===========================
//HH_GPROPID_SINGLETHREAD


//============================= Static Members ==============================
//unsigned short cls_HtmlHelp::Instances = 0; // Number of class instances


//---------------------------------------------------------------------------
HWND WINAPI cls_HtmlHelp::NoHtmlHelp(HWND h,LPCSTR f,UINT c,DWORD_PTR d)
{ // No Html API loaded, try to open CHM file without it

// . . . Check if must open or not
    if ( f==0 || c==HH_DISPLAY_TEXT_POPUP ) return 0; // Nothing to do if no path or popups

// . . . Retrieve path to chm (cannot access object members...)
    String chmpath = String(f); // Get passed string path
    // Discard possible suffix "::\" or ">"
    int p = chmpath.AnsiPos("::/");
    if ( p ) chmpath = chmpath.SubString(1,p-1);
    else {
          p = chmpath.AnsiPos(">");
          if ( p ) chmpath = chmpath.SubString(1,p-1);
         }
    // Can open topic with cmd switch '-mapid id'
        String par = "";
    if ( c == HH_HELP_CONTEXT ) par = " -mapid " + String(d);

// . . . Open chm
#ifdef NO_MATCOMMON
    ShellExecute(chmpath,par);
#else
    mat::ShellExecute(chmpath,par);
#endif

    return 0;
}


//---------------------------------------------------------------------------
String cls_HtmlHelp::WinHelpCmdToStr (int Command)
{ // Translates a WinHelp command to String for debug purposes
        // For notation convenience:
        #define HANDLECASE(x) case x : return #x;
    switch (Command)
       {
        HANDLECASE(HELP_CONTEXT) // Display topic
        HANDLECASE(HELP_QUIT) // Terminate help
        HANDLECASE(HELP_INDEX) // Display index
        HANDLECASE(HELP_HELPONHELP) // Display help on using help
        HANDLECASE(HELP_SETINDEX) // Set current Index for multi index help
        HANDLECASE(HELP_CONTEXTPOPUP)
        HANDLECASE(HELP_FORCEFILE)
        HANDLECASE(HELP_KEY) // Display topic for keyword in offabData
        HANDLECASE(HELP_COMMAND)
        HANDLECASE(HELP_PARTIALKEY)
        HANDLECASE(HELP_MULTIKEY)
        HANDLECASE(HELP_SETWINPOS)
        HANDLECASE(HELP_CONTEXTMENU)
        HANDLECASE(HELP_FINDER)
        HANDLECASE(HELP_WM_HELP)
        HANDLECASE(HELP_SETPOPUP_POS)
        default: return "Unknown WinHelp command";
       }
}

//---------------------------------------------------------------------------
String cls_HtmlHelp::hhctrlMaintenanceMessage ()
{ // A message with HH library tip
    return "The HtmlHelp related files are:\n\n"
           "* %windir%\HH.EXE (not registered)\n"
               "* %windir%\system32\HHCTRL.OCX (registered)\n"
               "* %windir%\system32\ITSS.DLL (registered)\n"
               "* %windir%\system32\ITIRCL.DLL (registered)\n"
               "\nMake sure they\'re present and registered.\n"
               "Download \'hhupd.exe\' from http://msdn.microsoft.com/";
}

#ifdef NO_MATCOMMON
//---------------------------------------------------------------------------
void cls_HtmlHelp::ShellExecute(const String& path, const String& exepar)
{ // Open a file with 'ShellExecute' API

    HINSTANCE H = ::ShellExecute (
                                  Application->Handle, // handle to parent window
                                  NULL, // pointer to string that specifies operation to perform
                                  path.c_str(), // pointer to filename or folder name string
                                  exepar.c_str(), // pointer to string that specifies executable-file parameters
                                  NULL, // pointer to string that specifies default directory
                                  SW_SHOWNORMAL  // whether file is shown when opened
                                 );

    int retcode = reinterpret_cast<int>(H);
    if ( retcode >= 0 && retcode <= 32 )
       {// errore di ShellExecute
        switch ( retcode )
           {
            case 0 : throw Exception("The operating system is out of memory or resources.");
            case ERROR_FILE_NOT_FOUND : throw Exception("The specified file was not found.");
            case ERROR_PATH_NOT_FOUND : throw Exception("The specified path was not found.");
            case ERROR_BAD_FORMAT : throw Exception("The .EXE file is invalid (non-Win32 .EXE or error in .EXE image).");
            case SE_ERR_ACCESSDENIED : throw Exception("The operating system denied access to the specified file.");
            case SE_ERR_ASSOCINCOMPLETE : throw Exception("The filename association is incomplete or invalid.");
            case SE_ERR_DDEBUSY : throw Exception("The DDE transaction could not be completed because other DDE transactions were being processed.");
            case SE_ERR_DDEFAIL : throw Exception("The DDE transaction failed.");
            case SE_ERR_DDETIMEOUT : throw Exception("The DDE transaction could not be completed because the request timed out.");
            case SE_ERR_DLLNOTFOUND : throw Exception("The specified dynamic-link library was not found.");
            //case SE_ERR_FNF : throw Exception("The specified file was not found.");
            case SE_ERR_NOASSOC : throw Exception("There is no application associated with the given filename extension.");
            case SE_ERR_OOM : throw Exception("There was not enough memory to complete the operation.");
            //case SE_ERR_PNF : throw Exception("The specified path was not found.");
            case SE_ERR_SHARE : throw Exception("A sharing violation occurred.");
            default : throw Exception("An unknown error occurred.";
           }
       }
    return ErrMsg;
}
#endif

//============================= Public methods ==============================


//---------------------------------------------------------------------------
cls_HtmlHelp::cls_HtmlHelp (bool noapi) : ForceNoAPI(noapi)
{ // Constructor

// . . . Default values
    HHLibrary = 0; // Handle to the loaded hhctrl.ocx, 0 if not loaded
    HtmlHelp = 0; // Main HH function API

    HelpWindow = 0; // Help Window handle
    StayOnTop = false; // This initializes ParentWindow
    #ifdef HHOCX_SINGLE_THREAD
      Cookie = 0; // Cookie returned by Hhctrl.ocx
    #endif

// . . . Dynamically load the HH library (deal with chm files)
    if ( ForceNoAPI ) HtmlHelp = &NoHtmlHelp; // Forced to not load HH API
    else {
          try { LoadHHLibrary(); }
          catch(...) { HtmlHelp = &NoHtmlHelp; }
         }
    #ifdef HHOCX_SINGLE_THREAD
      if (Loaded) HtmlHelp(NULL, NULL, HH_INITIALIZE, reinterpret_cast<DWORD>(&Cookie) ); // Fuckin' MS developers...
    #endif

    // If here, an object is instantiated
    //Instances++;

// . . . Final operations
    RECT margins = {-1,-1,-1,-1};
    SetupPopups("Popups.txt",-1,-1,margins,"MS Sans Serif,12,,ITALIC"); // Popups settings Font: facename,pointsize,charset,BOLD ITALIC UNDERLINE
}


//---------------------------------------------------------------------------
cls_HtmlHelp::~cls_HtmlHelp()
{ // Destructor

// . . . Assure library loaded ( HHLibrary != 0 )
    if ( Loaded )
       {
        #ifdef HHOCX_SINGLE_THREAD
          HtmlHelp(NULL, NULL, HH_UNINITIALIZE, Cookie);
        #endif

        // Close an eventual opened help window
        if ( ::IsWindow(HelpWindow) ) ::SendMessage(HelpWindow, WM_CLOSE, 0, 0);

        // Unload the dynamically loaded HH library
        FreeHHLibrary(); // (Has effect for initialized library only)
       }
}


//---------------------------------------------------------------------------
void cls_HtmlHelp::ToForeground()
{ // Put main help window in foreground

    if ( ::IsWindow(HelpWindow) ) ::ShowWindow(HelpWindow, SW_SHOW);
}



//---------------------------------------------------------------------------
void cls_HtmlHelp::Open (const String& chm_file, const String& topic_name, const String& window_name)
{ // Open help file by topic name (ex. "Welcome.htm")

    Topic = chm_file + "::/" + topic_name;
    if ( !window_name.IsEmpty() ) Topic += ">" + window_name;

    HelpWindow = HtmlHelp(ParentWindow, Topic.c_str(), HH_DISPLAY_TOPIC, 0);
}

//---------------------------------------------------------------------------
void cls_HtmlHelp::Open (const String& chm_file, int context_id, const String& window_name)
{ // Open help file by topicID (ex. 1002)

    Topic = chm_file;
    if ( !window_name.IsEmpty() ) Topic += ">" + window_name;

    HelpWindow = HtmlHelp(ParentWindow,Topic.c_str(),HH_HELP_CONTEXT,context_id);
}



//---------------------------------------------------------------------------
void cls_HtmlHelp::Popup (const String& chm_file, int context_id)
{ // Open a help file text popup by topicID (ex. 1002)

    Topic = chm_file + "::/" + PopupFile;

    // Position
    GetCursorPos( &PopupData.pt );

    PopupData.hinst = 0; // No handle required
    PopupData.idString = context_id; // Specifies a topic id
    PopupData.pszText = NULL; // No given text
    PopupData.cbStruct = sizeof(PopupData);

    HelpWindow = HtmlHelp(ParentWindow,Topic.c_str(),HH_DISPLAY_TEXT_POPUP,reinterpret_cast<DWORD>(&PopupData));
}


//---------------------------------------------------------------------------
void cls_HtmlHelp::Popup ( HINSTANCE res_handle, int res_id )
{ // Open a text popup from resource

    // Position
    GetCursorPos( &PopupData.pt );

    PopupData.hinst = res_handle; // Handle of the program or DLL that owns resource
    PopupData.idString = res_id; // Specifies the resource id
    PopupData.pszText = NULL; // No given text
    PopupData.cbStruct = sizeof(PopupData);

    HelpWindow = HtmlHelp(ParentWindow,0,HH_DISPLAY_TEXT_POPUP,reinterpret_cast<DWORD>(&PopupData));
}


//---------------------------------------------------------------------------
void cls_HtmlHelp::Popup (const String& text)
{ // Open a text popup containing a given text

    // Position
    GetCursorPos( &PopupData.pt );

    PopupData.hinst = 0; // No handle required
    PopupData.idString = 0; // No id required
    PopupData.pszText = text.c_str(); // (LPCTSTR) Specifies the text to display if 'idString' is zero
        PopupData.cbStruct = sizeof(PopupData);

    HelpWindow = HtmlHelp(ParentWindow,0,HH_DISPLAY_TEXT_POPUP,reinterpret_cast<DWORD>(&PopupData));
}


//-------------------------------- Utilities --------------------------------

//---------------------------------------------------------------------------
void cls_HtmlHelp::SetupPopups(const String& txtfile, COLORREF forcol, COLORREF bkgcol, RECT mrgns, const String& fnt)
{ // Popups settings

    PopupFile = txtfile; // Popups file definition in 'hhp' project
    PopupFont = fnt; // Text font of popups

    PopupData.clrForeground = forcol;   // (COLORREF) Foreground RGB color: use -1 for default
    PopupData.clrBackground = bkgcol;   // (COLORREF) Background RGB color: use -1 for default
    PopupData.rcMargins = mrgns;    // (RECT) Space between edges and text
    PopupData.pszFont = PopupFont.c_str();  // (LPCTSTR) Font
}





//================================= Properties ==============================
//---------------------------------------------------------------------------




//============================== Private Methods ============================


//---------------------------------------------------------------------------
void __fastcall cls_HtmlHelp::LoadHHLibrary()
{ // Try to dynamically load the HH library

    char HHLibraryPath[MAX_PATH+1];
    HHLibrary = 0;
    try { GetHHLibraryPath(HHLibraryPath, MAX_PATH); }
    catch (Exception& e) {throw Exception("Cannot retrieve library path: " + e.Message);}

    // Ok, now I have the library path; try to dinamically load the library
    HHLibrary = ::LoadLibrary(HHLibraryPath);
    if ( HHLibrary != 0 )
         { // Loading library success

          // Now get API functions
          HtmlHelp = reinterpret_cast<typ_HtmlHelp_Funct>( ::GetProcAddress(HHLibrary, "HtmlHelpA") );
          if ( HtmlHelp == NULL )
               { // Failed to get API function pointer
                ::FreeLibrary(HHLibrary);
                HHLibrary = 0;
                throw Exception("Failed to get API function pointer, I\'ll free the opened library");
               }
         } // end 'loading library success'
    else { // Loading library failed
          throw Exception("Loading library failed");
         }
}


//---------------------------------------------------------------------------
void __fastcall cls_HtmlHelp::GetHHLibraryPath(char* path, DWORD pathsize)
{ // Try to get the HH library path from registry

    // Registry subkey where find the 'hhctrl.ocx' library path
    const char HHPathRegKey[] = "CLSID\\{adb880a6-d8ff-11cf-9377-00aa003b7a11}\\InprocServer32";

    // Let's query Windows registry
    HKEY HHKey;
    if ( ::RegOpenKeyExA(HKEY_CLASSES_ROOT, HHPathRegKey, 0, KEY_QUERY_VALUE, (void **)&HHKey) == ERROR_SUCCESS )
         {
          // Now HHKey is ready to use, query it
          LONG r = ::RegQueryValueExA(HHKey, "", NULL, NULL, (LPBYTE)path, &pathsize);
          ::RegCloseKey(HHKey); // Now close the opened registry key
          if ( r != ERROR_SUCCESS ) throw Exception("Cannot query the opened registry key");
         }
    else { // Can't access the registry key
          throw Exception("Cannot open the registry key");
         }
}


//---------------------------------------------------------------------------
void cls_HtmlHelp::FreeHHLibrary()
{ // Free the dynamically loaded HH library
  // Warning: assure library loaded ( HHLibrary != 0 )

// . . . First of all close opened HTML windows
        HtmlHelp(NULL, NULL, HH_CLOSE_ALL, 0); // TODO 5: hang-up when exit with opened help
        Sleep(10); // Additional security that HH_CLOSE_ALL has finished...

// . . . Now unload library for this application
        ::FreeLibrary(HHLibrary);
        HHLibrary = 0;
}



//=============================== END OF FILE ===============================
