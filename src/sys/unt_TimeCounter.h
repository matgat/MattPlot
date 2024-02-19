//---------------------------------------------------------------------------
#ifndef unt_TimeCounterH
#define unt_TimeCounterH
//---------------------------------------------------------------------------


    /* * * * * * * * * * * * * * * * * * * * * * * * * * * *
    *
    *    unt_TimeCounter (nov.2006)
    *
    *    2006 - Matteo Gattanini
    *
    *    cls_TimeCounter
    *        -> cls_PerformanceCounter
    *
    *    This unit defines a set of classes for
    *    counting time
    *
    *    Example of usage:
    *
    *    #include "unt_TimeCounter"
    *
    *    if ( cnt ) delete cnt;
    *    if (RadioGroup1->Items->Strings[RadioGroup1->ItemIndex] == "Performance Counter")
    *           cnt = new cls_PerformanceCounter();
    *    else if (RadioGroup1->Items->Strings[RadioGroup1->ItemIndex] == "Windows Timer")
    *           cnt = new cls_WinTimeCounter();
    *
    *    cnt->counter; // [counts] ticks passed from starting count
    *    cnt->t; // [s] time passed from starting count
    *    cnt->dt; // [s] time passed from last read
    *    cnt->TimeToOverflow();  // [s] Time to Registry overflow
    *
    *    #include "unt_TimeValue.h"
    *    Label1->Caption = cnt->counter;
    *    Label2->Caption = cls_TimeValue(cnt->t).StrRep;
    *    Label3->Caption = cls_TimeValue(cnt->dt).StrRep;
    *    Label4->Caption = cls_TimeValue(cnt->TimeToOverflow()).StrRep;
    *
    * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <Classes.hpp>

//**************************** cls_TimeCounter ******************************

//--------------------------- Class Definition ------------------------------
class cls_TimeCounter {
public://                            .............
// . . . Types definition
        //typedef void __fastcall (__closure *fastvoid)();

// . . . Public Methods
        __fastcall cls_TimeCounter() : Name("Performance Counter"), Size(64) {f;}
        //__fastcall ~cls_TimeCounter ();

        void StartTime (unsigned short n =0) {st[n] = t;} // Start time measure
        double GetTime (unsigned short n =0) const {return (t-st[n]);} // Get measured time [s]

// . . . Properties
        __property bool Available = { read=get_Available }; // Availability
        __property double TimeToOverflow = { read=get_TimeToOverflow }; // [s] Time to counter overflow
        __property unsigned __int64 Count = { read=get_Count }; // [counts] ticks passed from starting count
        __property double t = { read=get_t }; // [s] time passed from starting count
        //__property double Dt = { read=get_Dt }; // [s] time passed from last read
        __property double f = { read=get_f }; // [Hz]=[count/s] frequency
        __property double dt = { read=get_dt }; // [ms/count] resolution

// . . . Events
        //__property fastvoid OnTime = { read=i_OnTime, write=i_OnTime };

// . . . Public attributes (!)
        const String Name; // Counter Name
        const unsigned short Size; // [bits] registry dimension
        static const unsigned __int64 MaxCount;

private://                            .............

// . . . Private attributes
        double st[4]; // stored times
        //mutable double t0; // last time read

// . . . Properties
        double i_f; // [Hz]

        bool get_Available() { LARGE_INTEGER i; return ::QueryPerformanceCounter(&i); }
        double get_TimeToOverflow () {return ( MaxCount - Count ) / f;} // TODO: avoid constant overflow, suffix 'LL'

        unsigned __int64 get_Count () const
            { // Get current counter value [ticks]
                LARGE_INTEGER x;
                ::QueryPerformanceCounter(&x);
                return x.QuadPart;
            }
        double get_f ()
            { // Get counting frequency [Hz]
                LARGE_INTEGER Freq;
                if ( ::QueryPerformanceFrequency(&Freq) ) {i_f = double (Freq.QuadPart);} // [Hz]
                else i_f = -1;
                return i_f;
            }
        double get_dt() const {return 1000/i_f;} // [ms/count]

        double get_t() const { return Count/i_f;} // [s]
        //double get_Dt() const {double tmp=t0; return t-tmp;} // [s]

};//-------------------- end of cls_TimeCounter definition ------------------



//---------------------------------------------------------------------------
#endif

