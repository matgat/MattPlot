//---------------------------------------------------------------------------
#ifndef TCalcoliCalendarioH
#define TCalcoliCalendarioH
//---------------------------------------------------------------------------

	/*******************************************************
	*                                                      *
	*	          Calcoli calendario                   *
	*                                                      *
	*               2004 - Matteo Gattanini                *
	*                                                      *
	*******************************************************/


//---------------------------------------------------------------------------
//				TCalcoliCalendario
//      ( Calcoli che riguardano il caledario , basato su Sysutils )

class TCalcoliCalendario {
private:
//------------------------------------------

//------------------------------------------
public:
	TCalcoliCalendario () {
        	       Adesso = Now();
                       // = Date() + Time();
        }
//------------------------------------------
	// Date components
 	Word anno,mese,giorno;
        // Time components
        Word h,m,s,ms;
        // Now
        TDateTime Adesso;
//------------------------------------------

	unsigned short Giorno();
	unsigned short Settimana();
        unsigned short GiorniAFineAnno();
        unsigned short AnnoProssimo();

// - - - - - - - - - - - - - - - - - - - - -

//------------------------------------------
};// end Class TCalcoliCalendario

//---------------------------------------------------------------------------




//---------------------------------------------------------------------------
#endif
