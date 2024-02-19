//---------------------------------------------------------------------------
#ifndef unt_TimeCountersH
#define unt_TimeCountersH
//---------------------------------------------------------------------------


    /* * * * * * * * * * * * * * * * * * * * * * * * * * * *
    *
    *    cls_TimeCounter
    *        -> cls_PerformanceCounter
    *        -> cls_WinTimeCounter
    *    2006 - Matteo Gattanini
    *
    *    unt_TimeCounters (mar.2006)
    *        ver: 01    (08.nov.2008)
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


//**************************** cls_TimeCounter ******************************

//--------------------------- Class Definition ------------------------------
class cls_TimeCounter {
public://                            .............
// . . . Types definition
        typedef void __fastcall (__closure *fastvoid)();

// . . . Public Methods

        __fastcall cls_TimeCounter (const String nm, unsigned short b) : Name(nm), size(b) {i_f = -1;}
        //__fastcall ~cls_TimeCounter ();
        double TimeToOverflow () {return ( ::pow(2,size) - Count ) / f;} // [s] Time to Registry overflow
        virtual bool __fastcall IsAvailable() = 0;

// . . . Properties
        __property unsigned __int64 Count = { read=get_Count}; // [counts] ticks passed from starting count
        __property double t = { read=get_t }; // [s] time passed from starting count
        __property double Dt = { read=get_Dt }; // [s] time passed from last read
        __property double f = { read=get_f, write=set_f }; // [Hz]=[count/s] frequency
        __property double dt = { read=get_dt, write=set_dt }; // [ms/count] resolution

// . . . Events
        __property fastvoid OnTime = { read=i_OnTime, write=i_OnTime };

// . . . Public attributes (!)
        const String Name; // Counter Name
        const unsigned short size; // [bits] registry dimension

protected://                            .............

// . . . Protected attributes

// . . . Protected properties
        double i_f; // [Hz]
        fastvoid i_OnTime; // internal pointer for event handler

        virtual unsigned __int64 __fastcall get_Count (void) = 0;
        virtual double __fastcall get_f (void) = 0;
        virtual void __fastcall set_f (const double) = 0;
        virtual double __fastcall get_dt (void) = 0;
        virtual void __fastcall set_dt (const double) = 0;

        double __fastcall get_t() { return Count/i_f;} // [s]
        double __fastcall get_Dt () const {static double t0=0; double Dt=t-t0; t0+=Dt; return Dt;} // [s]

// . . . Events
        
        
private://                            .............

// . . . Private attributes

// . . . Private methods


};//-------------------- end of cls_TimeCounter definition ------------------




//************************** cls_PerformanceCounter *************************
class cls_PerformanceCounter : public cls_TimeCounter {
public://                            .............

// . . . Public Methods
        __fastcall cls_PerformanceCounter ()  : cls_TimeCounter("Performance Counter", 64) {;}
        //__fastcall ~cls_PerformanceCounter ();

        virtual bool __fastcall IsAvailable() { LARGE_INTEGER i; return ::QueryPerformanceCounter(&i); }

// . . . Public attributes (!)

protected://                            .............

// . . . Protected properties
    virtual U64 __fastcall get_Count ()
        { // Get current counter value [ticks]
            LARGE_INTEGER x;
            ::QueryPerformanceCounter(&x);
            return x.QuadPart;
        }
    virtual double __fastcall get_f ()
        { // Get counting frequency [Hz]
            LARGE_INTEGER Freq;
            if ( QueryPerformanceFrequency(&Freq) ) i_f = double (Freq.QuadPart); // [Hz]
            else i_f = -1;
            return i_f;
        }
    virtual double __fastcall get_dt (void) {return 1000/f;} // [ms/count]
    virtual void __fastcall set_f (const double f){;}
    virtual void __fastcall set_dt (const double r){;}

private://                            .............

// . . . Private attributes

// . . . Private methods

};//------------------ end of cls_PerformanceCounter definition -------------



/*

//*************************** cls_WinTimeCounter ****************************
class cls_WinTimeCounter : public cls_TimeCounter {
public://                            .............

// . . . Public Methods
        __fastcall cls_WinTimeCounter () : cls_TimeCounter("Windows Timer", 32) { dt=1; }
        __fastcall ~cls_WinTimeCounter () { ::timeEndPeriod(1000/f); }
        virtual bool __fastcall IsAvailable() {return ::timeGetDevCaps(&wTimerRes,sizeof(wTimerRes)) == TIMERR_NOERROR; }

// . . . Properties
        __property double MaxResolution = { read=get_MaxResolution }; // Counter max resolution [ms/counts]

// . . . Public attributes (!)

protected://                            .............

// . . . Protected properties
        virtual U64 __fastcall get_Count() { return timeGetTime(); }
        virtual double __fastcall get_f () {return i_f; }
        virtual void __fastcall set_f (const double Hz) { dt=1000/Hz; }
        virtual double __fastcall get_dt ()
            {
                if ( ::timeGetDevCaps(&wTimerRes,sizeof(wTimerRes)) == TIMERR_NOERROR ) i_f = 1000/wTimerRes.wPeriodMin;
                else i_f = -1;
                return 1000/i_f;
            }
        virtual void __fastcall set_dt (const double ms)
            {
                if ( ::timeBeginPeriod(ms) == TIMERR_NOERROR ) i_f = 1000/ms;
                else ShowMessage("cls_WinTimeCounter:: Errore nel dichiarare la risoluzione del winTimer: " + String(ms));
            }
        virtual double __fastcall get_MaxResolution ()
            {
                if ( ::timeGetDevCaps(&wTimerRes,sizeof(wTimerRes)) == TIMERR_NOERROR ) return wTimerRes.wPeriodMax;
                else return -1;
            }

private://                            .............

// . . . Private attributes
        TIMECAPS wTimerRes;

// . . . Private methods

};//------------------ end of cls_WinTimeCounter definition -----------------

*/

//---------------------------------------------------------------------------
#endif
 
