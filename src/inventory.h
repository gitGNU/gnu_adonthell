/*
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.


  this is the main class for the inventory subsystem.
  it uses a hash table based on the objects name to 
  allocate storage in the inventory. It supports inventory
  growth through prime sequences. A 101 element array can
  easily store hundreds more using linked lists inside the
  element. I felt this was optimal since a person isn't 
  carry that many objects, but allows for growth if they
  do. Searching is fast. A simple matter of grabbing the key
  and doing a linear search down a short list...
  
  methods to add: remove, get, etc.
*/

#ifndef _INVENTORY_H_
#define _INVENTORY_H_

#include <fstream>
#include <string>
#include <vector>
#include <math.h>

#include "item.h"
#include "types.h"

class inventory
{
 public:

  // CONSTANTS
  // ========================================================================

  const static u_int16 base = 37;     // needed by the hashcode generator
  const static u_int16 HASH_SIZE = 1; /* size of the hash array itself; the
					 number of items that can be held by
					 the inventory defaults to HASH_SIZE
					 * 5, but this can be changed with a
					 call to setMaxItems() */
    
  // ========================================================================

  // CONSTRUCTORS
  // ========================================================================

  inventory(); // default constructor
  inventory( const string fn ); /* from file constructor: given a filename, fn,
				   it sucks an inventory object in from file */
  /*
  ~inventory();
  */
  // The copy constructor is implicitly defined by the compiler

  // ========================================================================

  // OPERATORS
  // ========================================================================

  inventory* operator=( const inventory& it ); // assignment operator
  bool operator==( const inventory& it ) const; // equality operator
  bool operator!=( const inventory& it ) const; // inequality operator

  /* These operators add and subract the contents of an inventory.

     += works like this: if you have an inventory, A, that contains 27 items,
     and one, B, that contains 10 items, A += B will add the contents of B to
     A, thus making the number of items contained in A 37.

     -= is a little different: Given the same A and B from the last example,
     A -= B will subtract from A all the items that exist in a intersection of
     A and B, that is, everything that exists in *both* A and B. So, supposing
     that the intersection of A and B contains 7 items, A -= B will result in
     A having 20 items. */
     
  bool operator+=( const inventory& it ); // plus assignment operator
  bool operator-=( const inventory& it ); // minus assignment operator

  /* These operators are all comparisons on the number of items contained in
     the inventory */

  bool operator>( const inventory& it ) const; // greater than operator
  bool operator<( const inventory& it ) const; // less than operator
  bool operator>=( const inventory& it ) const; // greater than or equal to?
  bool operator<=( const inventory& it ) const; // less than or equal to?
  
  // ========================================================================

  // DEBUG FUNCTIONS
  // ========================================================================
  
  bool debug( const bool val="true" ); // turns cout debugging on/off (t/f)
  void showHash() const; // displays the contents of the hash
  void showDisp() const; // displays the display list

  // ========================================================================

  // GET FUNCTIONS (VALUE RETURNING)
  // ========================================================================

  u_int32 getNumItemO() const; /* returns the number of item objects currently
				  contained in the inventory */
  u_int32 getMaxItemO() const; /* returns the maximum number of items that
				  can be stored in the inventory (the default
				  is HASH_SIZE * 5); as storage is dynamically
				  allocated, this may be changed at will */
  u_int32 getNumItems() const; /* returns the number of items currently
				  contained in the inventory */
  u_int32 getMaxItems() const; /* returns the maximum number of item objects
				  that may be stored in the inventory (the
				  default is 200) */
  u_int32 getDispItems() const; /* returns the number of items and placeholders
				   in the display vector */
  u_int32 getWeight() const; /* returns the total weight of all items
				contained in the inventory */
  item* getItem( const u_int32 i ); /* returns a pointer to the item at
				       position i in the display list or NULL
				       if i is an invalid index */
  item* getItem( const string name ); /* returns a point to the item with the
					 name name or NULL if there are no
					 items with said name in the
					 inventory */

  // ========================================================================

  // SET FUNCTIONS (SET MEMBERS)
  // ========================================================================

  bool setMaxItemO( const u_int32 mi ); /* sets the maximum number of item
					   objects that may be stored in the
					   inventory; this defaults to
					   HASH_SIZE * 5 */
  bool setMaxItems( const u_int32 mi ); /* sets the maximum number of items
					   that may be stored (default is
					   200) */

  // ========================================================================

  // INSERTION/DELETION FUNCTIONS
  // ========================================================================

  bool insert( item* it ); // inserts an item into the inventory
  bool remove( const u_int32 i, const u_int16 n ); /* removes the item at index
						     i of the display list from
						     the inventory */
  bool remove( const string name, const u_int16 n ); /* removes n of the item
						       with the name name
						       from the inventory; if n
						       is 0, removes all of the
						       items with said name
						       (please note that a
						       deletion of an item with
						       this function takes
						       linear time, which I
						       hope to fix, but it is
						       much better to use the
						       other remove() when
						       possible) */

  // ========================================================================

  // MATHEMATICAL SET FUNCTIONS
  // ========================================================================

  inventory interSet( const inventory& it ) const; /* returns an inventory that
						      is the intersection of
						      *this and it */

  inventory unionSet( const inventory& it ) const; /* returns an inventory that
						      is the union of *this and
						      it */

  // ========================================================================

  // DISPLAY FUNCTIONS
  // ========================================================================

  bool swap( const u_int32 i1, const u_int32 i2 ); /* swaps the position in the
						      display list of the item
						      at indexes i1 and i2 */

  // ========================================================================

 private:

  // PROTECTED FUNCTIONS
  // ========================================================================
  
  bool setNumItemO( const u_int32 ni ); /* sets the number of item objects
					   currently in the inventory */
  bool setNumItems( const u_int32 ni ); /* sets the number of items currently
					   in the inventory */
  bool setWeight( const u_int32 tw ); /* sets the total weight of all
					 items in the inventory */
  u_int32 hashcode( const string name ); // generates a hashcode given a name
  bool getHash( const string name, u_int32& i, u_int32& d, u_int32& di );
                              /* for a given name 'name', sets the index 'i'
				 and depth 'd' of the item's location in the
				 hash table and index in the display vector
				 'di' or returns false if the item is not
				 found in the table */
  bool removeDisp( const string nm, const u_int16 n, const u_int32 i );
                              /* removes 'n' of the item with the index
				 'i' from the display list; 'nm' is just passed
				 as a precaution against accidental deletion */
  bool removeHash( const string nm, u_int16 n, 
		   const u_int32 i, const u_int32 d ); /* removes n of the item
							  at index 'i', depth
							  'd' from the hash and
							  sets 'di' to the
							  index of the item in
							  the display list;
							  'nm' is passed along
							  simply to ensure that
							  the wrong item is not
							  accidentally 
							  deleted */
  item* getPlaceholder() const; // returns a pointer to the placeholder item
  bool binarySearch( const u_int target, u_int32 first,
		     u_int32 last, u_int32& posi ); /* the good old binary
						       search algorithm, acting
						       on the placeholder
						       position stack */
  bool binaryPairSearch( const u_int target, u_int32 first,
			 u_int32 last, u_int32& posi ); /* the modified binary
							   search algorithm
							   that operates on the
							   placeholder position
							   stack and returns
							   the place to stick
							   the new placeholder
							   position */

  // ========================================================================

  // PROTECTED MEMBER VARIABLES
  // ========================================================================

  bool debug_mode; // if true, some cout debugging will be done
  u_int32 total_weight; // total weight of all items in the inventory
  u_int32 num_item_o; // number of item objects currently stored
  u_int32 max_item_o; // maximum number of item objects that may be stored
  u_int32 num_items; /* number of actual items that are stored in this
			inventory; this differs from num_items in that if you
			have an inventory containing:
			Short Sword (getNumber() == 1),
			Long Bow (getNumber() == 1), and
			Sheaf Arrows (getNumber == 27),
			num_item_o will be 3, but num_items will be 29 */
  u_int32 max_items; /* maximum number of items that may be stored (default is
			200); see the above note on num_items for an 
			explanation of the difference between max_item_o and
			max_items */
  item* inv[HASH_SIZE]; // hash table
  vector< item* > disp; // display list
  item* pl; // The placeholder item for the display vector
  vector< u_int32 > pl_pos; /* the positions of all placeholders in the display
			       vector */
};

#endif
