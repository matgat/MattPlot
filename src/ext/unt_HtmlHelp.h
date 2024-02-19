#ifndef unt_HtmlHelpH
#define unt_HtmlHelpH

//#include "..\\..\\ext\\htmlhelp\\htmlhelp.h" // from Microsoft™ HTML Help Workshop
#include <htmlhelp.h> // Already in BCB5 includes

        /* * * * * * * * * * * * * * * * * * * * * * * * * * * *
        *
        *   unt_HtmlHelp
        *   2008 - Matteo Gattanini
        *
        *   Derived from code by: * Marcel van Brakel
        *                           (brakelm@chello.nl)
        *                         * Hens Zimmerman
        *    Overview
        *    --------------------------------------------
        *    This unit contains a class 'cls_HtmlHelp' to
        *    manage Microsoft™ compiled HTML help files,
        *    using the Microsoft™ HTML Help `hhctrl.ocx´
        *    activeX control.
        *    With the current implementation strategy, when
        *    HH API fails to load each call to HtmlHelp service
        *    is translated in a normal 'ShellExecute' call on
        *    the passed chm file.
        *
        *    Licenses
        *    --------------------------------------------
        *    Use and modify freely
        *
        *    Release history
        *    --------------------------------------------
        *    ver 0.7 (dec.2008)
        *        .Using exceptions
        *    ver: 0.61    (feb.2008)
        *        .First release
        *
        *    Known bugs and todos
        *    --------------------------------------------
        *    *) Cast from pointer to member to pointer to funct
        *
        *    Example of usage:
        *    --------------------------------------------
        *    Example of usage:
        *    #include "unt_HtmlHelp.h"
        *    cls_HtmlHelp HH; // In automatic memory
        *    if ( !HH.Loaded )
        *     ShowMessage("API not loaded, using ShellExecute\n"
        *                 "HTML Help not properly installed");
        *    // Now must indicate the help file path
        *    String ChmPath = ::ExtractFilePath(ParamStr(0)) +
        *                     "..\\hlp\\help.chm";
        *    HH.Open(ChmPath,"Topic1.htm");
        *    HH.Open(ChmPath,ID_TOPIC_X);
        *
        *    Tips:
        *    --------------------------------------------
        *    MAPPING CONTEXT INTEGERS IN HTMLHELP PROJECTS
        *
        *   Prepare a header file {helptopics.h}
        *
        *   |  #define IDH_TOPICA 1020
        *   |  #define IDH_TOPICB 1030
        *   |  #define IDH_TOPICC 1040
        *
        *   Prepare an alias file {helptopics.ali}
        *
        *   |  IDH_TOPICA=html\topicA.htm : this is the topic a
        *   |  IDH_TOPICB=html\topicB.htm : this is the topic b
        *   |  IDH_TOPICC=html\topicC.htm : this is the topic c
        *
        *   Prepare a text popup file {popups.txt}
        *
        *   |  .topic IDH_TOPICA
        *   |  text popup for topic a
        *   |
        *   |  .topic IDH_TOPICB
        *   |  text popup for topic b
        *
        *   Now in the HtmlHelp project add these sections:
        *
        *   |  [FILES]
        *   |
        *   |  html\topicA.htm
        *   |  html\topicB.htm
        *   |  html\topicC.htm
        *   |
        *   |
        *   |  [ALIAS]
        *   |  #include helptopics.ali
        *   |
        *   |  [MAP]
        *   |  #include helptopics.h
        *   |
        *   |  [TEXT POPUPS]
        *   |  helptopics.h
        *   |  popups.txt
        *
        *
        *    HOOKING HELP MESSAGES IN VCL
        *
        *   To divert WinHelp call in a BCB project to
        *   HTML help actions, you can perform translation
        *   using the TApplicationEvents OnHelp event, for
        *   example:
        *   //-----------------------------------------------
        *   bool __fastcall TForm1::AppEventsHelp
        *              (WORD Command,int Data,bool &CallHelp)
        *   {
        *   CallHelp = false; // VCL should not call WinHelp
        *
        *   switch ( Command )
        *   {
        *   case HELP_CONTEXT :
        *       {
        *        HH.Open(Data);
        *        return true;
        *       }
        *   case HELP_CONTEXTPOPUP :
        *       {
        *        HH.Popup(Data);
        *        return true;
        *       }
        *   case ...
        *   default:
        *       {
        *        Application->HelpContext(HelpContext);
        *        return true;
        *       }
        *   }
        *   }
        *   // To create help message you can use:
        *   Application->HelpCommand(Command,Data);
        *
        * * * * * * * * * * * * * * * * * * * * * * * * * * * */


//-------------------------------- Settings ---------------------------------
// To avoid closing Html windows problem at destroying configures
// HTML Help to run on the same thread
//#define HHOCX_SINGLE_THREAD


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class cls_HtmlHelp {

public://                           ~~~~~~~~~~~~~
// ~~~~~~~~~~~~~ Types definitions              ~~~~~~~~~~~~~
    // Declare which functions I'll use
    typedef HWND WINAPI (*typ_HtmlHelp_Funct)(HWND, LPCSTR, UINT, DWORD_PTR);

// . . . Public methods
    cls_HtmlHelp (bool noapi =false);
    ~cls_HtmlHelp ();

    void Open (const String&, const String&, const String& =""); // Open help file by topic name (ex. "Welcome.htm")
    void Open (const String&, int, const String& =""); // Open help file by topic ID (ex. 1002)
    void Popup (const String&, int ); // Popup from topic text definition. Be sure 'PopupFile' is the correct text file
    void Popup ( HINSTANCE res_handle, int res_id ); // Popup from resource
    void Popup (const String& text); // Popup a given text

    void ToForeground(); // Put main help window in foreground

    // Utilities
    void SetupPopups(const String& txtfile, COLORREF forcol, COLORREF bkgcol, RECT mrgns, const String& fnt); // Popups settings
    static String WinHelpCmdToStr (int Command); // Translates a WinHelp command to String for debug purposes
    String hhctrlMaintenanceMessage(); // A message with HH library tip

// . . . Properties
    __property bool StayOnTop = {read=get_StayOnTop, write=set_StayOnTop}; // A HelpWindow behaviour
    __property bool Loaded = {read=get_Loaded}; // Determine if API is loaded or not

// . . . Public attributes (!)
    const bool ForceNoAPI; // A construction-time choice

private://                          ~~~~~~~~~~~~~

// . . . Private attributes

    // Html Help API related
    HANDLE HHLibrary; // Handle to the loaded hhctrl.ocx, 0 if not loaded
    typ_HtmlHelp_Funct HtmlHelp; // Main HH function API
    HWND HelpWindow,ParentWindow; // Window handles
    // Popups
    HH_POPUP PopupData;
    String PopupFile; // Popups text file definition in 'hhp' project
    String PopupFont; // Text font of popups

    // Auxiliary
    //static unsigned short Instances; // Number of class instances
    String Topic; // An auxiliary String
    #ifdef HHOCX_SINGLE_THREAD
      DWORD Cookie; // Cookie returned by Hhctrl.ocx (initialize/uninitialize)
    #endif

// . . . Private methods
    static HWND WINAPI NoHtmlHelp(HWND h,LPCSTR f,UINT c,DWORD_PTR d); // Open CHM without API
    #ifdef NO_MATCOMMON
    static void ShellExecute(const String&, const String&); // Open a file with 'ShellExecute' API
    #endif
    // API library dynamic load utilities
    void __fastcall LoadHHLibrary(); // Try to dynamically load the HH library
    void __fastcall GetHHLibraryPath(char* path, DWORD pathsize); // Try to get the HH library path from registry
    void FreeHHLibrary(); // Free the dynamically loaded HH library

// . . . Properties
    bool get_StayOnTop () const { return !(ParentWindow == ::GetDesktopWindow());}
    void set_StayOnTop (bool b) { ParentWindow = b ? Application->Handle : ::GetDesktopWindow(); }

    bool get_Loaded () const { return bool(HHLibrary); }

}; // end 'cls_HtmlHelp'
#endif
