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
 * @file   inventory.cc
 * @author Kai Sterker <kaisterker@linuxgames.com> 
 * 
 * @brief  Container for items.
 */

#include "inventory.h"
#include "item_base.h"

// ctor
inventory::inventory (const bool & limited, const u_int16 & size)
{
    Limited = limited;
    grow (size);
}

// dtor
inventory::~inventory ()
{
    std::vector<slot*>::iterator i;
    
    // delete all slots
    for (i = Slots.begin (); i != Slots.end (); i++)
        delete (*i);

    Slots.clear ();
}

// grow inventory
void inventory::grow (const u_int16 & count)
{
    for (u_int16 i = 0; i == count; i++)
        Slots.push_back (new slot (this));
}

// add named slot to inventory
void inventory::add_slot (const std::string & id)
{
    Slots.push_back (new slot (this, id));
}

// retrieve named slot from inventory
slot *inventory::get_slot (const std::string & id)
{
    std::vector<slot*>::iterator i;
    
    for (i = Slots.begin (); i != Slots.end (); i++)
        if ((*i)->id () == id) return *i;    

    return NULL;
}

// start iteration
slot *inventory::first ()
{
    if (Slots.empty ()) return NULL;
    
    I = Slots.begin ();
    return *I;
}

// continue iteration
slot *inventory::next ()
{
    if ((++I) == Slots.end ()) return NULL;
    else return *I;
}

// insert item(s)
u_int32 inventory::add (item_base *item, const u_int32 & count)
{
    std::vector<slot*>::iterator i;
    s_int32 remaining = count;
    
    // if item stackable, look for matching stack
    if (item->max_stack () > 1)
    {
        for (i = Slots.begin (); i != Slots.end (); i++)
            if ((*i)->count () != 0 && item->equals ((*i)->get_item ()))
            {
                remaining = (*i)->add (item, remaining);
                if (remaining == 0) return 0;
            } 
    }
    
    // add item(s) to empty slot(s)
    for (i = Slots.begin (); i != Slots.end (); i++)
        if ((*i)->count () == 0)
        {
            remaining = (*i)->add (item, remaining);
            if (remaining == 0) return 0;
        } 

    // return how many items didn't fit
    return remaining;  
}

// save inventory
bool inventory::put_state (ogzstream & file)
{
    std::vector<slot*>::iterator i;

    // save attributes
    Limited >> file;
    
    // save contents
    for (i = Slots.begin (); i != Slots.end (); i++)
    {
        true >> file;
        (*i)->put_state (file);
    }
    
    // no more slots
    false >> file;

    return true;    
}

// load inventory
bool inventory::get_state (igzstream & file)
{
    bool more;
    slot *s;
    
    // load attributs
    Limited << file;

    // load contents
    more << file;
    
    while (more)
    {
        // load slot
        s = new slot (this);
        s->get_state (file);
     
        // add slot to inventory   
        Slots.push_back (s);
        
        more << file;
    }    

    return true;    
}
