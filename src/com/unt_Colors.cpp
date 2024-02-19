//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "unt_Colors.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


namespace nms_MatCommon//::::::::::::::::::::::::::::::::::::::::::::::::::::
{
 //============================== Some constants ============================

    const int ChannelBits = 8*sizeof(typ_Channel);
    const typ_Channel MaxChannel = 255;
    const typ_Fractional MaxChannelFr = 255.0;
    const typ_Fractional Max2ChannelFr = 2.0*MaxChannelFr;

 //------------------------------ Predefined colors definitions

    const TColor clAcquamarina  = TColor(0x97D023); // TColor ( RGB( 35,208,151) )
    const TColor clAcquamarinaS = TColor(0xD7FF00);
    const TColor clAlluminio    = TColor(0xD7D6D1); // TColor ( RGB(209,214,215) )
    const TColor clArancio      = TColor(0x65C7FF);
    const TColor clArancioS     = TColor(0x02B0FF); // TColor ( RGB(255,176,  2) )
    const TColor clAzzurro      = TColor(0xF5E171); // TColor ( RGB(113,225,245) )
    const TColor clAzzurroCC    = TColor(0xFFFBF1); // TColor ( RGB(241,251,255) )
    const TColor clBlu          = TColor(0xD73900);
    const TColor clBluC         = TColor(0xFF8080);
    const TColor clBluElettrico = TColor(0xFA0075);
    const TColor clBtnFaceCC    = TColor(0xF8FBFC); // TColor ( RGB(252,251,248) )
    const TColor clBordeaux     = TColor(0x7300AF);
    const TColor clCiano        = TColor(0xC9BD07);
    const TColor clGialloCC     = TColor(0xEFFFFF); // TColor ( RGB(255,255,239) )
    const TColor clGialloS      = TColor(0x08C1D6); // TColor ( RGB(214,193,  8) )
    const TColor clLilla        = TColor(0xFCB8DB); // TColor ( RGB(219,216,252) )
    const TColor clMagenta      = TColor(0xCE00CE);
    const TColor clMarrone      = TColor(0x19659B); // TColor ( RGB(155,101, 25) )
    const TColor clNavyBlue     = TColor(0x73001F);
    const TColor clOroS         = TColor(0x006E84);
    const TColor clPesca        = TColor(0x97D0F1); // TColor ( RGB(241,208,151) )
    const TColor clPescaS       = TColor(0x0052CE);
    const TColor clRame         = TColor(0x89A3FD); // TColor ( RGB(253,163,137) )
    const TColor clRameS        = TColor(0x4D68BE); // TColor ( RGB(190,104, 77) )
    const TColor clRosa         = TColor(0xD595FF);
    const TColor clRosaC        = TColor(0xF3F3FF); // TColor ( RGB(255,243,243) )
    const TColor clRosso        = TColor(0x2221FF);
    const TColor clRossoDE      = TColor(0x6761ED); // TColor ( RGB(237, 97,103) )
    const TColor clSmeraldo     = TColor(0xBCF199); // TColor ( RGB(153,241,188) )
    const TColor clSmeraldoS    = TColor(0xB2DF1F);
    const TColor clTerra        = TColor(0x61597F);
    const TColor clVerde        = TColor(0x00C200);
    const TColor clVerdeDE      = TColor(0x7CD454); // TColor ( RGB( 84,212,124) )
    const TColor clVerdeDECC    = TColor(0xEDF1EB); // TColor ( RGB(235,241,237) )
    const TColor clVerdeCC      = TColor(0xEDFFED); // TColor ( RGB(237,255,237) )
    const TColor clViola        = TColor(0xF141BC); // TColor ( RGB(188, 65,241) )
    const TColor clViolaDE      = TColor(0xBC76A7); // TColor ( RGB(167,118,188) )
    const TColor clViolaS       = TColor(0x5B0074);

/*
 //------------------------------ A color sequence
    const TColor ColorSequence[] =
        { clSmeraldoS,
          clRosso,
          clBlu,
          clVerde,
          clArancio,
          clMagenta,
          clBluC,
          clOroS,
          clCiano,
          clBluElettrico,
          clViola,
          clAcquamarinaS,
          clGialloS,
          clNavyBlue,
          clMarrone,
          clRameS,
          clPescaS,
          clViolaS,
          clBordeaux,
          clTerra,
          clRosa,
          clBlue,
          clRed,
          clGreen,
          clNavy,
          clTeal,
          clOlive,
          clFuchsia,
          clMaroon,
          clAqua,
          clLime,
          clBlack
        };
    const short int ColorSequenceSize = sizeof(ColorSequence)/sizeof(ColorSequence[0]);
*/

}// End namespace nms_MatCommon::::::::::::::::::::::::::::::::::::::::::::::




//================================ Functions ================================


//---------------------------------------------------------------------------
void nms_MatCommon::rgb2hsl(typ_Channel r, typ_Channel g, typ_Channel b, typ_Fractional& h, typ_Fractional& s, typ_Fractional& l)
{ // RGB —> HSL

// . . . Retrieve some rgb quantities
        typ_Channel rgbmin,rgbmax; // minimum and maximum intensity of channels
        if ( r < g )
             {
              if ( g < b ) {rgbmin=r; rgbmax=b;}
              else {
                    rgbmax=g;
                    if ( r < b ) rgbmin=r;
                    else rgbmin=b;
                   }
             }
        else {
              if ( r < b ) {rgbmin=g; rgbmax=b;}
              else {
                    rgbmax=r;
                    if ( g < b ) rgbmin=g;
                    else rgbmin=b;
                   }
             }

        typ_Fractional rgbdiff = typ_Fractional(rgbmax) - typ_Fractional(rgbmin);
        typ_Fractional rgbsum  = typ_Fractional(rgbmax) + typ_Fractional(rgbmin);

        l = rgbsum / Max2ChannelFr; // Luminance

        if ( rgbmax == rgbmin ) h=s=0.0; // gray color
        else {
              typ_Fractional rnorm = (rgbmax - r) / rgbdiff;
              typ_Fractional gnorm = (rgbmax - g) / rgbdiff;
              typ_Fractional bnorm = (rgbmax - b) / rgbdiff;

              s = (l <= 0.5) ? (rgbdiff/rgbsum) : (rgbdiff/(Max2ChannelFr-rgbsum));

              if (r == rgbmax) h = 60.0 * (6.0 + bnorm - gnorm);
              if (g == rgbmax) h = 60.0 * (2.0 + rnorm - bnorm);
              if (b == rgbmax) h = 60.0 * (4.0 + gnorm - rnorm);
              while (h >= 360.0) h -= 360.0;
              //h = fmod( h, 360.0 );
             }
}

  //-------------------------------------------------------------------------
  inline nms_MatCommon::typ_Channel nms_MatCommon::Hue2rgb(typ_Fractional rm1,typ_Fractional rm2,typ_Fractional rh)
  { // Auxiliary for hsl2rgb
          while (rh > 360.0) rh -= 360.0;
          while (rh <   0.0) rh += 360.0;

          if      (rh <  60.0) rm1 = rm1 + (rm2 - rm1) * rh / 60.0;
          else if (rh < 180.0) rm1 = rm2;
          else if (rh < 240.0) rm1 = rm1 + (rm2 - rm1) * (240.0 - rh) / 60.0;

          return static_cast<typ_Channel>(MaxChannelFr * rm1);
  }

//---------------------------------------------------------------------------
void nms_MatCommon::hsl2rgb (typ_Fractional h, typ_Fractional s, typ_Fractional l, typ_Channel& r, typ_Channel& g, typ_Channel& b)
{ // HSL -> RGB
    if (s == 0.0) // Grayscale
         {
          r = g = b = static_cast<typ_Channel>(MaxChannelFr * l);
         }
    else {
          typ_Fractional rm1, rm2;
          if (l <= 0.5) rm2 = l * (1+s);
          else rm2 = l + s - l*s;
          rm1 = 2.0*l - rm2;
          r = Hue2rgb(rm1, rm2, h + 120.0);
          g = Hue2rgb(rm1, rm2, h);
          b = Hue2rgb(rm1, rm2, h - 120.0);
         }
}

//---------------------------------------------------------------------------
// Adjust luminosity
nms_MatCommon::typ_Color nms_MatCommon::Darken (const typ_Color& C, const short d)
{
    typ_Fractional h,s,l;
    rgb2hsl(C,h,s,l);
    l -= typ_Fractional(d)/100.0;
    if (l<0) l = 0;
    else if (l>1) l = 1;
    return hsl2col(h,s,l);
}

//---------------------------------------------------------------------------
// Adjust saturation
nms_MatCommon::typ_Color nms_MatCommon::Desaturate (const typ_Color& C, const short d)
{
    typ_Fractional h,s,l;
    rgb2hsl(C,h,s,l);
    s -= typ_Fractional(d)/100.0;
    if (s<0) s = 0;
    else if (s>1) s = 1;
    return hsl2col(h,s,l);
}

//---------------------------------------------------------------------------
// Adjust Hue
nms_MatCommon::typ_Color nms_MatCommon::Hue (const typ_Color& C, const short d)
{
    typ_Fractional h,s,l;
    rgb2hsl(C,h,s,l);
    h += typ_Fractional(d)/360.0;
    return hsl2col(h,s,l);
}



//================================= Classes =================================



//=============================== END OF FILE ===============================
