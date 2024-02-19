//---------------------------------------------------------------------------

#include <vcl.h>
#include "Functions.h"
#include "Constants.h"
#pragma hdrstop

#include "TFiles.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
//				TFile
//			( operazioni su files )


//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

String TFile::GetName ()
{
 // Retrieve file name from his path (based on Sysutils)

        Name = ExtractFileName ( Path );
        return Name;
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

String TFile::GetDirectory ()
{
 // Retrieve file Directory from his path (based on Sysutils)

        Directory = IncludeTrailingBackslash( ExtractFileDir ( Path ) );
        // Directory = ExtractFilePath ( Path );
        return Directory;
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

String TFile::GetExtension ()
{
 // Retrieve file extension from his path (based on Sysutils)

        Extension = ExtractFileExt ( Path );
        return Extension;
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

String TFile::GetType ()
{
 // Retrieve file type by handle

        // Local variables
	String FunctLabel = "TFile::GetType " + GetName();
                                         
        if ( PickHandle() == INVALID_HANDLE_VALUE ) {
              Results.Error(FunctLabel,"Unable to open file");
              Type = UNKNOWN_STRING;
              }
        else {
              DWORD t = GetFileType ( Handle );
              switch ( t ) {
		case FILE_TYPE_DISK    : Type = "disk file"; break;
                case FILE_TYPE_CHAR    : Type = "character file"; break;
                case FILE_TYPE_PIPE    : Type = "named or anonymous pipe"; break;
                case FILE_TYPE_UNKNOWN : Type = "unknown type"; break;
		default : Type = "unknown type";
                }
              FreeHandle ();
              }
        return Type;
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

String TFile::GetLastWrite ()
{
 // Retrieve date of last file change with Sysutils

	// Local variables
	String FunctLabel = "TFile::GetLastWrite " + GetName();

        // With Sysutils FileAge
        int date = FileAge (Path);

        if ( date < 0) {
        	        Results.Error(FunctLabel,"Failed, file don't exists");
        		Date.LastWrite = UNKNOWN_STRING;
                        }
        else {
              Results.Ok(FunctLabel,"Success");
              Date.LastWrite = FormatDateTime("dd/mm/yy ',' hh:nn:ss",
              		       FileDateToDateTime( date ) );
              }
        return Date.LastWrite;

        /* Retrieve file timestamp with Sysutils FileGetDate
        if ( PickHandle() == INVALID_HANDLE_VALUE ) {
              Results.Error(FunctLabel,"Unable to open file");
              Date.LastWrite = UNKNOWN_STRING;
              }
        int date = FileGetDate( int (Handle) );
        if ( date < 0) {
        	        Results.Error(FunctLabel,"Failed, File handle invalid");
        		LastWrite = UNKNOWN_STRING;
                        }
        else {
              Results.Ok(FunctLabel,"Success");
              Date.LastWrite = FormatDateTime("dd/mm/yy ',' hh:nn:ss",
              		       FileDateToDateTime( date ) );
              }
        FreeHandle (); */
}


//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

bool TFile::GetDates ()
{
 // Retrieve file dates with winAPI

	// Local variables
	String FunctLabel = "TFile::GetDates " + GetName();
        bool r = false;

        if ( PickHandle() == INVALID_HANDLE_VALUE ) {
              Results.Error(FunctLabel,"Unable to open file");
              Date.Creation = UNKNOWN_STRING;
              Date.LastAccess = UNKNOWN_STRING;
              Date.LastWrite = UNKNOWN_STRING;
              }
        else {
              FILETIME  ftCreate, ftAccess, ftWrite;

              // Retrieve the file times for the file
              r = GetFileTime(Handle, &ftCreate, &ftAccess, &ftWrite);

              if ( !r ) {
              	    Results.Error(FunctLabel,NULL);
		    Date.Creation = UNKNOWN_STRING;
                    Date.LastAccess = UNKNOWN_STRING;
                    Date.LastWrite = UNKNOWN_STRING;
		    }
              else {
                    Results.Ok(FunctLabel,"Success");

                    // Convert filetimes to String                    Date.Creation = FileTimeToString (ftCreate);                    Date.LastAccess = FileTimeToString (ftAccess);
                    Date.LastWrite = FileTimeToString (ftWrite);
              	    }
              FreeHandle ();
              }
        return r;
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

__int64 TFile::GetSize ()
{
 // Retrieve file date (based on Sysutils)

        //#include <io.h>
	//long filelength(int handle);

        // Local variables
	String FunctLabel = "TFile::GetSize " + GetName();

        if ( PickHandle() == INVALID_HANDLE_VALUE ) {
              Results.Error(FunctLabel,"Unable to open file");
              Size = -1;
              }
        else {
              DWORD dwsizeL;
              DWORD dwsizeH = 0;
              LARGE_INTEGER size;

              dwsizeL = GetFileSize (Handle, &dwsizeH);

              if ( dwsizeL == 0xFFFFFFFF ) {
              	    Results.Error(FunctLabel,"GetFileSize failed");
		    Size = -1;
		    }
              else {
                    Results.Ok(FunctLabel,"Success");
                    size.LowPart = dwsizeL;
                    size.HighPart = dwsizeH;
                    Size = size.QuadPart;
              	    }
              FreeHandle ();
              }
        return Size;
}


//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

String TFile::GetAttributes ()
{
 // Retrieve file Attributes (based on Sysutils)
 // R=readonly,S=system,H=hidden,A=archive,V=volumeID,D=directory

	// Local variables
	String FunctLabel = "TFile::GetAttributes " + GetName();
	int Attrs = FileGetAttr( Path );

	if ( Attrs == -1 ) {
       			    Results.Error(FunctLabel,"Failed");
                            return "";
                            }
        // Local variables
        String AttrStr = "";

        if ( Attrs & faReadOnly )  AttrStr = AttrStr + "R";
        if ( Attrs & faHidden )    AttrStr = AttrStr + "H";
        if ( Attrs & faSysFile )   AttrStr = AttrStr + "S";
        if ( Attrs & faArchive )   AttrStr = AttrStr + "A";
        if ( Attrs & faDirectory ) AttrStr = AttrStr + "D";
        if ( Attrs & faVolumeID )  AttrStr = AttrStr + "V";

        return AttrStr;
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int TFile::SetAttributes (String AttrStr)
{
 // Set file Attributes stored in a String (based on Sysutils)
 // R=readonly,S=system,H=hidden,A=archive,V=volumeID,D=directory

        // Local variables
	String FunctLabel = "TFile::SetAttributes " + GetName();
        int ErrorCode;
        int Attrs = 0;

        // Input conditioner
        AttrStr = AttrStr.UpperCase();

        // Build Wanted attributes
        // Ogni carattere della stringa
    	for (int i=1; i <= AttrStr.Length(); i++) {
        	switch ( AttrStr[i] ){
		case 'R' : Attrs += faReadOnly; break;
                case 'H' : Attrs += faHidden; break;
                case 'S' : Attrs += faSysFile; break;
                case 'A' : Attrs += faArchive; break;
                case 'D' : Attrs += faDirectory; break;
                case 'V' : Attrs += faVolumeID; break;
		default : Results.Error(FunctLabel,"Warning: Attribute not valid");
                }}
        ErrorCode = FileSetAttr(Path,Attrs);

        if ( !ErrorCode ) Results.Ok(FunctLabel,"Success");
        else Results.Error(FunctLabel,SysErrorMessage( ErrorCode ) );

        return ErrorCode;

        // Es. If hidden, make visible:
        // FileGetAttr( Path );
	// if ( Attrs & faHidden ) FileSetAttr( Path , Attrs & !faHidden);
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

bool TFile::GetVersionInfo ()
{
 // Retrieve file meta-informations (based on Win32API)

        // Local variables
	String FunctLabel = "TFile::GetInfo " + GetName();
        bool error = false;
        
        TFileVersionInfo fvi(Path);
        char* text;

        #define QUERY(X) \
        if ( fvi.StringQuery(#X,text) ) {\
              Info.X = text;\
              Results.Ok(FunctLabel,#X" success" ); } \
        else { Info.X = UNKNOWN_STRING;\
               error = true;\
               Results.Error(FunctLabel,#X" failed" ); }

        QUERY(FileVersion)
        QUERY(FileDescription)
        QUERY(InternalName)
        QUERY(OriginalFilename)
        QUERY(ProductName)
        QUERY(ProductVersion)
        QUERY(LegalCopyright)
        QUERY(LegalTrademarks)
        QUERY(Author)
        QUERY(CompanyName)
        QUERY(Comments)

        if ( !error ) Results.Ok(FunctLabel,"Wow, Total success!" );

        return error;
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

bool TFile::GetInfosByHandle ()
{
 // A lot of informations, using win32API

	// Local variables
	String FunctLabel = "TFile::GetSomeInfoByHandle " + GetName();
        bool r = false;
        LARGE_INTEGER li;

        if ( PickHandle() == INVALID_HANDLE_VALUE ) {
              Results.Error(FunctLabel,"Unable to open file");
              // operations UNKNOWN_STRING;
              }
        else {
              BY_HANDLE_FILE_INFORMATION  finf;

              // Retrieve the informations from the file
              r = GetFileInformationByHandle(Handle,&finf);

              if ( !r ) {
              	    Results.Error(FunctLabel,NULL);
		    // operations UNKNOWN_STRING;
		    }
              else {
                    Results.Ok(FunctLabel,"Success");
                    ExtendedAttributes = "";                    String Sep = "\n";                    // - - - - - Attributes                    if ( finf.dwFileAttributes & FILE_ATTRIBUTE_NORMAL )                       ExtendedAttributes = ExtendedAttributes + "no attributes";                    else {                     if ( finf.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE )        		ExtendedAttributes = ExtendedAttributes + "archive (backup or removal)" + Sep;                     if ( finf.dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED )        		ExtendedAttributes = ExtendedAttributes + "compressed" + Sep;                     if ( finf.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )        		ExtendedAttributes = ExtendedAttributes + "directory" + Sep;                     if ( finf.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN )        		ExtendedAttributes = ExtendedAttributes + "hidden" + Sep;                     if ( finf.dwFileAttributes & FILE_ATTRIBUTE_OFFLINE )        		ExtendedAttributes = ExtendedAttributes + "offline" + Sep;                     if ( finf.dwFileAttributes & FILE_ATTRIBUTE_READONLY )        		ExtendedAttributes = ExtendedAttributes + "read-only" + Sep;                     if ( finf.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM )        		ExtendedAttributes = ExtendedAttributes + "system" + Sep;                     if ( finf.dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY )        		ExtendedAttributes = ExtendedAttributes + "temporary" + Sep;                    }                    ExtendedAttributes = ExtendedAttributes.TrimRight();
                    // - - - - - Dates                    Date.Creation = FileTimeToString (finf.ftCreationTime);                    Date.LastAccess = FileTimeToString (finf.ftLastAccessTime);
                    Date.LastWrite = FileTimeToString (finf.ftLastWriteTime);

                    // - - - - - Volume serial number
                    VolumeSerial = finf.dwVolumeSerialNumber;

                    // - - - - - Size
                    li.LowPart = finf.nFileSizeLow;
                    li.HighPart = finf.nFileSizeHigh;
                    Size = li.QuadPart;

                    // - - - - - Number of Links to file
                    NLinks = finf.nNumberOfLinks;

    		    // - - - - - Index
                    // This identifier, with the volume serial
                    // number, uniquely identify a file
                    li.LowPart = finf.nFileIndexLow;
                    li.HighPart = finf.nFileIndexHigh;
                    Index = li.QuadPart;
                    
              	    }
              FreeHandle ();
              }
        return r;
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void TFile::GetAll ()
{
 // Retrieve all members values

	GetName ();
        GetDirectory ();
        GetExtension ();
        GetType ();
        GetInfosByHandle();
          //GetLastWrite();
          //GetDates ();
          //GetSize ();
        GetAttributes ();
        GetVersionInfo ();
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

bool TFile::Exists ( )
{
 // Controls the existence of a given file (based on Sysutils)

        return FileExists ( Path );
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
/*
bool TFile::NotExists ( )
{
 // Controls the existence of a given file (based on stdio.h)

	// Local variables
	String FilePath = FixStringPath(Path);

        FILE * FilePointer = fopen (FilePath.c_str(), "r");

        if ( FilePointer == NULL ) return true;
        else return false;
}
*/

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

bool TFile::Delete ()
{
 // Delete the file - Returns false if fails

 	// Local variables
	String FunctLabel = "TFile::Delete " + GetName();

        if ( !Exists() ) {
              Results.Error(FunctLabel,"Warning: No File to Delete");
              return true;
              }

        bool r = DeleteFile( Path );

	if ( r ) Results.Ok(FunctLabel,"Ok");
        else Results.Error(FunctLabel,"Fail");
        
        return r;
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

bool TFile::Copy (String DestinationDir, bool AskOverwrite)
{
 // Copy to a directory - Returns false if fails

 // Best portability: system("copy c:\\foofoo.txt d:\\data\\foofoo.txt");
 // Without security attributes: CopyFile,CopyFileEx
 // ex. CopyFile("c:\\foofoo.txt", "d:\\data\\foofoo.txt", FALSE);
 // With security attributes: SHFileOperation

        // Local variables
        bool DestFileExists;
        String FunctLabel = "TFile::Copy " + GetName();

        // Input conditioner
        if (DestinationDir[DestinationDir.Length()]!='\\') DestinationDir += "\\";

        String DestinationPath = DestinationDir + Name;

        // Destination file exists? (Sysutils)
        DestFileExists = FileExists ( DestinationPath );

        // Conditioner
        //DestinationPath = FixStringPath ( DestinationPath );

        if ( Exists() ) {// File to copy exists

        if ( DestFileExists ) {// A destination file already exists
              if ( AskOverwrite ) {// Ask if overwrite
              	    String Msg = "Destination file: " + DestinationPath + "\nalready exists, overwrite?";
                    if ( Application->MessageBox(Msg.c_str(), FunctLabel.c_str(), MB_YESNO) == IDYES ) {
                          Results.Ok(FunctLabel,"Attempt overwriting");
                          }
                    else {
                          Results.Ok(FunctLabel,"Aborted: You chose to not overwrite");
                          return true;
                          }
              	    }
              else {// Overwrite without prompting
                    Results.Ok(FunctLabel,"Attempt overwriting");
              	    }
              }
        else {// No destination file
              }
        }// end File to copy exists
        else {// File to copy doesn't exists
              Results.Error(FunctLabel,"Warning: no file to Copy");
              return true;
        }

	// Perform operation
        bool r = CopyFile( Path.c_str(), DestinationPath.c_str(), false );
        if ( !r ) Results.Error( FunctLabel,NULL);
        else Results.Ok(FunctLabel,"Success");
        return r;
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

bool TFile::Move ( String DestinationDir )
{
 // Move to a directory - Returns false if fails

 // Best portability: system("move c:\\foofoo.txt d:\\data\\foofoo.txt");
 // Without security attributes: MoveFile,MoveFileEx
 // With security attributes: SHFileOperation

        // Local variables
        bool DestFileExists;
        String FunctLabel = "TFile::Move " + GetName();

        // Input conditioner
        if (DestinationDir[DestinationDir.Length()]!='\\') DestinationDir += "\\";

        String DestinationPath = DestinationDir + Name;

        // Destination file exists? (Sysutils)
        DestFileExists = FileExists ( DestinationPath );

        // Conditioner
        //DestinationPath = FixStringPath ( DestinationPath );

        if ( Exists() ) {// File exists

              if ( DestFileExists ) {// A destination file already exists
           	   Results.Error(FunctLabel,"Operation failed, destination file already exists");
              	   return false;
              	   }
              else {// No destination file
              	    // Perform operation
                    bool r = MoveFile( Path.c_str(), DestinationPath.c_str() );
                    if ( !r ) Results.Error( FunctLabel,NULL);
                    else Results.Ok(FunctLabel,"Success");
                    return r;
                    }
              }

        else {// File doesn't exists
              Results.Error(FunctLabel,"Operation failed, source file doesn't exists");
              return false;
              }
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void TFile::Open ()
{
 // Acts like a double click

 	// If file doesn't exists, do nothing
 	if ( !Exists() ) return;

        // Local variables
        bool DestFileExists;
        String FunctLabel = "TFile::Open " + GetName();
        // Path = FixStringPath(Path);
        String ErrMsg = "";
	try {
	     int E = (int) ShellExecute(
        		   0,	   	 // handle to parent window
                           "open",	 // pointer to string that specifies operation to perform
                           Path.c_str(), // pointer to filename or folder name string
                           "",		 // pointer to string that specifies executable-file parameters
                           "",		 // pointer to string that specifies default directory
                           SW_SHOWNORMAL // whether file is shown when opened
     			   );

             if ( E >= 0 && E <= 32 ) {// errore di ShellExecute
              switch ( E ){
               case 0 : ErrMsg = "The operating system is out of memory or resources."; break;
               case ERROR_FILE_NOT_FOUND : ErrMsg = "The specified file was not found."; break;
               case ERROR_PATH_NOT_FOUND : ErrMsg = "The specified path was not found."; break;
               case ERROR_BAD_FORMAT : ErrMsg = "The .EXE file is invalid (non-Win32 .EXE or error in .EXE image)."; break;
               case SE_ERR_ACCESSDENIED : ErrMsg = "The operating system denied access to the specified file."; break;
               case SE_ERR_ASSOCINCOMPLETE : ErrMsg = "The filename association is incomplete or invalid."; break;
               case SE_ERR_DDEBUSY : ErrMsg = "The DDE transaction could not be completed because other DDE transactions were being processed."; break;
               case SE_ERR_DDEFAIL : ErrMsg = "The DDE transaction failed."; break;
               case SE_ERR_DDETIMEOUT : ErrMsg = "The DDE transaction could not be completed because the request timed out."; break;
               case SE_ERR_DLLNOTFOUND : ErrMsg = "The specified dynamic-link library was not found."; break;
               //case SE_ERR_FNF : ErrMsg = "The specified file was not found."; break;
               case SE_ERR_NOASSOC : ErrMsg = "There is no application associated with the given filename extension."; break;
               case SE_ERR_OOM : ErrMsg = "There was not enough memory to complete the operation."; break;
               //case SE_ERR_PNF : ErrMsg = "The specified path was not found."; break;
               case SE_ERR_SHARE : ErrMsg = "A sharing violation occurred."; break;
               default : ErrMsg = "An Error occurred.";
               }// end switch
              ErrMsg = "Errore di ShellExecute nell\'aprire:\n\n" + Path + "\n\n" + ErrMsg;
              Application->MessageBox(ErrMsg.c_str(), FunctLabel.c_str(), MB_OK|MB_ICONEXCLAMATION );
              }// end if
             }// end try
	catch (Exception&) {
        		    ErrMsg = "Eccezione nell'aprire:\n\n" + Path;
                            Application->MessageBox(ErrMsg.c_str(), FunctLabel.c_str(), MB_OK|MB_ICONEXCLAMATION );
                            }
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

String TFile::CustomReport ( String Template )
{
 // A string containing chosen file characteristics
 // Template Syntax ( some chars ° ` ´ · ¶ ¹ ² ³ « » ¼ ½ ¾ ÷ ¤ )
 // Template string is a series of field separate each others:
 // <right delim decl><left delim decl><field><separator><field><sep>...
 // First and second chars of Template must be delimiters:
 // First char : left field delimiter declaration
 // Second char : right field delimiter declaration
 // These delimiters embrace each field
 // Fields contain a small string representing the information
 // to display, according to following conventions:

 // pt = Path               (percorso completo del file)
 // nm = Name               (nome del file)
 // dr = Directory          (directory che contiene il file
 // ex = Extension          (estensione del file)
 // tp = Type               (tipo di file)
 // at = Attributes         (sintesi attributi del file)
 // ax = ExtendedAttributes (attributi in formato esteso)
 // tc = Creation           (data di creazione)
 // ta = LastAccess         (data ultimo accesso)
 // tw = LastWrite          (data ultima modifica)
 // sz = Size               (dimensione del file)
 // nl = NLinks             (numero di links al file)
 // id = Index              (indice univoco del file)
 // sv = VolumeSerial       (numero di serie del volume)
 // 			    (meta informazioni nel file)
 // fv = FileVersion;
 // fd = FileDescription;
 // in = InternalName;
 // on = OriginalFilename;
 // pn = ProductName;
 // pv = ProductVersion;
 // lc = LegalCopyright;
 // lt = LegalTrademarks;
 // au = Author;
 // cn = CompanyName;
 // cm = Comments;

 // Example: CustomReport ("() -(nm) | (tc) and (sz)\n");  produces:
 // -file.fl | 12/06/2004 and 8.32Kb

 #define MAX_FIELDS 30

 #define ABORT(mex) {\
 Results.Error("TFile::CustomReport","Template syntax error: " mex); \
 return ""; }

        // First Control
        int L = Template.Length();
        if ( L < 6 ) ABORT ("Template Too Short")

        // Read delimiters declaration
        char ld,rd;
        ld = Template[1];
        rd = Template[2];

        if ( isalpha(ld) ) ABORT ("Invalid Left Delimiter")
        if ( isalpha(rd) ) ABORT ("Invalid Right Delimiter")

        // Some local variables
        char c; // readed character
        bool InField = false;  // I'm in a field
        bool SepFound = false; // previously found a separator
        int n = 0; // current couple field-separator
        // Sep[0]-Field[0]-Sep[1]-Field[1]-...-Field[L-1]-LastSep
        String Field[MAX_FIELDS],Sep[MAX_FIELDS+1];
        Sep[0] = "";
        int i=3; // the char counter

        // --------------------------- Parse Template String
    	for (i; i <= L; i++) {

            if ( Template[i] == ld ) {// Field encountered
            
            	if ( InField ) ABORT ("Nested fields")
                else {
                      if ( !SepFound ) Sep[n] = ""; // empty separator
                      SepFound = false;
                      InField = true;
                      Field[n] = "";

                      // Read field string
                      i++;
                      while ( Template[i] != rd ) {

                          c = Template[i];
                          if ( !isalpha(c) ) ABORT ("Invalid Field String")
                          else Field[n] = Field[n] + c;

                          i++;
                          if ( i > L ) ABORT("Missing Right Delimiter")
                      	  }
                      InField = false;
                      n++;
                      if ( n > MAX_FIELDS ) ABORT ("Too Many Fields")
                      Sep[n] = "";
                      }// end parsing Field
                }// end Field encountered

            else { // Read the separator
                  if ( InField ) ABORT ("Something Wrong")
                  SepFound = true;
                  Sep[n] = Sep[n] + Template[i];
            	  }
            }// end for

        // --------------------------- End parsing


        // --------------------------- Build the Report String

        String Report = "";
        int k=0; // couple Sep-Field counter
        
    	for (k ; k<n; k++) {// every Sep-Field

           // Adding the Separator
           Report =  Report + Sep[k];

           // Setting the Field Value
           
           	if ( Field[k] == "pt" ) Field[k] = Path;
           else if ( Field[k] == "nm" ) Field[k] = Name;
           else if ( Field[k] == "dr" ) Field[k] = Directory;
           else if ( Field[k] == "ex" ) Field[k] = Extension;
           else if ( Field[k] == "tp" ) Field[k] = Type;
           else if ( Field[k] == "at" ) Field[k] = Attributes;
           else if ( Field[k] == "ax" ) Field[k] = ExtendedAttributes;
           else if ( Field[k] == "tc" ) Field[k] = Date.Creation;
           else if ( Field[k] == "ta" ) Field[k] = Date.LastAccess;
           else if ( Field[k] == "tw" ) Field[k] = Date.LastWrite;
           else if ( Field[k] == "sz" ) Field[k] = SizeString(Size);
           else if ( Field[k] == "nl" ) Field[k] = String(NLinks);
           else if ( Field[k] == "id" ) Field[k] = String(Index);
           else if ( Field[k] == "sv" ) Field[k] = String(VolumeSerial);
           else if ( Field[k] == "fv" ) Field[k] = Info.FileVersion;
           else if ( Field[k] == "fd" ) Field[k] = Info.FileDescription;
           else if ( Field[k] == "in" ) Field[k] = Info.InternalName;
           else if ( Field[k] == "on" ) Field[k] = Info.OriginalFilename;
           else if ( Field[k] == "pn" ) Field[k] = Info.ProductName;
           else if ( Field[k] == "pv" ) Field[k] = Info.ProductVersion;
           else if ( Field[k] == "lc" ) Field[k] = Info.LegalCopyright;
           else if ( Field[k] == "lt" ) Field[k] = Info.LegalTrademarks;
           else if ( Field[k] == "au" ) Field[k] = Info.Author;
           else if ( Field[k] == "cn" ) Field[k] = Info.CompanyName;
           else	if ( Field[k] == "cm" ) Field[k] = Info.Comments;
	   else {
                 Results.Error("TFile::CustomReport","Warning: field not recognized");
                 Field[k] = UNKNOWN_STRING;
                 }

           // Adding the Field Value
           Report = Report + Field[k];

           } // end for

        // Adding the Last Separator
           Report = Report + Sep[k];

        // --------------------------- End building

        return Report;
}


//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

String TFile::SizeString (__int64 siz)
{
 // Given a quantity in bytes, returns a string
 // representing the size with a proper unit

        int n = sizeof(SizeUnits) / sizeof(SizeUnits[0]);

        if ( siz == 0 ) return "0";
        if ( siz < 0 ) return UNKNOWN_STRING;
        else {
              double v = double (siz);
              int i;

              for (i=0;i<n-1;i++)
              	if ( (v / double(SizeUnits[i+1].Value) ) < 1 ) break;
                
              if ( i > n-1 ) i = n-1; // non si sa mai...

              return FloatToStrF (v/double(SizeUnits[i].Value),ffFixed,10,3) + " " + SizeUnits[i].Unit;
              }
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
HANDLE TFile::PickHandle ()
{
 // Get file handle; remember to close

	Handle = CreateFile ( Path.c_str(),
        		      GENERIC_READ,
                              FILE_SHARE_READ,
                              NULL,
                              OPEN_EXISTING,
                              FILE_ATTRIBUTE_NORMAL,
                              NULL );
        return Handle;
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void TFile::FreeHandle ()
{
 // Close file handle

	// Local variables
	String FunctLabel = "TFile::CloseHandle " + GetName();

        if ( Handle == INVALID_HANDLE_VALUE ) {
              return;
              }
        else {
              if ( !CloseHandle( Handle ) ) Results.Error(FunctLabel,NULL);
              return;
              }
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

String TFile::FileTimeToString (FILETIME ftTime)
{
 // converts a file time to a String

	// Local variables
	String FunctLabel = "TFile::FileTimeToString";
        FILETIME ftlocTime;
        SYSTEMTIME stTime;
        char BufStr[25];  // xx/xx/xxxx--xx:xx:xx = 20

	// Convert the filetimes to local times
        if ( !FileTimeToLocalFileTime(&ftTime, &ftlocTime) ) {        			Results.Error(FunctLabel,NULL);                                return UNKNOWN_STRING;                                }        // Convert the local filetimes from UTC to systemtimes        if ( !FileTimeToSystemTime(&ftlocTime, &stTime) ) {        			Results.Error(FunctLabel,NULL);                                return UNKNOWN_STRING;                                }        // Build strings showing the date and time        wsprintf(BufStr, "%02d/%02d/%d  %02d:%02d:%02d",        	 stTime.wDay, stTime.wMonth, stTime.wYear,                 stTime.wHour, stTime.wMinute, stTime.wSecond);        Results.Ok(FunctLabel,"Success");        return String ( BufStr );
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
String TFile::FixStringPath (String InputString)
{
 // A String representing a Path has to be correctly recognized

	// Local variables
	String OutputString = "";

	// Fixing `\' characters
        int len = InputString.Length();
    	for (int i=1; i <= len; i++) {
                // Se incontro un BackSlash
        	if (InputString[i] == '\\' ) {
                      do i++;
                      while ( InputString[i] == '\\' );

                      OutputString = OutputString + InputString[i] + "\\";
                      }
                else  OutputString = OutputString + InputString[i];
        	}// end for

        // Adds a final double BackSlash (Only Directories)
        // if (InputString[InputString.Length()]!='\\') InputString += "\\";
        // Dir = IncludeTrailingBackslash(Dir);

	// Adds double quotes avoiding problems with spaces
        //if (OutputString[1] != '\"' ) OutputString = "\"" + OutputString + "\"";

        // AnsiQuotedStr(String , char Quote);
        // AnsiExtractQuotedStr(char * &Src, char Quote);

        return OutputString;
}


//---------------------------------------------------------------------------




//---------------------------------------------------------------------------
//				TThisApplication

//---------------------------------------------------------------------------
TThisApplication::TThisApplication ()
{ // Constructor
        ShowBuild = true;
        Init ();
}
//---------------------------------------------------------------------------

void TThisApplication::Init ()
{
 // Retrieves application name, version, ecc...
 
        // Local variables
        TFile ExeFile;
        ExeFile.Path = Application->ExeName;
        ExeFile.GetVersionInfo ();

        // Deciding the name
        if ( ExeFile.Info.ProductName != UNKNOWN_STRING && ExeFile.Info.ProductName != NULL )
              Name = ExeFile.Info.ProductName;
        else {
              if ( ExeFile.Info.InternalName != UNKNOWN_STRING && ExeFile.Info.InternalName != NULL )
              	    Name = ExeFile.Info.InternalName;
              else {
                    if ( ExeFile.Info.OriginalFilename != UNKNOWN_STRING && ExeFile.Info.OriginalFilename != NULL )
              	    	  Name = ExeFile.Info.OriginalFilename;
                    else {
                          // Se non trovo nulla prendo il nome dell'eseguibile stesso:
 			  Name = ExtractFileName(Application->ExeName);
        		  // Tolgo l'estensione:
        		  Name.SetLength ( Name.LastDelimiter(".") - 1 );
                    	  }
              	    }
              }
              
        // Getting Author available information
        if ( ExeFile.Info.Author != UNKNOWN_STRING && ExeFile.Info.Author != NULL )
              Author = ExeFile.Info.Author;
        else {
              if ( ExeFile.Info.CompanyName != UNKNOWN_STRING  && ExeFile.Info.CompanyName != NULL )
              	    Author = ExeFile.Info.CompanyName;
              else {
              	    Author = UNKNOWN_STRING;
                    }
              }
        AuthorMail = UNKNOWN_STRING;

        // Getting the main directory
        Directory = ExeFile.GetDirectory();

        // Writing the Version string
	Version = ExeFile.Info.ProductVersion;
        if ( ShowBuild ) Version = Version + " (" + ExeFile.Info.FileVersion + ")";
}

//---------------------------------------------------------------------------

void TThisApplication::CreateAboutBox ()
{
 // Create an about box

	// Local variables
        TFile ExeFile;
        ExeFile.Path = Application->ExeName;
        ExeFile.GetVersionInfo();
        String Tit,Msg;

        if ( ExeFile.Info.CompanyName == Author ) ExeFile.Info.CompanyName = AnsiString::StringOfChar('-',30);

        Tit = "About " + Name;

        Msg = "\n" + Name + "\n\n" +\
              " Product version: " + ExeFile.Info.ProductVersion + "\n\n" +\
              " File version: "    + ExeFile.Info.FileVersion + "\n\n" +\
              " "		   + ExeFile.Info.FileDescription + "\n\n" +\
              " "                  + ExeFile.Info.Comments + "\n\n" +\
              " "  	    	   + Author + "\n" +\
              " " 	   	   + AuthorMail + "\n\n" +\
              " "  	   	   + ExeFile.Info.CompanyName + "\n\n" +\
              " "  	   	   + ExeFile.Info.LegalCopyright + "\n\n";
              //" Trademarks: "	   + ExeFile.Info.LegalTrademarks + "\n\n";

        // Wrap the text
        TSysCharSet wrchars;
	wrchars << '.' << '-' << ' ' << '\n';
	Msg = WrapText( Msg, "\n", wrchars, 50);
        
	Application->MessageBox(Msg.c_str(), Tit.c_str(), MB_ICONINFORMATION);
}
//---------------------------------------------------------------------------
TThisApplication* appl;
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
//				TFiles
//			  ( serie di files )


//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void TFiles::AddFile ( String FilePath )
{
 // Adds a file path in file list
        Paths->Append( FilePath );
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void TFiles::AddFiles ( TStringList * FilePaths )
{
 // Adds a file path in file list
        Paths->AddStrings( FilePaths );
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

String TFiles::GetNames ( String RecordSep )
{
 // Retrieve file names from their paths (based on TFile)
 // The result a String, each name separated by RecordSep

        // Local variables
        String Names = "";

        // For all files
        for (int i=0;i<Paths->Count;i++) {
                CurrFile.Path = Paths->Strings[i];
                Names = Names + CurrFile.GetName() + RecordSep;
                }
        return Names;
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

String TFiles::ReportFiles ( String Template )
{
 // Report Files Added

        // Local variables
        String Report = "";

        if ( Paths->Count ) {
        // For all files:
        for (int i=0;i<Paths->Count;i++) {
                CurrFile.Path = Paths->Strings[i];
                CurrFile.GetAll();
                Report = Report + CurrFile.CustomReport (Template);
                }
        }
        else Report = "No files";
        return Report.TrimRight();
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

bool TFiles::Exists ( )
{
 // Controls the existence of all files (based TFile)
        // Internal variables
        bool AllExists = true;

        // Check all files
        for (int i=0;i<Paths->Count;i++) {
                CurrFile.Path = Paths->Strings[i];
                AllExists = AllExists && CurrFile.Exists();
                }
        return AllExists;
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

bool TFiles::ListExistingIn ( String Dir , TStringList* S )
{
 // Build a List containing the files in Dir having the
 // same names of the files contained in Paths.
 // This list is ready to be assigned to a TFiles.Paths
 // Returns true if almost one exists

        // Internal variables
        bool AlmostOneExists = false;
        TSearchRec F;

        // Input Control
        if (Dir[Dir.Length()]!='\\') Dir += "\\";

        // For all files in Paths
        for (int i=0;i<Paths->Count;i++) {
                CurrFile.Path = Paths->Strings[i];

        	// Cerca il file in Dir
        	if ( !FindFirst( Dir + CurrFile.GetName() , faAnyFile , F) ) {
        		do {
                    	    S->Append( Dir + F.Name);
                            AlmostOneExists = true;
                            }
                	while ( !FindNext(F) );
                	}
		FindClose(F);
                }

        return AlmostOneExists;
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

bool TFiles::Delete ()
{
 // Delete all files contained, returns false if fails

        // Local variables
	String FunctLabel = "TFiles::Delete";
        bool r = true;

        // For all files in Paths
        for (int i=0;i<Paths->Count;i++) {
        
        	CurrFile.Path = Paths->Strings[i];
                r = r && CurrFile.Delete();
                }
        return r;
}


//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

bool TFiles::Copy (String DestinationDir, bool AskOverwrite)
{
 // Copy all files to a directory, returns false if one fails

	// Local variables
        String FunctLabel = "TFiles::Copy";
        bool r = true;

        // For all files in Paths
        for (int i=0;i<Paths->Count;i++) {
        	CurrFile.Path = Paths->Strings[i];
                r = r && CurrFile.Copy( DestinationDir,AskOverwrite );
                }
        return r;
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

bool TFiles::Move (String DestinationDir)
{
 // Move all files to a directory, returns false if one fails

 	// Local variables
        String FunctLabel = "TFiles::Move";
        bool r = true;

        // For all files in Paths
        for (int i=0;i<Paths->Count;i++) {
        	CurrFile.Path = Paths->Strings[i];
                r = r && CurrFile.Move( DestinationDir );
                }
        return r;
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void TFiles::Open ( )
{
 // Acts like a double click on all files
	// Internal variables
        String Names = "";

        // For all files in Paths
        for (int i=0;i<Paths->Count;i++) {
                CurrFile.Path = Paths->Strings[i];
                CurrFile.Open();
                }
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int TFiles::AddSearch (String Dir, String Mask, bool IncludeSubDir)
{
 // Search and Add Files; returns the number of files found
	// Internal variables
        int n=0;
        TSearchRec F;

        // Input Control
        if (Dir[Dir.Length()]!='\\') Dir += "\\";

        // Cerca i files nella directory corrente
        if ( !FindFirst( Dir + Mask , faAnyFile , F) ) {
        	do {
                    AddFile ( Dir + F.Name);
                    n++;
                    }
                while ( !FindNext(F) );
                }
	FindClose(F);

        // Ricorsione nelle subdirectories
	if (IncludeSubDir) {

        TSearchRec D;
        if ( !FindFirst( Dir + "*", faDirectory, D) ) {
        	do {
                    if ( (D.Name != "." && D.Name != ".." && D.Attr&faDirectory) ) {
                    	     AddSearch ( Dir + D.Name + "\\" , Mask , IncludeSubDir );
                    	     }
                    }
                    while ( !FindNext(D) );
                }
        FindClose(D);
    	}// End include SubDirectories
        return n;
}
//---------------------------------------------------------------------------