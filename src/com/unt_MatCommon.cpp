//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
#include <windows.h>
#include <FileCtrl.hpp> // '::DirectoryExists()'
#include <Registry.hpp> 'TRegistry'
#include <CDERR.H> // common dialog errors
#include <shfolder.h>  // 'SHGetFolderPath()'  in bds:<SHFolder.hpp>
//#include <shlobj.h>  // 'SHGetMalloc()'
#include <mmsystem.h> // Win32 multimedia (PlaySound)
#include <limits> // x86 implementation number limits
//#include <exception> // std::exception
//#include <valarray>
//#include <algorithm> // std containers algorithms (max_element)
//#include <numeric> // std numeric algorithms (accumulate)
//#include <functional> // std elements functionals (multiplies)
//#include <cstdlib.h> // 'std::random()'
//#include <stdio.h>
//#include <time.h>
//#include <mapi.h> // Definitions used in 'MapiResult()'
#include <Mapi.hpp> // 'TMapiMessage'
//---------------------------------------------------------------------------
#include "unt_MatCommon.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

 //========================== Constants definition ==========================

// . . . Bool constants

// . . . Characters constants
        const char nms_MatCommon::InvalidChar = 0;
        //const char nms_MatCommon::AsciiCtrlChar_Size = 32;
        const char nms_MatCommon::AsciiCtrlCharStr[nms_MatCommon::AsciiCtrlChar_Size][4] = {"NUL","SOH","STX","ETX","EOT","ENQ","ACK","BEL","BS","TAB","LF","VT","FF","CR","SO","SI","DLE","DC1","DC2","DC3","DC4","NAK","SYN","ETB","CAN","EM","SUB","ESC","FS","GS","RS","US"};


// . . . Numeric constants
        const int nms_MatCommon::MaxInt = std::numeric_limits<int>::max(); // 2147483647L
        const int nms_MatCommon::MinInt = std::numeric_limits<int>::min(); //-2147483648L
        const double nms_MatCommon::SupDouble = std::numeric_limits<double>::max(); // 1.7E+308
        const double nms_MatCommon::InfDouble = std::numeric_limits<double>::min(); // 2.2E-308
        const double nms_MatCommon::Huge = 1E200;
        const double nms_MatCommon::eps = std::numeric_limits<double>::epsilon();
        const double nms_MatCommon::e = 2.7182818284590452353602874713527;
        const double nms_MatCommon::NaN = std::numeric_limits<double>::quiet_NaN(); // std::numeric_limits<double>::max(); //std::fmod(1.0/0.0)
        const double nms_MatCommon::pi = 3.1415926535897932384626433832795;
        const double nms_MatCommon::duepi = 6.283185307179586476925286766559;
        const double nms_MatCommon::pimezzi = 1.5707963267948966192313216916398;
        const double nms_MatCommon::deg2rad = 0.017453292519943295769236907684886; // [deg]->[rad]
        const double nms_MatCommon::rad2deg = 57.295779513082320876798154814105; // [rad]->[deg]
        const double nms_MatCommon::rps2rpm = 9.5492965855137201461330258023509; // [rad/s]->[rev/min]
        const double nms_MatCommon::rpm2rps = 0.10471975511965977461542144610932; // [rev/min]->[rad/s]
        const double nms_MatCommon::Log2 = 0.30102999566398119521373889472449; // base10 logarithm of 2
        const double nms_MatCommon::Log5 = 0.69897000433601880478626110527551; // base10 logarithm of 5
        const double nms_MatCommon::inv_argmin = 100 / nms_MatCommon::SupDouble;
        const double nms_MatCommon::log_argmin = 100 / nms_MatCommon::SupDouble;

// . . . System constants
        const double nms_MatCommon::OneMegaByte = 1048576; // = 1024*1024 bytes

// . . . Strings
        const String nms_MatCommon::NaNstr = String("NaN");
        const String nms_MatCommon::HexPrefix = String("0x");
        const char nms_MatCommon::HexDigits[] = "0123456789ABCDEF";


 //============================ Common functions ============================

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Numerics utilities

//---------------------------------------------------------------------------
// Get a random boolean given % probability of false
bool nms_MatCommon::GetRandomBool(const int p)
{
    static bool randomize_done = false;
    if (!randomize_done) { std::randomize(); randomize_done=true; }
    return std::random(100)>p;
}

//---------------------------------------------------------------------------
// Get a random integer between [m,M] (included)
int nms_MatCommon::GetRandomNumber(const int M, const int m)
{
    static bool randomize_done = false;
    if (!randomize_done) { std::randomize(); randomize_done=true; }
    return m + std::random(M-m+1);
}

//---------------------------------------------------------------------------
// Get a random number between [m,M] (included) and granularity g
double nms_MatCommon::GetRandomNumber(const double& M, const double& m, const double& g)
{
    static bool randomize_done = false;
    if (!randomize_done) { std::randomize(); randomize_done=true; }
    int iM = (M-m+1)/g;
    return m + g*std::random(iM);
}


//***************************************************************************
//              Redefinition of _matherr: throw exceptions
//    Remember to put:
//    _control87(MCW_EM,MCW_EM); // mask floating-point errors
//    in WinMain
//---------------------------------------------------------------------------
#ifdef USE_LONG_DOUBLES
int _matherrl(struct _exceptionl *except)
#else
int _matherr(struct _exception *except)
#endif
{return 1;}


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>> String utilities

//---------------------------------------------------------------------------
String nms_MatCommon::ForceToBeIdentifier (const String& s)
{ // Force a string to be an identifier name (alphanumeric or '_')
  // ex. "abc (d1)" -> "abc_d1"

    String sOut = "";
    for (int i=1; i<=s.Length(); ++i)
        {
         // Initial character can't be a number
         if ( IsAlpha(s[i]) || (IsNumber(s[i]) && i!=1) || s[i]=='_' ) sOut += s[i];
         else if ( s[i] == ' ' ) sOut += '_';
         else sOut += '_';
        }
    return sOut;
}

//---------------------------------------------------------------------------
String nms_MatCommon::HexToString(const String& s)
{ // Hex chars to chars ( ex: "64 65 66" => "def")
    String bag, o = "";
    int i=1;
    while ( i<=s.Length() )
        {
         bag = "";
         while ( i<=s.Length() && (bag.Length()<2) ) bag = (bag + String(s[i++])).Trim();
         if ( bag.Length() > 0 ) o += char( ("0x" + bag).ToIntDef(0) );
         ++i;
        }
    return o;
}

//---------------------------------------------------------------------------
String nms_MatCommon::StringToHex(const String& s)
{ // chars to hex codes ( ex: "def" => "64 65 66" )

    String o = "";
    for ( int i=1; i<=s.Length(); ++i ) o += ByteToHex(s[i]) + String("\ ");
    return o.Trim();
}

//---------------------------------------------------------------------------
WideString nms_MatCommon::ShiftCtrlChars(WideString s, const bool back)
{ // Shift non-printable chars to unicode representation

    if ( back )
         {
          for ( int i=1; i<=s.Length(); ++i ) if ( s[i]>=0x2400 && s[i]>0x2420 ) s[i] -= 0x2400;
         }
    else {
          for ( int i=1; i<=s.Length(); ++i ) if ( s[i]<0x20 ) s[i] += 0x2400;
         }
    return s;
}


//---------------------------------------------------------------------------
// Extract first part, ex: COM1,9600,E,8,1 => COM1
String nms_MatCommon::ExtractFirstPart(String& s, const char d)
{
    String bag = "";
    int i = 1;
    while ( i<=s.Length() && s[i]!=d ) bag += s[i++];
    s = After(s,i);
    return bag;
}


/*
//---------------------------------------------------------------------------
String nms_MatCommon::ChangeDecimalSeparator (const String& s)
{ // Toggle '.' - ','

    if (LastDelimiter(",",s)>0) return StringReplace(s, ",", ".", TReplaceFlags()<<rfReplaceAll);
    else if (LastDelimiter(".",s)>0) return StringReplace(s, ".", ",", TReplaceFlags()<<rfReplaceAll);
    else return s;
}

//---------------------------------------------------------------------------
String nms_MatCommon::CleanFloatLiteral ( const String& sIn )
{ // Clean a float literal from other chars

    String sOut = ""; // stringa di uscita
    bool IsSepMet = false; // separatore decimale già incontrato

    // Scansione
    for (int i=sIn.Length(); i>=1; i--)
        {
         // Mantengo l'ultimo DecimalSeparator e ignoro gli altri
         if ( !IsSepMet && sIn[i]==DecimalSeparator )
            {
             sOut += DecimalSeparator;
             IsSepMet = true;
             continue;
            }
         // Tengo solo i caratteri che formano 'Float literal strings'
         if ( IsFloat(sIn[i]) ) sOut += sIn[i];
        }
    return sOut;
}


//---------------------------------------------------------------------------
double nms_MatCommon::ForceStrToDouble ( String s )
{ // Converts a String to `double', forcing conversion if needed

        s = s.Trim();
        double v;
        if ( s.IsEmpty() ) v = 0;
        else
            {
             try{v = s.ToDouble();}
             catch (EConvertError& e)
                {
                 s = CleanFloatLiteral(s);
                 if ( s.IsEmpty() ) v = 0;
                 else v = s.ToDouble();
                }
            }
        return v;
}
*/



//>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Scanning utilities

//---------------------------------------------------------------------------
void nms_MatCommon::DecomposeArgs (mat::stringlist& arglst, char* argstr)
{ // Decompose properly command line arguments

    static const char quot = '\"';
    bool inquot = false; String arg =""; int i=0;
    while ( argstr[i] != '\0' )
      {
       if ( argstr[i] == quot ) inquot = !inquot;
       else if ( mat::IsSpacingChar(argstr[i]) && !arg.IsEmpty() && !inquot ) {arglst.push_back(arg); arg="";}
       else arg += argstr[i];
       ++i;
      }
    if (!arg.IsEmpty()) arglst.push_back(arg);
}

//---------------------------------------------------------------------------
void nms_MatCommon::StringToNumlist ( numlist& nl, const String& strseq )
{ // Get a sequence of ints from strings like:
  // all delimiters allowed: "12|234|1|3" or "34:5:33" or "345M65M21"
  // supports additions: "23+5.4;-3;-5.2+4"

    // . . . Detect delimiter
    char delm = '|'; // A default delimiter
    {int i=1;
     while ( i<=strseq.Length() )
           {
            if ( !IsFloat(strseq[i]) && !IsSpacingChar(strseq[i]) ) { delm=strseq[i]; break; }
            else ++i;
           }
    }

    // . . . Get strings
    stringlist tokens;
    Tokenize ( tokens, strseq.c_str(), delm );

    // Try to convert and collect this number
    for (unsigned int i=0; i<tokens.size(); ++i)
        {
         try { nl.push_back( Evaluate(tokens[i]) ); }
         catch(EConvertError& e){throw EConvertError("Invalid number \'" +tokens[i]+ "\' in position " + String(i) + " in: " + strseq);}
        }
}

//---------------------------------------------------------------------------
String nms_MatCommon::NumlistToString ( const numlist& nl, char delm )
{ // Translate numlist into String

    String ls = "";
    for (unsigned int i=0; i<nl.size(); ++i)
        {
         ls += DoubleToString(nl[i]) + String(delm);
        }
    return ls.SubString(1,ls.Length()-1);
}

//---------------------------------------------------------------------------
String nms_MatCommon::GetDelimitedSubStrings ( stringlist& sl, const String& s, const char d_start, const char d_end )
{ // Get the substrings delimited by given characters (trimmed)
  // ex. String s="<aa> <bb cc> <dd ee ff>";
  //     GetDelimitedSubStrings(sl,s,"<>");// sl = { "aa", "bb cc", "dd ee ff" }
  // returns the string external to the enclosed fields (prefixing)

//  . . . Preparation
    bool InField = false; // Am I in a delimited zone?
    String Inner; // A bag for substrings
    // Prefixing string
    int found_fields = 0;
    String prfx_str = "";

// . . . Parse string
    int i = 1; // current character position
    while ( i <= s.Length() )
       {
        if ( s[i] == d_start )
             { // Left delimiter encountered
              InField = true;
              Inner = "";
              // Read substring
              while ( ++i <= s.Length() )
                  {
                   if ( s[i] == d_start ) throw(Exception(String("Fields enclosed by \'")+String(d_start)+String(d_end)+String("\' cannot be nested")));
                   else if ( s[i] != d_end ) Inner += s[i];
                   else { // Right delimiter encountered
                         InField = false;
                         break;
                        }
                  }
              if ( InField ) throw(Exception(String("Missing right delimiter ")+String(d_end)));
              else {
                    sl.push_back(Inner.Trim());
                    ++found_fields;
                   }
             }
        else { // Outer space
              // . . . Outer string
              if ( found_fields )
                   {
                    // If I want only spaces or tabs
                    //if ( s[i]!=' ' && s[i]!='\t' ) throw(Exception("Characters between fields not allowed"));
                   }
              else { // Get prefixing string
                    prfx_str += s[i];
                   }
              ++i;
             }
       }// end string parsing
    return prfx_str;
}



//>>>>>>>>>>>>>>>>>>>>>>>>>>>>> I/O

/*
//---------------------------------------------------------------------------
void __fastcall nms_MatCommon::WriteFile (const String& FilePath, const String& Content)
{ //Save to a file

        // > Write with fprintf ( required #include <stdio.h> )
        FilePointer = fopen (FilePath.c_str(), "w");
        fprintf (FilePointer, "text to write %s\n",StringVariable.c_str());
        // Rimember escape sequences: \",\',\t,\n,\r, ecc
        fclose (FilePointer);
}
*/




//>>>>>>>>>>>>>>>>>>>>>>>>>>>>> System (Win32) - needs <windows.h>


/*---------------------------------------------------------------------------
HFONT nms_MatCommon::CreateRotatedFont ( const class TFont* Font, const int Deg )
{ // Create a rotated version of a TFont
  // example: Font->Handle = mat::CreateRotatedFont(FontOrig);
  // remark: don't call ::DeleteObject( Font->Handle );

    LOGFONT lf; // Windows native font structure
    ::ZeroMemory(&lf, sizeof(LOGFONT));
    ::GetObject(Font->Handle, sizeof(LOGFONT), &lf);
    //lf.lfPitchAndFamily = FIXED_PITCH;
    lf.lfEscapement = 10 * Deg; // degrees to rotate
    lf.lfOrientation = 10 * Deg;
    //lf.lfHeight = (170*Font->Size)/100; // how to calculate height??
    lf.lfQuality = PROOF_QUALITY;
    return ::CreateFontIndirect(&lf);
}*/

//---------------------------------------------------------------------------
void nms_MatCommon::TextOutEx (class TCanvas* Canvas, const int X, const int Y, const String& Txt, const int Deg)
{ // Canvas TextOut with angle

    //Canvas->Font->PixelsPerInch = ::GetDeviceCaps(Canvas->Handle, LOGPIXELSY);
    LOGFONT lf; // Windows native font structure
    ::GetObject(Canvas->Font->Handle, sizeof(lf), &lf);
    lf.lfEscapement = lf.lfOrientation = 10 * Deg; // degrees to rotate

    HFONT new_font = ::CreateFontIndirect (&lf);
    HFONT old_font = ::SelectObject (Canvas->Handle, new_font);

    //Canvas->TextOut(X,Y,Txt);
    ::TextOut(Canvas->Handle,X,Y,Txt.c_str(),Txt.Length());

    ::SelectObject(Canvas->Handle, old_font);
    ::DeleteObject(new_font);
}

//---------------------------------------------------------------------------
// Carica una stringa dalle risorse
String __fastcall nms_MatCommon::LoadString(const UINT id)
{
    char buf[512]; // TODO 4: dynamic size
    if ( ::LoadString(::HInstance,id,buf,sizeof(buf)) )
         {
          return String(buf);
         }
    else {
          MsgLastSysError();
          return "";
         }
}

//---------------------------------------------------------------------------
// Carica una bitmap dalle risorse
void __fastcall nms_MatCommon::LoadBitmap(Graphics::TBitmap* bmp, const UINT id)
{
    bmp->LoadFromResourceID(int(::HInstance), id);
    //bmp->LoadFromResourceName(HInstance, MAKEINTRESOURCE(id));
}

//---------------------------------------------------------------------------
// Carica un cursore dalle risorse
void __fastcall nms_MatCommon::LoadCursor(const TCursor& cr, const UINT id)
{
    HCURSOR Cur = ::LoadCursor(::HInstance, MAKEINTRESOURCE(id));
    if ( Cur ) Screen->Cursors[cr] = Cur;
    else MsgLastSysError();
}

//---------------------------------------------------------------------------
// Riproduce un suono dalle risorse
void __fastcall nms_MatCommon::PlaySoundFromRes(const UINT id)
{
    ::PlaySound(MAKEINTRESOURCE(id), ::HInstance, SND_ASYNC | SND_RESOURCE);
}


//---------------------------------------------------------------------------
// Execute external file in Os
void __fastcall nms_MatCommon::ShellExecute(const String& path, const String& exepar)
{
    HINSTANCE H = ::ShellExecute (
                                  Application->Handle, // handle to parent window
                                  NULL, // pointer to string that specifies operation to perform
                                  path.c_str(), // pointer to filename or folder name string
                                  exepar.c_str(), // pointer to string that specifies executable-file parameters
                                  NULL, // pointer to string that specifies default directory
                                  SW_SHOWNORMAL  // whether file is shown when opened
                                 );
// . . . Check errors
    int retcode = reinterpret_cast<int>(H);
    if ( retcode >= 0 && retcode <= 32 )
       {// ShellExecute error
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
            default : throw Exception("An unknown error occurred.");
           }
       }
}

//---------------------------------------------------------------------------
// See Mapi result, throwing exceptions
void __fastcall nms_MatCommon::MapiResult(const unsigned int retcode)
{
    if ( retcode != SUCCESS_SUCCESS )
       { // error
        switch ( retcode )
           {
            case MAPI_E_AMBIGUOUS_RECIPIENT : throw Exception("A recipient matched more than one of the recipient descriptor structures and MAPI_DIALOG was not set No message was sent");
            case MAPI_E_ATTACHMENT_NOT_FOUND : throw Exception("The specified attachment was not found; no message was sent");
            case MAPI_E_ATTACHMENT_OPEN_FAILURE : throw Exception("The specified attachment could not be open; no message was sent");
            case MAPI_E_BAD_RECIPTYPE : throw Exception("The type of a recipient was not MAPI_TO, MAPI_CC, or MAPI_BCC No message was sent");
            case MAPI_E_FAILURE : throw Exception("One or more unspecified errors occurred; no message was sent");
            case MAPI_E_INSUFFICIENT_MEMORY : throw Exception("There was insufficient memory to proceed No message was sent");
            case MAPI_E_LOGIN_FAILURE : throw Exception("There was no default logon, and the user failed to log on successfully when the logon dialog box was displayed No message was sent");
            case MAPI_E_TEXT_TOO_LARGE : throw Exception("The text in the message was too large to sent; the message was not sent");
            case MAPI_E_TOO_MANY_FILES : throw Exception("There were too many file attachments; no message was sent");
            case MAPI_E_TOO_MANY_RECIPIENTS : throw Exception("There were too many recipients; no message was sent");
            case MAPI_E_UNKNOWN_RECIPIENT : throw Exception("A recipient did not appear in the address list; no message was sent");
            case MAPI_E_USER_ABORT : throw Exception("The user canceled the process; no message was sent");
            default : throw Exception("An unknown MAPI error occurred");
           }
       }
}


//---------------------------------------------------------------------------
// Traslate Working Area
void nms_MatCommon::TraslateWorkingArea (int x)
{

    RECT rect;
    ::SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
    rect.left += x;
    ::SystemParametersInfo(SPI_SETWORKAREA, 0, &rect, SPIF_SENDCHANGE);
}

//---------------------------------------------------------------------------
// Process all windows messages in queue
void nms_MatCommon::ProcessMessages()
{
    MSG message;
    while ( ::PeekMessage(&message, NULL, 0, 0, PM_REMOVE) )
              {
               ::TranslateMessage(&message);
               ::DispatchMessage(&message);
              }
}


//---------------------------------------------------------------------------
// Retrieve the value of a Windows environment variable (if not found, returns the name)
String nms_MatCommon::GetEnvironmentVariable ( const String& AName )
{
    if( AName.IsEmpty() ) return AName;
    String Var;
    // See string length
    DWORD len = ::GetEnvironmentVariable(AName.c_str(),NULL,0);
    if ( len<1 ) return ""; // Failed/Not found
    cls_DynamicArray<TCHAR> Buf(len);
    len = ::GetEnvironmentVariable(AName.c_str(), Buf.get(), Buf.Size);
    return len ? String(Buf.get()) : String("");
}

//---------------------------------------------------------------------------
// Add permanently a user environment variable
void nms_MatCommon::AppendUserEnvironmentVariable( const String& AName, const String& AValue )
{
    if ( AName.IsEmpty() ) throw Exception("Attemp to sei an empty variable name");
// . . . Write proper key
    std::auto_ptr<TRegistry> Reg(new TRegistry);
    Reg->RootKey = HKEY_CURRENT_USER;
    if ( Reg->OpenKey("Environment",true) )
        {
         //if ( Reg->ValueExists(AName) ) String CurrentValue = Reg->ReadString(AName);
         Reg->WriteString( AName, AValue );
         Reg->CloseKey();
        }

// . . . Broadcast a 'WM_SETTINGCHANGE'
    DWORD dwRet;
    ::SendMessageTimeout(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM) "Environment", SMTO_ABORTIFHUNG, 2000, &dwRet);
}


//---------------------------------------------------------------------------
// Retrieve the system temporary folder path
String nms_MatCommon::GetTmpPath()
{
    TCHAR buf[MAX_PATH+1];
    return ::GetTempPath(MAX_PATH, buf) ? String(buf) : String("");
}

//---------------------------------------------------------------------------
String nms_MatCommon::GetHomePath()
{ // Get user home directory

// . . . Try specific API service
    TCHAR buf[MAX_PATH+1];
    if ( SUCCEEDED(::SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, NULL /*SHGFP_TYPE_CURRENT*/, buf)) )
    //if ( SUCCEEDED(::SHGetSpecialFolderPath(NULL, pszPath, CSIDL_PERSONAL, FALSE)) )
       {
        return String(buf);
       }

/* . . . Try other APIs
    String S = "";
    LPMALLOC ShellMalloc;
    if ( NOERROR == ::SHGetMalloc(&ShellMalloc) )
       {
        LPITEMIDLIST DesktopPidl;
        if ( SUCCEEDED(::SHGetSpecialFolderLocation(NULL, CSIDL_PERSONAL, &DesktopPidl)))
           {
            if ( ::SHGetPathFromIDList(DesktopPidl, buf) ) S = String(buf);
            ShellMalloc->Free(DesktopPidl);
           } // end 'SHGetSpecialFolderLocation succeeded'
        ShellMalloc->Release();
       } // end 'SHGetMalloc succeeded'
    if ( !S.IsEmpty() ) return S;
*/

// . . . Use environmental variable
    return ::IncludeTrailingBackslash(GetEnvironmentVariable("USERPROFILE")) + String("Documenti\\"); // %HOMEDRIVE%%HOMEPATH%

/* . . . Try get from registry

char *sMyDocumentsPath;
DWORD dwValData = 0;
CRegKey regKey;
if(regKey.Open(HKEY_CURRENT_USER,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders") != ERROR_SUCCESS)
{
	printf("Opening Registry Key for retrieving path of My Documents directory failed\n");
	return false;
}
else
{
	regKey.QueryValue(NULL,"Personal",&dwValData);
	sMyDocumentsPath = new char[dwValData];
	if(regKey.QueryValue(sMyDocumentsPath,"Personal",&dwValData) != ERROR_SUCCESS)
	{
		printf("Querying Registry key for retrieving path of My Documents directory failed \n");
        delete [] sMyDocumentsPath;
		return false;
	}
    delete [] sMyDocumentsPath;
}
*/

}

//---------------------------------------------------------------------------
// TODO 4: Select a folder (#include <shfolder.h>)
String nms_MatCommon::SelectFolder()
{
    TCHAR buf[MAX_PATH+1];
    // SHBrowseForFolder

    /*
    typedef struct _browseinfo {  
                                HWND hwndOwner;          // see below
                                LPCITEMIDLIST pidlRoot;  // see below
                                LPSTR pszDisplayName;    // see below
                                LPCSTR lpszTitle;        // see below
                                UINT ulFlags;            // see below
                                BFFCALLBACK lpfn;        // see below
                                LPARAM lParam;           // see below
                                int iImage;              // see below
                            } BROWSEINFO, *PBROWSEINFO, *LPBROWSEINFO;
    WINSHELLAPI LPITEMIDLIST WINAPI SHBrowseForFolder(LPBROWSEINFO lpbi);
    WINSHELLAPI BOOL WINAPI SHGetPathFromIDList(LPCITEMIDLIST pidl,LPSTR pszPath);
    */

/*
 char path[MAX_PATH];
 BROWSEINFO bi;
 ZeroMemory(&bi, sizeof(bi));
 bi.hwndOwner = Handle;
 bi.lpszTitle = "???????";
 bi.ulFlags = BIF_RETURNONLYFSDIRS;
 LPITEMIDLIST itemIdList = SHBrowseForFolder(&bi);
 if(itemIdList){
   SHGetPathFromIDList(itemIdList, path);
 }
*/

/*
//////CODE//////

// Here is how you get the elusive "Shell Task Allocator"!!!
        //
        LPMALLOC pMalloc;
        SHGetMalloc(&pMalloc);

        // buffer - a place to hold the file system pathname
        char buffer[MAX_PATH];

        LPITEMIDLIST pidlRoot;

        CoInitialize(NULL);

        // This struct holds the various options for the dialog
        BROWSEINFO bi;
        bi.hwndOwner = this->m_hWnd;
        bi.pidlRoot = NULL;
        bi.pszDisplayName = buffer;
        bi.lpszTitle = "Please choose the folder";
        bi.ulFlags = 0;
        bi.lpfn = NULL;

        // Now cause the dialog to appear.
        if((pidlRoot = SHBrowseForFolder(&bi)) == NULL)
        {
        // User hit cancel - do whatever
        return;
        }

        //
        // Again, almost undocumented. How to get a ASCII pathname
        // from the LPITEMIDLIST struct. I guess you just have to
        // "know" this stuff.
        //
        if(SHGetPathFromIDList(pidlRoot, buffer))
        {
        // Do something with the converted string.
                //SHGetPathFromIDList(myinfo,rep);

        CEdit *myedit;
        myedit = (CEdit *)GetDlgItem(IDC_CHOOSENFOLDER);
        myedit->SetWindowText(buffer);
        }

        // Free the returned item identifier list using the
        // shell's task allocator! Arghhhh.
        pMalloc->Free(pidlRoot);

//////END CODE ///////
*/
    return String(buf);
}

//---------------------------------------------------------------------------
// State if path is a directory
bool __fastcall nms_MatCommon::IsDirectory(const String& pth)
{
    return ::DirectoryExists(pth) && (::FileGetAttr(pth) & faDirectory);
}

//---------------------------------------------------------------------------
void nms_MatCommon::ListFiles( TStringList* sl, String path, const String mask, const bool subdirs)
{ // Fill a string list with files found in a dir, mask is ex. "*"

// . . . Preliminary
    WIN32_FIND_DATA fd;
    path = ::IncludeTrailingBackslash(path);

// . . . Find files
    HANDLE hFind = ::FindFirstFile((path + mask).c_str(), &fd);
    if ( hFind != INVALID_HANDLE_VALUE )
       {
        do {
            String s = String(fd.cFileName);

            // If a subdirectory, recursive search
            if ( subdirs && (fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) )
                 {
                  if (s != "." && s != "..") ListFiles(sl, path + s, mask, subdirs);
                 }
            else {
                  sl->Add(path + s);
                 }
           }
        while ( ::FindNextFile(hFind, &fd) );

        ::FindClose(hFind);
       }
}

//---------------------------------------------------------------------------
void nms_MatCommon::DeleteFiles( const String& pth, const String& mask )
{ // Delete all files in path

    TSearchRec sr;
    String path = ::IncludeTrailingBackslash(pth);
    if ( ::FindFirst(path+mask, faHidden, sr) == 0 )
       {
        do DeleteFile(path+sr.Name);
        while ( ::FindNext(sr) == 0 );
       }
    FindClose(sr);
}

//---------------------------------------------------------------------------
unsigned __int64 __fastcall nms_MatCommon::FileSize(const String &FileName)
{
    TSearchRec sr;
    if ( ::FindFirst(FileName, faAnyFile, sr) == 0 )
         {
          FindClose(sr);
          ULARGE_INTEGER ul;
          ul.LowPart = sr.FindData.nFileSizeLow;
          ul.HighPart = sr.FindData.nFileSizeHigh;
          return ul.QuadPart;
         }
    else {
          return 0;
         }
} 

//---------------------------------------------------------------------------
// Append to path
void __fastcall nms_MatCommon::PathAppend(String &pth, const String &app)
{
    pth = ::IncludeTrailingBackslash(pth) + app;
} 


//---------------------------------------------------------------------------
//  Changes priority of current process
//  fdwPiority:  IDLE_PRIORITY_CLASS, NORMAL_PRIORITY_CLASS,
//       HIGH_PRIORITY_CLASS, REALTIME_PRIORITY_CLASS
bool nms_MatCommon::SetProcPriority(DWORD dwPiority)
{
   // Detemine Process Handle using OpenProcess() and pass it to SetPriorityClass():
   return ::SetPriorityClass(::GetCurrentProcess(),dwPiority);
}

//---------------------------------------------------------------------------
// MAPI mail service must be configured by the user
void nms_MatCommon::MapiSend(const String& to, const String& sbj, const String& body, TStringList* attachs)
{
    // Note: MAPI mail service must be configured by the user

// . . . Sender structure
    MapiRecipDesc sender = {0};
    sender.ulRecipClass = MAPI_ORIG; // (ULONG) MAPI_TO, MAPI_CC, MAPI_BCC, MAPI_ORIG
    sender.lpszName = sbj.c_str(); // (LPTSTR) Recipient name
    //sender.lpszAddress = "a@a.a"; // (LPTSTR) Recipient address (optional)
    //sender.ulEIDSize = 0; // (ULONG) Count in bytes of size of pEntryID
    //sender.lpEntryID = NULL; // (LPVOID) System-specific recipient reference

// . . . Recipients structure
    MapiRecipDesc dest = {0};
    dest.ulRecipClass = MAPI_TO; // (ULONG) MAPI_TO, MAPI_CC, MAPI_BCC, MAPI_ORIG
    dest.lpszName = to.c_str(); // (LPTSTR) Recipient name
    dest.lpszAddress = to.c_str(); // (LPTSTR) Recipient address (optional)
    //dest.ulEIDSize = 0; // (ULONG) Count in bytes of size of pEntryID
    //dest.lpEntryID = NULL; // (LPVOID) System-specific recipient reference

// . . . Attachments structure
    const int attachments_maxnum = 2;
    MapiFileDesc attachments[attachments_maxnum] = {0};
    int attachments_num = attachs->Count;
    if ( attachments_num > attachments_maxnum ) throw Exception("Too many attachments in mail");
    for ( int i=0; i<attachments_num; ++i )
        {
         // . . . First attachment
         attachments[i].flFlags = 0; // (ULONG) Flags MAPI_OLE
         attachments[i].nPosition = -1; // (ULONG) character in text to be replaced by attachment
         attachments[i].lpszPathName = attachs->Strings[i].c_str(); // (LPSTR) Full path name of attachment file
         attachments[i].lpszFileName = NULL; // (LPSTR) Original file name (optional)
         attachments[i].lpFileType = 0; // (LPVOID) Attachment file type (can be lpMapiFileTagExt)
        }

// . . . Message structure
    TMapiMessage MapiMsg = {0};
    MapiMsg.ulReserved = 0; // (ULONG) Reserved for future use (M.B. 0)
    MapiMsg.lpszSubject = sbj.c_str();  // (LPSTR) Message Subject
    MapiMsg.lpszNoteText = body.c_str(); // (LPSTR) Message Text
    MapiMsg.lpszMessageType = NULL; // (LPSTR) Message Class
    MapiMsg.lpszDateReceived = NULL; // (LPSTR) in YYYY/MM/DD HH:MM format
    MapiMsg.lpszConversationID = NULL; // (LPSTR) conversation thread ID
    MapiMsg.flFlags = 0;  // (FLAGS) unread,return receipt
    MapiMsg.lpOriginator = &sender; // (lpMapiRecipDesc) Originator descriptor
    MapiMsg.nRecipCount = 1; // (ULONG) Number of recipients
    MapiMsg.lpRecips = &dest; // (lpMapiRecipDesc) Recipient descriptors
    MapiMsg.nFileCount = attachments_num; // (ULONG) Num of file attachments
    MapiMsg.lpFiles = attachments; // (lpMapiFileDesc) Attachment descriptors

// . . . Flags
    unsigned int MapiFlags = MAPI_DIALOG | MAPI_LOGON_UI | MAPI_NEW_SESSION;

// . . . Send message
    MapiResult( ::MapiSendMail(0, // unsigned session
                               0, // unsigned UIParam
                               MapiMsg, // MapiMessage Message
                               MapiFlags, // unsigned flags
                               0) // unsigned Reserved
              );
}


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Borland's VCL - needs <vcl>

//---------------------------------------------------------------------------
// Returns a string representation of a component (like in .dfm files)
String nms_MatCommon::ComponentToString(TComponent* Comp)
{
    std::auto_ptr<TStringStream> StrStream(new TStringStream(""));
    { // Create component binary stream and convert to string stream
     std::auto_ptr<TMemoryStream> BinStream(new TMemoryStream());
     BinStream->WriteComponent(Comp);
     BinStream->Seek(0, soFromBeginning);
     ::ObjectBinaryToText(BinStream.get(), StrStream.get());
    }
    // Return component string
    StrStream->Seek(0, soFromBeginning);
    return StrStream->DataString;
}


//---------------------------------------------------------------------------
// Returns a component from a given string
void nms_MatCommon::StringToComponent(const String& Source, TComponent* ReadComp)
{
    std::auto_ptr<TMemoryStream> BinStream(new TMemoryStream());

    { // Convert component string stream to binary stream
     std::auto_ptr<TStringStream> StrStream(new TStringStream(Source));
     StrStream->Seek(0, soFromBeginning);
     ::ObjectTextToBinary(StrStream.get(),BinStream.get());
    }
    // Instantiate component
    BinStream->Seek(0, soFromBeginning);
    ReadComp = BinStream->ReadComponent(NULL);
}

//---------------------------------------------------------------------------
// Returns a component from a given text file
void nms_MatCommon::TxtfileToComponent(const String& pth, TComponent* ReadComp)
{
    std::auto_ptr<TMemoryStream> BinStream(new TMemoryStream());
    std::auto_ptr<TFileStream> SrcStream(new TFileStream(pth, fmOpenRead|fmShareDenyWrite));

    { // Get string stream and convert to binary stream
     SrcStream->Seek(0, soFromBeginning);
     ::ObjectTextToBinary(SrcStream.get(),BinStream.get());
    }
    // Instantiate component
    BinStream->Seek(0, soFromBeginning);
    ReadComp = BinStream->ReadComponent(NULL);
}




//---------------------------------------------------------------------------
String nms_MatCommon::ExtractFileName ( const String& path )
{ // Extract file name without extension

    String fname = ::ExtractFileName(path);
    return fname.SubString(1,fname.LastDelimiter(".")-1);
}



//---------------------------------------------------------------------------
String nms_MatCommon::ShiftStateToString ( const TShiftState& Shift )
{ // Converts a TShiftState Set to String
    String s = "";
    if ( Shift.Contains(ssShift)  ) s+="+Shift";
    if ( Shift.Contains(ssAlt)    ) s+="+Alt";
    if ( Shift.Contains(ssCtrl)   ) s+="+Ctrl";
    if ( Shift.Contains(ssLeft)   ) s+="+Left";
    if ( Shift.Contains(ssRight)  ) s+="+Right";
    if ( Shift.Contains(ssMiddle) ) s+="+Middle";
    if ( Shift.Contains(ssDouble) ) s+="+Double";
    return s;
}

//---------------------------------------------------------------------------
String nms_MatCommon::PenStyleToString (const TPenStyle& ps)
{ // Converts TPenStyle to String

    switch ( ps )
           {
            case psSolid       : return "___";
            case psClear       : return "";
            case psDash        : return "_ _";
            case psDot         : return "...";
            case psDashDot     : return "_._";
            case psDashDotDot  : return "_..";
            case psInsideFrame : return "|_|";
            default            : return "??";
           }
}

//---------------------------------------------------------------------------
TPenStyle nms_MatCommon::StringToPenStyle (String s)
{ // Converts a String to TPenStyle

    //s = s.LowerCase();
    s = s.Trim();
    if      ( s == "___" ) return psSolid;
    else if ( s.IsEmpty()) return psClear;
    else if ( s == "_ _" ) return psDash;
    else if ( s == "..." ) return psDot;
    else if ( s == "_._" ) return psDashDot;
    else if ( s == "_.." ) return psDashDotDot;
    else if ( s == "|_|" ) return psInsideFrame;
    else throw EConvertError(s + " is not a valid TPenStyle");
}

//---------------------------------------------------------------------------
String nms_MatCommon::BrushStyleToString (const TBrushStyle& bs)
{ // Converts TBrushStyle to String

    switch ( bs )
           {
            case bsSolid      : return "##";
            case bsClear      : return "";
            case bsBDiagonal  : return "//";
            case bsFDiagonal  : return "\\\\";
            case bsDiagCross  : return "xx";
            case bsHorizontal : return "==";
            case bsVertical   : return "||";
            case bsCross      : return "++";
            default           : return "??";
           }
}

//---------------------------------------------------------------------------
TBrushStyle nms_MatCommon::StringToBrushStyle (String s)
{ // Converts a String to TBrushStyle

    s = s.Trim().LowerCase();
    if      ( s == "##" )   return bsSolid;
    else if ( s.IsEmpty())  return bsClear;
    else if ( s == "//" )   return bsBDiagonal;
    else if ( s == "\\\\" ) return bsFDiagonal;
    else if ( s == "xx" )   return bsDiagCross;
    else if ( s == "==" )   return bsHorizontal;
    else if ( s == "||" )   return bsVertical;
    else if ( s == "++" )   return bsCross;
    else throw EConvertError(s + " is not a valid TBrushStyle");
}

//---------------------------------------------------------------------------
String nms_MatCommon::PenModeToString (const TPenMode& pm)
{ // Converts TPenMode to String

    switch ( pm )
           {
            case pmBlack       : return "pmBlack";       // Always black
            case pmWhite       : return "pmWhite";       // Always white
            case pmNop         : return "pmNop";         // Unchanged
            case pmNot         : return "pmNot";         // Inverse of canvas background color
            case pmCopy        : return "pmCopy";        // Pen color specified in Color property
            case pmNotCopy     : return "pmNotCopy";     // Inverse of pen color
            case pmMergePenNot : return "pmMergePenNot"; // Combination of pen color and inverse of canvas background
            case pmMaskPenNot  : return "pmMaskPenNot";  // Combination of colors common to both pen and inverse of canvas background.
            case pmMergeNotPen : return "pmMergeNotPen"; // Combination of canvas background color and inverse of pen color
            case pmMaskNotPen  : return "pmMaskNotPen";  // Combination of colors common to both canvas background and inverse of pen
            case pmMerge       : return "pmMerge";       // Combination of pen color and canvas background color
            case pmNotMerge    : return "pmNotMerge";    // Inverse of pmMerge: combination of pen color and canvas background color
            case pmMask        : return "pmMask";        // Combination of colors common to both pen and canvas background
            case pmNotMask     : return "pmNotMask";     // Inverse of pmMask: combination of colors common to both pen and canvas background
            case pmXor         : return "pmXor";         // Combination of colors in either pen or canvas background, but not both
            case pmNotXor      : return "pmNotXor";      // Inverse of pmXor: combination of colors in either pen or canvas background, but not both
            default            : return "??";
           }
}

//---------------------------------------------------------------------------
TPenMode nms_MatCommon::StringToPenMode (String s)
{ // Converts a String to TPenMode

    s = s.Trim().LowerCase();

    if      ( s == "pmblack" )       return pmBlack;
    else if ( s == "pmwhite" )       return pmWhite;
    else if ( s == "pmnop" )         return pmNop;
    else if ( s == "pmnot" )         return pmNot;
    else if ( s == "pmcopy" )        return pmCopy;
    else if ( s == "pmnotcopy" )     return pmNotCopy;
    else if ( s == "pmmergepennot" ) return pmMergePenNot;
    else if ( s == "pmmaskpennot" )  return pmMaskPenNot;
    else if ( s == "pmmergenotpen" ) return pmMergeNotPen;
    else if ( s == "pmmasknotpen" )  return pmMaskNotPen;
    else if ( s == "pmmerge" )       return pmMerge;
    else if ( s == "pmnotmerge" )    return pmNotMerge;
    else if ( s == "pmmask" )        return pmMask;
    else if ( s == "pmnotmask" )     return pmNotMask;
    else if ( s == "pmxor" )         return pmXor;
    else if ( s == "pmnotxor" )      return pmNotXor;
    else throw EConvertError(s + " is not a valid TPenMode");
}


//---------------------------------------------------------------------------
String nms_MatCommon::TFontToString (TFont* f)
{ // Converts TFont* to String; ex. "Microsoft Sans Serif,10,0x000000,b"
// . . . Settings
    const String del = ",";

// . . . Build string
    String fstr = f->Name + del;
    fstr += String(f->Size) + del;
    fstr += "0x" + ::IntToHex(f->Color,6) + del;
    if ( f->Style.Contains(fsBold) ) fstr += "b";
    if ( f->Style.Contains(fsItalic) ) fstr += "i";
    if ( f->Style.Contains(fsUnderline) ) fstr += "u";
    if ( f->Style.Contains(fsStrikeOut) ) fstr += "s";

    if ( fstr[fstr.Length()] == del[1] ) fstr = fstr.SubString(1,fstr.Length()-1);

    return fstr;
}

//---------------------------------------------------------------------------
void nms_MatCommon::StringToTFont (const String& s, TFont* f)
{ // Converts a String to TFont; ex. "Microsoft Sans Serif,10,0x00000A,b"
// . . . Settings
    const char del = ',';

    if ( !f ) return;
// . . . Parse string
    stringlist toks;
    Tokenize(toks, s.c_str(), del);
    // Name
    if ( toks.size() > 0 )
       {
        f->Name = toks[0].Trim();
        // Size
        if ( toks.size() > 1 )
           {
            f->Size = StrToInt( toks[1].Trim() );
            // Color
            if ( toks.size() > 2 )
               {
                f->Color = TColor(StrToInt( toks[2].Trim() ));
                // Style
                f->Style = TFontStyles();
                if ( toks.size() > 3 )
                   {
                    for ( int i=toks[3].Length(); i>0; --i )
                        {
                         if (toks[3][i] == 'b') f->Style = f->Style << fsBold;
                         else if (toks[3][i] == 'i') f->Style = f->Style << fsItalic;
                         else if (toks[3][i] == 'u') f->Style = f->Style << fsUnderline;
                         else if (toks[3][i] == 's') f->Style = f->Style << fsStrikeOut;
                        }
                   }
               }
           }
       }
}

//---------------------------------------------------------------------------
bool nms_MatCommon::AreDifferent(class TFont* f1, class TFont* f2)
{ // A comparison function between TFont

    return (f1->Name != f2->Name) ||
           (f1->Size != f2->Size) ||
           (f1->Color != f2->Color) ||
           (f1->Pitch != f2->Pitch) ||
           (f1->Charset != f2->Charset);
}

//---------------------------------------------------------------------------
void nms_MatCommon::Copy(const class TCanvas* c1, class TCanvas* c2)
{ // A copy function, ehmm using 'Assign'?

    c2->Pen->Width = c1->Pen->Width;
    c2->Pen->Color = c1->Pen->Color;
    c2->Pen->Style = c1->Pen->Style;
    c2->Brush->Color = c1->Brush->Color;
    c2->Brush->Style = c1->Brush->Style;
    c2->Pen->Mode = c1->Pen->Mode;
    c2->Font->Assign(c1->Font);
}

//---------------------------------------------------------------------------
void __fastcall nms_MatCommon::SetFormTransparence ( TForm* form, int tr )
{// Set transparence of a Form (0-255)

    //if ( ComponentState.Contains(csDesigning) ) return;

    if ( IsWin2k() ) // Transparency os supported only for win2k or more
       {
        sat(tr,0,255);
        HWND h = form->Handle;
        ::SetWindowLong(h, GWL_EXSTYLE, ::GetWindowLong(h, GWL_EXSTYLE) | WS_EX_LAYERED);
        ::SetLayeredWindowAttributes(h, 0, tr, LWA_ALPHA);
       }
}

//---------------------------------------------------------------------------
void __fastcall nms_MatCommon::SetFormRoundRectRegion ( TForm* form, int joint)
{// Crop a rounded region on the form

    // We will be passing the dimensions of our form
    HRGN Region = ::CreateRoundRectRgn(0,0,form->Width,form->Height,joint,joint);
    // Check if the function returned an error
    if( Region != NULL )
            {
             //Attach the region we just created to our form
             ::SetWindowRgn(form->Handle, Region, true);
             DeleteObject(Region);
            }
}

//---------------------------------------------------------------------------
void __fastcall nms_MatCommon::SetFormEllipticRegion ( TForm* form )
{// Crop an elliptical region on the form

    // We will be passing the dimensions of our form
    HRGN Region = ::CreateEllipticRgn(0,0,form->Width,form->Height);
    // Check if the function returned an error
    if( Region != NULL )
            {
             //Attach the region we just created to our form
             ::SetWindowRgn(form->Handle, Region, true);
             DeleteObject(Region);
            }
}

//---------------------------------------------------------------------------
void __fastcall nms_MatCommon::FormPleaseWait (bool start)
{ // Feedback during waiting, remember to call FormPleaseWait(false) in the end

    static TForm* frmWait = 0;
    if ( start )
         {
          if ( !frmWait )
               {
                frmWait = new TForm(Application);
                frmWait = CreateMessageDialog("Please wait...", mtInformation, TMsgDlgButtons());
                Screen->Cursor = crHourGlass;
                frmWait->Show();
                frmWait->Repaint();
                Application->ProcessMessages();
               }
         }
    else {
          if ( frmWait )
               {
                Screen->Cursor = crDefault;
                frmWait->Close();
                delete frmWait;
                frmWait = 0;
               }
         }
}


//---------------------------------------------------------------------------
void __fastcall nms_MatCommon::FillGradientControl (TControl* ctrl, unsigned int Density)
{ // Fill a TControl with a gradient

    float Step;
    TRect BandRect;
    // Create the control canvas property
    TControlCanvas *PanelCanvas = new TControlCanvas;
    PanelCanvas->Control = ctrl;

    Step = static_cast<float>(ctrl->ClientHeight)/Density;
    for (unsigned int i=0; i<Density; i++)
        {
         BandRect.Left = 0;
         BandRect.Top = static_cast<int>(i*Step);
         BandRect.Right = ctrl->ClientWidth+1;
         BandRect.Bottom = static_cast<int>((i+1)*Step);
         PanelCanvas->Brush->Color = TColor(RGB(0,0,255-i)); // Blue
         PanelCanvas->FillRect(BandRect);
        }
    delete PanelCanvas;
}


//---------------------------------------------------------------------------
void nms_MatCommon::PickRandomPen( class TCanvas* Canvas )
{ // Choose a random Pen

    static bool FirstCall = true;
    if (FirstCall) { std::randomize(); FirstCall=false; } // call once

    Canvas->Pen->Color = Graphics::TColor( std::random(0xFFFFFF) );
    Canvas->Pen->Mode = pmCopy;
    switch ( std::random(6) )
            {
             case 1  : Canvas->Pen->Style = psDash; break;
             case 2  : Canvas->Pen->Style = psDot; break;
             case 3  : Canvas->Pen->Style = psDashDot; break;
             case 4  : Canvas->Pen->Style = psDashDotDot; break;
             default : Canvas->Pen->Style = psSolid;
            }
}


//---------------------------------------------------------------------------
void nms_MatCommon::PickRandomBrush( class TCanvas* Canvas )
{ // Choose a random Brush

    static bool FirstCall = true;
    if (FirstCall) { std::randomize(); FirstCall=false; } // call once

    Canvas->Brush->Color = Graphics::TColor( std::random(0xFFFFFF) );
    switch ( std::random(7) )
            {
             case 1  : Canvas->Brush->Style = bsCross; break;
             case 2  : Canvas->Brush->Style = bsDiagCross; break;
             case 3  : Canvas->Brush->Style = bsBDiagonal; break;
             case 4  : Canvas->Brush->Style = bsHorizontal; break;
             case 5  : Canvas->Brush->Style = bsFDiagonal; break;
             case 6  : Canvas->Brush->Style = bsVertical; break;
             default : Canvas->Brush->Style = bsSolid;
            }
}




//---------------------------------------------------------------------------
// Xoring text using temporary memory bitmap (winapi)
void nms_MatCommon::TextOutNotXor_Bmp (HDC hdc, const int X, const int Y, const char* Text, const int Length)
{
// . . . Prepare bitmap
    SIZE s;
    ::GetTextExtentPoint32(hdc,Text,Length,&s);

    HDC hdcmem = ::CreateCompatibleDC(hdc);
    ::SetTextColor(hdcmem, RGB(255,255,255));
    ::SetBkColor(hdcmem, RGB(0,0,0));
    HBITMAP hbitmap = ::CreateBitmap(s.cx,s.cy,1,1,NULL);
    hbitmap = (HBITMAP) ::SelectObject(hdcmem, hbitmap);

// . . . Write text
    ::ExtTextOut(hdcmem,0,0,0,NULL,Text,Length,NULL);
    ::BitBlt(hdc,X,Y,s.cx,s.cy,hdcmem,0,0,SRCINVERT);

// . . . Finally
    hbitmap = (HBITMAP) ::SelectObject(hdcmem, hbitmap);
    ::DeleteObject(hbitmap);
    ::DeleteDC(hdcmem);
}

//---------------------------------------------------------------------------
// Xoring text using temporary memory bitmap (VCL)
void nms_MatCommon::TextOutNotXor_Vcl (TCanvas* Canvas, const int X, const int Y, const String& Text)
{
// . . . Prepare bitmap
    Graphics::TBitmap* bmp;
    bmp = new Graphics::TBitmap();
    try {
         bmp->Width = Canvas->TextWidth(Text);
         bmp->Height = Canvas->TextHeight(Text);
         TRect R1 = Rect(0,0,bmp->Width, bmp->Height);
         TRect R2 = Rect(X, Y, X+bmp->Width, Y+bmp->Height);
         bmp->Canvas->Font->Assign( Canvas->Font );
         bmp->Canvas->Font->Color = TColor(mat::Invert(Canvas->Font->Color));
         bmp->Canvas->Brush->Color = TColor(mat::Invert(Canvas->Brush->Color));
         bmp->Canvas->FillRect(R1);
    // . . . Out text
         bmp->Canvas->Brush->Style = bsClear;
         bmp->Canvas->TextOut(0, 0, Text);
         Canvas->CopyMode = cmSrcInvert;
         Canvas->CopyRect(R2, bmp->Canvas, R1);
        }
    __finally {
               delete bmp;
              }
}

//---------------------------------------------------------------------------
// Xoring text using paths
void nms_MatCommon::TextOutNotXor_Pth (TCanvas* Canvas, const int X, const int Y, const String& Text)
{
// . . . Prepare path
    ::SetBkMode( Canvas->Handle, TRANSPARENT );
    //::SetTextAlign(Canvas->Handle, TA_LEFT|TA_CENTER); // TA_BASELINE|TA_TOP
    ::BeginPath(Canvas->Handle);
        Canvas->TextOut(X, Y, Text);
    ::EndPath(Canvas->Handle);

// . . . Fill path
    Canvas->Brush->Color = Canvas->Font->Color;
    Canvas->Brush->Style = bsSolid;
    Canvas->Pen->Style = psClear;
    Canvas->Pen->Mode = pmNotXor;
    ::StrokeAndFillPath(Canvas->Handle);
    //::FillPath(Canvas->Handle);
}




//>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Other

//---------------------------------------------------------------------------
String nms_MatCommon::BuildMatlabArrayDef (const double start, const double step, const double end)
{ // Eavenly spaced array matlab definition string
    return String("(")+ ::FloatToStr(start) + String(":") +
                        ::FloatToStr(step)  + String(":") +
                        ::FloatToStr(end)   + String(")");
}


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Debug

#ifdef _DEBUG
    /*-----------------------------------------------------------------------
    int HandleAsserting(char *testStr,char *msgStr,char *fileStr,int line,int *callFlag)
    { // Asserting function
       // assert message & and return flag regarding aborting:
       // callFlag set if repeating forbidden
       static char s_text[199]=""; // don't assign dynamically in
                                   // case of 'out of memory' error
       wsprintf(s_text,"FAILED: %srn"
                       "Error: ( %s )rn"
                       "File '%s', Line %drn"
                       "Abort execution, allow assert Retry, or Ignore in future?",
                       msgStr,testStr,fileStr,line);
       switch ( ::MessageBox(NULL,s_text,"ASSERTION ERROR",MB_ABORTRETRYIGNORE) )
           {
             case IDIGNORE: // prevent calling again - turn off flag
                       *callFlag=0; // never call again
                       break;
             case IDABORT:  // return flag and break
                       return 1;    // abort/break
           }
       return 0;
    }*/
//---------------------------------------------------------------------------
void nms_MatCommon::HandleAsserting(char *tst, char *msg, char *fil, int lin, char *fnc, bool& skip)
{ // Asserting function (!use static char[] for memory errs)

    ::MessageBeep(MB_ICONASTERISK);
    String txt = String(fil) + String("\n") +
                 String(lin) + String("\n") +
                 String(fnc) + String("\n") +
                 String(tst) + String("\n") +
                 String(msg);

    int res = ::MessageBox(
                           Application->Handle,
                           txt.c_str(), "Assertion",
                           MB_APPLMODAL|MB_DEFBUTTON1|MB_ABORTRETRYIGNORE|MB_ICONWARNING
                          );
    switch (res) {
                  case IDABORT  : Application->Terminate(); break;
                  case IDIGNORE : skip = true; break;
                  default       : skip = false; //  IDRETRY or other
                 }
}
#endif

/*---------------------------------------------------------------------------
                    APPUNTI

        // Lunghezza di un array
        int n = sizeof(Array) / sizeof(Array[0]);

        // Controllo assegnazione
        if ( !Assigned(a) ) a = 1;

        // Array inizialization
        float y[3][3] = {
                         {1, 3, 5},       // y[0][0], y[0][1], y[0][2]
                         {2, 4, 6},       // y[1][0], y[1][1], y[1][2]
                         {3, 5, 7}        // y[2][0], y[2][1], y[2][2]
                        };

        // Puntatori intelligenti
        std::auto_ptr<TStringList> sl(new TStringList());

---------------------------------------------------------------------------*/


//============================== END OF FILE ================================

