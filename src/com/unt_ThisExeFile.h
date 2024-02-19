//---------------------------------------------------------------------------
#ifndef unt_ThisExeFileH
#define unt_ThisExeFileH
//---------------------------------------------------------------------------
#include "unt_FileMetaData.h"

	/*******************************************************
	*
	*	cls_ThisExeFile  v0.1
	*       information about current application
	*	2005/2006 - Matteo Gattanini
	*
	*	Uso:
	*	cls_ThisExeFile exeinfo;
	*       exeinfo.AuthorMail = "mat@mail.com";
	*       ShowMessage(exeinfo.Version);
	*       exeinfo.CreateAboutBox ();
	*
	*       ThisExe = new cls_ThisExeFile;
	*       ThisExe->AuthorMail = "mat@mail.com";
	*       Memo1->Lines->Add(ThisExe->Version);
	*       ThisExe->CreateAboutBox ();
	*       delete ThisExe;
	*       
	*
	*******************************************************/


//---------------------------------------------------------------------------
class cls_ThisExeFile{

public://							.............
    cls_ThisExeFile ();
    //~cls_ThisExeFile () {}

// . . . Other public methods
	void __fastcall CreateAboutBox ();
	
// . . . Public attributes (!)
	String ExePath;
	String Directory;
	String FileName;
	String Name;
	String Version;
	String Author;
	String Date;
	String AuthorMail;
    String WebSite;
	cls_FileMetaData MetaData;

private://							.............

// . . . Private attributes

// . . . Private methods

}; // end Class cls_ThisExeFile definition
//---------------------------------------------------------------------------
//extern cls_ThisExeFile* ThisExe;
//---------------------------------------------------------------------------

//===========================================================================

#endif
