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
inventory::inventory (const u_int16 & size, const bool & limited)
{
    QueryType = 0;
    QueryKey = "";
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
    for (u_int16 i = 0; i < count; i++)
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
    // nothing to do
    if (count == 0) return 0;
    
    std::vector<slot*>::iterator i;
    s_int32 remaining = count;
    
    // if item stackable, look for matching stack
    if (item->max_stack () > 1)
    {
        for (i = Slots.begin (); i != Slots.end (); i++)
            if ((*i)->count () > 0 && (*i)->accepts (item)) 
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

    // if there are still items left, grow inventory if allowed
    if (remaining && !Limited)
    {
        slot *s;
        
        while (remaining)
        {
            s = new slot (this);
            remaining = s->add (item, remaining);
            Slots.push_back (s);
        }
    }
    
    // return how many items didn't fit
    return remaining;
}

// combine two (stacks of) items
bool inventory::combine (slot *target, slot *agent)
{
    slot *tmp = new slot (NULL);
    u_int32 count = 0;
    item_base *result, *item;
    
    // combine agent with all items in target slot
    while (target->count () > 0 && agent->count () > 0)
    {
        // get result of combination
        item = target->get_item ();
        result = item->combine (agent->get_item ());
        
        // combination successful?
        if (!result)
        {
            // if it wasn't the first try, we screwed up
            if (count > 0)
            {
                fprintf (stderr, "*** error:: inventory::combine: not all items could be combined!\n");
                
                // try to recover already created items
                break;
            }
            
            // otherwise we can happily tell that the items weren't combinable
            delete tmp;
            return false;
        }
        
        // if new item was created, destroy old one
        if (item != result) item->destroy ();
        
        // this shouldn't fail if items are designed properly
        if (tmp->add (result) != 0)
        {
            fprintf (stderr, "*** error: inventory::combine: result not fitting into temporary slot!\n");
            
            // cleanup
            result->destroy ();
            break;
        }
        
        count++;
    }
    
    // if target slot is empty, add resulting items to target slot
    if (target->count () == 0) target->add (tmp->get_item (), count);
    
    // otherwise add result to a free slot (or existing stack)
    else add (tmp->get_item (), count);
    
    // do items remain in temporary slot? (this is not supposed to happen)
    if (tmp->count () != 0)
    {
        // FIXME: place remaining items on the ground 
        //        for now just utter warning
        fprintf (stderr, "*** warning: inventory::combine: %i newly created items do not fit\n", tmp->count ());
    }
    
    // cleanup
    delete tmp;
    
    return true;
}

// find first matching item
item_base *inventory::find (const string & key, const u_int8 & match)
{
    // init query
    QueryKey = key;
    QueryType = match;
    I = Slots.begin ();
    
    // actually start searching
    return find_next ();
}

// find more matching items
item_base *inventory::find_next ()
{
    item_base *item;
    
    for (; I != Slots.end (); I++)
    {
        // get item from slot
        if ((*I)->count () > 0) item = (*I)->get_item ();
        else continue;
        
        // compare
        switch (QueryType)
        {
            // match name
            case inventory::MATCH_NAME:
                if (item->name () == QueryKey) return item;
                else break;
            
            // match category
            case inventory::MATCH_CATEGORY:
                if (item->is_a (QueryKey)) return item;
                else break;
            
            // wrong query type
            default: return NULL;
        }
    }
    
    // reached end of inventory
    return NULL;
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
        // in case of unlimited inventory, don't bother with saving empty slots
        if (!Limited && (*i)->count () == 0) continue;
        
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
