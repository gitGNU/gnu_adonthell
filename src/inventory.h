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

/*
#include <stdlib.h>
*/

#include <fstream>
#include <string>
#include <math.h>

#include "item.h"

using namespace std;

class inventory
{
 public:

  // CONSTANTS
  // ========================================================================

  const static int DEF_MAX_SIZE = 99;
    
  // ========================================================================

  // CONSTRUCTORS
  // ========================================================================

  inventory(); // default constructor

  // ========================================================================


  // PUBLIC DEBUG FUNCTIONS
  // ========================================================================

  bool debug( const bool val = true );

  // ========================================================================

  // PUBLIC FUNCTIONS
  // ========================================================================

  bool insert( item& it ); // add an item to the inventory
  bool delete( const string name ); // delete an item from the inventory
  item* getItem( const string name );
  item* getItem( const unsigned int id );

  // ========================================================================

 protected:

  // PROTECTED MEMBER VARIABLES
  // ========================================================================

  bool debug_mode; // print debug messages?

  item *inv; // start of the inventory list
  unsigned int total_weight; // total weight
  unsigned int num_items; // number of items stored
  unsigned int max_items; // maximum number of items that may be stored

  // ========================================================================

  // PROTECTED DEBUG FUNCTIONS
  // ========================================================================

  string pb( const bool val ); // returns a printable string for a bool value

  // ========================================================================
};

#endif
