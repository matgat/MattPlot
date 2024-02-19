//---------------------------------------------------------------------------
#ifndef unt_StreamParserH
#define unt_StreamParserH
//---------------------------------------------------------------------------

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    *
    *       unt_StreamParser
    *       2008 - Matteo Gattanini
    *
    *       cls_StreamParser  ver: 09    (aug.2008)
    *
    *       This unit defines an ancestor of a class hierarchy to
    *       parse data streams and fills a set of arrays of numbers.
    *
    *       Example of usage:
    *       #include "cls_StreamParser.h"
    *
    * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Header inclusions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include <Classes.hpp> // VCL classes like String,Exception

//-------------------------------- Settings ---------------------------------
#ifdef _DEBUG
  #define PARSER_DEBUG
#endif



/////////////////////////////////////////////////////////////////////////////
class cls_StreamParser
{
 public:
    // . Types definitions
    typedef char typ_ReadChar; // Character from stream
    typedef void __fastcall (__closure *fProgress)(int,bool&); // A progress function

    // . Public methods
    cls_StreamParser(class cls_Log*);
    //~cls_StreamParser();

    virtual bool Read(class TStream*,const String&) =0; // Parse text stream (appending additional lines), returns 'jobdone'

    static const typ_ReadChar EndChar = 0x04; // EOT

    // . Properties
    __property fProgress OnProgress = {read=i_OnProgress, write=i_OnProgress}; // OnProgress handler
    __property bool Abort = { read=i_Abort, write=i_Abort}; // When true, abort long task
    __property String ExpectedDelimiters = { read=i_ExpectedDelimiters, write=set_ExpectedDelimiters }; // expected data delimiters
    __property String Location = { read=get_Location }; // A string with current row and col

 protected:

    // . Protected attributes
    // . Class references
    class cls_Log* Log; // Owner's log

    // . State
    #ifdef KEEPCOUNT_POSITION
      // Position in stream is in Source->Position
      int i_Position; // Current character index from start stream
    #endif
    unsigned int row, col; // Current character line and index in line
    unsigned int dcol; // Data token column
    bool CanReadNewNumber; // Used to detect spaces in numbers when 'IsStrictDelims'
    typ_ReadChar ReadChar; // Read character
    String Token; // Current token bag

    // . Settings
    bool Tolerant; // number recognition tolerance
    typ_ReadChar StrictDelimiter; // Fussy expect a precise data delimiter
    typ_ReadChar DecimalSep, ThousandSep; // Local decimal and thousand separator
    unsigned int MaxLine; // Limit maximum parsed line (4294967295)

    // . Protected methods
    // . State
    inline bool IsStrictDelims() const {return StrictDelimiter != '\n';}
    inline int Position(class TStream* strSource) const
           { // Get current char position
            #ifdef KEEPCOUNT_POSITION
              return i_Position;
            #else
              return strSource->Position;
            #endif
           }
    inline int ProgressPos() const {return ProgressCount*CharDeltaProgress;}

    // . Parsing primitives
    inline typ_ReadChar GetChar( class TStream* s )
          { // Read a new char from stream
            #ifdef KEEPCOUNT_POSITION
              ++i_Position;
            #endif
            ++col; // Keep count of read char number
            // . Check if no more data to read
			if ( s->Read(&ReadChar,sizeof(ReadChar)) < static_cast<int>(sizeof(ReadChar)) ) ReadChar = EndChar;
			return ReadChar;
          }
    typ_ReadChar PeekChar (class TStream*); // Peek next char in stream
    bool EatLineEnd (class TStream*,typ_ReadChar); // Eat line end (given eventual next char)
    void __fastcall EatLine (class TStream*); // Eat an entire line
    String __fastcall GetToken (class TStream*); // Get a space-char delimited string
    //inline String GetLineString (class TStream*); // Get line delimited string
    bool __fastcall EatNumber (class TStream*); // Jump a number, true if jumped one
    double __fastcall ReadNumber(class TStream*); // Read a float number literal
    // Characters recognition
    //inline bool IsNumber(const typ_ReadChar c) const {return (c>='0'&&c<='9');} // Is a number char?
    inline bool IsFloatLiteral(const typ_ReadChar c) const {return ( (c>='0'&&c<='9')||c==DecimalSep||c=='+'||c=='-'||c=='E'||c=='e');}  // Is part of a float literal?
    inline bool IsEOL (const typ_ReadChar c) const {return (c=='\n'||c=='\r');}
    inline bool IsDelimiter(const typ_ReadChar c) const {for (int i=i_ExpectedDelimiters.Length(); i>=1; i--) if (i_ExpectedDelimiters[i] == c) return true;return false;}
    inline bool IsSpacingChar(const typ_ReadChar c) const {return (c=='\ '||c=='\t');}
    //inline bool IsStrangeDelimiter(const typ_ReadChar c) const {return ((c>='A'&&c<='Z')||(c>='a'&&c<='z'));}
    // Auxiliary
    void __fastcall ProgressFeedbackInit(TStream*);
    void __fastcall ProgressFeedback(TStream*);
    void SaveState (class TStream*); // Save parser state
    void RestoreState (class TStream*); // Restore saved state
    void LogEndParsing ( const int pos ) const; // An end parsing log message

    // . Properties
    fProgress i_OnProgress;
    bool i_Abort;
    String i_ExpectedDelimiters;
    void set_ExpectedDelimiters (const String&);
    String get_Location() const {return "Line " + String(row) + ", Column " + String(col);}

 private:

    // . Private attributes
    // . State
    // Saved with 'SaveState' and restored with 'RestoreState'
    unsigned int Saved_Position,Saved_row,Saved_col,Saved_dcol;
    typ_ReadChar Saved_ReadChar;

    // . Auxiliary
    int CharDeltaProgress; // every read char to update progress
    int ProgressCount; // progress updating counter

    // . Private methods


}; // 'cls_StreamParser'


//---------------------------------------------------------------------------
#endif
