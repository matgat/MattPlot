//---------------------------------------------------------------------------

#ifndef unt_TracksCollectionH
#define unt_TracksCollectionH

        /* * * * * * * * * * * * * * * * * * * * * * * * * * * *
        *
        *        unt_Collections - Matteo Gattanini
        *                ver: 01    (17.mar.2007)
        *
        *        This unit defines classes to manage collections
        *        of tracks
        *
        *       cls_TracksCollection
        *
        *        Example of usage:
        *        #include "unt_TracksCollection.h"
        *
        * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include <set> // for track pointers





///////////////////////////// cls_TracksCollection //////////////////////////
//---------------------------------------------------------------------------
class cls_TracksCollection { // A set of tracks contained in a Frame

public://                                                       ~~~~~~~~~~~~~

// ~ ~ ~ Types definitions
        typedef std::set<class cls_Track*> typ_Tracks;
        typedef typ_Tracks::iterator typ_TracksIt;
        typedef typ_Tracks::const_iterator typ_TracksConstIt;

// . . . Public methods
        cls_TracksCollection ( class cls_Frame*const );
        ~cls_TracksCollection();

        // Main
        void Plot(); // Plot all

        // Status
        bool Empty() const {return tracks.empty();} // Tell is is empty
        uint Count() const {return tracks.size();} // Contained number
        bool Contains (class cls_Track*const pt) {return tracks.count(pt);} // Tell if contains a track
        class cls_Track* Find (const String& name); // First matching-name track
        class cls_Track* Over(const TPoint& P) const; // Return clicked Track
        void List(class TCanvas*, TRect&) const; // Draw a list (used in Legend)
        class cls_Track* OverList(const TPoint& P, TRect&) const; // Return clicked Track on list

        // Modifying
        void SelectNext(); // Select next track
        void SelectPrevious(); // Select previous track
        void Add(const class TStream* ,const String&,const String&,const String&); // Read tracks from stream
        void Add(class cls_Track*); // Add a track, managing name conflicts
        void Remove(class cls_Track*const); // Deallocate a track
        void Remove() {Remove(Selected);} // Deallocate selected track
        void Clear(); // Clear all tracks

// . . . Properties
        __property class cls_Frame* Owner = { read=i_Owner };
        __property class cls_Track* Selected = { read=get_Selected};
        __property String LongestName = { read=i_LongestName };

// . . . Public attributes (!)

private://                                                      ~~~~~~~~~~~~~

// . . . Private attributes
        typ_Tracks tracks; // set of tracks
        mutable typ_TracksIt tr; // an iterator

// . . . Properties
        class cls_Frame* i_Owner;
        typ_TracksIt i_SelectedIt;
        class cls_Track* get_Selected() const {return (static_cast<typ_TracksConstIt>(i_SelectedIt)!=tracks.end()) ? *i_SelectedIt : 0;}
        void set_Selected(class cls_Track*const pt)
                { if ( pt )
                      { tr = tracks.find(pt);
                        if ( tr!=tracks.end() ) i_SelectedIt = tr;
                      }
                  else i_SelectedIt = tracks.end();
                }

        String i_LongestName;

// . . . Private methods
        void CheckInvariant(); // Maintain coherence

};//-------------------------------------------------------------------------



//---------------------------------------------------------------------------
#endif
