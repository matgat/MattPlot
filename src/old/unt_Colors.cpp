//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "unt_Colors.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//using namespace nms_Colors;

//===========================================================================
//                       Functions of namespace


//---------------------------------------------------------------------------
TColor nms_Colors::Darken(const TColor& C, const float a)
{
        float h,s,l;
        RGB2HSL(C,h,s,l);
        l -= a;
        return HSL2RGB(h,s,l);
}

//---------------------------------------------------------------------------
TColor nms_Colors::Vary(const TColor& C, const int A)
{
        BYTE R,G,B;
        R = GetRValue(C);
        G = GetGValue(C);
        B = GetBValue(C);

        if  ( R > G )
            {
             if ( B > R )
                {
                 // B is dominant
                 B += A;
                 R += A/2;
                 G += A/2;
                }
             else
                {
                 // R is dominant
                 R += A;
                 B += A/2;
                 G += A/2;
                }
            }
        else if ( G > B )
            {
             // G is dominant
             G += A;
             R += A/2;
             B += A/2;
            }
        else
            {
             // B is dominant
             B += A;
             R += A/2;
             G += A/2;
            }

        return TColor( RGB(R,G,B) );
}


//---------------------------------------------------------------------------

void nms_Colors::RGB2HSL(const TColor& C, float& h, float& s, float& l)
{ // RGB —> HSL

/*
    float r, g, b, delta;
    float colorMax, colorMin;
    float h=0, s=0, v=0;
    float4 hsv=0;
    r = color[0];
    g = color[1];
    b = color[2];

    colorMax = max (r,g);
    colorMax = max (colorMax,b);
    colorMin = min (r,g);
    colorMin = min (colorMin,b);
    v = colorMax; // this is value
    if (colorMax != 0)
	{
	s = (colorMax - colorMin) / colorMax;
	}
    if (s != 0) // if not achromatic
	{
	delta = colorMax - colorMin;
	if (r == colorMax)
	    {
	    h = (g-b)/delta;
	    }
	else if (g == colorMax)
	    {
	    h = 2.0 + (b-r) / delta;
	    }
	else // b is max
	    {
	    h = 4.0 + (r-g)/delta;
	    }
	h *= 60;
	if( h < 0)
	    {
	    h +=360;
	    }
	hsv[0] = h / 360.0; // moving h to be between 0 and 1.
	hsv[1] = s;
	hsv[2] = v;
	}
    return hsv;
*/

        // Get RGB normalized values
        float r = GetRValue(C) / 255.0;
        float g = GetGValue(C) / 255.0;
        float b = GetBValue(C) / 255.0;

        // max, min and range value of RGB
        float RGBmin = min( min( r, g ) , b );
        float RGBmax = max( min( r, g ) , b );
        float RGBrange = RGBmax - RGBmin;

        // Luminance
        l = ( RGBmax + RGBmin ) / 2;

        if ( RGBrange == 0 ) // Gray
           {
            s = h = 0;
           }
        else  // Chromatic
           {
            if ( l < 0.5 ) s = RGBrange / ( RGBmax + RGBmin );
            else           s = RGBrange / ( 2 - RGBmax - RGBmin );

            float Rrange = ( ( ( RGBmax - r ) / 6 ) + ( RGBrange / 2 ) ) / RGBrange;
            float Grange = ( ( ( RGBmax - g ) / 6 ) + ( RGBrange / 2 ) ) / RGBrange;
            float Brange = ( ( ( RGBmax - b ) / 6 ) + ( RGBrange / 2 ) ) / RGBrange;

            if ( r == RGBmax ) h = Brange - Grange;
            else if ( g == RGBmax ) h = ( 1 / 3 ) + Rrange - Brange;
            else if ( b == RGBmax ) h = ( 2 / 3 ) + Grange - Rrange;

            if ( h < 0 ) h += 1;
            if ( h > 1 ) h -= 1;
           }
}

//---------------------------------------------------------------------------
TColor nms_Colors::HSL2RGB( float h, float s, float l )
{ // HSL -> RGB

/*
    float4 color=0;
    float f,p,q,t;
    float h,s,v;
    float r=0,g=0,b=0;
    float i;
    if (hsv[1] == 0)
	{
	if (hsv[2] != 0)
	    {
	    color = hsv[2];
	    }
	}
    else
    {
	h = hsv.x * 360.0;
	s = hsv.y;
	v = hsv.z;
	if (h == 360.0)
	    {
	    h=0;
	    }
	h /=60;
	i = floor (h);
	f = h-i;
	p = v * (1.0 - s);
	q = v * (1.0 - (s * f));
	t = v * (1.0 - (s * (1.0 -f)));
	if (i == 0)
	    {
	    r = v;
	    g = t;
	    b = p;
	    }
	else if (i == 1)
	    {
	    r = q;
	    g = v;
	    b = p;
	    }
	else if (i == 2)
	    {
	    r = p;
	    g = v;
	    b = t;
	    }
	else if (i == 3)
	    {
	    r = p;
	    g = q;
	    b = v;
	    }
	else if (i == 4)
	    {
	    r = t;
	    g = p;
	    b = v;
	    }
	else if (i == 5)
	    {
	    r = v;
	    g = p;
	    b = q;
	    }
	color.r = r;
	color.g = g;
	color.b = b;
    }
    return color;
*/

        BYTE R, G, B;
        float tmp1,tmp2;

        if ( s == 0 )
           {
            R = l * 255;
            G = l * 255;
            B = l * 255;
           }
        else
           {
            if ( l < 0.5 ) tmp2 = l * ( 1 + s );
            else           tmp2 = ( l + s ) - ( s * l );

            tmp1 = 2 * l - tmp2;

            R = 255 * Hue2RGB( tmp1, tmp2, h + ( 1 / 3 ) );
            G = 255 * Hue2RGB( tmp1, tmp2, h );
            B = 255 * Hue2RGB( tmp1, tmp2, h - ( 1 / 3 ) );
           }
        return TColor( RGB( R, G, B ) );
}

//---------------------------------------------------------------------------
float nms_Colors::Hue2RGB( float v1, float v2, float vH )
{
        if ( vH < 0 ) vH += 1;
        if ( vH > 1 ) vH -= 1;
        if ( ( 6 * vH ) < 1 ) return ( v1 + ( v2 - v1 ) * 6 * vH );
        if ( ( 2 * vH ) < 1 ) return ( v2 );
        if ( ( 3 * vH ) < 2 ) return ( v1 + ( v2 - v1 ) * ( ( 2 / 3 ) - vH ) * 6 );
        return ( v1 );
}

//---------------------------------------------------------------------------




//===========================================================================
//                       Constructor, Destructor

//---------------------------------------------------------------------------
nms_Colors::cls_Color::cls_Color() { Color = 0; }
nms_Colors::cls_Color::cls_Color( TColor C ) { Color = C; }
nms_Colors::cls_Color::cls_Color(BYTE r,BYTE g,BYTE b) { Color = TColor(RGB(r,g,b)); }

//---------------------------------------------------------------------------
//cls_Color::~cls_Color()
//{ // Class destructor
//        //
//}
//---------------------------------------------------------------------------


//===========================================================================
//                              Public methods

//---------------------------------------------------------------------------

TColor nms_Colors::cls_Color::Darken ( short amount )
{ // Darken color
        R += amount;
        G += amount;
        B += amount;

        return Color = TColor(RGB(R,G,B));
}

//---------------------------------------------------------------------------


//===========================================================================
//                              Operators

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------


//===========================================================================
//                              Properties

//---------------------------------------------------------------------------
TColor nms_Colors::cls_Color::RColor () const
{ // Get
        return iColor;
}

//---------------------------------------------------------------------------
void nms_Colors::cls_Color::WColor (TColor newColor)
{ // Set
		
	iColor = newColor;
        R = GetRValue(newColor);
        G = GetBValue(newColor);
        B = GetGValue(newColor);
}

//---------------------------------------------------------------------------




//===========================================================================
//                              Events

//---------------------------------------------------------------------------



//===========================================================================
//                              Private methods

//---------------------------------------------------------------------------
