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
 * @file mapobject.cc
 *
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * @brief Defines the mapobject class.
 */


#include "mapobject.h"

using namespace std;  


// Public methods.


mapobject::mapobject () : drawable () 
{
    clear (); 
}
 
mapobject::~mapobject ()
{
    clear ();
}

void mapobject::clear ()
{
    vector <animation *>::iterator i; 
    
    for (i = anim.begin (); i != anim.end (); i++)
        delete (*i);     
    anim.clear ();
    
    area.clear (); 
    
    basex = 0;
    basey = 0; 
}  

void mapobject::resize_area (u_int16 nl, u_int16 nh)
{
    vector <vector<mapsquare_walkable> >::iterator i;

    area.resize (nl);
    for (i = area.begin (); i !=  area.end (); i++)
        i->resize (nh);
    
    set_length (nl);
    set_height (nh); 
}

void mapobject::set_base (u_int16 nx, u_int16 ny)
{
    basex = nx;
    basey = ny; 
} 


void mapobject::update ()
{
    vector <animation *>::iterator i; 
    
    for (i = anim.begin (); i != anim.end (); i++)
        (*i)->update ();
}

void mapobject::draw (s_int16 x, s_int16 y, const drawing_area * da_opt = NULL, surface * target = NULL) const
{
    vector <animation *>::iterator i; 
    
    for (i = anim.begin (); i != anim.end (); i++)
        (*i)->draw (x, y, da_opt, target);
}

void mapobject::draw_from_base (s_int16 x, s_int16 y,
                                const drawing_area * da_opt = NULL, surface * target = NULL) const
{
    draw (x - basex * MAPSQUARE_SIZE, y - basey * MAPSQUARE_SIZE,
          da_opt, target);
}

s_int8 mapobject::get (igzstream & file)
{
    u_int16 i;
    u_int16 nbr_of_parts;
    u_int16 t_length, t_height;
    u_int16 basex_, basey_;
    vector <vector<mapsquare_walkable> >::iterator it;
    vector<mapsquare_walkable>::iterator jt;
    
    // Clear everything.
    clear ();

    // Read all the animations.
    nbr_of_parts << file;
    for (i = 0; i < nbr_of_parts; i++)
    {
        anim.push_back (new animation);
        anim.back ()->get (file);
        anim.back ()->play ();
    }

    // Get the area size.
    t_length << file;
    t_height << file; 
    resize_area (t_length, t_height); 

    // Load the area.
    for (it = area.begin (); it != area.end (); it++)
        for (jt = it->begin (); jt < it->end (); jt++)
            jt->get (file);
    
    // Load the base square information.
    basex_ << file;
    basey_ << file;  
    set_base (basex_, basey_); 

    // Update the mapobject's dimensions.
    calculate_dimensions ();

    return 0;
}

s_int8 mapobject::load (string fname)
{
    igzstream file;
    s_int8 retvalue = -1;

    string fdef = MAPOBJECTS_DIR;

    fdef += fname;

    file.open (fdef);
    if (!file.is_open ())
        return -1;
    if (fileops::get_version (file, 1, 1, fdef))
        retvalue = get (file);
    file.close ();
    return retvalue;
}

s_int8 mapobject::insert_animation (animation * an, u_int16 pos)
{
    vector <animation *>::iterator i;
    if (pos > nbr_of_animations ())
        return -2;
    i = anim.begin ();
    while (pos--)
        i++;
    anim.insert (i, an);
    an->play ();
    return 0;
}

s_int8 mapobject::delete_animation (u_int16 pos)
{
    vector <animation *>::iterator i;

    if (pos > nbr_of_animations () - 1)
        return -2;
    i = anim.begin ();
    while (pos--)
        i++;
    anim.erase (i);
    return 0;
}
  
mapobject & mapobject::operator = (const mapobject & src)
{
    u_int16 i, j;

    // Clear everything.
    clear (); 

    (drawable&) (*this) = (drawable&) src; 

    // Copy all animations.
    vector <animation *>::iterator it;
    for (it = ((mapobject&) src).anim.begin (); it != ((mapobject&) src).anim.end (); it++)
    {
        animation * an = new animation;
        *an = *(*it);
        insert_animation (an, nbr_of_animations ()); 
    }

    // Copy the area.
    for (i = 0; i < src.area_length (); i++)
        for (j = 0; j < src.area_height (); j++)
            area[i][j] = src.area[i][j]; 
    
    // Copy the base square information.
    set_base (src.base_x (), src.base_y ());
    
    return *this;
}



// Private methods.



void mapobject::calculate_dimensions ()
{
    vector <animation *>::iterator i;

    set_length (0);
    set_height (0); 
    for (i = anim.begin (); i != anim.end (); i++)
    {
        u_int16 tl, th;

        if ((tl = (*i)->length () + (*i)->xoffset ()) > length ())
            set_length (tl);

        if ((th = (*i)->height () + (*i)->yoffset ()) > height ())
            set_height (th);
    }
}
