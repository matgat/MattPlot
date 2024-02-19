//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "unt_ThisExeFile.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------


//----------------------- Useful file-scope declarations --------------------

	const String UNKNOWN_STRING = "??";


//------------------------------ Public methods------------------------------

//---------------------------------------------------------------------------
cls_ThisExeFile::cls_ThisExeFile ()
{ // Constructor
	
	// Retrieve application name, version, ecc...
	ExePath = Application->ExeName;
	Directory = ::IncludeTrailingBackslash( ExtractFileDir(ExePath) );
	FileName = ExtractFileName(ExePath);

	// Query for file metadatas
	MetaData.RetrieveFrom(ExePath);

	// Choosing the name
	if ( MetaData.ProductName != UNKNOWN_STRING && MetaData.ProductName != "" )
	      Name = MetaData.ProductName;
	else {
	      if ( MetaData.InternalName != UNKNOWN_STRING && MetaData.InternalName != "" )
	      	    Name = MetaData.InternalName;
	      else {
		    if ( MetaData.OriginalFilename != UNKNOWN_STRING && MetaData.OriginalFilename != "" )
	      	    	  Name = MetaData.OriginalFilename;
		    else {
			      // Se non trovo nulla prendo il nome dell'eseguibile stesso:
 			      Name = FileName;
			      // Tolgo l'estensione:
			      Name.SetLength ( Name.LastDelimiter(".") - 1 );
		    	 }
            }
	      }

	// Getting Author available information
	if ( MetaData.CompanyName != UNKNOWN_STRING && MetaData.CompanyName.Length() > 0 ) Author = MetaData.CompanyName;
	else Author = UNKNOWN_STRING;

	AuthorMail = UNKNOWN_STRING;
    WebSite = UNKNOWN_STRING;

	// Writing the Version string
	Version = MetaData.ProductVersion + " (" + MetaData.FileVersion + ")";
}


//---------------------------------------------------------------------------
void __fastcall cls_ThisExeFile::CreateAboutBox ()
{
 // Create an about box
	String Tit,Msg;

	if ( MetaData.CompanyName == Author ) MetaData.CompanyName = AnsiString::StringOfChar('-',30);

	Tit = "About " + Name;

	Msg = "\n\t" + Name + "\n" +
	      "\t" + Version + "\n\n" +\
	      " Description:\t" + MetaData.FileDescription + "\n" +\
	      "  Comments:\t"   + MetaData.Comments + "\n\n" +
	      "     Author:\t"  + Author + "\n" +\
	      "\t\t" 		+ AuthorMail + "\n\n" +\
	      "      Other:\t"  + MetaData.LegalCopyright + "\n" +\
	      "\t\t"		+ MetaData.LegalTrademarks + "\n";

	// Wrap the text
	//TSysCharSet wrchars;
	//wrchars << '.' << '-' << ' ' << '\n';
	//Msg = WrapText( Msg, "\n", wrchars, 50);

	Application->MessageBox(Msg.c_str(), Tit.c_str(), MB_ICONINFORMATION);
}
//---------------------------------------------------------------------------




//----------------------------- Private methods -----------------------------


//============================== END OF FILE ================================
