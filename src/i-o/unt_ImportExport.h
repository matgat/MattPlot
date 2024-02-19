//---------------------------------------------------------------------------
#ifndef unt_ImportExportH
#define unt_ImportExportH
//---------------------------------------------------------------------------

       /* * * * * * * * * * * * * * * * * * * * * * * * * * * *
       *
       *     unt_ImportExport  ver: 01 (aug.2008)
       *     Matteo Gattanini
       *
       *     This unit defines facilities to import/export
       *     plot informations
       *
       *     Example of usage:
       *     #include "unt_ImportExport.h"
       *
       * * * * * * * * * * * * * * * * * * * * * * * * * * * */


//:::::::::::::::::::::::::::: Namespace content ::::::::::::::::::::::::::::
namespace nms_ImportExport
{
// . Constants
    // File filters
    extern const String FL_ALL_FILES;
    extern const String FL_ALL_TEXTFILES;

// . Functions
    //String BuildDirectiveString(const String& dname, const String& darg) {return String(DirectivePrefix) + dname + String(' ') + darg;}

// . Concrete classes
    //-----------------------------------------------------------
    class cls_FileType { // File type info
    public:
        cls_FileType(const String& d, const String& e, const bool en) : i_Extension(e), i_Description(d), i_Enabled(en) {;}

        void set_Enabled (const bool b) {i_Enabled=b;}
        bool Enabled() const {return i_Enabled;} // Tells if program can handle this type
        String Extension() const {return i_Extension;} // Get file type extension (with dot, ex: ".txt")
        //String Description() const {return i_Description;} // Get file type description
        String Filter() const {return i_Description+"\ ("+i_Extension+")|*"+i_Extension;} // Get dialog filter

     private:
        bool i_Enabled; // Tells if program can handle this type
        const String i_Extension; // File type extension (with dot, ex: ".txt")
        const String i_Description; // File type description
    };

    //-----------------------------------------------------------
    class cls_IniFileType : public cls_FileType {
    public:
        cls_IniFileType(const bool e) : cls_FileType("INI files",".ini",e) {;}
    };

    //-----------------------------------------------------------
    class cls_MPlotTextFileType : public cls_FileType {
    public:
        cls_MPlotTextFileType(const bool e) : cls_FileType("MPlot text files",".mpl",e) {;}
    };

    //-----------------------------------------------------------
    class cls_MPlotXmlFileType : public cls_FileType {
    public:
        cls_MPlotXmlFileType(const bool e) : cls_FileType("MPlot XML files",".mpx",e) {;}
    };

    //-----------------------------------------------------------
    class cls_MPlotMFileType : public cls_FileType {
    public:
        cls_MPlotMFileType(const bool e) : cls_FileType("Matlab scripts",".m",e) {;}
    };

    //-----------------------------------------------------------
    class cls_TextFileType : public cls_FileType {
    public:
        cls_TextFileType(const bool e) : cls_FileType("Text files",".txt",e) {;}
    };

    //-----------------------------------------------------------
    class cls_CsvFileType : public cls_FileType {
    public:
        cls_CsvFileType(const bool e) : cls_FileType("Comma Separated Values files",".csv",e) {;}
    };

    //-----------------------------------------------------------
    class cls_GzFileType : public cls_FileType {
    public:
        cls_GzFileType(const bool e) : cls_FileType("gzip compressed files",".gz",e) {;}
    };

    //-----------------------------------------------------------
    class cls_SevenZipFileType : public cls_FileType {
    public:
        cls_SevenZipFileType(const bool e) : cls_FileType("7z archives",".7z",e) {;}
    };

    //-----------------------------------------------------------
    class cls_EmfFileType : public cls_FileType {
    public:
        cls_EmfFileType(const bool e) : cls_FileType("Enhanced metafiles",".emf",e) {;}
    };

    //-----------------------------------------------------------
    class cls_SvgFileType : public cls_FileType {
    public:
        cls_SvgFileType(const bool e) : cls_FileType("Scalable Vector Graphics",".svg",e) {;}
    };

// . Instances
    extern cls_IniFileType IniFileType;
    extern cls_MPlotTextFileType MPlotTextFileType;
    extern cls_MPlotXmlFileType MPlotXmlFileType;
    extern cls_MPlotMFileType MPlotMFileType;
    extern cls_TextFileType TextFileType;
    extern cls_CsvFileType CsvFileType;
    extern cls_GzFileType GzFileType;
    extern cls_SevenZipFileType SevenZipFileType;
    extern cls_EmfFileType EmfFileType;
    extern cls_SvgFileType SvgFileType;

}//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
namespace iex = nms_ImportExport; // a short alias for this namespace


//---------------------------------------------------------------------------
#endif
