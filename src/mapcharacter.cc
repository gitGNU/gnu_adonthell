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
 * @file mapcharacter.cc
 *
 * @author Alexandre Courbot <alexandrecourbot@linuxgames.com>
 * @brief Defines the mapcharacter class.
 */
 


#include "mapcharacter.h" 
#include "landmap.h"

using namespace std; 



// Public methods


mapcharacter::mapcharacter () : mapsquare_walkable_area (), character_base ()
{
    submap_ = posx_ = posy_ = offx_ = offy_ = 0;
    refmap = NULL;
    anim.resize (NBR_MOVES);
    for (u_int16 i = 0; i < NBR_MOVES; i++)
        anim[i] = new animation;
    current_move = STAND_NORTH;
    ask_move = NO_MOVE;

    locals = PyDict_New ();
    PyObject * myself = python::pass_instance(this,"mapcharacter"); 
    PyDict_SetItemString (locals,"myself", myself);
    Py_DECREF (myself); 
}
 
mapcharacter::~mapcharacter ()
{
    clear ();
    for (u_int16 i = 0; i < anim.size (); i++)
        delete anim[i];
    anim.clear (); 
    Py_DECREF (locals); 
}

void mapcharacter::clear ()
{
    for (u_int16 i = 0; i < anim.size (); i++)
        anim[i]->clear ();
    
    mapsquare_walkable_area::clear (); 
    
    schedule.clear (); 
    action.clear (); 
    action_file_ = "";
    schedule_file_ = "";
    
    filename_ = "";
    
    PyDict_Clear (locals); 
}

s_int8 mapcharacter::get (igzstream& file)
{
    int i;

    for (i = 0; i < NBR_MOVES; i++)
    {
        anim[i]->get (file);
        anim[i]->play ();
    }

    mapsquare_walkable_area::get (file); 

    return 0;
}

s_int8 mapcharacter::load (string fname)
{
    string s = MAPCHAR_DIR;

    s += fname;
    igzstream file (s);
    if (!file.is_open ())
        return -1;
    
    s_int8 retvalue; 
    if (fileops::get_version (file, 1, 1, s))
        retvalue = get (file);
    file.close ();
    filename_ = fname;

    return 0;
}

s_int8 mapcharacter::get_state (igzstream& file)
{
    // Load the schedule's and graphical data
    string t; 
    bool activated; 
    
    remove_from_pos (); 
    
    t << file; 
    load (t);
    t << file; 
    set_schedule (t);
    t << file; 
    set_action (t);

    // Reads the data members
    current_move << file;
    ask_move << file;
    submap_ << file;
    posx_ << file;
    posy_ << file;
    offx_ << file;
    offy_ << file;
    activated << file;
    schedule.set_active (activated); 
    activated << file; 
    action.set_active (activated); 

    jump_to (submap (), posx (), posy ()); 
    
    return 0;
}

s_int8 mapcharacter::put_state (ogzstream& file) const
{
    // Write the schedule's and data file name
    filename_ >> file;
    schedule_file_ >> file;
    action_file_ >> file; 

    // Write the data members
    current_move >> file;
    ask_move >> file;
    submap_ >> file;
    posx_ >> file;
    posy_ >> file;
    offx_ >> file;
    offy_ >> file;
    schedule.is_activated () >> file;
    action.is_activated () >> file; 

    return 0;
}

void mapcharacter::set_map (landmap * m) 
{
    if (mymap ()) return;

    m->mapchar.push_back (this);
    
    refmap = m;
    PyObject *mymap = python::pass_instance (refmap,"landmap"); 
    PyDict_SetItemString (locals, "mymap", mymap);
    Py_DECREF (mymap); 
}

void mapcharacter::remove_from_map ()
{
    if (!mymap ()) return;
    
    leave_position ();
    
    vector <mapcharacter *>::iterator i;
    for (i = mymap ()->mapchar.begin (); (*i) != this; i++);
    mymap ()->mapchar.erase (i); 
    
    refmap = NULL;
    PyDict_DelItemString (locals, "mymap");
}

void mapcharacter::remove_from_pos ()
{
    leave_position (); 
}

void mapcharacter::jump_to (u_int16 smap, u_int16 x, u_int16 y,
                         u_int16 pos = NO_MOVE)
{
    leave_position (); 
    set_pos (smap, x, y);
    switch (pos)
    {
        case NO_MOVE:
            break;
        case STAND_NORTH:
            stand_north ();
            break;
        case STAND_SOUTH:
            stand_south ();
            break;
        case STAND_WEST:
            stand_west ();
            break;
        case STAND_EAST:
            stand_east ();
            break;
    }
}

void mapcharacter::stand ()
{
    if (current_move >= WALK_NORTH && current_move != NO_MOVE)
    {
        anim[current_move]->stop ();
        anim[current_move]->rewind ();
        current_move -= WALK_NORTH;
        anim[current_move]->play ();
    }
    ask_move = NO_MOVE;
}

void mapcharacter::stand_north ()
{
    anim[current_move]->stop ();
    anim[current_move]->rewind ();
    current_move = STAND_NORTH;
    anim[current_move]->play ();
}

void mapcharacter::stand_south ()
{
    anim[current_move]->stop ();
    anim[current_move]->rewind ();
    current_move = STAND_SOUTH;
    anim[current_move]->play ();
}

void mapcharacter::stand_east ()
{
    anim[current_move]->stop ();
    anim[current_move]->rewind ();
    current_move = STAND_EAST;
    anim[current_move]->play ();
}

void mapcharacter::stand_west ()
{
    anim[current_move]->stop ();
    anim[current_move]->rewind ();
    current_move = STAND_WEST;
    anim[current_move]->play ();
}

bool mapcharacter::can_go_north () const
{
    if (!posy ())
        return false;
    u_int16 i, j;
    u_int16 sx = (posx () - base_x ()  < 0) ? 0 : posx () - base_x ();
    u_int16 sy = (posy () - base_y ()  < 0) ? 0 : posy () - base_y ();
    s_int16 ax = sx - (posx () - base_x ());
    s_int16 ay = sy - (posy () - base_y ());
    u_int16 ex =
        (posx () - base_x () + area_length () >=
         refmap->submap[submap ()]->area_length ()) ? refmap->submap[submap ()]->area_length () -
        1 : posx () - base_x () + area_length ();
    u_int16 ey =
        (posy () - base_y () + area_height () >=
         refmap->submap[submap ()]->area_height ()) ? refmap->submap[submap ()]->area_height () -
        1 : posy () - base_y () + area_height ();

    for (j = sy; j < ey; j++)
        for (i = sx; i < ex; i++)
        {
            if (get_square (i - sx + ax, j - sy + ay)->get_walkable ())
                continue;
            if (!j)
                continue;
            if (!(refmap->submap[submap ()]->area[i][j].is_walkable_north () &&
                  refmap->submap[submap ()]->area[i][j - 1].is_walkable_south ()
                  && refmap->submap[submap ()]->area[i][j - 1].is_free ()))
                return false;
        }
    return true;
}

bool mapcharacter::can_go_south () const
{
    if (posy () == refmap->submap[submap ()]->area_height () - 1)
        return false;
    u_int16 i, j;
    u_int16 sx = (posx () - base_x () < 0) ? 0 : posx () - base_x ();
    u_int16 sy = (posy () - base_y () < 0) ? 0 : posy () - base_y ();
    s_int16 ax = sx - (posx () - base_x ());
    s_int16 ay = sy - (posy () - base_y ());
    u_int16 ex =
        (posx () - base_x () + area_length () >=
         refmap->submap[submap ()]->area_length ()) ? refmap->submap[submap ()]->area_length () -
        1 : posx () - base_x () + area_length ();
    u_int16 ey =
        (posy () - base_y () + area_height () >=
         refmap->submap[submap ()]->area_height ()) ? refmap->submap[submap ()]->area_height () -
        1 : posy () - base_y () + area_height ();

    for (j = sy; j < ey; j++)
        for (i = sx; i < ex; i++)
        {
            if (get_square (i - sx + ax, j - sy + ay)->get_walkable ())
                continue;
            if (j == refmap->submap[submap ()]->area_height () - 1)
                continue;
            if (!(refmap->submap[submap ()]->area[i][j].is_walkable_south () &&
                  refmap->submap[submap ()]->area[i][j +
                                                  1].is_walkable_north ()
                  && refmap->submap[submap ()]->area[i][j + 1].is_free ()))
                return false;
        }
    return true;
}

bool mapcharacter::can_go_east () const
{
    if (posx () == refmap->submap[submap ()]->area_length () - 1)
        return false;
    u_int16 i, j;
    u_int16 sx = (posx () - base_x () < 0) ? 0 : posx () - base_x ();
    u_int16 sy = (posy () - base_y () < 0) ? 0 : posy () - base_y ();
    s_int16 ax = sx - (posx () - base_x ());
    s_int16 ay = sy - (posy () - base_y ());
    u_int16 ex =
        (posx () - base_x () + area_length () >=
         refmap->submap[submap ()]->area_length ()) ? refmap->submap[submap ()]->area_length () -
        1 : posx () - base_x () + area_length ();
    u_int16 ey =
        (posy () - base_y () + area_height () >=
         refmap->submap[submap ()]->area_height ()) ? refmap->submap[submap ()]->area_height () -
        1 : posy () - base_y () + area_height ();

    for (j = sy; j < ey; j++)
        for (i = sx; i < ex; i++)
        {
            if (get_square (i - sx + ax, j - sy + ay)->get_walkable ())
                continue;
            if (i == refmap->submap[submap ()]->area_length () - 1)
                continue;
            if (!(refmap->submap[submap ()]->area[i][j].is_walkable_east () &&
                  refmap->submap[submap ()]->area[i + 1][j].is_walkable_west ()
                  && refmap->submap[submap ()]->area[i + 1][j].is_free ()))
                return false;
        }
    return true;
}

bool mapcharacter::can_go_west () const
{
    if (!posx ())
        return false;
    u_int16 i, j;
    u_int16 sx = (posx () - base_x () < 0) ? 0 : posx () - base_x ();
    u_int16 sy = (posy () - base_y () < 0) ? 0 : posy () - base_y ();
    s_int16 ax = sx - (posx () - base_x ());
    s_int16 ay = sy - (posy () - base_y ());
    u_int16 ex =
        (posx () - base_x () + area_length () >=
         refmap->submap[submap ()]->area_length ()) ? refmap->submap[submap ()]->area_length () -
        1 : posx () - base_x () + area_length ();
    u_int16 ey =
        (posy () - base_y () + area_height () >=
         refmap->submap[submap ()]->area_height ()) ? refmap->submap[submap ()]->area_height () -
        1 : posy () - base_y () + area_height ();

    for (j = sy; j < ey; j++)
        for (i = sx; i < ex; i++)
        {
            if (get_square (i - sx + ax, j - sy + ay)->get_walkable ())
                continue;
            if (!i)
                continue;
            if (!(refmap->submap[submap ()]->area[i][j].is_walkable_west () &&
                  refmap->submap[submap ()]->area[i - 1][j].is_walkable_east ()
                  && refmap->submap[submap ()]->area[i - 1][j].is_free ()))
                return false;
        }
    return true;
}

void mapcharacter::go_north ()
{
    if (current_move < WALK_NORTH)
    {
        anim[current_move]->stop ();
        anim[current_move]->rewind ();
        current_move = WALK_NORTH;
        anim[current_move]->play ();
        return;
    }
    ask_move = WALK_NORTH;
}

void mapcharacter::go_south ()
{
    if (current_move < WALK_NORTH)
    {
        anim[current_move]->stop ();
        anim[current_move]->rewind ();
        current_move = WALK_SOUTH;
        anim[current_move]->play ();
        return;
    }
    ask_move = WALK_SOUTH;
}

void mapcharacter::go_east ()
{
    if (current_move < WALK_NORTH)
    {
        anim[current_move]->stop ();
        anim[current_move]->rewind ();
        current_move = WALK_EAST;
        anim[current_move]->play ();
        return;
    }
    ask_move = WALK_EAST;
}

void mapcharacter::go_west ()
{
    if (current_move < WALK_NORTH)
    {
        anim[current_move]->stop ();
        anim[current_move]->rewind ();
        current_move = WALK_WEST;
        anim[current_move]->play ();
        return;
    }
    ask_move = WALK_WEST;
}

void mapcharacter::look_invert (u_int16 p)
{
    switch (p)
    {
        case STAND_NORTH:
            stand_south ();
            break;
        case STAND_SOUTH:
            stand_north ();
            break;
        case STAND_EAST:
            stand_west ();
            break;
        case STAND_WEST:
            stand_east ();
            break;
    }
}

mapcharacter *mapcharacter::whosnext () const
{
    switch (current_move)
    {
        case STAND_NORTH:
            if (posy () == 0)
                return NULL;
            return refmap->submap[submap ()]->area[posx ()][posy () - 1].whoshere ();
            break;
        case STAND_SOUTH:
            if (posy () == refmap->submap[submap ()]->area_height () - 1)
                return NULL;
            return refmap->submap[submap ()]->area[posx ()][posy () + 1].whoshere ();
            break;
        case STAND_WEST:
            if (posx () == 0)
                return NULL;
            return refmap->submap[submap ()]->area[posx () - 1][posy ()].whoshere ();
            break;
        case STAND_EAST:
            if (posx () == refmap->submap[submap ()]->area_length () - 1)
                return NULL;
            return refmap->submap[submap ()]->area[posx () + 1][posy ()].whoshere ();
            break;
    }
    return NULL;
}

void mapcharacter::set_schedule (string file)
{
    schedule.set_locals (locals); 
    if (file == "") schedule.set_script (file);
    else schedule.set_script ("scripts/schedules/" + file + ".py");
    schedule_file_ = file; 
}

void mapcharacter::set_action (string file)
{
    action.set_locals (locals); 
    if (file == "") action.set_script (file);
    else action.set_script ("scripts/schedules/" + file + ".py");
    action_file_ = file; 
}
 
void mapcharacter::update ()
{
    schedule.run (); 
    update_move ();
}

void mapcharacter::launch_action (mapcharacter * requester)
{
    PyObject *req = python::pass_instance (requester, "mapcharacter"); 
    PyDict_SetItemString (locals, "requester", req);
    Py_DECREF (req); 
    action.run (); 
    PyDict_DelItemString (locals, "requester");
}

void mapcharacter::draw (s_int16 x, s_int16 y, const drawing_area * da_opt = NULL, surface * target = NULL) const
{
    anim[current_move]->draw (x, y, da_opt, target);
}

mapcharacter & mapcharacter::operator = (const mapcharacter & src)
{
    u_int16 i;

    clear (); 

    (mapsquare_walkable_area&) (*this) = (mapsquare_walkable_area&) src;
    (character_base&) (*this) = (character_base&) src;
    
    for (i = 0; i < NBR_MOVES; i++)
        (*anim[i]) = (*src.anim[i]);

    schedule = src.schedule;
    schedule_file_ = src.schedule_file_; 

    action = src.action;
    action_file_ = src.action_file_; 

    current_move = src.currentmove ();
    if (src.mymap ())
    {
        set_map (src.mymap ());
        set_pos (src.submap (), src.posx (), src.posy ());
        set_offset (src.offx (), src.offy ()); 
    }

    filename_ = src.filename_;
    
    return *this;
}



// Private methods


void mapcharacter::occupy (u_int16 smap, u_int16 px, u_int16 py)
{
    mapsquare_char mschar;

    list <mapsquare_char>::iterator it;
    u_int16 sx = (px - base_x () < 0) ? 0 : px - base_x ();
    u_int16 sy = (py - base_y () < 0) ? 0 : py - base_y ();
    u_int16 ex = (sx + area_length () > refmap->submap[smap]->area_length ()) ?
        refmap->submap[smap]->area_length () : sx + area_length ();
    u_int16 ey = (sy + area_height () > refmap->submap[smap]->area_height ()) ?
        refmap->submap[smap]->area_height () : sy + area_height ();
    u_int16 i, j;

    // Placing the base tile first
    mschar.mchar = this;
    mschar.is_base = true;
    mschar.x = px;
    mschar.y = py;
    mschar.walkable =
        get_square (base_x (), base_y ())->get_walkable ()  == ALL_WALKABLE;
    
    refmap->submap[smap]->area[px][py].mapchars.push_back (mschar);
    it = --refmap->submap[smap]->area[px][py].mapchars.end ();
    it->base_tile = it;
    mschar.base_tile = it;
    mschar.is_base = false;
    
    // Ready to place the rest now
    for (i = sx; i < ex; i++)
        for (j = sy; j < ey; j++)
            if (i != px || j != py)
            {
                mschar.x = i;
                mschar.y = j;
                mschar.walkable =
                    get_square (sx + base_x () - px, sy + base_y () - py)->
                    get_walkable () == ALL_WALKABLE;
                refmap->submap[smap]->area[i][j].mapchars.push_back (mschar);
            }
} 

void mapcharacter::leave (u_int16 smap, u_int16 px, u_int16 py)
{
    list <mapsquare_char>::iterator it;
    list <mapsquare_char>::iterator e;

    u_int16 sx = (px - base_x () < 0) ? 0 : px - base_x ();
    u_int16 sy = (py - base_y () < 0) ? 0 : py - base_y ();
    u_int16 ex = (sx + area_length () > refmap->submap[smap]->area_length ()) ?
        refmap->submap[smap]->area_length () : sx + area_length ();
    u_int16 ey = (sy + area_height () > refmap->submap[smap]->area_height ()) ?
        refmap->submap[smap]->area_height () : sy + area_height ();
    u_int16 i, j;


    for (i = sx; i < ex; i++)
        for (j = sy; j < ey; j++)
        {
            it = refmap->submap[smap]->area[i][j].mapchars.begin ();
            e = refmap->submap[smap]->area[i][j].mapchars.end ();

            while (it != e && it->mchar != this)
                it++;
            if (it != e)
                refmap->submap[smap]->area[px][py].mapchars.erase (it);
        }     
}

void mapcharacter::leave_position () 
{
    leave (submap (), posx (), posy ());
    switch (current_move) 
    {
        case WALK_NORTH: 
        case WALK_SOUTH:
            leave (submap (), posx (), posy () - 1); 
            
        case WALK_WEST: 
        case WALK_EAST:
            leave (submap (), posx () - 1, posy ()); 
    }
}

void mapcharacter::set_pos (u_int16 smap, u_int16 x, u_int16 y)
{
    submap_ = smap;
    posx_ = x;
    posy_ = y;
    occupy (submap (), posx (), posy ()); 
}

void mapcharacter::update_move ()
{
    if (refmap)
        switch (current_move)
        {
            case WALK_NORTH:
                if (!offy ())
                {
                    if (!can_go_north ())
                    {
                        stand_north ();
                        break;
                    }
                    occupy (submap (), posx (), posy () - 1);
                }
                set_offset (offx (), offy () - 1);
                
                if (offy () == -MAPSQUARE_SIZE)
                {
                    leave (submap (), posx (), posy ());
                    leave (submap (), posx (), posy () - 1);
                    set_pos (submap (), posx (), posy () - 1);
                    set_offset (offx (), 0); 
                    if (ask_move != WALK_NORTH)
                        stand_north ();
                    enter_event evt;

                    evt.submap = submap ();
                    evt.x = posx ();
                    evt.y = posy ();
                    evt.c = this;
                    event_handler::raise_event (evt);
                }
                break;
            case WALK_SOUTH:
                if (!offy ())
                {
                    if (!can_go_south ())
                    {
                        stand_south ();
                        break;
                    }
                    leave (submap (), posx (), posy ());
                    occupy (submap (), posx (), posy ());
                    set_pos (submap (), posx (), posy () + 1);
                    set_offset (0, -(MAPSQUARE_SIZE - 1));
                }
                else
                {
                    set_offset (offx (), offy () + 1);

                    if (!offy ())
                    {
                        leave (submap (), posx (), posy () - 1);
                        if (ask_move != WALK_SOUTH)
                            stand_south ();
                        enter_event evt;

                        evt.submap = submap ();
                        evt.x = posx ();
                        evt.y = posy ();
                        evt.c = this;
                        event_handler::raise_event (evt);
                    }
                }
                break;
            case WALK_WEST:
                if (!offx ())
                {
                    if (!can_go_west ())
                    {
                        stand_west ();
                        break;
                    }
                    occupy (submap (), posx () - 1, posy ());
                }
                set_offset (offx () -1, offy ());  
                if (offx () == -MAPSQUARE_SIZE)
                {
                    leave (submap (), posx (), posy ());
                    leave (submap (), posx () - 1, posy ());
                    set_pos (submap (), posx () - 1, posy ());
                    set_offset (0, offy ());  
                    if (ask_move != WALK_WEST)
                        stand_west ();
                    enter_event evt;

                    evt.submap = submap ();
                    evt.x = posx ();
                    evt.y = posy ();
                    evt.c = this;
                    event_handler::raise_event (evt);
                }
                break;
            case WALK_EAST:
                if (!offx ())
                {
                    if (!can_go_east ())
                    {
                        stand_east ();
                        break;
                    }
                    leave (submap (), posx (), posy ());
                    occupy (submap (), posx (), posy ());
                    set_pos (submap (), posx () + 1, posy ());
                    set_offset (-(MAPSQUARE_SIZE - 1), 0);
                }
                else
                {
                    set_offset (offx () + 1, offy ());  
                    if (!offx ())
                    {
                        leave (submap (), posx () - 1, posy ());
                        if (ask_move != WALK_EAST)
                            stand_east ();
                        enter_event evt;

                        evt.submap = submap ();
                        evt.x = posx ();
                        evt.y = posy ();
                        evt.c = this;
                        event_handler::raise_event (evt);
                    }
                }
                break;
        }
    anim[current_move]->update ();     
    ask_move = NO_MOVE;
}
