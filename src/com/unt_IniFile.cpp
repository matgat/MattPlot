//---------------------------------------------------------------------------
#include <vcl.h>
#include "unt_Log.h"
#include "unt_MatCommon.h"
#pragma hdrstop
#include "unt_IniFile.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------



//=============================== File scope ================================
        //#define INI_DEBUG // DONE: Comment debug define

//============================= Static Members ==============================
//const char cls_IniFile::SepChar = '=';
const String cls_IniFile::SectionBrackets = "[]";
const char cls_IniFile::CommentChar = ';';


//============================= Public methods ==============================

//---------------------------------------------------------------------------
__fastcall cls_IniFile::cls_IniFile(cls_Log* l, const String& pth, bool coll)
        : Log(l), FilePath(pth), Collapse(coll)
{ // Costruttore

// . . . Valori di default/Stato iniziale
    PreviousWrittenComment = "";
    Trim = false;

// . . . Altre operazioni
    Sections = new TStringList(); // prepare buffer
    Read(); // Read and digest file content
}

//---------------------------------------------------------------------------
__fastcall cls_IniFile::~cls_IniFile()
{ // Distruttore
    for (int i=0; i<Sections->Count; ++i) delete GetSection(i);
    delete Sections;
}


//---------------------------------------------------------------------------
bool __fastcall cls_IniFile::ValueExists(const String& sect, const String& lbl)
{ // See if a value exists

    // Retrieve desired section StringList
    TStringList* s = GetSection(sect);

    return s ? s->IndexOfName(lbl)>=0 : false;
}

//---------------------------------------------------------------------------
String __fastcall cls_IniFile::Get(const String& sect, const String& lbl)
{ // Get a value, returns true if a value is found

    // Retrieve desired section StringList
    TStringList* s = GetSection(sect);

    if (s)
       {
        String ValStr = s->Values[lbl];
        return Trim ? ValStr.Trim() : ValStr; // here decide if trim or not
       }
    else return "";
}

//---------------------------------------------------------------------------
bool __fastcall cls_IniFile::Set(const String& sect, const String& lbl, const String& towrite, bool Overwrite, const String& cmt, String def)
{ // Set a value to buffer, returns true if the value is written

    TStringList* s; // Section lines pointer

    // Section not yet existing? Add it
    int j = Sections->IndexOf(sect);
    if ( j < 0 )
         { // Section not existing
          s = new TStringList();
          Sections->AddObject(sect, static_cast<TObject*>(s));
         }
    else {
          s = GetSection(j);
         }

    // Value already existing? Overwrite or not
    j = s->IndexOfName(lbl);
    if ( j >= 0 )
         { // Value already exists!
          if (Overwrite) s->Values[lbl] = towrite;
          else return false;
         }
    else { // Value doesn't exists yet
          if ( cmt != PreviousWrittenComment )
             {
              if ( !cmt.IsEmpty() )
                 {
                  s->Add("");
                  if ( !def.IsEmpty() ) def = " {default:`" +def+ "´}";
                  s->Add(String(CommentChar)+" "+cmt+def); // Add comment
                 }
              PreviousWrittenComment = cmt;
             }
          s->Add(lbl+"="+towrite);
         }
    return true;
}


//---------------------------------------------------------------------------
void __fastcall cls_IniFile::Read()
{ // Read and digest file content

    // A check to avoid unnecessary work
    if ( !FileExists(FilePath) )
       {
        Log->SubEntry("Ini file not existing! What about creating a new one?");
        return;
       }

    // Get file content
    TStringList* Lines = new TStringList();
    try{
        // Try to read
        try { Lines->LoadFromFile(FilePath); }
        catch (...) {
                     String Msg = "Unable to read " + ExtractFileName(FilePath);
                     Log->Error("I/O error",Msg);
                     throw(Exception(Msg));
                    }

        // Now decode structure: fill a StringList for each section
        Sections->Clear();
        int i=0; // counter
        while ( i<Lines->Count )
              {
               if ( IsSection(Lines->Strings[i]) )
                   {
                    String Title = GetSectionTitle( Lines->Strings[i] );
                    TStringList* sectl; // Section lines pointer

                    // Not yet existing? Add it
                    int j = Sections->IndexOf(Title);
                    if ( j < 0 )
                         { // Section not existing
                          sectl = new TStringList();
                          Sections->AddObject(Title, static_cast<TObject*>(sectl));

                          #ifdef INI_DEBUG
                            Log->SubEntry("Got new section " + Title);
                          #endif
                         }
                    else {
                          sectl = GetSection(j);

                          #ifdef INI_DEBUG
                            Log->SubEntry("Got an existing section " + Title);
                          #endif
                         }
                    ++i; // next line

                    // Now add section lines 'til end or another section
                    while ( i<Lines->Count && !IsSection(Lines->Strings[i]) )
                          {
                           if (Collapse) { if ( IsDefinition(Lines->Strings[i]) ) sectl->Add(Lines->Strings[i]); }
                           else  sectl->Add(Lines->Strings[i]);
                           ++i; // next line
                          }
                    #ifdef INI_DEBUG
                      Log->Append(sectl,"\t|\t");
                    #endif
                   }
               else ++i; // next line
              }
       }
    __finally { delete Lines; }
}

//---------------------------------------------------------------------------
void __fastcall cls_IniFile::Write()
{ // Write file with current buffer

    // Prepare complete StringList
    TStringList* Lines = new TStringList();

    for (int i=0; i<Sections->Count; i++)
        {
         Lines->Add("");
         Lines->Add(String(SectionBrackets[1])+Sections->Strings[i]+String(SectionBrackets[2]));
         Lines->AddStrings( GetSection(i) );
        }

    // Now overwrite destination file
    Lines->SaveToFile(FilePath);

    delete Lines;
}


//=============================== Properties ================================

//================================= Events ==================================

//============================= Private methods =============================

//---------------------------------------------------------------------------
inline TStringList* __fastcall cls_IniFile::GetSection(const int& idx)
{ // Retrieve desired section lines
    return static_cast<TStringList*>(Sections->Objects[idx]);
}

//---------------------------------------------------------------------------
inline TStringList* __fastcall cls_IniFile::GetSection(const String& sect)
{ // Retrieve desired section lines

    int j = Sections->IndexOf(sect);
    if ( j<0 )
       {
        Log->Warn("Section `" +sect+ "´ not existing");
        return 0; // Section not existing
       }
    else return GetSection(j);
}

//---------------------------------------------------------------------------
String __fastcall cls_IniFile::GetSectionTitle(const String& line)
{ // Returns a section line title

    // prepare
    int len = line.Length();
    String title = "";

    int i=1; // counter

    // Reach left delimiter
    while ( (i<=len) && (line[i]!=SectionBrackets[1]) ) i++;
    i++; // next char
    // Get all until right delimiter
    while ( (i<=len) && (line[i]!=SectionBrackets[2]) ) title += line[i++];

    return title;
}

//---------------------------------------------------------------------------
bool __fastcall cls_IniFile::IsSection(const String& line)
{ // State if a line is a section

    // prepare
    int len = line.Length();
    bool l_reached = false;
    int i=1; // counter

    // Initial characters: allowed only spaces and tabs
    while ( (i<=len) && (line[i]!=SectionBrackets[1]) )
          {
           if ( line[i]!=' ' || line[i]!='\t' ) return false;
           i++;
          }
    // Rest of line: assure there are the two brackets
    while ( i<=len )
          {
           if ( line[i] == SectionBrackets[1] )
              {
               if ( l_reached ) return false; // double '.[..[.' encountered
               else l_reached = true;
              }
           else if ( line[i] == SectionBrackets[2] )
              {
               if ( l_reached ) return true; // ignore the rest
               else return false; // no '[' found
              }
           i++; // next char
          }// end string parsing
    return false;
}

//---------------------------------------------------------------------------
inline bool __fastcall cls_IniFile::IsDefinition(const String& line)
{ // State if a line is of type name=value
    return line.Pos("=") > 1;
    // or (File->Names[line index]).IsEmpty()
}

//---------------------------------------------------------------------------
inline bool __fastcall cls_IniFile::IsComment(const String& line)
{ // State if a line is a comment

    int i=1;
    int len = line.Length();
    // line mustn't be empty
    if ( len < 1 ) return false;
    // Ignore spaces
    while ( (i<len) && mat::IsSpacingChar(line[i]) ) i++;
    // See first non-space char
    return ( line[i] == CommentChar );
}

//=============================== End of File ===============================
