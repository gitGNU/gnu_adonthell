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


mapview::mapview () : da () 
{
    drawable::set_length (0);
    drawable::set_height (0); 
    d_length = d_height = currentsubmap_ = posx_ = posy_ = 0;
    m_map = NULL;
    offx_ = offy_ = 0;
    locals = PyDict_New ();
    PyObject *myself = python::pass_instance (this, "mapview"); 
    PyDict_SetItemString (locals, "myself", myself);
    Py_DECREF (myself); 
}

mapview::~mapview ()
{
    detach_map (); 
    Py_DECREF (locals);
}

void mapview::attach_map (landmap * m)
{
    m_map = m;

    PyObject * mymap = python::pass_instance (m_map, "landmap"); 
    PyDict_SetItemString (locals, "mymap", mymap);
    Py_DECREF (mymap); 

    set_pos (0, 0, 0);
}

void mapview::detach_map ()
{
    if (!m_map) return;
    
    m_map = NULL;
    PyDict_DelItemString (locals, "mymap");
}

s_int8 mapview::set_pos (u_int16 sm, u_int16 px, u_int16 py, s_int16 ox = 0, s_int16 oy = 0)
{
    currentsubmap_ = sm;
    mapsquare_area * ms = m_map->submap[sm]; 
    
    s_int32 tpx = px * MAPSQUARE_SIZE + ox;
    s_int32 tpy = py * MAPSQUARE_SIZE + oy;
    
    if (tpx + length () > ms->area_length () * MAPSQUARE_SIZE)
        tpx = ms->area_length () * MAPSQUARE_SIZE - length (); 
    if (tpy + height () > ms->area_height () * MAPSQUARE_SIZE)
        tpy = ms->area_height () * MAPSQUARE_SIZE - height (); 

    if (tpx < 0) tpx = 0;
    if (tpy < 0) tpy = 0; 
    
    px = tpx / MAPSQUARE_SIZE;
    py = tpy / MAPSQUARE_SIZE;

    ox = tpx % MAPSQUARE_SIZE;
    oy = tpy % MAPSQUARE_SIZE;
    
    posx_ = px;
    posy_ = py;
    offx_ = ox;
    offy_ = oy;
    
    return 0;
}

s_int8 mapview::center_on (u_int16 sm, u_int16 px, u_int16 py, s_int16 ox =
                           0, s_int16 oy = 0)
{
    s_int32 tpx = px * MAPSQUARE_SIZE + ox - (length () - MAPSQUARE_SIZE >> 1); 
    s_int32 tpy = py * MAPSQUARE_SIZE + oy - (height () - MAPSQUARE_SIZE >> 1); 

    if (tpx < 0) tpx = 0; 
    if (tpy < 0) tpy = 0; 
    
    s_int16 npx = tpx / MAPSQUARE_SIZE; 
    s_int16 npy = tpy / MAPSQUARE_SIZE;
    
    s_int16 nox = tpx % MAPSQUARE_SIZE;
    s_int16 noy = tpy % MAPSQUARE_SIZE;

    return set_pos (sm, npx, npy, nox, noy);     
}

void mapview::scroll_right ()
{
    if (!can_scroll_right ())
        return;
    if (offx_ == MAPSQUARE_SIZE - 1)
    {
        offx_ = 0;
        posx_++;
    }
    else
        offx_++;
}

void mapview::scroll_left ()
{
    if (!can_scroll_left ())
        return;
    if (offx_ == 0)
    {
        offx_ = MAPSQUARE_SIZE - 1;
        posx_--;
    }
    else
        offx_--;
}

void mapview::scroll_down ()
{
    if (!can_scroll_down ())
        return;
    if (offy_ == MAPSQUARE_SIZE - 1)
    {
        offy_ = 0;
        posy_++;
    }
    else
        offy_++;
}

void mapview::scroll_up ()
{
    if (!can_scroll_up ())
        return;
    if (offy_ == 0)
    {
        offy_ = MAPSQUARE_SIZE - 1;
        posy_--;
    }
    else
        offy_--;
}

void mapview::resize (u_int16 l, u_int16 h)
{
    drawable::set_length (l);
    drawable::set_height (h); 
    d_length = (l / MAPSQUARE_SIZE) + (l % MAPSQUARE_SIZE != 0);
    d_height = (h / MAPSQUARE_SIZE) + (h % MAPSQUARE_SIZE != 0);
    da.resize (length (), height ());
}

s_int8 mapview::get_state (igzstream& file)
{
    // Read the mapview's schedule
    string t;
    t << file;
    set_schedule (t);

    u_int16 a, b, c, d, sm;

    // Read the mapview's dimensions
    // Length and height
    a << file;
    b << file; 
    resize (a, b);

    // Currentsubmap
    sm << file;
    
    // Position on map
    a << file;
    b << file;
    c << file;
    d << file; 
    set_pos (sm, a, b, c, d);

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
    currentsubmap_ >> file; 

    // Position
    posx_ >> file;
    posy_ >> file;
    offx_ >> file;
    offy_ >> file; 
    
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
    static u_int16 offx, offy; 
    
    if (!m_map)
        return;

    static SDL_Rect trect; 
    static drawing_area tda;

    da.move (x, y);
    if (da_opt) da.assign_drawing_area (da_opt);
    
    trect = da.setup_rects ();
    tda = trect;
    
    l = m_map->submap[currentsubmap_];
    if (!l->area_length () || !l->area_height ())
        return;

    i0 = posx_;
    j0 = posy_;
    ie = i0 + d_length + (offx_ != 0) < l->area_length () ? i0 + d_length + (offx_ !=
                                                                    0) : l->area_length ();
    je = j0 + d_height + (offy_ != 0) < l->area_height () ? j0 + d_height + (offy_ !=
                                                                    0) : l->area_height ();

    offx = l->area_length () * MAPSQUARE_SIZE < length () ?
        (length () - l->area_length () * MAPSQUARE_SIZE) >> 1 : 0; 

    offy = l->area_height () * MAPSQUARE_SIZE < height () ?
        (height () - l->area_height () * MAPSQUARE_SIZE) >> 1 : 0; 

    x += offx;
    y += offy; 
    
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
                    draw_tile (x, y, &tda, target, it);
                    it++;
                }
                else
                {
                    draw_mapchar (x, y, &tda, target, itc); 
                    itc++;
                }
            }
            else
            {
                draw_mapchar (x, y, &tda, target, itc); 
                itc++;
            }
        }
        else
        {
            draw_tile (x, y, &tda, target, it);
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
                draw_tile (x, y, &tda, target, it); 

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
                draw_tile (x, y, &tda, target, it);
                
            for (itc = l->area[i][j].mapchars.begin ();
                 itc != l->area[i][j].mapchars.end (); itc++)
                if (*itc == *(itc->base_tile))
                    characters_draw.push_back (*itc);
        }
        
        // Right overflow
        for (it = l->area[ie - 1][j].tiles.begin ();
             it != l->area[ie - 1][j].tiles.end (); it++)
            if (it->y == it->base_tile->y && it->x < it->base_tile->x)
                draw_tile (x, y, &tda, target, it); 

        for (itc = l->area[ie - 1][j].mapchars.begin ();
             itc != l->area[ie - 1][j].mapchars.end (); itc++)
            if (itc->y == itc->base_tile->y && itc->x < itc->base_tile->x)
                characters_draw.push_back (*itc);

        // Drawing characters
        for (itc = characters_draw.begin (); itc != characters_draw.end ();
             itc++)
            draw_mapchar (x, y, &tda, target, itc); 
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
                    draw_tile (x, y, &tda, target, it);
                    it++;
                }
                else
                {
                    draw_mapchar (x, y, &tda, target, itc); 
                    itc++;
                }
            }
            else
            {
                draw_mapchar (x, y, &tda, target, itc); 
                itc++;
            }
        }
        else
        {
            draw_tile (x, y, &tda, target, it);
            it++;
        }
    }
    critical_draw.clear ();
    characters_draw.clear ();

    if (da_opt) da.detach_drawing_area ();  
}



// Private methods


void mapview::draw_tile (s_int16 x, s_int16 y, const drawing_area * da_opt,
                         surface * target, list<mapsquare_tile>::iterator it) const
{ 
    it->mapobj->draw_from_base
        ((it->base_tile->x - posx_) * MAPSQUARE_SIZE - offx_ + x,
         (it->base_tile->y - posy_) * MAPSQUARE_SIZE - offy_ + y,
         da_opt, target);
}

void mapview::draw_mapchar (s_int16 x, s_int16 y, const drawing_area * da_opt,
                            surface * target, list<mapsquare_char>::iterator itc) const
{ 
    u_int16 xdraw =
        ((itc->mchar->posx () - posx_ - itc->mchar->base_x ()) * MAPSQUARE_SIZE)
        + itc->mchar->offx () - offx_ + x;
    
    u_int16 ydraw =
        ((itc->mchar->posy () - posy_ - itc->mchar->base_y ()) * MAPSQUARE_SIZE)
        + itc->mchar->offy () - offy_ + y;
    
    itc->mchar->draw (xdraw, ydraw, da_opt, target);
}
