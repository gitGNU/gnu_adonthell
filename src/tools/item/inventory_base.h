/*
   $Id$
   
   Copyright (C) 2002 Thorsten Riess <thorsten.riess@tu-ilmenau.de>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.

*/

#ifndef _INVENTORY_BASE_H
#define _INVENTORY_BASE_H

#include <vector>
#include <map>
#include <string>
#include "types.h"
#include "py_object.h"
#include "item_base.h"

class character_base;

/*
 * The inventory
 */
class inventory_base
{
public:
    inventory_base();
#ifndef SWIG
    inventory_base ( string n, string d, string f, character_base * c = NULL );
    inventory_base ( character_base * c );
#endif
    ~inventory_base (); 
    
    void init(string n, string d, string f, character_base * c = NULL );

    /*
    character_base * get_owner () const { return owner; };
    void set_owner ( character_base * c );*/
    
    string get_name () const { return name; };
    string get_description () const { return description; };
    
    void set_name ( string n ) { name = n; };
    void set_description ( string d ) { description = d; };
    
    bool add_item ( item_base * i , s_int32 a = 1 );
    bool remove_item ( item_base * i );

    item_base * get_item ( string n );

    s_int8 put ( ogzstream & file ) ;
    s_int8 save ( string f ) ;
    s_int8 get ( igzstream & file ) ;
    s_int8 load ( string f ) ;

    s_int8 put_state ( ogzstream & file ) ;
    s_int8 get_state ( igzstream & file ) ;

    void clear ();


#ifndef SWIG
    item_base * operator[] ( string n );

    typedef vector<item_base *>::iterator iterator;
    iterator begin () { return items_vector.begin (); }
    iterator end () { return items_vector.end (); }
    u_int32 size () { return items_vector.size (); }
#endif    

    bool transfer_item_to_inventory(item_base * it, inventory_base * in);

    u_int32 get_amount ( item_base * it );
    bool set_amount ( item_base * it, u_int32 am ); 
    bool adjust_amount ( item_base * it, s_int32 adj );

protected:
    string name;
    string description;
    character_base * owner;
    map<string, item_base *> items_map;
    map<item_base *, u_int32> amount_map;
    vector<item_base *> items_vector;
    py_object actions;
    PyObject * self;
};

#endif
