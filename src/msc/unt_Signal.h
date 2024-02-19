//---------------------------------------------------------------------------
#ifndef unt_SignalH
#define unt_SignalH
//---------------------------------------------------------------------------

	/*******************************************************
	*                                                      *
	*	        A virtual signal class                 *
	*                                                      *
	*              Use:                                    *
	*         s1 = new cls_Signal(2,1,10);                 *
	*                                                      *
	*         s2 = new cls_Signal( &MySignal , 1.3);       *
	*                                                      *
	*         double MySignal (double)                     *
	*                                                      *
	*                                                      *
	*               2005 - Matteo Gattanini                *
	*                                                      *
	*******************************************************/


typedef double (__closure *CALCFUNCT)(double);

//========================== Class Definition ===============================
class cls_Signal { // Virtual signal

public://....................................................................

        cls_Signal( short=0, double=1, double=1 );
        cls_Signal( CALCFUNCT );
        //~cls_Signal();

// . . . Public methods

        double GetValue (double);

// . . . Public attributes

        // . . . Number of requests
        __property int n = { read = Rn,
                             write = Wn };

        // . . . An event handler
        __property CALCFUNCT CalculateValue = { read = RFunct,
                                                write = WFunct };

private://...................................................................

// . . . Private methods
        double Sine (double);
        double Ramp (double);
        double Trapezoidal (double);
        double Random (double);

// . . . Private attributes
        double A,T; // signal Amplitude and period

// . . . Properties

        int in; // Number of requests
        int Rn ();
        void Wn (int);

        // . . . Event
        CALCFUNCT iFunct; // internal pointer for event handler
        CALCFUNCT RFunct (); // Get Handler pointer
        void WFunct (CALCFUNCT); // Set Handler pointer

//...........................................................................
};//=========================================================================


//---------------------------------------------------------------------------
#endif
