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
 * @file   item_storage.h
 * @author Kai Sterker <kaisterker@linuxgames.com> 
 * 
 * @brief  Global storage for immutable items.
 */

#ifndef ITEM_STORAGE_H
#define ITEM_STORAGE_H

#include "storage.h"
#include "item_base.h"

using std::string;

/**
 * For efficiency reasons, immutable items are only instanciated once
 * throughout a game. In order to access them, they are kept in a globally
 * available %storage.
 */
class item_storage
{
public:
    /**
     * Initialization at engine startup.
     */
    static void init () {}

    /**
     * Remove all items from the %storage. This should be done whenever
     * a %game is unloaded.
     */
    static void cleanup ();

    /**
     * @name Item handling
     */
    //@{
    /**
     * Add an item to the %storage. The item needs to be immutable,
     * otherwise it won't be added. It should also have a unique name.
     * @param item The item to add to the storage
     */
    static void add (item_base *item);
    
    /**
     * Retrieve immutable item with given name from the %storage. 
     * Instead of an immutable item's name, one may also pass the file
     * name of a mutable item. In that case, the mutable item will be
     * instanciated and returned.
     * @param name name of the item to retrieve.
     * @return item with given name, or \c NULL if no such item exists.
     */
    static item_base *get (const string & name); 
    //@}
    
#ifndef SWIG
    /**
     * @name Loading/Saving
     */
    //@{
    /**
     * Load all items kept in the %storage from stream. 
     * @param file stream to load items from.
     * @return \b true if loading successful, \b false otherwise.
     */
    static bool get_state (igzstream & file);
    
    /**
     * Save contents of the %storage to a stream.
     * @param file stream to save %item_storage to.
     * @return \b true if saving successful, \b false otherwise.
     */
    static bool put_state (ogzstream & file);
    //@}
        
private:
    /**
     * Hashmap for keeping the items
     */
    static dictionary<item_base*> Items;
#endif // SWIG
};

#endif // ITEM_STORAGE_H
