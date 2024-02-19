//---------------------------------------------------------------------------
#include <vcl.h>
#include "unt_MatCommon.h"
#pragma hdrstop
#include "unt_TimeCounter.h" // 'cls_TimeCounter'
#include "unt_Quantities.h" // 'cls_TimeQuantity'
#include "unt_Log.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//================================== Global =================================
    //cls_Log *Log =0;

//================================ File Scope ===============================
    cls_TimeCounter TimeCounter;

//============================== Static Members =============================
    bool cls_Log::IsHeaderDone = false; // Already printed initial header


//============================== Public methods =============================


//---------------------------------------------------------------------------
cls_Log::cls_Log (const String ownname, TStrings* stringlist) : OwnerName(ownname)
{ // Constructor

// . . . Valori di default
    DefaultSaveDir = IncludeTrailingBackslash(ExtractFileDir(Application->ExeName));
    MaxLines = 5000;
    Options >> loNumbered << loTimed >> loNamed << loVerbose >> loScrolling;
    IsInternalLines = false;

    // Eventi
    OnError = 0;
    OnEntry = 0;
    OnGetCallResult = 0;
    OnGetErrorMessage = 0;

// . . . Argomenti
    Link(stringlist);

// . . . Altre inizializzazioni
    CurrentLine = 1;
    SetLinePattern ("\t\t\t\t\t\t ---", // EntrySeparator
                    " ", // EntryPrefix
                    "    .", // SubEntryPrefix / indenting string
                    " | ", // NumberTimeSeparator
                    "  ", // TimeOwnernameSeparator
                    "  ", // OwnernameActionSeparator
                    " -- ", // ActionMessageSeparator
                    "(ok) ", // SuccessPrefix
                    "(X) ", // ErrorPrefix
                    "(!) " // WarnPrefix
                    );
    if (!IsHeaderDone) Clear();
}

//---------------------------------------------------------------------------
cls_Log::~cls_Log ()
{ // Destructor
    //Entry("Closing Log","Bye bye"); Avoid errors when Lines has cleared

// . . . Maintenance
    if ( IsInternalLines )
       {
        delete Lines;
        Lines=0;
        IsInternalLines = false;
       }

// . . . Saving
    //if ( Options.Contains(loAutosave) ) Save(); // Could be problematic
}


//---------------------------------------------------------------------------
void cls_Log::Link (TStrings* stringlist)
{ // Collega a una StringList esterna o ne crea una interna

    if ( IsInternalLines )
        {
         delete Lines;
         IsInternalLines = false;
        }

    if ( stringlist )
         {
          Lines = stringlist;
          IsInternalLines = false;
         }
    else {
          UnLink ();
         }
}

//---------------------------------------------------------------------------
void cls_Log::UnLink ()
{ // Crea una StringList interna

    if ( !IsInternalLines )
         {
          // Create new
          TStringList* l = new TStringList();
          // If Lines points to a valid TStringList, get its content
          // Ehmm, can't detect if Lines is disposed or not...
          //TStrings* L = dynamic_cast<TStrings*>(Lines);
          //if (L) l->Assign(L);
          Lines = l;
          IsInternalLines = true;
         }
}

//---------------------------------------------------------------------------
void cls_Log::SetLinePattern(const String EntrySeparator,
                             const String EntryPrefix,
                             const String SubEntryPrefix,
                             const String NumberTimeSeparator,
                             const String TimeOwnernameSeparator,
                             const String OwnernameActionSeparator,
                             const String ActionMessageSeparator,
                             const String SuccessPrefix,
                             const String ErrorPrefix,
                             const String WarningPrefix)
{ // Sets line layout strings
    sepEntry = EntrySeparator;
    prxEntry = EntryPrefix;
    prxSubEntry = SubEntryPrefix;
    sepNT = NumberTimeSeparator;
    sepTO = TimeOwnernameSeparator;
    sepOA = OwnernameActionSeparator;
    sepAM = ActionMessageSeparator;
    prxSuccess = SuccessPrefix;
    prxError = ErrorPrefix;
    prxWarn = WarningPrefix;
}

//---------------------------------------------------------------------------
void cls_Log::Save ( const String& path )
{ // Save in a directory

    //FILE * logfile = fopen (LogFileName.c_str(), "w");
    //if ( logfile == NULL ) ShowMessage("Errore nell'aprire il file \n" + LogFileName);
    //if ( fprintf (logfile, MemoLog->Text) == EOF ) ShowMessage("Errore nel salvare il log in \n" + LogFileName);
    //if ( fclose (logfile) == EOF ) ShowMessage("Errore nel chiudere il file \n" + LogFileName);

    //String Dir = IncludeTrailingBackslash(ExtractFileDir(SavePath));
    //String Suffix = ExtractFileName(SavePath);
    //String Prefix = ChangeFileExt(ExtractFileName(SavePath),"");
    //String Ext = ExtractFileExt(SavePath);

    Lines->SaveToFile (path);

    LastSavedLog = path; //::IncludeTrailingBackslash(path) + Now().FormatString ("yyyy'.'mm'.'dd'-'hh'.'nn'.'ss'.'zzz'_'") + OwnerName + ".log";
    Entry ("Log saved in", path);
}

//---------------------------------------------------------------------------
void __fastcall cls_Log::Clear()
{ // Svuota il Log
    Lines->Clear();
    PrintHeader();
}

//---------------------------------------------------------------------------
void __fastcall cls_Log::WinAPICall(const String& act, int retcode)
{ // WinAPI type: if fails returns a nonzero; error is retrieved with GetLastError

// . . . Update
    LastRetcode = retcode;
    LastWasError = bool (retcode);

    if ( !LastWasError )
        { // No winAPI errors
         Success(act);
        }
    else
        { // An error occurred
         Error(act,FormatErrMsg(retcode, mat::GetLastErrorMessage()));
        }
}

//---------------------------------------------------------------------------
void __fastcall cls_Log::Call(const String& act, int retcode)
{ // Calling integer-returning function
  // error and error message are retrieved with extern functions

// . . . Update
    LastRetcode = retcode;

    if ( OnGetCallResult )
        {
         LastWasError = OnGetCallResult(retcode);

         if ( !LastWasError )
            { // No winAPI errors
             Success(act);
            }
         else
            { // An error occurred
             WideString ErrMsg;
             if ( OnGetErrorMessage ) ErrMsg = OnGetErrorMessage(retcode);
             else ErrMsg = "Error not known";
             Error(act, FormatErrMsg(retcode, ErrMsg));
            }
        }
    else
        {
         LastWasError = true;
         Entry( act, "return code: " + String(retcode) );
        }
}

//---------------------------------------------------------------------------
void __fastcall cls_Log::Success (const String& act, const String& msg)
{ // Notifica un Successo
    if ( !Options.Contains(loVerbose) ) return; // Only if Verbose is true
    if ( msg.IsEmpty() ) SubEntry(prxSuccess + sepAM + act);
    else Entry(prxSuccess + act, msg);
}
//---------------------------------------------------------------------------
void __fastcall cls_Log::Error (const String& act, const String& msg)
{ // Notifica un Errore
    //::Beep(80,200)
    if ( msg.IsEmpty() ) SubEntry(prxError + sepAM + act);
    else Entry(prxError + act, msg);
    if (OnError) OnError(msg);
}
//---------------------------------------------------------------------------
void __fastcall cls_Log::Warn (const String& act, const String& msg)
{ // Avverti

    if ( msg.IsEmpty() ) SubEntry(prxWarn + sepAM + act);
    else Entry(prxWarn + act, msg);
}



//---------------------------------------------------------------------------
void __fastcall cls_Log::Entry (const String& act, const String& msg)
{ // Scrive un `Evento'

    // Linea di separazione
    Add (sepEntry);
    // Costruzione stringa
    LastEntry = prxEntry; //prxSubEntry
    if ( Options.Contains(loNumbered) ) LastEntry = LastEntry + String(CurrentLine++) + sepNT;
    if ( Options.Contains(loTimed) ) LastEntry = LastEntry + mat::GetTime() + sepTO;
    if ( Options.Contains(loNamed) ) LastEntry = LastEntry + "[" + OwnerName + "]" + sepOA;
    if ( msg.IsEmpty() ) LastEntry = LastEntry  + act;
    else {
          LastAction = act;
          LastEntry = LastEntry  + act + sepAM + msg;
         }

    // Scrittura
    Write (LastEntry);

    if (OnEntry) OnEntry(msg); // OnEntry event
}

//---------------------------------------------------------------------------
void __fastcall cls_Log::SubEntry (const String& msg, int level)
{ // Scrive un `SubEvento'
    LastSubEntry = msg;
    String prfx = prxSubEntry;
    for (int i=0; i<level; ++i) prfx += prxSubEntry;
    //AnsiString::StringOfChar('\t',level)
    Write (prfx + prxSubEntry + LastSubEntry);
}

//---------------------------------------------------------------------------
void __fastcall cls_Log::SubEntry (const std::vector<String>& sl)
{ // Scrive un `SubEvento'

    for(unsigned int i=0; i<sl.size(); ++i) Write(prxSubEntry + sl[i]);
}


//---------------------------------------------------------------------------
void cls_Log::Append (const String& str, const String& prx)
{ // Appends a String also containing `escape sequences' (\n,\t,ecc)
     ClearIfTooBig();
     if ( mat::HasNonVisibleChars(str) )
        { // Contains escape sequences (\n, ...)
             TStrings* strlst = new TStringList;
             strlst->SetText ( str.c_str() );
             Append (strlst,prx);
             delete strlst;
            }
     else Add(prx + str);
    if (Options.Contains(loScrolling)) Scroll();
}


//---------------------------------------------------------------------------
void cls_Log::StartTime (unsigned short n)
{ // Start time measure
    //StartTime = ::Time();
    TimeCounter.StartTime(n);
}

//---------------------------------------------------------------------------
String cls_Log::GetTime (unsigned short n)
{ // Get measured time
    //return (::Time()-StartTime).FormatString("hh':'nn':'ss'.'zzz");
    return String (cls_TimeQuantity(TimeCounter.GetTime(n)));
}



//=============================== Properties ================================
//---------------------------------------------------------------------------


//============================= Private methods =============================


//---------------------------------------------------------------------------
void cls_Log::PrintHeader()
{ // Print the initial header

    Title = OwnerName + " Log   (" + ::Date().FormatString("dd.mm.yyyy") + ")";
    String sep = String("        ") + AnsiString::StringOfChar('_', Title.Length());
    //Add(sep);
    Add(String("        ") + Title);
    Add(sep);
    Add("");
    IsHeaderDone = true;
}


/*---------------------------------------------------------------------------
int __fastcall cls_Log::NumChars (TStrings * sl)
{ // Retrieve the number of characters in a (TStrings)
    int nchars = 0;
    for (int i=0; i < sl->Count; i++)
        nchars += sl->Strings[i].Length();
    return nchars;
} */

//=============================== End of File ===============================
