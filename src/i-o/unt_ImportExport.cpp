#include <vcl.h>
#include "unt_MatCommon.h"
#include "unt_Log.h"
#pragma hdrstop
#include "unt_ImportExport.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
/*
            File -> Process file
          /
    Input - Directory -> Process all files in directory
         \
           Archive -> Create directory in temp dir -> Process directory
*/

    // . Constants
    // File filters
    const String iex::FL_ALL_FILES     = "All files|*";
    const String iex::FL_ALL_TEXTFILES = "All data files (.mpl,.txt,.csv,.out,.dat)|*.mpl;*.txt;*.csv;*.out;*.dat";

    // . Instances
    iex::cls_IniFileType iex::IniFileType(true); // Always supported
    iex::cls_MPlotTextFileType iex::MPlotTextFileType(true); // Always supported
    iex::cls_MPlotXmlFileType iex::MPlotXmlFileType(false); // TODO 5: Not yet supported
    iex::cls_MPlotMFileType iex::MPlotMFileType(true); // Always supported
    iex::cls_TextFileType iex::TextFileType(true); // Always supported
    iex::cls_CsvFileType iex::CsvFileType(true); // Always supported
    iex::cls_GzFileType iex::GzFileType(false); // Not always supported
    iex::cls_SevenZipFileType iex::SevenZipFileType(false); // Not always supported
    iex::cls_EmfFileType iex::EmfFileType(true); // Always supported
    iex::cls_SvgFileType iex::SvgFileType(true); // Always supported

