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
 * @file   item_storage.cc
 * @author Kai Sterker <kaisterker@linuxgames.com> 
 * 
 * @brief  Global storage for immutable items.
 */

#include "item_storage.h"

/**
 * Hashmap for keeping immutable items
 */
dictionary<item_base*> item_storage::Items;

// delete all items
void item_storage::cleanup ()
{
    std::hash_map<std::string, item_base*>::iterator i;
    
    // just a precaution
    for (i = Items.begin (); i != Items.end (); i++)
        (*i).second->set_slot (NULL);

    Items.clear ();
}

// add an (immutable) item
void item_storage::add (item_base *item)
{
    if (!item || item->is_mutable ()) return;

    // items are stored by name    
    Items[item->name ()] = item;
}

// get an item from storage
item_base *item_storage::get (const string & name)
{
    std::hash_map<std::string, item_base*>::iterator i = Items.find (name);
    
    // no such item
    if (i == Items.end ())
    {
        fprintf (stderr, "*** error: item_storage::get: no item called '%s'\n", name.c_str ());
        return NULL;
    }
    
    else return (*i).second;
}

// save storage to disk
bool item_storage::put_state (ogzstream & file)
{
    std::hash_map<std::string, item_base*>::iterator i;
    
    // iterate over hash_map and save all the items
    for (i = Items.begin (); i != Items.end (); i++)
    {
        true >> file;
        (*i).second->put_state (file);    
    }
    
    // no more items follow
    false >> file;
    
    return true;
}

// load storage from disk
bool item_storage::get_state (igzstream & file)
{
    item_base *item;
    bool more;
    more << file;
    
    while (more)
    {
        // retrieve item from file
        item = new item_base (false);
        item->get_state (file);
        
        // add item to storage
        Items[item->name ()] = item;
    
        // check whether more items follow
        more << file;    
    }
    
    return true;
}
