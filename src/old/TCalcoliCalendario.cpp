//---------------------------------------------------------------------------

#include "Constants.h"
#include "Functions.h"
#pragma hdrstop

#include "TCalcoliCalendario.h"

//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
//				TCalcoliCalendario
//      ( Calcoli che riguardano il caledario , basato su Sysutils )



//------------------------------------------
unsigned short TCalcoliCalendario::Giorno()
{
 // Giorni dall'inizio dell'anno

 	// Local variables
        TDateTime InizioAnno;

        TDateTime Oggi = Date();
        DecodeDate(Oggi, anno, mese, giorno);

        InizioAnno = EncodeDate(anno, 1, 1);

        return ( int(Oggi) - int(InizioAnno) + 1 );
}

//------------------------------------------
unsigned short TCalcoliCalendario::Settimana()
{
 // Numero della settimana

        // Local variables
        TDateTime InizioAnno;
	TDateTime Oggi = Date();

        DecodeDate(Oggi, anno, mese, giorno);
        InizioAnno = EncodeDate(anno, 1, 1);

        return ceil ( ( double(Giorno()) + DayOfWeek(InizioAnno) - 1 ) /7 );
}

//------------------------------------------
unsigned short TCalcoliCalendario::GiorniAFineAnno()
{
 // Giorni alla fine dell'anno
 	// Local variables
        TDateTime FineAnno;

        Adesso = Now();
        DecodeDate(Adesso, anno, mese, giorno);
        // DecodeTime(Adesso, h, m, s, ms);

        FineAnno = EncodeDate(anno+1, 1, 1);

        return ( round (double(FineAnno) - double(Adesso)) );
}

//------------------------------------------
unsigned short TCalcoliCalendario::AnnoProssimo()
{
 // Valore anno prossimo

        DecodeDate(Date(), anno, mese, giorno);

	return anno+1;
}


// int day = DayOfWeek(TDateTime Date);
// TDateTime Date = EncodeDate(Word Year, Word Month, Word Day);
// DecodeDate(TDateTime Date, Word &Year, Word &Month, Word &Day);
// TDateTime Time = EncodeTime(Word Hour, Word Min, Word Sec, Word MSec);
// DecodeTime(TDateTime Time, Word &Hour, Word &Min, Word &Sec, Word &MSec);
// bool IsLeapYear(Word Year);

// up: TCalcoliCalendario
//---------------------------------------------------------------------------