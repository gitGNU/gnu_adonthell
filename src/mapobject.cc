/*
   Copyright (C) 1999/2000/2001   Alexandre Courbot
   Part of the Adonthell Project <http://adonthell.nongnu.org>

   Adonthell is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Adonthell is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Adonthell.  If not, see <http://www.gnu.org/licenses/>.
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


mapobject::mapobject () : mapsquare_walkable_area ()
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
    mapsquare_walkable_area::clear (); 
}   

bool mapobject::update ()
{
    vector <animation *>::iterator i; 
    
    for (i = anim.begin (); i != anim.end (); i++)
        (*i)->update ();

    return true; 
}

void mapobject::draw (s_int16 x, s_int16 y, const drawing_area * da_opt, surface * target) const
{
    vector <animation *>::iterator i; 
    
    for (i = anim.begin (); i != anim.end (); i++)
        (*i)->draw (x, y, da_opt, target);
}

void mapobject::draw_from_base (s_int16 x, s_int16 y,
                                const drawing_area * da_opt, surface * target) const
{
    draw (x - base_x () * MAPSQUARE_SIZE, y - base_y () * MAPSQUARE_SIZE,
          da_opt, target);
}

s_int8 mapobject::get (igzstream & file)
{
    u_int16 i;
    u_int16 nbr_of_parts;
    
    if (!fileops::get_version (file, 1, 1, ""))
        return -1;
    
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

    mapsquare_walkable_area::get (file); 
    
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
    retvalue = get (file);
    file.close ();
    return retvalue;
}

s_int8 mapobject::put (ogzstream & file) const
{
    u_int16 i;
    
    fileops::put_version (file, 1); 

    // Write all the animations.
    nbr_of_animations () >> file;
    for (i = 0; i < nbr_of_animations (); i++)
    {
        anim[i]->put (file);
    }

    mapsquare_walkable_area::put (file); 
    
    return 0;
}

s_int8 mapobject::save (string fname) const
{
    ogzstream file;
    s_int8 retvalue = -1;

    string fdef = MAPOBJECTS_DIR;

    fdef += fname;

    file.open (fdef);
    if (!file.is_open ())
        return -1;
    retvalue = put (file);
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
    // Clear everything.
    clear (); 

    // Copy the area.
    (mapsquare_walkable_area&) (*this) = (mapsquare_walkable_area&) src; 

    // Copy all animations.
    vector <animation *>::iterator it;
    for (it = ((mapobject&) src).anim.begin (); it != ((mapobject&) src).anim.end (); it++)
    {
        animation * an = new animation;
        *an = *(*it);
        insert_animation (an, nbr_of_animations ()); 
    }
    
    return *this;
}
