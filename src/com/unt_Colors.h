//---------------------------------------------------------------------------
#ifndef unt_ColorsH
#define unt_ColorsH
//---------------------------------------------------------------------------
#include <Graphics.hpp>

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * *
    *
    *   unt_Colors ver: 0.3   (oct.2008)
    *   2008 - Matteo Gattanini
    *
    *   This unit contains some predefined colors,
    *   some functions and a color class
    *
    * * * * * * * * * * * * * * * * * * * * * * * * * * * */


//:::::::::::::::::::::::::::: Namespace content ::::::::::::::::::::::::::::
namespace nms_MatCommon
{
 //------------------------------ Macros

 //------------------------------ Types definition
    typedef unsigned int   typ_Color;
    typedef unsigned char  typ_Channel;
    typedef double         typ_Fractional;

 //------------------------------ Constants
    extern const int ChannelBits;
    extern const typ_Channel MaxChannel;
    extern const typ_Fractional MaxChannelFr;
    extern const typ_Fractional Max2ChannelFr;

 //------------------------------ Predefined colors
    extern const TColor clAcquamarina;
    extern const TColor clAcquamarinaS;
    extern const TColor clAlluminio;
    extern const TColor clArancio;
    extern const TColor clArancioS;
    extern const TColor clAzzurro;
    extern const TColor clAzzurroCC;
    extern const TColor clBlu;
    extern const TColor clBluC;
    extern const TColor clBluElettrico;
    extern const TColor clBtnFaceCC;
    extern const TColor clBordeaux;
    extern const TColor clCiano;
    extern const TColor clGialloCC;
    extern const TColor clGialloS;
    extern const TColor clLilla;
    extern const TColor clMagenta;
    extern const TColor clMarrone;
    extern const TColor clNavyBlue;
    extern const TColor clOroS;
    extern const TColor clPesca;
    extern const TColor clPescaS;
    extern const TColor clRame;
    extern const TColor clRameS;
    extern const TColor clRosa;
    extern const TColor clRosaC;
    extern const TColor clRosso;
    extern const TColor clRossoDE;
    extern const TColor clSmeraldo;
    extern const TColor clSmeraldoS;
    extern const TColor clTerra;
    extern const TColor clVerde;
    extern const TColor clVerdeDE;
    extern const TColor clVerdeDECC;
    extern const TColor clVerdeCC;
    extern const TColor clViola;
    extern const TColor clViolaDE;
    extern const TColor clViolaS;

 //------------------------------ 
    //extern const TColor ColorSequence[];
        //extern const short int ColorSequenceSize;

 //------------------------------ Auxiliary functions
        inline typ_Channel Hue2rgb(typ_Fractional,typ_Fractional,typ_Fractional); // Auxiliary for hsl2rgb

 //------------------------------ Functions
        inline typ_Channel GetR(typ_Color c){return typ_Channel(c&0xFF);} // Get Red value of a RGB color
        inline typ_Channel GetG(typ_Color c){return typ_Channel((c>>ChannelBits)&0xFF);} // Get Green value of a RGB color
        inline typ_Channel GetB(typ_Color c){return typ_Channel((c>>(2*ChannelBits))&0xFF);} // Get Blue value of a RGB color
        inline typ_Color rgb2col(typ_Channel r,typ_Channel g,typ_Channel b) {return typ_Color(r)|(typ_Color(g)<<ChannelBits)|(typ_Color(b)<<(2*ChannelBits));}
        void rgb2hsl (typ_Channel r,typ_Channel g,typ_Channel b,typ_Fractional& h,typ_Fractional& s,typ_Fractional& l);
        inline void rgb2hsl (typ_Channel* rgb, typ_Fractional* hsl) {rgb2hsl(rgb[0],rgb[1],rgb[2],hsl[0],hsl[1],hsl[2]);}
        inline void rgb2hsl(const typ_Color c, typ_Fractional& h, typ_Fractional& s, typ_Fractional& l) {rgb2hsl(GetR(c),GetG(c),GetB(c),h,s,l);}
        void hsl2rgb (typ_Fractional h,typ_Fractional s,typ_Fractional l,typ_Channel& r,typ_Channel& g,typ_Channel& b);
        inline typ_Color hsl2col(typ_Fractional h,typ_Fractional s,typ_Fractional l) {typ_Channel r,g,b; hsl2rgb(h,s,l,r,g,b); return rgb2col(r,g,b);}
        //-------------------------------------------------------------------
        typ_Color Darken (const typ_Color& C, const short d); // Adjust luminosity
        typ_Color Desaturate (const typ_Color& C, const short d);// Adjust saturation
        typ_Color Hue (const typ_Color& C, const short d); // Adjust Hue
        typ_Color Invert (const typ_Color& C) {return rgb2col(MaxChannel-GetR(C),MaxChannel-GetG(C),MaxChannel-GetB(C));} // Invert color


 //------------------------------ Classes

        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        class cls_Color// A color class
        {
         public://                                              ~~~~~~~~~~~~~

        // . . . Public methods
        cls_Color() { Color=0; } // Costruttore di default
        cls_Color( cls_Color& cl ) { Color=cl.Color; } // Costruttore di copia
        cls_Color( TColor c ) { Color=static_cast<typ_Color>(c); } // Costruttore di copia
        cls_Color(typ_Color c) : Color(c) {}
        cls_Color(typ_Channel r, typ_Channel g, typ_Channel b) { Color=rgb2col(r,g,b); }
        cls_Color(typ_Fractional h, typ_Fractional s, typ_Fractional l) { Color=hsl2col(h,s,l); }

        //cls_Color& Darken (short a) {L -= typ_Fractional(a)/100.0; return *this;}
        String GetRGBString() const {return "("+String(R)+","+String(G)+","+String(B)+")";}

        // . . . Operators
         template<typename T> cls_Color& operator= ( T C ) { Color=typ_Color(C); return *this; }  // assegnamento di `TColor'
         cls_Color& operator= ( TColor C ) { Color=typ_Color(C); return *this; }  // assegnamento di `TColor'
         operator TColor() const {return static_cast<TColor>(Color);} // Cast to `TColor'
         operator String() const {return IntToHex(int(Color),8);}

        // . . . Properties
         __property typ_Color Color = { read=i_Color, write=i_Color };
         __property typ_Channel R = { read=get_R, write=set_R };
         __property typ_Channel G = { read=get_G, write=set_G };
         __property typ_Channel B = { read=get_B, write=set_B };
         __property typ_Fractional H = { read=get_H, write=set_H };
         __property typ_Fractional S = { read=get_S, write=set_S };
         __property typ_Fractional L = { read=get_L, write=set_L };

        // . . . Public attributes (!)

         private://                                             ~~~~~~~~~~~~~

        // . . . Private attributes
         // typ_Channel R,G,B;
         typ_Color i_Color; // Most significative byte is not used
         mutable typ_Fractional hue;  // 0.0 .. 360.0  (Angle)
         mutable typ_Fractional sat;  // 0.0 .. 1.0    (Percent)
         mutable typ_Fractional lum;  // 0.0 .. 1.0    (Percent)
        // . . . Private methods
        // . . . Properties
         // Red Green Blue
         typ_Channel get_R() const {return GetR(Color);}
         void set_R(typ_Channel r) {Color&=0xFFFFFF00; Color+=r;}
         typ_Channel get_G() const {return GetG(Color);}
         void set_G(typ_Channel g) {Color&=0xFFFF00FF; Color+=typ_Color(g)<<ChannelBits;}
         typ_Channel get_B() const {return GetB(Color);}
         void set_B(typ_Channel b) {Color&=0xFF00FFFF; Color+=typ_Color(b)<<(2*ChannelBits);}
         // Hue Saturation Luminance
         typ_Fractional get_H() const { rgb2hsl(Color,hue,sat,lum); return hue; }
         void set_H (typ_Fractional h) {rgb2hsl(Color,hue,sat,lum); Color=hsl2col(h,sat,lum);}
         typ_Fractional get_S() const { rgb2hsl(Color,hue,sat,lum); return sat; }
         void set_S (typ_Fractional s) {rgb2hsl(Color,hue,sat,lum); Color=hsl2col(hue,s,lum);}
         typ_Fractional get_L() const { rgb2hsl(Color,hue,sat,lum); return lum; }
         void set_L (typ_Fractional l) {rgb2hsl(Color,hue,sat,lum); Color=hsl2col(hue,sat,l);}

        };//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

}//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
namespace mcl = nms_MatCommon; // a short alias for this namespace

#endif//=====================================================================
