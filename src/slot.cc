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
 * @file   slot.cc
 * @author Kai Sterker <kaisterker@linuxgames.com> 
 * 
 * @brief  One unit of the inventory.
 */

#include "slot.h"
#include "item_base.h"

// ctor
slot::slot (/*inventory *owner,*/ const std::string & id)
{
    // Owner = owner;
    Id = id;
    Count = 0;
    Item = NULL;
}

// dtor
slot::~slot ()
{
    // if the slot contains item(s), delete them
    if (Item && Item->is_mutable ()) 
    {
        Item->set_slot (NULL);
        delete Item;
    }
}

// add item(s) to slot
u_int32 slot::add (item_base *item, const u_int32 & count)
{
    // nothing needs be done
    if (count == 0) return 0;

    // if we have an empty slot, we can just add the items
    if (!Item) Item = item;
    
    // otherwise check whether given items may go into this slot 
    else 
    {
        // item stackable?
        if (Count >= Item->max_stack ()) return count;
        
        // immutable items of same kind?
        if (!item->is_mutable () && item != Item) return count;
        
        // mutable item of same kind?
        if (item->is_mutable ())
        {
            if (item->name () == Item->name ()) Item->stack (item);
            else return count;
        }
    }
    
    // clculate how many items will fit into the slot
    u_int32 fitting = item->max_stack () - Count;
    fitting = (fitting > count) ? count : fitting;
    
    // adjust counts
    Count += fitting;
    
    // remove items from the inventory they came
    if (item->get_slot ()) item->get_slot ()->remove (item, fitting);
    
    // adjust slots
    if (item->is_mutable ()) item->set_slot (this);
    
    // return number of items that weren't added
    return count - fitting;
}

// remove item(s) from slot
bool slot::remove (item_base *item, const u_int32 & count)
{
    // nothing needs be done
    if (count == 0) return false;

    // in case items match, remove count items
    if (Item == item)
    {
        u_int32 remove = (Count > count) ? count : Count;
        
        // for immutable items, simply decrease the refcount
        Count -= remove;
        
        // for mutable items, also remove them from the stack
        if (Item->is_mutable ()) Item = Item->split (remove);
        
        // slot empty?
        if (Count == 0) Item = NULL;
        
        // return true if count items have been removed
        return remove == count;
    }
    
    // otherwise search through the stack to remove the item
    for (item_base *i = Item; i != NULL; i = i->Next)
        if (i->Next == item)
        {
            i->Next = item->Next;
            item->Next = NULL;
            item->Slot = NULL;
            Count--;
            
            return true;
        }
        
    return false;
}
