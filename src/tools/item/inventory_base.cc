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


/**
 * @file   inventory_base.h
 * @author Thorsten Riess <thorsten.riess@tu-ilmenau.de>
 * 
 * @brief  Superclass for all inventories.
 */

#include "inventory_base.h"
#include "character_base.h"
#include "item_base.h"
#include "python_class.h"

/*
 * standard constructor
 */
inventory_base::inventory_base ()
{
    //owner = NULL;
    name = "";
    description = "";

    self = python::pass_instance (this, "inventory_base");

}

inventory_base::inventory_base ( character_base * c )
{
    //owner = c;
    name = "";
    description = "";
    
    self = python::pass_instance (this, "inventory_base");
}
/*
 * constructor (not visible from python)
 */
/*
inventory_base::inventory_base (string n, string d, string f, character_base * c)
{
    init (n, d, f, c);
}*/

/*
 * initialize the class (called by the constructor)
 */
void inventory_base::init (string n, string d, string f, character_base * c)
{
    owner = c ;
    name = n ;
    description = d ;
    
    string file = ACTION_DIR;
    file+=f;
    

    actions.create_instance (file, f);
    self = python::pass_instance (this, "inventory_base");
}

/*
 * Destructor
 * deletes all items in the inventory
 */
inventory_base::~inventory_base ()
{
    clear ();
}

void inventory_base::clear ()
{
    vector<item_base *>::iterator it = items_vector.begin ();
    while ( it != items_vector.end () ) {
        delete ( *it );
        it++;
    }
    items_vector.clear ();
    items_map.clear ();
    amount_map.clear ();
}

/*
void inventory_base::set_owner ( character_base * c )
{
    owner = c;
    vector<item_base *>::iterator iter=items_vector.begin ();
    while ( iter != items_vector.end () ) {
        (*iter)->set_owner ( c );
        iter++;
    }
}
*/

/*
 * add an item to the inventory
 */
bool inventory_base::add_item(item_base * i, s_int32 a)
{
    if ( i == NULL )
        return false;
    string n = i->get_name ();
    map<string,item_base *>::iterator iter = items_map.find ( n );
    if ( iter != items_map.end () ) {
        adjust_amount ( iter->second, a);
        delete i;
        return true;
    }
    else {
        items_vector.push_back ( i );
        items_map [ i->get_name() ] = i;
        amount_map [ i ] = a;
        return true;
    }
    return false;
/*
    if( i != NULL ) {
        s_int32 amount_there = i->get_val("amount");
        
        if ( amount_there == 0 ) {
            amount_there = 1;
            i->set_val("amount",1);
        }
        
        if ( a > amount_there )
            return false; // cannot transfer more than there is
        
        inventory_base *inv;
        if ( ( inv=i->get_inventory () ) != NULL ) { // an item cannot be in two inventories
            inv->remove_item ( i );
        }
        
        item_base * item_here;
        if ( (item_here = get_item_by_name ( i->get_name() ) ) != NULL ) { // is the item already here?
            s_int32 amount_here=item_here->get_val("amount");
            if ( amount_here == 0 )
                item_here->set_val("amount",2);
            else
                item_here->set_val("amount",amount_here+a);
            if ( amount_there-a == 0 )
                delete i;
            else
                i->set_val("amount",amount_there-a);
        }
        else {
            if ( amount_there == a ) {
                i->set_inventory( this );
                items_vector.push_back(i);
                items_map_string[ i->get_name() ] = i;
            }
            else {
                item_here = new item_base( *i );
            }
        }
    }
    else
        return false;
    return true;
*/
}

/*
 * remove an item from the inventory
 */
bool inventory_base::remove_item(item_base * i)
{
    map<string, item_base *>::iterator iter2 = items_map.find ( i->get_name () );
    if ( iter2 != items_map.end () ) {
        items_map.erase ( iter2 );
    }
    else
        return false;

    map<item_base *, u_int32>::iterator iter3 = amount_map.find ( i );
    if ( iter3 != amount_map.end () ) {
        amount_map.erase ( iter3 );
    }
    else
        return false;
    
    vector<item_base *>::iterator iter=find(items_vector.begin(),items_vector.end(),i);
    if(iter!=items_vector.end()) {
        //i->set_inventory( NULL );
        items_vector.erase(iter);
    }
    else
        return false;
    return true;
}

/*
 * transfer item to another inventory
 */
bool inventory_base::transfer_item_to_inventory(item_base * it, inventory_base * in)
{
    vector<item_base *>::iterator iter = find ( items_vector.begin (), items_vector.end (), it );
    if ( iter != items_vector.end () ) {
        in->add_item(it);
        remove_item(it);
    }
    else
        return false;
    return true;
}

item_base * inventory_base::get_item ( string n )
{
    map<string, item_base *>::iterator iter = items_map.find ( n );
    if ( iter != items_map.end () )
        return items_map [ n ];
    else
        return NULL;
}

item_base * inventory_base::operator[] (string n )
{
    return get_item ( n );
}

u_int32 inventory_base::get_amount ( item_base * it )
{
    vector<item_base *>::iterator iter = find ( items_vector.begin (), items_vector.end (), it );
    if ( iter != items_vector.end () )
        return amount_map [ it ];
    else
        return 0;
}

bool inventory_base::set_amount ( item_base * it , u_int32 am)
{
    vector<item_base *>::iterator iter = find ( items_vector.begin (), items_vector.end (), it );
    if ( iter != items_vector.end () )
    {
        amount_map [ it ] = am;
        return true;
    }
    else
        return false;
}

bool inventory_base::adjust_amount ( item_base * it , s_int32 adj )
{
    vector<item_base *>::iterator iter = find ( items_vector.begin (), items_vector.end (), it );
    if ( iter != items_vector.end () )
    {
        amount_map [ it ] += adj;
        return true;
    }
    else
        return false;

}

s_int8 inventory_base::load ( string f )
{
    igzstream is ( f );
    return get ( is );
}

s_int8 inventory_base::get ( igzstream & file )
{
    clear ();
    name << file;
    description << file;
    u_int32 i, j;
    item_base * new_item;
    i << file;
    for ( j = 0; j < i; j++ ) {
        new_item = new item_base ();
        new_item->get ( file );
        add_item ( new_item );
    }
    return 0;
}

s_int8 inventory_base::save ( string f )
{
    ogzstream os ( f );
    return put ( os );
}

s_int8 inventory_base::put ( ogzstream & file )
{
    name >> file;
    description >> file;
    u_int32 s = items_vector.size ();
    s >> file;
    item_base * item_pointer;
    vector<item_base *>::iterator it = items_vector.begin ();
    while ( it != items_vector.end () ) {
        item_pointer = (*it);
        item_pointer->put ( file );
        it++;
    }
    return 0;
}

s_int8 inventory_base::get_state ( igzstream & file )
{
    // owner->get...
    return 0;
}

s_int8 inventory_base::put_state ( ogzstream & file )
{
    // owner->put...
    return 0;
}

