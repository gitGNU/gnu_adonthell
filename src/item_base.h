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
 * @file   item_base.h
 * @author Kai Sterker <kaisterker@linuxgames.com> 
 * 
 * @brief  Superclass for all items.
 */


#ifndef ITEM_BASE_H
#define ITEM_BASE_H

#include "character_base.h"
#include "py_object.h"
#include "fileops.h"

using std::string;

/**
 *
 */
class item_base : public py_object 
{
public:
    /**
     * Create a new item from the given template. 
     * @param item Name of the Python class defining the desired item.
     */
    item_base (string item);
    
    /**
     * Destructor.
     */
    ~item_base ();
    
    /**
     * Initialize the basic item attributes from python side.
     * @param item Derived item instance.
     */
    void init (PyObject *item);
    
    /**
     * @name Item Actions
     */
    //@{
    /**
     * Use an item. Triggers its main functionality if it can be used at all.
     * @param character The character using this item.
     * @return \b true if item has been used successfully, \b false otherwise. 
     */
    bool use (character_base *character);
    //@}
            
    /**
     * @name Attribute access
     */
    //@{
    /**
     * Make the item member of the given category.
     * @param type the category this item should belong to.
     */
    void add_type (const string &type);
    
    /**
     * Take a category away from the item.
     * @param type category to remove from the item. 
     */
    void remove_type (const string &type);
    //@}
    
protected:
    /**
     * @name Loading/Saving
     */
    //@{
    /**
     * Load item state from file. This method has to be called from the
     * derived class before loading itself. Calling it from C++ side will 
     * fail to load the complete item.
     *
     * @param file stream to load item from
     * @return \b true if loading successful, \b false otherwise
     */
    bool get_state (igzstream& file);
    
    /**
     * Save item state to file. This method has to be called from the
     * derived class before saving itself. Calling it from C++ side will 
     * fail to save the complete item.
     *
     * @param file stream to save item to
     */
    void put_state (ogzstream &file) const;
    //@}

    /**
     * @name Basic Item Attributes
     */
    //@{
    /**
     * List of categories this item belongs to.
     */
    std::vector<std::string> types;
    
    /**
     * Fuel or number of uses an item has.
     */
    u_int16 charge;
    //@}
};

#endif // ITEM_BASE_H
