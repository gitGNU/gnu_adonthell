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



#include <iostream>
#include "mapobject.h"
#include "fileops.h"
#include <strstream>

void mapobject::init ()
{
    length_ = height_ = 0;
}

void mapobject::clear ()
{
    u_int16 i;

    for (i = 0; i < nbr_of_animations (); i++)
        delete anim[i];

    anim.clear ();
    init ();
}

mapobject::mapobject ():maptpl (0, 0, 1, 1, 9, 9)
{
    init ();
}

mapobject::~mapobject ()
{
    clear ();
}

void mapobject::play ()
{
    u_int16 i;

    for (i = 0; i < nbr_of_animations (); i++)
        anim[i]->play ();
}

void mapobject::stop ()
{
    u_int16 i;

    for (i = 0; i < nbr_of_animations (); i++)
        anim[i]->stop ();
}

void mapobject::rewind ()
{
    u_int16 i;

    for (i = 0; i < nbr_of_animations (); i++)
        anim[i]->rewind ();
}

mapobject & mapobject::operator = (const mapobject & mo)
{
    u_int16 i, j;

    posx = mo.posx;
    posy = mo.posy;
    length_ = mo.length_;
    height_ = mo.height_;
    basex = mo.basex;
    basey = mo.basey;
    for (i = 0; i < maptpl::length (); i++)
        delete[]placetpl[i];
    delete[]placetpl;
    maptpl::set_length (mo.maptpl::length_);
    maptpl::set_height (mo.maptpl::height_);
    placetpl = new (mapsquaretpl *)[maptpl::length ()];
    for (i = 0; i < maptpl::length (); i++)
    {
        placetpl[i] = new mapsquaretpl[maptpl::height ()];
        for (j = 0; j < maptpl::height (); j++)
            placetpl[i][j].walkable = mo.placetpl[i][j].walkable;
    }
    for (i = 0; i < nbr_of_animations (); i++)
        delete anim[i];

    anim.clear ();
    for (i = 0; i < mo.anim.size (); i++)
        anim.push_back (NULL);
    for (i = 0; i < nbr_of_animations (); i++)
    {
        anim[i] = new animation;
        *(anim[i]) = *(mo.anim[i]);
    }
    return *this;
}

void mapobject::update ()
{
    static u_int16 i;

    for (i = 0; i < nbr_of_animations (); i++)
        anim[i]->update ();
}

void mapobject::draw_free (s_int16 x, s_int16 y, drawing_area * da_opt = NULL, surface * target = NULL)
{
    static u_int16 i;

    for (i = 0; i < nbr_of_animations (); i++)
        anim[i]->draw (x, y, da_opt, target);
}

void mapobject::draw (s_int16 x, s_int16 y, drawing_area * da_opt = NULL, surface * target = NULL)
{
    draw_free (x - basex * MAPSQUARE_SIZE, y - basey * MAPSQUARE_SIZE,
               da_opt, target);
}

void mapobject::draw_border_free (u_int16 x, u_int16 y,
                                  drawing_area * da_opt = NULL)
{
    screen::display.fillrect (x, y, length (), 1, 0xFFFFFF, da_opt);
    screen::display.fillrect (x, y + height (), length (), 1, 0xFFFFFF, da_opt);
    screen::display.fillrect (x + length (), y, 1, height () + 1, 0xFFFFFF, da_opt);
    screen::display.fillrect (x, y, 1, height () + 1, 0xFFFFFF, da_opt);
}

void mapobject::draw_border (u_int16 x, u_int16 y, drawing_area * da_opt =
                             NULL)
{
    draw_border_free (x - basex * MAPSQUARE_SIZE, y - basey * MAPSQUARE_SIZE,
                      da_opt);
}

void mapobject::calculate_dimensions ()
{
    u_int16 i;

    length_ = 0;
    height_ = 0;
    for (i = 0; i < nbr_of_animations (); i++)
    {
        u_int16 tl, th;

        if ((tl = anim[i]->length () + anim[i]->xoffset ()) > length ())
            length_ = tl;

        if ((th = anim[i]->height () + anim[i]->yoffset ()) > height ())
            height_ = th;
    }
}

s_int8 mapobject::get (igzstream & file)
{
    u_int16 i;
    u_int16 nbr_of_parts;

    anim.clear ();
    nbr_of_parts << file;
    for (i = 0; i < nbr_of_parts; i++)
    {
        anim.push_back (new animation);
        anim.back ()->get (file);
        anim.back ()->play ();
    }
    maptpl::get (file);
    resize_view (9, 9);
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
    //   gzclose(file);
    file.close ();
    return retvalue;
}

s_int8 mapobject::insert_animation (animation * an, u_int16 pos)
{
    vector < animation * >::iterator i;
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
    vector < animation * >::iterator i;

    if (pos > nbr_of_animations () - 1)
        return -2;
    i = anim.begin ();
    while (pos--)
        i++;
    anim.erase (i);
    return 0;
}

