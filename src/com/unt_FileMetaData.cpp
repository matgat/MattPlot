//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "unt_FileMetaData.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------


//----------------------- Useful file-scope declarations --------------------

	const String UNKNOWN_STRING = "??";


//---------------------------------------------------------------------------
cls_FileMetaData::cls_FileMetaData(const String& path)
{ // Constructor
	RetrieveFrom(path);
}

//---------------------------------------------------------------------------
cls_FileMetaData::~cls_FileMetaData()
{ // Destructor
}

//---------------------------------------------------------------------------
void __fastcall cls_FileMetaData::RetrieveFrom(const String& path)
{ // See a file

    CurrentFile = path;
	if ( !FileExists(CurrentFile) ) return;

	byte* Buffer = 0;
	DWORD handle = 0;
	
	// Figure out how much version info there is:
	DWORD VerSize = GetFileVersionInfoSize(CurrentFile.c_str(),&handle);
	if ( VerSize == 0 ) return; // No informations
 
	Buffer = new byte[VerSize];
 
	if ( !GetFileVersionInfo(CurrentFile.c_str(),handle,VerSize,Buffer) )
	   {
	    ShowMessage("cls_ThisExeFile::RetrieveMetaData Failed to query file version information.");
	    if (Buffer) delete[] Buffer;
	    return;
	   }
 
	DWORD *Langs = 0;
	UINT LangSize = 0;

	// Get the locale info from the version info:
	if ( !VerQueryValue(Buffer,"\\VarFileInfo\\Translation",(void**)&Langs,&LangSize) )
	   {
	    ShowMessage("cls_ThisExeFile::RetrieveMetaData Failed to query locale information.");
	    if (Buffer) delete[] Buffer;
	    return;
	   }

	// Here we create a prefix string that is the same for all the keys.
	UINT Lang = LOWORD(*Langs);
	UINT SubLang = HIWORD(*Langs);
	String KeyPrefix = "\\StringFileInfo\\" + String::IntToHex(Lang,4) + String::IntToHex(SubLang,4) + "\\";

	// Now retrieve datas
	i_FileVersion = GetVersionKey(KeyPrefix+"FileVersion", Buffer);
	i_FileDescription = GetVersionKey(KeyPrefix+"FileDescription", Buffer);
	i_InternalName = GetVersionKey(KeyPrefix+"InternalName", Buffer);
	i_OriginalFilename = GetVersionKey(KeyPrefix+"OriginalFilename", Buffer);
	i_ProductName = GetVersionKey(KeyPrefix+"ProductName", Buffer);
	i_ProductVersion = GetVersionKey(KeyPrefix+"ProductVersion", Buffer);
	i_LegalCopyright = GetVersionKey(KeyPrefix+"LegalCopyright", Buffer);
	i_LegalTrademarks = GetVersionKey(KeyPrefix+"LegalTrademarks", Buffer);
	i_CompanyName = GetVersionKey(KeyPrefix+"CompanyName", Buffer);
	i_Comments = GetVersionKey(KeyPrefix+"Comments", Buffer);

	// Finally
	if (Buffer) delete[] Buffer;
}


//-------------------------------- Properties -------------------------------



//---------------------------------------------------------------------------
String cls_FileMetaData::get_FileVersion() const
{
	return i_FileVersion;
}
void cls_FileMetaData::set_FileVersion(const String& s)
{
	i_FileVersion = s;
}
//---------------------------------------------------------------------------
String cls_FileMetaData::get_FileDescription() const
{
	return i_FileDescription;
}
void cls_FileMetaData::set_FileDescription(const String& s)
{
	i_FileDescription = s;
}
//---------------------------------------------------------------------------
String cls_FileMetaData::get_InternalName() const
{
	return i_InternalName;
}
void cls_FileMetaData::set_InternalName(const String& s)
{
	i_InternalName = s;
}
//---------------------------------------------------------------------------
String cls_FileMetaData::get_OriginalFilename() const
{
	return i_OriginalFilename;
}
void cls_FileMetaData::set_OriginalFilename(const String& s)
{
	i_OriginalFilename = s;
}
//---------------------------------------------------------------------------
String cls_FileMetaData::get_ProductName() const
{
	return i_ProductName;
}
void cls_FileMetaData::set_ProductName(const String& s)
{
	i_ProductName = s;
}
//---------------------------------------------------------------------------
String cls_FileMetaData::get_ProductVersion() const
{
	return i_ProductVersion;
}
void cls_FileMetaData::set_ProductVersion(const String& s)
{
	i_ProductVersion = s;
}
//---------------------------------------------------------------------------
String cls_FileMetaData::get_LegalCopyright() const
{
	return i_LegalCopyright;
}
void cls_FileMetaData::set_LegalCopyright(const String& s)
{
	i_LegalCopyright = s;
}
//---------------------------------------------------------------------------
String cls_FileMetaData::get_LegalTrademarks() const
{
	return i_LegalTrademarks;
}
void cls_FileMetaData::set_LegalTrademarks(const String& s)
{
	i_LegalTrademarks = s;
}
//---------------------------------------------------------------------------
String cls_FileMetaData::get_CompanyName() const
{
	return i_CompanyName;
}
void cls_FileMetaData::set_CompanyName(const String& s)
{
	i_CompanyName = s;
}
//---------------------------------------------------------------------------
String cls_FileMetaData::get_Comments() const
{
	return i_Comments;
}
void cls_FileMetaData::set_Comments(const String& s)
{
	i_Comments = s;
}

//----------------------------- Private methods -----------------------------


//---------------------------------------------------------------------------
String cls_FileMetaData::GetVersionKey (const String &KeyString, byte* buf)
{
        char *data;
        UINT datasize;
        if ( VerQueryValue(buf, KeyString.c_str(),(void **) &data, &datasize) ) return String(data);
        else return UNKNOWN_STRING;
}

//============================= END OF FILE =================================
