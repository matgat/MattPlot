//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "unt_CreateCtrls.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)



//---------------------------------------------------------------------------
void nms_CreateCtrls::CommonSettings (TControl* ctr, TWinControl* par, const TRect& rct )
{ // Common settings
    ctr->Parent = par;
    //ctr->AutoSize = true;
    if ( rct != TRect(0,0,0,0) )
       {
        ctr->Top = rct.top;
        ctr->Left = rct.left;
        ctr->Width = rct.Width();
        if ( rct.Height() > 0 ) ctr->Height = rct.Height();
       }
}

//---------------------------------------------------------------------------
void nms_CreateCtrls::Position ( TControl* ctr, TControl* sib, int sep, int wdt )
{ // Position control

    if ( dynamic_cast<TShape*>(ctr) ) ctr->Height = 2 * sep;
    if ( wdt > 0 )
         { // Beside
          ctr->Top = sib->Top + sib->Height/2 - ctr->Height/2;
          if ( !dynamic_cast<TLabel*>(ctr) ) ctr->Width = wdt;
          if ( sep >= 0 )
               { // Right
                ctr->Left = sib->Left + sib->Width + sep;
               }
          else { // Left
                ctr->Left = sib->Left - ctr->Width + sep;
               }
         }
    else { // Above/Below
          ctr->Left = sib->Left;
          ctr->Width = sib->Width;
          if ( sep >= 0 )
               { // Below
                ctr->Top = sib->Top + sib->Height + sep;
               }
          else { // Above
                ctr->Top = sib->Top - ctr->Height + sep;
               }
         }
}

//---------------------------------------------------------------------------



//=============================== End of File ===============================
