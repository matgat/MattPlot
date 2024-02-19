//---------------------------------------------------------------------------
//#include <vcl.h>
#include "unt_MatCommon.h"
#pragma hdrstop

#include "unt_Tracks.h"
#include "unt_Frames.h"
//#include "unt_StreamParser.h"  // TODO: streamparser
#include "unt_TracksCollection.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------




///////////////////////////// cls_TracksCollection //////////////////////////

//============================= Static Members ==============================

//============================= Public methods ==============================


//---------------------------------------------------------------------------
cls_TracksCollection::cls_TracksCollection( const class cls_Frame* f ) : i_Owner(f)
{ // Constructor

// . . . Defaults
        i_SelectedIt = tracks.end();
        i_LongestName = "";
}

//---------------------------------------------------------------------------
cls_TracksCollection::~cls_TracksCollection()
{ // Destructor

        Clear();
}


//---------------------------------------------------------------------------
void cls_TracksCollection::Plot()
{ // Plot all
        for( tr=tracks.begin(); tr!=tracks.end(); ++tr ) (*tr)->Paint();
}

//---------------------------------------------------------------------------
class cls_Track* cls_TracksCollection::Find( const String& name )
{ // Returns the first track named 'name'

        for (tr=Tracks.begin(); tr!=Tracks.end(); ++tr) if ( (*tr)->Name == name ) return *tr;
        return 0;
}

//---------------------------------------------------------------------------
class cls_Track* cls_TracksCollection::Over(const TPoint& P) const
{ // Return clicked Track

        for ( tr=Tracks.begin(); tr!=Tracks.end(); ++tr ) if( (*tr)->IsOver(P) ) return *tr;
        return 0;
}


//---------------------------------------------------------------------------
void cls_TracksCollection::List(class TCanvas*, TRect&) const
{ // Draw a list (used in Legend)

        // Legend rectangle
        Margin = Frame->Canvas->TextHeight("X")/2;
        LineHeight = Frame->Canvas->TextHeight("X");
        StrokeLength = 2 * Margin;
        Width = 3*Margin + StrokeLength + Frame->Canvas->TextWidth( Frame->Tracks.LongestName );
        Height = Frame->Tracks.Count() * LineHeight + 2 * Margin;
        Frame->Canvas->Rectangle(BoundsRect);

        float n = 0.5;
        // TODO: cls_TracksCollection::List()
        for ( cls_Frame::typ_TracksIt tr=Frame->Tracks.begin(); tr!=Frame->Tracks.end(); ++tr )
            {
             if ( *tr == Frame->SelectedTrack) Frame->Canvas->Font->Style = TFontStyles() << fsBold << fsUnderline;

             TPoint P1 = TPoint(Left+Margin,Top+Margin+(n++)*LineHeight);
             TPoint P2 = TPoint(P1.x+StrokeLength,P1.y);
             (*tr)->Stroke(Frame->Canvas,P1,P2);
             Frame->Canvas->TextOut(P2.x+Margin,P2.y-LineHeight/2,(*tr)->Name);
             Frame->Canvas->Font->Style = TFontStyles() >> fsBold >> fsUnderline;
            }
}


//---------------------------------------------------------------------------
class cls_Track* cls_TracksCollection::OverList(const TPoint& P, TRect&) const
{ // Return clicked Track on list (used in Legend)
}





//---------------------------------------------------------------------------
void cls_TracksCollection::SelectNext ()
{ // Select next track

        // Null 'Selected' pointer if no tracks
        if ( tracks.empty() ) { Selected=0; return; }
        if ( ++i_Selected == tracks.end() ) i_Selected = tracks.begin(); // seleziono la prima
}

//---------------------------------------------------------------------------
void cls_TracksCollection::SelectPrevious ()
{ // Select previous track

        // Null 'Selected' pointer if no tracks
        if ( tracks.empty() ) { Selected=0; return; }
        if ( i_Selected == tracks.begin() ) i_Selected = tracks.rbegin(); // seleziono l'ultima
        else --i_Selected;
}


//---------------------------------------------------------------------------
void cls_TracksCollection::Add (class cls_TracksCollection*const ptrcoll)
{ // Add tracks from another collection

        ptrcoll
        for ( tr=frameptr->Tracks.begin(); tr!=frameptr->Tracks.end(); ++tr )
            {
             std::pair<typ_TracksIt, bool> r = Tracks.insert( *tr );
             if ( r.second ) (*tr)->InsertOwner(frameptr); // Update track owner list
            }

        // Legend update
        Owner->Legend->FitToContent();
}


//---------------------------------------------------------------------------
void cls_TracksCollection::Add( const class TStream* Source,
                                const String& DirectiveChars,
                                const String& DataDelimiters,
                                const String& PassedDirectives
                               )
{ // Read tracks from string

// . . . Data parser instantiation
        /*
        //cls_StreamParser* dat = new cls_StreamParser(Log,DirectiveChars,DataDelimiters);
		cls_StreamParser dat = cls_StreamParser(Log,DirectiveChars,DataDelimiters);

        // Set parser options according to given options
        if ( RigidDelimiters ) dat->Options << cls_TextDataReader::opRigidDelim;
        else dat->Options >> cls_TextDataReader::opRigidDelim;
        // Directives priority: external overwrite embedded
        dat->Options >> cls_StreamParser::opEmbeddedDirsPriority;

        // Perform parsing
        if ( dat->ImportFrom(Source, PassedDirectives) )
		     { // Build Tracks
			 }
		else {
		      Log->Error("Importing data","Something failed, check data source");
		     }
		//delete dat;
        */

// . . . Build Tracks
        typ_Tracks AddedTracks;
        cls_Track* newtrack = new cls_PolynomialTrack("trackX",clBlue);
        AddedTracks.insert( newtrack );
        //

// . . . Assign Tracks to a Frame and add to plot track-set

             // ###
             AddTrack( newtrack );
             Log->SubEntry("Assigning Track x to Frame " + String(SelectedFrame->Tag));
             SelectedFrame->InsertTrack( newtrack );

/*
        for ( tr=AddedTracks.begin(); tr!=AddedTracks.end(); ++tr )
            {
             // Add to plot track-set
             AddTrack( (*tr) );

             // Assign these tracks to the current selected Frame (prepared by PrepareFrame())
             Log->SubEntry("Assigning Track x to Frame " + String(SelectedFrame->Tag)); // ###
             SelectedFrame->InsertTrack( (*tr) );

             else Log->Error("No selected frame, cannot insert track to plot");
            }
*/

// . . . Finally
        //Invalidate();
}

//---------------------------------------------------------------------------
void cls_TracksCollection::Add ( class cls_Track* trackptr )
{ // Add a track to owned ones, managing name conflicts

        // . . . Check: do nothing if already exists
        if ( tracks.count(trackptr) )
           {
            Log->Warn("Trying to re-add an existing track!");
            return;
           }

        // . . . Avoid duplicate names
        static unsigned int k = 2; // duplicate value
        if ( Find(trackptr->Name) )
             { // Manage name conflict
              if ( k<=2 ) trackptr->Name += " (" + String(k++) + ")";
              else trackptr->Name = mat::Before(trackptr->Name," ") + " (" + String(k++) + ")";
              AddTrack(trackptr); // Call itself recursively
             }
        else { // Can add track
              k = 2; // reset counter
              tracks.insert( trackptr );

              // . . . Maintain coherence
              CheckInvariant();
             }
}


//---------------------------------------------------------------------------
void cls_TracksCollection::Remove(class cls_Track*const trackptr)
{ // Deallocate and erase a track

        if ( trackptr )
             {
              // Maintain selected track pointer
              if ( trackptr == Selected ) SelectPrevious();
              // Play_Sound(ID_WAV_DELTRACK); TODO: where?

              // Memory deallocation
              delete trackptr;

              tracks.erase( trackptr ); // Log->Warn("cls_MatPlot::ClearTrack","Track not found");

              // . . . Maintain coherence
              CheckInvariant();
             }
}



//---------------------------------------------------------------------------
void cls_TracksCollection::Clear()
{ // Clear all tracks

        // Deallocate memory
        for ( tr=Tracks.begin(); tr!=Tracks.end(); ++tr ) if ((*tr)) delete (*tr);
        //mat::ViewAddress((*tr));
        tracks.clear();

        // . . . Maintain coherence
        CheckInvariant();
}




//============================= Private Methods =============================


//---------------------------------------------------------------------------
void cls_TracksCollection::CheckInvariant ()
{ // Maintain coherence

        static bool NoSelected; NoSelected = true;

        for ( tr=tracks.begin(); tr!=tracks.end(); ++tr )
            {
             // Becomes false only if encounter 'Selected' pointer
             NoSelected = ( (*tr) != Selected ) && NoSelected;
             // Maintain longest name property
             if ( ((*tr)->Name).Length() > i_LongestName.Length() ) i_LongestName = (*tr)->Name;
            }

        // Maintain selected track
        if ( NoSelected ) SelectPrevious();
}



//------------------------------- Properties --------------------------------





//=============================== END OF FILE ===============================