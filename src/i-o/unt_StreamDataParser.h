#ifndef unt_StreamDataParserH
#define unt_StreamDataParserH
/*      ---------------------------------------------
        unt_StreamDataParser
        2008-2013 Matteo Gattanini
        
        Overview
        --------------------------------------------
        This unit defines class hierarchy to parse data streams
        and fills a set of arrays of numbers.

      cls_StreamParser         cls_StreamTextTableParser
           \                  /
        cls_StreamDataParser - cls_StreamCsvParser
                              \
                               cls_StreamMfileParser

        Features
        --------------------------------------------
        .Supports only 8bit chars
        .Fixed line delimiters ('\r', '\n' or couples of them)
        .Data delimiters cannot be '\r' or '\n'

        Licenses
        --------------------------------------------
        Use and modify freely
        
        Release history
        --------------------------------------------
        0.1 (aug.2008)
            First release
        
        Example of usage:
        --------------------------------------------
        #include "unt_StreamTextTableParser.h"
        cls_StreamTextTableParser* cols = new cls_StreamTextTableParser(Log);
        cols->Read(stringlist);
        //
        delete cols;

        DEPENDENCIES:
        ---------------------------------------------    */
        #include "unt_StreamParser.h"
        #include "unt_DataColumns.h"
        #include <Classes.hpp> // VCL classes like String,Exception


// . Settings
//#define PARSER_DEBUG


/////////////////////////////////////////////////////////////////////////////
class cls_StreamDataParser : public cls_StreamParser
{
 public:
    typedef enum {ST_ABORT,ST_SESSION_START,ST_HEADING_PART,ST_EXTERN_PART,ST_HEADING_END,ST_DATA_PART,ST_SESSION_END,ST_TERMINATE} en_ParseState;

    // . Public methods
    cls_StreamDataParser(class cls_Log*);
    //~cls_StreamDataParser();

    // . Public attributes (!)
    class cls_Columns Columns; // Collected data, organized in columns

 protected:

    bool __fastcall SeeComment (class TStream*); // Maybe there is a comment?
    String __fastcall GetLineStringUntilComment (class TStream*); // Get line string part before comment
    void __fastcall EatBlockComment (class TStream*); // Eat all until '*/' reached (or end)

    // Postprocessing
    void PerformSplit(cls_Columns&, const mat::numlist&); // Apply SPlit argument to columns
    void PerformResampling(cls_Columns&, const mat::cls_IndexSequence&); // Apply Resample argument to columns

    en_ParseState State;
    String ExtLines; // A bag for appended external lines
    bool ExtLinesParsed;
    cls_Columns TempCols; // Session collected columns container
    double Value; // Converted number bag

}; // cls_StreamDataParser



/////////////////////////////////////////////////////////////////////////////
class cls_StreamTextTableParser : public cls_StreamDataParser
{
 public:
    // . Public methods
    cls_StreamTextTableParser(class cls_Log*);
    //~cls_StreamTextTableParser();

    virtual bool Read(class TStream*,const String& =String(""));

    // . Public attributes (!)


 private:

    void SessionStateMachine ( class TStream* ); // Session data state machine
    en_ParseState ParseHeading (class TStream*);// Parse heading part
    en_ParseState ParseData (class TStream*); // Parse data part
    // Submodules
    void ReadDirective (class TStream*); // Read and process directive
    void ProcessDirective (const String&, const String&); // Process directive
    void AnalyzeFirstDataRow (class TStream*); // Analyze the first data row allocating columns container


    // Directives
    cls_DataHeader Header; // Data header
    mat::numlist Split; // split proportions
    mat::cls_IndexSequence Resample; // resample sequence

}; // cls_StreamTextTableParser


/////////////////////////////////////////////////////////////////////////////
class cls_StreamCsvParser : public cls_StreamDataParser
{
 public:
    // . Public methods
    cls_StreamCsvParser(class cls_Log*);
    ~cls_StreamCsvParser();

    virtual bool Read(class TStream*,const String& =String(""));

    // . Public attributes (!)

 private:
    void SessionStateMachine ( class TStream* ); // Session data state machine
    en_ParseState ParseHeading (class TStream*);// Parse heading part
    en_ParseState ParseData (class TStream*); // Parse data part

    class TStrings* Header; // Data header

}; // cls_StreamCsvParser


//---------------------------------------------------------------------------
#endif
