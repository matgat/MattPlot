//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Sysutils.hpp>
#pragma hdrstop
#include "unt_Shutdown.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------


//========================== File scope variables ===========================
#define BADPLATFORM (WM_USER+3426)

//============================= Static Members ==============================
/*
static inline void ValidCtrCheck(cls_Shutdown *)
{// ValidCtrCheck is used to assure that the components
 // created do not have any pure virtual functions.
    new cls_Shutdown(NULL);
}
*/

//============================= Public methods ==============================

//---------------------------------------------------------------------------
__fastcall cls_Shutdown::cls_Shutdown (typ_ShutdownFlags flgs) : Flags(flgs)
{// Constructor

// . . . Default values
        i_Error = "";
        OnShutdown = 0;

// . . . Check for windows version
    ::OSVERSIONINFO osvi;
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    if ( ::GetVersionEx(&osvi) ) dwPlatformId = osvi.dwPlatformId;
    else dwPlatformId = BADPLATFORM;
}

//---------------------------------------------------------------------------
bool __fastcall cls_Shutdown::Shutdown()
{ // Main service

        // Execute event handler
        if (OnShutdown) OnShutdown();

        // Different behaviuor depending on windows version
    switch (dwPlatformId)
        {
         case BADPLATFORM : return false;
         case VER_PLATFORM_WIN32s : return ShutdownW32s();
         case VER_PLATFORM_WIN32_WINDOWS : return Shutdown95();
         case VER_PLATFORM_WIN32_NT : return ShutdownNT();
         default : i_Error = "Unknown windows system"; return false;
        }
}


//============================== Private Methods ============================

//---------------------------------------------------------------------------
bool __fastcall cls_Shutdown::ShutdownW32s()
{ // Win 9x
    bool ret = ::ExitWindows(NULL,NULL);
        if ( !ret ) i_Error = ::SysErrorMessage( ::GetLastError() );
        return ret;
}

//---------------------------------------------------------------------------
bool __fastcall cls_Shutdown::Shutdown95()
{ // Old win95
    bool ret = ::ExitWindowsEx(dwFlags,NULL);
        if ( !ret ) i_Error = ::SysErrorMessage( ::GetLastError() );
        return ret;
}

//---------------------------------------------------------------------------
bool __fastcall cls_Shutdown::ShutdownNT()
{ // Win NT,2K,XP
        bool ret = false;

    ::HANDLE hToken,hProcess;
    ::TOKEN_PRIVILEGES tp;
    ::LUID_AND_ATTRIBUTES la;
    ::LUID luid;
    if ( ::LookupPrivilegeValue(NULL,SE_SHUTDOWN_NAME,&luid) )
       {
        la.Luid = luid;
        la.Attributes = SE_PRIVILEGE_ENABLED;
        tp.PrivilegeCount = 1;
        tp.Privileges[0] = la;
        hProcess = ::OpenProcess( PROCESS_ALL_ACCESS,false,::GetCurrentProcessId() );
        if ( hProcess != INVALID_HANDLE_VALUE )
           {
        if ( ::OpenProcessToken(hProcess,TOKEN_ADJUST_PRIVILEGES,&hToken) )
           {
            if ( ::AdjustTokenPrivileges(hToken,false,&tp,NULL,NULL,NULL) )
                 {
                          ::CloseHandle(hToken);
                          ret = ::ExitWindowsEx(dwFlags,NULL);
                          if ( !ret ) i_Error = ::SysErrorMessage( ::GetLastError() );
                 }
                    else {
                          i_Error = "AdjustTokenPrivileges failed";
                  ::CloseHandle(hToken);
                         }
           }
                else i_Error = "OpenProcessToken failed";
           }
            else i_Error = "OpenProcessToken failed";
       }
        else i_Error = "hProcess is INVALID_HANDLE_VALUE";
    return ret;
}


//================================= Properties ==============================

//---------------------------------------------------------------------------
UINT cls_Shutdown::get_dwFlags()
{ // Get the flags in double-word format
        UINT fl = 0;
        if (i_Flags.Contains(sfFORCE)) fl |= EWX_FORCE;
        if (i_Flags.Contains(sfLOGOFF)) fl |= EWX_LOGOFF;
        if (i_Flags.Contains(sfREBOOT)) fl |= EWX_REBOOT;
        if (i_Flags.Contains(sfSHUTDOWN)) fl |= EWX_SHUTDOWN;
        if (i_Flags.Contains(sfPOWEROFF)) fl |= EWX_POWEROFF;
        return fl;
}


//=============================== END OF FILE ===============================
