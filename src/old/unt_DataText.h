//---------------------------------------------------------------------------

#ifndef unt_DataTextH
#define unt_DataTextH
//---------------------------------------------------------------------------

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
	*
	*	        cls_DataColumn, cls_DataText
	*	2006 - Matteo Gattanini
	*
	*	unt_DataText
	*		ver: 01    (04.jun.2006)
	*
	*	This unit defines a class that represents
	*	a set of data column ready to be passed to
        *       a plotting utility. Here are defined the
        *       data header declaration and directives sintax
        *
        *       Header example
        *       «x [ms]» «r(x) [Km]» «sins(0.01ms)» «exp(x) [A]»
        *       Directive example
        *       #split 4|8|16
	*
	*	Example of usage:
        *       #include "unt_DataColumns.h"
        *       cls_DataText Data = cls_DataText(Log);
        *       Data.Load(Lines);
	*
	*	cls_DataText* cols = new cls_DataText(Log);
	*       cols->ImportFrom(stringlist);
        *       // note that columns must have the same size or
        *       // be in descending size to avoid confusion
	*       delete cols;
	*
	* * * * * * * * * * * * * * * * * * * * * * * * * * * */


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Header inclusions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include <vector>



        //~~~~~~~~~~~~~~~~~~~~~~~~~ Class Definition ~~~~~~~~~~~~~~~~~~~~~~~~
        class cls_DataColumn {
        public://                                               ~~~~~~~~~~~~~

        // ~ ~ ~ Types definitions
                typedef std::vector <double> typ_DataColumn;
	        typedef std::vector <double>::iterator typ_DataColumnIt;

        // . . . Public methods
                cls_DataColumn(){// costruttore di default
                                 Label=Unit=IdxUnit="";
                                 Scale=IdxScale=1;
                                 XLabel="";
                                 XIndex=-1; // Here the default behaviour: plot by index
                                 IsUnsorted = false;
                                 Values.reserve(100);
                                }
        //~cls_DataColumn();

                void AssignExceptValues(const cls_DataColumn& dc){// un assegnamento
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
                // . . . Column of values
                typ_DataColumn Values;

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



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Class Definition ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class cls_DataText {

// ~~~~~~~~~~~~~ Types definitions				~~~~~~~~~~~~~
        typedef std::vector <class cls_DataColumn> typ_DataColumns;
	typedef std::vector <class cls_DataColumn>::iterator typ_DataColumnsIt;

public://							~~~~~~~~~~~~~

// . . . Public methods
        cls_DataText(class cls_Log*, const String&, const String&);
        //~cls_DataText();

        String __fastcall ImportFrom(TStrings*,const mat::stringlist& = mat::stringlist()); // Import data from Strings
        void __fastcall ExportTo(TStrings*); // Export data to Strings
        void __fastcall LogColumnsSummary(); // Write the column structure summary in the log
        void __fastcall Split(String,bool); // Split columns with a pattern
        void __fastcall Reorder(String); // Reorder columns items (reindexing)
        void __fastcall UnMap(); // Reset abscissa mapping done by DecodeHeader (plot by index)

        void __fastcall ExecuteDirectives(); // ### Try to execute the directives

// . . . Operators

// . . . Properties
        __property String DataDelimiters = { read=get_DataDelimiters, write=set_DataDelimiters }; // Characters taken as data delimiters
        //__property int ColCount = { read=get_ColCount };
        __property U32 XCount = { read=get_XCount };
        __property U32 MaxColSize = { read=get_MaxColSize };
        __property typ_DataColumnsIt LongestCol = { read=get_LongestCol };
        // . . . Header chars
        __property String HeaderChars = { read = get_HeaderChars, write=set_HeaderChars };

// . . . Public attributes (!)
        typ_DataColumns Columns;
        mat::stringlist Directives; // A bag for encountered directives

protected://							~~~~~~~~~~~~~

// . . . Protected attributes

// . . . Protected methods
        // . . . Properties

private://							~~~~~~~~~~~~~

// . . . Private attributes
        // . . . Class references
        class cls_Log* Log; // Owner's log

        // . . . Auxiliary
        U32 colcount; // Number of columns
        int firstxcol; // The column index of first abscissa
        mutable typ_DataColumnsIt ci; // An iterator

        // . . . Options: header chars
        String Brackets_Column,
               Brackets_Abscissa,
               Brackets_Unit;
        char Prefix_Directive;


// . . . Private methods
        void __fastcall BuildDefaultColumns(int);
        void __fastcall DecodeHeader(const mat::stringlist&);

        // . . . Properties
        // Auxiliary
        U32 i_Xcount; // Number of abscissa columns
        U32 __fastcall get_XCount (void) const {return i_Xcount;}

        U32 __fastcall get_MaxColSize () const {if (Columns.empty()) return 0; U32 M=0; for(U32 i=0;i<Columns.size();i++) {if (M<Columns[i].Values.size()) M=Columns[i].Values.size();} return M;}
        typ_DataColumnsIt __fastcall get_LongestCol () {if (Columns.empty()) return Columns.end(); typ_DataColumnsIt M=Columns.begin(); for(ci=Columns.begin()+1;ci!=Columns.end();ci++) {if (M->Values.size()<ci->Values.size()) M=ci;} return M;}

        String i_DataDelimiters;
        String __fastcall get_DataDelimiters (void) const {return i_DataDelimiters;}
        void __fastcall set_DataDelimiters (const String&);

        String __fastcall get_HeaderChars (void) const {return Brackets_Column+Brackets_Abscissa+Brackets_Unit+String(Prefix_Directive);}
        void __fastcall set_HeaderChars (const String);

};//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


//---------------------------------------------------------------------------
#endif
