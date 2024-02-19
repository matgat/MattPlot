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
//#define OPT_DEBUG
// TODO 5: void cls_Options::LoadDefaults(); // Reload defaults


//============================= Static Members ==============================
class cls_OptionValue*const cls_Options::SKIP_ADDR = reinterpret_cast<cls_OptionValue*>(-1);


//============================= Public methods ==============================


//---------------------------------------------------------------------------
__fastcall cls_Options::cls_Options(cls_Log* l) : Log(l)
{ // Constructor

    OnRead = 0;
    OnStore = 0;
}

//---------------------------------------------------------------------------
//__fastcall cls_Options::~cls_Options()
//{ // Destructor
//}

//---------------------------------------------------------------------------
void cls_Options::ReadFromIniFile (class cls_OptionValue** vls,const String& path)
{ // Reads the variables from a file
  // Here the default values replacing incorrect or absent entries

    // Feedback
    String FName = ExtractFileName(path);
    Log->Entry("I/O","Reading options from " + FName); Log->StartTime(2);

    // Open ini file
    cls_IniFile *ini;
    try {
         ini = new cls_IniFile(Log,path,true);

          // Perform read
          for ( unsigned int i=0; vls[i]!=0; i++ ) // For all values
              {
               if ( vls[i] == SKIP_ADDR ) continue;
               if ( ini->ValueExists(vls[i]->Group,vls[i]->Label) )
                    { // Found
                     String ReadValue;
                     try {
                          ReadValue = ini->Get(vls[i]->Group,vls[i]->Label);
                          *vls[i] = ReadValue;

                          #ifdef OPT_DEBUG
                             Log->SubEntry(vls[i]->Label +" = "+ *vls[i],1);
                          #endif
                         }
                     catch(EConvertError&)
                         {
                          Log->Warn(vls[i]->Label+": invalid " +vls[i]->Type+ " `" +ReadValue+ "´, using default: `" +vls[i]->GetDefault()+ "´");
                          vls[i]->SetDefault();
                         }
                    }
               else { // Not found
                     Log->Warn("Value `" +vls[i]->Label+ "´ in section `" +vls[i]->Group+ "´ is missing, using default: `" +vls[i]->GetDefault()+ "´");
                     vls[i]->SetDefault();
                    }
              }

          // OnRead event call
          if (OnRead) OnRead();

          // Feedback
          Log->SubEntry("Options parsed in " + Log->GetTime(2));
        }
    __finally {delete ini;}
}

//---------------------------------------------------------------------------
void cls_Options::StoreToIniFile(class cls_OptionValue** vls, const String& path )
{ // Writes the variables to a file

    // Feedback
    String FName = ExtractFileName(path);
    Log->Entry("I/O","Storing options in " + FName);

    // OnStore event call
    if (OnStore) OnStore();

    // Open ini file
    cls_IniFile *ini = 0;
    try {
         ini = new cls_IniFile(Log,path,false);

          // Perform write
          for ( unsigned int i=0; vls[i]!=0; i++ ) // For all values
              {
               if ( vls[i] == SKIP_ADDR ) continue;
               try {
                    ini->Set(vls[i]->Group, vls[i]->Label, (*vls[i]).StringRep(), true, vls[i]->Comment, vls[i]->GetDefault());
                    #ifdef OPT_DEBUG
                      if ( !ini->Set(vls[i]->Group, vls[i]->Label, (*vls[i]).StringRep(), true, vls[i]->Comment, vls[i]->GetDefault()); )
                      Log->Warn("Value `"+ vls[i]->Label +"´ not written");
                    #endif
                   }
               catch(EConvertError&)
                   { // Wow, failed a to-string conversion??
                    String Msg = vls[i]->Label+": failed conversion to String! Resetting to default: `" +vls[i]->GetDefault()+ "´";
                    Log->Error(Msg);
                    mat::MsgError(Msg);
                    vls[i]->SetDefault();
                    ini->Set(vls[i]->Group, vls[i]->Label, *vls[i]);
                   }
              }

          // Now write buffer to file
          ini->Write();

          // Feedback
          Log->SubEntry("Options stored in " + FName);
        }
    __finally { delete ini; }
}

//---------------------------------------------------------------------------
void cls_Options::SetDefaults (class cls_OptionValue** vls)
{ // Set all options to their default values

// . . . Perform task
    for ( unsigned int i=0; vls[i]!=0; i++ ) // For all values
        {
         if ( vls[i] != SKIP_ADDR ) vls[i]->SetDefault();
        }
}


//============================= Protected Methods ===========================


//=============================== END OF FILE ===============================
