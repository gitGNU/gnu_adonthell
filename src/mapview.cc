/*
   $Id$

   Copyright (C) 1999/2000/2001    Alexandre Courbot
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/


/**
 * @file   mapview.cc
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * 
 * @brief  Defines the mapview class.
 * 
 * 
 */



#include "mapview.h"
#include "landmap.h"


void mapview::init ()
{
    drawable::set_length (0);
    drawable::set_height (0); 
    d_length = d_height = currentsubmap = posx = posy = 0;
    m_map = NULL;
    offx = offy = draw_offx = draw_offy = 0;
    da = new drawing_area ();
    locals = PyDict_New ();
    PyDict_SetItemString (locals, "myself", pass_instance (this, "mapview"));
}

mapview::mapview ()
{
    init ();
}

mapview::~mapview ()
{
    delete da;
    
    Py_DECREF (locals);
}

void mapview::attach_map (landmap * m)
{
    m_map = m;
    currentsubmap = 0;
    PyDict_SetItemString (locals, "mymap", pass_instance (m_map, "landmap"));
    if (m_map->nbr_of_submaps ())
    {
        set_pos (0, 0);
    }
}

void mapview::detach_map ()
{
    m_map = NULL;
    PyDict_DelItemString (locals, "mymap");
}

s_int8 mapview::set_current_submap (u_int16 sm)
{
    if (sm >= m_map->nbr_of_submaps ())
        return -1;
    currentsubmap = sm;
    return 0;
}

s_int8 mapview::set_pos (u_int16 px, u_int16 py, s_int16 ox = 0, s_int16 oy =
                         0)
{
    if (!m_map->nbr_of_submaps ())
        return -1;
    px += ox / MAPSQUARE_SIZE;
    ox %= MAPSQUARE_SIZE;
    py += oy / MAPSQUARE_SIZE;
    oy %= MAPSQUARE_SIZE;
    if (px >= m_map->submap[currentsubmap]->area_length () ||
        py >= m_map->submap[currentsubmap]->area_height ())
        return -1;

    if (ox < 0)
    {
        px--;
        ox = MAPSQUARE_SIZE + ox;
    }
    if (oy < 0)
    {
        py--;
        oy = MAPSQUARE_SIZE + oy;
    }

    posx = px;
    posy = py;
    offx = ox;
    offy = oy;
    return 0;
}

s_int8 mapview::center_on (u_int16 px, u_int16 py, s_int16 ox =
                           0, s_int16 oy = 0)
{
    s_int32 npx = px - (d_length >> 1);
    s_int32 npy = py - (d_height >> 1);

    if (!(d_length % 2))
        ox += MAPSQUARE_SIZE / 2;
    if (!(d_height % 2))
        oy += MAPSQUARE_SIZE / 2;

    if (ox < 0)
    {
        npx--;
        ox = MAPSQUARE_SIZE + ox;
    }
    if (oy < 0)
    {
        npy--;
        oy = MAPSQUARE_SIZE + oy;
    }
    if (npx < 0 || (!npx && ox < 0))
    {
        npx = 0;
        ox = 0;
    }
    if (npy < 0 || (!npy && oy < 0))
    {
        npy = 0;
        oy = 0;
    }

    if (npx > m_map->submap[currentsubmap]->area_length () - d_length ||
        (npx == m_map->submap[currentsubmap]->area_length () - d_length && ox))
    {
        npx = m_map->submap[currentsubmap]->area_length () - d_length;
        ox = 0;
    }
    if (npy > m_map->submap[currentsubmap]->area_height () - d_height ||
        (npy == m_map->submap[currentsubmap]->area_height () - d_height && oy))
    {
        npy = m_map->submap[currentsubmap]->area_height () - d_height;
        oy = 0;
    }

    return set_pos (npx, npy, ox, oy);
}

void mapview::scroll_right ()
{
    if (!can_scroll_right ())
        return;
    if (offx == MAPSQUARE_SIZE - 1)
    {
        offx = 0;
        posx++;
    }
    else
        offx++;
}

void mapview::scroll_left ()
{
    if (!can_scroll_left ())
        return;
    if (offx == 0)
    {
        offx = MAPSQUARE_SIZE - 1;
        posx--;
    }
    else
        offx--;
}

void mapview::scroll_down ()
{
    if (!can_scroll_down ())
        return;
    if (offy == MAPSQUARE_SIZE - 1)
    {
        offy = 0;
        posy++;
    }
    else
        offy++;
}

void mapview::scroll_up ()
{
    if (!can_scroll_up ())
        return;
    if (offy == 0)
    {
        offy = MAPSQUARE_SIZE - 1;
        posy--;
    }
    else
        offy--;
}

void mapview::resize (u_int16 l, u_int16 h)
{
    drawable::set_length (l);
    drawable::set_height (h); 
    draw_offx = (l % MAPSQUARE_SIZE);
    draw_offy = (h % MAPSQUARE_SIZE);
    d_length = (l / MAPSQUARE_SIZE) + (l % MAPSQUARE_SIZE != 0);
    d_height = (h / MAPSQUARE_SIZE) + (h % MAPSQUARE_SIZE != 0);
    da->resize (drawable::length (), drawable::height ());
}

s_int8 mapview::get_state (igzstream& file)
{
    // Read the mapview's schedule
    string t;
    t << file;
    set_schedule (t);

    u_int16 a, b, c, d;

    // Read the mapview's dimensions
    // Length and height
    a << file;
    b << file; 
    resize (a, b);

    // Currentsubmap
    a << file; 
    set_current_submap (a);

    // Screen position (FIXME: obsolete!)
    a << file;
    b << file; 

    // Position on map
    a << file;
    b << file;
    c << file;
    d << file; 
    set_pos (a, b, c, d);

    return 0;
}

s_int8 mapview::put_state (ogzstream& file)
{
    u_int16 b;

    // Write the mapview's schedule
    schedule_file_ >> file; 

    // Write the mapview's dimensions
    b = length (); 
    b >> file;
    b = height (); 
    b >> file; 
    currentsubmap >> file; 

    // FIXME: Obsolete x and y members - Fire them!
    b >> file;
    b >> file;

    // Position
    posx >> file;
    posy >> file;
    offx >> file;
    offy >> file; 
    
    return 0;
}

void mapview::set_schedule (string file)
{
    schedule.set_locals (locals); 
    if (file == "") schedule.set_script (file);
    else schedule.set_script ("scripts/schedules/" + file + ".py");
    schedule_file_ = file;      
}

void mapview::update ()
{
    schedule.run (); 
}

void mapview::draw (s_int16 x, s_int16 y, const drawing_area * da_opt = NULL,
                    surface *target = NULL) const
{
    static u_int16 i, j;
    static u_int16 i0, j0, ie, je;
    static list <mapsquare_tile>::iterator it;
    static list <mapsquare_char>::iterator itc;
    static mapsquare_area *l;

    if (!m_map)
        return;

    da->move (x, y);

    l = m_map->submap[currentsubmap];
    if (!l->area_length () || !l->area_height ())
        return;

    i0 = posx;
    j0 = posy;
    ie = i0 + d_length + (offx != 0) < l->area_length () ? i0 + d_length + (offx !=
                                                                    0) : l->area_length ();
    je = j0 + d_height + (offy != 0) < l->area_height () ? j0 + d_height + (offy !=
                                                                    0) : l->area_height ();

    // 1st horizontal parse to check top overflows
    // Top-left corner
    for (it = l->area[i0][j0].tiles.begin ();
         it != l->area[i0][j0].tiles.end () && *(it->base_tile) < *it; it++)
        if (it->x > it->base_tile->x && it->y > it->base_tile->y)
            critical_draw.push_back (*(it->base_tile));

    for (itc = l->area[i0][j0].mapchars.begin ();
         itc != l->area[i0][j0].mapchars.end (); itc++)
        if (itc->x > itc->base_tile->x && itc->y > itc->base_tile->y)
            characters_draw.push_back (*itc);

    // Top line
    for (i = i0; i < ie && i < l->area_length (); i++)
    {
        for (it = l->area[i][j0].tiles.begin ();
             it != l->area[i][j0].tiles.end (); it++)
            if (it->x == it->base_tile->x && it->y > it->base_tile->y)
                critical_draw.push_back (*(it->base_tile));

        for (itc = l->area[i][j0].mapchars.begin ();
             itc != l->area[i][j0].mapchars.end (); itc++)
            if (itc->x == itc->base_tile->x && itc->y > itc->base_tile->y)
                characters_draw.push_back (*itc);
    }

    // Top right corner
    for (it = l->area[ie - 1][j0].tiles.begin ();
         it != l->area[ie - 1][j0].tiles.end (); it++)
        if (it->x < it->base_tile->x && it->y > it->base_tile->y)
            critical_draw.push_back (*(it->base_tile));

    for (itc = l->area[ie - 1][j0].mapchars.begin ();
         itc != l->area[ie - 1][j0].mapchars.end (); itc++)
        if (itc->x < itc->base_tile->x && itc->y > itc->base_tile->y)
            characters_draw.push_back (*itc);

    // Drawing characters and top overflowing gfx
    critical_draw.sort ();
    characters_draw.sort ();

    it = critical_draw.begin ();
    itc = characters_draw.begin ();
    while (itc != characters_draw.end () || it != critical_draw.end ())
    {
        if (itc != characters_draw.end ())
        {
            if (it != critical_draw.end ())
            {
                if (it->base_tile->y <= itc->base_tile->y)
                {
                    draw_tile (x, y, da_opt, target, it);
                    it++;
                }
                else
                {
                    draw_mapchar (x, y, da_opt, target, itc); 
                    itc++;
                }
            }
            else
            {
                draw_mapchar (x, y, da_opt, target, itc); 
                itc++;
            }
        }
        else
        {
            draw_tile (x, y, da_opt, target, it);
            it++;
        }
    }
    critical_draw.clear ();
    characters_draw.clear ();

    // Now drawing objects without any top or bottom overflow
    for (j = j0; j < je; j++)
    {
        // Left overflow
        for (it = l->area[i0][j].tiles.begin ();
             it != l->area[i0][j].tiles.end () && *(it->base_tile) <= *it;
             it++)
            if (it->y == it->base_tile->y && it->x > it->base_tile->x)
                draw_tile (x, y, da_opt, target, it); 

        for (itc = l->area[i0][j].mapchars.begin ();
             itc != l->area[i0][j].mapchars.end (); itc++)
            if (itc->y == itc->base_tile->y && itc->x > itc->base_tile->x)
                characters_draw.push_back (*itc);

        // Objects which base tile is visible on the map view
        for (i = i0; i < ie; i++)
        {
            for (it = l->area[i][j].base_begin;
                 it != l->area[i][j].tiles.end () && *(it->base_tile) < *it;
                 it++);
            for (; it != l->area[i][j].tiles.end () && *(it->base_tile) == *it;
                 it++)
                draw_tile (x, y, da_opt, target, it);
                
            for (itc = l->area[i][j].mapchars.begin ();
                 itc != l->area[i][j].mapchars.end (); itc++)
                if (*itc == *(itc->base_tile))
                    characters_draw.push_back (*itc);
        }
        
        // Right overflow
        for (it = l->area[ie - 1][j].tiles.begin ();
             it != l->area[ie - 1][j].tiles.end (); it++)
            if (it->y == it->base_tile->y && it->x < it->base_tile->x)
                draw_tile (x, y, da_opt, target, it); 

        for (itc = l->area[ie - 1][j].mapchars.begin ();
             itc != l->area[ie - 1][j].mapchars.end (); itc++)
            if (itc->y == itc->base_tile->y && itc->x < itc->base_tile->x)
                characters_draw.push_back (*itc);

        // Drawing characters
        for (itc = characters_draw.begin (); itc != characters_draw.end ();
             itc++)
            draw_mapchar (x, y, da_opt, target, itc); 
        characters_draw.clear ();
    }

    // Last horizontal parse to check bottom overflows
    // Bottom left overflow
    if (!l->area[i0][je - 1].tiles.empty ())
        for (it = --l->area[i0][je - 1].tiles.end ();
             it->y < it->base_tile->y; it--)
        {
            if (it->x > it->base_tile->x && it->y < it->base_tile->y)
                critical_draw.push_front (*(it->base_tile));
            if (it == l->area[i0][je - 1].tiles.begin ())
                break;
        }

    for (itc = l->area[i0][je - 1].mapchars.begin ();
         itc != l->area[i0][je - 1].mapchars.end (); itc++)
        if (itc->x > itc->base_tile->x && itc->y < itc->base_tile->y)
            characters_draw.push_back (*itc);

    // Bottom line
    for (i = i0; i < ie && i < l->area_length (); i++)
    {
        if (!l->area[i][je - 1].tiles.empty ())
            for (it = --l->area[i][je - 1].tiles.end ();
                 it->y < it->base_tile->y; it--)
            {
                if (it->x == it->base_tile->x && it->y < it->base_tile->y)
                    critical_draw.push_front (*(it->base_tile));
                if (it == l->area[i][je - 1].tiles.begin ())
                    break;
            }

        for (itc = l->area[i][je - 1].mapchars.begin ();
             itc != l->area[i][je - 1].mapchars.end (); itc++)
        {
            if (itc->x == itc->base_tile->x && itc->y < itc->base_tile->y)
            {
                characters_draw.push_back (*itc);
            }
        }
    }

    // Bottom right corner
    if (!l->area[ie - 1][je - 1].tiles.empty ())
        for (it = --l->area[ie - 1][je - 1].tiles.end ();
             it->y < it->base_tile->y; it--)
        {
            if (it->x < it->base_tile->x && it->y < it->base_tile->y)
                critical_draw.push_front (*(it->base_tile));
            if (it == l->area[ie - 1][je - 1].tiles.begin ())
                break;
        }

    for (itc = l->area[ie - 1][je - 1].mapchars.begin ();
         itc != l->area[ie - 1][je - 1].mapchars.end (); itc++)
        if (itc->x < itc->base_tile->x && itc->y < itc->base_tile->y)
            characters_draw.push_back (*itc);


    // Drawing characters and bottom overflowing gfx
    critical_draw.sort ();
    characters_draw.sort ();

    it = critical_draw.begin ();
    itc = characters_draw.begin ();
    while (itc != characters_draw.end () || it != critical_draw.end ())
    {
        if (itc != characters_draw.end ())
        {
            if (it != critical_draw.end ())
            {
                if (it->base_tile->y <= itc->base_tile->y)
                {
                    draw_tile (x, y, da_opt, target, it);
                    it++;
                }
                else
                {
                    draw_mapchar (x, y, da_opt, target, itc); 
                    itc++;
                }
            }
            else
            {
                draw_mapchar (x, y, da_opt, target, itc); 
                itc++;
            }
        }
        else
        {
            draw_tile (x, y, da_opt, target, it);
            it++;
        }
    }
    critical_draw.clear ();
    characters_draw.clear ();
}



// Private methods


void mapview::draw_tile (s_int16 x, s_int16 y, const drawing_area * da_opt,
                         surface * target, list<mapsquare_tile>::iterator it) const
{ 
    it->mapobj->draw_from_base
        ((it->base_tile->x - posx) * MAPSQUARE_SIZE - offx + x - draw_offx,
         (it->base_tile->y - posy) * MAPSQUARE_SIZE - offy + y - draw_offy,
         da, target);
}

void mapview::draw_mapchar (s_int16 x, s_int16 y, const drawing_area * da_opt,
                            surface * target, list<mapsquare_char>::iterator itc) const
{ 
    u_int16 xdraw =
        ((itc->mchar->posx () - posx - itc->mchar->base_x ()) * MAPSQUARE_SIZE)
        + itc->mchar->offx () - offx + x - draw_offx;
    
    u_int16 ydraw =
        ((itc->mchar->posy () - posy - itc->mchar->base_y ()) * MAPSQUARE_SIZE)
        + itc->mchar->offy () - offy + y - draw_offy;
    
    itc->mchar->draw (xdraw, ydraw, da, target);
}
