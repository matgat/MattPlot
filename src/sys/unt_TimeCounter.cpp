//#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
//#include <mmsystem.h> // Include file for Multimedia API's
//#include <time.h>   // clock()
//#include "unt_TimeValue.h"
#include "unt_TimeCounter.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------


//===================== Useful file scope declarations ======================

//============================= Static members ==============================
#if __BORLANDC__ < 0x560   
  const unsigned __int64 cls_TimeCounter::MaxCount = 18446744073709551616;
#else
  const unsigned __int64 cls_TimeCounter::MaxCount = 0xFFFFFFFFFFFFFFFFLL;
#endif

//============================= Public methods ==============================

//=============================== Operators =================================

//============================== Properties =================================

//================================ Events ===================================

//============================ Private Methods ==============================



//=============================== END OF FILE ===============================
