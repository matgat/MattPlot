//---------------------------------------------------------------------------

#ifndef unt_CollectionsH
#define unt_CollectionsH

        /* * * * * * * * * * * * * * * * * * * * * * * * * * * *
        *
        *        unt_Collections - Matteo Gattanini
        *                ver: 01    (17.mar.2007)
        *
        *        This unit defines classes to manage collections
        *        of frames and tracks
        *
        *       cls_TracksCollection
        *       cls_FramesCollection
        *
        *        Example of usage:
        *        #include "unt_Collections.h"
        *
        * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include <set> // for track pointers



/*///////////////////////////// cls_Collection //////////////////////////////
//---------------------------------------------------------------------------
template <class T>
class cls_Collection { // A set of objects

public://                                                       ~~~~~~~~~~~~~

// ~ ~ ~ Types definitions

// . . . Public methods
        cls_Collection ();
        ~cls_Collection();

        // Status
        virtual bool Empty() const =0; // Tell is is empty
        virtual unsigned int Count() const =0; // Contained number
        virtual bool Contains (const T) =0; // Tell if contains the object
        virtual T Find (const String& name) =0; // First matching-name track
        virtual T Over(const TPoint& P) const =0; // Return clicked object
        virtual void List(class TCanvas*) const =0; // Draw a list

        // Modifying
        virtual void SelectNext() =0; // Select next
        virtual void SelectPrevious() =0; // Select previous
        virtual void Add(T) =0; // Add an object
        virtual void Remove(const T) =0; // Deallocate and erase an object
        virtual void Clear() =0; // Clear all

// . . . Properties

// . . . Public attributes (!)

private://                                                      ~~~~~~~~~~~~~

// . . . Private attributes

// . . . Properties

// . . . Private methods

};//-----------------------------------------------------------------------*/




//---------------------------------------------------------------------------
#endif
