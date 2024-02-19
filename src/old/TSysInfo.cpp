//---------------------------------------------------------------------------

#include <vcl.h>
#include "Constants.h"
#include "Functions.h"
#pragma hdrstop

#include "TSysInfo.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------




//---------------------------------------------------------------------------
//				TSysInfo
//		      ( informazioni sul sistema )

//---------------------------------------------------------------------------

String TSysInfo::WindowsVersion ()
{
 // Use win32API to retrieve Windows version

 	// Local variables
        String FunctLabel = "TSysInfo::WindowsVersion";
	OSVERSIONINFO osvi;
	char szVersion [80];

	memset(&osvi, 0, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
	if ( !GetVersionEx (&osvi) ) Results.Error(FunctLabel,NULL);
        else Results.Ok(FunctLabel,"Success");

        switch ( osvi.dwPlatformId ) {

        case VER_PLATFORM_WIN32_NT  : {      		// Windows NT

        switch ( osvi.dwMajorVersion ) {
        /*
        case 3	 : { // Windows NT 3.x

        wsprintf (szVersion, "MS Windows NT %d.%d (Build %d) %s",
        	osvi.dwMajorVersion,
        	osvi.dwMinorVersion,
        	osvi.dwBuildNumber & 0xFFFF,
                osvi.szCSDVersion);

        } break; // end Windows NT 3.x

        case 4	 : { // Windows NT 4.x

        wsprintf (szVersion, "MS Windows NT %d.%d (Build %d) %s",
        	osvi.dwMajorVersion,
        	osvi.dwMinorVersion,
        	osvi.dwBuildNumber & 0xFFFF,
                osvi.szCSDVersion);

        } break; // end Windows NT 4.x

        case 5	 : { // Windows 2000

        wsprintf (szVersion, "MS Windows 2000 %d.%d (Build %d) %s",
        	osvi.dwMajorVersion,
        	osvi.dwMinorVersion,
        	osvi.dwBuildNumber & 0xFFFF,
                osvi.szCSDVersion);

        } break; // end Windows 2000

        case 6	 : { // Windows XP

        wsprintf (szVersion, "MS Windows XP %d.%d (Build %d) %s",
        	osvi.dwMajorVersion,
        	osvi.dwMinorVersion,
        	osvi.dwBuildNumber & 0xFFFF,
                osvi.szCSDVersion);

        } break; // end Windows XP
        */
        default : { // Windows NT xx

        wsprintf (szVersion, "MS Windows NT %d.%d (Build %d) %s",
        	osvi.dwMajorVersion,
        	osvi.dwMinorVersion,
        	osvi.dwBuildNumber & 0xFFFF,
                osvi.szCSDVersion);

        } // End Windows NT xx
        } // End switch majorversion

        } break; // end Windows NT

        case VER_PLATFORM_WIN32_WINDOWS	 : { 		// Windows 9x

        wsprintf (szVersion, "MS Windows 9x %d.%d (Build %d) %s",
        	osvi.dwMajorVersion,
        	osvi.dwMinorVersion,
                //osvi.dwBuildNumber & 0xFF00000,
                //osvi.dwBuildNumber & 0x00FF0000,
        	osvi.dwBuildNumber & 0x0000FFFF,
                osvi.szCSDVersion);

        } break; // end Windows 9x

        case VER_PLATFORM_WIN32s : {	     		// Windows 3.1

        wsprintf (szVersion, "MS Win32s %d.%d (Build %d)",
        	osvi.dwMajorVersion,
        	osvi.dwMinorVersion,
        	osvi.dwBuildNumber & 0xFFFF);

        } break; // end Windows 3.1

        default : {                          // No platform detected

        wsprintf (szVersion, "Unknown windows version %d.%d (Build %d)",
        	osvi.dwMajorVersion,
        	osvi.dwMinorVersion,
        	osvi.dwBuildNumber & 0xFFFF);
        }
        }// end switch platform

        return String (szVersion);
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

String TSysInfo::UserName ()
{
 // Use win32API to retrieve User Name

        // Local variables
        String FunctLabel = "TSysInfo::UserName";
    	String UserName = "";
 	TCHAR acUserName[NAME_BUFF];
	DWORD nUserName = sizeof(acUserName);

	if ( GetUserName(acUserName, &nUserName) ) {
             UserName = String (acUserName);
             Results.Ok(FunctLabel,"Success");
             }
        else {
              UserName = "User";
              Results.Error(FunctLabel,NULL);
              }
        return UserName;
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

String TSysInfo::ComputerName ()
{
 // Use win32API to retrieve User Name

        // Local variables
        String FunctLabel = "TSysInfo::ComputerName";
    	String ComputerName = "";
 	TCHAR acCompName[NAME_BUFF];
	DWORD nCompName = sizeof(acCompName);

	if ( GetComputerName(acCompName, &nCompName) ) {
             ComputerName = String (acCompName);
             Results.Ok(FunctLabel,"Success");
             }
        else {
              ComputerName = UNKNOWN_STRING;
              Results.Error(FunctLabel,NULL);
              }
        return ComputerName;
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

String TSysInfo::WindowsDir ()
{
 // Use win32API to retrieve Windows Directory

        // Local variables
        String FunctLabel = "TSysInfo::WindowsDir";
    	String WinDir = "";
 	TCHAR acWinDir[MAX_PATH];

	if ( GetWindowsDirectory( acWinDir , MAX_PATH ) ) {
             WinDir = String (acWinDir);
             Results.Ok(FunctLabel,"Success");
             }
        else {
              WinDir = UNKNOWN_STRING;
              Results.Error(FunctLabel,NULL);
              }
        return WinDir;
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

String TSysInfo::SystemDir ()
{
 // Use win32API to retrieve Windows System Directory

        // Local variables
        String FunctLabel = "TSysInfo::SystemDir";
    	String SysDir = "";
 	TCHAR acSysDir[MAX_PATH];

	if ( GetSystemDirectory( acSysDir , MAX_PATH ) ) {
             SysDir = String (acSysDir);
             Results.Ok(FunctLabel,"Success");
             }
        else {
              SysDir = UNKNOWN_STRING;
              Results.Error(FunctLabel,NULL);
              }
        return SysDir;
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

String TSysInfo::EnvironmentVar ( String Var )
{
 // Use win32API to retrieve the value of a Windows environment variable

 	// Local variables
        String FunctLabel = "TSysInfo::EnvironmentVar";

        Var = "%" + Var + "%";
        /*
        // Better a control?
        switch ( Var ) {
        	case "OS"       : Var = "%OS%"; break;
                case "PATH"     : Var = "%PATH%"; break;
                case "LIBPATH"  : Var = "%LIBPATH%"; break;
                case "HOMEPATH" : Var = "%HOMEPATH%"; break;
                case "TEMP"     : Var = "%TEMP%"; break;
                case "TMP"      : Var = "%TMP%"; break;
                default : return "Unknown";
                } // end switch
        */

        String ExpandedVar;
	TCHAR  StrBuf[HUGE_BUFF];
	DWORD  bufCharCount;

	bufCharCount = ExpandEnvironmentStrings( Var.c_str(),StrBuf,HUGE_BUFF );

	if( !bufCharCount ) {
              ExpandedVar = UNKNOWN_STRING;
              Results.Error(FunctLabel,NULL);
              }
    	else {
	      if ( bufCharCount > HUGE_BUFF ) {
              	   ExpandedVar = String ( StrBuf );
                   Results.Error(FunctLabel,"Warning: buffer too small to expand");
                   }
              else {
              	    ExpandedVar = String ( StrBuf );
                    Results.Ok(FunctLabel,"Success");
                    }
              }
        return ExpandedVar;
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

String TSysInfo::LastMsgError ()
{
 // Retrieves the message relative to the last error

        TCHAR sysMsg[MSG_BUFF];
        // PVOID * sysMsg // (char*) dynamic allocation
        DWORD eNum;
        eNum = GetLastError();
        FormatMessage(
        	      FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, // source and processing options  // dynamic allocation: | FORMAT_MESSAGE_ALLOCATE_BUFFER
                      NULL,       // pointer to  message source
                      eNum,   	  // requested message identifier
                      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),  // language identifier for requested message
                      sysMsg,	 // pointer to message buffer
                      MSG_BUFF,  // maximum size of message buffer  // dynamic allocation: minimum length to allocate
                      NULL       // address of array of message inserts
                      );
        String ErrMsg = String (sysMsg);
	// LocalFree( sysMsg ); // dynamic allocation
        return "(" + String(eNum) + ") " + ErrMsg.TrimRight();
}


//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void TSysInfo::CPU (TStringList* S)
{
 // Use win32API to retrieve some system informations

	// Local variables
        String FunctLabel = "TSysInfo::CPU";
 	SYSTEM_INFO sysi;   // struct for system information
        OSVERSIONINFO osvi; // struct for OS version information

	// Retrieve informations calling API functions
	GetSystemInfo(&sysi);

        memset(&osvi, 0, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
	if ( !GetVersionEx (&osvi) ) Results.Error("CPU",NULL);

        // Output strings
        String arch,level,revision;

        // Some translations depending on platform
        switch ( osvi.dwPlatformId ) {

        case VER_PLATFORM_WIN32_NT  : {      // Win32 on Windows NT

        Results.Ok(FunctLabel,"Success");

        // Processor architecture/level/revision
        switch ( sysi.wProcessorArchitecture ){// architecture
		case PROCESSOR_ARCHITECTURE_INTEL : {
                arch="Intel";
                switch ( sysi.wProcessorLevel ) {// level

                	case 1 : {
                        level = "1";
                        } break;

                	case 3 : {
                        level = "80386";
                        unsigned short xx = (sysi.wProcessorRevision & 0xFF00) / 0xFF;
                        if ( xx == 0xFF ) revision = "Model n° " + String( ( (sysi.wProcessorRevision & 0x00F0)/0xF ) - 0xA ) + " , " + "stepping identifier " + String( (sysi.wProcessorRevision & 0xF) );
                        else revision = "Stepping " + String( xx ) + "A" + " , " + "minStepping " + String( LOBYTE(sysi.wProcessorRevision) );
                        } break;

                        case 4 : {
                        level = "80486";
                        } break;

                        case 5 : {
                        level = "80586"; // Pentium
                        revision = "Model " + String( HIBYTE(sysi.wProcessorRevision) ) + " , " + "Stepping " + String( LOBYTE(sysi.wProcessorRevision) );
                        } break;

                        case 6 : {
                        // Look wProcessorRevision high byte
                        	switch ( HIBYTE(sysi.wProcessorRevision) ) {
                                case 1  : level = "Pentium pro"; break;
                                case 3  :
                                case 4  :
                                case 5  : level = "Pentium II"; break;
                                case 6  : level = "Celeron"; break;
                                case 7  :
                                case 8  :
                                case 10 :
                                case 11 : level = "Pentium III"; break;
                                }
                        revision = "Model " + String( HIBYTE(sysi.wProcessorRevision) ) + " , " + "Stepping " + String( LOBYTE(sysi.wProcessorRevision) );
                        } break; // end case 6

                        case 15 : {
                        level = "Pentium 4";
                        revision = String( HIBYTE(sysi.wProcessorRevision) ) + "-" + String( LOBYTE(sysi.wProcessorRevision) );
                        } break;

                        default : {
                        arch = String (sysi.wProcessorArchitecture);
                        level = String (sysi.wProcessorLevel);
                        revision = String (sysi.wProcessorRevision);
                        }
                        } // ens switch level for Intel architecture
                } break;

                //case PROCESSOR_ARCHITECTURE_AMD64 : {
                //arch="AMD64";
                //} break;

                case PROCESSOR_ARCHITECTURE_IA64 : {
                arch="IA64";
                level = String (sysi.wProcessorLevel);
                revision = String( HIBYTE(sysi.wProcessorRevision) ) + "-" + String( LOBYTE(sysi.wProcessorRevision) );
                } break;

                case PROCESSOR_ARCHITECTURE_MIPS : {
                arch="Mips";
                level = "R" + String (sysi.wProcessorLevel) + "000";
                revision = String (sysi.wProcessorRevision );
                } break;

                case PROCESSOR_ARCHITECTURE_ALPHA : {
                arch="Alpha";
                level = String (sysi.wProcessorLevel);
                revision = "Model A" + String( HIBYTE(sysi.wProcessorRevision) ) + " Pass " + String( LOBYTE(sysi.wProcessorRevision) );
                } break;

                case PROCESSOR_ARCHITECTURE_PPC : {
                arch="PPC";
                	switch (sysi.wProcessorLevel) {// level PPC
                        case 1 : level = "PPC 601"; break;
                        case 3 : level = "PPC 603"; break;
                        case 4 : level = "PPC 604"; break;
                        case 6 : level = "PPC 603+"; break;
                        case 9 : level = "PPC 604+"; break;
                        case 20 : level = "PPC 620"; break;
                        default : level = "Unknown";
                        } // end switch level PPC
                revision = String( HIBYTE(sysi.wProcessorRevision) ) + "." + String( LOBYTE(sysi.wProcessorRevision) );
                } break;

                case PROCESSOR_ARCHITECTURE_UNKNOWN  : {
                arch = "Unknown";
        	level = "Unknown";
        	revision = "Unknown";
                } break;
                
  		default : {
                arch = String (sysi.wProcessorArchitecture);
                level = String (sysi.wProcessorLevel);
                revision = String (sysi.wProcessorRevision);
                }
		}// end switch architecture
        } break; // end Win32 on Windows NT

        case VER_PLATFORM_WIN32_WINDOWS	 : { // Win32 on Windows 95

        Results.Ok(FunctLabel,"Success");
        
        revision = "Not Retrieved";
        switch ( sysi.dwProcessorType ) { // processor type

		case PROCESSOR_INTEL_386 : {
                arch="Intel";
                level="386";
                } break;

                case PROCESSOR_INTEL_486 : {
                arch="Intel";
                level="486";
                } break;

                case PROCESSOR_INTEL_PENTIUM : {
                arch="Intel";
                level="Pentium or better";
                } break;

		//case PROCESSOR_INTEL_PENTIUM2 : {
                //arch="Intel";
                //level="Pentium II"
                //} break;
                //case PROCESSOR_INTEL_PENTIUM3 : {
                //arch="Intel";
                //level="Pentium III"
                //} break;
                case PROCESSOR_MIPS_R4000 : {
                arch="MIPS";
                level="R4000";
                } break;
                
                default : {
                arch = "Unknown";
                level = "Unknown";
                }
                } // end switch dwProcessorType
        } break; // end Win32 on Windows 95

        case VER_PLATFORM_WIN32s : {	     // Win32s on Windows 3.1

        Results.Error(FunctLabel,"Warning: Old Windows version");

        arch = "Not Retrieved";
        level = "Not Retrieved";
        revision = "Not Retrieved";
        } break; // end Win32s on Windows 3.1

        default : {

        Results.Error(FunctLabel,"Warning: Unknown Windows version");

        arch = String (sysi.wProcessorArchitecture);
        level = String (sysi.wProcessorLevel);
        revision = String( HIBYTE(sysi.wProcessorRevision) ) + "-" + String( LOBYTE(sysi.wProcessorRevision) );

        }
        }// end switch platform

        // Display the contents of the SYSTEM_INFO structure.
        S->Add("n°CPUs: " + String(sysi.dwNumberOfProcessors) );
        S->Add("CPU Arch: " + arch );
        S->Add("CPU Lvl: " + level );
        S->Add("CPU rev: " + revision );
        S->Add("Page size: " + String(sysi.dwPageSize) );
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void TSysInfo::Memory (TStringList* S)
{
 // Use win32API to retrieve system memory informations

        // Internal variables
        const double OneMega = 1048576; // = 1024*1024

        MEMORYSTATUS mems;
	ZeroMemory(&mems,sizeof(MEMORYSTATUS));
	mems.dwLength = sizeof (MEMORYSTATUS);
	GlobalMemoryStatus (&mems);

        S->Add("Installed RAM: " + String( ceil(mems.dwTotalPhys/OneMega) ) + "Mb, " + String(mems.dwMemoryLoad) + "% in use" + " (" + String( ceil(mems.dwAvailPhys/OneMega) ) + "Mb free)" );
        S->Add("Paging File: " + String( ceil(mems.dwTotalPageFile/OneMega) ) + "Mb (" + FloatToStrF ( 100 * float (mems.dwAvailPageFile) / mems.dwTotalPageFile,ffGeneral,5,1) + "% free)" );

        Results.Ok("TSysInfo::Memory","Done");
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void TSysInfo::RetrieveDiskInfo (String Drive)
{
 // Use Sysutils to retrieve HardDisk informations
 // If Drive is a number 0=current drive,1=A:,2=B:,3=C:,4=D:,ecc...
 // if Drive is a number > 64 is interpreted as the ASCII code of drive label
 // If Drive is not a number its first char is considered as the drive label

        // Local variables
        String DRlbl;
        int DRnum = Drive.ToIntDef(-1);

        // Recognizing input
        // String or Number?
        if ( DRnum == -1 ) {// String
              DRlbl = Drive.UpperCase()[1];
              // Now I must retrieve disk number
              DRnum = DRlbl[1] - 64;
              }
        else {// Number
              // Now I must retrieve the drive label
              // Representing a Char or a Number?
              if ( DRnum < 65 ) {// Number
              	    if ( DRnum ) DRlbl = char (DRnum+64);
              	    else DRlbl = ExtractFileDrive( GetCurrentDir() )[1];
              	    }
              else {// Char
              	    DRlbl = char (DRnum);
                    DRnum -= 64;
              	    }
              }

        // Retrieve member values of sDiskInfo

        // Label
        DiskInfo.Label = DRlbl + ":\\";

        String FunctLabel = "TSysInfo::RetrieveDiskInfo " + DiskInfo.Label;

        // Type
        unsigned int dt = GetDriveType( DiskInfo.Label.c_str() );
        switch ( dt ) {
        case 1 : {
        	  DiskInfo.Type = "Not existing";
                  Results.Error(FunctLabel,"Drive not existing");
                  } break;

        case DRIVE_REMOVABLE : DiskInfo.Type = "Removable drive"; break;
        case DRIVE_FIXED : DiskInfo.Type = "Fixed disk"; break;
        case DRIVE_REMOTE : DiskInfo.Type = "remote (network) drive"; break;
        case DRIVE_CDROM : DiskInfo.Type = "CD-ROM drive"; break;
        case DRIVE_RAMDISK : DiskInfo.Type = "RAM disk"; break;

        default : {
        	   DiskInfo.Type = "Unknown";
                   Results.Error(FunctLabel,"Warning: Unknown drive type");
                  }
        }

        // Total and Free space
        double byteSize = double ( DiskSize(DRnum) );
        double byteFree = double ( DiskFree(DRnum) );

        // detecting error
        if ( byteSize < 0 || byteFree < 0 ) {
              Results.Error(FunctLabel,"Invalid drive size");
              }

	DiskInfo.Size = byteSize / 1073741824;
        DiskInfo.Free = byteFree / 1073741824;

        // proper scale
        DiskInfo.SizeUnit = "Gb";
        DiskInfo.FreeUnit = "Gb";
        if ( DiskInfo.Size < 1 ) {
        			  DiskInfo.Size *= 1024;
                                  DiskInfo.SizeUnit = "Mb";
                                  }
        if ( DiskInfo.Free < 1 ) {
        			  DiskInfo.Free *= 1024;
                                  DiskInfo.FreeUnit = "Mb";
                                  }

        // Free space percentile
        DiskInfo.PrcFree = FloatToStrF (byteFree*100/byteSize,ffFixed,3,1) + "%";

        // Report
	DiskInfo.Report.sprintf("%s %s has %.3f %s free ( %s of %.3f %s)",
                        DiskInfo.Type,
        		DiskInfo.Label,
                        DiskInfo.Free,
                        DiskInfo.FreeUnit,
                        DiskInfo.PrcFree,
                        DiskInfo.Size,
                        DiskInfo.SizeUnit );
                        
        // Results.Ok(FunctLabel,"Success");
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
