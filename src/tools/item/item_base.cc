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
 * @file   item_base.cc
 * @author Kai Sterker <kaisterker@linuxgames.com> 
 *         Thorsten Riess <thorsten.riess@tu-ilmenau.de>
 * 
 * @brief  Superclass for all items.
 */

#include "item_base.h"
#include "python_class.h"

item_base::item_base ()
{
    abilities = 0;
    name = "";
    description = "";
    //owner = NULL;
    //inventory = NULL;
    self = python::pass_instance (this, "item_base");
}

item_base::item_base (string n, string d, string f, u_int16 a, character_base * o)
{
    init (n, d, f, a, o);
}

void item_base::init (string n, string d, string f, u_int16 a, character_base * o)
{
    name = n;
    description = d;
    abilities = a;
    //owner = o;
    //inventory = NULL

    string file = ACTION_DIR;
    file += f;
    
    types.push_back ( f );
    std_type = f;
    actions.create_instance (file, f);
    self = python::pass_instance (this, "item_base");
}

item_base::~item_base ()
{
    clear ();
}

void item_base::clear ()
{
    actions.clear ();
    types.clear ();
    name = "";
    description = "";
    storage::clear();
}

s_int8 item_base::load ( string f )
{
    igzstream is ( f );
    return get ( is );
}

s_int8 item_base::get ( igzstream & file )
{
    clear ();
    name << file;
    description << file;
    u_int32 s;
    string str;
    s << file;
    for ( u_int32 i = 0 ; i < s ; i++ ) {
        str << file;
        types.push_back ( str );
    }
    string std_type_tmp;
    std_type_tmp << file;
    set_default_type ( std_type_tmp );
    abilities << file;
    get_state ( file );
    return 0;
}

s_int8 item_base::save ( string f )
{
    ogzstream os ( f );
    return put ( os );
}

s_int8 item_base::put ( ogzstream & file )
{
    name >> file;
    description >> file;
    u_int32 s = types.size ();
    s >> file;
    vector<string>::iterator it = types.begin ();
    while ( it != types.end () ) {
        (*it) >> file;
        it++;
    }
    std_type >> file;
    abilities >> file;
    put_state ( file );
    return 0;
}

/*
s_int8 item_base::get_state ( igzstream & file )
{
    // owner->get...
    return 0;
}

s_int8 item_base::put_state ( ogzstream & file )
{
    // owner->put...
    return 0;
}
*/

bool item_base::set_default_type ( string t )
{
    vector<string>::iterator iter = find ( types.begin (), types.end (), t );
    if ( iter == types.end () )
        return false;
    if ( t == std_type )
        return true;
    actions.clear ();
    
    string file = ACTION_DIR;
    file += t;
    
    //types.push_back ( t );
    std_type = t;
    actions.create_instance (file, t);
    return true;
}

bool item_base::equip (character_base *c)
{
    if ( (abilities & EQUIP == 0) /* || (owner!=c) */ ) return false;
    
    PyObject* args = PyTuple_New (2);
    Py_INCREF (self);
    PyTuple_SetItem (args, 0, self);
    PyTuple_SetItem (args, 1, python::pass_instance (c, "character_base"));
    actions.call_method ("equip", args);  
    Py_DECREF (args);

    return true;
}

bool item_base::combine (item_base *i)
{
    if (abilities & COMBINE == 0) return false;
    
    PyObject* args = PyTuple_New (2);
    Py_INCREF (self);
    PyTuple_SetItem (args, 0, self);
    PyTuple_SetItem (args, 1, python::pass_instance (i, "item_base"));
    actions.call_method ("combine", args);  
    Py_DECREF (args);

    return true;
}

bool item_base::use ()
{
    if (abilities & USE == 0) return false;
    
    PyObject* args = PyTuple_New (1);
    Py_INCREF (self);
    PyTuple_SetItem (args, 0, self);
    actions.call_method ("use", args);  
    Py_DECREF (args);

    return true;
}

bool item_base::eat ( character_base * c )
{
    if (abilities & EAT == 0) return false;
    
    PyObject* args = PyTuple_New (2);
    Py_INCREF (self);
    PyTuple_SetItem (args, 0, self);
    PyTuple_SetItem (args, 1, python::pass_instance(c, "character_base") );
    actions.call_method ("eat", args);  
    Py_DECREF (args);

    return true;
}



bool item_base::drop ()
{
    if (abilities & DROP == 0) return false;

    PyObject* args = PyTuple_New (1);
    Py_INCREF (self);
    PyTuple_SetItem (args, 0, self);
    actions.call_method("drop", args);
    Py_DECREF (args);
    //owner = NULL;

    return true;
}

bool item_base::pickup (character_base * c)
{
    if (abilities & PICKUP == 0) return false;

    PyObject* args = PyTuple_New (2);
    Py_INCREF (self);
    PyTuple_SetItem (args, 0, self);
    PyTuple_SetItem (args, 1, python::pass_instance(c, "character_base"));
    actions.call_method("pickup", args);
    Py_DECREF (args);
    //owner = c;

    return true;
}

bool item_base::hand_over (character_base * c)
{
    if ( /* (owner == NULL) || */ (c==NULL) ) return false;
    return drop() && pickup(c);
}

bool item_base::enchant (/* spell... */)
{
    if ( abilities & ENCHANT == 0) return false;
    
    PyObject * args = PyTuple_New(1);
    Py_INCREF (self);
    PyTuple_SetItem (args, 0, self);
    actions.call_method("enchant", args);
    Py_DECREF (args);

    return true;
}

bool item_base::sell (character_base * c, u_int16 p)
{
    if ( (abilities & DROP == 0) || (c == NULL) /* || (owner == NULL) */ ) return false;

    if (! (drop() && pickup(c)) )
	return false;
    
//    inventory_base * inv_owner = owner->get_inventory();
//    inventory_base * inv_buyer = c->get_inventory();
    return true;
}

bool item_base::add_type ( string t )
{
    vector<string>::iterator iter = find ( types.begin (), types.end (), t );
    if ( iter != types.end () )
        types.push_back ( t );
    return true;
}

bool item_base::del_type ( string t )
{
    vector<string>::iterator iter = find ( types.begin (), types.end (), t );
    if ( ( iter != types.end () ) && ( t != std_type ) )
    {
        types.erase ( iter );
        return true;
    }
    else
        return false;
}

vector<string> & item_base::get_types ()
{
    return types;
}
