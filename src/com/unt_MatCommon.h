//---------------------------------------------------------------------------
#ifndef unt_MatCommonH
#define unt_MatCommonH
//---------------------------------------------------------------------------

        /* * * * * * * * * * * * * * * * * * * * * * * * * * * *
        *
        *    unt_MatCommon
        *    2009 - matteo.gattanini@gmail.com
        *
        *    Overview
        *    --------------------------------------------
        *    This unit contains common types and
        *    service functions. It heavily uses
        *    Borland's 'SysUtils' and <vcl.h>
        *
        *    Licenses
        *    --------------------------------------------
        *    Use and modify freely
        *
        *    Release history
        *    --------------------------------------------
        *    0.6  (nov.2009)
        *        .Removed some classes, changed some functs
        *    0.53 (apr.2009)
        *        .Removed some containers
        *    0.51 (mar.2009)
        *        .'cls_Scope' and some other classes
        *
        *    Example of usage:
        *    --------------------------------------------
        *    #include "unt_MatCommon.h"
        *    //using namespace mat
        *    double MM = mat::Huge;
        *
        * * * * * * * * * * * * * * * * * * * * * * * * * * * */

//---------------------------------------------------------------------------
//#include <fastmath> // no error checking
#include <cmath> // math in inlined functions
#include <cfloat> // _isnan
#include <vector> // standard vector container
#include <Forms.hpp> // 'TApplication'
#include <Dialogs.hpp> // 'ShowMessage'
#include <Classes.hpp> // VCL classes
#include <Graphics.hpp> // 'TBitmap' etc...
//#include <Math.hpp> // VCL math definitions
#include "unt_Colors.h" // My color definitions
//#include <cassert> // assert()
//---------------------------------------------------------------------------

//------------------------ Common Macros definition -------------------------

    // Debug: ASSERTING(int test,char *errorMessage)
    #ifdef _DEBUG
      #define DEBUGLOG(x) ::OutputDebugString((x).c_str());
      #define ASSERTING(test,msg) {if(!(test)){static bool skip=false; if(!skip){mat::HandleAsserting(#test,#msg,__FILE__,__LINE__,__FUNC__,skip);}}};
    #else
      #define DEBUGLOG(x)
      #define ASSERTING(test,msg)
    #endif
    
    //#define MS_BUFFER_SIZE 32767

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Fundamental types renomination (!)

        typedef unsigned int        uint;
        //typedef char                TChar;
        //typedef AnsiString          TString;

        // . . . Implementation-dependent names
        typedef char                I8;
        typedef unsigned char       U8;
        typedef short int           I16;
        typedef unsigned short int  U16;
        typedef long int            I32;
        typedef unsigned long int   U32;
        typedef __int64             I64;
        typedef unsigned __int64    U64;
        typedef float               F32;
        typedef double              F64;

        #if __BORLANDC__ < 0x560
        typedef unsigned char       UINT8;
        typedef char                INT8;
        typedef unsigned short int  UINT16;
        typedef short int           INT16;
        typedef unsigned int        UINT32;
        typedef int                 INT32;
        #endif

        typedef unsigned char       T_BYTE;
        typedef unsigned short int  T_WORD;
        typedef unsigned long int   T_DWORD;
        typedef unsigned __int64    T_QWORD;
        typedef std::vector<T_WORD>  T_WORDS;
        typedef std::vector<T_DWORD> T_DWORDS;

//:::::::::::::::::::::::::::: Namespace content ::::::::::::::::::::::::::::
namespace nms_MatCommon
{

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Types definition

        typedef std::vector<String>       stringlist;
        typedef std::vector<double>       numlist;
        //typedef std::valarray<double> numlist;
        typedef std::vector<int>          intlist;

        // Pointer to functions/members
        typedef void __fastcall (__closure *typ_FnFastVoid)(void); // A void function
        typedef void __fastcall (__closure *typ_FnProgress)(unsigned int,bool&); // A progress function


//---------------------------- Common constants
        // . . . Boolean

        // . . . Characters
        extern const char InvalidChar;
        const char AsciiCtrlChar_Size = 32;
        extern const char AsciiCtrlCharStr[AsciiCtrlChar_Size][4];

        // . . . Numerics
        extern const int MaxInt; // Maximum integer value
        extern const int MinInt; // Minimum integer value
        extern const double SupDouble; // Higher double value
        extern const double InfDouble; // Lower double value
        extern const double Huge; // Huge value
        extern const double eps; // infinitesimal value
        extern const double NaN; // Not a Number (quiet) NAN = 0x7FFFFFFFFFFFFFFF;
        extern const double e; // Euler constant
        extern const double pi; // pigreco
        extern const double duepi;
        extern const double pimezzi;
        extern const double deg2rad; // [deg]->[rad]
        extern const double rad2deg; // [rad]->[deg]
        extern const double rps2rpm; // [rad/s]->[rev/min]
        extern const double rpm2rps; // [rev/min]->[rad/s]
        extern const double Log2; // base10 logarithm of 2
        extern const double Log5; // base10 logarithm of 5
        extern const double inv_argmin; // minimum 1/x argument
        extern const double log_argmin; // minimum log10 argument

        // . . . System constants
        extern const double OneMegaByte; // 1024*1024 bytes

        // . . . Strings
        extern const String NaNstr; // NaN string representation
        extern const String HexPrefix; // Hexadecimal literals prefix
        extern const char HexDigits[]; // Hexadecimal digits


//---------------------------- Common inline functions

    // . . . Generic
    template<typename T> inline void swap(T& a, T& b) {T tmp=a; a=b; b=tmp;}

    // . . . Logic
    inline bool Xor(const bool b1,const bool b2){return ((!b1&&b2)||(b1&&!b2));} // Return true if different
    inline bool XorNot(const bool b1,const bool b2){return ((!b1||b2)&&(b1||!b2));} // Return true if equal
    inline bool isNaN (const double d) {return std::_isnan(d);} // (return d!=d;) (return d!=NaN)

    // . . . Bytes
    inline U8 HiByte(const U16 w){return U8((w>>8) & 0x00FF);}
    inline U8 LoByte(const U16 w){return U8(w & 0x00FF);}
    inline U16 HiWord(const U32 dw){return U16((dw>>16) & 0x0000FFFF);}
    inline U16 LoWord(const U32 dw){return U16(dw & 0x0000FFFF);}
    //template <typename T, typename S> inline T HiPart (S w) {return T((w>>(4*sizeof(w)) & ... ));}
    //template <typename T, typename S> inline T LoPart (S w) {return T(w);}
    //inline U32 CombineLowParts(const U32 d1, const U32 d2){return ( ( ((d1 & 0xFFFF)<<16) & 0xFFFF0000 ) | (d2 & 0xFFFF));}
    //inline U64 Combine(const U32 l, const U32 h){return (U64(h) << 32) | U64(l);}
    //inline U64 Combine(const U16 ll, const U16 lh, const U16 hl, const U16 hh){return Combine(Combine(ll,lh),Combine(hl,hh));}
    inline U32 Combine(const U16 l, const U16 h){return ((U32(h) << 16) & 0xFFFF0000) | (U32(l) & 0x0000FFFF);}
    template <typename T> inline unsigned int NumofHexDigits(T n){return 2*sizeof(T);} // Size in hex digits ( 4bit)
    U16 EndianConversion(U16 w) {return ((w>>8)&0x00FF) | ((w<<8)&0xFF00);}
    U32 EndianConversion(U32 dw) {return ((dw>>24)&0x000000FF) | ((dw>>8)&0x0000FF00) | ((dw<<8)&0x00FF0000) | ((dw<<24)&0xFF000000);}
    AnsiString __fastcall ByteToHex(const T_BYTE b) {char s[2]={HexDigits[(b>>4)&0x0F],HexDigits[b&0x0F]}; return AnsiString(s,2);} // Convert to hexadecimal string
    AnsiString __fastcall WordToHex(const T_WORD w) {char s[4]= {HexDigits[(w>>12)&0x0F],HexDigits[(w>>8)&0x0F],HexDigits[(w>>4)&0x0F],HexDigits[w&0x0F]}; return AnsiString(s,4);} // Convert to hexadecimal string

    // . . . Numerics
    template<typename T> inline T sign (T x) {return (x>=0 ? 1 : -1);} // Returns the sign of a number
    inline bool IsEven (int n) {return !bool(n%2);}

    // Float operations
    inline bool IsZero (const double d) {return(std::fabs(d)<=eps);} // A number is infinitesimal
    //inline bool AreEqual (const double d1, const double d2) {return(::fabs(d1-d2)<eps);} // Two numbers are equal
    //inline bool IsInteger (double d) {return IsZero(::modf(d,&d));} // Tells if a float is integer

    //template<typename T> inline bool AreNear (const T& d1, const T& d2){return(::fabs(d1-d2)<eps);} // Two numbers are equal
    inline double EnsureNotZero (double v) {if (std::fabs(v)<eps) return eps*sign(v); else return v; } // Self explanatory
    //inline double EnsureGreaterThan (double v, double M) {if v<M) return M; else return v; } // Ensure greater than a value
    //inline double EnsureModGreaterThan (double v, double M) {if ::fabs(v)<M) return M*sign(v); else return v; } // Ensure modulus greater than a value
    //inline double Divide (double n, double d) { if (::fabs(n) return } // Divide two floats: InfDouble < |r=n/d| < SupDouble

    // Casts and constraints
    template<typename T, typename S> inline bool iscastable (const T& v){ return (v>=std::numeric_limits<S>::min() && v<= std::numeric_limits<S>::max()); } // Is castable
    template<typename T, typename S> inline S safe_numcast (const T& v){ forcein(v,std::numeric_limits<S>::min(),std::numeric_limits<S>::max()); return static_cast<S>(v);} // Safe cast
    // Force values in an allowed range
    template<typename T> inline void sat(T& v, const T& M) {if(v>M) v=M;}
    template<typename T> inline void sat(T& v, const T& m, const T& M) {if(v<m) v=m; else if(v>M) v=M;}
    //template<class T> inline T constrain(const T& v, const T& m,const T& M) {return (M < v) ? M : (v < m) ? m : v;}

    // Rounding functions
    template <typename T, typename S> inline T round (S x) // General round function
            {// Rounds to nearest integer
             if (std::numeric_limits<S>::is_integer) return static_cast<T>(x); // Integer type needs no rounding
             const S round_error = std::numeric_limits<S>::round_error(); // Find rounding error
             if (std::numeric_limits<T>::is_integer)
                  {// Integer calculation (fastest)
                   const S minimum = static_cast<S>(std::numeric_limits<T>::min());
                   const S maximum = static_cast<S>(std::numeric_limits<T>::max());
                   if (x < minimum) x = minimum;
                   else if (x > maximum) X = maximum;
                   return static_cast<T>(x+round_error);
                  }
             else {// Floating point calculation (slower)
                   return (x>=0 ? floor(x+round_error) : ceil(x+round_error);
                  }
            }
    /*
    int asmround (double f)
            { // Rounds to nearest integer: improved speed
             int temp;
             _asm {
                    FLD QWORD PTR [f];
                    WAIT;
                    FISTP DWORD PTR [temp];
                    WAIT;
                  }
             return temp;
            }
    */

    inline int round (double x)
            {// Rounds to nearest integer
             if (x<MinInt) x=MinInt; else if (x>MaxInt) x=MaxInt;
             return static_cast<int>(x>0?x+.5:x-.5);
            }

    inline int floor (double x)
            {// Find nearest integer less than x
             if (x<MinInt) x=MinInt; else if (x>MaxInt) x=MaxInt;
             return static_cast<int>(std::floor(x));
            }

    inline int ceil (double x)
            {// Find nearest integer greater than x
             if (x<MinInt) x=MinInt; else if (x>MaxInt) x=MaxInt;
             return static_cast<int>(std::ceil(x));
            }
    inline double round (double val, const double gran){val/=gran; return gran*(val>=0 ? std::floor(val+0.5) : std::ceil(val+0.5));} // Round value to granularity
    inline int round (const int val, const int gran){return round(double(val)/double(gran))*gran;} // Round integer value to integer granularity

    inline int gd (const int i)
            {// Greatest (not banal) divisor
             int d = 1;
             while ( i%++d ) if(d>100){d=1;break;} // give up
             return i/d;
            }

    inline int gcd( int n1, int n2 )
            { // Greatest Common Divisor
                do {
                    n1 = n1%n2;
                    if( n1 == 0 ) return n2;
                    n2 = n2%n1;
                    if ( n2 == 0 ) return n1;
                   }
                while (true);
            }

    inline int lcm( int n1, int n2 ) {return n1*n2/gcd(n1,n2);} // Least Common Multiple

    template<typename T> inline T max(const T a, const T b) {return (a>=b) ? a : b;}
    template<typename T> inline T max(const T a, const T b, const T c) {return max(max(a,b),c);}
    template<typename T> inline T min(const T a, const T b) {return (a<b) ? a : b;}
    template<typename T> inline T min(const T a, const T b, const T c) {return min(min(a,b),c);}
    template<typename T> inline T max(const class std::vector<T>& nl){if (nl.empty()) return NaN; T M=nl[0]; for(uint i=1;i<nl.size();++i) {if (M<nl[i]) M=nl[i];} return M;} // return numlist max
    template<typename T> inline T sum(const class std::vector<T>& nl){T s=0;for(uint i=0;i<nl.size();++i) s+=nl[i];return s;} // compute numlist sum

    // . . . Math
    double SquareWave(const double t, const double T, const double d)
                     { // Square wave given amplitude, period and dutycycle
                      return (std::fmod(t,T)<d*T) ? 1.0 : 0.0;
                     }
    double SquareWave(const double A, const double w0t, const int na)
                     { // Square wave harmonic approximation
                      double s = 0;
                      for (int n=1, nmax=2*na-1; n<=nmax; n+=2) s += std::sin(n*w0t)/n;
                      return 4*A/M_PI * s;
                     }

    // . . . Characters
    inline bool IsAlpha(const char c){return (c>='A'&&c<='Z') || (c>='a'&&c<='z');} // Is anphabetic char?
    inline bool IsContainedIn(const char c,const String& s){for(int i=s.Length();i>=1;--i) if (c==s[i]) return true; return false;}
    inline bool IsNumber(const char c){return (c>='0'&&c<='9');} // Is a number char?
    inline bool IsFloat(const char c){return ( (c>='0'&&c<='9')||c==::DecimalSeparator||c=='+'||c=='-'||c=='E'||c=='e');}  // Is part of a float literal?
    inline bool IsEscapeSequence (const char c){return (c=='\n'||c=='\t'||c=='\r'||c=='\f'||c=='\v');}
    inline bool IsSpacingChar(const char c){return (c=='\ '||c=='\t');}
    //inline bool IsDelimiter(const char c){return (c==' '||c==';'||c=='|'||IsEscapeSequence(c));}
    inline char ClosingBracket(const char c){switch(c){case'(': return')'; case'[': return']'; case'{': return'}'; case'<': return'>'; case'«': return'»'; case'‹': return'›'; case'`': return'´'; default: return ' ';}}

    // . . . Scanning utilities
    inline void Tokenize(stringlist& tokens, const char* s, const String& delims)
        { // Decompose a phrase into tokens given a delimiter set
         String token =""; int i=0;
         while ( s[i] != '\0' )
            {
             if ( IsContainedIn(s[i],delims) ) {if (!token.IsEmpty()) {tokens.push_back(token); token = "";}}
             else token += s[i];
             ++i;
            }
         if (!token.IsEmpty()) tokens.push_back(token);
        }
    inline void Tokenize(stringlist& tokens, const char* s, char d =' ')
        { // Decompose a phrase into tokens given a rigid delimiter, eat spaces between delims and tokens
         String token =""; int i=0;
         while ( s[i] != '\0' )
            {
             if ( s[i] != d ) token += s[i];
             else {tokens.push_back(token.Trim()); token = "";}
             ++i;
            }
         tokens.push_back(token);
        }
    inline double Evaluate ( const String& s )
        { // Gets an input string like "3" or "4.2+5" or "-34+2.1" and returns the result

            // Auxiliary variables
            int len = s.Length(); int i=1; double res=0; String sn = ""; bool E_found;
            while ( i <= len )
               {
                if ((s[i]=='+' || s[i]=='-') && i!=1 && !E_found) {res += sn.Trim().ToDouble(); sn = "";}
                E_found = (s[i]=='E') || (s[i]=='e');
                sn += s[i++];
               }
            return res+sn.Trim().ToDouble();
        }
    // . . . Strings
    inline void DropFirstChar (String& s) {s = s.SubString(2,s.Length()-1);} // Drop first character    
    inline void DropLastChar (String& s) {s = s.SubString(1,s.Length()-1);} // Drop last delimiter
    inline int First (const String& s, const char& d) {for(int i=1; i<=s.Length(); ++i) if(s[i]==d) return i; return -1;} // position of first char occurrence
    inline int Last (const String& s, const char& d) {for(int i=s.Length(); i>=1; --i) if(s[i]==d) return i; return -1;} // position of last char occurrence
    inline String Left (const String& s, const uint n) {return s.SubString(1,n);} // First n chars
    inline String Right (const String& s, const uint n) {return s.SubString(s.Length()-n+1,n);} // Last n chars
    inline String Middle (const String& s, int p1, int p2) {if(p1>s.Length()) p1=s.Length(); if(p2>s.Length()) p2=s.Length(); if(p1>0 && p2>p1) return s.SubString(p1,p2-p1); else return "";} // substring by positions
    inline String After (const String& s, const uint n) {return s.SubString(n+1,s.Length()-n);} // Remaining chars after nth (excluded)
    inline String Before (const String& s, const uint n) {return s.SubString(1,n-1);} // String before nth char (excluded)
    inline String Before (const String& s, const String& d) { return Before(s,::LastDelimiter(d,s)); } // String before last delimiter
    // Strings contained in brackets
    inline String Inner (const String& s) {char d1=s[1]; char d2=ClosingBracket(d1); return Middle(s,2,Last(s,d2));} // bracket contained string
    inline String OuterDx (const String& s) {char d1=s[1]; char d2=ClosingBracket(d1); return After(s,Last(s,d2));} // after a bracket contained string

    inline bool HasNonVisibleChars (const String& str)
        { // Return true if string contains non visible chars
         for (int i=str.Length(); i>=1; i--) if (::IsDelimiter("\n\r\t\v\f\b\a",str,i)) return true;
         return false;
        }

    inline bool IsQuotedString (const String& str)
        { // Return true if string is a quoted string
         return (str.IsEmpty() ? false : str[1]=='"' && str[1]==str[str.Length()]);
        }

    inline String FromQuotedString (const String& sin)
        { // "ab\tc"  -->  ab   c
         char *s = sin.c_str();
         String sout = ::AnsiExtractQuotedStr(s, '"');
         // Resolve escape sequences
         TReplaceFlags RFlags = TReplaceFlags() << rfReplaceAll;
         sout = ::StringReplace(sout, "\\ ", "\ ", RFlags);
         sout = ::StringReplace(sout, "\\n", "\n", RFlags);
         sout = ::StringReplace(sout, "\\r", "\r", RFlags);
         sout = ::StringReplace(sout, "\\t", "\t", RFlags);
         sout = ::StringReplace(sout, "\\v", "\v", RFlags);
         sout = ::StringReplace(sout, "\\f", "\f", RFlags);
         sout = ::StringReplace(sout, "\\b", "\b", RFlags);
         sout = ::StringReplace(sout, "\\a", "\a", RFlags);
         return sout;
        }

    inline String ToQuotedString (const String& sin)
        { // ab   c  -->  "ab\tc"
         String sout = ::AnsiQuotedStr(sin, '"');
         // Translate non visible chars in escape sequences
         TReplaceFlags RFlags = TReplaceFlags() << rfReplaceAll;
         sout = ::StringReplace(sout, "\ ", "\\ ", RFlags);
         sout = ::StringReplace(sout, "\n", "\\n", RFlags);
         sout = ::StringReplace(sout, "\r", "\\r", RFlags);
         sout = ::StringReplace(sout, "\t", "\\t", RFlags);
         sout = ::StringReplace(sout, "\v", "\\v", RFlags);
         sout = ::StringReplace(sout, "\f", "\\f", RFlags);
         sout = ::StringReplace(sout, "\b", "\\b", RFlags);
         sout = ::StringReplace(sout, "\a", "\\a", RFlags);
         return sout;
        }

    inline AnsiString StringListToString (const stringlist& sl) {String s=""; for(unsigned int i=0; i<sl.size(); ++i) s+=sl[i]+"\n"; return s.TrimRight();} // TStringList to AnsiString
    inline AnsiString DoubleToString (const double v, const int p =6, const int d =3) {return isNaN(v) ? NaNstr : ::FloatToStrF(v,ffGeneral,p,d); } // double to AnsiString
    inline AnsiString TPointToString (const TPoint& P) { return String("( ")+ String(P.x)+String(" ; ")+String(P.y)+String(" )"); } // TPoint to AnsiString
    inline AnsiString BufToString (const char*const buf, const U32 siz) {AnsiString s = AnsiString::StringOfChar('\0', siz); for(U32 i=0;i<siz;++i) s[i+1]=buf[i]; return s;} // Byte buffer to AnsiString
    inline AnsiString CharToString(const char c) {return (c>=0 && c<AsciiCtrlChar_Size) ? String(AsciiCtrlCharStr[c]) : String(c);} // Convert to string representation
    template<typename T> inline AnsiString ValToBin(const T v) {AnsiString s = AnsiString::StringOfChar('\0',8*sizeof(v)); for(unsigned int i=0, im=s.Length(); i<im; ++i) s[im-i]=((v>>i)&1)?'1':'0'; return s;} // Convert int to binary representation

    inline char StringToChar (const String& s)
            { // String to Boolean
             if ( s.Length() == 1 ) return s[1];
             else throw (EConvertError("\'" + s + "\' is not a valid character"));
            }

    inline String BoolToString (const bool b) {return b?"true":"false";} // Boolean to String
    inline bool StringToBool (const String& s)
            { // String to Boolean
             if (s=="true"||s=="1"||s=="yes") return true;
             else if (s=="false"||s=="0"||s=="no") return false;
             else throw (EConvertError("\'" + s + "\' is not a valid boolean value"));
            }

    inline String TColorToRGBStr (const TColor& c) {return "("+String(int(GetRValue(c)))+","+String(int(GetGValue(c)))+","+String(int(GetBValue(c)))+")";} // TColor to RGB String es.(23,4,255)
    inline TColor RGBStrToTColor (const String& rgbstr)
            { // RGB String es.(23,4,255) to TColor
             stringlist tokens;
             String s = Inner( rgbstr.Trim() );
             Tokenize(tokens,s.c_str(),',');
             //ShowMessage(StringListToString(tokens));
             if (tokens.size()!=3) throw (EConvertError("\'" + s + "\' is not a valid RGB string"));
             return static_cast<TColor>(RGB(tokens[0].Trim().ToInt(),tokens[1].Trim().ToInt(),tokens[2].Trim().ToInt()));
            }

    // . . . System (Win32)
    inline bool IsWin2k() {OSVERSIONINFO v;v.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);::GetVersionEx(&v);return ( (v.dwMajorVersion>=5) && (v.dwPlatformId == VER_PLATFORM_WIN32_NT) );} // Operating system is win2000 or more

    // . . . System (BCB)
    inline TRect Shrink (const TRect& R, int perc) {static int dw,dh; dw=(perc*R.Width())/200; dh=(perc*R.Height())/200; return TRect(R.left+dw,R.top+dh,R.right-dw,R.bottom-dh);} // Shrink rectangle respect center given a percentage
    inline TRect Normalize (TRect R) { if (R.left>R.right) swap(R.left,R.right); if (R.top>R.bottom) swap(R.top,R.bottom); return R;} // Normalize a TRect
    inline bool IsInRect (const TPoint& P, const TRect& R) {return (R.left<R.right ? P.x>=R.left && P.x<=R.right : P.x>=R.right && P.x<=R.left) && (R.top<R.bottom ? P.y>=R.top && P.y<=R.bottom : P.y>=R.bottom && P.y<=R.top);} // The Point is in the Rectangle

    inline void __fastcall MsgInfo(const String& who,const String& msg) {::MessageBeep(MB_ICONASTERISK); Application->MessageBox(msg.c_str(), who.c_str(),MB_OK|MB_ICONASTERISK);} // Info MessageBox
    inline bool __fastcall MsgAsk(const String& who,const String& msg) {::MessageBeep(MB_ICONQUESTION); return Application->MessageBox(msg.c_str(), who.c_str(),MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2) == IDYES;}
    inline int __fastcall MsgAskCancel(const String& who,const String& msg) {::MessageBeep(MB_ICONQUESTION); return Application->MessageBox(msg.c_str(), who.c_str(),MB_YESNOCANCEL|MB_ICONQUESTION|MB_DEFBUTTON3);} // IDYES,IDNO
    inline bool __fastcall MsgAskWarn(const String& who,const String& msg) {::MessageBeep(MB_ICONHAND); return Application->MessageBox(msg.c_str(), who.c_str(),MB_YESNO|MB_ICONWARNING|MB_DEFBUTTON2) == IDYES;}
    inline void __fastcall MsgWarn(const String& who,const String& msg) {::MessageBeep(MB_ICONHAND); Application->MessageBox(msg.c_str(), who.c_str(),MB_OK|MB_ICONWARNING);} // Warn MessageBox
    inline void __fastcall MsgError(const String& who,const String& msg) {::MessageBeep(MB_ICONEXCLAMATION); Application->MessageBox(msg.c_str(), who.c_str(),MB_OK|MB_ICONERROR);} // Error MessageBox

    inline void __fastcall MsgInfo(const String& msg) {MsgInfo(::Application->Title,msg);}
    inline int __fastcall MsgAsk(const String& msg) {return MsgAsk(::Application->Title,msg);}
    inline void __fastcall MsgWarn(const String& msg) {MsgWarn(::Application->Title,msg);}
    inline void __fastcall MsgError(const String& msg) {MsgError(::Application->Title,msg);}

    inline void MsgLastSysError() {MsgError(::SysErrorMessage(::GetLastError()));} // WinAPI Error MessageBox
    inline String GetLastErrorMessage() {return ::SysErrorMessage(::GetLastError());} // Retrieves the message relative to the last winAPI error
    inline String GetDate() {return ::Date().DateString();}// Get current date; Date().FormatString ("dd.mm.yyyy");
    inline String Today() {return ::Date().DayOfWeek();}// Get current day name
    inline String GetTime() {return ::Time().FormatString("hh':'nn':'ss'.'zzz");}// Get current time
    inline void AssignBmpOfList(Graphics::TBitmap* ptrB, TImageList* iml, int idx){Graphics::TBitmap* tmpB = new Graphics::TBitmap();iml->GetBitmap(idx,tmpB);ptrB->Assign(tmpB);delete tmpB;} // Assegna ad una bitmap un'immagine della lista
    inline Word GetKey (const TShortCut& sh){Word k; TShiftState s; ::ShortCutToKey(sh,k,s); return k;}
    inline void SetShortCut(TAction* act,TShortCut sh){act->ShortCut=sh;}
    inline void SetShortCut(TAction* act,Word Key,TShiftState Shift){SetShortCut(act,::ShortCut(Key, Shift));}

    // . . . Debug
    inline void ViewAddress(void* p) { MsgInfo("ViewAddress",::IntToHex(reinterpret_cast<int>(p),8)); } // Show an address


    
//---------------------------- Common functions prototypes

    // . . . Numerics utilities
    bool GetRandomBool(const int); // Get a random boolean given % probability of false
    int GetRandomNumber(const int M, const int m =0); // Get a random integer between [m,M]
    double GetRandomNumber(const double& M,const double& m,const double& g); // Get a random number between [m,M] and granularity g

    // . . . String utilities
    String ForceToBeIdentifier (const String&); // Force a string to be an identifier name (alphanumeric or '_')

    String HexToString(const String&); // Hex chars to chars ( ex: "64 65 66" => "def")
    String StringToHex(const String&); // chars to hex codes ( ex: "def" => "64 65 66" )
    WideString ShiftCtrlChars(WideString s, const bool back =false); // Shift non-printable chars to unicode representation

    String ExtractFirstPart(String& s, const char d); // Extract first part, ex: COM1,9600,E,8,1 => COM1

    // . . . Scanning utilities
    void DecomposeArgs (stringlist&, char*); // Decompose properly command line arguments
    stringlist DecomposeArgs(char* s) {stringlist sl; DecomposeArgs(sl, s); return sl;}
    void StringToNumlist ( numlist&, const String& ); // Get a sequence of numbers
    inline numlist StringToNumlist ( const String& s) {numlist nl; StringToNumlist(nl,s); return nl;}
    String NumlistToString ( const numlist&, char ='|' ); // Translate into String
    String GetDelimitedSubStrings ( stringlist&, const String&, const char, const char ); // Get the substrings delimited by given characters

    // . . . System (Win32)
    //HFONT CreateRotatedFont(const class TFont*, const int Deg =90); // Create a rotated version of a TFont
    void TextOutEx (class TCanvas* Canvas, const int X, const int Y, const String& Txt, const int Deg=90); // Canvas TextOut with angle
    String __fastcall LoadString(const UINT);
    void __fastcall LoadBitmap(Graphics::TBitmap*,const UINT);
    void __fastcall LoadCursor(const TCursor&, const UINT);
    void __fastcall PlaySoundFromRes(const UINT); // Riproduce un suono dalle risorse
    void __fastcall ShellExecute(const String&,const String& =""); // Execute external file in Os
    void __fastcall MapiResult(const unsigned int); // See Mapi result, throwing exceptions
    void TraslateWorkingArea (int);
    void ProcessMessages(); // Process all windows messages in queue
    String GetEnvironmentVariable(const String&); // Retrieve the value of a Windows environment variable
    void AppendUserEnvironmentVariable(const String&, const String&); // Add permanently a user environment variable
    String GetTmpPath(); // Retrieve the system temporary folder path
    String GetHomePath(); // Get user home directory
    String SelectFolder();
    bool __fastcall IsDirectory(const String&); // State if path is a directory
    void ListFiles(class TStringList*, String, const String, const bool =false); // Fill a string list with files found in a dir
    void DeleteFiles(const String&,const String&); // Delete all matching files
    unsigned __int64 __fastcall FileSize(const String&); // Get file size
    void __fastcall PathAppend(String&, const String&); // Append to path
    bool SetProcPriority(DWORD); //  Changes priority of current process
    void MapiSend(const String&, const String&, const String&, class TStringList*); // Send mail with MAPI mail service

    // . . . Borland's VCL
    String ComponentToString(class TComponent*); // Returns a string representation of a component (like in .dfm files)
    void StringToComponent(const String&, class TComponent*); // Fills a component from a given string
    void TxtfileToComponent(const String&, class TComponent*); // Fills a component from a given path
    String ExtractFileName (const String&); // Extract file name without extension
    String ShiftStateToString (const TShiftState&); // Converts a TShiftState Set to String
    String PenStyleToString (const TPenStyle&); // Converts TPenStyle to String
    TPenStyle StringToPenStyle (String); // Converts a String to TPenStyle
    String BrushStyleToString (const TBrushStyle&); // Converts TBrushStyle to String
    TBrushStyle StringToBrushStyle (String); // Converts a String to TBrushStyle
    String PenModeToString (const TPenMode&); // Converts TPenMode to String
    TPenMode StringToPenMode (String); // Converts a String to TPenMode
    String TFontToString (TFont*); // Converts TFont* to String
    void StringToTFont (const String&, TFont*); // Converts a String to TFont
    bool AreDifferent(class TFont*, class TFont*); // A comparison function between TFont
    void Copy(const class TCanvas*, class TCanvas*); // A copy function

    void __fastcall SetFormTransparence (TForm*, int); // Set transparence of a Form (0-255)
    void __fastcall SetFormRoundRectRegion (TForm*, int joint); // Crop a rounded region on the form
    void __fastcall SetFormEllipticRegion (TForm*); // Crop an elliptical region on the form
    void __fastcall FormPleaseWait (bool); // A feedback during long elaboration
    void __fastcall FillGradientControl (TControl* ctrl, unsigned int Density =256); // Fill a TControl with a gradient

    void PickRandomPen(class TCanvas*); // Random Pen in Canvas
    void PickRandomBrush(class TCanvas*); // Random Brush in Canvas

    void TextOutNotXor_Bmp (HDC hdc, const int X, const int Y, const char* Text, const int Length); // Xoring text using memory bitmap (winapi)
    void TextOutNotXor_Vcl (TCanvas* Canvas, const int X, const int Y, const String& Text); // Xoring text using temporary memory bitmap (VCL)
    void TextOutNotXor_Pth (TCanvas* Canvas, const int X, const int Y, const String& Text); // Xoring text using paths


    // . . . Other
    String BuildMatlabArrayDef(const double start, const double step, const double end); // Eavenly spaced array matlabl definition string

    // . . . Debug
  #ifdef _DEBUG
    void HandleAsserting(char *tst, char *msg, char *fil, int lin, char *fnc, bool& skip); // Asserting function in ASSERTING
  #endif

//---------------------------- Common classes Definition

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Exceptions <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    //-------------------------------------------------------------------
    class EAbort : public Exception
    { // An abort operation was requested
     public: //                                             .............
     // . . . Public methods
     EAbort(const String& msg) : Exception(msg) { Message = msg; }
     virtual String ClassName() const {return "EAbort";}
     protected: //                                          .............
     // . . . Protected methods
     private: //                                            .............
     // . . . Private attributes
     // . . . Private methods
    }; // end 'EAbort'

    //-------------------------------------------------------------------
    class EParsingError : public Exception
    {
     public: //                                             .............
     // . . . Public methods
     EParsingError(const String& msg, const unsigned int ln, const unsigned int cl)
                  : Exception(msg), row(ln), col(cl) { Message = BuildMessage(msg); }
     virtual String ClassName() const {return "EParserError";}
     // . . . Properties
     __property unsigned int Row = { read=row }; // offending line number
     __property unsigned int Col = { read=col }; // offending character column
     __property String Position = { read=get_Position }; // offending position msg
     protected: //                                          .............
     // . . . Protected methods
     String BuildMessage(const String& m) {return ClassName() + String(" in ") + Position + String(" : ") + m;}
     private: //                                            .............
     // . . . Private attributes
     unsigned int row,col;
     // . . . Private methods
     virtual String get_Position() const {return "Line " + String(Row) + ", Character " + String(Col);}
    }; // end 'EParsingError'

    //-------------------------------------------------------------------
    class EUnmatchingData : public EParsingError
    {
     public: //                                             .............
     // . . . Public methods
     EUnmatchingData(const unsigned int ln, const unsigned int cl, const unsigned int dcol)
                    : EParsingError("",ln,cl), declcols(dcol)
                    { Message = BuildMessage("expected " + String(DeclColumns) + " data columns"); }
     virtual String ClassName() const {return "EUnmatchingData";}
     // . . . Properties
     __property unsigned int DeclColumns = { read=declcols }; // declared columns number
     private: //                                            .............
     // . . . Private attributes
     unsigned int declcols;
    }; // end 'EUnmatchingData'

    //-------------------------------------------------------------------
    class ESyntaxError : public EParsingError
    {
     public: //                                             .............
     // . . . Public methods
     ESyntaxError(const unsigned int ln, const unsigned int cl, const char ch)
                 : EParsingError("Syntax error",ln,cl), offchar(ch)
                 { Message = BuildMessage("check and fix"); }
     virtual String ClassName() const {return "ESyntaxError";}
     // . . . Properties
     __property char OffChar = { read=offchar }; // offending line number
     private: //                                            .............
     // . . . Private attributes
     char offchar; // Offending character
     //virtual String get_Position() const {return "Line " + String(Row) + ", Character " + String(Col) + " (offending char:" + String(OffChar) + ")";}
    }; // end 'ESyntaxError'

    //-------------------------------------------------------------------
    class EInvalidToken : public ESyntaxError
    {
     public: //                                             .............
     // . . . Public methods
     EInvalidToken(const unsigned int ln, const unsigned int cl, const char ch, const String& tk)
                  : ESyntaxError(ln,cl,ch) , Token(tk)
                  { Message = BuildMessage("\'" + Token +" \' is not a valid token"); }
         virtual String ClassName() const {return "EInvalidToken";}
     // . . . Properties
         String Token; // Offending token
     private: //                                            .............
     // . . . Private attributes
    }; // end 'EInvalidToken'

    //-------------------------------------------------------------------
    class EInvalidNumber : public ESyntaxError
    {
     public: //                                             .............
     // . . . Public methods
     EInvalidNumber(const unsigned int ln, const unsigned int cl, const char ch, const unsigned int dcol)
                   : ESyntaxError(ln,cl,ch) , DataColumn(dcol)
                   { Message = BuildMessage("not a valid number in data column " + String(DataColumn)); }
     virtual String ClassName() const {return "EInvalidNumber";}
     // . . . Properties
     unsigned int DataColumn;
     private: //                                            .............
     // . . . Private attributes
    }; // end 'EInvalidNumber'





//>>>>>>>>>>>>>>>>>>>> Concrete classes

    //.........................................................
    // Generic bags
    struct stu_Range {
                      stu_Range() {t1=t2=0;}
                      stu_Range(double l, double r) : t1(l), t2(r) {}
                      stu_Range(stu_Range& r) {t1=r.t1; t2=r.t2;}
                      double t1, t2;
                     };

    struct stu_Rect {
                     stu_Rect() {left=top=right=bottom=0;}
                     stu_Rect(double l,double t,double r,double b){left=l;top=t;right=r;bottom=b;}
                     stu_Rect(const stu_Rect& r) {left=r.left; top=r.top; right=r.right; bottom=r.bottom;}
                     double width () const { return right-left; }
                     double height() const { return bottom-top; }
                     stu_Rect& operator= (const double d) { left=right=top=bottom=d; return *this; }
                     double left, right; double top, bottom;
                    };


    //-----------------------------------------------------------------------
    template <class T> class cls_DynamicArray // A simple managed dynamic array
    { /******************************************************************
      *   mat::cls_DynArray<double> a(6);
      *   a[1] = 3.4;
      *   for (unsigned int i=0; i<a.Size; ++i) cout << a[i];
      ******************************************************************/
    public:
        // . . . Public methods
        cls_DynamicArray<T>( unsigned int siz ) : Size(siz) {vals = new T[siz];}
        ~cls_DynamicArray<T>() { delete[] vals; }
        // Accessing
        T& operator[](unsigned int i) {if(i>=Size)i=Size-1; return vals[i];}
        T* get() const {return vals;}
        // Casts
        operator T*() const {return vals;}
        operator String() const {String s=""; for(unsigned int i=0; i<Size; ++i) s += String(vals[i]) + String("\ "); return s.TrimRight();}
        // . . . Public attributes (!)
        const unsigned int Size;

     private:
       // . . . Private attributes
       T *vals; // Internal container
       // . . . Private methods
    }; // end 'cls_DynamicArray'


    //-----------------------------------------------------------------------
    // A class to do something when entering and exiting a scope
    class cls_Scope {
    public:
        // . . . Public methods
        __fastcall cls_Scope(typ_FnFastVoid in, typ_FnFastVoid ou) : i_OnExit(ou) {if(in) in();}
        __fastcall ~cls_Scope() {if(i_OnExit) i_OnExit();}
        // . . . Properties
        //__property typ_FnFastVoid OnEnter = {read=i_OnEnter, write=i_OnEnter};
        //__property typ_FnFastVoid OnExit = {read=i_OnExit, write=i_OnExit};

        private:
        // . . . Private attributes
        typ_FnFastVoid i_OnExit;
        // . . . Private methods
    }; // end 'cls_Scope'

    //-----------------------------------------------------------------------
    class cls_Progress {
    public:
        // . . . Public methods
        __fastcall cls_Progress(typ_FnProgress f, const unsigned int p, const unsigned int r)
                   : i_OnProgress(f), i_ExpectedPasses(p), i_RefreshTimes(r)
        {
         i_Abort = false;
         i_CurrentPass = i_RefreshPass = 0;
         i_Delta = max(2U, i_ExpectedPasses/i_RefreshTimes);
         if (i_OnProgress) i_OnProgress(0,i_Abort);
        }
        //__fastcall ~cls_Progress() {if (i_OnProgress) i_OnProgress(0,i_Abort);}

        //-------------------------------------------------------
        // Called at each pass, returns true if abort is requested
        bool __fastcall CheckPass(const unsigned int passes =1)
        {
         i_CurrentPass += passes;
         if ( (i_CurrentPass-i_RefreshPass) > i_Delta )
             {
              i_RefreshPass = i_CurrentPass;
              if (i_OnProgress) i_OnProgress((100*i_CurrentPass)/i_ExpectedPasses,i_Abort);
             }
         return i_Abort;
        }
        // . . . Properties
        unsigned int __fastcall CurrentPass() const {return i_CurrentPass;}

        private:
        // . . . Private attributes
        typ_FnProgress i_OnProgress;
        unsigned int i_ExpectedPasses,
                     i_CurrentPass,
                     i_RefreshPass,
                     i_Delta;
        unsigned int i_RefreshTimes;
        bool i_Abort;
    }; // end 'cls_Progress'



    //-----------------------------------------------------------------------
    // A wrapper to TFileStream
    class cls_FileStream {
    public:
        // . . . Public methods
        __fastcall cls_FileStream(const AnsiString FileName, const Word Mode) {i_Stream = new TFileStream(FileName, Mode);}
        __fastcall ~cls_FileStream() {delete i_Stream;}
        // Main facilities
        void __fastcall Write(const AnsiString& s) {i_Stream->Write(s.c_str(), s.Length());}
        void __fastcall WriteLine(const AnsiString& s) {Write(s + "\r\n");}
        //FStream->WriteComponent(Memo1);
        // . . . Properties
        //__property TStream* Stream = {read=i_Stream};
        private:
        // . . . Private attributes
        TStream* i_Stream;
        // . . . Private methods
    }; // end 'cls_FileStream'


    /*
    //-------------------------------------------------------------------
    class TcStringStream : public TStream {
    public://                                               ~~~~~~~~~~~~~

    // . . . Public methods
      TcStringStream();
      //~TcStringStream();

      virtual int __fastcall Read(void *Buffer, int Count);
      virtual int __fastcall Write(const void *Buffer, int Count);
      virtual int __fastcall Seek(int Offset, Word Origin)
         {switch(Origin) {case soFromBeginning : return Offset; break;
                          case soFromCurrentPosition : return (Position+Offset); break;
                          case soFromEnd : return (Size-Offset); break;
                          default: return 0;}
         }

    // . . . Properties (inherited)
      //__property int Position = {read=GetPosition, write=SetPosition, nodefault};
      //__property int Size = {read=GetSize, write=SetSize, nodefault};

    // . . . Public attributes (!)

    private://                                              ~~~~~~~~~~~~~

    // . . . Private attributes
      char* str;

    // . . . Private methods
      // . . . Properties
      //bool __fastcall get_IsOrdinate (void) const {return ();}

    }; // end 'TcStringStream'
    */

    //...................................................................
    class cls_SimpleIndexSequence // A single-step sequence of indexes
    {
     public://                                              .............
     // . . . Public methods
     cls_SimpleIndexSequence(int t =0, int e =0, int s =1) : top(t),end(e),step(s) {}
     cls_SimpleIndexSequence(const cls_SimpleIndexSequence& sq) {Assign(sq);}
     cls_SimpleIndexSequence(const String& s) {Assign(s);}
     operator String() const {return String("[")+String(top)+String(":")+String(step)+String(":")+String(end)+String(")");}
     cls_SimpleIndexSequence& operator= (const cls_SimpleIndexSequence& sq) { Assign(sq); return *this; }
     cls_SimpleIndexSequence& operator= (const String& s) { Assign(s); return *this; }
     // . . . Services
     void Rescale (const double r, const bool alsostep =true)
         { // Rescale operation
          // TODO 2: return rounding errors
          top = round(r*top);
          end = round(r*end);
          if ( alsostep )
             { step = round(r*step);
               if ( step == 0 ) step = sign(end-top); // ensure at least 1
             }
         }
     // . . . Properties
     unsigned int Dimension() const {return (std::abs(end-top)/step);}
     int MaxIndex() const {return max(top,end);}
     // . . . Public attributes (!)
     int top,end,step;

     private://                                             .............
     // . . . Private attributes
     // . . . Private methods
     void Assign (const cls_SimpleIndexSequence& s) {top=s.top; end=s.end; step=s.step;}
     void Assign (const String& s)
         { // Assign from string like "1:3:15" , "1;4+6" , "53|-2|4"
          numlist nums; StringToNumlist(nums, s);
          switch ( nums.size() )
                 {
                  case 2 : // Only extremes given
                      top = round(nums[0]);
                      end = round(nums[1]);
                      step = sign(end-top);
                      break;
                  case 3 : // Step given
                      top = round(nums[0]);
                      end = round(nums[2]);
                      step = sign(end-top) * std::abs(round(nums[1]));
                      //if ( (end-top)%step ) throw EConvertError(String("Invalid sequence, ") + String(end) + String(" can\'t be reached with step ") + String(step) );
                      break;
                  default : throw EConvertError(String("Invalid sequence (declared with ") + String(nums.size()) + String(" numbers)"));
                 }
         }
    }; // end 'cls_SimpleIndexSequence'

    //...................................................................
    class cls_IndexSequence : public std::vector<cls_SimpleIndexSequence>
    { // A sequence of indexes, (last indexes are excluded)
     public://                                              .............
     // . . . Public methods
     cls_IndexSequence() {;}
     cls_IndexSequence(const cls_IndexSequence& s) {Assign(s);}
     cls_IndexSequence(const String& s) {Assign(s);}
     operator String() const
         {
          String s = "";
          for ( unsigned int i=0; i<size(); ++i ) s += String(operator[](i));
          return s;
         }
     cls_IndexSequence& operator= (const cls_IndexSequence& s) { Assign(s); return *this; }
     cls_IndexSequence& operator= (const String& s) { Assign(s); return *this; }
     // . . . Services
     void Rescale (const int sz) // TODO 2: signal rounding errors
         { // Rescale respect another size
         double ratio = double(sz)/double(MaxIndex());
          for ( unsigned int i=0; i<size(); ++i ) operator[](i).Rescale(ratio,false);
         }
     // . . . Properties
     unsigned int Dimension () const
         {
          unsigned int S = 0;
          for ( unsigned int i=0; i<size(); ++i ) S += operator[](i).Dimension();
          return S;
         }
     int MaxIndex () const
         {
          if ( empty() ) return -1;
          int idx = front().MaxIndex();
          for ( unsigned int i=1; i<size(); ++i ) if (idx < operator[](i).MaxIndex()) idx = operator[](i).MaxIndex();
          return idx;
         }
     // . . . Public attributes (!)

     private://                                             .............
     // . . . Private methods
     void Assign (const cls_IndexSequence& sq)
         { // Copy assignment
          clear();
          for (unsigned int i=0; i<sq.size(); ++i) push_back(sq[i]);
         }
     void Assign (const String& s)
         { // Assign from string like "[1:50)" , [30:-1:0) , "[10+3:4:-50+3)[0:13)[53:2:97)"
          stringlist sseqs;
          GetDelimitedSubStrings ( sseqs, s, '[', ')' );
          for (unsigned int i=0; i<sseqs.size(); ++i) push_back(cls_SimpleIndexSequence(sseqs[i]));
         }
    }; // end 'cls_IndexSequence'

    //-----------------------------------------------------------------------
    class cls_Font {// TFont wrapper // : public TFont
    public:
        // . . . Public methods
        cls_Font () { i_Font = new TFont(); }
        ~cls_Font () { delete i_Font; i_Font=0;}
        // Assegnamento e cast verso `String'
        cls_Font& operator= (const String& s) {StringToTFont(s,i_Font); return *this;}
        operator String() const {return TFontToString(i_Font);}
        // Assegnamento e cast verso `TFont*'
        cls_Font& operator= (TFont* f) {i_Font->Assign(f); return *this;}
        operator TFont*() const {return i_Font;}

    private:
    // . . . Private attributes
        class TFont* i_Font;
    // . . . Private methods
    }; // end 'cls_Font'




    //-----------------------------------------------------------------------
    class cls_ScaleUnitOffset {// set of scale,unit,offset
    public:
        // . . . Public methods
        cls_ScaleUnitOffset(const double s =1, const String& u =String(""), const double o=0)
            : Scale(s), Unit(u), Offset(o), DataType("") {;}
        cls_ScaleUnitOffset(const cls_ScaleUnitOffset& suo)
            : Scale(suo.Scale), Unit(suo.Unit), Offset(suo.Offset), DataType(suo.DataType) {;}
        //~cls_ScaleUnitOffset () {}
        bool IsIdentity() const {return (IsZero(Scale-1.0) && IsZero(Offset));} // Tells if linear transformation is an identity
        bool HasDataType() const {return DataType.Length()>0;} // A type is specified
        bool Undefined() const {return (Unit.IsEmpty() && IsIdentity());} // Default values
        String GetUnitString() const {return Unit.IsEmpty()? String(""):String("[")+Unit+String("]");}
        // . . . Assegnamento e cast verso `String'
        //-------------------------------------------------
        cls_ScaleUnitOffset& operator= (String s)
            { // ex: "1.2cm+2.2" , "34E2 s" , "Km+3" , "A" , "23-0.4" , "1V+3|u16|"
              // defaults: 1 if no scale found, 0 if no offset found, "" if no unit found
              // . . . Preparation
              int i=1; // Current character position
              bool E_found = false; // Exponential found
              String tok = "";
              // . . . Get scale
              while ( i<=s.Length() && IsFloat(s[i]) && !((s[i]=='+'||s[i]=='-') && i!=1 && !E_found))
                    {
                     E_found = (s[i]=='E') || (s[i]=='e');
                     tok += s[i++];
                    }
              tok = tok.Trim();
              if ( tok.IsEmpty() ) Scale = 1;
              else Scale = tok.ToDouble();
              // . . . Get unit
              tok = "";
              while(i<=s.Length() && s[i]!='+' && s[i]!='-' && s[i]!='|') tok += s[i++];
              Unit = tok.Trim();
              // . . . Get offset
              if(i<=s.Length() && (s[i]=='+' || s[i]=='-'))
                  {
                   tok = "";
                   while(i<=s.Length() && s[i]!='|') tok += s[i++];
                   tok = tok.Trim();
                   if ( tok.IsEmpty() ) Offset = 0;
                   else Offset = tok.ToDouble();
                  }
              // . . . Get data type
              if(i<=s.Length() && s[i]=='|')
                  {
                   tok = ""; ++i;
                   while(i<=s.Length() && s[i]!='|') tok += s[i++];
                   DataType = tok.Trim();
                  }
              return *this;
            }
        //-------------------------------------------------
        virtual operator String() const
            { // Build string
             if ( Undefined() ) return "";
             String s = ::FloatToStr(Scale);
             s += Unit;
             if ( !IsZero(Offset) )
                {
                 if ( Offset > 0 ) s += String("+");
                 s += ::FloatToStr(Offset);
                }
             if(HasDataType()) s+= "|"+DataType+"|";
             return s;
            }
        // . . . public attributes (!)
        double Scale;
        String Unit;
        double Offset;
        String DataType;

    private://
    // . . . Private attributes
    // . . . Private methods
    }; // end 'cls_ScaleUnitOffset'


    //-----------------------------------------------------------------------
    class cls_StringIntInt {
    public://

        // . . . Public methods
        cls_StringIntInt(const String& s) {FromString(s);}
        // Assegnamento e cast verso `String'
        cls_StringIntInt& operator= (const String& s) {FromString(s); return *this;}
        operator String() const {return ToString();}

        void FromString(const String& s)
            {
             stringlist sl;
             Tokenize(sl, s.c_str(), ',');
             if ( sl.size() != 3 ) throw EConvertError("Invalid cls_StringIntInt: " + s);
             Label = sl[0];
             Offset = sl[1].ToInt();
             Size = sl[2].ToInt();
            }
        //String ToString() const {return Label + ',' + String(Offset) + ',' + String(Size);}
        String ToString() const {return Label + ", 0x" + ::IntToHex(Offset,6) + ", 0x" + ::IntToHex(Size,4);}
        // . . . Public attributes (!)
        String Label;
        int Offset;
        int Size;
    }; // end 'cls_StringIntInt'

    //-----------------------------------------------------------------------
    class cls_StringIntIntList : public std::vector<cls_StringIntInt> {
    public://

        // . . . Public methods
        cls_StringIntIntList(const String& s) {FromString(s);}
        // Assegnamento e cast verso `String'
        cls_StringIntIntList& operator= (const String& s) {FromString(s); return *this;}
        operator String() const {return ToString();}
        void FromString(const String& s)
            {
             stringlist st;
             GetDelimitedSubStrings ( st, s, '{', '}');
             clear();
             for (unsigned int i=0; i<st.size(); ++i) push_back(cls_StringIntInt(st[i]));
            }
        String ToString() const
            {
             String s = "";
             for (unsigned int i=0; i<size(); ++i) s += "{" + String(operator[](i)) + "},";
             DropLastChar(s);
             return s;
            }

    private://
    // . . . Private attributes
    // . . . Private methods
    }; // end 'cls_StringIntIntList'




}//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
namespace mat = nms_MatCommon; // a short alias for this namespace

#endif//=====================================================================

