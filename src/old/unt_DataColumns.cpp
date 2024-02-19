
//---------------------------------------------------------------------------
#include <vcl.h>
#include "unt_MatCommon.h"
#include "unt_Log.h"
#pragma hdrstop
#include "unt_DataColumns.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


//====================== Useful file scope declarations =====================

//============================= Static members ==============================

//============================= Public methods ==============================

//---------------------------------------------------------------------------
cls_DataColumns::cls_DataColumns(class cls_Log* l) : Log(l)
{ // Class constructor

// . . . Defaults
        colcount = xcount = 0;
        firstxcol = -1; // First column abscissa idx

        // . . . Header chars
        Brackets_Column = "«»";
        Brackets_Abscissa = "()";
        Brackets_Unit = "[]";
        Prefix_Directive = '#';

// . . . Other initializations
}

//---------------------------------------------------------------------------
//cls_DataColumns::~cls_DataColumns()
//{ // Class destructor
//        //
//}
//---------------------------------------------------------------------------





//---------------------------------------------------------------------------
String __fastcall cls_DataColumns::ImportFrom(TStrings* Lines,const mat::stringlist& GivenStrDirs)
{ // Import columns from strings

// . . . A preliminary stuff
        if ( Lines->Count < 1 ) return "No lines to read";
        Directives.clear();

// . . . Which directives have you given to me?
        //cls_Directives GivenDirs = DecodeDirectives(GivenStrDirs);
        //cls_Directives CollectedDirs;

// . . . Read data heading part (text lines)

        // . . . Some variables
        int line=0; // current line counter
        String Line; // current line
        bool IsDataReached = false; // Is data block reached?
        bool IsHeaderFound = false; // Is a declaration header found?
        mat::stringlist Tokens; // An auxiliary list of strings

        // . . . Now I'll search for an header and directives
        do {
            // Get a line
            Line = Lines->Strings[line].TrimLeft();

            // Ignore empty lines
            if ( Line.IsEmpty() )
               {
                Log->SubEntry("line " + String(line)+ " : ignoring empty line");
                continue;
               }

            // A number means data block reached
            IsDataReached = mat::IsFloat(Line[1]);

            // Text lines
            if ( !IsDataReached ) // Line starts with a not-a-number char
               {
                // Is it a directive?
                if ( Line[1] == Prefix_Directive)
                   {// Collect the directive; it will be executed once columns are filled
                    Directives.push_back(Line);
                   }
                else
                   {// Not a directive. Is it an header?
                    String Res = mat::GetDelimitedSubStrings(Line,Brackets_Column,Tokens);
                    if ( Res.IsEmpty() )
                       {// An header found
                        if ( IsHeaderFound )
                           {
                            Log->Error("Multiple headers defined! Check your file");
                            return "Only one header allowed!";
                           }
                        else
                          {
                           Log->SubEntry("line " +String(line)+ " : header found");
                           IsHeaderFound = true;
                           DecodeHeader(Tokens);
                          }
                       }
                    else
                       {// Not a valid header
                        Log->SubEntry("line " +String(line)+ " : not an header");
                        Log->SubEntry(Res,1);
                       }
                   }
               }
            else break; // Avoid line increment
           }
        while ( ++line<Lines->Count );


// . . . Read data values part (data lines)

         Log->SubEntry("line " +String(line)+ ": data reached");

        // . . . Columns count
        // First data line columns count
        mat::FillTokens(Line,Tokens);
        Log->SubEntry("First data line has: " +String(Tokens.size())+ " columns");

        if ( IsHeaderFound )
           {
            // Columns are already counted and arranged when decoding header
            Log->SubEntry("Header says: " +String(colcount)+ " columns, " +String(XCount)+ " abscissa cols" );
            if ( colcount != Tokens.size() )
               {
                Log->Error("Header and data columns don't agree! Check your file");
                return "Header and data columns don't agree!";
               }
            else Log->Success("Agreement between header and data");
           }
        else
           {
            Log->SubEntry("Header not found, I'll assume this as column number");
            colcount = Tokens.size();
            // . . . build here default columns
            BuildDefaultColumns(colcount);
           }

        // A check before continue
        if ( colcount < 1 )
                {
                 Log->Error("No data columns found, check your file!");
                 return "No data columns found!";
                }
        Log->SubEntry("I believe there are " +String(xcount)+ " abscissa columns");

        // . . . Now fill columns (retrieving values)
        int row=0; // current row counter (data lines)

        // Reserve some space for vals
        for(ci=Columns.begin();ci!=Columns.end();ci++) ci->Values.reserve(Lines->Count);

        // Continue to read lines
        for ( line; line<Lines->Count; line++)
            {
             // Get a data line
             Line = Lines->Strings[line].Trim();

             // Analyze it
             if ( Line.IsEmpty() ) continue; // Ignore empty lines
             else if ( !mat::IsFloat(Line[1]) ) // A text line: what to do?
                     {// Signal text lines inside data
                      Log->SubEntry("Line " +String(line)+ " : text in data block? Check your file!");
                      return "Line " +String(line)+ " : text in data block";
                     }

             // Ok, now I have a data line: get the tokens
             mat::FillTokens(Line,Tokens);

             // Now I have the tokens, how many are they?
             if ( Tokens.size() != colcount )
                {// Tokens read are lesser than initial columns! What to do?
                 if ( Tokens.size() < colcount )
                    {// if decreased all ok, but warn of possible misunderstandings
                     Log->SubEntry("Line " +String(line)+ " : column number decreased");
                    }
                 else
                    {// if are greater than initial columns something is wrong
                     Log->SubEntry("Line " +String(line)+ " : column number increased to " + String(Tokens.size()) + " from " + String(colcount) + " ; check your file");
                     return "Columns in line " +String(line)+ " increased!";
                    }
                }

             // Get values
             double v;
             for (U32 i=0; i<Tokens.size(); i++)
                 {
                  try {// Conversion from string to float
                       v = Tokens[i].ToDouble();

                       // Abscissa sorting control
                       if ( Columns[i].XIndex==int(i) && !Columns[i].IsUnsorted && !Columns[i].Values.empty() )
                         {
                          if ( (v-Columns[i].Values.back()) < 0 )
                             {
                              Columns[i].IsUnsorted = true;
                              Log->SubEntry("Line " + String(line)+ ", column " + String(i) + " : unsorted abscissa");
                             }
                         }
                       // Now can add new value
                       Columns[i].Values.push_back( v );
                      }
                  catch(EConvertError& e)
                      {// Conversion failed
                       // Column[i].Values.push_back( 0 ); // A default value?
                       String FlLit = Tokens[i];
                       Log->SubEntry("Line " + String(line)+ ", column " + String(i) + " : not a valid float");
                       Log->Error(e.Message);
                       return "Line " + String(line) + ": conversion to float error";
                      }
                 }
             row++; // Update data rows count
            }
        Log->SubEntry("Data columns read; total data rows:" + String(row));

// . . . Now can do a data check
        // . . . Mapped arrays dimensions: abscissas must be equal or greater
        if ( IsHeaderFound )
           {
            for (U32 i=0; i<colcount; i++)
                {
                 if ( Columns[i].XIndex>0 && Columns[i].XIndex!=int(i) ) // Only mapped ordinates
                    {
                     if ( Columns[i].Values.size() < Columns[Columns[i].XIndex].Values.size() )
                        {
                         Log->SubEntry("Warning: Column " +String(i)+ + " « " + Columns[i].Label + " » is shorter than its mapped abscissa column " +
                                       String(i)+ " « " + Columns[Columns[i].XIndex].Label + " »");
                        }
                     else if ( Columns[i].Values.size() > Columns[Columns[i].XIndex].Values.size() )
                        {
                         Log->Error("Column " +String(i)+ + " « " + Columns[i].Label + " » is greater than its mapped abscissa column " +
                                    String(i)+ " « " + Columns[Columns[i].XIndex].Label + " »");
                         return "Different size of mapped columns";
                        }
                    }
                }
           }

// . . . Finally
        return "";
}

//---------------------------------------------------------------------------
void __fastcall cls_DataColumns::ExportTo(TStrings* Lines)
{ // Export datas to strings

        String Line;

// . . . Header
        Line = "";
        for (U32 i=0; i<Columns.size(); i++) // For each column
            {
             Line += "  " + String(br_cl[1]) + Columns[i].Label + " ";
             if ( Columns[i].XIndex!=int(i) )
                {
                 if ( Columns[i].XIndex>=0 ) Line += String(br_ab[1])+Columns[i].XLabel+String(br_ab[2])+" ";
                 else Line += String(br_ab[1])+(Columns[i].IdxScale==1?String(""):String(Columns[i].IdxScale))+Columns[i].IdxUnit+String(br_ab[2])+" ";
                }
             Line += String(br_un[1]);
             if ( Columns[i].Scale != 1 ) Line += String(Columns[i].Scale);
             Line += Columns[i].Unit + String(br_un[2]) + String(br_cl[2]);
            }
        Lines->Add(Line);
        Lines->Add("");

// . . . Valori
        U32 MaxRows = MaxColSize;
        for (U32 l=0; l<MaxRows; l++) // For each row
            {
             Line = "";
             for (U32 i=0; i<Columns.size(); i++) // For each column
                 {
                  try { Line += "\t" + String(Columns[i].Values.at(l)); }
                  catch(std::out_of_range& e) {Line += "\t     ";}
                 }
             Lines->Add(Line);
            }
}



//---------------------------------------------------------------------------
void __fastcall cls_DataColumns::LogColumnsSummary()
{ // Write the column structure summary in the log
        Log->Entry("Column structure summary");
        Log->SubEntry("Got " +String(Columns.size())+ " columns; max size is " +String(MaxColSize));

        String coldes;
        for (U32 i=0; i<Columns.size(); i++)
            {
             coldes = "Column " +String(i)+ " ";
             if ( !Columns[i].Label.IsEmpty() ) coldes += " «" + Columns[i].Label + "» ";
             coldes += " of size " +String(Columns[i].Values.size())+ " ";
             if ( Columns[i].Scale!=1 ) coldes += " scaled " +String(Columns[i].Scale);
             if ( !Columns[i].Unit.IsEmpty() ) coldes += " [" + Columns[i].Unit + "] ";
             if ( Columns[i].XIndex==int(i) ) coldes += " is abscissa ";
             else
                {
                 coldes += " is ordinate ";
                 if ( Columns[i].XIndex<0 )
                    {
                     coldes += " plotted by index ";
                     if ( Columns[i].IdxScale!=1 ) coldes += " scaled with " + String(Columns[i].IdxScale);
                     if ( !Columns[i].IdxUnit.IsEmpty() ) coldes += " [" + String(Columns[i].IdxUnit) + "]";
                    }
                 else coldes += " linked to column " + String(Columns[i].XIndex)+ " «" + Columns[Columns[i].XIndex].Label + "»";
                }
             Log->SubEntry(coldes);
            }
}


//---------------------------------------------------------------------------
void __fastcall cls_DataColumns::Split(String strpat, bool samewidth)
{ // Splitting columns
  // 'strpat': pattern of splitting, such as |2|1|3|
  // 'samewidth': indexes/abscissa will be rescaled to have splitted chunks of same absc width
  // Splitted chunks abscissa scale will be rescaled taking total abscissa width as invariant

        Log->SubEntry("Splitting columns");
        // . . . Rescale indexes or abscissas if requested
        if ( samewidth )
           {
            Log->SubEntry("Abscissa scale of each chunk will be rescaled",1);
           }

// . . . Decode pattern
        mat::numlist PatternNums; // A bag for the splitting pattern number list
        U32 chunkscount,Psum,Pmax;
        String Res = mat::GetNumberList(strpat,PatternNums);
        if ( Res.IsEmpty() )
           {// Success
            Log->Success("Splitting pattern read: " + mat::NumlistToStr(PatternNums));
            // . . . Retrieve info from pattern
            chunkscount = PatternNums.size();
            Psum = mat::round(mat::sum(PatternNums));
            Pmax = mat::max(PatternNums);
            Log->SubEntry("Size: " +String(chunkscount)+ "  Sum: " +String(Psum)+ "  Max: " +String(Pmax),1);
           }
        else
           {
            Log->Error("Invalid splitting pattern: " + Res);
            return;
           }

// . . . A check
        if ( Psum<=0 )
           {
            Log->Error("Invalid splitting pattern");
            return;
           }

// . . . Instantiate new columns set
        U32 oldcolcount = Columns.size();
        U32 newcolcount = chunkscount * oldcolcount;
        U32 colstoadd = newcolcount - oldcolcount;
        Columns.reserve(newcolcount);
        for (U32 i=0; i<colstoadd; i++) Columns.push_back(cls_DataColumn());

// . . . Remap old columns to the new positions
        for (U32 i=oldcolcount-1; i>=1; i--) // All columns starting to last, except first
            {
             // Shift Column
             if ( Columns[i].XIndex >= 0 ) Columns[i].XIndex *= chunkscount;
             Log->SubEntry("Copying column " +String(i)+ " to " +String(i*chunkscount),1);
             Columns[i*chunkscount] = Columns[i]; // Here the column copy
             //LogColumnsSummary();

             // Leave a default column in this position
             Columns[i].Values.clear();
             Columns[i] = cls_DataColumn();
            }

// . . . Split operation
        mat::uintlist chunksizes (chunkscount); // size of chunks of the current column
        //for (ci=Columns.begin(); ci!=Columns.end(); ci++) // For each column
        for (U32 i=0; i<newcolcount; i+=chunkscount) // For each old full column
            {
             // . . . Chunks dimension
             chunksizes.clear();
             U32 chunksizessum = 0;
             U32 chunksize;
             for (U32 c=0; c<chunkscount; c++) // For each chunk (except first)
                 {
                  chunksize = mat::round((PatternNums[c] * Columns[i].Values.size() ) / Psum);
                  chunksizessum += chunksize;
                  chunksizes.push_back(chunksize);
                 }
             // Equalization of chunks sizes to fit all column size
             chunksize = Columns[i].Values.size() - chunksizessum;
             if ( !chunksize ) Log->Success("Perfect fit");
             else
                {
                 Log->Warn("Rest of division: " +String(chunksize)+ " added to last chunk");
                 chunksizes.back() += chunksize;
                }

             // . . . Now I have chunks sizes; fill chunk columns
             U32 maxsize = mat::max(chunksizes);
             U32 row = chunksizes[0]; // Current row of current column
             for (U32 c=1; c<chunkscount; c++) // For each chunk (except first)
                 {
                  // . . . Inherit other attributes...
                  Columns[i+c].AssignExceptValues(Columns[i]);
                  Columns[i+c].Label += String(c);
                  if ( Columns[i+c].XIndex >= 0 ) Columns[i+c].XIndex += c;

                  // . . . Rescale indexes or abscissas if requested
                  //if ( samewidth ) Columns[i+c].IdxScale /= PatternNums[c];
                  //if ( samewidth ) Columns[i+c].IdxScale *= Pmax/PatternNums[c];
                  if ( samewidth ) Columns[i+c].IdxScale *= double(maxsize-1)/double(chunksizes[c]-1);

                  // . . . And finally chunk values
                  Columns[i+c].Values.reserve(chunksizes[c]);
                  Columns[i+c].Values.assign(Columns[i].Values.begin()+row, Columns[i].Values.begin()+row+chunksizes[c]);
                  row += chunksizes[c]; // Update current row
                 }

             // . . . Now attend to first chunk
             // Leave only first chunk in the old full column
             Columns[i].Values.erase(Columns[i].Values.begin()+chunksizes[0], Columns[i].Values.end());
             Columns[i].Label += String(0);
             //if ( samewidth ) Columns[i].IdxScale *= Pmax/PatternNums[0];
             if ( samewidth ) Columns[i].IdxScale *= double(maxsize-1)/double(chunksizes[0]-1); // Abscissa rescale
            }
}


//---------------------------------------------------------------------------
void __fastcall cls_DataColumns::Reorder(String strseq)
{ // Reorder columns items (reindexing)
  // 'strseq': sequence of reordering
  // indexes will be rescaled and are referred to longest column
        Log->SubEntry("Reordering columns");

// . . . Settings
        static const String top = "top"; // start sample index identifier
        static const String end = "end"; // end sample index identifier

// . . .  Auxiliary variables
        std::vector<mat::cls_IntSerie> series (10);
        String ret, resolved_strseq;
        TReplaceFlags ReplOpts = TReplaceFlags() << rfReplaceAll << rfIgnoreCase;

// . . . Decode sequence
        U32 LongestColSize = LongestCol->Values.size();
        // Resolving symbolic identifiers in sequence string respect max sized column
        resolved_strseq = StringReplace(strseq, top, "0", ReplOpts);
        resolved_strseq = StringReplace(resolved_strseq, end, String(LongestColSize-1), ReplOpts);
        //resolved_strseq = StringReplace(resolved_strseq, "half", String(SizeMax/2), ReplOpts);
        Log->SubEntry("Resolved sequence is " + resolved_strseq);

        // Now can retrieve sequence numbers
        ret = mat::GetIntSerieSequence(resolved_strseq, series);
        if ( !ret.IsEmpty() ) // An error decoding sequence
           {
            Log->Error("Invalid sequence: " + ret);
            return;
           }

// . . . Reordering each column
        cls_DataColumn::typ_DataColumn tmpVals;
        I32 first,last,step;
        for (ci=Columns.begin(); ci!=Columns.end(); ci++) // For each column
            {
             tmpVals.clear();
             for (U32 i=0; i<series.size(); i++) // For each interval
                 {
                  Log->SubEntry("Not rescaled interval " +String(i+1)+ " : " + series[i]);
                  // Ordering indexes will be rescaled according to each column size
                  first = mat::round((double(series[i].top)*double(ci->Values.size()))/double(LongestColSize));
                  last = mat::round((double(series[i].end)*double(ci->Values.size()))/double(LongestColSize));
                  if ( first < 0 )
                     {
                      Log->Warn("Reordered column «" +ci->Label + "» , interval " +String(i)+ " has negative first index");
                      first = 0;
                     }
                  else if ( U32(first) >= ci->Values.size() )
                     {
                      Log->Warn("Reordered column «" +ci->Label + "» , interval " +String(i)+ " has first index too large");
                      first = ci->Values.size()-1;
                     }
                  if ( last < 0 )
                     {
                      Log->Warn("Reordered column «" +ci->Label + "» , interval " +String(i)+ " has negative last index");
                      last = 0;
                     }
                  else if ( U32(last) >= ci->Values.size() )
                     {
                      Log->Warn("Reordered column «" +ci->Label + "» , interval " +String(i)+ " has last index too large");
                      last = ci->Values.size()-1;
                     }
                  step = series[i].step;

                  Log->SubEntry("Interval " +String(i+1)+ " : from " +String(first)+ " to " +String(last)+ " step " +String(step));

                  I32 n = first;
                  while ( n*mat::sign(step) <= last*mat::sign(step) ) // For each index
                      {
                       //Log->Debug("reordering index " +String(n)+ " value " +String(ci->Values[n]));
                       try { tmpVals.push_back(ci->Values.at(n)); }
                       catch(std::out_of_range& e) { Log->Error("Reordered column «" +ci->Label + "» , interval " +String(i)+ ", invalid index: check order sequence"); return; }
                       n+=step;
                      }
                 }
             // Now overwrite reordered values
             if ( tmpVals.size() != ci->Values.size() ) Log->Warn("Reordered column «" +ci->Label + "» size is " +String(tmpVals.size())+ " rather than " +String(ci->Values.size()));
             ci->Values.assign(tmpVals.begin(),tmpVals.end());
            }// end for each column
}


//---------------------------------------------------------------------------
void __fastcall cls_DataColumns::UnMap()
{ // Reset abscissa mapping done by DecodeHeader (plot by index)
        for (ci=Columns.begin(); ci!=Columns.end(); ci++)
            {
             ci->XIndex = -1;
             ci->XLabel = "";
            }
}


//---------------------------------------------------------------------------
void __fastcall cls_DataColumns::ExecuteDirectives()
{ // Try to decode and execute the directives

// . . . Preliminary stuff
        if ( !Directives.empty() ) Log->Entry("Executing directives");
        else {
              Log->Warn("No directives, nothing to do");
             }

// . . . Auxiliary
        bool SameWidth; // samewidth flag

// . . . Preparation
        String dirstr;
        mat::stringlist Tokens;

// . . . Decode each directive
        for (U32 i=0; i<Directives.size(); i++)
              {
               // Get entire directive string...
               dirstr = Directives[i];
               // ...And analyze it
               if ( dirstr.Length() < 2 )
                  {// What?? Something wrong!
                   Log->Error(dirstr + " isn\'t a valid directive");
                   continue; // Pass to next directive
                  }
               else if ( dirstr[1] != px_dr )
                  {// What?? Something wrong!
                   Log->Error("An invalid string was passed for directive! " + dirstr);
                   continue; // Pass to next directive
                  }
               else
                  {// Directive string OK, get tokens...
                   dirstr = dirstr.SubString(2,dirstr.Length()-1); // Discard prefix
                   mat::FillSpacedTokens(dirstr,Tokens);
                   //...And analyze them/execute
                   if ( Tokens.size() < 1 ) // Ensure there's at least one token
                      {// What?? Something wrong!
                       Log->Warn("Empty directive");
                       continue; // Pass to next directive
                      }
                   if ( Tokens[0].UpperCase() == "PLOTBYIDX" )
                      {
                       Log->SubEntry("Found directive `plotbyidx´");
                       if ( Tokens.size() > 1 ) // Warn there's more tokens
                          {
                           Log->Warn("Ignored tokens after directive");
                          }
                       // Execute directive
                       Log->SubEntry("All columns will be plotted by index, ignoring header",1);
                       UnMap();
                      }
                   if ( Tokens[0].UpperCase() == "SAMEWIDTH" )
                      {
                       Log->SubEntry("Found directive `samewidth´");
                       if ( Tokens.size() > 1 ) // Warn there's more tokens
                          {
                           Log->Warn("Ignored tokens after directive");
                          }
                       // Execute directive
                       Log->SubEntry("Splitted chunks abscissas will rescaled to fit the same width",1);
                       SameWidth = true;
                      }
                   else if ( Tokens[0].UpperCase() == "SPLIT" )
                      {
                       Log->SubEntry("Found directive `split´");
                       if ( Tokens.size() != 2 ) // Ensure there's two tokens
                          {
                           Log->Error("Invalid splitting pattern argument");
                           continue; // Pass to next directive
                          }
                       // Execute directive
                       Log->SubEntry("Data columns will be splitted with the given pattern: " + Tokens[1],1);
                       Split(Tokens[1],SameWidth);
                      }
                   else if ( Tokens[0].UpperCase() == "ORDER" )
                      {
                       Log->SubEntry("Found directive `order´");
                       if ( Tokens.size() != 2 ) // Ensure there's two tokens
                          {
                           Log->Error("Invalid order sequence argument");
                           continue; // Pass to next directive
                          }
                       // Execute directive
                       Log->SubEntry("Columns data will be reordered according to given sequence: " + Tokens[1],1);
                       Reorder(Tokens[1]);
                      }
                   else
                      {
                       Log->Error(dirstr + " isn\'t a valid directive");
                       continue; // Pass to next directive
                      }
                  }
              }
}



//=============================== Operators =================================

//============================== Properties =================================

//================================ Events ===================================

//============================ Private Methods ==============================


//---------------------------------------------------------------------------
void __fastcall cls_DataColumns::BuildDefaultColumns( int num )
{ // Build defailt columns

        firstxcol = -1; // First column abscissa idx
        xcount = 0; // Number of encountered abscissa columns
        Columns.reserve(num);

        for ( int i=0; i<num; i++ )
            {
             Columns.push_back(cls_DataColumn());
             Columns[i].Label = "track " + String(i);
            }
        // Default behaviour
        Log->SubEntry("I\'ll plot all columns respect their ordinal index");
}

//---------------------------------------------------------------------------
void __fastcall cls_DataColumns::DecodeHeader(const mat::stringlist& Tokens)
{ // Decode a recognized header string

        // For sure
        Columns.clear();

        // Now can easily retrieve the number of declared columns
        colcount = Tokens.size();
        firstxcol = -1; // First column abscissa idx
        xcount = 0; // Number of encountered abscissa columns
        Columns.reserve(colcount);

// . . . Fill Column Data
        String logmsg;
        String Token; int p; // Token and delimiter position
        bool AllUndeclared = true; // A flag for not explicitly linked abscissa
        mat::stringlist Bracketed;

        for (U32 i=0; i<colcount; i++)
            {
             Columns.push_back(cls_DataColumn());
             logmsg = "  > Column " + String(i);

             // Take a column declaration
             Token = Tokens[i].Trim();

             // Contains a unit?
             if ( mat::GetDelimitedSubStrings(Token,Brackets_Unit,Bracketed).IsEmpty() )
                {// A unit is declared; recognize if there's a number
                 double v;
                 mat::GetNumberAndUnit(Bracketed[Bracketed.size()-1],v,Columns.back().Unit);
                 Columns.back().Scale = mat::IsZero(v) ? 1 : v;

                 // Take the (remaining) track label
                 p = LastDelimiter(Brackets_Unit[1],Token);
                 Columns.back().Label = Token.SubString(1,p-1).Trim();

                 logmsg += "\tlabel: " + String(Columns.back().Label) + "\tscale: " + String(Columns.back().Scale) + "\tunit: " + Columns.back().Unit;
                }
             else
                {// No units
                 Columns.back().Unit = "";
                 Columns.back().Label = Token.Trim();
                 logmsg += "\tlabel: " + String(Columns.back().Label) + "\tno scale or unit";
                }

             // An abscissa is declared? (Is an ordinate column?)
             if ( mat::GetDelimitedSubStrings(Token,Brackets_Abscissa,Bracketed).IsEmpty() )
                {// An ordinate column: get the label or the ordinal index scale and unit
                 double v; String s;
                 mat::GetNumberAndUnit(Bracketed[Bracketed.size()-1],v,s);
                 if ( mat::IsZero(v) )
                    {
                     Columns.back().XLabel = s; // Is given a link to an abscissa column
                     AllUndeclared = AllUndeclared && s.IsEmpty(); // Update flag
                     logmsg += "\tis an ordinate linked to column " + s;
                    }
                 else
                    {// Is given a scale and unit for data ordinal index
                     Columns.back().IdxScale = v;
                     Columns.back().IdxUnit = s;
                     logmsg += "\tis an ordinate whose index scales as " +String(v) + "[" + s +"]";
                    }
                 Columns.back().XIndex = -1; // Column linking will be resolved in a second pass
                }
             else
                {// An abscissa column
                 Columns.back().XLabel = ""; // Doesn't have a link to another abscissa
                 Columns.back().XIndex = i; // Abscissa index is equal to its column index
                 xcount++; // Update abscissa columns counter
                 if ( firstxcol<0 ) firstxcol = i; // Register first abscissa column
                 logmsg += "\tis an abscissa column";
                }
             Log->SubEntry(logmsg);
            }

// . . . Resolving abscissa links

        // Distinguish possible cases

        // *) No ordinates declared
        if ( xcount == colcount )
           {
            Log->SubEntry("No ordinates declared, I'll plot all by index");
            // for (int i=0; i<colcount; i++) Columns[i].XIndex = -1; // This is the default
           }
        // *) No abscissas declared
        else if ( xcount == 0 )
           {
            Log->SubEntry("No abscissas declared, I'll plot all by index");
            // for (int i=0; i<colcount; i++) Columns[i].XIndex = -1; // This is the default
           }
        // *) All ordinates have not an explicitly declared abscissa
        else if ( AllUndeclared )
           {
            Log->SubEntry("I\'ll assume column " +String(firstxcol)+ " as common abscissa for the undeclared ordinates, ignoring the rest");
            for (U32 i=0;i<colcount;i++)
                if ( Columns[i].XIndex < 0 ) // Only ordinates
                        Columns[i].XIndex = firstxcol;
           }
        // *) All other cases
        else
           {
            for (U32 i=0; i<colcount; i++)
                {
                 if ( Columns[i].XIndex < 0 ) // Ordinates only
                    {// Manage unresolved track ordinate-abscissa link
                     if ( !Columns[i].XLabel.IsEmpty() )
                        {// Find correponding abscissa column
                         int lnk = -1;
                         for (U32 j=0; j<colcount; j++)
                             {
                              if ( Columns[i].XLabel == Columns[j].Label )
                                 {
                                  lnk = j;
                                  break;
                                 }
                             }
                         if ( lnk >= 0 ) Columns[i].XIndex = lnk;
                        }
                     else
                       {// Ordinate with an empty abscissa label
                        // Nothing to do, plot in default mode
                        Log->SubEntry("Column " +String(i)+ " « " + Columns[i].Label + " » will be plotted by ordinal index");
                       }
                    }
                }
           }
}



//============================== END OF FILE ================================
