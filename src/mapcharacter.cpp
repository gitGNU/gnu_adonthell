/*
   $Id$

   Copyright (C) 1999   The Adonthell Project
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "types.h"
#include "image.h"
#include "mapsquare.h"
#include "mapevent.h"
#include "mappattern.h"
#include "window.h"
#include "map.h"
#include "event.h"
#include "game.h"
#include "py_inc.h"
#include "mapcharacter.h"

mapcharacter::mapcharacter ()
{
//  data->posx=0;
//  data->posy=0;
    addx = 0;
    addy = 0;
    prevmovtype = 0;
    speeddelay = 0;
    speedcounter = 0;
    movtype = 0;
    framefactor = 0;
    walkcounter = 0;
    moveframe = 0;
    scridx = 0;
    waitcounter = 0;
    nbr_of_frames = 0;
    is_event = 0;
    mapcharnbr = 0;
}

mapcharacter::~mapcharacter ()
{
    u_int16 i;

    for (i = 0; i < nbr_of_frames; i++)
        frame[i]. ~ image ();
    free (frame);
    frame = NULL;
    nbr_of_frames = 0;
}

s_int8 mapcharacter::get (SDL_RWops * file)
{
    u_int16 i;
    s_int8 res;

    walkcounter = 11;
    SDL_RWread (file, &nbr_of_frames, sizeof (nbr_of_frames), 1);
    frame = new image[nbr_of_frames];
    for (i = 0; i < nbr_of_frames; i++)
    {
        if ((res = frame[i].get_raw (file)))
            return res;
        frame[i].set_mask (true);
        frame[i].set_trans (0);
    }
    return 0;
}

s_int8 mapcharacter::load (char *fname)
{
    u_int16 retvalue;
    SDL_RWops *file;

    file = SDL_RWFromFile (fname, "r");
    if (!file)
        return (1);
    else
    {
        retvalue = get (file);
        SDL_RWclose (file);
        return (retvalue);
    }
}

void mapcharacter::get_heroe_stat (SDL_RWops * file)
{
    data = (player*) objects::get ("the_player");
    SDL_RWread (file, &data->posx, sizeof (data->posx), 1);
    SDL_RWread (file, &data->posy, sizeof (data->posy), 1);
    SDL_RWread (file, &speeddelay, sizeof (speeddelay), 1);
    SDL_RWread (file, &framefactor, sizeof (framefactor), 1);
    mapcharnbr = 255;
}

void mapcharacter::put_heroe_stat (SDL_RWops * file)
{
    SDL_RWwrite (file, &data->posx, sizeof (data->posx), 1);
    SDL_RWwrite (file, &data->posy, sizeof (data->posy), 1);
    SDL_RWwrite (file, &speeddelay, sizeof (speeddelay), 1);
    SDL_RWwrite (file, &framefactor, sizeof (framefactor), 1);
}

void mapcharacter::get_NPC_stat (SDL_RWops * file, u_int16 nbr)
{
    // normally we'd load the character in at the beginning and just set a pointer
    data = new npc;
    data->name = new char[16];
    sprintf (data->name, "Clone_%i", nbr);
    if (nbr%2) ((npc*) data)->set_schedule ("scripts/follow_player.py");
    else ((npc*) data)->set_schedule ("scripts/random_walk.py");
    objects::set (data->name, data);
    PyDict_SetItemString (game::globals, data->name, pass_instance (data, "npc"));
    
    SDL_RWread (file, &data->posx, sizeof (data->posx), 1);
    SDL_RWread (file, &data->posy, sizeof (data->posy), 1);
    SDL_RWread (file, &speeddelay, sizeof (speeddelay), 1);
    SDL_RWread (file, &framefactor, sizeof (framefactor), 1);
    SDL_RWread (file, &movtype, sizeof (movtype), 1);
    mapcharnbr = nbr;
}

void mapcharacter::put_NPC_stat (SDL_RWops * file)
{
    SDL_RWwrite (file, &data->posx, sizeof (data->posx), 1);
    SDL_RWwrite (file, &data->posy, sizeof (data->posy), 1);
    SDL_RWwrite (file, &speeddelay, sizeof (speeddelay), 1);
    SDL_RWwrite (file, &framefactor, sizeof (framefactor), 1);
    SDL_RWwrite (file, &movtype, sizeof (movtype), 1);
}

u_int16 mapcharacter::mapcharacter::get_nbr ()
{
    return (mapcharnbr);
}

u_int16 mapcharacter::mapcharacter::get_posx ()
{
    return (data->posx);
}

u_int16 mapcharacter::mapcharacter::get_posy ()
{
    return (data->posy);
}

u_int8 mapcharacter::get_addx ()
{
    return (addx);
}

u_int8 mapcharacter::get_addy ()
{
    return (addy);
}

u_int8 mapcharacter::get_scridx ()
{
    return (scridx);
}

u_int8 mapcharacter::get_movtype ()
{
    return (movtype);
}

u_int8 mapcharacter::get_speeddelay ()
{
    return (speeddelay);
}

void mapcharacter::update_frame ()
{
    if ((movtype != 0) && (movtype != 5))
        walkcounter++;
    if (walkcounter == (16 * speeddelay))
    {
        if (moveframe == 0)
            moveframe = 1;
        else
            moveframe = 0;
        walkcounter = 0;
    }
    currentframe = framefactor + moveframe;
}

void mapcharacter::init_moveframe ()
{
    moveframe = 0;
    if ((data->posx % 2) || (data->posy % 2))
        moveframe = 1;
    if ((data->posx % 2) && (data->posy % 2))
        moveframe = 0;
}

void mapcharacter::draw (u_int16 x, u_int16 y, drawing_area * da_opt)
{
    frame[currentframe].draw (x, y, da_opt);
}

/*void mapcharacter::draw_part(u_int16 x, u_int16 y, u_int16 bw, u_int16 bh, 
	       u_int16 xo, u_int16 yo)
{
  frame[currentframe].putbox_mask_part(x,y,bw,bh,xo,yo);
}
*/
bool mapcharacter::is_ready ()
{
    return (speedcounter == 0);
}

bool mapcharacter::is_speed_ready ()
{
    if (speedcounter == speeddelay)
    {
        speedcounter = 0;
        return 1;
    }
    else
        return 0;
}

void mapcharacter::set_speeddelay (u_int8 sd)
{
    speeddelay = sd;
}

void mapcharacter::set_movtype (u_int8 mt)
{
    movtype = mt;
}

void mapcharacter::set_framefactor (u_int8 ff)
{
    framefactor = ff;
}

void mapcharacter::set_posx (u_int16 x)
{
    data->posx = x;
}

void mapcharacter::set_posy (u_int16 y)
{
    data->posy = y;
}

void mapcharacter::update_NPC (landmap * amap)
{
    // freeze characters too far from the player
    character *player = (character *) objects::get("the_player");
    if (abs (data->posx - player->posx) > 15) return;
    if (abs (data->posy - player->posy) > 15) return;

    if ((scridx == 0) && (is_ready ()))
    {
        // update_dir ();
        // just a quick hack
        waitcounter++;
        if (waitcounter == (16 * speeddelay)) waitcounter = 0;
        if (waitcounter == 0) movtype = ((npc*)data)->move (movtype);
        else movtype = 0;
        
        update_move (amap);
    }
    update_eventleave (amap);
    update_frame ();
    update_pos (amap);
    update_eventcome (amap);
}

void mapcharacter::update_dir ()
{
    switch (movtype)
    {
    case RIGHT:
        prevmovtype = LEFT;
        break;
    case LEFT:
        prevmovtype = RIGHT;
        break;
    case UP:
        prevmovtype = DOWN;
        break;
    case DOWN:
        prevmovtype = UP;
        break;
    case 0:
        prevmovtype = 0;
        waitcounter++;
        if (waitcounter == (16 * speeddelay))
            waitcounter = 0;
        break;
    }
    if (movtype != 5)
        do
        {
            movtype = rand () % 5;
        }
        while (movtype == prevmovtype);
    if ((prevmovtype == 0) && (waitcounter != 0))
        movtype = 0;
}

void mapcharacter::update_move (landmap * amap)
{
    switch (movtype)
    {
    case DOWN:
        if (data->posy == amap->get_height () - 1)
        {
            movtype = 0;
            break;
        }
        else
        {
            if (amap->is_others_unreachable (data->posx, data->posy + 1))
            {
                movtype = 0;
                break;
            }
            if ((amap->is_occuped (data->posx, data->posy + 1)) || (amap->is_solid_down (data->posx, data->posy)))
            {
                movtype = 0;
                break;
            }
            else
                amap->put_character (data->posx, data->posy + 1, this);
            framefactor = 0;
        }
        break;
    case UP:
        if (data->posy == 0)
        {
            movtype = 0;
            break;
        }
        else
        {
            if (amap->is_others_unreachable (data->posx, data->posy - 1))
            {
                movtype = 0;
                break;
            }
            if ((amap->is_occuped (data->posx, data->posy - 1)) || (amap->is_solid_up (data->posx, data->posy)))
            {
                movtype = 0;
                break;
            }
            else
                amap->put_character (data->posx, data->posy - 1, this);
            framefactor = 2;
        }
        break;
    case RIGHT:
        if (data->posx == amap->get_lenght () - 1)
        {
            movtype = 0;
            break;
        }
        else
        {
            if (amap->is_others_unreachable (data->posx + 1, data->posy))
            {
                movtype = 0;
                break;
            }
            if ((amap->is_occuped (data->posx + 1, data->posy)) || (amap->is_solid_right (data->posx, data->posy)))
            {
                movtype = 0;
                break;
            }
            else
                amap->put_character (data->posx + 1, data->posy, this);
            framefactor = 4;
        }
        break;
    case LEFT:
        if (data->posx == 0)
        {
            movtype = 0;
            break;
        }
        else
        {
            if (amap->is_others_unreachable (data->posx - 1, data->posy))
            {
                movtype = 0;
                break;
            }
            if ((amap->is_occuped (data->posx - 1, data->posy)) || (amap->is_solid_left (data->posx, data->posy)))
            {
                movtype = 0;
                break;
            }
            else
                amap->put_character (data->posx - 1, data->posy, this);
            framefactor = 6;
        }
        break;
    }
}

void mapcharacter::update_main_move (landmap * amap)
{
    if (get_scridx ()) return;
    
    switch (movtype)
    {
    case DOWN:
        is_event = MAPCHAR_EVENT_LEFT;
        if ((data->posy == amap->get_height () - 1) || amap->is_unreachable (data->posx, data->posy + 1) || (amap->is_occuped (data->posx, data->posy + 1)) || (amap->is_solid_down (data->posx, data->posy)))
            movtype = 0;
        else
            amap->put_character (data->posx, data->posy + 1, this);
        framefactor = 0;
        break;
    case UP:
        is_event = MAPCHAR_EVENT_LEFT;
        if ((data->posy == 0) || amap->is_unreachable (data->posx, data->posy - 1) || (amap->is_occuped (data->posx, data->posy - 1)) || (amap->is_solid_up (data->posx, data->posy)))
            movtype = 0;
        else
            amap->put_character (data->posx, data->posy - 1, this);
        framefactor = 2;
        break;
    case RIGHT:
        is_event = MAPCHAR_EVENT_LEFT;
        if ((data->posx == amap->get_lenght () - 1) || amap->is_unreachable (data->posx + 1, data->posy) || (amap->is_occuped (data->posx + 1, data->posy)) || (amap->is_solid_right (data->posx, data->posy)))
            movtype = 0;
        else
            amap->put_character (data->posx + 1, data->posy, this);
        framefactor = 4;
        break;
    case LEFT:
        is_event = MAPCHAR_EVENT_LEFT;
        if ((data->posx == 0) || amap->is_unreachable (data->posx - 1, data->posy) || (amap->is_occuped (data->posx - 1, data->posy)) || (amap->is_solid_left (data->posx, data->posy)))
            movtype = 0;
        else
            amap->put_character (data->posx - 1, data->posy, this);
        framefactor = 6;
        break;
    }
}

void mapcharacter::update_pos (landmap * amap)
{
    if (movtype != 0) speedcounter++;
    
    switch (movtype)
    {
    case RIGHT:
        if (!is_speed_ready ()) break;

        scridx++;
        addx++;

        if (scridx == 16)
        {
            scridx = 0;
            addx = 0;
            data->posx++;
            amap->leave_character (data->posx - 1, data->posy);
            is_event = MAPCHAR_EVENT_COME;
        }
        break;
    case LEFT:
        if (!is_speed_ready ()) break;

        if (scridx == 0)
        {
            scridx = addx = 16;
            data->posx--;
        }

        scridx--;
        addx--;

        if (scridx == 0)
        {
            amap->leave_character (data->posx + 1, data->posy);
            is_event = MAPCHAR_EVENT_COME;
        }
        break;
    case UP:
        if (!is_speed_ready ()) break;
        if (scridx == 0)
        {
            scridx = addy = 16;
            data->posy--;
        }

        scridx--;
        addy--;

        if (scridx == 0)
        {
            amap->leave_character (data->posx, data->posy + 1);
            is_event = MAPCHAR_EVENT_COME;
        }
        break;

    case DOWN:
        if (!is_speed_ready ())break;
        
        scridx++;
        addy++;
        if (scridx == 16)
        {
            scridx = 0;
            addy = 0;
            data->posy++;
            amap->leave_character (data->posx, data->posy - 1);
            is_event = MAPCHAR_EVENT_COME;
        }
        break;
    }
}

void mapcharacter::update_eventleave (landmap * amap)
{
    if (!is_event) return;
    
    if (is_event == MAPCHAR_EVENT_LEFT)
    {
        is_event = 0;
        amap->launch_event (this, amap->get_square_eventleavenbr (data->posx, data->posy));
        return;
    }
}

void mapcharacter::update_eventcome (landmap * amap)
{
    if (!is_event) return;
    if (is_event == MAPCHAR_EVENT_COME)
    {
        is_event = 0;
        amap->launch_event (this, amap->get_square_eventcomenbr (data->posx, data->posy));

        // test:
        enter_event e;

        e.x = data->posx;
        e.y = data->posy;
        e.dir = movtype;
        e.map = 0;
        e.c = data;

        event_handler::raise_event (&e);

        return;
    }
}
