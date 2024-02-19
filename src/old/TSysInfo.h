//---------------------------------------------------------------------------
#ifndef TSysInfoH
#define TSysInfoH
//---------------------------------------------------------------------------

	/*******************************************************
	*                                                      *
	*	Common Resources, qualche classe utile         *
	*                                                      *
	*               2004 - Matteo Gattanini                *
	*                                                      *
	*******************************************************/


//---------------------------------------------------------------------------
//			TSysInfo - System informations
//		(a class that retrieves some info with win32APIs)

class TSysInfo {
public:
	TSysInfo () {
    	}
//--------------------------------------------
    	// FeedBack delle funzioni
       	TFunctResults Results;

        // Struttura delle informazioni sul disco
        struct sDiskInfo{
                String Label;
                String Type;
        	double Size;
                String SizeUnit;
                double Free;
                String FreeUnit;
                String PrcFree;
                String Report;
        	} DiskInfo;

//--------------------------------------------

    	String WindowsVersion ();
    	String UserName ();
    	String ComputerName ();
    	String WindowsDir ();
    	String SystemDir ();
    	String EnvironmentVar (String);
    	String LastMsgError ();
    
    	void CPU (TStringList*);
    	void Memory (TStringList*);
    	void RetrieveDiskInfo (String);
// - - - - - - - - - - - - - - - - - - - - - -

//--------------------------------------------
}; // end Class TSysInfo definition

//---------------------------------------------------------------------------




//---------------------------------------------------------------------------
#endif
