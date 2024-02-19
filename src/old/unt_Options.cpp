//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "unt_Options.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


//===========================================================================
//                      cls_Options Methods


//---------------------------------------------------------------------------
cls_Options::cls_Options()
{ // Constructor
        ini = 0;
	OnReadData=0;
        OnStoreData=0;
}

//---------------------------------------------------------------------------

String cls_Options::ReadFrom (const String& file)
{
 // Reads the variables from a file
 // Here the default values replacing incorrect or absent entries

        missingsects = missingvals = 0;
        String Result, defstr;
        String filename = ExtractFileName(file);

	ini = new TIniFile( file );
        if (!ini) return "Unable to open " + filename;

	// Call Specific Read Data function
	if ( OnReadData )
             {
              OnReadData();

              if ( missingsects + missingvals )
                   {
                    Result = "Some default substituted in " + filename +\
                             " (missing sections:" + String(missingsects) + \
                             ", missing values:" + String(missingvals) + ")";
                   }
              else Result = "Options readed from " + filename;
             }
        else Result = "No option data to read";

        delete ini;
        ini = 0;

        return Result;
}

//---------------------------------------------------------------------------

String cls_Options::StoreIn(const String& file)
{
 // Writes the variables in a file; returns the result of the operation

        String Result;
        String filename = ExtractFileName(file);

	ini = new TIniFile( file );
        if (!ini) return "Unable to initialize " + filename;

	// Call specific Store Data function
	if ( OnStoreData )
             {
              OnStoreData();
              Result = "Options stored in " + filename;
             }
        else Result = "No option data to store";

	delete ini;
        ini = 0;

        return Result;
}


//===========================================================================
//                      Protected Methods

//---------------------------------------------------------------------------

void cls_Options::Section (String sect)
{
 // Setta la sezione corrente e ne controlla l'esistenza
        section = sect;
        if (ini) {
                  if ( !ini->SectionExists(section) ) missingsects++;
                  }
        else ShowMessage("cls_Options::Section - Attempt to use an unopened ini file");
}

//---------------------------------------------------------------------------
bool cls_Options::Read(String Label, bool DefaultValue)
{ // controllo esistenza e lettura valore dal file - Overloading for `bool´

        if ( !ini->ValueExists(section, Label) ) missingvals++;
        return ini->ReadBool(section, Label, DefaultValue);
}
//---------------------------------------------------------------------------
int cls_Options::Read(String Label, int DefaultValue)
{ // controllo esistenza e lettura valore dal file - Overloading for `int´

        if ( !ini->ValueExists(section, Label) ) missingvals++;
        return ini->ReadInteger(section, Label, DefaultValue);
}
//---------------------------------------------------------------------------
String cls_Options::Read(String Label, String DefaultValue)
{ // controllo esistenza e lettura valore dal file - Overloading for `String´

        if ( !ini->ValueExists(section, Label) ) missingvals++;
        return ini->ReadString(section, Label, DefaultValue);
}
//---------------------------------------------------------------------------
double cls_Options::Read(String Label, double DefaultValue)
{ // controllo esistenza e lettura valore dal file - Overloading for `String´

        tmp = FloatToStr(DefaultValue);
        if ( !ini->ValueExists(section, Label) ) missingvals++;

        double conv;
        try  {
              conv = ( ToDoubleLiteral(ini->ReadString(section, Label, tmp)) ).ToDouble();
             }

        catch(EConvertError&)
             {
              conv = DefaultValue;
             }

        return conv;
}


//---------------------------------------------------------------------------
void cls_Options::Write(String Label, bool Value)
{ // scrittura valore nel file - Overloading for `bool´

        ini->WriteBool(section, Label, Value);
}
//---------------------------------------------------------------------------
void cls_Options::Write(String Label, int Value)
{ // scrittura valore nel file - Overloading for `int´

        ini->WriteInteger(section, Label, Value);
}
//---------------------------------------------------------------------------
void cls_Options::Write(String Label, String Value)
{ // scrittura valore nel file - Overloading for `String´

        ini->WriteString(section, Label, Value);
}
//---------------------------------------------------------------------------
void cls_Options::Write(String Label, double Value)
{ // scrittura valore nel file - Overloading for `String´

        ini->WriteString(section, Label, FloatToStr(Value));
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

String cls_Options::ToDoubleLiteral ( String strIn )
{ // Clean a String for conversion ToDouble

        //  Controlli preliminari
        if ( strIn.IsEmpty() ) return "";

        // Variabili interne di supporto
        String strTmp = ""; // stringa temporanea
        String strOut = ""; // stringa di uscita
        bool DecSepFound = false; // trovato un separatore decimale
	
        // Prima scansione: lascio solo i numeri e conto i separatori decimali

	for (int i=1; i <= strIn.Length(); i++)
	    {
	     // Tengo solo i caratteri 123456789+-E
             if ( (strIn[i]>='0' && strIn[i]<='9') ||
		   strIn[i]=='+' ||
		   strIn[i]=='-' ||
                   strIn[i]=='E' )
		strTmp += strIn[i];
	     // Ed anche i caratteri di separazione decimale, che sostituisco
	     // con il carattere di default DecimalSeparator		
             if (strIn[i] == '.' || strIn[i] == ',') strTmp += DecimalSeparator;
	    }

	// Seconda scansione: elimino separatori e segni in più
	// (mantengo l'ultimo separatore e l'eventuale segno all'inizio della stringa) 
		
	for (int i=1; i <= strTmp.Length(); i++)
		{
                 if (strTmp[i] == '.' || strTmp[i] == ',')
		    {
		     if (DecSepFound) continue; 
		     else
		         {
			  strOut += strTmp[i];
			  DecSepFound = true;
			 }
		    }
		 else if ( strTmp[i] == '+' || strTmp[i] == '-' ) continue;
		    
		 else strOut += strTmp[i];
                }
        return strOut;
}


//===========================================================================
//                      Events

//---------------------------------------------------------------------------
fastvoid cls_Options::ROnReadData ()
{ // Get Handler pointer
        //if ( !iOnReadData )
        return iOnReadData;
}
//---------------------------------------------------------------------------
void cls_Options::WOnReadData (fastvoid F)
{ // Set Handler pointer
        iOnReadData = F;
}
//---------------------------------------------------------------------------
fastvoid cls_Options::ROnStoreData ()
{ // Get Handler pointer
        //if ( !iOnStoreData )
        return iOnStoreData;
}
//---------------------------------------------------------------------------
void cls_Options::WOnStoreData (fastvoid F)
{ // Set Handler pointer
        iOnStoreData = F;
}
//---------------------------------------------------------------------------


//===========================================================================
