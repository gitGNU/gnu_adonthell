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

#include "types.h"
#include "effect.h"

class item
{
 public:

  // CONSTANTS
  // ===================================================================
  
  const static u_int8 NUM_ATT = 4;   // number of attributes
  const static u_int8 NUM_CHR = 12;  // number of characteristics
  const static u_int8 NUM_EFF = 5;   // number of effects
  const static u_int16 MAX_NUM = 99; // maximum number of items
  
  // ===================================================================

  // CONSTRUCTORS AND DESTRUCTORS
  // ===================================================================

  item(); // default constructor
  item( const u_int32 ix, const string nm, const string gf,
	const u_int8 ty, const u_int16 at[], const bool ch[],
	const bool ae[], effect ef[], const u_int16 n );
          // "full" constructor
  item( const u_int32 ix, const string nm, const string gf,
	const u_int8 ty, const u_int16 n ); // "sparse" constructor
  item( const string fn ); // from file constructor

  // ===================================================================


  // OVERLOADED OPERATORS
  // ===================================================================

  item operator=( const item& it );
  bool operator==( const item& it ) const;
  bool operator!=( const item& it ) const;
  bool operator++();
  bool operator--();
  bool operator+=( const item& it );
  bool operator-=( const item& it );
  bool operator>( const item& it ) const;
  bool operator<( const item& it ) const;
  bool operator>=( const item& it ) const;
  bool operator<=( const item& it ) const;

  // ===================================================================


  // PUBLIC MEMBER VARIABLES
  // ===================================================================

  item *next; // pointer to the next item in the container
  item *prev; // pointer to the previous item in the container

  // ===================================================================


  // PUBLIC DEBUG FUNCTIONS
  // ===================================================================

  bool debug( bool val = true ); // turns debug on or off

  // ===================================================================


  // "GET" FUNCTIONS, VALUE RETURNING
  // ===================================================================

  u_int16 getNumber() const; // returns number of items
  u_int32 getId() const; // returns ID number
  string getName() const; // returns name
  string getGfx() const; // returns gfx filename
  u_int8 getType() const; // returns type
  u_int16 getSize() const; // returns size
  u_int16 getWeight() const; // returns weight
  u_int16 getValue() const; // returns value
  u_int16 getUses() const; // returns number of uses
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
  u_int32 getPos() const; /* returns the index in the display list; *NOTE* that
			     if the item is not in a list, this property is
			     invalid, so be careful or you might "set up us the
			     segfault" */

  // ===================================================================


  // "SET" FUNCTIONS, MODIFY
  // ===================================================================

  bool setNumber( const u_int16 n ); // sets number of items
  bool setId( const u_int32 val ); // sets ID number
  bool setName( const string val ); // sets name
  bool setGfx( const string val ); // sets gfx filename
  bool setType( const u_int8 val ); // sets type
  bool setSize( const u_int16 val ); // sets size
  bool setWeight( const u_int16 val ); // sets weight
  bool setValue( const u_int16 val ); // sets value
  bool setUses( const u_int16 val ); // sets number of uses
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
  bool setPos( const u_int32 n ); // sets index in the display list

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
  u_int16 num; // number of items
  u_int32 id; // ID number
  string name; // name
  string gfx; // gfx filename
  u_int8 type; // type of item, see item.txt for indexes
  u_int16 item_att[NUM_ATT]; // attributes array, see item.txt
  bool item_chr[NUM_CHR]; // characteristics array, see item.txt
  bool item_ace[NUM_EFF]; // active effects array, see item.txt
  effect* item_eff[NUM_EFF]; // effects array, see item.txt
  item* attached; // pointer to combined item, see item.txt
  u_int32 pos; /* position in the display list (index of the item in the
		  inventory class's vector 'display' */

  // ===================================================================


  // PROTECTED FUNCTIONS
  // ===================================================================

  bool setAttached( const bool val ); // sets Attached?

  // ===================================================================

  // PROTECTED DEBUG FUNCTIONS
  // ===================================================================

  string pb( const bool val ) const; // returns a printable string for a
                                     // bool value

  // ===================================================================

};

#endif
