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

#include "item.h"

// CONSTRUCTORS AND DESTRUCTORS
// ===================================================================

item::item() // default constructor
{
  debug_mode = false;
  next = NULL;
  prev = NULL;
  setNumber( 1 );
  setId( 0 );
  setName( "" );
  setGfx( "" );
  setType( 0 );
  setSize( 0 );
  setWeight( 0 );
  setValue( 0 );
  setUses( 0 );
  setMoveable( false );
  setContainer( false );
  setLight( false );
  setLocked( false );
  setOpenable( false );
  setUseable( false );
  setObtainable( true );
  setDroppable( true );
  setCombineable( true );
  setEquipable( false );
  setUnequipable( false );
  setAttached( false );
  setObtainEffAct( false );
  setDropEffAct( false );
  setEquipEffAct( false );
  setUnequipEffAct( false );
  setUseEffAct( false );
  for( u_int8 i = 0; i < NUM_EFF; i++ )
    item_eff[i] = NULL;
  attached = NULL;
  pos = 0;
}

item::item( const u_int32 ix, const string nm, const string gf,
	    const u_int8 ty, const u_int16 at[],
	    const bool ch[], const bool ae[], effect ef[],
	    const u_int16 n )
  // "full" constructor
{
  debug_mode = false;
  next = NULL;
  prev = NULL;
  setNumber( n );
  setId( ix );
  setName( nm );
  setGfx( gf );
  setType( ty );
  for( u_int8 i = 0; i < NUM_ATT; i++ )
    item_att[i] = at[i];
  for( u_int8 i = 0; i < NUM_CHR; i++ )
    item_chr[i] = ch[i];
  for( u_int8 i = 0; i < NUM_EFF; i++ )
  {  
    item_ace[i] = ae[i];
    item_eff[i] = &ef[i];
  }
  attached = NULL;
  pos = 0;
}

item::item( const u_int32 ix, const string nm, const string gf,
	    const u_int8 ty, const u_int16 n ) // sparse constructor
{
  debug_mode = false;
  next = NULL;
  prev = NULL;
  setNumber( n );
  setId( ix );
  setName( nm );
  setGfx( gf );
  setType( ty );
  setSize( 0 );
  setWeight( 0 );
  setValue( 0 );
  setUses( 0 );
  setMoveable( false );
  setContainer( false );
  setLight( false );
  setLocked( false );
  setOpenable( false );
  setUseable( false );
  setObtainable( true );
  setDroppable( true );
  setCombineable( true );
  setEquipable( false );
  setUnequipable( false );
  setAttached( false );
  setObtainEffAct( false );
  setDropEffAct( false );
  setEquipEffAct( false );
  setUnequipEffAct( false );
  setUseEffAct( false );
  for( u_int8 i = 0; i < NUM_EFF; i++ )
    item_eff[i] = NULL;
  attached = NULL;
  pos = 0;
}

item::item( const string fn ) // from file constructor
{
  // not yet implemented (less work for me!)
}

// ===================================================================


// OVERLOADED OPERATORS
// ===================================================================

item item::operator=( const item& it )
{
  if( *this != it )
  {
    setNumber( it.getNumber() );
    setName( it.getName() );
    setGfx( it.getGfx() );
    setType( it.getType() );
    setSize( it.getSize() );
    setWeight( it.getWeight() );
    setValue( it.getValue() );
    setUses( it.getUses() );
    setMoveable( it.getMoveable() );
    setContainer( it.getContainer() );
    setLight( it.getLight() );
    setLocked( it.getLocked() );
    setOpenable( it.getOpenable() );
    setUseable( it.getUseable() );
    setObtainable( it.getObtainable() );
    setDroppable( it.getDroppable() );
    setCombineable( it.getCombineable() );
    setEquipable( it.getEquipable() );
    setUnequipable( it.getUnequipable() );
    setObtainEffAct( it.getObtainEffAct() );
    setDropEffAct( it.getDropEffAct() );
    setEquipEffAct( it.getEquipEffAct() );
    setUnequipEffAct( it.getUnequipEffAct() );
    setUseEffAct( it.getUseEffAct() );
    setObtainEff( *it.getObtainEff() );
    setDropEff( *it.getDropEff() );
    setEquipEff( *it.getEquipEff() );
    setUnequipEff( *it.getUnequipEff() );
    setUseEff( *it.getUseEff() );
    if( it.getAttached() )
      attach( *( it.getAttItem() ) );
    else if( getAttached() )
      detach();
  }
  return *this;
}

bool item::operator==( const item& it ) const
{
  if( getNumber() == it.getNumber() && 
      getName() == it.getName() &&
      getGfx() == it.getGfx() && 
      getType() == it.getType() &&
      getSize() == it.getSize() &&
      getWeight() == it.getWeight() &&
      getValue() == it.getValue() &&
      getValue() == it.getValue() &&
      getUses() == it.getUses() &&
      getMoveable() == it.getMoveable() &&
      getContainer() == it.getContainer() &&
      getLight() == it.getLight() &&
      getLocked() == it.getLocked() &&
      getOpenable() == it.getOpenable() &&
      getUseable() == it.getUseable() &&
      getObtainable() == it.getObtainable() &&
      getDroppable() == it.getDroppable() &&
      getCombineable() == it.getCombineable() &&
      getEquipable() == it.getEquipable() &&
      getUnequipable() == it.getUnequipable() &&
      getAttached() == it.getAttached() &&
      getObtainEffAct() == it.getObtainEffAct() &&
      getDropEffAct() == it.getDropEffAct() &&
      getEquipEffAct() == it.getEquipEffAct() &&
      getUnequipEffAct() == it.getUnequipEffAct() &&
      getUseEffAct() == it.getUseEffAct() &&
      getObtainEff() == it.getObtainEff() &&
      getDropEff() == it.getDropEff() &&
      getEquipEff() == it.getEquipEff() &&
      getUnequipEff() == it.getUnequipEff() &&
      getUseEff() == it.getUseEff() &&
      getAttItem() == it.getAttItem() )
    return true;
  else
    return false;
}

bool item::operator!=( const item& it ) const
{
  if( !( *this == it ) )
    return true;
  else
    return false;
}

bool item::operator+=( const item& it )
{
  if( getName() != it.getName() )
  {
    if( debug_mode )
      cout << "+=: Cannot add '" << it.getName() << "' to different item '"
	   << getName() << "'\n";
    return false;
  }
  else if( setNumber( getNumber() + it.getNumber() ) )
  {
    if( debug_mode )
      cout << "+=: Added " << it.getNumber() << " '" << getName()
	   << "', making the new quantity " << getNumber() << endl;
    return true;
  }
  else
  {
    if( debug_mode )
      cout << "+=: Could not add " << it.getNumber() << " '" << getName()
	   << "' items, because MAX_NUM is " << MAX_NUM << endl;
    return false;
  }
}

bool item::operator-=( const item& it )
{
  if( getName() != it.getName() )
  {
    if( debug_mode )
      cout << "-=: Cannot subtract '" << it.getName()
	   << "' from different item '" << getName() << "'\n";
    return false;
  }
  else if( setNumber( getNumber() - it.getNumber() ) )
  {
    if( debug_mode )
      cout << "-=: Subtracted " << it.getNumber() << " '" << getName()
	   << "', making the new quantity " << getNumber() << endl;
    return true;
  }
  else
    return false;
}

bool item::operator++()
{
  if( setNumber( getNumber() + 1 ) )
  {
    if( debug_mode )
      cout << "++: Quantity incremented to " << getNumber() << endl;
    return true;
  }
  else
    return false;
}

bool item::operator--()
{
  if( setNumber( getNumber() - 1 ) )
  {
    if( debug_mode )
      cout << "++: Quantity decremented to " << getNumber() << endl;
    return true;
  }
  else
    return false;
}

bool item::operator>( const item& it ) const
{
  if( getNumber() > it.getNumber() )
  {
    if( debug_mode )
      cout << ">: " << getNumber() << " is greater than " << it.getNumber()
	   << endl;
    return true;
  }
  else
    return false;
}

bool item::operator<( const item& it ) const
{
  if( getNumber() < it.getNumber() )
  {
    if( debug_mode )
      cout << "<: " << getNumber() << " is less than " << it.getNumber()
	   << endl;
    return true;
  }
  else
    return false;
}

bool item::operator>=( const item& it ) const
{
  if( !( *this < it ) )
    return true;
  else
    return false;
}

bool item::operator<=( const item& it ) const
{
  if( !( *this > it ) )
    return true;
  else
    return false;
}

// ===================================================================


// DEBUG FUNCTIONS
// ===================================================================

bool item::debug( bool val = true )
{
  debug_mode = val;
  return true;
}

string item::pb( const bool val ) const
{
  if( val == true )
    return "true";
  else
    return "false";
}

// ===================================================================


// GET FUNCTIONS (RETURNING A VALUE)
// ===================================================================

u_int16 item::getNumber() const // returns number of items
{
  if( debug_mode )
    cout << "getNumber(): There are " << num << " " << name << endl;
  return num;
}

u_int32 item::getId() const // returns ID number
{
  if( debug_mode )
    cout << "getId(): ID is '" << id << "'\n";
  return id;
}

string item::getName() const // returns name
{
  if( debug_mode )
    cout << "getName(): name is '" << name << "'\n";
  return name;
}

string item::getGfx() const // returns gfx filename
{
  if( debug_mode )
    cout << "getGfx(): gfx filename is '" << gfx << "'\n";
  return gfx;
}

u_int8 item::getType() const // returns type
{
  if( debug_mode )
    cout << "getType(): type is " << type << endl;
  return type;
}

u_int16 item::getSize() const // returns size
{
  if( debug_mode )
    cout << "getSize(): size is " << item_att[0] << endl;
  return item_att[0];
}

u_int16 item::getWeight() const // returns weight
{
  if( debug_mode )
    cout << "getWeight(): weight is " << item_att[1] << endl;
  return item_att[1];
}

u_int16 item::getValue() const // returns value
{
  if( debug_mode )
    cout << "getValue(): value is " << item_att[2] << endl;
  return item_att[2];
}

u_int16 item::getUses() const // returns number of uses
{
  if( debug_mode )
    cout << "getUses(): number of uses is " << item_att[3] << endl;
  return item_att[3];
}

bool item::getMoveable() const // returns moveable?
{
  if( debug_mode )
    cout << "getMoveable(): moveable? is " << pb( item_chr[0] ) << endl;
  return item_chr[0];
}

bool item::getContainer() const // returns container?
{
  if( debug_mode )
    cout << "getContainer(): container? is " << pb( item_chr[1] ) << endl;
  return item_chr[1];
}

bool item::getLight() const // returns light?
{
  if( debug_mode )
    cout << "getLight(): light? is " << pb( item_chr[2] ) << endl;
  return item_chr[2];
}

bool item::getLocked() const // returns locked?
{
  if( debug_mode )
    cout << "getLocked(): locked? is " << pb( item_chr[3] ) << endl;
  return item_chr[3];
}

bool item::getOpenable() const // returns openable?
{
  if( debug_mode )
    cout << "getOpenable(): openable? is " << pb( item_chr[4] ) << endl;
  return item_chr[4];
}

bool item::getUseable() const // returns useable?
{
  if( debug_mode )
    cout << "getUseable(): useable? is " << pb( item_chr[5] ) << endl;
  return item_chr[5];
}

bool item::getObtainable() const // returns obtainable?
{
  if( debug_mode )
    cout << "getObtainable(): obtainable? is " << pb( item_chr[6] ) << endl;
  return item_chr[6];
}

bool item::getDroppable() const // returns droppable?
{
  if( debug_mode )
    cout << "getDroppable(): droppable? is " << pb( item_chr[7] ) << endl;
  return item_chr[7];
}

bool item::getCombineable() const // returns combinable?
{
  if( debug_mode )
    cout << "getCombineable(): combineable? is " << pb( item_chr[8] ) << endl;
  return item_chr[8];
}

bool item::getEquipable() const // returns equipable?
{
  if( debug_mode )
    cout << "getEquipable(): equipable? is " << pb( item_chr[9] ) << endl;
  return item_chr[9];
}

bool item::getUnequipable() const // returns unequipable?
{
  if( debug_mode )
    cout << "getUnequipable(): unequipable? is " << pb( item_chr[10] ) << endl;
  return item_chr[10];
}

bool item::getAttached() const // returns combined?
{
  if( debug_mode )
    cout << "getAttached(): attached? is " << pb( item_chr[11] ) << endl;
  return item_chr[11];
}

bool item::getObtainEffAct() const // returns obtain effect active?
{
  if( debug_mode )
  {
    cout << "getObtainEffAct(): obtain effect active? is "
	 << pb( item_ace[0] ) << endl;
  }
  return item_ace[0];
}

bool item::getDropEffAct() const // returns drop effect active?
{
  if( debug_mode )
  {
    cout << "getDropEffAct(): drop effect active? is "
	 << pb( item_ace[1] ) << endl;
  }
  return item_ace[1];
}

bool item::getEquipEffAct() const // returns equip effect active?
{
  if( debug_mode )
  {
    cout << "getEquipEffAct(): equip effect active? is "
	 << pb( item_ace[2] ) << endl;
  }
  return item_ace[2];
}

bool item::getUnequipEffAct() const // returns unequip effect active?
{
  if( debug_mode )
  {
    cout << "getUnequipEffAct(): unequip effect active? is "
	 << pb( item_ace[3] ) << endl;
  }
  return item_ace[3];
}

bool item::getUseEffAct() const // returns use effect active
{
  if( debug_mode )
  {
    cout << "getUseEffAct(): use effect active? is "
	 << pb( item_ace[4] ) << endl;
  }
  return item_ace[4];
}

effect* item::getObtainEff() const // returns obtain effect
{
  return item_eff[0];
}

effect* item::getDropEff() const // returns drop effect
{
  return item_eff[1];
}

effect* item::getEquipEff() const // returns equip effect
{
  return item_eff[2];
}

effect* item::getUnequipEff() const // returns unequip effect
{
  return item_eff[3];
}

effect* item::getUseEff() const // returns use effect
{
  return item_eff[4];
}

item* item::getAttItem() const // returns a pointer to the attached item
{
  return attached;
}

u_int32 item::getPos() const // returns the index in the display list
{
  return pos;
}

// ===================================================================


// SET FUNCTIONS (MODIFYING)
// ===================================================================

bool item::setNumber( const u_int16 n ) // sets number of items
{
  if( n <= MAX_NUM )
  {  
    if( debug_mode )
      cout << "setNumber(): There are now " << n << " " << getName() << endl;
    num = n;
    return true;
  }
  else
  {
    if( debug_mode )
      cout << "setNumber(): Asked to set number to " << n << ", "
	   << "but MAX_NUM is " << MAX_NUM << ", so did nothing\n";
    return false;
  }
}

bool item::setId( const u_int32 val ) // sets ID number
{
  id = val;
  if( debug_mode )
    cout << "setId(): ID set to '" << id << "'\n";
  return true;
}

bool item::setName( const string val ) // sets name
{
  name = val;
  if( debug_mode )
    cout << "setName(): name set to '" << name << "'\n";
  return true;
}

bool item::setGfx( const string val ) // sets gfx filename
{
  gfx = val;
  if( debug_mode )
    cout << "setGfx(): gfx filename set to '" << gfx << "'\n";
  return true;
}

bool item::setType( const u_int8 val ) // sets type
{
  type = val;
  if( debug_mode )
    cout << "setType(): type set to " << type << endl;
  return true;
}

bool item::setSize( const u_int16 val ) // sets size
{
  item_att[0] = val;
  if( debug_mode )
    cout << "setSize(): size set to " << item_att[0] << endl;
  return true;
}

bool item::setWeight( const u_int16 val ) // sets weight
{
  item_att[1] = val;
  if( debug_mode )
    cout << "setWeight(): weight set to " << item_att[1] << endl;
  return true;
}

bool item::setValue( const u_int16 val ) // sets value
{
  item_att[2] = val;
  if( debug_mode )
    cout << "setValue(): value set to " << item_att[2] << endl;
  return true;
}

bool item::setUses( const u_int16 val ) // sets number of uses
{
  item_att[3] = val;
  if( debug_mode )
    cout << "setUses(): number of uses set to " << item_att[3] << endl;
  return true;
}

bool item::setMoveable( const bool val ) // sets moveable?
{
  item_chr[0] = val;
  if( debug_mode )
    cout << "setMoveable(): moveable? set to " << pb( item_chr[0] ) << endl;
  return true;
}

bool item::setContainer( const bool val ) // sets container?
{
  item_chr[1] = val;
  if( debug_mode )
    cout << "setContainer(): container? set to " << pb( item_chr[1] ) << endl;
  return true;
}

bool item::setLight( const bool val ) // sets light?
{
  item_chr[2] = val;
  if( debug_mode )
    cout << "setLight(): light? set to " << pb( item_chr[2] ) << endl;
  return true;
}

bool item::setLocked( const bool val ) // sets locked?
{
  item_chr[3] = val;
  if( debug_mode )
    cout << "setLocked(): locked? set to " << pb( item_chr[3] ) << endl;
  return true;
}

bool item::setOpenable( const bool val ) // sets openable?
{
  item_chr[4] = val;
  if( debug_mode )
    cout << "setOpenable(): openable? set to " << pb( item_chr[4] ) << endl;
  return true;
}

bool item::setUseable( const bool val ) // sets useable?
{
  item_chr[5] = val;
  if( debug_mode )
    cout << "setUseable(): useable? set to " << pb( item_chr[5] ) << endl;
  return true;
}

bool item::setObtainable( const bool val ) // sets obtainable?
{
  item_chr[6] = val;
  if( debug_mode )
    cout << "setObtainable(): obtainable? set to " << pb( item_chr[6] ) << endl;
  return true;
}

bool item::setDroppable( const bool val ) // sets droppable?
{
  item_chr[7] = val;
  if( debug_mode )
    cout << "setDroppable(): droppable? set to " << pb( item_chr[7] ) << endl;
  return true;
}

bool item::setCombineable( const bool val ) // sets combinable?
{
  item_chr[8] = val;
  if( debug_mode )
    cout << "setCombineable(): combineable? set to " << pb( item_chr[8] ) << endl;
  return true;
}

bool item::setEquipable( const bool val ) // sets equipable?
{
  item_chr[9] = val;
  if( debug_mode )
    cout << "setEquipable(): equipable? set to " << pb( item_chr[9] ) << endl;
  return true;
}

bool item::setUnequipable( const bool val ) // sets unequipable?
{
  item_chr[10] = val;
  if( debug_mode )
    cout << "setUnequipable(): unequipable? set to " << pb( item_chr[10] ) << endl;
  return true;
}

bool item::setObtainEffAct( const bool val ) // sets obtain effect active?
{
  item_ace[0] = val;
  if( debug_mode )
  {
    cout << "setObtainEffAct(): obtain effect active? set to "
	 << pb( item_ace[0] ) << endl;
  }
  return true;
}

bool item::setDropEffAct( const bool val ) // sets drop effect active?
{
  item_ace[1] = val;
  if( debug_mode )
  {
    cout << "setDropEffAct(): drop effect active? set to "
	 << pb( item_ace[1] ) << endl;
  }
  return true;
}

bool item::setEquipEffAct( const bool val ) // sets equip effect active?
{
  item_ace[2] = val;
  if( debug_mode )
  {
    cout << "setEquipEffAct(): equip effect active? set to "
	 << pb( item_ace[2] ) << endl;
  }
  return true;
}

bool item::setUnequipEffAct( const bool val ) // sets unequip effect active?
{
  item_ace[3] = val;
  if( debug_mode )
  {
    cout << "setUnequipEffAct(): unequip effect active? set to "
	 << pb( item_ace[3] ) << endl;
  }
  return true;
}

bool item::setUseEffAct( const bool val ) // sets use effect active
{
  item_ace[4] = val;
  if( debug_mode )
  {
    cout << "setUseEffAct(): use effect active? set to "
	 << pb( item_ace[4] ) << endl;
  }
  return true;
}

bool item::setObtainEff( effect& val ) // sets obtain effect
{
  item_eff[0] = &val;
  return true;
}

bool item::setDropEff( effect& val ) // sets drop effect
{
  item_eff[1] = &val;
  return true;
}

bool item::setEquipEff( effect& val ) // sets equip effect
{
  item_eff[2] = &val;
  return true;
}

bool item::setUnequipEff( effect& val ) // sets unequip effect
{
  item_eff[3] = &val;
  return true;
}

bool item::setUseEff( effect& val ) // sets use effect
{
  item_eff[4] = &val;
  return true;
}

bool item::setPos( const u_int32 n ) // sets index in the display list
{
  pos = n;
  return true;
}

// ===================================================================


// ADVANCED FUNCTIONS AND WRAPPERS
// ===================================================================

bool item::attach( item& it )
{
  if( getAttached() == false && getCombineable() == true &&
      ( it.getType() != getType() ||
	( ( it.getType() == 0 || it.getType() == 3 ) &&
	  ( getType() == 0 || getType() == 3 ) ) ) )
  {
    setAttached( true );
    attached = &it;
    return true;
  }
  else
    return false;
}

bool item::detach()
{
  if( getAttached() )
  {
    attached = NULL; /* since the user has passed a reference to an already
			existing item, this will not cause a leak, as that
			item will be destroyed outside of the class */
    setAttached( false );
    return true;
  }
  else
    return false;
}

// ===================================================================


// PROTECTED FUNCTIONS
// ===================================================================

bool item::setAttached( const bool val )
{
  item_chr[11] = val;
  if( debug_mode )
    cout << "setAttached(): attached? set to " << pb( item_chr[11] ) << endl;
  return true;
}

// ===================================================================
