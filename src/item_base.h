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

#include "py_object.h"
#include "fileops.h"
#include "slot.h"

using std::string;

class character_base;

/**
 * It is a wrapper for item.py, which is the actual item superclass.
 * For flexibility, items are implemented on python side. But since they
 * are often used on C++ side, this class provides methods to the most
 * basic item attributes and all possible actions an item might support.
 *
 * So while each item does have the wrapped attributes, an item may only
 * implement a subset of the available actions ("pick_up", "drop", "equip",
 * "unequip", "combine" and "use"). Further, an item may have additional
 * attributes. If required, they can be accessed via methods provided by
 * the py_object class.
 *
 * There are two 'classes' of items. Of immutable items, only one %object
 * will be instanciated when loading a saved %game. All copies of such an
 * item are actually references to the one object. Mutable items however 
 * will be instanciated as often as copies of that item exist.
 */
class item_base : public py_object 
{
public:

    /**
     * Default constructor. Creates an empty item. For safety reasons
     * the item's 'class' has to set at construction time. It cannot
     * be changed ever after.
     * @param is_mutable whether the item may change its state or not.
     * @see is_mutable ()
     */
    item_base (bool is_mutable) : py_object () 
    {
        Mutable = is_mutable;
        MaxStack = 1;
        Next = NULL;
        Slot = NULL;
    }
    
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
     * Removes an item from its slot and deletes it if necessary. This
     * method should be preferred over deleting an item directly, as it
     * takes care of both mutable and immutable items.
     */
    void destroy ()
    {
        if (!Mutable)
        {
            if (Slot) Slot->remove (this);
        }
        else delete this;
    }
    
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

    /**
     * Combine this item with another item. Returns the result of the
     * combination, or NULL if the combination fails.
     * @param item the item to combine with this one
     * @return item resulting from combination, or \c NULL if combination fails.
     */
    item_base *combine (item_base * item);
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

    /**
     * Check whether the item is mutable or immutable. Mutable
     * items are those that may change their attributes during
     * livetime. Therefore they need to be treated different
     * from items that do never change as long as they live.
     * @return \b true if the item is mutable, \b false otherwise.
     */
    bool is_mutable () const
    {
        return Mutable;
    }
    
    /**
     * If the item is kept in an inventory, it will be assigned
     * to a certain slot. For mutable items, the assignment will be
     * pertinent. For immutable items however, it will be only temporary
     * and only valid for the item that is currently manipulated. In
     * reality, no difference should be notable though.
     * @return slot holding the item or \c NULL, if it isn't kept in
     *      an inventory.
     */
    slot *get_slot () const
    {
        return Slot;
    }
    
    /**
     * Set the slot this (stack of) item(s) is kept in.
     * @param s the slot to assign to the item(s).
     */
    void set_slot (slot *s)
    {
        Slot = s;
        if (Next != NULL) Next->set_slot (s);
    }
    
    /** 
     * Check how many items of this kind may be stacked in one slot.
     * @return maximum size of a stack of this kind of item.
     */    
    u_int32 max_stack () const
    {
        return MaxStack;
    }
    
    /** 
     * Set how many items of this kind may be stacked in one slot.
     * @param max_stack maximum size of a stack of this kind of item.
     */    
    void set_max_stack (u_int32 max_stack)
    {
        MaxStack = max_stack;
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

    /**
     * Chack whether two are of the same kind. For mutable items this is
     * the case if they have the same name. For immutable items, they need
     * to be the same instances.
     * @param the item to compare this one to.
     * @return \b true if items are (roughly) equal, \b false otherwise.
     */
    bool equals (item_base *item);
             
protected:
#ifndef SWIG
    /**
     * The number of items that can be stacked
     */
    u_int32 MaxStack;
            
    /**
     * The slot this item is kept in.
     */
    slot *Slot;

    /**
     * Whether the item is mutable or immutable
     */
    bool Mutable;
    
    /**
     * If we're dealing with a stack of mutable items, Next will point
     * to the next item in the stack.
     */
    item_base *Next;
    
    /**
     * @name Item Stack handling
     */
    //@{
    friend u_int32 slot::add (item_base * item, const u_int32 & count = 1);
    friend bool slot::remove (item_base * item, const u_int32 & count = 1);

    /**
     * Add an item (or stack of items) to this item's stack. We assume
     * that checks for item equality and number of items on the stack
     * have been made already. That's why this method is only available 
     * to friends of %item_base. In fact, it should only be used by
     * slot::add_item and slot::remove_item.
     * @param stack Item (stack) to be added to this item. It will be the
     *      new top of stack.
     */
    void stack (item_base *stack);
    
    /**
     * Retrieve number items from this stack. This method will return
     * the topmost item(s) of the stack. Like item_base::stack, this
     * method is only available to the slot class.
     * @param number Number of items to retrieve from the stack.
     * @return the item(s) remaining in this stack.
     */
    item_base *split (u_int32 number = 1);
    //@}
#endif SWIG
};

#endif // ITEM_BASE_H
