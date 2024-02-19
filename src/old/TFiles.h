//---------------------------------------------------------------------------
#ifndef TFilesH
#define TFilesH
//---------------------------------------------------------------------------

	/*******************************************************
	*                                                      *
	*	     Tre classi per gestire i files            *
	*                                                      *
	*               2004 - Matteo Gattanini                *
	*                                                      *
	*******************************************************/

//---------------------------------------------------------------------------

//#include <stdlib.h>    // Standard library (es. random)
#include "TInternalLog.h"

//---------------------------------------------------------------------------

#define UNKNOWN_STRING "??"
#define NAME_BUFF 64    // buffer length for names strings
#define MSG_BUFF 256    // buffer length for messages strings
#define HUGE_BUFF 32768 // buffer length used for long strings

//---------------------------------------------------------------------------




//===========================================================================
//				CLASSI


//===========================================================================
//			TFileVersionInfo - file informations
//      (una classe per leggere le metainformazioni dei files con win32APIs)

class TFileVersionInfo {
public:
	TFileVersionInfo(String Path) : Block(0), Lang(0), SubLang(0)
        {
		 UINT word;
                 DWORD param;
                 //std::auto_ptr<char> path(new char[_MAX_PATH]);
                 //GetModuleFileName(0,path.get(),_MAX_PATH);
                 int size = GetFileVersionInfoSize(Path.c_str(), &param);
                 if (size) {
			    Block =  new char [size] ;
                            DWORD *langcodes = 0; //= LANG_USER_DEFAULT
                            if (GetFileVersionInfo(Path.c_str(),param,size,Block)) {
				if ( !VerQueryValue(Block,"\\VarFileInfo\\Translation",(void * *)&langcodes, &word) ) return;
				if ( !langcodes ) return;
				Lang = LOWORD(*langcodes);
				SubLang = HIWORD(*langcodes);
                                }
                            }
        }

        ~TFileVersionInfo()
        {
		 if (Block) delete [] Block;
        }

//--------------------------------------------

	bool StringQuery(const char *key, char *& text)
        {
            if (!(Block && Lang && SubLang && key && key[0])) return false;
            UINT word;
            char query[255];
            wsprintf(query,"\\StringFileInfo\\%04x%04x\\%s",
            				 Lang, SubLang, key);
            return VerQueryValue(Block,query,(void * *)&text, &word);
        }

protected:
	void *Block;
        UINT Lang;    // language code
        UINT SubLang; // sub language
//--------------------------------------------
}; // end Class TFileVersionInfo definition
//===========================================================================



    
//===========================================================================
//				TFile
// (files operations with win32APIs, some operations are based on Sysutils)

class TFile{
public:
    	TFile () {
        	  Results.CompleteReport = false;
                  Path = NULL;
                  Handle = INVALID_HANDLE_VALUE;
                  ExtendedAttributes = UNKNOWN_STRING;
    	}
//--------------------------------------------
        // Percorso completo del file:
        // è assolutamente necessario definirlo!
    	String Path;
        // Nome del file
    	String Name;
        // Directory che contiene il file
    	String Directory;
        // Estensione del file
    	String Extension;
        // Tipo di file
        String Type;
        // Attributi del file
    	String Attributes;
    	String ExtendedAttributes;
        // TimeStamps del file
        struct sFileDate{
                         String Creation;
                         String LastAccess;
    			 String LastWrite;
                         } Date;
        // Dimensione del file
    	__int64 Size;
        // Numero dei link al file
        DWORD NLinks;
        // Indice univoco del file
        LONGLONG Index;
        // Numero di serie del volume
        DWORD VolumeSerial;
        // Handle del file
    	HANDLE Handle;

        // Meta informazioni nel file
        struct sFileInfo{
                	 String FileVersion;
                	 String FileDescription;
               		 String InternalName;
			 String OriginalFilename;
               		 String ProductName;
               		 String ProductVersion;
               		 String LegalCopyright;
                         String LegalTrademarks;
               		 String Author;
               		 String CompanyName;
                         String Comments;
        		 } Info;

        // FeedBack delle funzioni
       	TInternalLog Results;

//--------------------------------------------

        void GetAll ();
        String GetName ();
        String GetDirectory ();
        String GetExtension ();
        String GetType ();
        String GetLastWrite();
        bool GetDates ();
        __int64 GetSize ();
        String GetAttributes ();
        int SetAttributes (String);
        bool GetVersionInfo ();
        bool GetInfosByHandle ();

	bool Exists ();

        bool Delete ();
        bool Copy (String,bool);
    	bool Move (String);
        void Open ();
// - - - - - - - - - - - - - - - - - - - - - -
	String CustomReport (String);
        String SizeString (__int64);
	HANDLE PickHandle ();
        void FreeHandle ();
        String FileTimeToString (FILETIME);
        String FixStringPath (String);
//--------------------------------------------
}; // end Class TFile definition
//===========================================================================




//===========================================================================
//				TThisApplication
// 		    (information about current application)

class TThisApplication{
public:
    	TThisApplication ();
        //~TThisApplication () {}
//--------------------------------------------

	String Name;
        String Directory;
	String Version;
        String Build;
        String Author;
        String AuthorMail;
        bool ShowBuild;

//--------------------------------------------

        void Init ();
        void CreateAboutBox ();

// - - - - - - - - - - - - - - - - - - - - - -
}; // end Class TThisApplication definition

//===========================================================================
//---------------------------------------------------------------------------
extern TThisApplication* appl;
//---------------------------------------------------------------------------



//===========================================================================
//				TFiles
// 		  (series of files - based on TFile)

class TFiles{
public:
    	TFiles () {
                   Paths = new TStringList;
                   Paths->Clear();
    	}
        ~TFiles () {
                    delete Paths;
    	}
//--------------------------------------------
        // Percorsi ai files
    	TStringList * Paths;
        // Current Single File
 	TFile CurrFile;

//--------------------------------------------

// - - - - - - - - - - - - - - - - - - - - - -
        void AddFile (String);
        void AddFiles (TStringList*);
	String GetNames (String);
        String ReportFiles (String);
	bool Exists ();
        bool ListExistingIn (String,TStringList*);
        void Open ();
        bool Delete ();
    	bool Copy (String,bool);
        bool Move (String);
        int AddSearch (String, String, bool);
// - - - - - - - - - - - - - - - - - - - - - -

        
// - - - - - - - - - - - - - - - - - - - - - -
}; // end Class TFiles definition

//===========================================================================

#endif
