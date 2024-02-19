//---------------------------------------------------------------------------

#include <vcl.h>
#include "unt_Log.h"
#include "unt_MatCommon.h"
#pragma hdrstop
#include "unt_OptionValues.h"
#include "unt_IniFile.h"
#include "unt_Options.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


//========================== File scope variables ===========================

//============================= Static Members ==============================

//============================= Public methods ==============================


//---------------------------------------------------------------------------
__fastcall cls_Options::cls_Options(class cls_OptionValue** v) : Values(v)
{ // Constructor

	OnRead=0;
	OnStore=0;
}

//---------------------------------------------------------------------------
//__fastcall cls_Options::~cls_Options()
//{ // Destructor
//}

//---------------------------------------------------------------------------

void __fastcall cls_Options::ReadFromFile (const String& path)
{ // Reads the variables from a file
  // Here the default values replacing incorrect or absent entries

	Log->Entry("Reading options from " + path);

	// Initializations
	MissingSects = MissingVals = 0;


        /*
	// Some checks
	if ( !FileExists(path) )
	   {
	    Log->Error("This file doesn\'t exists: " + path);
	    return;
	   }

	String FileName = ExtractFileName(path);
	ini = new cls_IniFile(path);
	if (!ini)
		{
		 Log->Error("Unable to open " + FileName);
		 return;
		}

        */

	if (OnRead) OnRead(); // Call Specific Read Data functions

        ShowMessage(*Values[0]);

        /*
        for (unsigned int i=0; i<Size; i++)
            {
             ShowMessage(*Values[i]);
            }
        */
            /*
        if ( MissingSects + MissingVals )
		   {
		    Log->SubEntry("Some defaults substituted in " + FileName +\
				  " (missing sections:" + String(MissingSects) + \
				  ", missing values:" + String(MissingVals) + ")");
		   }


        Log->SubEntry("Options read from " + FileName);

        */
        
	// Finally
	//delete ini;
	//ini = 0;
}

//---------------------------------------------------------------------------

void __fastcall cls_Options::StoreToFile( const String& path )
{ // Writes the variables to a file

	Log->Entry("Storing options in " + path);
	String FileName = ExtractFileName(path);

	ini = new cls_IniFile(path);
	if (!ini) Log->Error("Unable to initialize " + FileName);

	if ( OnStore) OnStore();

        Log->SubEntry("Options stored in " + FileName);


	//ini->UpdateFile(); // In case of buffered files

	// Finally
	delete ini;
	ini = 0;
}


/*

//============================= Protected Methods ===========================

//---------------------------------------------------------------------------
void __fastcall cls_Options::SetSection (const String& s)
{ // Setta la sezione corrente e ne controlla l'esistenza

	if ( ini->SectionExists(s) ) CurrentSection = s;
	else {
	      Log->SubEntry("Section `"+s+"´ is missing");
	      MissingSects++;
	     }
}



//------------------------------- Read methods ------------------------------

//---------------------------------------------------------------------------
bool __fastcall cls_Options::Read(const String& Label, const bool& DefaultValue)
{ // Lettura `bool´

	if ( ini->ValueExists(CurrentSection,Label) ) return ini->ReadBool(CurrentSection, Label, DefaultValue);
	else {
	      Log->SubEntry("Boolean `"+Label+"´ in section `"+CurrentSection+"´ is missing, using default ("+mat::BoolToStr(DefaultValue)+")");
	      MissingVals++;
	      return DefaultValue;
	     }
}

//---------------------------------------------------------------------------
int __fastcall cls_Options::Read(const String& Label, const int& DefaultValue)
{ // Lettura `int´

	if ( ini->ValueExists(CurrentSection,Label) ) return ini->ReadInteger(CurrentSection, Label, DefaultValue);
	else {
	      Log->SubEntry("Integer `"+Label+"´ in section `"+CurrentSection+"´ is missing, using default ("+String(DefaultValue)+")");
	      MissingVals++;
	      return DefaultValue;
	     }
}

//---------------------------------------------------------------------------
String __fastcall cls_Options::Read(const String& Label, const String& DefaultValue)
{ // Lettura `String´

	if ( ini->ValueExists(CurrentSection,Label) ) return ini->ReadString(CurrentSection, Label, DefaultValue);
	else {
	      Log->SubEntry("String `"+Label+"´ in section `"+CurrentSection+"´ is missing, using default ("+DefaultValue+")");
	      MissingVals++;
	      return DefaultValue;
	     }
}

//---------------------------------------------------------------------------
double __fastcall cls_Options::Read(const String& Label, const double& DefaultValue)
{ // Lettura `double´

	if ( ini->ValueExists(CurrentSection, Label) )
	     {
	      String StrValue = ini->ReadString(CurrentSection,Label,FloatToStr(DefaultValue));
	      double val;
	      try {val = StrToFloat(StrValue);}
	      catch(EConvertError&)
		  {
		   Log->SubEntry(Label+": invalid float ("+StrValue+"), using default ("+FloatToStr(DefaultValue)+")");
		   val = DefaultValue;
		  }
	      return val;
	     }
	else {
	      Log->SubEntry("Float `"+Label+"´ in section `"+CurrentSection+"´ is missing, using default ("+FloatToStr(DefaultValue)+")");
	      MissingVals++;
	      return DefaultValue;
	     }
}

//---------------------------------------------------------------------------
TShortCut __fastcall cls_Options::Read(const String& Label, const TShortCut& DefaultValue)
{ // Lettura `TShortCut´

	if ( ini->ValueExists(CurrentSection, Label) )
	     {
	      String StrValue = ini->ReadString(CurrentSection,Label,ShortCutToText(DefaultValue));
	      TShortCut val;
	      try {val = TextToShortCut(StrValue);} // example "Ctrl+N"
	      catch(EConvertError&)
		  {
		   Log->SubEntry(Label+": invalid TShortCut ("+StrValue+"), using default ("+ShortCutToText(DefaultValue)+")");
		   val = DefaultValue;
		  }
	      return val;
	     }
	else {
	      Log->SubEntry("TShortCut `"+Label+"´ in section `"+CurrentSection+"´ is missing, using default ("+ShortCutToText(DefaultValue)+")");
	      MissingVals++;
	      return DefaultValue;
	     }
}




//------------------------------- Write methods -----------------------------

//---------------------------------------------------------------------------
void __fastcall cls_Options::Write(const String& Label, const bool& Value)
{ // Scrittura `bool´

	ini->WriteBool(CurrentSection, Label, Value);
}

//---------------------------------------------------------------------------
void __fastcall cls_Options::Write(const String& Label, const int& Value)
{ // Scrittura `int´

	ini->WriteInteger(CurrentSection, Label, Value);
}

//---------------------------------------------------------------------------
void __fastcall cls_Options::Write(const String& Label, const String& Value)
{ // Scrittura `String´

	ini->WriteString(CurrentSection, Label, Value);
}

//---------------------------------------------------------------------------
void __fastcall cls_Options::Write(const String& Label, const double& Value)
{ // Scrittura `double´

	ini->WriteString(CurrentSection, Label, FloatToStr(Value));
}

//---------------------------------------------------------------------------
void __fastcall cls_Options::Write(const String& Label, const TShortCut& Value)
{ // Scrittura `TShortCut´

	ini->WriteString(CurrentSection, Label, ShortCutToText(Value));
}

*/


//=============================== END OF FILE ===============================
