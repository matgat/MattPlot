//---------------------------------------------------------------------------
#include <vcl.h>
#include "unt_MatCommon.h"
#include "unt_Log.h"
#pragma hdrstop
#include "unt_DataColumns.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
/*                                            Open or Drop
                               TFileStream* /
                     TStream* /             \ command line (appended rows)
    Input channels: /         \ other
*/


//================================ Settings =================================
    // Directive strings
    const String nms_DataColumns::STR_HEADER    = "header";
    const String nms_DataColumns::STR_RESAMPLE  = "resample";
    const String nms_DataColumns::STR_SPLIT     = "split";
    const String nms_DataColumns::STR_TITLE     = "title";
    const String nms_DataColumns::STR_XLABEL    = "xlabel";
    const String nms_DataColumns::STR_YLABEL    = "ylabel";
    const String nms_DataColumns::STR_XMARKER   = "xmarker";
    const String nms_DataColumns::STR_YMARKER   = "ymarker";
    const String nms_DataColumns::STR_TEXT      = "text";
    const String nms_DataColumns::STR_TOLERANT  = "tolerant";
    const String nms_DataColumns::STR_DECSEP    = "decimalseparator";
    const String nms_DataColumns::STR_THOUSEP   = "thousandseparator";
    const String nms_DataColumns::STR_STRICTDEL = "strictdelimiter";
    const String nms_DataColumns::STR_MAXLINES  = "maxlines";
    const String nms_DataColumns::STR_END       = "end";

    // Directive characters
    const typ_ReadChar nms_DataColumns::DirectivePrefix = '#';
    const typ_ReadChar nms_DataColumns::LineCmtChar = '/';
    const typ_ReadChar nms_DataColumns::BlockCmtChar = '*';

    // Header characters
    const typ_ReadChar nms_DataColumns::EscapeChar = '\\';
    const typ_ReadChar nms_DataColumns::PrefixArgOpen = '{';
    const typ_ReadChar nms_DataColumns::PrefixArgClose = '}';
    const typ_ReadChar nms_DataColumns::ColumnOpen = '<';
    const typ_ReadChar nms_DataColumns::ColumnClose = '>';
    const typ_ReadChar nms_DataColumns::AbscissaOpen = '(';
    const typ_ReadChar nms_DataColumns::AbscissaClose = ')';
    const typ_ReadChar nms_DataColumns::UnitOpen = '[';
    const typ_ReadChar nms_DataColumns::UnitClose = ']';
    const String nms_DataColumns::PrefixRealPart = "Re";
    const String nms_DataColumns::PrefixImgPart = "Im";



/////////////////////////// cls_ColumnDeclaration ///////////////////////////

//============================= Static members ==============================

//============================= Public methods ==============================
//---------------------------------------------------------------------------
cls_ColumnDeclaration::cls_ColumnDeclaration ( )
{ // Constructor

// . Defaults
    Label = "";
    Type = TYP_DEPVAR; // if no header, treat as an indexed ordinate
}

//---------------------------------------------------------------------------
void cls_ColumnDeclaration::RetrieveFrom ( String s )
{ // Get the columns data from string

// . Handle prefix
    mat::stringlist PrefixArg;
    String Prefix = mat::GetDelimitedSubStrings(PrefixArg,s,dcol::PrefixArgOpen,dcol::PrefixArgClose);
    if ( PrefixArg.size() > 0 )
         { // There is a prefix: extract string
          // A pair of checks
          if ( PrefixArg.size() > 1 ) throw(Exception("Too many fields enclosed by " + String(dcol::PrefixArgOpen) + String(dcol::PrefixArgClose)));
          if ( Prefix.IsEmpty() ) throw(Exception("Empty prefix of argument enclosed by " + String(dcol::PrefixArgOpen) + String(dcol::PrefixArgClose)));
          s = PrefixArg.back();
         }
    else { // No prefix
          Prefix = "";
         }

// . Call parser machine for all characters
    // . Preparation
    bool Escaping = false; // handle escape character to include ()[] in track names
    int SubState; // Auxiliary
    int i = 0; // current character position
    ParserState = ST_GETTING_LABEL;
    Label = "";
    IndepVar.Label = "";
    String IndepVarScalingString = "none"; // Temporary bag for indepvar scaling string
    Scaling.Unit = ""; // Temporary used as bag for scaling string
    Type = TYP_INDEPVAR;
    // . Parse
    while ( ++i <= s.Length() )
       {
        switch ( ParserState )
          {
           case ST_GETTING_LABEL :
               // Collect chars until 'AbscissaOpen' or 'UnitOpen'
               if ( s[i]==dcol::AbscissaOpen && !Escaping )
                    {
                     Type = TYP_DEPVAR;
                     ParserState = ST_GETTING_VARIABLE;
                    }
               else if ( s[i]==dcol::UnitOpen && !Escaping )
                    {
                     ParserState = ST_GETTING_UNIT;
                    }
               else if ( s[i] == dcol::EscapeChar ) Escaping = true;
               else {
                     Label += s[i];
                     Escaping = false;
                    }
               break;
           case ST_GETTING_VARIABLE :
               // Collect chars until 'AbscissaClose'
               SubState = 0;
               do {
                   switch(SubState)
                       {
                        case 0 : // collecting label
                            switch( s[i] )
                                {
                                 case dcol::AbscissaClose : SubState = 3; break;
                                 case dcol::UnitOpen      : SubState = 1; break;
                                 default : IndepVar.Label += s[i++];
                                }
                            break;
                        case 1 : // preparing to collect scaling string
                            ++i; // char after 'dcol::UnitOpen'
                            IndepVarScalingString = "";
                            SubState = 2;
                            break;
                        case 2 : // collecting scaling string
                            switch( s[i] )
                                {
                                 case dcol::AbscissaClose :
                                 case dcol::UnitClose     : SubState = 3; break;
                                 default : IndepVarScalingString += s[i++];
                                }
                            break;
                       }
                  }
               while ( i <= s.Length() && SubState < 3 );
               ParserState = ST_EATING_FOR_UNIT;
               break;
           case ST_EATING_FOR_UNIT :
               // Eat until 'UnitOpen'
               if ( s[i] == dcol::UnitOpen ) ParserState = ST_GETTING_UNIT;
               break;
           case ST_GETTING_UNIT :
               // Collect chars until 'UnitClose'
               if ( s[i] != dcol::UnitClose ) Scaling.Unit += s[i];
               break;
          } // end 'switch state'
       } // end 'while'

// . Trim retrieved strings
    Prefix = Prefix.Trim();
    Label = Label.Trim();
    IndepVar.Label = IndepVar.Label.Trim();
    Scaling.Unit = Scaling.Unit.Trim();

// . Column name
    if ( Label.IsEmpty() ) throw(Exception("Empty column label"));

// . Prefix and independent variable label: assign column type
    if ( !Prefix.IsEmpty() )
         { // Part of a complex
          if ( Prefix == dcol::PrefixRealPart ) Type = TYP_REALPART;
          else if ( Prefix == dcol::PrefixImgPart) Type = TYP_IMGPART;
          else throw(Exception(String("Unknown prefix '")+Prefix+String("'")));
         }
    else if ( Type == TYP_DEPVAR )
         { // An ordinate: deal with its independent variable
          // Three possible cases:
          // "" --> empty label, plotted by index
          // "x" --> label of a linked column
          // "[3s+2]" or [x[3s+2]] --> self declared variable
          if ( IndepVarScalingString != "none" )
               { // Surely is self contained, [] encountered
                IndepVar.IsSelfContained = true;
                IndepVar.Scaling = IndepVarScalingString;
               }
          else { // Could be linked
                IndepVar.IsSelfContained = IndepVar.Label.IsEmpty();
                // Eventual column label link will be resolved later
               }
         }

// . Unit informations
    // Retrieved string may contain type info
    Scaling = Scaling.Unit;
}

//---------------------------------------------------------------------------
String cls_ColumnDeclaration::ToHeaderString() const
{ // Encode column data into string

    // . Column declaration opening brace
    String ColStr = String(dcol::ColumnOpen);

    // . Prefix
    String Prefix = "";
    if ( Type == TYP_REALPART ) Prefix = dcol::PrefixRealPart;
    else if ( Type == TYP_IMGPART ) Prefix = dcol::PrefixImgPart;

    // . Prefix arg opening brace
    if ( !Prefix.IsEmpty() ) ColStr += Prefix + String(dcol::PrefixArgOpen);

    // . Column Label (name)
    ColStr += Label;

    // . Independent variable part
    if ( Type != TYP_INDEPVAR )
       {
        ColStr += String(dcol::AbscissaOpen) + IndepVar.Label;

        if ( IndepVar.IsSelfContained )
              {
               ColStr += String(dcol::UnitOpen) + IndepVar.Scaling + String(dcol::UnitClose);
              }

        ColStr += dcol::AbscissaClose;
       }

    // . Units part
    if ( !(Scaling.IsIdentity() && Scaling.Unit.IsEmpty()) )
       {
        ColStr += dcol::UnitOpen;
        ColStr += Scaling;
        ColStr += dcol::UnitClose;
       }

    // . Prefix arg closing brace
    if ( !Prefix.IsEmpty() ) ColStr += String(dcol::PrefixArgClose);

    // . Column declaration closing brace
    ColStr += String(dcol::ColumnClose);

    return ColStr;
}

//---------------------------------------------------------------------------
void cls_ColumnDeclaration::Summary ( class cls_Log* Log ) const
{ // Put retrieved data in the log

    String strResolved = (IndepVar.Index<0) ? String(" (not yet resolved)") : String(" (col ") + String(IndepVar.Index) + String(")");
    String strPlotted = (IndepVar.IsSelfContained) ? String(" plotted by index") : String(" plotted respect '") + IndepVar.Label + String("'") + strResolved;
    String strMeasured = (Scaling.Unit.IsEmpty()) ? String("") : String(", measured in ") + Scaling.Unit;
    String strTransform = (MustBeScaled()) ? String(", scaled with ") + String(Scaling.Scale) + String(" and shifted by ") + String(Scaling.Offset) + Scaling.Unit : String("");
    String strIndexUnit = (IndepVar.Scaling.Unit.IsEmpty()) ? String(", index is ") : String(", index is measured in ") + IndepVar.Scaling.Unit + String(", ");
    String strIndexScale = (IndexScaled()) ? String("scaled with ")+String(IndepVar.Scaling.Scale) + String(" and shifted by ") + String(IndepVar.Scaling.Offset) + IndepVar.Scaling.Unit : String("not scaled");
    strIndexScale = (IndepVar.IsSelfContained)? strIndexUnit + strIndexScale : String("");
    String strDescr;
    switch ( Type )
       {
        case TYP_INDEPVAR :
          strDescr = String("'") + Label + String("' is an independent variable (not plotted)") + strMeasured + strTransform;
        break;
        case TYP_DEPVAR :
          strDescr = String("'") + Label + String("' is an ordinate") + strPlotted + strMeasured + strTransform + strIndexScale;
        break;
        case TYP_REALPART :
          strDescr = String("is the real part of complex ") + Label + strPlotted + strMeasured + strTransform + strIndexScale;
        break;
        case TYP_IMGPART :
          strDescr = String("is the imaginary part of complex ") + Label + strPlotted + strMeasured + strTransform + strIndexScale;
        break;
        default :
          strDescr = String("'") + Label + String("' is an unknown type and will be ignored");
       }

    Log->SubEntry(strDescr,1);
}



//============================ Private methods ==============================




/////////////////////////////// cls_DataHeader //////////////////////////////


//---------------------------------------------------------------------------
cls_DataHeader::cls_DataHeader()
{ // Constructor

// . Defaults
    Reset();
}

//---------------------------------------------------------------------------
void cls_DataHeader::RetrieveFrom ( const String& s )
{ // Retrieve from String

// . Get columns declarations
    mat::stringlist ColDecls;
    mat::GetDelimitedSubStrings(ColDecls,s,dcol::ColumnOpen,dcol::ColumnClose);
    // Now 'ColDecls' contains cols declarations; their number is:
    int ColDeclsCount = ColDecls.size();
    // A check:
    if ( ColDeclsCount <= 0 ) throw(Exception(String("Header declares no columns, enclose them with ") + String(dcol::ColumnOpen) + String(dcol::ColumnClose) ));

// . Prepare container and analyze each column declaration
    reserve( ColDeclsCount );
    for ( int c=0; c<ColDeclsCount; ++c )
        {
         // Get column declaration from this string
         cls_ColumnDeclaration newcoldecl;
         newcoldecl.RetrieveFrom( ColDecls[c] );
         // . Label overlapping check
         for ( typ_it cd=begin(); cd!=end(); ++cd )
             {
              if ( !newcoldecl.IsCplxPart() && (newcoldecl.Label==(*cd).Label) ) throw(Exception("Two or more columns have the same name " + newcoldecl.Label));
             }
         // Collect this declaration
         push_back( newcoldecl );
        }

// . Resolve linked columns
    // No, are resolved at the end of session in overall columns container

// . Finally
    i_Declared = true;
}

//---------------------------------------------------------------------------
void cls_DataHeader::Summary ( class cls_Log* Log ) const
{ // Put retrieved data in the log

    Log->SubEntry("Header summary:");
    for ( unsigned int c=0; c<size(); ++c )
        {
         Log->SubEntry(String("-- Column ") + String(c));
         operator[](c).Summary(Log);
        }
}

//---------------------------------------------------------------------------
void cls_DataHeader::Reset()
{ // Reset header for a new session

    clear();
    i_Declared = false;
}

//---------------------------------------------------------------------------
String cls_DataHeader::BuildSplitArg ()
{ // Build split argument from scaling
    ASSERTING((i_Declared),"Header must be declared for this")

    mat::numlist spl;
    for ( typ_it i=begin(); i!=end(); ++i )
        {
         if ( i->IsOrdinate() && !(i->IsLinked()) )
              {
               spl.push_back( 1.0 / i->IndepVar.Scaling.Scale );
              }
         else throw(Exception("Column to be splitted must be indexed ordinates"));
        }
    return mat::NumlistToString(spl);
}



//////////////////////////////// cls_Column /////////////////////////////////

//---------------------------------------------------------------------------
cls_Column::cls_Column ( unsigned int sz )
{ // Constructor

// . Defaults

// . Memory
    reserve(sz);
}

//---------------------------------------------------------------------------
void cls_Column::GetArithmeticProgression (double* k, double* o) const
{ // Retrieve eventual arithmetic progression

    *k = 0;
    if ( size() < 3 ) return;
    typ_cit ci = begin();
    *o = *(ci++);
    *k = *ci - *o;
    typ_cit ci_end = ci;
    while ( ++ci != end() )
        {
         if ( !mat::IsZero( *k - (*ci - *ci_end) ) ) { *k = 0; return; }
         ci_end = ci;
        }
}

//---------------------------------------------------------------------------
void cls_Column::BuildArrayDef( String& s, const char delm) const
{ // fills a string with data array definition

    String Delm = String(delm);
    //s.SetLength( s.Length() + 16*size() );
    double k,o;
    GetArithmeticProgression (&k, &o);
    if ( k ) s += mat::BuildMatlabArrayDef(o, k, *(rbegin())) + String("'");
    else {
          s += String("[");
          for ( typ_cit ci=begin(); ci!=end(); ++ci ) s += ::FloatToStr(*ci) + Delm;
          //s = s.SubString(1,s.Length()-1); // Drop last delimiter
          s[s.Length()] = ']'; // Change last delimiter
         }
}


//////////////////////////////// cls_Columns ////////////////////////////////


//---------------------------------------------------------------------------
cls_Columns::cls_Columns()
{ // Constructor

// . Defaults
    Title = "";
    Xlabel = "";
    Ylabel = "";
    Count = 0;
}

//---------------------------------------------------------------------------
cls_Columns::~cls_Columns()
{ // Destructor
    // Deallocate columns
    for ( typ_it i=begin(); i!=end(); ++i ) delete (*i);
    clear();
}

//---------------------------------------------------------------------------
void cls_Columns::ResolveLinks ( class cls_Log* Log )
{ // Resolve column links

// . Analyze each column
    unsigned int AbscissaCount = 0;
    for ( typ_it i=begin(); i!=end(); ++i )
        {
         // Count abscissa columns
         if ( (*i)->IsIndepVar() ) ++AbscissaCount;
        }

// . Resolve links
    if ( AbscissaCount == size() )
         {
          Log->Warn("No ordinate declared, I\'ll plot all by index");
          //for ( unsigned int i=0; i<size(); ++i ) operator[](i)->Type = cls_ColumnDeclaration::TYP_DEPVAR;
          for ( typ_it i=begin(); i!=end(); ++i ) (*i)->Type = cls_ColumnDeclaration::TYP_DEPVAR;
         }
    else { // Resolve links
          Log->SubEntry("Resolving ordinate-abscissa links...");
          String LabelToFind;
          for ( unsigned int i=0; i<size(); ++i )
              {
               // Skip if no label
               if ( operator[](i)->Label.IsEmpty() ) continue;
               // If real part, find corresponding img part
               else if ( operator[](i)->IsRealPart() ) LabelToFind = operator[](i)->Label;
               // Else if must be linked to an abscissa, find it
               else if ( !(operator[](i)->IndepVar.IsSelfContained) )
                       {
                        if ( operator[](i)->IndepVar.Label.Length() > 0 ) LabelToFind = operator[](i)->IndepVar.Label;
                        else throw Exception("Empty independent variable label of column " + operator[](i)->Label);
                       }
               else LabelToFind = ""; // In any case proceed to check label overlapping
               //else continue; // Otherwise skip: no, I want to do Labels overlapping check

               // Find the column index whose label is 'LabelToFind'
               for ( unsigned int j=0; j<size(); ++j )
                   {
                    if ( j != i )
                       {
                        // Labels overlapping check
                        if ( !(operator[](i)->IsCplxPart()) && (operator[](i)->Label==operator[](j)->Label) ) throw (Exception("Two or more columns have the same name " + operator[](i)->Label));
                        // Label found
                        else if ( LabelToFind == operator[](j)->Label ) operator[](i)->IndepVar.Index = j;
                       }
                   }
              }// end 'for all column declarations'
         }
}

//---------------------------------------------------------------------------
void cls_Columns::Summary ( class cls_Log* Log ) const
{ // Put retrieved data in the log

    Log->SubEntry("Overall columns summary:");
    for ( unsigned int c=0; c<size(); ++c )
        {
         Log->SubEntry(String("-- Column ") + String(c));
         operator[](c)->Summary(Log);
        }
}

//---------------------------------------------------------------------------
String cls_Columns::BuildHeader() const
{ // Build header directive

    String sh = "";
    for ( typ_cit c=begin(); c!=end(); ++c ) sh += (*c)->ToHeaderString();
    return dcol::BuildDirectiveString(dcol::STR_HEADER, sh);
}


//---------------------------------------------------------------------------
void cls_Columns::ToStrings (class TStringList* sl, const typ_ReadChar delm) const
{ // Save to text file, given column delimiter

// . Auxiliary variables
    unsigned int rmax = MaxColSize; // row max
    sl->BeginUpdate();

// . Overall infos
    // . Frame title and axes labels
    sl->Add( dcol::BuildDirectiveString(dcol::STR_TITLE, Title) );
    if (Xlabel.Length()>0) sl->Add( dcol::BuildDirectiveString(dcol::STR_XLABEL, Xlabel) );
    if (Ylabel.Length()>0) sl->Add( dcol::BuildDirectiveString(dcol::STR_YLABEL, Ylabel) );
    // . Value markers
    for (cls_Columns::typ_MarkersConstIt m=Markers.begin(); m!=Markers.end(); ++m)
      {
       if ( m->Vertical ) sl->Add( dcol::BuildDirectiveString(dcol::STR_XMARKER, String(m->Value)) );
       else sl->Add( dcol::BuildDirectiveString(dcol::STR_YMARKER, String(m->Value)) );
      }
    // . Text labels
    for (cls_Columns::typ_LabelsConstIt l=Labels.begin(); l!=Labels.end(); ++l)
      {
       sl->Add( dcol::BuildDirectiveString(dcol::STR_TEXT, String("(")+ String(l->x) + String(";") + String(l->y) + String(") ") + String(l->Text)) );
      }

// . Data part
    if ( delm == mat::InvalidChar )
         { // Multisession file
          // Settings
          sl->Capacity = (rmax+2) * size();
          // For each column
          for ( typ_cit c=begin(); c!=end(); ++c )
              {
               // Session header
               sl->Add( dcol::BuildDirectiveString(dcol::STR_HEADER, (*c)->ToHeaderString()) );
               // For each value
               for ( unsigned int i=0; i<(*c)->size(); ++i ) sl->Add( ::FloatToStr((*c)->operator[](i)) );
               sl->Add(""); // end session
              }
         }
    else { // Data table file
          // Auxiliary variables
          String Delm = String(delm);
          sl->Capacity = (rmax+2);
          String Line;
          // Data header
          if ( !empty() ) sl->Add( BuildHeader() );
          // Data part: for each row
          for ( unsigned int r=0; r<rmax; ++r )
              {
               Line = "";
               // For each column
               for ( typ_cit c=begin(); c!=end(); ++c )
                   {
                    if ( r<(*c)->size() ) Line += ::FloatToStr((*c)->operator[](r)) + Delm;
                   }
               Line = Line.SubString(1,Line.Length()-1); // Drop last delimiter
               sl->Add(Line);
              }
         }

// . Finally
    sl->EndUpdate();
}

