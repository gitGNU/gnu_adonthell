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
 * @file   slot.h
 * @author Kai Sterker <kaisterker@linuxgames.com> 
 * 
 * @brief  One unit of the inventory.
 */

#ifndef SLOT_H
#define SLOT_H

#include "fileops.h"

class item_base;
class inventory;

/**
 * The %slot class may contain one item or a stack of (roughly) equal
 * items. It is used by inventories, which are basically a list of
 * slots.
 */
class slot
{
public:
    /**
     * Create a new %slot with an optional id. The id can be used to
     * retrieve certain slots from the %inventory.
     * @param owner The %inventory the %slot belongs to
     * @param id The slot's id
     */
     slot (inventory *owner, const string & id = "");

    /**
     * Delete a slot.
     */
    ~slot ();
    
    /**
     * @name Item handling
     */
    //@{
    /**
     * Retrieve pointer to the item kept in this %slot. In case of an
     * immutable item, it will also adjust item_base::Slot.
     * @return item in this %slot, or \c NULL in case it is empty.
     */
    item_base *get_item ();

    /**
     * Checks whether the given item is allowed to go into this %slot.
     * This is the case when the %slot is empty, or if the items
     * already in the %slot (roughly) equal the given item.
     * @param item Item to test against.
     * @return \b true if the item may go into the %slot, \b falso otherwise
     */
    bool accepts (item_base *item);
    
    /**
     * Add an item or stack of items to this %slot. If the %slot is not
     * empty, the given item must be of roughly the same kind (i.e. have 
     * the same name) as the item kept in the %slot and it must be stackable. 
     * If more items are given than fit, the remaining number will be
     * returned.
     *
     * If items that are added come from another %inventory, they will be
     * removed from there automatically, as long as they fit into this %slot.
     *
     * @param item the item or stack of items to be added.
     * @param count number of items in the stack.
     * @return number of items that didn't fit into the %slot.
     */
    u_int32 add (item_base * item, const u_int32 & count = 1);
    
    /**
     * Removes the topmost item(s) from the %slot, if possible. Up to count
     * items are removed, unless there aren't as many items in the slot.
     * Note that mutable items are only removed, but not deleted.
     *
     * @param item the item to be removed from the slot.
     * @param count number of items to be removed.
     * @return number of items that could not be removed.
     */
    u_int32 remove (item_base * item, const u_int32 & count = 1);
    
    /**
     * Remove all items from the slot, no matter whether they are mutable
     * or not.
     */
    void clear ();
    
    /**
     * Retrieve the number of items in this %slot. 
     * @return number of items this %slot contains.
     */
    u_int32 count () const
    {
        return Count;
    }
    //@}

    /**
     * Attribute access
     */
    //@{
    /**
     * Retrieve the slot's id.
     * @return id of the %slot.
     */
    const string & id () const
    {
        return Id;
    }
    
    /**
     * Give this %slot a new Id.
     * @param id A new identifier for this %slot. 
     */
    void set_id (const string & id)
    {
        Id = id;
    }
    //@}
    
    /**
     * @name Loading/Saving
     */
    //@{
    /**
     * Load %slot and its contents from stream. 
     * @param file stream to load %slot from.
     * @return \b true if loading successful, \b false otherwise.
     */
    bool get_state (igzstream & file);
    
    /**
     * Save %slot and its contents to stream.
     * @param file stream to save %slot to.
     * @return \b true if saving successful, \b false otherwise.
     */
    bool put_state (ogzstream & file) const;
    //@}
    
private:
#ifndef SWIG
    /**
     * Identifier for this %slot, to retrieve it from its %inventory
     * if neccessary.
     */    
    string Id;

    /**
     * The %inventory this %slot belongs to.
     */
    inventory *Owner;
    
    /**
     * The item(s) currently kept in this %slot.
     */
    item_base *Item;
    
    /**
     * The number of items in this %slot.
     */
    u_int32 Count;
#endif // SWIG
};

#endif // SLOT_H
