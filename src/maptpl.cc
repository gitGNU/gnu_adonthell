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

#include "maptpl.h"
#include "mapsquares.h"

mapsquaretpl::mapsquaretpl ()
{
    walkable = ALL_WALKABLE;
}

s_int8 mapsquaretpl::get (igzstream& file)
{
    walkable << file; 
    return 0;
}

s_int8 mapsquaretpl::put (ogzstream& file)
{
    walkable >> file; 
    return 0;
}

maptpl::maptpl (u_int16 x, u_int16 y, u_int16 l, u_int16 h,
                u_int16 d_l, u_int16 d_h):mapselect (x, y, l, h, d_l, d_h)
{
    u_int16 i;

    placetpl = new (mapsquaretpl *)[length ()];
    for (i = 0; i < length (); i++)
        placetpl[i] = new mapsquaretpl[h];
#ifdef _EDIT_
    u_int16 j; 
    selimg = new image (MAPSQUARE_SIZE, MAPSQUARE_SIZE);
    selbaseimg = new image (MAPSQUARE_SIZE, MAPSQUARE_SIZE);
    for (i = 0; i < selimg->length (); i++)
        for (j = 0; j < selimg->height (); j++)
        {
            selimg->put_pix (i, j, 0x0000FF);
            selbaseimg->put_pix (i, j, 0x00FF00);
        }
    selimg->set_alpha (110);
    selbaseimg->set_alpha (110);
#endif
    basex = 0;
    basey = 0;
}

maptpl::~maptpl ()
{
    u_int16 i;

    for (i = 0; i < length (); i++)
        delete[]placetpl[i];
    delete[]placetpl;
#ifdef _EDIT_
    delete selimg;
    delete selbaseimg;
#endif
}

maptpl & maptpl::operator = (const maptpl & mt)
{
    return *this;
}

void maptpl::resize (u_int16 l, u_int16 h)
{
    u_int16 i, j;
    mapsquaretpl **oldplacetpl = placetpl;

    placetpl = new (mapsquaretpl *)[l];
    for (i = 0; i < l; i++)
        placetpl[i] = new mapsquaretpl[h];
    for (i = 0; i < l && i < length (); i++)
        for (j = 0; j < h && j < height (); j++)
            placetpl[i][j] = oldplacetpl[i][j];
    for (i = 0; i < length (); i++)
        delete[]oldplacetpl[i];
    delete[]oldplacetpl;
    mapselect::resize (l, h);
}

s_int8 maptpl::get (igzstream& file)
{
    u_int16 i, j;

    for (i = 0; i < length (); i++)
        delete[]placetpl[i];
    delete[]placetpl;
    length_ << file;
    height_ << file;

    placetpl = new (mapsquaretpl *)[length ()];
    u_int16 h = height ();
    for (i = 0; i < length (); i++)
    {
        placetpl[i] = new mapsquaretpl[h];
        for (j = 0; j < height (); j++)
            placetpl[i][j].get (file);
    }
    basex << file;
    basey << file; 
    return 0;
}

s_int8 maptpl::put (ogzstream& file)
{
    u_int16 i, j;

    length_ >> file;
    height_ >> file;
    for (i = 0; i < length (); i++)
        for (j = 0; j < height (); j++)
            placetpl[i][j].put (file);
    basex >> file;
    basey >> file; 
    return 0;
}

void maptpl::set_base_tile (u_int16 x, u_int16 y)
{
    basex = x;
    basey = y;
}

void maptpl::toggle_walkable ()
{
    placetpl[posx][posy].walkable =
        placetpl[posx][posy].walkable ? NONE_WALKABLE : ALL_WALKABLE;
}

void maptpl::draw_walkables ()
{
    u_int16 i, j;

    for (i = d_posx; (i < d_posx + dl) && (i < length ()); i++)
        for (j = d_posy; (j < d_posy + dh) && (j < height ()); j++)
        {
            const u_int32 col = 0x0ff000;

            if (!placetpl[i][j].is_walkable_west ())
                screen::display.fillrect (s_posx + ((i - d_posx) * MAPSQUARE_SIZE + 1),
                                 s_posy + ((j - d_posy) * MAPSQUARE_SIZE + 1),
                                 1, MAPSQUARE_SIZE - 2, col);

            if (!placetpl[i][j].is_walkable_east ())
                screen::display.fillrect (s_posx + ((i - d_posx) * MAPSQUARE_SIZE) +
                                 MAPSQUARE_SIZE - 1,
                                 s_posy + ((j - d_posy) * MAPSQUARE_SIZE + 1), 1,
                                 MAPSQUARE_SIZE - 2, col);

            if (!placetpl[i][j].is_walkable_north ())
                screen::display.fillrect (s_posx + ((i - d_posx) * MAPSQUARE_SIZE + 1),
                                 s_posy + ((j - d_posy) * MAPSQUARE_SIZE + 1),
                                 MAPSQUARE_SIZE - 2, 1, col);

            if (!placetpl[i][j].is_walkable_south ())
                screen::display.fillrect (s_posx + ((i - d_posx) * MAPSQUARE_SIZE) + 1,
                                 s_posy + ((j - d_posy) * MAPSQUARE_SIZE) +
                                 MAPSQUARE_SIZE - 1, MAPSQUARE_SIZE - 2, 1, col);

        }
}

void maptpl::draw_base_tile ()
{
#ifdef _EDIT_
    if ((basex >= d_posx && basex < d_posx + dl) &&
        (basey >= d_posy && basey < d_posy + dh))
        selbaseimg->draw (s_posx + (basex - d_posx) * MAPSQUARE_SIZE,
                          s_posy + (basey - d_posy) * MAPSQUARE_SIZE);
#endif
}

void maptpl::draw_base_tile (u_int16 x, u_int16 y, drawing_area * da_opt =
                             NULL)
{
#ifdef _EDIT_
    selbaseimg->draw (x, y, da_opt);
#endif
}

void maptpl::draw ()
{
    mapselect::draw ();
    draw_base_tile ();
    draw_walkables ();
}
