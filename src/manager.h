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
 * @file   manager.h
 * @author Kai Sterker <kaisterker@linuxgames.com> 
 * 
 * @brief  Takes care of character/item issues.
 */

#ifndef MANAGER_H
#define MANAGER_H

#include <vector>
#include "str_hash.h"
#include "item_base.h"

/**
 *
 */
class manager
{
public:
    /**
     * Return list of equipment slots this item fits into.
     * @param item The item whose slots to retrieve.
     * @returns List of slot ids.
     */
    const std::vector<std::string> fits (item_base *item);
    
    /**
     * Check whether a given item can be equipped in the given slot.
     * @param target An equipment slot.
     * @param item An item 
     * @return \b true if item can be equipped, \b false otherwise.
     */
    bool accepts (slot* target, item_base* item);

    /**
     * Actually (try to) equip the given item in the given slot. 
     * Replaces the item currently in the slot.
     * @param target An equipment slot.
     * @param item The item to equip.
     * @param count Number of items to add to the slot.
     * @return 
     *     - \c NULL if the the slot was empty and the item could be equipped.
     *     - The previously equipped item if the item could be equipped.
     *       \c count will contain the number of items that were equipped.
     *     - The given item if equipping wasn't possible.
     */    
    item_base* equip (slot *target, item_base* item, u_int32 *count);
    
    /**
     * Remove the item from the given slot.
     * @param target An equipment slot.
     * @param count Number of items to remove from the slot.
     * @return The item from the slot or \c NULL if the slot was empty.
     *     \c count will contain the number of items actually removed.
     */
    item_base* unequip (slot *target, u_int32 *count);

    /** 
     * Tell the manager that the given item category may go in the given slot.
     * @param slot The slot id.
     * @param category An item category.
     */
    static void add_mapping (const std::string & slot, const std::string & category);

private:
#ifndef SWIG
    static std::hash_map<std::string, std::vector<std::string>,
        hash<std::string> > SlotCategoryMap;
#endif // SWIG
};

#endif // MANAGER_H
