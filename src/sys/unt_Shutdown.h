//---------------------------------------------------------------------------
#ifndef unt_ShutdownH
#define unt_ShutdownH
//---------------------------------------------------------------------------

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    *
    *       unt_Shutdown
    *
    *   cls_Shutdown
    *       ver: 0.02    (20.aug.2006)
    *       written by xiphias@bytamin-c.com
    *       readapted by Matteo Gattanini
    *
    *   Wraps up the API code needed to shutdown Windows
    *
    *   Example of usage:
    *   #include "unt_Shutdown.h"
    *   // ...
    *   cls_Shutdown sd( cls_Shutdown::typ_ShutdownFlags() << cls_Shutdown::sfLOGOFF);
    *   sd.OnShutdown = Bye;
    *   if ( !sd.Error.IsEmpty() ) ShowMessage("Wrong initialization " + sd.Error);
    *   if ( !sd.Shutdown() )
    *      {
    *       ShowMessage("Unable to logoff: " + sd.Error);
    *       sd.Flags >> cls_Shutdown::sfLOGOFF << cls_Shutdown::sfREBOOT << cls_Shutdown::sfFORCE;
    *       if ( !sd.Shutdown() ) ShowMessage("Unable forcing reboot: " + sd.Error);
    *      }
    *
    *   // Where
    *   //-------------------------------
    *   void __fastcall TForm1::Bye()
    *   {
    *       ShowMessage("Bye bye");
    *   }
    *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

//---------------------------------------------------------------------------
class cls_Shutdown
{
public://                                                       ~~~~~~~~~~~~~
// . . . Types definitions
    enum en_Flags {sfFORCE,sfLOGOFF,sfREBOOT,sfSHUTDOWN,sfPOWEROFF};
    typedef Set<en_Flags,sfFORCE,sfPOWEROFF> typ_ShutdownFlags;
    typedef void __fastcall (__closure *typ_FastVoidFun)();

// . . . Public methods
    __fastcall cls_Shutdown( typ_ShutdownFlags =typ_ShutdownFlags());
    bool __fastcall Shutdown(); // The main service

// . . . Properties
    __property typ_ShutdownFlags Flags = { read=i_Flags, write=i_Flags };
    __property UINT dwFlags = { read=get_dwFlags };
    __property String Error = { read=i_Error, write=i_Error };
    __property typ_FastVoidFun OnShutdown = {read=i_OnShutdown, write=i_OnShutdown};

// . . . Public attributes (!)

private://                                                      ~~~~~~~~~~~~~

// . . . Private attributes
    typ_ShutdownFlags i_Flags;
    // Auxiliary
    DWORD dwPlatformId;
    String i_Error;
    typ_FastVoidFun i_OnShutdown;

// . . . Private methods
    bool __fastcall ShutdownW32s();
    bool __fastcall Shutdown95();
    bool __fastcall ShutdownNT();

// . . . Properties
    UINT get_dwFlags();

};//-------------------------------------------------------------------------
#endif
 
