/*
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.

   item.h is the definition for the item superclass
   item.cc is the implementation for this class
   item.txt is the documentation for this class
*/

#ifndef _ITEM_H_
#define _ITEM_H_

#include <fstream>
#include <string>

#include "effect.h"

// CONSTANTS
// ===================================================================

const unsigned int NUM_ATT = 4;  // number of attributes
const unsigned int NUM_CHR = 12; // number of characteristics
const unsigned int NUM_EFF = 5;  // number of effects

// ===================================================================


class item
{
 public:

  // CONSTRUCTORS AND DESTRUCTORS
  // ===================================================================

  item(); // default constructor
  item( const unsigned int ix, const string nm, const string gf,
	const unsigned int ty, const unsigned int at[], const bool ch[],
	const bool ae[], effect ef[], const unsigned int n );
          // "full" constructor
  item( const unsigned int ix, const string nm, const string gf,
	const unsigned int ty, const unsigned int n ); // "sparse" constructor
  item( const string fn ); // from file constructor

  // ===================================================================


  // OVERLOADED OPERATORS
  // ===================================================================

  item operator=( const item& it );
  bool operator==( const item& it ) const;
  bool operator!=( const item& it ) const;
  
  // ===================================================================


  // PUBLIC MEMBER VARIABLES
  // ===================================================================

  item *next; // pointer to the next item in the container
  item *prev; // pointer to the previous item in the container
  unsigned int number; // number of item of this type in this stack

  // ===================================================================


  // DEBUG FUNCTIONS
  // ===================================================================

  bool debug( bool val = true ); // turns debug on or off
  string pb( const bool val ) const; // returns a printable string for a
                                     // bool value

  // ===================================================================


  // "GET" FUNCTIONS, VALUE RETURNING
  // ===================================================================

  unsigned int getId() const; // returns ID number
  unsigned int getNumCombined() const; // returns number of combined items
  string getName() const; // returns name
  string getGfx() const; // returns gfx filename
  unsigned int getType() const; // returns type
  unsigned int getSize() const; // returns size
  unsigned int getWeight() const; // returns weight
  unsigned int getValue() const; // returns value
  unsigned int getUses() const; // returns number of uses
  bool getMoveable() const; // returns moveable?
  bool getContainer() const; // returns container?
  bool getLight() const; // returns light?
  bool getLocked() const; // returns locked?
  bool getOpenable() const; // returns openable?
  bool getUseable() const; // returns useable?
  bool getObtainable() const; // returns obtainable?
  bool getDroppable() const; // returns droppable?
  bool getCombineable() const; // returns combinable?
  bool getEquipable() const; // returns equipable?
  bool getUnequipable() const; // returns unequipable?
  bool getAttached() const; // is an item currently attached to this one?
  bool getObtainEffAct() const; // returns obtain effect active?
  bool getDropEffAct() const; // returns drop effect active?
  bool getEquipEffAct() const; // returns equip effect active?
  bool getUnequipEffAct() const; // returns unequip effect active?
  bool getUseEffAct() const; // returns use effect active
  effect* getObtainEff() const; // returns obtain effect
  effect* getDropEff() const; // returns drop effect
  effect* getEquipEff() const; // returns equip effect
  effect* getUnequipEff() const; // returns unequip effect
  effect* getUseEff() const; // returns use effect
  item* getAttItem() const; // returns a pointer to attached item

  // ===================================================================


  // "SET" FUNCTIONS, MODIFY
  // ===================================================================

  bool setId( const unsigned int val ); // sets ID number
  bool setName( const string val ); // sets name
  bool setGfx( const string val ); // sets gfx filename
  bool setType( const unsigned int val ); // sets type
  bool setSize( const unsigned int val ); // sets size
  bool setWeight( const unsigned int val ); // sets weight
  bool setValue( const unsigned int val ); // sets value
  bool setUses( const unsigned int val ); // sets number of uses
  bool setMoveable( const bool val ); // sets moveable?
  bool setContainer( const bool val ); // sets container?
  bool setLight( const bool val ); // sets light?
  bool setLocked( const bool val ); // sets locked?
  bool setOpenable( const bool val ); // sets openable?
  bool setUseable( const bool val ); // sets useable?
  bool setObtainable( const bool val ); // sets obtainable?
  bool setDroppable( const bool val ); // sets droppable?
  bool setCombineable( const bool val ); // sets combinable?
  bool setEquipable( const bool val ); // sets equipable?
  bool setUnequipable( const bool val ); // sets unequipable?
  bool setObtainEffAct( const bool val ); // sets obtain effect active?
  bool setDropEffAct( const bool val ); // sets drop effect active?
  bool setEquipEffAct( const bool val ); // sets equip effect active?
  bool setUnequipEffAct( const bool val ); // sets unequip effect active?
  bool setUseEffAct( const bool val ); // sets use effect active
  bool setObtainEff( effect& val ); // sets obtain effect
  bool setDropEff( effect& val ); // sets drop effect
  bool setEquipEff( effect& val ); // sets equip effect
  bool setUnequipEff( effect& val ); // sets unequip effect
  bool setUseEff( effect& val ); // sets use effect
  
  // ===================================================================


  // ADVANCED FUNCTIONS AND WRAPPERS
  // ===================================================================

  bool attach( item& it ); // attaches an item to this one
  bool detach(); // detaches the combined item

  // ===================================================================


 protected:

  
  // PROTECTED MEMBER VARIABLES
  // ===================================================================

  bool debug_mode; // is debug mode on or off?
  unsigned int id; // ID number
  string name; // name
  string gfx; // gfx filename
  unsigned int type; // type of item, see item.txt for indexes
  unsigned int item_att[NUM_ATT]; // attributes array, see item.txt
  bool item_chr[NUM_CHR]; // characteristics array, see item.txt
  bool item_ace[NUM_EFF]; // active effects array, see item.txt
  effect* item_eff[NUM_EFF]; // effects array, see item.txt
  item* attached; // pointer to combined item, see item.txt

  // ===================================================================


  // PROTECTED FUNCTIONS
  // ===================================================================

  bool setAttached( const bool val ); // sets Attached?

  // ===================================================================


};

#endif
