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
 * @file   manager.cc
 * @author Kai Sterker <kaisterker@linuxgames.com> 
 * 
 * @brief  Takes care of character/item issues.
 */

#include <algorithm>
#include "manager.h"
#include "inventory.h"

/**
 * Mapping from item categories to equipment slots.
 */
std::hash_map<std::string, std::vector<std::string>,
    hash<std::string> > manager::SlotCategoryMap;

// retrieve a list of slots the item would fit into    
const std::vector<std::string> manager::fits (item_base *item)
{
    if (item == NULL) return std::vector<std::string> ();
    
    std::vector<std::string> slots;
    std::vector<std::string> categories = item->categories ();
    std::vector<std::string>::iterator i;
    std::hash_map<std::string, std::vector<std::string>,
        hash<std::string> >::iterator s;

    // find all slots that accept the given category
    for (i = categories.begin (); i != categories.end (); i++)
    {
        s = SlotCategoryMap.find (*i);
        if (s == SlotCategoryMap.end ()) continue;
        
        // append some slots
        slots.insert (slots.end (), (*s).second.begin (), (*s).second.end ());
    }
    
    return slots;
}

// check whether the item can be equipped in given slot
bool manager::accepts (slot* target, item_base* item)
{
    // sanity check
    if (!target || !target->owner () || !item || !target->is_equipment () ||
         target->is_negated ()) return false;
    
    // get slots suitable for this item
    const std::vector<std::string> slots = fits (item);
    
    // is our slot among them?
    if (find (slots.begin (), slots.end (), target->id ()) == slots.end ())
        return false;
    
    // whether the item can be actually equipped depends on the character
    return item->can_equip (target->owner ()->owner ());
}

// try to equip given item
item_base *manager::equip (slot *target, item_base *item, u_int32 *count)
{
    // nothing to do
    if (*count == 0) return NULL;
    
    // item doesn't fit in slot for some reason.
    if (!accepts (target, item)) return item;

    slot *source = item->get_slot ();

    // make sure that unequip is invoked before item is equipped elsewhere
    if (source && source->is_equipment () && source->count () == *count)
        item->unequipped (source);
    
    // target slot occupied?
    if (target->count () > 0 && !target->accepts (item))
    {
        // exchange items if they are not stackable
        u_int32 old_count = target->count ();
        item_base *old_item = unequip (target, &old_count);
    
        target->add (item, *count);
        item->equipped (target);
        
        *count = old_count;
        return old_item;
    }
    else
    {
        // otherwise try to add as many items as possible
        u_int32 remaining = target->add (item, *count);
        
        // have any items been equipped?
        if (remaining < *count)
            item->equipped (target);
        
        // adjust count
        *count = remaining;
        
        if (*count > 0) return item;
        else return NULL;
    }
}

// unequip items from equipment slot
item_base *manager::unequip (slot *target, u_int32 *count)
{
    // nothing to do
    if (*count == 0) return NULL;

    // the item we will remove from the slot
    item_base *item = target->get_item ();

    if (item != NULL)
    {
        // try to remove the requested number of items    
        *count -= target->remove (item, *count);
    
        // let the character know when a slot was totally emptied.
        if (target->count () == 0) item->unequipped (target);
    
        // returned item(s) are no longer within a slot
        item->set_slot (NULL);
    }
   
    return item;
}

// tell manager that given slot accepts given category
void manager::add_mapping (const std::string & slot, const std::string & category)
{
    std::hash_map<std::string, std::vector<std::string>,
        hash<std::string> >::iterator s = SlotCategoryMap.find (category);
    
    // does the given category already exist?
    if (s != SlotCategoryMap.end ())
    {
        // add key to existing category
        std::vector<std::string> slots = (*s).second;
        if (find (slots.begin (), slots.end (), slot) == slots.end ())
            slots.push_back (slot);
    }
    else
    {
        // create new category
        std::vector<std::string> slots (1, slot);
        SlotCategoryMap[category] = slots;
    }
}
