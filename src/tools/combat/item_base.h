/*
   $Id$
   
   Copyright (C) 2002 Kai Sterker <kaisterker@linuxgames.com>
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

#include <string>
#include "types.h"
#include "storage.h"
#include "py_object.h"
#include "character_base.h"

#define ACTION_DIR ""

typedef enum
{
    EQUIP   = 1,
    COMBINE = 2,
    USE     = 4,
    ENCHANT = 8
    // ...
} action;

#define BOOK    1
#define WEAPON  2
    
class item_base : public storage
{
public:
    item_base ();
    ~item_base () { actions.clear (); }
#ifndef SWIG
    item_base (string n, string d, string s, u_int16 a);
#endif
    void init (string n, string d, string s, u_int16 a);

    string get_name () const { return name; }
    string get_description () const { return description; }
    
    bool equip (character_base *c);
    bool combine (item_base *i);
    bool use ();
    
    bool has_ability (action a) { return abilities & a; }
    void set_ability (action a) { abilities |= a; }

protected:
    string name;
    string description;
    u_int16 abilities;
    py_object actions;
    PyObject *self;
};
