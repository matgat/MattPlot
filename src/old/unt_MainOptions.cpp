//---------------------------------------------------------------------------
#include <vcl.h>
#include "unt_Log.h"
#include "unt_MatCommon.h"
#pragma hdrstop

#include "unt_OptionValues.h"
#include "unt_MainOptions.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//========================== File scope variables ===========================
cls_OptionValue* values[] =
       {
        &cls_OptionBool("aasect","aalabel","aacomment",&PlaySound,true),
        &cls_OptionBool("aasect2","aalabel2","aacomment2",&PlaySound,false)
       }

//============================= Static Members ==============================

//============================= Public methods ==============================


//---------------------------------------------------------------------------
__fastcall cls_MainOptions::cls_MainOptions() : cls_IniOptions(values)
{ // Constructor

        //Values[OPT_PLAYSOUND] = new cls_OptionBool("aasect","aalabel","aacomment",&PlaySound,true);
        //Values[OPT_BORDER] = new cls_OptionBool("aasect","aalabel","aacomment",&PlaySound,true);

        ShowMessage(*Values[0]);
        ShowMessage(Values[1]->Default());
}

//---------------------------------------------------------------------------
//__fastcall cls_MainOptions::~cls_MainOptions ()
//{ // Destructor
//}

//============================ Protected Methods ============================


//=============================== END OF FILE ===============================