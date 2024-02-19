//---------------------------------------------------------------------------
#ifndef unt_DataColumnsH
#define unt_DataColumnsH
//---------------------------------------------------------------------------

        /* * * * * * * * * * * * * * * * * * * * * * * * * * *
        *
        *        unt_DataColumns  ver: 02    (aug.2008)
        *        2008 - Matteo Gattanini
        *
        *       This unit defines variables and classes used
        *       by Importer and Exporter
        *
        *       Example of usage:
        *       #include "unt_DataColumns.h"
        *
        * * * * * * * * * * * * * * * * * * * * * * * * * * * */


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Header inclusions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include <vector> // numbers container in 'cls_Column'
#include <Classes.hpp> // VCL classes like String,Exception

//-------------------------------- Settings ---------------------------------
#ifdef _DEBUG
#endif

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Types Definition ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    typedef char typ_ReadChar; // Character from stream

    
//:::::::::::::::::::::::::::: Namespace content ::::::::::::::::::::::::::::
namespace nms_DataColumns
{
// . Constants

    // Directive strings
    extern const String STR_HEADER;
    extern const String STR_RESAMPLE;
    extern const String STR_SPLIT;
    extern const String STR_TITLE;
    extern const String STR_XLABEL;
    extern const String STR_YLABEL;
    extern const String STR_XMARKER;
    extern const String STR_YMARKER;
    extern const String STR_TEXT;
    extern const String STR_TOLERANT;
    extern const String STR_DECSEP;
    extern const String STR_THOUSEP;
    extern const String STR_STRICTDEL;
    extern const String STR_MAXLINES;
    extern const String STR_END;

    // Directive characters
    extern const typ_ReadChar DirectivePrefix;
    extern const typ_ReadChar LineCmtChar;
    extern const typ_ReadChar BlockCmtChar;

    // Header characters
    extern const typ_ReadChar EscapeChar;
    extern const typ_ReadChar PrefixArgOpen;
    extern const typ_ReadChar PrefixArgClose;
    extern const typ_ReadChar ColumnOpen;
    extern const typ_ReadChar ColumnClose;
    extern const typ_ReadChar AbscissaOpen;
    extern const typ_ReadChar AbscissaClose;
    extern const typ_ReadChar UnitOpen;
    extern const typ_ReadChar UnitClose;
    extern const String PrefixRealPart;
    extern const String PrefixImgPart;

// . Functions
    String BuildDirectiveString(const String& dname, const String& darg) {return String(DirectivePrefix) + dname + String(" ") + darg;}
    String PutBracketsEscapeChar(const String& s) {String sout=""; for(int i=1;i<=s.Length();++i) if(s.IsDelimiter("[](){}",i)) sout += String('\\')+String(s[i]); else sout += s[i]; return sout; } // used for build columns names that include brackets
    // TODO: StrReplace?

// . Concrete classes
    //-----------------------------------------------------------
    class cls_IndependentVariable
    { // Independent variable infos
     public:
        cls_IndependentVariable() {
                                   IsSelfContained = true;
                                   Label = "";
                                   Index = -1;
                                   //Scaling;
                                  }
        // . Public attributes (!)
        bool IsSelfContained; // Tells if info refers to another column or not
        String Label; // Variable label
        int Index; // Linked column index
        mat::cls_ScaleUnitOffset Scaling; // Self contained infos
    }; // 'cls_IndependentVariable'
    
}//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
namespace dcol = nms_DataColumns; // a short alias for this namespace



    /////////////////////////////////////////////////////////////////////////
    class cls_ColumnDeclaration
    { // Single data column declaration
     public:
      typedef enum {ST_EATING,ST_GETTING_LABEL,ST_GETTING_VARIABLE,ST_EATING_FOR_UNIT,ST_GETTING_UNIT} en_ParserState;
      typedef enum {TYP_INDEPVAR,TYP_DEPVAR,TYP_REALPART,TYP_IMGPART} en_Type;

      // . Public methods
      cls_ColumnDeclaration();
      //~cls_ColumnDeclaration();

      void RetrieveFrom (String); // Get the column data from string
      String ToHeaderString() const; // Encode column data into string (header syntax)
      void Summary ( class cls_Log* ) const; // Put retrieved data in the log
      void Assign (const cls_ColumnDeclaration& cdl)
          {// Copy constructor implementation
           Label = cdl.Label;
           IndepVar = cdl.IndepVar;
           Scaling = cdl.Scaling;
           Type = cdl.Type;
          }
      // . Properties
      bool MustBeScaled() const {return !Scaling.IsIdentity();} // A scaling transform is specified
      bool HasDataType() const {return Scaling.HasDataType();} // A type is specified
      bool IndexScaled() const {return !IndepVar.Scaling.IsIdentity(); } // Index scale information present
      bool IsIndepVar() const {return (Type == TYP_INDEPVAR);} // Tell if is an abscissa
      bool IsOrdinate() const {return (Type == TYP_DEPVAR);} // Tell if is an ordinate
      bool IsRealPart() const {return (Type == TYP_REALPART);} // Tell if is a real part
      bool IsImgPart() const {return (Type == TYP_IMGPART);} // Tell if is an imaginary part
      bool IsLinked() const {return (IndepVar.Index >= 0);} // Tell if is linked to an abscissa column
      bool IsCplxPart() const {return (Type==TYP_REALPART)||(Type==TYP_IMGPART);} // Tell if is a complex part

      // . Public attributes (!)
      // Contained data
      String Label; // column label (name)
      dcol::cls_IndependentVariable IndepVar; // Linked column or scale and unit
      mat::cls_ScaleUnitOffset Scaling; // Unit and linear transformation at postprocessing
      en_Type Type; // Type of column

     private:

      // . Private attributes
      en_ParserState ParserState; // Parser machine state

    }; // 'cls_ColumnDeclaration

    /////////////////////////////////////////////////////////////////////////
    class cls_DataHeader : public std::vector<cls_ColumnDeclaration>
    { // Header data container
     public:
      typedef std::vector<cls_ColumnDeclaration>::iterator typ_it;

      // . Public methods
      cls_DataHeader();
      //~cls_DataHeader();

      void RetrieveFrom ( const String& ); // Retrieve from String
      void Summary ( class cls_Log* ) const; // Put retrieved data in the log
      void Reset(); // Reset header for a new session
      String BuildSplitArg(); // Build split argument from scaling

      // . Properties
      __property bool Declared = { read=i_Declared }; // header declared or not

      // . Public attributes (!)

    private:
      // . Private attributes
      bool i_Declared;

    }; // 'cls_DataHeader'


    /////////////////////////////////////////////////////////////////////////
    class cls_Column : public std::vector<double>, public cls_ColumnDeclaration
    { // Single column container
     public:
      typedef std::vector<double>::iterator typ_it;
      typedef std::vector<double>::const_iterator typ_cit;

      // . Public methods
      cls_Column (unsigned int =10);
      //~cls_Column();

      double TransformIndex (const double v) {return (IndepVar.Scaling.Scale*v)+IndepVar.Scaling.Offset;} // Index linear transformation
      void ApplyTransform() {for(unsigned int n=0;n<size();++n) (*this)[n]=(Scaling.Scale*(*this)[n])+Scaling.Offset;} // Apply on all data
      void CastToDataType() { try{// Get type
                                  if(Scaling.DataType.Length()<2) throw Exception("too short");
                                  char typ = Scaling.DataType[1];
                                  unsigned int bits = Scaling.DataType.SubString(2,Scaling.DataType.Length()-1).ToInt();
                                  bits = (bits/8<sizeof(bits)) ? (1<<bits)-1 : 0xFFFFFFFF;
                                  switch(typ)
                                   {
                                    case 'u': case 'U': for(unsigned int n=0;n<size();++n) (*this)[n]=static_cast<unsigned int>(mat::round((*this)[n])) & bits; break;
                                    case 'i': case 'I': for(unsigned int n=0;n<size();++n) (*this)[n]=mat::round((*this)[n]) & bits; break;
                                    default: throw Exception("unknown type " + String(typ));
                                   }
                                 }
                              catch(Exception &e){ throw Exception("Invalid data type \'" + Scaling.DataType + "\' for column: " + Label + " - " + e.Message);}
                            }
      void GetArithmeticProgression (double*, double*) const; // Retrieve eventual arithmetic progression
      void BuildArrayDef (String&, const char) const; // fills a string with data array definition

    // . Public attributes (!)

     private:

      // . Private attributes

    }; // 'cls_Column'

    /////////////////////////////////////////////////////////////////////////
     // A meta container for a table of data retrieved from text file
    class cls_Columns : public std::vector<class cls_Column*>
    {
     public:
      typedef std::vector<class cls_Column*>::iterator typ_it;
      typedef std::vector<class cls_Column*>::const_iterator typ_cit;
      // Markers
      typedef struct {double Value; bool Vertical;} stu_Marker;
      typedef std::vector<stu_Marker> typ_Markers;
      typedef typ_Markers::const_iterator typ_MarkersConstIt;
      // Labels
      typedef struct {double x,y; String Text;} stu_Label;
      typedef std::vector<stu_Label> typ_Labels;
      typedef typ_Labels::const_iterator typ_LabelsConstIt;

      // . Public methods
      cls_Columns();
      ~cls_Columns();

      void ResolveLinks ( class cls_Log* ); // Resolve column links
      void Summary ( class cls_Log* ) const; // Put summary in the log

      void ToStrings (class TStringList*, const typ_ReadChar) const; // Convert to a stringlist given delimiter
      //void SaveToFile (const String*, const typ_ReadChar) const;

      // . Properties
      __property bool MaxColSize = { read=get_MaxColSize }; // max column size

      // . Public attributes (!)
      String Title; // Columns title
      String Xlabel,Ylabel; // Axes labels
      typ_Markers Markers; // Value markers in plot
      typ_Labels Labels; // Text labels in plot
      unsigned int Count; // just a bag for size()

     private:
      String BuildHeader() const; // Build header directive
      // . Properties
      unsigned int get_MaxColSize() const {unsigned int s=0; for(typ_cit c=begin(); c!=end(); ++c) if(s<(*c)->size()) s=(*c)->size(); return s;}

    }; // 'cls_Columns'


//---------------------------------------------------------------------------
#endif
