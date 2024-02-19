//---------------------------------------------------------------------------
#ifndef unt_LogH
#define unt_LogH
//---------------------------------------------------------------------------

        /* * * * * * * * * * * * * * * * * * * * * * * * * * * *
        *
        *    unt_Log    ver: 06    (mar.2009)
        *    2009 - matteo.gattanini@gmail.com
        *
        *    Overview
        *    --------------------------------------------
        *    This unit defines a class for logging actions,
        *    feedback results, report errors.
        *
        *    Licenses
        *    --------------------------------------------
        *    Use and modify freely
        *
        *    Release history
        *    --------------------------------------------
        *
        *    Example of usage:
        *    --------------------------------------------
        *    #include "unt_Log.h"
        *    cls_Log* Log = new cls_Log("TForm1",Memo2->Lines);
        *
        *    Options >> loNumbered  // numera linee
        *            << loTimed  // scrive tempo
        *            << loNamed  // nome proprietario
        *            >> loVerbose  // scrive tutto
        *            >> loScrolling  // scrolla
        *            >> loAutoSave; // salva all'uscita
        *
        *    // Chiamata API Windows
        *    Log->WinAPICall("Beep 50", Beep(50,1000) );
        *
        *    // Sono disponibili le informazioni:
        *    ShowMessage(Log->LastAction); // Nome chiamata
        *    // Valore della chiamata
        *    if ( Log->LastRetcode > 2 ) Log->Success("Good response");
        *    // Risultato della chiamata
        *    if ( Log->LastWasError ) Log->Error("Mmh, something wrong");
        *
        *    // Uso con funzione qualsiasi (ritorna intero)
        *
        *    // Stabilisci funzioni esterne
        *    Log->OnGetCallResult = MyGetCallResult;
        *    Log->OnGetErrorMessage = MyGetErrorMsg;
        *
        *    // Ad esempio:
        *
        *    //--------------------------------------------------
        *    bool __fastcall TForm1::MyGetCallResult(int errcode)
        *    { // Returns true if call result is an error
        *
        *        return ( errcode < 0 );
        *    }
        *
        *    //--------------------------------------------------
        *    String __fastcall TForm1::MyGetErrorMsg(int errcode)
        *    { // Returns error message
        *
        *        switch (errcode)
        *        {
        *          case 0 : return "No error";
        *          case -1 : return "Error one";
        *          default: return "Unknown error";
        *        }
        *    }
        *
        *    // Chiamata
        *    Log->Call ( "My function", CustomFunct() );
        *
        *    // Sono disponibili le informazioni:
        *    ShowMessage(Log->LastAction); // Nome chiamata
        *    // Valore della chiamata
        *    if ( Log->LastRetcode > 2 ) Log->Success(Log->LastAction,"Good response");
        *    // Risultato della chiamata
        *    if ( Log->LastWasError ) Log->Error("Mmh, something wrong");
        *
        *    // Scrivere:
        *    Log->Success("Message");
        *    Log->Error("Message");
        *    Log->Entry("Action","Message");
        *    Log->SubEntry("Message");
        *
        *    // Sono disponibili le informazioni:
        *    ShowMessage(Log->LastAction); // Ultima azione
        *    ShowMessage(Log->LastEntry); // Ultimo evento
        *    ShowMessage(Log->LastSubEntry); // Ultima scrittura
        *
        *    // Altri eventi
        *
        *    //--------------------------------------------------
        *    void TForm1::MyStringHandler(const String& msg)
        *    {
        *        String txt = "String: " + msg.Trim();
        *        Application->MessageBox(txt.c_str(),
        *                    Application->Title.c_str(),
        *                    MB_OK | MB_ICONSTOP);
        *        StatusBar->Panels->Items[3] = txt;
        *    }
        *
        *    // Handler errori chiamate
        *    Log->OnError = MyStringHandler;
        *    // Handler messaggi ( Log->Message("ciao"); )
        *    Log->OnEntry = MyStringHandler;
        *
        *    // Salvataggio
        *    Log->AutoSave = true; // salvataggio all'uscita
        *    Log->DefaultSaveDir = "C:\\Logs";
        *    Log->Save("C:\\Logs\\Backup");
        *
        *    //... and at the end:
        *    delete Log;
        * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*----------------------------- Line structure ------------------------------
    Entry
      |prxEntry| |Linecount|sepNT| |[Time]|sepTO| |[OwnerName]|sepOA| |Action|sepAM| |Message|
                  (Numbered)        (Timed)          (Named)
    Subentry
      |prxSubEntry| |Message|
*/

#include <vector>

//--------------------------- Class Definition ------------------------------
class cls_Log {//                    .....................
// . . . Type definitions
    // Event functions
    typedef void __fastcall (__closure *typ_StringHandler)(const String&);
    typedef bool __fastcall (__closure *typ_GetCallResultFunct)(int);
    typedef String __fastcall (__closure *typ_GetErrMsgFunct)(int);
    // Options
    enum en_Option { loVerbose, // Notifica tutto
                     loNumbered, // Numera le linee
                     loTimed, // Aggiungi l'istante temporale
                     loNamed, // Aggiungi il nome
                     //loAutosave, // Save on exit
                     loScrolling // Scroll after adding line
                   };
    typedef Set<en_Option, loVerbose, loScrolling> typ_Options;

public: //                        .....................
// . . . Public methods

    cls_Log (const String, TStrings* = 0);
    ~cls_Log ();
    
    // Maintenance
    void Link(TStrings* = 0); // Write to an external String List
    void UnLink(); // Create internal StringList
    void SetLinePattern(const String,const String,const String,const String,const String,const String,const String,const String,const String,const String);

    void Save (const String&); // Save log in a file
    void Save() {Save(DefaultSaveDir); } // Save in default directory

    // Main methods
    void __fastcall Clear (); // Cancella il Log e stampa un header
    void __fastcall WinAPICall(const String&, int); // Call a win API
    void __fastcall Call(const String&, int); // Call a function

    void __fastcall Success(const String&, const String&); // Notify a success
    void __fastcall Success(const String& s){Success(s,"");}
    void __fastcall Error(const String&, const String&); // Notify an error
    void __fastcall Error(const String& s){Error(s,"");}
    void __fastcall Warn(const String&, const String&); // Warn
    void __fastcall Warn(const String& s){Warn(s,"");}

    void __fastcall Entry(const String&,const String&); // Insert an entry
    void __fastcall Entry(const String& s){Entry(s,"");}
    void __fastcall SubEntry (const String&, int=0); // Insert a subentry
    void __fastcall SubEntry (const std::vector<String>&);
    void Append (const String&, const String&); // Appends a String also containing `escape sequences' (\n,\t,ecc)
    void Append (const String& s){Append(s,"\t| ");}
    void Append (TStrings* sl, const String prx)
        { // Appends a set of lines with an optional prefix
         ClearIfTooBig();
         for (int i=0; i<sl->Count; ++i) Lines->Add(prx+sl->Strings[i]);
         if (Options.Contains(loScrolling)) Scroll();
        }
    void Append (TStrings* sl){Append(sl,"\t| ");}
    void Add(const String& s){Lines->Add(s);} // Adds a String (plain)

    // Other
    void StartTime (unsigned short =0); // Start time measure
    String GetTime (unsigned short =0); // Get measured time

// . . . Properties
    __property typ_Options Options = {read=i_Options, write=i_Options};

// . . . Events
    __property typ_StringHandler OnError = {read=i_OnError, write=i_OnError};
    __property typ_StringHandler OnEntry = {read=i_OnEntry, write=i_OnEntry};
    
    __property typ_GetCallResultFunct OnGetCallResult = {read=get_OnGetCallResult, write=set_OnGetCallResult};
    __property typ_GetErrMsgFunct OnGetErrorMessage = {read=get_OnGetErrorMessage, write=set_OnGetErrorMessage};

// . . . Public attributes (!)
    int MaxLines; // Massimo numero di linee consentito
    String DefaultSaveDir; // Saving log default path

    // Records
    String LastAction; // Ultima azione effettuata
    String LastEntry; // Ultimo evento
    String LastSubEntry; // Ultima scrittura nel log
    int LastRetcode; // Valore della chiamata
    bool LastWasError; // Risultato della chiamata
    String LastSavedLog; // Path where log was saved
    String Title; // Valorized in 'PrintHeader'

private: //                        .....................

// . . . Private attributes
    const String OwnerName;
    TStrings* Lines; // Il contenuto del Log
    int CurrentLine; // Counter for numbering lines (fails with multi log)

    // Stringhe
    String sepEntry; // Linea di separazione tra due entries contigue
    String prxEntry, prxSubEntry; // Prefisso dei messaggi
    String sepNT,sepTO,sepOA,sepAM; // Stringhe separazione
    String prxSuccess,prxError,prxWarn; // Prefissi notifiche successi ed errori

    // Auxiliary
    //class TDateTime StartTime; // For time measuring
    static bool IsHeaderDone;
    bool IsInternalLines; // Created an internal TStringList

// . . . Properties
    typ_Options i_Options;
    TStrings* __fastcall get_LogLines () const {return Lines;} // Get property

// . . . Events
    typ_StringHandler i_OnError;
    typ_StringHandler i_OnEntry;

    typ_GetCallResultFunct i_OnGetCallResult;
    typ_GetCallResultFunct get_OnGetCallResult () const {return i_OnGetCallResult;}
    void __fastcall set_OnGetCallResult (typ_GetCallResultFunct h) {i_OnGetCallResult=h;}

    typ_GetErrMsgFunct i_OnGetErrorMessage;
    typ_GetErrMsgFunct get_OnGetErrorMessage () const {return i_OnGetErrorMessage;}
    void __fastcall set_OnGetErrorMessage (typ_GetErrMsgFunct h) {i_OnGetErrorMessage=h;}

// . . . Private methods
    void PrintHeader ();
    inline void Write (const String& str) {ClearIfTooBig();Lines->Add(str);if(Options.Contains(loScrolling))Scroll();} // For optimization avoiding 'has escape sequences' control of append
    //inline void ClearIfTooBig() {if(Lines->Count>MaxLines){Lines->Clear();Entry("cls_Log - clearing lines","Log max lines limit reached");}} // Clear string list if limit reached
    inline void ClearIfTooBig() {while(Lines->Count>MaxLines)Lines->Delete(3);} // Clear string list if limit reached
    inline void Scroll() {Lines->Append(" ");Lines->Delete(Lines->Count-1);}// Scrolls the Memo
    // or access TMemo* with: edt->SelStart = edt->GetTextLen();
    static inline String FormatErrMsg(const int ret,const String& msg) {return "("+String(ret)+") "+msg.Trim();} // Formats error string and corresponding code
    static inline WideString FormatErrMsg(const int ret,const WideString& msg) {return "("+String(ret)+") "+msg;}

};//-------------------------------------------------------------------------
//extern cls_Log *Log;
//---------------------------------------------------------------------------
#endif
