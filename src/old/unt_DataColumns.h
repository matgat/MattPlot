//---------------------------------------------------------------------------

#ifndef unt_DataColumnsH
#define unt_DataColumnsH
//---------------------------------------------------------------------------

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
	*
	*	        cls_DataColumn, cls_DataColumns
	*	2006 - Matteo Gattanini
	*
	*	unt_DataColumns
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
        *       cls_DataColumns Data = cls_DataColumns(Log);
        *       Data.Load(Lines);
	*
	*	cls_DataColumns* cols = new cls_DataColumns(Log);
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
class cls_DataColumns {

// ~~~~~~~~~~~~~ Types definitions				~~~~~~~~~~~~~
        typedef std::vector <class cls_DataColumn> typ_DataColumns;
	typedef std::vector <class cls_DataColumn>::iterator typ_DataColumnsIt;

public://							~~~~~~~~~~~~~

// . . . Public methods
        cls_DataColumns(class cls_Log*);
        //~cls_DataColumns();

        String __fastcall ImportFrom(TStrings*,const mat::stringlist& = mat::stringlist()); // Import data from Strings
        void __fastcall ExportTo(TStrings*); // Export data to Strings
        void __fastcall LogColumnsSummary(); // Write the column structure summary in the log
        void __fastcall Split(String,bool); // Split columns with a pattern
        void __fastcall Reorder(String); // Reorder columns items (reindexing)
        void __fastcall UnMap(); // Reset abscissa mapping done by DecodeHeader (plot by index)

        void __fastcall ExecuteDirectives(); // ### Try to execute the directives

// . . . Operators

// . . . Properties
        //__property int ColCount = { read = get_ColCount };
        __property int XCount = { read = get_XCount };
        __property U32 MaxColSize = { read = get_MaxColSize };
        __property typ_DataColumnsIt LongestCol = { read = get_LongestCol };
        // . . . Header chars
        __property String Brackets_Column = { read = get_Brackets_Column, write = set_Brackets_Column };
        __property String Brackets_Abscissa = { read = get_Brackets_Abscissa, write = set_Brackets_Abscissa };
        __property String Brackets_Unit = { read = get_Brackets_Unit, write = set_Brackets_Unit };
        __property char Prefix_Directive = { read = get_Prefix_Directive, write = set_Prefix_Directive };

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
        U32 xcount; // Number of abscissa columns
        int firstxcol; // The column index of first abscissa
        mutable typ_DataColumnsIt ci; // An iterator

        // . . . Options: header chars
        String br_cl,br_ab,br_un;
        char px_dr;


// . . . Private methods
        void __fastcall BuildDefaultColumns(int);
        void __fastcall DecodeHeader(const mat::stringlist&);

        // . . . Properties
        U32 __fastcall get_MaxColSize () const {if (Columns.empty()) return 0; U32 M=0; for(U32 i=0;i<Columns.size();i++) {if (M<Columns[i].Values.size()) M=Columns[i].Values.size();} return M;}
        typ_DataColumnsIt __fastcall get_LongestCol () {if (Columns.empty()) return Columns.end(); typ_DataColumnsIt M=Columns.begin(); for(ci=Columns.begin()+1;ci!=Columns.end();ci++) {if (M->Values.size()<ci->Values.size()) M=ci;} return M;}
        int __fastcall get_XCount (void) const {return xcount;}
        String __fastcall get_Brackets_Column (void) const {return br_cl;}
        void __fastcall set_Brackets_Column (const String s) {if (s.Length()==2) br_cl=s;else ShowMessage("cls_DataColumns::Invalid Brackets");}
        String __fastcall get_Brackets_Abscissa (void) const {return br_ab;}
        void __fastcall set_Brackets_Abscissa(const String s) {if (s.Length()==2) br_ab=s;else ShowMessage("cls_DataColumns::Invalid Brackets");}
        String __fastcall get_Brackets_Unit (void) const {return br_un;}
        void __fastcall set_Brackets_Unit (const String s) {if (s.Length()==2) br_un=s;else ShowMessage("cls_DataColumns::Invalid Brackets");}
        char __fastcall get_Prefix_Directive (void) const {return px_dr;}
        void __fastcall set_Prefix_Directive (const char c) {px_dr=c;}

};//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


//---------------------------------------------------------------------------
#endif
