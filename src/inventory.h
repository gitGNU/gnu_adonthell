/*
   $Id$
   
   Copyright (C) 2003 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/**
 * @file   inventory.h
 * @author Kai Sterker <kaisterker@linuxgames.com> 
 * 
 * @brief  Container for items.
 */

#ifndef INVENTORY_H
#define INVENTORY_H

#include <vector>
#include "slot.h"

/**
 * Whenever items need to be stored by an object -- be it %character,
 * chest, shop or %map -- an %inventory should be used. Inventories are
 * more than a list of items. They allow combining and using of items
 * (if the items involved provide this functionality), take care of
 * passing items between inventories and provide means to query for
 * items with certain properties.
 */
class inventory
{
public:
    /**
     * Create an %inventory with 'count' anonymous slots.
     * @param limited Whether %inventory has fixed size or not.
     * @param size Initial size of the %inventory.
     */
    inventory (const bool & limited = true, const u_int16 & size = 0);
        
    /**
     * Destroy inventory and its contents.
     */
    ~inventory ();
    
    /**
     * @name Slot Handling
     */
    //@{
    /**
     * Adds 'count' anonymous slots to the %inventory, thus increasing its
     * size.
     * @param count Number of slots to add.
     */
    void grow (const u_int16 & count);
    
    /**
     * Add a %slot with the given id. The id should be unique, otherwise
     * you might have troubles in retrieving the %slot with the given id.
     * The size of the %inventory will grow by one.
     * @param id Id of the %slot to add.
     */
    void add_slot (const string & id);
    
    /**
     * Retrieve the %slot with given id from the %inventory.
     * @param id Id of the %slot to retrieve.
     * @return %slot with the given id, or \c NULL if no matching %slot exists.
     */
    slot *get_slot (const string & id);
    //@}
        
    /**
     * @name Inventory iterators
     */
    //@{
    /**
     * Retrieves the first %slot of the %inventory. All subsequent slots
     * can be retrieved via inventory::next ().
     * @return inventories first %slot, or \c NULL if there are no slots.
     */
    slot *first ();
    
    /**
     * After a call to inventory::first (), subsequent calls to this
     * method will return all slots of this %inventory, one by one.
     * Mainly useful for a GUI to display the inventories contents.
     * @return the next slot, or \c NULL if there are no more slots.
     */
    slot *next ();
    //@}
    
    /**
     * @name Item handling
     */
    //@{
    /**
     * Add item(s) to the %inventory. This method should be used when
     * the given items should not be added to a specific %slot, but to
     * this %inventory in general. It then tries to add them in the best
     * possible way: if the item(s) are stackable, they will be added to
     * an already existing stack. Otherwise they will go into the first 
     * empty %slot. If not all items fitted, the number of the remaining
     * items will be returned.
     *
     * Not that there is no explicit remove method, as item removal will
     * happen automatically.
     *
     * @param item The item(s) to add.
     * @param count The number of items in the stack.
     * @return the number of items that did not fit.
     */
    u_int32 add (item_base *item, const u_int32 & count = 1);
    //@}
    
    /**
     * @name Loading/Saving
     */
    //@{
    /**
     * Load %inventory (and its contents) from stream. 
     * @param file stream to load %invnetory from.
     * @return \b true if loading successful, \b false otherwise.
     */
    bool get_state (igzstream & file);
    
    /**
     * Save %inventory (and contents) to a stream.
     * @param file stream to save %inventory to.
     * @return \b true if saving successful, \b false otherwise.
     */
    bool put_state (ogzstream & file);
    //@}

private:
#ifndef SWIG
    /**
     * The slots this inventory provides.
     */
    std::vector<slot*> Slots;

    /**
     * Inventory iterator.
     */
    std::vector<slot*>::iterator I;
    
    /**
     * Whether this inventory provides unlimited space or not. Inventories
     * with unlimited space will automatically grow as items are added.
     */
    bool Limited;
#endif // SWIG
};

#endif // INVENTORY_H
