/*
   $Id$
       
   Copyright (C) 2002 Kai Sterker <kaisterker@linuxgames.com>
   Copyright (C) 2002 Thorsten Riess <thorsten.riess@tu-ilmenau.de>
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
 *         Thorsten Riess <thorsten.riess@tu-ilmenau.de>
 * 
 * @brief  Superclass for all items.
 */

#ifndef _ITEM_BASE_H
#define _ITEM_BASE_H

#include <string>
#include "types.h"
#include "storage.h"
#include "py_object.h"
//#include "character_base.h"
#include "fileops.h"

#define ACTION_DIR ""

//class inventory_base;
class character_base;

typedef enum
{
    EQUIP   = 1,
    COMBINE = 2,
    USE     = 4,
    ENCHANT = 8,
    DROP    = 16,
    PICKUP  = 32,
    CHARGE  = 64,
    EAT     = 128
    // ...
} action;

/**
 * defines for the different types of items
 */
#define BOOK    1
#define WEAPON  2
#define POTION  3
#define FOOD    4
#define KEY     5
#define ARMOR   6

/**
 * Base item class
 */    
class item_base : public storage
{
public:
    item_base ();
    ~item_base ();
#ifndef SWIG
    item_base (string n, string d, string f, u_int16 a, character_base * o = NULL);
#endif
    void init (string n, string d, string f, u_int16 a, character_base * o = NULL);

    string get_name () const { return name; }
    string get_description () const { return description; }

    /* add a new type
     *
     * available: weapon, book, food, ...
     */
    bool add_type ( string t );
    /* delete a type
     */
    bool del_type ( string t );
    /* get the complete vector of types
     */
    vector<string> & get_types ();
    /*
     * set the standard type
     */
    bool set_default_type ( string t );
    
    bool equip (character_base * c);
    bool combine (item_base * i);
    bool use ();
    bool drop ();
    bool pickup (character_base * c);
    bool hand_over (character_base * c);
    bool enchant ();
    bool sell (character_base * c, u_int16 p);
    bool eat ( character_base * c );
    
    bool has_ability (action a) { return abilities & a; };
    void set_ability (action a) { abilities |= a; };

    /*character_base * get_owner () { return owner; };
    void set_owner (character_base * owner) { owner = owner; };*/

    /*inventory_base * get_inventory () { return inventory; };
    void set_inventory ( inventory_base * i ) { inventory = i; };*/
    
    s_int8 put ( ogzstream & file ) ;
    s_int8 save ( string f ) ;
    s_int8 get ( igzstream & file ) ;
    s_int8 load ( string f ) ;

    //s_int8 put_state ( ogzstream & file ) ;
    //s_int8 get_state ( igzstream & file ) ;

    void clear ();
    
protected:
    string name;
    string description;
    u_int16 abilities;
    string std_type;
    vector<string> types;
    /*character_base * owner;
    inventory_base * inventory;*/
    py_object actions;
    PyObject *self;
};

#endif
