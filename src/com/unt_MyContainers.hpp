//---------------------------------------------------------------------------
#ifndef unt_MyContainersH
#define unt_MyContainersH
//---------------------------------------------------------------------------

        /* * * * * * * * * * * * * * * * * * * * * * * * * * * *
        *
        *    unt_MyContainers
        *    2009 - matteo.gattanini@gmail.com
        *
        *    Overview
        *    --------------------------------------------
        *    This unit contains some class that implement
        *    some data containers
        *
        *    Licenses
        *    --------------------------------------------
        *    Use and modify freely
        *
        *    Release history
        *    --------------------------------------------
        *    0.1 (apr.2009)
        *        .First draft
        *
        *    Example of usage:
        *    --------------------------------------------
        *    #include "unt_MyContainers.hpp"
        *
        * * * * * * * * * * * * * * * * * * * * * * * * * * * */

//---------------------------------------------------------------------------
#include "unt_MatCommon.h" // My common definitions

//---------------------------------------------------------------------------



//:::::::::::::::::::::::::::: Namespace content ::::::::::::::::::::::::::::
namespace nms_MatCommon
{


    //...................................................................
    template <class typ_Item> class cls_FastQueue // A simple queue
    { /******************************************************************
      *  cls_FastQueue<double> q(6);
      *  q.Push(45);
      *  double d = *q.Pop();
      *  for (q.i=0; q.i<q.Used; ++q.i) cout << q.CurrItem;
      ******************************************************************/
     public://                                              .............
       // . . . Type definitions
       typedef int typ_ItemIndex;
       // . . . Public methods
       cls_FastQueue<typ_Item>( typ_ItemIndex N ) : Size(N) {Clear(); Allocate();}
       ~cls_FastQueue<typ_Item>() { DeAllocate(); }

       //-----------------------------------------------------
       typ_Item* Push ( const typ_Item& newit )
          { // Push a new item in the last position of queue
           while ( i_Used >= Size ) // If max items number reached
                 {
                  //Overflow = true; // An overflow occurred
                  //if ( Overwrite ) Pop();
                  //else return 0; // Else refuse new item
                  Pop();
                 }
           // Now there is surely a free slot in:
           i_Last = ( i_Last + 1 ) % Size;
           // Note that initially q->last = -1 , so (q->last+1) is 0
           // First push
           if ( i_Used == 0 )
              {
               i_First = i_Last; // When no pop operation has yet occurred, set First
               //i_MaxItem = i_MinItem = newit;
              }
           // Now can push the new item in the 'last' slot
           Item = &(Items[i_Last]); // Remember pushed item
           Items[i_Last] = newit;
           ++i_Used; // Update used slots
           // Maintain Max and Min items
           //if ( newit > i_MaxItem ) i_MaxItem = newit;
           //else if ( newit < i_MinItem ) i_MinItem = newit;
           return Item;
          }
       //-----------------------------------------------------
       typ_Item* Pop()
          { // Remove current first item
           // Nothing to do if no items
           if ( i_Used <= 0 ) return 0; // Nothing to do if no items
           // Reset overflow state
           //q->Overflow = false;
           // Maintain Max and Min items
           //if ( Items[i_First] > i_MaxItem ) i_MaxItem = 0;
           //else if ( Items[i_First] < i_MinItem ) i_MinItem = 0;
           // Take and remember first item
           Item = &(Items[i_First]);
           // Now can remove first item
           i_First = (i_First+1) % Size;
           --i_Used; // Update used slots
           // No more items? Reset pointers
           if ( i_Used == 0 ) i_First = i_Last = -1;
           // Finally
           return Item;
          }
       //-----------------------------------------------------
       void Clear() {i_First=i_Last=-1; i_Used=0;}
       bool Empty() {return (i_Used == 0);}
       //-----------------------------------------------------
       //typ_ItemIndex NextIndex( typ_ItemIndex idx ) { return (idx+1)%Size; }
       //typ_ItemIndex PrevIndex( typ_ItemIndex idx ) { return (idx-1)%Size; }

       // . . . Operators
       //cls_FastQueue& operator= ( const cls_FastQueue& q ) { i_Used = q.Used; return *this;}

       // . . . Properties
       __property typ_ItemIndex First = { read = i_First }; // Indexes
       __property typ_ItemIndex Last = { read = i_Last };
       __property typ_ItemIndex Used = { read = i_Used }; // Used slots
       __property typ_ItemIndex Free = { read = get_Free }; // Free slots
       __property typ_Item FirstItem = { read = get_FirstItem }; // Item going to be served
       __property typ_Item LastItem = { read = get_LastItem }; // Last item
       __property typ_Item CurrItem = { read = get_CurrItem }; // Current iteration item
       //__property typ_Item MaxItem = { read = i_MaxItem }; // Maximum item
       //__property typ_Item MinItem = { read = i_MinItem }; // Minimum item

       // . . . Public attributes (!)
       const typ_ItemIndex Size;
       typ_Item* Items;
       typ_ItemIndex i; // An iterator

     private://                                             .............

       // . . . Private attributes
       typ_ItemIndex i_First, i_Last, i_Used;
       typ_Item* Item;
       //typ_Item i_MaxItem, i_MinItem;

       // . . . Properties
       inline typ_ItemIndex get_Free (void) const {return Size-i_Used;}
       inline typ_Item get_FirstItem (void) const {return Items[i_First];}
       inline typ_Item get_LastItem (void) const {return Items[i_Last];}
       inline typ_Item get_CurrItem (void) const {return Items[(i+i_First)%Size];}

       // . . . Private methods
       inline void Allocate(){Items = new typ_Item[Size];}// Allocate memory for queue
       inline void DeAllocate(){delete[] Items;}// Deallocate memory for queue
    }; // end 'cls_FastQueue'

    
    /*...................................................................
    template <class typ_Item> class cls_CircularBuffer // A circular buffer
    { /******************************************************************
      *  mat::numlist nl;
      *  cls_CircularBuffer<double> b(nl,nl.size());
      *  b.Cycle = 1;
      *  double d = b.Next();
      ******************************************************************
     public://                                              .............
       // . . . Type definitions
       typedef int typ_ItemIndex;
       // . . . Public methods
       cls_CircularBuffer<typ_Item>( typ_Item* a, typ_ItemIndex s ) {Items=0; Assign(a,s);}
       cls_CircularBuffer<typ_Item>( const std::vector<typ_Item>& v ) {Items=0; Assign(v);}
       ~cls_CircularBuffer<typ_Item>() { DeAllocate(); }
       //-----------------------------------------------------
       typ_Item Current() { return Items[idx]; } // Get current item
       typ_Item Next() {++idx; if (idx>=Size){idx=0;} return Current();} // Point to next item
       typ_Item Prev() {if (idx<=0){idx=Size;} --idx; return Current();} // Point to prev item
       //-----------------------------------------------------
       void Assign (typ_Item* a, typ_ItemIndex s){Reset(); Allocate(s); for(typ_ItemIndex i=0; i<s; ++i) Items[i] = a[i];}
       void Assign (const std::vector<typ_Item>& v) {Reset(); Allocate(v.size()); for(typ_ItemIndex i=0; i<v.size(); ++i) Items[i]=v[i];}
       void Reset() {i_Cycle=i_Current=0;}
       // . . . Properties
       //__property int Cycle = { read=i_Cycle, write=i_Cycle }; // Current cycle
       // . . . Public attributes (!)
       const typ_ItemIndex Size;
     private://                                             .............
       // . . . Private attributes
       typ_Item* Items;
       typ_ItemIndex idx; // Current item index inr i_Cycle;
       // . . . Private methods
       void Allocate(typ_ItemIndex s) { if (Items) DeAllocate();
                                        Size=s;
                                        Items=new typ_Item[Size]; }// Allocate memory
       void DeAllocate() {Size=0; delete[] Items; Items=0;}// Deallocate memory
    }; // end 'cls_CircularBuffer'
    */


}//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

#endif//=====================================================================
