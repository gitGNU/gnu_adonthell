/*
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.


  This is the main class for the inventory subsystem.
  It uses a hash table based on the object's name to 
  allocate storage in the inventory. It supports inventory
  growth through prime sequences. A 101 element array can
  easily store hundreds more using linked lists inside the
  element. I felt this was optimal since a person isn't 
  carry that many objects, but allows for growth if they
  do. Searching is fast. A simple matter of grabbing the key
  and doing a linear search down a short list.
*/

#include "inventory.h"


// CONSTRUCTORS
// ========================================================================

inventory::inventory()
{
  debug( false );
  setNumItemO( 0 );
  setMaxItemO( HASH_SIZE * 5 );
  setNumItems( 0 );
  setMaxItems( 200 );
  total_weight = 0;
  for( u_int16 i = 0; i < HASH_SIZE; i++ )
    inv[i] = NULL; 
  // Build the placeholder item
  pl = new item( 0, "", "", 0, 0 );
  pl->setObtainable( false );
  pl->setDroppable( false );
  pl->setCombineable( false );
}

inventory::inventory( const string fn )
{
}

/*
inventory::~inventory()
{
  delete pl;
}
*/

// ========================================================================


// OPERATORS
// ========================================================================

inventory* inventory::operator=( const inventory& it ) // assignment operator
{
  if( *this != it )
  {
  }
  return this;
}

bool inventory::operator==( const inventory& it ) const // equality operator
{
  return true;
}

bool inventory::operator!=( const inventory& it ) const // inequality operator
{
  if( !( *this == it ) )
    return true;
  else
    return false;
}

/* These operators add and subract the contents of an inventory.
   
   += works like this: if you have an inventory, A, that contains 27 items,
   and one, B, that contains 10 items, A += B will add the contents of B to
   A, thus making the number of items contained in A 37.
   
   -= is a little different: Given the same A and B from the last example,
   A -= B will subtract from A all the items that exist in a intersection of
   A and B, that is, everything that exists in *both* A and B. So, supposing
   that the intersection of A and B contains 7 items, A -= B will result in
   A having 20 items. */

bool inventory::operator+=( const inventory& it ) // plus assignment operator
{
  return true;
}

bool inventory::operator-=( const inventory& it ) // minus assignment operator
{
  return true;
}

/* These operators are all comparisons on the number of items contained in
   the inventory */

bool inventory::operator>( const inventory& it ) const // greater than?
{
  return true;
}

bool inventory::operator<( const inventory& it ) const // less than?
{
  return true;
}

bool inventory::operator>=( const inventory& it ) const
{
  return true;
}

bool inventory::operator<=( const inventory& it ) const
{
  return true;
}

// ========================================================================


// DEBUG FUNCTIONS
// ========================================================================

bool inventory::debug( const bool val="true" ) // turns cout debugging on/off
{
  debug_mode = val;
  return true;
}

void inventory::showHash() const
{
  cout << endl << "Index\tNumber\tName" << endl
       << "====================================" << endl;
  for( u_int32 i = 0; i < HASH_SIZE; i++ )
    if( inv[i] != NULL )
    {
      item* current = inv[i];
      u_int16 d = 0;
      
      while( current->next != NULL )
      {
	cout << i << "." << d << "\t" << current->getNumber() << "\t"
	     << current->getName() << endl;
	current = current->next;
	d++;
      }
      cout << i << "." << d << "\t" << current->getNumber() << "\t"
	   << current->getName() << endl;
    }
  cout << "====================================" << endl;
}

void inventory::showDisp() const
{
  cout << endl << "Index\tPos\tNumber\tName" << endl
       << "====================================" << endl;
  for( u_int32 i = 0; i < disp.size(); i++ )
  {
    if( disp[i]->getNumber() == 0 )
      cout << i << "\t[ Placeholder ]\n";
    else
      cout << i << "\t" << disp[i]->getPos() << "\t"
	   << disp[i]->getNumber() << "\t"
	   << disp[i]->getName() << endl;
  }
  cout << "====================================" << endl;
}

// ========================================================================


// GET FUNCTIONS (VALUE RETURNING)
// ========================================================================

u_int32 inventory::getNumItemO() const
{
  return num_item_o;
}

u_int32 inventory::getMaxItemO() const
{
  return max_item_o;
}

u_int32 inventory::getNumItems() const
{
  return num_items;
}

u_int32 inventory::getMaxItems() const
{
  return max_items;
}

u_int32 inventory::getDispItems() const
{
  return disp.size();
}

u_int32 inventory::getWeight() const
{
  return total_weight;
}

item* inventory::getItem( const u_int32 i )
{
  // Make sure that 'i' is a legal index or fail
  if( i >= disp.size() )
  {
    if( debug_mode )
      cout << "getItem(): There are only " << disp.size() 
	   << " items in the display vector, so " << i 
	   << " is an illegal index\n";
    return NULL;
  }

  // Make sure that 'i' is not a placeholder or fail
  if( disp[i]->getNumber() == 0 )
  {
    if( debug_mode )
      cout << "getItem(): Sorry, I can't return the placeholder at index "
	   << i << endl;
    return NULL;
  }

  // We cool, so return the item already!
  if( debug_mode )
    cout << "getItem(): Returning '" << disp[i]->getName() << "' at index "
	 << i << endl;
  return disp[i];
}

item* inventory::getItem( const string nm )
{
  u_int32 i = 0; // the index of the item in the hash table
  u_int32 d = 0; // the depth of the item at index 'i' in the hash table
  u_int32 di = 0; // the index of 'nm' in the display vector

  if( getHash( nm, i, d, di ) )
  {
    if( debug_mode )
      cout << "getItem(): Returning '" << disp[di]->getName() << "' at index "
	   << i << "." << d << " in the hash table, index " << di
	   << " in the display vector\n";
    return disp[di];
  }
  else
  {
    if( debug_mode )
      cout << "getItem(): Did not find '" << nm << "'\n";
    return NULL;
  }
}

// ========================================================================


// PROTECTED SET FUNCTIONS (SET MEMBERS)
// ========================================================================

bool inventory::setMaxItemO( const u_int32 mi )
{
  max_item_o = mi;
  return true;
}

bool inventory::setMaxItems( const u_int32 mi )
{
  max_items = mi;
  return true;
}

// ========================================================================


// INSERTION AND DELETION FUNCTIONS
// ========================================================================

bool inventory::insert( item* it )
{
  // Make sure that another item object is allowed or fail
  if( getNumItemO() == getMaxItemO() )
  {
    if( debug_mode )
      cout << "insert(): Inventory already contains " << getMaxItemO()
	   << " item objects, the maximum allowed\n";
    return false;
  }

  /* Make sure that the addition would not put the number of actual items over
     the allowed maximum or fail */
  if( (getNumItems() + it->getNumber()) > getMaxItems() )
  {
    if( debug_mode )
      cout << "insert(): Adding " << it->getNumber()
	   << " more items would put the total over " << getMaxItems()
	   << " items, the maximum allowed\n";
    return false;
  }

  // Make the insertion
  u_int32 index = 0;

  index = hashcode( it->getName() );
  
  bool new_item = false;
  u_int32 d = 1; // depth in the index
  u_int16 num = it->getNumber();
  item *current = inv[index];
  
  if( current == NULL )
  {
    inv[index] = it;
    current = it;
    new_item = true;
  }
  else
    while( current->next != NULL && current->getName() != it->getName() )
    {
      current = current->next;
      d++;
    }

  /* If there is already some of 'it' in the inventory, simply increase the
     number of items at 'current' */
  if( current->getName() == it->getName() && !( new_item ) )
  {
    if( !( *current += *it ) )
    {
      cout << "insert(): '" << current->getName() << "' += '"
	   << it->getName()
	   << "' failed; if you don't know why, turn on debugging for '"
	   << current->getName() << "'\n";
      return false;
    }
    d--;
  }
  else
  {
    if( !( new_item ) )
      current->next = it;

    /* If the display vector has filled up, put this item in the display
       vector in the place of the last placeholder */
    if( getDispItems() == getMaxItemO() )
    {
      disp.insert( disp.begin() + pl_pos[pl_pos.size() - 1], it );
      disp.erase( disp.begin() + pl_pos[pl_pos.size() - 1] + 1 );
      pl_pos.pop_back();
    }
    else
    {
      disp.push_back( it );
      it->setPos( disp.size() - 1 );
    }
    setNumItemO( getNumItemO() + 1 );
  }

  if( inv[index]->getName() == it->getName() )
    d = 0;

  setNumItems( getNumItems() + it->getNumber() );
  setWeight( getWeight() + (it->getWeight() * it->getNumber()) );
  
  if( debug_mode )
    cout << "insert(): Added " << num << " '" << it->getName()
	 << "' in " << index << "." << d << endl;
  
  return true;
}

bool inventory::remove( const u_int32 i, const u_int16 n )
{
  u_int32 hi = 0; // index of the item in the hash table
  u_int32 hd = 0; // depth of the item at index i in the hash table
  u_int32 di = 0; // the index of the item in the display vector
  string nm; // the name of the item

  // Make sure that 'i' is a valid index or fail
  if( i >= disp.size() )
  {
    if( debug_mode )
      cout << "remove(): index " << i << " is not a legal index\n";
    return false;
  }

  // Make sure that the item at index 'i' is not a placeholder or fail
  if( disp[i]->getNumber() == 0 )
  {
    if( debug_mode )
      cout << "remove(): There is a placeholder at index " << i << endl;
    return false;
  }

  // Set 'nm' to the name of the item at index 'i'
  nm = disp[i]->getName();

  // Look for 'nm' in the hash or fail
  if( !( getHash( nm, hi, hd, di ) ) )
    return false;

  // Remove 'nm' from the hash or fail
  if( !( removeHash( nm, n, hi, hd ) ) )
    return false;

  // Remove 'nm' from the display or fail
  if( !( removeDisp( nm, n, di ) ) )
    return false;

  return true;
}

bool inventory::remove( const string nm, const u_int16 n )
{
  u_int32 i = 0; // index of the item in the hash table
  u_int32 d = 0; // depth of the item at index i in the hash table
  u_int32 di = 0; // the index of the item in the display vector

  // Look for 'nm' in the hash or fail
  if( !( getHash( nm, i, d, di ) ) )
    return false;

  // Remove 'nm' from the hash or fail
  if( !( removeHash( nm, n, i, d ) ) )
    return false;

  // Remove 'nm' from the display or fail
  if( !( removeDisp( nm, n, di ) ) )
    return false;

  return true;
}

// ========================================================================


// DISPLAY FUNCTIONS
// ========================================================================

bool inventory::swap( const u_int32 i1, const u_int32 i2 )
{
  // Make sure that both 'i1' and 'i2' are legal indexes or fail
  if( i1 >= disp.size() || i2 >= disp.size() )
  {
    if( debug_mode )
      cout << "swap(): Since there are only " << disp.size()
	   << " items in the display vector, either " << i1 << " or " << i2
	   << " is not a legal index\n";
    return false;
  }

  if( debug_mode )
    cout << "swap(): Swapped '" << disp[i1]->getName() << "' (index " << i1
	 << ") with '" << disp[i2]->getName() << "' (index " << i2 << ")\n";

  // Swap the positions of the items (not in the vector)
  disp[i1]->setPos( i2 );
  disp[i2]->setPos( i1 );

  /* If one of the items is a placeholder, fix the pl_pos stack. If they both
     are placeholders, nothing needs to be done. Think about it... ;) */
  bool pl_exists = false;
  u_int32 pl_posi; // where the placeholder is in the position stack
  u_int32 pl_old_posi; // the placeholder's old position
  u_int32 pl_new_posi; // the placeholder's target position

  if( disp[i1]->getNumber() == 0 && disp[i2]->getNumber() != 0 )
  {
    pl_exists = true;
    pl_old_posi = i1;
    pl_new_posi = i2;
  }
  else if( disp[i1]->getNumber() != 0 && disp[i2]->getNumber() == 0 )
  {
    pl_exists = true;
    pl_old_posi = i2;
    pl_new_posi = i1;
  }

  /* If there was a placeholder, update the placeholder position stack. Since
     this stack also happens to be an ordered list, we can use a binary search
     on it to find the damn thing in a hurry. Time complexity? O( Log N ) */
  if( pl_exists )
  {
    // Find the placeholder in question or fail
    if( !( binarySearch( pl_old_posi, 0, pl_pos.size() - 1, pl_posi ) ) )
    {
      if( debug_mode )
	cout << "swap(): Could not find the placeholder at index " 
	     << pl_old_posi << endl;
      return false;
    }

    // Make sure that the position is right or fail
    if( pl_old_posi != pl_pos[pl_posi] )
    {
      if( debug_mode )
	cout << "swap(): The position in the stack at " << pl_posi
	     << " should be " << pl_old_posi << ", but it is "
	     << pl_pos[pl_posi] << endl;
      return false;
    }

    // Remove the old position from the stack
    pl_pos.erase( pl_pos.begin() + pl_posi );

    /* Put the new position into the placeholder position stack. This will
       require a modified search algorithm, inasmuch as we now need a tight
       border on the new position. */
    if( !( binaryPairSearch( pl_new_posi, 0, pl_pos.size() - 1, pl_posi ) ) )
    {
      if( debug_mode )
	cout << "swap(): Could not find a place to insert the placeholder"
	     << " index " << pl_new_posi << endl;
      return false;
    }

    /* If the resulting position is an illegal index, push the new thing onto
       the top of the stack */
    if( pl_posi >= pl_pos.size() )
      pl_pos.push_back( pl_new_posi );
    else
      pl_pos.insert( pl_pos.begin() + pl_posi, pl_new_posi );
    
    if( debug_mode )
    {
      cout << "swap(): There was a placeholder at index " << pl_old_posi
	   << ", so the placeholder position stack was updated:\n\t";
      for( u_int32 i = 0; i < pl_pos.size(); i++ )
	cout << pl_pos[i] << " ";
      cout << endl;
    }
  }

  // Make the swap happen!
  disp.insert( disp.begin() + i1, disp[i2] );
  disp.insert( disp.begin() + i2 + 1, disp[i1 + 1] );
  disp.erase( disp.begin() + i1 + 1 );
  disp.erase( disp.begin() + i2 + 1 );

  return true;
}
						      
// ========================================================================


// PRIVATE FUNCTIONS
// ========================================================================

bool inventory::setNumItemO( const u_int32 ni )
{
  num_item_o = ni;
  return true;
}

bool inventory::setNumItems( const u_int32 ni )
{
  num_items = ni;
  return true;
}

bool inventory::setWeight( const u_int32 tw )
{
  total_weight = tw;
  return true;
}

u_int32 inventory::hashcode( const string name )
{
  u_int32 index = 0;
  u_int32 code = 0;
  u_int32 length = name.length(), max = name.length();
  while( index < length )
  {
    code += ((u_int32)name[index] * ((u_int32)pow (base, max)));
    index++;
    max--;
  }
  u_int32 var = abs( code % HASH_SIZE );
  
  return var;
}

bool inventory::getHash( const string nm, u_int32& i, u_int32& d, u_int32& di )
{
  i = 0;
  d = 0;
  di = 0;

  i = hashcode( nm );
  item* current = inv[i];

  while( current->next != NULL && current->getName() != nm )
  {
    current = current->next;
    d++;
  }
  if( current->getName() == nm )
  {
    di = current->getPos();
    if( debug_mode )
      cout << "getHash(): Found '" << nm << "' at " << i << "." << d <<endl;
    return true;
  }
  else
  {
    if( debug_mode )
      cout << "getHash(): Did not find '" << nm << "'\n";
    return false;
  }
}

bool inventory::removeDisp( const string nm, const u_int16 n, const u_int32 i )
{
  // Make sure that the index is legal or fail
  if( i >= disp.size() )
  {
    if( debug_mode )
      cout << "removeDisp(): There are only " << disp.size()
	   << " items in the display list, so index " << i
	   << " is invalid\n";
    return false;
  }

  // Make sure that the item at 'i' is the one that should be deleted or fail
  if( nm != disp[i]->getName() )
  {
    if( debug_mode )
      cout << "removeDisp(): The item at index " << i << " is '"
	   << disp[i]->getName() << "'; it should be '" << nm << "'\n";
    return false;
  }

  // If the number of items at index 'i' is 0, put a placeholder item at 'i'
  if( disp[i]->getNumber() == 0 || n == 0 )
  {
    // Get rid of the item at 'i' and insert the placeholder
    disp.erase( disp.begin() + i );
    disp.insert( disp.begin() + i, getPlaceholder() );

    // Push the placeholder onto the stack of placeholder positions
    pl_pos.push_back( i );

    // Display a debug message?
    if( debug_mode )
      cout << "removeDisp(): Put a placeholder at index " << i << endl;

    /* If the display vector contains nothing but placeholders, get rid of
       all of them */
    if( getNumItemO() == 0 )
    {
      disp.clear();
      pl_pos.clear();
      if( debug_mode )
	cout << "removeDisp(): The display vector contained nothing but "
	     << "placeholders, so it was cleared\n";
    }
  }
  else
    if( debug_mode )
      cout << "removeDisp(): There are still " << disp[i]->getNumber()
	   << " '" << disp[i]->getName() << "' at index " << i
	   << ", so did nothing\n";

  return true;
}

bool inventory::removeHash( const string nm, u_int16 n,
			    const u_int32 i, const u_int32 d )
{
  item* current = inv[i];
  item* last = inv[i];
  u_int32 depth = 0;
  
  while( current->next != NULL && depth < d )
  {
    last = current;
    current = current->next;
    depth++;
  }
  if( current->getName() == nm )
  {
    if( n == 0 )
      n = current->getNumber();
    
    if( !( current->setNumber( current->getNumber() - n ) ) )
    {
      if( debug_mode )
	cout << "removeHash(): item::setNumber( " << current->getNumber()
	     << " - " << n
	     << " ) failed; if you don't know why, turn on debugging for '"
	     << current->getName() << "'\n";
      return false;
    }

    if( current->getNumber() == 0 )
    {
      if( debug_mode )
	cout << "removeHash(): Removed '" << nm << "' from "
	     << i << "." << d << endl;
      setNumItemO( getNumItemO() - 1 );
      setNumItems( getNumItems() - n );
      setWeight( getWeight() - (current->getWeight() * n) );
      last->next = current->next;
      current = NULL;
    }
    else
    {
      if( debug_mode )
	cout << "removeHash(): Decreased quantity of " << nm << " by " << n
	     << " to " << current->getNumber() << endl;
      setNumItems( getNumItems() - n );
      setWeight( getWeight() - (current->getWeight() * n) );
    }
    return true;
  }
  else
  {
    if( debug_mode )
      cout << "removeHash(): Did not find " << nm << endl;
    return false;
  }
}

item* inventory::getPlaceholder() const
{
  return pl;
}

bool inventory::binarySearch( const u_int32 target, u_int32 first,
			      u_int32 last, u_int32& posi )
{
  u_int32 middle = (first + last) / 2;

  if( first > last )
    return false;
  
  if( pl_pos[middle] > target )
    binarySearch( target, first, middle - 1, posi );
  else if( pl_pos[middle] < target )
    binarySearch( target, middle + 1, last, posi );
  else
  {
    posi = middle;
    return true;
  }

  return true;
}

bool inventory::binaryPairSearch( const u_int32 target, u_int32 first,
				  u_int32 last, u_int32& posi )
{
  u_int32 low_middle = (first + last) / 2;
  u_int32 high_middle = low_middle + 1;

  if( first > last )
    return false;
  else if( first == last && first < target )
  {
    posi = first + 1;
    return true;
  }
  else if( first == last && first > target )
  {
    posi = first;
    return true;
  }

  if( pl_pos[low_middle] < target && pl_pos[high_middle] > target )
  {
    posi = high_middle;
    return true;
  }
  else if( pl_pos[low_middle] > target )
    binarySearch( target, first, low_middle, posi );
  else if( pl_pos[high_middle] < target )
    binarySearch( target, high_middle, last, posi );

  return true;
}

// ========================================================================
