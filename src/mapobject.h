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
 * @file mapobject.h
 *
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * @brief Declares the mapobject class.
 */


#ifndef _MAPOBJECT_H
#define _MAPOBJECT_H

#include <string>
#include <vector>
#include "fileops.h"
#include "animation.h"
#include "maptpl.h"

/// Where does the mapobjects resides in the data tree?
#define MAPOBJECTS_DIR "gfx/mapobjects/"

class mapobject : public maptpl
{
public:
    mapobject ();
    void clear ();
    ~mapobject ();

    u_int16 length ()
    {
        return length_;
    }

    u_int16 height ()
    {
        return height_;
    }

    void play ();
    void stop ();
    void rewind ();

    void update ();

    // FIXME: draw -> draw_from_base, draw_free -> draw
    void draw (s_int16 x, s_int16 y, drawing_area * da_opt = NULL, surface * target = NULL);
    void draw_free (s_int16 x, s_int16 y, drawing_area * da_opt = NULL, surface * target = NULL); 
    void draw_border_free (u_int16 x, u_int16 y, drawing_area * da_opt = NULL);
    void draw_border (u_int16 x, u_int16 y, drawing_area * da_opt = NULL);

    s_int8 get (igzstream & file);
    s_int8 load (string fname);

    u_int16 nbr_of_animations ()
    {
        return anim.size ();
    }
    
    animation *get_animation (u_int16 nbr)
    {
        return anim[nbr];
    }

    s_int8 insert_animation (animation * an, u_int16 pos);
    s_int8 delete_animation (u_int16 pos);
 
    mapobject & operator = (const mapobject & mo);

protected:
    u_int16 length_, height_;
    vector <animation *>anim;

    void init ();

 private:
    void calculate_dimensions ();
};

#endif
