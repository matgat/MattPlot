//---------------------------------------------------------------------------
#ifndef unt_CreateCtrlsH
#define unt_CreateCtrlsH

//:::::::::::::::::::::::::::: Namespace content ::::::::::::::::::::::::::::
namespace nms_CreateCtrls
{
//---------------------------------------------------------------------------
// Common functions
void CommonSettings (TControl* ctr, TWinControl* par, const TRect& rct); // Common settings
void Position ( TControl* ctr, TControl* sib, int sep, int wdt ); // Position control

//---------------------------------------------------------------------------
// Create a control at position
template<typename T> T* CreateControl ( TWinControl* par, const TRect& rct =TRect(0,0,0,0)) { T* ctr = new T(par); CommonSettings (ctr, par, rct ); return ctr; }

//---------------------------------------------------------------------------
// Create a control below or beside if 'wdt' is specified
template<typename T> T* CreateControl ( TControl* sib, int sep, int wdt =-1) { T* ctr = CreateControl<T>(sib->Parent); Position( ctr, sib, sep, wdt ); return ctr; }

}//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
namespace ccl = nms_CreateCtrls; // a short alias for this namespace


#endif
