/*
   $Id$

   Copyright (C) 1999/2000/2001/2002 Alexandre Courbot
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
#include "map_event.h"
#include "time_event.h"
#include "event_handler.h"
#include "landmap.h"
#include "win_manager.h"

using namespace std; 


// Public methods
mapcharacter::mapcharacter () : mapsquare_walkable_area (), character_base (), event_list ()
{
    submap_ = posx_ = posy_ = offx_ = offy_ = 0;
    refmap = NULL;
    anim.resize (NBR_MOVES);
    for (u_int16 i = 0; i < NBR_MOVES; i++)
        anim[i] = new animation;
    current_move = STAND_NORTH;
    previous_move = NO_MOVE;

    saying = NULL;

    schedule_activated = true;
    action_activated = true; 
    goal_reached_ = true;

    schedule_args = NULL;
    action_args = NULL;
    
    callback = NULL;
}
 
mapcharacter::~mapcharacter ()
{
    clear ();
    for (u_int16 i = 0; i < anim.size (); i++)
        delete anim[i];
    anim.clear (); 
}

void mapcharacter::clear ()
{
    event_list::clear ();
    
    if (saying) delete saying;

    for (u_int16 i = 0; i < anim.size (); i++)
        anim[i]->clear ();
    
    mapsquare_walkable_area::clear (); 
    
    schedule.clear (); 
    action.clear (); 
    
    filename_ = "";

    Py_XDECREF (schedule_args);
    schedule_args = NULL;
    
    Py_XDECREF (action_args);
    action_args = NULL; 
    schedule_file_ = "";
    action_file_ = "";
    
    if (callback) delete callback;
}

s_int8 mapcharacter::get (igzstream& file)
{
    int i;

    for (i = 0; i < NBR_MOVES; i++)
    {
        anim[i]->get (file);
        anim[i]->stop ();
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

s_int8 mapcharacter::put (ogzstream& file) const
{
    int i;

    for (i = 0; i < NBR_MOVES; i++)
    {
        anim[i]->put (file);
    }

    mapsquare_walkable_area::put (file); 

    return 0;
}

s_int8 mapcharacter::save (string fname) const
{
    string s = MAPCHAR_DIR;

    s += fname;
    ogzstream file (s);
    if (!file.is_open ())
        return -1;
    
    s_int8 retvalue; 
    fileops::put_version (file, 1); 
    retvalue = put (file);
    file.close ();

    return 0;
}

s_int8 mapcharacter::get_state (igzstream& file)
{
    string t; 
    bool b;
    u_int16 current_move__; 
    s_int8 offx__, offy__; 
    
    remove_from_pos (); 
    
    t << file; 
    load (t);

    // Reads the data members
    current_move__ << file;
    previous_move << file;
    submap_ << file;
    posx_ << file;
    posy_ << file;
    offx__ << file;
    offy__ << file;

    jump_to (submap (), posx (), posy ());
    set_offset (offx__, offy__); 
    
    current_move = current_move__; 
    
    // Get the path state
    mypath.get_state (file);
    // The map must be attached manually for now! :(
    mypath.refmap = refmap; 
    
    pathindex << file; 
    goal_reached_ << file;
    
    // Schedule state
    PyObject * args; 
    t << file;
    b << file;
    args = NULL; 
    if (b) args = python::get_tuple (file); 
    set_schedule (t, args);      
    Py_XDECREF (args); 
    b << file;
    set_schedule_active (b);

    // Action state
    t << file;
    b << file; 
    args = NULL; 
    if (b) args = python::get_tuple (file); 
    set_action (t, args);      
    Py_XDECREF (args); 
    b << file;
    set_action_active (b);

    // get the events
    py_callback::instance = schedule.get_instance (false);
    return event_list::get_state (file);
}

s_int8 mapcharacter::put_state (ogzstream& file) const
{
    // Write the mapcharacter's file name
    filename_ >> file;

    // Write the data members
    current_move >> file;
    previous_move >> file;
    submap_ >> file;
    posx_ >> file;
    posy_ >> file;
    offx_ >> file;
    offy_ >> file;

    // Save the path state
    mypath.put_state (file);
    pathindex >> file; 
    goal_reached_ >> file;
    
    // Save the schedule script state
    schedule_file () >> file;
    if (schedule_args) 
    {
        true >> file; 
        python::put_tuple (schedule_args, file);
    }
    else false >> file; 
    is_schedule_activated () >> file;
    
    // Save the action script state
    action_file () >> file;
    if (action_args) 
    {
        true >> file; 
        python::put_tuple (action_args, file);
    }
    else false >> file; 
    is_action_activated () >> file; 
    
    // save the events
    event_list::put_state (file);
    
    return 0;
}

void mapcharacter::set_map (landmap * m) 
{
    if (mymap ()) return;

    m->mapchar.push_back (this);
    
    refmap = m;
}

void mapcharacter::remove_from_map ()
{
    if (!mymap ()) return;
    
    leave_position ();
    
    vector <mapcharacter *>::iterator i;
    for (i = mymap ()->mapchar.begin (); (*i) != this; i++);
    mymap ()->mapchar.erase (i); 
    
    refmap = NULL;
}

void mapcharacter::remove_from_pos ()
{
    leave_position (); 
}

void mapcharacter::jump_to (u_int16 smap, u_int16 x, u_int16 y,
			    u_int16 pos)
{
    leave_position (); 
    set_pos (smap, x, y);
    set_offset (0, 0);

    switch (pos)
    {
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
        default:
            stand ();
            break;
    }

    enter_event evt; 
    evt.submap = submap ();
    evt.x = posx ();
    evt.y = posy ();
    evt.c = this;
    evt.dir = pos; 
    event_handler::raise_event (&evt);
}

void mapcharacter::stand ()
{
    if (current_move >= WALK_NORTH && current_move != NO_MOVE)
    {
        previous_move = current_move; 
        current_move -= WALK_NORTH;
    }
}

void mapcharacter::stand_north ()
{
    previous_move = current_move; 
    current_move = STAND_NORTH;
}

void mapcharacter::stand_south ()
{
    previous_move = current_move; 
    current_move = STAND_SOUTH;
}

void mapcharacter::stand_east ()
{
    previous_move = current_move; 
    current_move = STAND_EAST;
}

void mapcharacter::stand_west ()
{
    previous_move = current_move; 
    current_move = STAND_WEST;
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
        (posx () - base_x () + area_length () >
         refmap->submap[submap ()]->area_length ()) ? refmap->submap[submap ()]->area_length ()
        : posx () - base_x () + area_length ();
    u_int16 ey =
        (posy () - base_y () + area_height () >
         refmap->submap[submap ()]->area_height ()) ? refmap->submap[submap ()]->area_height ()
        : posy () - base_y () + area_height ();

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
         refmap->submap[submap ()]->area_length ()) ? refmap->submap[submap ()]->area_length ()
        : posx () - base_x () + area_length ();
    u_int16 ey =
        (posy () - base_y () + area_height () >=
         refmap->submap[submap ()]->area_height ()) ? refmap->submap[submap ()]->area_height ()
        : posy () - base_y () + area_height ();

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
         refmap->submap[submap ()]->area_length ()) ? refmap->submap[submap ()]->area_length ()
        : posx () - base_x () + area_length ();
    u_int16 ey =
        (posy () - base_y () + area_height () >=
         refmap->submap[submap ()]->area_height ()) ? refmap->submap[submap ()]->area_height ()
        : posy () - base_y () + area_height ();

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
        (posx () - base_x () + area_length () >
         refmap->submap[submap ()]->area_length ()) ? refmap->submap[submap ()]->area_length ()
        : posx () - base_x () + area_length ();
    u_int16 ey =
        (posy () - base_y () + area_height () >
         refmap->submap[submap ()]->area_height ()) ? refmap->submap[submap ()]->area_height ()
        : posy () - base_y () + area_height ();

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

bool mapcharacter::go_north ()
{
    if (current_move < WALK_NORTH)
    {
        bool ret = can_go_north ();
        previous_move = current_move;  
        if (ret) 
            current_move = WALK_NORTH;
        else current_move = STAND_NORTH;
        return ret; 
    }
    return false; 
}

bool mapcharacter::go_south ()
{
    if (current_move < WALK_NORTH)
    {
        bool ret = can_go_south (); 
        previous_move = current_move;
        if (ret) 
            current_move = WALK_SOUTH;
        else current_move = STAND_SOUTH; 
        return ret; 
    }
    return false; 
}

bool mapcharacter::go_east ()
{
    if (current_move < WALK_NORTH)
    {
        bool ret = can_go_east (); 
        previous_move = current_move;
        if (ret) 
            current_move = WALK_EAST;
        else current_move = STAND_EAST; 
        return ret; 
    }
    return false; 
}

bool mapcharacter::go_west ()
{
    if (current_move < WALK_NORTH)
    {
        bool ret = can_go_west (); 
        previous_move = current_move;
        if (ret) 
            current_move = WALK_WEST;
        else current_move = STAND_WEST; 
        return ret; 
    }
    return false; 
}

bool mapcharacter::set_goal (u_int16 x, u_int16 y, u_int16 dir)
{
    mypath.refmap = mymap ();
    mypath.submap = submap (); 
    mypath.start.x = posx ();
    mypath.start.y = posy (); 
    mypath.goal.x = x;
    mypath.goal.y = y;
    mypath.dir = dir;
    pathindex = 0; 
    goal_reached_ = false;
    
    return mypath.calculate (); 
}

void mapcharacter::set_callback (PyObject *cb, PyObject *args)
{
    if (callback) delete callback;
    callback = new py_callback (cb, args);
}

void mapcharacter::time_callback (string delay, PyObject *cb, PyObject *args)
{
    time_event *ev = new time_event (delay);
    ev->set_callback (cb, args);
    add_event (ev);
}

void mapcharacter::time_callback_string (string delay, string cb, PyObject *args)
{
    PyObject *instance = schedule.get_instance (false);

    // check that we have a valid instance that contains our callback
    if (instance == NULL)
    {
        fprintf (stderr, "*** error: mapcharacter::time_callback: Invalid instance!");
        return;
    }
     
    PyObject *callback = PyObject_GetAttrString (instance, (char *) cb.c_str ());

    if (!PyCallable_Check (callback))
    {
        fprintf (stderr, "*** error: mapcharacter::time_callback: Setting callback ' %s' failed!", cb.c_str ());
    }
    else
    {
        time_event *ev = new time_event (delay);
        ev->set_callback (callback, args);
        add_event (ev);
    }
    
    Py_XDECREF (callback);
}

bool mapcharacter::follow_path () 
{
    //  If a movment is engaged, let it finish first.
    if (offx () || offy ()) 
        return false;
    
    // If the goal isn't reached yet.
    if (pathindex < mypath.nbr_moves ())
    {
        u_int16 dir = mypath.get_move (pathindex);
        u_int8 success = 0; 

        // Try to follow the direction
        switch (dir) 
        {
            case WALK_NORTH:
                if (go_north ()) success = 1; 
                break;

            case WALK_SOUTH:
                if (go_south ()) success = 1; 
                break;
                
            case WALK_WEST:
                if (go_west ()) success = 1; 
                break;
                
            case WALK_EAST:
                if (go_east ()) success = 1; 
                break;
        }
                 
        // Who the fuck is on my way!!?@! I have to find a new path now!
        if (!success) 
        {
            mypath.start.x = posx ();
            mypath.start.y = posy ();
            mypath.submap = submap ();
            mypath.calculate ();
            pathindex = 0; 
        }
        else pathindex++; 
    }
    else
    {
        switch (mypath.dir)
        {
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

        // goal reached -> notify script (as the script might immediately
        // set the next goal, we gotta set goal_reached_ before that)
        goal_reached_ = true;
        if (callback) callback->callback_func0 ();
        return true;
    }
    return false;
}

void mapcharacter::stop_moving ()
{
    set_goal (posx (), posy ());
}

bool mapcharacter::goal_reached () 
{ 
    return goal_reached_;
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

bool mapcharacter::do_stuff (string method, PyObject *args)
{
    if (!schedule.has_attribute (method)) return false;
    else schedule.call_method (method, args == Py_None ? NULL : args);
    
    return true;
}

void mapcharacter::set_schedule (string file, PyObject * args)
{     
    // Clears the schedule
    schedule.clear ();
    Py_XDECREF (schedule_args);
    schedule_args = NULL;

    // Set new schedule
    if (file != "")
    {
    	if (args == Py_None) args = NULL;
        Py_XINCREF (args); 
        schedule_args = args; 
        u_int16 argssize = args == NULL ? 1 : PyTuple_Size (args) + 1; 
        PyObject * theargs;
        
        theargs = PyTuple_New (argssize);
        
        // We can pass_instance directly 'cause PyTuple_SetItem steals a
        // reference to the result of pass_instance.
        PyTuple_SetItem (theargs, 0, python::pass_instance (this, "mapcharacter"));
        for (u_int16 i = 1; i < argssize; i++)
        {
            PyObject * intref = PyTuple_GetItem (args, i - 1);
            Py_INCREF (intref); 
            PyTuple_SetItem (theargs, i, intref); 
        }
        schedule.create_instance ("schedules.mapcharacters." + file, file, theargs);
        Py_DECREF (theargs);
        
        if (!schedule.has_attribute ("run"))
            set_schedule_active (false);
    }
    schedule_file_ = file;
}

void mapcharacter::set_action (string file, PyObject * args)
{     
    // Clears the action script
    action.clear ();
    Py_XDECREF (action_args);
    action_args = NULL;

    if (file != "")
    {
        Py_XINCREF (args);
        action_args = args; 
        u_int16 argssize = args == NULL ? 1 : PyTuple_Size (args) + 1; 
        PyObject * theargs;
        
        theargs = PyTuple_New (argssize);
        
        // We can pass_instance directly 'cause PyTuple_SetItem steals a
        // reference to the result of pass_instance.
        PyTuple_SetItem (theargs, 0, python::pass_instance (this, "mapcharacter"));
        for (u_int16 i = 1; i < argssize; i++)
        {
            PyObject * intref = PyTuple_GetItem (args, i - 1);
            Py_INCREF (intref); 
            PyTuple_SetItem (theargs, i, intref); 
        }
        action.create_instance ("actions." + file, file, theargs);
        Py_DECREF (theargs); 
    }
    action_file_ = file;
}
 
bool mapcharacter::update ()
{
    update_move ();

    if (is_schedule_activated ()) 
        schedule.run ();

    // if we have a goal, then go there!
    if (!goal_reached ()) 
        follow_path ();

    if (previous_move != NO_MOVE && previous_move != current_move) 
    {
        anim[previous_move]->stop ();
        anim[previous_move]->rewind ();
        anim[current_move]->play (); 
    }
    
    if (saying && !saying->update ())
    {
        delete saying;
        saying = NULL; 
    }
        
    return true;
}

void mapcharacter::launch_action (mapcharacter * requester)
{
    PyObject *args = PyTuple_New (1);
    PyTuple_SetItem (args, 0, python::pass_instance (requester, "mapcharacter"));      
    if (is_action_activated ()) action.run (args); 
    Py_DECREF (args);
}

void mapcharacter::draw (s_int16 x, s_int16 y, const drawing_area * da_opt, surface * target) const
{
    anim[current_move]->draw (x, y, da_opt, target);
}

void mapcharacter::draw_bubble (s_int16 x, s_int16 y, const drawing_area * da_opt,
                                surface * target) const
{
    if (saying) 
    {
        s_int16 dx = x - (saying->drawing_area::length () >> 1) + (anim[current_move]->length () >> 1); 
        s_int16 dy = y - (saying->drawing_area::height ()) + 5;

        if (dx < 4) dx = 4;
        else if (dx + saying->drawing_area::length () > da_opt->x () + da_opt->length () - 4)
            dx = da_opt->x () + da_opt->length () - saying->drawing_area::length () - 4; 
        
        saying->move (dx, dy);
        saying->assign_drawing_area (da_opt);
        saying->draw ();
        saying->detach_drawing_area (); 
    }     
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

    action = src.action;

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
            break;
            
        case WALK_WEST:
        case WALK_EAST:
            leave (submap (), posx () - 1, posy ());
            break;
    }
}

void mapcharacter::set_pos (u_int16 smap, u_int16 x, u_int16 y)
{
    // update character position
    submap_ = smap;
    posx_ = x;
    posy_ = y;

    // mark the character's place as occupied
    occupy (submap (), posx (), posy ()); 
}

void mapcharacter::update_move ()
{
    if (refmap)
        switch (currentmove ())
        {
            case WALK_NORTH:
                if (!offy ())
                {
                    if (!can_go_north ())
                    {
                        stand_north ();
                        break;
                    }
                    leave_event evt;
                    
                    evt.submap = submap ();
                    evt.x = posx ();
                    evt.y = posy ();
                    evt.c = this;
                    evt.dir = WALK_NORTH; 
                    event_handler::raise_event (&evt);
                    
                    occupy (submap (), posx (), posy () - 1);
                    set_offset (offx (), offy () - 1);
                }
                
                set_offset (offx (), offy () - 1);

                if (offy () == -MAPSQUARE_SIZE)
                {
                    leave (submap (), posx (), posy ());
                    leave (submap (), posx (), posy () - 1);
                    set_pos (submap (), posx (), posy () - 1);
                    set_offset (offx (), 0); 
                    stand_north ();
                    
                    enter_event evt;

                    evt.submap = submap ();
                    evt.x = posx ();
                    evt.y = posy ();
                    evt.c = this;
                    evt.dir = WALK_NORTH; 
                    event_handler::raise_event (&evt);
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
                    leave_event evt;

                    evt.submap = submap ();
                    evt.x = posx ();
                    evt.y = posy ();
                    evt.c = this;
                    evt.dir = WALK_SOUTH; 
                    event_handler::raise_event (&evt);

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
                        stand_south ();

                        enter_event evt; 
                        evt.submap = submap ();
                        evt.x = posx ();
                        evt.y = posy ();
                        evt.c = this;
                        evt.dir = WALK_SOUTH; 
                        event_handler::raise_event (&evt);
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
                    leave_event evt;

                    evt.submap = submap ();
                    evt.x = posx ();
                    evt.y = posy ();
                    evt.c = this;
                    evt.dir = WALK_WEST; 
                    event_handler::raise_event (&evt);

                    occupy (submap (), posx () - 1, posy ()); 
                }
                set_offset (offx () -1, offy ());  
                if (offx () == -MAPSQUARE_SIZE)
                {
                    leave (submap (), posx (), posy ());
                    leave (submap (), posx () - 1, posy ());
                    set_pos (submap (), posx () - 1, posy ());
                    set_offset (0, offy ());  
                    stand_west ();
                    
                    enter_event evt; 
                    evt.submap = submap ();
                    evt.x = posx ();
                    evt.y = posy ();
                    evt.c = this;
                    evt.dir = WALK_WEST; 
                    event_handler::raise_event (&evt);
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
                    leave_event evt;
                    
                    evt.submap = submap ();
                    evt.x = posx ();
                    evt.y = posy ();
                    evt.c = this;
                    evt.dir = WALK_EAST; 
                    event_handler::raise_event (&evt);

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
                        stand_east ();
                        
                        enter_event evt; 
                        evt.submap = submap ();
                        evt.x = posx ();
                        evt.y = posy ();
                        evt.c = this;
                        evt.dir = WALK_EAST; 
                        event_handler::raise_event (&evt);
                    }
                }
                break;
        }
    anim[current_move]->update ();     
}

void mapcharacter::speak (const string & text)
{
    if (saying)
        delete saying;

    string col; 
    switch (get_color ()) 
    { 
        case 1: col = "yellow"; break; 
        case 2: col = "red"; break; 
        case 3: col = "violet"; break; 
        case 4: col = "blue"; break; 
        case 5: col = "green"; break;
        default: col = "white"; break; 
    } 
    
    saying = new text_bubble (text, col, "original"); 
}
