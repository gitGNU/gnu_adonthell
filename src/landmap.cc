/*
   $Id$

   Copyright (C) 1999/2000/2001   Alexandre Courbot
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/**
 * @file landmap.cc
 * Defines the landmap class.
 */


#include "landmap.h"
#include "character.h"

using namespace std; 

/** 
 * Returns a newly allocated enter_event.
 *  
 */ 
NEW_EVENT(enter_event); 

/** 
 * Returns a newly allocated leave_event.
 *  
 */ 
NEW_EVENT(leave_event); 

enter_event::enter_event () : base_map_event ()
{
    type = ENTER_EVENT; 
}

leave_event::leave_event () : base_map_event ()
{
    type = LEAVE_EVENT; 
}

base_map_event::base_map_event ()
{
    submap = x = y = dir = map = -1;
    c = NULL;
}

// compare two enter events
bool base_map_event::equals (event &e)
{
    // we know that we've got an enter_event :)
    base_map_event tmp = (base_map_event &) e;

    if (submap != -1 && tmp.submap != submap) return false;
    if (x != -1 && tmp.x != x) return false;
    if (y != -1 && tmp.y != y) return false;
    if (dir != -1 && tmp.dir != dir) return false;
    if (map != -1 && tmp.map != map) return false;
    if (c && tmp.c != c) return false;
    
    return true;
}

// Execute enter event's script
void base_map_event::execute (event& e)
{
    base_map_event t = (base_map_event&) e; 
    // Build the event script's local namespace
    PyObject *locals = Py_BuildValue ("{s:i,s:i,s:i,s:i,s:s}", "posx", t.x, 
        "posy", t.y, "dir", t.dir, "map", t.map, "name", t.c->get_name().c_str ());

    // Execute script
    script.set_locals (locals); 
    script.run ();
    script.set_locals (NULL);
    
    // Cleanup
    Py_DECREF (locals);

#ifdef _DEBUG_
    show_traceback ();
#endif
}

// Load a enter event from file
bool base_map_event::load (igzstream& f)
{
    string name;
    string s; 
    
    submap << f; 
    x << f;
    y << f;

    dir << f;
    map << f;

    s << f;
    if (s != "") 
        c = (mapcharacter*) data::characters[s.c_str ()];
    else c = NULL; 
    
    s << f;
    set_script (s); 

    return true;
}

// Save enter_event to file
void base_map_event::save (ogzstream& out) const
{
    type >> out;
    submap >> out; 
    x >> out;
    y >> out;
    dir >> out;
    map >> out;
    if (c)
        c->get_name () >> out;
    else 
    {
        string s = ""; 
        s >> out;
    }

    script_file () >> out; 
}  

landmap::landmap () : event_list () 
{
    // doing this here means some slight overhead, but at least
    // its save and we don't have additional dependencies.
    REGISTER_EVENT (ENTER_EVENT, enter_event)
    REGISTER_EVENT (LEAVE_EVENT, leave_event) 
}

landmap::~landmap ()
{
    clear (); 
}

void landmap::clear () 
{
    // Clear all events
    event_list::clear (); 
    
    // Remove all mapcharacters from this map.

    while (mapchar.size ())
        mapchar.front ()->remove_from_map ();  
    
    // Delete all mapobjects
    vector <mapobject *>::iterator io;
    for (io = mobj.begin (); io != mobj.end (); io++)
        delete (*io);
    mobj.clear ();

    // Delete all mapobjects filenames
    mobjsrc.clear (); 
    
    // Delete all submaps
    vector <mapsquare_area *>::iterator is;
    for (is = submap.begin (); is != submap.end (); is++)
        delete (*is);
    submap.clear ();

    // Reset the filename.
    filename_ = ""; 
} 

void landmap::update () 
{
    // Update mapobjects
    vector <mapobject *>::iterator io; 
    for (io = mobj.begin (); io != mobj.end (); io++)
        (*io)->update (); 

    // Update mapcharacters
    vector <mapcharacter *>::iterator ic; 
    for (ic = mapchar.begin (); ic != mapchar.end (); ic++)
        (*ic)->update ();  
}

s_int8 landmap::get (igzstream& file) 
{
    u_int16 i, j;
    string tstr;
    
    clear ();

    // Load mapobjects
    i << file;
    for (; i; i--) 
    {
        mapobject * tobj = new mapobject; 
        tstr << file; 
        tobj->load (tstr);
        insert_mapobject (tobj, nbr_of_mapobjects (), tstr.c_str ()); 
    }

    // Load submaps
    i << file; 
    for (j = 0; j < i; j++) 
    { 
        insert_submap (nbr_of_submaps ());
        mapsquare_area * sm = submap[j]; 
        
        u_int16 k, l;
        k << file;
        l << file;
        sm->resize_area (k, l);

        for (l = 0; l < sm->area_height (); l++)
            for (k = 0; k < sm->area_length (); k++) 
            {
                u_int16 n, t; 
                // Read the number of mapobjects which have their base tile here
                n << file;
                while (n) 
                {
                    // Get the mapobject number
                    t << file;
                    sm->put_mapobject (k, l, mobj[t]);
                    n--; 
                }
            }         
    }
    
    return 0; 
}

s_int8 landmap::load (string fname) 
{
    igzstream file;
    s_int8 retvalue = -1;
    string fdef (MAPS_DIR);

    fdef += fname;
    file.open (fdef);
    if (!file.is_open ())
        return -1;
    if (fileops::get_version (file, 1, 1, fdef))
        retvalue = get (file);
    file.close ();
    filename_ = fname;
    return retvalue;
}

s_int8 landmap::put (ogzstream& file) const
{
    u_int16 i;

    fileops::put_version (file, 1);
    
    // Save mapobjects
    nbr_of_mapobjects () >> file; 
    for (i = 0; i < nbr_of_mapobjects (); i++) 
    {
        mobjsrc[i] >> file; 
    }

    // Save submaps
    nbr_of_submaps () >> file;
    for (i = 0; i < nbr_of_submaps (); i++) 
    {
        u_int16 k, l;
        
        submap[i]->area_length () >> file;
        submap[i]->area_height () >> file; 
        
        for (l = 0; l < submap[i]->area_height (); l++)
            for (k = 0; k < submap[i]->area_length (); k++) 
            {
                u_int16 nbr_base = 0; 
                list <mapsquare_tile>::iterator it = submap[i]->area[k][l].tiles.begin ();
                while (it != submap[i]->area[k][l].tiles.end ())
                { 
                    if (it->is_base) nbr_base++;
                    it++;
                }
                nbr_base >> file;
                
                it = submap[i]->area[k][l].tiles.begin (); 
                while (it != submap[i]->area[k][l].tiles.end ())
                { 
                    if (it->is_base) 
                    {
                        u_int16 y = 0;
                        while (mobj[y] != (*it).mapobj) y++;
                        y >> file;
                    }
                    it++; 
                }
            }    
    }
    return 0;      
}

s_int8 landmap::save (string fname)
{
    ogzstream file (MAPS_DIR + fname);
    u_int8 retvalue;

    if (!file.is_open ())
        return -1;
    retvalue = put (file);
    file.close (); 
    filename_ = fname;
    return retvalue;
}

s_int8 landmap::get_state (igzstream& file)
{
    return event_list::load (file); 
}

s_int8 landmap::put_state (ogzstream& file) const
{
    event_list::save (file); 
    return 0; 
}

s_int8 landmap::insert_mapobject (mapobject * an, u_int16 pos,
                                  string srcfile = "")
{     
    if (pos > nbr_of_mapobjects ())
        return -2;

    vector <mapobject *>::iterator i = mobj.begin (); 
    vector <string>::iterator j = mobjsrc.begin (); 

    while (pos--)
    {
        i++;
        j++; 
    }
    
    mobj.insert (i, an);
    mobjsrc.insert (j, srcfile); 
    
    return 0; 
}

s_int8 landmap::delete_mapobject (u_int16 pos)
{
    mapobject * dptr = mobj[pos]; 
    
    // Update all the submaps to delete any mapsquare_tile that points
    // to the deleted object.
    u_int16 k; 
    for (k = 0; k < nbr_of_submaps (); k++)
    {
        u_int16 i, j; 
        for (i = 0; i < submap[k]->area_length (); i++)
            for (j = 0; j < submap[k]->area_height (); j++)
            {
                mapsquare & ms = submap[k]->area[i][j]; 
                list <mapsquare_tile>::iterator imt; 
                for (imt = ms.tiles.begin (); imt != ms.tiles.end (); imt++)
                    if (imt->mapobj == dptr) 
                    {
                        remove_mapobject (k, i, j, pos); 
                        
                        // The iterator is invalidated by the delete operation
                        imt = ms.tiles.begin (); 
                    }
            }
    }
    
    vector <mapobject *>::iterator i; 
    
    if (pos > nbr_of_mapobjects () - 1)
        return -2;

    i = mobj.begin ();
    while (pos--) i++;

    delete (*i);
    mobj.erase (i); 
     
    return 0; 
}

s_int8 landmap::insert_submap (u_int16 pos) 
{
    if (pos > nbr_of_mapobjects ())
        return -2;

    // Update the mapcharacters so they are on the same map as before.
    vector <mapcharacter *>::iterator ic;
    for (ic = mapchar.begin (); ic != mapchar.end (); ic++) 
        if ((*ic)->submap_ >= pos) (*ic)->submap_++; 

    // Insert the submap
    vector <mapsquare_area *>::iterator i = submap.begin ();
    while (pos--) i++; 
    
    mapsquare_area * t = new mapsquare_area;
    submap.insert (i, t);
    
    return 0; 
}

s_int8 landmap::delete_submap (u_int16 pos)
{
    // Update the mapcharacters so they are on the same map as before
    // and remove those who were on the deleted map.
    vector <mapcharacter *>::iterator ic;
    for (ic = mapchar.begin (); ic != mapchar.end (); ic++) 
    { 
        if ((*ic)->submap_ > pos) (*ic)->submap_--; 
        else if ((*ic)->submap_ == pos)
            (*ic)->remove_from_map ();
    }
    
    // Suppress the submap
    vector <mapsquare_area *>::iterator i; 
    
    if (pos > nbr_of_submaps () - 1)
        return -2;
    
    i = submap.begin ();
    while (pos--) i++;
    
    delete (*i);
    submap.erase (i); 
    
    return 0; 
}

s_int8 landmap::put_mapobject (u_int16 smap, u_int16 px, u_int16 py,
                               u_int16 mobjnbr)
{
    return submap[smap]->put_mapobject (px, py, mobj[mobjnbr]); 
}

void landmap::remove_mapobject (u_int16 smap, u_int16 px, u_int16 py, 
                                u_int16 mobjnbr)
{
    submap[smap]->remove_mapobject (px, py, mobj[mobjnbr]);
}
