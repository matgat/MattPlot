//---------------------------------------------------------------------------

#ifndef unt_IniFileH
#define unt_IniFileH

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * *
    *
    *       cls_IniFile
    *   2006 - Matteo Gattanini
    *
    *   unt_IniFile (mar.2006)
    *       ver: 00    (jul.2006)
    *
    *   This unit defines a class to read and write
    *   `ini files' instead Borland's wrapper TIniFile
    *
    *   Example of usage:
    *
    *   #include "unt_IniFile.h"
    *   cls_IniFile* ini;
    *   ini = new cls_IniFile(Log,"C:\\Windows\\mat.ini");
    *   String ValueA;
    *   ini->get("sectionA","LabelA",ValueA);
    *   delete ini;
    *
    * * * * * * * * * * * * * * * * * * * * * * * * * * * */

//------------------------------ External files -----------------------------


/*--------------------------------- Example ---------------------------------
[section1]
; warning: spaces will be considered
label1=value1
[another sect]
label2=value2
; comment
label3=value3
*/


//--------------------------- Class Definition ------------------------------
class cls_IniFile {
public: //                              .............

// ~~~~~~~~~~~~~ Types definitions              ~~~~~~~~~~~~~
    //typedef struct { TStringList* List; int Line; } stu_Section;
    //typedef std::vector<double> numlist;

// . . . Public methods

    __fastcall cls_IniFile (class cls_Log*,const String& ,bool =true);
    __fastcall ~cls_IniFile ();

    bool __fastcall ValueExists(const String& sect, const String& lbl);
    String __fastcall Get(const String& sect, const String& lbl); // Get a value, returns true if the value is found
    bool __fastcall Set(const String& sect, const String& lbl, const String& towrite, bool Overwrite =true, const String& cmt ="", String def =""); // Set a value, returns true if the value is written

    void __fastcall Read(); // Read and digest file content
    void __fastcall Write(); // Write buffer to file content

// . . . Properties
    __property bool Trim = {read=i_Trim, write=i_Trim}; // Trim values from spaces

// . . . Events

// . . . Public attributes (!)
    //static const char SepChar; // the separator char between label and value
    static const String SectionBrackets; // the section characters
    static const char CommentChar; // the comment character
    const String FilePath;

private: //                                                     .............

// . . . Private attributes
    TStringList* Sections;
    class cls_Log* Log;
    bool Collapse; // Discard empty or nonsense lines (typically for reading only)
    // Auxiliary
    String PreviousWrittenComment;

// . . . Properties
    bool i_Trim;

// . . . Events

// . . . Private methods
    inline TStringList* __fastcall GetSection(const int& idx);
    inline TStringList* __fastcall GetSection(const String& sect);
    String __fastcall GetSectionTitle(const String& line);

    bool __fastcall IsSection (const String& line);
    inline bool __fastcall IsDefinition (const String& line);
    inline bool __fastcall IsComment (const String& line);

};//-------------------------------------------------------------------------



//---------------------------------------------------------------------------
#endif
