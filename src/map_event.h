/*
   $Id$

   Copyright (C) 2002 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

/**
 * @file map_event.h
 *
 * @author Kai Sterker
 * @brief Declares the different map events.
 */

#ifndef MAP_EVENT_H__
#define MAP_EVENT_H__

#include "event.h"
#include "character.h"

/**
 * Baseclass for map enter/leave/action events.
 * The event will be launched if all the member's data matches.
 */ 
class map_event : public event
{
public:
    /**
     * Default constructor.
     */ 
    map_event ();

    /** 
     * Saves the basic %event %data (such as the type or script data)
     * to a file.
     * 
     * @param out file where to save the %event.
     */ 
    void put_state (ogzstream&) const;

    /** 
     * Loads the basic %event %date from a file.
     * 
     * @param in file to load the %event from.
     * @return \e true if the %event could be loaded, \e false otherwise
     */
    bool get_state (igzstream&);

    /**
     * Submap index (-1 for any).
     */
    s_int32 submap;

    /**
     * X position (-1 for any).
     */
    s_int32 x;

    /**
     * Y position (-1 for any).
     */
    s_int32 y;

    /**
     * Direction where the character is looking (-1 for any).
     */ 
    s_int8 dir;

    /**
     * Useless (for now).
     */ 
    s_int32 map;

    /**
     * Pointer to the mapcharacter that can launch this event (NULL for any).
     */ 
    mapcharacter *c;

protected:
    /**
     * Compare two map events for equality.
     *
     * @param evnt The map event to compare this to.
     * @return <b>True</b> if the two events equal, <b>false</b> otherwise.
     */
    bool equals (const event& evnt);

    /**
     * Executes the script associated with this map %event. If the
     * event does not repeat it is removed from the %event handler.
     *
     * @param evnt The %event that triggered this map %event.
     */
    void execute (const event& evnt);
};


/**
 * To notify when a character entered a mapsquare.
 */ 
class enter_event : public map_event
{
public:
    /**
     * Default constructor.
     */ 
    enter_event ();
};


/**
 * To notify when a mapcharacter left a mapsquare.
 */ 
class leave_event : public map_event
{
public:
    /**
     * Default constructor.
     */ 
    leave_event ();
};


/**
 * To notify when a mapcharacter "act" on a square.
 */ 
class action_event : public map_event
{
public:
    /**
     * Default constructor.
     */ 
    action_event ();
};

#endif // MAP_EVENT_H__
