//---------------------------------------------------------------------------

#ifndef unt_StreamParserH
#define unt_StreamParserH
//---------------------------------------------------------------------------

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    *
    *        unt_StreamParser
    *        2007 - Matteo Gattanini
    *
    *        cls_StreamParser  ver: 02    (jul.2007)
    *
    *       This unit defines a class to parse data text stream
    *       and fills a set of arrays of numbers.
    *
    *       Characteristics:
    *       .supports only 8bit chars
    *       .fixed line delimiters ('\r', '\n' or couples of them)
    *       .data delimiters cannot be '\r' or '\n'
    *
    *       Example of usage:
    *       #include "cls_StreamParser.h"
    *
    *       cls_StreamParser* cols = new cls_StreamParser(Log);
    *       cols->ImportFrom(stringlist);
    *       //
    *       //
    *       delete cols;
    *
    * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Header inclusions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include <vector> // standard vector container
#include <Classes.hpp> // VCL classes like String,Exception


//~~~~~~~~~~~~~~~~~~~~~~~ Auxiliary Classes Definition ~~~~~~~~~~~~~~~~~~~~~~

//>>>>>>>>>>>>>>>>>>>> Exceptions
    //...................................................................
    class ESyntaxError : public Exception
    {
     public://                                              .............
     // . . . Public methods
            ESyntaxError(const unsigned int ln, const unsigned int cl, const char ch)
                         : Exception("Syntax error in line " + String(ln) + ", column " + String(cl)),
                           Line(ln), Col(cl), c(ch)
            { Message = ClassName() + ": line " + String(Line) + ", column " + String(Col); }

            virtual String ClassName() const {return "ESyntaxError";}

     // . . . Public attributes (!)
            unsigned int Line, // Offending line number
            unsigned int Col, // Offending character column
            char c, // Offending character
     private://                                             .............
    };
    //...................................................................
    class EInvalidToken : public ESyntaxError
    {
     public://                                              .............
     // . . . Public methods
         EInvalidToken(const unsigned int ln, const unsigned int cl, const char ch,const String& tk)
                      : ESyntaxError(ln,cl,ch) , Token(tk)
         { Message = ClassName() + ": \'" + Token +"\' is not a valid token"; }
         virtual String ClassName() const {return "EInvalidToken";}
     // . . . Public attributes (!)
            String Token // Offending token
     private://                                             .............
    };


//>>>>>>>>>>>>>>>>>>>> Containers
    //-------------------------------------------------------------------
    class cls_DataColumn : public std::vector <double> {
    public://                                               ~~~~~~~~~~~~~

    // ~ ~ ~ Types definitions
      typedef std::vector <double> typ_DataColumn;
      typedef typ_DataColumn::iterator typ_DataColumnIt;

    // . . . Public methods
      cls_DataColumn()
      {// costruttore di default
                       Label=Unit=IdxUnit="";
                       Scale=IdxScale=1;
                       XLabel="";
                       XIndex=-1; // Here the default behaviour: plot by index
                       IsUnsorted = false;
                       Values.reserve(100);
                          }
      //~cls_DataColumn();

      void AssignExceptValues(const cls_DataColumn& dc)
                  {// un assegnamento
                      Label = dc.Label;
                      Unit = dc.Unit;
                      Scale = dc.Scale;
                      IdxScale = dc.IdxScale;
                      IdxUnit = dc.IdxUnit;
                      XLabel = dc.XLabel;
                      XIndex = dc.XIndex;
                      IsUnsorted = dc.IsUnsorted;
                      //Values.reserve(100); // !! Values not copied
                  }

    // . . . Properties
      //__property bool IsOrdinate = { read = get_IsOrdinate };
      //__property bool IsAbscissa = { read = get_IsAbscissa };

    // . . . Public attributes (!)

      // . . . Some column attributes
      String Label,Unit;
      int Index;
      double Scale; // Column values will be multiplied by this
      bool IsUnsorted; // For abscissa type only

      // . . . Linked column (abscissa)
      String XLabel; // Label of linked abscissa
      int XIndex; // The abscissa column; if -1 is ordinal index
      double IdxScale; // A scale for ordinal index
      String IdxUnit; // A unit for ordinal index

    private://                                              ~~~~~~~~~~~~~

    // . . . Private attributes
      typ_DataColumnIt vi; // data values iterator

    // . . . Private methods
      // . . . Properties
      //bool __fastcall get_IsOrdinate (void) const {return ();}

    };//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


    //-------------------------------------------------------------------
    class cls_DataColumns : public std::vector <class cls_DataColumn> {
    public://                                               ~~~~~~~~~~~~~

    // ~ ~ ~ Types definitions
    typedef std::vector <class cls_DataColumn> typ_DataColumns;
    typedef typ_DataColumns::iterator typ_DataColumnsIt;

    // . . . Public methods
      cls_DataColumns()
      {// costruttore di default

      }
      //~cls_DataColumns();


    // . . . Properties
      //__property bool IsOrdinate = { read = get_IsOrdinate };
      //__property bool IsAbscissa = { read = get_IsAbscissa };

    // . . . Public attributes (!)
      // . . . Column of values

    private://                                              ~~~~~~~~~~~~~

    // . . . Private attributes
      typ_DataColumnsIt ci; // iterator

    // . . . Private methods
    // . . . Properties

    };//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Class Definition ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class cls_StreamParser {

// ~~~~~~~~~~~~~ Types definitions                              ~~~~~~~~~~~~~
    typedef void __fastcall (__closure *fProgress)(int,bool&); // A progress function

public://                                                       ~~~~~~~~~~~~~

// ~~~~~~~~~~~~~ Types definitions
    // Options
    enum en_Option { opRigidDelim, // Rigid data delimiters
                     opEmbeddedDirsPriority // Priorità alle direttive nello stream
                    };
    typedef Set<en_Option, opRigidDelim, opEmbeddedDirsPriority> typ_Options;

// . . . Public methods
    cls_StreamParser(class cls_Log*, const String&, const String&);
    //~cls_StreamParser();

    bool Parse(const class TStream*,const String& = ""); // Import text data from stream given external directives if present


    void __fastcall ExportTo(TStrings*); // Export data to Strings
    void __fastcall LogColumnsSummary(); // Write the column structure summary in the log
    void __fastcall Split(String,bool); // Split columns with a pattern
    void __fastcall Reorder(String); // Reorder columns items (reindexing)
    void __fastcall UnMap(); // Reset abscissa mapping done by DecodeHeader (plot by index)

    void __fastcall ExecuteDirectives(); // ### Try to execute the directives



// . . . Properties
    __property typ_Options Options = { read=i_Options, write=i_Options };
    __property String HeaderChars = { read=get_HeaderChars, write=set_HeaderChars };
    __property String DataDelimiters = { read=get_DataDelimiters, write=set_DataDelimiters }; // Characters taken as data delimiters

    __property bool Abort = { read=i_Abort, write=i_Abort};
    
    __property uint XCount = { read=get_XCount };
    __property uint MaxColSize = { read=get_MaxColSize };
    __property typ_DataColumnsIt LongestCol = { read=get_LongestCol };
    __property int ColCount = { read=get_ColCount };

// . . . Public attributes (!)
    class cls_DataColumns Columns;

protected://                                                    ~~~~~~~~~~~~~

// . . . Protected attributes
    // . . . Class references
    const class TStream* Source; // Source stream
    class cls_Log* Log; // Owner's log

// . . . Protected methods

    // . . . Properties
    __property fProgress OnProgress = {read=i_OnProgress, write=i_OnProgress};
    fProgress i_OnProgress;

private://                                                      ~~~~~~~~~~~~~

// . . . Private attributes

    // . . . State
    // Position in stream is in Source->Position
    char c; // Current character
    uint Col; // Current character column
    uint Line; // Current line
    String Token; // Current token

    // . . . Auxiliary
    mat::stringlist EmbeddedDirectives; // A bag for encountered directives
    uint colcount; // Number of columns
    int firstxcol; // The column index of first abscissa
    mutable typ_DataColumnsIt ci; // An iterator

    // . . . Options:
    typ_Options i_Options;
    // Directive chars
    String brColumn,brAbscissa,brUnit; // brackets
    char chDirective; // prefix

// . . . Private methods
    void BuildDefaultColumns(int);
    void DecodeHeader(const mat::stringlist&);

    // . . . Parsing Stream
    inline double Read_double();

    // . . . Properties
    String get_HeaderChars (void) const {return brColumn+brAbscissa+brUnit+String(chDirective);}
    void set_HeaderChars (const String&);

    String i_DataDelimiters;
    String get_DataDelimiters (void) const {return i_DataDelimiters;}
    void set_DataDelimiters (const String&);

    // Auxiliary -- to datacolumns
    uint i_Xcount; // Number of abscissa columns
    uint get_XCount (void) const {return i_Xcount;}

    uint get_MaxColSize () const {if (Columns.empty()) return 0; uint M=0; for(uint i=0;i<Columns.size();i++) {if (M<Columns[i].Values.size()) M=Columns[i].Values.size();} return M;}
    typ_DataColumnsIt get_LongestCol () {if (Columns.empty()) return Columns.end(); typ_DataColumnsIt M=Columns.begin(); for(ci=Columns.begin()+1;ci!=Columns.end();ci++) {if (M->Values.size()<ci->Values.size()) M=ci;} return M;}
};//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


//---------------------------------------------------------------------------
#endif
