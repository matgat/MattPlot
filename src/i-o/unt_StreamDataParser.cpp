#include <vcl.h>
#include "unt_MatCommon.h"
#include "unt_Log.h"
#pragma hdrstop
#include "unt_StreamDataParser.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


/////////////////////////////////////////////////////////////////////////////
////////////////////////////// cls_StreamDataParser /////////////////////////

//---------------------------------------------------------------------------
// Class constructor
cls_StreamDataParser::cls_StreamDataParser(class cls_Log* l) : cls_StreamParser(l)
{

// . Inputs

// . Defaults
    // . State (initialized at parsing)
    // . Settings
    Tolerant = false;
    ExpectedDelimiters = ";|";
    StrictDelimiter = '\n'; // disable strict delims
    DecimalSep = ::DecimalSeparator; // Local decimal separator
    ThousandSep = ::ThousandSeparator; // Local thousand separator
    MaxLine = 4294967295;

    // . Auxiliary
    i_Abort = false;
    i_OnProgress = 0;

// . Other things
    // A check
    if ( DecimalSep == ThousandSep ) Log->Error("Parser","Decimal and thousand separators are identical! (" + String(DecimalSep) + ") parsed numbers may be wrong");
}


//---------------------------------------------------------------------------
bool __fastcall cls_StreamDataParser::SeeComment (class TStream* strSource)
{ // If after the first cmt char there is a comment, eat it and return true

    typ_ReadChar nextch = PeekChar(strSource);
    if ( nextch == dcol::LineCmtChar )
         {
          GetChar(strSource); // consume next char
          EatLine(strSource);
          return true;
         }
    else if ( nextch == dcol::BlockCmtChar )
         {
          GetChar(strSource); // consume next char
          EatBlockComment(strSource);
          return true;
         }
    else return false; // Not a comment
}

//---------------------------------------------------------------------------
String __fastcall cls_StreamDataParser::GetLineStringUntilComment (class TStream* strSource)
{ // Get line string part before comment

    Token = "";
    while ( (ReadChar!=EndChar) && !IsEOL(ReadChar) )
          {
           if ( ReadChar == dcol::LineCmtChar )
                {
                 if ( SeeComment(strSource) ) return Token; // A comment found: break
                }
           // Collect line chars
           Token += ReadChar;
           // Next char
           GetChar(strSource);
          }
    return Token;
}

//---------------------------------------------------------------------------
void __fastcall cls_StreamDataParser::EatBlockComment (class TStream* strSource)
{ // Eat all until '*/' reached (or end of stream)

    while ( ReadChar != EndChar )
          {
           // . . . If encounter end block */, break
           if ( ReadChar == dcol::BlockCmtChar )
              {
               if ( GetChar(strSource) == dcol::LineCmtChar ) { GetChar(strSource); break; } // Eat and break
              }
           // . . . Keep count of lines
           else if ( ReadChar == '\r' ) EatLineEnd(strSource,'\n');
           else if ( ReadChar == '\n' ) EatLineEnd(strSource,'\r');
           // . . . Eat all meanwhile
           else GetChar(strSource);
          }
}


//---------------------------------------------------------------------------
void cls_StreamDataParser::PerformSplit(cls_Columns& cols, const mat::numlist& Split)
{ // Apply Split argument to session columns

    Log->SubEntry("Splitting according to these fractions: " + mat::NumlistToString(Split));

// . . . Put retrieved columns in a temporary container
    std::vector<class cls_Column*> RetrievedCols (cols.begin(),cols.end());

// . . . Prepare final columns container
    int FinalColsNumber =  Split.size() * RetrievedCols.size(); // Final columns number
    cols.clear(); // Now can reset session columns
    cols.reserve(FinalColsNumber);

// . . . For each retrieved column
    for (cls_Columns::typ_it c=RetrievedCols.begin(); c!=RetrievedCols.end(); ++c)
        {
         unsigned int idx = 0; // current index in overall column
         double rest = 0; // cumulative rest of splits size calculation
         Log->SubEntry( String("-- Splitting a column sized ") + (*c)->size() );

         // . . . For each split
         for ( unsigned int s=0; s<Split.size(); ++s )
             {
              // . . . Allocate splitted columns
              // Splitted column size
              double cs = (*c)->size() * Split[s];
              double ics; // integer part
			  double dcs = std::modf(cs,&ics); // decimal part (should be zero)
              unsigned int colsize = mat::round(ics);
              if ( !mat::IsZero(dcs) )
                   { // oops, got some rest!
                    if ( dcs > 0.5 ) { rest -= (1-dcs); ++colsize;}
                    else rest += dcs;
                    Log->Warn( String("Size of split ") + String(s) + String(" is forced to ") + String(colsize) + String(" instead of ") + String(cs) );
                    Log->SubEntry( String("It seems that total samples number is not divisible in the indicated parts") ,1);
                    Log->SubEntry( String("Check what\'s wrong, or if this rounding error it\'s what you want") ,1);
                   }
              else {
                    Log->Success( String("Size of split ") + String(s) + String(" is perfectly ") + String(colsize) );
                   }

              // . . . Prepare split container
              cols.push_back( new cls_Column(colsize) ); // will be deleted by 'Columns'
              // . . . Copy values in it
              // see end index
              unsigned int idx_end = idx + colsize;
              if ( idx_end > (*c)->size() ) idx_end = (*c)->size(); // Ensure in overall column
              while ( idx < idx_end ) cols.back()->push_back( (*c)->operator[](idx++) );
             }// end 'for each split'
         // . . . Finally
         delete (*c);
        }// end 'for each retrieved column'
}


//---------------------------------------------------------------------------
void cls_StreamDataParser::PerformResampling(cls_Columns& cols, const mat::cls_IndexSequence& Resample)
{ // Apply Resample argument to session columns

     Log->SubEntry("Resampling according to sequence:");
     Log->SubEntry(String(Resample));

// . . . For each retrieved column
    for (cls_Columns::typ_it c=cols.begin(); c!=cols.end(); ++c)
        {
         Log->SubEntry(String("Resampling column ") + (*c)->Label ,1);

         // . . . Mirror original samples and clear destination column
         std::vector<double> samples((*c)->begin(),(*c)->end());
         (*c)->clear();

         // . . . Check if must rescale sequence indexes
         mat::cls_IndexSequence seq = Resample;
         if ( int(samples.size()) != seq.MaxIndex() )
            {
             // . . . Must rescale
             Log->SubEntry(String("Resample sequence must be rescaled for this column") ,2);
             seq.Rescale( samples.size() );
             Log->SubEntry(String("Rescaled sequence is ")+String(seq), 2);
             // . . . Rounding errors check: column sizes must be divisible by min size
             if ( samples.size() % seq.MaxIndex() )
                {
                 Log->Warn(String("This column is sized ") + String(samples.size()) + String(", not divisible by ") + String(seq.MaxIndex()) + String(", so rounding errors on indexes may occur"));
                }
            }

         // . . . Prepare destination container
         (*c)->reserve( seq.Dimension() );
         //Log->SubEntry(String("Column ") + (*c)->Label + String(" had seq sized ") + String(seq.Dimension()) ,1);
         // . . . For each resample interval
         for ( unsigned int ri=0; ri<seq.size(); ++ri )
             {
              // TODO 5: interpolation function with fractional steps
              if ( seq[ri].step < 1 )
                 {
                  Log->Warn("Fractional steps not allowed");
                  seq[ri].step = 1;
                 }
              try {
                   if (seq[ri].step>0) for ( int i=seq[ri].top; i<seq[ri].end; i+=seq[ri].step ) (*c)->push_back( samples.at(i) );
                   else for ( int i=seq[ri].top; i>seq[ri].end; i+=seq[ri].step ) (*c)->push_back( samples.at(i) );
                  }
              catch(std::out_of_range& e)
                  {
                   throw(Exception("In reordering column '" + (*c)->Label + "' at interval " +String(ri)+ " found an invalid index: check order sequence"));
                  }
             }// end 'for each resample interval'

         // . . . Finally
         if ( seq.Dimension()!=samples.size() ) Log->SubEntry(String("Column ") + (*c)->Label + String(" had change its size to ") + String(seq.Dimension()) ,1);
        }// end 'for each column'
}




/////////////////////////////////////////////////////////////////////////////
////////////////////////// cls_StreamTextTableParser ////////////////////////

/*  +--------------------+     . Directives, Comments, warning for all other
    |  Declarative part  |     . First data line detection
    +--------------------+ -
    |                    |     . Data collection
    |     Data part      |     . Comments, error on all other things
    |        ...         |     . Post elaboration
    +--------------------+

    Comments:     // and /*
    Directives:
               #header <x><y(x)><z(x)>
               #resample (20:30)(0:19)(31:100)
               #split |1|2|1|
               #tolerant true
               #title Current and speed acquisition
               #decimalseparator .
               #strictdelimiter ;
               #thousandseparator ,
               #maxline 1000
               #end
          Other?
               #sort // force sort points respect abscissa
               #define MacroName macroexpansion // define a macro

 ---- Examples

    * Headers
      #header <t[0.5ms+1.2]> <v(t)[V]> <x> <y(x)> <A[.1]>
      #header <i1 [A]> <i2 [A]> < P(i1) [.001 KW]>  <v(i2) [V]>
      // Re and Im for polar plots
      #header <w [rad/s]> <Re{A(w) [Db]}> <Im{A(w) [Db]}>

    * Resample
      #resample (20:30)(0:19)(31:end)
      #resample (end:2:0) // undersample and resorting
      #resample (0:0.5:end) // linear/cubic resampling

    * Split according to header (one column only)
      {x([.5ms])} {y([2ms])} {z([1ms])}
      123 \
      456  \
      789  / x
      123 /
      456 > y
      789 \
      123 / z                                                              */



//---------------------------------------------------------------------------
// Constructor
cls_StreamTextTableParser::cls_StreamTextTableParser(class cls_Log* l) : cls_StreamDataParser(l)
{

}



//---------------------------------------------------------------------------
bool cls_StreamTextTableParser::Read (TStream* strSource, const String& AddedLines)
{ // Parse text stream (appending additional lines); returns false if fails

// . Preparation
    ExtLines = AddedLines;
    ExtLinesParsed = false;
    // . Initialize global session quantities
    State = ST_SESSION_START;
    //ReadChar; // does not need initialization
    // Current character coordinates
    #ifdef KEEPCOUNT_POSITION
      i_Position = 0;
    #endif
    row = 1; // Current line
    col = 0;

    // . Progress feedback: progress update is equal to percentile
    ProgressFeedbackInit(strSource);

// . Start parsing
    Log->StartTime();
    GetChar(strSource); // Important: read first char

// . Data sessions
    while ( State != ST_ABORT && State != ST_TERMINATE ) SessionStateMachine(strSource);
    if ( State == ST_ABORT )
       {
        Log->Warn("Parsing aborted by user");
        return false;
       }

// . Final checks

// . Operations on overall columns
    try {
         Columns.ResolveLinks ( Log ); // Column links
         Columns.Summary ( Log );
        }
    catch ( Exception &e )
        {
         throw mat::EParsingError(e.Message, row, col);
        }

// . Finally
    Log->SubEntry("Nothing more to do");
    LogEndParsing (Position(strSource));
    return true;
}



//---------------------------------------------------------------------------
void cls_StreamTextTableParser::SessionStateMachine ( class TStream* strSource )
{ // Session data state machine, return false when finished

// . Local variables
    TStringStream* strAppendedLines;

// . State machine
    switch ( State ) // What to do?
       {
        case ST_SESSION_START : // . Start parsing

            Log->SubEntry("");
            Log->SubEntry(" =========== Starting a new data session");
            // . Reset for new session
            Header.Reset();
            Resample.clear();
            Split.clear();

            // . Next state: see if must parse added external lines
            if ( !ExtLinesParsed && !ExtLines.IsEmpty() ) State = ST_EXTERN_PART;
            else State = ST_HEADING_PART;
        break;

        case ST_EXTERN_PART : // . Parse extern appended lines

            Log->SubEntry("Parsing external added lines");

            SaveState(strSource);

            strAppendedLines = new TStringStream(ExtLines);
            GetChar(strAppendedLines); // Important: read first char
            try {
                 try { ParseHeading(strAppendedLines); } // Abort temporarily suspended
                 catch (Exception& e) {Log->Error("Invalid added lines: " + e.Message);}
                }
            __finally { delete strAppendedLines; ExtLinesParsed=true; }

            RestoreState(strSource);

            // . Next state
            State = ST_HEADING_PART;
        break;

        case ST_HEADING_PART : // . Parsing the heading part

            // . Next state
            State = ParseHeading(strSource);
        break;

        case ST_HEADING_END : // . Heading ended

            // . Now we're in first char of data part
            // Allocate columns container confronting header with first data row
            AnalyzeFirstDataRow(strSource);

            // . Next state
            State = ST_DATA_PART;
        break;

        case ST_DATA_PART : // . Data part

            Log->SubEntry("Parsing data part");
            // . Reset auxiliary vars for parsing data
            dcol = 0; // Data token column
            CanReadNewNumber = true;

            // . Next state
            State = ParseData(strSource);
        break;

        case ST_SESSION_END : // . Data part ended with other text

            // . Postprocessing
            Log->SubEntry("Data ended, postprocessing...");

            // . Splitting
            if ( Split.size() > 0 ) PerformSplit(TempCols, Split);

            // . Copy header informations (columns must be already splitted)
            if ( Header.Declared )
               {
                // After split I'm sure to have exact correspondence between header and columns
                if ( TempCols.size()==Header.size() )
                   { // When header is declared and its columns match with data can safely copy 'cls_ColumnDeclaration' data
                    for ( unsigned int i=0; i<TempCols.size(); ++i ) TempCols[i]->Assign( Header[i] );
                   }
                else throw(mat::EParsingError(String("Declared columns (") + String(Header.size()) +
                                              String(") does not match actual colums after split (")+
                                              String(TempCols.size()) + String(")"), row, col));
               }

            // . Rescaling (must be copied header info)
            Log->SubEntry(String("Rescaling/casting operation check..."));
            for (cls_Columns::typ_it c=TempCols.begin(); c!=TempCols.end(); ++c)
                {
                 // TODO 3: put progress here
                 if ( (*c)->HasDataType() )
                    {
                     Log->SubEntry(String("Casting column ") + (*c)->Label, 1);
                     (*c)->CastToDataType();
                    }
                 if ( (*c)->MustBeScaled() )
                   {
                    Log->SubEntry(String("Rescaling column ") + (*c)->Label, 1);
                    (*c)->ApplyTransform();
                   }
                }

            // . Resampling (indexes must be already rescaled)
            if ( Resample.size() > 0 ) PerformResampling(TempCols, Resample);

            // . Pass retrieved columns to main container
            for (unsigned int i=0; i<TempCols.size(); ++i) Columns.push_back( TempCols[i] );
            TempCols.clear(); // Important, otherwise he'll try to delete pointers

            // . Next state
            if ( ReadChar != EndChar ) State = ST_SESSION_START;
            else State = ST_TERMINATE;
        break;

        default : // . End of stream
            ;//Log->SubEntry("End");

       }// end 'switch State'
}

//---------------------------------------------------------------------------
cls_StreamDataParser::en_ParseState cls_StreamTextTableParser::ParseHeading (class TStream* strSource)
{ // Parse heading part

    Log->SubEntry("Parsing heading part");
    while ( !IsFloatLiteral(ReadChar) && (ReadChar!=EndChar) )
          {
           #ifdef PARSER_DEBUG
             //Log->SubEntry("found " + String(ReadChar));
           #endif
           // . Process directives
           if ( ReadChar == dcol::DirectivePrefix ) ReadDirective(strSource);
           // . Eat spaces
           else if ( IsSpacingChar(ReadChar) ) GetChar(strSource);
           // . Eat comments
           else if ( ReadChar == dcol::LineCmtChar )
                   {
                    if ( !SeeComment(strSource) ) Log->Warn(String("Single comment char at line ") + String(row) + String(", char ") + String(col));
                   }
           // . Line end
           else if ( ReadChar == '\r' ) {if (EatLineEnd(strSource,'\n')) return ST_TERMINATE;}
           else if ( ReadChar == '\n' ) {if (EatLineEnd(strSource,'\r')) return ST_TERMINATE;}
           // . Other: warn
           else Log->Warn("Misplaced text in heading part: " + GetToken(strSource));
           //else throw EInvalidToken(row, col, ReadChar, GetToken(strSource));

           // . Progress feedback
           if ( Position(strSource) >= ProgressPos() )
              {
               ProgressFeedback(strSource);
               if (Abort) return ST_ABORT;
              }
          }

// . If here, heading part ended
    return ST_HEADING_END;
}

//---------------------------------------------------------------------------
cls_StreamDataParser::en_ParseState cls_StreamTextTableParser::ParseData (class TStream* strSource)
{ // Parse data part
  // Contract: follows 'ParseHeading', so ReadChar is valorized

    //throw ESyntaxError(row, col, ReadChar);
    //EInvalidNumber(row, col, ReadChar);
    while ( ReadChar != EndChar )
          {
           // . Eat number
           if ( IsFloatLiteral(ReadChar) )
                {
                 // Columns number check
                 if ( dcol >= TempCols.Count ) throw mat::EUnmatchingData(row, col, TempCols.Count);
                 // Read number
                 Value = ReadNumber(strSource);
                 // Remark: number elaboration is applied during postprocessing

                 // Then add parsed number current column
                 if ( IsStrictDelims() )
                      {// Fussy delimiters: spaces between numbers are not allowed, so
                       //                   must detect value overwriting and signal error
                       if ( CanReadNewNumber ) CanReadNewNumber = false; // I'm filling this slot
                       else throw mat::EInvalidNumber(row,col,ReadChar,dcol);

                       TempCols[dcol]->push_back( Value );
                       // dcol is updated when delimiter is encountered
                      }
                 else {// Sloppy delimiters
                       TempCols[dcol]->push_back( Value );
                       // Update here data column index
                       ++dcol;
                      }
                 #ifdef PARSER_DEBUG
                     if ( strSource->Size < 5000 ) // skip when trying large data
                        { int c = IsStrictDelims() ? dcol : dcol-1;
                          Log->SubEntry("Added number " + String(TempCols.at(c)->back()) + " to column " + String(c));}
                 #endif
                }
           // . Line end
           else if ( ReadChar == '\r' )
                   {
                    if ( col < 1 ) Log->Warn(String("Line ") + String(row) + String(" in data part is empty"));
                    if (EatLineEnd(strSource,'\n')) return ST_TERMINATE;
                   }
           else if ( ReadChar == '\n' )
                   {
                    if ( col < 1 ) Log->Warn(String("Line ") + String(row) + String(" in data part is empty"));
                    if (EatLineEnd(strSource,'\r')) return ST_TERMINATE;
                   }
           // . Eat comments
           else if ( ReadChar == dcol::LineCmtChar )
                {
                 if ( SeeComment(strSource) ) Log->Warn("Found a comment in data part");
                 else throw mat::EInvalidNumber(row,col,ReadChar,dcol);
                }
           // . Directives cause session end
           else if ( ReadChar == dcol::DirectivePrefix ) return ST_SESSION_END;
           // . Other: must be a data delimiter or spaces
           else {
                 if ( IsStrictDelims() )
                      {
                       if ( ReadChar == StrictDelimiter )
                            {
                             GetChar(strSource); // Eat delimiter
                             ++dcol; // Update data column
                             CanReadNewNumber = true;
                            }
                       else if ( IsSpacingChar(ReadChar) ) GetChar(strSource); // Eat spaces
                       else throw mat::EInvalidNumber(row,col,ReadChar,dcol);
                      }
                 else {
                       if ( IsDelimiter(ReadChar) ) GetChar(strSource); // Eat delimiter
                       else throw mat::EInvalidNumber(row,col,ReadChar,dcol);
                      }
                }

           // . Progress feedback
           if ( Position(strSource) >= ProgressPos() )
              {
               ProgressFeedback(strSource);
               if (Abort) return ST_ABORT;
              }
          }

// . If here, data part ended
    return ST_SESSION_END;
}


//------------------------------ Submodules ---------------------------------
// Contract: ReadChar is valorized, and at return points to next char



//---------------------------------------------------------------------------
void cls_StreamTextTableParser::ReadDirective ( class TStream* strSource )
{ // Read and process directive

// . Get directive name and argument
    GetChar(strSource); // Skip directive prefix
    String DirectiveName = GetToken(strSource).Trim().LowerCase();
    String DirectiveArg = GetLineStringUntilComment(strSource).Trim();
    ProcessDirective ( DirectiveName, DirectiveArg );
}

//---------------------------------------------------------------------------
void cls_StreamTextTableParser::ProcessDirective ( const String& DirectiveName, const String& DirectiveArg)
{ // Process directive

// . See directive
    try {
         if ( DirectiveName == dcol::STR_HEADER )
              {
               if ( Header.Declared ) Log->Warn("Ignoring multiple header " + DirectiveArg);
               else {
                     Log->SubEntry("Processing header: " + DirectiveArg);
                     Header.RetrieveFrom(DirectiveArg);
                     Header.Summary(Log);
                    }
              }
         else if ( DirectiveName == dcol::STR_RESAMPLE )
              {
               Log->SubEntry("Found resample directive: " + DirectiveArg);
               Resample = DirectiveArg;
               /* Here could resolve some symbolic identifiers
                  String ret, resolved_strseq;
                  TReplaceFlags ReplOpts = TReplaceFlags() << rfReplaceAll << rfIgnoreCase;
                  resolved_strseq = ::StringReplace(strseq, top, "0", ReplOpts);
                  resolved_strseq = ::StringReplace(resolved_strseq, end, String(LongestColSize-1), ReplOpts);
                  //resolved_strseq = ::StringReplace(resolved_strseq, "half", String(SizeMax/2), ReplOpts);
               */
              }
         else if ( DirectiveName == dcol::STR_SPLIT )
              {
               Log->SubEntry("Found split directive: " + DirectiveArg);
               mat::StringToNumlist(Split, DirectiveArg);
               // Normalize values
               double spl_sum = mat::sum(Split);
               for ( unsigned int i=0; i<Split.size(); ++i ) Split[i] /= spl_sum;
              }
         else if ( DirectiveName == dcol::STR_TITLE )
              {
               Log->SubEntry("Setting title: " + DirectiveArg);
               Columns.Title = DirectiveArg;
              }
         else if ( DirectiveName == dcol::STR_XLABEL )
              {
               Log->SubEntry("Setting x label: " + DirectiveArg);
               Columns.Xlabel = DirectiveArg;
              }
         else if ( DirectiveName == dcol::STR_YLABEL )
              {
               Log->SubEntry("Setting y label: " + DirectiveArg);
               Columns.Ylabel = DirectiveArg;
              }
         else if ( DirectiveName == dcol::STR_XMARKER )
              {
               Log->SubEntry("Setting x marker: " + DirectiveArg);
               cls_Columns::stu_Marker m;
               m.Value = DirectiveArg.ToDouble();
               m.Vertical = true;
               Columns.Markers.push_back(m);
              }
         else if ( DirectiveName == dcol::STR_YMARKER )
              {
               Log->SubEntry("Setting y marker: " + DirectiveArg);
               cls_Columns::stu_Marker m;
               m.Value = DirectiveArg.ToDouble();
               m.Vertical = false;
               Columns.Markers.push_back(m);
              }
         else if ( DirectiveName == dcol::STR_TEXT )
              {
               Log->SubEntry("Setting a text label: " + DirectiveArg);
               mat::numlist coords;
               mat::StringToNumlist ( coords, mat::Inner(DirectiveArg.Trim()) );
               if ( coords.size() != 2 ) throw Exception("For a text label you must provide coordinates and text, ex: #text (0.1;2.1) hello");
               cls_Columns::stu_Label l;
               l.x = coords[0];
               l.y = coords[1];
               l.Text = mat::OuterDx(DirectiveArg.Trim()).Trim();
               Log->SubEntry("label coords: " + String(l.x) + String(";") + String(l.y),1);
               Columns.Labels.push_back(l);
               /*
               mat::stringlist DirectiveArgs;
               mat::Tokenize(DirectiveArgs, DirectiveArg.c_str(), ' ');
               if ( DirectiveArgs.size() != 3 ) throw Exception("");
               */
              }
         else if ( DirectiveName == dcol::STR_TOLERANT )
              {
               Log->SubEntry("Setting tolerant flag: " + DirectiveArg);
               Tolerant = mat::StringToBool(DirectiveArg);
              }
         else if ( DirectiveName == dcol::STR_DECSEP )
              {
               Log->SubEntry("Setting decimal separator: " + DirectiveArg);
               DecimalSep = mat::StringToChar(DirectiveArg);
              }
         else if ( DirectiveName == dcol::STR_THOUSEP )
              {
               Log->SubEntry("Setting thousand separator: " + DirectiveArg);
               if ( DirectiveArg.IsEmpty() ) ThousandSep = mat::InvalidChar; // Disable
               else ThousandSep = mat::StringToChar(DirectiveArg);
              }
         else if ( DirectiveName == dcol::STR_STRICTDEL )
              {
               Log->SubEntry("Setting strict delimiter: " + DirectiveArg);
               StrictDelimiter = mat::StringToChar(DirectiveArg);
              }
         else if ( DirectiveName == dcol::STR_MAXLINES )
              {
               Log->SubEntry("Setting maximum lines: " + DirectiveArg);
               MaxLine = ::StrToInt(DirectiveArg);
              }
         else if ( DirectiveName == dcol::STR_END )
              {
               Log->SubEntry("End directive encountered" );
               // TODO 4: #end does not work
               State = ST_TERMINATE;
              }
         else {
               //throw mat::EParsingError(String("Unknown directive"), row, col);
               Log->Warn("Skipping unknown directive " + DirectiveName + String(" at line ") + String(row));
              }
        }
    catch ( Exception &e )
        {
         throw mat::EParsingError(String("Invalid directive argument: ")+e.Message, row, col);
        }
}

//---------------------------------------------------------------------------
void cls_StreamTextTableParser::AnalyzeFirstDataRow ( class TStream* strSource )
{ // Analyze the first data row allocating columns container

// . Save state
    SaveState(strSource);

// . Count data columns
    TempCols.Count = 0;
    while ( EatNumber(strSource) ) ++TempCols.Count;
    if ( IsStrictDelims() ) ++TempCols.Count; // Consider also last number
    // A check
    if ( TempCols.Count <= 0 ) throw mat::EParsingError("No data columns found", row, col);

// . Compare with declared data if present
    if ( Header.Declared )
       {
        // Assure that data column match declared columns number or at least be one
        if ( TempCols.Count == Header.size() )
             {
              Log->Success(String("Found ") + String(TempCols.Count) + String(" data columns as expected"));
             }
        else {
              Log->Warn(String("Found ") + String(TempCols.Count) + String(" columns, while Header declares ") + String(Header.size()));

              // Stop if unmatching, except when one data column splitted by header scaling
              if ( TempCols.Count == 1 )
                   {
                    try {
                         String SplArg = Header.BuildSplitArg();
                         Log->SubEntry(String("This single column will be splitted according to Header: " + SplArg));
                         ProcessDirective(dcol::STR_SPLIT, SplArg);
                        }
                    catch (Exception& e) {throw(mat::EParsingError(e.Message, row, col));}
                   }
              else {
                    throw (mat::EUnmatchingData(row, col, Header.size()));
                   } 
             }
       }

// . Allocate session data container
    TempCols.clear(); // Assure empty
    TempCols.reserve(TempCols.Count);
    // Estimate each column size
    unsigned int colsize = strSource->Size / (TempCols.Count * 16); // 16 chars per literal
    for ( unsigned int i=0; i<TempCols.Count; ++i )
        {
         TempCols.push_back( new cls_Column(colsize) ); // will be deleted by 'Columns'
        }

/*
// . Handle header informations
    if ( Header.Declared && TempCols.Count==Header.size() )
         { // When header is declared and its columns match with data
           // can safely copy 'cls_ColumnDeclaration' data
          for ( unsigned int i=0; i<TempCols.Count; ++i ) TempCols[i]->Assign( Header[i] );
         }
    //else an unique column will be splitted: this operation will be performed at split ending
*/

// . Restore state
    RestoreState(strSource);
}


/////////////////////////////////////////////////////////////////////////////
//////////////////////////// cls_StreamCsvParser ////////////////////////////

/*
    Item,VQ 2900,VB 2900,VB 2901,INPUT 21
    ,,,,
    ,Mach.Sys.LiftLocks,Mach.AxesAtRest,Mach.Cnc.Axes.Still,IFC_BRDG_PARKED
    ,,,,
    0, 38208705, 11826902, 1185356, 107941
    1, 38129011, 12081360, 1186640, 108285
    2, 38047621, 12335287, 1187924, 108636


    Item,ASSE 1,ASSE 2,ASSE 1,ASSE 2
    ,Velocita Teorica [mm/min],Velocita Teorica [mm/min],Quota Reale Assoluta [mm],Quota Reale Assoluta [mm]
    ,,,,
    ,,,,
    0, 38208705, 11826902, 1185356, 107941
    1, 38129011, 12081360, 1186640, 108285
    2, 38047621, 12335287, 1187924, 108636
*/

//---------------------------------------------------------------------------
// Constructor
cls_StreamCsvParser::cls_StreamCsvParser(class cls_Log* l) : cls_StreamDataParser(l)
{
    Header = new TStringList();
}
//---------------------------------------------------------------------------
// Destructor
cls_StreamCsvParser::~cls_StreamCsvParser()
{
    delete Header;
}


//---------------------------------------------------------------------------
// Parse text stream (appending additional lines); returns false if fails
bool cls_StreamCsvParser::Read (TStream* strSource, const String& AddedLines)
{
// . Preparation
    ExtLines = AddedLines;
    ExtLinesParsed = false;

    // Current character coordinates
    #ifdef KEEPCOUNT_POSITION
      i_Position = 0;
    #endif
    row = 1; // Current line
    col = 0;

    // . Progress feedback: progress update is equal to percentile
    ProgressFeedbackInit(strSource);

// . Start parsing
    Log->StartTime();
    GetChar(strSource); // Important: read first char

// . Data sessions
    while ( State != ST_ABORT && State != ST_TERMINATE ) SessionStateMachine(strSource);
    if ( State == ST_ABORT )
       {
        Log->Warn("Parsing aborted by user");
        return false;
       }

// . Operations on overall columns
    try {
         Columns.ResolveLinks ( Log ); // Column links
         Columns.Summary ( Log );
        }
    catch ( Exception &e )
        {
         throw mat::EParsingError(e.Message, row, col);
        }

// . Finally
    Log->SubEntry("Nothing more to do");
    LogEndParsing (Position(strSource));
    return true;
}

//---------------------------------------------------------------------------
void cls_StreamCsvParser::SessionStateMachine ( class TStream* strSource )
{ // Session data state machine, return false when finished

// . Local variables
    TStringStream* strAppendedLines;

// . State machine
    switch ( State ) // What to do?
       {
        case ST_SESSION_START : // . Start parsing

            Log->SubEntry("");
            Log->SubEntry(" =========== Starting a new data session");

            // . Next state: see if must parse added external lines
            if ( !ExtLinesParsed && !ExtLines.IsEmpty() ) State = ST_EXTERN_PART;
            else State = ST_HEADING_PART;
        break;

        case ST_EXTERN_PART : // . Parse extern appended lines

            Log->SubEntry("Parsing external added lines");

            SaveState(strSource);

            strAppendedLines = new TStringStream(ExtLines);
            GetChar(strAppendedLines); // Important: read first char
            try {
                 try { ParseHeading(strAppendedLines); } // Abort temporarily suspended
                 catch (Exception& e) {Log->Error("Invalid added lines: " + e.Message);}
                }
            __finally { delete strAppendedLines; ExtLinesParsed=true; }

            RestoreState(strSource);

            // . Next state
            State = ST_HEADING_PART;
        break;

        case ST_HEADING_PART : // . Parsing the heading part

            // . Next state
            State = ParseHeading(strSource);
        break;

        case ST_HEADING_END : // . Heading ended

            // . Now we're in first char of data part
            // Allocate columns container confronting header with first data row
            //AnalyzeFirstDataRow(strSource);

            // . Next state
            State = ST_DATA_PART;
        break;

        case ST_DATA_PART : // . Data part

            Log->SubEntry("Parsing data part");
            // . Reset auxiliary vars for parsing data
            dcol = 0; // Data token column
            CanReadNewNumber = true;

            // . Next state
            State = ParseData(strSource);
        break;

        case ST_SESSION_END : // . Data part ended with other text

            // . Postprocessing
            Log->SubEntry("Data ended, postprocessing...");

            // . Copy header informations
            for ( int i=0; i<TempCols.size() && i<Header->Count; ++i ) TempCols[i]->Label = Header->Strings[i];

            // . Pass retrieved columns to main container
            for (unsigned int i=0; i<TempCols.size(); ++i) Columns.push_back( TempCols[i] );
            TempCols.clear(); // Important, otherwise he'll try to delete pointers

            // . Next state
            if ( ReadChar != EndChar ) State = ST_SESSION_START;
            else State = ST_TERMINATE;
        break;

        default : // . End of stream
            ;//Log->SubEntry("End");

       }// end 'switch State'
}



//---------------------------------------------------------------------------
cls_StreamDataParser::en_ParseState cls_StreamCsvParser::ParseHeading (class TStream* strSource)
{ // Parse heading part

    Log->SubEntry("Parsing heading part");
    while ( !IsFloatLiteral(ReadChar) && (ReadChar!=EndChar) )
          {
           #ifdef PARSER_DEBUG
             //Log->SubEntry("found " + String(ReadChar));
           #endif
           // . Eat spaces
           if ( IsSpacingChar(ReadChar) ) GetChar(strSource);
           // . Eat comments
           else if ( ReadChar == dcol::LineCmtChar )
                   {
                    if ( !SeeComment(strSource) ) Log->Warn(String("Single comment char at line ") + String(row) + String(", char ") + String(col));
                   }
           // . Line end
           else if ( ReadChar == '\r' ) {if (EatLineEnd(strSource,'\n')) return ST_TERMINATE;}
           else if ( ReadChar == '\n' ) {if (EatLineEnd(strSource,'\r')) return ST_TERMINATE;}
           // . Other: warn
           else Log->Warn("Misplaced text in heading part: " + GetToken(strSource));
           //else throw EInvalidToken(row, col, ReadChar, GetToken(strSource));

           // . Progress feedback
           if ( Position(strSource) >= ProgressPos() )
              {
               ProgressFeedback(strSource);
               if (Abort) return ST_ABORT;
              }
          }

// . If here, heading part ended
    return ST_HEADING_END;
}

//---------------------------------------------------------------------------
cls_StreamDataParser::en_ParseState cls_StreamCsvParser::ParseData (class TStream* strSource)
{ // Parse data part
  // Contract: follows 'ParseHeading', so ReadChar is valorized

    //throw ESyntaxError(row, col, ReadChar);
    //EInvalidNumber(row, col, ReadChar);
    while ( ReadChar != EndChar )
          {
           // . Eat number
           if ( IsFloatLiteral(ReadChar) )
                {
                 // Columns number check
                 if ( dcol >= TempCols.Count ) throw mat::EUnmatchingData(row, col, TempCols.Count);
                 // Read number
                 Value = ReadNumber(strSource);
                 // Remark: number elaboration is applied during postprocessing

                 // Then add parsed number current column
                 if ( IsStrictDelims() )
                      {// Fussy delimiters: spaces between numbers are not allowed, so
                       //                   must detect value overwriting and signal error
                       if ( CanReadNewNumber ) CanReadNewNumber = false; // I'm filling this slot
                       else throw mat::EInvalidNumber(row,col,ReadChar,dcol);

                       TempCols[dcol]->push_back( Value );
                       // dcol is updated when delimiter is encountered
                      }
                 else {// Sloppy delimiters
                       TempCols[dcol]->push_back( Value );
                       // Update here data column index
                       ++dcol;
                      }
                 #ifdef PARSER_DEBUG
                     if ( strSource->Size < 5000 ) // skip when trying large data
                        { int c = IsStrictDelims() ? dcol : dcol-1;
                          Log->SubEntry("Added number " + String(TempCols.at(c)->back()) + " to column " + String(c));}
                 #endif
                }
           // . Line end
           else if ( ReadChar == '\r' )
                   {
                    if ( col < 1 ) Log->Warn(String("Line ") + String(row) + String(" in data part is empty"));
                    if (EatLineEnd(strSource,'\n')) return ST_TERMINATE;
                   }
           else if ( ReadChar == '\n' )
                   {
                    if ( col < 1 ) Log->Warn(String("Line ") + String(row) + String(" in data part is empty"));
                    if (EatLineEnd(strSource,'\r')) return ST_TERMINATE;
                   }
           // . Eat comments
           else if ( ReadChar == dcol::LineCmtChar )
                {
                 if ( SeeComment(strSource) ) Log->Warn("Found a comment in data part");
                 else throw mat::EInvalidNumber(row,col,ReadChar,dcol);
                }
           // . Directives cause session end
           else if ( ReadChar == dcol::DirectivePrefix ) return ST_SESSION_END;
           // . Other: must be a data delimiter or spaces
           else {
                 if ( IsStrictDelims() )
                      {
                       if ( ReadChar == StrictDelimiter )
                            {
                             GetChar(strSource); // Eat delimiter
                             ++dcol; // Update data column
                             CanReadNewNumber = true;
                            }
                       else if ( IsSpacingChar(ReadChar) ) GetChar(strSource); // Eat spaces
                       else throw mat::EInvalidNumber(row,col,ReadChar,dcol);
                      }
                 else {
                       if ( IsDelimiter(ReadChar) ) GetChar(strSource); // Eat delimiter
                       else throw mat::EInvalidNumber(row,col,ReadChar,dcol);
                      }
                }

           // . Progress feedback
           if ( Position(strSource) >= ProgressPos() )
              {
               ProgressFeedback(strSource);
               if (Abort) return ST_ABORT;
              }
          }

// . If here, data part ended
    return ST_SESSION_END;
}


