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
 * It is a wrapper for item_base.py, which is the actual item superclass.
 * For flexibility, items are implemented on python side. But since they
 * are often used on C++ side, this class provides methods to the most
 * basic item attributes and all possible actions an item might support.
 *
 * So while each item does have the wrapped attributes, an item may only
 * implement a subset of the available actions ("pick_up", "drop", "equip",
 * "unequip", "combine" and "use"). Further, an item may have additional
 * attributes. If required, they can be accessed via methods provided by
 * the py_object class.
 */
class item_base : public py_object 
{
public:

    /**
     * Default constructor. Creates an empty item.
     */
    item_base () : py_object () {};
#ifndef SWIG    
    /**
     * Create a new item from the given item data. 
     * @param item Name of the item data file defining the desired item.
     */
    item_base (const string & item);
#endif    
    /**
     * Destructor.
     */
    ~item_base ();
    
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
     * Get the item's name.
     * @return name of the item.
     */
    string name ()
    {
        return get_attribute_string ("Name");
    }

    /**
     * Set the item's name.
     * @param name the item's new name.
     */
    void set_name (const string & name)  
    {
        set_attribute_string ("Name", name); 
    }
    
    /**
     * Make the item member of the given category.
     * @param category the category this item should belong to.
     */
    void add_category (const string & category)
    {
        PyObject *args = Py_BuildValue ("(s)", (char *) category.c_str ());
        call_method ("add_category", args);
    }
    
    /**
     * Take a category away from the item.
     * @param category the category to remove from the item. 
     */
    void remove_category (const string & category)
    {
        PyObject *args = Py_BuildValue ("(s)", (char *) category.c_str ());
        call_method ("remove_category", args);
    }

    /**
     * Check whether item belongs to a certain category.
     * @param category the category to test against.
     * @return \b true if item belongs to given category, \b false otherwise.
     */
    bool is_a (const string & category)
    {
        int result = 0;
        PyObject *args = Py_BuildValue ("(s)", (char *) category.c_str ());
        PyObject *retval = call_method_ret ("is_a", args);
        
        if (retval && PyInt_Check (retval)) result = PyInt_AS_LONG (retval);
        Py_XDECREF (retval);
        
        return (result != 0);
    }
    
    /**
     * Get the charge the item has left.
     * @return number of charges the item has.
     */
    u_int16 charge ()
    {
        return (u_int16) get_attribute_int ("Charge");
    }

    /**
     * Set the number of charges the item may hold.
     * @param charge item's number of charges.
     */
    void set_charge (u_int16 charge)  
    {
        set_attribute_int ("Charge", charge); 
    }
    
    /**
     * Get the maximum number of charges the item may hold.
     * @return item's maximum number of charges.
     */
    u_int16 max_charge ()
    {
        return (u_int16) get_attribute_int ("MaxCharge");
    }

    /**
     * Set the maximum number of charges the item may hold.
     * @param max_charge item's maximum number of charges.
     */
    void set_max_charge (u_int16 max_charge)
    { 
        set_attribute_int ("MaxCharge", max_charge); 
    }
    //@}
    
    /**
     * @name Loading/Saving
     */
    //@{
#ifndef SWIG
    /**
     * Load item from named file. This will first load the item template
     * to instanciate the underlying Python item class. Then it will
     * restore the actual item data. If an item is already instanciated,
     * it will be replaced.
     *
     * @param file name of the file to load item from.
     * @return \b true if loading successful, \b false otherwise.
     */
    bool get_state (const string & file);
    
    /**
     * Save item to named file. This will save both the item template
     * plus the actual data to the given file. The file will be replaced
     * if it already exists.
     *
     * @param file name of the file to save item to.
     * @return \b true if saving successful, \b false otherwise.
     */
    bool put_state (const string & file) const;
#endif
    
    // wrappers needed for SWIG
    bool load (const string & file) { return get_state (file); }
    bool save (const string & file) { return put_state (file); }
        
    /**
     * Load item from stream. This will first load the item template
     * to instanciate the underlying Python item class. Then it will
     * restore the actual item data. If an item is already instanciated,
     * it will be replaced.
     *
     * @param file stream to load item from.
     * @return \b true if loading successful, \b false otherwise.
     */
    bool get_state (igzstream & file);
    
    /**
     * Save item to stream. This will save both the item template
     * plus the actual data to the given file. The file will be replaced
     * if it already exists.
     *
     * @param file stream to save item to.
     * @return \b true if saving successful, \b false otherwise.
     */
    bool put_state (ogzstream & file) const;
    //@}
};

#endif // ITEM_BASE_H
