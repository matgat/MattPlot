//---------------------------------------------------------------------------

//#include <vcl.h>
//#pragma hdrstop

#include "unt_App_settings.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

cls_App_settings* opts = 0;  // global pointer

//---------------------------------------------------------------------------
cls_App_settings::cls_App_settings()
{ // Constructor
        OnReadData = ReadData;
        OnStoreData = StoreData;

        // Initial default option values

        dt = 200;
        DragEnabled = false;
        ChooseMovement = false;
}

//---------------------------------------------------------------------------

void __fastcall cls_App_settings::ReadData ()
{ // Datas to read

//. . . [general]
        Section ("general");

        // velocity-position request interval [ms]
        dt = Read("dt", 200);

        DragEnabled = Read("DragEnabled", 0);
        ChooseMovement = Read("ChooseMovement", 0);
}

//---------------------------------------------------------------------------

void __fastcall cls_App_settings::StoreData()
{ // Datas to write

//. . . [general]
        Section ("general");

        // velocity-position request interval [ms]
        Write("dt", dt);
        Write("DragEnabled", DragEnabled);
        Write("ChooseMovement", ChooseMovement);
}

//---------------------------------------------------------------------------