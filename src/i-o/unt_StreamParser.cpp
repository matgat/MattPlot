//---------------------------------------------------------------------------
#include <vcl.h>
#include "unt_MatCommon.h"
#include "unt_Log.h"
#pragma hdrstop
#include "unt_StreamParser.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//================================ Settings =================================
/*                                            Open or Drop
                               TFileStream* /
                     TStream* /             \ command line (appended rows)
    Input channels: /         \ other                                     */



////////////////////////////// cls_StreamParser /////////////////////////////
// TODO 4: Buffered GetChar that takes Buffer[BufSize] from stream

//================================ Settings =================================
//============================= Static members ==============================

//============================= Public methods ==============================

//---------------------------------------------------------------------------
cls_StreamParser::cls_StreamParser ( class cls_Log* l ) : Log(l)
{ // Class constructor

// . . . Inputs

// . . . Defaults
    // . . . State (initialized at parsing)
    // . . . Settings
    Tolerant = false;
    ExpectedDelimiters = ";|";
    StrictDelimiter = '\n'; // disable strict delims
    DecimalSep = ::DecimalSeparator; // Local decimal separator
    ThousandSep = ::ThousandSeparator; // Local thousand separator
    MaxLine = 4294967295;

    // . . . Auxiliary
    i_Abort = false;
    i_OnProgress = 0;

// . . . Other things
    // A check
    if ( DecimalSep == ThousandSep ) Log->Error("Parser","Decimal and thousand separators are identical! (" + String(DecimalSep) + ") parsed numbers may be wrong");
}

//---------------------------------------------------------------------------
//cls_StreamParser::~cls_StreamParser()
//{ // Class destructor
//        //
//}
//---------------------------------------------------------------------------



//============================ Private Methods ==============================


//---------------------------------------------------------------------------
cls_StreamParser::typ_ReadChar cls_StreamParser::PeekChar ( class TStream* s )
{ // Peek next char in stream

    // . . . Local vars
    typ_ReadChar NextChar;

    // . . . Check if no more data to read
	if ( s->Read(&NextChar,sizeof(NextChar)) < static_cast<int>(sizeof(NextChar)) ) NextChar = EndChar;

    #ifdef KEEPCOUNT_POSITION
      i_Position = -= sizeof(NextChar); // Restore stream state
    #endif
    s->Position -= sizeof(NextChar); // Restore stream state
    return NextChar;
}

//---------------------------------------------------------------------------
bool cls_StreamParser::EatLineEnd (class TStream* strSource, typ_ReadChar nxt)
{ // Eat line end (given eventual next char); return true if must end parsing
    // Surely if here line is ending
// . . . Manage maxline
    if ( ++row > MaxLine )
       {
        Log->Warn("MaxLine reached, ending parse");
        return true;
       }
// . . . Eat eventual next end line char
    if ( GetChar(strSource) == nxt ) GetChar(strSource);

// . . . Update state
    dcol = col = 0;
    if ( IsStrictDelims() ) CanReadNewNumber = true;

// . . . Finally
    return false;
}

//---------------------------------------------------------------------------
void __fastcall cls_StreamParser::EatLine (class TStream* strSource)
{ // Eat an entire line

    while ( (ReadChar!=EndChar) && !IsEOL(ReadChar) ) GetChar(strSource);
}

//---------------------------------------------------------------------------
String __fastcall cls_StreamParser::GetToken (class TStream* strSource)
{ // Get a space-char delimited string

    Token = "";
    // Eat eventual initial spaces
    while ( (ReadChar!=EndChar) && IsSpacingChar(ReadChar) ) GetChar(strSource);
    // Get string until a space or end-line is encountered
    while ( (ReadChar!=EndChar) && !IsSpacingChar(ReadChar) && !IsEOL(ReadChar) )
          {
           Token += ReadChar;
           GetChar(strSource);
          }
    return Token;
}

/*---------------------------------------------------------------------------
String __fastcall cls_StreamParser::GetLineString (class TStream* strSource)
{ // Get line delimited string

    Token = "";
    while ( (ReadChar!=EndChar) && !IsEOL(ReadChar) )
          {
           Token += ReadChar;
           GetChar(strSource);
          }
    return Token;
}  commented because at present noone uses this service */



//---------------------------------------------------------------------------
bool __fastcall cls_StreamParser::EatNumber ( class TStream* strSource )
{ // Jump a number, true if jumped one

   if ( IsStrictDelims() )
        { // is enough searching the first strict delimiter
         while ( ReadChar != StrictDelimiter )
               {
                if ( (ReadChar==EndChar) || IsEOL(ReadChar) ) return false;
                GetChar(strSource); // Next char
               }
         GetChar(strSource); // Eat delimiter
         return true;
        }
   else { // detect and pass a number literal token
         // . . . Eat all until number
         while ( !IsFloatLiteral(ReadChar) )
               {
                if ( (ReadChar==EndChar) || IsEOL(ReadChar) ) return false;
                GetChar(strSource); // Next char
               }
         // . . . Eat number
         while ( (ReadChar!=EndChar) && !IsEOL(ReadChar) && IsFloatLiteral(ReadChar) ) GetChar(strSource);
         return true;
        }
}

//---------------------------------------------------------------------------
double __fastcall cls_StreamParser::ReadNumber (class TStream* strSource)
{ // Read a float number literal // Contract: ReadChar is valorized

// . . . State variables
    double m; int s; // mantissa and sign
    int e, se; // exponent and sign
    double k; // shift of decimal part
    double value; // final value
    // Diagnostic
    bool i_found; // integer part found
    bool f_found; // fractional part found
    bool e_found; // exponential part found
    bool expchar_found; // exponential character found

// . . . State variables initialization
    m=0; s=1;
    e=0; se=1;
    i_found = false;
    f_found = false;
    e_found = false;
    expchar_found = false;

// . . . Get integer part sign
    if ( ReadChar == '-' ) {s = -1; GetChar(strSource);}
    else if ( ReadChar == '+' ) GetChar(strSource); // s = 1;

// . . . Get integer part value
    if ( ReadChar >= '0' && ReadChar <= '9' )
       {
        i_found = true;
        do {
            m = (10*m) + (ReadChar - '0');
            while ( GetChar(strSource) == ThousandSep ); // Skip a char
           }
        while ( ReadChar >= '0' && ReadChar <= '9' );
       }

// . . . Get decimal part
    if ( ReadChar == DecimalSep )
       {
        GetChar(strSource);
        k = .1;
        if ( ReadChar >= '0' && ReadChar <= '9' )
           {
            f_found = true;
            do {
                m += k*double(ReadChar - '0');
                k *= .1;
                GetChar(strSource);
               }
            while ( ReadChar >= '0' && ReadChar <= '9' );
           }
       }

// . . . Get exponential part
    if ( ReadChar == 'E' || ReadChar == 'e' )
       {
        expchar_found = true;
        GetChar(strSource);
        // sign
        if ( ReadChar == '-' ) {se = -1; GetChar(strSource);}
        else if ( ReadChar == '+' ) GetChar(strSource); // se = 1;
        // value
        if ( ReadChar >= '0' && ReadChar <= '9' )
           {
            e_found = true;
            do {
                e = (10*e) + (ReadChar - '0');
                GetChar(strSource);
               }
            while ( ReadChar >= '0' && ReadChar <= '9' );
           }
       }

// . . . Number finished: encountered a delimiter or no more chars
    // Calculate value
    if ( i_found || f_found )
         {
		  if ( e_found ) value = s*m*std::pow10(se*e); // All part given
          else if ( expchar_found && !Tolerant ) throw mat::EInvalidNumber(row,col,ReadChar,dcol);
          else value = s*m; // no exp part
         }
    else {
          if ( e_found )
               {
				value = s*std::pow10(se*e); // Only exponential
               }
          else {// No part at all
                if ( !Tolerant ) throw mat::EInvalidNumber(row,col,ReadChar,dcol);
                if (expchar_found) value = 1; // things like 'E,+E,-e,e+,E-,...'
                else value = 0; // things like '+,-,,...'
               }
         }

// . . . Finally
    return value;
}



//------------------------------- Auxiliary ---------------------------------

//---------------------------------------------------------------------------
void __fastcall cls_StreamParser::ProgressFeedbackInit (TStream* strSource)
{
    CharDeltaProgress = strSource->Size/100;
    if ( CharDeltaProgress <= 0 ) CharDeltaProgress = 2;
    ProgressFeedback(strSource);
}

//---------------------------------------------------------------------------
void __fastcall cls_StreamParser::ProgressFeedback (TStream* strSource)
{
    ProgressCount = Position(strSource)/CharDeltaProgress;
    if (OnProgress) OnProgress(ProgressCount,Abort);
}

//---------------------------------------------------------------------------
void cls_StreamParser::SaveState ( class TStream* strSource )
{ // Save parser state

    Saved_Position = Position(strSource);
    Saved_row = row;
    Saved_col = col;
    Saved_dcol = dcol;
    Saved_ReadChar = ReadChar;
}
//---------------------------------------------------------------------------
void cls_StreamParser::RestoreState ( class TStream* strSource )
{ // Restore saved state

    #ifdef KEEPCOUNT_POSITION
      i_Position = Saved_Position;
    #endif
    strSource->Position = Saved_Position;
    row = Saved_row;
    col = Saved_col;
    dcol = Saved_dcol;
    ReadChar = Saved_ReadChar;
}

//---------------------------------------------------------------------------
void cls_StreamParser::LogEndParsing ( const int pos ) const
{ // An end parsing log message
    Log->Entry("Parsing ended successfully",
               String("Parsed ") + String(pos) + String(" chars (") +
               String(row) + String(" lines) in ") + Log->GetTime());
}

//============================== Properties =================================

//---------------------------------------------------------------------------
void cls_StreamParser::set_ExpectedDelimiters (const String& s)
{ // Set expected data delimiters
    i_ExpectedDelimiters = String(EndChar) +
                           StrictDelimiter +
                           String("\v\f\n\r") +
                           s + String("\t\ ");
}


