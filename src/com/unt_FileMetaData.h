//---------------------------------------------------------------------------
#ifndef unt_FileMetaDataH
#define unt_FileMetaDataH
//---------------------------------------------------------------------------

	/*******************************************************
	*
	*	      cls_FileMetaData
	*       A win32API wrapper to retrieve file metadatas
	*       2005/2006 - Matteo Gattanini
	*
	*	Uso:
	*	String path = "C:\\dir\\file.dll";
	*	cls_FileMetaData mdat(path);
        *       exeinfo.AuthorMail = "mat@mail.com";
	*       ShowMessage(mdat.FileVersion);
	*
	*       cls_FileMetaData* pmdat = new cls_FileMetaData(path);
	*       pmdat->LegalCopyright = "mat"; // writing not yet supported
	*       Memo1->Lines->Add(pmdat->Version);
	*       delete pmdat;
	*       
	*
	*******************************************************/


//---------------------------------------------------------------------------
class cls_FileMetaData {
public://							.............
	cls_FileMetaData(const String& =0);
        ~cls_FileMetaData();

// . . . Other public methods
	void __fastcall RetrieveFrom(const String&);

// . . . Properties
	__property String FileVersion = { read = get_FileVersion, write = set_FileVersion };
	__property String FileDescription = { read = get_FileDescription, write = set_FileDescription };
	__property String InternalName = { read = get_InternalName, write = set_InternalName };
	__property String OriginalFilename = { read = get_OriginalFilename, write = set_OriginalFilename };
	__property String ProductName = { read = get_ProductName, write = set_ProductName };
	__property String ProductVersion = { read = get_ProductVersion, write = set_ProductVersion };
	__property String LegalCopyright = { read = get_LegalCopyright, write = set_LegalCopyright };
	__property String LegalTrademarks = { read = get_LegalTrademarks, write = set_LegalTrademarks };
	__property String CompanyName = { read = get_CompanyName, write = set_CompanyName };
	__property String Comments = { read = get_Comments, write = set_Comments };
  
// . . . Public attributes (!)
	String CurrentFile;

private://							.............

// . . . Private attributes
	String  i_FileVersion,
		i_FileDescription,
		i_InternalName,
		i_OriginalFilename,
		i_ProductName,
		i_ProductVersion,
		i_LegalCopyright,
		i_LegalTrademarks,
		i_CompanyName,
		i_Comments;

// . . . Properties
	String get_FileVersion() const;
	void set_FileVersion(const String&);
	String get_FileDescription() const;
	void set_FileDescription(const String&);
	String get_InternalName() const;
	void set_InternalName(const String&);
	String get_OriginalFilename() const;
	void set_OriginalFilename(const String&);
	String get_ProductName() const;
	void set_ProductName(const String&);
	String get_ProductVersion() const;
	void set_ProductVersion(const String&);
	String get_LegalCopyright() const;
	void set_LegalCopyright(const String&);
	String get_LegalTrademarks() const;
	void set_LegalTrademarks(const String&);
	String get_CompanyName() const;
	void set_CompanyName(const String&);
	String get_Comments() const;
	void set_Comments(const String&);

// . . . Private methods
	String GetVersionKey (const String&, byte*);
	
}; // end Class cls_FileMetaData definition//			.............
//---------------------------------------------------------------------------
#endif
